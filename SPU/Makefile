CC = g++

BUILD_DIR = build
SOURCE = source
INCLUDE = include

SUB_STACK = ../Stack
SUB_ONEGIN = ../Onegin

DATA = ../data

DFLAGS  = -D SPU_DUMP
CFLAGS  = -c -fsanitize=address,alignment -I$(SUB_STACK)/$(INCLUDE) -I$(INCLUDE) -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -fPIE -Werror=vla
LDFLAGS = -fsanitize=address,alignment

STACK_OBJECTS = $(BUILD_DIR)/stack.o $(BUILD_DIR)/log.o $(BUILD_DIR)/stack_utilities.o
SPU_OBJECTS = $(BUILD_DIR)/spu_utilities.o $(BUILD_DIR)/spu.o
ONEGIN_OBJECTS = $(BUILD_DIR)/Sorting.o $(BUILD_DIR)/utilities.o

OBJECTS = $(BUILD_DIR)/main.o $(STACK_OBJECTS) $(SPU_OBJECTS) $(ONEGIN_OBJECTS)

EXECUTABLE = $(BUILD_DIR)/proc

default: clean $(EXECUTABLE)

run: default
	@echo; find $(DATA) -type file -exec basename {} \;
	@echo; read -p "Enter name of file with machine code: " input; echo; \
	./$(BUILD_DIR)/proc $(DATA)/$$input

$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: $(SOURCE)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SUB_STACK)/$(SOURCE)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SUB_ONEGIN)/$(SOURCE)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rf $(BUILD_DIR)
