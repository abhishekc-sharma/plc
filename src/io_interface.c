#include "io_interface.h"
#include <stdio.h>
#include <stdlib.h>

io_interface_t *io_interface_create(io_type_t type, FILE *input_stream, FILE *output_stream) {
  io_interface_t *io_interface = (io_interface_t *) malloc(sizeof(io_interface_t));

  io_interface->type = type;
  io_interface->input_stream = input_stream;
  io_interface->is_eof = 0;
  io_interface->output_stream = output_stream;

  return io_interface;
}

void io_interface_release(io_interface_t *io_interface) {
  free(io_interface);
}

char io_interface_getchar(io_interface_t *io_interface) {
  if(io_interface->is_eof) {
    return EOF;
  }

  char c = getc(io_interface->input_stream);
  if(io_interface->type == IO_FILE) {
    if(c == EOF) {
      io_interface->is_eof = 1;
    }
  } else if(io_interface->type == IO_REPL) {
    switch(c) {
      case '\n':
        c = EOF;
        io_interface->is_eof = 1;
        break;
      default:
        break;
    }
  }

  return c;
}

char io_interface_peekchar(io_interface_t *io_interface) {
  char c = io_interface_getchar(io_interface);
  io_interface_ungetchar(io_interface, c);
  return c;
}

void io_interface_ungetchar(io_interface_t *io_interface, char c) {
  if(c == EOF) {
      io_interface->is_eof = 0;
  }

  if(io_interface->type == IO_REPL && c == EOF) {
    c = '\n';
  }

  ungetc(c, io_interface->input_stream);
}

void io_interface_flush(io_interface_t *io_interface) {
  if(io_interface->type == IO_REPL) {
    char c;
    do {
      c = io_interface_getchar(io_interface);
    } while(c != '\n' && c != EOF);

    if(io_interface->is_eof) {
      io_interface->is_eof = 0;
      return;
    }
  }
}
