// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#include <linux/completion.h>
#include <linux/iio/iio.h>
#include <linux/iio/buffer.h>
#include <linux/iio/kfifo_buf.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
=======

#include <linux/bits.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/kernel.h>
>>>>>>> upstream/android-13
#include <linux/ktime.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

<<<<<<< HEAD
#include <linux/mfd/mt6360-private.h>

/* CHG_CTRL3 0x13 */
#define MT6360_AICR_MASK	(0xFC)
#define MT6360_AICR_SHFT	(2)
#define MT6360_AICR_400MA	(0x6)
/* ADC_CONFIG 0x56 */
#define MT6360_ADCEN_SHFT	(7)
/* ADC_RPT_1 0x5A */
#define MT6360_PREFERCH_MASK	(0xF0)
#define MT6360_PREFERCH_SHFT	(4)
#define MT6360_RPTCH_MASK	(0x0F)
=======
#include <linux/iio/buffer.h>
#include <linux/iio/iio.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/iio/triggered_buffer.h>

#include <asm/unaligned.h>

#define MT6360_REG_PMUCHGCTRL3	0x313
#define MT6360_REG_PMUADCCFG	0x356
#define MT6360_REG_PMUADCIDLET	0x358
#define MT6360_REG_PMUADCRPT1	0x35A

/* PMUCHGCTRL3 0x313 */
#define MT6360_AICR_MASK	GENMASK(7, 2)
#define MT6360_AICR_SHFT	2
#define MT6360_AICR_400MA	0x6
/* PMUADCCFG 0x356 */
#define MT6360_ADCEN_MASK	BIT(15)
/* PMUADCRPT1 0x35A */
#define MT6360_PREFERCH_MASK	GENMASK(7, 4)
#define MT6360_PREFERCH_SHFT	4
#define MT6360_RPTCH_MASK	GENMASK(3, 0)
#define MT6360_NO_PREFER	15

/* Time in ms */
#define ADC_WAIT_TIME_MS	25
#define ADC_CONV_TIMEOUT_MS	100
#define ADC_LOOP_TIME_US	2000
>>>>>>> upstream/android-13

enum {
	MT6360_CHAN_USBID = 0,
	MT6360_CHAN_VBUSDIV5,
	MT6360_CHAN_VBUSDIV2,
	MT6360_CHAN_VSYS,
	MT6360_CHAN_VBAT,
	MT6360_CHAN_IBUS,
	MT6360_CHAN_IBAT,
	MT6360_CHAN_CHG_VDDP,
	MT6360_CHAN_TEMP_JC,
	MT6360_CHAN_VREF_TS,
	MT6360_CHAN_TS,
<<<<<<< HEAD
	MT6360_CHAN_MAX,
};

struct mt6360_adc_info {
	struct device *dev;
	struct regmap *regmap;
	struct task_struct *scan_task;
	struct completion adc_complete;
	struct mutex adc_lock;
	ktime_t last_off_timestamps[MT6360_CHAN_MAX];
	int irq;
};

static inline int mt6360_adc_val_converte(int val, int multiplier,
					   int offset, int divisor)
{
	/* assume val = ((val * multiplier) + offset) / divisor */
	return ((val * multiplier) + offset) / divisor;
}

static int mt6360_adc_get_process_val(struct mt6360_adc_info *info,
				      int chan_idx, int *val)
{
	unsigned int regval = 0;
	int ret;
	const struct converter {
		int multiplier;
		int offset;
		int divisor;
	} adc_converter[MT6360_CHAN_MAX] = {
		{ 1250, 0, 1}, /* USBID */
		{ 6250, 0, 1}, /* VBUSDIV5 */
		{ 2500, 0, 1}, /* VBUSDIV2 */
		{ 1250, 0, 1}, /* VSYS */
		{ 1250, 0, 1}, /* VBAT */
		{ 2500, 0, 1}, /* IBUS */
		{ 2500, 0, 1}, /* IBAT */
		{ 1250, 0, 1}, /* CHG_VDDP */
		{ 105, -8000, 100}, /* TEMP_JC */
		{ 1250, 0, 1}, /* VREF_TS */
		{ 1250, 0, 1}, /* TS */
	}, sp_ibus_adc_converter = { 1900, 0, 1 }, *sel_converter;

	if (chan_idx < 0 || chan_idx >= MT6360_CHAN_MAX)
		return -EINVAL;
	sel_converter = adc_converter + chan_idx;
	if (chan_idx == MT6360_CHAN_IBUS) {
		/* ibus chan will be affected by aicr config */
		/* if aicr < 400, apply the special ibus converter */
		ret = regmap_read(info->regmap, MT6360_PMU_CHG_CTRL3, &regval);
		if (ret < 0)
			return ret;
		regval = (regval & MT6360_AICR_MASK) >> MT6360_AICR_SHFT;
		if (regval < MT6360_AICR_400MA)
			sel_converter = &sp_ibus_adc_converter;
	}
	*val = mt6360_adc_val_converte(*val, sel_converter->multiplier,
				 sel_converter->offset, sel_converter->divisor);
	return 0;
}

