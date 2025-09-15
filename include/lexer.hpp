#ifndef LEXER_HPP
#define LEXER_HPP

// Includes

#include "tokens.hpp"
#include <vector>

// Lexer

class Lexer {
   const std::string& code;
   std::vector<Token> tokens;
   
   char as_escape(char ch);

public: 
   Lexer(const std::string& code);
   std::vector<Token>& lex();
};

#endif
