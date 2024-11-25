<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/include/asm/assembler.h, arch/arm/mm/proc-macros.S
 *
 * Copyright (C) 1996-2000 Russell King
 * Copyright (C) 2012 ARM Ltd.
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
#ifndef __ASSEMBLY__
#error "Only include this from assembly code"
#endif

#ifndef __ASM_ASSEMBLER_H
#define __ASM_ASSEMBLER_H

#include <asm-generic/export.h>

#include <asm/asm-offsets.h>
<<<<<<< HEAD
#include <asm/cpufeature.h>
=======
#include <asm/alternative.h>
#include <asm/asm-bug.h>
#include <asm/cpufeature.h>
#include <asm/cputype.h>
>>>>>>> upstream/android-13
#include <asm/debug-monitors.h>
#include <asm/page.h>
#include <asm/pgtable-hwdef.h>
#include <asm/ptrace.h>
#include <asm/thread_info.h>

<<<<<<< HEAD
=======
	/*
	 * Provide a wxN alias for each wN register so what we can paste a xN
	 * reference after a 'w' to obtain the 32-bit version.
	 */
	.irp	n,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
	wx\n	.req	w\n
	.endr

>>>>>>> upstream/android-13
	.macro save_and_disable_daif, flags
	mrs	\flags, daif
	msr	daifset, #0xf
	.endm

	.macro disable_daif
	msr	daifset, #0xf
	.endm

	.macro enable_daif
	msr	daifclr, #0xf
	.endm

	.macro	restore_daif, flags:req
	msr	daif, \flags
	.endm

<<<<<<< HEAD
	/* Only on aarch64 pstate, PSR_D_BIT is different for aarch32 */
	.macro	inherit_daif, pstate:req, tmp:req
	and	\tmp, \pstate, #(PSR_D_BIT | PSR_A_BIT | PSR_I_BIT | PSR_F_BIT)
	msr	daif, \tmp
	.endm

	/* IRQ is the lowest priority flag, unconditionally unmask the rest. */
	.macro enable_da_f
	msr	daifclr, #(8 | 4 | 1)
	.endm

/*
 * Enable and disable interrupts.
 */
	.macro	disable_irq
	msr	daifset, #2
	.endm

	.macro	enable_irq
	msr	daifclr, #2
	.endm

	.macro	save_and_disable_irq, flags
	mrs	\flags, daif
	msr	daifset, #2
=======
	/* IRQ/FIQ are the lowest priority flags, unconditionally unmask the rest. */
	.macro enable_da
	msr	daifclr, #(8 | 4)
	.endm

/*
 * Save/restore interrupts.
 */
	.macro	save_and_disable_irq, flags
	mrs	\flags, daif
	msr	daifset, #3
>>>>>>> upstream/android-13
	.endm

	.macro	restore_irq, flags
	msr	daif, \flags
	.endm

<<<<<<< HEAD
/*
 * Save/disable and restore interrupts.
 */
	.macro	save_and_disable_irqs, olddaif
	mrs	\olddaif, daif
	disable_irq
	.endm

	.macro	restore_irqs, olddaif
	msr	daif, \olddaif
	.endm

=======
>>>>>>> upstream/android-13
	.macro	enable_dbg
	msr	daifclr, #8
	.endm

	.macro	disable_step_tsk, flgs, tmp
	tbz	\flgs, #TIF_SINGLESTEP, 9990f
	mrs	\tmp, mdscr_el1
	bic	\tmp, \tmp, #DBG_MDSCR_SS
	msr	mdscr_el1, \tmp
	isb	// Synchronise with enable_dbg
9990:
	.endm

	/* call with daif masked */
	.macro	enable_step_tsk, flgs, tmp
	tbz	\flgs, #TIF_SINGLESTEP, 9990f
	mrs	\tmp, mdscr_el1
	orr	\tmp, \tmp, #DBG_MDSCR_SS
	msr	mdscr_el1, \tmp
9990:
	.endm

/*
<<<<<<< HEAD
 * SMP data memory barrier
 */
	.macro	smp_dmb, opt
	dmb	\opt
	.endm

/*
=======
>>>>>>> upstream/android-13
 * RAS Error Synchronization barrier
 */
	.macro  esb
#ifdef CONFIG_ARM64_RAS_EXTN
	hint    #16
#else
	nop
#endif
	.endm

/*
 * Value prediction barrier
 */
	.macro	csdb
	hint	#20
	.endm

/*
 * Clear Branch History instruction
 */
	.macro clearbhb
	hint	#22
	.endm

/*
<<<<<<< HEAD
 * Sanitise a 64-bit bounded index wrt speculation, returning zero if out
 * of bounds.
 */
	.macro	mask_nospec64, idx, limit, tmp
	sub	\tmp, \idx, \limit
	bic	\tmp, \tmp, \idx
	and	\idx, \idx, \tmp, asr #63
	csdb
=======
 * Speculation barrier
 */
	.macro	sb
alternative_if_not ARM64_HAS_SB
	dsb	nsh
	isb
alternative_else
	SB_BARRIER_INSN
	nop
