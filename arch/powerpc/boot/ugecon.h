<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * arch/powerpc/boot/ugecon.h
 *
 * USB Gecko early bootwrapper console.
 * Copyright (C) 2008-2009 The GameCube Linux Team
 * Copyright (C) 2008,2009 Albert Herranz
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef __UGECON_H
#define __UGECON_H

extern void *ug_probe(void);

extern void ug_putc(char ch);
extern void ug_console_write(const char *buf, int len);

#endif /* __UGECON_H */