static int mt6360_adc_read_raw(struct iio_dev *iio_dev,
			       const struct iio_chan_spec *chan,
			       int *val, int *val2, long mask)
{
	struct mt6360_adc_info *mai = iio_priv(iio_dev);
	long timeout;
	u8 tmp[2], rpt[3];
	ktime_t start_t, predict_end_t;
	int ret;

	dev_dbg(&iio_dev->dev, "%s: channel [%d] s\n", __func__, chan->channel);
	mutex_lock(&mai->adc_lock);
	/* select preferred channel that we want */
	ret = regmap_update_bits(mai->regmap,
				 MT6360_PMU_ADC_RPT_1, MT6360_PREFERCH_MASK,
				 chan->channel << MT6360_PREFERCH_SHFT);
	if (ret < 0)
		goto err_adc_init;
	/* enable adc channel we want and adc_en */
	memset(tmp, 0, sizeof(tmp));
	tmp[0] |= BIT(MT6360_ADCEN_SHFT);
	tmp[(chan->channel / 8) ? 0 : 1] |= BIT(chan->channel % 8);
	ret = regmap_bulk_write(mai->regmap,
				MT6360_PMU_ADC_CONFIG, tmp, sizeof(tmp));
	if (ret < 0)
		goto err_adc_init;
	start_t = ktime_get();
	predict_end_t = ktime_add_ms(
				   mai->last_off_timestamps[chan->channel], 50);
	if (ktime_after(start_t, predict_end_t))
		predict_end_t = ktime_add_ms(start_t, 25);
	else
		predict_end_t = ktime_add_ms(start_t, 75);
	enable_irq(mai->irq);
retry:
	reinit_completion(&mai->adc_complete);
	/* wait for conversion to complete */
	timeout = wait_for_completion_interruptible_timeout(
				     &mai->adc_complete, msecs_to_jiffies(200));
	if (timeout == 0) {
		ret = -ETIMEDOUT;
		goto err_adc_conv;
	} else if (timeout < 0) {
		ret = -EINTR;
		goto err_adc_conv;
	}
	memset(rpt, 0, sizeof(rpt));
	ret = regmap_bulk_read(mai->regmap,
			       MT6360_PMU_ADC_RPT_1, rpt, sizeof(rpt));
	if (ret < 0)
		goto err_adc_conv;
	/* get report channel */
	if ((rpt[0] & MT6360_RPTCH_MASK) != chan->channel) {
		dev_dbg(&iio_dev->dev,
			"not wanted channel report [%02x]\n", rpt[0]);
		goto retry;
	}
	if (!ktime_after(ktime_get(), predict_end_t)) {
		dev_dbg(&iio_dev->dev, "time is not after 26ms chan_time\n");
		goto retry;
	}
	switch (mask) {
	case IIO_CHAN_INFO_RAW:
		*val = (rpt[1] << 8) | rpt[2];
		break;
	case IIO_CHAN_INFO_PROCESSED:
		*val = (rpt[1] << 8) | rpt[2];
		ret = mt6360_adc_get_process_val(mai, chan->channel, val);
		if (ret < 0)
			goto err_adc_conv;
		break;
	default:
		break;
	}
	ret = IIO_VAL_INT;
err_adc_conv:
	disable_irq(mai->irq);
	/* whatever disable all channel and keep adc_en*/
	memset(tmp, 0, sizeof(tmp));
	tmp[0] |= BIT(MT6360_ADCEN_SHFT);
	regmap_bulk_write(mai->regmap, MT6360_PMU_ADC_CONFIG, tmp, sizeof(tmp));
	mai->last_off_timestamps[chan->channel] = ktime_get();
	/* set prefer channel to 0xf */
	regmap_update_bits(mai->regmap, MT6360_PMU_ADC_RPT_1,
			   MT6360_PREFERCH_MASK, 0xF << MT6360_PREFERCH_SHFT);
err_adc_init:
	mutex_unlock(&mai->adc_lock);
	dev_dbg(&iio_dev->dev, "%s: channel [%d] e\n", __func__, chan->channel);
	return ret;
=======
	MT6360_CHAN_MAX
};

