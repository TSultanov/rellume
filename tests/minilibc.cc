#include "minilibc.h"
#include <llvm/IR/IRBuilder.h>

llvm::Function* create_write_stub(llvm::LLVMContext& ctx, llvm::Module& mod) {
    llvm::IRBuilder builder(ctx);

    auto t_ssize_t = builder.getInt64Ty();
    llvm::Type* arg_types[] = {
            builder.getInt32Ty(),
            builder.getPtrTy(),
            builder.getInt64Ty()
    };
    auto fn_type = llvm::FunctionType::get(t_ssize_t, llvm::ArrayRef(arg_types, 3), false);
    llvm::Function* fn = llvm::Function::Create(fn_type, llvm::Function::ExternalLinkage, "write", mod);
    llvm::BasicBlock *body = llvm::BasicBlock::Create(ctx, "body", fn);
    builder.SetInsertPoint(body);
    auto count = fn->getArg(2);
    builder.CreateRet(count);

    return fn;
}