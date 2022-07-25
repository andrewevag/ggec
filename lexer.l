%{

#define T_eof           0
#define T_bool          1001
#define T_break         1002
#define T_byref         1003
#define T_char          1004
#define T_continue      1005
#define T_delete        1006
#define T_double        1007
#define T_else          1008
#define T_for           1009
#define T_false         1010
#define T_if            1011
#define T_int           1012
#define T_new           1013
#define T_NULL          1014 
#define T_return        1015
#define T_true          1016
#define T_void          1017
#define T_id            1018
#define T_int_const     1019
#define T_double_const  1020
#define T_char_const    1021

int lineno = 1;
%}

L [a-z]
D [0-9]
W [ \t\r]
P [\x20-\x26\x28-\x7f]

%option noyywrap

%%

"bool"	    { return T_bool; }
"break"	    { return T_break; }
"byref"	    { return T_byref; }
"char"	    { return T_char; }
"continue"	{ return T_continue; }
"delete"	{ return T_delete; }
"double"	{ return T_double; }
"else"	    { return T_else; }
"for"	    { return T_for; }
"false"     { return T_false; }
"if"	    { return T_if; }
"int"	    { return T_int; }
"new"	    { return T_new; }
"NULL"	    { return T_NULL; }
"return"	{ return T_return; }
"true"	    { return T_true; }
"void"	    { return T_void; }


[\=\+\-\*\/\%\(\)]	{ return yytext[0]; }

{L}({L}|{D}|_)*	    { return T_id; }
{D}+	            { return T_int_const; }
{D}+(\.{D}+((e|E)(\-|\+)?{D}+)?) { return T_double_const; }
\'({P}|(\\(n|t|r|0|\\|\'|\"|(x[0-9a-fA-F][0-9a-fA-F]))))\'  { return T_char_const; }

\n      { ++lineno; }
{W}+	{ /* ignore */ }
'.*\n	{ ++lineno; }

.	{ fprintf(stderr,"Illegal character %c at line %d\n",yytext[0],lineno);
          exit(42); }

%%
