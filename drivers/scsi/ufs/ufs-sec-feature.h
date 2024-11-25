// SPDX-License-Identifier: GPL-2.0
/*
 * Samsung Specific feature
 *
<<<<<<< HEAD
 * Copyright (C) 2021 Samsung Electronics Co., Ltd.
 *
 * Authors:
 *	Storage Driver <storage.sec@samsung.com>
=======
 * Copyright (C) 2022 Samsung Electronics Co., Ltd.
 *
 * Authors:
 * Storage Driver <storage.sec@samsung.com>
>>>>>>> upstream/android-13
 */

#ifndef __UFS_SEC_FEATURE_H__
#define __UFS_SEC_FEATURE_H__

#include "ufshcd.h"
<<<<<<< HEAD

/* unique number */
#define SERIAL_NUM_SIZE 7

/* the user LU for mtk solution is set as 2 */
#define UFS_USER_LUN 2

#define SEC_UFS_ERR_COUNT_INC(count, max) ((count) += ((count) < (max)) ? 1 : 0)

struct SEC_UFS_op_count {
	unsigned int HW_RESET_count;
	unsigned int link_startup_count;
	unsigned int Hibern8_enter_count;
	unsigned int Hibern8_exit_count;
	unsigned int op_err;
};

struct SEC_UFS_UIC_err_count {
	u8 PA_ERR_cnt;
	u8 DL_PA_INIT_ERROR_cnt;
	u8 DL_NAC_RECEIVED_ERROR_cnt;
	u8 DL_TC_REPLAY_ERROR_cnt;
	u8 NL_ERROR_cnt;
	u8 TL_ERROR_cnt;
	u8 DME_ERROR_cnt;
	unsigned int UIC_err;
	unsigned int PA_ERR_linereset;
	unsigned int PA_ERR_lane[3];
};

struct SEC_UFS_Fatal_err_count {
	u8 DFE;         // Device_Fatal
	u8 CFE;         // Controller_Fatal
	u8 SBFE;        // System_Bus_Fatal
	u8 CEFE;        // Crypto_Engine_Fatal
	u8 LLE;         // Link Lost
	unsigned int Fatal_err;
};

struct SEC_UFS_UTP_count {
	u8 UTMR_query_task_count;
	u8 UTMR_abort_task_count;
	u8 UTMR_logical_reset_count;
	u8 UTR_read_err;
	u8 UTR_write_err;
	u8 UTR_sync_cache_err;
	u8 UTR_unmap_err;
	u8 UTR_etc_err;
	unsigned int UTP_err;
};

struct SEC_UFS_UIC_cmd_count {
	u8 DME_GET_err;
	u8 DME_SET_err;
	u8 DME_PEER_GET_err;
	u8 DME_PEER_SET_err;
	u8 DME_POWERON_err;
	u8 DME_POWEROFF_err;
	u8 DME_ENABLE_err;
	u8 DME_RESET_err;
	u8 DME_END_PT_RST_err;
	u8 DME_LINK_STARTUP_err;
	u8 DME_HIBER_ENTER_err;
	u8 DME_HIBER_EXIT_err;
	u8 DME_TEST_MODE_err;
	unsigned int UIC_cmd_err;
};

struct SEC_UFS_QUERY_count {
	u8 NOP_err;
	u8 R_Desc_err;
	u8 W_Desc_err;
	u8 R_Attr_err;
	u8 W_Attr_err;
	u8 R_Flag_err;
	u8 Set_Flag_err;
	u8 Clear_Flag_err;
	u8 Toggle_Flag_err;
	unsigned int Query_err;
};

struct SEC_SCSI_SENSE_count {
	unsigned int scsi_medium_err;
	unsigned int scsi_hw_err;
};

