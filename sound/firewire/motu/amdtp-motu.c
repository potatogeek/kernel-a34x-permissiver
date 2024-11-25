<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * amdtp-motu.c - a part of driver for MOTU FireWire series
 *
 * Copyright (c) 2015-2017 Takashi Sakamoto <o-takashi@sakamocchi.jp>
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#include <linux/slab.h>
#include <sound/pcm.h>
#include "motu.h"

#define CREATE_TRACE_POINTS
#include "amdtp-motu-trace.h"

#define CIP_FMT_MOTU		0x02
#define CIP_FMT_MOTU_TX_V3	0x22
#define MOTU_FDF_AM824		0x22

<<<<<<< HEAD
=======
#define TICKS_PER_CYCLE		3072
#define CYCLES_PER_SECOND	8000
#define TICKS_PER_SECOND	(TICKS_PER_CYCLE * CYCLES_PER_SECOND)

#define CIP_SPH_CYCLE_SHIFT	12
#define CIP_SPH_CYCLE_MASK	0x01fff000
#define CIP_SPH_OFFSET_MASK	0x00000fff

>>>>>>> upstream/android-13
/*
 * Nominally 3125 bytes/second, but the MIDI port's clock might be
 * 1% too slow, and the bus clock 100 ppm too fast.
 */
#define MIDI_BYTES_PER_SECOND	3093

struct amdtp_motu {
<<<<<<< HEAD
	/* For timestamp processing.  */
	unsigned int quotient_ticks_per_event;
	unsigned int remainder_ticks_per_event;
	unsigned int next_ticks;
	unsigned int next_accumulated;
	unsigned int next_cycles;
	unsigned int next_seconds;

=======
>>>>>>> upstream/android-13
	unsigned int pcm_chunks;
	unsigned int pcm_byte_offset;

	struct snd_rawmidi_substream *midi;
	unsigned int midi_ports;
	unsigned int midi_flag_offset;
	unsigned int midi_byte_offset;

	int midi_db_count;
	unsigned int midi_db_interval;
<<<<<<< HEAD
=======

	struct amdtp_motu_cache *cache;
>>>>>>> upstream/android-13
};

int amdtp_motu_set_parameters(struct amdtp_stream *s, unsigned int rate,
			      unsigned int midi_ports,
			      struct snd_motu_packet_format *formats)
{
<<<<<<< HEAD
	static const struct {
		unsigned int quotient_ticks_per_event;
		unsigned int remainder_ticks_per_event;
	} params[] = {
		[CIP_SFC_44100]  = { 557, 123 },
		[CIP_SFC_48000]  = { 512,   0 },
		[CIP_SFC_88200]  = { 278, 282 },
		[CIP_SFC_96000]  = { 256,   0 },
		[CIP_SFC_176400] = { 139, 141 },
		[CIP_SFC_192000] = { 128,   0 },
	};
	struct amdtp_motu *p = s->protocol;
	unsigned int pcm_chunks, data_chunks, data_block_quadlets;
	unsigned int delay;
=======
	struct amdtp_motu *p = s->protocol;
	unsigned int pcm_chunks, data_chunks, data_block_quadlets;
>>>>>>> upstream/android-13
	unsigned int mode;
	int i, err;

	if (amdtp_stream_running(s))
		return -EBUSY;

	for (i = 0; i < ARRAY_SIZE(snd_motu_clock_rates); ++i) {
		if (snd_motu_clock_rates[i] == rate) {
			mode = i >> 1;
			break;
		}
	}
	if (i == ARRAY_SIZE(snd_motu_clock_rates))
		return -EINVAL;

<<<<<<< HEAD
	pcm_chunks = formats->fixed_part_pcm_chunks[mode] +
		     formats->differed_part_pcm_chunks[mode];
	data_chunks = formats->msg_chunks + pcm_chunks;

