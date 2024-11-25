<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  arch/arm/include/asm/atomic.h
 *
 *  Copyright (C) 1996 Russell King.
 *  Copyright (C) 2002 Deep Blue Solutions Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef __ASM_ARM_ATOMIC_H
#define __ASM_ARM_ATOMIC_H

#include <linux/compiler.h>
#include <linux/prefetch.h>
#include <linux/types.h>
#include <linux/irqflags.h>
#include <asm/barrier.h>
#include <asm/cmpxchg.h>

<<<<<<< HEAD
#define ATOMIC_INIT(i)	{ (i) }

=======
>>>>>>> upstream/android-13
#ifdef __KERNEL__

/*
 * On ARM, ordinary assignment (str instruction) doesn't clear the local
 * strex/ldrex monitor on some implementations. The reason we can use it for
 * atomic_set() is the clrex or dummy strex done on every exception return.
 */
<<<<<<< HEAD
#define atomic_read(v)	READ_ONCE((v)->counter)
#define atomic_set(v,i)	WRITE_ONCE(((v)->counter), (i))
=======
#define arch_atomic_read(v)	READ_ONCE((v)->counter)
#define arch_atomic_set(v,i)	WRITE_ONCE(((v)->counter), (i))
>>>>>>> upstream/android-13

#if __LINUX_ARM_ARCH__ >= 6

/*
 * ARMv6 UP and SMP safe atomic ops.  We use load exclusive and
 * store exclusive to ensure that these are atomic.  We may loop
 * to ensure that the update happens.
 */

