

#include "code.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static int labels = 0;

void c_write_push_pop(FILE *out, COMMAND_TYPE ct, const char *seg, int *index) {
  if (ct == C_PUSH) {
      fprintf(out, "@%d\n", *index); // A = VAL
      fprintf(out, "D=A\n");
      fprintf(out, "@SP\n");
      fprintf(out, "A=M\n");
      fprintf(out, "M=D\n"); // SP = A
      fprintf(out, "@SP\n");
      fprintf(out, "M=M+1\n"); // SP++;
  } else {
      const char *reg = get_register(seg);
      fprintf(out, "@%s\n", reg);
      fprintf(out, "D=M\n");
      fprintf(out, "@%d\n", *index);
      fprintf(out, "D=D+A\n"); // D = SEG + INDEX;
      fprintf(out, "@R13\n");
      fprintf(out, "M=D\n"); // R13 = D //  SEG + INDEX;

      fprintf(out, "@SP\n");
      fprintf(out, "M=M-1\n");
      fprintf(out, "A=M\n");
      fprintf(out, "D=M\n"); // D = POP;

      fprintf(out, "@R13\n");
      fprintf(out, "A=M\n");
      fprintf(out, "M=D\n"); // SEG + INDEX = D
  }
}

void c_write_arithmetic(FILE *out, const char *cmd) {
  fprintf(out, "@SP\n");
  fprintf(out, "M=M-1\n");
  fprintf(out, "A=M\n");
  fprintf(out, "D=M\n"); // D = STACK POP

  if (strcmp(cmd, "add") == 0) {
    fprintf(out, "@SP\n");
    fprintf(out, "AM=M-1\n");
    fprintf(out, "M=M+D\n"); // SP = D + SP
    fprintf(out, "@SP\n");
    fprintf(out, "M=M+1\n");
  } else if (strcmp(cmd, "sub") == 0) {
    fprintf(out, "@SP\n");
    fprintf(out, "AM=M-1\n");
    fprintf(out, "M=M-D\n"); // SP = D - SP
    fprintf(out, "@SP\n");
    fprintf(out, "M=M+1\n");
  } else if (strcmp(cmd, "and") == 0) {
    fprintf(out, "@SP\n");
    fprintf(out, "AM=M-1\n");
    fprintf(out, "M=M&D\n"); // SP = D & SP
    fprintf(out, "@SP\n");
    fprintf(out, "M=M+1\n");
  } else if (strcmp(cmd, "or") == 0) {
    fprintf(out, "@SP\n");
    fprintf(out, "AM=M-1\n");
    fprintf(out, "M=M|D\n"); // SP = D | SP
    fprintf(out, "@SP\n");
    fprintf(out, "M=M+1\n");
  } else if (strcmp(cmd, "neg") == 0) {
    fprintf(out, "M=-D\n"); // SP = NEG(SP)
    fprintf(out, "@SP\n");
    fprintf(out, "M=M+1\n");
  } else if (strcmp(cmd, "not") == 0) {
    fprintf(out, "M=!D\n"); // SP = NEG(SP)
    fprintf(out, "@SP\n");
    fprintf(out, "M=M+1\n");
  } else {
    fprintf(out, "@SP\n");
    fprintf(out, "AM=M-1\n");
    fprintf(out, "D=M-D\n"); // D = POP2 - POP1

    fprintf(out, "@JMP_TRUE%d\n", ++labels);
    if (strcmp(cmd, "eq") == 0) fprintf(out, "D;JEQ\n");
    if (strcmp(cmd, "lt") == 0) fprintf(out, "D;JLT\n");
    if (strcmp(cmd, "gt") == 0) fprintf(out, "D;JGT\n");

    fprintf(out, "@SP\n");
    fprintf(out, "A=M\n");
    fprintf(out, "M=0\n"); // false

    fprintf(out, "@JMP_END%d\n", labels);
    fprintf(out, "0;JMP\n");

    fprintf(out, "(JMP_TRUE%d)\n", labels);
    fprintf(out, "@SP\n");
    fprintf(out, "A=M\n");
    fprintf(out, "M=-1\n"); // true

    fprintf(out, "(JMP_END%d)\n", labels);
    fprintf(out, "@SP\n");
    fprintf(out, "M=M+1\n");
  }
}
