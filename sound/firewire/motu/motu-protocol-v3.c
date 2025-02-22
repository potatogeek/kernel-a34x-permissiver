<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * motu-protocol-v3.c - a part of driver for MOTU FireWire series
 *
 * Copyright (c) 2015-2017 Takashi Sakamoto <o-takashi@sakamocchi.jp>
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#include <linux/delay.h>
#include "motu.h"

#define V3_CLOCK_STATUS_OFFSET		0x0b14
#define  V3_FETCH_PCM_FRAMES		0x02000000
#define  V3_CLOCK_RATE_MASK		0x0000ff00
#define  V3_CLOCK_RATE_SHIFT		8
#define  V3_CLOCK_SOURCE_MASK		0x000000ff
<<<<<<< HEAD
=======
#define   V3_CLOCK_SRC_INTERNAL		0x00
#define   V3_CLOCK_SRC_WORD_ON_BNC	0x01
#define   V3_CLOCK_SRC_SPH		0x02
#define   V3_CLOCK_SRC_SPDIF_ON_COAX	0x10
#define   V3_CLOCK_SRC_OPT_IFACE_A	0x18
#define   V3_CLOCK_SRC_OPT_IFACE_B	0x19
>>>>>>> upstream/android-13

#define V3_OPT_IFACE_MODE_OFFSET	0x0c94
#define  V3_ENABLE_OPT_IN_IFACE_A	0x00000001
#define  V3_ENABLE_OPT_IN_IFACE_B	0x00000002
#define  V3_ENABLE_OPT_OUT_IFACE_A	0x00000100
#define  V3_ENABLE_OPT_OUT_IFACE_B	0x00000200
#define  V3_NO_ADAT_OPT_IN_IFACE_A	0x00010000
#define  V3_NO_ADAT_OPT_IN_IFACE_B	0x00100000
#define  V3_NO_ADAT_OPT_OUT_IFACE_A	0x00040000
#define  V3_NO_ADAT_OPT_OUT_IFACE_B	0x00400000

<<<<<<< HEAD
static int v3_get_clock_rate(struct snd_motu *motu, unsigned int *rate)
=======
#define V3_MSG_FLAG_CLK_CHANGED		0x00000002
#define V3_CLK_WAIT_MSEC		4000

int snd_motu_protocol_v3_get_clock_rate(struct snd_motu *motu,
					unsigned int *rate)
