#ifndef GET_CMD_ARG_INCLUDE
#define GET_CMD_ARG_INCLUDE

#include "assembler.hpp"

AsmStatusCode GetCommand(const char* operation, Commands* opCode);
AsmStatusCode GetArgs(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode GetNumber(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode GetRegister(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode GetLabel(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode MemoryUseCheck(String* string, Assembler* assembler, int cmd_len);

#endif //GET_CMD_ARG_INCLUDE
