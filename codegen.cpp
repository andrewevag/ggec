#include "ast.hpp"
#include "symbol.hpp"

static llvm::LLVMContext TheContext;
static std::unique_ptr<llvm::Module> TheModule =  std::make_unique<llvm::Module>("mainprogram", TheContext);
static std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM =  std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
static std::unique_ptr<llvm::IRBuilder<> > Builder = std::make_unique<llvm::IRBuilder<> >(TheContext);

llvm::Value* Program::codegen(){
	this->sem();
	return nullptr;
}

llvm::Value* VariableDeclaration::codegen(){
	if(currentScope->nestingLevel == 0){
		// this is a global variable!!
		// getNewGlobalVariable in LLVM
		llvm::Value* val = new llvm::GlobalVariable(
			*TheModule,
			toLLVMType(this->_typeExpr->toType()),
			false,
			llvm::GlobalValue::PrivateLinkage,
			nullptr,
			this->_name
		);
		return val;
	}else{
		// the offset will suffice
		// this offset if fixed in the symbol table by calling it
		return nullptr;
	}
}