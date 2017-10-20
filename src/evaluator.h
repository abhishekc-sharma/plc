#ifndef PLC_EVALUATOR_H
#define PLC_EVALUATOR_H

#include "ast.h"

typedef enum {
  EV_SUCCESS,
  EV_FAILURE
} evaluation_status_t;

evaluation_status_t evaluate(ast_t **ast_ptr);

#endif
