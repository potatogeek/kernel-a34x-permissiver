<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Accelerated GHASH implementation with NEON/ARMv8 vmull.p8/64 instructions.
 *
 * Copyright (C) 2015 - 2017 Linaro Ltd. <ard.biesheuvel@linaro.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/linkage.h>
#include <asm/assembler.h>

<<<<<<< HEAD
=======
	.arch		armv8-a
	.fpu		crypto-neon-fp-armv8

>>>>>>> upstream/android-13
	SHASH		.req	q0
	T1		.req	q1
	XL		.req	q2
	XM		.req	q3
	XH		.req	q4
	IN1		.req	q4

	SHASH_L		.req	d0
	SHASH_H		.req	d1
	T1_L		.req	d2
	T1_H		.req	d3
	XL_L		.req	d4
	XL_H		.req	d5
	XM_L		.req	d6
	XM_H		.req	d7
	XH_L		.req	d8

	t0l		.req	d10
	t0h		.req	d11
	t1l		.req	d12
	t1h		.req	d13
	t2l		.req	d14
	t2h		.req	d15
	t3l		.req	d16
	t3h		.req	d17
	t4l		.req	d18
	t4h		.req	d19

	t0q		.req	q5
	t1q		.req	q6
	t2q		.req	q7
	t3q		.req	q8
	t4q		.req	q9
	T2		.req	q9

	s1l		.req	d20
	s1h		.req	d21
	s2l		.req	d22
	s2h		.req	d23
	s3l		.req	d24
	s3h		.req	d25
	s4l		.req	d26
	s4h		.req	d27

	MASK		.req	d28
	SHASH2_p8	.req	d28

	k16		.req	d29
	k32		.req	d30
	k48		.req	d31
	SHASH2_p64	.req	d31

<<<<<<< HEAD
	.text
	.fpu		crypto-neon-fp-armv8
=======
	HH		.req	q10
	HH3		.req	q11
	HH4		.req	q12
	HH34		.req	q13

	HH_L		.req	d20
	HH_H		.req	d21
	HH3_L		.req	d22
	HH3_H		.req	d23
	HH4_L		.req	d24
	HH4_H		.req	d25
	HH34_L		.req	d26
	HH34_H		.req	d27
	SHASH2_H	.req	d29

	XL2		.req	q5
	XM2		.req	q6
	XH2		.req	q7
	T3		.req	q8

	XL2_L		.req	d10
	XL2_H		.req	d11
	XM2_L		.req	d12
	XM2_H		.req	d13
	T3_L		.req	d16
	T3_H		.req	d17

	.text
