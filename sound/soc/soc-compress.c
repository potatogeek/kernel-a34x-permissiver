// SPDX-License-Identifier: GPL-2.0+
//
// soc-compress.c  --  ALSA SoC Compress
//
// Copyright (C) 2012 Intel Corp.
//
// Authors: Namarta Kohli <namartax.kohli@intel.com>
//          Ramesh Babu K V <ramesh.babu@linux.intel.com>
//          Vinod Koul <vinod.koul@linux.intel.com>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <sound/core.h>
#include <sound/compress_params.h>
#include <sound/compress_driver.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <sound/soc-dpcm.h>
<<<<<<< HEAD

static int soc_compr_components_open(struct snd_compr_stream *cstream,
				     struct snd_soc_component **last)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	int ret;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->open)
			continue;

		ret = component->driver->compr_ops->open(cstream);
		if (ret < 0) {
			dev_err(component->dev,
				"Compress ASoC: can't open platform %s: %d\n",
				component->name, ret);

			*last = component;
			return ret;
		}
	}

	*last = NULL;
	return 0;
}

static int soc_compr_components_free(struct snd_compr_stream *cstream,
				     struct snd_soc_component *last)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (component == last)
			break;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->free)
			continue;

		component->driver->compr_ops->free(cstream);
	}

	return 0;
=======
#include <sound/soc-link.h>
#include <linux/pm_runtime.h>

static int soc_compr_clean(struct snd_compr_stream *cstream, int rollback)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
	struct snd_soc_dai *codec_dai = asoc_rtd_to_codec(rtd, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	if (!rollback)
		snd_soc_runtime_deactivate(rtd, stream);

	snd_soc_dai_digital_mute(codec_dai, 1, stream);

	if (!snd_soc_dai_active(cpu_dai))
		cpu_dai->rate = 0;

	if (!snd_soc_dai_active(codec_dai))
		codec_dai->rate = 0;

	snd_soc_link_compr_shutdown(cstream, rollback);

	snd_soc_component_compr_free(cstream, rollback);

	snd_soc_dai_compr_shutdown(cpu_dai, cstream, rollback);

	if (!rollback)
		snd_soc_dapm_stream_stop(rtd, stream);

	mutex_unlock(&rtd->card->pcm_mutex);

	snd_soc_pcm_component_pm_runtime_put(rtd, cstream, rollback);

	return 0;
}

static int soc_compr_free(struct snd_compr_stream *cstream)
{
	return soc_compr_clean(cstream, 0);
>>>>>>> upstream/android-13
}

static int soc_compr_open(struct snd_compr_stream *cstream)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
<<<<<<< HEAD
	struct snd_soc_component *component;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->startup) {
		ret = cpu_dai->driver->cops->startup(cstream, cpu_dai);
		if (ret < 0) {
			dev_err(cpu_dai->dev,
				"Compress ASoC: can't open interface %s: %d\n",
				cpu_dai->name, ret);
			goto out;
		}
	}

	ret = soc_compr_components_open(cstream, &component);
	if (ret < 0)
		goto machine_err;

	if (rtd->dai_link->compr_ops && rtd->dai_link->compr_ops->startup) {
		ret = rtd->dai_link->compr_ops->startup(cstream);
		if (ret < 0) {
			dev_err(rtd->dev,
				"Compress ASoC: %s startup failed: %d\n",
				rtd->dai_link->name, ret);
			goto machine_err;
		}
	}

	snd_soc_runtime_activate(rtd, cstream->direction);

	mutex_unlock(&rtd->pcm_mutex);

	return 0;

machine_err:
	soc_compr_components_free(cstream, component);

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->shutdown)
		cpu_dai->driver->cops->shutdown(cstream, cpu_dai);
out:
	mutex_unlock(&rtd->pcm_mutex);
=======
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

	ret = snd_soc_pcm_component_pm_runtime_get(rtd, cstream);
	if (ret < 0)
		goto err_no_lock;

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	ret = snd_soc_dai_compr_startup(cpu_dai, cstream);
	if (ret < 0)
		goto err;

	ret = snd_soc_component_compr_open(cstream);
	if (ret < 0)
		goto err;

	ret = snd_soc_link_compr_startup(cstream);
	if (ret < 0)
		goto err;

	snd_soc_runtime_activate(rtd, stream);
err:
	mutex_unlock(&rtd->card->pcm_mutex);
err_no_lock:
	if (ret < 0)
		soc_compr_clean(cstream, 1);

>>>>>>> upstream/android-13
	return ret;
}

static int soc_compr_open_fe(struct snd_compr_stream *cstream)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
	struct snd_pcm_substream *fe_substream =
		 fe->pcm->streams[cstream->direction].substream;
