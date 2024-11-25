<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * motu-stream.c - a part of driver for MOTU FireWire series
 *
 * Copyright (c) 2015-2017 Takashi Sakamoto <o-takashi@sakamocchi.jp>
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#include "motu.h"

<<<<<<< HEAD
#define	CALLBACK_TIMEOUT	200
=======
#define	READY_TIMEOUT_MS	200
>>>>>>> upstream/android-13

#define ISOC_COMM_CONTROL_OFFSET		0x0b00
#define  ISOC_COMM_CONTROL_MASK			0xffff0000
#define  CHANGE_RX_ISOC_COMM_STATE		0x80000000
#define  RX_ISOC_COMM_IS_ACTIVATED		0x40000000
#define  RX_ISOC_COMM_CHANNEL_MASK		0x3f000000
#define  RX_ISOC_COMM_CHANNEL_SHIFT		24
#define  CHANGE_TX_ISOC_COMM_STATE		0x00800000
#define  TX_ISOC_COMM_IS_ACTIVATED		0x00400000
#define  TX_ISOC_COMM_CHANNEL_MASK		0x003f0000
#define  TX_ISOC_COMM_CHANNEL_SHIFT		16

#define PACKET_FORMAT_OFFSET			0x0b10
#define  TX_PACKET_EXCLUDE_DIFFERED_DATA_CHUNKS	0x00000080
#define  RX_PACKET_EXCLUDE_DIFFERED_DATA_CHUNKS	0x00000040
#define  TX_PACKET_TRANSMISSION_SPEED_MASK	0x0000000f

