<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * governor.h - internal header for devfreq governors.
 *
 * Copyright (C) 2011 Samsung Electronics
 *	MyungJoo Ham <myungjoo.ham@samsung.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 * This header is for devfreq governors in drivers/devfreq/
 */

#ifndef _GOVERNOR_H
#define _GOVERNOR_H

#include <linux/devfreq.h>

<<<<<<< HEAD
=======
#define DEVFREQ_NAME_LEN			16

>>>>>>> upstream/android-13
#define to_devfreq(DEV)	container_of((DEV), struct devfreq, dev)

/* Devfreq events */
#define DEVFREQ_GOV_START			0x1
#define DEVFREQ_GOV_STOP			0x2
<<<<<<< HEAD
#define DEVFREQ_GOV_INTERVAL			0x3
#define DEVFREQ_GOV_SUSPEND			0x4
#define DEVFREQ_GOV_RESUME			0x5

=======
#define DEVFREQ_GOV_UPDATE_INTERVAL		0x3
#define DEVFREQ_GOV_SUSPEND			0x4
#define DEVFREQ_GOV_RESUME			0x5

#define DEVFREQ_MIN_FREQ			0
#define DEVFREQ_MAX_FREQ			ULONG_MAX

/*
 * Definition of the governor feature flags
 * - DEVFREQ_GOV_FLAG_IMMUTABLE
 *   : This governor is never changeable to other governors.
 * - DEVFREQ_GOV_FLAG_IRQ_DRIVEN
 *   : The devfreq won't schedule the work for this governor.
 */
#define DEVFREQ_GOV_FLAG_IMMUTABLE			BIT(0)
#define DEVFREQ_GOV_FLAG_IRQ_DRIVEN			BIT(1)

/*
 * Definition of governor attribute flags except for common sysfs attributes
 * - DEVFREQ_GOV_ATTR_POLLING_INTERVAL
 *   : Indicate polling_interval sysfs attribute
 * - DEVFREQ_GOV_ATTR_TIMER
 *   : Indicate timer sysfs attribute
 */
#define DEVFREQ_GOV_ATTR_POLLING_INTERVAL		BIT(0)
#define DEVFREQ_GOV_ATTR_TIMER				BIT(1)

>>>>>>> upstream/android-13
/**
 * struct devfreq_governor - Devfreq policy governor
 * @node:		list node - contains registered devfreq governors
 * @name:		Governor's name
<<<<<<< HEAD
 * @immutable:		Immutable flag for governor. If the value is 1,
 *			this govenror is never changeable to other governor.
=======
 * @attrs:		Governor's sysfs attribute flags
 * @flags:		Governor's feature flags
>>>>>>> upstream/android-13
 * @get_target_freq:	Returns desired operating frequency for the device.
 *			Basically, get_target_freq will run
 *			devfreq_dev_profile.get_dev_status() to get the
 *			status of the device (load = busy_time / total_time).
<<<<<<< HEAD
 *			If no_central_polling is set, this callback is called
 *			only with update_devfreq() notified by OPP.
=======
>>>>>>> upstream/android-13
 * @event_handler:      Callback for devfreq core framework to notify events
 *                      to governors. Events include per device governor
 *                      init and exit, opp changes out of devfreq, suspend
 *                      and resume of per device devfreq during device idle.
 *
 * Note that the callbacks are called with devfreq->lock locked by devfreq.
 */
struct devfreq_governor {
	struct list_head node;

	const char name[DEVFREQ_NAME_LEN];
<<<<<<< HEAD
	const unsigned int immutable;
=======
	const u64 attrs;
	const u64 flags;
>>>>>>> upstream/android-13
	int (*get_target_freq)(struct devfreq *this, unsigned long *freq);
	int (*event_handler)(struct devfreq *devfreq,
				unsigned int event, void *data);
};

<<<<<<< HEAD
/* Caution: devfreq->lock must be locked before calling update_devfreq */
extern int update_devfreq(struct devfreq *devfreq);

extern void devfreq_monitor_start(struct devfreq *devfreq);
extern void devfreq_monitor_stop(struct devfreq *devfreq);
extern void devfreq_monitor_suspend(struct devfreq *devfreq);
extern void devfreq_monitor_resume(struct devfreq *devfreq);
extern void devfreq_interval_update(struct devfreq *devfreq,
					unsigned int *delay);

extern int devfreq_add_governor(struct devfreq_governor *governor);
extern int devfreq_remove_governor(struct devfreq_governor *governor);

extern int devfreq_update_status(struct devfreq *devfreq, unsigned long freq);

static inline int devfreq_update_stats(struct devfreq *df)
{
=======
void devfreq_monitor_start(struct devfreq *devfreq);
void devfreq_monitor_stop(struct devfreq *devfreq);
void devfreq_monitor_suspend(struct devfreq *devfreq);
void devfreq_monitor_resume(struct devfreq *devfreq);
void devfreq_update_interval(struct devfreq *devfreq, unsigned int *delay);

int devfreq_add_governor(struct devfreq_governor *governor);
int devfreq_remove_governor(struct devfreq_governor *governor);

int devfreq_update_status(struct devfreq *devfreq, unsigned long freq);
int devfreq_update_target(struct devfreq *devfreq, unsigned long freq);

static inline int devfreq_update_stats(struct devfreq *df)
{
	if (!df->profile->get_dev_status)
		return -EINVAL;

>>>>>>> upstream/android-13
	return df->profile->get_dev_status(df->dev.parent, &df->last_status);
}
#endif /* _GOVERNOR_H */
