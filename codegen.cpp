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
	if(currentScope->nestingLevel > 1){
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
	e->u.eFunction.env = env;

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
	// Builder->CreateRetVoid();
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
	SymbolEntry * e = lookupActiveFun();
	llvm::Function * f = e->u.eFunction.fun;

	// Basic Blocks for Jumping
	llvm::BasicBlock * IfTrueBlock = llvm::BasicBlock::Create(TheContext, "iftrue", f);
	llvm::BasicBlock * IfFalseBlock = llvm::BasicBlock::Create(TheContext, "iffalse", f);
	llvm::BasicBlock * EndIfBlock = llvm::BasicBlock::Create(TheContext, "endif", f);

	llvm::Value * conditionVal = this->_condition->codegen();
	auto condBit = Builder->CreateICmpNE(conditionVal, c8(0), "cond");
	Builder->CreateCondBr(condBit, IfTrueBlock, IfFalseBlock);

	// Build If body;
	Builder->SetInsertPoint(IfTrueBlock);
	this->_ifbody->codegen();
	Builder->CreateBr(EndIfBlock);

	// Build Else body;
	Builder->SetInsertPoint(IfFalseBlock);
	this->_elsebody->codegen();
	Builder->CreateBr(EndIfBlock);
	
	// Go to EndIfBlock
	Builder->SetInsertPoint(EndIfBlock);

	return nullptr;
}
llvm::Value* ForStatement::codegen(){
	// label: for(ex1 ; ex2; ex3)
	// 				statement
	/*
	 * ex1;
	 * goto loophead:
	 * loophead:
	 * ex2;
	 * if (ex2 == true) goto loopbody else goto endfor
	 * 
	 * loopbody:
	 * statement
	 * goto loopafter
	 * 
	 * loopafter:
	 * ex3;
	 * goto loophead
	 * 
	 * endfor:
	 */
	SymbolEntry * e = lookupActiveFun();
	llvm::Function * f = e->u.eFunction.fun;
	
	SymbolEntry * lblEntry= lookupLabel(this->_label->getLabelName().c_str(), true);
	llvm::BasicBlock * LoopHeadBlock  = llvm::BasicBlock::Create(TheContext, "loophead", f);
	llvm::BasicBlock * LoopBodyBlock  = llvm::BasicBlock::Create(TheContext, "loopbody", f);
	llvm::BasicBlock * LoopAfterBlock = llvm::BasicBlock::Create(TheContext, "loopafter", f);
	llvm::BasicBlock * EndForBlock    = llvm::BasicBlock::Create(TheContext, "endfor", f);
	
	// Put the basic blocks so they can be available to continue && break statements.
	lblEntry->u.eLabel.entry = LoopHeadBlock;
	lblEntry->u.eLabel.exit  = EndForBlock;
	

	if(this->_first != nullptr){
		this->_first->codegen();
	}
	Builder->CreateBr(LoopHeadBlock);

	// loophead
	llvm::Value* condition; // of type bool aka i8
	Builder->SetInsertPoint(LoopHeadBlock);
	if(this->_second != nullptr){
		condition = this->_second->codegen();
	}else{
		condition = c8(1); // defaulting to true.
	}

	auto condBit = Builder->CreateICmpNE(condition, c8(0), "cond");
	Builder->CreateCondBr(condBit, LoopBodyBlock, EndForBlock);

	//loopbody
	Builder->SetInsertPoint(LoopBodyBlock);
	this->_body->codegen();
	Builder->CreateBr(LoopAfterBlock);

	//loopafter
	Builder->SetInsertPoint(LoopAfterBlock);
	if (this->_third != nullptr){
		this->_third->codegen();
	}
	Builder->CreateBr(LoopHeadBlock);

	return nullptr;
}
llvm::Value* ContinueStatement::codegen(){
	/*
	 * continue [label];
	 *  
	 * 
	 */
	SymbolEntry * lblEntry ;
	if(this->_target == ""){
		lblEntry = lookupLabel(NULL, false);
	}else{
		lblEntry = lookupLabel(this->_target.c_str(), true);
	}

	Builder->CreateBr(lblEntry->u.eLabel.entry);
	return nullptr;
}
llvm::Value* BreakStatement::codegen(){
	SymbolEntry * lblEntry ;
	if(this->_target == ""){
		lblEntry = lookupLabel(NULL, false);
	}else{
		lblEntry = lookupLabel(this->_target.c_str(), true);
	}

	Builder->CreateBr(lblEntry->u.eLabel.exit);
	return nullptr;
}
llvm::Value* ReturnStatement::codegen(){
	// return [e].
	if(this->_expr == nullptr){
		Builder->CreateRetVoid();
	}else{
		llvm::Value * retExp = this->_expr->codegen();
		Builder->CreateRet(retExp);
	}
	return nullptr;
}
llvm::Value* Id::codegen(){
	/*
	 * 
	 */
	SymbolEntry * e = lookupEntry(this->_name.c_str(), LOOKUP_ALL_SCOPES, false);
	auto nestingLevel = e->nestingLevel;
	auto varOffset    = ( e->entryType == ENTRY_PARAMETER ) ? e->u.eParameter.offset : e->u.eVariable.offset;
	
	llvm::Value* crtPtr = this->calculateAddressOf();
	llvm::Value* valOfVar = Builder->CreateLoad(crtPtr, this->_name);
	return valOfVar;
}


