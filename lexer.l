%{
#include <vector>
#include <set>
#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <cstdio>

std::string tempstr;
std::set<std::string> fileset;
int lineno = 1;
int columnno = 1;
std::string currentFilename;
#define YY_USER_ACTION \
    for(int i = 0; yytext[i] != '\0'; i++) { \
        if(yytext[i] == '\n') { \
            columnno = 0; \
        } \
        else { \
            columnno++; \
        } \
    }

%}

L [a-zA-Z]
D [0-9]
W [ \t\r]
P [^\x00-\x1f\x7f\\\'\"]

%option header-file="lexer_funcs.hpp"
%option noyywrap
%option nounput noinput
%x CHAR
%x STRING
%x COMMENT
%x INCLUDE
%x INCLUDESTRING

%%

^"#include"         { BEGIN(INCLUDE); }
<INCLUDE>{W}+       { /* nothing */   }
<INCLUDE>\"         { BEGIN(INCLUDESTRING); tempstr = ""; }
                        
<INCLUDESTRING>\"   { 
                        if(fileset.count(tempstr) != 0){
                            fprintf(stderr, "File \"%s\" already included\n", tempstr.c_str());
                            exit(1);
                        }
                        yyin = fopen( tempstr.c_str(), "r" );
                        if( ! yyin){
                            fprintf(stderr, "failed to open %s\n", tempstr.c_str());
                            exit(1);
                        }
                        fileset.insert(tempstr);
                    	currentFilename = fileset.find(tempstr)->c_str();
                        yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
                        BEGIN(INITIAL);
                    }

<INCLUDESTRING>\\x[0-9a-fA-F][0-9a-fA-F]   { tempstr.append(yytext); }
<INCLUDESTRING>\\[ntr0\\\'\"]              { tempstr.append(yytext); }
<INCLUDESTRING>{P}                         { tempstr.append(yytext); }
<INCLUDESTRING>.|\n                        { fprintf(stderr, "didnot expect non printable in string %d\n", yytext[0]); exit(1);}
<INCLUDESTRING><<EOF>>                     { fprintf(stderr, "received EOF without string closing\n"); exit(1);}


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

{L}({L}|{D}|_)*	                    { char* s = new char[strlen(yytext)+1]; memcpy(s, yytext, strlen(yytext)+1); yylval.str = s; return T_id; }
{D}+	                            { yylval.i = atoi(yytext); return T_int_const; }
{D}+(\.{D}+([eE][\-\+]?{D}+)?)      { yylval.d = strtold(yytext, NULL); return T_double_const; }


\'                                  { BEGIN(CHAR); }
<CHAR>\\x[0-9a-fA-F][0-9a-fA-F]\'   { yylval.c = getCharFromString(yytext); BEGIN(INITIAL); return T_char_const; }
<CHAR>\\[ntr0\\\'\"]\'              { yylval.c = getCharFromString(yytext); BEGIN(INITIAL); return T_char_const; }
<CHAR>{P}\'                         { yylval.c = yytext[0]; BEGIN(INITIAL); return T_char_const; }
<CHAR>.|\n                          { fprintf(stderr, "didnot expect non printable in string %d\n", yytext[0]); exit(1); }
<CHAR><<EOF>>                       { fprintf(stderr, "received EOF without string closing\n"); exit(1);}

\"                                  { BEGIN(STRING); tempstr = ""; }
<STRING>\"                          { yylval.str = new char[strlen(tempstr.c_str())+1]; memcpy(yylval.str, tempstr.c_str(), strlen(tempstr.c_str())+1); ; BEGIN(INITIAL); return T_string_const; }
<STRING>\\x[0-9a-fA-F][0-9a-fA-F]   { tempstr.append(yytext); }
<STRING>\\[ntr0\\\'\"]              { tempstr.append(yytext); }
<STRING>{P}                         { tempstr.append(yytext); }
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

<<EOF>>                             { yypop_buffer_state(); if (!YY_CURRENT_BUFFER) {yytext = ""; return 0;} }
%%


uint8_t getCharFromString(char* s){
    uint8_t c;
    if(s[0] == '\\'){
        if(strlen(s) < 2){
            yyerror("getCharFromString: got \\ but no character after that!\n");
        }
        switch(s[1]) {
            case '\\':
                c = '\\';
                break;
            case 'n':
                c = '\n';
                break;
            case 't':
                c = '\t';
                break;
            case 'r' :
                c = '\r';
                break;
            case '0':
                c = '\0';
                break;
            case 'x':
                if (strlen(s) < 4){
                    yyerror("getCharFromString: got \\x but no 2 digit hecadecimal number afterwards");
                }
                int i;
                sscanf(s+2, "%02x", &i);
                return i;
            default:
                c=s[1];
        }
        return c;
    }else{
        return (uint8_t)s[0];
    }
}