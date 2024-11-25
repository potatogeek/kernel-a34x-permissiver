/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright 2011 Broadcom Corporation.  All rights reserved. */

#ifndef __SOUND_ARM_BCM2835_H
#define __SOUND_ARM_BCM2835_H

#include <linux/device.h>
<<<<<<< HEAD
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <sound/core.h>
#include <sound/initval.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/pcm-indirect.h>
#include <linux/workqueue.h>

#include "interface/vchi/vchi.h"

/*
 * #define AUDIO_DEBUG_ENABLE
 * #define AUDIO_VERBOSE_DEBUG_ENABLE
 */

/* Debug macros */

#ifdef AUDIO_DEBUG_ENABLE
#ifdef AUDIO_VERBOSE_DEBUG_ENABLE

#define audio_debug(fmt, arg...) \
	pr_info("%s:%d " fmt, __func__, __LINE__, ##arg)

#define audio_info(fmt, arg...) \
	pr_info("%s:%d " fmt, __func__, __LINE__, ##arg)

#else

#define audio_debug(fmt, arg...)

#define audio_info(fmt, arg...)

#endif /* AUDIO_VERBOSE_DEBUG_ENABLE */

#else

#define audio_debug(fmt, arg...)

#define audio_info(fmt, arg...)

#endif /* AUDIO_DEBUG_ENABLE */

#define audio_error(fmt, arg...) \
	pr_err("%s:%d " fmt, __func__, __LINE__, ##arg)

#define audio_warning(fmt, arg...) \
	pr_warn("%s:%d " fmt, __func__, __LINE__, ##arg)

#define audio_alert(fmt, arg...) \
	pr_alert("%s:%d " fmt, __func__, __LINE__, ##arg)
=======
#include <linux/wait.h>
#include <linux/raspberrypi/vchiq.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm-indirect.h>
>>>>>>> upstream/android-13

#define MAX_SUBSTREAMS   (8)
#define AVAIL_SUBSTREAMS_MASK  (0xff)

enum {
	CTRL_VOL_MUTE,
	CTRL_VOL_UNMUTE
};

/* macros for alsa2chip and chip2alsa, instead of functions */

// convert alsa to chip volume (defined as macro rather than function call)
<<<<<<< HEAD
#define alsa2chip(vol) (uint)(-(((vol) << 8) / 100))
=======
#define alsa2chip(vol) ((uint)(-(((vol) << 8) / 100)))
>>>>>>> upstream/android-13

// convert chip to alsa volume
#define chip2alsa(vol) -(((vol) * 100) >> 8)

<<<<<<< HEAD
=======
#define CHIP_MIN_VOLUME		26214 /* minimum level aka mute */

>>>>>>> upstream/android-13
/* Some constants for values .. */
enum snd_bcm2835_route {
	AUDIO_DEST_AUTO = 0,
	AUDIO_DEST_HEADPHONES = 1,
	AUDIO_DEST_HDMI = 2,
	AUDIO_DEST_MAX,
};

enum snd_bcm2835_ctrl {
	PCM_PLAYBACK_VOLUME,
	PCM_PLAYBACK_MUTE,
	PCM_PLAYBACK_DEVICE,
};

struct bcm2835_vchi_ctx {
<<<<<<< HEAD
	VCHI_INSTANCE_T vchi_instance;
	VCHI_CONNECTION_T *vchi_connection;
=======
	struct vchiq_instance *instance;
>>>>>>> upstream/android-13
};

/* definition of the chip-specific record */
struct bcm2835_chip {
	struct snd_card *card;
	struct snd_pcm *pcm;
	struct snd_pcm *pcm_spdif;
<<<<<<< HEAD
	/* Bitmat for valid reg_base and irq numbers */
	unsigned int avail_substreams;
=======
>>>>>>> upstream/android-13
	struct device *dev;
	struct bcm2835_alsa_stream *alsa_stream[MAX_SUBSTREAMS];

	int volume;
<<<<<<< HEAD
	int old_volume; /* stores the volume value whist muted */
=======
>>>>>>> upstream/android-13
	int dest;
	int mute;

	unsigned int opened;
	unsigned int spdif_status;
	struct mutex audio_mutex;

	struct bcm2835_vchi_ctx *vchi_ctx;
};

struct bcm2835_alsa_stream {
	struct bcm2835_chip *chip;
	struct snd_pcm_substream *substream;
	struct snd_pcm_indirect pcm_indirect;

<<<<<<< HEAD
	spinlock_t lock;

	int open;
	int running;
	int draining;

	int channels;
	int params_rate;
	int pcm_format_width;

	unsigned int pos;
	unsigned int buffer_size;
	unsigned int period_size;

	atomic_t retrieved;
	struct bcm2835_audio_instance *instance;
	struct workqueue_struct *my_wq;
=======
	int draining;

	atomic_t pos;
	unsigned int period_offset;
	unsigned int buffer_size;
	unsigned int period_size;
	ktime_t interpolate_start;

	struct bcm2835_audio_instance *instance;
>>>>>>> upstream/android-13
	int idx;
};

int snd_bcm2835_new_ctl(struct bcm2835_chip *chip);
<<<<<<< HEAD
int snd_bcm2835_new_pcm(struct bcm2835_chip *chip, u32 numchannels);
int snd_bcm2835_new_spdif_pcm(struct bcm2835_chip *chip);
int snd_bcm2835_new_simple_pcm(struct bcm2835_chip *chip,
			       const char *name,
			       enum snd_bcm2835_route route,
			       u32 numchannels);
=======
int snd_bcm2835_new_pcm(struct bcm2835_chip *chip, const char *name,
			int idx, enum snd_bcm2835_route route,
			u32 numchannels, bool spdif);
>>>>>>> upstream/android-13

int snd_bcm2835_new_hdmi_ctl(struct bcm2835_chip *chip);
int snd_bcm2835_new_headphones_ctl(struct bcm2835_chip *chip);

<<<<<<< HEAD
int bcm2835_new_vchi_ctx(struct bcm2835_vchi_ctx *vchi_ctx);
=======
int bcm2835_new_vchi_ctx(struct device *dev, struct bcm2835_vchi_ctx *vchi_ctx);
>>>>>>> upstream/android-13
void bcm2835_free_vchi_ctx(struct bcm2835_vchi_ctx *vchi_ctx);

int bcm2835_audio_open(struct bcm2835_alsa_stream *alsa_stream);
int bcm2835_audio_close(struct bcm2835_alsa_stream *alsa_stream);
int bcm2835_audio_set_params(struct bcm2835_alsa_stream *alsa_stream,
			     unsigned int channels, unsigned int samplerate,
			     unsigned int bps);
<<<<<<< HEAD
int bcm2835_audio_setup(struct bcm2835_alsa_stream *alsa_stream);
int bcm2835_audio_start(struct bcm2835_alsa_stream *alsa_stream);
int bcm2835_audio_stop(struct bcm2835_alsa_stream *alsa_stream);
int bcm2835_audio_set_ctls(struct bcm2835_chip *chip);
int bcm2835_audio_write(struct bcm2835_alsa_stream *alsa_stream,
			unsigned int count,
			void *src);
void bcm2835_playback_fifo(struct bcm2835_alsa_stream *alsa_stream);
unsigned int bcm2835_audio_retrieve_buffers(struct bcm2835_alsa_stream *alsa_stream);
void bcm2835_audio_flush_buffers(struct bcm2835_alsa_stream *alsa_stream);
void bcm2835_audio_flush_playback_buffers(struct bcm2835_alsa_stream *alsa_stream);
=======
int bcm2835_audio_start(struct bcm2835_alsa_stream *alsa_stream);
int bcm2835_audio_stop(struct bcm2835_alsa_stream *alsa_stream);
int bcm2835_audio_drain(struct bcm2835_alsa_stream *alsa_stream);
int bcm2835_audio_set_ctls(struct bcm2835_alsa_stream *alsa_stream);
int bcm2835_audio_write(struct bcm2835_alsa_stream *alsa_stream,
			unsigned int count,
			void *src);
void bcm2835_playback_fifo(struct bcm2835_alsa_stream *alsa_stream,
			   unsigned int size);
unsigned int bcm2835_audio_retrieve_buffers(struct bcm2835_alsa_stream *alsa_stream);
>>>>>>> upstream/android-13

#endif /* __SOUND_ARM_BCM2835_H */
