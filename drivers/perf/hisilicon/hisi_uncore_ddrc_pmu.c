<<<<<<< HEAD
/*
 * HiSilicon SoC DDRC uncore Hardware event counters support
 *
 * Copyright (C) 2017 Hisilicon Limited
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * HiSilicon SoC DDRC uncore Hardware event counters support
 *
 * Copyright (C) 2017 HiSilicon Limited
>>>>>>> upstream/android-13
 * Author: Shaokun Zhang <zhangshaokun@hisilicon.com>
 *         Anurup M <anurup.m@huawei.com>
 *
 * This code is based on the uncore PMUs like arm-cci and arm-ccn.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#include <linux/acpi.h>
#include <linux/bug.h>
#include <linux/cpuhotplug.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/list.h>
<<<<<<< HEAD
#include <linux/platform_device.h>
=======
>>>>>>> upstream/android-13
#include <linux/smp.h>

#include "hisi_uncore_pmu.h"

<<<<<<< HEAD
/* DDRC register definition */
=======
/* DDRC register definition in v1 */
>>>>>>> upstream/android-13
#define DDRC_PERF_CTRL		0x010
#define DDRC_FLUX_WR		0x380
#define DDRC_FLUX_RD		0x384
#define DDRC_FLUX_WCMD          0x388
#define DDRC_FLUX_RCMD          0x38c
#define DDRC_PRE_CMD            0x3c0
#define DDRC_ACT_CMD            0x3c4
#define DDRC_RNK_CHG            0x3cc
#define DDRC_RW_CHG             0x3d0
#define DDRC_EVENT_CTRL         0x6C0
#define DDRC_INT_MASK		0x6c8
#define DDRC_INT_STATUS		0x6cc
#define DDRC_INT_CLEAR		0x6d0
<<<<<<< HEAD

/* DDRC has 8-counters */
#define DDRC_NR_COUNTERS	0x8
#define DDRC_PERF_CTRL_EN	0x2

/*
 * For DDRC PMU, there are eight-events and every event has been mapped
=======
#define DDRC_VERSION		0x710

/* DDRC register definition in v2 */
#define DDRC_V2_INT_MASK	0x528
#define DDRC_V2_INT_STATUS	0x52c
#define DDRC_V2_INT_CLEAR	0x530
#define DDRC_V2_EVENT_CNT	0xe00
#define DDRC_V2_EVENT_CTRL	0xe70
#define DDRC_V2_EVENT_TYPE	0xe74
#define DDRC_V2_PERF_CTRL	0xeA0

/* DDRC has 8-counters */
#define DDRC_NR_COUNTERS	0x8
#define DDRC_V1_PERF_CTRL_EN	0x2
#define DDRC_V2_PERF_CTRL_EN	0x1
#define DDRC_V1_NR_EVENTS	0x7
#define DDRC_V2_NR_EVENTS	0x90

/*
 * For PMU v1, there are eight-events and every event has been mapped
>>>>>>> upstream/android-13
 * to fixed-purpose counters which register offset is not consistent.
 * Therefore there is no write event type and we assume that event
 * code (0 to 7) is equal to counter index in PMU driver.
 */
#define GET_DDRC_EVENTID(hwc)	(hwc->config_base & 0x7)

static const u32 ddrc_reg_off[] = {
	DDRC_FLUX_WR, DDRC_FLUX_RD, DDRC_FLUX_WCMD, DDRC_FLUX_RCMD,
	DDRC_PRE_CMD, DDRC_ACT_CMD, DDRC_RNK_CHG, DDRC_RW_CHG
};

/*
 * Select the counter register offset using the counter index.
<<<<<<< HEAD
 * In DDRC there are no programmable counter, the count
 * is readed form the statistics counter register itself.
 */
static u32 hisi_ddrc_pmu_get_counter_offset(int cntr_idx)
=======
 * In PMU v1, there are no programmable counter, the count
 * is read form the statistics counter register itself.
 */
static u32 hisi_ddrc_pmu_v1_get_counter_offset(int cntr_idx)
>>>>>>> upstream/android-13
{
	return ddrc_reg_off[cntr_idx];
}

<<<<<<< HEAD
static u64 hisi_ddrc_pmu_read_counter(struct hisi_pmu *ddrc_pmu,
				      struct hw_perf_event *hwc)
{
	/* Use event code as counter index */
	u32 idx = GET_DDRC_EVENTID(hwc);

