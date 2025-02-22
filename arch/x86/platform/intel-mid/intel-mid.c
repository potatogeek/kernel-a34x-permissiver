<<<<<<< HEAD
/*
 * intel-mid.c: Intel MID platform setup code
 *
 * (C) Copyright 2008, 2012 Intel Corporation
 * Author: Jacob Pan (jacob.jun.pan@intel.com)
 * Author: Sathyanarayanan Kuppuswamy <sathyanarayanan.kuppuswamy@intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Intel MID platform setup code
 *
 * (C) Copyright 2008, 2012, 2021 Intel Corporation
 * Author: Jacob Pan (jacob.jun.pan@intel.com)
 * Author: Sathyanarayanan Kuppuswamy <sathyanarayanan.kuppuswamy@intel.com>
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) "intel_mid: " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/regulator/machine.h>
#include <linux/scatterlist.h>
<<<<<<< HEAD
#include <linux/sfi.h>
=======
>>>>>>> upstream/android-13
#include <linux/irq.h>
#include <linux/export.h>
#include <linux/notifier.h>

#include <asm/setup.h>
#include <asm/mpspec_def.h>
#include <asm/hw_irq.h>
#include <asm/apic.h>
#include <asm/io_apic.h>
#include <asm/intel-mid.h>
<<<<<<< HEAD
#include <asm/intel_mid_vrtc.h>
#include <asm/io.h>
#include <asm/i8259.h>
#include <asm/intel_scu_ipc.h>
#include <asm/apb_timer.h>
#include <asm/reboot.h>

/*
 * the clockevent devices on Moorestown/Medfield can be APBT or LAPIC clock,
 * cmdline option x86_intel_mid_timer can be used to override the configuration
 * to prefer one or the other.
 * at runtime, there are basically three timer configurations:
 * 1. per cpu apbt clock only
 * 2. per cpu always-on lapic clocks only, this is Penwell/Medfield only
 * 3. per cpu lapic clock (C3STOP) and one apbt clock, with broadcast.
 *
 * by default (without cmdline option), platform code first detects cpu type
 * to see if we are on lincroft or penwell, then set up both lapic or apbt
 * clocks accordingly.
 * i.e. by default, medfield uses configuration #2, moorestown uses #1.
 * config #3 is supported but not recommended on medfield.
 *
 * rating and feature summary:
 * lapic (with C3STOP) --------- 100
 * apbt (always-on) ------------ 110
 * lapic (always-on,ARAT) ------ 150
 */

enum intel_mid_timer_options intel_mid_timer_options;

enum intel_mid_cpu_type __intel_mid_cpu_chip;
EXPORT_SYMBOL_GPL(__intel_mid_cpu_chip);
=======
#include <asm/io.h>
#include <asm/i8259.h>
#include <asm/intel_scu_ipc.h>
#include <asm/reboot.h>

#define IPCMSG_COLD_OFF		0x80	/* Only for Tangier */
#define IPCMSG_COLD_RESET	0xF1
>>>>>>> upstream/android-13

static void intel_mid_power_off(void)
{
	/* Shut down South Complex via PWRMU */
	intel_mid_pwr_power_off();

	/* Only for Tangier, the rest will ignore this command */
<<<<<<< HEAD
	intel_scu_ipc_simple_command(IPCMSG_COLD_OFF, 1);
=======
	intel_scu_ipc_dev_simple_command(NULL, IPCMSG_COLD_OFF, 1);
>>>>>>> upstream/android-13
};

static void intel_mid_reboot(void)
{
<<<<<<< HEAD
	intel_scu_ipc_simple_command(IPCMSG_COLD_RESET, 0);
}

static void __init intel_mid_setup_bp_timer(void)
{
	apbt_time_init();
	setup_boot_APIC_clock();
=======
	intel_scu_ipc_dev_simple_command(NULL, IPCMSG_COLD_RESET, 0);
>>>>>>> upstream/android-13
}

static void __init intel_mid_time_init(void)
{
<<<<<<< HEAD
	sfi_table_parse(SFI_SIG_MTMR, NULL, NULL, sfi_parse_mtmr);

	switch (intel_mid_timer_options) {
	case INTEL_MID_TIMER_APBT_ONLY:
		break;
	case INTEL_MID_TIMER_LAPIC_APBT:
		/* Use apbt and local apic */
		x86_init.timers.setup_percpu_clockev = intel_mid_setup_bp_timer;
		x86_cpuinit.setup_percpu_clockev = setup_secondary_APIC_clock;
		return;
	default:
		if (!boot_cpu_has(X86_FEATURE_ARAT))
			break;
		/* Lapic only, no apbt */
		x86_init.timers.setup_percpu_clockev = setup_boot_APIC_clock;
		x86_cpuinit.setup_percpu_clockev = setup_secondary_APIC_clock;
		return;
	}

	x86_init.timers.setup_percpu_clockev = apbt_time_init;
=======
	/* Lapic only, no apbt */
	x86_init.timers.setup_percpu_clockev = setup_boot_APIC_clock;
	x86_cpuinit.setup_percpu_clockev = setup_secondary_APIC_clock;
>>>>>>> upstream/android-13
}

