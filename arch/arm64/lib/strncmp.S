<<<<<<< HEAD
/*
 * Copyright (C) 2013 ARM Ltd.
 * Copyright (C) 2013 Linaro.
 *
 * This code is based on glibc cortex strings work originally authored by Linaro
 * and re-licensed under GPLv2 for the Linux kernel. The original code can
 * be found @
 *
 * http://bazaar.launchpad.net/~linaro-toolchain-dev/cortex-strings/trunk/
 * files/head:/src/aarch64/
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
 * Copyright (c) 2013-2021, Arm Limited.
 *
 * Adapted from the original at:
 * https://github.com/ARM-software/optimized-routines/blob/e823e3abf5f89ecb/string/aarch64/strncmp.S
>>>>>>> upstream/android-13
 */

#include <linux/linkage.h>
#include <asm/assembler.h>

<<<<<<< HEAD
/*
 * compare two strings
 *
 * Parameters:
 *  x0 - const string 1 pointer
 *  x1 - const string 2 pointer
 *  x2 - the maximal length to be compared
 * Returns:
 *  x0 - an integer less than, equal to, or greater than zero if s1 is found,
 *     respectively, to be less than, to match, or be greater than s2.
 */

=======
/* Assumptions:
 *
 * ARMv8-a, AArch64
 */

#define L(label) .L ## label

>>>>>>> upstream/android-13
#define REP8_01 0x0101010101010101
#define REP8_7f 0x7f7f7f7f7f7f7f7f
#define REP8_80 0x8080808080808080

/* Parameters and result.  */
<<<<<<< HEAD
src1		.req	x0
src2		.req	x1
limit		.req	x2
result		.req	x0

/* Internal variables.  */
data1		.req	x3
data1w		.req	w3
data2		.req	x4
data2w		.req	w4
has_nul		.req	x5
diff		.req	x6
syndrome	.req	x7
tmp1		.req	x8
tmp2		.req	x9
tmp3		.req	x10
zeroones	.req	x11
pos		.req	x12
limit_wd	.req	x13
mask		.req	x14
endloop		.req	x15

WEAK(strncmp)
	cbz	limit, .Lret0
	eor	tmp1, src1, src2
	mov	zeroones, #REP8_01
	tst	tmp1, #7
	b.ne	.Lmisaligned8
	ands	tmp1, src1, #7
	b.ne	.Lmutual_align
	/* Calculate the number of full and partial words -1.  */
	/*
	* when limit is mulitply of 8, if not sub 1,
	* the judgement of last dword will wrong.
	*/
	sub	limit_wd, limit, #1 /* limit != 0, so no underflow.  */
	lsr	limit_wd, limit_wd, #3  /* Convert to Dwords.  */

	/*
	* NUL detection works on the principle that (X - 1) & (~X) & 0x80
	* (=> (X - 1) & ~(X | 0x7f)) is non-zero iff a byte is zero, and
	* can be done in parallel across the entire word.
	*/
.Lloop_aligned:
	ldr	data1, [src1], #8
	ldr	data2, [src2], #8
.Lstart_realigned:
	subs	limit_wd, limit_wd, #1
	sub	tmp1, data1, zeroones
	orr	tmp2, data1, #REP8_7f
	eor	diff, data1, data2  /* Non-zero if differences found.  */
	csinv	endloop, diff, xzr, pl  /* Last Dword or differences.*/
	bics	has_nul, tmp1, tmp2 /* Non-zero if NUL terminator.  */
	ccmp	endloop, #0, #0, eq
	b.eq	.Lloop_aligned

	/*Not reached the limit, must have found the end or a diff.  */
	tbz	limit_wd, #63, .Lnot_limit

	/* Limit % 8 == 0 => all bytes significant.  */
	ands	limit, limit, #7
	b.eq	.Lnot_limit

	lsl	limit, limit, #3    /* Bits -> bytes.  */
	mov	mask, #~0
CPU_BE( lsr	mask, mask, limit )
CPU_LE( lsl	mask, mask, limit )
=======
#define src1		x0
#define src2		x1
#define limit		x2
#define result		x0

