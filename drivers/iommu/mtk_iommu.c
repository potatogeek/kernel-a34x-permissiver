<<<<<<< HEAD
/*
 * Copyright (c) 2015-2016 MediaTek Inc.
 * Author: Yong Wu <yong.wu@mediatek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/bootmem.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015-2016 MediaTek Inc.
 * Author: Yong Wu <yong.wu@mediatek.com>
 */
#include <linux/bitfield.h>
>>>>>>> upstream/android-13
#include <linux/bug.h>
#include <linux/clk.h>
#include <linux/component.h>
#include <linux/device.h>
<<<<<<< HEAD
#include <linux/dma-iommu.h>
=======
#include <linux/dma-direct.h>
>>>>>>> upstream/android-13
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/iommu.h>
#include <linux/iopoll.h>
#include <linux/list.h>
<<<<<<< HEAD
#include <linux/of_address.h>
#include <linux/of_iommu.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <asm/barrier.h>
#ifndef CONFIG_ARM64
#include <asm/dma-iommu.h>
#endif
#include <soc/mediatek/smi.h>

#include "mtk_iommu.h"
#ifdef CONFIG_MTK_PSEUDO_M4U
#include "pseudo_m4u.h"
#endif
=======
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/soc/mediatek/infracfg.h>
#include <asm/barrier.h>
#include <soc/mediatek/smi.h>

#include "mtk_iommu.h"
>>>>>>> upstream/android-13

#define REG_MMU_PT_BASE_ADDR			0x000
#define MMU_PT_ADDR_MASK			GENMASK(31, 7)

#define REG_MMU_INVALIDATE			0x020
#define F_ALL_INVLD				0x2
#define F_MMU_INV_RANGE				0x1

#define REG_MMU_INVLD_START_A			0x024
#define REG_MMU_INVLD_END_A			0x028

<<<<<<< HEAD
#define REG_MMU_INV_SEL				0x038
#define F_INVLD_EN0				BIT(0)
#define F_INVLD_EN1				BIT(1)

#define REG_MMU_STANDARD_AXI_MODE		0x048
#define REG_MMU_DCM_DIS				0x050

#define REG_MMU_CTRL_REG			0x110
#define F_MMU_PREFETCH_RT_REPLACE_MOD		BIT(4)
#define F_MMU_TF_PROTECT_SEL_SHIFT(data) \
	((data)->plat_data->m4u_plat == M4U_MT8173 ? 5 : 4)
/* It's named by F_MMU_TF_PROT_SEL in mt2712. */
#define F_MMU_TF_PROTECT_SEL(prot, data) \
	(((prot) & 0x3) << F_MMU_TF_PROTECT_SEL_SHIFT(data))
=======
#define REG_MMU_INV_SEL_GEN2			0x02c
#define REG_MMU_INV_SEL_GEN1			0x038
#define F_INVLD_EN0				BIT(0)
#define F_INVLD_EN1				BIT(1)

#define REG_MMU_MISC_CTRL			0x048
#define F_MMU_IN_ORDER_WR_EN_MASK		(BIT(1) | BIT(17))
#define F_MMU_STANDARD_AXI_MODE_MASK		(BIT(3) | BIT(19))

#define REG_MMU_DCM_DIS				0x050
#define REG_MMU_WR_LEN_CTRL			0x054
#define F_MMU_WR_THROT_DIS_MASK			(BIT(5) | BIT(21))

#define REG_MMU_CTRL_REG			0x110
#define F_MMU_TF_PROT_TO_PROGRAM_ADDR		(2 << 4)
#define F_MMU_PREFETCH_RT_REPLACE_MOD		BIT(4)
#define F_MMU_TF_PROT_TO_PROGRAM_ADDR_MT8173	(2 << 5)
>>>>>>> upstream/android-13

#define REG_MMU_IVRP_PADDR			0x114

#define REG_MMU_VLD_PA_RNG			0x118
#define F_MMU_VLD_PA_RNG(EA, SA)		(((EA) << 8) | (SA))

#define REG_MMU_INT_CONTROL0			0x120
#define F_L2_MULIT_HIT_EN			BIT(0)
#define F_TABLE_WALK_FAULT_INT_EN		BIT(1)
#define F_PREETCH_FIFO_OVERFLOW_INT_EN		BIT(2)
#define F_MISS_FIFO_OVERFLOW_INT_EN		BIT(3)
#define F_PREFETCH_FIFO_ERR_INT_EN		BIT(5)
#define F_MISS_FIFO_ERR_INT_EN			BIT(6)
#define F_INT_CLR_BIT				BIT(12)

<<<<<<< HEAD
#define REG_MMU_INT_MAIN_CONTROL		0x124 /* mmu0 | mmu1 */
=======
#define REG_MMU_INT_MAIN_CONTROL		0x124
						/* mmu0 | mmu1 */
>>>>>>> upstream/android-13
#define F_INT_TRANSLATION_FAULT			(BIT(0) | BIT(7))
#define F_INT_MAIN_MULTI_HIT_FAULT		(BIT(1) | BIT(8))
#define F_INT_INVALID_PA_FAULT			(BIT(2) | BIT(9))
#define F_INT_ENTRY_REPLACEMENT_FAULT		(BIT(3) | BIT(10))
#define F_INT_TLB_MISS_FAULT			(BIT(4) | BIT(11))
#define F_INT_MISS_TRANSACTION_FIFO_FAULT	(BIT(5) | BIT(12))
#define F_INT_PRETETCH_TRANSATION_FIFO_FAULT	(BIT(6) | BIT(13))

#define REG_MMU_CPE_DONE			0x12C

#define REG_MMU_FAULT_ST1			0x134
#define F_REG_MMU0_FAULT_MASK			GENMASK(6, 0)
#define F_REG_MMU1_FAULT_MASK			GENMASK(13, 7)

#define REG_MMU0_FAULT_VA			0x13c
<<<<<<< HEAD
=======
#define F_MMU_INVAL_VA_31_12_MASK		GENMASK(31, 12)
#define F_MMU_INVAL_VA_34_32_MASK		GENMASK(11, 9)
#define F_MMU_INVAL_PA_34_32_MASK		GENMASK(8, 6)
>>>>>>> upstream/android-13
#define F_MMU_FAULT_VA_WRITE_BIT		BIT(1)
#define F_MMU_FAULT_VA_LAYER_BIT		BIT(0)

#define REG_MMU0_INVLD_PA			0x140
#define REG_MMU1_FAULT_VA			0x144
#define REG_MMU1_INVLD_PA			0x148
#define REG_MMU0_INT_ID				0x150
#define REG_MMU1_INT_ID				0x154
<<<<<<< HEAD
#define F_MMU_INT_ID_LARB_ID(a)			(((a) >> 7) & 0x7)
#define F_MMU_INT_ID_PORT_ID(a)			(((a) >> 2) & 0x1f)
#define F_MMU_INT_ID_LARB_ID_MT8168(a)		(((a) >> 8) & 0x7)
#define F_MMU_INT_ID_PORT_ID_MT8168(a)		(((a) >> 2) & 0x3f)

#define MTK_PROTECT_PA_ALIGN			128

/* reserve iova region for VPU */
#define IOVPU_RANGE_START		0x7DA00000
#define IOVPU_RANGE_LEN			0x04C00000

/*
 * Get the local arbiter ID and the portid within the larb arbiter
 * from mtk_m4u_id which is defined by MTK_M4U_ID.
 */
#define MTK_M4U_TO_LARB(id)		(((id) >> 5) & 0xf)
#define MTK_M4U_TO_PORT(id)		((id) & 0x1f)

struct mtk_iommu_domain {
	spinlock_t			pgtlock; /* lock for page table */

	struct io_pgtable_cfg		cfg;
	struct io_pgtable_ops		*iop;

	struct iommu_domain		domain;

#if (defined(CONFIG_MTK_PSEUDO_M4U) && defined(M4U_TEE_SERVICE_ENABLE))
	unsigned int			sec_iova_size;
#endif
};

struct mtk_iommu_resv_iova_region {
	unsigned long iova_base;
	size_t iova_size;
	enum iommu_resv_type type;
	void (*get_resv_data)(unsigned long *base, size_t *size,
			      struct mtk_iommu_data *data);
};

static struct iommu_ops mtk_iommu_ops;
=======
#define F_MMU_INT_ID_COMM_ID(a)			(((a) >> 9) & 0x7)
#define F_MMU_INT_ID_SUB_COMM_ID(a)		(((a) >> 7) & 0x3)
#define F_MMU_INT_ID_LARB_ID(a)			(((a) >> 7) & 0x7)
#define F_MMU_INT_ID_PORT_ID(a)			(((a) >> 2) & 0x1f)

#define MTK_PROTECT_PA_ALIGN			256

#define HAS_4GB_MODE			BIT(0)
/* HW will use the EMI clock if there isn't the "bclk". */
#define HAS_BCLK			BIT(1)
#define HAS_VLD_PA_RNG			BIT(2)
#define RESET_AXI			BIT(3)
#define OUT_ORDER_WR_EN			BIT(4)
#define HAS_SUB_COMM			BIT(5)
#define WR_THROT_EN			BIT(6)
#define HAS_LEGACY_IVRP_PADDR		BIT(7)
#define IOVA_34_EN			BIT(8)
#define PGTABLE_PA_35_EN		BIT(9)

#define MTK_IOMMU_HAS_FLAG(pdata, _x) \
		((((pdata)->flags) & (_x)) == (_x))

struct mtk_iommu_domain {
	struct io_pgtable_cfg		cfg;
	struct io_pgtable_ops		*iop;
	u32				ttbr;

	struct mtk_iommu_data		*data;
	struct iommu_domain		domain;
};

