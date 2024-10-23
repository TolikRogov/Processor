#ifndef SPU_DUMP_INCLUDE
#define SPU_DUMP_INCLUDE

#include "spu.hpp"

SPUStatusCode SPUDump(SPU* proc, size_t pc);
SPUStatusCode LogDump(SPU* proc);

#endif // SPU_DUMP_INCLUDE
