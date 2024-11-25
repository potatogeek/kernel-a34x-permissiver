<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Based on arm/arm64/include/asm/current.h
 *
 * Copyright (C) 2016 ARM
 * Copyright (C) 2017 SiFive
<<<<<<< HEAD
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 */


#ifndef __ASM_CURRENT_H
#define __ASM_CURRENT_H
=======
 */


#ifndef _ASM_RISCV_CURRENT_H
#define _ASM_RISCV_CURRENT_H
>>>>>>> upstream/android-13

#include <linux/bug.h>
#include <linux/compiler.h>

#ifndef __ASSEMBLY__

struct task_struct;

<<<<<<< HEAD
=======
register struct task_struct *riscv_current_is_tp __asm__("tp");

>>>>>>> upstream/android-13
/*
 * This only works because "struct thread_info" is at offset 0 from "struct
 * task_struct".  This constraint seems to be necessary on other architectures
 * as well, but __switch_to enforces it.  We can't check TASK_TI here because
 * <asm/asm-offsets.h> includes this, and I can't get the definition of "struct
 * task_struct" here due to some header ordering problems.
 */
static __always_inline struct task_struct *get_current(void)
{
<<<<<<< HEAD
	register struct task_struct *tp __asm__("tp");
	return tp;
=======
	return riscv_current_is_tp;
>>>>>>> upstream/android-13
}

#define current get_current()

#endif /* __ASSEMBLY__ */

<<<<<<< HEAD
#endif /* __ASM_CURRENT_H */
=======
#endif /* _ASM_RISCV_CURRENT_H */
>>>>>>> upstream/android-13
