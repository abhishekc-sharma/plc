#ifndef PLC_PARSER_H
#define PLC_PARSER_H

#include "ast.h"
#include "io_interface.h"
#include "list.h"
#include "scanner.h"

typedef struct {
  scanner_t *scanner;
  list_t *context;
} parser_t;

parser_t *parser_create();
void parser_release(parser_t *parser);
ast_t *parser_run(parser_t *parser, io_interface_t *io_interface);
#endif
