<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * include/linux/mfd/wm831x/status.h -- Status LEDs for WM831x
 *
 * Copyright 2009 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef __MFD_WM831X_STATUS_H__
#define __MFD_WM831X_STATUS_H__

#define WM831X_LED_SRC_MASK                    0xC000  /* LED_SRC - [15:14] */
#define WM831X_LED_SRC_SHIFT                       14  /* LED_SRC - [15:14] */
#define WM831X_LED_SRC_WIDTH                        2  /* LED_SRC - [15:14] */
#define WM831X_LED_MODE_MASK                   0x0300  /* LED_MODE - [9:8] */
#define WM831X_LED_MODE_SHIFT                       8  /* LED_MODE - [9:8] */
#define WM831X_LED_MODE_WIDTH                       2  /* LED_MODE - [9:8] */
#define WM831X_LED_SEQ_LEN_MASK                0x0030  /* LED_SEQ_LEN - [5:4] */
#define WM831X_LED_SEQ_LEN_SHIFT                    4  /* LED_SEQ_LEN - [5:4] */
#define WM831X_LED_SEQ_LEN_WIDTH                    2  /* LED_SEQ_LEN - [5:4] */
#define WM831X_LED_DUR_MASK                    0x000C  /* LED_DUR - [3:2] */
#define WM831X_LED_DUR_SHIFT                        2  /* LED_DUR - [3:2] */
#define WM831X_LED_DUR_WIDTH                        2  /* LED_DUR - [3:2] */
#define WM831X_LED_DUTY_CYC_MASK               0x0003  /* LED_DUTY_CYC - [1:0] */
#define WM831X_LED_DUTY_CYC_SHIFT                   0  /* LED_DUTY_CYC - [1:0] */
#define WM831X_LED_DUTY_CYC_WIDTH                   2  /* LED_DUTY_CYC - [1:0] */

#endif
