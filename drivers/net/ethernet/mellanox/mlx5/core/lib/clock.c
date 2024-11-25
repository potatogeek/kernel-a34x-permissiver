/*
 * Copyright (c) 2015, Mellanox Technologies. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <linux/clocksource.h>
#include <linux/highmem.h>
<<<<<<< HEAD
#include <rdma/mlx5-abi.h>
=======
#include <linux/ptp_clock_kernel.h>
#include <rdma/mlx5-abi.h>
#include "lib/eq.h"
>>>>>>> upstream/android-13
#include "en.h"
#include "clock.h"

enum {
	MLX5_CYCLES_SHIFT	= 23
};

enum {
	MLX5_PIN_MODE_IN		= 0x0,
	MLX5_PIN_MODE_OUT		= 0x1,
};

enum {
	MLX5_OUT_PATTERN_PULSE		= 0x0,
	MLX5_OUT_PATTERN_PERIODIC	= 0x1,
};

enum {
	MLX5_EVENT_MODE_DISABLE	= 0x0,
	MLX5_EVENT_MODE_REPETETIVE	= 0x1,
	MLX5_EVENT_MODE_ONCE_TILL_ARM	= 0x2,
};

enum {
	MLX5_MTPPS_FS_ENABLE			= BIT(0x0),
	MLX5_MTPPS_FS_PATTERN			= BIT(0x2),
	MLX5_MTPPS_FS_PIN_MODE			= BIT(0x3),
	MLX5_MTPPS_FS_TIME_STAMP		= BIT(0x4),
	MLX5_MTPPS_FS_OUT_PULSE_DURATION	= BIT(0x5),
	MLX5_MTPPS_FS_ENH_OUT_PER_ADJ		= BIT(0x7),
};

<<<<<<< HEAD
static u64 read_internal_timer(const struct cyclecounter *cc)
{
	struct mlx5_clock *clock = container_of(cc, struct mlx5_clock, cycles);
	struct mlx5_core_dev *mdev = container_of(clock, struct mlx5_core_dev,
						  clock);

	return mlx5_read_internal_timer(mdev) & cc->mask;
=======
static bool mlx5_real_time_mode(struct mlx5_core_dev *mdev)
{
	return (mlx5_is_real_time_rq(mdev) || mlx5_is_real_time_sq(mdev));
}

static bool mlx5_modify_mtutc_allowed(struct mlx5_core_dev *mdev)
{
	return MLX5_CAP_MCAM_FEATURE(mdev, ptpcyc2realtime_modify);
}

static int mlx5_set_mtutc(struct mlx5_core_dev *dev, u32 *mtutc, u32 size)
{
	u32 out[MLX5_ST_SZ_DW(mtutc_reg)] = {};

	if (!MLX5_CAP_MCAM_REG(dev, mtutc))
		return -EOPNOTSUPP;

	return mlx5_core_access_reg(dev, mtutc, size, out, sizeof(out),
				    MLX5_REG_MTUTC, 0, 1);
}

static u64 mlx5_read_time(struct mlx5_core_dev *dev,
			  struct ptp_system_timestamp *sts,
			  bool real_time)
{
	u32 timer_h, timer_h1, timer_l;

	timer_h = ioread32be(real_time ? &dev->iseg->real_time_h :
			     &dev->iseg->internal_timer_h);
	ptp_read_system_prets(sts);
	timer_l = ioread32be(real_time ? &dev->iseg->real_time_l :
			     &dev->iseg->internal_timer_l);
	ptp_read_system_postts(sts);
	timer_h1 = ioread32be(real_time ? &dev->iseg->real_time_h :
			      &dev->iseg->internal_timer_h);
	if (timer_h != timer_h1) {
		/* wrap around */
		ptp_read_system_prets(sts);
		timer_l = ioread32be(real_time ? &dev->iseg->real_time_l :
				     &dev->iseg->internal_timer_l);
		ptp_read_system_postts(sts);
	}

	return real_time ? REAL_TIME_TO_NS(timer_h1, timer_l) :
			   (u64)timer_l | (u64)timer_h1 << 32;
}

static u64 read_internal_timer(const struct cyclecounter *cc)
{
	struct mlx5_timer *timer = container_of(cc, struct mlx5_timer, cycles);
	struct mlx5_clock *clock = container_of(timer, struct mlx5_clock, timer);
	struct mlx5_core_dev *mdev = container_of(clock, struct mlx5_core_dev,
						  clock);

	return mlx5_read_time(mdev, NULL, false) & cc->mask;
>>>>>>> upstream/android-13
}

static void mlx5_update_clock_info_page(struct mlx5_core_dev *mdev)
{
	struct mlx5_ib_clock_info *clock_info = mdev->clock_info;
	struct mlx5_clock *clock = &mdev->clock;
<<<<<<< HEAD
=======
	struct mlx5_timer *timer;
>>>>>>> upstream/android-13
	u32 sign;

	if (!clock_info)
		return;

	sign = smp_load_acquire(&clock_info->sign);
	smp_store_mb(clock_info->sign,
		     sign | MLX5_IB_CLOCK_INFO_KERNEL_UPDATING);

<<<<<<< HEAD
	clock_info->cycles = clock->tc.cycle_last;
	clock_info->mult   = clock->cycles.mult;
	clock_info->nsec   = clock->tc.nsec;
	clock_info->frac   = clock->tc.frac;
=======
	timer = &clock->timer;
	clock_info->cycles = timer->tc.cycle_last;
	clock_info->mult   = timer->cycles.mult;
	clock_info->nsec   = timer->tc.nsec;
	clock_info->frac   = timer->tc.frac;
>>>>>>> upstream/android-13

	smp_store_release(&clock_info->sign,
			  sign + MLX5_IB_CLOCK_INFO_KERNEL_UPDATING * 2);
}

