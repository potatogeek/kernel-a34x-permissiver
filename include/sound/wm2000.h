<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * linux/sound/wm2000.h -- Platform data for WM2000
 *
 * Copyright 2010 Wolfson Microelectronics. PLC.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_SND_WM2000_H
#define __LINUX_SND_WM2000_H

struct wm2000_platform_data {
	/** Filename for system-specific image to download to device. */
	const char *download_file;

	/** Disable speech clarity enhancement, for use when an
	 * external algorithm is used. */
	unsigned int speech_enh_disable:1;
};

#endif
