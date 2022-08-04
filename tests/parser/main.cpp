#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "ast.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include <string>

#define print(it) std::cout << it << std::endl

AST* syntaxTree;
int main(){
	int ret = yyparse();

	if(syntaxTree==nullptr){
		std::cerr << "Got back nullptr" << std::endl;
		exit(EXIT_FAILURE);
	}
	print(syntaxTree->toJSONString());
	// Program *p = new Program(new DeclarationList());

	

	// Declaration* v = new VariableDeclaration(new Pointer(new BasicType("int")), "x2");
	// print(v->toJSONString());

	// Expression* e = new Constant((long double)23.00);
	// print(e->toJSONString());
	// e = new Constant((int16_t)120);
	// print(e->toJSONString());
	// e = new Constant(true);
	// print(e->toJSONString());
	// e = new Constant('c');
	// print(e->toJSONString());

	// e = new Constant((std::string)"hellopeopl\\n");
	// print(e->toJSONString());
	
	// e = new BinaryOp(BinaryOp::BinaryOpType::MINUS, new Constant(false), new Constant((long double)32.00));
	// print(e->toJSONString());
	// ExpressionList* el = new ExpressionList({e, new Constant((int16_t)32)});
	// print(el->toJSONString());
	return ret;
}
