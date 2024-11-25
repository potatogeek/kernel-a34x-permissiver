// SPDX-License-Identifier: GPL-2.0
/*
 * Samsung Specific feature : sysfs-nodes
 *
<<<<<<< HEAD
 * Copyright (C) 2021 Samsung Electronics Co., Ltd.
=======
 * Copyright (C) 2022 Samsung Electronics Co., Ltd.
>>>>>>> upstream/android-13
 *
 * Authors:
 *	Storage Driver <storage.sec@samsung.com>
 */

#include <linux/sysfs.h>

#include "ufs-sec-sysfs.h"

<<<<<<< HEAD
/* sec specific vendor sysfs nodes */
static struct device *sec_ufs_cmd_dev;

#ifdef CONFIG_BLK_TURBO_WRITE
#define SEC_UFS_TW_INFO_DIFF(t, n, o, member) ({		\
		(t)->member = (n)->member - (o)->member;	\
		(o)->member = (n)->member; })
#endif

=======
#define get_vdi_member(member) ufs_sec_features.vdi->member
#define get_wb_member(member) ufs_sec_features.ufs_wb->member

/* sec specific vendor sysfs nodes */
static struct device *sec_ufs_cmd_dev;

>>>>>>> upstream/android-13
/* UFS info nodes : begin */
static ssize_t ufs_sec_unique_number_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct ufs_hba *hba = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%s\n", hba->unique_number);
}
static DEVICE_ATTR(un, 0440, ufs_sec_unique_number_show, NULL);

static ssize_t ufs_sec_manufacturer_id_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	struct ufs_dev_info *dev_info = &hba->dev_info;

	return snprintf(buf, PAGE_SIZE, "%04x\n", dev_info->wmanufacturerid);
}
static DEVICE_ATTR(man_id, 0444, ufs_sec_manufacturer_id_show, NULL);

static ssize_t ufs_sec_lt_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	u8 lt;

	if (!hba) {
		dev_info(dev, "skipping ufs lt read\n");
		lt = 0;
	} else if (hba->ufshcd_state == UFSHCD_STATE_OPERATIONAL) {
		pm_runtime_get_sync(hba->dev);
		ufs_sec_get_health_info(hba);
		pm_runtime_put(hba->dev);
		lt = hba->lifetime;
	} else {
		/* return previous LT value if not operational */
		dev_info(hba->dev, "ufshcd_state : %d, old LT: %01x\n",
					hba->ufshcd_state, hba->lifetime);
		lt = hba->lifetime;
	}

	return snprintf(buf, PAGE_SIZE, "%01x\n", lt);
}
static DEVICE_ATTR(lt, 0444, ufs_sec_lt_show, NULL);

static ssize_t ufs_sec_lc_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);

	return sprintf(buf, "%u\n", hba->lc_info);
}

static ssize_t ufs_sec_lc_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
=======
	return snprintf(buf, PAGE_SIZE, "%s\n", get_vdi_member(unique_number));
}
static DEVICE_ATTR(un, 0440, ufs_sec_unique_number_show, NULL);

static ssize_t ufs_sec_lt_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba;

	hba = get_vdi_member(hba);
	if (!hba) {
		dev_err(dev, "skipping ufs lt read\n");
		get_vdi_member(lt) = 0;
	} else if (hba->ufshcd_state == UFSHCD_STATE_OPERATIONAL) {
		pm_runtime_get_sync(&hba->sdev_ufs_device->sdev_gendev);
		ufs_sec_get_health_desc(hba);
		pm_runtime_put(&hba->sdev_ufs_device->sdev_gendev);
	} else {
		/* return previous LT value if not operational */
		dev_info(hba->dev, "ufshcd_state: %d, old LT: %01x\n",
				hba->ufshcd_state, get_vdi_member(lt));
	}
	return snprintf(buf, PAGE_SIZE, "%01x\n", get_vdi_member(lt));
}
static DEVICE_ATTR(lt, 0444, ufs_sec_lt_show, NULL);

static ssize_t ufs_sec_lc_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", get_vdi_member(lc));
}

static ssize_t ufs_sec_lc_info_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
>>>>>>> upstream/android-13
	unsigned int value;

	if (kstrtou32(buf, 0, &value))
		return -EINVAL;

<<<<<<< HEAD
	hba->lc_info = value;

	return count;
}
static DEVICE_ATTR(lc, 0664, ufs_sec_lc_show, ufs_sec_lc_store);

static struct attribute *sec_ufs_info_attributes[] = {
	&dev_attr_un.attr,
	&dev_attr_man_id.attr,
	&dev_attr_lt.attr,
	&dev_attr_lc.attr,
=======
	get_vdi_member(lc) = value;

	return count;
}
static DEVICE_ATTR(lc, 0664, ufs_sec_lc_info_show, ufs_sec_lc_info_store);

static ssize_t ufs_sec_man_id_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba;

	hba = get_vdi_member(hba);
	if (!hba) {
		dev_err(dev, "skipping ufs manid read\n");
		return -EINVAL;
	}

	return snprintf(buf, PAGE_SIZE, "%04x\n", hba->dev_info.wmanufacturerid);
}
static DEVICE_ATTR(man_id, 0444, ufs_sec_man_id_show, NULL);

static struct attribute *sec_ufs_info_attributes[] = {
	&dev_attr_un.attr,
	&dev_attr_lt.attr,
	&dev_attr_lc.attr,
	&dev_attr_man_id.attr,
>>>>>>> upstream/android-13
	NULL
};

static struct attribute_group sec_ufs_info_attribute_group = {
	.attrs	= sec_ufs_info_attributes,
};
<<<<<<< HEAD

void ufs_sec_create_sysfs(struct ufs_hba *hba)
{
	int ret = 0;

	/* sec specific vendor sysfs nodes */
	if (!sec_ufs_cmd_dev)
		sec_ufs_cmd_dev = sec_device_create(hba, "ufs");

	if (IS_ERR(sec_ufs_cmd_dev))
		pr_err("Fail to create sysfs dev\n");
	else {
		ret = sysfs_create_group(&sec_ufs_cmd_dev->kobj,
				&sec_ufs_info_attribute_group);
		if (ret)
			dev_err(hba->dev, "%s: Failed to create sec_ufs_info sysfs group, %d\n",
					__func__, ret);
	}
}
/* UFS info nodes : end */

