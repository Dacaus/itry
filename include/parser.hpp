
#pragma once
#include "ast.hpp"
#include "token.hpp"
#include <initializer_list>
#include <vector>
namespace itry {
class Parser {

public:
  Parser() {};
  std::vector<Stmt> parse(std::vector<Token> tokens);

private:
  std::vector<Stmt> parseProgram();
  std::unique_ptr<Block> parseBlock();
  Stmt parseStmt();
  Stmt parseFunctionDeclStmt();
  Stmt parseIdentifierDeclStmt();
  Stmt parseAssignStmt();
  Stmt parseReturnStmt();
  Stmt parseExpressionStmt();
  Expr parseExpression();
  Expr parseTerm();
  Expr parseFactor();
  Expr parsePrimary();
  Expr parseNumber();
  Expr parseFunctionCallExpr();
  Expr parseIdentifier();
  Expr parseParenthesizedExpr();

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
    else
      throw std::runtime_error("Parser: Unexpected end of input.");

    return previous();
  }

  Token consume(TokenType type, const std::string &errorMessage) {
    if (peek().getType() == type)
      return advance();
    throw std::runtime_error("Parser: " + errorMessage +
                             " Found: " + peek().to_string());
  }

  Token previous() { return tokens[pos - 1]; }
  Token peek() { return tokens[pos]; }
  bool isAtEnd() { return pos >= tokens.size(); }

  void error(const std::string &message) {
    throw std::runtime_error("Parser Error: " + message);
  }
private:
  int pos;
  std::vector<Token> tokens;
  std::vector<Stmt> statements;
};
} // namespace itry