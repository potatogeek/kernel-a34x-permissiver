<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  linux/arch/arm/lib/changebit.S
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

bitop	_change_bit, eor
