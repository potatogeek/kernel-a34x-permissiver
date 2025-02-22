// SPDX-License-Identifier: GPL-2.0
/*
 * TSC frequency enumeration via MSR
 *
 * Copyright (C) 2013, 2018 Intel Corporation
 * Author: Bin Gao <bin.gao@intel.com>
 */

#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/thread_info.h>
>>>>>>> upstream/android-13

#include <asm/apic.h>
#include <asm/cpu_device_id.h>
#include <asm/intel-family.h>
#include <asm/msr.h>
#include <asm/param.h>
#include <asm/tsc.h>

<<<<<<< HEAD
#define MAX_NUM_FREQS	9
=======
#define MAX_NUM_FREQS	16 /* 4 bits to select the frequency */

/*
 * The frequency numbers in the SDM are e.g. 83.3 MHz, which does not contain a
 * lot of accuracy which leads to clock drift. As far as we know Bay Trail SoCs
 * use a 25 MHz crystal and Cherry Trail uses a 19.2 MHz crystal, the crystal
 * is the source clk for a root PLL which outputs 1600 and 100 MHz. It is
 * unclear if the root PLL outputs are used directly by the CPU clock PLL or
 * if there is another PLL in between.
 * This does not matter though, we can model the chain of PLLs as a single PLL
 * with a quotient equal to the quotients of all PLLs in the chain multiplied.
 * So we can create a simplified model of the CPU clock setup using a reference
 * clock of 100 MHz plus a quotient which gets us as close to the frequency
 * from the SDM as possible.
 * For the 83.3 MHz example from above this would give us 100 MHz * 5 / 6 =
 * 83 and 1/3 MHz, which matches exactly what has been measured on actual hw.
 */
#define TSC_REFERENCE_KHZ 100000

struct muldiv {
	u32 multiplier;
	u32 divider;
};
>>>>>>> upstream/android-13

/*
 * If MSR_PERF_STAT[31] is set, the maximum resolved bus ratio can be
 * read in MSR_PLATFORM_ID[12:8], otherwise in MSR_PERF_STAT[44:40].
 * Unfortunately some Intel Atom SoCs aren't quite compliant to this,
 * so we need manually differentiate SoC families. This is what the
<<<<<<< HEAD
 * field msr_plat does.
 */
