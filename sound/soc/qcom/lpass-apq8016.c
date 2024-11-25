<<<<<<< HEAD
/*
 * Copyright (c) 2010-2011,2013-2015 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * lpass-apq8016.c -- ALSA SoC CPU DAI driver for APQ8016 LPASS
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2010-2011,2013-2015 The Linux Foundation. All rights reserved.
 *
 * lpass-apq8016.c -- ALSA SoC CPU DAI driver for APQ8016 LPASS
>>>>>>> upstream/android-13
 */


#include <linux/clk.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dai.h>

#include <dt-bindings/sound/apq8016-lpass.h>
#include "lpass-lpaif-reg.h"
#include "lpass.h"

static struct snd_soc_dai_driver apq8016_lpass_cpu_dai_driver[] = {
	[MI2S_PRIMARY] =  {
		.id = MI2S_PRIMARY,
		.name = "Primary MI2S",
		.playback = {
			.stream_name	= "Primary Playback",
			.formats	= SNDRV_PCM_FMTBIT_S16 |
						SNDRV_PCM_FMTBIT_S24 |
						SNDRV_PCM_FMTBIT_S32,
			.rates		= SNDRV_PCM_RATE_8000 |
						SNDRV_PCM_RATE_16000 |
						SNDRV_PCM_RATE_32000 |
						SNDRV_PCM_RATE_48000 |
						SNDRV_PCM_RATE_96000,
			.rate_min	= 8000,
			.rate_max	= 96000,
			.channels_min	= 1,
			.channels_max	= 8,
		},
		.probe	= &asoc_qcom_lpass_cpu_dai_probe,
		.ops    = &asoc_qcom_lpass_cpu_dai_ops,
	},
	[MI2S_SECONDARY] =  {
		.id = MI2S_SECONDARY,
		.name = "Secondary MI2S",
		.playback = {
			.stream_name	= "Secondary Playback",
			.formats	= SNDRV_PCM_FMTBIT_S16 |
						SNDRV_PCM_FMTBIT_S24 |
						SNDRV_PCM_FMTBIT_S32,
			.rates		= SNDRV_PCM_RATE_8000 |
						SNDRV_PCM_RATE_16000 |
						SNDRV_PCM_RATE_32000 |
						SNDRV_PCM_RATE_48000 |
						SNDRV_PCM_RATE_96000,
			.rate_min	= 8000,
			.rate_max	= 96000,
			.channels_min	= 1,
			.channels_max	= 8,
		},
		.probe	= &asoc_qcom_lpass_cpu_dai_probe,
		.ops    = &asoc_qcom_lpass_cpu_dai_ops,
	},
	[MI2S_TERTIARY] =  {
		.id = MI2S_TERTIARY,
		.name = "Tertiary MI2S",
		.capture = {
			.stream_name	= "Tertiary Capture",
			.formats	= SNDRV_PCM_FMTBIT_S16 |
						SNDRV_PCM_FMTBIT_S24 |
						SNDRV_PCM_FMTBIT_S32,
			.rates		= SNDRV_PCM_RATE_8000 |
						SNDRV_PCM_RATE_16000 |
						SNDRV_PCM_RATE_32000 |
						SNDRV_PCM_RATE_48000 |
						SNDRV_PCM_RATE_96000,
			.rate_min	= 8000,
			.rate_max	= 96000,
			.channels_min	= 1,
			.channels_max	= 8,
		},
		.probe	= &asoc_qcom_lpass_cpu_dai_probe,
		.ops    = &asoc_qcom_lpass_cpu_dai_ops,
	},
	[MI2S_QUATERNARY] =  {
		.id = MI2S_QUATERNARY,
		.name = "Quatenary MI2S",
		.playback = {
			.stream_name	= "Quatenary Playback",
			.formats	= SNDRV_PCM_FMTBIT_S16 |
						SNDRV_PCM_FMTBIT_S24 |
						SNDRV_PCM_FMTBIT_S32,
			.rates		= SNDRV_PCM_RATE_8000 |
						SNDRV_PCM_RATE_16000 |
						SNDRV_PCM_RATE_32000 |
						SNDRV_PCM_RATE_48000 |
						SNDRV_PCM_RATE_96000,
			.rate_min	= 8000,
			.rate_max	= 96000,
			.channels_min	= 1,
			.channels_max	= 8,
		},
		.capture = {
			.stream_name	= "Quatenary Capture",
			.formats	= SNDRV_PCM_FMTBIT_S16 |
						SNDRV_PCM_FMTBIT_S24 |
						SNDRV_PCM_FMTBIT_S32,
			.rates		= SNDRV_PCM_RATE_8000 |
						SNDRV_PCM_RATE_16000 |
						SNDRV_PCM_RATE_32000 |
						SNDRV_PCM_RATE_48000 |
						SNDRV_PCM_RATE_96000,
			.rate_min	= 8000,
			.rate_max	= 96000,
			.channels_min	= 1,
			.channels_max	= 8,
		},
		.probe	= &asoc_qcom_lpass_cpu_dai_probe,
		.ops    = &asoc_qcom_lpass_cpu_dai_ops,
	},
};

