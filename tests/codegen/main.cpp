#include "ast.hpp"
#include "parser.hpp"
#include <string>
#include "general.hpp"

AST* syntaxTree = nullptr;


int main(int argc, char** argv){
	
	handleArguments(argc, argv);

	int ret = yyparse();
	
	// print Syntax Tree
	// syntaxTree->printTree(std::cout);

	syntaxTree->codegen();
	
	handleOutput();
	
	delete syntaxTree;
	
	return ret;
}



