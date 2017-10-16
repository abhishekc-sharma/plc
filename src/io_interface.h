#ifndef __IO_INTERFACE_H
#define __IO_INTERFACE_H

#include <stdio.h>

typedef enum {
  IO_FILE,
  IO_REPL
} io_type_t;

typedef struct {
  io_type_t type;
  FILE *input_stream;
  FILE *output_stream;
} io_interface_t;

io_interface_t *io_interface_create(io_type_t type, FILE *input_stream, FILE *output_stream);
void io_interface_release(io_interface_t *io_interface);

char io_interface_getchar(io_interface_t *io_interface);
char io_interface_peekchar(io_interface_t *io_interface);
void io_interface_ungetchar(io_interface_t *io_interface, char c);

#endif
