#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "parser.h"
#include "code.h"

typedef struct {
  char* fn; // filename (no ext)
  char* fp; // full path (input)
  char* fd; // file dir
} VMFILE;

int fill_file_struc(VMFILE *file, const char *input, const char *fn, const char* end) {
  size_t fn_len = end - fn;
  file->fn = malloc(fn_len + 1);
  if (!file->fn) {
    fprintf(stderr, "Memory allocation failed for fn\n");
    return EXIT_FAILURE;
  }
  memcpy(file->fn, fn, fn_len);
  file->fn[fn_len] = '\0';

  file->fp = strdup(input);

  size_t fd_len = (fn-1) - input;
  if (fn == input) {
    fd_len = strlen(input);
  }

  file->fd = malloc(fd_len + 1);
  if (!file->fd) {
    fprintf(stderr, "Memory allocation failed for fd\n");
    free(file->fn);
    return EXIT_FAILURE;
  }
  memcpy(file->fd, input, fd_len);
  file->fn[fn_len] = '\0';
  file->fp = strdup(input);

  return 0;
}

// Not the best code but will do for now.
VMFILE *get_files(const char *input, char **out, size_t *fc /* *fq = 0 */) {
  const char *fn = strrchr(input, '/');
  if (fn != NULL) {
    ++fn;
  } else {
    fn = input;
  }

  const char *end = strchr(fn, '.');
  VMFILE *files;

  if (end != NULL) {
    files = malloc(sizeof(VMFILE));
    if (!files) { 
      fprintf(stderr, "Memory allocation failed for files\n");
      exit(EXIT_FAILURE);
    }

    if(fill_file_struc(&files[0], input, fn, end) == EXIT_FAILURE) {
      free(files);
      fprintf(stderr, "Couldn't fill file struct\n");
      exit(EXIT_FAILURE);
    }
    *out = strdup(files[0].fn);
    ++(*fc);
  } else {
    size_t capacity = 10;
    DIR *dir;
    struct dirent *entry;
    files = malloc(capacity * sizeof(char*));
    *out = (char *) malloc(sizeof(char) * 4);
    memcpy(*out, "out", 4);

    if (!files) {
        fprintf(stderr, "Memory allocation failed for files\n");
        exit(EXIT_FAILURE);
    }

    dir = opendir(input);
    if (dir == NULL) {
        fprintf(stderr, "Couldn't open dir\n");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_type != DT_REG) continue;
      char *end = strchr(entry->d_name, '.');
      if (end == NULL) continue;
      if (strcmp(end + 1, "vm") != 0) continue;

      size_t input_len = strlen(input);
      size_t name_len = strlen(entry->d_name);

      char* full_path = malloc(input_len + name_len + 1);
      memcpy(full_path, input, input_len);
      memcpy(full_path + input_len, entry->d_name, name_len);
      full_path[input_len + name_len] = '\0';

      const char *fn = strrchr(full_path, '/');
      ++fn;

      fill_file_struc(&files[*fc], full_path, fn, (full_path + input_len + name_len) - 3);
      ++(*fc);
    } 
  }

  return files;
}

int main(int argc, char * argv[]) {
  if (argc < 2) {
    printf("No source file or directory provided for trasnlation\n");
    exit(EXIT_FAILURE);
  }

  size_t fq = 0;
  // input must be a .vm file or a dir with .vm files (dir must end with /)
  // ./translator ../some/path/MyFile.vm
  // or
  // ./translator ../some/path/
  char *outt;
  VMFILE *files = get_files(argv[1], &outt, &fq);
  /* printf("Assembling file: %s\n", input); */

  FILE* out = fopen("out.asm", "w");
  for(int i = 0; i < fq; i++) {
    FILE* in = initialize_parser(files[i].fp);

    char cur_cmd[MAX_LINE_SIZE];

    int loc = 0;
    // generating labels
    while (has_more_commands(in)) {
      loc++;
      if (!advance(in, cur_cmd)) {
        continue;
      }

      COMMAND_TYPE ct = command_type(cur_cmd);

      char *a1, *a1_end;
      int *a2;
      if (ct != C_RETURN) {
        a1 = arg1(cur_cmd, ct, &a1_end);
        printf("arg1: %s\n", a1);
      }

      if (ct == C_PUSH || ct == C_POP || ct == C_FUNCTION || ct == C_CALL) {
        a2 = arg2(a1_end);
        printf("arg2: %d\n", a2);
      }

      if (ct == C_PUSH || ct == C_POP) {
        c_write_push_pop(out, ct, a1, a2);
      }

      if (ct == C_ARITHMETIC) {
        c_write_arithmetic(out, a1);
      }
    }

    fclose(in);
  }

  return 0;
}


