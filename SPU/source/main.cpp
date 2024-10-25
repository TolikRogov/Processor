#include "spu.hpp"

// TODO:
// CODE_GEN (double* GetArg, Jmp без копипаста)
// квадратка
// Неравномерный код
//  потом уже попробовать ебануть кружок нормально
//  потом можно под разными флагами сделать листинги (--IntelEdition и --ChursinEdition)
//  Bad Apple
// после Bad Apple можно добавить, чтобы буквы печатались, просто пушить в стек аски коды

int main(int argc, char* argv[]) {

	SPUStatusCode status = SPU_NO_ERROR;

	if (argc < 2)
		SPU_ERROR_DEMO(SPU_NO_FILE_TO_RUN);

	SPU processor = {};

	status = SPUCtor(&processor, argv[1]);
	SPU_ERROR_DEMO(status);

	status = SPURun(&processor);
	SPU_ERROR_DEMO(status);

	return 0;
}