struct freq_desc {
	u8 msr_plat;	/* 1: use MSR_PLATFORM_INFO, 0: MSR_IA32_PERF_STATUS */
	u32 freqs[MAX_NUM_FREQS];
=======
 * field use_msr_plat does.
 */
struct freq_desc {
	bool use_msr_plat;
	struct muldiv muldiv[MAX_NUM_FREQS];
	/*
	 * Some CPU frequencies in the SDM do not map to known PLL freqs, in
	 * that case the muldiv array is empty and the freqs array is used.
	 */
	u32 freqs[MAX_NUM_FREQS];
	u32 mask;
>>>>>>> upstream/android-13
};

/*
 * Penwell and Clovertrail use spread spectrum clock,
 * so the freq number is not exactly the same as reported
 * by MSR based on SDM.
 */
static const struct freq_desc freq_desc_pnw = {
<<<<<<< HEAD
	0, { 0, 0, 0, 0, 0, 99840, 0, 83200 }
};

static const struct freq_desc freq_desc_clv = {
	0, { 0, 133200, 0, 0, 0, 99840, 0, 83200 }
};

static const struct freq_desc freq_desc_byt = {
	1, { 83300, 100000, 133300, 116700, 80000, 0, 0, 0 }
};

static const struct freq_desc freq_desc_cht = {
	1, { 83300, 100000, 133300, 116700, 80000, 93300, 90000, 88900, 87500 }
};

static const struct freq_desc freq_desc_tng = {
	1, { 0, 100000, 133300, 0, 0, 0, 0, 0 }
};

static const struct freq_desc freq_desc_ann = {
	1, { 83300, 100000, 133300, 100000, 0, 0, 0, 0 }
};

static const struct x86_cpu_id tsc_msr_cpu_ids[] = {
	INTEL_CPU_FAM6(ATOM_SALTWELL_MID,	freq_desc_pnw),
	INTEL_CPU_FAM6(ATOM_SALTWELL_TABLET,	freq_desc_clv),
	INTEL_CPU_FAM6(ATOM_SILVERMONT,		freq_desc_byt),
	INTEL_CPU_FAM6(ATOM_SILVERMONT_MID,	freq_desc_tng),
	INTEL_CPU_FAM6(ATOM_AIRMONT,		freq_desc_cht),
	INTEL_CPU_FAM6(ATOM_AIRMONT_MID,	freq_desc_ann),
=======
	.use_msr_plat = false,
	.freqs = { 0, 0, 0, 0, 0, 99840, 0, 83200 },
	.mask = 0x07,
};

static const struct freq_desc freq_desc_clv = {
	.use_msr_plat = false,
	.freqs = { 0, 133200, 0, 0, 0, 99840, 0, 83200 },
	.mask = 0x07,
};

/*
 * Bay Trail SDM MSR_FSB_FREQ frequencies simplified PLL model:
 *  000:   100 *  5 /  6  =  83.3333 MHz
 *  001:   100 *  1 /  1  = 100.0000 MHz
 *  010:   100 *  4 /  3  = 133.3333 MHz
 *  011:   100 *  7 /  6  = 116.6667 MHz
 *  100:   100 *  4 /  5  =  80.0000 MHz
 */
static const struct freq_desc freq_desc_byt = {
	.use_msr_plat = true,
	.muldiv = { { 5, 6 }, { 1, 1 }, { 4, 3 }, { 7, 6 },
		    { 4, 5 } },
	.mask = 0x07,
};

/*
 * Cherry Trail SDM MSR_FSB_FREQ frequencies simplified PLL model:
 * 0000:   100 *  5 /  6  =  83.3333 MHz
 * 0001:   100 *  1 /  1  = 100.0000 MHz
 * 0010:   100 *  4 /  3  = 133.3333 MHz
 * 0011:   100 *  7 /  6  = 116.6667 MHz
 * 0100:   100 *  4 /  5  =  80.0000 MHz
 * 0101:   100 * 14 / 15  =  93.3333 MHz
 * 0110:   100 *  9 / 10  =  90.0000 MHz
 * 0111:   100 *  8 /  9  =  88.8889 MHz
 * 1000:   100 *  7 /  8  =  87.5000 MHz
 */
static const struct freq_desc freq_desc_cht = {
	.use_msr_plat = true,
	.muldiv = { { 5, 6 }, {  1,  1 }, { 4,  3 }, { 7, 6 },
		    { 4, 5 }, { 14, 15 }, { 9, 10 }, { 8, 9 },
		    { 7, 8 } },
	.mask = 0x0f,
};

/*
 * Merriefield SDM MSR_FSB_FREQ frequencies simplified PLL model:
 * 0001:   100 *  1 /  1  = 100.0000 MHz
 * 0010:   100 *  4 /  3  = 133.3333 MHz
 */
static const struct freq_desc freq_desc_tng = {
	.use_msr_plat = true,
	.muldiv = { { 0, 0 }, { 1, 1 }, { 4, 3 } },
	.mask = 0x07,
};

/*
 * Moorefield SDM MSR_FSB_FREQ frequencies simplified PLL model:
 * 0000:   100 *  5 /  6  =  83.3333 MHz
 * 0001:   100 *  1 /  1  = 100.0000 MHz
 * 0010:   100 *  4 /  3  = 133.3333 MHz
 * 0011:   100 *  1 /  1  = 100.0000 MHz
 */
static const struct freq_desc freq_desc_ann = {
	.use_msr_plat = true,
	.muldiv = { { 5, 6 }, { 1, 1 }, { 4, 3 }, { 1, 1 } },
	.mask = 0x0f,
};

/*
 * 24 MHz crystal? : 24 * 13 / 4 = 78 MHz
 * Frequency step for Lightning Mountain SoC is fixed to 78 MHz,
 * so all the frequency entries are 78000.
 */
static const struct freq_desc freq_desc_lgm = {
	.use_msr_plat = true,
	.freqs = { 78000, 78000, 78000, 78000, 78000, 78000, 78000, 78000,
		   78000, 78000, 78000, 78000, 78000, 78000, 78000, 78000 },
	.mask = 0x0f,
};

static const struct x86_cpu_id tsc_msr_cpu_ids[] = {
	X86_MATCH_INTEL_FAM6_MODEL(ATOM_SALTWELL_MID,	&freq_desc_pnw),
	X86_MATCH_INTEL_FAM6_MODEL(ATOM_SALTWELL_TABLET,&freq_desc_clv),
	X86_MATCH_INTEL_FAM6_MODEL(ATOM_SILVERMONT,	&freq_desc_byt),
	X86_MATCH_INTEL_FAM6_MODEL(ATOM_SILVERMONT_MID,	&freq_desc_tng),
	X86_MATCH_INTEL_FAM6_MODEL(ATOM_AIRMONT,	&freq_desc_cht),
	X86_MATCH_INTEL_FAM6_MODEL(ATOM_AIRMONT_MID,	&freq_desc_ann),
	X86_MATCH_INTEL_FAM6_MODEL(ATOM_AIRMONT_NP,	&freq_desc_lgm),
>>>>>>> upstream/android-13
	{}
};

/*
 * MSR-based CPU/TSC frequency discovery for certain CPUs.
 *
<<<<<<< HEAD
 * Set global "lapic_timer_frequency" to bus_clock_cycles/jiffy
=======
 * Set global "lapic_timer_period" to bus_clock_cycles/jiffy
>>>>>>> upstream/android-13
 * Return processor base frequency in KHz, or 0 on failure.
 */
unsigned long cpu_khz_from_msr(void)
{
<<<<<<< HEAD
	u32 lo, hi, ratio, freq;
	const struct freq_desc *freq_desc;
	const struct x86_cpu_id *id;
	unsigned long res;
=======
	u32 lo, hi, ratio, freq, tscref;
	const struct freq_desc *freq_desc;
	const struct x86_cpu_id *id;
	const struct muldiv *md;
	unsigned long res;
	int index;
>>>>>>> upstream/android-13

	id = x86_match_cpu(tsc_msr_cpu_ids);
	if (!id)
		return 0;

	freq_desc = (struct freq_desc *)id->driver_data;
<<<<<<< HEAD
	if (freq_desc->msr_plat) {
=======
	if (freq_desc->use_msr_plat) {
>>>>>>> upstream/android-13
		rdmsr(MSR_PLATFORM_INFO, lo, hi);
		ratio = (lo >> 8) & 0xff;
	} else {
		rdmsr(MSR_IA32_PERF_STATUS, lo, hi);
		ratio = (hi >> 8) & 0x1f;
	}

	/* Get FSB FREQ ID */
	rdmsr(MSR_FSB_FREQ, lo, hi);
<<<<<<< HEAD

	/* Map CPU reference clock freq ID(0-7) to CPU reference clock freq(KHz) */
	freq = freq_desc->freqs[lo & 0x7];

	/* TSC frequency = maximum resolved freq * maximum resolved bus ratio */
	res = freq * ratio;

#ifdef CONFIG_X86_LOCAL_APIC
	lapic_timer_frequency = (freq * 1000) / HZ;
=======
	index = lo & freq_desc->mask;
	md = &freq_desc->muldiv[index];

	/*
	 * Note this also catches cases where the index points to an unpopulated
	 * part of muldiv, in that case the else will set freq and res to 0.
	 */
	if (md->divider) {
		tscref = TSC_REFERENCE_KHZ * md->multiplier;
		freq = DIV_ROUND_CLOSEST(tscref, md->divider);
		/*
		 * Multiplying by ratio before the division has better
		 * accuracy than just calculating freq * ratio.
		 */
		res = DIV_ROUND_CLOSEST(tscref * ratio, md->divider);
	} else {
		freq = freq_desc->freqs[index];
		res = freq * ratio;
	}

	if (freq == 0)
		pr_err("Error MSR_FSB_FREQ index %d is unknown\n", index);

#ifdef CONFIG_X86_LOCAL_APIC
	lapic_timer_period = (freq * 1000) / HZ;
>>>>>>> upstream/android-13
#endif

	/*
	 * TSC frequency determined by MSR is always considered "known"
	 * because it is reported by HW.
	 * Another fact is that on MSR capable platforms, PIT/HPET is
	 * generally not available so calibration won't work at all.
	 */
	setup_force_cpu_cap(X86_FEATURE_TSC_KNOWN_FREQ);

	/*
	 * Unfortunately there is no way for hardware to tell whether the
	 * TSC is reliable.  We were told by silicon design team that TSC
	 * on Atom SoCs are always "reliable". TSC is also the only
	 * reliable clocksource on these SoCs (HPET is either not present
	 * or not functional) so mark TSC reliable which removes the
	 * requirement for a watchdog clocksource.
	 */
	setup_force_cpu_cap(X86_FEATURE_TSC_RELIABLE);

	return res;
}
