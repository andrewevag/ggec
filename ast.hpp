#ifndef __AST_HPP__
#define __AST_HPP__
#include <vector>
#include <string>
#include <deque>
#include "tree.hpp"
#include "symbol.hpp"
#include "llvmhead.hpp"
#include "error.hpp"


//====================================================================================//
// Global Variables For Handling Nested Labels                                        //
//====================================================================================//
class Label;
extern std::vector<Label*> NestingNamedloops;


//====================================================================================//
// Nodes for AST 																	  //
//====================================================================================//

class AST : public Tree, public ErrorInfo {
public:
	virtual ~AST();
	/**
	 * @brief Print Syntax Tree in json format 
	 */
	virtual std::string toJSONString() = 0;
	
	/**
	 * @brief Function that does the semantic analysis of the syntax tree
	 */
	virtual void sem() = 0;

	/**
	 * @brief Function That performs the code generation for the syntax tree.
	 * 
	 * @return llvm::Value* NULL if not needed to calculate something else the value 
	 * where it is calculated.
	 */
	virtual llvm::Value* codegen() = 0;

	
	/* Printing Syntax Tree Functions See README */
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;



	/* Global LLVM Variables For Code Generation */
	static llvm::LLVMContext TheContext;
	static std::unique_ptr<llvm::Module> TheModule;
	static std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;
	static std::unique_ptr<llvm::IRBuilder<> > Builder;

	
	
	
	/* More Constant LLVM Variables */
	static llvm::Type *i8;
  	static llvm::Type *i16;
	static llvm::Type *i8p;
	static llvm::Type *i64;

	/* Functions For Creating Constants */
	static llvm::ConstantInt* c8(char c) {
    	return llvm::ConstantInt::get(TheContext, llvm::APInt(8, c, true));
  	}
  	static llvm::ConstantInt* c32(int n) {
    	return llvm::ConstantInt::get(TheContext, llvm::APInt(32, n, true));
  	}
	static llvm::ConstantInt* c16(int n) {
    	return llvm::ConstantInt::get(TheContext, llvm::APInt(16, n, true));
  	}
  	static llvm::ConstantInt* c64(int n) {
    	return llvm::ConstantInt::get(TheContext, llvm::APInt(64, n, true));
  	}
  	static llvm::Function* newF;
	static llvm::Function* deleteF;
	static llvm::Value* getEnvAt(unsigned int nestinglevel);
};
class Declaration;
class TypeExpression;
class Parameter;
class Statement;
class Expression;
class StatementList;
class ParameterList;
class ExpressionList;
class DeclarationList;
class BasicType;


//====================================================================================//
// Base Node For Types       														  //
//====================================================================================//

class TypeExpression : public AST {
public:
	virtual ~TypeExpression();
	virtual void penetrate(TypeExpression*) = 0;
	virtual std::string getName() = 0;
	virtual TypeExpression* copy() = 0;

	/**
	 * @brief Get the name of the type for function definitions
	 * 
	 * @return std::string
	 */
	virtual std::string getDefName() = 0;
	/**
	 * @brief Transforms the Syntax Type to Semantic Type
	 *
	 * @return Type 
	 */
	virtual Type toType() = 0;
	
	/**
	 * @brief Transforms Semantic Type to Syntax Type
	 *
	 */
	static TypeExpression* fromType(Type t);
	// virtual void sem() = 0;

	virtual llvm::Value* codegen() override;

	/* Printing Syntax Tree Functions */
	// virtual std::vector<Tree*> getChildren() = 0;
	// virtual void printNode(std::ostream& out) = 0;
	// virtual std::string toJSONString() = 0;
private:
};


//====================================================================================//
// Syntax Nodes For Paraemeters														  //
//====================================================================================//


class Parameter : public AST {
public:
	enum PassingWay { ByCall, ByRef };
	static std::string passingWayToString(PassingWay pw) { return pw == PassingWay::ByCall ? "ByCall" : "ByRef"; }
	Parameter(PassingWay pw, TypeExpression* type, std::string name) 
	: _pw(pw), _name(name), _type(type) {}
	virtual ~Parameter();

