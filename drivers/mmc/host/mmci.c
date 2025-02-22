<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  linux/drivers/mmc/host/mmci.c - ARM PrimeCell MMCI PL180/1 driver
 *
 *  Copyright (C) 2003 Deep Blue Solutions, Ltd, All Rights Reserved.
 *  Copyright (C) 2010 ST-Ericsson SA
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/highmem.h>
#include <linux/log2.h>
<<<<<<< HEAD
#include <linux/mmc/pm.h>
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
=======
#include <linux/mmc/mmc.h>
#include <linux/mmc/pm.h>
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sd.h>
>>>>>>> upstream/android-13
#include <linux/mmc/slot-gpio.h>
#include <linux/amba/bus.h>
#include <linux/clk.h>
#include <linux/scatterlist.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/of_gpio.h>
=======
#include <linux/of.h>
>>>>>>> upstream/android-13
#include <linux/regulator/consumer.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/amba/mmci.h>
#include <linux/pm_runtime.h>
#include <linux/types.h>
#include <linux/pinctrl/consumer.h>
<<<<<<< HEAD
=======
#include <linux/reset.h>
#include <linux/gpio/consumer.h>
>>>>>>> upstream/android-13

#include <asm/div64.h>
#include <asm/io.h>

#include "mmci.h"
<<<<<<< HEAD
#include "mmci_qcom_dml.h"

#define DRIVER_NAME "mmci-pl18x"

=======

#define DRIVER_NAME "mmci-pl18x"

static void mmci_variant_init(struct mmci_host *host);
static void ux500_variant_init(struct mmci_host *host);
static void ux500v2_variant_init(struct mmci_host *host);

>>>>>>> upstream/android-13
static unsigned int fmax = 515633;

static struct variant_data variant_arm = {
	.fifosize		= 16 * 4,
	.fifohalfsize		= 8 * 4,
<<<<<<< HEAD
	.datalength_bits	= 16,
=======
	.cmdreg_cpsm_enable	= MCI_CPSM_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_RESPONSE | MCI_CPSM_LONGRSP,
	.cmdreg_srsp_crc	= MCI_CPSM_RESPONSE,
	.cmdreg_srsp		= MCI_CPSM_RESPONSE,
	.datalength_bits	= 16,
	.datactrl_blocksz	= 11,
>>>>>>> upstream/android-13
	.pwrreg_powerup		= MCI_PWR_UP,
	.f_max			= 100000000,
	.reversed_irq_handling	= true,
	.mmcimask1		= true,
<<<<<<< HEAD
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_ROD,
=======
	.irq_pio_mask		= MCI_IRQ_PIO_MASK,
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_ROD,
	.init			= mmci_variant_init,
>>>>>>> upstream/android-13
};

static struct variant_data variant_arm_extended_fifo = {
	.fifosize		= 128 * 4,
	.fifohalfsize		= 64 * 4,
<<<<<<< HEAD
	.datalength_bits	= 16,
	.pwrreg_powerup		= MCI_PWR_UP,
	.f_max			= 100000000,
	.mmcimask1		= true,
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_ROD,
=======
	.cmdreg_cpsm_enable	= MCI_CPSM_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_RESPONSE | MCI_CPSM_LONGRSP,
	.cmdreg_srsp_crc	= MCI_CPSM_RESPONSE,
	.cmdreg_srsp		= MCI_CPSM_RESPONSE,
	.datalength_bits	= 16,
	.datactrl_blocksz	= 11,
	.pwrreg_powerup		= MCI_PWR_UP,
	.f_max			= 100000000,
	.mmcimask1		= true,
	.irq_pio_mask		= MCI_IRQ_PIO_MASK,
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_ROD,
	.init			= mmci_variant_init,
>>>>>>> upstream/android-13
};

static struct variant_data variant_arm_extended_fifo_hwfc = {
	.fifosize		= 128 * 4,
	.fifohalfsize		= 64 * 4,
	.clkreg_enable		= MCI_ARM_HWFCEN,
<<<<<<< HEAD
	.datalength_bits	= 16,
	.pwrreg_powerup		= MCI_PWR_UP,
	.f_max			= 100000000,
	.mmcimask1		= true,
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_ROD,
=======
	.cmdreg_cpsm_enable	= MCI_CPSM_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_RESPONSE | MCI_CPSM_LONGRSP,
	.cmdreg_srsp_crc	= MCI_CPSM_RESPONSE,
	.cmdreg_srsp		= MCI_CPSM_RESPONSE,
	.datalength_bits	= 16,
	.datactrl_blocksz	= 11,
	.pwrreg_powerup		= MCI_PWR_UP,
	.f_max			= 100000000,
	.mmcimask1		= true,
	.irq_pio_mask		= MCI_IRQ_PIO_MASK,
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_ROD,
	.init			= mmci_variant_init,
>>>>>>> upstream/android-13
};

static struct variant_data variant_u300 = {
	.fifosize		= 16 * 4,
	.fifohalfsize		= 8 * 4,
	.clkreg_enable		= MCI_ST_U300_HWFCEN,
	.clkreg_8bit_bus_enable = MCI_ST_8BIT_BUS,
<<<<<<< HEAD
	.datalength_bits	= 16,
=======
	.cmdreg_cpsm_enable	= MCI_CPSM_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_RESPONSE | MCI_CPSM_LONGRSP,
	.cmdreg_srsp_crc	= MCI_CPSM_RESPONSE,
	.cmdreg_srsp		= MCI_CPSM_RESPONSE,
	.datalength_bits	= 16,
	.datactrl_blocksz	= 11,
>>>>>>> upstream/android-13
	.datactrl_mask_sdio	= MCI_DPSM_ST_SDIOEN,
	.st_sdio			= true,
	.pwrreg_powerup		= MCI_PWR_ON,
	.f_max			= 100000000,
	.signal_direction	= true,
	.pwrreg_clkgate		= true,
	.pwrreg_nopower		= true,
	.mmcimask1		= true,
<<<<<<< HEAD
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_OD,
=======
	.irq_pio_mask		= MCI_IRQ_PIO_MASK,
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_OD,
	.init			= mmci_variant_init,
>>>>>>> upstream/android-13
};

static struct variant_data variant_nomadik = {
	.fifosize		= 16 * 4,
	.fifohalfsize		= 8 * 4,
	.clkreg			= MCI_CLK_ENABLE,
	.clkreg_8bit_bus_enable = MCI_ST_8BIT_BUS,
<<<<<<< HEAD
	.datalength_bits	= 24,
=======
	.cmdreg_cpsm_enable	= MCI_CPSM_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_RESPONSE | MCI_CPSM_LONGRSP,
	.cmdreg_srsp_crc	= MCI_CPSM_RESPONSE,
	.cmdreg_srsp		= MCI_CPSM_RESPONSE,
	.datalength_bits	= 24,
	.datactrl_blocksz	= 11,
>>>>>>> upstream/android-13
	.datactrl_mask_sdio	= MCI_DPSM_ST_SDIOEN,
	.st_sdio		= true,
	.st_clkdiv		= true,
	.pwrreg_powerup		= MCI_PWR_ON,
	.f_max			= 100000000,
	.signal_direction	= true,
	.pwrreg_clkgate		= true,
	.pwrreg_nopower		= true,
	.mmcimask1		= true,
<<<<<<< HEAD
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_OD,
=======
	.irq_pio_mask		= MCI_IRQ_PIO_MASK,
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_OD,
	.init			= mmci_variant_init,
>>>>>>> upstream/android-13
};

static struct variant_data variant_ux500 = {
	.fifosize		= 30 * 4,
	.fifohalfsize		= 8 * 4,
	.clkreg			= MCI_CLK_ENABLE,
	.clkreg_enable		= MCI_ST_UX500_HWFCEN,
	.clkreg_8bit_bus_enable = MCI_ST_8BIT_BUS,
	.clkreg_neg_edge_enable	= MCI_ST_UX500_NEG_EDGE,
<<<<<<< HEAD
	.datalength_bits	= 24,
=======
	.cmdreg_cpsm_enable	= MCI_CPSM_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_RESPONSE | MCI_CPSM_LONGRSP,
	.cmdreg_srsp_crc	= MCI_CPSM_RESPONSE,
	.cmdreg_srsp		= MCI_CPSM_RESPONSE,
	.datalength_bits	= 24,
	.datactrl_blocksz	= 11,
	.datactrl_any_blocksz	= true,
	.dma_power_of_2		= true,
>>>>>>> upstream/android-13
	.datactrl_mask_sdio	= MCI_DPSM_ST_SDIOEN,
	.st_sdio		= true,
	.st_clkdiv		= true,
	.pwrreg_powerup		= MCI_PWR_ON,
	.f_max			= 100000000,
	.signal_direction	= true,
	.pwrreg_clkgate		= true,
	.busy_detect		= true,
	.busy_dpsm_flag		= MCI_DPSM_ST_BUSYMODE,
	.busy_detect_flag	= MCI_ST_CARDBUSY,
	.busy_detect_mask	= MCI_ST_BUSYENDMASK,
	.pwrreg_nopower		= true,
	.mmcimask1		= true,
<<<<<<< HEAD
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_OD,
=======
	.irq_pio_mask		= MCI_IRQ_PIO_MASK,
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_OD,
	.init			= ux500_variant_init,
>>>>>>> upstream/android-13
};

static struct variant_data variant_ux500v2 = {
	.fifosize		= 30 * 4,
	.fifohalfsize		= 8 * 4,
	.clkreg			= MCI_CLK_ENABLE,
	.clkreg_enable		= MCI_ST_UX500_HWFCEN,
	.clkreg_8bit_bus_enable = MCI_ST_8BIT_BUS,
	.clkreg_neg_edge_enable	= MCI_ST_UX500_NEG_EDGE,
<<<<<<< HEAD
	.datactrl_mask_ddrmode	= MCI_DPSM_ST_DDRMODE,
	.datalength_bits	= 24,
	.datactrl_mask_sdio	= MCI_DPSM_ST_SDIOEN,
	.st_sdio		= true,
	.st_clkdiv		= true,
	.blksz_datactrl16	= true,
=======
	.cmdreg_cpsm_enable	= MCI_CPSM_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_RESPONSE | MCI_CPSM_LONGRSP,
	.cmdreg_srsp_crc	= MCI_CPSM_RESPONSE,
	.cmdreg_srsp		= MCI_CPSM_RESPONSE,
	.datactrl_mask_ddrmode	= MCI_DPSM_ST_DDRMODE,
	.datalength_bits	= 24,
	.datactrl_blocksz	= 11,
	.datactrl_any_blocksz	= true,
	.dma_power_of_2		= true,
	.datactrl_mask_sdio	= MCI_DPSM_ST_SDIOEN,
	.st_sdio		= true,
	.st_clkdiv		= true,
>>>>>>> upstream/android-13
	.pwrreg_powerup		= MCI_PWR_ON,
	.f_max			= 100000000,
	.signal_direction	= true,
	.pwrreg_clkgate		= true,
	.busy_detect		= true,
	.busy_dpsm_flag		= MCI_DPSM_ST_BUSYMODE,
	.busy_detect_flag	= MCI_ST_CARDBUSY,
	.busy_detect_mask	= MCI_ST_BUSYENDMASK,
	.pwrreg_nopower		= true,
	.mmcimask1		= true,
<<<<<<< HEAD
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_OD,
=======
	.irq_pio_mask		= MCI_IRQ_PIO_MASK,
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_OD,
	.init			= ux500v2_variant_init,
>>>>>>> upstream/android-13
};

static struct variant_data variant_stm32 = {
	.fifosize		= 32 * 4,
	.fifohalfsize		= 8 * 4,
	.clkreg			= MCI_CLK_ENABLE,
	.clkreg_enable		= MCI_ST_UX500_HWFCEN,
	.clkreg_8bit_bus_enable = MCI_ST_8BIT_BUS,
	.clkreg_neg_edge_enable	= MCI_ST_UX500_NEG_EDGE,
<<<<<<< HEAD
	.datalength_bits	= 24,
=======
	.cmdreg_cpsm_enable	= MCI_CPSM_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_RESPONSE | MCI_CPSM_LONGRSP,
	.cmdreg_srsp_crc	= MCI_CPSM_RESPONSE,
	.cmdreg_srsp		= MCI_CPSM_RESPONSE,
	.irq_pio_mask		= MCI_IRQ_PIO_MASK,
	.datalength_bits	= 24,
	.datactrl_blocksz	= 11,
>>>>>>> upstream/android-13
	.datactrl_mask_sdio	= MCI_DPSM_ST_SDIOEN,
	.st_sdio		= true,
	.st_clkdiv		= true,
	.pwrreg_powerup		= MCI_PWR_ON,
	.f_max			= 48000000,
	.pwrreg_clkgate		= true,
	.pwrreg_nopower		= true,
<<<<<<< HEAD
=======
	.init			= mmci_variant_init,
};

