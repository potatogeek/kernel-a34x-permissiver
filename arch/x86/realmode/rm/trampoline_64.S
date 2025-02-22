/* SPDX-License-Identifier: GPL-2.0 */
/*
 *
 *	Trampoline.S	Derived from Setup.S by Linus Torvalds
 *
 *	4 Jan 1997 Michael Chastain: changed to gnu as.
 *	15 Sept 2005 Eric Biederman: 64bit PIC support
 *
 *	Entry: CS:IP point to the start of our code, we are
 *	in real mode with no stack, but the rest of the
 *	trampoline page to make our stack and everything else
 *	is a mystery.
 *
 *	On entry to trampoline_start, the processor is in real mode
 *	with 16-bit addressing and 16-bit data.  CS has some value
 *	and IP is zero.  Thus, data addresses need to be absolute
 *	(no relocation) and are taken with regard to r_base.
 *
 *	With the addition of trampoline_level4_pgt this code can
 *	now enter a 64bit kernel that lives at arbitrary 64bit
 *	physical addresses.
 *
 *	If you work on this file, check the object module with objdump
 *	--full-contents --reloc to make sure there are no relocation
 *	entries.
 */

#include <linux/linkage.h>
#include <asm/pgtable_types.h>
#include <asm/page_types.h>
#include <asm/msr.h>
#include <asm/segment.h>
#include <asm/processor-flags.h>
#include <asm/realmode.h>
#include "realmode.h"

	.text
	.code16

	.balign	PAGE_SIZE
<<<<<<< HEAD
ENTRY(trampoline_start)
=======
SYM_CODE_START(trampoline_start)
>>>>>>> upstream/android-13
	cli			# We should be safe anyway
	wbinvd

	LJMPW_RM(1f)
1:
	mov	%cs, %ax	# Code and data in the same place
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %ss

<<<<<<< HEAD
	movl	$0xA5A5A5A5, trampoline_status
	# write marker for master knows we're running

=======
>>>>>>> upstream/android-13
	# Setup stack
	movl	$rm_stack_end, %esp

	call	verify_cpu		# Verify the cpu supports long mode
	testl   %eax, %eax		# Check for return code
	jnz	no_longmode

<<<<<<< HEAD
=======
.Lswitch_to_protected:
>>>>>>> upstream/android-13
	/*
	 * GDT tables in non default location kernel can be beyond 16MB and
	 * lgdt will not be able to load the address as in real mode default
	 * operand size is 16bit. Use lgdtl instead to force operand size
	 * to 32 bit.
	 */

	lidtl	tr_idt	# load idt with 0, 0
	lgdtl	tr_gdt	# load gdt with whatever is appropriate

	movw	$__KERNEL_DS, %dx	# Data segment descriptor

	# Enable protected mode
	movl	$X86_CR0_PE, %eax	# protected mode (PE) bit
	movl	%eax, %cr0		# into protected mode

	# flush prefetch and jump to startup_32
	ljmpl	$__KERNEL32_CS, $pa_startup_32

no_longmode:
	hlt
	jmp no_longmode
<<<<<<< HEAD
=======
SYM_CODE_END(trampoline_start)

#ifdef CONFIG_AMD_MEM_ENCRYPT
/* SEV-ES supports non-zero IP for entry points - no alignment needed */
SYM_CODE_START(sev_es_trampoline_start)
	cli			# We should be safe anyway

	LJMPW_RM(1f)
1:
	mov	%cs, %ax	# Code and data in the same place
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %ss

	# Setup stack
	movl	$rm_stack_end, %esp

	jmp	.Lswitch_to_protected
SYM_CODE_END(sev_es_trampoline_start)
#endif	/* CONFIG_AMD_MEM_ENCRYPT */

>>>>>>> upstream/android-13
#include "../kernel/verify_cpu.S"

	.section ".text32","ax"
	.code32
	.balign 4
<<<<<<< HEAD
ENTRY(startup_32)
=======
SYM_CODE_START(startup_32)
>>>>>>> upstream/android-13
	movl	%edx, %ss
	addl	$pa_real_mode_base, %esp
	movl	%edx, %ds
	movl	%edx, %es
	movl	%edx, %fs
	movl	%edx, %gs

	/*
	 * Check for memory encryption support. This is a safety net in
	 * case BIOS hasn't done the necessary step of setting the bit in
	 * the MSR for this AP. If SME is active and we've gotten this far
	 * then it is safe for us to set the MSR bit and continue. If we
	 * don't we'll eventually crash trying to execute encrypted
	 * instructions.
	 */
	btl	$TH_FLAGS_SME_ACTIVE_BIT, pa_tr_flags
	jnc	.Ldone
