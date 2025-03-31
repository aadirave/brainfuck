#include "lexer.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long get_size(FILE *fp);
static char *strip_input(char *original);
static Lexer *populate_tok_list(Lexer *lex);

bool preprocess_matching_brackets(Lexer *lex) {
  lex->matching_bracket = calloc(lex->len, sizeof(int));
  if (lex->matching_bracket == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return false;
  }

  int *stack = calloc(lex->len, sizeof(int));
  int top = -1;

  for (int i = 0; i < lex->len; i++) {
    if (lex->tok_list[i].sym == LOOP_OPEN) {
      stack[++top] = i;
    } else if (lex->tok_list[i].sym == LOOP_CLOSE) {
      if (top == -1) {
        fprintf(stderr, "Syntax Error: Unmatched ']'\n");

        free(stack);
        return false;
      }
      int match = stack[top--];
      lex->matching_bracket[match] = i;
      lex->matching_bracket[i] = match;
    }
  }

  if (top != -1) {
    fprintf(stderr, "Syntax Error: Unmatched '['\n");

    free(stack);
    return false;
  }

  free(stack);
  return true;
}

Lexer *make_lex(char *filename) {
  FILE *input = fopen(filename, "r");
  if (input == NULL) {
    return NULL;
  }

  long size = get_size(input);

  char *program = calloc(size + 1, sizeof(char));
  fread(program, sizeof(char), size, input);

  Lexer *lex = calloc(1, sizeof(Lexer));
  lex->program = program;
  lex = populate_tok_list(lex);

  assert(lex->pos == 0);

  if (preprocess_matching_brackets(lex) == false) {
    free_lexer(lex);
    fclose(input);
    return NULL;
  }

  fclose(input);
  return lex;
}

static char *strip_input(char *original) {
  int req = 0, idx = 0;
  while (original[idx] != 0) {
    if (!isspace(original[idx])) {
      req++;
    }
    idx++;
  }

  char *stripped = calloc(req + 1, sizeof(char));
  if (stripped == NULL) {
    free(original);
    return NULL;
  }

  req = idx = 0;
  while (original[idx] != 0) {
    if (!isspace(original[idx])) {
      stripped[req] = original[idx];
      req++;
    }
    idx++;
  }

  free(original);
  return stripped;
}

static Lexer *populate_tok_list(Lexer *lex) {
  lex->program = strip_input(lex->program);
  lex->tok_list = calloc(strlen(lex->program), sizeof(Token));

  Token read;
  int i = 0;
  while ((read = next(lex)).sym != SYM_EOF && read.sym != ERROR) {
    lex->tok_list[i] = read;
    i++;
  }

  lex->len = i;

  // reset position to read token list
  lex->pos = 0;
  return lex;
}

Token next(Lexer *lex) {
  char *stream = lex->program;
  int pos = lex->pos;

  Token to_return = {.repeat = 0};

  Symbols to_read;
  switch (stream[pos]) {
  case 0:
    return (Token){.sym = SYM_EOF, .repeat = 1};
  case '<':
  case '>':
  case '+':
  case '-':
  case '.':
  case ',':
    to_read = stream[pos];
    break;
  case '[':
  case ']':
    // dealing with this by itself to do recursive descent later
    // can't really group this
    lex->pos++;
    return (Token){.sym = stream[pos], .repeat = 1};
  default:
    return (Token){.sym = ERROR, .repeat = 0};
  }

  to_return.sym = to_read;

  while (stream[pos] == to_read) {
    pos++;
    to_return.repeat++;
  }

  lex->pos = pos;

  return to_return;
}

void free_lexer(Lexer *lex) {
  free(lex->matching_bracket);
  free(lex->program);
  free(lex->tok_list);
  free(lex);
}

static long get_size(FILE *fp) {
  fseek(fp, 0L, SEEK_END);
  long size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  return size;
}
