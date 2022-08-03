#include <cstdio>
#include <cstdlib>
#include <vector>
#include "ast.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include <string>

AST* syntaxTree = nullptr;

int main(){
	
	int ret = yyparse();
	syntaxTree->printTree(std::cout);
	return ret;
}