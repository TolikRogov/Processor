#include "spu.hpp"

StackStatusCode SPURun() {

	INIT_STACK(stk);

	STACK_CTOR(&stk, 1);

	while(1) {

	}

	STACK_DTOR(&stk);

	return STACK_NO_ERROR;
}
