// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2019-2021 Samsung Electronics Co. Ltd.
 */

#define pr_fmt(fmt) "[VIB] " fmt
<<<<<<< HEAD
#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/of_device.h>
=======
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
>>>>>>> upstream/android-13
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/vibrator/sec_vibrator.h>
#if IS_ENABLED(CONFIG_SEC_VIB_NOTIFIER)
#include <linux/vibrator/sec_vibrator_notifier.h>
#endif
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
#if defined(CONFIG_BATTERY_GKI)
#include <linux/battery/sec_battery_common.h>
#else
<<<<<<< HEAD
#include <linux/battery/common/sb_psy.h>
=======
#include "../../battery/common/sec_charging_common.h"
>>>>>>> upstream/android-13
#endif
#endif
#if defined(CONFIG_SEC_KUNIT)
#include "kunit_test/sec_vibrator_test.h"
#else
#define __visible_for_testing static
#endif

<<<<<<< HEAD
enum {
	VIB_NO_COMPEMSATION = 0,
	VIB_COMPENSATION_WAY1,
	VIB_COMPENSATION_WAY2,
};

=======
>>>>>>> upstream/android-13
static const int  kMaxBufSize = 7;
static const int kMaxHapticStepSize = 7;
static const char *str_newline = "\n";

static struct sec_vibrator_drvdata *g_ddata;

static char vib_event_cmd[MAX_STR_LEN_EVENT_CMD];

static const char sec_vib_event_cmd[EVENT_CMD_MAX][MAX_STR_LEN_EVENT_CMD] = {
	[EVENT_CMD_NONE]					= "NONE",
	[EVENT_CMD_FOLDER_CLOSE]				= "FOLDER_CLOSE",
	[EVENT_CMD_FOLDER_OPEN]					= "FOLDER_OPEN",
	[EVENT_CMD_ACCESSIBILITY_BOOST_ON]			= "ACCESSIBILITY_BOOST_ON",
	[EVENT_CMD_ACCESSIBILITY_BOOST_OFF]			= "ACCESSIBILITY_BOOST_OFF",
	[EVENT_CMD_TENT_CLOSE]					= "FOLDER_TENT_CLOSE",
	[EVENT_CMD_TENT_OPEN]					= "FOLDER_TENT_OPEN",
};

<<<<<<< HEAD
static struct sec_vibrator_pdata sec_vibrator_pdata = {
	.probe_done = false,
	.normal_ratio = 100,
	.high_temp_ratio = SEC_VIBRATOR_DEFAULT_HIGH_TEMP_RATIO,
	.high_temp_ref = SEC_VIBRATOR_DEFAULT_HIGH_TEMP_REF,
};

=======
>>>>>>> upstream/android-13
#if IS_ENABLED(CONFIG_SEC_VIB_NOTIFIER)
static struct vib_notifier_context vib_notifier;
static struct blocking_notifier_head sec_vib_nb_head = BLOCKING_NOTIFIER_INIT(sec_vib_nb_head);

int sec_vib_notifier_register(struct notifier_block *noti_block)
{
	int ret = 0;

	pr_info("%s\n", __func__);

	if (!noti_block)
		return -EINVAL;

	ret = blocking_notifier_chain_register(&sec_vib_nb_head, noti_block);
	if (ret < 0)
		pr_err("%s: failed(%d)\n", __func__, ret);

	return ret;
}
EXPORT_SYMBOL_GPL(sec_vib_notifier_register);

int sec_vib_notifier_unregister(struct notifier_block *noti_block)
{
	int ret = 0;

	pr_info("%s\n", __func__);

	if (!noti_block)
		return -EINVAL;

	ret = blocking_notifier_chain_unregister(&sec_vib_nb_head, noti_block);
	if (ret < 0)
		pr_err("%s: failed(%d)\n", __func__, ret);

	return ret;
}
EXPORT_SYMBOL_GPL(sec_vib_notifier_unregister);

static int sec_vib_notifier_notify(int en, struct sec_vibrator_drvdata *ddata)
{
	int ret = 0;

	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return -ENODATA;
	}

	vib_notifier.index = ddata->index;
	vib_notifier.timeout = ddata->timeout;

	pr_info("%s: %s, idx: %d timeout: %d\n", __func__, en ? "ON" : "OFF",
		vib_notifier.index, vib_notifier.timeout);

	ret = blocking_notifier_call_chain(&sec_vib_nb_head, en, &vib_notifier);

	switch (ret) {
	case NOTIFY_DONE:
	case NOTIFY_OK:
		pr_info("%s done(0x%x)\n", __func__, ret);
		break;
	default:
		pr_info("%s failed(0x%x)\n", __func__, ret);
		break;
	}

	return ret;
}
#endif /* if IS_ENABLED(CONFIG_SEC_VIB_NOTIFIER) */

#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
static int sec_vibrator_check_temp(struct sec_vibrator_drvdata *ddata)
{
	int ret = 0;
	union power_supply_propval value = {0, };

	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return -ENODATA;
	}

	if (!ddata->vib_ops->set_tuning_with_temp)
		return -ENOSYS;

	psy_do_property("battery", get, POWER_SUPPLY_PROP_TEMP, value);

	ret = ddata->vib_ops->set_tuning_with_temp(ddata->dev, value.intval);

	if (ret)
		pr_err("%s error(%d)\n", __func__, ret);

	return ret;
}
#endif
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
__visible_for_testing int sec_vibrator_set_enable(struct sec_vibrator_drvdata *ddata, bool en)
{
	int ret = 0;

	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return -ENODATA;
	}

	if (!ddata->vib_ops->enable)
		return -ENOSYS;

	ret = ddata->vib_ops->enable(ddata->dev, en);
	if (ret)
		pr_err("%s error(%d)\n", __func__, ret);

#if IS_ENABLED(CONFIG_SEC_VIB_NOTIFIER)
	sec_vib_notifier_notify(en, ddata);
#endif
	return ret;
}

__visible_for_testing int sec_vibrator_set_intensity(struct sec_vibrator_drvdata *ddata, int intensity)
{
	int ret = 0;

	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return -ENODATA;
	}

	if (!ddata->vib_ops->set_intensity)
		return -ENOSYS;

	if ((intensity < -(MAX_INTENSITY)) || (intensity > MAX_INTENSITY)) {
		pr_err("%s out of range(%d)\n", __func__, intensity);
		return -EINVAL;
	}

	ret = ddata->vib_ops->set_intensity(ddata->dev, intensity);

	if (ret)
		pr_err("%s error(%d)\n", __func__, ret);

	return ret;
}

__visible_for_testing int sec_vibrator_set_frequency(struct sec_vibrator_drvdata *ddata, int frequency)
{
	int ret = 0;

	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return -ENODATA;
	}

	if (!ddata->vib_ops->set_frequency)
		return -ENOSYS;

	if ((frequency < FREQ_ALERT) ||
	    ((frequency >= FREQ_MAX) && (frequency < HAPTIC_ENGINE_FREQ_MIN)) ||
	    (frequency > HAPTIC_ENGINE_FREQ_MAX)) {
		pr_err("%s out of range(%d)\n", __func__, frequency);
		return -EINVAL;
	}

	ret = ddata->vib_ops->set_frequency(ddata->dev, frequency);

	if (ret)
		pr_err("%s error(%d)\n", __func__, ret);

	return ret;
}

