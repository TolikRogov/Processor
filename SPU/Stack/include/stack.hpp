#ifndef STACK_INCLUDE
#define STACK_INCLUDE

#include "config.hpp"
#include "stack_utilities.hpp"

typedef double Stack_elem_t;
typedef u_int64_t Canary_t;
typedef u_int64_t Hash_t;

struct Stack_t {

#ifdef HASH_PROTECTION
	Hash_t hash;
#endif

#ifdef CANARY_PROTECTION
	Canary_t canary1;
#endif

	int status;

#ifdef HTML_DUMP
	StackLogInfo stack_info;
#endif

#ifdef HASH_PROTECTION
	Hash_t data_hash;
#endif

	Stack_elem_t* data;
	size_t size;
	size_t capacity;

	Dir log;

#ifdef CANARY_PROTECTION
	Canary_t canary2;
#endif
};

StackStatusCode DoStackCtor(Stack_t* stk, size_t capacity);
StackStatusCode DoStackCalloc(Stack_t* stk);
StackStatusCode DoStackReallocUp(Stack_t* stk);
StackStatusCode DoStackReallocDown(Stack_t* stk);
StackStatusCode DoStackVerify(Stack_t* stk);
StackStatusCode DoStackPush(Stack_t* stk, Stack_elem_t value);
StackStatusCode DoStackPop(Stack_t* stk, Stack_elem_t* value);
StackStatusCode DoStackDtor(Stack_t* stk);

StackStatusCode CheckerStackStatus(Stack_t* stk, const char* file, const char* func, const size_t line);
StackStatusCode VerifyHashAndStatus(Stack_t* stk);
StackStatusCode VerifyCanaries(Stack_t* stk);
Hash_t DJB2Hash(const void* array, const size_t size_in_bytes);

#endif //STACK_INCLUDE
