#include <cstdio>
#include <cstdlib>
#include <vector>
#include "ast.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include <string>


int main(){
	
	//VariableDeclaration*v = new ArrayDeclaration(new BasicType("int"),"x",new Constant((int16_t)12));
	//delete v;

	int ret = yyparse();
	return ret;
}