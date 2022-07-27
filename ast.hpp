#ifndef __AST_HPP__
#define __AST_HPP__
#include <vector>
#include <string>

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

class Program : public AST{
public:
	virtual ~Program() override = default;
private:
	std::vector<Declaration*> _decls;
};

class Declaration : public AST {
public:
	virtual ~Declaration() override = default;

};


class VariableDeclaration : public Declaration {
public:
	virtual ~VariableDeclaration() override = default;
private:
	TypeExpression* _typeExpr;
	std::vector<std::string> _names;
};

class FunctionDeclaration : public Declaration {
public:
	virtual ~FunctionDeclaration() override = default;
private:
	TypeExpression* _resultType;
	std::string _name;
	std::vector<Parameter*> _parameters;

};

class FunctionDefinition : public Declaration {
public:
	virtual ~FunctionDefinition() override = default;
private:
	TypeExpression* _resultType;
	std::string _name;
	std::vector<Parameter*> _parameters;
	std::vector<Statement*> _statement;
};

class TypeExpression : public AST {
public:
	virtual ~TypeExpression() override = default;
private:
};

class BasicType : public TypeExpression {
public:
	virtual ~BasicType() override = default;
private:
	std::string _name;
};

class Pointer : public TypeExpression {
public:
	virtual ~Pointer() override = default;
public:
	TypeExpression* _inner;
};

class Statement : public AST {
public:
	virtual ~Statement() override = default;
};

class EmptyStatement : public Statement {
public:
	virtual ~EmptyStatement() override = default;

};

class IfStatement : public Statement {
public:
	virtual ~IfStatement() override = default;
private:
	Expression* _condition;
	Statement* _ifbody;
};

class IfElseStatement : public Statement {
public:
	virtual ~IfElseStatement() override = default;
private:
	Expression* _condition;
	Statement* _ifbody;
	Statement* _elsebody;
};

class ForStatement : public Statement {
public:
	virtual ~ForStatement() override = default;
private:
	Label* _label;
	Expression* _first, *_second, *_third;
	Statement* _body;
};

class ContinueStatement : public Statement {
public:
	virtual ~ContinueStatement() override = default;
private:
	//empty string indicating no label
	std::string _target;
};

class BreakStatement : public Statement {
public:
	virtual ~BreakStatement() override = default;
private:
	//empty string indicating no label
	std::string _target;
};

class ReturnStatement : public Statement {
public:
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
	virtual ~Id() override = default;
private:
	std::string _name;
};

class Constant : public Expression {
public:
	virtual ~Constant() override = default;
	enum ConstantType { True, False, Null, Int, Char, Double, String };

private:
	ConstantType _ct;
	int16_t _int;
	char _char;
	long double _double;
	std::string _string;
};

class FunctionCall : public Expression {
public:
	virtual ~FunctionCall() override = default;
private:
	std::string _functionName;
	std::vector<Expression*> _arguments;
};

class BracketedIndex : public Expression {
public:
	virtual ~BracketedIndex() override = default;
private:
	Expression* _in;
	Expression* _out;
};

class UnaryOp : public Expression {
public:
	virtual ~UnaryOp() override = default;
private:
	int _UnOp;
	Expression* _operand;
};

class BinaryOp : public Expression {
public:
	virtual ~BinaryOp() override = default;
private:
	int _BinOp;
	Expression* _leftOperand;
	Expression* _rightOperand;
};

class PrefixUnAss : public Expression {
public:
	virtual ~PrefixUnAss() override = default;
private:
	int _UnAss;
	Expression* _operand;
};
// kalo einai na fygei to CP kanei kako || factoring
class PostfixUnAss : public Expression {
public:
	virtual ~PostfixUnAss() override = default;
private:
	int _UnAss;
	Expression* _operand;
};


class BinaryAss : public Expression {
public:
	virtual ~BinaryAss() override = default;
private:
	int _BinAss;
	Expression* _leftOperand;
	Expression* _rightOperand;
};

class TypeCast : public Expression {
public:
	virtual ~TypeCast() override = default;
private:
	TypeExpression* _type;
	Expression* _expr;
};

class TernaryOp : public Expression {
public:
	virtual ~TernaryOp() override = default;
private:
	Expression* _condition;
	Expression* _ifBody;
	Expression* _elseBody;
};

class New : public Expression {
public:
	virtual ~New() override = default;
private:
	TypeExpression* _type;
	Expression* _size;
};

class Delete : public Expression {
public:
	virtual ~Delete() override = default;
private:
	Expression* _expr;
};







class StatementList : public AST {
public:
	virtual ~StatementList() override = default;
private:
	std::vector<Statement*> _stmts;
};

class ParameterList : public AST {
public:
	virtual ~ParameterList() override = default;
private:
	std::vector<Parameter*> _parameters;
};

class ExpressionList : public AST {
public:
	virtual ~ExpressionList() override = default;
private:
	std::vector<Expression*> _expressions;
};

class DeclarationList : public AST {
public:
	virtual ~DeclarationList() override = default;
private:
	std::vector<Declaration*> _decls;
};






#endif/*__AST_HPP__*/