static const struct iommu_ops mtk_iommu_ops;

static int mtk_iommu_hw_init(const struct mtk_iommu_data *data);

#define MTK_IOMMU_TLB_ADDR(iova) ({					\
	dma_addr_t _addr = iova;					\
	((lower_32_bits(_addr) & GENMASK(31, 12)) | upper_32_bits(_addr));\
})
>>>>>>> upstream/android-13

/*
 * In M4U 4GB mode, the physical address is remapped as below:
 *
 * CPU Physical address:
 * ====================
 *
 * 0      1G       2G     3G       4G     5G
 * |---A---|---B---|---C---|---D---|---E---|
 * +--I/O--+------------Memory-------------+
 *
 * IOMMU output physical address:
 *  =============================
 *
 *                                 4G      5G     6G      7G      8G
 *                                 |---E---|---B---|---C---|---D---|
 *                                 +------------Memory-------------+
 *
 * The Region 'A'(I/O) can NOT be mapped by M4U; For Region 'B'/'C'/'D', the
 * bit32 of the CPU physical address always is needed to set, and for Region
 * 'E', the CPU physical address keep as is.
 * Additionally, The iommu consumers always use the CPU phyiscal address.
 */
<<<<<<< HEAD
#define MTK_IOMMU_4GB_MODE_REMAP_BASE	 0x40000000
=======
#define MTK_IOMMU_4GB_MODE_REMAP_BASE	 0x140000000UL
>>>>>>> upstream/android-13

static LIST_HEAD(m4ulist);	/* List all the M4U HWs */

#define for_each_m4u(data)	list_for_each_entry(data, &m4ulist, list)

<<<<<<< HEAD
=======
struct mtk_iommu_iova_region {
	dma_addr_t		iova_base;
	unsigned long long	size;
};

static const struct mtk_iommu_iova_region single_domain[] = {
	{.iova_base = 0,		.size = SZ_4G},
};

static const struct mtk_iommu_iova_region mt8192_multi_dom[] = {
	{ .iova_base = 0x0,		.size = SZ_4G},		/* disp: 0 ~ 4G */
	#if IS_ENABLED(CONFIG_ARCH_DMA_ADDR_T_64BIT)
	{ .iova_base = SZ_4G,		.size = SZ_4G},		/* vdec: 4G ~ 8G */
	{ .iova_base = SZ_4G * 2,	.size = SZ_4G},		/* CAM/MDP: 8G ~ 12G */
	{ .iova_base = 0x240000000ULL,	.size = 0x4000000},	/* CCU0 */
	{ .iova_base = 0x244000000ULL,	.size = 0x4000000},	/* CCU1 */
	#endif
};

>>>>>>> upstream/android-13
/*
 * There may be 1 or 2 M4U HWs, But we always expect they are in the same domain
 * for the performance.
 *
 * Here always return the mtk_iommu_data of the first probed M4U where the
 * iommu domain information is recorded.
 */
static struct mtk_iommu_data *mtk_iommu_get_m4u_data(void)
{
	struct mtk_iommu_data *data;

	for_each_m4u(data)
		return data;

	return NULL;
}

static struct mtk_iommu_domain *to_mtk_domain(struct iommu_domain *dom)
{
	return container_of(dom, struct mtk_iommu_domain, domain);
}

<<<<<<< HEAD
static void mtk_iommu_tlb_flush_all(void *cookie)
{
	struct mtk_iommu_data *data = cookie;

	for_each_m4u(data) {
		writel_relaxed(F_INVLD_EN1 | F_INVLD_EN0,
			       data->base + REG_MMU_INV_SEL);
		writel_relaxed(F_ALL_INVLD, data->base + REG_MMU_INVALIDATE);
		wmb(); /* Make sure the tlb flush all done */
	}
}

static void mtk_iommu_tlb_add_flush_nosync(unsigned long iova, size_t size,
					   size_t granule, bool leaf,
					   void *cookie)
{
	struct mtk_iommu_data *data = cookie;

	for_each_m4u(data) {
		writel_relaxed(F_INVLD_EN1 | F_INVLD_EN0,
			       data->base + REG_MMU_INV_SEL);

		writel_relaxed(iova, data->base + REG_MMU_INVLD_START_A);
		writel_relaxed(iova + size - 1,
			       data->base + REG_MMU_INVLD_END_A);
		writel(F_MMU_INV_RANGE, data->base + REG_MMU_INVALIDATE);
		data->tlb_flush_active = true;
	}
}

static void mtk_iommu_tlb_sync(void *cookie)
{
	struct mtk_iommu_data *data = cookie;
=======
static void mtk_iommu_tlb_flush_all(struct mtk_iommu_data *data)
{
	for_each_m4u(data) {
		if (pm_runtime_get_if_in_use(data->dev) <= 0)
			continue;

		writel_relaxed(F_INVLD_EN1 | F_INVLD_EN0,
			       data->base + data->plat_data->inv_sel_reg);
		writel_relaxed(F_ALL_INVLD, data->base + REG_MMU_INVALIDATE);
		wmb(); /* Make sure the tlb flush all done */

		pm_runtime_put(data->dev);
	}
}

static void mtk_iommu_tlb_flush_range_sync(unsigned long iova, size_t size,
					   size_t granule,
					   struct mtk_iommu_data *data)
{
	bool has_pm = !!data->dev->pm_domain;
	unsigned long flags;
>>>>>>> upstream/android-13
	int ret;
	u32 tmp;

	for_each_m4u(data) {
<<<<<<< HEAD
		/* Avoid timing out if there's nothing to wait for */
		if (!data->tlb_flush_active)
			return;

		ret = readl_poll_timeout_atomic(data->base + REG_MMU_CPE_DONE,
						tmp, tmp != 0, 10, 100000);
		if (ret) {
			dev_warn(data->dev,
				 "Partial TLB flush timed out, falling back to full flush\n");
			mtk_iommu_tlb_flush_all(cookie);
		}
		/* Clear the CPE status */
		writel_relaxed(0, data->base + REG_MMU_CPE_DONE);
		data->tlb_flush_active = false;
	}
}

static const struct iommu_gather_ops mtk_iommu_gather_ops = {
	.tlb_flush_all = mtk_iommu_tlb_flush_all,
	.tlb_add_flush = mtk_iommu_tlb_add_flush_nosync,
	.tlb_sync = mtk_iommu_tlb_sync,
};

static unsigned int mtk_iommu_get_larbid(const unsigned int *larbid_in_common,
					 const unsigned int fault_larb)
{
	int i;

	for (i = 0; i < MTK_LARB_NR_MAX; i++)
		if (larbid_in_common[i] == fault_larb)
			return i;
	return MTK_LARB_NR_MAX; /* Invalid larb id. */
}

=======
		if (has_pm) {
			if (pm_runtime_get_if_in_use(data->dev) <= 0)
				continue;
		}

		spin_lock_irqsave(&data->tlb_lock, flags);
		writel_relaxed(F_INVLD_EN1 | F_INVLD_EN0,
			       data->base + data->plat_data->inv_sel_reg);

		writel_relaxed(MTK_IOMMU_TLB_ADDR(iova),
			       data->base + REG_MMU_INVLD_START_A);
		writel_relaxed(MTK_IOMMU_TLB_ADDR(iova + size - 1),
			       data->base + REG_MMU_INVLD_END_A);
		writel_relaxed(F_MMU_INV_RANGE,
			       data->base + REG_MMU_INVALIDATE);

		/* tlb sync */
		ret = readl_poll_timeout_atomic(data->base + REG_MMU_CPE_DONE,
						tmp, tmp != 0, 10, 1000);
		if (ret) {
			dev_warn(data->dev,
				 "Partial TLB flush timed out, falling back to full flush\n");
			mtk_iommu_tlb_flush_all(data);
		}
		/* Clear the CPE status */
		writel_relaxed(0, data->base + REG_MMU_CPE_DONE);
		spin_unlock_irqrestore(&data->tlb_lock, flags);

		if (has_pm)
			pm_runtime_put(data->dev);
	}
}

