/* SPDX-License-Identifier: GPL-2.0 */
/*
 * GCC stack protector support.
 *
 * Stack protector works by putting predefined pattern at the start of
 * the stack frame and verifying that it hasn't been overwritten when
 * returning from the function.  The pattern is called stack canary
 * and gcc expects it to be defined by a global variable called
<<<<<<< HEAD
 * "__stack_chk_guard" on ARM.  This unfortunately means that on SMP
 * we cannot have a different canary value per task.
=======
 * "__stack_chk_guard" on ARM.  This prevents SMP systems from using a
 * different value for each task unless we enable a GCC plugin that
 * replaces these symbol references with references to each task's own
 * value.
>>>>>>> upstream/android-13
 */

#ifndef _ASM_STACKPROTECTOR_H
#define _ASM_STACKPROTECTOR_H 1

#include <linux/random.h>
#include <linux/version.h>

<<<<<<< HEAD
=======
#include <asm/thread_info.h>

>>>>>>> upstream/android-13
extern unsigned long __stack_chk_guard;

/*
 * Initialize the stackprotector canary value.
 *
 * NOTE: this must only be called from functions that never return,
 * and it must always be inlined.
 */
static __always_inline void boot_init_stack_canary(void)
{
	unsigned long canary;

	/* Try to get a semi random initial value. */
	get_random_bytes(&canary, sizeof(canary));
	canary ^= LINUX_VERSION_CODE;

	current->stack_canary = canary;
<<<<<<< HEAD
	__stack_chk_guard = current->stack_canary;
=======
#ifndef CONFIG_STACKPROTECTOR_PER_TASK
	__stack_chk_guard = current->stack_canary;
#else
	current_thread_info()->stack_canary = current->stack_canary;
#endif
>>>>>>> upstream/android-13
}

#endif	/* _ASM_STACKPROTECTOR_H */
