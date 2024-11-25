/* SPDX-License-Identifier: GPL-2.0 */
/*
 * PC-Speaker driver for Linux
 *
 * Copyright (C) 2001-2008  Stas Sergeev
 */

#ifndef __PCSP_INPUT_H__
#define __PCSP_INPUT_H__

int pcspkr_input_init(struct input_dev **rdev, struct device *dev);
<<<<<<< HEAD
int pcspkr_input_remove(struct input_dev *dev);
=======
>>>>>>> upstream/android-13
void pcspkr_stop_sound(void);

#endif
