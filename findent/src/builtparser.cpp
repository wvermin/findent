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
#line 30 "../src/parser.y"

#include <iostream>

#include "debug.h"
#include "lexer.h"
#include "line_prep.h"
#include "prop.h"

struct propstruct properties;
bool parselabeling = false;
int  itemnumber = 0;

#line 83 "parser.cpp"

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
typedef yytype_int16 yy_state_t;

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
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   567

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  160
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  124
/* YYNRULES -- Number of rules.  */
#define YYNRULES  258
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  486

#define YYUNDEFTOK  2
#define YYMAXUTOK   409


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   155,     2,     2,
       2,     2,   157,     2,   156,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   159,     2,
       2,   158,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    96,    96,    98,    99,   100,   101,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   179,   181,   183,   185,   187,   189,   191,   192,
     194,   196,   198,   199,   202,   204,   218,   219,   221,   223,
     225,   226,   228,   230,   231,   233,   234,   235,   236,   237,
     238,   239,   240,   242,   244,   246,   247,   249,   251,   253,
     254,   256,   257,   258,   260,   262,   264,   266,   268,   270,
     272,   274,   276,   277,   279,   281,   283,   285,   286,   288,
     289,   291,   293,   295,   296,   298,   300,   302,   304,   306,
     308,   309,   311,   313,   314,   317,   319,   321,   324,   327,
     330,   331,   334,   337,   340,   341,   342,   343,   344,   345,
     349,   351,   354,   355,   358,   359,   361,   362,   364,   367,
     368,   371,   372,   375,   376,   378,   380,   381,   382,   383,
     385,   385,   386,   386,   387,   387,   388,   390,   391,   394,
     395,   396,   399,   401,   403,   406,   407,   408,   410,   412,
     413,   414,   416,   417,   420,   422,   425,   427,   430,   433,
     435,   438,   440,   442,   444,   446,   447,   449,   452,   453,
     454,   455,   458,   461,   462,   463,   465,   466,   468,   469,
     470,   471,   474,   476,   478,   480,   482,   484,   486,   488,
     490,   492,   494,   496,   499,   505,   509,   511,   514
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "MODULE", "SUBFUN", "ENTRY", "SUBMODULE",
  "MODULESUBROUTINE", "MODULEFUNCTION", "END", "ENDSUBROUTINE",
  "ENDFUNCTION", "ENDPROGRAM", "ENDMODULE", "ENDSUBMODULE", "IF", "THEN",
  "ELSE", "ELSEIF", "ENDIF", "SIMPLEIF", "NWAYIF", "WHERE", "ENDWHERE",
  "FORALL", "ENDFORALL", "ELSEWHERE", "DO", "DOCOMMA", "DOWHILE",
  "DOCONCURRENT", "ENDDO", "SELECTCASE", "SELECTTYPE", "ENDSELECT", "CASE",
  "CASEDEFAULT", "TYPEIS", "CLASSIS", "CLASSDEFAULT", "SELECTRANK", "RANK",
  "RANKDEFAULT", "INTERFACE", "INTERFACE1", "ABSTRACTINTERFACE",
  "ENDINTERFACE", "CONTAINS", "BLOCK", "ENDBLOCK", "BLOCKDATA",
  "ENDBLOCKDATA", "ASSOCIATE", "ENDASSOCIATE", "CRITICAL", "ENDCRITICAL",
  "CHANGETEAM", "ENDTEAM", "ENUM", "ENDENUM", "ASSIGNMENT", "ASSIGN", "TO",
  "GOTO", "GOTO_COMPUTED", "GOTO_ASSIGNED", "READ", "PRINT", "WRITE", "IO",
  "IOLIST", "STARTIO", "LABELIS", "OTHER", "MODULEPROCEDURE", "PROCEDURE",
  "ENDPROCEDURE", "TIDENTIFIER", "BLANK", "CHAR", "FINDFORMAT", "UNKNOWN",
  "FREE", "FIXED", "UNSURE", "PROBFREE", "FINDENTFIX", "FIXFINDENTFIX",
  "P_ON", "P_OFF", "SCANFIXPRE", "CPP_IF", "CPP_ENDIF", "CPP_ELSE",
  "CPP_ELIF", "CPP", "COCO_IF", "COCO_ENDIF", "COCO_ELSE", "COCO_ELIF",
  "COCO", "INCLUDE", "INCLUDE_CPP", "INCLUDE_CPP_STD", "INCLUDE_COCO",
  "INCFILENAME", "USE", "SEGMENT", "ENDSEGMENT", "ESOPE", "IDENTIFIER",
  "CIDENTIFIER", "SKIP", "SKIPALL", "SKIPNOOP", "SKIPNOS", "KEYWORD",
  "ELEMENTAL", "IMPURE", "PURE", "RECURSIVE", "NON_RECURSIVE",
  "SUBROUTINE", "FUNCTION", "PROGRAM", "EOL", "NAMED_LABEL", "STLABEL",
  "LABEL", "LABELS", "LABELLIST", "CALL", "STARTCALL", "CALLLIST", "TYPE",
  "ENDTYPE", "CLASS", "BASICTYPE", "TYPEC", "QSTRING", "HSTRING", "LR",
  "LRB", "DOTOPERATOR", "I_NUMBER", "UNCLASSIFIED", "ERROR", "OMP",
  "SCANOMPFIXED", "SCANOMPFREE", "DEF", "EXEC", "ENDDEF", "NONE",
  "LAST_TOKEN", "'%'", "','", "'*'", "'='", "':'", "$accept", "lline",
  "labels", "line", "blank", "stlabel", "named_label", "module", "use",
  "include", "identifiers", "abstractinterface", "contains", "interface",
  "moduleprocedure", "procedure", "program_stmt", "subroutine_stmt",
  "subroutine", "subroutine_spec", "subroutinename", "subroutineprefix",
  "subroutineprefix_spec", "function_stmt", "function", "function_spec",
  "functionname", "submodule", "intrinsic_type_spec", "kind_selector",
  "entry", "endassociate", "endblock", "endblockdata", "endcritical",
  "enddo", "endenum", "endforall", "endfunction", "endif", "endinterface",
  "endmodule", "endprocedure", "endprogram", "endselect", "endsubmodule",
  "endsubroutine", "endteam", "endtype", "endsegment", "endwhere",
  "simple_end", "gidentifier", "assignment", "assign", "else", "elseif",
  "elsewhere", "if_construct", "if", "nwayif", "where_construct",
  "forall_construct", "do_construct", "do", "docomma", "goto",
  "goto_computed", "goto_assigned", "goto_prefix", "read", "print", "io",
  "iolist", "itemlist", "item", "$@1", "$@2", "$@3", "otheritem",
  "labellist", "selectcase", "selectrank", "selecttype", "call",
  "calllist", "clist", "citem", "case", "casedefault", "rank",
  "rankdefault", "classdefault", "classis", "typeis", "changeteam",
  "block", "blockdata", "associate", "critical", "enum", "type", "type1",
  "segment", "lvalue", "construct_name", "lr_construct_name", "skipall",
  "skipnoop", "skipnos", "enable_identifier", "enable_cidentifier",
  "enable_skip", "enable_skipall", "enable_skipnoop", "enable_skipnos",
  "getname", "getstlabel", "getlabel", "getlabel1", "getlabel2",
  "getstring", "getlr", "empty", YY_NULLPTR
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,    37,    44,    42,    61,    58
};
# endif

