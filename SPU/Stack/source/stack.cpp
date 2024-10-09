#include "../include/STACK_LIBRARY.hpp"

#ifdef HASH_PROTECTION
static StackStatusCode DoStackHash(Stack_t* stk) {

	if (!stk)
		STACK_ERROR_CHECK(STACK_POINTER_ERROR, stk);

	if (!stk->data)
		STACK_ERROR_CHECK(STACK_DATA_POINTER_ERROR, stk);

	#ifdef CANARY_PROTECTION
		size_t size = stk->capacity * sizeof(Stack_elem_t) + 2 * sizeof(Canary_t) + 2 * sizeof(Canary_t) % ALIGNMENT;
		stk->data_hash = DJB2Hash((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT,
								  size + (ALIGNMENT - size % ALIGNMENT));
	#else
		stk->data_hash = DJB2Hash(stk->data, stk->capacity * sizeof(Stack_elem_t));
	#endif

	stk->hash = DJB2Hash(stk + sizeof(Hash_t), sizeof(Stack_t));

	return STACK_NO_ERROR;
}
#endif

StackStatusCode DoStackCtor(Stack_t* stk, size_t capacity) {

	const size_t MAX_SIZE = 1e10;

	StackStatusCode status = STACK_NO_ERROR;

	if (!stk)
		STACK_ERROR_CHECK(STACK_POINTER_ERROR, stk);

	if (capacity > MAX_SIZE || !capacity)
		STACK_ERROR_CHECK(STACK_CAPACITY_ERROR, stk);

#ifdef HTML_DUMP
	status = HtmlLogStarter(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

#ifdef CANARY_PROTECTION
	stk->canary1 = STACK_CANARY_HEX;
	stk->canary2 = STACK_CANARY_HEX;
#endif

	stk->capacity = (capacity < DEFAULT_CAPACITY && capacity != 0 ? DEFAULT_CAPACITY : capacity);

	status = DoStackCalloc(stk);
	STACK_ERROR_CHECK(status, stk);

	stk->size = 0;

#ifdef HASH_PROTECTION
	status = DoStackHash(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

	STACK_VERIFY(stk);

	return STACK_NO_ERROR;
}

StackStatusCode DoStackCalloc(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

#ifdef CANARY_PROTECTION
	size_t size = stk->capacity * sizeof(Stack_elem_t) + 2 * sizeof(Canary_t);
	stk->data = (Stack_elem_t*)((char*)calloc(1, size + (ALIGNMENT - size % ALIGNMENT)) + sizeof(Canary_t));
	if (!stk->data)
		STACK_ERROR_CHECK(STACK_ALLOC_ERROR, stk);

	size_t data_size = stk->capacity * sizeof(Stack_elem_t);
	*(Canary_t*)((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT) = DATA_CANARY_HEX;
	*(Canary_t*)((char*)stk->data + data_size + (ALIGNMENT - data_size % ALIGNMENT)) = DATA_CANARY_HEX;
#else
	if (stk->capacity != 0) {
		stk->data = (Stack_elem_t*)calloc(stk->capacity, sizeof(Stack_elem_t));
		if (!stk->data)
			STACK_ERROR_CHECK(STACK_ALLOC_ERROR, stk);
	}
#endif

#ifdef HASH_PROTECTION
	status = DoStackHash(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

	status = CustomMemsetDouble(stk->data, stk->capacity, POISON);
	STACK_ERROR_CHECK(status, stk);

#ifdef HASH_PROTECTION
	status = DoStackHash(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

	return STACK_NO_ERROR;
}

StackStatusCode DoStackPush(Stack_t* stk, Stack_elem_t value) {

	StackStatusCode status = STACK_NO_ERROR;

	STACK_VERIFY(stk);

	if (stk->size == stk->capacity) {
		status = DoStackReallocUp(stk);
		STACK_ERROR_CHECK(status, stk);
	}

	STACK_VERIFY(stk);

	*(stk->data + stk->size++) = value;

#ifdef HASH_PROTECTION
	status = DoStackHash(stk);
	STACK_ERROR_CHECK(status, stk)
#endif

	STACK_VERIFY(stk);

	return STACK_NO_ERROR;
}

StackStatusCode DoStackReallocUp(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

#ifdef CANARY_PROTECTION
	size_t size = (stk->capacity *= 2) * sizeof(Stack_elem_t) + 2 * sizeof(Canary_t);
	stk->data = (Stack_elem_t*)((char*)realloc((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT,
											   size + (ALIGNMENT - size % ALIGNMENT)) + sizeof(Canary_t));

	size_t data_size = stk->capacity * sizeof(Stack_elem_t);
	*(Canary_t*)((char*)stk->data + data_size + (ALIGNMENT - data_size % ALIGNMENT)) = DATA_CANARY_HEX;
#else
	stk->data = (Stack_elem_t*)realloc(stk->data, (stk->capacity *= 2) * sizeof(Stack_elem_t));
#endif

	if (!stk->data)
		STACK_ERROR_CHECK(STACK_ALLOC_ERROR, stk);

#ifdef HASH_PROTECTION
	status = DoStackHash(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

	status = CustomMemsetDouble(stk->data + stk->size, stk->capacity - stk->size, POISON);
	STACK_ERROR_CHECK(status, stk);

#ifdef HASH_PROTECTION
	status = DoStackHash(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

	return STACK_NO_ERROR;
}

StackStatusCode DoStackPop(Stack_t* stk, Stack_elem_t* value) {

	StackStatusCode status = STACK_NO_ERROR;

	STACK_VERIFY(stk);

	if ((4 * stk->size < stk->capacity && stk->size > DEFAULT_CAPACITY) ||
		(stk->size <= DEFAULT_CAPACITY && stk->capacity > DEFAULT_CAPACITY)) {
		status = DoStackReallocDown(stk);
		STACK_ERROR_CHECK(status, stk);
	}

	STACK_VERIFY(stk);

	if (stk->size == 0)
		STACK_ERROR_CHECK(STACK_POP_ERROR, stk);

	*value = *(stk->data + (--stk->size));
	*(stk->data + stk->size) = POISON;

#ifdef HASH_PROTECTION
	status = DoStackHash(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

	STACK_VERIFY(stk);

	return STACK_NO_ERROR;
}

StackStatusCode DoStackReallocDown(Stack_t* stk) {

#ifdef CANARY_PROTECTION
	size_t size = (stk->capacity = (stk->size < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : stk->size)) * sizeof(Stack_elem_t) + 2 * sizeof(Canary_t);
	stk->data = (Stack_elem_t*)((char*)realloc((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT,
											   size + (ALIGNMENT - size % ALIGNMENT)) + sizeof(Canary_t));

	size_t data_size = stk->capacity * sizeof(Stack_elem_t);
	*(Canary_t*)((char*)stk->data + data_size + (ALIGNMENT - data_size % ALIGNMENT)) = DATA_CANARY_HEX;
#else
	stk->data = (Stack_elem_t*)realloc(stk->data, (stk->capacity = stk->size) * sizeof(Stack_elem_t));
#endif
	if (!stk->data)
		STACK_ERROR_CHECK(STACK_ALLOC_ERROR, stk);

#ifdef HASH_PROTECTION
	StackStatusCode status = DoStackHash(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

	return STACK_NO_ERROR;
}

StackStatusCode DoStackVerify(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	if (!stk)
		STACK_ERROR_CHECK(STACK_POINTER_ERROR, stk);

	status = VerifyHashAndStatus(stk);
	STACK_ERROR_CHECK(status, stk);

	if (!stk->capacity && !stk->size)
		stk->status |= (1 << STACK_UNDERFLOW);

	if (!stk->data)
		stk->status |= (1 << STACK_DATA_POINTER_ERROR);

	if (stk->capacity < 0)
		stk->status |= (1 << STACK_CAPACITY_ERROR);

	if (stk->size < 0)
		stk->status |= (1 << STACK_SIZE_ERROR);

	if (stk->size > stk->capacity)
		stk->status |= (1 << STACK_DIMENSIONS_ERROR);

#ifdef CANARY_PROTECTION
	status = VerifyCanaries(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

	for (size_t i = 0; i < sizeof(stk->status) * 8; i++)
		if (((1 << i) & stk->status) >> i)
			return STACK_ERROR;

#ifdef HASH_PROTECTION
	status = DoStackHash(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

	return STACK_NO_ERROR;
}

StackStatusCode DoStackDtor(Stack_t* stk) {

	if (!stk)
		STACK_ERROR_CHECK(STACK_POINTER_ERROR, stk);

#ifdef HASH_PROTECTION
	stk->hash 		= (Hash_t)TRASH;
	stk->data_hash 	= (Hash_t)TRASH;
#endif

#ifdef CANARY_PROTECTION
	stk->canary1 = (Canary_t)TRASH;
	stk->canary2 = (Canary_t)TRASH;
#endif

	stk->capacity = TRASH;
	stk->size 	  = TRASH;

	if (stk->data && ((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT)) {

#ifdef CANARY_PROTECTION
		char* alloc_memory = ((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT);
#else
		char* alloc_memory = (char*)stk->data;
#endif

		free(alloc_memory);
		alloc_memory = NULL;

#ifdef N_DEBUG
		printf("\n\n" GREEN("ALLOCATED MEMORY FREED") "\n\n");
#endif

	}

#ifdef HTML_DUMP
	StackStatusCode status = HtmlLogFinisher(stk);
	STACK_ERROR_CHECK(status, stk);
#endif

	stk->log = {};

	return STACK_NO_ERROR;
}

StackStatusCode VerifyHashAndStatus(Stack_t* stk) {

	int stack_status = 0;

#ifdef HASH_PROTECTION
	if (stk->hash != DJB2Hash(stk + sizeof(Hash_t), sizeof(Stack_t)))
		stack_status |= (1 << STACK_HASH_ERROR);

	#ifdef CANARY_PROTECTION
		size_t size = stk->capacity * sizeof(Stack_elem_t) + 2 * sizeof(Canary_t);
		Hash_t data_hash = DJB2Hash((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT, size + (ALIGNMENT - size % ALIGNMENT));
	#else
		Hash_t data_hash = DJB2Hash(stk->data, stk->capacity * sizeof(Stack_elem_t));
	#endif
	if (stk->data_hash != data_hash)
		stack_status |= (1 << STACK_DATA_HASH_ERROR);
#endif

	if (stk->status) {
		stk->status = 0;
		stack_status |= (1 << STACK_STATUS_ERROR);
	}

	stk->status |= stack_status;

	return STACK_NO_ERROR;
}

StackStatusCode VerifyCanaries(Stack_t* stk) {

	if (!stk)
		STACK_ERROR_CHECK(STACK_POINTER_ERROR, stk);

#ifdef CANARY_PROTECTION
	if (!CompareDouble(stk->canary1, STACK_CANARY_HEX))
		stk->status |= (1 << STACK_LEFT_CANARY_ERROR);

	if (!CompareDouble(stk->canary2, STACK_CANARY_HEX))
		stk->status |= (1 << STACK_RIGHT_CANARY_ERROR);

	if (stk->data) {
		if (!CompareDouble(*(Canary_t*)((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT), DATA_CANARY_HEX))
			stk->status |= (1 << STACK_DATA_LEFT_CANARY_ERROR);

		size_t data_size = stk->capacity * sizeof(Stack_elem_t);
		if (!CompareDouble(*(Canary_t*)((char*)stk->data + data_size + (ALIGNMENT - data_size % ALIGNMENT)), DATA_CANARY_HEX))
			stk->status |= (1 << STACK_DATA_RIGHT_CANARY_ERROR);
	}
#endif

	return STACK_NO_ERROR;
}

StackStatusCode CheckerStackStatus(Stack_t* stk, const char* file, const char* func, const size_t line) {

	if (!stk)
		return STACK_POINTER_ERROR;

	for (size_t i = 0; i < sizeof(stk->status) * 8; i++) {
		if (((1 << i) & stk->status) >> i)
			fprintf(stderr, "\n\n" RED("Error (code %zu): %s, ") YELLOW("File: %s, Function: %s, Line: %zu\n\n"),
					i, StackErrorsMessenger((StackStatusCode)(i)), file, func, line);
	}

	return STACK_NO_ERROR;
}

Hash_t DJB2Hash(const void* array, const size_t size_in_bytes) {

	Hash_t hash = 5381;

	for (size_t i = 0; i < size_in_bytes; i++)
		hash = hash * 33 ^ (Hash_t)(*((const char*)array + i));

	return hash;
}
