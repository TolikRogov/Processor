#ifndef ASM_UTILITIES_INCLUDE
#define ASM_UTILITIES_INCLUDE

#include <stdio.h>
#include "../../include/commands.hpp"
#include "../../include/errors.hpp"

#define RED(str) 		"\033[31;1m" str "\033[0m"
#define YELLOW(str) 	"\033[33;4m" str "\033[0m"
#define GREEN(str) 		"\033[32;6m" str "\033[0m"

#define ASM_ERROR_DEMO(status) {														 							 \
	if (status != ASM_NO_ERROR)	{																					\
		fprintf(stderr, "\n\n" RED("Error (code %d): %s, ") YELLOW("File: %s, Function: %s, Line: %d\n\n"),   		\
					status, AsmErrorsMessenger(status), __FILE__, __PRETTY_FUNCTION__, __LINE__);					\
		fclose(stderr);																								\
		return status;																								\
	}																												\
}

enum AsmStatusCode {

	ASM_NO_ERROR,

	ASM_FILE_OPEN_ERROR,
	ASM_FILE_CLOSE_ERROR

};

const char* AsmErrorsMessenger(AsmStatusCode status);

#endif