#ifdef CONFIG_BLK_TURBO_WRITE
static void SEC_UFS_TW_info_get_diff(struct SEC_UFS_TW_info *target,
		struct SEC_UFS_TW_info *new, struct SEC_UFS_TW_info *old)
{
		SEC_UFS_TW_INFO_DIFF(target, new, old, tw_enable_count);
		SEC_UFS_TW_INFO_DIFF(target, new, old, tw_disable_count);
		SEC_UFS_TW_INFO_DIFF(target, new, old, tw_amount_W_kb);
		SEC_UFS_TW_INFO_DIFF(target, new, old, hibern8_enter_count);
		SEC_UFS_TW_INFO_DIFF(target, new, old, hibern8_enter_count_100ms);
		SEC_UFS_TW_INFO_DIFF(target, new, old, hibern8_amount_ms);
		SEC_UFS_TW_INFO_DIFF(target, new, old, hibern8_amount_ms_100ms);

		target->hibern8_max_ms = new->hibern8_max_ms;

		// initialize min, max h8 time values.
		new->hibern8_max_ms = 0;
		get_monotonic_boottime(&(old->timestamp));	/* update timestamp */
}

static ssize_t SEC_UFS_TW_info_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct Scsi_Host *Shost = container_of(dev, struct Scsi_Host, shost_dev);
	struct ufs_hba *hba = shost_priv(Shost);
	struct SEC_UFS_TW_info tw_info;
	struct SEC_UFS_TW_info *tw_info_old = &(hba->SEC_tw_info_old);
	struct SEC_UFS_TW_info *tw_info_new = &(hba->SEC_tw_info);
	long hours = 0;

	get_monotonic_boottime(&(tw_info_new->timestamp));
	hours = (tw_info_new->timestamp.tv_sec - tw_info_old->timestamp.tv_sec) / 60;	/* min */
	hours = (hours + 30) / 60;	/* round up to hours */

	SEC_UFS_TW_info_get_diff(&tw_info, tw_info_new, tw_info_old);

	return sprintf(buf, "\"TWCTRLCNT\":\"%llu\","
			"\"TWCTRLERRCNT\":\"%llu\","
			"\"TWDAILYMB\":\"%llu\","
			"\"TWTOTALMB\":\"%llu\","
			"\"H8CNT\":\"%llu\",\"H8MS\":\"%llu\","
			"\"H8CNT100MS\":\"%llu\",\"H8MS100MS\":\"%llu\","
			"\"H8MAXMS\":\"%llu\","
			"\"TWhours\":\"%ld\"\n",
			(tw_info.tw_enable_count + tw_info.tw_disable_count),
			tw_info_new->tw_setflag_error_count,    /* total error count */
			(tw_info.tw_amount_W_kb >> 10),         /* TW write daily : MB */
			(tw_info_new->tw_amount_W_kb >> 10),    /* TW write total : MB */
			tw_info.hibern8_enter_count, tw_info.hibern8_amount_ms,
			tw_info.hibern8_enter_count_100ms,
			tw_info.hibern8_amount_ms_100ms,
			tw_info.hibern8_max_ms,
			hours);
}
static DEVICE_ATTR(SEC_UFS_TW_info, 0444, SEC_UFS_TW_info_show, NULL);
#endif

/* UFS error info : begin */
static ssize_t SEC_UFS_op_cnt_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	if ((buf[0] != 'C' && buf[0] != 'c') || (count != 1))
		return -EINVAL;

	SEC_UFS_ERR_INFO_BACKUP(op_count, HW_RESET_count);
	SEC_UFS_ERR_INFO_BACKUP(op_count, link_startup_count);
	SEC_UFS_ERR_INFO_BACKUP(op_count, Hibern8_enter_count);
	SEC_UFS_ERR_INFO_BACKUP(op_count, Hibern8_exit_count);
=======
/* UFS info nodes : end */

/* UFS SEC WB : begin */
static ssize_t ufs_sec_wb_support_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);

	return sprintf(buf, "%s:%s\n",
			get_wb_member(support) ? "Support" : "No support",
			hba->dev_info.wb_enabled ? "on" : "off");
}
static DEVICE_ATTR(sec_wb_support, 0444, ufs_sec_wb_support_show, NULL);

static ssize_t ufs_sec_wb_enable_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	unsigned long flags;
	u32 value;

	if (!get_wb_member(setup_done)) {
		dev_err(hba->dev, "SEC WB is not ready yet.\n");
		return -ENODEV;
	}

	if (!ufs_sec_is_wb_allowed()) {
		pr_err("%s: not allowed.\n", __func__);
		return -EPERM;
	}

	if (kstrtou32(buf, 0, &value))
		return -EINVAL;

	spin_lock_irqsave(hba->host->host_lock, flags);
	value = !!value;

	if (!value) {
		if (atomic_inc_return(&get_wb_member(wb_off_cnt)) == 1) {
			get_wb_member(wb_off) = true;
			pr_err("disable SEC WB : state %d.\n", get_wb_member(state));
		}
	} else {
		if (atomic_dec_and_test(&get_wb_member(wb_off_cnt))) {
			get_wb_member(wb_off) = false;
			pr_err("enable SEC WB.\n");
		}
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
>>>>>>> upstream/android-13

	return count;
}

<<<<<<< HEAD
static ssize_t SEC_UFS_uic_cmd_cnt_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
=======
static ssize_t ufs_sec_wb_enable_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", get_wb_member(wb_off) ? "off" : "Enabled");
}
static DEVICE_ATTR(sec_wb_enable, 0664, ufs_sec_wb_enable_show, ufs_sec_wb_enable_store);