	/* Getter Functions */
	std::string getDefName() {
		return this->_type->getDefName();
	}

	TypeExpression* getType(){
		return this->_type;
	}
	
	std::string getName(){
		return this->_name;
	}
	
	PassingWay getPassingWay(){
		return this->_pw;
	}
	/* End of Getter Functions */
	
	virtual void sem() override;

	virtual llvm::Value* codegen() override;

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

class ParameterList : public AST {
public:
	ParameterList() : _parameters(std::deque<Parameter*>()) {}
	virtual ~ParameterList();
	std::deque<Parameter*> _parameters;


	std::string getAggregatedName(){
		std::string s;
		for(auto &par: this->_parameters){
			s +="_" + par->getDefName();
		}
		return s;
	};



	virtual void sem() override;

	virtual llvm::Value* codegen() override;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_parameters.begin(), _parameters.end()}; }
	virtual void printNode(std::ostream& out) override { out << "ParameterList"; }
	virtual std::string toJSONString() override;
};


//====================================================================================//
// Syntax Node For The Whole Program   											      //
//====================================================================================//

class Program : public AST {
public:
	Program(DeclarationList* decls) : _decls(decls) {}
	virtual ~Program();
	virtual void sem() override;

	virtual llvm::Value* codegen() override;
	
	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_decls}; }
	virtual void printNode(std::ostream& out) override { out << "Program"; } 
	virtual std::string toJSONString() override;
private:
	DeclarationList* _decls;
};

//====================================================================================//
// Syntax Nodes For Declarations													  //
//====================================================================================//


class Declaration : public AST {
public:
	virtual ~Declaration();
	//only interesting in variables || when parsed in line we later add the type of the first declared
	virtual void embedType(TypeExpression*) = 0;
	virtual void sem() = 0;

	/**
	 * @brief Get the Name of the declaration. It is important for
	 * functions to handle overloading since new names are generated
	 * from the parameterList.
	 * 
	 * @return std::string 
	 */
	virtual std::string getName() = 0;

	virtual llvm::Value* codegen() = 0;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;
	virtual std::string toJSONString() = 0;
};


class VariableDeclaration : public Declaration {
public:
	VariableDeclaration(TypeExpression* typeExpr, std::string name) : _typeExpr(typeExpr), _name(name) {} 
	~VariableDeclaration();
	virtual void embedType(TypeExpression* type) override {
		this->_typeExpr = type;
	}

	virtual std::string getName() override {
		return this->_name;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

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
	~ArrayDeclaration();

	virtual std::string getName() override {
		return this->_name;
	}


	virtual void sem() override;

	virtual llvm::Value* codegen() override;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)this->_typeExpr, (Tree*)this->_expr}; }
	virtual void printNode(std::ostream& out) override { out << "ArrayDeclaration(" << _name << ')';  }
	virtual std::string toJSONString() override;
private: 
	Expression* _expr;
};


class FunctionHead : public Declaration{
public:
	// The Common Part of FunctionDeclaration & FunctionDefinition
	virtual void embedType(TypeExpression* type) = 0;
	virtual void sem() = 0;
	virtual std::string getName() = 0;
	virtual llvm::Value* codegen() = 0;
	/**
	 * @brief Used in codegen to declare the function and put llvm::Function* in the 
	 * Symbol Table.
	 */
	virtual void declare();
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;
	virtual std::string toJSONString() = 0;

protected:
	TypeExpression* _resultType;
	std::string _name;
	/* this will be an empty list and not null ptr in case of no parameters */
	ParameterList* _parameters;
};

class FunctionDeclaration : public FunctionHead {
public:
	FunctionDeclaration(TypeExpression* typeExpr, std::string name, ParameterList* parameters) {
		this->_resultType = typeExpr; this->_name = name; this->_parameters = parameters;
	}
	~FunctionDeclaration();
	virtual void embedType(TypeExpression* type) override {}