__visible_for_testing int sec_vibrator_set_overdrive(struct sec_vibrator_drvdata *ddata, bool en)
{
	int ret = 0;

	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return -ENODATA;
	}

	if (!ddata->vib_ops->set_overdrive)
		return -ENOSYS;

	ret = ddata->vib_ops->set_overdrive(ddata->dev, en);
	if (ret)
		pr_err("%s error(%d)\n", __func__, ret);

	return ret;
}

static void sec_vibrator_haptic_enable(struct sec_vibrator_drvdata *ddata)
{
	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return;
	}

#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	sec_vibrator_check_temp(ddata);
#endif
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
	sec_vibrator_set_frequency(ddata, ddata->frequency);
	sec_vibrator_set_intensity(ddata, ddata->intensity);
	sec_vibrator_set_enable(ddata, true);

	if (ddata->vib_ops->set_frequency)
		pr_info("freq:%d, intensity:%d, %dms\n", ddata->frequency, ddata->intensity, ddata->timeout);
	else if (ddata->vib_ops->set_intensity)
		pr_info("intensity:%d, %dms\n", ddata->intensity, ddata->timeout);
	else
		pr_info("%dms\n", ddata->timeout);
}

static void sec_vibrator_haptic_disable(struct sec_vibrator_drvdata *ddata)
{
	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return;
	}

	/* clear common variables */
	ddata->index = 0;

	/* clear haptic engine variables */
	ddata->f_packet_en = false;
	ddata->packet_cnt = 0;
	ddata->packet_size = 0;

	sec_vibrator_set_enable(ddata, false);
	sec_vibrator_set_overdrive(ddata, false);
	sec_vibrator_set_frequency(ddata, FREQ_ALERT);
	sec_vibrator_set_intensity(ddata, 0);

	if (ddata->timeout > 0)
		pr_info("timeout, off\n");
	else
		pr_info("off\n");
}

static void sec_vibrator_engine_run_packet(struct sec_vibrator_drvdata *ddata, struct vib_packet packet)
{
	int frequency = packet.freq;
	int intensity = packet.intensity;
	int overdrive = packet.overdrive;
<<<<<<< HEAD
	int fifo =  packet.fifo_flag;
	int file_num = 0;
=======
>>>>>>> upstream/android-13

	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return;
	}

	if (!ddata->f_packet_en) {
		pr_err("haptic packet is empty\n");
		return;
	}

<<<<<<< HEAD
	if (fifo == 0)
		pr_info("%s [%d] freq:%d, intensity:%d, time:%d overdrive: %d\n",
		__func__, ddata->packet_cnt, frequency, intensity,
		ddata->timeout, overdrive);
	else
		pr_info("%s [%d] fifo num:%d, intensity:%d, time:%d overdrive: %d\n",
		__func__, ddata->packet_cnt, frequency, intensity,
		ddata->timeout, overdrive);

	sec_vibrator_set_overdrive(ddata, overdrive);
	if (fifo == 0)
		sec_vibrator_set_frequency(ddata, frequency);
	else
		file_num = frequency;

	if (intensity) {
		if (fifo == 0) {
			sec_vibrator_set_intensity(ddata, intensity);
			if (!ddata->packet_running) {
				pr_info("[haptic engine] motor run\n");
				sec_vibrator_set_enable(ddata, true);
				ddata->packet_running = true;
			} else if (ddata->vib_ops->update_packet_params) {
				pr_info("motor running, parameters update\n");
				ddata->vib_ops->update_packet_params(ddata->dev);
			}
		} else {
			ddata->vib_ops->set_fifo_intensity(ddata->dev, intensity);
			ddata->packet_running = false;
			sec_vibrator_set_enable(ddata, false);
			ddata->vib_ops->enable_fifo(ddata->dev, file_num);
		}
=======
	sec_vibrator_set_overdrive(ddata, overdrive);
	sec_vibrator_set_frequency(ddata, frequency);
	if (intensity) {
		sec_vibrator_set_intensity(ddata, intensity);
		if (!ddata->packet_running) {
			pr_info("[haptic engine] motor run\n");
			sec_vibrator_set_enable(ddata, true);
		}
		ddata->packet_running = true;
>>>>>>> upstream/android-13
	} else {
		if (ddata->packet_running) {
			pr_info("[haptic engine] motor stop\n");
			sec_vibrator_set_enable(ddata, false);
		}
		ddata->packet_running = false;
		sec_vibrator_set_intensity(ddata, intensity);
	}

<<<<<<< HEAD
	pr_info("%s end\n", __func__);
=======
	pr_info("%s [%d] freq:%d, intensity:%d, time:%d overdrive: %d\n",
		__func__, ddata->packet_cnt, frequency, intensity, ddata->timeout, overdrive);
>>>>>>> upstream/android-13
}

static void timed_output_enable(struct sec_vibrator_drvdata *ddata, unsigned int value)
{
	struct hrtimer *timer = &ddata->timer;
	int ret = 0;

	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return;
	}

	ret = hrtimer_cancel(timer);
	kthread_flush_worker(&ddata->kworker);

	mutex_lock(&ddata->vib_mutex);

	value = min_t(int, value, MAX_TIMEOUT);
	ddata->timeout = value;

	if (value) {
		if (ddata->f_packet_en) {
			ddata->packet_running = false;
			ddata->timeout = ddata->vib_pac[0].time;
			sec_vibrator_engine_run_packet(ddata, ddata->vib_pac[0]);
		} else {
			sec_vibrator_haptic_enable(ddata);
		}

		if (!ddata->index)
			hrtimer_start(timer, ns_to_ktime((u64)ddata->timeout * NSEC_PER_MSEC), HRTIMER_MODE_REL);
	} else {
		sec_vibrator_haptic_disable(ddata);
	}

	mutex_unlock(&ddata->vib_mutex);
}

static enum hrtimer_restart haptic_timer_func(struct hrtimer *timer)
{
	struct sec_vibrator_drvdata *ddata;

	if (!timer) {
		pr_err("%s : timer is NULL\n", __func__);
		return -ENODATA;
	}

	ddata = container_of(timer, struct sec_vibrator_drvdata, timer);

	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return -ENODATA;
	}

	pr_info("%s\n", __func__);
	kthread_queue_work(&ddata->kworker, &ddata->kwork);
	return HRTIMER_NORESTART;
}

