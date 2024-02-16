/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 19 "parser.y"

#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include "code_gen.h"

int yylex(void);
void yyerror (char const *mensagem);

extern int get_line_number(void);
extern void *arvore;


#line 85 "parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TK_PR_INT = 3,                  /* TK_PR_INT  */
  YYSYMBOL_TK_PR_FLOAT = 4,                /* TK_PR_FLOAT  */
  YYSYMBOL_TK_PR_BOOL = 5,                 /* TK_PR_BOOL  */
  YYSYMBOL_TK_PR_IF = 6,                   /* TK_PR_IF  */
  YYSYMBOL_TK_PR_ELSE = 7,                 /* TK_PR_ELSE  */
  YYSYMBOL_TK_PR_WHILE = 8,                /* TK_PR_WHILE  */
  YYSYMBOL_TK_PR_RETURN = 9,               /* TK_PR_RETURN  */
  YYSYMBOL_TK_OC_LE = 10,                  /* TK_OC_LE  */
  YYSYMBOL_TK_OC_GE = 11,                  /* TK_OC_GE  */
  YYSYMBOL_TK_OC_EQ = 12,                  /* TK_OC_EQ  */
  YYSYMBOL_TK_OC_NE = 13,                  /* TK_OC_NE  */
  YYSYMBOL_TK_OC_AND = 14,                 /* TK_OC_AND  */
  YYSYMBOL_TK_OC_OR = 15,                  /* TK_OC_OR  */
  YYSYMBOL_TK_IDENTIFICADOR = 16,          /* TK_IDENTIFICADOR  */
  YYSYMBOL_TK_LIT_INT = 17,                /* TK_LIT_INT  */
  YYSYMBOL_TK_LIT_FLOAT = 18,              /* TK_LIT_FLOAT  */
  YYSYMBOL_TK_LIT_FALSE = 19,              /* TK_LIT_FALSE  */
  YYSYMBOL_TK_LIT_TRUE = 20,               /* TK_LIT_TRUE  */
  YYSYMBOL_TK_ERRO = 21,                   /* TK_ERRO  */
  YYSYMBOL_22_ = 22,                       /* ';'  */
  YYSYMBOL_23_ = 23,                       /* '{'  */
  YYSYMBOL_24_ = 24,                       /* '}'  */
  YYSYMBOL_25_ = 25,                       /* '('  */
  YYSYMBOL_26_ = 26,                       /* ')'  */
  YYSYMBOL_27_ = 27,                       /* '!'  */
  YYSYMBOL_28_ = 28,                       /* ','  */
  YYSYMBOL_29_ = 29,                       /* '='  */
  YYSYMBOL_30_ = 30,                       /* '<'  */
  YYSYMBOL_31_ = 31,                       /* '>'  */
  YYSYMBOL_32_ = 32,                       /* '+'  */
  YYSYMBOL_33_ = 33,                       /* '-'  */
  YYSYMBOL_34_ = 34,                       /* '*'  */
  YYSYMBOL_35_ = 35,                       /* '/'  */
  YYSYMBOL_36_ = 36,                       /* '%'  */
  YYSYMBOL_YYACCEPT = 37,                  /* $accept  */
  YYSYMBOL_push_scope = 38,                /* push_scope  */
  YYSYMBOL_pop_scope = 39,                 /* pop_scope  */
  YYSYMBOL_program = 40,                   /* program  */
  YYSYMBOL_global_list = 41,               /* global_list  */
  YYSYMBOL_function_header = 42,           /* function_header  */
  YYSYMBOL_comma_separated_variables_0 = 43, /* comma_separated_variables_0  */
  YYSYMBOL_comma_separated_variables_1 = 44, /* comma_separated_variables_1  */
  YYSYMBOL_comma_separated_expressions_0 = 45, /* comma_separated_expressions_0  */
  YYSYMBOL_comma_separated_expressions_1 = 46, /* comma_separated_expressions_1  */
  YYSYMBOL_comma_separated_identifiers_1 = 47, /* comma_separated_identifiers_1  */
  YYSYMBOL_variable = 48,                  /* variable  */
  YYSYMBOL_command_list = 49,              /* command_list  */
  YYSYMBOL_expression = 50,                /* expression  */
  YYSYMBOL_expr_7 = 51,                    /* expr_7  */
  YYSYMBOL_expr_6 = 52,                    /* expr_6  */
  YYSYMBOL_expr_5 = 53,                    /* expr_5  */
  YYSYMBOL_expr_4 = 54,                    /* expr_4  */
  YYSYMBOL_expr_3 = 55,                    /* expr_3  */
  YYSYMBOL_expr_2 = 56,                    /* expr_2  */
  YYSYMBOL_expr_1 = 57,                    /* expr_1  */
  YYSYMBOL_expr_v = 58,                    /* expr_v  */
  YYSYMBOL_type = 59                       /* type  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
typedef yytype_int8 yy_state_t;

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if 1

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
#endif /* 1 */

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   140

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  126

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   276


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,     2,     2,     2,    36,     2,     2,
      25,    26,    34,    32,    28,    33,     2,    35,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    22,
      30,    29,    31,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    23,     2,    24,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    86,    86,    88,    92,    96,   100,   104,   106,   110,
     112,   114,   116,   120,   122,   124,   126,   134,   136,   140,
     144,   146,   148,   150,   152,   154,   156,   158,   160,   164,
     166,   168,   170,   172,   174,   176,   178,   180,   182,   184,
     186,   188,   190,   192,   194,   196,   198,   200,   202,   204,
     206,   208,   212,   214,   216,   218,   220,   222,   224,   228,
     229,   230
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TK_PR_INT",
  "TK_PR_FLOAT", "TK_PR_BOOL", "TK_PR_IF", "TK_PR_ELSE", "TK_PR_WHILE",
  "TK_PR_RETURN", "TK_OC_LE", "TK_OC_GE", "TK_OC_EQ", "TK_OC_NE",
  "TK_OC_AND", "TK_OC_OR", "TK_IDENTIFICADOR", "TK_LIT_INT",
  "TK_LIT_FLOAT", "TK_LIT_FALSE", "TK_LIT_TRUE", "TK_ERRO", "';'", "'{'",
  "'}'", "'('", "')'", "'!'", "','", "'='", "'<'", "'>'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "$accept", "push_scope", "pop_scope", "program",
  "global_list", "function_header", "comma_separated_variables_0",
  "comma_separated_variables_1", "comma_separated_expressions_0",
  "comma_separated_expressions_1", "comma_separated_identifiers_1",
  "variable", "command_list", "expression", "expr_7", "expr_6", "expr_5",
  "expr_4", "expr_3", "expr_2", "expr_1", "expr_v", "type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-57)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -57,   -57,    10,     1,   -57,   -57,   -57,   -57,    52,   -57,
       6,    16,    22,    37,   -57,    69,   -57,   -57,    -7,    23,
      52,   -57,    14,   -57,    77,    52,   -57,    70,    71,    87,
      20,   -57,    85,    16,   -57,    82,    87,    87,    86,   -57,
     -57,   -57,   -57,    87,    87,    87,    88,    98,   101,     0,
      67,     7,    65,   -57,   -57,    87,    87,   -57,     3,   100,
      91,    92,    87,    93,   -57,   -57,   -57,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      95,    94,   -57,   102,    38,   -57,   -57,   103,   104,    97,
     -57,   101,     0,    67,    67,     7,     7,     7,     7,    65,
      65,   -57,   -57,   -57,   106,    87,   -57,   -57,   -57,   -57,
     -57,   -57,   -57,   107,    55,    64,   -57,     2,   108,   109,
     -57,   -57,   -57,    78,   111,   -57
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     5,     0,     3,     1,    59,    60,    61,     9,     4,
       0,     0,     0,    10,    11,     0,    20,    17,     0,     0,
       0,    19,     2,     6,     0,     0,    12,     0,     0,     0,
       0,     7,     0,     0,    18,     0,     0,     0,    57,    53,
      54,    56,    55,     0,     0,     0,     0,    29,    30,    32,
      34,    37,    42,    45,    49,    13,     0,    20,     0,     0,
       0,     0,    13,     0,    51,    50,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    14,    15,     0,     2,    21,     8,     0,     0,     0,
      52,    31,    33,    35,    36,    40,    41,    38,    39,    43,
      44,    46,    47,    48,     0,     0,    22,     3,    20,    20,
      58,    23,    16,     0,     2,     2,    28,     0,     0,     0,
      26,    27,    20,     2,     0,    25
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -57,   125,    24,   -57,   -57,   -57,   -57,   -57,    72,   -57,
     105,   115,   -56,   -29,   -57,    73,    68,   -19,    18,    -1,
     -42,   -57,     8
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    32,     9,     2,     3,    10,    12,    13,    80,    81,
      18,    14,    22,    82,    47,    48,    49,    50,    51,    52,
      53,    54,    33
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      46,    84,    64,    65,     5,     6,     7,    60,    61,   119,
       4,    11,    69,    70,    63,    23,    15,     5,     6,     7,
      27,    24,    28,    29,   120,    85,     8,    83,    15,    16,
      30,    24,    17,    35,    25,   101,   102,   103,    31,    75,
      76,     5,     6,     7,    27,    55,    28,    29,    19,    56,
      93,    94,   114,   115,    30,     5,     6,     7,     5,     6,
       7,    27,   107,    28,    29,    20,   123,     5,     6,     7,
      27,    30,    28,    29,    99,   100,   112,    71,    72,   117,
      30,     5,     6,     7,    27,    21,    28,    29,   118,    95,
      96,    97,    98,    34,    30,    36,    37,    73,    74,    77,
      78,    79,   124,    38,    39,    40,    41,    42,    57,    59,
      66,    62,    43,    67,    44,    68,    86,    87,    88,    90,
      45,   104,   105,   110,   106,     1,   108,   109,   111,   116,
     121,   113,   122,   125,    89,    26,    92,     0,    58,     0,
      91
};

