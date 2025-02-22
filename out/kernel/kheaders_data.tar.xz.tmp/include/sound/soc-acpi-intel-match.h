/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2017, Intel Corporation. All rights reserved.
 */

#ifndef __LINUX_SND_SOC_ACPI_INTEL_MATCH_H
#define __LINUX_SND_SOC_ACPI_INTEL_MATCH_H

#include <linux/module.h>
#include <linux/stddef.h>
#include <linux/acpi.h>


extern struct snd_soc_acpi_mach snd_soc_acpi_intel_haswell_machines[];
extern struct snd_soc_acpi_mach snd_soc_acpi_intel_broadwell_machines[];
extern struct snd_soc_acpi_mach snd_soc_acpi_intel_baytrail_legacy_machines[];
extern struct snd_soc_acpi_mach snd_soc_acpi_intel_baytrail_machines[];
extern struct snd_soc_acpi_mach snd_soc_acpi_intel_cherrytrail_machines[];
extern struct snd_soc_acpi_mach snd_soc_acpi_intel_skl_machines[];
extern struct snd_soc_acpi_mach snd_soc_acpi_intel_kbl_machines[];
extern struct snd_soc_acpi_mach snd_soc_acpi_intel_bxt_machines[];
extern struct snd_soc_acpi_mach snd_soc_acpi_intel_glk_machines[];
extern struct snd_soc_acpi_mach snd_soc_acpi_intel_cnl_machines[];

#endif