static void mlx5_pps_out(struct work_struct *work)
{
	struct mlx5_pps *pps_info = container_of(work, struct mlx5_pps,
						 out_work);
	struct mlx5_clock *clock = container_of(pps_info, struct mlx5_clock,
						pps_info);
	struct mlx5_core_dev *mdev = container_of(clock, struct mlx5_core_dev,
						  clock);
	u32 in[MLX5_ST_SZ_DW(mtpps_reg)] = {0};
	unsigned long flags;
	int i;

	for (i = 0; i < clock->ptp_info.n_pins; i++) {
		u64 tstart;

<<<<<<< HEAD
		write_lock_irqsave(&clock->lock, flags);
		tstart = clock->pps_info.start[i];
		clock->pps_info.start[i] = 0;
		write_unlock_irqrestore(&clock->lock, flags);
=======
		write_seqlock_irqsave(&clock->lock, flags);
		tstart = clock->pps_info.start[i];
		clock->pps_info.start[i] = 0;
		write_sequnlock_irqrestore(&clock->lock, flags);
>>>>>>> upstream/android-13
		if (!tstart)
			continue;

		MLX5_SET(mtpps_reg, in, pin, i);
		MLX5_SET64(mtpps_reg, in, time_stamp, tstart);
		MLX5_SET(mtpps_reg, in, field_select, MLX5_MTPPS_FS_TIME_STAMP);
		mlx5_set_mtpps(mdev, in, sizeof(in));
	}
}

static void mlx5_timestamp_overflow(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
<<<<<<< HEAD
	struct mlx5_clock *clock = container_of(dwork, struct mlx5_clock,
						overflow_work);
	unsigned long flags;

	write_lock_irqsave(&clock->lock, flags);
	timecounter_read(&clock->tc);
	mlx5_update_clock_info_page(clock->mdev);
	write_unlock_irqrestore(&clock->lock, flags);
	schedule_delayed_work(&clock->overflow_work, clock->overflow_period);
}

static int mlx5_ptp_settime(struct ptp_clock_info *ptp,
			    const struct timespec64 *ts)
{
	struct mlx5_clock *clock = container_of(ptp, struct mlx5_clock,
						 ptp_info);
	u64 ns = timespec64_to_ns(ts);
	unsigned long flags;

	write_lock_irqsave(&clock->lock, flags);
	timecounter_init(&clock->tc, &clock->cycles, ns);
	mlx5_update_clock_info_page(clock->mdev);
	write_unlock_irqrestore(&clock->lock, flags);
=======
	struct mlx5_core_dev *mdev;
	struct mlx5_timer *timer;
	struct mlx5_clock *clock;
	unsigned long flags;

	timer = container_of(dwork, struct mlx5_timer, overflow_work);
	clock = container_of(timer, struct mlx5_clock, timer);
	mdev = container_of(clock, struct mlx5_core_dev, clock);

	write_seqlock_irqsave(&clock->lock, flags);
	timecounter_read(&timer->tc);
	mlx5_update_clock_info_page(mdev);
	write_sequnlock_irqrestore(&clock->lock, flags);
	schedule_delayed_work(&timer->overflow_work, timer->overflow_period);
}

static int mlx5_ptp_settime_real_time(struct mlx5_core_dev *mdev,
				      const struct timespec64 *ts)
{
	u32 in[MLX5_ST_SZ_DW(mtutc_reg)] = {};

	if (!mlx5_modify_mtutc_allowed(mdev))
		return 0;

	if (ts->tv_sec < 0 || ts->tv_sec > U32_MAX ||
	    ts->tv_nsec < 0 || ts->tv_nsec > NSEC_PER_SEC)
		return -EINVAL;

	MLX5_SET(mtutc_reg, in, operation, MLX5_MTUTC_OPERATION_SET_TIME_IMMEDIATE);
	MLX5_SET(mtutc_reg, in, utc_sec, ts->tv_sec);
	MLX5_SET(mtutc_reg, in, utc_nsec, ts->tv_nsec);

	return mlx5_set_mtutc(mdev, in, sizeof(in));
}

