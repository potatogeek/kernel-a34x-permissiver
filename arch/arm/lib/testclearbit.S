<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  linux/arch/arm/lib/testclearbit.S
 *
 *  Copyright (C) 1995-1996 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
#include "bitops.h"
                .text

testop	_test_and_clear_bit, bicne, strne
