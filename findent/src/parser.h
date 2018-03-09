/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
    MODULE = 258,
    SUBFUN = 259,
    ENTRY = 260,
    SUBMODULE = 261,
    MODULESUBROUTINE = 262,
    MODULEFUNCTION = 263,
    END = 264,
    ENDSUBROUTINE = 265,
    ENDFUNCTION = 266,
    ENDPROGRAM = 267,
    ENDMODULE = 268,
    ENDSUBMODULE = 269,
    IF = 270,
    THEN = 271,
    ELSE = 272,
    ELSEIF = 273,
    ENDIF = 274,
    WHERE = 275,
    ENDWHERE = 276,
    FORALL = 277,
    ENDFORALL = 278,
    ELSEWHERE = 279,
    DO = 280,
    DOWHILE = 281,
    DOCONCURRENT = 282,
    ENDDO = 283,
    SELECTCASE = 284,
    SELECTTYPE = 285,
    ENDSELECT = 286,
    CASE = 287,
    CASEDEFAULT = 288,
    TYPEIS = 289,
    CLASSIS = 290,
    CLASSDEFAULT = 291,
    INTERFACE = 292,
    ABSTRACTINTERFACE = 293,
    ENDINTERFACE = 294,
    CONTAINS = 295,
    BLOCK = 296,
    ENDBLOCK = 297,
    BLOCKDATA = 298,
    ENDBLOCKDATA = 299,
    ASSOCIATE = 300,
    ENDASSOCIATE = 301,
    CRITICAL = 302,
    ENDCRITICAL = 303,
    ENUM = 304,
    ENDENUM = 305,
    ASSIGNMENT = 306,
    ASSIGN = 307,
    TO = 308,
    MODULEPROCEDURE = 309,
    PROCEDURE = 310,
    ENDPROCEDURE = 311,
    TIDENTIFIER = 312,
    BLANK = 313,
    FINDFORMAT = 314,
    FREE = 315,
    FIXED = 316,
    UNSURE = 317,
    PROBFREE = 318,
    IDENTIFIER = 319,
    SKIP = 320,
    SKIPALL = 321,
    SKIPNOOP = 322,
    KEYWORD = 323,
    ELEMENTAL = 324,
    IMPURE = 325,
    PURE = 326,
    RECURSIVE = 327,
    SUBROUTINE = 328,
    FUNCTION = 329,
    PROGRAM = 330,
    EOL = 331,
    NAMED_LABEL = 332,
    STLABEL = 333,
    TYPE = 334,
    ENDTYPE = 335,
    CLASS = 336,
    BASICTYPE = 337,
    TYPEC = 338,
    QSTRING = 339,
    HSTRING = 340,
    LR = 341,
    LRB = 342,
    DOTOPERATOR = 343,
    I_NUMBER = 344,
    UNCLASSIFIED = 345,
    ERROR = 346,
    LAST_TOKEN = 347
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
