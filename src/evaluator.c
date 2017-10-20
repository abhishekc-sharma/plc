#include "ast.h"
#include "evaluator.h"

int is_value(ast_t *ast) {
  if(ast->type == A_VARIABLE || ast->type == A_ABSTRACTION) {
    return 1;
  }

  return 0;
}

evaluation_status_t evaluate_single_step(ast_t **, int context);

evaluation_status_t evaluate(ast_t **ast_ptr) {
  switch(evaluate_single_step(ast_ptr, 0)) {
    case EV_SUCCESS:
      return evaluate(ast_ptr);
      break;
    case EV_FAILURE:
      if(is_value(*ast_ptr)) {
        return EV_SUCCESS;
      }
      return EV_FAILURE;
      break;
  }

  return EV_FAILURE;
}

void shift(ast_t *, int, int);
int substitute(ast_t **, ast_t *, int);

evaluation_status_t evaluate_single_step(ast_t **ast_ptr, int context) {
  ast_t *ast = *ast_ptr;
  if(ast->type == A_APPLICATION) {
    if(!is_value(ast->children[0])) {
      return evaluate_single_step(&ast->children[0], context + 1);
    } else if(!is_value(ast->children[1])) {
      return evaluate_single_step(&ast->children[1], context + 1);
    } else {
      shift(ast->children[1], 1, 0);
      substitute(&ast->children[0], ast->children[1], -1);
      shift(ast->children[0], -1, 0);
      *ast_ptr = ast->children[0]->children[0];
      ast_release(ast->children[0], R_EXCEPT_0);
      ast_release(ast, R_EXCEPT_0);
      return EV_SUCCESS;
    }
  }

  return EV_FAILURE;
}

void shift(ast_t *ast, int d, int c) {
  switch(ast->type) {
    case A_VARIABLE:
      if(ast->variable_index >= c) {
        ast->variable_index += d;
      }
      break;
    case A_ABSTRACTION:
      shift(ast->children[0], d, c + 1);
      break;
    case A_APPLICATION:
      shift(ast->children[0], d, c);
      shift(ast->children[1], d, c);
      break;
  }
}

int substitute(ast_t **lhs_ptr, ast_t *rhs, int variable_index) {
  ast_t *lhs = *lhs_ptr;
  switch(lhs->type) {
    case A_VARIABLE:
      if(lhs->variable_index == variable_index) {
        ast_release(*lhs_ptr, R_ALL);
        *lhs_ptr = rhs;
        return 1;
      }
      return 0;
      break;
    case A_ABSTRACTION:
      ;
      ast_t *rhs_clone = ast_clone(rhs);
      shift(rhs_clone, 1, 0);
      int b = substitute(&lhs->children[0], rhs_clone, variable_index + 1);
      if(!b) {
        ast_release(rhs_clone, R_ALL);
      }
      return 0;
      break;
    case A_APPLICATION:
      ;
      int b0 = substitute(&lhs->children[0], rhs, variable_index);
      int b1 = substitute(&lhs->children[1], rhs, variable_index);
      return b0 || b1;
      break;
  }

  return 0;
}
