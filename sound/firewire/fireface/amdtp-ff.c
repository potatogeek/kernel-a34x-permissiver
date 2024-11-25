<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * amdtp-ff.c - a part of driver for RME Fireface series
 *
 * Copyright (c) 2015-2017 Takashi Sakamoto
<<<<<<< HEAD
 *
 * Licensed under the terms of the GNU General Public License, version 2.
=======
>>>>>>> upstream/android-13
 */

#include <sound/pcm.h>
#include "ff.h"

struct amdtp_ff {
	unsigned int pcm_channels;
};

int amdtp_ff_set_parameters(struct amdtp_stream *s, unsigned int rate,
			    unsigned int pcm_channels)
{
	struct amdtp_ff *p = s->protocol;
	unsigned int data_channels;

	if (amdtp_stream_running(s))
		return -EBUSY;

	p->pcm_channels = pcm_channels;
	data_channels = pcm_channels;

	return amdtp_stream_set_parameters(s, rate, data_channels);
}

<<<<<<< HEAD
static void write_pcm_s32(struct amdtp_stream *s,
			  struct snd_pcm_substream *pcm,
			  __le32 *buffer, unsigned int frames)
{
	struct amdtp_ff *p = s->protocol;
	struct snd_pcm_runtime *runtime = pcm->runtime;
	unsigned int channels, remaining_frames, i, c;
	const u32 *src;

	channels = p->pcm_channels;
	src = (void *)runtime->dma_area +
			frames_to_bytes(runtime, s->pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;
=======
static void write_pcm_s32(struct amdtp_stream *s, struct snd_pcm_substream *pcm,
			  __le32 *buffer, unsigned int frames,
			  unsigned int pcm_frames)
{
	struct amdtp_ff *p = s->protocol;
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
			buffer[c] = cpu_to_le32(*src);
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
			 __le32 *buffer, unsigned int frames)
{
	struct amdtp_ff *p = s->protocol;
	struct snd_pcm_runtime *runtime = pcm->runtime;
	unsigned int channels, remaining_frames, i, c;
	u32 *dst;

	channels = p->pcm_channels;
	dst  = (void *)runtime->dma_area +
			frames_to_bytes(runtime, s->pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;
=======
static void read_pcm_s32(struct amdtp_stream *s, struct snd_pcm_substream *pcm,
			 __le32 *buffer, unsigned int frames,
			 unsigned int pcm_frames)
{
	struct amdtp_ff *p = s->protocol;
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

	for (i = 0; i < frames; ++i) {
		for (c = 0; c < channels; ++c) {
			*dst = le32_to_cpu(buffer[c]) & 0xffffff00;
			dst++;
		}
		buffer += s->data_block_quadlets;
		if (--remaining_frames == 0)
			dst = (void *)runtime->dma_area;
	}
}

static void write_pcm_silence(struct amdtp_stream *s,
			      __le32 *buffer, unsigned int frames)
{
	struct amdtp_ff *p = s->protocol;
	unsigned int i, c, channels = p->pcm_channels;

	for (i = 0; i < frames; ++i) {
		for (c = 0; c < channels; ++c)
			buffer[c] = cpu_to_le32(0x00000000);
		buffer += s->data_block_quadlets;
	}
}

int amdtp_ff_add_pcm_hw_constraints(struct amdtp_stream *s,
				    struct snd_pcm_runtime *runtime)
{
	int err;

	err = snd_pcm_hw_constraint_msbits(runtime, 0, 32, 24);
	if (err < 0)
		return err;

	return amdtp_stream_add_pcm_hw_constraints(s, runtime);
}

<<<<<<< HEAD
static unsigned int process_rx_data_blocks(struct amdtp_stream *s,
					   __be32 *buffer,
					   unsigned int data_blocks,
					   unsigned int *syt)
{
	struct snd_pcm_substream *pcm = READ_ONCE(s->pcm);
	unsigned int pcm_frames;

	if (pcm) {
		write_pcm_s32(s, pcm, (__le32 *)buffer, data_blocks);
		pcm_frames = data_blocks;
	} else {
		write_pcm_silence(s, (__le32 *)buffer, data_blocks);
		pcm_frames = 0;
=======
static unsigned int process_it_ctx_payloads(struct amdtp_stream *s,
					   const struct pkt_desc *descs,
					   unsigned int packets,
					   struct snd_pcm_substream *pcm)
{
	unsigned int pcm_frames = 0;
	int i;

	for (i = 0; i < packets; ++i) {
		const struct pkt_desc *desc = descs + i;
		__le32 *buf = (__le32 *)desc->ctx_payload;
		unsigned int data_blocks = desc->data_blocks;

		if (pcm) {
			write_pcm_s32(s, pcm, buf, data_blocks, pcm_frames);
			pcm_frames += data_blocks;
		} else {
			write_pcm_silence(s, buf, data_blocks);
		}
>>>>>>> upstream/android-13
	}

	return pcm_frames;
}

<<<<<<< HEAD
static unsigned int process_tx_data_blocks(struct amdtp_stream *s,
					   __be32 *buffer,
					   unsigned int data_blocks,
					   unsigned int *syt)
{
	struct snd_pcm_substream *pcm = READ_ONCE(s->pcm);
	unsigned int pcm_frames;

	if (pcm) {
		read_pcm_s32(s, pcm, (__le32 *)buffer, data_blocks);
		pcm_frames = data_blocks;
	} else {
		pcm_frames = 0;
=======
static unsigned int process_ir_ctx_payloads(struct amdtp_stream *s,
					    const struct pkt_desc *descs,
					    unsigned int packets,
					    struct snd_pcm_substream *pcm)
{
	unsigned int pcm_frames = 0;
	int i;

	for (i = 0; i < packets; ++i) {
		const struct pkt_desc *desc = descs + i;
		__le32 *buf = (__le32 *)desc->ctx_payload;
		unsigned int data_blocks = desc->data_blocks;

		if (pcm) {
			read_pcm_s32(s, pcm, buf, data_blocks, pcm_frames);
			pcm_frames += data_blocks;
		}
>>>>>>> upstream/android-13
	}

	return pcm_frames;
}

int amdtp_ff_init(struct amdtp_stream *s, struct fw_unit *unit,
		  enum amdtp_stream_direction dir)
{
<<<<<<< HEAD
	amdtp_stream_process_data_blocks_t process_data_blocks;

	if (dir == AMDTP_IN_STREAM)
		process_data_blocks = process_tx_data_blocks;
	else
		process_data_blocks = process_rx_data_blocks;

	return amdtp_stream_init(s, unit, dir, CIP_NO_HEADER, 0,
				 process_data_blocks, sizeof(struct amdtp_ff));
=======
	amdtp_stream_process_ctx_payloads_t process_ctx_payloads;

	if (dir == AMDTP_IN_STREAM)
		process_ctx_payloads = process_ir_ctx_payloads;
	else
		process_ctx_payloads = process_it_ctx_payloads;

	return amdtp_stream_init(s, unit, dir, CIP_BLOCKING | CIP_UNAWARE_SYT | CIP_NO_HEADER, 0,
				 process_ctx_payloads, sizeof(struct amdtp_ff));
>>>>>>> upstream/android-13
}
