/*
 * Copyright (c) 2012 Intel Corporation.  All rights reserved.
 * Copyright (c) 2006 - 2012 QLogic Corporation. All rights reserved.
 * Copyright (c) 2006 PathScale, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <linux/ctype.h>
<<<<<<< HEAD
=======
#include <rdma/ib_sysfs.h>
>>>>>>> upstream/android-13

#include "qib.h"
#include "qib_mad.h"

<<<<<<< HEAD
/* start of per-port functions */
/*
 * Get/Set heartbeat enable. OR of 1=enabled, 2=auto
 */
static ssize_t show_hrtbt_enb(struct qib_pportdata *ppd, char *buf)
{
	struct qib_devdata *dd = ppd->dd;
	int ret;

	ret = dd->f_get_ib_cfg(ppd, QIB_IB_CFG_HRTBT);
	ret = scnprintf(buf, PAGE_SIZE, "%d\n", ret);
	return ret;
}

static ssize_t store_hrtbt_enb(struct qib_pportdata *ppd, const char *buf,
			       size_t count)
{
	struct qib_devdata *dd = ppd->dd;
=======
static struct qib_pportdata *qib_get_pportdata_kobj(struct kobject *kobj)
{
	u32 port_num;
	struct ib_device *ibdev = ib_port_sysfs_get_ibdev_kobj(kobj, &port_num);
	struct qib_devdata *dd = dd_from_ibdev(ibdev);

	return &dd->pport[port_num - 1];
}

/*
 * Get/Set heartbeat enable. OR of 1=enabled, 2=auto
 */
static ssize_t hrtbt_enable_show(struct ib_device *ibdev, u32 port_num,
				 struct ib_port_attribute *attr, char *buf)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_pportdata *ppd = &dd->pport[port_num - 1];

	return sysfs_emit(buf, "%d\n", dd->f_get_ib_cfg(ppd, QIB_IB_CFG_HRTBT));
}

static ssize_t hrtbt_enable_store(struct ib_device *ibdev, u32 port_num,
				  struct ib_port_attribute *attr,
				  const char *buf, size_t count)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_pportdata *ppd = &dd->pport[port_num - 1];
>>>>>>> upstream/android-13
	int ret;
	u16 val;

	ret = kstrtou16(buf, 0, &val);
	if (ret) {
		qib_dev_err(dd, "attempt to set invalid Heartbeat enable\n");
		return ret;
	}

	/*
	 * Set the "intentional" heartbeat enable per either of
	 * "Enable" and "Auto", as these are normally set together.
	 * This bit is consulted when leaving loopback mode,
	 * because entering loopback mode overrides it and automatically
	 * disables heartbeat.
	 */
	ret = dd->f_set_ib_cfg(ppd, QIB_IB_CFG_HRTBT, val);
	return ret < 0 ? ret : count;
}
<<<<<<< HEAD

static ssize_t store_loopback(struct qib_pportdata *ppd, const char *buf,
			      size_t count)
{
	struct qib_devdata *dd = ppd->dd;
=======
static IB_PORT_ATTR_RW(hrtbt_enable);

static ssize_t loopback_store(struct ib_device *ibdev, u32 port_num,
			      struct ib_port_attribute *attr, const char *buf,
			      size_t count)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_pportdata *ppd = &dd->pport[port_num - 1];
>>>>>>> upstream/android-13
	int ret = count, r;

	r = dd->f_set_ib_loopback(ppd, buf);
	if (r < 0)
		ret = r;

	return ret;
}
<<<<<<< HEAD

static ssize_t store_led_override(struct qib_pportdata *ppd, const char *buf,
				  size_t count)
{
	struct qib_devdata *dd = ppd->dd;
=======
static IB_PORT_ATTR_WO(loopback);

static ssize_t led_override_store(struct ib_device *ibdev, u32 port_num,
				  struct ib_port_attribute *attr,
				  const char *buf, size_t count)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_pportdata *ppd = &dd->pport[port_num - 1];
>>>>>>> upstream/android-13
	int ret;
	u16 val;

	ret = kstrtou16(buf, 0, &val);
	if (ret) {
		qib_dev_err(dd, "attempt to set invalid LED override\n");
		return ret;
	}

	qib_set_led_override(ppd, val);
	return count;
}
<<<<<<< HEAD

static ssize_t show_status(struct qib_pportdata *ppd, char *buf)
{
	ssize_t ret;

	if (!ppd->statusp)
		ret = -EINVAL;
	else
		ret = scnprintf(buf, PAGE_SIZE, "0x%llx\n",
				(unsigned long long) *(ppd->statusp));
	return ret;
}
=======
static IB_PORT_ATTR_WO(led_override);

static ssize_t status_show(struct ib_device *ibdev, u32 port_num,
			   struct ib_port_attribute *attr, char *buf)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_pportdata *ppd = &dd->pport[port_num - 1];

	if (!ppd->statusp)
		return -EINVAL;

	return sysfs_emit(buf, "0x%llx\n", (unsigned long long)*(ppd->statusp));
}
static IB_PORT_ATTR_RO(status);
>>>>>>> upstream/android-13

/*
 * For userland compatibility, these offsets must remain fixed.
 * They are strings for QIB_STATUS_*
 */
static const char * const qib_status_str[] = {
	"Initted",
	"",
	"",
	"",
	"",
	"Present",
	"IB_link_up",
	"IB_configured",
	"",
	"Fatal_Hardware_Error",
	NULL,
};

