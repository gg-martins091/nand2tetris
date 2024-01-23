
#ifndef CODEH
#define CODEH
#include "parser.h"
#include <stdint.h>
#include <stdio.h>

void c_write_push_pop(FILE *out, COMMAND_TYPE ct, const char *seg, int *index);
void c_write_arithmetic(FILE *out, const char *cmd);


#endif
