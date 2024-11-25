<<<<<<< HEAD
/* Copyright (c) 2010-2015, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2010-2015,2019 The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */
#ifndef __QCOM_SCM_INT_H
#define __QCOM_SCM_INT_H

<<<<<<< HEAD
#define QCOM_SCM_SVC_BOOT		0x1
#define QCOM_SCM_BOOT_ADDR		0x1
#define QCOM_SCM_SET_DLOAD_MODE		0x10
#define QCOM_SCM_BOOT_ADDR_MC		0x11
#define QCOM_SCM_SET_REMOTE_STATE	0xa
extern int __qcom_scm_set_remote_state(struct device *dev, u32 state, u32 id);
extern int __qcom_scm_set_dload_mode(struct device *dev, bool enable);

#define QCOM_SCM_FLAG_HLOS		0x01
#define QCOM_SCM_FLAG_COLDBOOT_MC	0x02
#define QCOM_SCM_FLAG_WARMBOOT_MC	0x04
extern int __qcom_scm_set_warm_boot_addr(struct device *dev, void *entry,
		const cpumask_t *cpus);
extern int __qcom_scm_set_cold_boot_addr(void *entry, const cpumask_t *cpus);

#define QCOM_SCM_CMD_TERMINATE_PC	0x2
#define QCOM_SCM_FLUSH_FLAG_MASK	0x3
#define QCOM_SCM_CMD_CORE_HOTPLUGGED	0x10
extern void __qcom_scm_cpu_power_down(u32 flags);

#define QCOM_SCM_SVC_IO			0x5
#define QCOM_SCM_IO_READ		0x1
#define QCOM_SCM_IO_WRITE		0x2
extern int __qcom_scm_io_readl(struct device *dev, phys_addr_t addr, unsigned int *val);
extern int __qcom_scm_io_writel(struct device *dev, phys_addr_t addr, unsigned int val);

#define QCOM_SCM_SVC_INFO		0x6
#define QCOM_IS_CALL_AVAIL_CMD		0x1
extern int __qcom_scm_is_call_available(struct device *dev, u32 svc_id,
		u32 cmd_id);

#define QCOM_SCM_SVC_HDCP		0x11
#define QCOM_SCM_CMD_HDCP		0x01
extern int __qcom_scm_hdcp_req(struct device *dev,
		struct qcom_scm_hdcp_req *req, u32 req_cnt, u32 *resp);

extern void __qcom_scm_init(void);

#define QCOM_SCM_SVC_PIL		0x2
#define QCOM_SCM_PAS_INIT_IMAGE_CMD	0x1
#define QCOM_SCM_PAS_MEM_SETUP_CMD	0x2
#define QCOM_SCM_PAS_AUTH_AND_RESET_CMD	0x5
#define QCOM_SCM_PAS_SHUTDOWN_CMD	0x6
#define QCOM_SCM_PAS_IS_SUPPORTED_CMD	0x7
#define QCOM_SCM_PAS_MSS_RESET		0xa
extern bool __qcom_scm_pas_supported(struct device *dev, u32 peripheral);
extern int  __qcom_scm_pas_init_image(struct device *dev, u32 peripheral,
		dma_addr_t metadata_phys);
extern int  __qcom_scm_pas_mem_setup(struct device *dev, u32 peripheral,
		phys_addr_t addr, phys_addr_t size);
extern int  __qcom_scm_pas_auth_and_reset(struct device *dev, u32 peripheral);
extern int  __qcom_scm_pas_shutdown(struct device *dev, u32 peripheral);
extern int  __qcom_scm_pas_mss_reset(struct device *dev, bool reset);

=======
enum qcom_scm_convention {
	SMC_CONVENTION_UNKNOWN,
	SMC_CONVENTION_LEGACY,
	SMC_CONVENTION_ARM_32,
	SMC_CONVENTION_ARM_64,
};

extern enum qcom_scm_convention qcom_scm_convention;

#define MAX_QCOM_SCM_ARGS 10
#define MAX_QCOM_SCM_RETS 3

enum qcom_scm_arg_types {
	QCOM_SCM_VAL,
	QCOM_SCM_RO,
	QCOM_SCM_RW,
	QCOM_SCM_BUFVAL,
};

#define QCOM_SCM_ARGS_IMPL(num, a, b, c, d, e, f, g, h, i, j, ...) (\
			   (((a) & 0x3) << 4) | \
			   (((b) & 0x3) << 6) | \
			   (((c) & 0x3) << 8) | \
			   (((d) & 0x3) << 10) | \
			   (((e) & 0x3) << 12) | \
			   (((f) & 0x3) << 14) | \
			   (((g) & 0x3) << 16) | \
			   (((h) & 0x3) << 18) | \
			   (((i) & 0x3) << 20) | \
			   (((j) & 0x3) << 22) | \
			   ((num) & 0xf))

#define QCOM_SCM_ARGS(...) QCOM_SCM_ARGS_IMPL(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)


/**
 * struct qcom_scm_desc
 * @arginfo:	Metadata describing the arguments in args[]
 * @args:	The array of arguments for the secure syscall
 */
struct qcom_scm_desc {
	u32 svc;
	u32 cmd;
	u32 arginfo;
	u64 args[MAX_QCOM_SCM_ARGS];
	u32 owner;
};

/**
 * struct qcom_scm_res
 * @result:	The values returned by the secure syscall
 */
struct qcom_scm_res {
	u64 result[MAX_QCOM_SCM_RETS];
};

#define SCM_SMC_FNID(s, c)	((((s) & 0xFF) << 8) | ((c) & 0xFF))
extern int __scm_smc_call(struct device *dev, const struct qcom_scm_desc *desc,
			  enum qcom_scm_convention qcom_convention,
			  struct qcom_scm_res *res, bool atomic);
