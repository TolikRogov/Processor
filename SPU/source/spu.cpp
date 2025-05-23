#include "spu.hpp"
#include "spu_dump.hpp"

SPUStatusCode SPUCtor(SPU* proc, const char* file) {

	SPUStatusCode spu_status = SPU_NO_ERROR;

	FILE* input = fopen(file, "rb");
	if (!input)
		SPU_ERROR_DEMO(SPU_FILE_OPEN_ERROR);

	STACK_CTOR(&proc->stk, 1);
	STACK_CTOR(&proc->ret_addr_stk, 1);

	spu_status = CodeHeaderChecker(proc, input);
	SPU_ERROR_DEMO(spu_status);

	size_t read_check = fread(proc->code, sizeof(unsigned char), proc->size, input);
	if (read_check != proc->size)
		SPU_ERROR_DEMO(SPU_FILE_READ_ERROR);

	if (fclose(input))
		SPU_ERROR_DEMO(SPU_FILE_CLOSE_ERROR);

	return SPU_NO_ERROR;
}

SPUStatusCode SPUDtor(SPU* proc) {

	if (proc->code) {
		free(proc->code);
		proc->code = NULL;
	}

	DoStackDtor(&proc->stk);
	DoStackDtor(&proc->ret_addr_stk);

	return SPU_NO_ERROR;
}

SPUStatusCode CodeHeaderChecker(SPU* proc, FILE* file) {

	McHeader header = {};

	size_t read_check = fread(&header, sizeof(char), sizeof(McHeader), file);
	if (read_check != sizeof(McHeader))
		SPU_ERROR_DEMO(SPU_FILE_READ_ERROR);

	if (header.signature != *(const long long*)SIGNATURE)
		SPU_ERROR_DEMO(SPU_SIGNATURE_ERROR);

	if (header.code_version != CODE_VERSION)
		SPU_ERROR_DEMO(SPU_CODE_VERSION_ERROR);

	proc->size = header.code_size;

	proc->code = (unsigned char*)calloc(proc->size, sizeof(unsigned char));
	if (!proc->code)
		SPU_ERROR_DEMO(SPU_ALLOC_ERROR);

	return SPU_NO_ERROR;
}

SPUStatusCode SPURun(SPU* proc) {

	SPUStatusCode spu_status = SPU_NO_ERROR;

	for (; proc->pc < proc->size; proc->pc++) {

#ifdef SPU_DUMP
		SPUDump(proc, proc->pc);
#endif

		switch (*(proc->code + proc->pc) & MASK_FOR_COMMANDS) {
			#include "../../include/commands"
			default:
				SPU_ERROR_DEMO(SPU_COMMAND_ERROR);
		}
	}

	return SPU_NO_ERROR;
}

double* GetArg(SPU* proc) {

	double* argValue = NULL;
	proc->registers[0] = 0;

	Command_t argType = proc->code[proc->pc];
	proc->pc += sizeof(Command_t);

	if (argType & MASK_FOR_REGISTER) {
		proc->registers[0] = proc->registers[*(Register_t*)(proc->code + proc->pc)];

		argValue = &proc->registers[*(Register_t*)(proc->code + proc->pc)];

		proc->pc += sizeof(Register_t);
	}
	if (argType & MASK_FOR_NUMBER) {
		argValue = &(proc->registers[0] += *(Immediate_t*)(proc->code + proc->pc));
		proc->pc += sizeof(Immediate_t);
	}
	if (argType & MASK_FOR_MEMORY)	argValue = &proc->ram[(size_t)proc->registers[0]];

	proc->pc--;
	return argValue;
}
