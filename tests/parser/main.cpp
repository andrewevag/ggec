#include <iostream>
#include "ast.hpp"
#define print(it) std::cout << it << std::endl
int main(){
	Program *p = new Program(new DeclarationList());

	// print(p->toJSONString());

	Declaration* v = new VariableDeclaration(new Pointer(new BasicType("int")), "x2");
	print(v->toJSONString());

	Expression* e = new Constant((long double)23.00);
	print(e->toJSONString());
	e = new Constant((int16_t)120);
	print(e->toJSONString());
	e = new Constant(true);
	print(e->toJSONString());
	e = new Constant('c');
	print(e->toJSONString());

	e = new Constant((std::string)"hellopeopl\\n");
	print(e->toJSONString());
	
	e = new BinaryOp(BinaryOp::BinaryOpType::MINUS, new Constant(false), new Constant((long double)32.00));
	print(e->toJSONString());
	ExpressionList* el = new ExpressionList({e, new Constant((int16_t)32)});
	print(el->toJSONString());
}
