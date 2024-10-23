#include "assembler.hpp"

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	FILE* listing = fopen("../data/listing.txt", "w");
	if (!listing)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	asm_status = AssemblerCtor(storage, assembler);
	ASM_ERROR_DEMO(asm_status);

	assembler->listing = listing;
	ListingHeader(assembler);

	for (size_t i = 0; i < storage->str_cnt; i++) {

		char command[MAX(MAX_COMMAND_LENGTH, MAX_LABEL_LENGTH)] = {};
		int cur_cmd_len = 0;
		int symbols_num = sscanf(storage->text[i].cur_str, "%s%n", command, &cur_cmd_len);
		if (symbols_num < 1)
			ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

		StringToLower(command);

		if (*command == '#')
			continue;

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

		fprintf(listing, "%." ALIGNMENT "zu\t", i);
		fprintf(listing, "%" ALIGNMENT "s\t", command);

		*(assembler->code + assembler->pc) |= opCode;
		MemoryUseCheck(&storage->text[i], assembler, cur_cmd_len);
		asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
		ASM_ERROR_DEMO(asm_status);

		fprintf(listing, "\n");
	}

	asm_status = AsmLabels(assembler);
	ASM_ERROR_DEMO(asm_status);

	assembler->header.code_size = assembler->pc;

	fprintf(listing, "Code size: %zu\n", assembler->header.code_size);

#ifdef ASM_DUMP
	asm_status = AsmDump(assembler, "");
	ASM_ERROR_DEMO(asm_status);
#endif

	if (fclose(listing))
		ASM_ERROR_DEMO(ASM_FILE_CLOSE_ERROR);

	return ASM_NO_ERROR;
}

AsmStatusCode AsmLabels(Assembler* assembler) {

	fprintf(assembler->listing, "Labels: \n");

	for (size_t i = 0; i < assembler->labels_table.label_size; i++) {
		fprintf(assembler->listing, "\t%" ALIGNMENT "s" " #%zu, addr %d\n",
				assembler->labels_table.labels[i].name, i + 1,
				assembler->labels_table.labels[i].addr);
	}
	fprintf(assembler->listing, "\n");

	fprintf(assembler->listing, "FixUps: \n");

	for (size_t i = 0; i < assembler->labels_table.fixup_size; i++) {
		*(assembler->code + assembler->labels_table.undef_labels[i].pc - 1) =
		assembler->labels_table.labels[assembler->labels_table.undef_labels[i].label_num - 1].addr;
		fprintf(assembler->listing, "\t%" ALIGNMENT "s" " #%zu, addr %d, fix %zu\n",
				assembler->labels_table.labels[assembler->labels_table.undef_labels[i].label_num - 1].name,
				assembler->labels_table.undef_labels[i].label_num,
				assembler->labels_table.labels[assembler->labels_table.undef_labels[i].label_num - 1].addr,
				assembler->labels_table.undef_labels[i].pc - 1);
	}
	fprintf(assembler->listing, "\n");

	return ASM_NO_ERROR;
}

AsmStatusCode MemoryUseCheck(String* string, Assembler* assembler, int cmd_len) {

	int brackets = 0;
	int open_br = 0;
	int close_br = 0;
	for (size_t i = (size_t)cmd_len + 1; i < string->cur_str_size; i++) {
		if (*(string->cur_str + i) == '[') {
			brackets++;
			open_br++;
			*(string->cur_str + i) = ' ';
		}
		if (*(string->cur_str + i) == ']') {
			brackets--;
			close_br++;
			*(string->cur_str + i) = ' ';
		}
		if (brackets < 0 || open_br > 1 || close_br > 1)
			return ASM_COMMAND_READ_ERROR;
	}

	if (open_br < 1 || close_br < 1)
		return ASM_COMMAND_READ_ERROR;

	*(assembler->code + assembler->pc) |= MASK_FOR_MEMORY;

	return ASM_NO_ERROR;
}

AsmStatusCode ListingHeader(Assembler* assembler) {

	FILE* lst = assembler->listing;

	fprintf(lst, "Signature: %s\n", (char*)&assembler->header.signature);
	fprintf(lst, "Version: %zu\n", assembler->header.code_version);

	LISTING_HEADER_PRINT(lst, "LINE");
	LISTING_HEADER_PRINT(lst, "CMD_NAME");
	LISTING_HEADER_PRINT(lst, "ARG_NAME");
	LISTING_HEADER_PRINT(lst, "CMD_CODE");
	LISTING_HEADER_PRINT(lst, "CMD_HEX");
	LISTING_HEADER_PRINT(lst, "ARG_CODE");
	LISTING_HEADER_PRINT(lst, "ARG_HEX");
	fprintf(lst, "\n");

	return ASM_NO_ERROR;
}

