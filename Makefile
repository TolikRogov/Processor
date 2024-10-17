MAKE = make

SPU_DIR = SPU
ASM_DIR = Assembler

PROJECTS = $(ASM_DIR) $(SPU_DIR)

default:
	@for dir in $(PROJECTS); do \
		cd $$dir; $(MAKE); cd ../; \
	done

run:
	clear
	@for dir in $(PROJECTS); do \
		cd $$dir; $(MAKE) run; cd ../; \
	done
