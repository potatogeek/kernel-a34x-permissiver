<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Driver for Texas Instruments INA219, INA226 power monitor chips
 *
 * Copyright (C) 2012 Lothar Felten <lothar.felten@gmail.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * For further information, see the Documentation/hwmon/ina2xx file.
=======
 * For further information, see the Documentation/hwmon/ina2xx.rst file.
>>>>>>> upstream/android-13
 */

/**
 * struct ina2xx_platform_data - ina2xx info
 * @shunt_uohms		shunt resistance in microohms
 */
struct ina2xx_platform_data {
	long shunt_uohms;
};
