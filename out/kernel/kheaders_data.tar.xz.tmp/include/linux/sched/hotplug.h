/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_HOTPLUG_H
#define _LINUX_SCHED_HOTPLUG_H



extern int sched_cpu_starting(unsigned int cpu);
extern int sched_cpu_activate(unsigned int cpu);
extern int sched_cpu_deactivate(unsigned int cpu);

#ifdef CONFIG_HOTPLUG_CPU
extern int sched_cpu_dying(unsigned int cpu);
#else
# define sched_cpu_dying	NULL
#endif

#ifdef CONFIG_HOTPLUG_CPU
extern void idle_task_exit(void);
#else
static inline void idle_task_exit(void) {}
#endif

#endif 
