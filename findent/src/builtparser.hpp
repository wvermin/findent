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

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
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
    MODULE = 258,                  /* MODULE  */
    SUBFUN = 259,                  /* SUBFUN  */
    ENTRY = 260,                   /* ENTRY  */
    SUBMODULE = 261,               /* SUBMODULE  */
    MODULESUBROUTINE = 262,        /* MODULESUBROUTINE  */
    MODULEFUNCTION = 263,          /* MODULEFUNCTION  */
    END = 264,                     /* END  */
    ENDSUBROUTINE = 265,           /* ENDSUBROUTINE  */
    ENDFUNCTION = 266,             /* ENDFUNCTION  */
    ENDPROGRAM = 267,              /* ENDPROGRAM  */
    ENDMODULE = 268,               /* ENDMODULE  */
    ENDSUBMODULE = 269,            /* ENDSUBMODULE  */
    IF = 270,                      /* IF  */
    THEN = 271,                    /* THEN  */
    ELSE = 272,                    /* ELSE  */
    ELSEIF = 273,                  /* ELSEIF  */
    ENDIF = 274,                   /* ENDIF  */
    SIMPLEIF = 275,                /* SIMPLEIF  */
    NWAYIF = 276,                  /* NWAYIF  */
    WHERE = 277,                   /* WHERE  */
    ENDWHERE = 278,                /* ENDWHERE  */
    FORALL = 279,                  /* FORALL  */
    ENDFORALL = 280,               /* ENDFORALL  */
    ELSEWHERE = 281,               /* ELSEWHERE  */
    DO = 282,                      /* DO  */
    DOCOMMA = 283,                 /* DOCOMMA  */
    DOWHILE = 284,                 /* DOWHILE  */
    DOCONCURRENT = 285,            /* DOCONCURRENT  */
    ENDDO = 286,                   /* ENDDO  */
    SELECTCASE = 287,              /* SELECTCASE  */
    SELECTTYPE = 288,              /* SELECTTYPE  */
    ENDSELECT = 289,               /* ENDSELECT  */
    CASE = 290,                    /* CASE  */
    CASEDEFAULT = 291,             /* CASEDEFAULT  */
    TYPEIS = 292,                  /* TYPEIS  */
    CLASSIS = 293,                 /* CLASSIS  */
    CLASSDEFAULT = 294,            /* CLASSDEFAULT  */
    SELECTRANK = 295,              /* SELECTRANK  */
    RANK = 296,                    /* RANK  */
    RANKDEFAULT = 297,             /* RANKDEFAULT  */
    INTERFACE = 298,               /* INTERFACE  */
    INTERFACE1 = 299,              /* INTERFACE1  */
    ABSTRACTINTERFACE = 300,       /* ABSTRACTINTERFACE  */
    ENDINTERFACE = 301,            /* ENDINTERFACE  */
    CONTAINS = 302,                /* CONTAINS  */
    BLOCK = 303,                   /* BLOCK  */
    ENDBLOCK = 304,                /* ENDBLOCK  */
    BLOCKDATA = 305,               /* BLOCKDATA  */
    ENDBLOCKDATA = 306,            /* ENDBLOCKDATA  */
    ASSOCIATE = 307,               /* ASSOCIATE  */
    ENDASSOCIATE = 308,            /* ENDASSOCIATE  */
    CRITICAL = 309,                /* CRITICAL  */
    ENDCRITICAL = 310,             /* ENDCRITICAL  */
    CHANGETEAM = 311,              /* CHANGETEAM  */
    ENDTEAM = 312,                 /* ENDTEAM  */
    ENUM = 313,                    /* ENUM  */
    ENDENUM = 314,                 /* ENDENUM  */
    ASSIGNMENT = 315,              /* ASSIGNMENT  */
    ASSIGN = 316,                  /* ASSIGN  */
    TO = 317,                      /* TO  */
    GOTO = 318,                    /* GOTO  */
    GOTO_COMPUTED = 319,           /* GOTO_COMPUTED  */
    GOTO_ASSIGNED = 320,           /* GOTO_ASSIGNED  */
    READ = 321,                    /* READ  */
    PRINT = 322,                   /* PRINT  */
    WRITE = 323,                   /* WRITE  */
    IO = 324,                      /* IO  */
    IOLIST = 325,                  /* IOLIST  */
    STARTIO = 326,                 /* STARTIO  */
    LABELIS = 327,                 /* LABELIS  */
    OTHER = 328,                   /* OTHER  */
    MODULEPROCEDURE = 329,         /* MODULEPROCEDURE  */
    PROCEDURE = 330,               /* PROCEDURE  */
    ENDPROCEDURE = 331,            /* ENDPROCEDURE  */
    TIDENTIFIER = 332,             /* TIDENTIFIER  */
    BLANK = 333,                   /* BLANK  */
    CHAR = 334,                    /* CHAR  */
    FINDFORMAT = 335,              /* FINDFORMAT  */
    UNKNOWN = 336,                 /* UNKNOWN  */
    FREE = 337,                    /* FREE  */
    FIXED = 338,                   /* FIXED  */
    UNSURE = 339,                  /* UNSURE  */
    PROBFREE = 340,                /* PROBFREE  */
    FINDENTFIX = 341,              /* FINDENTFIX  */
    FIXFINDENTFIX = 342,           /* FIXFINDENTFIX  */
    P_ON = 343,                    /* P_ON  */
    P_OFF = 344,                   /* P_OFF  */
    SCANFIXPRE = 345,              /* SCANFIXPRE  */
    CPP_IF = 346,                  /* CPP_IF  */
    CPP_ENDIF = 347,               /* CPP_ENDIF  */
    CPP_ELSE = 348,                /* CPP_ELSE  */
    CPP_ELIF = 349,                /* CPP_ELIF  */
    CPP = 350,                     /* CPP  */
    COCO_IF = 351,                 /* COCO_IF  */
    COCO_ENDIF = 352,              /* COCO_ENDIF  */
    COCO_ELSE = 353,               /* COCO_ELSE  */
    COCO_ELIF = 354,               /* COCO_ELIF  */
    COCO = 355,                    /* COCO  */
    INCLUDE = 356,                 /* INCLUDE  */
    INCLUDE_CPP = 357,             /* INCLUDE_CPP  */
    INCLUDE_CPP_STD = 358,         /* INCLUDE_CPP_STD  */
    INCLUDE_COCO = 359,            /* INCLUDE_COCO  */
    INCFILENAME = 360,             /* INCFILENAME  */
    USE = 361,                     /* USE  */
    SEGMENT = 362,                 /* SEGMENT  */
    ENDSEGMENT = 363,              /* ENDSEGMENT  */
    ESOPE = 364,                   /* ESOPE  */
    IDENTIFIER = 365,              /* IDENTIFIER  */
    CIDENTIFIER = 366,             /* CIDENTIFIER  */
    SKIP = 367,                    /* SKIP  */
    SKIPALL = 368,                 /* SKIPALL  */
    SKIPNOOP = 369,                /* SKIPNOOP  */
    SKIPNOS = 370,                 /* SKIPNOS  */
    KEYWORD = 371,                 /* KEYWORD  */
    ELEMENTAL = 372,               /* ELEMENTAL  */
    IMPURE = 373,                  /* IMPURE  */
    PURE = 374,                    /* PURE  */
    RECURSIVE = 375,               /* RECURSIVE  */
    NON_RECURSIVE = 376,           /* NON_RECURSIVE  */
    SUBROUTINE = 377,              /* SUBROUTINE  */
    FUNCTION = 378,                /* FUNCTION  */
    PROGRAM = 379,                 /* PROGRAM  */
    EOL = 380,                     /* EOL  */
    NAMED_LABEL = 381,             /* NAMED_LABEL  */
    STLABEL = 382,                 /* STLABEL  */
    LABEL = 383,                   /* LABEL  */
    LABELS = 384,                  /* LABELS  */
    LABELLIST = 385,               /* LABELLIST  */
    CALL = 386,                    /* CALL  */
    STARTCALL = 387,               /* STARTCALL  */
    CALLLIST = 388,                /* CALLLIST  */
    TYPE = 389,                    /* TYPE  */
    ENDTYPE = 390,                 /* ENDTYPE  */
    CLASS = 391,                   /* CLASS  */
    BASICTYPE = 392,               /* BASICTYPE  */
    TYPEC = 393,                   /* TYPEC  */
    QSTRING = 394,                 /* QSTRING  */
    HSTRING = 395,                 /* HSTRING  */
    LR = 396,                      /* LR  */
    LRB = 397,                     /* LRB  */
    DOTOPERATOR = 398,             /* DOTOPERATOR  */
    I_NUMBER = 399,                /* I_NUMBER  */
    UNCLASSIFIED = 400,            /* UNCLASSIFIED  */
    ERROR = 401,                   /* ERROR  */
    OMP = 402,                     /* OMP  */
    SCANOMPFIXED = 403,            /* SCANOMPFIXED  */
    SCANOMPFREE = 404,             /* SCANOMPFREE  */
    DEF = 405,                     /* DEF  */
    EXEC = 406,                    /* EXEC  */
    ENDDEF = 407,                  /* ENDDEF  */
    NONE = 408,                    /* NONE  */
    LAST_TOKEN = 409               /* LAST_TOKEN  */
  };
  typedef enum yytokentype yytoken_kind_t;
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
