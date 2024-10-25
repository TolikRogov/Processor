#include "labels.hpp"
#include "assembler.hpp"

AsmStatusCode AsmLabels(Assembler* assembler) {

	fprintf(assembler->listing, "Labels: \n");

	for (size_t i = 0; i < assembler->labels_table.label_size; i++) {
		fprintf(assembler->listing, "\t%" ALIGNMENT "s" " #%zu, addr %d\n",
				assembler->labels_table.labels[i].name, i + 1,
				assembler->labels_table.labels[i].addr);
	}
	fprintf(assembler->listing, "\n");

	fprintf(assembler->listing, "FixUps: \n");

	for (size_t i = 0; i < assembler->labels_table.fixup_size; i++) {
		*(assembler->code + assembler->labels_table.undef_labels[i].pc - 1) =
		assembler->labels_table.labels[assembler->labels_table.undef_labels[i].label_num - 1].addr;
		fprintf(assembler->listing, "\t%" ALIGNMENT "s" " #%zu, addr %d, fix %zu\n",
				assembler->labels_table.labels[assembler->labels_table.undef_labels[i].label_num - 1].name,
				assembler->labels_table.undef_labels[i].label_num,
				assembler->labels_table.labels[assembler->labels_table.undef_labels[i].label_num - 1].addr,
				assembler->labels_table.undef_labels[i].pc - 1);
	}
	fprintf(assembler->listing, "\n");

	return ASM_NO_ERROR;
}

AsmStatusCode LabelCheck(Assembler* assembler, char* string) {

	AsmStatusCode asm_status = ASM_NO_ERROR;

	asm_status = FindCharInString(string, ':');
	if (asm_status != ASM_NO_ERROR)
		return ASM_UNDEFINE_LABEL;

	asm_status = IncreaseLabels(&assembler->labels_table);
	ASM_ERROR_DEMO(asm_status);

	int label_index = FindLabelInTable(assembler, string);
	if (label_index > -1) {
		assembler->labels_table.labels[label_index].addr = (int)assembler->pc;
		return ASM_NO_ERROR;
	}

	for (size_t j = 0; *(string + j) != '\0'; j++)
		assembler->labels_table.labels[assembler->labels_table.label_size].name[j] = string[j];
	assembler->labels_table.labels[assembler->labels_table.label_size++].addr = (int)assembler->pc;

	return ASM_NO_ERROR;
}

AsmStatusCode IncreaseLabels(LabelsTable* labels_table) {

	if (labels_table->capacity == labels_table->label_size ||
		labels_table->capacity == labels_table->fixup_size) {
		printf("capacity: %zu, size: %zu\n", labels_table->capacity, labels_table->label_size);
		labels_table->labels = (Label*)realloc(labels_table->labels,
											  (labels_table->capacity *= 2) * sizeof(Label));
		if (!labels_table->labels)
			ASM_ERROR_DEMO(ASM_ALLOC_ERROR);

		labels_table->undef_labels = (FixUp*)realloc(labels_table->undef_labels,
													 labels_table->capacity * sizeof(FixUp));
		if (!labels_table->undef_labels)
			ASM_ERROR_DEMO(ASM_ALLOC_ERROR);
		printf("capacity: %zu, size: %zu\n", labels_table->capacity, labels_table->label_size);
	}

	return ASM_NO_ERROR;
}

AsmStatusCode LabelStatus(Assembler* assembler, char* label) {

	LabelsTable* labels_table = &assembler->labels_table;

	int label_index = FindLabelInTable(assembler, label);
	if (label_index > -1 && labels_table->labels[label_index].addr != -1) {
		*(assembler->code + assembler->pc++) = labels_table->labels[label_index].addr;
		return ASM_NO_ERROR;
	}

	if (label_index < 0) {
		for (size_t i = 0; *(label + i) != '\0'; i++)
			labels_table->labels[labels_table->label_size].name[i] = label[i];
		label_index = (int)labels_table->label_size++;
	}
	*(assembler->code + assembler->pc++) = labels_table->labels[label_index].addr = -1;

	FixUp* cur_fix_up = &labels_table->undef_labels[labels_table->fixup_size++];

	cur_fix_up->label_num = (size_t)label_index + 1;
	cur_fix_up->pc = assembler->pc;

	return ASM_UNDEFINE_LABEL;
}

int FindLabelInTable(Assembler* assembler, char* label) {

	for (int i = 0; i < (int)assembler->labels_table.label_size; i++) {
		if (StrCmp(assembler->labels_table.labels[i].name, label) == 0)
			return i;
	}

	return -1;
}
