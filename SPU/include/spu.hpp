#ifndef SPU_INCLUDE
#define SPU_INCLUDE

#include "STACK_LIBRARY.hpp"
#include "spu_utilities.hpp"
#include <sys/stat.h>

struct SPU {
	const char* file;
	size_t size;
	int* code;
};

SPUStatusCode SPUCtor(SPU* proc);
SPUStatusCode SPURun(SPU* proc);

#endif // SPU_INCLUDE
