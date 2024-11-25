<<<<<<< HEAD
/*
  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY
  Copyright(c) 2014 Intel Corporation.
  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  qat-linux@intel.com

  BSD LICENSE
  Copyright(c) 2014 Intel Corporation.
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
=======
/* SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0-only) */
/* Copyright(c) 2014 - 2020 Intel Corporation */
>>>>>>> upstream/android-13
#ifndef ADF_ACCEL_DEVICES_H_
#define ADF_ACCEL_DEVICES_H_
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/io.h>
#include <linux/ratelimit.h>
#include "adf_cfg_common.h"

#define ADF_DH895XCC_DEVICE_NAME "dh895xcc"
#define ADF_DH895XCCVF_DEVICE_NAME "dh895xccvf"
#define ADF_C62X_DEVICE_NAME "c6xx"
#define ADF_C62XVF_DEVICE_NAME "c6xxvf"
#define ADF_C3XXX_DEVICE_NAME "c3xxx"
#define ADF_C3XXXVF_DEVICE_NAME "c3xxxvf"
<<<<<<< HEAD
#define ADF_DH895XCC_PCI_DEVICE_ID 0x435
#define ADF_DH895XCCIOV_PCI_DEVICE_ID 0x443
#define ADF_C62X_PCI_DEVICE_ID 0x37c8
#define ADF_C62XIOV_PCI_DEVICE_ID 0x37c9
#define ADF_C3XXX_PCI_DEVICE_ID 0x19e2
#define ADF_C3XXXIOV_PCI_DEVICE_ID 0x19e3
#define ADF_ERRSOU3 (0x3A000 + 0x0C)
#define ADF_ERRSOU5 (0x3A000 + 0xD8)
=======
#define ADF_4XXX_DEVICE_NAME "4xxx"
#define ADF_4XXX_PCI_DEVICE_ID 0x4940
#define ADF_4XXXIOV_PCI_DEVICE_ID 0x4941
>>>>>>> upstream/android-13
#define ADF_DEVICE_FUSECTL_OFFSET 0x40
#define ADF_DEVICE_LEGFUSE_OFFSET 0x4C
#define ADF_DEVICE_FUSECTL_MASK 0x80000000
#define ADF_PCI_MAX_BARS 3
#define ADF_DEVICE_NAME_LENGTH 32
#define ADF_ETR_MAX_RINGS_PER_BANK 16
#define ADF_MAX_MSIX_VECTOR_NAME 16
#define ADF_DEVICE_NAME_PREFIX "qat_"

enum adf_accel_capabilities {
	ADF_ACCEL_CAPABILITIES_NULL = 0,
	ADF_ACCEL_CAPABILITIES_CRYPTO_SYMMETRIC = 1,
	ADF_ACCEL_CAPABILITIES_CRYPTO_ASYMMETRIC = 2,
	ADF_ACCEL_CAPABILITIES_CIPHER = 4,
	ADF_ACCEL_CAPABILITIES_AUTHENTICATION = 8,
	ADF_ACCEL_CAPABILITIES_COMPRESSION = 32,
	ADF_ACCEL_CAPABILITIES_LZS_COMPRESSION = 64,
	ADF_ACCEL_CAPABILITIES_RANDOM_NUMBER = 128
};

struct adf_bar {
	resource_size_t base_addr;
	void __iomem *virt_addr;
	resource_size_t size;
} __packed;

struct adf_accel_msix {
	struct msix_entry *entries;
	char **names;
	u32 num_entries;
} __packed;

struct adf_accel_pci {
	struct pci_dev *pci_dev;
	struct adf_accel_msix msix_entries;
	struct adf_bar pci_bars[ADF_PCI_MAX_BARS];
<<<<<<< HEAD
	uint8_t revid;
	uint8_t sku;
=======
	u8 revid;
	u8 sku;
>>>>>>> upstream/android-13
} __packed;

enum dev_state {
	DEV_DOWN = 0,
	DEV_UP
};

enum dev_sku_info {
	DEV_SKU_1 = 0,
	DEV_SKU_2,
	DEV_SKU_3,
	DEV_SKU_4,
	DEV_SKU_VF,
	DEV_SKU_UNKNOWN,
};

static inline const char *get_sku_info(enum dev_sku_info info)
{
	switch (info) {
	case DEV_SKU_1:
		return "SKU1";
	case DEV_SKU_2:
		return "SKU2";
	case DEV_SKU_3:
		return "SKU3";
	case DEV_SKU_4:
		return "SKU4";
	case DEV_SKU_VF:
		return "SKUVF";
	case DEV_SKU_UNKNOWN:
	default:
		break;
	}
	return "Unknown SKU";
}

struct adf_hw_device_class {
	const char *name;
	const enum adf_device_type type;
<<<<<<< HEAD
	uint32_t instances;
} __packed;

=======
	u32 instances;
} __packed;

struct arb_info {
	u32 arb_cfg;
	u32 arb_offset;
	u32 wt2sam_offset;
};

