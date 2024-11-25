// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright IBM Corp. 2016
 * Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>
 *
 * Adjunct processor bus, card related code.
 */

#define KMSG_COMPONENT "ap"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/init.h>
#include <linux/slab.h>
#include <asm/facility.h>
<<<<<<< HEAD
=======
#include <asm/sclp.h>
>>>>>>> upstream/android-13

#include "ap_bus.h"

/*
 * AP card related attributes.
 */
static ssize_t hwtype_show(struct device *dev,
			   struct device_attribute *attr, char *buf)
{
	struct ap_card *ac = to_ap_card(dev);

<<<<<<< HEAD
	return snprintf(buf, PAGE_SIZE, "%d\n", ac->ap_dev.device_type);
=======
	return scnprintf(buf, PAGE_SIZE, "%d\n", ac->ap_dev.device_type);
>>>>>>> upstream/android-13
}

static DEVICE_ATTR_RO(hwtype);

static ssize_t raw_hwtype_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct ap_card *ac = to_ap_card(dev);

<<<<<<< HEAD
	return snprintf(buf, PAGE_SIZE, "%d\n", ac->raw_hwtype);
=======
	return scnprintf(buf, PAGE_SIZE, "%d\n", ac->raw_hwtype);
>>>>>>> upstream/android-13
}

static DEVICE_ATTR_RO(raw_hwtype);

static ssize_t depth_show(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct ap_card *ac = to_ap_card(dev);

<<<<<<< HEAD
	return snprintf(buf, PAGE_SIZE, "%d\n", ac->queue_depth);
=======
	return scnprintf(buf, PAGE_SIZE, "%d\n", ac->queue_depth);
>>>>>>> upstream/android-13
}

static DEVICE_ATTR_RO(depth);

static ssize_t ap_functions_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct ap_card *ac = to_ap_card(dev);

<<<<<<< HEAD
	return snprintf(buf, PAGE_SIZE, "0x%08X\n", ac->functions);
=======
	return scnprintf(buf, PAGE_SIZE, "0x%08X\n", ac->functions);
>>>>>>> upstream/android-13
}

static DEVICE_ATTR_RO(ap_functions);

static ssize_t request_count_show(struct device *dev,
				  struct device_attribute *attr,
				  char *buf)
{
	struct ap_card *ac = to_ap_card(dev);
	u64 req_cnt;

	req_cnt = 0;
<<<<<<< HEAD
	spin_lock_bh(&ap_list_lock);
	req_cnt = atomic64_read(&ac->total_request_count);
	spin_unlock_bh(&ap_list_lock);
	return snprintf(buf, PAGE_SIZE, "%llu\n", req_cnt);
=======
	spin_lock_bh(&ap_queues_lock);
	req_cnt = atomic64_read(&ac->total_request_count);
	spin_unlock_bh(&ap_queues_lock);
	return scnprintf(buf, PAGE_SIZE, "%llu\n", req_cnt);
>>>>>>> upstream/android-13
}

static ssize_t request_count_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
<<<<<<< HEAD
	struct ap_card *ac = to_ap_card(dev);
	struct ap_queue *aq;

	spin_lock_bh(&ap_list_lock);
	for_each_ap_queue(aq, ac)
		aq->total_request_count = 0;
	spin_unlock_bh(&ap_list_lock);
=======
	int bkt;
	struct ap_queue *aq;
	struct ap_card *ac = to_ap_card(dev);

	spin_lock_bh(&ap_queues_lock);
	hash_for_each(ap_queues, bkt, aq, hnode)
		if (ac == aq->card)
			aq->total_request_count = 0;
	spin_unlock_bh(&ap_queues_lock);
>>>>>>> upstream/android-13
	atomic64_set(&ac->total_request_count, 0);

	return count;
}

static DEVICE_ATTR_RW(request_count);

static ssize_t requestq_count_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct ap_card *ac = to_ap_card(dev);
	struct ap_queue *aq;
	unsigned int reqq_cnt;

	reqq_cnt = 0;
	spin_lock_bh(&ap_list_lock);
	for_each_ap_queue(aq, ac)
		reqq_cnt += aq->requestq_count;
	spin_unlock_bh(&ap_list_lock);
	return snprintf(buf, PAGE_SIZE, "%d\n", reqq_cnt);
=======
	int bkt;
	struct ap_queue *aq;
	unsigned int reqq_cnt;
	struct ap_card *ac = to_ap_card(dev);

	reqq_cnt = 0;
	spin_lock_bh(&ap_queues_lock);
	hash_for_each(ap_queues, bkt, aq, hnode)
		if (ac == aq->card)
			reqq_cnt += aq->requestq_count;
	spin_unlock_bh(&ap_queues_lock);
	return scnprintf(buf, PAGE_SIZE, "%d\n", reqq_cnt);
>>>>>>> upstream/android-13
}

static DEVICE_ATTR_RO(requestq_count);

