<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Fitipower FC0013 tuner driver
 *
 * Copyright (C) 2012 Hans-Frieder Vogt <hfvogt@gmx.net>
<<<<<<< HEAD
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef _FC0013_H_
#define _FC0013_H_

#include <media/dvb_frontend.h>
#include "fc001x-common.h"

#if IS_REACHABLE(CONFIG_MEDIA_TUNER_FC0013)
extern struct dvb_frontend *fc0013_attach(struct dvb_frontend *fe,
					struct i2c_adapter *i2c,
					u8 i2c_address, int dual_master,
					enum fc001x_xtal_freq xtal_freq);
extern int fc0013_rc_cal_add(struct dvb_frontend *fe, int rc_val);
extern int fc0013_rc_cal_reset(struct dvb_frontend *fe);
#else
static inline struct dvb_frontend *fc0013_attach(struct dvb_frontend *fe,
					struct i2c_adapter *i2c,
					u8 i2c_address, int dual_master,
					enum fc001x_xtal_freq xtal_freq)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}

static inline int fc0013_rc_cal_add(struct dvb_frontend *fe, int rc_val)
{
	return 0;
}

static inline int fc0013_rc_cal_reset(struct dvb_frontend *fe)
{
	return 0;
}
#endif

#endif