static struct variant_data variant_stm32_sdmmc = {
	.fifosize		= 16 * 4,
	.fifohalfsize		= 8 * 4,
	.f_max			= 208000000,
	.stm32_clkdiv		= true,
	.cmdreg_cpsm_enable	= MCI_CPSM_STM32_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_STM32_LRSP_CRC,
	.cmdreg_srsp_crc	= MCI_CPSM_STM32_SRSP_CRC,
	.cmdreg_srsp		= MCI_CPSM_STM32_SRSP,
	.cmdreg_stop		= MCI_CPSM_STM32_CMDSTOP,
	.data_cmd_enable	= MCI_CPSM_STM32_CMDTRANS,
	.irq_pio_mask		= MCI_IRQ_PIO_STM32_MASK,
	.datactrl_first		= true,
	.datacnt_useless	= true,
	.datalength_bits	= 25,
	.datactrl_blocksz	= 14,
	.datactrl_any_blocksz	= true,
	.datactrl_mask_sdio	= MCI_DPSM_ST_SDIOEN,
	.stm32_idmabsize_mask	= GENMASK(12, 5),
	.busy_timeout		= true,
	.busy_detect		= true,
	.busy_detect_flag	= MCI_STM32_BUSYD0,
	.busy_detect_mask	= MCI_STM32_BUSYD0ENDMASK,
	.init			= sdmmc_variant_init,
};

static struct variant_data variant_stm32_sdmmcv2 = {
	.fifosize		= 16 * 4,
	.fifohalfsize		= 8 * 4,
	.f_max			= 208000000,
	.stm32_clkdiv		= true,
	.cmdreg_cpsm_enable	= MCI_CPSM_STM32_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_STM32_LRSP_CRC,
	.cmdreg_srsp_crc	= MCI_CPSM_STM32_SRSP_CRC,
	.cmdreg_srsp		= MCI_CPSM_STM32_SRSP,
	.cmdreg_stop		= MCI_CPSM_STM32_CMDSTOP,
	.data_cmd_enable	= MCI_CPSM_STM32_CMDTRANS,
	.irq_pio_mask		= MCI_IRQ_PIO_STM32_MASK,
	.datactrl_first		= true,
	.datacnt_useless	= true,
	.datalength_bits	= 25,
	.datactrl_blocksz	= 14,
	.datactrl_any_blocksz	= true,
	.datactrl_mask_sdio	= MCI_DPSM_ST_SDIOEN,
	.stm32_idmabsize_mask	= GENMASK(16, 5),
	.dma_lli		= true,
	.busy_timeout		= true,
	.busy_detect		= true,
	.busy_detect_flag	= MCI_STM32_BUSYD0,
	.busy_detect_mask	= MCI_STM32_BUSYD0ENDMASK,
	.init			= sdmmc_variant_init,
>>>>>>> upstream/android-13
};

static struct variant_data variant_qcom = {
	.fifosize		= 16 * 4,
	.fifohalfsize		= 8 * 4,
	.clkreg			= MCI_CLK_ENABLE,
	.clkreg_enable		= MCI_QCOM_CLK_FLOWENA |
				  MCI_QCOM_CLK_SELECT_IN_FBCLK,
	.clkreg_8bit_bus_enable = MCI_QCOM_CLK_WIDEBUS_8,
	.datactrl_mask_ddrmode	= MCI_QCOM_CLK_SELECT_IN_DDR_MODE,
<<<<<<< HEAD
	.data_cmd_enable	= MCI_CPSM_QCOM_DATCMD,
	.blksz_datactrl4	= true,
	.datalength_bits	= 24,
=======
	.cmdreg_cpsm_enable	= MCI_CPSM_ENABLE,
	.cmdreg_lrsp_crc	= MCI_CPSM_RESPONSE | MCI_CPSM_LONGRSP,
	.cmdreg_srsp_crc	= MCI_CPSM_RESPONSE,
	.cmdreg_srsp		= MCI_CPSM_RESPONSE,
	.data_cmd_enable	= MCI_CPSM_QCOM_DATCMD,
	.datalength_bits	= 24,
	.datactrl_blocksz	= 11,
	.datactrl_any_blocksz	= true,
>>>>>>> upstream/android-13
	.pwrreg_powerup		= MCI_PWR_UP,
	.f_max			= 208000000,
	.explicit_mclk_control	= true,
	.qcom_fifo		= true,
	.qcom_dml		= true,
	.mmcimask1		= true,
<<<<<<< HEAD
=======
	.irq_pio_mask		= MCI_IRQ_PIO_MASK,
>>>>>>> upstream/android-13
	.start_err		= MCI_STARTBITERR,
	.opendrain		= MCI_ROD,
	.init			= qcom_variant_init,
};

/* Busy detection for the ST Micro variant */
static int mmci_card_busy(struct mmc_host *mmc)
{
	struct mmci_host *host = mmc_priv(mmc);
	unsigned long flags;
	int busy = 0;

	spin_lock_irqsave(&host->lock, flags);
	if (readl(host->base + MMCISTATUS) & host->variant->busy_detect_flag)
		busy = 1;
	spin_unlock_irqrestore(&host->lock, flags);

	return busy;
}

<<<<<<< HEAD
/*
 * Validate mmc prerequisites
 */
static int mmci_validate_data(struct mmci_host *host,
			      struct mmc_data *data)
{
	if (!data)
		return 0;

	if (!is_power_of_2(data->blksz)) {
		dev_err(mmc_dev(host->mmc),
			"unsupported block size (%d bytes)\n", data->blksz);
		return -EINVAL;
	}

	return 0;
}

=======
>>>>>>> upstream/android-13
static void mmci_reg_delay(struct mmci_host *host)
{
	/*
	 * According to the spec, at least three feedback clock cycles
	 * of max 52 MHz must pass between two writes to the MMCICLOCK reg.
	 * Three MCLK clock cycles must pass between two MMCIPOWER reg writes.
	 * Worst delay time during card init is at 100 kHz => 30 us.
	 * Worst delay time when up and running is at 25 MHz => 120 ns.
	 */
	if (host->cclk < 25000000)
		udelay(30);
	else
		ndelay(120);
}

/*
 * This must be called with host->lock held
 */
<<<<<<< HEAD
static void mmci_write_clkreg(struct mmci_host *host, u32 clk)
=======
void mmci_write_clkreg(struct mmci_host *host, u32 clk)
>>>>>>> upstream/android-13
{
	if (host->clk_reg != clk) {
		host->clk_reg = clk;
		writel(clk, host->base + MMCICLOCK);
	}
}

/*
 * This must be called with host->lock held
 */
<<<<<<< HEAD
static void mmci_write_pwrreg(struct mmci_host *host, u32 pwr)
=======
void mmci_write_pwrreg(struct mmci_host *host, u32 pwr)
>>>>>>> upstream/android-13
{
	if (host->pwr_reg != pwr) {
		host->pwr_reg = pwr;
		writel(pwr, host->base + MMCIPOWER);
	}
}

/*
 * This must be called with host->lock held
 */
static void mmci_write_datactrlreg(struct mmci_host *host, u32 datactrl)
{
	/* Keep busy mode in DPSM if enabled */
	datactrl |= host->datactrl_reg & host->variant->busy_dpsm_flag;

	if (host->datactrl_reg != datactrl) {
		host->datactrl_reg = datactrl;
		writel(datactrl, host->base + MMCIDATACTRL);
	}
}

/*
 * This must be called with host->lock held
 */
static void mmci_set_clkreg(struct mmci_host *host, unsigned int desired)
{
	struct variant_data *variant = host->variant;
	u32 clk = variant->clkreg;

	/* Make sure cclk reflects the current calculated clock */
	host->cclk = 0;

	if (desired) {
		if (variant->explicit_mclk_control) {
			host->cclk = host->mclk;
		} else if (desired >= host->mclk) {
			clk = MCI_CLK_BYPASS;
			if (variant->st_clkdiv)
				clk |= MCI_ST_UX500_NEG_EDGE;
			host->cclk = host->mclk;
		} else if (variant->st_clkdiv) {
			/*
			 * DB8500 TRM says f = mclk / (clkdiv + 2)
			 * => clkdiv = (mclk / f) - 2
			 * Round the divider up so we don't exceed the max
			 * frequency
			 */
			clk = DIV_ROUND_UP(host->mclk, desired) - 2;
			if (clk >= 256)
				clk = 255;
			host->cclk = host->mclk / (clk + 2);
		} else {
			/*
			 * PL180 TRM says f = mclk / (2 * (clkdiv + 1))
			 * => clkdiv = mclk / (2 * f) - 1
			 */
			clk = host->mclk / (2 * desired) - 1;
			if (clk >= 256)
				clk = 255;
			host->cclk = host->mclk / (2 * (clk + 1));
		}

		clk |= variant->clkreg_enable;
		clk |= MCI_CLK_ENABLE;
		/* This hasn't proven to be worthwhile */
		/* clk |= MCI_CLK_PWRSAVE; */
	}

	/* Set actual clock for debug */
	host->mmc->actual_clock = host->cclk;

	if (host->mmc->ios.bus_width == MMC_BUS_WIDTH_4)
		clk |= MCI_4BIT_BUS;
	if (host->mmc->ios.bus_width == MMC_BUS_WIDTH_8)
		clk |= variant->clkreg_8bit_bus_enable;

	if (host->mmc->ios.timing == MMC_TIMING_UHS_DDR50 ||
	    host->mmc->ios.timing == MMC_TIMING_MMC_DDR52)
		clk |= variant->clkreg_neg_edge_enable;

	mmci_write_clkreg(host, clk);
}

<<<<<<< HEAD
=======
static void mmci_dma_release(struct mmci_host *host)
{
	if (host->ops && host->ops->dma_release)
		host->ops->dma_release(host);

	host->use_dma = false;
}

static void mmci_dma_setup(struct mmci_host *host)
{
	if (!host->ops || !host->ops->dma_setup)
		return;

	if (host->ops->dma_setup(host))
		return;

	/* initialize pre request cookie */
	host->next_cookie = 1;

	host->use_dma = true;
}

/*
 * Validate mmc prerequisites
 */
static int mmci_validate_data(struct mmci_host *host,
			      struct mmc_data *data)
{
	struct variant_data *variant = host->variant;

	if (!data)
		return 0;
	if (!is_power_of_2(data->blksz) && !variant->datactrl_any_blocksz) {
		dev_err(mmc_dev(host->mmc),
			"unsupported block size (%d bytes)\n", data->blksz);
		return -EINVAL;
	}

	if (host->ops && host->ops->validate_data)
		return host->ops->validate_data(host, data);

	return 0;
}

static int mmci_prep_data(struct mmci_host *host, struct mmc_data *data, bool next)
{
	int err;

	if (!host->ops || !host->ops->prep_data)
		return 0;

	err = host->ops->prep_data(host, data, next);

	if (next && !err)
		data->host_cookie = ++host->next_cookie < 0 ?
			1 : host->next_cookie;

	return err;
}

static void mmci_unprep_data(struct mmci_host *host, struct mmc_data *data,
		      int err)
{
	if (host->ops && host->ops->unprep_data)
		host->ops->unprep_data(host, data, err);

	data->host_cookie = 0;
}

static void mmci_get_next_data(struct mmci_host *host, struct mmc_data *data)
{
	WARN_ON(data->host_cookie && data->host_cookie != host->next_cookie);

	if (host->ops && host->ops->get_next_data)
		host->ops->get_next_data(host, data);
}

static int mmci_dma_start(struct mmci_host *host, unsigned int datactrl)
{
	struct mmc_data *data = host->data;
	int ret;

	if (!host->use_dma)
		return -EINVAL;

	ret = mmci_prep_data(host, data, false);
	if (ret)
		return ret;

	if (!host->ops || !host->ops->dma_start)
		return -EINVAL;

	/* Okay, go for it. */
	dev_vdbg(mmc_dev(host->mmc),
		 "Submit MMCI DMA job, sglen %d blksz %04x blks %04x flags %08x\n",
		 data->sg_len, data->blksz, data->blocks, data->flags);

	ret = host->ops->dma_start(host, &datactrl);
	if (ret)
		return ret;

	/* Trigger the DMA transfer */
	mmci_write_datactrlreg(host, datactrl);

	/*
	 * Let the MMCI say when the data is ended and it's time
	 * to fire next DMA request. When that happens, MMCI will
	 * call mmci_data_end()
	 */
	writel(readl(host->base + MMCIMASK0) | MCI_DATAENDMASK,
	       host->base + MMCIMASK0);
	return 0;
}

static void mmci_dma_finalize(struct mmci_host *host, struct mmc_data *data)
{
	if (!host->use_dma)
		return;

	if (host->ops && host->ops->dma_finalize)
		host->ops->dma_finalize(host, data);
}

static void mmci_dma_error(struct mmci_host *host)
{
	if (!host->use_dma)
		return;

	if (host->ops && host->ops->dma_error)
		host->ops->dma_error(host);
}

>>>>>>> upstream/android-13
static void
mmci_request_end(struct mmci_host *host, struct mmc_request *mrq)
{
	writel(0, host->base + MMCICOMMAND);

	BUG_ON(host->data);

	host->mrq = NULL;
	host->cmd = NULL;

	mmc_request_done(host->mmc, mrq);
}

