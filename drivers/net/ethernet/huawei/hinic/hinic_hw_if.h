<<<<<<< HEAD
/*
 * Huawei HiNIC PCI Express Linux driver
 * Copyright(c) 2017 Huawei Technologies Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Huawei HiNIC PCI Express Linux driver
 * Copyright(c) 2017 Huawei Technologies Co., Ltd
>>>>>>> upstream/android-13
 */

#ifndef HINIC_HW_IF_H
#define HINIC_HW_IF_H

#include <linux/pci.h>
#include <linux/io.h>
#include <linux/types.h>
#include <asm/byteorder.h>

<<<<<<< HEAD
=======
#define HINIC_PCIE_LINK_DOWN					0xFFFFFFFF

>>>>>>> upstream/android-13
#define HINIC_DMA_ATTR_ST_SHIFT                                 0
#define HINIC_DMA_ATTR_AT_SHIFT                                 8
#define HINIC_DMA_ATTR_PH_SHIFT                                 10
#define HINIC_DMA_ATTR_NO_SNOOPING_SHIFT                        12
#define HINIC_DMA_ATTR_TPH_EN_SHIFT                             13

#define HINIC_DMA_ATTR_ST_MASK                                  0xFF
#define HINIC_DMA_ATTR_AT_MASK                                  0x3
#define HINIC_DMA_ATTR_PH_MASK                                  0x3
#define HINIC_DMA_ATTR_NO_SNOOPING_MASK                         0x1
#define HINIC_DMA_ATTR_TPH_EN_MASK                              0x1

