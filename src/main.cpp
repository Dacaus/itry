#include "irCodegen.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "token.hpp"
#include <endian.h>
#include <iostream>
int main() {
  auto sources = "123.23 * 456 + 789.45 / 456 + 34 - 23 * 45";
  itry::Lexer lexer1 = itry::Lexer(sources);
  auto tokens = lexer1.scanTokens();
  itry::Parser iparser = itry::Parser(tokens);

  auto expr = iparser.parse();
  print(expr);
  itry::ItryIRBuilder irBuilder;
  llvm::Value *value = itry::IRCodegen().operator()(expr, irBuilder);
  std::cout << "Generated LLVM IR:\n";
  value->print(llvm::outs());
  llvm::outs() << "\n";

  if (llvm::ConstantFP *constFP = llvm::dyn_cast<llvm::ConstantFP>(value)) {
    // 获取 APFloat 值
    llvm::APFloat floatVal = constFP->getValueAPF();

    // 转换为 double
    double result = floatVal.convertToDouble();

    std::cout << "Computed result: " << std::setprecision(15) << result
              << std::endl;

    // 或者输出十六进制表示
    std::cout << "Hex representation: " << std::hexfloat << result
              << std::defaultfloat << std::endl;
  }
}