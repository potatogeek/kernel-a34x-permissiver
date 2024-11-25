<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * linux/sound/soc/pxa/mmp-sspa.c
 * Base on pxa2xx-ssp.c
 *
 * Copyright (C) 2011 Marvell International Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
=======
>>>>>>> upstream/android-13
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/pxa2xx_ssp.h>
#include <linux/io.h>
#include <linux/dmaengine.h>
=======
#include <linux/io.h>
#include <linux/dmaengine.h>
#include <linux/pm_runtime.h>
>>>>>>> upstream/android-13

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/pxa2xx-lib.h>
#include <sound/dmaengine_pcm.h>
#include "mmp-sspa.h"

/*
 * SSPA audio private data
 */
struct sspa_priv {
<<<<<<< HEAD
	struct ssp_device *sspa;
	struct snd_dmaengine_dai_dma_data *dma_params;
	struct clk *audio_clk;
	struct clk *sysclk;
	int dai_fmt;
	int running_cnt;
};

static void mmp_sspa_write_reg(struct ssp_device *sspa, u32 reg, u32 val)
{
	__raw_writel(val, sspa->mmio_base + reg);
}

static u32 mmp_sspa_read_reg(struct ssp_device *sspa, u32 reg)
{
	return __raw_readl(sspa->mmio_base + reg);
}

static void mmp_sspa_tx_enable(struct ssp_device *sspa)
{
	unsigned int sspa_sp;

	sspa_sp = mmp_sspa_read_reg(sspa, SSPA_TXSP);
	sspa_sp |= SSPA_SP_S_EN;
	sspa_sp |= SSPA_SP_WEN;
	mmp_sspa_write_reg(sspa, SSPA_TXSP, sspa_sp);
}

static void mmp_sspa_tx_disable(struct ssp_device *sspa)
{
	unsigned int sspa_sp;

	sspa_sp = mmp_sspa_read_reg(sspa, SSPA_TXSP);
	sspa_sp &= ~SSPA_SP_S_EN;
	sspa_sp |= SSPA_SP_WEN;
	mmp_sspa_write_reg(sspa, SSPA_TXSP, sspa_sp);
}

static void mmp_sspa_rx_enable(struct ssp_device *sspa)
{
	unsigned int sspa_sp;

	sspa_sp = mmp_sspa_read_reg(sspa, SSPA_RXSP);
	sspa_sp |= SSPA_SP_S_EN;
	sspa_sp |= SSPA_SP_WEN;
	mmp_sspa_write_reg(sspa, SSPA_RXSP, sspa_sp);
}

static void mmp_sspa_rx_disable(struct ssp_device *sspa)
{
	unsigned int sspa_sp;

	sspa_sp = mmp_sspa_read_reg(sspa, SSPA_RXSP);
	sspa_sp &= ~SSPA_SP_S_EN;
	sspa_sp |= SSPA_SP_WEN;
	mmp_sspa_write_reg(sspa, SSPA_RXSP, sspa_sp);
=======
	void __iomem *tx_base;
	void __iomem *rx_base;

	struct snd_dmaengine_dai_dma_data playback_dma_data;
	struct snd_dmaengine_dai_dma_data capture_dma_data;
	struct clk *clk;
	struct clk *audio_clk;
	struct clk *sysclk;

	int running_cnt;
	u32 sp;
	u32 ctrl;
};

static void mmp_sspa_tx_enable(struct sspa_priv *sspa)
{
	unsigned int sspa_sp = sspa->sp;

	sspa_sp &= ~SSPA_SP_MSL;
	sspa_sp |= SSPA_SP_S_EN;
	sspa_sp |= SSPA_SP_WEN;
	__raw_writel(sspa_sp, sspa->tx_base + SSPA_SP);
}

static void mmp_sspa_tx_disable(struct sspa_priv *sspa)
{
	unsigned int sspa_sp = sspa->sp;

	sspa_sp &= ~SSPA_SP_MSL;
	sspa_sp &= ~SSPA_SP_S_EN;
	sspa_sp |= SSPA_SP_WEN;
	__raw_writel(sspa_sp, sspa->tx_base + SSPA_SP);
}

static void mmp_sspa_rx_enable(struct sspa_priv *sspa)
{
	unsigned int sspa_sp = sspa->sp;

	sspa_sp |= SSPA_SP_S_EN;
	sspa_sp |= SSPA_SP_WEN;
	__raw_writel(sspa_sp, sspa->rx_base + SSPA_SP);
}

