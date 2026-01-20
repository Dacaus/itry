

#include "Token.hpp"
#include "ast.hpp"
#include <initializer_list>
#include <map>
#include <queue>
#include <vector>
namespace itry {
class Parser {

public:
  Parser(std::vector<Token> tokens) : tokens(tokens), pos(0) {};

  Expr parse();

private:
  Expr parseExpression();
  Expr parseTerm(); // multiply
  Expr parseFactor(); // 

  bool match(std::initializer_list<TokenType> types) {
    for (auto &type : types) {
      if (type == tokens[pos].getType())
        return true;
    }
    return false;
  }
  // int GetOpPrecedence(Binary::Op op);

private:
  int pos;
  std::vector<Token> tokens;
  // const static std::map<Binary::Op, int> BinOpPrecedence;
};
} // namespace itry