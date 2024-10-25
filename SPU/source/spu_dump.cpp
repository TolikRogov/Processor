#include "spu_dump.hpp"

SPUStatusCode SPUDump(SPU* proc, size_t pc) {

	for (size_t i = 0; i < 100; i++)
		printf("-");
	printf("\n");

	for (size_t i = 0; i < proc->size; i++) {
		printf("%.3zu: " "%.3d ", i, *(proc->code + i));
		if (i == pc)
			printf(YELLOW("<"));
		printf("\n");
	}
	printf("\n");

	printf("pc = %zu\n\n", pc);

	printf("Registers: ");
	printf("%cX = %d ", 'X', proc->registers[0]);
	for (size_t i = 1; i < MAX_REG_AMOUNT; i++)
		printf("%cX = %d ", 'A' + (int)i - 1, proc->registers[i]);
	printf("\n\n");

	printf("RAM: \n");
	for (size_t i = 0; i < RAM_SIZE;) {
		for (size_t j = 0; j < DRAW_WIDTH; j++) {
			printf("%.3zu: %.3d | ", i, proc->ram[i]);
			i++;
		}
		printf("\n");
	}

	for (size_t i = 0; i < 100; i++)
		printf("-");
	printf("\n");

	getchar();

	return SPU_NO_ERROR;
}

SPUStatusCode LogDump(SPU* proc) {

	FILE* log = fopen("log.log", "w");
	if (!log)
		SPU_ERROR_DEMO(SPU_FILE_OPEN_ERROR);

	for (size_t i = 0; i < 4 * proc->size; i++)
		fprintf(log, "-");
	fprintf(log, "\n");

	for (size_t i = 0; i < proc->size; i++)
		fprintf(log, "%.3zu ", i);
	fprintf(log, "\n");

	for (size_t i = 0; i < proc->size; i++)
		fprintf(log, "%.3d ", *(proc->code + i));
	fprintf(log, "\n");

	fprintf(log, "Registers: ");
	fprintf(log, "%cX = %d ", 'X', proc->registers[0]);
	for (size_t i = 1; i < MAX_REG_AMOUNT; i++)
		fprintf(log, "%cX = %d ", 'A' + (int)i - 1, proc->registers[i]);
	fprintf(log, "\n");

	fprintf(log, "RAM: \n");
	for (size_t i = 0; i < RAM_SIZE;) {
		for (size_t j = 0; j < DRAW_WIDTH; j++) {
			fprintf(log, "%.3zu: %.3d | ", i, proc->ram[i]);
			i++;
		}
		fprintf(log, "\n");
	}

	for (size_t i = 0; i < 4 * proc->size; i++)
		fprintf(log, "-");
	fprintf(log, "\n");

	if (fclose(log))
		SPU_ERROR_DEMO(SPU_FILE_CLOSE_ERROR);

	return SPU_NO_ERROR;
}
