<<<<<<< HEAD
/*
 * Copyright (C) 2017 Chen Liqin <liqin.chen@sunplusct.com>
 * Copyright (C) 2012 Regents of the University of California
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2017 Chen Liqin <liqin.chen@sunplusct.com>
 * Copyright (C) 2012 Regents of the University of California
>>>>>>> upstream/android-13
 */

#ifndef _ASM_RISCV_CACHE_H
#define _ASM_RISCV_CACHE_H

#define L1_CACHE_SHIFT		6

#define L1_CACHE_BYTES		(1 << L1_CACHE_SHIFT)

<<<<<<< HEAD
=======
/*
 * RISC-V requires the stack pointer to be 16-byte aligned, so ensure that
 * the flat loader aligns it accordingly.
 */
#ifndef CONFIG_MMU
#define ARCH_SLAB_MINALIGN	16
#endif

>>>>>>> upstream/android-13
#endif /* _ASM_RISCV_CACHE_H */
