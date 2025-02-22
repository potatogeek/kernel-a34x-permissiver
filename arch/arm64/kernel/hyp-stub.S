<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Hypervisor stub
 *
 * Copyright (C) 2012 ARM Ltd.
 * Author:	Marc Zyngier <marc.zyngier@arm.com>
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

#include <linux/init.h>
#include <linux/linkage.h>
<<<<<<< HEAD
#include <linux/irqchip/arm-gic-v3.h>

#include <asm/assembler.h>
=======

#include <asm/assembler.h>
#include <asm/el2_setup.h>
>>>>>>> upstream/android-13
#include <asm/kvm_arm.h>
#include <asm/kvm_asm.h>
#include <asm/ptrace.h>
#include <asm/virt.h>

	.text
	.pushsection	.hyp.text, "ax"

	.align 11

<<<<<<< HEAD
ENTRY(__hyp_stub_vectors)
=======
SYM_CODE_START(__hyp_stub_vectors)
>>>>>>> upstream/android-13
	ventry	el2_sync_invalid		// Synchronous EL2t
	ventry	el2_irq_invalid			// IRQ EL2t
	ventry	el2_fiq_invalid			// FIQ EL2t
	ventry	el2_error_invalid		// Error EL2t

<<<<<<< HEAD
	ventry	el2_sync_invalid		// Synchronous EL2h
=======
	ventry	elx_sync			// Synchronous EL2h
>>>>>>> upstream/android-13
	ventry	el2_irq_invalid			// IRQ EL2h
	ventry	el2_fiq_invalid			// FIQ EL2h
	ventry	el2_error_invalid		// Error EL2h

<<<<<<< HEAD
	ventry	el1_sync			// Synchronous 64-bit EL1
=======
	ventry	elx_sync			// Synchronous 64-bit EL1
>>>>>>> upstream/android-13
	ventry	el1_irq_invalid			// IRQ 64-bit EL1
	ventry	el1_fiq_invalid			// FIQ 64-bit EL1
	ventry	el1_error_invalid		// Error 64-bit EL1

	ventry	el1_sync_invalid		// Synchronous 32-bit EL1
	ventry	el1_irq_invalid			// IRQ 32-bit EL1
	ventry	el1_fiq_invalid			// FIQ 32-bit EL1
	ventry	el1_error_invalid		// Error 32-bit EL1
<<<<<<< HEAD
ENDPROC(__hyp_stub_vectors)

	.align 11

el1_sync:
	cmp	x0, #HVC_SET_VECTORS
	b.ne	2f
	msr	vbar_el2, x1
	b	9f

=======
SYM_CODE_END(__hyp_stub_vectors)

	.align 11

SYM_CODE_START_LOCAL(elx_sync)
	cmp	x0, #HVC_SET_VECTORS
	b.ne	1f
	msr	vbar_el2, x1
	b	9f

1:	cmp	x0, #HVC_VHE_RESTART
	b.eq	mutate_to_vhe

>>>>>>> upstream/android-13
2:	cmp	x0, #HVC_SOFT_RESTART
	b.ne	3f
	mov	x0, x2
	mov	x2, x4
	mov	x4, x1
	mov	x1, x3
	br	x4				// no return

3:	cmp	x0, #HVC_RESET_VECTORS
	beq	9f				// Nothing to reset!

	/* Someone called kvm_call_hyp() against the hyp-stub... */
<<<<<<< HEAD
	ldr	x0, =HVC_STUB_ERR
=======
	mov_q	x0, HVC_STUB_ERR
>>>>>>> upstream/android-13
	eret

9:	mov	x0, xzr
	eret
<<<<<<< HEAD
ENDPROC(el1_sync)

.macro invalid_vector	label
\label:
	b \label
ENDPROC(\label)
=======
SYM_CODE_END(elx_sync)

// nVHE? No way! Give me the real thing!
SYM_CODE_START_LOCAL(mutate_to_vhe)
	// Sanity check: MMU *must* be off
	mrs	x1, sctlr_el2
	tbnz	x1, #0, 1f

	// Needs to be VHE capable, obviously
	mrs	x1, id_aa64mmfr1_el1
	ubfx	x1, x1, #ID_AA64MMFR1_VHE_SHIFT, #4
	cbz	x1, 1f

	// Check whether VHE is disabled from the command line
	adr_l	x1, id_aa64mmfr1_override
	ldr	x2, [x1, FTR_OVR_VAL_OFFSET]
	ldr	x1, [x1, FTR_OVR_MASK_OFFSET]
	ubfx	x2, x2, #ID_AA64MMFR1_VHE_SHIFT, #4
	ubfx	x1, x1, #ID_AA64MMFR1_VHE_SHIFT, #4
	cmp	x1, xzr
	and	x2, x2, x1
	csinv	x2, x2, xzr, ne
	cbnz	x2, 2f

1:	mov_q	x0, HVC_STUB_ERR
	eret
