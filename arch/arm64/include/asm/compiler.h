<<<<<<< HEAD
/*
 * Based on arch/arm/include/asm/compiler.h
 *
 * Copyright (C) 2012 ARM Ltd.
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
 */
#ifndef __ASM_COMPILER_H
#define __ASM_COMPILER_H

/*
 * This is used to ensure the compiler did actually allocate the register we
 * asked it for some inline assembly sequences.  Apparently we can't trust the
 * compiler from one version to another so a bit of paranoia won't hurt.  This
 * string is meant to be concatenated with the inline asm string and will
 * cause compilation to stop on mismatch.  (for details, see gcc PR 15089)
 */
#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

#endif	/* __ASM_COMPILER_H */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_COMPILER_H
#define __ASM_COMPILER_H

#ifdef ARM64_ASM_ARCH
#define ARM64_ASM_PREAMBLE ".arch " ARM64_ASM_ARCH "\n"
#else
#define ARM64_ASM_PREAMBLE
#endif

/*
 * The EL0/EL1 pointer bits used by a pointer authentication code.
 * This is dependent on TBI0/TBI1 being enabled, or bits 63:56 would also apply.
 */
#define ptrauth_user_pac_mask()		GENMASK_ULL(54, vabits_actual)
#define ptrauth_kernel_pac_mask()	GENMASK_ULL(63, vabits_actual)

/* Valid for EL0 TTBR0 and EL1 TTBR1 instruction pointers */
#define ptrauth_clear_pac(ptr)						\
	((ptr & BIT_ULL(55)) ? (ptr | ptrauth_kernel_pac_mask()) :	\
			       (ptr & ~ptrauth_user_pac_mask()))

#define __builtin_return_address(val)					\
	(void *)(ptrauth_clear_pac((unsigned long)__builtin_return_address(val)))

#ifdef CONFIG_CFI_CLANG
/*
 * With CONFIG_CFI_CLANG, the compiler replaces function address
 * references with the address of the function's CFI jump table
 * entry. The function_nocfi macro always returns the address of the
 * actual function instead.
 */
#define function_nocfi(x) ({						\
	void *addr;							\
	asm("adrp %0, " __stringify(x) "\n\t"				\
	    "add  %0, %0, :lo12:" __stringify(x)			\
	    : "=r" (addr));						\
	addr;								\
})
#endif

#endif /* __ASM_COMPILER_H */
>>>>>>> upstream/android-13
