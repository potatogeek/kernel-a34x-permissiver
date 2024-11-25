<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2017-2018 Samsung Electronics Co., Ltd.
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Universal Flash Storage Host Performance Booster
 *
 * Copyright (C) 2017-2021 Samsung Electronics Co., Ltd.
 *
 * Authors:
 *	Yongmyung Lee <ymhungry.lee@samsung.com>
 *	Jinyoung Choi <j-young.choi@samsung.com>
>>>>>>> upstream/android-13
 */

#ifndef _UFSHPB_H_
#define _UFSHPB_H_

<<<<<<< HEAD
#include <linux/interrupt.h>
#include <linux/sysfs.h>
#include <linux/blktrace_api.h>
#include <linux/blkdev.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_driver.h>

#include "../../../block/blk.h"
#include "../scsi_priv.h"

/* Version info*/
#define UFSHPB_VER				0x0200
#define UFSHPB_DD_VER				0x0208

/* Constant value*/
#define MAX_ACTIVE_NUM				2
#define MAX_INACTIVE_NUM			2

#define HPB_ENTRY_SIZE				0x08
#define HPB_ENTREIS_PER_OS_PAGE			(OS_PAGE_SIZE / HPB_ENTRY_SIZE)

#define RETRY_DELAY_MS				5000

/* HPB Support Chunk Size */
#define HPB_MULTI_CHUNK_LOW			9
#define HPB_MULTI_CHUNK_HIGH			128
#define MAX_HPB_CONTEXT_ID			0x7f

/* Description */
#define UFS_FEATURE_SUPPORT_HPB_BIT		0x80

/* Response UPIU types */
#define HPB_RSP_NONE				0x00
#define HPB_RSP_REQ_REGION_UPDATE		0x01

/* Vender defined OPCODE */
#define UFSHPB_READ_BUFFER			0xF9
#define UFSHPB_WRITE_BUFFER			0xFA

#define UFSHPB_GROUP_NUMBER			0x11
#define UFSHPB_READ_BUFFER_ID			0x01
#define UFSHPB_WRITE_BUFFER_ID			0x02
#define TRANSFER_LEN				0x01

=======
/* hpb response UPIU macro */
#define HPB_RSP_NONE				0x0
#define HPB_RSP_REQ_REGION_UPDATE		0x1
#define HPB_RSP_DEV_RESET			0x2
#define MAX_ACTIVE_NUM				2
#define MAX_INACTIVE_NUM			2
>>>>>>> upstream/android-13
#define DEV_DATA_SEG_LEN			0x14
#define DEV_SENSE_SEG_LEN			0x12
#define DEV_DES_TYPE				0x80
#define DEV_ADDITIONAL_LEN			0x10

<<<<<<< HEAD
/* For read10 debug */
#define READ10_DEBUG_LUN			0x7F
#define READ10_DEBUG_LBA			0x48504230

/*
 * UFSHPB DEBUG
 */

