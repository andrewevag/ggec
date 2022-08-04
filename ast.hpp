#ifndef __AST_HPP__
#define __AST_HPP__
#include <vector>
#include <string>
#include <deque>
#include "tree.hpp"

class AST : public Tree {
public:
	virtual ~AST() = default;
	virtual std::string toJSONString() = 0;

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
	virtual ~Program() override {};


	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_decls}; }
	virtual void printNode(std::ostream& out) override { out << "Program"; } 
	virtual std::string toJSONString() override;
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
	virtual std::string toJSONString() = 0;
};


class VariableDeclaration : public Declaration {
public:
	VariableDeclaration(TypeExpression* typeExpr, std::string name) : _typeExpr(typeExpr), _name(name) {} 
	virtual ~VariableDeclaration() override = default;
	virtual void embedType(TypeExpression* type) override {
		this->_typeExpr = type;
	}


	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*) _typeExpr }; } 
	virtual void printNode(std::ostream& out) override { out << "VariableDeclaration(" << _name << ')';}
	virtual std::string toJSONString() override;
protected:
	TypeExpression* _typeExpr;
	std::string _name;
};

class ArrayDeclaration : public VariableDeclaration {
public:
	ArrayDeclaration(TypeExpression* typeExp, std::string name, Expression* expr)
	:  VariableDeclaration(typeExp, name), _expr(expr) {}
	virtual ~ArrayDeclaration() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)this->_typeExpr, (Tree*)this->_expr}; }
	virtual void printNode(std::ostream& out) override { out << "ArrayDeclaration(" << _name << ')';  }
	virtual std::string toJSONString() override;
private: 
	Expression* _expr;
};

class FunctionDeclaration : public Declaration {
public:
	FunctionDeclaration(TypeExpression* typeExpr, std::string name, ParameterList* parameters) : 
	_resultType(typeExpr), _name(name), _parameters(parameters) {}
	virtual ~FunctionDeclaration() override = default;
	virtual void embedType(TypeExpression* type) override {}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)this->_resultType, (Tree*)this->_parameters}; };
	virtual void printNode(std::ostream& out) override { out << "FunctionDeclaration(" << _name << ")"; };
	virtual std::string toJSONString() override;
private:
	TypeExpression* _resultType;
	std::string _name;
	/* this will be an empty list and not null ptr in case of no parameters */
	ParameterList* _parameters;

};

class FunctionDefinition : public Declaration {
public:
	FunctionDefinition(TypeExpression* typeExpr, std::string name, ParameterList* parameters, 
	DeclarationList* decls, StatementList* statements) : 
	_resultType(typeExpr), _name(name), _parameters(parameters), _decls(decls), _statements(statements) {}
	virtual ~FunctionDefinition() override = default;
	virtual void embedType(TypeExpression* type) override {}


	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)this->_resultType, (Tree*)this->_parameters, (Tree*)this->_decls, (Tree*)this->_statements}; }
	virtual void printNode(std::ostream& out) override { out << "FunctionDefinition(" << _name << ")"; }
	virtual std::string toJSONString() override;
private:
	TypeExpression* _resultType;
	std::string _name;
	/* this will be an empty list and not null ptr in case of no parameters */
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
	virtual ~Parameter() override = default;
	

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return { (Tree*)this->_type }; };
	virtual void printNode(std::ostream& out) override { 
		out << "Parameter(" << passingWayToString(_pw) <<  ", " << _name << ")"; 
	}
	virtual std::string toJSONString() override;
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
	virtual TypeExpression* copy() = 0;
	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;
	virtual std::string toJSONString() = 0;
private:
};

class BasicType : public TypeExpression {
public:
	BasicType(std::string name) : _name(name) {}
	virtual ~BasicType() override = default;
	virtual std::string getName() override { return this->_name; }
	virtual void penetrate(TypeExpression* me) override { 
		this->_name = me->getName();
	}
	virtual TypeExpression* copy() override {
		return new BasicType(_name);
	}


	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override {return {};};
	virtual void printNode(std::ostream& out) override { out << "BasicType(" << _name << ")"; };
	virtual std::string toJSONString() override;
private:
	std::string _name;
};

