<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Intel Low Power Subsystem PWM controller driver
 *
 * Copyright (C) 2014, Intel Corporation
 *
 * Derived from the original pwm-lpss.c
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __PWM_LPSS_H
#define __PWM_LPSS_H

#include <linux/device.h>
#include <linux/pwm.h>

<<<<<<< HEAD
struct pwm_lpss_chip;
=======
#define MAX_PWMS			4

struct pwm_lpss_chip {
	struct pwm_chip chip;
	void __iomem *regs;
	const struct pwm_lpss_boardinfo *info;
};
>>>>>>> upstream/android-13

struct pwm_lpss_boardinfo {
	unsigned long clk_rate;
	unsigned int npwm;
	unsigned long base_unit_bits;
	bool bypass;
<<<<<<< HEAD
=======
	/*
	 * On some devices the _PS0/_PS3 AML code of the GPU (GFX0) device
	 * messes with the PWM0 controllers state,
	 */
	bool other_devices_aml_touches_pwm_regs;
>>>>>>> upstream/android-13
};

struct pwm_lpss_chip *pwm_lpss_probe(struct device *dev, struct resource *r,
				     const struct pwm_lpss_boardinfo *info);
<<<<<<< HEAD
int pwm_lpss_remove(struct pwm_lpss_chip *lpwm);
int pwm_lpss_suspend(struct device *dev);
int pwm_lpss_resume(struct device *dev);
=======
>>>>>>> upstream/android-13

#endif	/* __PWM_LPSS_H */
