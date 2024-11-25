<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * motu-protocol-v2.c - a part of driver for MOTU FireWire series
 *
 * Copyright (c) 2015-2017 Takashi Sakamoto <o-takashi@sakamocchi.jp>
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#include "motu.h"

#define V2_CLOCK_STATUS_OFFSET			0x0b14
#define  V2_CLOCK_RATE_MASK			0x00000038
#define  V2_CLOCK_RATE_SHIFT			3
#define  V2_CLOCK_SRC_MASK			0x00000007
#define  V2_CLOCK_SRC_SHIFT			0
<<<<<<< HEAD
#define  V2_CLOCK_TRAVELER_FETCH_DISABLE	0x04000000
#define  V2_CLOCK_TRAVELER_FETCH_ENABLE		0x03000000
=======
#define   V2_CLOCK_SRC_AESEBU_ON_XLR		0x07	// In Traveler.
#define   V2_CLOCK_SRC_ADAT_ON_DSUB		0x05
#define   V2_CLOCK_SRC_WORD_ON_BNC		0x04
#define   V2_CLOCK_SRC_SPH			0x03
#define   V2_CLOCK_SRC_SPDIF			0x02	// on either coaxial or optical. AES/EBU in 896HD.
#define   V2_CLOCK_SRC_ADAT_ON_OPT		0x01
#define   V2_CLOCK_SRC_INTERNAL			0x00
#define  V2_CLOCK_FETCH_ENABLE			0x02000000
#define  V2_CLOCK_MODEL_SPECIFIC		0x04000000
>>>>>>> upstream/android-13

#define V2_IN_OUT_CONF_OFFSET			0x0c04
#define  V2_OPT_OUT_IFACE_MASK			0x00000c00
#define  V2_OPT_OUT_IFACE_SHIFT			10
#define  V2_OPT_IN_IFACE_MASK			0x00000300
#define  V2_OPT_IN_IFACE_SHIFT			8
#define  V2_OPT_IFACE_MODE_NONE			0
#define  V2_OPT_IFACE_MODE_ADAT			1
#define  V2_OPT_IFACE_MODE_SPDIF		2

