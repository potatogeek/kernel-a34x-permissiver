/*
 * include/linux/wakeup_reason.h
 *
 * Logs the reason which caused the kernel to resume
 * from the suspend mode.
 *
 * Copyright (C) 2014 Google, Inc.
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _LINUX_WAKEUP_REASON_H
#define _LINUX_WAKEUP_REASON_H

#define MAX_SUSPEND_ABORT_LEN 256

#ifdef CONFIG_SUSPEND
<<<<<<< HEAD
#ifdef CONFIG_SEC_PM
void log_wakeup_reason_spm(int irq, char *wakesrc_str,
			unsigned int assert_pc);
#endif /* CONFIG_SEC_PM */
=======
>>>>>>> upstream/android-13
void log_irq_wakeup_reason(int irq);
void log_threaded_irq_wakeup_reason(int irq, int parent_irq);
void log_suspend_abort_reason(const char *fmt, ...);
void log_abnormal_wakeup_reason(const char *fmt, ...);
void clear_wakeup_reasons(void);
#else
static inline void log_irq_wakeup_reason(int irq) { }
static inline void log_threaded_irq_wakeup_reason(int irq, int parent_irq) { }
static inline void log_suspend_abort_reason(const char *fmt, ...) { }
static inline void log_abnormal_wakeup_reason(const char *fmt, ...) { }
static inline void clear_wakeup_reasons(void) { }
#endif

#endif /* _LINUX_WAKEUP_REASON_H */
