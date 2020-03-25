/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y" /* yacc.c:337  */


#include <stdio.h>
#include <glib.h>
#include "util.h"
#include "lang.h"

#define YYDEBUG 1

int yylex();
int yyerror(struct Program **prog, const char *msg);
extern int yylineno;


#line 85 "parser.tab.c" /* yacc.c:337  */
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
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
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
    RELOP = 258,
    UNOP = 259,
    BINOP = 260,
    EQOP = 261,
    ID = 262,
    INTEGER = 263,
    NEWLINE = 264,
    RETURN = 265,
    IF = 266,
    ELSE = 267,
    FOR = 268,
    WHILE = 269,
    BREAK = 270,
    CONTINUE = 271,
    GOTO = 272,
    SWITCH = 273,
    CASE = 274,
    DEFAULT = 275,
    INT = 276,
    VOID = 277
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "parser.y" /* yacc.c:352  */

    int integer;
    char *str;
    GList *list;
    enum Op op;
    struct Constant *constant;
    struct Identifier *identifier;
    struct Declarator *declarator;
    struct Declaration *declaration;
    struct Expr *expr;
    struct Statement *statement;
    struct StatementFor *for_loop;
    struct StatementJump *jump;
    struct StatementLabeled *labeled;
    struct StatementSelection *selection;
    struct StatementSwitch *switch_statement;
    struct StatementWhile *while_loop;
    struct Program *program;
    struct Function *function;
    struct Type *type;
    struct Block *block;
    void *block_element;

#line 175 "parser.tab.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (struct Program **prog);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
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
#define YYLAST   227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  91
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  158

#define YYUNDEFTOK  2
#define YYMAXUTOK   277

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      26,    27,    33,    35,    24,    36,     2,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    32,    23,
       2,    25,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    28,     2,    29,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,    31,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    80,    80,    87,    94,   100,   102,   106,   110,   116,
     120,   126,   130,   136,   138,   145,   163,   167,   169,   171,
     175,   179,   183,   187,   202,   204,   208,   212,   221,   223,
     227,   231,   235,   241,   246,   252,   259,   265,   271,   277,
     283,   289,   295,   301,   309,   315,   321,   325,   331,   336,
     341,   347,   355,   360,   365,   372,   377,   389,   396,   403,
     410,   420,   425,   432,   434,   441,   448,   454,   458,   460,
     467,   477,   479,   486,   495,   497,   506,   508,   517,   522,
     527,   532,   537,   542,   546,   550,   552,   559,   573,   577,
     581,   587
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "RELOP", "UNOP", "BINOP", "EQOP", "ID",
  "INTEGER", "NEWLINE", "RETURN", "IF", "ELSE", "FOR", "WHILE", "BREAK",
  "CONTINUE", "GOTO", "SWITCH", "CASE", "DEFAULT", "INT", "VOID", "';'",
  "','", "'='", "'('", "')'", "'['", "']'", "'{'", "'}'", "':'", "'*'",
  "'/'", "'+'", "'-'", "$accept", "program", "top_level_block",
  "top_level_block_element", "declaration", "declaration_specifiers",
  "init_declarator_list", "init_declarator", "initializer", "declarator",
  "direct_declarator", "type_specifier", "block", "block_element",
  "parameter_type_list", "parameter_list", "parameter_declaration",
  "function", "statement", "compound_statement", "expr_statement",
  "for_statement", "jump_statement", "labeled_statement",
  "selection_statement", "switch_statement", "while_statement",
  "primary_expr", "argument_expr_list", "postfix_expr", "unary_expr",
  "multiplicative_expr", "additive_expr", "relational_expr",
  "equality_expr", "and_expr", "exclusive_or_expr", "inclusive_or_expr",
  "logical_and_expr", "logical_or_expr", "conditional_expr",
  "constant_expr", "assignment_expr", "expr", "identifier_list",
  "identifier", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    59,    44,    61,    40,    41,    91,    93,
     123,   125,    58,    42,    47,    43,    45
};
# endif

