<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * uda134x.h  --  UDA134x ALSA SoC Codec driver
 *
 * Copyright 2007 Dension Audio Systems Ltd.
 * Author: Zoltan Devai
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _UDA134X_H
#define _UDA134X_H

#include <sound/l3.h>

struct uda134x_platform_data {
	struct l3_pins l3;
	void (*power) (int);
	int model;
#define UDA134X_UDA1340 1
#define UDA134X_UDA1341 2
#define UDA134X_UDA1344 3
#define UDA134X_UDA1345 4
};

#endif /* _UDA134X_H */
