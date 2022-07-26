

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
%token T_id            
%token T_int_const     
%token T_double_const  
%token T_char_const    
%token T_string_const  
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
    type declarator sep_by_comma_declarator ';'
;

sep_by_comma_declarator:
    /* nothing */
|   ',' declarator sep_by_comma_declarator
;

type:
    basic_type star_list   
;

star_list:
    /* nothing */
|   '*' star_list    
;

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

// thelei factoring
function_declaration:
    result_type T_id '(' ')' ';'
|   result_type T_id '(' parameter_list ')' ';'
;

result_type:
    type
|   "void"
;

parameter_list:
    parameter sep_by_comma_parameter
;

sep_by_comma_parameter:
    /* nothing */
|   ',' parameter sep_by_comma_parameter
;

parameter:
    "byref" type T_id
|   type T_id
;

function_definition:
    result_type T_id '(' ')' '{' declaration_list statement_list '}'
|   result_type T_id '(' parameter_list ')' '{' declaration_list statement_list '}'
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
|   '&' expression
|   '*' expression
|   '+' expression
|   '-' expression
|   '!' expression
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
|   expression ',' expression
|   "++" expression
|   "--" expression
|   expression "++"
|   expression "--"
|   expression '=' expression
|   expression "*=" expression
|   expression "/=" expression
|   expression "%=" expression
|   expression "+=" expression
|   expression "-=" expression
|   '(' type ')' expression
|   expression '?' expression ':' expression
|   "new" type
|   "new" type '[' expression ']'
|   "delete" expression
;

expression_list:
    expression sep_by_comma_expression
;

sep_by_comma_expression:
    /* nothing */
|   ',' expression sep_by_comma_expression
;

constant_expression:
    expression
;


%%