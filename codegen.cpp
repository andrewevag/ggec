#include "ast.hpp"
#include "symbol.hpp"
#include "error.hpp"
#include <iostream>
#include <fstream>
#define forCurrentScope(e) for(SymbolEntry* e = currentScope->entries; e != NULL; e = e->nextInScope)
#define llvmPointer(inner) llvm::PointerType::get(inner, 0)

llvm::LLVMContext AST::TheContext;
std::unique_ptr<llvm::Module> AST::TheModule =  std::make_unique<llvm::Module>("mainprogram", TheContext);
std::unique_ptr<llvm::legacy::FunctionPassManager> AST::TheFPM =  std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
std::unique_ptr<llvm::IRBuilder<> > AST::Builder = std::make_unique<llvm::IRBuilder<> >(TheContext);
llvm::Type* AST::i8 = llvm::IntegerType::get(TheContext, 8);
llvm::Type* AST::i16 = llvm::IntegerType::get(TheContext, 16);
llvm::Type* AST::i8p = llvm::PointerType::get(i8, 0);
llvm::Type* AST::i64 = llvm::IntegerType::get(TheContext, 64);

llvm::Function* AST::newF = llvm::Function::Create(
		llvm::FunctionType::get(AST::i8p,{AST::i16},false),
		llvm::Function::ExternalLinkage,
		"_newF",
		*AST::TheModule
	);
llvm::Function* AST::deleteF = llvm::Function::Create(
		llvm::FunctionType::get(llvm::Type::getVoidTy(AST::TheContext),{AST::i8p},false),
		llvm::Function::ExternalLinkage,
		"_deleteF",
		*AST::TheModule
	);

// DISCLAIMER IF NOTHING WORKS CHANGE *TheModule to TheModule.get()

llvm::Value* Program::codegen(){
	
	TheFPM->add(llvm::createPromoteMemoryToRegisterPass());
    TheFPM->add(llvm::createInstructionCombiningPass());
    TheFPM->add(llvm::createReassociatePass());
    TheFPM->add(llvm::createGVNPass());
    TheFPM->add(llvm::createCFGSimplificationPass());
    TheFPM->doInitialization();

	this->sem();
	
	//==========================================================
	// Main Wrapper for exiting to system with exit code 0
	//==========================================================
	llvm::Function* ProgramMain;
	llvm::Function* main = 
		llvm::Function::Create(
			llvm::FunctionType::get(i8, {}, false),
			llvm::Function::ExternalLinkage,
			"main",
			*TheModule
		);
	llvm::BasicBlock *FB = llvm::BasicBlock::Create(TheContext, "entry", main);
	Builder->SetInsertPoint(FB);
	
	SymbolEntry* e;
	forCurrentScope(e)
		if(e->entryType == ENTRY_FUNCTION && std::string(e->id) == "main")
			if(! e->u.eFunction.isForward)
				ProgramMain = e->u.eFunction.fun;
	Builder->CreateCall(ProgramMain, {});
	Builder->CreateRet(c8(0));
	closeScope();
	
	//==========================================================
	//==========================================================
	//==========================================================


	bool bad = verifyModule(*TheModule, &llvm::errs());
	if(bad){
		TheModule->print(llvm::outs(), nullptr);
		fatal("Failed to verify module");
	}

	std::ofstream f;
	std::string str;
	llvm::raw_string_ostream output(str);
	f.open("out.ll", std::ios::trunc | std::ios::out);
	TheModule->print(output, nullptr);
	f << str;
	f.close();
	destroySymbolTable();
	return nullptr;
}

