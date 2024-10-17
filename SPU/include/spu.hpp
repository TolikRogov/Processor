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
};

SPUStatusCode SPUCtor(SPU* proc, const char* file);
SPUStatusCode CodeHeaderChecker(SPU* proc, FILE* file);
SPUStatusCode SPURun(SPU* proc);

#endif // SPU_INCLUDE
