#include "assembler.hpp"

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler) {

	assembler->code = (int*)calloc(storage->str_cnt, sizeof(int));
	if (!assembler->code)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	for (size_t i = 0; i < storage->str_cnt; i++) {

		char command[MAX_COMMAND_LENGTH] = {};
		int symbols_num = sscanf(storage->text[i].cur_str, "%s", command);
		if (symbols_num == -1)
			ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

		if (StrCmp(command, "push") == 0) {
			printf("command: 0x%x\n", CMD_PUSH);
		}
		if (StrCmp(command, "hlt") == 0) {
			printf("command: 0x%x\n", CMD_HLT);
		}
		if (StrCmp(command, "add") == 0) {
			printf("command: 0x%x\n", CMD_ADD);
		}
		if (StrCmp(command, "sub") == 0) {
			printf("command: 0x%x\n", CMD_SUB);
		}
		if (StrCmp(command, "div") == 0) {
			printf("command: 0x%x\n", CMD_DIV);
		}
		if (StrCmp(command, "out") == 0) {
			printf("command: 0x%x\n", CMD_OUT);
		}
		if (StrCmp(command, "mul") == 0) {
			printf("command: 0x%x\n", CMD_MUL);
		}
		if (StrCmp(command, "in") == 0) {
			printf("command: 0x%x\n", CMD_IN);
		}
		if (StrCmp(command, "sqrt") == 0) {
			printf("command: 0x%x\n", CMD_SQRT);
		}
		if (StrCmp(command, "sin") == 0) {
			printf("command: 0x%x\n", CMD_SIN);
		}
		if (StrCmp(command, "cos") == 0) {
			printf("command: 0x%x\n", CMD_COS);
		}

	}

	return ASM_NO_ERROR;
}
