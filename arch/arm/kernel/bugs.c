<<<<<<< HEAD
// SPDX-Identifier: GPL-2.0
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
#include <linux/init.h>
#include <asm/bugs.h>
#include <asm/proc-fns.h>

void check_other_bugs(void)
{
#ifdef MULTI_CPU
	if (cpu_check_bugs)
		cpu_check_bugs();
#endif
}

void __init check_bugs(void)
{
	check_writebuffer_bugs();
	check_other_bugs();
}
