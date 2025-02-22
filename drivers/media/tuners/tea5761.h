<<<<<<< HEAD
/*
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
>>>>>>> upstream/android-13
*/

#ifndef __TEA5761_H__
#define __TEA5761_H__

#include <linux/i2c.h>
#include <media/dvb_frontend.h>

#if IS_REACHABLE(CONFIG_MEDIA_TUNER_TEA5761)
extern int tea5761_autodetection(struct i2c_adapter* i2c_adap, u8 i2c_addr);

extern struct dvb_frontend *tea5761_attach(struct dvb_frontend *fe,
					   struct i2c_adapter* i2c_adap,
					   u8 i2c_addr);
#else
static inline int tea5761_autodetection(struct i2c_adapter* i2c_adap,
					u8 i2c_addr)
{
	printk(KERN_INFO "%s: not probed - driver disabled by Kconfig\n",
	       __func__);
	return -EINVAL;
}

static inline struct dvb_frontend *tea5761_attach(struct dvb_frontend *fe,
						   struct i2c_adapter* i2c_adap,
						   u8 i2c_addr)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif

#endif /* __TEA5761_H__ */
