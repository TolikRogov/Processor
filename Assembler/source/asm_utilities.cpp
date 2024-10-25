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

AsmStatusCode StringToUpper(char* string) {

	for (size_t i = 0; *(string + i) != '\0'; i++) {
		*(string + i) = (char)toupper(*(string + i));
	}

	return ASM_NO_ERROR;
}

AsmStatusCode FindCharInString(const char* str, const char x) {

	for (size_t i = 0; *(str + i) != '\0'; i++) {
		if (*(str + i) == x)
			return ASM_NO_ERROR;
	}

	return ASM_NO_CHAR_IN_STRING;
}

AsmStatusCode FindAndReplaceCharInString(char* str, const char x, const char rep, size_t* char_index) {

	for (size_t i = 0; *(str + i) != '\0'; i++) {
		if (*(str + i) == x) {
			*(str + i) = rep;
			if (char_index)
				*char_index = i;
			return ASM_NO_ERROR;
		}
	}

	return ASM_NO_CHAR_IN_STRING;
}

size_t StrLen(const char* str) {

	size_t length = 0;

	for (size_t i = 0; *(str + i) != '\0'; i++)
		length++;

	return length;
}

AsmStatusCode ConvertFileToAnother(const char* first_file, char* second_file, const char* extension) {

	size_t first_file_size = StrLen(first_file);

	size_t point_index = first_file_size;
	for (; *(first_file + point_index) != '.'; point_index--) {}

	for (size_t i = 0; i <= point_index; i++)
		*(second_file + i) = *(first_file + i);

	size_t extension_size = StrLen(extension);
	for (size_t i = 0; i < extension_size + 1; i++)
		*(second_file + point_index + i) = *(extension + i);

	return ASM_NO_ERROR;
}