<<<<<<< HEAD
	struct snd_soc_component *component;
	struct snd_soc_dai *cpu_dai = fe->cpu_dai;
	struct snd_soc_dpcm *dpcm;
	struct snd_soc_dapm_widget_list *list;
	int stream;
	int ret;

	if (cstream->direction == SND_COMPRESS_PLAYBACK)
		stream = SNDRV_PCM_STREAM_PLAYBACK;
	else
		stream = SNDRV_PCM_STREAM_CAPTURE;

=======
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(fe, 0);
	struct snd_soc_dpcm *dpcm;
	struct snd_soc_dapm_widget_list *list;
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

>>>>>>> upstream/android-13
	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
	fe->dpcm[stream].runtime = fe_substream->runtime;

	ret = dpcm_path_get(fe, stream, &list);
	if (ret < 0)
		goto be_err;
<<<<<<< HEAD
	else if (ret == 0)
		dev_dbg(fe->dev, "Compress ASoC: %s no valid %s route\n",
			fe->dai_link->name, stream ? "capture" : "playback");
=======

>>>>>>> upstream/android-13
	/* calculate valid and active FE <-> BE dpcms */
	dpcm_process_paths(fe, stream, &list, 1);
	fe->dpcm[stream].runtime = fe_substream->runtime;

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	ret = dpcm_be_dai_startup(fe, stream);
	if (ret < 0) {
		/* clean up all links */
<<<<<<< HEAD
		list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be)
=======
		for_each_dpcm_be(fe, stream, dpcm)
>>>>>>> upstream/android-13
			dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

		dpcm_be_disconnect(fe, stream);
		fe->dpcm[stream].runtime = NULL;
		goto out;
	}

<<<<<<< HEAD
	if (cpu_dai->driver->cops && cpu_dai->driver->cops->startup) {
		ret = cpu_dai->driver->cops->startup(cstream, cpu_dai);
		if (ret < 0) {
			dev_err(cpu_dai->dev,
				"Compress ASoC: can't open interface %s: %d\n",
				cpu_dai->name, ret);
			goto out;
		}
	}

	ret = soc_compr_components_open(cstream, &component);
	if (ret < 0)
		goto open_err;

	if (fe->dai_link->compr_ops && fe->dai_link->compr_ops->startup) {
		ret = fe->dai_link->compr_ops->startup(cstream);
		if (ret < 0) {
			pr_err("Compress ASoC: %s startup failed: %d\n",
			       fe->dai_link->name, ret);
			goto machine_err;
		}
	}
=======
	ret = snd_soc_dai_compr_startup(cpu_dai, cstream);
	if (ret < 0)
		goto out;

	ret = snd_soc_component_compr_open(cstream);
	if (ret < 0)
		goto open_err;

	ret = snd_soc_link_compr_startup(cstream);
	if (ret < 0)
		goto machine_err;
>>>>>>> upstream/android-13

	dpcm_clear_pending_state(fe, stream);
	dpcm_path_put(&list);

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_OPEN;
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;

<<<<<<< HEAD
	snd_soc_runtime_activate(fe, stream);
=======
	mutex_lock_nested(&fe->card->pcm_mutex, fe->card->pcm_subclass);
	snd_soc_runtime_activate(fe, stream);
	mutex_unlock(&fe->card->pcm_mutex);
>>>>>>> upstream/android-13

	mutex_unlock(&fe->card->mutex);

	return 0;

machine_err:
<<<<<<< HEAD
	soc_compr_components_free(cstream, component);
open_err:
	if (cpu_dai->driver->cops && cpu_dai->driver->cops->shutdown)
		cpu_dai->driver->cops->shutdown(cstream, cpu_dai);
=======
	snd_soc_component_compr_free(cstream, 1);
open_err:
	snd_soc_dai_compr_shutdown(cpu_dai, cstream, 1);
>>>>>>> upstream/android-13
out:
	dpcm_path_put(&list);
be_err:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	mutex_unlock(&fe->card->mutex);
	return ret;
}

<<<<<<< HEAD
/*
 * Power down the audio subsystem pmdown_time msecs after close is called.
 * This is to ensure there are no pops or clicks in between any music tracks
 * due to DAPM power cycling.
 */
