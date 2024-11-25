/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright IBM Corp. 1999, 2016
 * Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *	      Denis Joseph Barrow,
 *	      Arnd Bergmann,
 */

#ifndef __ARCH_S390_ATOMIC__
#define __ARCH_S390_ATOMIC__

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/atomic_ops.h>
#include <asm/barrier.h>
#include <asm/cmpxchg.h>

<<<<<<< HEAD
#define ATOMIC_INIT(i)  { (i) }

static inline int atomic_read(const atomic_t *v)
{
	int c;

	asm volatile(
		"	l	%0,%1\n"
		: "=d" (c) : "Q" (v->counter));
	return c;
}

static inline void atomic_set(atomic_t *v, int i)
{
	asm volatile(
		"	st	%1,%0\n"
		: "=Q" (v->counter) : "d" (i));
}

static inline int atomic_add_return(int i, atomic_t *v)
{
	return __atomic_add_barrier(i, &v->counter) + i;
}

static inline int atomic_fetch_add(int i, atomic_t *v)
{
	return __atomic_add_barrier(i, &v->counter);
}

static inline void atomic_add(int i, atomic_t *v)
{
#ifdef CONFIG_HAVE_MARCH_Z196_FEATURES
	if (__builtin_constant_p(i) && (i > -129) && (i < 128)) {
		__atomic_add_const(i, &v->counter);
		return;
	}
#endif
	__atomic_add(i, &v->counter);
}

#define atomic_sub(_i, _v)		atomic_add(-(int)(_i), _v)
#define atomic_sub_return(_i, _v)	atomic_add_return(-(int)(_i), _v)
#define atomic_fetch_sub(_i, _v)	atomic_fetch_add(-(int)(_i), _v)

#define ATOMIC_OPS(op)							\
static inline void atomic_##op(int i, atomic_t *v)			\
{									\
	__atomic_##op(i, &v->counter);					\
}									\
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
=======
static inline int arch_atomic_read(const atomic_t *v)
{
	return __atomic_read(v);
}
#define arch_atomic_read arch_atomic_read

static inline void arch_atomic_set(atomic_t *v, int i)
{
	__atomic_set(v, i);
}
#define arch_atomic_set arch_atomic_set

static inline int arch_atomic_add_return(int i, atomic_t *v)
{
	return __atomic_add_barrier(i, &v->counter) + i;
}
#define arch_atomic_add_return arch_atomic_add_return

static inline int arch_atomic_fetch_add(int i, atomic_t *v)
{
	return __atomic_add_barrier(i, &v->counter);
}
#define arch_atomic_fetch_add arch_atomic_fetch_add

static inline void arch_atomic_add(int i, atomic_t *v)
{
	__atomic_add(i, &v->counter);
}
#define arch_atomic_add arch_atomic_add

#define arch_atomic_sub(_i, _v)		arch_atomic_add(-(int)(_i), _v)
#define arch_atomic_sub_return(_i, _v)	arch_atomic_add_return(-(int)(_i), _v)
#define arch_atomic_fetch_sub(_i, _v)	arch_atomic_fetch_add(-(int)(_i), _v)

#define ATOMIC_OPS(op)							\
static inline void arch_atomic_##op(int i, atomic_t *v)			\
{									\
	__atomic_##op(i, &v->counter);					\
}									\
static inline int arch_atomic_fetch_##op(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	return __atomic_##op##_barrier(i, &v->counter);			\
}

ATOMIC_OPS(and)
ATOMIC_OPS(or)
ATOMIC_OPS(xor)

#undef ATOMIC_OPS

<<<<<<< HEAD
#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
{
	return __atomic_cmpxchg(&v->counter, old, new);
}

#define ATOMIC64_INIT(i)  { (i) }

static inline long atomic64_read(const atomic64_t *v)
{
	long c;

	asm volatile(
		"	lg	%0,%1\n"
		: "=d" (c) : "Q" (v->counter));
	return c;
}

static inline void atomic64_set(atomic64_t *v, long i)
{
	asm volatile(
		"	stg	%1,%0\n"
		: "=Q" (v->counter) : "d" (i));
}

static inline long atomic64_add_return(long i, atomic64_t *v)
{
	return __atomic64_add_barrier(i, &v->counter) + i;
}

static inline long atomic64_fetch_add(long i, atomic64_t *v)
{
	return __atomic64_add_barrier(i, &v->counter);
}

static inline void atomic64_add(long i, atomic64_t *v)
{
#ifdef CONFIG_HAVE_MARCH_Z196_FEATURES
	if (__builtin_constant_p(i) && (i > -129) && (i < 128)) {
		__atomic64_add_const(i, &v->counter);
		return;
	}
#endif
	__atomic64_add(i, &v->counter);
}

