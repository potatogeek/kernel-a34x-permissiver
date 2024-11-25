<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*

 * Copyright (C) 2015 Thomas Meyer (thomas@m3y3r.de)
 * Copyright (C) 2005 Jeff Dike (jdike@karaya.com)
<<<<<<< HEAD
 * Licensed under the GPL
=======
>>>>>>> upstream/android-13
 */

#ifndef __STUB_DATA_H
#define __STUB_DATA_H

struct stub_data {
	unsigned long offset;
	int fd;
<<<<<<< HEAD
	long err;
=======
	long parent_err, child_err;
>>>>>>> upstream/android-13
};

#endif
