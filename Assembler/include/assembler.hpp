#ifndef ASSEMBLER_INCLUDE
#define ASSEMBLER_INCLUDE

#include "asm_utilities.hpp"
#include "../../Onegin/include/Onegin.hpp"
#include "../../Onegin/include/Sorting.hpp"

const size_t MAX_SIGNATURE_LENGTH 			 = 20;
const size_t CODE_VERSION 					 = 2;
const char 	 SIGNATURE[MAX_SIGNATURE_LENGTH] = "TGF";

struct McHeader {
	const char* signature = SIGNATURE;
	size_t code_version   = CODE_VERSION;
};

struct Assembler {
	McHeader header;

	int* code;
	size_t code_size;
	size_t pc;
};

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler);

AsmStatusCode GetArgs(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode GetNumber(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode GetRegister(String* string, Assembler* assembler, int cmd_len);

AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out);

#endif // ASSEMBLER_INCLUDE