#define YYPACT_NINF -98

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-98)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      49,   -98,   -98,    27,    49,   -98,   -98,    74,   -98,   -98,
     -98,   -98,   -98,   -98,    15,    66,   -98,    14,    -9,   -98,
     -98,    22,   -98,    15,    84,   179,   -98,     2,     7,   -98,
     -98,    33,   -98,   -98,   -98,    31,   -98,    68,    82,    58,
      62,   -98,    47,    84,     6,    54,    56,    70,    75,    97,
      80,    84,    78,   -98,    74,   154,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,    89,     3,   -98,
      87,    94,    49,   -98,    13,   -98,   -98,    26,    62,   -98,
     -98,   -98,   -98,   -98,   -98,    85,   -98,    84,    18,    84,
      84,    84,    84,    84,    84,    84,    84,   101,    84,    84,
      84,   -98,   -98,   102,    84,    95,   197,   -98,   -98,   -98,
     197,   -98,   -98,   -98,    97,   -98,   -98,   -98,   -98,    19,
     -98,   100,   -98,   -98,    68,    68,    82,    58,   -98,   -98,
      99,    84,   103,   -98,   104,   197,   -98,   -98,   -98,    84,
     -98,   -98,   197,    23,   197,   197,   -98,   -98,   120,   197,
     106,   -98,   -98,   197,   -98,   197,   -98,   -98
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    24,    25,     0,     2,     3,     5,     0,     9,     6,
       1,     4,    91,     7,     0,     0,    11,    13,    16,    10,
      17,     0,     8,     0,     0,     0,    35,     0,     0,    18,
      12,    13,    60,    14,    63,    67,    68,    71,    74,    76,
      85,    15,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    28,     0,     0,    27,    29,    36,    37,
      38,    39,    40,    41,    42,    43,    88,     0,    59,    23,
      34,     0,    30,    31,     0,    89,    20,    67,    78,    79,
      80,    81,    82,    83,    84,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,     0,     0,     0,     0,    44,    26,    45,
       0,    33,    21,    32,     0,    22,    19,    87,    64,     0,
      61,     0,    69,    70,    72,    73,    75,    77,    86,    51,
       0,     0,     0,    50,     0,     0,    54,    52,    90,     0,
      65,    66,     0,     0,     0,     0,    53,    62,    55,     0,
       0,    58,    57,     0,    46,     0,    56,    47
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -98,   -98,   -98,   130,   -22,   -17,   -98,   113,   -98,    -3,
     -98,    -1,   -98,    86,   -98,   -98,    67,   -98,     5,   126,
     -97,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -16,
      29,    30,    50,    57,   117,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -23,   -38,   -98,    -7
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,     7,    15,    16,    33,    31,
      18,     8,    55,    56,    71,    72,    73,     9,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    34,   119,    35,
      36,    37,    38,    39,    40,    79,    80,    81,    82,    83,
      84,    85,    66,    67,    74,    42
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      20,    41,   131,    53,    17,    97,    19,    20,    54,    12,
      70,    21,    77,   105,    12,    32,    20,    27,    68,    28,
      75,    86,    12,     1,     2,    12,    32,    10,    96,    69,
      12,    32,    98,    53,   143,   110,    76,   114,    54,    24,
     115,    14,   103,   139,    25,   118,   140,    20,    68,    29,
     149,   121,    88,    19,    89,    70,    87,    88,    24,    89,
     130,    94,   132,    20,   117,   120,   134,   111,    95,    19,
       1,     2,    96,   128,    77,    77,    77,    77,    77,    77,
      99,    12,   100,    86,    86,    86,    86,    86,    86,    22,
      23,    12,    32,   101,    12,     1,     2,    13,   102,    68,
      14,    90,    91,    68,    12,   150,   104,   138,     1,     2,
     106,   136,   109,    14,   116,   137,   147,    92,    93,   122,
     123,   112,   124,   125,   129,   133,   142,   135,    68,   141,
     144,   145,   153,   155,    11,    68,    30,    68,    68,   113,
     146,   108,    68,    26,   126,    78,    68,   148,    68,   151,
     152,     0,   127,     0,   154,     0,     0,     0,   156,     0,
     157,    12,    32,     0,    43,    44,     0,    45,    46,    47,
      48,    49,    50,    51,    52,     1,     2,     0,     0,     0,
       0,     0,     0,     0,    25,   107,    12,    32,     0,    43,
      44,     0,    45,    46,    47,    48,    49,    50,    51,    52,
       1,     2,     0,     0,    12,    32,     0,    43,    44,    25,
      45,    46,    47,    48,    49,    50,    51,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    25
};