>>>>>>> upstream/android-13
{
	__be32 reg;
	u32 data;
	int err;

	err = snd_motu_transaction_read(motu, V3_CLOCK_STATUS_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;
	data = be32_to_cpu(reg);

	data = (data & V3_CLOCK_RATE_MASK) >> V3_CLOCK_RATE_SHIFT;
	if (data >= ARRAY_SIZE(snd_motu_clock_rates))
		return -EIO;

	*rate = snd_motu_clock_rates[data];

	return 0;
}

<<<<<<< HEAD
static int v3_set_clock_rate(struct snd_motu *motu, unsigned int rate)
=======
int snd_motu_protocol_v3_set_clock_rate(struct snd_motu *motu,
					unsigned int rate)
>>>>>>> upstream/android-13
{
	__be32 reg;
	u32 data;
	bool need_to_wait;
	int i, err;

	for (i = 0; i < ARRAY_SIZE(snd_motu_clock_rates); ++i) {
		if (snd_motu_clock_rates[i] == rate)
			break;
	}
	if (i == ARRAY_SIZE(snd_motu_clock_rates))
		return -EINVAL;

	err = snd_motu_transaction_read(motu, V3_CLOCK_STATUS_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;
	data = be32_to_cpu(reg);

	data &= ~(V3_CLOCK_RATE_MASK | V3_FETCH_PCM_FRAMES);
	data |= i << V3_CLOCK_RATE_SHIFT;

	need_to_wait = data != be32_to_cpu(reg);

	reg = cpu_to_be32(data);
	err = snd_motu_transaction_write(motu, V3_CLOCK_STATUS_OFFSET, &reg,
					 sizeof(reg));
	if (err < 0)
		return err;

	if (need_to_wait) {
<<<<<<< HEAD
		/* Cost expensive. */
		if (msleep_interruptible(4000) > 0)
			return -EINTR;
=======
		int result;

		motu->msg = 0;
		result = wait_event_interruptible_timeout(motu->hwdep_wait,
					motu->msg & V3_MSG_FLAG_CLK_CHANGED,
					msecs_to_jiffies(V3_CLK_WAIT_MSEC));
		if (result < 0)
			return result;
		if (result == 0)
			return -ETIMEDOUT;
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
static int v3_get_clock_source(struct snd_motu *motu,
			       enum snd_motu_clock_source *src)
{
	__be32 reg;
	u32 data;
	unsigned int val;
=======
int snd_motu_protocol_v3_get_clock_source(struct snd_motu *motu,
					  enum snd_motu_clock_source *src)
{
	__be32 reg;
	u32 data;
>>>>>>> upstream/android-13
	int err;

	err = snd_motu_transaction_read(motu, V3_CLOCK_STATUS_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;
<<<<<<< HEAD
	data = be32_to_cpu(reg);

	val = data & V3_CLOCK_SOURCE_MASK;
	if (val == 0x00) {
		*src = SND_MOTU_CLOCK_SOURCE_INTERNAL;
	} else if (val == 0x01) {
		*src = SND_MOTU_CLOCK_SOURCE_WORD_ON_BNC;
	} else if (val == 0x10) {
		*src = SND_MOTU_CLOCK_SOURCE_SPDIF_ON_COAX;
	} else if (val == 0x18 || val == 0x19) {
		err = snd_motu_transaction_read(motu, V3_OPT_IFACE_MODE_OFFSET,
						&reg, sizeof(reg));
		if (err < 0)
			return err;
		data = be32_to_cpu(reg);

		if (val == 0x18) {
			if (data & V3_NO_ADAT_OPT_IN_IFACE_A)
=======
	data = be32_to_cpu(reg) & V3_CLOCK_SOURCE_MASK;

	switch (data) {
	case V3_CLOCK_SRC_INTERNAL:
		*src = SND_MOTU_CLOCK_SOURCE_INTERNAL;
		break;
	case V3_CLOCK_SRC_WORD_ON_BNC:
		*src = SND_MOTU_CLOCK_SOURCE_WORD_ON_BNC;
		break;
	case V3_CLOCK_SRC_SPH:
		*src = SND_MOTU_CLOCK_SOURCE_SPH;
		break;
	case V3_CLOCK_SRC_SPDIF_ON_COAX:
		*src = SND_MOTU_CLOCK_SOURCE_SPDIF_ON_COAX;
		break;
	case V3_CLOCK_SRC_OPT_IFACE_A:
	case V3_CLOCK_SRC_OPT_IFACE_B:
	{
		__be32 reg;
		u32 options;

		err = snd_motu_transaction_read(motu,
				V3_OPT_IFACE_MODE_OFFSET, &reg, sizeof(reg));
		if (err < 0)
			return err;
		options = be32_to_cpu(reg);

		if (data == V3_CLOCK_SRC_OPT_IFACE_A) {
			if (options & V3_NO_ADAT_OPT_IN_IFACE_A)
>>>>>>> upstream/android-13
				*src = SND_MOTU_CLOCK_SOURCE_SPDIF_ON_OPT_A;
			else
				*src = SND_MOTU_CLOCK_SOURCE_ADAT_ON_OPT_A;
		} else {
<<<<<<< HEAD
			if (data & V3_NO_ADAT_OPT_IN_IFACE_B)
=======
			if (options & V3_NO_ADAT_OPT_IN_IFACE_B)
>>>>>>> upstream/android-13
				*src = SND_MOTU_CLOCK_SOURCE_SPDIF_ON_OPT_B;
			else
				*src = SND_MOTU_CLOCK_SOURCE_ADAT_ON_OPT_B;
		}
<<<<<<< HEAD
	} else {
		*src = SND_MOTU_CLOCK_SOURCE_UNKNOWN;
=======
		break;
	}
	default:
		*src = SND_MOTU_CLOCK_SOURCE_UNKNOWN;
		break;
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
static int v3_switch_fetching_mode(struct snd_motu *motu, bool enable)
=======
int snd_motu_protocol_v3_switch_fetching_mode(struct snd_motu *motu,
					      bool enable)
>>>>>>> upstream/android-13
{
	__be32 reg;
	u32 data;
	int err;

	err = snd_motu_transaction_read(motu, V3_CLOCK_STATUS_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return 0;
	data = be32_to_cpu(reg);

	if (enable)
		data |= V3_FETCH_PCM_FRAMES;
	else
		data &= ~V3_FETCH_PCM_FRAMES;

	reg = cpu_to_be32(data);
	return snd_motu_transaction_write(motu, V3_CLOCK_STATUS_OFFSET, &reg,
					  sizeof(reg));
}

<<<<<<< HEAD
static void calculate_fixed_part(struct snd_motu_packet_format *formats,
				 enum amdtp_stream_direction dir,
				 enum snd_motu_spec_flags flags,
				 unsigned char analog_ports)
{
	unsigned char pcm_chunks[3] = {0, 0, 0};

	formats->msg_chunks = 2;

	pcm_chunks[0] = analog_ports;
	pcm_chunks[1] = analog_ports;
	if (flags & SND_MOTU_SPEC_SUPPORT_CLOCK_X4)
		pcm_chunks[2] = analog_ports;

	if (dir == AMDTP_IN_STREAM) {
		if (flags & SND_MOTU_SPEC_TX_MICINST_CHUNK) {
			pcm_chunks[0] += 2;
			pcm_chunks[1] += 2;
			if (flags & SND_MOTU_SPEC_SUPPORT_CLOCK_X4)
				pcm_chunks[2] += 2;
		}

		if (flags & SND_MOTU_SPEC_TX_RETURN_CHUNK) {
			pcm_chunks[0] += 2;
			pcm_chunks[1] += 2;
			if (flags & SND_MOTU_SPEC_SUPPORT_CLOCK_X4)
				pcm_chunks[2] += 2;
		}

		if (flags & SND_MOTU_SPEC_TX_REVERB_CHUNK) {
			pcm_chunks[0] += 2;
			pcm_chunks[1] += 2;
		}
	} else {
		if (flags & SND_MOTU_SPEC_RX_SEPARETED_MAIN) {
			pcm_chunks[0] += 2;
			pcm_chunks[1] += 2;
		}

		// Packets to v3 units include 2 chunks for phone 1/2, except
		// for 176.4/192.0 kHz.
		pcm_chunks[0] += 2;
		pcm_chunks[1] += 2;
	}

	if (flags & SND_MOTU_SPEC_HAS_AESEBU_IFACE) {
		pcm_chunks[0] += 2;
		pcm_chunks[1] += 2;
	}

	/*
	 * At least, packets have two data chunks for S/PDIF on coaxial
	 * interface.
	 */
	pcm_chunks[0] += 2;
	pcm_chunks[1] += 2;

	/*
	 * Fixed part consists of PCM chunks multiple of 4, with msg chunks. As
	 * a result, this part can includes empty data chunks.
	 */
	formats->fixed_part_pcm_chunks[0] = round_up(2 + pcm_chunks[0], 4) - 2;
	formats->fixed_part_pcm_chunks[1] = round_up(2 + pcm_chunks[1], 4) - 2;
	if (flags & SND_MOTU_SPEC_SUPPORT_CLOCK_X4)
		formats->fixed_part_pcm_chunks[2] =
					round_up(2 + pcm_chunks[2], 4) - 2;
}

static void calculate_differed_part(struct snd_motu_packet_format *formats,
				    enum snd_motu_spec_flags flags, u32 data,
				    u32 a_enable_mask, u32 a_no_adat_mask,
				    u32 b_enable_mask, u32 b_no_adat_mask)
{
	unsigned char pcm_chunks[3] = {0, 0, 0};
	int i;

	if ((flags & SND_MOTU_SPEC_HAS_OPT_IFACE_A) && (data & a_enable_mask)) {
		if (data & a_no_adat_mask) {
			/*
			 * Additional two data chunks for S/PDIF on optical
			 * interface A. This includes empty data chunks.
			 */
			pcm_chunks[0] += 4;
			pcm_chunks[1] += 4;
		} else {
			/*
			 * Additional data chunks for ADAT on optical interface
			 * A.
			 */
			pcm_chunks[0] += 8;
			pcm_chunks[1] += 4;
		}
	}

	if ((flags & SND_MOTU_SPEC_HAS_OPT_IFACE_B) && (data & b_enable_mask)) {
		if (data & b_no_adat_mask) {
			/*
			 * Additional two data chunks for S/PDIF on optical
			 * interface B. This includes empty data chunks.
			 */
			pcm_chunks[0] += 4;
			pcm_chunks[1] += 4;
		} else {
			/*
			 * Additional data chunks for ADAT on optical interface
			 * B.
			 */
			pcm_chunks[0] += 8;
			pcm_chunks[1] += 4;
		}
	}

	for (i = 0; i < 3; ++i) {
		if (pcm_chunks[i] > 0)
			pcm_chunks[i] = round_up(pcm_chunks[i], 4);

		formats->differed_part_pcm_chunks[i] = pcm_chunks[i];
	}
}

static int v3_cache_packet_formats(struct snd_motu *motu)
=======
static int detect_packet_formats_828mk3(struct snd_motu *motu, u32 data)
{
	if (data & V3_ENABLE_OPT_IN_IFACE_A) {
		if (data & V3_NO_ADAT_OPT_IN_IFACE_A) {
			motu->tx_packet_formats.pcm_chunks[0] += 4;
			motu->tx_packet_formats.pcm_chunks[1] += 4;
		} else {
			motu->tx_packet_formats.pcm_chunks[0] += 8;
			motu->tx_packet_formats.pcm_chunks[1] += 4;
		}
	}

	if (data & V3_ENABLE_OPT_IN_IFACE_B) {
		if (data & V3_NO_ADAT_OPT_IN_IFACE_B) {
			motu->tx_packet_formats.pcm_chunks[0] += 4;
			motu->tx_packet_formats.pcm_chunks[1] += 4;
		} else {
			motu->tx_packet_formats.pcm_chunks[0] += 8;
			motu->tx_packet_formats.pcm_chunks[1] += 4;
		}
	}

	if (data & V3_ENABLE_OPT_OUT_IFACE_A) {
		if (data & V3_NO_ADAT_OPT_OUT_IFACE_A) {
			motu->rx_packet_formats.pcm_chunks[0] += 4;
			motu->rx_packet_formats.pcm_chunks[1] += 4;
		} else {
			motu->rx_packet_formats.pcm_chunks[0] += 8;
			motu->rx_packet_formats.pcm_chunks[1] += 4;
		}
	}

	if (data & V3_ENABLE_OPT_OUT_IFACE_B) {
		if (data & V3_NO_ADAT_OPT_OUT_IFACE_B) {
			motu->rx_packet_formats.pcm_chunks[0] += 4;
			motu->rx_packet_formats.pcm_chunks[1] += 4;
		} else {
			motu->rx_packet_formats.pcm_chunks[0] += 8;
			motu->rx_packet_formats.pcm_chunks[1] += 4;
		}
	}

	return 0;
}

int snd_motu_protocol_v3_cache_packet_formats(struct snd_motu *motu)
>>>>>>> upstream/android-13
{
	__be32 reg;
	u32 data;
	int err;

<<<<<<< HEAD
=======
	motu->tx_packet_formats.pcm_byte_offset = 10;
	motu->rx_packet_formats.pcm_byte_offset = 10;

	motu->tx_packet_formats.msg_chunks = 2;
	motu->rx_packet_formats.msg_chunks = 2;

>>>>>>> upstream/android-13
	err = snd_motu_transaction_read(motu, V3_OPT_IFACE_MODE_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;
	data = be32_to_cpu(reg);

<<<<<<< HEAD
	calculate_fixed_part(&motu->tx_packet_formats, AMDTP_IN_STREAM,
			     motu->spec->flags, motu->spec->analog_in_ports);
	calculate_differed_part(&motu->tx_packet_formats,
			motu->spec->flags, data,
			V3_ENABLE_OPT_IN_IFACE_A, V3_NO_ADAT_OPT_IN_IFACE_A,
			V3_ENABLE_OPT_IN_IFACE_B, V3_NO_ADAT_OPT_IN_IFACE_B);

	calculate_fixed_part(&motu->rx_packet_formats, AMDTP_OUT_STREAM,
			     motu->spec->flags, motu->spec->analog_out_ports);
	calculate_differed_part(&motu->rx_packet_formats,
			motu->spec->flags, data,
			V3_ENABLE_OPT_OUT_IFACE_A, V3_NO_ADAT_OPT_OUT_IFACE_A,
			V3_ENABLE_OPT_OUT_IFACE_B, V3_NO_ADAT_OPT_OUT_IFACE_B);

	motu->tx_packet_formats.pcm_byte_offset = 10;
	motu->rx_packet_formats.pcm_byte_offset = 10;

	return 0;
}

const struct snd_motu_protocol snd_motu_protocol_v3 = {
	.get_clock_rate		= v3_get_clock_rate,
	.set_clock_rate		= v3_set_clock_rate,
	.get_clock_source	= v3_get_clock_source,
	.switch_fetching_mode	= v3_switch_fetching_mode,
	.cache_packet_formats	= v3_cache_packet_formats,
=======
	memcpy(motu->tx_packet_formats.pcm_chunks,
	       motu->spec->tx_fixed_pcm_chunks,
	       sizeof(motu->tx_packet_formats.pcm_chunks));
	memcpy(motu->rx_packet_formats.pcm_chunks,
	       motu->spec->rx_fixed_pcm_chunks,
	       sizeof(motu->rx_packet_formats.pcm_chunks));

	if (motu->spec == &snd_motu_spec_828mk3_fw || motu->spec == &snd_motu_spec_828mk3_hybrid)
		return detect_packet_formats_828mk3(motu, data);
	else
		return 0;
}


const struct snd_motu_spec snd_motu_spec_828mk3_fw = {
	.name = "828mk3",
	.protocol_version = SND_MOTU_PROTOCOL_V3,
	.flags = SND_MOTU_SPEC_RX_MIDI_3RD_Q |
		 SND_MOTU_SPEC_TX_MIDI_3RD_Q,
	.tx_fixed_pcm_chunks = {18, 18, 14},
	.rx_fixed_pcm_chunks = {14, 14, 10},
};

const struct snd_motu_spec snd_motu_spec_828mk3_hybrid = {
	.name = "828mk3",
	.protocol_version = SND_MOTU_PROTOCOL_V3,
	.flags = SND_MOTU_SPEC_RX_MIDI_3RD_Q |
		 SND_MOTU_SPEC_TX_MIDI_3RD_Q,
	.tx_fixed_pcm_chunks = {18, 18, 14},
	.rx_fixed_pcm_chunks = {14, 14, 14},	// Additional 4 dummy chunks at higher rate.
};

const struct snd_motu_spec snd_motu_spec_ultralite_mk3 = {
	.name = "UltraLiteMk3",
	.protocol_version = SND_MOTU_PROTOCOL_V3,
	.flags = SND_MOTU_SPEC_RX_MIDI_3RD_Q |
		 SND_MOTU_SPEC_TX_MIDI_3RD_Q,
	.tx_fixed_pcm_chunks = {18, 14, 10},
	.rx_fixed_pcm_chunks = {14, 14, 14},
};

const struct snd_motu_spec snd_motu_spec_audio_express = {
	.name = "AudioExpress",
	.protocol_version = SND_MOTU_PROTOCOL_V3,
	.flags = SND_MOTU_SPEC_RX_MIDI_2ND_Q |
		 SND_MOTU_SPEC_TX_MIDI_3RD_Q,
	.tx_fixed_pcm_chunks = {10, 10, 0},
	.rx_fixed_pcm_chunks = {10, 10, 0},
};

const struct snd_motu_spec snd_motu_spec_4pre = {
	.name = "4pre",
	.protocol_version = SND_MOTU_PROTOCOL_V3,
	.tx_fixed_pcm_chunks = {10, 10, 0},
	.rx_fixed_pcm_chunks = {10, 10, 0},
>>>>>>> upstream/android-13
};
