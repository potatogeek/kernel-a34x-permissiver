<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2015-2016 MediaTek Inc.
 * Author: Honghui Zhang <honghui.zhang@mediatek.com>
 */

#ifndef _MTK_IOMMU_H_
#define _MTK_IOMMU_H_

#include <linux/clk.h>
#include <linux/component.h>
#include <linux/device.h>
#include <linux/io.h>
<<<<<<< HEAD
#include <linux/iommu.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <soc/mediatek/smi.h>

#include "io-pgtable.h"

struct mtk_iommu_suspend_reg {
	u32				standard_axi_mode;
=======
#include <linux/io-pgtable.h>
#include <linux/iommu.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/dma-mapping.h>
#include <soc/mediatek/smi.h>
#include <dt-bindings/memory/mtk-memory-port.h>

#define MTK_LARB_COM_MAX	8
#define MTK_LARB_SUBCOM_MAX	4

#define MTK_IOMMU_GROUP_MAX	8

struct mtk_iommu_suspend_reg {
	union {
		u32			standard_axi_mode;/* v1 */
		u32			misc_ctrl;/* v2 */
	};
>>>>>>> upstream/android-13
	u32				dcm_dis;
	u32				ctrl_reg;
	u32				int_control0;
	u32				int_main_control;
	u32				ivrp_paddr;
<<<<<<< HEAD
	u32				vld_pa_range;
	u32				pt_base;
	u32				wr_ctrl;
=======
	u32				vld_pa_rng;
	u32				wr_len_ctrl;
>>>>>>> upstream/android-13
};

enum mtk_iommu_plat {
	M4U_MT2701,
	M4U_MT2712,
<<<<<<< HEAD
	M4U_MT8167,
	M4U_MT8168,
	M4U_MT8173,
	M4U_MT8183,
	iommu_mt6xxx_v0,
};

struct mtk_iommu_resv_iova_region;
struct mtk_iommu_plat_data {
	enum mtk_iommu_plat m4u_plat;
	bool has_4gb_mode;
	int iommu_cnt;
	/* The larb-id may be remapped in the smi-common. */
	bool larbid_remap_enable;
	unsigned int larbid_in_common[MTK_LARB_NR_MAX];

	/* reserve/dir-mapping iova region data */
	const char spec_device_comp[32];
	const unsigned int spec_cnt;
	const struct mtk_iommu_resv_iova_region *spec_region;
=======
	M4U_MT6779,
	M4U_MT8167,
	M4U_MT8173,
	M4U_MT8183,
	M4U_MT8192,
};

struct mtk_iommu_iova_region;

struct mtk_iommu_plat_data {
	enum mtk_iommu_plat m4u_plat;
	u32                 flags;
	u32                 inv_sel_reg;

	unsigned int				iova_region_nr;
	const struct mtk_iommu_iova_region	*iova_region;
	unsigned char       larbid_remap[MTK_LARB_COM_MAX][MTK_LARB_SUBCOM_MAX];
>>>>>>> upstream/android-13
};

struct mtk_iommu_domain;

<<<<<<< HEAD
#ifdef CONFIG_MTK_IOMMU_V2
struct mtk_iommu_pgtable {
	spinlock_t			pgtlock; /* lock for page table */
	struct io_pgtable_cfg		cfg;
	struct io_pgtable_ops		*iop;
	struct list_head		m4u_dom;
	spinlock_t	domain_lock; /* lock for page table */
	unsigned int domain_count;
	unsigned int init_domain_id;
};

struct mtk_iommu_domain {
	unsigned int		id;
	int		owner;
	struct iommu_domain		domain;
	struct iommu_group		*group;
#ifndef CONFIG_ARM64
	struct dma_iommu_mapping *mapping;
	unsigned int		resv_status;
#endif
	struct mtk_iommu_pgtable	*pgtable;
	struct mtk_iommu_data *data;
	struct list_head list;
};

#define IOMMU_CLK_ID_COUNT (2)
struct mtk_iommu_clks {
	unsigned int	nr_clks;
	struct clk *clks[IOMMU_CLK_ID_COUNT];
	unsigned int	nr_powers;
	struct clk *powers[IOMMU_CLK_ID_COUNT];
};
#endif

#define MTK_IOMMU_BANK_NODE_COUNT (3)
struct mtk_iommu_data {
	void __iomem *base;
	int irq;
	void __iomem *base_sec;
	void __iomem *base_bank[MTK_IOMMU_BANK_NODE_COUNT];
	struct device *dev;
	struct clk *bclk;
	phys_addr_t protect_base; /* protect memory base */
	struct mtk_iommu_suspend_reg reg;
#ifdef CONFIG_MTK_IOMMU_V2
	struct mtk_iommu_pgtable	*pgtable;
	struct mtk_iommu_clks		*m4u_clks;
	spinlock_t     reg_lock;
	bool poweron;
	unsigned long isr_ref;
	struct timer_list iommu_isr_pause_timer;
#else
	struct mtk_iommu_domain	*m4u_dom;
	struct iommu_group *m4u_group;
	bool tlb_flush_active;
#endif
	struct mtk_smi_iommu smi_imu; /* SMI larb iommu info */
	bool enable_4GB;   /* Dram is over 4gb */

	struct iommu_device iommu;
	const struct mtk_iommu_plat_data *plat_data;

	struct list_head list;
	unsigned int m4uid;
=======
struct mtk_iommu_data {
	void __iomem			*base;
	int				irq;
	struct device			*dev;
	struct clk			*bclk;
	phys_addr_t			protect_base; /* protect memory base */
	struct mtk_iommu_suspend_reg	reg;
	struct mtk_iommu_domain		*m4u_dom;
	struct iommu_group		*m4u_group[MTK_IOMMU_GROUP_MAX];
	bool                            enable_4GB;
	spinlock_t			tlb_lock; /* lock for tlb range flush */

	struct iommu_device		iommu;
	const struct mtk_iommu_plat_data *plat_data;
	struct device			*smicomm_dev;

	struct dma_iommu_mapping	*mapping; /* For mtk_iommu_v1.c */

	struct list_head		list;
	struct mtk_smi_larb_iommu	larb_imu[MTK_LARB_NR_MAX];
>>>>>>> upstream/android-13
};

static inline int compare_of(struct device *dev, void *data)
{
	return dev->of_node == data;
}

static inline void release_of(struct device *dev, void *data)
{
	of_node_put(data);
}

static inline int mtk_iommu_bind(struct device *dev)
{
	struct mtk_iommu_data *data = dev_get_drvdata(dev);

<<<<<<< HEAD
	return component_bind_all(dev, &data->smi_imu);
=======
	return component_bind_all(dev, &data->larb_imu);
>>>>>>> upstream/android-13
}

static inline void mtk_iommu_unbind(struct device *dev)
{
	struct mtk_iommu_data *data = dev_get_drvdata(dev);

<<<<<<< HEAD
	component_unbind_all(dev, &data->smi_imu);
=======
	component_unbind_all(dev, &data->larb_imu);
>>>>>>> upstream/android-13
}

#endif
