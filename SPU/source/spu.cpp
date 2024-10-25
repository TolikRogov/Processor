#include "spu.hpp"
#include "spu_dump.hpp"

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

	SPUStatusCode spu_status = SPU_NO_ERROR;

	STACK_CTOR(&proc->stk, 1);
	STACK_CTOR(&proc->ret_addr_stk, 1);

	for (; proc->pc < proc->size; proc->pc++) {

#ifdef SPU_DUMP
		SPUDump(proc, proc->pc);
#endif

		switch (*(proc->code + proc->pc) & MASK_FOR_COMMANDS) {
			case CMD_PUSH: {
				STACK_PUSH(&proc->stk, *GetArg(proc));
				break;
			}
			case CMD_POP: {
				Stack_elem_t x = 0;

				STACK_POP(&proc->stk, &x);

				*GetArg(proc) = (int)x;
				break;
			}
			case CMD_ADD: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&proc->stk, &x1);
				STACK_POP(&proc->stk, &x2);

				STACK_PUSH(&proc->stk, x1 + x2);
				break;
			}
			case CMD_SUB: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&proc->stk, &x1);
				STACK_POP(&proc->stk, &x2);

				STACK_PUSH(&proc->stk, x2 - x1);
				break;
			}
			case CMD_DIV: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&proc->stk, &x1);
				STACK_POP(&proc->stk, &x2);

				STACK_PUSH(&proc->stk, x2 / x1);
				break;
			}
			case CMD_MUL: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&proc->stk, &x1);
				STACK_POP(&proc->stk, &x2);

				STACK_PUSH(&proc->stk, x2 * x1);
				break;
			}
			case CMD_MOD: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&proc->stk, &x1);
				STACK_POP(&proc->stk, &x2);

				STACK_PUSH(&proc->stk, (int)x2 % (int)x1);
				break;
			}
			case CMD_IN: {
				Stack_elem_t x = 0;

				printf(YELLOW("Enter your number:")" ");
				scanf("%lg", &x);

				STACK_PUSH(&proc->stk, x);
				break;
			}
			case CMD_SQRT: {
				Stack_elem_t x = 0;

				STACK_POP(&proc->stk, &x);

				STACK_PUSH(&proc->stk, sqrt(x));
				break;
			}
			case CMD_SIN: {
				Stack_elem_t x = 0;

				STACK_POP(&proc->stk, &x);

				STACK_PUSH(&proc->stk, sin(x));
				break;
			}
			case CMD_COS: {
				Stack_elem_t x = 0;

				STACK_POP(&proc->stk, &x);

				STACK_PUSH(&proc->stk, cos(x));
				break;
			}
			case CMD_OUT: {
				Stack_elem_t result = 0;

				STACK_POP(&proc->stk, &result);

				printf(GREEN("result = %lg")"\n", result);
				break;
			}
			case CMD_JB: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&proc->stk, &x1);
				STACK_POP(&proc->stk, &x2);

				if (x2 < x1)
					proc->pc = (size_t)*GetArg(proc) - 1;
				else
					proc->pc++;
				break;
			}
			case CMD_JE: {
				Stack_elem_t x1 = 0;
				Stack_elem_t x2 = 0;

				STACK_POP(&proc->stk, &x1);
				STACK_POP(&proc->stk, &x2);

				if (CompareDouble(x1, x2))
					proc->pc = (size_t)*GetArg(proc) - 1;
				else
					proc->pc++;
				break;
			}
			case CMD_JMP: {
				proc->pc = (size_t)*GetArg(proc) - 1;
				break;
			}
			case CMD_CALL: {
				STACK_PUSH(&proc->ret_addr_stk, proc->pc + 1);
				proc->pc = (size_t)*GetArg(proc) - 1;
				break;
			}
			case CMD_DRAW: {
				for (size_t i = 0; i < RAM_SIZE;) {
					for (size_t j = 0; j < DRAW_WIDTH; j++) {
						if (proc->ram[i++] == 0)
							printf(". ");
						else
							printf("* ");
					}
					printf("\n");
				}
				break;
			}
			case CMD_RET: {
				Stack_elem_t ret = 0;

				STACK_POP(&proc->ret_addr_stk, &ret);

				proc->pc = (size_t)ret;
				break;
			}
			case CMD_HLT: {
				DoStackDtor(&proc->stk);
				DoStackDtor(&proc->ret_addr_stk);

				spu_status = LogDump(proc);
				SPU_ERROR_DEMO(spu_status);

				return SPU_NO_ERROR;
			}
			default:
				SPU_ERROR_DEMO(SPU_COMMAND_ERROR);
		}
	}

	return SPU_NO_ERROR;
}

// TODO: double* GetArg
int* GetArg(SPU* proc) {
	proc->registers[0] = 0;
	int argType = proc->code[proc->pc++];
	int* argValue = NULL;

	if (argType & MASK_FOR_REGISTER) {
		proc->registers[0] = proc->registers[proc->code[proc->pc]];
		argValue = &proc->registers[proc->code[proc->pc++]];
	}
	if (argType & MASK_FOR_NUMBER) 		argValue = &(proc->registers[0] += proc->code[proc->pc++]);
	if (argType & MASK_FOR_MEMORY)		argValue = &proc->ram[proc->registers[0]];

	proc->pc--;
	return argValue;
}