2:
	// Engage the VHE magic!
	mov_q	x0, HCR_HOST_VHE_FLAGS
	msr	hcr_el2, x0
	isb

	// Use the EL1 allocated stack, per-cpu offset
	mrs	x0, sp_el1
	mov	sp, x0
	mrs	x0, tpidr_el1
	msr	tpidr_el2, x0

	// FP configuration, vectors
	mrs_s	x0, SYS_CPACR_EL12
	msr	cpacr_el1, x0
	mrs_s	x0, SYS_VBAR_EL12
	msr	vbar_el1, x0

	// Use EL2 translations for SPE & TRBE and disable access from EL1
	mrs	x0, mdcr_el2
	bic	x0, x0, #(MDCR_EL2_E2PB_MASK << MDCR_EL2_E2PB_SHIFT)
	bic	x0, x0, #(MDCR_EL2_E2TB_MASK << MDCR_EL2_E2TB_SHIFT)
	msr	mdcr_el2, x0

	// Transfer the MM state from EL1 to EL2
	mrs_s	x0, SYS_TCR_EL12
	msr	tcr_el1, x0
	mrs_s	x0, SYS_TTBR0_EL12
	msr	ttbr0_el1, x0
	mrs_s	x0, SYS_TTBR1_EL12
	msr	ttbr1_el1, x0
	mrs_s	x0, SYS_MAIR_EL12
	msr	mair_el1, x0
	isb

	// Hack the exception return to stay at EL2
	mrs	x0, spsr_el1
	and	x0, x0, #~PSR_MODE_MASK
	mov	x1, #PSR_MODE_EL2h
	orr	x0, x0, x1
	msr	spsr_el1, x0

	b	enter_vhe
SYM_CODE_END(mutate_to_vhe)

	// At the point where we reach enter_vhe(), we run with
	// the MMU off (which is enforced by mutate_to_vhe()).
	// We thus need to be in the idmap, or everything will
	// explode when enabling the MMU.

	.pushsection	.idmap.text, "ax"

SYM_CODE_START_LOCAL(enter_vhe)
	// Invalidate TLBs before enabling the MMU
	tlbi	vmalle1
	dsb	nsh
	isb

	// Enable the EL2 S1 MMU, as set up from EL1
	mrs_s	x0, SYS_SCTLR_EL12
	set_sctlr_el1	x0

	// Disable the EL1 S1 MMU for a good measure
	mov_q	x0, INIT_SCTLR_EL1_MMU_OFF
	msr_s	SYS_SCTLR_EL12, x0

	mov	x0, xzr

	eret
SYM_CODE_END(enter_vhe)

	.popsection

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

<<<<<<< HEAD
=======
	.popsection

>>>>>>> upstream/android-13
/*
 * __hyp_set_vectors: Call this after boot to set the initial hypervisor
 * vectors as part of hypervisor installation.  On an SMP system, this should
 * be called on each CPU.
 *
 * x0 must be the physical address of the new vector table, and must be
 * 2KB aligned.
 *
 * Before calling this, you must check that the stub hypervisor is installed
 * everywhere, by waiting for any secondary CPUs to be brought up and then
 * checking that is_hyp_mode_available() is true.
 *
 * If not, there is a pre-existing hypervisor, some CPUs failed to boot, or
 * something else went wrong... in such cases, trying to install a new
 * hypervisor is unlikely to work as desired.
 *
 * When you call into your shiny new hypervisor, sp_el2 will contain junk,
 * so you will need to set that to something sensible at the new hypervisor's
 * initialisation entry point.
 */

<<<<<<< HEAD
ENTRY(__hyp_set_vectors)
=======
SYM_FUNC_START(__hyp_set_vectors)
>>>>>>> upstream/android-13
	mov	x1, x0
	mov	x0, #HVC_SET_VECTORS
	hvc	#0
	ret
<<<<<<< HEAD
ENDPROC(__hyp_set_vectors)

ENTRY(__hyp_reset_vectors)
	mov	x0, #HVC_RESET_VECTORS
	hvc	#0
	ret
ENDPROC(__hyp_reset_vectors)
=======
SYM_FUNC_END(__hyp_set_vectors)

SYM_FUNC_START(__hyp_reset_vectors)
	mov	x0, #HVC_RESET_VECTORS
	hvc	#0
	ret
SYM_FUNC_END(__hyp_reset_vectors)

/*
 * Entry point to switch to VHE if deemed capable
 */
SYM_FUNC_START(switch_to_vhe)
	// Need to have booted at EL2
	adr_l	x1, __boot_cpu_mode
	ldr	w0, [x1]
	cmp	w0, #BOOT_CPU_MODE_EL2
	b.ne	1f

	// and still be at EL1
	mrs	x0, CurrentEL
	cmp	x0, #CurrentEL_EL1
	b.ne	1f

	// Turn the world upside down
	mov	x0, #HVC_VHE_RESTART
	hvc	#0
1:
	ret
SYM_FUNC_END(switch_to_vhe)
>>>>>>> upstream/android-13
