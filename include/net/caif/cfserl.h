<<<<<<< HEAD
/*
 * Copyright (C) ST-Ericsson AB 2010
 * Author:	Sjur Brendeland
 * License terms: GNU General Public License (GPL) version 2
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) ST-Ericsson AB 2010
 * Author:	Sjur Brendeland
>>>>>>> upstream/android-13
 */

#ifndef CFSERL_H_
#define CFSERL_H_
#include <net/caif/caif_layer.h>

struct cflayer *cfserl_create(int instance, bool use_stx);
<<<<<<< HEAD
=======
void cfserl_release(struct cflayer *layer);
>>>>>>> upstream/android-13
#endif