static void mmp_sspa_rx_disable(struct sspa_priv *sspa)
{
	unsigned int sspa_sp = sspa->sp;

	sspa_sp &= ~SSPA_SP_S_EN;
	sspa_sp |= SSPA_SP_WEN;
	__raw_writel(sspa_sp, sspa->rx_base + SSPA_SP);
>>>>>>> upstream/android-13
}

static int mmp_sspa_startup(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct sspa_priv *priv = snd_soc_dai_get_drvdata(dai);

	clk_enable(priv->sysclk);
	clk_enable(priv->sspa->clk);
=======
	struct sspa_priv *sspa = snd_soc_dai_get_drvdata(dai);

	clk_prepare_enable(sspa->sysclk);
	clk_prepare_enable(sspa->clk);
>>>>>>> upstream/android-13

	return 0;
}

static void mmp_sspa_shutdown(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct sspa_priv *priv = snd_soc_dai_get_drvdata(dai);

	clk_disable(priv->sspa->clk);
	clk_disable(priv->sysclk);

=======
	struct sspa_priv *sspa = snd_soc_dai_get_drvdata(dai);

	clk_disable_unprepare(sspa->clk);
	clk_disable_unprepare(sspa->sysclk);
>>>>>>> upstream/android-13
}

/*
 * Set the SSP ports SYSCLK.
 */
