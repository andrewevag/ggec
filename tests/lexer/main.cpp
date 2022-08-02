#include <cstdio>
#include <lexer.hpp>


extern char* yytext;

int main(){
	int token;
	do{
		token = yylex();
		printf("%d \"%s\"\n", token, yytext);
	}while(token != 0);


	return 0;
}