>>>>>>> upstream/android-13

	.macro		__pmull_p64, rd, rn, rm, b1, b2, b3, b4
	vmull.p64	\rd, \rn, \rm
	.endm

	/*
	 * This implementation of 64x64 -> 128 bit polynomial multiplication
	 * using vmull.p8 instructions (8x8 -> 16) is taken from the paper
	 * "Fast Software Polynomial Multiplication on ARM Processors Using
	 * the NEON Engine" by Danilo Camara, Conrado Gouvea, Julio Lopez and
	 * Ricardo Dahab (https://hal.inria.fr/hal-01506572)
	 *
	 * It has been slightly tweaked for in-order performance, and to allow
	 * 'rq' to overlap with 'ad' or 'bd'.
	 */
	.macro		__pmull_p8, rq, ad, bd, b1=t4l, b2=t3l, b3=t4l, b4=t3l
	vext.8		t0l, \ad, \ad, #1	@ A1
	.ifc		\b1, t4l
	vext.8		t4l, \bd, \bd, #1	@ B1
	.endif
	vmull.p8	t0q, t0l, \bd		@ F = A1*B
	vext.8		t1l, \ad, \ad, #2	@ A2
	vmull.p8	t4q, \ad, \b1		@ E = A*B1
	.ifc		\b2, t3l
	vext.8		t3l, \bd, \bd, #2	@ B2
	.endif
	vmull.p8	t1q, t1l, \bd		@ H = A2*B
	vext.8		t2l, \ad, \ad, #3	@ A3
	vmull.p8	t3q, \ad, \b2		@ G = A*B2
	veor		t0q, t0q, t4q		@ L = E + F
	.ifc		\b3, t4l
	vext.8		t4l, \bd, \bd, #3	@ B3
	.endif
	vmull.p8	t2q, t2l, \bd		@ J = A3*B
	veor		t0l, t0l, t0h		@ t0 = (L) (P0 + P1) << 8
	veor		t1q, t1q, t3q		@ M = G + H
	.ifc		\b4, t3l
	vext.8		t3l, \bd, \bd, #4	@ B4
	.endif
	vmull.p8	t4q, \ad, \b3		@ I = A*B3
	veor		t1l, t1l, t1h		@ t1 = (M) (P2 + P3) << 16
	vmull.p8	t3q, \ad, \b4		@ K = A*B4
	vand		t0h, t0h, k48
	vand		t1h, t1h, k32
	veor		t2q, t2q, t4q		@ N = I + J
	veor		t0l, t0l, t0h
	veor		t1l, t1l, t1h
	veor		t2l, t2l, t2h		@ t2 = (N) (P4 + P5) << 24
	vand		t2h, t2h, k16
	veor		t3l, t3l, t3h		@ t3 = (K) (P6 + P7) << 32
	vmov.i64	t3h, #0
	vext.8		t0q, t0q, t0q, #15
	veor		t2l, t2l, t2h
	vext.8		t1q, t1q, t1q, #14
	vmull.p8	\rq, \ad, \bd		@ D = A*B
	vext.8		t2q, t2q, t2q, #13
	vext.8		t3q, t3q, t3q, #12
	veor		t0q, t0q, t1q
	veor		t2q, t2q, t3q
	veor		\rq, \rq, t0q
	veor		\rq, \rq, t2q
	.endm

	//
	// PMULL (64x64->128) based reduction for CPUs that can do
	// it in a single instruction.
	//
	.macro		__pmull_reduce_p64
	vmull.p64	T1, XL_L, MASK

	veor		XH_L, XH_L, XM_H
	vext.8		T1, T1, T1, #8
	veor		XL_H, XL_H, XM_L
	veor		T1, T1, XL

	vmull.p64	XL, T1_H, MASK
	.endm

	//
	// Alternative reduction for CPUs that lack support for the
	// 64x64->128 PMULL instruction
	//
	.macro		__pmull_reduce_p8
	veor		XL_H, XL_H, XM_L
	veor		XH_L, XH_L, XM_H

	vshl.i64	T1, XL, #57
	vshl.i64	T2, XL, #62
	veor		T1, T1, T2
	vshl.i64	T2, XL, #63
	veor		T1, T1, T2
	veor		XL_H, XL_H, T1_L
	veor		XH_L, XH_L, T1_H

	vshr.u64	T1, XL, #1
	veor		XH, XH, XL
	veor		XL, XL, T1
	vshr.u64	T1, T1, #6
	vshr.u64	XL, XL, #1
	.endm

	.macro		ghash_update, pn
	vld1.64		{XL}, [r1]

	/* do the head block first, if supplied */
	ldr		ip, [sp]
	teq		ip, #0
	beq		0f
	vld1.64		{T1}, [ip]
	teq		r0, #0
<<<<<<< HEAD
	b		1f

0:	vld1.64		{T1}, [r2]!
	subs		r0, r0, #1

1:	/* multiply XL by SHASH in GF(2^128) */
=======
	b		3f

0:	.ifc		\pn, p64
	tst		r0, #3			// skip until #blocks is a
	bne		2f			// round multiple of 4

	vld1.8		{XL2-XM2}, [r2]!
