/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_LINKAGE_H
#define _ASM_X86_LINKAGE_H

#include <linux/stringify.h>

#undef notrace
#define notrace __attribute__((no_instrument_function))

#ifdef CONFIG_X86_32
#define asmlinkage CPP_ASMLINKAGE __attribute__((regparm(0)))
#endif /* CONFIG_X86_32 */

#ifdef __ASSEMBLY__

<<<<<<< HEAD
#define GLOBAL(name)	\
	.globl name;	\
	name:

=======
>>>>>>> upstream/android-13
#if defined(CONFIG_X86_64) || defined(CONFIG_X86_ALIGNMENT_16)
#define __ALIGN		.p2align 4, 0x90
#define __ALIGN_STR	__stringify(__ALIGN)
#endif

<<<<<<< HEAD
=======
#ifdef CONFIG_SLS
#define RET	ret; int3
#else
#define RET	ret
#endif

#else /* __ASSEMBLY__ */

#ifdef CONFIG_SLS
#define ASM_RET	"ret; int3\n\t"
#else
#define ASM_RET	"ret\n\t"
#endif

>>>>>>> upstream/android-13
#endif /* __ASSEMBLY__ */

#endif /* _ASM_X86_LINKAGE_H */