static int mlx5_ptp_settime(struct ptp_clock_info *ptp, const struct timespec64 *ts)
{
	struct mlx5_clock *clock = container_of(ptp, struct mlx5_clock, ptp_info);
	struct mlx5_timer *timer = &clock->timer;
	struct mlx5_core_dev *mdev;
	unsigned long flags;
	int err;

	mdev = container_of(clock, struct mlx5_core_dev, clock);
	err = mlx5_ptp_settime_real_time(mdev, ts);
	if (err)
		return err;

	write_seqlock_irqsave(&clock->lock, flags);
	timecounter_init(&timer->tc, &timer->cycles, timespec64_to_ns(ts));
	mlx5_update_clock_info_page(mdev);
	write_sequnlock_irqrestore(&clock->lock, flags);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int mlx5_ptp_gettime(struct ptp_clock_info *ptp, struct timespec64 *ts)
{
	struct mlx5_clock *clock = container_of(ptp, struct mlx5_clock,
						ptp_info);
	u64 ns;
	unsigned long flags;

	write_lock_irqsave(&clock->lock, flags);
	ns = timecounter_read(&clock->tc);
	write_unlock_irqrestore(&clock->lock, flags);

	*ts = ns_to_timespec64(ns);

	return 0;
}

static int mlx5_ptp_adjtime(struct ptp_clock_info *ptp, s64 delta)
{
	struct mlx5_clock *clock = container_of(ptp, struct mlx5_clock,
						ptp_info);
	unsigned long flags;

	write_lock_irqsave(&clock->lock, flags);
	timecounter_adjtime(&clock->tc, delta);
	mlx5_update_clock_info_page(clock->mdev);
	write_unlock_irqrestore(&clock->lock, flags);
=======
static
struct timespec64 mlx5_ptp_gettimex_real_time(struct mlx5_core_dev *mdev,
					      struct ptp_system_timestamp *sts)
{
	struct timespec64 ts;
	u64 time;

	time = mlx5_read_time(mdev, sts, true);
	ts = ns_to_timespec64(time);
	return ts;
}

static int mlx5_ptp_gettimex(struct ptp_clock_info *ptp, struct timespec64 *ts,
			     struct ptp_system_timestamp *sts)
{
	struct mlx5_clock *clock = container_of(ptp, struct mlx5_clock, ptp_info);
	struct mlx5_timer *timer = &clock->timer;
	struct mlx5_core_dev *mdev;
	unsigned long flags;
	u64 cycles, ns;

	mdev = container_of(clock, struct mlx5_core_dev, clock);
	if (mlx5_real_time_mode(mdev)) {
		*ts = mlx5_ptp_gettimex_real_time(mdev, sts);
		goto out;
	}

	write_seqlock_irqsave(&clock->lock, flags);
	cycles = mlx5_read_time(mdev, sts, false);
	ns = timecounter_cyc2time(&timer->tc, cycles);
	write_sequnlock_irqrestore(&clock->lock, flags);
	*ts = ns_to_timespec64(ns);
out:
	return 0;
}

static int mlx5_ptp_adjtime_real_time(struct mlx5_core_dev *mdev, s64 delta)
{
	u32 in[MLX5_ST_SZ_DW(mtutc_reg)] = {};

	if (!mlx5_modify_mtutc_allowed(mdev))
		return 0;

	/* HW time adjustment range is s16. If out of range, settime instead */
	if (delta < S16_MIN || delta > S16_MAX) {
		struct timespec64 ts;
		s64 ns;

		ts = mlx5_ptp_gettimex_real_time(mdev, NULL);
		ns = timespec64_to_ns(&ts) + delta;
		ts = ns_to_timespec64(ns);
		return mlx5_ptp_settime_real_time(mdev, &ts);
	}

	MLX5_SET(mtutc_reg, in, operation, MLX5_MTUTC_OPERATION_ADJUST_TIME);
	MLX5_SET(mtutc_reg, in, time_adjustment, delta);

	return mlx5_set_mtutc(mdev, in, sizeof(in));
}

static int mlx5_ptp_adjtime(struct ptp_clock_info *ptp, s64 delta)
{
	struct mlx5_clock *clock = container_of(ptp, struct mlx5_clock, ptp_info);
	struct mlx5_timer *timer = &clock->timer;
	struct mlx5_core_dev *mdev;
	unsigned long flags;
	int err;

	mdev = container_of(clock, struct mlx5_core_dev, clock);

	err = mlx5_ptp_adjtime_real_time(mdev, delta);
	if (err)
		return err;
	write_seqlock_irqsave(&clock->lock, flags);
	timecounter_adjtime(&timer->tc, delta);
	mlx5_update_clock_info_page(mdev);
	write_sequnlock_irqrestore(&clock->lock, flags);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int mlx5_ptp_adjfreq(struct ptp_clock_info *ptp, s32 delta)
{
	u64 adj;
	u32 diff;
	unsigned long flags;
	int neg_adj = 0;
	struct mlx5_clock *clock = container_of(ptp, struct mlx5_clock,
						ptp_info);
=======
static int mlx5_ptp_adjfreq_real_time(struct mlx5_core_dev *mdev, s32 freq)
{
	u32 in[MLX5_ST_SZ_DW(mtutc_reg)] = {};

	if (!mlx5_modify_mtutc_allowed(mdev))
		return 0;

	MLX5_SET(mtutc_reg, in, operation, MLX5_MTUTC_OPERATION_ADJUST_FREQ_UTC);
	MLX5_SET(mtutc_reg, in, freq_adjustment, freq);

	return mlx5_set_mtutc(mdev, in, sizeof(in));
}

static int mlx5_ptp_adjfreq(struct ptp_clock_info *ptp, s32 delta)
{
	struct mlx5_clock *clock = container_of(ptp, struct mlx5_clock, ptp_info);
	struct mlx5_timer *timer = &clock->timer;
	struct mlx5_core_dev *mdev;
	unsigned long flags;
	int neg_adj = 0;
	u32 diff;
	u64 adj;
	int err;

	mdev = container_of(clock, struct mlx5_core_dev, clock);
	err = mlx5_ptp_adjfreq_real_time(mdev, delta);
	if (err)
		return err;
>>>>>>> upstream/android-13

	if (delta < 0) {
		neg_adj = 1;
		delta = -delta;
	}

<<<<<<< HEAD
	adj = clock->nominal_c_mult;
	adj *= delta;
	diff = div_u64(adj, 1000000000ULL);

	write_lock_irqsave(&clock->lock, flags);
	timecounter_read(&clock->tc);
	clock->cycles.mult = neg_adj ? clock->nominal_c_mult - diff :
				       clock->nominal_c_mult + diff;
	mlx5_update_clock_info_page(clock->mdev);
	write_unlock_irqrestore(&clock->lock, flags);
=======
	adj = timer->nominal_c_mult;
	adj *= delta;
	diff = div_u64(adj, 1000000000ULL);

	write_seqlock_irqsave(&clock->lock, flags);
	timecounter_read(&timer->tc);
	timer->cycles.mult = neg_adj ? timer->nominal_c_mult - diff :
				       timer->nominal_c_mult + diff;
	mlx5_update_clock_info_page(mdev);
	write_sequnlock_irqrestore(&clock->lock, flags);
>>>>>>> upstream/android-13

	return 0;
}

static int mlx5_extts_configure(struct ptp_clock_info *ptp,
				struct ptp_clock_request *rq,
				int on)
{
	struct mlx5_clock *clock =
			container_of(ptp, struct mlx5_clock, ptp_info);
	struct mlx5_core_dev *mdev =
			container_of(clock, struct mlx5_core_dev, clock);
	u32 in[MLX5_ST_SZ_DW(mtpps_reg)] = {0};
	u32 field_select = 0;
	u8 pin_mode = 0;
	u8 pattern = 0;
	int pin = -1;
	int err = 0;

	if (!MLX5_PPS_CAP(mdev))
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (rq->extts.index >= clock->ptp_info.n_pins)
		return -EINVAL;

	if (on) {
		pin = ptp_find_pin(clock->ptp, PTP_PF_EXTTS, rq->extts.index);
		if (pin < 0)
			return -EBUSY;
=======
	/* Reject requests with unsupported flags */
	if (rq->extts.flags & ~(PTP_ENABLE_FEATURE |
				PTP_RISING_EDGE |
				PTP_FALLING_EDGE |
				PTP_STRICT_FLAGS))
		return -EOPNOTSUPP;

	/* Reject requests to enable time stamping on both edges. */
	if ((rq->extts.flags & PTP_STRICT_FLAGS) &&
	    (rq->extts.flags & PTP_ENABLE_FEATURE) &&
	    (rq->extts.flags & PTP_EXTTS_EDGES) == PTP_EXTTS_EDGES)
		return -EOPNOTSUPP;

	if (rq->extts.index >= clock->ptp_info.n_pins)
		return -EINVAL;

	pin = ptp_find_pin(clock->ptp, PTP_PF_EXTTS, rq->extts.index);
	if (pin < 0)
		return -EBUSY;

	if (on) {
>>>>>>> upstream/android-13
		pin_mode = MLX5_PIN_MODE_IN;
		pattern = !!(rq->extts.flags & PTP_FALLING_EDGE);
		field_select = MLX5_MTPPS_FS_PIN_MODE |
			       MLX5_MTPPS_FS_PATTERN |
			       MLX5_MTPPS_FS_ENABLE;
	} else {
<<<<<<< HEAD
		pin = rq->extts.index;
=======
>>>>>>> upstream/android-13
		field_select = MLX5_MTPPS_FS_ENABLE;
	}

	MLX5_SET(mtpps_reg, in, pin, pin);
	MLX5_SET(mtpps_reg, in, pin_mode, pin_mode);
	MLX5_SET(mtpps_reg, in, pattern, pattern);
	MLX5_SET(mtpps_reg, in, enable, on);
	MLX5_SET(mtpps_reg, in, field_select, field_select);

	err = mlx5_set_mtpps(mdev, in, sizeof(in));
	if (err)
		return err;

	return mlx5_set_mtppse(mdev, pin, 0,
			       MLX5_EVENT_MODE_REPETETIVE & on);
}

<<<<<<< HEAD
=======
static u64 find_target_cycles(struct mlx5_core_dev *mdev, s64 target_ns)
{
	struct mlx5_clock *clock = &mdev->clock;
	u64 cycles_now, cycles_delta;
	u64 nsec_now, nsec_delta;
	struct mlx5_timer *timer;
	unsigned long flags;

	timer = &clock->timer;

	cycles_now = mlx5_read_time(mdev, NULL, false);
	write_seqlock_irqsave(&clock->lock, flags);
	nsec_now = timecounter_cyc2time(&timer->tc, cycles_now);
	nsec_delta = target_ns - nsec_now;
	cycles_delta = div64_u64(nsec_delta << timer->cycles.shift,
				 timer->cycles.mult);
	write_sequnlock_irqrestore(&clock->lock, flags);

	return cycles_now + cycles_delta;
}

static u64 perout_conf_internal_timer(struct mlx5_core_dev *mdev, s64 sec)
{
	struct timespec64 ts = {};
	s64 target_ns;

	ts.tv_sec = sec;
	target_ns = timespec64_to_ns(&ts);

	return find_target_cycles(mdev, target_ns);
}

static u64 perout_conf_real_time(s64 sec)
{
	return (u64)sec << 32;
}

>>>>>>> upstream/android-13
static int mlx5_perout_configure(struct ptp_clock_info *ptp,
				 struct ptp_clock_request *rq,
				 int on)
{
	struct mlx5_clock *clock =
			container_of(ptp, struct mlx5_clock, ptp_info);
	struct mlx5_core_dev *mdev =
			container_of(clock, struct mlx5_core_dev, clock);
<<<<<<< HEAD
	u32 in[MLX5_ST_SZ_DW(mtpps_reg)] = {0};
	u64 nsec_now, nsec_delta, time_stamp = 0;
	u64 cycles_now, cycles_delta;
	struct timespec64 ts;
	unsigned long flags;
	u32 field_select = 0;
=======
	bool rt_mode = mlx5_real_time_mode(mdev);
	u32 in[MLX5_ST_SZ_DW(mtpps_reg)] = {0};
	struct timespec64 ts;
	u32 field_select = 0;
	u64 time_stamp = 0;
>>>>>>> upstream/android-13
	u8 pin_mode = 0;
	u8 pattern = 0;
	int pin = -1;
	int err = 0;
	s64 ns;

	if (!MLX5_PPS_CAP(mdev))
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (rq->perout.index >= clock->ptp_info.n_pins)
		return -EINVAL;

	if (on) {
		pin = ptp_find_pin(clock->ptp, PTP_PF_PEROUT,
				   rq->perout.index);
		if (pin < 0)
			return -EBUSY;
=======
	/* Reject requests with unsupported flags */
	if (rq->perout.flags)
		return -EOPNOTSUPP;

	if (rq->perout.index >= clock->ptp_info.n_pins)
		return -EINVAL;

	field_select = MLX5_MTPPS_FS_ENABLE;
	pin = ptp_find_pin(clock->ptp, PTP_PF_PEROUT, rq->perout.index);
	if (pin < 0)
		return -EBUSY;

	if (on) {
		bool rt_mode = mlx5_real_time_mode(mdev);
		s64 sec = rq->perout.start.sec;

		if (rq->perout.start.nsec)
			return -EINVAL;
>>>>>>> upstream/android-13

		pin_mode = MLX5_PIN_MODE_OUT;
		pattern = MLX5_OUT_PATTERN_PERIODIC;
		ts.tv_sec = rq->perout.period.sec;
		ts.tv_nsec = rq->perout.period.nsec;
		ns = timespec64_to_ns(&ts);

		if ((ns >> 1) != 500000000LL)
			return -EINVAL;

<<<<<<< HEAD
		ts.tv_sec = rq->perout.start.sec;
		ts.tv_nsec = rq->perout.start.nsec;
		ns = timespec64_to_ns(&ts);
		cycles_now = mlx5_read_internal_timer(mdev);
		write_lock_irqsave(&clock->lock, flags);
		nsec_now = timecounter_cyc2time(&clock->tc, cycles_now);
		nsec_delta = ns - nsec_now;
		cycles_delta = div64_u64(nsec_delta << clock->cycles.shift,
					 clock->cycles.mult);
		write_unlock_irqrestore(&clock->lock, flags);
		time_stamp = cycles_now + cycles_delta;
		field_select = MLX5_MTPPS_FS_PIN_MODE |
			       MLX5_MTPPS_FS_PATTERN |
			       MLX5_MTPPS_FS_ENABLE |
			       MLX5_MTPPS_FS_TIME_STAMP;
	} else {
		pin = rq->perout.index;
		field_select = MLX5_MTPPS_FS_ENABLE;
=======
		if (rt_mode && sec > U32_MAX)
			return -EINVAL;

		time_stamp = rt_mode ? perout_conf_real_time(sec) :
				       perout_conf_internal_timer(mdev, sec);

		field_select |= MLX5_MTPPS_FS_PIN_MODE |
				MLX5_MTPPS_FS_PATTERN |
				MLX5_MTPPS_FS_TIME_STAMP;
>>>>>>> upstream/android-13
	}

	MLX5_SET(mtpps_reg, in, pin, pin);
	MLX5_SET(mtpps_reg, in, pin_mode, pin_mode);
	MLX5_SET(mtpps_reg, in, pattern, pattern);
	MLX5_SET(mtpps_reg, in, enable, on);
	MLX5_SET64(mtpps_reg, in, time_stamp, time_stamp);
	MLX5_SET(mtpps_reg, in, field_select, field_select);

	err = mlx5_set_mtpps(mdev, in, sizeof(in));
	if (err)
		return err;

<<<<<<< HEAD
=======
	if (rt_mode)
		return 0;

>>>>>>> upstream/android-13
	return mlx5_set_mtppse(mdev, pin, 0,
			       MLX5_EVENT_MODE_REPETETIVE & on);
}

static int mlx5_pps_configure(struct ptp_clock_info *ptp,
			      struct ptp_clock_request *rq,
			      int on)
{
	struct mlx5_clock *clock =
			container_of(ptp, struct mlx5_clock, ptp_info);

	clock->pps_info.enabled = !!on;
	return 0;
}

static int mlx5_ptp_enable(struct ptp_clock_info *ptp,
			   struct ptp_clock_request *rq,
			   int on)
{
	switch (rq->type) {
	case PTP_CLK_REQ_EXTTS:
		return mlx5_extts_configure(ptp, rq, on);
	case PTP_CLK_REQ_PEROUT:
		return mlx5_perout_configure(ptp, rq, on);
	case PTP_CLK_REQ_PPS:
		return mlx5_pps_configure(ptp, rq, on);
	default:
		return -EOPNOTSUPP;
	}
	return 0;
}

enum {
	MLX5_MTPPS_REG_CAP_PIN_X_MODE_SUPPORT_PPS_IN = BIT(0),
	MLX5_MTPPS_REG_CAP_PIN_X_MODE_SUPPORT_PPS_OUT = BIT(1),
};

static int mlx5_ptp_verify(struct ptp_clock_info *ptp, unsigned int pin,
			   enum ptp_pin_function func, unsigned int chan)
{
	struct mlx5_clock *clock = container_of(ptp, struct mlx5_clock,
						ptp_info);

	switch (func) {
	case PTP_PF_NONE:
		return 0;
	case PTP_PF_EXTTS:
		return !(clock->pps_info.pin_caps[pin] &
			 MLX5_MTPPS_REG_CAP_PIN_X_MODE_SUPPORT_PPS_IN);
	case PTP_PF_PEROUT:
		return !(clock->pps_info.pin_caps[pin] &
			 MLX5_MTPPS_REG_CAP_PIN_X_MODE_SUPPORT_PPS_OUT);
	default:
		return -EOPNOTSUPP;
	}
<<<<<<< HEAD

	return -EOPNOTSUPP;
=======
>>>>>>> upstream/android-13
}

static const struct ptp_clock_info mlx5_ptp_clock_info = {
	.owner		= THIS_MODULE,
<<<<<<< HEAD
	.name		= "mlx5_p2p",
=======
	.name		= "mlx5_ptp",
>>>>>>> upstream/android-13
	.max_adj	= 100000000,
	.n_alarm	= 0,
	.n_ext_ts	= 0,
	.n_per_out	= 0,
	.n_pins		= 0,
	.pps		= 0,
	.adjfreq	= mlx5_ptp_adjfreq,
	.adjtime	= mlx5_ptp_adjtime,
<<<<<<< HEAD
	.gettime64	= mlx5_ptp_gettime,
=======
	.gettimex64	= mlx5_ptp_gettimex,
>>>>>>> upstream/android-13
	.settime64	= mlx5_ptp_settime,
	.enable		= NULL,
	.verify		= NULL,
};

<<<<<<< HEAD
static int mlx5_init_pin_config(struct mlx5_clock *clock)
{
	int i;

=======
static int mlx5_query_mtpps_pin_mode(struct mlx5_core_dev *mdev, u8 pin,
				     u32 *mtpps, u32 mtpps_size)
{
	u32 in[MLX5_ST_SZ_DW(mtpps_reg)] = {};

	MLX5_SET(mtpps_reg, in, pin, pin);

	return mlx5_core_access_reg(mdev, in, sizeof(in), mtpps,
				    mtpps_size, MLX5_REG_MTPPS, 0, 0);
}

static int mlx5_get_pps_pin_mode(struct mlx5_clock *clock, u8 pin)
{
	struct mlx5_core_dev *mdev = container_of(clock, struct mlx5_core_dev, clock);

	u32 out[MLX5_ST_SZ_DW(mtpps_reg)] = {};
	u8 mode;
	int err;

	err = mlx5_query_mtpps_pin_mode(mdev, pin, out, sizeof(out));
	if (err || !MLX5_GET(mtpps_reg, out, enable))
		return PTP_PF_NONE;

	mode = MLX5_GET(mtpps_reg, out, pin_mode);

	if (mode == MLX5_PIN_MODE_IN)
		return PTP_PF_EXTTS;
	else if (mode == MLX5_PIN_MODE_OUT)
		return PTP_PF_PEROUT;

	return PTP_PF_NONE;
}

static void mlx5_init_pin_config(struct mlx5_clock *clock)
{
	int i;

	if (!clock->ptp_info.n_pins)
		return;

>>>>>>> upstream/android-13
	clock->ptp_info.pin_config =
			kcalloc(clock->ptp_info.n_pins,
				sizeof(*clock->ptp_info.pin_config),
				GFP_KERNEL);
	if (!clock->ptp_info.pin_config)
<<<<<<< HEAD
		return -ENOMEM;
=======
		return;
>>>>>>> upstream/android-13
	clock->ptp_info.enable = mlx5_ptp_enable;
	clock->ptp_info.verify = mlx5_ptp_verify;
	clock->ptp_info.pps = 1;

	for (i = 0; i < clock->ptp_info.n_pins; i++) {
		snprintf(clock->ptp_info.pin_config[i].name,
			 sizeof(clock->ptp_info.pin_config[i].name),
			 "mlx5_pps%d", i);
		clock->ptp_info.pin_config[i].index = i;
<<<<<<< HEAD
		clock->ptp_info.pin_config[i].func = PTP_PF_NONE;
		clock->ptp_info.pin_config[i].chan = i;
	}

	return 0;
=======
		clock->ptp_info.pin_config[i].func = mlx5_get_pps_pin_mode(clock, i);
		clock->ptp_info.pin_config[i].chan = 0;
	}
>>>>>>> upstream/android-13
}

static void mlx5_get_pps_caps(struct mlx5_core_dev *mdev)
{
	struct mlx5_clock *clock = &mdev->clock;
	u32 out[MLX5_ST_SZ_DW(mtpps_reg)] = {0};

	mlx5_query_mtpps(mdev, out, sizeof(out));

	clock->ptp_info.n_pins = MLX5_GET(mtpps_reg, out,
					  cap_number_of_pps_pins);
	clock->ptp_info.n_ext_ts = MLX5_GET(mtpps_reg, out,
					    cap_max_num_of_pps_in_pins);
	clock->ptp_info.n_per_out = MLX5_GET(mtpps_reg, out,
					     cap_max_num_of_pps_out_pins);

	clock->pps_info.pin_caps[0] = MLX5_GET(mtpps_reg, out, cap_pin_0_mode);
	clock->pps_info.pin_caps[1] = MLX5_GET(mtpps_reg, out, cap_pin_1_mode);
	clock->pps_info.pin_caps[2] = MLX5_GET(mtpps_reg, out, cap_pin_2_mode);
	clock->pps_info.pin_caps[3] = MLX5_GET(mtpps_reg, out, cap_pin_3_mode);
	clock->pps_info.pin_caps[4] = MLX5_GET(mtpps_reg, out, cap_pin_4_mode);
	clock->pps_info.pin_caps[5] = MLX5_GET(mtpps_reg, out, cap_pin_5_mode);
	clock->pps_info.pin_caps[6] = MLX5_GET(mtpps_reg, out, cap_pin_6_mode);
	clock->pps_info.pin_caps[7] = MLX5_GET(mtpps_reg, out, cap_pin_7_mode);
}

<<<<<<< HEAD
void mlx5_pps_event(struct mlx5_core_dev *mdev,
		    struct mlx5_eqe *eqe)
{
	struct mlx5_clock *clock = &mdev->clock;
	struct ptp_clock_event ptp_event;
	struct timespec64 ts;
	u64 nsec_now, nsec_delta;
	u64 cycles_now, cycles_delta;
	int pin = eqe->data.pps.pin;
	s64 ns;
	unsigned long flags;
=======
static void ts_next_sec(struct timespec64 *ts)
{
	ts->tv_sec += 1;
	ts->tv_nsec = 0;
}

static u64 perout_conf_next_event_timer(struct mlx5_core_dev *mdev,
					struct mlx5_clock *clock)
{
	struct timespec64 ts;
	s64 target_ns;

	mlx5_ptp_gettimex(&clock->ptp_info, &ts, NULL);
	ts_next_sec(&ts);
	target_ns = timespec64_to_ns(&ts);

	return find_target_cycles(mdev, target_ns);
}

static int mlx5_pps_event(struct notifier_block *nb,
			  unsigned long type, void *data)
{
	struct mlx5_clock *clock = mlx5_nb_cof(nb, struct mlx5_clock, pps_nb);
	struct ptp_clock_event ptp_event;
	struct mlx5_eqe *eqe = data;
	int pin = eqe->data.pps.pin;
	struct mlx5_core_dev *mdev;
	unsigned long flags;
	u64 ns;

	mdev = container_of(clock, struct mlx5_core_dev, clock);
>>>>>>> upstream/android-13

	switch (clock->ptp_info.pin_config[pin].func) {
	case PTP_PF_EXTTS:
		ptp_event.index = pin;
<<<<<<< HEAD
		ptp_event.timestamp =
=======
		ptp_event.timestamp = mlx5_real_time_mode(mdev) ?
			mlx5_real_time_cyc2time(clock,
						be64_to_cpu(eqe->data.pps.time_stamp)) :
>>>>>>> upstream/android-13
			mlx5_timecounter_cyc2time(clock,
						  be64_to_cpu(eqe->data.pps.time_stamp));
		if (clock->pps_info.enabled) {
			ptp_event.type = PTP_CLOCK_PPSUSR;
			ptp_event.pps_times.ts_real =
					ns_to_timespec64(ptp_event.timestamp);
		} else {
			ptp_event.type = PTP_CLOCK_EXTTS;
		}
<<<<<<< HEAD
		ptp_clock_event(clock->ptp, &ptp_event);
		break;
	case PTP_PF_PEROUT:
		mlx5_ptp_gettime(&clock->ptp_info, &ts);
		cycles_now = mlx5_read_internal_timer(mdev);
		ts.tv_sec += 1;
		ts.tv_nsec = 0;
		ns = timespec64_to_ns(&ts);
		write_lock_irqsave(&clock->lock, flags);
		nsec_now = timecounter_cyc2time(&clock->tc, cycles_now);
		nsec_delta = ns - nsec_now;
		cycles_delta = div64_u64(nsec_delta << clock->cycles.shift,
					 clock->cycles.mult);
		clock->pps_info.start[pin] = cycles_now + cycles_delta;
		schedule_work(&clock->pps_info.out_work);
		write_unlock_irqrestore(&clock->lock, flags);
		break;
	default:
		mlx5_core_err(mdev, " Unhandled event\n");
	}
}

void mlx5_init_clock(struct mlx5_core_dev *mdev)
{
	struct mlx5_clock *clock = &mdev->clock;
	u64 overflow_cycles;
	u64 ns;
	u64 frac = 0;
	u32 dev_freq;

	dev_freq = MLX5_CAP_GEN(mdev, device_frequency_khz);
	if (!dev_freq) {
		mlx5_core_warn(mdev, "invalid device_frequency_khz, aborting HW clock init\n");
		return;
	}
	rwlock_init(&clock->lock);
	clock->cycles.read = read_internal_timer;
	clock->cycles.shift = MLX5_CYCLES_SHIFT;
	clock->cycles.mult = clocksource_khz2mult(dev_freq,
						  clock->cycles.shift);
	clock->nominal_c_mult = clock->cycles.mult;
	clock->cycles.mask = CLOCKSOURCE_MASK(41);
	clock->mdev = mdev;

	timecounter_init(&clock->tc, &clock->cycles,
			 ktime_to_ns(ktime_get_real()));
=======
		/* TODOL clock->ptp can be NULL if ptp_clock_register fails */
		ptp_clock_event(clock->ptp, &ptp_event);
		break;
	case PTP_PF_PEROUT:
		ns = perout_conf_next_event_timer(mdev, clock);
		write_seqlock_irqsave(&clock->lock, flags);
		clock->pps_info.start[pin] = ns;
		write_sequnlock_irqrestore(&clock->lock, flags);
		schedule_work(&clock->pps_info.out_work);
		break;
	default:
		mlx5_core_err(mdev, " Unhandled clock PPS event, func %d\n",
			      clock->ptp_info.pin_config[pin].func);
	}

	return NOTIFY_OK;
}

static void mlx5_timecounter_init(struct mlx5_core_dev *mdev)
{
	struct mlx5_clock *clock = &mdev->clock;
	struct mlx5_timer *timer = &clock->timer;
	u32 dev_freq;

	dev_freq = MLX5_CAP_GEN(mdev, device_frequency_khz);
	timer->cycles.read = read_internal_timer;
	timer->cycles.shift = MLX5_CYCLES_SHIFT;
	timer->cycles.mult = clocksource_khz2mult(dev_freq,
						  timer->cycles.shift);
	timer->nominal_c_mult = timer->cycles.mult;
	timer->cycles.mask = CLOCKSOURCE_MASK(41);

	timecounter_init(&timer->tc, &timer->cycles,
			 ktime_to_ns(ktime_get_real()));
}

static void mlx5_init_overflow_period(struct mlx5_clock *clock)
{
	struct mlx5_core_dev *mdev = container_of(clock, struct mlx5_core_dev, clock);
	struct mlx5_ib_clock_info *clock_info = mdev->clock_info;
	struct mlx5_timer *timer = &clock->timer;
	u64 overflow_cycles;
	u64 frac = 0;
	u64 ns;
>>>>>>> upstream/android-13

	/* Calculate period in seconds to call the overflow watchdog - to make
	 * sure counter is checked at least twice every wrap around.
	 * The period is calculated as the minimum between max HW cycles count
	 * (The clock source mask) and max amount of cycles that can be
	 * multiplied by clock multiplier where the result doesn't exceed
	 * 64bits.
	 */
<<<<<<< HEAD
	overflow_cycles = div64_u64(~0ULL >> 1, clock->cycles.mult);
	overflow_cycles = min(overflow_cycles, div_u64(clock->cycles.mask, 3));

	ns = cyclecounter_cyc2ns(&clock->cycles, overflow_cycles,
				 frac, &frac);
	do_div(ns, NSEC_PER_SEC / HZ);
	clock->overflow_period = ns;

	mdev->clock_info_page = alloc_page(GFP_KERNEL);
	if (mdev->clock_info_page) {
		mdev->clock_info = kmap(mdev->clock_info_page);
		if (!mdev->clock_info) {
			__free_page(mdev->clock_info_page);
			mlx5_core_warn(mdev, "failed to map clock page\n");
		} else {
			mdev->clock_info->sign   = 0;
			mdev->clock_info->nsec   = clock->tc.nsec;
			mdev->clock_info->cycles = clock->tc.cycle_last;
			mdev->clock_info->mask   = clock->cycles.mask;
			mdev->clock_info->mult   = clock->nominal_c_mult;
			mdev->clock_info->shift  = clock->cycles.shift;
			mdev->clock_info->frac   = clock->tc.frac;
			mdev->clock_info->overflow_period =
						clock->overflow_period;
		}
	}

	INIT_WORK(&clock->pps_info.out_work, mlx5_pps_out);
	INIT_DELAYED_WORK(&clock->overflow_work, mlx5_timestamp_overflow);
	if (clock->overflow_period)
		schedule_delayed_work(&clock->overflow_work, 0);
	else
		mlx5_core_warn(mdev, "invalid overflow period, overflow_work is not scheduled\n");
=======
	overflow_cycles = div64_u64(~0ULL >> 1, timer->cycles.mult);
	overflow_cycles = min(overflow_cycles, div_u64(timer->cycles.mask, 3));

	ns = cyclecounter_cyc2ns(&timer->cycles, overflow_cycles,
				 frac, &frac);
	do_div(ns, NSEC_PER_SEC / HZ);
	timer->overflow_period = ns;

	INIT_DELAYED_WORK(&timer->overflow_work, mlx5_timestamp_overflow);
	if (timer->overflow_period)
		schedule_delayed_work(&timer->overflow_work, 0);
	else
		mlx5_core_warn(mdev,
			       "invalid overflow period, overflow_work is not scheduled\n");

	if (clock_info)
		clock_info->overflow_period = timer->overflow_period;
}

static void mlx5_init_clock_info(struct mlx5_core_dev *mdev)
{
	struct mlx5_clock *clock = &mdev->clock;
	struct mlx5_ib_clock_info *info;
	struct mlx5_timer *timer;

	mdev->clock_info = (struct mlx5_ib_clock_info *)get_zeroed_page(GFP_KERNEL);
	if (!mdev->clock_info) {
		mlx5_core_warn(mdev, "Failed to allocate IB clock info page\n");
		return;
	}

	info = mdev->clock_info;
	timer = &clock->timer;

	info->nsec = timer->tc.nsec;
	info->cycles = timer->tc.cycle_last;
	info->mask = timer->cycles.mask;
	info->mult = timer->nominal_c_mult;
	info->shift = timer->cycles.shift;
	info->frac = timer->tc.frac;
}

static void mlx5_init_timer_clock(struct mlx5_core_dev *mdev)
{
	struct mlx5_clock *clock = &mdev->clock;

	mlx5_timecounter_init(mdev);
	mlx5_init_clock_info(mdev);
	mlx5_init_overflow_period(clock);
	clock->ptp_info = mlx5_ptp_clock_info;

	if (mlx5_real_time_mode(mdev)) {
		struct timespec64 ts;

		ktime_get_real_ts64(&ts);
		mlx5_ptp_settime(&clock->ptp_info, &ts);
	}
}

static void mlx5_init_pps(struct mlx5_core_dev *mdev)
{
	struct mlx5_clock *clock = &mdev->clock;

	if (!MLX5_PPS_CAP(mdev))
		return;

	mlx5_get_pps_caps(mdev);
	mlx5_init_pin_config(clock);
}

void mlx5_init_clock(struct mlx5_core_dev *mdev)
{
	struct mlx5_clock *clock = &mdev->clock;

	if (!MLX5_CAP_GEN(mdev, device_frequency_khz)) {
		mlx5_core_warn(mdev, "invalid device_frequency_khz, aborting HW clock init\n");
		return;
	}

	seqlock_init(&clock->lock);
	mlx5_init_timer_clock(mdev);
	INIT_WORK(&clock->pps_info.out_work, mlx5_pps_out);
>>>>>>> upstream/android-13

	/* Configure the PHC */
	clock->ptp_info = mlx5_ptp_clock_info;

	/* Initialize 1PPS data structures */
<<<<<<< HEAD
	if (MLX5_PPS_CAP(mdev))
		mlx5_get_pps_caps(mdev);
	if (clock->ptp_info.n_pins)
		mlx5_init_pin_config(clock);
=======
	mlx5_init_pps(mdev);
>>>>>>> upstream/android-13

	clock->ptp = ptp_clock_register(&clock->ptp_info,
					&mdev->pdev->dev);
	if (IS_ERR(clock->ptp)) {
		mlx5_core_warn(mdev, "ptp_clock_register failed %ld\n",
			       PTR_ERR(clock->ptp));
		clock->ptp = NULL;
	}
<<<<<<< HEAD
=======

	MLX5_NB_INIT(&clock->pps_nb, mlx5_pps_event, PPS_EVENT);
	mlx5_eq_notifier_register(mdev, &clock->pps_nb);
>>>>>>> upstream/android-13
}

void mlx5_cleanup_clock(struct mlx5_core_dev *mdev)
{
	struct mlx5_clock *clock = &mdev->clock;

	if (!MLX5_CAP_GEN(mdev, device_frequency_khz))
		return;

<<<<<<< HEAD
=======
	mlx5_eq_notifier_unregister(mdev, &clock->pps_nb);
>>>>>>> upstream/android-13
	if (clock->ptp) {
		ptp_clock_unregister(clock->ptp);
		clock->ptp = NULL;
	}

	cancel_work_sync(&clock->pps_info.out_work);
<<<<<<< HEAD
	cancel_delayed_work_sync(&clock->overflow_work);

	if (mdev->clock_info) {
		kunmap(mdev->clock_info_page);
		__free_page(mdev->clock_info_page);
=======
	cancel_delayed_work_sync(&clock->timer.overflow_work);

	if (mdev->clock_info) {
		free_page((unsigned long)mdev->clock_info);
>>>>>>> upstream/android-13
		mdev->clock_info = NULL;
	}

	kfree(clock->ptp_info.pin_config);
}
