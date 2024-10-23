#ifndef SPU_INCLUDE
#define SPU_INCLUDE

#include "../../Stack/include/STACK_LIBRARY.hpp"
#include "../../Onegin/include/Sorting.hpp"
#include "spu_utilities.hpp"
#include <math.h>

struct SPU {
	size_t size;
	int* code;

	int registers[MAX_REG_AMOUNT];

	INIT_STACK(stk);
	INIT_STACK(ret_addr_stk);

	int ram[RAM_SIZE];
};

SPUStatusCode SPUCtor(SPU* proc, const char* file);
SPUStatusCode CodeHeaderChecker(SPU* proc, FILE* file);
SPUStatusCode SPUDump(SPU* proc, size_t pc);
SPUStatusCode LogDump(SPU* proc);
SPUStatusCode SPURun(SPU* proc);

#endif // SPU_INCLUDE
