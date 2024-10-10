#ifndef SPU_INCLUDE
#define SPU_INCLUDE

#include "STACK_LIBRARY.hpp"

struct SPU {
	char* file_name;
	size_t size;
	int* code;
};

StackStatusCode SPURun();

#endif // SPU_INCLUDE
