CC = g++

BUILD_DIR = build
SOURCE = source
INCLUDE = include
SUB_ONEGIN = ../Onegin
DATA = ../data

CFLAGS = -c -fsanitize=address,alignment -I$(INCLUDE) -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -fPIE -Werror=vla
LDFLAGS = -fsanitize=address,alignment

OBJECTS = $(BUILD_DIR)/main.o $(BUILD_DIR)/assembler.o $(BUILD_DIR)/asm_utilities.o $(BUILD_DIR)/Onegin.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/Sorting.o
EXECUTABLE = $(BUILD_DIR)/asm

run: default
	@find $(DATA) -type file -exec basename {} \;
	@read -p "Enter name of file that you want to assemble: " input; \
	read -p "Enter path to output assembler file: " output; \
	./$(BUILD_DIR)/asm $(DATA)/$$input $(DATA)/$$output

default: clean $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: $(SOURCE)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SUB_ONEGIN)/$(SOURCE)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rf $(BUILD_DIR)
