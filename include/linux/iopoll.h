<<<<<<< HEAD
/*
 * Copyright (c) 2012-2014 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2012-2014 The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef _LINUX_IOPOLL_H
#define _LINUX_IOPOLL_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/io.h>

/**
<<<<<<< HEAD
=======
 * read_poll_timeout - Periodically poll an address until a condition is
 *			met or a timeout occurs
 * @op: accessor function (takes @args as its arguments)
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @sleep_us: Maximum time to sleep between reads in us (0
 *            tight-loops).  Should be less than ~20ms since usleep_range
 *            is used (see Documentation/timers/timers-howto.rst).
 * @timeout_us: Timeout in us, 0 means never timeout
 * @sleep_before_read: if it is true, sleep @sleep_us before read.
 * @args: arguments for @op poll
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout. In either
 * case, the last read value at @args is stored in @val. Must not
 * be called from atomic context if sleep_us or timeout_us are used.
 *
 * When available, you'll probably want to use one of the specialized
 * macros defined below rather than this macro directly.
 */
#define read_poll_timeout(op, val, cond, sleep_us, timeout_us, \
				sleep_before_read, args...) \
({ \
	u64 __timeout_us = (timeout_us); \
	unsigned long __sleep_us = (sleep_us); \
	ktime_t __timeout = ktime_add_us(ktime_get(), __timeout_us); \
	might_sleep_if((__sleep_us) != 0); \
	if (sleep_before_read && __sleep_us) \
		usleep_range((__sleep_us >> 2) + 1, __sleep_us); \
	for (;;) { \
		(val) = op(args); \
		if (cond) \
			break; \
		if (__timeout_us && \
		    ktime_compare(ktime_get(), __timeout) > 0) { \
			(val) = op(args); \
			break; \
		} \
		if (__sleep_us) \
			usleep_range((__sleep_us >> 2) + 1, __sleep_us); \
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})

/**
 * read_poll_timeout_atomic - Periodically poll an address until a condition is
 * 				met or a timeout occurs
 * @op: accessor function (takes @args as its arguments)
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @delay_us: Time to udelay between reads in us (0 tight-loops).  Should
 *            be less than ~10us since udelay is used (see
 *            Documentation/timers/timers-howto.rst).
 * @timeout_us: Timeout in us, 0 means never timeout
 * @delay_before_read: if it is true, delay @delay_us before read.
 * @args: arguments for @op poll
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout. In either
 * case, the last read value at @args is stored in @val.
 *
 * When available, you'll probably want to use one of the specialized
 * macros defined below rather than this macro directly.
 */
#define read_poll_timeout_atomic(op, val, cond, delay_us, timeout_us, \
					delay_before_read, args...) \
