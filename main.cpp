#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include "ast.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include <string>
#include "symbol.hpp"
#include "lexer_funcs.hpp"

AST* syntaxTree = nullptr;
extern std::set<std::string> fileset;


int main(int argc, char** argv){
	
	//VariableDeclaration*v = new ArrayDeclaration(new BasicType("int"),"x",new Constant((int16_t)12));
	//delete v;
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
	yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));

	int ret = yyparse();
	syntaxTree->printTree(std::cout);
	syntaxTree->sem();
	delete syntaxTree;
	return ret;
}
