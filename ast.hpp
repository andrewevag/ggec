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



#endif/*__AST_HPP__*/