class Pointer : public TypeExpression {
public:
	Pointer(TypeExpression* inner) : _inner(inner) {}
	virtual ~Pointer() override = default;
	virtual std::string getName() override { return "*"; }
	virtual void penetrate(TypeExpression* me) override {
		this->_inner->penetrate(me);
	}
	virtual TypeExpression* copy() override {
		return new Pointer(this->_inner->copy());
	}


	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_inner}; };
	virtual void printNode(std::ostream& out) override { out << "Pointer";}
	virtual std::string toJSONString() override;
public:
	TypeExpression* _inner;
};

class Statement : public AST {
public:
	virtual ~Statement() override = default;
	virtual std::string toJSONString() = 0;
};

class EmptyStatement : public Statement {
public:
	EmptyStatement() {}
	virtual ~EmptyStatement() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override { out << "EmptyStatement"; };
	virtual std::string toJSONString() override;

};

class SingleExpression : public Statement {
public: 
	SingleExpression(Expression* expr)
	: _expr(expr) {}
	virtual ~SingleExpression() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_expr}; }
	virtual void printNode(std::ostream& out) override { out << "SingleExpression"; };
	virtual std::string toJSONString() override;
private:
	Expression* _expr;
};

class IfStatement : public Statement {
public:
	IfStatement(Expression* cond, Statement* ifbody)
	: _condition(cond), _ifbody(ifbody) {}
	virtual ~IfStatement() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_condition, (Tree*)_ifbody}; }
	virtual void printNode(std::ostream& out) override { out << "IfStatement"; }
	virtual std::string toJSONString() override;
private:
	Expression* _condition;
	Statement* _ifbody;
};

class IfElseStatement : public Statement {
public:
	IfElseStatement(Expression* cond, Statement* ifbody, Statement* elsebody)
	: _condition(cond), _ifbody(ifbody), _elsebody(elsebody) {}
	virtual ~IfElseStatement() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_condition, (Tree*) _ifbody, (Tree*)_elsebody}; };
	virtual void printNode(std::ostream& out) override { out << "IfElseStatement"; }
	virtual std::string toJSONString() override;
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

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { 
		if (_label == nullptr) return {(Tree*)_first, (Tree*)_second, (Tree*)_third, (Tree*)_body};
		else return {(Tree*)_label, (Tree*)_first, (Tree*)_second, (Tree*)_third, (Tree*)_body};
	};
	virtual void printNode(std::ostream& out) override { out << "ForStatement"; }
	virtual std::string toJSONString() override;
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
	virtual ~ContinueStatement() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override { 
		if (_target == "") out << "ContinueStatement"; 
		else out << "ContinueStatement(" << _target << ")";  
	};
	virtual std::string toJSONString() override;
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

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override { 
		if (_target == "") out << "BreakStatement"; 
		else out << "BreakStatement(" << _target << ")";  
	} ;
	virtual std::string toJSONString() override;
private:
	//empty string indicating no label
	std::string _target;
};

class ReturnStatement : public Statement {
public:
	//the expression can be null indicating no return expression
	ReturnStatement(Expression* expr) : _expr(expr) {}
	virtual ~ReturnStatement() override = default;
	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override {
		if (_expr == nullptr) return {};
		else return {(Tree*)_expr}; 
	}
	virtual void printNode(std::ostream& out) override { out << "ReturnStatement"; };
	virtual std::string toJSONString() override;
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
	virtual std::string toJSONString() = 0;
};

class Id : public Expression {
public:
	Id(std::string name) : _name(name) {}
	virtual ~Id() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override { out << "Id(" << _name << ")"; };
	virtual std::string toJSONString() override;
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

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override {
		switch (_ct){
		case Bool: out << "BoolConstant(" << (_bool ? "true" : "false") << ")"; break;
		case Null: out << "NULL"; break;
		case Int: out << "IntConstant(" << _int << ")"; break;
		case Char : out << "CharConstant(" << (uint) _char << ")"; break;
		case Double : out << "DoubleConstant(" << _double << ")"; break;
		case String : out << "StringConstant(" << "\"" << _string << "\")"; break;
		default: break;
		}
	};
	virtual std::string toJSONString() override;
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

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_arguments}; };
	virtual void printNode(std::ostream& out) override { out << "FunctionCall(" << _functionName << ")"; }
	virtual std::string toJSONString() override;
private:
	std::string _functionName;
	ExpressionList* _arguments;
};

class BracketedIndex : public Expression {
public:
	BracketedIndex(Expression* in, Expression* out)
	: _in(in), _out(out) {} 
	virtual ~BracketedIndex() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_in, _out}; };
	virtual void printNode(std::ostream& out) override { out << "BracketedIndex"; }
	virtual std::string toJSONString() override;
