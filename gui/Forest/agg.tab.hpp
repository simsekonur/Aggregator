/* A Bison parser, made by GNU Bison 3.7.4.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_AGG_TAB_HPP_INCLUDED
# define YY_YY_AGG_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 16 "agg.yy"

	typedef void* yyscan_t;
	#include <vector>
    #include "AggParser.hpp"

#line 55 "agg.tab.hpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_FLOAT = 258,                 /* T_FLOAT  */
    T_STRING = 259,                /* T_STRING  */
    T_COMPARE = 260,               /* T_COMPARE  */
    T_PLUS = 261,                  /* T_PLUS  */
    T_MINUS = 262,                 /* T_MINUS  */
    T_MULTIPLY = 263,              /* T_MULTIPLY  */
    T_DIVIDE = 264,                /* T_DIVIDE  */
    T_LEFT = 265,                  /* T_LEFT  */
    T_RIGHT = 266,                 /* T_RIGHT  */
    T_POWER = 267,                 /* T_POWER  */
    T_LOG = 268,                   /* T_LOG  */
    T_SQRT = 269,                  /* T_SQRT  */
    T_COMMA = 270,                 /* T_COMMA  */
    T_SEMICOLON = 271              /* T_SEMICOLON  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 27 "agg.yy"

	double fval;
	char* sval;
	AggExp* expval;
    std::vector<AggExp*>* vectorval;
	Condition* condval;

#line 96 "agg.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (yyscan_t scanner, AggParser& aggParser);

#endif /* !YY_YY_AGG_TAB_HPP_INCLUDED  */
