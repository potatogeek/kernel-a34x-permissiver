/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ELFCORE_COMPAT_H
#define _LINUX_ELFCORE_COMPAT_H

#include <linux/elf.h>
#include <linux/elfcore.h>
#include <linux/compat.h>

/*
 * Make sure these layouts match the linux/elfcore.h native definitions.
 */

struct compat_elf_siginfo
{
	compat_int_t			si_signo;
	compat_int_t			si_code;
	compat_int_t			si_errno;
};

<<<<<<< HEAD
struct compat_elf_prstatus
=======
struct compat_elf_prstatus_common
>>>>>>> upstream/android-13
{
	struct compat_elf_siginfo	pr_info;
	short				pr_cursig;
	compat_ulong_t			pr_sigpend;
	compat_ulong_t			pr_sighold;
	compat_pid_t			pr_pid;
	compat_pid_t			pr_ppid;
	compat_pid_t			pr_pgrp;
	compat_pid_t			pr_sid;
<<<<<<< HEAD
	struct compat_timeval		pr_utime;
	struct compat_timeval		pr_stime;
	struct compat_timeval		pr_cutime;
	struct compat_timeval		pr_cstime;
	compat_elf_gregset_t		pr_reg;
#ifdef CONFIG_BINFMT_ELF_FDPIC
	compat_ulong_t			pr_exec_fdpic_loadmap;
	compat_ulong_t			pr_interp_fdpic_loadmap;
#endif
	compat_int_t			pr_fpvalid;
=======
	struct old_timeval32		pr_utime;
	struct old_timeval32		pr_stime;
	struct old_timeval32		pr_cutime;
	struct old_timeval32		pr_cstime;
>>>>>>> upstream/android-13
};

struct compat_elf_prpsinfo
{
	char				pr_state;
	char				pr_sname;
	char				pr_zomb;
	char				pr_nice;
	compat_ulong_t			pr_flag;
	__compat_uid_t			pr_uid;
	__compat_gid_t			pr_gid;
	compat_pid_t			pr_pid, pr_ppid, pr_pgrp, pr_sid;
	char				pr_fname[16];
	char				pr_psargs[ELF_PRARGSZ];
};

<<<<<<< HEAD
=======
#ifdef CONFIG_ARCH_HAS_ELFCORE_COMPAT
#include <asm/elfcore-compat.h>
#endif

struct compat_elf_prstatus
{
	struct compat_elf_prstatus_common	common;
	compat_elf_gregset_t		pr_reg;
	compat_int_t			pr_fpvalid;
};

>>>>>>> upstream/android-13
#endif /* _LINUX_ELFCORE_COMPAT_H */
