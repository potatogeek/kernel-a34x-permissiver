<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
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

#ifndef _PTP_CLOCK_KERNEL_H_
#define _PTP_CLOCK_KERNEL_H_

#include <linux/device.h>
#include <linux/pps_kernel.h>
#include <linux/ptp_clock.h>
<<<<<<< HEAD

=======
#include <linux/timecounter.h>
#include <linux/skbuff.h>

#define PTP_CLOCK_NAME_LEN	32
/**
 * struct ptp_clock_request - request PTP clock event
 *
 * @type:   The type of the request.
 *	    EXTTS:  Configure external trigger timestamping
 *	    PEROUT: Configure periodic output signal (e.g. PPS)
 *	    PPS:    trigger internal PPS event for input
 *	            into kernel PPS subsystem
 * @extts:  describes configuration for external trigger timestamping.
 *          This is only valid when event == PTP_CLK_REQ_EXTTS.
 * @perout: describes configuration for periodic output.
 *	    This is only valid when event == PTP_CLK_REQ_PEROUT.
 */
>>>>>>> upstream/android-13

struct ptp_clock_request {
	enum {
		PTP_CLK_REQ_EXTTS,
		PTP_CLK_REQ_PEROUT,
		PTP_CLK_REQ_PPS,
	} type;
	union {
		struct ptp_extts_request extts;
		struct ptp_perout_request perout;
	};
};

struct system_device_crosststamp;
<<<<<<< HEAD
/**
 * struct ptp_clock_info - decribes a PTP hardware clock
=======

/**
 * struct ptp_system_timestamp - system time corresponding to a PHC timestamp
 */
struct ptp_system_timestamp {
	struct timespec64 pre_ts;
	struct timespec64 post_ts;
};

/**
 * struct ptp_clock_info - describes a PTP hardware clock
>>>>>>> upstream/android-13
 *
 * @owner:     The clock driver should set to THIS_MODULE.
 * @name:      A short "friendly name" to identify the clock and to
 *             help distinguish PHY based devices from MAC based ones.
 *             The string is not meant to be a unique id.
 * @max_adj:   The maximum possible frequency adjustment, in parts per billon.
 * @n_alarm:   The number of programmable alarms.
 * @n_ext_ts:  The number of external time stamp channels.
 * @n_per_out: The number of programmable periodic signals.
 * @n_pins:    The number of programmable pins.
 * @pps:       Indicates whether the clock supports a PPS callback.
 * @pin_config: Array of length 'n_pins'. If the number of
 *              programmable pins is nonzero, then drivers must
 *              allocate and initialize this array.
 *
 * clock operations
 *
 * @adjfine:  Adjusts the frequency of the hardware clock.
 *            parameter scaled_ppm: Desired frequency offset from
 *            nominal frequency in parts per million, but with a
 *            16 bit binary fractional field.
 *
 * @adjfreq:  Adjusts the frequency of the hardware clock.
 *            This method is deprecated.  New drivers should implement
 *            the @adjfine method instead.
 *            parameter delta: Desired frequency offset from nominal frequency
 *            in parts per billion
 *
<<<<<<< HEAD
=======
 * @adjphase:  Adjusts the phase offset of the hardware clock.
 *             parameter delta: Desired change in nanoseconds.
 *
>>>>>>> upstream/android-13
 * @adjtime:  Shifts the time of the hardware clock.
 *            parameter delta: Desired change in nanoseconds.
 *
 * @gettime64:  Reads the current time from the hardware clock.
<<<<<<< HEAD
 *              parameter ts: Holds the result.
 *
=======
 *              This method is deprecated.  New drivers should implement
 *              the @gettimex64 method instead.
 *              parameter ts: Holds the result.
 *
 * @gettimex64:  Reads the current time from the hardware clock and optionally
 *               also the system clock.
 *               parameter ts: Holds the PHC timestamp.
 *               parameter sts: If not NULL, it holds a pair of timestamps from
 *               the system clock. The first reading is made right before
 *               reading the lowest bits of the PHC timestamp and the second
 *               reading immediately follows that.
 *
>>>>>>> upstream/android-13
 * @getcrosststamp:  Reads the current time from the hardware clock and
 *                   system clock simultaneously.
 *                   parameter cts: Contains timestamp (device,system) pair,
 *                   where system time is realtime and monotonic.
 *
 * @settime64:  Set the current time on the hardware clock.
 *              parameter ts: Time value to set.
 *
 * @enable:   Request driver to enable or disable an ancillary feature.
 *            parameter request: Desired resource to enable or disable.
 *            parameter on: Caller passes one to enable or zero to disable.
 *
 * @verify:   Confirm that a pin can perform a given function. The PTP
 *            Hardware Clock subsystem maintains the 'pin_config'
 *            array on behalf of the drivers, but the PHC subsystem
 *            assumes that every pin can perform every function. This
 *            hook gives drivers a way of telling the core about
 *            limitations on specific pins. This function must return
 *            zero if the function can be assigned to this pin, and
 *            nonzero otherwise.
 *            parameter pin: index of the pin in question.
 *            parameter func: the desired function to use.
 *            parameter chan: the function channel index to use.
 *
<<<<<<< HEAD
 * @do_work:  Request driver to perform auxiliary (periodic) operations
 *	      Driver should return delay of the next auxiliary work scheduling
 *	      time (>=0) or negative value in case further scheduling
 *	      is not required.
=======
 * @do_aux_work:  Request driver to perform auxiliary (periodic) operations
 *                Driver should return delay of the next auxiliary work
 *                scheduling time (>=0) or negative value in case further
 *                scheduling is not required.
>>>>>>> upstream/android-13
 *
 * Drivers should embed their ptp_clock_info within a private
 * structure, obtaining a reference to it using container_of().
 *
 * The callbacks must all return zero on success, non-zero otherwise.
 */

