// SPDX-License-Identifier: GPL-2.0
/*
 * APBridge ALSA SoC dummy codec driver
 * Copyright 2016 Google Inc.
 * Copyright 2016 Linaro Ltd.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>
#include <uapi/linux/input.h>

#include "audio_codec.h"
#include "audio_apbridgea.h"
#include "audio_manager.h"
<<<<<<< HEAD
=======
#include "audio_helper.h"
>>>>>>> upstream/android-13

static struct gbaudio_codec_info *gbcodec;

static struct gbaudio_data_connection *
find_data(struct gbaudio_module_info *module, int id)
{
	struct gbaudio_data_connection *data;

	list_for_each_entry(data, &module->data_list, list) {
		if (id == data->id)
			return data;
	}
	return NULL;
}

static struct gbaudio_stream_params *
find_dai_stream_params(struct gbaudio_codec_info *codec, int id, int stream)
{
	struct gbaudio_codec_dai *dai;

	list_for_each_entry(dai, &codec->dai_list, list) {
		if (dai->id == id)
			return &dai->params[stream];
	}
	return NULL;
}

static int gbaudio_module_enable_tx(struct gbaudio_codec_info *codec,
				    struct gbaudio_module_info *module, int id)
{
	int module_state, ret = 0;
	u16 data_cport, i2s_port, cportid;
	u8 sig_bits, channels;
	u32 format, rate;
	struct gbaudio_data_connection *data;
	struct gbaudio_stream_params *params;

	/* find the dai */
	data = find_data(module, id);
	if (!data) {
		dev_err(module->dev, "%d:DATA connection missing\n", id);
		return -ENODEV;
	}
	module_state = data->state[SNDRV_PCM_STREAM_PLAYBACK];

	params = find_dai_stream_params(codec, id, SNDRV_PCM_STREAM_PLAYBACK);
	if (!params) {
		dev_err(codec->dev, "Failed to fetch dai_stream pointer\n");
		return -EINVAL;
	}

	/* register cport */
	if (module_state < GBAUDIO_CODEC_STARTUP) {
		i2s_port = 0;	/* fixed for now */
		cportid = data->connection->hd_cport_id;
		ret = gb_audio_apbridgea_register_cport(data->connection,
<<<<<<< HEAD
						i2s_port, cportid,
						AUDIO_APBRIDGEA_DIRECTION_TX);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "reg_cport failed:%d\n", ret);
			return ret;
		}
		data->state[SNDRV_PCM_STREAM_PLAYBACK] =
			GBAUDIO_CODEC_STARTUP;
=======
							i2s_port, cportid,
							AUDIO_APBRIDGEA_DIRECTION_TX);
		if (ret) {
			dev_err_ratelimited(module->dev, "reg_cport failed:%d\n", ret);
			return ret;
		}
		data->state[SNDRV_PCM_STREAM_PLAYBACK] = GBAUDIO_CODEC_STARTUP;
>>>>>>> upstream/android-13
		dev_dbg(module->dev, "Dynamic Register %d DAI\n", cportid);
	}

	/* hw_params */
	if (module_state < GBAUDIO_CODEC_HWPARAMS) {
		format = params->format;
		channels = params->channels;
		rate = params->rate;
		sig_bits = params->sig_bits;
		data_cport = data->connection->intf_cport_id;
		ret = gb_audio_gb_set_pcm(module->mgmt_connection, data_cport,
					  format, rate, channels, sig_bits);
		if (ret) {
<<<<<<< HEAD
			dev_err_ratelimited(module->dev, "set_pcm failed:%d\n",
					    ret);
			return ret;
		}
		data->state[SNDRV_PCM_STREAM_PLAYBACK] =
			GBAUDIO_CODEC_HWPARAMS;
=======
			dev_err_ratelimited(module->dev, "set_pcm failed:%d\n", ret);
			return ret;
		}
		data->state[SNDRV_PCM_STREAM_PLAYBACK] = GBAUDIO_CODEC_HWPARAMS;
>>>>>>> upstream/android-13
		dev_dbg(module->dev, "Dynamic hw_params %d DAI\n", data_cport);
	}

	/* prepare */
	if (module_state < GBAUDIO_CODEC_PREPARE) {
		data_cport = data->connection->intf_cport_id;
		ret = gb_audio_gb_set_tx_data_size(module->mgmt_connection,
						   data_cport, 192);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "set_tx_data_size failed:%d\n",
					    ret);
			return ret;
		}
<<<<<<< HEAD
		ret = gb_audio_gb_activate_tx(module->mgmt_connection,
					      data_cport);
=======
		ret = gb_audio_gb_activate_tx(module->mgmt_connection, data_cport);
>>>>>>> upstream/android-13
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "activate_tx failed:%d\n", ret);
			return ret;
		}
<<<<<<< HEAD
		data->state[SNDRV_PCM_STREAM_PLAYBACK] =
			GBAUDIO_CODEC_PREPARE;
=======
		data->state[SNDRV_PCM_STREAM_PLAYBACK] = GBAUDIO_CODEC_PREPARE;
>>>>>>> upstream/android-13
		dev_dbg(module->dev, "Dynamic prepare %d DAI\n", data_cport);
	}

	return 0;
}

static int gbaudio_module_disable_tx(struct gbaudio_module_info *module, int id)
{
	int ret;
	u16 data_cport, cportid, i2s_port;
	int module_state;
	struct gbaudio_data_connection *data;

	/* find the dai */
	data = find_data(module, id);
	if (!data) {
		dev_err(module->dev, "%d:DATA connection missing\n", id);
		return -ENODEV;
	}
	module_state = data->state[SNDRV_PCM_STREAM_PLAYBACK];

	if (module_state > GBAUDIO_CODEC_HWPARAMS) {
		data_cport = data->connection->intf_cport_id;
		ret = gb_audio_gb_deactivate_tx(module->mgmt_connection,
						data_cport);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "deactivate_tx failed:%d\n", ret);
			return ret;
		}
		dev_dbg(module->dev, "Dynamic deactivate %d DAI\n", data_cport);
<<<<<<< HEAD
		data->state[SNDRV_PCM_STREAM_PLAYBACK] =
			GBAUDIO_CODEC_HWPARAMS;
