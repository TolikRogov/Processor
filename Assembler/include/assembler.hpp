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
const size_t CODE_VERSION = 2;

const char SIGNATURE[] = "TGF";

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler);
AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out);

#endif // ASSEMBLER_INCLUDE
