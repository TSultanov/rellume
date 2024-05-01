#ifndef RELLUME_MINILIBC_H
#define RELLUME_MINILIBC_H

#include <llvm/IR/Function.h>

llvm::Function* create_write_stub(llvm::LLVMContext& ctx, llvm::Module& mod);

#endif //RELLUME_MINILIBC_H
