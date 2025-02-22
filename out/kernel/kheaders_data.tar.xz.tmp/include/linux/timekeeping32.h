#ifndef _LINUX_TIMEKEEPING32_H
#define _LINUX_TIMEKEEPING32_H


static inline void do_gettimeofday(struct timeval *tv)
{
	struct timespec64 now;

	ktime_get_real_ts64(&now);
	tv->tv_sec = now.tv_sec;
	tv->tv_usec = now.tv_nsec/1000;
}

static inline unsigned long get_seconds(void)
{
	return ktime_get_real_seconds();
}

static inline struct timespec current_kernel_time(void)
{
	struct timespec64 ts64;

	ktime_get_coarse_real_ts64(&ts64);

	return timespec64_to_timespec(ts64);
}


static inline int do_settimeofday(const struct timespec *ts)
{
	struct timespec64 ts64;

	ts64 = timespec_to_timespec64(*ts);
	return do_settimeofday64(&ts64);
}

static inline void getnstimeofday(struct timespec *ts)
{
	struct timespec64 ts64;

	ktime_get_real_ts64(&ts64);
	*ts = timespec64_to_timespec(ts64);
}

static inline void ktime_get_ts(struct timespec *ts)
{
	struct timespec64 ts64;

	ktime_get_ts64(&ts64);
	*ts = timespec64_to_timespec(ts64);
}

static inline void ktime_get_real_ts(struct timespec *ts)
{
	struct timespec64 ts64;

	ktime_get_real_ts64(&ts64);
	*ts = timespec64_to_timespec(ts64);
}

static inline void getrawmonotonic(struct timespec *ts)
{
	struct timespec64 ts64;

	ktime_get_raw_ts64(&ts64);
	*ts = timespec64_to_timespec(ts64);
}

static inline struct timespec get_monotonic_coarse(void)
{
	struct timespec64 ts64;

	ktime_get_coarse_ts64(&ts64);

	return timespec64_to_timespec(ts64);
}

static inline void getboottime(struct timespec *ts)
{
	struct timespec64 ts64;

	getboottime64(&ts64);
	*ts = timespec64_to_timespec(ts64);
}


static inline void get_monotonic_boottime(struct timespec *ts)
{
	*ts = ktime_to_timespec(ktime_get_boottime());
}

static inline void timekeeping_clocktai(struct timespec *ts)
{
	*ts = ktime_to_timespec(ktime_get_clocktai());
}


extern void read_persistent_clock(struct timespec *ts);
extern int update_persistent_clock(struct timespec now);

#endif