SEC_UFS_WB_DATA_ATTR(wb_up_threshold_block, "%d\n", up_threshold_block);
SEC_UFS_WB_DATA_ATTR(wb_up_threshold_rqs, "%d\n", up_threshold_rqs);
SEC_UFS_WB_DATA_ATTR(wb_down_threshold_block, "%d\n", down_threshold_block);
SEC_UFS_WB_DATA_ATTR(wb_down_threshold_rqs, "%d\n", down_threshold_rqs);
SEC_UFS_WB_DATA_ATTR(lp_wb_up_threshold_block, "%d\n", lp_up_threshold_block);
SEC_UFS_WB_DATA_ATTR(lp_wb_up_threshold_rqs, "%d\n", lp_up_threshold_rqs);
SEC_UFS_WB_DATA_ATTR(lp_wb_down_threshold_block, "%d\n", lp_down_threshold_block);
SEC_UFS_WB_DATA_ATTR(lp_wb_down_threshold_rqs, "%d\n", lp_down_threshold_rqs);

SEC_UFS_WB_TIME_ATTR(wb_on_delay_ms, "%d\n", on_delay);
SEC_UFS_WB_TIME_ATTR(wb_off_delay_ms, "%d\n", off_delay);
SEC_UFS_WB_TIME_ATTR(lp_wb_on_delay_ms, "%d\n", lp_on_delay);
SEC_UFS_WB_TIME_ATTR(lp_wb_off_delay_ms, "%d\n", lp_off_delay);

SEC_UFS_WB_DATA_RO_ATTR(wb_state, "%d,%u\n",
		get_wb_member(state), jiffies_to_msecs(jiffies - get_wb_member(state_ts)));
SEC_UFS_WB_DATA_RO_ATTR(wb_current_stat, "current : block %d, rqs %d, issued blocks %d\n",
		get_wb_member(current_block), get_wb_member(current_rqs),
		get_wb_member(curr_issued_block));
SEC_UFS_WB_DATA_RO_ATTR(wb_current_min_max_stat, "current issued blocks : min %d, max %d.\n",
		(get_wb_member(curr_issued_min_block) == INT_MAX) ?
		0 : get_wb_member(curr_issued_min_block),
		get_wb_member(curr_issued_max_block));
SEC_UFS_WB_DATA_RO_ATTR(wb_total_stat, "total : %dMB\n\t<  4GB:%d\n\t<  8GB:%d\n\t< 16GB:%d\n\t>=16GB:%d\n",
		get_wb_member(total_issued_mb),
		get_wb_member(issued_size_cnt[0]),
		get_wb_member(issued_size_cnt[1]),
		get_wb_member(issued_size_cnt[2]),
		get_wb_member(issued_size_cnt[3]));

static struct attribute *sec_ufs_wb_attributes[] = {
	&dev_attr_sec_wb_support.attr,
	&dev_attr_sec_wb_enable.attr,
	&dev_attr_wb_up_threshold_block.attr,
	&dev_attr_wb_up_threshold_rqs.attr,
	&dev_attr_wb_down_threshold_block.attr,
	&dev_attr_wb_down_threshold_rqs.attr,
	&dev_attr_lp_wb_up_threshold_block.attr,
	&dev_attr_lp_wb_up_threshold_rqs.attr,
	&dev_attr_lp_wb_down_threshold_block.attr,
	&dev_attr_lp_wb_down_threshold_rqs.attr,
	&dev_attr_wb_on_delay_ms.attr,
	&dev_attr_wb_off_delay_ms.attr,
	&dev_attr_lp_wb_on_delay_ms.attr,
	&dev_attr_lp_wb_off_delay_ms.attr,
	&dev_attr_wb_state.attr,
	&dev_attr_wb_current_stat.attr,
	&dev_attr_wb_current_min_max_stat.attr,
	&dev_attr_wb_total_stat.attr,
	NULL
};

static struct attribute_group sec_ufs_wb_attribute_group = {
	.attrs	= sec_ufs_wb_attributes,
};
/* UFS SEC WB : end */

/* SEC error info : begin */
static ssize_t SEC_UFS_op_cnt_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	if ((buf[0] != 'C' && buf[0] != 'c') || (count != 1))
		return -EINVAL;

<<<<<<< HEAD
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_TEST_MODE_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_GET_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_SET_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_PEER_GET_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_PEER_SET_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_POWERON_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_POWEROFF_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_ENABLE_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_RESET_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_END_PT_RST_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_LINK_STARTUP_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_HIBER_ENTER_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, DME_HIBER_EXIT_err);
=======
	SEC_UFS_ERR_INFO_BACKUP(op_cnt, HW_RESET_cnt);
	SEC_UFS_ERR_INFO_BACKUP(op_cnt, link_startup_cnt);
	SEC_UFS_ERR_INFO_BACKUP(op_cnt, Hibern8_enter_cnt);
	SEC_UFS_ERR_INFO_BACKUP(op_cnt, Hibern8_exit_cnt);
>>>>>>> upstream/android-13

	return count;
}

<<<<<<< HEAD
static ssize_t SEC_UFS_uic_err_cnt_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
=======
static ssize_t SEC_UFS_uic_cmd_cnt_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	if ((buf[0] != 'C' && buf[0] != 'c') || (count != 1))
		return -EINVAL;

<<<<<<< HEAD
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_count, PA_ERR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_count, DL_PA_INIT_ERROR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_count, DL_NAC_RECEIVED_ERROR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_count, DL_TC_REPLAY_ERROR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_count, NL_ERROR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_count, TL_ERROR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_count, DME_ERROR_cnt);
=======
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_TEST_MODE_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_GET_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_SET_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_PEER_GET_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_PEER_SET_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_POWERON_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_POWEROFF_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_ENABLE_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_RESET_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_END_PT_RST_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_LINK_STARTUP_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_HIBER_ENTER_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, DME_HIBER_EXIT_err);
>>>>>>> upstream/android-13

	return count;
}

<<<<<<< HEAD
static ssize_t SEC_UFS_fatal_cnt_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
=======
static ssize_t SEC_UFS_uic_err_cnt_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	if ((buf[0] != 'C' && buf[0] != 'c') || (count != 1))
		return -EINVAL;

<<<<<<< HEAD
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_count, DFE);
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_count, CFE);
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_count, SBFE);
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_count, CEFE);
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_count, LLE);
=======
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_cnt, PAERR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_cnt, DL_PA_INIT_ERR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_cnt, DL_NAC_RCVD_ERR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_cnt, DL_TC_REPLAY_ERR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_cnt, NLERR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_cnt, TLERR_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_cnt, DMEERR_cnt);
>>>>>>> upstream/android-13

	return count;
}

