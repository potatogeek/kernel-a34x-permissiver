<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Hibernate low-level support
 *
 * Copyright (C) 2016 ARM Ltd.
 * Author:	James Morse <james.morse@arm.com>
<<<<<<< HEAD
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
>>>>>>> upstream/android-13
 */
#include <linux/linkage.h>
#include <linux/errno.h>

#include <asm/asm-offsets.h>
#include <asm/assembler.h>
#include <asm/cputype.h>
#include <asm/memory.h>
#include <asm/page.h>
#include <asm/virt.h>

/*
 * To prevent the possibility of old and new partial table walks being visible
 * in the tlb, switch the ttbr to a zero page when we invalidate the old
 * records. D4.7.1 'General TLB maintenance requirements' in ARM DDI 0487A.i
 * Even switching to our copied tables will cause a changed output address at
 * each stage of the walk.
 */
<<<<<<< HEAD
.macro break_before_make_ttbr_switch zero_page, page_table, tmp
=======
.macro break_before_make_ttbr_switch zero_page, page_table, tmp, tmp2
>>>>>>> upstream/android-13
	phys_to_ttbr \tmp, \zero_page
	msr	ttbr1_el1, \tmp
	isb
	tlbi	vmalle1
	dsb	nsh
	phys_to_ttbr \tmp, \page_table
<<<<<<< HEAD
=======
	offset_ttbr1 \tmp, \tmp2
>>>>>>> upstream/android-13
	msr	ttbr1_el1, \tmp
	isb
.endm


/*
 * Resume from hibernate
 *
 * Loads temporary page tables then restores the memory image.
 * Finally branches to cpu_resume() to restore the state saved by
 * swsusp_arch_suspend().
 *
 * Because this code has to be copied to a 'safe' page, it can't call out to
 * other functions by PC-relative address. Also remember that it may be
 * mid-way through over-writing other functions. For this reason it contains
<<<<<<< HEAD
 * code from flush_icache_range() and uses the copy_page() macro.
=======
 * code from caches_clean_inval_pou() and uses the copy_page() macro.
>>>>>>> upstream/android-13
 *
 * This 'safe' page is mapped via ttbr0, and executed from there. This function
 * switches to a copy of the linear map in ttbr1, performs the restore, then
 * switches ttbr1 to the original kernel's swapper_pg_dir.
 *
 * All of memory gets written to, including code. We need to clean the kernel
 * text to the Point of Coherence (PoC) before secondary cores can be booted.
 * Because the kernel modules and executable pages mapped to user space are
 * also written as data, we clean all pages we touch to the Point of
 * Unification (PoU).
 *
 * x0: physical address of temporary page tables
 * x1: physical address of swapper page tables
 * x2: address of cpu_resume
 * x3: linear map address of restore_pblist in the current kernel
 * x4: physical address of __hyp_stub_vectors, or 0
 * x5: physical address of a  zero page that remains zero after resume
 */
.pushsection    ".hibernate_exit.text", "ax"
<<<<<<< HEAD
ENTRY(swsusp_arch_suspend_exit)
=======
SYM_CODE_START(swsusp_arch_suspend_exit)
>>>>>>> upstream/android-13
	/*
	 * We execute from ttbr0, change ttbr1 to our copied linear map tables
	 * with a break-before-make via the zero page
	 */
<<<<<<< HEAD
	break_before_make_ttbr_switch	x5, x0, x6
=======
	break_before_make_ttbr_switch	x5, x0, x6, x8
>>>>>>> upstream/android-13

	mov	x21, x1
	mov	x30, x2
	mov	x24, x4
	mov	x25, x5

	/* walk the restore_pblist and use copy_page() to over-write memory */
	mov	x19, x3

