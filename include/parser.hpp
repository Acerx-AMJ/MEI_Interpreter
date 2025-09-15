#ifndef PARSER_HPP
#define PARSER_HPP

// Includes

#include "ast.hpp"

// Parser

class Parser {
   std::vector<Token>& tokens;
   Program program;
   size_t index = 0;

   // Parse functions

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

   // Helper functions

   void advance();
   bool is(Type type);
   Token& current();

public:
   Parser(std::vector<Token>& tokens);
   Program& parse();
};

#endif
