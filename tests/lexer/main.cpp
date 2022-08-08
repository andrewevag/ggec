#include <cstdio>
#include <cstdlib>
#include <vector>
#include "ast.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include <string>

AST* syntaxTree = nullptr;

extern char* yytext;

int main(){
	int token;
	do{
		token = yylex();
		printf("%d \"%s\"\n", token, yytext);
	}while(token != 0);


	return 0;
}