1:	ldr	x10, [x19, #HIBERN_PBE_ORIG]
	mov	x0, x10
	ldr	x1, [x19, #HIBERN_PBE_ADDR]

	copy_page	x0, x1, x2, x3, x4, x5, x6, x7, x8, x9

	add	x1, x10, #PAGE_SIZE
<<<<<<< HEAD
	/* Clean the copied page to PoU - based on flush_icache_range() */
	raw_dcache_line_size x2, x3
	sub	x3, x2, #1
	bic	x4, x10, x3
2:	dc	cvau, x4	/* clean D line / unified line */
=======
	/* Clean the copied page to PoU - based on caches_clean_inval_pou() */
	raw_dcache_line_size x2, x3
	sub	x3, x2, #1
	bic	x4, x10, x3
2:	/* clean D line / unified line */
alternative_insn "dc cvau, x4",  "dc civac, x4",  ARM64_WORKAROUND_CLEAN_CACHE
>>>>>>> upstream/android-13
	add	x4, x4, x2
	cmp	x4, x1
	b.lo	2b

	ldr	x19, [x19, #HIBERN_PBE_NEXT]
	cbnz	x19, 1b
	dsb	ish		/* wait for PoU cleaning to finish */

	/* switch to the restored kernels page tables */
<<<<<<< HEAD
	break_before_make_ttbr_switch	x25, x21, x6
=======
	break_before_make_ttbr_switch	x25, x21, x6, x8
>>>>>>> upstream/android-13

	ic	ialluis
	dsb	ish
	isb

	cbz	x24, 3f		/* Do we need to re-initialise EL2? */
	hvc	#0
3:	ret
<<<<<<< HEAD

	.ltorg
ENDPROC(swsusp_arch_suspend_exit)
=======
SYM_CODE_END(swsusp_arch_suspend_exit)
>>>>>>> upstream/android-13

/*
 * Restore the hyp stub.
 * This must be done before the hibernate page is unmapped by _cpu_resume(),
 * but happens before any of the hyp-stub's code is cleaned to PoC.
 *
 * x24: The physical address of __hyp_stub_vectors
 */
<<<<<<< HEAD
el1_sync:
	msr	vbar_el2, x24
	eret
ENDPROC(el1_sync)

.macro invalid_vector	label
\label:
	b \label
ENDPROC(\label)
=======
SYM_CODE_START_LOCAL(el1_sync)
	msr	vbar_el2, x24
	eret
SYM_CODE_END(el1_sync)

.macro invalid_vector	label
SYM_CODE_START_LOCAL(\label)
	b \label
SYM_CODE_END(\label)
>>>>>>> upstream/android-13
.endm

	invalid_vector	el2_sync_invalid
	invalid_vector	el2_irq_invalid
	invalid_vector	el2_fiq_invalid
	invalid_vector	el2_error_invalid
	invalid_vector	el1_sync_invalid
	invalid_vector	el1_irq_invalid
	invalid_vector	el1_fiq_invalid
	invalid_vector	el1_error_invalid

/* el2 vectors - switch el2 here while we restore the memory image. */
	.align 11
<<<<<<< HEAD
ENTRY(hibernate_el2_vectors)
=======
SYM_CODE_START(hibernate_el2_vectors)
>>>>>>> upstream/android-13
	ventry	el2_sync_invalid		// Synchronous EL2t
	ventry	el2_irq_invalid			// IRQ EL2t
	ventry	el2_fiq_invalid			// FIQ EL2t
	ventry	el2_error_invalid		// Error EL2t

	ventry	el2_sync_invalid		// Synchronous EL2h
	ventry	el2_irq_invalid			// IRQ EL2h
	ventry	el2_fiq_invalid			// FIQ EL2h
	ventry	el2_error_invalid		// Error EL2h

	ventry	el1_sync			// Synchronous 64-bit EL1
	ventry	el1_irq_invalid			// IRQ 64-bit EL1
	ventry	el1_fiq_invalid			// FIQ 64-bit EL1
	ventry	el1_error_invalid		// Error 64-bit EL1

	ventry	el1_sync_invalid		// Synchronous 32-bit EL1
	ventry	el1_irq_invalid			// IRQ 32-bit EL1
	ventry	el1_fiq_invalid			// FIQ 32-bit EL1
	ventry	el1_error_invalid		// Error 32-bit EL1
<<<<<<< HEAD
END(hibernate_el2_vectors)
=======
SYM_CODE_END(hibernate_el2_vectors)
>>>>>>> upstream/android-13

.popsection
