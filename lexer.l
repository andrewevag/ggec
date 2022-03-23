%{
#include "lexer.h"
#include "parser.h"

#define T_eof   0

int lineno = 1;
%}

L [a-z]
D [0-9]
W [ \t\r]

%option noyywrap

%%

"begin"	{ return T_begin; }
"do"	{ return T_do; }
"else"	{ return T_else; }
"end"	{ return T_end; }
"for"	{ return T_for; }
"if"	{ return T_if; }
"let"	{ return T_let; }
"print"	{ return T_print; }
"then"	{ return T_then; }

[\=\+\-\*\/\%\(\)]	{ return yytext[0]; }

{L}	{ return T_id; }
{D}+	{ return T_const; }

\n      { ++lineno; }
{W}+	{ /* ignore */ }
'.*\n	{ ++lineno; }

.	{ fprintf(stderr,"Illegal character %c at line %d\n",yytext[0],lineno);
          exit(42); }

%%