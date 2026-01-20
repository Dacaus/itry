#include "Lexer.hpp"
#include "Token.hpp"
#include <endian.h>
#include <iostream>

int main() {
  auto sources = "123.23 * 456 + 789.45 / 456";
  itry::Lexer lexer1 = itry::Lexer(sources);
  auto tokens = lexer1.scanTokens();
  std::cout<< "The tokens:\n";
  for (auto &token:tokens){
    std::cout<< token.to_string()<<"\n";
  }
  std::cout<<"\n";


}