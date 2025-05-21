#include "assembler.hpp"
#include "labels.hpp"
#include "get_cmd_arg.hpp"
#include "asm_dump.hpp"

AsmStatusCode StorageAssembler(Storage* storage, Assembler* assembler) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	asm_status = AssemblerCtor(storage, assembler);
	ASM_ERROR_DEMO(asm_status);

	assembler->listing = fopen(assembler->files.listing_file, "w");
	if (!assembler->listing)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	ListingHeader(assembler);

	for (size_t i = 0; i < storage->str_cnt; i++) {

		char command[MAX_LABEL_LENGTH] = {};
		int cur_cmd_len = 0;
		int symbols_num = sscanf(storage->text[i].cur_str, "%s%n", command, &cur_cmd_len);
		if (symbols_num < 1)
			ASM_ERROR_DEMO(ASM_COMMAND_READ_ERROR);

		if (*command == '#')
			continue;

		FindAndReplaceCharInString(storage->text[i].cur_str, '#', '\0', NULL);

		asm_status = LabelCheck(assembler, command);
		if (asm_status == ASM_NO_ERROR)
			continue;

		StringToUpper(command);

		Commands opCode = (Commands)0;
		asm_status = GetCommand(command, &opCode);
		if (asm_status == ASM_SYNTAX_COMMAND_ERROR)
			ASM_ERROR_DEMO(asm_status);

		fprintf(assembler->listing, "%." ALIGNMENT "zu\t", i);
		fprintf(assembler->listing, "%" ALIGNMENT "s\t", command);

		*(assembler->code + assembler->pc) |= (unsigned char)opCode;
		MemoryUseCheck(&storage->text[i], assembler, cur_cmd_len);

		asm_status = GetArgs(&storage->text[i], assembler, cur_cmd_len);
		ASM_ERROR_DEMO(asm_status);

		fprintf(assembler->listing, "\n");

#ifdef ASM_DUMP
		asm_status = AsmDump(assembler, storage->text[i].cur_str);
		ASM_ERROR_DEMO(asm_status);
#endif
	}

	asm_status = AsmLabels(assembler);
	ASM_ERROR_DEMO(asm_status);

	assembler->header.code_size = assembler->pc;

	fprintf(assembler->listing, "Code size: %zu\n", assembler->header.code_size);

#ifdef ASM_DUMP
	asm_status = AsmDump(assembler, "");
	ASM_ERROR_DEMO(asm_status);
#endif

	if (fclose(assembler->listing))
		ASM_ERROR_DEMO(ASM_FILE_CLOSE_ERROR);

	return ASM_NO_ERROR;
}

AsmStatusCode AssemblerCtor(Storage* storage, Assembler* assembler) {

	ConvertFileToAnother(assembler->files.asm_file, assembler->files.bin_file, ".bin");
	ConvertFileToAnother(assembler->files.asm_file, assembler->files.listing_file, ".txt");

	assembler->header = { .signature = *(const long long*)SIGNATURE, .code_version = CODE_VERSION };

	assembler->labels_table.labels = (Label*)calloc(assembler->labels_table.capacity, sizeof(Label));
	if (!assembler->labels_table.labels)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	assembler->labels_table.undef_labels = (FixUp*)calloc(assembler->labels_table.capacity, sizeof(FixUp));
	if (!assembler->labels_table.undef_labels)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	assembler->code = (unsigned char*)calloc(storage->str_cnt, (sizeof(Command_t) + sizeof(Register_t) + sizeof(Immediate_t)));
	if (!assembler->code)
		ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

	return ASM_NO_ERROR;
};

AsmStatusCode AssemblerDtor(Assembler* assembler) {

	if (assembler->labels_table.undef_labels) {
		free(assembler->labels_table.undef_labels);
		assembler->labels_table.undef_labels = NULL;
	}

	if (assembler->labels_table.labels) {
		free(assembler->labels_table.labels);
		assembler->labels_table.labels = NULL;
	}

	if (assembler->code) {
		free(assembler->code);
		assembler->code = NULL;
	}

	return ASM_NO_ERROR;
}

AsmStatusCode CodePrinter(Assembler* assembler, const char* file_out) {

	FILE* bin = fopen(file_out, "wb");
	if (!bin)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	size_t written_check = fwrite(&assembler->header, sizeof(char), sizeof(McHeader), bin);
	if (written_check != sizeof(McHeader))
		ASM_ERROR_DEMO(ASM_FILE_WRITE_ERROR);

	written_check = fwrite(assembler->code, sizeof(unsigned char), assembler->pc, bin);
	if (written_check != assembler->pc)
		ASM_ERROR_DEMO(ASM_FILE_WRITE_ERROR);

	if (fclose(bin))
		ASM_ERROR_DEMO(ASM_FILE_CLOSE_ERROR);

	return ASM_NO_ERROR;
}