=======
		data->state[SNDRV_PCM_STREAM_PLAYBACK] = GBAUDIO_CODEC_HWPARAMS;
>>>>>>> upstream/android-13
	}

	if (module_state > GBAUDIO_CODEC_SHUTDOWN) {
		i2s_port = 0;	/* fixed for now */
		cportid = data->connection->hd_cport_id;
		ret = gb_audio_apbridgea_unregister_cport(data->connection,
<<<<<<< HEAD
						i2s_port, cportid,
						AUDIO_APBRIDGEA_DIRECTION_TX);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "unregister_cport failed:%d\n",
					    ret);
			return ret;
		}
		dev_dbg(module->dev, "Dynamic Unregister %d DAI\n", cportid);
		data->state[SNDRV_PCM_STREAM_PLAYBACK] =
			GBAUDIO_CODEC_SHUTDOWN;
=======
							  i2s_port, cportid,
							  AUDIO_APBRIDGEA_DIRECTION_TX);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "unregister_cport failed:%d\n", ret);
			return ret;
		}
		dev_dbg(module->dev, "Dynamic Unregister %d DAI\n", cportid);
		data->state[SNDRV_PCM_STREAM_PLAYBACK] = GBAUDIO_CODEC_SHUTDOWN;
>>>>>>> upstream/android-13
	}

	return 0;
}

static int gbaudio_module_enable_rx(struct gbaudio_codec_info *codec,
				    struct gbaudio_module_info *module, int id)
{
	int module_state, ret = 0;
	u16 data_cport, i2s_port, cportid;
	u8 sig_bits, channels;
	u32 format, rate;
	struct gbaudio_data_connection *data;
	struct gbaudio_stream_params *params;

	/* find the dai */
	data = find_data(module, id);
	if (!data) {
		dev_err(module->dev, "%d:DATA connection missing\n", id);
		return -ENODEV;
	}
	module_state = data->state[SNDRV_PCM_STREAM_CAPTURE];

	params = find_dai_stream_params(codec, id, SNDRV_PCM_STREAM_CAPTURE);
	if (!params) {
		dev_err(codec->dev, "Failed to fetch dai_stream pointer\n");
		return -EINVAL;
	}

	/* register cport */
	if (module_state < GBAUDIO_CODEC_STARTUP) {
		i2s_port = 0;	/* fixed for now */
		cportid = data->connection->hd_cport_id;
		ret = gb_audio_apbridgea_register_cport(data->connection,
<<<<<<< HEAD
						i2s_port, cportid,
						AUDIO_APBRIDGEA_DIRECTION_RX);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "reg_cport failed:%d\n", ret);
			return ret;
		}
		data->state[SNDRV_PCM_STREAM_CAPTURE] =
			GBAUDIO_CODEC_STARTUP;
=======
							i2s_port, cportid,
							AUDIO_APBRIDGEA_DIRECTION_RX);
		if (ret) {
			dev_err_ratelimited(module->dev, "reg_cport failed:%d\n", ret);
			return ret;
		}
		data->state[SNDRV_PCM_STREAM_CAPTURE] = GBAUDIO_CODEC_STARTUP;
>>>>>>> upstream/android-13
		dev_dbg(module->dev, "Dynamic Register %d DAI\n", cportid);
	}

	/* hw_params */
	if (module_state < GBAUDIO_CODEC_HWPARAMS) {
		format = params->format;
		channels = params->channels;
		rate = params->rate;
		sig_bits = params->sig_bits;
		data_cport = data->connection->intf_cport_id;
		ret = gb_audio_gb_set_pcm(module->mgmt_connection, data_cport,
					  format, rate, channels, sig_bits);
		if (ret) {
<<<<<<< HEAD
			dev_err_ratelimited(module->dev, "set_pcm failed:%d\n",
					    ret);
			return ret;
		}
		data->state[SNDRV_PCM_STREAM_CAPTURE] =
			GBAUDIO_CODEC_HWPARAMS;
=======
			dev_err_ratelimited(module->dev, "set_pcm failed:%d\n", ret);
			return ret;
		}
		data->state[SNDRV_PCM_STREAM_CAPTURE] = GBAUDIO_CODEC_HWPARAMS;
>>>>>>> upstream/android-13
		dev_dbg(module->dev, "Dynamic hw_params %d DAI\n", data_cport);
	}

	/* prepare */
	if (module_state < GBAUDIO_CODEC_PREPARE) {
		data_cport = data->connection->intf_cport_id;
		ret = gb_audio_gb_set_rx_data_size(module->mgmt_connection,
						   data_cport, 192);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "set_rx_data_size failed:%d\n",
					    ret);
			return ret;
		}
		ret = gb_audio_gb_activate_rx(module->mgmt_connection,
					      data_cport);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "activate_rx failed:%d\n", ret);
			return ret;
		}
<<<<<<< HEAD
		data->state[SNDRV_PCM_STREAM_CAPTURE] =
			GBAUDIO_CODEC_PREPARE;
=======
		data->state[SNDRV_PCM_STREAM_CAPTURE] = GBAUDIO_CODEC_PREPARE;
>>>>>>> upstream/android-13
		dev_dbg(module->dev, "Dynamic prepare %d DAI\n", data_cport);
	}

	return 0;
}

static int gbaudio_module_disable_rx(struct gbaudio_module_info *module, int id)
{
	int ret;
	u16 data_cport, cportid, i2s_port;
	int module_state;
	struct gbaudio_data_connection *data;

	/* find the dai */
	data = find_data(module, id);
	if (!data) {
		dev_err(module->dev, "%d:DATA connection missing\n", id);
		return -ENODEV;
	}
	module_state = data->state[SNDRV_PCM_STREAM_CAPTURE];

	if (module_state > GBAUDIO_CODEC_HWPARAMS) {
		data_cport = data->connection->intf_cport_id;
		ret = gb_audio_gb_deactivate_rx(module->mgmt_connection,
						data_cport);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "deactivate_rx failed:%d\n", ret);
			return ret;
		}
		dev_dbg(module->dev, "Dynamic deactivate %d DAI\n", data_cport);
<<<<<<< HEAD
		data->state[SNDRV_PCM_STREAM_CAPTURE] =
			GBAUDIO_CODEC_HWPARAMS;
