#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"
#include <endian.h>
#include <iostream>


#include "llvm/Support/Error.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"

llvm::ExitOnError ExitOnErr;
#include <fstream>
#include <sstream>
#include <string>

#include "itryIRBuilder.hpp"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char **argv) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
    return 1;
  }

  // Initialize LLVM.
  llvm::InitLLVM X(argc, argv);

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();

  ExitOnErr.setBanner(std::string(argv[0]) + ": ");
  std::string filename = argv[1];
  auto sources = readFile(filename);

  itry::Lexer lexer1 = itry::Lexer(sources);

  auto tokens = lexer1.scanTokens();

  itry::Parser iparser = itry::Parser();

  auto expr = iparser.parse(tokens);

  itry::AstPrinter printer;

#ifdef _DEBUG
  printer.print(expr);
#endif
  itry::ItryIRBuilder irBuilder;
  llvm::Module *module = irBuilder.generateCode(expr);
  irBuilder.printIR();
  std::cout << std::endl;
  return 0;
}