static int mmp_sspa_set_dai_sysclk(struct snd_soc_dai *cpu_dai,
				    int clk_id, unsigned int freq, int dir)
{
<<<<<<< HEAD
	struct sspa_priv *priv = snd_soc_dai_get_drvdata(cpu_dai);
	int ret = 0;

	switch (clk_id) {
	case MMP_SSPA_CLK_AUDIO:
		ret = clk_set_rate(priv->audio_clk, freq);
=======
	struct sspa_priv *sspa = snd_soc_dai_get_drvdata(cpu_dai);
	struct device *dev = cpu_dai->component->dev;
	int ret = 0;

	if (dev->of_node)
		return -ENOTSUPP;

	switch (clk_id) {
	case MMP_SSPA_CLK_AUDIO:
		ret = clk_set_rate(sspa->audio_clk, freq);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
		break;
	case MMP_SSPA_CLK_PLL:
	case MMP_SSPA_CLK_VCXO:
		/* not support yet */
		return -EINVAL;
	default:
		return -EINVAL;
	}

	return 0;
}

static int mmp_sspa_set_dai_pll(struct snd_soc_dai *cpu_dai, int pll_id,
				 int source, unsigned int freq_in,
				 unsigned int freq_out)
{
<<<<<<< HEAD
	struct sspa_priv *priv = snd_soc_dai_get_drvdata(cpu_dai);
	int ret = 0;

	switch (pll_id) {
	case MMP_SYSCLK:
		ret = clk_set_rate(priv->sysclk, freq_out);
=======
	struct sspa_priv *sspa = snd_soc_dai_get_drvdata(cpu_dai);
	struct device *dev = cpu_dai->component->dev;
	int ret = 0;

	if (dev->of_node)
		return -ENOTSUPP;

	switch (pll_id) {
	case MMP_SYSCLK:
		ret = clk_set_rate(sspa->sysclk, freq_out);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
		break;
	case MMP_SSPA_CLK:
<<<<<<< HEAD
		ret = clk_set_rate(priv->sspa->clk, freq_out);
=======
		ret = clk_set_rate(sspa->clk, freq_out);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
		break;
	default:
		return -ENODEV;
	}

	return 0;
}

/*
<<<<<<< HEAD
 * Set up the sspa dai format. The sspa port must be inactive
 * before calling this function as the physical
 * interface format is changed.
=======
 * Set up the sspa dai format.
>>>>>>> upstream/android-13
 */
static int mmp_sspa_set_dai_fmt(struct snd_soc_dai *cpu_dai,
				 unsigned int fmt)
{
<<<<<<< HEAD
	struct sspa_priv *sspa_priv = snd_soc_dai_get_drvdata(cpu_dai);
	struct ssp_device *sspa = sspa_priv->sspa;
	u32 sspa_sp, sspa_ctrl;

	/* check if we need to change anything at all */
	if (sspa_priv->dai_fmt == fmt)
		return 0;

	/* we can only change the settings if the port is not in use */
	if ((mmp_sspa_read_reg(sspa, SSPA_TXSP) & SSPA_SP_S_EN) ||
	    (mmp_sspa_read_reg(sspa, SSPA_RXSP) & SSPA_SP_S_EN)) {
		dev_err(&sspa->pdev->dev,
			"can't change hardware dai format: stream is in use\n");
		return -EINVAL;
	}

	/* reset port settings */
	sspa_sp   = SSPA_SP_WEN | SSPA_SP_S_RST | SSPA_SP_FFLUSH;
	sspa_ctrl = 0;

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBS_CFS:
		sspa_sp |= SSPA_SP_MSL;
=======
	struct sspa_priv *sspa = snd_soc_dai_get_drvdata(cpu_dai);

	/* reset port settings */
	sspa->sp   = SSPA_SP_WEN | SSPA_SP_S_RST | SSPA_SP_FFLUSH;
	sspa->ctrl = 0;

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBS_CFS:
		sspa->sp |= SSPA_SP_MSL;
>>>>>>> upstream/android-13
		break;
	case SND_SOC_DAIFMT_CBM_CFM:
		break;
	default:
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
<<<<<<< HEAD
		sspa_sp |= SSPA_SP_FSP;
=======
		sspa->sp |= SSPA_SP_FSP;
>>>>>>> upstream/android-13
		break;
	default:
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
<<<<<<< HEAD
		sspa_sp |= SSPA_TXSP_FPER(63);
		sspa_sp |= SSPA_SP_FWID(31);
		sspa_ctrl |= SSPA_CTL_XDATDLY(1);
=======
		sspa->ctrl |= SSPA_CTL_XDATDLY(1);
>>>>>>> upstream/android-13
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	mmp_sspa_write_reg(sspa, SSPA_TXSP, sspa_sp);
	mmp_sspa_write_reg(sspa, SSPA_RXSP, sspa_sp);

	sspa_sp &= ~(SSPA_SP_S_RST | SSPA_SP_FFLUSH);
	mmp_sspa_write_reg(sspa, SSPA_TXSP, sspa_sp);
	mmp_sspa_write_reg(sspa, SSPA_RXSP, sspa_sp);

	/*
	 * FIXME: hw issue, for the tx serial port,
	 * can not config the master/slave mode;
	 * so must clean this bit.
	 * The master/slave mode has been set in the
	 * rx port.
	 */
	sspa_sp &= ~SSPA_SP_MSL;
	mmp_sspa_write_reg(sspa, SSPA_TXSP, sspa_sp);

	mmp_sspa_write_reg(sspa, SSPA_TXCTL, sspa_ctrl);
	mmp_sspa_write_reg(sspa, SSPA_RXCTL, sspa_ctrl);

=======
>>>>>>> upstream/android-13
	/* Since we are configuring the timings for the format by hand
	 * we have to defer some things until hw_params() where we
	 * know parameters like the sample size.
	 */
<<<<<<< HEAD
	sspa_priv->dai_fmt = fmt;
=======
>>>>>>> upstream/android-13
	return 0;
}

/*
 * Set the SSPA audio DMA parameters and sample size.
 * Can be called multiple times by oss emulation.
 */
static int mmp_sspa_hw_params(struct snd_pcm_substream *substream,
			       struct snd_pcm_hw_params *params,
			       struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct sspa_priv *sspa_priv = snd_soc_dai_get_drvdata(dai);
	struct ssp_device *sspa = sspa_priv->sspa;
	struct snd_dmaengine_dai_dma_data *dma_params;
	u32 sspa_ctrl;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		sspa_ctrl = mmp_sspa_read_reg(sspa, SSPA_TXCTL);
	else
		sspa_ctrl = mmp_sspa_read_reg(sspa, SSPA_RXCTL);

	sspa_ctrl &= ~SSPA_CTL_XFRLEN1_MASK;
	sspa_ctrl |= SSPA_CTL_XFRLEN1(params_channels(params) - 1);
	sspa_ctrl &= ~SSPA_CTL_XWDLEN1_MASK;
	sspa_ctrl |= SSPA_CTL_XWDLEN1(SSPA_CTL_32_BITS);
	sspa_ctrl &= ~SSPA_CTL_XSSZ1_MASK;

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S8:
		sspa_ctrl |= SSPA_CTL_XSSZ1(SSPA_CTL_8_BITS);
		break;
	case SNDRV_PCM_FORMAT_S16_LE:
		sspa_ctrl |= SSPA_CTL_XSSZ1(SSPA_CTL_16_BITS);
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		sspa_ctrl |= SSPA_CTL_XSSZ1(SSPA_CTL_20_BITS);
		break;
	case SNDRV_PCM_FORMAT_S24_3LE:
		sspa_ctrl |= SSPA_CTL_XSSZ1(SSPA_CTL_24_BITS);
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		sspa_ctrl |= SSPA_CTL_XSSZ1(SSPA_CTL_32_BITS);
=======
	struct sspa_priv *sspa = snd_soc_dai_get_drvdata(dai);
	struct device *dev = dai->component->dev;
	u32 sspa_ctrl = sspa->ctrl;
	int bits;
	int bitval;

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S8:
		bits = 8;
		bitval = SSPA_CTL_8_BITS;
		break;
	case SNDRV_PCM_FORMAT_S16_LE:
		bits = 16;
		bitval = SSPA_CTL_16_BITS;
		break;
	case SNDRV_PCM_FORMAT_S24_3LE:
		bits = 24;
		bitval = SSPA_CTL_24_BITS;
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		bits = 32;
		bitval = SSPA_CTL_32_BITS;
>>>>>>> upstream/android-13
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		mmp_sspa_write_reg(sspa, SSPA_TXCTL, sspa_ctrl);
		mmp_sspa_write_reg(sspa, SSPA_TXFIFO_LL, 0x1);
	} else {
		mmp_sspa_write_reg(sspa, SSPA_RXCTL, sspa_ctrl);
		mmp_sspa_write_reg(sspa, SSPA_RXFIFO_UL, 0x0);
	}

	dma_params = &sspa_priv->dma_params[substream->stream];
	dma_params->addr = substream->stream == SNDRV_PCM_STREAM_PLAYBACK ?
				(sspa->phys_base + SSPA_TXD) :
				(sspa->phys_base + SSPA_RXD);
	snd_soc_dai_set_dma_data(cpu_dai, substream, dma_params);
=======
	sspa_ctrl &= ~SSPA_CTL_XPH;
	if (dev->of_node || params_channels(params) == 2)
		sspa_ctrl |= SSPA_CTL_XPH;

	sspa_ctrl &= ~SSPA_CTL_XWDLEN1_MASK;
	sspa_ctrl |= SSPA_CTL_XWDLEN1(bitval);

	sspa_ctrl &= ~SSPA_CTL_XWDLEN2_MASK;
	sspa_ctrl |= SSPA_CTL_XWDLEN2(bitval);

	sspa_ctrl &= ~SSPA_CTL_XSSZ1_MASK;
	sspa_ctrl |= SSPA_CTL_XSSZ1(bitval);

	sspa_ctrl &= ~SSPA_CTL_XSSZ2_MASK;
	sspa_ctrl |= SSPA_CTL_XSSZ2(bitval);

	sspa->sp &= ~SSPA_SP_FWID_MASK;
	sspa->sp |= SSPA_SP_FWID(bits - 1);

	sspa->sp &= ~SSPA_TXSP_FPER_MASK;
	sspa->sp |= SSPA_TXSP_FPER(bits * 2 - 1);

	if (dev->of_node) {
		clk_set_rate(sspa->clk, params_rate(params) *
					params_channels(params) * bits);
	}

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		__raw_writel(sspa_ctrl, sspa->tx_base + SSPA_CTL);
		__raw_writel(0x1, sspa->tx_base + SSPA_FIFO_UL);
	} else {
		__raw_writel(sspa_ctrl, sspa->rx_base + SSPA_CTL);
		__raw_writel(0x0, sspa->rx_base + SSPA_FIFO_UL);
	}

>>>>>>> upstream/android-13
	return 0;
}