=======
		data->state[SNDRV_PCM_STREAM_CAPTURE] = GBAUDIO_CODEC_HWPARAMS;
>>>>>>> upstream/android-13
	}

	if (module_state > GBAUDIO_CODEC_SHUTDOWN) {
		i2s_port = 0;	/* fixed for now */
		cportid = data->connection->hd_cport_id;
		ret = gb_audio_apbridgea_unregister_cport(data->connection,
<<<<<<< HEAD
						i2s_port, cportid,
						AUDIO_APBRIDGEA_DIRECTION_RX);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "unregister_cport failed:%d\n",
					    ret);
			return ret;
		}
		dev_dbg(module->dev, "Dynamic Unregister %d DAI\n", cportid);
		data->state[SNDRV_PCM_STREAM_CAPTURE] =
			GBAUDIO_CODEC_SHUTDOWN;
=======
							  i2s_port, cportid,
							  AUDIO_APBRIDGEA_DIRECTION_RX);
		if (ret) {
			dev_err_ratelimited(module->dev,
					    "unregister_cport failed:%d\n", ret);
			return ret;
		}
		dev_dbg(module->dev, "Dynamic Unregister %d DAI\n", cportid);
		data->state[SNDRV_PCM_STREAM_CAPTURE] = GBAUDIO_CODEC_SHUTDOWN;
>>>>>>> upstream/android-13
	}

	return 0;
}

int gbaudio_module_update(struct gbaudio_codec_info *codec,
			  struct snd_soc_dapm_widget *w,
			  struct gbaudio_module_info *module, int enable)
{
	int dai_id, ret;
	char intf_name[NAME_SIZE], dir[NAME_SIZE];

	dev_dbg(module->dev, "%s:Module update %s sequence\n", w->name,
		enable ? "Enable" : "Disable");

	if ((w->id != snd_soc_dapm_aif_in) && (w->id != snd_soc_dapm_aif_out)) {
		dev_dbg(codec->dev, "No action required for %s\n", w->name);
		return 0;
	}

	/* parse dai_id from AIF widget's stream_name */
	ret = sscanf(w->sname, "%s %d %s", intf_name, &dai_id, dir);
	if (ret < 3) {
<<<<<<< HEAD
		dev_err(codec->dev, "Error while parsing dai_id for %s\n",
			w->name);
=======
		dev_err(codec->dev, "Error while parsing dai_id for %s\n", w->name);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	mutex_lock(&codec->lock);
	if (w->id == snd_soc_dapm_aif_in) {
		if (enable)
			ret = gbaudio_module_enable_tx(codec, module, dai_id);
		else
			ret = gbaudio_module_disable_tx(module, dai_id);
	} else if (w->id == snd_soc_dapm_aif_out) {
		if (enable)
			ret = gbaudio_module_enable_rx(codec, module, dai_id);
		else
			ret = gbaudio_module_disable_rx(module, dai_id);
	}

	mutex_unlock(&codec->lock);

	return ret;
}
EXPORT_SYMBOL(gbaudio_module_update);

/*
 * codec DAI ops
 */
static int gbcodec_startup(struct snd_pcm_substream *substream,
			   struct snd_soc_dai *dai)
{
	struct gbaudio_codec_info *codec = dev_get_drvdata(dai->dev);
	struct gbaudio_stream_params *params;

	mutex_lock(&codec->lock);

	if (list_empty(&codec->module_list)) {
		dev_err(codec->dev, "No codec module available\n");
		mutex_unlock(&codec->lock);
		return -ENODEV;
	}

	params = find_dai_stream_params(codec, dai->id, substream->stream);
	if (!params) {
		dev_err(codec->dev, "Failed to fetch dai_stream pointer\n");
		mutex_unlock(&codec->lock);
		return -EINVAL;
	}
	params->state = GBAUDIO_CODEC_STARTUP;
	mutex_unlock(&codec->lock);
	/* to prevent suspend in case of active audio */
	pm_stay_awake(dai->dev);

	return 0;
}

static void gbcodec_shutdown(struct snd_pcm_substream *substream,
			     struct snd_soc_dai *dai)
{
	struct gbaudio_codec_info *codec = dev_get_drvdata(dai->dev);
	struct gbaudio_stream_params *params;

	mutex_lock(&codec->lock);

	if (list_empty(&codec->module_list))
		dev_info(codec->dev, "No codec module available during shutdown\n");

	params = find_dai_stream_params(codec, dai->id, substream->stream);
	if (!params) {
		dev_err(codec->dev, "Failed to fetch dai_stream pointer\n");
		mutex_unlock(&codec->lock);
		return;
	}
	params->state = GBAUDIO_CODEC_SHUTDOWN;
	mutex_unlock(&codec->lock);
	pm_relax(dai->dev);
}

static int gbcodec_hw_params(struct snd_pcm_substream *substream,
			     struct snd_pcm_hw_params *hwparams,
			     struct snd_soc_dai *dai)
{
	int ret;
	u8 sig_bits, channels;
	u32 format, rate;
	struct gbaudio_module_info *module;
	struct gbaudio_data_connection *data;
	struct gb_bundle *bundle;
	struct gbaudio_codec_info *codec = dev_get_drvdata(dai->dev);
	struct gbaudio_stream_params *params;

	mutex_lock(&codec->lock);

	if (list_empty(&codec->module_list)) {
		dev_err(codec->dev, "No codec module available\n");
		mutex_unlock(&codec->lock);
		return -ENODEV;
	}

	/*
	 * assuming, currently only 48000 Hz, 16BIT_LE, stereo
	 * is supported, validate params before configuring codec
	 */
	if (params_channels(hwparams) != 2) {
		dev_err(dai->dev, "Invalid channel count:%d\n",
			params_channels(hwparams));
		mutex_unlock(&codec->lock);
		return -EINVAL;
	}
	channels = params_channels(hwparams);

	if (params_rate(hwparams) != 48000) {
		dev_err(dai->dev, "Invalid sampling rate:%d\n",
			params_rate(hwparams));
		mutex_unlock(&codec->lock);
		return -EINVAL;
	}
	rate = GB_AUDIO_PCM_RATE_48000;