#define SEC_MAX_LBA_LOGGING	10
#define SEC_ISSUE_REGION_STEP	(250 * 1024 / 4)	/* 250MB : 1 LBA = 4KB */
struct SEC_SCSI_SENSE_err_log {
	unsigned long issue_LBA_list[SEC_MAX_LBA_LOGGING];
	unsigned int issue_LBA_count;
	u64 issue_region_map;
=======
#include "ufshci.h"

#if IS_ENABLED(CONFIG_SEC_ABC)
#include <linux/sti/abc_common.h>
#endif

/*unique number*/
#define UFS_UN_20_DIGITS 20
#define UFS_UN_MAX_DIGITS (UFS_UN_20_DIGITS+1) //current max digit + 1

#define SERIAL_NUM_SIZE 7

#define UFS_WB_ISSUED_SIZE_CNT_MAX 4

struct ufs_vendor_dev_info {
	struct ufs_hba *hba;
	char unique_number[UFS_UN_MAX_DIGITS];
	u8 lt;
	unsigned int lc;

	bool device_stuck;
>>>>>>> upstream/android-13
};

struct ufs_sec_cmd_info {
	u8 opcode;
	u32 lba;
	int transfer_len;
	u8 lun;
};

<<<<<<< HEAD
struct ufs_sec_err_info {
	struct SEC_UFS_op_count op_count;
	struct SEC_UFS_UIC_err_count UIC_err_count;
	struct SEC_UFS_Fatal_err_count Fatal_err_count;
	struct SEC_UFS_UTP_count UTP_count;
	struct SEC_UFS_UIC_cmd_count UIC_cmd_count;
	struct SEC_UFS_QUERY_count query_count;
	struct SEC_SCSI_SENSE_count sense_count;
	struct SEC_SCSI_SENSE_err_log sense_err_log;
};

void ufs_sec_get_health_info(struct ufs_hba *hba);
void ufs_set_sec_features(struct ufs_hba *hba, u8 *str_desc_buf, u8 *desc_buf);
void ufs_remove_sec_features(struct ufs_hba *hba);
void ufs_sec_hwrst_cnt_check(void);
void ufs_sec_op_err_check(struct ufs_hba *hba, enum ufs_event_type evt, void *data);
void ufs_sec_uic_cmd_error_check(struct ufs_hba *hba, u32 cmd);
void ufs_sec_tm_error_check(u8 tm_cmd);
void ufs_sec_compl_cmd_check(struct ufs_hba *hba, struct ufshcd_lrb *lrbp);
void ufs_sec_print_err_info(struct ufs_hba *hba);
void ufs_sec_send_errinfo(struct ufs_hba *hba);
=======
enum ufs_sec_wb_state {
	WB_OFF = 0,
	WB_ON_READY,
	WB_OFF_READY,
	WB_ON,
	NR_WB_STATE
};

struct ufs_sec_wb_info {
	bool support;			/* feature support and enabled */
	bool setup_done;		/* setup is done or not */
	bool wb_off;			/* WB off or not */
	atomic_t wb_off_cnt;		/* WB off count */

	enum ufs_sec_wb_state state;	/* current state */
	unsigned long state_ts;		/* current state timestamp */

	int up_threshold_block;		/* threshold for WB on : block(4KB) count */
	int up_threshold_rqs;		/* threshold for WB on : request count */
	int down_threshold_block;	/* threshold for WB off : block count */
	int down_threshold_rqs;		/* threshold for WB off : request count */

	int disable_threshold_lt;	/* LT threshold that WB is not allowed */

	int on_delay;			/* WB on delay for WB_ON_READY -> WB_ON */
	int off_delay;			/* WB off delay for WB_OFF_READY -> WB_OFF */

	/* below values will be used when (wb_off == true) */
	int lp_up_threshold_block;	/* threshold for WB on : block(4KB) count */
	int lp_up_threshold_rqs;	/* threshold for WB on : request count */
	int lp_down_threshold_block;	/* threshold for WB off : block count */
	int lp_down_threshold_rqs;	/* threshold for WB off : request count */
	int lp_on_delay;		/* on_delay multiplier when (wb_off == true) */
	int lp_off_delay;		/* off_delay multiplier when (wb_off == true) */

