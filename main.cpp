#include "ast.hpp"
#include "parser.hpp"
#include <string>
#include "general.hpp"

AST* syntaxTree = nullptr;


int main(int argc, char** argv){
	
	handleArguments(argc, argv);

	int ret = yyparse();
	
	syntaxTree->codegen();
	
	handleOutput();
	
	delete syntaxTree;
	
	return ret;
}



