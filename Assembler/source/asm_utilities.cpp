#include "asm_utilities.hpp"

const char* AsmErrorsMessenger(AsmStatusCode status) {
	switch (status) {
		case ASM_NO_ERROR:					return "ASM ERROR - NO ERROR";

		case ASM_FILE_CLOSE_ERROR:			return "ASM ERROR - FILE WAS NOT CLOSED";
		case ASM_FILE_OPEN_ERROR:			return "ASM ERROR - FILE WAS NOT OPENED";
		case ASM_ERROR_AMOUNT_OF_ARGS:		return "ASM ERROR - LESS ARGUMENTS THAN NEEDED";

		case ASM_COMMAND_READ_ERROR:		return "ASM ERROR - COMMAND WAS NOT READ";

		case ASM_ALLOC_ERROR:				return "ASM ERROR - MEMORY ALLOCATION ERROR";
	}
}