<<<<<<< HEAD
	movl	$MSR_K8_SYSCFG, %ecx
	rdmsr
	bts	$MSR_K8_SYSCFG_MEM_ENCRYPT_BIT, %eax
=======
	movl	$MSR_AMD64_SYSCFG, %ecx
	rdmsr
	bts	$MSR_AMD64_SYSCFG_MEM_ENCRYPT_BIT, %eax
>>>>>>> upstream/android-13
	jc	.Ldone

	/*
	 * Memory encryption is enabled but the SME enable bit for this
	 * CPU has has not been set.  It is safe to set it, so do so.
	 */
	wrmsr
.Ldone:

	movl	pa_tr_cr4, %eax
	movl	%eax, %cr4		# Enable PAE mode

	# Setup trampoline 4 level pagetables
	movl	$pa_trampoline_pgd, %eax
	movl	%eax, %cr3

	# Set up EFER
	movl	pa_tr_efer, %eax
	movl	pa_tr_efer + 4, %edx
	movl	$MSR_EFER, %ecx
	wrmsr

	# Enable paging and in turn activate Long Mode
	movl	$(X86_CR0_PG | X86_CR0_WP | X86_CR0_PE), %eax
	movl	%eax, %cr0

	/*
	 * At this point we're in long mode but in 32bit compatibility mode
	 * with EFER.LME = 1, CS.L = 0, CS.D = 1 (and in turn
	 * EFER.LMA = 1). Now we want to jump in 64bit mode, to do that we use
	 * the new gdt/idt that has __KERNEL_CS with CS.L = 1.
	 */
	ljmpl	$__KERNEL_CS, $pa_startup_64
<<<<<<< HEAD
=======
SYM_CODE_END(startup_32)
>>>>>>> upstream/android-13

	.section ".text64","ax"
	.code64
	.balign 4
<<<<<<< HEAD
ENTRY(startup_64)
	# Now jump into the kernel using virtual addresses
	jmpq	*tr_start(%rip)
=======
SYM_CODE_START(startup_64)
	# Now jump into the kernel using virtual addresses
	jmpq	*tr_start(%rip)
SYM_CODE_END(startup_64)
>>>>>>> upstream/android-13

	.section ".rodata","a"
	# Duplicate the global descriptor table
	# so the kernel can live anywhere
	.balign	16
<<<<<<< HEAD
	.globl tr_gdt
tr_gdt:
=======
SYM_DATA_START(tr_gdt)
>>>>>>> upstream/android-13
	.short	tr_gdt_end - tr_gdt - 1	# gdt limit
	.long	pa_tr_gdt
	.short	0
	.quad	0x00cf9b000000ffff	# __KERNEL32_CS
	.quad	0x00af9b000000ffff	# __KERNEL_CS
	.quad	0x00cf93000000ffff	# __KERNEL_DS
<<<<<<< HEAD
tr_gdt_end:

	.bss
	.balign	PAGE_SIZE
GLOBAL(trampoline_pgd)		.space	PAGE_SIZE

	.balign	8
GLOBAL(trampoline_header)
	tr_start:		.space	8
	GLOBAL(tr_efer)		.space	8
	GLOBAL(tr_cr4)		.space	4
	GLOBAL(tr_flags)	.space	4
END(trampoline_header)
=======
SYM_DATA_END_LABEL(tr_gdt, SYM_L_LOCAL, tr_gdt_end)

	.bss
	.balign	PAGE_SIZE
SYM_DATA(trampoline_pgd, .space PAGE_SIZE)

	.balign	8
SYM_DATA_START(trampoline_header)
	SYM_DATA_LOCAL(tr_start,	.space 8)
	SYM_DATA(tr_efer,		.space 8)
	SYM_DATA(tr_cr4,		.space 4)
	SYM_DATA(tr_flags,		.space 4)
SYM_DATA_END(trampoline_header)
>>>>>>> upstream/android-13

#include "trampoline_common.S"