static void sec_vibrator_work(struct kthread_work *work)
{
	struct sec_vibrator_drvdata *ddata;
	struct hrtimer *timer;
<<<<<<< HEAD
	int fifo = 0;
=======
>>>>>>> upstream/android-13

	if (!work) {
		pr_err("%s : work is NULL\n", __func__);
		return;
	}

	ddata = container_of(work, struct sec_vibrator_drvdata, kwork);

	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return;
	}

	timer = &ddata->timer;

	if (!timer) {
		pr_err("%s : timer is NULL\n", __func__);
		return;
	}

<<<<<<< HEAD
	pr_info("%s\n", __func__);
=======
>>>>>>> upstream/android-13
	mutex_lock(&ddata->vib_mutex);

	if (ddata->f_packet_en) {
		ddata->packet_cnt++;
		if (ddata->packet_cnt < ddata->packet_size) {
			ddata->timeout = ddata->vib_pac[ddata->packet_cnt].time;
<<<<<<< HEAD
			fifo = ddata->vib_pac[ddata->packet_cnt].fifo_flag;
			if (fifo == 0) {
				sec_vibrator_engine_run_packet(ddata, ddata->vib_pac[ddata->packet_cnt]);
				hrtimer_start(timer, ns_to_ktime((u64)ddata->timeout * NSEC_PER_MSEC),
				HRTIMER_MODE_REL);
			} else {
				hrtimer_start(timer, ns_to_ktime((u64)ddata->timeout * NSEC_PER_MSEC),
				HRTIMER_MODE_REL);
				sec_vibrator_engine_run_packet(ddata, ddata->vib_pac[ddata->packet_cnt]);
			}
=======
			sec_vibrator_engine_run_packet(ddata, ddata->vib_pac[ddata->packet_cnt]);
			hrtimer_start(timer, ns_to_ktime((u64)ddata->timeout * NSEC_PER_MSEC), HRTIMER_MODE_REL);
>>>>>>> upstream/android-13
			goto unlock_without_vib_off;
		} else {
			ddata->f_packet_en = false;
			ddata->packet_cnt = 0;
			ddata->packet_size = 0;
		}
	}

	sec_vibrator_haptic_disable(ddata);

unlock_without_vib_off:
	mutex_unlock(&ddata->vib_mutex);
}

__visible_for_testing inline bool is_valid_params(struct device *dev, struct device_attribute *attr,
	const char *buf, struct sec_vibrator_drvdata *ddata)
{
	if (!dev) {
		pr_err("%s : dev is NULL\n", __func__);
		return false;
	}
	if (!attr) {
		pr_err("%s : attr is NULL\n", __func__);
		return false;
	}
	if (!buf) {
		pr_err("%s : buf is NULL\n", __func__);
		return false;
	}
	if (!ddata) {
		pr_err("%s : ddata is NULL\n", __func__);
		return false;
	}
	return true;
}

<<<<<<< HEAD
__visible_for_testing ssize_t default_duty_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int default_duty = 0, ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	ret = kstrtoint(buf, 0, &default_duty);
	if (ret) {
		pr_err("%s : fail to get default_duty\n", __func__);
		return -EINVAL;
	}

	pr_info("%s %d\n", __func__, default_duty);

	if ((default_duty < 0) || (default_duty > MAX_DUTY)) {
		pr_err("[VIB]: %s out of range\n", __func__);
		return -EINVAL;
	}

	if (ddata->vib_ops->set_default_duty) {
		ret = ddata->vib_ops->set_default_duty(ddata->dev, default_duty);
		if (ret) {
			pr_err("%s set_default_duty error : %d\n", __func__, ret);
			goto err;
		}
	} else {
		pr_info("%s this model doesn't need set_default_duty\n", __func__);
	}

err:
	return ret ? ret : count;
}

__visible_for_testing ssize_t default_duty_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_default_duty)
		return snprintf(buf, VIB_BUFSIZE, "NONE\n");

	ret = ddata->vib_ops->get_default_duty(ddata->dev, buf);

	return ret;
}

__visible_for_testing ssize_t fold_open_duty_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int fold_open_duty = 0, ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	ret = kstrtoint(buf, 0, &fold_open_duty);
	if (ret) {
		pr_err("%s : fail to get fold_open_duty\n", __func__);
		return -EINVAL;
	}

	pr_info("%s %d\n", __func__, fold_open_duty);

	if ((fold_open_duty < 0) || (fold_open_duty > MAX_DUTY)) {
		pr_err("[VIB]: %s out of range\n", __func__);
		return -EINVAL;
	}

	if (ddata->vib_ops->set_fold_open_duty) {
		ret = ddata->vib_ops->set_fold_open_duty(ddata->dev, fold_open_duty);
		if (ret) {
			pr_err("%s set_fold_open_duty error : %d\n", __func__, ret);
			goto err;
		}
	} else {
		pr_info("%s this model doesn't need set_fold_open_duty\n", __func__);
	}

err:
	return ret ? ret : count;
}

__visible_for_testing ssize_t fold_open_duty_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_fold_open_duty)
		return snprintf(buf, VIB_BUFSIZE, "NONE\n");

	ret = ddata->vib_ops->get_fold_open_duty(ddata->dev, buf);

	return ret;
}

__visible_for_testing ssize_t fold_close_duty_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int fold_close_duty = 0, ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	ret = kstrtoint(buf, 0, &fold_close_duty);
	if (ret) {
		pr_err("%s : fail to get fold_close_duty\n", __func__);
		return -EINVAL;
	}

	pr_info("%s %d\n", __func__, fold_close_duty);

	if ((fold_close_duty < 0) || (fold_close_duty > MAX_DUTY)) {
		pr_err("[VIB]: %s out of range\n", __func__);
		return -EINVAL;
	}

	if (ddata->vib_ops->set_fold_close_duty) {
		ret = ddata->vib_ops->set_fold_close_duty(ddata->dev, fold_close_duty);
		if (ret) {
			pr_err("%s set_fold_close_duty error : %d\n", __func__, ret);
			goto err;
		}
	} else {
		pr_info("%s this model doesn't need set_fold_close_duty\n", __func__);
	}

err:
	return ret ? ret : count;
}

__visible_for_testing ssize_t fold_close_duty_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_fold_close_duty)
		return snprintf(buf, VIB_BUFSIZE, "NONE\n");

	ret = ddata->vib_ops->get_fold_close_duty(ddata->dev, buf);

	return ret;
}

=======
>>>>>>> upstream/android-13
__visible_for_testing ssize_t intensity_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int intensity = 0, ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	ret = kstrtoint(buf, 0, &intensity);
	if (ret) {
		pr_err("%s : fail to get intensity\n", __func__);
		return -EINVAL;
	}

	pr_info("%s %d\n", __func__, intensity);

	if ((intensity < 0) || (intensity > MAX_INTENSITY)) {
		pr_err("[VIB]: %s out of range\n", __func__);
		return -EINVAL;
	}

	ddata->intensity = intensity;

	return count;
}

__visible_for_testing ssize_t intensity_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;
	return snprintf(buf, VIB_BUFSIZE, "intensity: %u\n", ddata->intensity);
}