static void close_delayed_work(struct work_struct *work)
{
	struct snd_soc_pcm_runtime *rtd =
			container_of(work, struct snd_soc_pcm_runtime, delayed_work.work);
	struct snd_soc_dai *codec_dai = rtd->codec_dai;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	dev_dbg(rtd->dev,
		"Compress ASoC: pop wq checking: %s status: %s waiting: %s\n",
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
}

static int soc_compr_free(struct snd_compr_stream *cstream)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int stream;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (cstream->direction == SND_COMPRESS_PLAYBACK)
		stream = SNDRV_PCM_STREAM_PLAYBACK;
	else
		stream = SNDRV_PCM_STREAM_CAPTURE;

	snd_soc_runtime_deactivate(rtd, stream);

	snd_soc_dai_digital_mute(codec_dai, 1, cstream->direction);

	if (!cpu_dai->active)
		cpu_dai->rate = 0;

	if (!codec_dai->active)
		codec_dai->rate = 0;

	if (rtd->dai_link->compr_ops && rtd->dai_link->compr_ops->shutdown)
		rtd->dai_link->compr_ops->shutdown(cstream);

	soc_compr_components_free(cstream, NULL);

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->shutdown)
		cpu_dai->driver->cops->shutdown(cstream, cpu_dai);

	if (cstream->direction == SND_COMPRESS_PLAYBACK) {
		if (snd_soc_runtime_ignore_pmdown_time(rtd)) {
			snd_soc_dapm_stream_event(rtd,
						  SNDRV_PCM_STREAM_PLAYBACK,
						  SND_SOC_DAPM_STREAM_STOP);
		} else {
			rtd->pop_wait = 1;
			queue_delayed_work(system_power_efficient_wq,
					   &rtd->delayed_work,
					   msecs_to_jiffies(rtd->pmdown_time));
		}
	} else {
		/* capture streams can be powered down now */
		snd_soc_dapm_stream_event(rtd,
					  SNDRV_PCM_STREAM_CAPTURE,
					  SND_SOC_DAPM_STREAM_STOP);
	}

	mutex_unlock(&rtd->pcm_mutex);
	return 0;
}

static int soc_compr_free_fe(struct snd_compr_stream *cstream)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
	struct snd_soc_dai *cpu_dai = fe->cpu_dai;
	struct snd_soc_dpcm *dpcm;
	int stream, ret;

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);

	if (cstream->direction == SND_COMPRESS_PLAYBACK)
		stream = SNDRV_PCM_STREAM_PLAYBACK;
	else
		stream = SNDRV_PCM_STREAM_CAPTURE;

	snd_soc_runtime_deactivate(fe, stream);

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	ret = dpcm_be_dai_hw_free(fe, stream);
	if (ret < 0)
		dev_err(fe->dev, "Compressed ASoC: hw_free failed: %d\n", ret);

	ret = dpcm_be_dai_shutdown(fe, stream);

	/* mark FE's links ready to prune */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be)
=======
static int soc_compr_free_fe(struct snd_compr_stream *cstream)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(fe, 0);
	struct snd_soc_dpcm *dpcm;
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);

	mutex_lock_nested(&fe->card->pcm_mutex, fe->card->pcm_subclass);
	snd_soc_runtime_deactivate(fe, stream);
	mutex_unlock(&fe->card->pcm_mutex);

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	dpcm_be_dai_hw_free(fe, stream);

	dpcm_be_dai_shutdown(fe, stream);

	/* mark FE's links ready to prune */
	for_each_dpcm_be(fe, stream, dpcm)
>>>>>>> upstream/android-13
		dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_STOP);

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;

	dpcm_be_disconnect(fe, stream);

	fe->dpcm[stream].runtime = NULL;

<<<<<<< HEAD
	if (fe->dai_link->compr_ops && fe->dai_link->compr_ops->shutdown)
		fe->dai_link->compr_ops->shutdown(cstream);

	soc_compr_components_free(cstream, NULL);

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->shutdown)
		cpu_dai->driver->cops->shutdown(cstream, cpu_dai);
=======
	snd_soc_link_compr_shutdown(cstream, 0);

	snd_soc_component_compr_free(cstream, 0);

	snd_soc_dai_compr_shutdown(cpu_dai, cstream, 0);
>>>>>>> upstream/android-13

	mutex_unlock(&fe->card->mutex);
	return 0;
}

static int soc_compr_trigger(struct snd_compr_stream *cstream, int cmd)
{
<<<<<<< HEAD

	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret = 0, __ret;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->trigger)
			continue;

		__ret = component->driver->compr_ops->trigger(cstream, cmd);
		if (__ret < 0)
			ret = __ret;
	}
	if (ret < 0)
		goto out;

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->trigger)
		cpu_dai->driver->cops->trigger(cstream, cmd, cpu_dai);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		snd_soc_dai_digital_mute(codec_dai, 0, cstream->direction);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		snd_soc_dai_digital_mute(codec_dai, 1, cstream->direction);