<<<<<<< HEAD
static ssize_t show_status_str(struct qib_pportdata *ppd, char *buf)
{
=======
static ssize_t status_str_show(struct ib_device *ibdev, u32 port_num,
			       struct ib_port_attribute *attr, char *buf)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_pportdata *ppd = &dd->pport[port_num - 1];
>>>>>>> upstream/android-13
	int i, any;
	u64 s;
	ssize_t ret;

	if (!ppd->statusp) {
		ret = -EINVAL;
		goto bail;
	}

	s = *(ppd->statusp);
	*buf = '\0';
	for (any = i = 0; s && qib_status_str[i]; i++) {
		if (s & 1) {
			/* if overflow */
			if (any && strlcat(buf, " ", PAGE_SIZE) >= PAGE_SIZE)
				break;
			if (strlcat(buf, qib_status_str[i], PAGE_SIZE) >=
					PAGE_SIZE)
				break;
			any = 1;
		}
		s >>= 1;
	}
	if (any)
		strlcat(buf, "\n", PAGE_SIZE);

	ret = strlen(buf);

bail:
	return ret;
}
<<<<<<< HEAD

/* end of per-port functions */

/*
 * Start of per-port file structures and support code
 * Because we are fitting into other infrastructure, we have to supply the
 * full set of kobject/sysfs_ops structures and routines.
 */
#define QIB_PORT_ATTR(name, mode, show, store) \
	static struct qib_port_attr qib_port_attr_##name = \
		__ATTR(name, mode, show, store)

struct qib_port_attr {
	struct attribute attr;
	ssize_t (*show)(struct qib_pportdata *, char *);
	ssize_t (*store)(struct qib_pportdata *, const char *, size_t);
};

QIB_PORT_ATTR(loopback, S_IWUSR, NULL, store_loopback);
QIB_PORT_ATTR(led_override, S_IWUSR, NULL, store_led_override);
QIB_PORT_ATTR(hrtbt_enable, S_IWUSR | S_IRUGO, show_hrtbt_enb,
	      store_hrtbt_enb);
QIB_PORT_ATTR(status, S_IRUGO, show_status, NULL);
QIB_PORT_ATTR(status_str, S_IRUGO, show_status_str, NULL);