>>>>>>> upstream/android-13
static irqreturn_t mtk_iommu_isr(int irq, void *dev_id)
{
	struct mtk_iommu_data *data = dev_id;
	struct mtk_iommu_domain *dom = data->m4u_dom;
<<<<<<< HEAD
	u32 int_state, regval, fault_iova, fault_pa;
	unsigned int fault_larb, fault_port;
=======
	unsigned int fault_larb, fault_port, sub_comm = 0;
	u32 int_state, regval, va34_32, pa34_32;
	u64 fault_iova, fault_pa;
>>>>>>> upstream/android-13
	bool layer, write;

	/* Read error info from registers */
	int_state = readl_relaxed(data->base + REG_MMU_FAULT_ST1);
	if (int_state & F_REG_MMU0_FAULT_MASK) {
		regval = readl_relaxed(data->base + REG_MMU0_INT_ID);
		fault_iova = readl_relaxed(data->base + REG_MMU0_FAULT_VA);
		fault_pa = readl_relaxed(data->base + REG_MMU0_INVLD_PA);
	} else {
		regval = readl_relaxed(data->base + REG_MMU1_INT_ID);
		fault_iova = readl_relaxed(data->base + REG_MMU1_FAULT_VA);
		fault_pa = readl_relaxed(data->base + REG_MMU1_INVLD_PA);
	}
	layer = fault_iova & F_MMU_FAULT_VA_LAYER_BIT;
	write = fault_iova & F_MMU_FAULT_VA_WRITE_BIT;
<<<<<<< HEAD

	if (data->plat_data->m4u_plat == M4U_MT8168) {
		fault_larb = F_MMU_INT_ID_LARB_ID_MT8168(regval);
		fault_port = F_MMU_INT_ID_PORT_ID_MT8168(regval);
	} else {
		fault_larb = F_MMU_INT_ID_LARB_ID(regval);
		fault_port = F_MMU_INT_ID_PORT_ID(regval);
	}

	if (data->plat_data->larbid_remap_enable)
		fault_larb = mtk_iommu_get_larbid(
					data->plat_data->larbid_in_common,
					fault_larb);
=======
	if (MTK_IOMMU_HAS_FLAG(data->plat_data, IOVA_34_EN)) {
		va34_32 = FIELD_GET(F_MMU_INVAL_VA_34_32_MASK, fault_iova);
		pa34_32 = FIELD_GET(F_MMU_INVAL_PA_34_32_MASK, fault_iova);
		fault_iova = fault_iova & F_MMU_INVAL_VA_31_12_MASK;
		fault_iova |= (u64)va34_32 << 32;
		fault_pa |= (u64)pa34_32 << 32;
	}

	fault_port = F_MMU_INT_ID_PORT_ID(regval);
	if (MTK_IOMMU_HAS_FLAG(data->plat_data, HAS_SUB_COMM)) {
		fault_larb = F_MMU_INT_ID_COMM_ID(regval);
		sub_comm = F_MMU_INT_ID_SUB_COMM_ID(regval);
	} else {
		fault_larb = F_MMU_INT_ID_LARB_ID(regval);
	}
	fault_larb = data->plat_data->larbid_remap[fault_larb][sub_comm];

>>>>>>> upstream/android-13
	if (report_iommu_fault(&dom->domain, data->dev, fault_iova,
			       write ? IOMMU_FAULT_WRITE : IOMMU_FAULT_READ)) {
		dev_err_ratelimited(
			data->dev,
<<<<<<< HEAD
			"fault type=0x%x iova=0x%x pa=0x%x(0x%x) larb=%d port=%d layer=%d %s sec 0x%x\n",
			int_state, fault_iova, fault_pa,
			(unsigned int)iommu_iova_to_phys(&(dom->domain),
				(fault_iova & 0xfffff000)),
			fault_larb, fault_port, layer,
			(write) ? "write" : "read",
			#if (defined(CONFIG_MTK_PSEUDO_M4U) &&\
				defined(M4U_TEE_SERVICE_ENABLE))
			m4u_dump_secpgd(fault_larb, fault_port, fault_iova)
			#else
			0
			#endif
			);
=======
			"fault type=0x%x iova=0x%llx pa=0x%llx larb=%d port=%d layer=%d %s\n",
			int_state, fault_iova, fault_pa, fault_larb, fault_port,
			layer, write ? "write" : "read");
>>>>>>> upstream/android-13
	}

	/* Interrupt clear */
	regval = readl_relaxed(data->base + REG_MMU_INT_CONTROL0);
	regval |= F_INT_CLR_BIT;
	writel_relaxed(regval, data->base + REG_MMU_INT_CONTROL0);

	mtk_iommu_tlb_flush_all(data);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static void mtk_iommu_config(struct mtk_iommu_data *data,
			     struct device *dev, bool enable)
{
	struct mtk_smi_larb_iommu    *larb_mmu;
	unsigned int                 larbid, portid;
	struct iommu_fwspec *fwspec = dev->iommu_fwspec;
=======
static int mtk_iommu_get_domain_id(struct device *dev,
				   const struct mtk_iommu_plat_data *plat_data)
{
	const struct mtk_iommu_iova_region *rgn = plat_data->iova_region;
	const struct bus_dma_region *dma_rgn = dev->dma_range_map;
	int i, candidate = -1;
	dma_addr_t dma_end;

	if (!dma_rgn || plat_data->iova_region_nr == 1)
		return 0;

	dma_end = dma_rgn->dma_start + dma_rgn->size - 1;
	for (i = 0; i < plat_data->iova_region_nr; i++, rgn++) {
		/* Best fit. */
		if (dma_rgn->dma_start == rgn->iova_base &&
		    dma_end == rgn->iova_base + rgn->size - 1)
			return i;
		/* ok if it is inside this region. */
		if (dma_rgn->dma_start >= rgn->iova_base &&
		    dma_end < rgn->iova_base + rgn->size)
			candidate = i;
	}

	if (candidate >= 0)
		return candidate;
	dev_err(dev, "Can NOT find the iommu domain id(%pad 0x%llx).\n",
		&dma_rgn->dma_start, dma_rgn->size);
	return -EINVAL;
}

static void mtk_iommu_config(struct mtk_iommu_data *data, struct device *dev,
			     bool enable, unsigned int domid)
{
	struct mtk_smi_larb_iommu    *larb_mmu;
	unsigned int                 larbid, portid;
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	const struct mtk_iommu_iova_region *region;
>>>>>>> upstream/android-13
	int i;

	for (i = 0; i < fwspec->num_ids; ++i) {
		larbid = MTK_M4U_TO_LARB(fwspec->ids[i]);
		portid = MTK_M4U_TO_PORT(fwspec->ids[i]);
<<<<<<< HEAD
		larb_mmu = &data->smi_imu.larb_imu[larbid];

		dev_dbg(dev, "%s iommu port: %d\n",
			enable ? "enable" : "disable", portid);
=======

		larb_mmu = &data->larb_imu[larbid];

		region = data->plat_data->iova_region + domid;
		larb_mmu->bank[portid] = upper_32_bits(region->iova_base);

		dev_dbg(dev, "%s iommu for larb(%s) port %d dom %d bank %d.\n",
			enable ? "enable" : "disable", dev_name(larb_mmu->dev),
			portid, domid, larb_mmu->bank[portid]);
>>>>>>> upstream/android-13

		if (enable)
			larb_mmu->mmu |= MTK_SMI_MMU_EN(portid);
		else
			larb_mmu->mmu &= ~MTK_SMI_MMU_EN(portid);
	}
}

<<<<<<< HEAD
static unsigned long mtk_iommu_pgt_base;

unsigned long mtk_get_pgt_base(void)
{
	return mtk_iommu_pgt_base;
}
EXPORT_SYMBOL(mtk_get_pgt_base);
static int mtk_iommu_domain_finalise(struct mtk_iommu_domain *dom)
{
	struct mtk_iommu_data *data = mtk_iommu_get_m4u_data();

	spin_lock_init(&dom->pgtlock);
=======
static int mtk_iommu_domain_finalise(struct mtk_iommu_domain *dom,
				     struct mtk_iommu_data *data,
				     unsigned int domid)
{
	const struct mtk_iommu_iova_region *region;

	/* Use the exist domain as there is only one pgtable here. */
	if (data->m4u_dom) {
		dom->iop = data->m4u_dom->iop;
		dom->cfg = data->m4u_dom->cfg;
		dom->ttbr = data->m4u_dom->ttbr;
		dom->domain.pgsize_bitmap = data->m4u_dom->cfg.pgsize_bitmap;
		goto update_iova_region;
	}
>>>>>>> upstream/android-13

	dom->cfg = (struct io_pgtable_cfg) {
		.quirks = IO_PGTABLE_QUIRK_ARM_NS |
			IO_PGTABLE_QUIRK_NO_PERMS |
<<<<<<< HEAD
			IO_PGTABLE_QUIRK_TLBI_ON_MAP,
		.pgsize_bitmap = mtk_iommu_ops.pgsize_bitmap,
		.ias = 32,
		.oas = 33,
		.tlb = &mtk_iommu_gather_ops,
		.iommu_dev = data->dev,
	};

	if (data->enable_4GB)
		dom->cfg.quirks |= IO_PGTABLE_QUIRK_ARM_MTK_4GB;
=======
			IO_PGTABLE_QUIRK_ARM_MTK_EXT,
		.pgsize_bitmap = mtk_iommu_ops.pgsize_bitmap,
		.ias = MTK_IOMMU_HAS_FLAG(data->plat_data, IOVA_34_EN) ? 34 : 32,
		.iommu_dev = data->dev,
	};

	if (MTK_IOMMU_HAS_FLAG(data->plat_data, PGTABLE_PA_35_EN))
		dom->cfg.quirks |= IO_PGTABLE_QUIRK_ARM_MTK_TTBR_EXT;

	if (MTK_IOMMU_HAS_FLAG(data->plat_data, HAS_4GB_MODE))
		dom->cfg.oas = data->enable_4GB ? 33 : 32;
	else
		dom->cfg.oas = 35;
>>>>>>> upstream/android-13

	dom->iop = alloc_io_pgtable_ops(ARM_V7S, &dom->cfg, data);
	if (!dom->iop) {
		dev_err(data->dev, "Failed to alloc io pgtable\n");
		return -EINVAL;
	}
<<<<<<< HEAD

	/* Update our support page sizes bitmap */
	dom->domain.pgsize_bitmap = dom->cfg.pgsize_bitmap;
	mtk_iommu_pgt_base = dom->cfg.arm_v7s_cfg.ttbr[0];
=======
	dom->ttbr = dom->cfg.quirks & IO_PGTABLE_QUIRK_ARM_MTK_TTBR_EXT ?
		    dom->cfg.arm_v7s_cfg.ttbr :
		    dom->cfg.arm_v7s_cfg.ttbr & MMU_PT_ADDR_MASK;

	/* Update our support page sizes bitmap */
	dom->domain.pgsize_bitmap = dom->cfg.pgsize_bitmap;

update_iova_region:
	/* Update the iova region for this domain */
	region = data->plat_data->iova_region + domid;
	dom->domain.geometry.aperture_start = region->iova_base;
	dom->domain.geometry.aperture_end = region->iova_base + region->size - 1;
	dom->domain.geometry.force_aperture = true;
>>>>>>> upstream/android-13
	return 0;
}

static struct iommu_domain *mtk_iommu_domain_alloc(unsigned type)
{
	struct mtk_iommu_domain *dom;

<<<<<<< HEAD
	if (type != IOMMU_DOMAIN_DMA && type != IOMMU_DOMAIN_UNMANAGED)
=======
	if (type != IOMMU_DOMAIN_DMA)
>>>>>>> upstream/android-13
		return NULL;

