#include "interpreter.hpp"

// Evaluation functions

Value Interpreter::evaluate(Program& program, Environment& env) {

}

Value Interpreter::call(Environment& env, Value func, std::vector<Value>& args) {

}

// Statement evaluation functions

Value Interpreter::evaluate_stmt(Environment& env, Stmt stmt) {

}

Value Interpreter::evaluate_var_decl(Environment& env, Stmt stmt) {

}

Value Interpreter::evaluate_fn_decl(Environment& env, Stmt stmt) {

}

Value Interpreter::evaluate_while_loop(Environment& env, Stmt stmt) {

}

Value Interpreter::evaluate_break(Environment& env, Stmt stmt) {

}

Value Interpreter::evaluate_continue(Environment& env, Stmt stmt) {

}

Value Interpreter::evaluate_return(Environment& env, Stmt stmt) {

}

// Expression evaluation functions

Value Interpreter::evaluate_expr(Environment& env, Stmt stmt) {

}

Value Interpreter::evaluate_ternary_expr(Environment& env, Stmt stmt) {
   
}

Value Interpreter::evaluate_call_expr(Environment& env, Stmt stmt) {

}

Value Interpreter::evaluate_command(Environment& env, Stmt stmt) {

}

Value Interpreter::evaluate_primary_expr(Environment& env, Stmt stmt) {
   
}
