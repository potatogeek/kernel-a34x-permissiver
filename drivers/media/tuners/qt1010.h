<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *  Driver for Quantek QT1010 silicon tuner
 *
 *  Copyright (C) 2006 Antti Palosaari <crope@iki.fi>
 *                     Aapo Tahkola <aet@rasterburn.org>
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
 *  GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef QT1010_H
#define QT1010_H

#include <media/dvb_frontend.h>

struct qt1010_config {
	u8 i2c_address;
};

/**
<<<<<<< HEAD
 * Attach a qt1010 tuner to the supplied frontend structure.
 *
 * @param fe   frontend to attach to
 * @param i2c  i2c adapter to use
 * @param cfg  tuner hw based configuration
=======
 * qt1010_attach() - Attach a qt1010 tuner to the supplied frontend structure
 *
 * @fe:   frontend to attach to
 * @i2c:  i2c adapter to use
 * @cfg:  tuner hw based configuration
>>>>>>> upstream/android-13
 * @return fe  pointer on success, NULL on failure
 */
#if IS_REACHABLE(CONFIG_MEDIA_TUNER_QT1010)
extern struct dvb_frontend *qt1010_attach(struct dvb_frontend *fe,
					  struct i2c_adapter *i2c,
					  struct qt1010_config *cfg);
#else
static inline struct dvb_frontend *qt1010_attach(struct dvb_frontend *fe,
						 struct i2c_adapter *i2c,
						 struct qt1010_config *cfg)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif // CONFIG_MEDIA_TUNER_QT1010

#endif
