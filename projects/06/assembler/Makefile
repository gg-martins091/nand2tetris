# Makefile for assembler project

# Compiler
CC = clang

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Build target
TARGET = assembler

# Object files
OBJS = assembler.o parser.o code.o symboltable.o

# Default target
all: $(TARGET)

# Rule to link the program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files
assembler.o: assembler.c parser.h
	$(CC) $(CFLAGS) -c assembler.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

code.o: code.c code.h
	$(CC) $(CFLAGS) -c code.c

symboltable.o: symboltable.c symboltable.h
	$(CC) $(CFLAGS) -c symboltable.c

# Clean the build
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