	if (params_format(hwparams) != SNDRV_PCM_FORMAT_S16_LE) {
<<<<<<< HEAD
		dev_err(dai->dev, "Invalid format:%d\n",
			params_format(hwparams));
=======
		dev_err(dai->dev, "Invalid format:%d\n", params_format(hwparams));
>>>>>>> upstream/android-13
		mutex_unlock(&codec->lock);
		return -EINVAL;
	}
	format = GB_AUDIO_PCM_FMT_S16_LE;

	/* find the data connection */
	list_for_each_entry(module, &codec->module_list, list) {
		data = find_data(module, dai->id);
		if (data)
			break;
	}

	if (!data) {
		dev_err(dai->dev, "DATA connection missing\n");
		mutex_unlock(&codec->lock);
		return -EINVAL;
	}

	params = find_dai_stream_params(codec, dai->id, substream->stream);
	if (!params) {
		dev_err(codec->dev, "Failed to fetch dai_stream pointer\n");
		mutex_unlock(&codec->lock);
		return -EINVAL;
	}

	bundle = to_gb_bundle(module->dev);
	ret = gb_pm_runtime_get_sync(bundle);
	if (ret) {
		mutex_unlock(&codec->lock);
		return ret;
	}

	ret = gb_audio_apbridgea_set_config(data->connection, 0,
					    AUDIO_APBRIDGEA_PCM_FMT_16,
					    AUDIO_APBRIDGEA_PCM_RATE_48000,
					    6144000);
	if (ret) {
		dev_err_ratelimited(dai->dev, "%d: Error during set_config\n",
				    ret);
		gb_pm_runtime_put_noidle(bundle);
		mutex_unlock(&codec->lock);
		return ret;
	}

	gb_pm_runtime_put_noidle(bundle);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		sig_bits = dai->driver->playback.sig_bits;
	else
		sig_bits = dai->driver->capture.sig_bits;

	params->state = GBAUDIO_CODEC_HWPARAMS;
	params->format = format;
	params->rate = rate;
	params->channels = channels;
	params->sig_bits = sig_bits;

	mutex_unlock(&codec->lock);
	return 0;
}

static int gbcodec_prepare(struct snd_pcm_substream *substream,
			   struct snd_soc_dai *dai)
{
	int ret;
	struct gbaudio_module_info *module;
	struct gbaudio_data_connection *data;
	struct gb_bundle *bundle;
	struct gbaudio_codec_info *codec = dev_get_drvdata(dai->dev);
	struct gbaudio_stream_params *params;

	mutex_lock(&codec->lock);

	if (list_empty(&codec->module_list)) {
		dev_err(codec->dev, "No codec module available\n");
		mutex_unlock(&codec->lock);
		return -ENODEV;
	}

	list_for_each_entry(module, &codec->module_list, list) {
		/* find the dai */
		data = find_data(module, dai->id);
		if (data)
			break;
	}
	if (!data) {
		dev_err(dai->dev, "DATA connection missing\n");
		mutex_unlock(&codec->lock);
		return -ENODEV;
	}

	params = find_dai_stream_params(codec, dai->id, substream->stream);
	if (!params) {
		dev_err(codec->dev, "Failed to fetch dai_stream pointer\n");
		mutex_unlock(&codec->lock);
		return -EINVAL;
	}

	bundle = to_gb_bundle(module->dev);
	ret = gb_pm_runtime_get_sync(bundle);
	if (ret) {
		mutex_unlock(&codec->lock);
		return ret;
	}

	switch (substream->stream) {
	case SNDRV_PCM_STREAM_PLAYBACK:
<<<<<<< HEAD
		ret = gb_audio_apbridgea_set_tx_data_size(data->connection, 0,
							  192);
		break;
	case SNDRV_PCM_STREAM_CAPTURE:
		ret = gb_audio_apbridgea_set_rx_data_size(data->connection, 0,
							  192);
=======
		ret = gb_audio_apbridgea_set_tx_data_size(data->connection, 0, 192);
		break;
	case SNDRV_PCM_STREAM_CAPTURE:
		ret = gb_audio_apbridgea_set_rx_data_size(data->connection, 0, 192);
>>>>>>> upstream/android-13
		break;
	}
	if (ret) {
		gb_pm_runtime_put_noidle(bundle);
		mutex_unlock(&codec->lock);
<<<<<<< HEAD
		dev_err_ratelimited(dai->dev, "set_data_size failed:%d\n",
				    ret);
=======
		dev_err_ratelimited(dai->dev, "set_data_size failed:%d\n", ret);
>>>>>>> upstream/android-13
		return ret;
	}

	gb_pm_runtime_put_noidle(bundle);

