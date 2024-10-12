MAKE = make

SPU_DIR = SPU
ASM_DIR = ASSEMBLER
BUILD_DIR = build

all: ASM VM

VM:
	@$(MAKE) -C $(SPU_DIR)
	@./$(SPU_DIR)/$(BUILD_DIR)/proc

ASM:
	clear
	@$(MAKE) -C $(ASM_DIR)
	@./$(ASM_DIR)/$(BUILD_DIR)/asm
