<<<<<<< HEAD
/*
 * Copyright (C) 2013 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2013 ARM Ltd.
>>>>>>> upstream/android-13
 */
#ifndef __ASM_PERCPU_H
#define __ASM_PERCPU_H

#include <linux/preempt.h>

#include <asm/alternative.h>
#include <asm/cmpxchg.h>
#include <asm/stack_pointer.h>

static inline void set_my_cpu_offset(unsigned long off)
{
	asm volatile(ALTERNATIVE("msr tpidr_el1, %0",
				 "msr tpidr_el2, %0",
				 ARM64_HAS_VIRT_HOST_EXTN)
			:: "r" (off) : "memory");
}

<<<<<<< HEAD
static inline unsigned long __my_cpu_offset(void)
=======
static inline unsigned long __hyp_my_cpu_offset(void)
{
	/*
	 * Non-VHE hyp code runs with preemption disabled. No need to hazard
	 * the register access against barrier() as in __kern_my_cpu_offset.
	 */
	return read_sysreg(tpidr_el2);
}

static inline unsigned long __kern_my_cpu_offset(void)
>>>>>>> upstream/android-13
{
	unsigned long off;

	/*
	 * We want to allow caching the value, so avoid using volatile and
	 * instead use a fake stack read to hazard against barrier().
	 */
	asm(ALTERNATIVE("mrs %0, tpidr_el1",
			"mrs %0, tpidr_el2",
			ARM64_HAS_VIRT_HOST_EXTN)
		: "=r" (off) :
		"Q" (*(const unsigned long *)current_stack_pointer));

	return off;
}
<<<<<<< HEAD
#define __my_cpu_offset __my_cpu_offset()

