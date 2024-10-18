#ifndef SPU_UTILITIES_INCLUDE
#define SPU_UTILITIES_INCLUDE

#include <stdio.h>
#include "../../include/errors.hpp" // try with -I ../../include
#include "../../include/commands.hpp"
#include "../../include/constants.hpp"

#define SPU_ERROR_DEMO(status) {														 							 \
	if (status != SPU_NO_ERROR)	{																					\
		fprintf(stderr, "\n\n" RED("Error (code %d): %s, ") YELLOW("File: %s, Function: %s, Line: %d\n\n"),   		\
					status, SPUErrorsMessenger(status), __FILE__, __PRETTY_FUNCTION__, __LINE__);					\
		fclose(stderr);																								\
		return status;																								\
	}																												\
}

const size_t MAX_SIGNATURE_LENGTH 			 = 20;
const size_t CODE_VERSION 					 = 2;
const char 	 SIGNATURE[MAX_SIGNATURE_LENGTH] = "TGF";

struct McHeader {
	const char* signature = SIGNATURE;
	size_t code_version   = CODE_VERSION;
};

const char* SPUErrorsMessenger(SPUStatusCode status);

#endif // SPU_UTILITIES_INCLUDE
