<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * motu.h - a part of driver for MOTU FireWire series
 *
 * Copyright (c) 2015-2017 Takashi Sakamoto <o-takashi@sakamocchi.jp>
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#ifndef SOUND_FIREWIRE_MOTU_H_INCLUDED
#define SOUND_FIREWIRE_MOTU_H_INCLUDED

#include <linux/device.h>
#include <linux/firewire.h>
#include <linux/firewire-constants.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/sched/signal.h>

#include <sound/control.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/info.h>
#include <sound/rawmidi.h>
#include <sound/firewire.h>
#include <sound/hwdep.h>

#include "../lib.h"
#include "../amdtp-stream.h"
#include "../iso-resources.h"

struct snd_motu_packet_format {
	unsigned char midi_flag_offset;
	unsigned char midi_byte_offset;
	unsigned char pcm_byte_offset;

	unsigned char msg_chunks;
<<<<<<< HEAD
	unsigned char fixed_part_pcm_chunks[3];
	unsigned char differed_part_pcm_chunks[3];
=======
	unsigned char pcm_chunks[3];
};

struct amdtp_motu_cache {
	unsigned int *event_offsets;
	unsigned int size;
	unsigned int tail;
	unsigned int tx_cycle_count;
	unsigned int head;
	unsigned int rx_cycle_count;
>>>>>>> upstream/android-13
};

struct snd_motu {
	struct snd_card *card;
	struct fw_unit *unit;
	struct mutex mutex;
	spinlock_t lock;

<<<<<<< HEAD
	bool registered;
	struct delayed_work dwork;

=======
>>>>>>> upstream/android-13
	/* Model dependent information. */
	const struct snd_motu_spec *spec;

	/* For packet streaming */
	struct snd_motu_packet_format tx_packet_formats;
	struct snd_motu_packet_format rx_packet_formats;
	struct amdtp_stream tx_stream;
	struct amdtp_stream rx_stream;
	struct fw_iso_resources tx_resources;
	struct fw_iso_resources rx_resources;
<<<<<<< HEAD
	unsigned int capture_substreams;
	unsigned int playback_substreams;
=======
	unsigned int substreams_counter;
>>>>>>> upstream/android-13

	/* For notification. */
	struct fw_address_handler async_handler;
	u32 msg;

	/* For uapi */
	int dev_lock_count;
	bool dev_lock_changed;
	wait_queue_head_t hwdep_wait;
<<<<<<< HEAD
};

enum snd_motu_spec_flags {
	SND_MOTU_SPEC_SUPPORT_CLOCK_X2	= 0x0001,
	SND_MOTU_SPEC_SUPPORT_CLOCK_X4	= 0x0002,
	SND_MOTU_SPEC_TX_MICINST_CHUNK	= 0x0004,
	SND_MOTU_SPEC_TX_RETURN_CHUNK	= 0x0008,
	SND_MOTU_SPEC_TX_REVERB_CHUNK	= 0x0010,
	SND_MOTU_SPEC_HAS_AESEBU_IFACE	= 0x0020,
	SND_MOTU_SPEC_HAS_OPT_IFACE_A	= 0x0040,
	SND_MOTU_SPEC_HAS_OPT_IFACE_B	= 0x0080,
	SND_MOTU_SPEC_RX_MIDI_2ND_Q	= 0x0100,
	SND_MOTU_SPEC_RX_MIDI_3RD_Q	= 0x0200,
	SND_MOTU_SPEC_TX_MIDI_2ND_Q	= 0x0400,
	SND_MOTU_SPEC_TX_MIDI_3RD_Q	= 0x0800,
	SND_MOTU_SPEC_RX_SEPARETED_MAIN	= 0x1000,
=======

	struct amdtp_domain domain;

	struct amdtp_motu_cache cache;
};

enum snd_motu_spec_flags {
	SND_MOTU_SPEC_RX_MIDI_2ND_Q	= 0x0001,
	SND_MOTU_SPEC_RX_MIDI_3RD_Q	= 0x0002,
	SND_MOTU_SPEC_TX_MIDI_2ND_Q	= 0x0004,
	SND_MOTU_SPEC_TX_MIDI_3RD_Q	= 0x0008,
>>>>>>> upstream/android-13
};

#define SND_MOTU_CLOCK_RATE_COUNT	6
extern const unsigned int snd_motu_clock_rates[SND_MOTU_CLOCK_RATE_COUNT];

