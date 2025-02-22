<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  FM Driver for Connectivity chip of Texas Instruments.
 *
 *  FM V4L2 module header.
 *
 *  Copyright (C) 2011 Texas Instruments
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef _FMDRV_V4L2_H
#define _FMDRV_V4L2_H

#include <media/v4l2-ioctl.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ctrls.h>

int fm_v4l2_init_video_device(struct fmdev *, int);
void *fm_v4l2_deinit_video_device(void);

#endif
