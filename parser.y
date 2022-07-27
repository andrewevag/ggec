%{

#include <string>
#include <vector>
#include "ast.hpp"
#include "lexer.hpp"


%}

%token T_bool          "bool"
%token T_break         "break"
%token T_byref         "byref"
%token T_char          "char"
%token T_continue      "continue"
%token T_delete        "delete"
%token T_double        "double"
%token T_else          "else"
%token T_for           "for"
%token T_false         "false"
%token T_if            "if"
%token T_int           "int"
%token T_new           "new"
%token T_NULL          "NULL" 
%token T_return        "return"
%token T_true          "true"
%token T_void          "void"
%token<std::string> T_id            
%token<uint16_t> T_int_const     
%token<long double> T_double_const  
%token<char> T_char_const    
%token<std::string> T_string_const  
%token T_eq            "=="
%token T_neq           "!="
%token T_geq           ">="   
%token T_leq           "<="
%token T_land          "&&"
%token T_lor           "||"
%token T_plusplus      "++"
%token T_minusminus    "--"
%token T_pluseq        "+="
%token T_minuseq       "-="
%token T_multeq        "*="
%token T_diveq         "/="
%token T_modeq         "%="

%precedence NO_COMMA EMPTY_STAR
%left ','
%precedence ARGLIST
%right '=' "+=" "-=" "*=" "/=" "%="
%nonassoc '?' ':'
%left "||"
%left "&&"
%nonassoc "==" "!=" '>' '<' "<=" ">="
%left '+' '-'
%left '*' '/' '%'
%nonassoc TYPECAST
%right IPLUSPLUS IMINUSMINUS "++" "--"
%right NEW DELETE
%right ADDRESS DEREF PLUSIGN MINUSIGN NEGATION
%left  PPLUSPLUS PMINUSMINUS
%nonassoc '(' ')' '[' ']' /* remember to fix if not what we want */



%type<decList> declaration_list //??
%type<decl> declaration
%type<decl> variable_declaration
%type<decl> function_declaration
%type<decl> function_definition
%type<typeExpr> type
%type<par> parameter
%type<parList> parameter_list //??
%type<stmtList> statement_list //??
%type<stmt> statement
%type<lbl> label
%type<expr> expression_or_empty
%type<exprList> expression_list
%type<expr> expression
%type<expr> constant_expression
%type<expr> no_comma_expression
%type<decList> sep_by_comma_declarator
%type<exprList> sep_by_comma_expression
%type<parList> sep_by_comma_parameter

// %expect 1
%union {
    DeclarationList* decList;
    Declaration* decl;
    TypeExpression* typeExpr;
    Parameter* par;
    ParameterList* parList;
    Statement* stmt;
    StatementList* stmtList;
    Expression* expr;
    ExpressionList* exprList;
    Label* lbl;
}

%%


program:
    declaration declaration_list
;

declaration_list:
    /* nothing */
|   declaration_list declaration
;

declaration:
    variable_declaration
|   function_declaration
|   function_definition
;

variable_declaration:
    type_id sep_by_comma_declarator ';'
|   type_id '[' constant_expression ']' sep_by_comma_declarator ';'
;

sep_by_comma_declarator:
    /* nothing */
|   ',' declarator sep_by_comma_declarator
;

type:
    basic_type star_list
;

type_id:
    type T_id
;

star_list:
    /* nothing */ %prec EMPTY_STAR
|   '*' star_list 
;

// one_or_more_stars:
//     '*'
// |   '*' one_or_more_stars    
// ;

basic_type:
    "int"
|   "char"
|   "bool"
|   "double"
;

declarator:
    T_id
|   T_id '[' constant_expression ']'
;

function_head:
    type_id '(' ')'
|   type_id '(' parameter_list ')'
|   "void" T_id '(' ')'
|   "void" T_id '(' parameter_list ')'
;

// thelei factoring
function_declaration:
    function_head ';'
;

// result_type:
//     type
// |   "void"
// ;

parameter_list:
    parameter sep_by_comma_parameter
;

sep_by_comma_parameter:
    /* nothing */
|   ',' parameter sep_by_comma_parameter
;

parameter:
    "byref" type_id
|   type_id
;

function_definition:
    function_head '{' declaration_list statement_list '}'
;

statement_list:
    /* nothing */
|   statement statement_list
;

statement:
    ';'
|   expression ';'
|   '{' statement_list '}'
|   "if" '(' expression ')' statement
|   "if" '(' expression ')' statement "else" statement
|   label "for" '(' expression_or_empty ';' expression_or_empty ';' expression_or_empty ')' statement
|   "continue" ';'
|   "continue" T_id ';'
|   "break" ';'
|   "break" T_id ';'
|   "return" expression_or_empty ';'
;

label:
    /* nothing */
|   T_id ':'
;

expression_or_empty:
    /* nothing */
|   expression
;

expression:
    no_comma_expression %prec NO_COMMA
|   no_comma_expression ',' expression 
;


no_comma_expression:
    T_id
|   '(' expression ')'
|   "true"
|   "false"
|   "NULL"
|   T_int_const
|   T_char_const
|   T_double_const
|   T_string_const
|   T_id '(' ')'
|   T_id '(' expression_list ')' 
|   expression '[' expression ']'
|   '&' expression %prec ADDRESS
|   '*' expression %prec DEREF
|   '+' expression %prec PLUSIGN
|   '-' expression %prec MINUSIGN
|   '!' expression %prec NEGATION
|   expression '*' expression
|   expression '/' expression
|   expression '%' expression
|   expression '+' expression
|   expression '-' expression
|   expression '<' expression
|   expression '>' expression
|   expression "<=" expression
|   expression ">=" expression
|   expression "==" expression
|   expression "!=" expression
|   expression "&&" expression
|   expression "||" expression
|   "++" expression %prec IPLUSPLUS
|   "--" expression %prec IMINUSMINUS
|   expression "++" %prec PPLUSPLUS
|   expression "--" %prec PMINUSMINUS
|   expression '=' expression
|   expression "*=" expression
|   expression "/=" expression
|   expression "%=" expression
|   expression "+=" expression
|   expression "-=" expression
|   '(' type ')' expression %prec TYPECAST
|   expression '?' expression ':' expression
|   "new" type %prec NEW
|   "new" type '[' expression ']' %prec NEW
|   "delete" expression %prec DELETE
;

expression_list:
    no_comma_expression sep_by_comma_expression %prec ARGLIST
;

sep_by_comma_expression:
    /* nothing */
|   ',' no_comma_expression sep_by_comma_expression
;

constant_expression:
    expression
;


%%