<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * amdtp-tascam.c - a part of driver for TASCAM FireWire series
 *
 * Copyright (c) 2015 Takashi Sakamoto
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#include <sound/pcm.h>
#include "tascam.h"

#define AMDTP_FMT_TSCM_TX	0x1e
#define AMDTP_FMT_TSCM_RX	0x3e

struct amdtp_tscm {
	unsigned int pcm_channels;
};

int amdtp_tscm_set_parameters(struct amdtp_stream *s, unsigned int rate)
{
	struct amdtp_tscm *p = s->protocol;
	unsigned int data_channels;

	if (amdtp_stream_running(s))
		return -EBUSY;

	data_channels = p->pcm_channels;

	/* Packets in in-stream have extra 2 data channels. */
	if (s->direction == AMDTP_IN_STREAM)
		data_channels += 2;

	return amdtp_stream_set_parameters(s, rate, data_channels);
}

<<<<<<< HEAD
static void write_pcm_s32(struct amdtp_stream *s,
			  struct snd_pcm_substream *pcm,
			  __be32 *buffer, unsigned int frames)
{
	struct amdtp_tscm *p = s->protocol;
	struct snd_pcm_runtime *runtime = pcm->runtime;
	unsigned int channels, remaining_frames, i, c;
	const u32 *src;

	channels = p->pcm_channels;
	src = (void *)runtime->dma_area +
			frames_to_bytes(runtime, s->pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;
=======
static void write_pcm_s32(struct amdtp_stream *s, struct snd_pcm_substream *pcm,
			  __be32 *buffer, unsigned int frames,
			  unsigned int pcm_frames)
{
	struct amdtp_tscm *p = s->protocol;
	unsigned int channels = p->pcm_channels;
	struct snd_pcm_runtime *runtime = pcm->runtime;
	unsigned int pcm_buffer_pointer;
	int remaining_frames;
	const u32 *src;
	int i, c;

	pcm_buffer_pointer = s->pcm_buffer_pointer + pcm_frames;
	pcm_buffer_pointer %= runtime->buffer_size;

	src = (void *)runtime->dma_area +
				frames_to_bytes(runtime, pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - pcm_buffer_pointer;
>>>>>>> upstream/android-13

	for (i = 0; i < frames; ++i) {
		for (c = 0; c < channels; ++c) {
			buffer[c] = cpu_to_be32(*src);
			src++;
		}
		buffer += s->data_block_quadlets;
		if (--remaining_frames == 0)
			src = (void *)runtime->dma_area;
	}
}

<<<<<<< HEAD
static void read_pcm_s32(struct amdtp_stream *s,
			 struct snd_pcm_substream *pcm,
			 __be32 *buffer, unsigned int frames)
{
	struct amdtp_tscm *p = s->protocol;
	struct snd_pcm_runtime *runtime = pcm->runtime;
	unsigned int channels, remaining_frames, i, c;
	u32 *dst;

	channels = p->pcm_channels;
	dst  = (void *)runtime->dma_area +
			frames_to_bytes(runtime, s->pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;
=======
static void read_pcm_s32(struct amdtp_stream *s, struct snd_pcm_substream *pcm,
			 __be32 *buffer, unsigned int frames,
			 unsigned int pcm_frames)
{
	struct amdtp_tscm *p = s->protocol;
	unsigned int channels = p->pcm_channels;
	struct snd_pcm_runtime *runtime = pcm->runtime;
	unsigned int pcm_buffer_pointer;
	int remaining_frames;
	u32 *dst;
	int i, c;

	pcm_buffer_pointer = s->pcm_buffer_pointer + pcm_frames;
	pcm_buffer_pointer %= runtime->buffer_size;

	dst  = (void *)runtime->dma_area +
				frames_to_bytes(runtime, pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - pcm_buffer_pointer;
>>>>>>> upstream/android-13

	/* The first data channel is for event counter. */
	buffer += 1;

	for (i = 0; i < frames; ++i) {
		for (c = 0; c < channels; ++c) {
			*dst = be32_to_cpu(buffer[c]);
			dst++;
		}
		buffer += s->data_block_quadlets;
		if (--remaining_frames == 0)
			dst = (void *)runtime->dma_area;
	}
}

static void write_pcm_silence(struct amdtp_stream *s, __be32 *buffer,
			      unsigned int data_blocks)
{
	struct amdtp_tscm *p = s->protocol;
	unsigned int channels, i, c;

	channels = p->pcm_channels;

	for (i = 0; i < data_blocks; ++i) {
		for (c = 0; c < channels; ++c)
			buffer[c] = 0x00000000;
		buffer += s->data_block_quadlets;
	}
}

int amdtp_tscm_add_pcm_hw_constraints(struct amdtp_stream *s,
				      struct snd_pcm_runtime *runtime)
{
	int err;

	/*
	 * Our implementation allows this protocol to deliver 24 bit sample in
	 * 32bit data channel.
	 */
	err = snd_pcm_hw_constraint_msbits(runtime, 0, 32, 24);
	if (err < 0)
		return err;

	return amdtp_stream_add_pcm_hw_constraints(s, runtime);
}

<<<<<<< HEAD
static unsigned int process_tx_data_blocks(struct amdtp_stream *s,
					   __be32 *buffer,
					   unsigned int data_blocks,
					   unsigned int *syt)
{
	struct snd_pcm_substream *pcm;

	pcm = READ_ONCE(s->pcm);
	if (data_blocks > 0 && pcm)
		read_pcm_s32(s, pcm, buffer, data_blocks);

	/* A place holder for control messages. */

	return data_blocks;
}

static unsigned int process_rx_data_blocks(struct amdtp_stream *s,
					   __be32 *buffer,
					   unsigned int data_blocks,
					   unsigned int *syt)
{
	struct snd_pcm_substream *pcm;

	/* This field is not used. */
	*syt = 0x0000;

	pcm = READ_ONCE(s->pcm);
	if (pcm)
		write_pcm_s32(s, pcm, buffer, data_blocks);
	else
		write_pcm_silence(s, buffer, data_blocks);

	return data_blocks;
=======
static void read_status_messages(struct amdtp_stream *s,
				 __be32 *buffer, unsigned int data_blocks)
{
	struct snd_tscm *tscm = container_of(s, struct snd_tscm, tx_stream);
	bool used = READ_ONCE(tscm->hwdep->used);
	int i;

	for (i = 0; i < data_blocks; i++) {
		unsigned int index;
		__be32 before;
		__be32 after;

		index = be32_to_cpu(buffer[0]) % SNDRV_FIREWIRE_TASCAM_STATE_COUNT;
		before = tscm->state[index];
		after = buffer[s->data_block_quadlets - 1];

		if (used && index > 4 && index < 16) {
			__be32 mask;

			if (index == 5)
				mask = cpu_to_be32(~0x0000ffff);
			else if (index == 6)
				mask = cpu_to_be32(~0x0000ffff);
			else if (index == 8)
				mask = cpu_to_be32(~0x000f0f00);
			else
				mask = cpu_to_be32(~0x00000000);

			if ((before ^ after) & mask) {
				struct snd_firewire_tascam_change *entry =
						&tscm->queue[tscm->push_pos];
				unsigned long flag;

				spin_lock_irqsave(&tscm->lock, flag);
				entry->index = index;
				entry->before = before;
				entry->after = after;
				if (++tscm->push_pos >= SND_TSCM_QUEUE_COUNT)
					tscm->push_pos = 0;
				spin_unlock_irqrestore(&tscm->lock, flag);

				wake_up(&tscm->hwdep_wait);
			}
		}

		tscm->state[index] = after;
		buffer += s->data_block_quadlets;
	}
}

static unsigned int process_ir_ctx_payloads(struct amdtp_stream *s,
					    const struct pkt_desc *descs,
					    unsigned int packets,
					    struct snd_pcm_substream *pcm)
{
	unsigned int pcm_frames = 0;
	int i;

	for (i = 0; i < packets; ++i) {
		const struct pkt_desc *desc = descs + i;
		__be32 *buf = desc->ctx_payload;
		unsigned int data_blocks = desc->data_blocks;

		if (pcm) {
			read_pcm_s32(s, pcm, buf, data_blocks, pcm_frames);
			pcm_frames += data_blocks;
		}

		read_status_messages(s, buf, data_blocks);
	}

	return pcm_frames;
}

static unsigned int process_it_ctx_payloads(struct amdtp_stream *s,
					    const struct pkt_desc *descs,
					    unsigned int packets,
					    struct snd_pcm_substream *pcm)
{
	unsigned int pcm_frames = 0;
	int i;

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
	}

	return pcm_frames;
>>>>>>> upstream/android-13
}

int amdtp_tscm_init(struct amdtp_stream *s, struct fw_unit *unit,
		    enum amdtp_stream_direction dir, unsigned int pcm_channels)
{
<<<<<<< HEAD
	amdtp_stream_process_data_blocks_t process_data_blocks;
=======
	amdtp_stream_process_ctx_payloads_t process_ctx_payloads;
	unsigned int flags = CIP_NONBLOCKING | CIP_SKIP_DBC_ZERO_CHECK | CIP_UNAWARE_SYT;
>>>>>>> upstream/android-13
	struct amdtp_tscm *p;
	unsigned int fmt;
	int err;

	if (dir == AMDTP_IN_STREAM) {
		fmt = AMDTP_FMT_TSCM_TX;
<<<<<<< HEAD
		process_data_blocks = process_tx_data_blocks;
	} else {
		fmt = AMDTP_FMT_TSCM_RX;
		process_data_blocks = process_rx_data_blocks;
	}

	err = amdtp_stream_init(s, unit, dir,
				CIP_NONBLOCKING | CIP_SKIP_DBC_ZERO_CHECK, fmt,
				process_data_blocks, sizeof(struct amdtp_tscm));
	if (err < 0)
		return 0;

	/* Use fixed value for FDF field. */
	s->fdf = 0x00;
=======
		process_ctx_payloads = process_ir_ctx_payloads;
	} else {
		fmt = AMDTP_FMT_TSCM_RX;
		process_ctx_payloads = process_it_ctx_payloads;
	}

	err = amdtp_stream_init(s, unit, dir, flags, fmt,
			process_ctx_payloads, sizeof(struct amdtp_tscm));
	if (err < 0)
		return 0;

	if (dir == AMDTP_OUT_STREAM) {
		// Use fixed value for FDF field.
		s->ctx_data.rx.fdf = 0x00;
	}
>>>>>>> upstream/android-13

	/* This protocol uses fixed number of data channels for PCM samples. */
	p = s->protocol;
	p->pcm_channels = pcm_channels;

	return 0;
}