static const yytype_int16 yycheck[] =
{
       7,    24,    99,    25,     7,    43,     7,    14,    25,     7,
      27,    14,    28,    51,     7,     8,    23,    26,    25,    28,
      27,    28,     7,    21,    22,     7,     8,     0,    25,    27,
       7,     8,    26,    55,   131,    32,    29,    24,    55,    25,
      27,    26,    49,    24,    30,    27,    27,    54,    55,    27,
      27,    89,    26,    54,    28,    72,    25,    26,    25,    28,
      98,     3,   100,    70,    87,    88,   104,    70,     6,    70,
      21,    22,    25,    96,    90,    91,    92,    93,    94,    95,
      26,     7,    26,    90,    91,    92,    93,    94,    95,    23,
      24,     7,     8,    23,     7,    21,    22,    23,    23,   106,
      26,    33,    34,   110,     7,   143,    26,   114,    21,    22,
      32,   106,    23,    26,    29,   110,   139,    35,    36,    90,
      91,    27,    92,    93,    23,    23,    27,    32,   135,    29,
      27,    27,    12,    27,     4,   142,    23,   144,   145,    72,
     135,    55,   149,    17,    94,    28,   153,   142,   155,   144,
     145,    -1,    95,    -1,   149,    -1,    -1,    -1,   153,    -1,
     155,     7,     8,    -1,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    31,     7,     8,    -1,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,     7,     8,    -1,    10,    11,    30,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    21,    22,    38,    39,    40,    41,    42,    48,    54,
       0,    40,     7,    23,    26,    43,    44,    46,    47,    48,
      82,    46,    23,    24,    25,    30,    56,    26,    28,    27,
      44,    46,     8,    45,    64,    66,    67,    68,    69,    70,
      71,    79,    82,    10,    11,    13,    14,    15,    16,    17,
      18,    19,    20,    41,    42,    49,    50,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    79,    80,    82,    27,
      42,    51,    52,    53,    81,    82,    29,    66,    71,    72,
      73,    74,    75,    76,    77,    78,    82,    25,    26,    28,
      33,    34,    35,    36,     3,     6,    25,    80,    26,    26,
      26,    23,    23,    82,    26,    80,    32,    31,    50,    23,
      32,    46,    27,    53,    24,    27,    29,    79,    27,    65,
      79,    80,    67,    67,    68,    68,    69,    70,    79,    23,
      80,    57,    80,    23,    80,    32,    55,    55,    82,    24,
      27,    29,    27,    57,    27,    27,    55,    79,    55,    27,
      80,    55,    55,    12,    55,    27,    55,    55
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    37,    38,    39,    39,    40,    40,    41,    41,    42,
      42,    43,    43,    44,    44,    45,    46,    47,    47,    47,
      47,    47,    47,    47,    48,    48,    49,    49,    50,    50,
      51,    52,    52,    53,    53,    54,    55,    55,    55,    55,
      55,    55,    55,    55,    56,    57,    58,    58,    59,    59,
      59,    59,    60,    60,    60,    61,    61,    62,    63,    64,
      64,    65,    65,    66,    66,    66,    66,    67,    68,    68,
      68,    69,    69,    69,    70,    70,    71,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    79,    79,    80,    81,
      81,    82
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     2,     3,     1,
       2,     1,     3,     1,     3,     1,     1,     1,     3,     4,
       3,     4,     4,     3,     1,     1,     2,     1,     1,     1,
       1,     1,     2,     2,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     6,     7,     2,     2,
       3,     3,     3,     4,     3,     5,     7,     5,     5,     1,
       1,     1,     3,     1,     3,     4,     4,     1,     1,     3,
       3,     1,     3,     3,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     1,     1,
       3,     1
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
        yyerror (prog, YY_("syntax error: cannot back up")); \
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
                  Type, Value, prog); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, struct Program **prog)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (prog);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, struct Program **prog)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, prog);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, struct Program **prog)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              , prog);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, prog); \
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
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
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
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
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, struct Program **prog)
{
  YYUSE (yyvaluep);
  YYUSE (prog);
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
yyparse (struct Program **prog)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
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
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
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

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
#line 80 "parser.y" /* yacc.c:1652  */
    {
                    struct Program *full_program = program_new((yyvsp[0].block));
                    *prog = full_program;

                }
#line 1406 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 3:
#line 87 "parser.y" /* yacc.c:1652  */
    {
                    GList *list = NULL;
                    list = g_list_append(list, (yyvsp[0].block_element));
                    struct Block *block = block_new(list, NULL);
                    (yyval.block) = block;
                }
#line 1417 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 4:
#line 94 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.block) = block_extend((yyvsp[-1].block), (yyvsp[0].block_element));
                }
