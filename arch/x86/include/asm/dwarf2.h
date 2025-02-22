/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_DWARF2_H
#define _ASM_X86_DWARF2_H

#ifndef __ASSEMBLY__
#warning "asm/dwarf2.h should be only included in pure assembly files"
#endif

<<<<<<< HEAD
/*
 * Macros for dwarf2 CFI unwind table entries.
 * See "as.info" for details on these pseudo ops. Unfortunately
 * they are only supported in very new binutils, so define them
 * away for older version.
 */

#ifdef CONFIG_AS_CFI

=======
>>>>>>> upstream/android-13
#define CFI_STARTPROC		.cfi_startproc
#define CFI_ENDPROC		.cfi_endproc
#define CFI_DEF_CFA		.cfi_def_cfa
#define CFI_DEF_CFA_REGISTER	.cfi_def_cfa_register
#define CFI_DEF_CFA_OFFSET	.cfi_def_cfa_offset
#define CFI_ADJUST_CFA_OFFSET	.cfi_adjust_cfa_offset
#define CFI_OFFSET		.cfi_offset
#define CFI_REL_OFFSET		.cfi_rel_offset
#define CFI_REGISTER		.cfi_register
#define CFI_RESTORE		.cfi_restore
#define CFI_REMEMBER_STATE	.cfi_remember_state
#define CFI_RESTORE_STATE	.cfi_restore_state
#define CFI_UNDEFINED		.cfi_undefined
#define CFI_ESCAPE		.cfi_escape

<<<<<<< HEAD
#ifdef CONFIG_AS_CFI_SIGNAL_FRAME
#define CFI_SIGNAL_FRAME	.cfi_signal_frame
#else
#define CFI_SIGNAL_FRAME
#endif

#if defined(CONFIG_AS_CFI_SECTIONS) && defined(__ASSEMBLY__)
=======
>>>>>>> upstream/android-13
#ifndef BUILD_VDSO
	/*
	 * Emit CFI data in .debug_frame sections, not .eh_frame sections.
	 * The latter we currently just discard since we don't do DWARF
	 * unwinding at runtime.  So only the offline DWARF information is
<<<<<<< HEAD
	 * useful to anyone.  Note we should not use this directive if
	 * vmlinux.lds.S gets changed so it doesn't discard .eh_frame.
=======
	 * useful to anyone.  Note we should not use this directive if we
	 * ever decide to enable DWARF unwinding at runtime.
>>>>>>> upstream/android-13
	 */
	.cfi_sections .debug_frame
#else
	 /*
	  * For the vDSO, emit both runtime unwind information and debug
	  * symbols for the .dbg file.
	  */
	.cfi_sections .eh_frame, .debug_frame
#endif
<<<<<<< HEAD
#endif

#else

/*
 * Due to the structure of pre-exisiting code, don't use assembler line
 * comment character # to ignore the arguments. Instead, use a dummy macro.
 */
.macro cfi_ignore a=0, b=0, c=0, d=0
.endm

#define CFI_STARTPROC		cfi_ignore
#define CFI_ENDPROC		cfi_ignore
#define CFI_DEF_CFA		cfi_ignore
#define CFI_DEF_CFA_REGISTER	cfi_ignore
#define CFI_DEF_CFA_OFFSET	cfi_ignore
#define CFI_ADJUST_CFA_OFFSET	cfi_ignore
#define CFI_OFFSET		cfi_ignore
#define CFI_REL_OFFSET		cfi_ignore
#define CFI_REGISTER		cfi_ignore
#define CFI_RESTORE		cfi_ignore
#define CFI_REMEMBER_STATE	cfi_ignore
#define CFI_RESTORE_STATE	cfi_ignore
#define CFI_UNDEFINED		cfi_ignore
#define CFI_ESCAPE		cfi_ignore
#define CFI_SIGNAL_FRAME	cfi_ignore

#endif
=======
>>>>>>> upstream/android-13

#endif /* _ASM_X86_DWARF2_H */
