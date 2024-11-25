<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Machine declaration for Alpine platforms.
 *
 * Copyright (C) 2015 Annapurna Labs Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/of_platform.h>

#include <asm/mach/arch.h>

static const char * const al_match[] __initconst = {
	"al,alpine",
	NULL,
};

DT_MACHINE_START(AL_DT, "Annapurna Labs Alpine")
	.dt_compat	= al_match,
MACHINE_END
