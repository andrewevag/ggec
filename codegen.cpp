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
// TODO Same as the above one maybe refactor it
// sem of array declaration needs to call codegen() on it
// and put the llvm::Value* on the symbol table 
llvm::Value* ArrayDeclaration::codegen(){
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


// TODO sem() needs to put this in the symbol table so it can be accessed

// sem()
// {
//	...
//	codegen()
//	...
//}

llvm::Value* FunctionDeclaration::codegen() {
	// Get the Function to put in Symbol Table
	// If this is a nested Function then it needs to have an extra argument
	// of type i8* which is the %env.
	std::vector<llvm::Type*> parameterTypes;
	if(currentScope->nestingLevel > 0){
		// we are a nested function so we need an extra argument 
		// for the environment...
	}
	for (auto &par : this->_parameters->_parameters){
		parameterTypes.push_back(toLLVMType(par->getType()->toType()));
	}

	llvm::Function* f = 
		llvm::Function::Create(
			llvm::FunctionType::get(toLLVMType(this->_resultType->toType()), 
									parameterTypes, false),
			
		)
}
