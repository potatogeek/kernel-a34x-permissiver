<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2015 Anton Ivanov (aivanov@{brocade.com,kot-begemot.co.uk})
 * Copyright (C) 2015 Thomas Meyer (thomas@m3y3r.de)
 * Copyright (C) 2012-2014 Cisco Systems
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike{addtoit,linux.intel}.com)
<<<<<<< HEAD
 * Licensed under the GPL
 */

#include <stddef.h>
=======
 */

#include <stddef.h>
#include <unistd.h>
>>>>>>> upstream/android-13
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <kern_util.h>
#include <os.h>
#include <string.h>
<<<<<<< HEAD
#include <timer-internal.h>
=======
>>>>>>> upstream/android-13

static timer_t event_high_res_timer = 0;

static inline long long timeval_to_ns(const struct timeval *tv)
{
	return ((long long) tv->tv_sec * UM_NSEC_PER_SEC) +
		tv->tv_usec * UM_NSEC_PER_USEC;
}

static inline long long timespec_to_ns(const struct timespec *ts)
{
<<<<<<< HEAD
	return ((long long) ts->tv_sec * UM_NSEC_PER_SEC) +
		ts->tv_nsec;
}

long long os_persistent_clock_emulation (void) {
=======
	return ((long long) ts->tv_sec * UM_NSEC_PER_SEC) + ts->tv_nsec;
}

long long os_persistent_clock_emulation(void)
{
>>>>>>> upstream/android-13
	struct timespec realtime_tp;

	clock_gettime(CLOCK_REALTIME, &realtime_tp);
	return timespec_to_ns(&realtime_tp);
}

/**
 * os_timer_create() - create an new posix (interval) timer
 */
<<<<<<< HEAD
int os_timer_create(void* timer) {

	timer_t* t = timer;

	if(t == NULL) {
		t = &event_high_res_timer;
	}

	if (timer_create(
		CLOCK_MONOTONIC,
		NULL,
		t) == -1) {
		return -1;
	}
	return 0;
}

int os_timer_set_interval(void* timer, void* i)
{
	struct itimerspec its;
	unsigned long long nsec;
	timer_t* t = timer;
	struct itimerspec* its_in = i;

	if(t == NULL) {
		t = &event_high_res_timer;
	}

	nsec = UM_NSEC_PER_SEC / UM_HZ;

	if(its_in != NULL) {
		its.it_value.tv_sec = its_in->it_value.tv_sec;
		its.it_value.tv_nsec = its_in->it_value.tv_nsec;
	} else {
		its.it_value.tv_sec = 0;
		its.it_value.tv_nsec = nsec;
	}

	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = nsec;

	if(timer_settime(*t, 0, &its, NULL) == -1) {
		return -errno;
	}
=======
int os_timer_create(void)
{
	timer_t *t = &event_high_res_timer;

	if (timer_create(CLOCK_MONOTONIC, NULL, t) == -1)
		return -1;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/**
 * os_timer_remain() - returns the remaining nano seconds of the given interval
 *                     timer
 * Because this is the remaining time of an interval timer, which correspondends
 * to HZ, this value can never be bigger than one second. Just
 * the nanosecond part of the timer is returned.
 * The returned time is relative to the start time of the interval timer.
 * Return an negative value in an error case.
 */
long os_timer_remain(void* timer)
{
	struct itimerspec its;
	timer_t* t = timer;

	if(t == NULL) {
		t = &event_high_res_timer;
	}

	if(timer_gettime(t, &its) == -1) {
		return -errno;
	}

	return its.it_value.tv_nsec;
}

int os_timer_one_shot(int ticks)
{
	struct itimerspec its;
	unsigned long long nsec;
	unsigned long sec;

    nsec = (ticks + 1);
    sec = nsec / UM_NSEC_PER_SEC;
	nsec = nsec % UM_NSEC_PER_SEC;

	its.it_value.tv_sec = nsec / UM_NSEC_PER_SEC;
	its.it_value.tv_nsec = nsec;

	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = 0; // we cheat here
=======
int os_timer_set_interval(unsigned long long nsecs)
{
	struct itimerspec its;

	its.it_value.tv_sec = nsecs / UM_NSEC_PER_SEC;
	its.it_value.tv_nsec = nsecs % UM_NSEC_PER_SEC;

	its.it_interval.tv_sec = nsecs / UM_NSEC_PER_SEC;
	its.it_interval.tv_nsec = nsecs % UM_NSEC_PER_SEC;

	if (timer_settime(event_high_res_timer, 0, &its, NULL) == -1)
		return -errno;

	return 0;
}

int os_timer_one_shot(unsigned long long nsecs)
{
	struct itimerspec its = {
		.it_value.tv_sec = nsecs / UM_NSEC_PER_SEC,
		.it_value.tv_nsec = nsecs % UM_NSEC_PER_SEC,

		.it_interval.tv_sec = 0,
		.it_interval.tv_nsec = 0, // we cheat here
	};
>>>>>>> upstream/android-13

	timer_settime(event_high_res_timer, 0, &its, NULL);
	return 0;
}

/**
 * os_timer_disable() - disable the posix (interval) timer
<<<<<<< HEAD
 * Returns the remaining interval timer time in nanoseconds
 */
long long os_timer_disable(void)
=======
 */
void os_timer_disable(void)
>>>>>>> upstream/android-13
{
	struct itimerspec its;

	memset(&its, 0, sizeof(struct itimerspec));
<<<<<<< HEAD
	timer_settime(event_high_res_timer, 0, &its, &its);

	return its.it_value.tv_sec * UM_NSEC_PER_SEC + its.it_value.tv_nsec;
}

long long os_vnsecs(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&ts);
	return timespec_to_ns(&ts);
=======
	timer_settime(event_high_res_timer, 0, &its, NULL);
>>>>>>> upstream/android-13
}

long long os_nsecs(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC,&ts);
	return timespec_to_ns(&ts);
}

/**
<<<<<<< HEAD
 * os_idle_sleep() - sleep for a given time of nsecs
 * @nsecs: nanoseconds to sleep
 */
void os_idle_sleep(unsigned long long nsecs)
{
	struct timespec ts;

	if (nsecs <= 0) {
		return;
	}

	ts = ((struct timespec) {
			.tv_sec  = nsecs / UM_NSEC_PER_SEC,
			.tv_nsec = nsecs % UM_NSEC_PER_SEC
	});

	/*
	 * Relay the signal if clock_nanosleep is interrupted.
	 */
	if (clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL)) {
		deliver_alarm();
	}
=======
 * os_idle_sleep() - sleep until interrupted
 */
void os_idle_sleep(void)
{
	struct itimerspec its;
	sigset_t set, old;

	/* block SIGALRM while we analyze the timer state */
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	sigprocmask(SIG_BLOCK, &set, &old);

	/* check the timer, and if it'll fire then wait for it */
	timer_gettime(event_high_res_timer, &its);
	if (its.it_value.tv_sec || its.it_value.tv_nsec)
		sigsuspend(&old);
	/* either way, restore the signal mask */
	sigprocmask(SIG_UNBLOCK, &set, NULL);
>>>>>>> upstream/android-13
}
