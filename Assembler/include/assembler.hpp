#ifndef ASSEMBLER_INCLUDE
#define ASSEMBLER_INCLUDE

#include "asm_utilities.hpp"
#include "../../Onegin/include/Onegin.hpp"
#include "../../Onegin/include/Sorting.hpp"

struct Assembler {
	int* code;
	size_t code_size;
	size_t pc;
};

const size_t REGISTER_NAME_LENGTH = 5;
const size_t MAX_COMMAND_LENGTH = 6;

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler);

AsmStatusCode GetArgs(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode GetNumber(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode GetRegister(String* string, Assembler* assembler, int cmd_len);

AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out);

#endif // ASSEMBLER_INCLUDE
