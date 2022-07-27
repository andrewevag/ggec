/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