	if (!hisi_uncore_pmu_counter_valid(ddrc_pmu, idx)) {
		dev_err(ddrc_pmu->dev, "Unsupported event index:%d!\n", idx);
		return 0;
	}

	return readl(ddrc_pmu->base + hisi_ddrc_pmu_get_counter_offset(idx));
}

static void hisi_ddrc_pmu_write_counter(struct hisi_pmu *ddrc_pmu,
					struct hw_perf_event *hwc, u64 val)
{
	u32 idx = GET_DDRC_EVENTID(hwc);

	if (!hisi_uncore_pmu_counter_valid(ddrc_pmu, idx)) {
		dev_err(ddrc_pmu->dev, "Unsupported event index:%d!\n", idx);
		return;
	}

	writel((u32)val,
	       ddrc_pmu->base + hisi_ddrc_pmu_get_counter_offset(idx));
}

/*
 * For DDRC PMU, event has been mapped to fixed-purpose counter by hardware,
 * so there is no need to write event type.
=======
static u32 hisi_ddrc_pmu_v2_get_counter_offset(int cntr_idx)
{
	return DDRC_V2_EVENT_CNT + cntr_idx * 8;
}

static u64 hisi_ddrc_pmu_v1_read_counter(struct hisi_pmu *ddrc_pmu,
				      struct hw_perf_event *hwc)
{
	return readl(ddrc_pmu->base +
		     hisi_ddrc_pmu_v1_get_counter_offset(hwc->idx));
}

static void hisi_ddrc_pmu_v1_write_counter(struct hisi_pmu *ddrc_pmu,
					struct hw_perf_event *hwc, u64 val)
{
	writel((u32)val,
	       ddrc_pmu->base + hisi_ddrc_pmu_v1_get_counter_offset(hwc->idx));
}

static u64 hisi_ddrc_pmu_v2_read_counter(struct hisi_pmu *ddrc_pmu,
					 struct hw_perf_event *hwc)
{
	return readq(ddrc_pmu->base +
		     hisi_ddrc_pmu_v2_get_counter_offset(hwc->idx));
}

static void hisi_ddrc_pmu_v2_write_counter(struct hisi_pmu *ddrc_pmu,
					   struct hw_perf_event *hwc, u64 val)
{
	writeq(val,
	       ddrc_pmu->base + hisi_ddrc_pmu_v2_get_counter_offset(hwc->idx));
}

/*
 * For DDRC PMU v1, event has been mapped to fixed-purpose counter by hardware,
 * so there is no need to write event type, while it is programmable counter in
 * PMU v2.
>>>>>>> upstream/android-13
 */
static void hisi_ddrc_pmu_write_evtype(struct hisi_pmu *hha_pmu, int idx,
				       u32 type)
{
<<<<<<< HEAD
}

static void hisi_ddrc_pmu_start_counters(struct hisi_pmu *ddrc_pmu)
=======
	u32 offset;

	if (hha_pmu->identifier >= HISI_PMU_V2) {
		offset = DDRC_V2_EVENT_TYPE + 4 * idx;
		writel(type, hha_pmu->base + offset);
	}
}

static void hisi_ddrc_pmu_v1_start_counters(struct hisi_pmu *ddrc_pmu)
>>>>>>> upstream/android-13
{
	u32 val;

	/* Set perf_enable in DDRC_PERF_CTRL to start event counting */
	val = readl(ddrc_pmu->base + DDRC_PERF_CTRL);
<<<<<<< HEAD
	val |= DDRC_PERF_CTRL_EN;
	writel(val, ddrc_pmu->base + DDRC_PERF_CTRL);
}

static void hisi_ddrc_pmu_stop_counters(struct hisi_pmu *ddrc_pmu)
=======
	val |= DDRC_V1_PERF_CTRL_EN;
	writel(val, ddrc_pmu->base + DDRC_PERF_CTRL);
}

static void hisi_ddrc_pmu_v1_stop_counters(struct hisi_pmu *ddrc_pmu)
>>>>>>> upstream/android-13
{
	u32 val;

	/* Clear perf_enable in DDRC_PERF_CTRL to stop event counting */
	val = readl(ddrc_pmu->base + DDRC_PERF_CTRL);
<<<<<<< HEAD
	val &= ~DDRC_PERF_CTRL_EN;
	writel(val, ddrc_pmu->base + DDRC_PERF_CTRL);
}

static void hisi_ddrc_pmu_enable_counter(struct hisi_pmu *ddrc_pmu,
					 struct hw_perf_event *hwc)
