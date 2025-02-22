/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ARCH_M68K_ATOMIC__
#define __ARCH_M68K_ATOMIC__

#include <linux/types.h>
#include <linux/irqflags.h>
#include <asm/cmpxchg.h>
#include <asm/barrier.h>

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

/*
 * We do not have SMP m68k systems, so we don't have to deal with that.
 */

<<<<<<< HEAD
#define ATOMIC_INIT(i)	{ (i) }

#define atomic_read(v)		READ_ONCE((v)->counter)
#define atomic_set(v, i)	WRITE_ONCE(((v)->counter), (i))
=======
#define arch_atomic_read(v)	READ_ONCE((v)->counter)
#define arch_atomic_set(v, i)	WRITE_ONCE(((v)->counter), (i))
>>>>>>> upstream/android-13

/*
 * The ColdFire parts cannot do some immediate to memory operations,
 * so for them we do not specify the "i" asm constraint.
 */
#ifdef CONFIG_COLDFIRE
#define	ASM_DI	"d"
#else
#define	ASM_DI	"di"
#endif

#define ATOMIC_OP(op, c_op, asm_op)					\
<<<<<<< HEAD
static inline void atomic_##op(int i, atomic_t *v)			\
=======
static inline void arch_atomic_##op(int i, atomic_t *v)			\
>>>>>>> upstream/android-13
{									\
	__asm__ __volatile__(#asm_op "l %1,%0" : "+m" (*v) : ASM_DI (i));\
}									\

#ifdef CONFIG_RMW_INSNS

#define ATOMIC_OP_RETURN(op, c_op, asm_op)				\
<<<<<<< HEAD
static inline int atomic_##op##_return(int i, atomic_t *v)		\
=======
static inline int arch_atomic_##op##_return(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	int t, tmp;							\
									\
	__asm__ __volatile__(						\
			"1:	movel %2,%1\n"				\
			"	" #asm_op "l %3,%1\n"			\
			"	casl %2,%1,%0\n"			\
			"	jne 1b"					\
			: "+m" (*v), "=&d" (t), "=&d" (tmp)		\
<<<<<<< HEAD
			: "g" (i), "2" (atomic_read(v)));		\
=======
			: "di" (i), "2" (arch_atomic_read(v)));		\
>>>>>>> upstream/android-13
	return t;							\
}

#define ATOMIC_FETCH_OP(op, c_op, asm_op)				\
<<<<<<< HEAD
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
=======
static inline int arch_atomic_fetch_##op(int i, atomic_t *v)		\
>>>>>>> upstream/android-13
{									\
	int t, tmp;							\
									\
	__asm__ __volatile__(						\
			"1:	movel %2,%1\n"				\
			"	" #asm_op "l %3,%1\n"			\
			"	casl %2,%1,%0\n"			\
			"	jne 1b"					\
			: "+m" (*v), "=&d" (t), "=&d" (tmp)		\
<<<<<<< HEAD
			: "g" (i), "2" (atomic_read(v)));		\
=======
			: "di" (i), "2" (arch_atomic_read(v)));		\
>>>>>>> upstream/android-13
	return tmp;							\
}

#else

#define ATOMIC_OP_RETURN(op, c_op, asm_op)				\
<<<<<<< HEAD
static inline int atomic_##op##_return(int i, atomic_t * v)		\
=======
static inline int arch_atomic_##op##_return(int i, atomic_t * v)	\
>>>>>>> upstream/android-13
{									\
	unsigned long flags;						\
	int t;								\
									\
	local_irq_save(flags);						\
	t = (v->counter c_op i);					\
	local_irq_restore(flags);					\
									\
	return t;							\
}

#define ATOMIC_FETCH_OP(op, c_op, asm_op)				\
<<<<<<< HEAD
static inline int atomic_fetch_##op(int i, atomic_t * v)		\
=======
static inline int arch_atomic_fetch_##op(int i, atomic_t * v)		\
>>>>>>> upstream/android-13
{									\
	unsigned long flags;						\
	int t;								\
									\
	local_irq_save(flags);						\
	t = v->counter;							\
	v->counter c_op i;						\
	local_irq_restore(flags);					\
									\
	return t;							\
}

#endif /* CONFIG_RMW_INSNS */

#define ATOMIC_OPS(op, c_op, asm_op)					\
	ATOMIC_OP(op, c_op, asm_op)					\
	ATOMIC_OP_RETURN(op, c_op, asm_op)				\
	ATOMIC_FETCH_OP(op, c_op, asm_op)

ATOMIC_OPS(add, +=, add)
ATOMIC_OPS(sub, -=, sub)

#undef ATOMIC_OPS
#define ATOMIC_OPS(op, c_op, asm_op)					\
	ATOMIC_OP(op, c_op, asm_op)					\
	ATOMIC_FETCH_OP(op, c_op, asm_op)

ATOMIC_OPS(and, &=, and)
ATOMIC_OPS(or, |=, or)
ATOMIC_OPS(xor, ^=, eor)

#undef ATOMIC_OPS
#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP

