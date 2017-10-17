#include <ctype.h>
#include "io_interface.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

void read_identifier(scanner_t *, io_interface_t *, char **);

scanner_t *scanner_create() {
  scanner_t *scanner = (scanner_t *) malloc(sizeof(scanner_t));

  scanner->line = 1;
  scanner->column = 0;
  scanner->saved_token = NULL;

  return scanner;
}
void scanner_release(scanner_t *scanner) {
  if(scanner->saved_token != NULL) {
    token_release(scanner->saved_token);
  }

  free(scanner);
}

token_t *scanner_gettoken(scanner_t *scanner, io_interface_t *io_interface) {
  if(scanner->saved_token != NULL) {
    token_t *t = scanner->saved_token;
    scanner->saved_token = NULL;
    return t;
  }

  char c;
  while((c = io_interface_getchar(io_interface)) != EOF) {
    ++scanner->column;
    switch(c) {
      case '(':
        return token_create(T_LPAREN, scanner->line, scanner->column, NULL);
        break;
      case ')':
        return token_create(T_RPAREN, scanner->line, scanner->column, NULL);
        break;
      case '\\':
        return token_create(T_BSLASH, scanner->line, scanner->column, NULL);
        break;
      case '.':
        return token_create(T_PERIOD, scanner->line, scanner->column, NULL);
        break;
      case '\n':
        ++scanner->line;
        scanner->column = 0;
        break;
      case ' ':
      case '\t':
        break;
      default:
          if(isalnum(c)) {
            long long line = scanner->line;
            long long column = scanner->column;

            io_interface_ungetchar(io_interface, c);
            --scanner->column;
            char *identifier;
            read_identifier(scanner, io_interface, &identifier);
            token_t *t = token_create(T_IDENTIFIER, line, column, identifier);
            free(identifier);
            return t;
          } else {
            return NULL;
          }
        break;
    }
  }

  return token_create(T_EOF, scanner->line, scanner->column, NULL);
}

token_t *scanner_peektoken(scanner_t *scanner, io_interface_t *io_interface) {
  if(scanner->saved_token != NULL) {
    return scanner->saved_token;
  }
  token_t *t = scanner_gettoken(scanner, io_interface);
  scanner_ungettoken(scanner, t);
  return t;
}

void scanner_ungettoken(scanner_t *scanner, token_t *token) {
  scanner->saved_token = token;
}

void read_identifier(scanner_t *scanner, io_interface_t *io_interface, char **idptr) {
  int size = 1;

  *idptr = (char *) malloc((size + 1) * sizeof(char));
  int i = 0;
  while(isalpha(io_interface_peekchar(io_interface))) {
    if(i >= size) {
      size *= 2;
      *idptr = (char *) realloc(*idptr, (size + 1) * sizeof(char));
    }
    ++scanner->column;
    (*idptr)[i] = io_interface_getchar(io_interface);
    i++;
  }

  (*idptr)[i] = '\0';
  *idptr = (char *) realloc(*idptr, (i + 1) * sizeof(char));
}
