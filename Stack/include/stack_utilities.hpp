#ifndef STACK_UTILITIES_INCLUDE
#define STACK_UTILITIES_INCLUDE

#include "libraries.hpp"

enum StackStatusCode {
	STACK_NO_ERROR,
	STACK_ERROR,

	STACK_ALLOC_ERROR,

	STACK_FILE_OPEN_ERROR,
	STACK_FILE_CLOSE_ERROR,
	STACK_DIR_OPEN_ERROR,
	STACK_DIR_CLOSE_ERROR,

	STACK_DIR_DELETE_ERROR,
	STACK_DIR_MAKE_ERROR,
	STACK_RUN_HTML_ERROR,

	STACK_POINTER_ERROR,
	STACK_DATA_POINTER_ERROR,

	STACK_UNDERFLOW,
	STACK_SIZE_ERROR,
	STACK_CAPACITY_ERROR,
	STACK_DIMENSIONS_ERROR,

	STACK_POP_ERROR,

	STACK_LEFT_CANARY_ERROR,
	STACK_RIGHT_CANARY_ERROR,

	STACK_DATA_LEFT_CANARY_ERROR,
	STACK_DATA_RIGHT_CANARY_ERROR,

	STACK_HASH_ERROR,
	STACK_DATA_HASH_ERROR,

	STACK_STATUS_ERROR
};

struct File {
	const char* file_name;
	char* file_path;
};

struct Dir {
	const char* project_path = realpath(".", NULL);
	const char* dir_name = "/log_eblan/";
	char* dir_path;

	size_t cnt_files = 2;
	File files[2] = { {.file_name = "main.html"},
					 {.file_name = "table.html"} };

	size_t cnt_dirs = 1;
	struct ChildDir {
		const char* c_dir_name = "css/";
		char* c_dir_path;

		size_t c_dir_cnt_files = 1;
		File files[1] = { { .file_name = "style.css" } };
	} styles;
};

struct StackLogInfo {
	const char* stack_name;
	const char* file_name;
	const size_t line;
};

#endif //STACK_UTILITIES_INCLUDE
