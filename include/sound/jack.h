<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
#ifndef __SOUND_JACK_H
#define __SOUND_JACK_H

/*
 *  Jack abstraction layer
 *
 *  Copyright 2008 Wolfson Microelectronics plc
<<<<<<< HEAD
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <sound/core.h>
=======
 */

#include <sound/core.h>
#include <linux/android_kabi.h>
>>>>>>> upstream/android-13

struct input_dev;

/**
 * enum snd_jack_types - Jack types which can be reported
 * @SND_JACK_HEADPHONE: Headphone
 * @SND_JACK_MICROPHONE: Microphone
 * @SND_JACK_HEADSET: Headset
 * @SND_JACK_LINEOUT: Line out
 * @SND_JACK_MECHANICAL: Mechanical switch
 * @SND_JACK_VIDEOOUT: Video out
 * @SND_JACK_AVOUT: AV (Audio Video) out
 * @SND_JACK_LINEIN:  Line in
 * @SND_JACK_BTN_0: Button 0
 * @SND_JACK_BTN_1: Button 1
 * @SND_JACK_BTN_2: Button 2
 * @SND_JACK_BTN_3: Button 3
 * @SND_JACK_BTN_4: Button 4
 * @SND_JACK_BTN_5: Button 5
 *
 * These values are used as a bitmask.
 *
 * Note that this must be kept in sync with the lookup table in
 * sound/core/jack.c.
 */
enum snd_jack_types {
	SND_JACK_HEADPHONE	= 0x0001,
	SND_JACK_MICROPHONE	= 0x0002,
	SND_JACK_HEADSET	= SND_JACK_HEADPHONE | SND_JACK_MICROPHONE,
	SND_JACK_LINEOUT	= 0x0004,
	SND_JACK_MECHANICAL	= 0x0008, /* If detected separately */
	SND_JACK_VIDEOOUT	= 0x0010,
	SND_JACK_AVOUT		= SND_JACK_LINEOUT | SND_JACK_VIDEOOUT,
	SND_JACK_LINEIN		= 0x0020,
<<<<<<< HEAD
	SND_JACK_OC_HPHL        = 0x0040,
	SND_JACK_OC_HPHR        = 0x0080,
	SND_JACK_UNSUPPORTED    = 0x0100,
	SND_JACK_MICROPHONE2    = 0x0200,
	SND_JACK_ANC_HEADPHONE  = SND_JACK_HEADPHONE | SND_JACK_MICROPHONE |
				  SND_JACK_MICROPHONE2,

	/* Kept separate from switches to facilitate implementation */
	SND_JACK_BTN_0		= 0x8000,
	SND_JACK_BTN_1		= 0x4000,
	SND_JACK_BTN_2		= 0x2000,
	SND_JACK_BTN_3		= 0x1000,
	SND_JACK_BTN_4		= 0x0800,
	SND_JACK_BTN_5		= 0x0400,
};

=======

	/* Kept separate from switches to facilitate implementation */
	SND_JACK_BTN_0		= 0x4000,
	SND_JACK_BTN_1		= 0x2000,
	SND_JACK_BTN_2		= 0x1000,
	SND_JACK_BTN_3		= 0x0800,
	SND_JACK_BTN_4		= 0x0400,
	SND_JACK_BTN_5		= 0x0200,
};

/* Keep in sync with definitions above */
#define SND_JACK_SWITCH_TYPES 6

>>>>>>> upstream/android-13
struct snd_jack {
	struct list_head kctl_list;
	struct snd_card *card;
	const char *id;
#ifdef CONFIG_SND_JACK_INPUT_DEV
	struct input_dev *input_dev;
	int registered;
	int type;
	char name[100];
	unsigned int key[6];   /* Keep in sync with definitions above */
#endif /* CONFIG_SND_JACK_INPUT_DEV */
<<<<<<< HEAD
	void *private_data;
	void (*private_free)(struct snd_jack *);
=======
	int hw_status_cache;
	void *private_data;
	void (*private_free)(struct snd_jack *);

	ANDROID_KABI_RESERVE(1);
>>>>>>> upstream/android-13
};

#ifdef CONFIG_SND_JACK

int snd_jack_new(struct snd_card *card, const char *id, int type,
		 struct snd_jack **jack, bool initial_kctl, bool phantom_jack);
int snd_jack_add_new_kctl(struct snd_jack *jack, const char * name, int mask);
#ifdef CONFIG_SND_JACK_INPUT_DEV
void snd_jack_set_parent(struct snd_jack *jack, struct device *parent);
int snd_jack_set_key(struct snd_jack *jack, enum snd_jack_types type,
		     int keytype);
#endif
void snd_jack_report(struct snd_jack *jack, int status);

#else
static inline int snd_jack_new(struct snd_card *card, const char *id, int type,
			       struct snd_jack **jack, bool initial_kctl, bool phantom_jack)
{
	return 0;
}

static inline int snd_jack_add_new_kctl(struct snd_jack *jack, const char * name, int mask)
{
	return 0;
}

static inline void snd_jack_report(struct snd_jack *jack, int status)
{
}

#endif

#if !defined(CONFIG_SND_JACK) || !defined(CONFIG_SND_JACK_INPUT_DEV)
static inline void snd_jack_set_parent(struct snd_jack *jack,
				       struct device *parent)
{
}

static inline int snd_jack_set_key(struct snd_jack *jack,
				   enum snd_jack_types type,
				   int keytype)
{
	return 0;
}
#endif /* !CONFIG_SND_JACK || !CONFIG_SND_JACK_INPUT_DEV */

#endif
