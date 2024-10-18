#ifndef CONSTANTS_INCLUDE
#define CONSTANTS_INCLUDE

#include <stdio.h>

const size_t REGISTER_NAME_LENGTH 				= 5;
const size_t MAX_COMMAND_LENGTH 				= 6;

const size_t MAX_SIGNATURE_LENGTH 				= 20;
const size_t CODE_VERSION 						= 2;
const char 	 SIGNATURE[MAX_SIGNATURE_LENGTH] 	= "TGF";

const size_t MAX_REG_AMOUNT 					= 8;

const size_t BIT_FOR_NUMBER 					= 5;
const size_t BIT_FOR_REGISTER 					= 6;
const size_t BIT_FOR_MEMORY 					= 7;

#endif // CONSTANTS_INCLUDE
