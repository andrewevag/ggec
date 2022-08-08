#include "ast.hpp"

AST::~AST(){}

Program::~Program(){
	delete _decls;
//	std::cout << "Deleting Program\n";
}

Declaration::~Declaration(){
//	std::cout << "Deleting Declaration\n";
}

VariableDeclaration::~VariableDeclaration(){ 
	delete _typeExpr;
//	std::cout << "Deleting Variable Declaration\n";
}

ArrayDeclaration::~ArrayDeclaration(){ 
	delete _expr;
//	std::cout << "Deleting Array Declaration\n";
}

FunctionDeclaration::~FunctionDeclaration() { 
	delete _resultType;
	delete _parameters;
 }

FunctionDefinition::~FunctionDefinition(){ 
	delete _resultType;
	delete _parameters;
	delete _decls;
	delete _statements;
}

Parameter::~Parameter(){
	delete _type; 
}

TypeExpression::~TypeExpression(){}


BasicType::~BasicType(){}

Pointer::~Pointer(){
	delete _inner;
}

Statement::~Statement(){}

EmptyStatement::~EmptyStatement(){}

SingleExpression::~SingleExpression(){
	delete _expr;
}

IfStatement::~IfStatement(){
	delete _condition; 
	delete _ifbody;
}
// to condition mporei na einai nullptr
IfElseStatement::~IfElseStatement() {
	delete _condition;
	delete _ifbody; 
	delete _elsebody;
}

ForStatement::~ForStatement(){ 
	
	delete _label;
	delete _first;
	delete _second;
	delete _third;
}

ContinueStatement::~ContinueStatement(){}

BreakStatement::~BreakStatement(){}

ReturnStatement::~ReturnStatement(){
	delete _expr;
}

Expression::~Expression(){}

Constant::~Constant(){};

FunctionCall::~FunctionCall(){
	delete _arguments;
}

BracketedIndex::~BracketedIndex(){
	delete _in; 
	delete _out;
}

UnaryOp::~UnaryOp(){
	delete _operand;
}

BinaryOp::~BinaryOp(){
	delete _leftOperand; 
	delete _rightOperand;
}


UnAss::~UnAss(){
	delete _operand;
}

PrefixUnAss::~PrefixUnAss(){}

PostfixUnAss::~PostfixUnAss(){}


BinaryAss::~BinaryAss(){
	delete _rightOperand; 
	delete _leftOperand;
}

TypeCast::~TypeCast(){
	delete _expr; 
	delete _type;
}


TernaryOp::~TernaryOp(){
	delete _condition;
	delete _ifBody; 
	delete _elseBody;
}

New::~New(){
	delete _size;
	delete _type;
	delete _type;
}

Delete::~Delete(){
	delete _expr;
}

CommaExpr::~CommaExpr(){
	delete _left;
	delete _right;
}

Label::~Label(){}

StatementList::~StatementList(){
	for(auto& i : this->_stmts) 
		delete i;
}

ParameterList::~ParameterList() {
	for(auto& i : this->_parameters)
		delete i;
}

ExpressionList::~ExpressionList(){
	for(auto& i : this->_expressions)
		delete i;
}

DeclarationList::~DeclarationList(){
	for(auto& i : this->_decls)
		delete i;
}