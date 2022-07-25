#include <cstdio>

extern int yylex();
extern char* yytext;


int main(){
	int token;
	do{
		token = yylex();
		printf("tokenId=%d, lexeme=\"%s\" ", token, yytext);
	}while(token != 0);


	return 0;
}