#define YYPACT_NINF (-396)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-247)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -95,  -396,  -396,    40,   432,   -85,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,   -91,   -74,  -396,  -396,  -396,  -396,  -396,
     -65,  -396,   -63,  -396,   -59,  -396,   -55,  -396,  -396,  -396,
    -396,   -46,   -37,  -396,   -36,   -19,  -396,   -17,  -396,   -15,
     -14,  -396,   -13,   -12,  -396,  -396,  -396,  -396,  -396,    17,
      18,  -396,  -396,  -396,     0,  -396,   -99,  -396,     3,  -396,
     -72,    20,     2,     5,     9,     6,   -57,  -396,  -396,  -396,
    -396,  -396,   -33,  -396,  -396,    29,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,    14,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
      15,  -396,  -396,  -396,  -396,  -396,  -396,    24,  -396,  -396,
    -396,  -396,   -64,   -52,  -396,  -396,  -396,   -43,  -396,  -396,
    -396,  -396,  -111,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,   -41,  -396,   -35,    96,  -396,  -396,    50,    51,
    -396,  -396,   -98,    52,   -92,   -89,    42,    45,    -4,    46,
     149,    47,    48,    49,    53,    57,    58,   -87,  -396,  -396,
      60,    62,    63,    65,    66,    67,  -396,    68,  -396,  -396,
      69,    70,  -396,    71,  -396,    64,    88,  -396,  -396,  -396,
    -396,    74,    75,    76,    77,    79,  -396,    80,    81,    82,
      83,  -396,  -396,  -396,  -396,  -396,  -396,   102,   108,  -396,
    -101,  -396,  -396,    99,   100,   -88,  -396,   103,   104,  -396,
     -81,  -396,   105,   -26,  -396,  -396,    87,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,    78,   -38,    84,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,    85,  -396,    97,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,    86,  -396,    89,  -396,  -396,    59,   -52,  -396,
    -396,  -396,  -396,  -396,  -396,    91,  -396,  -396,  -396,    95,
    -396,    98,  -396,  -396,   106,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,   111,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,   107,  -396,   110,   112,  -396,  -396,   113,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,   160,  -396,  -396,  -396,   114,   108,  -396,
    -396,  -396,   -57,  -396,    90,  -396,  -396,   115,   116,  -396,
     118,  -396,  -396,   119,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,   -27,  -396,  -396,  -396,   120,   121,   126,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,    94,  -396,   129,
    -396,  -396,   130,   124,  -396,   135,  -396,  -396,  -396,  -396,
    -396,   117,   131,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,   151,   138,
     139,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,   154,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,   109,  -396,  -396,  -396,  -396,   125,  -396,  -396,  -396,
     -52,  -396,  -396,  -396,  -396,   158,   162,  -396,  -396,  -396,
    -396,   163,  -396,  -396,  -396,  -396,   147,  -396,   152,  -396,
    -396,  -396,  -396,   166,  -396,  -396,   -97,  -396,  -396,  -396,
     134,  -396,  -396,  -396,   155,  -396
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     258,   251,   252,     0,   258,     4,     5,     3,    84,    83,
       1,   245,   245,     0,     0,   245,   245,   245,   245,   245,
       0,   245,     0,   245,     0,   245,     0,   245,   245,   245,
     245,     0,     0,   245,     0,     0,   245,     0,   245,     0,
       0,   245,     0,     0,   245,   248,   245,   248,   248,     0,
       0,   245,   245,   245,     0,   245,     0,   245,     0,   245,
       0,     0,     0,     0,     0,     0,     0,   245,   245,   245,
     151,    82,     0,   245,   245,     0,   150,   245,   253,   245,
     258,   245,   245,     2,    12,    60,    78,    57,     8,    24,
      58,    61,    65,    64,    73,     0,    74,    75,    49,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    81,    48,    72,
     233,    10,     9,    27,    28,    29,    56,    55,    62,    79,
      51,    26,     0,     0,    52,    54,    53,     0,    66,    63,
      67,    68,    59,    69,    70,    71,    15,    16,    17,    18,
      19,    20,    22,    23,    77,    21,    13,    14,    11,    25,
      50,    76,     0,    80,     0,     0,   103,     6,     0,     0,
     257,   149,     0,   258,     0,     0,     0,     0,   159,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   170,   171,
       0,     0,     0,     0,     0,     0,   245,     0,   245,   245,
       0,     0,   245,     0,    92,     0,     0,    90,   135,    91,
     222,     0,     0,     0,     0,     0,   225,     0,     0,     0,
       0,   248,   130,   253,   253,   253,   257,   192,   197,   190,
     185,   186,   196,     0,     0,     0,   256,     0,     0,   147,
       0,   199,     0,   208,   209,   231,     0,   101,   116,   105,
     106,   107,   108,   109,   100,   115,     0,   119,     0,   245,
      99,   104,   245,   114,   110,   234,     0,   154,   246,   173,
     175,   177,   180,   182,   184,   207,   167,   253,   248,   248,
     257,   253,     0,   201,     0,   251,   250,     0,     0,   249,
     178,   251,   248,   245,   143,     0,   251,   236,   132,     0,
     139,     0,   136,   142,     0,   253,   155,   245,   134,   162,
     148,   163,   131,   157,   251,   239,   238,   168,   169,   129,
     202,   204,   141,     0,   215,     0,     0,   218,   203,     0,
     217,   242,   251,   126,   223,   127,   224,   125,   226,   128,
     221,   145,   227,     0,   248,   248,   248,     0,     0,   198,
     254,   189,   188,    88,   251,   251,   137,     0,     0,   251,
       0,   251,    97,   257,   213,   211,   255,   210,   146,   112,
     123,     0,   120,   111,   247,     0,     0,     0,   159,   248,
     165,   166,   248,   248,   257,   253,   248,     0,   244,     0,
     235,   152,     0,     0,   124,     0,   144,   237,   133,   140,
     158,     0,     0,   240,   251,   214,   220,   219,   216,   248,
     245,   179,   181,   183,   253,   195,   191,   187,     0,     0,
       0,   138,    87,   248,   232,   247,   257,   248,   212,   122,
     121,     0,   251,   247,   251,   164,   174,   172,   248,   200,
     228,     0,   251,   243,    85,   251,     0,   156,   241,    93,
       0,   193,    89,    94,    95,     0,     0,   248,   206,    98,
     102,     0,   117,   176,   245,   248,     0,   253,     0,    86,
      96,   205,   113,     0,   229,   118,     0,   153,   251,   160,
       0,   248,   253,   230,     0,   161
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -396,  -396,  -396,  -396,  -396,  -396,   219,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -128,  -396,   275,  -396,  -396,  -396,  -396,   277,
    -396,  -396,  -396,  -396,  -396,  -396,   279,   280,   281,  -396,
     282,   283,   284,  -396,  -396,   -62,  -396,  -396,  -396,  -218,
    -396,  -396,  -396,  -396,   285,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,     4,    56,   232,   -45,  -396,  -396,
     -11,  -396,  -395,  -130,  -396,  -396,  -175,  -396,  -217,  -396,
    -396,  -396,  -215,    16
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,    83,    84,     5,     6,    85,    86,    87,
     354,    88,    89,    90,    91,    92,    93,    94,   259,   260,
     374,    95,   261,    96,   262,   263,   376,    97,   264,   372,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   267,   123,   124,   125,   126,   268,
     128,   129,   130,   131,   132,   133,   269,   270,   271,   137,
     272,   273,   274,   141,   230,   231,   350,   347,   348,   232,
     142,   143,   144,   145,   275,   147,   243,   367,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   172,   186,   204,   391,   387,
     173,   165,   431,   205,   392,   388,     8,     9,   241,   416,
     428,   358,   293,     7
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     168,   169,   207,   208,   278,   279,   343,   344,   345,   282,
     349,   346,   304,    70,   283,   227,   228,   187,   188,   189,
     166,   247,   248,   314,   351,    70,   216,   294,   479,   361,
     456,     1,     2,   298,    70,   206,   300,   356,   461,   266,
      10,     1,   217,   295,   362,   284,    76,   364,   187,   299,
     170,   171,   301,   357,   315,   352,   233,   234,    76,   480,
     379,   276,   237,   238,   383,   382,   240,    76,   242,   285,
     245,   229,   174,   175,   176,   177,   178,   179,   180,   181,
     277,   183,   182,   185,   221,    62,   184,  -246,   401,   192,
      63,    64,   195,    65,   197,   190,   244,   200,   280,   365,
     203,   281,   366,   370,   191,   193,   236,   211,   212,   213,
     386,   215,   266,   218,   429,   286,   393,   430,   287,   371,
     288,   397,   194,   289,   196,   235,   198,   199,   201,   202,
     415,   249,   250,   251,   252,   253,   254,   255,   246,   403,
     305,   214,   209,   210,   219,   222,   223,   226,   427,   224,
     256,   257,   258,   225,   239,    79,   265,   409,    62,   290,
     291,   292,   296,    63,    64,   307,    65,   302,   439,   438,
     303,   306,   308,   309,   310,  -194,   342,   331,   311,   419,
     420,   228,   312,   313,   423,   317,   425,   318,   319,   297,
     320,   321,   322,   324,   327,   328,   330,   451,   332,   333,
     334,   335,   336,   316,   337,   338,   339,   340,   341,   353,
     355,   457,   368,   359,   360,   363,   396,   385,   389,   369,
     398,   404,   410,   399,   167,   373,   378,   384,    79,   448,
     432,   400,   405,   380,   381,   406,   434,   407,   408,   442,
     421,   422,   414,   424,   443,   445,   418,   394,   375,   444,
     476,   377,   323,   441,   325,   326,   447,   460,   329,   462,
     426,   452,   433,   453,   454,   484,   459,   465,   464,   467,
     466,   469,   475,   446,   470,   472,   478,   477,   482,   122,
     485,   127,   395,   134,   135,   136,   138,   139,   140,   146,
     417,   220,   390,   455,     0,     0,     0,     0,     0,   411,
     412,   413,     0,   481,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,     0,     0,   436,   437,     0,
       0,   440,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   449,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   463,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,     0,     0,
     474,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    -7,     0,     0,    11,   483,    12,    13,     0,
       0,    14,    15,    16,    17,    18,    19,    20,     0,    21,
      22,    23,     0,   473,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,     0,    62,     0,  -246,     0,     0,    63,    64,
       0,    65,     0,    66,     0,     0,    67,    68,    69,    70,
      71,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    72,     0,     0,     0,     0,    73,    74,
      75,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,     0,     0,     0,
      78,     0,     0,    79,    80,     0,    81,    82
};

