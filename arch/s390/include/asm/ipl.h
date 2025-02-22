/* SPDX-License-Identifier: GPL-2.0 */
/*
 * s390 (re)ipl support
 *
 * Copyright IBM Corp. 2007
 */

#ifndef _ASM_S390_IPL_H
#define _ASM_S390_IPL_H

#include <asm/lowcore.h>
#include <asm/types.h>
#include <asm/cio.h>
#include <asm/setup.h>
<<<<<<< HEAD

#define NSS_NAME_SIZE	8

#define IPL_PARM_BLK_FCP_LEN (sizeof(struct ipl_list_hdr) + \
			      sizeof(struct ipl_block_fcp))

#define IPL_PARM_BLK0_FCP_LEN (sizeof(struct ipl_block_fcp) + 16)

#define IPL_PARM_BLK_CCW_LEN (sizeof(struct ipl_list_hdr) + \
			      sizeof(struct ipl_block_ccw))

#define IPL_PARM_BLK0_CCW_LEN (sizeof(struct ipl_block_ccw) + 16)

#define IPL_MAX_SUPPORTED_VERSION (0)

struct ipl_list_hdr {
	u32 len;
	u8  reserved1[3];
	u8  version;
	u32 blk0_len;
	u8  pbt;
	u8  flags;
	u16 reserved2;
	u8  loadparm[8];
} __attribute__((packed));

struct ipl_block_fcp {
	u8  reserved1[305-1];
	u8  opt;
	u8  reserved2[3];
	u16 reserved3;
	u16 devno;
	u8  reserved4[4];
	u64 wwpn;
	u64 lun;
	u32 bootprog;
	u8  reserved5[12];
	u64 br_lba;
	u32 scp_data_len;
	u8  reserved6[260];
	u8  scp_data[];
} __attribute__((packed));

#define DIAG308_VMPARM_SIZE	64
#define DIAG308_SCPDATA_SIZE	(PAGE_SIZE - (sizeof(struct ipl_list_hdr) + \
				 offsetof(struct ipl_block_fcp, scp_data)))

struct ipl_block_ccw {
	u8  reserved1[84];
	u16 reserved2 : 13;
	u8  ssid : 3;
	u16 devno;
	u8  vm_flags;
	u8  reserved3[3];
	u32 vm_parm_len;
	u8  nss_name[8];
	u8  vm_parm[DIAG308_VMPARM_SIZE];
	u8  reserved4[8];
} __attribute__((packed));

struct ipl_parameter_block {
	struct ipl_list_hdr hdr;
	union {
		struct ipl_block_fcp fcp;
		struct ipl_block_ccw ccw;
		char raw[PAGE_SIZE - sizeof(struct ipl_list_hdr)];
	} ipl_info;
} __packed __aligned(PAGE_SIZE);
=======
#include <asm/page.h>
#include <uapi/asm/ipl.h>

struct ipl_parameter_block {
	struct ipl_pl_hdr hdr;
	union {
		struct ipl_pb_hdr pb0_hdr;
		struct ipl_pb0_common common;
		struct ipl_pb0_fcp fcp;
		struct ipl_pb0_ccw ccw;
		struct ipl_pb0_nvme nvme;
		char raw[PAGE_SIZE - sizeof(struct ipl_pl_hdr)];
	};
} __packed __aligned(PAGE_SIZE);

#define NSS_NAME_SIZE 8

#define IPL_BP_FCP_LEN (sizeof(struct ipl_pl_hdr) + \
			      sizeof(struct ipl_pb0_fcp))
#define IPL_BP0_FCP_LEN (sizeof(struct ipl_pb0_fcp))

#define IPL_BP_NVME_LEN (sizeof(struct ipl_pl_hdr) + \
			      sizeof(struct ipl_pb0_nvme))
#define IPL_BP0_NVME_LEN (sizeof(struct ipl_pb0_nvme))

#define IPL_BP_CCW_LEN (sizeof(struct ipl_pl_hdr) + \
			      sizeof(struct ipl_pb0_ccw))
#define IPL_BP0_CCW_LEN (sizeof(struct ipl_pb0_ccw))

#define IPL_MAX_SUPPORTED_VERSION (0)

#define IPL_RB_CERT_UNKNOWN ((unsigned short)-1)

#define DIAG308_VMPARM_SIZE (64)
#define DIAG308_SCPDATA_OFFSET offsetof(struct ipl_parameter_block, \
					fcp.scp_data)
