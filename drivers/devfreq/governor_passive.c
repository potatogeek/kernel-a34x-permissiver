<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * linux/drivers/devfreq/governor_passive.c
 *
 * Copyright (C) 2016 Samsung Electronics
 * Author: Chanwoo Choi <cw00.choi@samsung.com>
 * Author: MyungJoo Ham <myungjoo.ham@samsung.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/device.h>
#include <linux/devfreq.h>
#include "governor.h"

static int devfreq_passive_get_target_freq(struct devfreq *devfreq,
					unsigned long *freq)
{
	struct devfreq_passive_data *p_data
			= (struct devfreq_passive_data *)devfreq->data;
	struct devfreq *parent_devfreq = (struct devfreq *)p_data->parent;
	unsigned long child_freq = ULONG_MAX;
<<<<<<< HEAD
	struct dev_pm_opp *opp;
	int i, count, ret = 0;
=======
	struct dev_pm_opp *opp, *p_opp;
	int i, count;
>>>>>>> upstream/android-13

	/*
	 * If the devfreq device with passive governor has the specific method
	 * to determine the next frequency, should use the get_target_freq()
	 * of struct devfreq_passive_data.
	 */
<<<<<<< HEAD
	if (p_data->get_target_freq) {
		ret = p_data->get_target_freq(devfreq, freq);
		goto out;
	}
=======
	if (p_data->get_target_freq)
		return p_data->get_target_freq(devfreq, freq);
>>>>>>> upstream/android-13

	/*
	 * If the parent and passive devfreq device uses the OPP table,
	 * get the next frequency by using the OPP table.
	 */

	/*
	 * - parent devfreq device uses the governors except for passive.
	 * - passive devfreq device uses the passive governor.
	 *
	 * Each devfreq has the OPP table. After deciding the new frequency
	 * from the governor of parent devfreq device, the passive governor
	 * need to get the index of new frequency on OPP table of parent
	 * device. And then the index is used for getting the suitable
	 * new frequency for passive devfreq device.
	 */
	if (!devfreq->profile || !devfreq->profile->freq_table
		|| devfreq->profile->max_state <= 0)
		return -EINVAL;

	/*
	 * The passive governor have to get the correct frequency from OPP
	 * list of parent device. Because in this case, *freq is temporary
	 * value which is decided by ondemand governor.
	 */
<<<<<<< HEAD
	opp = devfreq_recommended_opp(parent_devfreq->dev.parent, freq, 0);
	if (IS_ERR(opp)) {
		ret = PTR_ERR(opp);
		goto out;
	}

	dev_pm_opp_put(opp);

	/*
	 * Get the OPP table's index of decided freqeuncy by governor
=======
	if (devfreq->opp_table && parent_devfreq->opp_table) {
		p_opp = devfreq_recommended_opp(parent_devfreq->dev.parent,
						freq, 0);
		if (IS_ERR(p_opp))
			return PTR_ERR(p_opp);

		opp = dev_pm_opp_xlate_required_opp(parent_devfreq->opp_table,
						    devfreq->opp_table, p_opp);
		dev_pm_opp_put(p_opp);

		if (IS_ERR(opp))
			goto no_required_opp;

		*freq = dev_pm_opp_get_freq(opp);
		dev_pm_opp_put(opp);

		return 0;
	}

no_required_opp:
	/*
	 * Get the OPP table's index of decided frequency by governor
>>>>>>> upstream/android-13
	 * of parent device.
	 */
	for (i = 0; i < parent_devfreq->profile->max_state; i++)
		if (parent_devfreq->profile->freq_table[i] == *freq)
			break;

<<<<<<< HEAD
	if (i == parent_devfreq->profile->max_state) {
		ret = -EINVAL;
		goto out;
	}
=======
	if (i == parent_devfreq->profile->max_state)
		return -EINVAL;
>>>>>>> upstream/android-13

	/* Get the suitable frequency by using index of parent device. */
	if (i < devfreq->profile->max_state) {
		child_freq = devfreq->profile->freq_table[i];
	} else {
		count = devfreq->profile->max_state;
		child_freq = devfreq->profile->freq_table[count - 1];
	}

	/* Return the suitable frequency for passive device. */
	*freq = child_freq;

<<<<<<< HEAD
out:
	return ret;
}

