#include "STACK_LIBRARY.hpp"

int main() {

	INIT_STACK(stk);

	STACK_CTOR(&stk, 1);

	STACK_DTOR(&stk);

	return 0;
}
