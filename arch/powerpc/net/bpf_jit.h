<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * bpf_jit.h: BPF JIT compiler for PPC
 *
 * Copyright 2011 Matt Evans <matt@ozlabs.org>, IBM Corporation
 * 	     2016 Naveen N. Rao <naveen.n.rao@linux.vnet.ibm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
=======
>>>>>>> upstream/android-13
 */
#ifndef _BPF_JIT_H
#define _BPF_JIT_H

#ifndef __ASSEMBLY__

#include <asm/types.h>
<<<<<<< HEAD
=======
#include <asm/ppc-opcode.h>
>>>>>>> upstream/android-13

#ifdef PPC64_ELF_ABI_v1
#define FUNCTION_DESCR_SIZE	24
#else
#define FUNCTION_DESCR_SIZE	0
#endif

<<<<<<< HEAD
/*
 * 16-bit immediate helper macros: HA() is for use with sign-extending instrs
 * (e.g. LD, ADDI).  If the bottom 16 bits is "-ve", add another bit into the
 * top half to negate the effect (i.e. 0xffff + 1 = 0x(1)0000).
 */
#define IMM_H(i)		((uintptr_t)(i)>>16)
#define IMM_HA(i)		(((uintptr_t)(i)>>16) +			      \
					(((uintptr_t)(i) & 0x8000) >> 15))
#define IMM_L(i)		((uintptr_t)(i) & 0xffff)

=======
>>>>>>> upstream/android-13
#define PLANT_INSTR(d, idx, instr)					      \
	do { if (d) { (d)[idx] = instr; } idx++; } while (0)
#define EMIT(instr)		PLANT_INSTR(image, ctx->idx, instr)

<<<<<<< HEAD
#define PPC_NOP()		EMIT(PPC_INST_NOP)
#define PPC_BLR()		EMIT(PPC_INST_BLR)
#define PPC_BLRL()		EMIT(PPC_INST_BLRL)
#define PPC_MTLR(r)		EMIT(PPC_INST_MTLR | ___PPC_RT(r))
#define PPC_BCTR()		EMIT(PPC_INST_BCTR)
#define PPC_MTCTR(r)		EMIT(PPC_INST_MTCTR | ___PPC_RT(r))
#define PPC_ADDI(d, a, i)	EMIT(PPC_INST_ADDI | ___PPC_RT(d) |	      \
				     ___PPC_RA(a) | IMM_L(i))
#define PPC_MR(d, a)		PPC_OR(d, a, a)
#define PPC_LI(r, i)		PPC_ADDI(r, 0, i)
#define PPC_ADDIS(d, a, i)	EMIT(PPC_INST_ADDIS |			      \
				     ___PPC_RT(d) | ___PPC_RA(a) | IMM_L(i))
#define PPC_LIS(r, i)		PPC_ADDIS(r, 0, i)
#define PPC_STD(r, base, i)	EMIT(PPC_INST_STD | ___PPC_RS(r) |	      \
				     ___PPC_RA(base) | ((i) & 0xfffc))
#define PPC_STDX(r, base, b)	EMIT(PPC_INST_STDX | ___PPC_RS(r) |	      \
				     ___PPC_RA(base) | ___PPC_RB(b))
#define PPC_STDU(r, base, i)	EMIT(PPC_INST_STDU | ___PPC_RS(r) |	      \
				     ___PPC_RA(base) | ((i) & 0xfffc))
#define PPC_STW(r, base, i)	EMIT(PPC_INST_STW | ___PPC_RS(r) |	      \
				     ___PPC_RA(base) | IMM_L(i))
#define PPC_STWU(r, base, i)	EMIT(PPC_INST_STWU | ___PPC_RS(r) |	      \
				     ___PPC_RA(base) | IMM_L(i))
#define PPC_STH(r, base, i)	EMIT(PPC_INST_STH | ___PPC_RS(r) |	      \
				     ___PPC_RA(base) | IMM_L(i))
#define PPC_STB(r, base, i)	EMIT(PPC_INST_STB | ___PPC_RS(r) |	      \
				     ___PPC_RA(base) | IMM_L(i))

#define PPC_LBZ(r, base, i)	EMIT(PPC_INST_LBZ | ___PPC_RT(r) |	      \
				     ___PPC_RA(base) | IMM_L(i))
#define PPC_LD(r, base, i)	EMIT(PPC_INST_LD | ___PPC_RT(r) |	      \
				     ___PPC_RA(base) | ((i) & 0xfffc))
