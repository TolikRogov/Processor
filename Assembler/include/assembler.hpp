#ifndef ASSEMBLER_INCLUDE
#define ASSEMBLER_INCLUDE

#include "asm_utilities.hpp"
#include "../../Onegin/include/Onegin.hpp"
#include "../../Onegin/include/Sorting.hpp"

struct Assembler {
	int* code;
	size_t code_size;
};

const size_t MAX_COMMAND_LENGTH = 20;

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler);

#endif // ASSEMBLER_INCLUDE
