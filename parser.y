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
%token<str> T_id            
%token<i> T_int_const     
%token<d> T_double_const  
%token<c> T_char_const    
%token<str> T_string_const  
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
%type<decl> declaration  declarator
%type<decl> variable_declaration
%type<decl> function_declaration
%type<decl> function_definition
%type<typeExpr> type star_list basic_type
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
    Expression* expr;
    ExpressionList* exprList;
    StatementList* stmtList;
    Label* lbl;
    int16_t i;
    char* str;
    long double d;
    char c;
}

%%


program:
    declaration declaration_list  { $2->_decls.push_front($1); Program* p = new Program($2); p->printTree(std::cout);}
;

declaration_list:
    /* nothing */                  { $$ = new DeclarationList(); }
|   declaration_list declaration   { $1->_decls.push_back($2); $$ = $1; }
;

declaration:
    variable_declaration           { $$ = $1; }
|   function_declaration           { $$ = $1; }
|   function_definition            { $$ = $1; }
;

variable_declaration:
    type T_id sep_by_comma_declarator ';'                               { $3->_decls.push_front(new VariableDeclaration($1, $2)); for(auto &i : $3->_decls) i->embedType($1->copy()); $$ = $3;  }
|   type T_id '[' constant_expression ']' sep_by_comma_declarator ';'   { $6->_decls.push_front(new ArrayDeclaration($1, $2, $4)); for(auto &i : $6->_decls) i->embedType($1->copy()); $$ = $6; }
;

sep_by_comma_declarator:
    /* nothing */                           { $$ = new DeclarationList(); }
|   ',' declarator sep_by_comma_declarator  { $3->_decls.push_back($2); $$ = $3; }
;

type:
    basic_type star_list { $2->penetrate($1); delete $1; $$ = $2; }
;

star_list:
    /* nothing */ %prec EMPTY_STAR { $$ = new BasicType(""); }
|   '*' star_list { $$ = new Pointer($2); }
;

basic_type:
    "int"       { $$ = new BasicType("int");  }
|   "char"      { $$ = new BasicType("char"); }
|   "bool"      { $$ = new BasicType("bool"); }
|   "double"    { $$ = new BasicType("double"); }
;

declarator:
    T_id                                { $$ = new VariableDeclaration(nullptr, $1); }
|   T_id '[' constant_expression ']'    { $$ = new ArrayDeclaration(nullptr, $1, $3); }
;

function_declaration:
    type T_id '(' ')' ';'                   { $$ =  new FunctionDeclaration($1, $2, nullptr); }
|   type T_id '(' parameter_list ')' ';'    { $$ = new FunctionDeclaration($1, $2, $4); }
|   "void" T_id '(' ')' ';'                 { $$ = new FunctionDeclaration(new BasicType("void"), $2, nullptr); }
|   "void" T_id '(' parameter_list ')' ';'  { $$ = new FunctionDeclaration(new BasicType("void"), $2, $4); }
;

parameter_list:
    parameter sep_by_comma_parameter        { $2->_parameters.push_front($1); $$ = $2; }
;

sep_by_comma_parameter:
    /* nothing */                            { $$ = new ParameterList(); }
|   ',' parameter sep_by_comma_parameter     { $3->_parameters.push_front($2); $$ = $3;}
;

parameter:
    "byref" type T_id       { $$ = new Parameter(Parameter::ByRef, $2, $3); }
|   type T_id               { $$ = new Parameter(Parameter::ByCall, $1, $2); }
;

function_definition:
    type T_id '(' ')' '{' declaration_list statement_list '}'                   { $$ = new FunctionDefinition($1, $2, nullptr, $6, $7); }
|   type T_id '(' parameter_list ')' '{' declaration_list statement_list '}'    { $$ = new FunctionDefinition($1, $2, $4, $7, $8); }
|   "void" T_id '(' ')' '{' declaration_list statement_list '}'                 { $$ = new FunctionDefinition(new BasicType("void"), $2, nullptr, $6, $7); }
|   "void" T_id '(' parameter_list ')' '{' declaration_list statement_list '}'  { $$ = new FunctionDefinition(new BasicType("void"), $2, $4, $7, $8); }
;


statement_list:
    /* nothing */                               { $$ = new StatementList(); }
|   statement statement_list                    { $2->_stmts.push_front($1); $$ = $2; }
;

statement:
    ';'                                         { $$ = new EmptyStatement(); }
|   expression ';'                              { $$ = new SingleExpression($1); }
|   '{' statement_list '}'                      { $$ = $2; }
|   "if" '(' expression ')' statement                   { $$ = new IfStatement($3, $5); }
|   "if" '(' expression ')' statement "else" statement  { $$ = new IfElseStatement($3, $5, $7); }
|   label "for" '(' expression_or_empty ';' expression_or_empty ';' expression_or_empty ')' statement { $$ = new ForStatement($1, $4, $6, $8, $10); }
|   "continue" ';'                              { $$ = new ContinueStatement(); }
|   "continue" T_id ';'                         { $$ = new ContinueStatement($2); }
|   "break" ';'                                 { $$ = new BreakStatement(); }
|   "break" T_id ';'                            { $$ = new BreakStatement($2); }
|   "return" expression_or_empty ';'            { $$ = new ReturnStatement($2); }
;

