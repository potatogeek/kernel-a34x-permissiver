
#ifndef __ASM_ATOMIC_H
#define __ASM_ATOMIC_H

#include <linux/compiler.h>
#include <linux/types.h>

#include <asm/barrier.h>
#include <asm/lse.h>

#ifdef __KERNEL__

#define __ARM64_IN_ATOMIC_IMPL

#if defined(CONFIG_ARM64_LSE_ATOMICS) && defined(CONFIG_AS_LSE)
#include <asm/atomic_lse.h>
#else
#include <asm/atomic_ll_sc.h>
#endif

#undef __ARM64_IN_ATOMIC_IMPL

#include <asm/cmpxchg.h>

#define ATOMIC_INIT(i)	{ (i) }

#define atomic_read(v)			READ_ONCE((v)->counter)
#define atomic_set(v, i)		WRITE_ONCE(((v)->counter), (i))

#define atomic_add_return_relaxed	atomic_add_return_relaxed
#define atomic_add_return_acquire	atomic_add_return_acquire
#define atomic_add_return_release	atomic_add_return_release
#define atomic_add_return		atomic_add_return

#define atomic_sub_return_relaxed	atomic_sub_return_relaxed
#define atomic_sub_return_acquire	atomic_sub_return_acquire
#define atomic_sub_return_release	atomic_sub_return_release
#define atomic_sub_return		atomic_sub_return

#define atomic_fetch_add_relaxed	atomic_fetch_add_relaxed
#define atomic_fetch_add_acquire	atomic_fetch_add_acquire
#define atomic_fetch_add_release	atomic_fetch_add_release
#define atomic_fetch_add		atomic_fetch_add

#define atomic_fetch_sub_relaxed	atomic_fetch_sub_relaxed
#define atomic_fetch_sub_acquire	atomic_fetch_sub_acquire
#define atomic_fetch_sub_release	atomic_fetch_sub_release
#define atomic_fetch_sub		atomic_fetch_sub

#define atomic_fetch_and_relaxed	atomic_fetch_and_relaxed
#define atomic_fetch_and_acquire	atomic_fetch_and_acquire
#define atomic_fetch_and_release	atomic_fetch_and_release
#define atomic_fetch_and		atomic_fetch_and

#define atomic_fetch_andnot_relaxed	atomic_fetch_andnot_relaxed
#define atomic_fetch_andnot_acquire	atomic_fetch_andnot_acquire
#define atomic_fetch_andnot_release	atomic_fetch_andnot_release
#define atomic_fetch_andnot		atomic_fetch_andnot

#define atomic_fetch_or_relaxed		atomic_fetch_or_relaxed
#define atomic_fetch_or_acquire		atomic_fetch_or_acquire
#define atomic_fetch_or_release		atomic_fetch_or_release
#define atomic_fetch_or			atomic_fetch_or

#define atomic_fetch_xor_relaxed	atomic_fetch_xor_relaxed
#define atomic_fetch_xor_acquire	atomic_fetch_xor_acquire
#define atomic_fetch_xor_release	atomic_fetch_xor_release
#define atomic_fetch_xor		atomic_fetch_xor

#define atomic_xchg_relaxed(v, new)	xchg_relaxed(&((v)->counter), (new))
#define atomic_xchg_acquire(v, new)	xchg_acquire(&((v)->counter), (new))
#define atomic_xchg_release(v, new)	xchg_release(&((v)->counter), (new))
#define atomic_xchg(v, new)		xchg(&((v)->counter), (new))

#define atomic_cmpxchg_relaxed(v, old, new)				\
	cmpxchg_relaxed(&((v)->counter), (old), (new))
#define atomic_cmpxchg_acquire(v, old, new)				\
	cmpxchg_acquire(&((v)->counter), (old), (new))
#define atomic_cmpxchg_release(v, old, new)				\
	cmpxchg_release(&((v)->counter), (old), (new))
#define atomic_cmpxchg(v, old, new)	cmpxchg(&((v)->counter), (old), (new))

#define atomic_andnot			atomic_andnot


#define ATOMIC64_INIT			ATOMIC_INIT
#define atomic64_read			atomic_read
#define atomic64_set			atomic_set

#define atomic64_add_return_relaxed	atomic64_add_return_relaxed
#define atomic64_add_return_acquire	atomic64_add_return_acquire
#define atomic64_add_return_release	atomic64_add_return_release
#define atomic64_add_return		atomic64_add_return

#define atomic64_sub_return_relaxed	atomic64_sub_return_relaxed
#define atomic64_sub_return_acquire	atomic64_sub_return_acquire
#define atomic64_sub_return_release	atomic64_sub_return_release
#define atomic64_sub_return		atomic64_sub_return

#define atomic64_fetch_add_relaxed	atomic64_fetch_add_relaxed
#define atomic64_fetch_add_acquire	atomic64_fetch_add_acquire
#define atomic64_fetch_add_release	atomic64_fetch_add_release
#define atomic64_fetch_add		atomic64_fetch_add

#define atomic64_fetch_sub_relaxed	atomic64_fetch_sub_relaxed
#define atomic64_fetch_sub_acquire	atomic64_fetch_sub_acquire
#define atomic64_fetch_sub_release	atomic64_fetch_sub_release
#define atomic64_fetch_sub		atomic64_fetch_sub

#define atomic64_fetch_and_relaxed	atomic64_fetch_and_relaxed
#define atomic64_fetch_and_acquire	atomic64_fetch_and_acquire
#define atomic64_fetch_and_release	atomic64_fetch_and_release
#define atomic64_fetch_and		atomic64_fetch_and

#define atomic64_fetch_andnot_relaxed	atomic64_fetch_andnot_relaxed
#define atomic64_fetch_andnot_acquire	atomic64_fetch_andnot_acquire
#define atomic64_fetch_andnot_release	atomic64_fetch_andnot_release
#define atomic64_fetch_andnot		atomic64_fetch_andnot

#define atomic64_fetch_or_relaxed	atomic64_fetch_or_relaxed
#define atomic64_fetch_or_acquire	atomic64_fetch_or_acquire
#define atomic64_fetch_or_release	atomic64_fetch_or_release
#define atomic64_fetch_or		atomic64_fetch_or

#define atomic64_fetch_xor_relaxed	atomic64_fetch_xor_relaxed
#define atomic64_fetch_xor_acquire	atomic64_fetch_xor_acquire
#define atomic64_fetch_xor_release	atomic64_fetch_xor_release
#define atomic64_fetch_xor		atomic64_fetch_xor

#define atomic64_xchg_relaxed		atomic_xchg_relaxed
#define atomic64_xchg_acquire		atomic_xchg_acquire
#define atomic64_xchg_release		atomic_xchg_release
#define atomic64_xchg			atomic_xchg

#define atomic64_cmpxchg_relaxed	atomic_cmpxchg_relaxed
#define atomic64_cmpxchg_acquire	atomic_cmpxchg_acquire
#define atomic64_cmpxchg_release	atomic_cmpxchg_release
#define atomic64_cmpxchg		atomic_cmpxchg

#define atomic64_andnot			atomic64_andnot

#define atomic64_dec_if_positive	atomic64_dec_if_positive

#endif
#endif
