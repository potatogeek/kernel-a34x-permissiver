<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (C) ST-Ericsson SA 2012
 *
 * Author: Ola Lilja <ola.o.lilja@stericsson.com>
 *         for ST-Ericsson.
 *
 * License terms:
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef MOP500_AB8500_H
#define MOP500_AB8500_H

extern struct snd_soc_ops mop500_ab8500_ops[];

<<<<<<< HEAD
int mop500_ab8500_machine_init(struct snd_soc_pcm_runtime *runtime);
=======
int mop500_ab8500_machine_init(struct snd_soc_pcm_runtime *rtd);
>>>>>>> upstream/android-13
void mop500_ab8500_remove(struct snd_soc_card *card);

#endif
