/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_IA64_ATOMIC_H
#define _ASM_IA64_ATOMIC_H

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 *
 * NOTE: don't mess with the types below!  The "unsigned long" and
 * "int" types were carefully placed so as to ensure proper operation
 * of the macros.
 *
 * Copyright (C) 1998, 1999, 2002-2003 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */
#include <linux/types.h>

#include <asm/intrinsics.h>
#include <asm/barrier.h>


<<<<<<< HEAD
#define ATOMIC_INIT(i)		{ (i) }
#define ATOMIC64_INIT(i)	{ (i) }

#define atomic_read(v)		READ_ONCE((v)->counter)
#define atomic64_read(v)	READ_ONCE((v)->counter)

#define atomic_set(v,i)		WRITE_ONCE(((v)->counter), (i))
#define atomic64_set(v,i)	WRITE_ONCE(((v)->counter), (i))
=======
#define ATOMIC64_INIT(i)	{ (i) }

#define arch_atomic_read(v)	READ_ONCE((v)->counter)
#define arch_atomic64_read(v)	READ_ONCE((v)->counter)

#define arch_atomic_set(v,i)	WRITE_ONCE(((v)->counter), (i))
#define arch_atomic64_set(v,i)	WRITE_ONCE(((v)->counter), (i))
>>>>>>> upstream/android-13

#define ATOMIC_OP(op, c_op)						\
static __inline__ int							\
ia64_atomic_##op (int i, atomic_t *v)					\
{									\
	__s32 old, new;							\
	CMPXCHG_BUGCHECK_DECL						\
									\
	do {								\
		CMPXCHG_BUGCHECK(v);					\
<<<<<<< HEAD
		old = atomic_read(v);					\
=======
		old = arch_atomic_read(v);				\
>>>>>>> upstream/android-13
		new = old c_op i;					\
	} while (ia64_cmpxchg(acq, v, old, new, sizeof(atomic_t)) != old); \
	return new;							\
}

#define ATOMIC_FETCH_OP(op, c_op)					\
static __inline__ int							\
ia64_atomic_fetch_##op (int i, atomic_t *v)				\
{									\
	__s32 old, new;							\
	CMPXCHG_BUGCHECK_DECL						\
									\
	do {								\
		CMPXCHG_BUGCHECK(v);					\
<<<<<<< HEAD
		old = atomic_read(v);					\
=======
		old = arch_atomic_read(v);				\
>>>>>>> upstream/android-13
		new = old c_op i;					\
	} while (ia64_cmpxchg(acq, v, old, new, sizeof(atomic_t)) != old); \
	return old;							\
}

#define ATOMIC_OPS(op, c_op)						\
	ATOMIC_OP(op, c_op)						\
	ATOMIC_FETCH_OP(op, c_op)

ATOMIC_OPS(add, +)
ATOMIC_OPS(sub, -)

#ifdef __OPTIMIZE__
#define __ia64_atomic_const(i)						\
	static const int __ia64_atomic_p = __builtin_constant_p(i) ?	\
		((i) == 1 || (i) == 4 || (i) == 8 || (i) == 16 ||	\
		 (i) == -1 || (i) == -4 || (i) == -8 || (i) == -16) : 0;\
	__ia64_atomic_p
#else
#define __ia64_atomic_const(i)	0
#endif

<<<<<<< HEAD
#define atomic_add_return(i,v)						\
=======
#define arch_atomic_add_return(i,v)					\
>>>>>>> upstream/android-13
({									\
	int __ia64_aar_i = (i);						\
	__ia64_atomic_const(i)						\
		? ia64_fetch_and_add(__ia64_aar_i, &(v)->counter)	\
		: ia64_atomic_add(__ia64_aar_i, v);			\
})

<<<<<<< HEAD
#define atomic_sub_return(i,v)						\
=======
#define arch_atomic_sub_return(i,v)					\
>>>>>>> upstream/android-13
({									\
	int __ia64_asr_i = (i);						\
	__ia64_atomic_const(i)						\
		? ia64_fetch_and_add(-__ia64_asr_i, &(v)->counter)	\
		: ia64_atomic_sub(__ia64_asr_i, v);			\
})

<<<<<<< HEAD
#define atomic_fetch_add(i,v)						\
=======
#define arch_atomic_fetch_add(i,v)					\
>>>>>>> upstream/android-13
({									\
	int __ia64_aar_i = (i);						\
	__ia64_atomic_const(i)						\
		? ia64_fetchadd(__ia64_aar_i, &(v)->counter, acq)	\
		: ia64_atomic_fetch_add(__ia64_aar_i, v);		\
})