struct ptp_clock_info {
	struct module *owner;
<<<<<<< HEAD
	char name[16];
=======
	char name[PTP_CLOCK_NAME_LEN];
>>>>>>> upstream/android-13
	s32 max_adj;
	int n_alarm;
	int n_ext_ts;
	int n_per_out;
	int n_pins;
	int pps;
	struct ptp_pin_desc *pin_config;
	int (*adjfine)(struct ptp_clock_info *ptp, long scaled_ppm);
	int (*adjfreq)(struct ptp_clock_info *ptp, s32 delta);
<<<<<<< HEAD
	int (*adjtime)(struct ptp_clock_info *ptp, s64 delta);
	int (*gettime64)(struct ptp_clock_info *ptp, struct timespec64 *ts);
=======
	int (*adjphase)(struct ptp_clock_info *ptp, s32 phase);
	int (*adjtime)(struct ptp_clock_info *ptp, s64 delta);
	int (*gettime64)(struct ptp_clock_info *ptp, struct timespec64 *ts);
	int (*gettimex64)(struct ptp_clock_info *ptp, struct timespec64 *ts,
			  struct ptp_system_timestamp *sts);
>>>>>>> upstream/android-13
	int (*getcrosststamp)(struct ptp_clock_info *ptp,
			      struct system_device_crosststamp *cts);
	int (*settime64)(struct ptp_clock_info *p, const struct timespec64 *ts);
	int (*enable)(struct ptp_clock_info *ptp,
		      struct ptp_clock_request *request, int on);
	int (*verify)(struct ptp_clock_info *ptp, unsigned int pin,
		      enum ptp_pin_function func, unsigned int chan);
	long (*do_aux_work)(struct ptp_clock_info *ptp);
};

struct ptp_clock;

enum ptp_clock_events {
	PTP_CLOCK_ALARM,
	PTP_CLOCK_EXTTS,
	PTP_CLOCK_PPS,
	PTP_CLOCK_PPSUSR,
};

/**
 * struct ptp_clock_event - decribes a PTP hardware clock event
 *
 * @type:  One of the ptp_clock_events enumeration values.
 * @index: Identifies the source of the event.
 * @timestamp: When the event occurred (%PTP_CLOCK_EXTTS only).
 * @pps_times: When the event occurred (%PTP_CLOCK_PPSUSR only).
 */

struct ptp_clock_event {
	int type;
	int index;
	union {
		u64 timestamp;
		struct pps_event_time pps_times;
	};
};

<<<<<<< HEAD
#if IS_REACHABLE(CONFIG_PTP_1588_CLOCK)
=======
/**
 * scaled_ppm_to_ppb() - convert scaled ppm to ppb
 *
 * @ppm:    Parts per million, but with a 16 bit binary fractional field
 */
static inline long scaled_ppm_to_ppb(long ppm)
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
	return (long)ppb;
}

#if IS_ENABLED(CONFIG_PTP_1588_CLOCK)
>>>>>>> upstream/android-13

/**
 * ptp_clock_register() - register a PTP hardware clock driver
 *
 * @info:   Structure describing the new clock.
 * @parent: Pointer to the parent device of the new clock.
 *
 * Returns a valid pointer on success or PTR_ERR on failure.  If PHC
 * support is missing at the configuration level, this function
 * returns NULL, and drivers are expected to gracefully handle that
 * case separately.
 */

extern struct ptp_clock *ptp_clock_register(struct ptp_clock_info *info,
					    struct device *parent);

/**
 * ptp_clock_unregister() - unregister a PTP hardware clock driver
 *
 * @ptp:  The clock to remove from service.
 */

extern int ptp_clock_unregister(struct ptp_clock *ptp);

/**
 * ptp_clock_event() - notify the PTP layer about an event
 *
 * @ptp:    The clock obtained from ptp_clock_register().
 * @event:  Message structure describing the event.
 */

