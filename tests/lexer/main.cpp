#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include "ast.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include <string>
#include "lexer_funcs.hpp"

AST* syntaxTree = nullptr;
extern std::set<std::string> fileset;
extern std::string currentFilename;


extern char* yytext;
int main(int argc, char** argv){
	if(argc <= 1){
		fprintf(stderr, "Usage: ./ggec <filename>\n");
		exit(EXIT_FAILURE);
	}
	yypop_buffer_state();
	yyin = fopen(argv[1], "r");
	if(! yyin){
		fprintf(stderr, "Failed to open input file\n");
		exit(1);
	}
	fileset = { argv[1] };
	currentFilename = fileset.find(argv[1])->c_str();
	yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));

	int token;
	do{
		token = yylex();
		printf("%d \"%s\"\n", token, yytext);
	}while(token != 0);


	return 0;
}