static void mmci_set_mask1(struct mmci_host *host, unsigned int mask)
{
	void __iomem *base = host->base;
	struct variant_data *variant = host->variant;

	if (host->singleirq) {
		unsigned int mask0 = readl(base + MMCIMASK0);

<<<<<<< HEAD
		mask0 &= ~MCI_IRQ1MASK;
=======
		mask0 &= ~variant->irq_pio_mask;
>>>>>>> upstream/android-13
		mask0 |= mask;

		writel(mask0, base + MMCIMASK0);
	}

	if (variant->mmcimask1)
		writel(mask, base + MMCIMASK1);

	host->mask1_reg = mask;
}

static void mmci_stop_data(struct mmci_host *host)
{
	mmci_write_datactrlreg(host, 0);
	mmci_set_mask1(host, 0);
	host->data = NULL;
}

static void mmci_init_sg(struct mmci_host *host, struct mmc_data *data)
{
	unsigned int flags = SG_MITER_ATOMIC;

	if (data->flags & MMC_DATA_READ)
		flags |= SG_MITER_TO_SG;
	else
		flags |= SG_MITER_FROM_SG;

	sg_miter_start(&host->sg_miter, data->sg, data->sg_len, flags);
}

<<<<<<< HEAD
=======
static u32 mmci_get_dctrl_cfg(struct mmci_host *host)
{
	return MCI_DPSM_ENABLE | mmci_dctrl_blksz(host);
}

static u32 ux500v2_get_dctrl_cfg(struct mmci_host *host)
{
	return MCI_DPSM_ENABLE | (host->data->blksz << 16);
}

static bool ux500_busy_complete(struct mmci_host *host, u32 status, u32 err_msk)
{
	void __iomem *base = host->base;

	/*
	 * Before unmasking for the busy end IRQ, confirm that the
	 * command was sent successfully. To keep track of having a
	 * command in-progress, waiting for busy signaling to end,
	 * store the status in host->busy_status.
	 *
	 * Note that, the card may need a couple of clock cycles before
	 * it starts signaling busy on DAT0, hence re-read the
	 * MMCISTATUS register here, to allow the busy bit to be set.
	 * Potentially we may even need to poll the register for a
	 * while, to allow it to be set, but tests indicates that it
	 * isn't needed.
	 */
	if (!host->busy_status && !(status & err_msk) &&
	    (readl(base + MMCISTATUS) & host->variant->busy_detect_flag)) {
		writel(readl(base + MMCIMASK0) |
		       host->variant->busy_detect_mask,
		       base + MMCIMASK0);

		host->busy_status = status & (MCI_CMDSENT | MCI_CMDRESPEND);
		return false;
	}

	/*
	 * If there is a command in-progress that has been successfully
	 * sent, then bail out if busy status is set and wait for the
	 * busy end IRQ.
	 *
	 * Note that, the HW triggers an IRQ on both edges while
	 * monitoring DAT0 for busy completion, but there is only one
	 * status bit in MMCISTATUS for the busy state. Therefore
	 * both the start and the end interrupts needs to be cleared,
	 * one after the other. So, clear the busy start IRQ here.
	 */
	if (host->busy_status &&
	    (status & host->variant->busy_detect_flag)) {
		writel(host->variant->busy_detect_mask, base + MMCICLEAR);
		return false;
	}

	/*
	 * If there is a command in-progress that has been successfully
	 * sent and the busy bit isn't set, it means we have received
	 * the busy end IRQ. Clear and mask the IRQ, then continue to
	 * process the command.
	 */
	if (host->busy_status) {
		writel(host->variant->busy_detect_mask, base + MMCICLEAR);

		writel(readl(base + MMCIMASK0) &
		       ~host->variant->busy_detect_mask, base + MMCIMASK0);
		host->busy_status = 0;
	}

	return true;
}

>>>>>>> upstream/android-13
/*
 * All the DMA operation mode stuff goes inside this ifdef.
 * This assumes that you have a generic DMA device interface,
 * no custom DMA interfaces are supported.
 */
