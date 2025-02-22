<<<<<<< HEAD
/*
 * Copyright (C) 2012, Anish Kumar <anish198519851985@gmail.com>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012, Anish Kumar <anish198519851985@gmail.com>
>>>>>>> upstream/android-13
 */

#ifndef GENERIC_ADC_BATTERY_H
#define GENERIC_ADC_BATTERY_H

/**
 * struct gab_platform_data - platform_data for generic adc iio battery driver.
 * @battery_info:         recommended structure to specify static power supply
 *			   parameters
 * @cal_charge:           calculate charge level.
<<<<<<< HEAD
 * @gpio_charge_finished: gpio for the charger.
 * @gpio_inverted:        Should be 1 if the GPIO is active low otherwise 0
=======
>>>>>>> upstream/android-13
 * @jitter_delay:         delay required after the interrupt to check battery
 *			  status.Default set is 10ms.
 */
struct gab_platform_data {
	struct power_supply_info battery_info;
	int	(*cal_charge)(long value);
<<<<<<< HEAD
	int	gpio_charge_finished;
	bool	gpio_inverted;
=======
>>>>>>> upstream/android-13
	int     jitter_delay;
};

#endif /* GENERIC_ADC_BATTERY_H */