static int mmp_sspa_trigger(struct snd_pcm_substream *substream, int cmd,
			     struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct sspa_priv *sspa_priv = snd_soc_dai_get_drvdata(dai);
	struct ssp_device *sspa = sspa_priv->sspa;
=======
	struct sspa_priv *sspa = snd_soc_dai_get_drvdata(dai);
>>>>>>> upstream/android-13
	int ret = 0;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		/*
		 * whatever playback or capture, must enable rx.
		 * this is a hw issue, so need check if rx has been
		 * enabled or not; if has been enabled by another
		 * stream, do not enable again.
		 */
<<<<<<< HEAD
		if (!sspa_priv->running_cnt)
=======
		if (!sspa->running_cnt)
>>>>>>> upstream/android-13
			mmp_sspa_rx_enable(sspa);

		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			mmp_sspa_tx_enable(sspa);

<<<<<<< HEAD
		sspa_priv->running_cnt++;
=======
		sspa->running_cnt++;
>>>>>>> upstream/android-13
		break;

	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
<<<<<<< HEAD
		sspa_priv->running_cnt--;
=======
		sspa->running_cnt--;
>>>>>>> upstream/android-13

		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			mmp_sspa_tx_disable(sspa);

		/* have no capture stream, disable rx port */
<<<<<<< HEAD
		if (!sspa_priv->running_cnt)
=======
		if (!sspa->running_cnt)
>>>>>>> upstream/android-13
			mmp_sspa_rx_disable(sspa);
		break;

	default:
		ret = -EINVAL;
	}

	return ret;
}

