#include "ast.hpp"


Program::~Program(){
	delete _decls;
}

Declaration::~Declaration(){}

VariableDeclaration::~VariableDeclaration(){ 
	delete _typeExpr; 
}

ArrayDeclaration::~ArrayDeclaration(){ 
	delete this->_typeExpr;
	delete _expr;
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
	delete _body;
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

PrefixUnAss::~PrefixUnAss(){
	delete this->_operand;
}

PostfixUnAss::~PostfixUnAss(){
	delete this->_operand;
}


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