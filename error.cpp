#include <cstdio>
#include <cstdlib>
#include <cstdarg>
extern int lineno;


void yyerror(const char *msg)
{
	fprintf(stderr, "Error in line: %d\n", lineno);
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}