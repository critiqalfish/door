# Makefile

# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Source files
SRC = $(wildcard src/*.c)

# Object files
OBJ = $(SRC:src/%.c=src/%.o)

# Executable name
EXEC = door

# Default target
all: $(EXEC)

# Linking the executable from the object files
$(EXEC): $(OBJ)
	@echo ""
	@echo "- Linking"
	$(CC) $(OBJ) -o $(EXEC)
	@echo ""
	@echo "- Cleanup"
	rm -f $(OBJ)
	@echo ""
	@echo "- Success"
	@echo "executable: $(EXEC)"
# Compiling the source files into object files
$(info - Compiling)
%.o: src/%.c
	$(CC) $(CFLAGS) -c $<

# Clean target
clean:
	rm -f $(EXEC) $(OBJ)

# Phony targets
.PHONY: all clean