#define HINIC_DMA_ATTR_SET(val, member)                         \
	(((u32)(val) & HINIC_DMA_ATTR_##member##_MASK) <<       \
	 HINIC_DMA_ATTR_##member##_SHIFT)

#define HINIC_DMA_ATTR_CLEAR(val, member)                       \
	((val) & (~(HINIC_DMA_ATTR_##member##_MASK              \
	 << HINIC_DMA_ATTR_##member##_SHIFT)))

#define HINIC_FA0_FUNC_IDX_SHIFT                                0
#define HINIC_FA0_PF_IDX_SHIFT                                  10
#define HINIC_FA0_PCI_INTF_IDX_SHIFT                            14
<<<<<<< HEAD
=======
#define HINIC_FA0_VF_IN_PF_SHIFT				16
>>>>>>> upstream/android-13
/* reserved members - off 16 */
#define HINIC_FA0_FUNC_TYPE_SHIFT                               24

#define HINIC_FA0_FUNC_IDX_MASK                                 0x3FF
#define HINIC_FA0_PF_IDX_MASK                                   0xF
#define HINIC_FA0_PCI_INTF_IDX_MASK                             0x3
#define HINIC_FA0_FUNC_TYPE_MASK                                0x1
<<<<<<< HEAD
=======
#define HINIC_FA0_VF_IN_PF_MASK					0xFF
>>>>>>> upstream/android-13

#define HINIC_FA0_GET(val, member)                              \
	(((val) >> HINIC_FA0_##member##_SHIFT) & HINIC_FA0_##member##_MASK)

#define HINIC_FA1_AEQS_PER_FUNC_SHIFT                           8
/* reserved members - off 10 */
#define HINIC_FA1_CEQS_PER_FUNC_SHIFT                           12
/* reserved members - off 15 */
#define HINIC_FA1_IRQS_PER_FUNC_SHIFT                           20
#define HINIC_FA1_DMA_ATTR_PER_FUNC_SHIFT                       24
/* reserved members - off 27 */
<<<<<<< HEAD
#define HINIC_FA1_INIT_STATUS_SHIFT                             30
=======
#define HINIC_FA1_MGMT_INIT_STATUS_SHIFT			30
#define HINIC_FA1_PF_INIT_STATUS_SHIFT				31
>>>>>>> upstream/android-13

#define HINIC_FA1_AEQS_PER_FUNC_MASK                            0x3
#define HINIC_FA1_CEQS_PER_FUNC_MASK                            0x7
#define HINIC_FA1_IRQS_PER_FUNC_MASK                            0xF
#define HINIC_FA1_DMA_ATTR_PER_FUNC_MASK                        0x7
<<<<<<< HEAD
#define HINIC_FA1_INIT_STATUS_MASK                              0x1
=======
#define HINIC_FA1_MGMT_INIT_STATUS_MASK                         0x1
#define HINIC_FA1_PF_INIT_STATUS_MASK				0x1
>>>>>>> upstream/android-13

#define HINIC_FA1_GET(val, member)                              \
	(((val) >> HINIC_FA1_##member##_SHIFT) & HINIC_FA1_##member##_MASK)

<<<<<<< HEAD
=======
#define HINIC_FA2_GLOBAL_VF_ID_OF_PF_SHIFT	16
#define HINIC_FA2_GLOBAL_VF_ID_OF_PF_MASK	0x3FF

#define HINIC_FA2_GET(val, member)				\
	(((val) >> HINIC_FA2_##member##_SHIFT) & HINIC_FA2_##member##_MASK)

>>>>>>> upstream/android-13
#define HINIC_FA4_OUTBOUND_STATE_SHIFT                          0
#define HINIC_FA4_DB_STATE_SHIFT                                1

#define HINIC_FA4_OUTBOUND_STATE_MASK                           0x1
#define HINIC_FA4_DB_STATE_MASK                                 0x1

#define HINIC_FA4_GET(val, member)                              \
	(((val) >> HINIC_FA4_##member##_SHIFT) & HINIC_FA4_##member##_MASK)

#define HINIC_FA4_SET(val, member)                              \
	((((u32)val) & HINIC_FA4_##member##_MASK) << HINIC_FA4_##member##_SHIFT)

#define HINIC_FA4_CLEAR(val, member)                            \
	((val) & (~(HINIC_FA4_##member##_MASK << HINIC_FA4_##member##_SHIFT)))

#define HINIC_FA5_PF_ACTION_SHIFT                               0
#define HINIC_FA5_PF_ACTION_MASK                                0xFFFF

#define HINIC_FA5_SET(val, member)                              \
	(((u32)(val) & HINIC_FA5_##member##_MASK) << HINIC_FA5_##member##_SHIFT)

#define HINIC_FA5_CLEAR(val, member)                            \
	((val) & (~(HINIC_FA5_##member##_MASK << HINIC_FA5_##member##_SHIFT)))

#define HINIC_PPF_ELECTION_IDX_SHIFT                            0
#define HINIC_PPF_ELECTION_IDX_MASK                             0x1F

#define HINIC_PPF_ELECTION_SET(val, member)                     \
	(((u32)(val) & HINIC_PPF_ELECTION_##member##_MASK) <<   \
	 HINIC_PPF_ELECTION_##member##_SHIFT)

#define HINIC_PPF_ELECTION_GET(val, member)                     \
	(((val) >> HINIC_PPF_ELECTION_##member##_SHIFT) &       \
	 HINIC_PPF_ELECTION_##member##_MASK)

#define HINIC_PPF_ELECTION_CLEAR(val, member)                   \
	((val) & (~(HINIC_PPF_ELECTION_##member##_MASK          \
	 << HINIC_PPF_ELECTION_##member##_SHIFT)))

#define HINIC_MSIX_PENDING_LIMIT_SHIFT                          0
#define HINIC_MSIX_COALESC_TIMER_SHIFT                          8
#define HINIC_MSIX_LLI_TIMER_SHIFT                              16
#define HINIC_MSIX_LLI_CREDIT_SHIFT                             24
#define HINIC_MSIX_RESEND_TIMER_SHIFT                           29

#define HINIC_MSIX_PENDING_LIMIT_MASK                           0xFF
#define HINIC_MSIX_COALESC_TIMER_MASK                           0xFF
#define HINIC_MSIX_LLI_TIMER_MASK                               0xFF
#define HINIC_MSIX_LLI_CREDIT_MASK                              0x1F
#define HINIC_MSIX_RESEND_TIMER_MASK                            0x7

#define HINIC_MSIX_ATTR_SET(val, member)                        \
	(((u32)(val) & HINIC_MSIX_##member##_MASK) <<           \
	 HINIC_MSIX_##member##_SHIFT)

#define HINIC_MSIX_ATTR_GET(val, member)                        \
	(((val) >> HINIC_MSIX_##member##_SHIFT) &               \
	 HINIC_MSIX_##member##_MASK)

#define HINIC_MSIX_CNT_RESEND_TIMER_SHIFT                       29

#define HINIC_MSIX_CNT_RESEND_TIMER_MASK                        0x1

#define HINIC_MSIX_CNT_SET(val, member)                         \
	(((u32)(val) & HINIC_MSIX_CNT_##member##_MASK) <<       \
	 HINIC_MSIX_CNT_##member##_SHIFT)

#define HINIC_HWIF_NUM_AEQS(hwif)       ((hwif)->attr.num_aeqs)
#define HINIC_HWIF_NUM_CEQS(hwif)       ((hwif)->attr.num_ceqs)
#define HINIC_HWIF_NUM_IRQS(hwif)       ((hwif)->attr.num_irqs)
#define HINIC_HWIF_FUNC_IDX(hwif)       ((hwif)->attr.func_idx)
#define HINIC_HWIF_PCI_INTF(hwif)       ((hwif)->attr.pci_intf_idx)
#define HINIC_HWIF_PF_IDX(hwif)         ((hwif)->attr.pf_idx)
#define HINIC_HWIF_PPF_IDX(hwif)        ((hwif)->attr.ppf_idx)

#define HINIC_FUNC_TYPE(hwif)           ((hwif)->attr.func_type)
<<<<<<< HEAD
=======
#define HINIC_IS_VF(hwif)               (HINIC_FUNC_TYPE(hwif) == HINIC_VF)
>>>>>>> upstream/android-13
#define HINIC_IS_PF(hwif)               (HINIC_FUNC_TYPE(hwif) == HINIC_PF)
#define HINIC_IS_PPF(hwif)              (HINIC_FUNC_TYPE(hwif) == HINIC_PPF)

#define HINIC_PCI_CFG_REGS_BAR          0
<<<<<<< HEAD
=======
#define HINIC_PCI_INTR_REGS_BAR         2
>>>>>>> upstream/android-13
#define HINIC_PCI_DB_BAR                4

#define HINIC_PCIE_ST_DISABLE           0
#define HINIC_PCIE_AT_DISABLE           0
#define HINIC_PCIE_PH_DISABLE           0

#define HINIC_EQ_MSIX_PENDING_LIMIT_DEFAULT     0       /* Disabled */
#define HINIC_EQ_MSIX_COALESC_TIMER_DEFAULT     0xFF    /* max */
#define HINIC_EQ_MSIX_LLI_TIMER_DEFAULT         0       /* Disabled */
#define HINIC_EQ_MSIX_LLI_CREDIT_LIMIT_DEFAULT  0       /* Disabled */
#define HINIC_EQ_MSIX_RESEND_TIMER_DEFAULT      7       /* max */

<<<<<<< HEAD
=======
#define HINIC_PCI_MSIX_ENTRY_SIZE               16
#define HINIC_PCI_MSIX_ENTRY_VECTOR_CTRL        12
#define HINIC_PCI_MSIX_ENTRY_CTRL_MASKBIT       1

>>>>>>> upstream/android-13
enum hinic_pcie_nosnoop {
	HINIC_PCIE_SNOOP        = 0,
	HINIC_PCIE_NO_SNOOP     = 1,
};

enum hinic_pcie_tph {
	HINIC_PCIE_TPH_DISABLE  = 0,
	HINIC_PCIE_TPH_ENABLE   = 1,
};

enum hinic_func_type {
	HINIC_PF        = 0,
<<<<<<< HEAD
=======
	HINIC_VF	    = 1,
>>>>>>> upstream/android-13
	HINIC_PPF       = 2,
};

enum hinic_mod_type {
	HINIC_MOD_COMM  = 0,    /* HW communication module */
	HINIC_MOD_L2NIC = 1,    /* L2NIC module */
	HINIC_MOD_CFGM  = 7,    /* Configuration module */
<<<<<<< HEAD

=======
	HINIC_MOD_HILINK = 14,  /* Hilink module */
>>>>>>> upstream/android-13
	HINIC_MOD_MAX   = 15
};

enum hinic_node_id {
	HINIC_NODE_ID_MGMT = 21,
};

enum hinic_pf_action {
	HINIC_PF_MGMT_INIT = 0x0,

	HINIC_PF_MGMT_ACTIVE = 0x11,
};

enum hinic_outbound_state {
	HINIC_OUTBOUND_ENABLE  = 0,
	HINIC_OUTBOUND_DISABLE = 1,
};

enum hinic_db_state {
	HINIC_DB_ENABLE  = 0,
	HINIC_DB_DISABLE = 1,
};

<<<<<<< HEAD
=======
enum hinic_msix_state {
	HINIC_MSIX_ENABLE,
	HINIC_MSIX_DISABLE,
};

>>>>>>> upstream/android-13
struct hinic_func_attr {
	u16                     func_idx;
	u8                      pf_idx;
	u8                      pci_intf_idx;

	enum hinic_func_type    func_type;

	u8                      ppf_idx;

	u16                     num_irqs;
	u8                      num_aeqs;
	u8                      num_ceqs;

	u8                      num_dma_attr;
<<<<<<< HEAD
=======

	u16						global_vf_id_of_pf;
>>>>>>> upstream/android-13
};

struct hinic_hwif {
	struct pci_dev          *pdev;
	void __iomem            *cfg_regs_bar;
<<<<<<< HEAD
=======
	void __iomem		*intr_regs_base;
>>>>>>> upstream/android-13

	struct hinic_func_attr  attr;
};

static inline u32 hinic_hwif_read_reg(struct hinic_hwif *hwif, u32 reg)
{
<<<<<<< HEAD
	return be32_to_cpu(readl(hwif->cfg_regs_bar + reg));
=======
	u32 out = readl(hwif->cfg_regs_bar + reg);

	return be32_to_cpu(*(__be32 *)&out);
>>>>>>> upstream/android-13
}

static inline void hinic_hwif_write_reg(struct hinic_hwif *hwif, u32 reg,
					u32 val)
{
<<<<<<< HEAD
	writel(cpu_to_be32(val), hwif->cfg_regs_bar + reg);
=======
	__be32 in = cpu_to_be32(val);

	writel(*(u32 *)&in, hwif->cfg_regs_bar + reg);
>>>>>>> upstream/android-13
}

int hinic_msix_attr_set(struct hinic_hwif *hwif, u16 msix_index,
			u8 pending_limit, u8 coalesc_timer,
			u8 lli_timer_cfg, u8 lli_credit_limit,
			u8 resend_timer);

int hinic_msix_attr_get(struct hinic_hwif *hwif, u16 msix_index,
			u8 *pending_limit, u8 *coalesc_timer_cfg,
			u8 *lli_timer, u8 *lli_credit_limit,
			u8 *resend_timer);

<<<<<<< HEAD
=======
void hinic_set_msix_state(struct hinic_hwif *hwif, u16 msix_idx,
			  enum hinic_msix_state flag);

>>>>>>> upstream/android-13
int hinic_msix_attr_cnt_clear(struct hinic_hwif *hwif, u16 msix_index);

void hinic_set_pf_action(struct hinic_hwif *hwif, enum hinic_pf_action action);

enum hinic_outbound_state hinic_outbound_state_get(struct hinic_hwif *hwif);

void hinic_outbound_state_set(struct hinic_hwif *hwif,
			      enum hinic_outbound_state outbound_state);

enum hinic_db_state hinic_db_state_get(struct hinic_hwif *hwif);

void hinic_db_state_set(struct hinic_hwif *hwif,
			enum hinic_db_state db_state);

<<<<<<< HEAD
=======
u16 hinic_glb_pf_vf_offset(struct hinic_hwif *hwif);

u16 hinic_global_func_id_hw(struct hinic_hwif *hwif);

u16 hinic_pf_id_of_vf_hw(struct hinic_hwif *hwif);

>>>>>>> upstream/android-13
int hinic_init_hwif(struct hinic_hwif *hwif, struct pci_dev *pdev);

void hinic_free_hwif(struct hinic_hwif *hwif);

#endif