#line 1425 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 5:
#line 100 "parser.y" /* yacc.c:1652  */
    { (yyval.block_element) = (yyvsp[0].declaration); }
#line 1431 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 6:
#line 102 "parser.y" /* yacc.c:1652  */
    { (yyval.block_element) = (yyvsp[0].function); }
#line 1437 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 7:
#line 106 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.declaration) = declaration_new((yyvsp[-1].list), NULL);
                }
#line 1445 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 8:
#line 110 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.declaration) = declaration_new((yyvsp[-2].list), (yyvsp[-1].list));
                }
#line 1453 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 9:
#line 116 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.list) = g_list_append(NULL, GINT_TO_POINTER((yyvsp[0].integer)));
                }
#line 1461 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 10:
#line 120 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.list) = g_list_prepend((yyvsp[-1].list), GINT_TO_POINTER((yyvsp[-1].list)));
                }
#line 1469 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 11:
#line 126 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.list) = g_list_append(NULL, (yyvsp[0].declarator));
                }
#line 1477 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 12:
#line 130 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.list) = g_list_append((yyvsp[-2].list), (yyvsp[0].declarator));
                }
#line 1485 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 13:
#line 136 "parser.y" /* yacc.c:1652  */
    { (yyval.declarator) = (yyvsp[0].declarator); }
#line 1491 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 14:
#line 138 "parser.y" /* yacc.c:1652  */
    {
                    (yyvsp[-2].declarator)->initializer = (yyvsp[0].expr);
                    (yyval.declarator) = (yyvsp[-2].declarator);
                }
#line 1500 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 15:
#line 145 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1506 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 16:
#line 163 "parser.y" /* yacc.c:1652  */
    { (yyval.declarator) = (yyvsp[0].declarator); }
#line 1512 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 17:
#line 167 "parser.y" /* yacc.c:1652  */
    { (yyval.declarator) = declarator_new(DECLARATOR_IDENTIFIER, NULL, (yyvsp[0].identifier)); }
#line 1518 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 18:
#line 169 "parser.y" /* yacc.c:1652  */
    { (yyval.declarator) = (yyvsp[-1].declarator); }
#line 1524 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 19:
#line 171 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.declarator) = declarator_new(DECLARATOR_ARRAY, (yyvsp[-3].declarator), (yyvsp[-1].expr));
                }
