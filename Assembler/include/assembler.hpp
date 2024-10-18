#ifndef ASSEMBLER_INCLUDE
#define ASSEMBLER_INCLUDE

#include "asm_utilities.hpp"
#include "../../Onegin/include/Onegin.hpp"
#include "../../Onegin/include/Sorting.hpp"

const size_t DEFAULT_LABELS_CAPACITY		 = 5;
const size_t MAX_LABEL_LENGTH 				 = 10; // 32

const size_t MAX_SIGNATURE_LENGTH 			 = 10;
const size_t CODE_VERSION 					 = 2;
const char 	 SIGNATURE[MAX_SIGNATURE_LENGTH] = "TGF";

struct McHeader {
	const char* signature = SIGNATURE;
	size_t code_version   = CODE_VERSION;
};

struct FixUp {
	size_t pc;
	size_t label_num;
};

struct Label {
	char name[MAX_LABEL_LENGTH];
	size_t addr;
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
};

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler);

AsmStatusCode GetArgs(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode GetNumber(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode GetRegister(String* string, Assembler* assembler, int cmd_len);
AsmStatusCode GetLabel(String* string, Assembler* assembler, int cmd_len);

AsmStatusCode LabelStatus(Assembler* assembler, char* label);
AsmStatusCode FindLabelInTable(Assembler* assembler, char* label);
AsmStatusCode IncreaseLabels(Assembler* assembler);

AsmStatusCode AsmDump(Assembler* assembler, const char* string);

AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out);

#endif // ASSEMBLER_INCLUDE
