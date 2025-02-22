/*
 * Copyright (C) 2014 Stefan Kristiansson <stefan.kristiansson@saunalahti.fi>
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#ifndef __ASM_OPENRISC_ATOMIC_H
#define __ASM_OPENRISC_ATOMIC_H

#include <linux/types.h>

/* Atomically perform op with v->counter and i */
#define ATOMIC_OP(op)							\
<<<<<<< HEAD
static inline void atomic_##op(int i, atomic_t *v)			\
=======
static inline void arch_atomic_##op(int i, atomic_t *v)			\
>>>>>>> upstream/android-13
{									\
	int tmp;							\
									\
	__asm__ __volatile__(						\
		"1:	l.lwa	%0,0(%1)	\n"			\
		"	l." #op " %0,%0,%2	\n"			\
		"	l.swa	0(%1),%0	\n"			\
		"	l.bnf	1b		\n"			\
		"	 l.nop			\n"			\
		: "=&r"(tmp)						\
		: "r"(&v->counter), "r"(i)				\
		: "cc", "memory");					\
}

/* Atomically perform op with v->counter and i, return the result */
#define ATOMIC_OP_RETURN(op)						\
<<<<<<< HEAD
static inline int atomic_##op##_return(int i, atomic_t *v)		\
=======
static inline int arch_atomic_##op##_return(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	int tmp;							\
									\
	__asm__ __volatile__(						\
		"1:	l.lwa	%0,0(%1)	\n"			\
		"	l." #op " %0,%0,%2	\n"			\
		"	l.swa	0(%1),%0	\n"			\
		"	l.bnf	1b		\n"			\
		"	 l.nop			\n"			\
		: "=&r"(tmp)						\
		: "r"(&v->counter), "r"(i)				\
		: "cc", "memory");					\
									\
	return tmp;							\
}

/* Atomically perform op with v->counter and i, return orig v->counter */
#define ATOMIC_FETCH_OP(op)						\
<<<<<<< HEAD
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
=======
static inline int arch_atomic_fetch_##op(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	int tmp, old;							\
									\
	__asm__ __volatile__(						\
		"1:	l.lwa	%0,0(%2)	\n"			\
		"	l." #op " %1,%0,%3	\n"			\
		"	l.swa	0(%2),%1	\n"			\
		"	l.bnf	1b		\n"			\
		"	 l.nop			\n"			\
		: "=&r"(old), "=&r"(tmp)				\
		: "r"(&v->counter), "r"(i)				\
		: "cc", "memory");					\
									\
	return old;							\
}

ATOMIC_OP_RETURN(add)
ATOMIC_OP_RETURN(sub)

ATOMIC_FETCH_OP(add)
ATOMIC_FETCH_OP(sub)
ATOMIC_FETCH_OP(and)
ATOMIC_FETCH_OP(or)
ATOMIC_FETCH_OP(xor)

<<<<<<< HEAD
=======
ATOMIC_OP(add)
ATOMIC_OP(sub)
>>>>>>> upstream/android-13
ATOMIC_OP(and)
ATOMIC_OP(or)
ATOMIC_OP(xor)

#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP

<<<<<<< HEAD
#define atomic_add_return	atomic_add_return
#define atomic_sub_return	atomic_sub_return
#define atomic_fetch_add	atomic_fetch_add
#define atomic_fetch_sub	atomic_fetch_sub
#define atomic_fetch_and	atomic_fetch_and
#define atomic_fetch_or		atomic_fetch_or
#define atomic_fetch_xor	atomic_fetch_xor
#define atomic_and	atomic_and
#define atomic_or	atomic_or
#define atomic_xor	atomic_xor
=======
#define arch_atomic_add_return	arch_atomic_add_return
#define arch_atomic_sub_return	arch_atomic_sub_return
#define arch_atomic_fetch_add	arch_atomic_fetch_add
#define arch_atomic_fetch_sub	arch_atomic_fetch_sub
#define arch_atomic_fetch_and	arch_atomic_fetch_and
#define arch_atomic_fetch_or	arch_atomic_fetch_or
#define arch_atomic_fetch_xor	arch_atomic_fetch_xor
#define arch_atomic_add		arch_atomic_add
#define arch_atomic_sub		arch_atomic_sub
#define arch_atomic_and		arch_atomic_and
#define arch_atomic_or		arch_atomic_or
#define arch_atomic_xor		arch_atomic_xor
>>>>>>> upstream/android-13

/*
 * Atomically add a to v->counter as long as v is not already u.
 * Returns the original value at v->counter.
 *
 * This is often used through atomic_inc_not_zero()
 */
<<<<<<< HEAD
static inline int atomic_fetch_add_unless(atomic_t *v, int a, int u)
=======
static inline int arch_atomic_fetch_add_unless(atomic_t *v, int a, int u)
>>>>>>> upstream/android-13
{
	int old, tmp;

	__asm__ __volatile__(
		"1:	l.lwa %0, 0(%2)		\n"
		"	l.sfeq %0, %4		\n"
		"	l.bf 2f			\n"
		"	 l.add %1, %0, %3	\n"
		"	l.swa 0(%2), %1		\n"
		"	l.bnf 1b		\n"
		"	 l.nop			\n"
		"2:				\n"
		: "=&r"(old), "=&r" (tmp)
		: "r"(&v->counter), "r"(a), "r"(u)
		: "cc", "memory");

	return old;
}
<<<<<<< HEAD
#define atomic_fetch_add_unless	atomic_fetch_add_unless

#include <asm-generic/atomic.h>
=======
#define arch_atomic_fetch_add_unless	arch_atomic_fetch_add_unless

#define arch_atomic_read(v)		READ_ONCE((v)->counter)
#define arch_atomic_set(v,i)		WRITE_ONCE((v)->counter, (i))

#include <asm/cmpxchg.h>

#define arch_atomic_xchg(ptr, v)		(arch_xchg(&(ptr)->counter, (v)))
#define arch_atomic_cmpxchg(v, old, new)	(arch_cmpxchg(&((v)->counter), (old), (new)))
>>>>>>> upstream/android-13

#endif /* __ASM_OPENRISC_ATOMIC_H */