enum snd_motu_clock_source {
	SND_MOTU_CLOCK_SOURCE_INTERNAL,
	SND_MOTU_CLOCK_SOURCE_ADAT_ON_DSUB,
	SND_MOTU_CLOCK_SOURCE_ADAT_ON_OPT,
	SND_MOTU_CLOCK_SOURCE_ADAT_ON_OPT_A,
	SND_MOTU_CLOCK_SOURCE_ADAT_ON_OPT_B,
	SND_MOTU_CLOCK_SOURCE_SPDIF_ON_OPT,
	SND_MOTU_CLOCK_SOURCE_SPDIF_ON_OPT_A,
	SND_MOTU_CLOCK_SOURCE_SPDIF_ON_OPT_B,
	SND_MOTU_CLOCK_SOURCE_SPDIF_ON_COAX,
	SND_MOTU_CLOCK_SOURCE_AESEBU_ON_XLR,
	SND_MOTU_CLOCK_SOURCE_WORD_ON_BNC,
<<<<<<< HEAD
	SND_MOTU_CLOCK_SOURCE_UNKNOWN,
};

struct snd_motu_protocol {
	int (*get_clock_rate)(struct snd_motu *motu, unsigned int *rate);
	int (*set_clock_rate)(struct snd_motu *motu, unsigned int rate);
	int (*get_clock_source)(struct snd_motu *motu,
				enum snd_motu_clock_source *source);
	int (*switch_fetching_mode)(struct snd_motu *motu, bool enable);
	int (*cache_packet_formats)(struct snd_motu *motu);
=======
	SND_MOTU_CLOCK_SOURCE_SPH,
	SND_MOTU_CLOCK_SOURCE_UNKNOWN,
};

enum snd_motu_protocol_version {
	SND_MOTU_PROTOCOL_V1,
	SND_MOTU_PROTOCOL_V2,
	SND_MOTU_PROTOCOL_V3,
>>>>>>> upstream/android-13
};

struct snd_motu_spec {
	const char *const name;
<<<<<<< HEAD
	enum snd_motu_spec_flags flags;

	unsigned char analog_in_ports;
	unsigned char analog_out_ports;

	const struct snd_motu_protocol *const protocol;
};

extern const struct snd_motu_protocol snd_motu_protocol_v2;
extern const struct snd_motu_protocol snd_motu_protocol_v3;

extern const struct snd_motu_spec snd_motu_spec_traveler;

int amdtp_motu_init(struct amdtp_stream *s, struct fw_unit *unit,
		    enum amdtp_stream_direction dir,
		    const struct snd_motu_protocol *const protocol);
=======
	enum snd_motu_protocol_version protocol_version;
	// The combination of snd_motu_spec_flags enumeration-constants.
	unsigned int flags;

	unsigned char tx_fixed_pcm_chunks[3];
	unsigned char rx_fixed_pcm_chunks[3];
};

extern const struct snd_motu_spec snd_motu_spec_828;
extern const struct snd_motu_spec snd_motu_spec_896;

extern const struct snd_motu_spec snd_motu_spec_828mk2;
extern const struct snd_motu_spec snd_motu_spec_896hd;
extern const struct snd_motu_spec snd_motu_spec_traveler;
extern const struct snd_motu_spec snd_motu_spec_ultralite;
extern const struct snd_motu_spec snd_motu_spec_8pre;

extern const struct snd_motu_spec snd_motu_spec_828mk3_fw;
extern const struct snd_motu_spec snd_motu_spec_828mk3_hybrid;
extern const struct snd_motu_spec snd_motu_spec_ultralite_mk3;
extern const struct snd_motu_spec snd_motu_spec_audio_express;
extern const struct snd_motu_spec snd_motu_spec_4pre;

int amdtp_motu_init(struct amdtp_stream *s, struct fw_unit *unit,
		    enum amdtp_stream_direction dir,
		    const struct snd_motu_spec *spec,
		    struct amdtp_motu_cache *cache);
>>>>>>> upstream/android-13
int amdtp_motu_set_parameters(struct amdtp_stream *s, unsigned int rate,
			      unsigned int midi_ports,
			      struct snd_motu_packet_format *formats);
int amdtp_motu_add_pcm_hw_constraints(struct amdtp_stream *s,
				      struct snd_pcm_runtime *runtime);
void amdtp_motu_midi_trigger(struct amdtp_stream *s, unsigned int port,
			     struct snd_rawmidi_substream *midi);

int snd_motu_transaction_read(struct snd_motu *motu, u32 offset, __be32 *reg,
			      size_t size);
int snd_motu_transaction_write(struct snd_motu *motu, u32 offset, __be32 *reg,
			       size_t size);
int snd_motu_transaction_register(struct snd_motu *motu);
int snd_motu_transaction_reregister(struct snd_motu *motu);
void snd_motu_transaction_unregister(struct snd_motu *motu);

int snd_motu_stream_init_duplex(struct snd_motu *motu);
void snd_motu_stream_destroy_duplex(struct snd_motu *motu);
int snd_motu_stream_cache_packet_formats(struct snd_motu *motu);
<<<<<<< HEAD
int snd_motu_stream_start_duplex(struct snd_motu *motu, unsigned int rate);
=======
int snd_motu_stream_reserve_duplex(struct snd_motu *motu, unsigned int rate,
				   unsigned int frames_per_period,
				   unsigned int frames_per_buffer);
