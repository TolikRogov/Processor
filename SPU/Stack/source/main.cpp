#include "../include/STACK_LIBRARY.hpp"

int main() {

	INIT_STACK(stk);

	STACK_CTOR(&stk, 1);

	for (Stack_elem_t i = 0; i < 20; i++)
		STACK_PUSH(&stk, i);

	Stack_elem_t x = 0;
	for (Stack_elem_t i = 0; i < 20; i++)
		STACK_POP(&stk, &x);

	STACK_DTOR(&stk);

	return 0;
}
