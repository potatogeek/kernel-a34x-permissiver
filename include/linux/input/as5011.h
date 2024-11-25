<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
#ifndef _AS5011_H
#define _AS5011_H

/*
 * Copyright (c) 2010, 2011 Fabien Marteau <fabien.marteau@armadeus.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

struct as5011_platform_data {
	unsigned int button_gpio;
	unsigned int axis_irq; /* irq number */
	unsigned long axis_irqflags;
	char xp, xn; /* threshold for x axis */
	char yp, yn; /* threshold for y axis */
};

#endif /* _AS5011_H */
