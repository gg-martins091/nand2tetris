#include "parser.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

FILE* initialize_parser(char* filename) {
  FILE* fp = fopen(filename, "rw");
  if (fp == NULL) {
    perror("Error openning source file");
  }

  return fp;
}

bool has_more_commands(FILE* file) {
  return !feof(file);
}

inline char* remove_leading_spaces(char* str) {
  while (*str != '\0' && isspace((unsigned char)*str)) {
      str++;
  }

  return str;
}

bool advance(FILE* file, char* cmd) {
  char in_cmd[MAX_LINE_SIZE];

  if (fgets(in_cmd, MAX_LINE_SIZE, file)) {
    char* cur_cmd = remove_leading_spaces(in_cmd);
    size_t len = strlen(cur_cmd);
    bool is_empty_line = len == 0;// && isspace((unsigned char) cur_cmd[0]);
    bool is_comment_line = strncmp(cur_cmd, "//", 2) == 0;

    if (is_empty_line || is_comment_line) {
      return false;
    } else {
      char* comment_start = strstr(cur_cmd, "//");
      if (comment_start != NULL) {
        * comment_start = '\0';
      }
      char* end = cur_cmd + strlen(cur_cmd) - 1;
      while (end > cur_cmd && isspace((unsigned char)* end)) {
        end--;
      }
      *(end + 1) = '\0';
    }

    strcpy(cmd, cur_cmd);
    return true;
  } 
  return false;
}

COMMAND_TYPE command_type(char* cmd) {
  if (strncmp(cmd, "push", 4) == 0) {
    return C_PUSH;
  } else if (strncmp(cmd, "pop", 3) == 0) {
    return C_POP;
  }

  return C_ARITHMETIC;
}


char* arg1(char* cmd, COMMAND_TYPE ct, char **end) {
  if (ct == C_ARITHMETIC) {
    char* ret = strdup(cmd);
    return cmd;
  }

  char* a1_start = strchr(cmd, ' ');
  if (a1_start == NULL) {
    fprintf(stderr, "Invalid command: %s\n", cmd);
    exit(EXIT_FAILURE);
  }
  while(*a1_start != '\0' && isspace((unsigned char) *a1_start)) {
    ++a1_start;
  }

  char* a1_end = a1_start;
  while(*a1_end != '\0' && !isspace((unsigned char) *a1_end)) {
    ++a1_end;
  }
  *end = a1_end;

  size_t len = a1_end - a1_start;
  char* s = malloc(len + 1);
  if (s == NULL) {
    return NULL;
  }
  s[len] = '\0';

  memcpy(s, a1_start, len);
  return s;
}

int* arg2(char* cmd) {
  char* a2_start = cmd;
  if (a2_start == NULL) {
    fprintf(stderr, "Invalid command: %s\n", cmd);
    exit(EXIT_FAILURE);
  }
  while(*a2_start != '\0' && isspace((unsigned char) *a2_start)) {
    ++a2_start;
  }

  int *a2 = malloc(sizeof(int));
  sscanf(a2_start, "%d", a2);

  return a2;
}

const char* get_register(const char *seg) {
  if (strcmp(seg, "local") == 0) {
    return "LCL";
  } else if (strcmp(seg, "argument") == 0) {
    return "ARG";
  } else if (strcmp(seg, "this") == 0) {
    return "THIS";
  } else if (strcmp(seg, "that") == 0) {
    return "THAT";
  }

  fprintf(stderr, "Invalid segment: %s\n", seg);
  exit(EXIT_FAILURE);
}
