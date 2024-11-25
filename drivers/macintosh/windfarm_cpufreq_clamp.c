<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/wait.h>
=======
#include <linux/pm_qos.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/cpu.h>
>>>>>>> upstream/android-13
#include <linux/cpufreq.h>

#include <asm/prom.h>

#include "windfarm.h"

#define VERSION "0.3"

static int clamped;
static struct wf_control *clamp_control;
<<<<<<< HEAD

static int clamp_notifier_call(struct notifier_block *self,
			       unsigned long event, void *data)
{
	struct cpufreq_policy *p = data;
	unsigned long max_freq;

	if (event != CPUFREQ_ADJUST)
		return 0;

	max_freq = clamped ? (p->cpuinfo.min_freq) : (p->cpuinfo.max_freq);
	cpufreq_verify_within_limits(p, 0, max_freq);

	return 0;
}

static struct notifier_block clamp_notifier = {
	.notifier_call = clamp_notifier_call,
};

static int clamp_set(struct wf_control *ct, s32 value)
{
	if (value)
		printk(KERN_INFO "windfarm: Clamping CPU frequency to "
		       "minimum !\n");
	else
		printk(KERN_INFO "windfarm: CPU frequency unclamped !\n");
	clamped = value;
	cpufreq_update_policy(0);
	return 0;
=======
static struct freq_qos_request qos_req;
static unsigned int min_freq, max_freq;

static int clamp_set(struct wf_control *ct, s32 value)
{
	unsigned int freq;

	if (value) {
		freq = min_freq;
		printk(KERN_INFO "windfarm: Clamping CPU frequency to "
		       "minimum !\n");
	} else {
		freq = max_freq;
		printk(KERN_INFO "windfarm: CPU frequency unclamped !\n");
	}
	clamped = value;

	return freq_qos_update_request(&qos_req, freq);
>>>>>>> upstream/android-13
}

static int clamp_get(struct wf_control *ct, s32 *value)
{
	*value = clamped;
	return 0;
}

static s32 clamp_min(struct wf_control *ct)
{
	return 0;
}

static s32 clamp_max(struct wf_control *ct)
{
	return 1;
}

static const struct wf_control_ops clamp_ops = {
	.set_value	= clamp_set,
	.get_value	= clamp_get,
	.get_min	= clamp_min,
	.get_max	= clamp_max,
	.owner		= THIS_MODULE,
};

static int __init wf_cpufreq_clamp_init(void)
{
<<<<<<< HEAD
	struct wf_control *clamp;

	clamp = kmalloc(sizeof(struct wf_control), GFP_KERNEL);
	if (clamp == NULL)
		return -ENOMEM;
	cpufreq_register_notifier(&clamp_notifier, CPUFREQ_POLICY_NOTIFIER);
	clamp->ops = &clamp_ops;
	clamp->name = "cpufreq-clamp";
	if (wf_register_control(clamp))
		goto fail;
	clamp_control = clamp;
	return 0;
 fail:
	kfree(clamp);
	return -ENODEV;
=======
	struct cpufreq_policy *policy;
	struct wf_control *clamp;
	struct device *dev;
	int ret;

	policy = cpufreq_cpu_get(0);
	if (!policy) {
		pr_warn("%s: cpufreq policy not found cpu0\n", __func__);
		return -EPROBE_DEFER;
	}

	min_freq = policy->cpuinfo.min_freq;
	max_freq = policy->cpuinfo.max_freq;

	ret = freq_qos_add_request(&policy->constraints, &qos_req, FREQ_QOS_MAX,
				   max_freq);

	cpufreq_cpu_put(policy);

	if (ret < 0) {
		pr_err("%s: Failed to add freq constraint (%d)\n", __func__,
		       ret);
		return ret;
	}

	dev = get_cpu_device(0);
	if (unlikely(!dev)) {
		pr_warn("%s: No cpu device for cpu0\n", __func__);
		ret = -ENODEV;
		goto fail;
	}

	clamp = kmalloc(sizeof(struct wf_control), GFP_KERNEL);
	if (clamp == NULL) {
		ret = -ENOMEM;
		goto fail;
	}

	clamp->ops = &clamp_ops;
	clamp->name = "cpufreq-clamp";
	ret = wf_register_control(clamp);
	if (ret)
		goto free;

	clamp_control = clamp;
	return 0;

 free:
	kfree(clamp);
 fail:
	freq_qos_remove_request(&qos_req);
	return ret;
>>>>>>> upstream/android-13
}

static void __exit wf_cpufreq_clamp_exit(void)
{
<<<<<<< HEAD
	if (clamp_control)
		wf_unregister_control(clamp_control);
=======
	if (clamp_control) {
		wf_unregister_control(clamp_control);
		freq_qos_remove_request(&qos_req);
	}
>>>>>>> upstream/android-13
}


module_init(wf_cpufreq_clamp_init);
module_exit(wf_cpufreq_clamp_exit);

MODULE_AUTHOR("Benjamin Herrenschmidt <benh@kernel.crashing.org>");
MODULE_DESCRIPTION("CPU frequency clamp for PowerMacs thermal control");
MODULE_LICENSE("GPL");

