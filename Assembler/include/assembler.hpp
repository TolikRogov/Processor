#ifndef ASSEMBLER_INCLUDE
#define ASSEMBLER_INCLUDE

#include "asm_utilities.hpp"

const size_t DEFAULT_LABELS_CAPACITY = 5;
const size_t MAX_LABEL_LENGTH 		 = 32;

struct AsmFiles {
	const char* asm_file;
	char bin_file[MAX_FILE_NAME_SIZE];
	char listing_file[MAX_FILE_NAME_SIZE];
};

struct FixUp {
	size_t pc;
	size_t label_num;
};

struct Label {
	char name[MAX_LABEL_LENGTH];
	int addr;
};

struct LabelsTable {
	Label* labels;
	size_t label_size;

	size_t capacity = DEFAULT_LABELS_CAPACITY;

	FixUp* undef_labels;
	size_t fixup_size;
};

struct Assembler {
	McHeader header;

	int* code;
	size_t pc;

	LabelsTable labels_table;
	FILE* listing;
	AsmFiles files;
};

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler);
AsmStatusCode AssemblerCtor(Storage* storage, Assembler* assembler);
AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out);

#endif // ASSEMBLER_INCLUDE
