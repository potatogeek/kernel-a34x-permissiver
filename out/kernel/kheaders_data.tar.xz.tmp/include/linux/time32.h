#ifndef _LINUX_TIME32_H
#define _LINUX_TIME32_H


#include <linux/time64.h>

#include <linux/timex.h>

#include <vdso/time32.h>

#define TIME_T_MAX	(time_t)((1UL << ((sizeof(time_t) << 3) - 1)) - 1)

#if __BITS_PER_LONG == 64


static inline struct timespec timespec64_to_timespec(const struct timespec64 ts64)
{
	return *(const struct timespec *)&ts64;
}

static inline struct timespec64 timespec_to_timespec64(const struct timespec ts)
{
	return *(const struct timespec64 *)&ts;
}

#else
static inline struct timespec timespec64_to_timespec(const struct timespec64 ts64)
{
	struct timespec ret;

	ret.tv_sec = (time_t)ts64.tv_sec;
	ret.tv_nsec = ts64.tv_nsec;
	return ret;
}

static inline struct timespec64 timespec_to_timespec64(const struct timespec ts)
{
	struct timespec64 ret;

	ret.tv_sec = ts.tv_sec;
	ret.tv_nsec = ts.tv_nsec;
	return ret;
}
#endif

static inline int timespec_equal(const struct timespec *a,
				 const struct timespec *b)
{
	return (a->tv_sec == b->tv_sec) && (a->tv_nsec == b->tv_nsec);
}


static inline int timespec_compare(const struct timespec *lhs, const struct timespec *rhs)
{
	if (lhs->tv_sec < rhs->tv_sec)
		return -1;
	if (lhs->tv_sec > rhs->tv_sec)
		return 1;
	return lhs->tv_nsec - rhs->tv_nsec;
}

extern void set_normalized_timespec(struct timespec *ts, time_t sec, s64 nsec);

static inline struct timespec timespec_add(struct timespec lhs,
						struct timespec rhs)
{
	struct timespec ts_delta;

	set_normalized_timespec(&ts_delta, lhs.tv_sec + rhs.tv_sec,
				lhs.tv_nsec + rhs.tv_nsec);
	return ts_delta;
}


static inline struct timespec timespec_sub(struct timespec lhs,
						struct timespec rhs)
{
	struct timespec ts_delta;

	set_normalized_timespec(&ts_delta, lhs.tv_sec - rhs.tv_sec,
				lhs.tv_nsec - rhs.tv_nsec);
	return ts_delta;
}


static inline bool timespec_valid(const struct timespec *ts)
{
	
	if (ts->tv_sec < 0)
		return false;
	
	if ((unsigned long)ts->tv_nsec >= NSEC_PER_SEC)
		return false;
	return true;
}

static inline bool timespec_valid_strict(const struct timespec *ts)
{
	if (!timespec_valid(ts))
		return false;
	
	if ((unsigned long long)ts->tv_sec >= KTIME_SEC_MAX)
		return false;
	return true;
}


static inline s64 timespec_to_ns(const struct timespec *ts)
{
	return ((s64) ts->tv_sec * NSEC_PER_SEC) + ts->tv_nsec;
}


extern struct timespec ns_to_timespec(const s64 nsec);


static __always_inline void timespec_add_ns(struct timespec *a, u64 ns)
{
	a->tv_sec += __iter_div_u64_rem(a->tv_nsec + ns, NSEC_PER_SEC, &ns);
	a->tv_nsec = ns;
}


static inline void time_to_tm(time_t totalsecs, int offset, struct tm *result)
{
	time64_to_tm(totalsecs, offset, result);
}

static inline unsigned long mktime(const unsigned int year,
			const unsigned int mon, const unsigned int day,
			const unsigned int hour, const unsigned int min,
			const unsigned int sec)
{
	return mktime64(year, mon, day, hour, min, sec);
}

static inline bool timeval_valid(const struct timeval *tv)
{
	
	if (tv->tv_sec < 0)
		return false;

	
	if (tv->tv_usec < 0 || tv->tv_usec >= USEC_PER_SEC)
		return false;

	return true;
}

extern struct timespec timespec_trunc(struct timespec t, unsigned int gran);


static inline s64 timeval_to_ns(const struct timeval *tv)
{
	return ((s64) tv->tv_sec * NSEC_PER_SEC) +
		tv->tv_usec * NSEC_PER_USEC;
}


extern struct timeval ns_to_timeval(const s64 nsec);
extern struct __kernel_old_timeval ns_to_kernel_old_timeval(s64 nsec);


#define old_itimerspec32	compat_itimerspec
#define ns_to_old_timeval32	ns_to_compat_timeval
#define get_old_itimerspec32	get_compat_itimerspec64
#define put_old_itimerspec32	put_compat_itimerspec64
#define get_old_timespec32	compat_get_timespec64
#define put_old_timespec32	compat_put_timespec64

#endif
