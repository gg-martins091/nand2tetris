#include "parser.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

FILE* initialize_parser(char* filename) {
  FILE* fp = fopen(filename, "rw");
  if (fp == NULL) {
    perror("Error openning source file");
  }

  char cur_cmd[MAX_LINE_SIZE];

  while (has_more_commands(fp)) {
    if (!advance(fp, cur_cmd)) {
      continue;
    }

    COMMAND_TYPE ct = command_type(cur_cmd);

    printf("(%d) Line of code: %s ", ct, cur_cmd);

    if (ct == C_COMMAND) {
      char* d = dest(cur_cmd);
      char* j = jump(cur_cmd);
      char* c = comp(cur_cmd);
      printf("-> Comp: %s ", c);
      printf("-> Destination: %s ", d);
      printf("-> Jump: %s ", j);
    }

    if (ct == A_COMMAND) {
      char* s = symbol(cur_cmd);
      printf("-> Symbol: %s", s);
    }

    printf("\n");

  }
  return fp;
}

bool has_more_commands(FILE* file) {
  return !feof(file);
}

bool advance(FILE* file, char* cmd) {
  char cur_cmd[MAX_LINE_SIZE];

  if (fgets(cur_cmd, MAX_LINE_SIZE, file)) {
    size_t len = strlen(cur_cmd);
    bool is_empty_line = len == 2 && isspace((unsigned char) cur_cmd[0]);
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

      // remove leading white spaces
      size_t start = 0;
      while (cur_cmd[start] != '\0' && isspace((unsigned char) cur_cmd[start])) {
        start++;
      }

      if (start > 0) {
        int j = 0;
        while (cur_cmd[j + start] != '\0') {
          cur_cmd[j] = cur_cmd[j + start];
          j++;
        }
        cur_cmd[j] = '\0';
      }
    }

    strcpy(cmd, cur_cmd);
    return true;
  } else {
    perror("Failure reading line");
  }

  return false;

}

COMMAND_TYPE command_type(char* cmd) {
  if (cmd[0] == '@') {
    return A_COMMAND;
  }

  // Label
  if (cmd[0] == '(') {
    return L_COMMAND;
  }

  return C_COMMAND;
}

char* symbol(char* cmd) {
  char* cmdstr = cmd;
  size_t len = strlen(cmd);
  char* s = malloc(len);
  if (s == NULL) {
    return NULL;
  }

  strncpy(s, ++cmdstr, len);
  s[len] = '\0';

  return s;
}


char* dest(char* cmd) {
  const char* eqpos = strchr(cmd, '=');
  if (eqpos == NULL) {
    return NULL;
  }

  size_t length = eqpos - cmd;
  char* d = malloc(length + 1);
  if (d == NULL) {
    return NULL;
  }

  strncpy(d, cmd, length);
  d[length] = '\0';

  return d;
}

char* jump(char* cmd) {
  const char* eqpos = strchr(cmd, ';');
  if (eqpos == NULL) {
    return NULL;
  }

  char* end = cmd + strlen(cmd) - 1;
  size_t length = end - eqpos;
  char* j = malloc(length + 1);
  if (j == NULL) {
    return NULL;
  }

  strncpy(j, ++eqpos, length);
  j[length] = '\0';

  return j;
}

char* comp(char* cmd) {
  char* eq = strchr(cmd, ';');
  size_t length = 0;
  if (eq != NULL) {
    length = eq - cmd;
    eq = cmd;
  } else {
    eq = strchr(cmd, '=');
    if (eq == NULL) {
      perror("Invalid command");
      return NULL;
    }

    char* end = cmd + strlen(cmd) - 1;
    length = end - eq;
    ++eq;
  }

  char* c = malloc(length + 1);
  if (c == NULL) {
    return NULL;
  }

  strncpy(c, eq, length);
  c[length] = '\0';

  return c;
}