=======
	val &= ~DDRC_V1_PERF_CTRL_EN;
	writel(val, ddrc_pmu->base + DDRC_PERF_CTRL);
}

static void hisi_ddrc_pmu_v1_enable_counter(struct hisi_pmu *ddrc_pmu,
					    struct hw_perf_event *hwc)
>>>>>>> upstream/android-13
{
	u32 val;

	/* Set counter index(event code) in DDRC_EVENT_CTRL register */
	val = readl(ddrc_pmu->base + DDRC_EVENT_CTRL);
	val |= (1 << GET_DDRC_EVENTID(hwc));
	writel(val, ddrc_pmu->base + DDRC_EVENT_CTRL);
}

<<<<<<< HEAD
static void hisi_ddrc_pmu_disable_counter(struct hisi_pmu *ddrc_pmu,
					  struct hw_perf_event *hwc)
=======
static void hisi_ddrc_pmu_v1_disable_counter(struct hisi_pmu *ddrc_pmu,
					     struct hw_perf_event *hwc)
>>>>>>> upstream/android-13
{
	u32 val;

	/* Clear counter index(event code) in DDRC_EVENT_CTRL register */
	val = readl(ddrc_pmu->base + DDRC_EVENT_CTRL);
	val &= ~(1 << GET_DDRC_EVENTID(hwc));
	writel(val, ddrc_pmu->base + DDRC_EVENT_CTRL);
}

<<<<<<< HEAD
static int hisi_ddrc_pmu_get_event_idx(struct perf_event *event)
=======
static int hisi_ddrc_pmu_v1_get_event_idx(struct perf_event *event)
>>>>>>> upstream/android-13
{
	struct hisi_pmu *ddrc_pmu = to_hisi_pmu(event->pmu);
	unsigned long *used_mask = ddrc_pmu->pmu_events.used_mask;
	struct hw_perf_event *hwc = &event->hw;
	/* For DDRC PMU, we use event code as counter index */
	int idx = GET_DDRC_EVENTID(hwc);

	if (test_bit(idx, used_mask))
		return -EAGAIN;

	set_bit(idx, used_mask);

	return idx;
}

<<<<<<< HEAD
static void hisi_ddrc_pmu_enable_counter_int(struct hisi_pmu *ddrc_pmu,
=======
static int hisi_ddrc_pmu_v2_get_event_idx(struct perf_event *event)
{
	return hisi_uncore_pmu_get_event_idx(event);
}

static void hisi_ddrc_pmu_v2_start_counters(struct hisi_pmu *ddrc_pmu)
{
	u32 val;

	val = readl(ddrc_pmu->base + DDRC_V2_PERF_CTRL);
	val |= DDRC_V2_PERF_CTRL_EN;
	writel(val, ddrc_pmu->base + DDRC_V2_PERF_CTRL);
}

static void hisi_ddrc_pmu_v2_stop_counters(struct hisi_pmu *ddrc_pmu)
{
	u32 val;

	val = readl(ddrc_pmu->base + DDRC_V2_PERF_CTRL);
	val &= ~DDRC_V2_PERF_CTRL_EN;
	writel(val, ddrc_pmu->base + DDRC_V2_PERF_CTRL);
}

static void hisi_ddrc_pmu_v2_enable_counter(struct hisi_pmu *ddrc_pmu,
					    struct hw_perf_event *hwc)
{
	u32 val;

	val = readl(ddrc_pmu->base + DDRC_V2_EVENT_CTRL);
	val |= 1 << hwc->idx;
	writel(val, ddrc_pmu->base + DDRC_V2_EVENT_CTRL);
}

static void hisi_ddrc_pmu_v2_disable_counter(struct hisi_pmu *ddrc_pmu,
>>>>>>> upstream/android-13
					     struct hw_perf_event *hwc)
{
	u32 val;

<<<<<<< HEAD
	/* Write 0 to enable interrupt */
	val = readl(ddrc_pmu->base + DDRC_INT_MASK);
	val &= ~(1 << GET_DDRC_EVENTID(hwc));
	writel(val, ddrc_pmu->base + DDRC_INT_MASK);
}

static void hisi_ddrc_pmu_disable_counter_int(struct hisi_pmu *ddrc_pmu,
					      struct hw_perf_event *hwc)
=======
	val = readl(ddrc_pmu->base + DDRC_V2_EVENT_CTRL);
	val &= ~(1 << hwc->idx);
	writel(val, ddrc_pmu->base + DDRC_V2_EVENT_CTRL);
}