private:
	Expression* _in;
	Expression* _out;
};

class UnaryOp : public Expression {
public:
	/* For Readbility And Being Easy to Print */
	enum UnaryOpType { ADDRESS, DEREF, POS, NEG, NOT };
	static std::string unaryOpTypeToString(UnaryOpType t){
		switch (t){
			case ADDRESS: return "ADDRESS";
			case DEREF:   return "DEREF";
			case POS: 	  return "POS";
			case NEG: 	  return "NEG";
			case NOT: 	  return "NOT";
		}
		std::cerr << "No such UnaryOpType" << std::endl;
		return "";
	}

	UnaryOp(UnaryOpType unop, Expression* operand)
	: _UnOp(unop), _operand(operand) {}
	virtual ~UnaryOp() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_operand}; }
	virtual void printNode(std::ostream& out) override { out << "UnaryOp(" << unaryOpTypeToString(_UnOp) << ")"; }
	virtual std::string toJSONString() override;
private:
	UnaryOpType _UnOp;
	Expression* _operand;
};

class BinaryOp : public Expression {
public:
	enum BinaryOpType { MULT, DIV, MOD, PLUS, MINUS, LESS, GREATER, LESSEQ, GREATEREQ, EQUALS, NOTEQ, LAND, LOR, COMMA };
	static std::string binaryOpTypeToString(BinaryOpType t){
		switch (t){
		case MULT: return "MULT";
		case DIV: return "DIV";
		case MOD: return "MOD";
		case PLUS: return "PLUS";
		case MINUS: return "MINUS";
		case LESS: return "LESS";
		case GREATER: return "GREATER";
		case LESSEQ: return "LESSEQ";
		case GREATEREQ: return "GREATEREQ";
		case EQUALS: return "EQUALS";
		case NOTEQ: return "NOTEQ";
		case LAND: return "LAND";
		case LOR: return "LOR";
		case COMMA: return "COMMA";
		}
		std::cerr << "No such BinaryOpType" << std::endl;
		return "";
	}

	BinaryOp(BinaryOpType binop, Expression* left, Expression* right)
	: _BinOp(binop), _leftOperand(left), _rightOperand(right) {}
	virtual ~BinaryOp() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_leftOperand, _rightOperand}; }
	virtual void printNode(std::ostream& out) override { out << "BinaryOp(" << binaryOpTypeToString(_BinOp) << ")"; }
	virtual std::string toJSONString() override;
private:
	BinaryOpType _BinOp;
	Expression* _leftOperand;
	Expression* _rightOperand;
};

class UnAss : public Expression {
public:
	enum UnAssType { PLUSPLUS, MINUSMINUS };
	static std::string UnAssTypeToString(UnAssType t){
		switch (t) {
			case PLUSPLUS: return "PLUSPLUS";
			case MINUSMINUS: return "MINUSMINUS";
		}
		std::cerr << "No such UnAssType" << std::endl;
		return "";
	}
	UnAss(UnAssType unass, Expression* operand)
	: _Unass(unass), _operand(operand) {}
	virtual ~UnAss() = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;
	virtual std::string toJSONString() = 0;
protected:
	UnAssType _Unass;
	Expression* _operand;
};

class PrefixUnAss : public UnAss {
public:	
	PrefixUnAss(UnAss::UnAssType unass, Expression* operand) 
	: UnAss(unass, operand) {}
	virtual ~PrefixUnAss() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {this->_operand}; }
	virtual void printNode(std::ostream& out) override { out << "PrefixUnAss(" << UnAss::UnAssTypeToString(this->_Unass) << ")"; }
	virtual std::string toJSONString() override;

};
// kalo einai na fygei to CP kanei kako || factoring
class PostfixUnAss : public UnAss {
public:
	PostfixUnAss(UnAss::UnAssType unass, Expression* operand) 
	: UnAss(unass, operand) {}
	virtual ~PostfixUnAss() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {this->_operand}; }
	virtual void printNode(std::ostream& out) override { out << "PostfixUnAss(" << UnAss::UnAssTypeToString(this->_Unass) << ")"; }
	virtual std::string toJSONString() override;
};


