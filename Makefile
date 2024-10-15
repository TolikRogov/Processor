MAKE = make

SPU_DIR = SPU
ASM_DIR = Assembler
BUILD_DIR = build

all: ASM VM

VM:
	@$(MAKE) -C $(SPU_DIR)
	@./$(SPU_DIR)/$(BUILD_DIR)/proc data/output.txt

ASM:
	clear
	@cd $(ASM_DIR); \
	$(MAKE) run
