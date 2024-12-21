#ifndef ASM_UTILITIES_INCLUDE
#define ASM_UTILITIES_INCLUDE

#include <stdio.h>
#include <ctype.h>

#include "../../include/enum.hpp"
#include "../../include/array.hpp"
#include "../../include/errors.hpp"
#include "../../include/constants.hpp"

#include "../../Onegin/include/Onegin.hpp"
#include "../../Onegin/include/Sorting.hpp"

#define RED(str) 		"\033[31;1m" str "\033[0m"
#define YELLOW(str) 	"\033[33;4m" str "\033[0m"
#define GREEN(str) 		"\033[32;6m" str "\033[0m"

#define ALIGNMENT "8"
#define LISTING_HEADER_PRINT(file, str) fprintf(file, "%" ALIGNMENT "s%s", str, "\t");

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
	ASM_FILE_CLOSE_ERROR,
	ASM_FILE_WRITE_ERROR,
	ASM_ERROR_AMOUNT_OF_ARGS,

	ASM_COMMAND_READ_ERROR,
	ASM_SYNTAX_COMMAND_ERROR,

	ASM_NO_CHAR_IN_STRING,
	ASM_BIG_NAME_FOR_REGISTER,
	ASM_WRONG_LETTER_IN_REG_NAME,

	ASM_BIG_NAME_FOR_LABEL,
	ASM_UNDEFINE_LABEL,

	ASM_ALLOC_ERROR
};

const size_t DEFAULT_LABELS_CAPACITY	= 20;
const size_t MAX_LABEL_LENGTH 		 	= 100;

const size_t MAX_FILE_NAME_SIZE 		= 1024;

const size_t MAX_COMMAND_LENGTH 		= 6;

const size_t MAX_REG_PLUS_NUMBER_ARG	= 8;
const size_t REGISTER_NAME_LENGTH 		= 3;

const double EPS = 1e-10;


size_t StrLen(const char* str);
const char* AsmErrorsMessenger(AsmStatusCode status);
int CompareDouble(const double var1, const double var2);

AsmStatusCode ConvertFileToAnother(const char* first_file, char* second_file, const char* extension);
AsmStatusCode StringToUpper(char* string);
AsmStatusCode FindCharInString(const char* str, const char x);
AsmStatusCode FindAndReplaceCharInString(char* str, const char x, const char rep, size_t* char_index);

#endif // ASM_UTILITIES_INCLUDE