static int mmp_sspa_probe(struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct sspa_priv *priv = dev_get_drvdata(dai->dev);

	snd_soc_dai_set_drvdata(dai, priv);
	return 0;

=======
	struct sspa_priv *sspa = dev_get_drvdata(dai->dev);

	snd_soc_dai_init_dma_data(dai,
				&sspa->playback_dma_data,
				&sspa->capture_dma_data);

	return 0;
>>>>>>> upstream/android-13
}

#define MMP_SSPA_RATES SNDRV_PCM_RATE_8000_192000
#define MMP_SSPA_FORMATS (SNDRV_PCM_FMTBIT_S8 | \
		SNDRV_PCM_FMTBIT_S16_LE | \
<<<<<<< HEAD
		SNDRV_PCM_FMTBIT_S24_LE | \
=======
		SNDRV_PCM_FMTBIT_S24_3LE | \
>>>>>>> upstream/android-13
		SNDRV_PCM_FMTBIT_S32_LE)

static const struct snd_soc_dai_ops mmp_sspa_dai_ops = {
	.startup	= mmp_sspa_startup,
	.shutdown	= mmp_sspa_shutdown,
	.trigger	= mmp_sspa_trigger,
	.hw_params	= mmp_sspa_hw_params,
	.set_sysclk	= mmp_sspa_set_dai_sysclk,
	.set_pll	= mmp_sspa_set_dai_pll,
	.set_fmt	= mmp_sspa_set_dai_fmt,
};

static struct snd_soc_dai_driver mmp_sspa_dai = {
	.probe = mmp_sspa_probe,
	.playback = {
		.channels_min = 1,
		.channels_max = 128,
		.rates = MMP_SSPA_RATES,
		.formats = MMP_SSPA_FORMATS,
	},
	.capture = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = MMP_SSPA_RATES,
		.formats = MMP_SSPA_FORMATS,
	},
	.ops = &mmp_sspa_dai_ops,
};

<<<<<<< HEAD
static const struct snd_soc_component_driver mmp_sspa_component = {
	.name		= "mmp-sspa",
=======
#define MMP_PCM_INFO (SNDRV_PCM_INFO_MMAP |	\
		SNDRV_PCM_INFO_MMAP_VALID |	\
		SNDRV_PCM_INFO_INTERLEAVED |	\
		SNDRV_PCM_INFO_PAUSE |		\
		SNDRV_PCM_INFO_RESUME |		\
		SNDRV_PCM_INFO_NO_PERIOD_WAKEUP)

static const struct snd_pcm_hardware mmp_pcm_hardware[] = {
	{
		.info			= MMP_PCM_INFO,
		.period_bytes_min	= 1024,
		.period_bytes_max	= 2048,
		.periods_min		= 2,
		.periods_max		= 32,
		.buffer_bytes_max	= 4096,
		.fifo_size		= 32,
	},
	{
		.info			= MMP_PCM_INFO,
		.period_bytes_min	= 1024,
		.period_bytes_max	= 2048,
		.periods_min		= 2,
		.periods_max		= 32,
		.buffer_bytes_max	= 4096,
		.fifo_size		= 32,
	},
};

