#include "assembler.hpp"
#include "labels.hpp"
#include "get_cmd_arg.hpp"
#include "asm_dump.hpp"

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	FILE* listing = fopen("../data/listing.txt", "w");
	if (!listing)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	asm_status = AssemblerCtor(storage, assembler);
	ASM_ERROR_DEMO(asm_status);

	assembler->listing = listing;
	ListingHeader(assembler);

	for (size_t i = 0; i < storage->str_cnt; i++) {

		char command[MAX(MAX_COMMAND_LENGTH, MAX_LABEL_LENGTH)] = {};
		int cur_cmd_len = 0;
		int symbols_num = sscanf(storage->text[i].cur_str, "%s%n", command, &cur_cmd_len);
		if (symbols_num < 1)
			ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

		StringToLower(command);

		if (*command == '#')
			continue;

#ifdef ASM_DUMP
		asm_status = AsmDump(assembler, storage->text[i].cur_str);
		ASM_ERROR_DEMO(asm_status);
#endif

		asm_status = LabelCheck(assembler, command);
		if (asm_status == ASM_NO_ERROR)
			continue;

		Commands opCode = (Commands)0;
		asm_status = GetCommand(command, &opCode);
		if (asm_status == ASM_SYNTAX_COMMAND_ERROR)
			ASM_ERROR_DEMO(asm_status);

		fprintf(listing, "%." ALIGNMENT "zu\t", i);
		fprintf(listing, "%" ALIGNMENT "s\t", command);

		*(assembler->code + assembler->pc) |= opCode;
		MemoryUseCheck(&storage->text[i], assembler, cur_cmd_len);
		asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
		ASM_ERROR_DEMO(asm_status);

		fprintf(listing, "\n");
	}

	asm_status = AsmLabels(assembler);
	ASM_ERROR_DEMO(asm_status);

	assembler->header.code_size = assembler->pc;

	fprintf(listing, "Code size: %zu\n", assembler->header.code_size);

#ifdef ASM_DUMP
	asm_status = AsmDump(assembler, "");
	ASM_ERROR_DEMO(asm_status);
#endif

	if (fclose(listing))
		ASM_ERROR_DEMO(ASM_FILE_CLOSE_ERROR);

	return ASM_NO_ERROR;
}

AsmStatusCode AssemblerCtor(Storage* storage, Assembler* assembler) {

	assembler->header = { .signature = *(const long long*)SIGNATURE, .code_version = CODE_VERSION };

	assembler->labels_table.labels = (Label*)calloc(assembler->labels_table.capacity, sizeof(Label));
	if (!assembler->labels_table.labels)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	assembler->labels_table.undef_labels = (FixUp*)calloc(assembler->labels_table.capacity, sizeof(FixUp));
	if (!assembler->labels_table.undef_labels)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	assembler->code = (int*)calloc(storage->str_cnt * 2, sizeof(int));
	if (!assembler->code)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	return ASM_NO_ERROR;
};

AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out) {

	FILE* bin = fopen(file_out, "wb");
	if (!bin)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	size_t written_check = fwrite(&assembler->header, sizeof(char), sizeof(McHeader), bin);
	if (written_check != sizeof(McHeader))
		ASM_ERROR_DEMO(ASM_FILE_WRITE_ERROR);

	written_check = fwrite(assembler->code, sizeof(int), assembler->pc, bin);
	if (written_check != assembler->pc)
		ASM_ERROR_DEMO(ASM_FILE_WRITE_ERROR);

	if (fclose(bin))
		ASM_ERROR_DEMO(ASM_FILE_CLOSE_ERROR);

	return ASM_NO_ERROR;
}