<<<<<<< HEAD
static ssize_t fifo_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = (struct sec_vibrator_drvdata *) dev_get_drvdata(dev);
	int ret;
	int file_num;

	pr_info("%s +\n", __func__);

	ret = kstrtoint(buf, 0, &file_num);
	if (ret) {
		pr_err("fail to get file_num\n");
		return -EINVAL;
	}

	if (!ddata->vib_ops->enable_fifo || !ddata->vib_ops->set_fifo_intensity) {
		pr_err("fifo functionality doesn't support\n");
		return -EOPNOTSUPP;
	}

	ret = ddata->vib_ops->set_fifo_intensity(ddata->dev, 10000);
	ret = ddata->vib_ops->enable_fifo(ddata->dev, file_num);

	pr_info("%s -\n", __func__);
	return count;
}

static ssize_t fifo_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = (struct sec_vibrator_drvdata *)
		dev_get_drvdata(dev);
	int ret = 0;

	if (!ddata->vib_ops->get_fifo_filepath)
		return snprintf(buf, VIB_BUFSIZE, "NONE\n");

	ret = ddata->vib_ops->get_fifo_filepath(ddata->dev, buf);

	return ret;
}

=======
>>>>>>> upstream/android-13
__visible_for_testing ssize_t multi_freq_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int num, ret;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	ret = kstrtoint(buf, 0, &num);
	if (ret) {
		pr_err("fail to get frequency\n");
		return -EINVAL;
	}

	pr_info("%s %d\n", __func__, num);

	ddata->frequency = num;

	return count;
}

__visible_for_testing ssize_t multi_freq_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;
	return snprintf(buf, VIB_BUFSIZE, "frequency: %d\n", ddata->frequency);
}

// TODO: need to update
__visible_for_testing ssize_t haptic_engine_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int index = 0, _data = 0, tmp = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (sscanf(buf, "%6d", &_data) != 1)
		return count;

	if (_data > PACKET_MAX_SIZE * VIB_PACKET_MAX) {
		pr_info("%s, [%d] packet size over\n", __func__, _data);
		return count;
	}
	ddata->packet_size = _data / VIB_PACKET_MAX;
	ddata->packet_cnt = 0;
	ddata->f_packet_en = true;
	buf = strstr(buf, " ");

	for (index = 0; index < ddata->packet_size; index++) {
		for (tmp = 0; tmp < VIB_PACKET_MAX; tmp++) {
			if (buf == NULL) {
				pr_err("%s, buf is NULL, Please check packet data again\n", __func__);
				ddata->f_packet_en = false;
				return count;
			}

			if (sscanf(buf++, "%6d", &_data) != 1) {
				pr_err("%s, packet data error, Please check packet data again\n", __func__);
				ddata->f_packet_en = false;
				return count;
			}

			switch (tmp) {
			case VIB_PACKET_TIME:
				ddata->vib_pac[index].time = _data;
				break;
			case VIB_PACKET_INTENSITY:
				ddata->vib_pac[index].intensity = _data;
				break;
			case VIB_PACKET_FREQUENCY:
				ddata->vib_pac[index].freq = _data;
				break;
			case VIB_PACKET_OVERDRIVE:
				ddata->vib_pac[index].overdrive = _data;
				break;
			}
			buf = strstr(buf, " ");
		}
	}

	return count;
}

__visible_for_testing ssize_t haptic_engine_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int index = 0;
	size_t size = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	for (index = 0; index < ddata->packet_size && ddata->f_packet_en &&
	     ((4 * VIB_BUFSIZE + size) < PAGE_SIZE); index++) {
		size += snprintf(&buf[size], VIB_BUFSIZE, "%u,", ddata->vib_pac[index].time);
		size += snprintf(&buf[size], VIB_BUFSIZE, "%u,", ddata->vib_pac[index].intensity);
		size += snprintf(&buf[size], VIB_BUFSIZE, "%u,", ddata->vib_pac[index].freq);
		size += snprintf(&buf[size], VIB_BUFSIZE, "%u,", ddata->vib_pac[index].overdrive);
	}

	return size;
}

<<<<<<< HEAD
static ssize_t hybrid_haptic_engine_store(struct device *dev,
	struct device_attribute *devattr, const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = (struct sec_vibrator_drvdata *)
		dev_get_drvdata(dev);
	int i = 0, _data = 0, tmp = 0, check_fifo = 0, origin_time = 0;
	int way1_cmp_sum = 0, way2_cmp_sum = 0;
	int normal_pac_time_sum = 0;
	int compensation_way = VIB_NO_COMPEMSATION;

	pr_info("%s +\n", __func__);
	if (sscanf(buf, "%6d", &_data) != 1)
		return count;

	if (_data > PACKET_MAX_SIZE * VIB_PACKET_MAX)
		pr_info("%s, [%d] packet size over\n", __func__, _data);
	else {
		ddata->packet_size = _data / VIB_PACKET_MAX;
		ddata->packet_cnt = 0;
		ddata->f_packet_en = true;

		buf = strstr(buf, " ");

		for (i = 0; i < ddata->packet_size; i++) {
			check_fifo = 0;
			for (tmp = 0; tmp < VIB_PACKET_MAX; tmp++) {
				if (buf == NULL) {
					pr_err("%s, buf is NULL, Please check packet data again\n",
							__func__);
					ddata->f_packet_en = false;
					return count;
				}

				if (*(buf+1) == '#') {
					buf = buf+2;
					if ((*buf != '\0') && (sscanf(buf++, "%6d", &_data) == 1))
						check_fifo = 1;
				}

				else if (sscanf(buf++, "%6d", &_data) != 1) {
					pr_err("%s, packet data error, Please check packet data again\n",
							__func__);
					ddata->f_packet_en = false;
					return count;
				}

				switch (tmp) {
				case VIB_PACKET_TIME:
					ddata->vib_pac[i].time = _data;
					break;
				case VIB_PACKET_INTENSITY:
					ddata->vib_pac[i].intensity = _data;
					break;
				case VIB_PACKET_FREQUENCY:
					ddata->vib_pac[i].freq = _data;
					if (check_fifo == 0) {
						ddata->vib_pac[i].fifo_flag = 0;
						if (ddata->time_compensation && ddata->max_delay_ms) {
							origin_time = ddata->vib_pac[i].time;
							normal_pac_time_sum += origin_time;
							way1_cmp_sum += (origin_time*ddata->time_compensation)/100;
							way2_cmp_sum += ddata->max_delay_ms;
						}
					} else
						ddata->vib_pac[i].fifo_flag = 1;
					break;
				case VIB_PACKET_OVERDRIVE:
					ddata->vib_pac[i].overdrive = _data;
					break;
				}
				buf = strstr(buf, " ");
			}
		}

		if (ddata->time_compensation == 0 || way2_cmp_sum <= ddata->max_delay_ms) {
			compensation_way = VIB_NO_COMPEMSATION;
		} else if (way1_cmp_sum < way2_cmp_sum) {
			compensation_way = VIB_COMPENSATION_WAY1;
			pr_info("%s compensation way1:way1_cmp_sum=%d\n",
				__func__, way1_cmp_sum);
		} else {
			compensation_way = VIB_COMPENSATION_WAY2;
			pr_info("%s compensation way2:way2_cmp_sum=%d,normal_pac_time_sum=%d\n",
				__func__, way2_cmp_sum, normal_pac_time_sum);
		}

		for (i = 0; i < ddata->packet_size; i++) {
			if (ddata->vib_pac[i].fifo_flag == 1) {
				pr_info("%s i=%d, file=%d time=%d intensity=%d\n", __func__, i,
					ddata->vib_pac[i].freq,
					ddata->vib_pac[i].time,
					ddata->vib_pac[i].intensity);
				continue;
			}
			origin_time = ddata->vib_pac[i].time;
			if (compensation_way == VIB_COMPENSATION_WAY1)
				ddata->vib_pac[i].time -= (origin_time*ddata->time_compensation)/100;
			else if (compensation_way == VIB_COMPENSATION_WAY2 && normal_pac_time_sum != 0)
				ddata->vib_pac[i].time -= (way2_cmp_sum*origin_time)/normal_pac_time_sum;

			pr_info("%s i=%d, origin_time=%d->time=%d intensity=%d freq=%d\n",
				__func__, i, origin_time,
				ddata->vib_pac[i].time,
				ddata->vib_pac[i].intensity,
				ddata->vib_pac[i].freq);
		}
	}
	pr_info("%s -\n", __func__);
	return count;
}

