#ifndef __AST_HPP__
#define __AST_HPP__
#include <vector>
#include <string>
#include <deque>

class AST{
public:
	virtual ~AST() = default;
};
class Declaration;
class TypeExpression;
class Parameter;
class Statement;
class Expression;
class Label;
class StatementList;
class ParameterList;
class ExpressionList;
class DeclarationList;


class Program : public AST{
public:
	Program(std::vector<Declaration*> decls) : _decls(decls) {}
	virtual ~Program() override {
		for(auto &i : this->_decls)
			delete i;
	};
private:
	std::vector<Declaration*> _decls;
};

class Declaration : public AST {
public:
	virtual ~Declaration() override = default;
	virtual void embedType(TypeExpression*) = 0;
};


class VariableDeclaration : public Declaration {
public:
	VariableDeclaration(TypeExpression* typeExpr, std::string name) : _typeExpr(typeExpr), _names(name) {} 
	virtual ~VariableDeclaration() override = default;
	virtual void embedType(TypeExpression* type){
		this->_typeExpr = type;
	}
protected:
	TypeExpression* _typeExpr;
	std::string _name;
};

class ArrayDeclaration : public VariableDeclaration {
public:
	ArrayDeclaration(TypeExpression* typeExp, std::string name, Expression* expr)
	: _expr(expr), VariableDeclaration(typeExp, name) {}
	virtual ~ArrayDeclaration() override = default;
private: 
	Expression* _expr;
};

class FunctionDeclaration : public Declaration {
public:
	FunctionDeclaration(TypeExpression* typeExpr, std::string name, std::vector<Parameter*> parameters) : 
	_resultType(typeExpr), _name(name), _parameters(parameters) {}
	virtual ~FunctionDeclaration() override = default;
	virtual void embedType(TypeExpression* type) {}
private:
	TypeExpression* _resultType;
	std::string _name;
	std::vector<Parameter*> _parameters;

};

class FunctionDefinition : public Declaration {
public:
	FunctionDefinition(TypeExpression* typeExpr, std::string name, std::vector<Parameter*> parameters, 
	std::vector<Statement*> statement) : 
	_resultType(typeExpr), _name(name), _parameters(parameters), _statement(statement) {}
	virtual ~FunctionDefinition() override = default;
	virtual void embedType(TypeExpression* type) {}
private:
	TypeExpression* _resultType;
	std::string _name;
	std::vector<Parameter*> _parameters;
	std::vector<Statement*> _statement;
};

class TypeExpression : public AST {
public:
	virtual ~TypeExpression() override = default;
	virtual void penetrate(BasicType*) = 0;
private:
};

class BasicType : public TypeExpression {
public:
	BasicType(std::string name) : _name(name) {}
	virtual ~BasicType() override = default;
	std::string getName() { return this->_name; }
	virtual void penetrate(BasicType* me){
		this->_name = me->getName();
	}
private:
	std::string _name;
};

class Pointer : public TypeExpression {
public:
	Pointer(TypeExpression* inner) : _inner(inner) {}
	virtual ~Pointer() override = default;
	virtual void penetrate(BasicType* me){
		this->_inner->penetrate(me);
	}
public:
	TypeExpression* _inner;
};

class Statement : public AST {
public:
	virtual ~Statement() override = default;
};

class EmptyStatement : public Statement {
public:
	EmptyStatement() {}
	virtual ~EmptyStatement() override = default;

};

class SingleExpression : public Statement {
public: 
	SingleExpression(Expression* expr)
	: _expr(expr) {}
	virtual ~SingleExpression() override = default;
private:
	Expression* _expr;
};

class IfStatement : public Statement {
public:
	IfStatement(Expression* cond, Statement* ifbody)
	: _condition(cond), _ifbody(ifbody) {}
	virtual ~IfStatement() override = default;
private:
	Expression* _condition;
	Statement* _ifbody;
};

class IfElseStatement : public Statement {
public:
	IfElseStatement(Expression* cond, Statement* ifbody, Statement* elsebody)
	: _condition(cond), _ifbody(ifbody), _elsebody(elsebody) {}
	virtual ~IfElseStatement() override = default;
private:
	Expression* _condition;
	Statement* _ifbody;
	Statement* _elsebody;
};

class ForStatement : public Statement {
public:
	// label can be null and expressions
	ForStatement(Label* lbl, Expression* first, Expression* second, Expression* third, Statement* body)
	: _label(lbl), _first(first), _second(second), _third(third), _body(body) {}
	virtual ~ForStatement() override = default;
private:
	Label* _label;
	Expression* _first, *_second, *_third;
	Statement* _body;
};

class ContinueStatement : public Statement {
public:
	ContinueStatement() : _target("") {}
	ContinueStatement(std::string target) : _target(target) {}
	virtual ~ContinueStatement() override = default;
private:
	//empty string indicating no label
	std::string _target;
};

class BreakStatement : public Statement {
public:
	// can be "" indicating no target label
	BreakStatement() : _target("") {}
	BreakStatement(std::string target) : _target(target) {}
	virtual ~BreakStatement() override = default;
private:
	//empty string indicating no label
	std::string _target;
};

class ReturnStatement : public Statement {
public:
	//the expression can be null indicating no return expression
	ReturnStatement(Expression* expr) : _expr(expr) {}
	virtual ~ReturnStatement() override = default;
private:
	//nullptr indicating no expression in the return statement
	Expression* _expr;
};


class Expression : public AST {
public:
	virtual ~Expression() override = default;
};

