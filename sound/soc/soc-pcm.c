// SPDX-License-Identifier: GPL-2.0+
//
// soc-pcm.c  --  ALSA SoC PCM
//
// Copyright 2005 Wolfson Microelectronics PLC.
// Copyright 2005 Openedhand Ltd.
// Copyright (C) 2010 Slimlogic Ltd.
// Copyright (C) 2010 Texas Instruments Inc.
//
// Authors: Liam Girdwood <lrg@ti.com>
//          Mark Brown <broonie@opensource.wolfsonmicro.com>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/export.h>
#include <linux/debugfs.h>
<<<<<<< HEAD
#include <linux/dma-mapping.h>
=======
>>>>>>> upstream/android-13
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dpcm.h>
<<<<<<< HEAD
=======
#include <sound/soc-link.h>
>>>>>>> upstream/android-13
#include <sound/initval.h>

#define DPCM_MAX_BE_USERS	8

<<<<<<< HEAD
/*
 * ASoC no host IO hardware.
 * TODO: fine tune these values for all host less transfers.
 */
static const struct snd_pcm_hardware no_host_hardware = {
	.info			= SNDRV_PCM_INFO_MMAP |
				  SNDRV_PCM_INFO_MMAP_VALID |
				  SNDRV_PCM_INFO_INTERLEAVED |
				  SNDRV_PCM_INFO_PAUSE |
				  SNDRV_PCM_INFO_RESUME,
	.formats		= SNDRV_PCM_FMTBIT_S16_LE |
				  SNDRV_PCM_FMTBIT_S32_LE,
	.period_bytes_min	= PAGE_SIZE >> 2,
	.period_bytes_max	= PAGE_SIZE >> 1,
	.periods_min		= 2,
	.periods_max		= 4,
	/*
	 * Increase the max buffer bytes as PAGE_SIZE bytes is
	 * not enough to encompass all the scenarios sent by
	 * userspapce.
	 */
	.buffer_bytes_max	= PAGE_SIZE * 4,
};

/*
 * snd_soc_dai_stream_valid() - check if a DAI supports the given stream
 *
 * Returns true if the DAI supports the indicated stream type.
 */
static bool snd_soc_dai_stream_valid(struct snd_soc_dai *dai, int stream)
{
	struct snd_soc_pcm_stream *codec_stream;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		codec_stream = &dai->driver->playback;
	else
		codec_stream = &dai->driver->capture;

	/* If the codec specifies any channels at all, it supports the stream */
	return codec_stream->channels_min;
}

/**
 * snd_soc_runtime_activate() - Increment active count for PCM runtime components
 * @rtd: ASoC PCM runtime that is activated
 * @stream: Direction of the PCM stream
 *
 * Increments the active count for all the DAIs and components attached to a PCM
 * runtime. Should typically be called when a stream is opened.
 *
 * Must be called with the rtd->pcm_mutex being held
 */
void snd_soc_runtime_activate(struct snd_soc_pcm_runtime *rtd, int stream)
{
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int i;

	lockdep_assert_held(&rtd->pcm_mutex);

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		cpu_dai->playback_active++;
		for (i = 0; i < rtd->num_codecs; i++)
			rtd->codec_dais[i]->playback_active++;
	} else {
		cpu_dai->capture_active++;
		for (i = 0; i < rtd->num_codecs; i++)
			rtd->codec_dais[i]->capture_active++;
	}

	cpu_dai->active++;
	cpu_dai->component->active++;
	for (i = 0; i < rtd->num_codecs; i++) {
		rtd->codec_dais[i]->active++;
		rtd->codec_dais[i]->component->active++;
	}
}

/**
 * snd_soc_runtime_deactivate() - Decrement active count for PCM runtime components
 * @rtd: ASoC PCM runtime that is deactivated
 * @stream: Direction of the PCM stream
 *
 * Decrements the active count for all the DAIs and components attached to a PCM
 * runtime. Should typically be called when a stream is closed.
 *
 * Must be called with the rtd->pcm_mutex being held
 */
void snd_soc_runtime_deactivate(struct snd_soc_pcm_runtime *rtd, int stream)
{
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int i;

	lockdep_assert_held(&rtd->pcm_mutex);

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		cpu_dai->playback_active--;
		for (i = 0; i < rtd->num_codecs; i++)
			rtd->codec_dais[i]->playback_active--;
	} else {
		cpu_dai->capture_active--;
		for (i = 0; i < rtd->num_codecs; i++)
			rtd->codec_dais[i]->capture_active--;
	}

	cpu_dai->active--;
	cpu_dai->component->active--;
	for (i = 0; i < rtd->num_codecs; i++) {
		rtd->codec_dais[i]->component->active--;
		rtd->codec_dais[i]->active--;
	}
}
=======
static inline const char *soc_cpu_dai_name(struct snd_soc_pcm_runtime *rtd)
{
	return (rtd)->num_cpus == 1 ? asoc_rtd_to_cpu(rtd, 0)->name : "multicpu";
}
static inline const char *soc_codec_dai_name(struct snd_soc_pcm_runtime *rtd)
{
	return (rtd)->num_codecs == 1 ? asoc_rtd_to_codec(rtd, 0)->name : "multicodec";
}

#ifdef CONFIG_DEBUG_FS
static const char *dpcm_state_string(enum snd_soc_dpcm_state state)
{
	switch (state) {
	case SND_SOC_DPCM_STATE_NEW:
		return "new";
	case SND_SOC_DPCM_STATE_OPEN:
		return "open";
	case SND_SOC_DPCM_STATE_HW_PARAMS:
		return "hw_params";
	case SND_SOC_DPCM_STATE_PREPARE:
		return "prepare";
	case SND_SOC_DPCM_STATE_START:
		return "start";
	case SND_SOC_DPCM_STATE_STOP:
		return "stop";
	case SND_SOC_DPCM_STATE_SUSPEND:
		return "suspend";
	case SND_SOC_DPCM_STATE_PAUSED:
		return "paused";
	case SND_SOC_DPCM_STATE_HW_FREE:
		return "hw_free";
	case SND_SOC_DPCM_STATE_CLOSE:
		return "close";
	}

	return "unknown";
}

static ssize_t dpcm_show_state(struct snd_soc_pcm_runtime *fe,
			       int stream, char *buf, size_t size)
{
	struct snd_pcm_hw_params *params = &fe->dpcm[stream].hw_params;
	struct snd_soc_dpcm *dpcm;
	ssize_t offset = 0;
	unsigned long flags;

	/* FE state */
	offset += scnprintf(buf + offset, size - offset,
			   "[%s - %s]\n", fe->dai_link->name,
			   stream ? "Capture" : "Playback");

	offset += scnprintf(buf + offset, size - offset, "State: %s\n",
			   dpcm_state_string(fe->dpcm[stream].state));

	if ((fe->dpcm[stream].state >= SND_SOC_DPCM_STATE_HW_PARAMS) &&
	    (fe->dpcm[stream].state <= SND_SOC_DPCM_STATE_STOP))
		offset += scnprintf(buf + offset, size - offset,
				   "Hardware Params: "
				   "Format = %s, Channels = %d, Rate = %d\n",
				   snd_pcm_format_name(params_format(params)),
				   params_channels(params),
				   params_rate(params));

	/* BEs state */
	offset += scnprintf(buf + offset, size - offset, "Backends:\n");

	if (list_empty(&fe->dpcm[stream].be_clients)) {
		offset += scnprintf(buf + offset, size - offset,
				   " No active DSP links\n");
		goto out;
	}

	spin_lock_irqsave(&fe->card->dpcm_lock, flags);
	for_each_dpcm_be(fe, stream, dpcm) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		params = &dpcm->hw_params;

		offset += scnprintf(buf + offset, size - offset,
				   "- %s\n", be->dai_link->name);

		offset += scnprintf(buf + offset, size - offset,
				   "   State: %s\n",
				   dpcm_state_string(be->dpcm[stream].state));

		if ((be->dpcm[stream].state >= SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state <= SND_SOC_DPCM_STATE_STOP))
			offset += scnprintf(buf + offset, size - offset,
					   "   Hardware Params: "
					   "Format = %s, Channels = %d, Rate = %d\n",
					   snd_pcm_format_name(params_format(params)),
					   params_channels(params),
					   params_rate(params));
	}
	spin_unlock_irqrestore(&fe->card->dpcm_lock, flags);
out:
	return offset;
}

static ssize_t dpcm_state_read_file(struct file *file, char __user *user_buf,
				    size_t count, loff_t *ppos)
{
	struct snd_soc_pcm_runtime *fe = file->private_data;
	ssize_t out_count = PAGE_SIZE, offset = 0, ret = 0;
	int stream;
	char *buf;

	if (fe->num_cpus > 1) {
		dev_err(fe->dev,
			"%s doesn't support Multi CPU yet\n", __func__);
		return -EINVAL;
	}

	buf = kmalloc(out_count, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	for_each_pcm_streams(stream)
		if (snd_soc_dai_stream_valid(asoc_rtd_to_cpu(fe, 0), stream))
			offset += dpcm_show_state(fe, stream,
						  buf + offset,
						  out_count - offset);

	ret = simple_read_from_buffer(user_buf, count, ppos, buf, offset);

	kfree(buf);
	return ret;
}

static const struct file_operations dpcm_state_fops = {
	.open = simple_open,
	.read = dpcm_state_read_file,
	.llseek = default_llseek,
};

void soc_dpcm_debugfs_add(struct snd_soc_pcm_runtime *rtd)
{
	if (!rtd->dai_link->dynamic)
		return;

	if (!rtd->card->debugfs_card_root)
		return;

	rtd->debugfs_dpcm_root = debugfs_create_dir(rtd->dai_link->name,
						    rtd->card->debugfs_card_root);

	debugfs_create_file("state", 0444, rtd->debugfs_dpcm_root,
			    rtd, &dpcm_state_fops);
}

static void dpcm_create_debugfs_state(struct snd_soc_dpcm *dpcm, int stream)
{
	char *name;

	name = kasprintf(GFP_KERNEL, "%s:%s", dpcm->be->dai_link->name,
			 stream ? "capture" : "playback");
	if (name) {
		dpcm->debugfs_state = debugfs_create_dir(
			name, dpcm->fe->debugfs_dpcm_root);
		debugfs_create_u32("state", 0644, dpcm->debugfs_state,
				   &dpcm->state);
		kfree(name);
	}
}

static void dpcm_remove_debugfs_state(struct snd_soc_dpcm *dpcm)
{
	debugfs_remove_recursive(dpcm->debugfs_state);
}

#else
static inline void dpcm_create_debugfs_state(struct snd_soc_dpcm *dpcm,
					     int stream)
{
}

static inline void dpcm_remove_debugfs_state(struct snd_soc_dpcm *dpcm)
{
}
#endif

/* Set FE's runtime_update state; the state is protected via PCM stream lock
 * for avoiding the race with trigger callback.
 * If the state is unset and a trigger is pending while the previous operation,
 * process the pending trigger action here.
 */
static int dpcm_fe_dai_do_trigger(struct snd_pcm_substream *substream, int cmd);
static void dpcm_set_fe_update_state(struct snd_soc_pcm_runtime *fe,
				     int stream, enum snd_soc_dpcm_update state)
{
	struct snd_pcm_substream *substream =
		snd_soc_dpcm_get_substream(fe, stream);

	snd_pcm_stream_lock_irq(substream);
	if (state == SND_SOC_DPCM_UPDATE_NO && fe->dpcm[stream].trigger_pending) {
		dpcm_fe_dai_do_trigger(substream,
				       fe->dpcm[stream].trigger_pending - 1);
		fe->dpcm[stream].trigger_pending = 0;
	}
	fe->dpcm[stream].runtime_update = state;
	snd_pcm_stream_unlock_irq(substream);
}

static void dpcm_set_be_update_state(struct snd_soc_pcm_runtime *be,
				     int stream, enum snd_soc_dpcm_update state)
{
	be->dpcm[stream].runtime_update = state;
}

/**
 * snd_soc_runtime_action() - Increment/Decrement active count for
 * PCM runtime components
 * @rtd: ASoC PCM runtime that is activated
 * @stream: Direction of the PCM stream
 * @action: Activate stream if 1. Deactivate if -1.
 *
 * Increments/Decrements the active count for all the DAIs and components
 * attached to a PCM runtime.
 * Should typically be called when a stream is opened.
 *
 * Must be called with the rtd->card->pcm_mutex being held
 */
void snd_soc_runtime_action(struct snd_soc_pcm_runtime *rtd,
			    int stream, int action)
{
	struct snd_soc_dai *dai;
	int i;

	lockdep_assert_held(&rtd->card->pcm_mutex);

	for_each_rtd_dais(rtd, i, dai)
		snd_soc_dai_action(dai, stream, action);
}
EXPORT_SYMBOL_GPL(snd_soc_runtime_action);
>>>>>>> upstream/android-13

/**
 * snd_soc_runtime_ignore_pmdown_time() - Check whether to ignore the power down delay
 * @rtd: The ASoC PCM runtime that should be checked.
 *
 * This function checks whether the power down delay should be ignored for a
 * specific PCM runtime. Returns true if the delay is 0, if it the DAI link has
 * been configured to ignore the delay, or if none of the components benefits
 * from having the delay.
 */
bool snd_soc_runtime_ignore_pmdown_time(struct snd_soc_pcm_runtime *rtd)
{
<<<<<<< HEAD
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_component *component;
	bool ignore = true;
=======
	struct snd_soc_component *component;
	bool ignore = true;
	int i;
>>>>>>> upstream/android-13

	if (!rtd->pmdown_time || rtd->dai_link->ignore_pmdown_time)
		return true;

<<<<<<< HEAD
	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		ignore &= !component->driver->use_pmdown_time;
	}
=======
	for_each_rtd_components(rtd, i, component)
		ignore &= !component->driver->use_pmdown_time;
>>>>>>> upstream/android-13

	return ignore;
}

/**
 * snd_soc_set_runtime_hwparams - set the runtime hardware parameters
 * @substream: the pcm substream
 * @hw: the hardware parameters
 *
 * Sets the substream runtime hardware parameters.
 */
int snd_soc_set_runtime_hwparams(struct snd_pcm_substream *substream,
	const struct snd_pcm_hardware *hw)
{
<<<<<<< HEAD
	struct snd_pcm_runtime *runtime = substream->runtime;
	if (!runtime)
		return 0;
	runtime->hw.info = hw->info;
	runtime->hw.formats = hw->formats;
	runtime->hw.period_bytes_min = hw->period_bytes_min;
	runtime->hw.period_bytes_max = hw->period_bytes_max;
	runtime->hw.periods_min = hw->periods_min;
	runtime->hw.periods_max = hw->periods_max;
	runtime->hw.buffer_bytes_max = hw->buffer_bytes_max;
	runtime->hw.fifo_size = hw->fifo_size;
=======
	substream->runtime->hw = *hw;

>>>>>>> upstream/android-13
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_set_runtime_hwparams);

