/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_COREDUMP_H
#define _LINUX_COREDUMP_H

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <asm/siginfo.h>


struct coredump_params;
extern int dump_skip(struct coredump_params *cprm, size_t nr);
extern int dump_emit(struct coredump_params *cprm, const void *addr, int nr);
extern int dump_align(struct coredump_params *cprm, int align);
extern void dump_truncate(struct coredump_params *cprm);
#ifdef CONFIG_COREDUMP
extern void do_coredump(const siginfo_t *siginfo);
#else
static inline void do_coredump(const siginfo_t *siginfo) {}
#endif

#endif 