static ssize_t hybrid_haptic_engine_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = (struct sec_vibrator_drvdata *)
		dev_get_drvdata(dev);
	int i = 0;
	size_t size = 0;

	for (i = 0; i < ddata->packet_size && ddata->f_packet_en &&
			((4 * VIB_BUFSIZE + size) < PAGE_SIZE); i++) {
		size += snprintf(&buf[size], VIB_BUFSIZE, "%u,", ddata->vib_pac[i].time);
		size += snprintf(&buf[size], VIB_BUFSIZE, "%u,", ddata->vib_pac[i].intensity);
		size += snprintf(&buf[size], VIB_BUFSIZE, "%u,", ddata->vib_pac[i].freq);
		size += snprintf(&buf[size], VIB_BUFSIZE, "%u,", ddata->vib_pac[i].overdrive);
	}

	return size;
}

=======
>>>>>>> upstream/android-13
__visible_for_testing ssize_t cp_trigger_index_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_cp_trigger_index)
		return -ENOSYS;

	ret = ddata->vib_ops->get_cp_trigger_index(ddata->dev, buf);

	return ret;
}

__visible_for_testing ssize_t cp_trigger_index_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;
	unsigned int index;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->set_cp_trigger_index)
		return -ENOSYS;

	ret = kstrtou32(buf, 10, &index);
	if (ret)
		return -EINVAL;

	ddata->index = index;

	ret = ddata->vib_ops->set_cp_trigger_index(ddata->dev, buf);
	if (ret < 0)
		pr_err("%s error(%d)\n", __func__, ret);

	return count;
}

__visible_for_testing ssize_t cp_trigger_queue_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_cp_trigger_queue)
		return -ENOSYS;

	ret = ddata->vib_ops->get_cp_trigger_queue(ddata->dev, buf);
	if (ret < 0)
		pr_err("%s error(%d)\n", __func__, ret);

	return ret;
}

__visible_for_testing ssize_t cp_trigger_queue_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->set_cp_trigger_queue)
		return -ENOSYS;

	ret = ddata->vib_ops->set_cp_trigger_queue(ddata->dev, buf);
	if (ret < 0)
		pr_err("%s error(%d)\n", __func__, ret);

	return count;
}

__visible_for_testing ssize_t pwle_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_pwle)
		return -ENOSYS;

	ret = ddata->vib_ops->get_pwle(ddata->dev, buf);
	if (ret < 0)
		pr_err("%s error(%d)\n", __func__, ret);

	return ret;
}

__visible_for_testing ssize_t pwle_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->set_pwle)
		return -ENOSYS;

	ret = ddata->vib_ops->set_pwle(ddata->dev, buf);
	if (ret < 0)
		pr_err("%s error(%d)\n", __func__, ret);

	return count;
}

__visible_for_testing ssize_t virtual_composite_indexes_show(struct device *dev, struct device_attribute *attr,
	char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_virtual_composite_indexes)
		return -ENOSYS;

	ret = ddata->vib_ops->get_virtual_composite_indexes(ddata->dev, buf);
	if (ret < 0)
		pr_err("%s error(%d)\n", __func__, ret);

	return ret;
}

__visible_for_testing ssize_t virtual_pwle_indexes_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_virtual_pwle_indexes)
		return -ENOSYS;

	ret = ddata->vib_ops->get_virtual_pwle_indexes(ddata->dev, buf);
	if (ret < 0)
		pr_err("%s error(%d)\n", __func__, ret);

	return ret;
}

__visible_for_testing ssize_t enable_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	struct hrtimer *timer = &ddata->timer;
	int remaining = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (hrtimer_active(timer)) {
		ktime_t remain = hrtimer_get_remaining(timer);
		struct timespec64 t = ns_to_timespec64(remain);

		remaining = t.tv_sec * 1000 + t.tv_nsec / 1000;
	}
	return sprintf(buf, "%d\n", remaining);
}

__visible_for_testing ssize_t enable_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t size)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int value;
	int ret;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	ret = kstrtoint(buf, 0, &value);
	if (ret != 0)
		return -EINVAL;

	timed_output_enable(ddata, value);
	return size;
}

__visible_for_testing ssize_t motor_type_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_motor_type)
		return snprintf(buf, VIB_BUFSIZE, "NONE\n");

	ret = ddata->vib_ops->get_motor_type(ddata->dev, buf);

	return ret;
}

__visible_for_testing ssize_t use_sep_index_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t size)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;
	bool use_sep_index;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;
	
	ret = kstrtobool(buf, &use_sep_index);
	if (ret < 0) {
		pr_err("%s kstrtobool error : %d\n", __func__, ret);
		goto err;
	}
	
	pr_info("%s use_sep_index:%d\n", __func__, use_sep_index);
	
	if (ddata->vib_ops->set_use_sep_index) {
		ret = ddata->vib_ops->set_use_sep_index(ddata->dev, use_sep_index);
		if (ret) {
			pr_err("%s set_use_sep_index error : %d\n", __func__, ret);
			goto err;
		}
	} else {
		pr_info("%s this model doesn't need use_sep_index\n", __func__);
	}
err:
	return ret ? ret : size;
}

<<<<<<< HEAD
int sec_vibrator_recheck_ratio(struct sec_vibrator_drvdata *ddata)
{
	int temp = ddata->temperature;

	if (temp >= ddata->pdata->high_temp_ref)
		return ddata->pdata->high_temp_ratio;

	return ddata->pdata->normal_ratio;
}
EXPORT_SYMBOL_GPL(sec_vibrator_recheck_ratio);

