#ifndef LOG_INCLUDE
#define LOG_INCLUDE

#include "stack.hpp"

enum HTML {
	MAIN,
	TABLE
};

enum STYLES {
	CSS
};

struct DumpInfo {
	const char* file;
	const size_t line;
	const char* func;
};

const char LOG_DELETE[] 	= "rm -rf ";
const char LOG_MAKE_DIR[] 	= "mkdir ";

#ifdef __APPLE__
	const char LOG_OPEN[]   = "open ";
#else
	const char LOG_OPEN[] 	= "xdg-open ";
#endif

StackStatusCode HtmlLogStarter(Stack_t* stk);

StackStatusCode DirCtor(Stack_t* stk);
StackStatusCode FilesCtor(Stack_t* stk);

StackStatusCode MakeDirsPaths(Stack_t* stk);
StackStatusCode MakeFilesPaths(Stack_t* stk);
StackStatusCode MakeDirsFolders(Stack_t* stk);
StackStatusCode DeleteLogDir(Stack_t* stk);

StackStatusCode CssLogStarter(Stack_t* stk);

StackStatusCode HtmlTableLog(Stack_t* stk);
StackStatusCode HtmlTableLogFinisher(Stack_t* stk);

StackStatusCode DoStackDumpMain(Stack_t* stk, DumpInfo dump_info);
StackStatusCode DoStackDumpTable(Stack_t* stk);

StackStatusCode HtmlLogFinisher(Stack_t* stk);
StackStatusCode RunMainHtmlFile(Stack_t* stk);

#endif //LOG_INCLUDE
