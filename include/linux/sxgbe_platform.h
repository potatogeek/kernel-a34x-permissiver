<<<<<<< HEAD
/*
 * 10G controller driver for Samsung EXYNOS SoCs
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * 10G controller driver for Samsung Exynos SoCs
>>>>>>> upstream/android-13
 *
 * Copyright (C) 2013 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Author: Siva Reddy Kallam <siva.kallam@samsung.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef __SXGBE_PLATFORM_H__
#define __SXGBE_PLATFORM_H__

<<<<<<< HEAD
=======
#include <linux/phy.h>

>>>>>>> upstream/android-13
/* MDC Clock Selection define*/
#define SXGBE_CSR_100_150M	0x0	/* MDC = clk_scr_i/62 */
#define SXGBE_CSR_150_250M	0x1	/* MDC = clk_scr_i/102 */
#define SXGBE_CSR_250_300M	0x2	/* MDC = clk_scr_i/122 */
#define SXGBE_CSR_300_350M	0x3	/* MDC = clk_scr_i/142 */
#define SXGBE_CSR_350_400M	0x4	/* MDC = clk_scr_i/162 */
#define SXGBE_CSR_400_500M	0x5	/* MDC = clk_scr_i/202 */

/* Platfrom data for platform device structure's
 * platform_data field
 */
struct sxgbe_mdio_bus_data {
	unsigned int phy_mask;
	int *irqs;
	int probed_phy_irq;
};

struct sxgbe_dma_cfg {
	int pbl;
	int fixed_burst;
	int burst_map;
	int adv_addr_mode;
};

struct sxgbe_plat_data {
	char *phy_bus_name;
	int bus_id;
	int phy_addr;
<<<<<<< HEAD
	int interface;
=======
	phy_interface_t interface;
>>>>>>> upstream/android-13
	struct sxgbe_mdio_bus_data *mdio_bus_data;
	struct sxgbe_dma_cfg *dma_cfg;
	int clk_csr;
	int pmt;
	int force_sf_dma_mode;
	int force_thresh_dma_mode;
	int riwt_off;
};

#endif /* __SXGBE_PLATFORM_H__ */