	int current_block;		/* current block counts in WB_ON state */
	int current_rqs;		/* current request counts in WB_ON */

	int curr_issued_min_block;		/* min. issued block count */
	int curr_issued_max_block;		/* max. issued block count */
	unsigned int curr_issued_block;		/* amount issued block count during current WB_ON session */
	/* volume count of amount issued block per WB_ON session */
	unsigned int issued_size_cnt[UFS_WB_ISSUED_SIZE_CNT_MAX];

	unsigned int total_issued_mb;	/* amount issued Write Size(MB) in all WB_ON */

	struct workqueue_struct *wb_workq;
	struct work_struct on_work;
	struct work_struct off_work;
};

enum ufs_sec_log_str_t {
	UFS_SEC_CMD_SEND,
	UFS_SEC_CMD_COMP,
	UFS_SEC_QUERY_SEND,
	UFS_SEC_QUERY_COMP,
	UFS_SEC_NOP_SEND,
	UFS_SEC_NOP_COMP,
	UFS_SEC_TM_SEND,
	UFS_SEC_TM_COMP,
	UFS_SEC_TM_ERR,
	UFS_SEC_UIC_SEND,
	UFS_SEC_UIC_COMP,
};

static const char * const ufs_sec_log_str[] = {
	"scsi_send",
	"scsi_cmpl",
	"query_send",
	"query_cmpl",
	"nop_send",
	"nop_cmpl",
	"tm_send",
	"tm_cmpl",
	"tm_err",
	"uic_send",
	"uic_cmpl",
};

struct ufs_sec_cmd_log_entry {
	const char *str;	/* ufs_sec_log_str */
	u8 lun;
	u8 cmd_id;
	u32 lba;
	int transfer_len;
	u8 idn;		/* used only for query idn */
	unsigned long outstanding_reqs;
	unsigned int tag;
	ktime_t tstamp;
};

#define UFS_SEC_CMD_LOGGING_MAX 200
#define UFS_SEC_CMD_LOGNODE_MAX 64
struct ufs_sec_cmd_log_info {
	struct ufs_sec_cmd_log_entry *entries;
	int pos;
};

struct ufs_sec_feature_info {
	struct ufs_vendor_dev_info *vdi;
	struct ufs_sec_wb_info *ufs_wb;
	struct ufs_sec_err_info *ufs_err;
	struct ufs_sec_err_info *ufs_err_backup;
#if IS_ENABLED(CONFIG_SEC_UFS_CMD_LOGGING)
	struct ufs_sec_cmd_log_info *ufs_cmd_log;
#endif

	u32 ext_ufs_feature_sup;

	u32 last_ucmd;
	bool ucmd_complete;

	enum query_opcode last_qcmd;
	enum dev_cmd_type qcmd_type;
	bool qcmd_complete;
};

/* call by vendor module */
void ufs_sec_config_features(struct ufs_hba *hba);
void ufs_sec_set_features(struct ufs_hba *hba);
void ufs_sec_remove_features(struct ufs_hba *hba);
void ufs_sec_register_vendor_hooks(void);
void ufs_sec_print_err_info(struct ufs_hba *hba);

void ufs_sec_get_health_desc(struct ufs_hba *hba);

inline bool ufs_sec_is_wb_allowed(void);
void ufs_sec_wb_force_off(struct ufs_hba *hba);

inline bool ufs_sec_is_err_cnt_allowed(void);
void ufs_sec_inc_hwrst_cnt(void);
void ufs_sec_inc_op_err(struct ufs_hba *hba, enum ufs_event_type evt, void *data);
void ufs_sec_init_logging(struct device *dev);
void ufs_sec_check_device_stuck(void);
inline bool ufs_sec_is_cmd_log_allowed(void);
>>>>>>> upstream/android-13

#endif
