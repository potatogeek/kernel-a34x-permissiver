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
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2010-2011,2013-2015 The Linux Foundation. All rights reserved.
 *
>>>>>>> upstream/android-13
 * lpass-platform.c -- ALSA SoC platform driver for QTi LPASS
 */

#include <linux/dma-mapping.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <sound/pcm_params.h>
#include <linux/regmap.h>
#include <sound/soc.h>
#include "lpass-lpaif-reg.h"
#include "lpass.h"

#define DRV_NAME "lpass-platform"

struct lpass_pcm_data {
	int dma_ch;
	int i2s_port;
};

<<<<<<< HEAD
#define LPASS_PLATFORM_BUFFER_SIZE	(16 * 1024)
=======
#define LPASS_PLATFORM_BUFFER_SIZE	(24 *  2 * 1024)
>>>>>>> upstream/android-13
#define LPASS_PLATFORM_PERIODS		2

static const struct snd_pcm_hardware lpass_platform_pcm_hardware = {
	.info			=	SNDRV_PCM_INFO_MMAP |
					SNDRV_PCM_INFO_MMAP_VALID |
					SNDRV_PCM_INFO_INTERLEAVED |
					SNDRV_PCM_INFO_PAUSE |
					SNDRV_PCM_INFO_RESUME,
	.formats		=	SNDRV_PCM_FMTBIT_S16 |
					SNDRV_PCM_FMTBIT_S24 |
					SNDRV_PCM_FMTBIT_S32,
	.rates			=	SNDRV_PCM_RATE_8000_192000,
	.rate_min		=	8000,
	.rate_max		=	192000,
	.channels_min		=	1,
	.channels_max		=	8,
	.buffer_bytes_max	=	LPASS_PLATFORM_BUFFER_SIZE,
	.period_bytes_max	=	LPASS_PLATFORM_BUFFER_SIZE /
						LPASS_PLATFORM_PERIODS,
	.period_bytes_min	=	LPASS_PLATFORM_BUFFER_SIZE /
						LPASS_PLATFORM_PERIODS,
	.periods_min		=	LPASS_PLATFORM_PERIODS,
	.periods_max		=	LPASS_PLATFORM_PERIODS,
	.fifo_size		=	0,
};