#ifdef CONFIG_DMA_ENGINE
<<<<<<< HEAD
static void mmci_dma_setup(struct mmci_host *host)
{
	const char *rxname, *txname;

	host->dma_rx_channel = dma_request_slave_channel(mmc_dev(host->mmc), "rx");
	host->dma_tx_channel = dma_request_slave_channel(mmc_dev(host->mmc), "tx");

	/* initialize pre request cookie */
	host->next_data.cookie = 1;
=======
struct mmci_dmae_next {
	struct dma_async_tx_descriptor *desc;
	struct dma_chan	*chan;
};

struct mmci_dmae_priv {
	struct dma_chan	*cur;
	struct dma_chan	*rx_channel;
	struct dma_chan	*tx_channel;
	struct dma_async_tx_descriptor	*desc_current;
	struct mmci_dmae_next next_data;
};

int mmci_dmae_setup(struct mmci_host *host)
{
	const char *rxname, *txname;
	struct mmci_dmae_priv *dmae;

	dmae = devm_kzalloc(mmc_dev(host->mmc), sizeof(*dmae), GFP_KERNEL);
	if (!dmae)
		return -ENOMEM;

	host->dma_priv = dmae;

	dmae->rx_channel = dma_request_chan(mmc_dev(host->mmc), "rx");
	if (IS_ERR(dmae->rx_channel)) {
		int ret = PTR_ERR(dmae->rx_channel);
		dmae->rx_channel = NULL;
		return ret;
	}

	dmae->tx_channel = dma_request_chan(mmc_dev(host->mmc), "tx");
	if (IS_ERR(dmae->tx_channel)) {
		if (PTR_ERR(dmae->tx_channel) == -EPROBE_DEFER)
			dev_warn(mmc_dev(host->mmc),
				 "Deferred probe for TX channel ignored\n");
		dmae->tx_channel = NULL;
	}
>>>>>>> upstream/android-13

	/*
	 * If only an RX channel is specified, the driver will
	 * attempt to use it bidirectionally, however if it is
	 * is specified but cannot be located, DMA will be disabled.
	 */
<<<<<<< HEAD
	if (host->dma_rx_channel && !host->dma_tx_channel)
		host->dma_tx_channel = host->dma_rx_channel;

	if (host->dma_rx_channel)
		rxname = dma_chan_name(host->dma_rx_channel);
	else
		rxname = "none";

	if (host->dma_tx_channel)
		txname = dma_chan_name(host->dma_tx_channel);
=======
	if (dmae->rx_channel && !dmae->tx_channel)
		dmae->tx_channel = dmae->rx_channel;

	if (dmae->rx_channel)
		rxname = dma_chan_name(dmae->rx_channel);
	else
		rxname = "none";

	if (dmae->tx_channel)
		txname = dma_chan_name(dmae->tx_channel);
>>>>>>> upstream/android-13
	else
		txname = "none";

	dev_info(mmc_dev(host->mmc), "DMA channels RX %s, TX %s\n",
		 rxname, txname);

	/*
	 * Limit the maximum segment size in any SG entry according to
	 * the parameters of the DMA engine device.
	 */
<<<<<<< HEAD
	if (host->dma_tx_channel) {
		struct device *dev = host->dma_tx_channel->device->dev;
=======
	if (dmae->tx_channel) {
		struct device *dev = dmae->tx_channel->device->dev;
>>>>>>> upstream/android-13
		unsigned int max_seg_size = dma_get_max_seg_size(dev);

		if (max_seg_size < host->mmc->max_seg_size)
			host->mmc->max_seg_size = max_seg_size;
	}
<<<<<<< HEAD
	if (host->dma_rx_channel) {
		struct device *dev = host->dma_rx_channel->device->dev;
=======
	if (dmae->rx_channel) {
		struct device *dev = dmae->rx_channel->device->dev;
>>>>>>> upstream/android-13
		unsigned int max_seg_size = dma_get_max_seg_size(dev);

		if (max_seg_size < host->mmc->max_seg_size)
			host->mmc->max_seg_size = max_seg_size;
	}

<<<<<<< HEAD
	if (host->ops && host->ops->dma_setup)
		host->ops->dma_setup(host);
=======
	if (!dmae->tx_channel || !dmae->rx_channel) {
		mmci_dmae_release(host);
		return -EINVAL;
	}

	return 0;
>>>>>>> upstream/android-13
}

/*
 * This is used in or so inline it
 * so it can be discarded.
 */
<<<<<<< HEAD
static inline void mmci_dma_release(struct mmci_host *host)
{
	if (host->dma_rx_channel)
		dma_release_channel(host->dma_rx_channel);
	if (host->dma_tx_channel)
		dma_release_channel(host->dma_tx_channel);
	host->dma_rx_channel = host->dma_tx_channel = NULL;
}

static void mmci_dma_data_error(struct mmci_host *host)
{
	dev_err(mmc_dev(host->mmc), "error during DMA transfer!\n");
	dmaengine_terminate_all(host->dma_current);
	host->dma_in_progress = false;
	host->dma_current = NULL;
	host->dma_desc_current = NULL;
	host->data->host_cookie = 0;
=======
void mmci_dmae_release(struct mmci_host *host)
{
	struct mmci_dmae_priv *dmae = host->dma_priv;

	if (dmae->rx_channel)
		dma_release_channel(dmae->rx_channel);
	if (dmae->tx_channel)
		dma_release_channel(dmae->tx_channel);
	dmae->rx_channel = dmae->tx_channel = NULL;
>>>>>>> upstream/android-13
}

static void mmci_dma_unmap(struct mmci_host *host, struct mmc_data *data)
{
<<<<<<< HEAD
	struct dma_chan *chan;

	if (data->flags & MMC_DATA_READ)
		chan = host->dma_rx_channel;
	else
		chan = host->dma_tx_channel;
=======
	struct mmci_dmae_priv *dmae = host->dma_priv;
	struct dma_chan *chan;

	if (data->flags & MMC_DATA_READ)
		chan = dmae->rx_channel;
	else
		chan = dmae->tx_channel;
>>>>>>> upstream/android-13

	dma_unmap_sg(chan->device->dev, data->sg, data->sg_len,
		     mmc_get_dma_dir(data));
}

<<<<<<< HEAD
static void mmci_dma_finalize(struct mmci_host *host, struct mmc_data *data)
{
	u32 status;
	int i;

=======
void mmci_dmae_error(struct mmci_host *host)
{
	struct mmci_dmae_priv *dmae = host->dma_priv;

	if (!dma_inprogress(host))
		return;

	dev_err(mmc_dev(host->mmc), "error during DMA transfer!\n");
	dmaengine_terminate_all(dmae->cur);
	host->dma_in_progress = false;
	dmae->cur = NULL;
	dmae->desc_current = NULL;
	host->data->host_cookie = 0;

	mmci_dma_unmap(host, host->data);
}

void mmci_dmae_finalize(struct mmci_host *host, struct mmc_data *data)
{
	struct mmci_dmae_priv *dmae = host->dma_priv;
	u32 status;
	int i;

	if (!dma_inprogress(host))
		return;

>>>>>>> upstream/android-13
	/* Wait up to 1ms for the DMA to complete */
	for (i = 0; ; i++) {
		status = readl(host->base + MMCISTATUS);
		if (!(status & MCI_RXDATAAVLBLMASK) || i >= 100)
			break;
		udelay(10);
	}

	/*
	 * Check to see whether we still have some data left in the FIFO -
	 * this catches DMA controllers which are unable to monitor the
	 * DMALBREQ and DMALSREQ signals while allowing us to DMA to non-
	 * contiguous buffers.  On TX, we'll get a FIFO underrun error.
	 */
	if (status & MCI_RXDATAAVLBLMASK) {
<<<<<<< HEAD
		mmci_dma_data_error(host);
		if (!data->error)
			data->error = -EIO;
	}

	if (!data->host_cookie)
		mmci_dma_unmap(host, data);
=======
		mmci_dma_error(host);
		if (!data->error)
			data->error = -EIO;
	} else if (!data->host_cookie) {
		mmci_dma_unmap(host, data);
	}
>>>>>>> upstream/android-13

	/*
	 * Use of DMA with scatter-gather is impossible.
	 * Give up with DMA and switch back to PIO mode.
	 */
	if (status & MCI_RXDATAAVLBLMASK) {
		dev_err(mmc_dev(host->mmc), "buggy DMA detected. Taking evasive action.\n");
		mmci_dma_release(host);
	}

	host->dma_in_progress = false;
<<<<<<< HEAD
	host->dma_current = NULL;
	host->dma_desc_current = NULL;
}

/* prepares DMA channel and DMA descriptor, returns non-zero on failure */
static int __mmci_dma_prep_data(struct mmci_host *host, struct mmc_data *data,
				struct dma_chan **dma_chan,
				struct dma_async_tx_descriptor **dma_desc)
{
=======
	dmae->cur = NULL;
	dmae->desc_current = NULL;
}

/* prepares DMA channel and DMA descriptor, returns non-zero on failure */
static int _mmci_dmae_prep_data(struct mmci_host *host, struct mmc_data *data,
				struct dma_chan **dma_chan,
				struct dma_async_tx_descriptor **dma_desc)
{
	struct mmci_dmae_priv *dmae = host->dma_priv;
>>>>>>> upstream/android-13
	struct variant_data *variant = host->variant;
	struct dma_slave_config conf = {
		.src_addr = host->phybase + MMCIFIFO,
		.dst_addr = host->phybase + MMCIFIFO,
		.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES,
		.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES,
		.src_maxburst = variant->fifohalfsize >> 2, /* # of words */
		.dst_maxburst = variant->fifohalfsize >> 2, /* # of words */
		.device_fc = false,
	};
	struct dma_chan *chan;
	struct dma_device *device;
	struct dma_async_tx_descriptor *desc;
	int nr_sg;
	unsigned long flags = DMA_CTRL_ACK;

	if (data->flags & MMC_DATA_READ) {
		conf.direction = DMA_DEV_TO_MEM;
<<<<<<< HEAD
		chan = host->dma_rx_channel;
	} else {
		conf.direction = DMA_MEM_TO_DEV;
		chan = host->dma_tx_channel;
=======
		chan = dmae->rx_channel;
	} else {
		conf.direction = DMA_MEM_TO_DEV;
		chan = dmae->tx_channel;
>>>>>>> upstream/android-13
	}

	/* If there's no DMA channel, fall back to PIO */
	if (!chan)
		return -EINVAL;

	/* If less than or equal to the fifo size, don't bother with DMA */
	if (data->blksz * data->blocks <= variant->fifosize)
		return -EINVAL;

<<<<<<< HEAD
=======
	/*
	 * This is necessary to get SDIO working on the Ux500. We do not yet
	 * know if this is a bug in:
	 * - The Ux500 DMA controller (DMA40)
	 * - The MMCI DMA interface on the Ux500
	 * some power of two blocks (such as 64 bytes) are sent regularly
	 * during SDIO traffic and those work fine so for these we enable DMA
	 * transfers.
	 */
	if (host->variant->dma_power_of_2 && !is_power_of_2(data->blksz))
		return -EINVAL;

>>>>>>> upstream/android-13
	device = chan->device;
	nr_sg = dma_map_sg(device->dev, data->sg, data->sg_len,
			   mmc_get_dma_dir(data));
	if (nr_sg == 0)
		return -EINVAL;

	if (host->variant->qcom_dml)
		flags |= DMA_PREP_INTERRUPT;

	dmaengine_slave_config(chan, &conf);
	desc = dmaengine_prep_slave_sg(chan, data->sg, nr_sg,
					    conf.direction, flags);
	if (!desc)
		goto unmap_exit;

	*dma_chan = chan;
	*dma_desc = desc;

	return 0;

 unmap_exit:
	dma_unmap_sg(device->dev, data->sg, data->sg_len,
		     mmc_get_dma_dir(data));
	return -ENOMEM;
}

<<<<<<< HEAD
static inline int mmci_dma_prep_data(struct mmci_host *host,
				     struct mmc_data *data)
{
	/* Check if next job is already prepared. */
	if (host->dma_current && host->dma_desc_current)
		return 0;

	/* No job were prepared thus do it now. */
	return __mmci_dma_prep_data(host, data, &host->dma_current,
				    &host->dma_desc_current);
}

static inline int mmci_dma_prep_next(struct mmci_host *host,
				     struct mmc_data *data)
{
	struct mmci_host_next *nd = &host->next_data;
	return __mmci_dma_prep_data(host, data, &nd->dma_chan, &nd->dma_desc);
}

static int mmci_dma_start_data(struct mmci_host *host, unsigned int datactrl)
{
	int ret;
	struct mmc_data *data = host->data;

	ret = mmci_dma_prep_data(host, host->data);
	if (ret)
		return ret;

	/* Okay, go for it. */
	dev_vdbg(mmc_dev(host->mmc),
		 "Submit MMCI DMA job, sglen %d blksz %04x blks %04x flags %08x\n",
		 data->sg_len, data->blksz, data->blocks, data->flags);
	host->dma_in_progress = true;
	dmaengine_submit(host->dma_desc_current);
	dma_async_issue_pending(host->dma_current);

	if (host->variant->qcom_dml)
		dml_start_xfer(host, data);

	datactrl |= MCI_DPSM_DMAENABLE;

	/* Trigger the DMA transfer */
	mmci_write_datactrlreg(host, datactrl);

	/*
	 * Let the MMCI say when the data is ended and it's time
	 * to fire next DMA request. When that happens, MMCI will
	 * call mmci_data_end()
	 */
	writel(readl(host->base + MMCIMASK0) | MCI_DATAENDMASK,
	       host->base + MMCIMASK0);
	return 0;
}

static void mmci_get_next_data(struct mmci_host *host, struct mmc_data *data)
{
	struct mmci_host_next *next = &host->next_data;

	WARN_ON(data->host_cookie && data->host_cookie != next->cookie);
	WARN_ON(!data->host_cookie && (next->dma_desc || next->dma_chan));

	host->dma_desc_current = next->dma_desc;
	host->dma_current = next->dma_chan;
	next->dma_desc = NULL;
	next->dma_chan = NULL;
=======
int mmci_dmae_prep_data(struct mmci_host *host,
			struct mmc_data *data,
			bool next)
{
	struct mmci_dmae_priv *dmae = host->dma_priv;
	struct mmci_dmae_next *nd = &dmae->next_data;

	if (!host->use_dma)
		return -EINVAL;

	if (next)
		return _mmci_dmae_prep_data(host, data, &nd->chan, &nd->desc);
	/* Check if next job is already prepared. */
	if (dmae->cur && dmae->desc_current)
		return 0;

	/* No job were prepared thus do it now. */
	return _mmci_dmae_prep_data(host, data, &dmae->cur,
				    &dmae->desc_current);
}

int mmci_dmae_start(struct mmci_host *host, unsigned int *datactrl)
{
	struct mmci_dmae_priv *dmae = host->dma_priv;
	int ret;

	host->dma_in_progress = true;
	ret = dma_submit_error(dmaengine_submit(dmae->desc_current));
	if (ret < 0) {
		host->dma_in_progress = false;
		return ret;
	}
	dma_async_issue_pending(dmae->cur);

	*datactrl |= MCI_DPSM_DMAENABLE;

	return 0;
}

void mmci_dmae_get_next_data(struct mmci_host *host, struct mmc_data *data)
{
	struct mmci_dmae_priv *dmae = host->dma_priv;
	struct mmci_dmae_next *next = &dmae->next_data;

	if (!host->use_dma)
		return;

	WARN_ON(!data->host_cookie && (next->desc || next->chan));

	dmae->desc_current = next->desc;
	dmae->cur = next->chan;
	next->desc = NULL;
	next->chan = NULL;
}

void mmci_dmae_unprep_data(struct mmci_host *host,
			   struct mmc_data *data, int err)

{
	struct mmci_dmae_priv *dmae = host->dma_priv;

	if (!host->use_dma)
		return;

	mmci_dma_unmap(host, data);

	if (err) {
		struct mmci_dmae_next *next = &dmae->next_data;
		struct dma_chan *chan;
		if (data->flags & MMC_DATA_READ)
			chan = dmae->rx_channel;
		else
			chan = dmae->tx_channel;
		dmaengine_terminate_all(chan);

		if (dmae->desc_current == next->desc)
			dmae->desc_current = NULL;

		if (dmae->cur == next->chan) {
			host->dma_in_progress = false;
			dmae->cur = NULL;
		}

		next->desc = NULL;
		next->chan = NULL;
	}
}

static struct mmci_host_ops mmci_variant_ops = {
	.prep_data = mmci_dmae_prep_data,
	.unprep_data = mmci_dmae_unprep_data,
	.get_datactrl_cfg = mmci_get_dctrl_cfg,
	.get_next_data = mmci_dmae_get_next_data,
	.dma_setup = mmci_dmae_setup,
	.dma_release = mmci_dmae_release,
	.dma_start = mmci_dmae_start,
	.dma_finalize = mmci_dmae_finalize,
	.dma_error = mmci_dmae_error,
};
#else
static struct mmci_host_ops mmci_variant_ops = {
	.get_datactrl_cfg = mmci_get_dctrl_cfg,
};
#endif

static void mmci_variant_init(struct mmci_host *host)
{
	host->ops = &mmci_variant_ops;
}

static void ux500_variant_init(struct mmci_host *host)
{
	host->ops = &mmci_variant_ops;
	host->ops->busy_complete = ux500_busy_complete;
}

static void ux500v2_variant_init(struct mmci_host *host)
{
	host->ops = &mmci_variant_ops;
	host->ops->busy_complete = ux500_busy_complete;
	host->ops->get_datactrl_cfg = ux500v2_get_dctrl_cfg;
>>>>>>> upstream/android-13
}

static void mmci_pre_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	struct mmci_host *host = mmc_priv(mmc);
	struct mmc_data *data = mrq->data;
<<<<<<< HEAD
	struct mmci_host_next *nd = &host->next_data;
=======
>>>>>>> upstream/android-13

	if (!data)
		return;

<<<<<<< HEAD
	BUG_ON(data->host_cookie);
=======
	WARN_ON(data->host_cookie);
>>>>>>> upstream/android-13

	if (mmci_validate_data(host, data))
		return;

<<<<<<< HEAD
	if (!mmci_dma_prep_next(host, data))
		data->host_cookie = ++nd->cookie < 0 ? 1 : nd->cookie;
=======
	mmci_prep_data(host, data, true);
>>>>>>> upstream/android-13
}

static void mmci_post_request(struct mmc_host *mmc, struct mmc_request *mrq,
			      int err)
{
	struct mmci_host *host = mmc_priv(mmc);
	struct mmc_data *data = mrq->data;

	if (!data || !data->host_cookie)
		return;

<<<<<<< HEAD
	mmci_dma_unmap(host, data);

	if (err) {
		struct mmci_host_next *next = &host->next_data;
		struct dma_chan *chan;
		if (data->flags & MMC_DATA_READ)
			chan = host->dma_rx_channel;
		else
			chan = host->dma_tx_channel;
		dmaengine_terminate_all(chan);

		if (host->dma_desc_current == next->dma_desc)
			host->dma_desc_current = NULL;

		if (host->dma_current == next->dma_chan) {
			host->dma_in_progress = false;
			host->dma_current = NULL;
		}

		next->dma_desc = NULL;
		next->dma_chan = NULL;
		data->host_cookie = 0;
	}
}

#else
/* Blank functions if the DMA engine is not available */
static void mmci_get_next_data(struct mmci_host *host, struct mmc_data *data)
{
}
static inline void mmci_dma_setup(struct mmci_host *host)
{
}

static inline void mmci_dma_release(struct mmci_host *host)
{
}

static inline void mmci_dma_unmap(struct mmci_host *host, struct mmc_data *data)
{
}

static inline void mmci_dma_finalize(struct mmci_host *host,
				     struct mmc_data *data)
{
}

static inline void mmci_dma_data_error(struct mmci_host *host)
{
}

static inline int mmci_dma_start_data(struct mmci_host *host, unsigned int datactrl)
{
	return -ENOSYS;
}

#define mmci_pre_request NULL
#define mmci_post_request NULL

#endif

=======
	mmci_unprep_data(host, data, err);
}

>>>>>>> upstream/android-13
static void mmci_start_data(struct mmci_host *host, struct mmc_data *data)
{
	struct variant_data *variant = host->variant;
	unsigned int datactrl, timeout, irqmask;
	unsigned long long clks;
	void __iomem *base;
<<<<<<< HEAD
	int blksz_bits;
=======
>>>>>>> upstream/android-13

	dev_dbg(mmc_dev(host->mmc), "blksz %04x blks %04x flags %08x\n",
		data->blksz, data->blocks, data->flags);

	host->data = data;
	host->size = data->blksz * data->blocks;
	data->bytes_xfered = 0;

	clks = (unsigned long long)data->timeout_ns * host->cclk;
	do_div(clks, NSEC_PER_SEC);

	timeout = data->timeout_clks + (unsigned int)clks;

	base = host->base;
	writel(timeout, base + MMCIDATATIMER);
	writel(host->size, base + MMCIDATALENGTH);

<<<<<<< HEAD
	blksz_bits = ffs(data->blksz) - 1;
	BUG_ON(1 << blksz_bits != data->blksz);

	if (variant->blksz_datactrl16)
		datactrl = MCI_DPSM_ENABLE | (data->blksz << 16);
	else if (variant->blksz_datactrl4)
		datactrl = MCI_DPSM_ENABLE | (data->blksz << 4);
	else
		datactrl = MCI_DPSM_ENABLE | blksz_bits << 4;

	if (data->flags & MMC_DATA_READ)
		datactrl |= MCI_DPSM_DIRECTION;
=======
	datactrl = host->ops->get_datactrl_cfg(host);
	datactrl |= host->data->flags & MMC_DATA_READ ? MCI_DPSM_DIRECTION : 0;
>>>>>>> upstream/android-13

	if (host->mmc->card && mmc_card_sdio(host->mmc->card)) {
		u32 clk;

		datactrl |= variant->datactrl_mask_sdio;

		/*
		 * The ST Micro variant for SDIO small write transfers
		 * needs to have clock H/W flow control disabled,
		 * otherwise the transfer will not start. The threshold
		 * depends on the rate of MCLK.
		 */
		if (variant->st_sdio && data->flags & MMC_DATA_WRITE &&
		    (host->size < 8 ||
		     (host->size <= 8 && host->mclk > 50000000)))
			clk = host->clk_reg & ~variant->clkreg_enable;
		else
			clk = host->clk_reg | variant->clkreg_enable;

		mmci_write_clkreg(host, clk);
	}

	if (host->mmc->ios.timing == MMC_TIMING_UHS_DDR50 ||
	    host->mmc->ios.timing == MMC_TIMING_MMC_DDR52)
		datactrl |= variant->datactrl_mask_ddrmode;

	/*
	 * Attempt to use DMA operation mode, if this
	 * should fail, fall back to PIO mode
	 */
<<<<<<< HEAD
	if (!mmci_dma_start_data(host, datactrl))
=======
	if (!mmci_dma_start(host, datactrl))
>>>>>>> upstream/android-13
		return;

	/* IRQ mode, map the SG list for CPU reading/writing */
	mmci_init_sg(host, data);

	if (data->flags & MMC_DATA_READ) {
		irqmask = MCI_RXFIFOHALFFULLMASK;

		/*
		 * If we have less than the fifo 'half-full' threshold to
		 * transfer, trigger a PIO interrupt as soon as any data
		 * is available.
		 */
		if (host->size < variant->fifohalfsize)
			irqmask |= MCI_RXDATAAVLBLMASK;
	} else {
		/*
		 * We don't actually need to include "FIFO empty" here
		 * since its implicit in "FIFO half empty".
		 */
		irqmask = MCI_TXFIFOHALFEMPTYMASK;
	}

	mmci_write_datactrlreg(host, datactrl);
	writel(readl(base + MMCIMASK0) & ~MCI_DATAENDMASK, base + MMCIMASK0);
	mmci_set_mask1(host, irqmask);
}

static void
mmci_start_command(struct mmci_host *host, struct mmc_command *cmd, u32 c)
{
	void __iomem *base = host->base;
<<<<<<< HEAD
=======
	unsigned long long clks;
>>>>>>> upstream/android-13

	dev_dbg(mmc_dev(host->mmc), "op %02x arg %08x flags %08x\n",
	    cmd->opcode, cmd->arg, cmd->flags);

<<<<<<< HEAD
	if (readl(base + MMCICOMMAND) & MCI_CPSM_ENABLE) {
=======
	if (readl(base + MMCICOMMAND) & host->variant->cmdreg_cpsm_enable) {
>>>>>>> upstream/android-13
		writel(0, base + MMCICOMMAND);
		mmci_reg_delay(host);
	}

<<<<<<< HEAD
	c |= cmd->opcode | MCI_CPSM_ENABLE;
	if (cmd->flags & MMC_RSP_PRESENT) {
		if (cmd->flags & MMC_RSP_136)
			c |= MCI_CPSM_LONGRSP;
		c |= MCI_CPSM_RESPONSE;
	}
=======
	if (host->variant->cmdreg_stop &&
	    cmd->opcode == MMC_STOP_TRANSMISSION)
		c |= host->variant->cmdreg_stop;

	c |= cmd->opcode | host->variant->cmdreg_cpsm_enable;
	if (cmd->flags & MMC_RSP_PRESENT) {
		if (cmd->flags & MMC_RSP_136)
			c |= host->variant->cmdreg_lrsp_crc;
		else if (cmd->flags & MMC_RSP_CRC)
			c |= host->variant->cmdreg_srsp_crc;
		else
			c |= host->variant->cmdreg_srsp;
	}

	if (host->variant->busy_timeout && cmd->flags & MMC_RSP_BUSY) {
		if (!cmd->busy_timeout)
			cmd->busy_timeout = 10 * MSEC_PER_SEC;

		if (cmd->busy_timeout > host->mmc->max_busy_timeout)
			clks = (unsigned long long)host->mmc->max_busy_timeout * host->cclk;
		else
			clks = (unsigned long long)cmd->busy_timeout * host->cclk;

		do_div(clks, MSEC_PER_SEC);
		writel_relaxed(clks, host->base + MMCIDATATIMER);
	}

	if (host->ops->pre_sig_volt_switch && cmd->opcode == SD_SWITCH_VOLTAGE)
		host->ops->pre_sig_volt_switch(host);

>>>>>>> upstream/android-13
	if (/*interrupt*/0)
		c |= MCI_CPSM_INTERRUPT;

	if (mmc_cmd_type(cmd) == MMC_CMD_ADTC)
		c |= host->variant->data_cmd_enable;

	host->cmd = cmd;

	writel(cmd->arg, base + MMCIARGUMENT);
	writel(c, base + MMCICOMMAND);
}

<<<<<<< HEAD
=======
static void mmci_stop_command(struct mmci_host *host)
{
	host->stop_abort.error = 0;
	mmci_start_command(host, &host->stop_abort, 0);
}

>>>>>>> upstream/android-13
static void
mmci_data_irq(struct mmci_host *host, struct mmc_data *data,
	      unsigned int status)
{
	unsigned int status_err;

	/* Make sure we have data to handle */
	if (!data)
		return;

	/* First check for errors */
	status_err = status & (host->variant->start_err |
			       MCI_DATACRCFAIL | MCI_DATATIMEOUT |
			       MCI_TXUNDERRUN | MCI_RXOVERRUN);

	if (status_err) {
		u32 remain, success;

		/* Terminate the DMA transfer */
<<<<<<< HEAD
		if (dma_inprogress(host)) {
			mmci_dma_data_error(host);
			mmci_dma_unmap(host, data);
		}
=======
		mmci_dma_error(host);
>>>>>>> upstream/android-13

		/*
		 * Calculate how far we are into the transfer.  Note that
		 * the data counter gives the number of bytes transferred
		 * on the MMC bus, not on the host side.  On reads, this
		 * can be as much as a FIFO-worth of data ahead.  This
		 * matters for FIFO overruns only.
		 */
<<<<<<< HEAD
		remain = readl(host->base + MMCIDATACNT);
		success = data->blksz * data->blocks - remain;
=======
		if (!host->variant->datacnt_useless) {
			remain = readl(host->base + MMCIDATACNT);
			success = data->blksz * data->blocks - remain;
		} else {
			success = 0;
		}
>>>>>>> upstream/android-13

		dev_dbg(mmc_dev(host->mmc), "MCI ERROR IRQ, status 0x%08x at 0x%08x\n",
			status_err, success);
		if (status_err & MCI_DATACRCFAIL) {
			/* Last block was not successful */
			success -= 1;
			data->error = -EILSEQ;
		} else if (status_err & MCI_DATATIMEOUT) {
			data->error = -ETIMEDOUT;
		} else if (status_err & MCI_STARTBITERR) {
			data->error = -ECOMM;
		} else if (status_err & MCI_TXUNDERRUN) {
			data->error = -EIO;
		} else if (status_err & MCI_RXOVERRUN) {
			if (success > host->variant->fifosize)
				success -= host->variant->fifosize;
			else
				success = 0;
			data->error = -EIO;
		}
		data->bytes_xfered = round_down(success, data->blksz);
	}

	if (status & MCI_DATABLOCKEND)
		dev_err(mmc_dev(host->mmc), "stray MCI_DATABLOCKEND interrupt\n");

	if (status & MCI_DATAEND || data->error) {
<<<<<<< HEAD
		if (dma_inprogress(host))
			mmci_dma_finalize(host, data);
=======
		mmci_dma_finalize(host, data);

>>>>>>> upstream/android-13
		mmci_stop_data(host);

		if (!data->error)
			/* The error clause is handled above, success! */
			data->bytes_xfered = data->blksz * data->blocks;

<<<<<<< HEAD
		if (!data->stop || host->mrq->sbc) {
=======
		if (!data->stop) {
			if (host->variant->cmdreg_stop && data->error)
				mmci_stop_command(host);
			else
				mmci_request_end(host, data->mrq);
		} else if (host->mrq->sbc && !data->error) {
>>>>>>> upstream/android-13
			mmci_request_end(host, data->mrq);
		} else {
			mmci_start_command(host, data->stop, 0);
		}
	}
}

static void
mmci_cmd_irq(struct mmci_host *host, struct mmc_command *cmd,
	     unsigned int status)
{
<<<<<<< HEAD
	void __iomem *base = host->base;
	bool sbc;
=======
	u32 err_msk = MCI_CMDCRCFAIL | MCI_CMDTIMEOUT;
	void __iomem *base = host->base;
	bool sbc, busy_resp;
>>>>>>> upstream/android-13

	if (!cmd)
		return;

	sbc = (cmd == host->mrq->sbc);
<<<<<<< HEAD
=======
	busy_resp = !!(cmd->flags & MMC_RSP_BUSY);
>>>>>>> upstream/android-13

	/*
	 * We need to be one of these interrupts to be considered worth
	 * handling. Note that we tag on any latent IRQs postponed
	 * due to waiting for busy status.
	 */
<<<<<<< HEAD
	if (!((status|host->busy_status) &
	      (MCI_CMDCRCFAIL|MCI_CMDTIMEOUT|MCI_CMDSENT|MCI_CMDRESPEND)))
		return;

	/*
	 * ST Micro variant: handle busy detection.
	 */
	if (host->variant->busy_detect) {
		bool busy_resp = !!(cmd->flags & MMC_RSP_BUSY);

		/* We are busy with a command, return */
		if (host->busy_status &&
		    (status & host->variant->busy_detect_flag))
			return;

		/*
		 * We were not busy, but we now got a busy response on
		 * something that was not an error, and we double-check
		 * that the special busy status bit is still set before
		 * proceeding.
		 */
		if (!host->busy_status && busy_resp &&
		    !(status & (MCI_CMDCRCFAIL|MCI_CMDTIMEOUT)) &&
		    (readl(base + MMCISTATUS) & host->variant->busy_detect_flag)) {

			/* Clear the busy start IRQ */
			writel(host->variant->busy_detect_mask,
			       host->base + MMCICLEAR);

			/* Unmask the busy end IRQ */
			writel(readl(base + MMCIMASK0) |
			       host->variant->busy_detect_mask,
			       base + MMCIMASK0);
			/*
			 * Now cache the last response status code (until
			 * the busy bit goes low), and return.
			 */
			host->busy_status =
				status & (MCI_CMDSENT|MCI_CMDRESPEND);
			return;
		}

		/*
		 * At this point we are not busy with a command, we have
		 * not received a new busy request, clear and mask the busy
		 * end IRQ and fall through to process the IRQ.
		 */
		if (host->busy_status) {

			writel(host->variant->busy_detect_mask,
			       host->base + MMCICLEAR);

			writel(readl(base + MMCIMASK0) &
			       ~host->variant->busy_detect_mask,
			       base + MMCIMASK0);
			host->busy_status = 0;
		}
	}

=======
	if (host->variant->busy_timeout && busy_resp)
		err_msk |= MCI_DATATIMEOUT;

	if (!((status | host->busy_status) &
	      (err_msk | MCI_CMDSENT | MCI_CMDRESPEND)))
		return;

	/* Handle busy detection on DAT0 if the variant supports it. */
	if (busy_resp && host->variant->busy_detect)
		if (!host->ops->busy_complete(host, status, err_msk))
			return;

>>>>>>> upstream/android-13
	host->cmd = NULL;

	if (status & MCI_CMDTIMEOUT) {
		cmd->error = -ETIMEDOUT;
	} else if (status & MCI_CMDCRCFAIL && cmd->flags & MMC_RSP_CRC) {
		cmd->error = -EILSEQ;
<<<<<<< HEAD
=======
	} else if (host->variant->busy_timeout && busy_resp &&
		   status & MCI_DATATIMEOUT) {
		cmd->error = -ETIMEDOUT;
		host->irq_action = IRQ_WAKE_THREAD;
>>>>>>> upstream/android-13
	} else {
		cmd->resp[0] = readl(base + MMCIRESPONSE0);
		cmd->resp[1] = readl(base + MMCIRESPONSE1);
		cmd->resp[2] = readl(base + MMCIRESPONSE2);
		cmd->resp[3] = readl(base + MMCIRESPONSE3);
	}

	if ((!sbc && !cmd->data) || cmd->error) {
		if (host->data) {
			/* Terminate the DMA transfer */
<<<<<<< HEAD
			if (dma_inprogress(host)) {
				mmci_dma_data_error(host);
				mmci_dma_unmap(host, host->data);
			}
			mmci_stop_data(host);
		}
		mmci_request_end(host, host->mrq);
	} else if (sbc) {
		mmci_start_command(host, host->mrq->cmd, 0);
	} else if (!(cmd->data->flags & MMC_DATA_READ)) {
=======
			mmci_dma_error(host);

			mmci_stop_data(host);
			if (host->variant->cmdreg_stop && cmd->error) {
				mmci_stop_command(host);
				return;
			}
		}

		if (host->irq_action != IRQ_WAKE_THREAD)
			mmci_request_end(host, host->mrq);

	} else if (sbc) {
		mmci_start_command(host, host->mrq->cmd, 0);
	} else if (!host->variant->datactrl_first &&
		   !(cmd->data->flags & MMC_DATA_READ)) {
>>>>>>> upstream/android-13
		mmci_start_data(host, cmd->data);
	}
}

static int mmci_get_rx_fifocnt(struct mmci_host *host, u32 status, int remain)
{
	return remain - (readl(host->base + MMCIFIFOCNT) << 2);
}

static int mmci_qcom_get_rx_fifocnt(struct mmci_host *host, u32 status, int r)
{
	/*
	 * on qcom SDCC4 only 8 words are used in each burst so only 8 addresses
	 * from the fifo range should be used
	 */
	if (status & MCI_RXFIFOHALFFULL)
		return host->variant->fifohalfsize;
	else if (status & MCI_RXDATAAVLBL)
		return 4;

	return 0;
}

static int mmci_pio_read(struct mmci_host *host, char *buffer, unsigned int remain)
{
	void __iomem *base = host->base;
	char *ptr = buffer;
	u32 status = readl(host->base + MMCISTATUS);
	int host_remain = host->size;

	do {
		int count = host->get_rx_fifocnt(host, status, host_remain);

		if (count > remain)
			count = remain;

		if (count <= 0)
			break;

		/*
		 * SDIO especially may want to send something that is
		 * not divisible by 4 (as opposed to card sectors
		 * etc). Therefore make sure to always read the last bytes
		 * while only doing full 32-bit reads towards the FIFO.
		 */
		if (unlikely(count & 0x3)) {
			if (count < 4) {
				unsigned char buf[4];
				ioread32_rep(base + MMCIFIFO, buf, 1);
				memcpy(ptr, buf, count);
			} else {
				ioread32_rep(base + MMCIFIFO, ptr, count >> 2);
				count &= ~0x3;
			}
		} else {
			ioread32_rep(base + MMCIFIFO, ptr, count >> 2);
		}

		ptr += count;
		remain -= count;
		host_remain -= count;

		if (remain == 0)
			break;

		status = readl(base + MMCISTATUS);
	} while (status & MCI_RXDATAAVLBL);

	return ptr - buffer;
}

static int mmci_pio_write(struct mmci_host *host, char *buffer, unsigned int remain, u32 status)
{
	struct variant_data *variant = host->variant;
	void __iomem *base = host->base;
	char *ptr = buffer;

	do {
		unsigned int count, maxcnt;

		maxcnt = status & MCI_TXFIFOEMPTY ?
			 variant->fifosize : variant->fifohalfsize;
		count = min(remain, maxcnt);

		/*
		 * SDIO especially may want to send something that is
		 * not divisible by 4 (as opposed to card sectors
		 * etc), and the FIFO only accept full 32-bit writes.
		 * So compensate by adding +3 on the count, a single
		 * byte become a 32bit write, 7 bytes will be two
		 * 32bit writes etc.
		 */
		iowrite32_rep(base + MMCIFIFO, ptr, (count + 3) >> 2);

		ptr += count;
		remain -= count;

		if (remain == 0)
			break;

		status = readl(base + MMCISTATUS);
	} while (status & MCI_TXFIFOHALFEMPTY);

	return ptr - buffer;
}

/*
 * PIO data transfer IRQ handler.
 */
static irqreturn_t mmci_pio_irq(int irq, void *dev_id)
{
	struct mmci_host *host = dev_id;
	struct sg_mapping_iter *sg_miter = &host->sg_miter;
	struct variant_data *variant = host->variant;
	void __iomem *base = host->base;
	u32 status;

	status = readl(base + MMCISTATUS);

	dev_dbg(mmc_dev(host->mmc), "irq1 (pio) %08x\n", status);

	do {
		unsigned int remain, len;
		char *buffer;

		/*
		 * For write, we only need to test the half-empty flag
		 * here - if the FIFO is completely empty, then by
		 * definition it is more than half empty.
		 *
		 * For read, check for data available.
		 */
		if (!(status & (MCI_TXFIFOHALFEMPTY|MCI_RXDATAAVLBL)))
			break;

		if (!sg_miter_next(sg_miter))
			break;

		buffer = sg_miter->addr;
		remain = sg_miter->length;

		len = 0;
		if (status & MCI_RXACTIVE)
			len = mmci_pio_read(host, buffer, remain);
		if (status & MCI_TXACTIVE)
			len = mmci_pio_write(host, buffer, remain, status);

		sg_miter->consumed = len;

		host->size -= len;
		remain -= len;

		if (remain)
			break;

		status = readl(base + MMCISTATUS);
	} while (1);

	sg_miter_stop(sg_miter);

	/*
	 * If we have less than the fifo 'half-full' threshold to transfer,
	 * trigger a PIO interrupt as soon as any data is available.
	 */
	if (status & MCI_RXACTIVE && host->size < variant->fifohalfsize)
		mmci_set_mask1(host, MCI_RXDATAAVLBLMASK);

	/*
	 * If we run out of data, disable the data IRQs; this
	 * prevents a race where the FIFO becomes empty before
	 * the chip itself has disabled the data path, and
	 * stops us racing with our data end IRQ.
	 */
	if (host->size == 0) {
		mmci_set_mask1(host, 0);
		writel(readl(base + MMCIMASK0) | MCI_DATAENDMASK, base + MMCIMASK0);
	}

	return IRQ_HANDLED;
}

/*
 * Handle completion of command and data transfers.
 */
static irqreturn_t mmci_irq(int irq, void *dev_id)
{
	struct mmci_host *host = dev_id;
	u32 status;
<<<<<<< HEAD
	int ret = 0;

	spin_lock(&host->lock);
=======

	spin_lock(&host->lock);
	host->irq_action = IRQ_HANDLED;
>>>>>>> upstream/android-13

	do {
		status = readl(host->base + MMCISTATUS);

		if (host->singleirq) {
			if (status & host->mask1_reg)
				mmci_pio_irq(irq, dev_id);

<<<<<<< HEAD
			status &= ~MCI_IRQ1MASK;
		}

		/*
		 * We intentionally clear the MCI_ST_CARDBUSY IRQ (if it's
		 * enabled) in mmci_cmd_irq() function where ST Micro busy
		 * detection variant is handled. Considering the HW seems to be
		 * triggering the IRQ on both edges while monitoring DAT0 for
		 * busy completion and that same status bit is used to monitor
		 * start and end of busy detection, special care must be taken
		 * to make sure that both start and end interrupts are always
		 * cleared one after the other.
=======
			status &= ~host->variant->irq_pio_mask;
		}

		/*
		 * Busy detection is managed by mmci_cmd_irq(), including to
		 * clear the corresponding IRQ.
>>>>>>> upstream/android-13
		 */
		status &= readl(host->base + MMCIMASK0);
		if (host->variant->busy_detect)
			writel(status & ~host->variant->busy_detect_mask,
			       host->base + MMCICLEAR);
		else
			writel(status, host->base + MMCICLEAR);

		dev_dbg(mmc_dev(host->mmc), "irq0 (data+cmd) %08x\n", status);

		if (host->variant->reversed_irq_handling) {
			mmci_data_irq(host, host->data, status);
			mmci_cmd_irq(host, host->cmd, status);
		} else {
			mmci_cmd_irq(host, host->cmd, status);
			mmci_data_irq(host, host->data, status);
		}

		/*
		 * Busy detection has been handled by mmci_cmd_irq() above.
		 * Clear the status bit to prevent polling in IRQ context.
		 */
		if (host->variant->busy_detect_flag)
			status &= ~host->variant->busy_detect_flag;

<<<<<<< HEAD
		ret = 1;
=======
>>>>>>> upstream/android-13
	} while (status);

	spin_unlock(&host->lock);

<<<<<<< HEAD
	return IRQ_RETVAL(ret);
=======
	return host->irq_action;
}

/*
 * mmci_irq_thread() - A threaded IRQ handler that manages a reset of the HW.
 *
 * A reset is needed for some variants, where a datatimeout for a R1B request
 * causes the DPSM to stay busy (non-functional).
 */
static irqreturn_t mmci_irq_thread(int irq, void *dev_id)
{
	struct mmci_host *host = dev_id;
	unsigned long flags;

	if (host->rst) {
		reset_control_assert(host->rst);
		udelay(2);
		reset_control_deassert(host->rst);
	}

	spin_lock_irqsave(&host->lock, flags);
	writel(host->clk_reg, host->base + MMCICLOCK);
	writel(host->pwr_reg, host->base + MMCIPOWER);
	writel(MCI_IRQENABLE | host->variant->start_err,
	       host->base + MMCIMASK0);

	host->irq_action = IRQ_HANDLED;
	mmci_request_end(host, host->mrq);
	spin_unlock_irqrestore(&host->lock, flags);

	return host->irq_action;
>>>>>>> upstream/android-13
}

static void mmci_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	struct mmci_host *host = mmc_priv(mmc);
	unsigned long flags;

	WARN_ON(host->mrq != NULL);

	mrq->cmd->error = mmci_validate_data(host, mrq->data);
	if (mrq->cmd->error) {
		mmc_request_done(mmc, mrq);
		return;
	}

	spin_lock_irqsave(&host->lock, flags);

	host->mrq = mrq;

	if (mrq->data)
		mmci_get_next_data(host, mrq->data);

<<<<<<< HEAD
	if (mrq->data && mrq->data->flags & MMC_DATA_READ)
=======
	if (mrq->data &&
	    (host->variant->datactrl_first || mrq->data->flags & MMC_DATA_READ))
>>>>>>> upstream/android-13
		mmci_start_data(host, mrq->data);

	if (mrq->sbc)
		mmci_start_command(host, mrq->sbc, 0);
	else
		mmci_start_command(host, mrq->cmd, 0);

	spin_unlock_irqrestore(&host->lock, flags);
}

<<<<<<< HEAD
=======
static void mmci_set_max_busy_timeout(struct mmc_host *mmc)
{
	struct mmci_host *host = mmc_priv(mmc);
	u32 max_busy_timeout = 0;

	if (!host->variant->busy_detect)
		return;

	if (host->variant->busy_timeout && mmc->actual_clock)
		max_busy_timeout = ~0UL / (mmc->actual_clock / MSEC_PER_SEC);

	mmc->max_busy_timeout = max_busy_timeout;
}

>>>>>>> upstream/android-13
static void mmci_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	struct mmci_host *host = mmc_priv(mmc);
	struct variant_data *variant = host->variant;
	u32 pwr = 0;
	unsigned long flags;
	int ret;

	if (host->plat->ios_handler &&
		host->plat->ios_handler(mmc_dev(mmc), ios))
			dev_err(mmc_dev(mmc), "platform ios_handler failed\n");

	switch (ios->power_mode) {
	case MMC_POWER_OFF:
		if (!IS_ERR(mmc->supply.vmmc))
			mmc_regulator_set_ocr(mmc, mmc->supply.vmmc, 0);

		if (!IS_ERR(mmc->supply.vqmmc) && host->vqmmc_enabled) {
			regulator_disable(mmc->supply.vqmmc);
			host->vqmmc_enabled = false;
		}

		break;
	case MMC_POWER_UP:
		if (!IS_ERR(mmc->supply.vmmc))
			mmc_regulator_set_ocr(mmc, mmc->supply.vmmc, ios->vdd);

		/*
		 * The ST Micro variant doesn't have the PL180s MCI_PWR_UP
		 * and instead uses MCI_PWR_ON so apply whatever value is
		 * configured in the variant data.
		 */
		pwr |= variant->pwrreg_powerup;

		break;
	case MMC_POWER_ON:
		if (!IS_ERR(mmc->supply.vqmmc) && !host->vqmmc_enabled) {
			ret = regulator_enable(mmc->supply.vqmmc);
			if (ret < 0)
				dev_err(mmc_dev(mmc),
					"failed to enable vqmmc regulator\n");
			else
				host->vqmmc_enabled = true;
		}

		pwr |= MCI_PWR_ON;
		break;
	}

	if (variant->signal_direction && ios->power_mode != MMC_POWER_OFF) {
		/*
		 * The ST Micro variant has some additional bits
		 * indicating signal direction for the signals in
		 * the SD/MMC bus and feedback-clock usage.
		 */
		pwr |= host->pwr_reg_add;

		if (ios->bus_width == MMC_BUS_WIDTH_4)
			pwr &= ~MCI_ST_DATA74DIREN;
		else if (ios->bus_width == MMC_BUS_WIDTH_1)
			pwr &= (~MCI_ST_DATA74DIREN &
				~MCI_ST_DATA31DIREN &
				~MCI_ST_DATA2DIREN);
	}

	if (variant->opendrain) {
		if (ios->bus_mode == MMC_BUSMODE_OPENDRAIN)
			pwr |= variant->opendrain;
	} else {
		/*
		 * If the variant cannot configure the pads by its own, then we
		 * expect the pinctrl to be able to do that for us
		 */
		if (ios->bus_mode == MMC_BUSMODE_OPENDRAIN)
			pinctrl_select_state(host->pinctrl, host->pins_opendrain);
		else
<<<<<<< HEAD
			pinctrl_select_state(host->pinctrl, host->pins_default);
=======
			pinctrl_select_default_state(mmc_dev(mmc));
>>>>>>> upstream/android-13
	}

	/*
	 * If clock = 0 and the variant requires the MMCIPOWER to be used for
	 * gating the clock, the MCI_PWR_ON bit is cleared.
	 */
	if (!ios->clock && variant->pwrreg_clkgate)
		pwr &= ~MCI_PWR_ON;

	if (host->variant->explicit_mclk_control &&
	    ios->clock != host->clock_cache) {
		ret = clk_set_rate(host->clk, ios->clock);
		if (ret < 0)
			dev_err(mmc_dev(host->mmc),
				"Error setting clock rate (%d)\n", ret);
		else
			host->mclk = clk_get_rate(host->clk);
	}
	host->clock_cache = ios->clock;

	spin_lock_irqsave(&host->lock, flags);

<<<<<<< HEAD
	mmci_set_clkreg(host, ios->clock);
	mmci_write_pwrreg(host, pwr);
=======
	if (host->ops && host->ops->set_clkreg)
		host->ops->set_clkreg(host, ios->clock);
	else
		mmci_set_clkreg(host, ios->clock);

	mmci_set_max_busy_timeout(mmc);

	if (host->ops && host->ops->set_pwrreg)
		host->ops->set_pwrreg(host, pwr);
	else
		mmci_write_pwrreg(host, pwr);

>>>>>>> upstream/android-13
	mmci_reg_delay(host);

	spin_unlock_irqrestore(&host->lock, flags);
}

static int mmci_get_cd(struct mmc_host *mmc)
{
	struct mmci_host *host = mmc_priv(mmc);
	struct mmci_platform_data *plat = host->plat;
	unsigned int status = mmc_gpio_get_cd(mmc);

	if (status == -ENOSYS) {
		if (!plat->status)
			return 1; /* Assume always present */

		status = plat->status(mmc_dev(host->mmc));
	}
	return status;
}

static int mmci_sig_volt_switch(struct mmc_host *mmc, struct mmc_ios *ios)
{
<<<<<<< HEAD
	int ret = 0;

	if (!IS_ERR(mmc->supply.vqmmc)) {

		switch (ios->signal_voltage) {
		case MMC_SIGNAL_VOLTAGE_330:
			ret = regulator_set_voltage(mmc->supply.vqmmc,
						2700000, 3600000);
			break;
		case MMC_SIGNAL_VOLTAGE_180:
			ret = regulator_set_voltage(mmc->supply.vqmmc,
						1700000, 1950000);
			break;
		case MMC_SIGNAL_VOLTAGE_120:
			ret = regulator_set_voltage(mmc->supply.vqmmc,
						1100000, 1300000);
			break;
		}

		if (ret)
			dev_warn(mmc_dev(mmc), "Voltage switch failed\n");
	}
=======
	struct mmci_host *host = mmc_priv(mmc);
	int ret;

	ret = mmc_regulator_set_vqmmc(mmc, ios);

	if (!ret && host->ops && host->ops->post_sig_volt_switch)
		ret = host->ops->post_sig_volt_switch(host, ios);
	else if (ret)
		ret = 0;

	if (ret < 0)
		dev_warn(mmc_dev(mmc), "Voltage switch failed\n");
>>>>>>> upstream/android-13

	return ret;
}

static struct mmc_host_ops mmci_ops = {
	.request	= mmci_request,
	.pre_req	= mmci_pre_request,
	.post_req	= mmci_post_request,
	.set_ios	= mmci_set_ios,
	.get_ro		= mmc_gpio_get_ro,
	.get_cd		= mmci_get_cd,
	.start_signal_voltage_switch = mmci_sig_volt_switch,
};

<<<<<<< HEAD
=======
static void mmci_probe_level_translator(struct mmc_host *mmc)
{
	struct device *dev = mmc_dev(mmc);
	struct mmci_host *host = mmc_priv(mmc);
	struct gpio_desc *cmd_gpio;
	struct gpio_desc *ck_gpio;
	struct gpio_desc *ckin_gpio;
	int clk_hi, clk_lo;

	/*
	 * Assume the level translator is present if st,use-ckin is set.
	 * This is to cater for DTs which do not implement this test.
	 */
	host->clk_reg_add |= MCI_STM32_CLK_SELCKIN;

	cmd_gpio = gpiod_get(dev, "st,cmd", GPIOD_OUT_HIGH);
	if (IS_ERR(cmd_gpio))
		goto exit_cmd;

	ck_gpio = gpiod_get(dev, "st,ck", GPIOD_OUT_HIGH);
	if (IS_ERR(ck_gpio))
		goto exit_ck;

	ckin_gpio = gpiod_get(dev, "st,ckin", GPIOD_IN);
	if (IS_ERR(ckin_gpio))
		goto exit_ckin;

	/* All GPIOs are valid, test whether level translator works */

	/* Sample CKIN */
	clk_hi = !!gpiod_get_value(ckin_gpio);

	/* Set CK low */
	gpiod_set_value(ck_gpio, 0);

	/* Sample CKIN */
	clk_lo = !!gpiod_get_value(ckin_gpio);

	/* Tristate all */
	gpiod_direction_input(cmd_gpio);
	gpiod_direction_input(ck_gpio);

	/* Level translator is present if CK signal is propagated to CKIN */
	if (!clk_hi || clk_lo) {
		host->clk_reg_add &= ~MCI_STM32_CLK_SELCKIN;
		dev_warn(dev,
			 "Level translator inoperable, CK signal not detected on CKIN, disabling.\n");
	}

	gpiod_put(ckin_gpio);

exit_ckin:
	gpiod_put(ck_gpio);
exit_ck:
	gpiod_put(cmd_gpio);
exit_cmd:
	pinctrl_select_default_state(dev);
}

>>>>>>> upstream/android-13
static int mmci_of_parse(struct device_node *np, struct mmc_host *mmc)
{
	struct mmci_host *host = mmc_priv(mmc);
	int ret = mmc_of_parse(mmc);

	if (ret)
		return ret;

	if (of_get_property(np, "st,sig-dir-dat0", NULL))
		host->pwr_reg_add |= MCI_ST_DATA0DIREN;
	if (of_get_property(np, "st,sig-dir-dat2", NULL))
		host->pwr_reg_add |= MCI_ST_DATA2DIREN;
	if (of_get_property(np, "st,sig-dir-dat31", NULL))
		host->pwr_reg_add |= MCI_ST_DATA31DIREN;
	if (of_get_property(np, "st,sig-dir-dat74", NULL))
		host->pwr_reg_add |= MCI_ST_DATA74DIREN;
	if (of_get_property(np, "st,sig-dir-cmd", NULL))
		host->pwr_reg_add |= MCI_ST_CMDDIREN;
	if (of_get_property(np, "st,sig-pin-fbclk", NULL))
		host->pwr_reg_add |= MCI_ST_FBCLKEN;
<<<<<<< HEAD
=======
	if (of_get_property(np, "st,sig-dir", NULL))
		host->pwr_reg_add |= MCI_STM32_DIRPOL;
	if (of_get_property(np, "st,neg-edge", NULL))
		host->clk_reg_add |= MCI_STM32_CLK_NEGEDGE;
	if (of_get_property(np, "st,use-ckin", NULL))
		mmci_probe_level_translator(mmc);
>>>>>>> upstream/android-13

	if (of_get_property(np, "mmc-cap-mmc-highspeed", NULL))
		mmc->caps |= MMC_CAP_MMC_HIGHSPEED;
	if (of_get_property(np, "mmc-cap-sd-highspeed", NULL))
		mmc->caps |= MMC_CAP_SD_HIGHSPEED;

	return 0;
}

static int mmci_probe(struct amba_device *dev,
	const struct amba_id *id)
{
	struct mmci_platform_data *plat = dev->dev.platform_data;
	struct device_node *np = dev->dev.of_node;
	struct variant_data *variant = id->data;
	struct mmci_host *host;
	struct mmc_host *mmc;
	int ret;

	/* Must have platform data or Device Tree. */
	if (!plat && !np) {
		dev_err(&dev->dev, "No plat data or DT found\n");
		return -EINVAL;
	}

	if (!plat) {
		plat = devm_kzalloc(&dev->dev, sizeof(*plat), GFP_KERNEL);
		if (!plat)
			return -ENOMEM;
	}

	mmc = mmc_alloc_host(sizeof(struct mmci_host), &dev->dev);
	if (!mmc)
		return -ENOMEM;

<<<<<<< HEAD
=======
	host = mmc_priv(mmc);
	host->mmc = mmc;
	host->mmc_ops = &mmci_ops;
	mmc->ops = &mmci_ops;

>>>>>>> upstream/android-13
	ret = mmci_of_parse(np, mmc);
	if (ret)
		goto host_free;

<<<<<<< HEAD
	host = mmc_priv(mmc);
	host->mmc = mmc;

=======
>>>>>>> upstream/android-13
	/*
	 * Some variant (STM32) doesn't have opendrain bit, nevertheless
	 * pins can be set accordingly using pinctrl
	 */
	if (!variant->opendrain) {
		host->pinctrl = devm_pinctrl_get(&dev->dev);
		if (IS_ERR(host->pinctrl)) {
			dev_err(&dev->dev, "failed to get pinctrl");
			ret = PTR_ERR(host->pinctrl);
			goto host_free;
		}

<<<<<<< HEAD
		host->pins_default = pinctrl_lookup_state(host->pinctrl,
							  PINCTRL_STATE_DEFAULT);
		if (IS_ERR(host->pins_default)) {
			dev_err(mmc_dev(mmc), "Can't select default pins\n");
			ret = PTR_ERR(host->pins_default);
			goto host_free;
		}

=======
>>>>>>> upstream/android-13
		host->pins_opendrain = pinctrl_lookup_state(host->pinctrl,
							    MMCI_PINCTRL_STATE_OPENDRAIN);
		if (IS_ERR(host->pins_opendrain)) {
			dev_err(mmc_dev(mmc), "Can't select opendrain pins\n");
			ret = PTR_ERR(host->pins_opendrain);
			goto host_free;
		}
	}

	host->hw_designer = amba_manf(dev);
	host->hw_revision = amba_rev(dev);
	dev_dbg(mmc_dev(mmc), "designer ID = 0x%02x\n", host->hw_designer);
	dev_dbg(mmc_dev(mmc), "revision = 0x%01x\n", host->hw_revision);

	host->clk = devm_clk_get(&dev->dev, NULL);
	if (IS_ERR(host->clk)) {
		ret = PTR_ERR(host->clk);
		goto host_free;
	}

	ret = clk_prepare_enable(host->clk);
	if (ret)
		goto host_free;

	if (variant->qcom_fifo)
		host->get_rx_fifocnt = mmci_qcom_get_rx_fifocnt;
	else
		host->get_rx_fifocnt = mmci_get_rx_fifocnt;

	host->plat = plat;
	host->variant = variant;
	host->mclk = clk_get_rate(host->clk);
	/*
	 * According to the spec, mclk is max 100 MHz,
	 * so we try to adjust the clock down to this,
	 * (if possible).
	 */
	if (host->mclk > variant->f_max) {
		ret = clk_set_rate(host->clk, variant->f_max);
		if (ret < 0)
			goto clk_disable;
		host->mclk = clk_get_rate(host->clk);
		dev_dbg(mmc_dev(mmc), "eventual mclk rate: %u Hz\n",
			host->mclk);
	}

	host->phybase = dev->res.start;
	host->base = devm_ioremap_resource(&dev->dev, &dev->res);
	if (IS_ERR(host->base)) {
		ret = PTR_ERR(host->base);
		goto clk_disable;
	}

	if (variant->init)
		variant->init(host);

	/*
	 * The ARM and ST versions of the block have slightly different
	 * clock divider equations which means that the minimum divider
	 * differs too.
	 * on Qualcomm like controllers get the nearest minimum clock to 100Khz
	 */
	if (variant->st_clkdiv)
		mmc->f_min = DIV_ROUND_UP(host->mclk, 257);
<<<<<<< HEAD
=======
	else if (variant->stm32_clkdiv)
		mmc->f_min = DIV_ROUND_UP(host->mclk, 2046);
>>>>>>> upstream/android-13
	else if (variant->explicit_mclk_control)
		mmc->f_min = clk_round_rate(host->clk, 100000);
	else
		mmc->f_min = DIV_ROUND_UP(host->mclk, 512);
	/*
	 * If no maximum operating frequency is supplied, fall back to use
	 * the module parameter, which has a (low) default value in case it
	 * is not specified. Either value must not exceed the clock rate into
	 * the block, of course.
	 */
	if (mmc->f_max)
		mmc->f_max = variant->explicit_mclk_control ?
				min(variant->f_max, mmc->f_max) :
				min(host->mclk, mmc->f_max);
	else
		mmc->f_max = variant->explicit_mclk_control ?
				fmax : min(host->mclk, fmax);


	dev_dbg(mmc_dev(mmc), "clocking block at %u Hz\n", mmc->f_max);

<<<<<<< HEAD
=======
	host->rst = devm_reset_control_get_optional_exclusive(&dev->dev, NULL);
	if (IS_ERR(host->rst)) {
		ret = PTR_ERR(host->rst);
		goto clk_disable;
	}
	ret = reset_control_deassert(host->rst);
	if (ret)
		dev_err(mmc_dev(mmc), "failed to de-assert reset\n");

>>>>>>> upstream/android-13
	/* Get regulators and the supported OCR mask */
	ret = mmc_regulator_get_supply(mmc);
	if (ret)
		goto clk_disable;

	if (!mmc->ocr_avail)
		mmc->ocr_avail = plat->ocr_mask;
	else if (plat->ocr_mask)
		dev_warn(mmc_dev(mmc), "Platform OCR mask is ignored\n");

<<<<<<< HEAD
	/* DT takes precedence over platform data. */
	if (!np) {
		if (!plat->cd_invert)
			mmc->caps2 |= MMC_CAP2_CD_ACTIVE_HIGH;
		mmc->caps2 |= MMC_CAP2_RO_ACTIVE_HIGH;
	}

=======
>>>>>>> upstream/android-13
	/* We support these capabilities. */
	mmc->caps |= MMC_CAP_CMD23;

	/*
	 * Enable busy detection.
	 */
	if (variant->busy_detect) {
		mmci_ops.card_busy = mmci_card_busy;
		/*
		 * Not all variants have a flag to enable busy detection
		 * in the DPSM, but if they do, set it here.
		 */
		if (variant->busy_dpsm_flag)
			mmci_write_datactrlreg(host,
					       host->variant->busy_dpsm_flag);
		mmc->caps |= MMC_CAP_WAIT_WHILE_BUSY;
<<<<<<< HEAD
		mmc->max_busy_timeout = 0;
	}

	mmc->ops = &mmci_ops;
=======
	}

	/* Variants with mandatory busy timeout in HW needs R1B responses. */
	if (variant->busy_timeout)
		mmc->caps |= MMC_CAP_NEED_RSP_BUSY;

	/* Prepare a CMD12 - needed to clear the DPSM on some variants. */
	host->stop_abort.opcode = MMC_STOP_TRANSMISSION;
	host->stop_abort.arg = 0;
	host->stop_abort.flags = MMC_RSP_R1B | MMC_CMD_AC;
>>>>>>> upstream/android-13

	/* We support these PM capabilities. */
	mmc->pm_caps |= MMC_PM_KEEP_POWER;

	/*
	 * We can do SGIO
	 */
	mmc->max_segs = NR_SG;

	/*
	 * Since only a certain number of bits are valid in the data length
	 * register, we must ensure that we don't exceed 2^num-1 bytes in a
	 * single request.
	 */
	mmc->max_req_size = (1 << variant->datalength_bits) - 1;

	/*
	 * Set the maximum segment size.  Since we aren't doing DMA
	 * (yet) we are only limited by the data length register.
	 */
	mmc->max_seg_size = mmc->max_req_size;

	/*
	 * Block size can be up to 2048 bytes, but must be a power of two.
	 */
<<<<<<< HEAD
	mmc->max_blk_size = 1 << 11;
=======
	mmc->max_blk_size = 1 << variant->datactrl_blocksz;
>>>>>>> upstream/android-13

	/*
	 * Limit the number of blocks transferred so that we don't overflow
	 * the maximum request size.
	 */
<<<<<<< HEAD
	mmc->max_blk_count = mmc->max_req_size >> 11;
=======
	mmc->max_blk_count = mmc->max_req_size >> variant->datactrl_blocksz;
>>>>>>> upstream/android-13

	spin_lock_init(&host->lock);

	writel(0, host->base + MMCIMASK0);

	if (variant->mmcimask1)
		writel(0, host->base + MMCIMASK1);

	writel(0xfff, host->base + MMCICLEAR);

	/*
	 * If:
	 * - not using DT but using a descriptor table, or
	 * - using a table of descriptors ALONGSIDE DT, or
	 * look up these descriptors named "cd" and "wp" right here, fail
<<<<<<< HEAD
	 * silently of these do not exist and proceed to try platform data
	 */
	if (!np) {
		ret = mmc_gpiod_request_cd(mmc, "cd", 0, false, 0, NULL);
		if (ret < 0) {
			if (ret == -EPROBE_DEFER)
				goto clk_disable;
			else if (gpio_is_valid(plat->gpio_cd)) {
				ret = mmc_gpio_request_cd(mmc, plat->gpio_cd, 0);
				if (ret)
					goto clk_disable;
			}
		}

		ret = mmc_gpiod_request_ro(mmc, "wp", 0, false, 0, NULL);
		if (ret < 0) {
			if (ret == -EPROBE_DEFER)
				goto clk_disable;
			else if (gpio_is_valid(plat->gpio_wp)) {
				ret = mmc_gpio_request_ro(mmc, plat->gpio_wp);
				if (ret)
					goto clk_disable;
			}
		}
	}

	ret = devm_request_irq(&dev->dev, dev->irq[0], mmci_irq, IRQF_SHARED,
			DRIVER_NAME " (cmd)", host);
=======
	 * silently of these do not exist
	 */
	if (!np) {
		ret = mmc_gpiod_request_cd(mmc, "cd", 0, false, 0);
		if (ret == -EPROBE_DEFER)
			goto clk_disable;

		ret = mmc_gpiod_request_ro(mmc, "wp", 0, 0);
		if (ret == -EPROBE_DEFER)
			goto clk_disable;
	}

	ret = devm_request_threaded_irq(&dev->dev, dev->irq[0], mmci_irq,
					mmci_irq_thread, IRQF_SHARED,
					DRIVER_NAME " (cmd)", host);
>>>>>>> upstream/android-13
	if (ret)
		goto clk_disable;

	if (!dev->irq[1])
		host->singleirq = true;
	else {
		ret = devm_request_irq(&dev->dev, dev->irq[1], mmci_pio_irq,
				IRQF_SHARED, DRIVER_NAME " (pio)", host);
		if (ret)
			goto clk_disable;
	}

	writel(MCI_IRQENABLE | variant->start_err, host->base + MMCIMASK0);

	amba_set_drvdata(dev, mmc);

	dev_info(&dev->dev, "%s: PL%03x manf %x rev%u at 0x%08llx irq %d,%d (pio)\n",
		 mmc_hostname(mmc), amba_part(dev), amba_manf(dev),
		 amba_rev(dev), (unsigned long long)dev->res.start,
		 dev->irq[0], dev->irq[1]);

	mmci_dma_setup(host);

	pm_runtime_set_autosuspend_delay(&dev->dev, 50);
	pm_runtime_use_autosuspend(&dev->dev);

	mmc_add_host(mmc);

	pm_runtime_put(&dev->dev);
	return 0;

 clk_disable:
	clk_disable_unprepare(host->clk);
 host_free:
	mmc_free_host(mmc);
	return ret;
}

<<<<<<< HEAD
static int mmci_remove(struct amba_device *dev)
=======
static void mmci_remove(struct amba_device *dev)
>>>>>>> upstream/android-13
{
	struct mmc_host *mmc = amba_get_drvdata(dev);

	if (mmc) {
		struct mmci_host *host = mmc_priv(mmc);
		struct variant_data *variant = host->variant;

		/*
		 * Undo pm_runtime_put() in probe.  We use the _sync
		 * version here so that we can access the primecell.
		 */
		pm_runtime_get_sync(&dev->dev);

		mmc_remove_host(mmc);

		writel(0, host->base + MMCIMASK0);

		if (variant->mmcimask1)
			writel(0, host->base + MMCIMASK1);

		writel(0, host->base + MMCICOMMAND);
		writel(0, host->base + MMCIDATACTRL);

		mmci_dma_release(host);
		clk_disable_unprepare(host->clk);
		mmc_free_host(mmc);
	}
<<<<<<< HEAD

	return 0;
=======
>>>>>>> upstream/android-13
}

#ifdef CONFIG_PM
static void mmci_save(struct mmci_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);

	writel(0, host->base + MMCIMASK0);
	if (host->variant->pwrreg_nopower) {
		writel(0, host->base + MMCIDATACTRL);
		writel(0, host->base + MMCIPOWER);
		writel(0, host->base + MMCICLOCK);
	}
	mmci_reg_delay(host);

	spin_unlock_irqrestore(&host->lock, flags);
}

