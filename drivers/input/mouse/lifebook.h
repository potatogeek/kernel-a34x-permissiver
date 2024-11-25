<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Fujitsu B-series Lifebook PS/2 TouchScreen driver
 *
 * Copyright (c) 2005 Vojtech Pavlik
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _LIFEBOOK_H
#define _LIFEBOOK_H

<<<<<<< HEAD
#ifdef CONFIG_MOUSE_PS2_LIFEBOOK
void lifebook_module_init(void);
int lifebook_detect(struct psmouse *psmouse, bool set_properties);
int lifebook_init(struct psmouse *psmouse);
=======
int lifebook_detect(struct psmouse *psmouse, bool set_properties);
int lifebook_init(struct psmouse *psmouse);

#ifdef CONFIG_MOUSE_PS2_LIFEBOOK
void lifebook_module_init(void);
>>>>>>> upstream/android-13
#else
static inline void lifebook_module_init(void)
{
}
<<<<<<< HEAD
static inline int lifebook_detect(struct psmouse *psmouse, bool set_properties)
{
	return -ENOSYS;
}
static inline int lifebook_init(struct psmouse *psmouse)
{
	return -ENOSYS;
}
=======
>>>>>>> upstream/android-13
#endif

#endif