<<<<<<< HEAD
static int lpass_platform_pcmops_open(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *soc_runtime = substream->private_data;
	struct snd_soc_dai *cpu_dai = soc_runtime->cpu_dai;
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(soc_runtime, DRV_NAME);
=======
static int lpass_platform_alloc_dmactl_fields(struct device *dev,
					 struct regmap *map)
{
	struct lpass_data *drvdata = dev_get_drvdata(dev);
	struct lpass_variant *v = drvdata->variant;
	struct lpaif_dmactl *rd_dmactl, *wr_dmactl;
	int rval;

	drvdata->rd_dmactl = devm_kzalloc(dev, sizeof(struct lpaif_dmactl),
					  GFP_KERNEL);
	if (drvdata->rd_dmactl == NULL)
		return -ENOMEM;

	drvdata->wr_dmactl = devm_kzalloc(dev, sizeof(struct lpaif_dmactl),
					  GFP_KERNEL);
	if (drvdata->wr_dmactl == NULL)
		return -ENOMEM;

	rd_dmactl = drvdata->rd_dmactl;
	wr_dmactl = drvdata->wr_dmactl;

	rval = devm_regmap_field_bulk_alloc(dev, map, &rd_dmactl->intf,
					    &v->rdma_intf, 6);
	if (rval)
		return rval;

	return devm_regmap_field_bulk_alloc(dev, map, &wr_dmactl->intf,
					    &v->wrdma_intf, 6);
}

static int lpass_platform_alloc_hdmidmactl_fields(struct device *dev,
					 struct regmap *map)
{
	struct lpass_data *drvdata = dev_get_drvdata(dev);
	struct lpass_variant *v = drvdata->variant;
	struct lpaif_dmactl *rd_dmactl;

	rd_dmactl = devm_kzalloc(dev, sizeof(struct lpaif_dmactl), GFP_KERNEL);
	if (rd_dmactl == NULL)
		return -ENOMEM;

	drvdata->hdmi_rd_dmactl = rd_dmactl;

	return devm_regmap_field_bulk_alloc(dev, map, &rd_dmactl->bursten,
					    &v->hdmi_rdma_bursten, 8);
}

static int lpass_platform_pcmops_open(struct snd_soc_component *component,
				      struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *soc_runtime = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(soc_runtime, 0);
>>>>>>> upstream/android-13
	struct lpass_data *drvdata = snd_soc_component_get_drvdata(component);
	struct lpass_variant *v = drvdata->variant;
	int ret, dma_ch, dir = substream->stream;
	struct lpass_pcm_data *data;
<<<<<<< HEAD

=======
	struct regmap *map;
	unsigned int dai_id = cpu_dai->driver->id;

	component->id = dai_id;
>>>>>>> upstream/android-13
	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->i2s_port = cpu_dai->driver->id;
	runtime->private_data = data;

	if (v->alloc_dma_channel)
<<<<<<< HEAD
		dma_ch = v->alloc_dma_channel(drvdata, dir);
=======
		dma_ch = v->alloc_dma_channel(drvdata, dir, dai_id);
>>>>>>> upstream/android-13
	else
		dma_ch = 0;

	if (dma_ch < 0) {
		kfree(data);
		return dma_ch;
	}

<<<<<<< HEAD
	drvdata->substream[dma_ch] = substream;

	ret = regmap_write(drvdata->lpaif_map,
			LPAIF_DMACTL_REG(v, dma_ch, dir), 0);
	if (ret) {
		dev_err(soc_runtime->dev,
			"error writing to rdmactl reg: %d\n", ret);
			return ret;
	}

	data->dma_ch = dma_ch;

=======
	if (cpu_dai->driver->id == LPASS_DP_RX) {
		map = drvdata->hdmiif_map;
		drvdata->hdmi_substream[dma_ch] = substream;
	} else {
		map = drvdata->lpaif_map;
		drvdata->substream[dma_ch] = substream;
	}
	data->dma_ch = dma_ch;
	ret = regmap_write(map,
			LPAIF_DMACTL_REG(v, dma_ch, dir, data->i2s_port), 0);
	if (ret) {
		dev_err(soc_runtime->dev,
			"error writing to rdmactl reg: %d\n", ret);
		return ret;
	}
>>>>>>> upstream/android-13
	snd_soc_set_runtime_hwparams(substream, &lpass_platform_pcm_hardware);

	runtime->dma_bytes = lpass_platform_pcm_hardware.buffer_bytes_max;

	ret = snd_pcm_hw_constraint_integer(runtime,
			SNDRV_PCM_HW_PARAM_PERIODS);
	if (ret < 0) {
		kfree(data);
		dev_err(soc_runtime->dev, "setting constraints failed: %d\n",
			ret);
		return -EINVAL;
	}

<<<<<<< HEAD
	snd_pcm_set_runtime_buffer(substream, &substream->dma_buffer);

	return 0;
}

static int lpass_platform_pcmops_close(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *soc_runtime = substream->private_data;
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(soc_runtime, DRV_NAME);
	struct lpass_data *drvdata = snd_soc_component_get_drvdata(component);
	struct lpass_variant *v = drvdata->variant;
	struct lpass_pcm_data *data;

	data = runtime->private_data;
	drvdata->substream[data->dma_ch] = NULL;
	if (v->free_dma_channel)
		v->free_dma_channel(drvdata, data->dma_ch);
=======
	return 0;
}

static int lpass_platform_pcmops_close(struct snd_soc_component *component,
				       struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *soc_runtime = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(soc_runtime, 0);
	struct lpass_data *drvdata = snd_soc_component_get_drvdata(component);
	struct lpass_variant *v = drvdata->variant;
	struct lpass_pcm_data *data;
	unsigned int dai_id = cpu_dai->driver->id;

	data = runtime->private_data;
	if (dai_id == LPASS_DP_RX)
		drvdata->hdmi_substream[data->dma_ch] = NULL;
	else
		drvdata->substream[data->dma_ch] = NULL;
	if (v->free_dma_channel)
		v->free_dma_channel(drvdata, data->dma_ch, dai_id);
>>>>>>> upstream/android-13

	kfree(data);
	return 0;
}

<<<<<<< HEAD
static int lpass_platform_pcmops_hw_params(struct snd_pcm_substream *substream,
		struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *soc_runtime = substream->private_data;
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(soc_runtime, DRV_NAME);
=======
static int lpass_platform_pcmops_hw_params(struct snd_soc_component *component,
					   struct snd_pcm_substream *substream,
					   struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *soc_runtime = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(soc_runtime, 0);
>>>>>>> upstream/android-13
	struct lpass_data *drvdata = snd_soc_component_get_drvdata(component);
	struct snd_pcm_runtime *rt = substream->runtime;
	struct lpass_pcm_data *pcm_data = rt->private_data;
	struct lpass_variant *v = drvdata->variant;
	snd_pcm_format_t format = params_format(params);
	unsigned int channels = params_channels(params);
	unsigned int regval;
<<<<<<< HEAD
	int ch, dir = substream->stream;
	int bitwidth;
	int ret, dma_port = pcm_data->i2s_port + v->dmactl_audif_start;

	ch = pcm_data->dma_ch;
=======
	struct lpaif_dmactl *dmactl;
	int id, dir = substream->stream;
	int bitwidth;
	int ret, dma_port = pcm_data->i2s_port + v->dmactl_audif_start;
	unsigned int dai_id = cpu_dai->driver->id;

	if (dir ==  SNDRV_PCM_STREAM_PLAYBACK) {
		id = pcm_data->dma_ch;
		if (dai_id == LPASS_DP_RX)
			dmactl = drvdata->hdmi_rd_dmactl;
		else
			dmactl = drvdata->rd_dmactl;

	} else {
		dmactl = drvdata->wr_dmactl;
		id = pcm_data->dma_ch - v->wrdma_channel_start;
	}
>>>>>>> upstream/android-13

	bitwidth = snd_pcm_format_width(format);
	if (bitwidth < 0) {
		dev_err(soc_runtime->dev, "invalid bit width given: %d\n",
				bitwidth);
		return bitwidth;
	}

<<<<<<< HEAD
	regval = LPAIF_DMACTL_BURSTEN_INCR4 |
			LPAIF_DMACTL_AUDINTF(dma_port) |
			LPAIF_DMACTL_FIFOWM_8;

=======
	ret = regmap_fields_write(dmactl->bursten, id, LPAIF_DMACTL_BURSTEN_INCR4);
	if (ret) {
		dev_err(soc_runtime->dev, "error updating bursten field: %d\n", ret);
		return ret;
	}

	ret = regmap_fields_write(dmactl->fifowm, id, LPAIF_DMACTL_FIFOWM_8);
	if (ret) {
		dev_err(soc_runtime->dev, "error updating fifowm field: %d\n", ret);
		return ret;
	}

	switch (dai_id) {
	case LPASS_DP_RX:
		ret = regmap_fields_write(dmactl->burst8, id,
							LPAIF_DMACTL_BURSTEN_INCR4);
		if (ret) {
			dev_err(soc_runtime->dev, "error updating burst8en field: %d\n", ret);
			return ret;
		}
		ret = regmap_fields_write(dmactl->burst16, id,
							LPAIF_DMACTL_BURSTEN_INCR4);
		if (ret) {
			dev_err(soc_runtime->dev, "error updating burst16en field: %d\n", ret);
			return ret;
		}
		ret = regmap_fields_write(dmactl->dynburst, id,
							LPAIF_DMACTL_BURSTEN_INCR4);
		if (ret) {
			dev_err(soc_runtime->dev, "error updating dynbursten field: %d\n", ret);
			return ret;
		}
		break;
	case MI2S_PRIMARY:
	case MI2S_SECONDARY:
	case MI2S_TERTIARY:
	case MI2S_QUATERNARY:
	case MI2S_QUINARY:
		ret = regmap_fields_write(dmactl->intf, id,
						LPAIF_DMACTL_AUDINTF(dma_port));
		if (ret) {
			dev_err(soc_runtime->dev, "error updating audio interface field: %d\n",
					ret);
			return ret;
		}

		break;
	default:
		dev_err(soc_runtime->dev, "%s: invalid  interface: %d\n", __func__, dai_id);
		break;
	}
>>>>>>> upstream/android-13
	switch (bitwidth) {
	case 16:
		switch (channels) {
		case 1:
		case 2:
<<<<<<< HEAD
			regval |= LPAIF_DMACTL_WPSCNT_ONE;
			break;
		case 4:
			regval |= LPAIF_DMACTL_WPSCNT_TWO;
			break;
		case 6:
			regval |= LPAIF_DMACTL_WPSCNT_THREE;
			break;
		case 8:
			regval |= LPAIF_DMACTL_WPSCNT_FOUR;
			break;
		default:
			dev_err(soc_runtime->dev,
				"invalid PCM config given: bw=%d, ch=%u\n",
=======
			regval = LPAIF_DMACTL_WPSCNT_ONE;
			break;
		case 4:
			regval = LPAIF_DMACTL_WPSCNT_TWO;
			break;
		case 6:
			regval = LPAIF_DMACTL_WPSCNT_THREE;
			break;
		case 8:
			regval = LPAIF_DMACTL_WPSCNT_FOUR;
			break;
		default:
			dev_err(soc_runtime->dev, "invalid PCM config given: bw=%d, ch=%u\n",
>>>>>>> upstream/android-13
				bitwidth, channels);
			return -EINVAL;
		}
		break;
	case 24:
	case 32:
		switch (channels) {
		case 1:
<<<<<<< HEAD
			regval |= LPAIF_DMACTL_WPSCNT_ONE;
			break;
		case 2:
			regval |= LPAIF_DMACTL_WPSCNT_TWO;
			break;
		case 4:
			regval |= LPAIF_DMACTL_WPSCNT_FOUR;
			break;
		case 6:
			regval |= LPAIF_DMACTL_WPSCNT_SIX;
			break;
		case 8:
			regval |= LPAIF_DMACTL_WPSCNT_EIGHT;
			break;
		default:
			dev_err(soc_runtime->dev,
				"invalid PCM config given: bw=%d, ch=%u\n",
=======
			regval = LPAIF_DMACTL_WPSCNT_ONE;
			break;
		case 2:
			regval = (dai_id == LPASS_DP_RX ?
			LPAIF_DMACTL_WPSCNT_ONE :
			LPAIF_DMACTL_WPSCNT_TWO);
			break;
		case 4:
			regval = (dai_id == LPASS_DP_RX ?
			LPAIF_DMACTL_WPSCNT_TWO :
			LPAIF_DMACTL_WPSCNT_FOUR);
			break;
		case 6:
			regval = (dai_id == LPASS_DP_RX ?
			LPAIF_DMACTL_WPSCNT_THREE :
			LPAIF_DMACTL_WPSCNT_SIX);
			break;
		case 8:
			regval = (dai_id == LPASS_DP_RX ?
			LPAIF_DMACTL_WPSCNT_FOUR :
			LPAIF_DMACTL_WPSCNT_EIGHT);
			break;
		default:
			dev_err(soc_runtime->dev, "invalid PCM config given: bw=%d, ch=%u\n",
>>>>>>> upstream/android-13
				bitwidth, channels);
			return -EINVAL;
		}
		break;
	default:
		dev_err(soc_runtime->dev, "invalid PCM config given: bw=%d, ch=%u\n",
			bitwidth, channels);
		return -EINVAL;
	}

<<<<<<< HEAD
	ret = regmap_write(drvdata->lpaif_map,
			LPAIF_DMACTL_REG(v, ch, dir), regval);
	if (ret) {
		dev_err(soc_runtime->dev, "error writing to rdmactl reg: %d\n",
=======
	ret = regmap_fields_write(dmactl->wpscnt, id, regval);
	if (ret) {
		dev_err(soc_runtime->dev, "error writing to dmactl reg: %d\n",
>>>>>>> upstream/android-13
			ret);
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
static int lpass_platform_pcmops_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *soc_runtime = substream->private_data;
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(soc_runtime, DRV_NAME);
=======
static int lpass_platform_pcmops_hw_free(struct snd_soc_component *component,
					 struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *soc_runtime = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(soc_runtime, 0);
>>>>>>> upstream/android-13
	struct lpass_data *drvdata = snd_soc_component_get_drvdata(component);
	struct snd_pcm_runtime *rt = substream->runtime;
	struct lpass_pcm_data *pcm_data = rt->private_data;
	struct lpass_variant *v = drvdata->variant;
	unsigned int reg;
	int ret;
<<<<<<< HEAD

	reg = LPAIF_DMACTL_REG(v, pcm_data->dma_ch, substream->stream);
	ret = regmap_write(drvdata->lpaif_map, reg, 0);
=======
	struct regmap *map;
	unsigned int dai_id = cpu_dai->driver->id;

	if (dai_id == LPASS_DP_RX)
		map = drvdata->hdmiif_map;
	else
		map = drvdata->lpaif_map;

	reg = LPAIF_DMACTL_REG(v, pcm_data->dma_ch, substream->stream, dai_id);
	ret = regmap_write(map, reg, 0);
>>>>>>> upstream/android-13
	if (ret)
		dev_err(soc_runtime->dev, "error writing to rdmactl reg: %d\n",
			ret);

	return ret;
}

<<<<<<< HEAD
static int lpass_platform_pcmops_prepare(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *soc_runtime = substream->private_data;
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(soc_runtime, DRV_NAME);
=======
static int lpass_platform_pcmops_prepare(struct snd_soc_component *component,
					 struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *soc_runtime = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(soc_runtime, 0);
>>>>>>> upstream/android-13
	struct lpass_data *drvdata = snd_soc_component_get_drvdata(component);
	struct snd_pcm_runtime *rt = substream->runtime;
	struct lpass_pcm_data *pcm_data = rt->private_data;
	struct lpass_variant *v = drvdata->variant;
<<<<<<< HEAD
	int ret, ch, dir = substream->stream;

	ch = pcm_data->dma_ch;

	ret = regmap_write(drvdata->lpaif_map,
			LPAIF_DMABASE_REG(v, ch, dir),
			runtime->dma_addr);
=======
	struct lpaif_dmactl *dmactl;
	struct regmap *map;
	int ret, id, ch, dir = substream->stream;
	unsigned int dai_id = cpu_dai->driver->id;


	ch = pcm_data->dma_ch;
	if (dir ==  SNDRV_PCM_STREAM_PLAYBACK) {
		if (dai_id == LPASS_DP_RX) {
			dmactl = drvdata->hdmi_rd_dmactl;
			map = drvdata->hdmiif_map;
		} else {
			dmactl = drvdata->rd_dmactl;
			map = drvdata->lpaif_map;
		}

		id = pcm_data->dma_ch;
	} else {
		dmactl = drvdata->wr_dmactl;
		id = pcm_data->dma_ch - v->wrdma_channel_start;
		map = drvdata->lpaif_map;
	}

	ret = regmap_write(map, LPAIF_DMABASE_REG(v, ch, dir, dai_id),
				runtime->dma_addr);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(soc_runtime->dev, "error writing to rdmabase reg: %d\n",
			ret);
		return ret;
	}

<<<<<<< HEAD
	ret = regmap_write(drvdata->lpaif_map,
			LPAIF_DMABUFF_REG(v, ch, dir),
=======
	ret = regmap_write(map, LPAIF_DMABUFF_REG(v, ch, dir, dai_id),
>>>>>>> upstream/android-13
			(snd_pcm_lib_buffer_bytes(substream) >> 2) - 1);
	if (ret) {
		dev_err(soc_runtime->dev, "error writing to rdmabuff reg: %d\n",
			ret);
		return ret;
	}

<<<<<<< HEAD
	ret = regmap_write(drvdata->lpaif_map,
			LPAIF_DMAPER_REG(v, ch, dir),
=======
	ret = regmap_write(map, LPAIF_DMAPER_REG(v, ch, dir, dai_id),
>>>>>>> upstream/android-13
			(snd_pcm_lib_period_bytes(substream) >> 2) - 1);
	if (ret) {
		dev_err(soc_runtime->dev, "error writing to rdmaper reg: %d\n",
			ret);
		return ret;
	}

<<<<<<< HEAD
	ret = regmap_update_bits(drvdata->lpaif_map,
			LPAIF_DMACTL_REG(v, ch, dir),
			LPAIF_DMACTL_ENABLE_MASK, LPAIF_DMACTL_ENABLE_ON);
=======
	ret = regmap_fields_write(dmactl->enable, id, LPAIF_DMACTL_ENABLE_ON);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(soc_runtime->dev, "error writing to rdmactl reg: %d\n",
			ret);
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
static int lpass_platform_pcmops_trigger(struct snd_pcm_substream *substream,
		int cmd)
{
	struct snd_soc_pcm_runtime *soc_runtime = substream->private_data;
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(soc_runtime, DRV_NAME);
=======
static int lpass_platform_pcmops_trigger(struct snd_soc_component *component,
					 struct snd_pcm_substream *substream,
					 int cmd)
{
	struct snd_soc_pcm_runtime *soc_runtime = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(soc_runtime, 0);
>>>>>>> upstream/android-13
	struct lpass_data *drvdata = snd_soc_component_get_drvdata(component);
	struct snd_pcm_runtime *rt = substream->runtime;
	struct lpass_pcm_data *pcm_data = rt->private_data;
	struct lpass_variant *v = drvdata->variant;
<<<<<<< HEAD
	int ret, ch, dir = substream->stream;

	ch = pcm_data->dma_ch;
=======
	struct lpaif_dmactl *dmactl;
	struct regmap *map;
	int ret, ch, id;
	int dir = substream->stream;
	unsigned int reg_irqclr = 0, val_irqclr = 0;
	unsigned int  reg_irqen = 0, val_irqen = 0, val_mask = 0;
	unsigned int dai_id = cpu_dai->driver->id;

	ch = pcm_data->dma_ch;
	if (dir ==  SNDRV_PCM_STREAM_PLAYBACK) {
		id = pcm_data->dma_ch;
		if (dai_id == LPASS_DP_RX) {
			dmactl = drvdata->hdmi_rd_dmactl;
			map = drvdata->hdmiif_map;
		} else {
			dmactl = drvdata->rd_dmactl;
			map = drvdata->lpaif_map;
		}
	} else {
		dmactl = drvdata->wr_dmactl;
		id = pcm_data->dma_ch - v->wrdma_channel_start;
		map = drvdata->lpaif_map;
	}
>>>>>>> upstream/android-13

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
<<<<<<< HEAD
		/* clear status before enabling interrupts */
		ret = regmap_write(drvdata->lpaif_map,
				LPAIF_IRQCLEAR_REG(v, LPAIF_IRQ_PORT_HOST),
				LPAIF_IRQ_ALL(ch));
		if (ret) {
			dev_err(soc_runtime->dev,
				"error writing to irqclear reg: %d\n", ret);
			return ret;
		}

		ret = regmap_update_bits(drvdata->lpaif_map,
				LPAIF_IRQEN_REG(v, LPAIF_IRQ_PORT_HOST),
				LPAIF_IRQ_ALL(ch),
				LPAIF_IRQ_ALL(ch));
		if (ret) {
			dev_err(soc_runtime->dev,
				"error writing to irqen reg: %d\n", ret);
			return ret;
		}

		ret = regmap_update_bits(drvdata->lpaif_map,
				LPAIF_DMACTL_REG(v, ch, dir),
				LPAIF_DMACTL_ENABLE_MASK,
				LPAIF_DMACTL_ENABLE_ON);
=======
		ret = regmap_fields_write(dmactl->enable, id,
						 LPAIF_DMACTL_ENABLE_ON);
>>>>>>> upstream/android-13
		if (ret) {
			dev_err(soc_runtime->dev,
				"error writing to rdmactl reg: %d\n", ret);
			return ret;
		}
<<<<<<< HEAD
=======
		switch (dai_id) {
		case LPASS_DP_RX:
			ret = regmap_fields_write(dmactl->dyncclk, id,
					 LPAIF_DMACTL_DYNCLK_ON);
			if (ret) {
				dev_err(soc_runtime->dev,
					"error writing to rdmactl reg: %d\n", ret);
				return ret;
			}
			reg_irqclr = LPASS_HDMITX_APP_IRQCLEAR_REG(v);
			val_irqclr = (LPAIF_IRQ_ALL(ch) |
					LPAIF_IRQ_HDMI_REQ_ON_PRELOAD(ch) |
					LPAIF_IRQ_HDMI_METADONE |
					LPAIF_IRQ_HDMI_SDEEP_AUD_DIS(ch));

			reg_irqen = LPASS_HDMITX_APP_IRQEN_REG(v);
			val_mask = (LPAIF_IRQ_ALL(ch) |
					LPAIF_IRQ_HDMI_REQ_ON_PRELOAD(ch) |
					LPAIF_IRQ_HDMI_METADONE |
					LPAIF_IRQ_HDMI_SDEEP_AUD_DIS(ch));
			val_irqen = (LPAIF_IRQ_ALL(ch) |
					LPAIF_IRQ_HDMI_REQ_ON_PRELOAD(ch) |
					LPAIF_IRQ_HDMI_METADONE |
					LPAIF_IRQ_HDMI_SDEEP_AUD_DIS(ch));
			break;
		case MI2S_PRIMARY:
		case MI2S_SECONDARY:
		case MI2S_TERTIARY:
		case MI2S_QUATERNARY:
		case MI2S_QUINARY:
			reg_irqclr = LPAIF_IRQCLEAR_REG(v, LPAIF_IRQ_PORT_HOST);
			val_irqclr = LPAIF_IRQ_ALL(ch);


			reg_irqen = LPAIF_IRQEN_REG(v, LPAIF_IRQ_PORT_HOST);
			val_mask = LPAIF_IRQ_ALL(ch);
			val_irqen = LPAIF_IRQ_ALL(ch);
			break;
		default:
			dev_err(soc_runtime->dev, "%s: invalid %d interface\n", __func__, dai_id);
			return -EINVAL;
		}

		ret = regmap_write_bits(map, reg_irqclr, val_irqclr, val_irqclr);
		if (ret) {
			dev_err(soc_runtime->dev, "error writing to irqclear reg: %d\n", ret);
			return ret;
		}
		ret = regmap_update_bits(map, reg_irqen, val_mask, val_irqen);
		if (ret) {
			dev_err(soc_runtime->dev, "error writing to irqen reg: %d\n", ret);
			return ret;
		}
>>>>>>> upstream/android-13
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
<<<<<<< HEAD
		ret = regmap_update_bits(drvdata->lpaif_map,
				LPAIF_DMACTL_REG(v, ch, dir),
				LPAIF_DMACTL_ENABLE_MASK,
				LPAIF_DMACTL_ENABLE_OFF);
=======
		ret = regmap_fields_write(dmactl->enable, id,
					 LPAIF_DMACTL_ENABLE_OFF);
>>>>>>> upstream/android-13
		if (ret) {
			dev_err(soc_runtime->dev,
				"error writing to rdmactl reg: %d\n", ret);
			return ret;
		}
<<<<<<< HEAD

		ret = regmap_update_bits(drvdata->lpaif_map,
				LPAIF_IRQEN_REG(v, LPAIF_IRQ_PORT_HOST),
				LPAIF_IRQ_ALL(ch), 0);
=======
		switch (dai_id) {
		case LPASS_DP_RX:
			ret = regmap_fields_write(dmactl->dyncclk, id,
					 LPAIF_DMACTL_DYNCLK_OFF);
			if (ret) {
				dev_err(soc_runtime->dev,
					"error writing to rdmactl reg: %d\n", ret);
				return ret;
			}
			reg_irqen = LPASS_HDMITX_APP_IRQEN_REG(v);
			val_mask = (LPAIF_IRQ_ALL(ch) |
					LPAIF_IRQ_HDMI_REQ_ON_PRELOAD(ch) |
					LPAIF_IRQ_HDMI_METADONE |
					LPAIF_IRQ_HDMI_SDEEP_AUD_DIS(ch));
			val_irqen = 0;
			break;
		case MI2S_PRIMARY:
		case MI2S_SECONDARY:
		case MI2S_TERTIARY:
		case MI2S_QUATERNARY:
		case MI2S_QUINARY:
			reg_irqen = LPAIF_IRQEN_REG(v, LPAIF_IRQ_PORT_HOST);
			val_mask = LPAIF_IRQ_ALL(ch);
			val_irqen = 0;
			break;
		default:
			dev_err(soc_runtime->dev, "%s: invalid %d interface\n", __func__, dai_id);
			return -EINVAL;
		}

		ret = regmap_update_bits(map, reg_irqen, val_mask, val_irqen);
>>>>>>> upstream/android-13
		if (ret) {
			dev_err(soc_runtime->dev,
				"error writing to irqen reg: %d\n", ret);
			return ret;
		}
		break;
	}

	return 0;
}

static snd_pcm_uframes_t lpass_platform_pcmops_pointer(
<<<<<<< HEAD
		struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *soc_runtime = substream->private_data;
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(soc_runtime, DRV_NAME);
=======
		struct snd_soc_component *component,
		struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *soc_runtime = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(soc_runtime, 0);
>>>>>>> upstream/android-13
	struct lpass_data *drvdata = snd_soc_component_get_drvdata(component);
	struct snd_pcm_runtime *rt = substream->runtime;
	struct lpass_pcm_data *pcm_data = rt->private_data;
	struct lpass_variant *v = drvdata->variant;
	unsigned int base_addr, curr_addr;
	int ret, ch, dir = substream->stream;
<<<<<<< HEAD

	ch = pcm_data->dma_ch;

	ret = regmap_read(drvdata->lpaif_map,
			LPAIF_DMABASE_REG(v, ch, dir), &base_addr);
=======
	struct regmap *map;
	unsigned int dai_id = cpu_dai->driver->id;

	if (dai_id == LPASS_DP_RX)
		map = drvdata->hdmiif_map;
	else
		map = drvdata->lpaif_map;

	ch = pcm_data->dma_ch;

	ret = regmap_read(map,
			LPAIF_DMABASE_REG(v, ch, dir, dai_id), &base_addr);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(soc_runtime->dev,
			"error reading from rdmabase reg: %d\n", ret);
		return ret;
	}

<<<<<<< HEAD
	ret = regmap_read(drvdata->lpaif_map,
			LPAIF_DMACURR_REG(v, ch, dir), &curr_addr);
=======
	ret = regmap_read(map,
			LPAIF_DMACURR_REG(v, ch, dir, dai_id), &curr_addr);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(soc_runtime->dev,
			"error reading from rdmacurr reg: %d\n", ret);
		return ret;
	}

	return bytes_to_frames(substream->runtime, curr_addr - base_addr);
}

<<<<<<< HEAD
static int lpass_platform_pcmops_mmap(struct snd_pcm_substream *substream,
		struct vm_area_struct *vma)
{
	struct snd_pcm_runtime *runtime = substream->runtime;

	return dma_mmap_coherent(substream->pcm->card->dev, vma,
			runtime->dma_area, runtime->dma_addr,
			runtime->dma_bytes);
}

static const struct snd_pcm_ops lpass_platform_pcm_ops = {
	.open		= lpass_platform_pcmops_open,
	.close		= lpass_platform_pcmops_close,
	.ioctl		= snd_pcm_lib_ioctl,
	.hw_params	= lpass_platform_pcmops_hw_params,
	.hw_free	= lpass_platform_pcmops_hw_free,
	.prepare	= lpass_platform_pcmops_prepare,
	.trigger	= lpass_platform_pcmops_trigger,
	.pointer	= lpass_platform_pcmops_pointer,
	.mmap		= lpass_platform_pcmops_mmap,
};

=======
>>>>>>> upstream/android-13
static irqreturn_t lpass_dma_interrupt_handler(
			struct snd_pcm_substream *substream,
			struct lpass_data *drvdata,
			int chan, u32 interrupts)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *soc_runtime = substream->private_data;
	struct lpass_variant *v = drvdata->variant;
	irqreturn_t ret = IRQ_NONE;
	int rv;

	if (interrupts & LPAIF_IRQ_PER(chan)) {
		rv = regmap_write(drvdata->lpaif_map,
				LPAIF_IRQCLEAR_REG(v, LPAIF_IRQ_PORT_HOST),
				LPAIF_IRQ_PER(chan));
=======
	struct snd_soc_pcm_runtime *soc_runtime = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(soc_runtime, 0);
	struct lpass_variant *v = drvdata->variant;
	irqreturn_t ret = IRQ_NONE;
	int rv;
	unsigned int reg, val, mask;
	struct regmap *map;
	unsigned int dai_id = cpu_dai->driver->id;

	mask = LPAIF_IRQ_ALL(chan);
	switch (dai_id) {
	case LPASS_DP_RX:
		map = drvdata->hdmiif_map;
		reg = LPASS_HDMITX_APP_IRQCLEAR_REG(v);
		val = (LPAIF_IRQ_HDMI_REQ_ON_PRELOAD(chan) |
		LPAIF_IRQ_HDMI_METADONE |
		LPAIF_IRQ_HDMI_SDEEP_AUD_DIS(chan));
	break;
	case MI2S_PRIMARY:
	case MI2S_SECONDARY:
	case MI2S_TERTIARY:
	case MI2S_QUATERNARY:
	case MI2S_QUINARY:
		map = drvdata->lpaif_map;
		reg = LPAIF_IRQCLEAR_REG(v, LPAIF_IRQ_PORT_HOST);
		val = 0;
	break;
	default:
	dev_err(soc_runtime->dev, "%s: invalid  %d interface\n", __func__, dai_id);
	return -EINVAL;
	}
	if (interrupts & LPAIF_IRQ_PER(chan)) {
		rv = regmap_write_bits(map, reg, mask, (LPAIF_IRQ_PER(chan) | val));
>>>>>>> upstream/android-13
		if (rv) {
			dev_err(soc_runtime->dev,
				"error writing to irqclear reg: %d\n", rv);
			return IRQ_NONE;
		}
		snd_pcm_period_elapsed(substream);
		ret = IRQ_HANDLED;
	}

	if (interrupts & LPAIF_IRQ_XRUN(chan)) {
<<<<<<< HEAD
		rv = regmap_write(drvdata->lpaif_map,
				LPAIF_IRQCLEAR_REG(v, LPAIF_IRQ_PORT_HOST),
				LPAIF_IRQ_XRUN(chan));
=======
		rv = regmap_write_bits(map, reg, mask, (LPAIF_IRQ_XRUN(chan) | val));
>>>>>>> upstream/android-13
		if (rv) {
			dev_err(soc_runtime->dev,
				"error writing to irqclear reg: %d\n", rv);
			return IRQ_NONE;
		}
		dev_warn(soc_runtime->dev, "xrun warning\n");
		snd_pcm_stop_xrun(substream);
		ret = IRQ_HANDLED;
	}

	if (interrupts & LPAIF_IRQ_ERR(chan)) {
<<<<<<< HEAD
		rv = regmap_write(drvdata->lpaif_map,
				LPAIF_IRQCLEAR_REG(v, LPAIF_IRQ_PORT_HOST),
				LPAIF_IRQ_ERR(chan));
=======
		rv = regmap_write_bits(map, reg, mask, (LPAIF_IRQ_ERR(chan) | val));
>>>>>>> upstream/android-13
		if (rv) {
			dev_err(soc_runtime->dev,
				"error writing to irqclear reg: %d\n", rv);
			return IRQ_NONE;
		}
		dev_err(soc_runtime->dev, "bus access error\n");
		snd_pcm_stop(substream, SNDRV_PCM_STATE_DISCONNECTED);
		ret = IRQ_HANDLED;
	}

<<<<<<< HEAD
=======
	if (interrupts & val) {
		rv = regmap_write(map, reg, val);
		if (rv) {
			dev_err(soc_runtime->dev,
			"error writing to irqclear reg: %d\n", rv);
			return IRQ_NONE;
		}
		ret = IRQ_HANDLED;
	}

>>>>>>> upstream/android-13
	return ret;
}

static irqreturn_t lpass_platform_lpaif_irq(int irq, void *data)
{
	struct lpass_data *drvdata = data;
	struct lpass_variant *v = drvdata->variant;
	unsigned int irqs;
	int rv, chan;

	rv = regmap_read(drvdata->lpaif_map,
			LPAIF_IRQSTAT_REG(v, LPAIF_IRQ_PORT_HOST), &irqs);
	if (rv) {
		pr_err("error reading from irqstat reg: %d\n", rv);
		return IRQ_NONE;
	}

	/* Handle per channel interrupts */
	for (chan = 0; chan < LPASS_MAX_DMA_CHANNELS; chan++) {
		if (irqs & LPAIF_IRQ_ALL(chan) && drvdata->substream[chan]) {
			rv = lpass_dma_interrupt_handler(
						drvdata->substream[chan],
						drvdata, chan, irqs);
			if (rv != IRQ_HANDLED)
				return rv;
		}
	}

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static int lpass_platform_pcm_new(struct snd_soc_pcm_runtime *soc_runtime)
{
	struct snd_pcm *pcm = soc_runtime->pcm;
	struct snd_pcm_substream *psubstream, *csubstream;
	struct snd_soc_component *component = snd_soc_rtdcom_lookup(soc_runtime, DRV_NAME);
	int ret = -EINVAL;
	size_t size = lpass_platform_pcm_hardware.buffer_bytes_max;

	psubstream = pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream;
	if (psubstream) {
		ret = snd_dma_alloc_pages(SNDRV_DMA_TYPE_DEV,
					component->dev,
					size, &psubstream->dma_buffer);
		if (ret) {
			dev_err(soc_runtime->dev, "Cannot allocate buffer(s)\n");
			return ret;
		}
	}

	csubstream = pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream;
	if (csubstream) {
		ret = snd_dma_alloc_pages(SNDRV_DMA_TYPE_DEV,
					component->dev,
					size, &csubstream->dma_buffer);
		if (ret) {
			dev_err(soc_runtime->dev, "Cannot allocate buffer(s)\n");
			if (psubstream)
				snd_dma_free_pages(&psubstream->dma_buffer);
			return ret;
		}

	}
=======
static irqreturn_t lpass_platform_hdmiif_irq(int irq, void *data)
{
	struct lpass_data *drvdata = data;
	struct lpass_variant *v = drvdata->variant;
	unsigned int irqs;
	int rv, chan;

	rv = regmap_read(drvdata->hdmiif_map,
			LPASS_HDMITX_APP_IRQSTAT_REG(v), &irqs);
	if (rv) {
		pr_err("error reading from irqstat reg: %d\n", rv);
		return IRQ_NONE;
	}

	/* Handle per channel interrupts */
	for (chan = 0; chan < LPASS_MAX_HDMI_DMA_CHANNELS; chan++) {
		if (irqs & (LPAIF_IRQ_ALL(chan) | LPAIF_IRQ_HDMI_REQ_ON_PRELOAD(chan) |
				LPAIF_IRQ_HDMI_METADONE |
				LPAIF_IRQ_HDMI_SDEEP_AUD_DIS(chan))
			&& drvdata->hdmi_substream[chan]) {
			rv = lpass_dma_interrupt_handler(
						drvdata->hdmi_substream[chan],
						drvdata, chan, irqs);
			if (rv != IRQ_HANDLED)
				return rv;
		}
	}

	return IRQ_HANDLED;
}

static int lpass_platform_pcm_new(struct snd_soc_component *component,
				  struct snd_soc_pcm_runtime *soc_runtime)
{
	struct snd_pcm *pcm = soc_runtime->pcm;
	size_t size = lpass_platform_pcm_hardware.buffer_bytes_max;

	return snd_pcm_set_fixed_buffer_all(pcm, SNDRV_DMA_TYPE_DEV,
					    component->dev, size);
}

static int lpass_platform_pcmops_suspend(struct snd_soc_component *component)
{
	struct lpass_data *drvdata = snd_soc_component_get_drvdata(component);
	struct regmap *map;
	unsigned int dai_id = component->id;

	if (dai_id == LPASS_DP_RX)
		map = drvdata->hdmiif_map;
	else
		map = drvdata->lpaif_map;

	regcache_cache_only(map, true);
	regcache_mark_dirty(map);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void lpass_platform_pcm_free(struct snd_pcm *pcm)
{
	struct snd_pcm_substream *substream;
	int i;

	for (i = 0; i < ARRAY_SIZE(pcm->streams); i++) {
		substream = pcm->streams[i].substream;
		if (substream) {
			snd_dma_free_pages(&substream->dma_buffer);
			substream->dma_buffer.area = NULL;
			substream->dma_buffer.addr = 0;
		}
	}
}

static const struct snd_soc_component_driver lpass_component_driver = {
	.name		= DRV_NAME,
	.pcm_new	= lpass_platform_pcm_new,
	.pcm_free	= lpass_platform_pcm_free,
	.ops		= &lpass_platform_pcm_ops,
=======
static int lpass_platform_pcmops_resume(struct snd_soc_component *component)
{
	struct lpass_data *drvdata = snd_soc_component_get_drvdata(component);
	struct regmap *map;
	unsigned int dai_id = component->id;

	if (dai_id == LPASS_DP_RX)
		map = drvdata->hdmiif_map;
	else
		map = drvdata->lpaif_map;

	regcache_cache_only(map, false);
	return regcache_sync(map);
}


static const struct snd_soc_component_driver lpass_component_driver = {
	.name		= DRV_NAME,
	.open		= lpass_platform_pcmops_open,
	.close		= lpass_platform_pcmops_close,
	.hw_params	= lpass_platform_pcmops_hw_params,
	.hw_free	= lpass_platform_pcmops_hw_free,
	.prepare	= lpass_platform_pcmops_prepare,
	.trigger	= lpass_platform_pcmops_trigger,
	.pointer	= lpass_platform_pcmops_pointer,
	.pcm_construct	= lpass_platform_pcm_new,
	.suspend		= lpass_platform_pcmops_suspend,
	.resume			= lpass_platform_pcmops_resume,

>>>>>>> upstream/android-13
};

int asoc_qcom_lpass_platform_register(struct platform_device *pdev)
{
	struct lpass_data *drvdata = platform_get_drvdata(pdev);
	struct lpass_variant *v = drvdata->variant;
	int ret;

	drvdata->lpaif_irq = platform_get_irq_byname(pdev, "lpass-irq-lpaif");
<<<<<<< HEAD
	if (drvdata->lpaif_irq < 0) {
		dev_err(&pdev->dev, "error getting irq handle: %d\n",
			drvdata->lpaif_irq);
		return -ENODEV;
	}
=======
	if (drvdata->lpaif_irq < 0)
		return -ENODEV;
>>>>>>> upstream/android-13

	/* ensure audio hardware is disabled */
	ret = regmap_write(drvdata->lpaif_map,
			LPAIF_IRQEN_REG(v, LPAIF_IRQ_PORT_HOST), 0);
	if (ret) {
		dev_err(&pdev->dev, "error writing to irqen reg: %d\n", ret);
		return ret;
	}

	ret = devm_request_irq(&pdev->dev, drvdata->lpaif_irq,
			lpass_platform_lpaif_irq, IRQF_TRIGGER_RISING,
			"lpass-irq-lpaif", drvdata);
	if (ret) {
		dev_err(&pdev->dev, "irq request failed: %d\n", ret);
		return ret;
	}

<<<<<<< HEAD

=======
	ret = lpass_platform_alloc_dmactl_fields(&pdev->dev,
						 drvdata->lpaif_map);
	if (ret) {
		dev_err(&pdev->dev,
			"error initializing dmactl fields: %d\n", ret);
		return ret;
	}

	if (drvdata->hdmi_port_enable) {
		drvdata->hdmiif_irq = platform_get_irq_byname(pdev, "lpass-irq-hdmi");
		if (drvdata->hdmiif_irq < 0)
			return -ENODEV;

		ret = devm_request_irq(&pdev->dev, drvdata->hdmiif_irq,
				lpass_platform_hdmiif_irq, 0, "lpass-irq-hdmi", drvdata);
		if (ret) {
			dev_err(&pdev->dev, "irq hdmi request failed: %d\n", ret);
			return ret;
		}
		ret = regmap_write(drvdata->hdmiif_map,
				LPASS_HDMITX_APP_IRQEN_REG(v), 0);
		if (ret) {
			dev_err(&pdev->dev, "error writing to hdmi irqen reg: %d\n", ret);
			return ret;
		}

		ret = lpass_platform_alloc_hdmidmactl_fields(&pdev->dev,
							 drvdata->hdmiif_map);
		if (ret) {
			dev_err(&pdev->dev,
				"error initializing hdmidmactl fields: %d\n", ret);
			return ret;
		}
	}
>>>>>>> upstream/android-13
	return devm_snd_soc_register_component(&pdev->dev,
			&lpass_component_driver, NULL, 0);
}
EXPORT_SYMBOL_GPL(asoc_qcom_lpass_platform_register);

MODULE_DESCRIPTION("QTi LPASS Platform Driver");
MODULE_LICENSE("GPL v2");
