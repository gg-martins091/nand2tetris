#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main(int argc, char * argv[]) {
  if (argc < 2) {
    printf("No source file provided for assembly\n");
    exit(EXIT_FAILURE);
  }

  printf("Assembling file: %s\n", argv[1]);

  FILE * f = initialize_parser(argv[1]);

  return 0;
}
