MAKE = make

SPU_DIR = SPU
ASM_DIR = Assembler
BUILD_DIR = build

all: ASM VM

VM:
	@cd $(SPU_DIR); \
	$(MAKE) run

ASM:
	@cd $(ASM_DIR); \
	$(MAKE) run