/* DPCM stream event, send event to FE and all active BEs. */
int dpcm_dapm_stream_event(struct snd_soc_pcm_runtime *fe, int dir,
	int event)
{
	struct snd_soc_dpcm *dpcm;

<<<<<<< HEAD
	list_for_each_entry(dpcm, &fe->dpcm[dir].be_clients, list_be) {
=======
	for_each_dpcm_be(fe, dir, dpcm) {
>>>>>>> upstream/android-13

		struct snd_soc_pcm_runtime *be = dpcm->be;

		dev_dbg(be->dev, "ASoC: BE %s event %d dir %d\n",
				be->dai_link->name, event, dir);

		if ((event == SND_SOC_DAPM_STREAM_STOP) &&
		    (be->dpcm[dir].users >= 1))
			continue;

		snd_soc_dapm_stream_event(be, dir, event);
	}

	snd_soc_dapm_stream_event(fe, dir, event);

	return 0;
}

<<<<<<< HEAD
static int soc_pcm_apply_symmetry(struct snd_pcm_substream *substream,
					struct snd_soc_dai *soc_dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	int ret;

	if (soc_dai->rate && (soc_dai->driver->symmetric_rates ||
				rtd->dai_link->symmetric_rates)) {
		dev_dbg(soc_dai->dev, "ASoC: Symmetry forces %dHz rate\n",
				soc_dai->rate);

		ret = snd_pcm_hw_constraint_single(substream->runtime,
						SNDRV_PCM_HW_PARAM_RATE,
						soc_dai->rate);
		if (ret < 0) {
			dev_err(soc_dai->dev,
				"ASoC: Unable to apply rate constraint: %d\n",
				ret);
			return ret;
		}
	}

	if (soc_dai->channels && (soc_dai->driver->symmetric_channels ||
				rtd->dai_link->symmetric_channels)) {
		dev_dbg(soc_dai->dev, "ASoC: Symmetry forces %d channel(s)\n",
				soc_dai->channels);

		ret = snd_pcm_hw_constraint_single(substream->runtime,
						SNDRV_PCM_HW_PARAM_CHANNELS,
						soc_dai->channels);
		if (ret < 0) {
			dev_err(soc_dai->dev,
				"ASoC: Unable to apply channel symmetry constraint: %d\n",
				ret);
			return ret;
		}
	}

	if (soc_dai->sample_bits && (soc_dai->driver->symmetric_samplebits ||
				rtd->dai_link->symmetric_samplebits)) {
		dev_dbg(soc_dai->dev, "ASoC: Symmetry forces %d sample bits\n",
				soc_dai->sample_bits);

		ret = snd_pcm_hw_constraint_single(substream->runtime,
						SNDRV_PCM_HW_PARAM_SAMPLE_BITS,
						soc_dai->sample_bits);
		if (ret < 0) {
			dev_err(soc_dai->dev,
				"ASoC: Unable to apply sample bits symmetry constraint: %d\n",
				ret);
			return ret;
		}
	}
=======
static void soc_pcm_set_dai_params(struct snd_soc_dai *dai,
				   struct snd_pcm_hw_params *params)
{
	if (params) {
		dai->rate	 = params_rate(params);
		dai->channels	 = params_channels(params);
		dai->sample_bits = snd_pcm_format_physical_width(params_format(params));
	} else {
		dai->rate	 = 0;
		dai->channels	 = 0;
		dai->sample_bits = 0;
	}
}

static int soc_pcm_apply_symmetry(struct snd_pcm_substream *substream,
					struct snd_soc_dai *soc_dai)
{
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	int ret;

	if (!snd_soc_dai_active(soc_dai))
		return 0;

#define __soc_pcm_apply_symmetry(name, NAME)				\
	if (soc_dai->name && (soc_dai->driver->symmetric_##name ||	\
			      rtd->dai_link->symmetric_##name)) {	\
		dev_dbg(soc_dai->dev, "ASoC: Symmetry forces %s to %d\n",\
			#name, soc_dai->name);				\
									\
		ret = snd_pcm_hw_constraint_single(substream->runtime,	\
						   SNDRV_PCM_HW_PARAM_##NAME,\
						   soc_dai->name);	\
		if (ret < 0) {						\
			dev_err(soc_dai->dev,				\
				"ASoC: Unable to apply %s constraint: %d\n",\
				#name, ret);				\
			return ret;					\
		}							\
	}

	__soc_pcm_apply_symmetry(rate,		RATE);
	__soc_pcm_apply_symmetry(channels,	CHANNELS);
	__soc_pcm_apply_symmetry(sample_bits,	SAMPLE_BITS);
>>>>>>> upstream/android-13

	return 0;
}

static int soc_pcm_params_symmetry(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	unsigned int rate, channels, sample_bits, symmetry, i;

	rate = params_rate(params);
	channels = params_channels(params);
	sample_bits = snd_pcm_format_physical_width(params_format(params));

	/* reject unmatched parameters when applying symmetry */
	symmetry = cpu_dai->driver->symmetric_rates ||
		rtd->dai_link->symmetric_rates;

	for (i = 0; i < rtd->num_codecs; i++)
		symmetry |= rtd->codec_dais[i]->driver->symmetric_rates;

	if (symmetry && cpu_dai->rate && cpu_dai->rate != rate) {
		dev_err(rtd->dev, "ASoC: unmatched rate symmetry: %d - %d\n",
				cpu_dai->rate, rate);
		return -EINVAL;
	}

	symmetry = cpu_dai->driver->symmetric_channels ||
		rtd->dai_link->symmetric_channels;

	for (i = 0; i < rtd->num_codecs; i++)
		symmetry |= rtd->codec_dais[i]->driver->symmetric_channels;

	if (symmetry && cpu_dai->channels && cpu_dai->channels != channels) {
		dev_err(rtd->dev, "ASoC: unmatched channel symmetry: %d - %d\n",
				cpu_dai->channels, channels);
		return -EINVAL;
	}

	symmetry = cpu_dai->driver->symmetric_samplebits ||
		rtd->dai_link->symmetric_samplebits;

	for (i = 0; i < rtd->num_codecs; i++)
		symmetry |= rtd->codec_dais[i]->driver->symmetric_samplebits;

	if (symmetry && cpu_dai->sample_bits && cpu_dai->sample_bits != sample_bits) {
		dev_err(rtd->dev, "ASoC: unmatched sample bits symmetry: %d - %d\n",
				cpu_dai->sample_bits, sample_bits);
		return -EINVAL;
	}
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai d;
	struct snd_soc_dai *dai;
	struct snd_soc_dai *cpu_dai;
	unsigned int symmetry, i;

	d.name = __func__;
	soc_pcm_set_dai_params(&d, params);

#define __soc_pcm_params_symmetry(xxx)					\
	symmetry = rtd->dai_link->symmetric_##xxx;			\
	for_each_rtd_dais(rtd, i, dai)					\
		symmetry |= dai->driver->symmetric_##xxx;		\
									\
	if (symmetry)							\
		for_each_rtd_cpu_dais(rtd, i, cpu_dai)			\
			if (!snd_soc_dai_is_dummy(cpu_dai) &&		\
			    cpu_dai->xxx && cpu_dai->xxx != d.xxx) {	\
				dev_err(rtd->dev, "ASoC: unmatched %s symmetry: %s:%d - %s:%d\n", \
					#xxx, cpu_dai->name, cpu_dai->xxx, d.name, d.xxx); \
				return -EINVAL;				\
			}

	/* reject unmatched parameters when applying symmetry */
	__soc_pcm_params_symmetry(rate);
	__soc_pcm_params_symmetry(channels);
	__soc_pcm_params_symmetry(sample_bits);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static bool soc_pcm_has_symmetry(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai_driver *cpu_driver = rtd->cpu_dai->driver;
	struct snd_soc_dai_link *link = rtd->dai_link;
	unsigned int symmetry, i;

	symmetry = cpu_driver->symmetric_rates || link->symmetric_rates ||
		cpu_driver->symmetric_channels || link->symmetric_channels ||
		cpu_driver->symmetric_samplebits || link->symmetric_samplebits;

	for (i = 0; i < rtd->num_codecs; i++)
		symmetry = symmetry ||
			rtd->codec_dais[i]->driver->symmetric_rates ||
			rtd->codec_dais[i]->driver->symmetric_channels ||
			rtd->codec_dais[i]->driver->symmetric_samplebits;

	return symmetry;
=======
static void soc_pcm_update_symmetry(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai_link *link = rtd->dai_link;
	struct snd_soc_dai *dai;
	unsigned int symmetry, i;

	symmetry = link->symmetric_rate ||
		link->symmetric_channels ||
		link->symmetric_sample_bits;

	for_each_rtd_dais(rtd, i, dai)
		symmetry = symmetry ||
			dai->driver->symmetric_rate ||
			dai->driver->symmetric_channels ||
			dai->driver->symmetric_sample_bits;

	if (symmetry)
		substream->runtime->hw.info |= SNDRV_PCM_INFO_JOINT_DUPLEX;
>>>>>>> upstream/android-13
}

static void soc_pcm_set_msb(struct snd_pcm_substream *substream, int bits)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
>>>>>>> upstream/android-13
	int ret;

	if (!bits)
		return;

	ret = snd_pcm_hw_constraint_msbits(substream->runtime, 0, 0, bits);
	if (ret != 0)
		dev_warn(rtd->dev, "ASoC: Failed to set MSB %d: %d\n",
				 bits, ret);
}

static void soc_pcm_apply_msb(struct snd_pcm_substream *substream)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai;
	int i;
	unsigned int bits = 0, cpu_bits;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		for (i = 0; i < rtd->num_codecs; i++) {
			codec_dai = rtd->codec_dais[i];
			if (codec_dai->driver->playback.sig_bits == 0) {
				bits = 0;
				break;
			}
			bits = max(codec_dai->driver->playback.sig_bits, bits);
		}
		cpu_bits = cpu_dai->driver->playback.sig_bits;
	} else {
		for (i = 0; i < rtd->num_codecs; i++) {
			codec_dai = rtd->codec_dais[i];
			if (codec_dai->driver->capture.sig_bits == 0) {
				bits = 0;
				break;
			}
			bits = max(codec_dai->driver->capture.sig_bits, bits);
		}
		cpu_bits = cpu_dai->driver->capture.sig_bits;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai;
	struct snd_soc_dai *codec_dai;
	int stream = substream->stream;
	int i;
	unsigned int bits = 0, cpu_bits = 0;

	for_each_rtd_codec_dais(rtd, i, codec_dai) {
		struct snd_soc_pcm_stream *pcm_codec = snd_soc_dai_get_pcm_stream(codec_dai, stream);

		if (pcm_codec->sig_bits == 0) {
			bits = 0;
			break;
		}
		bits = max(pcm_codec->sig_bits, bits);
	}

	for_each_rtd_cpu_dais(rtd, i, cpu_dai) {
		struct snd_soc_pcm_stream *pcm_cpu = snd_soc_dai_get_pcm_stream(cpu_dai, stream);

		if (pcm_cpu->sig_bits == 0) {
			cpu_bits = 0;
			break;
		}
		cpu_bits = max(pcm_cpu->sig_bits, cpu_bits);
>>>>>>> upstream/android-13
	}

	soc_pcm_set_msb(substream, bits);
	soc_pcm_set_msb(substream, cpu_bits);
}

<<<<<<< HEAD
static void soc_pcm_init_runtime_hw(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_pcm_hardware *hw = &runtime->hw;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai_driver *cpu_dai_drv = rtd->cpu_dai->driver;
	struct snd_soc_dai_driver *codec_dai_drv;
	struct snd_soc_pcm_stream *codec_stream;
	struct snd_soc_pcm_stream *cpu_stream;
	unsigned int chan_min = 0, chan_max = UINT_MAX;
	unsigned int rate_min = 0, rate_max = UINT_MAX;
	unsigned int rates = UINT_MAX;
	u64 formats = ULLONG_MAX;
	int i;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		cpu_stream = &cpu_dai_drv->playback;
	else
		cpu_stream = &cpu_dai_drv->capture;

	/* first calculate min/max only for CODECs in the DAI link */
	for (i = 0; i < rtd->num_codecs; i++) {
=======
static void soc_pcm_hw_init(struct snd_pcm_hardware *hw)
{
	hw->rates		= UINT_MAX;
	hw->rate_min		= 0;
	hw->rate_max		= UINT_MAX;
	hw->channels_min	= 0;
	hw->channels_max	= UINT_MAX;
	hw->formats		= ULLONG_MAX;
}

static void soc_pcm_hw_update_rate(struct snd_pcm_hardware *hw,
				   struct snd_soc_pcm_stream *p)
{
	hw->rates = snd_pcm_rate_mask_intersect(hw->rates, p->rates);

	/* setup hw->rate_min/max via hw->rates first */
	snd_pcm_hw_limit_rates(hw);

	/* update hw->rate_min/max by snd_soc_pcm_stream */
	hw->rate_min = max(hw->rate_min, p->rate_min);
	hw->rate_max = min_not_zero(hw->rate_max, p->rate_max);
}

static void soc_pcm_hw_update_chan(struct snd_pcm_hardware *hw,
				   struct snd_soc_pcm_stream *p)
{
	hw->channels_min = max(hw->channels_min, p->channels_min);
	hw->channels_max = min(hw->channels_max, p->channels_max);
}

static void soc_pcm_hw_update_format(struct snd_pcm_hardware *hw,
				     struct snd_soc_pcm_stream *p)
{
	hw->formats &= p->formats;
}

/**
 * snd_soc_runtime_calc_hw() - Calculate hw limits for a PCM stream
 * @rtd: ASoC PCM runtime
 * @hw: PCM hardware parameters (output)
 * @stream: Direction of the PCM stream
 *
 * Calculates the subset of stream parameters supported by all DAIs
 * associated with the PCM stream.
 */
int snd_soc_runtime_calc_hw(struct snd_soc_pcm_runtime *rtd,
			    struct snd_pcm_hardware *hw, int stream)
{
	struct snd_soc_dai *codec_dai;
	struct snd_soc_dai *cpu_dai;
	struct snd_soc_pcm_stream *codec_stream;
	struct snd_soc_pcm_stream *cpu_stream;
	unsigned int cpu_chan_min = 0, cpu_chan_max = UINT_MAX;
	int i;

	soc_pcm_hw_init(hw);

	/* first calculate min/max only for CPUs in the DAI link */
	for_each_rtd_cpu_dais(rtd, i, cpu_dai) {

		/*
		 * Skip CPUs which don't support the current stream type.
		 * Otherwise, since the rate, channel, and format values will
		 * zero in that case, we would have no usable settings left,
		 * causing the resulting setup to fail.
		 */
		if (!snd_soc_dai_stream_valid(cpu_dai, stream))
			continue;

		cpu_stream = snd_soc_dai_get_pcm_stream(cpu_dai, stream);

		soc_pcm_hw_update_chan(hw, cpu_stream);
		soc_pcm_hw_update_rate(hw, cpu_stream);
		soc_pcm_hw_update_format(hw, cpu_stream);
	}
	cpu_chan_min = hw->channels_min;
	cpu_chan_max = hw->channels_max;

	/* second calculate min/max only for CODECs in the DAI link */
	for_each_rtd_codec_dais(rtd, i, codec_dai) {
>>>>>>> upstream/android-13

		/*
		 * Skip CODECs which don't support the current stream type.
		 * Otherwise, since the rate, channel, and format values will
		 * zero in that case, we would have no usable settings left,
		 * causing the resulting setup to fail.
<<<<<<< HEAD
		 * At least one CODEC should match, otherwise we should have
		 * bailed out on a higher level, since there would be no
		 * CODEC to support the transfer direction in that case.
		 */
		if (!snd_soc_dai_stream_valid(rtd->codec_dais[i],
					      substream->stream))
			continue;

		codec_dai_drv = rtd->codec_dais[i]->driver;
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			codec_stream = &codec_dai_drv->playback;
		else
			codec_stream = &codec_dai_drv->capture;
		chan_min = max(chan_min, codec_stream->channels_min);
		chan_max = min(chan_max, codec_stream->channels_max);
		rate_min = max(rate_min, codec_stream->rate_min);
		rate_max = min_not_zero(rate_max, codec_stream->rate_max);
		formats &= codec_stream->formats;
		rates = snd_pcm_rate_mask_intersect(codec_stream->rates, rates);
	}

	/*
	 * chan min/max cannot be enforced if there are multiple CODEC DAIs
	 * connected to a single CPU DAI, use CPU DAI's directly and let
	 * channel allocation be fixed up later
	 */
	if (rtd->num_codecs > 1) {
		chan_min = cpu_stream->channels_min;
		chan_max = cpu_stream->channels_max;
	}

	hw->channels_min = max(chan_min, cpu_stream->channels_min);
	hw->channels_max = min(chan_max, cpu_stream->channels_max);
	if (hw->formats)
		hw->formats &= formats & cpu_stream->formats;
	else
		hw->formats = formats & cpu_stream->formats;
	hw->rates = snd_pcm_rate_mask_intersect(rates, cpu_stream->rates);

	snd_pcm_limit_hw_rates(runtime);

	hw->rate_min = max(hw->rate_min, cpu_stream->rate_min);
	hw->rate_min = max(hw->rate_min, rate_min);
	hw->rate_max = min_not_zero(hw->rate_max, cpu_stream->rate_max);
	hw->rate_max = min_not_zero(hw->rate_max, rate_max);
}

static int soc_pcm_components_close(struct snd_pcm_substream *substream,
				    struct snd_soc_component *last)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_component *component;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (component == last)
			break;

		if (!component->driver->ops ||
		    !component->driver->ops->close)
			continue;

		component->driver->ops->close(substream);
=======
		 */
		if (!snd_soc_dai_stream_valid(codec_dai, stream))
			continue;

		codec_stream = snd_soc_dai_get_pcm_stream(codec_dai, stream);

		soc_pcm_hw_update_chan(hw, codec_stream);
		soc_pcm_hw_update_rate(hw, codec_stream);
		soc_pcm_hw_update_format(hw, codec_stream);
	}

	/* Verify both a valid CPU DAI and a valid CODEC DAI were found */
	if (!hw->channels_min)
		return -EINVAL;

	/*
	 * chan min/max cannot be enforced if there are multiple CODEC DAIs
	 * connected to CPU DAI(s), use CPU DAI's directly and let
	 * channel allocation be fixed up later
	 */
	if (rtd->num_codecs > 1) {
		hw->channels_min = cpu_chan_min;
		hw->channels_max = cpu_chan_max;
>>>>>>> upstream/android-13
	}

	return 0;
}
<<<<<<< HEAD

/*
 * Called by ALSA when a PCM substream is opened, the runtime->hw record is
 * then initialized and any private data can be allocated. This also calls
 * startup for the cpu DAI, component, machine and codec DAI.
 */
static int soc_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai;
	const char *codec_dai_name = "multicodec";
	int i, ret = 0;

	pinctrl_pm_select_default_state(cpu_dai->dev);
	for (i = 0; i < rtd->num_codecs; i++)
		pinctrl_pm_select_default_state(rtd->codec_dais[i]->dev);

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		pm_runtime_get_sync(component->dev);
	}

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (rtd->dai_link->no_host_mode == SND_SOC_DAI_LINK_NO_HOST)
		snd_soc_set_runtime_hwparams(substream, &no_host_hardware);

	/* startup the audio subsystem */
	if (cpu_dai->driver->ops->startup) {
		ret = cpu_dai->driver->ops->startup(substream, cpu_dai);
		if (ret < 0) {
			dev_err(cpu_dai->dev, "ASoC: can't open interface"
				" %s: %d\n", cpu_dai->name, ret);
			goto out;
		}
	}

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->open)
			continue;

		ret = component->driver->ops->open(substream);
		if (ret < 0) {
			dev_err(component->dev,
				"ASoC: can't open component %s: %d\n",
				component->name, ret);
			goto component_err;
		}
	}
	component = NULL;

	for (i = 0; i < rtd->num_codecs; i++) {
		codec_dai = rtd->codec_dais[i];
		if (codec_dai->driver->ops->startup) {
			ret = codec_dai->driver->ops->startup(substream,
							      codec_dai);
			if (ret < 0) {
				dev_err(codec_dai->dev,
					"ASoC: can't open codec %s: %d\n",
					codec_dai->name, ret);
				goto codec_dai_err;
			}
		}

		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			codec_dai->tx_mask = 0;
		else
			codec_dai->rx_mask = 0;
	}

	if (rtd->dai_link->ops->startup) {
		ret = rtd->dai_link->ops->startup(substream);
		if (ret < 0) {
			pr_err("ASoC: %s startup failed: %d\n",
			       rtd->dai_link->name, ret);
			goto machine_err;
		}
	}

	/* Dynamic PCM DAI links compat checks use dynamic capabilities */
	if (rtd->dai_link->dynamic || rtd->dai_link->no_pcm)
		goto dynamic;

	/* Check that the codec and cpu DAIs are compatible */
	soc_pcm_init_runtime_hw(substream);

	if (rtd->num_codecs == 1)
		codec_dai_name = rtd->codec_dai->name;

	if (soc_pcm_has_symmetry(substream))
		runtime->hw.info |= SNDRV_PCM_INFO_JOINT_DUPLEX;

	ret = -EINVAL;
	if (!runtime->hw.rates) {
		printk(KERN_ERR "ASoC: %s <-> %s No matching rates\n",
			codec_dai_name, cpu_dai->name);
		goto config_err;
	}
	if (!runtime->hw.formats) {
		printk(KERN_ERR "ASoC: %s <-> %s No matching formats\n",
			codec_dai_name, cpu_dai->name);
		goto config_err;
	}
	if (!runtime->hw.channels_min || !runtime->hw.channels_max ||
	    runtime->hw.channels_min > runtime->hw.channels_max) {
		printk(KERN_ERR "ASoC: %s <-> %s No matching channels\n",
				codec_dai_name, cpu_dai->name);
		goto config_err;
	}

	soc_pcm_apply_msb(substream);

	/* Symmetry only applies if we've already got an active stream. */
	if (cpu_dai->active) {
		ret = soc_pcm_apply_symmetry(substream, cpu_dai);
		if (ret != 0)
			goto config_err;
	}

	for (i = 0; i < rtd->num_codecs; i++) {
		if (rtd->codec_dais[i]->active) {
			ret = soc_pcm_apply_symmetry(substream,
						     rtd->codec_dais[i]);
			if (ret != 0)
				goto config_err;
		}
	}

	pr_debug("ASoC: %s <-> %s info:\n",
			codec_dai_name, cpu_dai->name);
	pr_debug("ASoC: rate mask 0x%x\n", runtime->hw.rates);
	pr_debug("ASoC: min ch %d max ch %d\n", runtime->hw.channels_min,
		 runtime->hw.channels_max);
	pr_debug("ASoC: min rate %d max rate %d\n", runtime->hw.rate_min,
		 runtime->hw.rate_max);

dynamic:

	snd_soc_runtime_activate(rtd, substream->stream);

	mutex_unlock(&rtd->pcm_mutex);
	return 0;

config_err:
	if (rtd->dai_link->ops->shutdown)
		rtd->dai_link->ops->shutdown(substream);

machine_err:
	i = rtd->num_codecs;

codec_dai_err:
	while (--i >= 0) {
		codec_dai = rtd->codec_dais[i];
		if (codec_dai->driver->ops->shutdown)
			codec_dai->driver->ops->shutdown(substream, codec_dai);
	}

component_err:
	soc_pcm_components_close(substream, component);

	if (cpu_dai->driver->ops->shutdown)
		cpu_dai->driver->ops->shutdown(substream, cpu_dai);
out:
	mutex_unlock(&rtd->pcm_mutex);

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		pm_runtime_mark_last_busy(component->dev);
		pm_runtime_put_autosuspend(component->dev);
	}

	for (i = 0; i < rtd->num_codecs; i++) {
		if (!rtd->codec_dais[i]->active)
			pinctrl_pm_select_sleep_state(rtd->codec_dais[i]->dev);
	}
	if (!cpu_dai->active)
		pinctrl_pm_select_sleep_state(cpu_dai->dev);

	return ret;
}

/*
 * Power down the audio subsystem pmdown_time msecs after close is called.
 * This is to ensure there are no pops or clicks in between any music tracks
 * due to DAPM power cycling.
 */
static void close_delayed_work(struct work_struct *work)
{
	struct snd_soc_pcm_runtime *rtd =
			container_of(work, struct snd_soc_pcm_runtime, delayed_work.work);
	struct snd_soc_dai *codec_dai = rtd->codec_dais[0];

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	dev_dbg(rtd->dev, "ASoC: pop wq checking: %s status: %s waiting: %s\n",
		 codec_dai->driver->playback.stream_name,
		 codec_dai->playback_active ? "active" : "inactive",
		 rtd->pop_wait ? "yes" : "no");

	/* are we waiting on this codec DAI stream */
	if (rtd->pop_wait == 1) {
		rtd->pop_wait = 0;
		snd_soc_dapm_stream_event(rtd, SNDRV_PCM_STREAM_PLAYBACK,
					  SND_SOC_DAPM_STREAM_STOP);
	}

	mutex_unlock(&rtd->pcm_mutex);
=======
EXPORT_SYMBOL_GPL(snd_soc_runtime_calc_hw);

static void soc_pcm_init_runtime_hw(struct snd_pcm_substream *substream)
{
	struct snd_pcm_hardware *hw = &substream->runtime->hw;
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	u64 formats = hw->formats;

	/*
	 * At least one CPU and one CODEC should match. Otherwise, we should
	 * have bailed out on a higher level, since there would be no CPU or
	 * CODEC to support the transfer direction in that case.
	 */
	snd_soc_runtime_calc_hw(rtd, hw, substream->stream);

	if (formats)
		hw->formats &= formats;
}

static int soc_pcm_components_open(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_component *component;
	int i, ret = 0;

	for_each_rtd_components(rtd, i, component) {
		ret = snd_soc_component_module_get_when_open(component, substream);
		if (ret < 0)
			break;

		ret = snd_soc_component_open(component, substream);
		if (ret < 0)
			break;
	}

	return ret;
}

static int soc_pcm_components_close(struct snd_pcm_substream *substream,
				    int rollback)
{
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_component *component;
	int i, ret = 0;

	for_each_rtd_components(rtd, i, component) {
		int r = snd_soc_component_close(component, substream, rollback);
		if (r < 0)
			ret = r; /* use last ret */

		snd_soc_component_module_put_when_close(component, substream, rollback);
	}

	return ret;
}

static int soc_pcm_clean(struct snd_pcm_substream *substream, int rollback)
{
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_component *component;
	struct snd_soc_dai *dai;
	int i;

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	if (!rollback)
		snd_soc_runtime_deactivate(rtd, substream->stream);

	for_each_rtd_dais(rtd, i, dai)
		snd_soc_dai_shutdown(dai, substream, rollback);

	snd_soc_link_shutdown(substream, rollback);

	soc_pcm_components_close(substream, rollback);


	mutex_unlock(&rtd->card->pcm_mutex);

	snd_soc_pcm_component_pm_runtime_put(rtd, substream, rollback);

	for_each_rtd_components(rtd, i, component)
		if (!snd_soc_component_active(component))
			pinctrl_pm_select_sleep_state(component->dev);

	return 0;
>>>>>>> upstream/android-13
}

/*
 * Called by ALSA when a PCM substream is closed. Private data can be
 * freed here. The cpu DAI, codec DAI, machine and components are also
 * shutdown.
 */
static int soc_pcm_close(struct snd_pcm_substream *substream)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai;
	int i;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	snd_soc_runtime_deactivate(rtd, substream->stream);

	/* clear the corresponding DAIs rate when inactive */
	if (!cpu_dai->active)
		cpu_dai->rate = 0;

	for (i = 0; i < rtd->num_codecs; i++) {
		codec_dai = rtd->codec_dais[i];
		if (!codec_dai->active)
			codec_dai->rate = 0;
	}

	snd_soc_dai_digital_mute(cpu_dai, 1, substream->stream);

	if (cpu_dai->driver->ops->shutdown)
		cpu_dai->driver->ops->shutdown(substream, cpu_dai);

	for (i = 0; i < rtd->num_codecs; i++) {
		codec_dai = rtd->codec_dais[i];
		if (codec_dai->driver->ops->shutdown)
			codec_dai->driver->ops->shutdown(substream, codec_dai);
	}

	if (rtd->dai_link->ops->shutdown)
		rtd->dai_link->ops->shutdown(substream);

	soc_pcm_components_close(substream, NULL);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		if (snd_soc_runtime_ignore_pmdown_time(rtd)) {
			/* powered down playback stream now */
			snd_soc_dapm_stream_event(rtd,
						  SNDRV_PCM_STREAM_PLAYBACK,
						  SND_SOC_DAPM_STREAM_STOP);
		} else {
			/* start delayed pop wq here for playback streams */
			rtd->pop_wait = 1;
			queue_delayed_work(system_power_efficient_wq,
					   &rtd->delayed_work,
					   msecs_to_jiffies(rtd->pmdown_time));
		}
	} else {
		/* capture streams can be powered down now */
		snd_soc_dapm_stream_event(rtd, SNDRV_PCM_STREAM_CAPTURE,
					  SND_SOC_DAPM_STREAM_STOP);
	}

	mutex_unlock(&rtd->pcm_mutex);

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		pm_runtime_mark_last_busy(component->dev);
		pm_runtime_put_autosuspend(component->dev);
	}

	for (i = 0; i < rtd->num_codecs; i++) {
		if (!rtd->codec_dais[i]->active)
			pinctrl_pm_select_sleep_state(rtd->codec_dais[i]->dev);
	}
	if (!cpu_dai->active)
		pinctrl_pm_select_sleep_state(cpu_dai->dev);

	return 0;