<<<<<<< HEAD
static ssize_t SEC_UFS_utp_cnt_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
=======
static ssize_t SEC_UFS_fatal_cnt_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	if ((buf[0] != 'C' && buf[0] != 'c') || (count != 1))
		return -EINVAL;

<<<<<<< HEAD
	SEC_UFS_ERR_INFO_BACKUP(UTP_count, UTMR_query_task_count);
	SEC_UFS_ERR_INFO_BACKUP(UTP_count, UTMR_abort_task_count);
	SEC_UFS_ERR_INFO_BACKUP(UTP_count, UTR_read_err);
	SEC_UFS_ERR_INFO_BACKUP(UTP_count, UTR_write_err);
	SEC_UFS_ERR_INFO_BACKUP(UTP_count, UTR_sync_cache_err);
	SEC_UFS_ERR_INFO_BACKUP(UTP_count, UTR_unmap_err);
	SEC_UFS_ERR_INFO_BACKUP(UTP_count, UTR_etc_err);
=======
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_cnt, DFE);
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_cnt, CFE);
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_cnt, SBFE);
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_cnt, CEFE);
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_cnt, LLE);
>>>>>>> upstream/android-13

	return count;
}

<<<<<<< HEAD
static ssize_t SEC_UFS_query_cnt_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
=======
static ssize_t SEC_UFS_utp_cnt_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	if ((buf[0] != 'C' && buf[0] != 'c') || (count != 1))
		return -EINVAL;

<<<<<<< HEAD
	SEC_UFS_ERR_INFO_BACKUP(query_count, NOP_err);
	SEC_UFS_ERR_INFO_BACKUP(query_count, R_Desc_err);
	SEC_UFS_ERR_INFO_BACKUP(query_count, W_Desc_err);
	SEC_UFS_ERR_INFO_BACKUP(query_count, R_Attr_err);
	SEC_UFS_ERR_INFO_BACKUP(query_count, W_Attr_err);
	SEC_UFS_ERR_INFO_BACKUP(query_count, R_Flag_err);
	SEC_UFS_ERR_INFO_BACKUP(query_count, Set_Flag_err);
	SEC_UFS_ERR_INFO_BACKUP(query_count, Clear_Flag_err);
	SEC_UFS_ERR_INFO_BACKUP(query_count, Toggle_Flag_err);
=======
	SEC_UFS_ERR_INFO_BACKUP(UTP_cnt, UTMR_query_task_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UTP_cnt, UTMR_abort_task_cnt);
	SEC_UFS_ERR_INFO_BACKUP(UTP_cnt, UTR_read_err);
	SEC_UFS_ERR_INFO_BACKUP(UTP_cnt, UTR_write_err);
	SEC_UFS_ERR_INFO_BACKUP(UTP_cnt, UTR_sync_cache_err);
	SEC_UFS_ERR_INFO_BACKUP(UTP_cnt, UTR_unmap_err);
	SEC_UFS_ERR_INFO_BACKUP(UTP_cnt, UTR_etc_err);
>>>>>>> upstream/android-13

	return count;
}

<<<<<<< HEAD
static ssize_t SEC_UFS_err_sum_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
=======
static ssize_t SEC_UFS_query_cnt_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	if ((buf[0] != 'C' && buf[0] != 'c') || (count != 1))
		return -EINVAL;

<<<<<<< HEAD
	SEC_UFS_ERR_INFO_BACKUP(op_count, op_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_count, UIC_cmd_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_count, UIC_err);
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_count, Fatal_err);
	SEC_UFS_ERR_INFO_BACKUP(UTP_count, UTP_err);
	SEC_UFS_ERR_INFO_BACKUP(query_count, Query_err);
=======
	SEC_UFS_ERR_INFO_BACKUP(Query_cnt, NOP_err);
	SEC_UFS_ERR_INFO_BACKUP(Query_cnt, R_Desc_err);
	SEC_UFS_ERR_INFO_BACKUP(Query_cnt, W_Desc_err);
	SEC_UFS_ERR_INFO_BACKUP(Query_cnt, R_Attr_err);
	SEC_UFS_ERR_INFO_BACKUP(Query_cnt, W_Attr_err);
	SEC_UFS_ERR_INFO_BACKUP(Query_cnt, R_Flag_err);
	SEC_UFS_ERR_INFO_BACKUP(Query_cnt, Set_Flag_err);
	SEC_UFS_ERR_INFO_BACKUP(Query_cnt, Clear_Flag_err);
	SEC_UFS_ERR_INFO_BACKUP(Query_cnt, Toggle_Flag_err);
>>>>>>> upstream/android-13

	return count;
}

<<<<<<< HEAD
static ssize_t sense_err_count_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
=======
static ssize_t SEC_UFS_err_sum_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> upstream/android-13
{
	if ((buf[0] != 'C' && buf[0] != 'c') || (count != 1))
		return -EINVAL;

<<<<<<< HEAD
	SEC_UFS_ERR_INFO_BACKUP(sense_count, scsi_medium_err);
	SEC_UFS_ERR_INFO_BACKUP(sense_count, scsi_hw_err);
=======
	SEC_UFS_ERR_INFO_BACKUP(op_cnt, op_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_cmd_cnt, UIC_cmd_err);
	SEC_UFS_ERR_INFO_BACKUP(UIC_err_cnt, UIC_err);
	SEC_UFS_ERR_INFO_BACKUP(Fatal_err_cnt, Fatal_err);
	SEC_UFS_ERR_INFO_BACKUP(UTP_cnt, UTP_err);
	SEC_UFS_ERR_INFO_BACKUP(Query_cnt, Query_err);
>>>>>>> upstream/android-13

	return count;
}

<<<<<<< HEAD
SEC_UFS_DATA_ATTR_RW(SEC_UFS_op_cnt, "\"HWRESET\":\"%u\",\"LINKFAIL\":\"%u\",\"H8ENTERFAIL\":\"%u\""
		",\"H8EXITFAIL\":\"%u\"\n",
		ufs_err_info.op_count.HW_RESET_count,
		ufs_err_info.op_count.link_startup_count,
		ufs_err_info.op_count.Hibern8_enter_count,
		ufs_err_info.op_count.Hibern8_exit_count);

