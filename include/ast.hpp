#ifndef AST_HPP
#define AST_HPP

#include "tokens.hpp"
#include <memory>
#include <vector>

enum class StmtType {
   var_decl, fn_decl, while_loop,
   break_stmt, continue_stmt, return_stmt,
   ternary, call, command,
   identifier, number, string, program
};

struct Statement;
using Stmt = std::shared_ptr<Statement>;

struct Statement {
   StmtType type;

   Statement(StmtType type)
      : type(type) {}
   virtual ~Statement() = default;
};

struct VarDecl : public Statement {
   Stmt identifier, value;

   VarDecl(Stmt identifier, Stmt value)
      : identifier(identifier), value(value), Statement(StmtType::var_decl) {}
      
   static Stmt make(Stmt identifier, Stmt value) {
      return std::make_shared<VarDecl>(std::move(identifier), std::move(value));
   }   
};

struct FnDecl : public Statement {
   Stmt identifier, body;
   std::vector<Stmt> args;

   FnDecl(Stmt identifier, Stmt body, std::vector<Stmt> args)
      : identifier(identifier), body(body), args(args), Statement(StmtType::fn_decl) {}
   
   static Stmt make(Stmt identifier, Stmt body, std::vector<Stmt> args) {
      return std::make_shared<FnDecl>(std::move(identifier), std::move(body), std::move(args));
   }
};

struct WhileLoop : public Statement {
   Stmt expr, body;

   WhileLoop(Stmt expr, Stmt body)
      : expr(expr), body(body), Statement(StmtType::while_loop) {}
   
   static Stmt make(Stmt expr, Stmt body) {
      return std::make_shared<WhileLoop>(std::move(expr), std::move(body));
   }
};

struct BreakStmt : public Statement {
   BreakStmt() : Statement(StmtType::break_stmt) {}
   static Stmt make() { return std::make_shared<BreakStmt>(); }
};

struct ContinueStmt : public Statement {
   ContinueStmt() : Statement(StmtType::continue_stmt) {}
   static Stmt make() { return std::make_shared<ContinueStmt>(); }
};

struct ReturnStmt : public Statement {
   Stmt value;

   ReturnStmt(Stmt value)
      : value(value), Statement(StmtType::break_stmt) {}

   static Stmt make(Stmt value) {
      return std::make_shared<ReturnStmt>(std::move(value));
   }
};

struct TernaryExpr : public Statement {
   Stmt left, middle, right;

   TernaryExpr(Stmt left, Stmt middle, Stmt right)
      : left(left), middle(middle), right(right), Statement(StmtType::ternary) {}
   
   static Stmt make(Stmt left, Stmt middle, Stmt right) {
      return std::make_shared<TernaryExpr>(std::move(left), std::move(middle), std::move(right));
   }
};

struct CallExpr : public Statement {
   Stmt identifier;
   std::vector<Stmt> args;

   CallExpr(Stmt identifier, std::vector<Stmt> args)
      : identifier(identifier), args(args), Statement(StmtType::call) {}
   
   static Stmt make(Stmt identifier, std::vector<Stmt> args) {
      return std::make_shared<CallExpr>(std::move(identifier), std::move(args));
   }
};

struct Command : public Statement {
   Type op;
   long times = 1;

   Command(Type op, long times = 1)
      : op(op), times(times), Statement(StmtType::command) {}
   
   static Stmt make(Type op, long times = 1) {
      return std::make_shared<Command>(op, times);
   }
};

struct IdentLiteral : public Statement {
   std::string identifier;

   IdentLiteral(const std::string& identifier)
      : identifier(identifier), Statement(StmtType::identifier) {}
   
   static Stmt make(const std::string& identifier) {
      return std::make_shared<IdentLiteral>(identifier);
   }
};

struct NumberLiteral : public Statement {
   long number;

   NumberLiteral(long number)
      : number(number), Statement(StmtType::number) {}
   
   static Stmt make(long number) {
      return std::make_shared<NumberLiteral>(number);
   }
};

struct StringLiteral : public Statement {
   std::string string;

   StringLiteral(const std::string& string)
      : string(string), Statement(StmtType::string) {}
   
   static Stmt make(const std::string& string) {
      return std::make_shared<StringLiteral>(string);
   }
};

struct Program : public Statement {
   std::vector<Stmt> stmts;

   Program(std::vector<Stmt> stmts)
      : stmts(stmts), Statement(StmtType::program) {}
   
   static Stmt make(std::vector<Stmt> stmts) {
      return std::make_shared<Program>(std::move(stmts));
   }
};

#endif