	/*
	 * Each data block includes SPH in its head. Data chunks follow with
	 * 3 byte alignment. Padding follows with zero to conform to quadlet
	 * alignment.
	 */
=======
	// Each data block includes SPH in its head. Data chunks follow with
	// 3 byte alignment. Padding follows with zero to conform to quadlet
	// alignment.
	pcm_chunks = formats->pcm_chunks[mode];
	data_chunks = formats->msg_chunks + pcm_chunks;
>>>>>>> upstream/android-13
	data_block_quadlets = 1 + DIV_ROUND_UP(data_chunks * 3, 4);

	err = amdtp_stream_set_parameters(s, rate, data_block_quadlets);
	if (err < 0)
		return err;

	p->pcm_chunks = pcm_chunks;
	p->pcm_byte_offset = formats->pcm_byte_offset;

	p->midi_ports = midi_ports;
	p->midi_flag_offset = formats->midi_flag_offset;
	p->midi_byte_offset = formats->midi_byte_offset;

	p->midi_db_count = 0;
	p->midi_db_interval = rate / MIDI_BYTES_PER_SECOND;

<<<<<<< HEAD
	/* IEEE 1394 bus requires. */
	delay = 0x2e00;

	/* For no-data or empty packets to adjust PCM sampling frequency. */
	delay += 8000 * 3072 * s->syt_interval / rate;

	p->next_seconds = 0;
	p->next_cycles = delay / 3072;
	p->quotient_ticks_per_event = params[s->sfc].quotient_ticks_per_event;
	p->remainder_ticks_per_event = params[s->sfc].remainder_ticks_per_event;
	p->next_ticks = delay % 3072;
	p->next_accumulated = 0;

	return 0;
}

