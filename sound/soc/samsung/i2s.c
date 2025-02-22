<<<<<<< HEAD
/* sound/soc/samsung/i2s.c
 *
 * ALSA SoC Audio Layer - Samsung I2S Controller driver
 *
 * Copyright (c) 2010 Samsung Electronics Co. Ltd.
 *	Jaswinder Singh <jassisinghbrar@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
=======
// SPDX-License-Identifier: GPL-2.0
//
// ALSA SoC Audio Layer - Samsung I2S Controller driver
//
// Copyright (c) 2010 Samsung Electronics Co. Ltd.
//	Jaswinder Singh <jassisinghbrar@gmail.com>
>>>>>>> upstream/android-13

#include <dt-bindings/sound/samsung-i2s.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/pm_runtime.h>

#include <sound/soc.h>
#include <sound/pcm_params.h>

#include <linux/platform_data/asoc-s3c.h>

#include "dma.h"
#include "idma.h"
#include "i2s.h"
#include "i2s-regs.h"

#define msecs_to_loops(t) (loops_per_jiffy / 1000 * HZ * t)

<<<<<<< HEAD
=======
#define SAMSUNG_I2S_ID_PRIMARY		1
#define SAMSUNG_I2S_ID_SECONDARY	2

>>>>>>> upstream/android-13
struct samsung_i2s_variant_regs {
	unsigned int	bfs_off;
	unsigned int	rfs_off;
	unsigned int	sdf_off;
	unsigned int	txr_off;
	unsigned int	rclksrc_off;
	unsigned int	mss_off;
	unsigned int	cdclkcon_off;
	unsigned int	lrp_off;
	unsigned int	bfs_mask;
	unsigned int	rfs_mask;
	unsigned int	ftx0cnt_off;
};

struct samsung_i2s_dai_data {
	u32 quirks;
	unsigned int pcm_rates;
	const struct samsung_i2s_variant_regs *i2s_variant_regs;
};

struct i2s_dai {
	/* Platform device for this DAI */
	struct platform_device *pdev;
<<<<<<< HEAD
	/* Memory mapped SFR region */
	void __iomem	*addr;
	/* Rate of RCLK source clock */
	unsigned long rclk_srcrate;
	/* Frame Clock */
	unsigned frmclk;
	/*
	 * Specifically requested RCLK,BCLK by MACHINE Driver.
	 * 0 indicates CPU driver is free to choose any value.
	 */
	unsigned rfs, bfs;
	/* I2S Controller's core clock */
	struct clk *clk;
	/* Clock for generating I2S signals */
	struct clk *op_clk;
=======

	/* Frame clock */
	unsigned frmclk;
	/*
	 * Specifically requested RCLK, BCLK by machine driver.
	 * 0 indicates CPU driver is free to choose any value.
	 */
	unsigned rfs, bfs;
>>>>>>> upstream/android-13
	/* Pointer to the Primary_Fifo if this is Sec_Fifo, NULL otherwise */
	struct i2s_dai *pri_dai;
	/* Pointer to the Secondary_Fifo if it has one, NULL otherwise */
	struct i2s_dai *sec_dai;
<<<<<<< HEAD
#define DAI_OPENED	(1 << 0) /* Dai is opened */
#define DAI_MANAGER	(1 << 1) /* Dai is the manager */
	unsigned mode;
	/* Driver for this DAI */
	struct snd_soc_dai_driver i2s_dai_drv;
=======

#define DAI_OPENED	(1 << 0) /* DAI is opened */
#define DAI_MANAGER	(1 << 1) /* DAI is the manager */
	unsigned mode;

	/* Driver for this DAI */
	struct snd_soc_dai_driver *drv;

>>>>>>> upstream/android-13
	/* DMA parameters */
	struct snd_dmaengine_dai_dma_data dma_playback;
	struct snd_dmaengine_dai_dma_data dma_capture;
	struct snd_dmaengine_dai_dma_data idma_playback;
	dma_filter_fn filter;
<<<<<<< HEAD
	u32	quirks;
	u32	suspend_i2smod;
	u32	suspend_i2scon;
	u32	suspend_i2spsr;
	const struct samsung_i2s_variant_regs *variant_regs;

	/* Spinlock protecting access to the device's registers */
	spinlock_t spinlock;
	spinlock_t *lock;

	/* Below fields are only valid if this is the primary FIFO */
	struct clk *clk_table[3];
	struct clk_onecell_data clk_data;
};

/* Lock for cross i/f checks */
static DEFINE_SPINLOCK(lock);

/* If this is the 'overlay' stereo DAI */
static inline bool is_secondary(struct i2s_dai *i2s)
{
	return i2s->pri_dai ? true : false;
}

/* If operating in SoC-Slave mode */
static inline bool is_slave(struct i2s_dai *i2s)
{
	u32 mod = readl(i2s->addr + I2SMOD);
	return (mod & (1 << i2s->variant_regs->mss_off)) ? true : false;
=======

	struct samsung_i2s_priv *priv;
};

struct samsung_i2s_priv {
	struct platform_device *pdev;
	struct platform_device *pdev_sec;

	/* Lock for cross interface checks */
	spinlock_t pcm_lock;

	/* CPU DAIs and their corresponding drivers */
	struct i2s_dai *dai;
	struct snd_soc_dai_driver *dai_drv;
	int num_dais;

	/* The I2S controller's core clock */
	struct clk *clk;

	/* Clock for generating I2S signals */
	struct clk *op_clk;

	/* Rate of RCLK source clock */
	unsigned long rclk_srcrate;

	/* Cache of selected I2S registers for system suspend */
	u32 suspend_i2smod;
	u32 suspend_i2scon;
	u32 suspend_i2spsr;

	const struct samsung_i2s_variant_regs *variant_regs;
	u32 quirks;

	/* The clock provider's data */
	struct clk *clk_table[3];
	struct clk_onecell_data clk_data;

	/* Spinlock protecting member fields below */
	spinlock_t lock;

	/* Memory mapped SFR region */
	void __iomem *addr;

	/* A flag indicating the I2S slave mode operation */
	bool slave_mode;
};

/* Returns true if this is the 'overlay' stereo DAI */
static inline bool is_secondary(struct i2s_dai *i2s)
{
	return i2s->drv->id == SAMSUNG_I2S_ID_SECONDARY;
>>>>>>> upstream/android-13
}

/* If this interface of the controller is transmitting data */
static inline bool tx_active(struct i2s_dai *i2s)
{
	u32 active;

	if (!i2s)
		return false;

<<<<<<< HEAD
	active = readl(i2s->addr + I2SCON);
=======
	active = readl(i2s->priv->addr + I2SCON);
>>>>>>> upstream/android-13

	if (is_secondary(i2s))
		active &= CON_TXSDMA_ACTIVE;
	else
		active &= CON_TXDMA_ACTIVE;

	return active ? true : false;
}

/* Return pointer to the other DAI */
static inline struct i2s_dai *get_other_dai(struct i2s_dai *i2s)
{
	return i2s->pri_dai ? : i2s->sec_dai;
}

/* If the other interface of the controller is transmitting data */
static inline bool other_tx_active(struct i2s_dai *i2s)
{
	struct i2s_dai *other = get_other_dai(i2s);

	return tx_active(other);
}

/* If any interface of the controller is transmitting data */
static inline bool any_tx_active(struct i2s_dai *i2s)
{
	return tx_active(i2s) || other_tx_active(i2s);
}

/* If this interface of the controller is receiving data */
static inline bool rx_active(struct i2s_dai *i2s)
{
	u32 active;

	if (!i2s)
		return false;

<<<<<<< HEAD
	active = readl(i2s->addr + I2SCON) & CON_RXDMA_ACTIVE;
=======
	active = readl(i2s->priv->addr + I2SCON) & CON_RXDMA_ACTIVE;
>>>>>>> upstream/android-13

	return active ? true : false;
}

/* If the other interface of the controller is receiving data */
static inline bool other_rx_active(struct i2s_dai *i2s)
{
	struct i2s_dai *other = get_other_dai(i2s);

	return rx_active(other);
}

/* If any interface of the controller is receiving data */
static inline bool any_rx_active(struct i2s_dai *i2s)
{
	return rx_active(i2s) || other_rx_active(i2s);
}

/* If the other DAI is transmitting or receiving data */
static inline bool other_active(struct i2s_dai *i2s)
{
	return other_rx_active(i2s) || other_tx_active(i2s);
}

/* If this DAI is transmitting or receiving data */
static inline bool this_active(struct i2s_dai *i2s)
{
	return tx_active(i2s) || rx_active(i2s);
}

/* If the controller is active anyway */
static inline bool any_active(struct i2s_dai *i2s)
{
	return this_active(i2s) || other_active(i2s);
}

static inline struct i2s_dai *to_info(struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	return snd_soc_dai_get_drvdata(dai);
=======
	struct samsung_i2s_priv *priv = snd_soc_dai_get_drvdata(dai);

	return &priv->dai[dai->id - 1];
>>>>>>> upstream/android-13
}

static inline bool is_opened(struct i2s_dai *i2s)
{
	if (i2s && (i2s->mode & DAI_OPENED))
		return true;
	else
		return false;
}

static inline bool is_manager(struct i2s_dai *i2s)
{
	if (is_opened(i2s) && (i2s->mode & DAI_MANAGER))
		return true;
	else
		return false;
}

/* Read RCLK of I2S (in multiples of LRCLK) */
static inline unsigned get_rfs(struct i2s_dai *i2s)
{
<<<<<<< HEAD
	u32 rfs;
	rfs = readl(i2s->addr + I2SMOD) >> i2s->variant_regs->rfs_off;
	rfs &= i2s->variant_regs->rfs_mask;
=======
	struct samsung_i2s_priv *priv = i2s->priv;
	u32 rfs;

	rfs = readl(priv->addr + I2SMOD) >> priv->variant_regs->rfs_off;
	rfs &= priv->variant_regs->rfs_mask;
>>>>>>> upstream/android-13

	switch (rfs) {
	case 7: return 192;
	case 6: return 96;
	case 5: return 128;
	case 4: return 64;
	case 3:	return 768;
	case 2: return 384;
	case 1:	return 512;
	default: return 256;
	}
}

