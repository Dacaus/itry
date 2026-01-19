#include "Lexer.hpp"
#include "Token.hpp"
#include <endian.h>
#include <iostream>

int main() {
  auto sources = "+";
  itry::Lexer lexer1 = itry::Lexer(sources);
  auto token = lexer1.scanToken();
  std::cout << token.to_string() << "\n";
  return 0;
}