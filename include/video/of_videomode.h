<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright 2012 Steffen Trumtrar <s.trumtrar@pengutronix.de>
 *
 * videomode of-helpers
<<<<<<< HEAD
 *
 * This file is released under the GPLv2
=======
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_OF_VIDEOMODE_H
#define __LINUX_OF_VIDEOMODE_H

struct device_node;
struct videomode;

int of_get_videomode(struct device_node *np, struct videomode *vm,
		     int index);

#endif /* __LINUX_OF_VIDEOMODE_H */
