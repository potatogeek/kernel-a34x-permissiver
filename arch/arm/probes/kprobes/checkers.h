<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * arch/arm/probes/kprobes/checkers.h
 *
 * Copyright (C) 2014 Huawei Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
=======
>>>>>>> upstream/android-13
 */
#ifndef _ARM_KERNEL_PROBES_CHECKERS_H
#define _ARM_KERNEL_PROBES_CHECKERS_H

#include <linux/kernel.h>
#include <linux/types.h>
#include "../decode.h"

extern probes_check_t checker_stack_use_none;
extern probes_check_t checker_stack_use_unknown;
#ifdef CONFIG_THUMB2_KERNEL
extern probes_check_t checker_stack_use_imm_0xx;
#else
extern probes_check_t checker_stack_use_imm_x0x;
#endif
extern probes_check_t checker_stack_use_imm_xxx;
extern probes_check_t checker_stack_use_stmdx;

enum {
	STACK_USE_NONE,
	STACK_USE_UNKNOWN,
#ifdef CONFIG_THUMB2_KERNEL
	STACK_USE_FIXED_0XX,
	STACK_USE_T32STRD,
#else
	STACK_USE_FIXED_X0X,
#endif
	STACK_USE_FIXED_XXX,
	STACK_USE_STMDX,
	NUM_STACK_USE_TYPES
};

extern const union decode_action stack_check_actions[];

#ifndef CONFIG_THUMB2_KERNEL
extern const struct decode_checker arm_stack_checker[];
extern const struct decode_checker arm_regs_checker[];
#else
#endif
extern const struct decode_checker t32_stack_checker[];
extern const struct decode_checker t16_stack_checker[];
#endif
