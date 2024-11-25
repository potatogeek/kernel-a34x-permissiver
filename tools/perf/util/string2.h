/* SPDX-License-Identifier: GPL-2.0 */
#ifndef PERF_STRING_H
#define PERF_STRING_H

<<<<<<< HEAD
#include <linux/types.h>
#include <stddef.h>
#include <string.h>

s64 perf_atoll(const char *str);
char **argv_split(const char *str, int *argcp);
void argv_free(char **argv);
=======
#include <linux/string.h>
#include <linux/types.h>
#include <sys/types.h> // pid_t
#include <stddef.h>
#include <string.h>

extern const char *graph_dotted_line;
extern const char *dots;

s64 perf_atoll(const char *str);
>>>>>>> upstream/android-13
bool strglobmatch(const char *str, const char *pat);
bool strglobmatch_nocase(const char *str, const char *pat);
bool strlazymatch(const char *str, const char *pat);
static inline bool strisglob(const char *str)
{
	return strpbrk(str, "*?[") != NULL;
}
int strtailcmp(const char *s1, const char *s2);
<<<<<<< HEAD
char *strxfrchar(char *s, char from, char to);

char *ltrim(char *s);
char *rtrim(char *s);

static inline char *trim(char *s)
{
	return ltrim(rtrim(s));
}
=======
>>>>>>> upstream/android-13

char *asprintf_expr_inout_ints(const char *var, bool in, size_t nints, int *ints);

static inline char *asprintf_expr_in_ints(const char *var, size_t nints, int *ints)
{
	return asprintf_expr_inout_ints(var, true, nints, ints);
}

static inline char *asprintf_expr_not_in_ints(const char *var, size_t nints, int *ints)
{
	return asprintf_expr_inout_ints(var, false, nints, ints);
}

<<<<<<< HEAD
char *strpbrk_esc(char *str, const char *stopset);
char *strdup_esc(const char *str);

=======
char *asprintf__tp_filter_pids(size_t npids, pid_t *pids);

char *strpbrk_esc(char *str, const char *stopset);
char *strdup_esc(const char *str);

unsigned int hex(char c);

>>>>>>> upstream/android-13
#endif /* PERF_STRING_H */
