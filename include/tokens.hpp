#ifndef TOKENS_HPP
#define TOKENS_HPP

// Includes

#include <string>
#include <unordered_map>
#include <unordered_set>

// Tokens

enum class Type {
   number, string, identifier, keyword, eof,
   tilde, grave, exclamation, at, hash, dollar, percent, caret, ampersand,
   asterisk, open_paren, close_paren, hyphen, plus, equal, open_brace,
   close_brace, open_bracket, close_bracket, pipe, backslash, colon,
   semicolon, apostrophe, comma, less, greater, period, slash, question,
   times, size, thn, els,
};

struct Token {
   Type type;
   std::string lexeme;
};

// Maps

constexpr int max_op_size = 2;
static const std::unordered_map<std::string_view, Type> operators {
   {"`", Type::tilde}, {"~", Type::grave}, {"!", Type::exclamation}, {"@", Type::at},
   {"#", Type::hash}, {"$", Type::dollar}, {"%", Type::percent}, {"^", Type::caret},
   {"&", Type::ampersand}, {"*", Type::asterisk}, {"(", Type::open_paren},
   {")", Type::close_paren}, {"-", Type::hyphen}, {"+", Type::plus}, {"=", Type::equal},
   {"{", Type::open_brace}, {"}", Type::close_brace}, {"[", Type::open_bracket},
   {"]", Type::close_bracket}, {"|", Type::pipe}, {"\\", Type::backslash},
   {":", Type::colon}, {";", Type::semicolon}, {"'", Type::apostrophe},
   {",", Type::comma}, {"<", Type::less}, {">", Type::greater}, {".", Type::period},
   {"/", Type::slash}, {"?", Type::question},
};

static const std::unordered_set<std::string_view> keywords {
   "VAR", "FUN", "WHL", "BRK", "CON", "RET", "IMP",
};

static const std::unordered_map<std::string_view, Type> keyword_ops {
   {"PSH", Type::semicolon}, {"X", Type::times}, {"S", Type::size}, {"TYP", Type::question},
   {"PUL", Type::hash},
};

#endif
