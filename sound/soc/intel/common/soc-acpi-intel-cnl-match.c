<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
/*
 * soc-apci-intel-cnl-match.c - tables and support for CNL ACPI enumeration.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * soc-acpi-intel-cnl-match.c - tables and support for CNL ACPI enumeration.
>>>>>>> upstream/android-13
 *
 * Copyright (c) 2018, Intel Corporation.
 *
 */

#include <sound/soc-acpi.h>
#include <sound/soc-acpi-intel-match.h>
#include "../skylake/skl.h"
<<<<<<< HEAD
=======
#include "soc-acpi-intel-sdw-mockup-match.h"
>>>>>>> upstream/android-13

static struct skl_machine_pdata cnl_pdata = {
	.use_tplg_pcm = true,
};

struct snd_soc_acpi_mach snd_soc_acpi_intel_cnl_machines[] = {
	{
		.id = "INT34C2",
		.drv_name = "cnl_rt274",
		.fw_filename = "intel/dsp_fw_cnl.bin",
		.pdata = &cnl_pdata,
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cnl.ri",
		.sof_tplg_filename = "intel/sof-cnl-rt274.tplg",
		.asoc_plat_name = "0000:00:1f.3",
=======
		.sof_fw_filename = "sof-cnl.ri",
		.sof_tplg_filename = "sof-cnl-rt274.tplg",
>>>>>>> upstream/android-13
	},
	{},
};
EXPORT_SYMBOL_GPL(snd_soc_acpi_intel_cnl_machines);

<<<<<<< HEAD
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Intel Common ACPI Match module");
=======
static const struct snd_soc_acpi_endpoint single_endpoint = {
	.num = 0,
	.aggregated = 0,
	.group_position = 0,
	.group_id = 0,
};

static const struct snd_soc_acpi_adr_device rt5682_2_adr[] = {
	{
		.adr = 0x000220025D568200ull,
		.num_endpoints = 1,
		.endpoints = &single_endpoint,
		.name_prefix = "rt5682"
	}
};

static const struct snd_soc_acpi_link_adr up_extreme_rt5682_2[] = {
	{
		.mask = BIT(2),
		.num_adr = ARRAY_SIZE(rt5682_2_adr),
		.adr_d = rt5682_2_adr,
	},
	{}
};

struct snd_soc_acpi_mach snd_soc_acpi_intel_cnl_sdw_machines[] = {
	{
		.link_mask = BIT(2),
		.links = up_extreme_rt5682_2,
		.drv_name = "sof_sdw",
		.sof_fw_filename = "sof-cnl.ri",
		.sof_tplg_filename = "sof-cnl-rt5682-sdw2.tplg"
	},
	{
		.link_mask = GENMASK(3, 0),
		.links = sdw_mockup_headset_2amps_mic,
		.drv_name = "sof_sdw",
		.sof_fw_filename = "sof-cnl.ri",
		.sof_tplg_filename = "sof-cml-rt711-rt1308-rt715.tplg",
	},
	{
		.link_mask = BIT(0) | BIT(1) | BIT(3),
		.links = sdw_mockup_headset_1amp_mic,
		.drv_name = "sof_sdw",
		.sof_fw_filename = "sof-cnl.ri",
		.sof_tplg_filename = "sof-cml-rt711-rt1308-mono-rt715.tplg",
	},
	{}
};
EXPORT_SYMBOL_GPL(snd_soc_acpi_intel_cnl_sdw_machines);
>>>>>>> upstream/android-13
