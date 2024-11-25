/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PERF_PARSE_REGS_OPTIONS_H
#define _PERF_PARSE_REGS_OPTIONS_H 1
struct option;
<<<<<<< HEAD
int parse_regs(const struct option *opt, const char *str, int unset);
=======
int parse_user_regs(const struct option *opt, const char *str, int unset);
int parse_intr_regs(const struct option *opt, const char *str, int unset);
>>>>>>> upstream/android-13
#endif /* _PERF_PARSE_REGS_OPTIONS_H */