#define scm_smc_call(dev, desc, res, atomic) \
	__scm_smc_call((dev), (desc), qcom_scm_convention, (res), (atomic))

#define SCM_LEGACY_FNID(s, c)	(((s) << 10) | ((c) & 0x3ff))
extern int scm_legacy_call_atomic(struct device *dev,
				  const struct qcom_scm_desc *desc,
				  struct qcom_scm_res *res);
extern int scm_legacy_call(struct device *dev, const struct qcom_scm_desc *desc,
			   struct qcom_scm_res *res);

#define QCOM_SCM_SVC_BOOT		0x01
#define QCOM_SCM_BOOT_SET_ADDR		0x01
#define QCOM_SCM_BOOT_TERMINATE_PC	0x02
#define QCOM_SCM_BOOT_SET_DLOAD_MODE	0x10
#define QCOM_SCM_BOOT_SET_REMOTE_STATE	0x0a
#define QCOM_SCM_FLUSH_FLAG_MASK	0x3

#define QCOM_SCM_SVC_PIL		0x02
#define QCOM_SCM_PIL_PAS_INIT_IMAGE	0x01
#define QCOM_SCM_PIL_PAS_MEM_SETUP	0x02
#define QCOM_SCM_PIL_PAS_AUTH_AND_RESET	0x05
#define QCOM_SCM_PIL_PAS_SHUTDOWN	0x06
#define QCOM_SCM_PIL_PAS_IS_SUPPORTED	0x07
#define QCOM_SCM_PIL_PAS_MSS_RESET	0x0a

#define QCOM_SCM_SVC_IO			0x05
#define QCOM_SCM_IO_READ		0x01
#define QCOM_SCM_IO_WRITE		0x02

#define QCOM_SCM_SVC_INFO		0x06
#define QCOM_SCM_INFO_IS_CALL_AVAIL	0x01

#define QCOM_SCM_SVC_MP				0x0c
#define QCOM_SCM_MP_RESTORE_SEC_CFG		0x02
#define QCOM_SCM_MP_IOMMU_SECURE_PTBL_SIZE	0x03
#define QCOM_SCM_MP_IOMMU_SECURE_PTBL_INIT	0x04
#define QCOM_SCM_MP_VIDEO_VAR			0x08
#define QCOM_SCM_MP_ASSIGN			0x16

#define QCOM_SCM_SVC_OCMEM		0x0f
#define QCOM_SCM_OCMEM_LOCK_CMD		0x01
#define QCOM_SCM_OCMEM_UNLOCK_CMD	0x02

#define QCOM_SCM_SVC_ES			0x10	/* Enterprise Security */
#define QCOM_SCM_ES_INVALIDATE_ICE_KEY	0x03
#define QCOM_SCM_ES_CONFIG_SET_ICE_KEY	0x04

#define QCOM_SCM_SVC_HDCP		0x11
#define QCOM_SCM_HDCP_INVOKE		0x01

#define QCOM_SCM_SVC_LMH			0x13
#define QCOM_SCM_LMH_LIMIT_PROFILE_CHANGE	0x01
#define QCOM_SCM_LMH_LIMIT_DCVSH		0x10

#define QCOM_SCM_SVC_SMMU_PROGRAM		0x15
#define QCOM_SCM_SMMU_CONFIG_ERRATA1		0x03
#define QCOM_SCM_SMMU_CONFIG_ERRATA1_CLIENT_ALL	0x02

extern void __qcom_scm_init(void);

>>>>>>> upstream/android-13
/* common error codes */
#define QCOM_SCM_V2_EBUSY	-12
#define QCOM_SCM_ENOMEM		-5
#define QCOM_SCM_EOPNOTSUPP	-4
#define QCOM_SCM_EINVAL_ADDR	-3
#define QCOM_SCM_EINVAL_ARG	-2
#define QCOM_SCM_ERROR		-1
#define QCOM_SCM_INTERRUPTED	1

static inline int qcom_scm_remap_error(int err)
{
	switch (err) {
	case QCOM_SCM_ERROR:
		return -EIO;
	case QCOM_SCM_EINVAL_ADDR:
	case QCOM_SCM_EINVAL_ARG:
		return -EINVAL;
	case QCOM_SCM_EOPNOTSUPP:
		return -EOPNOTSUPP;
	case QCOM_SCM_ENOMEM:
		return -ENOMEM;
	case QCOM_SCM_V2_EBUSY:
		return -EBUSY;
	}
	return -EINVAL;
}

<<<<<<< HEAD
#define QCOM_SCM_SVC_MP			0xc
#define QCOM_SCM_RESTORE_SEC_CFG	2
extern int __qcom_scm_restore_sec_cfg(struct device *dev, u32 device_id,
				      u32 spare);
#define QCOM_SCM_IOMMU_SECURE_PTBL_SIZE	3
#define QCOM_SCM_IOMMU_SECURE_PTBL_INIT	4
extern int __qcom_scm_iommu_secure_ptbl_size(struct device *dev, u32 spare,
					     size_t *size);
extern int __qcom_scm_iommu_secure_ptbl_init(struct device *dev, u64 addr,
					     u32 size, u32 spare);
#define QCOM_MEM_PROT_ASSIGN_ID	0x16
extern int  __qcom_scm_assign_mem(struct device *dev,
				  phys_addr_t mem_region, size_t mem_sz,
				  phys_addr_t src, size_t src_sz,
				  phys_addr_t dest, size_t dest_sz);

=======
>>>>>>> upstream/android-13
#endif
