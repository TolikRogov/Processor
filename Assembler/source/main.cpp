#include "assembler.hpp"

int main(int argc, const char* argv[]) {

	const char* file = (argc < 2 ? "../data/default.txt" : argv[1]);

	FILE* input = fopen(file, "r");
	if (!input)
		ASM_ERROR_DEMO(ASM_FILE_OPEN_ERROR);

	if (!fclose(input))

	return 0;
}
