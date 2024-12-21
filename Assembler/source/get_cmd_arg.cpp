#include "get_cmd_arg.hpp"
#include "labels.hpp"

AsmStatusCode GetCommand(const char* operation, Commands* opCode) {

	for (size_t i = 0; i < COUNT_OF_COMMANDS; i++) {
		if (StrCmp(operation, array_commands[i].cmd_name) == 0) {
			*opCode = array_commands[i].cmd_code;
			return ASM_NO_ERROR;
		}
	}

	return ASM_SYNTAX_COMMAND_ERROR;
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

AsmStatusCode GetArgs(String* string, Assembler* assembler, int cmd_len) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	FILE* lst = assembler->listing;

	switch (*(assembler->code + assembler->pc) & MASK_WITHOUT_MEMORY) {

		case CMD_PUSH: {
			if (!asm_status) asm_status = GetRegisterPlusNumber(string, assembler, cmd_len);
			if (asm_status)  asm_status = GetNumber(string, assembler, cmd_len);
			if (asm_status)  asm_status = GetRegister(string, assembler, cmd_len);
			break;
		}
		case CMD_POP: {
			size_t cmd = assembler->pc;
			if (*(assembler->code + cmd) & MASK_FOR_MEMORY) {
				if (!asm_status) asm_status = GetRegisterPlusNumber(string, assembler, cmd_len);
				if (asm_status)  asm_status = GetNumber(string, assembler, cmd_len);
				if (asm_status)  asm_status = GetRegister(string, assembler, cmd_len);
				break;
			}

			if (!(*(assembler->code + cmd) & MASK_FOR_MEMORY))
			   	asm_status = GetRegister(string, assembler, cmd_len);
			break;
		}
		case CMD_CALL:
		case CMD_JE:
		case CMD_JA:
		case CMD_JAE:
		case CMD_JNE:
		case CMD_JB:
		case CMD_JBE:
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
			assembler->pc += sizeof(Command_t);
			break;
		}

	}

	ASM_ERROR_DEMO(asm_status);

	return ASM_NO_ERROR;
}

AsmStatusCode GetRegisterPlusNumber(String* string, Assembler* assembler, int cmd_len) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	while(*(string->cur_str + cmd_len) == ' ')
		cmd_len++;
	for (int i = cmd_len; *(string->cur_str + i) != '\0'; i++) {
		if (*(string->cur_str + i) == ' ') {
			*(string->cur_str + i) = '\0';
			break;
		}
	}

	char argument[MAX_REG_PLUS_NUMBER_ARG] = {};
	int symbols_num = sscanf(string->cur_str + cmd_len, "%s", argument);
	if (symbols_num < 1)
		return ASM_COMMAND_READ_ERROR;

	asm_status = FindAndReplaceCharInString(string->cur_str + cmd_len, '+', ' ', NULL);
	if (asm_status)
		return ASM_NO_CHAR_IN_STRING;

	if (*(assembler->code + assembler->pc) & MASK_FOR_MEMORY)
		fprintf(assembler->listing, "[%" "6" "s]\t", argument);
	else
		fprintf(assembler->listing, "%" ALIGNMENT "s\t", argument);

	size_t cmd_pc = assembler->pc;

	if (fclose(assembler->listing))
		ASM_ERROR_DEMO(ASM_FILE_CLOSE_ERROR);

	asm_status = GetRegister(string, assembler, cmd_len);

	for (int i = cmd_len, null_cnt = 0; null_cnt < 1; i++) {
		null_cnt += (*(string->cur_str + i) == '\0' ? 1 : 0);
		cmd_len = (int)i + 1;
	}
	assembler->pc -= (sizeof(Command_t) + sizeof(Register_t));

	asm_status = GetNumber(string, assembler, cmd_len);

	assembler->listing = fopen(assembler->files.listing_file, "a");
	if (!assembler->listing)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	fprintf(assembler->listing, "%." ALIGNMENT "d\t", *(assembler->code + cmd_pc));
	fprintf(assembler->listing, "%." ALIGNMENT "x\t", *(assembler->code + cmd_pc));

	fprintf(assembler->listing, "%." ALIGNMENT "d\t", *(assembler->code + assembler->pc - sizeof(Immediate_t) - sizeof(Register_t)));
	fprintf(assembler->listing, "%." ALIGNMENT "x\t", *(assembler->code + assembler->pc - sizeof(Immediate_t) - sizeof(Register_t)));

	fprintf(assembler->listing, "%." ALIGNMENT "lg\t", *(Immediate_t*)(assembler->code + assembler->pc - sizeof(Immediate_t)));
	fprintf(assembler->listing, "%." ALIGNMENT "f\t",  *(Immediate_t*)(assembler->code + assembler->pc - sizeof(Immediate_t)));

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

	double num = 0;
	int symbols_num = sscanf(string->cur_str + cmd_len, "%lg", &num);
	if (symbols_num < 1)
		return ASM_COMMAND_READ_ERROR;

	size_t cmd_pc = assembler->pc;

	*(assembler->code + assembler->pc) |= MASK_FOR_NUMBER;
	assembler->pc += sizeof(Command_t);
	if (*(string->cur_str + cmd_len - 1) == '\0')
		assembler->pc += sizeof(Register_t);
	*(Immediate_t*)(assembler->code + assembler->pc) = num;
	assembler->pc += sizeof(Immediate_t);

	if (*(assembler->code + cmd_pc) & MASK_FOR_MEMORY)
		fprintf(lst, "[%" "6" "lg]\t", *(Immediate_t*)(assembler->code + assembler->pc - sizeof(Immediate_t)));
	else
		fprintf(lst, "%" ALIGNMENT "lg\t", *(Immediate_t*)(assembler->code + assembler->pc - sizeof(Immediate_t)));

	fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + cmd_pc));
	fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + cmd_pc));

	fprintf(lst, "%" ALIGNMENT "lg\t", *(Immediate_t*)(assembler->code + assembler->pc - sizeof(Immediate_t)));
	fprintf(lst, "%" ALIGNMENT "f\t",  *(Immediate_t*)(assembler->code + assembler->pc - sizeof(Immediate_t)));

	return ASM_NO_ERROR;
}

