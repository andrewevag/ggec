/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"


#include <string>
#include <vector>
#include "ast.hpp"
#include "lexer.hpp"

extern AST* syntaxTree;

#line 80 "parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_bool = 258,
    T_break = 259,
    T_byref = 260,
    T_char = 261,
    T_continue = 262,
    T_delete = 263,
    T_double = 264,
    T_else = 265,
    T_for = 266,
    T_false = 267,
    T_if = 268,
    T_int = 269,
    T_new = 270,
    T_NULL = 271,
    T_return = 272,
    T_true = 273,
    T_void = 274,
    T_id = 275,
    T_int_const = 276,
    T_double_const = 277,
    T_char_const = 278,
    T_string_const = 279,
    T_eq = 280,
    T_neq = 281,
    T_geq = 282,
    T_leq = 283,
    T_land = 284,
    T_lor = 285,
    T_plusplus = 286,
    T_minusminus = 287,
    T_pluseq = 288,
    T_minuseq = 289,
    T_multeq = 290,
    T_diveq = 291,
    T_modeq = 292,
    NO_COMMA = 293,
    EMPTY_STAR = 294,
    ARGLIST = 295,
    TYPECAST = 296,
    IPLUSPLUS = 297,
    IMINUSMINUS = 298,
    NEW = 299,
    DELETE = 300,
    ADDRESS = 301,
    DEREF = 302,
    PLUSIGN = 303,
    MINUSIGN = 304,
    NEGATION = 305,
    PPLUSPLUS = 306,
    PMINUSMINUS = 307
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 88 "parser.y"

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

#line 202 "parser.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   600

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  100
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  205

#define YYUNDEFTOK  2
#define YYMAXUTOK   307


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    72,     2,     2,     2,    51,    71,     2,
      64,    65,    49,    47,    40,    48,     2,    50,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    44,    68,
      46,    42,    45,    43,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    66,     2,    67,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    69,     2,    70,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    41,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   109,   109,   113,   114,   118,   119,   120,   124,   125,
     129,   130,   134,   138,   139,   143,   144,   145,   146,   150,
     151,   155,   156,   157,   158,   162,   166,   167,   171,   172,
     176,   177,   178,   179,   184,   185,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   203,   204,   208,
     209,   213,   214,   220,   221,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     276
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"bool\"", "\"break\"", "\"byref\"",
  "\"char\"", "\"continue\"", "\"delete\"", "\"double\"", "\"else\"",
  "\"for\"", "\"false\"", "\"if\"", "\"int\"", "\"new\"", "\"NULL\"",
  "\"return\"", "\"true\"", "\"void\"", "T_id", "T_int_const",
  "T_double_const", "T_char_const", "T_string_const", "\"==\"", "\"!=\"",
  "\">=\"", "\"<=\"", "\"&&\"", "\"||\"", "\"++\"", "\"--\"", "\"+=\"",
  "\"-=\"", "\"*=\"", "\"/=\"", "\"%=\"", "NO_COMMA", "EMPTY_STAR", "','",
  "ARGLIST", "'='", "'?'", "':'", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'",
  "'%'", "TYPECAST", "IPLUSPLUS", "IMINUSMINUS", "NEW", "DELETE",
  "ADDRESS", "DEREF", "PLUSIGN", "MINUSIGN", "NEGATION", "PPLUSPLUS",
  "PMINUSMINUS", "'('", "')'", "'['", "']'", "';'", "'{'", "'}'", "'&'",
  "'!'", "$accept", "program", "declaration_list", "declaration",
  "variable_declaration", "sep_by_comma_declarator", "type", "star_list",
  "basic_type", "declarator", "function_declaration", "parameter_list",
  "sep_by_comma_parameter", "parameter", "function_definition",
  "statement_list", "statement", "label", "expression_or_empty",
  "expression", "expression_list", "no_comma_expression",
  "constant_expression", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
      44,   295,    61,    63,    58,    62,    60,    43,    45,    42,
      47,    37,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,    40,    41,    91,    93,    59,   123,
     125,    38,    33
};
# endif

