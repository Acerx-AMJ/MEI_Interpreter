#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

// Includes

#include "environment.hpp"
#include <stack>

// Interpreter

class Interpreter {
   std::stack<int> loop_stack, fn_stack, return_stack;
   int fn_counter = 0;
   bool should_break = false, should_continue = false;

   // Statement evaluation functions

   Value evaluate_stmt(Environment& env, Stmt stmt);
   Value evaluate_var_decl(Environment& env, Stmt stmt);
   Value evaluate_fn_decl(Environment& env, Stmt stmt);
   Value evaluate_while_loop(Environment& env, Stmt stmt);
   Value evaluate_break(Environment& env, Stmt stmt);
   Value evaluate_continue(Environment& env, Stmt stmt);
   Value evaluate_return(Environment& env, Stmt stmt);
   Value evaluate_push(Environment& env, Stmt stmt);
   Value evaluate_type(Environment& env, Stmt stmt);
   Value evaluate_pull(Environment& env, Stmt stmt);
   Value evaluate_import(Environment& env, Stmt stmt);

   // Expression evaluation functions

   Value evaluate_expr(Environment& env, Stmt stmt);
   Value evaluate_ternary_expr(Environment& env, Stmt stmt);
   Value evaluate_call_expr(Environment& env, Stmt stmt);
   Value evaluate_command(Environment& env, Stmt stmt);
   Value evaluate_primary_expr(Environment& env, Stmt stmt);

public:
   // Evaluation functions

   Value evaluate(Program& program, Environment& env);
   Value call(Environment& env, Value func, std::vector<Value>& args);
};

#endif