	params->state = GBAUDIO_CODEC_PREPARE;
	mutex_unlock(&codec->lock);
	return 0;
}

static int gbcodec_mute_stream(struct snd_soc_dai *dai, int mute, int stream)
{
	int ret;
	struct gbaudio_data_connection *data;
	struct gbaudio_module_info *module;
	struct gb_bundle *bundle;
	struct gbaudio_codec_info *codec = dev_get_drvdata(dai->dev);
	struct gbaudio_stream_params *params;

	dev_dbg(dai->dev, "Mute:%d, Direction:%s\n", mute,
		stream ? "CAPTURE" : "PLAYBACK");

	mutex_lock(&codec->lock);

	params = find_dai_stream_params(codec, dai->id, stream);
	if (!params) {
		dev_err(codec->dev, "Failed to fetch dai_stream pointer\n");
		mutex_unlock(&codec->lock);
		return -EINVAL;
	}

	if (list_empty(&codec->module_list)) {
		dev_err(codec->dev, "No codec module available\n");
		if (mute) {
			params->state = GBAUDIO_CODEC_STOP;
			ret = 0;
		} else {
			ret = -ENODEV;
		}
		mutex_unlock(&codec->lock);
		return ret;
	}

	list_for_each_entry(module, &codec->module_list, list) {
		/* find the dai */
		data = find_data(module, dai->id);
		if (data)
			break;
	}
	if (!data) {
		dev_err(dai->dev, "%s:%s DATA connection missing\n",
			dai->name, module->name);
		mutex_unlock(&codec->lock);
		return -ENODEV;
	}

	bundle = to_gb_bundle(module->dev);
	ret = gb_pm_runtime_get_sync(bundle);
	if (ret) {
		mutex_unlock(&codec->lock);
		return ret;
	}

	if (!mute && !stream) {/* start playback */
<<<<<<< HEAD
		ret = gb_audio_apbridgea_prepare_tx(data->connection,
						    0);
		if (!ret)
			ret = gb_audio_apbridgea_start_tx(data->connection,
							  0, 0);
		params->state = GBAUDIO_CODEC_START;
	} else if (!mute && stream) {/* start capture */
		ret = gb_audio_apbridgea_prepare_rx(data->connection,
						    0);
		if (!ret)
			ret = gb_audio_apbridgea_start_rx(data->connection,
							  0);
=======
		ret = gb_audio_apbridgea_prepare_tx(data->connection, 0);
		if (!ret)
			ret = gb_audio_apbridgea_start_tx(data->connection, 0, 0);
		params->state = GBAUDIO_CODEC_START;
	} else if (!mute && stream) {/* start capture */
		ret = gb_audio_apbridgea_prepare_rx(data->connection, 0);
		if (!ret)
			ret = gb_audio_apbridgea_start_rx(data->connection, 0);
>>>>>>> upstream/android-13
		params->state = GBAUDIO_CODEC_START;
	} else if (mute && !stream) {/* stop playback */
		ret = gb_audio_apbridgea_stop_tx(data->connection, 0);
		if (!ret)
<<<<<<< HEAD
			ret = gb_audio_apbridgea_shutdown_tx(data->connection,
							     0);
=======
			ret = gb_audio_apbridgea_shutdown_tx(data->connection, 0);
>>>>>>> upstream/android-13
		params->state = GBAUDIO_CODEC_STOP;
	} else if (mute && stream) {/* stop capture */
		ret = gb_audio_apbridgea_stop_rx(data->connection, 0);
		if (!ret)
<<<<<<< HEAD
			ret = gb_audio_apbridgea_shutdown_rx(data->connection,
							     0);
=======
			ret = gb_audio_apbridgea_shutdown_rx(data->connection, 0);
>>>>>>> upstream/android-13
		params->state = GBAUDIO_CODEC_STOP;
	} else {
		ret = -EINVAL;
	}

	if (ret)
		dev_err_ratelimited(dai->dev,
				    "%s:Error during %s %s stream:%d\n",
				    module->name, mute ? "Mute" : "Unmute",
				    stream ? "Capture" : "Playback", ret);

	gb_pm_runtime_put_noidle(bundle);
	mutex_unlock(&codec->lock);
	return ret;
}

static const struct snd_soc_dai_ops gbcodec_dai_ops = {
	.startup = gbcodec_startup,
	.shutdown = gbcodec_shutdown,
	.hw_params = gbcodec_hw_params,
	.prepare = gbcodec_prepare,
	.mute_stream = gbcodec_mute_stream,
};

static struct snd_soc_dai_driver gbaudio_dai[] = {
	{
		.name = "apb-i2s0",
		.id = 0,
		.playback = {
			.stream_name = "I2S 0 Playback",
			.rates = SNDRV_PCM_RATE_48000,
<<<<<<< HEAD
			.formats = SNDRV_PCM_FORMAT_S16_LE,
=======
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
>>>>>>> upstream/android-13
			.rate_max = 48000,
			.rate_min = 48000,
			.channels_min = 1,
			.channels_max = 2,
			.sig_bits = 16,
		},
		.capture = {
			.stream_name = "I2S 0 Capture",
			.rates = SNDRV_PCM_RATE_48000,
<<<<<<< HEAD
			.formats = SNDRV_PCM_FORMAT_S16_LE,
=======
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
>>>>>>> upstream/android-13
			.rate_max = 48000,
			.rate_min = 48000,
			.channels_min = 1,
			.channels_max = 2,
			.sig_bits = 16,
		},
		.ops = &gbcodec_dai_ops,
	},
};

static int gbaudio_init_jack(struct gbaudio_module_info *module,
<<<<<<< HEAD
			     struct snd_soc_codec *codec)
{
	int ret;
=======
			     struct snd_soc_card *card)
{
	int ret;
	struct gbaudio_jack *jack, *n;
	struct snd_soc_jack_pin *headset, *button;
>>>>>>> upstream/android-13

	if (!module->jack_mask)
		return 0;

	snprintf(module->jack_name, NAME_SIZE, "GB %d Headset Jack",
		 module->dev_id);
<<<<<<< HEAD
	ret = snd_soc_jack_new(codec, module->jack_name, module->jack_mask,
			       &module->headset_jack);
=======

	headset = devm_kzalloc(module->dev, sizeof(*headset), GFP_KERNEL);
	if (!headset)
		return -ENOMEM;

	headset->pin = module->jack_name;
	headset->mask = module->jack_mask;
	ret = snd_soc_card_jack_new(card, module->jack_name, module->jack_mask,
				    &module->headset.jack, headset, 1);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(module->dev, "Failed to create new jack\n");
		return ret;
	}

<<<<<<< HEAD
=======
	/* Add to module's jack list */
	list_add(&module->headset.list, &module->jack_list);

>>>>>>> upstream/android-13
	if (!module->button_mask)
		return 0;

	snprintf(module->button_name, NAME_SIZE, "GB %d Button Jack",
		 module->dev_id);
<<<<<<< HEAD
	ret = snd_soc_jack_new(codec, module->button_name, module->button_mask,
			       &module->button_jack);
	if (ret) {
		dev_err(module->dev, "Failed to create button jack\n");
		return ret;
	}

=======
	button = devm_kzalloc(module->dev, sizeof(*button), GFP_KERNEL);
	if (!button) {
		ret = -ENOMEM;
		goto free_jacks;
	}

	button->pin = module->button_name;
	button->mask = module->button_mask;
	ret = snd_soc_card_jack_new(card, module->button_name,
				    module->button_mask, &module->button.jack,
				    button, 1);
	if (ret) {
		dev_err(module->dev, "Failed to create button jack\n");
		goto free_jacks;
	}

	/* Add to module's jack list */
	list_add(&module->button.list, &module->jack_list);

>>>>>>> upstream/android-13
	/*
	 * Currently, max 4 buttons are supported with following key mapping
	 * BTN_0 = KEY_MEDIA
	 * BTN_1 = KEY_VOICECOMMAND
	 * BTN_2 = KEY_VOLUMEUP
	 * BTN_3 = KEY_VOLUMEDOWN
	 */