SEC_UFS_DATA_ATTR_RW(SEC_UFS_uic_cmd_cnt, "\"TESTMODE\":\"%u\",\"DME_GET\":\"%u\",\"DME_SET\":\"%u\""
		",\"DME_PGET\":\"%u\",\"DME_PSET\":\"%u\",\"PWRON\":\"%u\",\"PWROFF\":\"%u\""
		",\"DME_EN\":\"%u\",\"DME_RST\":\"%u\",\"EPRST\":\"%u\",\"LINKSTARTUP\":\"%u\""
		",\"H8ENTER\":\"%u\",\"H8EXIT\":\"%u\"\n",
		ufs_err_info.UIC_cmd_count.DME_TEST_MODE_err,
		ufs_err_info.UIC_cmd_count.DME_GET_err,
		ufs_err_info.UIC_cmd_count.DME_SET_err,
		ufs_err_info.UIC_cmd_count.DME_PEER_GET_err,
		ufs_err_info.UIC_cmd_count.DME_PEER_SET_err,
		ufs_err_info.UIC_cmd_count.DME_POWERON_err,
		ufs_err_info.UIC_cmd_count.DME_POWEROFF_err,
		ufs_err_info.UIC_cmd_count.DME_ENABLE_err,
		ufs_err_info.UIC_cmd_count.DME_RESET_err,
		ufs_err_info.UIC_cmd_count.DME_END_PT_RST_err,
		ufs_err_info.UIC_cmd_count.DME_LINK_STARTUP_err,
		ufs_err_info.UIC_cmd_count.DME_HIBER_ENTER_err,
		ufs_err_info.UIC_cmd_count.DME_HIBER_EXIT_err);

SEC_UFS_DATA_ATTR_RW(SEC_UFS_uic_err_cnt, "\"PAERR\":\"%u\",\"DLPAINITERROR\":\"%u\",\"DLNAC\":\"%u\""
		",\"DLTCREPLAY\":\"%u\",\"NLERR\":\"%u\",\"TLERR\":\"%u\",\"DMEERR\":\"%u\"\n",
		ufs_err_info.UIC_err_count.PA_ERR_cnt,
		ufs_err_info.UIC_err_count.DL_PA_INIT_ERROR_cnt,
		ufs_err_info.UIC_err_count.DL_NAC_RECEIVED_ERROR_cnt,
		ufs_err_info.UIC_err_count.DL_TC_REPLAY_ERROR_cnt,
		ufs_err_info.UIC_err_count.NL_ERROR_cnt,
		ufs_err_info.UIC_err_count.TL_ERROR_cnt,
		ufs_err_info.UIC_err_count.DME_ERROR_cnt);

SEC_UFS_DATA_ATTR_RW(SEC_UFS_fatal_cnt, "\"DFE\":\"%u\",\"CFE\":\"%u\",\"SBFE\":\"%u\""
		",\"CEFE\":\"%u\",\"LLE\":\"%u\"\n",
		ufs_err_info.Fatal_err_count.DFE,		// Device_Fatal
		ufs_err_info.Fatal_err_count.CFE,		// Controller_Fatal
		ufs_err_info.Fatal_err_count.SBFE,		// System_Bus_Fatal
		ufs_err_info.Fatal_err_count.CEFE,		// Crypto_Engine_Fatal
		ufs_err_info.Fatal_err_count.LLE);		// Link_Lost

SEC_UFS_DATA_ATTR_RW(SEC_UFS_utp_cnt, "\"UTMRQTASK\":\"%u\",\"UTMRATASK\":\"%u\",\"UTRR\":\"%u\""
		",\"UTRW\":\"%u\",\"UTRSYNCCACHE\":\"%u\",\"UTRUNMAP\":\"%u\",\"UTRETC\":\"%u\"\n",
		ufs_err_info.UTP_count.UTMR_query_task_count,
		ufs_err_info.UTP_count.UTMR_abort_task_count,
		ufs_err_info.UTP_count.UTR_read_err,
		ufs_err_info.UTP_count.UTR_write_err,
		ufs_err_info.UTP_count.UTR_sync_cache_err,
		ufs_err_info.UTP_count.UTR_unmap_err,
		ufs_err_info.UTP_count.UTR_etc_err);

SEC_UFS_DATA_ATTR_RW(SEC_UFS_query_cnt, "\"NOPERR\":\"%u\",\"R_DESC\":\"%u\",\"W_DESC\":\"%u\""
		",\"R_ATTR\":\"%u\",\"W_ATTR\":\"%u\",\"R_FLAG\":\"%u\",\"S_FLAG\":\"%u\""
		",\"C_FLAG\":\"%u\",\"T_FLAG\":\"%u\"\n",
		ufs_err_info.query_count.NOP_err,
		ufs_err_info.query_count.R_Desc_err,
		ufs_err_info.query_count.W_Desc_err,
		ufs_err_info.query_count.R_Attr_err,
		ufs_err_info.query_count.W_Attr_err,
		ufs_err_info.query_count.R_Flag_err,
		ufs_err_info.query_count.Set_Flag_err,
		ufs_err_info.query_count.Clear_Flag_err,
		ufs_err_info.query_count.Toggle_Flag_err);

SEC_UFS_DATA_ATTR_RW(sense_err_count, "\"MEDIUM\":\"%u\",\"HWERR\":\"%u\"\n",
		ufs_err_info.sense_count.scsi_medium_err,
		ufs_err_info.sense_count.scsi_hw_err);

