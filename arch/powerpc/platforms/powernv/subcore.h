<<<<<<< HEAD
/*
 * Copyright 2013, Michael Ellerman, IBM Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright 2013, Michael Ellerman, IBM Corporation.
>>>>>>> upstream/android-13
 */

/* These are ordered and tested with <= */
#define SYNC_STEP_INITIAL	0
#define SYNC_STEP_UNSPLIT	1	/* Set by secondary when it sees unsplit */
#define SYNC_STEP_REAL_MODE	2	/* Set by secondary when in real mode  */
#define SYNC_STEP_FINISHED	3	/* Set by secondary when split/unsplit is done */

#ifndef __ASSEMBLY__

#ifdef CONFIG_SMP
void split_core_secondary_loop(u8 *state);
extern void update_subcore_sibling_mask(void);
#else
<<<<<<< HEAD
static inline void update_subcore_sibling_mask(void) { };
=======
static inline void update_subcore_sibling_mask(void) { }
>>>>>>> upstream/android-13
#endif /* CONFIG_SMP */

#endif /* __ASSEMBLY__ */