static const struct snd_dmaengine_pcm_config mmp_pcm_config = {
	.prepare_slave_config = snd_dmaengine_pcm_prepare_slave_config,
	.pcm_hardware = mmp_pcm_hardware,
	.prealloc_buffer_size = 4096,
};

static int mmp_pcm_mmap(struct snd_soc_component *component,
			struct snd_pcm_substream *substream,
			struct vm_area_struct *vma)
{
	vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	return remap_pfn_range(vma, vma->vm_start,
		substream->dma_buffer.addr >> PAGE_SHIFT,
		vma->vm_end - vma->vm_start, vma->vm_page_prot);
}

static int mmp_sspa_open(struct snd_soc_component *component,
			 struct snd_pcm_substream *substream)
{
	struct sspa_priv *sspa = snd_soc_component_get_drvdata(component);

	pm_runtime_get_sync(component->dev);

	/* we can only change the settings if the port is not in use */
	if ((__raw_readl(sspa->tx_base + SSPA_SP) & SSPA_SP_S_EN) ||
	    (__raw_readl(sspa->rx_base + SSPA_SP) & SSPA_SP_S_EN)) {
		dev_err(component->dev,
			"can't change hardware dai format: stream is in use\n");
		return -EBUSY;
	}

	__raw_writel(sspa->sp, sspa->tx_base + SSPA_SP);
	__raw_writel(sspa->sp, sspa->rx_base + SSPA_SP);

	sspa->sp &= ~(SSPA_SP_S_RST | SSPA_SP_FFLUSH);
	__raw_writel(sspa->sp, sspa->tx_base + SSPA_SP);
	__raw_writel(sspa->sp, sspa->rx_base + SSPA_SP);

	/*
	 * FIXME: hw issue, for the tx serial port,
	 * can not config the master/slave mode;
	 * so must clean this bit.
	 * The master/slave mode has been set in the
	 * rx port.
	 */
	__raw_writel(sspa->sp & ~SSPA_SP_MSL, sspa->tx_base + SSPA_SP);

	__raw_writel(sspa->ctrl, sspa->tx_base + SSPA_CTL);
	__raw_writel(sspa->ctrl, sspa->rx_base + SSPA_CTL);

	return 0;
}

static int mmp_sspa_close(struct snd_soc_component *component,
			  struct snd_pcm_substream *substream)
{
	pm_runtime_put_sync(component->dev);
	return 0;
}

static const struct snd_soc_component_driver mmp_sspa_component = {
	.name		= "mmp-sspa",
	.mmap		= mmp_pcm_mmap,
	.open		= mmp_sspa_open,
	.close		= mmp_sspa_close,
>>>>>>> upstream/android-13
};

static int asoc_mmp_sspa_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct sspa_priv *priv;
	struct resource *res;

	priv = devm_kzalloc(&pdev->dev,
				sizeof(struct sspa_priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->sspa = devm_kzalloc(&pdev->dev,
				sizeof(struct ssp_device), GFP_KERNEL);
	if (priv->sspa == NULL)
		return -ENOMEM;

	priv->dma_params = devm_kcalloc(&pdev->dev,
			2, sizeof(struct snd_dmaengine_dai_dma_data),
			GFP_KERNEL);
	if (priv->dma_params == NULL)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	priv->sspa->mmio_base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(priv->sspa->mmio_base))
		return PTR_ERR(priv->sspa->mmio_base);

	priv->sspa->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->sspa->clk))
		return PTR_ERR(priv->sspa->clk);

	priv->audio_clk = clk_get(NULL, "mmp-audio");
	if (IS_ERR(priv->audio_clk))
		return PTR_ERR(priv->audio_clk);

	priv->sysclk = clk_get(NULL, "mmp-sysclk");
	if (IS_ERR(priv->sysclk)) {
		clk_put(priv->audio_clk);
		return PTR_ERR(priv->sysclk);
	}
	clk_enable(priv->audio_clk);
	priv->dai_fmt = (unsigned int) -1;
	platform_set_drvdata(pdev, priv);

	return devm_snd_soc_register_component(&pdev->dev, &mmp_sspa_component,
					       &mmp_sspa_dai, 1);