static void hisi_ddrc_pmu_v1_enable_counter_int(struct hisi_pmu *ddrc_pmu,
						struct hw_perf_event *hwc)
{
	u32 val;

	/* Write 0 to enable interrupt */
	val = readl(ddrc_pmu->base + DDRC_INT_MASK);
	val &= ~(1 << hwc->idx);
	writel(val, ddrc_pmu->base + DDRC_INT_MASK);
}

static void hisi_ddrc_pmu_v1_disable_counter_int(struct hisi_pmu *ddrc_pmu,
						 struct hw_perf_event *hwc)
>>>>>>> upstream/android-13
{
	u32 val;

	/* Write 1 to mask interrupt */
	val = readl(ddrc_pmu->base + DDRC_INT_MASK);
<<<<<<< HEAD
	val |= (1 << GET_DDRC_EVENTID(hwc));
	writel(val, ddrc_pmu->base + DDRC_INT_MASK);
}

static irqreturn_t hisi_ddrc_pmu_isr(int irq, void *dev_id)
{
	struct hisi_pmu *ddrc_pmu = dev_id;
	struct perf_event *event;
	unsigned long overflown;
	int idx;

	/* Read the DDRC_INT_STATUS register */
	overflown = readl(ddrc_pmu->base + DDRC_INT_STATUS);
	if (!overflown)
		return IRQ_NONE;

	/*
	 * Find the counter index which overflowed if the bit was set
	 * and handle it
	 */
	for_each_set_bit(idx, &overflown, DDRC_NR_COUNTERS) {
		/* Write 1 to clear the IRQ status flag */
		writel((1 << idx), ddrc_pmu->base + DDRC_INT_CLEAR);

		/* Get the corresponding event struct */
		event = ddrc_pmu->pmu_events.hw_events[idx];
		if (!event)
			continue;

		hisi_uncore_pmu_event_update(event);
		hisi_uncore_pmu_set_event_period(event);
	}

	return IRQ_HANDLED;
}

static int hisi_ddrc_pmu_init_irq(struct hisi_pmu *ddrc_pmu,
				  struct platform_device *pdev)
{
	int irq, ret;

	/* Read and init IRQ */
	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "DDRC PMU get irq fail; irq:%d\n", irq);
		return irq;
	}

	ret = devm_request_irq(&pdev->dev, irq, hisi_ddrc_pmu_isr,
			       IRQF_NOBALANCING | IRQF_NO_THREAD,
			       dev_name(&pdev->dev), ddrc_pmu);
	if (ret < 0) {
		dev_err(&pdev->dev,
			"Fail to request IRQ:%d ret:%d\n", irq, ret);
		return ret;
	}

	ddrc_pmu->irq = irq;

	return 0;
=======
	val |= 1 << hwc->idx;
	writel(val, ddrc_pmu->base + DDRC_INT_MASK);
}

static void hisi_ddrc_pmu_v2_enable_counter_int(struct hisi_pmu *ddrc_pmu,
						struct hw_perf_event *hwc)
{
	u32 val;

	val = readl(ddrc_pmu->base + DDRC_V2_INT_MASK);
	val &= ~(1 << hwc->idx);
	writel(val, ddrc_pmu->base + DDRC_V2_INT_MASK);
}

static void hisi_ddrc_pmu_v2_disable_counter_int(struct hisi_pmu *ddrc_pmu,
						struct hw_perf_event *hwc)
{
	u32 val;

	val = readl(ddrc_pmu->base + DDRC_V2_INT_MASK);
	val |= 1 << hwc->idx;
	writel(val, ddrc_pmu->base + DDRC_V2_INT_MASK);
}

static u32 hisi_ddrc_pmu_v1_get_int_status(struct hisi_pmu *ddrc_pmu)
{
	return readl(ddrc_pmu->base + DDRC_INT_STATUS);
}

static void hisi_ddrc_pmu_v1_clear_int_status(struct hisi_pmu *ddrc_pmu,
					      int idx)
{
	writel(1 << idx, ddrc_pmu->base + DDRC_INT_CLEAR);
}

static u32 hisi_ddrc_pmu_v2_get_int_status(struct hisi_pmu *ddrc_pmu)
{
	return readl(ddrc_pmu->base + DDRC_V2_INT_STATUS);
}

