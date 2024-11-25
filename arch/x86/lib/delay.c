// SPDX-License-Identifier: GPL-2.0
/*
 *	Precise Delay Loops for i386
 *
 *	Copyright (C) 1993 Linus Torvalds
 *	Copyright (C) 1997 Martin Mares <mj@atrey.karlin.mff.cuni.cz>
 *	Copyright (C) 2008 Jiri Hladky <hladky _dot_ jiri _at_ gmail _dot_ com>
 *
 *	The __delay function must _NOT_ be inlined as its execution time
 *	depends wildly on alignment on many x86 processors. The additional
 *	jump magic is needed to get the timing stable on all the CPU's
 *	we have to worry about.
 */

#include <linux/export.h>
#include <linux/sched.h>
#include <linux/timex.h>
#include <linux/preempt.h>
#include <linux/delay.h>

#include <asm/processor.h>
#include <asm/delay.h>
#include <asm/timer.h>
#include <asm/mwait.h>

#ifdef CONFIG_SMP
# include <asm/smp.h>
#endif

<<<<<<< HEAD
/* simple loop based delay: */
static void delay_loop(unsigned long loops)
{
=======
static void delay_loop(u64 __loops);

/*
 * Calibration and selection of the delay mechanism happens only once
 * during boot.
 */
static void (*delay_fn)(u64) __ro_after_init = delay_loop;
static void (*delay_halt_fn)(u64 start, u64 cycles) __ro_after_init;

/* simple loop based delay: */
static void delay_loop(u64 __loops)
{
	unsigned long loops = (unsigned long)__loops;

>>>>>>> upstream/android-13
	asm volatile(
		"	test %0,%0	\n"
		"	jz 3f		\n"
		"	jmp 1f		\n"

		".align 16		\n"
		"1:	jmp 2f		\n"

		".align 16		\n"
		"2:	dec %0		\n"
		"	jnz 2b		\n"
		"3:	dec %0		\n"

		: /* we don't need output */
		:"a" (loops)
	);
}

/* TSC based delay: */
<<<<<<< HEAD
static void delay_tsc(unsigned long __loops)
{
	u64 bclock, now, loops = __loops;
=======
static void delay_tsc(u64 cycles)
{
	u64 bclock, now;
>>>>>>> upstream/android-13
	int cpu;

	preempt_disable();
	cpu = smp_processor_id();
	bclock = rdtsc_ordered();
	for (;;) {
		now = rdtsc_ordered();
<<<<<<< HEAD
		if ((now - bclock) >= loops)
=======
		if ((now - bclock) >= cycles)
>>>>>>> upstream/android-13
			break;

		/* Allow RT tasks to run */
		preempt_enable();
		rep_nop();
		preempt_disable();

		/*
		 * It is possible that we moved to another CPU, and
		 * since TSC's are per-cpu we need to calculate
		 * that. The delay must guarantee that we wait "at
		 * least" the amount of time. Being moved to another
		 * CPU could make the wait longer but we just need to
		 * make sure we waited long enough. Rebalance the
		 * counter for this CPU.
		 */
		if (unlikely(cpu != smp_processor_id())) {
<<<<<<< HEAD
			loops -= (now - bclock);
=======
			cycles -= (now - bclock);
>>>>>>> upstream/android-13
			cpu = smp_processor_id();
			bclock = rdtsc_ordered();
		}
	}
	preempt_enable();
}

/*
<<<<<<< HEAD
 * On some AMD platforms, MWAITX has a configurable 32-bit timer, that
 * counts with TSC frequency. The input value is the loop of the
 * counter, it will exit when the timer expires.
 */
static void delay_mwaitx(unsigned long __loops)
{
	u64 start, end, delay, loops = __loops;
=======
 * On Intel the TPAUSE instruction waits until any of:
 * 1) the TSC counter exceeds the value provided in EDX:EAX
 * 2) global timeout in IA32_UMWAIT_CONTROL is exceeded
 * 3) an external interrupt occurs
 */
static void delay_halt_tpause(u64 start, u64 cycles)
{
	u64 until = start + cycles;
	u32 eax, edx;

	eax = lower_32_bits(until);
	edx = upper_32_bits(until);

	/*
	 * Hard code the deeper (C0.2) sleep state because exit latency is
	 * small compared to the "microseconds" that usleep() will delay.
	 */
	__tpause(TPAUSE_C02_STATE, edx, eax);
}

/*
 * On some AMD platforms, MWAITX has a configurable 32-bit timer, that
 * counts with TSC frequency. The input value is the number of TSC cycles
 * to wait. MWAITX will also exit when the timer expires.
 */
