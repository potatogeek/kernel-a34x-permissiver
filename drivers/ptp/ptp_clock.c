<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * PTP 1588 clock support
 *
 * Copyright (C) 2010 OMICRON electronics GmbH
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> upstream/android-13
 */
#include <linux/idr.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/posix-clock.h>
#include <linux/pps_kernel.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <uapi/linux/sched/types.h>

#include "ptp_private.h"

#define PTP_MAX_ALARMS 4
#define PTP_PPS_DEFAULTS (PPS_CAPTUREASSERT | PPS_OFFSETASSERT)
#define PTP_PPS_EVENT PPS_CAPTUREASSERT
#define PTP_PPS_MODE (PTP_PPS_DEFAULTS | PPS_CANWAIT | PPS_TSFMT_TSPEC)

<<<<<<< HEAD
/* private globals */

static dev_t ptp_devt;
static struct class *ptp_class;
=======
struct class *ptp_class;

/* private globals */

static dev_t ptp_devt;
>>>>>>> upstream/android-13

static DEFINE_IDA(ptp_clocks_map);

/* time stamp event queue operations */

static inline int queue_free(struct timestamp_event_queue *q)
{
	return PTP_MAX_TIMESTAMPS - queue_cnt(q) - 1;
}

static void enqueue_external_timestamp(struct timestamp_event_queue *queue,
				       struct ptp_clock_event *src)
{
	struct ptp_extts_event *dst;
	unsigned long flags;
	s64 seconds;
	u32 remainder;

	seconds = div_u64_rem(src->timestamp, 1000000000, &remainder);

	spin_lock_irqsave(&queue->lock, flags);

	dst = &queue->buf[queue->tail];
	dst->index = src->index;
	dst->t.sec = seconds;
	dst->t.nsec = remainder;

	if (!queue_free(queue))
		queue->head = (queue->head + 1) % PTP_MAX_TIMESTAMPS;

	queue->tail = (queue->tail + 1) % PTP_MAX_TIMESTAMPS;

	spin_unlock_irqrestore(&queue->lock, flags);
}

<<<<<<< HEAD
static s32 scaled_ppm_to_ppb(long ppm)
{
	/*
	 * The 'freq' field in the 'struct timex' is in parts per
	 * million, but with a 16 bit binary fractional field.
	 *
	 * We want to calculate
	 *
	 *    ppb = scaled_ppm * 1000 / 2^16
	 *
	 * which simplifies to
	 *
	 *    ppb = scaled_ppm * 125 / 2^13
	 */
	s64 ppb = 1 + ppm;
	ppb *= 125;
	ppb >>= 13;
	return (s32) ppb;
}

=======
>>>>>>> upstream/android-13
/* posix clock implementation */

static int ptp_clock_getres(struct posix_clock *pc, struct timespec64 *tp)
{
	tp->tv_sec = 0;
	tp->tv_nsec = 1;
	return 0;
}

static int ptp_clock_settime(struct posix_clock *pc, const struct timespec64 *tp)
{
	struct ptp_clock *ptp = container_of(pc, struct ptp_clock, clock);

<<<<<<< HEAD
=======
	if (ptp_vclock_in_use(ptp)) {
		pr_err("ptp: virtual clock in use\n");
		return -EBUSY;
	}

>>>>>>> upstream/android-13
	return  ptp->info->settime64(ptp->info, tp);
}

static int ptp_clock_gettime(struct posix_clock *pc, struct timespec64 *tp)
{
	struct ptp_clock *ptp = container_of(pc, struct ptp_clock, clock);
	int err;

<<<<<<< HEAD
	err = ptp->info->gettime64(ptp->info, tp);
	return err;
}

static int ptp_clock_adjtime(struct posix_clock *pc, struct timex *tx)
=======
	if (ptp->info->gettimex64)
		err = ptp->info->gettimex64(ptp->info, tp, NULL);
	else
		err = ptp->info->gettime64(ptp->info, tp);
	return err;
}

