<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Driver for Infineon tua6100 PLL.
 *
 * (c) 2006 Andrew de Quincey
 *
 * Based on code found in budget-av.c, which has the following:
 * Compiled from various sources by Michael Hunold <michael@mihu.de>
 *
 * CI interface support (c) 2004 Olivier Gournet <ogournet@anevia.com> &
 *                               Andrew de Quincey <adq_dvb@lidskialf.net>
 *
 * Copyright (C) 2002 Ralph Metzler <rjkm@metzlerbros.de>
 *
 * Copyright (C) 1999-2002 Ralph  Metzler
 *                       & Marcus Metzler for convergence integrated media GmbH
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef __DVB_TUA6100_H__
#define __DVB_TUA6100_H__

#include <linux/i2c.h>
#include <media/dvb_frontend.h>

#if IS_REACHABLE(CONFIG_DVB_TUA6100)
extern struct dvb_frontend *tua6100_attach(struct dvb_frontend *fe, int addr, struct i2c_adapter *i2c);
#else
static inline struct dvb_frontend* tua6100_attach(struct dvb_frontend *fe, int addr, struct i2c_adapter *i2c)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif // CONFIG_DVB_TUA6100

#endif
