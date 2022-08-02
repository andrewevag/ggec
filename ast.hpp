#ifndef __AST_HPP__
#define __AST_HPP__
#include <vector>
#include <string>
#include <deque>
#include "tree.hpp"

class AST : public Tree {
public:
	virtual ~AST() = default;


	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;
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
class BasicType;


class Program : public AST {
public:
	Program(DeclarationList* decls) : _decls(decls) {}
	virtual ~Program() override { delete _decls; } /* edw to decls ti to kanw??*/
	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_decls}; }
	virtual void printNode(std::ostream& out) override { out << "Program"; } 
private:
	DeclarationList* _decls;
};

class Declaration : public AST {
public:
	virtual ~Declaration() override = default;
	//only interesting in variables || when parsed in line we later add the type of the first declared
	virtual void embedType(TypeExpression*) = 0;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;
};


class VariableDeclaration : public Declaration {
public:
	VariableDeclaration(TypeExpression* typeExpr, std::string name) : _typeExpr(typeExpr), _name(name) {} 
	~VariableDeclaration() { delete _typeExpr; }
	virtual void embedType(TypeExpression* type) override {
		this->_typeExpr = type;
	}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*) _typeExpr }; } 
	virtual void printNode(std::ostream& out) override { out << "VariableDeclaration(" << _name << ')';}
protected:
	TypeExpression* _typeExpr;
	std::string _name;
};

class ArrayDeclaration : public VariableDeclaration {
public:
	ArrayDeclaration(TypeExpression* typeExp, std::string name, Expression* expr)
	:  VariableDeclaration(typeExp, name), _expr(expr) {}
	~ArrayDeclaration() { delete this->_typeExpr; delete _expr; }

	/* Printing Syntax Tree Functiontgs */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)this->_typeExpr, (Tree*)this->_expr}; }
	virtual void printNode(std::ostream& out) override { out << "ArrayDeclaration(" << _name << ')';  }
private: 
	Expression* _expr;
};

class FunctionDeclaration : public Declaration {
public:
	FunctionDeclaration(TypeExpression* typeExpr, std::string name, ParameterList* parameters) : 
	_resultType(typeExpr), _name(name), _parameters(parameters) {}
	~FunctionDeclaration() override { delete _resultType; delete _parameters; }
	virtual void embedType(TypeExpression* type) override {}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)this->_resultType, (Tree*)this->_parameters}; };
	virtual void printNode(std::ostream& out) override { out << "FunctionDeclaration(" << _name << ")"; };
private:
	TypeExpression* _resultType;
	std::string _name;
	ParameterList* _parameters;

};

class FunctionDefinition : public Declaration {
public:
	FunctionDefinition(TypeExpression* typeExpr, std::string name, ParameterList* parameters, 
	DeclarationList* decls, StatementList* statements) : 
	_resultType(typeExpr), _name(name), _parameters(parameters), _decls(decls), _statements(statements) {}
	~FunctionDefinition() override 
	{ delete _resultType; delete _parameters; delete _decls; delete _statements;}
	virtual void embedType(TypeExpression* type) override {}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)this->_resultType, (Tree*)this->_parameters, (Tree*)this->_decls, (Tree*)this->_statements}; }
	virtual void printNode(std::ostream& out) override { out << "FunctionDefinition(" << _name << ")"; }
private:
	TypeExpression* _resultType;
	std::string _name;
	ParameterList* _parameters;
	DeclarationList* _decls;
	StatementList* _statements;
	
};


class Parameter : public AST {
public:
	enum PassingWay { ByCall, ByRef };
	static std::string passingWayToString(PassingWay pw) { return pw == PassingWay::ByCall ? "ByCall" : "ByRef"; }
	Parameter(PassingWay pw, TypeExpression* type, std::string name) 
	: _pw(pw), _name(name), _type(type) {}
	~Parameter() override { delete _type; }
	
	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return { (Tree*)this->_type }; };
	virtual void printNode(std::ostream& out) override { 
		out << "Parameter(" << passingWayToString(_pw) <<  ", " << _name << ")"; 
	}
