
#ifndef PARSERH
#define PARSERH

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 128

typedef enum  {
	C_ARITHMETIC,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL
} COMMAND_TYPE;

FILE *initialize_parser(char *filename);
bool has_more_commands(FILE *file);
bool advance(FILE *file, char *cmd);
COMMAND_TYPE command_type(char *cmd);
char *arg1(char *cmd, COMMAND_TYPE ct, char **end);
int *arg2(char *cmd);
const char* get_register(const char *seg);
/* char* symbol(char* cmd, COMMAND_TYPE ct); */
/* char* dest(char* cmd); */
/* char* jump(char* cmd); */
/* char* comp(char* cmd); */
/* uint16_t symbol_to_address(char* symbol); */
char* remove_leading_spaces(char* str);


#endif
