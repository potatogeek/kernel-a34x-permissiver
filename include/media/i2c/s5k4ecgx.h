<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * S5K4ECGX image sensor header file
 *
 * Copyright (C) 2012, Linaro
 * Copyright (C) 2012, Samsung Electronics Co., Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef S5K4ECGX_H
#define S5K4ECGX_H

/**
 * struct s5k4ecgx_gpio - data structure describing a GPIO
<<<<<<< HEAD
 * @gpio : GPIO number
=======
 * @gpio: GPIO number
>>>>>>> upstream/android-13
 * @level: indicates active state of the @gpio
 */
struct s5k4ecgx_gpio {
	int gpio;
	int level;
};

/**
<<<<<<< HEAD
 * struct ss5k4ecgx_platform_data- s5k4ecgx driver platform data
 * @gpio_reset:	 GPIO driving RESET pin
 * @gpio_stby :	 GPIO driving STBY pin
=======
 * struct s5k4ecgx_platform_data - s5k4ecgx driver platform data
 * @gpio_reset:	 GPIO driving RESET pin
 * @gpio_stby:	 GPIO driving STBY pin
>>>>>>> upstream/android-13
 */

struct s5k4ecgx_platform_data {
	struct s5k4ecgx_gpio gpio_reset;
	struct s5k4ecgx_gpio gpio_stby;
};

#endif /* S5K4ECGX_H */