	if (module->button_mask & SND_JACK_BTN_0) {
<<<<<<< HEAD
		ret = snd_jack_set_key(module->button_jack.jack, SND_JACK_BTN_0,
				       KEY_MEDIA);
		if (ret) {
			dev_err(module->dev, "Failed to set BTN_0\n");
			return ret;
=======
		ret = snd_jack_set_key(module->button.jack.jack, SND_JACK_BTN_0,
				       KEY_MEDIA);
		if (ret) {
			dev_err(module->dev, "Failed to set BTN_0\n");
			goto free_jacks;
>>>>>>> upstream/android-13
		}
	}

	if (module->button_mask & SND_JACK_BTN_1) {
<<<<<<< HEAD
		ret = snd_jack_set_key(module->button_jack.jack, SND_JACK_BTN_1,
				       KEY_VOICECOMMAND);
		if (ret) {
			dev_err(module->dev, "Failed to set BTN_1\n");
			return ret;
=======
		ret = snd_jack_set_key(module->button.jack.jack, SND_JACK_BTN_1,
				       KEY_VOICECOMMAND);
		if (ret) {
			dev_err(module->dev, "Failed to set BTN_1\n");
			goto free_jacks;
>>>>>>> upstream/android-13
		}
	}

	if (module->button_mask & SND_JACK_BTN_2) {
<<<<<<< HEAD
		ret = snd_jack_set_key(module->button_jack.jack, SND_JACK_BTN_2,
				       KEY_VOLUMEUP);
		if (ret) {
			dev_err(module->dev, "Failed to set BTN_2\n");
			return ret;
=======
		ret = snd_jack_set_key(module->button.jack.jack, SND_JACK_BTN_2,
				       KEY_VOLUMEUP);
		if (ret) {
			dev_err(module->dev, "Failed to set BTN_2\n");
			goto free_jacks;
>>>>>>> upstream/android-13
		}
	}

	if (module->button_mask & SND_JACK_BTN_3) {
<<<<<<< HEAD
		ret = snd_jack_set_key(module->button_jack.jack, SND_JACK_BTN_3,
				       KEY_VOLUMEDOWN);
		if (ret) {
			dev_err(module->dev, "Failed to set BTN_0\n");
			return ret;
=======
		ret = snd_jack_set_key(module->button.jack.jack, SND_JACK_BTN_3,
				       KEY_VOLUMEDOWN);
		if (ret) {
			dev_err(module->dev, "Failed to set BTN_0\n");
			goto free_jacks;
>>>>>>> upstream/android-13
		}
	}

	/* FIXME
	 * verify if this is really required
	set_bit(INPUT_PROP_NO_DUMMY_RELEASE,
<<<<<<< HEAD
		module->button_jack.jack->input_dev->propbit);
	*/

	return 0;
=======
		module->button.jack.jack->input_dev->propbit);
	*/

	return 0;

free_jacks:
	list_for_each_entry_safe(jack, n, &module->jack_list, list) {
		snd_device_free(card->snd_card, jack->jack.jack);
		list_del(&jack->list);
	}

	return ret;
>>>>>>> upstream/android-13
}

int gbaudio_register_module(struct gbaudio_module_info *module)
{
	int ret;
<<<<<<< HEAD
	struct snd_soc_codec *codec;
	struct snd_card *card;
	struct snd_soc_jack *jack = NULL;
=======
	struct snd_soc_component *comp;
	struct snd_card *card;
	struct gbaudio_jack *jack = NULL;
>>>>>>> upstream/android-13

	if (!gbcodec) {
		dev_err(module->dev, "GB Codec not yet probed\n");
		return -EAGAIN;
	}

<<<<<<< HEAD
	codec = gbcodec->codec;
	card = codec->card->snd_card;
=======
	comp = gbcodec->component;
	card = comp->card->snd_card;
>>>>>>> upstream/android-13

	down_write(&card->controls_rwsem);

	if (module->num_dais) {
		dev_err(gbcodec->dev,
			"%d:DAIs not supported via gbcodec driver\n",
			module->num_dais);
		up_write(&card->controls_rwsem);
		return -EINVAL;
	}

<<<<<<< HEAD
	ret = gbaudio_init_jack(module, codec);
=======
	ret = gbaudio_init_jack(module, comp->card);
>>>>>>> upstream/android-13
	if (ret) {
		up_write(&card->controls_rwsem);
		return ret;
	}

	if (module->dapm_widgets)
<<<<<<< HEAD
		snd_soc_dapm_new_controls(&codec->dapm, module->dapm_widgets,
					  module->num_dapm_widgets);
	if (module->controls)
		snd_soc_add_codec_controls(codec, module->controls,
					   module->num_controls);
	if (module->dapm_routes)
		snd_soc_dapm_add_routes(&codec->dapm, module->dapm_routes,
					module->num_dapm_routes);

	/* card already instantiated, create widgets here only */
	if (codec->card->instantiated) {
		snd_soc_dapm_link_component_dai_widgets(codec->card,
							&codec->dapm);
=======
		snd_soc_dapm_new_controls(&comp->dapm, module->dapm_widgets,
					  module->num_dapm_widgets);
	if (module->controls)
		snd_soc_add_component_controls(comp, module->controls,
					       module->num_controls);
	if (module->dapm_routes)
		snd_soc_dapm_add_routes(&comp->dapm, module->dapm_routes,
					module->num_dapm_routes);

	/* card already instantiated, create widgets here only */
	if (comp->card->instantiated) {
		gbaudio_dapm_link_component_dai_widgets(comp->card, &comp->dapm);
>>>>>>> upstream/android-13
#ifdef CONFIG_SND_JACK
		/*
		 * register jack devices for this module
		 * from codec->jack_list
		 */
<<<<<<< HEAD
		list_for_each_entry(jack, &codec->jack_list, list) {
			if ((jack == &module->headset_jack) ||
			    (jack == &module->button_jack))
				snd_device_register(codec->card->snd_card,
						    jack->jack);
=======
		list_for_each_entry(jack, &module->jack_list, list) {
			snd_device_register(comp->card->snd_card,
					    jack->jack.jack);
>>>>>>> upstream/android-13
		}
#endif
	}

	mutex_lock(&gbcodec->lock);
	list_add(&module->list, &gbcodec->module_list);
	mutex_unlock(&gbcodec->lock);

<<<<<<< HEAD
	if (codec->card->instantiated)
		ret = snd_soc_dapm_new_widgets(&codec->dapm);
	dev_dbg(codec->dev, "Registered %s module\n", module->name);
=======
	if (comp->card->instantiated)
		ret = snd_soc_dapm_new_widgets(comp->card);
	dev_dbg(comp->dev, "Registered %s module\n", module->name);
>>>>>>> upstream/android-13

	up_write(&card->controls_rwsem);
	return ret;
}
EXPORT_SYMBOL(gbaudio_register_module);

static void gbaudio_codec_clean_data_tx(struct gbaudio_data_connection *data)
{
	u16 i2s_port, cportid;
	int ret;

	if (list_is_singular(&gbcodec->module_list)) {
		ret = gb_audio_apbridgea_stop_tx(data->connection, 0);
		if (ret)
			return;
<<<<<<< HEAD
		ret = gb_audio_apbridgea_shutdown_tx(data->connection,
						     0);
=======
		ret = gb_audio_apbridgea_shutdown_tx(data->connection, 0);
>>>>>>> upstream/android-13
		if (ret)
			return;
	}
	i2s_port = 0;	/* fixed for now */
	cportid = data->connection->hd_cport_id;
	ret = gb_audio_apbridgea_unregister_cport(data->connection,
						  i2s_port, cportid,
						  AUDIO_APBRIDGEA_DIRECTION_TX);
	data->state[0] = GBAUDIO_CODEC_SHUTDOWN;
}

static void gbaudio_codec_clean_data_rx(struct gbaudio_data_connection *data)
{
	u16 i2s_port, cportid;
	int ret;

	if (list_is_singular(&gbcodec->module_list)) {
		ret = gb_audio_apbridgea_stop_rx(data->connection, 0);
		if (ret)
			return;
<<<<<<< HEAD
		ret = gb_audio_apbridgea_shutdown_rx(data->connection,
						     0);
=======
		ret = gb_audio_apbridgea_shutdown_rx(data->connection, 0);
>>>>>>> upstream/android-13
		if (ret)
			return;
	}
	i2s_port = 0;	/* fixed for now */
	cportid = data->connection->hd_cport_id;
	ret = gb_audio_apbridgea_unregister_cport(data->connection,
						  i2s_port, cportid,
						  AUDIO_APBRIDGEA_DIRECTION_RX);
	data->state[1] = GBAUDIO_CODEC_SHUTDOWN;
}

static void gbaudio_codec_cleanup(struct gbaudio_module_info *module)
{
	struct gbaudio_data_connection *data;
	int pb_state, cap_state;

	dev_dbg(gbcodec->dev, "%s: removed, cleanup APBridge\n", module->name);
	list_for_each_entry(data, &module->data_list, list) {
		pb_state = data->state[0];
		cap_state = data->state[1];

		if (pb_state > GBAUDIO_CODEC_SHUTDOWN)
			gbaudio_codec_clean_data_tx(data);

		if (cap_state > GBAUDIO_CODEC_SHUTDOWN)
			gbaudio_codec_clean_data_rx(data);
	}
}

void gbaudio_unregister_module(struct gbaudio_module_info *module)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = gbcodec->codec;
	struct snd_card *card = codec->card->snd_card;
	struct snd_soc_jack *jack, *next_j;
	int mask;

	dev_dbg(codec->dev, "Unregister %s module\n", module->name);
=======
	struct snd_soc_component *comp = gbcodec->component;
	struct snd_card *card = comp->card->snd_card;
	struct gbaudio_jack *jack, *n;
	int mask;

	dev_dbg(comp->dev, "Unregister %s module\n", module->name);
>>>>>>> upstream/android-13

	down_write(&card->controls_rwsem);
	mutex_lock(&gbcodec->lock);
	gbaudio_codec_cleanup(module);
	list_del(&module->list);
<<<<<<< HEAD
	dev_dbg(codec->dev, "Process Unregister %s module\n", module->name);
	mutex_unlock(&gbcodec->lock);

#ifdef CONFIG_SND_JACK
	/* free jack devices for this module from codec->jack_list */
	list_for_each_entry_safe(jack, next_j, &codec->jack_list, list) {
		if (jack == &module->headset_jack)
			mask = GBCODEC_JACK_MASK;
		else if (jack == &module->button_jack)
=======
	dev_dbg(comp->dev, "Process Unregister %s module\n", module->name);
	mutex_unlock(&gbcodec->lock);

#ifdef CONFIG_SND_JACK
	/* free jack devices for this module jack_list */
	list_for_each_entry_safe(jack, n, &module->jack_list, list) {
		if (jack == &module->headset)
			mask = GBCODEC_JACK_MASK;
		else if (jack == &module->button)
>>>>>>> upstream/android-13
			mask = GBCODEC_JACK_BUTTON_MASK;
		else
			mask = 0;
		if (mask) {
			dev_dbg(module->dev, "Report %s removal\n",
<<<<<<< HEAD
				jack->jack->id);
			snd_soc_jack_report(jack, 0, mask);
			snd_device_free(codec->card->snd_card, jack->jack);
=======
				jack->jack.jack->id);
			snd_soc_jack_report(&jack->jack, 0, mask);
			snd_device_free(comp->card->snd_card,
					jack->jack.jack);
>>>>>>> upstream/android-13
			list_del(&jack->list);
		}
	}
#endif

	if (module->dapm_routes) {
<<<<<<< HEAD
		dev_dbg(codec->dev, "Removing %d routes\n",
			module->num_dapm_routes);
		snd_soc_dapm_del_routes(&codec->dapm, module->dapm_routes,
					module->num_dapm_routes);
	}
	if (module->controls) {
		dev_dbg(codec->dev, "Removing %d controls\n",
			module->num_controls);
		snd_soc_remove_codec_controls(codec, module->controls,
					      module->num_controls);
	}
	if (module->dapm_widgets) {
		dev_dbg(codec->dev, "Removing %d widgets\n",
			module->num_dapm_widgets);
		snd_soc_dapm_free_controls(&codec->dapm, module->dapm_widgets,
					   module->num_dapm_widgets);
	}

	dev_dbg(codec->dev, "Unregistered %s module\n", module->name);
=======
		dev_dbg(comp->dev, "Removing %d routes\n",
			module->num_dapm_routes);
		snd_soc_dapm_del_routes(&comp->dapm, module->dapm_routes,
					module->num_dapm_routes);
	}
	if (module->controls) {
		dev_dbg(comp->dev, "Removing %d controls\n",
			module->num_controls);
		/* release control semaphore */
		up_write(&card->controls_rwsem);
		gbaudio_remove_component_controls(comp, module->controls,
						  module->num_controls);
		down_write(&card->controls_rwsem);
	}
	if (module->dapm_widgets) {
		dev_dbg(comp->dev, "Removing %d widgets\n",
			module->num_dapm_widgets);
		gbaudio_dapm_free_controls(&comp->dapm, module->dapm_widgets,
					   module->num_dapm_widgets);
	}

	dev_dbg(comp->dev, "Unregistered %s module\n", module->name);
>>>>>>> upstream/android-13

	up_write(&card->controls_rwsem);
}
EXPORT_SYMBOL(gbaudio_unregister_module);

/*
<<<<<<< HEAD
 * codec driver ops
 */
static int gbcodec_probe(struct snd_soc_codec *codec)
=======
 * component driver ops
 */
static int gbcodec_probe(struct snd_soc_component *comp)
>>>>>>> upstream/android-13
{
	int i;
	struct gbaudio_codec_info *info;
	struct gbaudio_codec_dai *dai;

<<<<<<< HEAD
	info = devm_kzalloc(codec->dev, sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	info->dev = codec->dev;
=======
	info = devm_kzalloc(comp->dev, sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	info->dev = comp->dev;
>>>>>>> upstream/android-13
	INIT_LIST_HEAD(&info->module_list);
	mutex_init(&info->lock);
	INIT_LIST_HEAD(&info->dai_list);

	/* init dai_list used to maintain runtime stream info */
	for (i = 0; i < ARRAY_SIZE(gbaudio_dai); i++) {
<<<<<<< HEAD
		dai = devm_kzalloc(codec->dev, sizeof(*dai), GFP_KERNEL);
=======
		dai = devm_kzalloc(comp->dev, sizeof(*dai), GFP_KERNEL);
>>>>>>> upstream/android-13
		if (!dai)
			return -ENOMEM;
		dai->id = gbaudio_dai[i].id;
		list_add(&dai->list, &info->dai_list);
	}

<<<<<<< HEAD
	info->codec = codec;
	snd_soc_codec_set_drvdata(codec, info);
	gbcodec = info;

	device_init_wakeup(codec->dev, 1);
	return 0;
}

static int gbcodec_remove(struct snd_soc_codec *codec)
{
	/* Empty function for now */
	return 0;
}

static int gbcodec_write(struct snd_soc_codec *codec, unsigned int reg,
=======
	info->component = comp;
	snd_soc_component_set_drvdata(comp, info);
	gbcodec = info;

	device_init_wakeup(comp->dev, 1);
	return 0;
}

static void gbcodec_remove(struct snd_soc_component *comp)
{
	/* Empty function for now */
	return;
}

static int gbcodec_write(struct snd_soc_component *comp, unsigned int reg,
>>>>>>> upstream/android-13
			 unsigned int value)
{
	return 0;
}

<<<<<<< HEAD
static unsigned int gbcodec_read(struct snd_soc_codec *codec,
=======
static unsigned int gbcodec_read(struct snd_soc_component *comp,
>>>>>>> upstream/android-13
				 unsigned int reg)
{
	return 0;
}

<<<<<<< HEAD
static struct snd_soc_codec_driver soc_codec_dev_gbaudio = {
=======
static const struct snd_soc_component_driver soc_codec_dev_gbaudio = {
>>>>>>> upstream/android-13
	.probe	= gbcodec_probe,
	.remove	= gbcodec_remove,

	.read = gbcodec_read,
	.write = gbcodec_write,
<<<<<<< HEAD

	.idle_bias_off = true,
	.ignore_pmdown_time = 1,
=======
>>>>>>> upstream/android-13
};

#ifdef CONFIG_PM
static int gbaudio_codec_suspend(struct device *dev)
{
	dev_dbg(dev, "%s: suspend\n", __func__);
	return 0;
}

static int gbaudio_codec_resume(struct device *dev)
{
	dev_dbg(dev, "%s: resume\n", __func__);
	return 0;
}

static const struct dev_pm_ops gbaudio_codec_pm_ops = {
	.suspend	= gbaudio_codec_suspend,
	.resume		= gbaudio_codec_resume,
};
#endif

static int gbaudio_codec_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	return snd_soc_register_codec(&pdev->dev, &soc_codec_dev_gbaudio,
=======
	return devm_snd_soc_register_component(&pdev->dev,
			&soc_codec_dev_gbaudio,
>>>>>>> upstream/android-13
			gbaudio_dai, ARRAY_SIZE(gbaudio_dai));
}

static int gbaudio_codec_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	snd_soc_unregister_codec(&pdev->dev);
=======
>>>>>>> upstream/android-13
	return 0;
}

static const struct of_device_id greybus_asoc_machine_of_match[]  = {
	{ .compatible = "toshiba,apb-dummy-codec", },
	{},
};

static struct platform_driver gbaudio_codec_driver = {
	.driver = {
		.name = "apb-dummy-codec",
<<<<<<< HEAD
		.owner = THIS_MODULE,
=======
>>>>>>> upstream/android-13
#ifdef CONFIG_PM
		.pm = &gbaudio_codec_pm_ops,
#endif
		.of_match_table = greybus_asoc_machine_of_match,
	},
	.probe = gbaudio_codec_probe,
	.remove = gbaudio_codec_remove,
};
module_platform_driver(gbaudio_codec_driver);

MODULE_DESCRIPTION("APBridge ALSA SoC dummy codec driver");
MODULE_AUTHOR("Vaibhav Agarwal <vaibhav.agarwal@linaro.org>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:apb-dummy-codec");
