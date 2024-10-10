#include "spu.hpp"

SPUStatusCode SPUCtor(SPU* proc) {

	FILE* input = fopen(proc->file, "r");
	if (!input)
		SPU_ERROR_DEMO(SPU_FILE_OPEN_ERROR);

	fscanf(input, "%zd", &proc->size);

	proc->code = (int*)calloc(proc->size, sizeof(int));
	if (!proc->code)
		SPU_ERROR_DEMO(SPU_ALLOC_ERROR);

	for (size_t i = 0; i < proc->size; i++)
		fscanf(input, "%d", (proc->code + i));

	return SPU_NO_ERROR;
}

SPUStatusCode SPURun(SPU* proc) {

	INIT_STACK(stk);

	STACK_CTOR(&stk, 1);

	for (size_t i = 0; i < proc->size; i++) {

		switch (*(proc->code + i)) {
			case PUSH: {
				STACK_PUSH(&stk, *(proc->code + (i++) + 1));
				break;
			}
			case ADD: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&stk, &x1);
				STACK_POP(&stk, &x2);

				STACK_PUSH(&stk, x1 + x2);

				break;
			}
			case SUB: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&stk, &x1);
				STACK_POP(&stk, &x2);

				STACK_PUSH(&stk, x2 - x1);

				break;
			}
			case DIV: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&stk, &x1);
				STACK_POP(&stk, &x2);

				STACK_PUSH(&stk, x2 / x1);

				break;
			}
			case MUL: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&stk, &x1);
				STACK_POP(&stk, &x2);

				STACK_PUSH(&stk, x2 * x1);

				break;
			}
			case IN: {
				Stack_elem_t x = 0;

				scanf(YELLOW("Enter integer number: %lg"), &x);

				STACK_PUSH(&stk, x);

				break;
			}
			case SQRT: {
				Stack_elem_t x = 0;

				STACK_POP(&stk, &x);

				x = sqrt(x);

				STACK_PUSH(&stk, x);

				break;
			}
			case SIN: {
				Stack_elem_t x = 0;

				x = sin(x);

				STACK_PUSH(&stk, x);

				break;
			}
			case COS: {
				Stack_elem_t x = 0;

				x = cos(x);

				STACK_PUSH(&stk, x);

				break;
			}
			case OUT: {
				Stack_elem_t result = 0;

				STACK_POP(&stk, &result);

				printf("\n result = %lg \n", result);

				break;
			}
			case HLT:
				break;
			default:
				SPU_ERROR_DEMO(SPU_COMMAND_ERROR);
		}

	}

	DoStackDtor(&stk);

	return SPU_NO_ERROR;
}