private:
	PassingWay _pw;
	std::string _name;
	TypeExpression* _type;
};

class TypeExpression : public AST {
public:
	virtual ~TypeExpression() override = default;
	virtual void penetrate(TypeExpression*) = 0;
	virtual std::string getName() = 0;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;
private:
};

class BasicType : public TypeExpression {
public:
	BasicType(std::string name) : _name(name) {}
	~BasicType() override = default;
	std::string getName() override { return this->_name; }
	virtual void penetrate(TypeExpression* me) override { this->_name = me->getName();	}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override {return {};};
	virtual void printNode(std::ostream& out) override { out << "BasicType(" << _name << ")"; };
private:
	std::string _name;
};

class Pointer : public TypeExpression {
public:
	Pointer(TypeExpression* inner) : _inner(inner) {}
	~Pointer() override { delete _inner; }
	virtual std::string getName() override { return "*"; }
	virtual void penetrate(TypeExpression* me) override {
		this->_inner->penetrate(me);
	}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_inner}; };
	virtual void printNode(std::ostream& out) override { out << "Pointer";}
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
	~EmptyStatement() override = default;
	
	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override { out << "EmptyStatement"; };

};

class SingleExpression : public Statement {
public: 
	SingleExpression(Expression* expr)
	: _expr(expr) {}
	~SingleExpression() override { delete _expr; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_expr}; }
	virtual void printNode(std::ostream& out) override { out << "SingleExpression"; };
private:
	Expression* _expr;
};

class IfStatement : public Statement {
public:
	IfStatement(Expression* cond, Statement* ifbody)
	: _condition(cond), _ifbody(ifbody) {}
	~IfStatement() override { delete _condition; delete _ifbody;}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_condition, (Tree*)_ifbody}; }
	virtual void printNode(std::ostream& out) override { out << "IfStatement"; }
private:
	Expression* _condition;
	Statement* _ifbody;
};

class IfElseStatement : public Statement {
public:
	IfElseStatement(Expression* cond, Statement* ifbody, Statement* elsebody)
	: _condition(cond), _ifbody(ifbody), _elsebody(elsebody) {}
	~IfElseStatement() override { delete _condition; delete _ifbody; delete _elsebody;}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_condition, (Tree*) _ifbody, (Tree*)_elsebody}; };
	virtual void printNode(std::ostream& out) override { out << "IfElseStatement"; }
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
	~ForStatement() override 
	{ delete _label; delete _first; delete _second; delete _third; delete _body; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { 
		if (_label == nullptr) return {(Tree*)_first, (Tree*)_second, (Tree*)_third, (Tree*)_body};
		else return {(Tree*)_label, (Tree*)_first, (Tree*)_second, (Tree*)_third, (Tree*)_body};
	};
	virtual void printNode(std::ostream& out) override { out << "ForStatement"; }
private:
	Label* _label;
	Expression* _first, *_second, *_third;
	Statement* _body;
};

class ContinueStatement : public Statement {
public:
	// can be "" indicating no target label
	ContinueStatement() : _target("") {}
	ContinueStatement(std::string target) : _target(target) {}
	~ContinueStatement() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override { 
		if (_target == "") out << "ContinueStatement"; 
		else out << "ContinueStatement(" << _target << ")";  
	};
private:
	//empty string indicating no label
	std::string _target;
};

class BreakStatement : public Statement {
public:
	// can be "" indicating no target label
	BreakStatement() : _target("") {}
	BreakStatement(std::string target) : _target(target) {}
	~BreakStatement() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override { 
		if (_target == "") out << "BreakStatement"; 
		else out << "BreakStatement(" << _target << ")";  
	} ;
private:
	//empty string indicating no label
	std::string _target;
};

class ReturnStatement : public Statement {
public:
	//the expression can be null indicating no return expression
	ReturnStatement(Expression* expr) : _expr(expr) {}
	~ReturnStatement() override { delete _expr; }
	
	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override {
		if (_expr == nullptr) return {};
		else return {(Tree*)_expr}; 
	}
	virtual void printNode(std::ostream& out) override { out << "ReturnStatement"; };
private:
	//nullptr indicating no expression in the return statement
	Expression* _expr;
};


