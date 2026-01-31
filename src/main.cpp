
#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"
#include <endian.h>
#include <iostream>

#include "llvm/Support/Error.h"


#include <fstream>
#include <sstream>
#include <string>

// #include "itryIRBuilder.hpp"
#include "itryJIT.hpp"
#include "itryJITIRBuilder.hpp"

llvm::ExitOnError ExitOnErr;

std::string readFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error(std::string("Cannot open file: ") + filename);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void initializeLLVM() {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
}

int main(int argc, char **argv) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
    return 1;
  }

  // Initialize LLVM.
  llvm::InitLLVM X(argc, argv);

  initializeLLVM();

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


  itry::ItryJITIRBuilder builder;
  auto tsm = builder.generateTSM(expr);

  auto jit = cantFail(ItryJIT::Create());
  jit->addModule(std::move(tsm));

  llvm::orc::ExecutorAddr addr = jit->lookup("foo");

  // auto fp = addr.toPtr<double (*)()>();
  using FnTy = double (*)(double, double);
  FnTy fp = addr.toPtr<FnTy>();

  double result = fp(1,2);
  std::cout << "Result: " << result << std::endl; 
  return 0;
}