#define PERCPU_OP(op, asm_op)						\
static inline unsigned long __percpu_##op(void *ptr,			\
			unsigned long val, int size)			\
{									\
	unsigned long loop, ret;					\
									\
	switch (size) {							\
	case 1:								\
		asm ("//__per_cpu_" #op "_1\n"				\
		"1:	ldxrb	  %w[ret], %[ptr]\n"			\
			#asm_op " %w[ret], %w[ret], %w[val]\n"		\
		"	stxrb	  %w[loop], %w[ret], %[ptr]\n"		\
		"	cbnz	  %w[loop], 1b"				\
		: [loop] "=&r" (loop), [ret] "=&r" (ret),		\
		  [ptr] "+Q"(*(u8 *)ptr)				\
		: [val] "Ir" (val));					\
		break;							\
	case 2:								\
		asm ("//__per_cpu_" #op "_2\n"				\
		"1:	ldxrh	  %w[ret], %[ptr]\n"			\
			#asm_op " %w[ret], %w[ret], %w[val]\n"		\
		"	stxrh	  %w[loop], %w[ret], %[ptr]\n"		\
		"	cbnz	  %w[loop], 1b"				\
		: [loop] "=&r" (loop), [ret] "=&r" (ret),		\
		  [ptr]  "+Q"(*(u16 *)ptr)				\
		: [val] "Ir" (val));					\
		break;							\
	case 4:								\
		asm ("//__per_cpu_" #op "_4\n"				\
		"1:	ldxr	  %w[ret], %[ptr]\n"			\
			#asm_op " %w[ret], %w[ret], %w[val]\n"		\
		"	stxr	  %w[loop], %w[ret], %[ptr]\n"		\
		"	cbnz	  %w[loop], 1b"				\
		: [loop] "=&r" (loop), [ret] "=&r" (ret),		\
		  [ptr] "+Q"(*(u32 *)ptr)				\
		: [val] "Ir" (val));					\
		break;							\
	case 8:								\
		asm ("//__per_cpu_" #op "_8\n"				\
		"1:	ldxr	  %[ret], %[ptr]\n"			\
			#asm_op " %[ret], %[ret], %[val]\n"		\
		"	stxr	  %w[loop], %[ret], %[ptr]\n"		\
		"	cbnz	  %w[loop], 1b"				\
		: [loop] "=&r" (loop), [ret] "=&r" (ret),		\
		  [ptr] "+Q"(*(u64 *)ptr)				\
		: [val] "Ir" (val));					\
		break;							\
	default:							\
		ret = 0;						\
		BUILD_BUG();						\
	}								\
=======

#ifdef __KVM_NVHE_HYPERVISOR__
#define __my_cpu_offset __hyp_my_cpu_offset()
#else
#define __my_cpu_offset __kern_my_cpu_offset()
#endif

#define PERCPU_RW_OPS(sz)						\
static inline unsigned long __percpu_read_##sz(void *ptr)		\
{									\
	return READ_ONCE(*(u##sz *)ptr);				\
}									\
									\
static inline void __percpu_write_##sz(void *ptr, unsigned long val)	\
{									\
	WRITE_ONCE(*(u##sz *)ptr, (u##sz)val);				\
}

#define __PERCPU_OP_CASE(w, sfx, name, sz, op_llsc, op_lse)		\
static inline void							\
__percpu_##name##_case_##sz(void *ptr, unsigned long val)		\
{									\
	unsigned int loop;						\
	u##sz tmp;							\
									\
	asm volatile (ARM64_LSE_ATOMIC_INSN(				\
	/* LL/SC */							\
	"1:	ldxr" #sfx "\t%" #w "[tmp], %[ptr]\n"			\
		#op_llsc "\t%" #w "[tmp], %" #w "[tmp], %" #w "[val]\n"	\
	"	stxr" #sfx "\t%w[loop], %" #w "[tmp], %[ptr]\n"		\
	"	cbnz	%w[loop], 1b",					\
	/* LSE atomics */						\
		#op_lse "\t%" #w "[val], %[ptr]\n"			\
		__nops(3))						\
	: [loop] "=&r" (loop), [tmp] "=&r" (tmp),			\
	  [ptr] "+Q"(*(u##sz *)ptr)					\
	: [val] "r" ((u##sz)(val)));					\
}

#define __PERCPU_RET_OP_CASE(w, sfx, name, sz, op_llsc, op_lse)		\
static inline u##sz							\
__percpu_##name##_return_case_##sz(void *ptr, unsigned long val)	\
{									\
	unsigned int loop;						\
	u##sz ret;							\
									\
	asm volatile (ARM64_LSE_ATOMIC_INSN(				\
	/* LL/SC */							\
	"1:	ldxr" #sfx "\t%" #w "[ret], %[ptr]\n"			\
		#op_llsc "\t%" #w "[ret], %" #w "[ret], %" #w "[val]\n"	\
	"	stxr" #sfx "\t%w[loop], %" #w "[ret], %[ptr]\n"		\
	"	cbnz	%w[loop], 1b",					\
	/* LSE atomics */						\
		#op_lse "\t%" #w "[val], %" #w "[ret], %[ptr]\n"	\
		#op_llsc "\t%" #w "[ret], %" #w "[ret], %" #w "[val]\n"	\
		__nops(2))						\
	: [loop] "=&r" (loop), [ret] "=&r" (ret),			\
	  [ptr] "+Q"(*(u##sz *)ptr)					\
	: [val] "r" ((u##sz)(val)));					\
>>>>>>> upstream/android-13
									\
	return ret;							\
}

<<<<<<< HEAD
PERCPU_OP(add, add)
PERCPU_OP(and, and)
PERCPU_OP(or, orr)
#undef PERCPU_OP

static inline unsigned long __percpu_read(void *ptr, int size)
{
	unsigned long ret;

	switch (size) {
	case 1:
		ret = READ_ONCE(*(u8 *)ptr);
		break;
	case 2:
		ret = READ_ONCE(*(u16 *)ptr);
		break;
	case 4:
		ret = READ_ONCE(*(u32 *)ptr);
		break;
	case 8:
		ret = READ_ONCE(*(u64 *)ptr);
		break;
	default:
		ret = 0;
		BUILD_BUG();
	}

	return ret;
}

static inline void __percpu_write(void *ptr, unsigned long val, int size)
{
	switch (size) {
	case 1:
		WRITE_ONCE(*(u8 *)ptr, (u8)val);
		break;
	case 2:
		WRITE_ONCE(*(u16 *)ptr, (u16)val);
		break;
	case 4:
		WRITE_ONCE(*(u32 *)ptr, (u32)val);
		break;
	case 8:
		WRITE_ONCE(*(u64 *)ptr, (u64)val);
		break;
	default:
		BUILD_BUG();
	}
}

static inline unsigned long __percpu_xchg(void *ptr, unsigned long val,
						int size)
{
	unsigned long ret, loop;

	switch (size) {
	case 1:
		asm ("//__percpu_xchg_1\n"
		"1:	ldxrb	%w[ret], %[ptr]\n"
		"	stxrb	%w[loop], %w[val], %[ptr]\n"
		"	cbnz	%w[loop], 1b"
		: [loop] "=&r"(loop), [ret] "=&r"(ret),
		  [ptr] "+Q"(*(u8 *)ptr)
		: [val] "r" (val));
		break;
	case 2:
		asm ("//__percpu_xchg_2\n"
		"1:	ldxrh	%w[ret], %[ptr]\n"
		"	stxrh	%w[loop], %w[val], %[ptr]\n"
		"	cbnz	%w[loop], 1b"
		: [loop] "=&r"(loop), [ret] "=&r"(ret),
		  [ptr] "+Q"(*(u16 *)ptr)
		: [val] "r" (val));
		break;
	case 4:
		asm ("//__percpu_xchg_4\n"
		"1:	ldxr	%w[ret], %[ptr]\n"
		"	stxr	%w[loop], %w[val], %[ptr]\n"
		"	cbnz	%w[loop], 1b"
		: [loop] "=&r"(loop), [ret] "=&r"(ret),
		  [ptr] "+Q"(*(u32 *)ptr)
		: [val] "r" (val));
		break;
	case 8:
		asm ("//__percpu_xchg_8\n"
		"1:	ldxr	%[ret], %[ptr]\n"
		"	stxr	%w[loop], %[val], %[ptr]\n"
		"	cbnz	%w[loop], 1b"
		: [loop] "=&r"(loop), [ret] "=&r"(ret),
		  [ptr] "+Q"(*(u64 *)ptr)
		: [val] "r" (val));
		break;
	default:
		ret = 0;
		BUILD_BUG();
	}

	return ret;
}

/* this_cpu_cmpxchg */
#define _protect_cmpxchg_local(pcp, o, n)			\
({								\
	typeof(*raw_cpu_ptr(&(pcp))) __ret;			\
	preempt_disable();					\
	__ret = cmpxchg_local(raw_cpu_ptr(&(pcp)), o, n);	\
	preempt_enable();					\
	__ret;							\
})

#define this_cpu_cmpxchg_1(ptr, o, n) _protect_cmpxchg_local(ptr, o, n)
#define this_cpu_cmpxchg_2(ptr, o, n) _protect_cmpxchg_local(ptr, o, n)
#define this_cpu_cmpxchg_4(ptr, o, n) _protect_cmpxchg_local(ptr, o, n)
#define this_cpu_cmpxchg_8(ptr, o, n) _protect_cmpxchg_local(ptr, o, n)

#define this_cpu_cmpxchg_double_8(ptr1, ptr2, o1, o2, n1, n2)		\
({									\
	int __ret;							\
	preempt_disable();						\
	__ret = cmpxchg_double_local(	raw_cpu_ptr(&(ptr1)),		\
					raw_cpu_ptr(&(ptr2)),		\
					o1, o2, n1, n2);		\
	preempt_enable();						\
	__ret;								\
})

#define _percpu_read(pcp)						\
({									\
	typeof(pcp) __retval;						\
	preempt_disable_notrace();					\
	__retval = (typeof(pcp))__percpu_read(raw_cpu_ptr(&(pcp)), 	\
					      sizeof(pcp));		\
=======
#define PERCPU_OP(name, op_llsc, op_lse)				\
	__PERCPU_OP_CASE(w, b, name,  8, op_llsc, op_lse)		\
	__PERCPU_OP_CASE(w, h, name, 16, op_llsc, op_lse)		\
	__PERCPU_OP_CASE(w,  , name, 32, op_llsc, op_lse)		\
	__PERCPU_OP_CASE( ,  , name, 64, op_llsc, op_lse)

#define PERCPU_RET_OP(name, op_llsc, op_lse)				\
	__PERCPU_RET_OP_CASE(w, b, name,  8, op_llsc, op_lse)		\
	__PERCPU_RET_OP_CASE(w, h, name, 16, op_llsc, op_lse)		\
	__PERCPU_RET_OP_CASE(w,  , name, 32, op_llsc, op_lse)		\
	__PERCPU_RET_OP_CASE( ,  , name, 64, op_llsc, op_lse)

PERCPU_RW_OPS(8)
PERCPU_RW_OPS(16)
PERCPU_RW_OPS(32)
PERCPU_RW_OPS(64)
PERCPU_OP(add, add, stadd)
PERCPU_OP(andnot, bic, stclr)
PERCPU_OP(or, orr, stset)
PERCPU_RET_OP(add, add, ldadd)

#undef PERCPU_RW_OPS
#undef __PERCPU_OP_CASE
#undef __PERCPU_RET_OP_CASE
#undef PERCPU_OP
#undef PERCPU_RET_OP

/*
 * It would be nice to avoid the conditional call into the scheduler when
 * re-enabling preemption for preemptible kernels, but doing that in a way
 * which builds inside a module would mean messing directly with the preempt
 * count. If you do this, peterz and tglx will hunt you down.
 */
#define this_cpu_cmpxchg_double_8(ptr1, ptr2, o1, o2, n1, n2)		\
({									\
	int __ret;							\
	preempt_disable_notrace();					\
	__ret = cmpxchg_double_local(	raw_cpu_ptr(&(ptr1)),		\
					raw_cpu_ptr(&(ptr2)),		\
					o1, o2, n1, n2);		\
	preempt_enable_notrace();					\
	__ret;								\
})

#define _pcp_protect(op, pcp, ...)					\
({									\
	preempt_disable_notrace();					\
	op(raw_cpu_ptr(&(pcp)), __VA_ARGS__);				\
	preempt_enable_notrace();					\
})

#define _pcp_protect_return(op, pcp, args...)				\
({									\
	typeof(pcp) __retval;						\
	preempt_disable_notrace();					\
	__retval = (typeof(pcp))op(raw_cpu_ptr(&(pcp)), ##args);	\
>>>>>>> upstream/android-13
	preempt_enable_notrace();					\
	__retval;							\
})

<<<<<<< HEAD
#define _percpu_write(pcp, val)						\
do {									\
	preempt_disable_notrace();					\
	__percpu_write(raw_cpu_ptr(&(pcp)), (unsigned long)(val), 	\
				sizeof(pcp));				\
	preempt_enable_notrace();					\
} while(0)								\

#define _pcp_protect(operation, pcp, val)			\
({								\
	typeof(pcp) __retval;					\
	preempt_disable();					\
	__retval = (typeof(pcp))operation(raw_cpu_ptr(&(pcp)),	\
					  (val), sizeof(pcp));	\
	preempt_enable();					\
	__retval;						\
})

#define _percpu_add(pcp, val) \
	_pcp_protect(__percpu_add, pcp, val)

#define _percpu_add_return(pcp, val) _percpu_add(pcp, val)

#define _percpu_and(pcp, val) \
	_pcp_protect(__percpu_and, pcp, val)

#define _percpu_or(pcp, val) \
	_pcp_protect(__percpu_or, pcp, val)

#define _percpu_xchg(pcp, val) (typeof(pcp)) \
	_pcp_protect(__percpu_xchg, pcp, (unsigned long)(val))

#define this_cpu_add_1(pcp, val) _percpu_add(pcp, val)
#define this_cpu_add_2(pcp, val) _percpu_add(pcp, val)
#define this_cpu_add_4(pcp, val) _percpu_add(pcp, val)
#define this_cpu_add_8(pcp, val) _percpu_add(pcp, val)

#define this_cpu_add_return_1(pcp, val) _percpu_add_return(pcp, val)
#define this_cpu_add_return_2(pcp, val) _percpu_add_return(pcp, val)
#define this_cpu_add_return_4(pcp, val) _percpu_add_return(pcp, val)
#define this_cpu_add_return_8(pcp, val) _percpu_add_return(pcp, val)

#define this_cpu_and_1(pcp, val) _percpu_and(pcp, val)
#define this_cpu_and_2(pcp, val) _percpu_and(pcp, val)
#define this_cpu_and_4(pcp, val) _percpu_and(pcp, val)
#define this_cpu_and_8(pcp, val) _percpu_and(pcp, val)

#define this_cpu_or_1(pcp, val) _percpu_or(pcp, val)
#define this_cpu_or_2(pcp, val) _percpu_or(pcp, val)
#define this_cpu_or_4(pcp, val) _percpu_or(pcp, val)
#define this_cpu_or_8(pcp, val) _percpu_or(pcp, val)

#define this_cpu_read_1(pcp) _percpu_read(pcp)
#define this_cpu_read_2(pcp) _percpu_read(pcp)
#define this_cpu_read_4(pcp) _percpu_read(pcp)
#define this_cpu_read_8(pcp) _percpu_read(pcp)

#define this_cpu_write_1(pcp, val) _percpu_write(pcp, val)
#define this_cpu_write_2(pcp, val) _percpu_write(pcp, val)
#define this_cpu_write_4(pcp, val) _percpu_write(pcp, val)
#define this_cpu_write_8(pcp, val) _percpu_write(pcp, val)

#define this_cpu_xchg_1(pcp, val) _percpu_xchg(pcp, val)
#define this_cpu_xchg_2(pcp, val) _percpu_xchg(pcp, val)
#define this_cpu_xchg_4(pcp, val) _percpu_xchg(pcp, val)
#define this_cpu_xchg_8(pcp, val) _percpu_xchg(pcp, val)

#include <asm-generic/percpu.h>

=======
#define this_cpu_read_1(pcp)		\
	_pcp_protect_return(__percpu_read_8, pcp)
#define this_cpu_read_2(pcp)		\
	_pcp_protect_return(__percpu_read_16, pcp)
#define this_cpu_read_4(pcp)		\
	_pcp_protect_return(__percpu_read_32, pcp)
#define this_cpu_read_8(pcp)		\
	_pcp_protect_return(__percpu_read_64, pcp)

#define this_cpu_write_1(pcp, val)	\
	_pcp_protect(__percpu_write_8, pcp, (unsigned long)val)
#define this_cpu_write_2(pcp, val)	\
	_pcp_protect(__percpu_write_16, pcp, (unsigned long)val)
#define this_cpu_write_4(pcp, val)	\
	_pcp_protect(__percpu_write_32, pcp, (unsigned long)val)
#define this_cpu_write_8(pcp, val)	\
	_pcp_protect(__percpu_write_64, pcp, (unsigned long)val)

#define this_cpu_add_1(pcp, val)	\
	_pcp_protect(__percpu_add_case_8, pcp, val)
#define this_cpu_add_2(pcp, val)	\
	_pcp_protect(__percpu_add_case_16, pcp, val)
#define this_cpu_add_4(pcp, val)	\
	_pcp_protect(__percpu_add_case_32, pcp, val)
#define this_cpu_add_8(pcp, val)	\
	_pcp_protect(__percpu_add_case_64, pcp, val)

#define this_cpu_add_return_1(pcp, val)	\
	_pcp_protect_return(__percpu_add_return_case_8, pcp, val)
#define this_cpu_add_return_2(pcp, val)	\
	_pcp_protect_return(__percpu_add_return_case_16, pcp, val)
#define this_cpu_add_return_4(pcp, val)	\
	_pcp_protect_return(__percpu_add_return_case_32, pcp, val)
#define this_cpu_add_return_8(pcp, val)	\
	_pcp_protect_return(__percpu_add_return_case_64, pcp, val)

#define this_cpu_and_1(pcp, val)	\
	_pcp_protect(__percpu_andnot_case_8, pcp, ~val)
#define this_cpu_and_2(pcp, val)	\
	_pcp_protect(__percpu_andnot_case_16, pcp, ~val)
#define this_cpu_and_4(pcp, val)	\
	_pcp_protect(__percpu_andnot_case_32, pcp, ~val)
#define this_cpu_and_8(pcp, val)	\
	_pcp_protect(__percpu_andnot_case_64, pcp, ~val)

#define this_cpu_or_1(pcp, val)		\
	_pcp_protect(__percpu_or_case_8, pcp, val)
#define this_cpu_or_2(pcp, val)		\
	_pcp_protect(__percpu_or_case_16, pcp, val)
#define this_cpu_or_4(pcp, val)		\
	_pcp_protect(__percpu_or_case_32, pcp, val)
#define this_cpu_or_8(pcp, val)		\
	_pcp_protect(__percpu_or_case_64, pcp, val)

#define this_cpu_xchg_1(pcp, val)	\
	_pcp_protect_return(xchg_relaxed, pcp, val)
#define this_cpu_xchg_2(pcp, val)	\
	_pcp_protect_return(xchg_relaxed, pcp, val)
#define this_cpu_xchg_4(pcp, val)	\
	_pcp_protect_return(xchg_relaxed, pcp, val)
#define this_cpu_xchg_8(pcp, val)	\
	_pcp_protect_return(xchg_relaxed, pcp, val)

#define this_cpu_cmpxchg_1(pcp, o, n)	\
	_pcp_protect_return(cmpxchg_relaxed, pcp, o, n)
#define this_cpu_cmpxchg_2(pcp, o, n)	\
	_pcp_protect_return(cmpxchg_relaxed, pcp, o, n)
#define this_cpu_cmpxchg_4(pcp, o, n)	\
	_pcp_protect_return(cmpxchg_relaxed, pcp, o, n)
#define this_cpu_cmpxchg_8(pcp, o, n)	\
	_pcp_protect_return(cmpxchg_relaxed, pcp, o, n)

#ifdef __KVM_NVHE_HYPERVISOR__
extern unsigned long __hyp_per_cpu_offset(unsigned int cpu);
#define __per_cpu_offset
#define per_cpu_offset(cpu)	__hyp_per_cpu_offset((cpu))
#endif

#include <asm-generic/percpu.h>

/* Redefine macros for nVHE hyp under DEBUG_PREEMPT to avoid its dependencies. */
#if defined(__KVM_NVHE_HYPERVISOR__) && defined(CONFIG_DEBUG_PREEMPT)
#undef	this_cpu_ptr
#define	this_cpu_ptr		raw_cpu_ptr
#undef	__this_cpu_read
#define	__this_cpu_read		raw_cpu_read
#undef	__this_cpu_write
#define	__this_cpu_write	raw_cpu_write
#endif

>>>>>>> upstream/android-13
#endif /* __ASM_PERCPU_H */