<<<<<<< HEAD
static int v2_get_clock_rate(struct snd_motu *motu, unsigned int *rate)
{
	__be32 reg;
	unsigned int index;
	int err;

	err = snd_motu_transaction_read(motu, V2_CLOCK_STATUS_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;

	index = (be32_to_cpu(reg) & V2_CLOCK_RATE_MASK) >> V2_CLOCK_RATE_SHIFT;
=======
static int get_clock_rate(u32 data, unsigned int *rate)
{
	unsigned int index = (data & V2_CLOCK_RATE_MASK) >> V2_CLOCK_RATE_SHIFT;
>>>>>>> upstream/android-13
	if (index >= ARRAY_SIZE(snd_motu_clock_rates))
		return -EIO;

	*rate = snd_motu_clock_rates[index];

	return 0;
}

<<<<<<< HEAD
static int v2_set_clock_rate(struct snd_motu *motu, unsigned int rate)
=======
int snd_motu_protocol_v2_get_clock_rate(struct snd_motu *motu,
					unsigned int *rate)
{
	__be32 reg;
	int err;

	err = snd_motu_transaction_read(motu, V2_CLOCK_STATUS_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;

	return get_clock_rate(be32_to_cpu(reg), rate);
}

int snd_motu_protocol_v2_set_clock_rate(struct snd_motu *motu,
					unsigned int rate)
>>>>>>> upstream/android-13
{
	__be32 reg;
	u32 data;
	int i;
	int err;

	for (i = 0; i < ARRAY_SIZE(snd_motu_clock_rates); ++i) {
		if (snd_motu_clock_rates[i] == rate)
			break;
	}
	if (i == ARRAY_SIZE(snd_motu_clock_rates))
		return -EINVAL;

	err = snd_motu_transaction_read(motu, V2_CLOCK_STATUS_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;
	data = be32_to_cpu(reg);

	data &= ~V2_CLOCK_RATE_MASK;
	data |= i << V2_CLOCK_RATE_SHIFT;

<<<<<<< HEAD
	if (motu->spec == &snd_motu_spec_traveler) {
		data &= ~V2_CLOCK_TRAVELER_FETCH_ENABLE;
		data |= V2_CLOCK_TRAVELER_FETCH_DISABLE;
	}

=======
>>>>>>> upstream/android-13
	reg = cpu_to_be32(data);
	return snd_motu_transaction_write(motu, V2_CLOCK_STATUS_OFFSET, &reg,
					  sizeof(reg));
}

<<<<<<< HEAD
static int v2_get_clock_source(struct snd_motu *motu,
			       enum snd_motu_clock_source *src)
{
	__be32 reg;
	unsigned int index;
=======
static int get_clock_source(struct snd_motu *motu, u32 data,
			    enum snd_motu_clock_source *src)
{
	switch (data & V2_CLOCK_SRC_MASK) {
	case V2_CLOCK_SRC_INTERNAL:
		*src = SND_MOTU_CLOCK_SOURCE_INTERNAL;
		break;
	case V2_CLOCK_SRC_ADAT_ON_OPT:
		*src = SND_MOTU_CLOCK_SOURCE_ADAT_ON_OPT;
		break;
	case V2_CLOCK_SRC_SPDIF:
	{
		bool support_iec60958_on_opt = (motu->spec == &snd_motu_spec_828mk2 ||
						motu->spec == &snd_motu_spec_traveler);

		if (motu->spec == &snd_motu_spec_896hd) {
			*src = SND_MOTU_CLOCK_SOURCE_AESEBU_ON_XLR;
		} else if (!support_iec60958_on_opt) {
			*src = SND_MOTU_CLOCK_SOURCE_SPDIF_ON_COAX;
		} else {
			__be32 reg;

			// To check the configuration of optical interface.
			int err = snd_motu_transaction_read(motu, V2_IN_OUT_CONF_OFFSET, &reg,
							    sizeof(reg));
			if (err < 0)
				return err;

			if (((data & V2_OPT_IN_IFACE_MASK) >> V2_OPT_IN_IFACE_SHIFT) ==
			    V2_OPT_IFACE_MODE_SPDIF)
				*src = SND_MOTU_CLOCK_SOURCE_SPDIF_ON_OPT;
			else
				*src = SND_MOTU_CLOCK_SOURCE_SPDIF_ON_COAX;
		}
		break;
	}
	case V2_CLOCK_SRC_SPH:
		*src = SND_MOTU_CLOCK_SOURCE_SPH;
		break;
	case V2_CLOCK_SRC_WORD_ON_BNC:
		*src = SND_MOTU_CLOCK_SOURCE_WORD_ON_BNC;
		break;
	case V2_CLOCK_SRC_ADAT_ON_DSUB:
		*src = SND_MOTU_CLOCK_SOURCE_ADAT_ON_DSUB;
		break;
	case V2_CLOCK_SRC_AESEBU_ON_XLR:
		// For Traveler.
		*src = SND_MOTU_CLOCK_SOURCE_AESEBU_ON_XLR;
		break;
	default:
		*src = SND_MOTU_CLOCK_SOURCE_UNKNOWN;
		break;
	}

	return 0;
}

int snd_motu_protocol_v2_get_clock_source(struct snd_motu *motu,
					  enum snd_motu_clock_source *src)
{
	__be32 reg;
>>>>>>> upstream/android-13
	int err;

	err = snd_motu_transaction_read(motu, V2_CLOCK_STATUS_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;

<<<<<<< HEAD
	index = be32_to_cpu(reg) & V2_CLOCK_SRC_MASK;
	if (index > 5)
		return -EIO;

	/* To check the configuration of optical interface. */
	err = snd_motu_transaction_read(motu, V2_IN_OUT_CONF_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;

	switch (index) {
	case 0:
		*src = SND_MOTU_CLOCK_SOURCE_INTERNAL;
		break;
	case 1:
		if (be32_to_cpu(reg) & 0x00000200)
			*src = SND_MOTU_CLOCK_SOURCE_SPDIF_ON_OPT;
		else
			*src = SND_MOTU_CLOCK_SOURCE_ADAT_ON_OPT;
		break;
	case 2:
		*src = SND_MOTU_CLOCK_SOURCE_SPDIF_ON_COAX;
		break;
	case 4:
		*src = SND_MOTU_CLOCK_SOURCE_WORD_ON_BNC;
		break;
	case 5:
		*src = SND_MOTU_CLOCK_SOURCE_ADAT_ON_DSUB;
		break;
	default:
		return -EIO;
	}
=======
	return get_clock_source(motu, be32_to_cpu(reg), src);
}

// Expected for Traveler, which implements Altera Cyclone EP1C3.
static int switch_fetching_mode_cyclone(struct snd_motu *motu, u32 *data,
					bool enable)
{
	*data |= V2_CLOCK_MODEL_SPECIFIC;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int v2_switch_fetching_mode(struct snd_motu *motu, bool enable)
{
	__be32 reg;
	u32 data;
	int err = 0;

	if (motu->spec == &snd_motu_spec_traveler) {
=======
// For UltraLite and 8pre, which implements Xilinx Spartan XC3S200.
static int switch_fetching_mode_spartan(struct snd_motu *motu, u32 *data,
					bool enable)
{
	unsigned int rate;
	enum snd_motu_clock_source src;
	int err;

	err = get_clock_source(motu, *data, &src);
	if (err < 0)
		return err;

	err = get_clock_rate(*data, &rate);
	if (err < 0)
		return err;

	if (src == SND_MOTU_CLOCK_SOURCE_SPH && rate > 48000)
		*data |= V2_CLOCK_MODEL_SPECIFIC;

	return 0;
}

int snd_motu_protocol_v2_switch_fetching_mode(struct snd_motu *motu,
					      bool enable)
{
	if (motu->spec == &snd_motu_spec_828mk2) {
		// 828mkII implements Altera ACEX 1K EP1K30. Nothing to do.
		return 0;
	} else if (motu->spec == &snd_motu_spec_896hd) {
		// 896HD implements Altera Cyclone EP1C3 but nothing to do.
		return 0;
	} else {
		__be32 reg;
		u32 data;
		int err;

>>>>>>> upstream/android-13
		err = snd_motu_transaction_read(motu, V2_CLOCK_STATUS_OFFSET,
						&reg, sizeof(reg));
		if (err < 0)
			return err;
		data = be32_to_cpu(reg);

<<<<<<< HEAD
		data &= ~(V2_CLOCK_TRAVELER_FETCH_DISABLE |
			  V2_CLOCK_TRAVELER_FETCH_ENABLE);

		if (enable)
			data |= V2_CLOCK_TRAVELER_FETCH_ENABLE;
		else
			data |= V2_CLOCK_TRAVELER_FETCH_DISABLE;

		reg = cpu_to_be32(data);
		err = snd_motu_transaction_write(motu, V2_CLOCK_STATUS_OFFSET,
						 &reg, sizeof(reg));
	}

	return err;
}

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
		}
		if (flags & SND_MOTU_SPEC_TX_RETURN_CHUNK) {
			pcm_chunks[0] += 2;
			pcm_chunks[1] += 2;
		}
	} else {
		if (flags & SND_MOTU_SPEC_RX_SEPARETED_MAIN) {
			pcm_chunks[0] += 2;
			pcm_chunks[1] += 2;
		}

		// Packets to v2 units include 2 chunks for phone 1/2, except
		// for 176.4/192.0 kHz.
		pcm_chunks[0] += 2;
		pcm_chunks[1] += 2;
	}

	if (flags & SND_MOTU_SPEC_HAS_AESEBU_IFACE) {
		pcm_chunks[0] += 2;
		pcm_chunks[1] += 2;
	}

	/*
	 * All of v2 models have a pair of coaxial interfaces for digital in/out
	 * port. At 44.1/48.0/88.2/96.0 kHz, packets includes PCM from these
	 * ports.
	 */
	pcm_chunks[0] += 2;
	pcm_chunks[1] += 2;

	formats->fixed_part_pcm_chunks[0] = pcm_chunks[0];
	formats->fixed_part_pcm_chunks[1] = pcm_chunks[1];
	formats->fixed_part_pcm_chunks[2] = pcm_chunks[2];
}

static void calculate_differed_part(struct snd_motu_packet_format *formats,
				    enum snd_motu_spec_flags flags,
				    u32 data, u32 mask, u32 shift)
{
	unsigned char pcm_chunks[2] = {0, 0};

	/*
	 * When optical interfaces are configured for S/PDIF (TOSLINK),
	 * the above PCM frames come from them, instead of coaxial
	 * interfaces.
	 */
	data = (data & mask) >> shift;
	if ((flags & SND_MOTU_SPEC_HAS_OPT_IFACE_A) &&
	    data == V2_OPT_IFACE_MODE_ADAT) {
		pcm_chunks[0] += 8;
		pcm_chunks[1] += 4;
	}

	/* At mode x4, no data chunks are supported in this part. */
	formats->differed_part_pcm_chunks[0] = pcm_chunks[0];
	formats->differed_part_pcm_chunks[1] = pcm_chunks[1];
}

static int v2_cache_packet_formats(struct snd_motu *motu)
{
=======
		data &= ~(V2_CLOCK_FETCH_ENABLE | V2_CLOCK_MODEL_SPECIFIC);
		if (enable)
			data |= V2_CLOCK_FETCH_ENABLE;

		if (motu->spec == &snd_motu_spec_traveler)
			err = switch_fetching_mode_cyclone(motu, &data, enable);
		else
			err = switch_fetching_mode_spartan(motu, &data, enable);
		if (err < 0)
			return err;

		reg = cpu_to_be32(data);
		return snd_motu_transaction_write(motu, V2_CLOCK_STATUS_OFFSET,
						  &reg, sizeof(reg));
	}
}

int snd_motu_protocol_v2_cache_packet_formats(struct snd_motu *motu)
{
	bool has_two_opt_ifaces = (motu->spec == &snd_motu_spec_8pre);
>>>>>>> upstream/android-13
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
	err = snd_motu_transaction_read(motu, V2_IN_OUT_CONF_OFFSET, &reg,
					sizeof(reg));
	if (err < 0)
		return err;
	data = be32_to_cpu(reg);

<<<<<<< HEAD
	calculate_fixed_part(&motu->tx_packet_formats, AMDTP_IN_STREAM,
			     motu->spec->flags, motu->spec->analog_in_ports);
	calculate_differed_part(&motu->tx_packet_formats, motu->spec->flags,
			data, V2_OPT_IN_IFACE_MASK, V2_OPT_IN_IFACE_SHIFT);

	calculate_fixed_part(&motu->rx_packet_formats, AMDTP_OUT_STREAM,
			     motu->spec->flags, motu->spec->analog_out_ports);
	calculate_differed_part(&motu->rx_packet_formats, motu->spec->flags,
			data, V2_OPT_OUT_IFACE_MASK, V2_OPT_OUT_IFACE_SHIFT);

	motu->tx_packet_formats.pcm_byte_offset = 10;
	motu->rx_packet_formats.pcm_byte_offset = 10;
=======
	memcpy(motu->tx_packet_formats.pcm_chunks,
	       motu->spec->tx_fixed_pcm_chunks,
	       sizeof(motu->tx_packet_formats.pcm_chunks));
	memcpy(motu->rx_packet_formats.pcm_chunks,
	       motu->spec->rx_fixed_pcm_chunks,
	       sizeof(motu->rx_packet_formats.pcm_chunks));

	if (((data & V2_OPT_IN_IFACE_MASK) >> V2_OPT_IN_IFACE_SHIFT) == V2_OPT_IFACE_MODE_ADAT) {
		motu->tx_packet_formats.pcm_chunks[0] += 8;

		if (!has_two_opt_ifaces)
			motu->tx_packet_formats.pcm_chunks[1] += 4;
		else
			motu->tx_packet_formats.pcm_chunks[1] += 8;
	}

	if (((data & V2_OPT_OUT_IFACE_MASK) >> V2_OPT_OUT_IFACE_SHIFT) == V2_OPT_IFACE_MODE_ADAT) {
		motu->rx_packet_formats.pcm_chunks[0] += 8;

		if (!has_two_opt_ifaces)
			motu->rx_packet_formats.pcm_chunks[1] += 4;
		else
			motu->rx_packet_formats.pcm_chunks[1] += 8;
	}
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
const struct snd_motu_protocol snd_motu_protocol_v2 = {
	.get_clock_rate		= v2_get_clock_rate,
	.set_clock_rate		= v2_set_clock_rate,
	.get_clock_source	= v2_get_clock_source,
	.switch_fetching_mode	= v2_switch_fetching_mode,
	.cache_packet_formats	= v2_cache_packet_formats,
=======
const struct snd_motu_spec snd_motu_spec_828mk2 = {
	.name = "828mk2",
	.protocol_version = SND_MOTU_PROTOCOL_V2,
	.flags = SND_MOTU_SPEC_RX_MIDI_2ND_Q |
		 SND_MOTU_SPEC_TX_MIDI_2ND_Q,
	.tx_fixed_pcm_chunks = {14, 14, 0},
	.rx_fixed_pcm_chunks = {14, 14, 0},
};

const struct snd_motu_spec snd_motu_spec_896hd = {
	.name = "896HD",
	.protocol_version = SND_MOTU_PROTOCOL_V2,
	// No support for MIDI.
	.tx_fixed_pcm_chunks = {14, 14, 8},
	.rx_fixed_pcm_chunks = {14, 14, 8},
};

const struct snd_motu_spec snd_motu_spec_traveler = {
	.name = "Traveler",
	.protocol_version = SND_MOTU_PROTOCOL_V2,
	.flags = SND_MOTU_SPEC_RX_MIDI_2ND_Q |
		 SND_MOTU_SPEC_TX_MIDI_2ND_Q,
	.tx_fixed_pcm_chunks = {14, 14, 8},
	.rx_fixed_pcm_chunks = {14, 14, 8},
};

const struct snd_motu_spec snd_motu_spec_ultralite = {
	.name = "UltraLite",
	.protocol_version = SND_MOTU_PROTOCOL_V2,
	.flags = SND_MOTU_SPEC_RX_MIDI_2ND_Q |
		 SND_MOTU_SPEC_TX_MIDI_2ND_Q,
	.tx_fixed_pcm_chunks = {14, 14, 0},
	.rx_fixed_pcm_chunks = {14, 14, 0},
};

const struct snd_motu_spec snd_motu_spec_8pre = {
	.name = "8pre",
	.protocol_version = SND_MOTU_PROTOCOL_V2,
	.flags = SND_MOTU_SPEC_RX_MIDI_2ND_Q |
		 SND_MOTU_SPEC_TX_MIDI_2ND_Q,
	// Two dummy chunks always in the end of data block.
	.tx_fixed_pcm_chunks = {10, 10, 0},
	.rx_fixed_pcm_chunks = {6, 6, 0},
>>>>>>> upstream/android-13
};
