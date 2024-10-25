#include "assembler.hpp"

int main(int argc, char* argv[]) {

	if (argc < 2)
		ASM_ERROR_DEMO(ASM_ERROR_AMOUNT_OF_ARGS);

	OneginStatusCode onegin_status = ONEGIN_NO_ERROR;
	AsmStatusCode asm_status = ASM_NO_ERROR;

	Storage storage = {};
	Assembler assembler = { .files.asm_file = argv[1] };

	onegin_status = StorageFiller(&storage, argv[1]);
	ONEGIN_ERROR_CHECK(onegin_status);

	asm_status = StorageAssembler(&storage, &assembler);
	ASM_ERROR_DEMO(asm_status);

	asm_status = CodePrinter(&assembler, assembler.files.bin_file);
	ASM_ERROR_DEMO(asm_status);

	return 0;
}