1:	vld1.8		{T3-T2}, [r2]!
	vrev64.8	XL2, XL2
	vrev64.8	XM2, XM2

	subs		r0, r0, #4

	vext.8		T1, XL2, XL2, #8
	veor		XL2_H, XL2_H, XL_L
	veor		XL, XL, T1

	vrev64.8	T3, T3
	vrev64.8	T1, T2

	vmull.p64	XH, HH4_H, XL_H			// a1 * b1
	veor		XL2_H, XL2_H, XL_H
	vmull.p64	XL, HH4_L, XL_L			// a0 * b0
	vmull.p64	XM, HH34_H, XL2_H		// (a1 + a0)(b1 + b0)

	vmull.p64	XH2, HH3_H, XM2_L		// a1 * b1
	veor		XM2_L, XM2_L, XM2_H
	vmull.p64	XL2, HH3_L, XM2_H		// a0 * b0
	vmull.p64	XM2, HH34_L, XM2_L		// (a1 + a0)(b1 + b0)

	veor		XH, XH, XH2
	veor		XL, XL, XL2
	veor		XM, XM, XM2

	vmull.p64	XH2, HH_H, T3_L			// a1 * b1
	veor		T3_L, T3_L, T3_H
	vmull.p64	XL2, HH_L, T3_H			// a0 * b0
	vmull.p64	XM2, SHASH2_H, T3_L		// (a1 + a0)(b1 + b0)

	veor		XH, XH, XH2
	veor		XL, XL, XL2
	veor		XM, XM, XM2

	vmull.p64	XH2, SHASH_H, T1_L		// a1 * b1
	veor		T1_L, T1_L, T1_H
	vmull.p64	XL2, SHASH_L, T1_H		// a0 * b0
	vmull.p64	XM2, SHASH2_p64, T1_L		// (a1 + a0)(b1 + b0)

	veor		XH, XH, XH2
	veor		XL, XL, XL2
	veor		XM, XM, XM2

	beq		4f

	vld1.8		{XL2-XM2}, [r2]!

	veor		T1, XL, XH
	veor		XM, XM, T1

	__pmull_reduce_p64

	veor		T1, T1, XH
	veor		XL, XL, T1

	b		1b
	.endif

2:	vld1.64		{T1}, [r2]!
	subs		r0, r0, #1

3:	/* multiply XL by SHASH in GF(2^128) */
>>>>>>> upstream/android-13
#ifndef CONFIG_CPU_BIG_ENDIAN
	vrev64.8	T1, T1
#endif
	vext.8		IN1, T1, T1, #8
	veor		T1_L, T1_L, XL_H
	veor		XL, XL, IN1

	__pmull_\pn	XH, XL_H, SHASH_H, s1h, s2h, s3h, s4h	@ a1 * b1
	veor		T1, T1, XL
	__pmull_\pn	XL, XL_L, SHASH_L, s1l, s2l, s3l, s4l	@ a0 * b0
	__pmull_\pn	XM, T1_L, SHASH2_\pn			@ (a1+a0)(b1+b0)

<<<<<<< HEAD
	veor		T1, XL, XH
=======
4:	veor		T1, XL, XH
>>>>>>> upstream/android-13
	veor		XM, XM, T1

	__pmull_reduce_\pn

	veor		T1, T1, XH
	veor		XL, XL, T1

	bne		0b

	vst1.64		{XL}, [r1]
	bx		lr
	.endm

	/*
	 * void pmull_ghash_update(int blocks, u64 dg[], const char *src,
	 *			   struct ghash_key const *k, const char *head)
	 */
ENTRY(pmull_ghash_update_p64)
<<<<<<< HEAD
	vld1.64		{SHASH}, [r3]
	veor		SHASH2_p64, SHASH_L, SHASH_H
=======
	vld1.64		{SHASH}, [r3]!
	vld1.64		{HH}, [r3]!
	vld1.64		{HH3-HH4}, [r3]

	veor		SHASH2_p64, SHASH_L, SHASH_H
	veor		SHASH2_H, HH_L, HH_H
	veor		HH34_L, HH3_L, HH3_H
	veor		HH34_H, HH4_L, HH4_H
>>>>>>> upstream/android-13

	vmov.i8		MASK, #0xe1
	vshl.u64	MASK, MASK, #57

	ghash_update	p64
ENDPROC(pmull_ghash_update_p64)

ENTRY(pmull_ghash_update_p8)
	vld1.64		{SHASH}, [r3]
	veor		SHASH2_p8, SHASH_L, SHASH_H

	vext.8		s1l, SHASH_L, SHASH_L, #1
	vext.8		s2l, SHASH_L, SHASH_L, #2
	vext.8		s3l, SHASH_L, SHASH_L, #3
	vext.8		s4l, SHASH_L, SHASH_L, #4
	vext.8		s1h, SHASH_H, SHASH_H, #1
	vext.8		s2h, SHASH_H, SHASH_H, #2
	vext.8		s3h, SHASH_H, SHASH_H, #3
	vext.8		s4h, SHASH_H, SHASH_H, #4

	vmov.i64	k16, #0xffff
	vmov.i64	k32, #0xffffffff
	vmov.i64	k48, #0xffffffffffff

	ghash_update	p8
ENDPROC(pmull_ghash_update_p8)