	virtual void sem() override;

	/**
	 * The Name to handle overloading
	 */
	virtual std::string getName() override {
		return this->_name + this->_parameters->getAggregatedName();
	}

	virtual llvm::Value* codegen() override;


	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)this->_resultType, (Tree*)this->_parameters}; };
	virtual void printNode(std::ostream& out) override { out << "FunctionDeclaration(" << _name << ")"; };
	virtual std::string toJSONString() override;

};

class FunctionDefinition : public FunctionHead {
public:
	FunctionDefinition(TypeExpression* typeExpr, std::string name, ParameterList* parameters, 
	DeclarationList* decls, StatementList* statements) :  _decls(decls), _statements(statements) {
		this->_resultType = typeExpr; this->_name = name; this->_parameters = parameters;
	}
	~FunctionDefinition();
	virtual void embedType(TypeExpression* type) override {}

	/**
	 * The Name to handle overloading
	 */
	virtual std::string getName() override {
		return this->_name + this->_parameters->getAggregatedName();
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)this->_resultType, (Tree*)this->_parameters, (Tree*)this->_decls, (Tree*)this->_statements}; }
	virtual void printNode(std::ostream& out) override { out << "FunctionDefinition(" << _name << ")"; }
	virtual std::string toJSONString() override;
private:
	DeclarationList* _decls;
	StatementList* _statements;
	
};


//====================================================================================//
// Syntax Nodes For Types       												      //
//====================================================================================//

class BasicType : public TypeExpression {
public:
	BasicType(std::string name) : _name(name) {}
	~BasicType();
	virtual std::string getName() override { return this->_name; }
	virtual void penetrate(TypeExpression* me) override { 
		this->_name = me->getName();
	}
	virtual TypeExpression* copy() override {
		return new BasicType(_name);
	}

	virtual std::string getDefName() override {
		return this->_name;
	}
	
	virtual Type toType() override {
		if(this->_name == "void")
			return typeVoid;
		if(this->_name == "int")
			return typeInteger;
		if(this->_name == "char")
			return typeChar;
		if(this->_name == "bool")
			return typeBoolean;
		if(this->_name == "double")
			return typeReal;
		fprintf(stderr, "Error toType: Cannot Transform to Type");
		exit(EXIT_FAILURE);
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

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
	~Pointer();
	virtual std::string getName() override { return "*"; }
	virtual void penetrate(TypeExpression* me) override {
		this->_inner->penetrate(me);
	}
	virtual TypeExpression* copy() override {
		return new Pointer(this->_inner->copy());
	}

	virtual std::string getDefName() override {
		return this->_inner->getDefName() + "_ptr";
	}
	virtual Type toType() override {
		Type inner = this->_inner->toType();
		Type p = typePointer(inner);
		destroyType(inner);
		return p;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {(Tree*)_inner}; };
	virtual void printNode(std::ostream& out) override { out << "Pointer";}
	virtual std::string toJSONString() override;
public:
	TypeExpression* _inner;
};


//====================================================================================//
// Base Node For Statements    														  //
//====================================================================================//

class Statement : public AST {
public:
	// Base Statement Class
	virtual ~Statement();
	virtual void sem() = 0;
	virtual llvm::Value* codegen() = 0;
	/**
	 * @brief Check if a statement returns. Used to determine that all paths
	 * return in a function and this it is well defined.
	 *
	 */
	virtual bool returns() = 0;
};

class EmptyStatement : public Statement {
public:
	// ;
	EmptyStatement() {}
	virtual ~EmptyStatement();
	
	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	virtual bool returns() override {
		return false;
	}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; };
	virtual void printNode(std::ostream& out) override { out << "EmptyStatement"; };
	virtual std::string toJSONString() override;

};