static int apq8016_lpass_alloc_dma_channel(struct lpass_data *drvdata,
<<<<<<< HEAD
					   int direction)
=======
					   int direction, unsigned int dai_id)
>>>>>>> upstream/android-13
{
	struct lpass_variant *v = drvdata->variant;
	int chan = 0;

	if (direction == SNDRV_PCM_STREAM_PLAYBACK) {
		chan = find_first_zero_bit(&drvdata->dma_ch_bit_map,
					v->rdma_channels);

		if (chan >= v->rdma_channels)
			return -EBUSY;
	} else {
		chan = find_next_zero_bit(&drvdata->dma_ch_bit_map,
					v->wrdma_channel_start +
					v->wrdma_channels,
					v->wrdma_channel_start);

		if (chan >=  v->wrdma_channel_start + v->wrdma_channels)
			return -EBUSY;
	}

	set_bit(chan, &drvdata->dma_ch_bit_map);

	return chan;
}

<<<<<<< HEAD
static int apq8016_lpass_free_dma_channel(struct lpass_data *drvdata, int chan)
=======
static int apq8016_lpass_free_dma_channel(struct lpass_data *drvdata, int chan, unsigned int dai_id)
>>>>>>> upstream/android-13
{
	clear_bit(chan, &drvdata->dma_ch_bit_map);

	return 0;
}

