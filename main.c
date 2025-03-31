#include "interpret.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int main(int argc, char **argv) {
  Lexer *lex = make_lex(argv[1]);

  run_brainfuck(lex);

  return EXIT_SUCCESS;
}
