//
// Accelerated CRC-T10DIF using arm64 NEON and Crypto Extensions instructions
//
// Copyright (C) 2016 Linaro Ltd <ard.biesheuvel@linaro.org>
<<<<<<< HEAD
=======
// Copyright (C) 2019 Google LLC <ebiggers@google.com>
>>>>>>> upstream/android-13
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

<<<<<<< HEAD
=======
// Derived from the x86 version:
>>>>>>> upstream/android-13
//
// Implement fast CRC-T10DIF computation with SSE and PCLMULQDQ instructions
//
// Copyright (c) 2013, Intel Corporation
//
// Authors:
//     Erdinc Ozturk <erdinc.ozturk@intel.com>
//     Vinodh Gopal <vinodh.gopal@intel.com>
//     James Guilford <james.guilford@intel.com>
//     Tim Chen <tim.c.chen@linux.intel.com>
//
// This software is available to you under a choice of one of two
// licenses.  You may choose to be licensed under the terms of the GNU
// General Public License (GPL) Version 2, available from the file
// COPYING in the main directory of this source tree, or the
// OpenIB.org BSD license below:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
//
// * Neither the name of the Intel Corporation nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
//
// THIS SOFTWARE IS PROVIDED BY INTEL CORPORATION ""AS IS"" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL CORPORATION OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
<<<<<<< HEAD
//       Function API:
//       UINT16 crc_t10dif_pcl(
//               UINT16 init_crc, //initial CRC value, 16 bits
//               const unsigned char *buf, //buffer pointer to calculate CRC on
//               UINT64 len //buffer length in bytes (64-bit data)
//       );
//
=======
>>>>>>> upstream/android-13
//       Reference paper titled "Fast CRC Computation for Generic
//	Polynomials Using PCLMULQDQ Instruction"
//       URL: http://www.intel.com/content/dam/www/public/us/en/documents
//  /white-papers/fast-crc-computation-generic-polynomials-pclmulqdq-paper.pdf
//
<<<<<<< HEAD
//
=======
>>>>>>> upstream/android-13

#include <linux/linkage.h>
#include <asm/assembler.h>

	.text
<<<<<<< HEAD
	.cpu		generic+crypto

	arg1_low32	.req	w19
	arg2		.req	x20
	arg3		.req	x21

	vzr		.req	v13