SEC_UFS_DATA_ATTR_RO(sense_err_logging, "\"LBA0\":\"%lx\",\"LBA1\":\"%lx\",\"LBA2\":\"%lx\""
		",\"LBA3\":\"%lx\",\"LBA4\":\"%lx\",\"LBA5\":\"%lx\""
		",\"LBA6\":\"%lx\",\"LBA7\":\"%lx\",\"LBA8\":\"%lx\",\"LBA9\":\"%lx\""
		",\"REGIONMAP\":\"%016llx\"\n",
		ufs_err_info.sense_err_log.issue_LBA_list[0],
		ufs_err_info.sense_err_log.issue_LBA_list[1],
		ufs_err_info.sense_err_log.issue_LBA_list[2],
		ufs_err_info.sense_err_log.issue_LBA_list[3],
		ufs_err_info.sense_err_log.issue_LBA_list[4],
		ufs_err_info.sense_err_log.issue_LBA_list[5],
		ufs_err_info.sense_err_log.issue_LBA_list[6],
		ufs_err_info.sense_err_log.issue_LBA_list[7],
		ufs_err_info.sense_err_log.issue_LBA_list[8],
		ufs_err_info.sense_err_log.issue_LBA_list[9],
		ufs_err_info.sense_err_log.issue_region_map);

/* daily err sum */
SEC_UFS_DATA_ATTR_RW(SEC_UFS_err_sum, "\"OPERR\":\"%u\",\"UICCMD\":\"%u\",\"UICERR\":\"%u\""
		",\"FATALERR\":\"%u\",\"UTPERR\":\"%u\",\"QUERYERR\":\"%u\"\n",
		ufs_err_info.op_count.op_err,
		ufs_err_info.UIC_cmd_count.UIC_cmd_err,
		ufs_err_info.UIC_err_count.UIC_err,
		ufs_err_info.Fatal_err_count.Fatal_err,
		ufs_err_info.UTP_count.UTP_err,
		ufs_err_info.query_count.Query_err);

/* accumulated err sum */
SEC_UFS_DATA_ATTR_RO(SEC_UFS_err_summary,
		"OPERR : %u, UICCMD : %u, UICERR : %u, FATALERR : %u, UTPERR : %u, QUERYERR : %u\n"
		"MEDIUM : %u, HWERR : %u\n",
		SEC_UFS_ERR_INFO_GET_VALUE(op_count, op_err),
		SEC_UFS_ERR_INFO_GET_VALUE(UIC_cmd_count, UIC_cmd_err),
		SEC_UFS_ERR_INFO_GET_VALUE(UIC_err_count, UIC_err),
		SEC_UFS_ERR_INFO_GET_VALUE(Fatal_err_count, Fatal_err),
		SEC_UFS_ERR_INFO_GET_VALUE(UTP_count, UTP_err),
		SEC_UFS_ERR_INFO_GET_VALUE(query_count, Query_err),
		SEC_UFS_ERR_INFO_GET_VALUE(sense_count, scsi_medium_err),
		SEC_UFS_ERR_INFO_GET_VALUE(sense_count, scsi_hw_err));
/* UFS error info : end */

static struct attribute *ufs_attributes[] = {
=======
static ssize_t sense_err_count_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	if ((buf[0] != 'C' && buf[0] != 'c') || (count != 1))
		return -EINVAL;

	SEC_UFS_ERR_INFO_BACKUP(sense_cnt, scsi_medium_err);
	SEC_UFS_ERR_INFO_BACKUP(sense_cnt, scsi_hw_err);

	return count;
}

SEC_UFS_DATA_ATTR_RW(SEC_UFS_op_cnt, "\"HWRESET\":\"%u\",\"LINKFAIL\":\"%u\""
		",\"H8ENTERFAIL\":\"%u\",\"H8EXITFAIL\":\"%u\"\n",
		get_err_member(op_cnt).HW_RESET_cnt,
		get_err_member(op_cnt).link_startup_cnt,
		get_err_member(op_cnt).Hibern8_enter_cnt,
		get_err_member(op_cnt).Hibern8_exit_cnt);

SEC_UFS_DATA_ATTR_RW(SEC_UFS_uic_cmd_cnt, "\"TESTMODE\":\"%u\""
		",\"DME_GET\":\"%u\",\"DME_SET\":\"%u\",\"DME_PGET\":\"%u\""
		",\"DME_PSET\":\"%u\",\"PWRON\":\"%u\",\"PWROFF\":\"%u\""
		",\"DME_EN\":\"%u\",\"DME_RST\":\"%u\",\"EPRST\":\"%u\""
		",\"LINKSTARTUP\":\"%u\",\"H8ENTER\":\"%u\""
		",\"H8EXIT\":\"%u\"\n",
		get_err_member(UIC_cmd_cnt).DME_TEST_MODE_err,
		get_err_member(UIC_cmd_cnt).DME_GET_err,
		get_err_member(UIC_cmd_cnt).DME_SET_err,
		get_err_member(UIC_cmd_cnt).DME_PEER_GET_err,
		get_err_member(UIC_cmd_cnt).DME_PEER_SET_err,
		get_err_member(UIC_cmd_cnt).DME_POWERON_err,
		get_err_member(UIC_cmd_cnt).DME_POWEROFF_err,
		get_err_member(UIC_cmd_cnt).DME_ENABLE_err,
		get_err_member(UIC_cmd_cnt).DME_RESET_err,
		get_err_member(UIC_cmd_cnt).DME_END_PT_RST_err,
		get_err_member(UIC_cmd_cnt).DME_LINK_STARTUP_err,
		get_err_member(UIC_cmd_cnt).DME_HIBER_ENTER_err,
		get_err_member(UIC_cmd_cnt).DME_HIBER_EXIT_err);

SEC_UFS_DATA_ATTR_RW(SEC_UFS_uic_err_cnt, "\"PAERR\":\"%u\""
		",\"DLPAINITERROR\":\"%u\",\"DLNAC\":\"%u\""
		",\"DLTCREPLAY\":\"%u\",\"NLERR\":\"%u\",\"TLERR\":\"%u\""
		",\"DMEERR\":\"%u\"\n",
		get_err_member(UIC_err_cnt).PAERR_cnt,
		get_err_member(UIC_err_cnt).DL_PA_INIT_ERR_cnt,
		get_err_member(UIC_err_cnt).DL_NAC_RCVD_ERR_cnt,
		get_err_member(UIC_err_cnt).DL_TC_REPLAY_ERR_cnt,
		get_err_member(UIC_err_cnt).NLERR_cnt,
		get_err_member(UIC_err_cnt).TLERR_cnt,
		get_err_member(UIC_err_cnt).DMEERR_cnt);

