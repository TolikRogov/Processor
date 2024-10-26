#ifndef SPU_INCLUDE
#define SPU_INCLUDE

#include "../../Stack/include/STACK_LIBRARY.hpp"
#include "../../Onegin/include/Sorting.hpp"
#include "spu_utilities.hpp"
#include <math.h>

#define DEF_CMD_(cmd, ...) 	 \
	case CMD_ ## cmd: {		\
		__VA_ARGS__;		\
		break;				\
	}

struct SPU {
	size_t size;
	unsigned char* code;
	size_t pc;

	Immediate_t registers[MAX_REG_AMOUNT];

	INIT_STACK(stk);
	INIT_STACK(ret_addr_stk);

	Immediate_t ram[RAM_SIZE];
};

SPUStatusCode SPUCtor(SPU* proc, const char* file);
SPUStatusCode SPUDtor(SPU* proc);
SPUStatusCode CodeHeaderChecker(SPU* proc, FILE* file);
SPUStatusCode SPURun(SPU* proc);
double* GetArg(SPU* proc);

#endif // SPU_INCLUDE