struct mt6360_adc_data {
	struct device *dev;
	struct regmap *regmap;
	/* Due to only one set of ADC control, this lock is used to prevent the race condition */
	struct mutex adc_lock;
	ktime_t last_off_timestamps[MT6360_CHAN_MAX];
};

static int mt6360_adc_read_channel(struct mt6360_adc_data *mad, int channel, int *val)
{
	__be16 adc_enable;
	u8 rpt[3];
	ktime_t predict_end_t, timeout;
	unsigned int pre_wait_time;
	int ret;

	mutex_lock(&mad->adc_lock);

	/* Select the preferred ADC channel */
	ret = regmap_update_bits(mad->regmap, MT6360_REG_PMUADCRPT1, MT6360_PREFERCH_MASK,
				 channel << MT6360_PREFERCH_SHFT);
	if (ret)
		goto out_adc_lock;

	adc_enable = cpu_to_be16(MT6360_ADCEN_MASK | BIT(channel));
	ret = regmap_raw_write(mad->regmap, MT6360_REG_PMUADCCFG, &adc_enable, sizeof(adc_enable));
	if (ret)
		goto out_adc_lock;

	predict_end_t = ktime_add_ms(mad->last_off_timestamps[channel], 2 * ADC_WAIT_TIME_MS);

	if (ktime_after(ktime_get(), predict_end_t))
		pre_wait_time = ADC_WAIT_TIME_MS;
	else
		pre_wait_time = 3 * ADC_WAIT_TIME_MS;

	if (msleep_interruptible(pre_wait_time)) {
		ret = -ERESTARTSYS;
		goto out_adc_conv;
	}

	timeout = ktime_add_ms(ktime_get(), ADC_CONV_TIMEOUT_MS);
	while (true) {
		ret = regmap_raw_read(mad->regmap, MT6360_REG_PMUADCRPT1, rpt, sizeof(rpt));
		if (ret)
			goto out_adc_conv;

		/*
		 * There are two functions, ZCV and TypeC OTP, running ADC VBAT and TS in
		 * background, and ADC samples are taken on a fixed frequency no matter read the
		 * previous one or not.
		 * To avoid conflict, We set minimum time threshold after enable ADC and
		 * check report channel is the same.
		 * The worst case is run the same ADC twice and background function is also running,
		 * ADC conversion sequence is desire channel before start ADC, background ADC,
		 * desire channel after start ADC.
		 * So the minimum correct data is three times of typical conversion time.
		 */
		if ((rpt[0] & MT6360_RPTCH_MASK) == channel)
			break;

		if (ktime_compare(ktime_get(), timeout) > 0) {
			ret = -ETIMEDOUT;
			goto out_adc_conv;
		}

		usleep_range(ADC_LOOP_TIME_US / 2, ADC_LOOP_TIME_US);
	}

	*val = rpt[1] << 8 | rpt[2];
	ret = IIO_VAL_INT;

out_adc_conv:
	/* Only keep ADC enable */
	adc_enable = cpu_to_be16(MT6360_ADCEN_MASK);
	regmap_raw_write(mad->regmap, MT6360_REG_PMUADCCFG, &adc_enable, sizeof(adc_enable));
	mad->last_off_timestamps[channel] = ktime_get();
	/* Config prefer channel to NO_PREFER */
	regmap_update_bits(mad->regmap, MT6360_REG_PMUADCRPT1, MT6360_PREFERCH_MASK,
			   MT6360_NO_PREFER << MT6360_PREFERCH_SHFT);
out_adc_lock:
	mutex_unlock(&mad->adc_lock);

	return ret;
}