=======
	return soc_pcm_clean(substream, 0);
}

static int soc_hw_sanity_check(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_pcm_hardware *hw = &substream->runtime->hw;
	const char *name_cpu = soc_cpu_dai_name(rtd);
	const char *name_codec = soc_codec_dai_name(rtd);
	const char *err_msg;
	struct device *dev = rtd->dev;

	err_msg = "rates";
	if (!hw->rates)
		goto config_err;

	err_msg = "formats";
	if (!hw->formats)
		goto config_err;

	err_msg = "channels";
	if (!hw->channels_min || !hw->channels_max ||
	     hw->channels_min  >  hw->channels_max)
		goto config_err;

	dev_dbg(dev, "ASoC: %s <-> %s info:\n",		name_codec,
							name_cpu);
	dev_dbg(dev, "ASoC: rate mask 0x%x\n",		hw->rates);
	dev_dbg(dev, "ASoC: ch   min %d max %d\n",	hw->channels_min,
							hw->channels_max);
	dev_dbg(dev, "ASoC: rate min %d max %d\n",	hw->rate_min,
							hw->rate_max);

	return 0;

config_err:
	dev_err(dev, "ASoC: %s <-> %s No matching %s\n",
		name_codec, name_cpu, err_msg);
	return -EINVAL;
}

/*
 * Called by ALSA when a PCM substream is opened, the runtime->hw record is
 * then initialized and any private data can be allocated. This also calls
 * startup for the cpu DAI, component, machine and codec DAI.
 */
static int soc_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_component *component;
	struct snd_soc_dai *dai;
	int i, ret = 0;

	for_each_rtd_components(rtd, i, component)
		pinctrl_pm_select_default_state(component->dev);

	ret = snd_soc_pcm_component_pm_runtime_get(rtd, substream);
	if (ret < 0)
		goto pm_err;

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	ret = soc_pcm_components_open(substream);
	if (ret < 0)
		goto err;

	ret = snd_soc_link_startup(substream);
	if (ret < 0)
		goto err;

	/* startup the audio subsystem */
	for_each_rtd_dais(rtd, i, dai) {
		ret = snd_soc_dai_startup(dai, substream);
		if (ret < 0)
			goto err;

		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			dai->tx_mask = 0;
		else
			dai->rx_mask = 0;
	}

	/* Dynamic PCM DAI links compat checks use dynamic capabilities */
	if (rtd->dai_link->dynamic || rtd->dai_link->no_pcm)
		goto dynamic;

	/* Check that the codec and cpu DAIs are compatible */
	soc_pcm_init_runtime_hw(substream);

	soc_pcm_update_symmetry(substream);

	ret = soc_hw_sanity_check(substream);
	if (ret < 0)
		goto err;

	soc_pcm_apply_msb(substream);

	/* Symmetry only applies if we've already got an active stream. */
	for_each_rtd_dais(rtd, i, dai) {
		ret = soc_pcm_apply_symmetry(substream, dai);
		if (ret != 0)
			goto err;
	}
dynamic:
	snd_soc_runtime_activate(rtd, substream->stream);
	ret = 0;
err:
	mutex_unlock(&rtd->card->pcm_mutex);
pm_err:
	if (ret < 0) {
		soc_pcm_clean(substream, 1);
		dev_err(rtd->dev, "%s() failed (%d)", __func__, ret);
	}

	return ret;
}

static void codec2codec_close_delayed_work(struct snd_soc_pcm_runtime *rtd)
{
	/*
	 * Currently nothing to do for c2c links
	 * Since c2c links are internal nodes in the DAPM graph and
	 * don't interface with the outside world or application layer
	 * we don't have to do any special handling on close.
	 */
>>>>>>> upstream/android-13
}

/*
 * Called by ALSA when the PCM substream is prepared, can set format, sample
 * rate, etc.  This function is non atomic and can be called multiple times,
 * it can refer to the runtime info.
 */
static int soc_pcm_prepare(struct snd_pcm_substream *substream)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai;
	int i, ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (rtd->dai_link->ops->prepare) {
		ret = rtd->dai_link->ops->prepare(substream);
		if (ret < 0) {
			dev_err(rtd->card->dev, "ASoC: machine prepare error:"
				" %d\n", ret);
			goto out;
		}
	}

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->prepare)
			continue;

		ret = component->driver->ops->prepare(substream);
		if (ret < 0) {
			dev_err(component->dev,
				"ASoC: platform prepare error: %d\n", ret);
			goto out;
		}
	}

	for (i = 0; i < rtd->num_codecs; i++) {
		codec_dai = rtd->codec_dais[i];
		if (codec_dai->driver->ops->prepare) {
			ret = codec_dai->driver->ops->prepare(substream,
							      codec_dai);
			if (ret < 0) {
				dev_err(codec_dai->dev,
					"ASoC: codec DAI prepare error: %d\n",
					ret);
				goto out;
			}
		}
	}

	if (cpu_dai->driver->ops->prepare) {
		ret = cpu_dai->driver->ops->prepare(substream, cpu_dai);
		if (ret < 0) {
			dev_err(cpu_dai->dev,
				"ASoC: cpu DAI prepare error: %d\n", ret);
			goto out;
		}
	}
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *dai;
	int i, ret = 0;

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	ret = snd_soc_link_prepare(substream);
	if (ret < 0)
		goto out;

	ret = snd_soc_pcm_component_prepare(substream);
	if (ret < 0)
		goto out;

	ret = snd_soc_pcm_dai_prepare(substream);
	if (ret < 0)
		goto out;
>>>>>>> upstream/android-13

	/* cancel any delayed stream shutdown that is pending */
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK &&
	    rtd->pop_wait) {
		rtd->pop_wait = 0;
		cancel_delayed_work(&rtd->delayed_work);
	}

	snd_soc_dapm_stream_event(rtd, substream->stream,
			SND_SOC_DAPM_STREAM_START);

<<<<<<< HEAD
	for (i = 0; i < rtd->num_codecs; i++)
		snd_soc_dai_digital_mute(rtd->codec_dais[i], 0,
					 substream->stream);
	snd_soc_dai_digital_mute(cpu_dai, 0, substream->stream);

out:
	mutex_unlock(&rtd->pcm_mutex);
=======
	for_each_rtd_dais(rtd, i, dai)
		snd_soc_dai_digital_mute(dai, 0, substream->stream);

out:
	mutex_unlock(&rtd->card->pcm_mutex);

	if (ret < 0)
		dev_err(rtd->dev, "ASoC: %s() failed (%d)\n", __func__, ret);

>>>>>>> upstream/android-13
	return ret;
}

static void soc_pcm_codec_params_fixup(struct snd_pcm_hw_params *params,
				       unsigned int mask)
{
	struct snd_interval *interval;
	int channels = hweight_long(mask);

	interval = hw_param_interval(params, SNDRV_PCM_HW_PARAM_CHANNELS);
	interval->min = channels;
	interval->max = channels;
}

<<<<<<< HEAD
int soc_dai_hw_params(struct snd_pcm_substream *substream,
		      struct snd_pcm_hw_params *params,
		      struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	int ret;

	/* perform any topology hw_params fixups before DAI  */
	if (rtd->dai_link->be_hw_params_fixup) {
		ret = rtd->dai_link->be_hw_params_fixup(rtd, params);
		if (ret < 0) {
			dev_err(rtd->dev,
				"ASoC: hw_params topology fixup failed %d\n",
				ret);
			return ret;
		}
	}

	if (dai->driver->ops->hw_params) {
		ret = dai->driver->ops->hw_params(substream, params, dai);
		if (ret < 0) {
			dev_err(dai->dev, "ASoC: can't set %s hw params: %d\n",
				dai->name, ret);
			return ret;
		}
	}

	return 0;
}

static int soc_pcm_components_hw_free(struct snd_pcm_substream *substream,
				      struct snd_soc_component *last)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_component *component;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (component == last)
			break;

		if (!component->driver->ops ||
		    !component->driver->ops->hw_free)
			continue;

		component->driver->ops->hw_free(substream);
	}

	return 0;
=======
static int soc_pcm_hw_clean(struct snd_pcm_substream *substream, int rollback)
{
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *dai;
	int i;

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	/* clear the corresponding DAIs parameters when going to be inactive */
	for_each_rtd_dais(rtd, i, dai) {
		int active = snd_soc_dai_stream_active(dai, substream->stream);

		if (snd_soc_dai_active(dai) == 1)
			soc_pcm_set_dai_params(dai, NULL);

		if (active == 1)
			snd_soc_dai_digital_mute(dai, 1, substream->stream);
	}

	/* run the stream event */
	snd_soc_dapm_stream_stop(rtd, substream->stream);

	/* free any machine hw params */
	snd_soc_link_hw_free(substream, rollback);

	/* free any component resources */
	snd_soc_pcm_component_hw_free(substream, rollback);

	/* now free hw params for the DAIs  */
	for_each_rtd_dais(rtd, i, dai) {
		if (!snd_soc_dai_stream_valid(dai, substream->stream))
			continue;

		snd_soc_dai_hw_free(dai, substream, rollback);
	}

	mutex_unlock(&rtd->card->pcm_mutex);
	return 0;
}

/*
 * Frees resources allocated by hw_params, can be called multiple times
 */
static int soc_pcm_hw_free(struct snd_pcm_substream *substream)
{
	return soc_pcm_hw_clean(substream, 0);
>>>>>>> upstream/android-13
}

/*
 * Called by ALSA when the hardware params are set by application. This
 * function can also be called multiple times and can allocate buffers
 * (using snd_pcm_lib_* ). It's non-atomic.
 */
static int soc_pcm_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int i, ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	/* perform any hw_params fixups */
	if ((rtd->dai_link->no_host_mode == SND_SOC_DAI_LINK_NO_HOST) &&
				rtd->dai_link->be_hw_params_fixup) {
		ret = rtd->dai_link->be_hw_params_fixup(rtd,
				params);
		if (ret < 0)
			dev_err(rtd->card->dev, "ASoC: fixup failed for %s\n",
				rtd->dai_link->name);
	}

	if (rtd->dai_link->ops->hw_params) {
		ret = rtd->dai_link->ops->hw_params(substream, params);
		if (ret < 0) {
			dev_err(rtd->card->dev, "ASoC: machine hw_params"
				" failed: %d\n", ret);
			goto out;
		}
	}

	for (i = 0; i < rtd->num_codecs; i++) {
		struct snd_soc_dai *codec_dai = rtd->codec_dais[i];
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai;
	struct snd_soc_dai *codec_dai;
	int i, ret = 0;

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	ret = soc_pcm_params_symmetry(substream, params);
	if (ret)
		goto out;

	ret = snd_soc_link_hw_params(substream, params);
	if (ret < 0)
		goto out;

	for_each_rtd_codec_dais(rtd, i, codec_dai) {
>>>>>>> upstream/android-13
		struct snd_pcm_hw_params codec_params;

		/*
		 * Skip CODECs which don't support the current stream type,
		 * the idea being that if a CODEC is not used for the currently
		 * set up transfer direction, it should not need to be
		 * configured, especially since the configuration used might
		 * not even be supported by that CODEC. There may be cases
		 * however where a CODEC needs to be set up although it is
		 * actually not being used for the transfer, e.g. if a
		 * capture-only CODEC is acting as an LRCLK and/or BCLK master
		 * for the DAI link including a playback-only CODEC.
		 * If this becomes necessary, we will have to augment the
		 * machine driver setup with information on how to act, so
		 * we can do the right thing here.
		 */
		if (!snd_soc_dai_stream_valid(codec_dai, substream->stream))
			continue;

		/* copy params for each codec */
		codec_params = *params;

		/* fixup params based on TDM slot masks */
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK &&
		    codec_dai->tx_mask)
			soc_pcm_codec_params_fixup(&codec_params,
						   codec_dai->tx_mask);

		if (substream->stream == SNDRV_PCM_STREAM_CAPTURE &&
		    codec_dai->rx_mask)
			soc_pcm_codec_params_fixup(&codec_params,
						   codec_dai->rx_mask);

<<<<<<< HEAD
		ret = soc_dai_hw_params(substream, &codec_params, codec_dai);
		if(ret < 0)
			goto codec_err;

		codec_dai->rate = params_rate(&codec_params);
		codec_dai->channels = params_channels(&codec_params);
		codec_dai->sample_bits = snd_pcm_format_physical_width(
						params_format(&codec_params));
	}

	ret = soc_dai_hw_params(substream, params, cpu_dai);
	if (ret < 0)
		goto interface_err;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->hw_params)
			continue;

		ret = component->driver->ops->hw_params(substream, params);
		if (ret < 0) {
			dev_err(component->dev,
				"ASoC: %s hw params failed: %d\n",
				component->name, ret);
			goto component_err;
		}
	}
	component = NULL;

	/* store the parameters for each DAIs */
	cpu_dai->rate = params_rate(params);
	cpu_dai->channels = params_channels(params);
	cpu_dai->sample_bits =
		snd_pcm_format_physical_width(params_format(params));

	ret = soc_pcm_params_symmetry(substream, params);
        if (ret)
		goto component_err;

	/* malloc a page for hostless IO.
	 * FIXME: rework with alsa-lib changes so that this malloc is
	 * not required.
	 */
	if (rtd->dai_link->no_host_mode == SND_SOC_DAI_LINK_NO_HOST) {
		substream->dma_buffer.dev.type = SNDRV_DMA_TYPE_DEV;
		substream->dma_buffer.dev.dev = rtd->dev;
		substream->dma_buffer.dev.dev->coherent_dma_mask =
					DMA_BIT_MASK(sizeof(dma_addr_t) * 8);
		substream->dma_buffer.private_data = NULL;

		arch_setup_dma_ops(substream->dma_buffer.dev.dev,
				   0, 0, NULL, 0);
		ret = snd_pcm_lib_malloc_pages(substream, PAGE_SIZE);
		if (ret < 0)
			goto component_err;
	}
out:
	mutex_unlock(&rtd->pcm_mutex);
	return ret;

component_err:
	soc_pcm_components_hw_free(substream, component);

	if (cpu_dai->driver->ops->hw_free)
		cpu_dai->driver->ops->hw_free(substream, cpu_dai);

interface_err:
	i = rtd->num_codecs;

codec_err:
	while (--i >= 0) {
		struct snd_soc_dai *codec_dai = rtd->codec_dais[i];
		if (codec_dai->driver->ops->hw_free)
			codec_dai->driver->ops->hw_free(substream, codec_dai);
		codec_dai->rate = 0;
	}

	if (rtd->dai_link->ops->hw_free)
		rtd->dai_link->ops->hw_free(substream);

	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

/*
 * Frees resources allocated by hw_params, can be called multiple times
 */
static int soc_pcm_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai;
	bool playback = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;
	int i;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	/* clear the corresponding DAIs parameters when going to be inactive */
	if (cpu_dai->active == 1) {
		cpu_dai->rate = 0;
		cpu_dai->channels = 0;
		cpu_dai->sample_bits = 0;
	}

	for (i = 0; i < rtd->num_codecs; i++) {
		codec_dai = rtd->codec_dais[i];
		if (codec_dai->active == 1) {
			codec_dai->rate = 0;
			codec_dai->channels = 0;
			codec_dai->sample_bits = 0;
		}
	}

	/* apply codec digital mute */
	for (i = 0; i < rtd->num_codecs; i++) {
		if ((playback && rtd->codec_dais[i]->playback_active == 1) ||
		    (!playback && rtd->codec_dais[i]->capture_active == 1))
			snd_soc_dai_digital_mute(rtd->codec_dais[i], 1,
						 substream->stream);
	}

	/* free any machine hw params */
	if (rtd->dai_link->ops->hw_free)
		rtd->dai_link->ops->hw_free(substream);

	/* free any component resources */
	soc_pcm_components_hw_free(substream, NULL);

	/* now free hw params for the DAIs  */
	for (i = 0; i < rtd->num_codecs; i++) {
		codec_dai = rtd->codec_dais[i];
		if (codec_dai->driver->ops->hw_free)
			codec_dai->driver->ops->hw_free(substream, codec_dai);
	}

	if (cpu_dai->driver->ops->hw_free)
		cpu_dai->driver->ops->hw_free(substream, cpu_dai);

	if (rtd->dai_link->no_host_mode == SND_SOC_DAI_LINK_NO_HOST)
		snd_pcm_lib_free_pages(substream);
	mutex_unlock(&rtd->pcm_mutex);
	return 0;
