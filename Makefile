MAKE = make

SPU_DIR = SPU
BUILD_DIR = build
PROC = proc

all: VM

VM:
	clear
	@$(MAKE) -C $(SPU_DIR) clean
	@$(MAKE) -C $(SPU_DIR)
	@./$(SPU_DIR)/$(BUILD_DIR)/$(PROC)
