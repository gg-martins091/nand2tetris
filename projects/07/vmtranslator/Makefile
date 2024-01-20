# Makefile for translator project

# Compiler
CC = clang

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Build target
TARGET = translator

# Object files
OBJS = translator.o parser.o code.o

# Default target
all: $(TARGET)

# Rule to link the program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files
translator.o: translator.c parser.h
	$(CC) $(CFLAGS) -c translator.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

code.o: code.c code.h
	$(CC) $(CFLAGS) -c code.c

# Clean the build
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