static void read_pcm_s32(struct amdtp_stream *s,
			 struct snd_pcm_runtime *runtime,
			 __be32 *buffer, unsigned int data_blocks)
{
	struct amdtp_motu *p = s->protocol;
	unsigned int channels, remaining_frames, i, c;
	u8 *byte;
	u32 *dst;

	channels = p->pcm_chunks;
	dst = (void *)runtime->dma_area +
			frames_to_bytes(runtime, s->pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;
=======
	return 0;
}

static void read_pcm_s32(struct amdtp_stream *s, struct snd_pcm_substream *pcm,
			 __be32 *buffer, unsigned int data_blocks,
			 unsigned int pcm_frames)
{
	struct amdtp_motu *p = s->protocol;
	unsigned int channels = p->pcm_chunks;
	struct snd_pcm_runtime *runtime = pcm->runtime;
	unsigned int pcm_buffer_pointer;
	int remaining_frames;
	u8 *byte;
	u32 *dst;
	int i, c;

	pcm_buffer_pointer = s->pcm_buffer_pointer + pcm_frames;
	pcm_buffer_pointer %= runtime->buffer_size;

	dst = (void *)runtime->dma_area +
				frames_to_bytes(runtime, pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - pcm_buffer_pointer;
>>>>>>> upstream/android-13

	for (i = 0; i < data_blocks; ++i) {
		byte = (u8 *)buffer + p->pcm_byte_offset;

		for (c = 0; c < channels; ++c) {
			*dst = (byte[0] << 24) |
			       (byte[1] << 16) |
			       (byte[2] << 8);
			byte += 3;
			dst++;
		}
		buffer += s->data_block_quadlets;
		if (--remaining_frames == 0)
			dst = (void *)runtime->dma_area;
	}
}

<<<<<<< HEAD
static void write_pcm_s32(struct amdtp_stream *s,
			  struct snd_pcm_runtime *runtime,
			  __be32 *buffer, unsigned int data_blocks)
{
	struct amdtp_motu *p = s->protocol;
	unsigned int channels, remaining_frames, i, c;
	u8 *byte;
	const u32 *src;

	channels = p->pcm_chunks;
	src = (void *)runtime->dma_area +
			frames_to_bytes(runtime, s->pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;
=======
static void write_pcm_s32(struct amdtp_stream *s, struct snd_pcm_substream *pcm,
			  __be32 *buffer, unsigned int data_blocks,
			  unsigned int pcm_frames)
{
	struct amdtp_motu *p = s->protocol;
	unsigned int channels = p->pcm_chunks;
	struct snd_pcm_runtime *runtime = pcm->runtime;
	unsigned int pcm_buffer_pointer;
	int remaining_frames;
	u8 *byte;
	const u32 *src;
	int i, c;

	pcm_buffer_pointer = s->pcm_buffer_pointer + pcm_frames;
	pcm_buffer_pointer %= runtime->buffer_size;

	src = (void *)runtime->dma_area +
				frames_to_bytes(runtime, pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - pcm_buffer_pointer;
>>>>>>> upstream/android-13

	for (i = 0; i < data_blocks; ++i) {
		byte = (u8 *)buffer + p->pcm_byte_offset;

		for (c = 0; c < channels; ++c) {
			byte[0] = (*src >> 24) & 0xff;
			byte[1] = (*src >> 16) & 0xff;
			byte[2] = (*src >>  8) & 0xff;
			byte += 3;
			src++;
		}

		buffer += s->data_block_quadlets;
		if (--remaining_frames == 0)
			src = (void *)runtime->dma_area;
	}
}

static void write_pcm_silence(struct amdtp_stream *s, __be32 *buffer,
			      unsigned int data_blocks)
{
	struct amdtp_motu *p = s->protocol;
	unsigned int channels, i, c;
	u8 *byte;

	channels = p->pcm_chunks;

	for (i = 0; i < data_blocks; ++i) {
		byte = (u8 *)buffer + p->pcm_byte_offset;

		for (c = 0; c < channels; ++c) {
			byte[0] = 0;
			byte[1] = 0;
			byte[2] = 0;
			byte += 3;
		}

		buffer += s->data_block_quadlets;
	}
}

int amdtp_motu_add_pcm_hw_constraints(struct amdtp_stream *s,
				      struct snd_pcm_runtime *runtime)
{
	int err;

	/* TODO: how to set an constraint for exactly 24bit PCM sample? */
	err = snd_pcm_hw_constraint_msbits(runtime, 0, 32, 24);
	if (err < 0)
		return err;

	return amdtp_stream_add_pcm_hw_constraints(s, runtime);
}

void amdtp_motu_midi_trigger(struct amdtp_stream *s, unsigned int port,
			     struct snd_rawmidi_substream *midi)
{
	struct amdtp_motu *p = s->protocol;

	if (port < p->midi_ports)
		WRITE_ONCE(p->midi, midi);
}

static void write_midi_messages(struct amdtp_stream *s, __be32 *buffer,
				unsigned int data_blocks)
{
	struct amdtp_motu *p = s->protocol;
	struct snd_rawmidi_substream *midi = READ_ONCE(p->midi);
	u8 *b;
	int i;

	for (i = 0; i < data_blocks; i++) {
		b = (u8 *)buffer;

		if (midi && p->midi_db_count == 0 &&
		    snd_rawmidi_transmit(midi, b + p->midi_byte_offset, 1) == 1) {
			b[p->midi_flag_offset] = 0x01;
		} else {
			b[p->midi_byte_offset] = 0x00;
			b[p->midi_flag_offset] = 0x00;
		}

		buffer += s->data_block_quadlets;

		if (--p->midi_db_count < 0)
			p->midi_db_count = p->midi_db_interval;
	}
}

static void read_midi_messages(struct amdtp_stream *s, __be32 *buffer,
			       unsigned int data_blocks)
{
	struct amdtp_motu *p = s->protocol;
	struct snd_rawmidi_substream *midi;
	u8 *b;
	int i;

	for (i = 0; i < data_blocks; i++) {
		b = (u8 *)buffer;
		midi = READ_ONCE(p->midi);

		if (midi && (b[p->midi_flag_offset] & 0x01))
			snd_rawmidi_receive(midi, b + p->midi_byte_offset, 1);

		buffer += s->data_block_quadlets;
	}
}

/* For tracepoints. */
static void __maybe_unused copy_sph(u32 *frames, __be32 *buffer,
				    unsigned int data_blocks,
				    unsigned int data_block_quadlets)
{
	unsigned int i;

	for (i = 0; i < data_blocks; ++i) {
		*frames = be32_to_cpu(*buffer);
		buffer += data_block_quadlets;
		frames++;
	}
}

/* For tracepoints. */
static void __maybe_unused copy_message(u64 *frames, __be32 *buffer,
					unsigned int data_blocks,
					unsigned int data_block_quadlets)
{
	unsigned int i;

	/* This is just for v2/v3 protocol. */
	for (i = 0; i < data_blocks; ++i) {
<<<<<<< HEAD
		*frames = (be32_to_cpu(buffer[1]) << 16) |
			  (be32_to_cpu(buffer[2]) >> 16);
		buffer += data_block_quadlets;
		frames++;
	}
}

static unsigned int process_tx_data_blocks(struct amdtp_stream *s,
				__be32 *buffer, unsigned int data_blocks,
				unsigned int *syt)
{
	struct amdtp_motu *p = s->protocol;
	struct snd_pcm_substream *pcm;

	trace_in_data_block_sph(s, data_blocks, buffer);
	trace_in_data_block_message(s, data_blocks, buffer);

	if (p->midi_ports)
		read_midi_messages(s, buffer, data_blocks);

	pcm = READ_ONCE(s->pcm);
	if (data_blocks > 0 && pcm)
		read_pcm_s32(s, pcm->runtime, buffer, data_blocks);

	return data_blocks;
}

static inline void compute_next_elapse_from_start(struct amdtp_motu *p)
{
	p->next_accumulated += p->remainder_ticks_per_event;
	if (p->next_accumulated >= 441) {
		p->next_accumulated -= 441;
		p->next_ticks++;
	}

	p->next_ticks += p->quotient_ticks_per_event;
	if (p->next_ticks >= 3072) {
		p->next_ticks -= 3072;
		p->next_cycles++;
	}

	if (p->next_cycles >= 8000) {
		p->next_cycles -= 8000;
		p->next_seconds++;
	}

	if (p->next_seconds >= 128)
		p->next_seconds -= 128;
}

static void write_sph(struct amdtp_stream *s, __be32 *buffer,
		      unsigned int data_blocks)
{
	struct amdtp_motu *p = s->protocol;
	unsigned int next_cycles;
	unsigned int i;
	u32 sph;

	for (i = 0; i < data_blocks; i++) {
		next_cycles = (s->start_cycle + p->next_cycles) % 8000;
		sph = ((next_cycles << 12) | p->next_ticks) & 0x01ffffff;
		*buffer = cpu_to_be32(sph);

		compute_next_elapse_from_start(p);

		buffer += s->data_block_quadlets;
	}
}

static unsigned int process_rx_data_blocks(struct amdtp_stream *s,
				__be32 *buffer, unsigned int data_blocks,
				unsigned int *syt)
{
	struct amdtp_motu *p = (struct amdtp_motu *)s->protocol;
	struct snd_pcm_substream *pcm;

	/* Not used. */
	*syt = 0xffff;

	/* TODO: how to interact control messages between userspace? */

	if (p->midi_ports)
		write_midi_messages(s, buffer, data_blocks);

	pcm = READ_ONCE(s->pcm);
	if (pcm)
		write_pcm_s32(s, pcm->runtime, buffer, data_blocks);
	else
		write_pcm_silence(s, buffer, data_blocks);

	write_sph(s, buffer, data_blocks);

	trace_out_data_block_sph(s, data_blocks, buffer);
	trace_out_data_block_message(s, data_blocks, buffer);

	return data_blocks;
=======
		*frames = be32_to_cpu(buffer[1]);
		*frames <<= 16;
		*frames |= be32_to_cpu(buffer[2]) >> 16;
		++frames;
		buffer += data_block_quadlets;
	}
}

static void probe_tracepoints_events(struct amdtp_stream *s,
				     const struct pkt_desc *descs,
				     unsigned int packets)
{
	int i;

	for (i = 0; i < packets; ++i) {
		const struct pkt_desc *desc = descs + i;
		__be32 *buf = desc->ctx_payload;
		unsigned int data_blocks = desc->data_blocks;

		trace_data_block_sph(s, data_blocks, buf);
		trace_data_block_message(s, data_blocks, buf);
	}
}

static void cache_event_offsets(struct amdtp_motu_cache *cache, const __be32 *buf,
				unsigned int data_blocks, unsigned int data_block_quadlets)
{
	unsigned int *event_offsets = cache->event_offsets;
	const unsigned int cache_size = cache->size;
	unsigned int cache_tail = cache->tail;
	unsigned int base_tick = cache->tx_cycle_count * TICKS_PER_CYCLE;
	int i;

	for (i = 0; i < data_blocks; ++i) {
		u32 sph = be32_to_cpu(*buf);
		unsigned int tick;

		tick = ((sph & CIP_SPH_CYCLE_MASK) >> CIP_SPH_CYCLE_SHIFT) * TICKS_PER_CYCLE +
		       (sph & CIP_SPH_OFFSET_MASK);

		if (tick < base_tick)
			tick += TICKS_PER_SECOND;
		event_offsets[cache_tail] = tick - base_tick;

		cache_tail = (cache_tail + 1) % cache_size;
		buf += data_block_quadlets;
	}

	cache->tail = cache_tail;
	cache->tx_cycle_count = (cache->tx_cycle_count + 1) % CYCLES_PER_SECOND;
}

static unsigned int process_ir_ctx_payloads(struct amdtp_stream *s,
					    const struct pkt_desc *descs,
					    unsigned int packets,
					    struct snd_pcm_substream *pcm)
{
	struct amdtp_motu *p = s->protocol;
	unsigned int pcm_frames = 0;
	int i;

	if (p->cache->tx_cycle_count == UINT_MAX)
		p->cache->tx_cycle_count = (s->domain->processing_cycle.tx_start % CYCLES_PER_SECOND);

	// For data block processing.
	for (i = 0; i < packets; ++i) {
		const struct pkt_desc *desc = descs + i;
		__be32 *buf = desc->ctx_payload;
		unsigned int data_blocks = desc->data_blocks;

		cache_event_offsets(p->cache, buf, data_blocks, s->data_block_quadlets);

		if (pcm) {
			read_pcm_s32(s, pcm, buf, data_blocks, pcm_frames);
			pcm_frames += data_blocks;
		}

		if (p->midi_ports)
			read_midi_messages(s, buf, data_blocks);
	}

	// For tracepoints.
	if (trace_data_block_sph_enabled() ||
	    trace_data_block_message_enabled())
		probe_tracepoints_events(s, descs, packets);

	return pcm_frames;
}

static void write_sph(struct amdtp_motu_cache *cache, __be32 *buffer, unsigned int data_blocks,
		      unsigned int data_block_quadlets)
{
	unsigned int *event_offsets = cache->event_offsets;
	const unsigned int cache_size = cache->size;
	unsigned int cache_head = cache->head;
	unsigned int base_tick = cache->rx_cycle_count * TICKS_PER_CYCLE;
	int i;

	for (i = 0; i < data_blocks; i++) {
		unsigned int tick = (base_tick + event_offsets[cache_head]) % TICKS_PER_SECOND;
		u32 sph = ((tick / TICKS_PER_CYCLE) << CIP_SPH_CYCLE_SHIFT) | (tick % TICKS_PER_CYCLE);
		*buffer = cpu_to_be32(sph);

		cache_head = (cache_head + 1) % cache_size;
		buffer += data_block_quadlets;
	}

	cache->head = cache_head;
	cache->rx_cycle_count = (cache->rx_cycle_count + 1) % CYCLES_PER_SECOND;
}

static unsigned int process_it_ctx_payloads(struct amdtp_stream *s,
					    const struct pkt_desc *descs,
					    unsigned int packets,
					    struct snd_pcm_substream *pcm)
{
	struct amdtp_motu *p = s->protocol;
	unsigned int pcm_frames = 0;
	int i;

	if (p->cache->rx_cycle_count == UINT_MAX)
		p->cache->rx_cycle_count = (s->domain->processing_cycle.rx_start % CYCLES_PER_SECOND);

	// For data block processing.
	for (i = 0; i < packets; ++i) {
		const struct pkt_desc *desc = descs + i;
		__be32 *buf = desc->ctx_payload;
		unsigned int data_blocks = desc->data_blocks;

		if (pcm) {
			write_pcm_s32(s, pcm, buf, data_blocks, pcm_frames);
			pcm_frames += data_blocks;
		} else {
			write_pcm_silence(s, buf, data_blocks);
		}

		if (p->midi_ports)
			write_midi_messages(s, buf, data_blocks);

		// TODO: how to interact control messages between userspace?

		write_sph(p->cache, buf, data_blocks, s->data_block_quadlets);
	}

	// For tracepoints.
	if (trace_data_block_sph_enabled() ||
	    trace_data_block_message_enabled())
		probe_tracepoints_events(s, descs, packets);

	return pcm_frames;
>>>>>>> upstream/android-13
}

int amdtp_motu_init(struct amdtp_stream *s, struct fw_unit *unit,
		    enum amdtp_stream_direction dir,
<<<<<<< HEAD
		    const struct snd_motu_protocol *const protocol)
{
	amdtp_stream_process_data_blocks_t process_data_blocks;
	int fmt = CIP_FMT_MOTU;
	int flags = CIP_BLOCKING;
	int err;

	if (dir == AMDTP_IN_STREAM) {
		process_data_blocks = process_tx_data_blocks;
=======
		    const struct snd_motu_spec *spec, struct amdtp_motu_cache *cache)
{
	amdtp_stream_process_ctx_payloads_t process_ctx_payloads;
	int fmt = CIP_FMT_MOTU;
	unsigned int flags = CIP_BLOCKING | CIP_UNAWARE_SYT;
	struct amdtp_motu *p;
	int err;

	if (dir == AMDTP_IN_STREAM) {
		process_ctx_payloads = process_ir_ctx_payloads;
>>>>>>> upstream/android-13

		/*
		 * Units of version 3 transmits packets with invalid CIP header
		 * against IEC 61883-1.
		 */
<<<<<<< HEAD
		if (protocol == &snd_motu_protocol_v3) {
=======
		if (spec->protocol_version == SND_MOTU_PROTOCOL_V3) {
>>>>>>> upstream/android-13
			flags |= CIP_WRONG_DBS |
				 CIP_SKIP_DBC_ZERO_CHECK |
				 CIP_HEADER_WITHOUT_EOH;
			fmt = CIP_FMT_MOTU_TX_V3;
		}
<<<<<<< HEAD
	} else {
		process_data_blocks = process_rx_data_blocks;
		flags |= CIP_DBC_IS_END_EVENT;
	}

	err = amdtp_stream_init(s, unit, dir, flags, fmt, process_data_blocks,
=======

		if (spec == &snd_motu_spec_8pre ||
		    spec == &snd_motu_spec_ultralite) {
			// 8pre has some quirks.
			flags |= CIP_WRONG_DBS |
				 CIP_SKIP_DBC_ZERO_CHECK;
		}
	} else {
		process_ctx_payloads = process_it_ctx_payloads;
		flags |= CIP_DBC_IS_END_EVENT;
	}

	err = amdtp_stream_init(s, unit, dir, flags, fmt, process_ctx_payloads,
>>>>>>> upstream/android-13
				sizeof(struct amdtp_motu));
	if (err < 0)
		return err;

	s->sph = 1;
<<<<<<< HEAD
	s->fdf = MOTU_FDF_AM824;
=======

	if (dir == AMDTP_OUT_STREAM) {
		// Use fixed value for FDF field.
		s->ctx_data.rx.fdf = MOTU_FDF_AM824;
	}

	p = s->protocol;
	p->cache = cache;
>>>>>>> upstream/android-13

	return 0;
}
