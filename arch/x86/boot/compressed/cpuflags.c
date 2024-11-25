// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#ifdef CONFIG_RANDOMIZE_BASE

=======
>>>>>>> upstream/android-13
#include "../cpuflags.c"

bool has_cpuflag(int flag)
{
	get_cpuflags();

	return test_bit(flag, cpu.flags);
}
<<<<<<< HEAD

#endif
=======
>>>>>>> upstream/android-13
