#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"

void parse(std::vector<Token>& tokens, Program& program);

Stmt parse_stmt();
Stmt parse_var_decl();
Stmt parse_fn_decl();
Stmt parse_while_loop();
Stmt parse_break_stmt();
Stmt parse_continue_stmt();
Stmt parse_return_stmt();

Stmt parse_expr();
Stmt parse_ternary_expr();
Stmt parse_call_expr();
Stmt parse_primary_expr();

void par_advance();
bool par_is(Type type);
Token& par_current();

#endif