class SingleExpression : public Statement {
public: 
	SingleExpression(Expression* expr)
	: _expr(expr) {}
	virtual ~SingleExpression();

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	virtual bool returns() override {
		return false;
	}

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
	virtual ~IfStatement();

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	virtual bool returns() override {
		return false;
	}

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
	virtual ~IfElseStatement();

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	virtual bool returns() override {
		bool lr = _ifbody->returns(), rr = _elsebody->returns();
		if( lr && rr)
			return true;
		else return false;
	}

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
	virtual ~ForStatement();

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	virtual bool returns() override {
		return false;
	}

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
	virtual ~ContinueStatement();

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	virtual bool returns() override {
		return false;
	}

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
	virtual ~BreakStatement();

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	virtual bool returns() override {
		return false;
	}

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
	virtual ~ReturnStatement();
	
	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	virtual bool returns() override {
		return true;
	}

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


//====================================================================================//
// Syntax Nodes For Expressions (They also have types)								  //
//====================================================================================//

class Expression : public AST, public TypedExpression {
public:
	virtual ~Expression();

	/**
	 * @brief Get if the expression is an IntConstant for defining
	 * the size of an array
	 * 
	 * @return int if zero non correct if non-zero correct and is the size.
	 */
	virtual int isIntConstant() = 0;

	virtual void sem() = 0;
	virtual llvm::Value* codegen() = 0;
	/**
	 * @brief This calculates the address of an lvalue and returns
	 * a llvm::Value with the pointer. Should not be called on r-vals.
	 * 
	 * @return llvm::Value* 
	 */
	virtual llvm::Value* calculateAddressOf() = 0;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() = 0;
	virtual void printNode(std::ostream& out) = 0;
	virtual std::string toJSONString() = 0;
};

class Id : public Expression {
public:
	Id(std::string name) : _name(name) {}
	virtual ~Id() = default;

