#include "spu.hpp"

int main(int argc, char* argv[]) {

	SPUStatusCode status = SPU_NO_ERROR;

	SPU processor = { .file = (argc < 2 ? "data/default.txt" : argv[1]) };

	status = SPUCtor(&processor);
	SPU_ERROR_DEMO(status);

	status = SPURun(&processor);
	SPU_ERROR_DEMO(status);

	return 0;
}
