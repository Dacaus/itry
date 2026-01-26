
#pragma once
#include "ast.hpp"
#include "token.hpp"
#include <initializer_list>
#include <vector>
namespace itry {
class Parser {

public:
  Parser(std::vector<Token> tokens) : tokens(tokens), pos(0) {};

  Expr parseProgram();

  Expr parseFunctionDecl();
  Expr parseVariableDecl();
  Expr parseStatement();
  Expr parseAssignment();
  Expr parseExpression();
  Expr parseFunctionCall();
  Expr parseTerm();
  Expr parseFactor();
  Expr parsePrimary();
  Expr parseNumber();
  Expr parseVariable();
private:

  bool match(std::initializer_list<TokenType> types) {
    if (isAtEnd())
      return false;
    for (auto &type : types) {
      if (type == tokens[pos].getType())
        return true;
    }
    return false;
  }

  Token advance() {
    if (!isAtEnd())
      pos++;
    return previous();
  }
  Token previous() { return tokens[pos - 1]; }
  Token peek() { return tokens[pos]; }
  bool isAtEnd() { return pos >= tokens.size(); }

private:
   
  int pos;
  std::vector<Token> tokens;
  // const static std::map<Binary::Op, int> BinOpPrecedence;
};
} // namespace itry