class Expression : public AST {
public:
	virtual ~Expression() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;
};

class Id : public Expression {
public:
	Id(std::string name) : _name(name) {}
	~Id() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override { out << "Id(" << _name << ")"; };
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
	~Constant() override = default;
	enum ConstantType { Bool, Null, Int, Char, Double, String };

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override {
		switch (_ct){
		case Bool: out << "BoolConstant(" << (_bool ? "true" : "false") << ")"; break;
		case Null: out << "NULL"; break;
		case Int: out << "IntConstant(" << _int << ")"; break;
		case Char : out << "CharConstant(" << (uint8_t) _char << ")"; break;
		case Double : out << "DoubleConstant(" << _double << ")"; break;
		case String : out << "StringConstant(" << "\"" << _string << "\")"; break;
		default: break;
		}
	};
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
	~FunctionCall() override { delete _arguments; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_arguments}; };
	virtual void printNode(std::ostream& out) override { out << "FunctionCall(" << _functionName << ")"; }
private:
	std::string _functionName;
	ExpressionList* _arguments;
};

class BracketedIndex : public Expression {
public:
	BracketedIndex(Expression* in, Expression* out)
	: _in(in), _out(out) {} 
	~BracketedIndex() override { delete _in; delete _out; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_in, _out}; };
	virtual void printNode(std::ostream& out) override { out << "BracketedIndex"; }
private:
	Expression* _in;
	Expression* _out;
};

class UnaryOp : public Expression {
public:
	UnaryOp(int unop, Expression* operand)
	: _UnOp(unop), _operand(operand) {}
	~UnaryOp() override { delete _operand; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_operand}; }
	virtual void printNode(std::ostream& out) override { out << "UnaryOp(" << _UnOp << ")"; }
private:
	int _UnOp;
	Expression* _operand;
};

class BinaryOp : public Expression {
public:
	BinaryOp(int binop, Expression* left, Expression* right)
	: _BinOp(binop), _leftOperand(left), _rightOperand(right) {}
	~BinaryOp() override { delete _leftOperand; delete _rightOperand; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_leftOperand, _rightOperand}; }
	virtual void printNode(std::ostream& out) override { out << "BinaryOp(" << _BinOp << ")"; }
private:
	int _BinOp;
	Expression* _leftOperand;
	Expression* _rightOperand;
};

class UnAss : public Expression {
public:
	UnAss(int unass, Expression* operand)
	: _Unass(unass), _operand(operand) {}
	~UnAss() override { delete _operand; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;
protected:
	int _Unass;
	Expression* _operand;
};

class PrefixUnAss : public UnAss {
public:	
	PrefixUnAss(int unass, Expression* operand) 
	: UnAss(unass, operand) {}
	~PrefixUnAss() override { delete this->_operand; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {this->_operand}; }
	virtual void printNode(std::ostream& out) override { out << "PrefixUnAss(" << this->_Unass << ")"; }

};
// kalo einai na fygei to CP kanei kako || factoring
class PostfixUnAss : public UnAss {
public:
	PostfixUnAss(int unass, Expression* operand) 
	: UnAss(unass, operand) {}
	~PostfixUnAss() override { delete this->_operand; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {this->_operand}; }
	virtual void printNode(std::ostream& out) override { out << "PostfixUnAss(" << this->_Unass << ")"; }
};


class BinaryAss : public Expression {
public:
	BinaryAss(int type, Expression* left, Expression* right)
	: _BinAss(type), _leftOperand(left), _rightOperand(right) {}
	~BinaryAss() override { delete _rightOperand; delete _leftOperand; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_leftOperand, _rightOperand}; }
	virtual void printNode(std::ostream& out) override { out << "BinaryAss(" << this->_BinAss << ")"; }
private:
	int _BinAss;
	Expression* _leftOperand;
	Expression* _rightOperand;
};

