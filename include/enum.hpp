#ifndef ENUM_INCLUDE
#define ENUM_INCLUDE

#define DEF_CMD_(cmd, ...) CMD_ ## cmd,

enum Commands {
	#include "commands"
	COUNT_OF_COMMANDS
};

#undef DEF_CMD_

#endif // ENUM_INCLUDE
