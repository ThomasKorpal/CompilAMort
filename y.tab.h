/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOK_VOID = 258,                /* TOK_VOID  */
    TOK_INT = 259,                 /* TOK_INT  */
    TOK_INTVAL = 260,              /* TOK_INTVAL  */
    TOK_BOOL = 261,                /* TOK_BOOL  */
    TOK_TRUE = 262,                /* TOK_TRUE  */
    TOK_FALSE = 263,               /* TOK_FALSE  */
    TOK_IDENT = 264,               /* TOK_IDENT  */
    TOK_IF = 265,                  /* TOK_IF  */
    TOK_ELSE = 266,                /* TOK_ELSE  */
    TOK_WHILE = 267,               /* TOK_WHILE  */
    TOK_FOR = 268,                 /* TOK_FOR  */
    TOK_PRINT = 269,               /* TOK_PRINT  */
    TOK_SEMICOL = 270,             /* TOK_SEMICOL  */
    TOK_COMMA = 271,               /* TOK_COMMA  */
    TOK_LPAR = 272,                /* TOK_LPAR  */
    TOK_RPAR = 273,                /* TOK_RPAR  */
    TOK_LACC = 274,                /* TOK_LACC  */
    TOK_RACC = 275,                /* TOK_RACC  */
    TOK_STRING = 276,              /* TOK_STRING  */
    TOK_DO = 277,                  /* TOK_DO  */
    TOK_THEN = 278,                /* TOK_THEN  */
    TOK_AFFECT = 279,              /* TOK_AFFECT  */
    TOK_OR = 280,                  /* TOK_OR  */
    TOK_AND = 281,                 /* TOK_AND  */
    TOK_BOR = 282,                 /* TOK_BOR  */
    TOK_BXOR = 283,                /* TOK_BXOR  */
    TOK_BAND = 284,                /* TOK_BAND  */
    TOK_EQ = 285,                  /* TOK_EQ  */
    TOK_NE = 286,                  /* TOK_NE  */
    TOK_GT = 287,                  /* TOK_GT  */
    TOK_LT = 288,                  /* TOK_LT  */
    TOK_GE = 289,                  /* TOK_GE  */
    TOK_LE = 290,                  /* TOK_LE  */
    TOK_SRL = 291,                 /* TOK_SRL  */
    TOK_SRA = 292,                 /* TOK_SRA  */
    TOK_SLL = 293,                 /* TOK_SLL  */
    TOK_PLUS = 294,                /* TOK_PLUS  */
    TOK_MINUS = 295,               /* TOK_MINUS  */
    TOK_MUL = 296,                 /* TOK_MUL  */
    TOK_DIV = 297,                 /* TOK_DIV  */
    TOK_MOD = 298,                 /* TOK_MOD  */
    TOK_UMINUS = 299,              /* TOK_UMINUS  */
    TOK_NOT = 300,                 /* TOK_NOT  */
    TOK_BNOT = 301                 /* TOK_BNOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define TOK_VOID 258
#define TOK_INT 259
#define TOK_INTVAL 260
#define TOK_BOOL 261
#define TOK_TRUE 262
#define TOK_FALSE 263
#define TOK_IDENT 264
#define TOK_IF 265
#define TOK_ELSE 266
#define TOK_WHILE 267
#define TOK_FOR 268
#define TOK_PRINT 269
#define TOK_SEMICOL 270
#define TOK_COMMA 271
#define TOK_LPAR 272
#define TOK_RPAR 273
#define TOK_LACC 274
#define TOK_RACC 275
#define TOK_STRING 276
#define TOK_DO 277
#define TOK_THEN 278
#define TOK_AFFECT 279
#define TOK_OR 280
#define TOK_AND 281
#define TOK_BOR 282
#define TOK_BXOR 283
#define TOK_BAND 284
#define TOK_EQ 285
#define TOK_NE 286
#define TOK_GT 287
#define TOK_LT 288
#define TOK_GE 289
#define TOK_LE 290
#define TOK_SRL 291
#define TOK_SRA 292
#define TOK_SLL 293
#define TOK_PLUS 294
#define TOK_MINUS 295
#define TOK_MUL 296
#define TOK_DIV 297
#define TOK_MOD 298
#define TOK_UMINUS 299
#define TOK_NOT 300
#define TOK_BNOT 301

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 45 "grammar.y"

    int32_t intval;
    char * strval;
    node_t ptr;

#line 165 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (node_t * program_root);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
