#include <cstdio>
#include <cstdlib>
#include <vector>
#include "lexer.hpp"
#include "ast.hpp"
#include "parser.hpp"


extern char* yytext;

void yyerror(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}


int main(){
	int ret = yyparse();
	return ret;
}