#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int main(int argc, char **argv) {
  Lexer *lex = make_lex(argv[1]);

  // evaluate
  for (int i = 0; i < lex->len; i++) {
    fprintf(stderr, "%c %d\n", lex->tok_list[i].sym, lex->tok_list[i].repeat);
  }

  free_lexer(lex);
  return EXIT_SUCCESS;
}