static int update_devfreq_passive(struct devfreq *devfreq, unsigned long freq)
{
	int ret;

	if (!devfreq->governor)
		return -EINVAL;

	mutex_lock_nested(&devfreq->lock, SINGLE_DEPTH_NESTING);

	ret = devfreq->governor->get_target_freq(devfreq, &freq);
	if (ret < 0)
		goto out;

	ret = devfreq->profile->target(devfreq->dev.parent, &freq, 0);
	if (ret < 0)
		goto out;

	if (devfreq->profile->freq_table
		&& (devfreq_update_status(devfreq, freq)))
		dev_err(&devfreq->dev,
			"Couldn't update frequency transition information.\n");

	devfreq->previous_freq = freq;

out:
	mutex_unlock(&devfreq->lock);

=======
>>>>>>> upstream/android-13
	return 0;
}

static int devfreq_passive_notifier_call(struct notifier_block *nb,
				unsigned long event, void *ptr)
{
	struct devfreq_passive_data *data
			= container_of(nb, struct devfreq_passive_data, nb);
	struct devfreq *devfreq = (struct devfreq *)data->this;
	struct devfreq *parent = (struct devfreq *)data->parent;
	struct devfreq_freqs *freqs = (struct devfreq_freqs *)ptr;
	unsigned long freq = freqs->new;
<<<<<<< HEAD

	switch (event) {
	case DEVFREQ_PRECHANGE:
		if (parent->previous_freq > freq)
			update_devfreq_passive(devfreq, freq);
		break;
	case DEVFREQ_POSTCHANGE:
		if (parent->previous_freq < freq)
			update_devfreq_passive(devfreq, freq);
		break;
	}
=======
	int ret = 0;

	mutex_lock_nested(&devfreq->lock, SINGLE_DEPTH_NESTING);
	switch (event) {
	case DEVFREQ_PRECHANGE:
		if (parent->previous_freq > freq)
			ret = devfreq_update_target(devfreq, freq);

		break;
	case DEVFREQ_POSTCHANGE:
		if (parent->previous_freq < freq)
			ret = devfreq_update_target(devfreq, freq);
		break;
	}
	mutex_unlock(&devfreq->lock);

	if (ret < 0)
		dev_warn(&devfreq->dev,
			"failed to update devfreq using passive governor\n");
>>>>>>> upstream/android-13

	return NOTIFY_DONE;
}

static int devfreq_passive_event_handler(struct devfreq *devfreq,
				unsigned int event, void *data)
{
	struct devfreq_passive_data *p_data
			= (struct devfreq_passive_data *)devfreq->data;
	struct devfreq *parent = (struct devfreq *)p_data->parent;
	struct notifier_block *nb = &p_data->nb;
	int ret = 0;

	if (!parent)
		return -EPROBE_DEFER;

	switch (event) {
	case DEVFREQ_GOV_START:
		if (!p_data->this)
			p_data->this = devfreq;

		nb->notifier_call = devfreq_passive_notifier_call;
		ret = devfreq_register_notifier(parent, nb,
					DEVFREQ_TRANSITION_NOTIFIER);
		break;
	case DEVFREQ_GOV_STOP:
		WARN_ON(devfreq_unregister_notifier(parent, nb,
					DEVFREQ_TRANSITION_NOTIFIER));
		break;
	default:
		break;
	}

	return ret;
}

static struct devfreq_governor devfreq_passive = {
	.name = DEVFREQ_GOV_PASSIVE,
<<<<<<< HEAD
	.immutable = 1,
=======
	.flags = DEVFREQ_GOV_FLAG_IMMUTABLE,
>>>>>>> upstream/android-13
	.get_target_freq = devfreq_passive_get_target_freq,
	.event_handler = devfreq_passive_event_handler,
};

static int __init devfreq_passive_init(void)
{
	return devfreq_add_governor(&devfreq_passive);
}
subsys_initcall(devfreq_passive_init);

static void __exit devfreq_passive_exit(void)
{
	int ret;

	ret = devfreq_remove_governor(&devfreq_passive);
	if (ret)
		pr_err("%s: failed remove governor %d\n", __func__, ret);
}
module_exit(devfreq_passive_exit);

MODULE_AUTHOR("Chanwoo Choi <cw00.choi@samsung.com>");
MODULE_AUTHOR("MyungJoo Ham <myungjoo.ham@samsung.com>");
MODULE_DESCRIPTION("DEVFREQ Passive governor");
MODULE_LICENSE("GPL v2");
