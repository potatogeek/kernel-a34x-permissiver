<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * helene.h
 *
 * Sony HELENE DVB-S/S2/T/T2/C/C2/ISDB-T/S tuner driver (CXD2858ER)
 *
 * Copyright 2012 Sony Corporation
 * Copyright (C) 2014 NetUP Inc.
 * Copyright (C) 2014 Abylay Ospan <aospan@netup.ru>
<<<<<<< HEAD
 *
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

#ifndef __DVB_HELENE_H__
#define __DVB_HELENE_H__

#include <linux/dvb/frontend.h>
#include <linux/i2c.h>

enum helene_xtal {
	SONY_HELENE_XTAL_16000, /* 16 MHz */
	SONY_HELENE_XTAL_20500, /* 20.5 MHz */
	SONY_HELENE_XTAL_24000, /* 24 MHz */
	SONY_HELENE_XTAL_41000 /* 41 MHz */
};

/**
 * struct helene_config - the configuration of 'Helene' tuner driver
 * @i2c_address:	I2C address of the tuner
 * @xtal_freq_mhz:	Oscillator frequency, MHz
 * @set_tuner_priv:	Callback function private context
 * @set_tuner_callback:	Callback function that notifies the parent driver
 *			which tuner is active now
 * @xtal: Cristal frequency as described by &enum helene_xtal
 * @fe: Frontend for which connects this tuner
 */
struct helene_config {
	u8	i2c_address;
	u8	xtal_freq_mhz;
	void	*set_tuner_priv;
	int	(*set_tuner_callback)(void *, int);
	enum helene_xtal xtal;

	struct dvb_frontend *fe;
};

#if IS_REACHABLE(CONFIG_DVB_HELENE)
/**
<<<<<<< HEAD
 * Attach a helene tuner (terrestrial and cable standards)
=======
 * helene_attach - Attach a helene tuner (terrestrial and cable standards)
>>>>>>> upstream/android-13
 *
 * @fe: frontend to be attached
 * @config: pointer to &struct helene_config with tuner configuration.
 * @i2c: i2c adapter to use.
 *
 * return: FE pointer on success, NULL on failure.
 */
extern struct dvb_frontend *helene_attach(struct dvb_frontend *fe,
					const struct helene_config *config,
					struct i2c_adapter *i2c);

/**
<<<<<<< HEAD
 * Attach a helene tuner (satellite standards)
=======
 * helene_attach_s - Attach a helene tuner (satellite standards)
>>>>>>> upstream/android-13
 *
 * @fe: frontend to be attached
 * @config: pointer to &struct helene_config with tuner configuration.
 * @i2c: i2c adapter to use.
 *
 * return: FE pointer on success, NULL on failure.
 */
extern struct dvb_frontend *helene_attach_s(struct dvb_frontend *fe,
					const struct helene_config *config,
					struct i2c_adapter *i2c);
#else
static inline struct dvb_frontend *helene_attach(struct dvb_frontend *fe,
					const struct helene_config *config,
					struct i2c_adapter *i2c)
{
	pr_warn("%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
static inline struct dvb_frontend *helene_attach_s(struct dvb_frontend *fe,
					const struct helene_config *config,
					struct i2c_adapter *i2c)
{
	pr_warn("%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif

#endif
