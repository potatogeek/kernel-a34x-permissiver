<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *  Driver for Conexant CX24113/CX24128 Tuner (Satellite)
 *
 *  Copyright (C) 2007-8 Patrick Boettcher <pb@linuxtv.org>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef CX24113_H
#define CX24113_H

struct dvb_frontend;

struct cx24113_config {
	u8 i2c_addr; /* 0x14 or 0x54 */

	u32 xtal_khz;
};

#if IS_REACHABLE(CONFIG_DVB_TUNER_CX24113)
extern struct dvb_frontend *cx24113_attach(struct dvb_frontend *,
	const struct cx24113_config *config, struct i2c_adapter *i2c);

extern void cx24113_agc_callback(struct dvb_frontend *fe);
#else
static inline struct dvb_frontend *cx24113_attach(struct dvb_frontend *fe,
	const struct cx24113_config *config, struct i2c_adapter *i2c)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}

static inline void cx24113_agc_callback(struct dvb_frontend *fe)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
}
#endif

#endif /* CX24113_H */