static int ptp_clock_adjtime(struct posix_clock *pc, struct __kernel_timex *tx)
>>>>>>> upstream/android-13
{
	struct ptp_clock *ptp = container_of(pc, struct ptp_clock, clock);
	struct ptp_clock_info *ops;
	int err = -EOPNOTSUPP;

<<<<<<< HEAD
=======
	if (ptp_vclock_in_use(ptp)) {
		pr_err("ptp: virtual clock in use\n");
		return -EBUSY;
	}

>>>>>>> upstream/android-13
	ops = ptp->info;

	if (tx->modes & ADJ_SETOFFSET) {
		struct timespec64 ts;
		ktime_t kt;
		s64 delta;

		ts.tv_sec  = tx->time.tv_sec;
		ts.tv_nsec = tx->time.tv_usec;

		if (!(tx->modes & ADJ_NANO))
			ts.tv_nsec *= 1000;

		if ((unsigned long) ts.tv_nsec >= NSEC_PER_SEC)
			return -EINVAL;

		kt = timespec64_to_ktime(ts);
		delta = ktime_to_ns(kt);
		err = ops->adjtime(ops, delta);
	} else if (tx->modes & ADJ_FREQUENCY) {
<<<<<<< HEAD
		s32 ppb = scaled_ppm_to_ppb(tx->freq);
=======
		long ppb = scaled_ppm_to_ppb(tx->freq);
>>>>>>> upstream/android-13
		if (ppb > ops->max_adj || ppb < -ops->max_adj)
			return -ERANGE;
		if (ops->adjfine)
			err = ops->adjfine(ops, tx->freq);
		else
			err = ops->adjfreq(ops, ppb);
		ptp->dialed_frequency = tx->freq;
<<<<<<< HEAD
=======
	} else if (tx->modes & ADJ_OFFSET) {
		if (ops->adjphase) {
			s32 offset = tx->offset;

			if (!(tx->modes & ADJ_NANO))
				offset *= NSEC_PER_USEC;

			err = ops->adjphase(ops, offset);
		}
>>>>>>> upstream/android-13
	} else if (tx->modes == 0) {
		tx->freq = ptp->dialed_frequency;
		err = 0;
	}

	return err;
}

static struct posix_clock_operations ptp_clock_ops = {
	.owner		= THIS_MODULE,
	.clock_adjtime	= ptp_clock_adjtime,
	.clock_gettime	= ptp_clock_gettime,
	.clock_getres	= ptp_clock_getres,
	.clock_settime	= ptp_clock_settime,
	.ioctl		= ptp_ioctl,
	.open		= ptp_open,
	.poll		= ptp_poll,
	.read		= ptp_read,
};

static void ptp_clock_release(struct device *dev)
{
	struct ptp_clock *ptp = container_of(dev, struct ptp_clock, dev);

	ptp_cleanup_pin_groups(ptp);
<<<<<<< HEAD
	mutex_destroy(&ptp->tsevq_mux);
	mutex_destroy(&ptp->pincfg_mux);
=======
	kfree(ptp->vclock_index);
	mutex_destroy(&ptp->tsevq_mux);
	mutex_destroy(&ptp->pincfg_mux);
	mutex_destroy(&ptp->n_vclocks_mux);
>>>>>>> upstream/android-13
	ida_simple_remove(&ptp_clocks_map, ptp->index);
	kfree(ptp);
}

static void ptp_aux_kworker(struct kthread_work *work)
{
	struct ptp_clock *ptp = container_of(work, struct ptp_clock,
					     aux_work.work);
	struct ptp_clock_info *info = ptp->info;
	long delay;

	delay = info->do_aux_work(info);

	if (delay >= 0)
		kthread_queue_delayed_work(ptp->kworker, &ptp->aux_work, delay);
}

/* public interface */

struct ptp_clock *ptp_clock_register(struct ptp_clock_info *info,
				     struct device *parent)
{
	struct ptp_clock *ptp;
	int err = 0, index, major = MAJOR(ptp_devt);
<<<<<<< HEAD
=======
	size_t size;
>>>>>>> upstream/android-13

	if (info->n_alarm > PTP_MAX_ALARMS)
		return ERR_PTR(-EINVAL);

	/* Initialize a clock structure. */
	err = -ENOMEM;
	ptp = kzalloc(sizeof(struct ptp_clock), GFP_KERNEL);
	if (ptp == NULL)
		goto no_memory;

	index = ida_simple_get(&ptp_clocks_map, 0, MINORMASK + 1, GFP_KERNEL);
	if (index < 0) {
		err = index;
		goto no_slot;
	}

	ptp->clock.ops = ptp_clock_ops;
	ptp->info = info;
	ptp->devid = MKDEV(major, index);
	ptp->index = index;
	spin_lock_init(&ptp->tsevq.lock);
	mutex_init(&ptp->tsevq_mux);
	mutex_init(&ptp->pincfg_mux);
<<<<<<< HEAD
	init_waitqueue_head(&ptp->tsev_wq);