#line 1532 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 20:
#line 175 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.declarator) = declarator_new(DECLARATOR_ARRAY, (yyvsp[-2].declarator), NULL);
                }
#line 1540 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 21:
#line 179 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.declarator) = declarator_new(DECLARATOR_FUNCTION, (yyvsp[-3].declarator), (yyvsp[-1].list));
                }
#line 1548 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 22:
#line 183 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.declarator) = declarator_new(DECLARATOR_FUNCTION, (yyvsp[-3].declarator), (yyvsp[-1].list));
                }
#line 1556 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 23:
#line 187 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.declarator) = declarator_new(DECLARATOR_FUNCTION, (yyvsp[-2].declarator), NULL);
                }
#line 1564 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 24:
#line 202 "parser.y" /* yacc.c:1652  */
    { (yyval.integer) = (yyvsp[0].integer); }
#line 1570 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 25:
#line 204 "parser.y" /* yacc.c:1652  */
    {(yyval.integer) = (yyvsp[0].integer); }
#line 1576 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 26:
#line 208 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.block) = block_extend((yyvsp[-1].block), (yyvsp[0].block_element));
                }
#line 1584 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 27:
#line 212 "parser.y" /* yacc.c:1652  */
    {
                    GList *list = NULL;
                    list = g_list_append(list, (yyvsp[0].block_element));
                    struct Block *block = block_new(list, NULL);
                    (yyval.block) = block;
                }
#line 1595 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 28:
#line 221 "parser.y" /* yacc.c:1652  */
    { (yyval.block_element) = (yyvsp[0].declaration); }
#line 1601 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 29:
#line 223 "parser.y" /* yacc.c:1652  */
    { (yyval.block_element) = (yyvsp[0].statement); }
#line 1607 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 30:
#line 227 "parser.y" /* yacc.c:1652  */
    { (yyval.list) = (yyvsp[0].list); }
#line 1613 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 31:
#line 231 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.list) = g_list_append(NULL, (yyvsp[0].declaration));
                }
#line 1621 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 32:
#line 235 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.list) = g_list_append((yyvsp[-1].list), (yyvsp[0].declaration));
                }
#line 1629 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 33:
#line 241 "parser.y" /* yacc.c:1652  */
    {
                    GList *declarators = g_list_append(NULL, (yyvsp[0].declarator));
                    (yyval.declaration) = declaration_new((yyvsp[-1].list), declarators);
                }
#line 1638 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 34:
#line 246 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.declaration) = declaration_new((yyvsp[0].list), NULL);
                }
#line 1646 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 35:
#line 252 "parser.y" /* yacc.c:1652  */
    {
                    struct Function *func = function_new((yyvsp[-1].declarator), (yyvsp[0].block), (yyvsp[-2].list));
                    (yyval.function) = func;
                }
#line 1655 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 36:
#line 259 "parser.y" /* yacc.c:1652  */
    {
                    struct Statement *stmt = statement_new(STMT_TYPE_COMPOUND);
                    stmt->compound = (yyvsp[0].block);
                    (yyval.statement) = stmt;
                }
#line 1665 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 37:
#line 265 "parser.y" /* yacc.c:1652  */
    {
                    struct Statement *stmt = statement_new(STMT_TYPE_EXPR);
                    stmt->expr = (yyvsp[0].expr);
                    (yyval.statement) = stmt;
                }
#line 1675 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 38:
#line 271 "parser.y" /* yacc.c:1652  */
    {
                    struct Statement *stmt = statement_new(STMT_TYPE_FOR);
                    stmt->for_loop = (yyvsp[0].for_loop);
                    (yyval.statement) = stmt;
                }
#line 1685 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 39:
#line 277 "parser.y" /* yacc.c:1652  */
    {
                    struct Statement *stmt = statement_new(STMT_TYPE_JUMP);
                    stmt->jump = (yyvsp[0].jump);
                    (yyval.statement) = stmt;
                }