static int apq8016_lpass_init(struct platform_device *pdev)
{
	struct lpass_data *drvdata = platform_get_drvdata(pdev);
<<<<<<< HEAD
	struct device *dev = &pdev->dev;
	int ret;

	drvdata->pcnoc_mport_clk = devm_clk_get(dev, "pcnoc-mport-clk");
	if (IS_ERR(drvdata->pcnoc_mport_clk)) {
		dev_err(&pdev->dev, "error getting pcnoc-mport-clk: %ld\n",
			PTR_ERR(drvdata->pcnoc_mport_clk));
		return PTR_ERR(drvdata->pcnoc_mport_clk);
	}

	ret = clk_prepare_enable(drvdata->pcnoc_mport_clk);
	if (ret) {
		dev_err(&pdev->dev, "Error enabling pcnoc-mport-clk: %d\n",
			ret);
		return ret;
	}

	drvdata->pcnoc_sway_clk = devm_clk_get(dev, "pcnoc-sway-clk");
	if (IS_ERR(drvdata->pcnoc_sway_clk)) {
		dev_err(&pdev->dev, "error getting pcnoc-sway-clk: %ld\n",
			PTR_ERR(drvdata->pcnoc_sway_clk));
		return PTR_ERR(drvdata->pcnoc_sway_clk);
	}

	ret = clk_prepare_enable(drvdata->pcnoc_sway_clk);
	if (ret) {
		dev_err(&pdev->dev, "Error enabling pcnoc_sway_clk: %d\n", ret);
		return ret;
	}

	return 0;
=======
	struct lpass_variant *variant = drvdata->variant;
	struct device *dev = &pdev->dev;
	int ret, i;


	drvdata->clks = devm_kcalloc(dev, variant->num_clks,
				     sizeof(*drvdata->clks), GFP_KERNEL);
	if (!drvdata->clks)
		return -ENOMEM;
	drvdata->num_clks = variant->num_clks;

	for (i = 0; i < drvdata->num_clks; i++)
		drvdata->clks[i].id = variant->clk_name[i];

	ret = devm_clk_bulk_get(dev, drvdata->num_clks, drvdata->clks);
	if (ret) {
		dev_err(dev, "Failed to get clocks %d\n", ret);
		return ret;
	}

	ret = clk_bulk_prepare_enable(drvdata->num_clks, drvdata->clks);
	if (ret) {
		dev_err(dev, "apq8016 clk_enable failed\n");
		return ret;
	}

	drvdata->ahbix_clk = devm_clk_get(dev, "ahbix-clk");
	if (IS_ERR(drvdata->ahbix_clk)) {
		dev_err(dev, "error getting ahbix-clk: %ld\n",
				PTR_ERR(drvdata->ahbix_clk));
		ret = PTR_ERR(drvdata->ahbix_clk);
		goto err_ahbix_clk;
	}

	ret = clk_set_rate(drvdata->ahbix_clk, LPASS_AHBIX_CLOCK_FREQUENCY);
	if (ret) {
		dev_err(dev, "error setting rate on ahbix_clk: %d\n", ret);
		goto err_ahbix_clk;
	}
	dev_dbg(dev, "set ahbix_clk rate to %lu\n",
			clk_get_rate(drvdata->ahbix_clk));

	ret = clk_prepare_enable(drvdata->ahbix_clk);
	if (ret) {
		dev_err(dev, "error enabling ahbix_clk: %d\n", ret);
		goto err_ahbix_clk;
	}

	return 0;

err_ahbix_clk:
	clk_bulk_disable_unprepare(drvdata->num_clks, drvdata->clks);
	return ret;
>>>>>>> upstream/android-13
}

static int apq8016_lpass_exit(struct platform_device *pdev)
{
	struct lpass_data *drvdata = platform_get_drvdata(pdev);

<<<<<<< HEAD
	clk_disable_unprepare(drvdata->pcnoc_mport_clk);
	clk_disable_unprepare(drvdata->pcnoc_sway_clk);
=======
	clk_bulk_disable_unprepare(drvdata->num_clks, drvdata->clks);
	clk_disable_unprepare(drvdata->ahbix_clk);
>>>>>>> upstream/android-13

	return 0;
}


static struct lpass_variant apq8016_data = {
	.i2sctrl_reg_base	= 0x1000,
	.i2sctrl_reg_stride	= 0x1000,
	.i2s_ports		= 4,
	.irq_reg_base		= 0x6000,
	.irq_reg_stride		= 0x1000,
	.irq_ports		= 3,
	.rdma_reg_base		= 0x8400,
	.rdma_reg_stride	= 0x1000,
	.rdma_channels		= 2,
	.dmactl_audif_start	= 1,
	.wrdma_reg_base		= 0xB000,
	.wrdma_reg_stride	= 0x1000,
	.wrdma_channel_start	= 5,
	.wrdma_channels		= 2,
<<<<<<< HEAD
=======
	.loopback		= REG_FIELD_ID(0x1000, 15, 15, 4, 0x1000),
	.spken			= REG_FIELD_ID(0x1000, 14, 14, 4, 0x1000),
	.spkmode		= REG_FIELD_ID(0x1000, 10, 13, 4, 0x1000),
	.spkmono		= REG_FIELD_ID(0x1000, 9, 9, 4, 0x1000),
	.micen			= REG_FIELD_ID(0x1000, 8, 8, 4, 0x1000),
	.micmode		= REG_FIELD_ID(0x1000, 4, 7, 4, 0x1000),
	.micmono		= REG_FIELD_ID(0x1000, 3, 3, 4, 0x1000),
	.wssrc			= REG_FIELD_ID(0x1000, 2, 2, 4, 0x1000),
	.bitwidth		= REG_FIELD_ID(0x1000, 0, 1, 4, 0x1000),

