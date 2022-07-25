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
#define T_string_const  1022

#define T_eq            1023
#define T_neq           1024
#define T_geq           1025   
#define T_leq           1026
#define T_land          1027
#define T_lor           1028
#define T_plusplus      1029
#define T_minusminus    1030
#define T_pluseq        1031
#define T_minuseq       1032
#define T_multeq        1033
#define T_diveq         1034
#define T_modeq         1035




int lineno = 1;
%}

L [a-zA-Z]
D [0-9]
W [ \t\r]
P [^\x00-\x1f\x7f\\\'\"]

%option noyywrap

%x CHAR
%x STRING
%x COMMENT

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


[\=\+\-\*\/\%\(\)\<\>\&\!\?\:\,\;\(\)\{\}\[\]]    { return yytext[0]; }

{L}({L}|{D}|_)*	                    { return T_id; }
{D}+	                            { return T_int_const; }
{D}+(\.{D}+([eE][\-\+]?{D}+)?)      { return T_double_const; }


\'                                  { BEGIN(CHAR); }
<CHAR>\\x[0-9a-fA-F][0-9a-fA-F]\'   { BEGIN(INITIAL); return T_char_const; }
<CHAR>\\[ntr0\\\'\"]\'              { BEGIN(INITIAL); return T_char_const; }
<CHAR>{P}\'                         { BEGIN(INITIAL); return T_char_const; }


\"                                  { BEGIN(STRING); }
<STRING>\"                          { BEGIN(INITIAL); return T_string_const; }
<STRING>\\x[0-9a-fA-F][0-9a-fA-F]   { /* nothing */ }
<STRING>\\[ntr0\\\'\"]              { /* nothing */ }
<STRING>{P}                         { /* nothing */ }


\n                                  { ++lineno; }
{W}+	                            { /* nothing */ }
\/\/.*\n                            { ++lineno; }

"/*"                                { BEGIN(COMMENT); }
<COMMENT>"*/"                       { BEGIN(INITIAL); }
<COMMENT>\n                         { ++lineno; }
<COMMENT>"*"                        { /* nothing */ }
<COMMENT>[^*\n]+                    { /* nothing */ }

.	                                { fprintf(stderr,"Illegal character %c at line %d\n",yytext[0],lineno);
                                        exit(1); }

%%
