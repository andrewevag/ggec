#include "ast.hpp"
#include "symbol.hpp"
#include "error.hpp"
#include <iostream>
#define llvmPointer(inner) llvm::PointerType::get(inner, 0)

llvm::LLVMContext AST::TheContext;
std::unique_ptr<llvm::Module> AST::TheModule =  std::make_unique<llvm::Module>("mainprogram", TheContext);
std::unique_ptr<llvm::legacy::FunctionPassManager> AST::TheFPM =  std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
std::unique_ptr<llvm::IRBuilder<> > AST::Builder = std::make_unique<llvm::IRBuilder<> >(TheContext);
llvm::Type* AST::i8 = llvm::IntegerType::get(TheContext, 8);
llvm::Type* AST::i16 = llvm::IntegerType::get(TheContext, 16);
llvm::Type* AST::i8p = llvm::PointerType::get(i8, 0);

// DISCLAIMER IF NOTHING WORKS CHANGE *TheModule to TheModule.get()

llvm::Value* Program::codegen(){
	this->sem();

	bool bad = verifyModule(*TheModule, &llvm::errs());
	if(bad){
		fatal("Failed to verift module");
	}
	TheModule->print(llvm::outs(), nullptr);

	return nullptr;
}

llvm::Value* VariableDeclaration::codegen(){
	SymbolEntry * e = lookupEntry(this->_name.c_str(), LOOKUP_ALL_SCOPES, false);
	if(e->nestingLevel == 0){
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
		e->u.eVariable.llvmVal = val;
	}else{
		// the offset will suffice
		// this offset if fixed in the symbol table by calling it
	}
	return nullptr;
}
// TODO Same as the above one maybe refactor it
// sem of array declaration needs to call codegen() on it
// and put the llvm::Value* on the symbol table 
llvm::Value* ArrayDeclaration::codegen(){
	SymbolEntry * e = lookupEntry(this->_name.c_str(), LOOKUP_ALL_SCOPES, false);
	if(e->nestingLevel == 0){
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
		e->u.eVariable.llvmVal = val;
	}else{
		// the offset will suffice
		// this offset if fixed in the symbol table by calling it
	}
	return nullptr;
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
	SymbolEntry * e = lookupEntry(this->getName().c_str(), LOOKUP_ALL_SCOPES, false);

	std::vector<llvm::Type*> parameterTypes;
	if(currentScope->nestingLevel > 0){
		// we are a nested function so we need an extra argument 
		// for the environment... i8* 
		parameterTypes.push_back(llvm::PointerType::get(i8, 0));
	}
	// Get the types for all parameters
	for (auto &par : this->_parameters->_parameters){
		parameterTypes.push_back(toLLVMType(par->getType()->toType()));
	}

	llvm::Function* f = 
		llvm::Function::Create(
			llvm::FunctionType::get(toLLVMType(this->_resultType->toType()), 
									parameterTypes, false),
			llvm::Function::ExternalLinkage,
			this->getName(),
			*TheModule
		);
	

	e->u.eFunction.fun = f;
	e->u.eFunction.hasHead = true;
	return nullptr;
}

/* When will sem() call it.
 * 
 * 
 */
llvm::Value* FunctionDefinition::codegen(){
	// If we have defined it aka FunctionDeclaration then on the symbol table
	// is the llvm::Function for it. -> so just grab it.
	SymbolEntry * e = lookupEntry(this->getName().c_str(), LOOKUP_ALL_SCOPES, false);
	// Else define the llvm::Function.
	llvm::Function* f;
	if ( e->u.eFunction.hasHead == false){
		
		// brand new definition
		std::vector<llvm::Type*> parameterTypes;
		std::cout << "<><><><>" << std::endl;
		std::cout << "FunctionName : " << this->getName() << std::endl;
		std::cout << "CurrentScope : " << e->nestingLevel << std::endl;
		std::cout << "<><><><>" << std::endl;
		if(e->nestingLevel > 1){
			// we are a nested function so we need an extra argument 
			// for the environment... i8* 
			parameterTypes.push_back(llvm::PointerType::get(i8, 0));
		}
		
		// Get the types for all parameters
		for (auto &par : this->_parameters->_parameters){
			if(par->getPassingWay() == Parameter::ByRef){
				parameterTypes.push_back(llvmPointer(toLLVMType(par->getType()->toType())));
			}else{
				parameterTypes.push_back(toLLVMType(par->getType()->toType()));
			}
		}

		f =	llvm::Function::Create(
				llvm::FunctionType::get(toLLVMType(this->_resultType->toType()), 
										parameterTypes, false),
				llvm::Function::ExternalLinkage,
				this->getName(),
				*TheModule
			);
		e->u.eFunction.fun = f;
		e->u.eFunction.hasHead = true;
	}
	else{
		f = e->u.eFunction.fun;
	}

	// Begin the first block of the function!! 
	llvm::BasicBlock *FB = llvm::BasicBlock::Create(TheContext, "entry", f);
	Builder->SetInsertPoint(FB);

	// Build the LocalEnv!! 
	// If we are at 0 nesting level -> no previous static link needs to be stored
	// If we are a nesting function -> store the previous static link given as argument to local env[0];
	size_t offset = currentScope->varOffset;
	llvm::Value* env = Builder->CreateAlloca(i8, 0, c64(offset), "env");


	// f : llvm::Function   "name" ([env], arguments)
	// On symbol table there is 
	// e                    "name" (pars)

	/*
	 * if nested:
	 *    localenv[0] = previousenv;
	 * 	  localenv[8] = firstParameter;
	 *    localenv[8+sizeof(firstParameter)] = secondParameter
	 *    ...
	 * 
	 * if not nested:
	 *    localenv[0] = firstParameter
	 * 	  localenv[0+sizeof(firstParameter)] = secondParameter
	 * 	  ...
	 */
	auto realArg = f->arg_begin();
	if ( e->nestingLevel > 1 ){
		realArg->setName("prevEnv");
		// i8* env;
		// i8* prevEnv;
		// ((i8**)env)[0] = prevEnv
		// ptrToStore = env + 0;
		// * ((i8**) ptrToStore) = prevEnv;

		// %env_0 = getelementptr i8, i8* %env, i16 0
		auto env_0 = Builder->CreateGEP(env, c64(0), "env.0"); 
		// %env_01 = bitcast i8* env_0 to i8**
		auto env_01 = Builder->CreateBitCast(env_0, llvm::PointerType::get(realArg->getType(), 0), "env.0.casted");
		// store i8* %prevEnv, i8** %env_01;
		Builder->CreateStore(realArg, env_01);
		realArg++;
	}
	for (auto par = e->u.eFunction.firstArgument; par!= NULL && realArg != f->arg_end(); realArg++, par = par->u.eParameter.next){
		realArg->setName(par->id);
		// put it in the context
		auto argLevel = Builder->CreateGEP(env, c64(par->u.eParameter.offset), "env.0");
		auto argLevelCasted = Builder->CreateBitCast(argLevel, llvm::PointerType::get(realArg->getType(), 0), "env.0.casted");
		Builder->CreateStore(realArg, argLevelCasted);
	}
	
	
	this->_statements->codegen();

	// TODO remove it
	Builder->CreateRetVoid();
	return nullptr;
}


