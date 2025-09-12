#include "lexer.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
   if (argc != 2) {
      std::cerr << "Expected two arguments.\n";
      std::exit(1);
   }

   std::string code = argv[1];
   std::ifstream file (code);
   if (file.is_open()) {
      std::string temp;
      code.clear();
      
      while (std::getline(file, temp)) {
         code += temp + '\n';
      }
   }
   
   std::vector<Token> tokens;
   lex(code, tokens);

   for (const auto& t : tokens) {
      std::cout << int(t.type) << " " << t.lexeme << '\n';
   }
   return 0;
}