#define DIAG308_SCPDATA_SIZE (PAGE_SIZE - DIAG308_SCPDATA_OFFSET)
>>>>>>> upstream/android-13

struct save_area;
struct save_area * __init save_area_alloc(bool is_boot_cpu);
struct save_area * __init save_area_boot_cpu(void);
void __init save_area_add_regs(struct save_area *, void *regs);
void __init save_area_add_vxrs(struct save_area *, __vector128 *vxrs);

extern void s390_reset_system(void);
<<<<<<< HEAD
extern void ipl_store_parameters(void);
extern size_t append_ipl_vmparm(char *, size_t);
extern size_t append_ipl_scpdata(char *, size_t);
=======
extern size_t ipl_block_get_ascii_vmparm(char *dest, size_t size,
					 const struct ipl_parameter_block *ipb);
>>>>>>> upstream/android-13

enum ipl_type {
	IPL_TYPE_UNKNOWN	= 1,
	IPL_TYPE_CCW		= 2,
	IPL_TYPE_FCP		= 4,
	IPL_TYPE_FCP_DUMP	= 8,
	IPL_TYPE_NSS		= 16,
<<<<<<< HEAD
=======
	IPL_TYPE_NVME		= 32,
	IPL_TYPE_NVME_DUMP	= 64,
>>>>>>> upstream/android-13
};

struct ipl_info
{
	enum ipl_type type;
	union {
		struct {
			struct ccw_dev_id dev_id;
		} ccw;
		struct {
			struct ccw_dev_id dev_id;
			u64 wwpn;
			u64 lun;
		} fcp;
		struct {
<<<<<<< HEAD
=======
			u32 fid;
			u32 nsid;
		} nvme;
		struct {
>>>>>>> upstream/android-13
			char name[NSS_NAME_SIZE + 1];
		} nss;
	} data;
};

extern struct ipl_info ipl_info;
extern void setup_ipl(void);
extern void set_os_info_reipl_block(void);

<<<<<<< HEAD
=======
static inline bool is_ipl_type_dump(void)
{
	return (ipl_info.type == IPL_TYPE_FCP_DUMP) ||
		(ipl_info.type == IPL_TYPE_NVME_DUMP);
}

struct ipl_report {
	struct ipl_parameter_block *ipib;
	struct list_head components;
	struct list_head certificates;
	size_t size;
};

struct ipl_report_component {
	struct list_head list;
	struct ipl_rb_component_entry entry;
};

struct ipl_report_certificate {
	struct list_head list;
	struct ipl_rb_certificate_entry entry;
	void *key;
};

struct kexec_buf;
struct ipl_report *ipl_report_init(struct ipl_parameter_block *ipib);
void *ipl_report_finish(struct ipl_report *report);
int ipl_report_free(struct ipl_report *report);
int ipl_report_add_component(struct ipl_report *report, struct kexec_buf *kbuf,
			     unsigned char flags, unsigned short cert);
int ipl_report_add_certificate(struct ipl_report *report, void *key,
			       unsigned long addr, unsigned long len);

>>>>>>> upstream/android-13
/*
 * DIAG 308 support
 */
enum diag308_subcode  {
	DIAG308_REL_HSA = 2,
	DIAG308_LOAD_CLEAR = 3,
	DIAG308_LOAD_NORMAL_DUMP = 4,
	DIAG308_SET = 5,
	DIAG308_STORE = 6,
<<<<<<< HEAD
};

enum diag308_ipl_type {
	DIAG308_IPL_TYPE_FCP	= 0,
	DIAG308_IPL_TYPE_CCW	= 2,
};

enum diag308_opt {
	DIAG308_IPL_OPT_IPL	= 0x10,
	DIAG308_IPL_OPT_DUMP	= 0x20,
};

enum diag308_flags {
	DIAG308_FLAGS_LP_VALID	= 0x80,
};

enum diag308_vm_flags {
	DIAG308_VM_FLAGS_NSS_VALID	= 0x80,
	DIAG308_VM_FLAGS_VP_VALID	= 0x40,
=======
	DIAG308_LOAD_NORMAL = 7,
>>>>>>> upstream/android-13
};

enum diag308_rc {
	DIAG308_RC_OK		= 0x0001,
	DIAG308_RC_NOCONFIG	= 0x0102,
};

extern int diag308(unsigned long subcode, void *addr);
<<<<<<< HEAD
extern void diag308_reset(void);
=======
>>>>>>> upstream/android-13
extern void store_status(void (*fn)(void *), void *data);
extern void lgr_info_log(void);

#endif /* _ASM_S390_IPL_H */