static void mmci_restore(struct mmci_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);

	if (host->variant->pwrreg_nopower) {
		writel(host->clk_reg, host->base + MMCICLOCK);
		writel(host->datactrl_reg, host->base + MMCIDATACTRL);
		writel(host->pwr_reg, host->base + MMCIPOWER);
	}
	writel(MCI_IRQENABLE | host->variant->start_err,
	       host->base + MMCIMASK0);
	mmci_reg_delay(host);

	spin_unlock_irqrestore(&host->lock, flags);
}

static int mmci_runtime_suspend(struct device *dev)
{
	struct amba_device *adev = to_amba_device(dev);
	struct mmc_host *mmc = amba_get_drvdata(adev);

	if (mmc) {
		struct mmci_host *host = mmc_priv(mmc);
		pinctrl_pm_select_sleep_state(dev);
		mmci_save(host);
		clk_disable_unprepare(host->clk);
	}

	return 0;
}

static int mmci_runtime_resume(struct device *dev)
{
	struct amba_device *adev = to_amba_device(dev);
	struct mmc_host *mmc = amba_get_drvdata(adev);

	if (mmc) {
		struct mmci_host *host = mmc_priv(mmc);
		clk_prepare_enable(host->clk);
		mmci_restore(host);
<<<<<<< HEAD
		pinctrl_pm_select_default_state(dev);
=======
		pinctrl_select_default_state(dev);
>>>>>>> upstream/android-13
	}

	return 0;
}
#endif

