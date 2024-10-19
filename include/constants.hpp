#ifndef CONSTANTS_INCLUDE
#define CONSTANTS_INCLUDE

#include <stdio.h>

const size_t REGISTER_NAME_LENGTH 	= 3;
const size_t MAX_COMMAND_LENGTH 	= 6;

const size_t MAX_REG_AMOUNT 		= 8;

enum Args {
	BIT_FOR_NUMBER 		= 1 << 5,
	BIT_FOR_MEMORY 		= 1 << 6,
	BIT_FOR_REGISTER 	= 1 << 7
};

#endif // CONSTANTS_INCLUDE
