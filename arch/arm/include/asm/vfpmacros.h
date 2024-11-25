/* SPDX-License-Identifier: GPL-2.0 */
/*
 * arch/arm/include/asm/vfpmacros.h
 *
 * Assembler-only file containing VFP macros and register definitions.
 */
#include <asm/hwcap.h>

#include <asm/vfp.h>

<<<<<<< HEAD
@ Macros to allow building with old toolkits (with no VFP support)
=======
#ifdef CONFIG_AS_VFP_VMRS_FPINST
	.macro	VFPFMRX, rd, sysreg, cond
	vmrs\cond	\rd, \sysreg
	.endm

	.macro	VFPFMXR, sysreg, rd, cond
	vmsr\cond	\sysreg, \rd
	.endm
#else
	@ Macros to allow building with old toolkits (with no VFP support)
>>>>>>> upstream/android-13
	.macro	VFPFMRX, rd, sysreg, cond
	MRC\cond	p10, 7, \rd, \sysreg, cr0, 0	@ FMRX	\rd, \sysreg
	.endm

	.macro	VFPFMXR, sysreg, rd, cond
	MCR\cond	p10, 7, \rd, \sysreg, cr0, 0	@ FMXR	\sysreg, \rd
	.endm
<<<<<<< HEAD

	@ read all the working registers back into the VFP
	.macro	VFPFLDMIA, base, tmp
#if __LINUX_ARM_ARCH__ < 6
	LDC	p11, cr0, [\base],#33*4		    @ FLDMIAX \base!, {d0-d15}
#else
	LDC	p11, cr0, [\base],#32*4		    @ FLDMIAD \base!, {d0-d15}
#endif
#ifdef CONFIG_VFPv3
=======
#endif

	@ read all the working registers back into the VFP
	.macro	VFPFLDMIA, base, tmp
	.fpu	vfpv2
#if __LINUX_ARM_ARCH__ < 6
	fldmiax	\base!, {d0-d15}
#else
	vldmia	\base!, {d0-d15}
#endif
#ifdef CONFIG_VFPv3
	.fpu	vfpv3
>>>>>>> upstream/android-13
#if __LINUX_ARM_ARCH__ <= 6
	ldr	\tmp, =elf_hwcap		    @ may not have MVFR regs
	ldr	\tmp, [\tmp, #0]
	tst	\tmp, #HWCAP_VFPD32
<<<<<<< HEAD
	ldclne	p11, cr0, [\base],#32*4		    @ FLDMIAD \base!, {d16-d31}
=======
	vldmiane \base!, {d16-d31}
>>>>>>> upstream/android-13
	addeq	\base, \base, #32*4		    @ step over unused register space
#else
	VFPFMRX	\tmp, MVFR0			    @ Media and VFP Feature Register 0
	and	\tmp, \tmp, #MVFR0_A_SIMD_MASK	    @ A_SIMD field
	cmp	\tmp, #2			    @ 32 x 64bit registers?
<<<<<<< HEAD
	ldcleq	p11, cr0, [\base],#32*4		    @ FLDMIAD \base!, {d16-d31}
=======
	vldmiaeq \base!, {d16-d31}
>>>>>>> upstream/android-13
	addne	\base, \base, #32*4		    @ step over unused register space
#endif
#endif
	.endm

	@ write all the working registers out of the VFP
	.macro	VFPFSTMIA, base, tmp
#if __LINUX_ARM_ARCH__ < 6
<<<<<<< HEAD
	STC	p11, cr0, [\base],#33*4		    @ FSTMIAX \base!, {d0-d15}
#else
	STC	p11, cr0, [\base],#32*4		    @ FSTMIAD \base!, {d0-d15}
#endif
#ifdef CONFIG_VFPv3
=======
	fstmiax	\base!, {d0-d15}
#else
	vstmia	\base!, {d0-d15}
#endif
#ifdef CONFIG_VFPv3
	.fpu	vfpv3
>>>>>>> upstream/android-13
#if __LINUX_ARM_ARCH__ <= 6
	ldr	\tmp, =elf_hwcap		    @ may not have MVFR regs
	ldr	\tmp, [\tmp, #0]
	tst	\tmp, #HWCAP_VFPD32
<<<<<<< HEAD
	stclne	p11, cr0, [\base],#32*4		    @ FSTMIAD \base!, {d16-d31}
=======
	vstmiane \base!, {d16-d31}
>>>>>>> upstream/android-13
	addeq	\base, \base, #32*4		    @ step over unused register space
#else
	VFPFMRX	\tmp, MVFR0			    @ Media and VFP Feature Register 0
	and	\tmp, \tmp, #MVFR0_A_SIMD_MASK	    @ A_SIMD field
	cmp	\tmp, #2			    @ 32 x 64bit registers?
<<<<<<< HEAD
	stcleq	p11, cr0, [\base],#32*4		    @ FSTMIAD \base!, {d16-d31}
=======
	vstmiaeq \base!, {d16-d31}
>>>>>>> upstream/android-13
	addne	\base, \base, #32*4		    @ step over unused register space
#endif
#endif
	.endm