#define PPC_LDX(r, base, b)	EMIT(PPC_INST_LDX | ___PPC_RT(r) |	      \
				     ___PPC_RA(base) | ___PPC_RB(b))
#define PPC_LWZ(r, base, i)	EMIT(PPC_INST_LWZ | ___PPC_RT(r) |	      \
				     ___PPC_RA(base) | IMM_L(i))
#define PPC_LHZ(r, base, i)	EMIT(PPC_INST_LHZ | ___PPC_RT(r) |	      \
				     ___PPC_RA(base) | IMM_L(i))
#define PPC_LHBRX(r, base, b)	EMIT(PPC_INST_LHBRX | ___PPC_RT(r) |	      \
				     ___PPC_RA(base) | ___PPC_RB(b))
#define PPC_LDBRX(r, base, b)	EMIT(PPC_INST_LDBRX | ___PPC_RT(r) |	      \
				     ___PPC_RA(base) | ___PPC_RB(b))

#define PPC_BPF_LDARX(t, a, b, eh) EMIT(PPC_INST_LDARX | ___PPC_RT(t) |	      \
					___PPC_RA(a) | ___PPC_RB(b) |	      \
					__PPC_EH(eh))
#define PPC_BPF_LWARX(t, a, b, eh) EMIT(PPC_INST_LWARX | ___PPC_RT(t) |	      \
					___PPC_RA(a) | ___PPC_RB(b) |	      \
					__PPC_EH(eh))
#define PPC_BPF_STWCX(s, a, b)	EMIT(PPC_INST_STWCX | ___PPC_RS(s) |	      \
					___PPC_RA(a) | ___PPC_RB(b))
#define PPC_BPF_STDCX(s, a, b)	EMIT(PPC_INST_STDCX | ___PPC_RS(s) |	      \
					___PPC_RA(a) | ___PPC_RB(b))
#define PPC_CMPWI(a, i)		EMIT(PPC_INST_CMPWI | ___PPC_RA(a) | IMM_L(i))
#define PPC_CMPDI(a, i)		EMIT(PPC_INST_CMPDI | ___PPC_RA(a) | IMM_L(i))
#define PPC_CMPW(a, b)		EMIT(PPC_INST_CMPW | ___PPC_RA(a) |	      \
					___PPC_RB(b))
#define PPC_CMPD(a, b)		EMIT(PPC_INST_CMPD | ___PPC_RA(a) |	      \
					___PPC_RB(b))
#define PPC_CMPLWI(a, i)	EMIT(PPC_INST_CMPLWI | ___PPC_RA(a) | IMM_L(i))
#define PPC_CMPLDI(a, i)	EMIT(PPC_INST_CMPLDI | ___PPC_RA(a) | IMM_L(i))
#define PPC_CMPLW(a, b)		EMIT(PPC_INST_CMPLW | ___PPC_RA(a) |	      \
					___PPC_RB(b))
#define PPC_CMPLD(a, b)		EMIT(PPC_INST_CMPLD | ___PPC_RA(a) |	      \
					___PPC_RB(b))

#define PPC_SUB(d, a, b)	EMIT(PPC_INST_SUB | ___PPC_RT(d) |	      \
				     ___PPC_RB(a) | ___PPC_RA(b))
#define PPC_ADD(d, a, b)	EMIT(PPC_INST_ADD | ___PPC_RT(d) |	      \
				     ___PPC_RA(a) | ___PPC_RB(b))
#define PPC_MULD(d, a, b)	EMIT(PPC_INST_MULLD | ___PPC_RT(d) |	      \
				     ___PPC_RA(a) | ___PPC_RB(b))
#define PPC_MULW(d, a, b)	EMIT(PPC_INST_MULLW | ___PPC_RT(d) |	      \
				     ___PPC_RA(a) | ___PPC_RB(b))
#define PPC_MULHWU(d, a, b)	EMIT(PPC_INST_MULHWU | ___PPC_RT(d) |	      \
				     ___PPC_RA(a) | ___PPC_RB(b))
#define PPC_MULI(d, a, i)	EMIT(PPC_INST_MULLI | ___PPC_RT(d) |	      \
				     ___PPC_RA(a) | IMM_L(i))
#define PPC_DIVWU(d, a, b)	EMIT(PPC_INST_DIVWU | ___PPC_RT(d) |	      \
				     ___PPC_RA(a) | ___PPC_RB(b))
#define PPC_DIVDU(d, a, b)	EMIT(PPC_INST_DIVDU | ___PPC_RT(d) |	      \
				     ___PPC_RA(a) | ___PPC_RB(b))