	.rdma_dyncclk		= REG_FIELD_ID(0x8400, 12, 12, 2, 0x1000),
	.rdma_bursten		= REG_FIELD_ID(0x8400, 11, 11, 2, 0x1000),
	.rdma_wpscnt		= REG_FIELD_ID(0x8400, 8, 10, 2, 0x1000),
	.rdma_intf		= REG_FIELD_ID(0x8400, 4, 7, 2, 0x1000),
	.rdma_fifowm		= REG_FIELD_ID(0x8400, 1, 3, 2, 0x1000),
	.rdma_enable		= REG_FIELD_ID(0x8400, 0, 0, 2, 0x1000),

	.wrdma_dyncclk		= REG_FIELD_ID(0xB000, 12, 12, 2, 0x1000),
	.wrdma_bursten		= REG_FIELD_ID(0xB000, 11, 11, 2, 0x1000),
	.wrdma_wpscnt		= REG_FIELD_ID(0xB000, 8, 10, 2, 0x1000),
	.wrdma_intf		= REG_FIELD_ID(0xB000, 4, 7, 2, 0x1000),
	.wrdma_fifowm		= REG_FIELD_ID(0xB000, 1, 3, 2, 0x1000),
	.wrdma_enable		= REG_FIELD_ID(0xB000, 0, 0, 2, 0x1000),

	.clk_name		= (const char*[]) {
				   "pcnoc-mport-clk",
				   "pcnoc-sway-clk",
				  },
	.num_clks		= 2,
>>>>>>> upstream/android-13
	.dai_driver		= apq8016_lpass_cpu_dai_driver,
	.num_dai		= ARRAY_SIZE(apq8016_lpass_cpu_dai_driver),
	.dai_osr_clk_names	= (const char *[]) {
				"mi2s-osr-clk0",
				"mi2s-osr-clk1",
				"mi2s-osr-clk2",
				"mi2s-osr-clk3",
				},
	.dai_bit_clk_names	= (const char *[]) {
				"mi2s-bit-clk0",
				"mi2s-bit-clk1",
				"mi2s-bit-clk2",
				"mi2s-bit-clk3",
				},
	.init			= apq8016_lpass_init,
	.exit			= apq8016_lpass_exit,
	.alloc_dma_channel	= apq8016_lpass_alloc_dma_channel,
	.free_dma_channel	= apq8016_lpass_free_dma_channel,
};

<<<<<<< HEAD
static const struct of_device_id apq8016_lpass_cpu_device_id[] = {
=======
static const struct of_device_id apq8016_lpass_cpu_device_id[] __maybe_unused = {
>>>>>>> upstream/android-13
	{ .compatible = "qcom,lpass-cpu-apq8016", .data = &apq8016_data },
	{}
};
MODULE_DEVICE_TABLE(of, apq8016_lpass_cpu_device_id);

static struct platform_driver apq8016_lpass_cpu_platform_driver = {
	.driver	= {
		.name		= "apq8016-lpass-cpu",
		.of_match_table	= of_match_ptr(apq8016_lpass_cpu_device_id),
	},
	.probe	= asoc_qcom_lpass_cpu_platform_probe,
	.remove	= asoc_qcom_lpass_cpu_platform_remove,
};
module_platform_driver(apq8016_lpass_cpu_platform_driver);

MODULE_DESCRIPTION("APQ8016 LPASS CPU Driver");
MODULE_LICENSE("GPL v2");