<<<<<<< HEAD
static int start_both_streams(struct snd_motu *motu, unsigned int rate)
{
	unsigned int midi_ports = 0;
=======
static int keep_resources(struct snd_motu *motu, unsigned int rate,
			  struct amdtp_stream *stream)
{
	struct fw_iso_resources *resources;
	struct snd_motu_packet_format *packet_format;
	unsigned int midi_ports = 0;
	int err;

	if (stream == &motu->rx_stream) {
		resources = &motu->rx_resources;
		packet_format = &motu->rx_packet_formats;

		if ((motu->spec->flags & SND_MOTU_SPEC_RX_MIDI_2ND_Q) ||
		    (motu->spec->flags & SND_MOTU_SPEC_RX_MIDI_3RD_Q))
			midi_ports = 1;
	} else {
		resources = &motu->tx_resources;
		packet_format = &motu->tx_packet_formats;

		if ((motu->spec->flags & SND_MOTU_SPEC_TX_MIDI_2ND_Q) ||
		    (motu->spec->flags & SND_MOTU_SPEC_TX_MIDI_3RD_Q))
			midi_ports = 1;
	}

	err = amdtp_motu_set_parameters(stream, rate, midi_ports,
					packet_format);
	if (err < 0)
		return err;

	return fw_iso_resources_allocate(resources,
				amdtp_stream_get_max_payload(stream),
				fw_parent_device(motu->unit)->max_speed);
}

static int begin_session(struct snd_motu *motu)
{
>>>>>>> upstream/android-13
	__be32 reg;
	u32 data;
	int err;

<<<<<<< HEAD
	if ((motu->spec->flags & SND_MOTU_SPEC_RX_MIDI_2ND_Q) ||
	    (motu->spec->flags & SND_MOTU_SPEC_RX_MIDI_3RD_Q))
		midi_ports = 1;

	/* Set packet formation to our packet streaming engine. */
	err = amdtp_motu_set_parameters(&motu->rx_stream, rate, midi_ports,
					&motu->rx_packet_formats);
	if (err < 0)
		return err;

	if ((motu->spec->flags & SND_MOTU_SPEC_TX_MIDI_2ND_Q) ||
	    (motu->spec->flags & SND_MOTU_SPEC_TX_MIDI_3RD_Q))
		midi_ports = 1;
	else
		midi_ports = 0;

	err = amdtp_motu_set_parameters(&motu->tx_stream, rate, midi_ports,
					&motu->tx_packet_formats);
	if (err < 0)
		return err;

	/* Get isochronous resources on the bus. */
	err = fw_iso_resources_allocate(&motu->rx_resources,
				amdtp_stream_get_max_payload(&motu->rx_stream),
				fw_parent_device(motu->unit)->max_speed);
	if (err < 0)
		return err;

	err = fw_iso_resources_allocate(&motu->tx_resources,
				amdtp_stream_get_max_payload(&motu->tx_stream),
				fw_parent_device(motu->unit)->max_speed);
	if (err < 0)
		return err;

	/* Configure the unit to start isochronous communication. */
=======
	// Configure the unit to start isochronous communication.
>>>>>>> upstream/android-13
	err = snd_motu_transaction_read(motu, ISOC_COMM_CONTROL_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;
	data = be32_to_cpu(reg) & ~ISOC_COMM_CONTROL_MASK;

	data |= CHANGE_RX_ISOC_COMM_STATE | RX_ISOC_COMM_IS_ACTIVATED |
		(motu->rx_resources.channel << RX_ISOC_COMM_CHANNEL_SHIFT) |
		CHANGE_TX_ISOC_COMM_STATE | TX_ISOC_COMM_IS_ACTIVATED |
		(motu->tx_resources.channel << TX_ISOC_COMM_CHANNEL_SHIFT);

	reg = cpu_to_be32(data);
	return snd_motu_transaction_write(motu, ISOC_COMM_CONTROL_OFFSET, &reg,
					  sizeof(reg));
}

<<<<<<< HEAD
static void stop_both_streams(struct snd_motu *motu)
=======
static void finish_session(struct snd_motu *motu)
>>>>>>> upstream/android-13
{
	__be32 reg;
	u32 data;
	int err;

<<<<<<< HEAD
	err = motu->spec->protocol->switch_fetching_mode(motu, false);
=======
	err = snd_motu_protocol_switch_fetching_mode(motu, false);
>>>>>>> upstream/android-13
	if (err < 0)
		return;

	err = snd_motu_transaction_read(motu, ISOC_COMM_CONTROL_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return;
	data = be32_to_cpu(reg);

	data &= ~(RX_ISOC_COMM_IS_ACTIVATED | TX_ISOC_COMM_IS_ACTIVATED);
	data |= CHANGE_RX_ISOC_COMM_STATE | CHANGE_TX_ISOC_COMM_STATE;

	reg = cpu_to_be32(data);
	snd_motu_transaction_write(motu, ISOC_COMM_CONTROL_OFFSET, &reg,
				   sizeof(reg));
<<<<<<< HEAD

	fw_iso_resources_free(&motu->tx_resources);
	fw_iso_resources_free(&motu->rx_resources);
}

static int start_isoc_ctx(struct snd_motu *motu, struct amdtp_stream *stream)
{
	struct fw_iso_resources *resources;
	int err;

	if (stream == &motu->rx_stream)
		resources = &motu->rx_resources;
	else
		resources = &motu->tx_resources;

	err = amdtp_stream_start(stream, resources->channel,
				 fw_parent_device(motu->unit)->max_speed);
	if (err < 0)
		return err;

	if (!amdtp_stream_wait_callback(stream, CALLBACK_TIMEOUT)) {
		amdtp_stream_stop(stream);
		fw_iso_resources_free(resources);
		return -ETIMEDOUT;
	}

	return 0;
}

static void stop_isoc_ctx(struct snd_motu *motu, struct amdtp_stream *stream)
{
	struct fw_iso_resources *resources;

	if (stream == &motu->rx_stream)
		resources = &motu->rx_resources;
	else
		resources = &motu->tx_resources;

	amdtp_stream_stop(stream);
	fw_iso_resources_free(resources);
=======
>>>>>>> upstream/android-13
}

int snd_motu_stream_cache_packet_formats(struct snd_motu *motu)
{
	int err;

<<<<<<< HEAD
	err = motu->spec->protocol->cache_packet_formats(motu);
=======
	err = snd_motu_protocol_cache_packet_formats(motu);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	if (motu->spec->flags & SND_MOTU_SPEC_TX_MIDI_2ND_Q) {
		motu->tx_packet_formats.midi_flag_offset = 4;
		motu->tx_packet_formats.midi_byte_offset = 6;
	} else if (motu->spec->flags & SND_MOTU_SPEC_TX_MIDI_3RD_Q) {
		motu->tx_packet_formats.midi_flag_offset = 8;
		motu->tx_packet_formats.midi_byte_offset = 7;
	}

	if (motu->spec->flags & SND_MOTU_SPEC_RX_MIDI_2ND_Q) {
		motu->rx_packet_formats.midi_flag_offset = 4;
		motu->rx_packet_formats.midi_byte_offset = 6;
	} else if (motu->spec->flags & SND_MOTU_SPEC_RX_MIDI_3RD_Q) {
		motu->rx_packet_formats.midi_flag_offset = 8;
		motu->rx_packet_formats.midi_byte_offset = 7;
	}

	return 0;
}

<<<<<<< HEAD
=======
int snd_motu_stream_reserve_duplex(struct snd_motu *motu, unsigned int rate,
				   unsigned int frames_per_period,
				   unsigned int frames_per_buffer)
{
	unsigned int curr_rate;
	int err;

	err = snd_motu_protocol_get_clock_rate(motu, &curr_rate);
	if (err < 0)
		return err;
	if (rate == 0)
		rate = curr_rate;

	if (motu->substreams_counter == 0 || curr_rate != rate) {
		amdtp_domain_stop(&motu->domain);
		finish_session(motu);

		fw_iso_resources_free(&motu->tx_resources);
		fw_iso_resources_free(&motu->rx_resources);

		kfree(motu->cache.event_offsets);
		motu->cache.event_offsets = NULL;

		err = snd_motu_protocol_set_clock_rate(motu, rate);
		if (err < 0) {
			dev_err(&motu->unit->device,
				"fail to set sampling rate: %d\n", err);
			return err;
		}

		err = snd_motu_stream_cache_packet_formats(motu);
		if (err < 0)
			return err;

		err = keep_resources(motu, rate, &motu->tx_stream);
		if (err < 0)
			return err;

		err = keep_resources(motu, rate, &motu->rx_stream);
		if (err < 0) {
			fw_iso_resources_free(&motu->tx_resources);
			return err;
		}

		err = amdtp_domain_set_events_per_period(&motu->domain,
					frames_per_period, frames_per_buffer);
		if (err < 0) {
			fw_iso_resources_free(&motu->tx_resources);
			fw_iso_resources_free(&motu->rx_resources);
			return err;
		}

		motu->cache.size = motu->tx_stream.syt_interval * frames_per_buffer;
		motu->cache.event_offsets = kcalloc(motu->cache.size, sizeof(*motu->cache.event_offsets),
						  GFP_KERNEL);
		if (!motu->cache.event_offsets) {
			fw_iso_resources_free(&motu->tx_resources);
			fw_iso_resources_free(&motu->rx_resources);
			return -ENOMEM;
		}
	}

	return 0;
}

>>>>>>> upstream/android-13
static int ensure_packet_formats(struct snd_motu *motu)
{
	__be32 reg;
	u32 data;
	int err;

	err = snd_motu_transaction_read(motu, PACKET_FORMAT_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;
	data = be32_to_cpu(reg);

	data &= ~(TX_PACKET_EXCLUDE_DIFFERED_DATA_CHUNKS |
		  RX_PACKET_EXCLUDE_DIFFERED_DATA_CHUNKS|
		  TX_PACKET_TRANSMISSION_SPEED_MASK);
<<<<<<< HEAD
	if (motu->tx_packet_formats.differed_part_pcm_chunks[0] == 0)
		data |= TX_PACKET_EXCLUDE_DIFFERED_DATA_CHUNKS;
	if (motu->rx_packet_formats.differed_part_pcm_chunks[0] == 0)
=======
	if (motu->spec->tx_fixed_pcm_chunks[0] == motu->tx_packet_formats.pcm_chunks[0])
		data |= TX_PACKET_EXCLUDE_DIFFERED_DATA_CHUNKS;
	if (motu->spec->rx_fixed_pcm_chunks[0] == motu->rx_packet_formats.pcm_chunks[0])
>>>>>>> upstream/android-13
		data |= RX_PACKET_EXCLUDE_DIFFERED_DATA_CHUNKS;
	data |= fw_parent_device(motu->unit)->max_speed;

	reg = cpu_to_be32(data);
	return snd_motu_transaction_write(motu, PACKET_FORMAT_OFFSET, &reg,
					  sizeof(reg));
}

<<<<<<< HEAD
int snd_motu_stream_start_duplex(struct snd_motu *motu, unsigned int rate)
{
	const struct snd_motu_protocol *protocol = motu->spec->protocol;
	unsigned int curr_rate;
	int err = 0;

	if (motu->capture_substreams == 0 && motu->playback_substreams == 0)
		return 0;

	/* Some packet queueing errors. */
	if (amdtp_streaming_error(&motu->rx_stream) ||
	    amdtp_streaming_error(&motu->tx_stream)) {
		amdtp_stream_stop(&motu->rx_stream);
		amdtp_stream_stop(&motu->tx_stream);
		stop_both_streams(motu);
	}

	err = snd_motu_stream_cache_packet_formats(motu);
	if (err < 0)
		return err;

	/* Stop stream if rate is different. */
	err = protocol->get_clock_rate(motu, &curr_rate);
	if (err < 0) {
		dev_err(&motu->unit->device,
			"fail to get sampling rate: %d\n", err);
		return err;
	}
	if (rate == 0)
		rate = curr_rate;
	if (rate != curr_rate) {
		amdtp_stream_stop(&motu->rx_stream);
		amdtp_stream_stop(&motu->tx_stream);
		stop_both_streams(motu);
	}

	if (!amdtp_stream_running(&motu->rx_stream)) {
		err = protocol->set_clock_rate(motu, rate);
		if (err < 0) {
			dev_err(&motu->unit->device,
				"fail to set sampling rate: %d\n", err);
			return err;
		}
=======
int snd_motu_stream_start_duplex(struct snd_motu *motu)
{
	unsigned int generation = motu->rx_resources.generation;
	int err = 0;

	if (motu->substreams_counter == 0)
		return 0;

	if (amdtp_streaming_error(&motu->rx_stream) ||
	    amdtp_streaming_error(&motu->tx_stream)) {
		amdtp_domain_stop(&motu->domain);
		finish_session(motu);
	}

	if (generation != fw_parent_device(motu->unit)->card->generation) {
		err = fw_iso_resources_update(&motu->rx_resources);
		if (err < 0)
			return err;

		err = fw_iso_resources_update(&motu->tx_resources);
		if (err < 0)
			return err;
	}

	if (!amdtp_stream_running(&motu->rx_stream)) {
		int spd = fw_parent_device(motu->unit)->max_speed;
>>>>>>> upstream/android-13

		err = ensure_packet_formats(motu);
		if (err < 0)
			return err;

<<<<<<< HEAD
		err = start_both_streams(motu, rate);
=======
		err = begin_session(motu);
>>>>>>> upstream/android-13
		if (err < 0) {
			dev_err(&motu->unit->device,
				"fail to start isochronous comm: %d\n", err);
			goto stop_streams;
		}

<<<<<<< HEAD
		err = start_isoc_ctx(motu, &motu->rx_stream);
		if (err < 0) {
			dev_err(&motu->unit->device,
				"fail to start IT context: %d\n", err);
			goto stop_streams;
		}

		err = protocol->switch_fetching_mode(motu, true);
=======
		err = amdtp_domain_add_stream(&motu->domain, &motu->tx_stream,
					      motu->tx_resources.channel, spd);
		if (err < 0)
			goto stop_streams;

		err = amdtp_domain_add_stream(&motu->domain, &motu->rx_stream,
					      motu->rx_resources.channel, spd);
		if (err < 0)
			goto stop_streams;

		motu->cache.tail = 0;
		motu->cache.tx_cycle_count = UINT_MAX;
		motu->cache.head = 0;
		motu->cache.rx_cycle_count = UINT_MAX;

		// NOTE: The device requires both of replay; the sequence of the number of data
		// blocks per packet, and the sequence of source packet header per data block as
		// presentation time.
		err = amdtp_domain_start(&motu->domain, 0, true, false);
		if (err < 0)
			goto stop_streams;

		if (!amdtp_domain_wait_ready(&motu->domain, READY_TIMEOUT_MS)) {
			err = -ETIMEDOUT;
			goto stop_streams;
		}

		err = snd_motu_protocol_switch_fetching_mode(motu, true);
>>>>>>> upstream/android-13
		if (err < 0) {
			dev_err(&motu->unit->device,
				"fail to enable frame fetching: %d\n", err);
			goto stop_streams;
		}
	}

<<<<<<< HEAD
	if (!amdtp_stream_running(&motu->tx_stream) &&
	    motu->capture_substreams > 0) {
		err = start_isoc_ctx(motu, &motu->tx_stream);
		if (err < 0) {
			dev_err(&motu->unit->device,
				"fail to start IR context: %d", err);
			amdtp_stream_stop(&motu->rx_stream);
			goto stop_streams;
		}
	}

	return 0;

stop_streams:
	stop_both_streams(motu);
=======
	return 0;

stop_streams:
	amdtp_domain_stop(&motu->domain);
	finish_session(motu);
>>>>>>> upstream/android-13
	return err;
}

void snd_motu_stream_stop_duplex(struct snd_motu *motu)
{
<<<<<<< HEAD
	if (motu->capture_substreams == 0) {
		if (amdtp_stream_running(&motu->tx_stream))
			stop_isoc_ctx(motu, &motu->tx_stream);

		if (motu->playback_substreams == 0) {
			if (amdtp_stream_running(&motu->rx_stream))
				stop_isoc_ctx(motu, &motu->rx_stream);
			stop_both_streams(motu);
		}
	}
}

static int init_stream(struct snd_motu *motu, enum amdtp_stream_direction dir)
{
	int err;
	struct amdtp_stream *stream;
	struct fw_iso_resources *resources;

	if (dir == AMDTP_IN_STREAM) {
		stream = &motu->tx_stream;
		resources = &motu->tx_resources;
	} else {
		stream = &motu->rx_stream;
		resources = &motu->rx_resources;
=======
	if (motu->substreams_counter == 0) {
		amdtp_domain_stop(&motu->domain);
		finish_session(motu);

		fw_iso_resources_free(&motu->tx_resources);
		fw_iso_resources_free(&motu->rx_resources);

		kfree(motu->cache.event_offsets);
		motu->cache.event_offsets = NULL;
	}
}

static int init_stream(struct snd_motu *motu, struct amdtp_stream *s)
{
	struct fw_iso_resources *resources;
	enum amdtp_stream_direction dir;
	int err;

	if (s == &motu->tx_stream) {
		resources = &motu->tx_resources;
		dir = AMDTP_IN_STREAM;
	} else {
		resources = &motu->rx_resources;
		dir = AMDTP_OUT_STREAM;
>>>>>>> upstream/android-13
	}

	err = fw_iso_resources_init(resources, motu->unit);
	if (err < 0)
		return err;

<<<<<<< HEAD
	err = amdtp_motu_init(stream, motu->unit, dir, motu->spec->protocol);
	if (err < 0) {
		amdtp_stream_destroy(stream);
		fw_iso_resources_destroy(resources);
	}
=======
	err = amdtp_motu_init(s, motu->unit, dir, motu->spec, &motu->cache);
	if (err < 0)
		fw_iso_resources_destroy(resources);
>>>>>>> upstream/android-13

	return err;
}

<<<<<<< HEAD
static void destroy_stream(struct snd_motu *motu,
			   enum amdtp_stream_direction dir)
{
	struct amdtp_stream *stream;
	struct fw_iso_resources *resources;

	if (dir == AMDTP_IN_STREAM) {
		stream = &motu->tx_stream;
		resources = &motu->tx_resources;
	} else {
		stream = &motu->rx_stream;
		resources = &motu->rx_resources;
	}

	amdtp_stream_destroy(stream);
	fw_iso_resources_destroy(resources);
=======
static void destroy_stream(struct snd_motu *motu, struct amdtp_stream *s)
{
	amdtp_stream_destroy(s);

	if (s == &motu->tx_stream)
		fw_iso_resources_destroy(&motu->tx_resources);
	else
		fw_iso_resources_destroy(&motu->rx_resources);
>>>>>>> upstream/android-13
}

int snd_motu_stream_init_duplex(struct snd_motu *motu)
{
	int err;

<<<<<<< HEAD
	err = init_stream(motu, AMDTP_IN_STREAM);
	if (err < 0)
		return err;

	err = init_stream(motu, AMDTP_OUT_STREAM);
	if (err < 0)
		destroy_stream(motu, AMDTP_IN_STREAM);
=======
	err = init_stream(motu, &motu->tx_stream);
	if (err < 0)
		return err;

	err = init_stream(motu, &motu->rx_stream);
	if (err < 0) {
		destroy_stream(motu, &motu->tx_stream);
		return err;
	}

	err = amdtp_domain_init(&motu->domain);
	if (err < 0) {
		destroy_stream(motu, &motu->tx_stream);
		destroy_stream(motu, &motu->rx_stream);
	}
>>>>>>> upstream/android-13

	return err;
}

<<<<<<< HEAD
/*
 * This function should be called before starting streams or after stopping
 * streams.
 */
void snd_motu_stream_destroy_duplex(struct snd_motu *motu)
{
	destroy_stream(motu, AMDTP_IN_STREAM);
	destroy_stream(motu, AMDTP_OUT_STREAM);

	motu->playback_substreams = 0;
	motu->capture_substreams = 0;
=======
// This function should be called before starting streams or after stopping
// streams.
void snd_motu_stream_destroy_duplex(struct snd_motu *motu)
{
	amdtp_domain_destroy(&motu->domain);

	destroy_stream(motu, &motu->rx_stream);
	destroy_stream(motu, &motu->tx_stream);

	motu->substreams_counter = 0;
>>>>>>> upstream/android-13
}

static void motu_lock_changed(struct snd_motu *motu)
{
	motu->dev_lock_changed = true;
	wake_up(&motu->hwdep_wait);
}

int snd_motu_stream_lock_try(struct snd_motu *motu)
{
	int err;

	spin_lock_irq(&motu->lock);

	if (motu->dev_lock_count < 0) {
		err = -EBUSY;
		goto out;
	}

	if (motu->dev_lock_count++ == 0)
		motu_lock_changed(motu);
	err = 0;
out:
	spin_unlock_irq(&motu->lock);
	return err;
}

void snd_motu_stream_lock_release(struct snd_motu *motu)
{
	spin_lock_irq(&motu->lock);

	if (WARN_ON(motu->dev_lock_count <= 0))
		goto out;

	if (--motu->dev_lock_count == 0)
		motu_lock_changed(motu);
out:
	spin_unlock_irq(&motu->lock);
}