=======
		ret = snd_soc_dai_hw_params(codec_dai, substream,
					    &codec_params);
		if(ret < 0)
			goto out;

		soc_pcm_set_dai_params(codec_dai, &codec_params);
		snd_soc_dapm_update_dai(substream, &codec_params, codec_dai);
	}

	for_each_rtd_cpu_dais(rtd, i, cpu_dai) {
		/*
		 * Skip CPUs which don't support the current stream
		 * type. See soc_pcm_init_runtime_hw() for more details
		 */
		if (!snd_soc_dai_stream_valid(cpu_dai, substream->stream))
			continue;

		ret = snd_soc_dai_hw_params(cpu_dai, substream, params);
		if (ret < 0)
			goto out;

		/* store the parameters for each DAI */
		soc_pcm_set_dai_params(cpu_dai, params);
		snd_soc_dapm_update_dai(substream, params, cpu_dai);
	}

	ret = snd_soc_pcm_component_hw_params(substream, params);
out:
	mutex_unlock(&rtd->card->pcm_mutex);

	if (ret < 0) {
		soc_pcm_hw_clean(substream, 1);
		dev_err(rtd->dev, "ASoC: %s() failed (%d)\n", __func__, ret);
	}

	return ret;
>>>>>>> upstream/android-13
}

static int soc_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai;
	int i, ret;

	for (i = 0; i < rtd->num_codecs; i++) {
		codec_dai = rtd->codec_dais[i];
		if (codec_dai->driver->ops->trigger) {
			ret = codec_dai->driver->ops->trigger(substream,
							      cmd, codec_dai);
			if (ret < 0)
				return ret;
		}
	}

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->trigger)
			continue;

		ret = component->driver->ops->trigger(substream, cmd);
		if (ret < 0)
			return ret;
	}

	if (cpu_dai->driver->ops->trigger) {
		ret = cpu_dai->driver->ops->trigger(substream, cmd, cpu_dai);
		if (ret < 0)
			return ret;
	}

	if (rtd->dai_link->ops->trigger) {
		ret = rtd->dai_link->ops->trigger(substream, cmd);
		if (ret < 0)
			return ret;
	}

	return 0;
}

static int soc_pcm_bespoke_trigger(struct snd_pcm_substream *substream,
				   int cmd)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai;
	int i, ret;

	for (i = 0; i < rtd->num_codecs; i++) {
		codec_dai = rtd->codec_dais[i];
		if (codec_dai->driver->ops->bespoke_trigger) {
			ret = codec_dai->driver->ops->bespoke_trigger(substream,
								cmd, codec_dai);
			if (ret < 0)
				return ret;
		}
	}

	if (cpu_dai->driver->ops->bespoke_trigger) {
		ret = cpu_dai->driver->ops->bespoke_trigger(substream, cmd, cpu_dai);
		if (ret < 0)
			return ret;
	}
	return 0;
}
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	int ret = -EINVAL, _ret = 0;
	int rollback = 0;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		ret = snd_soc_link_trigger(substream, cmd, 0);
		if (ret < 0)
			goto start_err;

		ret = snd_soc_pcm_component_trigger(substream, cmd, 0);
		if (ret < 0)
			goto start_err;

		ret = snd_soc_pcm_dai_trigger(substream, cmd, 0);
start_err:
		if (ret < 0)
			rollback = 1;
	}

	if (rollback) {
		_ret = ret;
		switch (cmd) {
		case SNDRV_PCM_TRIGGER_START:
			cmd = SNDRV_PCM_TRIGGER_STOP;
			break;
		case SNDRV_PCM_TRIGGER_RESUME:
			cmd = SNDRV_PCM_TRIGGER_SUSPEND;
			break;
		case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
			cmd = SNDRV_PCM_TRIGGER_PAUSE_PUSH;
			break;
		}
	}

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		if (rtd->dai_link->stop_dma_first) {
			ret = snd_soc_pcm_component_trigger(substream, cmd, rollback);
			if (ret < 0)
				break;

			ret = snd_soc_pcm_dai_trigger(substream, cmd, rollback);
			if (ret < 0)
				break;
		} else {
			ret = snd_soc_pcm_dai_trigger(substream, cmd, rollback);
			if (ret < 0)
				break;

			ret = snd_soc_pcm_component_trigger(substream, cmd, rollback);
			if (ret < 0)
				break;
		}
		ret = snd_soc_link_trigger(substream, cmd, rollback);
		break;
	}

	if (_ret)
		ret = _ret;

	return ret;
}

>>>>>>> upstream/android-13
/*
 * soc level wrapper for pointer callback
 * If cpu_dai, codec_dai, component driver has the delay callback, then
 * the runtime->delay will be updated accordingly.
 */
static snd_pcm_uframes_t soc_pcm_pointer(struct snd_pcm_substream *substream)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
=======
	struct snd_soc_pcm_runtime *rtd = asoc_substream_to_rtd(substream);
	struct snd_soc_dai *cpu_dai;
>>>>>>> upstream/android-13
	struct snd_soc_dai *codec_dai;
	struct snd_pcm_runtime *runtime = substream->runtime;
	snd_pcm_uframes_t offset = 0;
	snd_pcm_sframes_t delay = 0;
	snd_pcm_sframes_t codec_delay = 0;
<<<<<<< HEAD
=======
	snd_pcm_sframes_t cpu_delay = 0;
>>>>>>> upstream/android-13
	int i;

	/* clearing the previous total delay */
	runtime->delay = 0;

<<<<<<< HEAD
	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->pointer)
			continue;

		/* FIXME: use 1st pointer */
		offset = component->driver->ops->pointer(substream);
		break;
	}
	/* base delay if assigned in pointer callback */
	delay = runtime->delay;

	if (cpu_dai->driver->ops->delay)
		delay += cpu_dai->driver->ops->delay(substream, cpu_dai);

	for (i = 0; i < rtd->num_codecs; i++) {
		codec_dai = rtd->codec_dais[i];
		if (codec_dai->driver->ops->delay)
			codec_delay = max(codec_delay,
					codec_dai->driver->ops->delay(substream,
								    codec_dai));
=======
	offset = snd_soc_pcm_component_pointer(substream);

	/* base delay if assigned in pointer callback */
	delay = runtime->delay;

	for_each_rtd_cpu_dais(rtd, i, cpu_dai) {
		cpu_delay = max(cpu_delay,
				snd_soc_dai_delay(cpu_dai, substream));
	}
	delay += cpu_delay;

	for_each_rtd_codec_dais(rtd, i, codec_dai) {
		codec_delay = max(codec_delay,
				  snd_soc_dai_delay(codec_dai, substream));
>>>>>>> upstream/android-13
	}
	delay += codec_delay;

	runtime->delay = delay;

	return offset;
}

/* connect a FE and BE */
static int dpcm_be_connect(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
	struct snd_soc_dpcm *dpcm;
<<<<<<< HEAD

	/* only add new dpcms */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
=======
	unsigned long flags;

	/* only add new dpcms */
	for_each_dpcm_be(fe, stream, dpcm) {
>>>>>>> upstream/android-13
		if (dpcm->be == be && dpcm->fe == fe)
			return 0;
	}

	dpcm = kzalloc(sizeof(struct snd_soc_dpcm), GFP_KERNEL);
	if (!dpcm)
		return -ENOMEM;

	dpcm->be = be;
	dpcm->fe = fe;
	be->dpcm[stream].runtime = fe->dpcm[stream].runtime;
	dpcm->state = SND_SOC_DPCM_LINK_STATE_NEW;
<<<<<<< HEAD
	list_add(&dpcm->list_be, &fe->dpcm[stream].be_clients);
	list_add(&dpcm->list_fe, &be->dpcm[stream].fe_clients);
=======
	spin_lock_irqsave(&fe->card->dpcm_lock, flags);
	list_add(&dpcm->list_be, &fe->dpcm[stream].be_clients);
	list_add(&dpcm->list_fe, &be->dpcm[stream].fe_clients);
	spin_unlock_irqrestore(&fe->card->dpcm_lock, flags);
>>>>>>> upstream/android-13

	dev_dbg(fe->dev, "connected new DPCM %s path %s %s %s\n",
			stream ? "capture" : "playback",  fe->dai_link->name,
			stream ? "<-" : "->", be->dai_link->name);

<<<<<<< HEAD
#ifdef CONFIG_DEBUG_FS
	if (fe->debugfs_dpcm_root)
		dpcm->debugfs_state = debugfs_create_u32(be->dai_link->name, 0644,
				fe->debugfs_dpcm_root, &dpcm->state);
#endif
=======
	dpcm_create_debugfs_state(dpcm, stream);

>>>>>>> upstream/android-13
	return 1;
}

/* reparent a BE onto another FE */
static void dpcm_be_reparent(struct snd_soc_pcm_runtime *fe,
			struct snd_soc_pcm_runtime *be, int stream)
{
	struct snd_soc_dpcm *dpcm;
	struct snd_pcm_substream *fe_substream, *be_substream;

	/* reparent if BE is connected to other FEs */
	if (!be->dpcm[stream].users)
		return;

	be_substream = snd_soc_dpcm_get_substream(be, stream);

<<<<<<< HEAD
	list_for_each_entry(dpcm, &be->dpcm[stream].fe_clients, list_fe) {
=======
	for_each_dpcm_fe(be, stream, dpcm) {
>>>>>>> upstream/android-13
		if (dpcm->fe == fe)
			continue;

		dev_dbg(fe->dev, "reparent %s path %s %s %s\n",
			stream ? "capture" : "playback",
			dpcm->fe->dai_link->name,
			stream ? "<-" : "->", dpcm->be->dai_link->name);

		fe_substream = snd_soc_dpcm_get_substream(dpcm->fe, stream);
		be_substream->runtime = fe_substream->runtime;
		break;
	}
}

/* disconnect a BE and FE */
void dpcm_be_disconnect(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm, *d;
<<<<<<< HEAD

	list_for_each_entry_safe(dpcm, d, &fe->dpcm[stream].be_clients, list_be) {
=======
	unsigned long flags;

	for_each_dpcm_be_safe(fe, stream, dpcm, d) {
>>>>>>> upstream/android-13
		dev_dbg(fe->dev, "ASoC: BE %s disconnect check for %s\n",
				stream ? "capture" : "playback",
				dpcm->be->dai_link->name);

		if (dpcm->state != SND_SOC_DPCM_LINK_STATE_FREE)
			continue;

		dev_dbg(fe->dev, "freed DSP %s path %s %s %s\n",
			stream ? "capture" : "playback", fe->dai_link->name,
			stream ? "<-" : "->", dpcm->be->dai_link->name);

		/* BEs still alive need new FE */
		dpcm_be_reparent(fe, dpcm->be, stream);

<<<<<<< HEAD
#ifdef CONFIG_DEBUG_FS
		debugfs_remove(dpcm->debugfs_state);
#endif
		list_del(&dpcm->list_be);
		list_del(&dpcm->list_fe);
=======
		dpcm_remove_debugfs_state(dpcm);

		spin_lock_irqsave(&fe->card->dpcm_lock, flags);
		list_del(&dpcm->list_be);
		list_del(&dpcm->list_fe);
		spin_unlock_irqrestore(&fe->card->dpcm_lock, flags);
>>>>>>> upstream/android-13
		kfree(dpcm);
	}
}

/* get BE for DAI widget and stream */
static struct snd_soc_pcm_runtime *dpcm_get_be(struct snd_soc_card *card,
		struct snd_soc_dapm_widget *widget, int stream)
{
	struct snd_soc_pcm_runtime *be;
<<<<<<< HEAD
=======
	struct snd_soc_dapm_widget *w;
	struct snd_soc_dai *dai;
>>>>>>> upstream/android-13
	int i;

	dev_dbg(card->dev, "ASoC: find BE for widget %s\n", widget->name);

<<<<<<< HEAD
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		list_for_each_entry(be, &card->rtd_list, list) {

			if (!be->dai_link->no_pcm)
				continue;

			dev_dbg(card->dev, "ASoC: try BE : %s\n",
				be->cpu_dai->playback_widget ?
				be->cpu_dai->playback_widget->name : "(not set)");

			if (be->cpu_dai->playback_widget == widget)
				return be;

			for (i = 0; i < be->num_codecs; i++) {
				struct snd_soc_dai *dai = be->codec_dais[i];
				if (dai->playback_widget == widget)
					return be;
			}
		}
	} else {

		list_for_each_entry(be, &card->rtd_list, list) {

			if (!be->dai_link->no_pcm)
				continue;

			dev_dbg(card->dev, "ASoC: try BE %s\n",
				be->cpu_dai->capture_widget ?
				be->cpu_dai->capture_widget->name : "(not set)");

			if (be->cpu_dai->capture_widget == widget)
				return be;

			for (i = 0; i < be->num_codecs; i++) {
				struct snd_soc_dai *dai = be->codec_dais[i];
				if (dai->capture_widget == widget)
					return be;
			}
		}
	}

	/* dai link name and stream name set correctly ? */
	dev_err(card->dev, "ASoC: can't get %s BE for %s\n",
		stream ? "capture" : "playback", widget->name);
	return NULL;
}

static inline struct snd_soc_dapm_widget *
	dai_get_widget(struct snd_soc_dai *dai, int stream)
{
	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		return dai->playback_widget;
	else
		return dai->capture_widget;
}

static int widget_in_list(struct snd_soc_dapm_widget_list *list,
		struct snd_soc_dapm_widget *widget)
{
	int i;

	for (i = 0; i < list->num_widgets; i++) {
		if (widget == list->widgets[i])
			return 1;
	}
=======
	for_each_card_rtds(card, be) {

		if (!be->dai_link->no_pcm)
			continue;

		for_each_rtd_dais(be, i, dai) {
			w = snd_soc_dai_get_widget(dai, stream);

			dev_dbg(card->dev, "ASoC: try BE : %s\n",
				w ? w->name : "(not set)");

			if (w == widget)
				return be;
		}
	}

	/* Widget provided is not a BE */
	return NULL;
}

static int widget_in_list(struct snd_soc_dapm_widget_list *list,
		struct snd_soc_dapm_widget *widget)
{
	struct snd_soc_dapm_widget *w;
	int i;

	for_each_dapm_widgets(list, i, w)
		if (widget == w)
			return 1;
>>>>>>> upstream/android-13

	return 0;
}

static bool dpcm_end_walk_at_be(struct snd_soc_dapm_widget *widget,
		enum snd_soc_dapm_direction dir)
{
	struct snd_soc_card *card = widget->dapm->card;
	struct snd_soc_pcm_runtime *rtd;
<<<<<<< HEAD
	int i;

	if (dir == SND_SOC_DAPM_DIR_OUT) {
		list_for_each_entry(rtd, &card->rtd_list, list) {
			if (!rtd->dai_link->no_pcm)
				continue;

			if (rtd->cpu_dai->playback_widget == widget)
				return true;

			for (i = 0; i < rtd->num_codecs; ++i) {
				struct snd_soc_dai *dai = rtd->codec_dais[i];
				if (dai->playback_widget == widget)
					return true;
			}
		}
	} else { /* SND_SOC_DAPM_DIR_IN */
		list_for_each_entry(rtd, &card->rtd_list, list) {
			if (!rtd->dai_link->no_pcm)
				continue;

			if (rtd->cpu_dai->capture_widget == widget)
				return true;

			for (i = 0; i < rtd->num_codecs; ++i) {
				struct snd_soc_dai *dai = rtd->codec_dais[i];
				if (dai->capture_widget == widget)
					return true;
			}
		}
	}
=======
	int stream;

	/* adjust dir to stream */
	if (dir == SND_SOC_DAPM_DIR_OUT)
		stream = SNDRV_PCM_STREAM_PLAYBACK;
	else
		stream = SNDRV_PCM_STREAM_CAPTURE;

	rtd = dpcm_get_be(card, widget, stream);
	if (rtd)
		return true;
>>>>>>> upstream/android-13

	return false;
}

int dpcm_path_get(struct snd_soc_pcm_runtime *fe,
	int stream, struct snd_soc_dapm_widget_list **list)
{
<<<<<<< HEAD
	struct snd_soc_dai *cpu_dai = fe->cpu_dai;
	int paths;

	/* get number of valid DAI paths and their widgets */
	paths = snd_soc_dapm_dai_get_connected_widgets(cpu_dai, stream, list,
			dpcm_end_walk_at_be);

	dev_dbg(fe->dev, "ASoC: found %d audio %s paths\n", paths,
			stream ? "capture" : "playback");
=======
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(fe, 0);
	int paths;

	if (fe->num_cpus > 1) {
		dev_err(fe->dev,
			"%s doesn't support Multi CPU yet\n", __func__);
		return -EINVAL;
	}

	/* get number of valid DAI paths and their widgets */
	paths = snd_soc_dapm_dai_get_connected_widgets(cpu_dai, stream, list,
			fe->card->component_chaining ?
				NULL : dpcm_end_walk_at_be);

	if (paths > 0)
		dev_dbg(fe->dev, "ASoC: found %d audio %s paths\n", paths,
			stream ? "capture" : "playback");
	else if (paths == 0)
		dev_dbg(fe->dev, "ASoC: %s no valid %s path\n", fe->dai_link->name,
			 stream ? "capture" : "playback");
>>>>>>> upstream/android-13

	return paths;
}

<<<<<<< HEAD
static int dpcm_prune_paths(struct snd_soc_pcm_runtime *fe, int stream,
	struct snd_soc_dapm_widget_list **list_)
{
	struct snd_soc_dpcm *dpcm;
	struct snd_soc_dapm_widget_list *list = *list_;
	struct snd_soc_dapm_widget *widget;
	int prune = 0;

	/* Destroy any old FE <--> BE connections */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		unsigned int i;

		/* is there a valid CPU DAI widget for this BE */
		widget = dai_get_widget(dpcm->be->cpu_dai, stream);

		/* prune the BE if it's no longer in our active list */
		if (widget && widget_in_list(list, widget))
			continue;

		/* is there a valid CODEC DAI widget for this BE */
		for (i = 0; i < dpcm->be->num_codecs; i++) {
			struct snd_soc_dai *dai = dpcm->be->codec_dais[i];
			widget = dai_get_widget(dai, stream);

			/* prune the BE if it's no longer in our active list */
			if (widget && widget_in_list(list, widget))
				continue;
		}

=======
void dpcm_path_put(struct snd_soc_dapm_widget_list **list)
{
	snd_soc_dapm_dai_free_widgets(list);
}

static bool dpcm_be_is_active(struct snd_soc_dpcm *dpcm, int stream,
			      struct snd_soc_dapm_widget_list *list)
{
	struct snd_soc_dai *dai;
	unsigned int i;

	/* is there a valid DAI widget for this BE */
	for_each_rtd_dais(dpcm->be, i, dai) {
		struct snd_soc_dapm_widget *widget = snd_soc_dai_get_widget(dai, stream);

		/*
		 * The BE is pruned only if none of the dai
		 * widgets are in the active list.
		 */
		if (widget && widget_in_list(list, widget))
			return true;
	}

	return false;
}

static int dpcm_prune_paths(struct snd_soc_pcm_runtime *fe, int stream,
			    struct snd_soc_dapm_widget_list **list_)
{
	struct snd_soc_dpcm *dpcm;
	int prune = 0;

	/* Destroy any old FE <--> BE connections */
	for_each_dpcm_be(fe, stream, dpcm) {
		if (dpcm_be_is_active(dpcm, stream, *list_))
			continue;

>>>>>>> upstream/android-13
		dev_dbg(fe->dev, "ASoC: pruning %s BE %s for %s\n",
			stream ? "capture" : "playback",
			dpcm->be->dai_link->name, fe->dai_link->name);
		dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;
<<<<<<< HEAD
		dpcm->be->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_BE;
=======
		dpcm_set_be_update_state(dpcm->be, stream, SND_SOC_DPCM_UPDATE_BE);
>>>>>>> upstream/android-13
		prune++;
	}

	dev_dbg(fe->dev, "ASoC: found %d old BE paths for pruning\n", prune);
	return prune;
}

static int dpcm_add_paths(struct snd_soc_pcm_runtime *fe, int stream,
	struct snd_soc_dapm_widget_list **list_)
{
	struct snd_soc_card *card = fe->card;
	struct snd_soc_dapm_widget_list *list = *list_;
	struct snd_soc_pcm_runtime *be;
<<<<<<< HEAD
	int i, new = 0, err;

