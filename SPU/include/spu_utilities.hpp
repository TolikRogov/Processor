#ifndef SPU_UTILITIES_INCLUDE
#define SPU_UTILITIES_INCLUDE

#include "../../include/errors.hpp"
#include "../../include/commands.hpp"

#define SPU_ERROR_DEMO(status) {														 							 \
	if (status != SPU_NO_ERROR)	{																					\
		fprintf(stderr, "\n\n" RED("Error (code %d): %s, ") YELLOW("File: %s, Function: %s, Line: %d\n\n"),   		\
					status, SPUErrorsMessenger(status), __FILE__, __PRETTY_FUNCTION__, __LINE__);					\
		fclose(stderr);																								\
		return status;																								\
	}																												\
}

const char* SPUErrorsMessenger(SPUStatusCode status);

#endif // SPU_UTILITIES_INCLUDE