#define PPC_AND(d, a, b)	EMIT(PPC_INST_AND | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | ___PPC_RB(b))
#define PPC_ANDI(d, a, i)	EMIT(PPC_INST_ANDI | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | IMM_L(i))
#define PPC_AND_DOT(d, a, b)	EMIT(PPC_INST_ANDDOT | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | ___PPC_RB(b))
#define PPC_OR(d, a, b)		EMIT(PPC_INST_OR | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | ___PPC_RB(b))
#define PPC_MR(d, a)		PPC_OR(d, a, a)
#define PPC_ORI(d, a, i)	EMIT(PPC_INST_ORI | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | IMM_L(i))
#define PPC_ORIS(d, a, i)	EMIT(PPC_INST_ORIS | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | IMM_L(i))
#define PPC_XOR(d, a, b)	EMIT(PPC_INST_XOR | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | ___PPC_RB(b))
#define PPC_XORI(d, a, i)	EMIT(PPC_INST_XORI | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | IMM_L(i))
#define PPC_XORIS(d, a, i)	EMIT(PPC_INST_XORIS | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | IMM_L(i))
#define PPC_EXTSW(d, a)		EMIT(PPC_INST_EXTSW | ___PPC_RA(d) |	      \
				     ___PPC_RS(a))
#define PPC_SLW(d, a, s)	EMIT(PPC_INST_SLW | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | ___PPC_RB(s))
#define PPC_SLD(d, a, s)	EMIT(PPC_INST_SLD | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | ___PPC_RB(s))
#define PPC_SRW(d, a, s)	EMIT(PPC_INST_SRW | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | ___PPC_RB(s))
#define PPC_SRD(d, a, s)	EMIT(PPC_INST_SRD | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | ___PPC_RB(s))
#define PPC_SRAD(d, a, s)	EMIT(PPC_INST_SRAD | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | ___PPC_RB(s))
#define PPC_SRADI(d, a, i)	EMIT(PPC_INST_SRADI | ___PPC_RA(d) |	      \
				     ___PPC_RS(a) | __PPC_SH64(i))
#define PPC_RLWINM(d, a, i, mb, me)	EMIT(PPC_INST_RLWINM | ___PPC_RA(d) | \
					___PPC_RS(a) | __PPC_SH(i) |	      \
					__PPC_MB(mb) | __PPC_ME(me))
#define PPC_RLWIMI(d, a, i, mb, me)	EMIT(PPC_INST_RLWIMI | ___PPC_RA(d) | \
					___PPC_RS(a) | __PPC_SH(i) |	      \
					__PPC_MB(mb) | __PPC_ME(me))
#define PPC_RLDICL(d, a, i, mb)		EMIT(PPC_INST_RLDICL | ___PPC_RA(d) | \
					___PPC_RS(a) | __PPC_SH64(i) |	      \
					__PPC_MB64(mb))
#define PPC_RLDICR(d, a, i, me)		EMIT(PPC_INST_RLDICR | ___PPC_RA(d) | \
					___PPC_RS(a) | __PPC_SH64(i) |	      \
					__PPC_ME64(me))

/* slwi = rlwinm Rx, Ry, n, 0, 31-n */
#define PPC_SLWI(d, a, i)	PPC_RLWINM(d, a, i, 0, 31-(i))
/* srwi = rlwinm Rx, Ry, 32-n, n, 31 */
#define PPC_SRWI(d, a, i)	PPC_RLWINM(d, a, 32-(i), i, 31)
/* sldi = rldicr Rx, Ry, n, 63-n */
#define PPC_SLDI(d, a, i)	PPC_RLDICR(d, a, i, 63-(i))
/* sldi = rldicl Rx, Ry, 64-n, n */
#define PPC_SRDI(d, a, i)	PPC_RLDICL(d, a, 64-(i), i)

#define PPC_NEG(d, a)		EMIT(PPC_INST_NEG | ___PPC_RT(d) | ___PPC_RA(a))

/* Long jump; (unconditional 'branch') */
#define PPC_JMP(dest)		EMIT(PPC_INST_BRANCH |			      \
				     (((dest) - (ctx->idx * 4)) & 0x03fffffc))
/* "cond" here covers BO:BI fields. */
#define PPC_BCC_SHORT(cond, dest)	EMIT(PPC_INST_BRANCH_COND |	      \
					     (((cond) & 0x3ff) << 16) |	      \
					     (((dest) - (ctx->idx * 4)) &     \
					      0xfffc))
