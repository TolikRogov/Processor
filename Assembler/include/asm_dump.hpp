#ifndef ASM_DUMP_INCLUDE
#define ASM_DUMP_INCLUDE

#include "assembler.hpp"

AsmStatusCode AsmDump(Assembler* assembler, const char* string);
AsmStatusCode ListingHeader(Assembler* assembler);

#endif // ASM_DUMP_INCLUDE