ENTRY(crc_t10dif_pmull)
	frame_push	3, 128

	mov		arg1_low32, w0
	mov		arg2, x1
	mov		arg3, x2

	movi		vzr.16b, #0		// init zero register

	// adjust the 16-bit initial_crc value, scale it to 32 bits
	lsl		arg1_low32, arg1_low32, #16

	// check if smaller than 256
	cmp		arg3, #256

	// for sizes less than 128, we can't fold 64B at a time...
	b.lt		_less_than_128

	// load the initial crc value
	// crc value does not need to be byte-reflected, but it needs
	// to be moved to the high part of the register.
	// because data will be byte-reflected and will align with
	// initial crc at correct place.
	movi		v10.16b, #0
	mov		v10.s[3], arg1_low32		// initial crc

	// receive the initial 64B data, xor the initial crc value
	ldp		q0, q1, [arg2]
	ldp		q2, q3, [arg2, #0x20]
	ldp		q4, q5, [arg2, #0x40]
	ldp		q6, q7, [arg2, #0x60]
	add		arg2, arg2, #0x80

CPU_LE(	rev64		v0.16b, v0.16b			)
CPU_LE(	rev64		v1.16b, v1.16b			)
CPU_LE(	rev64		v2.16b, v2.16b			)
CPU_LE(	rev64		v3.16b, v3.16b			)
CPU_LE(	rev64		v4.16b, v4.16b			)
CPU_LE(	rev64		v5.16b, v5.16b			)
CPU_LE(	rev64		v6.16b, v6.16b			)
CPU_LE(	rev64		v7.16b, v7.16b			)

CPU_LE(	ext		v0.16b, v0.16b, v0.16b, #8	)
CPU_LE(	ext		v1.16b, v1.16b, v1.16b, #8	)
CPU_LE(	ext		v2.16b, v2.16b, v2.16b, #8	)
CPU_LE(	ext		v3.16b, v3.16b, v3.16b, #8	)
CPU_LE(	ext		v4.16b, v4.16b, v4.16b, #8	)
CPU_LE(	ext		v5.16b, v5.16b, v5.16b, #8	)
CPU_LE(	ext		v6.16b, v6.16b, v6.16b, #8	)
CPU_LE(	ext		v7.16b, v7.16b, v7.16b, #8	)

	// XOR the initial_crc value
	eor		v0.16b, v0.16b, v10.16b

	ldr_l		q10, rk3, x8	// xmm10 has rk3 and rk4
					// type of pmull instruction
					// will determine which constant to use

	//
	// we subtract 256 instead of 128 to save one instruction from the loop
	//
	sub		arg3, arg3, #256

	// at this section of the code, there is 64*x+y (0<=y<64) bytes of
	// buffer. The _fold_64_B_loop will fold 64B at a time
	// until we have 64+y Bytes of buffer


	// fold 64B at a time. This section of the code folds 4 vector
	// registers in parallel
_fold_64_B_loop:

	.macro		fold64, reg1, reg2
	ldp		q11, q12, [arg2], #0x20

	pmull2		v8.1q, \reg1\().2d, v10.2d
	pmull		\reg1\().1q, \reg1\().1d, v10.1d
=======
	.arch		armv8-a+crypto

	init_crc	.req	w0
	buf		.req	x1
	len		.req	x2
	fold_consts_ptr	.req	x3

	fold_consts	.req	v10

	ad		.req	v14

	k00_16		.req	v15
	k32_48		.req	v16

	t3		.req	v17
	t4		.req	v18
	t5		.req	v19
	t6		.req	v20
	t7		.req	v21
	t8		.req	v22
	t9		.req	v23

	perm1		.req	v24
	perm2		.req	v25
	perm3		.req	v26
	perm4		.req	v27

	bd1		.req	v28
	bd2		.req	v29
	bd3		.req	v30
	bd4		.req	v31

	.macro		__pmull_init_p64
	.endm

	.macro		__pmull_pre_p64, bd
	.endm

	.macro		__pmull_init_p8
	// k00_16 := 0x0000000000000000_000000000000ffff
	// k32_48 := 0x00000000ffffffff_0000ffffffffffff
	movi		k32_48.2d, #0xffffffff
	mov		k32_48.h[2], k32_48.h[0]
	ushr		k00_16.2d, k32_48.2d, #32

	// prepare the permutation vectors
	mov_q		x5, 0x080f0e0d0c0b0a09
	movi		perm4.8b, #8
	dup		perm1.2d, x5
	eor		perm1.16b, perm1.16b, perm4.16b
	ushr		perm2.2d, perm1.2d, #8
	ushr		perm3.2d, perm1.2d, #16
	ushr		perm4.2d, perm1.2d, #24
	sli		perm2.2d, perm1.2d, #56
	sli		perm3.2d, perm1.2d, #48
	sli		perm4.2d, perm1.2d, #40
	.endm

	.macro		__pmull_pre_p8, bd
	tbl		bd1.16b, {\bd\().16b}, perm1.16b
	tbl		bd2.16b, {\bd\().16b}, perm2.16b
	tbl		bd3.16b, {\bd\().16b}, perm3.16b
	tbl		bd4.16b, {\bd\().16b}, perm4.16b
	.endm

SYM_FUNC_START_LOCAL(__pmull_p8_core)
.L__pmull_p8_core:
	ext		t4.8b, ad.8b, ad.8b, #1			// A1
	ext		t5.8b, ad.8b, ad.8b, #2			// A2
	ext		t6.8b, ad.8b, ad.8b, #3			// A3

	pmull		t4.8h, t4.8b, fold_consts.8b		// F = A1*B
	pmull		t8.8h, ad.8b, bd1.8b			// E = A*B1
	pmull		t5.8h, t5.8b, fold_consts.8b		// H = A2*B
	pmull		t7.8h, ad.8b, bd2.8b			// G = A*B2
	pmull		t6.8h, t6.8b, fold_consts.8b		// J = A3*B
	pmull		t9.8h, ad.8b, bd3.8b			// I = A*B3
	pmull		t3.8h, ad.8b, bd4.8b			// K = A*B4
	b		0f

.L__pmull_p8_core2:
	tbl		t4.16b, {ad.16b}, perm1.16b		// A1
	tbl		t5.16b, {ad.16b}, perm2.16b		// A2
	tbl		t6.16b, {ad.16b}, perm3.16b		// A3

	pmull2		t4.8h, t4.16b, fold_consts.16b		// F = A1*B
	pmull2		t8.8h, ad.16b, bd1.16b			// E = A*B1
	pmull2		t5.8h, t5.16b, fold_consts.16b		// H = A2*B
	pmull2		t7.8h, ad.16b, bd2.16b			// G = A*B2
	pmull2		t6.8h, t6.16b, fold_consts.16b		// J = A3*B
	pmull2		t9.8h, ad.16b, bd3.16b			// I = A*B3
	pmull2		t3.8h, ad.16b, bd4.16b			// K = A*B4

0:	eor		t4.16b, t4.16b, t8.16b			// L = E + F
	eor		t5.16b, t5.16b, t7.16b			// M = G + H
	eor		t6.16b, t6.16b, t9.16b			// N = I + J

	uzp1		t8.2d, t4.2d, t5.2d
	uzp2		t4.2d, t4.2d, t5.2d
	uzp1		t7.2d, t6.2d, t3.2d
	uzp2		t6.2d, t6.2d, t3.2d

	// t4 = (L) (P0 + P1) << 8
	// t5 = (M) (P2 + P3) << 16
	eor		t8.16b, t8.16b, t4.16b
	and		t4.16b, t4.16b, k32_48.16b

	// t6 = (N) (P4 + P5) << 24
	// t7 = (K) (P6 + P7) << 32
	eor		t7.16b, t7.16b, t6.16b
	and		t6.16b, t6.16b, k00_16.16b

	eor		t8.16b, t8.16b, t4.16b
	eor		t7.16b, t7.16b, t6.16b

	zip2		t5.2d, t8.2d, t4.2d
	zip1		t4.2d, t8.2d, t4.2d
	zip2		t3.2d, t7.2d, t6.2d
	zip1		t6.2d, t7.2d, t6.2d

	ext		t4.16b, t4.16b, t4.16b, #15
	ext		t5.16b, t5.16b, t5.16b, #14
	ext		t6.16b, t6.16b, t6.16b, #13
	ext		t3.16b, t3.16b, t3.16b, #12

	eor		t4.16b, t4.16b, t5.16b
	eor		t6.16b, t6.16b, t3.16b
	ret
SYM_FUNC_END(__pmull_p8_core)

	.macro		__pmull_p8, rq, ad, bd, i
	.ifnc		\bd, fold_consts
	.err
	.endif
	mov		ad.16b, \ad\().16b
	.ifb		\i
	pmull		\rq\().8h, \ad\().8b, \bd\().8b		// D = A*B
	.else
	pmull2		\rq\().8h, \ad\().16b, \bd\().16b	// D = A*B
	.endif

	bl		.L__pmull_p8_core\i

	eor		\rq\().16b, \rq\().16b, t4.16b
	eor		\rq\().16b, \rq\().16b, t6.16b
	.endm

	// Fold reg1, reg2 into the next 32 data bytes, storing the result back
	// into reg1, reg2.
	.macro		fold_32_bytes, p, reg1, reg2
	ldp		q11, q12, [buf], #0x20

	__pmull_\p	v8, \reg1, fold_consts, 2
	__pmull_\p	\reg1, \reg1, fold_consts
>>>>>>> upstream/android-13

CPU_LE(	rev64		v11.16b, v11.16b		)
CPU_LE(	rev64		v12.16b, v12.16b		)

<<<<<<< HEAD
	pmull2		v9.1q, \reg2\().2d, v10.2d
	pmull		\reg2\().1q, \reg2\().1d, v10.1d
=======
	__pmull_\p	v9, \reg2, fold_consts, 2
	__pmull_\p	\reg2, \reg2, fold_consts
>>>>>>> upstream/android-13

CPU_LE(	ext		v11.16b, v11.16b, v11.16b, #8	)
CPU_LE(	ext		v12.16b, v12.16b, v12.16b, #8	)

	eor		\reg1\().16b, \reg1\().16b, v8.16b
	eor		\reg2\().16b, \reg2\().16b, v9.16b
	eor		\reg1\().16b, \reg1\().16b, v11.16b
	eor		\reg2\().16b, \reg2\().16b, v12.16b
	.endm

<<<<<<< HEAD
	fold64		v0, v1
	fold64		v2, v3
	fold64		v4, v5
	fold64		v6, v7

	subs		arg3, arg3, #128

	// check if there is another 64B in the buffer to be able to fold
	b.lt		_fold_64_B_end

	if_will_cond_yield_neon
	stp		q0, q1, [sp, #.Lframe_local_offset]
	stp		q2, q3, [sp, #.Lframe_local_offset + 32]
	stp		q4, q5, [sp, #.Lframe_local_offset + 64]
	stp		q6, q7, [sp, #.Lframe_local_offset + 96]
	do_cond_yield_neon
	ldp		q0, q1, [sp, #.Lframe_local_offset]
	ldp		q2, q3, [sp, #.Lframe_local_offset + 32]
	ldp		q4, q5, [sp, #.Lframe_local_offset + 64]
	ldp		q6, q7, [sp, #.Lframe_local_offset + 96]
	ldr_l		q10, rk3, x8
	movi		vzr.16b, #0		// init zero register
	endif_yield_neon

	b		_fold_64_B_loop

_fold_64_B_end:
	// at this point, the buffer pointer is pointing at the last y Bytes
	// of the buffer the 64B of folded data is in 4 of the vector
	// registers: v0, v1, v2, v3

	// fold the 8 vector registers to 1 vector register with different
	// constants

	ldr_l		q10, rk9, x8

	.macro		fold16, reg, rk
	pmull		v8.1q, \reg\().1d, v10.1d
	pmull2		\reg\().1q, \reg\().2d, v10.2d
	.ifnb		\rk
	ldr_l		q10, \rk, x8
	.endif
	eor		v7.16b, v7.16b, v8.16b
	eor		v7.16b, v7.16b, \reg\().16b
	.endm

	fold16		v0, rk11
	fold16		v1, rk13
	fold16		v2, rk15
	fold16		v3, rk17
	fold16		v4, rk19
	fold16		v5, rk1
	fold16		v6

	// instead of 64, we add 48 to the loop counter to save 1 instruction
	// from the loop instead of a cmp instruction, we use the negative
	// flag with the jl instruction
	adds		arg3, arg3, #(128-16)
	b.lt		_final_reduction_for_128

	// now we have 16+y bytes left to reduce. 16 Bytes is in register v7
	// and the rest is in memory. We can fold 16 bytes at a time if y>=16
	// continue folding 16B at a time

_16B_reduction_loop:
	pmull		v8.1q, v7.1d, v10.1d
	pmull2		v7.1q, v7.2d, v10.2d
	eor		v7.16b, v7.16b, v8.16b

	ldr		q0, [arg2], #16
CPU_LE(	rev64		v0.16b, v0.16b			)
CPU_LE(	ext		v0.16b, v0.16b, v0.16b, #8	)
	eor		v7.16b, v7.16b, v0.16b
	subs		arg3, arg3, #16

	// instead of a cmp instruction, we utilize the flags with the
	// jge instruction equivalent of: cmp arg3, 16-16
	// check if there is any more 16B in the buffer to be able to fold
	b.ge		_16B_reduction_loop

	// now we have 16+z bytes left to reduce, where 0<= z < 16.
	// first, we reduce the data in the xmm7 register

_final_reduction_for_128:
	// check if any more data to fold. If not, compute the CRC of
	// the final 128 bits
	adds		arg3, arg3, #16
	b.eq		_128_done

	// here we are getting data that is less than 16 bytes.
	// since we know that there was data before the pointer, we can
	// offset the input pointer before the actual point, to receive
	// exactly 16 bytes. after that the registers need to be adjusted.
_get_last_two_regs:
	add		arg2, arg2, arg3
	ldr		q1, [arg2, #-16]
CPU_LE(	rev64		v1.16b, v1.16b			)
CPU_LE(	ext		v1.16b, v1.16b, v1.16b, #8	)

	// get rid of the extra data that was loaded before
	// load the shift constant
	adr_l		x4, tbl_shf_table + 16
	sub		x4, x4, arg3
	ld1		{v0.16b}, [x4]

	// shift v2 to the left by arg3 bytes
	tbl		v2.16b, {v7.16b}, v0.16b

	// shift v7 to the right by 16-arg3 bytes
	movi		v9.16b, #0x80
	eor		v0.16b, v0.16b, v9.16b
	tbl		v7.16b, {v7.16b}, v0.16b

	// blend
	sshr		v0.16b, v0.16b, #7	// convert to 8-bit mask
	bsl		v0.16b, v2.16b, v1.16b

	// fold 16 Bytes
	pmull		v8.1q, v7.1d, v10.1d
	pmull2		v7.1q, v7.2d, v10.2d
	eor		v7.16b, v7.16b, v8.16b
	eor		v7.16b, v7.16b, v0.16b

_128_done:
	// compute crc of a 128-bit value
	ldr_l		q10, rk5, x8		// rk5 and rk6 in xmm10

	// 64b fold
	ext		v0.16b, vzr.16b, v7.16b, #8
	mov		v7.d[0], v7.d[1]
	pmull		v7.1q, v7.1d, v10.1d
	eor		v7.16b, v7.16b, v0.16b

	// 32b fold
	ext		v0.16b, v7.16b, vzr.16b, #4
	mov		v7.s[3], vzr.s[0]
	pmull2		v0.1q, v0.2d, v10.2d
	eor		v7.16b, v7.16b, v0.16b

	// barrett reduction
_barrett:
	ldr_l		q10, rk7, x8
	mov		v0.d[0], v7.d[1]

	pmull		v0.1q, v0.1d, v10.1d
	ext		v0.16b, vzr.16b, v0.16b, #12
	pmull2		v0.1q, v0.2d, v10.2d
	ext		v0.16b, vzr.16b, v0.16b, #12
	eor		v7.16b, v7.16b, v0.16b
	mov		w0, v7.s[1]

_cleanup:
	// scale the result back to 16 bits
	lsr		x0, x0, #16
	frame_pop
	ret

_less_than_128:
	cbz		arg3, _cleanup

	movi		v0.16b, #0
	mov		v0.s[3], arg1_low32	// get the initial crc value

	ldr		q7, [arg2], #0x10
CPU_LE(	rev64		v7.16b, v7.16b			)
CPU_LE(	ext		v7.16b, v7.16b, v7.16b, #8	)
	eor		v7.16b, v7.16b, v0.16b	// xor the initial crc value

	cmp		arg3, #16
	b.eq		_128_done		// exactly 16 left
	b.lt		_less_than_16_left

	ldr_l		q10, rk1, x8		// rk1 and rk2 in xmm10

	// update the counter. subtract 32 instead of 16 to save one
	// instruction from the loop
	subs		arg3, arg3, #32
	b.ge		_16B_reduction_loop

	add		arg3, arg3, #16
	b		_get_last_two_regs

_less_than_16_left:
	// shl r9, 4
	adr_l		x0, tbl_shf_table + 16
	sub		x0, x0, arg3
	ld1		{v0.16b}, [x0]
	movi		v9.16b, #0x80
	eor		v0.16b, v0.16b, v9.16b
	tbl		v7.16b, {v7.16b}, v0.16b
	b		_128_done
ENDPROC(crc_t10dif_pmull)

// precomputed constants
// these constants are precomputed from the poly:
// 0x8bb70000 (0x8bb7 scaled to 32 bits)
	.section	".rodata", "a"
	.align		4
// Q = 0x18BB70000
// rk1 = 2^(32*3) mod Q << 32
// rk2 = 2^(32*5) mod Q << 32
// rk3 = 2^(32*15) mod Q << 32
// rk4 = 2^(32*17) mod Q << 32
// rk5 = 2^(32*3) mod Q << 32
// rk6 = 2^(32*2) mod Q << 32
// rk7 = floor(2^64/Q)
// rk8 = Q

rk1:	.octa		0x06df0000000000002d56000000000000
rk3:	.octa		0x7cf50000000000009d9d000000000000
rk5:	.octa		0x13680000000000002d56000000000000
rk7:	.octa		0x000000018bb7000000000001f65a57f8
rk9:	.octa		0xbfd6000000000000ceae000000000000
rk11:	.octa		0x713c0000000000001e16000000000000
rk13:	.octa		0x80a6000000000000f7f9000000000000
rk15:	.octa		0xe658000000000000044c000000000000
rk17:	.octa		0xa497000000000000ad18000000000000
rk19:	.octa		0xe7b50000000000006ee3000000000000

tbl_shf_table:
// use these values for shift constants for the tbl/tbx instruction
// different alignments result in values as shown:
//	DDQ 0x008f8e8d8c8b8a898887868584838281 # shl 15 (16-1) / shr1
//	DDQ 0x01008f8e8d8c8b8a8988878685848382 # shl 14 (16-3) / shr2
//	DDQ 0x0201008f8e8d8c8b8a89888786858483 # shl 13 (16-4) / shr3
//	DDQ 0x030201008f8e8d8c8b8a898887868584 # shl 12 (16-4) / shr4
//	DDQ 0x04030201008f8e8d8c8b8a8988878685 # shl 11 (16-5) / shr5
//	DDQ 0x0504030201008f8e8d8c8b8a89888786 # shl 10 (16-6) / shr6
//	DDQ 0x060504030201008f8e8d8c8b8a898887 # shl 9  (16-7) / shr7
//	DDQ 0x07060504030201008f8e8d8c8b8a8988 # shl 8  (16-8) / shr8
//	DDQ 0x0807060504030201008f8e8d8c8b8a89 # shl 7  (16-9) / shr9
//	DDQ 0x090807060504030201008f8e8d8c8b8a # shl 6  (16-10) / shr10
//	DDQ 0x0a090807060504030201008f8e8d8c8b # shl 5  (16-11) / shr11
//	DDQ 0x0b0a090807060504030201008f8e8d8c # shl 4  (16-12) / shr12
//	DDQ 0x0c0b0a090807060504030201008f8e8d # shl 3  (16-13) / shr13
//	DDQ 0x0d0c0b0a090807060504030201008f8e # shl 2  (16-14) / shr14
//	DDQ 0x0e0d0c0b0a090807060504030201008f # shl 1  (16-15) / shr15

=======
	// Fold src_reg into dst_reg, optionally loading the next fold constants
	.macro		fold_16_bytes, p, src_reg, dst_reg, load_next_consts
	__pmull_\p	v8, \src_reg, fold_consts
	__pmull_\p	\src_reg, \src_reg, fold_consts, 2
	.ifnb		\load_next_consts
	ld1		{fold_consts.2d}, [fold_consts_ptr], #16
	__pmull_pre_\p	fold_consts
	.endif
	eor		\dst_reg\().16b, \dst_reg\().16b, v8.16b
	eor		\dst_reg\().16b, \dst_reg\().16b, \src_reg\().16b
	.endm

	.macro		__pmull_p64, rd, rn, rm, n
	.ifb		\n
	pmull		\rd\().1q, \rn\().1d, \rm\().1d
	.else
	pmull2		\rd\().1q, \rn\().2d, \rm\().2d
	.endif
	.endm

	.macro		crc_t10dif_pmull, p
	__pmull_init_\p

	// For sizes less than 256 bytes, we can't fold 128 bytes at a time.
	cmp		len, #256
	b.lt		.Lless_than_256_bytes_\@

	adr_l		fold_consts_ptr, .Lfold_across_128_bytes_consts

	// Load the first 128 data bytes.  Byte swapping is necessary to make
	// the bit order match the polynomial coefficient order.
	ldp		q0, q1, [buf]
	ldp		q2, q3, [buf, #0x20]
	ldp		q4, q5, [buf, #0x40]
	ldp		q6, q7, [buf, #0x60]
	add		buf, buf, #0x80
CPU_LE(	rev64		v0.16b, v0.16b			)
CPU_LE(	rev64		v1.16b, v1.16b			)
CPU_LE(	rev64		v2.16b, v2.16b			)
CPU_LE(	rev64		v3.16b, v3.16b			)
CPU_LE(	rev64		v4.16b, v4.16b			)
CPU_LE(	rev64		v5.16b, v5.16b			)
CPU_LE(	rev64		v6.16b, v6.16b			)
CPU_LE(	rev64		v7.16b, v7.16b			)
CPU_LE(	ext		v0.16b, v0.16b, v0.16b, #8	)
CPU_LE(	ext		v1.16b, v1.16b, v1.16b, #8	)
CPU_LE(	ext		v2.16b, v2.16b, v2.16b, #8	)
CPU_LE(	ext		v3.16b, v3.16b, v3.16b, #8	)
CPU_LE(	ext		v4.16b, v4.16b, v4.16b, #8	)
CPU_LE(	ext		v5.16b, v5.16b, v5.16b, #8	)
CPU_LE(	ext		v6.16b, v6.16b, v6.16b, #8	)
CPU_LE(	ext		v7.16b, v7.16b, v7.16b, #8	)

	// XOR the first 16 data *bits* with the initial CRC value.
	movi		v8.16b, #0
	mov		v8.h[7], init_crc
	eor		v0.16b, v0.16b, v8.16b

	// Load the constants for folding across 128 bytes.
	ld1		{fold_consts.2d}, [fold_consts_ptr]
	__pmull_pre_\p	fold_consts

	// Subtract 128 for the 128 data bytes just consumed.  Subtract another
	// 128 to simplify the termination condition of the following loop.
	sub		len, len, #256

	// While >= 128 data bytes remain (not counting v0-v7), fold the 128
	// bytes v0-v7 into them, storing the result back into v0-v7.
.Lfold_128_bytes_loop_\@:
	fold_32_bytes	\p, v0, v1
	fold_32_bytes	\p, v2, v3
	fold_32_bytes	\p, v4, v5
	fold_32_bytes	\p, v6, v7

	subs		len, len, #128
	b.ge		.Lfold_128_bytes_loop_\@

	// Now fold the 112 bytes in v0-v6 into the 16 bytes in v7.

	// Fold across 64 bytes.
	add		fold_consts_ptr, fold_consts_ptr, #16
	ld1		{fold_consts.2d}, [fold_consts_ptr], #16
	__pmull_pre_\p	fold_consts
	fold_16_bytes	\p, v0, v4
	fold_16_bytes	\p, v1, v5
	fold_16_bytes	\p, v2, v6
	fold_16_bytes	\p, v3, v7, 1
	// Fold across 32 bytes.
	fold_16_bytes	\p, v4, v6
	fold_16_bytes	\p, v5, v7, 1
	// Fold across 16 bytes.
	fold_16_bytes	\p, v6, v7

	// Add 128 to get the correct number of data bytes remaining in 0...127
	// (not counting v7), following the previous extra subtraction by 128.
	// Then subtract 16 to simplify the termination condition of the
	// following loop.
	adds		len, len, #(128-16)

	// While >= 16 data bytes remain (not counting v7), fold the 16 bytes v7
	// into them, storing the result back into v7.
	b.lt		.Lfold_16_bytes_loop_done_\@
.Lfold_16_bytes_loop_\@:
	__pmull_\p	v8, v7, fold_consts
	__pmull_\p	v7, v7, fold_consts, 2
	eor		v7.16b, v7.16b, v8.16b
	ldr		q0, [buf], #16
CPU_LE(	rev64		v0.16b, v0.16b			)
CPU_LE(	ext		v0.16b, v0.16b, v0.16b, #8	)
	eor		v7.16b, v7.16b, v0.16b
	subs		len, len, #16
	b.ge		.Lfold_16_bytes_loop_\@

.Lfold_16_bytes_loop_done_\@:
	// Add 16 to get the correct number of data bytes remaining in 0...15
	// (not counting v7), following the previous extra subtraction by 16.
	adds		len, len, #16
	b.eq		.Lreduce_final_16_bytes_\@

.Lhandle_partial_segment_\@:
	// Reduce the last '16 + len' bytes where 1 <= len <= 15 and the first
	// 16 bytes are in v7 and the rest are the remaining data in 'buf'.  To
	// do this without needing a fold constant for each possible 'len',
	// redivide the bytes into a first chunk of 'len' bytes and a second
	// chunk of 16 bytes, then fold the first chunk into the second.

	// v0 = last 16 original data bytes
	add		buf, buf, len
	ldr		q0, [buf, #-16]
CPU_LE(	rev64		v0.16b, v0.16b			)
CPU_LE(	ext		v0.16b, v0.16b, v0.16b, #8	)

	// v1 = high order part of second chunk: v7 left-shifted by 'len' bytes.
	adr_l		x4, .Lbyteshift_table + 16
	sub		x4, x4, len
	ld1		{v2.16b}, [x4]
	tbl		v1.16b, {v7.16b}, v2.16b

	// v3 = first chunk: v7 right-shifted by '16-len' bytes.
	movi		v3.16b, #0x80
	eor		v2.16b, v2.16b, v3.16b
	tbl		v3.16b, {v7.16b}, v2.16b

	// Convert to 8-bit masks: 'len' 0x00 bytes, then '16-len' 0xff bytes.
	sshr		v2.16b, v2.16b, #7

	// v2 = second chunk: 'len' bytes from v0 (low-order bytes),
	// then '16-len' bytes from v1 (high-order bytes).
	bsl		v2.16b, v1.16b, v0.16b

	// Fold the first chunk into the second chunk, storing the result in v7.
	__pmull_\p	v0, v3, fold_consts
	__pmull_\p	v7, v3, fold_consts, 2
	eor		v7.16b, v7.16b, v0.16b
	eor		v7.16b, v7.16b, v2.16b

.Lreduce_final_16_bytes_\@:
	// Reduce the 128-bit value M(x), stored in v7, to the final 16-bit CRC.

	movi		v2.16b, #0		// init zero register

	// Load 'x^48 * (x^48 mod G(x))' and 'x^48 * (x^80 mod G(x))'.
	ld1		{fold_consts.2d}, [fold_consts_ptr], #16
	__pmull_pre_\p	fold_consts

	// Fold the high 64 bits into the low 64 bits, while also multiplying by
	// x^64.  This produces a 128-bit value congruent to x^64 * M(x) and
	// whose low 48 bits are 0.
	ext		v0.16b, v2.16b, v7.16b, #8
	__pmull_\p	v7, v7, fold_consts, 2	// high bits * x^48 * (x^80 mod G(x))
	eor		v0.16b, v0.16b, v7.16b	// + low bits * x^64

	// Fold the high 32 bits into the low 96 bits.  This produces a 96-bit
	// value congruent to x^64 * M(x) and whose low 48 bits are 0.
	ext		v1.16b, v0.16b, v2.16b, #12	// extract high 32 bits
	mov		v0.s[3], v2.s[0]	// zero high 32 bits
	__pmull_\p	v1, v1, fold_consts	// high 32 bits * x^48 * (x^48 mod G(x))
	eor		v0.16b, v0.16b, v1.16b	// + low bits

	// Load G(x) and floor(x^48 / G(x)).
	ld1		{fold_consts.2d}, [fold_consts_ptr]
	__pmull_pre_\p	fold_consts

	// Use Barrett reduction to compute the final CRC value.
	__pmull_\p	v1, v0, fold_consts, 2	// high 32 bits * floor(x^48 / G(x))
	ushr		v1.2d, v1.2d, #32	// /= x^32
	__pmull_\p	v1, v1, fold_consts	// *= G(x)
	ushr		v0.2d, v0.2d, #48
	eor		v0.16b, v0.16b, v1.16b	// + low 16 nonzero bits
	// Final CRC value (x^16 * M(x)) mod G(x) is in low 16 bits of v0.

	umov		w0, v0.h[0]
	.ifc		\p, p8
	ldp		x29, x30, [sp], #16
	.endif
	ret

.Lless_than_256_bytes_\@:
	// Checksumming a buffer of length 16...255 bytes

	adr_l		fold_consts_ptr, .Lfold_across_16_bytes_consts

	// Load the first 16 data bytes.
	ldr		q7, [buf], #0x10
CPU_LE(	rev64		v7.16b, v7.16b			)
CPU_LE(	ext		v7.16b, v7.16b, v7.16b, #8	)

	// XOR the first 16 data *bits* with the initial CRC value.
	movi		v0.16b, #0
	mov		v0.h[7], init_crc
	eor		v7.16b, v7.16b, v0.16b

	// Load the fold-across-16-bytes constants.
	ld1		{fold_consts.2d}, [fold_consts_ptr], #16
	__pmull_pre_\p	fold_consts

	cmp		len, #16
	b.eq		.Lreduce_final_16_bytes_\@	// len == 16
	subs		len, len, #32
	b.ge		.Lfold_16_bytes_loop_\@		// 32 <= len <= 255
	add		len, len, #16
	b		.Lhandle_partial_segment_\@	// 17 <= len <= 31
	.endm

//
// u16 crc_t10dif_pmull_p8(u16 init_crc, const u8 *buf, size_t len);
//
// Assumes len >= 16.
//
SYM_FUNC_START(crc_t10dif_pmull_p8)
	stp		x29, x30, [sp, #-16]!
	mov		x29, sp
	crc_t10dif_pmull p8
SYM_FUNC_END(crc_t10dif_pmull_p8)

	.align		5
//
// u16 crc_t10dif_pmull_p64(u16 init_crc, const u8 *buf, size_t len);
//
// Assumes len >= 16.
//
SYM_FUNC_START(crc_t10dif_pmull_p64)
	crc_t10dif_pmull	p64
SYM_FUNC_END(crc_t10dif_pmull_p64)

	.section	".rodata", "a"
	.align		4

// Fold constants precomputed from the polynomial 0x18bb7
// G(x) = x^16 + x^15 + x^11 + x^9 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + x^0
.Lfold_across_128_bytes_consts:
	.quad		0x0000000000006123	// x^(8*128)	mod G(x)
	.quad		0x0000000000002295	// x^(8*128+64)	mod G(x)
// .Lfold_across_64_bytes_consts:
	.quad		0x0000000000001069	// x^(4*128)	mod G(x)
	.quad		0x000000000000dd31	// x^(4*128+64)	mod G(x)
// .Lfold_across_32_bytes_consts:
	.quad		0x000000000000857d	// x^(2*128)	mod G(x)
	.quad		0x0000000000007acc	// x^(2*128+64)	mod G(x)
.Lfold_across_16_bytes_consts:
	.quad		0x000000000000a010	// x^(1*128)	mod G(x)
	.quad		0x0000000000001faa	// x^(1*128+64)	mod G(x)
// .Lfinal_fold_consts:
	.quad		0x1368000000000000	// x^48 * (x^48 mod G(x))
	.quad		0x2d56000000000000	// x^48 * (x^80 mod G(x))
// .Lbarrett_reduction_consts:
	.quad		0x0000000000018bb7	// G(x)
	.quad		0x00000001f65a57f8	// floor(x^48 / G(x))

// For 1 <= len <= 15, the 16-byte vector beginning at &byteshift_table[16 -
// len] is the index vector to shift left by 'len' bytes, and is also {0x80,
// ..., 0x80} XOR the index vector to shift right by '16 - len' bytes.
.Lbyteshift_table:
>>>>>>> upstream/android-13
	.byte		 0x0, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87
	.byte		0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f
	.byte		 0x0,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7
	.byte		 0x8,  0x9,  0xa,  0xb,  0xc,  0xd,  0xe , 0x0