static ssize_t pendingq_count_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct ap_card *ac = to_ap_card(dev);
	struct ap_queue *aq;
	unsigned int penq_cnt;

	penq_cnt = 0;
	spin_lock_bh(&ap_list_lock);
	for_each_ap_queue(aq, ac)
		penq_cnt += aq->pendingq_count;
	spin_unlock_bh(&ap_list_lock);
	return snprintf(buf, PAGE_SIZE, "%d\n", penq_cnt);
=======
	int bkt;
	struct ap_queue *aq;
	unsigned int penq_cnt;
	struct ap_card *ac = to_ap_card(dev);

	penq_cnt = 0;
	spin_lock_bh(&ap_queues_lock);
	hash_for_each(ap_queues, bkt, aq, hnode)
		if (ac == aq->card)
			penq_cnt += aq->pendingq_count;
	spin_unlock_bh(&ap_queues_lock);
	return scnprintf(buf, PAGE_SIZE, "%d\n", penq_cnt);
>>>>>>> upstream/android-13
}

static DEVICE_ATTR_RO(pendingq_count);

static ssize_t modalias_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "ap:t%02X\n", to_ap_dev(dev)->device_type);
=======
	return scnprintf(buf, PAGE_SIZE, "ap:t%02X\n",
			 to_ap_dev(dev)->device_type);
>>>>>>> upstream/android-13
}

static DEVICE_ATTR_RO(modalias);

<<<<<<< HEAD
=======
static ssize_t config_show(struct device *dev,
			   struct device_attribute *attr, char *buf)
{
	struct ap_card *ac = to_ap_card(dev);

	return scnprintf(buf, PAGE_SIZE, "%d\n", ac->config ? 1 : 0);
}

static ssize_t config_store(struct device *dev,
			    struct device_attribute *attr,
			    const char *buf, size_t count)
{
	int rc = 0, cfg;
	struct ap_card *ac = to_ap_card(dev);

	if (sscanf(buf, "%d\n", &cfg) != 1 || cfg < 0 || cfg > 1)
		return -EINVAL;

	if (cfg && !ac->config)
		rc = sclp_ap_configure(ac->id);
	else if (!cfg && ac->config)
		rc = sclp_ap_deconfigure(ac->id);
	if (rc)
		return rc;

	ac->config = cfg ? true : false;

	ap_send_config_uevent(&ac->ap_dev, ac->config);

	return count;
}

static DEVICE_ATTR_RW(config);

static ssize_t max_msg_size_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct ap_card *ac = to_ap_card(dev);

	return scnprintf(buf, PAGE_SIZE, "%u\n", ac->maxmsgsize);
}

static DEVICE_ATTR_RO(max_msg_size);

>>>>>>> upstream/android-13
static struct attribute *ap_card_dev_attrs[] = {
	&dev_attr_hwtype.attr,
	&dev_attr_raw_hwtype.attr,
	&dev_attr_depth.attr,
	&dev_attr_ap_functions.attr,
	&dev_attr_request_count.attr,
	&dev_attr_requestq_count.attr,
	&dev_attr_pendingq_count.attr,
	&dev_attr_modalias.attr,
<<<<<<< HEAD
=======
	&dev_attr_config.attr,
	&dev_attr_max_msg_size.attr,
>>>>>>> upstream/android-13
	NULL
};

static struct attribute_group ap_card_dev_attr_group = {
	.attrs = ap_card_dev_attrs
};

static const struct attribute_group *ap_card_dev_attr_groups[] = {
	&ap_card_dev_attr_group,
	NULL
};

static struct device_type ap_card_type = {
	.name = "ap_card",
	.groups = ap_card_dev_attr_groups,
};

static void ap_card_device_release(struct device *dev)
{
	struct ap_card *ac = to_ap_card(dev);

<<<<<<< HEAD
	if (!list_empty(&ac->list)) {
		spin_lock_bh(&ap_list_lock);
		list_del_init(&ac->list);
		spin_unlock_bh(&ap_list_lock);
	}
=======
>>>>>>> upstream/android-13
	kfree(ac);
}

struct ap_card *ap_card_create(int id, int queue_depth, int raw_type,
<<<<<<< HEAD
			       int comp_type, unsigned int functions)
=======
			       int comp_type, unsigned int functions, int ml)
>>>>>>> upstream/android-13
{
	struct ap_card *ac;

	ac = kzalloc(sizeof(*ac), GFP_KERNEL);
	if (!ac)
		return NULL;
<<<<<<< HEAD
	INIT_LIST_HEAD(&ac->list);
	INIT_LIST_HEAD(&ac->queues);
=======
>>>>>>> upstream/android-13
	ac->ap_dev.device.release = ap_card_device_release;
	ac->ap_dev.device.type = &ap_card_type;
	ac->ap_dev.device_type = comp_type;
	ac->raw_hwtype = raw_type;
	ac->queue_depth = queue_depth;
	ac->functions = functions;
	ac->id = id;
<<<<<<< HEAD
=======
	ac->maxmsgsize = ml > 0 ?
		ml * AP_TAPQ_ML_FIELD_CHUNK_SIZE : AP_DEFAULT_MAX_MSG_SIZE;

>>>>>>> upstream/android-13
	return ac;
}