({ \
	u64 __timeout_us = (timeout_us); \
	unsigned long __delay_us = (delay_us); \
	ktime_t __timeout = ktime_add_us(ktime_get(), __timeout_us); \
	if (delay_before_read && __delay_us) \
		udelay(__delay_us); \
	for (;;) { \
		(val) = op(args); \
		if (cond) \
			break; \
		if (__timeout_us && \
		    ktime_compare(ktime_get(), __timeout) > 0) { \
			(val) = op(args); \
			break; \
		} \
		if (__delay_us) \
			udelay(__delay_us); \
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})

/**
>>>>>>> upstream/android-13
 * readx_poll_timeout - Periodically poll an address until a condition is met or a timeout occurs
 * @op: accessor function (takes @addr as its only argument)
 * @addr: Address to poll
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @sleep_us: Maximum time to sleep between reads in us (0
 *            tight-loops).  Should be less than ~20ms since usleep_range
<<<<<<< HEAD
 *            is used (see Documentation/timers/timers-howto.txt).
=======
 *            is used (see Documentation/timers/timers-howto.rst).
>>>>>>> upstream/android-13
 * @timeout_us: Timeout in us, 0 means never timeout
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout. In either
 * case, the last read value at @addr is stored in @val. Must not
 * be called from atomic context if sleep_us or timeout_us are used.
 *
 * When available, you'll probably want to use one of the specialized
 * macros defined below rather than this macro directly.
 */
#define readx_poll_timeout(op, addr, val, cond, sleep_us, timeout_us)	\
<<<<<<< HEAD
({ \
	u64 __timeout_us = (timeout_us); \
	unsigned long __sleep_us = (sleep_us); \
	ktime_t __timeout = ktime_add_us(ktime_get(), __timeout_us); \
	might_sleep_if((__sleep_us) != 0); \
	for (;;) { \
		(val) = op(addr); \
		if (cond) \
			break; \
		if (__timeout_us && \
		    ktime_compare(ktime_get(), __timeout) > 0) { \
			(val) = op(addr); \
			break; \
		} \
		if (__sleep_us) \
			usleep_range((__sleep_us >> 2) + 1, __sleep_us); \
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})
=======
	read_poll_timeout(op, val, cond, sleep_us, timeout_us, false, addr)
>>>>>>> upstream/android-13

/**
 * readx_poll_timeout_atomic - Periodically poll an address until a condition is met or a timeout occurs
 * @op: accessor function (takes @addr as its only argument)
 * @addr: Address to poll
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @delay_us: Time to udelay between reads in us (0 tight-loops).  Should
 *            be less than ~10us since udelay is used (see
<<<<<<< HEAD
 *            Documentation/timers/timers-howto.txt).
=======
 *            Documentation/timers/timers-howto.rst).
>>>>>>> upstream/android-13
 * @timeout_us: Timeout in us, 0 means never timeout
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout. In either
 * case, the last read value at @addr is stored in @val.
 *
 * When available, you'll probably want to use one of the specialized
 * macros defined below rather than this macro directly.
 */
#define readx_poll_timeout_atomic(op, addr, val, cond, delay_us, timeout_us) \
<<<<<<< HEAD
({ \
	u64 __timeout_us = (timeout_us); \
	unsigned long __delay_us = (delay_us); \
	ktime_t __timeout = ktime_add_us(ktime_get(), __timeout_us); \
	for (;;) { \
		(val) = op(addr); \
		if (cond) \
			break; \
		if (__timeout_us && \
		    ktime_compare(ktime_get(), __timeout) > 0) { \
			(val) = op(addr); \
			break; \
		} \
		if (__delay_us) \
			udelay(__delay_us);	\
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})

=======
	read_poll_timeout_atomic(op, val, cond, delay_us, timeout_us, false, addr)
>>>>>>> upstream/android-13

#define readb_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readb, addr, val, cond, delay_us, timeout_us)

#define readb_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readb, addr, val, cond, delay_us, timeout_us)

#define readw_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readw, addr, val, cond, delay_us, timeout_us)

#define readw_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readw, addr, val, cond, delay_us, timeout_us)

#define readl_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readl, addr, val, cond, delay_us, timeout_us)

#define readl_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readl, addr, val, cond, delay_us, timeout_us)

#define readq_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readq, addr, val, cond, delay_us, timeout_us)

#define readq_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readq, addr, val, cond, delay_us, timeout_us)

#define readb_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readb_relaxed, addr, val, cond, delay_us, timeout_us)

#define readb_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readb_relaxed, addr, val, cond, delay_us, timeout_us)

#define readw_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readw_relaxed, addr, val, cond, delay_us, timeout_us)

#define readw_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readw_relaxed, addr, val, cond, delay_us, timeout_us)

#define readl_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readl_relaxed, addr, val, cond, delay_us, timeout_us)

#define readl_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readl_relaxed, addr, val, cond, delay_us, timeout_us)

#define readq_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readq_relaxed, addr, val, cond, delay_us, timeout_us)

#define readq_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readq_relaxed, addr, val, cond, delay_us, timeout_us)

#endif /* _LINUX_IOPOLL_H */
