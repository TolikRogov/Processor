#include "asm_utilities.hpp"

const char* AsmErrorsMessenger(AsmStatusCode status) {
	switch (status) {
		case ASM_NO_ERROR:					return "ASM ERROR - NO ERROR";

		case ASM_FILE_CLOSE_ERROR:			return "ASM ERROR - FILE WAS NOT CLOSED";
		case ASM_FILE_OPEN_ERROR:			return "ASM ERROR - FILE WAS NOT CLOSED";
	}
}