__visible_for_testing ssize_t current_temp_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	return sprintf(buf, "%d\n", ddata->temperature);
}

__visible_for_testing ssize_t current_temp_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int temp, ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	ret = kstrtos32(buf, 0, &temp);
	if (ret < 0) {
		pr_err("%s kstrtos32 error : %d\n", __func__, ret);
		ddata->temperature = INT_MIN;
		goto err;
	}

	ddata->temperature = temp;
	pr_info("%s temperature : %d\n", __func__, ddata->temperature);
err:
	return ret ? ret : count;
}

=======
>>>>>>> upstream/android-13
__visible_for_testing ssize_t num_waves_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int ret = 0;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_num_waves)
		return -ENOSYS;

	ret = ddata->vib_ops->get_num_waves(ddata->dev, buf);

	return ret;
}

__visible_for_testing ssize_t array2str(char *buf, int *arr_intensity, int size)
{
	int index, ret = 0;
	char *str_buf = NULL;
	struct sec_vibrator_drvdata *ddata = g_ddata;

	if (!buf || !ddata)
		return -ENODATA;

	if (!arr_intensity || ((size < 1) && (size > kMaxHapticStepSize)))
		return -EINVAL;

	str_buf = kzalloc(kMaxBufSize, GFP_KERNEL);
	if (!str_buf)
		return -ENOMEM;

	mutex_lock(&ddata->vib_mutex);
	for (index = 0; index < size; index++) {
		if (index < (size - 1))
			snprintf(str_buf, kMaxBufSize, "%u,", arr_intensity[index]);
		else
			snprintf(str_buf, (kMaxBufSize - 1), "%u", arr_intensity[index]);
		strncat(buf, str_buf, strlen(str_buf));
	}
	strncat(buf, str_newline, strlen(str_newline));
	ret = strlen(buf);
	mutex_unlock(&ddata->vib_mutex);

	kfree(str_buf);
	return ret;
}

__visible_for_testing ssize_t intensities_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int *arr_intensity = NULL;
	int ret = 0, step_size = 0;

	pr_info("%s\n", __func__);

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_step_size || !ddata->vib_ops->get_intensities)
		return -EOPNOTSUPP;

	ret = ddata->vib_ops->get_step_size(ddata->dev, &step_size);
	if (ret)
		return -EINVAL;

	arr_intensity = kmalloc_array(kMaxHapticStepSize, sizeof(int), GFP_KERNEL);
	if (!arr_intensity)
		return -ENOMEM;

	if ((step_size > 0) && (step_size < kMaxHapticStepSize)) {
		ret = ddata->vib_ops->get_intensities(ddata->dev, arr_intensity);
		if (ret) {
			ret = -EINVAL;
			goto err_arr_alloc;
		}
		ret = array2str(buf, arr_intensity, step_size);
	} else {
		ret = -EINVAL;
	}

err_arr_alloc:
	kfree(arr_intensity);
	return ret;
}

__visible_for_testing ssize_t haptic_intensities_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int *arr_intensity = NULL;
	int ret = 0, step_size = 0;

	pr_info("%s\n", __func__);

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_step_size || !ddata->vib_ops->get_haptic_intensities)
		return -EOPNOTSUPP;

	ret = ddata->vib_ops->get_step_size(ddata->dev, &step_size);
	if (ret)
		return -EINVAL;

	arr_intensity = kmalloc_array(kMaxHapticStepSize, sizeof(int), GFP_KERNEL);
	if (!arr_intensity)
		return -ENOMEM;

	if ((step_size > 0) && (step_size < kMaxHapticStepSize)) {
		ret = ddata->vib_ops->get_haptic_intensities(ddata->dev, arr_intensity);
		if (ret) {
			ret = -EINVAL;
			goto err_arr_alloc;
		}
		ret = array2str(buf, arr_intensity, step_size);
	} else {
		ret = -EINVAL;
	}

err_arr_alloc:
	kfree(arr_intensity);
	return ret;
}

__visible_for_testing ssize_t haptic_durations_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	int *arr_duration = NULL;
	int ret = 0, step_size = 0;

	pr_info("%s\n", __func__);

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->get_step_size || !ddata->vib_ops->get_haptic_durations)
		return -EOPNOTSUPP;

	ret = ddata->vib_ops->get_step_size(ddata->dev, &step_size);
	if (ret)
		return -EINVAL;

	arr_duration = kmalloc_array(kMaxHapticStepSize, sizeof(int), GFP_KERNEL);
	if (!arr_duration)
		return -ENOMEM;

	if ((step_size > 0) && (step_size < kMaxHapticStepSize)) {
		ret = ddata->vib_ops->get_haptic_durations(ddata->dev, arr_duration);
		if (ret) {
			ret = -EINVAL;
			goto err_arr_alloc;
		}
		ret = array2str(buf, arr_duration, step_size);
	} else {
		ret = -EINVAL;
	}

err_arr_alloc:
	kfree(arr_duration);
	return ret;
}

__visible_for_testing int get_event_index_by_command(char *cur_cmd)
{
	int cmd_idx;

	for (cmd_idx = 0; cmd_idx < EVENT_CMD_MAX; cmd_idx++) {
		if (!strcmp(cur_cmd, sec_vib_event_cmd[cmd_idx]))
			return cmd_idx;
	}

	return EVENT_CMD_NONE;
}

__visible_for_testing ssize_t event_cmd_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;

	pr_info("%s event: %s\n", __func__, vib_event_cmd);

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	return snprintf(buf, MAX_STR_LEN_EVENT_CMD, "%s\n", vib_event_cmd);
}

__visible_for_testing ssize_t event_cmd_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct sec_vibrator_drvdata *ddata = g_ddata;
	char *cmd;
	int ret = 0;
	int event_idx;

	if (!is_valid_params(dev, attr, buf, ddata))
		return -ENODATA;

	if (!ddata->vib_ops->set_event_cmd)
		return -ENOSYS;

<<<<<<< HEAD
	if (count >= MAX_STR_LEN_EVENT_CMD) {
=======
	if (count > MAX_STR_LEN_EVENT_CMD) {
>>>>>>> upstream/android-13
		pr_err("%s: size(%zu) is too long.\n", __func__, count);
		goto error;
	}

	cmd = kzalloc(count + 1, GFP_KERNEL);
	if (!cmd)
		goto error;

	ret = sscanf(buf, "%s", cmd);
	if (ret != 1)
		goto error1;

	event_idx = get_event_index_by_command(cmd);

	pr_info("%s: event: %s(%d)\n", __func__, cmd, event_idx);

	ret = ddata->vib_ops->set_event_cmd(ddata->dev, event_idx);
	if (ret)
		pr_err("%s error(%d)\n", __func__, ret);

	sscanf(cmd, "%s", vib_event_cmd);

error1:
	kfree(cmd);
error:
	return count;
}

