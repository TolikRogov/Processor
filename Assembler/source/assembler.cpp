#include "assembler.hpp"

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	asm_status = AssemblerCtor(storage, assembler);
	ASM_ERROR_DEMO(asm_status);

	for (size_t i = 0; i < storage->str_cnt; i++) {

		char command[MAX(MAX_COMMAND_LENGTH, MAX_LABEL_LENGTH)] = {};
		int cur_cmd_len = 0;
		int symbols_num = sscanf(storage->text[i].cur_str, "%s%n", command, &cur_cmd_len);
		if (symbols_num < 1)
			ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

		StringToLower(command);

#ifdef ASM_DUMP
		asm_status = AsmDump(assembler, storage->text[i].cur_str);
		ASM_ERROR_DEMO(asm_status);
#endif

		asm_status = LabelCheck(assembler, command);
		if (asm_status == ASM_NO_ERROR)
			continue;

		Commands opCode = (Commands)0;
		asm_status = GetCommand(command, &opCode);
		if (asm_status == ASM_SYNTAX_COMMAND_ERROR)
			ASM_ERROR_DEMO(asm_status);

		*(assembler->code + assembler->pc) |= opCode;
		asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
		ASM_ERROR_DEMO(asm_status);
	}

	for (size_t i = 0; i < assembler->labels_table.fixup_size; i++)
		*(assembler->code + assembler->labels_table.undef_labels[i].pc - 1) =
		(int)assembler->labels_table.labels[assembler->labels_table.undef_labels[i].label_num - 1].addr;

#ifdef ASM_DUMP
	asm_status = AsmDump(assembler, "");
	ASM_ERROR_DEMO(asm_status);
#endif

	return ASM_NO_ERROR;
}

AsmStatusCode LabelCheck(Assembler* assembler, char* string) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	asm_status = FindCharInString(string, ':');
	if (asm_status != ASM_NO_ERROR)
		return ASM_UNDEFINE_LABEL;

	asm_status = IncreaseLabels(&assembler->labels_table);
	ASM_ERROR_DEMO(asm_status);

	asm_status = FindLabelInTable(assembler, string);
	if (asm_status == ASM_NO_ERROR)
		return ASM_NO_ERROR;

	for (size_t j = 0; *(string + j) != '\0'; j++)
		assembler->labels_table.labels[assembler->labels_table.label_size].name[j] = string[j];
	assembler->labels_table.labels[assembler->labels_table.label_size++].addr = (int)assembler->pc;

	return ASM_NO_ERROR;
}

AsmStatusCode GetCommand(const char* operation, Commands* opCode) {

	static const char* commands[COUNT_OF_COMMANDS] = {};
	commands[CMD_PUSH] 	= "push";
	commands[CMD_POP] 	= "pop";
	commands[CMD_HLT]	= "hlt";
	commands[CMD_ADD] 	= "add";
	commands[CMD_SUB] 	= "sub";
	commands[CMD_MUL] 	= "mul";
	commands[CMD_DIV] 	= "div";
	commands[CMD_OUT] 	= "out";
	commands[CMD_IN] 	= "in";
	commands[CMD_SQRT] 	= "sqrt";
	commands[CMD_SIN] 	= "sin";
	commands[CMD_COS] 	= "cos";
	commands[CMD_JB] 	= "jb";
	commands[CMD_JMP] 	= "jmp";

	for (size_t i = 0; i < COUNT_OF_COMMANDS; i++) {
		if (StrCmp(operation, commands[i]) == 0) {
			*opCode = (Commands)i;
			return ASM_NO_ERROR;
		}
	}

	return ASM_SYNTAX_COMMAND_ERROR;
}