=======
	struct sspa_priv *sspa;
	int ret;

	sspa = devm_kzalloc(&pdev->dev,
				sizeof(struct sspa_priv), GFP_KERNEL);
	if (!sspa)
		return -ENOMEM;

	if (pdev->dev.of_node) {
		sspa->rx_base = devm_platform_ioremap_resource(pdev, 0);
		if (IS_ERR(sspa->rx_base))
			return PTR_ERR(sspa->rx_base);

		sspa->tx_base = devm_platform_ioremap_resource(pdev, 1);
		if (IS_ERR(sspa->tx_base))
			return PTR_ERR(sspa->tx_base);

		sspa->clk = devm_clk_get(&pdev->dev, "bitclk");
		if (IS_ERR(sspa->clk))
			return PTR_ERR(sspa->clk);

		sspa->audio_clk = devm_clk_get(&pdev->dev, "audio");
		if (IS_ERR(sspa->audio_clk))
			return PTR_ERR(sspa->audio_clk);
	} else {
		struct resource *res;

		res = platform_get_resource(pdev, IORESOURCE_IO, 0);
		if (res == NULL)
			return -ENODEV;

		sspa->rx_base = devm_ioremap(&pdev->dev, res->start, 0x30);
		if (!sspa->rx_base)
			return -ENOMEM;

		sspa->tx_base = devm_ioremap(&pdev->dev,
					     res->start + 0x80, 0x30);
		if (!sspa->tx_base)
			return -ENOMEM;

		sspa->clk = devm_clk_get(&pdev->dev, NULL);
		if (IS_ERR(sspa->clk))
			return PTR_ERR(sspa->clk);

		sspa->audio_clk = clk_get(NULL, "mmp-audio");
		if (IS_ERR(sspa->audio_clk))
			return PTR_ERR(sspa->audio_clk);

		sspa->sysclk = clk_get(NULL, "mmp-sysclk");
		if (IS_ERR(sspa->sysclk)) {
			clk_put(sspa->audio_clk);
			return PTR_ERR(sspa->sysclk);
		}
	}
	platform_set_drvdata(pdev, sspa);

	sspa->playback_dma_data.maxburst = 4;
	sspa->capture_dma_data.maxburst = 4;
	/* You know, these addresses are actually ignored. */
	sspa->capture_dma_data.addr = SSPA_D;
	sspa->playback_dma_data.addr = 0x80 + SSPA_D;

	if (pdev->dev.of_node) {
		ret = devm_snd_dmaengine_pcm_register(&pdev->dev,
						      &mmp_pcm_config, 0);
		if (ret)
			return ret;
	}

	ret = devm_snd_soc_register_component(&pdev->dev, &mmp_sspa_component,
					      &mmp_sspa_dai, 1);
	if (ret)
		return ret;

	pm_runtime_enable(&pdev->dev);
	clk_prepare_enable(sspa->audio_clk);

	return 0;
>>>>>>> upstream/android-13
}

static int asoc_mmp_sspa_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct sspa_priv *priv = platform_get_drvdata(pdev);

	clk_disable(priv->audio_clk);
	clk_put(priv->audio_clk);
	clk_put(priv->sysclk);
	return 0;
}

static struct platform_driver asoc_mmp_sspa_driver = {
	.driver = {
		.name = "mmp-sspa-dai",
=======
	struct sspa_priv *sspa = platform_get_drvdata(pdev);

	clk_disable_unprepare(sspa->audio_clk);
	pm_runtime_disable(&pdev->dev);

	if (pdev->dev.of_node)
		return 0;

	clk_put(sspa->audio_clk);
	clk_put(sspa->sysclk);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id mmp_sspa_of_match[] = {
	{ .compatible = "marvell,mmp-sspa" },
	{},
};

MODULE_DEVICE_TABLE(of, mmp_sspa_of_match);
#endif

static struct platform_driver asoc_mmp_sspa_driver = {
	.driver = {
		.name = "mmp-sspa-dai",
		.of_match_table = of_match_ptr(mmp_sspa_of_match),
>>>>>>> upstream/android-13
	},
	.probe = asoc_mmp_sspa_probe,
	.remove = asoc_mmp_sspa_remove,
};

module_platform_driver(asoc_mmp_sspa_driver);

MODULE_AUTHOR("Leo Yan <leoy@marvell.com>");
MODULE_DESCRIPTION("MMP SSPA SoC Interface");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:mmp-sspa-dai");
