
#ifndef LINUX_SCHED_CLOCK
#define LINUX_SCHED_CLOCK

#ifdef CONFIG_GENERIC_SCHED_CLOCK
extern void generic_sched_clock_init(void);

extern void sched_clock_register(u64 (*read)(void), int bits,
				 unsigned long rate);
#else
static inline void generic_sched_clock_init(void) { }

static inline void sched_clock_register(u64 (*read)(void), int bits,
					unsigned long rate)
{
}
#endif

#endif
