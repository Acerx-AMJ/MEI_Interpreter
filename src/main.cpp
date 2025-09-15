// Includes

#include "lexer.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>

// Main function

int main(int argc, char* argv[]) {
   if (argc != 2) {
      std::cerr << "Expected two arguments.\n";
      std::exit(1);
   }

   std::string code = argv[1];
   std::ifstream file (code);
   code = (file.is_open() ? std::string{std::istreambuf_iterator<char>{file}, {}} : code);
   file.close();
   
   Lexer lexer (code);
   auto& tokens = lexer.lex();

   Parser parser (tokens);
   auto& program = parser.parse();
   return 0;
}