<<<<<<< HEAD
static inline void atomic_inc(atomic_t *v)
{
	__asm__ __volatile__("addql #1,%0" : "+m" (*v));
}
#define atomic_inc atomic_inc

static inline void atomic_dec(atomic_t *v)
{
	__asm__ __volatile__("subql #1,%0" : "+m" (*v));
}
#define atomic_dec atomic_dec

static inline int atomic_dec_and_test(atomic_t *v)
=======
static inline void arch_atomic_inc(atomic_t *v)
{
	__asm__ __volatile__("addql #1,%0" : "+m" (*v));
}
#define arch_atomic_inc arch_atomic_inc

static inline void arch_atomic_dec(atomic_t *v)
{
	__asm__ __volatile__("subql #1,%0" : "+m" (*v));
}
#define arch_atomic_dec arch_atomic_dec

static inline int arch_atomic_dec_and_test(atomic_t *v)
>>>>>>> upstream/android-13
{
	char c;
	__asm__ __volatile__("subql #1,%1; seq %0" : "=d" (c), "+m" (*v));
	return c != 0;
}
<<<<<<< HEAD
#define atomic_dec_and_test atomic_dec_and_test

static inline int atomic_dec_and_test_lt(atomic_t *v)
=======
#define arch_atomic_dec_and_test arch_atomic_dec_and_test

static inline int arch_atomic_dec_and_test_lt(atomic_t *v)
>>>>>>> upstream/android-13
{
	char c;
	__asm__ __volatile__(
		"subql #1,%1; slt %0"
		: "=d" (c), "=m" (*v)
		: "m" (*v));
	return c != 0;
}

<<<<<<< HEAD
static inline int atomic_inc_and_test(atomic_t *v)
=======
static inline int arch_atomic_inc_and_test(atomic_t *v)
>>>>>>> upstream/android-13
{
	char c;
	__asm__ __volatile__("addql #1,%1; seq %0" : "=d" (c), "+m" (*v));
	return c != 0;
}
<<<<<<< HEAD
#define atomic_inc_and_test atomic_inc_and_test

#ifdef CONFIG_RMW_INSNS

#define atomic_cmpxchg(v, o, n) ((int)cmpxchg(&((v)->counter), (o), (n)))
#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

#else /* !CONFIG_RMW_INSNS */

static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
=======
#define arch_atomic_inc_and_test arch_atomic_inc_and_test

#ifdef CONFIG_RMW_INSNS

#define arch_atomic_cmpxchg(v, o, n) ((int)arch_cmpxchg(&((v)->counter), (o), (n)))
#define arch_atomic_xchg(v, new) (arch_xchg(&((v)->counter), new))

#else /* !CONFIG_RMW_INSNS */

static inline int arch_atomic_cmpxchg(atomic_t *v, int old, int new)
>>>>>>> upstream/android-13
{
	unsigned long flags;
	int prev;

	local_irq_save(flags);
<<<<<<< HEAD
	prev = atomic_read(v);
	if (prev == old)
		atomic_set(v, new);
=======
	prev = arch_atomic_read(v);
	if (prev == old)
		arch_atomic_set(v, new);
>>>>>>> upstream/android-13
	local_irq_restore(flags);
	return prev;
}

<<<<<<< HEAD
static inline int atomic_xchg(atomic_t *v, int new)
=======
static inline int arch_atomic_xchg(atomic_t *v, int new)
>>>>>>> upstream/android-13
{
	unsigned long flags;
	int prev;

	local_irq_save(flags);
<<<<<<< HEAD
	prev = atomic_read(v);
	atomic_set(v, new);
=======
	prev = arch_atomic_read(v);
	arch_atomic_set(v, new);
>>>>>>> upstream/android-13
	local_irq_restore(flags);
	return prev;
}

#endif /* !CONFIG_RMW_INSNS */

<<<<<<< HEAD
static inline int atomic_sub_and_test(int i, atomic_t *v)
=======
static inline int arch_atomic_sub_and_test(int i, atomic_t *v)
>>>>>>> upstream/android-13
{
	char c;
	__asm__ __volatile__("subl %2,%1; seq %0"
			     : "=d" (c), "+m" (*v)
			     : ASM_DI (i));
	return c != 0;
}
<<<<<<< HEAD
#define atomic_sub_and_test atomic_sub_and_test

static inline int atomic_add_negative(int i, atomic_t *v)
=======
#define arch_atomic_sub_and_test arch_atomic_sub_and_test

static inline int arch_atomic_add_negative(int i, atomic_t *v)
>>>>>>> upstream/android-13
{
	char c;
	__asm__ __volatile__("addl %2,%1; smi %0"
			     : "=d" (c), "+m" (*v)
			     : ASM_DI (i));
	return c != 0;
}
<<<<<<< HEAD
#define atomic_add_negative atomic_add_negative
=======
#define arch_atomic_add_negative arch_atomic_add_negative
>>>>>>> upstream/android-13

#endif /* __ARCH_M68K_ATOMIC __ */