AsmStatusCode LabelCheck(Assembler* assembler, char* string) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	asm_status = FindCharInString(string, ':');
	if (asm_status != ASM_NO_ERROR)
		return ASM_UNDEFINE_LABEL;

	asm_status = IncreaseLabels(&assembler->labels_table);
	ASM_ERROR_DEMO(asm_status);

	int label_index = FindLabelInTable(assembler, string);
	if (label_index > -1) {
		assembler->labels_table.labels[label_index].addr = (int)assembler->pc;
		return ASM_NO_ERROR;
	}

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
	commands[CMD_JE] 	= "je";
	commands[CMD_CALL]	= "call";
	commands[CMD_RET] 	= "ret";
	commands[CMD_DRAW]  = "draw";
	commands[CMD_MOD] 	= "mod";

	for (size_t i = 0; i < COUNT_OF_COMMANDS; i++) {
		if (StrCmp(operation, commands[i]) == 0) {
			*opCode = (Commands)i;
			return ASM_NO_ERROR;
		}
	}

	return ASM_SYNTAX_COMMAND_ERROR;
}

AsmStatusCode AssemblerCtor(Storage* storage, Assembler* assembler) {

	assembler->header = { .signature = *(const long long*)SIGNATURE, .code_version = CODE_VERSION };

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

	FILE* lst = assembler->listing;

	switch (*(assembler->code + assembler->pc) & MASK_WITHOUT_MEMORY) {

		case CMD_PUSH: {
			if (!asm_status) asm_status = GetNumber(string, assembler, cmd_len);
			if (asm_status)  asm_status = GetRegister(string, assembler, cmd_len);
			break;
		}
		case CMD_POP: {
			size_t cmd = assembler->pc;
			if (!asm_status && *(assembler->code + cmd) & MASK_FOR_MEMORY)
				asm_status = GetNumber(string, assembler, cmd_len);

			if ((asm_status && *(assembler->code + cmd) & MASK_FOR_MEMORY) ||
			   (!asm_status && !(*(assembler->code + cmd) & MASK_FOR_MEMORY)))
			   	asm_status = GetRegister(string, assembler, cmd_len);
			break;
		}
		case CMD_CALL:
		case CMD_JE:
		case CMD_JB:
		case CMD_JMP: {
			if (!asm_status) asm_status = GetNumber(string, assembler, cmd_len);
			if (asm_status)  asm_status = GetLabel(string, assembler, cmd_len);
			break;
		}
		default: {
			size_t cmd_pc = assembler->pc;
			fprintf(lst, "%" ALIGNMENT "s", "--------\t");
			fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + cmd_pc));
			fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + cmd_pc));
			fprintf(lst, "%" ALIGNMENT "s", "--------\t");
			fprintf(lst, "%" ALIGNMENT "s", "--------\t");
			assembler->pc++;
			break;
		}

	}

	ASM_ERROR_DEMO(asm_status);

	return ASM_NO_ERROR;
}

AsmStatusCode GetNumber(String* string, Assembler* assembler, int cmd_len) {

	FILE* lst = assembler->listing;

	while(*(string->cur_str + cmd_len) == ' ')
		cmd_len++;
	for (int i = cmd_len; *(string->cur_str + i) != '\0'; i++) {
		if (*(string->cur_str + i) == ' ') {
			*(string->cur_str + i) = '\0';
			break;
		}
	}

	int num = 0;
	int symbols_num = sscanf(string->cur_str + cmd_len, "%d", &num);
	if (symbols_num < 1)
		return ASM_COMMAND_READ_ERROR;

	size_t cmd_pc = assembler->pc;

	*(assembler->code + assembler->pc++) |= MASK_FOR_NUMBER;
	*(assembler->code + assembler->pc++) = num;

	if (*(assembler->code + cmd_pc) & MASK_FOR_MEMORY)
		fprintf(lst, "[%" "6" "d]\t", *(assembler->code + assembler->pc - 1));
	else
		fprintf(lst, "%" ALIGNMENT "d\t", *(assembler->code + assembler->pc - 1));
	fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + cmd_pc));
	fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + cmd_pc));
	fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + assembler->pc - 1));
	fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + assembler->pc - 1));

	return ASM_NO_ERROR;
}

