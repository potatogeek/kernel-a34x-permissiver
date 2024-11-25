// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#include "perf.h"
#include "util/util.h"
#include "util/debug.h"
#include <subcmd/parse-options.h>
#include "util/parse-regs-options.h"

int
parse_regs(const struct option *opt, const char *str, int unset)
{
	uint64_t *mode = (uint64_t *)opt->value;
	const struct sample_reg *r;
	char *s, *os = NULL, *p;
	int ret = -1;
=======
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "util/debug.h"
#include <subcmd/parse-options.h>
#include "util/perf_regs.h"
#include "util/parse-regs-options.h"

static int
__parse_regs(const struct option *opt, const char *str, int unset, bool intr)
{
	uint64_t *mode = (uint64_t *)opt->value;
	const struct sample_reg *r = NULL;
	char *s, *os = NULL, *p;
	int ret = -1;
	uint64_t mask;
>>>>>>> upstream/android-13

	if (unset)
		return 0;

	/*
	 * cannot set it twice
	 */
	if (*mode)
		return -1;

<<<<<<< HEAD
=======
	if (intr)
		mask = arch__intr_reg_mask();
	else
		mask = arch__user_reg_mask();

>>>>>>> upstream/android-13
	/* str may be NULL in case no arg is passed to -I */
	if (str) {
		/* because str is read-only */
		s = os = strdup(str);
		if (!s)
			return -1;

		for (;;) {
			p = strchr(s, ',');
			if (p)
				*p = '\0';

			if (!strcmp(s, "?")) {
				fprintf(stderr, "available registers: ");
<<<<<<< HEAD
				for (r = sample_reg_masks; r->name; r++) {
					fprintf(stderr, "%s ", r->name);
				}
=======
#ifdef HAVE_PERF_REGS_SUPPORT
				for (r = sample_reg_masks; r->name; r++) {
					if (r->mask & mask)
						fprintf(stderr, "%s ", r->name);
				}
#endif
>>>>>>> upstream/android-13
				fputc('\n', stderr);
				/* just printing available regs */
				goto error;
			}
<<<<<<< HEAD
			for (r = sample_reg_masks; r->name; r++) {
				if (!strcasecmp(s, r->name))
					break;
			}
			if (!r->name) {
				ui__warning("unknown register %s,"
					    " check man page\n", s);
=======
#ifdef HAVE_PERF_REGS_SUPPORT
			for (r = sample_reg_masks; r->name; r++) {
				if ((r->mask & mask) && !strcasecmp(s, r->name))
					break;
			}
#endif
			if (!r || !r->name) {
				ui__warning("Unknown register \"%s\", check man page or run \"perf record %s?\"\n",
					    s, intr ? "-I" : "--user-regs=");
>>>>>>> upstream/android-13
				goto error;
			}

			*mode |= r->mask;

			if (!p)
				break;

			s = p + 1;
		}
	}
	ret = 0;

	/* default to all possible regs */
	if (*mode == 0)
<<<<<<< HEAD
		*mode = PERF_REGS_MASK;
=======
		*mode = mask;
>>>>>>> upstream/android-13
error:
	free(os);
	return ret;
}
<<<<<<< HEAD
=======

int
parse_user_regs(const struct option *opt, const char *str, int unset)
{
	return __parse_regs(opt, str, unset, false);
}

int
parse_intr_regs(const struct option *opt, const char *str, int unset)
{
	return __parse_regs(opt, str, unset, true);
}
>>>>>>> upstream/android-13
