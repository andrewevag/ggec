#include "ast.hpp"
#define nullsafedelete(ptr) if(ptr != nullptr) delete ptr


AST::~AST(){}

Program::~Program(){
	nullsafedelete(_decls);

}

Declaration::~Declaration(){

}

VariableDeclaration::~VariableDeclaration(){ 
	nullsafedelete(_typeExpr);

}

ArrayDeclaration::~ArrayDeclaration(){ 
	nullsafedelete(_expr);

}

FunctionDeclaration::~FunctionDeclaration() { 
	nullsafedelete(_resultType);
	nullsafedelete(_parameters);
 }

FunctionDefinition::~FunctionDefinition(){ 
	nullsafedelete(_resultType);
	nullsafedelete(_parameters);
	nullsafedelete(_decls);
	nullsafedelete(_statements);
}

Parameter::~Parameter(){
	nullsafedelete(_type); 
}

TypeExpression::~TypeExpression(){}


BasicType::~BasicType(){}

Pointer::~Pointer(){
	nullsafedelete(_inner);
}

Statement::~Statement(){}

EmptyStatement::~EmptyStatement(){}

SingleExpression::~SingleExpression(){
	nullsafedelete(_expr);
}

IfStatement::~IfStatement(){
	nullsafedelete(_condition); 
	nullsafedelete(_ifbody);
}
// to condition mporei na einai nullptr
IfElseStatement::~IfElseStatement() {
	nullsafedelete(_condition);
	nullsafedelete(_ifbody); 
	nullsafedelete(_elsebody);
}

ForStatement::~ForStatement(){ 
	
	nullsafedelete(_label);
	nullsafedelete(_first);
	nullsafedelete(_second);
	nullsafedelete(_third);
	nullsafedelete(_body);
}

ContinueStatement::~ContinueStatement(){}

BreakStatement::~BreakStatement(){}

ReturnStatement::~ReturnStatement(){
	nullsafedelete(_expr);
}

Expression::~Expression(){}

Constant::~Constant(){};

FunctionCall::~FunctionCall(){
	nullsafedelete(_arguments);
}

BracketedIndex::~BracketedIndex(){
	nullsafedelete(_in); 
	nullsafedelete(_out);
}

UnaryOp::~UnaryOp(){
	nullsafedelete(_operand);
}

BinaryOp::~BinaryOp(){
	nullsafedelete(_leftOperand); 
	nullsafedelete(_rightOperand);
}


UnAss::~UnAss(){
	nullsafedelete(_operand);
}

PrefixUnAss::~PrefixUnAss(){}

PostfixUnAss::~PostfixUnAss(){}


BinaryAss::~BinaryAss(){
	nullsafedelete(_rightOperand); 
	nullsafedelete(_leftOperand);
}

TypeCast::~TypeCast(){
	nullsafedelete(_expr); 
	nullsafedelete(_type);
}


TernaryOp::~TernaryOp(){
	nullsafedelete(_condition);
	nullsafedelete(_ifBody); 
	nullsafedelete(_elseBody);
}

New::~New(){
	nullsafedelete(_size);
	nullsafedelete(_type);
}

Delete::~Delete(){
	nullsafedelete(_expr);
}

CommaExpr::~CommaExpr(){
	nullsafedelete(_left);
	nullsafedelete(_right);
}

Label::~Label(){}

StatementList::~StatementList(){
	for(auto& i : this->_stmts) 
		nullsafedelete(i);
}

ParameterList::~ParameterList() {
	for(auto& i : this->_parameters)
		nullsafedelete(i);
}

ExpressionList::~ExpressionList(){
	for(auto& i : this->_expressions)
		nullsafedelete(i);
}

DeclarationList::~DeclarationList(){
	for(auto& i : this->_decls)
		nullsafedelete(i);
}