<<<<<<< HEAD
/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
>>>>>>> upstream/android-13
 */

#ifndef _ASM_ARC_SWITCH_TO_H
#define _ASM_ARC_SWITCH_TO_H

#ifndef __ASSEMBLY__

#include <linux/sched.h>
<<<<<<< HEAD

#ifdef CONFIG_ARC_FPU_SAVE_RESTORE

extern void fpu_save_restore(struct task_struct *p, struct task_struct *n);
#define ARC_FPU_PREV(p, n)	fpu_save_restore(p, n)
#define ARC_FPU_NEXT(t)

#else

#define ARC_FPU_PREV(p, n)
#define ARC_FPU_NEXT(n)

#endif /* !CONFIG_ARC_FPU_SAVE_RESTORE */

#ifdef CONFIG_ARC_PLAT_EZNPS
extern void dp_save_restore(struct task_struct *p, struct task_struct *n);
#define ARC_EZNPS_DP_PREV(p, n)      dp_save_restore(p, n)
#else
#define ARC_EZNPS_DP_PREV(p, n)

#endif /* !CONFIG_ARC_PLAT_EZNPS */
=======
#include <asm/dsp-impl.h>
#include <asm/fpu.h>
>>>>>>> upstream/android-13

struct task_struct *__switch_to(struct task_struct *p, struct task_struct *n);

#define switch_to(prev, next, last)	\
do {					\
<<<<<<< HEAD
	ARC_EZNPS_DP_PREV(prev, next);	\
	ARC_FPU_PREV(prev, next);	\
	last = __switch_to(prev, next);\
	ARC_FPU_NEXT(next);		\
=======
	dsp_save_restore(prev, next);	\
	fpu_save_restore(prev, next);	\
	last = __switch_to(prev, next);\
>>>>>>> upstream/android-13
	mb();				\
} while (0)

#endif

#endif