<<<<<<< HEAD
#define atomic_fetch_sub(i,v)						\
=======
#define arch_atomic_fetch_sub(i,v)					\
>>>>>>> upstream/android-13
({									\
	int __ia64_asr_i = (i);						\
	__ia64_atomic_const(i)						\
		? ia64_fetchadd(-__ia64_asr_i, &(v)->counter, acq)	\
		: ia64_atomic_fetch_sub(__ia64_asr_i, v);		\
})

ATOMIC_FETCH_OP(and, &)
ATOMIC_FETCH_OP(or, |)
ATOMIC_FETCH_OP(xor, ^)

<<<<<<< HEAD
#define atomic_and(i,v)	(void)ia64_atomic_fetch_and(i,v)
#define atomic_or(i,v)	(void)ia64_atomic_fetch_or(i,v)
#define atomic_xor(i,v)	(void)ia64_atomic_fetch_xor(i,v)

#define atomic_fetch_and(i,v)	ia64_atomic_fetch_and(i,v)
#define atomic_fetch_or(i,v)	ia64_atomic_fetch_or(i,v)
#define atomic_fetch_xor(i,v)	ia64_atomic_fetch_xor(i,v)
=======
#define arch_atomic_and(i,v)	(void)ia64_atomic_fetch_and(i,v)
#define arch_atomic_or(i,v)	(void)ia64_atomic_fetch_or(i,v)
#define arch_atomic_xor(i,v)	(void)ia64_atomic_fetch_xor(i,v)

#define arch_atomic_fetch_and(i,v)	ia64_atomic_fetch_and(i,v)
#define arch_atomic_fetch_or(i,v)	ia64_atomic_fetch_or(i,v)
#define arch_atomic_fetch_xor(i,v)	ia64_atomic_fetch_xor(i,v)
>>>>>>> upstream/android-13

#undef ATOMIC_OPS
#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP

#define ATOMIC64_OP(op, c_op)						\
<<<<<<< HEAD
static __inline__ long							\
ia64_atomic64_##op (__s64 i, atomic64_t *v)				\
{									\
	__s64 old, new;							\
=======
static __inline__ s64							\
ia64_atomic64_##op (s64 i, atomic64_t *v)				\
{									\
	s64 old, new;							\
>>>>>>> upstream/android-13
	CMPXCHG_BUGCHECK_DECL						\
									\
	do {								\
		CMPXCHG_BUGCHECK(v);					\
<<<<<<< HEAD
		old = atomic64_read(v);					\
=======
		old = arch_atomic64_read(v);				\
>>>>>>> upstream/android-13
		new = old c_op i;					\
	} while (ia64_cmpxchg(acq, v, old, new, sizeof(atomic64_t)) != old); \
	return new;							\
}

#define ATOMIC64_FETCH_OP(op, c_op)					\
<<<<<<< HEAD
static __inline__ long							\
ia64_atomic64_fetch_##op (__s64 i, atomic64_t *v)			\
{									\
	__s64 old, new;							\
=======
static __inline__ s64							\
ia64_atomic64_fetch_##op (s64 i, atomic64_t *v)				\
{									\
	s64 old, new;							\
>>>>>>> upstream/android-13
	CMPXCHG_BUGCHECK_DECL						\
									\
	do {								\
		CMPXCHG_BUGCHECK(v);					\
<<<<<<< HEAD
		old = atomic64_read(v);					\
=======
		old = arch_atomic64_read(v);				\
>>>>>>> upstream/android-13
		new = old c_op i;					\
	} while (ia64_cmpxchg(acq, v, old, new, sizeof(atomic64_t)) != old); \
	return old;							\
}

#define ATOMIC64_OPS(op, c_op)						\
	ATOMIC64_OP(op, c_op)						\
	ATOMIC64_FETCH_OP(op, c_op)

ATOMIC64_OPS(add, +)
ATOMIC64_OPS(sub, -)

<<<<<<< HEAD
#define atomic64_add_return(i,v)					\
({									\
	long __ia64_aar_i = (i);					\
=======
#define arch_atomic64_add_return(i,v)					\
({									\
	s64 __ia64_aar_i = (i);						\
>>>>>>> upstream/android-13
	__ia64_atomic_const(i)						\
		? ia64_fetch_and_add(__ia64_aar_i, &(v)->counter)	\
		: ia64_atomic64_add(__ia64_aar_i, v);			\
})

<<<<<<< HEAD
#define atomic64_sub_return(i,v)					\
({									\
	long __ia64_asr_i = (i);					\
=======
#define arch_atomic64_sub_return(i,v)					\
({									\
	s64 __ia64_asr_i = (i);						\
>>>>>>> upstream/android-13
	__ia64_atomic_const(i)						\
		? ia64_fetch_and_add(-__ia64_asr_i, &(v)->counter)	\
		: ia64_atomic64_sub(__ia64_asr_i, v);			\
})

<<<<<<< HEAD
#define atomic64_fetch_add(i,v)						\
({									\
	long __ia64_aar_i = (i);					\
=======
#define arch_atomic64_fetch_add(i,v)					\
({									\
	s64 __ia64_aar_i = (i);						\
>>>>>>> upstream/android-13
	__ia64_atomic_const(i)						\
		? ia64_fetchadd(__ia64_aar_i, &(v)->counter, acq)	\
		: ia64_atomic64_fetch_add(__ia64_aar_i, v);		\
})

<<<<<<< HEAD
#define atomic64_fetch_sub(i,v)						\
({									\
	long __ia64_asr_i = (i);					\
=======
#define arch_atomic64_fetch_sub(i,v)					\
({									\
	s64 __ia64_asr_i = (i);						\
>>>>>>> upstream/android-13
	__ia64_atomic_const(i)						\
		? ia64_fetchadd(-__ia64_asr_i, &(v)->counter, acq)	\
		: ia64_atomic64_fetch_sub(__ia64_asr_i, v);		\
})

ATOMIC64_FETCH_OP(and, &)
ATOMIC64_FETCH_OP(or, |)
ATOMIC64_FETCH_OP(xor, ^)

<<<<<<< HEAD
#define atomic64_and(i,v)	(void)ia64_atomic64_fetch_and(i,v)
#define atomic64_or(i,v)	(void)ia64_atomic64_fetch_or(i,v)
#define atomic64_xor(i,v)	(void)ia64_atomic64_fetch_xor(i,v)

#define atomic64_fetch_and(i,v)	ia64_atomic64_fetch_and(i,v)
#define atomic64_fetch_or(i,v)	ia64_atomic64_fetch_or(i,v)
#define atomic64_fetch_xor(i,v)	ia64_atomic64_fetch_xor(i,v)
=======
#define arch_atomic64_and(i,v)	(void)ia64_atomic64_fetch_and(i,v)
#define arch_atomic64_or(i,v)	(void)ia64_atomic64_fetch_or(i,v)
#define arch_atomic64_xor(i,v)	(void)ia64_atomic64_fetch_xor(i,v)

#define arch_atomic64_fetch_and(i,v)	ia64_atomic64_fetch_and(i,v)
#define arch_atomic64_fetch_or(i,v)	ia64_atomic64_fetch_or(i,v)
#define arch_atomic64_fetch_xor(i,v)	ia64_atomic64_fetch_xor(i,v)
>>>>>>> upstream/android-13

#undef ATOMIC64_OPS
#undef ATOMIC64_FETCH_OP
#undef ATOMIC64_OP

<<<<<<< HEAD
#define atomic_cmpxchg(v, old, new) (cmpxchg(&((v)->counter), old, new))
#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

#define atomic64_cmpxchg(v, old, new) \
	(cmpxchg(&((v)->counter), old, new))
#define atomic64_xchg(v, new) (xchg(&((v)->counter), new))

#define atomic_add(i,v)			(void)atomic_add_return((i), (v))
#define atomic_sub(i,v)			(void)atomic_sub_return((i), (v))

#define atomic64_add(i,v)		(void)atomic64_add_return((i), (v))
#define atomic64_sub(i,v)		(void)atomic64_sub_return((i), (v))
=======
#define arch_atomic_cmpxchg(v, old, new) (arch_cmpxchg(&((v)->counter), old, new))
#define arch_atomic_xchg(v, new) (arch_xchg(&((v)->counter), new))

#define arch_atomic64_cmpxchg(v, old, new) \
	(arch_cmpxchg(&((v)->counter), old, new))
#define arch_atomic64_xchg(v, new) (arch_xchg(&((v)->counter), new))

#define arch_atomic_add(i,v)		(void)arch_atomic_add_return((i), (v))
#define arch_atomic_sub(i,v)		(void)arch_atomic_sub_return((i), (v))

#define arch_atomic64_add(i,v)		(void)arch_atomic64_add_return((i), (v))
#define arch_atomic64_sub(i,v)		(void)arch_atomic64_sub_return((i), (v))
>>>>>>> upstream/android-13

#endif /* _ASM_IA64_ATOMIC_H */