extern void ptp_clock_event(struct ptp_clock *ptp,
			    struct ptp_clock_event *event);

/**
 * ptp_clock_index() - obtain the device index of a PTP clock
 *
 * @ptp:    The clock obtained from ptp_clock_register().
 */

extern int ptp_clock_index(struct ptp_clock *ptp);

/**
 * ptp_find_pin() - obtain the pin index of a given auxiliary function
 *
<<<<<<< HEAD
=======
 * The caller must hold ptp_clock::pincfg_mux.  Drivers do not have
 * access to that mutex as ptp_clock is an opaque type.  However, the
 * core code acquires the mutex before invoking the driver's
 * ptp_clock_info::enable() callback, and so drivers may call this
 * function from that context.
 *
>>>>>>> upstream/android-13
 * @ptp:    The clock obtained from ptp_clock_register().
 * @func:   One of the ptp_pin_function enumerated values.
 * @chan:   The particular functional channel to find.
 * Return:  Pin index in the range of zero to ptp_clock_caps.n_pins - 1,
 *          or -1 if the auxiliary function cannot be found.
 */

int ptp_find_pin(struct ptp_clock *ptp,
		 enum ptp_pin_function func, unsigned int chan);

/**
<<<<<<< HEAD
=======
 * ptp_find_pin_unlocked() - wrapper for ptp_find_pin()
 *
 * This function acquires the ptp_clock::pincfg_mux mutex before
 * invoking ptp_find_pin().  Instead of using this function, drivers
 * should most likely call ptp_find_pin() directly from their
 * ptp_clock_info::enable() method.
 *
 */

int ptp_find_pin_unlocked(struct ptp_clock *ptp,
			  enum ptp_pin_function func, unsigned int chan);

/**
>>>>>>> upstream/android-13
 * ptp_schedule_worker() - schedule ptp auxiliary work
 *
 * @ptp:    The clock obtained from ptp_clock_register().
 * @delay:  number of jiffies to wait before queuing
 *          See kthread_queue_delayed_work() for more info.
 */

int ptp_schedule_worker(struct ptp_clock *ptp, unsigned long delay);

<<<<<<< HEAD
=======
/**
 * ptp_cancel_worker_sync() - cancel ptp auxiliary clock
 *
 * @ptp:     The clock obtained from ptp_clock_register().
 */
void ptp_cancel_worker_sync(struct ptp_clock *ptp);

>>>>>>> upstream/android-13
#else
static inline struct ptp_clock *ptp_clock_register(struct ptp_clock_info *info,
						   struct device *parent)
{ return NULL; }
static inline int ptp_clock_unregister(struct ptp_clock *ptp)
{ return 0; }
static inline void ptp_clock_event(struct ptp_clock *ptp,
				   struct ptp_clock_event *event)
{ }
static inline int ptp_clock_index(struct ptp_clock *ptp)
{ return -1; }
static inline int ptp_find_pin(struct ptp_clock *ptp,
			       enum ptp_pin_function func, unsigned int chan)
{ return -1; }
static inline int ptp_schedule_worker(struct ptp_clock *ptp,
				      unsigned long delay)
{ return -EOPNOTSUPP; }
<<<<<<< HEAD

#endif

=======
static inline void ptp_cancel_worker_sync(struct ptp_clock *ptp)
{ }
#endif

#if IS_BUILTIN(CONFIG_PTP_1588_CLOCK)
/*
 * These are called by the network core, and don't work if PTP is in
 * a loadable module.
 */

/**
 * ptp_get_vclocks_index() - get all vclocks index on pclock, and
 *                           caller is responsible to free memory
 *                           of vclock_index
 *
 * @pclock_index: phc index of ptp pclock.
 * @vclock_index: pointer to pointer of vclock index.
 *
 * return number of vclocks.
 */
int ptp_get_vclocks_index(int pclock_index, int **vclock_index);

/**
 * ptp_convert_timestamp() - convert timestamp to a ptp vclock time
 *
 * @hwtstamps:    skb_shared_hwtstamps structure pointer
 * @vclock_index: phc index of ptp vclock.
 *
 * Returns converted timestamp, or 0 on error.
 */
ktime_t ptp_convert_timestamp(const struct skb_shared_hwtstamps *hwtstamps,
			      int vclock_index);
#else
static inline int ptp_get_vclocks_index(int pclock_index, int **vclock_index)
{ return 0; }
static inline ktime_t ptp_convert_timestamp(const struct skb_shared_hwtstamps *hwtstamps,
					    int vclock_index)
{ return 0; }

#endif

static inline void ptp_read_system_prets(struct ptp_system_timestamp *sts)
{
	if (sts)
		ktime_get_real_ts64(&sts->pre_ts);
}

static inline void ptp_read_system_postts(struct ptp_system_timestamp *sts)
{
	if (sts)
		ktime_get_real_ts64(&sts->post_ts);
}

>>>>>>> upstream/android-13
#endif
