<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * System Control Driver
 *
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 * Copyright (C) 2012 Linaro Ltd.
 *
 * Author: Dong Aisheng <dong.aisheng@linaro.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_MFD_SYSCON_H__
#define __LINUX_MFD_SYSCON_H__

#include <linux/err.h>
#include <linux/errno.h>

struct device_node;

#ifdef CONFIG_MFD_SYSCON
<<<<<<< HEAD
extern struct regmap *syscon_node_to_regmap(struct device_node *np);
extern struct regmap *syscon_regmap_lookup_by_compatible(const char *s);
extern struct regmap *syscon_regmap_lookup_by_pdevname(const char *s);
extern struct regmap *syscon_regmap_lookup_by_phandle(
					struct device_node *np,
					const char *property);
#else
=======
extern struct regmap *device_node_to_regmap(struct device_node *np);
extern struct regmap *syscon_node_to_regmap(struct device_node *np);
extern struct regmap *syscon_regmap_lookup_by_compatible(const char *s);
extern struct regmap *syscon_regmap_lookup_by_phandle(
					struct device_node *np,
					const char *property);
extern struct regmap *syscon_regmap_lookup_by_phandle_args(
					struct device_node *np,
					const char *property,
					int arg_count,
					unsigned int *out_args);
extern struct regmap *syscon_regmap_lookup_by_phandle_optional(
					struct device_node *np,
					const char *property);
#else
static inline struct regmap *device_node_to_regmap(struct device_node *np)
{
	return ERR_PTR(-ENOTSUPP);
}

>>>>>>> upstream/android-13
static inline struct regmap *syscon_node_to_regmap(struct device_node *np)
{
	return ERR_PTR(-ENOTSUPP);
}

static inline struct regmap *syscon_regmap_lookup_by_compatible(const char *s)
{
	return ERR_PTR(-ENOTSUPP);
}

<<<<<<< HEAD
static inline struct regmap *syscon_regmap_lookup_by_pdevname(const char *s)
{
	return ERR_PTR(-ENOTSUPP);
}

=======
>>>>>>> upstream/android-13
static inline struct regmap *syscon_regmap_lookup_by_phandle(
					struct device_node *np,
					const char *property)
{
	return ERR_PTR(-ENOTSUPP);
}
<<<<<<< HEAD
=======

static inline struct regmap *syscon_regmap_lookup_by_phandle_args(
					struct device_node *np,
					const char *property,
					int arg_count,
					unsigned int *out_args)
{
	return ERR_PTR(-ENOTSUPP);
}

static inline struct regmap *syscon_regmap_lookup_by_phandle_optional(
					struct device_node *np,
					const char *property)
{
	return NULL;
}

>>>>>>> upstream/android-13
#endif

#endif /* __LINUX_MFD_SYSCON_H__ */
