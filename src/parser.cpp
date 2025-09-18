#include "parser.hpp"

// Includes

#include <iostream>

// Parser

Parser::Parser(std::vector<Token>& tokens)
   : tokens(tokens), program(std::vector<Stmt>{}) {}

Program& Parser::parse() {
   while (!is(Type::eof)) {
      program.stmts.push_back(std::move(parse_expr()));
   }
   return program;
}

// Parse functions

// Parse statements

Stmt Parser::parse_stmt() {
   auto& token = current();

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
   } else if (token.lexeme == "IMP") {
      return parse_import();
   } else {
      std::cerr << "Unknown keyword.\n";
      std::exit(1);
   }
}

Stmt Parser::parse_var_decl() {
   advance();
   auto identifier = parse_expr(), value = parse_expr();
   return VarDecl::make(identifier, value);
}

Stmt Parser::parse_fn_decl() {
   advance();
   auto identifier = parse_primary_expr();

   if (!is(Type::open_paren)) {
      std::cerr << "Expected 'open_paren' after identifier in function declaration.\n";
      std::exit(1);
   }
   advance();

   std::vector<Stmt> params;
   while (!is(Type::close_paren)) {
      params.push_back(parse_expr());
   }

   if (!is(Type::close_paren)) {
      std::cerr << "Unterminated parameter list.\n";
      std::exit(1);
   }
   advance();
   auto body = parse_expr();
   return FnDecl::make(identifier, body, params);
}

Stmt Parser::parse_while_loop() {
   advance();
   auto body = parse_expr();
   return WhileLoop::make(body);
}

Stmt Parser::parse_break_stmt() {
   advance();
   return BreakStmt::make();
}

Stmt Parser::parse_continue_stmt() {
   advance();
   return ContinueStmt::make();
}

Stmt Parser::parse_return_stmt() {
   advance();
   auto expr = parse_expr();
   return ReturnStmt::make(expr);
}

Stmt Parser::parse_push_stmt() {
   advance();
   auto expr = parse_expr();
   return PushStmt::make(expr);
}

Stmt Parser::parse_type_stmt() {
   advance();
   auto expr = parse_expr();
   return TypeStmt::make(expr);
}

Stmt Parser::parse_import() {
   advance();
   auto import = parse_expr();
   return ImportStmt::make(import);
}

// Parse expressions

Stmt Parser::parse_expr() {
   return parse_ternary_expr();
}

Stmt Parser::parse_ternary_expr() {
   auto left = parse_call_expr();

   while (is(Type::pipe)) {
      advance();
      auto right = parse_expr();
      left = TernaryExpr::make(left, right);
   }
   return left;
}

Stmt Parser::parse_call_expr() {
   auto identifier = parse_primary_expr();
   if (identifier->type != StmtType::identifier) {
      return identifier;
   }

   while (is(Type::open_paren)) {
      advance();

      std::vector<Stmt> args;
      while (!is(Type::close_paren)) {
         args.push_back(parse_expr());
      }

      if (!is(Type::close_paren)) {
         std::cerr << "Unterminated argument list.\n";
         std::exit(1);
      }
      advance();
      identifier = CallExpr::make(identifier, args);
   }
   return identifier;
}

// Parse primary expressions

Stmt Parser::parse_primary_expr() {
   if (is(Type::identifier)) {
      std::string identifier = current().lexeme;
      advance();
      return IdentLiteral::make(identifier);
   } else if (is(Type::number)) {
      long number = 0;

      try {
         number = std::stol(current().lexeme);
      } catch (...) {
         std::cerr << "Could not convert string to number.\n";
         std::exit(1);
      }
      advance();
      return NumberLiteral::make(number);
   } else if (is(Type::string)) {
      std::string string = current().lexeme;
      advance();
      return StringLiteral::make(string);
   } else if (is(Type::open_brace)) {
      advance();
      auto program = std::make_shared<Program>(std::vector<Stmt>{});

      while (!is(Type::close_brace)) {
         auto stmt = parse_expr();
         program->stmts.push_back(stmt);
      }

      if (!is(Type::close_brace)) {
         std::cerr << "Unterminated scope.\n";
         std::exit(1);
      }
      advance();
      return program;
   } else if (is(Type::keyword)) {
      return parse_stmt();
   } else if (is(Type::semicolon)) {
      return parse_push_stmt();
   } else if (is(Type::question)) {
      return parse_type_stmt();
   } else if (is(Type::hash)) {
      advance();
      return PullStmt::make();
   } else if (is(Type::eof)) {
      std::cerr << "Unexpected token: 'EOF'.\n";
      std::exit(1);
   } else {
      auto type = current().type;
      advance();

      if (is(Type::times)) {
         advance();
         auto times = parse_expr();
         return Command::make(type, std::optional(times));
      }
      return Command::make(type, std::nullopt);
   }
}

// Helper functions

void Parser::advance() {
   if (index + 1 < tokens.size()) {
      ++index;
   }
}

bool Parser::is(Type type) {
   return index < tokens.size() && tokens.at(index).type == type;
}

Token& Parser::current() {
   return tokens.at(index);
}
