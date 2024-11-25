<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
/*
 * soc-apci-intel-bxt-match.c - tables and support for BXT ACPI enumeration.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * soc-acpi-intel-bxt-match.c - tables and support for BXT ACPI enumeration.
>>>>>>> upstream/android-13
 *
 * Copyright (c) 2018, Intel Corporation.
 *
 */

<<<<<<< HEAD
#include <sound/soc-acpi.h>
#include <sound/soc-acpi-intel-match.h>

=======
#include <linux/dmi.h>
#include <sound/soc-acpi.h>
#include <sound/soc-acpi-intel-match.h>

enum {
	APL_RVP,
};

static const struct dmi_system_id apl_table[] = {
	{
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Intel Corp."),
			DMI_MATCH(DMI_BOARD_NAME, "Apollolake RVP1A"),
		},
		.driver_data = (void *)(APL_RVP),
	},
	{}
};

static struct snd_soc_acpi_mach *apl_quirk(void *arg)
{
	struct snd_soc_acpi_mach *mach = arg;
	const struct dmi_system_id *dmi_id;
	unsigned long apl_machine_id;

	dmi_id = dmi_first_match(apl_table);
	if (dmi_id) {
		apl_machine_id = (unsigned long)dmi_id->driver_data;
		if (apl_machine_id == APL_RVP)
			return NULL;
	}

	return mach;
}

>>>>>>> upstream/android-13
static struct snd_soc_acpi_codecs bxt_codecs = {
	.num_codecs = 1,
	.codecs = {"MX98357A"}
};

struct snd_soc_acpi_mach snd_soc_acpi_intel_bxt_machines[] = {
	{
		.id = "INT343A",
		.drv_name = "bxt_alc298s_i2s",
		.fw_filename = "intel/dsp_fw_bxtn.bin",
<<<<<<< HEAD
	},
	{
		.id = "DLGS7219",
		.drv_name = "bxt_da7219_max98357a",
		.fw_filename = "intel/dsp_fw_bxtn.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &bxt_codecs,
		.sof_fw_filename = "intel/sof-apl.ri",
		.sof_tplg_filename = "intel/sof-apl-da7219.tplg",
		.asoc_plat_name = "0000:00:0e.0",
	},
	{
		.id = "104C5122",
		.drv_name = "bxt-pcm512x",
		.sof_fw_filename = "intel/sof-apl.ri",
		.sof_tplg_filename = "intel/sof-apl-pcm512x.tplg",
		.asoc_plat_name = "0000:00:0e.0",
	},
	{
		.id = "1AEC8804",
		.drv_name = "bxt-wm8804",
		.sof_fw_filename = "intel/sof-apl.ri",
		.sof_tplg_filename = "intel/sof-apl-wm8804.tplg",
		.asoc_plat_name = "0000:00:0e.0",
=======
		.sof_fw_filename = "sof-apl.ri",
		.sof_tplg_filename = "sof-apl-rt298.tplg",
	},
	{
		.id = "DLGS7219",
		.drv_name = "bxt_da7219_mx98357a",
		.fw_filename = "intel/dsp_fw_bxtn.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &bxt_codecs,
		.sof_fw_filename = "sof-apl.ri",
		.sof_tplg_filename = "sof-apl-da7219.tplg",
	},
	{
		.id = "104C5122",
		.drv_name = "sof_pcm512x",
		.sof_fw_filename = "sof-apl.ri",
		.sof_tplg_filename = "sof-apl-pcm512x.tplg",
	},
	{
		.id = "1AEC8804",
		.drv_name = "sof-wm8804",
		.sof_fw_filename = "sof-apl.ri",
		.sof_tplg_filename = "sof-apl-wm8804.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "INT34C3",
		.drv_name = "bxt_tdf8532",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-apl.ri",
		.sof_tplg_filename = "intel/sof-apl-tdf8532.tplg",
		.asoc_plat_name = "0000:00:0e.0",
=======
		.machine_quirk = apl_quirk,
		.sof_fw_filename = "sof-apl.ri",
		.sof_tplg_filename = "sof-apl-tdf8532.tplg",
>>>>>>> upstream/android-13
	},
	{},
};
EXPORT_SYMBOL_GPL(snd_soc_acpi_intel_bxt_machines);
<<<<<<< HEAD

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Intel Common ACPI Match module");
=======
>>>>>>> upstream/android-13
