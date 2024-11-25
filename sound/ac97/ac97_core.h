<<<<<<< HEAD
/*
 * Copyright (C) 2016 Robert Jarzmik <robert.jarzmik@free.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

unsigned int snd_ac97_bus_scan_one(struct ac97_controller *ac97,
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2016 Robert Jarzmik <robert.jarzmik@free.fr>
 */

unsigned int snd_ac97_bus_scan_one(struct ac97_controller *adrv,
>>>>>>> upstream/android-13
				   unsigned int codec_num);

static inline bool ac97_ids_match(unsigned int id1, unsigned int id2,
				  unsigned int mask)
{
	return (id1 & mask) == (id2 & mask);
}
