#ifndef CONSTANTS_INCLUDE
#define CONSTANTS_INCLUDE

#include <stdio.h>

const size_t REGISTER_NAME_LENGTH 	= 3;
const size_t MAX_COMMAND_LENGTH 	= 6;

const size_t MAX_REG_AMOUNT 		= 8;

const size_t CODE_VERSION 				  = 2;
const char 	 SIGNATURE[sizeof(long long)] = "TGF";

struct McHeader {
	long long signature;
	size_t code_version;
	size_t code_size;
};

enum Args {
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
