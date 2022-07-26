%{



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


"=="        { return T_eq; }
"!="        { return T_neq; }
">="        { return T_geq; }
"<="        { return T_leq; }
"&&"        { return T_land; }
"||"        { return T_lor; }
"++"        { return T_plusplus; }
"--"        { return T_minusminus; }
"+="        { return T_pluseq; }
"-="        { return T_minuseq; }
"*="        { return T_multeq; }
"/="        { return T_diveq; }
"%="        { return T_modeq; }
[\=\+\-\*\/\%\(\)\<\>\&\!\?\:\,\;\(\)\{\}\[\]]    { return yytext[0]; }

{L}({L}|{D}|_)*	                    { return T_id; }
{D}+	                            { return T_int_const; }
{D}+(\.{D}+([eE][\-\+]?{D}+)?)      { return T_double_const; }


\'                                  { BEGIN(CHAR); }
<CHAR>\\x[0-9a-fA-F][0-9a-fA-F]\'   { BEGIN(INITIAL); return T_char_const; }
<CHAR>\\[ntr0\\\'\"]\'              { BEGIN(INITIAL); return T_char_const; }
<CHAR>{P}\'                         { BEGIN(INITIAL); return T_char_const; }
<CHAR>.|\n                          { fprintf(stderr, "didnot expect non printable in string %d\n", yytext[0]); exit(1); }
<CHAR><<EOF>>                       { fprintf(stderr, "received EOF without string closing\n"); exit(1);}

\"                                  { BEGIN(STRING); }
<STRING>\"                          { BEGIN(INITIAL); return T_string_const; }
<STRING>\\x[0-9a-fA-F][0-9a-fA-F]   { /* nothing */ }
<STRING>\\[ntr0\\\'\"]              { /* nothing */ }
<STRING>{P}                         { /* nothing */ }
<STRING>.|\n                        { fprintf(stderr, "didnot expect non printable in string %d\n", yytext[0]); exit(1);}
<STRING><<EOF>>                     { fprintf(stderr, "received EOF without string closing\n"); exit(1);}

\n                                  { ++lineno; }
{W}+	                            { /* nothing */ }
\/\/.*\n                            { ++lineno; }

"/*"                                { BEGIN(COMMENT); }
<COMMENT>"*/"                       { BEGIN(INITIAL); }
<COMMENT>\n                         { ++lineno; }
<COMMENT>"*"                        { /* nothing */ }
<COMMENT>[^*\n]+                    { /* nothing */ }
<COMMENT><<EOF>>                    { fprintf(stderr, "received EOF without string closing\n"); exit(1);}
.	                                { fprintf(stderr,"Illegal character %c at line %d\n",yytext[0],lineno);
                                        exit(1); }

%%