=======
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_dai *codec_dai = asoc_rtd_to_codec(rtd, 0);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	ret = snd_soc_component_compr_trigger(cstream, cmd);
	if (ret < 0)
		goto out;

	ret = snd_soc_dai_compr_trigger(cpu_dai, cstream, cmd);
	if (ret < 0)
		goto out;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		snd_soc_dai_digital_mute(codec_dai, 0, stream);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		snd_soc_dai_digital_mute(codec_dai, 1, stream);
>>>>>>> upstream/android-13
		break;
	}

out:
<<<<<<< HEAD
	mutex_unlock(&rtd->pcm_mutex);
=======
	mutex_unlock(&rtd->card->pcm_mutex);
>>>>>>> upstream/android-13
	return ret;
}

static int soc_compr_trigger_fe(struct snd_compr_stream *cstream, int cmd)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
<<<<<<< HEAD
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = fe->cpu_dai;
	int ret = 0, __ret, stream;

	if (cmd == SND_COMPR_TRIGGER_PARTIAL_DRAIN ||
		cmd == SND_COMPR_TRIGGER_DRAIN) {

		for_each_rtdcom(fe, rtdcom) {
			component = rtdcom->component;

			if (!component->driver->compr_ops ||
			    !component->driver->compr_ops->trigger)
				continue;

			__ret = component->driver->compr_ops->trigger(cstream, cmd);
			if (__ret < 0)
				ret = __ret;
		}
		return ret;
	}

	if (cstream->direction == SND_COMPRESS_PLAYBACK)
		stream = SNDRV_PCM_STREAM_PLAYBACK;
	else
		stream = SNDRV_PCM_STREAM_CAPTURE;

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->trigger) {
		ret = cpu_dai->driver->cops->trigger(cstream, cmd, cpu_dai);
		if (ret < 0)
			goto out;
	}

	for_each_rtdcom(fe, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->trigger)
			continue;

		__ret = component->driver->compr_ops->trigger(cstream, cmd);
		if (__ret < 0)
			ret = __ret;
	}
=======
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(fe, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

	if (cmd == SND_COMPR_TRIGGER_PARTIAL_DRAIN ||
	    cmd == SND_COMPR_TRIGGER_DRAIN)
		return snd_soc_component_compr_trigger(cstream, cmd);

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);

	ret = snd_soc_dai_compr_trigger(cpu_dai, cstream, cmd);
	if (ret < 0)
		goto out;

	ret = snd_soc_component_compr_trigger(cstream, cmd);
>>>>>>> upstream/android-13
	if (ret < 0)
		goto out;

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	ret = dpcm_be_dai_trigger(fe, stream, cmd);

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
	mutex_unlock(&fe->card->mutex);
	return ret;
}

static int soc_compr_set_params(struct snd_compr_stream *cstream,
<<<<<<< HEAD
					struct snd_compr_params *params)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret = 0, __ret;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);
=======
				struct snd_compr_params *params)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);
>>>>>>> upstream/android-13

	/*
	 * First we call set_params for the CPU DAI, then the component
	 * driver this should configure the SoC side. If the machine has
	 * compressed ops then we call that as well. The expectation is
	 * that these callbacks will configure everything for this compress
	 * path, like configuring a PCM port for a CODEC.
	 */
<<<<<<< HEAD
	if (cpu_dai->driver->cops && cpu_dai->driver->cops->set_params) {
		ret = cpu_dai->driver->cops->set_params(cstream, params, cpu_dai);
		if (ret < 0)
			goto err;
	}

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->set_params)
			continue;

		__ret = component->driver->compr_ops->set_params(cstream, params);
		if (__ret < 0)
			ret = __ret;
	}
	if (ret < 0)
		goto err;

	if (rtd->dai_link->compr_ops && rtd->dai_link->compr_ops->set_params) {
		ret = rtd->dai_link->compr_ops->set_params(cstream);
		if (ret < 0)
			goto err;
	}

	if (cstream->direction == SND_COMPRESS_PLAYBACK)
		snd_soc_dapm_stream_event(rtd, SNDRV_PCM_STREAM_PLAYBACK,
					  SND_SOC_DAPM_STREAM_START);
	else
		snd_soc_dapm_stream_event(rtd, SNDRV_PCM_STREAM_CAPTURE,
					  SND_SOC_DAPM_STREAM_START);

	/* cancel any delayed stream shutdown that is pending */
	rtd->pop_wait = 0;
	mutex_unlock(&rtd->pcm_mutex);

	cancel_delayed_work_sync(&rtd->delayed_work);

	return ret;