=======
/* Long jump; (unconditional 'branch') */
#define PPC_JMP(dest)							      \
	do {								      \
		long offset = (long)(dest) - (ctx->idx * 4);		      \
		if (!is_offset_in_branch_range(offset)) {		      \
			pr_err_ratelimited("Branch offset 0x%lx (@%u) out of range\n", offset, ctx->idx);			\
			return -ERANGE;					      \
		}							      \
		EMIT(PPC_INST_BRANCH | (offset & 0x03fffffc));		      \
	} while (0)

/* blr; (unconditional 'branch' with link) to absolute address */
#define PPC_BL_ABS(dest)	EMIT(PPC_INST_BL |			      \
				     (((dest) - (unsigned long)(image + ctx->idx)) & 0x03fffffc))
/* "cond" here covers BO:BI fields. */
#define PPC_BCC_SHORT(cond, dest)					      \
	do {								      \
		long offset = (long)(dest) - (ctx->idx * 4);		      \
		if (!is_offset_in_cond_branch_range(offset)) {		      \
			pr_err_ratelimited("Conditional branch offset 0x%lx (@%u) out of range\n", offset, ctx->idx);		\
			return -ERANGE;					      \
		}							      \
		EMIT(PPC_INST_BRANCH_COND | (((cond) & 0x3ff) << 16) | (offset & 0xfffc));					\
	} while (0)

>>>>>>> upstream/android-13
/* Sign-extended 32-bit immediate load */
#define PPC_LI32(d, i)		do {					      \
		if ((int)(uintptr_t)(i) >= -32768 &&			      \
				(int)(uintptr_t)(i) < 32768)		      \
<<<<<<< HEAD
			PPC_LI(d, i);					      \
		else {							      \
			PPC_LIS(d, IMM_H(i));				      \
			if (IMM_L(i))					      \
				PPC_ORI(d, d, IMM_L(i));		      \
		} } while(0)

=======
			EMIT(PPC_RAW_LI(d, i));				      \
		else {							      \
			EMIT(PPC_RAW_LIS(d, IMM_H(i)));			      \
			if (IMM_L(i))					      \
				EMIT(PPC_RAW_ORI(d, d, IMM_L(i)));	      \
		} } while(0)

#ifdef CONFIG_PPC32
#define PPC_EX32(r, i)		EMIT(PPC_RAW_LI((r), (i) < 0 ? -1 : 0))
#endif

>>>>>>> upstream/android-13
#define PPC_LI64(d, i)		do {					      \
		if ((long)(i) >= -2147483648 &&				      \
				(long)(i) < 2147483648)			      \
			PPC_LI32(d, i);					      \
		else {							      \
			if (!((uintptr_t)(i) & 0xffff800000000000ULL))	      \
<<<<<<< HEAD
				PPC_LI(d, ((uintptr_t)(i) >> 32) & 0xffff);   \
			else {						      \
				PPC_LIS(d, ((uintptr_t)(i) >> 48));	      \
				if ((uintptr_t)(i) & 0x0000ffff00000000ULL)   \
					PPC_ORI(d, d,			      \
					  ((uintptr_t)(i) >> 32) & 0xffff);   \
			}						      \
			PPC_SLDI(d, d, 32);				      \
			if ((uintptr_t)(i) & 0x00000000ffff0000ULL)	      \
				PPC_ORIS(d, d,				      \
					 ((uintptr_t)(i) >> 16) & 0xffff);    \
			if ((uintptr_t)(i) & 0x000000000000ffffULL)	      \
				PPC_ORI(d, d, (uintptr_t)(i) & 0xffff);	      \
=======
				EMIT(PPC_RAW_LI(d, ((uintptr_t)(i) >> 32) &   \
						0xffff));		      \
			else {						      \
				EMIT(PPC_RAW_LIS(d, ((uintptr_t)(i) >> 48))); \
				if ((uintptr_t)(i) & 0x0000ffff00000000ULL)   \
					EMIT(PPC_RAW_ORI(d, d,		      \
					  ((uintptr_t)(i) >> 32) & 0xffff));  \
			}						      \
			EMIT(PPC_RAW_SLDI(d, d, 32));			      \
			if ((uintptr_t)(i) & 0x00000000ffff0000ULL)	      \
				EMIT(PPC_RAW_ORIS(d, d,			      \
					 ((uintptr_t)(i) >> 16) & 0xffff));   \
			if ((uintptr_t)(i) & 0x000000000000ffffULL)	      \
				EMIT(PPC_RAW_ORI(d, d, (uintptr_t)(i) &       \
							0xffff));             \
>>>>>>> upstream/android-13
		} } while (0)

