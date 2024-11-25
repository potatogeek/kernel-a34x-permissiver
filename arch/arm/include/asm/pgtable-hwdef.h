<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  arch/arm/include/asm/pgtable-hwdef.h
 *
 *  Copyright (C) 1995-2002 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef _ASMARM_PGTABLE_HWDEF_H
#define _ASMARM_PGTABLE_HWDEF_H

#ifdef CONFIG_ARM_LPAE
#include <asm/pgtable-3level-hwdef.h>
#else
#include <asm/pgtable-2level-hwdef.h>
#endif

#endif
