#include "ast.hpp"
#include "error.hpp"
#include <sstream> 
#include <string>

/* ---------------------------------------------------------------------
   ----------- Καθολικές μεταβλητές για τον χειρισμό Label -------------
   --------------------------------------------------------------------- */

// std::vector<Label*> NestingNamedloops;
// Variable for generating new names for for loops with no explicit labels.

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

/**
 * @brief Semantically check the condition and the body. 
 * The condition needs to be of type \b bool.
 * 
 */
void IfStatement::sem(){
	this->_condition->sem();
	if(! equalType(this->_condition->getType(), typeBoolean)){
		fatal("In if statement the condition should be of type bool");
	}
	this->_ifbody->sem();
}

/**
 * @brief Semantically check the condition, the ifbody and the else body
 * The condition needs to be of type \b bool.
 * 
 */
void IfElseStatement::sem(){
	this->_condition->sem();
	if(! equalType(this->_condition->getType(), typeBoolean)){
		fatal("In if statement the condition should be of type bool");
	}
	this->_ifbody->sem();
	this->_elsebody->sem();
}

/**
 * @brief Semantically check all three expressions in loop head and for body.
 * The second expressions needs to be of type \b bool.
 * If missing it is defaulted to \b true
 * If a label is present add it in the stack of active labels since it can be used
 * in the loop body.
 * If a label is not present in the for loop header add it with a new name in the 
 * stack of active labels to indicate that we are in a loop
 * 
 */