	dom = kzalloc(sizeof(*dom), GFP_KERNEL);
	if (!dom)
		return NULL;

<<<<<<< HEAD
	if (iommu_get_dma_cookie(&dom->domain))
		goto  free_dom;

	if (mtk_iommu_domain_finalise(dom))
		goto  put_dma_cookie;

	dom->domain.geometry.aperture_start = 0;
	dom->domain.geometry.aperture_end = DMA_BIT_MASK(32);
	dom->domain.geometry.force_aperture = true;

	return &dom->domain;

put_dma_cookie:
	iommu_put_dma_cookie(&dom->domain);
free_dom:
	kfree(dom);
	return NULL;
=======
	return &dom->domain;
>>>>>>> upstream/android-13
}

static void mtk_iommu_domain_free(struct iommu_domain *domain)
{
<<<<<<< HEAD
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);

	free_io_pgtable_ops(dom->iop);
	iommu_put_dma_cookie(domain);
	kfree(to_mtk_domain(domain));
}

#ifdef CONFIG_ARM64
static void mtk_iommu_get_resv_regions(struct device *dev,
				      struct list_head *head)
{
	struct mtk_iommu_data *data = mtk_iommu_get_m4u_data();
	unsigned int i, total_cnt = data->plat_data->spec_cnt;
	const struct mtk_iommu_resv_iova_region *spec_data;
	struct iommu_resv_region *region;
	unsigned long base = 0;
	size_t size = 0;
	int prot = IOMMU_WRITE | IOMMU_READ;

	if (!total_cnt || !of_device_is_compatible(dev->of_node,
				data->plat_data->spec_device_comp))
		return;

	spec_data = data->plat_data->spec_region;

	for (i = 0; i < total_cnt; i++) {
		size = 0;
		if (spec_data[i].iova_size) {
			base = spec_data[i].iova_base;
			size = spec_data[i].iova_size;
		} else if (spec_data[i].get_resv_data)
			spec_data[i].get_resv_data(&base, &size, data);
		if (!size)
			continue;

		region = iommu_alloc_resv_region(base, size, prot,
						 spec_data[i].type);
		if (!region)
			return;

		list_add_tail(&region->list, head);

		/* for debug */
		dev_info(data->dev, "%s iova 0x%x ~ 0x%x\n",
			(spec_data[i].type == IOMMU_RESV_DIRECT) ? "dm" : "rsv",
			(unsigned int)base, (unsigned int)(base + size - 1));
	}
}

static void mtk_iommu_put_resv_regions(struct device *dev,
				      struct list_head *head)
{
	struct iommu_resv_region *entry, *next;

	list_for_each_entry_safe(entry, next, head, list)
		kfree(entry);
}
#else
static int mtk_iommu_reserve_region(struct mtk_iommu_data *data)
{
	struct iommu_domain *domain = &data->m4u_dom->domain;
	unsigned int i, total_cnt = data->plat_data->spec_cnt;
	const struct mtk_iommu_resv_iova_region *spec_data;
	unsigned long base = 0;
	size_t size = 0;
	enum iommu_resv_type type;
	int prot = (IOMMU_WRITE | IOMMU_READ);
	int ret;

	if (!total_cnt)
		return 0;

	spec_data = data->plat_data->spec_region;

	for (i = 0; i < total_cnt; i++) {
		size = 0;
		if (spec_data[i].iova_size) {
			base = spec_data[i].iova_base;
			size = spec_data[i].iova_size;
		} else if (spec_data[i].get_resv_data)
			spec_data[i].get_resv_data(&base, &size, data);
		if (!size)
			continue;

		type = spec_data[i].type;
		ret = 0;
		if (type == IOMMU_RESV_DIRECT)
			ret = iommu_map(domain, base, (phys_addr_t)base,
					size, prot);
		else if (type == IOMMU_RESV_RESERVED)
			ret = arm_dma_reserve(data->dev->archdata.iommu,
					(dma_addr_t)base, size);

		if (ret)
			dev_err(data->dev, "%s iova 0x%x ~ 0x%x failed\n",
				(type == IOMMU_RESV_DIRECT) ?
				"dir map" : "reserve",
				(unsigned int)base,
				(unsigned int)(base + size - 1));

		/* for debug */
		dev_info(data->dev, "%s iova 0x%x ~ 0x%x\n",
			(type == IOMMU_RESV_DIRECT) ? "dm" : "rsv",
			(unsigned int)base, (unsigned int)(base + size - 1));
	}

	return 0;
}
#endif

static int mtk_iommu_attach_device(struct iommu_domain *domain,
				   struct device *dev)
{
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);
	struct mtk_iommu_data *data = dev->iommu_fwspec->iommu_priv;

	if (!data)
		return -ENODEV;

	/* Update the pgtable base address register of the M4U HW */
	if (!data->m4u_dom) {
		data->m4u_dom = dom;
		writel(dom->cfg.arm_v7s_cfg.ttbr[0] & MMU_PT_ADDR_MASK,
		       data->base + REG_MMU_PT_BASE_ADDR);
	#ifndef CONFIG_ARM64
		mtk_iommu_reserve_region(data);
	#endif
	}

	mtk_iommu_config(data, dev, true);

	if (!dev->dma_parms) {
		dev->dma_parms = kzalloc(sizeof(*dev->dma_parms), GFP_KERNEL);
		if (!dev->dma_parms)
			return -ENOMEM;
	}
	dma_set_max_seg_size(dev, DMA_BIT_MASK(32));

=======
	kfree(to_mtk_domain(domain));
}

static int mtk_iommu_attach_device(struct iommu_domain *domain,
				   struct device *dev)
{
	struct mtk_iommu_data *data = dev_iommu_priv_get(dev);
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);
	struct device *m4udev = data->dev;
	int ret, domid;

	domid = mtk_iommu_get_domain_id(dev, data->plat_data);
	if (domid < 0)
		return domid;

	if (!dom->data) {
		if (mtk_iommu_domain_finalise(dom, data, domid))
			return -ENODEV;
		dom->data = data;
	}

	if (!data->m4u_dom) { /* Initialize the M4U HW */
		ret = pm_runtime_resume_and_get(m4udev);
		if (ret < 0)
			return ret;

		ret = mtk_iommu_hw_init(data);
		if (ret) {
			pm_runtime_put(m4udev);
			return ret;
		}
		data->m4u_dom = dom;
		writel(data->m4u_dom->ttbr, data->base + REG_MMU_PT_BASE_ADDR);

		pm_runtime_put(m4udev);
	}

	mtk_iommu_config(data, dev, true, domid);
>>>>>>> upstream/android-13
	return 0;
}

static void mtk_iommu_detach_device(struct iommu_domain *domain,
				    struct device *dev)
{
<<<<<<< HEAD
	struct mtk_iommu_data *data = dev->iommu_fwspec->iommu_priv;

	if (!data)
		return;

	if (!dev->dma_parms)
		kfree(dev->dma_parms);
	dev->dma_parms = NULL;

	mtk_iommu_config(data, dev, false);
}

static int mtk_iommu_map(struct iommu_domain *domain, unsigned long iova,
			 phys_addr_t paddr, size_t size, int prot)
{
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);
	struct mtk_iommu_data *data = mtk_iommu_get_m4u_data();
	unsigned long flags;
	int ret;

	if (IS_ENABLED(CONFIG_PHYS_ADDR_T_64BIT) &&
	    data->plat_data->has_4gb_mode &&
	    data->enable_4GB)
		paddr |= BIT_ULL(32);

	spin_lock_irqsave(&dom->pgtlock, flags);
	ret = dom->iop->map(dom->iop, iova, paddr, size, prot);
	spin_unlock_irqrestore(&dom->pgtlock, flags);

	return ret;
}

static size_t mtk_iommu_unmap(struct iommu_domain *domain,
			      unsigned long iova, size_t size)
{
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);
	unsigned long flags;
	size_t unmapsz;

	spin_lock_irqsave(&dom->pgtlock, flags);
	unmapsz = dom->iop->unmap(dom->iop, iova, size);
	spin_unlock_irqrestore(&dom->pgtlock, flags);

	return unmapsz;
}

static void mtk_iommu_iotlb_sync(struct iommu_domain *domain)
{
	mtk_iommu_tlb_sync(mtk_iommu_get_m4u_data());
=======
	struct mtk_iommu_data *data = dev_iommu_priv_get(dev);

	mtk_iommu_config(data, dev, false, 0);
}

static int mtk_iommu_map(struct iommu_domain *domain, unsigned long iova,
			 phys_addr_t paddr, size_t size, int prot, gfp_t gfp)
{
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);

	/* The "4GB mode" M4U physically can not use the lower remap of Dram. */
	if (dom->data->enable_4GB)
		paddr |= BIT_ULL(32);

	/* Synchronize with the tlb_lock */
	return dom->iop->map(dom->iop, iova, paddr, size, prot, gfp);
}

static size_t mtk_iommu_unmap(struct iommu_domain *domain,
			      unsigned long iova, size_t size,
			      struct iommu_iotlb_gather *gather)
{
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);

	iommu_iotlb_gather_add_range(gather, iova, size);
	return dom->iop->unmap(dom->iop, iova, size, gather);
}

static void mtk_iommu_flush_iotlb_all(struct iommu_domain *domain)
{
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);

	mtk_iommu_tlb_flush_all(dom->data);
}