	/* Create any new FE <--> BE connections */
	for (i = 0; i < list->num_widgets; i++) {

		switch (list->widgets[i]->id) {
=======
	struct snd_soc_dapm_widget *widget;
	int i, new = 0, err;

	/* Create any new FE <--> BE connections */
	for_each_dapm_widgets(list, i, widget) {

		switch (widget->id) {
>>>>>>> upstream/android-13
		case snd_soc_dapm_dai_in:
			if (stream != SNDRV_PCM_STREAM_PLAYBACK)
				continue;
			break;
		case snd_soc_dapm_dai_out:
			if (stream != SNDRV_PCM_STREAM_CAPTURE)
				continue;
			break;
		default:
			continue;
		}

		/* is there a valid BE rtd for this widget */
<<<<<<< HEAD
		be = dpcm_get_be(card, list->widgets[i], stream);
		if (!be) {
			dev_err(fe->dev, "ASoC: no BE found for %s\n",
					list->widgets[i]->name);
			continue;
		}

		/* make sure BE is a real BE */
		if (!be->dai_link->no_pcm)
			continue;

=======
		be = dpcm_get_be(card, widget, stream);
		if (!be) {
			dev_dbg(fe->dev, "ASoC: no BE found for %s\n",
				widget->name);
			continue;
		}

>>>>>>> upstream/android-13
		/* don't connect if FE is not running */
		if (!fe->dpcm[stream].runtime && !fe->fe_compr)
			continue;

		/* newly connected FE and BE */
		err = dpcm_be_connect(fe, be, stream);
		if (err < 0) {
			dev_err(fe->dev, "ASoC: can't connect %s\n",
<<<<<<< HEAD
				list->widgets[i]->name);
=======
				widget->name);
>>>>>>> upstream/android-13
			break;
		} else if (err == 0) /* already connected */
			continue;

		/* new */
<<<<<<< HEAD
		be->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_BE;
=======
		dpcm_set_be_update_state(be, stream, SND_SOC_DPCM_UPDATE_BE);
>>>>>>> upstream/android-13
		new++;
	}

	dev_dbg(fe->dev, "ASoC: found %d new BE paths\n", new);
	return new;
}

/*
 * Find the corresponding BE DAIs that source or sink audio to this
 * FE substream.
 */
int dpcm_process_paths(struct snd_soc_pcm_runtime *fe,
	int stream, struct snd_soc_dapm_widget_list **list, int new)
{
	if (new)
		return dpcm_add_paths(fe, stream, list);
	else
		return dpcm_prune_paths(fe, stream, list);
}

void dpcm_clear_pending_state(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm;
<<<<<<< HEAD

	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be)
		dpcm->be->dpcm[stream].runtime_update =
						SND_SOC_DPCM_UPDATE_NO;
}

static void dpcm_be_dai_startup_unwind(struct snd_soc_pcm_runtime *fe,
	int stream)
=======
	unsigned long flags;

	spin_lock_irqsave(&fe->card->dpcm_lock, flags);
	for_each_dpcm_be(fe, stream, dpcm)
		dpcm_set_be_update_state(dpcm->be, stream, SND_SOC_DPCM_UPDATE_NO);
	spin_unlock_irqrestore(&fe->card->dpcm_lock, flags);
}

void dpcm_be_dai_stop(struct snd_soc_pcm_runtime *fe, int stream,
		      int do_hw_free, struct snd_soc_dpcm *last)
>>>>>>> upstream/android-13
{
	struct snd_soc_dpcm *dpcm;

	/* disable any enabled and non active backends */
<<<<<<< HEAD
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

=======
	for_each_dpcm_be(fe, stream, dpcm) {
>>>>>>> upstream/android-13
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

<<<<<<< HEAD
		if (be->dpcm[stream].users == 0)
			dev_err(be->dev, "ASoC: no users %s at close - state %d\n",
				stream ? "capture" : "playback",
				be->dpcm[stream].state);
=======
		if (dpcm == last)
			return;

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		if (be->dpcm[stream].users == 0) {
			dev_err(be->dev, "ASoC: no users %s at close - state %d\n",
				stream ? "capture" : "playback",
				be->dpcm[stream].state);
			continue;
		}
>>>>>>> upstream/android-13

		if (--be->dpcm[stream].users != 0)
			continue;

<<<<<<< HEAD
		if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN)
			continue;
=======
		if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN) {
			if (!do_hw_free)
				continue;

			if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE) {
				soc_pcm_hw_free(be_substream);
				be->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_FREE;
			}
		}
>>>>>>> upstream/android-13

		soc_pcm_close(be_substream);
		be_substream->runtime = NULL;
		be->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	}
}

int dpcm_be_dai_startup(struct snd_soc_pcm_runtime *fe, int stream)
{
<<<<<<< HEAD
=======
	struct snd_soc_pcm_runtime *be;
>>>>>>> upstream/android-13
	struct snd_soc_dpcm *dpcm;
	int err, count = 0;

	/* only startup BE DAIs that are either sinks or sources to this FE DAI */
<<<<<<< HEAD
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);
=======
	for_each_dpcm_be(fe, stream, dpcm) {
		struct snd_pcm_substream *be_substream;

		be = dpcm->be;
		be_substream = snd_soc_dpcm_get_substream(be, stream);
>>>>>>> upstream/android-13

		if (!be_substream) {
			dev_err(be->dev, "ASoC: no backend %s stream\n",
				stream ? "capture" : "playback");
			continue;
		}

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		/* first time the dpcm is open ? */
<<<<<<< HEAD
		if (be->dpcm[stream].users == DPCM_MAX_BE_USERS)
			dev_err(be->dev, "ASoC: too many users %s at open %d\n",
				stream ? "capture" : "playback",
				be->dpcm[stream].state);
=======
		if (be->dpcm[stream].users == DPCM_MAX_BE_USERS) {
			dev_err(be->dev, "ASoC: too many users %s at open %d\n",
				stream ? "capture" : "playback",
				be->dpcm[stream].state);
			continue;
		}
>>>>>>> upstream/android-13

		if (be->dpcm[stream].users++ != 0)
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_NEW) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_CLOSE))
			continue;

		dev_dbg(be->dev, "ASoC: open %s BE %s\n",
			stream ? "capture" : "playback", be->dai_link->name);

		be_substream->runtime = be->dpcm[stream].runtime;
		err = soc_pcm_open(be_substream);
		if (err < 0) {
<<<<<<< HEAD
			dev_err(be->dev, "ASoC: BE open failed %d\n", err);
=======
>>>>>>> upstream/android-13
			be->dpcm[stream].users--;
			if (be->dpcm[stream].users < 0)
				dev_err(be->dev, "ASoC: no users %s at unwind %d\n",
					stream ? "capture" : "playback",
					be->dpcm[stream].state);

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
			goto unwind;
		}

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_OPEN;
		count++;
	}

	return count;

unwind:
<<<<<<< HEAD
	/* disable any enabled and non active backends */
	list_for_each_entry_continue_reverse(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		if (be->dpcm[stream].users == 0)
			dev_err(be->dev, "ASoC: no users %s at close %d\n",
				stream ? "capture" : "playback",
				be->dpcm[stream].state);

		if (--be->dpcm[stream].users != 0)
			continue;

		if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN)
			continue;

		soc_pcm_close(be_substream);
		be_substream->runtime = NULL;
		be->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	}
=======
	dpcm_be_dai_startup_rollback(fe, stream, dpcm);

	dev_err(fe->dev, "ASoC: %s() failed at %s (%d)\n",
		__func__, be->dai_link->name, err);
>>>>>>> upstream/android-13

	return err;
}

<<<<<<< HEAD
static void dpcm_init_runtime_hw(struct snd_pcm_runtime *runtime,
				 struct snd_soc_pcm_stream *stream)
{
	runtime->hw.rate_min = stream->rate_min;
	runtime->hw.rate_max = min_not_zero(stream->rate_max, UINT_MAX);
	runtime->hw.channels_min = stream->channels_min;
	runtime->hw.channels_max = stream->channels_max;
	if (runtime->hw.formats)
		runtime->hw.formats &= stream->formats;
	else
		runtime->hw.formats = stream->formats;
	runtime->hw.rates = stream->rates;
}

static void dpcm_runtime_merge_format(struct snd_pcm_substream *substream,
				      u64 *formats)
{
	struct snd_soc_pcm_runtime *fe = substream->private_data;
	struct snd_soc_dpcm *dpcm;
=======
static void dpcm_runtime_setup_fe(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_pcm_hardware *hw = &runtime->hw;
	struct snd_soc_dai *dai;
	int stream = substream->stream;
	int i;

	soc_pcm_hw_init(hw);

	for_each_rtd_cpu_dais(fe, i, dai) {
		struct snd_soc_pcm_stream *cpu_stream;

		/*
		 * Skip CPUs which don't support the current stream
		 * type. See soc_pcm_init_runtime_hw() for more details
		 */
		if (!snd_soc_dai_stream_valid(dai, stream))
			continue;

		cpu_stream = snd_soc_dai_get_pcm_stream(dai, stream);

		soc_pcm_hw_update_rate(hw, cpu_stream);
		soc_pcm_hw_update_chan(hw, cpu_stream);
		soc_pcm_hw_update_format(hw, cpu_stream);
	}

}

static void dpcm_runtime_setup_be_format(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_pcm_hardware *hw = &runtime->hw;
	struct snd_soc_dpcm *dpcm;
	struct snd_soc_dai *dai;
>>>>>>> upstream/android-13
	int stream = substream->stream;

	if (!fe->dai_link->dpcm_merged_format)
		return;

	/*
	 * It returns merged BE codec format
	 * if FE want to use it (= dpcm_merged_format)
	 */

<<<<<<< HEAD
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_soc_dai_driver *codec_dai_drv;
		struct snd_soc_pcm_stream *codec_stream;
		int i;

		for (i = 0; i < be->num_codecs; i++) {
=======
	for_each_dpcm_be(fe, stream, dpcm) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_soc_pcm_stream *codec_stream;
		int i;

		for_each_rtd_codec_dais(be, i, dai) {
>>>>>>> upstream/android-13
			/*
			 * Skip CODECs which don't support the current stream
			 * type. See soc_pcm_init_runtime_hw() for more details
			 */
<<<<<<< HEAD
			if (!snd_soc_dai_stream_valid(be->codec_dais[i],
						      stream))
				continue;

			codec_dai_drv = be->codec_dais[i]->driver;
			if (stream == SNDRV_PCM_STREAM_PLAYBACK)
				codec_stream = &codec_dai_drv->playback;
			else
				codec_stream = &codec_dai_drv->capture;

			*formats &= codec_stream->formats;
=======
			if (!snd_soc_dai_stream_valid(dai, stream))
				continue;

			codec_stream = snd_soc_dai_get_pcm_stream(dai, stream);

			soc_pcm_hw_update_format(hw, codec_stream);
>>>>>>> upstream/android-13
		}
	}
}

<<<<<<< HEAD
static void dpcm_runtime_merge_chan(struct snd_pcm_substream *substream,
				    unsigned int *channels_min,
				    unsigned int *channels_max)
{
	struct snd_soc_pcm_runtime *fe = substream->private_data;
=======
static void dpcm_runtime_setup_be_chan(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_pcm_hardware *hw = &runtime->hw;
>>>>>>> upstream/android-13
	struct snd_soc_dpcm *dpcm;
	int stream = substream->stream;

	if (!fe->dai_link->dpcm_merged_chan)
		return;

	/*
	 * It returns merged BE codec channel;
	 * if FE want to use it (= dpcm_merged_chan)
	 */

<<<<<<< HEAD
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_soc_dai_driver *cpu_dai_drv =  be->cpu_dai->driver;
		struct snd_soc_dai_driver *codec_dai_drv;
		struct snd_soc_pcm_stream *codec_stream;
		struct snd_soc_pcm_stream *cpu_stream;

		if (stream == SNDRV_PCM_STREAM_PLAYBACK)
			cpu_stream = &cpu_dai_drv->playback;
		else
			cpu_stream = &cpu_dai_drv->capture;

		*channels_min = max(*channels_min, cpu_stream->channels_min);
		*channels_max = min(*channels_max, cpu_stream->channels_max);
=======
	for_each_dpcm_be(fe, stream, dpcm) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_soc_pcm_stream *cpu_stream;
		struct snd_soc_dai *dai;
		int i;

		for_each_rtd_cpu_dais(be, i, dai) {
			/*
			 * Skip CPUs which don't support the current stream
			 * type. See soc_pcm_init_runtime_hw() for more details
			 */
			if (!snd_soc_dai_stream_valid(dai, stream))
				continue;

			cpu_stream = snd_soc_dai_get_pcm_stream(dai, stream);

			soc_pcm_hw_update_chan(hw, cpu_stream);
		}
>>>>>>> upstream/android-13

		/*
		 * chan min/max cannot be enforced if there are multiple CODEC
		 * DAIs connected to a single CPU DAI, use CPU DAI's directly
		 */
		if (be->num_codecs == 1) {
<<<<<<< HEAD
			codec_dai_drv = be->codec_dais[0]->driver;

			if (stream == SNDRV_PCM_STREAM_PLAYBACK)
				codec_stream = &codec_dai_drv->playback;
			else
				codec_stream = &codec_dai_drv->capture;

			*channels_min = max(*channels_min,
					    codec_stream->channels_min);
			*channels_max = min(*channels_max,
					    codec_stream->channels_max);
=======
			struct snd_soc_pcm_stream *codec_stream = snd_soc_dai_get_pcm_stream(
				asoc_rtd_to_codec(be, 0), stream);

			soc_pcm_hw_update_chan(hw, codec_stream);
>>>>>>> upstream/android-13
		}
	}
}

<<<<<<< HEAD
static void dpcm_runtime_merge_rate(struct snd_pcm_substream *substream,
				    unsigned int *rates,
				    unsigned int *rate_min,
				    unsigned int *rate_max)
{
	struct snd_soc_pcm_runtime *fe = substream->private_data;
=======
static void dpcm_runtime_setup_be_rate(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_pcm_hardware *hw = &runtime->hw;
>>>>>>> upstream/android-13
	struct snd_soc_dpcm *dpcm;
	int stream = substream->stream;

	if (!fe->dai_link->dpcm_merged_rate)
		return;

	/*
	 * It returns merged BE codec channel;
	 * if FE want to use it (= dpcm_merged_chan)
	 */

<<<<<<< HEAD
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_soc_dai_driver *cpu_dai_drv =  be->cpu_dai->driver;
		struct snd_soc_dai_driver *codec_dai_drv;
		struct snd_soc_pcm_stream *codec_stream;
		struct snd_soc_pcm_stream *cpu_stream;
		int i;

		if (stream == SNDRV_PCM_STREAM_PLAYBACK)
			cpu_stream = &cpu_dai_drv->playback;
		else
			cpu_stream = &cpu_dai_drv->capture;

		*rate_min = max(*rate_min, cpu_stream->rate_min);
		*rate_max = min_not_zero(*rate_max, cpu_stream->rate_max);
		*rates = snd_pcm_rate_mask_intersect(*rates, cpu_stream->rates);

		for (i = 0; i < be->num_codecs; i++) {
			/*
			 * Skip CODECs which don't support the current stream
			 * type. See soc_pcm_init_runtime_hw() for more details
			 */
			if (!snd_soc_dai_stream_valid(be->codec_dais[i],
						      stream))
				continue;

			codec_dai_drv = be->codec_dais[i]->driver;
			if (stream == SNDRV_PCM_STREAM_PLAYBACK)
				codec_stream = &codec_dai_drv->playback;
			else
				codec_stream = &codec_dai_drv->capture;

			*rate_min = max(*rate_min, codec_stream->rate_min);
			*rate_max = min_not_zero(*rate_max,
						 codec_stream->rate_max);
			*rates = snd_pcm_rate_mask_intersect(*rates,
						codec_stream->rates);
=======
	for_each_dpcm_be(fe, stream, dpcm) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_soc_pcm_stream *pcm;
		struct snd_soc_dai *dai;
		int i;

		for_each_rtd_dais(be, i, dai) {
			/*
			 * Skip DAIs which don't support the current stream
			 * type. See soc_pcm_init_runtime_hw() for more details
			 */
			if (!snd_soc_dai_stream_valid(dai, stream))
				continue;

			pcm = snd_soc_dai_get_pcm_stream(dai, stream);

			soc_pcm_hw_update_rate(hw, pcm);
>>>>>>> upstream/android-13
		}
	}
}

<<<<<<< HEAD
static void dpcm_set_fe_runtime(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai_driver *cpu_dai_drv = cpu_dai->driver;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dpcm_init_runtime_hw(runtime, &cpu_dai_drv->playback);
	else
		dpcm_init_runtime_hw(runtime, &cpu_dai_drv->capture);

	dpcm_runtime_merge_format(substream, &runtime->hw.formats);
	dpcm_runtime_merge_chan(substream, &runtime->hw.channels_min,
				&runtime->hw.channels_max);
	dpcm_runtime_merge_rate(substream, &runtime->hw.rates,
				&runtime->hw.rate_min, &runtime->hw.rate_max);
}

static int dpcm_fe_dai_do_trigger(struct snd_pcm_substream *substream, int cmd);

/* Set FE's runtime_update state; the state is protected via PCM stream lock
 * for avoiding the race with trigger callback.
 * If the state is unset and a trigger is pending while the previous operation,
 * process the pending trigger action here.
 */
static void dpcm_set_fe_update_state(struct snd_soc_pcm_runtime *fe,
				     int stream, enum snd_soc_dpcm_update state)
{
	struct snd_pcm_substream *substream =
		snd_soc_dpcm_get_substream(fe, stream);

	snd_pcm_stream_lock_irq(substream);
	if (state == SND_SOC_DPCM_UPDATE_NO && fe->dpcm[stream].trigger_pending) {
		dpcm_fe_dai_do_trigger(substream,
				       fe->dpcm[stream].trigger_pending - 1);
		fe->dpcm[stream].trigger_pending = 0;
	}
	fe->dpcm[stream].runtime_update = state;
	snd_pcm_stream_unlock_irq(substream);
}

=======
>>>>>>> upstream/android-13
static int dpcm_apply_symmetry(struct snd_pcm_substream *fe_substream,
			       int stream)
{
	struct snd_soc_dpcm *dpcm;
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *fe = fe_substream->private_data;
	struct snd_soc_dai *fe_cpu_dai = fe->cpu_dai;
	int err;

	/* apply symmetry for FE */
	if (soc_pcm_has_symmetry(fe_substream))
		fe_substream->runtime->hw.info |= SNDRV_PCM_INFO_JOINT_DUPLEX;

	/* Symmetry only applies if we've got an active stream. */
	if (fe_cpu_dai->active) {
		err = soc_pcm_apply_symmetry(fe_substream, fe_cpu_dai);
		if (err < 0)
			return err;
	}

	/* apply symmetry for BE */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);
		struct snd_soc_pcm_runtime *rtd = be_substream->private_data;
		int i;

		if (rtd->dai_link->be_hw_params_fixup)
			continue;

		if (soc_pcm_has_symmetry(be_substream))
			be_substream->runtime->hw.info |= SNDRV_PCM_INFO_JOINT_DUPLEX;

		/* Symmetry only applies if we've got an active stream. */
		if (rtd->cpu_dai->active) {
			err = soc_pcm_apply_symmetry(fe_substream,
						     rtd->cpu_dai);
			if (err < 0)
				return err;
		}

		for (i = 0; i < rtd->num_codecs; i++) {
			if (rtd->codec_dais[i]->active) {
				err = soc_pcm_apply_symmetry(fe_substream,
							     rtd->codec_dais[i]);
				if (err < 0)
					return err;
			}
		}
	}

	return 0;
=======
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(fe_substream);
	struct snd_soc_dai *fe_cpu_dai;
	int err = 0;
	int i;

	/* apply symmetry for FE */
	soc_pcm_update_symmetry(fe_substream);

	for_each_rtd_cpu_dais (fe, i, fe_cpu_dai) {
		/* Symmetry only applies if we've got an active stream. */
		err = soc_pcm_apply_symmetry(fe_substream, fe_cpu_dai);
		if (err < 0)
			goto error;
	}

	/* apply symmetry for BE */
	for_each_dpcm_be(fe, stream, dpcm) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);
		struct snd_soc_pcm_runtime *rtd;
		struct snd_soc_dai *dai;

		/* A backend may not have the requested substream */
		if (!be_substream)
			continue;

		rtd = asoc_substream_to_rtd(be_substream);
		if (rtd->dai_link->be_hw_params_fixup)
			continue;

		soc_pcm_update_symmetry(be_substream);

		/* Symmetry only applies if we've got an active stream. */
		for_each_rtd_dais(rtd, i, dai) {
			err = soc_pcm_apply_symmetry(fe_substream, dai);
			if (err < 0)
				goto error;
		}
	}
error:
	if (err < 0)
		dev_err(fe->dev, "ASoC: %s failed (%d)\n", __func__, err);

	return err;
>>>>>>> upstream/android-13
}

static int dpcm_fe_dai_startup(struct snd_pcm_substream *fe_substream)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *fe = fe_substream->private_data;
	struct snd_pcm_runtime *runtime = fe_substream->runtime;
