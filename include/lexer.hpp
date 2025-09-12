#ifndef LEXER_HPP
#define LEXER_HPP

#include "tokens.hpp"
#include <vector>

void lex(const std::string& code, std::vector<Token>& tokens);
char as_escape(char ch);

#endif