static void hisi_ddrc_pmu_v2_clear_int_status(struct hisi_pmu *ddrc_pmu,
					      int idx)
{
	writel(1 << idx, ddrc_pmu->base + DDRC_V2_INT_CLEAR);
>>>>>>> upstream/android-13
}

static const struct acpi_device_id hisi_ddrc_pmu_acpi_match[] = {
	{ "HISI0233", },
<<<<<<< HEAD
	{},
=======
	{ "HISI0234", },
	{}
>>>>>>> upstream/android-13
};
MODULE_DEVICE_TABLE(acpi, hisi_ddrc_pmu_acpi_match);

static int hisi_ddrc_pmu_init_data(struct platform_device *pdev,
				   struct hisi_pmu *ddrc_pmu)
{
<<<<<<< HEAD
	struct resource *res;

=======
>>>>>>> upstream/android-13
	/*
	 * Use the SCCL_ID and DDRC channel ID to identify the
	 * DDRC PMU, while SCCL_ID is in MPIDR[aff2].
	 */
	if (device_property_read_u32(&pdev->dev, "hisilicon,ch-id",
				     &ddrc_pmu->index_id)) {
		dev_err(&pdev->dev, "Can not read ddrc channel-id!\n");
		return -EINVAL;
	}

	if (device_property_read_u32(&pdev->dev, "hisilicon,scl-id",
				     &ddrc_pmu->sccl_id)) {
		dev_err(&pdev->dev, "Can not read ddrc sccl-id!\n");
		return -EINVAL;
	}
	/* DDRC PMUs only share the same SCCL */
	ddrc_pmu->ccl_id = -1;

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	ddrc_pmu->base = devm_ioremap_resource(&pdev->dev, res);
=======
	ddrc_pmu->base = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(ddrc_pmu->base)) {
		dev_err(&pdev->dev, "ioremap failed for ddrc_pmu resource\n");
		return PTR_ERR(ddrc_pmu->base);
	}

<<<<<<< HEAD
	return 0;
}

static struct attribute *hisi_ddrc_pmu_format_attr[] = {
=======
	ddrc_pmu->identifier = readl(ddrc_pmu->base + DDRC_VERSION);
	if (ddrc_pmu->identifier >= HISI_PMU_V2) {
		if (device_property_read_u32(&pdev->dev, "hisilicon,sub-id",
					     &ddrc_pmu->sub_id)) {
			dev_err(&pdev->dev, "Can not read sub-id!\n");
			return -EINVAL;
		}
	}

	return 0;
}

static struct attribute *hisi_ddrc_pmu_v1_format_attr[] = {
>>>>>>> upstream/android-13
	HISI_PMU_FORMAT_ATTR(event, "config:0-4"),
	NULL,
};

<<<<<<< HEAD
static const struct attribute_group hisi_ddrc_pmu_format_group = {
	.name = "format",
	.attrs = hisi_ddrc_pmu_format_attr,
};

