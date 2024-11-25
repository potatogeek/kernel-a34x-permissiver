<<<<<<< HEAD
/*
 * Generic C implementation of atomic counter operations. Usable on
 * UP systems only. Do not include in machine independent code.
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Generic C implementation of atomic counter operations. Do not include in
 * machine independent code.
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
>>>>>>> upstream/android-13
 */
#ifndef __ASM_GENERIC_ATOMIC_H
#define __ASM_GENERIC_ATOMIC_H

#include <asm/cmpxchg.h>
#include <asm/barrier.h>

<<<<<<< HEAD
/*
 * atomic_$op() - $op integer to atomic variable
 * @i: integer value to $op
 * @v: pointer to the atomic variable
 *
 * Atomically $ops @i to @v. Does not strictly guarantee a memory-barrier, use
 * smp_mb__{before,after}_atomic().
 */

/*
 * atomic_$op_return() - $op interer to atomic variable and returns the result
 * @i: integer value to $op
 * @v: pointer to the atomic variable
 *
 * Atomically $ops @i to @v. Does imply a full memory barrier.
 */

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_SMP

/* we can build all atomic primitives from cmpxchg */

#define ATOMIC_OP(op, c_op)						\
<<<<<<< HEAD
static inline void atomic_##op(int i, atomic_t *v)			\
=======
static inline void generic_atomic_##op(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	int c, old;							\
									\
	c = v->counter;							\
<<<<<<< HEAD
	while ((old = cmpxchg(&v->counter, c, c c_op i)) != c)		\
=======
	while ((old = arch_cmpxchg(&v->counter, c, c c_op i)) != c)	\
>>>>>>> upstream/android-13
		c = old;						\
}

#define ATOMIC_OP_RETURN(op, c_op)					\
<<<<<<< HEAD
static inline int atomic_##op##_return(int i, atomic_t *v)		\
=======
static inline int generic_atomic_##op##_return(int i, atomic_t *v)	\
>>>>>>> upstream/android-13
{									\
	int c, old;							\
									\
	c = v->counter;							\
<<<<<<< HEAD
	while ((old = cmpxchg(&v->counter, c, c c_op i)) != c)		\
=======
	while ((old = arch_cmpxchg(&v->counter, c, c c_op i)) != c)	\
>>>>>>> upstream/android-13
		c = old;						\
									\
	return c c_op i;						\
}

#define ATOMIC_FETCH_OP(op, c_op)					\
<<<<<<< HEAD
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
=======
static inline int generic_atomic_fetch_##op(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	int c, old;							\
									\
	c = v->counter;							\
<<<<<<< HEAD
	while ((old = cmpxchg(&v->counter, c, c c_op i)) != c)		\
=======
	while ((old = arch_cmpxchg(&v->counter, c, c c_op i)) != c)	\
>>>>>>> upstream/android-13
		c = old;						\
									\
	return c;							\
}

#else

#include <linux/irqflags.h>

#define ATOMIC_OP(op, c_op)						\
<<<<<<< HEAD
static inline void atomic_##op(int i, atomic_t *v)			\
=======
static inline void generic_atomic_##op(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	unsigned long flags;						\
									\
	raw_local_irq_save(flags);					\
	v->counter = v->counter c_op i;					\
	raw_local_irq_restore(flags);					\
}

#define ATOMIC_OP_RETURN(op, c_op)					\
<<<<<<< HEAD
static inline int atomic_##op##_return(int i, atomic_t *v)		\
=======
static inline int generic_atomic_##op##_return(int i, atomic_t *v)	\
>>>>>>> upstream/android-13
{									\
	unsigned long flags;						\
	int ret;							\
									\
	raw_local_irq_save(flags);					\
	ret = (v->counter = v->counter c_op i);				\
	raw_local_irq_restore(flags);					\
									\
	return ret;							\
}

