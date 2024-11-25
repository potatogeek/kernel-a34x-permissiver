<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *  Driver for Dummy Frontend
 *
 *  Written by Emard <emard@softhome.net>
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

#ifndef DVB_DUMMY_FE_H
#define DVB_DUMMY_FE_H

#include <linux/dvb/frontend.h>
#include <media/dvb_frontend.h>

#if IS_REACHABLE(CONFIG_DVB_DUMMY_FE)
<<<<<<< HEAD
extern struct dvb_frontend* dvb_dummy_fe_ofdm_attach(void);
extern struct dvb_frontend* dvb_dummy_fe_qpsk_attach(void);
extern struct dvb_frontend* dvb_dummy_fe_qam_attach(void);
#else
static inline struct dvb_frontend *dvb_dummy_fe_ofdm_attach(void)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
=======
struct dvb_frontend *dvb_dummy_fe_ofdm_attach(void);
struct dvb_frontend *dvb_dummy_fe_qpsk_attach(void);
struct dvb_frontend *dvb_dummy_fe_qam_attach(void);
#else
static inline struct dvb_frontend *dvb_dummy_fe_ofdm_attach(void)
{
	pr_warn("%s: driver disabled by Kconfig\n", __func__);
>>>>>>> upstream/android-13
	return NULL;
}
static inline struct dvb_frontend *dvb_dummy_fe_qpsk_attach(void)
{
<<<<<<< HEAD
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
=======
	pr_warn("%s: driver disabled by Kconfig\n", __func__);
>>>>>>> upstream/android-13
	return NULL;
}
static inline struct dvb_frontend *dvb_dummy_fe_qam_attach(void)
{
<<<<<<< HEAD
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
=======
	pr_warn("%s: driver disabled by Kconfig\n", __func__);
>>>>>>> upstream/android-13
	return NULL;
}
#endif /* CONFIG_DVB_DUMMY_FE */

#endif // DVB_DUMMY_FE_H
