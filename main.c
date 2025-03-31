#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

static long get_size(FILE *fp);
static bool eval(char *program);

int main(int argc, char **argv) {
  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");

  // understand what to allocate
  long size = get_size(input);
  char *program = calloc(size + 1, sizeof(char));

  // read program
  fread(program, sizeof(char), size, input);

  // evaluate

  free(program);
  fclose(output);
  fclose(input);
  return EXIT_SUCCESS;
}

static long get_size(FILE *fp) {
  fseek(fp, 0L, SEEK_END);
  long size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  return size;
}
