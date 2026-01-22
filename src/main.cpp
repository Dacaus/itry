#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <endian.h>
#include <iostream>
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/Error.h"
  

llvm::ExitOnError ExitOnErr;

int main(int argc, char** argv) {

  // Initialize LLVM.
  llvm::InitLLVM X(argc, argv);

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();

  llvm::cl::ParseCommandLineOptions(argc, argv, "HowToUseLLJIT");
  ExitOnErr.setBanner(std::string(argv[0]) + ": ");

  auto sources = "a = 123";
  itry::Lexer lexer1 = itry::Lexer(sources);
  auto tokens = lexer1.scanTokens();



  itry::Parser iparser = itry::Parser(tokens);

  auto expr = iparser.parse();

  itry::AstPrinter printer;
  printer.print(expr);

  // print(expr);
  std::cout << std::endl;
  return 0;
}