#define ATOMIC_OP(op, c_op, asm_op)					\
<<<<<<< HEAD
static inline void atomic_##op(int i, atomic_t *v)			\
=======
static inline void arch_atomic_##op(int i, atomic_t *v)			\
>>>>>>> upstream/android-13
{									\
	unsigned long tmp;						\
	int result;							\
									\
	prefetchw(&v->counter);						\
	__asm__ __volatile__("@ atomic_" #op "\n"			\
"1:	ldrex	%0, [%3]\n"						\
"	" #asm_op "	%0, %0, %4\n"					\
"	strex	%1, %0, [%3]\n"						\
"	teq	%1, #0\n"						\
"	bne	1b"							\
	: "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)		\
	: "r" (&v->counter), "Ir" (i)					\
	: "cc");							\
}									\

#define ATOMIC_OP_RETURN(op, c_op, asm_op)				\
<<<<<<< HEAD
static inline int atomic_##op##_return_relaxed(int i, atomic_t *v)	\
=======
static inline int arch_atomic_##op##_return_relaxed(int i, atomic_t *v)	\
>>>>>>> upstream/android-13
{									\
	unsigned long tmp;						\
	int result;							\
									\
	prefetchw(&v->counter);						\
									\
	__asm__ __volatile__("@ atomic_" #op "_return\n"		\
"1:	ldrex	%0, [%3]\n"						\
"	" #asm_op "	%0, %0, %4\n"					\
"	strex	%1, %0, [%3]\n"						\
"	teq	%1, #0\n"						\
"	bne	1b"							\
	: "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)		\
	: "r" (&v->counter), "Ir" (i)					\
	: "cc");							\
									\
	return result;							\
}

#define ATOMIC_FETCH_OP(op, c_op, asm_op)				\
<<<<<<< HEAD
static inline int atomic_fetch_##op##_relaxed(int i, atomic_t *v)	\
=======
static inline int arch_atomic_fetch_##op##_relaxed(int i, atomic_t *v)	\
>>>>>>> upstream/android-13
{									\
	unsigned long tmp;						\
	int result, val;						\
									\
	prefetchw(&v->counter);						\
									\
	__asm__ __volatile__("@ atomic_fetch_" #op "\n"			\
"1:	ldrex	%0, [%4]\n"						\
"	" #asm_op "	%1, %0, %5\n"					\
"	strex	%2, %1, [%4]\n"						\
"	teq	%2, #0\n"						\
"	bne	1b"							\
	: "=&r" (result), "=&r" (val), "=&r" (tmp), "+Qo" (v->counter)	\
	: "r" (&v->counter), "Ir" (i)					\
	: "cc");							\
									\
	return result;							\
}

<<<<<<< HEAD
#define atomic_add_return_relaxed	atomic_add_return_relaxed
#define atomic_sub_return_relaxed	atomic_sub_return_relaxed
#define atomic_fetch_add_relaxed	atomic_fetch_add_relaxed
#define atomic_fetch_sub_relaxed	atomic_fetch_sub_relaxed

#define atomic_fetch_and_relaxed	atomic_fetch_and_relaxed
#define atomic_fetch_andnot_relaxed	atomic_fetch_andnot_relaxed
#define atomic_fetch_or_relaxed		atomic_fetch_or_relaxed
#define atomic_fetch_xor_relaxed	atomic_fetch_xor_relaxed

static inline int atomic_cmpxchg_relaxed(atomic_t *ptr, int old, int new)
=======
#define arch_atomic_add_return_relaxed		arch_atomic_add_return_relaxed
#define arch_atomic_sub_return_relaxed		arch_atomic_sub_return_relaxed
#define arch_atomic_fetch_add_relaxed		arch_atomic_fetch_add_relaxed
#define arch_atomic_fetch_sub_relaxed		arch_atomic_fetch_sub_relaxed

#define arch_atomic_fetch_and_relaxed		arch_atomic_fetch_and_relaxed
#define arch_atomic_fetch_andnot_relaxed	arch_atomic_fetch_andnot_relaxed
#define arch_atomic_fetch_or_relaxed		arch_atomic_fetch_or_relaxed
#define arch_atomic_fetch_xor_relaxed		arch_atomic_fetch_xor_relaxed

static inline int arch_atomic_cmpxchg_relaxed(atomic_t *ptr, int old, int new)
>>>>>>> upstream/android-13
{
	int oldval;
	unsigned long res;

	prefetchw(&ptr->counter);

	do {
		__asm__ __volatile__("@ atomic_cmpxchg\n"
		"ldrex	%1, [%3]\n"
		"mov	%0, #0\n"
		"teq	%1, %4\n"
		"strexeq %0, %5, [%3]\n"
		    : "=&r" (res), "=&r" (oldval), "+Qo" (ptr->counter)
		    : "r" (&ptr->counter), "Ir" (old), "r" (new)
		    : "cc");
	} while (res);

	return oldval;
}
<<<<<<< HEAD
#define atomic_cmpxchg_relaxed		atomic_cmpxchg_relaxed

static inline int atomic_fetch_add_unless(atomic_t *v, int a, int u)
=======
#define arch_atomic_cmpxchg_relaxed		arch_atomic_cmpxchg_relaxed

static inline int arch_atomic_fetch_add_unless(atomic_t *v, int a, int u)
>>>>>>> upstream/android-13
{
	int oldval, newval;
	unsigned long tmp;

	smp_mb();
	prefetchw(&v->counter);

	__asm__ __volatile__ ("@ atomic_add_unless\n"
"1:	ldrex	%0, [%4]\n"
"	teq	%0, %5\n"
"	beq	2f\n"
"	add	%1, %0, %6\n"
"	strex	%2, %1, [%4]\n"
"	teq	%2, #0\n"
"	bne	1b\n"
"2:"
	: "=&r" (oldval), "=&r" (newval), "=&r" (tmp), "+Qo" (v->counter)
	: "r" (&v->counter), "r" (u), "r" (a)
	: "cc");

	if (oldval != u)
		smp_mb();

	return oldval;
}
<<<<<<< HEAD
#define atomic_fetch_add_unless		atomic_fetch_add_unless
=======
#define arch_atomic_fetch_add_unless		arch_atomic_fetch_add_unless
>>>>>>> upstream/android-13

#else /* ARM_ARCH_6 */

#ifdef CONFIG_SMP
#error SMP not supported on pre-ARMv6 CPUs
#endif

#define ATOMIC_OP(op, c_op, asm_op)					\
<<<<<<< HEAD
static inline void atomic_##op(int i, atomic_t *v)			\
=======
static inline void arch_atomic_##op(int i, atomic_t *v)			\
>>>>>>> upstream/android-13
{									\
	unsigned long flags;						\
									\
	raw_local_irq_save(flags);					\
	v->counter c_op i;						\
	raw_local_irq_restore(flags);					\
}									\

#define ATOMIC_OP_RETURN(op, c_op, asm_op)				\
<<<<<<< HEAD
static inline int atomic_##op##_return(int i, atomic_t *v)		\
=======
static inline int arch_atomic_##op##_return(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	unsigned long flags;						\
	int val;							\
									\
	raw_local_irq_save(flags);					\
	v->counter c_op i;						\
	val = v->counter;						\
	raw_local_irq_restore(flags);					\
									\
	return val;							\
}

#define ATOMIC_FETCH_OP(op, c_op, asm_op)				\
<<<<<<< HEAD
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
=======
static inline int arch_atomic_fetch_##op(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	unsigned long flags;						\
	int val;							\
									\
	raw_local_irq_save(flags);					\
	val = v->counter;						\
	v->counter c_op i;						\
	raw_local_irq_restore(flags);					\
									\
	return val;							\
}

<<<<<<< HEAD
static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
=======
static inline int arch_atomic_cmpxchg(atomic_t *v, int old, int new)
>>>>>>> upstream/android-13
{
	int ret;
	unsigned long flags;

	raw_local_irq_save(flags);
	ret = v->counter;
	if (likely(ret == old))
		v->counter = new;
	raw_local_irq_restore(flags);

	return ret;
}

<<<<<<< HEAD
#define atomic_fetch_andnot		atomic_fetch_andnot
=======
#define arch_atomic_fetch_andnot		arch_atomic_fetch_andnot
>>>>>>> upstream/android-13

#endif /* __LINUX_ARM_ARCH__ */

#define ATOMIC_OPS(op, c_op, asm_op)					\
	ATOMIC_OP(op, c_op, asm_op)					\
	ATOMIC_OP_RETURN(op, c_op, asm_op)				\
	ATOMIC_FETCH_OP(op, c_op, asm_op)

ATOMIC_OPS(add, +=, add)
ATOMIC_OPS(sub, -=, sub)

<<<<<<< HEAD
#define atomic_andnot atomic_andnot
=======
#define arch_atomic_andnot arch_atomic_andnot
>>>>>>> upstream/android-13

#undef ATOMIC_OPS
#define ATOMIC_OPS(op, c_op, asm_op)					\
	ATOMIC_OP(op, c_op, asm_op)					\
	ATOMIC_FETCH_OP(op, c_op, asm_op)

ATOMIC_OPS(and, &=, and)
ATOMIC_OPS(andnot, &= ~, bic)
ATOMIC_OPS(or,  |=, orr)
ATOMIC_OPS(xor, ^=, eor)

#undef ATOMIC_OPS
#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP

<<<<<<< HEAD
#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

#ifndef CONFIG_GENERIC_ATOMIC64
typedef struct {
	long long counter;
=======
#define arch_atomic_xchg(v, new) (arch_xchg(&((v)->counter), new))

#ifndef CONFIG_GENERIC_ATOMIC64
typedef struct {
	s64 counter;
>>>>>>> upstream/android-13
} atomic64_t;

#define ATOMIC64_INIT(i) { (i) }

#ifdef CONFIG_ARM_LPAE
<<<<<<< HEAD
static inline long long atomic64_read(const atomic64_t *v)
{
	long long result;
=======
static inline s64 arch_atomic64_read(const atomic64_t *v)
{
	s64 result;
>>>>>>> upstream/android-13

	__asm__ __volatile__("@ atomic64_read\n"
"	ldrd	%0, %H0, [%1]"
	: "=&r" (result)
	: "r" (&v->counter), "Qo" (v->counter)
	);

	return result;
}

<<<<<<< HEAD
static inline void atomic64_set(atomic64_t *v, long long i)
=======
static inline void arch_atomic64_set(atomic64_t *v, s64 i)
>>>>>>> upstream/android-13
{
	__asm__ __volatile__("@ atomic64_set\n"
"	strd	%2, %H2, [%1]"
	: "=Qo" (v->counter)
	: "r" (&v->counter), "r" (i)
	);
}
#else
<<<<<<< HEAD
static inline long long atomic64_read(const atomic64_t *v)
{
	long long result;
=======
static inline s64 arch_atomic64_read(const atomic64_t *v)
{
	s64 result;
>>>>>>> upstream/android-13

	__asm__ __volatile__("@ atomic64_read\n"
"	ldrexd	%0, %H0, [%1]"
	: "=&r" (result)
	: "r" (&v->counter), "Qo" (v->counter)
	);

	return result;
}

<<<<<<< HEAD
static inline void atomic64_set(atomic64_t *v, long long i)
{
	long long tmp;
=======
static inline void arch_atomic64_set(atomic64_t *v, s64 i)
{
	s64 tmp;
>>>>>>> upstream/android-13

	prefetchw(&v->counter);
	__asm__ __volatile__("@ atomic64_set\n"
"1:	ldrexd	%0, %H0, [%2]\n"
"	strexd	%0, %3, %H3, [%2]\n"
"	teq	%0, #0\n"
"	bne	1b"
	: "=&r" (tmp), "=Qo" (v->counter)
	: "r" (&v->counter), "r" (i)
	: "cc");
}
#endif

#define ATOMIC64_OP(op, op1, op2)					\
<<<<<<< HEAD
static inline void atomic64_##op(long long i, atomic64_t *v)		\
{									\
	long long result;						\
=======
static inline void arch_atomic64_##op(s64 i, atomic64_t *v)		\
{									\
	s64 result;							\
>>>>>>> upstream/android-13
	unsigned long tmp;						\
									\
	prefetchw(&v->counter);						\
	__asm__ __volatile__("@ atomic64_" #op "\n"			\
"1:	ldrexd	%0, %H0, [%3]\n"					\
"	" #op1 " %Q0, %Q0, %Q4\n"					\
"	" #op2 " %R0, %R0, %R4\n"					\
"	strexd	%1, %0, %H0, [%3]\n"					\
"	teq	%1, #0\n"						\
"	bne	1b"							\
	: "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)		\
	: "r" (&v->counter), "r" (i)					\
	: "cc");							\
}									\

#define ATOMIC64_OP_RETURN(op, op1, op2)				\
<<<<<<< HEAD
static inline long long							\
atomic64_##op##_return_relaxed(long long i, atomic64_t *v)		\
{									\
	long long result;						\
=======
static inline s64							\
arch_atomic64_##op##_return_relaxed(s64 i, atomic64_t *v)		\
{									\
	s64 result;							\
>>>>>>> upstream/android-13
	unsigned long tmp;						\
									\
	prefetchw(&v->counter);						\
									\
	__asm__ __volatile__("@ atomic64_" #op "_return\n"		\
"1:	ldrexd	%0, %H0, [%3]\n"					\
"	" #op1 " %Q0, %Q0, %Q4\n"					\
"	" #op2 " %R0, %R0, %R4\n"					\
"	strexd	%1, %0, %H0, [%3]\n"					\
"	teq	%1, #0\n"						\
"	bne	1b"							\
	: "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)		\
	: "r" (&v->counter), "r" (i)					\
	: "cc");							\
									\
	return result;							\
}

#define ATOMIC64_FETCH_OP(op, op1, op2)					\
<<<<<<< HEAD
static inline long long							\
atomic64_fetch_##op##_relaxed(long long i, atomic64_t *v)		\
{									\
	long long result, val;						\
=======
static inline s64							\
arch_atomic64_fetch_##op##_relaxed(s64 i, atomic64_t *v)		\
{									\
	s64 result, val;						\
>>>>>>> upstream/android-13
	unsigned long tmp;						\
									\
	prefetchw(&v->counter);						\
									\
	__asm__ __volatile__("@ atomic64_fetch_" #op "\n"		\
"1:	ldrexd	%0, %H0, [%4]\n"					\
"	" #op1 " %Q1, %Q0, %Q5\n"					\
"	" #op2 " %R1, %R0, %R5\n"					\
"	strexd	%2, %1, %H1, [%4]\n"					\
"	teq	%2, #0\n"						\
"	bne	1b"							\
	: "=&r" (result), "=&r" (val), "=&r" (tmp), "+Qo" (v->counter)	\
	: "r" (&v->counter), "r" (i)					\
	: "cc");							\
									\
	return result;							\
}

#define ATOMIC64_OPS(op, op1, op2)					\
	ATOMIC64_OP(op, op1, op2)					\
	ATOMIC64_OP_RETURN(op, op1, op2)				\
	ATOMIC64_FETCH_OP(op, op1, op2)

ATOMIC64_OPS(add, adds, adc)
ATOMIC64_OPS(sub, subs, sbc)

<<<<<<< HEAD
#define atomic64_add_return_relaxed	atomic64_add_return_relaxed
#define atomic64_sub_return_relaxed	atomic64_sub_return_relaxed
#define atomic64_fetch_add_relaxed	atomic64_fetch_add_relaxed
#define atomic64_fetch_sub_relaxed	atomic64_fetch_sub_relaxed
=======
#define arch_atomic64_add_return_relaxed	arch_atomic64_add_return_relaxed
#define arch_atomic64_sub_return_relaxed	arch_atomic64_sub_return_relaxed
#define arch_atomic64_fetch_add_relaxed		arch_atomic64_fetch_add_relaxed
#define arch_atomic64_fetch_sub_relaxed		arch_atomic64_fetch_sub_relaxed
>>>>>>> upstream/android-13

#undef ATOMIC64_OPS
#define ATOMIC64_OPS(op, op1, op2)					\
	ATOMIC64_OP(op, op1, op2)					\
	ATOMIC64_FETCH_OP(op, op1, op2)

<<<<<<< HEAD
#define atomic64_andnot atomic64_andnot
=======
#define arch_atomic64_andnot arch_atomic64_andnot
>>>>>>> upstream/android-13

ATOMIC64_OPS(and, and, and)
ATOMIC64_OPS(andnot, bic, bic)
ATOMIC64_OPS(or,  orr, orr)
ATOMIC64_OPS(xor, eor, eor)

<<<<<<< HEAD
#define atomic64_fetch_and_relaxed	atomic64_fetch_and_relaxed
#define atomic64_fetch_andnot_relaxed	atomic64_fetch_andnot_relaxed
#define atomic64_fetch_or_relaxed	atomic64_fetch_or_relaxed
#define atomic64_fetch_xor_relaxed	atomic64_fetch_xor_relaxed
=======
#define arch_atomic64_fetch_and_relaxed		arch_atomic64_fetch_and_relaxed
#define arch_atomic64_fetch_andnot_relaxed	arch_atomic64_fetch_andnot_relaxed
#define arch_atomic64_fetch_or_relaxed		arch_atomic64_fetch_or_relaxed
#define arch_atomic64_fetch_xor_relaxed		arch_atomic64_fetch_xor_relaxed
>>>>>>> upstream/android-13

#undef ATOMIC64_OPS
#undef ATOMIC64_FETCH_OP
#undef ATOMIC64_OP_RETURN
#undef ATOMIC64_OP

<<<<<<< HEAD
static inline long long
atomic64_cmpxchg_relaxed(atomic64_t *ptr, long long old, long long new)
{
	long long oldval;
=======
static inline s64 arch_atomic64_cmpxchg_relaxed(atomic64_t *ptr, s64 old, s64 new)
{
	s64 oldval;
>>>>>>> upstream/android-13
	unsigned long res;

	prefetchw(&ptr->counter);

	do {
		__asm__ __volatile__("@ atomic64_cmpxchg\n"
		"ldrexd		%1, %H1, [%3]\n"
		"mov		%0, #0\n"
		"teq		%1, %4\n"
		"teqeq		%H1, %H4\n"
		"strexdeq	%0, %5, %H5, [%3]"
		: "=&r" (res), "=&r" (oldval), "+Qo" (ptr->counter)
		: "r" (&ptr->counter), "r" (old), "r" (new)
		: "cc");
	} while (res);

	return oldval;
}
<<<<<<< HEAD
#define atomic64_cmpxchg_relaxed	atomic64_cmpxchg_relaxed

static inline long long atomic64_xchg_relaxed(atomic64_t *ptr, long long new)
{
	long long result;
=======
#define arch_atomic64_cmpxchg_relaxed	arch_atomic64_cmpxchg_relaxed

static inline s64 arch_atomic64_xchg_relaxed(atomic64_t *ptr, s64 new)
{
	s64 result;
>>>>>>> upstream/android-13
	unsigned long tmp;

	prefetchw(&ptr->counter);

	__asm__ __volatile__("@ atomic64_xchg\n"
"1:	ldrexd	%0, %H0, [%3]\n"
"	strexd	%1, %4, %H4, [%3]\n"
"	teq	%1, #0\n"
"	bne	1b"
	: "=&r" (result), "=&r" (tmp), "+Qo" (ptr->counter)
	: "r" (&ptr->counter), "r" (new)
	: "cc");

	return result;
}
<<<<<<< HEAD
#define atomic64_xchg_relaxed		atomic64_xchg_relaxed

static inline long long atomic64_dec_if_positive(atomic64_t *v)
{
	long long result;
=======
#define arch_atomic64_xchg_relaxed		arch_atomic64_xchg_relaxed

static inline s64 arch_atomic64_dec_if_positive(atomic64_t *v)
{
	s64 result;
>>>>>>> upstream/android-13
	unsigned long tmp;

	smp_mb();
	prefetchw(&v->counter);

	__asm__ __volatile__("@ atomic64_dec_if_positive\n"
"1:	ldrexd	%0, %H0, [%3]\n"
"	subs	%Q0, %Q0, #1\n"
"	sbc	%R0, %R0, #0\n"
"	teq	%R0, #0\n"
"	bmi	2f\n"
"	strexd	%1, %0, %H0, [%3]\n"
"	teq	%1, #0\n"
"	bne	1b\n"
"2:"
	: "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)
	: "r" (&v->counter)
	: "cc");

	smp_mb();

	return result;
}
<<<<<<< HEAD
#define atomic64_dec_if_positive atomic64_dec_if_positive

static inline long long atomic64_fetch_add_unless(atomic64_t *v, long long a,
						  long long u)
{
	long long oldval, newval;
=======
#define arch_atomic64_dec_if_positive arch_atomic64_dec_if_positive

static inline s64 arch_atomic64_fetch_add_unless(atomic64_t *v, s64 a, s64 u)
{
	s64 oldval, newval;
>>>>>>> upstream/android-13
	unsigned long tmp;

	smp_mb();
	prefetchw(&v->counter);

	__asm__ __volatile__("@ atomic64_add_unless\n"
"1:	ldrexd	%0, %H0, [%4]\n"
"	teq	%0, %5\n"
"	teqeq	%H0, %H5\n"
"	beq	2f\n"
"	adds	%Q1, %Q0, %Q6\n"
"	adc	%R1, %R0, %R6\n"
"	strexd	%2, %1, %H1, [%4]\n"
"	teq	%2, #0\n"
"	bne	1b\n"
"2:"
	: "=&r" (oldval), "=&r" (newval), "=&r" (tmp), "+Qo" (v->counter)
	: "r" (&v->counter), "r" (u), "r" (a)
	: "cc");

	if (oldval != u)
		smp_mb();

	return oldval;
}
<<<<<<< HEAD
#define atomic64_fetch_add_unless atomic64_fetch_add_unless
=======
#define arch_atomic64_fetch_add_unless arch_atomic64_fetch_add_unless
>>>>>>> upstream/android-13

#endif /* !CONFIG_GENERIC_ATOMIC64 */
#endif
#endif