static int mt6360_adc_read_scale(struct mt6360_adc_data *mad, int channel, int *val, int *val2)
{
	unsigned int regval;
	int ret;

	switch (channel) {
	case MT6360_CHAN_USBID:
	case MT6360_CHAN_VSYS:
	case MT6360_CHAN_VBAT:
	case MT6360_CHAN_CHG_VDDP:
	case MT6360_CHAN_VREF_TS:
	case MT6360_CHAN_TS:
		*val = 1250;
		return IIO_VAL_INT;
	case MT6360_CHAN_VBUSDIV5:
		*val = 6250;
		return IIO_VAL_INT;
	case MT6360_CHAN_VBUSDIV2:
	case MT6360_CHAN_IBUS:
	case MT6360_CHAN_IBAT:
		*val = 2500;

		if (channel == MT6360_CHAN_IBUS) {
			/* IBUS will be affected by input current limit for the different Ron */
			/* Check whether the config is <400mA or not */
			ret = regmap_read(mad->regmap, MT6360_REG_PMUCHGCTRL3, &regval);
			if (ret)
				return ret;

			regval = (regval & MT6360_AICR_MASK) >> MT6360_AICR_SHFT;
			if (regval < MT6360_AICR_400MA)
				*val = 1900;
		}

		return IIO_VAL_INT;
	case MT6360_CHAN_TEMP_JC:
		*val = 105;
		*val2 = 100;
		return IIO_VAL_FRACTIONAL;
	}

	return -EINVAL;
}

static int mt6360_adc_read_offset(struct mt6360_adc_data *mad, int channel, int *val)
{
	*val = (channel == MT6360_CHAN_TEMP_JC) ? -80 : 0;
	return IIO_VAL_INT;
}

static int mt6360_adc_read_raw(struct iio_dev *iio_dev, const struct iio_chan_spec *chan,
			       int *val, int *val2, long mask)
{
	struct mt6360_adc_data *mad = iio_priv(iio_dev);

	switch (mask) {
	case IIO_CHAN_INFO_RAW:
		return mt6360_adc_read_channel(mad, chan->channel, val);
	case IIO_CHAN_INFO_SCALE:
		return mt6360_adc_read_scale(mad, chan->channel, val, val2);
	case IIO_CHAN_INFO_OFFSET:
		return mt6360_adc_read_offset(mad, chan->channel, val);
	}

	return -EINVAL;
}

static const char *mt6360_channel_labels[MT6360_CHAN_MAX] = {
	"usbid", "vbusdiv5", "vbusdiv2", "vsys", "vbat", "ibus", "ibat", "chg_vddp",
	"temp_jc", "vref_ts", "ts",
};

static int mt6360_adc_read_label(struct iio_dev *iio_dev, const struct iio_chan_spec *chan,
				 char *label)
{
	return snprintf(label, PAGE_SIZE, "%s\n", mt6360_channel_labels[chan->channel]);
>>>>>>> upstream/android-13
}

static const struct iio_info mt6360_adc_iio_info = {
	.read_raw = mt6360_adc_read_raw,
<<<<<<< HEAD
=======
	.read_label = mt6360_adc_read_label,
>>>>>>> upstream/android-13
};

#define MT6360_ADC_CHAN(_idx, _type) {				\
	.type = _type,						\
	.channel = MT6360_CHAN_##_idx,				\
	.scan_index = MT6360_CHAN_##_idx,			\
<<<<<<< HEAD
	.scan_type =  {						\
		.sign = 's',					\
		.realbits = 32,					\
		.storagebits = 32,				\
		.shift = 0,					\
		.endianness = IIO_CPU,				\
	},							\
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |		\
				BIT(IIO_CHAN_INFO_PROCESSED),	\
	.datasheet_name = #_idx,					\
	.indexed = 1,						\
=======
	.datasheet_name = #_idx,				\
	.scan_type =  {						\
		.sign = 'u',					\
		.realbits = 16,					\
		.storagebits = 16,				\
		.endianness = IIO_CPU,				\
	},							\
	.indexed = 1,						\
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |		\
				BIT(IIO_CHAN_INFO_SCALE) |	\
				BIT(IIO_CHAN_INFO_OFFSET),	\
>>>>>>> upstream/android-13
}

