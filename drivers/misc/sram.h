<<<<<<< HEAD
/*
 * Defines for the SRAM driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Defines for the SRAM driver
>>>>>>> upstream/android-13
 */
#ifndef __SRAM_H
#define __SRAM_H

<<<<<<< HEAD
=======
struct sram_config {
	int (*init)(void);
	bool map_only_reserved;
};

>>>>>>> upstream/android-13
struct sram_partition {
	void __iomem *base;

	struct gen_pool *pool;
	struct bin_attribute battr;
	struct mutex lock;
	struct list_head list;
};

struct sram_dev {
<<<<<<< HEAD
	struct device *dev;
	void __iomem *virt_base;
=======
	const struct sram_config *config;

	struct device *dev;
	void __iomem *virt_base;
	bool no_memory_wc;
>>>>>>> upstream/android-13

	struct gen_pool *pool;
	struct clk *clk;

	struct sram_partition *partition;
	u32 partitions;
};

struct sram_reserve {
	struct list_head list;
	u32 start;
	u32 size;
<<<<<<< HEAD
=======
	struct resource res;
>>>>>>> upstream/android-13
	bool export;
	bool pool;
	bool protect_exec;
	const char *label;
};

#ifdef CONFIG_SRAM_EXEC
int sram_check_protect_exec(struct sram_dev *sram, struct sram_reserve *block,
			    struct sram_partition *part);
int sram_add_protect_exec(struct sram_partition *part);
#else
static inline int sram_check_protect_exec(struct sram_dev *sram,
					  struct sram_reserve *block,
					  struct sram_partition *part)
{
	return -ENODEV;
}

static inline int sram_add_protect_exec(struct sram_partition *part)
{
	return -ENODEV;
}
#endif /* CONFIG_SRAM_EXEC */
#endif /* __SRAM_H */
