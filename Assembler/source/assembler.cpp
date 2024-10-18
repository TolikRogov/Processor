#include "assembler.hpp"

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	assembler->labels_table.labels = (Label*)calloc(assembler->labels_table.capacity, sizeof(Label));
	if (!assembler->labels_table.labels)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	assembler->labels_table.undef_labels = (FixUp*)calloc(assembler->labels_table.capacity, sizeof(FixUp));
	if (!assembler->labels_table.undef_labels)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	assembler->code = (int*)calloc(storage->str_cnt * 2, sizeof(int));
	if (!assembler->code)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

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

		asm_status = FindCharInString(command, ':');
		if (asm_status == ASM_NO_ERROR) {
			asm_status = IncreaseLabels(assembler);
			ASM_ERROR_DEMO(asm_status);

			asm_status = FindLabelInTable(assembler, command);
			if (asm_status == ASM_NO_ERROR)
				continue;

			for (size_t j = 0; *(command + j) != '\0'; j++)
				assembler->labels_table.labels[assembler->labels_table.label_size].name[j] = command[j];
			assembler->labels_table.labels[assembler->labels_table.label_size++].addr = assembler->pc;
			continue;
		}

		// Func GetCommand(char*) {return EnumCode}
		if (StrCmp(command, "push") == 0) {
			*(assembler->code + assembler->pc) |= CMD_PUSH;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "pop") == 0) {
			*(assembler->code + assembler->pc) |= CMD_POP;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "hlt") == 0) {
			*(assembler->code + assembler->pc) |= CMD_HLT;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "add") == 0) {
			*(assembler->code + assembler->pc) |= CMD_ADD;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "sub") == 0) {
			*(assembler->code + assembler->pc) |= CMD_SUB;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "div") == 0) {
			*(assembler->code + assembler->pc) |= CMD_DIV;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "out") == 0) {
			*(assembler->code + assembler->pc) |= CMD_OUT;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "mul") == 0) {
			*(assembler->code + assembler->pc) |= CMD_MUL;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "in") == 0) {
			*(assembler->code + assembler->pc) |= CMD_IN;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "sqrt") == 0) {
			*(assembler->code + assembler->pc) |= CMD_SQRT;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "sin") == 0) {
			*(assembler->code + assembler->pc) |= CMD_SIN;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "cos") == 0) {
			*(assembler->code + assembler->pc) |= CMD_COS;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "jb") == 0) {
			*(assembler->code + assembler->pc) |= CMD_JB;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}
		if (StrCmp(command, "jmp") == 0) {
			*(assembler->code + assembler->pc) |= CMD_JMP;
			asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
			ASM_ERROR_DEMO(asm_status);
			continue;
		}

		ASM_ERROR_DEMO(ASM_SYNTAX_COMMAND_ERROR);

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

AsmStatusCode GetArgs(String* string, Assembler* assembler, int cmd_len) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	switch (*(assembler->code + assembler->pc)) {

		case CMD_PUSH: {
			if (!asm_status)  asm_status = GetNumber(string, assembler, cmd_len);
			if (asm_status)   asm_status = GetRegister(string, assembler, cmd_len);
			break;
		}
		case CMD_JB: {
			asm_status = GetLabel(string, assembler, cmd_len);
			break;
		}
		case CMD_JMP: {
			asm_status = GetLabel(string, assembler, cmd_len);
			break;
		}
		case CMD_POP: {
			asm_status = GetRegister(string, assembler, cmd_len);
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

											// TODO: Enum Args {}
	*(assembler->code + assembler->pc++) |= (1 << BIT_FOR_NUMBER);
	*(assembler->code + assembler->pc++) = num;

	return ASM_NO_ERROR;
}

AsmStatusCode GetRegister(String* string, Assembler* assembler, int cmd_len) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	if (string->cur_str_size > MAX_COMMAND_LENGTH + REGISTER_NAME_LENGTH)
		ASM_ERROR_DEMO(ASM_BIG_NAME_FOR_REGISTER);

	// Res_Name_len = 3;
	char reg[REGISTER_NAME_LENGTH + 1] = {};
	int symbols_num = sscanf(string->cur_str + cmd_len, "%s", reg);
	if (symbols_num < 1)
		ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

	StringToLower(reg);

	asm_status = FindCharInString(reg, 'x');
	ASM_ERROR_DEMO(asm_status);

	*(assembler->code + assembler->pc++) |= (1 << BIT_FOR_REGISTER);

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

	asm_status = IncreaseLabels(assembler);
	ASM_ERROR_DEMO(asm_status);

	*(assembler->code + assembler->pc++) |= (1 << BIT_FOR_NUMBER);

	asm_status = LabelStatus(assembler, label);

	return ASM_NO_ERROR;
}

AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out) {

	FILE* output = fopen(file_out, "w");
	if (!output)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	fprintf(output, "%s\n", assembler->header.signature);
	fprintf(output, "%zu\n", assembler->header.code_version);
	fprintf(output, "%zu\n", assembler->pc);

	// TODO: fwrite(buffer, file)
	for (size_t i = 0; i < assembler->pc; i++) {
		int cmd = *(assembler->code + i) & 0x1F;

		fprintf(output, "0x%.2x", *(assembler->code + i));

		if (cmd == CMD_PUSH  ||
			cmd == CMD_POP	 ||
			cmd == CMD_JB	 ||
			cmd == CMD_JMP)
			fprintf(output, " 0x%.2x", *(assembler->code + i++ + 1));

		fprintf(output, "\n");
	}

	if (fclose(output))
		ASM_ERROR_DEMO(ASM_FILE_CLOSE_ERROR);

	return ASM_NO_ERROR;
}

// TODO: In other file
AsmStatusCode IncreaseLabels(Assembler* assembler) {

	if (assembler->labels_table.capacity == assembler->labels_table.label_size ||
		assembler->labels_table.capacity == assembler->labels_table.fixup_size) {
		assembler->labels_table.labels = (Label*)realloc(assembler->labels_table.labels,
														(assembler->labels_table.capacity *= 2) * sizeof(Label));
		if (!assembler->labels_table.labels)
			ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

		assembler->labels_table.undef_labels = (FixUp*)realloc(assembler->labels_table.undef_labels,
															   assembler->labels_table.capacity * sizeof(FixUp));
		if (!assembler->labels_table.undef_labels)
			ASM_ERROR_DEMO(ASM_ALLOC_ERROR);
	}

	return ASM_NO_ERROR;
}

AsmStatusCode LabelStatus(Assembler* assembler, char* label) {

	for (size_t i = 0; i < assembler->labels_table.label_size; i++) {
		if (StrCmp(assembler->labels_table.labels[i].name, label) == 0) {
			*(assembler->code + assembler->pc++) = (int)assembler->labels_table.labels[i].addr;
			return ASM_NO_ERROR;
		}
	}

	for (size_t i = 0; *(label + i) != '\0'; i++)
		assembler->labels_table.labels[assembler->labels_table.label_size].name[i] = label[i];
	assembler->labels_table.labels[assembler->labels_table.label_size].addr = 666;
	*(assembler->code + assembler->pc++) = (int)assembler->labels_table.labels[assembler->labels_table.label_size++].addr;

	assembler->labels_table.undef_labels[assembler->labels_table.fixup_size].label_num = assembler->labels_table.label_size;
	assembler->labels_table.undef_labels[assembler->labels_table.fixup_size++].pc = assembler->pc;

	return ASM_UNDEFINE_LABEL;
}

AsmStatusCode FindLabelInTable(Assembler* assembler, char* label) {

	for (size_t j = 0; j < assembler->labels_table.label_size; j++) {
		if (StrCmp(assembler->labels_table.labels[j].name, label) == 0) {
			assembler->labels_table.labels[j].addr = assembler->pc;
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
		printf("label #%zu: name - %s, addr - %zu\n", i, assembler->labels_table.labels[i].name,
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