=======
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(fe_substream);
>>>>>>> upstream/android-13
	int stream = fe_substream->stream, ret = 0;

	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_FE);

<<<<<<< HEAD
	ret = dpcm_be_dai_startup(fe, fe_substream->stream);
	if (ret < 0) {
		dev_err(fe->dev,"ASoC: failed to start some BEs %d\n", ret);
		goto be_err;
	}
=======
	ret = dpcm_be_dai_startup(fe, stream);
	if (ret < 0)
		goto be_err;
>>>>>>> upstream/android-13

	dev_dbg(fe->dev, "ASoC: open FE %s\n", fe->dai_link->name);

	/* start the DAI frontend */
	ret = soc_pcm_open(fe_substream);
<<<<<<< HEAD
	if (ret < 0) {
		dev_err(fe->dev,"ASoC: failed to start FE %d\n", ret);
		goto unwind;
	}

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_OPEN;

	dpcm_set_fe_runtime(fe_substream);
	snd_pcm_limit_hw_rates(runtime);

	ret = dpcm_apply_symmetry(fe_substream, stream);
	if (ret < 0) {
		dev_err(fe->dev, "ASoC: failed to apply dpcm symmetry %d\n",
			ret);
		goto unwind;
	}

	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_NO);
	return 0;

unwind:
	dpcm_be_dai_startup_unwind(fe, fe_substream->stream);
be_err:
	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_NO);
	return ret;
}

int dpcm_be_dai_shutdown(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm;

	/* only shutdown BEs that are either sinks or sources to this FE DAI */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		if (be->dpcm[stream].users == 0)
			dev_err(be->dev, "ASoC: no users %s at close - state %d\n",
				stream ? "capture" : "playback",
				be->dpcm[stream].state);

		if (--be->dpcm[stream].users != 0)
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN)) {
			soc_pcm_hw_free(be_substream);
			be->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_FREE;
		}

		dev_dbg(be->dev, "ASoC: close BE %s\n",
			be->dai_link->name);

		soc_pcm_close(be_substream);
		be_substream->runtime = NULL;

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	}
	return 0;
}

static int dpcm_fe_dai_shutdown(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *fe = substream->private_data;
=======
	if (ret < 0)
		goto unwind;

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_OPEN;

	dpcm_runtime_setup_fe(fe_substream);

	dpcm_runtime_setup_be_format(fe_substream);
	dpcm_runtime_setup_be_chan(fe_substream);
	dpcm_runtime_setup_be_rate(fe_substream);

	ret = dpcm_apply_symmetry(fe_substream, stream);

unwind:
	if (ret < 0)
		dpcm_be_dai_startup_unwind(fe, stream);
be_err:
	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_NO);

	if (ret < 0)
		dev_err(fe->dev, "%s() failed (%d)\n", __func__, ret);

	return ret;
}

static int dpcm_fe_dai_shutdown(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
>>>>>>> upstream/android-13
	int stream = substream->stream;

	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_FE);

	/* shutdown the BEs */
<<<<<<< HEAD
	dpcm_be_dai_shutdown(fe, substream->stream);
=======
	dpcm_be_dai_shutdown(fe, stream);
>>>>>>> upstream/android-13

	dev_dbg(fe->dev, "ASoC: close FE %s\n", fe->dai_link->name);

	/* now shutdown the frontend */
	soc_pcm_close(substream);

<<<<<<< HEAD
	/* run the stream event for each BE */
=======
	/* run the stream stop event */
>>>>>>> upstream/android-13
	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_STOP);

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_NO);
	return 0;
}

<<<<<<< HEAD
int dpcm_be_dai_hw_free(struct snd_soc_pcm_runtime *fe, int stream)
=======
void dpcm_be_dai_hw_free(struct snd_soc_pcm_runtime *fe, int stream)
>>>>>>> upstream/android-13
{
	struct snd_soc_dpcm *dpcm;

	/* only hw_params backends that are either sinks or sources
	 * to this frontend DAI */
<<<<<<< HEAD
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
=======
	for_each_dpcm_be(fe, stream, dpcm) {
>>>>>>> upstream/android-13

		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		/* only free hw when no longer used - check all FEs */
		if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
				continue;

		/* do not free hw if this BE is used by other FE */
		if (be->dpcm[stream].users > 1)
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_PREPARE) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_PAUSED) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_SUSPEND))
			continue;

		dev_dbg(be->dev, "ASoC: hw_free BE %s\n",
			be->dai_link->name);

		soc_pcm_hw_free(be_substream);

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_FREE;
	}
<<<<<<< HEAD

	return 0;
=======
>>>>>>> upstream/android-13
}

static int dpcm_fe_dai_hw_free(struct snd_pcm_substream *substream)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *fe = substream->private_data;
	int err, stream = substream->stream;
=======
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
	int stream = substream->stream;
>>>>>>> upstream/android-13

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_FE);

	dev_dbg(fe->dev, "ASoC: hw_free FE %s\n", fe->dai_link->name);

	/* call hw_free on the frontend */
<<<<<<< HEAD
	err = soc_pcm_hw_free(substream);
	if (err < 0)
		dev_err(fe->dev,"ASoC: hw_free FE %s failed\n",
			fe->dai_link->name);

	/* only hw_params backends that are either sinks or sources
	 * to this frontend DAI */
	err = dpcm_be_dai_hw_free(fe, stream);
=======
	soc_pcm_hw_free(substream);

	/* only hw_params backends that are either sinks or sources
	 * to this frontend DAI */
	dpcm_be_dai_hw_free(fe, stream);
>>>>>>> upstream/android-13

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_FREE;
	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_NO);

	mutex_unlock(&fe->card->mutex);
	return 0;
}

int dpcm_be_dai_hw_params(struct snd_soc_pcm_runtime *fe, int stream)
{
<<<<<<< HEAD
	struct snd_soc_dpcm *dpcm;
	int ret;

	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);
=======
	struct snd_soc_pcm_runtime *be;
	struct snd_pcm_substream *be_substream;
	struct snd_soc_dpcm *dpcm;
	int ret;

	for_each_dpcm_be(fe, stream, dpcm) {
		be = dpcm->be;
		be_substream = snd_soc_dpcm_get_substream(be, stream);
>>>>>>> upstream/android-13

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		/* copy params for each dpcm */
		memcpy(&dpcm->hw_params, &fe->dpcm[stream].hw_params,
				sizeof(struct snd_pcm_hw_params));

		/* perform any hw_params fixups */
<<<<<<< HEAD
		if (be->dai_link->be_hw_params_fixup) {
			ret = be->dai_link->be_hw_params_fixup(be,
					&dpcm->hw_params);
			if (ret < 0) {
				dev_err(be->dev,
					"ASoC: hw_params BE fixup failed %d\n",
					ret);
				goto unwind;
			}
		}
=======
		ret = snd_soc_link_be_hw_params_fixup(be, &dpcm->hw_params);
		if (ret < 0)
			goto unwind;

		/* copy the fixed-up hw params for BE dai */
		memcpy(&be->dpcm[stream].hw_params, &dpcm->hw_params,
		       sizeof(struct snd_pcm_hw_params));
>>>>>>> upstream/android-13

		/* only allow hw_params() if no connected FEs are running */
		if (!snd_soc_dpcm_can_be_params(fe, be, stream))
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE))
			continue;

		dev_dbg(be->dev, "ASoC: hw_params BE %s\n",
			be->dai_link->name);

		ret = soc_pcm_hw_params(be_substream, &dpcm->hw_params);
<<<<<<< HEAD
		if (ret < 0) {
			dev_err(dpcm->be->dev,
				"ASoC: hw_params BE failed %d\n", ret);
			goto unwind;
		}
=======
		if (ret < 0)
			goto unwind;
>>>>>>> upstream/android-13

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_PARAMS;
	}
	return 0;

unwind:
<<<<<<< HEAD
	/* disable any enabled and non active backends */
	list_for_each_entry_continue_reverse(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);
=======
	dev_dbg(fe->dev, "ASoC: %s() failed at %s (%d)\n",
		__func__, be->dai_link->name, ret);

	/* disable any enabled and non active backends */
	for_each_dpcm_be_rollback(fe, stream, dpcm) {
		be = dpcm->be;
		be_substream = snd_soc_dpcm_get_substream(be, stream);
>>>>>>> upstream/android-13

		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		/* only allow hw_free() if no connected FEs are running */
		if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN) &&
		   (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		   (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE) &&
		   (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP))
			continue;

		soc_pcm_hw_free(be_substream);
	}

	return ret;
}

static int dpcm_fe_dai_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *params)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *fe = substream->private_data;
=======
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
>>>>>>> upstream/android-13
	int ret, stream = substream->stream;

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_FE);

<<<<<<< HEAD
	memcpy(&fe->dpcm[substream->stream].hw_params, params,
			sizeof(struct snd_pcm_hw_params));
	ret = dpcm_be_dai_hw_params(fe, substream->stream);
	if (ret < 0) {
		dev_err(fe->dev,"ASoC: hw_params BE failed %d\n", ret);
		goto out;
	}
=======
	memcpy(&fe->dpcm[stream].hw_params, params,
			sizeof(struct snd_pcm_hw_params));
	ret = dpcm_be_dai_hw_params(fe, stream);
	if (ret < 0)
		goto out;
>>>>>>> upstream/android-13

	dev_dbg(fe->dev, "ASoC: hw_params FE %s rate %d chan %x fmt %d\n",
			fe->dai_link->name, params_rate(params),
			params_channels(params), params_format(params));

	/* call hw_params on the frontend */
	ret = soc_pcm_hw_params(substream, params);
<<<<<<< HEAD
	if (ret < 0) {
		dev_err(fe->dev,"ASoC: hw_params FE failed %d\n", ret);
		dpcm_be_dai_hw_free(fe, stream);
	 } else
=======
	if (ret < 0)
		dpcm_be_dai_hw_free(fe, stream);
	else
>>>>>>> upstream/android-13
		fe->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_PARAMS;

out:
	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_NO);
	mutex_unlock(&fe->card->mutex);
<<<<<<< HEAD
	return ret;
}

static int dpcm_do_trigger(struct snd_soc_dpcm *dpcm,
		struct snd_pcm_substream *substream, int cmd)
{
	int ret;

	dev_dbg(dpcm->be->dev, "ASoC: trigger BE %s cmd %d\n",
			dpcm->be->dai_link->name, cmd);

	ret = soc_pcm_trigger(substream, cmd);
	if (ret < 0)
		dev_err(dpcm->be->dev,"ASoC: trigger BE failed %d\n", ret);
=======

	if (ret < 0)
		dev_err(fe->dev, "ASoC: %s failed (%d)\n", __func__, ret);
>>>>>>> upstream/android-13

	return ret;
}

int dpcm_be_dai_trigger(struct snd_soc_pcm_runtime *fe, int stream,
			       int cmd)
{
<<<<<<< HEAD
	struct snd_soc_dpcm *dpcm;
	int ret = 0;

	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);
=======
	struct snd_soc_pcm_runtime *be;
	struct snd_soc_dpcm *dpcm;
	int ret = 0;

	for_each_dpcm_be(fe, stream, dpcm) {
		struct snd_pcm_substream *be_substream;

		be = dpcm->be;
		be_substream = snd_soc_dpcm_get_substream(be, stream);
>>>>>>> upstream/android-13

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

<<<<<<< HEAD
=======
		dev_dbg(be->dev, "ASoC: trigger BE %s cmd %d\n",
			be->dai_link->name, cmd);

>>>>>>> upstream/android-13
		switch (cmd) {
		case SNDRV_PCM_TRIGGER_START:
			if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_PREPARE) &&
			    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP) &&
			    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_PAUSED))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
			if (ret)
				return ret;
=======
			ret = soc_pcm_trigger(be_substream, cmd);
			if (ret)
				goto end;
>>>>>>> upstream/android-13

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_START;
			break;
		case SNDRV_PCM_TRIGGER_RESUME:
			if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_SUSPEND))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
			if (ret)
				return ret;
=======
			ret = soc_pcm_trigger(be_substream, cmd);
			if (ret)
				goto end;
>>>>>>> upstream/android-13

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_START;
			break;
		case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
			if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_PAUSED))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
			if (ret)
				return ret;
=======
			ret = soc_pcm_trigger(be_substream, cmd);
			if (ret)
				goto end;
>>>>>>> upstream/android-13

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_START;
			break;
		case SNDRV_PCM_TRIGGER_STOP:
			if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_START) &&
			    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_PAUSED))
				continue;

			if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
			if (ret)
				return ret;
=======
			ret = soc_pcm_trigger(be_substream, cmd);
			if (ret)
				goto end;
>>>>>>> upstream/android-13

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_STOP;
			break;
		case SNDRV_PCM_TRIGGER_SUSPEND:
			if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_START)
				continue;

			if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
			if (ret)
				return ret;
=======
			ret = soc_pcm_trigger(be_substream, cmd);
			if (ret)
				goto end;
>>>>>>> upstream/android-13

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_SUSPEND;
			break;
		case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
			if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_START)
				continue;

			if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
			if (ret)
				return ret;
=======
			ret = soc_pcm_trigger(be_substream, cmd);
			if (ret)
				goto end;
>>>>>>> upstream/android-13

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_PAUSED;
			break;
		}
	}
<<<<<<< HEAD

=======
end:
	if (ret < 0)
		dev_err(fe->dev, "ASoC: %s() failed at %s (%d)\n",
			__func__, be->dai_link->name, ret);
>>>>>>> upstream/android-13
	return ret;
}
EXPORT_SYMBOL_GPL(dpcm_be_dai_trigger);

static int dpcm_dai_trigger_fe_be(struct snd_pcm_substream *substream,
				  int cmd, bool fe_first)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *fe = substream->private_data;
=======
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
>>>>>>> upstream/android-13
	int ret;

	/* call trigger on the frontend before the backend. */
	if (fe_first) {
		dev_dbg(fe->dev, "ASoC: pre trigger FE %s cmd %d\n",
			fe->dai_link->name, cmd);

		ret = soc_pcm_trigger(substream, cmd);
		if (ret < 0)
			return ret;

		ret = dpcm_be_dai_trigger(fe, substream->stream, cmd);
		return ret;
	}

	/* call trigger on the frontend after the backend. */
	ret = dpcm_be_dai_trigger(fe, substream->stream, cmd);
	if (ret < 0)
		return ret;

	dev_dbg(fe->dev, "ASoC: post trigger FE %s cmd %d\n",
		fe->dai_link->name, cmd);

	ret = soc_pcm_trigger(substream, cmd);

	return ret;
}

static int dpcm_fe_dai_do_trigger(struct snd_pcm_substream *substream, int cmd)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *fe = substream->private_data;
=======
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
>>>>>>> upstream/android-13
	int stream = substream->stream;
	int ret = 0;
	enum snd_soc_dpcm_trigger trigger = fe->dai_link->trigger[stream];

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	switch (trigger) {
	case SND_SOC_DPCM_TRIGGER_PRE:
		switch (cmd) {
		case SNDRV_PCM_TRIGGER_START:
		case SNDRV_PCM_TRIGGER_RESUME:
		case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		case SNDRV_PCM_TRIGGER_DRAIN:
			ret = dpcm_dai_trigger_fe_be(substream, cmd, true);
			break;
		case SNDRV_PCM_TRIGGER_STOP:
		case SNDRV_PCM_TRIGGER_SUSPEND:
		case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
			ret = dpcm_dai_trigger_fe_be(substream, cmd, false);
			break;
		default:
			ret = -EINVAL;
			break;
		}
		break;
	case SND_SOC_DPCM_TRIGGER_POST:
		switch (cmd) {
		case SNDRV_PCM_TRIGGER_START:
		case SNDRV_PCM_TRIGGER_RESUME:
		case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		case SNDRV_PCM_TRIGGER_DRAIN:
			ret = dpcm_dai_trigger_fe_be(substream, cmd, false);
			break;
		case SNDRV_PCM_TRIGGER_STOP:
		case SNDRV_PCM_TRIGGER_SUSPEND:
		case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
			ret = dpcm_dai_trigger_fe_be(substream, cmd, true);
			break;
		default:
			ret = -EINVAL;
			break;
		}
		break;
	case SND_SOC_DPCM_TRIGGER_BESPOKE:
		/* bespoke trigger() - handles both FE and BEs */

		dev_dbg(fe->dev, "ASoC: bespoke trigger FE %s cmd %d\n",
				fe->dai_link->name, cmd);

<<<<<<< HEAD
		ret = soc_pcm_bespoke_trigger(substream, cmd);
=======
		ret = snd_soc_pcm_dai_bespoke_trigger(substream, cmd);
>>>>>>> upstream/android-13
		break;
	default:
		dev_err(fe->dev, "ASoC: invalid trigger cmd %d for %s\n", cmd,
				fe->dai_link->name);
		ret = -EINVAL;
		goto out;
	}

	if (ret < 0) {
		dev_err(fe->dev, "ASoC: trigger FE cmd: %d failed: %d\n",
			cmd, ret);
		goto out;
	}

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		fe->dpcm[stream].state = SND_SOC_DPCM_STATE_START;
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
		fe->dpcm[stream].state = SND_SOC_DPCM_STATE_STOP;
		break;
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		fe->dpcm[stream].state = SND_SOC_DPCM_STATE_PAUSED;
		break;
	}

out:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	return ret;
}

static int dpcm_fe_dai_trigger(struct snd_pcm_substream *substream, int cmd)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *fe = substream->private_data;
=======
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
>>>>>>> upstream/android-13
	int stream = substream->stream;

	/* if FE's runtime_update is already set, we're in race;
	 * process this trigger later at exit
	 */
	if (fe->dpcm[stream].runtime_update != SND_SOC_DPCM_UPDATE_NO) {
		fe->dpcm[stream].trigger_pending = cmd + 1;
		return 0; /* delayed, assuming it's successful */
	}

	/* we're alone, let's trigger */
	return dpcm_fe_dai_do_trigger(substream, cmd);
}

int dpcm_be_dai_prepare(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm;
	int ret = 0;

<<<<<<< HEAD
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
=======
	for_each_dpcm_be(fe, stream, dpcm) {
>>>>>>> upstream/android-13

		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_SUSPEND) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_PAUSED))
			continue;

		dev_dbg(be->dev, "ASoC: prepare BE %s\n",
			be->dai_link->name);

		ret = soc_pcm_prepare(be_substream);
<<<<<<< HEAD
		if (ret < 0) {
			dev_err(be->dev, "ASoC: backend prepare failed %d\n",
				ret);
			break;
		}

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_PREPARE;
	}
=======
		if (ret < 0)
			break;

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_PREPARE;
	}

	if (ret < 0)
		dev_err(fe->dev, "ASoC: %s() failed (%d)\n", __func__, ret);

>>>>>>> upstream/android-13
	return ret;
}

static int dpcm_fe_dai_prepare(struct snd_pcm_substream *substream)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *fe = substream->private_data;
=======
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(substream);
>>>>>>> upstream/android-13
	int stream = substream->stream, ret = 0;

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);

	dev_dbg(fe->dev, "ASoC: prepare FE %s\n", fe->dai_link->name);

	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_FE);

	/* there is no point preparing this FE if there are no BEs */
	if (list_empty(&fe->dpcm[stream].be_clients)) {
		dev_err(fe->dev, "ASoC: no backend DAIs enabled for %s\n",
				fe->dai_link->name);
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	ret = dpcm_be_dai_prepare(fe, substream->stream);
=======
	ret = dpcm_be_dai_prepare(fe, stream);
>>>>>>> upstream/android-13
	if (ret < 0)
		goto out;

	/* call prepare on the frontend */
	ret = soc_pcm_prepare(substream);
<<<<<<< HEAD
	if (ret < 0) {
		dev_err(fe->dev,"ASoC: prepare FE %s failed\n",
			fe->dai_link->name);
		goto out;
	}

	/* run the stream event for each BE */
	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_START);
=======
	if (ret < 0)
		goto out;

>>>>>>> upstream/android-13
	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_PREPARE;

out:
	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_NO);
	mutex_unlock(&fe->card->mutex);

<<<<<<< HEAD
	return ret;
}

static int soc_pcm_ioctl(struct snd_pcm_substream *substream,
		     unsigned int cmd, void *arg)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->ioctl)
			continue;

		/* FIXME: use 1st ioctl */
		return component->driver->ops->ioctl(substream, cmd, arg);
	}

	return snd_pcm_lib_ioctl(substream, cmd, arg);
}

