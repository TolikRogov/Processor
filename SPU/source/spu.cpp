#include "spu.hpp"

SPUStatusCode SPUCtor(SPU* proc, const char* file) {

	SPUStatusCode spu_status = SPU_NO_ERROR;

	FILE* input = fopen(file, "r");
	if (!input)
		SPU_ERROR_DEMO(SPU_FILE_OPEN_ERROR);

	spu_status = CodeHeaderChecker(proc, input);
	SPU_ERROR_DEMO(spu_status);

	for (size_t i = 0; i < proc->size; i++) {
		int check = fscanf(input, "%x", (proc->code + i));
		if (check < 1)
			SPU_ERROR_DEMO(SPU_FILE_READ_ERROR);
	}

	if (fclose(input))
		SPU_ERROR_DEMO(SPU_FILE_CLOSE_ERROR);

	return SPU_NO_ERROR;
}

SPUStatusCode CodeHeaderChecker(SPU* proc, FILE* file) {

	char sign[MAX_SIGNATURE_LENGTH] = {};
	int check = fscanf(file, "%s", sign);
	if (check < 1)
		SPU_ERROR_DEMO(SPU_FILE_READ_ERROR);

	if (StrCmp(sign, SIGNATURE) != 0)
		SPU_ERROR_DEMO(SPU_SIGNATURE_ERROR);

	size_t code_version = 0;
	check = fscanf(file, "%zu", &code_version);
	if (check < 1)
		SPU_ERROR_DEMO(SPU_FILE_READ_ERROR);

	if (code_version != CODE_VERSION)
		SPU_ERROR_DEMO(SPU_CODE_VERSION_ERROR);

	check = fscanf(file, "%zu", &proc->size);
	if (check < 1)
		SPU_ERROR_DEMO(SPU_FILE_READ_ERROR);

	proc->code = (int*)calloc(proc->size, sizeof(int));
	if (!proc->code)
		SPU_ERROR_DEMO(SPU_ALLOC_ERROR);

	return SPU_NO_ERROR;
}

// TODO: double* GetArg();
SPUStatusCode SPURun(SPU* proc) {

	INIT_STACK(stk);

	STACK_CTOR(&stk, 1);

	for (size_t pc = 0; pc < proc->size; pc++) {

#ifdef SPU_DUMP
		SPUDump(proc, pc);
#endif

		switch (*(proc->code + pc) & 0x1F) { // TODO: const 0xiF
			case CMD_PUSH: {
				if ((*(proc->code + pc) & (1 << BIT_FOR_NUMBER)) >> BIT_FOR_NUMBER) {
					STACK_PUSH(&stk, *(proc->code + (pc++) + 1));
					break;
				}

				if ((*(proc->code + pc) & (1 << BIT_FOR_REGISTER)) >> BIT_FOR_REGISTER) {
					STACK_PUSH(&stk, proc->registers[*(proc->code + (pc++) + 1)]);
					break;
				}
			}
			case CMD_POP: {
				if ((*(proc->code + pc) & (1 << BIT_FOR_REGISTER)) >> BIT_FOR_REGISTER) {
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

				printf(GREEN("result = %d")"\n", (int)result);

				break;
			}
			case CMD_JB: {
				if ((*(proc->code + pc) & (1 << BIT_FOR_NUMBER)) >> BIT_FOR_NUMBER) {
					Stack_elem_t x1 = 0;
					Stack_elem_t x2 = 0;

					STACK_POP(&stk, &x1);
					STACK_POP(&stk, &x2);

					if (x2 < x1)
						pc = (size_t)*(proc->code + pc + 1) - 1;
					else
						pc++;
				}
			}
			case CMD_JMP: {
				if ((*(proc->code + pc) & (1 << BIT_FOR_NUMBER)) >> BIT_FOR_NUMBER) {
					pc = (size_t)*(proc->code + pc++ + 1) - 1;
				}
			}
			case CMD_HLT:
				break;
			default:
				SPU_ERROR_DEMO(SPU_COMMAND_ERROR);
		}

	}

	DoStackDtor(&stk);

	return SPU_NO_ERROR;
}

SPUStatusCode SPUDump(SPU* proc, size_t pc) {

	for (size_t i = 0; i < 3 * proc->size; i++)
		printf("-");
	printf("\n");

	for (size_t i = 0; i < proc->size; i++)
		printf("%.2zu ", i);
	printf("\n");

	for (size_t i = 0; i < proc->size; i++)
		printf("%.2d ", *(proc->code + i));
	printf("\n");

	for (size_t i = 0; i < pc; i++)
		printf("   ");
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