static const yytype_int16 yycheck[] =
{
      11,    12,    47,    48,   132,   133,   223,   224,   225,   137,
     228,   226,    16,    77,   125,    72,    73,    28,    29,    30,
       4,     7,     8,   110,   125,    77,   125,   125,   125,   110,
     425,   126,   127,   125,    77,    46,   125,   125,   433,    15,
       0,   126,   141,   141,   125,   156,   110,    73,    59,   141,
     141,   125,   141,   141,   141,   156,    67,    68,   110,   156,
     277,   125,    73,    74,   281,   280,    77,   110,    79,   110,
      81,   128,    16,    17,    18,    19,   141,    21,   141,    23,
     144,    25,   141,    27,   156,    61,   141,    63,   305,    33,
      66,    67,    36,    69,    38,   141,    80,    41,   141,   125,
      44,   144,   128,   141,   141,   141,   139,    51,    52,    53,
     285,    55,    15,    57,   141,   156,   291,   144,   159,   157,
     155,   296,   141,   158,   141,    69,   141,   141,   141,   141,
     348,   117,   118,   119,   120,   121,   122,   123,    82,   314,
     144,   141,   125,   125,   141,   125,   144,   141,   363,   144,
     136,   137,   138,   144,   125,   131,   141,   332,    61,    63,
     110,   110,   110,    66,    67,    16,    69,   125,   385,   384,
     125,   125,   125,   125,   125,    73,   221,   113,   125,   354,
     355,    73,   125,   125,   359,   125,   361,   125,   125,   173,
     125,   125,   125,   125,   125,   125,   125,   414,   110,   125,
     125,   125,   125,   187,   125,   125,   125,   125,   125,   110,
     110,   426,   125,   110,   110,   110,   125,   128,   159,   141,
     125,   110,    62,   125,     5,   141,   141,   141,   131,   404,
     110,   125,   125,   278,   279,   125,   110,   125,   125,   110,
     125,   125,   128,   125,   114,   110,   156,   292,   259,   125,
     467,   262,   196,   159,   198,   199,   125,   432,   202,   434,
     141,   110,   141,   125,   125,   482,   112,   442,   159,   144,
     445,   113,   125,   156,   112,   112,   110,   125,   144,     4,
     125,     4,   293,     4,     4,     4,     4,     4,     4,     4,
     352,    59,   288,   423,    -1,    -1,    -1,    -1,    -1,   344,
     345,   346,    -1,   478,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   450,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   379,    -1,    -1,   382,   383,    -1,
      -1,   386,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   307,   409,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   427,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   438,    -1,    -1,    -1,    -1,    -1,   410,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   457,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     465,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     0,    -1,    -1,     3,   481,     5,     6,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    -1,    17,
      18,    19,    -1,   464,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    -1,    61,    -1,    63,    -1,    -1,    66,    67,
      -1,    69,    -1,    71,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,   106,   107,
     108,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
     128,    -1,    -1,   131,   132,    -1,   134,   135
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   126,   127,   161,   162,   165,   166,   283,   276,   277,
       0,     3,     5,     6,     9,    10,    11,    12,    13,    14,
      15,    17,    18,    19,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    61,    66,    67,    69,    71,    74,    75,    76,
      77,    78,   101,   106,   107,   108,   110,   124,   128,   131,
     132,   134,   135,   163,   164,   167,   168,   169,   171,   172,
     173,   174,   175,   176,   177,   181,   183,   187,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   240,   241,   242,   243,   244,   245,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   271,   283,   166,   270,   270,
     141,   125,   265,   270,   265,   265,   265,   265,   141,   265,
     141,   265,   141,   265,   141,   265,   266,   270,   270,   270,
     141,   141,   265,   141,   141,   265,   141,   265,   141,   141,
     265,   141,   141,   265,   267,   273,   270,   267,   267,   125,
     125,   265,   265,   265,   141,   265,   125,   141,   265,   141,
     266,   156,   125,   144,   144,   144,   141,    72,    73,   128,
     234,   235,   239,   270,   270,   265,   139,   270,   270,   125,
     270,   278,   270,   246,   283,   270,   265,     7,     8,   117,
     118,   119,   120,   121,   122,   123,   136,   137,   138,   178,
     179,   182,   184,   185,   188,   141,    15,   214,   219,   226,
     227,   228,   230,   231,   232,   244,   125,   144,   212,   212,
     141,   144,   212,   125,   156,   110,   156,   159,   155,   158,
      63,   110,   110,   282,   125,   141,   110,   283,   125,   141,
     125,   141,   125,   125,    16,   144,   125,    16,   125,   125,
     125,   125,   125,   125,   110,   141,   283,   125,   125,   125,
     125,   125,   125,   265,   125,   265,   265,   125,   125,   265,
     125,   113,   110,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   267,   278,   278,   278,   282,   237,   238,   239,
     236,   125,   156,   110,   170,   110,   125,   141,   281,   110,
     110,   110,   125,   110,    73,   125,   128,   247,   125,   141,
     141,   157,   189,   141,   180,   270,   186,   270,   141,   278,
     267,   267,   282,   278,   141,   128,   276,   269,   275,   159,
     264,   268,   274,   276,   267,   270,   125,   276,   125,   125,
     125,   278,   265,   276,   110,   125,   125,   125,   125,   276,
      62,   267,   267,   267,   128,   239,   279,   235,   156,   276,
     276,   125,   125,   276,   125,   276,   141,   282,   280,   141,
     144,   272,   110,   141,   110,   267,   267,   267,   282,   278,
     267,   159,   110,   114,   125,   110,   156,   125,   276,   267,
     270,   278,   110,   125,   125,   273,   272,   282,   267,   112,
     276,   272,   276,   267,   159,   276,   276,   144,   212,   113,
     112,   267,   112,   270,   267,   125,   278,   125,   110,   125,
     156,   276,   144,   267,   278,   125
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   160,   161,   162,   162,   162,   162,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   164,   165,   166,   167,   168,   169,   170,   170,
     171,   172,   173,   173,   174,   175,   176,   176,   177,   178,
     179,   179,   180,   181,   181,   182,   182,   182,   182,   182,
     182,   182,   182,   183,   184,   185,   185,   186,   187,   188,
     188,   189,   189,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   198,   199,   200,   201,   202,   202,   203,
     203,   204,   205,   206,   206,   207,   208,   209,   210,   211,
     212,   212,   213,   214,   214,   215,   216,   217,   218,   219,
     220,   220,   221,   222,   223,   223,   223,   223,   223,   223,
     224,   225,   226,   226,   227,   227,   228,   228,   229,   230,
     230,   231,   231,   232,   232,   233,   234,   234,   234,   234,
     236,   235,   237,   235,   238,   235,   235,   239,   239,   240,
     240,   240,   241,   242,   243,   244,   244,   244,   245,   246,
     246,   246,   247,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   259,   260,   261,   261,
     261,   262,   263,   264,   264,   264,   265,   265,   266,   266,
     266,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     5,     6,     4,     1,     3,
       2,     2,     2,     5,     5,     5,     6,     3,     5,     1,
       1,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     6,     1,     1,     1,     3,     7,     1,
       2,     2,     2,     1,     4,     3,     3,     3,     3,     3,
       2,     3,     3,     4,     3,     2,     3,     3,     4,     3,
       4,     3,     3,     3,     4,     3,     3,     2,     3,     2,
       1,     1,     3,     7,     2,     3,     5,     3,     4,     2,
       8,    11,     3,     3,     4,     3,     3,     2,     3,     3,
       2,     2,     4,     2,     4,     2,     5,     2,     2,     4,
       2,     4,     2,     4,     2,     2,     1,     3,     2,     2,
       0,     3,     0,     4,     0,     3,     1,     1,     2,     2,
       4,     2,     3,     3,     3,     6,     5,     2,     2,     1,
       2,     2,     2,     1,     4,     3,     4,     3,     3,     4,
       4,     3,     2,     3,     3,     2,     3,     3,     4,     6,
       9,     2,     4,     1,     2,     3,     2,     3,     2,     2,
       3,     4,     2,     2,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0
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
  case 8:
#line 104 "../src/parser.y"
                                              { properties.kind = ABSTRACTINTERFACE; properties.type = DEF;    }
#line 1881 "parser.cpp"
    break;

  case 9:
#line 105 "../src/parser.y"
                                              { properties.kind = ASSIGN;            properties.type = EXEC;   }
#line 1887 "parser.cpp"
    break;

  case 10:
#line 106 "../src/parser.y"
                                              { properties.kind = ASSIGNMENT;        properties.type = EXEC;   }
#line 1893 "parser.cpp"
    break;

  case 11:
#line 107 "../src/parser.y"
                                              { properties.kind = ASSOCIATE;         properties.type = EXEC;   }
#line 1899 "parser.cpp"
    break;

  case 12:
#line 108 "../src/parser.y"
                                              { properties.kind = BLANK;             properties.type = NONE;   }
#line 1905 "parser.cpp"
    break;

  case 13:
#line 109 "../src/parser.y"
                                              { properties.kind = BLOCK;             properties.type = EXEC;   }
#line 1911 "parser.cpp"
    break;

  case 14:
#line 110 "../src/parser.y"
                                              { properties.kind = BLOCKDATA;         properties.type = DEF;    }
#line 1917 "parser.cpp"
    break;

  case 15:
#line 111 "../src/parser.y"
                                              { properties.kind = CALL;              properties.type = EXEC;   }
#line 1923 "parser.cpp"
    break;

  case 16:
#line 112 "../src/parser.y"
                                              { properties.kind = CALLLIST;          properties.type = NONE;   }
#line 1929 "parser.cpp"
    break;

  case 17:
#line 113 "../src/parser.y"
                                              { properties.kind = CASE;              properties.type = EXEC;   }
#line 1935 "parser.cpp"
    break;

  case 18:
#line 114 "../src/parser.y"
                                              { properties.kind = CASEDEFAULT;       properties.type = EXEC;   }
#line 1941 "parser.cpp"
    break;

  case 19:
#line 115 "../src/parser.y"
                                              { properties.kind = RANK;              properties.type = EXEC;   }
#line 1947 "parser.cpp"
    break;

  case 20:
#line 116 "../src/parser.y"
                                              { properties.kind = RANKDEFAULT;       properties.type = EXEC;   }
#line 1953 "parser.cpp"
    break;

  case 21:
#line 117 "../src/parser.y"
                                              { properties.kind = CHANGETEAM;        properties.type = EXEC;   }
#line 1959 "parser.cpp"
    break;

  case 22:
#line 118 "../src/parser.y"
                                              { properties.kind = CLASSDEFAULT;      properties.type = EXEC;   }
#line 1965 "parser.cpp"
    break;

  case 23:
#line 119 "../src/parser.y"
                                              { properties.kind = CLASSIS;           properties.type = EXEC;   }
#line 1971 "parser.cpp"
    break;

  case 24:
#line 120 "../src/parser.y"
                                              { properties.kind = CONTAINS;          properties.type = DEF;    }
#line 1977 "parser.cpp"
    break;

  case 25:
#line 121 "../src/parser.y"
                                              { properties.kind = CRITICAL;          properties.type = DEF;    }
#line 1983 "parser.cpp"
    break;

  case 26:
#line 122 "../src/parser.y"
                                              { properties.kind = DO;                properties.type = EXEC;   }
#line 1989 "parser.cpp"
    break;

  case 27:
#line 123 "../src/parser.y"
                                              { properties.kind = ELSE;              properties.type = EXEC;   }
#line 1995 "parser.cpp"
    break;

  case 28:
#line 124 "../src/parser.y"
                                              { properties.kind = ELSEIF;            properties.type = EXEC;   }
#line 2001 "parser.cpp"
    break;

  case 29:
#line 125 "../src/parser.y"
                                              { properties.kind = ELSEWHERE;         properties.type = EXEC;   }
#line 2007 "parser.cpp"
    break;

  case 30:
#line 126 "../src/parser.y"
                                              { properties.kind = ENDASSOCIATE;      properties.type = ENDDEF; }
#line 2013 "parser.cpp"
    break;

  case 31:
#line 127 "../src/parser.y"
                                              { properties.kind = ENDBLOCK;          properties.type = EXEC;   }
#line 2019 "parser.cpp"
    break;

  case 32:
#line 128 "../src/parser.y"
                                              { properties.kind = ENDBLOCKDATA;      properties.type = ENDDEF; }
#line 2025 "parser.cpp"
    break;

  case 33:
#line 129 "../src/parser.y"
                                              { properties.kind = ENDCRITICAL;       properties.type = ENDDEF; }
#line 2031 "parser.cpp"
    break;

  case 34:
#line 130 "../src/parser.y"
                                              { properties.kind = ENDDO;             properties.type = EXEC;   }
#line 2037 "parser.cpp"
    break;

  case 35:
#line 131 "../src/parser.y"
                                              { properties.kind = ENDENUM;           properties.type = DEF;    }
#line 2043 "parser.cpp"
    break;

  case 36:
#line 132 "../src/parser.y"
                                              { properties.kind = ENDFORALL;         properties.type = EXEC;   }
#line 2049 "parser.cpp"
    break;

  case 37:
#line 133 "../src/parser.y"
                                              { properties.kind = ENDFUNCTION;       properties.type = ENDDEF; }
#line 2055 "parser.cpp"
    break;

  case 38:
#line 134 "../src/parser.y"
                                              { properties.kind = ENDIF;             properties.type = EXEC;   }
#line 2061 "parser.cpp"
    break;

  case 39:
#line 135 "../src/parser.y"
                                              { properties.kind = ENDINTERFACE;      properties.type = ENDDEF; }
#line 2067 "parser.cpp"
    break;

  case 40:
#line 136 "../src/parser.y"
                                              { properties.kind = ENDMODULE;         properties.type = ENDDEF; }
#line 2073 "parser.cpp"
    break;

  case 41:
#line 137 "../src/parser.y"
                                              { properties.kind = ENDPROCEDURE;      properties.type = ENDDEF; }
#line 2079 "parser.cpp"
    break;

  case 42:
#line 138 "../src/parser.y"
                                              { properties.kind = ENDPROGRAM;        properties.type = ENDDEF; }
#line 2085 "parser.cpp"
    break;

  case 43:
#line 139 "../src/parser.y"
                                              { properties.kind = ENDSELECT;         properties.type = EXEC;   }
#line 2091 "parser.cpp"
    break;

  case 44:
#line 140 "../src/parser.y"
                                              { properties.kind = ENDSUBMODULE;      properties.type = ENDDEF; }
#line 2097 "parser.cpp"
    break;

  case 45:
#line 141 "../src/parser.y"
                                              { properties.kind = ENDSUBROUTINE;     properties.type = ENDDEF; }
#line 2103 "parser.cpp"
    break;

  case 46:
#line 142 "../src/parser.y"
                                              { properties.kind = ENDTEAM;           properties.type = EXEC;   }
#line 2109 "parser.cpp"
    break;

  case 47:
#line 143 "../src/parser.y"
                                              { properties.kind = ENDTYPE;           properties.type = ENDDEF; }
#line 2115 "parser.cpp"
    break;

  case 48:
#line 144 "../src/parser.y"
                                              { properties.kind = ENDWHERE;          properties.type = EXEC;   }
#line 2121 "parser.cpp"
    break;

  case 49:
#line 145 "../src/parser.y"
                                              { properties.kind = ENTRY;             properties.type = DEF;    }
#line 2127 "parser.cpp"
    break;

  case 50:
#line 146 "../src/parser.y"
                                              { properties.kind = ENUM;              properties.type = DEF;    }
#line 2133 "parser.cpp"
    break;

  case 51:
#line 147 "../src/parser.y"
                                              { properties.kind = FORALL;            properties.type = EXEC;   }
#line 2139 "parser.cpp"
    break;

  case 52:
#line 148 "../src/parser.y"
                                              { properties.kind = GOTO;              properties.type = EXEC;   }
#line 2145 "parser.cpp"
    break;

  case 53:
#line 149 "../src/parser.y"
                                              { properties.kind = GOTO_ASSIGNED;     properties.type = EXEC;   }
#line 2151 "parser.cpp"
    break;

  case 54:
#line 150 "../src/parser.y"
                                              { properties.kind = GOTO_COMPUTED;     properties.type = EXEC;   }
#line 2157 "parser.cpp"
    break;

  case 55:
#line 151 "../src/parser.y"
                                              { properties.kind = SIMPLEIF;          properties.type = EXEC;   }
#line 2163 "parser.cpp"
    break;

  case 56:
#line 152 "../src/parser.y"
                                              { properties.kind = IF;                properties.type = EXEC;   }
#line 2169 "parser.cpp"
    break;

  case 57:
#line 153 "../src/parser.y"
                                              { properties.kind = INCLUDE;           properties.type = NONE;   }
#line 2175 "parser.cpp"
    break;

  case 58:
#line 154 "../src/parser.y"
                                              { properties.kind = INTERFACE;         properties.type = DEF;    }
#line 2181 "parser.cpp"
    break;

  case 59:
#line 155 "../src/parser.y"
                                              { properties.kind = LABELLIST;         properties.type = NONE;   }
#line 2187 "parser.cpp"
    break;

  case 60:
#line 156 "../src/parser.y"
                                              { properties.kind = MODULE;            properties.type = DEF;    }
#line 2193 "parser.cpp"
    break;

  case 61:
#line 157 "../src/parser.y"
                                              { properties.kind = PROCEDURE;         properties.type = DEF;    }
#line 2199 "parser.cpp"
    break;

  case 62:
#line 158 "../src/parser.y"
                                              { properties.kind = NWAYIF;            properties.type = EXEC;   }
#line 2205 "parser.cpp"
    break;

  case 63:
#line 159 "../src/parser.y"
                                              { properties.kind = PRINT;             properties.type = EXEC;   }
#line 2211 "parser.cpp"
    break;

  case 64:
#line 160 "../src/parser.y"
                                              { properties.kind = PROGRAM;           properties.type = DEF;    }
#line 2217 "parser.cpp"
    break;

  case 65:
#line 161 "../src/parser.y"
                                              { properties.kind = PROCEDURE;         properties.type = DEF;    }
#line 2223 "parser.cpp"
    break;

  case 66:
#line 162 "../src/parser.y"
                                              { properties.kind = READ;              properties.type = EXEC;   }
#line 2229 "parser.cpp"
    break;

  case 67:
#line 163 "../src/parser.y"
                                              { properties.kind = IO;                properties.type = EXEC;   }
#line 2235 "parser.cpp"
    break;

  case 68:
#line 164 "../src/parser.y"
                                              { properties.kind = IOLIST;            properties.type = NONE;   }
#line 2241 "parser.cpp"
    break;

  case 69:
#line 165 "../src/parser.y"
                                              { properties.kind = SELECTCASE;        properties.type = EXEC;   }
#line 2247 "parser.cpp"
    break;

  case 70:
#line 166 "../src/parser.y"
                                              { properties.kind = SELECTRANK;        properties.type = EXEC;   }
#line 2253 "parser.cpp"
    break;

  case 71:
#line 167 "../src/parser.y"
                                              { properties.kind = SELECTTYPE;        properties.type = EXEC;   }
#line 2259 "parser.cpp"
    break;

  case 72:
#line 168 "../src/parser.y"
                                              { properties.kind = END;               properties.type = EXEC;   }
#line 2265 "parser.cpp"
    break;

  case 73:
#line 169 "../src/parser.y"
                                              { properties.kind = SUBROUTINE;        properties.type = DEF;    }
#line 2271 "parser.cpp"
    break;

  case 74:
#line 170 "../src/parser.y"
                                              { properties.kind = FUNCTION;          properties.type = DEF;    }
#line 2277 "parser.cpp"
    break;

  case 75:
#line 171 "../src/parser.y"
                                              { properties.kind = SUBMODULE;         properties.type = DEF;    }
#line 2283 "parser.cpp"
    break;

  case 76:
#line 172 "../src/parser.y"
                                              { properties.kind = TYPE;              properties.type = DEF;    }
#line 2289 "parser.cpp"
    break;

  case 77:
#line 173 "../src/parser.y"
                                              { properties.kind = TYPEIS;            properties.type = EXEC;   }
#line 2295 "parser.cpp"
    break;

  case 78:
#line 174 "../src/parser.y"
                                              { properties.kind = USE;               properties.type = DEF;    }
#line 2301 "parser.cpp"
    break;

  case 79:
#line 175 "../src/parser.y"
                                              { properties.kind = WHERE;             properties.type = EXEC;   }
#line 2307 "parser.cpp"
    break;

  case 80:
#line 176 "../src/parser.y"
                                              { properties.kind = SEGMENT;           properties.type = EXEC;   }
#line 2313 "parser.cpp"
    break;

  case 81:
#line 177 "../src/parser.y"
                                              { properties.kind = ENDSEGMENT;        properties.type = EXEC;   }
#line 2319 "parser.cpp"
    break;

  case 87:
#line 189 "../src/parser.y"
                                                       {D(O("include"););}
#line 2325 "parser.cpp"
    break;

  case 190:
#line 385 "../src/parser.y"
                                 {itemnumber++;}
#line 2331 "parser.cpp"
    break;

  case 192:
#line 386 "../src/parser.y"
                                 {itemnumber=123456;}
#line 2337 "parser.cpp"
    break;

  case 194:
#line 387 "../src/parser.y"
                                 {itemnumber=123456;}
#line 2343 "parser.cpp"
    break;

  case 196:
#line 388 "../src/parser.y"
                                   {itemnumber++;}
#line 2349 "parser.cpp"
    break;

  case 245:
#line 480 "../src/parser.y"
                         {lexer_enable(IDENTIFIER);}
#line 2355 "parser.cpp"
    break;

  case 246:
#line 482 "../src/parser.y"
                         {lexer_enable(CIDENTIFIER);}
#line 2361 "parser.cpp"
    break;

  case 247:
#line 484 "../src/parser.y"
                         {lexer_enable(SKIP);}
#line 2367 "parser.cpp"
    break;

  case 248:
#line 486 "../src/parser.y"
                         {lexer_enable(SKIPALL);}
#line 2373 "parser.cpp"
    break;

  case 249:
#line 488 "../src/parser.y"
                         {lexer_enable(SKIPNOOP);}
#line 2379 "parser.cpp"
    break;

  case 250:
#line 490 "../src/parser.y"
                         {lexer_enable(SKIPNOS);}
#line 2385 "parser.cpp"
    break;

  case 251:
#line 492 "../src/parser.y"
                         {properties.name=lexer_getname();}
#line 2391 "parser.cpp"
    break;

  case 252:
#line 494 "../src/parser.y"
                         {properties.stlabel=lexer_getstlabel();}
#line 2397 "parser.cpp"
    break;

  case 253:
#line 496 "../src/parser.y"
                         {properties.labels.push_back(lexer_geti_number());
	                  properties.label_pos.push_back(lexer_get_pos()-lexer_get_len());}
#line 2404 "parser.cpp"
    break;

  case 254:
#line 499 "../src/parser.y"
                         {if (itemnumber == 2)
	                    {properties.labels.push_back(lexer_geti_number());
	                     properties.label_pos.push_back(lexer_get_pos()-lexer_get_len());
			     }
			  }
#line 2414 "parser.cpp"
    break;

  case 255:
#line 505 "../src/parser.y"
                          {properties.labels.push_back(lexer_geti_number());
	                   /* minus one for the extra comma, plus two for preceding ',*' */
	                  properties.label_pos.push_back(lexer_get_pos()-lexer_get_len()-1+2);}
#line 2422 "parser.cpp"
    break;

  case 256:
#line 509 "../src/parser.y"
                         {properties.stringvalue=lexer_getstring();}
#line 2428 "parser.cpp"
    break;

  case 257:
#line 511 "../src/parser.y"
                         {properties.lrvalue=lexer_getlr();
                          properties.lrpos=lexer_getlrpos();}
#line 2435 "parser.cpp"
    break;


#line 2439 "parser.cpp"

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
#line 516 "../src/parser.y"


void resetprop(void)
{
   properties.kind              = UNCLASSIFIED;
   properties.type              = EXEC;
   properties.labels.clear();
   properties.label_pos.clear();
   properties.stlabel           = "";
   properties.name              = "";
   properties.stringvalue       = "";
   properties.lrvalue           = "";
   properties.lrpos             = -123;
}

propstruct parseline(const Line_prep &p)
{
   D(O("entering parseline"););
   lexer_set(p,IDENTIFIER);
   resetprop();
   yyparse();
   if (properties.kind != UNCLASSIFIED)
      return properties;
#ifdef USEESOPE
   lexer_set(p,ESOPE);    // enables KEYWORD+ESOPE (SEGMENT, ENDSEGMENT)
#else
   lexer_set(p,KEYWORD);  // enables KEYWORD
#endif
   yyparse();
   return properties;
}

propstruct parselabels(const std::string &s)
{
   D(O("entering parselabels"););
   lexer_set(s,LABELS);
   resetprop();
   parselabeling = true;
   yyparse();
   return properties;
}

propstruct parseio(const std::string &s)
{
   D(O("entering parseio"););
   lexer_set(s,IOLIST);
   resetprop();
   parselabeling = true;
   itemnumber = 0;
   yyparse();
   return properties;
}

propstruct parsecall(const std::string &s)
{
   D(O("entering parsecall"););
   lexer_set(','+s,CALLLIST);    // ',' to assist lexer to recognise first label
   resetprop();
   parselabeling = true;
   yyparse();
   return properties;
}

void yyerror(const char *c)
{
    (void)c;
    D(O("ERROR");O(c););
    if(parselabeling)
    {
       properties.kind = UNCLASSIFIED;
       properties.type = EXEC;
       }
}

extern "C" int yywrap()
{
  D(O("yywrap"););
  return 1;
}