SEC_UFS_DATA_ATTR_RW(SEC_UFS_fatal_cnt, "\"DFE\":\"%u\",\"CFE\":\"%u\""
		",\"SBFE\":\"%u\",\"CEFE\":\"%u\",\"LLE\":\"%u\"\n",
		get_err_member(Fatal_err_cnt).DFE,
		get_err_member(Fatal_err_cnt).CFE,
		get_err_member(Fatal_err_cnt).SBFE,
		get_err_member(Fatal_err_cnt).CEFE,
		get_err_member(Fatal_err_cnt).LLE);

SEC_UFS_DATA_ATTR_RW(SEC_UFS_utp_cnt, "\"UTMRQTASK\":\"%u\""
		",\"UTMRATASK\":\"%u\",\"UTRR\":\"%u\",\"UTRW\":\"%u\""
		",\"UTRSYNCCACHE\":\"%u\",\"UTRUNMAP\":\"%u\""
		",\"UTRETC\":\"%u\"\n",
		get_err_member(UTP_cnt).UTMR_query_task_cnt,
		get_err_member(UTP_cnt).UTMR_abort_task_cnt,
		get_err_member(UTP_cnt).UTR_read_err,
		get_err_member(UTP_cnt).UTR_write_err,
		get_err_member(UTP_cnt).UTR_sync_cache_err,
		get_err_member(UTP_cnt).UTR_unmap_err,
		get_err_member(UTP_cnt).UTR_etc_err);

SEC_UFS_DATA_ATTR_RW(SEC_UFS_query_cnt, "\"NOPERR\":\"%u\",\"R_DESC\":\"%u\""
		",\"W_DESC\":\"%u\",\"R_ATTR\":\"%u\",\"W_ATTR\":\"%u\""
		",\"R_FLAG\":\"%u\",\"S_FLAG\":\"%u\",\"C_FLAG\":\"%u\""
		",\"T_FLAG\":\"%u\"\n",
		get_err_member(Query_cnt).NOP_err,
		get_err_member(Query_cnt).R_Desc_err,
		get_err_member(Query_cnt).W_Desc_err,
		get_err_member(Query_cnt).R_Attr_err,
		get_err_member(Query_cnt).W_Attr_err,
		get_err_member(Query_cnt).R_Flag_err,
		get_err_member(Query_cnt).Set_Flag_err,
		get_err_member(Query_cnt).Clear_Flag_err,
		get_err_member(Query_cnt).Toggle_Flag_err);

/* daily err sum */
SEC_UFS_DATA_ATTR_RW(SEC_UFS_err_sum, "\"OPERR\":\"%u\",\"UICCMD\":\"%u\""
		",\"UICERR\":\"%u\",\"FATALERR\":\"%u\",\"UTPERR\":\"%u\""
		",\"QUERYERR\":\"%u\"\n",
		get_err_member(op_cnt).op_err,
		get_err_member(UIC_cmd_cnt).UIC_cmd_err,
		get_err_member(UIC_err_cnt).UIC_err,
		get_err_member(Fatal_err_cnt).Fatal_err,
		get_err_member(UTP_cnt).UTP_err,
		get_err_member(Query_cnt).Query_err);

SEC_UFS_DATA_ATTR_RW(sense_err_count, "\"MEDIUM\":\"%u\",\"HWERR\":\"%u\"\n",
		get_err_member(sense_cnt).scsi_medium_err,
		get_err_member(sense_cnt).scsi_hw_err);

SEC_UFS_DATA_ATTR_RO(sense_err_logging, "\"LBA0\":\"%lx\",\"LBA1\":\"%lx\""
		",\"LBA2\":\"%lx\",\"LBA3\":\"%lx\",\"LBA4\":\"%lx\""
		",\"LBA5\":\"%lx\",\"LBA6\":\"%lx\",\"LBA7\":\"%lx\""
		",\"LBA8\":\"%lx\",\"LBA9\":\"%lx\""
		",\"REGIONMAP\":\"%016llx\"\n",
		get_err_member(sense_err_log).issue_LBA_list[0],
		get_err_member(sense_err_log).issue_LBA_list[1],
		get_err_member(sense_err_log).issue_LBA_list[2],
		get_err_member(sense_err_log).issue_LBA_list[3],
		get_err_member(sense_err_log).issue_LBA_list[4],
		get_err_member(sense_err_log).issue_LBA_list[5],
		get_err_member(sense_err_log).issue_LBA_list[6],
		get_err_member(sense_err_log).issue_LBA_list[7],
		get_err_member(sense_err_log).issue_LBA_list[8],
		get_err_member(sense_err_log).issue_LBA_list[9],
		get_err_member(sense_err_log).issue_region_map);

/* accumulated err sum */
SEC_UFS_DATA_ATTR_RO(SEC_UFS_err_summary,
		"OPERR : %u, UICCMD : %u, UICERR : %u, FATALERR : %u"
		", UTPERR : %u, QUERYERR : %u\n"
		"MEDIUM : %u, HWERR : %u\n",
		SEC_UFS_ERR_INFO_GET_VALUE(op_cnt, op_err),
		SEC_UFS_ERR_INFO_GET_VALUE(UIC_cmd_cnt, UIC_cmd_err),
		SEC_UFS_ERR_INFO_GET_VALUE(UIC_err_cnt, UIC_err),
		SEC_UFS_ERR_INFO_GET_VALUE(Fatal_err_cnt, Fatal_err),
		SEC_UFS_ERR_INFO_GET_VALUE(UTP_cnt, UTP_err),
		SEC_UFS_ERR_INFO_GET_VALUE(Query_cnt, Query_err),
		SEC_UFS_ERR_INFO_GET_VALUE(sense_cnt, scsi_medium_err),
		SEC_UFS_ERR_INFO_GET_VALUE(sense_cnt, scsi_hw_err));