llvm::Value* TypeExpression::codegen(){
	return nullptr;
}
llvm::Value* Parameter::codegen(){
	return nullptr;
}
llvm::Value* ParameterList::codegen(){
	return nullptr;
}

llvm::Value* BasicType::codegen(){
	return nullptr;
}
llvm::Value* Pointer::codegen(){
	return nullptr;
}

llvm::Value* EmptyStatement::codegen(){
	return nullptr;
}
llvm::Value* SingleExpression::codegen(){
	this->_expr->codegen();
	return nullptr;
}
llvm::Value* IfStatement::codegen(){
	// condition = this->_condition->codegen()
	// conidtion is i8 aka boolean 
	// conditionEval = icmp ne i8 condition, 0
	// br i1 conditionEval, label trueLabel, label Endif  
	// TheModule->get functionStack
	SymbolEntry * e   = lookupActiveFun();
	llvm::Function* f = e->u.eFunction.fun;
	
	// Basic Blocks for Jumping
	llvm::BasicBlock *TrueBlock  = llvm::BasicBlock::Create(TheContext, "iftrue", f);
	llvm::BasicBlock *EndIfBlock = llvm::BasicBlock::Create(TheContext, "endif", f);

	llvm::Value* conditionVal = this->_condition->codegen();
	auto condBit = Builder->CreateICmpNE(conditionVal, c8(0), "cond");
	Builder->CreateCondBr(condBit, TrueBlock, EndIfBlock);
	
	// Build If body;
	Builder->SetInsertPoint(TrueBlock);
	this->_ifbody->codegen();
	Builder->CreateBr(EndIfBlock);
	// Go to EndIfBlock
	Builder->SetInsertPoint(EndIfBlock);

	return nullptr;
}
llvm::Value* IfElseStatement::codegen(){
	return nullptr;
}
llvm::Value* ForStatement::codegen(){
	return nullptr;
}
llvm::Value* ContinueStatement::codegen(){
	return nullptr;
}
llvm::Value* BreakStatement::codegen(){
	return nullptr;
}
llvm::Value* ReturnStatement::codegen(){
	return nullptr;
}
llvm::Value* Id::codegen(){
	return nullptr;
}
llvm::Value* Constant::codegen(){
	// for testing reason i will build true constant
	if(this->_ct == Constant::ConstantType::Bool){
		if(this->_bool){
			auto added = Builder->CreateAdd(c8(10), c8(22));
			return added;

		}else{
			auto added = Builder->CreateAdd(c8(10), c8(22));
			return added;
		}
	}
	return nullptr;
}
llvm::Value* FunctionCall::codegen(){
	return nullptr;
}
llvm::Value* BracketedIndex::codegen(){
	return nullptr;
}
llvm::Value* UnaryOp::codegen(){
	return nullptr;
}
llvm::Value* BinaryOp::codegen(){
	return nullptr;
}
llvm::Value* PrefixUnAss::codegen(){
	return nullptr;
}
llvm::Value* PostfixUnAss::codegen(){
	return nullptr;
}
llvm::Value* BinaryAss::codegen(){
	return nullptr;
}
llvm::Value* TypeCast::codegen(){
	return nullptr;
}
llvm::Value* TernaryOp::codegen(){
	return nullptr;
}
llvm::Value* New::codegen(){
	return nullptr;
}
llvm::Value* Delete::codegen(){
	return nullptr;
}
llvm::Value* CommaExpr::codegen(){
	return nullptr;
}
llvm::Value* Label::codegen(){
	return nullptr;
}
llvm::Value* StatementList::codegen(){
	for(auto &stmt : this->_stmts)
		stmt->codegen();
	return nullptr;
}
llvm::Value* ExpressionList::codegen(){
	return nullptr;
}
llvm::Value* DeclarationList::codegen(){
	return nullptr;
}