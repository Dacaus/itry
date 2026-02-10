#pragma once
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <memory>
class ItryJIT {
public:
  static llvm::Expected<std::unique_ptr<ItryJIT>> Create() {
    auto jit = llvm::orc::LLJITBuilder().create();
    if (!jit)
      return jit.takeError();
    return std::make_unique<ItryJIT>(std::move(*jit));
  }

  void addModule(llvm::orc::ThreadSafeModule tsm) {
    // Print the module before adding it to the JIT
    tsm.withModuleDo([](llvm::Module &M) {
      M.print(llvm::outs(), nullptr);
    });
    cantFail(jit->addIRModule(std::move(tsm)));
  }

  llvm::orc::ExecutorAddr lookup(const std::string &name) {
    auto sym = jit->lookup(llvm::StringRef(name));
    if (!sym)
      llvm::cantFail(sym.takeError());
    return *sym;
  }


  void printIR() {
    // Modules are printed when added to the JIT
  }

  explicit ItryJIT(std::unique_ptr<llvm::orc::LLJIT> jit)
      : jit(std::move(jit)) {}
private:
  std::unique_ptr<llvm::orc::LLJIT> jit;
};
