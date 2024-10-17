#ifndef SPU_UTILITIES_INCLUDE
#define SPU_UTILITIES_INCLUDE

#include "spu_config.hpp"
#include <stdio.h>
#include "../../include/errors.hpp"
#include "../../include/commands.hpp"
#include "../../include/CodeProtect.hpp"

#define SPU_ERROR_DEMO(status) {														 							 \
	if (status != SPU_NO_ERROR)	{																					\
		fprintf(stderr, "\n\n" RED("Error (code %d): %s, ") YELLOW("File: %s, Function: %s, Line: %d\n\n"),   		\
					status, SPUErrorsMessenger(status), __FILE__, __PRETTY_FUNCTION__, __LINE__);					\
		fclose(stderr);																								\
		return status;																								\
	}																												\
}

const size_t MAX_REG_AMOUNT = 8;

const char* SPUErrorsMessenger(SPUStatusCode status);

#endif // SPU_UTILITIES_INCLUDE
