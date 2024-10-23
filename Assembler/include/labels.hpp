#ifndef LABELS_INCLUDE
#define LABELS_INCLUDE

#include "assembler.hpp"

AsmStatusCode LabelCheck(Assembler* assembler, char* string);
AsmStatusCode AsmLabels(Assembler* assembler);
AsmStatusCode LabelStatus(Assembler* assembler, char* label);
AsmStatusCode IncreaseLabels(LabelsTable* labels_table);
int FindLabelInTable(Assembler* assembler, char* label);

#endif //LABELS_INCLUDE
