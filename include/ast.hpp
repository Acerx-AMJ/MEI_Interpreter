#ifndef AST_HPP
#define AST_HPP

// Includes

#include "tokens.hpp"
#include <memory>
#include <optional>
#include <vector>

// Statement

enum class StmtType {
   var_decl, fn_decl, while_loop, import,
   break_stmt, continue_stmt, return_stmt,
   ternary, call, command, push, type, pull,
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

// Statements

// Variable declaration statement

struct VarDecl : public Statement {
   Stmt identifier, value;

   VarDecl(Stmt identifier, Stmt value)
      : identifier(identifier), value(value), Statement(StmtType::var_decl) {}
      
   static Stmt make(Stmt identifier, Stmt value) {
      return std::make_shared<VarDecl>(std::move(identifier), std::move(value));
   }   
};

// Function declaration statement

struct FnDecl : public Statement {
   Stmt identifier, body;
   std::vector<Stmt> args;

   FnDecl(Stmt identifier, Stmt body, std::vector<Stmt> args)
      : identifier(identifier), body(body), args(args), Statement(StmtType::fn_decl) {}
   
   static Stmt make(Stmt identifier, Stmt body, std::vector<Stmt> args) {
      return std::make_shared<FnDecl>(std::move(identifier), std::move(body), std::move(args));
   }
};

// While loop statement

struct WhileLoop : public Statement {
   Stmt body;

   WhileLoop(Stmt body)
      : body(body), Statement(StmtType::while_loop) {}
   
   static Stmt make(Stmt body) {
      return std::make_shared<WhileLoop>(std::move(body));
   }
};

// Import statement

struct ImportStmt : public Statement {
   Stmt import;

   ImportStmt(Stmt import)
      : import(import), Statement(StmtType::import) {}
   
   static Stmt make(Stmt import) {
      return std::make_shared<ImportStmt>(std::move(import));
   }
};

// Break statement

struct BreakStmt : public Statement {
   BreakStmt() : Statement(StmtType::break_stmt) {}
   static Stmt make() { return std::make_shared<BreakStmt>(); }
};

// Continue statement

struct ContinueStmt : public Statement {
   ContinueStmt() : Statement(StmtType::continue_stmt) {}
   static Stmt make() { return std::make_shared<ContinueStmt>(); }
};

// Return statement

struct ReturnStmt : public Statement {
   Stmt value;

   ReturnStmt(Stmt value)
      : value(value), Statement(StmtType::return_stmt) {}

   static Stmt make(Stmt value) {
      return std::make_shared<ReturnStmt>(std::move(value));
   }
};

// Push statement

struct PushStmt : public Statement {
   Stmt stmt;

   PushStmt(Stmt stmt)
      : stmt(stmt), Statement(StmtType::push) {}
   
   static Stmt make(Stmt stmt) {
      return std::make_shared<PushStmt>(stmt);
   }
};

// Type statement

struct TypeStmt : public Statement {
   Stmt stmt;

   TypeStmt(Stmt stmt)
      : stmt(stmt), Statement(StmtType::type) {}
   
   static Stmt make(Stmt stmt) {
      return std::make_shared<TypeStmt>(stmt);
   }
};

// Pull statement

struct PullStmt : public Statement {
   PullStmt() : Statement(StmtType::pull) {}
   static Stmt make() { return std::make_shared<PullStmt>(); }
};

// Expressions

// Ternary expression

struct TernaryExpr : public Statement {
   Stmt left, right;

   TernaryExpr(Stmt left, Stmt right)
      : left(left), right(right), Statement(StmtType::ternary) {}
   
   static Stmt make(Stmt left, Stmt right) {
      return std::make_shared<TernaryExpr>(std::move(left), std::move(right));
   }
};

// Call expression

struct CallExpr : public Statement {
   Stmt identifier;
   std::vector<Stmt> args;

   CallExpr(Stmt identifier, std::vector<Stmt> args)
      : identifier(identifier), args(args), Statement(StmtType::call) {}
   
   static Stmt make(Stmt identifier, std::vector<Stmt> args) {
      return std::make_shared<CallExpr>(std::move(identifier), std::move(args));
   }
};

// Command call expression

struct Command : public Statement {
   Type op;
   std::optional<Stmt> right;

   Command(Type op, std::optional<Stmt> right)
      : op(op), right(right), Statement(StmtType::command) {}
   
   static Stmt make(Type op, std::optional<Stmt> right) {
      return std::make_shared<Command>(op, right);
   }
};

// Literals

// Identifier literal

struct IdentLiteral : public Statement {
   std::string identifier;

   IdentLiteral(const std::string& identifier)
      : identifier(identifier), Statement(StmtType::identifier) {}
   
   static Stmt make(const std::string& identifier) {
      return std::make_shared<IdentLiteral>(identifier);
   }
};

// Number literal

struct NumberLiteral : public Statement {
   long number;

   NumberLiteral(long number)
      : number(number), Statement(StmtType::number) {}
   
   static Stmt make(long number) {
      return std::make_shared<NumberLiteral>(number);
   }
};

// String literal

struct StringLiteral : public Statement {
   std::string string;

   StringLiteral(const std::string& string)
      : string(string), Statement(StmtType::string) {}
   
   static Stmt make(const std::string& string) {
      return std::make_shared<StringLiteral>(string);
   }
};

// Program

struct Program : public Statement {
   std::vector<Stmt> stmts;

   Program(std::vector<Stmt> stmts)
      : stmts(stmts), Statement(StmtType::program) {}
   
   static Stmt make(std::vector<Stmt> stmts) {
      return std::make_shared<Program>(std::move(stmts));
   }
};

#endif