err:
	mutex_unlock(&rtd->pcm_mutex);
=======
	ret = snd_soc_dai_compr_set_params(cpu_dai, cstream, params);
	if (ret < 0)
		goto err;

	ret = snd_soc_component_compr_set_params(cstream, params);
	if (ret < 0)
		goto err;

	ret = snd_soc_link_compr_set_params(cstream);
	if (ret < 0)
		goto err;

	snd_soc_dapm_stream_event(rtd, stream, SND_SOC_DAPM_STREAM_START);

	/* cancel any delayed stream shutdown that is pending */
	rtd->pop_wait = 0;
	mutex_unlock(&rtd->card->pcm_mutex);

	cancel_delayed_work_sync(&rtd->delayed_work);

	return 0;

err:
	mutex_unlock(&rtd->card->pcm_mutex);
>>>>>>> upstream/android-13
	return ret;
}

static int soc_compr_set_params_fe(struct snd_compr_stream *cstream,
				   struct snd_compr_params *params)
{
	struct snd_soc_pcm_runtime *fe = cstream->private_data;
	struct snd_pcm_substream *fe_substream =
		 fe->pcm->streams[cstream->direction].substream;
<<<<<<< HEAD
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = fe->cpu_dai;
	int ret = 0, __ret, stream;

	if (cstream->direction == SND_COMPRESS_PLAYBACK)
		stream = SNDRV_PCM_STREAM_PLAYBACK;
	else
		stream = SNDRV_PCM_STREAM_CAPTURE;
=======
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(fe, 0);
	int stream = cstream->direction; /* SND_COMPRESS_xxx is same as SNDRV_PCM_STREAM_xxx */
	int ret;
>>>>>>> upstream/android-13

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);

	/*
	 * Create an empty hw_params for the BE as the machine driver must
	 * fix this up to match DSP decoder and ASRC configuration.
	 * I.e. machine driver fixup for compressed BE is mandatory.
	 */
	memset(&fe->dpcm[fe_substream->stream].hw_params, 0,
		sizeof(struct snd_pcm_hw_params));

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	ret = dpcm_be_dai_hw_params(fe, stream);
	if (ret < 0)
		goto out;

	ret = dpcm_be_dai_prepare(fe, stream);
	if (ret < 0)
		goto out;

<<<<<<< HEAD
	if (cpu_dai->driver->cops && cpu_dai->driver->cops->set_params) {
		ret = cpu_dai->driver->cops->set_params(cstream, params, cpu_dai);
		if (ret < 0)
			goto out;
	}

	for_each_rtdcom(fe, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->set_params)
			continue;

		__ret = component->driver->compr_ops->set_params(cstream, params);
		if (__ret < 0)
			ret = __ret;
	}
	if (ret < 0)
		goto out;

	if (fe->dai_link->compr_ops && fe->dai_link->compr_ops->set_params) {
		ret = fe->dai_link->compr_ops->set_params(cstream);
		if (ret < 0)
			goto out;
	}
=======
	ret = snd_soc_dai_compr_set_params(cpu_dai, cstream, params);
	if (ret < 0)
		goto out;

	ret = snd_soc_component_compr_set_params(cstream, params);
	if (ret < 0)
		goto out;

	ret = snd_soc_link_compr_set_params(cstream);
	if (ret < 0)
		goto out;
>>>>>>> upstream/android-13

	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_START);
	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_PREPARE;

out:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	mutex_unlock(&fe->card->mutex);
	return ret;
}

static int soc_compr_get_params(struct snd_compr_stream *cstream,
				struct snd_codec *params)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
<<<<<<< HEAD
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret = 0, __ret;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->get_params) {
		ret = cpu_dai->driver->cops->get_params(cstream, params, cpu_dai);
		if (ret < 0)
			goto err;
	}

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->get_params)
			continue;

		__ret = component->driver->compr_ops->get_params(cstream, params);
		if (__ret < 0)
			ret = __ret;
	}

err:
	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

static int soc_compr_get_caps(struct snd_compr_stream *cstream,
			      struct snd_compr_caps *caps)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	int ret = 0, __ret;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->get_caps)
			continue;

		__ret = component->driver->compr_ops->get_caps(cstream, caps);
		if (__ret < 0)
			ret = __ret;
	}

	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

static int soc_compr_get_codec_caps(struct snd_compr_stream *cstream,
				    struct snd_compr_codec_caps *codec)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	int ret = 0, __ret;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->get_codec_caps)
			continue;

		__ret = component->driver->compr_ops->get_codec_caps(cstream, codec);
		if (__ret < 0)
			ret = __ret;
	}

	mutex_unlock(&rtd->pcm_mutex);
