#include "spu.hpp"

int main(int argc, char* argv[]) {

	SPUStatusCode spu_status = SPU_NO_ERROR;

	if (argc < 2)
		SPU_ERROR_DEMO(SPU_NO_FILE_TO_RUN);

	SPU processor = {};

	spu_status = SPUCtor(&processor, argv[1]);
	SPU_ERROR_DEMO(spu_status);

	spu_status = SPURun(&processor);
	SPU_ERROR_DEMO(spu_status);

	spu_status = SPUDtor(&processor);
	SPU_ERROR_DEMO(spu_status);

	return 0;
}
