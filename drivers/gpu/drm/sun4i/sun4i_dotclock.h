<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2015 Free Electrons
 * Copyright (C) 2015 NextThing Co
 *
 * Maxime Ripard <maxime.ripard@free-electrons.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _SUN4I_DOTCLOCK_H_
#define _SUN4I_DOTCLOCK_H_

struct sun4i_tcon;

int sun4i_dclk_create(struct device *dev, struct sun4i_tcon *tcon);
int sun4i_dclk_free(struct sun4i_tcon *tcon);

#endif /* _SUN4I_DOTCLOCK_H_ */
