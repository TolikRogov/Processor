#ifndef SPU_UTILITIES_INCLUDE
#define SPU_UTILITIES_INCLUDE

enum SPUStatusCode {
	SPU_NO_ERROR,

	SPU_FILE_OPEN_ERROR,
	SPU_FILE_CLOSE_ERROR,
	SPU_FILE_READ_ERROR,

	SPU_ALLOC_ERROR,

	SPU_COMMAND_ERROR
};

enum Commands {
	HLT,
	PUSH,
	ADD,
	SUB,
	DIV,
	OUT,
	MUL,
	IN,
	SQRT,
	SIN,
	COS
};

#define SPU_ERROR_DEMO(status) {														 							 \
	if (status != SPU_NO_ERROR)	{																					\
		if (!(fclose(stderr)))																						\
			fprintf(stderr, "\n\n" RED("Error (code %d): %s, ") YELLOW("File: %s, Function: %s, Line: %d\n\n"),   	\
					status, SPUErrorsMessenger(status), __FILE__, __PRETTY_FUNCTION__, __LINE__);					\
		return status;																								\
	}																												\
}

const char* SPUErrorsMessenger(SPUStatusCode status);

#endif // SPU_UTILITIES_INCLUDE