#line 1695 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 40:
#line 283 "parser.y" /* yacc.c:1652  */
    {
                    struct Statement *stmt = statement_new(STMT_TYPE_LABELED);
                    stmt->labeled = (yyvsp[0].labeled);
                    (yyval.statement) = stmt;
                }
#line 1705 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 41:
#line 289 "parser.y" /* yacc.c:1652  */
    {
                    struct Statement *stmt = statement_new(STMT_TYPE_SELECTION);
                    stmt->selection = (yyvsp[0].selection);
                    (yyval.statement) = stmt;
                }
#line 1715 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 42:
#line 295 "parser.y" /* yacc.c:1652  */
    {
                    struct Statement *stmt = statement_new(STMT_TYPE_SWITCH);
                    stmt->switch_statement = (yyvsp[0].switch_statement);
                    (yyval.statement) = stmt;
                }
#line 1725 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 43:
#line 301 "parser.y" /* yacc.c:1652  */
    {
                    struct Statement *stmt = statement_new(STMT_TYPE_WHILE);
                    stmt->while_loop = (yyvsp[0].while_loop);
                    (yyval.statement) = stmt;
                }
#line 1735 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 44:
#line 309 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.block) = (yyvsp[-1].block);
                }
#line 1743 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 45:
#line 315 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.expr) = (yyvsp[-1].expr);
                }
#line 1751 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 46:
#line 321 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.for_loop) = statement_for_new((yyvsp[-3].expr), (yyvsp[-2].expr), NULL, (yyvsp[0].statement));
                }
#line 1759 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 47:
#line 325 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.for_loop) = statement_for_new((yyvsp[-4].expr), (yyvsp[-3].expr), (yyvsp[-2].expr), (yyvsp[0].statement));
                }
#line 1767 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 48:
#line 331 "parser.y" /* yacc.c:1652  */
    {
                    struct StatementJump *jump = statement_jump_new(JUMP_BREAK);
                    (yyval.jump) = jump;
                }
#line 1776 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 49:
#line 336 "parser.y" /* yacc.c:1652  */
    {
                    struct StatementJump *jump = statement_jump_new(JUMP_CONTINUE);
                    (yyval.jump) = jump;
                }
#line 1785 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 50:
#line 341 "parser.y" /* yacc.c:1652  */
    {
                    struct StatementJump *jump = statement_jump_new(JUMP_GOTO);
                    jump->identifier = (yyvsp[-1].identifier);
                    (yyval.jump) = jump;
                }
#line 1795 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 51:
#line 347 "parser.y" /* yacc.c:1652  */
    {
                    struct StatementJump *jump = statement_jump_new(JUMP_RETURN);
                    jump->expr = (yyvsp[-1].expr);
                    (yyval.jump) = jump;
                }
#line 1805 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 52:
#line 355 "parser.y" /* yacc.c:1652  */
    {
                    struct StatementLabeled *labeled = statement_labeled_new(LABELED_LABEL, (yyvsp[-2].identifier), (yyvsp[0].statement));
                    (yyval.labeled) = labeled;
                }
#line 1814 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 53:
#line 360 "parser.y" /* yacc.c:1652  */
    {
                    struct StatementLabeled *labeled = statement_labeled_new(LABELED_CASE, (yyvsp[-2].expr), (yyvsp[0].statement));
                    (yyval.labeled) = labeled;
                }
#line 1823 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 54:
#line 365 "parser.y" /* yacc.c:1652  */
    {
                    struct StatementLabeled *labeled = statement_labeled_new(LABELED_DEFAULT, NULL, (yyvsp[0].statement));
                    (yyval.labeled) = labeled;
                }
#line 1832 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 55:
#line 372 "parser.y" /* yacc.c:1652  */
    {
                    struct StatementSelection *cond = statement_selection_new((yyvsp[-2].expr), (yyvsp[0].statement), NULL);
                    (yyval.selection) = cond;
                }