static const struct iio_chan_spec mt6360_adc_channels[] = {
	MT6360_ADC_CHAN(USBID, IIO_VOLTAGE),
	MT6360_ADC_CHAN(VBUSDIV5, IIO_VOLTAGE),
	MT6360_ADC_CHAN(VBUSDIV2, IIO_VOLTAGE),
	MT6360_ADC_CHAN(VSYS, IIO_VOLTAGE),
	MT6360_ADC_CHAN(VBAT, IIO_VOLTAGE),
	MT6360_ADC_CHAN(IBUS, IIO_CURRENT),
	MT6360_ADC_CHAN(IBAT, IIO_CURRENT),
	MT6360_ADC_CHAN(CHG_VDDP, IIO_VOLTAGE),
	MT6360_ADC_CHAN(TEMP_JC, IIO_TEMP),
	MT6360_ADC_CHAN(VREF_TS, IIO_VOLTAGE),
	MT6360_ADC_CHAN(TS, IIO_VOLTAGE),
	IIO_CHAN_SOFT_TIMESTAMP(MT6360_CHAN_MAX),
};

<<<<<<< HEAD
static irqreturn_t mt6360_pmu_adc_donei_handler(int irq, void *data)
{
	struct mt6360_adc_info *mai = iio_priv(data);

	dev_dbg(mai->dev, "%s\n", __func__);
	complete(&mai->adc_complete);
	return IRQ_HANDLED;
}

static int mt6360_adc_scan_task_threadfn(void *data)
{
	struct mt6360_adc_info *mai = data;
	struct iio_dev *indio_dev = iio_priv_to_dev(mai);
	int channel_vals[MT6360_CHAN_MAX];
	int i, bit, var = 0;
	int ret;

	dev_dbg(mai->dev, "%s ++\n", __func__);
	while (!kthread_should_stop()) {
		memset(channel_vals, 0, sizeof(channel_vals));
		i = 0;
		for_each_set_bit(bit, indio_dev->active_scan_mask,
				 indio_dev->masklength) {
			ret = mt6360_adc_read_raw(indio_dev,
						  mt6360_adc_channels + bit,
						  &var, NULL,
						  IIO_CHAN_INFO_PROCESSED);
			if (ret < 0)
				dev_err(mai->dev, "get adc[%d] fail\n", bit);
			channel_vals[i++] = var;
			if (kthread_should_stop())
				goto out;
		}
		iio_push_to_buffers_with_timestamp(indio_dev, channel_vals,
						   iio_get_time_ns(indio_dev));
	}
out:
	dev_dbg(mai->dev, "%s --\n", __func__);
	do_exit(0);
	return 0;
}

static int mt6360_adc_iio_post_enable(struct iio_dev *iio_dev)
{
	struct mt6360_adc_info *mai = iio_priv(iio_dev);

	dev_dbg(&iio_dev->dev, "%s ++\n", __func__);
	mai->scan_task = kthread_run(mt6360_adc_scan_task_threadfn, mai,
				     "scan_thread.%s", dev_name(&iio_dev->dev));
	dev_dbg(&iio_dev->dev, "%s --\n", __func__);
	return PTR_ERR_OR_ZERO(mai->scan_task);
}

static int mt6360_adc_iio_pre_disable(struct iio_dev *iio_dev)
{
	struct mt6360_adc_info *mai = iio_priv(iio_dev);

	dev_dbg(&iio_dev->dev, "%s ++\n", __func__);
	if (mai->scan_task) {
		kthread_stop(mai->scan_task);
		mai->scan_task = NULL;
	}
	dev_dbg(&iio_dev->dev, "%s --\n", __func__);
	return 0;
}

static const struct iio_buffer_setup_ops mt6360_adc_iio_setup_ops = {
	.postenable = mt6360_adc_iio_post_enable,
	.predisable = mt6360_adc_iio_pre_disable,
};