=======
	if (ret < 0)
		dev_err(fe->dev, "ASoC: %s() failed (%d)\n", __func__, ret);

	return ret;
}

>>>>>>> upstream/android-13
static int dpcm_run_update_shutdown(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_pcm_substream *substream =
		snd_soc_dpcm_get_substream(fe, stream);
	enum snd_soc_dpcm_trigger trigger = fe->dai_link->trigger[stream];
	int err;

	dev_dbg(fe->dev, "ASoC: runtime %s close on FE %s\n",
			stream ? "capture" : "playback", fe->dai_link->name);

	if (trigger == SND_SOC_DPCM_TRIGGER_BESPOKE) {
		/* call bespoke trigger - FE takes care of all BE triggers */
		dev_dbg(fe->dev, "ASoC: bespoke trigger FE %s cmd stop\n",
				fe->dai_link->name);

<<<<<<< HEAD
		err = soc_pcm_bespoke_trigger(substream, SNDRV_PCM_TRIGGER_STOP);
		if (err < 0)
			dev_err(fe->dev,"ASoC: trigger FE failed %d\n", err);
=======
		err = snd_soc_pcm_dai_bespoke_trigger(substream, SNDRV_PCM_TRIGGER_STOP);
>>>>>>> upstream/android-13
	} else {
		dev_dbg(fe->dev, "ASoC: trigger FE %s cmd stop\n",
			fe->dai_link->name);

		err = dpcm_be_dai_trigger(fe, stream, SNDRV_PCM_TRIGGER_STOP);
<<<<<<< HEAD
		if (err < 0)
			dev_err(fe->dev,"ASoC: trigger FE failed %d\n", err);
	}

	err = dpcm_be_dai_hw_free(fe, stream);
	if (err < 0)
		dev_err(fe->dev,"ASoC: hw_free FE failed %d\n", err);

	err = dpcm_be_dai_shutdown(fe, stream);
	if (err < 0)
		dev_err(fe->dev,"ASoC: shutdown FE failed %d\n", err);
=======
	}

	dpcm_be_dai_hw_free(fe, stream);

	dpcm_be_dai_shutdown(fe, stream);
>>>>>>> upstream/android-13

	/* run the stream event for each BE */
	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_NOP);

<<<<<<< HEAD
	return 0;
=======
	if (err < 0)
		dev_err(fe->dev, "ASoC: %s() failed (%d)\n", __func__, err);

	return err;
>>>>>>> upstream/android-13
}

static int dpcm_run_update_startup(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_pcm_substream *substream =
		snd_soc_dpcm_get_substream(fe, stream);
	struct snd_soc_dpcm *dpcm;
	enum snd_soc_dpcm_trigger trigger = fe->dai_link->trigger[stream];
<<<<<<< HEAD
	int ret;
=======
	int ret = 0;
	unsigned long flags;
>>>>>>> upstream/android-13

	dev_dbg(fe->dev, "ASoC: runtime %s open on FE %s\n",
			stream ? "capture" : "playback", fe->dai_link->name);

	/* Only start the BE if the FE is ready */
	if (fe->dpcm[stream].state == SND_SOC_DPCM_STATE_HW_FREE ||
<<<<<<< HEAD
		fe->dpcm[stream].state == SND_SOC_DPCM_STATE_CLOSE)
		return -EINVAL;
=======
		fe->dpcm[stream].state == SND_SOC_DPCM_STATE_CLOSE) {
		dev_err(fe->dev, "ASoC: FE %s is not ready %d\n",
			fe->dai_link->name, fe->dpcm[stream].state);
		ret = -EINVAL;
		goto disconnect;
	}
>>>>>>> upstream/android-13

	/* startup must always be called for new BEs */
	ret = dpcm_be_dai_startup(fe, stream);
	if (ret < 0)
		goto disconnect;

	/* keep going if FE state is > open */
	if (fe->dpcm[stream].state == SND_SOC_DPCM_STATE_OPEN)
		return 0;

	ret = dpcm_be_dai_hw_params(fe, stream);
	if (ret < 0)
		goto close;

	/* keep going if FE state is > hw_params */
	if (fe->dpcm[stream].state == SND_SOC_DPCM_STATE_HW_PARAMS)
		return 0;

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
	ret = dpcm_be_dai_prepare(fe, stream);
	if (ret < 0)
		goto hw_free;

	/* run the stream event for each BE */
	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_NOP);

	/* keep going if FE state is > prepare */
	if (fe->dpcm[stream].state == SND_SOC_DPCM_STATE_PREPARE ||
		fe->dpcm[stream].state == SND_SOC_DPCM_STATE_STOP)
		return 0;

	if (trigger == SND_SOC_DPCM_TRIGGER_BESPOKE) {
		/* call trigger on the frontend - FE takes care of all BE triggers */
		dev_dbg(fe->dev, "ASoC: bespoke trigger FE %s cmd start\n",
				fe->dai_link->name);

<<<<<<< HEAD
		ret = soc_pcm_bespoke_trigger(substream, SNDRV_PCM_TRIGGER_START);
		if (ret < 0) {
			dev_err(fe->dev,"ASoC: bespoke trigger FE failed %d\n", ret);
			goto hw_free;
		}
=======
		ret = snd_soc_pcm_dai_bespoke_trigger(substream, SNDRV_PCM_TRIGGER_START);
		if (ret < 0)
			goto hw_free;
>>>>>>> upstream/android-13
	} else {
		dev_dbg(fe->dev, "ASoC: trigger FE %s cmd start\n",
			fe->dai_link->name);

		ret = dpcm_be_dai_trigger(fe, stream,
					SNDRV_PCM_TRIGGER_START);
<<<<<<< HEAD
		if (ret < 0) {
			dev_err(fe->dev,"ASoC: trigger FE failed %d\n", ret);
			goto hw_free;
		}
=======
		if (ret < 0)
			goto hw_free;
>>>>>>> upstream/android-13
	}

	return 0;

hw_free:
	dpcm_be_dai_hw_free(fe, stream);
close:
	dpcm_be_dai_shutdown(fe, stream);
disconnect:
<<<<<<< HEAD
	/* disconnect any non started BEs */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_START)
				dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;
	}

	return ret;
}

static int dpcm_run_new_update(struct snd_soc_pcm_runtime *fe, int stream)
{
	int ret;

	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_BE);
	ret = dpcm_run_update_startup(fe, stream);
	if (ret < 0)
		dev_err(fe->dev, "ASoC: failed to startup some BEs\n");
	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_NO);

	return ret;
}

static int dpcm_run_old_update(struct snd_soc_pcm_runtime *fe, int stream)
{
	int ret;

	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_BE);
	ret = dpcm_run_update_shutdown(fe, stream);
	if (ret < 0)
		dev_err(fe->dev, "ASoC: failed to shutdown some BEs\n");
	dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_NO);
=======
	/* disconnect any pending BEs */
	spin_lock_irqsave(&fe->card->dpcm_lock, flags);
	for_each_dpcm_be(fe, stream, dpcm) {
		struct snd_soc_pcm_runtime *be = dpcm->be;

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		if (be->dpcm[stream].state == SND_SOC_DPCM_STATE_CLOSE ||
			be->dpcm[stream].state == SND_SOC_DPCM_STATE_NEW)
				dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;
	}
	spin_unlock_irqrestore(&fe->card->dpcm_lock, flags);

	if (ret < 0)
		dev_err(fe->dev, "ASoC: %s() failed (%d)\n", __func__, ret);
>>>>>>> upstream/android-13

	return ret;
}

static int soc_dpcm_fe_runtime_update(struct snd_soc_pcm_runtime *fe, int new)
{
	struct snd_soc_dapm_widget_list *list;
<<<<<<< HEAD
=======
	int stream;
>>>>>>> upstream/android-13
	int count, paths;

	if (!fe->dai_link->dynamic)
		return 0;

<<<<<<< HEAD
	/* only check active links */
	if (!fe->cpu_dai->active)
=======
	if (fe->num_cpus > 1) {
		dev_err(fe->dev,
			"%s doesn't support Multi CPU yet\n", __func__);
		return -EINVAL;
	}

	/* only check active links */
	if (!snd_soc_dai_active(asoc_rtd_to_cpu(fe, 0)))
>>>>>>> upstream/android-13
		return 0;

	/* DAPM sync will call this to update DSP paths */
	dev_dbg(fe->dev, "ASoC: DPCM %s runtime update for FE %s\n",
		new ? "new" : "old", fe->dai_link->name);

<<<<<<< HEAD
	/* skip if FE doesn't have playback capability */
	if (!fe->cpu_dai->driver->playback.channels_min ||
	    !fe->codec_dai->driver->playback.channels_min)
		goto capture;

	/* skip if FE isn't currently playing */
	if (!fe->cpu_dai->playback_active || !fe->codec_dai->playback_active)
		goto capture;

	paths = dpcm_path_get(fe, SNDRV_PCM_STREAM_PLAYBACK, &list);
	if (paths < 0) {
		dev_warn(fe->dev, "ASoC: %s no valid %s path\n",
			 fe->dai_link->name,  "playback");
		return paths;
	}

	/* update any playback paths */
	count = dpcm_process_paths(fe, SNDRV_PCM_STREAM_PLAYBACK, &list, new);
	if (count) {
		if (new)
			dpcm_run_new_update(fe, SNDRV_PCM_STREAM_PLAYBACK);
		else
			dpcm_run_old_update(fe, SNDRV_PCM_STREAM_PLAYBACK);

		dpcm_clear_pending_state(fe, SNDRV_PCM_STREAM_PLAYBACK);
		dpcm_be_disconnect(fe, SNDRV_PCM_STREAM_PLAYBACK);
	}

	dpcm_path_put(&list);

capture:
	/* skip if FE doesn't have capture capability */
	if (!fe->cpu_dai->driver->capture.channels_min ||
	    !fe->codec_dai->driver->capture.channels_min)
		return 0;

	/* skip if FE isn't currently capturing */
	if (!fe->cpu_dai->capture_active || !fe->codec_dai->capture_active)
		return 0;

	paths = dpcm_path_get(fe, SNDRV_PCM_STREAM_CAPTURE, &list);
	if (paths < 0) {
		dev_warn(fe->dev, "ASoC: %s no valid %s path\n",
			 fe->dai_link->name,  "capture");
		return paths;
	}

	/* update any old capture paths */
	count = dpcm_process_paths(fe, SNDRV_PCM_STREAM_CAPTURE, &list, new);
	if (count) {
		if (new)
			dpcm_run_new_update(fe, SNDRV_PCM_STREAM_CAPTURE);
		else
			dpcm_run_old_update(fe, SNDRV_PCM_STREAM_CAPTURE);

		dpcm_clear_pending_state(fe, SNDRV_PCM_STREAM_CAPTURE);
		dpcm_be_disconnect(fe, SNDRV_PCM_STREAM_CAPTURE);
	}

	dpcm_path_put(&list);

=======
	for_each_pcm_streams(stream) {

		/* skip if FE doesn't have playback/capture capability */
		if (!snd_soc_dai_stream_valid(asoc_rtd_to_cpu(fe, 0),   stream) ||
		    !snd_soc_dai_stream_valid(asoc_rtd_to_codec(fe, 0), stream))
			continue;

		/* skip if FE isn't currently playing/capturing */
		if (!snd_soc_dai_stream_active(asoc_rtd_to_cpu(fe, 0), stream) ||
		    !snd_soc_dai_stream_active(asoc_rtd_to_codec(fe, 0), stream))
			continue;

		paths = dpcm_path_get(fe, stream, &list);
		if (paths < 0)
			return paths;

		/* update any playback/capture paths */
		count = dpcm_process_paths(fe, stream, &list, new);
		if (count) {
			dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_BE);
			if (new)
				dpcm_run_update_startup(fe, stream);
			else
				dpcm_run_update_shutdown(fe, stream);
			dpcm_set_fe_update_state(fe, stream, SND_SOC_DPCM_UPDATE_NO);

			dpcm_clear_pending_state(fe, stream);
			dpcm_be_disconnect(fe, stream);
		}

		dpcm_path_put(&list);
	}

>>>>>>> upstream/android-13
	return 0;
}

/* Called by DAPM mixer/mux changes to update audio routing between PCMs and
 * any DAI links.
 */
<<<<<<< HEAD
int soc_dpcm_runtime_update(struct snd_soc_card *card)
=======
int snd_soc_dpcm_runtime_update(struct snd_soc_card *card)
>>>>>>> upstream/android-13
{
	struct snd_soc_pcm_runtime *fe;
	int ret = 0;

	mutex_lock_nested(&card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
	/* shutdown all old paths first */
<<<<<<< HEAD
	list_for_each_entry(fe, &card->rtd_list, list) {
=======
	for_each_card_rtds(card, fe) {
>>>>>>> upstream/android-13
		ret = soc_dpcm_fe_runtime_update(fe, 0);
		if (ret)
			goto out;
	}

	/* bring new paths up */
<<<<<<< HEAD
	list_for_each_entry(fe, &card->rtd_list, list) {
=======
	for_each_card_rtds(card, fe) {
>>>>>>> upstream/android-13
		ret = soc_dpcm_fe_runtime_update(fe, 1);
		if (ret)
			goto out;
	}

out:
	mutex_unlock(&card->mutex);
	return ret;
}
<<<<<<< HEAD
int soc_dpcm_be_digital_mute(struct snd_soc_pcm_runtime *fe, int mute)
{
	struct snd_soc_dpcm *dpcm;
	struct list_head *clients =
		&fe->dpcm[SNDRV_PCM_STREAM_PLAYBACK].be_clients;

	list_for_each_entry(dpcm, clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
		int i;

		if (be->dai_link->ignore_suspend)
			continue;

		for (i = 0; i < be->num_codecs; i++) {
			struct snd_soc_dai *dai = be->codec_dais[i];
			struct snd_soc_dai_driver *drv = dai->driver;

			dev_dbg(be->dev, "ASoC: BE digital mute %s\n",
					 be->dai_link->name);

			if (drv->ops && drv->ops->digital_mute &&
							dai->playback_active)
				drv->ops->digital_mute(dai, mute);
		}
	}

	return 0;
=======
EXPORT_SYMBOL_GPL(snd_soc_dpcm_runtime_update);

static void dpcm_fe_dai_cleanup(struct snd_pcm_substream *fe_substream)
{
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(fe_substream);
	struct snd_soc_dpcm *dpcm;
	int stream = fe_substream->stream;

	/* mark FE's links ready to prune */
	for_each_dpcm_be(fe, stream, dpcm)
		dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

	dpcm_be_disconnect(fe, stream);

	fe->dpcm[stream].runtime = NULL;
}

static int dpcm_fe_dai_close(struct snd_pcm_substream *fe_substream)
{
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(fe_substream);
	int ret;

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
	ret = dpcm_fe_dai_shutdown(fe_substream);

	dpcm_fe_dai_cleanup(fe_substream);

	mutex_unlock(&fe->card->mutex);
	return ret;
>>>>>>> upstream/android-13
}

static int dpcm_fe_dai_open(struct snd_pcm_substream *fe_substream)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *fe = fe_substream->private_data;
	struct snd_soc_dpcm *dpcm;
=======
	struct snd_soc_pcm_runtime *fe = asoc_substream_to_rtd(fe_substream);
>>>>>>> upstream/android-13
	struct snd_soc_dapm_widget_list *list;
	int ret;
	int stream = fe_substream->stream;

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
	fe->dpcm[stream].runtime = fe_substream->runtime;

	ret = dpcm_path_get(fe, stream, &list);
<<<<<<< HEAD
	if (ret < 0) {
		mutex_unlock(&fe->card->mutex);
		return ret;
	} else if (ret == 0) {
		dev_dbg(fe->dev, "ASoC: %s no valid %s route\n",
			fe->dai_link->name, stream ? "capture" : "playback");
	}
=======
	if (ret < 0)
		goto open_end;
>>>>>>> upstream/android-13

	/* calculate valid and active FE <-> BE dpcms */
	dpcm_process_paths(fe, stream, &list, 1);

	ret = dpcm_fe_dai_startup(fe_substream);
<<<<<<< HEAD
	if (ret < 0) {
		/* clean up all links */
		list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be)
			dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

		dpcm_be_disconnect(fe, stream);
		fe->dpcm[stream].runtime = NULL;
	}

	dpcm_clear_pending_state(fe, stream);
	dpcm_path_put(&list);
=======
	if (ret < 0)
		dpcm_fe_dai_cleanup(fe_substream);

	dpcm_clear_pending_state(fe, stream);
	dpcm_path_put(&list);
open_end:
>>>>>>> upstream/android-13
	mutex_unlock(&fe->card->mutex);
	return ret;
}

<<<<<<< HEAD
static int dpcm_fe_dai_close(struct snd_pcm_substream *fe_substream)
{
	struct snd_soc_pcm_runtime *fe = fe_substream->private_data;
	struct snd_soc_dpcm *dpcm;
	int stream = fe_substream->stream, ret;

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
	ret = dpcm_fe_dai_shutdown(fe_substream);

	/* mark FE's links ready to prune */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be)
		dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

	dpcm_be_disconnect(fe, stream);

	fe->dpcm[stream].runtime = NULL;
	mutex_unlock(&fe->card->mutex);
	return ret;
}

static void soc_pcm_private_free(struct snd_pcm *pcm)
{
	struct snd_soc_pcm_runtime *rtd = pcm->private_data;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_component *component;

	/* need to sync the delayed work before releasing resources */
	flush_delayed_work(&rtd->delayed_work);
	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (component->driver->pcm_free)
			component->driver->pcm_free(pcm);
	}
}

static int soc_rtdcom_ack(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_component *component;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->ack)
			continue;

		/* FIXME. it returns 1st ask now */
		return component->driver->ops->ack(substream);
	}

	return -EINVAL;
}

static int soc_rtdcom_copy_user(struct snd_pcm_substream *substream, int channel,
				unsigned long pos, void __user *buf,
				unsigned long bytes)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_component *component;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->copy_user)
			continue;

		/* FIXME. it returns 1st copy now */
		return component->driver->ops->copy_user(substream, channel,
							 pos, buf, bytes);
	}

	return -EINVAL;
}

static int soc_rtdcom_copy_kernel(struct snd_pcm_substream *substream, int channel,
				  unsigned long pos, void *buf, unsigned long bytes)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_component *component;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->copy_kernel)
			continue;

		/* FIXME. it returns 1st copy now */
		return component->driver->ops->copy_kernel(substream, channel,
							   pos, buf, bytes);
	}

	return -EINVAL;
}

static int soc_rtdcom_fill_silence(struct snd_pcm_substream *substream, int channel,
				   unsigned long pos, unsigned long bytes)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_component *component;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->fill_silence)
			continue;

		/* FIXME. it returns 1st silence now */
		return component->driver->ops->fill_silence(substream, channel,
							    pos, bytes);
	}

	return -EINVAL;
}

static struct page *soc_rtdcom_page(struct snd_pcm_substream *substream,
				    unsigned long offset)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_component *component;
	struct page *page;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->page)
			continue;

		/* FIXME. it returns 1st page now */
		page = component->driver->ops->page(substream, offset);
		if (page)
			return page;
	}

	return NULL;
}

static int soc_rtdcom_mmap(struct snd_pcm_substream *substream,
			   struct vm_area_struct *vma)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_component *component;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->ops ||
		    !component->driver->ops->mmap)
			continue;

		/* FIXME. it returns 1st mmap now */
		return component->driver->ops->mmap(substream, vma);
	}

	return -EINVAL;
}