label:
    /* nothing */                               { $$ = nullptr; }
|   T_id ':'                                    { $$ = new Label($1); }
;

expression_or_empty:
    /* nothing */                               { $$ = nullptr; }
|   expression                                  { $$ = $1; }
;

expression:
    no_comma_expression %prec NO_COMMA          { $$ = $1; }
|   no_comma_expression ',' expression          { $$ = new CommaExpr($1, $3); }
;


no_comma_expression:
    T_id                                        { $$ = new Id($1); }
|   '(' expression ')'                          { $$ = $2; }
|   "true"                                      { $$ = new Constant(true); }
|   "false"                                     { $$ = new Constant(false); }
|   "NULL"                                      { $$ = new Constant(); }
|   T_int_const                                 { $$ = new Constant($1); }
|   T_char_const                                { $$ = new Constant($1); }
|   T_double_const                              { $$ = new Constant($1); }
|   T_string_const                              { $$ = new Constant($1); }
|   T_id '(' ')'                                { $$ = new FunctionCall($1, nullptr); }
|   T_id '(' expression_list ')'                { $$ = new FunctionCall($1, $3); }
|   expression '[' expression ']'               { $$ = new BracketedIndex($1, $3); }
|   '&' expression %prec ADDRESS                { $$ = new UnaryOp('&', $2); }
|   '*' expression %prec DEREF                  { $$ = new UnaryOp('*', $2); }
|   '+' expression %prec PLUSIGN                { $$ = new UnaryOp('+', $2); }
|   '-' expression %prec MINUSIGN               { $$ = new UnaryOp('-', $2); }
|   '!' expression %prec NEGATION               { $$ = new UnaryOp('!', $2); }               
|   expression '*' expression                   { $$ = new BinaryOp('*', $1, $3); }
|   expression '/' expression                   { $$ = new BinaryOp('/', $1, $3); }
|   expression '%' expression                   { $$ = new BinaryOp('%', $1, $3); }
|   expression '+' expression                   { $$ = new BinaryOp('+', $1, $3); }
|   expression '-' expression                   { $$ = new BinaryOp('-', $1, $3); }
|   expression '<' expression                   { $$ = new BinaryOp('<', $1, $3); }
|   expression '>' expression                   { $$ = new BinaryOp('>', $1, $3); }
|   expression "<=" expression                  { $$ = new BinaryOp(T_leq, $1, $3); }                  
|   expression ">=" expression                  { $$ = new BinaryOp(T_geq, $1, $3); }
|   expression "==" expression                  { $$ = new BinaryOp(T_eq, $1, $3); }
|   expression "!=" expression                  { $$ = new BinaryOp(T_neq, $1, $3); }
|   expression "&&" expression                  { $$ = new BinaryOp(T_land, $1, $3); }
|   expression "||" expression                  { $$ = new BinaryOp(T_lor, $1, $3); }
|   "++" expression %prec IPLUSPLUS             { $$ = new PrefixUnAss(T_plusplus, $2); }
|   "--" expression %prec IMINUSMINUS           { $$ = new PrefixUnAss(T_minusminus, $2); }
|   expression "++" %prec PPLUSPLUS             { $$ = new PostfixUnAss(T_minusminus, $1); }
|   expression "--" %prec PMINUSMINUS           { $$ = new PostfixUnAss(T_plusplus, $1); }
|   expression '=' expression                   { $$ = new BinaryAss('=', $1, $3); }
|   expression "*=" expression                  { $$ = new BinaryAss(T_multeq, $1, $3); }
|   expression "/=" expression                  { $$ = new BinaryAss(T_diveq, $1, $3); }
|   expression "%=" expression                  { $$ = new BinaryAss(T_modeq, $1, $3); }
|   expression "+=" expression                  { $$ = new BinaryAss(T_pluseq, $1, $3); }
|   expression "-=" expression                  { $$ = new BinaryAss(T_minuseq, $1, $3); }
|   '(' type ')' expression %prec TYPECAST      { $$ = new TypeCast($2, $4); }
|   expression '?' expression ':' expression    { $$ = new TernaryOp($1, $3, $5); }
|   "new" type %prec NEW                        { $$ = new New($2);}
|   "new" type '[' expression ']' %prec NEW     { $$ = new New($2, $4); }
|   "delete" expression %prec DELETE            { $$ = new Delete($2); }
;

expression_list:
    no_comma_expression sep_by_comma_expression %prec ARGLIST { $2->_expressions.push_front($1); $$ = $2; }
;

sep_by_comma_expression:
    /* nothing */                                   { $$ = new ExpressionList; }
|   ',' no_comma_expression sep_by_comma_expression { $3->_expressions.push_front($2); $$ = $3; }
;

constant_expression:
    expression          { $$ = $1; }
;


%%