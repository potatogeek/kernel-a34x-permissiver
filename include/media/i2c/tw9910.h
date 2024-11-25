<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * tw9910 Driver header
 *
 * Copyright (C) 2008 Renesas Solutions Corp.
 * Kuninori Morimoto <morimoto.kuninori@renesas.com>
 *
 * Based on ov772x.h
 *
 * Copyright (C) Kuninori Morimoto <morimoto.kuninori@renesas.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __TW9910_H__
#define __TW9910_H__

<<<<<<< HEAD
#include <media/soc_camera.h>

/**
 * tw9910_mpout_pin - MPOUT (multi-purpose output) pin functions
 */
=======
/* MPOUT (multi-purpose output) pin functions */
>>>>>>> upstream/android-13
enum tw9910_mpout_pin {
	TW9910_MPO_VLOSS,
	TW9910_MPO_HLOCK,
	TW9910_MPO_SLOCK,
	TW9910_MPO_VLOCK,
	TW9910_MPO_MONO,
	TW9910_MPO_DET50,
	TW9910_MPO_FIELD,
	TW9910_MPO_RTCO,
};

/**
<<<<<<< HEAD
 * tw9910_video_info -	tw9910 driver interface structure
 * @buswidth:		Parallel data bus width (8 or 16).
 * @mpout:		Selected function of MPOUT (multi-purpose output) pin.
 *			See &enum tw9910_mpout_pin
=======
 * struct tw9910_video_info - tw9910 driver interface structure
 * @buswidth:		Parallel data bus width (8 or 16).
 * @mpout:		Selected function of MPOUT (multi-purpose output) pin.
 *			See enum tw9910_mpout_pin
>>>>>>> upstream/android-13
 */
struct tw9910_video_info {
	unsigned long		buswidth;
	enum tw9910_mpout_pin	mpout;
};


#endif /* __TW9910_H__ */
