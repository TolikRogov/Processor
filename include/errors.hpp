#ifndef PROC_ERRORS_INCLUDE
#define PROC_ERRORS_INCLUDE

enum SPUStatusCode {
	SPU_NO_ERROR,

	SPU_NO_FILE_TO_RUN,
	SPU_FILE_OPEN_ERROR,
	SPU_FILE_CLOSE_ERROR,
	SPU_FILE_READ_ERROR,

	SPU_ALLOC_ERROR,

	SPU_COMMAND_ERROR,

	SPU_SIGNATURE_ERROR
};

#endif // PROC_ERRORS_INCLUDE