#define atomic64_xchg(v, new) (xchg(&((v)->counter), new))

static inline long atomic64_cmpxchg(atomic64_t *v, long old, long new)
{
	return __atomic64_cmpxchg(&v->counter, old, new);
}

#define ATOMIC64_OPS(op)						\
static inline void atomic64_##op(long i, atomic64_t *v)			\
{									\
	__atomic64_##op(i, &v->counter);				\
}									\
static inline long atomic64_fetch_##op(long i, atomic64_t *v)		\
{									\
	return __atomic64_##op##_barrier(i, &v->counter);		\
=======
#define arch_atomic_and			arch_atomic_and
#define arch_atomic_or			arch_atomic_or
#define arch_atomic_xor			arch_atomic_xor
#define arch_atomic_fetch_and		arch_atomic_fetch_and
#define arch_atomic_fetch_or		arch_atomic_fetch_or
#define arch_atomic_fetch_xor		arch_atomic_fetch_xor

#define arch_atomic_xchg(v, new)	(arch_xchg(&((v)->counter), new))

static inline int arch_atomic_cmpxchg(atomic_t *v, int old, int new)
{
	return __atomic_cmpxchg(&v->counter, old, new);
}
#define arch_atomic_cmpxchg arch_atomic_cmpxchg

#define ATOMIC64_INIT(i)  { (i) }

static inline s64 arch_atomic64_read(const atomic64_t *v)
{
	return __atomic64_read(v);
}
#define arch_atomic64_read arch_atomic64_read

static inline void arch_atomic64_set(atomic64_t *v, s64 i)
{
	__atomic64_set(v, i);
}
#define arch_atomic64_set arch_atomic64_set

static inline s64 arch_atomic64_add_return(s64 i, atomic64_t *v)
{
	return __atomic64_add_barrier(i, (long *)&v->counter) + i;
}
#define arch_atomic64_add_return arch_atomic64_add_return

static inline s64 arch_atomic64_fetch_add(s64 i, atomic64_t *v)
{
	return __atomic64_add_barrier(i, (long *)&v->counter);
}
#define arch_atomic64_fetch_add arch_atomic64_fetch_add

static inline void arch_atomic64_add(s64 i, atomic64_t *v)
{
	__atomic64_add(i, (long *)&v->counter);
}
#define arch_atomic64_add arch_atomic64_add

#define arch_atomic64_xchg(v, new)	(arch_xchg(&((v)->counter), new))

static inline s64 arch_atomic64_cmpxchg(atomic64_t *v, s64 old, s64 new)
{
	return __atomic64_cmpxchg((long *)&v->counter, old, new);
}
#define arch_atomic64_cmpxchg arch_atomic64_cmpxchg

#define ATOMIC64_OPS(op)						\
static inline void arch_atomic64_##op(s64 i, atomic64_t *v)		\
{									\
	__atomic64_##op(i, (long *)&v->counter);			\
}									\
static inline long arch_atomic64_fetch_##op(s64 i, atomic64_t *v)	\
{									\
	return __atomic64_##op##_barrier(i, (long *)&v->counter);	\
>>>>>>> upstream/android-13
}

ATOMIC64_OPS(and)
ATOMIC64_OPS(or)
ATOMIC64_OPS(xor)

#undef ATOMIC64_OPS

<<<<<<< HEAD
#define atomic64_sub_return(_i, _v)	atomic64_add_return(-(long)(_i), _v)
#define atomic64_fetch_sub(_i, _v)	atomic64_fetch_add(-(long)(_i), _v)
#define atomic64_sub(_i, _v)		atomic64_add(-(long)(_i), _v)
=======
#define arch_atomic64_and		arch_atomic64_and
#define arch_atomic64_or		arch_atomic64_or
#define arch_atomic64_xor		arch_atomic64_xor
#define arch_atomic64_fetch_and		arch_atomic64_fetch_and
#define arch_atomic64_fetch_or		arch_atomic64_fetch_or
#define arch_atomic64_fetch_xor		arch_atomic64_fetch_xor

#define arch_atomic64_sub_return(_i, _v) arch_atomic64_add_return(-(s64)(_i), _v)
#define arch_atomic64_fetch_sub(_i, _v)  arch_atomic64_fetch_add(-(s64)(_i), _v)
#define arch_atomic64_sub(_i, _v)	 arch_atomic64_add(-(s64)(_i), _v)
>>>>>>> upstream/android-13

#endif /* __ARCH_S390_ATOMIC__  */
