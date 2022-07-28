#include <cstdio>
#include <cstdlib>
#include <vector>
#include "ast.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include <string>


int main(){
	
	int ret = yyparse();
	return ret;
}