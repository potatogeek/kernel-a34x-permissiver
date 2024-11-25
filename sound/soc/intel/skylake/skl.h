<<<<<<< HEAD
/*
 *  skl.h - HD Audio skylake defintions.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 *  skl.h - HD Audio skylake definitions.
>>>>>>> upstream/android-13
 *
 *  Copyright (C) 2015 Intel Corp
 *  Author: Jeeja KP <jeeja.kp@intel.com>
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
<<<<<<< HEAD
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
=======
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
>>>>>>> upstream/android-13
 */

#ifndef __SOUND_SOC_SKL_H
#define __SOUND_SOC_SKL_H

#include <sound/hda_register.h>
#include <sound/hdaudio_ext.h>
<<<<<<< HEAD
#include <sound/soc.h>
#include "skl-nhlt.h"
#include "skl-ssp-clk.h"
=======
#include <sound/hda_codec.h>
#include <sound/soc.h>
#include "skl-ssp-clk.h"
#include "skl-sst-ipc.h"
>>>>>>> upstream/android-13

#define SKL_SUSPEND_DELAY 2000

#define SKL_MAX_ASTATE_CFG		3

#define AZX_PCIREG_PGCTL		0x44
#define AZX_PGCTL_LSRMD_MASK		(1 << 4)
#define AZX_PGCTL_ADSPPGD		BIT(2)
#define AZX_PCIREG_CGCTL		0x48
#define AZX_CGCTL_MISCBDCGE_MASK	(1 << 6)
#define AZX_CGCTL_ADSPDCGE		BIT(1)
/* D0I3C Register fields */
#define AZX_REG_VS_D0I3C_CIP      0x1 /* Command in progress */
#define AZX_REG_VS_D0I3C_I3       0x4 /* D0i3 enable */
#define SKL_MAX_DMACTRL_CFG	18
#define DMA_CLK_CONTROLS	1
#define DMA_TRANSMITION_START	2
#define DMA_TRANSMITION_STOP	3

<<<<<<< HEAD
#define AZX_REG_VS_EM2_L1SEN		BIT(13)

struct skl_dsp_resource {
	u32 max_mcps;
	u32 max_mem;
	u32 mcps;
	u32 mem;
};

=======
#define AZX_VS_EM2_DUM			BIT(23)
#define AZX_REG_VS_EM2_L1SEN		BIT(13)

>>>>>>> upstream/android-13
struct skl_debug;

struct skl_astate_param {
	u32 kcps;
	u32 clk_src;
};

struct skl_astate_config {
	u32 count;
<<<<<<< HEAD
	struct skl_astate_param astate_table[0];
=======
	struct skl_astate_param astate_table[];
>>>>>>> upstream/android-13
};

struct skl_fw_config {
	struct skl_astate_config *astate_cfg;
};

<<<<<<< HEAD
struct skl {
	struct hdac_bus hbus;
=======
struct skl_dev {
	struct hda_bus hbus;
>>>>>>> upstream/android-13
	struct pci_dev *pci;

	unsigned int init_done:1; /* delayed init status */
	struct platform_device *dmic_dev;
	struct platform_device *i2s_dev;
	struct platform_device *clk_dev;
	struct snd_soc_component *component;
	struct snd_soc_dai_driver *dais;

	struct nhlt_acpi_table *nhlt; /* nhlt ptr */
<<<<<<< HEAD
	struct skl_sst *skl_sst; /* sst skl ctx */

	struct skl_dsp_resource resource;
=======

>>>>>>> upstream/android-13
	struct list_head ppl_list;
	struct list_head bind_list;

	const char *fw_name;
	char tplg_name[64];
	unsigned short pci_id;
<<<<<<< HEAD
	const struct firmware *tplg;
=======
>>>>>>> upstream/android-13

	int supend_active;

	struct work_struct probe_work;

	struct skl_debug *debugfs;
	u8 nr_modules;
	struct skl_module **modules;
	bool use_tplg_pcm;
	struct skl_fw_config cfg;
	struct snd_soc_acpi_mach *mach;
<<<<<<< HEAD
};

#define skl_to_bus(s)  (&(s)->hbus)
#define bus_to_skl(bus) container_of(bus, struct skl, hbus)
=======

	struct device *dev;
	struct sst_dsp *dsp;

	/* boot */
	wait_queue_head_t boot_wait;
	bool boot_complete;

	/* module load */
	wait_queue_head_t mod_load_wait;
	bool mod_load_complete;
	bool mod_load_status;

	/* IPC messaging */
	struct sst_generic_ipc ipc;

	/* callback for miscbdge */
	void (*enable_miscbdcge)(struct device *dev, bool enable);
	/* Is CGCTL.MISCBDCGE disabled */
	bool miscbdcg_disabled;

	/* Populate module information */
	struct list_head uuid_list;

	/* Is firmware loaded */
	bool fw_loaded;

	/* first boot ? */
	bool is_first_boot;

	/* multi-core */
	struct skl_dsp_cores cores;

	/* library info */
	struct skl_lib_info  lib_info[SKL_MAX_LIB];
	int lib_count;

	/* Callback to update D0i3C register */
	void (*update_d0i3c)(struct device *dev, bool enable);

	struct skl_d0i3_data d0i3;

	const struct skl_dsp_ops *dsp_ops;

