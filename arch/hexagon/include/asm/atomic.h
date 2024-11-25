<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Atomic operations for the Hexagon architecture
 *
 * Copyright (c) 2010-2013, The Linux Foundation. All rights reserved.
<<<<<<< HEAD
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
=======
>>>>>>> upstream/android-13
 */

#ifndef _ASM_ATOMIC_H
#define _ASM_ATOMIC_H

#include <linux/types.h>
#include <asm/cmpxchg.h>
#include <asm/barrier.h>

<<<<<<< HEAD
#define ATOMIC_INIT(i)		{ (i) }

/*  Normal writes in our arch don't clear lock reservations  */

static inline void atomic_set(atomic_t *v, int new)
=======
/*  Normal writes in our arch don't clear lock reservations  */

static inline void arch_atomic_set(atomic_t *v, int new)
>>>>>>> upstream/android-13
{
	asm volatile(
		"1:	r6 = memw_locked(%0);\n"
		"	memw_locked(%0,p0) = %1;\n"
		"	if (!P0) jump 1b;\n"
		:
		: "r" (&v->counter), "r" (new)
		: "memory", "p0", "r6"
	);
}

<<<<<<< HEAD
#define atomic_set_release(v, i)	atomic_set((v), (i))

/**
 * atomic_read - reads a word, atomically
=======
#define arch_atomic_set_release(v, i)	arch_atomic_set((v), (i))

/**
 * arch_atomic_read - reads a word, atomically
>>>>>>> upstream/android-13
 * @v: pointer to atomic value
 *
 * Assumes all word reads on our architecture are atomic.
 */
<<<<<<< HEAD
#define atomic_read(v)		READ_ONCE((v)->counter)

/**
 * atomic_xchg - atomic
 * @v: pointer to memory to change
 * @new: new value (technically passed in a register -- see xchg)
 */
#define atomic_xchg(v, new)	(xchg(&((v)->counter), (new)))


/**
 * atomic_cmpxchg - atomic compare-and-exchange values
=======
#define arch_atomic_read(v)		READ_ONCE((v)->counter)

/**
 * arch_atomic_xchg - atomic
 * @v: pointer to memory to change
 * @new: new value (technically passed in a register -- see xchg)
 */
#define arch_atomic_xchg(v, new)	(arch_xchg(&((v)->counter), (new)))


/**
 * arch_atomic_cmpxchg - atomic compare-and-exchange values
>>>>>>> upstream/android-13
 * @v: pointer to value to change
 * @old:  desired old value to match
 * @new:  new value to put in
 *
 * Parameters are then pointer, value-in-register, value-in-register,
 * and the output is the old value.
 *
 * Apparently this is complicated for archs that don't support
 * the memw_locked like we do (or it's broken or whatever).
 *
 * Kind of the lynchpin of the rest of the generically defined routines.
 * Remember V2 had that bug with dotnew predicate set by memw_locked.
 *
 * "old" is "expected" old val, __oldval is actual old value
 */
<<<<<<< HEAD
static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
=======
static inline int arch_atomic_cmpxchg(atomic_t *v, int old, int new)
>>>>>>> upstream/android-13
{
	int __oldval;

	asm volatile(
		"1:	%0 = memw_locked(%1);\n"
		"	{ P0 = cmp.eq(%0,%2);\n"
		"	  if (!P0.new) jump:nt 2f; }\n"
		"	memw_locked(%1,P0) = %3;\n"
		"	if (!P0) jump 1b;\n"
		"2:\n"
		: "=&r" (__oldval)
		: "r" (&v->counter), "r" (old), "r" (new)
		: "memory", "p0"
	);

	return __oldval;
}

#define ATOMIC_OP(op)							\
<<<<<<< HEAD
static inline void atomic_##op(int i, atomic_t *v)			\
=======
static inline void arch_atomic_##op(int i, atomic_t *v)			\
>>>>>>> upstream/android-13
{									\
	int output;							\
									\
	__asm__ __volatile__ (						\
		"1:	%0 = memw_locked(%1);\n"			\
		"	%0 = "#op "(%0,%2);\n"				\
		"	memw_locked(%1,P3)=%0;\n"			\
		"	if (!P3) jump 1b;\n"				\
		: "=&r" (output)					\
		: "r" (&v->counter), "r" (i)				\
		: "memory", "p3"					\
	);								\
}									\

#define ATOMIC_OP_RETURN(op)						\
<<<<<<< HEAD
static inline int atomic_##op##_return(int i, atomic_t *v)		\
=======
static inline int arch_atomic_##op##_return(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	int output;							\
									\
	__asm__ __volatile__ (						\
		"1:	%0 = memw_locked(%1);\n"			\
		"	%0 = "#op "(%0,%2);\n"				\
		"	memw_locked(%1,P3)=%0;\n"			\
		"	if (!P3) jump 1b;\n"				\
		: "=&r" (output)					\
		: "r" (&v->counter), "r" (i)				\
		: "memory", "p3"					\
	);								\
	return output;							\
}

#define ATOMIC_FETCH_OP(op)						\
<<<<<<< HEAD
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
=======
static inline int arch_atomic_fetch_##op(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	int output, val;						\
									\
	__asm__ __volatile__ (						\
		"1:	%0 = memw_locked(%2);\n"			\
		"	%1 = "#op "(%0,%3);\n"				\
		"	memw_locked(%2,P3)=%1;\n"			\
		"	if (!P3) jump 1b;\n"				\
		: "=&r" (output), "=&r" (val)				\
		: "r" (&v->counter), "r" (i)				\
		: "memory", "p3"					\
	);								\
	return output;							\
}

#define ATOMIC_OPS(op) ATOMIC_OP(op) ATOMIC_OP_RETURN(op) ATOMIC_FETCH_OP(op)

ATOMIC_OPS(add)
ATOMIC_OPS(sub)

#undef ATOMIC_OPS
#define ATOMIC_OPS(op) ATOMIC_OP(op) ATOMIC_FETCH_OP(op)

ATOMIC_OPS(and)
ATOMIC_OPS(or)
ATOMIC_OPS(xor)

#undef ATOMIC_OPS
#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP

/**
<<<<<<< HEAD
 * atomic_fetch_add_unless - add unless the number is a given value
=======
 * arch_atomic_fetch_add_unless - add unless the number is a given value
>>>>>>> upstream/android-13
 * @v: pointer to value
 * @a: amount to add
 * @u: unless value is equal to u
 *
 * Returns old value.
 *
 */

<<<<<<< HEAD
static inline int atomic_fetch_add_unless(atomic_t *v, int a, int u)
=======
static inline int arch_atomic_fetch_add_unless(atomic_t *v, int a, int u)
>>>>>>> upstream/android-13
{
	int __oldval;
	register int tmp;

	asm volatile(
		"1:	%0 = memw_locked(%2);"
		"	{"
		"		p3 = cmp.eq(%0, %4);"
		"		if (p3.new) jump:nt 2f;"
		"		%1 = add(%0, %3);"
		"	}"
		"	memw_locked(%2, p3) = %1;"
		"	{"
		"		if (!p3) jump 1b;"
		"	}"
		"2:"
		: "=&r" (__oldval), "=&r" (tmp)
		: "r" (v), "r" (a), "r" (u)
		: "memory", "p3"
	);
	return __oldval;
}
<<<<<<< HEAD
#define atomic_fetch_add_unless atomic_fetch_add_unless
=======
#define arch_atomic_fetch_add_unless arch_atomic_fetch_add_unless
>>>>>>> upstream/android-13

#endif