static int mt6360_adc_iio_device_register(struct iio_dev *indio_dev)
{
	struct mt6360_adc_info *mai = iio_priv(indio_dev);
	struct iio_buffer *buffer;
	int ret;

	dev_dbg(mai->dev, "%s ++\n", __func__);
	indio_dev->name = dev_name(mai->dev);
	indio_dev->dev.parent = mai->dev;
	indio_dev->dev.of_node = mai->dev->of_node;
	indio_dev->info = &mt6360_adc_iio_info;
	indio_dev->channels = mt6360_adc_channels;
	indio_dev->num_channels = ARRAY_SIZE(mt6360_adc_channels);
	indio_dev->modes = INDIO_DIRECT_MODE | INDIO_BUFFER_SOFTWARE;
	indio_dev->setup_ops = &mt6360_adc_iio_setup_ops;
	buffer = devm_iio_kfifo_allocate(mai->dev);
	if (!buffer)
		return -ENOMEM;
	iio_device_attach_buffer(indio_dev, buffer);
	ret = devm_iio_device_register(mai->dev, indio_dev);
	if (ret < 0) {
		dev_err(mai->dev, "iio device  register fail\n");
		return ret;
	}
	dev_dbg(mai->dev, "%s --\n", __func__);
	return 0;
}

static inline int mt6360_adc_reset(struct mt6360_adc_info *info)
{
	u8 tmp[3] = {0x80, 0, 0};
	ktime_t all_off_time;
	int i;

	all_off_time = ktime_get();
	for (i = 0; i < MT6360_CHAN_MAX; i++)
		info->last_off_timestamps[i] = all_off_time;
	/* enable adc_en, clear adc_chn_en/zcv/en/adc_wait_t/adc_idle_t */
	return regmap_bulk_write(info->regmap,
				 MT6360_PMU_ADC_CONFIG, tmp, sizeof(tmp));
=======
static irqreturn_t mt6360_adc_trigger_handler(int irq, void *p)
{
	struct iio_poll_func *pf = p;
	struct iio_dev *indio_dev = pf->indio_dev;
	struct mt6360_adc_data *mad = iio_priv(indio_dev);
	struct {
		u16 values[MT6360_CHAN_MAX];
		int64_t timestamp;
	} data __aligned(8);
	int i = 0, bit, val, ret;

	memset(&data, 0, sizeof(data));
	for_each_set_bit(bit, indio_dev->active_scan_mask, indio_dev->masklength) {
		ret = mt6360_adc_read_channel(mad, bit, &val);
		if (ret < 0) {
			dev_warn(&indio_dev->dev, "Failed to get channel %d conversion val\n", bit);
			goto out;
		}

		data.values[i++] = val;
	}
	iio_push_to_buffers_with_timestamp(indio_dev, &data, iio_get_time_ns(indio_dev));
out:
	iio_trigger_notify_done(indio_dev->trig);

	return IRQ_HANDLED;
}

static inline int mt6360_adc_reset(struct mt6360_adc_data *info)
{
	__be16 adc_enable;
	ktime_t all_off_time;
	int i, ret;

	/* Clear ADC idle wait time to 0 */
	ret = regmap_write(info->regmap, MT6360_REG_PMUADCIDLET, 0);
	if (ret)
		return ret;

	/* Only keep ADC enable, but keep all channels off */
	adc_enable = cpu_to_be16(MT6360_ADCEN_MASK);
	ret = regmap_raw_write(info->regmap, MT6360_REG_PMUADCCFG, &adc_enable, sizeof(adc_enable));
	if (ret)
		return ret;

	/* Reset all channel off time to the current one */
	all_off_time = ktime_get();
	for (i = 0; i < MT6360_CHAN_MAX; i++)
		info->last_off_timestamps[i] = all_off_time;

	return 0;
>>>>>>> upstream/android-13
}

static int mt6360_adc_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct mt6360_adc_info *mai;
	struct iio_dev *indio_dev;
	int ret;

	dev_dbg(&pdev->dev, "%s\n", __func__);
	indio_dev = devm_iio_device_alloc(&pdev->dev, sizeof(*mai));
	if (!indio_dev)
		return -ENOMEM;
	mai = iio_priv(indio_dev);
	mai->dev = &pdev->dev;
	init_completion(&mai->adc_complete);
	mutex_init(&mai->adc_lock);
	platform_set_drvdata(pdev, indio_dev);

	/* get parent regmap */
	mai->regmap = dev_get_regmap(pdev->dev.parent, NULL);
	if (!mai->regmap) {
		dev_err(&pdev->dev, "Faled to get parent regmap\n");
		return -ENODEV;
	}
	/* first reset all channels before use */
	ret = mt6360_adc_reset(mai);
	if (ret < 0) {
		dev_err(&pdev->dev, "adc reset fail\n");
		return ret;
	}
	/* adc iio device register */
	ret = mt6360_adc_iio_device_register(indio_dev);
	if (ret < 0) {
		dev_err(&pdev->dev, "iio dev register fail\n");
		return ret;
	}
	/* irq register */
	mai->irq = platform_get_irq_byname(pdev, "adc_donei");
	if (mai->irq < 0) {
		dev_err(&pdev->dev, "Failed to get adc_done irq\n");
		return mai->irq;
	}
	/* default disable adc_donei irq by default */
	irq_set_status_flags(mai->irq, IRQ_NOAUTOEN);
	ret = devm_request_threaded_irq(&pdev->dev, mai->irq, NULL,
					mt6360_pmu_adc_donei_handler,
					IRQF_TRIGGER_FALLING, "adc_donei",
					platform_get_drvdata(pdev));
	if (ret < 0) {
		dev_err(&pdev->dev, "Fail to register adc_done irq\n");
		return ret;
	}
	dev_info(&pdev->dev, "Successfully probed\n");
	return 0;
}