alternative_endif
>>>>>>> upstream/android-13
	.endm

/*
 * NOP sequence
 */
	.macro	nops, num
	.rept	\num
	nop
	.endr
	.endm

/*
<<<<<<< HEAD
 * Emit an entry into the exception table
 */
	.macro		_asm_extable, from, to
	.pushsection	__ex_table, "a"
	.align		3
	.long		(\from - .), (\to - .)
	.popsection
	.endm

=======
 * Create an exception table entry for `insn`, which will branch to `fixup`
 * when an unhandled fault is taken.
 */
	.macro		_asm_extable, insn, fixup
	.pushsection	__ex_table, "a"
	.align		3
	.long		(\insn - .), (\fixup - .)
	.popsection
	.endm

/*
 * Create an exception table entry for `insn` if `fixup` is provided. Otherwise
 * do nothing.
 */
	.macro		_cond_extable, insn, fixup
	.ifnc		\fixup,
	_asm_extable	\insn, \fixup
	.endif
	.endm


>>>>>>> upstream/android-13
#define USER(l, x...)				\
9999:	x;					\
	_asm_extable	9999b, l

/*
 * Register aliases.
 */
lr	.req	x30		// link register

/*
 * Vector entry
 */
	 .macro	ventry	label
	.align	7
	b	\label
	.endm

/*
 * Select code when configured for BE.
 */
#ifdef CONFIG_CPU_BIG_ENDIAN
#define CPU_BE(code...) code
#else
#define CPU_BE(code...)
#endif

/*
 * Select code when configured for LE.
 */
#ifdef CONFIG_CPU_BIG_ENDIAN
#define CPU_LE(code...)
#else
#define CPU_LE(code...) code
#endif

/*
 * Define a macro that constructs a 64-bit value by concatenating two
 * 32-bit registers. Note that on big endian systems the order of the
 * registers is swapped.
 */
#ifndef CONFIG_CPU_BIG_ENDIAN
	.macro	regs_to_64, rd, lbits, hbits
#else
	.macro	regs_to_64, rd, hbits, lbits
#endif
	orr	\rd, \lbits, \hbits, lsl #32
	.endm

/*
 * Pseudo-ops for PC-relative adr/ldr/str <reg>, <symbol> where
 * <symbol> is within the range +/- 4 GB of the PC.
 */
	/*
	 * @dst: destination register (64 bit wide)
	 * @sym: name of the symbol
	 */
	.macro	adr_l, dst, sym
	adrp	\dst, \sym
	add	\dst, \dst, :lo12:\sym
	.endm

	/*
	 * @dst: destination register (32 or 64 bit wide)
	 * @sym: name of the symbol
	 * @tmp: optional 64-bit scratch register to be used if <dst> is a
	 *       32-bit wide register, in which case it cannot be used to hold
	 *       the address
	 */
	.macro	ldr_l, dst, sym, tmp=
	.ifb	\tmp
	adrp	\dst, \sym
	ldr	\dst, [\dst, :lo12:\sym]
	.else
	adrp	\tmp, \sym
	ldr	\dst, [\tmp, :lo12:\sym]
	.endif
	.endm

	/*
	 * @src: source register (32 or 64 bit wide)
	 * @sym: name of the symbol
	 * @tmp: mandatory 64-bit scratch register to calculate the address
	 *       while <src> needs to be preserved.
	 */
	.macro	str_l, src, sym, tmp
	adrp	\tmp, \sym
	str	\src, [\tmp, :lo12:\sym]
	.endm

	/*
<<<<<<< HEAD
=======
	 * @dst: destination register
	 */
#if defined(__KVM_NVHE_HYPERVISOR__) || defined(__KVM_VHE_HYPERVISOR__)
	.macro	get_this_cpu_offset, dst
	mrs	\dst, tpidr_el2
	.endm
#else
	.macro	get_this_cpu_offset, dst
alternative_if_not ARM64_HAS_VIRT_HOST_EXTN
	mrs	\dst, tpidr_el1
alternative_else
	mrs	\dst, tpidr_el2
alternative_endif
	.endm

	.macro	set_this_cpu_offset, src
alternative_if_not ARM64_HAS_VIRT_HOST_EXTN
	msr	tpidr_el1, \src
alternative_else
	msr	tpidr_el2, \src
alternative_endif
	.endm
#endif

	/*
>>>>>>> upstream/android-13
	 * @dst: Result of per_cpu(sym, smp_processor_id()) (can be SP)
	 * @sym: The name of the per-cpu variable
	 * @tmp: scratch register
	 */
	.macro adr_this_cpu, dst, sym, tmp
	adrp	\tmp, \sym
	add	\dst, \tmp, #:lo12:\sym
<<<<<<< HEAD
alternative_if_not ARM64_HAS_VIRT_HOST_EXTN
	mrs	\tmp, tpidr_el1
alternative_else
	mrs	\tmp, tpidr_el2
alternative_endif
=======
	get_this_cpu_offset \tmp