static void intel_mid_arch_setup(void)
{
<<<<<<< HEAD
	if (boot_cpu_data.x86 != 6) {
		pr_err("Unknown Intel MID CPU (%d:%d), default to Penwell\n",
			boot_cpu_data.x86, boot_cpu_data.x86_model);
		__intel_mid_cpu_chip = INTEL_MID_CPU_CHIP_PENWELL;
		goto out;
	}

	switch (boot_cpu_data.x86_model) {
	case 0x35:
		__intel_mid_cpu_chip = INTEL_MID_CPU_CHIP_CLOVERVIEW;
		break;
	case 0x3C:
	case 0x4A:
		__intel_mid_cpu_chip = INTEL_MID_CPU_CHIP_TANGIER;
		x86_platform.legacy.rtc = 1;
		break;
	case 0x27:
	default:
		__intel_mid_cpu_chip = INTEL_MID_CPU_CHIP_PENWELL;
		break;
	}

out:
=======
	switch (boot_cpu_data.x86_model) {
	case 0x3C:
	case 0x4A:
		x86_platform.legacy.rtc = 1;
		break;
	default:
		break;
	}

>>>>>>> upstream/android-13
	/*
	 * Intel MID platforms are using explicitly defined regulators.
	 *
	 * Let the regulator core know that we do not have any additional
	 * regulators left. This lets it substitute unprovided regulators with
	 * dummy ones:
	 */
	regulator_has_full_constraints();
}

/*
 * Moorestown does not have external NMI source nor port 0x61 to report
 * NMI status. The possible NMI sources are from pmu as a result of NMI
 * watchdog or lock debug. Reading io port 0x61 results in 0xff which
 * misled NMI handler.
 */
static unsigned char intel_mid_get_nmi_reason(void)
{
	return 0;
}

/*
 * Moorestown specific x86_init function overrides and early setup
 * calls.
 */
void __init x86_intel_mid_early_setup(void)
{
	x86_init.resources.probe_roms = x86_init_noop;
	x86_init.resources.reserve_resources = x86_init_noop;

	x86_init.timers.timer_init = intel_mid_time_init;
	x86_init.timers.setup_percpu_clockev = x86_init_noop;
<<<<<<< HEAD
	x86_init.timers.wallclock_init = intel_mid_rtc_init;
=======
>>>>>>> upstream/android-13

	x86_init.irqs.pre_vector_init = x86_init_noop;

	x86_init.oem.arch_setup = intel_mid_arch_setup;

<<<<<<< HEAD
	x86_cpuinit.setup_percpu_clockev = apbt_setup_secondary_clock;

=======
>>>>>>> upstream/android-13
	x86_platform.get_nmi_reason = intel_mid_get_nmi_reason;

	x86_init.pci.arch_init = intel_mid_pci_init;
	x86_init.pci.fixup_irqs = x86_init_noop;

	legacy_pic = &null_legacy_pic;

	/*
	 * Do nothing for now as everything needed done in
	 * x86_intel_mid_early_setup() below.
	 */
	x86_init.acpi.reduced_hw_early_init = x86_init_noop;

	pm_power_off = intel_mid_power_off;
	machine_ops.emergency_restart  = intel_mid_reboot;

	/* Avoid searching for BIOS MP tables */
	x86_init.mpparse.find_smp_config = x86_init_noop;
	x86_init.mpparse.get_smp_config = x86_init_uint_noop;
	set_bit(MP_BUS_ISA, mp_bus_not_pci);
}
<<<<<<< HEAD

/*
 * if user does not want to use per CPU apb timer, just give it a lower rating
 * than local apic timer and skip the late per cpu timer init.
 */
static inline int __init setup_x86_intel_mid_timer(char *arg)
{
	if (!arg)
		return -EINVAL;

	if (strcmp("apbt_only", arg) == 0)
		intel_mid_timer_options = INTEL_MID_TIMER_APBT_ONLY;
	else if (strcmp("lapic_and_apbt", arg) == 0)
		intel_mid_timer_options = INTEL_MID_TIMER_LAPIC_APBT;
	else {
		pr_warn("X86 INTEL_MID timer option %s not recognised use x86_intel_mid_timer=apbt_only or lapic_and_apbt\n",
			arg);
		return -EINVAL;
	}
	return 0;
}
__setup("x86_intel_mid_timer=", setup_x86_intel_mid_timer);
=======
>>>>>>> upstream/android-13
