#include <cstdio>
#include <cstdlib>
#include <vector>
#include "ast.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include <string>
#include "symbol.hpp"

AST* syntaxTree = nullptr;

int main(){
	
	//VariableDeclaration*v = new ArrayDeclaration(new BasicType("int"),"x",new Constant((int16_t)12));
	//delete v;

	int ret = yyparse();
	syntaxTree->printTree(std::cout);
	// syntaxTree->sem();
	syntaxTree->codegen();
	delete syntaxTree;
	return ret;
}