AsmStatusCode GetRegister(String* string, Assembler* assembler, int cmd_len) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	FILE* lst = assembler->listing;

	size_t reg_len = 0;

	while(*(string->cur_str + cmd_len) == ' ')
		cmd_len++;
	for (int i = cmd_len; *(string->cur_str + i) != '\0'; i++) {
		if (*(string->cur_str + i) == ' ' || *(string->cur_str + i) == '\t') {
			*(string->cur_str + i) = '\0';
			break;
		}
		reg_len++;
	}

	char* reg_addr = string->cur_str + cmd_len;

	if (reg_len > REGISTER_NAME_LENGTH)
		ASM_ERROR_DEMO(ASM_BIG_NAME_FOR_REGISTER);

	StringToUpper(reg_addr);

	asm_status = FindCharInString(reg_addr, 'X');
	ASM_ERROR_DEMO(asm_status);

	size_t cmd_pc = assembler->pc;

	*(assembler->code + assembler->pc) |= MASK_FOR_REGISTER;
	assembler->pc += sizeof(Command_t);

	if ((size_t)(*reg_addr - 'A' + 1) > MAX_REG_AMOUNT - 1 && *(reg_addr) != 'X')
		ASM_ERROR_DEMO(ASM_WRONG_LETTER_IN_REG_NAME);

	*(assembler->code + assembler->pc) = (*(reg_addr) == 'X' ? 0 : (unsigned char)(*reg_addr - 'A' + 1));
	assembler->pc += sizeof(Register_t);

	if (*(assembler->code + cmd_pc) & MASK_FOR_MEMORY)
		fprintf(lst, "[%" "6" "s]\t", reg_addr);
	else
		fprintf(lst, "%" ALIGNMENT "s\t", reg_addr);

	fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + cmd_pc));
	fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + cmd_pc));

	fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + assembler->pc - sizeof(Register_t)));
	fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + assembler->pc - sizeof(Register_t)));

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

	asm_status = IncreaseLabels(&assembler->labels_table);
	ASM_ERROR_DEMO(asm_status);

	size_t cmd_pc = assembler->pc;

	*(assembler->code + assembler->pc) |= MASK_FOR_NUMBER;
	assembler->pc += sizeof(Command_t);

	asm_status = LabelStatus(assembler, label);

	fprintf(lst, "%" ALIGNMENT "s\t", label);
	fprintf(lst, "%." ALIGNMENT "d\t", *(assembler->code + cmd_pc));
	fprintf(lst, "%." ALIGNMENT "x\t", *(assembler->code + cmd_pc));

	if (CompareDouble(*(Immediate_t*)(assembler->code + assembler->pc - sizeof(Immediate_t)), -1) == 0) {
		fprintf(lst, "%" ALIGNMENT "lg\t", *(Immediate_t*)(assembler->code + (assembler->pc - sizeof(Immediate_t))));
		fprintf(lst, "%" ALIGNMENT "f\t",  *(Immediate_t*)(assembler->code + (assembler->pc - sizeof(Immediate_t))));
	}
	else {
		fprintf(lst, "%" ALIGNMENT "s", "????????\t");
		fprintf(lst, "%" ALIGNMENT "s", "????????\t");
	}

	return ASM_NO_ERROR;
}