int snd_motu_stream_start_duplex(struct snd_motu *motu);
>>>>>>> upstream/android-13
void snd_motu_stream_stop_duplex(struct snd_motu *motu);
int snd_motu_stream_lock_try(struct snd_motu *motu);
void snd_motu_stream_lock_release(struct snd_motu *motu);

void snd_motu_proc_init(struct snd_motu *motu);

int snd_motu_create_pcm_devices(struct snd_motu *motu);

int snd_motu_create_midi_devices(struct snd_motu *motu);

int snd_motu_create_hwdep_device(struct snd_motu *motu);
<<<<<<< HEAD
=======

int snd_motu_protocol_v1_get_clock_rate(struct snd_motu *motu,
					unsigned int *rate);
int snd_motu_protocol_v1_set_clock_rate(struct snd_motu *motu,
					unsigned int rate);
int snd_motu_protocol_v1_get_clock_source(struct snd_motu *motu,
					  enum snd_motu_clock_source *src);
int snd_motu_protocol_v1_switch_fetching_mode(struct snd_motu *motu,
					      bool enable);
int snd_motu_protocol_v1_cache_packet_formats(struct snd_motu *motu);

int snd_motu_protocol_v2_get_clock_rate(struct snd_motu *motu,
					unsigned int *rate);
int snd_motu_protocol_v2_set_clock_rate(struct snd_motu *motu,
					unsigned int rate);
int snd_motu_protocol_v2_get_clock_source(struct snd_motu *motu,
					  enum snd_motu_clock_source *src);
int snd_motu_protocol_v2_switch_fetching_mode(struct snd_motu *motu,
					      bool enable);
int snd_motu_protocol_v2_cache_packet_formats(struct snd_motu *motu);

int snd_motu_protocol_v3_get_clock_rate(struct snd_motu *motu,
					unsigned int *rate);
int snd_motu_protocol_v3_set_clock_rate(struct snd_motu *motu,
					unsigned int rate);
int snd_motu_protocol_v3_get_clock_source(struct snd_motu *motu,
					  enum snd_motu_clock_source *src);
int snd_motu_protocol_v3_switch_fetching_mode(struct snd_motu *motu,
					      bool enable);
int snd_motu_protocol_v3_cache_packet_formats(struct snd_motu *motu);

static inline int snd_motu_protocol_get_clock_rate(struct snd_motu *motu,
						   unsigned int *rate)
{
	if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V2)
		return snd_motu_protocol_v2_get_clock_rate(motu, rate);
	else if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V3)
		return snd_motu_protocol_v3_get_clock_rate(motu, rate);
	else if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V1)
		return snd_motu_protocol_v1_get_clock_rate(motu, rate);
	else
		return -ENXIO;
}

static inline int snd_motu_protocol_set_clock_rate(struct snd_motu *motu,
						   unsigned int rate)
{
	if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V2)
		return snd_motu_protocol_v2_set_clock_rate(motu, rate);
	else if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V3)
		return snd_motu_protocol_v3_set_clock_rate(motu, rate);
	else if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V1)
		return snd_motu_protocol_v1_set_clock_rate(motu, rate);
	else
		return -ENXIO;
}

static inline int snd_motu_protocol_get_clock_source(struct snd_motu *motu,
					enum snd_motu_clock_source *source)
{
	if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V2)
		return snd_motu_protocol_v2_get_clock_source(motu, source);
	else if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V3)
		return snd_motu_protocol_v3_get_clock_source(motu, source);
	else if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V1)
		return snd_motu_protocol_v1_get_clock_source(motu, source);
	else
		return -ENXIO;
}

static inline int snd_motu_protocol_switch_fetching_mode(struct snd_motu *motu,
							 bool enable)
{
	if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V2)
		return snd_motu_protocol_v2_switch_fetching_mode(motu, enable);
	else if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V3)
		return snd_motu_protocol_v3_switch_fetching_mode(motu, enable);
	else if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V1)
		return snd_motu_protocol_v1_switch_fetching_mode(motu, enable);
	else
		return -ENXIO;
}

static inline int snd_motu_protocol_cache_packet_formats(struct snd_motu *motu)
{
	if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V2)
		return snd_motu_protocol_v2_cache_packet_formats(motu);
	else if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V3)
		return snd_motu_protocol_v3_cache_packet_formats(motu);
	else if (motu->spec->protocol_version == SND_MOTU_PROTOCOL_V1)
		return snd_motu_protocol_v1_cache_packet_formats(motu);
	else
		return -ENXIO;
}

>>>>>>> upstream/android-13
#endif
