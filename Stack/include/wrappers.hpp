#ifndef WRAPPERS_INCLUDE
#define WRAPPERS_INCLUDE

#define RED(str) 		"\033[31;1m" str "\033[0m"
#define YELLOW(str) 	"\033[33;4m" str "\033[0m"
#define GREEN(str) 		"\033[32;6m" str "\033[0m"

#ifdef HTML_DUMP
#define INIT_STACK(stk) Stack_t stk = { .stack_info = {#stk, __FILE__, __LINE__}}
#else
#define INIT_STACK(stk) Stack_t stk = {}
#endif

#ifdef RETURN_ERRORS
	#define RETURN_STATUS(...) __VA_ARGS__
#else
	#define RETURN_STATUS(...)
#endif

#define STACK_ERROR_CHECK(status, stk) {																	 	 		 \
	if (status != STACK_NO_ERROR) {																						\
		CheckerStackStatus(stk, __FILE__, __PRETTY_FUNCTION__, __LINE__);												\
		if (status != STACK_ERROR)																						\
			fprintf(stderr, "\n\n" RED("Error (code %d): %s, ") YELLOW("File: %s, Function: %s, Line: %d\n\n"),   		\
					status, StackErrorsMessenger(status), __FILE__, __PRETTY_FUNCTION__, __LINE__);						\
		if (!(fclose(stderr)))																							\
			DoStackDtor(stk);																							\
		RETURN_STATUS(return status;)																					\
	}																													\
}

#ifdef HTML_DUMP
#define DUMP(stk, func_name) {				 	  		 										 				 \
	status = DoStackDumpMain(stk, {.file = __FILE__, .line = __LINE__, .func = func_name});	 			 		 \
	STACK_ERROR_CHECK(status, stk);																				\
	status = DoStackDumpTable(stk);																				\
	STACK_ERROR_CHECK(status, stk);																				\
}
#else
#define DUMP(stk, func)
#endif

#define STACK_CTOR(stk, capacity) {					 	 			 \
	StackStatusCode status = DoStackCtor(stk, (size_t)capacity);	\
	STACK_ERROR_CHECK(status, stk);									\
	DUMP(stk, "STACK_CTOR");										\
}

#define STACK_PUSH(stk, value) {		 						 	 \
	StackStatusCode status = DoStackPush(stk, value);				\
	STACK_ERROR_CHECK(status, stk);									\
	DUMP(stk, "STACK_PUSH");										\
}

#define STACK_POP(stk, var_pointer) {		 				 		 \
	StackStatusCode status = DoStackPop(stk, var_pointer); 			\
	STACK_ERROR_CHECK(status, stk);									\
	DUMP(stk, "STACK_POP");											\
}

#define STACK_VERIFY(stk) {					 				 		 \
	status = DoStackVerify(stk);									\
	STACK_ERROR_CHECK(status, stk);									\
}

#define STACK_DTOR(stk) {					 				 		 \
	StackStatusCode status = DoStackDtor(stk);						\
	STACK_ERROR_CHECK(status, stk);									\
}

#endif //WRAPPERS_INCLUDE
