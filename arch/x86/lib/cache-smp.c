// SPDX-License-Identifier: GPL-2.0
#include <linux/smp.h>
#include <linux/export.h>

static void __wbinvd(void *dummy)
{
	wbinvd();
}

void wbinvd_on_cpu(int cpu)
{
	smp_call_function_single(cpu, __wbinvd, NULL, 1);
}
EXPORT_SYMBOL(wbinvd_on_cpu);

int wbinvd_on_all_cpus(void)
{
<<<<<<< HEAD
	return on_each_cpu(__wbinvd, NULL, 1);
=======
	on_each_cpu(__wbinvd, NULL, 1);
	return 0;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(wbinvd_on_all_cpus);