=======
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
	int ret = 0;

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	ret = snd_soc_dai_compr_get_params(cpu_dai, cstream, params);
	if (ret < 0)
		goto err;

	ret = snd_soc_component_compr_get_params(cstream, params);
err:
	mutex_unlock(&rtd->card->pcm_mutex);
>>>>>>> upstream/android-13
	return ret;
}

static int soc_compr_ack(struct snd_compr_stream *cstream, size_t bytes)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
<<<<<<< HEAD
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret = 0, __ret;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->ack) {
		ret = cpu_dai->driver->cops->ack(cstream, bytes, cpu_dai);
		if (ret < 0)
			goto err;
	}

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->ack)
			continue;

		__ret = component->driver->compr_ops->ack(cstream, bytes);
		if (__ret < 0)
			ret = __ret;
	}

err:
	mutex_unlock(&rtd->pcm_mutex);
=======
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
	int ret;

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	ret = snd_soc_dai_compr_ack(cpu_dai, cstream, bytes);
	if (ret < 0)
		goto err;

	ret = snd_soc_component_compr_ack(cstream, bytes);
err:
	mutex_unlock(&rtd->card->pcm_mutex);
>>>>>>> upstream/android-13
	return ret;
}

static int soc_compr_pointer(struct snd_compr_stream *cstream,
			     struct snd_compr_tstamp *tstamp)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
<<<<<<< HEAD
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	int ret = 0, __ret;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->pointer)
		cpu_dai->driver->cops->pointer(cstream, tstamp, cpu_dai);

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->pointer)
			continue;

		__ret = component->driver->compr_ops->pointer(cstream, tstamp);
		if (__ret < 0)
			ret = __ret;
	}

	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

static int soc_compr_copy(struct snd_compr_stream *cstream,
			  char __user *buf, size_t count)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->copy)
			continue;

		ret = component->driver->compr_ops->copy(cstream, buf, count);
		break;
	}

	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

static int sst_compr_set_next_track_param(struct snd_compr_stream *cstream,
				union snd_codec_options *codec_options)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	int ret = 0;

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (component->driver->compr_ops &&
			component->driver->compr_ops->set_next_track_param)
			ret =
			component->driver->compr_ops->set_next_track_param(
				cstream, codec_options);
	}

	return ret;
}


=======
	int ret;
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);

	mutex_lock_nested(&rtd->card->pcm_mutex, rtd->card->pcm_subclass);

	ret = snd_soc_dai_compr_pointer(cpu_dai, cstream, tstamp);
	if (ret < 0)
		goto out;

	ret = snd_soc_component_compr_pointer(cstream, tstamp);
out:
	mutex_unlock(&rtd->card->pcm_mutex);
	return ret;
}

>>>>>>> upstream/android-13
static int soc_compr_set_metadata(struct snd_compr_stream *cstream,
				  struct snd_compr_metadata *metadata)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
<<<<<<< HEAD
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret = 0, __ret;

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->set_metadata) {
		ret = cpu_dai->driver->cops->set_metadata(cstream, metadata, cpu_dai);
		if (ret < 0)
			return ret;
	}

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->set_metadata)
			continue;

		__ret = component->driver->compr_ops->set_metadata(cstream, metadata);
		if (__ret < 0)
			ret = __ret;
	}

	return ret;
=======
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
	int ret;

	ret = snd_soc_dai_compr_set_metadata(cpu_dai, cstream, metadata);
	if (ret < 0)
		return ret;

	return snd_soc_component_compr_set_metadata(cstream, metadata);
>>>>>>> upstream/android-13
}

static int soc_compr_get_metadata(struct snd_compr_stream *cstream,
				  struct snd_compr_metadata *metadata)
{
	struct snd_soc_pcm_runtime *rtd = cstream->private_data;
<<<<<<< HEAD
	struct snd_soc_component *component;
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret = 0, __ret;

	if (cpu_dai->driver->cops && cpu_dai->driver->cops->get_metadata) {
		ret = cpu_dai->driver->cops->get_metadata(cstream, metadata, cpu_dai);
		if (ret < 0)
			return ret;
	}

	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->get_metadata)
			continue;

		__ret = component->driver->compr_ops->get_metadata(cstream, metadata);
		if (__ret < 0)
			ret = __ret;
	}

	return ret;
=======
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
	int ret;

	ret = snd_soc_dai_compr_get_metadata(cpu_dai, cstream, metadata);
	if (ret < 0)
		return ret;

	return snd_soc_component_compr_get_metadata(cstream, metadata);
>>>>>>> upstream/android-13
}

