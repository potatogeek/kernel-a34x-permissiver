<<<<<<< HEAD
/*
 * soc-apci-intel-cht-match.c - tables and support for CHT ACPI enumeration.
 *
 * Copyright (c) 2017, Intel Corporation.
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * soc-acpi-intel-cht-match.c - tables and support for CHT ACPI enumeration.
 *
 * Copyright (c) 2017, Intel Corporation.
>>>>>>> upstream/android-13
 */

#include <linux/dmi.h>
#include <sound/soc-acpi.h>
#include <sound/soc-acpi-intel-match.h>

static unsigned long cht_machine_id;

#define CHT_SURFACE_MACH 1

static int cht_surface_quirk_cb(const struct dmi_system_id *id)
{
	cht_machine_id = CHT_SURFACE_MACH;
	return 1;
}

static const struct dmi_system_id cht_table[] = {
	{
		.callback = cht_surface_quirk_cb,
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Microsoft Corporation"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Surface 3"),
		},
	},
	{ }
};

static struct snd_soc_acpi_mach cht_surface_mach = {
	.id = "10EC5640",
	.drv_name = "cht-bsw-rt5645",
	.fw_filename = "intel/fw_sst_22a8.bin",
	.board = "cht-bsw",
<<<<<<< HEAD
	.sof_fw_filename = "intel/sof-cht.ri",
	.sof_tplg_filename = "intel/sof-cht-rt5645.tplg",
	.asoc_plat_name = "sst-mfld-platform",
=======
	.sof_fw_filename = "sof-cht.ri",
	.sof_tplg_filename = "sof-cht-rt5645.tplg",
>>>>>>> upstream/android-13
};

static struct snd_soc_acpi_mach *cht_quirk(void *arg)
{
	struct snd_soc_acpi_mach *mach = arg;

	dmi_check_system(cht_table);

	if (cht_machine_id == CHT_SURFACE_MACH)
		return &cht_surface_mach;
	else
		return mach;
}

/* Cherryview-based platforms: CherryTrail and Braswell */
struct snd_soc_acpi_mach  snd_soc_acpi_intel_cherrytrail_machines[] = {
	{
		.id = "10EC5670",
		.drv_name = "cht-bsw-rt5672",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "cht-bsw",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-rt5670.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-rt5670.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "10EC5672",
		.drv_name = "cht-bsw-rt5672",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "cht-bsw",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-rt5670.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-rt5670.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "10EC5645",
		.drv_name = "cht-bsw-rt5645",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "cht-bsw",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-rt5645.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-rt5645.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "10EC5650",
		.drv_name = "cht-bsw-rt5645",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "cht-bsw",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-rt5645.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-rt5645.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "10EC3270",
		.drv_name = "cht-bsw-rt5645",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "cht-bsw",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-rt5645.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-rt5645.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "193C9890",
		.drv_name = "cht-bsw-max98090",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "cht-bsw",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-max98090.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-max98090.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "10508824",
		.drv_name = "cht-bsw-nau8824",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "cht-bsw",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-nau8824.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-nau8824.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "DLGS7212",
		.drv_name = "bytcht_da7213",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "bytcht_da7213",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-da7213.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-da7213.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "DLGS7213",
		.drv_name = "bytcht_da7213",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "bytcht_da7213",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-da7213.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-da7213.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "ESSX8316",
		.drv_name = "bytcht_es8316",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "bytcht_es8316",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-es8316.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-es8316.tplg",
>>>>>>> upstream/android-13
	},
	/* some CHT-T platforms rely on RT5640, use Baytrail machine driver */
	{
		.id = "10EC5640",
		.drv_name = "bytcr_rt5640",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "bytcr_rt5640",
		.machine_quirk = cht_quirk,
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-rt5640.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-rt5640.tplg",
>>>>>>> upstream/android-13
	},
	{
		.id = "10EC3276",
		.drv_name = "bytcr_rt5640",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "bytcr_rt5640",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-rt5640.tplg",
		.asoc_plat_name = "sst-mfld-platform",
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-rt5640.tplg",
	},
	{
		.id = "10EC5682",
		.drv_name = "sof_rt5682",
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-rt5682.tplg",
>>>>>>> upstream/android-13
	},
	/* some CHT-T platforms rely on RT5651, use Baytrail machine driver */
	{
		.id = "10EC5651",
		.drv_name = "bytcr_rt5651",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "bytcr_rt5651",
<<<<<<< HEAD
		.sof_fw_filename = "intel/sof-cht.ri",
		.sof_tplg_filename = "intel/sof-cht-rt5651.tplg",
		.asoc_plat_name = "sst-mfld-platform",
	},
=======
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-rt5651.tplg",
	},
	{
		.id = "14F10720",
		.drv_name = "bytcht_cx2072x",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "bytcht_cx2072x",
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-cx2072x.tplg",
	},
	{
		.id = "104C5122",
		.drv_name = "sof_pcm512x",
		.sof_fw_filename = "sof-cht.ri",
		.sof_tplg_filename = "sof-cht-src-50khz-pcm512x.tplg",
	},

>>>>>>> upstream/android-13
#if IS_ENABLED(CONFIG_SND_SOC_INTEL_BYT_CHT_NOCODEC_MACH)
	/*
	 * This is always last in the table so that it is selected only when
	 * enabled explicitly and there is no codec-related information in SSDT
	 */
	{
		.id = "808622A8",
		.drv_name = "bytcht_nocodec",
		.fw_filename = "intel/fw_sst_22a8.bin",
		.board = "bytcht_nocodec",
	},
#endif
	{},
};
EXPORT_SYMBOL_GPL(snd_soc_acpi_intel_cherrytrail_machines);
<<<<<<< HEAD

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Intel Common ACPI Match module");
=======
>>>>>>> upstream/android-13
