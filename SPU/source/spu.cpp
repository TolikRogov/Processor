#include "spu.hpp"

SPUStatusCode SPUCtor(SPU* proc, const char* file) {

	SPUStatusCode spu_status = SPU_NO_ERROR;

	FILE* input = fopen(file, "rb");
	if (!input)
		SPU_ERROR_DEMO(SPU_FILE_OPEN_ERROR);

	spu_status = CodeHeaderChecker(proc, input);
	SPU_ERROR_DEMO(spu_status);

	size_t read_check = fread(proc->code, sizeof(int), proc->size, input);
	if (read_check != proc->size)
		SPU_ERROR_DEMO(SPU_FILE_READ_ERROR);

	if (fclose(input))
		SPU_ERROR_DEMO(SPU_FILE_CLOSE_ERROR);

	return SPU_NO_ERROR;
}

SPUStatusCode CodeHeaderChecker(SPU* proc, FILE* file) {

	McHeader header = {};

	size_t read_check = fread(&header, sizeof(char), sizeof(McHeader), file);
	if (read_check != sizeof(McHeader))
		SPU_ERROR_DEMO(SPU_FILE_READ_ERROR);

	if (header.signature != *(const long long*)SIGNATURE)
		SPU_ERROR_DEMO(SPU_SIGNATURE_ERROR);

	if (header.code_version != CODE_VERSION)
		SPU_ERROR_DEMO(SPU_CODE_VERSION_ERROR);

	proc->size = header.code_size;

	proc->code = (int*)calloc(proc->size, sizeof(int));
	if (!proc->code)
		SPU_ERROR_DEMO(SPU_ALLOC_ERROR);

	return SPU_NO_ERROR;
}

SPUStatusCode SPURun(SPU* proc) {

	INIT_STACK(stk);
	INIT_STACK(ret_addr_stk);

	STACK_CTOR(&stk, 1);
	STACK_CTOR(&ret_addr_stk, 1);

	for (size_t pc = 0; pc < proc->size; pc++) {

#ifdef SPU_DUMP
		SPUDump(proc, pc);
#endif

		switch (*(proc->code + pc) & MASK_FOR_COMMANDS) {
			case CMD_PUSH: {
				if (*(proc->code + pc) & BIT_FOR_NUMBER) {
					STACK_PUSH(&stk, *(proc->code + (pc++) + 1));
					break;
				}

				if (*(proc->code + pc) & BIT_FOR_REGISTER) {
					STACK_PUSH(&stk, proc->registers[*(proc->code + (pc++) + 1)]);
					break;
				}
			}
			case CMD_POP: {
				if (*(proc->code + pc) & BIT_FOR_REGISTER) {
					Stack_elem_t x = 0;
					STACK_POP(&stk, &x);

					proc->registers[*(proc->code + (pc++) + 1)] = (int)x;
					break;
				}
			}
			case CMD_ADD: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&stk, &x1);
				STACK_POP(&stk, &x2);

				STACK_PUSH(&stk, x1 + x2);

				break;
			}
			case CMD_SUB: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&stk, &x1);
				STACK_POP(&stk, &x2);

				STACK_PUSH(&stk, x2 - x1);

				break;
			}
			case CMD_DIV: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&stk, &x1);
				STACK_POP(&stk, &x2);

				STACK_PUSH(&stk, x2 / x1);

				break;
			}
			case CMD_MUL: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&stk, &x1);
				STACK_POP(&stk, &x2);

				STACK_PUSH(&stk, x2 * x1);

				break;
			}
			case CMD_IN: {
				Stack_elem_t x = 0;

				printf(YELLOW("Enter your number:")" ");
				scanf("%lg", &x);

				STACK_PUSH(&stk, x);

				break;
			}
			case CMD_SQRT: {
				Stack_elem_t x = 0;

				STACK_POP(&stk, &x);

				x = sqrt(x);

				STACK_PUSH(&stk, x);

				break;
			}
			case CMD_SIN: {
				Stack_elem_t x = 0;

				STACK_POP(&stk, &x);

				x = sin(x);

				STACK_PUSH(&stk, x);

				break;
			}
			case CMD_COS: {
				Stack_elem_t x = 0;

				STACK_POP(&stk, &x);

				x = cos(x);

				STACK_PUSH(&stk, x);

				break;
			}
			case CMD_OUT: {
				Stack_elem_t result = 0;

				STACK_POP(&stk, &result);

				printf(GREEN("result = %lg")"\n", result);

				break;
			}
			case CMD_JB: {
				if (*(proc->code + pc) & BIT_FOR_NUMBER) {
					Stack_elem_t x1 = 0;
					Stack_elem_t x2 = 0;

					STACK_POP(&stk, &x1);
					STACK_POP(&stk, &x2);

					if (x2 < x1)
						pc = (size_t)*(proc->code + pc + 1) - 1;
					else
						pc++;
				}
				break;
			}
			case CMD_JMP: {
				if (*(proc->code + pc) & BIT_FOR_NUMBER) {
					pc = (size_t)*(proc->code + pc++ + 1) - 1;
				}
				break;
			}
			case CMD_CALL: {
				if (*(proc->code + pc) & BIT_FOR_NUMBER) {
					STACK_PUSH(&ret_addr_stk, pc + 1);

					pc = (size_t)*(proc->code + pc++ + 1) - 1;
				}
				break;
			}
			case CMD_RET: {
				Stack_elem_t ret = 0;

				STACK_POP(&ret_addr_stk, &ret);

				pc = (size_t)ret;

				break;
			}
			case CMD_HLT: {
				DoStackDtor(&stk);
				DoStackDtor(&ret_addr_stk);
				return SPU_NO_ERROR;
			}
			default:
				SPU_ERROR_DEMO(SPU_COMMAND_ERROR);
		}

	}

	return SPU_NO_ERROR;
}

SPUStatusCode SPUDump(SPU* proc, size_t pc) {

	for (size_t i = 0; i < 3 * proc->size; i++)
		printf("-");
	printf("\n");

	for (size_t i = 0; i < proc->size; i++)
		printf("%.3zu ", i);
	printf("\n");

	for (size_t i = 0; i < proc->size; i++)
		printf("%.3d ", *(proc->code + i));
	printf("\n");

	for (size_t i = 0; i < pc; i++)
		printf("    ");
	printf(YELLOW("^")"\n");

	printf("pc = %zu\n", pc);

	printf("Registers: ");
	printf("%cX = %d ", 'X', proc->registers[0]);
	for (size_t i = 1; i < MAX_REG_AMOUNT; i++)
		printf("%cX = %d ", 'A' + (int)i - 1, proc->registers[i]);
	printf("\n");

	for (size_t i = 0; i < 3 * proc->size; i++)
		printf("-");
	printf("\n");

	getchar();

	return SPU_NO_ERROR;
}