	/* Callback to update dynamic clock and power gating registers */
	void (*clock_power_gating)(struct device *dev, bool enable);
};

#define skl_to_bus(s)  (&(s)->hbus.core)
#define bus_to_skl(bus) container_of(bus, struct skl_dev, hbus.core)

#define skl_to_hbus(s) (&(s)->hbus)
#define hbus_to_skl(hbus) container_of((hbus), struct skl_dev, (hbus))
>>>>>>> upstream/android-13

/* to pass dai dma data */
struct skl_dma_params {
	u32 format;
	u8 stream_tag;
};

struct skl_machine_pdata {
<<<<<<< HEAD
	u32 dmic_num;
=======
>>>>>>> upstream/android-13
	bool use_tplg_pcm; /* use dais and dai links from topology */
};

struct skl_dsp_ops {
	int id;
	unsigned int num_cores;
	struct skl_dsp_loader_ops (*loader_ops)(void);
	int (*init)(struct device *dev, void __iomem *mmio_base,
			int irq, const char *fw_name,
			struct skl_dsp_loader_ops loader_ops,
<<<<<<< HEAD
			struct skl_sst **skl_sst);
	int (*init_fw)(struct device *dev, struct skl_sst *ctx);
	void (*cleanup)(struct device *dev, struct skl_sst *ctx);
=======
			struct skl_dev **skl_sst);
	int (*init_fw)(struct device *dev, struct skl_dev *skl);
	void (*cleanup)(struct device *dev, struct skl_dev *skl);
>>>>>>> upstream/android-13
};

int skl_platform_unregister(struct device *dev);
int skl_platform_register(struct device *dev);

<<<<<<< HEAD
struct nhlt_acpi_table *skl_nhlt_init(struct device *dev);
void skl_nhlt_free(struct nhlt_acpi_table *addr);
struct nhlt_specific_cfg *skl_get_ep_blob(struct skl *skl, u32 instance,
					u8 link_type, u8 s_fmt, u8 no_ch,
					u32 s_rate, u8 dirn, u8 dev_type);

int skl_get_dmic_geo(struct skl *skl);
int skl_nhlt_update_topology_bin(struct skl *skl);
int skl_init_dsp(struct skl *skl);
int skl_free_dsp(struct skl *skl);
int skl_suspend_late_dsp(struct skl *skl);
int skl_suspend_dsp(struct skl *skl);
int skl_resume_dsp(struct skl *skl);
void skl_cleanup_resources(struct skl *skl);
const struct skl_dsp_ops *skl_get_dsp_ops(int pci_id);
void skl_update_d0i3c(struct device *dev, bool enable);
int skl_nhlt_create_sysfs(struct skl *skl);
void skl_nhlt_remove_sysfs(struct skl *skl);
void skl_get_clks(struct skl *skl, struct skl_ssp_clk *ssp_clks);
struct skl_clk_parent_src *skl_get_parent_clk(u8 clk_id);
int skl_dsp_set_dma_control(struct skl_sst *ctx, u32 *caps,
=======
struct nhlt_specific_cfg *skl_get_ep_blob(struct skl_dev *skl, u32 instance,
					u8 link_type, u8 s_fmt, u8 num_ch,
					u32 s_rate, u8 dirn, u8 dev_type);

int skl_nhlt_update_topology_bin(struct skl_dev *skl);
int skl_init_dsp(struct skl_dev *skl);
int skl_free_dsp(struct skl_dev *skl);
int skl_suspend_late_dsp(struct skl_dev *skl);
int skl_suspend_dsp(struct skl_dev *skl);
int skl_resume_dsp(struct skl_dev *skl);
void skl_cleanup_resources(struct skl_dev *skl);
const struct skl_dsp_ops *skl_get_dsp_ops(int pci_id);
void skl_update_d0i3c(struct device *dev, bool enable);
int skl_nhlt_create_sysfs(struct skl_dev *skl);
void skl_nhlt_remove_sysfs(struct skl_dev *skl);
void skl_get_clks(struct skl_dev *skl, struct skl_ssp_clk *ssp_clks);
struct skl_clk_parent_src *skl_get_parent_clk(u8 clk_id);
int skl_dsp_set_dma_control(struct skl_dev *skl, u32 *caps,
>>>>>>> upstream/android-13
				u32 caps_size, u32 node_id);

struct skl_module_cfg;

#ifdef CONFIG_DEBUG_FS
<<<<<<< HEAD
struct skl_debug *skl_debugfs_init(struct skl *skl);
=======
struct skl_debug *skl_debugfs_init(struct skl_dev *skl);
void skl_debugfs_exit(struct skl_dev *skl);
>>>>>>> upstream/android-13
void skl_debug_init_module(struct skl_debug *d,
			struct snd_soc_dapm_widget *w,
			struct skl_module_cfg *mconfig);
#else
<<<<<<< HEAD
static inline struct skl_debug *skl_debugfs_init(struct skl *skl)
{
	return NULL;
}
=======
static inline struct skl_debug *skl_debugfs_init(struct skl_dev *skl)
{
	return NULL;
}

static inline void skl_debugfs_exit(struct skl_dev *skl)
{}

>>>>>>> upstream/android-13
static inline void skl_debug_init_module(struct skl_debug *d,
					 struct snd_soc_dapm_widget *w,
					 struct skl_module_cfg *mconfig)
{}
#endif

#endif /* __SOUND_SOC_SKL_H */