static int mt6360_adc_remove(struct platform_device *pdev)
{
	struct mt6360_adc_info *mai = platform_get_drvdata(pdev);

	if (mai->scan_task)
		kthread_stop(mai->scan_task);
	return 0;
}

static const struct of_device_id __maybe_unused mt6360_adc_of_id[] = {
	{ .compatible = "mediatek,mt6360_adc", },
	{},
};
MODULE_DEVICE_TABLE(of, mt6360_adc_of_id);

static const struct platform_device_id mt6360_adc_id[] = {
	{ "mt6360_adc", 0 },
	{},
};
MODULE_DEVICE_TABLE(platform, mt6360_adc_id);

static struct platform_driver mt6360_adc_driver = {
	.driver = {
		.name = "mt6360_adc",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(mt6360_adc_of_id),
	},
	.probe = mt6360_adc_probe,
	.remove = mt6360_adc_remove,
	.id_table = mt6360_adc_id,
};
module_platform_driver(mt6360_adc_driver);

MODULE_AUTHOR("CY_Huang <cy_huang@richtek.com>");
MODULE_DESCRIPTION("MT6360 ADC Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
=======
	struct mt6360_adc_data *mad;
	struct regmap *regmap;
	struct iio_dev *indio_dev;
	int ret;

	regmap = dev_get_regmap(pdev->dev.parent, NULL);
	if (!regmap) {
		dev_err(&pdev->dev, "Failed to get parent regmap\n");
		return -ENODEV;
	}

	indio_dev = devm_iio_device_alloc(&pdev->dev, sizeof(*mad));
	if (!indio_dev)
		return -ENOMEM;

	mad = iio_priv(indio_dev);
	mad->dev = &pdev->dev;
	mad->regmap = regmap;
	mutex_init(&mad->adc_lock);

	ret = mt6360_adc_reset(mad);
	if (ret < 0) {
		dev_err(&pdev->dev, "Failed to reset adc\n");
		return ret;
	}

	indio_dev->name = dev_name(&pdev->dev);
	indio_dev->info = &mt6360_adc_iio_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = mt6360_adc_channels;
	indio_dev->num_channels = ARRAY_SIZE(mt6360_adc_channels);

	ret = devm_iio_triggered_buffer_setup(&pdev->dev, indio_dev, NULL,
					      mt6360_adc_trigger_handler, NULL);
	if (ret) {
		dev_err(&pdev->dev, "Failed to allocate iio trigger buffer\n");
		return ret;
	}

	return devm_iio_device_register(&pdev->dev, indio_dev);
}

static const struct of_device_id __maybe_unused mt6360_adc_of_id[] = {
	{ .compatible = "mediatek,mt6360-adc", },
	{}
};
MODULE_DEVICE_TABLE(of, mt6360_adc_of_id);

static struct platform_driver mt6360_adc_driver = {
	.driver = {
		.name = "mt6360-adc",
		.of_match_table = mt6360_adc_of_id,
	},
	.probe = mt6360_adc_probe,
};
module_platform_driver(mt6360_adc_driver);

MODULE_AUTHOR("Gene Chen <gene_chen@richtek.com>");
MODULE_DESCRIPTION("MT6360 ADC Driver");
MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