	virtual int isIntConstant() override {
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

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
	virtual ~Constant();
	
	// What constant this nodes represents
	enum ConstantType { Bool, Null, Int, Char, Double, String };

	virtual int isIntConstant() override {
		switch (this->_ct)
		{
		case Bool: case Null: case Char: case Double: case String:
			return 0;
		case Int:
			return this->_int;
		}
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

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
	virtual ~FunctionCall();

	virtual int isIntConstant() override {
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

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
	BracketedIndex(Expression* i1, Expression* i2)
	: _in(i2), _out(i1) {} 
	virtual ~BracketedIndex();

	virtual int isIntConstant() override {
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

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
	virtual ~UnaryOp();

	virtual int isIntConstant() override {
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

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
	// Comma not used here but in specific node CommaExpr
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
	virtual ~BinaryOp();

	virtual int isIntConstant() override {
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

	// see semantical.cpp
	friend void binaryOpAnalysis(BinaryOp&);

	void setLeft(Expression* l) { this->_leftOperand = l; }
	void setRight(Expression* r) { this->_rightOperand = r; }


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
	// Base class to handle the common part of postfix and prefix unary assignment.
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
	virtual ~UnAss();

	// virtual int isIntConstant() = 0;
	// virtual void sem() = 0;
	// virtual llvm::Value* codegen() = 0;
	virtual llvm::Value* calculateAddressOf() override;

	/* Printing Syntax Tree Functions */
	// virtual std::vector<Tree*> getChildren() = 0;
	// virtual void printNode(std::ostream& out) = 0;
	// virtual std::string toJSONString() = 0;
protected:
	UnAssType _Unass;
	Expression* _operand;
};

class PrefixUnAss : public UnAss {
public:	
	PrefixUnAss(UnAss::UnAssType unass, Expression* operand) 
	: UnAss(unass, operand) {}
	virtual ~PrefixUnAss();

	virtual int isIntConstant() override {
		return 0;
	}
	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {this->_operand}; }
	virtual void printNode(std::ostream& out) override { out << "PrefixUnAss(" << UnAss::UnAssTypeToString(this->_Unass) << ")"; }
	virtual std::string toJSONString() override;

};

class PostfixUnAss : public UnAss {
public:
	PostfixUnAss(UnAss::UnAssType unass, Expression* operand) 
	: UnAss(unass, operand) {}
	virtual ~PostfixUnAss();

	virtual int isIntConstant() override {
		return 0;
	}
	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

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
	virtual ~BinaryAss();

	virtual int isIntConstant() override{
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

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
	virtual ~TypeCast();

	virtual int isIntConstant() override {
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

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
	virtual ~TernaryOp();

	virtual int isIntConstant() override {
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

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
	// _size can be null! indicating that size is 1.
	New(TypeExpression* type) : _type(type), _size(nullptr) {}
	New(TypeExpression* type, Expression* size) : _type(type), _size(size) {}
	virtual ~New();

	virtual int isIntConstant() override {
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

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
	virtual ~Delete();

	virtual int isIntConstant() override {
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_expr}; }
	virtual void printNode(std::ostream& out) override { out << "Delete"; }
	virtual std::string toJSONString() override;

private:
	Expression* _expr;
};

class CommaExpr : public Expression {
public:
	CommaExpr(Expression* left, Expression* right)
	: _left(left), _right(right) {}
	virtual ~CommaExpr();

	virtual int isIntConstant() override {
		return 0;
	}

	virtual void sem() override;
	virtual llvm::Value* codegen() override;
	virtual llvm::Value* calculateAddressOf() override;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_left, _right}; }
	virtual void printNode(std::ostream& out) override { out << "CommaExpr"; }
	virtual std::string toJSONString() override;

private:
	Expression* _left, *_right;
};


//====================================================================================//
// Class For Handling Labels   														  //
//====================================================================================//

class Label : public AST {
public:
	Label(std::string lblname) : _lblname(lblname) {}
	virtual ~Label();

	std::string getLabelName() { return this->_lblname; }

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {}; }
	virtual void printNode(std::ostream& out) override { out << "Label(" << _lblname << ")"; }
	virtual std::string toJSONString() override;

private:
	std::string _lblname;
};




//====================================================================================//
// Syntax Nodes For Handling Multiples Of Statements, Expressions, Declarations  	  //
//====================================================================================//

class StatementList : public Statement {
public:
	StatementList() : _stmts(std::deque<Statement*>()) {}
	StatementList(std::deque<Statement*> stmts) 
	: _stmts(stmts) {}
	virtual ~StatementList();
	std::deque<Statement*> _stmts;

	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	virtual bool returns() override {
		bool myRet = false;
		for(auto &stmt : this->_stmts){
			myRet = myRet || stmt->returns();
		}
		return myRet;
	}

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_stmts.begin(), _stmts.end()}; }
	virtual void printNode(std::ostream& out) override { out << "StatementList"; }
	virtual std::string toJSONString() override;

};

class ExpressionList : public AST {
public:

	ExpressionList() : _expressions(std::deque<Expression*>()) {}
	ExpressionList(std::deque<Expression*> expressions) : _expressions(expressions) {}
	virtual ~ExpressionList();
	std::deque<Expression*> _expressions;

	virtual void sem() override;
	virtual llvm::Value* codegen() override;


	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_expressions.begin(), _expressions.end()}; }
	virtual void printNode(std::ostream& out) override { out << "ExpressionList"; }
	virtual std::string toJSONString() override;

};

class DeclarationList : public Declaration {
public:
	DeclarationList() : _decls(std::deque<Declaration*>()) {}
	DeclarationList(std::deque<Declaration*> decls) : _decls(decls) {}
	virtual ~DeclarationList();
	std::deque<Declaration*> _decls;
	virtual void embedType(TypeExpression* t) override {
		for(Declaration* i : this->_decls)
			i->embedType(t);
	}
	virtual std::string toJSONString() override;
	

	virtual std::string getName() override { return "";}
	virtual void sem() override;
	virtual llvm::Value* codegen() override;

	/* Printing Syntax Tree Functions */
	virtual std::vector<Tree*> getChildren() override { return {_decls.begin(), _decls.end()}; }
	virtual void printNode(std::ostream& out) override { out << "DeclarationList"; }
};


#endif/*__AST_HPP__*/