#ifdef CONFIG_PPC64
#define PPC_FUNC_ADDR(d,i) do { PPC_LI64(d, i); } while(0)
#else
#define PPC_FUNC_ADDR(d,i) do { PPC_LI32(d, i); } while(0)
#endif

<<<<<<< HEAD
static inline bool is_nearbranch(int offset)
{
	return (offset < 32768) && (offset >= -32768);
}

=======
>>>>>>> upstream/android-13
/*
 * The fly in the ointment of code size changing from pass to pass is
 * avoided by padding the short branch case with a NOP.	 If code size differs
 * with different branch reaches we will have the issue of code moving from
 * one pass to the next and will need a few passes to converge on a stable
 * state.
 */
#define PPC_BCC(cond, dest)	do {					      \
<<<<<<< HEAD
		if (is_nearbranch((dest) - (ctx->idx * 4))) {		      \
			PPC_BCC_SHORT(cond, dest);			      \
			PPC_NOP();					      \
=======
		if (is_offset_in_cond_branch_range((long)(dest) - (ctx->idx * 4))) {	\
			PPC_BCC_SHORT(cond, dest);			      \
			EMIT(PPC_RAW_NOP());				      \
>>>>>>> upstream/android-13
		} else {						      \
			/* Flip the 'T or F' bit to invert comparison */      \
			PPC_BCC_SHORT(cond ^ COND_CMP_TRUE, (ctx->idx+2)*4);  \
			PPC_JMP(dest);					      \
		} } while(0)

/* To create a branch condition, select a bit of cr0... */
#define CR0_LT		0
#define CR0_GT		1
#define CR0_EQ		2
/* ...and modify BO[3] */
#define COND_CMP_TRUE	0x100
#define COND_CMP_FALSE	0x000
/* Together, they make all required comparisons: */
#define COND_GT		(CR0_GT | COND_CMP_TRUE)
#define COND_GE		(CR0_LT | COND_CMP_FALSE)
#define COND_EQ		(CR0_EQ | COND_CMP_TRUE)
#define COND_NE		(CR0_EQ | COND_CMP_FALSE)
#define COND_LT		(CR0_LT | COND_CMP_TRUE)
#define COND_LE		(CR0_GT | COND_CMP_FALSE)

<<<<<<< HEAD
=======
#define SEEN_FUNC	0x20000000 /* might call external helpers */
#define SEEN_STACK	0x40000000 /* uses BPF stack */
#define SEEN_TAILCALL	0x80000000 /* uses tail calls */

#define SEEN_VREG_MASK	0x1ff80000 /* Volatile registers r3-r12 */
#define SEEN_NVREG_MASK	0x0003ffff /* Non volatile registers r14-r31 */

#ifdef CONFIG_PPC64
extern const int b2p[MAX_BPF_JIT_REG + 2];
#else
extern const int b2p[MAX_BPF_JIT_REG + 1];
#endif

struct codegen_context {
	/*
	 * This is used to track register usage as well
	 * as calls to external helpers.
	 * - register usage is tracked with corresponding
	 *   bits (r3-r31)
	 * - rest of the bits can be used to track other
	 *   things -- for now, we use bits 0 to 2
	 *   encoded in SEEN_* macros above
	 */
	unsigned int seen;
	unsigned int idx;
	unsigned int stack_size;
	int b2p[ARRAY_SIZE(b2p)];
};

static inline void bpf_flush_icache(void *start, void *end)
{
	smp_wmb();	/* smp write barrier */
	flush_icache_range((unsigned long)start, (unsigned long)end);
}

static inline bool bpf_is_seen_register(struct codegen_context *ctx, int i)
{
	return ctx->seen & (1 << (31 - i));
}

static inline void bpf_set_seen_register(struct codegen_context *ctx, int i)
{
	ctx->seen |= 1 << (31 - i);
}

static inline void bpf_clear_seen_register(struct codegen_context *ctx, int i)
{
	ctx->seen &= ~(1 << (31 - i));
}

void bpf_jit_emit_func_call_rel(u32 *image, struct codegen_context *ctx, u64 func);
int bpf_jit_build_body(struct bpf_prog *fp, u32 *image, struct codegen_context *ctx,
		       u32 *addrs, bool extra_pass);
void bpf_jit_build_prologue(u32 *image, struct codegen_context *ctx);
void bpf_jit_build_epilogue(u32 *image, struct codegen_context *ctx);
void bpf_jit_realloc_regs(struct codegen_context *ctx);

>>>>>>> upstream/android-13
#endif

#endif
