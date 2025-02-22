<<<<<<< HEAD
/*
 * Copyright (c) 2012, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2012, NVIDIA CORPORATION.  All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef __SOC_TEGRA_FUSE_H__
#define __SOC_TEGRA_FUSE_H__

#define TEGRA20		0x20
#define TEGRA30		0x30
#define TEGRA114	0x35
#define TEGRA124	0x40
#define TEGRA132	0x13
#define TEGRA210	0x21
<<<<<<< HEAD
=======
#define TEGRA186	0x18
#define TEGRA194	0x19
#define TEGRA234	0x23
>>>>>>> upstream/android-13

#define TEGRA_FUSE_SKU_CALIB_0	0xf0
#define TEGRA30_FUSE_SATA_CALIB	0x124
#define TEGRA_FUSE_USB_CALIB_EXT_0 0x250

#ifndef __ASSEMBLY__

u32 tegra_read_chipid(void);
u8 tegra_get_chip_id(void);
<<<<<<< HEAD
=======
u8 tegra_get_platform(void);
bool tegra_is_silicon(void);
>>>>>>> upstream/android-13

enum tegra_revision {
	TEGRA_REVISION_UNKNOWN = 0,
	TEGRA_REVISION_A01,
	TEGRA_REVISION_A02,
	TEGRA_REVISION_A03,
	TEGRA_REVISION_A03p,
	TEGRA_REVISION_A04,
	TEGRA_REVISION_MAX,
};

struct tegra_sku_info {
	int sku_id;
	int cpu_process_id;
	int cpu_speedo_id;
	int cpu_speedo_value;
	int cpu_iddq_value;
	int soc_process_id;
	int soc_speedo_id;
	int soc_speedo_value;
	int gpu_process_id;
	int gpu_speedo_id;
	int gpu_speedo_value;
	enum tegra_revision revision;
};

<<<<<<< HEAD
u32 tegra_read_straps(void);
u32 tegra_read_ram_code(void);
u32 tegra_read_chipid(void);
int tegra_fuse_readl(unsigned long offset, u32 *value);

extern struct tegra_sku_info tegra_sku_info;
=======
#ifdef CONFIG_ARCH_TEGRA
extern struct tegra_sku_info tegra_sku_info;
u32 tegra_read_straps(void);
u32 tegra_read_ram_code(void);
int tegra_fuse_readl(unsigned long offset, u32 *value);
#else
static struct tegra_sku_info tegra_sku_info __maybe_unused;

static inline u32 tegra_read_straps(void)
{
	return 0;
}

static inline u32 tegra_read_ram_code(void)
{
	return 0;
}

static inline int tegra_fuse_readl(unsigned long offset, u32 *value)
{
	return -ENODEV;
}
#endif
>>>>>>> upstream/android-13

struct device *tegra_soc_device_register(void);

#endif /* __ASSEMBLY__ */

#endif /* __SOC_TEGRA_FUSE_H__ */
