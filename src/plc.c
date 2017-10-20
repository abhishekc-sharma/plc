#include "ast.h"
#include "evaluator.h"
#include "io_interface.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if(argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
    printf("Usage: plc [.plc file]\n");
  } else if(argc > 1) {
    // File Mode
    FILE *fp = fopen(argv[1], "r");
    io_interface_t *io_interface = io_interface_create(IO_FILE, fp, stdout);
    parser_t *parser = parser_create();
    ast_t *program = parser_run(parser, io_interface);

    if(program != NULL) {
      evaluation_status_t status = evaluate(&program);
      if(status == EV_FAILURE) {
        printf("Error\n");
        parser_release(parser);
        ast_release(program, R_ALL);
      } else {
        ast_print(program);
      }
    } else {
      printf("Error\n");
      parser_release(parser);
      io_interface_release(io_interface);
    }

    ast_release(program, R_ALL);
    parser_release(parser);
    io_interface_release(io_interface);
    fclose(fp);
  } else {
    // REPL Mode
    io_interface_t *io_interface = io_interface_create(IO_REPL, stdin, stdout);
    printf("Pure Lambda Calculus\n\n:q to Exit\n");
    while(1) {
      printf("> ");
      if(io_interface_peekchar(io_interface) == ':') {
        io_interface_getchar(io_interface);
        char c = io_interface_getchar(io_interface);
        if(c == 'q') {
          printf("Bye!\n");
          break;
        } else {
          io_interface_flush(io_interface);
          printf("Invalid Command\n");
          continue;
        }
      }
      parser_t *parser = parser_create();
      ast_t *program = parser_run(parser, io_interface);

      if(program != NULL) {
        evaluation_status_t status = evaluate(&program);
        if(status == EV_FAILURE) {
          printf("Error\n");
          io_interface_flush(io_interface);
          parser_release(parser);
          ast_release(program, R_ALL);
          continue;
        } else {
          ast_print(program);
        }
      } else {
        printf("Error\n");
        io_interface_flush(io_interface);
        parser_release(parser);
        continue;
      }
      io_interface_flush(io_interface);
      ast_release(program, R_ALL);
      parser_release(parser);
    }
    io_interface_release(io_interface);
  }

  return 0;
}