static DEVICE_ATTR_RW(haptic_engine);
static DEVICE_ATTR_RW(multi_freq);
static DEVICE_ATTR_RW(intensity);
static DEVICE_ATTR_RW(cp_trigger_index);
static DEVICE_ATTR_RW(cp_trigger_queue);
static DEVICE_ATTR_RW(pwle);
static DEVICE_ATTR_RO(virtual_composite_indexes);
static DEVICE_ATTR_RO(virtual_pwle_indexes);
static DEVICE_ATTR_RW(enable);
<<<<<<< HEAD
static DEVICE_ATTR_RW(default_duty);
static DEVICE_ATTR_RW(fold_open_duty);
static DEVICE_ATTR_RW(fold_close_duty);
static DEVICE_ATTR_RO(motor_type);
static DEVICE_ATTR_WO(use_sep_index);
static DEVICE_ATTR_RW(current_temp);
static DEVICE_ATTR_RW(fifo);
static DEVICE_ATTR_RW(hybrid_haptic_engine);
=======
static DEVICE_ATTR_RO(motor_type);
static DEVICE_ATTR_WO(use_sep_index);
>>>>>>> upstream/android-13
static DEVICE_ATTR_RO(num_waves);
static DEVICE_ATTR_RO(intensities);
static DEVICE_ATTR_RO(haptic_intensities);
static DEVICE_ATTR_RO(haptic_durations);
static DEVICE_ATTR_RW(event_cmd);

static struct attribute *sec_vibrator_attributes[] = {
	&dev_attr_enable.attr,
<<<<<<< HEAD
	&dev_attr_default_duty.attr,
	&dev_attr_fold_open_duty.attr,
	&dev_attr_fold_close_duty.attr,
	&dev_attr_motor_type.attr,
	&dev_attr_use_sep_index.attr,
	&dev_attr_current_temp.attr,
	&dev_attr_fifo.attr,
	&dev_attr_hybrid_haptic_engine.attr,
=======
	&dev_attr_motor_type.attr,
	&dev_attr_use_sep_index.attr,
>>>>>>> upstream/android-13
	NULL,
};

static struct attribute_group sec_vibrator_attr_group = {
	.attrs = sec_vibrator_attributes,
};

static struct attribute *multi_freq_attributes[] = {
	&dev_attr_haptic_engine.attr,
	&dev_attr_multi_freq.attr,
	NULL,
};

static struct attribute_group multi_freq_attr_group = {
	.attrs = multi_freq_attributes,
};

static struct attribute *cp_trigger_attributes[] = {
	&dev_attr_haptic_engine.attr,
	&dev_attr_num_waves.attr,
	&dev_attr_cp_trigger_index.attr,
	&dev_attr_cp_trigger_queue.attr,
	NULL,
};

static struct attribute_group cp_trigger_attr_group = {
	.attrs = cp_trigger_attributes,
};

static struct attribute *pwle_attributes[] = {
	&dev_attr_pwle.attr,
	&dev_attr_virtual_composite_indexes.attr,
	&dev_attr_virtual_pwle_indexes.attr,
	NULL,
};

static struct attribute_group pwle_attr_group = {
	.attrs = pwle_attributes,
};