AsmStatusCode AssemblerCtor(Storage* storage, Assembler* assembler) {

	assembler->header = { .signature = *(const long double*)SIGNATURE, .code_size = CODE_VERSION };

	assembler->labels_table.labels = (Label*)calloc(assembler->labels_table.capacity, sizeof(Label));
	if (!assembler->labels_table.labels)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	assembler->labels_table.undef_labels = (FixUp*)calloc(assembler->labels_table.capacity, sizeof(FixUp));
	if (!assembler->labels_table.undef_labels)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	assembler->code = (int*)calloc(storage->str_cnt * 2, sizeof(int));
	if (!assembler->code)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	return ASM_NO_ERROR;
};

AsmStatusCode GetArgs(String* string, Assembler* assembler, int cmd_len) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	switch (*(assembler->code + assembler->pc)) {

		case CMD_PUSH: {
			if (!asm_status) asm_status = GetNumber(string, assembler, cmd_len);
			if (asm_status)  asm_status = GetRegister(string, assembler, cmd_len);
			break;
		}
		case CMD_POP: {
			asm_status = GetRegister(string, assembler, cmd_len);
			break;
		}
		case CMD_JB:
		case CMD_JMP: {
			if (!asm_status) asm_status = GetNumber(string, assembler, cmd_len);
			if (asm_status)  asm_status = GetLabel(string, assembler, cmd_len);
			break;
		}
		default: {
			assembler->pc++;
			break;
		}

	}

	ASM_ERROR_DEMO(asm_status);

	return ASM_NO_ERROR;
}

AsmStatusCode GetNumber(String* string, Assembler* assembler, int cmd_len) {

	int num = 0;
	int symbols_num = sscanf(string->cur_str + cmd_len, "%d", &num);
	if (symbols_num < 1)
		return ASM_COMMAND_READ_ERROR;

	*(assembler->code + assembler->pc++) |= BIT_FOR_NUMBER;
	*(assembler->code + assembler->pc++) = num;

	return ASM_NO_ERROR;
}

AsmStatusCode GetRegister(String* string, Assembler* assembler, int cmd_len) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	if (string->cur_str_size > MAX_COMMAND_LENGTH + REGISTER_NAME_LENGTH)
		ASM_ERROR_DEMO(ASM_BIG_NAME_FOR_REGISTER);

	char reg[REGISTER_NAME_LENGTH + 1] = {};
	int symbols_num = sscanf(string->cur_str + cmd_len, "%s", reg);
	if (symbols_num < 1)
		ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

	StringToLower(reg);

	asm_status = FindCharInString(reg, 'x');
	ASM_ERROR_DEMO(asm_status);

	*(assembler->code + assembler->pc++) |= BIT_FOR_REGISTER;

	if ((size_t)(*reg - 'a' + 1) > MAX_REG_AMOUNT - 1 && *(reg) != 'x')
		ASM_ERROR_DEMO(ASM_WRONG_LETTER_IN_REG_NAME);

	*(assembler->code + assembler->pc++) = (*(reg) == 'x' ? 0 : *reg - 'a' + 1);

	return ASM_NO_ERROR;
}

AsmStatusCode GetLabel(String* string, Assembler* assembler, int cmd_len) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	if (string->cur_str_size > MAX_COMMAND_LENGTH + MAX_LABEL_LENGTH)
		ASM_ERROR_DEMO(ASM_BIG_NAME_FOR_LABEL);

	char label[MAX_LABEL_LENGTH + 1] = {};
	int symbols_num = sscanf(string->cur_str + cmd_len, "%s", label);
	if (symbols_num < 1)
		ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

	StringToLower(label);

	asm_status = IncreaseLabels(&assembler->labels_table);
	ASM_ERROR_DEMO(asm_status);

	*(assembler->code + assembler->pc++) |= BIT_FOR_NUMBER;

	asm_status = LabelStatus(assembler, label);

	return ASM_NO_ERROR;
}

AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out) {

	FILE* bin = fopen(file_out, "wb");
	if (!bin)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	assembler->header.code_size = assembler->pc;

	size_t written_check = fwrite(&assembler->header, sizeof(char), sizeof(McHeader), bin);
	if (written_check != sizeof(McHeader))
		ASM_ERROR_DEMO(ASM_FILE_WRITE_ERROR);

	written_check = fwrite(&assembler->code, sizeof(char), assembler->pc, bin);
	if (written_check != assembler->pc)
		ASM_ERROR_DEMO(ASM_FILE_WRITE_ERROR);

	if (fclose(bin))
		ASM_ERROR_DEMO(ASM_FILE_CLOSE_ERROR);

	return ASM_NO_ERROR;
}

AsmStatusCode IncreaseLabels(LabelsTable* labels_table) {

	if (labels_table->capacity == labels_table->label_size ||
		labels_table->capacity == labels_table->fixup_size) {
		labels_table->labels = (Label*)realloc(labels_table->labels,
											  (labels_table->capacity *= 2) * sizeof(Label));
		if (!labels_table->labels)
			ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

		labels_table->undef_labels = (FixUp*)realloc(labels_table->undef_labels,
													 labels_table->capacity * sizeof(FixUp));
		if (!labels_table->undef_labels)
			ASM_ERROR_DEMO(ASM_ALLOC_ERROR);
	}

	return ASM_NO_ERROR;
}

AsmStatusCode LabelStatus(Assembler* assembler, char* label) {

	LabelsTable* labels_table = &assembler->labels_table;

	for (size_t i = 0; i < labels_table->label_size; i++) {
		if (StrCmp(labels_table->labels[i].name, label) == 0) {
			*(assembler->code + assembler->pc++) = labels_table->labels[i].addr;
			return ASM_NO_ERROR;
		}
	}

	for (size_t i = 0; *(label + i) != '\0'; i++)
		labels_table->labels[labels_table->label_size].name[i] = label[i];
	*(assembler->code + assembler->pc++) = labels_table->labels[labels_table->label_size++].addr = -1;

	FixUp* cur_fix_up = &labels_table->undef_labels[labels_table->fixup_size++];

	cur_fix_up->label_num = labels_table->label_size;
	cur_fix_up->pc = assembler->pc;

	return ASM_UNDEFINE_LABEL;
}

AsmStatusCode FindLabelInTable(Assembler* assembler, char* label) {

	for (size_t j = 0; j < assembler->labels_table.label_size; j++) {
		if (StrCmp(assembler->labels_table.labels[j].name, label) == 0) {
			assembler->labels_table.labels[j].addr = (int)assembler->pc;
			return ASM_NO_ERROR;
		}
	}

	return ASM_UNDEFINE_LABEL;
}

AsmStatusCode AsmDump(Assembler* assembler, const char* string) {

	for (size_t i = 0; i < 5 * assembler->pc; i++)
		printf("-");
	printf("\n");

	for (size_t i = 0; i < assembler->pc; i++)
		printf("%.4zu ", i);
	printf("\n");

	for (size_t i = 0; i < assembler->pc; i++)
		printf("0x%.2x ", *(assembler->code + i));
	printf("\n");

	for (size_t i = 0; i < assembler->pc; i++)
		printf("     ");
	printf(YELLOW("^")"\n");

	printf("pc: %zu\n", assembler->pc);
	printf("current cmd string: %s\n", string);

	printf("\nLabels:\n");
	for (size_t i = 0; i < assembler->labels_table.label_size; i++)
		printf("label #%zu: name - %s, addr - %d\n", i, assembler->labels_table.labels[i].name,
														assembler->labels_table.labels[i].addr);
	printf("\n");

	printf("\nFixUp:\n");
	for (size_t i = 0; i < assembler->labels_table.fixup_size; i++)
		printf("label #%zu: addr - %zu, num - %zu\n", i, assembler->labels_table.undef_labels[i].pc,
														 assembler->labels_table.undef_labels[i].label_num);
	printf("\n");

	for (size_t i = 0; i < 5 * assembler->pc; i++)
		printf("-");
	printf("\n");

	getchar();

	return ASM_NO_ERROR;
}
