/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_HYPEVISOR_H
#define __LINUX_HYPEVISOR_H



#ifdef CONFIG_X86

#include <asm/jailhouse_para.h>
#include <asm/x86_init.h>

static inline void hypervisor_pin_vcpu(int cpu)
{
	x86_platform.hyper.pin_vcpu(cpu);
}

#else 

#include <linux/of.h>

static inline void hypervisor_pin_vcpu(int cpu)
{
}

static inline bool jailhouse_paravirt(void)
{
	return of_find_compatible_node(NULL, NULL, "jailhouse,cell");
}

#endif 

#endif 
