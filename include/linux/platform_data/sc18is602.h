<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Platform data for NXP SC18IS602/603
 *
 * Copyright (C) 2012 Guenter Roeck <linux@roeck-us.net>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * For further information, see the Documentation/spi/spi-sc18is602 file.
=======
 * For further information, see the Documentation/spi/spi-sc18is602.rst file.
>>>>>>> upstream/android-13
 */

/**
 * struct sc18is602_platform_data - sc18is602 info
 * @clock_frequency		SC18IS603 oscillator frequency
 */
struct sc18is602_platform_data {
	u32 clock_frequency;
};
