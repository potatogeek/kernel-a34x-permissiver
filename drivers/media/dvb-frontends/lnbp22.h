<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * lnbp22.h - driver for lnb supply and control ic lnbp22
 *
 * Copyright (C) 2006 Dominik Kuhlen
 * Based on lnbp21.h
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

#ifndef _LNBP22_H
#define _LNBP22_H

/* Enable */
#define LNBP22_EN	  0x10
/* Voltage selection */
#define LNBP22_VSEL	0x02
/* Plus 1 Volt Bit */
#define LNBP22_LLC	0x01

#include <linux/dvb/frontend.h>

#if IS_REACHABLE(CONFIG_DVB_LNBP22)
/*
 * override_set and override_clear control which system register bits (above)
 * to always set & clear
 */
extern struct dvb_frontend *lnbp22_attach(struct dvb_frontend *fe,
						struct i2c_adapter *i2c);
#else
static inline struct dvb_frontend *lnbp22_attach(struct dvb_frontend *fe,
						struct i2c_adapter *i2c)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif /* CONFIG_DVB_LNBP22 */

#endif /* _LNBP22_H */
