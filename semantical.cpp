#include "ast.hpp"
#include "error.hpp"
/* ---------------------------------------------------------------------
   ----------- Καθολικές μεταβλητές για τον χειρισμό Label -------------
   --------------------------------------------------------------------- */

std::vector<Label*> NestingNamedloops;
SymbolEntry* entryForFunction;


/* ---------------------------------------------------------------------
   ------------- Semantic Analysis For Declarations --------------------
   --------------------------------------------------------------------- */


void Program::sem(){
	initSymbolTable(256);
	openScope();
	
	/* perform semantic analysis of the program declarations. */
	this->_decls->sem();
	
	closeScope();
	printSymbolTable();
}

void VariableDeclaration::sem() {
	newVariable(this->getName().c_str(), this->_typeExpr->toType());
}

void ArrayDeclaration::sem(){
	int size = this->_expr->isIntConstant();
	if(size <= 0){
		fatal("Not positive int constant used as size for a constant array!\n");
	}
	newVariable(this->getName().c_str(), typeArray(size, this->_typeExpr->toType()));

}

void FunctionDeclaration::sem(){
	/* This is a forward function declaration */
	//    p = newFunction("f");
	//    forwardFunction(p);
	//    openScope();
	//    newParameter("x", typeInteger, PASS_BY_VALUE, p);
	//    newParameter("y", typeReal, PASS_BY_REFERENCE, p);
	//    endFunctionHeader(p, typeVoid);
	//    closeScope();
	// void f(int g, int k);
	// f_int_int;

	SymbolEntry* f;
	/* Calculate the new name based on the parameter list */
	f = newFunction(this->getName().c_str());
	forwardFunction(f);
	openScope();
	// Should register all parameters of the function
	// !!!!!!!!!!!!!!!!!!!!!!
	// TEMPORARY !!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!
	entryForFunction = f;
	this->_parameters->sem();
	endFunctionHeader(f, this->_resultType->toType());
	closeScope();
}

void FunctionDefinition::sem() {
	SymbolEntry* f;
	f = newFunction(this->getName().c_str());
	openScope();
	this->_parameters->sem();
	endFunctionHeader(f, this->_resultType->toType());

	this->_decls->sem();
	this->_statements->sem();
	closeScope();
}

void Parameter::sem(){
	newParameter(
		this->_name.c_str(), 
		this->_type->toType(), 
		(this->_pw == ByCall) ? PASS_BY_VALUE : PASS_BY_REFERENCE,
		entryForFunction
	);
}

void ParameterList::sem(){
	for(auto &par : this->_parameters)
		par->sem();
}



/* ---------------------------------------------------------------------
   ------------- Semantic Analysis For Statements --------------------
   --------------------------------------------------------------------- */


void EmptyStatement::sem(){}

void SingleExpression::sem(){
	this->_expr->sem();
}

void IfStatement::sem(){
	this->_condition->sem();
	if(! equalType(this->_condition->getType(), typeBoolean)){
		fatal("In if statement the condition should be of type bool");
	}
	this->_ifbody->sem();
}

void IfElseStatement::sem(){
	this->_condition->sem();
	if(! equalType(this->_condition->getType(), typeBoolean)){
		fatal("In if statement the condition should be of type bool");
	}
	this->_ifbody->sem();
	this->_elsebody->sem();
}

void ForStatement::sem(){
	if(this->_label != nullptr){
		NestingNamedloops.push_back(this->_label);
		newLabel(this->_label->getLabelName().c_str());
	}

	if(this->_first != nullptr){
		this->_first->sem();
	}
	if(this->_second != nullptr){
		this->_second->sem();
		if( ! equalType(this->_second->getType(), typeBoolean));
	}else{
		this->_second = new Constant(true);
		// this->_second->sem();
	}
	if(this->_third != nullptr){
		this->_third->sem();
	}

	this->_body->sem();
}