int sec_vibrator_register(struct sec_vibrator_drvdata *ddata)
{
	struct task_struct *kworker_task;
	int ret = 0;

	if (!ddata) {
		pr_err("%s no ddata\n", __func__);
		return -ENODEV;
	}

	g_ddata = ddata;

	mutex_init(&ddata->vib_mutex);
	kthread_init_worker(&ddata->kworker);
	kworker_task = kthread_run(kthread_worker_fn, &ddata->kworker, "sec_vibrator");

	if (IS_ERR(kworker_task)) {
		pr_err("Failed to create message pump task\n");
		ret = -ENOMEM;
		goto err_kthread;
	}

	kthread_init_work(&ddata->kwork, sec_vibrator_work);
	hrtimer_init(&ddata->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	ddata->timer.function = haptic_timer_func;

	/* create /sys/class/timed_output/vibrator */
	ddata->to_class = class_create(THIS_MODULE, "timed_output");
	if (IS_ERR(ddata->to_class)) {
		ret = PTR_ERR(ddata->to_class);
		goto err_class_create;
	}
	ddata->to_dev = device_create(ddata->to_class, NULL, MKDEV(0, 0), ddata, "vibrator");
	if (IS_ERR(ddata->to_dev)) {
		ret = PTR_ERR(ddata->to_dev);
		goto err_device_create;
	}

	ret = sysfs_create_group(&ddata->to_dev->kobj, &sec_vibrator_attr_group);
	if (ret) {
		ret = -ENODEV;
		pr_err("Failed to create sysfs1 %d\n", ret);
		goto err_sysfs1;
	}

	if (ddata->vib_ops->set_intensity) {
		ret = sysfs_create_file(&ddata->to_dev->kobj, &dev_attr_intensity.attr);
		if (ret) {
			ret = -ENODEV;
			pr_err("Failed to create sysfs2 %d\n", ret);
			goto err_sysfs2;
		}

		ddata->intensity = MAX_INTENSITY;
	}

	if (ddata->vib_ops->set_frequency) {
		ret = sysfs_create_group(&ddata->to_dev->kobj, &multi_freq_attr_group);
		if (ret) {
			ret = -ENODEV;
			pr_err("Failed to create sysfs3 %d\n", ret);
			goto err_sysfs3;
		}

		ddata->frequency = FREQ_ALERT;
	}

	if (ddata->vib_ops->set_cp_trigger_index) {
		ret = sysfs_create_group(&ddata->to_dev->kobj, &cp_trigger_attr_group);
		if (ret) {
			ret = -ENODEV;
			pr_err("Failed to create sysfs4 %d\n", ret);
			goto err_sysfs4;
		}

	}

	if (ddata->vib_ops->set_pwle) {
		ret = sysfs_create_group(&ddata->to_dev->kobj, &pwle_attr_group);
		if (ret) {
			ret = -ENODEV;
			pr_err("Failed to create sysfs5 %d\n", ret);
			goto err_sysfs5;
		}

	}

	if (ddata->vib_ops->set_event_cmd) {
		/* initialize event_cmd string for HAL init */
		sscanf(ddata->event_cmd, "%s", vib_event_cmd);
		pr_info("%s: vib_event_cmd: %s\n", __func__, vib_event_cmd);

		ret = sysfs_create_file(&ddata->to_dev->kobj, &dev_attr_event_cmd.attr);
		if (ret) {
			ret = -ENODEV;
			pr_err("Failed to create sysfs6 %d\n", ret);
			goto err_sysfs6;
		}
	}

	if (ddata->vib_ops->get_calibration && ddata->vib_ops->get_calibration(ddata->dev)) {
		if (ddata->vib_ops->get_intensities) {
			ret = sysfs_create_file(&ddata->to_dev->kobj, &dev_attr_intensities.attr);
			if (ret) {
				ret = -ENODEV;
				pr_err("Failed to create intensities %d\n", ret);
				goto err_cal1;
			}
		}

		if (ddata->vib_ops->get_haptic_intensities) {
			ret = sysfs_create_file(&ddata->to_dev->kobj, &dev_attr_haptic_intensities.attr);
			if (ret) {
				ret = -ENODEV;
				pr_err("Failed to create haptic_intensities %d\n", ret);
				goto err_cal2;
			}
		}

		if (ddata->vib_ops->get_haptic_durations) {
			ret = sysfs_create_file(&ddata->to_dev->kobj, &dev_attr_haptic_durations.attr);
			if (ret) {
				ret = -ENODEV;
				pr_err("Failed to create haptic_intensities %d\n", ret);
				goto err_cal3;
			}
		}
	}

<<<<<<< HEAD
	ddata->pdata = &sec_vibrator_pdata;

=======
>>>>>>> upstream/android-13
	pr_info("%s done\n", __func__);

	ddata->is_registered = true;

	return ret;

err_cal3:
	if (ddata->vib_ops->get_calibration && ddata->vib_ops->get_calibration(ddata->dev)) {
		if (ddata->vib_ops->get_haptic_intensities)
			sysfs_remove_file(&ddata->to_dev->kobj, &dev_attr_haptic_intensities.attr);
	}
err_cal2:
	if (ddata->vib_ops->get_calibration && ddata->vib_ops->get_calibration(ddata->dev)) {
		if (ddata->vib_ops->get_intensities)
			sysfs_remove_file(&ddata->to_dev->kobj, &dev_attr_intensities.attr);
	}
err_cal1:
	if (ddata->vib_ops->set_event_cmd)
		sysfs_remove_file(&ddata->to_dev->kobj, &dev_attr_event_cmd.attr);
err_sysfs6:
	if (ddata->vib_ops->set_pwle)
		sysfs_remove_group(&ddata->to_dev->kobj, &pwle_attr_group);
err_sysfs5:
	if (ddata->vib_ops->set_cp_trigger_index)
		sysfs_remove_group(&ddata->to_dev->kobj, &cp_trigger_attr_group);
err_sysfs4:
	if (ddata->vib_ops->set_frequency)
		sysfs_remove_group(&ddata->to_dev->kobj, &multi_freq_attr_group);
err_sysfs3:
	if (ddata->vib_ops->set_intensity)
		sysfs_remove_file(&ddata->to_dev->kobj, &dev_attr_intensity.attr);
err_sysfs2:
	sysfs_remove_group(&ddata->to_dev->kobj, &sec_vibrator_attr_group);
err_sysfs1:
	device_destroy(ddata->to_class, MKDEV(0, 0));
err_device_create:
	class_destroy(ddata->to_class);
err_class_create:
err_kthread:
	mutex_destroy(&ddata->vib_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(sec_vibrator_register);

int sec_vibrator_unregister(struct sec_vibrator_drvdata *ddata)
{
	if (!ddata || !ddata->is_registered) {
		pr_info("%s: sec_vibrator not registered, just return\n", __func__);
		return -ENODEV;
	}

	sec_vibrator_haptic_disable(ddata);
	g_ddata = NULL;

	if (ddata->vib_ops->get_calibration && ddata->vib_ops->get_calibration(ddata->dev)) {
		if (ddata->vib_ops->get_haptic_intensities)
			sysfs_remove_file(&ddata->to_dev->kobj, &dev_attr_haptic_intensities.attr);
		if (ddata->vib_ops->get_intensities)
			sysfs_remove_file(&ddata->to_dev->kobj, &dev_attr_intensities.attr);
	}
	if (ddata->vib_ops->set_event_cmd)
		sysfs_remove_file(&ddata->to_dev->kobj, &dev_attr_event_cmd.attr);
	if (ddata->vib_ops->set_pwle)
		sysfs_remove_group(&ddata->to_dev->kobj, &pwle_attr_group);
	if (ddata->vib_ops->set_cp_trigger_index)
		sysfs_remove_group(&ddata->to_dev->kobj, &cp_trigger_attr_group);
	if (ddata->vib_ops->set_frequency)
		sysfs_remove_group(&ddata->to_dev->kobj, &multi_freq_attr_group);
	if (ddata->vib_ops->set_intensity)
		sysfs_remove_file(&ddata->to_dev->kobj, &dev_attr_intensity.attr);

	sysfs_remove_group(&ddata->to_dev->kobj, &sec_vibrator_attr_group);

	device_destroy(ddata->to_class, MKDEV(0, 0));
	class_destroy(ddata->to_class);
	mutex_destroy(&ddata->vib_mutex);
	return 0;
}
EXPORT_SYMBOL_GPL(sec_vibrator_unregister);

<<<<<<< HEAD
static int sec_vibrator_parse_dt(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	int ret = 0;
	int temp;

	pr_info("%s +++\n", __func__);

	if (unlikely(!np)) {
		pr_err("err: could not parse dt node\n");
		return -ENODEV;
	}

	ret = of_property_read_u32(np, "haptic,normal_ratio", &temp);
	if (ret) {
		pr_err("%s: WARNING! normal_ratio not found\n", __func__);
		sec_vibrator_pdata.normal_ratio = 100;  // 100%
	} else
		sec_vibrator_pdata.normal_ratio = (int)temp;

	ret = of_property_read_u32(np, "haptic,high_temp_ref", &temp);
	if (ret) {
		pr_info("%s: high temperature concept isn't used\n", __func__);
		sec_vibrator_pdata.high_temp_ref = SEC_VIBRATOR_DEFAULT_HIGH_TEMP_REF;
	} else {
		sec_vibrator_pdata.high_temp_ref = (int)temp;

		ret = of_property_read_u32(np, "haptic,high_temp_ratio", &temp);
		if (ret) {
			pr_info("%s: high_temp_ratio isn't used\n", __func__);
			sec_vibrator_pdata.high_temp_ratio = SEC_VIBRATOR_DEFAULT_HIGH_TEMP_RATIO;
		} else
			sec_vibrator_pdata.high_temp_ratio = (int)temp;
	}

	pr_info("%s : done! ---\n", __func__);
	return 0;
}

static int sec_vibrator_probe(struct platform_device *pdev)
{
	pr_info("%s +++\n", __func__);

	if (unlikely(sec_vibrator_parse_dt(pdev)))
		pr_err("%s: WARNING!>..parse dt failed\n", __func__);

	sec_vibrator_pdata.probe_done = true;
	pr_info("%s : done! ---\n", __func__);

	return 0;
}


static const struct of_device_id sec_vibrator_id[] = {
	{ .compatible = "sec_vibrator" },
	{ }
};

static struct platform_driver sec_vibrator_driver = {
	.probe		= sec_vibrator_probe,
	.driver		= {
		.name	=  "sec_vibrator",
		.owner	= THIS_MODULE,
		.of_match_table = sec_vibrator_id,
	},
};

module_platform_driver(sec_vibrator_driver);

=======
>>>>>>> upstream/android-13
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("sec vibrator driver");
