<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
/*
 * soc-apci-intel-glk-match.c - tables and support for GLK ACPI enumeration.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * soc-acpi-intel-glk-match.c - tables and support for GLK ACPI enumeration.
>>>>>>> upstream/android-13
 *
 * Copyright (c) 2018, Intel Corporation.
 *
 */

#include <sound/soc-acpi.h>
#include <sound/soc-acpi-intel-match.h>

static struct snd_soc_acpi_codecs glk_codecs = {
	.num_codecs = 1,
	.codecs = {"MX98357A"}
};

struct snd_soc_acpi_mach snd_soc_acpi_intel_glk_machines[] = {
	{
		.id = "INT343A",
		.drv_name = "glk_alc298s_i2s",
		.fw_filename = "intel/dsp_fw_glk.bin",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-glk.ri",
		.sof_tplg_filename = "intel/sof-glk-alc298.tplg",
		.asoc_plat_name = "0000:00:0e.0",
	},
	{
		.id = "DLGS7219",
		.drv_name = "glk_da7219_max98357a",
		.fw_filename = "intel/dsp_fw_glk.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &glk_codecs,
		.sof_fw_filename = "intel/sof-glk.ri",
		.sof_tplg_filename = "intel/sof-glk-da7219.tplg",
		.asoc_plat_name = "0000:00:0e.0",
	},
	{},
};
EXPORT_SYMBOL_GPL(snd_soc_acpi_intel_glk_machines);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Intel Common ACPI Match module");
=======
		.sof_fw_filename = "sof-glk.ri",
		.sof_tplg_filename = "sof-glk-alc298.tplg",
	},
	{
		.id = "DLGS7219",
		.drv_name = "glk_da7219_mx98357a",
		.fw_filename = "intel/dsp_fw_glk.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &glk_codecs,
		.sof_fw_filename = "sof-glk.ri",
		.sof_tplg_filename = "sof-glk-da7219.tplg",
	},
	{
		.id = "10EC5682",
		.drv_name = "glk_rt5682_mx98357a",
		.fw_filename = "intel/dsp_fw_glk.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &glk_codecs,
		.sof_fw_filename = "sof-glk.ri",
		.sof_tplg_filename = "sof-glk-rt5682.tplg",
	},
	{
		.id = "10134242",
		.drv_name = "glk_cs4242_mx98357a",
		.fw_filename = "intel/dsp_fw_glk.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &glk_codecs,
		.sof_fw_filename = "sof-glk.ri",
		.sof_tplg_filename = "sof-glk-cs42l42.tplg",
	},

	{},
};
EXPORT_SYMBOL_GPL(snd_soc_acpi_intel_glk_machines);
>>>>>>> upstream/android-13
