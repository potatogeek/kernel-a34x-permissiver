<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * isl6421.h - driver for lnb supply and control ic ISL6421
 *
 * Copyright (C) 2006 Andrew de Quincey
 * Copyright (C) 2006 Oliver Endriss
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * To obtain the license, point your browser to
 * http://www.gnu.org/copyleft/gpl.html
 *
 *
=======
>>>>>>> upstream/android-13
 * the project's page is at https://linuxtv.org
 */

#ifndef _ISL6421_H
#define _ISL6421_H

#include <linux/dvb/frontend.h>

/* system register bits */
#define ISL6421_OLF1	0x01
#define ISL6421_EN1	0x02
#define ISL6421_VSEL1	0x04
#define ISL6421_LLC1	0x08
#define ISL6421_ENT1	0x10
#define ISL6421_ISEL1	0x20
#define ISL6421_DCL	0x40

#if IS_REACHABLE(CONFIG_DVB_ISL6421)
/* override_set and override_clear control which system register bits (above) to always set & clear */
extern struct dvb_frontend *isl6421_attach(struct dvb_frontend *fe, struct i2c_adapter *i2c, u8 i2c_addr,
			  u8 override_set, u8 override_clear, bool override_tone);
#else
static inline struct dvb_frontend *isl6421_attach(struct dvb_frontend *fe, struct i2c_adapter *i2c, u8 i2c_addr,
						  u8 override_set, u8 override_clear, bool override_tone)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif // CONFIG_DVB_ISL6421

#endif