static struct attribute *port_default_attributes[] = {
	&qib_port_attr_loopback.attr,
	&qib_port_attr_led_override.attr,
	&qib_port_attr_hrtbt_enable.attr,
	&qib_port_attr_status.attr,
	&qib_port_attr_status_str.attr,
	NULL
=======
static IB_PORT_ATTR_RO(status_str);

/* end of per-port functions */

static struct attribute *port_linkcontrol_attributes[] = {
	&ib_port_attr_loopback.attr,
	&ib_port_attr_led_override.attr,
	&ib_port_attr_hrtbt_enable.attr,
	&ib_port_attr_status.attr,
	&ib_port_attr_status_str.attr,
	NULL
};

static const struct attribute_group port_linkcontrol_group = {
	.name = "linkcontrol",
	.attrs = port_linkcontrol_attributes,
>>>>>>> upstream/android-13
};

/*
 * Start of per-port congestion control structures and support code
 */

/*
 * Congestion control table size followed by table entries
 */
<<<<<<< HEAD
static ssize_t read_cc_table_bin(struct file *filp, struct kobject *kobj,
		struct bin_attribute *bin_attr,
		char *buf, loff_t pos, size_t count)
{
	int ret;
	struct qib_pportdata *ppd =
		container_of(kobj, struct qib_pportdata, pport_cc_kobj);
=======
static ssize_t cc_table_bin_read(struct file *filp, struct kobject *kobj,
				 struct bin_attribute *bin_attr, char *buf,
				 loff_t pos, size_t count)
{
	struct qib_pportdata *ppd = qib_get_pportdata_kobj(kobj);
	int ret;
>>>>>>> upstream/android-13

	if (!qib_cc_table_size || !ppd->ccti_entries_shadow)
		return -EINVAL;

	ret = ppd->total_cct_entry * sizeof(struct ib_cc_table_entry_shadow)
		 + sizeof(__be16);

	if (pos > ret)
		return -EINVAL;

	if (count > ret - pos)
		count = ret - pos;

	if (!count)
		return count;

	spin_lock(&ppd->cc_shadow_lock);
	memcpy(buf, ppd->ccti_entries_shadow, count);
	spin_unlock(&ppd->cc_shadow_lock);

	return count;
}
<<<<<<< HEAD

static void qib_port_release(struct kobject *kobj)
{
	/* nothing to do since memory is freed by qib_free_devdata() */
}

static struct kobj_type qib_port_cc_ktype = {
	.release = qib_port_release,
};

static const struct bin_attribute cc_table_bin_attr = {
	.attr = {.name = "cc_table_bin", .mode = 0444},
	.read = read_cc_table_bin,
	.size = PAGE_SIZE,
};
=======
static BIN_ATTR_RO(cc_table_bin, PAGE_SIZE);
>>>>>>> upstream/android-13

/*
 * Congestion settings: port control, control map and an array of 16
 * entries for the congestion entries - increase, timer, event log
 * trigger threshold and the minimum injection rate delay.
 */
<<<<<<< HEAD
static ssize_t read_cc_setting_bin(struct file *filp, struct kobject *kobj,
		struct bin_attribute *bin_attr,
		char *buf, loff_t pos, size_t count)
{
	int ret;
	struct qib_pportdata *ppd =
		container_of(kobj, struct qib_pportdata, pport_cc_kobj);
=======
static ssize_t cc_setting_bin_read(struct file *filp, struct kobject *kobj,
				   struct bin_attribute *bin_attr, char *buf,
				   loff_t pos, size_t count)
{
	struct qib_pportdata *ppd = qib_get_pportdata_kobj(kobj);
	int ret;
>>>>>>> upstream/android-13

	if (!qib_cc_table_size || !ppd->congestion_entries_shadow)
		return -EINVAL;

	ret = sizeof(struct ib_cc_congestion_setting_attr_shadow);

	if (pos > ret)
		return -EINVAL;
	if (count > ret - pos)
		count = ret - pos;

	if (!count)
		return count;

	spin_lock(&ppd->cc_shadow_lock);
	memcpy(buf, ppd->congestion_entries_shadow, count);
	spin_unlock(&ppd->cc_shadow_lock);

	return count;
}
<<<<<<< HEAD

static const struct bin_attribute cc_setting_bin_attr = {
	.attr = {.name = "cc_settings_bin", .mode = 0444},
	.read = read_cc_setting_bin,
	.size = PAGE_SIZE,
};


static ssize_t qib_portattr_show(struct kobject *kobj,
	struct attribute *attr, char *buf)
{
	struct qib_port_attr *pattr =
		container_of(attr, struct qib_port_attr, attr);
	struct qib_pportdata *ppd =
		container_of(kobj, struct qib_pportdata, pport_kobj);

	if (!pattr->show)
		return -EIO;

	return pattr->show(ppd, buf);
}

static ssize_t qib_portattr_store(struct kobject *kobj,
	struct attribute *attr, const char *buf, size_t len)
{
	struct qib_port_attr *pattr =
		container_of(attr, struct qib_port_attr, attr);
	struct qib_pportdata *ppd =
		container_of(kobj, struct qib_pportdata, pport_kobj);

	if (!pattr->store)
		return -EIO;

	return pattr->store(ppd, buf, len);
}


static const struct sysfs_ops qib_port_ops = {
	.show = qib_portattr_show,
	.store = qib_portattr_store,
};

static struct kobj_type qib_port_ktype = {
	.release = qib_port_release,
	.sysfs_ops = &qib_port_ops,
	.default_attrs = port_default_attributes
=======
static BIN_ATTR_RO(cc_setting_bin, PAGE_SIZE);

static struct bin_attribute *port_ccmgta_attributes[] = {
	&bin_attr_cc_setting_bin,
	&bin_attr_cc_table_bin,
	NULL,
};

static umode_t qib_ccmgta_is_bin_visible(struct kobject *kobj,
				 struct bin_attribute *attr, int n)
{
	struct qib_pportdata *ppd = qib_get_pportdata_kobj(kobj);

	if (!qib_cc_table_size || !ppd->congestion_entries_shadow)
		return 0;
	return attr->attr.mode;
}

static const struct attribute_group port_ccmgta_attribute_group = {
	.name = "CCMgtA",
	.is_bin_visible = qib_ccmgta_is_bin_visible,
	.bin_attrs = port_ccmgta_attributes,
>>>>>>> upstream/android-13
};

/* Start sl2vl */

<<<<<<< HEAD
#define QIB_SL2VL_ATTR(N) \
	static struct qib_sl2vl_attr qib_sl2vl_attr_##N = { \
		.attr = { .name = __stringify(N), .mode = 0444 }, \
		.sl = N \
	}

struct qib_sl2vl_attr {
	struct attribute attr;
	int sl;
};

=======
struct qib_sl2vl_attr {
	struct ib_port_attribute attr;
	int sl;
};

static ssize_t sl2vl_attr_show(struct ib_device *ibdev, u32 port_num,
			       struct ib_port_attribute *attr, char *buf)
{
	struct qib_sl2vl_attr *sattr =
		container_of(attr, struct qib_sl2vl_attr, attr);
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_ibport *qibp = &dd->pport[port_num - 1].ibport_data;

	return sysfs_emit(buf, "%u\n", qibp->sl_to_vl[sattr->sl]);
}

#define QIB_SL2VL_ATTR(N)                                                      \
	static struct qib_sl2vl_attr qib_sl2vl_attr_##N = {                    \
		.attr = __ATTR(N, 0444, sl2vl_attr_show, NULL),                \
		.sl = N,                                                       \
	}

>>>>>>> upstream/android-13
QIB_SL2VL_ATTR(0);
QIB_SL2VL_ATTR(1);
QIB_SL2VL_ATTR(2);
QIB_SL2VL_ATTR(3);
QIB_SL2VL_ATTR(4);
QIB_SL2VL_ATTR(5);
QIB_SL2VL_ATTR(6);
QIB_SL2VL_ATTR(7);
QIB_SL2VL_ATTR(8);
QIB_SL2VL_ATTR(9);
QIB_SL2VL_ATTR(10);
QIB_SL2VL_ATTR(11);
QIB_SL2VL_ATTR(12);
QIB_SL2VL_ATTR(13);
QIB_SL2VL_ATTR(14);
QIB_SL2VL_ATTR(15);

<<<<<<< HEAD
static struct attribute *sl2vl_default_attributes[] = {
	&qib_sl2vl_attr_0.attr,
	&qib_sl2vl_attr_1.attr,
	&qib_sl2vl_attr_2.attr,
	&qib_sl2vl_attr_3.attr,
	&qib_sl2vl_attr_4.attr,
	&qib_sl2vl_attr_5.attr,
	&qib_sl2vl_attr_6.attr,
	&qib_sl2vl_attr_7.attr,
	&qib_sl2vl_attr_8.attr,
	&qib_sl2vl_attr_9.attr,
	&qib_sl2vl_attr_10.attr,
	&qib_sl2vl_attr_11.attr,
	&qib_sl2vl_attr_12.attr,
	&qib_sl2vl_attr_13.attr,
	&qib_sl2vl_attr_14.attr,
	&qib_sl2vl_attr_15.attr,
	NULL
};

static ssize_t sl2vl_attr_show(struct kobject *kobj, struct attribute *attr,
			       char *buf)
{
	struct qib_sl2vl_attr *sattr =
		container_of(attr, struct qib_sl2vl_attr, attr);
	struct qib_pportdata *ppd =
		container_of(kobj, struct qib_pportdata, sl2vl_kobj);
	struct qib_ibport *qibp = &ppd->ibport_data;

	return sprintf(buf, "%u\n", qibp->sl_to_vl[sattr->sl]);
}

static const struct sysfs_ops qib_sl2vl_ops = {
	.show = sl2vl_attr_show,
};

static struct kobj_type qib_sl2vl_ktype = {
	.release = qib_port_release,
	.sysfs_ops = &qib_sl2vl_ops,
	.default_attrs = sl2vl_default_attributes
=======
static struct attribute *port_sl2vl_attributes[] = {
	&qib_sl2vl_attr_0.attr.attr,
	&qib_sl2vl_attr_1.attr.attr,
	&qib_sl2vl_attr_2.attr.attr,
	&qib_sl2vl_attr_3.attr.attr,
	&qib_sl2vl_attr_4.attr.attr,
	&qib_sl2vl_attr_5.attr.attr,
	&qib_sl2vl_attr_6.attr.attr,
	&qib_sl2vl_attr_7.attr.attr,
	&qib_sl2vl_attr_8.attr.attr,
	&qib_sl2vl_attr_9.attr.attr,
	&qib_sl2vl_attr_10.attr.attr,
	&qib_sl2vl_attr_11.attr.attr,
	&qib_sl2vl_attr_12.attr.attr,
	&qib_sl2vl_attr_13.attr.attr,
	&qib_sl2vl_attr_14.attr.attr,
	&qib_sl2vl_attr_15.attr.attr,
	NULL
};

static const struct attribute_group port_sl2vl_group = {
	.name = "sl2vl",
	.attrs = port_sl2vl_attributes,
>>>>>>> upstream/android-13
};

/* End sl2vl */

/* Start diag_counters */

<<<<<<< HEAD
#define QIB_DIAGC_ATTR(N) \
	static struct qib_diagc_attr qib_diagc_attr_##N = { \
		.attr = { .name = __stringify(N), .mode = 0664 }, \
		.counter = offsetof(struct qib_ibport, rvp.n_##N) \
	}

#define QIB_DIAGC_ATTR_PER_CPU(N) \
	static struct qib_diagc_attr qib_diagc_attr_##N = { \
		.attr = { .name = __stringify(N), .mode = 0664 }, \
		.counter = offsetof(struct qib_ibport, rvp.z_##N) \
	}

struct qib_diagc_attr {
	struct attribute attr;
	size_t counter;
};

QIB_DIAGC_ATTR_PER_CPU(rc_acks);
QIB_DIAGC_ATTR_PER_CPU(rc_qacks);
QIB_DIAGC_ATTR_PER_CPU(rc_delayed_comp);
=======
struct qib_diagc_attr {
	struct ib_port_attribute attr;
	size_t counter;
};

static ssize_t diagc_attr_show(struct ib_device *ibdev, u32 port_num,
			       struct ib_port_attribute *attr, char *buf)
{
	struct qib_diagc_attr *dattr =
		container_of(attr, struct qib_diagc_attr, attr);
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_ibport *qibp = &dd->pport[port_num - 1].ibport_data;

	return sysfs_emit(buf, "%llu\n", *((u64 *)qibp + dattr->counter));
}

static ssize_t diagc_attr_store(struct ib_device *ibdev, u32 port_num,
				struct ib_port_attribute *attr, const char *buf,
				size_t count)
{
	struct qib_diagc_attr *dattr =
		container_of(attr, struct qib_diagc_attr, attr);
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_ibport *qibp = &dd->pport[port_num - 1].ibport_data;
	u64 val;
	int ret;

	ret = kstrtou64(buf, 0, &val);
	if (ret)
		return ret;
	*((u64 *)qibp + dattr->counter) = val;
	return count;
}

#define QIB_DIAGC_ATTR(N)                                                      \
	static_assert(__same_type(((struct qib_ibport *)0)->rvp.n_##N, u64));  \
	static struct qib_diagc_attr qib_diagc_attr_##N = {                    \
		.attr = __ATTR(N, 0664, diagc_attr_show, diagc_attr_store),    \
		.counter =                                                     \
			offsetof(struct qib_ibport, rvp.n_##N) / sizeof(u64)   \
	}
>>>>>>> upstream/android-13

QIB_DIAGC_ATTR(rc_resends);
QIB_DIAGC_ATTR(seq_naks);
QIB_DIAGC_ATTR(rdma_seq);
QIB_DIAGC_ATTR(rnr_naks);
QIB_DIAGC_ATTR(other_naks);
QIB_DIAGC_ATTR(rc_timeouts);
QIB_DIAGC_ATTR(loop_pkts);
QIB_DIAGC_ATTR(pkt_drops);
QIB_DIAGC_ATTR(dmawait);
QIB_DIAGC_ATTR(unaligned);
QIB_DIAGC_ATTR(rc_dupreq);
QIB_DIAGC_ATTR(rc_seqnak);
<<<<<<< HEAD

static struct attribute *diagc_default_attributes[] = {
	&qib_diagc_attr_rc_resends.attr,
	&qib_diagc_attr_rc_acks.attr,
	&qib_diagc_attr_rc_qacks.attr,
	&qib_diagc_attr_rc_delayed_comp.attr,
	&qib_diagc_attr_seq_naks.attr,
	&qib_diagc_attr_rdma_seq.attr,
	&qib_diagc_attr_rnr_naks.attr,
	&qib_diagc_attr_other_naks.attr,
	&qib_diagc_attr_rc_timeouts.attr,
	&qib_diagc_attr_loop_pkts.attr,
	&qib_diagc_attr_pkt_drops.attr,
	&qib_diagc_attr_dmawait.attr,
	&qib_diagc_attr_unaligned.attr,
	&qib_diagc_attr_rc_dupreq.attr,
	&qib_diagc_attr_rc_seqnak.attr,
	NULL
};
=======
QIB_DIAGC_ATTR(rc_crwaits);
>>>>>>> upstream/android-13

static u64 get_all_cpu_total(u64 __percpu *cntr)
{
	int cpu;
	u64 counter = 0;

	for_each_possible_cpu(cpu)
		counter += *per_cpu_ptr(cntr, cpu);
	return counter;
}

<<<<<<< HEAD
#define def_write_per_cpu(cntr) \
static void write_per_cpu_##cntr(struct qib_pportdata *ppd, u32 data)	\
{									\
	struct qib_devdata *dd = ppd->dd;				\
	struct qib_ibport *qibp = &ppd->ibport_data;			\
	/*  A write can only zero the counter */			\
	if (data == 0)							\
		qibp->rvp.z_##cntr = get_all_cpu_total(qibp->rvp.cntr); \
	else								\
		qib_dev_err(dd, "Per CPU cntrs can only be zeroed");	\
}

def_write_per_cpu(rc_acks)
def_write_per_cpu(rc_qacks)
def_write_per_cpu(rc_delayed_comp)

#define READ_PER_CPU_CNTR(cntr) (get_all_cpu_total(qibp->rvp.cntr) - \
							qibp->rvp.z_##cntr)

static ssize_t diagc_attr_show(struct kobject *kobj, struct attribute *attr,
			       char *buf)
{
	struct qib_diagc_attr *dattr =
		container_of(attr, struct qib_diagc_attr, attr);
	struct qib_pportdata *ppd =
		container_of(kobj, struct qib_pportdata, diagc_kobj);
	struct qib_ibport *qibp = &ppd->ibport_data;

	if (!strncmp(dattr->attr.name, "rc_acks", 7))
		return sprintf(buf, "%llu\n", READ_PER_CPU_CNTR(rc_acks));
	else if (!strncmp(dattr->attr.name, "rc_qacks", 8))
		return sprintf(buf, "%llu\n", READ_PER_CPU_CNTR(rc_qacks));
	else if (!strncmp(dattr->attr.name, "rc_delayed_comp", 15))
		return sprintf(buf, "%llu\n",
					READ_PER_CPU_CNTR(rc_delayed_comp));
	else
		return sprintf(buf, "%u\n",
				*(u32 *)((char *)qibp + dattr->counter));
}

static ssize_t diagc_attr_store(struct kobject *kobj, struct attribute *attr,
				const char *buf, size_t size)
{
	struct qib_diagc_attr *dattr =
		container_of(attr, struct qib_diagc_attr, attr);
	struct qib_pportdata *ppd =
		container_of(kobj, struct qib_pportdata, diagc_kobj);
	struct qib_ibport *qibp = &ppd->ibport_data;
=======
static ssize_t qib_store_per_cpu(struct qib_devdata *dd, const char *buf,
				 size_t count, u64 *zero, u64 cur)
{
>>>>>>> upstream/android-13
	u32 val;
	int ret;

	ret = kstrtou32(buf, 0, &val);
	if (ret)
		return ret;
<<<<<<< HEAD

	if (!strncmp(dattr->attr.name, "rc_acks", 7))
		write_per_cpu_rc_acks(ppd, val);
	else if (!strncmp(dattr->attr.name, "rc_qacks", 8))
		write_per_cpu_rc_qacks(ppd, val);
	else if (!strncmp(dattr->attr.name, "rc_delayed_comp", 15))
		write_per_cpu_rc_delayed_comp(ppd, val);
	else
		*(u32 *)((char *)qibp + dattr->counter) = val;
	return size;
}

static const struct sysfs_ops qib_diagc_ops = {
	.show = diagc_attr_show,
	.store = diagc_attr_store,
};

static struct kobj_type qib_diagc_ktype = {
	.release = qib_port_release,
	.sysfs_ops = &qib_diagc_ops,
	.default_attrs = diagc_default_attributes
=======
	if (val != 0) {
		qib_dev_err(dd, "Per CPU cntrs can only be zeroed");
		return count;
	}
	*zero = cur;
	return count;
}

static ssize_t rc_acks_show(struct ib_device *ibdev, u32 port_num,
			    struct ib_port_attribute *attr, char *buf)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_ibport *qibp = &dd->pport[port_num - 1].ibport_data;

	return sysfs_emit(buf, "%llu\n",
			  get_all_cpu_total(qibp->rvp.rc_acks) -
				  qibp->rvp.z_rc_acks);
}

static ssize_t rc_acks_store(struct ib_device *ibdev, u32 port_num,
			     struct ib_port_attribute *attr, const char *buf,
			     size_t count)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_ibport *qibp = &dd->pport[port_num - 1].ibport_data;

	return qib_store_per_cpu(dd, buf, count, &qibp->rvp.z_rc_acks,
				 get_all_cpu_total(qibp->rvp.rc_acks));
}
static IB_PORT_ATTR_RW(rc_acks);

static ssize_t rc_qacks_show(struct ib_device *ibdev, u32 port_num,
			     struct ib_port_attribute *attr, char *buf)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_ibport *qibp = &dd->pport[port_num - 1].ibport_data;

	return sysfs_emit(buf, "%llu\n",
			  get_all_cpu_total(qibp->rvp.rc_qacks) -
				  qibp->rvp.z_rc_qacks);
}

static ssize_t rc_qacks_store(struct ib_device *ibdev, u32 port_num,
			      struct ib_port_attribute *attr, const char *buf,
			      size_t count)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_ibport *qibp = &dd->pport[port_num - 1].ibport_data;

	return qib_store_per_cpu(dd, buf, count, &qibp->rvp.z_rc_qacks,
				 get_all_cpu_total(qibp->rvp.rc_qacks));
}
static IB_PORT_ATTR_RW(rc_qacks);

static ssize_t rc_delayed_comp_show(struct ib_device *ibdev, u32 port_num,
				    struct ib_port_attribute *attr, char *buf)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_ibport *qibp = &dd->pport[port_num - 1].ibport_data;

	return sysfs_emit(buf, "%llu\n",
			 get_all_cpu_total(qibp->rvp.rc_delayed_comp) -
				 qibp->rvp.z_rc_delayed_comp);
}

static ssize_t rc_delayed_comp_store(struct ib_device *ibdev, u32 port_num,
				     struct ib_port_attribute *attr,
				     const char *buf, size_t count)
{
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	struct qib_ibport *qibp = &dd->pport[port_num - 1].ibport_data;

	return qib_store_per_cpu(dd, buf, count, &qibp->rvp.z_rc_delayed_comp,
				 get_all_cpu_total(qibp->rvp.rc_delayed_comp));
}
static IB_PORT_ATTR_RW(rc_delayed_comp);

static struct attribute *port_diagc_attributes[] = {
	&qib_diagc_attr_rc_resends.attr.attr,
	&qib_diagc_attr_seq_naks.attr.attr,
	&qib_diagc_attr_rdma_seq.attr.attr,
	&qib_diagc_attr_rnr_naks.attr.attr,
	&qib_diagc_attr_other_naks.attr.attr,
	&qib_diagc_attr_rc_timeouts.attr.attr,
	&qib_diagc_attr_loop_pkts.attr.attr,
	&qib_diagc_attr_pkt_drops.attr.attr,
	&qib_diagc_attr_dmawait.attr.attr,
	&qib_diagc_attr_unaligned.attr.attr,
	&qib_diagc_attr_rc_dupreq.attr.attr,
	&qib_diagc_attr_rc_seqnak.attr.attr,
	&qib_diagc_attr_rc_crwaits.attr.attr,
	&ib_port_attr_rc_acks.attr,
	&ib_port_attr_rc_qacks.attr,
	&ib_port_attr_rc_delayed_comp.attr,
	NULL
};

static const struct attribute_group port_diagc_group = {
	.name = "diag_counters",
	.attrs = port_diagc_attributes,
>>>>>>> upstream/android-13
};

/* End diag_counters */

<<<<<<< HEAD
=======
const struct attribute_group *qib_attr_port_groups[] = {
	&port_linkcontrol_group,
	&port_ccmgta_attribute_group,
	&port_sl2vl_group,
	&port_diagc_group,
	NULL,
};

>>>>>>> upstream/android-13
/* end of per-port file structures and support code */

/*
 * Start of per-unit (or driver, in some cases, but replicated
 * per unit) functions (these get a device *)
 */
<<<<<<< HEAD
static ssize_t show_rev(struct device *device, struct device_attribute *attr,
			char *buf)
{
	struct qib_ibdev *dev =
		container_of(device, struct qib_ibdev, rdi.ibdev.dev);

	return sprintf(buf, "%x\n", dd_from_dev(dev)->minrev);
}

static ssize_t show_hca(struct device *device, struct device_attribute *attr,
			char *buf)
{
	struct qib_ibdev *dev =
		container_of(device, struct qib_ibdev, rdi.ibdev.dev);
	struct qib_devdata *dd = dd_from_dev(dev);
	int ret;

	if (!dd->boardname)
		ret = -EINVAL;
	else
		ret = scnprintf(buf, PAGE_SIZE, "%s\n", dd->boardname);
	return ret;
}

static ssize_t show_version(struct device *device,
			    struct device_attribute *attr, char *buf)
{
	/* The string printed here is already newline-terminated. */
	return scnprintf(buf, PAGE_SIZE, "%s", (char *)ib_qib_version);
}

static ssize_t show_boardversion(struct device *device,
				 struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		container_of(device, struct qib_ibdev, rdi.ibdev.dev);
	struct qib_devdata *dd = dd_from_dev(dev);

	/* The string printed here is already newline-terminated. */
	return scnprintf(buf, PAGE_SIZE, "%s", dd->boardversion);
}


static ssize_t show_localbus_info(struct device *device,
				  struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		container_of(device, struct qib_ibdev, rdi.ibdev.dev);
	struct qib_devdata *dd = dd_from_dev(dev);

	/* The string printed here is already newline-terminated. */
	return scnprintf(buf, PAGE_SIZE, "%s", dd->lbus_info);
}


static ssize_t show_nctxts(struct device *device,
			   struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		container_of(device, struct qib_ibdev, rdi.ibdev.dev);
=======
static ssize_t hw_rev_show(struct device *device, struct device_attribute *attr,
			   char *buf)
{
	struct qib_ibdev *dev =
		rdma_device_to_drv_device(device, struct qib_ibdev, rdi.ibdev);

	return sysfs_emit(buf, "%x\n", dd_from_dev(dev)->minrev);
}
static DEVICE_ATTR_RO(hw_rev);

static ssize_t hca_type_show(struct device *device,
			     struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		rdma_device_to_drv_device(device, struct qib_ibdev, rdi.ibdev);
	struct qib_devdata *dd = dd_from_dev(dev);

	if (!dd->boardname)
		return -EINVAL;
	return sysfs_emit(buf, "%s\n", dd->boardname);
}
static DEVICE_ATTR_RO(hca_type);
static DEVICE_ATTR(board_id, 0444, hca_type_show, NULL);

static ssize_t version_show(struct device *device,
			    struct device_attribute *attr, char *buf)
{
	/* The string printed here is already newline-terminated. */
	return sysfs_emit(buf, "%s", (char *)ib_qib_version);
}
static DEVICE_ATTR_RO(version);

static ssize_t boardversion_show(struct device *device,
				 struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		rdma_device_to_drv_device(device, struct qib_ibdev, rdi.ibdev);
	struct qib_devdata *dd = dd_from_dev(dev);

	/* The string printed here is already newline-terminated. */
	return sysfs_emit(buf, "%s", dd->boardversion);
}
static DEVICE_ATTR_RO(boardversion);

static ssize_t localbus_info_show(struct device *device,
				  struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		rdma_device_to_drv_device(device, struct qib_ibdev, rdi.ibdev);
	struct qib_devdata *dd = dd_from_dev(dev);

	/* The string printed here is already newline-terminated. */
	return sysfs_emit(buf, "%s", dd->lbus_info);
}
static DEVICE_ATTR_RO(localbus_info);

static ssize_t nctxts_show(struct device *device,
			   struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		rdma_device_to_drv_device(device, struct qib_ibdev, rdi.ibdev);
>>>>>>> upstream/android-13
	struct qib_devdata *dd = dd_from_dev(dev);

	/* Return the number of user ports (contexts) available. */
	/* The calculation below deals with a special case where
	 * cfgctxts is set to 1 on a single-port board. */
<<<<<<< HEAD
	return scnprintf(buf, PAGE_SIZE, "%u\n",
			(dd->first_user_ctxt > dd->cfgctxts) ? 0 :
			(dd->cfgctxts - dd->first_user_ctxt));
}

static ssize_t show_nfreectxts(struct device *device,
			   struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		container_of(device, struct qib_ibdev, rdi.ibdev.dev);
	struct qib_devdata *dd = dd_from_dev(dev);

	/* Return the number of free user ports (contexts) available. */
	return scnprintf(buf, PAGE_SIZE, "%u\n", dd->freectxts);
}

static ssize_t show_serial(struct device *device,
			   struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		container_of(device, struct qib_ibdev, rdi.ibdev.dev);
	struct qib_devdata *dd = dd_from_dev(dev);

	buf[sizeof(dd->serial)] = '\0';
	memcpy(buf, dd->serial, sizeof(dd->serial));
	strcat(buf, "\n");
	return strlen(buf);
}

static ssize_t store_chip_reset(struct device *device,
=======
	return sysfs_emit(buf, "%u\n",
			  (dd->first_user_ctxt > dd->cfgctxts) ?
				  0 :
				  (dd->cfgctxts - dd->first_user_ctxt));
}
static DEVICE_ATTR_RO(nctxts);

static ssize_t nfreectxts_show(struct device *device,
			       struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		rdma_device_to_drv_device(device, struct qib_ibdev, rdi.ibdev);
	struct qib_devdata *dd = dd_from_dev(dev);

	/* Return the number of free user ports (contexts) available. */
	return sysfs_emit(buf, "%u\n", dd->freectxts);
}
static DEVICE_ATTR_RO(nfreectxts);

static ssize_t serial_show(struct device *device, struct device_attribute *attr,
			   char *buf)
{
	struct qib_ibdev *dev =
		rdma_device_to_drv_device(device, struct qib_ibdev, rdi.ibdev);
	struct qib_devdata *dd = dd_from_dev(dev);
	const u8 *end = memchr(dd->serial, 0, ARRAY_SIZE(dd->serial));
	int size = end ? end - dd->serial : ARRAY_SIZE(dd->serial);

	return sysfs_emit(buf, ".%*s\n", size, dd->serial);
}
static DEVICE_ATTR_RO(serial);

static ssize_t chip_reset_store(struct device *device,
>>>>>>> upstream/android-13
				struct device_attribute *attr, const char *buf,
				size_t count)
{
	struct qib_ibdev *dev =
<<<<<<< HEAD
		container_of(device, struct qib_ibdev, rdi.ibdev.dev);
=======
		rdma_device_to_drv_device(device, struct qib_ibdev, rdi.ibdev);
>>>>>>> upstream/android-13
	struct qib_devdata *dd = dd_from_dev(dev);
	int ret;

	if (count < 5 || memcmp(buf, "reset", 5) || !dd->diag_client) {
		ret = -EINVAL;
		goto bail;
	}

	ret = qib_reset_device(dd->unit);
bail:
	return ret < 0 ? ret : count;
}
<<<<<<< HEAD
=======
static DEVICE_ATTR_WO(chip_reset);
>>>>>>> upstream/android-13

/*
 * Dump tempsense regs. in decimal, to ease shell-scripts.
 */
<<<<<<< HEAD
static ssize_t show_tempsense(struct device *device,
			      struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		container_of(device, struct qib_ibdev, rdi.ibdev.dev);
	struct qib_devdata *dd = dd_from_dev(dev);
	int ret;
	int idx;
	u8 regvals[8];

	ret = -ENXIO;
	for (idx = 0; idx < 8; ++idx) {
		if (idx == 6)
			continue;
		ret = dd->f_tempsense_rd(dd, idx);
		if (ret < 0)
			break;
		regvals[idx] = ret;
	}
	if (idx == 8)
		ret = scnprintf(buf, PAGE_SIZE, "%d %d %02X %02X %d %d\n",
				*(signed char *)(regvals),
				*(signed char *)(regvals + 1),
				regvals[2], regvals[3],
				*(signed char *)(regvals + 5),
				*(signed char *)(regvals + 7));
	return ret;
}
=======
static ssize_t tempsense_show(struct device *device,
			      struct device_attribute *attr, char *buf)
{
	struct qib_ibdev *dev =
		rdma_device_to_drv_device(device, struct qib_ibdev, rdi.ibdev);
	struct qib_devdata *dd = dd_from_dev(dev);
	int i;
	u8 regvals[8];

	for (i = 0; i < 8; i++) {
		int ret;

		if (i == 6)
			continue;
		ret = dd->f_tempsense_rd(dd, i);
		if (ret < 0)
			return ret;	/* return error on bad read */
		regvals[i] = ret;
	}
	return sysfs_emit(buf, "%d %d %02X %02X %d %d\n",
			  (signed char)regvals[0],
			  (signed char)regvals[1],
			  regvals[2],
			  regvals[3],
			  (signed char)regvals[5],
			  (signed char)regvals[7]);
}
static DEVICE_ATTR_RO(tempsense);
>>>>>>> upstream/android-13

/*
 * end of per-unit (or driver, in some cases, but replicated
 * per unit) functions
 */

/* start of per-unit file structures and support code */
<<<<<<< HEAD
static DEVICE_ATTR(hw_rev, S_IRUGO, show_rev, NULL);
static DEVICE_ATTR(hca_type, S_IRUGO, show_hca, NULL);
static DEVICE_ATTR(board_id, S_IRUGO, show_hca, NULL);
static DEVICE_ATTR(version, S_IRUGO, show_version, NULL);
static DEVICE_ATTR(nctxts, S_IRUGO, show_nctxts, NULL);
static DEVICE_ATTR(nfreectxts, S_IRUGO, show_nfreectxts, NULL);
static DEVICE_ATTR(serial, S_IRUGO, show_serial, NULL);
static DEVICE_ATTR(boardversion, S_IRUGO, show_boardversion, NULL);
static DEVICE_ATTR(tempsense, S_IRUGO, show_tempsense, NULL);
static DEVICE_ATTR(localbus_info, S_IRUGO, show_localbus_info, NULL);
static DEVICE_ATTR(chip_reset, S_IWUSR, NULL, store_chip_reset);

static struct device_attribute *qib_attributes[] = {
	&dev_attr_hw_rev,
	&dev_attr_hca_type,
	&dev_attr_board_id,
	&dev_attr_version,
	&dev_attr_nctxts,
	&dev_attr_nfreectxts,
	&dev_attr_serial,
	&dev_attr_boardversion,
	&dev_attr_tempsense,
	&dev_attr_localbus_info,
	&dev_attr_chip_reset,
};

int qib_create_port_files(struct ib_device *ibdev, u8 port_num,
			  struct kobject *kobj)
{
	struct qib_pportdata *ppd;
	struct qib_devdata *dd = dd_from_ibdev(ibdev);
	int ret;

	if (!port_num || port_num > dd->num_pports) {
		qib_dev_err(dd,
			"Skipping infiniband class with invalid port %u\n",
			port_num);
		ret = -ENODEV;
		goto bail;
	}
	ppd = &dd->pport[port_num - 1];

	ret = kobject_init_and_add(&ppd->pport_kobj, &qib_port_ktype, kobj,
				   "linkcontrol");
	if (ret) {
		qib_dev_err(dd,
			"Skipping linkcontrol sysfs info, (err %d) port %u\n",
			ret, port_num);
		goto bail_link;
	}
	kobject_uevent(&ppd->pport_kobj, KOBJ_ADD);

	ret = kobject_init_and_add(&ppd->sl2vl_kobj, &qib_sl2vl_ktype, kobj,
				   "sl2vl");
	if (ret) {
		qib_dev_err(dd,
			"Skipping sl2vl sysfs info, (err %d) port %u\n",
			ret, port_num);
		goto bail_sl;
	}
	kobject_uevent(&ppd->sl2vl_kobj, KOBJ_ADD);

	ret = kobject_init_and_add(&ppd->diagc_kobj, &qib_diagc_ktype, kobj,
				   "diag_counters");
	if (ret) {
		qib_dev_err(dd,
			"Skipping diag_counters sysfs info, (err %d) port %u\n",
			ret, port_num);
		goto bail_diagc;
	}
	kobject_uevent(&ppd->diagc_kobj, KOBJ_ADD);

	if (!qib_cc_table_size || !ppd->congestion_entries_shadow)
		return 0;

	ret = kobject_init_and_add(&ppd->pport_cc_kobj, &qib_port_cc_ktype,
				kobj, "CCMgtA");
	if (ret) {
		qib_dev_err(dd,
		 "Skipping Congestion Control sysfs info, (err %d) port %u\n",
		 ret, port_num);
		goto bail_cc;
	}

	kobject_uevent(&ppd->pport_cc_kobj, KOBJ_ADD);

	ret = sysfs_create_bin_file(&ppd->pport_cc_kobj,
				&cc_setting_bin_attr);
	if (ret) {
		qib_dev_err(dd,
		 "Skipping Congestion Control setting sysfs info, (err %d) port %u\n",
		 ret, port_num);
		goto bail_cc;
	}

	ret = sysfs_create_bin_file(&ppd->pport_cc_kobj,
				&cc_table_bin_attr);
	if (ret) {
		qib_dev_err(dd,
		 "Skipping Congestion Control table sysfs info, (err %d) port %u\n",
		 ret, port_num);
		goto bail_cc_entry_bin;
	}

	qib_devinfo(dd->pcidev,
		"IB%u: Congestion Control Agent enabled for port %d\n",
		dd->unit, port_num);

	return 0;

bail_cc_entry_bin:
	sysfs_remove_bin_file(&ppd->pport_cc_kobj, &cc_setting_bin_attr);
bail_cc:
	kobject_put(&ppd->pport_cc_kobj);
bail_diagc:
	kobject_put(&ppd->diagc_kobj);
bail_sl:
	kobject_put(&ppd->sl2vl_kobj);
bail_link:
	kobject_put(&ppd->pport_kobj);
bail:
	return ret;
}

/*
 * Register and create our files in /sys/class/infiniband.
 */
int qib_verbs_register_sysfs(struct qib_devdata *dd)
{
	struct ib_device *dev = &dd->verbs_dev.rdi.ibdev;
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(qib_attributes); ++i) {
		ret = device_create_file(&dev->dev, qib_attributes[i]);
		if (ret)
			goto bail;
	}

	return 0;
bail:
	for (i = 0; i < ARRAY_SIZE(qib_attributes); ++i)
		device_remove_file(&dev->dev, qib_attributes[i]);
	return ret;
}

/*
 * Unregister and remove our files in /sys/class/infiniband.
 */
void qib_verbs_unregister_sysfs(struct qib_devdata *dd)
{
	struct qib_pportdata *ppd;
	int i;

	for (i = 0; i < dd->num_pports; i++) {
		ppd = &dd->pport[i];
		if (qib_cc_table_size &&
			ppd->congestion_entries_shadow) {
			sysfs_remove_bin_file(&ppd->pport_cc_kobj,
				&cc_setting_bin_attr);
			sysfs_remove_bin_file(&ppd->pport_cc_kobj,
				&cc_table_bin_attr);
			kobject_put(&ppd->pport_cc_kobj);
		}
		kobject_put(&ppd->diagc_kobj);
		kobject_put(&ppd->sl2vl_kobj);
		kobject_put(&ppd->pport_kobj);
	}
}
=======
static struct attribute *qib_attributes[] = {
	&dev_attr_hw_rev.attr,
	&dev_attr_hca_type.attr,
	&dev_attr_board_id.attr,
	&dev_attr_version.attr,
	&dev_attr_nctxts.attr,
	&dev_attr_nfreectxts.attr,
	&dev_attr_serial.attr,
	&dev_attr_boardversion.attr,
	&dev_attr_tempsense.attr,
	&dev_attr_localbus_info.attr,
	&dev_attr_chip_reset.attr,
	NULL,
};

const struct attribute_group qib_attr_group = {
	.attrs = qib_attributes,
};
>>>>>>> upstream/android-13