class Id : public Expression {
public:
	Id(std::string name) : _name(name) {}
	virtual ~Id() override = default;
private:
	std::string _name;
};

class Constant : public Expression {
public:
	Constant(int16_t i) : _ct(ConstantType::Int), _int(i) {}
	Constant(long double d) : _ct(ConstantType::Double), _double(d) {}
	Constant(char c) : _ct(ConstantType::Char), _char(c) {}
	Constant(std::string s) : _ct(ConstantType::String), _string(s) {}
	Constant(bool f) : _ct(ConstantType::Bool), _bool(f) {}
	Constant() : _ct(ConstantType::Null) {}
	virtual ~Constant() override = default;
	enum ConstantType { Bool, Null, Int, Char, Double, String };

private:
	ConstantType _ct;
	int16_t _int;
	bool _bool;
	char _char;
	long double _double;
	std::string _string;
};

class FunctionCall : public Expression {
public:
	FunctionCall(std::string name, ExpressionList* args) 
	: _functionName(name), _arguments(args) {}
	virtual ~FunctionCall() override = default;
private:
	std::string _functionName;
	ExpressionList* _arguments;
};

class BracketedIndex : public Expression {
public:
	BracketedIndex(Expression* in, Expression* out)
	: _in(in), _out(out) {} 
	virtual ~BracketedIndex() override = default;
private:
	Expression* _in;
	Expression* _out;
};

class UnaryOp : public Expression {
public:
	UnaryOp(int unop, Expression* operand)
	: _UnOp(unop), _operand(operand) {}
	virtual ~UnaryOp() override = default;
private:
	int _UnOp;
	Expression* _operand;
};

class BinaryOp : public Expression {
public:
	BinaryOp(int binop, Expression* left, Expression* right)
	: _BinOp(binop), _leftOperand(left), _rightOperand(right) {}
	virtual ~BinaryOp() override = default;
private:
	int _BinOp;
	Expression* _leftOperand;
	Expression* _rightOperand;
};

class UnAss : public Expression {
public:
	UnAss(int unass, Expression* operand)
	: _Unass(unass), _operand(operand) {}
	virtual ~UnAss() = default;
protected:
	int _Unass;
	Expression* _operand;
};

class PrefixUnAss : public UnAss {
public:	
	PrefixUnAss(int unass, Expression* operand) 
	: UnAss(unass, operand) {}
	virtual ~PrefixUnAss() override = default;

};
// kalo einai na fygei to CP kanei kako || factoring
class PostfixUnAss : public UnAss {
public:
	PostfixUnAss(int unass, Expression* operand) 
	: UnAss(unass, operand) {}
	virtual ~PostfixUnAss() override = default;

};


class BinaryAss : public Expression {
public:
	BinaryAss(int type, Expression* left, Expression* right)
	: _leftOperand(left), _BinAss(type), _rightOperand(right) {}
	virtual ~BinaryAss() override = default;
private:
	int _BinAss;
	Expression* _leftOperand;
	Expression* _rightOperand;
};

class TypeCast : public Expression {
public:
	TypeCast(TypeExpression* type, Expression* expr)
	: _type(type), _expr(expr) {}
	virtual ~TypeCast() override = default;
private:
	TypeExpression* _type;
	Expression* _expr;
};

class TernaryOp : public Expression {
public:
	TernaryOp(Expression* cond, Expression* ifbody, Expression* elsebody)
	: _condition(cond), _ifBody(ifbody), _elseBody(elsebody) {}
	virtual ~TernaryOp() override = default;
private:
	Expression* _condition;
	Expression* _ifBody;
	Expression* _elseBody;
};

class New : public Expression {
public:
	New(TypeExpression* type) : _type(type), _size(nullptr) {}
	New(TypeExpression* type, Expression* size) : _type(type), _size(size) {}
	virtual ~New() override = default;
private:
	TypeExpression* _type;
	Expression* _size;
};

class Delete : public Expression {
public:
	Delete(Expression* expr) : _expr(expr) {} 
	virtual ~Delete() override = default;
private:
	Expression* _expr;
};

class CommaExpr : public Expression {
public:
	CommaExpr(Expression* left, Expression* right) {}
	virtual ~CommaExpr() override = default;
private:
	Expression* left, *right;
};

class Label : public AST {
public:
	Label(std::string lblname) : _lblname(lblname) {}
	virtual ~Label() = default;
private:
	std::string _lblname;
};





class StatementList : public Statement {
public:
	StatementList() : _stmts(std::deque<Statement*>()) {}
	StatementList(std::deque<Statement*> stmts) 
	: _stmts(stmts) {}
	virtual ~StatementList() override = default;
	std::deque<Statement*> _stmts;
};

class ParameterList : public AST {
public:
	virtual ~ParameterList() override = default;
	std::vector<Parameter*> _parameters;
};

class ExpressionList : public AST {
public:

	ExpressionList() : _expressions(std::deque<Expression*>()) {}
	ExpressionList(std::deque<Expression*> expressions) : _expressions(expressions) {}
	virtual ~ExpressionList() override = default;
	std::deque<Expression*> _expressions;
};

class DeclarationList : public Declaration {
public:
	DeclarationList() : _decls(std::deque<Declaration*>()) {}
	DeclarationList(std::deque<Declaration*> decls) : _decls(decls) {}
	virtual ~DeclarationList() override = default;
	std::deque<Declaration*> _decls;
};






#endif/*__AST_HPP__*/