/* Internal variables.  */
#define data1		x3
#define data1w		w3
#define data2		x4
#define data2w		w4
#define has_nul		x5
#define diff		x6
#define syndrome	x7
#define tmp1		x8
#define tmp2		x9
#define tmp3		x10
#define zeroones	x11
#define pos		x12
#define limit_wd	x13
#define mask		x14
#define endloop		x15
#define count		mask

SYM_FUNC_START_WEAK_PI(strncmp)
	cbz	limit, L(ret0)
	eor	tmp1, src1, src2
	mov	zeroones, #REP8_01
	tst	tmp1, #7
	and	count, src1, #7
	b.ne	L(misaligned8)
	cbnz	count, L(mutual_align)
	/* Calculate the number of full and partial words -1.  */
	sub	limit_wd, limit, #1	/* limit != 0, so no underflow.  */
	lsr	limit_wd, limit_wd, #3	/* Convert to Dwords.  */

	/* NUL detection works on the principle that (X - 1) & (~X) & 0x80
	   (=> (X - 1) & ~(X | 0x7f)) is non-zero iff a byte is zero, and
	   can be done in parallel across the entire word.  */
	.p2align 4
L(loop_aligned):
	ldr	data1, [src1], #8
	ldr	data2, [src2], #8
L(start_realigned):
	subs	limit_wd, limit_wd, #1
	sub	tmp1, data1, zeroones
	orr	tmp2, data1, #REP8_7f
	eor	diff, data1, data2	/* Non-zero if differences found.  */
	csinv	endloop, diff, xzr, pl	/* Last Dword or differences.  */
	bics	has_nul, tmp1, tmp2	/* Non-zero if NUL terminator.  */
	ccmp	endloop, #0, #0, eq
	b.eq	L(loop_aligned)
	/* End of main loop */

	/* Not reached the limit, must have found the end or a diff.  */
	tbz	limit_wd, #63, L(not_limit)

	/* Limit % 8 == 0 => all bytes significant.  */
	ands	limit, limit, #7
	b.eq	L(not_limit)

	lsl	limit, limit, #3	/* Bits -> bytes.  */
	mov	mask, #~0
#ifdef __AARCH64EB__
	lsr	mask, mask, limit
#else
	lsl	mask, mask, limit
#endif
>>>>>>> upstream/android-13
	bic	data1, data1, mask
	bic	data2, data2, mask

	/* Make sure that the NUL byte is marked in the syndrome.  */
	orr	has_nul, has_nul, mask

<<<<<<< HEAD
.Lnot_limit:
	orr	syndrome, diff, has_nul
	b	.Lcal_cmpresult

.Lmutual_align:
	/*
	* Sources are mutually aligned, but are not currently at an
	* alignment boundary.  Round down the addresses and then mask off
	* the bytes that precede the start point.
	* We also need to adjust the limit calculations, but without
	* overflowing if the limit is near ULONG_MAX.
	*/
	bic	src1, src1, #7
	bic	src2, src2, #7
	ldr	data1, [src1], #8
	neg	tmp3, tmp1, lsl #3  /* 64 - bits(bytes beyond align). */
	ldr	data2, [src2], #8
	mov	tmp2, #~0
	sub	limit_wd, limit, #1 /* limit != 0, so no underflow.  */
	/* Big-endian.  Early bytes are at MSB.  */
CPU_BE( lsl	tmp2, tmp2, tmp3 )	/* Shift (tmp1 & 63).  */
	/* Little-endian.  Early bytes are at LSB.  */
CPU_LE( lsr	tmp2, tmp2, tmp3 )	/* Shift (tmp1 & 63).  */

	and	tmp3, limit_wd, #7
	lsr	limit_wd, limit_wd, #3
	/* Adjust the limit. Only low 3 bits used, so overflow irrelevant.*/
	add	limit, limit, tmp1
	add	tmp3, tmp3, tmp1
	orr	data1, data1, tmp2
	orr	data2, data2, tmp2
	add	limit_wd, limit_wd, tmp3, lsr #3
	b	.Lstart_realigned

