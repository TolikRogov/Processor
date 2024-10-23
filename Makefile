MAKE = make

SPU_DIR = SPU
ASM_DIR = Assembler

PROJECTS = $(ASM_DIR) $(SPU_DIR)
PUSK = run all

default:
	@for dir in $(PROJECTS); do \
		cd $$dir; $(MAKE); cd ../; \
	done

$(PUSK): default
	clear
	@for dir in $(PROJECTS); do \
		cd $$dir; $(MAKE) $@; cd ../; \
	done
