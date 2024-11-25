/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_STACKPROTECTOR_H
#define _LINUX_STACKPROTECTOR_H 1

#include <linux/compiler.h>
#include <linux/sched.h>
#include <linux/random.h>

<<<<<<< HEAD
#ifdef CONFIG_STACKPROTECTOR
=======
#if defined(CONFIG_STACKPROTECTOR) || defined(CONFIG_ARM64_PTR_AUTH)
>>>>>>> upstream/android-13
# include <asm/stackprotector.h>
#else
static inline void boot_init_stack_canary(void)
{
}
#endif

#endif