>>>>>>> upstream/android-13
	add	\dst, \dst, \tmp
	.endm

	/*
	 * @dst: Result of READ_ONCE(per_cpu(sym, smp_processor_id()))
	 * @sym: The name of the per-cpu variable
	 * @tmp: scratch register
	 */
	.macro ldr_this_cpu dst, sym, tmp
	adr_l	\dst, \sym
<<<<<<< HEAD
alternative_if_not ARM64_HAS_VIRT_HOST_EXTN
	mrs	\tmp, tpidr_el1
alternative_else
	mrs	\tmp, tpidr_el2
alternative_endif
=======
	get_this_cpu_offset \tmp
>>>>>>> upstream/android-13
	ldr	\dst, [\dst, \tmp]
	.endm

/*
 * vma_vm_mm - get mm pointer from vma pointer (vma->vm_mm)
 */
	.macro	vma_vm_mm, rd, rn
	ldr	\rd, [\rn, #VMA_VM_MM]
	.endm

/*
<<<<<<< HEAD
 * mmid - get context id from mm pointer (mm->context.id)
 */
	.macro	mmid, rd, rn
	ldr	\rd, [\rn, #MM_CONTEXT_ID]
	.endm
/*
 * read_ctr - read CTR_EL0. If the system has mismatched
 * cache line sizes, provide the system wide safe value
 * from arm64_ftr_reg_ctrel0.sys_val
 */
	.macro	read_ctr, reg
alternative_if_not ARM64_MISMATCHED_CACHE_LINE_SIZE
=======
 * read_ctr - read CTR_EL0. If the system has mismatched register fields,
 * provide the system wide safe value from arm64_ftr_reg_ctrel0.sys_val
 */
	.macro	read_ctr, reg
#ifndef __KVM_NVHE_HYPERVISOR__
alternative_if_not ARM64_MISMATCHED_CACHE_TYPE
>>>>>>> upstream/android-13
	mrs	\reg, ctr_el0			// read CTR
	nop
alternative_else
	ldr_l	\reg, arm64_ftr_reg_ctrel0 + ARM64_FTR_SYSVAL
alternative_endif
<<<<<<< HEAD
=======
#else
alternative_if_not ARM64_KVM_PROTECTED_MODE
	ASM_BUG()
alternative_else_nop_endif
alternative_cb kvm_compute_final_ctr_el0
	movz	\reg, #0
	movk	\reg, #0, lsl #16
	movk	\reg, #0, lsl #32
	movk	\reg, #0, lsl #48
alternative_cb_end
#endif
>>>>>>> upstream/android-13
	.endm


/*
 * raw_dcache_line_size - get the minimum D-cache line size on this CPU
 * from the CTR register.
 */
	.macro	raw_dcache_line_size, reg, tmp
	mrs	\tmp, ctr_el0			// read CTR
	ubfm	\tmp, \tmp, #16, #19		// cache line size encoding
	mov	\reg, #4			// bytes per word
	lsl	\reg, \reg, \tmp		// actual cache line size
	.endm

/*
 * dcache_line_size - get the safe D-cache line size across all CPUs
 */
	.macro	dcache_line_size, reg, tmp
	read_ctr	\tmp
	ubfm		\tmp, \tmp, #16, #19	// cache line size encoding
	mov		\reg, #4		// bytes per word
	lsl		\reg, \reg, \tmp	// actual cache line size
	.endm

/*
 * raw_icache_line_size - get the minimum I-cache line size on this CPU
 * from the CTR register.
 */
	.macro	raw_icache_line_size, reg, tmp
	mrs	\tmp, ctr_el0			// read CTR
	and	\tmp, \tmp, #0xf		// cache line size encoding
	mov	\reg, #4			// bytes per word
	lsl	\reg, \reg, \tmp		// actual cache line size
	.endm

/*
 * icache_line_size - get the safe I-cache line size across all CPUs
 */
	.macro	icache_line_size, reg, tmp
	read_ctr	\tmp
	and		\tmp, \tmp, #0xf	// cache line size encoding
	mov		\reg, #4		// bytes per word
	lsl		\reg, \reg, \tmp	// actual cache line size
	.endm

/*
<<<<<<< HEAD
 * tcr_set_idmap_t0sz - update TCR.T0SZ so that we can load the ID map
 */
	.macro	tcr_set_idmap_t0sz, valreg, tmpreg
	ldr_l	\tmpreg, idmap_t0sz
	bfi	\valreg, \tmpreg, #TCR_T0SZ_OFFSET, #TCR_TxSZ_WIDTH
=======
 * tcr_set_t0sz - update TCR.T0SZ so that we can load the ID map
 */
	.macro	tcr_set_t0sz, valreg, t0sz
	bfi	\valreg, \t0sz, #TCR_T0SZ_OFFSET, #TCR_TxSZ_WIDTH
	.endm

/*
 * tcr_set_t1sz - update TCR.T1SZ
 */
	.macro	tcr_set_t1sz, valreg, t1sz
	bfi	\valreg, \t1sz, #TCR_T1SZ_OFFSET, #TCR_TxSZ_WIDTH
>>>>>>> upstream/android-13
	.endm

/*
 * tcr_compute_pa_size - set TCR.(I)PS to the highest supported
 * ID_AA64MMFR0_EL1.PARange value
 *
 *	tcr:		register with the TCR_ELx value to be updated
 *	pos:		IPS or PS bitfield position
 *	tmp{0,1}:	temporary registers
 */
	.macro	tcr_compute_pa_size, tcr, pos, tmp0, tmp1
	mrs	\tmp0, ID_AA64MMFR0_EL1
	// Narrow PARange to fit the PS field in TCR_ELx
	ubfx	\tmp0, \tmp0, #ID_AA64MMFR0_PARANGE_SHIFT, #3
	mov	\tmp1, #ID_AA64MMFR0_PARANGE_MAX
	cmp	\tmp0, \tmp1
	csel	\tmp0, \tmp1, \tmp0, hi
	bfi	\tcr, \tmp0, \pos, #3
	.endm

<<<<<<< HEAD
/*
 * Macro to perform a data cache maintenance for the interval
 * [kaddr, kaddr + size)
 *
 * 	op:		operation passed to dc instruction
 * 	domain:		domain used in dsb instruciton
 * 	kaddr:		starting virtual address of the region
 * 	size:		size of the region
 * 	Corrupts:	kaddr, size, tmp1, tmp2
 */
	.macro __dcache_op_workaround_clean_cache, op, kaddr
alternative_if_not ARM64_WORKAROUND_CLEAN_CACHE
	dc	\op, \kaddr
alternative_else
	dc	civac, \kaddr
alternative_endif
	.endm

	.macro dcache_by_line_op op, domain, kaddr, size, tmp1, tmp2
	dcache_line_size \tmp1, \tmp2
	add	\size, \kaddr, \size
	sub	\tmp2, \tmp1, #1
	bic	\kaddr, \kaddr, \tmp2
9998:
	.ifc	\op, cvau
	__dcache_op_workaround_clean_cache \op, \kaddr
	.else
	.ifc	\op, cvac
	__dcache_op_workaround_clean_cache \op, \kaddr
	.else
	.ifc	\op, cvap
	sys	3, c7, c12, 1, \kaddr	// dc cvap
	.else
	dc	\op, \kaddr
	.endif
	.endif
	.endif
	add	\kaddr, \kaddr, \tmp1
	cmp	\kaddr, \size
	b.lo	9998b
	dsb	\domain
=======
	.macro __dcache_op_workaround_clean_cache, op, addr
alternative_if_not ARM64_WORKAROUND_CLEAN_CACHE
	dc	\op, \addr
alternative_else
	dc	civac, \addr
alternative_endif
	.endm

/*
 * Macro to perform a data cache maintenance for the interval
 * [start, end)
 *
 * 	op:		operation passed to dc instruction
 * 	domain:		domain used in dsb instruciton
 * 	start:          starting virtual address of the region
 * 	end:            end virtual address of the region
 * 	fixup:		optional label to branch to on user fault
 * 	Corrupts:       start, end, tmp1, tmp2
 */
	.macro dcache_by_line_op op, domain, start, end, tmp1, tmp2, fixup
	dcache_line_size \tmp1, \tmp2
	sub	\tmp2, \tmp1, #1
	bic	\start, \start, \tmp2
.Ldcache_op\@:
	.ifc	\op, cvau
	__dcache_op_workaround_clean_cache \op, \start
	.else
	.ifc	\op, cvac
	__dcache_op_workaround_clean_cache \op, \start
	.else
	.ifc	\op, cvap
	sys	3, c7, c12, 1, \start	// dc cvap
	.else
	.ifc	\op, cvadp
	sys	3, c7, c13, 1, \start	// dc cvadp
	.else
	dc	\op, \start
	.endif
	.endif
	.endif
	.endif
	add	\start, \start, \tmp1
	cmp	\start, \end
	b.lo	.Ldcache_op\@
	dsb	\domain

	_cond_extable .Ldcache_op\@, \fixup
>>>>>>> upstream/android-13
	.endm

/*
 * Macro to perform an instruction cache maintenance for the interval
 * [start, end)
 *
 * 	start, end:	virtual addresses describing the region
<<<<<<< HEAD
 *	label:		A label to branch to on user fault.
 * 	Corrupts:	tmp1, tmp2
 */
	.macro invalidate_icache_by_line start, end, tmp1, tmp2, label
	icache_line_size \tmp1, \tmp2
	sub	\tmp2, \tmp1, #1
	bic	\tmp2, \start, \tmp2
9997:
USER(\label, ic	ivau, \tmp2)			// invalidate I line PoU
	add	\tmp2, \tmp2, \tmp1
	cmp	\tmp2, \end
	b.lo	9997b
	dsb	ish
	isb
=======
 *	fixup:		optional label to branch to on user fault
 * 	Corrupts:	tmp1, tmp2
 */
	.macro invalidate_icache_by_line start, end, tmp1, tmp2, fixup
	icache_line_size \tmp1, \tmp2
	sub	\tmp2, \tmp1, #1
	bic	\tmp2, \start, \tmp2
.Licache_op\@:
	ic	ivau, \tmp2			// invalidate I line PoU
	add	\tmp2, \tmp2, \tmp1
	cmp	\tmp2, \end
	b.lo	.Licache_op\@
	dsb	ish
	isb

	_cond_extable .Licache_op\@, \fixup
>>>>>>> upstream/android-13
	.endm

/*
 * reset_pmuserenr_el0 - reset PMUSERENR_EL0 if PMUv3 present
 */
	.macro	reset_pmuserenr_el0, tmpreg
<<<<<<< HEAD
	mrs	\tmpreg, id_aa64dfr0_el1	// Check ID_AA64DFR0_EL1 PMUVer
	sbfx	\tmpreg, \tmpreg, #8, #4
=======
	mrs	\tmpreg, id_aa64dfr0_el1
	sbfx	\tmpreg, \tmpreg, #ID_AA64DFR0_PMUVER_SHIFT, #4
>>>>>>> upstream/android-13
	cmp	\tmpreg, #1			// Skip if no PMU present
	b.lt	9000f
	msr	pmuserenr_el0, xzr		// Disable PMU access from EL0
9000:
	.endm

/*
<<<<<<< HEAD
=======
 * reset_amuserenr_el0 - reset AMUSERENR_EL0 if AMUv1 present
 */
	.macro	reset_amuserenr_el0, tmpreg
	mrs	\tmpreg, id_aa64pfr0_el1	// Check ID_AA64PFR0_EL1
	ubfx	\tmpreg, \tmpreg, #ID_AA64PFR0_AMU_SHIFT, #4
	cbz	\tmpreg, .Lskip_\@		// Skip if no AMU present
	msr_s	SYS_AMUSERENR_EL0, xzr		// Disable AMU access from EL0
.Lskip_\@:
	.endm
/*
>>>>>>> upstream/android-13
 * copy_page - copy src to dest using temp registers t1-t8
 */
	.macro copy_page dest:req src:req t1:req t2:req t3:req t4:req t5:req t6:req t7:req t8:req
9998:	ldp	\t1, \t2, [\src]
	ldp	\t3, \t4, [\src, #16]
	ldp	\t5, \t6, [\src, #32]
	ldp	\t7, \t8, [\src, #48]
	add	\src, \src, #64
	stnp	\t1, \t2, [\dest]
	stnp	\t3, \t4, [\dest, #16]
	stnp	\t5, \t6, [\dest, #32]
	stnp	\t7, \t8, [\dest, #48]
	add	\dest, \dest, #64
	tst	\src, #(PAGE_SIZE - 1)
	b.ne	9998b
	.endm

/*
<<<<<<< HEAD
 * Annotate a function as position independent, i.e., safe to be called before
 * the kernel virtual mapping is activated.
 */
#define ENDPIPROC(x)			\
	.globl	__pi_##x;		\
	.type 	__pi_##x, %function;	\
	.set	__pi_##x, x;		\
	.size	__pi_##x, . - x;	\
	ENDPROC(x)

/*
=======
>>>>>>> upstream/android-13
 * Annotate a function as being unsuitable for kprobes.
 */
#ifdef CONFIG_KPROBES
#define NOKPROBE(x)				\
	.pushsection "_kprobe_blacklist", "aw";	\
	.quad	x;				\
	.popsection;
#else
#define NOKPROBE(x)
#endif

<<<<<<< HEAD
#ifdef CONFIG_KASAN
=======
#if defined(CONFIG_KASAN_GENERIC) || defined(CONFIG_KASAN_SW_TAGS)
>>>>>>> upstream/android-13
#define EXPORT_SYMBOL_NOKASAN(name)
#else
#define EXPORT_SYMBOL_NOKASAN(name)	EXPORT_SYMBOL(name)
#endif

<<<<<<< HEAD
=======
#ifdef CONFIG_KASAN_HW_TAGS
#define EXPORT_SYMBOL_NOHWKASAN(name)
#else
#define EXPORT_SYMBOL_NOHWKASAN(name)	EXPORT_SYMBOL_NOKASAN(name)
#endif
>>>>>>> upstream/android-13
	/*
	 * Emit a 64-bit absolute little endian symbol reference in a way that
	 * ensures that it will be resolved at build time, even when building a
	 * PIE binary. This requires cooperation from the linker script, which
	 * must emit the lo32/hi32 halves individually.
	 */
	.macro	le64sym, sym
	.long	\sym\()_lo32
	.long	\sym\()_hi32
	.endm

	/*
	 * mov_q - move an immediate constant into a 64-bit register using
	 *         between 2 and 4 movz/movk instructions (depending on the
	 *         magnitude and sign of the operand)
	 */
	.macro	mov_q, reg, val
	.if (((\val) >> 31) == 0 || ((\val) >> 31) == 0x1ffffffff)
	movz	\reg, :abs_g1_s:\val
	.else
	.if (((\val) >> 47) == 0 || ((\val) >> 47) == 0x1ffff)
	movz	\reg, :abs_g2_s:\val
	.else
	movz	\reg, :abs_g3:\val
	movk	\reg, :abs_g2_nc:\val
	.endif
	movk	\reg, :abs_g1_nc:\val
	.endif
	movk	\reg, :abs_g0_nc:\val
	.endm

/*
<<<<<<< HEAD
 * Return the current thread_info.
 */
	.macro	get_thread_info, rd
=======
 * Return the current task_struct.
 */
	.macro	get_current_task, rd
>>>>>>> upstream/android-13
	mrs	\rd, sp_el0
	.endm

/*
<<<<<<< HEAD
=======
 * Offset ttbr1 to allow for 48-bit kernel VAs set with 52-bit PTRS_PER_PGD.
 * orr is used as it can cover the immediate value (and is idempotent).
 * In future this may be nop'ed out when dealing with 52-bit kernel VAs.
 * 	ttbr: Value of ttbr to set, modified.
 */
	.macro	offset_ttbr1, ttbr, tmp
#ifdef CONFIG_ARM64_VA_BITS_52
	mrs_s	\tmp, SYS_ID_AA64MMFR2_EL1
	and	\tmp, \tmp, #(0xf << ID_AA64MMFR2_LVA_SHIFT)
	cbnz	\tmp, .Lskipoffs_\@
	orr	\ttbr, \ttbr, #TTBR1_BADDR_4852_OFFSET
.Lskipoffs_\@ :
#endif
	.endm

/*
 * Perform the reverse of offset_ttbr1.
 * bic is used as it can cover the immediate value and, in future, won't need
 * to be nop'ed out when dealing with 52-bit kernel VAs.
 */
	.macro	restore_ttbr1, ttbr
#ifdef CONFIG_ARM64_VA_BITS_52
	bic	\ttbr, \ttbr, #TTBR1_BADDR_4852_OFFSET
#endif
	.endm

/*
>>>>>>> upstream/android-13
 * Arrange a physical address in a TTBR register, taking care of 52-bit
 * addresses.
 *
 * 	phys:	physical address, preserved
 * 	ttbr:	returns the TTBR value
 */
	.macro	phys_to_ttbr, ttbr, phys
#ifdef CONFIG_ARM64_PA_BITS_52
	orr	\ttbr, \phys, \phys, lsr #46
	and	\ttbr, \ttbr, #TTBR_BADDR_MASK_52
#else
	mov	\ttbr, \phys
#endif
	.endm

	.macro	phys_to_pte, pte, phys
#ifdef CONFIG_ARM64_PA_BITS_52
	/*
	 * We assume \phys is 64K aligned and this is guaranteed by only
	 * supporting this configuration with 64K pages.
	 */
	orr	\pte, \phys, \phys, lsr #36
	and	\pte, \pte, #PTE_ADDR_MASK
#else
	mov	\pte, \phys
#endif
	.endm

	.macro	pte_to_phys, phys, pte
#ifdef CONFIG_ARM64_PA_BITS_52
	ubfiz	\phys, \pte, #(48 - 16 - 12), #16
	bfxil	\phys, \pte, #16, #32
	lsl	\phys, \phys, #16
#else
	and	\phys, \pte, #PTE_ADDR_MASK
#endif
	.endm

<<<<<<< HEAD
=======
/*
 * tcr_clear_errata_bits - Clear TCR bits that trigger an errata on this CPU.
 */
	.macro	tcr_clear_errata_bits, tcr, tmp1, tmp2
#ifdef CONFIG_FUJITSU_ERRATUM_010001
	mrs	\tmp1, midr_el1

	mov_q	\tmp2, MIDR_FUJITSU_ERRATUM_010001_MASK
	and	\tmp1, \tmp1, \tmp2
	mov_q	\tmp2, MIDR_FUJITSU_ERRATUM_010001
	cmp	\tmp1, \tmp2
	b.ne	10f

	mov_q	\tmp2, TCR_CLEAR_FUJITSU_ERRATUM_010001
	bic	\tcr, \tcr, \tmp2
10:
#endif /* CONFIG_FUJITSU_ERRATUM_010001 */
	.endm

>>>>>>> upstream/android-13
/**
 * Errata workaround prior to disable MMU. Insert an ISB immediately prior
 * to executing the MSR that will change SCTLR_ELn[M] from a value of 1 to 0.
 */
	.macro pre_disable_mmu_workaround
#ifdef CONFIG_QCOM_FALKOR_ERRATUM_E1041
	isb
#endif
	.endm

	/*
	 * frame_push - Push @regcount callee saved registers to the stack,
	 *              starting at x19, as well as x29/x30, and set x29 to
	 *              the new value of sp. Add @extra bytes of stack space
	 *              for locals.
	 */
	.macro		frame_push, regcount:req, extra
	__frame		st, \regcount, \extra
	.endm

	/*
	 * frame_pop  - Pop the callee saved registers from the stack that were
	 *              pushed in the most recent call to frame_push, as well
	 *              as x29/x30 and any extra stack space that may have been
	 *              allocated.
	 */
	.macro		frame_pop
	__frame		ld
	.endm

	.macro		__frame_regs, reg1, reg2, op, num
	.if		.Lframe_regcount == \num
	\op\()r		\reg1, [sp, #(\num + 1) * 8]
	.elseif		.Lframe_regcount > \num
	\op\()p		\reg1, \reg2, [sp, #(\num + 1) * 8]
	.endif
	.endm

	.macro		__frame, op, regcount, extra=0
	.ifc		\op, st
	.if		(\regcount) < 0 || (\regcount) > 10
	.error		"regcount should be in the range [0 ... 10]"
	.endif
	.if		((\extra) % 16) != 0
	.error		"extra should be a multiple of 16 bytes"
	.endif
	.ifdef		.Lframe_regcount
	.if		.Lframe_regcount != -1
	.error		"frame_push/frame_pop may not be nested"
	.endif
	.endif
	.set		.Lframe_regcount, \regcount
	.set		.Lframe_extra, \extra
	.set		.Lframe_local_offset, ((\regcount + 3) / 2) * 16
	stp		x29, x30, [sp, #-.Lframe_local_offset - .Lframe_extra]!
	mov		x29, sp
	.endif

	__frame_regs	x19, x20, \op, 1
	__frame_regs	x21, x22, \op, 3
	__frame_regs	x23, x24, \op, 5
	__frame_regs	x25, x26, \op, 7
	__frame_regs	x27, x28, \op, 9

	.ifc		\op, ld
	.if		.Lframe_regcount == -1
	.error		"frame_push/frame_pop may not be nested"
	.endif
	ldp		x29, x30, [sp], #.Lframe_local_offset + .Lframe_extra
	.set		.Lframe_regcount, -1
	.endif
	.endm

/*
<<<<<<< HEAD
 * Check whether to yield to another runnable task from kernel mode NEON code
 * (which runs with preemption disabled).
 *
 * if_will_cond_yield_neon
 *        // pre-yield patchup code
 * do_cond_yield_neon
 *        // post-yield patchup code
 * endif_yield_neon    <label>
 *
 * where <label> is optional, and marks the point where execution will resume
 * after a yield has been performed. If omitted, execution resumes right after
 * the endif_yield_neon invocation. Note that the entire sequence, including
 * the provided patchup code, will be omitted from the image if CONFIG_PREEMPT
 * is not defined.
 *
 * As a convenience, in the case where no patchup code is required, the above
 * sequence may be abbreviated to
 *
 * cond_yield_neon <label>
 *
 * Note that the patchup code does not support assembler directives that change
 * the output section, any use of such directives is undefined.
 *
 * The yield itself consists of the following:
 * - Check whether the preempt count is exactly 1, in which case disabling
 *   preemption once will make the task preemptible. If this is not the case,
 *   yielding is pointless.
 * - Check whether TIF_NEED_RESCHED is set, and if so, disable and re-enable
 *   kernel mode NEON (which will trigger a reschedule), and branch to the
 *   yield fixup code.
 *
 * This macro sequence may clobber all CPU state that is not guaranteed by the
 * AAPCS to be preserved across an ordinary function call.
 */

	.macro		cond_yield_neon, lbl
	if_will_cond_yield_neon
	do_cond_yield_neon
	endif_yield_neon	\lbl
	.endm

	.macro		if_will_cond_yield_neon
#ifdef CONFIG_PREEMPT
	get_thread_info	x0
	ldr		w1, [x0, #TSK_TI_PREEMPT]
	ldr		x0, [x0, #TSK_TI_FLAGS]
	cmp		w1, #PREEMPT_DISABLE_OFFSET
	csel		x0, x0, xzr, eq
	tbnz		x0, #TIF_NEED_RESCHED, .Lyield_\@	// needs rescheduling?
	/* fall through to endif_yield_neon */
	.subsection	1
.Lyield_\@ :
#else
	.section	".discard.cond_yield_neon", "ax"
#endif
	.endm

	.macro		do_cond_yield_neon
	bl		kernel_neon_end
	bl		kernel_neon_begin
	.endm

	.macro		endif_yield_neon, lbl
	.ifnb		\lbl
	b		\lbl
	.else
	b		.Lyield_out_\@
	.endif
	.previous
.Lyield_out_\@ :
	.endm
=======
 * Set SCTLR_ELx to the @reg value, and invalidate the local icache
 * in the process. This is called when setting the MMU on.
 */
.macro set_sctlr, sreg, reg
	msr	\sreg, \reg
	isb
	/*
	 * Invalidate the local I-cache so that any instructions fetched
	 * speculatively from the PoC are discarded, since they may have
	 * been dynamically patched at the PoU.
	 */
	ic	iallu
	dsb	nsh
	isb
.endm

.macro set_sctlr_el1, reg
	set_sctlr sctlr_el1, \reg
.endm

.macro set_sctlr_el2, reg
	set_sctlr sctlr_el2, \reg
.endm

	/*
	 * Check whether preempt/bh-disabled asm code should yield as soon as
	 * it is able. This is the case if we are currently running in task
	 * context, and either a softirq is pending, or the TIF_NEED_RESCHED
	 * flag is set and re-enabling preemption a single time would result in
	 * a preempt count of zero. (Note that the TIF_NEED_RESCHED flag is
	 * stored negated in the top word of the thread_info::preempt_count
	 * field)
	 */
	.macro		cond_yield, lbl:req, tmp:req, tmp2:req
	get_current_task \tmp
	ldr		\tmp, [\tmp, #TSK_TI_PREEMPT]
	/*
	 * If we are serving a softirq, there is no point in yielding: the
	 * softirq will not be preempted no matter what we do, so we should
	 * run to completion as quickly as we can.
	 */
	tbnz		\tmp, #SOFTIRQ_SHIFT, .Lnoyield_\@
#ifdef CONFIG_PREEMPTION
	sub		\tmp, \tmp, #PREEMPT_DISABLE_OFFSET
	cbz		\tmp, \lbl
#endif
	adr_l		\tmp, irq_stat + IRQ_CPUSTAT_SOFTIRQ_PENDING
	get_this_cpu_offset	\tmp2
	ldr		w\tmp, [\tmp, \tmp2]
	cbnz		w\tmp, \lbl	// yield on pending softirq in task context
.Lnoyield_\@:
	.endm

/*
 * This macro emits a program property note section identifying
 * architecture features which require special handling, mainly for
 * use in assembly files included in the VDSO.
 */

#define NT_GNU_PROPERTY_TYPE_0  5
#define GNU_PROPERTY_AARCH64_FEATURE_1_AND      0xc0000000

#define GNU_PROPERTY_AARCH64_FEATURE_1_BTI      (1U << 0)
#define GNU_PROPERTY_AARCH64_FEATURE_1_PAC      (1U << 1)

#ifdef CONFIG_ARM64_BTI_KERNEL
#define GNU_PROPERTY_AARCH64_FEATURE_1_DEFAULT		\
		((GNU_PROPERTY_AARCH64_FEATURE_1_BTI |	\
		  GNU_PROPERTY_AARCH64_FEATURE_1_PAC))
#endif

#ifdef GNU_PROPERTY_AARCH64_FEATURE_1_DEFAULT
.macro emit_aarch64_feature_1_and, feat=GNU_PROPERTY_AARCH64_FEATURE_1_DEFAULT
	.pushsection .note.gnu.property, "a"
	.align  3
	.long   2f - 1f
	.long   6f - 3f
	.long   NT_GNU_PROPERTY_TYPE_0
1:      .string "GNU"
2:
	.align  3
3:      .long   GNU_PROPERTY_AARCH64_FEATURE_1_AND
	.long   5f - 4f
4:
	/*
	 * This is described with an array of char in the Linux API
	 * spec but the text and all other usage (including binutils,
	 * clang and GCC) treat this as a 32 bit value so no swizzling
	 * is required for big endian.
	 */
	.long   \feat
5:
	.align  3
6:
	.popsection
.endm

#else
.macro emit_aarch64_feature_1_and, feat=0
.endm

#endif /* GNU_PROPERTY_AARCH64_FEATURE_1_DEFAULT */
>>>>>>> upstream/android-13

	.macro __mitigate_spectre_bhb_loop      tmp
#ifdef CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY
alternative_cb  spectre_bhb_patch_loop_iter
	mov	\tmp, #32		// Patched to correct the immediate
alternative_cb_end
.Lspectre_bhb_loop\@:
	b	. + 4
	subs	\tmp, \tmp, #1
	b.ne	.Lspectre_bhb_loop\@
<<<<<<< HEAD
	dsb	nsh
	isb
=======
	sb
#endif /* CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY */
	.endm

	.macro mitigate_spectre_bhb_loop	tmp
#ifdef CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY
alternative_cb	spectre_bhb_patch_loop_mitigation_enable
	b	.L_spectre_bhb_loop_done\@	// Patched to NOP
alternative_cb_end
	__mitigate_spectre_bhb_loop	\tmp
.L_spectre_bhb_loop_done\@:
>>>>>>> upstream/android-13
#endif /* CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY */
	.endm

	/* Save/restores x0-x3 to the stack */
	.macro __mitigate_spectre_bhb_fw
#ifdef CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY
	stp	x0, x1, [sp, #-16]!
	stp	x2, x3, [sp, #-16]!
	mov	w0, #ARM_SMCCC_ARCH_WORKAROUND_3
<<<<<<< HEAD
alternative_cb	arm64_update_smccc_conduit
=======
alternative_cb	smccc_patch_fw_mitigation_conduit
>>>>>>> upstream/android-13
	nop					// Patched to SMC/HVC #0
alternative_cb_end
	ldp	x2, x3, [sp], #16
	ldp	x0, x1, [sp], #16
#endif /* CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY */
	.endm
<<<<<<< HEAD
=======

	.macro mitigate_spectre_bhb_clear_insn
#ifdef CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY
alternative_cb	spectre_bhb_patch_clearbhb
	/* Patched to NOP when not supported */
	clearbhb
	isb
alternative_cb_end
#endif /* CONFIG_MITIGATE_SPECTRE_BRANCH_HISTORY */
	.endm
>>>>>>> upstream/android-13
#endif	/* __ASM_ASSEMBLER_H */
