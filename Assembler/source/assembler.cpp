#include "assembler.hpp"

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	assembler->code = (int*)calloc(storage->str_cnt * 2, sizeof(int));
	if (!assembler->code)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	for (size_t i = 0; i < storage->str_cnt; i++) {

		char command[MAX_COMMAND_LENGTH] = {};
		int cur_cmd_len = 0;
		int symbols_num = sscanf(storage->text[i].cur_str, "%s%n", command, &cur_cmd_len);
		if (symbols_num < 1)
			ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

		StringToLower(command);

		if (StrCmp(command, "push") == 0) {
			*(assembler->code + assembler->pc) = CMD_PUSH;
			assembler->pc++;

			int num = 0;
			symbols_num = sscanf(storage->text[i].cur_str + cur_cmd_len, "%d", &num);
			if (symbols_num < 1)
				ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

			*(assembler->code + assembler->pc) = num;
			assembler->pc++;

			continue;
		}
		if (StrCmp(command, "pushr") == 0) {

			if (storage->text[i].cur_str_size > MAX_COMMAND_LENGTH + REGISTER_NAME_LENGTH)
				ASM_ERROR_DEMO(ASM_BIG_NAME_FOR_REGISTER);

			*(assembler->code + assembler->pc) = CMD_PUSHR;
			assembler->pc++;

			char reg[REGISTER_NAME_LENGTH] = {};
			symbols_num = sscanf(storage->text[i].cur_str + cur_cmd_len, "%s", reg);
			if (symbols_num < 1)
				ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

			StringToLower(reg);

			asm_status = FindXinRegister(reg);
			ASM_ERROR_DEMO(asm_status);

			*(assembler->code + assembler->pc) = *reg - 'a' + 1;
			assembler->pc++;

			continue;
		}
		if (StrCmp(command, "pop") == 0) {
			if (storage->text[i].cur_str_size > MAX_COMMAND_LENGTH + REGISTER_NAME_LENGTH)
				ASM_ERROR_DEMO(ASM_BIG_NAME_FOR_REGISTER);

			*(assembler->code + assembler->pc) = CMD_POP;
			assembler->pc++;

			char reg[REGISTER_NAME_LENGTH] = {};
			symbols_num = sscanf(storage->text[i].cur_str + cur_cmd_len, "%s", reg);
			if (symbols_num < 1)
				ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

			StringToLower(reg);

			asm_status = FindXinRegister(reg);
			ASM_ERROR_DEMO(asm_status);

			*(assembler->code + assembler->pc) = *reg - 'a' + 1;
			assembler->pc++;

			continue;
		}
		if (StrCmp(command, "hlt") == 0) {
			*(assembler->code + assembler->pc) = CMD_HLT;
			assembler->pc++;
			continue;
		}
		if (StrCmp(command, "add") == 0) {
			*(assembler->code + assembler->pc) = CMD_ADD;
			assembler->pc++;
			continue;
		}
		if (StrCmp(command, "sub") == 0) {
			*(assembler->code + assembler->pc) = CMD_SUB;
			assembler->pc++;
			continue;
		}
		if (StrCmp(command, "div") == 0) {
			*(assembler->code + assembler->pc) = CMD_DIV;
			assembler->pc++;
			continue;
		}
		if (StrCmp(command, "out") == 0) {
			*(assembler->code + assembler->pc) = CMD_OUT;
			assembler->pc++;
			continue;
		}
		if (StrCmp(command, "mul") == 0) {
			*(assembler->code + assembler->pc) = CMD_MUL;
			assembler->pc++;
			continue;
		}
		if (StrCmp(command, "in") == 0) {
			*(assembler->code + assembler->pc) = CMD_IN;
			assembler->pc++;
			continue;
		}
		if (StrCmp(command, "sqrt") == 0) {
			*(assembler->code + assembler->pc) = CMD_SQRT;
			assembler->pc++;
			continue;
		}
		if (StrCmp(command, "sin") == 0) {
			*(assembler->code + assembler->pc) = CMD_SIN;
			assembler->pc++;
			continue;
		}
		if (StrCmp(command, "cos") == 0) {
			*(assembler->code + assembler->pc) = CMD_COS;
			assembler->pc++;
			continue;
		}

		ASM_ERROR_DEMO(ASM_SYNTAX_COMMAND_ERROR);

	}

	return ASM_NO_ERROR;
}

AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out) {

	FILE* output = fopen(file_out, "w");
	if (!output)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	fprintf(output, "%s\n", SIGNATURE);
	fprintf(output, "%zu\n", CODE_VERSION);
	fprintf(output, "%zu\n", assembler->pc);

	for (size_t i = 0; i < assembler->pc; i++) {
		int cmd = *(assembler->code + i);

		fprintf(output, "0x%.2x", cmd);

		if (cmd == CMD_PUSH  ||
			cmd == CMD_PUSHR ||
			cmd == CMD_POP)
			fprintf(output, " 0x%.2x", *(assembler->code + i++ + 1));

		fprintf(output, "\n");
	}

	if (fclose(output))
		ASM_ERROR_DEMO(ASM_FILE_CLOSE_ERROR);

	return ASM_NO_ERROR;
}
