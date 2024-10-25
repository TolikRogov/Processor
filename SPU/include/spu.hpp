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
	int* code;
	size_t pc;

	int registers[MAX_REG_AMOUNT];

	INIT_STACK(stk);
	INIT_STACK(ret_addr_stk);

	int ram[RAM_SIZE];
};

SPUStatusCode SPUCtor(SPU* proc, const char* file);
SPUStatusCode CodeHeaderChecker(SPU* proc, FILE* file);
SPUStatusCode SPURun(SPU* proc);
int* GetArg(SPU* proc);

#endif // SPU_INCLUDE
