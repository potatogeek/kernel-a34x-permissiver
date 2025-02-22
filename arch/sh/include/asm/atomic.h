/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_ATOMIC_H
#define __ASM_SH_ATOMIC_H

#if defined(CONFIG_CPU_J2)

#include <asm-generic/atomic.h>

#else

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 *
 */

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/cmpxchg.h>
#include <asm/barrier.h>

<<<<<<< HEAD
#define ATOMIC_INIT(i)	{ (i) }

#define atomic_read(v)		READ_ONCE((v)->counter)
#define atomic_set(v,i)		WRITE_ONCE((v)->counter, (i))
=======
#define arch_atomic_read(v)		READ_ONCE((v)->counter)
#define arch_atomic_set(v,i)		WRITE_ONCE((v)->counter, (i))
>>>>>>> upstream/android-13

#if defined(CONFIG_GUSA_RB)
#include <asm/atomic-grb.h>
#elif defined(CONFIG_CPU_SH4A)
#include <asm/atomic-llsc.h>
#else
#include <asm/atomic-irq.h>
#endif

<<<<<<< HEAD
#define atomic_xchg(v, new)		(xchg(&((v)->counter), new))
#define atomic_cmpxchg(v, o, n)		(cmpxchg(&((v)->counter), (o), (n)))
=======
#define arch_atomic_xchg(v, new)	(arch_xchg(&((v)->counter), new))
#define arch_atomic_cmpxchg(v, o, n)	(arch_cmpxchg(&((v)->counter), (o), (n)))
>>>>>>> upstream/android-13

#endif /* CONFIG_CPU_J2 */

#endif /* __ASM_SH_ATOMIC_H */