	if (ptp->info->do_aux_work) {
		char *worker_name = kasprintf(GFP_KERNEL, "ptp%d", ptp->index);

		kthread_init_delayed_work(&ptp->aux_work, ptp_aux_kworker);
		ptp->kworker = kthread_create_worker(0, worker_name ?
						     worker_name : info->name);
		kfree(worker_name);
=======
	mutex_init(&ptp->n_vclocks_mux);
	init_waitqueue_head(&ptp->tsev_wq);

	if (ptp->info->do_aux_work) {
		kthread_init_delayed_work(&ptp->aux_work, ptp_aux_kworker);
		ptp->kworker = kthread_create_worker(0, "ptp%d", ptp->index);
>>>>>>> upstream/android-13
		if (IS_ERR(ptp->kworker)) {
			err = PTR_ERR(ptp->kworker);
			pr_err("failed to create ptp aux_worker %d\n", err);
			goto kworker_err;
		}
	}

<<<<<<< HEAD
=======
	/* PTP virtual clock is being registered under physical clock */
	if (parent && parent->class && parent->class->name &&
	    strcmp(parent->class->name, "ptp") == 0)
		ptp->is_virtual_clock = true;

	if (!ptp->is_virtual_clock) {
		ptp->max_vclocks = PTP_DEFAULT_MAX_VCLOCKS;

		size = sizeof(int) * ptp->max_vclocks;
		ptp->vclock_index = kzalloc(size, GFP_KERNEL);
		if (!ptp->vclock_index) {
			err = -ENOMEM;
			goto no_mem_for_vclocks;
		}
	}

>>>>>>> upstream/android-13
	err = ptp_populate_pin_groups(ptp);
	if (err)
		goto no_pin_groups;

	/* Register a new PPS source. */
	if (info->pps) {
		struct pps_source_info pps;
		memset(&pps, 0, sizeof(pps));
		snprintf(pps.name, PPS_MAX_NAME_LEN, "ptp%d", index);
		pps.mode = PTP_PPS_MODE;
		pps.owner = info->owner;
		ptp->pps_source = pps_register_source(&pps, PTP_PPS_DEFAULTS);
<<<<<<< HEAD
		if (!ptp->pps_source) {
			err = -EINVAL;
			pr_err("failed to register pps source\n");
			goto no_pps;
		}
=======
		if (IS_ERR(ptp->pps_source)) {
			err = PTR_ERR(ptp->pps_source);
			pr_err("failed to register pps source\n");
			goto no_pps;
		}
		ptp->pps_source->lookup_cookie = ptp;
>>>>>>> upstream/android-13
	}

	/* Initialize a new device of our class in our clock structure. */
	device_initialize(&ptp->dev);
	ptp->dev.devt = ptp->devid;
	ptp->dev.class = ptp_class;
	ptp->dev.parent = parent;
	ptp->dev.groups = ptp->pin_attr_groups;
	ptp->dev.release = ptp_clock_release;
	dev_set_drvdata(&ptp->dev, ptp);
	dev_set_name(&ptp->dev, "ptp%d", ptp->index);

	/* Create a posix clock and link it to the device. */
	err = posix_clock_register(&ptp->clock, &ptp->dev);
	if (err) {
<<<<<<< HEAD
		pr_err("failed to create posix clock\n");
		goto no_clock;
=======
	        if (ptp->pps_source)
	                pps_unregister_source(ptp->pps_source);

		if (ptp->kworker)
	                kthread_destroy_worker(ptp->kworker);

		put_device(&ptp->dev);

		pr_err("failed to create posix clock\n");
		return ERR_PTR(err);
>>>>>>> upstream/android-13
	}

	return ptp;

<<<<<<< HEAD
no_clock:
	if (ptp->pps_source)
		pps_unregister_source(ptp->pps_source);
no_pps:
	ptp_cleanup_pin_groups(ptp);
no_pin_groups:
=======
no_pps:
	ptp_cleanup_pin_groups(ptp);
no_pin_groups:
	kfree(ptp->vclock_index);
no_mem_for_vclocks:
>>>>>>> upstream/android-13
	if (ptp->kworker)
		kthread_destroy_worker(ptp->kworker);
kworker_err:
	mutex_destroy(&ptp->tsevq_mux);
	mutex_destroy(&ptp->pincfg_mux);
<<<<<<< HEAD
=======
	mutex_destroy(&ptp->n_vclocks_mux);
>>>>>>> upstream/android-13
	ida_simple_remove(&ptp_clocks_map, index);
no_slot:
	kfree(ptp);
no_memory:
	return ERR_PTR(err);
}
EXPORT_SYMBOL(ptp_clock_register);

<<<<<<< HEAD
int ptp_clock_unregister(struct ptp_clock *ptp)
{
=======
static int unregister_vclock(struct device *dev, void *data)
{
	struct ptp_clock *ptp = dev_get_drvdata(dev);

	ptp_vclock_unregister(info_to_vclock(ptp->info));
	return 0;
}

int ptp_clock_unregister(struct ptp_clock *ptp)
{
	if (ptp_vclock_in_use(ptp)) {
		device_for_each_child(&ptp->dev, NULL, unregister_vclock);
	}

>>>>>>> upstream/android-13
	ptp->defunct = 1;
	wake_up_interruptible(&ptp->tsev_wq);

	if (ptp->kworker) {
		kthread_cancel_delayed_work_sync(&ptp->aux_work);
		kthread_destroy_worker(ptp->kworker);
	}

	/* Release the clock's resources. */
	if (ptp->pps_source)
		pps_unregister_source(ptp->pps_source);

	posix_clock_unregister(&ptp->clock);

	return 0;
}
EXPORT_SYMBOL(ptp_clock_unregister);

void ptp_clock_event(struct ptp_clock *ptp, struct ptp_clock_event *event)
{
	struct pps_event_time evt;

	switch (event->type) {

	case PTP_CLOCK_ALARM:
		break;

	case PTP_CLOCK_EXTTS:
		enqueue_external_timestamp(&ptp->tsevq, event);
		wake_up_interruptible(&ptp->tsev_wq);
		break;

	case PTP_CLOCK_PPS:
		pps_get_ts(&evt);
		pps_event(ptp->pps_source, &evt, PTP_PPS_EVENT, NULL);
		break;

	case PTP_CLOCK_PPSUSR:
		pps_event(ptp->pps_source, &event->pps_times,
			  PTP_PPS_EVENT, NULL);
		break;
	}
}
EXPORT_SYMBOL(ptp_clock_event);

int ptp_clock_index(struct ptp_clock *ptp)
{
	return ptp->index;
}
EXPORT_SYMBOL(ptp_clock_index);

int ptp_find_pin(struct ptp_clock *ptp,
		 enum ptp_pin_function func, unsigned int chan)
{
	struct ptp_pin_desc *pin = NULL;
	int i;

<<<<<<< HEAD
	mutex_lock(&ptp->pincfg_mux);
=======
>>>>>>> upstream/android-13
	for (i = 0; i < ptp->info->n_pins; i++) {
		if (ptp->info->pin_config[i].func == func &&
		    ptp->info->pin_config[i].chan == chan) {
			pin = &ptp->info->pin_config[i];
			break;
		}
	}
<<<<<<< HEAD
	mutex_unlock(&ptp->pincfg_mux);
=======
>>>>>>> upstream/android-13

	return pin ? i : -1;
}
EXPORT_SYMBOL(ptp_find_pin);

<<<<<<< HEAD
=======
int ptp_find_pin_unlocked(struct ptp_clock *ptp,
			  enum ptp_pin_function func, unsigned int chan)
{
	int result;

	mutex_lock(&ptp->pincfg_mux);

	result = ptp_find_pin(ptp, func, chan);

	mutex_unlock(&ptp->pincfg_mux);

	return result;
}
EXPORT_SYMBOL(ptp_find_pin_unlocked);

>>>>>>> upstream/android-13
int ptp_schedule_worker(struct ptp_clock *ptp, unsigned long delay)
{
	return kthread_mod_delayed_work(ptp->kworker, &ptp->aux_work, delay);
}
EXPORT_SYMBOL(ptp_schedule_worker);

<<<<<<< HEAD
=======
void ptp_cancel_worker_sync(struct ptp_clock *ptp)
{
	kthread_cancel_delayed_work_sync(&ptp->aux_work);
}
EXPORT_SYMBOL(ptp_cancel_worker_sync);

>>>>>>> upstream/android-13
/* module operations */

static void __exit ptp_exit(void)
{
	class_destroy(ptp_class);
	unregister_chrdev_region(ptp_devt, MINORMASK + 1);
	ida_destroy(&ptp_clocks_map);
}

static int __init ptp_init(void)
{
	int err;

	ptp_class = class_create(THIS_MODULE, "ptp");
	if (IS_ERR(ptp_class)) {
		pr_err("ptp: failed to allocate class\n");
		return PTR_ERR(ptp_class);
	}

	err = alloc_chrdev_region(&ptp_devt, 0, MINORMASK + 1, "ptp");
	if (err < 0) {
		pr_err("ptp: failed to allocate device region\n");
		goto no_region;
	}

	ptp_class->dev_groups = ptp_groups;
	pr_info("PTP clock support registered\n");
	return 0;

no_region:
	class_destroy(ptp_class);
	return err;
}

subsys_initcall(ptp_init);
module_exit(ptp_exit);

MODULE_AUTHOR("Richard Cochran <richardcochran@gmail.com>");
MODULE_DESCRIPTION("PTP clocks support");
MODULE_LICENSE("GPL");