/* ASoC Compress operations */
static struct snd_compr_ops soc_compr_ops = {
	.open		= soc_compr_open,
	.free		= soc_compr_free,
	.set_params	= soc_compr_set_params,
	.set_metadata   = soc_compr_set_metadata,
<<<<<<< HEAD
	.set_next_track_param	= sst_compr_set_next_track_param,
=======
>>>>>>> upstream/android-13
	.get_metadata	= soc_compr_get_metadata,
	.get_params	= soc_compr_get_params,
	.trigger	= soc_compr_trigger,
	.pointer	= soc_compr_pointer,
	.ack		= soc_compr_ack,
<<<<<<< HEAD
	.get_caps	= soc_compr_get_caps,
	.get_codec_caps = soc_compr_get_codec_caps
=======
	.get_caps	= snd_soc_component_compr_get_caps,
	.get_codec_caps = snd_soc_component_compr_get_codec_caps,
>>>>>>> upstream/android-13
};

/* ASoC Dynamic Compress operations */
static struct snd_compr_ops soc_compr_dyn_ops = {
	.open		= soc_compr_open_fe,
	.free		= soc_compr_free_fe,
	.set_params	= soc_compr_set_params_fe,
	.get_params	= soc_compr_get_params,
	.set_metadata   = soc_compr_set_metadata,
<<<<<<< HEAD
	.set_next_track_param	= sst_compr_set_next_track_param,
=======
>>>>>>> upstream/android-13
	.get_metadata	= soc_compr_get_metadata,
	.trigger	= soc_compr_trigger_fe,
	.pointer	= soc_compr_pointer,
	.ack		= soc_compr_ack,
<<<<<<< HEAD
	.get_caps	= soc_compr_get_caps,
	.get_codec_caps = soc_compr_get_codec_caps
=======
	.get_caps	= snd_soc_component_compr_get_caps,
	.get_codec_caps = snd_soc_component_compr_get_codec_caps,
>>>>>>> upstream/android-13
};

/**
 * snd_soc_new_compress - create a new compress.
 *
 * @rtd: The runtime for which we will create compress
 * @num: the device index number (zero based - shared with normal PCMs)
 *
 * Return: 0 for success, else error.
 */
