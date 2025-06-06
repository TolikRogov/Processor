#ifndef CONSTANTS_INCLUDE
#define CONSTANTS_INCLUDE

#include <stdio.h>

typedef double 			Immediate_t;
typedef double 			Label_t;
typedef unsigned char   Register_t;
typedef unsigned char   Command_t;

const size_t MAX_REG_AMOUNT = 8;

const size_t CODE_VERSION 				  = 3;
const char 	 SIGNATURE[sizeof(long long)] = "TGF";

struct McHeader {
	long long signature;
	size_t code_version;
	size_t code_size;
};

enum Args: unsigned char {
	BIT_FOR_NUMBER		= 5,
	BIT_FOR_REGISTER	= 6,
	BIT_FOR_MEMORY		= 7,

	MASK_WITHOUT_MEMORY = 0xFF >> 1,
	MASK_FOR_COMMANDS 	= 0x1F,
	MASK_FOR_NUMBER 	= 1 << BIT_FOR_NUMBER,
	MASK_FOR_REGISTER 	= 1 << BIT_FOR_REGISTER,
	MASK_FOR_MEMORY 	= 1 << BIT_FOR_MEMORY
};

#endif // CONSTANTS_INCLUDE