#define HPB_DEBUG(hpb, msg, args...)			\
	do { if (hpb->debug)				\
		printk(KERN_ERR "%s:%d " msg "\n",	\
		       __func__, __LINE__, ##args);	\
	} while (0)

#define TMSG_CMD(hpb, msg, rq, rgn, srgn)				\
	do { if (hpb->ufsf->sdev_ufs_lu[hpb->lun] &&			\
		 hpb->ufsf->sdev_ufs_lu[hpb->lun]->request_queue)	\
			blk_add_trace_msg(				\
			hpb->ufsf->sdev_ufs_lu[hpb->lun]->request_queue,\
			"%llu + %u " msg " %d - %d",			\
			(unsigned long long) blk_rq_pos(rq),		\
			(unsigned int) blk_rq_sectors(rq), rgn, srgn);	\
	} while (0)

enum UFSHPB_STATE {
	HPB_PRESENT = 1,
	HPB_NOT_SUPPORTED = -1,
	HPB_FAILED = -2,
	HPB_NEED_INIT = 0,
	HPB_RESET = -3,
};

enum HPBREGION_STATE {
	HPBREGION_INACTIVE, HPBREGION_ACTIVE, HPBREGION_PINNED,
};

enum HPBSUBREGION_STATE {
	HPBSUBREGION_UNUSED,
	HPBSUBREGION_DIRTY,
	HPBSUBREGION_CLEAN,
	HPBSUBREGION_ISSUED,
};

struct ufshpb_dev_info {
	bool hpb_device;
	int hpb_number_lu;
	int hpb_ver;
	int hpb_rgn_size;
	int hpb_srgn_size;
	int hpb_device_max_active_rgns;
};

struct ufshpb_active_field {
	__be16 active_rgn;
	__be16 active_srgn;
};

struct ufshpb_rsp_field {
	__be16 sense_data_len;
	u8 desc_type;
	u8 additional_len;
	u8 hpb_type;
	u8 reserved;
	u8 active_rgn_cnt;
	u8 inactive_rgn_cnt;
	struct ufshpb_active_field hpb_active_field[2];
	__be16 hpb_inactive_field[2];
=======
/* hpb map & entries macro */
#define HPB_RGN_SIZE_UNIT			512
#define HPB_ENTRY_BLOCK_SIZE			4096
#define HPB_ENTRY_SIZE				0x8
#define PINNED_NOT_SET				U32_MAX

/* hpb support chunk size */
#define HPB_LEGACY_CHUNK_HIGH			1
#define HPB_MULTI_CHUNK_HIGH			255

/* hpb vender defined opcode */
#define UFSHPB_READ				0xF8
#define UFSHPB_READ_BUFFER			0xF9
#define UFSHPB_READ_BUFFER_ID			0x01
#define UFSHPB_WRITE_BUFFER			0xFA
#define UFSHPB_WRITE_BUFFER_INACT_SINGLE_ID	0x01
#define UFSHPB_WRITE_BUFFER_PREFETCH_ID		0x02
#define UFSHPB_WRITE_BUFFER_INACT_ALL_ID	0x03
#define HPB_WRITE_BUFFER_CMD_LENGTH		10
#define MAX_HPB_READ_ID				0x7F
#define HPB_READ_BUFFER_CMD_LENGTH		10
#define LU_ENABLED_HPB_FUNC			0x02

#define HPB_RESET_REQ_RETRIES			10
#define HPB_MAP_REQ_RETRIES			5
#define HPB_REQUEUE_TIME_MS			0

#define HPB_SUPPORT_VERSION			0x200
#define HPB_SUPPORT_LEGACY_VERSION		0x100

enum UFSHPB_MODE {
	HPB_HOST_CONTROL,
	HPB_DEVICE_CONTROL,
};

enum UFSHPB_STATE {
	HPB_INIT = 0,
	HPB_PRESENT = 1,
	HPB_SUSPEND,
	HPB_FAILED,
	HPB_RESET,
};

enum HPB_RGN_STATE {
	HPB_RGN_INACTIVE,
	HPB_RGN_ACTIVE,
	/* pinned regions are always active */
	HPB_RGN_PINNED,
};

enum HPB_SRGN_STATE {
	HPB_SRGN_UNUSED,
	HPB_SRGN_INVALID,
	HPB_SRGN_VALID,
	HPB_SRGN_ISSUED,
};

/**
 * struct ufshpb_lu_info - UFSHPB logical unit related info
 * @num_blocks: the number of logical block
 * @pinned_start: the start region number of pinned region
 * @num_pinned: the number of pinned regions
 * @max_active_rgns: maximum number of active regions
 */
struct ufshpb_lu_info {
	int num_blocks;
	int pinned_start;
	int num_pinned;
	int max_active_rgns;
>>>>>>> upstream/android-13
};

struct ufshpb_map_ctx {
	struct page **m_page;
<<<<<<< HEAD
	unsigned int *ppn_dirty;

	struct list_head list_table;
=======
	unsigned long *ppn_dirty;
>>>>>>> upstream/android-13
};

struct ufshpb_subregion {
	struct ufshpb_map_ctx *mctx;
<<<<<<< HEAD
	enum HPBSUBREGION_STATE srgn_state;
	int rgn_idx;
	int srgn_idx;
=======
	enum HPB_SRGN_STATE srgn_state;
	int rgn_idx;
	int srgn_idx;
	bool is_last;

	/* subregion reads - for host mode */
	unsigned int reads;
>>>>>>> upstream/android-13

	/* below information is used by rsp_list */
	struct list_head list_act_srgn;
};

struct ufshpb_region {
<<<<<<< HEAD
	struct ufshpb_subregion *srgn_tbl;
	enum HPBREGION_STATE rgn_state;
=======
	struct ufshpb_lu *hpb;
	struct ufshpb_subregion *srgn_tbl;
	enum HPB_RGN_STATE rgn_state;
>>>>>>> upstream/android-13
	int rgn_idx;
	int srgn_cnt;

	/* below information is used by rsp_list */
	struct list_head list_inact_rgn;

	/* below information is used by lru */
	struct list_head list_lru_rgn;
<<<<<<< HEAD
};

=======
	unsigned long rgn_flags;
#define RGN_FLAG_DIRTY 0
#define RGN_FLAG_UPDATE 1

	/* region reads - for host mode */
	spinlock_t rgn_lock;
	unsigned int reads;
	/* region "cold" timer - for host mode */
	ktime_t read_timeout;
	unsigned int read_timeout_expiries;
	struct list_head list_expired_rgn;
};

#define for_each_sub_region(rgn, i, srgn)				\
	for ((i) = 0;							\
	     ((i) < (rgn)->srgn_cnt) && ((srgn) = &(rgn)->srgn_tbl[i]); \
	     (i)++)

/**
 * struct ufshpb_req - HPB related request structure (write/read buffer)
 * @req: block layer request structure
 * @bio: bio for this request
 * @hpb: ufshpb_lu structure that related to
 * @list_req: ufshpb_req mempool list
 * @sense: store its sense data
 * @mctx: L2P map information
 * @rgn_idx: target region index
 * @srgn_idx: target sub-region index
 * @lun: target logical unit number
 * @m_page: L2P map information data for pre-request
 * @len: length of host-side cached L2P map in m_page
 * @lpn: start LPN of L2P map in m_page
 */
>>>>>>> upstream/android-13
struct ufshpb_req {
	struct request *req;
	struct bio *bio;
	struct ufshpb_lu *hpb;
	struct list_head list_req;
<<<<<<< HEAD
	void (*end_io)(struct request *rq, int err);
	void *end_io_data;
	char sense[SCSI_SENSE_BUFFERSIZE];

=======
>>>>>>> upstream/android-13
	union {
		struct {
			struct ufshpb_map_ctx *mctx;
			unsigned int rgn_idx;
			unsigned int srgn_idx;
			unsigned int lun;
		} rb;
		struct {
			struct page *m_page;
			unsigned int len;
			unsigned long lpn;
		} wb;
	};
};

<<<<<<< HEAD
enum selection_type {
	LRU = 1,
};

struct victim_select_info {
	int selection_type;
	struct list_head lh_lru_rgn;
	int max_lru_active_cnt; /* supported hpb #region - pinned #region */
	atomic64_t active_cnt;
};

struct ufshpb_lu {
	struct ufsf_feature *ufsf;
	u8 lun;
	int qd;
	struct ufshpb_region *rgn_tbl;

	spinlock_t hpb_lock;

	struct ufshpb_req *map_req;
	int num_inflight_map_req;
	int throttle_map_req;
	struct list_head lh_map_req_free;
	struct list_head lh_map_req_retry;
	struct list_head lh_map_ctx_free;

	spinlock_t rsp_list_lock;
	struct list_head lh_pinned_srgn;
	struct list_head lh_act_srgn;
	struct list_head lh_inact_rgn;

	struct kobject kobj;
	struct mutex sysfs_lock;
	struct ufshpb_sysfs_entry *sysfs_entries;

	struct ufshpb_req *pre_req;
	int num_inflight_pre_req;
	int throttle_pre_req;
	struct list_head lh_pre_req_free;
	struct list_head lh_pre_req_dummy; /* dummy for blk_start_requests() */
	int ctx_id_ticket;
	int pre_req_min_tr_len;
	int pre_req_max_tr_len;

	struct work_struct ufshpb_work;
	struct delayed_work ufshpb_retry_work;
	struct work_struct ufshpb_task_workq;

	/* for selecting victim */
	struct victim_select_info lru_info;

	int hpb_ver;
	int lu_max_active_rgns;
	int lu_pinned_rgn_startidx;
	int lu_pinned_end_offset;
	int lu_num_pinned_rgns;
	int srgns_per_lu;
	int rgns_per_lu;
	int srgns_per_rgn;
	int srgn_mem_size;
	int entries_per_rgn_shift;
	int entries_per_rgn_mask;
	int entries_per_srgn;
	int entries_per_srgn_shift;
	int entries_per_srgn_mask;
	int dwords_per_srgn;
	unsigned long long srgn_unit_size;
	int mpage_bytes;
	int mpages_per_srgn;
	int lu_num_blocks;

	/* for debug */
	int alloc_mctx;
	int debug_free_table;
	bool force_disable;
	bool force_map_req_disable;
	bool debug;
	atomic64_t hit;
	atomic64_t miss;
	atomic64_t rb_noti_cnt;
	atomic64_t rb_active_cnt;
	atomic64_t rb_inactive_cnt;
	atomic64_t map_req_cnt;
	atomic64_t pre_req_cnt;
};

struct ufshpb_sysfs_entry {
	struct attribute    attr;
	ssize_t (*show)(struct ufshpb_lu *hpb, char *buf);
	ssize_t (*store)(struct ufshpb_lu *hpb, const char *, size_t);
=======
struct victim_select_info {
	struct list_head lh_lru_rgn; /* LRU list of regions */
	int max_lru_active_cnt; /* supported hpb #region - pinned #region */
	atomic_t active_cnt;
};

/**
 * ufshpb_params - ufs hpb parameters
 * @requeue_timeout_ms - requeue threshold of wb command (0x2)
 * @activation_thld - min reads [IOs] to activate/update a region
 * @normalization_factor - shift right the region's reads
 * @eviction_thld_enter - min reads [IOs] for the entering region in eviction
 * @eviction_thld_exit - max reads [IOs] for the exiting region in eviction
 * @read_timeout_ms - timeout [ms] from the last read IO to the region
 * @read_timeout_expiries - amount of allowable timeout expireis
 * @timeout_polling_interval_ms - frequency in which timeouts are checked
 * @inflight_map_req - number of inflight map requests
 */
struct ufshpb_params {
	unsigned int requeue_timeout_ms;
	unsigned int activation_thld;
	unsigned int normalization_factor;
	unsigned int eviction_thld_enter;
	unsigned int eviction_thld_exit;
	unsigned int read_timeout_ms;
	unsigned int read_timeout_expiries;
	unsigned int timeout_polling_interval_ms;
	unsigned int inflight_map_req;
};

struct ufshpb_stats {
	u64 hit_cnt;
	u64 miss_cnt;
	u64 rb_noti_cnt;
	u64 rb_active_cnt;
	u64 rb_inactive_cnt;
	u64 map_req_cnt;
	u64 pre_req_cnt;
	u64 umap_req_cnt;
};

struct ufshpb_lu {
	int lun;
	struct scsi_device *sdev_ufs_lu;

	spinlock_t rgn_state_lock; /* for protect rgn/srgn state */
	struct ufshpb_region *rgn_tbl;

	atomic_t hpb_state;

	spinlock_t rsp_list_lock;
	struct list_head lh_act_srgn; /* hold rsp_list_lock */
	struct list_head lh_inact_rgn; /* hold rsp_list_lock */

	/* pre request information */
	struct ufshpb_req *pre_req;
	int num_inflight_pre_req;
	int throttle_pre_req;
	int num_inflight_map_req; /* hold param_lock */
	spinlock_t param_lock;

	struct list_head lh_pre_req_free;
	int pre_req_max_tr_len;

	/* cached L2P map management worker */
	struct work_struct map_work;

	/* for selecting victim */
	struct victim_select_info lru_info;
	struct work_struct ufshpb_normalization_work;
	struct delayed_work ufshpb_read_to_work;
	unsigned long work_data_bits;
#define TIMEOUT_WORK_RUNNING 0

	/* pinned region information */
	u32 lu_pinned_start;
	u32 lu_pinned_end;

	/* HPB related configuration */
	u32 rgns_per_lu;
	u32 srgns_per_lu;
	u32 last_srgn_entries;
	int srgns_per_rgn;
	u32 srgn_mem_size;
	u32 entries_per_rgn_mask;
	u32 entries_per_rgn_shift;
	u32 entries_per_srgn;
	u32 entries_per_srgn_mask;
	u32 entries_per_srgn_shift;
	u32 pages_per_srgn;

	bool is_hcm;

	struct ufshpb_stats stats;
	struct ufshpb_params params;

	struct kmem_cache *map_req_cache;
	struct kmem_cache *m_page_cache;

	struct list_head list_hpb_lu;
>>>>>>> upstream/android-13
};

struct ufs_hba;
struct ufshcd_lrb;

<<<<<<< HEAD
int ufshpb_prepare_pre_req(struct ufsf_feature *ufsf, struct scsi_cmnd *cmd,
			   u8 lun);
int ufshpb_prepare_add_lrbp(struct ufsf_feature *ufsf, int add_tag);
void ufshpb_end_pre_req(struct ufsf_feature *ufsf, struct request *req);
void ufshpb_get_dev_info(struct ufshpb_dev_info *hpb_dev_info, u8 *desc_buf);
void ufshpb_get_geo_info(struct ufshpb_dev_info *hpb_dev_info, u8 *geo_buf);
int ufshpb_get_lu_info(struct ufsf_feature *ufsf, u8 lun, u8 *unit_buf);
void ufshpb_init_handler(struct work_struct *work);
void ufshpb_reset_handler(struct work_struct *work);
void ufshpb_prep_fn(struct ufsf_feature *ufsf, struct ufshcd_lrb *lrbp);
void ufshpb_rsp_upiu(struct ufsf_feature *ufsf, struct ufshcd_lrb *lrbp);
void ufshpb_release(struct ufsf_feature *ufsf, int state);
int ufshpb_issue_req_dev_ctx(struct ufshpb_lu *hpb, unsigned char *buf,
			     int buf_length);
void ufshpb_resume(struct ufsf_feature *ufsf);
void ufshpb_suspend(struct ufsf_feature *ufsf);
=======
#ifndef CONFIG_SCSI_UFS_HPB
static int ufshpb_prep(struct ufs_hba *hba, struct ufshcd_lrb *lrbp) { return 0; }
static void ufshpb_rsp_upiu(struct ufs_hba *hba, struct ufshcd_lrb *lrbp) {}
static void ufshpb_resume(struct ufs_hba *hba) {}
static void ufshpb_suspend(struct ufs_hba *hba) {}
static void ufshpb_reset(struct ufs_hba *hba) {}
static void ufshpb_reset_host(struct ufs_hba *hba) {}
static void ufshpb_init(struct ufs_hba *hba) {}
static void ufshpb_init_hpb_lu(struct ufs_hba *hba, struct scsi_device *sdev) {}
static void ufshpb_destroy_lu(struct ufs_hba *hba, struct scsi_device *sdev) {}
static void ufshpb_remove(struct ufs_hba *hba) {}
static bool ufshpb_is_allowed(struct ufs_hba *hba) { return false; }
static void ufshpb_get_geo_info(struct ufs_hba *hba, u8 *geo_buf) {}
static void ufshpb_get_dev_info(struct ufs_hba *hba, u8 *desc_buf) {}
static bool ufshpb_is_legacy(struct ufs_hba *hba) { return false; }
#else
int ufshpb_prep(struct ufs_hba *hba, struct ufshcd_lrb *lrbp);
void ufshpb_rsp_upiu(struct ufs_hba *hba, struct ufshcd_lrb *lrbp);
void ufshpb_resume(struct ufs_hba *hba);
void ufshpb_suspend(struct ufs_hba *hba);
void ufshpb_reset(struct ufs_hba *hba);
void ufshpb_reset_host(struct ufs_hba *hba);
void ufshpb_init(struct ufs_hba *hba);
void ufshpb_init_hpb_lu(struct ufs_hba *hba, struct scsi_device *sdev);
void ufshpb_destroy_lu(struct ufs_hba *hba, struct scsi_device *sdev);
void ufshpb_remove(struct ufs_hba *hba);
bool ufshpb_is_allowed(struct ufs_hba *hba);
void ufshpb_get_geo_info(struct ufs_hba *hba, u8 *geo_buf);
void ufshpb_get_dev_info(struct ufs_hba *hba, u8 *desc_buf);
bool ufshpb_is_legacy(struct ufs_hba *hba);
extern struct attribute_group ufs_sysfs_hpb_stat_group;
extern struct attribute_group ufs_sysfs_hpb_param_group;
#endif

>>>>>>> upstream/android-13
#endif /* End of Header */