#define ATOMIC_FETCH_OP(op, c_op)					\
<<<<<<< HEAD
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
=======
static inline int generic_atomic_fetch_##op(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	unsigned long flags;						\
	int ret;							\
									\
	raw_local_irq_save(flags);					\
	ret = v->counter;						\
	v->counter = v->counter c_op i;					\
	raw_local_irq_restore(flags);					\
									\
	return ret;							\
}

#endif /* CONFIG_SMP */

<<<<<<< HEAD
#ifndef atomic_add_return
ATOMIC_OP_RETURN(add, +)
#endif

#ifndef atomic_sub_return
ATOMIC_OP_RETURN(sub, -)
#endif

#ifndef atomic_fetch_add
ATOMIC_FETCH_OP(add, +)
#endif

#ifndef atomic_fetch_sub
ATOMIC_FETCH_OP(sub, -)
#endif

#ifndef atomic_fetch_and
ATOMIC_FETCH_OP(and, &)
#endif

#ifndef atomic_fetch_or
ATOMIC_FETCH_OP(or, |)
#endif

#ifndef atomic_fetch_xor
ATOMIC_FETCH_OP(xor, ^)
#endif

#ifndef atomic_and
ATOMIC_OP(and, &)
#endif

#ifndef atomic_or
ATOMIC_OP(or, |)
#endif

#ifndef atomic_xor
ATOMIC_OP(xor, ^)
#endif
=======
ATOMIC_OP_RETURN(add, +)
ATOMIC_OP_RETURN(sub, -)

ATOMIC_FETCH_OP(add, +)
ATOMIC_FETCH_OP(sub, -)
ATOMIC_FETCH_OP(and, &)
ATOMIC_FETCH_OP(or, |)
ATOMIC_FETCH_OP(xor, ^)

ATOMIC_OP(add, +)
ATOMIC_OP(sub, -)
ATOMIC_OP(and, &)
ATOMIC_OP(or, |)
ATOMIC_OP(xor, ^)
>>>>>>> upstream/android-13

#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP

<<<<<<< HEAD
/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

#define ATOMIC_INIT(i)	{ (i) }

/**
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.
 */
#ifndef atomic_read
#define atomic_read(v)	READ_ONCE((v)->counter)
#endif

/**
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
#define atomic_set(v, i) WRITE_ONCE(((v)->counter), (i))

#include <linux/irqflags.h>

static inline void atomic_add(int i, atomic_t *v)
{
	atomic_add_return(i, v);
}

static inline void atomic_sub(int i, atomic_t *v)
{
	atomic_sub_return(i, v);
}

#define atomic_xchg(ptr, v)		(xchg(&(ptr)->counter, (v)))
#define atomic_cmpxchg(v, old, new)	(cmpxchg(&((v)->counter), (old), (new)))
=======
#define arch_atomic_add_return			generic_atomic_add_return
#define arch_atomic_sub_return			generic_atomic_sub_return

#define arch_atomic_fetch_add			generic_atomic_fetch_add
#define arch_atomic_fetch_sub			generic_atomic_fetch_sub
#define arch_atomic_fetch_and			generic_atomic_fetch_and
#define arch_atomic_fetch_or			generic_atomic_fetch_or
#define arch_atomic_fetch_xor			generic_atomic_fetch_xor

#define arch_atomic_add				generic_atomic_add
#define arch_atomic_sub				generic_atomic_sub
#define arch_atomic_and				generic_atomic_and
#define arch_atomic_or				generic_atomic_or
#define arch_atomic_xor				generic_atomic_xor

#define arch_atomic_read(v)			READ_ONCE((v)->counter)
#define arch_atomic_set(v, i)			WRITE_ONCE(((v)->counter), (i))

#define arch_atomic_xchg(ptr, v)		(arch_xchg(&(ptr)->counter, (v)))
#define arch_atomic_cmpxchg(v, old, new)	(arch_cmpxchg(&((v)->counter), (old), (new)))
>>>>>>> upstream/android-13

#endif /* __ASM_GENERIC_ATOMIC_H */
