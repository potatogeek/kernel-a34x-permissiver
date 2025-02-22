/* Simple expression parser */
%{
<<<<<<< HEAD
#include "util.h"
#include "util/debug.h"
=======
#define YYDEBUG 1
#include <stdio.h>
#include "util.h"
#include "util/debug.h"
#include <stdlib.h> // strtod()
>>>>>>> upstream/android-13
#define IN_EXPR_Y 1
#include "expr.h"
#include "smt.h"
#include <string.h>

<<<<<<< HEAD
#define MAXIDLEN 256
=======
static double d_ratio(double val0, double val1)
{
	if (val1 == 0) {
		return 0;
	}
	return  val0 / val1;
}

>>>>>>> upstream/android-13
%}

%define api.pure full

%parse-param { double *final_val }
<<<<<<< HEAD
%parse-param { struct parse_ctx *ctx }
%parse-param { const char **pp }
%lex-param { const char **pp }

%union {
	double num;
	char id[MAXIDLEN+1];
}

%token <num> NUMBER
%token <id> ID
%token MIN MAX IF ELSE SMT_ON
=======
%parse-param { struct expr_parse_ctx *ctx }
%parse-param {void *scanner}
%lex-param {void* scanner}

%union {
	double	 num;
	char	*str;
}

%token EXPR_PARSE EXPR_OTHER EXPR_ERROR
%token <num> NUMBER
%token <str> ID
%destructor { free ($$); } <str>
%token MIN MAX IF ELSE SMT_ON D_RATIO
>>>>>>> upstream/android-13
%left MIN MAX IF
%left '|'
%left '^'
%left '&'
<<<<<<< HEAD
=======
%left '<' '>'
>>>>>>> upstream/android-13
%left '-' '+'
%left '*' '/' '%'
%left NEG NOT
%type <num> expr if_expr

%{
<<<<<<< HEAD
static int expr__lex(YYSTYPE *res, const char **pp);

static void expr__error(double *final_val __maybe_unused,
		       struct parse_ctx *ctx __maybe_unused,
		       const char **pp __maybe_unused,
=======
static void expr_error(double *final_val __maybe_unused,
		       struct expr_parse_ctx *ctx __maybe_unused,
		       void *scanner,
>>>>>>> upstream/android-13
		       const char *s)
{
	pr_debug("%s\n", s);
}

<<<<<<< HEAD
static int lookup_id(struct parse_ctx *ctx, char *id, double *val)
{
	int i;

	for (i = 0; i < ctx->num_ids; i++) {
		if (!strcasecmp(ctx->ids[i].name, id)) {
			*val = ctx->ids[i].val;
			return 0;
		}
	}
	return -1;
}

%}
%%

=======
%}
%%

start:
EXPR_PARSE all_expr
|
EXPR_OTHER all_other

all_other: all_other other
|

other: ID
{
	expr__add_id(ctx, $1);
}
|
MIN | MAX | IF | ELSE | SMT_ON | NUMBER | '|' | '^' | '&' | '-' | '+' | '*' | '/' | '%' | '(' | ')' | ','
|
'<' | '>' | D_RATIO

>>>>>>> upstream/android-13
all_expr: if_expr			{ *final_val = $1; }
	;

if_expr:
	expr IF expr ELSE expr { $$ = $3 ? $1 : $5; }
	| expr
	;

