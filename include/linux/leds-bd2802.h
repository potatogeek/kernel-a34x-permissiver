<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * leds-bd2802.h - RGB LED Driver
 *
 * Copyright (C) 2009 Samsung Electronics
 * Kim Kyuwon <q1.kim@samsung.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Datasheet: http://www.rohm.com/products/databook/driver/pdf/bd2802gu-e.pdf
 *
=======
 * Datasheet: http://www.rohm.com/products/databook/driver/pdf/bd2802gu-e.pdf
>>>>>>> upstream/android-13
 */
#ifndef _LEDS_BD2802_H_
#define _LEDS_BD2802_H_

struct bd2802_led_platform_data{
<<<<<<< HEAD
	int	reset_gpio;
=======
>>>>>>> upstream/android-13
	u8	rgb_time;
};

#define RGB_TIME(slopedown, slopeup, waveform) \
	((slopedown) << 6 | (slopeup) << 4 | (waveform))

#endif /* _LEDS_BD2802_H_ */

