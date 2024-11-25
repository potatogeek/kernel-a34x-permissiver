<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * dice_stream.c - a part of driver for DICE based devices
 *
 * Copyright (c) Clemens Ladisch <clemens@ladisch.de>
 * Copyright (c) 2014 Takashi Sakamoto <o-takashi@sakamocchi.jp>
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#include "dice.h"

<<<<<<< HEAD
#define	CALLBACK_TIMEOUT	200
#define NOTIFICATION_TIMEOUT_MS	(2 * MSEC_PER_SEC)
=======
#define	READY_TIMEOUT_MS	200
#define NOTIFICATION_TIMEOUT_MS	100
>>>>>>> upstream/android-13

struct reg_params {
	unsigned int count;
	unsigned int size;
};

const unsigned int snd_dice_rates[SND_DICE_RATES_COUNT] = {
	/* mode 0 */
	[0] =  32000,
	[1] =  44100,
	[2] =  48000,
	/* mode 1 */
	[3] =  88200,
	[4] =  96000,
	/* mode 2 */
	[5] = 176400,
	[6] = 192000,
};

int snd_dice_stream_get_rate_mode(struct snd_dice *dice, unsigned int rate,
				  enum snd_dice_rate_mode *mode)
{
	/* Corresponding to each entry in snd_dice_rates. */
	static const enum snd_dice_rate_mode modes[] = {
		[0] = SND_DICE_RATE_MODE_LOW,
		[1] = SND_DICE_RATE_MODE_LOW,
		[2] = SND_DICE_RATE_MODE_LOW,
		[3] = SND_DICE_RATE_MODE_MIDDLE,
		[4] = SND_DICE_RATE_MODE_MIDDLE,
		[5] = SND_DICE_RATE_MODE_HIGH,
		[6] = SND_DICE_RATE_MODE_HIGH,
	};
	int i;

	for (i = 0; i < ARRAY_SIZE(snd_dice_rates); i++) {
		if (!(dice->clock_caps & BIT(i)))
			continue;
		if (snd_dice_rates[i] != rate)
			continue;

		*mode = modes[i];
		return 0;
	}

	return -EINVAL;
}

<<<<<<< HEAD
/*
 * This operation has an effect to synchronize GLOBAL_STATUS/GLOBAL_SAMPLE_RATE
 * to GLOBAL_STATUS. Especially, just after powering on, these are different.
 */
static int ensure_phase_lock(struct snd_dice *dice, unsigned int rate)
{
	__be32 reg, nominal;
=======
static int select_clock(struct snd_dice *dice, unsigned int rate)
{
	__be32 reg;
>>>>>>> upstream/android-13
	u32 data;
	int i;
	int err;

	err = snd_dice_transaction_read_global(dice, GLOBAL_CLOCK_SELECT,
					       &reg, sizeof(reg));
	if (err < 0)
		return err;

	data = be32_to_cpu(reg);

	data &= ~CLOCK_RATE_MASK;
	for (i = 0; i < ARRAY_SIZE(snd_dice_rates); ++i) {
		if (snd_dice_rates[i] == rate)
			break;
	}
	if (i == ARRAY_SIZE(snd_dice_rates))
		return -EINVAL;
	data |= i << CLOCK_RATE_SHIFT;

	if (completion_done(&dice->clock_accepted))
		reinit_completion(&dice->clock_accepted);

	reg = cpu_to_be32(data);
	err = snd_dice_transaction_write_global(dice, GLOBAL_CLOCK_SELECT,
						&reg, sizeof(reg));
	if (err < 0)
		return err;

	if (wait_for_completion_timeout(&dice->clock_accepted,
<<<<<<< HEAD
			msecs_to_jiffies(NOTIFICATION_TIMEOUT_MS)) == 0) {
		/*
		 * Old versions of Dice firmware transfer no notification when
		 * the same clock status as current one is set. In this case,
		 * just check current clock status.
		 */
		err = snd_dice_transaction_read_global(dice, GLOBAL_STATUS,
						&nominal, sizeof(nominal));
		if (err < 0)
			return err;
		if (!(be32_to_cpu(nominal) & STATUS_SOURCE_LOCKED))
			return -ETIMEDOUT;
	}
=======
			msecs_to_jiffies(NOTIFICATION_TIMEOUT_MS)) == 0)
		return -ETIMEDOUT;
