<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  FM Driver for Connectivity chip of Texas Instruments.
 *  FM TX module header.
 *
 *  Copyright (C) 2011 Texas Instruments
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef _FMDRV_TX_H
#define _FMDRV_TX_H

int fm_tx_set_freq(struct fmdev *, u32);
int fm_tx_set_pwr_lvl(struct fmdev *, u8);
int fm_tx_set_region(struct fmdev *, u8);
int fm_tx_set_mute_mode(struct fmdev *, u8);
int fm_tx_set_stereo_mono(struct fmdev *, u16);
int fm_tx_set_rds_mode(struct fmdev *, u8);
int fm_tx_set_radio_text(struct fmdev *, u8 *, u8);
int fm_tx_set_af(struct fmdev *, u32);
int fm_tx_set_preemph_filter(struct fmdev *, u32);
int fm_tx_get_tune_cap_val(struct fmdev *);

#endif

