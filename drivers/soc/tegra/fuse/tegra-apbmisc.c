<<<<<<< HEAD
/*
 * Copyright (c) 2014, NVIDIA CORPORATION.  All rights reserved.
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
 *
 */

=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2014, NVIDIA CORPORATION.  All rights reserved.
 */

#include <linux/export.h>
>>>>>>> upstream/android-13
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/io.h>

#include <soc/tegra/fuse.h>
#include <soc/tegra/common.h>

#include "fuse.h"

#define FUSE_SKU_INFO	0x10

#define PMC_STRAPPING_OPT_A_RAM_CODE_SHIFT	4
#define PMC_STRAPPING_OPT_A_RAM_CODE_MASK_LONG	\
	(0xf << PMC_STRAPPING_OPT_A_RAM_CODE_SHIFT)
#define PMC_STRAPPING_OPT_A_RAM_CODE_MASK_SHORT	\
	(0x3 << PMC_STRAPPING_OPT_A_RAM_CODE_SHIFT)

<<<<<<< HEAD
static void __iomem *apbmisc_base;
static void __iomem *strapping_base;
static bool long_ram_code;

u32 tegra_read_chipid(void)
{
	if (!apbmisc_base) {
		WARN(1, "Tegra Chip ID not yet available\n");
		return 0;
	}

	return readl_relaxed(apbmisc_base + 4);
=======
static bool long_ram_code;
static u32 strapping;
static u32 chipid;

u32 tegra_read_chipid(void)
{
	WARN(!chipid, "Tegra APB MISC not yet available\n");

	return chipid;
>>>>>>> upstream/android-13
}

u8 tegra_get_chip_id(void)
{
	return (tegra_read_chipid() >> 8) & 0xff;
}

<<<<<<< HEAD
u32 tegra_read_straps(void)
{
	if (strapping_base)
		return readl_relaxed(strapping_base);
	else
		return 0;
=======
u8 tegra_get_major_rev(void)
{
	return (tegra_read_chipid() >> 4) & 0xf;
}

u8 tegra_get_minor_rev(void)
{
	return (tegra_read_chipid() >> 16) & 0xf;
}

u8 tegra_get_platform(void)
{
	return (tegra_read_chipid() >> 20) & 0xf;
}

bool tegra_is_silicon(void)
{
	switch (tegra_get_chip_id()) {
	case TEGRA194:
	case TEGRA234:
		if (tegra_get_platform() == 0)
			return true;

		return false;
	}

	/*
	 * Chips prior to Tegra194 have a different way of determining whether
	 * they are silicon or not. Since we never supported simulation on the
	 * older Tegra chips, don't bother extracting the information and just
	 * report that we're running on silicon.
	 */
	return true;
}

u32 tegra_read_straps(void)
{
	WARN(!chipid, "Tegra ABP MISC not yet available\n");

	return strapping;
>>>>>>> upstream/android-13
}

u32 tegra_read_ram_code(void)
{
	u32 straps = tegra_read_straps();

	if (long_ram_code)
		straps &= PMC_STRAPPING_OPT_A_RAM_CODE_MASK_LONG;
	else
		straps &= PMC_STRAPPING_OPT_A_RAM_CODE_MASK_SHORT;

	return straps >> PMC_STRAPPING_OPT_A_RAM_CODE_SHIFT;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(tegra_read_ram_code);
>>>>>>> upstream/android-13

static const struct of_device_id apbmisc_match[] __initconst = {
	{ .compatible = "nvidia,tegra20-apbmisc", },
	{ .compatible = "nvidia,tegra186-misc", },
<<<<<<< HEAD
=======
	{ .compatible = "nvidia,tegra194-misc", },
	{ .compatible = "nvidia,tegra234-misc", },
>>>>>>> upstream/android-13
	{},
};

void __init tegra_init_revision(void)
{
<<<<<<< HEAD
	u32 id, chip_id, minor_rev;
	int rev;

	id = tegra_read_chipid();
	chip_id = (id >> 8) & 0xff;
	minor_rev = (id >> 16) & 0xf;

	switch (minor_rev) {
	case 1:
		rev = TEGRA_REVISION_A01;
		break;
	case 2:
		rev = TEGRA_REVISION_A02;
=======
	u8 chip_id, minor_rev;

	chip_id = tegra_get_chip_id();
	minor_rev = tegra_get_minor_rev();

	switch (minor_rev) {
	case 1:
		tegra_sku_info.revision = TEGRA_REVISION_A01;
		break;
	case 2:
		tegra_sku_info.revision = TEGRA_REVISION_A02;
>>>>>>> upstream/android-13
		break;
	case 3:
		if (chip_id == TEGRA20 && (tegra_fuse_read_spare(18) ||
					   tegra_fuse_read_spare(19)))
<<<<<<< HEAD
			rev = TEGRA_REVISION_A03p;
		else
			rev = TEGRA_REVISION_A03;
		break;
	case 4:
		rev = TEGRA_REVISION_A04;
		break;
	default:
		rev = TEGRA_REVISION_UNKNOWN;
	}

	tegra_sku_info.revision = rev;

=======
			tegra_sku_info.revision = TEGRA_REVISION_A03p;
		else
			tegra_sku_info.revision = TEGRA_REVISION_A03;
		break;
	case 4:
		tegra_sku_info.revision = TEGRA_REVISION_A04;
		break;
	default:
		tegra_sku_info.revision = TEGRA_REVISION_UNKNOWN;
	}

>>>>>>> upstream/android-13
	tegra_sku_info.sku_id = tegra_fuse_read_early(FUSE_SKU_INFO);
}

void __init tegra_init_apbmisc(void)
{
<<<<<<< HEAD
=======
	void __iomem *apbmisc_base, *strapping_base;
>>>>>>> upstream/android-13
	struct resource apbmisc, straps;
	struct device_node *np;

	np = of_find_matching_node(NULL, apbmisc_match);
	if (!np) {
		/*
		 * Fall back to legacy initialization for 32-bit ARM only. All
		 * 64-bit ARM device tree files for Tegra are required to have
		 * an APBMISC node.
		 *
		 * This is for backwards-compatibility with old device trees
		 * that didn't contain an APBMISC node.
		 */
		if (IS_ENABLED(CONFIG_ARM) && soc_is_tegra()) {
			/* APBMISC registers (chip revision, ...) */
			apbmisc.start = 0x70000800;
			apbmisc.end = 0x70000863;
			apbmisc.flags = IORESOURCE_MEM;

			/* strapping options */
			if (of_machine_is_compatible("nvidia,tegra124")) {
				straps.start = 0x7000e864;
				straps.end = 0x7000e867;
			} else {
				straps.start = 0x70000008;
				straps.end = 0x7000000b;
			}

			straps.flags = IORESOURCE_MEM;

			pr_warn("Using APBMISC region %pR\n", &apbmisc);
			pr_warn("Using strapping options registers %pR\n",
				&straps);
		} else {
			/*
			 * At this point we're not running on Tegra, so play
			 * nice with multi-platform kernels.
			 */
			return;
		}
	} else {
		/*
		 * Extract information from the device tree if we've found a
		 * matching node.
		 */
		if (of_address_to_resource(np, 0, &apbmisc) < 0) {
			pr_err("failed to get APBMISC registers\n");
			return;
		}

		if (of_address_to_resource(np, 1, &straps) < 0) {
			pr_err("failed to get strapping options registers\n");
			return;
		}
	}

<<<<<<< HEAD
	apbmisc_base = ioremap_nocache(apbmisc.start, resource_size(&apbmisc));
	if (!apbmisc_base)
		pr_err("failed to map APBMISC registers\n");

	strapping_base = ioremap_nocache(straps.start, resource_size(&straps));
	if (!strapping_base)
		pr_err("failed to map strapping options registers\n");
=======
	apbmisc_base = ioremap(apbmisc.start, resource_size(&apbmisc));
	if (!apbmisc_base) {
		pr_err("failed to map APBMISC registers\n");
	} else {
		chipid = readl_relaxed(apbmisc_base + 4);
		iounmap(apbmisc_base);
	}

	strapping_base = ioremap(straps.start, resource_size(&straps));
	if (!strapping_base) {
		pr_err("failed to map strapping options registers\n");
	} else {
		strapping = readl_relaxed(strapping_base);
		iounmap(strapping_base);
	}
>>>>>>> upstream/android-13

	long_ram_code = of_property_read_bool(np, "nvidia,long-ram-code");
}
