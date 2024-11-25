<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/* ----------------------------------------------------------------------------
 * touchkit_ps2.h  --  Driver for eGalax TouchKit PS/2 Touchscreens
 *
 * Copyright (C) 2005 by Stefan Lucke
 * Copyright (c) 2005 Vojtech Pavlik
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _TOUCHKIT_PS2_H
#define _TOUCHKIT_PS2_H

<<<<<<< HEAD
#ifdef CONFIG_MOUSE_PS2_TOUCHKIT
int touchkit_ps2_detect(struct psmouse *psmouse, bool set_properties);
#else
static inline int touchkit_ps2_detect(struct psmouse *psmouse,
				      bool set_properties)
{
	return -ENOSYS;
}
#endif /* CONFIG_MOUSE_PS2_TOUCHKIT */
=======
int touchkit_ps2_detect(struct psmouse *psmouse, bool set_properties);
>>>>>>> upstream/android-13

#endif