struct admin_info {
	u32 admin_msg_ur;
	u32 admin_msg_lr;
	u32 mailbox_offset;
};

struct adf_hw_csr_ops {
	u64 (*build_csr_ring_base_addr)(dma_addr_t addr, u32 size);
	u32 (*read_csr_ring_head)(void __iomem *csr_base_addr, u32 bank,
				  u32 ring);
	void (*write_csr_ring_head)(void __iomem *csr_base_addr, u32 bank,
				    u32 ring, u32 value);
	u32 (*read_csr_ring_tail)(void __iomem *csr_base_addr, u32 bank,
				  u32 ring);
	void (*write_csr_ring_tail)(void __iomem *csr_base_addr, u32 bank,
				    u32 ring, u32 value);
	u32 (*read_csr_e_stat)(void __iomem *csr_base_addr, u32 bank);
	void (*write_csr_ring_config)(void __iomem *csr_base_addr, u32 bank,
				      u32 ring, u32 value);
	void (*write_csr_ring_base)(void __iomem *csr_base_addr, u32 bank,
				    u32 ring, dma_addr_t addr);
	void (*write_csr_int_flag)(void __iomem *csr_base_addr, u32 bank,
				   u32 value);
	void (*write_csr_int_srcsel)(void __iomem *csr_base_addr, u32 bank);
	void (*write_csr_int_col_en)(void __iomem *csr_base_addr, u32 bank,
				     u32 value);
	void (*write_csr_int_col_ctl)(void __iomem *csr_base_addr, u32 bank,
				      u32 value);
	void (*write_csr_int_flag_and_col)(void __iomem *csr_base_addr,
					   u32 bank, u32 value);
	void (*write_csr_ring_srv_arb_en)(void __iomem *csr_base_addr, u32 bank,
					  u32 value);
};

>>>>>>> upstream/android-13
struct adf_cfg_device_data;
struct adf_accel_dev;
struct adf_etr_data;
struct adf_etr_ring_data;

struct adf_hw_device_data {
	struct adf_hw_device_class *dev_class;
<<<<<<< HEAD
	uint32_t (*get_accel_mask)(uint32_t fuse);
	uint32_t (*get_ae_mask)(uint32_t fuse);
	uint32_t (*get_sram_bar_id)(struct adf_hw_device_data *self);
	uint32_t (*get_misc_bar_id)(struct adf_hw_device_data *self);
	uint32_t (*get_etr_bar_id)(struct adf_hw_device_data *self);
	uint32_t (*get_num_aes)(struct adf_hw_device_data *self);
	uint32_t (*get_num_accels)(struct adf_hw_device_data *self);
	uint32_t (*get_pf2vf_offset)(uint32_t i);
	uint32_t (*get_vintmsk_offset)(uint32_t i);
=======
	u32 (*get_accel_mask)(struct adf_hw_device_data *self);
	u32 (*get_ae_mask)(struct adf_hw_device_data *self);
	u32 (*get_accel_cap)(struct adf_accel_dev *accel_dev);
	u32 (*get_sram_bar_id)(struct adf_hw_device_data *self);
	u32 (*get_misc_bar_id)(struct adf_hw_device_data *self);
	u32 (*get_etr_bar_id)(struct adf_hw_device_data *self);
	u32 (*get_num_aes)(struct adf_hw_device_data *self);
	u32 (*get_num_accels)(struct adf_hw_device_data *self);
	u32 (*get_pf2vf_offset)(u32 i);
	void (*get_arb_info)(struct arb_info *arb_csrs_info);
	void (*get_admin_info)(struct admin_info *admin_csrs_info);
>>>>>>> upstream/android-13
	enum dev_sku_info (*get_sku)(struct adf_hw_device_data *self);
	int (*alloc_irq)(struct adf_accel_dev *accel_dev);
	void (*free_irq)(struct adf_accel_dev *accel_dev);
	void (*enable_error_correction)(struct adf_accel_dev *accel_dev);
	int (*init_admin_comms)(struct adf_accel_dev *accel_dev);
	void (*exit_admin_comms)(struct adf_accel_dev *accel_dev);
	int (*send_admin_init)(struct adf_accel_dev *accel_dev);
	int (*init_arb)(struct adf_accel_dev *accel_dev);
	void (*exit_arb)(struct adf_accel_dev *accel_dev);
<<<<<<< HEAD
	void (*get_arb_mapping)(struct adf_accel_dev *accel_dev,
				const uint32_t **cfg);
	void (*disable_iov)(struct adf_accel_dev *accel_dev);
	void (*enable_ints)(struct adf_accel_dev *accel_dev);
	int (*enable_vf2pf_comms)(struct adf_accel_dev *accel_dev);
	void (*reset_device)(struct adf_accel_dev *accel_dev);
	const char *fw_name;
	const char *fw_mmp_name;
	uint32_t fuses;
	uint32_t accel_capabilities_mask;
	uint32_t instance_id;
	uint16_t accel_mask;
	uint16_t ae_mask;
	uint16_t tx_rings_mask;
	uint8_t tx_rx_gap;
	uint8_t num_banks;
	uint8_t num_accel;
	uint8_t num_logical_accel;
	uint8_t num_engines;
	uint8_t min_iov_compat_ver;
=======
	const u32 *(*get_arb_mapping)(void);
	int (*init_device)(struct adf_accel_dev *accel_dev);
	void (*disable_iov)(struct adf_accel_dev *accel_dev);
	void (*configure_iov_threads)(struct adf_accel_dev *accel_dev,
				      bool enable);
	void (*enable_ints)(struct adf_accel_dev *accel_dev);
	void (*set_ssm_wdtimer)(struct adf_accel_dev *accel_dev);
	int (*enable_pfvf_comms)(struct adf_accel_dev *accel_dev);
	void (*reset_device)(struct adf_accel_dev *accel_dev);
	void (*set_msix_rttable)(struct adf_accel_dev *accel_dev);
	char *(*uof_get_name)(u32 obj_num);
	u32 (*uof_get_num_objs)(void);
	u32 (*uof_get_ae_mask)(u32 obj_num);
	struct adf_hw_csr_ops csr_ops;
	const char *fw_name;
	const char *fw_mmp_name;
	u32 fuses;
	u32 straps;
	u32 accel_capabilities_mask;
	u32 instance_id;
	u16 accel_mask;
	u32 ae_mask;
	u32 admin_ae_mask;
	u16 tx_rings_mask;
	u8 tx_rx_gap;
	u8 num_banks;
	u8 num_rings_per_bank;
	u8 num_accel;
	u8 num_logical_accel;
	u8 num_engines;
	u8 min_iov_compat_ver;
>>>>>>> upstream/android-13
} __packed;

