#ifndef UTILITIES_INCLUDE
#define UTILITIES_INCLUDE

#include "libraries.hpp"
#include "wrappers.hpp"
#include "stack.hpp"

const size_t 		ALIGNMENT	 		= 8;
const size_t 		DEFAULT_CAPACITY 	= 16;

const Stack_elem_t 	POISON 	  			= -6e-99;
const double 		EPS 			 	= 1e-10;

const u_int64_t 	TRASH 			  	= 0xBEDADEDBEDA;
const u_int64_t 	STACK_CANARY_HEX	= 0xDEADDED;
const u_int64_t		DATA_CANARY_HEX		= 0xBADFACE;

StackStatusCode StrConcatenation(const char* string1, const char* string2, char** str_out, Stack_t* stk);
StackStatusCode CustomMemsetDouble(double* memory_start, const size_t cnt, const double value);

size_t StrLen(const char* str);
int CompareDouble(const double var1, const double var2);
const char* StackErrorsMessenger(StackStatusCode status);

#endif //UTILITIES_INCLUDE