#define YYPACT_NINF (-81)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-48)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     395,   -81,   -81,   -81,   -81,    -4,    19,   -81,   -81,     8,
      -3,   -81,   -81,    10,   -81,   395,   -32,    -3,   -81,    17,
     -81,    52,    30,   368,     9,   -81,   157,   -59,    59,    25,
      70,    18,    87,   -44,    60,   368,   -81,   157,   -81,   -81,
      65,   -81,   -81,   -81,   -81,   368,   368,   368,   368,   368,
     161,   368,   368,    91,   471,    78,   -81,   131,   -81,   -81,
     -81,   -26,   138,   -81,   368,   -81,   -81,   -81,    44,   -28,
      92,   347,   -28,   -28,   -28,   -28,   -28,    94,   -35,   -28,
     -28,   368,   368,   368,   368,   368,   368,   368,   -81,   -81,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,    87,   -81,    85,   -81,   -81,
      70,    98,    85,   -81,   -81,   368,   -81,    96,   444,   368,
     -81,   471,   507,   507,   507,   507,   534,   498,   471,   471,
     471,   471,   471,   471,   298,   507,   507,    89,    89,   -28,
     -28,   -28,   232,   104,   -14,   -13,   116,   368,    71,   -81,
     223,   117,   223,   175,     5,    85,   -81,   -81,   118,    85,
     416,   -81,   368,   -28,   368,   -81,   -81,   121,   -81,   122,
     -81,   368,   123,    91,   -81,   124,   -81,   -81,   140,   -81,
     125,   -81,   135,   -81,   -81,   171,   -81,   -81,    46,   -81,
     -81,   368,   -81,   -81,   289,   139,   196,   368,   289,   143,
     -81,   368,   147,   289,   -81
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    17,    16,    18,    15,     0,     0,     3,     5,     0,
      13,     6,     7,     0,     1,     2,    10,    13,    12,     0,
       4,     0,     0,     0,     0,    14,     0,     0,     0,     0,
      26,    19,    10,     0,     0,     0,    58,     0,    59,    57,
      55,    60,    62,    61,    63,     0,     0,     0,     0,     0,
       0,     0,     0,   100,    51,     0,     8,     0,    23,     3,
      29,     0,     0,    25,     0,    11,    21,     3,     0,    99,
      97,     0,    85,    86,    69,    70,    68,     0,     0,    67,
      71,     0,     0,     0,     0,     0,     0,     0,    87,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    10,    28,    34,    24,     3,
      26,     0,    34,    22,     3,     0,    64,     0,    53,     0,
      56,    52,    81,    82,    80,    79,    83,    84,    93,    94,
      90,    91,    92,    89,     0,    78,    77,    75,    76,    72,
      73,    74,     0,     0,     0,     0,     0,    49,    55,    36,
      34,     0,    34,     0,     0,    34,    27,    20,     0,    34,
       0,    65,     0,    95,     0,    66,     9,     0,    44,     0,
      42,     0,     0,    50,    48,     0,    32,    35,     0,    37,
       0,    30,     0,    98,    54,    96,    45,    43,     0,    46,
      38,    49,    33,    31,    47,     0,    39,    49,    47,     0,
      40,    49,     0,    47,    41
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -81,   -81,   -38,   213,   -81,   -30,   100,   198,   -81,   -81,
     -81,   201,   114,   164,   -81,   -72,   -80,   -81,   -55,   -23,
      66,   -34,   165
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     6,    15,    20,     8,    24,     9,    18,    10,    32,
      11,    29,    63,    30,    12,   151,   152,   153,   172,   154,
     117,    54,    55
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      53,    69,    65,    88,    89,    81,   167,   169,    21,    58,
      59,    72,    73,    74,    75,    76,    13,    79,    80,    14,
       1,   107,    26,     2,    66,    67,     3,    78,    16,   112,
     120,     4,    22,     1,    23,    26,     2,   118,   104,     3,
     158,    53,   108,   109,     4,    81,    17,   121,   122,   123,
     124,   125,   126,   127,   168,   170,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   155,    31,   179,    19,   143,   159,    56,   175,    60,
     177,   160,    27,   180,    64,   163,    81,   182,     1,   144,
      61,     2,   145,    35,     3,    33,   -47,    36,   146,     4,
      37,    38,   147,    39,     5,   148,    41,    42,    43,    44,
      62,   194,   113,   114,   196,   174,    45,    46,   200,    28,
      88,    89,    28,   204,   173,    68,    57,    21,   118,    71,
     185,    81,    47,    48,    49,    71,   195,    70,   101,   102,
     103,     1,   199,    26,     2,   105,   202,     3,   188,    50,
      77,   106,     4,   149,   150,   104,    51,    52,   115,   119,
       1,   161,    28,     2,     1,   157,     3,     2,   173,    35,
       3,     4,   166,    36,   173,     4,    37,    38,   173,    39,
     171,    40,    41,    42,    43,    44,   178,   176,   181,   186,
     187,   189,    45,    46,   190,   192,    82,    83,    84,    85,
      86,    87,    88,    89,   191,   193,   198,   197,    47,    48,
      49,   201,   203,     7,   -48,    25,    97,    98,    99,   100,
     101,   102,   103,    34,   156,    50,   110,   144,   184,   111,
     145,    35,    51,    52,   -47,    36,   146,   104,    37,    38,
     147,    39,     0,   148,    41,    42,    43,    44,     0,     0,
       0,     0,     0,     0,    45,    46,     0,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      47,    48,    49,     0,    95,    96,     0,    97,    98,    99,
     100,   101,   102,   103,     0,     0,     0,    50,     0,     0,
       0,   149,   150,   144,    51,    52,   145,    35,   104,   165,
       0,    36,   146,     0,    37,    38,   147,    39,     0,   148,
      41,    42,    43,    44,     0,     0,     0,     0,     0,     0,
      45,    46,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    47,    48,    49,     0,
      95,    96,   164,    97,    98,    99,   100,   101,   102,   103,
       0,     0,     0,    50,     0,    35,     0,   149,   150,    36,
      51,    52,    37,    38,   104,    39,     0,    40,    41,    42,
      43,    44,     0,     0,     0,     0,    35,     0,    45,    46,
      36,     0,     0,    37,    38,     0,    39,     0,    40,    41,
      42,    43,    44,     0,    47,    48,    49,     0,     1,    45,
      46,     2,     0,     0,     3,     0,     0,     0,     0,     4,
       0,    50,   116,     0,     5,    47,    48,    49,    51,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,    51,
      52,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,    96,
       0,    97,    98,    99,   100,   101,   102,   103,     0,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,   104,   183,   162,     0,    95,    96,     0,    97,
      98,    99,   100,   101,   102,   103,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
     104,     0,     0,    95,    96,     0,    97,    98,    99,   100,
     101,   102,   103,    82,    83,    84,    85,    86,     0,    88,
      89,     0,   -48,   -48,   -48,   -48,     0,   104,    88,    89,
       0,     0,     0,    97,    98,    99,   100,   101,   102,   103,
       0,     0,   -48,   -48,    99,   100,   101,   102,   103,    82,
      83,    84,    85,     0,   104,    88,    89,     0,     0,     0,
       0,     0,     0,   104,     0,     0,     0,     0,     0,    97,
      98,    99,   100,   101,   102,   103,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104
};

