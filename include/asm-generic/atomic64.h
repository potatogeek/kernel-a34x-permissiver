<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Generic implementation of 64-bit atomics using spinlocks,
 * useful on processors that don't have 64-bit atomic instructions.
 *
 * Copyright © 2009 Paul Mackerras, IBM Corp. <paulus@au1.ibm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#ifndef _ASM_GENERIC_ATOMIC64_H
#define _ASM_GENERIC_ATOMIC64_H
#include <linux/types.h>

typedef struct {
<<<<<<< HEAD
	long long counter;
=======
	s64 counter;
>>>>>>> upstream/android-13
} atomic64_t;

#define ATOMIC64_INIT(i)	{ (i) }

<<<<<<< HEAD
extern long long atomic64_read(const atomic64_t *v);
extern void	 atomic64_set(atomic64_t *v, long long i);

#define atomic64_set_release(v, i)	atomic64_set((v), (i))

#define ATOMIC64_OP(op)							\
extern void	 atomic64_##op(long long a, atomic64_t *v);

#define ATOMIC64_OP_RETURN(op)						\
extern long long atomic64_##op##_return(long long a, atomic64_t *v);

#define ATOMIC64_FETCH_OP(op)						\
extern long long atomic64_fetch_##op(long long a, atomic64_t *v);
=======
extern s64 generic_atomic64_read(const atomic64_t *v);
extern void generic_atomic64_set(atomic64_t *v, s64 i);

#define ATOMIC64_OP(op)							\
extern void generic_atomic64_##op(s64 a, atomic64_t *v);

#define ATOMIC64_OP_RETURN(op)						\
extern s64 generic_atomic64_##op##_return(s64 a, atomic64_t *v);

#define ATOMIC64_FETCH_OP(op)						\
extern s64 generic_atomic64_fetch_##op(s64 a, atomic64_t *v);
>>>>>>> upstream/android-13

#define ATOMIC64_OPS(op)	ATOMIC64_OP(op) ATOMIC64_OP_RETURN(op) ATOMIC64_FETCH_OP(op)

ATOMIC64_OPS(add)
ATOMIC64_OPS(sub)

#undef ATOMIC64_OPS
#define ATOMIC64_OPS(op)	ATOMIC64_OP(op) ATOMIC64_FETCH_OP(op)

ATOMIC64_OPS(and)
ATOMIC64_OPS(or)
ATOMIC64_OPS(xor)

#undef ATOMIC64_OPS
#undef ATOMIC64_FETCH_OP
#undef ATOMIC64_OP_RETURN
#undef ATOMIC64_OP

<<<<<<< HEAD
extern long long atomic64_dec_if_positive(atomic64_t *v);
#define atomic64_dec_if_positive atomic64_dec_if_positive
extern long long atomic64_cmpxchg(atomic64_t *v, long long o, long long n);
extern long long atomic64_xchg(atomic64_t *v, long long new);
extern long long atomic64_fetch_add_unless(atomic64_t *v, long long a, long long u);
#define atomic64_fetch_add_unless atomic64_fetch_add_unless
=======
extern s64 generic_atomic64_dec_if_positive(atomic64_t *v);
extern s64 generic_atomic64_cmpxchg(atomic64_t *v, s64 o, s64 n);
extern s64 generic_atomic64_xchg(atomic64_t *v, s64 new);
extern s64 generic_atomic64_fetch_add_unless(atomic64_t *v, s64 a, s64 u);

#define arch_atomic64_read		generic_atomic64_read
#define arch_atomic64_set		generic_atomic64_set
#define arch_atomic64_set_release	generic_atomic64_set

#define arch_atomic64_add		generic_atomic64_add
#define arch_atomic64_add_return	generic_atomic64_add_return
#define arch_atomic64_fetch_add		generic_atomic64_fetch_add
#define arch_atomic64_sub		generic_atomic64_sub
#define arch_atomic64_sub_return	generic_atomic64_sub_return
#define arch_atomic64_fetch_sub		generic_atomic64_fetch_sub

#define arch_atomic64_and		generic_atomic64_and
#define arch_atomic64_fetch_and		generic_atomic64_fetch_and
#define arch_atomic64_or		generic_atomic64_or
#define arch_atomic64_fetch_or		generic_atomic64_fetch_or
#define arch_atomic64_xor		generic_atomic64_xor
#define arch_atomic64_fetch_xor		generic_atomic64_fetch_xor

#define arch_atomic64_dec_if_positive	generic_atomic64_dec_if_positive
#define arch_atomic64_cmpxchg		generic_atomic64_cmpxchg
#define arch_atomic64_xchg		generic_atomic64_xchg
#define arch_atomic64_fetch_add_unless	generic_atomic64_fetch_add_unless
>>>>>>> upstream/android-13

#endif  /*  _ASM_GENERIC_ATOMIC64_H  */