>>>>>>> upstream/android-13

	return 0;
}

static int get_register_params(struct snd_dice *dice,
			       struct reg_params *tx_params,
			       struct reg_params *rx_params)
{
	__be32 reg[2];
	int err;

	err = snd_dice_transaction_read_tx(dice, TX_NUMBER, reg, sizeof(reg));
	if (err < 0)
		return err;
	tx_params->count =
			min_t(unsigned int, be32_to_cpu(reg[0]), MAX_STREAMS);
	tx_params->size = be32_to_cpu(reg[1]) * 4;

	err = snd_dice_transaction_read_rx(dice, RX_NUMBER, reg, sizeof(reg));
	if (err < 0)
		return err;
	rx_params->count =
			min_t(unsigned int, be32_to_cpu(reg[0]), MAX_STREAMS);
	rx_params->size = be32_to_cpu(reg[1]) * 4;

	return 0;
}

static void release_resources(struct snd_dice *dice)
{
<<<<<<< HEAD
	unsigned int i;

	for (i = 0; i < MAX_STREAMS; i++) {
		if (amdtp_stream_running(&dice->tx_stream[i])) {
			amdtp_stream_pcm_abort(&dice->tx_stream[i]);
			amdtp_stream_stop(&dice->tx_stream[i]);
		}
		if (amdtp_stream_running(&dice->rx_stream[i])) {
			amdtp_stream_pcm_abort(&dice->rx_stream[i]);
			amdtp_stream_stop(&dice->rx_stream[i]);
		}

=======
	int i;

	for (i = 0; i < MAX_STREAMS; ++i) {
>>>>>>> upstream/android-13
		fw_iso_resources_free(&dice->tx_resources[i]);
		fw_iso_resources_free(&dice->rx_resources[i]);
	}
}

static void stop_streams(struct snd_dice *dice, enum amdtp_stream_direction dir,
			 struct reg_params *params)
{
	__be32 reg;
	unsigned int i;

	for (i = 0; i < params->count; i++) {
		reg = cpu_to_be32((u32)-1);
		if (dir == AMDTP_IN_STREAM) {
			snd_dice_transaction_write_tx(dice,
					params->size * i + TX_ISOCHRONOUS,
					&reg, sizeof(reg));
		} else {
			snd_dice_transaction_write_rx(dice,
					params->size * i + RX_ISOCHRONOUS,
					&reg, sizeof(reg));
		}
	}
}

<<<<<<< HEAD
static int keep_resources(struct snd_dice *dice,
			  enum amdtp_stream_direction dir, unsigned int index,
			  unsigned int rate, unsigned int pcm_chs,
			  unsigned int midi_ports)
{
	struct amdtp_stream *stream;
	struct fw_iso_resources *resources;
=======
static int keep_resources(struct snd_dice *dice, struct amdtp_stream *stream,
			  struct fw_iso_resources *resources, unsigned int rate,
			  unsigned int pcm_chs, unsigned int midi_ports)
{
>>>>>>> upstream/android-13
	bool double_pcm_frames;
	unsigned int i;
	int err;

<<<<<<< HEAD
	if (dir == AMDTP_IN_STREAM) {
		stream = &dice->tx_stream[index];
		resources = &dice->tx_resources[index];
	} else {
		stream = &dice->rx_stream[index];
		resources = &dice->rx_resources[index];
	}

	/*
	 * At 176.4/192.0 kHz, Dice has a quirk to transfer two PCM frames in
	 * one data block of AMDTP packet. Thus sampling transfer frequency is
	 * a half of PCM sampling frequency, i.e. PCM frames at 192.0 kHz are
	 * transferred on AMDTP packets at 96 kHz. Two successive samples of a
	 * channel are stored consecutively in the packet. This quirk is called
	 * as 'Dual Wire'.
	 * For this quirk, blocking mode is required and PCM buffer size should
	 * be aligned to SYT_INTERVAL.
	 */
	double_pcm_frames = rate > 96000;
=======
	// At 176.4/192.0 kHz, Dice has a quirk to transfer two PCM frames in
	// one data block of AMDTP packet. Thus sampling transfer frequency is
	// a half of PCM sampling frequency, i.e. PCM frames at 192.0 kHz are
	// transferred on AMDTP packets at 96 kHz. Two successive samples of a
	// channel are stored consecutively in the packet. This quirk is called
	// as 'Dual Wire'.
	// For this quirk, blocking mode is required and PCM buffer size should
	// be aligned to SYT_INTERVAL.
	double_pcm_frames = (rate > 96000 && !dice->disable_double_pcm_frames);
>>>>>>> upstream/android-13
	if (double_pcm_frames) {
		rate /= 2;
		pcm_chs *= 2;
	}

	err = amdtp_am824_set_parameters(stream, rate, pcm_chs, midi_ports,
					 double_pcm_frames);
	if (err < 0)
		return err;

	if (double_pcm_frames) {
		pcm_chs /= 2;

		for (i = 0; i < pcm_chs; i++) {
			amdtp_am824_set_pcm_position(stream, i, i * 2);
			amdtp_am824_set_pcm_position(stream, i + pcm_chs,
						     i * 2 + 1);
		}
	}

	return fw_iso_resources_allocate(resources,
				amdtp_stream_get_max_payload(stream),
				fw_parent_device(dice->unit)->max_speed);
}

<<<<<<< HEAD
static int start_streams(struct snd_dice *dice, enum amdtp_stream_direction dir,
			 unsigned int rate, struct reg_params *params)
{
	__be32 reg[2];
	enum snd_dice_rate_mode mode;
	unsigned int i, pcm_chs, midi_ports;
	struct amdtp_stream *streams;
	struct fw_iso_resources *resources;
	struct fw_device *fw_dev = fw_parent_device(dice->unit);
	int err = 0;

	if (dir == AMDTP_IN_STREAM) {
		streams = dice->tx_stream;
		resources = dice->tx_resources;
	} else {
		streams = dice->rx_stream;
		resources = dice->rx_resources;
	}
=======
static int keep_dual_resources(struct snd_dice *dice, unsigned int rate,
			       enum amdtp_stream_direction dir,
			       struct reg_params *params)
{
	enum snd_dice_rate_mode mode;
	int i;
	int err;
>>>>>>> upstream/android-13

	err = snd_dice_stream_get_rate_mode(dice, rate, &mode);
	if (err < 0)
		return err;

<<<<<<< HEAD
	for (i = 0; i < params->count; i++) {
		unsigned int pcm_cache;
		unsigned int midi_cache;

		if (dir == AMDTP_IN_STREAM) {
			pcm_cache = dice->tx_pcm_chs[i][mode];
			midi_cache = dice->tx_midi_ports[i];
=======
	for (i = 0; i < params->count; ++i) {
		__be32 reg[2];
		struct amdtp_stream *stream;
		struct fw_iso_resources *resources;
		unsigned int pcm_cache;
		unsigned int pcm_chs;
		unsigned int midi_ports;

		if (dir == AMDTP_IN_STREAM) {
			stream = &dice->tx_stream[i];
			resources = &dice->tx_resources[i];

			pcm_cache = dice->tx_pcm_chs[i][mode];
>>>>>>> upstream/android-13
			err = snd_dice_transaction_read_tx(dice,
					params->size * i + TX_NUMBER_AUDIO,
					reg, sizeof(reg));
		} else {
<<<<<<< HEAD
			pcm_cache = dice->rx_pcm_chs[i][mode];
			midi_cache = dice->rx_midi_ports[i];
=======
			stream = &dice->rx_stream[i];
			resources = &dice->rx_resources[i];

			pcm_cache = dice->rx_pcm_chs[i][mode];
>>>>>>> upstream/android-13
			err = snd_dice_transaction_read_rx(dice,
					params->size * i + RX_NUMBER_AUDIO,
					reg, sizeof(reg));
		}
		if (err < 0)
			return err;
		pcm_chs = be32_to_cpu(reg[0]);
		midi_ports = be32_to_cpu(reg[1]);

<<<<<<< HEAD
		/* These are important for developer of this driver. */
		if (pcm_chs != pcm_cache || midi_ports != midi_cache) {
			dev_info(&dice->unit->device,
				 "cache mismatch: pcm: %u:%u, midi: %u:%u\n",
				 pcm_chs, pcm_cache, midi_ports, midi_cache);
			return -EPROTO;
		}

		err = keep_resources(dice, dir, i, rate, pcm_chs, midi_ports);
		if (err < 0)
			return err;

		reg[0] = cpu_to_be32(resources[i].channel);
		if (dir == AMDTP_IN_STREAM) {
			err = snd_dice_transaction_write_tx(dice,
					params->size * i + TX_ISOCHRONOUS,
					reg, sizeof(reg[0]));
		} else {
			err = snd_dice_transaction_write_rx(dice,
					params->size * i + RX_ISOCHRONOUS,
					reg, sizeof(reg[0]));
		}
		if (err < 0)
			return err;

		if (dir == AMDTP_IN_STREAM) {
			reg[0] = cpu_to_be32(fw_dev->max_speed);
			err = snd_dice_transaction_write_tx(dice,
					params->size * i + TX_SPEED,
					reg, sizeof(reg[0]));
			if (err < 0)
				return err;
		}

		err = amdtp_stream_start(&streams[i], resources[i].channel,
					 fw_dev->max_speed);
=======
		// These are important for developer of this driver.
		if (pcm_chs != pcm_cache) {
			dev_info(&dice->unit->device,
				 "cache mismatch: pcm: %u:%u, midi: %u\n",
				 pcm_chs, pcm_cache, midi_ports);
			return -EPROTO;
		}

		err = keep_resources(dice, stream, resources, rate, pcm_chs,
				     midi_ports);
>>>>>>> upstream/android-13
		if (err < 0)
			return err;
	}

<<<<<<< HEAD
	return err;
}

static int start_duplex_streams(struct snd_dice *dice, unsigned int rate)
{
	struct reg_params tx_params, rx_params;
	int i;
	int err;

	err = get_register_params(dice, &tx_params, &rx_params);
	if (err < 0)
		return err;

	/* Stop transmission. */
	stop_streams(dice, AMDTP_IN_STREAM, &tx_params);
	stop_streams(dice, AMDTP_OUT_STREAM, &rx_params);
	snd_dice_transaction_clear_enable(dice);
	release_resources(dice);

	err = ensure_phase_lock(dice, rate);
	if (err < 0) {
		dev_err(&dice->unit->device, "fail to ensure phase lock\n");
		return err;
	}

	/* Likely to have changed stream formats. */
	err = get_register_params(dice, &tx_params, &rx_params);
	if (err < 0)
		return err;

	/* Start both streams. */
	err = start_streams(dice, AMDTP_IN_STREAM, rate, &tx_params);
	if (err < 0)
		goto error;
	err = start_streams(dice, AMDTP_OUT_STREAM, rate, &rx_params);
	if (err < 0)
		goto error;

	err = snd_dice_transaction_set_enable(dice);
	if (err < 0) {
		dev_err(&dice->unit->device, "fail to enable interface\n");
		goto error;
	}

	for (i = 0; i < MAX_STREAMS; i++) {
		if ((i < tx_params.count &&
		    !amdtp_stream_wait_callback(&dice->tx_stream[i],
						CALLBACK_TIMEOUT)) ||
		    (i < rx_params.count &&
		     !amdtp_stream_wait_callback(&dice->rx_stream[i],
						 CALLBACK_TIMEOUT))) {
			err = -ETIMEDOUT;
			goto error;
		}
=======
	return 0;
}

static void finish_session(struct snd_dice *dice, struct reg_params *tx_params,
			   struct reg_params *rx_params)
{
	stop_streams(dice, AMDTP_IN_STREAM, tx_params);
	stop_streams(dice, AMDTP_OUT_STREAM, rx_params);

	snd_dice_transaction_clear_enable(dice);
}

int snd_dice_stream_reserve_duplex(struct snd_dice *dice, unsigned int rate,
				   unsigned int events_per_period,
				   unsigned int events_per_buffer)
{
	unsigned int curr_rate;
	int err;

	// Check sampling transmission frequency.
	err = snd_dice_transaction_get_rate(dice, &curr_rate);
	if (err < 0)
		return err;
	if (rate == 0)
		rate = curr_rate;

	if (dice->substreams_counter == 0 || curr_rate != rate) {
		struct reg_params tx_params, rx_params;

		amdtp_domain_stop(&dice->domain);

		err = get_register_params(dice, &tx_params, &rx_params);
		if (err < 0)
			return err;
		finish_session(dice, &tx_params, &rx_params);

		release_resources(dice);

		// Just after owning the unit (GLOBAL_OWNER), the unit can
		// return invalid stream formats. Selecting clock parameters
		// have an effect for the unit to refine it.
		err = select_clock(dice, rate);
		if (err < 0)
			return err;

		// After changing sampling transfer frequency, the value of
		// register can be changed.
		err = get_register_params(dice, &tx_params, &rx_params);
		if (err < 0)
			return err;

		err = keep_dual_resources(dice, rate, AMDTP_IN_STREAM,
					  &tx_params);
		if (err < 0)
			goto error;

		err = keep_dual_resources(dice, rate, AMDTP_OUT_STREAM,
					  &rx_params);
		if (err < 0)
			goto error;

		err = amdtp_domain_set_events_per_period(&dice->domain,
					events_per_period, events_per_buffer);
		if (err < 0)
			goto error;
>>>>>>> upstream/android-13
	}

	return 0;
error:
<<<<<<< HEAD
	stop_streams(dice, AMDTP_IN_STREAM, &tx_params);
	stop_streams(dice, AMDTP_OUT_STREAM, &rx_params);
	snd_dice_transaction_clear_enable(dice);
=======
>>>>>>> upstream/android-13
	release_resources(dice);
	return err;
}

<<<<<<< HEAD
=======
static int start_streams(struct snd_dice *dice, enum amdtp_stream_direction dir,
			 unsigned int rate, struct reg_params *params)
{
	unsigned int max_speed = fw_parent_device(dice->unit)->max_speed;
	int i;
	int err;

	for (i = 0; i < params->count; i++) {
		struct amdtp_stream *stream;
		struct fw_iso_resources *resources;
		__be32 reg;

		if (dir == AMDTP_IN_STREAM) {
			stream = dice->tx_stream + i;
			resources = dice->tx_resources + i;
		} else {
			stream = dice->rx_stream + i;
			resources = dice->rx_resources + i;
		}

		reg = cpu_to_be32(resources->channel);
		if (dir == AMDTP_IN_STREAM) {
			err = snd_dice_transaction_write_tx(dice,
					params->size * i + TX_ISOCHRONOUS,
					&reg, sizeof(reg));
		} else {
			err = snd_dice_transaction_write_rx(dice,
					params->size * i + RX_ISOCHRONOUS,
					&reg, sizeof(reg));
		}
		if (err < 0)
			return err;

		if (dir == AMDTP_IN_STREAM) {
			reg = cpu_to_be32(max_speed);
			err = snd_dice_transaction_write_tx(dice,
					params->size * i + TX_SPEED,
					&reg, sizeof(reg));
			if (err < 0)
				return err;
		}

		err = amdtp_domain_add_stream(&dice->domain, stream,
					      resources->channel, max_speed);
		if (err < 0)
			return err;
	}

	return 0;
}

>>>>>>> upstream/android-13
/*
 * MEMO: After this function, there're two states of streams:
 *  - None streams are running.
 *  - All streams are running.
 */
<<<<<<< HEAD
int snd_dice_stream_start_duplex(struct snd_dice *dice, unsigned int rate)
{
	unsigned int curr_rate;
	unsigned int i;
=======
int snd_dice_stream_start_duplex(struct snd_dice *dice)
{
	unsigned int generation = dice->rx_resources[0].generation;
	struct reg_params tx_params, rx_params;
	unsigned int i;
	unsigned int rate;
>>>>>>> upstream/android-13
	enum snd_dice_rate_mode mode;
	int err;

	if (dice->substreams_counter == 0)
		return -EIO;

<<<<<<< HEAD
	/* Check sampling transmission frequency. */
	err = snd_dice_transaction_get_rate(dice, &curr_rate);
	if (err < 0) {
		dev_err(&dice->unit->device,
			"fail to get sampling rate\n");
		return err;
	}
	if (rate == 0)
		rate = curr_rate;
	if (rate != curr_rate)
		goto restart;

	/* Check error of packet streaming. */
	for (i = 0; i < MAX_STREAMS; ++i) {
		if (amdtp_streaming_error(&dice->tx_stream[i]))
			break;
		if (amdtp_streaming_error(&dice->rx_stream[i]))
			break;
	}
	if (i < MAX_STREAMS)
		goto restart;

	/* Check required streams are running or not. */
=======
	err = get_register_params(dice, &tx_params, &rx_params);
	if (err < 0)
		return err;

	// Check error of packet streaming.
	for (i = 0; i < MAX_STREAMS; ++i) {
		if (amdtp_streaming_error(&dice->tx_stream[i]) ||
		    amdtp_streaming_error(&dice->rx_stream[i])) {
			amdtp_domain_stop(&dice->domain);
			finish_session(dice, &tx_params, &rx_params);
			break;
		}
	}

	if (generation != fw_parent_device(dice->unit)->card->generation) {
		for (i = 0; i < MAX_STREAMS; ++i) {
			if (i < tx_params.count)
				fw_iso_resources_update(dice->tx_resources + i);
			if (i < rx_params.count)
				fw_iso_resources_update(dice->rx_resources + i);
		}
	}

	// Check required streams are running or not.
	err = snd_dice_transaction_get_rate(dice, &rate);
	if (err < 0)
		return err;
>>>>>>> upstream/android-13
	err = snd_dice_stream_get_rate_mode(dice, rate, &mode);
	if (err < 0)
		return err;
	for (i = 0; i < MAX_STREAMS; ++i) {
		if (dice->tx_pcm_chs[i][mode] > 0 &&
		    !amdtp_stream_running(&dice->tx_stream[i]))
			break;
		if (dice->rx_pcm_chs[i][mode] > 0 &&
		    !amdtp_stream_running(&dice->rx_stream[i]))
			break;
	}
<<<<<<< HEAD
	if (i < MAX_STREAMS)
		goto restart;

	return 0;
restart:
	return start_duplex_streams(dice, rate);
=======
	if (i < MAX_STREAMS) {
		// Start both streams.
		err = start_streams(dice, AMDTP_IN_STREAM, rate, &tx_params);
		if (err < 0)
			goto error;

		err = start_streams(dice, AMDTP_OUT_STREAM, rate, &rx_params);
		if (err < 0)
			goto error;

		err = snd_dice_transaction_set_enable(dice);
		if (err < 0) {
			dev_err(&dice->unit->device,
				"fail to enable interface\n");
			goto error;
		}

		// MEMO: The device immediately starts packet transmission when enabled. Some
		// devices are strictly to generate any discontinuity in the sequence of tx packet
		// when they receives invalid sequence of presentation time in CIP header. The
		// sequence replay for media clock recovery can suppress the behaviour.
		err = amdtp_domain_start(&dice->domain, 0, true, false);
		if (err < 0)
			goto error;

		if (!amdtp_domain_wait_ready(&dice->domain, READY_TIMEOUT_MS)) {
			err = -ETIMEDOUT;
			goto error;
		}
	}

	return 0;
error:
	amdtp_domain_stop(&dice->domain);
	finish_session(dice, &tx_params, &rx_params);
	return err;
>>>>>>> upstream/android-13
}

/*
 * MEMO: After this function, there're two states of streams:
 *  - None streams are running.
 *  - All streams are running.
 */
void snd_dice_stream_stop_duplex(struct snd_dice *dice)
{
	struct reg_params tx_params, rx_params;

<<<<<<< HEAD
	if (dice->substreams_counter > 0)
		return;

	snd_dice_transaction_clear_enable(dice);

	if (get_register_params(dice, &tx_params, &rx_params) == 0) {
		stop_streams(dice, AMDTP_IN_STREAM, &tx_params);
		stop_streams(dice, AMDTP_OUT_STREAM, &rx_params);
	}

	release_resources(dice);
=======
	if (dice->substreams_counter == 0) {
		if (get_register_params(dice, &tx_params, &rx_params) >= 0)
			finish_session(dice, &tx_params, &rx_params);

		amdtp_domain_stop(&dice->domain);
		release_resources(dice);
	}
>>>>>>> upstream/android-13
}

static int init_stream(struct snd_dice *dice, enum amdtp_stream_direction dir,
		       unsigned int index)
{
	struct amdtp_stream *stream;
	struct fw_iso_resources *resources;
	int err;

	if (dir == AMDTP_IN_STREAM) {
		stream = &dice->tx_stream[index];
		resources = &dice->tx_resources[index];
	} else {
		stream = &dice->rx_stream[index];
		resources = &dice->rx_resources[index];
	}

	err = fw_iso_resources_init(resources, dice->unit);
	if (err < 0)
		goto end;
	resources->channels_mask = 0x00000000ffffffffuLL;

	err = amdtp_am824_init(stream, dice->unit, dir, CIP_BLOCKING);
	if (err < 0) {
		amdtp_stream_destroy(stream);
		fw_iso_resources_destroy(resources);
	}
end:
	return err;
}

/*
 * This function should be called before starting streams or after stopping
 * streams.
 */
static void destroy_stream(struct snd_dice *dice,
			   enum amdtp_stream_direction dir,
			   unsigned int index)
{
	struct amdtp_stream *stream;
	struct fw_iso_resources *resources;

	if (dir == AMDTP_IN_STREAM) {
		stream = &dice->tx_stream[index];
		resources = &dice->tx_resources[index];
	} else {
		stream = &dice->rx_stream[index];
		resources = &dice->rx_resources[index];
	}

	amdtp_stream_destroy(stream);
	fw_iso_resources_destroy(resources);
}

int snd_dice_stream_init_duplex(struct snd_dice *dice)
{
	int i, err;

	for (i = 0; i < MAX_STREAMS; i++) {
		err = init_stream(dice, AMDTP_IN_STREAM, i);
		if (err < 0) {
			for (; i >= 0; i--)
				destroy_stream(dice, AMDTP_IN_STREAM, i);
			goto end;
		}
	}

	for (i = 0; i < MAX_STREAMS; i++) {
		err = init_stream(dice, AMDTP_OUT_STREAM, i);
		if (err < 0) {
			for (; i >= 0; i--)
				destroy_stream(dice, AMDTP_OUT_STREAM, i);
			for (i = 0; i < MAX_STREAMS; i++)
				destroy_stream(dice, AMDTP_IN_STREAM, i);
<<<<<<< HEAD
			break;
=======
			goto end;
		}
	}

	err = amdtp_domain_init(&dice->domain);
	if (err < 0) {
		for (i = 0; i < MAX_STREAMS; ++i) {
			destroy_stream(dice, AMDTP_OUT_STREAM, i);
			destroy_stream(dice, AMDTP_IN_STREAM, i);
>>>>>>> upstream/android-13
		}
	}
end:
	return err;
}

void snd_dice_stream_destroy_duplex(struct snd_dice *dice)
{
	unsigned int i;

	for (i = 0; i < MAX_STREAMS; i++) {
		destroy_stream(dice, AMDTP_IN_STREAM, i);
		destroy_stream(dice, AMDTP_OUT_STREAM, i);
	}
<<<<<<< HEAD
=======

	amdtp_domain_destroy(&dice->domain);
>>>>>>> upstream/android-13
}

void snd_dice_stream_update_duplex(struct snd_dice *dice)
{
	struct reg_params tx_params, rx_params;

	/*
	 * On a bus reset, the DICE firmware disables streaming and then goes
	 * off contemplating its own navel for hundreds of milliseconds before
	 * it can react to any of our attempts to reenable streaming.  This
	 * means that we lose synchronization anyway, so we force our streams
	 * to stop so that the application can restart them in an orderly
	 * manner.
	 */
	dice->global_enabled = false;

	if (get_register_params(dice, &tx_params, &rx_params) == 0) {
<<<<<<< HEAD
=======
		amdtp_domain_stop(&dice->domain);

>>>>>>> upstream/android-13
		stop_streams(dice, AMDTP_IN_STREAM, &tx_params);
		stop_streams(dice, AMDTP_OUT_STREAM, &rx_params);
	}
}

int snd_dice_stream_detect_current_formats(struct snd_dice *dice)
{
	unsigned int rate;
	enum snd_dice_rate_mode mode;
	__be32 reg[2];
	struct reg_params tx_params, rx_params;
	int i;
	int err;

	/* If extended protocol is available, detect detail spec. */
	err = snd_dice_detect_extension_formats(dice);
	if (err >= 0)
		return err;

	/*
	 * Available stream format is restricted at current mode of sampling
	 * clock.
	 */
	err = snd_dice_transaction_get_rate(dice, &rate);
	if (err < 0)
		return err;

	err = snd_dice_stream_get_rate_mode(dice, rate, &mode);
	if (err < 0)
		return err;

	/*
	 * Just after owning the unit (GLOBAL_OWNER), the unit can return
	 * invalid stream formats. Selecting clock parameters have an effect
	 * for the unit to refine it.
	 */
<<<<<<< HEAD
	err = ensure_phase_lock(dice, rate);
=======
	err = select_clock(dice, rate);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	err = get_register_params(dice, &tx_params, &rx_params);
	if (err < 0)
		return err;

	for (i = 0; i < tx_params.count; ++i) {
		err = snd_dice_transaction_read_tx(dice,
				tx_params.size * i + TX_NUMBER_AUDIO,
				reg, sizeof(reg));
		if (err < 0)
			return err;
		dice->tx_pcm_chs[i][mode] = be32_to_cpu(reg[0]);
		dice->tx_midi_ports[i] = max_t(unsigned int,
				be32_to_cpu(reg[1]), dice->tx_midi_ports[i]);
	}
	for (i = 0; i < rx_params.count; ++i) {
		err = snd_dice_transaction_read_rx(dice,
				rx_params.size * i + RX_NUMBER_AUDIO,
				reg, sizeof(reg));
		if (err < 0)
			return err;
		dice->rx_pcm_chs[i][mode] = be32_to_cpu(reg[0]);
		dice->rx_midi_ports[i] = max_t(unsigned int,
				be32_to_cpu(reg[1]), dice->rx_midi_ports[i]);
	}

	return 0;
}

static void dice_lock_changed(struct snd_dice *dice)
{
	dice->dev_lock_changed = true;
	wake_up(&dice->hwdep_wait);
}

int snd_dice_stream_lock_try(struct snd_dice *dice)
{
	int err;

	spin_lock_irq(&dice->lock);

	if (dice->dev_lock_count < 0) {
		err = -EBUSY;
		goto out;
	}

	if (dice->dev_lock_count++ == 0)
		dice_lock_changed(dice);
	err = 0;
out:
	spin_unlock_irq(&dice->lock);
	return err;
}

void snd_dice_stream_lock_release(struct snd_dice *dice)
{
	spin_lock_irq(&dice->lock);

	if (WARN_ON(dice->dev_lock_count <= 0))
		goto out;

	if (--dice->dev_lock_count == 0)
		dice_lock_changed(dice);
out:
	spin_unlock_irq(&dice->lock);
}
