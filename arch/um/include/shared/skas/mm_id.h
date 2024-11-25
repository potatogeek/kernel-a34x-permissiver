<<<<<<< HEAD
/*
 * Copyright (C) 2005 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2005 Jeff Dike (jdike@karaya.com)
>>>>>>> upstream/android-13
 */

#ifndef __MM_ID_H
#define __MM_ID_H

struct mm_id {
	union {
		int mm_fd;
		int pid;
	} u;
	unsigned long stack;
<<<<<<< HEAD
=======
	int kill;
>>>>>>> upstream/android-13
};

#endif
