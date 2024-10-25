#include "asm_dump.hpp"

AsmStatusCode ListingHeader(Assembler* assembler) {

	FILE* lst = assembler->listing;

	fprintf(lst, "Signature: %s\n", (char*)&assembler->header.signature);
	fprintf(lst, "Version: %zu\n", assembler->header.code_version);

	LISTING_HEADER_PRINT(lst, "LINE");
	LISTING_HEADER_PRINT(lst, "CMD_NAME");
	LISTING_HEADER_PRINT(lst, "ARG_NAME");
	LISTING_HEADER_PRINT(lst, "CMD_CODE");
	LISTING_HEADER_PRINT(lst, "CMD_HEX");
	LISTING_HEADER_PRINT(lst, "ARG_CODE");
	LISTING_HEADER_PRINT(lst, "ARG_HEX");
	fprintf(lst, "\n");

	return ASM_NO_ERROR;
}

AsmStatusCode AsmDump(Assembler* assembler, const char* string) {

	for (size_t i = 0; i < 5 * assembler->pc && i < 150; i++)
		printf("-");
	printf("\n");

	for (size_t i = 0; i < assembler->pc; i++)
		printf("%.4zu ", i);
	printf("\n");

	for (size_t i = 0; i < assembler->pc; i++)
		printf("0x%.2x ", *(assembler->code + i));
	printf("\n");

	for (size_t i = 0; i < assembler->pc; i++)
		printf("     ");
	printf(YELLOW("^")"\n");

	printf("pc: %zu\n", assembler->pc);
	printf("current cmd string: %s\n", string);

	printf("\nLabels: %zu\n", assembler->labels_table.label_size);
	for (size_t i = 0; i < assembler->labels_table.label_size; i++)
		printf("label #%zu: name - %" ALIGNMENT "s, addr - %d\n", i + 1, assembler->labels_table.labels[i].name,
																		 assembler->labels_table.labels[i].addr);
	printf("\n");

	printf("\nFixUp:\n");
	for (size_t i = 0; i < assembler->labels_table.fixup_size; i++)
		printf("label #%zu: addr - %zu, num - %zu\n", i + 1, assembler->labels_table.undef_labels[i].pc,
															 assembler->labels_table.undef_labels[i].label_num);
	printf("\n");

	for (size_t i = 0; i < 5 * assembler->pc && i < 150; i++)
		printf("-");
	printf("\n");

	getchar();

	return ASM_NO_ERROR;
}
