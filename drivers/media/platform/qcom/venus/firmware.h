<<<<<<< HEAD
/*
 * Copyright (C) 2017 Linaro Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2017 Linaro Ltd.
>>>>>>> upstream/android-13
 */
#ifndef __VENUS_FIRMWARE_H__
#define __VENUS_FIRMWARE_H__

struct device;

<<<<<<< HEAD
int venus_boot(struct device *dev, const char *fwname);
int venus_shutdown(struct device *dev);
=======
int venus_firmware_init(struct venus_core *core);
void venus_firmware_deinit(struct venus_core *core);
int venus_boot(struct venus_core *core);
int venus_shutdown(struct venus_core *core);
int venus_set_hw_state(struct venus_core *core, bool suspend);

static inline int venus_set_hw_state_suspend(struct venus_core *core)
{
	return venus_set_hw_state(core, false);
}

static inline int venus_set_hw_state_resume(struct venus_core *core)
{
	return venus_set_hw_state(core, true);
}
>>>>>>> upstream/android-13

#endif
