<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
#ifndef __SOUND_TIMER_H
#define __SOUND_TIMER_H

/*
 *  Timer abstract layer
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>,
 *		     Abramo Bagnara <abramo@alsa-project.org>
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
=======
>>>>>>> upstream/android-13
 */

#include <sound/asound.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
=======
#include <linux/android_kabi.h>
>>>>>>> upstream/android-13

#define snd_timer_chip(timer) ((timer)->private_data)

#define SNDRV_TIMER_DEVICES	16

#define SNDRV_TIMER_DEV_FLG_PCM	0x10000000

#define SNDRV_TIMER_HW_AUTO	0x00000001	/* auto trigger is supported */
#define SNDRV_TIMER_HW_STOP	0x00000002	/* call stop before start */
#define SNDRV_TIMER_HW_SLAVE	0x00000004	/* only slave timer (variable resolution) */
#define SNDRV_TIMER_HW_FIRST	0x00000008	/* first tick can be incomplete */
<<<<<<< HEAD
#define SNDRV_TIMER_HW_TASKLET	0x00000010	/* timer is called from tasklet */
=======
#define SNDRV_TIMER_HW_WORK	0x00000010	/* timer is called from work */
>>>>>>> upstream/android-13

#define SNDRV_TIMER_IFLG_SLAVE	  0x00000001
#define SNDRV_TIMER_IFLG_RUNNING  0x00000002
#define SNDRV_TIMER_IFLG_START	  0x00000004
#define SNDRV_TIMER_IFLG_AUTO	  0x00000008	/* auto restart */
<<<<<<< HEAD
#define SNDRV_TIMER_IFLG_FAST	  0x00000010	/* fast callback (do not use tasklet) */
=======
#define SNDRV_TIMER_IFLG_FAST	  0x00000010	/* fast callback (do not use work) */
>>>>>>> upstream/android-13
#define SNDRV_TIMER_IFLG_CALLBACK 0x00000020	/* timer callback is active */
#define SNDRV_TIMER_IFLG_EXCLUSIVE 0x00000040	/* exclusive owner - no more instances */
#define SNDRV_TIMER_IFLG_EARLY_EVENT 0x00000080	/* write early event to the poll queue */

#define SNDRV_TIMER_FLG_CHANGE	0x00000001
#define SNDRV_TIMER_FLG_RESCHED	0x00000002	/* need reschedule */

struct snd_timer;

struct snd_timer_hardware {
	/* -- must be filled with low-level driver */
	unsigned int flags;		/* various flags */
	unsigned long resolution;	/* average timer resolution for one tick in nsec */
	unsigned long resolution_min;	/* minimal resolution */
	unsigned long resolution_max;	/* maximal resolution */
	unsigned long ticks;		/* max timer ticks per interrupt */
	/* -- low-level functions -- */
	int (*open) (struct snd_timer * timer);
	int (*close) (struct snd_timer * timer);
	unsigned long (*c_resolution) (struct snd_timer * timer);
	int (*start) (struct snd_timer * timer);
	int (*stop) (struct snd_timer * timer);
	int (*set_period) (struct snd_timer * timer, unsigned long period_num, unsigned long period_den);
	int (*precise_resolution) (struct snd_timer * timer, unsigned long *num, unsigned long *den);
<<<<<<< HEAD
=======

	ANDROID_KABI_RESERVE(1);
>>>>>>> upstream/android-13
};

struct snd_timer {
	int tmr_class;
	struct snd_card *card;
	struct module *module;
	int tmr_device;
	int tmr_subdevice;
	char id[64];
	char name[80];
	unsigned int flags;
	int running;			/* running instances */
	unsigned long sticks;		/* schedule ticks */
	void *private_data;
	void (*private_free) (struct snd_timer *timer);
	struct snd_timer_hardware hw;
	spinlock_t lock;
	struct list_head device_list;
	struct list_head open_list_head;
	struct list_head active_list_head;
	struct list_head ack_list_head;
	struct list_head sack_list_head; /* slow ack list head */
<<<<<<< HEAD
	struct tasklet_struct task_queue;
	int max_instances;	/* upper limit of timer instances */
	int num_instances;	/* current number of timer instances */
=======
	struct work_struct task_work;
	int max_instances;	/* upper limit of timer instances */
	int num_instances;	/* current number of timer instances */

	ANDROID_KABI_RESERVE(1);
>>>>>>> upstream/android-13
};

struct snd_timer_instance {
	struct snd_timer *timer;
	char *owner;
	unsigned int flags;
	void *private_data;
	void (*private_free) (struct snd_timer_instance *ti);
	void (*callback) (struct snd_timer_instance *timeri,
			  unsigned long ticks, unsigned long resolution);
	void (*ccallback) (struct snd_timer_instance * timeri,
			   int event,
<<<<<<< HEAD
			   struct timespec * tstamp,
=======
			   struct timespec64 * tstamp,
>>>>>>> upstream/android-13
			   unsigned long resolution);
	void (*disconnect)(struct snd_timer_instance *timeri);
	void *callback_data;
	unsigned long ticks;		/* auto-load ticks when expired */
	unsigned long cticks;		/* current ticks */
	unsigned long pticks;		/* accumulated ticks for callback */
<<<<<<< HEAD
	unsigned long resolution;	/* current resolution for tasklet */
=======
	unsigned long resolution;	/* current resolution for work */
>>>>>>> upstream/android-13
	unsigned long lost;		/* lost ticks */
	int slave_class;
	unsigned int slave_id;
	struct list_head open_list;
	struct list_head active_list;
	struct list_head ack_list;
	struct list_head slave_list_head;
	struct list_head slave_active_head;
	struct snd_timer_instance *master;
<<<<<<< HEAD
=======

	ANDROID_KABI_RESERVE(1);
>>>>>>> upstream/android-13
};

/*
 *  Registering
 */

int snd_timer_new(struct snd_card *card, char *id, struct snd_timer_id *tid, struct snd_timer **rtimer);
<<<<<<< HEAD
void snd_timer_notify(struct snd_timer *timer, int event, struct timespec *tstamp);
=======
void snd_timer_notify(struct snd_timer *timer, int event, struct timespec64 *tstamp);
>>>>>>> upstream/android-13
int snd_timer_global_new(char *id, int device, struct snd_timer **rtimer);
int snd_timer_global_free(struct snd_timer *timer);
int snd_timer_global_register(struct snd_timer *timer);

<<<<<<< HEAD
int snd_timer_open(struct snd_timer_instance **ti, char *owner, struct snd_timer_id *tid, unsigned int slave_id);
int snd_timer_close(struct snd_timer_instance *timeri);
=======
struct snd_timer_instance *snd_timer_instance_new(const char *owner);
void snd_timer_instance_free(struct snd_timer_instance *timeri);
int snd_timer_open(struct snd_timer_instance *timeri, struct snd_timer_id *tid, unsigned int slave_id);
void snd_timer_close(struct snd_timer_instance *timeri);
>>>>>>> upstream/android-13
unsigned long snd_timer_resolution(struct snd_timer_instance *timeri);
int snd_timer_start(struct snd_timer_instance *timeri, unsigned int ticks);
int snd_timer_stop(struct snd_timer_instance *timeri);
int snd_timer_continue(struct snd_timer_instance *timeri);
int snd_timer_pause(struct snd_timer_instance *timeri);

void snd_timer_interrupt(struct snd_timer *timer, unsigned long ticks_left);

#endif /* __SOUND_TIMER_H */
