<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * ff-stream.c - a part of driver for RME Fireface series
 *
 * Copyright (c) 2015-2017 Takashi Sakamoto
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#include "ff.h"

<<<<<<< HEAD
#define CALLBACK_TIMEOUT_MS	200

static int get_rate_mode(unsigned int rate, unsigned int *mode)
{
	int i;

	for (i = 0; i < CIP_SFC_COUNT; i++) {
		if (amdtp_rate_table[i] == rate)
			break;
	}

	if (i == CIP_SFC_COUNT)
		return -EINVAL;

	*mode = ((int)i - 1) / 2;
=======
#define READY_TIMEOUT_MS	200

int snd_ff_stream_get_multiplier_mode(enum cip_sfc sfc,
				      enum snd_ff_stream_mode *mode)
{
	static const enum snd_ff_stream_mode modes[] = {
		[CIP_SFC_32000] = SND_FF_STREAM_MODE_LOW,
		[CIP_SFC_44100] = SND_FF_STREAM_MODE_LOW,
		[CIP_SFC_48000] = SND_FF_STREAM_MODE_LOW,
		[CIP_SFC_88200] = SND_FF_STREAM_MODE_MID,
		[CIP_SFC_96000] = SND_FF_STREAM_MODE_MID,
		[CIP_SFC_176400] = SND_FF_STREAM_MODE_HIGH,
		[CIP_SFC_192000] = SND_FF_STREAM_MODE_HIGH,
	};

	if (sfc >= CIP_SFC_COUNT)
		return -EINVAL;

	*mode = modes[sfc];
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/*
 * Fireface 400 manages isochronous channel number in 3 bit field. Therefore,
 * we can allocate between 0 and 7 channel.
 */
static int keep_resources(struct snd_ff *ff, unsigned int rate)
{
	int mode;
	int err;

	err = get_rate_mode(rate, &mode);
	if (err < 0)
		return err;

	/* Keep resources for in-stream. */
	err = amdtp_ff_set_parameters(&ff->tx_stream, rate,
				      ff->spec->pcm_capture_channels[mode]);
	if (err < 0)
		return err;
	ff->tx_resources.channels_mask = 0x00000000000000ffuLL;
	err = fw_iso_resources_allocate(&ff->tx_resources,
			amdtp_stream_get_max_payload(&ff->tx_stream),
			fw_parent_device(ff->unit)->max_speed);
	if (err < 0)
		return err;

	/* Keep resources for out-stream. */
	err = amdtp_ff_set_parameters(&ff->rx_stream, rate,
				      ff->spec->pcm_playback_channels[mode]);
	if (err < 0)
		return err;
	ff->rx_resources.channels_mask = 0x00000000000000ffuLL;
	err = fw_iso_resources_allocate(&ff->rx_resources,
			amdtp_stream_get_max_payload(&ff->rx_stream),
			fw_parent_device(ff->unit)->max_speed);
	if (err < 0)
		fw_iso_resources_free(&ff->tx_resources);

	return err;
}

static void release_resources(struct snd_ff *ff)
{
	fw_iso_resources_free(&ff->tx_resources);
	fw_iso_resources_free(&ff->rx_resources);
}

=======
>>>>>>> upstream/android-13
static inline void finish_session(struct snd_ff *ff)
{
	ff->spec->protocol->finish_session(ff);
	ff->spec->protocol->switch_fetching_mode(ff, false);
}

<<<<<<< HEAD
static int init_stream(struct snd_ff *ff, enum amdtp_stream_direction dir)
{
	int err;
	struct fw_iso_resources *resources;
	struct amdtp_stream *stream;

	if (dir == AMDTP_IN_STREAM) {
		resources = &ff->tx_resources;
		stream = &ff->tx_stream;
	} else {
		resources = &ff->rx_resources;
		stream = &ff->rx_stream;
=======
static int init_stream(struct snd_ff *ff, struct amdtp_stream *s)
{
	struct fw_iso_resources *resources;
	enum amdtp_stream_direction dir;
	int err;

	if (s == &ff->tx_stream) {
		resources = &ff->tx_resources;
		dir = AMDTP_IN_STREAM;
	} else {
		resources = &ff->rx_resources;
		dir = AMDTP_OUT_STREAM;
>>>>>>> upstream/android-13
	}

	err = fw_iso_resources_init(resources, ff->unit);
	if (err < 0)
		return err;

<<<<<<< HEAD
	err = amdtp_ff_init(stream, ff->unit, dir);
=======
	err = amdtp_ff_init(s, ff->unit, dir);
>>>>>>> upstream/android-13
	if (err < 0)
		fw_iso_resources_destroy(resources);

	return err;
}

<<<<<<< HEAD
static void destroy_stream(struct snd_ff *ff, enum amdtp_stream_direction dir)
{
	if (dir == AMDTP_IN_STREAM) {
		amdtp_stream_destroy(&ff->tx_stream);
		fw_iso_resources_destroy(&ff->tx_resources);
	} else {
		amdtp_stream_destroy(&ff->rx_stream);
		fw_iso_resources_destroy(&ff->rx_resources);
	}
=======
static void destroy_stream(struct snd_ff *ff, struct amdtp_stream *s)
{
	amdtp_stream_destroy(s);

	if (s == &ff->tx_stream)
		fw_iso_resources_destroy(&ff->tx_resources);
	else
		fw_iso_resources_destroy(&ff->rx_resources);
>>>>>>> upstream/android-13
}

int snd_ff_stream_init_duplex(struct snd_ff *ff)
{
	int err;

<<<<<<< HEAD
	err = init_stream(ff, AMDTP_OUT_STREAM);
	if (err < 0)
		goto end;

	err = init_stream(ff, AMDTP_IN_STREAM);
	if (err < 0)
		destroy_stream(ff, AMDTP_OUT_STREAM);
end:
=======
	err = init_stream(ff, &ff->rx_stream);
	if (err < 0)
		return err;

	err = init_stream(ff, &ff->tx_stream);
	if (err < 0) {
		destroy_stream(ff, &ff->rx_stream);
		return err;
	}

	err = amdtp_domain_init(&ff->domain);
	if (err < 0) {
		destroy_stream(ff, &ff->rx_stream);
		destroy_stream(ff, &ff->tx_stream);
	}

>>>>>>> upstream/android-13
	return err;
}

/*
 * This function should be called before starting streams or after stopping
 * streams.
 */
void snd_ff_stream_destroy_duplex(struct snd_ff *ff)
{
<<<<<<< HEAD
	destroy_stream(ff, AMDTP_IN_STREAM);
	destroy_stream(ff, AMDTP_OUT_STREAM);
}

int snd_ff_stream_start_duplex(struct snd_ff *ff, unsigned int rate)
=======
	amdtp_domain_destroy(&ff->domain);

	destroy_stream(ff, &ff->rx_stream);
	destroy_stream(ff, &ff->tx_stream);
}

int snd_ff_stream_reserve_duplex(struct snd_ff *ff, unsigned int rate,
				 unsigned int frames_per_period,
				 unsigned int frames_per_buffer)
>>>>>>> upstream/android-13
{
	unsigned int curr_rate;
	enum snd_ff_clock_src src;
	int err;

<<<<<<< HEAD
	if (ff->substreams_counter == 0)
		return 0;

	err = ff->spec->protocol->get_clock(ff, &curr_rate, &src);
	if (err < 0)
		return err;
	if (curr_rate != rate ||
	    amdtp_streaming_error(&ff->tx_stream) ||
	    amdtp_streaming_error(&ff->rx_stream)) {
		finish_session(ff);

		amdtp_stream_stop(&ff->tx_stream);
		amdtp_stream_stop(&ff->rx_stream);

		release_resources(ff);
=======
	err = ff->spec->protocol->get_clock(ff, &curr_rate, &src);
	if (err < 0)
		return err;

	if (ff->substreams_counter == 0 || curr_rate != rate) {
		enum snd_ff_stream_mode mode;
		int i;

		amdtp_domain_stop(&ff->domain);
		finish_session(ff);

		fw_iso_resources_free(&ff->tx_resources);
		fw_iso_resources_free(&ff->rx_resources);

		for (i = 0; i < CIP_SFC_COUNT; ++i) {
			if (amdtp_rate_table[i] == rate)
				break;
		}
		if (i >= CIP_SFC_COUNT)
			return -EINVAL;

		err = snd_ff_stream_get_multiplier_mode(i, &mode);
		if (err < 0)
			return err;

		err = amdtp_ff_set_parameters(&ff->tx_stream, rate,
					ff->spec->pcm_capture_channels[mode]);
		if (err < 0)
			return err;

		err = amdtp_ff_set_parameters(&ff->rx_stream, rate,
					ff->spec->pcm_playback_channels[mode]);
		if (err < 0)
			return err;

		err = ff->spec->protocol->allocate_resources(ff, rate);
		if (err < 0)
			return err;

		err = amdtp_domain_set_events_per_period(&ff->domain,
					frames_per_period, frames_per_buffer);
		if (err < 0) {
			fw_iso_resources_free(&ff->tx_resources);
			fw_iso_resources_free(&ff->rx_resources);
			return err;
		}
	}

	return 0;
}

int snd_ff_stream_start_duplex(struct snd_ff *ff, unsigned int rate)
{
	int err;

	if (ff->substreams_counter == 0)
		return 0;

	if (amdtp_streaming_error(&ff->tx_stream) ||
	    amdtp_streaming_error(&ff->rx_stream)) {
		amdtp_domain_stop(&ff->domain);
		finish_session(ff);
>>>>>>> upstream/android-13
	}

	/*
	 * Regardless of current source of clock signal, drivers transfer some
	 * packets. Then, the device transfers packets.
	 */
	if (!amdtp_stream_running(&ff->rx_stream)) {
<<<<<<< HEAD
		err = keep_resources(ff, rate);
		if (err < 0)
			goto error;
=======
		int spd = fw_parent_device(ff->unit)->max_speed;
>>>>>>> upstream/android-13

		err = ff->spec->protocol->begin_session(ff, rate);
		if (err < 0)
			goto error;

<<<<<<< HEAD
		err = amdtp_stream_start(&ff->rx_stream,
					 ff->rx_resources.channel,
					 fw_parent_device(ff->unit)->max_speed);
		if (err < 0)
			goto error;

		if (!amdtp_stream_wait_callback(&ff->rx_stream,
						CALLBACK_TIMEOUT_MS)) {
=======
		err = amdtp_domain_add_stream(&ff->domain, &ff->rx_stream,
					      ff->rx_resources.channel, spd);
		if (err < 0)
			goto error;

		err = amdtp_domain_add_stream(&ff->domain, &ff->tx_stream,
					      ff->tx_resources.channel, spd);
		if (err < 0)
			goto error;

		// NOTE: The device doesn't transfer packets unless receiving any packet. The
		// sequence of tx packets includes cycle skip corresponding to empty packet or
		// NODATA packet in IEC 61883-1/6. The sequence of the number of data blocks per
		// packet is important for media clock recovery.
		err = amdtp_domain_start(&ff->domain, 0, true, true);
		if (err < 0)
			goto error;

		if (!amdtp_domain_wait_ready(&ff->domain, READY_TIMEOUT_MS)) {
>>>>>>> upstream/android-13
			err = -ETIMEDOUT;
			goto error;
		}

		err = ff->spec->protocol->switch_fetching_mode(ff, true);
		if (err < 0)
			goto error;
	}

<<<<<<< HEAD
	if (!amdtp_stream_running(&ff->tx_stream)) {
		err = amdtp_stream_start(&ff->tx_stream,
					 ff->tx_resources.channel,
					 fw_parent_device(ff->unit)->max_speed);
		if (err < 0)
			goto error;

		if (!amdtp_stream_wait_callback(&ff->tx_stream,
						CALLBACK_TIMEOUT_MS)) {
			err = -ETIMEDOUT;
			goto error;
		}
	}

	return 0;
error:
	amdtp_stream_stop(&ff->tx_stream);
	amdtp_stream_stop(&ff->rx_stream);

	finish_session(ff);
	release_resources(ff);
=======
	return 0;
error:
	amdtp_domain_stop(&ff->domain);
	finish_session(ff);
>>>>>>> upstream/android-13

	return err;
}

void snd_ff_stream_stop_duplex(struct snd_ff *ff)
{
<<<<<<< HEAD
	if (ff->substreams_counter > 0)
		return;

	amdtp_stream_stop(&ff->tx_stream);
	amdtp_stream_stop(&ff->rx_stream);
	finish_session(ff);
	release_resources(ff);
=======
	if (ff->substreams_counter == 0) {
		amdtp_domain_stop(&ff->domain);
		finish_session(ff);

		fw_iso_resources_free(&ff->tx_resources);
		fw_iso_resources_free(&ff->rx_resources);
	}
>>>>>>> upstream/android-13
}

void snd_ff_stream_update_duplex(struct snd_ff *ff)
{
<<<<<<< HEAD
	/* The device discontinue to transfer packets.  */
	amdtp_stream_pcm_abort(&ff->tx_stream);
	amdtp_stream_stop(&ff->tx_stream);

	amdtp_stream_pcm_abort(&ff->rx_stream);
	amdtp_stream_stop(&ff->rx_stream);

	fw_iso_resources_update(&ff->tx_resources);
	fw_iso_resources_update(&ff->rx_resources);
=======
	amdtp_domain_stop(&ff->domain);

	// The device discontinue to transfer packets.
	amdtp_stream_pcm_abort(&ff->tx_stream);
	amdtp_stream_pcm_abort(&ff->rx_stream);
>>>>>>> upstream/android-13
}

void snd_ff_stream_lock_changed(struct snd_ff *ff)
{
	ff->dev_lock_changed = true;
	wake_up(&ff->hwdep_wait);
}

int snd_ff_stream_lock_try(struct snd_ff *ff)
{
	int err;

	spin_lock_irq(&ff->lock);

	/* user land lock this */
	if (ff->dev_lock_count < 0) {
		err = -EBUSY;
		goto end;
	}

	/* this is the first time */
	if (ff->dev_lock_count++ == 0)
		snd_ff_stream_lock_changed(ff);
	err = 0;
end:
	spin_unlock_irq(&ff->lock);
	return err;
}

void snd_ff_stream_lock_release(struct snd_ff *ff)
{
	spin_lock_irq(&ff->lock);

	if (WARN_ON(ff->dev_lock_count <= 0))
		goto end;
	if (--ff->dev_lock_count == 0)
		snd_ff_stream_lock_changed(ff);
end:
	spin_unlock_irq(&ff->lock);
}