int snd_soc_new_compress(struct snd_soc_pcm_runtime *rtd, int num)
{
	struct snd_soc_component *component;
<<<<<<< HEAD
	struct snd_soc_rtdcom_list *rtdcom;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
=======
	struct snd_soc_dai *codec_dai = asoc_rtd_to_codec(rtd, 0);
	struct snd_soc_dai *cpu_dai = asoc_rtd_to_cpu(rtd, 0);
>>>>>>> upstream/android-13
	struct snd_compr *compr;
	struct snd_pcm *be_pcm;
	char new_name[64];
	int ret = 0, direction = 0;
	int playback = 0, capture = 0;
<<<<<<< HEAD

	if (rtd->num_codecs > 1) {
		dev_err(rtd->card->dev,
			"Compress ASoC: Multicodec not supported\n");
=======
	int i;

	/*
	 * make sure these are same value,
	 * and then use these as equally
	 */
	BUILD_BUG_ON((int)SNDRV_PCM_STREAM_PLAYBACK != (int)SND_COMPRESS_PLAYBACK);
	BUILD_BUG_ON((int)SNDRV_PCM_STREAM_CAPTURE  != (int)SND_COMPRESS_CAPTURE);

	if (rtd->num_cpus > 1 ||
	    rtd->num_codecs > 1) {
		dev_err(rtd->card->dev,
			"Compress ASoC: Multi CPU/Codec not supported\n");
		return -EINVAL;
	}

	if (!codec_dai) {
		dev_err(rtd->card->dev, "Missing codec\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	/* check client and interface hw capabilities */
<<<<<<< HEAD
	if (codec_dai->driver->playback.channels_min)
		playback = 1;
	if (codec_dai->driver->capture.channels_min)
		capture = 1;

	capture = capture && cpu_dai->driver->capture.channels_min;
	playback = playback && cpu_dai->driver->playback.channels_min;

=======
	if (snd_soc_dai_stream_valid(codec_dai, SNDRV_PCM_STREAM_PLAYBACK) &&
	    snd_soc_dai_stream_valid(cpu_dai,   SNDRV_PCM_STREAM_PLAYBACK))
		playback = 1;
	if (snd_soc_dai_stream_valid(codec_dai, SNDRV_PCM_STREAM_CAPTURE) &&
	    snd_soc_dai_stream_valid(cpu_dai,   SNDRV_PCM_STREAM_CAPTURE))
		capture = 1;

>>>>>>> upstream/android-13
	/*
	 * Compress devices are unidirectional so only one of the directions
	 * should be set, check for that (xor)
	 */
	if (playback + capture != 1) {
		dev_err(rtd->card->dev,
			"Compress ASoC: Invalid direction for P %d, C %d\n",
			playback, capture);
		return -EINVAL;
	}

	if (playback)
		direction = SND_COMPRESS_PLAYBACK;
	else
		direction = SND_COMPRESS_CAPTURE;

<<<<<<< HEAD
	compr = kzalloc(sizeof(*compr), GFP_KERNEL);
=======
	compr = devm_kzalloc(rtd->card->dev, sizeof(*compr), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!compr)
		return -ENOMEM;

	compr->ops = devm_kzalloc(rtd->card->dev, sizeof(soc_compr_ops),
				  GFP_KERNEL);
<<<<<<< HEAD
	if (!compr->ops) {
		ret = -ENOMEM;
		goto compr_err;
	}
=======
	if (!compr->ops)
		return -ENOMEM;
>>>>>>> upstream/android-13

	if (rtd->dai_link->dynamic) {
		snprintf(new_name, sizeof(new_name), "(%s)",
			rtd->dai_link->stream_name);

		ret = snd_pcm_new_internal(rtd->card->snd_card, new_name, num,
				rtd->dai_link->dpcm_playback,
				rtd->dai_link->dpcm_capture, &be_pcm);
		if (ret < 0) {
			dev_err(rtd->card->dev,
				"Compress ASoC: can't create compressed for %s: %d\n",
				rtd->dai_link->name, ret);
<<<<<<< HEAD
			goto compr_err;
=======
			return ret;
>>>>>>> upstream/android-13
		}

		rtd->pcm = be_pcm;
		rtd->fe_compr = 1;
		if (rtd->dai_link->dpcm_playback)
			be_pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream->private_data = rtd;
		else if (rtd->dai_link->dpcm_capture)
			be_pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream->private_data = rtd;
		memcpy(compr->ops, &soc_compr_dyn_ops, sizeof(soc_compr_dyn_ops));
	} else {
		snprintf(new_name, sizeof(new_name), "%s %s-%d",
			rtd->dai_link->stream_name, codec_dai->name, num);

		memcpy(compr->ops, &soc_compr_ops, sizeof(soc_compr_ops));
	}

<<<<<<< HEAD
	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (!component->driver->compr_ops ||
		    !component->driver->compr_ops->copy)
			continue;

		compr->ops->copy = soc_compr_copy;
		break;
	}

	mutex_init(&compr->lock);
	ret = snd_compress_new(rtd->card->snd_card, num, direction,
				new_name, compr);
	if (ret < 0) {
		component = rtd->codec_dai->component;
		dev_err(component->dev,
			"Compress ASoC: can't create compress for codec %s: %d\n",
			component->name, ret);
		goto compr_err;
	}

	/* DAPM dai link stream work */
	INIT_DELAYED_WORK(&rtd->delayed_work, close_delayed_work);
=======
	for_each_rtd_components(rtd, i, component) {
		if (!component->driver->compress_ops ||
		    !component->driver->compress_ops->copy)
			continue;

		compr->ops->copy = snd_soc_component_compr_copy;
		break;
	}

	ret = snd_compress_new(rtd->card->snd_card, num, direction,
				new_name, compr);
	if (ret < 0) {
		component = asoc_rtd_to_codec(rtd, 0)->component;
		dev_err(component->dev,
			"Compress ASoC: can't create compress for codec %s: %d\n",
			component->name, ret);
		return ret;
	}

	/* DAPM dai link stream work */
	rtd->close_delayed_work_func = snd_soc_close_delayed_work;
>>>>>>> upstream/android-13

	rtd->compr = compr;
	compr->private_data = rtd;

<<<<<<< HEAD
	for_each_rtdcom(rtd, rtdcom) {
		component = rtdcom->component;

		if (component->driver->pcm_new) {
			ret = component->driver->pcm_new(rtd);
			if (ret < 0) {
				pr_err("asoc: compress pcm constructor failed\n");
				goto compr_err;
			}
		}
	}

	dev_info(rtd->card->dev, "Compress ASoC: %s <-> %s mapping ok\n",
		 codec_dai->name, cpu_dai->name);

	return ret;

compr_err:
	kfree(compr);
	return ret;
=======
	dev_dbg(rtd->card->dev, "Compress ASoC: %s <-> %s mapping ok\n",
		codec_dai->name, cpu_dai->name);

	return 0;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(snd_soc_new_compress);
