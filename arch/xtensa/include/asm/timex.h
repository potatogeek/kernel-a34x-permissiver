/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2013 Tensilica Inc.
 */

#ifndef _XTENSA_TIMEX_H
#define _XTENSA_TIMEX_H

#include <asm/processor.h>
<<<<<<< HEAD
#include <linux/stringify.h>
=======
>>>>>>> upstream/android-13

#if XCHAL_NUM_TIMERS > 0 && \
	XTENSA_INT_LEVEL(XCHAL_TIMER0_INTERRUPT) <= XCHAL_EXCM_LEVEL
# define LINUX_TIMER     0
# define LINUX_TIMER_INT XCHAL_TIMER0_INTERRUPT
#elif XCHAL_NUM_TIMERS > 1 && \
	XTENSA_INT_LEVEL(XCHAL_TIMER1_INTERRUPT) <= XCHAL_EXCM_LEVEL
# define LINUX_TIMER     1
# define LINUX_TIMER_INT XCHAL_TIMER1_INTERRUPT
#elif XCHAL_NUM_TIMERS > 2 && \
	XTENSA_INT_LEVEL(XCHAL_TIMER2_INTERRUPT) <= XCHAL_EXCM_LEVEL
# define LINUX_TIMER     2
# define LINUX_TIMER_INT XCHAL_TIMER2_INTERRUPT
#else
# error "Bad timer number for Linux configurations!"
#endif

extern unsigned long ccount_freq;

typedef unsigned long long cycles_t;

#define get_cycles()	(0)

void local_timer_setup(unsigned cpu);

/*
 * Register access.
 */

<<<<<<< HEAD
#define WSR_CCOUNT(r)	  asm volatile ("wsr %0, ccount" :: "a" (r))
#define RSR_CCOUNT(r)	  asm volatile ("rsr %0, ccount" : "=a" (r))
#define WSR_CCOMPARE(x,r) asm volatile ("wsr %0,"__stringify(SREG_CCOMPARE)"+"__stringify(x) :: "a"(r))
#define RSR_CCOMPARE(x,r) asm volatile ("rsr %0,"__stringify(SREG_CCOMPARE)"+"__stringify(x) : "=a"(r))

static inline unsigned long get_ccount (void)
{
	unsigned long ccount;
	RSR_CCOUNT(ccount);
	return ccount;
=======
static inline unsigned long get_ccount (void)
{
	return xtensa_get_sr(ccount);
>>>>>>> upstream/android-13
}

static inline void set_ccount (unsigned long ccount)
{
<<<<<<< HEAD
	WSR_CCOUNT(ccount);
=======
	xtensa_set_sr(ccount, ccount);
>>>>>>> upstream/android-13
}

static inline unsigned long get_linux_timer (void)
{
<<<<<<< HEAD
	unsigned ccompare;
	RSR_CCOMPARE(LINUX_TIMER, ccompare);
	return ccompare;
=======
	return xtensa_get_sr(SREG_CCOMPARE + LINUX_TIMER);
>>>>>>> upstream/android-13
}

static inline void set_linux_timer (unsigned long ccompare)
{
<<<<<<< HEAD
	WSR_CCOMPARE(LINUX_TIMER, ccompare);
=======
	xtensa_set_sr(ccompare, SREG_CCOMPARE + LINUX_TIMER);
>>>>>>> upstream/android-13
}

#endif	/* _XTENSA_TIMEX_H */