/*when src1 offset is not equal to src2 offset...*/
.Lmisaligned8:
	cmp	limit, #8
	b.lo	.Ltiny8proc /*limit < 8... */
	/*
	* Get the align offset length to compare per byte first.
	* After this process, one string's address will be aligned.*/
	and	tmp1, src1, #7
	neg	tmp1, tmp1
	add	tmp1, tmp1, #8
	and	tmp2, src2, #7
	neg	tmp2, tmp2
	add	tmp2, tmp2, #8
	subs	tmp3, tmp1, tmp2
	csel	pos, tmp1, tmp2, hi /*Choose the maximum. */
	/*
	* Here, limit is not less than 8, so directly run .Ltinycmp
	* without checking the limit.*/
	sub	limit, limit, pos
.Ltinycmp:
	ldrb	data1w, [src1], #1
	ldrb	data2w, [src2], #1
	subs	pos, pos, #1
	ccmp	data1w, #1, #0, ne  /* NZCV = 0b0000.  */
	ccmp	data1w, data2w, #0, cs  /* NZCV = 0b0000.  */
	b.eq	.Ltinycmp
	cbnz	pos, 1f /*find the null or unequal...*/
	cmp	data1w, #1
	ccmp	data1w, data2w, #0, cs
	b.eq	.Lstart_align /*the last bytes are equal....*/
1:
	sub	result, data1, data2
	ret

.Lstart_align:
	lsr	limit_wd, limit, #3
	cbz	limit_wd, .Lremain8
	/*process more leading bytes to make str1 aligned...*/
	ands	xzr, src1, #7
	b.eq	.Lrecal_offset
	add	src1, src1, tmp3	/*tmp3 is positive in this branch.*/
	add	src2, src2, tmp3
	ldr	data1, [src1], #8
	ldr	data2, [src2], #8

	sub	limit, limit, tmp3
	lsr	limit_wd, limit, #3
	subs	limit_wd, limit_wd, #1

	sub	tmp1, data1, zeroones
	orr	tmp2, data1, #REP8_7f
	eor	diff, data1, data2  /* Non-zero if differences found.  */
	csinv	endloop, diff, xzr, ne/*if limit_wd is 0,will finish the cmp*/
	bics	has_nul, tmp1, tmp2
	ccmp	endloop, #0, #0, eq /*has_null is ZERO: no null byte*/
	b.ne	.Lunequal_proc
	/*How far is the current str2 from the alignment boundary...*/
	and	tmp3, tmp3, #7
.Lrecal_offset:
	neg	pos, tmp3
.Lloopcmp_proc:
	/*
	* Divide the eight bytes into two parts. First,backwards the src2
	* to an alignment boundary,load eight bytes from the SRC2 alignment
	* boundary,then compare with the relative bytes from SRC1.
	* If all 8 bytes are equal,then start the second part's comparison.
	* Otherwise finish the comparison.
	* This special handle can garantee all the accesses are in the
	* thread/task space in avoid to overrange access.
	*/
	ldr	data1, [src1,pos]
	ldr	data2, [src2,pos]
	sub	tmp1, data1, zeroones
	orr	tmp2, data1, #REP8_7f
	bics	has_nul, tmp1, tmp2 /* Non-zero if NUL terminator.  */
	eor	diff, data1, data2  /* Non-zero if differences found.  */
	csinv	endloop, diff, xzr, eq
	cbnz	endloop, .Lunequal_proc

	/*The second part process*/
	ldr	data1, [src1], #8
	ldr	data2, [src2], #8
	subs	limit_wd, limit_wd, #1
	sub	tmp1, data1, zeroones
	orr	tmp2, data1, #REP8_7f
	eor	diff, data1, data2  /* Non-zero if differences found.  */
	csinv	endloop, diff, xzr, ne/*if limit_wd is 0,will finish the cmp*/
	bics	has_nul, tmp1, tmp2
	ccmp	endloop, #0, #0, eq /*has_null is ZERO: no null byte*/
	b.eq	.Lloopcmp_proc

.Lunequal_proc:
	orr	syndrome, diff, has_nul
	cbz	syndrome, .Lremain8
.Lcal_cmpresult:
	/*
	* reversed the byte-order as big-endian,then CLZ can find the most
	* significant zero bits.
	*/
CPU_LE( rev	syndrome, syndrome )
CPU_LE( rev	data1, data1 )
CPU_LE( rev	data2, data2 )
	/*
	* For big-endian we cannot use the trick with the syndrome value
	* as carry-propagation can corrupt the upper bits if the trailing
	* bytes in the string contain 0x01.
	* However, if there is no NUL byte in the dword, we can generate
	* the result directly.  We can't just subtract the bytes as the
	* MSB might be significant.
	*/