#line 1841 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 56:
#line 377 "parser.y" /* yacc.c:1652  */
    {
                    struct StatementSelection *cond = statement_selection_new((yyvsp[-4].expr), (yyvsp[-2].statement), (yyvsp[0].statement));
                    (yyval.selection) = cond;
                }
#line 1850 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 57:
#line 389 "parser.y" /* yacc.c:1652  */
    {
                    struct StatementSwitch *sel = statement_switch_new((yyvsp[-2].expr), (yyvsp[0].statement));
                    (yyval.switch_statement) = sel;
                }
#line 1859 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 58:
#line 396 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.while_loop) = statement_while_new((yyvsp[-2].expr), (yyvsp[0].statement));
                }
#line 1867 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 59:
#line 403 "parser.y" /* yacc.c:1652  */
    {
                    struct ExprIdentifier *expr_identifier = expr_identifier_new((yyvsp[0].identifier));
                    struct Expr *expr = expr_new(EXPR_IDENTIFIER);
                    expr->id = expr_identifier;
                    (yyval.expr) = expr;
                }
#line 1878 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 60:
#line 410 "parser.y" /* yacc.c:1652  */
    {
                    struct Constant *constant = constant_new((yyvsp[0].integer));
                    struct ExprConstant *expr_constant = expr_constant_new(constant);
                    struct Expr *expr = expr_new(EXPR_CONSTANT);
                    expr->constant = expr_constant;
                    (yyval.expr) = expr;
                }
#line 1890 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 61:
#line 420 "parser.y" /* yacc.c:1652  */
    {
                    GList *expr_list = g_list_append(NULL, (yyvsp[0].expr));
                    (yyval.list) = expr_list;
                }
#line 1899 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 62:
#line 425 "parser.y" /* yacc.c:1652  */
    {
                    GList *expr_list = g_list_append((yyvsp[-2].list), (yyvsp[0].expr));
                    (yyval.list) = expr_list;
                }
#line 1908 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 63:
#line 432 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1914 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 64:
#line 434 "parser.y" /* yacc.c:1652  */
    {
                    struct ExprCall *call = expr_call_new((yyvsp[-2].expr), NULL);
                    struct Expr *expr = expr_new(EXPR_CALL);
                    expr->call = call;
                    (yyval.expr) = expr;
                }
#line 1925 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 65:
#line 441 "parser.y" /* yacc.c:1652  */
    {
                    struct ExprCall *call = expr_call_new((yyvsp[-3].expr), (yyvsp[-1].list));
                    struct Expr *expr = expr_new(EXPR_CALL);
                    expr->call = call;
                    (yyval.expr) = expr;
                }
#line 1936 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 66:
#line 448 "parser.y" /* yacc.c:1652  */
    {

                }
#line 1944 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 67:
#line 454 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1950 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 68:
#line 458 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1956 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 69:
#line 460 "parser.y" /* yacc.c:1652  */
    {
                    struct ExprOp *binop = expr_op_new(Op_mul, (yyvsp[-2].expr), (yyvsp[0].expr));
                    struct Expr *expr = expr_new(EXPR_BINOP);
                    expr->op = binop;
                    (yyval.expr) = expr;
                }
#line 1967 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 70:
#line 467 "parser.y" /* yacc.c:1652  */
    {
                    struct ExprOp *binop = expr_op_new(Op_div, (yyvsp[-2].expr), (yyvsp[0].expr));
                    struct Expr *expr = expr_new(EXPR_BINOP);
                    expr->op = binop;
                    (yyval.expr) = expr;
                }
#line 1978 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 71:
#line 477 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1984 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 72:
#line 479 "parser.y" /* yacc.c:1652  */
    {
                    struct ExprOp *binop = expr_op_new(Op_add, (yyvsp[-2].expr), (yyvsp[0].expr));
                    struct Expr *expr = expr_new(EXPR_BINOP);
                    expr->op = binop;
                    (yyval.expr) = expr;
                }
