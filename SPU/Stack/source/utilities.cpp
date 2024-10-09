#include "../include/utilities.hpp"

const char* StackErrorsMessenger(StackStatusCode status) {
	switch (status) {
		case STACK_NO_ERROR: 				return "STACK ERROR - NO ERRORS";
		case STACK_ERROR:					return "STACK ERROR - ERROR";

		case STACK_ALLOC_ERROR: 			return "STACK ERROR - MEMORY ALLOCATION FAILED";

		case STACK_FILE_CLOSE_ERROR:		return "STACK ERROR - FILE WAS NOT CLOSED";
		case STACK_FILE_OPEN_ERROR:			return "STACK ERROR - FILE WAS NOT OPENED";

		case STACK_DIR_OPEN_ERROR:			return "STACK ERROR - DIRECTORY WAS NOT OPENED";
		case STACK_DIR_CLOSE_ERROR:			return "STACK ERROR - DIRECTORY WAS NOT CLOSED";

		case STACK_DIR_DELETE_ERROR:		return "STACK ERROR - DIRECTORY WAS NOT DELETED";
		case STACK_DIR_MAKE_ERROR:			return "STACK ERROR - DIRECTORY WAS NOT MADE";
		case STACK_RUN_HTML_ERROR:			return "STACK ERROR - HTML FILE WAS NOT RUNNING";

		case STACK_UNDERFLOW:				return "STACK ERROR - STACK UNDERFLOW";
		case STACK_SIZE_ERROR:				return "STACK ERROR - WRONG STACK SIZE";
		case STACK_CAPACITY_ERROR:			return "STACK ERROR - WRONG STACK CAPACITY";
		case STACK_DIMENSIONS_ERROR:		return "STACK ERROR - STACK SIZE GREATER THAN CAPACITY";

		case STACK_POINTER_ERROR:			return "STACK ERROR - STACK POINTER IS NULL";
		case STACK_DATA_POINTER_ERROR:		return "STACK ERROR - STACK DATA POINTER IS NULL";

		case STACK_POP_ERROR:				return "STACK ERROR - NOTHING TO POP FROM STACK";

		case STACK_LEFT_CANARY_ERROR:		return "STACK ERROR - LEFT STACK CANARY IS WRONG";
		case STACK_RIGHT_CANARY_ERROR:		return "STACK ERROR - RIGHT STACK CANARY IS WRONG";

		case STACK_DATA_LEFT_CANARY_ERROR:	return "STACK ERROR - LEFT DATA CANARY IS WRONG";
		case STACK_DATA_RIGHT_CANARY_ERROR:	return "STACK ERROR - RIGHT DATA CANARY IS WRONG";

		case STACK_HASH_ERROR:				return "STACK ERROR - STACK HASH HAS CHANGED";
		case STACK_DATA_HASH_ERROR:			return "STACK ERROR - STACK DATA HASH HAS CHANGED";

		case STACK_STATUS_ERROR:			return "STACK ERROR - STACK STATUS WAS CHANGED FROM OUTSIDE";
		default:			 				return "UNDEFINED ERROR";
	}
}

size_t StrLen(const char* str) {

	size_t length = 0;

	for (size_t i = 0; *(str + i) != '\0'; i++)
		length++;

	return length;
}

StackStatusCode StrConcatenation(const char* string1, const char* string2, char** str_out, Stack_t* stk) {

	size_t size = StrLen(string1) + StrLen(string2);

	*str_out = (char*)calloc(size + 1, sizeof(char));
	if (!str_out)
		STACK_ERROR_CHECK(STACK_ALLOC_ERROR, stk);

	size_t i = 0;
	for (;*(string1 + i) != '\0'; i++)
		*(*str_out + i) = *(string1 + i);

	for (size_t j = 0; *(string2 + j) != '\0'; j++) {
		*(*str_out + i) = *(string2 + j);
		i++;
	}

	*(*str_out + i) = '\0';

	return STACK_NO_ERROR;
}

StackStatusCode CustomMemsetDouble(double* memory_start, const size_t cnt, const double value) {

	if (!memory_start)
		return STACK_DATA_POINTER_ERROR;

	for (size_t i = 0; i < cnt; i++)
		*(memory_start + i) = value;

	return STACK_NO_ERROR;
};

int CompareDouble(const double var1, const double var2) {
	return (abs(var1 - var2) <= EPS);
}