/* CSR write macro */
#define ADF_CSR_WR(csr_base, csr_offset, val) \
	__raw_writel(val, csr_base + csr_offset)

/* CSR read macro */
#define ADF_CSR_RD(csr_base, csr_offset) __raw_readl(csr_base + csr_offset)

#define GET_DEV(accel_dev) ((accel_dev)->accel_pci_dev.pci_dev->dev)
#define GET_BARS(accel_dev) ((accel_dev)->accel_pci_dev.pci_bars)
#define GET_HW_DATA(accel_dev) (accel_dev->hw_device)
#define GET_MAX_BANKS(accel_dev) (GET_HW_DATA(accel_dev)->num_banks)
<<<<<<< HEAD
#define GET_MAX_ACCELENGINES(accel_dev) (GET_HW_DATA(accel_dev)->num_engines)
=======
#define GET_NUM_RINGS_PER_BANK(accel_dev) \
	GET_HW_DATA(accel_dev)->num_rings_per_bank
#define GET_MAX_ACCELENGINES(accel_dev) (GET_HW_DATA(accel_dev)->num_engines)
#define GET_CSR_OPS(accel_dev) (&(accel_dev)->hw_device->csr_ops)
>>>>>>> upstream/android-13
#define accel_to_pci_dev(accel_ptr) accel_ptr->accel_pci_dev.pci_dev

struct adf_admin_comms;
struct icp_qat_fw_loader_handle;
struct adf_fw_loader_data {
	struct icp_qat_fw_loader_handle *fw_loader;
	const struct firmware *uof_fw;
	const struct firmware *mmp_fw;
};

struct adf_accel_vf_info {
	struct adf_accel_dev *accel_dev;
<<<<<<< HEAD
	struct tasklet_struct vf2pf_bh_tasklet;
=======
>>>>>>> upstream/android-13
	struct mutex pf2vf_lock; /* protect CSR access for PF2VF messages */
	struct ratelimit_state vf2pf_ratelimit;
	u32 vf_nr;
	bool init;
};

struct adf_accel_dev {
	struct adf_etr_data *transport;
	struct adf_hw_device_data *hw_device;
	struct adf_cfg_device_data *cfg;
	struct adf_fw_loader_data *fw_loader;
	struct adf_admin_comms *admin;
	struct list_head crypto_list;
	unsigned long status;
	atomic_t ref_count;
	struct dentry *debugfs_dir;
	struct list_head list;
	struct module *owner;
	struct adf_accel_pci accel_pci_dev;
	union {
		struct {
<<<<<<< HEAD
=======
			/* protects VF2PF interrupts access */
			spinlock_t vf2pf_ints_lock;
>>>>>>> upstream/android-13
			/* vf_info is non-zero when SR-IOV is init'ed */
			struct adf_accel_vf_info *vf_info;
		} pf;
		struct {
			char *irq_name;
			struct tasklet_struct pf2vf_bh_tasklet;
			struct mutex vf2pf_lock; /* protect CSR access */
			struct completion iov_msg_completion;
<<<<<<< HEAD
			uint8_t compatible;
			uint8_t pf_version;
=======
			u8 compatible;
			u8 pf_version;
>>>>>>> upstream/android-13
		} vf;
	};
	bool is_vf;
	u32 accel_id;
} __packed;
#endif