class TypeCast : public Expression {
public:
	TypeCast(TypeExpression* type, Expression* expr)
	: _type(type), _expr(expr) {}
	~TypeCast() override { delete _expr; delete _type; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return { _type, _expr }; }
	virtual void printNode(std::ostream& out) override { out << "TypeCast"; }
private:
	TypeExpression* _type;
	Expression* _expr;
};

class TernaryOp : public Expression {
public:
	TernaryOp(Expression* cond, Expression* ifbody, Expression* elsebody)
	: _condition(cond), _ifBody(ifbody), _elseBody(elsebody) {}
	~TernaryOp() override { delete _condition; delete _ifBody; delete _elseBody; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_condition, _ifBody, _elseBody}; }
	virtual void printNode(std::ostream& out) override { out << "TernaryOp"; }
private:
	Expression* _condition;
	Expression* _ifBody;
	Expression* _elseBody;
};

class New : public Expression {
public:
	New(TypeExpression* type) : _type(type), _size(nullptr) {}
	New(TypeExpression* type, Expression* size) : _type(type), _size(size) {}
	~New() override { delete _size; delete _type; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_type, _size}; }
	virtual void printNode(std::ostream& out) override { out << "New"; }
private:
	TypeExpression* _type;
	Expression* _size;
};

class Delete : public Expression {
public:
	Delete(Expression* expr) : _expr(expr) {} 
	~Delete() override { delete _expr; }

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_expr}; }
	virtual void printNode(std::ostream& out) override { out << "Delete"; }
private:
	Expression* _expr;
};

class CommaExpr : public Expression {
public:
	CommaExpr(Expression* left, Expression* right) {}
	~CommaExpr() override { delete _left; delete _right;}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_left, _right}; }
	virtual void printNode(std::ostream& out) override { out << "CommaExpr"; }
private:
	Expression* _left, *_right;
};

class Label : public AST {
public:
	Label(std::string lblname) : _lblname(lblname) {}
	virtual ~Label() = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; }
	virtual void printNode(std::ostream& out) override { out << "Label(" << _lblname << ")"; }
private:
	std::string _lblname;
};





class StatementList : public Statement {
public:
	StatementList() : _stmts(std::deque<Statement*>()) {}
	StatementList(std::deque<Statement*> stmts) 
	: _stmts(stmts) {}
	~StatementList() override { for(auto& i : this->_stmts) delete i; }
	std::deque<Statement*> _stmts;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_stmts.begin(), _stmts.end()}; }
	virtual void printNode(std::ostream& out) override { out << "StatementList"; }
};

class ParameterList : public AST {
public:
	ParameterList() : _parameters(std::deque<Parameter*>()) {}
	virtual ~ParameterList() override { for(auto& i : this->_parameters) delete i; }	/*ki edw ti kanw to deque*/
	std::deque<Parameter*> _parameters;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_parameters.begin(), _parameters.end()}; }
	virtual void printNode(std::ostream& out) override { out << "ParameterList"; }
};

class ExpressionList : public AST {
public:

	ExpressionList() : _expressions(std::deque<Expression*>()) {}
	ExpressionList(std::deque<Expression*> expressions) : _expressions(expressions) {}
	virtual ~ExpressionList() override { for(auto& i : this->_expressions) delete i; }
	std::deque<Expression*> _expressions;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_expressions.begin(), _expressions.end()}; }
	virtual void printNode(std::ostream& out) override { out << "ExpressionList"; }
};

class DeclarationList : public Declaration {
public:
	DeclarationList() : _decls(std::deque<Declaration*>()) {}
	DeclarationList(std::deque<Declaration*> decls) : _decls(decls) {}
	~DeclarationList() override { for(auto& i : this->_decls) delete i; } /* edw to decls ti fasi?? Ti to kanw */
	std::deque<Declaration*> _decls;
	virtual void embedType(TypeExpression* t) override {
		for(auto& i : this->_decls)
			i->embedType(t);
	}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_decls.begin(), _decls.end()}; }
	virtual void printNode(std::ostream& out) override { out << "DeclarationList"; }
};






#endif/*__AST_HPP__*/