static struct attribute *hisi_ddrc_pmu_events_attr[] = {
=======
static const struct attribute_group hisi_ddrc_pmu_v1_format_group = {
	.name = "format",
	.attrs = hisi_ddrc_pmu_v1_format_attr,
};

static struct attribute *hisi_ddrc_pmu_v2_format_attr[] = {
	HISI_PMU_FORMAT_ATTR(event, "config:0-7"),
	NULL
};

static const struct attribute_group hisi_ddrc_pmu_v2_format_group = {
	.name = "format",
	.attrs = hisi_ddrc_pmu_v2_format_attr,
};

static struct attribute *hisi_ddrc_pmu_v1_events_attr[] = {
>>>>>>> upstream/android-13
	HISI_PMU_EVENT_ATTR(flux_wr,		0x00),
	HISI_PMU_EVENT_ATTR(flux_rd,		0x01),
	HISI_PMU_EVENT_ATTR(flux_wcmd,		0x02),
	HISI_PMU_EVENT_ATTR(flux_rcmd,		0x03),
	HISI_PMU_EVENT_ATTR(pre_cmd,		0x04),
	HISI_PMU_EVENT_ATTR(act_cmd,		0x05),
	HISI_PMU_EVENT_ATTR(rnk_chg,		0x06),
	HISI_PMU_EVENT_ATTR(rw_chg,		0x07),
	NULL,
};

<<<<<<< HEAD
static const struct attribute_group hisi_ddrc_pmu_events_group = {
	.name = "events",
	.attrs = hisi_ddrc_pmu_events_attr,
=======
static const struct attribute_group hisi_ddrc_pmu_v1_events_group = {
	.name = "events",
	.attrs = hisi_ddrc_pmu_v1_events_attr,
};

static struct attribute *hisi_ddrc_pmu_v2_events_attr[] = {
	HISI_PMU_EVENT_ATTR(cycles,		0x00),
	HISI_PMU_EVENT_ATTR(flux_wr,		0x83),
	HISI_PMU_EVENT_ATTR(flux_rd,		0x84),
	NULL
};

static const struct attribute_group hisi_ddrc_pmu_v2_events_group = {
	.name = "events",
	.attrs = hisi_ddrc_pmu_v2_events_attr,
>>>>>>> upstream/android-13
};

static DEVICE_ATTR(cpumask, 0444, hisi_cpumask_sysfs_show, NULL);

static struct attribute *hisi_ddrc_pmu_cpumask_attrs[] = {
	&dev_attr_cpumask.attr,
	NULL,
};

static const struct attribute_group hisi_ddrc_pmu_cpumask_attr_group = {
	.attrs = hisi_ddrc_pmu_cpumask_attrs,
};

<<<<<<< HEAD
static const struct attribute_group *hisi_ddrc_pmu_attr_groups[] = {
	&hisi_ddrc_pmu_format_group,
	&hisi_ddrc_pmu_events_group,
	&hisi_ddrc_pmu_cpumask_attr_group,
	NULL,
};

static const struct hisi_uncore_ops hisi_uncore_ddrc_ops = {
	.write_evtype           = hisi_ddrc_pmu_write_evtype,
	.get_event_idx		= hisi_ddrc_pmu_get_event_idx,
	.start_counters		= hisi_ddrc_pmu_start_counters,
	.stop_counters		= hisi_ddrc_pmu_stop_counters,
	.enable_counter		= hisi_ddrc_pmu_enable_counter,
	.disable_counter	= hisi_ddrc_pmu_disable_counter,
	.enable_counter_int	= hisi_ddrc_pmu_enable_counter_int,
	.disable_counter_int	= hisi_ddrc_pmu_disable_counter_int,
	.write_counter		= hisi_ddrc_pmu_write_counter,
	.read_counter		= hisi_ddrc_pmu_read_counter,
=======
static struct device_attribute hisi_ddrc_pmu_identifier_attr =
	__ATTR(identifier, 0444, hisi_uncore_pmu_identifier_attr_show, NULL);

static struct attribute *hisi_ddrc_pmu_identifier_attrs[] = {
	&hisi_ddrc_pmu_identifier_attr.attr,
	NULL
};

static const struct attribute_group hisi_ddrc_pmu_identifier_group = {
	.attrs = hisi_ddrc_pmu_identifier_attrs,
};

static const struct attribute_group *hisi_ddrc_pmu_v1_attr_groups[] = {
	&hisi_ddrc_pmu_v1_format_group,
	&hisi_ddrc_pmu_v1_events_group,
	&hisi_ddrc_pmu_cpumask_attr_group,
	&hisi_ddrc_pmu_identifier_group,
	NULL,
};

static const struct attribute_group *hisi_ddrc_pmu_v2_attr_groups[] = {
	&hisi_ddrc_pmu_v2_format_group,
	&hisi_ddrc_pmu_v2_events_group,
	&hisi_ddrc_pmu_cpumask_attr_group,
	&hisi_ddrc_pmu_identifier_group,
	NULL
};

static const struct hisi_uncore_ops hisi_uncore_ddrc_v1_ops = {
	.write_evtype           = hisi_ddrc_pmu_write_evtype,
	.get_event_idx		= hisi_ddrc_pmu_v1_get_event_idx,
	.start_counters		= hisi_ddrc_pmu_v1_start_counters,
	.stop_counters		= hisi_ddrc_pmu_v1_stop_counters,
	.enable_counter		= hisi_ddrc_pmu_v1_enable_counter,
	.disable_counter	= hisi_ddrc_pmu_v1_disable_counter,
	.enable_counter_int	= hisi_ddrc_pmu_v1_enable_counter_int,
	.disable_counter_int	= hisi_ddrc_pmu_v1_disable_counter_int,
	.write_counter		= hisi_ddrc_pmu_v1_write_counter,
	.read_counter		= hisi_ddrc_pmu_v1_read_counter,
	.get_int_status		= hisi_ddrc_pmu_v1_get_int_status,
	.clear_int_status	= hisi_ddrc_pmu_v1_clear_int_status,
};

static const struct hisi_uncore_ops hisi_uncore_ddrc_v2_ops = {
	.write_evtype           = hisi_ddrc_pmu_write_evtype,
	.get_event_idx		= hisi_ddrc_pmu_v2_get_event_idx,
	.start_counters		= hisi_ddrc_pmu_v2_start_counters,
	.stop_counters		= hisi_ddrc_pmu_v2_stop_counters,
	.enable_counter		= hisi_ddrc_pmu_v2_enable_counter,
	.disable_counter	= hisi_ddrc_pmu_v2_disable_counter,
	.enable_counter_int	= hisi_ddrc_pmu_v2_enable_counter_int,
	.disable_counter_int	= hisi_ddrc_pmu_v2_disable_counter_int,
	.write_counter		= hisi_ddrc_pmu_v2_write_counter,
	.read_counter		= hisi_ddrc_pmu_v2_read_counter,
	.get_int_status		= hisi_ddrc_pmu_v2_get_int_status,
	.clear_int_status	= hisi_ddrc_pmu_v2_clear_int_status,
>>>>>>> upstream/android-13
};

static int hisi_ddrc_pmu_dev_probe(struct platform_device *pdev,
				   struct hisi_pmu *ddrc_pmu)
{
	int ret;

	ret = hisi_ddrc_pmu_init_data(pdev, ddrc_pmu);
	if (ret)
		return ret;

<<<<<<< HEAD
	ret = hisi_ddrc_pmu_init_irq(ddrc_pmu, pdev);
	if (ret)
		return ret;

	ddrc_pmu->num_counters = DDRC_NR_COUNTERS;
	ddrc_pmu->counter_bits = 32;
	ddrc_pmu->ops = &hisi_uncore_ddrc_ops;
	ddrc_pmu->dev = &pdev->dev;
	ddrc_pmu->on_cpu = -1;
	ddrc_pmu->check_event = 7;
=======
	ret = hisi_uncore_pmu_init_irq(ddrc_pmu, pdev);
	if (ret)
		return ret;

	if (ddrc_pmu->identifier >= HISI_PMU_V2) {
		ddrc_pmu->counter_bits = 48;
		ddrc_pmu->check_event = DDRC_V2_NR_EVENTS;
		ddrc_pmu->pmu_events.attr_groups = hisi_ddrc_pmu_v2_attr_groups;
		ddrc_pmu->ops = &hisi_uncore_ddrc_v2_ops;
	} else {
		ddrc_pmu->counter_bits = 32;
		ddrc_pmu->check_event = DDRC_V1_NR_EVENTS;
		ddrc_pmu->pmu_events.attr_groups = hisi_ddrc_pmu_v1_attr_groups;
		ddrc_pmu->ops = &hisi_uncore_ddrc_v1_ops;
	}

	ddrc_pmu->num_counters = DDRC_NR_COUNTERS;
	ddrc_pmu->dev = &pdev->dev;
	ddrc_pmu->on_cpu = -1;
>>>>>>> upstream/android-13

	return 0;
}

static int hisi_ddrc_pmu_probe(struct platform_device *pdev)
{
	struct hisi_pmu *ddrc_pmu;
	char *name;
	int ret;

	ddrc_pmu = devm_kzalloc(&pdev->dev, sizeof(*ddrc_pmu), GFP_KERNEL);
	if (!ddrc_pmu)
		return -ENOMEM;

	platform_set_drvdata(pdev, ddrc_pmu);

	ret = hisi_ddrc_pmu_dev_probe(pdev, ddrc_pmu);
	if (ret)
		return ret;

	ret = cpuhp_state_add_instance(CPUHP_AP_PERF_ARM_HISI_DDRC_ONLINE,
				       &ddrc_pmu->node);
	if (ret) {
		dev_err(&pdev->dev, "Error %d registering hotplug;\n", ret);
		return ret;
	}

<<<<<<< HEAD
	name = devm_kasprintf(&pdev->dev, GFP_KERNEL, "hisi_sccl%u_ddrc%u",
			      ddrc_pmu->sccl_id, ddrc_pmu->index_id);
	ddrc_pmu->pmu = (struct pmu) {
		.name		= name,
=======
	if (ddrc_pmu->identifier >= HISI_PMU_V2)
		name = devm_kasprintf(&pdev->dev, GFP_KERNEL,
				      "hisi_sccl%u_ddrc%u_%u",
				      ddrc_pmu->sccl_id, ddrc_pmu->index_id,
				      ddrc_pmu->sub_id);
	else
		name = devm_kasprintf(&pdev->dev, GFP_KERNEL,
				      "hisi_sccl%u_ddrc%u", ddrc_pmu->sccl_id,
				      ddrc_pmu->index_id);

	ddrc_pmu->pmu = (struct pmu) {
		.name		= name,
		.module		= THIS_MODULE,
>>>>>>> upstream/android-13
		.task_ctx_nr	= perf_invalid_context,
		.event_init	= hisi_uncore_pmu_event_init,
		.pmu_enable	= hisi_uncore_pmu_enable,
		.pmu_disable	= hisi_uncore_pmu_disable,
		.add		= hisi_uncore_pmu_add,
		.del		= hisi_uncore_pmu_del,
		.start		= hisi_uncore_pmu_start,
		.stop		= hisi_uncore_pmu_stop,
		.read		= hisi_uncore_pmu_read,
<<<<<<< HEAD
		.attr_groups	= hisi_ddrc_pmu_attr_groups,
=======
		.attr_groups	= ddrc_pmu->pmu_events.attr_groups,
		.capabilities	= PERF_PMU_CAP_NO_EXCLUDE,
>>>>>>> upstream/android-13
	};

	ret = perf_pmu_register(&ddrc_pmu->pmu, name, -1);
	if (ret) {
		dev_err(ddrc_pmu->dev, "DDRC PMU register failed!\n");
<<<<<<< HEAD
		cpuhp_state_remove_instance(CPUHP_AP_PERF_ARM_HISI_DDRC_ONLINE,
					    &ddrc_pmu->node);
=======
		cpuhp_state_remove_instance_nocalls(
			CPUHP_AP_PERF_ARM_HISI_DDRC_ONLINE, &ddrc_pmu->node);
>>>>>>> upstream/android-13
	}

	return ret;
}

static int hisi_ddrc_pmu_remove(struct platform_device *pdev)
{
	struct hisi_pmu *ddrc_pmu = platform_get_drvdata(pdev);

	perf_pmu_unregister(&ddrc_pmu->pmu);
<<<<<<< HEAD
	cpuhp_state_remove_instance(CPUHP_AP_PERF_ARM_HISI_DDRC_ONLINE,
				    &ddrc_pmu->node);

=======
	cpuhp_state_remove_instance_nocalls(CPUHP_AP_PERF_ARM_HISI_DDRC_ONLINE,
					    &ddrc_pmu->node);
>>>>>>> upstream/android-13
	return 0;
}

static struct platform_driver hisi_ddrc_pmu_driver = {
	.driver = {
		.name = "hisi_ddrc_pmu",
		.acpi_match_table = ACPI_PTR(hisi_ddrc_pmu_acpi_match),
<<<<<<< HEAD
=======
		.suppress_bind_attrs = true,
>>>>>>> upstream/android-13
	},
	.probe = hisi_ddrc_pmu_probe,
	.remove = hisi_ddrc_pmu_remove,
};

static int __init hisi_ddrc_pmu_module_init(void)
{
	int ret;

	ret = cpuhp_setup_state_multi(CPUHP_AP_PERF_ARM_HISI_DDRC_ONLINE,
				      "AP_PERF_ARM_HISI_DDRC_ONLINE",
				      hisi_uncore_pmu_online_cpu,
				      hisi_uncore_pmu_offline_cpu);
	if (ret) {
		pr_err("DDRC PMU: setup hotplug, ret = %d\n", ret);
		return ret;
	}

	ret = platform_driver_register(&hisi_ddrc_pmu_driver);
	if (ret)
		cpuhp_remove_multi_state(CPUHP_AP_PERF_ARM_HISI_DDRC_ONLINE);

	return ret;
}
module_init(hisi_ddrc_pmu_module_init);

static void __exit hisi_ddrc_pmu_module_exit(void)
{
	platform_driver_unregister(&hisi_ddrc_pmu_driver);
	cpuhp_remove_multi_state(CPUHP_AP_PERF_ARM_HISI_DDRC_ONLINE);

}
module_exit(hisi_ddrc_pmu_module_exit);

MODULE_DESCRIPTION("HiSilicon SoC DDRC uncore PMU driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Shaokun Zhang <zhangshaokun@hisilicon.com>");
MODULE_AUTHOR("Anurup M <anurup.m@huawei.com>");
