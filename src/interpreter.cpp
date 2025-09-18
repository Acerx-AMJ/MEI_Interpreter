#include "interpreter.hpp"

// Includes

#include "lexer.hpp"
#include "parser.hpp"
#include "stack.hpp"
#include <cmath>
#include <fstream>
#include <termios.h>
#include <unistd.h>

// Evaluation functions

Value Interpreter::evaluate(Program& program, Environment& env) {
   Value last;
   int id =++ fn_counter;

   for (auto& ast : program.stmts) {
      last = evaluate_stmt(env, ast);

      if (!return_stack.empty() && return_stack.top() >= id) {
         while (!return_stack.empty() && return_stack.top() != id) {
            return_stack.pop();
         }
         return last;
      }

      if (should_break || should_continue) {
         return last;
      }
   }
   return last;
}

Value Interpreter::call(Environment& env, Value func, std::vector<Value>& args) {
   if (func->type != ValueType::fn) {
      std::cerr << "Only functions are callable.\n";
      std::exit(1);
   }

   auto& fn = static_cast<Fn&>(*func.get());
   if (args.size() != fn.params.size()) {
      std::cerr << "Function parameter count does not match call expression argument count.\n";
      std::exit(1);
   }
   fn_stack.push(1);
   Environment new_env (fn.env);

   for (int i = 0; i < args.size(); ++i) {
      new_env.set(fn.params[i], args[i]);
   }
   auto result = evaluate(static_cast<Program&>(*fn.body.get()), new_env);
   fn_stack.pop();
   return result;
}

// Statement evaluation functions

Value Interpreter::evaluate_stmt(Environment& env, Stmt stmt) {
   switch (stmt->type) {
   case StmtType::var_decl:
      return evaluate_var_decl(env, stmt);
   case StmtType::fn_decl:
      return evaluate_fn_decl(env, stmt);
   case StmtType::while_loop:
      return evaluate_while_loop(env, stmt);
   case StmtType::break_stmt:
      return evaluate_break(env, stmt);
   case StmtType::continue_stmt:
      return evaluate_continue(env, stmt);
   case StmtType::return_stmt:
      return evaluate_return(env, stmt);
   case StmtType::push:
      return evaluate_push(env, stmt);
   case StmtType::type:
      return evaluate_type(env, stmt);
   case StmtType::pull:
      return evaluate_pull(env, stmt);
   case StmtType::import:
      return evaluate_import(env, stmt);
   default:
      return evaluate_expr(env, stmt);
   }
}

Value Interpreter::evaluate_var_decl(Environment& env, Stmt stmt) {
   auto& decl = static_cast<VarDecl&>(*stmt.get());
   auto value = evaluate_stmt(env, decl.value);

   if (decl.identifier->type != StmtType::identifier) {
      std::cerr << "Expected identifier in variable declaration.\n";
      std::exit(1);
   }
   env.set(static_cast<IdentLiteral&>(*decl.identifier.get()).identifier, value);
   return value;
}

Value Interpreter::evaluate_fn_decl(Environment& env, Stmt stmt) {
   auto& decl = static_cast<FnDecl&>(*stmt.get());
   std::vector<std::string> params;

   for (auto& param : decl.args) {
      if (param->type != StmtType::identifier) {
         std::cerr << "Expected identifier in function declaration parameter list.\n";
         std::exit(1);
      }
      params.push_back(static_cast<IdentLiteral&>(*param.get()).identifier);
   }

   if (decl.identifier->type != StmtType::identifier) {
      std::cerr << "Expected identifier in function declaration.\n";
      std::exit(1);
   }

   auto identifier = static_cast<IdentLiteral&>(*decl.identifier.get()).identifier;
   auto fn = Fn::make(identifier, params, &env, decl.body);
   env.set(identifier, fn);
   return fn;
}

Value Interpreter::evaluate_while_loop(Environment& env, Stmt stmt) {
   auto& whl = static_cast<WhileLoop&>(*stmt.get());
   Value result = Null::make();
   loop_stack.push(1);

   while (true) {
      if (stack::empty() || !stack::pop()) {
         loop_stack.pop();
         return result;
      }
      result = evaluate_stmt(env, whl.body);

      if (should_break) {
         should_break = false;
         loop_stack.pop();
         return result;
      }

      if (should_continue) {
         should_continue = false;
      }
   }
}

Value Interpreter::evaluate_break(Environment& env, Stmt stmt) {
   if (loop_stack.empty()) {
      std::cerr << "'BreakStmt' outside of a loop.\n";
      std::exit(1);
   }
   should_break = true;
   return Null::make();
}

Value Interpreter::evaluate_continue(Environment& env, Stmt stmt) {
   if (loop_stack.empty()) {
      std::cerr << "'ContinueStmt' outside of a loop.\n";
      std::exit(1);
   }
   should_continue = true;
   return Null::make();
}

