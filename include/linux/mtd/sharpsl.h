<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * SharpSL NAND support
 *
 * Copyright (C) 2008 Dmitry Baryshkov
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/mtd/rawnand.h>
#include <linux/mtd/nand_ecc.h>
=======
 */

#ifndef _MTD_SHARPSL_H
#define _MTD_SHARPSL_H

#include <linux/mtd/rawnand.h>
>>>>>>> upstream/android-13
#include <linux/mtd/partitions.h>

struct sharpsl_nand_platform_data {
	struct nand_bbt_descr	*badblock_pattern;
	const struct mtd_ooblayout_ops *ecc_layout;
	struct mtd_partition	*partitions;
	unsigned int		nr_partitions;
	const char *const	*part_parsers;
};
<<<<<<< HEAD
=======

#endif /* _MTD_SHARPSL_H */
>>>>>>> upstream/android-13
