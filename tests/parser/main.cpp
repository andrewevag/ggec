#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include "ast.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include <string>
#include "lexer_funcs.hpp"
#define print(it) std::cout << it << std::endl

AST* syntaxTree;
extern std::set<std::string> fileset;
extern std::string currentFilename;

int main(int argc, char** argv){
	if(argc <= 1){
		fprintf(stderr, "Usage: ./ggec <filename>\n");
		exit(EXIT_FAILURE);
	}
	yypop_buffer_state();
	yyin = fopen(argv[1], "r");
	if(! yyin){
		fprintf(stderr, "Failed to open input file\n");
		exit(1);
	}
	fileset = { argv[1] };
	currentFilename = fileset.find(argv[1])->c_str();
	yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));

	int ret = yyparse();

	if(syntaxTree==nullptr){
		std::cerr << "Got back nullptr" << std::endl;
		exit(EXIT_FAILURE);
	}
	print(syntaxTree->toJSONString());
	// Program *p = new Program(new DeclarationList());

	
	delete syntaxTree;
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
