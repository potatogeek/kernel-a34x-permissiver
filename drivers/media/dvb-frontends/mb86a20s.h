<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *   Fujitsu mb86a20s driver
 *
 *   Copyright (C) 2010 Mauro Carvalho Chehab
<<<<<<< HEAD
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef MB86A20S_H
#define MB86A20S_H

#include <linux/dvb/frontend.h>

/**
 * struct mb86a20s_config - Define the per-device attributes of the frontend
 *
 * @fclk:		Clock frequency. If zero, assumes the default
 *			(32.57142 Mhz)
 * @demod_address:	the demodulator's i2c address
 * @is_serial:		if true, TS is serial. Otherwise, TS is parallel
 */
struct mb86a20s_config {
	u32	fclk;
	u8	demod_address;
	bool	is_serial;
};

#if IS_REACHABLE(CONFIG_DVB_MB86A20S)
/**
<<<<<<< HEAD
 * Attach a mb86a20s demod
=======
 * mb86a20s_attach - Attach a mb86a20s demod
>>>>>>> upstream/android-13
 *
 * @config: pointer to &struct mb86a20s_config with demod configuration.
 * @i2c: i2c adapter to use.
 *
 * return: FE pointer on success, NULL on failure.
 */
extern struct dvb_frontend *mb86a20s_attach(const struct mb86a20s_config *config,
					   struct i2c_adapter *i2c);

#else
static inline struct dvb_frontend *mb86a20s_attach(
	const struct mb86a20s_config *config, struct i2c_adapter *i2c)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif

#endif /* MB86A20S */
