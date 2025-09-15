#include "parser.hpp"
#include <iostream>

std::vector<Token> null_tok_vec;
Program null_program (std::vector<Stmt>{});

std::vector<Token>& tokens = null_tok_vec;
Program& program = null_program;
size_t index = 0;

void parse(std::vector<Token>& tkns, Program& prgrm) {
   program = prgrm;
   tokens = tkns;

   while (!par_is(Type::eof)) {
      program.stmts.push_back(std::move(parse_expr()));
   }
}

Stmt parse_stmt() {
   auto& token = par_current();
   
   if (token.lexeme == "VAR") {
      return parse_var_decl();
   } else if (token.lexeme == "FUN") {
      return parse_fn_decl();
   } else if (token.lexeme == "WHL") {
      return parse_while_loop();
   } else if (token.lexeme == "BRK") {
      return parse_break_stmt();
   } else if (token.lexeme == "CON") {
      return parse_continue_stmt();
   } else if (token.lexeme == "RET") {
      return parse_return_stmt();
   } else {
      std::cerr << "Segmentation fault (core dumped).\n";
      std::exit(1);
   }
}

Stmt parse_var_decl() {
   par_advance();
   auto identifier = parse_expr();

   if (!par_is(Type::equal)) {
      std::cerr << "Expected 'equal' token after identifier in variable declaration.\n";
      std::exit(1);
   }
   par_advance();
   auto value = parse_expr();
   return VarDecl::make(identifier, value);
}

Stmt parse_fn_decl() {
   par_advance();
   auto identifier = parse_primary_expr();

   if (!par_is(Type::open_paren)) {
      std::cerr << "Expected 'open_paren' after identifier in function declaration.\n";
      std::exit(1);
   }
   par_advance();

   std::vector<Stmt> params;
   while (!par_is(Type::close_paren)) {
      params.push_back(parse_expr());
   }

   if (!par_is(Type::close_paren)) {
      std::cerr << "Unterminated parameter list.\n";
      std::exit(1);
   }
   auto body = parse_expr();
   return FnDecl::make(identifier, body, params);
}

Stmt parse_while_loop() {
   par_advance();
   auto expr = parse_expr(), body = parse_expr();
   return WhileLoop::make(expr, body);
}

Stmt parse_break_stmt() {
   par_advance();
   return BreakStmt::make();
}

Stmt parse_continue_stmt() {
   par_advance();
   return ContinueStmt::make();
}

Stmt parse_return_stmt() {
   par_advance();
   auto expr = parse_expr();
   return ReturnStmt::make(expr);
}

Stmt parse_expr() {
   return parse_ternary_expr();
}

Stmt parse_ternary_expr() {
   auto left = parse_call_expr();

   while (par_is(Type::question)) {
      par_advance();
      auto middle = parse_expr();

      if (!par_is(Type::colon)) {
         std::cerr << "Expected a 'colon' after middle expression in ternary expression.\n";
         std::exit(1);
      }
      par_advance();
      auto right = parse_expr();
      left = TernaryExpr::make(left, middle, right);
   }
   return left;
}

Stmt parse_call_expr() {
   auto identifier = parse_primary_expr();
   if (identifier->type != StmtType::identifier) {
      return identifier;
   }

   while (par_is(Type::open_paren)) {
      par_advance();

      std::vector<Stmt> args;
      while (!par_is(Type::close_paren)) {
         args.push_back(parse_expr());
      }

      if (!par_is(Type::close_paren)) {
         std::cerr << "Unterminated argument list.\n";
         std::exit(1);
      }
      identifier = CallExpr::make(identifier, args);
   }
   return identifier;
}

Stmt parse_primary_expr() {
   if (par_is(Type::identifier)) {
      std::string identifier = par_current().lexeme;
      par_advance();
      return IdentLiteral::make(identifier);
   } else if (par_is(Type::number)) {
      long number = 0;

      try {
         number = std::stol(par_current().lexeme);
      } catch (...) {
         std::cerr << "Could not convert string to number.\n";
         std::exit(1);
      }
      par_advance();
      return NumberLiteral::make(number);
   } else if (par_is(Type::string)) {
      std::string string = par_current().lexeme;
      par_advance();
      return StringLiteral::make(string);
   } else if (par_is(Type::open_paren)) {
      par_advance();
      auto value = parse_expr();

      if (!par_is(Type::close_paren)) {
         std::cerr << "Unterminated parentheses.\n";
         std::exit(1);
      }
      par_advance();
      return value;
   } else if (par_is(Type::open_brace)) {
      par_advance();
      auto program = std::make_shared<Program>(std::vector<Stmt>{});

      while (!par_is(Type::close_brace)) {
         auto stmt = parse_expr();
         program->stmts.push_back(stmt);
      }

      if (!par_is(Type::close_brace)) {
         std::cerr << "Unterminated scope.\n";
         std::exit(1);
      }
      par_advance();
      return program;
   } else if (par_is(Type::keyword)) {
      return parse_stmt();
   } else if (par_is(Type::eof)) {
      std::cerr << "Unexpected token: 'EOF'.\n";
      std::exit(1);
   } else {
      auto type = par_current().type;
      par_advance();

      if (par_is(Type::times)) {
         par_advance();
         auto times = parse_expr();

         if (times->type != StmtType::number) {
            std::cerr << "Expected a number after 'X' operator.\n";
            std::exit(1);
         }
         return Command::make(type, static_cast<NumberLiteral&>(*times).number);
      }
      return Command::make(type);
   }
}

void par_advance() {
   if (index + 1 < tokens.size()) {
      ++index;
   }
}

bool par_is(Type type) {
   return index < tokens.size() && tokens.at(index).type == type;
}

Token& par_current() {
   return tokens.at(index);
}
