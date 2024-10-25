#ifndef ARRAY_INCLUDE
#define ARRAY_INCLUDE

#include "enum.hpp"

#define DEF_CMD_(cmd, ...) {CMD_ ## cmd, #cmd},

struct ArrayCommand {
	Commands cmd_code;
	const char* cmd_name;
};

const ArrayCommand array_commands[] = {
	#include "commands"
};

#endif // ARRAY_INCLUDE