static struct attribute *sec_ufs_error_attributes[] = {
>>>>>>> upstream/android-13
	&dev_attr_SEC_UFS_op_cnt.attr,
	&dev_attr_SEC_UFS_uic_cmd_cnt.attr,
	&dev_attr_SEC_UFS_uic_err_cnt.attr,
	&dev_attr_SEC_UFS_fatal_cnt.attr,
	&dev_attr_SEC_UFS_utp_cnt.attr,
	&dev_attr_SEC_UFS_query_cnt.attr,
	&dev_attr_SEC_UFS_err_sum.attr,
	&dev_attr_sense_err_count.attr,
	&dev_attr_sense_err_logging.attr,
	&dev_attr_SEC_UFS_err_summary.attr,
<<<<<<< HEAD
#ifdef CONFIG_BLK_TURBO_WRITE
	&dev_attr_SEC_UFS_TW_info.attr,
#endif
	NULL
};

static struct attribute_group ufs_attribute_group = {
	.attrs	= ufs_attributes,
};

void ufs_sec_create_err_sysfs(struct ufs_hba *hba)
{
	int ret = 0;
	struct device *dev = &(hba->host->shost_dev);

	/* scsi_host sysfs nodes */
	ret = sysfs_create_group(&dev->kobj, &ufs_attribute_group);
	if (ret)
		pr_err("cannot create sysfs group err: %d\n", ret);
=======
	NULL
};

static struct attribute_group sec_ufs_error_attribute_group = {
	.attrs	= sec_ufs_error_attributes,
};
/* SEC error info : end */

#if IS_ENABLED(CONFIG_SEC_UFS_CMD_LOGGING)
/* SEC cmd log : begin */
static ssize_t ufs_sec_cmd_log_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_sec_cmd_log_info *ufs_cmd_log =
		ufs_sec_features.ufs_cmd_log;
	struct ufs_sec_cmd_log_entry *entry = NULL;
	int i = (ufs_cmd_log->pos + UFS_SEC_CMD_LOGGING_MAX
			- UFS_SEC_CMD_LOGNODE_MAX);
	int idx = 0;
	int len = 0;

	len += snprintf(buf + len, PAGE_SIZE - len,
			"%02s: %10s: %2s %3s %4s %9s %6s %16s\n",
			"No", "log string", "lu", "tag",
			"c_id", "lba", "length", "time");

	for (idx = 0; idx < UFS_SEC_CMD_LOGNODE_MAX; idx++, i++) {
		i %= UFS_SEC_CMD_LOGGING_MAX;
		entry = &ufs_cmd_log->entries[i];
		len += snprintf(buf + len, PAGE_SIZE - len,
				"%2d: %10s: %2d %3d 0x%02x %9u %6d %16llu\n",
				idx,
				entry->str, entry->lun, entry->tag,
				entry->cmd_id, entry->lba,
				entry->transfer_len, entry->tstamp);
	}

	return len;
}
static DEVICE_ATTR(cmd_log, 0440, ufs_sec_cmd_log_show, NULL);

static struct attribute *sec_ufs_cmd_log_attributes[] = {
	&dev_attr_cmd_log.attr,
	NULL
};

static struct attribute_group sec_ufs_cmd_log_attribute_group = {
	.attrs	= sec_ufs_cmd_log_attributes,
};
/* SEC cmd log : end */
#endif

static int ufs_sec_create_sysfs_dev(struct ufs_hba *hba)
{
	if (!sec_ufs_cmd_dev)
		sec_ufs_cmd_dev = sec_device_create(hba, "ufs");

	if (IS_ERR(sec_ufs_cmd_dev)) {
		pr_err("Fail to create sysfs dev\n");
		return -ENODEV;
	}

	return 0;
}

static void ufs_sec_create_sysfs_group(struct ufs_hba *hba, struct device **dev,
		const struct attribute_group *dev_attr_group, const char *group_name)
{
	int ret = 0;

	ret = sysfs_create_group(&(*dev)->kobj,	dev_attr_group);
	if (ret)
		dev_err(hba->dev, "%s: Failed to create %s sysfs group (err = %d)\n",
				__func__, group_name, ret);
>>>>>>> upstream/android-13
}

void ufs_sec_add_sysfs_nodes(struct ufs_hba *hba)
{
<<<<<<< HEAD
	/* sec specific vendor sysfs nodes */
	ufs_sec_create_sysfs(hba);
	ufs_sec_create_err_sysfs(hba);
=======
	struct device *shost_dev = &(hba->host->shost_dev);

	if (ufs_sec_is_err_cnt_allowed())
		ufs_sec_create_sysfs_group(hba, &shost_dev,
			&sec_ufs_error_attribute_group, "sec_ufs_err");

	/* sec specific vendor sysfs nodes */
	if (!ufs_sec_create_sysfs_dev(hba)) {
		ufs_sec_create_sysfs_group(hba, &sec_ufs_cmd_dev,
			&sec_ufs_info_attribute_group, "sec_ufs_info");

		if (get_wb_member(setup_done))
			ufs_sec_create_sysfs_group(hba, &sec_ufs_cmd_dev,
				&sec_ufs_wb_attribute_group, "sec_ufs_wb");

#if IS_ENABLED(CONFIG_SEC_UFS_CMD_LOGGING)
		if (ufs_sec_is_cmd_log_allowed())
			ufs_sec_create_sysfs_group(hba, &sec_ufs_cmd_dev,
				&sec_ufs_cmd_log_attribute_group, "sec_ufs_cmd_log");
#endif
	}
>>>>>>> upstream/android-13
}

void ufs_sec_remove_sysfs_nodes(struct ufs_hba *hba)
{
<<<<<<< HEAD
	sysfs_remove_group(&sec_ufs_cmd_dev->kobj, &sec_ufs_info_attribute_group);
=======
	struct device *shost_dev = &(hba->host->shost_dev);

	if (sec_ufs_cmd_dev) {
		sysfs_remove_group(&sec_ufs_cmd_dev->kobj,
				&sec_ufs_info_attribute_group);

		sysfs_remove_group(&sec_ufs_cmd_dev->kobj,
				&sec_ufs_wb_attribute_group);

		sysfs_remove_group(&sec_ufs_cmd_dev->kobj,
				&sec_ufs_cmd_log_attribute_group);
	}

	if (shost_dev)
		sysfs_remove_group(&shost_dev->kobj,
				&sec_ufs_error_attribute_group);
>>>>>>> upstream/android-13
}