llvm::Value* Constant::codegen(){
	switch (_ct){
		case Bool:
			return c8(this->_bool ? 1 : 0);
		case Null:   
			// by semantic analysis this needs to be typecasted to be
			// usable
			return llvm::Constant::getNullValue(i8p);
		case Int: 
			return c16(this->_int);
		case Char :  
			return c8(this->_char);
		case Double:
			return llvm::ConstantFP::get(toLLVMType(this->_t), this->_double);
		case String:
			// First allocate the string as a global variable
			// and return the address of it.
			//0. Defs
			auto str = this->_string;
			auto charType = i8;

			//1. Initialize chars vector
			std::vector<llvm::Constant *> chars(str.length());
			for(unsigned int i = 0; i < str.size(); i++) {
			chars[i] = c8(str[i]);
			}

			//1b. add a zero terminator too
			chars.push_back(c8(0));


			//2. Initialize the string from the characters
			auto stringType = llvm::ArrayType::get(charType, chars.size());

			//3. Create the declaration statement
			auto v = new llvm::GlobalVariable(
				*TheModule,
				stringType,
				false,
				llvm::GlobalValue::PrivateLinkage,
				llvm::ConstantArray::get(stringType, chars),
				"string.const"
			);
			//4. Return a cast to an i8*
			llvm::Value* ptr = Builder->CreateBitCast(v, i8p, "string.ptr");
			return ptr;
	}
	return nullptr;

}


