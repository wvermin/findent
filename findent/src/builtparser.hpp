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
    SIMPLEIF = 275,
    NWAYIF = 276,
    WHERE = 277,
    ENDWHERE = 278,
    FORALL = 279,
    ENDFORALL = 280,
    ELSEWHERE = 281,
    DO = 282,
    DOCOMMA = 283,
    DOWHILE = 284,
    DOCONCURRENT = 285,
    ENDDO = 286,
    SELECTCASE = 287,
    SELECTTYPE = 288,
    ENDSELECT = 289,
    CASE = 290,
    CASEDEFAULT = 291,
    TYPEIS = 292,
    CLASSIS = 293,
    CLASSDEFAULT = 294,
    SELECTRANK = 295,
    RANK = 296,
    RANKDEFAULT = 297,
    INTERFACE = 298,
    INTERFACE1 = 299,
    ABSTRACTINTERFACE = 300,
    ENDINTERFACE = 301,
    CONTAINS = 302,
    BLOCK = 303,
    ENDBLOCK = 304,
    BLOCKDATA = 305,
    ENDBLOCKDATA = 306,
    ASSOCIATE = 307,
    ENDASSOCIATE = 308,
    CRITICAL = 309,
    ENDCRITICAL = 310,
    CHANGETEAM = 311,
    ENDTEAM = 312,
    ENUM = 313,
    ENDENUM = 314,
    ASSIGNMENT = 315,
    ASSIGN = 316,
    TO = 317,
    GOTO = 318,
    GOTO_COMPUTED = 319,
    GOTO_ASSIGNED = 320,
    READ = 321,
    PRINT = 322,
    WRITE = 323,
    IO = 324,
    IOLIST = 325,
    STARTIO = 326,
    LABELIS = 327,
    OTHER = 328,
    MODULEPROCEDURE = 329,
    PROCEDURE = 330,
    ENDPROCEDURE = 331,
    TIDENTIFIER = 332,
    BLANK = 333,
    CHAR = 334,
    FINDFORMAT = 335,
    UNKNOWN = 336,
    FREE = 337,
    FIXED = 338,
    UNSURE = 339,
    PROBFREE = 340,
    FINDENTFIX = 341,
    FIXFINDENTFIX = 342,
    P_ON = 343,
    P_OFF = 344,
    SCANFIXPRE = 345,
    CPP_IF = 346,
    CPP_ENDIF = 347,
    CPP_ELSE = 348,
    CPP_ELIF = 349,
    CPP = 350,
    COCO_IF = 351,
    COCO_ENDIF = 352,
    COCO_ELSE = 353,
    COCO_ELIF = 354,
    COCO = 355,
    INCLUDE = 356,
    INCLUDE_CPP = 357,
    INCLUDE_CPP_STD = 358,
    INCLUDE_COCO = 359,
    INCFILENAME = 360,
    USE = 361,
    SEGMENT = 362,
    ENDSEGMENT = 363,
    ESOPE = 364,
    IDENTIFIER = 365,
    CIDENTIFIER = 366,
    SKIP = 367,
    SKIPALL = 368,
    SKIPNOOP = 369,
    SKIPNOS = 370,
    KEYWORD = 371,
    ELEMENTAL = 372,
    IMPURE = 373,
    PURE = 374,
    RECURSIVE = 375,
    NON_RECURSIVE = 376,
    SUBROUTINE = 377,
    FUNCTION = 378,
    PROGRAM = 379,
    EOL = 380,
    NAMED_LABEL = 381,
    STLABEL = 382,
    LABEL = 383,
    LABELS = 384,
    LABELLIST = 385,
    CALL = 386,
    STARTCALL = 387,
    CALLLIST = 388,
    TYPE = 389,
    ENDTYPE = 390,
    CLASS = 391,
    BASICTYPE = 392,
    TYPEC = 393,
    QSTRING = 394,
    HSTRING = 395,
    LR = 396,
    LRB = 397,
    DOTOPERATOR = 398,
    I_NUMBER = 399,
    UNCLASSIFIED = 400,
    ERROR = 401,
    OMP = 402,
    SCANOMPFIXED = 403,
    SCANOMPFREE = 404,
    DEF = 405,
    EXEC = 406,
    ENDDEF = 407,
    NONE = 408,
    LAST_TOKEN = 409
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