static void mtk_iommu_iotlb_sync(struct iommu_domain *domain,
				 struct iommu_iotlb_gather *gather)
{
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);
	size_t length = gather->end - gather->start + 1;

	mtk_iommu_tlb_flush_range_sync(gather->start, length, gather->pgsize,
				       dom->data);
}

static void mtk_iommu_sync_map(struct iommu_domain *domain, unsigned long iova,
			       size_t size)
{
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);

	mtk_iommu_tlb_flush_range_sync(iova, size, size, dom->data);
>>>>>>> upstream/android-13
}

static phys_addr_t mtk_iommu_iova_to_phys(struct iommu_domain *domain,
					  dma_addr_t iova)
{
	struct mtk_iommu_domain *dom = to_mtk_domain(domain);
<<<<<<< HEAD
	struct mtk_iommu_data *data = mtk_iommu_get_m4u_data();
	unsigned long flags;
	phys_addr_t pa;

	spin_lock_irqsave(&dom->pgtlock, flags);
	pa = dom->iop->iova_to_phys(dom->iop, iova);
	spin_unlock_irqrestore(&dom->pgtlock, flags);

	if (data->enable_4GB && pa && pa < MTK_IOMMU_4GB_MODE_REMAP_BASE)
		pa |= BIT_ULL(32);
=======
	phys_addr_t pa;

	pa = dom->iop->iova_to_phys(dom->iop, iova);
	if (IS_ENABLED(CONFIG_PHYS_ADDR_T_64BIT) &&
	    dom->data->enable_4GB &&
	    pa >= MTK_IOMMU_4GB_MODE_REMAP_BASE)
		pa &= ~BIT_ULL(32);
>>>>>>> upstream/android-13

	return pa;
}

<<<<<<< HEAD
#ifdef CONFIG_ARM64
static int mtk_iommu_add_device(struct device *dev)
{
	struct mtk_iommu_data *data;
	struct iommu_group *group;

	if (!dev->iommu_fwspec || dev->iommu_fwspec->ops != &mtk_iommu_ops)
		return -ENODEV; /* Not a iommu client device */

	data = dev->iommu_fwspec->iommu_priv;
	iommu_device_link(&data->iommu, dev);

	group = iommu_group_get_for_dev(dev);
	if (IS_ERR(group))
		return PTR_ERR(group);

	iommu_group_put(group);
	return 0;
}
#else
static int mtk_iommu_init_arm_mapping(struct device *dev)
{
	struct mtk_iommu_data *data;
	struct dma_iommu_mapping *mtk_mapping;
	struct device *m4udev;
	int ret;

	data = dev->iommu_fwspec->iommu_priv;
	m4udev = data->dev;
	mtk_mapping = m4udev->archdata.iommu;
	if (!mtk_mapping) {
		/* MTK iommu support 4GB iova address space. */
		mtk_mapping = arm_iommu_create_mapping(&platform_bus_type,
						0, 1ULL << 32);
		if (IS_ERR(mtk_mapping))
			return PTR_ERR(mtk_mapping);

		m4udev->archdata.iommu = mtk_mapping;
	}

	ret = arm_iommu_attach_device(dev, mtk_mapping);
	if (ret)
		dev_info(dev, "attach fail %d\n", ret);
	return ret;
}

static int mtk_iommu_add_device(struct device *dev)
{
	struct mtk_iommu_data *data;
	struct iommu_group *group;
	int err;

	if (!dev->iommu_fwspec || dev->iommu_fwspec->ops != &mtk_iommu_ops)
		return -ENODEV; /* Not a iommu client device */

	/*
	 * This is a short-term bodge because the ARM DMA code doesn't
	 * understand multi-device groups, but we have to call into it
	 * successfully (and not just rely on a normal IOMMU API attach
	 * here) in order to set the correct DMA API ops on @dev.
	 */
	group = iommu_group_alloc();
	if (IS_ERR(group))
		return PTR_ERR(group);

	err = iommu_group_add_device(group, dev);
	iommu_group_put(group);
	if (err)
		return err;

	err = mtk_iommu_init_arm_mapping(dev);
	if (err) {
		iommu_group_remove_device(dev);
		return err;
	}

	data = dev->iommu_fwspec->iommu_priv;
	return iommu_device_link(&data->iommu, dev);
}
#endif

