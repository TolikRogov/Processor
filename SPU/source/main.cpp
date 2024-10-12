#include "spu.hpp"

int main(int argc, char* argv[]) {

	SPUStatusCode status = SPU_NO_ERROR;

	if (argc < 2)
		SPU_ERROR_DEMO(SPU_NO_FILE_TO_RUN);

	SPU processor = { .file = argv[1] };

	status = SPUCtor(&processor);
	SPU_ERROR_DEMO(status);

	status = SPURun(&processor);
	SPU_ERROR_DEMO(status);

	return 0;
}
