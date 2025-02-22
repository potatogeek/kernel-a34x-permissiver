<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * OpenFirmware helpers for memory drivers
 *
 * Copyright (C) 2012 Texas Instruments, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
 * Copyright (C) 2020 Krzysztof Kozlowski <krzk@kernel.org>
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_MEMORY_OF_REG_H
#define __LINUX_MEMORY_OF_REG_H

#if defined(CONFIG_OF) && defined(CONFIG_DDR)
<<<<<<< HEAD
extern const struct lpddr2_min_tck *of_get_min_tck(struct device_node *np,
		struct device *dev);
extern const struct lpddr2_timings
	*of_get_ddr_timings(struct device_node *np_ddr, struct device *dev,
	u32 device_type, u32 *nr_frequencies);
=======
const struct lpddr2_min_tck *of_get_min_tck(struct device_node *np,
					    struct device *dev);
const struct lpddr2_timings *of_get_ddr_timings(struct device_node *np_ddr,
						struct device *dev,
						u32 device_type, u32 *nr_frequencies);
const struct lpddr3_min_tck *of_lpddr3_get_min_tck(struct device_node *np,
						   struct device *dev);
const struct lpddr3_timings *
of_lpddr3_get_ddr_timings(struct device_node *np_ddr,
			  struct device *dev, u32 device_type, u32 *nr_frequencies);
>>>>>>> upstream/android-13
#else
static inline const struct lpddr2_min_tck
	*of_get_min_tck(struct device_node *np, struct device *dev)
{
	return NULL;
}

static inline const struct lpddr2_timings
	*of_get_ddr_timings(struct device_node *np_ddr, struct device *dev,
	u32 device_type, u32 *nr_frequencies)
{
	return NULL;
}
<<<<<<< HEAD
=======

static inline const struct lpddr3_min_tck
	*of_lpddr3_get_min_tck(struct device_node *np, struct device *dev)
{
	return NULL;
}

static inline const struct lpddr3_timings
	*of_lpddr3_get_ddr_timings(struct device_node *np_ddr,
	struct device *dev, u32 device_type, u32 *nr_frequencies)
{
	return NULL;
}
>>>>>>> upstream/android-13
#endif /* CONFIG_OF && CONFIG_DDR */

#endif /* __LINUX_MEMORY_OF_REG_ */