static void delay_halt_mwaitx(u64 unused, u64 cycles)
{
	u64 delay;

	delay = min_t(u64, MWAITX_MAX_WAIT_CYCLES, cycles);
	/*
	 * Use cpu_tss_rw as a cacheline-aligned, seldomly accessed per-cpu
	 * variable as the monitor target.
	 */
	 __monitorx(raw_cpu_ptr(&cpu_tss_rw), 0, 0);

	/*
	 * AMD, like Intel, supports the EAX hint and EAX=0xf means, do not
	 * enter any deep C-state and we use it here in delay() to minimize
	 * wakeup latency.
	 */
	__mwaitx(MWAITX_DISABLE_CSTATES, delay, MWAITX_ECX_TIMER_ENABLE);
}

/*
 * Call a vendor specific function to delay for a given amount of time. Because
 * these functions may return earlier than requested, check for actual elapsed
 * time and call again until done.
 */
static void delay_halt(u64 __cycles)
{
	u64 start, end, cycles = __cycles;
>>>>>>> upstream/android-13

	/*
	 * Timer value of 0 causes MWAITX to wait indefinitely, unless there
	 * is a store on the memory monitored by MONITORX.
	 */
<<<<<<< HEAD
	if (loops == 0)
=======
	if (!cycles)
>>>>>>> upstream/android-13
		return;

	start = rdtsc_ordered();

	for (;;) {
<<<<<<< HEAD
		delay = min_t(u64, MWAITX_MAX_LOOPS, loops);

		/*
		 * Use cpu_tss_rw as a cacheline-aligned, seldomly
		 * accessed per-cpu variable as the monitor target.
		 */
		__monitorx(raw_cpu_ptr(&cpu_tss_rw), 0, 0);

		/*
		 * AMD, like Intel's MWAIT version, supports the EAX hint and
		 * EAX=0xf0 means, do not enter any deep C-state and we use it
		 * here in delay() to minimize wakeup latency.
		 */
		__mwaitx(MWAITX_DISABLE_CSTATES, delay, MWAITX_ECX_TIMER_ENABLE);

		end = rdtsc_ordered();

		if (loops <= end - start)
			break;

		loops -= end - start;

=======
		delay_halt_fn(start, cycles);
		end = rdtsc_ordered();

		if (cycles <= end - start)
			break;

		cycles -= end - start;
>>>>>>> upstream/android-13
		start = end;
	}
}

<<<<<<< HEAD
/*
 * Since we calibrate only once at boot, this
 * function should be set once at boot and not changed
 */
static void (*delay_fn)(unsigned long) = delay_loop;

void use_tsc_delay(void)
=======
void __init use_tsc_delay(void)
>>>>>>> upstream/android-13
{
	if (delay_fn == delay_loop)
		delay_fn = delay_tsc;
}

<<<<<<< HEAD
void use_mwaitx_delay(void)
{
	delay_fn = delay_mwaitx;
=======
void __init use_tpause_delay(void)
{
	delay_halt_fn = delay_halt_tpause;
	delay_fn = delay_halt;
}

void use_mwaitx_delay(void)
{
	delay_halt_fn = delay_halt_mwaitx;
	delay_fn = delay_halt;
>>>>>>> upstream/android-13
}

int read_current_timer(unsigned long *timer_val)
{
	if (delay_fn == delay_tsc) {
		*timer_val = rdtsc();
		return 0;
	}
	return -1;
}

void __delay(unsigned long loops)
{
	delay_fn(loops);
}
EXPORT_SYMBOL(__delay);

<<<<<<< HEAD
void __const_udelay(unsigned long xloops)
=======
noinline void __const_udelay(unsigned long xloops)
>>>>>>> upstream/android-13
{
	unsigned long lpj = this_cpu_read(cpu_info.loops_per_jiffy) ? : loops_per_jiffy;
	int d0;

	xloops *= 4;
	asm("mull %%edx"
		:"=d" (xloops), "=&a" (d0)
		:"1" (xloops), "0" (lpj * (HZ / 4)));

	__delay(++xloops);
}
EXPORT_SYMBOL(__const_udelay);

void __udelay(unsigned long usecs)
{
	__const_udelay(usecs * 0x000010c7); /* 2**32 / 1000000 (rounded up) */
}
EXPORT_SYMBOL(__udelay);

void __ndelay(unsigned long nsecs)
{
	__const_udelay(nsecs * 0x00005); /* 2**32 / 1000000000 (rounded up) */
}
EXPORT_SYMBOL(__ndelay);