/* Write RCLK of I2S (in multiples of LRCLK) */
static inline void set_rfs(struct i2s_dai *i2s, unsigned rfs)
{
<<<<<<< HEAD
	u32 mod = readl(i2s->addr + I2SMOD);
	int rfs_shift = i2s->variant_regs->rfs_off;

	mod &= ~(i2s->variant_regs->rfs_mask << rfs_shift);
=======
	struct samsung_i2s_priv *priv = i2s->priv;
	u32 mod = readl(priv->addr + I2SMOD);
	int rfs_shift = priv->variant_regs->rfs_off;

	mod &= ~(priv->variant_regs->rfs_mask << rfs_shift);
>>>>>>> upstream/android-13

	switch (rfs) {
	case 192:
		mod |= (EXYNOS7_MOD_RCLK_192FS << rfs_shift);
		break;
	case 96:
		mod |= (EXYNOS7_MOD_RCLK_96FS << rfs_shift);
		break;
	case 128:
		mod |= (EXYNOS7_MOD_RCLK_128FS << rfs_shift);
		break;
	case 64:
		mod |= (EXYNOS7_MOD_RCLK_64FS << rfs_shift);
		break;
	case 768:
		mod |= (MOD_RCLK_768FS << rfs_shift);
		break;
	case 512:
		mod |= (MOD_RCLK_512FS << rfs_shift);
		break;
	case 384:
		mod |= (MOD_RCLK_384FS << rfs_shift);
		break;
	default:
		mod |= (MOD_RCLK_256FS << rfs_shift);
		break;
	}

<<<<<<< HEAD
	writel(mod, i2s->addr + I2SMOD);
}

/* Read Bit-Clock of I2S (in multiples of LRCLK) */
static inline unsigned get_bfs(struct i2s_dai *i2s)
{
	u32 bfs;
	bfs = readl(i2s->addr + I2SMOD) >> i2s->variant_regs->bfs_off;
	bfs &= i2s->variant_regs->bfs_mask;
=======
	writel(mod, priv->addr + I2SMOD);
}

/* Read bit-clock of I2S (in multiples of LRCLK) */
static inline unsigned get_bfs(struct i2s_dai *i2s)
{
	struct samsung_i2s_priv *priv = i2s->priv;
	u32 bfs;

	bfs = readl(priv->addr + I2SMOD) >> priv->variant_regs->bfs_off;
	bfs &= priv->variant_regs->bfs_mask;
>>>>>>> upstream/android-13

	switch (bfs) {
	case 8: return 256;
	case 7: return 192;
	case 6: return 128;
	case 5: return 96;
	case 4: return 64;
	case 3: return 24;
	case 2: return 16;
	case 1:	return 48;
	default: return 32;
	}
}