static const yytype_int8 yycheck[] =
{
      29,    57,    44,    45,     3,     4,     5,    36,    37,     7,
       0,     3,    12,    13,    43,    22,     8,     3,     4,     5,
       6,    28,     8,     9,    22,    22,    25,    56,    20,    23,
      16,    28,    16,    25,    11,    77,    78,    79,    24,    32,
      33,     3,     4,     5,     6,    25,     8,     9,    26,    29,
      69,    70,   108,   109,    16,     3,     4,     5,     3,     4,
       5,     6,    24,     8,     9,    28,   122,     3,     4,     5,
       6,    16,     8,     9,    75,    76,   105,    10,    11,    24,
      16,     3,     4,     5,     6,    16,     8,     9,    24,    71,
      72,    73,    74,    16,    16,    25,    25,    30,    31,    34,
      35,    36,    24,    16,    17,    18,    19,    20,    23,    27,
      22,    25,    25,    15,    27,    14,    16,    26,    26,    26,
      33,    26,    28,    26,    22,     0,    23,    23,    22,    22,
      22,   107,    23,    22,    62,    20,    68,    -1,    33,    -1,
      67
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    38,    40,    41,     0,     3,     4,     5,    25,    39,
      42,    59,    43,    44,    48,    59,    23,    16,    47,    26,
      28,    16,    49,    22,    28,    11,    48,     6,     8,     9,
      16,    24,    38,    59,    16,    59,    25,    25,    16,    17,
      18,    19,    20,    25,    27,    33,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    25,    29,    23,    47,    27,
      50,    50,    25,    50,    57,    57,    22,    15,    14,    12,
      13,    10,    11,    30,    31,    32,    33,    34,    35,    36,
      45,    46,    50,    50,    49,    22,    16,    26,    26,    45,
      26,    52,    53,    54,    54,    55,    55,    55,    55,    56,
      56,    57,    57,    57,    26,    28,    22,    24,    23,    23,
      26,    22,    50,    39,    49,    49,    22,    24,    24,     7,
      22,    22,    23,    49,    24,    22
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    38,    39,    40,    41,    41,    41,    42,    43,
      43,    44,    44,    45,    45,    46,    46,    47,    47,    48,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    50,
      51,    51,    52,    52,    53,    53,    53,    54,    54,    54,
      54,    54,    55,    55,    55,    56,    56,    56,    56,    57,
      57,    57,    58,    58,    58,    58,    58,    58,    58,    59,
      59,    59
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     0,     3,     0,     4,     5,     7,     0,
       1,     1,     3,     0,     1,     1,     3,     1,     3,     2,
       0,     4,     5,     6,     4,    13,     9,     9,     7,     1,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       2,     2,     3,     1,     1,     1,     1,     1,     4,     1,
       1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
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
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif

#ifndef yytnamerr
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
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
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
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
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
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
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


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* push_scope: %empty  */
#line 87 "parser.y"
    { reduce_push_scope(); }
#line 1485 "parser.tab.c"
    break;

  case 3: /* pop_scope: %empty  */
#line 89 "parser.y"
    { reduce_pop_scope(); }
#line 1491 "parser.tab.c"
    break;

  case 4: /* program: push_scope global_list pop_scope  */
#line 93 "parser.y"
    { (yyval.ast_t) = reduce_program((yyvsp[-1].ast_t)); arvore = (yyval.ast_t); }
#line 1497 "parser.tab.c"
    break;

  case 5: /* global_list: %empty  */
#line 97 "parser.y"
    { (yyval.ast_t) = reduce_global_list_empty(); }
#line 1503 "parser.tab.c"
    break;

  case 6: /* global_list: global_list type comma_separated_identifiers_1 ';'  */
#line 101 "parser.y"
    { (yyval.ast_t) = reduce_global_list_variable((yyvsp[-3].ast_t), (yyvsp[-2].type_t), (yyvsp[-1].list_t)); }
#line 1509 "parser.tab.c"
    break;

  case 7: /* global_list: global_list function_header '{' command_list '}'  */
#line 105 "parser.y"
    { (yyval.ast_t) = reduce_global_list_function((yyvsp[-4].ast_t), (yyvsp[-3].ast_t), (yyvsp[-1].ast_t)); }
#line 1515 "parser.tab.c"
    break;

  case 8: /* function_header: '(' comma_separated_variables_0 ')' TK_OC_GE type '!' TK_IDENTIFICADOR  */
#line 107 "parser.y"
    { (yyval.ast_t) = reduce_function_header((yyvsp[-5].list_t), (yyvsp[-2].type_t), (yyvsp[0].lexeme_t)); }
#line 1521 "parser.tab.c"
    break;

  case 9: /* comma_separated_variables_0: %empty  */
#line 111 "parser.y"
    { (yyval.list_t) = empty_list(); }
#line 1527 "parser.tab.c"
    break;

  case 10: /* comma_separated_variables_0: comma_separated_variables_1  */
#line 113 "parser.y"
    { (yyval.list_t) = (yyvsp[0].list_t); }
#line 1533 "parser.tab.c"
    break;

  case 11: /* comma_separated_variables_1: variable  */
#line 115 "parser.y"
    { (yyval.list_t) = empty_list(); list_push((yyval.list_t), (yyvsp[0].ast_t)); }
#line 1539 "parser.tab.c"
    break;

  case 12: /* comma_separated_variables_1: comma_separated_variables_1 ',' variable  */
#line 117 "parser.y"
    { (yyval.list_t) = (yyvsp[-2].list_t); list_push((yyval.list_t), (yyvsp[0].ast_t)); }
#line 1545 "parser.tab.c"
    break;

  case 13: /* comma_separated_expressions_0: %empty  */
#line 121 "parser.y"
    { (yyval.list_t) = empty_list(); }
#line 1551 "parser.tab.c"
    break;

  case 14: /* comma_separated_expressions_0: comma_separated_expressions_1  */
#line 123 "parser.y"
    { (yyval.list_t) = (yyvsp[0].list_t); }
#line 1557 "parser.tab.c"
    break;

  case 15: /* comma_separated_expressions_1: expression  */
#line 125 "parser.y"
    { (yyval.list_t) = empty_list(); list_push((yyval.list_t), (yyvsp[0].ast_t)); }
#line 1563 "parser.tab.c"
    break;

  case 16: /* comma_separated_expressions_1: comma_separated_expressions_1 ',' expression  */
#line 127 "parser.y"
    { (yyval.list_t) = (yyvsp[-2].list_t); list_push((yyval.list_t), (yyvsp[0].ast_t)); }
#line 1569 "parser.tab.c"
    break;

  case 17: /* comma_separated_identifiers_1: TK_IDENTIFICADOR  */
#line 135 "parser.y"
    { (yyval.list_t) = empty_list(); list_push((yyval.list_t), (yyvsp[0].lexeme_t)); }
#line 1575 "parser.tab.c"
    break;

  case 18: /* comma_separated_identifiers_1: comma_separated_identifiers_1 ',' TK_IDENTIFICADOR  */
#line 137 "parser.y"
    { (yyval.list_t) = (yyvsp[-2].list_t); list_push((yyval.list_t), (yyvsp[0].lexeme_t)); }
#line 1581 "parser.tab.c"
    break;

  case 19: /* variable: type TK_IDENTIFICADOR  */
#line 141 "parser.y"
    { (yyval.ast_t) = reduce_variable((yyvsp[-1].type_t), (yyvsp[0].lexeme_t)); }
#line 1587 "parser.tab.c"
    break;

  case 20: /* command_list: %empty  */
#line 145 "parser.y"
    { (yyval.ast_t) = reduce_command_empty(); }
#line 1593 "parser.tab.c"
    break;

  case 21: /* command_list: command_list type comma_separated_identifiers_1 ';'  */
#line 147 "parser.y"
    { (yyval.ast_t) = reduce_command_variable((yyvsp[-3].ast_t), (yyvsp[-2].type_t), (yyvsp[-1].list_t)); }
#line 1599 "parser.tab.c"
    break;

  case 22: /* command_list: command_list TK_IDENTIFICADOR '=' expression ';'  */
#line 149 "parser.y"
    { (yyval.ast_t) = reduce_command_assignment((yyvsp[-4].ast_t), (yyvsp[-3].lexeme_t), (yyvsp[-1].ast_t)); }
#line 1605 "parser.tab.c"
    break;

  case 23: /* command_list: command_list TK_IDENTIFICADOR '(' comma_separated_expressions_0 ')' ';'  */
#line 151 "parser.y"
    { (yyval.ast_t) = reduce_command_call((yyvsp[-5].ast_t), (yyvsp[-4].lexeme_t), (yyvsp[-2].list_t)); }
#line 1611 "parser.tab.c"
    break;

  case 24: /* command_list: command_list TK_PR_RETURN expression ';'  */
#line 153 "parser.y"
    { (yyval.ast_t) = reduce_command_return((yyvsp[-3].ast_t), (yyvsp[-1].ast_t)); }
#line 1617 "parser.tab.c"
    break;

  case 25: /* command_list: command_list TK_PR_IF '(' expression ')' '{' command_list '}' TK_PR_ELSE '{' command_list '}' ';'  */
#line 155 "parser.y"
    { (yyval.ast_t) = reduce_command_if_else((yyvsp[-12].ast_t), (yyvsp[-9].ast_t), (yyvsp[-6].ast_t), (yyvsp[-2].ast_t)); }
#line 1623 "parser.tab.c"
    break;

  case 26: /* command_list: command_list TK_PR_IF '(' expression ')' '{' command_list '}' ';'  */
#line 157 "parser.y"
    { (yyval.ast_t) = reduce_command_if((yyvsp[-8].ast_t), (yyvsp[-5].ast_t), (yyvsp[-2].ast_t)); }
#line 1629 "parser.tab.c"
    break;

  case 27: /* command_list: command_list TK_PR_WHILE '(' expression ')' '{' command_list '}' ';'  */
#line 159 "parser.y"
    { (yyval.ast_t) = reduce_command_while((yyvsp[-8].ast_t), (yyvsp[-5].ast_t), (yyvsp[-2].ast_t)); }
#line 1635 "parser.tab.c"
    break;

  case 28: /* command_list: command_list push_scope '{' command_list '}' pop_scope ';'  */
#line 161 "parser.y"
    { (yyval.ast_t) = reduce_command_block((yyvsp[-6].ast_t), (yyvsp[-3].ast_t)); }
#line 1641 "parser.tab.c"
    break;

  case 29: /* expression: expr_7  */
#line 165 "parser.y"
    { (yyval.ast_t) = (yyvsp[0].ast_t); }
#line 1647 "parser.tab.c"
    break;

  case 30: /* expr_7: expr_6  */
#line 167 "parser.y"
    { (yyval.ast_t) = (yyvsp[0].ast_t); }
#line 1653 "parser.tab.c"
    break;

  case 31: /* expr_7: expr_7 TK_OC_OR expr_6  */
#line 169 "parser.y"
    { (yyval.ast_t) = reduce_expr_or((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1659 "parser.tab.c"
    break;

  case 32: /* expr_6: expr_5  */
#line 171 "parser.y"
    { (yyval.ast_t) = (yyvsp[0].ast_t); }
#line 1665 "parser.tab.c"
    break;

  case 33: /* expr_6: expr_6 TK_OC_AND expr_5  */
#line 173 "parser.y"
    { (yyval.ast_t) = reduce_expr_and((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1671 "parser.tab.c"
    break;

  case 34: /* expr_5: expr_4  */
#line 175 "parser.y"
    { (yyval.ast_t) = (yyvsp[0].ast_t); }
#line 1677 "parser.tab.c"
    break;

  case 35: /* expr_5: expr_5 TK_OC_EQ expr_4  */
#line 177 "parser.y"
    { (yyval.ast_t) = reduce_expr_eq((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1683 "parser.tab.c"
    break;

  case 36: /* expr_5: expr_5 TK_OC_NE expr_4  */
#line 179 "parser.y"
    { (yyval.ast_t) = reduce_expr_ne((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1689 "parser.tab.c"
    break;

  case 37: /* expr_4: expr_3  */
#line 181 "parser.y"
    { (yyval.ast_t) = (yyvsp[0].ast_t); }
#line 1695 "parser.tab.c"
    break;

  case 38: /* expr_4: expr_4 '<' expr_3  */
#line 183 "parser.y"
    { (yyval.ast_t) = reduce_expr_lt((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1701 "parser.tab.c"
    break;

  case 39: /* expr_4: expr_4 '>' expr_3  */
#line 185 "parser.y"
    { (yyval.ast_t) = reduce_expr_gt((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1707 "parser.tab.c"
    break;

  case 40: /* expr_4: expr_4 TK_OC_LE expr_3  */
#line 187 "parser.y"
    { (yyval.ast_t) = reduce_expr_le((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1713 "parser.tab.c"
    break;

  case 41: /* expr_4: expr_4 TK_OC_GE expr_3  */
#line 189 "parser.y"
    { (yyval.ast_t) = reduce_expr_ge((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1719 "parser.tab.c"
    break;

  case 42: /* expr_3: expr_2  */
#line 191 "parser.y"
    { (yyval.ast_t) = (yyvsp[0].ast_t); }
#line 1725 "parser.tab.c"
    break;

  case 43: /* expr_3: expr_3 '+' expr_2  */
#line 193 "parser.y"
    { (yyval.ast_t) = reduce_expr_add((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1731 "parser.tab.c"
    break;

  case 44: /* expr_3: expr_3 '-' expr_2  */
#line 195 "parser.y"
    { (yyval.ast_t) = reduce_expr_sub((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1737 "parser.tab.c"
    break;

  case 45: /* expr_2: expr_1  */
#line 197 "parser.y"
    { (yyval.ast_t) = (yyvsp[0].ast_t); }
#line 1743 "parser.tab.c"
    break;

  case 46: /* expr_2: expr_2 '*' expr_1  */
#line 199 "parser.y"
    { (yyval.ast_t) = reduce_expr_mul((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1749 "parser.tab.c"
    break;

  case 47: /* expr_2: expr_2 '/' expr_1  */
#line 201 "parser.y"
    { (yyval.ast_t) = reduce_expr_div((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1755 "parser.tab.c"
    break;

  case 48: /* expr_2: expr_2 '%' expr_1  */
#line 203 "parser.y"
    { (yyval.ast_t) = reduce_expr_mod((yyvsp[-2].ast_t), (yyvsp[0].ast_t)); }
#line 1761 "parser.tab.c"
    break;

  case 49: /* expr_1: expr_v  */
#line 205 "parser.y"
    { (yyval.ast_t) = (yyvsp[0].ast_t); }
#line 1767 "parser.tab.c"
    break;

  case 50: /* expr_1: '-' expr_1  */
#line 207 "parser.y"
    { (yyval.ast_t) = reduce_expr_inv((yyvsp[0].ast_t)); }
#line 1773 "parser.tab.c"
    break;

  case 51: /* expr_1: '!' expr_1  */
#line 209 "parser.y"
    { (yyval.ast_t) = reduce_expr_not((yyvsp[0].ast_t)); }
#line 1779 "parser.tab.c"
    break;

  case 52: /* expr_v: '(' expression ')'  */
#line 213 "parser.y"
    { (yyval.ast_t) = (yyvsp[-1].ast_t); }
#line 1785 "parser.tab.c"
    break;

  case 53: /* expr_v: TK_LIT_INT  */
#line 215 "parser.y"
    { (yyval.ast_t) = reduce_expr_int((yyvsp[0].lexeme_t)); }
#line 1791 "parser.tab.c"
    break;

  case 54: /* expr_v: TK_LIT_FLOAT  */
#line 217 "parser.y"
    { (yyval.ast_t) = reduce_expr_float((yyvsp[0].lexeme_t)); }
#line 1797 "parser.tab.c"
    break;

  case 55: /* expr_v: TK_LIT_TRUE  */
#line 219 "parser.y"
    { (yyval.ast_t) = reduce_expr_bool((yyvsp[0].lexeme_t)); }
#line 1803 "parser.tab.c"
    break;

  case 56: /* expr_v: TK_LIT_FALSE  */
#line 221 "parser.y"
    { (yyval.ast_t) = reduce_expr_bool((yyvsp[0].lexeme_t)); }
#line 1809 "parser.tab.c"
    break;

  case 57: /* expr_v: TK_IDENTIFICADOR  */
#line 223 "parser.y"
    { (yyval.ast_t) = reduce_expr_ident((yyvsp[0].lexeme_t)); }
#line 1815 "parser.tab.c"
    break;

  case 58: /* expr_v: TK_IDENTIFICADOR '(' comma_separated_expressions_0 ')'  */
#line 225 "parser.y"
    { (yyval.ast_t) = reduce_expr_call((yyvsp[-3].lexeme_t), (yyvsp[-1].list_t)); }
#line 1821 "parser.tab.c"
    break;

  case 59: /* type: TK_PR_INT  */
#line 228 "parser.y"
                { (yyval.type_t) = type_int; }
#line 1827 "parser.tab.c"
    break;

  case 60: /* type: TK_PR_FLOAT  */
#line 229 "parser.y"
                  { (yyval.type_t) = type_float; }
#line 1833 "parser.tab.c"
    break;

  case 61: /* type: TK_PR_BOOL  */
#line 230 "parser.y"
                 { (yyval.type_t) = type_bool; }
#line 1839 "parser.tab.c"
    break;


#line 1843 "parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 232 "parser.y"


void yyerror (char const *mensagem) {
    // A seguinte linha de código demontra como apresentar a coluna do erro, 
    // functionalidade que nao foi habilitada para seguir a especificacao com mais regor:
    // printf("Erro na linha %d, coluna %d: \n - %s\n", get_line_number(), get_col_number(), mensagem);
    fprintf(stderr, "Erro na linha %d: \n - %s\n", get_line_number(), mensagem);
}