AsmStatusCode GetRegister(String* string, Assembler* assembler, int cmd_len) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	FILE* lst = assembler->listing;

	char* reg_addr = NULL;
	size_t reg_len = 0;
	for (size_t i = (size_t)cmd_len; *(string->cur_str + i) != '\0'; i++) {
		if (*(string->cur_str + i) == ' ') {
			if (reg_len > 0) {
				*(string->cur_str + i) = '\0';
				break;
			}
			reg_addr = (string->cur_str + i + 1);
		}
		else
			reg_len++;
	}

	if (reg_len > REGISTER_NAME_LENGTH)
		ASM_ERROR_DEMO(ASM_BIG_NAME_FOR_REGISTER);

	StringToLower(reg_addr);
	asm_status = FindCharInString(reg_addr, 'x');
	ASM_ERROR_DEMO(asm_status);

	size_t cmd_pc = assembler->pc;

	*(assembler->code + assembler->pc++) |= MASK_FOR_REGISTER;

	if ((size_t)(*reg_addr - 'a' + 1) > MAX_REG_AMOUNT - 1 && *(reg_addr) != 'x')
		ASM_ERROR_DEMO(ASM_WRONG_LETTER_IN_REG_NAME);

	*(assembler->code + assembler->pc++) = (*(reg_addr) == 'x' ? 0 : *reg_addr - 'a' + 1);

	if (*(assembler->code + cmd_pc) & MASK_FOR_MEMORY)
		fprintf(lst, "[%" "6" "s]\t", reg_addr);
	else
		fprintf(lst, "%" ALIGNMENT "s\t", reg_addr);
	fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + cmd_pc));
	fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + cmd_pc));
	fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + assembler->pc - 1));
	fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + assembler->pc - 1));

	return ASM_NO_ERROR;
}

AsmStatusCode GetLabel(String* string, Assembler* assembler, int cmd_len) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	FILE* lst = assembler->listing;

	char label[MAX_LABEL_LENGTH + 1] = {};
	int cur_label_len = 0;
	int symbols_num = sscanf(string->cur_str + cmd_len, "%s%n", label, &cur_label_len);
	if (symbols_num < 1)
		ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

	if (cur_label_len > (int)MAX_LABEL_LENGTH)
		ASM_ERROR_DEMO(ASM_BIG_NAME_FOR_LABEL);

	StringToLower(label);

	asm_status = IncreaseLabels(&assembler->labels_table);
	ASM_ERROR_DEMO(asm_status);

	size_t cmd_pc = assembler->pc;

	*(assembler->code + assembler->pc++) |= MASK_FOR_NUMBER;

	asm_status = LabelStatus(assembler, label);

	fprintf(lst, "%" ALIGNMENT "s\t", label);
	fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + cmd_pc));
	fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + cmd_pc));

	if (*(assembler->code + assembler->pc - 1) != -1) {
		fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + assembler->pc - 1));
		fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + assembler->pc - 1));
	}
	else {
		fprintf(lst, "%" ALIGNMENT "s", "????????\t");
		fprintf(lst, "%" ALIGNMENT "s", "????????\t");
	}

	return ASM_NO_ERROR;
}

AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out) {

	FILE* bin = fopen(file_out, "wb");
	if (!bin)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	size_t written_check = fwrite(&assembler->header, sizeof(char), sizeof(McHeader), bin);
	if (written_check != sizeof(McHeader))
		ASM_ERROR_DEMO(ASM_FILE_WRITE_ERROR);

	written_check = fwrite(assembler->code, sizeof(int), assembler->pc, bin);
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

	int label_index = FindLabelInTable(assembler, label);
	if (label_index > -1 && labels_table->labels[label_index].addr != -1) {
		*(assembler->code + assembler->pc++) = labels_table->labels[label_index].addr;
		return ASM_NO_ERROR;
	}

	if (label_index < 0) {
		for (size_t i = 0; *(label + i) != '\0'; i++)
			labels_table->labels[labels_table->label_size].name[i] = label[i];
		label_index = (int)labels_table->label_size++;
	}
	*(assembler->code + assembler->pc++) = labels_table->labels[label_index].addr = -1;

	FixUp* cur_fix_up = &labels_table->undef_labels[labels_table->fixup_size++];

	cur_fix_up->label_num = (size_t)label_index + 1;
	cur_fix_up->pc = assembler->pc;

	return ASM_UNDEFINE_LABEL;
}

int FindLabelInTable(Assembler* assembler, char* label) {

	for (int i = 0; i < (int)assembler->labels_table.label_size; i++) {
		if (StrCmp(assembler->labels_table.labels[i].name, label) == 0)
			return i;
	}

	return -1;
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
		printf("label #%zu: name - %" ALIGNMENT "s, addr - %d\n", i + 1, assembler->labels_table.labels[i].name,
																		 assembler->labels_table.labels[i].addr);
	printf("\n");

	printf("\nFixUp:\n");
	for (size_t i = 0; i < assembler->labels_table.fixup_size; i++)
		printf("label #%zu: addr - %zu, num - %zu\n", i + 1, assembler->labels_table.undef_labels[i].pc,
															 assembler->labels_table.undef_labels[i].label_num);
	printf("\n");

	for (size_t i = 0; i < 5 * assembler->pc; i++)
		printf("-");
	printf("\n");

	getchar();

	return ASM_NO_ERROR;
}
