#include "asm_utilities.hpp"

const char* AsmErrorsMessenger(AsmStatusCode status) {
	switch (status) {
		case ASM_NO_ERROR:					return "ASM ERROR - NO ERROR";

		case ASM_FILE_CLOSE_ERROR:			return "ASM ERROR - FILE WAS NOT CLOSED";
		case ASM_FILE_OPEN_ERROR:			return "ASM ERROR - FILE WAS NOT OPENED";
		case ASM_FILE_WRITE_ERROR:			return "ASM ERROR - DATA WAS NOT WRITTEN TO FILE";
		case ASM_ERROR_AMOUNT_OF_ARGS:		return "ASM ERROR - LESS ARGUMENTS THAN NEEDED";

		case ASM_COMMAND_READ_ERROR:		return "ASM ERROR - COMMAND WAS NOT READ";
		case ASM_SYNTAX_COMMAND_ERROR:		return "ASM ERROR - ERROR SYNTAX OF COMMAND";

		case ASM_NO_CHAR_IN_STRING:			return "ASM ERROR - THERE IS NO THE CHAR IN STRING";
		case ASM_BIG_NAME_FOR_REGISTER:		return "ASM ERROR - NAME OF YOUR REGISTER SO BIG";
		case ASM_WRONG_LETTER_IN_REG_NAME:	return "ASM ERROR - WRONG NAME FOR REGISTER";

		case ASM_BIG_NAME_FOR_LABEL:		return "ASM ERROR - LABEL NAME MUST BE LESS";
		case ASM_UNDEFINE_LABEL:			return "ASM ERROR - LABEL IS UNDEFINED";

		case ASM_ALLOC_ERROR:				return "ASM ERROR - MEMORY ALLOCATION ERROR";
	}
}

AsmStatusCode StringToLower(char* string) {

	for (size_t i = 0; *(string + i) != '\0'; i++) {
		*(string + i) = (char)tolower(*(string + i));
	}

	return ASM_NO_ERROR;
}

AsmStatusCode FindCharInString(const char* reg, const char x) {

	for (size_t i = 0; *(reg + i) != '\0'; i++) {
		if (*(reg + i) == x)
			return ASM_NO_ERROR;
	}

	return ASM_NO_CHAR_IN_STRING;
}