static const yytype_int16 yycheck[] =
{
      23,    35,    32,    31,    32,    40,    20,    20,    40,    68,
      69,    45,    46,    47,    48,    49,    20,    51,    52,     0,
       3,    59,     5,     6,    68,    69,     9,    50,    20,    67,
      65,    14,    64,     3,    66,     5,     6,    71,    66,     9,
     112,    64,    68,    69,    14,    40,    49,    81,    82,    83,
      84,    85,    86,    87,    68,    68,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   109,    20,    68,    64,   105,   114,    68,   150,    20,
     152,   115,    65,   155,    66,   119,    40,   159,     3,     4,
      65,     6,     7,     8,     9,    65,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      40,    65,    68,    69,   194,    44,    31,    32,   198,    19,
      31,    32,    22,   203,   147,    65,    26,    40,   162,    64,
     164,    40,    47,    48,    49,    64,   191,    37,    49,    50,
      51,     3,   197,     5,     6,    67,   201,     9,   171,    64,
      50,    20,    14,    68,    69,    66,    71,    72,    66,    65,
       3,    65,    62,     6,     3,    67,     9,     6,   191,     8,
       9,    14,    68,    12,   197,    14,    15,    16,   201,    18,
      64,    20,    21,    22,    23,    24,    11,    70,    70,    68,
      68,    68,    31,    32,    70,    70,    25,    26,    27,    28,
      29,    30,    31,    32,    64,    70,    10,    68,    47,    48,
      49,    68,    65,     0,    43,    17,    45,    46,    47,    48,
      49,    50,    51,    22,   110,    64,    62,     4,   162,    64,
       7,     8,    71,    72,    11,    12,    13,    66,    15,    16,
      17,    18,    -1,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      47,    48,    49,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    68,    69,     4,    71,    72,     7,     8,    66,    67,
      -1,    12,    13,    -1,    15,    16,    17,    18,    -1,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    47,    48,    49,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      -1,    -1,    -1,    64,    -1,     8,    -1,    68,    69,    12,
      71,    72,    15,    16,    66,    18,    -1,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,     8,    -1,    31,    32,
      12,    -1,    -1,    15,    16,    -1,    18,    -1,    20,    21,
      22,    23,    24,    -1,    47,    48,    49,    -1,     3,    31,
      32,     6,    -1,    -1,     9,    -1,    -1,    -1,    -1,    14,
      -1,    64,    65,    -1,    19,    47,    48,    49,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    -1,    -1,    -1,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    -1,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    66,    67,    40,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    -1,
      66,    -1,    -1,    42,    43,    -1,    45,    46,    47,    48,
      49,    50,    51,    25,    26,    27,    28,    29,    -1,    31,
      32,    -1,    25,    26,    27,    28,    -1,    66,    31,    32,
      -1,    -1,    -1,    45,    46,    47,    48,    49,    50,    51,
      -1,    -1,    45,    46,    47,    48,    49,    50,    51,    25,
      26,    27,    28,    -1,    66,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    45,
      46,    47,    48,    49,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     6,     9,    14,    19,    74,    76,    77,    79,
      81,    83,    87,    20,     0,    75,    20,    49,    80,    64,
      76,    40,    64,    66,    78,    80,     5,    65,    79,    84,
      86,    20,    82,    65,    84,     8,    12,    15,    16,    18,
      20,    21,    22,    23,    24,    31,    32,    47,    48,    49,
      64,    71,    72,    92,    94,    95,    68,    79,    68,    69,
      20,    65,    40,    85,    66,    78,    68,    69,    65,    94,
      79,    64,    94,    94,    94,    94,    94,    79,    92,    94,
      94,    40,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    42,    43,    45,    46,    47,
      48,    49,    50,    51,    66,    67,    20,    75,    68,    69,
      86,    95,    75,    68,    69,    66,    65,    93,    94,    65,
      65,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    78,     4,     7,    13,    17,    20,    68,
      69,    88,    89,    90,    92,    75,    85,    67,    88,    75,
      94,    65,    40,    94,    44,    67,    68,    20,    68,    20,
      68,    64,    91,    92,    44,    88,    70,    88,    11,    68,
      88,    70,    88,    67,    93,    94,    68,    68,    92,    68,
      70,    64,    70,    70,    65,    91,    89,    68,    10,    91,
      89,    68,    91,    65,    89
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    73,    74,    75,    75,    76,    76,    76,    77,    77,
      78,    78,    79,    80,    80,    81,    81,    81,    81,    82,
      82,    83,    83,    83,    83,    84,    85,    85,    86,    86,
      87,    87,    87,    87,    88,    88,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    90,    90,    91,
      91,    92,    92,    93,    93,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      95
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     2,     1,     1,     1,     4,     7,
       0,     3,     2,     0,     2,     1,     1,     1,     1,     1,
       4,     5,     6,     5,     6,     2,     0,     3,     3,     2,
       8,     9,     8,     9,     0,     2,     1,     2,     3,     5,
       7,    10,     2,     3,     2,     3,     3,     0,     2,     0,
       1,     1,     3,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     4,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     4,     5,     2,     5,     2,
       1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 109 "parser.y"
                                  { (yyvsp[0].decList)->_decls.push_front((yyvsp[-1].decl)); Program *p = new Program((yyvsp[0].decList)); syntaxTree = p;}
#line 1606 "parser.cpp"
    break;

  case 3:
#line 113 "parser.y"
                                   { (yyval.decList) = new DeclarationList(); }
#line 1612 "parser.cpp"
    break;

  case 4:
#line 114 "parser.y"
                                   { (yyvsp[-1].decList)->_decls.push_back((yyvsp[0].decl)); (yyval.decList) = (yyvsp[-1].decList); }
#line 1618 "parser.cpp"
    break;

  case 5:
#line 118 "parser.y"
                                   { (yyval.decl) = (yyvsp[0].decl); }
#line 1624 "parser.cpp"
    break;

  case 6:
#line 119 "parser.y"
                                   { (yyval.decl) = (yyvsp[0].decl); }
#line 1630 "parser.cpp"
    break;

  case 7:
#line 120 "parser.y"
                                   { (yyval.decl) = (yyvsp[0].decl); }
#line 1636 "parser.cpp"
    break;

  case 8:
#line 124 "parser.y"
                                                                        { for(auto &i : (yyvsp[-1].decList)->_decls) i->embedType((yyvsp[-3].typeExpr)->copy()); (yyvsp[-1].decList)->_decls.push_front(new VariableDeclaration((yyvsp[-3].typeExpr), (yyvsp[-2].str)));; (yyval.decl) = (yyvsp[-1].decList); delete[] (yyvsp[-2].str); }
#line 1642 "parser.cpp"
    break;

  case 9:
#line 125 "parser.y"
                                                                        { for(auto &i : (yyvsp[-1].decList)->_decls) i->embedType((yyvsp[-6].typeExpr)->copy()); (yyvsp[-1].decList)->_decls.push_front(new ArrayDeclaration((yyvsp[-6].typeExpr), (yyvsp[-5].str), (yyvsp[-3].expr))); (yyval.decl) = (yyvsp[-1].decList); delete[] (yyvsp[-5].str); }
#line 1648 "parser.cpp"
    break;

  case 10:
#line 129 "parser.y"
                                            { (yyval.decList) = new DeclarationList(); }
#line 1654 "parser.cpp"
    break;

  case 11:
#line 130 "parser.y"
                                            { (yyvsp[0].decList)->_decls.push_front((yyvsp[-1].decl)); (yyval.decList) = (yyvsp[0].decList); }
#line 1660 "parser.cpp"
    break;

  case 12:
#line 134 "parser.y"
                         { (yyvsp[0].typeExpr)->penetrate((yyvsp[-1].typeExpr)); delete (yyvsp[-1].typeExpr); (yyval.typeExpr) = (yyvsp[0].typeExpr); }
#line 1666 "parser.cpp"
    break;

  case 13:
#line 138 "parser.y"
                                   { (yyval.typeExpr) = new BasicType(""); }
#line 1672 "parser.cpp"
    break;

  case 14:
#line 139 "parser.y"
                  { (yyval.typeExpr) = new Pointer((yyvsp[0].typeExpr)); }
#line 1678 "parser.cpp"
    break;

  case 15:
#line 143 "parser.y"
                { (yyval.typeExpr) = new BasicType("int");  }
#line 1684 "parser.cpp"
    break;

  case 16:
#line 144 "parser.y"
                { (yyval.typeExpr) = new BasicType("char"); }
#line 1690 "parser.cpp"
    break;

  case 17:
#line 145 "parser.y"
                { (yyval.typeExpr) = new BasicType("bool"); }
#line 1696 "parser.cpp"
    break;

  case 18:
#line 146 "parser.y"
                { (yyval.typeExpr) = new BasicType("double"); }
#line 1702 "parser.cpp"
    break;

  case 19:
#line 150 "parser.y"
                                        { (yyval.decl) = new VariableDeclaration(nullptr, (yyvsp[0].str)); delete[] (yyvsp[0].str);}
#line 1708 "parser.cpp"
    break;

  case 20:
#line 151 "parser.y"
                                        { (yyval.decl) = new ArrayDeclaration(nullptr, (yyvsp[-3].str), (yyvsp[-1].expr)); delete[] (yyvsp[-3].str); }
#line 1714 "parser.cpp"
    break;

  case 21:
#line 155 "parser.y"
                                            { (yyval.decl) =  new FunctionDeclaration((yyvsp[-4].typeExpr), (yyvsp[-3].str), new ParameterList()); delete[] (yyvsp[-3].str); }
#line 1720 "parser.cpp"
    break;

  case 22:
#line 156 "parser.y"
                                            { (yyval.decl) = new FunctionDeclaration((yyvsp[-5].typeExpr), (yyvsp[-4].str), (yyvsp[-2].parList)); delete[] (yyvsp[-4].str); }
#line 1726 "parser.cpp"
    break;

  case 23:
#line 157 "parser.y"
                                            { (yyval.decl) = new FunctionDeclaration(new BasicType("void"), (yyvsp[-3].str), new ParameterList()); delete[] (yyvsp[-3].str);}
#line 1732 "parser.cpp"
    break;

  case 24:
#line 158 "parser.y"
                                            { (yyval.decl) = new FunctionDeclaration(new BasicType("void"), (yyvsp[-4].str), (yyvsp[-2].parList)); delete[] (yyvsp[-4].str); }
#line 1738 "parser.cpp"
    break;

  case 25:
#line 162 "parser.y"
                                            { (yyvsp[0].parList)->_parameters.push_front((yyvsp[-1].par)); (yyval.parList) = (yyvsp[0].parList); }
#line 1744 "parser.cpp"
    break;

  case 26:
#line 166 "parser.y"
                                             { (yyval.parList) = new ParameterList(); }
#line 1750 "parser.cpp"
    break;

  case 27:
#line 167 "parser.y"
                                             { (yyvsp[0].parList)->_parameters.push_front((yyvsp[-1].par)); (yyval.parList) = (yyvsp[0].parList);}
#line 1756 "parser.cpp"
    break;

  case 28:
#line 171 "parser.y"
                            { (yyval.par) = new Parameter(Parameter::ByRef, (yyvsp[-1].typeExpr), (yyvsp[0].str)); delete[] (yyvsp[0].str);}
#line 1762 "parser.cpp"
    break;

  case 29:
#line 172 "parser.y"
                            { (yyval.par) = new Parameter(Parameter::ByCall, (yyvsp[-1].typeExpr), (yyvsp[0].str)); delete[] (yyvsp[0].str); }
#line 1768 "parser.cpp"
    break;

  case 30:
#line 176 "parser.y"
                                                                                { (yyval.decl) = new FunctionDefinition((yyvsp[-7].typeExpr), (yyvsp[-6].str), new ParameterList(), (yyvsp[-2].decList), (yyvsp[-1].stmtList)); delete[] (yyvsp[-6].str);}
#line 1774 "parser.cpp"
    break;

  case 31:
#line 177 "parser.y"
                                                                                { (yyval.decl) = new FunctionDefinition((yyvsp[-8].typeExpr), (yyvsp[-7].str), (yyvsp[-5].parList), (yyvsp[-2].decList), (yyvsp[-1].stmtList)); delete[] (yyvsp[-7].str);}
#line 1780 "parser.cpp"
    break;

  case 32:
#line 178 "parser.y"
                                                                                { (yyval.decl) = new FunctionDefinition(new BasicType("void"), (yyvsp[-6].str), new ParameterList(), (yyvsp[-2].decList), (yyvsp[-1].stmtList)); delete[] (yyvsp[-6].str);}
#line 1786 "parser.cpp"
    break;

  case 33:
#line 179 "parser.y"
                                                                                { (yyval.decl) = new FunctionDefinition(new BasicType("void"), (yyvsp[-7].str), (yyvsp[-5].parList), (yyvsp[-2].decList), (yyvsp[-1].stmtList)); delete[] (yyvsp[-7].str); }
#line 1792 "parser.cpp"
    break;

  case 34:
#line 184 "parser.y"
                                                { (yyval.stmtList) = new StatementList(); }
#line 1798 "parser.cpp"
    break;

  case 35:
#line 185 "parser.y"
                                                { (yyvsp[0].stmtList)->_stmts.push_front((yyvsp[-1].stmt)); (yyval.stmtList) = (yyvsp[0].stmtList); }
#line 1804 "parser.cpp"
    break;

  case 36:
#line 189 "parser.y"
                                                { (yyval.stmt) = new EmptyStatement(); }
#line 1810 "parser.cpp"
    break;

  case 37:
#line 190 "parser.y"
                                                { (yyval.stmt) = new SingleExpression((yyvsp[-1].expr)); }
#line 1816 "parser.cpp"
    break;

  case 38:
#line 191 "parser.y"
                                                { (yyval.stmt) = (yyvsp[-1].stmtList); }
#line 1822 "parser.cpp"
    break;

  case 39:
#line 192 "parser.y"
                                                        { (yyval.stmt) = new IfStatement((yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 1828 "parser.cpp"
    break;

  case 40:
#line 193 "parser.y"
                                                        { (yyval.stmt) = new IfElseStatement((yyvsp[-4].expr), (yyvsp[-2].stmt), (yyvsp[0].stmt)); }
#line 1834 "parser.cpp"
    break;

  case 41:
#line 194 "parser.y"
                                                                                                      { (yyval.stmt) = new ForStatement((yyvsp[-9].lbl), (yyvsp[-6].expr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 1840 "parser.cpp"
    break;

  case 42:
#line 195 "parser.y"
                                                { (yyval.stmt) = new ContinueStatement(); }
#line 1846 "parser.cpp"
    break;

  case 43:
#line 196 "parser.y"
                                                { (yyval.stmt) = new ContinueStatement((yyvsp[-1].str)); delete[] (yyvsp[-1].str);}
#line 1852 "parser.cpp"
    break;

  case 44:
#line 197 "parser.y"
                                                { (yyval.stmt) = new BreakStatement(); }
#line 1858 "parser.cpp"
    break;

  case 45:
#line 198 "parser.y"
                                                { (yyval.stmt) = new BreakStatement((yyvsp[-1].str)); delete[] (yyvsp[-1].str); }
#line 1864 "parser.cpp"
    break;

  case 46:
#line 199 "parser.y"
                                                { (yyval.stmt) = new ReturnStatement((yyvsp[-1].expr)); }
#line 1870 "parser.cpp"
    break;

  case 47:
#line 203 "parser.y"
                                                { (yyval.lbl) = nullptr; }
#line 1876 "parser.cpp"
    break;

  case 48:
#line 204 "parser.y"
                                                { (yyval.lbl) = new Label((yyvsp[-1].str)); delete[] (yyvsp[-1].str); }
#line 1882 "parser.cpp"
    break;

  case 49:
#line 208 "parser.y"
                                                { (yyval.expr) = nullptr; }
#line 1888 "parser.cpp"
    break;

  case 50:
#line 209 "parser.y"
                                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1894 "parser.cpp"
    break;

  case 51:
#line 213 "parser.y"
                                                           { (yyval.expr) = (yyvsp[0].expr); }
#line 1900 "parser.cpp"
    break;

  case 52:
#line 214 "parser.y"
                                             { (yyval.expr) = new CommaExpr((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1906 "parser.cpp"
    break;

  case 53:
#line 220 "parser.y"
                                                          { (yyval.exprList) = new ExpressionList({(yyvsp[0].expr)}); }
#line 1912 "parser.cpp"
    break;

  case 54:
#line 221 "parser.y"
                                                          { (yyvsp[0].exprList)->_expressions.push_front((yyvsp[-2].expr)); (yyval.exprList) = (yyvsp[0].exprList); }
#line 1918 "parser.cpp"
    break;

  case 55:
#line 226 "parser.y"
                                                { (yyval.expr) = new Id((yyvsp[0].str)); delete[] (yyvsp[0].str); }
#line 1924 "parser.cpp"
    break;

  case 56:
#line 227 "parser.y"
                                                { (yyval.expr) = (yyvsp[-1].expr); }
#line 1930 "parser.cpp"
    break;

  case 57:
#line 228 "parser.y"
                                                { (yyval.expr) = new Constant(true); }
#line 1936 "parser.cpp"
    break;

  case 58:
#line 229 "parser.y"
                                                { (yyval.expr) = new Constant(false); }
#line 1942 "parser.cpp"
    break;

  case 59:
#line 230 "parser.y"
                                                { (yyval.expr) = new Constant(); }
#line 1948 "parser.cpp"
    break;

  case 60:
#line 231 "parser.y"
                                                { (yyval.expr) = new Constant((int16_t)(yyvsp[0].i)); }
#line 1954 "parser.cpp"
    break;

  case 61:
#line 232 "parser.y"
                                                { (yyval.expr) = new Constant((char)(yyvsp[0].c)); }
#line 1960 "parser.cpp"
    break;

  case 62:
#line 233 "parser.y"
                                                { (yyval.expr) = new Constant((long double)(yyvsp[0].d)); }
#line 1966 "parser.cpp"
    break;

  case 63:
#line 234 "parser.y"
                                                { (yyval.expr) = new Constant((std::string)(yyvsp[0].str)); delete[] (yyvsp[0].str); }
#line 1972 "parser.cpp"
    break;

  case 64:
#line 235 "parser.y"
                                                { (yyval.expr) = new FunctionCall((yyvsp[-2].str), new ExpressionList()); delete[] (yyvsp[-2].str); }
#line 1978 "parser.cpp"
    break;

  case 65:
#line 236 "parser.y"
                                                { (yyval.expr) = new FunctionCall((yyvsp[-3].str), (yyvsp[-1].exprList)); delete[] (yyvsp[-3].str); }
#line 1984 "parser.cpp"
    break;

  case 66:
#line 237 "parser.y"
                                                                  { (yyval.expr) = new BracketedIndex((yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1990 "parser.cpp"
    break;

  case 67:
#line 238 "parser.y"
                                                         { (yyval.expr) = new UnaryOp(UnaryOp::UnaryOpType::ADDRESS, (yyvsp[0].expr)); }
#line 1996 "parser.cpp"
    break;

  case 68:
#line 239 "parser.y"
                                                         { (yyval.expr) = new UnaryOp(UnaryOp::UnaryOpType::DEREF, (yyvsp[0].expr)); }
#line 2002 "parser.cpp"
    break;

  case 69:
#line 240 "parser.y"
                                                         { (yyval.expr) = new UnaryOp(UnaryOp::UnaryOpType::POS, (yyvsp[0].expr)); }
#line 2008 "parser.cpp"
    break;

  case 70:
#line 241 "parser.y"
                                                         { (yyval.expr) = new UnaryOp(UnaryOp::UnaryOpType::NEG, (yyvsp[0].expr)); }
#line 2014 "parser.cpp"
    break;

  case 71:
#line 242 "parser.y"
                                                         { (yyval.expr) = new UnaryOp(UnaryOp::UnaryOpType::NOT, (yyvsp[0].expr)); }
#line 2020 "parser.cpp"
    break;

  case 72:
#line 243 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::MULT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2026 "parser.cpp"
    break;

  case 73:
#line 244 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2032 "parser.cpp"
    break;

  case 74:
#line 245 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::MOD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2038 "parser.cpp"
    break;

  case 75:
#line 246 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::PLUS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2044 "parser.cpp"
    break;

  case 76:
#line 247 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::MINUS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2050 "parser.cpp"
    break;

  case 77:
#line 248 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::LESS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2056 "parser.cpp"
    break;

  case 78:
#line 249 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::GREATER, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2062 "parser.cpp"
    break;

  case 79:
#line 250 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::LESSEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2068 "parser.cpp"
    break;

  case 80:
#line 251 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::GREATEREQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2074 "parser.cpp"
    break;

  case 81:
#line 252 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::EQUALS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2080 "parser.cpp"
    break;

  case 82:
#line 253 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::NOTEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2086 "parser.cpp"
    break;

  case 83:
#line 254 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::LAND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2092 "parser.cpp"
    break;

  case 84:
#line 255 "parser.y"
                                                                  { (yyval.expr) = new BinaryOp(BinaryOp::BinaryOpType::LOR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2098 "parser.cpp"
    break;

  case 85:
#line 256 "parser.y"
                                                         { (yyval.expr) = new PrefixUnAss(UnAss::UnAssType::PLUSPLUS, (yyvsp[0].expr)); }
#line 2104 "parser.cpp"
    break;

  case 86:
#line 257 "parser.y"
                                                         { (yyval.expr) = new PrefixUnAss(UnAss::UnAssType::MINUSMINUS, (yyvsp[0].expr)); }
#line 2110 "parser.cpp"
    break;

  case 87:
#line 258 "parser.y"
                                                         { (yyval.expr) = new PostfixUnAss(UnAss::UnAssType::PLUSPLUS, (yyvsp[-1].expr)); }
#line 2116 "parser.cpp"
    break;

  case 88:
#line 259 "parser.y"
                                                         { (yyval.expr) = new PostfixUnAss(UnAss::UnAssType::MINUSMINUS, (yyvsp[-1].expr)); }
#line 2122 "parser.cpp"
    break;

  case 89:
#line 260 "parser.y"
                                                                  { (yyval.expr) = new BinaryAss(BinaryAss::BinaryAssType::ASS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2128 "parser.cpp"
    break;

  case 90:
#line 261 "parser.y"
                                                                  { (yyval.expr) = new BinaryAss(BinaryAss::BinaryAssType::MULTASS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2134 "parser.cpp"
    break;

  case 91:
#line 262 "parser.y"
                                                                  { (yyval.expr) = new BinaryAss(BinaryAss::BinaryAssType::DIVASS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2140 "parser.cpp"
    break;

  case 92:
#line 263 "parser.y"
                                                                  { (yyval.expr) = new BinaryAss(BinaryAss::BinaryAssType::MODASS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2146 "parser.cpp"
    break;

  case 93:
#line 264 "parser.y"
                                                                  { (yyval.expr) = new BinaryAss(BinaryAss::BinaryAssType::PLUSASS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2152 "parser.cpp"
    break;

  case 94:
#line 265 "parser.y"
                                                                  { (yyval.expr) = new BinaryAss(BinaryAss::BinaryAssType::MINUSASS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2158 "parser.cpp"
    break;

  case 95:
#line 266 "parser.y"
                                                         { (yyval.expr) = new TypeCast((yyvsp[-2].typeExpr), (yyvsp[0].expr)); }
#line 2164 "parser.cpp"
    break;

  case 96:
#line 267 "parser.y"
                                                                           { (yyval.expr) = new TernaryOp((yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2170 "parser.cpp"
    break;

  case 97:
#line 268 "parser.y"
                                                { (yyval.expr) = new New((yyvsp[0].typeExpr));}
#line 2176 "parser.cpp"
    break;

  case 98:
#line 269 "parser.y"
                                                         { (yyval.expr) = new New((yyvsp[-3].typeExpr), (yyvsp[-1].expr)); }
#line 2182 "parser.cpp"
    break;

  case 99:
#line 270 "parser.y"
                                                         { (yyval.expr) = new Delete((yyvsp[0].expr)); }
#line 2188 "parser.cpp"
    break;

  case 100:
#line 276 "parser.y"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 2194 "parser.cpp"
    break;


#line 2198 "parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 280 "parser.y"