void ForStatement::sem(){
	if(this->_label != nullptr){
		newLabel(this->_label->getLabelName().c_str());
	}else{
		newLabel();
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
	
	
	//inactivate the label
	SymbolEntry* e;
	if(this->_label != nullptr){
		e = lookupLabel(this->_label->getLabelName().c_str(), true);
		if(e != NULL){
			e->u.eLabel.active = false;
		}else{
			fatal("Internal Error(ForStatement::sem): label added in symbol table and not found afterwards!");
		}
	}else{
		e = lookupLabel(nullptr, false);
		if(e != NULL){
			e->u.eLabel.active = false;
		}else{
			fatal("Internal Error(ForStatement::sem): unnamed label not found afterwards");
		}
	}
}

/**
 * @brief Check if the continue statement is valid.
 * If searching with a name check the symbol table that the label exists
 * and is active.
 * If not searching with a name get the last label (which is the deepest in
 * the case of nested loops) and verify that this is active.
 * If not in loop semantics fail!
 */
void ContinueStatement::sem(){
	SymbolEntry* e;
	if(this->_target != "")
	{
		// check that the label is valid
		e = lookupLabel(this->_target.c_str(), true);
		// existance check
		if(e == NULL){
			fatal("Continue Statement not inside of a for loop named %s\n", this->_target.c_str());
		}
		else{
			// activeness check
			if( ! e->u.eLabel.active ){
				fatal("Continue Statement not inside of a for loop named %s\n", this->_target.c_str());
			}
		}
	}
	else
	{
		e = lookupLabel(nullptr, false);
		// existance check
		if(e == NULL){
			fatal("Continue Statement outside of a for loop\n");
		}
		else{
			// activeness check
			if( ! e->u.eLabel.active ){
				fatal("Continue Statement outside of a for loop\n");
			}
		}
	}
}

/**
 * @brief Check if the break statement is valid.
 * If searching with a name check the symbol table that the label exists
 * and is active.
 * If not searching with a name get the last label (which is the deepest in
 * the case of nested loops) and verify that this is active.
 * If not in loop semantics fail!
 */
void BreakStatement::sem(){
	SymbolEntry* e;

	if(this->_target != "")
	{
		e = lookupLabel(this->_target.c_str(), true);
		// existance check
		if(e == NULL){
			fatal("Break Statement not inside of a for loop named %s\n", this->_target.c_str());
		}else{
			// activeness check
			if( ! e->u.eLabel.active ){
				fatal("Break Statement not inside of a for loop named %s\n", this->_target.c_str());
			}
		}
	}
	else
	{
		e = lookupLabel(nullptr, false);
		// existance check
		if(e == NULL){
			fatal("Break Statement not inside of a for loop");
		}
		else{
			// activeness check
			if( ! e->u.eLabel.active ){
				fatal("Break Statement not inside of a for loop");
			}
		}
	}
}

/**
 * @brief If no expression with the return statement -> check that the function
 * in which the return statement is defined returns void.
 * If the expression exist -> match expression type with return type of the function
 * 
 */
void ReturnStatement::sem() {
	SymbolEntry *e;
	e = lookupActiveFun();
	if (e == NULL)
		fatal("Return Statement Outside of a function");
	else{
		Type resultType = e->u.eFunction.resultType;
		if (this->_expr == nullptr && (! equalType(typeVoid, resultType))){
			fatal("Expected an expression in the return statement");
		}
		if (this->_expr != nullptr){
			// semantically analyze expression.
			this->_expr->sem();
			Type eType = this->_expr->getType();
			if(! equalType(eType, resultType)){
				fatal("Returning type does not match return type of the function");
			}
		}
	}
}


/* ---------------------------------------------------------------------
   ------------- Semantic Analysis For Expressions ---------------------
   --------------------------------------------------------------------- */

/**
 * @brief Check that it is already defined as a variable or parameter
 * and get it's type. It is lval.
 * 
 */
void Id::sem()
{
	SymbolEntry* e = lookupEntry(this->_name.c_str(), LOOKUP_ALL_SCOPES, false);
	if(e == NULL){
		fatal("Identifier not previously declared %s", this->_name.c_str());
	}
	else{
		switch (e->entryType)
		{
		case ENTRY_VARIABLE:
			this->_t = copyType(e->u.eVariable.type);
			break;
		case ENTRY_PARAMETER:
			this->_t = copyType(e->u.eParameter.type);
			break;
		
		default:
			fatal("Identifier %s not variable or parameter\n", this->_name.c_str());
			break;
		}
	}
	this->_isLval = true;
}


/**
 * @brief Figure the type of the constant
 * Is not lval.
 * 
 */
void Constant::sem()
{
	switch (this->_ct)
	{
	case Bool:
		this->_t = copyType(typeBoolean);
		break;
	case Null:
		/**
		 * TODO Figure out it's type and add it here
		 */
		this->_t = copyType(typeAny);
		break;
	case Char:
		this->_t = copyType(typeChar);
		break;
	case Int:
		this->_t = copyType(typeInteger);
		break;
	case Double:
		this->_t = copyType(typeReal);
		break;
	case String:
		this->_t = typePointer(copyType(typeChar));
		break;
	}
	this->_isLval = false;
}

#define forParameters(i, g) for(SymbolEntry* i = g->u.eFunction.firstArgument; i != NULL; i = i->u.eParameter.next)
/**
 * @brief Grab the function from the symbol table.
 * Match the argument types with byrefs required being lval.
 * 
 */
void FunctionCall::sem(){
	SymbolEntry* e = lookupEntry(this->_functionName.c_str(), LOOKUP_ALL_SCOPES, false);
	
	// existance of the name
	if(e == NULL){
		fatal("Calling a function not previously defined");
	}
	// check if the name is a function.
	else if(e->entryType != ENTRY_FUNCTION){
		fatal(std::string(this->_functionName + " is not a function.").c_str());
	}

	// argument matching
	this->_arguments->sem();
	size_t i = 0;
	forParameters(p, e){
		if(p->entryType != ENTRY_PARAMETER){
			fatal("Internal Error: lookup table entry for function doesnt have parameter entries as parameters");
		}
		// match the argument to the expresion
		if (! equalType(p->u.eParameter.type, this->_arguments->_expressions[i]->getType()) ){
			fatal("Argument no: %d does not match with definition of function", i+1);
		}

		// byref -> lval
		// not byref or lval
		if( !  (p->u.eParameter.mode != PASS_BY_REFERENCE || this->_arguments->_expressions[i]->isLval()) ){
			fatal("Argument no: %d expected lval to match byref argument", i+1);
		}
		i++;
	}	

	// type of the expression is the result type of the function.
	this->_t = copyType(e->u.eFunction.resultType);
	this->_isLval = false;
}

/**
 * @brief Check that the thing being accessed is a ptr and the inner 
 * expression is a int expression. This is an lval.
 * 
 */
void BracketedIndex::sem(){
	/* first semantically analyze both the expressions used in the indexing */
	this->_in->sem();
	this->_out->sem();
	if( ! this->_out->isPtrType() ){
		fatal("No pointer type used in array indexing\n");
	}
	if ( ! equalType(this->_in->getType(), typeInteger) ){
		fatal("No integer type used as index in array indexing\n");
	}

	// Figure out type
	this->_t = copyType(this->_out->getType()->refType);
	this->_isLval = true;
}

/**
 * @brief Check the inner expression and figure it's type. Perform the 
 * semantic rules based on the operator.
 * 
 */

void UnaryOp::sem() {
	this->_operand->sem();
	std::string printable[] = {"+", "-"};
	switch (this->_UnOp) {		
	case UnaryOp::ADDRESS:
		if( ! this->_operand->isLval()){
			fatal("& not on l-value");
		}
		this->_t = typePointer(copyType(this->_operand->getType()));
		this->_isLval = false;
		break;
	case UnaryOp::DEREF:
		if ( ! this->_operand->isPtrType() ){
			fatal("* operator used on a non pointer type");
		}
		this->_t = copyType(this->_operand->getType()->refType);
		this->_isLval = true;
		break;
	case UnaryOp::POS: case UnaryOp::NEG:
		if ( ! equalType(this->_operand->getType(), typeInteger) && 
		  	 ! equalType(this->_operand->getType(), typeReal))
		 {
			fatal("Unary %s operator used on a non int or double operand", 
		 		printable[((this->_UnOp == UnaryOp::POS) ? 0: 1)].c_str() );
		 }

		 this->_t = copyType(this->_operand->getType());
		 this->_isLval = false;
		break;
	case UnaryOp::NOT:
		if (! equalType(this->_operand->getType(), typeBoolean)){
			fatal("Unary ! operator used on non bool operrand");
		}
		this->_t = copyType(typeBoolean);
		this->_isLval = false;
		break;
	default:
		break;
	}
}


void binaryOpAnalysis(BinaryOp& b)
{
	// BinaryOp* bop = &b;
	
	b._leftOperand->sem();
	b._rightOperand->sem();
	
	switch (b._BinOp){
		
		case BinaryOp::PLUS: case BinaryOp::MINUS: 
			// int <> int, double <> double, ptr <> int
			b._isLval = false;
			// int <> int
			if (	equalType(b._leftOperand->getType(), typeInteger) 
				&&	equalType(b._rightOperand->getType(), typeInteger)
			)
				b._t = TypedExpression::copyType(typeInteger);

			// double <> double
			else if(equalType(b._leftOperand->getType(), typeReal) 
				&&  equalType(b._rightOperand->getType(), typeReal)
			)
				b._t = TypedExpression::copyType(typeReal);
			
			// ptr <> int
			else if(b._leftOperand->isPtrType()
				&&  equalType(b._rightOperand->getType(), typeInteger)
			)
				b._t = TypedExpression::copyType(b._leftOperand->getType());
			
			else {
				throw b._BinOp;
			}
			break;
		
		case BinaryOp::MULT: case BinaryOp::DIV: 
			// int <> int, double <> double
			b._isLval = false;

			// int <> int
			if (	equalType(b._leftOperand->getType(), typeInteger) 
				&&	equalType(b._rightOperand->getType(), typeInteger)
			)
				b._t = TypedExpression::copyType(typeInteger);

			// double <> double
			else if(equalType(b._leftOperand->getType(), typeReal) 
				&&  equalType(b._rightOperand->getType(), typeReal)
			)
				b._t = TypedExpression::copyType(typeReal);
			
			else {
				throw b._BinOp;
			}

			break;
		
		case BinaryOp::MOD: 
			// int <> int

			b._isLval = false;

			// int <> int
			if (	equalType(b._leftOperand->getType(), typeInteger) 
				&&	equalType(b._rightOperand->getType(), typeInteger)
			)
				b._t = TypedExpression::copyType(typeInteger);
						
			else {
				throw b._BinOp;
			}

			break;
		case BinaryOp::LESS: 
		case BinaryOp::GREATER: 
		case BinaryOp::LESSEQ: 
		case BinaryOp::GREATEREQ: 
		case BinaryOp::EQUALS: 
		case BinaryOp::NOTEQ: 
			// t <> t
			b._isLval = false;
			if (equalType(b._leftOperand->getType(), b._rightOperand->getType()))
			{
				b._t = TypedExpression::copyType(typeBoolean);
			}

			else {
				throw b._BinOp;
			}
			break;
		case BinaryOp::LAND: 
		case BinaryOp::LOR: 
			// bool <> bool
			if (	equalType(b._leftOperand->getType(), typeBoolean) 
				&&	equalType(b._rightOperand->getType(), typeBoolean)
			)
				b._t = TypedExpression::copyType(typeBoolean);
						
			else {
				throw b._BinOp;
			}

			break;
		case BinaryOp::COMMA: 
			// p <> q.
			b._isLval = false;
			b._t = TypedExpression::copyType(b._rightOperand->getType());
			break;
		}
}

/**
 * @brief Semantically analyze binOp
 * 
 */
void BinaryOp::sem(){
	try {
		binaryOpAnalysis(*this);
	} catch (BinaryOpType opr){
		std::string printable[] = {
			"*", "/", "%", "+", "-", "<", ">", "<=", ">=", "==", "!=", "&&", "||", ","
		};

		std::string op = printable[opr];
		switch (opr){
		
		case PLUS: case MINUS:
			fatal(std::string("Operator " + op + " not used on correctly typed operands\n"
					"Expected int " + op + " int or double " + op + "double or t*" + 
					op + "int").c_str());
		
		case MULT: case DIV: 
			fatal(std::string("Operator "+ op +" not used on correctly typed operands\n"
					"Expected int "+ op +" int or double "+ op +" double").c_str());
		
		case MOD: 
			fatal(std::string("Operator "+ op +" not used on correctly typed operands\n"
					"Expected int "+ op +" int").c_str());
		
		case LESS: case GREATER: case LESSEQ: case GREATEREQ: case EQUALS: case NOTEQ: 
			fatal(std::string("Operator "+ op +" not used on same typed operands\n"
					"Expected t "+ op +" t").c_str());
		
		case LAND: case LOR: 
			fatal(std::string("Operator "+ op +" not used on correctly typed operands\n"
					"Expected bool "+ op +" bool").c_str());
		default: break;
		}
	}
}

void PrefixUnAss::sem(){
	this->_operand->sem();
	this->_isLval = false;
	std::string printable[] = { "++", "--"};  
	
	if(! this->_operand->isLval()){
		fatal("operator %s not used on an l-value", printable[this->_Unass].c_str());
	}
	if ( equalType(this->_operand->getType(), typeInteger) 
	  || equalType(this->_operand->getType(), typeReal)
	  || this->_operand->isPtrType()
	 ){
		this->_t = copyType(this->_operand->getType());
	 }else{
		fatal("operator %s not used on correct type\n"
		      "expected int or double or t*", printable[this->_Unass].c_str()
		);
	 }
}

void PostfixUnAss::sem(){
	this->_operand->sem();
	this->_isLval = false;
	std::string printable[] = { "++", "--"};  
	
	if(! this->_operand->isLval()){
		fatal("operator %s not used on an l-value", printable[this->_Unass].c_str());
	}
	if ( equalType(this->_operand->getType(), typeInteger) 
	  || equalType(this->_operand->getType(), typeReal)
	  || this->_operand->isPtrType()
	 ){
		this->_t = copyType(this->_operand->getType());
	 }else{
		fatal("operator %s not used on correct type\n"
		      "expected int or double or t*", printable[this->_Unass].c_str()
		);
	 }
}

/**
 * @brief 
 * 
 */
void BinaryAss::sem(){
	// enum BinaryOpType { MULT, DIV, MOD, PLUS, MINUS, LESS, GREATER, LESSEQ,
	// GREATEREQ, EQUALS, NOTEQ, LAND, LOR, COMMA };
	// enum BinaryAssType { ASS, MULTASS, DIVASS, MODASS, PLUSASS, MINUSASS };

	BinaryOp::BinaryOpType toBinOp[] = { BinaryOp::EQUALS, BinaryOp::MULT, BinaryOp::DIV, 
		BinaryOp::MOD, BinaryOp::PLUS, BinaryOp::MINUS };
	BinaryOp *b = new BinaryOp(
		toBinOp[this->_BinAss], this->_leftOperand, this->_rightOperand
	);

	if( ! this->_leftOperand->isLval() ){
		fatal("Assignemnt not used with l-value");
	}
	try {
		binaryOpAnalysis(*b);
	} catch (BinaryOp::BinaryOpType opr){
		switch(opr){
			case BinaryOp::EQUALS:
				fatal("Not same types on left and right of = assignment");
			case BinaryOp::MULT:
				fatal("Operator *= not used correctly, expected int *= int or double *= double");
			case BinaryOp::DIV:
				fatal("Operator /= not used correctly, expected int /= int or double /= double");
			case BinaryOp::MOD:
				fatal("Operator %= not used correctly, expected int %= int");
			case BinaryOp::PLUS:
				fatal("Operator += not used correctly, expected int += int or double += double or t* += int");
			case BinaryOp::MINUS:
				fatal("Operator -= not used correctly, expected int -= int or double -= double or t* -= int");
			default:
				fatal("Internal error BinaryAss::sem()");
		}
	}

	b->setLeft(nullptr);
	b->setRight(nullptr);
	delete b;

}

void TypeCast::sem(){
	this->_expr->sem();
	// Type t = this->_expr->getType();
	Type t = this->_type->toType();
	this->_t = copyType(t);
	// destroyType(t);
	this->_isLval = false;
}

void TernaryOp::sem(){
	this->_condition->sem();
	this->_ifBody->sem();
	this->_elseBody->sem();
	
	if ( !  equalType(this->_condition->getType(), typeBoolean))
		fatal("Expected boolean in ternary operator condition");
	if ( ! equalType(this->_ifBody->getType(), this->_elseBody->getType()))
		fatal("Expected the same type in the two clauses of ternary operator");
	
	this->_t = copyType(this->_ifBody->getType());
	this->_isLval = false;
}

/**
 * @brief If size is given the expression should be int => return type* 
 *        If size is not given just return type*this->getType()
 * 		  Is not lval.
 * 
 * 
 */
void New::sem(){
	if(this->_size != nullptr){
		this->_size->sem(); 
		if(! equalType(this->getType(), typeInteger)){
			fatal("New operator needs type int as size");
		}
	}
	Type t = this->_type->toType();
	// destroyType(t);
	this->_t = typePointer(copyType(t));
	this->_isLval = false;
}

/**
 * @brief Gets pointer returns pointer is not lval
 * 
 */
void Delete::sem(){
	this->_expr->sem();
	if(! this->_expr->isPtrType())
		fatal("Expected pointer type in delete operator");
	this->_t = copyType(this->_expr->getType());
	this->_isLval = false;
}



void CommaExpr::sem(){
	this->_left->sem();
	this->_right->sem();
	this->_t = copyType(this->_right->getType());
	this->_isLval = false;
}

void StatementList::sem(){
	for(auto &stmt : this->_stmts)
		stmt->sem();
}

void ExpressionList::sem(){
	for(auto &expr : this->_expressions)
		expr->sem();
}

void DeclarationList::sem(){
	for(auto &decl : this->_decls)
		decl->sem();
}



/** Unused **/
void BasicType::sem(){}
void Pointer::sem(){}
void Label::sem(){}