class BinaryAss : public Expression {
public:
	enum BinaryAssType { ASS, MULTASS, DIVASS, MODASS, PLUSASS, MINUSASS };
	static std::string binaryAssTypeToString(BinaryAssType t){
		switch (t){
			case ASS: return "ASS";
			case MULTASS: return "MULTASS";
			case DIVASS: return "DIVASS";
			case MODASS: return "MODASS";
			case PLUSASS: return "PLUSASS";
			case MINUSASS: return "MINUSASS";
		}
		std::cerr << "No such BinaryAssType" << std::endl;
		return "";
	}
	BinaryAss(BinaryAssType type, Expression* left, Expression* right)
	: _BinAss(type), _leftOperand(left), _rightOperand(right) {}
	virtual ~BinaryAss() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_leftOperand, _rightOperand}; }
	virtual void printNode(std::ostream& out) override { out << "BinaryAss(" << binaryAssTypeToString(this->_BinAss) << ")"; }
	virtual std::string toJSONString() override;
private:
	BinaryAssType _BinAss;
	Expression* _leftOperand;
	Expression* _rightOperand;
};

class TypeCast : public Expression {
public:
	TypeCast(TypeExpression* type, Expression* expr)
	: _type(type), _expr(expr) {}
	virtual ~TypeCast() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return { _type, _expr }; }
	virtual void printNode(std::ostream& out) override { out << "TypeCast"; }
	virtual std::string toJSONString() override;
private:
	TypeExpression* _type;
	Expression* _expr;
};

class TernaryOp : public Expression {
public:
	TernaryOp(Expression* cond, Expression* ifbody, Expression* elsebody)
	: _condition(cond), _ifBody(ifbody), _elseBody(elsebody) {}
	virtual ~TernaryOp() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_condition, _ifBody, _elseBody}; }
	virtual void printNode(std::ostream& out) override { out << "TernaryOp"; }
	virtual std::string toJSONString() override;
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

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { if (this->_size != nullptr) return {_type, _size}; else return {_type}; }
	virtual void printNode(std::ostream& out) override { out << "New"; }
	virtual std::string toJSONString() override;

private:
	TypeExpression* _type;
	Expression* _size;
};

class Delete : public Expression {
public:
	Delete(Expression* expr) : _expr(expr) {} 
	virtual ~Delete() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_expr}; }
	virtual void printNode(std::ostream& out) override { out << "Delete"; }
	virtual std::string toJSONString() override;

private:
	Expression* _expr;
};

class CommaExpr : public Expression {
public:
	CommaExpr(Expression* left, Expression* right) {}
	virtual ~CommaExpr() override = default;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_left, _right}; }
	virtual void printNode(std::ostream& out) override { out << "CommaExpr"; }
	virtual std::string toJSONString() override;

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
	virtual std::string toJSONString() override;

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

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_stmts.begin(), _stmts.end()}; }
	virtual void printNode(std::ostream& out) override { out << "StatementList"; }
	virtual std::string toJSONString() override;

};

class ParameterList : public AST {
public:
	ParameterList() : _parameters(std::deque<Parameter*>()) {}
	virtual ~ParameterList() override = default;
	std::deque<Parameter*> _parameters;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_parameters.begin(), _parameters.end()}; }
	virtual void printNode(std::ostream& out) override { out << "ParameterList"; }
	virtual std::string toJSONString() override;
};

class ExpressionList : public AST {
public:

	ExpressionList() : _expressions(std::deque<Expression*>()) {}
	ExpressionList(std::deque<Expression*> expressions) : _expressions(expressions) {}
	virtual ~ExpressionList() override = default;
	std::deque<Expression*> _expressions;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_expressions.begin(), _expressions.end()}; }
	virtual void printNode(std::ostream& out) override { out << "ExpressionList"; }
	virtual std::string toJSONString() override;

};

class DeclarationList : public Declaration {
public:
	DeclarationList() : _decls(std::deque<Declaration*>()) {}
	DeclarationList(std::deque<Declaration*> decls) : _decls(decls) {}
	virtual ~DeclarationList() override = default;
	std::deque<Declaration*> _decls;
	virtual void embedType(TypeExpression* t) override {
		for(Declaration* i : this->_decls)
			i->embedType(t);
	}
	virtual std::string toJSONString() override;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_decls.begin(), _decls.end()}; }
	virtual void printNode(std::ostream& out) override { out << "DeclarationList"; }
};






#endif/*__AST_HPP__*/