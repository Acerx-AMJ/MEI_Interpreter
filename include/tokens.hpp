#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <string>
#include <unordered_map>

enum class Type {
   number, string, identifier, eof,
   tilde, grave, exclamation, at, hash, dollar, percent, caret, ampersand,
   asterisk, open_paren, close_paren, underscore, hyphen, plus, equal,
   open_brace, close_brace, open_bracket, close_bracket, pipe, backslash,
   colon, semicolon, apostrophe, comma, less, greater, period, slash, question, 
};

struct Token {
   Type type;
   std::string lexeme;
};

constexpr int max_op_size = 2;
static const std::unordered_map<std::string_view, Type> operators {
   {"`", Type::tilde}, {"~", Type::grave}, {"!", Type::exclamation}, {"@", Type::at},
   {"#", Type::hash}, {"$", Type::dollar}, {"%", Type::percent}, {"^", Type::caret},
   {"&", Type::ampersand}, {"*", Type::asterisk}, {"(", Type::open_paren},
   {")", Type::close_paren}, {"_", Type::underscore}, {"-", Type::hyphen},
   {"+", Type::plus}, {"=", Type::equal}, {"{", Type::open_brace}, {"}", Type::close_brace},
   {"[", Type::open_bracket}, {"]", Type::close_bracket}, {"|", Type::pipe},
   {"\\", Type::backslash}, {":", Type::colon}, {";", Type::semicolon},
   {"'", Type::apostrophe}, {",", Type::comma}, {"<", Type::less}, {">", Type::greater},
   {".", Type::period}, {"/", Type::slash}, {"?", Type::question}
};

#endif
