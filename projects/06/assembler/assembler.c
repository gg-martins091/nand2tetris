#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "code.h"

int main(int argc, char * argv[]) {
  if (argc < 2) {
    printf("No source file provided for assembly\n");
    exit(EXIT_FAILURE);
  }

  char* input = argv[1];
  printf("Assembling file: %s\n", input);

  FILE* in = initialize_parser(input);
  FILE* out = fopen("out.hack", "w");

  char cur_cmd[MAX_LINE_SIZE];
  int line = 0;

  while (has_more_commands(in)) {
    line++;
    if (!advance(in, cur_cmd)) {
      continue;
    }

    COMMAND_TYPE ct = command_type(cur_cmd);

    uint16_t dd = 0, jj = 0, cc = 0;
    if (ct == C_COMMAND) {
      char* d = dest(cur_cmd);
      dd = c_dest(d);
      free(d);
      char* j = jump(cur_cmd);
      jj = c_jump(j);
      free(j);
      char* c = comp(cur_cmd);
      cc = c_comp(c);
      free(c);
    }

    uint16_t sa = 0;
    if (ct == A_COMMAND) {
      char* s = symbol(cur_cmd);
      sa = symbol_to_address(s);
      free(s);
      if (sa >= UINT16_MAX) {
        printf("Line of code (%d): %s ", line, cur_cmd);
        exit(-1);
      }
    }

    uint16_t line = 0 | (((int) ct * 7) << 13); // c instruction;
    line |= sa;
    line |= (jj | dd | (cc << 6)); // shift cc left to jump the 3bits of jmp and 3bits of dest
    char* lc = c_tostr(line);

    fprintf(out, "%s\n", lc);
    free(lc);
  }

  fclose(out);
  fclose(in);

  return 0;
}
