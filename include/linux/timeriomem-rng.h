<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * linux/include/linux/timeriomem-rng.h
 *
 * Copyright (c) 2009 Alexander Clouter <alex@digriz.org.uk>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

=======
 */

#ifndef _LINUX_TIMERIOMEM_RNG_H
#define _LINUX_TIMERIOMEM_RNG_H

>>>>>>> upstream/android-13
struct timeriomem_rng_data {
	void __iomem		*address;

	/* measures in usecs */
	unsigned int		period;

	/* bits of entropy per 1024 bits read */
	unsigned int		quality;
};
<<<<<<< HEAD
=======

#endif /* _LINUX_TIMERIOMEM_RNG_H */
>>>>>>> upstream/android-13
