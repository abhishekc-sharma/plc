#ifndef __SCANNER_H
#define __SCANNER_H

#include "io_interface.h"
#include "token.h"

typedef struct {
  long long line;
  long long column;
  token_t *saved_token;
} scanner_t;

scanner_t *scanner_create();
void scanner_release(scanner_t *scanner);

token_t *scanner_gettoken(scanner_t *scanner, io_interface_t *io_interface);
token_t *scanner_peektoken(scanner_t *scanner, io_interface_t *io_interface);
void scanner_ungettoken(scanner_t *scanner, token_t *token);

#endif