llvm::Value* VariableDeclaration::codegen(){
	SymbolEntry * e = lookupEntry(this->_name.c_str(), LOOKUP_ALL_SCOPES, false);
	if(e->nestingLevel == GLOBAL_SCOPE){
		// this is a global variable!!
		// getNewGlobalVariable in LLVM
		auto tt = this->_typeExpr->toType();
		llvm::Value* val = new llvm::GlobalVariable(
			*TheModule,
			toLLVMType(tt),
			false,
			llvm::GlobalValue::CommonLinkage,
			llvm::ConstantAggregateZero::get(toLLVMType(tt)),
			this->_name
		);
		e->u.eVariable.llvmVal = val;
		destroyType(tt);
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
	if(e->nestingLevel == GLOBAL_SCOPE){
		// this is a global variable!!
		// getNewGlobalVariable in LLVM
		auto tt = this->_typeExpr->toType();
		llvm::Value* val = new llvm::GlobalVariable(
			*TheModule,
			// toLLVMType(this->_typeExpr->toType()),
			llvm::ArrayType::get(toLLVMType(tt),this->_expr->isIntConstant()),
			false,
			llvm::GlobalValue::CommonLinkage,
			llvm::ConstantAggregateZero::get(llvm::ArrayType::get(toLLVMType(tt),this->_expr->isIntConstant())),
			this->_name
		);
		e->u.eVariable.llvmVal = val;
		destroyType(tt);
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
void FunctionHead::declare(){
	// Get the Function to put in Symbol Table
	// If this is a nested Function then it needs to have an extra argument
	// of type i8* which is the %env.
	std::cout << "In declare!!!\n";
	SymbolEntry * e = lookupEntry(this->getName().c_str(), LOOKUP_ALL_SCOPES, false);

	
	std::vector<llvm::Type*> parameterTypes;
	if(e->nestingLevel > GLOBAL_SCOPE){
		// we are a nested function so we need an extra argument 
		// for the environment... i8* 
		//
		parameterTypes.push_back(llvm::PointerType::get(i8, 0));
	}
	// Get the types for all parameters
	for (auto &par : this->_parameters->_parameters){
		auto tt = par->getType()->toType();
		if(par->getPassingWay() == Parameter::PassingWay::ByRef){
			parameterTypes.push_back(llvmPointer(toLLVMType(tt))); 
		}else 
			parameterTypes.push_back(toLLVMType(tt));
		destroyType(tt);
	}
	auto tt = this->_resultType->toType();
	llvm::Function* f = 
		llvm::Function::Create(
			llvm::FunctionType::get(toLLVMType(tt), 
									parameterTypes, false),
			llvm::Function::ExternalLinkage,
			this->getName() == "main" ? "_main" : this->getName(),
			*TheModule
		);
	

	e->u.eFunction.fun = f;
	e->u.eFunction.hasHead = true;
	destroyType(tt);
}

llvm::Value* FunctionDeclaration::codegen() {
	this->declare();
	return nullptr;
}

/* When will sem() call it.
 * 
 * 
 */
llvm::Value* FunctionDefinition::codegen(){
	// If we have defined it aka FunctionDeclaration then on the symbol table
	// is the llvm::Function for it. -> so just grab it.
	// SymbolEntry * e = lookupEntry(this->getName().c_str(), LOOKUP_ALL_SCOPES, false);
	SymbolEntry * e = lookupActiveFun();
	// Else define the llvm::Function.
	llvm::Function* f;
	f = e->u.eFunction.fun;

	std::cout << "<><><>When Definining The function<><><>" << std::endl;
	f->print(llvm::outs());
	std::cout << "<><><><><><>" << std::endl;

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
	auto tt = this->_resultType->toType();
	if ( equalType(tt, typeVoid) ){
		Builder->CreateRetVoid();
	}else{
		// this means that the last block of a non void returning function is not terminated
		// control will never reach here because all returns should have been called before getting 
		// to this part. Thus this is dead code and will be removed by the analysis
		// This creates errors if the last block has a phi so a trap block is better.
		llvm::BasicBlock *LB = llvm::BasicBlock::Create(TheContext, "lastBlock", f);
		Builder->CreateBr(LB);
		Builder->SetInsertPoint(LB);
		Builder->CreateBr(LB);
	}
	destroyType(tt);
	
	// Oprimize the Function
	TheFPM->run(*f);

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
	lblEntry->u.eLabel.entry     = LoopHeadBlock;
	lblEntry->u.eLabel.exit      = EndForBlock;
	lblEntry->u.eLabel.loopAfter = LoopAfterBlock;
	

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
	Builder->SetInsertPoint(EndForBlock);
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

	Builder->CreateBr(lblEntry->u.eLabel.loopAfter);
	SymbolEntry * e   = lookupActiveFun();
	llvm::Function* f = e->u.eFunction.fun;
	
	// Basic Blocks for Jumping
	llvm::BasicBlock *AfterContinue  = llvm::BasicBlock::Create(TheContext, "afterContinue", f);
	Builder->SetInsertPoint(AfterContinue);
	return nullptr;
}
llvm::Value* BreakStatement::codegen(){
	SymbolEntry * lblEntry ;
	if(this->_target == ""){
		lblEntry = lookupLabel(NULL, false);
		std::cout << lblEntry->id << std::endl;
	}else{
		lblEntry = lookupLabel(this->_target.c_str(), true);
	}

	Builder->CreateBr(lblEntry->u.eLabel.exit);
	SymbolEntry * e   = lookupActiveFun();
	llvm::Function* f = e->u.eFunction.fun;
	
	// Basic Blocks for Jumping
	llvm::BasicBlock *AfterBreak  = llvm::BasicBlock::Create(TheContext, "afterBreak", f);
	Builder->SetInsertPoint(AfterBreak);
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
	SymbolEntry * e = lookupActiveFun();
	llvm::Function * f = e->u.eFunction.fun;
	// Basic Blocks for Going after this is terminated
	llvm::BasicBlock * NewEntryBlock = llvm::BasicBlock::Create(TheContext, "newEntry", f);
	Builder->SetInsertPoint(NewEntryBlock);
	return nullptr;
}
llvm::Value* Id::codegen(){
	/*
	 * 
	 */
	SymbolEntry * e = lookupEntry(this->_name.c_str(), LOOKUP_ALL_SCOPES, false);
	auto nestingLevel = e->nestingLevel;
	
	llvm::Value* crtPtr = this->calculateAddressOf();
	llvm::Value* valOfVar = crtPtr;
	
	if (! (e->u.eVariable.type->kind == Type_tag::TYPE_ARRAY)){
		valOfVar = Builder->CreateLoad(crtPtr, this->_name);
	}
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
		evalArgs.push_back(getEnvAt(e->nestingLevel));
	}

	// #define forParameters(i, g) for(SymbolEntry* i = g->u.eFunction.firstArgument; i != NULL; i = i->u.eParameter.next)
	SymbolEntry * i = e->u.eFunction.firstArgument;
	for(auto &arg : this->_arguments->_expressions){
		if(i->u.eParameter.mode == PASS_BY_REFERENCE){
			evalArgs.push_back(arg->calculateAddressOf());
		}else
			evalArgs.push_back(arg->codegen());
		i = i->u.eParameter.next;
	}
	printSymbolTable();
	llvm::Function* f = e->u.eFunction.fun;
	std::cout<< f<< std::endl;
	f->print(llvm::outs());

	return Builder->CreateCall(f,evalArgs);
	

}

llvm::Value* BracketedIndex::codegen(){
	llvm::Value* brAdd = this->calculateAddressOf();
	return Builder->CreateLoad(brAdd,"indexed");
}

llvm::Value* UnaryOp::codegen(){
	switch (this->_UnOp){
			case ADDRESS: return this->_operand->calculateAddressOf();
			case DEREF:   return Builder->CreateLoad(this->_operand->codegen(),"deref");
			case POS: 	  return this->_operand->codegen();
			case NEG: 	  
				if(equalType(this->_t, typeInteger))
					return Builder->CreateSub(c16(0),this->_operand->codegen(),"negint");
				else
					return Builder->CreateFNeg(this->_operand->codegen(),"negfp");
			case NOT: 	  
				llvm::Value* cond = Builder->CreateICmpEQ(c8(0),this->_operand->codegen(),"cond");
				return Builder->CreateZExt(cond,i8,"not");
		}
	
	return nullptr;
}
llvm::Value* BinaryOp::codegen(){
	llvm::Value* left;
	llvm::Value* right;
	switch (this->_BinOp){
		case MULT:
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			if(equalType(this->_t, typeInteger))
				return Builder->CreateMul(left,right,"multint");
			else
				return Builder->CreateFMul(left,right,"multfp");
		case DIV:
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			if(equalType(this->_t, typeInteger))
				return Builder->CreateSDiv(left,right,"divint");
			else
				return Builder->CreateFDiv(left,right,"divfp");
		case MOD:
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			return Builder->CreateSRem(left,right,"modint");
		case PLUS:
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			if(equalType(this->_t, typeInteger))
				return Builder->CreateAdd(left,right,"plusint");
			else if(equalType(this->_t, typeReal))
				return Builder->CreateFAdd(left,right,"plusfp");
			else
				return Builder->CreateGEP(left,right,"plusptr");
		case MINUS:
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			if(equalType(this->_t, typeInteger))
				return Builder->CreateSub(left,right,"minusint");
			if(equalType(this->_t, typeReal))
				return Builder->CreateFSub(left,right,"minusfp");
			else{
				llvm::Value* minus = Builder->CreateSub(c16(0),right,"negoff");
				return Builder->CreateGEP(left,minus,"minusptr");
			}
		case LESS: 
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			if(equalType(this->_leftOperand->getType(), typeInteger) ||
			   equalType(this->_leftOperand->getType(), typeChar) ||
			   equalType(this->_leftOperand->getType(), typeBoolean)){
				llvm::Value* cmp = Builder->CreateICmpSLT(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"less");
			}
			if(equalType(this->_leftOperand->getType(), typeReal)){
				llvm::Value* cmp = Builder->CreateFCmpOLT(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"less");
			}else{
				llvm::Value* ptr1 = Builder->CreatePtrToInt(left,i64,"ltint");
				llvm::Value* ptr2 = Builder->CreatePtrToInt(right,i64,"rtint");
				llvm::Value* cmp = Builder->CreateICmpSLT(ptr1,ptr2,"cmp");
				return Builder->CreateZExt(cmp,i8,"less");
			}
		case GREATER:
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			if(equalType(this->_leftOperand->getType(), typeInteger) || 
			   equalType(this->_leftOperand->getType(), typeChar) ||
			   equalType(this->_leftOperand->getType(), typeBoolean)){
				llvm::Value* cmp = Builder->CreateICmpSGT(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"gr");
			}
			if(equalType(this->_leftOperand->getType(), typeReal)){
				llvm::Value* cmp = Builder->CreateFCmpOGT(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"gr");
			}else{
				llvm::Value* ptr1 = Builder->CreatePtrToInt(left,i64,"ltint");
				llvm::Value* ptr2 = Builder->CreatePtrToInt(right,i64,"rtint");
				llvm::Value* cmp = Builder->CreateICmpSGT(ptr1,ptr2,"cmp");
				return Builder->CreateZExt(cmp,i8,"gr");
			}
		case LESSEQ:
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			if(equalType(this->_leftOperand->getType(), typeInteger) || 
			   equalType(this->_leftOperand->getType(), typeChar) || 
			   equalType(this->_leftOperand->getType(), typeBoolean)){
				llvm::Value* cmp = Builder->CreateICmpSLE(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"leq");
			}
			if(equalType(this->_leftOperand->getType(), typeReal)){
				llvm::Value* cmp = Builder->CreateFCmpOLE(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"leq");
			}else{
				llvm::Value* ptr1 = Builder->CreatePtrToInt(left,i64,"ltint");
				llvm::Value* ptr2 = Builder->CreatePtrToInt(right,i64,"rtint");
				llvm::Value* cmp = Builder->CreateICmpSLE(ptr1,ptr2,"cmp");
				return Builder->CreateZExt(cmp,i8,"leq");
			}
		case GREATEREQ:
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			if(equalType(this->_leftOperand->getType(), typeInteger) || 
			   equalType(this->_leftOperand->getType(), typeChar) || 
			   equalType(this->_leftOperand->getType(), typeBoolean)){
				llvm::Value* cmp = Builder->CreateICmpSGE(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"greq");
			}
			if(equalType(this->_leftOperand->getType(), typeReal)){
				llvm::Value* cmp = Builder->CreateFCmpOGE(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"greq");
			}else{
				llvm::Value* ptr1 = Builder->CreatePtrToInt(left,i64,"ltint");
				llvm::Value* ptr2 = Builder->CreatePtrToInt(right,i64,"rtint");
				llvm::Value* cmp = Builder->CreateICmpSGE(ptr1,ptr2,"cmp");
				return Builder->CreateZExt(cmp,i8,"greq");
			}
		case EQUALS:
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			if(equalType(this->_leftOperand->getType(), typeInteger) || 
			   equalType(this->_leftOperand->getType(), typeChar) || 
			   equalType(this->_leftOperand->getType(), typeBoolean)){
				llvm::Value* cmp = Builder->CreateICmpEQ(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"eq");
			}
			if(equalType(this->_leftOperand->getType(), typeReal)){
				llvm::Value* cmp = Builder->CreateFCmpOEQ(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"eq");
			}else{
				llvm::Value* ptr1 = Builder->CreatePtrToInt(left,i64,"ltint");
				llvm::Value* ptr2 = Builder->CreatePtrToInt(right,i64,"rtint");
				llvm::Value* cmp = Builder->CreateICmpEQ(ptr1,ptr2,"cmp");
				return Builder->CreateZExt(cmp,i8,"eq");
			}
		case NOTEQ:
			left = this->_leftOperand->codegen();
			right = this->_rightOperand->codegen();
			if(equalType(this->_leftOperand->getType(), typeInteger) || 
			   equalType(this->_leftOperand->getType(), typeChar) || 
			   equalType(this->_leftOperand->getType(), typeBoolean)){
				llvm::Value* cmp = Builder->CreateICmpNE(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"neq");
			}
			if(equalType(this->_leftOperand->getType(), typeReal)){
				llvm::Value* cmp = Builder->CreateFCmpONE(left,right,"cmp");
				return Builder->CreateZExt(cmp,i8,"neq");
			}else{
				llvm::Value* ptr1 = Builder->CreatePtrToInt(left,i64,"ltint");
				llvm::Value* ptr2 = Builder->CreatePtrToInt(right,i64,"rtint");
				llvm::Value* cmp = Builder->CreateICmpNE(ptr1,ptr2,"cmp");
				return Builder->CreateZExt(cmp,i8,"neq");
			}
		case LAND:{
			llvm::Value* cmpl;
			SymbolEntry * e = lookupActiveFun();
			llvm::Function * f = e->u.eFunction.fun;
			llvm::BasicBlock* CurrentBB = Builder->GetInsertBlock();
			llvm::BasicBlock* ResultKnown        = llvm::BasicBlock::Create(TheContext, "resultKnown", f);
			llvm::BasicBlock* NeedEvaluateSecond = llvm::BasicBlock::Create(TheContext, "needEvaluateSecond", f);

			// Evaluate the left hand side. If it is 0 then the result is known
			// to be zero 
			left = this->_leftOperand->codegen();
			cmpl = Builder->CreateICmpEQ(left,c8(0),"cmp");
			Builder->CreateCondBr(cmpl, ResultKnown, NeedEvaluateSecond);
			CurrentBB = Builder->GetInsertBlock();

			// Evaluate the right hand side since the left hand side wasn't enough to know the result
			Builder->SetInsertPoint(NeedEvaluateSecond);
			right = this->_rightOperand->codegen();
			Builder->CreateBr(ResultKnown);
			NeedEvaluateSecond = Builder->GetInsertBlock();

			// Adjust the result based on the path
			Builder->SetInsertPoint(ResultKnown);
			llvm::PHINode* phi = Builder->CreatePHI(toLLVMType(this->getType()), 2, "val");
			// If from currentBB => the result was known to be 0.
			phi->addIncoming(c8(0), CurrentBB);
			// if from NeedEvaluateSecond the result is right since 
			// true && x = x;
			phi->addIncoming(right, NeedEvaluateSecond);
			return phi;
			}
		case LOR:{
			llvm::Value* cmpl;
			SymbolEntry * e = lookupActiveFun();
			llvm::Function * f = e->u.eFunction.fun;
			llvm::BasicBlock* CurrentBB = Builder->GetInsertBlock();
			llvm::BasicBlock* ResultKnown        = llvm::BasicBlock::Create(TheContext, "resultKnown", f);
			llvm::BasicBlock* NeedEvaluateSecond = llvm::BasicBlock::Create(TheContext, "needEvaluateSecond", f);

			// Evaluate the left hand side. If it is true then the result is known
			// to be true
			left = this->_leftOperand->codegen();
			cmpl = Builder->CreateICmpNE(left,c8(0),"cmp");
			Builder->CreateCondBr(cmpl, ResultKnown, NeedEvaluateSecond);
			CurrentBB = Builder->GetInsertBlock();

			// Evaluate the right hand side since the left hand side wasn't enough to know the result
			Builder->SetInsertPoint(NeedEvaluateSecond);
			right = this->_rightOperand->codegen();
			Builder->CreateBr(ResultKnown);
			NeedEvaluateSecond = Builder->GetInsertBlock();

			// Adjust the result based on the path
			Builder->SetInsertPoint(ResultKnown);
			llvm::PHINode* phi = Builder->CreatePHI(toLLVMType(this->getType()), 2, "val");
			// If from currentBB => the result was known to be 1.
			phi->addIncoming(c8(1), CurrentBB);
			// if from NeedEvaluateSecond the result is right since 
			// false || x = x;
			phi->addIncoming(right, NeedEvaluateSecond);
			return phi;
			}
		case COMMA: return nullptr;
	}
	return nullptr;

}


llvm::Value* PrefixUnAss::codegen(){
	llvm::Value* addr = this->_operand->calculateAddressOf();
	llvm::Value* val = Builder->CreateLoad(addr,"val");
	llvm::Value* nval;
	if(this->_Unass == PLUSPLUS){
		if(equalType(this->_t, typeInteger)){
			nval = Builder->CreateAdd(val,c16(1),"nval");
		}
		else if(equalType(this->_t, typeReal)){
			nval = Builder->CreateFAdd(val,llvm::ConstantFP::get(toLLVMType(typeReal), 1.0L),"nval");
		}else{
			nval = Builder->CreateGEP(val,c64(1),"nval");
		}
		
	}else{
		if(equalType(this->_t, typeInteger)){
			nval = Builder->CreateSub(val,c16(1),"nval");
		}
		else if(equalType(this->_t, typeReal)){
			nval = Builder->CreateFSub(val,llvm::ConstantFP::get(toLLVMType(typeReal),1.0L),"nval");
		}else{
			// llvm::Value* minus = Builder->CreateSub(c16(0),c16(1),"negoff");
			nval = Builder->CreateGEP(val,c16(-1),"minusptr");
		}

	}
	Builder->CreateStore(nval,addr);
	return nval;
}
llvm::Value* PostfixUnAss::codegen(){
	llvm::Value* addr = this->_operand->calculateAddressOf();
	llvm::Value* val = Builder->CreateLoad(addr,"val");
	llvm::Value* nval;
	
	if(this->_Unass == PLUSPLUS){
		std::cout << this->_operand->getType()->kind << std::endl;
		if(equalType(this->_t, typeInteger)){
			nval = Builder->CreateAdd(val,c16(1),"nval");
		}
		else if(equalType(this->_operand->getType(), typeReal)){
			nval = Builder->CreateFAdd(val,llvm::ConstantFP::get(toLLVMType(typeReal), 1.0L),"nval");
		}else{
			nval = Builder->CreateGEP(val,c64(1),"nval");
		}
		
	}else{
		if(equalType(this->_t, typeInteger)){
			nval = Builder->CreateSub(val,c16(1),"nval");
		}
		else if(equalType(this->_t, typeReal)){
			nval = Builder->CreateFSub(val,llvm::ConstantFP::get(toLLVMType(typeReal),1.0L),"nval");
		}else{
			// llvm::Value* minus = Builder->CreateSub(c16(0),c16(1),"negoff");
			nval = Builder->CreateGEP(val,c16(-1),"minusptr");
		}

	}
	Builder->CreateStore(nval,addr);
	return val;
}
llvm::Value* BinaryAss::codegen(){
	llvm::Value* addr = this->_leftOperand->calculateAddressOf();
	llvm::Value* right = this->_rightOperand->codegen();
	llvm::Value* left = Builder->CreateLoad(addr,"left");
	llvm::Value* nval = nullptr;
	switch (this->_BinAss){
		case ASS: 
			nval = right;
			break;
		case MULTASS: 
			if(equalType(this->_t, typeInteger))
				nval = Builder->CreateMul(left,right,"multint");
			else
				nval = Builder->CreateFMul(left,right,"multfp");
			break;
		case DIVASS:
			if(equalType(this->_t, typeInteger))
				nval = Builder->CreateSDiv(left,right,"divint");
			else
				nval = Builder->CreateFDiv(left,right,"divfp");
			break;
		case MODASS:
				nval = Builder->CreateSRem(left,right,"modint");
				break;
		case PLUSASS:
			if(equalType(this->_t, typeInteger))
				nval = Builder->CreateAdd(left,right,"plusint");
			else if(equalType(this->_t, typeReal))
				nval = Builder->CreateFAdd(left,right,"plusfp");
			else
				nval = Builder->CreateGEP(left,right,"plusptr");
			break;
		case MINUSASS:
			if(equalType(this->_t, typeInteger))
				nval = Builder->CreateSub(left,right,"minusint");
			else if(equalType(this->_t, typeReal))
				nval = Builder->CreateFSub(left,right,"minusfp");
			else{
				llvm::Value* minus = Builder->CreateSub(c16(0),right,"negoff");
				nval = Builder->CreateGEP(left,minus,"minusptr");
			}
	}
	Builder->CreateStore(nval,addr);
	return nval;
}
llvm::Value* TypeCast::codegen(){
	llvm::Value* inner = this->_expr->codegen();
	auto tt = this->_type->toType();
	size_t index = tt->kind * 9 +  this->_expr->getType()->kind;
	llvm::Value* casted = inner;
	llvm::Value* castedInter;
	#define getCast(to, from) Type_tag::to * 9 + Type_tag::from
	switch (index)
	{
	// TO INTEGER TYPECASTS
	case getCast(TYPE_INTEGER, TYPE_BOOLEAN): 
		casted = Builder->CreateZExt(inner, i16, "casted");
		break;
	case getCast(TYPE_INTEGER, TYPE_CHAR): 
		casted = Builder->CreateSExt(inner, i16, "casted");
		break;
	case getCast(TYPE_INTEGER, TYPE_REAL): 
		casted = Builder->CreateFPToSI(inner, i16, "casted");
		break;
	case getCast(TYPE_INTEGER, TYPE_POINTER): 
	case getCast(TYPE_INTEGER, TYPE_ARRAY):
		casted = Builder->CreatePtrToInt(inner, i16, "casted");
		break;
	

	// TO BOOL TYPECASTS
	case getCast(TYPE_BOOLEAN, TYPE_INTEGER): 
		casted = Builder->CreateTrunc(inner, i8, "casted");
		break;
	case getCast(TYPE_BOOLEAN, TYPE_REAL): 
		casted = Builder->CreateFPToUI(inner, i8, "casted");
		break;
	case getCast(TYPE_BOOLEAN, TYPE_POINTER): 
	case getCast(TYPE_BOOLEAN, TYPE_ARRAY): 
		casted = Builder->CreatePtrToInt(inner, i8, "casted");
		break;

	// TO CHAR TYPECASTS
	case getCast(TYPE_CHAR, TYPE_INTEGER): 
		casted = Builder->CreateTrunc(inner, i8, "casted");
		break;
	case getCast(TYPE_CHAR, TYPE_REAL): 
		casted = Builder->CreateFPToSI(inner, i8, "casted");
		break;
	case getCast(TYPE_CHAR, TYPE_POINTER): 
	case getCast(TYPE_CHAR, TYPE_ARRAY): 
		casted = Builder->CreatePtrToInt(inner, i8, "casted");
		break;

	// TO DOUBLE TYPECASTS
	case getCast(TYPE_REAL, TYPE_CHAR): 
	case getCast(TYPE_REAL, TYPE_INTEGER): 
		casted = Builder->CreateSIToFP(inner, toLLVMType(this->getType()), "casted");
		break;
	case getCast(TYPE_REAL, TYPE_BOOLEAN): 
		casted = Builder->CreateUIToFP(inner, toLLVMType(this->getType()), "casted");
		break;
	case getCast(TYPE_REAL, TYPE_POINTER): 
	case getCast(TYPE_REAL, TYPE_ARRAY): 
		castedInter = Builder->CreatePtrToInt(inner, i16, "inter.cast");
		casted = Builder->CreateUIToFP(castedInter, toLLVMType(this->getType()), "casted");
		break;
	
	// TO POINTER TYPECASTS
	case getCast(TYPE_POINTER, TYPE_INTEGER): 
	case getCast(TYPE_POINTER, TYPE_BOOLEAN): 
	case getCast(TYPE_POINTER, TYPE_CHAR):
		casted = Builder->CreateIntToPtr(inner, toLLVMType(this->getType()), "casted");
		break;
	case getCast(TYPE_POINTER, TYPE_REAL): 
		castedInter = Builder->CreateFPToUI(inner, i16, "inter.cast");
		casted = Builder->CreateIntToPtr(castedInter, toLLVMType(this->getType()), "casted");
		break;
	case getCast(TYPE_POINTER, TYPE_POINTER): 
	case getCast(TYPE_POINTER, TYPE_ARRAY): 
		casted = Builder->CreateBitCast(inner, toLLVMType(this->getType()), "casted");
		break;
	default:
		break;
	}
	destroyType(tt);
	return casted;
}
llvm::Value* TernaryOp::codegen(){
	SymbolEntry * e = lookupActiveFun();
	llvm::Function * f = e->u.eFunction.fun;

	// Basic Blocks for Jumping
	llvm::BasicBlock * IfTrueBlock = llvm::BasicBlock::Create(TheContext, "iftrue", f);
	llvm::BasicBlock * IfFalseBlock = llvm::BasicBlock::Create(TheContext, "iffalse", f);
	llvm::BasicBlock * EndIfBlock = llvm::BasicBlock::Create(TheContext, "endif", f);
	
	llvm::BasicBlock * EndOfIfTrueBlock;
	llvm::BasicBlock * EndOfIfFalseBlock;

	llvm::Value * conditionVal = this->_condition->codegen();
	auto condBit = Builder->CreateICmpNE(conditionVal, c8(0), "cond");
	Builder->CreateCondBr(condBit, IfTrueBlock, IfFalseBlock);
	// Build If body;
	Builder->SetInsertPoint(IfTrueBlock);
	llvm::Value* ifbody = this->_ifBody->codegen();
	Builder->CreateBr(EndIfBlock);
	EndOfIfTrueBlock = Builder->GetInsertBlock();

	// Build Else body;
	Builder->SetInsertPoint(IfFalseBlock);
	llvm::Value* elsebody = this->_elseBody->codegen();
	Builder->CreateBr(EndIfBlock);
	EndOfIfFalseBlock = Builder->GetInsertBlock();

	// Go to EndIfBlock
	Builder->SetInsertPoint(EndIfBlock);
	llvm::PHINode* phi = Builder->CreatePHI(toLLVMType(this->_t), 2, "val");
	phi->addIncoming(ifbody,EndOfIfTrueBlock);
	phi->addIncoming(elsebody,EndOfIfFalseBlock);


	return phi;
}

llvm::Value* New::codegen(){
	llvm::Value* index;
	auto tt = this->_type->toType();
	if(this->_size == nullptr)
		index = c16(sizeOfType(tt));
	else{
		// ex. new int[16] 
		llvm::Value* size = this->_size->codegen();
		auto castedNull = llvm::Constant::getNullValue(toLLVMType(this->_t));
		auto indexBy1   = Builder->CreateGEP(castedNull, c64(1), "indexedNull");
		auto sizeOfT    = Builder->CreatePtrToInt(indexBy1, i16);
		index = Builder->CreateMul(size,sizeOfT,"idx");
	}
	llvm::Value* rawptr = Builder->CreateCall(newF,{index},"rawptr");
	destroyType(tt);
	return Builder->CreateBitCast(rawptr,toLLVMType(this->_t),"castptr");
}

llvm::Value* Delete::codegen(){
	// free((i8*) this->_t)
	llvm::Value* ptrToDelete = this->_expr->codegen();
	llvm::Value* rawPtr = Builder->CreateBitCast(ptrToDelete, i8p, "rawptr");
	Builder->CreateCall(deleteF, { rawPtr });
	return llvm::Constant::getNullValue(toLLVMType(this->getType()));
}

llvm::Value* CommaExpr::codegen(){
	llvm::Value* left = this->_left->codegen();
	llvm::Value* right = this->_right->codegen();
	return right;
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
	 *	call getEnvAt(nestinglevel of callee - 1)
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
	
	if(e->nestingLevel == GLOBAL_SCOPE){
		// then the value for it is in 
		// std::cout << "======================||===================\n";
		// TheModule->print(llvm::outs(), nullptr);
		// std::cout << "===========================================\n";
		
		// auto temp = Builder->CreateBitCast(e->u.eVariable.llvmVal,llvmPointer(toLLVMType(e->u.eVariable.type)));
		// llvm::Value* value = Builder->CreateGEP(temp, c64(0), this->_name);
		// return value;
		llvm::Value* val = e->u.eVariable.llvmVal;
		if(e->u.eVariable.type->kind == Type_tag::TYPE_ARRAY){
			val = Builder->CreateBitCast(val,(toLLVMType(e->u.eVariable.type)));
		}
		return val;
	}else{
		if(e->nestingLevel > GLOBAL_SCOPE && e->u.eVariable.type->kind == Type_tag::TYPE_ARRAY) {
			llvm::Value* envOfVariable = getEnvAt(e->nestingLevel);
			// get pointer to the offset. TypeCast it to type of Id * 
			llvm::Value* rawBytePtr = Builder->CreateGEP(envOfVariable, c64(e->u.eVariable.offset), "rawBytePtr");
			llvm::Value* actualVarPointer = Builder->CreateBitCast(
				rawBytePtr, 
				(toLLVMType(e->u.eVariable.type)),
				this->_name + ".ptr"
			);
			// return the pointer.
			return actualVarPointer;		
		}
		if(e->entryType != ENTRY_PARAMETER || (e->entryType == ENTRY_PARAMETER && e->u.eParameter.mode == PASS_BY_VALUE)){
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
		if(e->entryType == ENTRY_PARAMETER && e->u.eParameter.mode == PASS_BY_REFERENCE){
			llvm::Value* envOfVariable = getEnvAt(e->nestingLevel);
			// get pointer to the offset. TypeCast it to type of Id * 
			llvm::Value* rawBytePtr = Builder->CreateGEP(envOfVariable, c64(e->u.eVariable.offset), "rawBytePtr");
			llvm::Value* actualVarPointer = Builder->CreateBitCast(
				rawBytePtr, 
				llvmPointer(llvmPointer(toLLVMType(e->u.eVariable.type))),
				this->_name + ".ptr"
			);
			actualVarPointer = Builder->CreateLoad(actualVarPointer, this->_name + ".byref.ptr");
			// return the pointer.
			return actualVarPointer;		
		}
		
		
	}
	return nullptr;
}
llvm::Value* Constant::calculateAddressOf() {
	fatal("calculateAddressOf on Constant.");
	return nullptr;
}
llvm::Value* FunctionCall::calculateAddressOf() {
	fatal("calculateAddressOf on FunctionCall.");
	return nullptr;
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
	return nullptr;

}
llvm::Value* BinaryOp::calculateAddressOf() {
	fatal("calculateAddressOf on BinaryOp.");
	return nullptr;
}
llvm::Value* UnAss::calculateAddressOf() {
	fatal("calculateAddressOf on UnAss.");
	return nullptr;

}
llvm::Value* BinaryAss::calculateAddressOf() {
	fatal("calculateAddressOf on BinaryAss.");
	return nullptr;
}
llvm::Value* TypeCast::calculateAddressOf() {
	fatal("calculateAddressOf on TypeCast.");
	return nullptr;
}
llvm::Value* TernaryOp::calculateAddressOf() {
	fatal("calculateAddressOf on TernaryOp.");
	return nullptr;
}
llvm::Value* New::calculateAddressOf() {
	fatal("calculateAddressOf on New");
	return nullptr;
}
llvm::Value* Delete::calculateAddressOf() {
	fatal("calculateAddressOf on Delete.");
	return nullptr;
}
llvm::Value* CommaExpr::calculateAddressOf() {
	fatal("calculateAddressOf on FunctionCall.");
	return nullptr;
}

llvm::Value* PrefixUnAss::calculateAddressOf(){
	fatal("calculateAddressOf on PrefixUnAss.");
	return nullptr;
}

llvm::Value* PostfixUnAss::calculateAddressOf(){
	fatal("calculateAddressOf on PostfixUnAss");
	return nullptr;
}