#line 1995 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 73:
#line 486 "parser.y" /* yacc.c:1652  */
    {
                    struct ExprOp *binop = expr_op_new(Op_sub, (yyvsp[-2].expr), (yyvsp[0].expr));
                    struct Expr *expr = expr_new(EXPR_BINOP);
                    expr->op = binop;
                    (yyval.expr) = expr;
                }
#line 2006 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 74:
#line 495 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2012 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 75:
#line 497 "parser.y" /* yacc.c:1652  */
    {
                    struct ExprOp *relop = expr_op_new((yyvsp[-1].op), (yyvsp[-2].expr), (yyvsp[0].expr));
                    struct Expr *expr = expr_new(EXPR_RELOP);
                    expr->op = relop;
                    (yyval.expr) = expr;
                }
#line 2023 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 76:
#line 506 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2029 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 77:
#line 508 "parser.y" /* yacc.c:1652  */
    {
                    struct ExprOp *relop = expr_op_new((yyvsp[-1].op), (yyvsp[-2].expr), (yyvsp[0].expr));
                    struct Expr *expr = expr_new(EXPR_RELOP);
                    expr->op = relop;
                    (yyval.expr) = expr;
                }
#line 2040 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 78:
#line 517 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2046 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 79:
#line 522 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2052 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 80:
#line 527 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2058 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 81:
#line 532 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2064 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 82:
#line 537 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2070 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 83:
#line 542 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2076 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 84:
#line 546 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2082 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 85:
#line 550 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2088 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 86:
#line 552 "parser.y" /* yacc.c:1652  */
    {
                    struct ExprAssignment *assignment = expr_assignment_new(LVALUE_IDENTIFIER, (yyvsp[-2].identifier), (yyvsp[0].expr));
                    struct Expr *expr = expr_new(EXPR_ASSIGNMENT);
                    expr->assignment = assignment;
                    (yyval.expr) = expr;
                }
#line 2099 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 87:
#line 559 "parser.y" /* yacc.c:1652  */
    {
                    struct Expr *postfix = (yyvsp[-2].expr);
                    if (postfix->type != EXPR_INDEXED) {
                        log_err("Only Identifiers and indexed Expressions can appear on the LHS of an assignment");
                        YYABORT;
                    } else {
                        struct ExprAssignment *assignment = expr_assignment_new(LVALUE_INDEXED, (yyvsp[-2].expr), (yyvsp[0].expr));
                        struct Expr *expr = expr_new(EXPR_ASSIGNMENT);
                        expr->assignment = assignment;
                        (yyval.expr) = expr;
                    }
                }
#line 2116 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 88:
#line 573 "parser.y" /* yacc.c:1652  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2122 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 89:
#line 577 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.list) = g_list_append(NULL, (yyvsp[0].identifier));
                }
#line 2130 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 90:
#line 581 "parser.y" /* yacc.c:1652  */
    {
                    (yyval.list) = g_list_append((yyvsp[-2].list), (yyvsp[0].identifier));
                }
#line 2138 "parser.tab.c" /* yacc.c:1652  */
    break;

  case 91:
#line 587 "parser.y" /* yacc.c:1652  */
    {
                    struct Identifier *id = identifier_new((yyvsp[0].str), NULL);
                    (yyval.identifier) = id;
                }
#line 2147 "parser.tab.c" /* yacc.c:1652  */
    break;


#line 2151 "parser.tab.c" /* yacc.c:1652  */
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
      yyerror (prog, YY_("syntax error"));
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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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
        yyerror (prog, yymsgp);
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
                      yytoken, &yylval, prog);
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
                  yystos[yystate], yyvsp, prog);
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
  yyerror (prog, YY_("memory exhausted"));
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
                  yytoken, &yylval, prog);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, prog);
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
#line 593 "parser.y" /* yacc.c:1918  */


int yyerror (struct Program **p, const char *s) {
    fprintf(stderr, "%d: error: %s\n", yylineno, s);
    return 1;
}
