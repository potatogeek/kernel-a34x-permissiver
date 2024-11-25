<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * generic display timing functions
 *
 * Copyright (c) 2012 Steffen Trumtrar <s.trumtrar@pengutronix.de>, Pengutronix
<<<<<<< HEAD
 *
 * This file is released under the GPLv2
=======
>>>>>>> upstream/android-13
 */

#include <linux/export.h>
#include <linux/slab.h>
#include <video/display_timing.h>

void display_timings_release(struct display_timings *disp)
{
	if (disp->timings) {
		unsigned int i;

		for (i = 0; i < disp->num_timings; i++)
			kfree(disp->timings[i]);
		kfree(disp->timings);
	}
	kfree(disp);
}
EXPORT_SYMBOL_GPL(display_timings_release);