static void mtk_iommu_remove_device(struct device *dev)
{
	struct mtk_iommu_data *data;

	if (!dev->iommu_fwspec || dev->iommu_fwspec->ops != &mtk_iommu_ops)
		return;

	data = dev->iommu_fwspec->iommu_priv;
	iommu_device_unlink(&data->iommu, dev);

	iommu_group_remove_device(dev);
=======
static struct iommu_device *mtk_iommu_probe_device(struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	struct mtk_iommu_data *data;
	struct device_link *link;
	struct device *larbdev;
	unsigned int larbid, larbidx, i;

	if (!fwspec || fwspec->ops != &mtk_iommu_ops)
		return ERR_PTR(-ENODEV); /* Not a iommu client device */

	data = dev_iommu_priv_get(dev);

	/*
	 * Link the consumer device with the smi-larb device(supplier).
	 * The device that connects with each a larb is a independent HW.
	 * All the ports in each a device should be in the same larbs.
	 */
	larbid = MTK_M4U_TO_LARB(fwspec->ids[0]);
	for (i = 1; i < fwspec->num_ids; i++) {
		larbidx = MTK_M4U_TO_LARB(fwspec->ids[i]);
		if (larbid != larbidx) {
			dev_err(dev, "Can only use one larb. Fail@larb%d-%d.\n",
				larbid, larbidx);
			return ERR_PTR(-EINVAL);
		}
	}
	larbdev = data->larb_imu[larbid].dev;
	link = device_link_add(dev, larbdev,
			       DL_FLAG_PM_RUNTIME | DL_FLAG_STATELESS);
	if (!link)
		dev_err(dev, "Unable to link %s\n", dev_name(larbdev));
	return &data->iommu;
}

static void mtk_iommu_release_device(struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	struct mtk_iommu_data *data;
	struct device *larbdev;
	unsigned int larbid;

	if (!fwspec || fwspec->ops != &mtk_iommu_ops)
		return;

	data = dev_iommu_priv_get(dev);
	larbid = MTK_M4U_TO_LARB(fwspec->ids[0]);
	larbdev = data->larb_imu[larbid].dev;
	device_link_remove(dev, larbdev);

>>>>>>> upstream/android-13
	iommu_fwspec_free(dev);
}

static struct iommu_group *mtk_iommu_device_group(struct device *dev)
{
	struct mtk_iommu_data *data = mtk_iommu_get_m4u_data();
<<<<<<< HEAD
=======
	struct iommu_group *group;
	int domid;
>>>>>>> upstream/android-13

	if (!data)
		return ERR_PTR(-ENODEV);

<<<<<<< HEAD
	/* All the client devices are in the same m4u iommu-group */
	if (!data->m4u_group) {
		data->m4u_group = iommu_group_alloc();
		if (IS_ERR(data->m4u_group))
			dev_err(dev, "Failed to allocate M4U IOMMU group\n");
	} else {
		iommu_group_ref_get(data->m4u_group);
	}
	return data->m4u_group;
=======
	domid = mtk_iommu_get_domain_id(dev, data->plat_data);
	if (domid < 0)
		return ERR_PTR(domid);

	group = data->m4u_group[domid];
	if (!group) {
		group = iommu_group_alloc();
		if (!IS_ERR(group))
			data->m4u_group[domid] = group;
	} else {
		iommu_group_ref_get(group);
	}
	return group;
>>>>>>> upstream/android-13
}

static int mtk_iommu_of_xlate(struct device *dev, struct of_phandle_args *args)
{
	struct platform_device *m4updev;

	if (args->args_count != 1) {
		dev_err(dev, "invalid #iommu-cells(%d) property for IOMMU\n",
			args->args_count);
		return -EINVAL;
	}

<<<<<<< HEAD
	if (!dev->iommu_fwspec->iommu_priv) {
=======
	if (!dev_iommu_priv_get(dev)) {
>>>>>>> upstream/android-13
		/* Get the m4u device */
		m4updev = of_find_device_by_node(args->np);
		if (WARN_ON(!m4updev))
			return -EINVAL;

<<<<<<< HEAD
		dev->iommu_fwspec->iommu_priv = platform_get_drvdata(m4updev);
=======
		dev_iommu_priv_set(dev, platform_get_drvdata(m4updev));
>>>>>>> upstream/android-13
	}

	return iommu_fwspec_add_ids(dev, args->args, 1);
}

<<<<<<< HEAD
static struct iommu_ops mtk_iommu_ops = {
=======
static void mtk_iommu_get_resv_regions(struct device *dev,
				       struct list_head *head)
{
	struct mtk_iommu_data *data = dev_iommu_priv_get(dev);
	unsigned int domid = mtk_iommu_get_domain_id(dev, data->plat_data), i;
	const struct mtk_iommu_iova_region *resv, *curdom;
	struct iommu_resv_region *region;
	int prot = IOMMU_WRITE | IOMMU_READ;

	if ((int)domid < 0)
		return;
	curdom = data->plat_data->iova_region + domid;
	for (i = 0; i < data->plat_data->iova_region_nr; i++) {
		resv = data->plat_data->iova_region + i;

		/* Only reserve when the region is inside the current domain */
		if (resv->iova_base <= curdom->iova_base ||
		    resv->iova_base + resv->size >= curdom->iova_base + curdom->size)
			continue;

		region = iommu_alloc_resv_region(resv->iova_base, resv->size,
						 prot, IOMMU_RESV_RESERVED);
		if (!region)
			return;

		list_add_tail(&region->list, head);
	}
}

static const struct iommu_ops mtk_iommu_ops = {
>>>>>>> upstream/android-13
	.domain_alloc	= mtk_iommu_domain_alloc,
	.domain_free	= mtk_iommu_domain_free,
	.attach_dev	= mtk_iommu_attach_device,
	.detach_dev	= mtk_iommu_detach_device,
	.map		= mtk_iommu_map,
	.unmap		= mtk_iommu_unmap,
<<<<<<< HEAD
	.map_sg		= default_iommu_map_sg,
	.flush_iotlb_all = mtk_iommu_iotlb_sync,
	.iotlb_sync	= mtk_iommu_iotlb_sync,
	.iova_to_phys	= mtk_iommu_iova_to_phys,
	.add_device	= mtk_iommu_add_device,
	.remove_device	= mtk_iommu_remove_device,
	.device_group	= mtk_iommu_device_group,
	.of_xlate	= mtk_iommu_of_xlate,
#ifdef CONFIG_ARM64
	.get_resv_regions = mtk_iommu_get_resv_regions,
	.put_resv_regions = mtk_iommu_put_resv_regions,
#endif
	.pgsize_bitmap	= SZ_4K | SZ_64K | SZ_1M | SZ_16M,
=======
	.flush_iotlb_all = mtk_iommu_flush_iotlb_all,
	.iotlb_sync	= mtk_iommu_iotlb_sync,
	.iotlb_sync_map	= mtk_iommu_sync_map,
	.iova_to_phys	= mtk_iommu_iova_to_phys,
	.probe_device	= mtk_iommu_probe_device,
	.release_device	= mtk_iommu_release_device,
	.device_group	= mtk_iommu_device_group,
	.of_xlate	= mtk_iommu_of_xlate,
	.get_resv_regions = mtk_iommu_get_resv_regions,
	.put_resv_regions = generic_iommu_put_resv_regions,
	.pgsize_bitmap	= SZ_4K | SZ_64K | SZ_1M | SZ_16M,
	.owner		= THIS_MODULE,
>>>>>>> upstream/android-13
};

static int mtk_iommu_hw_init(const struct mtk_iommu_data *data)
{
<<<<<<< HEAD
	enum mtk_iommu_plat m4u_plat = data->plat_data->m4u_plat;
	u32 regval;
	int ret;

	ret = clk_prepare_enable(data->bclk);
	if (ret) {
		dev_err(data->dev, "Failed to enable iommu bclk(%d)\n", ret);
		return ret;
	}

	regval = F_MMU_TF_PROTECT_SEL(2, data);
	if (m4u_plat == M4U_MT8173)
		regval |= F_MMU_PREFETCH_RT_REPLACE_MOD;
=======
	u32 regval;

	if (data->plat_data->m4u_plat == M4U_MT8173) {
		regval = F_MMU_PREFETCH_RT_REPLACE_MOD |
			 F_MMU_TF_PROT_TO_PROGRAM_ADDR_MT8173;
	} else {
		regval = readl_relaxed(data->base + REG_MMU_CTRL_REG);
		regval |= F_MMU_TF_PROT_TO_PROGRAM_ADDR;
	}
>>>>>>> upstream/android-13
	writel_relaxed(regval, data->base + REG_MMU_CTRL_REG);

	regval = F_L2_MULIT_HIT_EN |
		F_TABLE_WALK_FAULT_INT_EN |
		F_PREETCH_FIFO_OVERFLOW_INT_EN |
		F_MISS_FIFO_OVERFLOW_INT_EN |
		F_PREFETCH_FIFO_ERR_INT_EN |
		F_MISS_FIFO_ERR_INT_EN;
	writel_relaxed(regval, data->base + REG_MMU_INT_CONTROL0);

	regval = F_INT_TRANSLATION_FAULT |
		F_INT_MAIN_MULTI_HIT_FAULT |
		F_INT_INVALID_PA_FAULT |
		F_INT_ENTRY_REPLACEMENT_FAULT |
		F_INT_TLB_MISS_FAULT |
		F_INT_MISS_TRANSACTION_FIFO_FAULT |
		F_INT_PRETETCH_TRANSATION_FIFO_FAULT;
	writel_relaxed(regval, data->base + REG_MMU_INT_MAIN_CONTROL);

<<<<<<< HEAD
	if (m4u_plat == M4U_MT8173 || m4u_plat == M4U_MT8167)
=======
	if (MTK_IOMMU_HAS_FLAG(data->plat_data, HAS_LEGACY_IVRP_PADDR))
>>>>>>> upstream/android-13
		regval = (data->protect_base >> 1) | (data->enable_4GB << 31);
	else
		regval = lower_32_bits(data->protect_base) |
			 upper_32_bits(data->protect_base);
	writel_relaxed(regval, data->base + REG_MMU_IVRP_PADDR);

<<<<<<< HEAD
	if (data->enable_4GB && m4u_plat == M4U_MT2712) {
=======
	if (data->enable_4GB &&
	    MTK_IOMMU_HAS_FLAG(data->plat_data, HAS_VLD_PA_RNG)) {
>>>>>>> upstream/android-13
		/*
		 * If 4GB mode is enabled, the validate PA range is from
		 * 0x1_0000_0000 to 0x1_ffff_ffff. here record bit[32:30].
		 */
		regval = F_MMU_VLD_PA_RNG(7, 4);
		writel_relaxed(regval, data->base + REG_MMU_VLD_PA_RNG);
	}
	writel_relaxed(0, data->base + REG_MMU_DCM_DIS);
<<<<<<< HEAD

	/*
	 * It's MISC control register whose default value is ok
	 * except mt8173 and mt8183.
	 */
	if (m4u_plat == M4U_MT8167 || m4u_plat == M4U_MT8168 ||
	    m4u_plat == M4U_MT8173 || m4u_plat == M4U_MT8183)
		writel_relaxed(0, data->base + REG_MMU_STANDARD_AXI_MODE);
=======
	if (MTK_IOMMU_HAS_FLAG(data->plat_data, WR_THROT_EN)) {
		/* write command throttling mode */
		regval = readl_relaxed(data->base + REG_MMU_WR_LEN_CTRL);
		regval &= ~F_MMU_WR_THROT_DIS_MASK;
		writel_relaxed(regval, data->base + REG_MMU_WR_LEN_CTRL);
	}

	if (MTK_IOMMU_HAS_FLAG(data->plat_data, RESET_AXI)) {
		/* The register is called STANDARD_AXI_MODE in this case */
		regval = 0;
	} else {
		regval = readl_relaxed(data->base + REG_MMU_MISC_CTRL);
		regval &= ~F_MMU_STANDARD_AXI_MODE_MASK;
		if (MTK_IOMMU_HAS_FLAG(data->plat_data, OUT_ORDER_WR_EN))
			regval &= ~F_MMU_IN_ORDER_WR_EN_MASK;
	}
	writel_relaxed(regval, data->base + REG_MMU_MISC_CTRL);
>>>>>>> upstream/android-13

	if (devm_request_irq(data->dev, data->irq, mtk_iommu_isr, 0,
			     dev_name(data->dev), (void *)data)) {
		writel_relaxed(0, data->base + REG_MMU_PT_BASE_ADDR);
<<<<<<< HEAD
		clk_disable_unprepare(data->bclk);
=======
>>>>>>> upstream/android-13
		dev_err(data->dev, "Failed @ IRQ-%d Request\n", data->irq);
		return -ENODEV;
	}

	return 0;
}

static const struct component_master_ops mtk_iommu_com_ops = {
	.bind		= mtk_iommu_bind,
	.unbind		= mtk_iommu_unbind,
};

static int mtk_iommu_probe(struct platform_device *pdev)
{
	struct mtk_iommu_data   *data;
	struct device           *dev = &pdev->dev;
<<<<<<< HEAD
	struct resource         *res;
	resource_size_t		ioaddr;
	struct component_match  *match = NULL;
	void                    *protect;
	int                     i, larb_nr, ret;
=======
	struct device_node	*larbnode, *smicomm_node;
	struct platform_device	*plarbdev;
	struct device_link	*link;
	struct resource         *res;
	resource_size_t		ioaddr;
	struct component_match  *match = NULL;
	struct regmap		*infracfg;
	void                    *protect;
	int                     i, larb_nr, ret;
	u32			val;
	char                    *p;
>>>>>>> upstream/android-13

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;
	data->dev = dev;
	data->plat_data = of_device_get_match_data(dev);

	/* Protect memory. HW will access here while translation fault.*/
	protect = devm_kzalloc(dev, MTK_PROTECT_PA_ALIGN * 2, GFP_KERNEL);
	if (!protect)
		return -ENOMEM;
	data->protect_base = ALIGN(virt_to_phys(protect), MTK_PROTECT_PA_ALIGN);

<<<<<<< HEAD
	/* Whether the current dram is over 4GB */
	data->enable_4GB = !!(max_pfn > (BIT_ULL(32) >> PAGE_SHIFT));
=======
	if (MTK_IOMMU_HAS_FLAG(data->plat_data, HAS_4GB_MODE)) {
		switch (data->plat_data->m4u_plat) {
		case M4U_MT2712:
			p = "mediatek,mt2712-infracfg";
			break;
		case M4U_MT8173:
			p = "mediatek,mt8173-infracfg";
			break;
		default:
			p = NULL;
		}

		infracfg = syscon_regmap_lookup_by_compatible(p);

		if (IS_ERR(infracfg))
			return PTR_ERR(infracfg);

		ret = regmap_read(infracfg, REG_INFRA_MISC, &val);
		if (ret)
			return ret;
		data->enable_4GB = !!(val & F_DDR_4GB_SUPPORT_EN);
	}
>>>>>>> upstream/android-13

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	data->base = devm_ioremap_resource(dev, res);
	if (IS_ERR(data->base))
		return PTR_ERR(data->base);
	ioaddr = res->start;

	data->irq = platform_get_irq(pdev, 0);
	if (data->irq < 0)
		return data->irq;

<<<<<<< HEAD
	data->bclk = devm_clk_get(dev, "bclk");
	if (PTR_ERR(data->bclk) == -ENOENT)
		data->bclk = NULL;
	else if (IS_ERR(data->bclk))
		return PTR_ERR(data->bclk);
=======
	if (MTK_IOMMU_HAS_FLAG(data->plat_data, HAS_BCLK)) {
		data->bclk = devm_clk_get(dev, "bclk");
		if (IS_ERR(data->bclk))
			return PTR_ERR(data->bclk);
	}
>>>>>>> upstream/android-13

	larb_nr = of_count_phandle_with_args(dev->of_node,
					     "mediatek,larbs", NULL);
	if (larb_nr < 0)
		return larb_nr;
<<<<<<< HEAD
	data->smi_imu.larb_nr = larb_nr;

	for (i = 0; i < larb_nr; i++) {
		struct device_node *larbnode;
		struct platform_device *plarbdev;
=======

	for (i = 0; i < larb_nr; i++) {
>>>>>>> upstream/android-13
		u32 id;

		larbnode = of_parse_phandle(dev->of_node, "mediatek,larbs", i);
		if (!larbnode)
			return -EINVAL;

<<<<<<< HEAD
		if (!of_device_is_available(larbnode))
			continue;
=======
		if (!of_device_is_available(larbnode)) {
			of_node_put(larbnode);
			continue;
		}
>>>>>>> upstream/android-13

		ret = of_property_read_u32(larbnode, "mediatek,larb-id", &id);
		if (ret)/* The id is consecutive if there is no this property */
			id = i;

		plarbdev = of_find_device_by_node(larbnode);
<<<<<<< HEAD
		if (!plarbdev)
			return -EPROBE_DEFER;
		data->smi_imu.larb_imu[id].dev = &plarbdev->dev;
=======
		if (!plarbdev) {
			of_node_put(larbnode);
			return -ENODEV;
		}
		data->larb_imu[id].dev = &plarbdev->dev;
>>>>>>> upstream/android-13

		component_match_add_release(dev, &match, release_of,
					    compare_of, larbnode);
	}

<<<<<<< HEAD
	platform_set_drvdata(pdev, data);

	ret = mtk_iommu_hw_init(data);
	if (ret)
		return ret;
=======
	/* Get smi-common dev from the last larb. */
	smicomm_node = of_parse_phandle(larbnode, "mediatek,smi", 0);
	if (!smicomm_node)
		return -EINVAL;

	plarbdev = of_find_device_by_node(smicomm_node);
	of_node_put(smicomm_node);
	data->smicomm_dev = &plarbdev->dev;

	pm_runtime_enable(dev);

	link = device_link_add(data->smicomm_dev, dev,
			DL_FLAG_STATELESS | DL_FLAG_PM_RUNTIME);
	if (!link) {
		dev_err(dev, "Unable to link %s.\n", dev_name(data->smicomm_dev));
		ret = -EINVAL;
		goto out_runtime_disable;
	}

	platform_set_drvdata(pdev, data);
>>>>>>> upstream/android-13

	ret = iommu_device_sysfs_add(&data->iommu, dev, NULL,
				     "mtk-iommu.%pa", &ioaddr);
	if (ret)
<<<<<<< HEAD
		return ret;

	iommu_device_set_ops(&data->iommu, &mtk_iommu_ops);
	iommu_device_set_fwnode(&data->iommu, &pdev->dev.of_node->fwnode);

	ret = iommu_device_register(&data->iommu);
	if (ret)
		return ret;

	list_add_tail(&data->list, &m4ulist);

	if (!iommu_present(&platform_bus_type))
		bus_set_iommu(&platform_bus_type, &mtk_iommu_ops);

	return component_master_add_with_match(dev, &mtk_iommu_com_ops, match);
=======
		goto out_link_remove;

	ret = iommu_device_register(&data->iommu, &mtk_iommu_ops, dev);
	if (ret)
		goto out_sysfs_remove;

	spin_lock_init(&data->tlb_lock);
	list_add_tail(&data->list, &m4ulist);

	if (!iommu_present(&platform_bus_type)) {
		ret = bus_set_iommu(&platform_bus_type, &mtk_iommu_ops);
		if (ret)
			goto out_list_del;
	}

	ret = component_master_add_with_match(dev, &mtk_iommu_com_ops, match);
	if (ret)
		goto out_bus_set_null;
	return ret;

out_bus_set_null:
	bus_set_iommu(&platform_bus_type, NULL);
out_list_del:
	list_del(&data->list);
	iommu_device_unregister(&data->iommu);
out_sysfs_remove:
	iommu_device_sysfs_remove(&data->iommu);
out_link_remove:
	device_link_remove(data->smicomm_dev, dev);
out_runtime_disable:
	pm_runtime_disable(dev);
	return ret;
>>>>>>> upstream/android-13
}

static int mtk_iommu_remove(struct platform_device *pdev)
{
	struct mtk_iommu_data *data = platform_get_drvdata(pdev);

	iommu_device_sysfs_remove(&data->iommu);
	iommu_device_unregister(&data->iommu);

	if (iommu_present(&platform_bus_type))
		bus_set_iommu(&platform_bus_type, NULL);

	clk_disable_unprepare(data->bclk);
<<<<<<< HEAD
=======
	device_link_remove(data->smicomm_dev, &pdev->dev);
	pm_runtime_disable(&pdev->dev);
>>>>>>> upstream/android-13
	devm_free_irq(&pdev->dev, data->irq, data);
	component_master_del(&pdev->dev, &mtk_iommu_com_ops);
	return 0;
}

<<<<<<< HEAD
static void mtk_iommu_shutdown(struct platform_device *pdev)
{
	mtk_iommu_remove(pdev);
}

static int __maybe_unused mtk_iommu_suspend(struct device *dev)
=======
static int __maybe_unused mtk_iommu_runtime_suspend(struct device *dev)
>>>>>>> upstream/android-13
{
	struct mtk_iommu_data *data = dev_get_drvdata(dev);
	struct mtk_iommu_suspend_reg *reg = &data->reg;
	void __iomem *base = data->base;

<<<<<<< HEAD
	reg->standard_axi_mode = readl_relaxed(base +
					       REG_MMU_STANDARD_AXI_MODE);
=======
	reg->wr_len_ctrl = readl_relaxed(base + REG_MMU_WR_LEN_CTRL);
	reg->misc_ctrl = readl_relaxed(base + REG_MMU_MISC_CTRL);
>>>>>>> upstream/android-13
	reg->dcm_dis = readl_relaxed(base + REG_MMU_DCM_DIS);
	reg->ctrl_reg = readl_relaxed(base + REG_MMU_CTRL_REG);
	reg->int_control0 = readl_relaxed(base + REG_MMU_INT_CONTROL0);
	reg->int_main_control = readl_relaxed(base + REG_MMU_INT_MAIN_CONTROL);
	reg->ivrp_paddr = readl_relaxed(base + REG_MMU_IVRP_PADDR);
<<<<<<< HEAD
	reg->vld_pa_range = readl_relaxed(base + REG_MMU_VLD_PA_RNG);
=======
	reg->vld_pa_rng = readl_relaxed(base + REG_MMU_VLD_PA_RNG);
>>>>>>> upstream/android-13
	clk_disable_unprepare(data->bclk);
	return 0;
}

<<<<<<< HEAD
static int __maybe_unused mtk_iommu_resume(struct device *dev)
{
	struct mtk_iommu_data *data = dev_get_drvdata(dev);
	struct mtk_iommu_suspend_reg *reg = &data->reg;
	struct mtk_iommu_domain *dom = data->m4u_dom;
=======
static int __maybe_unused mtk_iommu_runtime_resume(struct device *dev)
{
	struct mtk_iommu_data *data = dev_get_drvdata(dev);
	struct mtk_iommu_suspend_reg *reg = &data->reg;
	struct mtk_iommu_domain *m4u_dom = data->m4u_dom;
>>>>>>> upstream/android-13
	void __iomem *base = data->base;
	int ret;

	ret = clk_prepare_enable(data->bclk);
	if (ret) {
		dev_err(data->dev, "Failed to enable clk(%d) in resume\n", ret);
		return ret;
	}
<<<<<<< HEAD
	writel_relaxed(reg->standard_axi_mode,
		       base + REG_MMU_STANDARD_AXI_MODE);
=======

	/*
	 * Uppon first resume, only enable the clk and return, since the values of the
	 * registers are not yet set.
	 */
	if (!m4u_dom)
		return 0;

	writel_relaxed(reg->wr_len_ctrl, base + REG_MMU_WR_LEN_CTRL);
	writel_relaxed(reg->misc_ctrl, base + REG_MMU_MISC_CTRL);
>>>>>>> upstream/android-13
	writel_relaxed(reg->dcm_dis, base + REG_MMU_DCM_DIS);
	writel_relaxed(reg->ctrl_reg, base + REG_MMU_CTRL_REG);
	writel_relaxed(reg->int_control0, base + REG_MMU_INT_CONTROL0);
	writel_relaxed(reg->int_main_control, base + REG_MMU_INT_MAIN_CONTROL);
	writel_relaxed(reg->ivrp_paddr, base + REG_MMU_IVRP_PADDR);
<<<<<<< HEAD
	writel_relaxed(reg->vld_pa_range, base + REG_MMU_VLD_PA_RNG);
	if (dom)
		writel(dom->cfg.arm_v7s_cfg.ttbr[0] & MMU_PT_ADDR_MASK,
		       base + REG_MMU_PT_BASE_ADDR);
=======
	writel_relaxed(reg->vld_pa_rng, base + REG_MMU_VLD_PA_RNG);
	writel(m4u_dom->ttbr, base + REG_MMU_PT_BASE_ADDR);
>>>>>>> upstream/android-13
	return 0;
}

static const struct dev_pm_ops mtk_iommu_pm_ops = {
<<<<<<< HEAD
	SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(mtk_iommu_suspend, mtk_iommu_resume)
};

static const struct mtk_iommu_plat_data mt2712_data = {
	.m4u_plat = M4U_MT2712,
	.has_4gb_mode = true,
};

static void mtk_get_disp_dm_region(unsigned long *base, size_t *size,
				   struct mtk_iommu_data *data)
{
/*
 * Display show fastlogo in lk. it is contiougous buffer. when entering kernel,
 * the iommu HW will be enabled. Avoid the display show fastlogo smoothly,
 * defautly map the fastlogo physicall address as the iova address.
 * directly mapping (iova == pa).
 *
 * This pa for a project can be get by mtkfv_get_fb_xxx().
 */
#ifdef CONFIG_MTK_FB
	*base = (unsigned long)mtkfb_get_fb_base();
	*size = (size_t)mtkfb_get_fb_size();
#else
	*base = 0;
	*size = 0;
#endif
}

static void mtk_get_sec_rsv_region(unsigned long *base, size_t *size,
				   struct mtk_iommu_data *data)
{
#if (defined(CONFIG_MTK_PSEUDO_M4U) && defined(M4U_TEE_SERVICE_ENABLE))
	struct mtk_iommu_domain *dom = data->m4u_dom;

	if (!dom) {
		*size = 0;
		*base = 0;
		return;
	}

	if (!dom->sec_iova_size)
		dom->sec_iova_size = mtk_init_tz_m4u();

	*size = dom->sec_iova_size;
#else
	/* Here reserve the region with zero iova,
	 * so that user wouldn't get a zero iova.
	 */
	*size = 0x10000;
#endif
	*base = 0;
}

static const struct mtk_iommu_resv_iova_region mt8167_iommu_rsv_list[2] = {
	{	.iova_base = 0,
		.iova_size = 0,
		.type = IOMMU_RESV_DIRECT,
		.get_resv_data = mtk_get_disp_dm_region,
	},
	{	.iova_base = 0,
		.iova_size = 0,
		.type = IOMMU_RESV_RESERVED,
		.get_resv_data = mtk_get_sec_rsv_region,
	},
};

static const struct mtk_iommu_plat_data mt8167_data = {
	.m4u_plat = M4U_MT8167,
	.has_4gb_mode = true,
	.spec_device_comp = "mediatek,mtk-pseudo-m4u",
	.spec_cnt = ARRAY_SIZE(mt8167_iommu_rsv_list),
	.spec_region = &mt8167_iommu_rsv_list[0],
};

static const struct mtk_iommu_resv_iova_region mt8168_iommu_rsv_list[3] = {
	{	.iova_base = 0,
		.iova_size = 0,
		.type = IOMMU_RESV_DIRECT,
		.get_resv_data = mtk_get_disp_dm_region,
	},
	{	.iova_base = 0,
		.iova_size = 0,
		.type = IOMMU_RESV_RESERVED,
		.get_resv_data = mtk_get_sec_rsv_region,
	},
	{	.iova_base = 0x7D100000,
		.iova_size = 0x05500000,	/* s 9M + ns 76M */
		.type = IOMMU_RESV_RESERVED,
	},
};

static const struct mtk_iommu_plat_data mt8168_data = {
	.m4u_plat = M4U_MT8168,
	.has_4gb_mode = true,
	.spec_device_comp = "mediatek,mtk-pseudo-m4u",
	.spec_cnt = ARRAY_SIZE(mt8168_iommu_rsv_list),
	.spec_region = &mt8168_iommu_rsv_list[0],
};

static const struct mtk_iommu_plat_data mt8173_data = {
	.m4u_plat = M4U_MT8173,
	.has_4gb_mode = true,
};

static const struct mtk_iommu_plat_data mt8183_data = {
	.m4u_plat = M4U_MT8183,
	.larbid_remap_enable = true,
	.larbid_in_common = {0, 7, 5, 6, 1, 2, 3, 4},
=======
	SET_RUNTIME_PM_OPS(mtk_iommu_runtime_suspend, mtk_iommu_runtime_resume, NULL)
	SET_LATE_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
				     pm_runtime_force_resume)
};

static const struct mtk_iommu_plat_data mt2712_data = {
	.m4u_plat     = M4U_MT2712,
	.flags        = HAS_4GB_MODE | HAS_BCLK | HAS_VLD_PA_RNG,
	.inv_sel_reg  = REG_MMU_INV_SEL_GEN1,
	.iova_region  = single_domain,
	.iova_region_nr = ARRAY_SIZE(single_domain),
	.larbid_remap = {{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}},
};

static const struct mtk_iommu_plat_data mt6779_data = {
	.m4u_plat      = M4U_MT6779,
	.flags         = HAS_SUB_COMM | OUT_ORDER_WR_EN | WR_THROT_EN |
			 PGTABLE_PA_35_EN,
	.inv_sel_reg   = REG_MMU_INV_SEL_GEN2,
	.iova_region   = single_domain,
	.iova_region_nr = ARRAY_SIZE(single_domain),
	.larbid_remap  = {{0}, {1}, {2}, {3}, {5}, {7, 8}, {10}, {9}},
};

static const struct mtk_iommu_plat_data mt8167_data = {
	.m4u_plat     = M4U_MT8167,
	.flags        = RESET_AXI | HAS_LEGACY_IVRP_PADDR,
	.inv_sel_reg  = REG_MMU_INV_SEL_GEN1,
	.iova_region  = single_domain,
	.iova_region_nr = ARRAY_SIZE(single_domain),
	.larbid_remap = {{0}, {1}, {2}}, /* Linear mapping. */
};

static const struct mtk_iommu_plat_data mt8173_data = {
	.m4u_plat     = M4U_MT8173,
	.flags	      = HAS_4GB_MODE | HAS_BCLK | RESET_AXI |
			HAS_LEGACY_IVRP_PADDR,
	.inv_sel_reg  = REG_MMU_INV_SEL_GEN1,
	.iova_region  = single_domain,
	.iova_region_nr = ARRAY_SIZE(single_domain),
	.larbid_remap = {{0}, {1}, {2}, {3}, {4}, {5}}, /* Linear mapping. */
};

static const struct mtk_iommu_plat_data mt8183_data = {
	.m4u_plat     = M4U_MT8183,
	.flags        = RESET_AXI,
	.inv_sel_reg  = REG_MMU_INV_SEL_GEN1,
	.iova_region  = single_domain,
	.iova_region_nr = ARRAY_SIZE(single_domain),
	.larbid_remap = {{0}, {4}, {5}, {6}, {7}, {2}, {3}, {1}},
};

static const struct mtk_iommu_plat_data mt8192_data = {
	.m4u_plat       = M4U_MT8192,
	.flags          = HAS_BCLK | HAS_SUB_COMM | OUT_ORDER_WR_EN |
			  WR_THROT_EN | IOVA_34_EN,
	.inv_sel_reg    = REG_MMU_INV_SEL_GEN2,
	.iova_region    = mt8192_multi_dom,
	.iova_region_nr = ARRAY_SIZE(mt8192_multi_dom),
	.larbid_remap   = {{0}, {1}, {4, 5}, {7}, {2}, {9, 11, 19, 20},
			   {0, 14, 16}, {0, 13, 18, 17}},
>>>>>>> upstream/android-13
};

static const struct of_device_id mtk_iommu_of_ids[] = {
	{ .compatible = "mediatek,mt2712-m4u", .data = &mt2712_data},
<<<<<<< HEAD
	{ .compatible = "mediatek,mt8167-m4u", .data = &mt8167_data},
	{ .compatible = "mediatek,mt8168-m4u", .data = &mt8168_data},
	{ .compatible = "mediatek,mt8173-m4u", .data = &mt8173_data},
	{ .compatible = "mediatek,mt8183-m4u", .data = &mt8183_data},
=======
	{ .compatible = "mediatek,mt6779-m4u", .data = &mt6779_data},
	{ .compatible = "mediatek,mt8167-m4u", .data = &mt8167_data},
	{ .compatible = "mediatek,mt8173-m4u", .data = &mt8173_data},
	{ .compatible = "mediatek,mt8183-m4u", .data = &mt8183_data},
	{ .compatible = "mediatek,mt8192-m4u", .data = &mt8192_data},
>>>>>>> upstream/android-13
	{}
};

static struct platform_driver mtk_iommu_driver = {
	.probe	= mtk_iommu_probe,
	.remove	= mtk_iommu_remove,
<<<<<<< HEAD
	.shutdown = mtk_iommu_shutdown,
	.driver	= {
		.name = "mtk-iommu",
		.of_match_table = of_match_ptr(mtk_iommu_of_ids),
		.pm = &mtk_iommu_pm_ops,
	}
};

static int __init mtk_iommu_init(void)
{
	int ret;

	ret = platform_driver_register(&mtk_iommu_driver);
	if (ret != 0)
		pr_err("Failed to register MTK IOMMU driver\n");

	return ret;
}

subsys_initcall(mtk_iommu_init)
=======
	.driver	= {
		.name = "mtk-iommu",
		.of_match_table = mtk_iommu_of_ids,
		.pm = &mtk_iommu_pm_ops,
	}
};
module_platform_driver(mtk_iommu_driver);

MODULE_DESCRIPTION("IOMMU API for MediaTek M4U implementations");
MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