expr:	  NUMBER
<<<<<<< HEAD
	| ID			{ if (lookup_id(ctx, $1, &$$) < 0) {
					pr_debug("%s not found\n", $1);
					YYABORT;
				  }
=======
	| ID			{
					struct expr_id_data *data;

					if (expr__resolve_id(ctx, $1, &data)) {
						free($1);
						YYABORT;
					}

					$$ = expr_id_data__value(data);
					free($1);
>>>>>>> upstream/android-13
				}
	| expr '|' expr		{ $$ = (long)$1 | (long)$3; }
	| expr '&' expr		{ $$ = (long)$1 & (long)$3; }
	| expr '^' expr		{ $$ = (long)$1 ^ (long)$3; }
<<<<<<< HEAD
	| expr '+' expr		{ $$ = $1 + $3; }
	| expr '-' expr		{ $$ = $1 - $3; }
	| expr '*' expr		{ $$ = $1 * $3; }
	| expr '/' expr		{ if ($3 == 0) YYABORT; $$ = $1 / $3; }
	| expr '%' expr		{ if ((long)$3 == 0) YYABORT; $$ = (long)$1 % (long)$3; }
=======
	| expr '<' expr		{ $$ = $1 < $3; }
	| expr '>' expr		{ $$ = $1 > $3; }
	| expr '+' expr		{ $$ = $1 + $3; }
	| expr '-' expr		{ $$ = $1 - $3; }
	| expr '*' expr		{ $$ = $1 * $3; }
	| expr '/' expr		{ if ($3 == 0) {
					pr_debug("division by zero\n");
					YYABORT;
				  }
				  $$ = $1 / $3;
	                        }
	| expr '%' expr		{ if ((long)$3 == 0) {
					pr_debug("division by zero\n");
					YYABORT;
				  }
				  $$ = (long)$1 % (long)$3;
	                        }
>>>>>>> upstream/android-13
	| '-' expr %prec NEG	{ $$ = -$2; }
	| '(' if_expr ')'	{ $$ = $2; }
	| MIN '(' expr ',' expr ')' { $$ = $3 < $5 ? $3 : $5; }
	| MAX '(' expr ',' expr ')' { $$ = $3 > $5 ? $3 : $5; }
	| SMT_ON		 { $$ = smt_on() > 0; }
<<<<<<< HEAD
	;

%%

static int expr__symbol(YYSTYPE *res, const char *p, const char **pp)
{
	char *dst = res->id;
	const char *s = p;

	if (*p == '#')
		*dst++ = *p++;

	while (isalnum(*p) || *p == '_' || *p == '.' || *p == ':' || *p == '@' || *p == '\\') {
		if (p - s >= MAXIDLEN)
			return -1;
		/*
		 * Allow @ instead of / to be able to specify pmu/event/ without
		 * conflicts with normal division.
		 */
		if (*p == '@')
			*dst++ = '/';
		else if (*p == '\\')
			*dst++ = *++p;
		else
			*dst++ = *p;
		p++;
	}
	*dst = 0;
	*pp = p;
	dst = res->id;
	switch (dst[0]) {
	case 'm':
		if (!strcmp(dst, "min"))
			return MIN;
		if (!strcmp(dst, "max"))
			return MAX;
		break;
	case 'i':
		if (!strcmp(dst, "if"))
			return IF;
		break;
	case 'e':
		if (!strcmp(dst, "else"))
			return ELSE;
		break;
	case '#':
		if (!strcasecmp(dst, "#smt_on"))
			return SMT_ON;
		break;
	}
	return ID;
}

static int expr__lex(YYSTYPE *res, const char **pp)
{
	int tok;
	const char *s;
	const char *p = *pp;

	while (isspace(*p))
		p++;
	s = p;
	switch (*p++) {
	case '#':
	case 'a' ... 'z':
	case 'A' ... 'Z':
		return expr__symbol(res, p - 1, pp);
	case '0' ... '9': case '.':
		res->num = strtod(s, (char **)&p);
		tok = NUMBER;
		break;
	default:
		tok = *s;
		break;
	}
	*pp = p;
	return tok;
}

/* Caller must make sure id is allocated */
void expr__add_id(struct parse_ctx *ctx, const char *name, double val)
{
	int idx;
	assert(ctx->num_ids < MAX_PARSE_ID);
	idx = ctx->num_ids++;
	ctx->ids[idx].name = name;
	ctx->ids[idx].val = val;
}

void expr__ctx_init(struct parse_ctx *ctx)
{
	ctx->num_ids = 0;
}

static bool already_seen(const char *val, const char *one, const char **other,
			 int num_other)
{
	int i;

	if (one && !strcasecmp(one, val))
		return true;
	for (i = 0; i < num_other; i++)
		if (!strcasecmp(other[i], val))
			return true;
	return false;
}

int expr__find_other(const char *p, const char *one, const char ***other,
		     int *num_otherp)
{
	const char *orig = p;
	int err = -1;
	int num_other;

	*other = malloc((EXPR_MAX_OTHER + 1) * sizeof(char *));
	if (!*other)
		return -1;

	num_other = 0;
	for (;;) {
		YYSTYPE val;
		int tok = expr__lex(&val, &p);
		if (tok == 0) {
			err = 0;
			break;
		}
		if (tok == ID && !already_seen(val.id, one, *other, num_other)) {
			if (num_other >= EXPR_MAX_OTHER - 1) {
				pr_debug("Too many extra events in %s\n", orig);
				break;
			}
			(*other)[num_other] = strdup(val.id);
			if (!(*other)[num_other])
				return -1;
			num_other++;
		}
	}
	(*other)[num_other] = NULL;
	*num_otherp = num_other;
	if (err) {
		*num_otherp = 0;
		free(*other);
		*other = NULL;
	}
	return err;
}
=======
	| D_RATIO '(' expr ',' expr ')' { $$ = d_ratio($3,$5); }
	;

%%
>>>>>>> upstream/android-13
