/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_COREDUMP_H
#define _LINUX_COREDUMP_H

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <asm/siginfo.h>

<<<<<<< HEAD
=======
#ifdef CONFIG_COREDUMP
struct core_vma_metadata {
	unsigned long start, end;
	unsigned long flags;
	unsigned long dump_size;
	unsigned long pgoff;
	struct file   *file;
};

extern int core_uses_pid;
extern char core_pattern[];
extern unsigned int core_pipe_limit;

>>>>>>> upstream/android-13
/*
 * These are the only things you should do on a core-file: use only these
 * functions to write out all the necessary info.
 */
struct coredump_params;
<<<<<<< HEAD
extern int dump_skip(struct coredump_params *cprm, size_t nr);
extern int dump_emit(struct coredump_params *cprm, const void *addr, int nr);
extern int dump_align(struct coredump_params *cprm, int align);
extern void dump_truncate(struct coredump_params *cprm);
#ifdef CONFIG_COREDUMP
extern void do_coredump(const siginfo_t *siginfo);
#else
static inline void do_coredump(const siginfo_t *siginfo) {}
=======
extern void dump_skip_to(struct coredump_params *cprm, unsigned long to);
extern void dump_skip(struct coredump_params *cprm, size_t nr);
extern int dump_emit(struct coredump_params *cprm, const void *addr, int nr);
extern int dump_align(struct coredump_params *cprm, int align);
int dump_user_range(struct coredump_params *cprm, unsigned long start,
		    unsigned long len);
extern void do_coredump(const kernel_siginfo_t *siginfo);
#else
static inline void do_coredump(const kernel_siginfo_t *siginfo) {}
>>>>>>> upstream/android-13
#endif

#endif /* _LINUX_COREDUMP_H */