Value Interpreter::evaluate_return(Environment& env, Stmt stmt) {
   if (fn_stack.empty()) {
      std::cerr << "'ReturnStmt' outside of a function.\n";
      std::exit(1);
   }
   return_stack.push(fn_counter);
   return evaluate_stmt(env, static_cast<ReturnStmt&>(*stmt.get()).value);
}

Value Interpreter::evaluate_push(Environment& env, Stmt stmt) {
   auto& psh = static_cast<PushStmt&>(*stmt.get());
   auto value = evaluate_stmt(env, psh.stmt);

   if (value->type == ValueType::number) {
      stack::push(value->as_number());
   } else if (value->type != ValueType::null) {
      auto string = value->as_string();
      for (int i = string.size() - 1; i >= 0; --i) {
         stack::push(string[i]);
      }
   } else {
      std::cerr << "Invalid value to push to stack.\n";
      std::exit(1);
   }
   return value;
}

Value Interpreter::evaluate_type(Environment& env, Stmt stmt) {
   auto& typ = static_cast<TypeStmt&>(*stmt.get());
   auto value = evaluate_stmt(env, typ.stmt);
   auto result = long(value->type);

   stack::push(result);
   return NumberValue::make(result);
}

Value Interpreter::evaluate_pull(Environment& env, Stmt stmt) {
   if (stack::empty()) {
      std::cerr << "'#': Expected stack to not be empty.\n";
      std::exit(1);
   }
   return NumberValue::make(stack::pop());
}

Value Interpreter::evaluate_import(Environment& env, Stmt stmt) {
   auto& imp = static_cast<ImportStmt&>(*stmt.get());
   auto code = evaluate_stmt(env, imp.import)->as_string();
   std::ifstream file (code);
   code = (file.is_open() ? std::string{std::istreambuf_iterator<char>{file}, {}} : code);
   file.close();
   
   Lexer lexer (code);
   auto& tokens = lexer.lex();

   Parser parser (tokens);
   auto& program = parser.parse();

   return evaluate(program, env);
}

// Expression evaluation functions

Value Interpreter::evaluate_expr(Environment& env, Stmt stmt) {
   switch (stmt->type) {
   case StmtType::ternary:
      return evaluate_ternary_expr(env, stmt);
   case StmtType::call:
      return evaluate_call_expr(env, stmt);
   case StmtType::command:
      return evaluate_command(env, stmt);
   default:
      return evaluate_primary_expr(env, stmt);
   }
}

Value Interpreter::evaluate_ternary_expr(Environment& env, Stmt stmt) {
   auto& ternary = static_cast<TernaryExpr&>(*stmt.get());
   auto cond = (!stack::empty() && stack::pop());
   return evaluate_stmt(env, (cond ? ternary.left : ternary.right));
}

Value Interpreter::evaluate_call_expr(Environment& env, Stmt stmt) {
   auto& call = static_cast<CallExpr&>(*stmt.get());
   std::vector<Value> args;
   for (auto& arg : call.args) {
      args.push_back(evaluate_stmt(env, arg));
   }

   if (call.identifier->type != StmtType::identifier) {
      return this->call(env, evaluate_call_expr(env, call.identifier), args);
   } else {
      return this->call(env, env.get(static_cast<IdentLiteral&>(*call.identifier.get()).identifier), args);
   }
}