<<<<<<< HEAD
/* Write Bit-Clock of I2S (in multiples of LRCLK) */
static inline void set_bfs(struct i2s_dai *i2s, unsigned bfs)
{
	u32 mod = readl(i2s->addr + I2SMOD);
	int tdm = i2s->quirks & QUIRK_SUPPORTS_TDM;
	int bfs_shift = i2s->variant_regs->bfs_off;
=======
/* Write bit-clock of I2S (in multiples of LRCLK) */
static inline void set_bfs(struct i2s_dai *i2s, unsigned bfs)
{
	struct samsung_i2s_priv *priv = i2s->priv;
	u32 mod = readl(priv->addr + I2SMOD);
	int tdm = priv->quirks & QUIRK_SUPPORTS_TDM;
	int bfs_shift = priv->variant_regs->bfs_off;
>>>>>>> upstream/android-13

	/* Non-TDM I2S controllers do not support BCLK > 48 * FS */
	if (!tdm && bfs > 48) {
		dev_err(&i2s->pdev->dev, "Unsupported BCLK divider\n");
		return;
	}

<<<<<<< HEAD
	mod &= ~(i2s->variant_regs->bfs_mask << bfs_shift);
=======
	mod &= ~(priv->variant_regs->bfs_mask << bfs_shift);
>>>>>>> upstream/android-13

	switch (bfs) {
	case 48:
		mod |= (MOD_BCLK_48FS << bfs_shift);
		break;
	case 32:
		mod |= (MOD_BCLK_32FS << bfs_shift);
		break;
	case 24:
		mod |= (MOD_BCLK_24FS << bfs_shift);
		break;
	case 16:
		mod |= (MOD_BCLK_16FS << bfs_shift);
		break;
	case 64:
		mod |= (EXYNOS5420_MOD_BCLK_64FS << bfs_shift);
		break;
	case 96:
		mod |= (EXYNOS5420_MOD_BCLK_96FS << bfs_shift);
		break;
	case 128:
		mod |= (EXYNOS5420_MOD_BCLK_128FS << bfs_shift);
		break;
	case 192:
		mod |= (EXYNOS5420_MOD_BCLK_192FS << bfs_shift);
		break;
	case 256:
		mod |= (EXYNOS5420_MOD_BCLK_256FS << bfs_shift);
		break;
	default:
		dev_err(&i2s->pdev->dev, "Wrong BCLK Divider!\n");
		return;
	}

<<<<<<< HEAD
	writel(mod, i2s->addr + I2SMOD);
}

/* Sample-Size */
static inline int get_blc(struct i2s_dai *i2s)
{
	int blc = readl(i2s->addr + I2SMOD);
=======
	writel(mod, priv->addr + I2SMOD);
}

/* Sample size */
static inline int get_blc(struct i2s_dai *i2s)
{
	int blc = readl(i2s->priv->addr + I2SMOD);
>>>>>>> upstream/android-13

	blc = (blc >> 13) & 0x3;

	switch (blc) {
	case 2: return 24;
	case 1:	return 8;
	default: return 16;
	}
}

<<<<<<< HEAD
/* TX Channel Control */
static void i2s_txctrl(struct i2s_dai *i2s, int on)
{
	void __iomem *addr = i2s->addr;
	int txr_off = i2s->variant_regs->txr_off;
=======
/* TX channel control */
static void i2s_txctrl(struct i2s_dai *i2s, int on)
{
	struct samsung_i2s_priv *priv = i2s->priv;
	void __iomem *addr = priv->addr;
	int txr_off = priv->variant_regs->txr_off;
>>>>>>> upstream/android-13
	u32 con = readl(addr + I2SCON);
	u32 mod = readl(addr + I2SMOD) & ~(3 << txr_off);

	if (on) {
		con |= CON_ACTIVE;
		con &= ~CON_TXCH_PAUSE;

		if (is_secondary(i2s)) {
			con |= CON_TXSDMA_ACTIVE;
			con &= ~CON_TXSDMA_PAUSE;
		} else {
			con |= CON_TXDMA_ACTIVE;
			con &= ~CON_TXDMA_PAUSE;
		}

		if (any_rx_active(i2s))
			mod |= 2 << txr_off;
		else
			mod |= 0 << txr_off;
	} else {
		if (is_secondary(i2s)) {
			con |=  CON_TXSDMA_PAUSE;
			con &= ~CON_TXSDMA_ACTIVE;
		} else {
			con |=  CON_TXDMA_PAUSE;
			con &= ~CON_TXDMA_ACTIVE;
		}

		if (other_tx_active(i2s)) {
			writel(con, addr + I2SCON);
			return;
		}

		con |=  CON_TXCH_PAUSE;

		if (any_rx_active(i2s))
			mod |= 1 << txr_off;
		else
			con &= ~CON_ACTIVE;
	}

	writel(mod, addr + I2SMOD);
	writel(con, addr + I2SCON);
}

/* RX Channel Control */
static void i2s_rxctrl(struct i2s_dai *i2s, int on)
{
<<<<<<< HEAD
	void __iomem *addr = i2s->addr;
	int txr_off = i2s->variant_regs->txr_off;
=======
	struct samsung_i2s_priv *priv = i2s->priv;
	void __iomem *addr = priv->addr;
	int txr_off = priv->variant_regs->txr_off;
>>>>>>> upstream/android-13
	u32 con = readl(addr + I2SCON);
	u32 mod = readl(addr + I2SMOD) & ~(3 << txr_off);

	if (on) {
		con |= CON_RXDMA_ACTIVE | CON_ACTIVE;
		con &= ~(CON_RXDMA_PAUSE | CON_RXCH_PAUSE);

		if (any_tx_active(i2s))
			mod |= 2 << txr_off;
		else
			mod |= 1 << txr_off;
	} else {
		con |=  CON_RXDMA_PAUSE | CON_RXCH_PAUSE;
		con &= ~CON_RXDMA_ACTIVE;

		if (any_tx_active(i2s))
			mod |= 0 << txr_off;
		else
			con &= ~CON_ACTIVE;
	}

	writel(mod, addr + I2SMOD);
	writel(con, addr + I2SCON);
}

/* Flush FIFO of an interface */
static inline void i2s_fifo(struct i2s_dai *i2s, u32 flush)
{
	void __iomem *fic;
	u32 val;

	if (!i2s)
		return;

	if (is_secondary(i2s))
<<<<<<< HEAD
		fic = i2s->addr + I2SFICS;
	else
		fic = i2s->addr + I2SFIC;
=======
		fic = i2s->priv->addr + I2SFICS;
	else
		fic = i2s->priv->addr + I2SFIC;
>>>>>>> upstream/android-13

	/* Flush the FIFO */
	writel(readl(fic) | flush, fic);

	/* Be patient */
	val = msecs_to_loops(1) / 1000; /* 1 usec */
	while (--val)
		cpu_relax();

	writel(readl(fic) & ~flush, fic);
}

<<<<<<< HEAD
static int i2s_set_sysclk(struct snd_soc_dai *dai,
	  int clk_id, unsigned int rfs, int dir)
{
	struct i2s_dai *i2s = to_info(dai);
	struct i2s_dai *other = get_other_dai(i2s);
	const struct samsung_i2s_variant_regs *i2s_regs = i2s->variant_regs;
=======
static int i2s_set_sysclk(struct snd_soc_dai *dai, int clk_id, unsigned int rfs,
			  int dir)
{
	struct samsung_i2s_priv *priv = snd_soc_dai_get_drvdata(dai);
	struct i2s_dai *i2s = to_info(dai);
	struct i2s_dai *other = get_other_dai(i2s);
	const struct samsung_i2s_variant_regs *i2s_regs = priv->variant_regs;
>>>>>>> upstream/android-13
	unsigned int cdcon_mask = 1 << i2s_regs->cdclkcon_off;
	unsigned int rsrc_mask = 1 << i2s_regs->rclksrc_off;
	u32 mod, mask, val = 0;
	unsigned long flags;
	int ret = 0;

	pm_runtime_get_sync(dai->dev);

<<<<<<< HEAD
	spin_lock_irqsave(i2s->lock, flags);
	mod = readl(i2s->addr + I2SMOD);
	spin_unlock_irqrestore(i2s->lock, flags);
=======
	spin_lock_irqsave(&priv->lock, flags);
	mod = readl(priv->addr + I2SMOD);
	spin_unlock_irqrestore(&priv->lock, flags);
>>>>>>> upstream/android-13

	switch (clk_id) {
	case SAMSUNG_I2S_OPCLK:
		mask = MOD_OPCLK_MASK;
		val = (dir << MOD_OPCLK_SHIFT) & MOD_OPCLK_MASK;
		break;
	case SAMSUNG_I2S_CDCLK:
		mask = 1 << i2s_regs->cdclkcon_off;
		/* Shouldn't matter in GATING(CLOCK_IN) mode */
		if (dir == SND_SOC_CLOCK_IN)
			rfs = 0;

		if ((rfs && other && other->rfs && (other->rfs != rfs)) ||
				(any_active(i2s) &&
				(((dir == SND_SOC_CLOCK_IN)
					&& !(mod & cdcon_mask)) ||
				((dir == SND_SOC_CLOCK_OUT)
					&& (mod & cdcon_mask))))) {
			dev_err(&i2s->pdev->dev,
				"%s:%d Other DAI busy\n", __func__, __LINE__);
			ret = -EAGAIN;
			goto err;
		}

		if (dir == SND_SOC_CLOCK_IN)
			val = 1 << i2s_regs->cdclkcon_off;

		i2s->rfs = rfs;
		break;

	case SAMSUNG_I2S_RCLKSRC_0: /* clock corrsponding to IISMOD[10] := 0 */
	case SAMSUNG_I2S_RCLKSRC_1: /* clock corrsponding to IISMOD[10] := 1 */
		mask = 1 << i2s_regs->rclksrc_off;

<<<<<<< HEAD
		if ((i2s->quirks & QUIRK_NO_MUXPSR)
=======
		if ((priv->quirks & QUIRK_NO_MUXPSR)
>>>>>>> upstream/android-13
				|| (clk_id == SAMSUNG_I2S_RCLKSRC_0))
			clk_id = 0;
		else
			clk_id = 1;

		if (!any_active(i2s)) {
<<<<<<< HEAD
			if (i2s->op_clk && !IS_ERR(i2s->op_clk)) {
				if ((clk_id && !(mod & rsrc_mask)) ||
					(!clk_id && (mod & rsrc_mask))) {
					clk_disable_unprepare(i2s->op_clk);
					clk_put(i2s->op_clk);
				} else {
					i2s->rclk_srcrate =
						clk_get_rate(i2s->op_clk);
=======
			if (priv->op_clk && !IS_ERR(priv->op_clk)) {
				if ((clk_id && !(mod & rsrc_mask)) ||
					(!clk_id && (mod & rsrc_mask))) {
					clk_disable_unprepare(priv->op_clk);
					clk_put(priv->op_clk);
				} else {
					priv->rclk_srcrate =
						clk_get_rate(priv->op_clk);
>>>>>>> upstream/android-13
					goto done;
				}
			}

			if (clk_id)
<<<<<<< HEAD
				i2s->op_clk = clk_get(&i2s->pdev->dev,
						"i2s_opclk1");
			else
				i2s->op_clk = clk_get(&i2s->pdev->dev,
						"i2s_opclk0");

			if (WARN_ON(IS_ERR(i2s->op_clk))) {
				ret = PTR_ERR(i2s->op_clk);
				i2s->op_clk = NULL;
				goto err;
			}

			ret = clk_prepare_enable(i2s->op_clk);
			if (ret) {
				clk_put(i2s->op_clk);
				i2s->op_clk = NULL;
				goto err;
			}
			i2s->rclk_srcrate = clk_get_rate(i2s->op_clk);

			/* Over-ride the other's */
			if (other) {
				other->op_clk = i2s->op_clk;
				other->rclk_srcrate = i2s->rclk_srcrate;
			}
=======
				priv->op_clk = clk_get(&i2s->pdev->dev,
						"i2s_opclk1");
			else
				priv->op_clk = clk_get(&i2s->pdev->dev,
						"i2s_opclk0");

			if (WARN_ON(IS_ERR(priv->op_clk))) {
				ret = PTR_ERR(priv->op_clk);
				priv->op_clk = NULL;
				goto err;
			}

			ret = clk_prepare_enable(priv->op_clk);
			if (ret) {
				clk_put(priv->op_clk);
				priv->op_clk = NULL;
				goto err;
			}
			priv->rclk_srcrate = clk_get_rate(priv->op_clk);

>>>>>>> upstream/android-13
		} else if ((!clk_id && (mod & rsrc_mask))
				|| (clk_id && !(mod & rsrc_mask))) {
			dev_err(&i2s->pdev->dev,
				"%s:%d Other DAI busy\n", __func__, __LINE__);
			ret = -EAGAIN;
			goto err;
		} else {
			/* Call can't be on the active DAI */
<<<<<<< HEAD
			i2s->op_clk = other->op_clk;
			i2s->rclk_srcrate = other->rclk_srcrate;
=======
>>>>>>> upstream/android-13
			goto done;
		}

		if (clk_id == 1)
			val = 1 << i2s_regs->rclksrc_off;
		break;
	default:
		dev_err(&i2s->pdev->dev, "We don't serve that!\n");
		ret = -EINVAL;
		goto err;
	}

<<<<<<< HEAD
	spin_lock_irqsave(i2s->lock, flags);
	mod = readl(i2s->addr + I2SMOD);
	mod = (mod & ~mask) | val;
	writel(mod, i2s->addr + I2SMOD);
	spin_unlock_irqrestore(i2s->lock, flags);
=======
	spin_lock_irqsave(&priv->lock, flags);
	mod = readl(priv->addr + I2SMOD);
	mod = (mod & ~mask) | val;
	writel(mod, priv->addr + I2SMOD);
	spin_unlock_irqrestore(&priv->lock, flags);
>>>>>>> upstream/android-13
done:
	pm_runtime_put(dai->dev);

	return 0;
err:
	pm_runtime_put(dai->dev);
	return ret;
}

<<<<<<< HEAD
static int i2s_set_fmt(struct snd_soc_dai *dai,
	unsigned int fmt)
{
	struct i2s_dai *i2s = to_info(dai);
	struct i2s_dai *other = get_other_dai(i2s);
=======
static int i2s_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	struct samsung_i2s_priv *priv = snd_soc_dai_get_drvdata(dai);
	struct i2s_dai *i2s = to_info(dai);
>>>>>>> upstream/android-13
	int lrp_shift, sdf_shift, sdf_mask, lrp_rlow, mod_slave;
	u32 mod, tmp = 0;
	unsigned long flags;

<<<<<<< HEAD
	lrp_shift = i2s->variant_regs->lrp_off;
	sdf_shift = i2s->variant_regs->sdf_off;
	mod_slave = 1 << i2s->variant_regs->mss_off;
=======
	lrp_shift = priv->variant_regs->lrp_off;
	sdf_shift = priv->variant_regs->sdf_off;
	mod_slave = 1 << priv->variant_regs->mss_off;
>>>>>>> upstream/android-13

	sdf_mask = MOD_SDF_MASK << sdf_shift;
	lrp_rlow = MOD_LR_RLOW << lrp_shift;

	/* Format is priority */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_RIGHT_J:
		tmp |= lrp_rlow;
		tmp |= (MOD_SDF_MSB << sdf_shift);
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		tmp |= lrp_rlow;
		tmp |= (MOD_SDF_LSB << sdf_shift);
		break;
	case SND_SOC_DAIFMT_I2S:
		tmp |= (MOD_SDF_IIS << sdf_shift);
		break;
	default:
		dev_err(&i2s->pdev->dev, "Format not supported\n");
		return -EINVAL;
	}

	/*
	 * INV flag is relative to the FORMAT flag - if set it simply
	 * flips the polarity specified by the Standard
	 */
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		break;
	case SND_SOC_DAIFMT_NB_IF:
		if (tmp & lrp_rlow)
			tmp &= ~lrp_rlow;
		else
			tmp |= lrp_rlow;
		break;
	default:
		dev_err(&i2s->pdev->dev, "Polarity not supported\n");
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		tmp |= mod_slave;
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
		/*
		 * Set default source clock in Master mode, only when the
		 * CLK_I2S_RCLK_SRC clock is not exposed so we ensure any
		 * clock configuration assigned in DT is not overwritten.
		 */
<<<<<<< HEAD
		if (i2s->rclk_srcrate == 0 && i2s->clk_data.clks == NULL &&
		    other->clk_data.clks == NULL)
=======
		if (priv->rclk_srcrate == 0 && priv->clk_data.clks == NULL)
>>>>>>> upstream/android-13
			i2s_set_sysclk(dai, SAMSUNG_I2S_RCLKSRC_0,
							0, SND_SOC_CLOCK_IN);
		break;
	default:
		dev_err(&i2s->pdev->dev, "master/slave format not supported\n");
		return -EINVAL;
	}

	pm_runtime_get_sync(dai->dev);
<<<<<<< HEAD
	spin_lock_irqsave(i2s->lock, flags);
	mod = readl(i2s->addr + I2SMOD);
=======
	spin_lock_irqsave(&priv->lock, flags);
	mod = readl(priv->addr + I2SMOD);
>>>>>>> upstream/android-13
	/*
	 * Don't change the I2S mode if any controller is active on this
	 * channel.
	 */
	if (any_active(i2s) &&
		((mod & (sdf_mask | lrp_rlow | mod_slave)) != tmp)) {
<<<<<<< HEAD
		spin_unlock_irqrestore(i2s->lock, flags);
=======
		spin_unlock_irqrestore(&priv->lock, flags);
>>>>>>> upstream/android-13
		pm_runtime_put(dai->dev);
		dev_err(&i2s->pdev->dev,
				"%s:%d Other DAI busy\n", __func__, __LINE__);
		return -EAGAIN;
	}

	mod &= ~(sdf_mask | lrp_rlow | mod_slave);
	mod |= tmp;
<<<<<<< HEAD
	writel(mod, i2s->addr + I2SMOD);
	spin_unlock_irqrestore(i2s->lock, flags);
=======
	writel(mod, priv->addr + I2SMOD);
	priv->slave_mode = (mod & mod_slave);
	spin_unlock_irqrestore(&priv->lock, flags);
>>>>>>> upstream/android-13
	pm_runtime_put(dai->dev);

	return 0;
}

static int i2s_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params, struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct i2s_dai *i2s = to_info(dai);
	struct i2s_dai *other = get_other_dai(i2s);
=======
	struct samsung_i2s_priv *priv = snd_soc_dai_get_drvdata(dai);
	struct i2s_dai *i2s = to_info(dai);
>>>>>>> upstream/android-13
	u32 mod, mask = 0, val = 0;
	struct clk *rclksrc;
	unsigned long flags;

	WARN_ON(!pm_runtime_active(dai->dev));

	if (!is_secondary(i2s))
		mask |= (MOD_DC2_EN | MOD_DC1_EN);

	switch (params_channels(params)) {
	case 6:
		val |= MOD_DC2_EN;
<<<<<<< HEAD
		/* fall through */
=======
		fallthrough;
>>>>>>> upstream/android-13
	case 4:
		val |= MOD_DC1_EN;
		break;
	case 2:
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			i2s->dma_playback.addr_width = 4;
		else
			i2s->dma_capture.addr_width = 4;
		break;
	case 1:
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			i2s->dma_playback.addr_width = 2;
		else
			i2s->dma_capture.addr_width = 2;

		break;
	default:
		dev_err(&i2s->pdev->dev, "%d channels not supported\n",
				params_channels(params));
		return -EINVAL;
	}

	if (is_secondary(i2s))
		mask |= MOD_BLCS_MASK;
	else
		mask |= MOD_BLCP_MASK;

	if (is_manager(i2s))
		mask |= MOD_BLC_MASK;

	switch (params_width(params)) {
	case 8:
		if (is_secondary(i2s))
			val |= MOD_BLCS_8BIT;
		else
			val |= MOD_BLCP_8BIT;
		if (is_manager(i2s))
			val |= MOD_BLC_8BIT;
		break;
	case 16:
		if (is_secondary(i2s))
			val |= MOD_BLCS_16BIT;
		else
			val |= MOD_BLCP_16BIT;
		if (is_manager(i2s))
			val |= MOD_BLC_16BIT;
		break;
	case 24:
		if (is_secondary(i2s))
			val |= MOD_BLCS_24BIT;
		else
			val |= MOD_BLCP_24BIT;
		if (is_manager(i2s))
			val |= MOD_BLC_24BIT;
		break;
	default:
		dev_err(&i2s->pdev->dev, "Format(%d) not supported\n",
				params_format(params));
		return -EINVAL;
	}

<<<<<<< HEAD
	spin_lock_irqsave(i2s->lock, flags);
	mod = readl(i2s->addr + I2SMOD);
	mod = (mod & ~mask) | val;
	writel(mod, i2s->addr + I2SMOD);
	spin_unlock_irqrestore(i2s->lock, flags);
=======
	spin_lock_irqsave(&priv->lock, flags);
	mod = readl(priv->addr + I2SMOD);
	mod = (mod & ~mask) | val;
	writel(mod, priv->addr + I2SMOD);
	spin_unlock_irqrestore(&priv->lock, flags);
>>>>>>> upstream/android-13

	snd_soc_dai_init_dma_data(dai, &i2s->dma_playback, &i2s->dma_capture);

	i2s->frmclk = params_rate(params);

<<<<<<< HEAD
	rclksrc = i2s->clk_table[CLK_I2S_RCLK_SRC];
	if (!rclksrc || IS_ERR(rclksrc))
		rclksrc = other->clk_table[CLK_I2S_RCLK_SRC];

	if (rclksrc && !IS_ERR(rclksrc))
		i2s->rclk_srcrate = clk_get_rate(rclksrc);
=======
	rclksrc = priv->clk_table[CLK_I2S_RCLK_SRC];
	if (rclksrc && !IS_ERR(rclksrc))
		priv->rclk_srcrate = clk_get_rate(rclksrc);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/* We set constraints on the substream acc to the version of I2S */
static int i2s_startup(struct snd_pcm_substream *substream,
	  struct snd_soc_dai *dai)
{
=======
/* We set constraints on the substream according to the version of I2S */
static int i2s_startup(struct snd_pcm_substream *substream,
	  struct snd_soc_dai *dai)
{
	struct samsung_i2s_priv *priv = snd_soc_dai_get_drvdata(dai);
>>>>>>> upstream/android-13
	struct i2s_dai *i2s = to_info(dai);
	struct i2s_dai *other = get_other_dai(i2s);
	unsigned long flags;

	pm_runtime_get_sync(dai->dev);

<<<<<<< HEAD
	spin_lock_irqsave(&lock, flags);
=======
	spin_lock_irqsave(&priv->pcm_lock, flags);
>>>>>>> upstream/android-13

	i2s->mode |= DAI_OPENED;

	if (is_manager(other))
		i2s->mode &= ~DAI_MANAGER;
	else
		i2s->mode |= DAI_MANAGER;

<<<<<<< HEAD
	if (!any_active(i2s) && (i2s->quirks & QUIRK_NEED_RSTCLR))
		writel(CON_RSTCLR, i2s->addr + I2SCON);

	spin_unlock_irqrestore(&lock, flags);
=======
	if (!any_active(i2s) && (priv->quirks & QUIRK_NEED_RSTCLR))
		writel(CON_RSTCLR, i2s->priv->addr + I2SCON);

	spin_unlock_irqrestore(&priv->pcm_lock, flags);
>>>>>>> upstream/android-13

	return 0;
}

static void i2s_shutdown(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
<<<<<<< HEAD
=======
	struct samsung_i2s_priv *priv = snd_soc_dai_get_drvdata(dai);
>>>>>>> upstream/android-13
	struct i2s_dai *i2s = to_info(dai);
	struct i2s_dai *other = get_other_dai(i2s);
	unsigned long flags;

<<<<<<< HEAD
	spin_lock_irqsave(&lock, flags);
=======
	spin_lock_irqsave(&priv->pcm_lock, flags);
>>>>>>> upstream/android-13

	i2s->mode &= ~DAI_OPENED;
	i2s->mode &= ~DAI_MANAGER;

	if (is_opened(other))
		other->mode |= DAI_MANAGER;

	/* Reset any constraint on RFS and BFS */
	i2s->rfs = 0;
	i2s->bfs = 0;

<<<<<<< HEAD
	spin_unlock_irqrestore(&lock, flags);
=======
	spin_unlock_irqrestore(&priv->pcm_lock, flags);
>>>>>>> upstream/android-13

	pm_runtime_put(dai->dev);
}

static int config_setup(struct i2s_dai *i2s)
{
<<<<<<< HEAD
=======
	struct samsung_i2s_priv *priv = i2s->priv;
>>>>>>> upstream/android-13
	struct i2s_dai *other = get_other_dai(i2s);
	unsigned rfs, bfs, blc;
	u32 psr;

	blc = get_blc(i2s);

	bfs = i2s->bfs;

	if (!bfs && other)
		bfs = other->bfs;

	/* Select least possible multiple(2) if no constraint set */
	if (!bfs)
		bfs = blc * 2;

	rfs = i2s->rfs;

	if (!rfs && other)
		rfs = other->rfs;

	if ((rfs == 256 || rfs == 512) && (blc == 24)) {
		dev_err(&i2s->pdev->dev,
			"%d-RFS not supported for 24-blc\n", rfs);
		return -EINVAL;
	}

	if (!rfs) {
		if (bfs == 16 || bfs == 32)
			rfs = 256;
		else
			rfs = 384;
	}

	/* If already setup and running */
	if (any_active(i2s) && (get_rfs(i2s) != rfs || get_bfs(i2s) != bfs)) {
		dev_err(&i2s->pdev->dev,
				"%s:%d Other DAI busy\n", __func__, __LINE__);
		return -EAGAIN;
	}

	set_bfs(i2s, bfs);
	set_rfs(i2s, rfs);

	/* Don't bother with PSR in Slave mode */
<<<<<<< HEAD
	if (is_slave(i2s))
		return 0;

	if (!(i2s->quirks & QUIRK_NO_MUXPSR)) {
		psr = i2s->rclk_srcrate / i2s->frmclk / rfs;
		writel(((psr - 1) << 8) | PSR_PSREN, i2s->addr + I2SPSR);
		dev_dbg(&i2s->pdev->dev,
			"RCLK_SRC=%luHz PSR=%u, RCLK=%dfs, BCLK=%dfs\n",
				i2s->rclk_srcrate, psr, rfs, bfs);
=======
	if (priv->slave_mode)
		return 0;

	if (!(priv->quirks & QUIRK_NO_MUXPSR)) {
		psr = priv->rclk_srcrate / i2s->frmclk / rfs;
		writel(((psr - 1) << 8) | PSR_PSREN, priv->addr + I2SPSR);
		dev_dbg(&i2s->pdev->dev,
			"RCLK_SRC=%luHz PSR=%u, RCLK=%dfs, BCLK=%dfs\n",
				priv->rclk_srcrate, psr, rfs, bfs);
>>>>>>> upstream/android-13
	}

	return 0;
}

static int i2s_trigger(struct snd_pcm_substream *substream,
	int cmd, struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	int capture = (substream->stream == SNDRV_PCM_STREAM_CAPTURE);
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct i2s_dai *i2s = to_info(rtd->cpu_dai);
=======
	struct samsung_i2s_priv *priv = snd_soc_dai_get_drvdata(dai);
	int capture = (substream->stream == SNDRV_PCM_STREAM_CAPTURE);
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct i2s_dai *i2s = to_info(asoc_rtd_to_cpu(rtd, 0));
>>>>>>> upstream/android-13
	unsigned long flags;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		pm_runtime_get_sync(dai->dev);
<<<<<<< HEAD
		spin_lock_irqsave(i2s->lock, flags);

		if (config_setup(i2s)) {
			spin_unlock_irqrestore(i2s->lock, flags);
=======
		spin_lock_irqsave(&priv->lock, flags);

		if (config_setup(i2s)) {
			spin_unlock_irqrestore(&priv->lock, flags);
>>>>>>> upstream/android-13
			return -EINVAL;
		}

		if (capture)
			i2s_rxctrl(i2s, 1);
		else
			i2s_txctrl(i2s, 1);

<<<<<<< HEAD
		spin_unlock_irqrestore(i2s->lock, flags);
=======
		spin_unlock_irqrestore(&priv->lock, flags);
>>>>>>> upstream/android-13
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
<<<<<<< HEAD
		spin_lock_irqsave(i2s->lock, flags);
=======
		spin_lock_irqsave(&priv->lock, flags);
>>>>>>> upstream/android-13

		if (capture) {
			i2s_rxctrl(i2s, 0);
			i2s_fifo(i2s, FIC_RXFLUSH);
		} else {
			i2s_txctrl(i2s, 0);
			i2s_fifo(i2s, FIC_TXFLUSH);
		}

<<<<<<< HEAD
		spin_unlock_irqrestore(i2s->lock, flags);
=======
		spin_unlock_irqrestore(&priv->lock, flags);
>>>>>>> upstream/android-13
		pm_runtime_put(dai->dev);
		break;
	}

	return 0;
}

static int i2s_set_clkdiv(struct snd_soc_dai *dai,
	int div_id, int div)
{
	struct i2s_dai *i2s = to_info(dai);
	struct i2s_dai *other = get_other_dai(i2s);

	switch (div_id) {
	case SAMSUNG_I2S_DIV_BCLK:
		pm_runtime_get_sync(dai->dev);
		if ((any_active(i2s) && div && (get_bfs(i2s) != div))
			|| (other && other->bfs && (other->bfs != div))) {
			pm_runtime_put(dai->dev);
			dev_err(&i2s->pdev->dev,
				"%s:%d Other DAI busy\n", __func__, __LINE__);
			return -EAGAIN;
		}
		i2s->bfs = div;
		pm_runtime_put(dai->dev);
		break;
	default:
		dev_err(&i2s->pdev->dev,
			"Invalid clock divider(%d)\n", div_id);
		return -EINVAL;
	}

	return 0;
}

static snd_pcm_sframes_t
i2s_delay(struct snd_pcm_substream *substream, struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct i2s_dai *i2s = to_info(dai);
	u32 reg = readl(i2s->addr + I2SFIC);
	snd_pcm_sframes_t delay;
	const struct samsung_i2s_variant_regs *i2s_regs = i2s->variant_regs;
=======
	struct samsung_i2s_priv *priv = snd_soc_dai_get_drvdata(dai);
	struct i2s_dai *i2s = to_info(dai);
	u32 reg = readl(priv->addr + I2SFIC);
	snd_pcm_sframes_t delay;
>>>>>>> upstream/android-13

	WARN_ON(!pm_runtime_active(dai->dev));

	if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
		delay = FIC_RXCOUNT(reg);
	else if (is_secondary(i2s))
<<<<<<< HEAD
		delay = FICS_TXCOUNT(readl(i2s->addr + I2SFICS));
	else
		delay = (reg >> i2s_regs->ftx0cnt_off) & 0x7f;
=======
		delay = FICS_TXCOUNT(readl(priv->addr + I2SFICS));
	else
		delay = (reg >> priv->variant_regs->ftx0cnt_off) & 0x7f;
>>>>>>> upstream/android-13

	return delay;
}

#ifdef CONFIG_PM
<<<<<<< HEAD
static int i2s_suspend(struct snd_soc_dai *dai)
{
	return pm_runtime_force_suspend(dai->dev);
}

static int i2s_resume(struct snd_soc_dai *dai)
{
	return pm_runtime_force_resume(dai->dev);
=======
static int i2s_suspend(struct snd_soc_component *component)
{
	return pm_runtime_force_suspend(component->dev);
}

static int i2s_resume(struct snd_soc_component *component)
{
	return pm_runtime_force_resume(component->dev);
>>>>>>> upstream/android-13
}
#else
#define i2s_suspend NULL
#define i2s_resume  NULL
#endif

static int samsung_i2s_dai_probe(struct snd_soc_dai *dai)
{
<<<<<<< HEAD
=======
	struct samsung_i2s_priv *priv = snd_soc_dai_get_drvdata(dai);
>>>>>>> upstream/android-13
	struct i2s_dai *i2s = to_info(dai);
	struct i2s_dai *other = get_other_dai(i2s);
	unsigned long flags;

	pm_runtime_get_sync(dai->dev);

<<<<<<< HEAD
	if (is_secondary(i2s)) { /* If this is probe on the secondary DAI */
		snd_soc_dai_init_dma_data(dai, &other->sec_dai->dma_playback,
					   NULL);
	} else {
		snd_soc_dai_init_dma_data(dai, &i2s->dma_playback,
					   &i2s->dma_capture);

		if (i2s->quirks & QUIRK_NEED_RSTCLR)
			writel(CON_RSTCLR, i2s->addr + I2SCON);

		if (i2s->quirks & QUIRK_SUPPORTS_IDMA)
			idma_reg_addr_init(i2s->addr,
					i2s->sec_dai->idma_playback.addr);
=======
	if (is_secondary(i2s)) {
		/* If this is probe on the secondary DAI */
		snd_soc_dai_init_dma_data(dai, &i2s->dma_playback, NULL);
	} else {
		snd_soc_dai_init_dma_data(dai, &i2s->dma_playback,
					  &i2s->dma_capture);

		if (priv->quirks & QUIRK_NEED_RSTCLR)
			writel(CON_RSTCLR, priv->addr + I2SCON);

		if (priv->quirks & QUIRK_SUPPORTS_IDMA)
			idma_reg_addr_init(priv->addr,
					   other->idma_playback.addr);
>>>>>>> upstream/android-13
	}

	/* Reset any constraint on RFS and BFS */
	i2s->rfs = 0;
	i2s->bfs = 0;
<<<<<<< HEAD
	i2s->rclk_srcrate = 0;

	spin_lock_irqsave(i2s->lock, flags);
=======

	spin_lock_irqsave(&priv->lock, flags);
>>>>>>> upstream/android-13
	i2s_txctrl(i2s, 0);
	i2s_rxctrl(i2s, 0);
	i2s_fifo(i2s, FIC_TXFLUSH);
	i2s_fifo(other, FIC_TXFLUSH);
	i2s_fifo(i2s, FIC_RXFLUSH);
<<<<<<< HEAD
	spin_unlock_irqrestore(i2s->lock, flags);
=======
	spin_unlock_irqrestore(&priv->lock, flags);
>>>>>>> upstream/android-13

	/* Gate CDCLK by default */
	if (!is_opened(other))
		i2s_set_sysclk(dai, SAMSUNG_I2S_CDCLK,
				0, SND_SOC_CLOCK_IN);
	pm_runtime_put(dai->dev);

	return 0;
}

static int samsung_i2s_dai_remove(struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct i2s_dai *i2s = snd_soc_dai_get_drvdata(dai);
=======
	struct samsung_i2s_priv *priv = snd_soc_dai_get_drvdata(dai);
	struct i2s_dai *i2s = to_info(dai);
>>>>>>> upstream/android-13
	unsigned long flags;

	pm_runtime_get_sync(dai->dev);

	if (!is_secondary(i2s)) {
<<<<<<< HEAD
		if (i2s->quirks & QUIRK_NEED_RSTCLR) {
			spin_lock_irqsave(i2s->lock, flags);
			writel(0, i2s->addr + I2SCON);
			spin_unlock_irqrestore(i2s->lock, flags);
=======
		if (priv->quirks & QUIRK_NEED_RSTCLR) {
			spin_lock_irqsave(&priv->lock, flags);
			writel(0, priv->addr + I2SCON);
			spin_unlock_irqrestore(&priv->lock, flags);
>>>>>>> upstream/android-13
		}
	}

	pm_runtime_put(dai->dev);

	return 0;
}

static const struct snd_soc_dai_ops samsung_i2s_dai_ops = {
	.trigger = i2s_trigger,
	.hw_params = i2s_hw_params,
	.set_fmt = i2s_set_fmt,
	.set_clkdiv = i2s_set_clkdiv,
	.set_sysclk = i2s_set_sysclk,
	.startup = i2s_startup,
	.shutdown = i2s_shutdown,
	.delay = i2s_delay,
};

<<<<<<< HEAD
static const struct snd_soc_component_driver samsung_i2s_component = {
	.name		= "samsung-i2s",
};

#define SAMSUNG_I2S_FMTS	(SNDRV_PCM_FMTBIT_S8 | \
					SNDRV_PCM_FMTBIT_S16_LE | \
					SNDRV_PCM_FMTBIT_S24_LE)

static struct i2s_dai *i2s_alloc_dai(struct platform_device *pdev,
				const struct samsung_i2s_dai_data *i2s_dai_data,
				bool sec)
{
	struct i2s_dai *i2s;

	i2s = devm_kzalloc(&pdev->dev, sizeof(struct i2s_dai), GFP_KERNEL);
	if (i2s == NULL)
		return NULL;

	i2s->pdev = pdev;
	i2s->pri_dai = NULL;
	i2s->sec_dai = NULL;
	i2s->i2s_dai_drv.id = 1;
	i2s->i2s_dai_drv.symmetric_rates = 1;
	i2s->i2s_dai_drv.probe = samsung_i2s_dai_probe;
	i2s->i2s_dai_drv.remove = samsung_i2s_dai_remove;
	i2s->i2s_dai_drv.ops = &samsung_i2s_dai_ops;
	i2s->i2s_dai_drv.suspend = i2s_suspend;
	i2s->i2s_dai_drv.resume = i2s_resume;
	i2s->i2s_dai_drv.playback.channels_min = 1;
	i2s->i2s_dai_drv.playback.channels_max = 2;
	i2s->i2s_dai_drv.playback.rates = i2s_dai_data->pcm_rates;
	i2s->i2s_dai_drv.playback.formats = SAMSUNG_I2S_FMTS;

	if (!sec) {
		i2s->i2s_dai_drv.name = SAMSUNG_I2S_DAI;
		i2s->i2s_dai_drv.capture.channels_min = 1;
		i2s->i2s_dai_drv.capture.channels_max = 2;
		i2s->i2s_dai_drv.capture.rates = i2s_dai_data->pcm_rates;
		i2s->i2s_dai_drv.capture.formats = SAMSUNG_I2S_FMTS;
	} else {
		i2s->i2s_dai_drv.name = SAMSUNG_I2S_DAI_SEC;
	}
	return i2s;
=======
static const struct snd_soc_dapm_widget samsung_i2s_widgets[] = {
	/* Backend DAI  */
	SND_SOC_DAPM_AIF_OUT("Mixer DAI TX", NULL, 0, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_AIF_IN("Mixer DAI RX", NULL, 0, SND_SOC_NOPM, 0, 0),

	/* Playback Mixer */
	SND_SOC_DAPM_MIXER("Playback Mixer", SND_SOC_NOPM, 0, 0, NULL, 0),
};

static const struct snd_soc_dapm_route samsung_i2s_dapm_routes[] = {
	{ "Playback Mixer", NULL, "Primary Playback" },
	{ "Playback Mixer", NULL, "Secondary Playback" },

	{ "Mixer DAI TX", NULL, "Playback Mixer" },
	{ "Primary Capture", NULL, "Mixer DAI RX" },
};

static const struct snd_soc_component_driver samsung_i2s_component = {
	.name = "samsung-i2s",

	.dapm_widgets = samsung_i2s_widgets,
	.num_dapm_widgets = ARRAY_SIZE(samsung_i2s_widgets),

	.dapm_routes = samsung_i2s_dapm_routes,
	.num_dapm_routes = ARRAY_SIZE(samsung_i2s_dapm_routes),

	.suspend = i2s_suspend,
	.resume = i2s_resume,
};

#define SAMSUNG_I2S_FMTS (SNDRV_PCM_FMTBIT_S8 | SNDRV_PCM_FMTBIT_S16_LE | \
			  SNDRV_PCM_FMTBIT_S24_LE)

static int i2s_alloc_dais(struct samsung_i2s_priv *priv,
			  const struct samsung_i2s_dai_data *i2s_dai_data,
			  int num_dais)
{
	static const char *dai_names[] = { "samsung-i2s", "samsung-i2s-sec" };
	static const char *stream_names[] = { "Primary Playback",
					      "Secondary Playback" };
	struct snd_soc_dai_driver *dai_drv;
	int i;

	priv->dai = devm_kcalloc(&priv->pdev->dev, num_dais,
				     sizeof(struct i2s_dai), GFP_KERNEL);
	if (!priv->dai)
		return -ENOMEM;

	priv->dai_drv = devm_kcalloc(&priv->pdev->dev, num_dais,
				     sizeof(*dai_drv), GFP_KERNEL);
	if (!priv->dai_drv)
		return -ENOMEM;

	for (i = 0; i < num_dais; i++) {
		dai_drv = &priv->dai_drv[i];

		dai_drv->probe = samsung_i2s_dai_probe;
		dai_drv->remove = samsung_i2s_dai_remove;

		dai_drv->symmetric_rate = 1;
		dai_drv->ops = &samsung_i2s_dai_ops;

		dai_drv->playback.channels_min = 1;
		dai_drv->playback.channels_max = 2;
		dai_drv->playback.rates = i2s_dai_data->pcm_rates;
		dai_drv->playback.formats = SAMSUNG_I2S_FMTS;
		dai_drv->playback.stream_name = stream_names[i];

		dai_drv->id = i + 1;
		dai_drv->name = dai_names[i];

		priv->dai[i].drv = &priv->dai_drv[i];
		priv->dai[i].pdev = priv->pdev;
	}

	/* Initialize capture only for the primary DAI */
	dai_drv = &priv->dai_drv[SAMSUNG_I2S_ID_PRIMARY - 1];

	dai_drv->capture.channels_min = 1;
	dai_drv->capture.channels_max = 2;
	dai_drv->capture.rates = i2s_dai_data->pcm_rates;
	dai_drv->capture.formats = SAMSUNG_I2S_FMTS;
	dai_drv->capture.stream_name = "Primary Capture";

	return 0;
>>>>>>> upstream/android-13
}

#ifdef CONFIG_PM
static int i2s_runtime_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct i2s_dai *i2s = dev_get_drvdata(dev);

	i2s->suspend_i2smod = readl(i2s->addr + I2SMOD);
	i2s->suspend_i2scon = readl(i2s->addr + I2SCON);
	i2s->suspend_i2spsr = readl(i2s->addr + I2SPSR);

	if (i2s->op_clk)
		clk_disable_unprepare(i2s->op_clk);
	clk_disable_unprepare(i2s->clk);
=======
	struct samsung_i2s_priv *priv = dev_get_drvdata(dev);

	priv->suspend_i2smod = readl(priv->addr + I2SMOD);
	priv->suspend_i2scon = readl(priv->addr + I2SCON);
	priv->suspend_i2spsr = readl(priv->addr + I2SPSR);

	clk_disable_unprepare(priv->op_clk);
	clk_disable_unprepare(priv->clk);
>>>>>>> upstream/android-13

	return 0;
}

static int i2s_runtime_resume(struct device *dev)
{
<<<<<<< HEAD
	struct i2s_dai *i2s = dev_get_drvdata(dev);
	int ret;

	ret = clk_prepare_enable(i2s->clk);
	if (ret)
		return ret;

	if (i2s->op_clk) {
		ret = clk_prepare_enable(i2s->op_clk);
		if (ret) {
			clk_disable_unprepare(i2s->clk);
=======
	struct samsung_i2s_priv *priv = dev_get_drvdata(dev);
	int ret;

	ret = clk_prepare_enable(priv->clk);
	if (ret)
		return ret;

	if (priv->op_clk) {
		ret = clk_prepare_enable(priv->op_clk);
		if (ret) {
			clk_disable_unprepare(priv->clk);
>>>>>>> upstream/android-13
			return ret;
		}
	}

<<<<<<< HEAD
	writel(i2s->suspend_i2scon, i2s->addr + I2SCON);
	writel(i2s->suspend_i2smod, i2s->addr + I2SMOD);
	writel(i2s->suspend_i2spsr, i2s->addr + I2SPSR);
=======
	writel(priv->suspend_i2scon, priv->addr + I2SCON);
	writel(priv->suspend_i2smod, priv->addr + I2SMOD);
	writel(priv->suspend_i2spsr, priv->addr + I2SPSR);
>>>>>>> upstream/android-13

	return 0;
}
#endif /* CONFIG_PM */

<<<<<<< HEAD
static void i2s_unregister_clocks(struct i2s_dai *i2s)
{
	int i;

	for (i = 0; i < i2s->clk_data.clk_num; i++) {
		if (!IS_ERR(i2s->clk_table[i]))
			clk_unregister(i2s->clk_table[i]);
	}
}

static void i2s_unregister_clock_provider(struct platform_device *pdev)
{
	struct i2s_dai *i2s = dev_get_drvdata(&pdev->dev);

	of_clk_del_provider(pdev->dev.of_node);
	i2s_unregister_clocks(i2s);
}

static int i2s_register_clock_provider(struct platform_device *pdev)
{
	const char * const i2s_clk_desc[] = { "cdclk", "rclk_src", "prescaler" };
	const char *clk_name[2] = { "i2s_opclk0", "i2s_opclk1" };
	const char *p_names[2] = { NULL };
	struct device *dev = &pdev->dev;
	struct i2s_dai *i2s = dev_get_drvdata(dev);
	const struct samsung_i2s_variant_regs *reg_info = i2s->variant_regs;
=======
static void i2s_unregister_clocks(struct samsung_i2s_priv *priv)
{
	int i;

	for (i = 0; i < priv->clk_data.clk_num; i++) {
		if (!IS_ERR(priv->clk_table[i]))
			clk_unregister(priv->clk_table[i]);
	}
}

static void i2s_unregister_clock_provider(struct samsung_i2s_priv *priv)
{
	of_clk_del_provider(priv->pdev->dev.of_node);
	i2s_unregister_clocks(priv);
}


static int i2s_register_clock_provider(struct samsung_i2s_priv *priv)
{

	const char * const i2s_clk_desc[] = { "cdclk", "rclk_src", "prescaler" };
	const char *clk_name[2] = { "i2s_opclk0", "i2s_opclk1" };
	const char *p_names[2] = { NULL };
	struct device *dev = &priv->pdev->dev;
	const struct samsung_i2s_variant_regs *reg_info = priv->variant_regs;
>>>>>>> upstream/android-13
	const char *i2s_clk_name[ARRAY_SIZE(i2s_clk_desc)];
	struct clk *rclksrc;
	int ret, i;

	/* Register the clock provider only if it's expected in the DTB */
	if (!of_find_property(dev->of_node, "#clock-cells", NULL))
		return 0;

	/* Get the RCLKSRC mux clock parent clock names */
	for (i = 0; i < ARRAY_SIZE(p_names); i++) {
		rclksrc = clk_get(dev, clk_name[i]);
		if (IS_ERR(rclksrc))
			continue;
		p_names[i] = __clk_get_name(rclksrc);
		clk_put(rclksrc);
	}

	for (i = 0; i < ARRAY_SIZE(i2s_clk_desc); i++) {
		i2s_clk_name[i] = devm_kasprintf(dev, GFP_KERNEL, "%s_%s",
						dev_name(dev), i2s_clk_desc[i]);
		if (!i2s_clk_name[i])
			return -ENOMEM;
	}

<<<<<<< HEAD
	if (!(i2s->quirks & QUIRK_NO_MUXPSR)) {
		/* Activate the prescaler */
		u32 val = readl(i2s->addr + I2SPSR);
		writel(val | PSR_PSREN, i2s->addr + I2SPSR);

		i2s->clk_table[CLK_I2S_RCLK_SRC] = clk_register_mux(dev,
				i2s_clk_name[CLK_I2S_RCLK_SRC], p_names,
				ARRAY_SIZE(p_names),
				CLK_SET_RATE_NO_REPARENT | CLK_SET_RATE_PARENT,
				i2s->addr + I2SMOD, reg_info->rclksrc_off,
				1, 0, i2s->lock);

		i2s->clk_table[CLK_I2S_RCLK_PSR] = clk_register_divider(dev,
				i2s_clk_name[CLK_I2S_RCLK_PSR],
				i2s_clk_name[CLK_I2S_RCLK_SRC],
				CLK_SET_RATE_PARENT,
				i2s->addr + I2SPSR, 8, 6, 0, i2s->lock);

		p_names[0] = i2s_clk_name[CLK_I2S_RCLK_PSR];
		i2s->clk_data.clk_num = 2;
	}

	i2s->clk_table[CLK_I2S_CDCLK] = clk_register_gate(dev,
				i2s_clk_name[CLK_I2S_CDCLK], p_names[0],
				CLK_SET_RATE_PARENT,
				i2s->addr + I2SMOD, reg_info->cdclkcon_off,
				CLK_GATE_SET_TO_DISABLE, i2s->lock);

	i2s->clk_data.clk_num += 1;
	i2s->clk_data.clks = i2s->clk_table;

	ret = of_clk_add_provider(dev->of_node, of_clk_src_onecell_get,
				  &i2s->clk_data);
	if (ret < 0) {
		dev_err(dev, "failed to add clock provider: %d\n", ret);
		i2s_unregister_clocks(i2s);
=======
	if (!(priv->quirks & QUIRK_NO_MUXPSR)) {
		/* Activate the prescaler */
		u32 val = readl(priv->addr + I2SPSR);
		writel(val | PSR_PSREN, priv->addr + I2SPSR);

		priv->clk_table[CLK_I2S_RCLK_SRC] = clk_register_mux(dev,
				i2s_clk_name[CLK_I2S_RCLK_SRC], p_names,
				ARRAY_SIZE(p_names),
				CLK_SET_RATE_NO_REPARENT | CLK_SET_RATE_PARENT,
				priv->addr + I2SMOD, reg_info->rclksrc_off,
				1, 0, &priv->lock);

		priv->clk_table[CLK_I2S_RCLK_PSR] = clk_register_divider(dev,
				i2s_clk_name[CLK_I2S_RCLK_PSR],
				i2s_clk_name[CLK_I2S_RCLK_SRC],
				CLK_SET_RATE_PARENT,
				priv->addr + I2SPSR, 8, 6, 0, &priv->lock);

		p_names[0] = i2s_clk_name[CLK_I2S_RCLK_PSR];
		priv->clk_data.clk_num = 2;
	}

	priv->clk_table[CLK_I2S_CDCLK] = clk_register_gate(dev,
				i2s_clk_name[CLK_I2S_CDCLK], p_names[0],
				CLK_SET_RATE_PARENT,
				priv->addr + I2SMOD, reg_info->cdclkcon_off,
				CLK_GATE_SET_TO_DISABLE, &priv->lock);

	priv->clk_data.clk_num += 1;
	priv->clk_data.clks = priv->clk_table;

	ret = of_clk_add_provider(dev->of_node, of_clk_src_onecell_get,
				  &priv->clk_data);
	if (ret < 0) {
		dev_err(dev, "failed to add clock provider: %d\n", ret);
		i2s_unregister_clocks(priv);
>>>>>>> upstream/android-13
	}

	return ret;
}

<<<<<<< HEAD
=======
/* Create platform device for the secondary PCM */
static int i2s_create_secondary_device(struct samsung_i2s_priv *priv)
{
	struct platform_device *pdev_sec;
	const char *devname;
	int ret;

	devname = devm_kasprintf(&priv->pdev->dev, GFP_KERNEL, "%s-sec",
				 dev_name(&priv->pdev->dev));
	if (!devname)
		return -ENOMEM;

	pdev_sec = platform_device_alloc(devname, -1);
	if (!pdev_sec)
		return -ENOMEM;

	pdev_sec->driver_override = kstrdup("samsung-i2s", GFP_KERNEL);

	ret = platform_device_add(pdev_sec);
	if (ret < 0) {
		platform_device_put(pdev_sec);
		return ret;
	}

	ret = device_attach(&pdev_sec->dev);
	if (ret <= 0) {
		platform_device_unregister(priv->pdev_sec);
		dev_info(&pdev_sec->dev, "device_attach() failed\n");
		return ret;
	}

	priv->pdev_sec = pdev_sec;

	return 0;
}

static void i2s_delete_secondary_device(struct samsung_i2s_priv *priv)
{
	platform_device_unregister(priv->pdev_sec);
	priv->pdev_sec = NULL;
}

>>>>>>> upstream/android-13
static int samsung_i2s_probe(struct platform_device *pdev)
{
	struct i2s_dai *pri_dai, *sec_dai = NULL;
	struct s3c_audio_pdata *i2s_pdata = pdev->dev.platform_data;
<<<<<<< HEAD
	struct resource *res;
	u32 regs_base, quirks = 0, idma_addr = 0;
	struct device_node *np = pdev->dev.of_node;
	const struct samsung_i2s_dai_data *i2s_dai_data;
	int ret;

	if (IS_ENABLED(CONFIG_OF) && pdev->dev.of_node)
		i2s_dai_data = of_device_get_match_data(&pdev->dev);
	else
		i2s_dai_data = (struct samsung_i2s_dai_data *)
				platform_get_device_id(pdev)->driver_data;

	pri_dai = i2s_alloc_dai(pdev, i2s_dai_data, false);
	if (!pri_dai) {
		dev_err(&pdev->dev, "Unable to alloc I2S_pri\n");
		return -ENOMEM;
	}

	spin_lock_init(&pri_dai->spinlock);
	pri_dai->lock = &pri_dai->spinlock;

	if (!np) {
		if (i2s_pdata == NULL) {
			dev_err(&pdev->dev, "Can't work without s3c_audio_pdata\n");
			return -EINVAL;
		}

=======
	u32 regs_base, idma_addr = 0;
	struct device_node *np = pdev->dev.of_node;
	const struct samsung_i2s_dai_data *i2s_dai_data;
	const struct platform_device_id *id;
	struct samsung_i2s_priv *priv;
	struct resource *res;
	int num_dais, ret;

	if (IS_ENABLED(CONFIG_OF) && pdev->dev.of_node) {
		i2s_dai_data = of_device_get_match_data(&pdev->dev);
	} else {
		id = platform_get_device_id(pdev);

		/* Nothing to do if it is the secondary device probe */
		if (!id)
			return 0;

		i2s_dai_data = (struct samsung_i2s_dai_data *)id->driver_data;
	}

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	if (np) {
		priv->quirks = i2s_dai_data->quirks;
	} else {
		if (!i2s_pdata) {
			dev_err(&pdev->dev, "Missing platform data\n");
			return -EINVAL;
		}
		priv->quirks = i2s_pdata->type.quirks;
	}

	num_dais = (priv->quirks & QUIRK_SEC_DAI) ? 2 : 1;
	priv->pdev = pdev;
	priv->variant_regs = i2s_dai_data->i2s_variant_regs;

	ret = i2s_alloc_dais(priv, i2s_dai_data, num_dais);
	if (ret < 0)
		return ret;

	pri_dai = &priv->dai[SAMSUNG_I2S_ID_PRIMARY - 1];

	spin_lock_init(&priv->lock);
	spin_lock_init(&priv->pcm_lock);

	if (!np) {
>>>>>>> upstream/android-13
		pri_dai->dma_playback.filter_data = i2s_pdata->dma_playback;
		pri_dai->dma_capture.filter_data = i2s_pdata->dma_capture;
		pri_dai->filter = i2s_pdata->dma_filter;

<<<<<<< HEAD
		quirks = i2s_pdata->type.quirks;
		idma_addr = i2s_pdata->type.idma_addr;
	} else {
		quirks = i2s_dai_data->quirks;
		if (of_property_read_u32(np, "samsung,idma-addr",
					 &idma_addr)) {
			if (quirks & QUIRK_SUPPORTS_IDMA) {
=======
		idma_addr = i2s_pdata->type.idma_addr;
	} else {
		if (of_property_read_u32(np, "samsung,idma-addr",
					 &idma_addr)) {
			if (priv->quirks & QUIRK_SUPPORTS_IDMA) {
>>>>>>> upstream/android-13
				dev_info(&pdev->dev, "idma address is not"\
						"specified");
			}
		}
	}
<<<<<<< HEAD
	quirks &= ~(QUIRK_SEC_DAI | QUIRK_SUPPORTS_IDMA);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	pri_dai->addr = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(pri_dai->addr))
		return PTR_ERR(pri_dai->addr);

	regs_base = res->start;

	pri_dai->clk = devm_clk_get(&pdev->dev, "iis");
	if (IS_ERR(pri_dai->clk)) {
		dev_err(&pdev->dev, "Failed to get iis clock\n");
		return PTR_ERR(pri_dai->clk);
	}

	ret = clk_prepare_enable(pri_dai->clk);
=======

	priv->addr = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
	if (IS_ERR(priv->addr))
		return PTR_ERR(priv->addr);

	regs_base = res->start;

	priv->clk = devm_clk_get(&pdev->dev, "iis");
	if (IS_ERR(priv->clk)) {
		dev_err(&pdev->dev, "Failed to get iis clock\n");
		return PTR_ERR(priv->clk);
	}

	ret = clk_prepare_enable(priv->clk);
>>>>>>> upstream/android-13
	if (ret != 0) {
		dev_err(&pdev->dev, "failed to enable clock: %d\n", ret);
		return ret;
	}
	pri_dai->dma_playback.addr = regs_base + I2STXD;
	pri_dai->dma_capture.addr = regs_base + I2SRXD;
	pri_dai->dma_playback.chan_name = "tx";
	pri_dai->dma_capture.chan_name = "rx";
	pri_dai->dma_playback.addr_width = 4;
	pri_dai->dma_capture.addr_width = 4;
<<<<<<< HEAD
	pri_dai->quirks = quirks;
	pri_dai->variant_regs = i2s_dai_data->i2s_variant_regs;

	if (quirks & QUIRK_PRI_6CHAN)
		pri_dai->i2s_dai_drv.playback.channels_max = 6;

	ret = samsung_asoc_dma_platform_register(&pdev->dev, pri_dai->filter,
						 NULL, NULL);
	if (ret < 0)
		goto err_disable_clk;

	ret = devm_snd_soc_register_component(&pdev->dev,
					&samsung_i2s_component,
					&pri_dai->i2s_dai_drv, 1);
	if (ret < 0)
		goto err_disable_clk;

	if (quirks & QUIRK_SEC_DAI) {
		sec_dai = i2s_alloc_dai(pdev, i2s_dai_data, true);
		if (!sec_dai) {
			dev_err(&pdev->dev, "Unable to alloc I2S_sec\n");
			ret = -ENOMEM;
			goto err_disable_clk;
		}

		sec_dai->lock = &pri_dai->spinlock;
		sec_dai->variant_regs = pri_dai->variant_regs;
=======
	pri_dai->priv = priv;

	if (priv->quirks & QUIRK_PRI_6CHAN)
		pri_dai->drv->playback.channels_max = 6;

	ret = samsung_asoc_dma_platform_register(&pdev->dev, pri_dai->filter,
						 "tx", "rx", NULL);
	if (ret < 0)
		goto err_disable_clk;

	if (priv->quirks & QUIRK_SEC_DAI) {
		sec_dai = &priv->dai[SAMSUNG_I2S_ID_SECONDARY - 1];

>>>>>>> upstream/android-13
		sec_dai->dma_playback.addr = regs_base + I2STXDS;
		sec_dai->dma_playback.chan_name = "tx-sec";

		if (!np) {
			sec_dai->dma_playback.filter_data = i2s_pdata->dma_play_sec;
			sec_dai->filter = i2s_pdata->dma_filter;
		}

		sec_dai->dma_playback.addr_width = 4;
<<<<<<< HEAD
		sec_dai->addr = pri_dai->addr;
		sec_dai->clk = pri_dai->clk;
		sec_dai->quirks = quirks;
		sec_dai->idma_playback.addr = idma_addr;
		sec_dai->pri_dai = pri_dai;
		pri_dai->sec_dai = sec_dai;

		ret = samsung_asoc_dma_platform_register(&pdev->dev,
					sec_dai->filter, "tx-sec", NULL);
		if (ret < 0)
			goto err_disable_clk;

		ret = devm_snd_soc_register_component(&pdev->dev,
						&samsung_i2s_component,
						&sec_dai->i2s_dai_drv, 1);
		if (ret < 0)
			goto err_disable_clk;
=======
		sec_dai->idma_playback.addr = idma_addr;
		sec_dai->pri_dai = pri_dai;
		sec_dai->priv = priv;
		pri_dai->sec_dai = sec_dai;

		ret = i2s_create_secondary_device(priv);
		if (ret < 0)
			goto err_disable_clk;

		ret = samsung_asoc_dma_platform_register(&priv->pdev_sec->dev,
						sec_dai->filter, "tx-sec", NULL,
						&pdev->dev);
		if (ret < 0)
			goto err_del_sec;

>>>>>>> upstream/android-13
	}

	if (i2s_pdata && i2s_pdata->cfg_gpio && i2s_pdata->cfg_gpio(pdev)) {
		dev_err(&pdev->dev, "Unable to configure gpio\n");
		ret = -EINVAL;
<<<<<<< HEAD
		goto err_disable_clk;
	}

	dev_set_drvdata(&pdev->dev, pri_dai);
=======
		goto err_del_sec;
	}

	dev_set_drvdata(&pdev->dev, priv);

	ret = devm_snd_soc_register_component(&pdev->dev,
					&samsung_i2s_component,
					priv->dai_drv, num_dais);
	if (ret < 0)
		goto err_del_sec;
>>>>>>> upstream/android-13

	pm_runtime_set_active(&pdev->dev);
	pm_runtime_enable(&pdev->dev);

<<<<<<< HEAD
	ret = i2s_register_clock_provider(pdev);
	if (ret < 0)
		goto err_disable_pm;

	pri_dai->op_clk = clk_get_parent(pri_dai->clk_table[CLK_I2S_RCLK_SRC]);
=======
	ret = i2s_register_clock_provider(priv);
	if (ret < 0)
		goto err_disable_pm;

	priv->op_clk = clk_get_parent(priv->clk_table[CLK_I2S_RCLK_SRC]);
>>>>>>> upstream/android-13

	return 0;

err_disable_pm:
	pm_runtime_disable(&pdev->dev);
<<<<<<< HEAD
err_disable_clk:
	clk_disable_unprepare(pri_dai->clk);
=======
err_del_sec:
	i2s_delete_secondary_device(priv);
err_disable_clk:
	clk_disable_unprepare(priv->clk);
>>>>>>> upstream/android-13
	return ret;
}

static int samsung_i2s_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct i2s_dai *pri_dai;

	pri_dai = dev_get_drvdata(&pdev->dev);
=======
	struct samsung_i2s_priv *priv = dev_get_drvdata(&pdev->dev);

	/* The secondary device has no driver data assigned */
	if (!priv)
		return 0;
>>>>>>> upstream/android-13

	pm_runtime_get_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

<<<<<<< HEAD
	i2s_unregister_clock_provider(pdev);
	clk_disable_unprepare(pri_dai->clk);
=======
	i2s_unregister_clock_provider(priv);
	i2s_delete_secondary_device(priv);
	clk_disable_unprepare(priv->clk);

>>>>>>> upstream/android-13
	pm_runtime_put_noidle(&pdev->dev);

	return 0;
}

static const struct samsung_i2s_variant_regs i2sv3_regs = {
	.bfs_off = 1,
	.rfs_off = 3,
	.sdf_off = 5,
	.txr_off = 8,
	.rclksrc_off = 10,
	.mss_off = 11,
	.cdclkcon_off = 12,
	.lrp_off = 7,
	.bfs_mask = 0x3,
	.rfs_mask = 0x3,
	.ftx0cnt_off = 8,
};

static const struct samsung_i2s_variant_regs i2sv6_regs = {
	.bfs_off = 0,
	.rfs_off = 4,
	.sdf_off = 6,
	.txr_off = 8,
	.rclksrc_off = 10,
	.mss_off = 11,
	.cdclkcon_off = 12,
	.lrp_off = 15,
	.bfs_mask = 0xf,
	.rfs_mask = 0x3,
	.ftx0cnt_off = 8,
};

static const struct samsung_i2s_variant_regs i2sv7_regs = {
	.bfs_off = 0,
	.rfs_off = 4,
	.sdf_off = 7,
	.txr_off = 9,
	.rclksrc_off = 11,
	.mss_off = 12,
	.cdclkcon_off = 22,
	.lrp_off = 15,
	.bfs_mask = 0xf,
	.rfs_mask = 0x7,
	.ftx0cnt_off = 0,
};

static const struct samsung_i2s_variant_regs i2sv5_i2s1_regs = {
	.bfs_off = 0,
	.rfs_off = 3,
	.sdf_off = 6,
	.txr_off = 8,
	.rclksrc_off = 10,
	.mss_off = 11,
	.cdclkcon_off = 12,
	.lrp_off = 15,
	.bfs_mask = 0x7,
	.rfs_mask = 0x7,
	.ftx0cnt_off = 8,
};

static const struct samsung_i2s_dai_data i2sv3_dai_type = {
	.quirks = QUIRK_NO_MUXPSR,
	.pcm_rates = SNDRV_PCM_RATE_8000_96000,
	.i2s_variant_regs = &i2sv3_regs,
};

<<<<<<< HEAD
static const struct samsung_i2s_dai_data i2sv5_dai_type = {
=======
static const struct samsung_i2s_dai_data i2sv5_dai_type __maybe_unused = {
>>>>>>> upstream/android-13
	.quirks = QUIRK_PRI_6CHAN | QUIRK_SEC_DAI | QUIRK_NEED_RSTCLR |
			QUIRK_SUPPORTS_IDMA,
	.pcm_rates = SNDRV_PCM_RATE_8000_96000,
	.i2s_variant_regs = &i2sv3_regs,
};

<<<<<<< HEAD
static const struct samsung_i2s_dai_data i2sv6_dai_type = {
=======
static const struct samsung_i2s_dai_data i2sv6_dai_type __maybe_unused = {
>>>>>>> upstream/android-13
	.quirks = QUIRK_PRI_6CHAN | QUIRK_SEC_DAI | QUIRK_NEED_RSTCLR |
			QUIRK_SUPPORTS_TDM | QUIRK_SUPPORTS_IDMA,
	.pcm_rates = SNDRV_PCM_RATE_8000_96000,
	.i2s_variant_regs = &i2sv6_regs,
};

<<<<<<< HEAD
static const struct samsung_i2s_dai_data i2sv7_dai_type = {
=======
static const struct samsung_i2s_dai_data i2sv7_dai_type __maybe_unused = {
>>>>>>> upstream/android-13
	.quirks = QUIRK_PRI_6CHAN | QUIRK_SEC_DAI | QUIRK_NEED_RSTCLR |
			QUIRK_SUPPORTS_TDM,
	.pcm_rates = SNDRV_PCM_RATE_8000_192000,
	.i2s_variant_regs = &i2sv7_regs,
};

<<<<<<< HEAD
static const struct samsung_i2s_dai_data i2sv5_dai_type_i2s1 = {
=======
static const struct samsung_i2s_dai_data i2sv5_dai_type_i2s1 __maybe_unused = {
>>>>>>> upstream/android-13
	.quirks = QUIRK_PRI_6CHAN | QUIRK_NEED_RSTCLR,
	.pcm_rates = SNDRV_PCM_RATE_8000_96000,
	.i2s_variant_regs = &i2sv5_i2s1_regs,
};

static const struct platform_device_id samsung_i2s_driver_ids[] = {
	{
		.name           = "samsung-i2s",
		.driver_data	= (kernel_ulong_t)&i2sv3_dai_type,
	},
	{},
};
MODULE_DEVICE_TABLE(platform, samsung_i2s_driver_ids);

#ifdef CONFIG_OF
static const struct of_device_id exynos_i2s_match[] = {
	{
		.compatible = "samsung,s3c6410-i2s",
		.data = &i2sv3_dai_type,
	}, {
		.compatible = "samsung,s5pv210-i2s",
		.data = &i2sv5_dai_type,
	}, {
		.compatible = "samsung,exynos5420-i2s",
		.data = &i2sv6_dai_type,
	}, {
		.compatible = "samsung,exynos7-i2s",
		.data = &i2sv7_dai_type,
	}, {
		.compatible = "samsung,exynos7-i2s1",
		.data = &i2sv5_dai_type_i2s1,
	},
	{},
};
MODULE_DEVICE_TABLE(of, exynos_i2s_match);
#endif

static const struct dev_pm_ops samsung_i2s_pm = {
	SET_RUNTIME_PM_OPS(i2s_runtime_suspend,
				i2s_runtime_resume, NULL)
	SET_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
				     pm_runtime_force_resume)
};

static struct platform_driver samsung_i2s_driver = {
	.probe  = samsung_i2s_probe,
	.remove = samsung_i2s_remove,
	.id_table = samsung_i2s_driver_ids,
	.driver = {
		.name = "samsung-i2s",
		.of_match_table = of_match_ptr(exynos_i2s_match),
		.pm = &samsung_i2s_pm,
	},
};

module_platform_driver(samsung_i2s_driver);

/* Module information */
MODULE_AUTHOR("Jaswinder Singh, <jassisinghbrar@gmail.com>");
MODULE_DESCRIPTION("Samsung I2S Interface");
MODULE_ALIAS("platform:samsung-i2s");
MODULE_LICENSE("GPL");