static const struct dev_pm_ops mmci_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
				pm_runtime_force_resume)
	SET_RUNTIME_PM_OPS(mmci_runtime_suspend, mmci_runtime_resume, NULL)
};

static const struct amba_id mmci_ids[] = {
	{
		.id	= 0x00041180,
		.mask	= 0xff0fffff,
		.data	= &variant_arm,
	},
	{
		.id	= 0x01041180,
		.mask	= 0xff0fffff,
		.data	= &variant_arm_extended_fifo,
	},
	{
		.id	= 0x02041180,
		.mask	= 0xff0fffff,
		.data	= &variant_arm_extended_fifo_hwfc,
	},
	{
		.id	= 0x00041181,
		.mask	= 0x000fffff,
		.data	= &variant_arm,
	},
	/* ST Micro variants */
	{
		.id     = 0x00180180,
		.mask   = 0x00ffffff,
		.data	= &variant_u300,
	},
	{
		.id     = 0x10180180,
		.mask   = 0xf0ffffff,
		.data	= &variant_nomadik,
	},
	{
		.id     = 0x00280180,
		.mask   = 0x00ffffff,
		.data	= &variant_nomadik,
	},
	{
		.id     = 0x00480180,
		.mask   = 0xf0ffffff,
		.data	= &variant_ux500,
	},
	{
		.id     = 0x10480180,
		.mask   = 0xf0ffffff,
		.data	= &variant_ux500v2,
	},
	{
		.id     = 0x00880180,
		.mask   = 0x00ffffff,
		.data	= &variant_stm32,
	},
<<<<<<< HEAD
=======
	{
		.id     = 0x10153180,
		.mask	= 0xf0ffffff,
		.data	= &variant_stm32_sdmmc,
	},
	{
		.id     = 0x00253180,
		.mask	= 0xf0ffffff,
		.data	= &variant_stm32_sdmmcv2,
	},
>>>>>>> upstream/android-13
	/* Qualcomm variants */
	{
		.id     = 0x00051180,
		.mask	= 0x000fffff,
		.data	= &variant_qcom,
	},
	{ 0, 0 },
};

MODULE_DEVICE_TABLE(amba, mmci_ids);

static struct amba_driver mmci_driver = {
	.drv		= {
		.name	= DRIVER_NAME,
		.pm	= &mmci_dev_pm_ops,
	},
	.probe		= mmci_probe,
	.remove		= mmci_remove,
	.id_table	= mmci_ids,
};

module_amba_driver(mmci_driver);

module_param(fmax, uint, 0444);

MODULE_DESCRIPTION("ARM PrimeCell PL180/181 Multimedia Card Interface driver");
MODULE_LICENSE("GPL");
