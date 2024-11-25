/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ARCH_SPARC64_PERCPU__
#define __ARCH_SPARC64_PERCPU__

#include <linux/compiler.h>

<<<<<<< HEAD
register unsigned long __local_per_cpu_offset asm("g5");
=======
#ifndef BUILD_VDSO
register unsigned long __local_per_cpu_offset asm("g5");
#endif
>>>>>>> upstream/android-13

#ifdef CONFIG_SMP

#include <asm/trap_block.h>

#define __per_cpu_offset(__cpu) \
	(trap_block[(__cpu)].__per_cpu_base)
#define per_cpu_offset(x) (__per_cpu_offset(x))

#define __my_cpu_offset __local_per_cpu_offset

#else /* ! SMP */

#endif	/* SMP */

#include <asm-generic/percpu.h>

#endif /* __ARCH_SPARC64_PERCPU__ */