Value Interpreter::evaluate_command(Environment& env, Stmt stmt) {
   auto& command = static_cast<Command&>(*stmt.get());
   Value final = Null::make();
   auto times = (command.right.has_value() ? evaluate_stmt(env, command.right.value())->as_number() : 1);

   for (int i = 0; i < times; ++i) {
      switch (command.op) {
      case Type::tilde: {
         int num = 0;
         std::cin >> num;
         std::cin.clear();
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         stack::push(num);
         final = NumberValue::make(num);
         break;
      }
      case Type::grave: {
         termios oldt, newt;
         tcgetattr(STDIN_FILENO, &oldt);
         newt = oldt;
         newt.c_lflag &= ~(ICANON | ECHO);
         tcsetattr(STDIN_FILENO, TCSANOW, &newt);
         char ch = getchar();
         tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
         stack::push(ch);
         final = NumberValue::make(ch);
         break;
      }
      case Type::exclamation:
         if (stack::empty()) {
            std::cerr << "'!': Expected stack to not be empty.\n";
            std::exit(1);
         }
         stack::push(!stack::pop());
         final = NumberValue::make(stack::top());
         break;
      case Type::at:
         std::exit(0);
      case Type::dollar:
         final = NumberValue::make(stack::pop());
         break;
      case Type::percent: {
         if (stack::size() < 2) {
            std::cerr << "'%': Expected stack to have at least 2 values.\n";
            std::exit(1);
         }
         long a = stack::pop(), b = stack::pop();
         if (a == 0) {
            std::cerr << "Division by zero error.\n";
            std::exit(1);
         }
         long result = b % a;
         stack::push(result);
         final = NumberValue::make(result);
         break;
      }
      case Type::caret: {
         if (stack::empty()) {
            std::cerr << "'^': Expected stack to not be empty.\n";
            std::exit(1);
         }
         long result = std::sqrt(stack::pop());
         stack::push(result);
         final = NumberValue::make(result);
         break;
      }
      case Type::ampersand: {
         std::string string;
         std::getline(std::cin >> std::ws, string);
         for (int i = string.size() - 1; i >= 0; --i) {
            stack::push(string[i]);
         }
         final = StringValue::make(string);
         break;
      }
      case Type::asterisk: {
         if (stack::size() < 2) {
            std::cerr << "'*': Expected stack to have at least 2 values.\n";
            std::exit(1);
         }
         long result = stack::pop() * stack::pop();
         stack::push(result);
         final = NumberValue::make(result);
         break;
      }
      case Type::hyphen: {
         if (stack::size() < 2) {
            std::cerr << "'-': Expected stack to have at least 2 values.\n";
            std::exit(1);
         }
         long a = stack::pop(), b = stack::pop();
         long result = b - a;
         stack::push(result);
         final = NumberValue::make(result);
         break;
      }
      case Type::plus: {
         if (stack::size() < 2) {
            std::cerr << "'+': Expected stack to have at least 2 values.\n";
            std::exit(1);
         }
         long result = stack::pop() + stack::pop();
         stack::push(result);
         final = NumberValue::make(result);
         break;
      }
      case Type::equal: {
         if (stack::size() < 2) {
            std::cerr << "'=': Expected stack to have at least 2 values.\n";
            std::exit(1);
         }
         bool result = stack::pop() == stack::pop();
         stack::push(result);
         final = NumberValue::make(result);
         break;
      }
      case Type::backslash: {
         long a = stack::pop(), b = stack::pop();
         stack::push(a);
         stack::push(b);
         final = NumberValue::make(stack::top());
         break;
      }
      case Type::colon:
         stack::push(stack::top());
         final = NumberValue::make(stack::top());
         break;
      case Type::apostrophe: {
         if (stack::empty()) {
            std::cerr << "''': Expected stack to not be empty.\n";
            std::exit(1);
         }
         stack::push(-stack::pop());
         final = NumberValue::make(stack::top());
         break;
      }
      case Type::comma:
         if (stack::empty()) {
            std::cerr << "',': Expected stack to not be empty.\n";
            std::exit(1);
         }
         std::cout << char(stack::pop());
         break;
      case Type::less: {
         if (stack::size() < 2) {
            std::cerr << "'<': Expected stack to have at least 2 values.\n";
            std::exit(1);
         }
         long result = stack::pop() > stack::pop();
         stack::push(result);
         final = NumberValue::make(result);
         break;
      }
      case Type::greater: {
         if (stack::size() < 2) {
            std::cerr << "'>': Expected stack to have at least 2 values.\n";
            std::exit(1);
         }
         long result = stack::pop() < stack::pop();
         stack::push(result);
         final = NumberValue::make(result);
         break;
      }
      case Type::period:
         if (stack::empty()) {
            std::cerr << "'.': Expected stack to not be empty.\n";
            std::exit(1);
         }
         std::cout << stack::pop();
         break;
      case Type::slash: {
         if (stack::size() < 2) {
            std::cerr << "'/': Expected stack to have at least 2 values.\n";
            std::exit(1);
         }
         long a = stack::pop(), b = stack::pop();
         if (a == 0) {
            std::cerr << "Division by zero error.\n";
            std::exit(1);
         }
         long result = b / a;
         stack::push(result);
         final = NumberValue::make(result);
         break;
      }
      case Type::size: {
         long size = stack::size();
         stack::push(size);
         final = NumberValue::make(size);
         break;
      }
      default:
         std::cerr << "Unknown command '" << int(command.op) << "'.\n";
         std::exit(1);
         break;
      }
   }
   return final;
}

Value Interpreter::evaluate_primary_expr(Environment& env, Stmt stmt) {
   switch (stmt->type) {
   case StmtType::identifier:
      return env.get(static_cast<IdentLiteral&>(*stmt.get()).identifier);
   case StmtType::number:
      return NumberValue::make(static_cast<NumberLiteral&>(*stmt.get()).number);
   case StmtType::string:
      return StringValue::make(static_cast<StringLiteral&>(*stmt.get()).string);
   case StmtType::program: {
      Environment new_env (&env);
      return evaluate(static_cast<Program&>(*stmt.get()), new_env);
   }
   default:
      std::cerr << "Unexpected expression while evaluating.\n";
      std::exit(1);
   }
}