llvm::Value* FunctionCall::codegen(){
	std::cout<< "mpika fCall\n";
	std::cout<< this->_functionName << std::endl;
	SymbolEntry* e = lookupEntry(this->_functionName.c_str(), LOOKUP_ALL_SCOPES, false);

	if(e == NULL)
		std::cout<< "enai null i malakia\n";

	std::vector<llvm::Value*> evalArgs;

	if(e->nestingLevel != GLOBAL_SCOPE){
		std::cout<< e->nestingLevel << std::endl;
		evalArgs.push_back(getEnvAt(e->nestingLevel-1));
	}

	for(auto &arg : this->_arguments->_expressions){
		evalArgs.push_back(arg->codegen());
	}

	llvm::Function* f = e->u.eFunction.fun;
	std::cout<< f<< std::endl;
	f->print(llvm::outs());

	return Builder->CreateCall(f,evalArgs,"fres");

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


llvm::Value* AST::getEnvAt(unsigned int nestinglevel){
	auto currNest = currentScope->nestingLevel;
	SymbolEntry *  currFun  = lookupActiveFun();
	auto currEnv = currFun->u.eFunction.env;
	long iter = currNest - nestinglevel;
	
	/*
	 * For calling functions: 
	 *		+1 -> currentNest
	 *		 0 -> currNest - nestinglevel = 1 => nestinglevel = currNest -1
	 *		-1 -> currNest - nestinglevel = 2 => nestinglevel = currNest -2
	 *	call getEnvAt(nestinglevel of callee -1)
	 * 
	 */



	for(int i = 0; i < iter; i++){
		auto bitCastedEnv = Builder->CreateBitCast(currEnv, llvmPointer(i8p), "bitcastedEnv");
		currEnv = Builder->CreateLoad(bitCastedEnv, "prevEnv");
	}
	return currEnv;
}




llvm::Value* Id::calculateAddressOf() {
	// query the Symbol Table for the name && nesting level.
	// get the env of the nested level of the variable (can be global here).
	// get the offset in that environment and add it to the pointer of the env.
	SymbolEntry * e = lookupEntry(this->_name.c_str(), LOOKUP_ALL_SCOPES, false);
	
	if(e->nestingLevel == 1){
		// then the value for it is in 
		llvm::Value* value = Builder->CreateGEP(e->u.eVariable.llvmVal, c64(0), this->_name);
		return value;
	}else{
		llvm::Value* envOfVariable = getEnvAt(e->nestingLevel);
		// get pointer to the offset. TypeCast it to type of Id * 
		llvm::Value* rawBytePtr = Builder->CreateGEP(envOfVariable, c64(e->u.eVariable.offset), "rawBytePtr");
		llvm::Value* actualVarPointer = Builder->CreateBitCast(
			rawBytePtr, 
			llvmPointer(toLLVMType(e->u.eVariable.type)),
			this->_name + ".ptr"
		);
		
		// return the pointer.
		return actualVarPointer;		
	}
	return nullptr;
}
llvm::Value* Constant::calculateAddressOf() {
	fatal("calculateAddressOf on Constant.");

}
llvm::Value* FunctionCall::calculateAddressOf() {
	fatal("calculateAddressOf on FunctionCall.");
}
llvm::Value* BracketedIndex::calculateAddressOf() {
	// Calculate the base pointer
	// this->_out : Expression of type Pointer, this->_in : Expression of type Int.
	llvm::Value* basePtr = this->_out->codegen();
	// Add to it the inner expression
	llvm::Value* offset = this->_in->codegen();
	// return the added pointer
	return Builder->CreateGEP(basePtr, offset, "calc.ptr");
}
llvm::Value* UnaryOp::calculateAddressOf() {
	if(this->_UnOp ==  DEREF){
		// this->_operand : Pointer(t)
		return this->_operand->codegen();
	}else{
		fatal("calculateAddressOf on UnaryOp(%s)\n", UnaryOp::unaryOpTypeToString(this->_UnOp).c_str());
	}
}
llvm::Value* BinaryOp::calculateAddressOf() {
	fatal("calculateAddressOf on BinaryOp.");

}
llvm::Value* UnAss::calculateAddressOf() {
	fatal("calculateAddressOf on UnAss.");

}
llvm::Value* BinaryAss::calculateAddressOf() {
	fatal("calculateAddressOf on BinaryAss.");
}
llvm::Value* TypeCast::calculateAddressOf() {
	fatal("calculateAddressOf on TypeCast.");
}
llvm::Value* TernaryOp::calculateAddressOf() {
	fatal("calculateAddressOf on TernaryOp.");
}
llvm::Value* New::calculateAddressOf() {
	fatal("calculateAddressOf on New");
}
llvm::Value* Delete::calculateAddressOf() {
	fatal("calculateAddressOf on Delete.");
}
llvm::Value* CommaExpr::calculateAddressOf() {
	fatal("calculateAddressOf on FunctionCall.");
}

llvm::Value* PrefixUnAss::calculateAddressOf(){
	fatal("calculateAddressOf on PrefixUnAss.");
}

llvm::Value* PostfixUnAss::calculateAddressOf(){
	fatal("calculateAddressOf on PostfixUnAss");
}