/* create a new pcm */
int soc_new_pcm(struct snd_soc_pcm_runtime *rtd, int num)
{
	struct snd_soc_dai *codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_pcm *pcm;
	struct snd_pcm_str *stream;
	char new_name[64];
	int ret = 0, playback = 0, capture = 0;
	int i;

	if (rtd->dai_link->dynamic || rtd->dai_link->no_pcm) {
		playback = rtd->dai_link->dpcm_playback;
		capture = rtd->dai_link->dpcm_capture;
	} else {
		for (i = 0; i < rtd->num_codecs; i++) {
			codec_dai = rtd->codec_dais[i];
			if (codec_dai->driver->playback.channels_min)
				playback = 1;
			if (codec_dai->driver->capture.channels_min)
				capture = 1;
		}

		capture = capture && cpu_dai->driver->capture.channels_min;
		playback = playback && cpu_dai->driver->playback.channels_min;
	}

	if (rtd->dai_link->playback_only) {
		playback = 1;
		capture = 0;
	}

	if (rtd->dai_link->capture_only) {
		playback = 0;
		capture = 1;
	}

	/* create the PCM */
	if (rtd->dai_link->no_pcm) {
=======
static int soc_get_playback_capture(struct snd_soc_pcm_runtime *rtd,
				    int *playback, int *capture)
{
	struct snd_soc_dai *cpu_dai;
	int i;

	if (rtd->dai_link->dynamic && rtd->num_cpus > 1) {
		dev_err(rtd->dev,
			"DPCM doesn't support Multi CPU for Front-Ends yet\n");
		return -EINVAL;
	}

	if (rtd->dai_link->dynamic || rtd->dai_link->no_pcm) {
		int stream;

		if (rtd->dai_link->dpcm_playback) {
			stream = SNDRV_PCM_STREAM_PLAYBACK;

			for_each_rtd_cpu_dais(rtd, i, cpu_dai) {
				if (snd_soc_dai_stream_valid(cpu_dai, stream)) {
					*playback = 1;
					break;
				}
			}
			if (!*playback) {
				dev_err(rtd->card->dev,
					"No CPU DAIs support playback for stream %s\n",
					rtd->dai_link->stream_name);
				return -EINVAL;
			}
		}
		if (rtd->dai_link->dpcm_capture) {
			stream = SNDRV_PCM_STREAM_CAPTURE;

			for_each_rtd_cpu_dais(rtd, i, cpu_dai) {
				if (snd_soc_dai_stream_valid(cpu_dai, stream)) {
					*capture = 1;
					break;
				}
			}

			if (!*capture) {
				dev_err(rtd->card->dev,
					"No CPU DAIs support capture for stream %s\n",
					rtd->dai_link->stream_name);
				return -EINVAL;
			}
		}
	} else {
		struct snd_soc_dai *codec_dai;

		/* Adapt stream for codec2codec links */
		int cpu_capture = rtd->dai_link->params ?
			SNDRV_PCM_STREAM_PLAYBACK : SNDRV_PCM_STREAM_CAPTURE;
		int cpu_playback = rtd->dai_link->params ?
			SNDRV_PCM_STREAM_CAPTURE : SNDRV_PCM_STREAM_PLAYBACK;

		for_each_rtd_codec_dais(rtd, i, codec_dai) {
			if (rtd->num_cpus == 1) {
				cpu_dai = asoc_rtd_to_cpu(rtd, 0);
			} else if (rtd->num_cpus == rtd->num_codecs) {
				cpu_dai = asoc_rtd_to_cpu(rtd, i);
			} else {
				dev_err(rtd->card->dev,
					"N cpus to M codecs link is not supported yet\n");
				return -EINVAL;
			}

			if (snd_soc_dai_stream_valid(codec_dai, SNDRV_PCM_STREAM_PLAYBACK) &&
			    snd_soc_dai_stream_valid(cpu_dai,   cpu_playback))
				*playback = 1;
			if (snd_soc_dai_stream_valid(codec_dai, SNDRV_PCM_STREAM_CAPTURE) &&
			    snd_soc_dai_stream_valid(cpu_dai,   cpu_capture))
				*capture = 1;
		}
	}

	if (rtd->dai_link->playback_only) {
		*playback = 1;
		*capture = 0;
	}

	if (rtd->dai_link->capture_only) {
		*playback = 0;
		*capture = 1;
	}

	return 0;
}

static int soc_create_pcm(struct snd_pcm **pcm,
			  struct snd_soc_pcm_runtime *rtd,
			  int playback, int capture, int num)
{
	char new_name[64];
	int ret;

	/* create the PCM */
	if (rtd->dai_link->params) {
		snprintf(new_name, sizeof(new_name), "codec2codec(%s)",
			 rtd->dai_link->stream_name);

		ret = snd_pcm_new_internal(rtd->card->snd_card, new_name, num,
					   playback, capture, pcm);
	} else if (rtd->dai_link->no_pcm) {
>>>>>>> upstream/android-13
		snprintf(new_name, sizeof(new_name), "(%s)",
			rtd->dai_link->stream_name);

		ret = snd_pcm_new_internal(rtd->card->snd_card, new_name, num,
<<<<<<< HEAD
				playback, capture, &pcm);
=======
				playback, capture, pcm);
>>>>>>> upstream/android-13
	} else {
		if (rtd->dai_link->dynamic)
			snprintf(new_name, sizeof(new_name), "%s (*)",
				rtd->dai_link->stream_name);
		else
			snprintf(new_name, sizeof(new_name), "%s %s-%d",
				rtd->dai_link->stream_name,
<<<<<<< HEAD
				(rtd->num_codecs > 1) ?
				"multicodec" : rtd->codec_dai->name, num);

		ret = snd_pcm_new(rtd->card->snd_card, new_name, num, playback,
			capture, &pcm);
	}
	if (ret < 0) {
		dev_err(rtd->card->dev, "ASoC: can't create pcm for %s\n",
			rtd->dai_link->name);
=======
				soc_codec_dai_name(rtd), num);

		ret = snd_pcm_new(rtd->card->snd_card, new_name, num, playback,
			capture, pcm);
	}
	if (ret < 0) {
		dev_err(rtd->card->dev, "ASoC: can't create pcm %s for dailink %s: %d\n",
			new_name, rtd->dai_link->name, ret);
>>>>>>> upstream/android-13
		return ret;
	}
	dev_dbg(rtd->card->dev, "ASoC: registered pcm #%d %s\n",num, new_name);

<<<<<<< HEAD
	/* DAPM dai link stream work */
	INIT_DELAYED_WORK(&rtd->delayed_work, close_delayed_work);

	pcm->nonatomic = rtd->dai_link->nonatomic;
	rtd->pcm = pcm;
	pcm->private_data = rtd;

	if (rtd->dai_link->no_pcm) {
=======
	return 0;
}

/* create a new pcm */
int soc_new_pcm(struct snd_soc_pcm_runtime *rtd, int num)
{
	struct snd_soc_component *component;
	struct snd_pcm *pcm;
	int ret = 0, playback = 0, capture = 0;
	int i;

	ret = soc_get_playback_capture(rtd, &playback, &capture);
	if (ret < 0)
		return ret;

	ret = soc_create_pcm(&pcm, rtd, playback, capture, num);
	if (ret < 0)
		return ret;

	/* DAPM dai link stream work */
	if (rtd->dai_link->params)
		rtd->close_delayed_work_func = codec2codec_close_delayed_work;
	else
		rtd->close_delayed_work_func = snd_soc_close_delayed_work;

	rtd->pcm = pcm;
	pcm->nonatomic = rtd->dai_link->nonatomic;
	pcm->private_data = rtd;

	if (rtd->dai_link->no_pcm || rtd->dai_link->params) {
>>>>>>> upstream/android-13
		if (playback)
			pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream->private_data = rtd;
		if (capture)
			pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream->private_data = rtd;
		goto out;
	}

<<<<<<< HEAD
	/* setup any hostless PCMs - i.e. no host IO is performed */
	if (rtd->dai_link->no_host_mode) {
		if (pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream) {
			stream = &pcm->streams[SNDRV_PCM_STREAM_PLAYBACK];
			stream->substream->hw_no_buffer = 1;
			snd_soc_set_runtime_hwparams(stream->substream,
						     &no_host_hardware);
		}
		if (pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream) {
			stream = &pcm->streams[SNDRV_PCM_STREAM_CAPTURE];
			stream->substream->hw_no_buffer = 1;
			snd_soc_set_runtime_hwparams(stream->substream,
						     &no_host_hardware);
		}
	}

=======
>>>>>>> upstream/android-13
	/* ASoC PCM operations */
	if (rtd->dai_link->dynamic) {
		rtd->ops.open		= dpcm_fe_dai_open;
		rtd->ops.hw_params	= dpcm_fe_dai_hw_params;
		rtd->ops.prepare	= dpcm_fe_dai_prepare;
		rtd->ops.trigger	= dpcm_fe_dai_trigger;
		rtd->ops.hw_free	= dpcm_fe_dai_hw_free;
		rtd->ops.close		= dpcm_fe_dai_close;
		rtd->ops.pointer	= soc_pcm_pointer;
<<<<<<< HEAD
		rtd->ops.ioctl		= soc_pcm_ioctl;
=======
>>>>>>> upstream/android-13
	} else {
		rtd->ops.open		= soc_pcm_open;
		rtd->ops.hw_params	= soc_pcm_hw_params;
		rtd->ops.prepare	= soc_pcm_prepare;
		rtd->ops.trigger	= soc_pcm_trigger;
		rtd->ops.hw_free	= soc_pcm_hw_free;
		rtd->ops.close		= soc_pcm_close;
		rtd->ops.pointer	= soc_pcm_pointer;
<<<<<<< HEAD
		rtd->ops.ioctl		= soc_pcm_ioctl;
	}

	for_each_rtdcom(rtd, rtdcom) {
		const struct snd_pcm_ops *ops = rtdcom->component->driver->ops;

		if (!ops)
			continue;

		if (ops->ack)
			rtd->ops.ack		= soc_rtdcom_ack;
		if (ops->copy_user)
			rtd->ops.copy_user	= soc_rtdcom_copy_user;
		if (ops->copy_kernel)
			rtd->ops.copy_kernel	= soc_rtdcom_copy_kernel;
		if (ops->fill_silence)
			rtd->ops.fill_silence	= soc_rtdcom_fill_silence;
		if (ops->page)
			rtd->ops.page		= soc_rtdcom_page;
		if (ops->mmap)
			rtd->ops.mmap		= soc_rtdcom_mmap;
=======
	}

	for_each_rtd_components(rtd, i, component) {
		const struct snd_soc_component_driver *drv = component->driver;

		if (drv->ioctl)
			rtd->ops.ioctl		= snd_soc_pcm_component_ioctl;
		if (drv->sync_stop)
			rtd->ops.sync_stop	= snd_soc_pcm_component_sync_stop;
		if (drv->copy_user)
			rtd->ops.copy_user	= snd_soc_pcm_component_copy_user;
		if (drv->page)
			rtd->ops.page		= snd_soc_pcm_component_page;
		if (drv->mmap)
			rtd->ops.mmap		= snd_soc_pcm_component_mmap;
		if (drv->ack)
			rtd->ops.ack            = snd_soc_pcm_component_ack;
>>>>>>> upstream/android-13
	}

	if (playback)
		snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK, &rtd->ops);

	if (capture)
		snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_CAPTURE, &rtd->ops);

<<<<<<< HEAD
	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->pcm_new)
			continue;

		ret = component->driver->pcm_new(rtd);
		if (ret < 0) {
			dev_err(component->dev,
				"ASoC: pcm constructor failed: %d\n",
				ret);
			return ret;
		}
	}

	pcm->private_free = soc_pcm_private_free;
out:
	dev_info(rtd->card->dev, "%s <-> %s mapping ok\n",
		 (rtd->num_codecs > 1) ? "multicodec" : rtd->codec_dai->name,
		 cpu_dai->name);
=======
	ret = snd_soc_pcm_component_new(rtd);
	if (ret < 0)
		return ret;

	pcm->no_device_suspend = true;
out:
	dev_dbg(rtd->card->dev, "%s <-> %s mapping ok\n",
		soc_codec_dai_name(rtd), soc_cpu_dai_name(rtd));
>>>>>>> upstream/android-13
	return ret;
}

/* is the current PCM operation for this FE ? */
int snd_soc_dpcm_fe_can_update(struct snd_soc_pcm_runtime *fe, int stream)
{
	if (fe->dpcm[stream].runtime_update == SND_SOC_DPCM_UPDATE_FE)
		return 1;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_fe_can_update);

/* is the current PCM operation for this BE ? */
int snd_soc_dpcm_be_can_update(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
	if ((fe->dpcm[stream].runtime_update == SND_SOC_DPCM_UPDATE_FE) ||
	   ((fe->dpcm[stream].runtime_update == SND_SOC_DPCM_UPDATE_BE) &&
		  be->dpcm[stream].runtime_update))
		return 1;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_be_can_update);

/* get the substream for this BE */
struct snd_pcm_substream *
	snd_soc_dpcm_get_substream(struct snd_soc_pcm_runtime *be, int stream)
{
	return be->pcm->streams[stream].substream;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_get_substream);

<<<<<<< HEAD
/* get the BE runtime state */
enum snd_soc_dpcm_state
	snd_soc_dpcm_be_get_state(struct snd_soc_pcm_runtime *be, int stream)
{
	return be->dpcm[stream].state;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_be_get_state);

/* set the BE runtime state */
void snd_soc_dpcm_be_set_state(struct snd_soc_pcm_runtime *be,
		int stream, enum snd_soc_dpcm_state state)
{
	be->dpcm[stream].state = state;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_be_set_state);
=======
static int snd_soc_dpcm_check_state(struct snd_soc_pcm_runtime *fe,
				    struct snd_soc_pcm_runtime *be,
				    int stream,
				    const enum snd_soc_dpcm_state *states,
				    int num_states)
{
	struct snd_soc_dpcm *dpcm;
	int state;
	int ret = 1;
	unsigned long flags;
	int i;

	spin_lock_irqsave(&fe->card->dpcm_lock, flags);
	for_each_dpcm_fe(be, stream, dpcm) {

		if (dpcm->fe == fe)
			continue;

		state = dpcm->fe->dpcm[stream].state;
		for (i = 0; i < num_states; i++) {
			if (state == states[i]) {
				ret = 0;
				break;
			}
		}
	}
	spin_unlock_irqrestore(&fe->card->dpcm_lock, flags);

	/* it's safe to do this BE DAI */
	return ret;
}
>>>>>>> upstream/android-13

/*
 * We can only hw_free, stop, pause or suspend a BE DAI if any of it's FE
 * are not running, paused or suspended for the specified stream direction.
 */
int snd_soc_dpcm_can_be_free_stop(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
<<<<<<< HEAD
	struct snd_soc_dpcm *dpcm;
	int state;

	list_for_each_entry(dpcm, &be->dpcm[stream].fe_clients, list_fe) {

		if (dpcm->fe == fe)
			continue;

		state = dpcm->fe->dpcm[stream].state;
		if (state == SND_SOC_DPCM_STATE_START ||
			state == SND_SOC_DPCM_STATE_PAUSED ||
			state == SND_SOC_DPCM_STATE_SUSPEND)
			return 0;
	}

	/* it's safe to free/stop this BE DAI */
	return 1;
=======
	const enum snd_soc_dpcm_state state[] = {
		SND_SOC_DPCM_STATE_START,
		SND_SOC_DPCM_STATE_PAUSED,
		SND_SOC_DPCM_STATE_SUSPEND,
	};

	return snd_soc_dpcm_check_state(fe, be, stream, state, ARRAY_SIZE(state));
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_can_be_free_stop);

/*
 * We can only change hw params a BE DAI if any of it's FE are not prepared,
 * running, paused or suspended for the specified stream direction.
 */
int snd_soc_dpcm_can_be_params(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
<<<<<<< HEAD
	struct snd_soc_dpcm *dpcm;
	int state;

	list_for_each_entry(dpcm, &be->dpcm[stream].fe_clients, list_fe) {

		if (dpcm->fe == fe)
			continue;

		state = dpcm->fe->dpcm[stream].state;
		if (state == SND_SOC_DPCM_STATE_START ||
			state == SND_SOC_DPCM_STATE_PAUSED ||
			state == SND_SOC_DPCM_STATE_SUSPEND ||
			state == SND_SOC_DPCM_STATE_PREPARE)
			return 0;
	}

	/* it's safe to change hw_params */
	return 1;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_can_be_params);

#ifdef CONFIG_DEBUG_FS
static const char *dpcm_state_string(enum snd_soc_dpcm_state state)
{
	switch (state) {
	case SND_SOC_DPCM_STATE_NEW:
		return "new";
	case SND_SOC_DPCM_STATE_OPEN:
		return "open";
	case SND_SOC_DPCM_STATE_HW_PARAMS:
		return "hw_params";
	case SND_SOC_DPCM_STATE_PREPARE:
		return "prepare";
	case SND_SOC_DPCM_STATE_START:
		return "start";
	case SND_SOC_DPCM_STATE_STOP:
		return "stop";
	case SND_SOC_DPCM_STATE_SUSPEND:
		return "suspend";
	case SND_SOC_DPCM_STATE_PAUSED:
		return "paused";
	case SND_SOC_DPCM_STATE_HW_FREE:
		return "hw_free";
	case SND_SOC_DPCM_STATE_CLOSE:
		return "close";
	}

	return "unknown";
}

static ssize_t dpcm_show_state(struct snd_soc_pcm_runtime *fe,
				int stream, char *buf, size_t size)
{
	struct snd_pcm_hw_params *params = &fe->dpcm[stream].hw_params;
	struct snd_soc_dpcm *dpcm;
	ssize_t offset = 0;

	/* FE state */
	offset += scnprintf(buf + offset, size - offset,
			"[%s - %s]\n", fe->dai_link->name,
			stream ? "Capture" : "Playback");

	offset += scnprintf(buf + offset, size - offset, "State: %s\n",
	                dpcm_state_string(fe->dpcm[stream].state));

	if ((fe->dpcm[stream].state >= SND_SOC_DPCM_STATE_HW_PARAMS) &&
	    (fe->dpcm[stream].state <= SND_SOC_DPCM_STATE_STOP))
		offset += scnprintf(buf + offset, size - offset,
				"Hardware Params: "
				"Format = %s, Channels = %d, Rate = %d\n",
				snd_pcm_format_name(params_format(params)),
				params_channels(params),
				params_rate(params));

	/* BEs state */
	offset += scnprintf(buf + offset, size - offset, "Backends:\n");

	if (list_empty(&fe->dpcm[stream].be_clients)) {
		offset += scnprintf(buf + offset, size - offset,
				" No active DSP links\n");
		goto out;
	}

	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		params = &dpcm->hw_params;

		offset += scnprintf(buf + offset, size - offset,
				"- %s\n", be->dai_link->name);

		offset += scnprintf(buf + offset, size - offset,
				"   State: %s\n",
				dpcm_state_string(be->dpcm[stream].state));

		if ((be->dpcm[stream].state >= SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state <= SND_SOC_DPCM_STATE_STOP))
			offset += scnprintf(buf + offset, size - offset,
				"   Hardware Params: "
				"Format = %s, Channels = %d, Rate = %d\n",
				snd_pcm_format_name(params_format(params)),
				params_channels(params),
				params_rate(params));
	}

out:
	return offset;
}

static ssize_t dpcm_state_read_file(struct file *file, char __user *user_buf,
				size_t count, loff_t *ppos)
{
	struct snd_soc_pcm_runtime *fe = file->private_data;
	ssize_t out_count = PAGE_SIZE, offset = 0, ret = 0;
	char *buf;

	buf = kmalloc(out_count, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	if (fe->cpu_dai->driver->playback.channels_min)
		offset += dpcm_show_state(fe, SNDRV_PCM_STREAM_PLAYBACK,
					buf + offset, out_count - offset);

	if (fe->cpu_dai->driver->capture.channels_min)
		offset += dpcm_show_state(fe, SNDRV_PCM_STREAM_CAPTURE,
					buf + offset, out_count - offset);

	ret = simple_read_from_buffer(user_buf, count, ppos, buf, offset);

	kfree(buf);
	return ret;
}

static const struct file_operations dpcm_state_fops = {
	.open = simple_open,
	.read = dpcm_state_read_file,
	.llseek = default_llseek,
};

void soc_dpcm_debugfs_add(struct snd_soc_pcm_runtime *rtd)
{
	if (!rtd->dai_link)
		return;

	if (!rtd->card->debugfs_card_root)
		return;

	rtd->debugfs_dpcm_root = debugfs_create_dir(rtd->dai_link->name,
			rtd->card->debugfs_card_root);
	if (!rtd->debugfs_dpcm_root) {
		dev_dbg(rtd->dev,
			 "ASoC: Failed to create dpcm debugfs directory %s\n",
			 rtd->dai_link->name);
		return;
	}

	debugfs_create_file("state", 0444, rtd->debugfs_dpcm_root,
			    rtd, &dpcm_state_fops);
}
#endif
=======
	const enum snd_soc_dpcm_state state[] = {
		SND_SOC_DPCM_STATE_START,
		SND_SOC_DPCM_STATE_PAUSED,
		SND_SOC_DPCM_STATE_SUSPEND,
		SND_SOC_DPCM_STATE_PREPARE,
	};

	return snd_soc_dpcm_check_state(fe, be, stream, state, ARRAY_SIZE(state));
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_can_be_params);
>>>>>>> upstream/android-13
