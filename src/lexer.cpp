#include "lexer.hpp"

// Includes

#include <iostream>
#include <unordered_map>

// Lexer functions

Lexer::Lexer(const std::string& code)
   : code(code) {}

std::vector<Token>& Lexer::lex() {
   for (int i = 0; i < code.size(); ++i) {
      char ch = code.at(i);
      
      if (isspace(ch)) {
         continue;
      }

      if (ch == '/' && i + 1 < code.size() && code.at(i + 1) == '*') {
         for (; i + 1 < code.size(); ++i) {
            if (code.at(i) == '*' && code.at(i + 1) == '/') {
               ++i;
               break;
            }
         }

         if (i >= code.size()) {
            std::cerr << "Unterminated comment.\n";
            std::exit(1);
         }
      } else if (isdigit(ch)) {
         std::string number;
         
         for (; i < code.size() && isdigit(code.at(i)); ++i) {
            number += code.at(i);
         }
         tokens.push_back({Type::number, number});
         --i;
      } else if (isalpha(ch)) {
         std::string identifier;

         for (; i < code.size() && (isalnum(code.at(i)) || code.at(i) == '_'); ++i) {
            identifier += code.at(i);
         }

         if (keywords.find(identifier) != keywords.end()) {
            tokens.push_back({Type::keyword, identifier});
         } else {
            tokens.push_back({Type::identifier, identifier});
         }
         --i;
      } else if (ch == '"') {
         std::string string;

         for (++i; i < code.size() && code.at(i) != '"'; ++i) {
            if (code.at(i) == '\\' && i + 1 < code.size()) {
               ++i;
               string += as_escape(code.at(i));
            } else {
               string += code.at(i);
            }
         }
         tokens.push_back({Type::string, string});
         
         if (i >= code.size()) {
            std::cerr << "Unterminated string.\n";
            std::exit(1);
         }
      } else {
         std::string op;
         for (int j = 0; j < max_op_size && j + i < code.size(); ++j) {
            op += code.at(j + i);
         }
         auto original_size = op.size();

         for (int j = 0; j < original_size; ++j) {
            if (auto t = operators.find(op); t != operators.end()) {
               tokens.push_back({t->second, op});
               break;
            }
            op.pop_back();
         }

         if (op.empty()) {
            std::cerr << "Unknown character.\n";
            std::exit(1);
         }
         i += op.size() - 1;
      }
   }
   tokens.push_back({Type::eof, "EOF"});
   return tokens;
}

// Helper functions

char as_escape(char ch) {
   static const std::unordered_map<char, char> escape_codes {
      {'a', '\a'}, {'b', '\b'}, {'t', '\t'}, {'n', '\n'}, {'v', '\v'}, {'f', '\f'},
      {'r', '\r'}, {'e', '\e'}, {'\\', '\\'}, {'\'', '\''}, {'"', '"'}
   };

   if (escape_codes.find(ch) == escape_codes.end()) {
      std::cerr << "Unknown escape code.\n";
      std::exit(1);
   }
   return escape_codes.at(ch);
}
