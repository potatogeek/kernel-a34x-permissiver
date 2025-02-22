<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
#ifndef __SOUND_SEQ_VIRMIDI_H
#define __SOUND_SEQ_VIRMIDI_H

/*
 *  Virtual Raw MIDI client on Sequencer
 *  Copyright (c) 2000 by Takashi Iwai <tiwai@suse.de>,
 *                        Jaroslav Kysela <perex@perex.cz>
<<<<<<< HEAD
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
=======
>>>>>>> upstream/android-13
 */

#include <sound/rawmidi.h>
#include <sound/seq_midi_event.h>

/*
 * device file instance:
 * This instance is created at each time the midi device file is
 * opened.  Each instance has its own input buffer and MIDI parser
 * (buffer), and is associated with the device instance.
 */
struct snd_virmidi {
	struct list_head list;
	int seq_mode;
	int client;
	int port;
	bool trigger;
	struct snd_midi_event *parser;
	struct snd_seq_event event;
	struct snd_virmidi_dev *rdev;
	struct snd_rawmidi_substream *substream;
	struct work_struct output_work;
};

#define SNDRV_VIRMIDI_SUBSCRIBE		(1<<0)
#define SNDRV_VIRMIDI_USE		(1<<1)

/*
 * device record:
 * Each virtual midi device has one device instance.  It contains
 * common information and the linked-list of opened files, 
 */
struct snd_virmidi_dev {
	struct snd_card *card;		/* associated card */
	struct snd_rawmidi *rmidi;		/* rawmidi device */
	int seq_mode;			/* SNDRV_VIRMIDI_XXX */
	int device;			/* sequencer device */
	int client;			/* created/attached client */
	int port;			/* created/attached port */
	unsigned int flags;		/* SNDRV_VIRMIDI_* */
	rwlock_t filelist_lock;
	struct rw_semaphore filelist_sem;
	struct list_head filelist;
};

/* sequencer mode:
 * ATTACH = input/output events from midi device are routed to the
 *          attached sequencer port.  sequencer port is not created
 *          by virmidi itself.
 *          the input to rawmidi must be processed by passing the
 *          incoming events via snd_virmidi_receive()
 * DISPATCH = input/output events are routed to subscribers.
 *            sequencer port is created in virmidi.
 */
#define SNDRV_VIRMIDI_SEQ_NONE		0
#define SNDRV_VIRMIDI_SEQ_ATTACH	1
#define SNDRV_VIRMIDI_SEQ_DISPATCH	2

int snd_virmidi_new(struct snd_card *card, int device, struct snd_rawmidi **rrmidi);

#endif /* __SOUND_SEQ_VIRMIDI */
