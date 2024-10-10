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

				STACK_PUSH(&stk, x1 - x2);

				break;
			}
			case SUB: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&stk, &x1);
				STACK_POP(&stk, &x2);

				STACK_PUSH(&stk, x1 * x2);

				break;
			}
			case DIV: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&stk, &x1);
				STACK_POP(&stk, &x2);

				STACK_PUSH(&stk, x1 / x2);

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