CPU_BE( cbnz	has_nul, 1f )
CPU_BE( cmp	data1, data2 )
CPU_BE( cset	result, ne )
CPU_BE( cneg	result, result, lo )
CPU_BE( ret )
CPU_BE( 1: )
	/* Re-compute the NUL-byte detection, using a byte-reversed value.*/
CPU_BE( rev	tmp3, data1 )
CPU_BE( sub	tmp1, tmp3, zeroones )
CPU_BE( orr	tmp2, tmp3, #REP8_7f )
CPU_BE( bic	has_nul, tmp1, tmp2 )
CPU_BE( rev	has_nul, has_nul )
CPU_BE( orr	syndrome, diff, has_nul )
	/*
	* The MS-non-zero bit of the syndrome marks either the first bit
	* that is different, or the top bit of the first zero byte.
	* Shifting left now will bring the critical information into the
	* top bits.
	*/
	clz	pos, syndrome
	lsl	data1, data1, pos
	lsl	data2, data2, pos
	/*
	* But we need to zero-extend (char is unsigned) the value and then
	* perform a signed 32-bit subtraction.
	*/
	lsr	data1, data1, #56
	sub	result, data1, data2, lsr #56
	ret

.Lremain8:
	/* Limit % 8 == 0 => all bytes significant.  */
	ands	limit, limit, #7
	b.eq	.Lret0
.Ltiny8proc:
	ldrb	data1w, [src1], #1
	ldrb	data2w, [src2], #1
	subs	limit, limit, #1

	ccmp	data1w, #1, #0, ne  /* NZCV = 0b0000.  */
	ccmp	data1w, data2w, #0, cs  /* NZCV = 0b0000.  */
	b.eq	.Ltiny8proc
	sub	result, data1, data2
	ret

.Lret0:
	mov	result, #0
	ret
ENDPIPROC(strncmp)
=======
L(not_limit):
	orr	syndrome, diff, has_nul

#ifndef	__AARCH64EB__
	rev	syndrome, syndrome
	rev	data1, data1
	/* The MS-non-zero bit of the syndrome marks either the first bit
	   that is different, or the top bit of the first zero byte.
	   Shifting left now will bring the critical information into the
	   top bits.  */
	clz	pos, syndrome
	rev	data2, data2
	lsl	data1, data1, pos
	lsl	data2, data2, pos
	/* But we need to zero-extend (char is unsigned) the value and then
	   perform a signed 32-bit subtraction.  */
	lsr	data1, data1, #56
	sub	result, data1, data2, lsr #56
	ret
#else
	/* For big-endian we cannot use the trick with the syndrome value
	   as carry-propagation can corrupt the upper bits if the trailing
	   bytes in the string contain 0x01.  */
	/* However, if there is no NUL byte in the dword, we can generate
	   the result directly.  We can't just subtract the bytes as the
	   MSB might be significant.  */
	cbnz	has_nul, 1f
	cmp	data1, data2
	cset	result, ne
	cneg	result, result, lo
	ret
1:
	/* Re-compute the NUL-byte detection, using a byte-reversed value.  */
	rev	tmp3, data1
	sub	tmp1, tmp3, zeroones
	orr	tmp2, tmp3, #REP8_7f
	bic	has_nul, tmp1, tmp2
	rev	has_nul, has_nul
	orr	syndrome, diff, has_nul
	clz	pos, syndrome
	/* The MS-non-zero bit of the syndrome marks either the first bit
	   that is different, or the top bit of the first zero byte.
	   Shifting left now will bring the critical information into the
	   top bits.  */
	lsl	data1, data1, pos
	lsl	data2, data2, pos
	/* But we need to zero-extend (char is unsigned) the value and then
	   perform a signed 32-bit subtraction.  */
	lsr	data1, data1, #56
	sub	result, data1, data2, lsr #56
	ret
#endif

L(mutual_align):
	/* Sources are mutually aligned, but are not currently at an
	   alignment boundary.  Round down the addresses and then mask off
	   the bytes that precede the start point.
	   We also need to adjust the limit calculations, but without
	   overflowing if the limit is near ULONG_MAX.  */
	bic	src1, src1, #7
	bic	src2, src2, #7
	ldr	data1, [src1], #8
	neg	tmp3, count, lsl #3	/* 64 - bits(bytes beyond align). */
	ldr	data2, [src2], #8
	mov	tmp2, #~0
	sub	limit_wd, limit, #1	/* limit != 0, so no underflow.  */
#ifdef __AARCH64EB__
	/* Big-endian.  Early bytes are at MSB.  */
	lsl	tmp2, tmp2, tmp3	/* Shift (count & 63).  */
#else
	/* Little-endian.  Early bytes are at LSB.  */
	lsr	tmp2, tmp2, tmp3	/* Shift (count & 63).  */
#endif
	and	tmp3, limit_wd, #7
	lsr	limit_wd, limit_wd, #3
	/* Adjust the limit. Only low 3 bits used, so overflow irrelevant.  */
	add	limit, limit, count
	add	tmp3, tmp3, count
	orr	data1, data1, tmp2
	orr	data2, data2, tmp2
	add	limit_wd, limit_wd, tmp3, lsr #3
	b	L(start_realigned)

	.p2align 4
	/* Don't bother with dwords for up to 16 bytes.  */
L(misaligned8):
	cmp	limit, #16
	b.hs	L(try_misaligned_words)

L(byte_loop):
	/* Perhaps we can do better than this.  */
	ldrb	data1w, [src1], #1
	ldrb	data2w, [src2], #1
	subs	limit, limit, #1
	ccmp	data1w, #1, #0, hi	/* NZCV = 0b0000.  */
	ccmp	data1w, data2w, #0, cs	/* NZCV = 0b0000.  */
	b.eq	L(byte_loop)
L(done):
	sub	result, data1, data2
	ret
	/* Align the SRC1 to a dword by doing a bytewise compare and then do
	   the dword loop.  */
L(try_misaligned_words):
	lsr	limit_wd, limit, #3
	cbz	count, L(do_misaligned)

	neg	count, count
	and	count, count, #7
	sub	limit, limit, count
	lsr	limit_wd, limit, #3

L(page_end_loop):
	ldrb	data1w, [src1], #1
	ldrb	data2w, [src2], #1
	cmp	data1w, #1
	ccmp	data1w, data2w, #0, cs	/* NZCV = 0b0000.  */
	b.ne	L(done)
	subs	count, count, #1
	b.hi	L(page_end_loop)

L(do_misaligned):
	/* Prepare ourselves for the next page crossing.  Unlike the aligned
	   loop, we fetch 1 less dword because we risk crossing bounds on
	   SRC2.  */
	mov	count, #8
	subs	limit_wd, limit_wd, #1
	b.lo	L(done_loop)
L(loop_misaligned):
	and	tmp2, src2, #0xff8
	eor	tmp2, tmp2, #0xff8
	cbz	tmp2, L(page_end_loop)

	ldr	data1, [src1], #8
	ldr	data2, [src2], #8
	sub	tmp1, data1, zeroones
	orr	tmp2, data1, #REP8_7f
	eor	diff, data1, data2	/* Non-zero if differences found.  */
	bics	has_nul, tmp1, tmp2	/* Non-zero if NUL terminator.  */
	ccmp	diff, #0, #0, eq
	b.ne	L(not_limit)
	subs	limit_wd, limit_wd, #1
	b.pl	L(loop_misaligned)

L(done_loop):
	/* We found a difference or a NULL before the limit was reached.  */
	and	limit, limit, #7
	cbz	limit, L(not_limit)
	/* Read the last word.  */
	sub	src1, src1, 8
	sub	src2, src2, 8
	ldr	data1, [src1, limit]
	ldr	data2, [src2, limit]
	sub	tmp1, data1, zeroones
	orr	tmp2, data1, #REP8_7f
	eor	diff, data1, data2	/* Non-zero if differences found.  */
	bics	has_nul, tmp1, tmp2	/* Non-zero if NUL terminator.  */
	ccmp	diff, #0, #0, eq
	b.ne	L(not_limit)

L(ret0):
	mov	result, #0
	ret

SYM_FUNC_END_PI(strncmp)
EXPORT_SYMBOL_NOHWKASAN(strncmp)
>>>>>>> upstream/android-13
