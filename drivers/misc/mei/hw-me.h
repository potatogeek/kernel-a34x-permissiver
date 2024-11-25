<<<<<<< HEAD
/*
 *
 * Intel Management Engine Interface (Intel MEI) Linux driver
 * Copyright (c) 2003-2012, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */



=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2012-2020, Intel Corporation. All rights reserved.
 * Intel Management Engine Interface (Intel MEI) Linux driver
 */

>>>>>>> upstream/android-13
#ifndef _MEI_INTERFACE_H_
#define _MEI_INTERFACE_H_

#include <linux/irqreturn.h>
#include <linux/pci.h>
#include <linux/mei.h>

#include "mei_dev.h"
#include "client.h"

/*
 * mei_cfg - mei device configuration
 *
 * @fw_status: FW status
 * @quirk_probe: device exclusion quirk
<<<<<<< HEAD
 * @dma_size: device DMA buffers size
 * @fw_ver_supported: is fw version retrievable from FW
 */
struct mei_cfg {
	const struct mei_fw_status fw_status;
	bool (*quirk_probe)(struct pci_dev *pdev);
	size_t dma_size[DMA_DSCR_NUM];
	u32 fw_ver_supported:1;
=======
 * @kind: MEI head kind
 * @dma_size: device DMA buffers size
 * @fw_ver_supported: is fw version retrievable from FW
 * @hw_trc_supported: does the hw support trc register
 */
struct mei_cfg {
	const struct mei_fw_status fw_status;
	bool (*quirk_probe)(const struct pci_dev *pdev);
	const char *kind;
	size_t dma_size[DMA_DSCR_NUM];
	u32 fw_ver_supported:1;
	u32 hw_trc_supported:1;
>>>>>>> upstream/android-13
};


#define MEI_PCI_DEVICE(dev, cfg) \
	.vendor = PCI_VENDOR_ID_INTEL, .device = (dev), \
	.subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID, \
	.driver_data = (kernel_ulong_t)(cfg),

#define MEI_ME_RPM_TIMEOUT    500 /* ms */

/**
 * struct mei_me_hw - me hw specific data
 *
 * @cfg: per device generation config and ops
 * @mem_addr: io memory address
<<<<<<< HEAD
 * @pg_state: power gating state
 * @d0i3_supported: di03 support
 * @hbuf_depth: depth of hardware host/write buffer in slots
=======
 * @irq: irq number
 * @pg_state: power gating state
 * @d0i3_supported: di03 support
 * @hbuf_depth: depth of hardware host/write buffer in slots
 * @read_fws: read FW status register handler
>>>>>>> upstream/android-13
 */
struct mei_me_hw {
	const struct mei_cfg *cfg;
	void __iomem *mem_addr;
<<<<<<< HEAD
	enum mei_pg_state pg_state;
	bool d0i3_supported;
	u8 hbuf_depth;
=======
	int irq;
	enum mei_pg_state pg_state;
	bool d0i3_supported;
	u8 hbuf_depth;
	int (*read_fws)(const struct mei_device *dev, int where, u32 *val);
>>>>>>> upstream/android-13
};

#define to_me_hw(dev) (struct mei_me_hw *)((dev)->hw)

/**
 * enum mei_cfg_idx - indices to platform specific configurations.
 *
 * Note: has to be synchronized with mei_cfg_list[]
 *
 * @MEI_ME_UNDEF_CFG:      Lower sentinel.
 * @MEI_ME_ICH_CFG:        I/O Controller Hub legacy devices.
 * @MEI_ME_ICH10_CFG:      I/O Controller Hub platforms Gen10
 * @MEI_ME_PCH6_CFG:       Platform Controller Hub platforms (Gen6).
 * @MEI_ME_PCH7_CFG:       Platform Controller Hub platforms (Gen7).
 * @MEI_ME_PCH_CPT_PBG_CFG:Platform Controller Hub workstations
 *                         with quirk for Node Manager exclusion.
 * @MEI_ME_PCH8_CFG:       Platform Controller Hub Gen8 and newer
 *                         client platforms.
<<<<<<< HEAD
 * @MEI_ME_PCH8_SPS_CFG:   Platform Controller Hub Gen8 and newer
 *                         servers platforms with quirk for
 *                         SPS firmware exclusion.
 * @MEI_ME_PCH12_CFG:      Platform Controller Hub Gen12 and newer
=======
 * @MEI_ME_PCH8_ITOUCH_CFG:Platform Controller Hub Gen8 and newer
 *                         client platforms (iTouch).
 * @MEI_ME_PCH8_SPS_4_CFG: Platform Controller Hub Gen8 and newer
 *                         servers platforms with quirk for
 *                         SPS firmware exclusion.
 * @MEI_ME_PCH12_CFG:      Platform Controller Hub Gen12 and newer
 * @MEI_ME_PCH12_SPS_4_CFG:Platform Controller Hub Gen12 up to 4.0
 *                         servers platforms with quirk for
 *                         SPS firmware exclusion.
 * @MEI_ME_PCH12_SPS_CFG:  Platform Controller Hub Gen12 5.0 and newer
 *                         servers platforms with quirk for
 *                         SPS firmware exclusion.
 * @MEI_ME_PCH15_CFG:      Platform Controller Hub Gen15 and newer
 * @MEI_ME_PCH15_SPS_CFG:  Platform Controller Hub Gen15 and newer
 *                         servers platforms with quirk for
 *                         SPS firmware exclusion.
>>>>>>> upstream/android-13
 * @MEI_ME_NUM_CFG:        Upper Sentinel.
 */
enum mei_cfg_idx {
	MEI_ME_UNDEF_CFG,
	MEI_ME_ICH_CFG,
	MEI_ME_ICH10_CFG,
	MEI_ME_PCH6_CFG,
	MEI_ME_PCH7_CFG,
	MEI_ME_PCH_CPT_PBG_CFG,
	MEI_ME_PCH8_CFG,
<<<<<<< HEAD
	MEI_ME_PCH8_SPS_CFG,
	MEI_ME_PCH12_CFG,
=======
	MEI_ME_PCH8_ITOUCH_CFG,
	MEI_ME_PCH8_SPS_4_CFG,
	MEI_ME_PCH12_CFG,
	MEI_ME_PCH12_SPS_4_CFG,
	MEI_ME_PCH12_SPS_CFG,
	MEI_ME_PCH12_SPS_ITOUCH_CFG,
	MEI_ME_PCH15_CFG,
	MEI_ME_PCH15_SPS_CFG,
>>>>>>> upstream/android-13
	MEI_ME_NUM_CFG,
};

const struct mei_cfg *mei_me_get_cfg(kernel_ulong_t idx);

<<<<<<< HEAD
struct mei_device *mei_me_dev_init(struct pci_dev *pdev,
=======
struct mei_device *mei_me_dev_init(struct device *parent,
>>>>>>> upstream/android-13
				   const struct mei_cfg *cfg);

int mei_me_pg_enter_sync(struct mei_device *dev);
int mei_me_pg_exit_sync(struct mei_device *dev);

irqreturn_t mei_me_irq_quick_handler(int irq, void *dev_id);
irqreturn_t mei_me_irq_thread_handler(int irq, void *dev_id);

#endif /* _MEI_INTERFACE_H_ */
