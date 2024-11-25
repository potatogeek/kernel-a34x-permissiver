<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * pmi backend for the cbe_cpufreq driver
 *
 * (C) Copyright IBM Deutschland Entwicklung GmbH 2005-2007
 *
 * Author: Christian Krafft <krafft@de.ibm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/of_platform.h>
<<<<<<< HEAD
=======
#include <linux/pm_qos.h>
>>>>>>> upstream/android-13

#include <asm/processor.h>
#include <asm/prom.h>
#include <asm/pmi.h>
#include <asm/cell-regs.h>

#ifdef DEBUG
#include <asm/time.h>
#endif

#include "ppc_cbe_cpufreq.h"

<<<<<<< HEAD
static u8 pmi_slow_mode_limit[MAX_CBE];

=======
>>>>>>> upstream/android-13
bool cbe_cpufreq_has_pmi = false;
EXPORT_SYMBOL_GPL(cbe_cpufreq_has_pmi);

/*
 * hardware specific functions
 */

int cbe_cpufreq_set_pmode_pmi(int cpu, unsigned int pmode)
{
	int ret;
	pmi_message_t pmi_msg;
#ifdef DEBUG
	long time;
#endif
	pmi_msg.type = PMI_TYPE_FREQ_CHANGE;
	pmi_msg.data1 =	cbe_cpu_to_node(cpu);
	pmi_msg.data2 = pmode;

#ifdef DEBUG
	time = jiffies;
#endif
	pmi_send_message(pmi_msg);

#ifdef DEBUG
	time = jiffies  - time;
	time = jiffies_to_msecs(time);
	pr_debug("had to wait %lu ms for a transition using " \
		 "PMI\n", time);
#endif
	ret = pmi_msg.data2;
	pr_debug("PMI returned slow mode %d\n", ret);

	return ret;
}
EXPORT_SYMBOL_GPL(cbe_cpufreq_set_pmode_pmi);


static void cbe_cpufreq_handle_pmi(pmi_message_t pmi_msg)
{
<<<<<<< HEAD
	u8 node, slow_mode;
=======
	struct cpufreq_policy *policy;
	struct freq_qos_request *req;
	u8 node, slow_mode;
	int cpu, ret;
>>>>>>> upstream/android-13

	BUG_ON(pmi_msg.type != PMI_TYPE_FREQ_CHANGE);

	node = pmi_msg.data1;
	slow_mode = pmi_msg.data2;

<<<<<<< HEAD
	pmi_slow_mode_limit[node] = slow_mode;

	pr_debug("cbe_handle_pmi: node: %d max_freq: %d\n", node, slow_mode);
}

static int pmi_notifier(struct notifier_block *nb,
				       unsigned long event, void *data)
{
	struct cpufreq_policy *policy = data;
	struct cpufreq_frequency_table *cbe_freqs = policy->freq_table;
	u8 node;

	/* Should this really be called for CPUFREQ_ADJUST and CPUFREQ_NOTIFY
	 * policy events?)
	 */
	node = cbe_cpu_to_node(policy->cpu);

	pr_debug("got notified, event=%lu, node=%u\n", event, node);

	if (pmi_slow_mode_limit[node] != 0) {
		pr_debug("limiting node %d to slow mode %d\n",
			 node, pmi_slow_mode_limit[node]);

		cpufreq_verify_within_limits(policy, 0,

			cbe_freqs[pmi_slow_mode_limit[node]].frequency);
	}

	return 0;
}

static struct notifier_block pmi_notifier_block = {
	.notifier_call = pmi_notifier,
};
=======
	cpu = cbe_node_to_cpu(node);

	pr_debug("cbe_handle_pmi: node: %d max_freq: %d\n", node, slow_mode);

	policy = cpufreq_cpu_get(cpu);
	if (!policy) {
		pr_warn("cpufreq policy not found cpu%d\n", cpu);
		return;
	}

	req = policy->driver_data;

	ret = freq_qos_update_request(req,
			policy->freq_table[slow_mode].frequency);
	if (ret < 0)
		pr_warn("Failed to update freq constraint: %d\n", ret);
	else
		pr_debug("limiting node %d to slow mode %d\n", node, slow_mode);

	cpufreq_cpu_put(policy);
}
>>>>>>> upstream/android-13

static struct pmi_handler cbe_pmi_handler = {
	.type			= PMI_TYPE_FREQ_CHANGE,
	.handle_pmi_message	= cbe_cpufreq_handle_pmi,
};

<<<<<<< HEAD


static int __init cbe_cpufreq_pmi_init(void)
{
	cbe_cpufreq_has_pmi = pmi_register_handler(&cbe_pmi_handler) == 0;

	if (!cbe_cpufreq_has_pmi)
		return -ENODEV;

	cpufreq_register_notifier(&pmi_notifier_block, CPUFREQ_POLICY_NOTIFIER);

	return 0;
}
device_initcall(cbe_cpufreq_pmi_init);
=======
void cbe_cpufreq_pmi_policy_init(struct cpufreq_policy *policy)
{
	struct freq_qos_request *req;
	int ret;

	if (!cbe_cpufreq_has_pmi)
		return;

	req = kzalloc(sizeof(*req), GFP_KERNEL);
	if (!req)
		return;

	ret = freq_qos_add_request(&policy->constraints, req, FREQ_QOS_MAX,
				   policy->freq_table[0].frequency);
	if (ret < 0) {
		pr_err("Failed to add freq constraint (%d)\n", ret);
		kfree(req);
		return;
	}

	policy->driver_data = req;
}
EXPORT_SYMBOL_GPL(cbe_cpufreq_pmi_policy_init);

void cbe_cpufreq_pmi_policy_exit(struct cpufreq_policy *policy)
{
	struct freq_qos_request *req = policy->driver_data;

	if (cbe_cpufreq_has_pmi) {
		freq_qos_remove_request(req);
		kfree(req);
	}
}
EXPORT_SYMBOL_GPL(cbe_cpufreq_pmi_policy_exit);

void cbe_cpufreq_pmi_init(void)
{
	if (!pmi_register_handler(&cbe_pmi_handler))
		cbe_cpufreq_has_pmi = true;
}
EXPORT_SYMBOL_GPL(cbe_cpufreq_pmi_init);

void cbe_cpufreq_pmi_exit(void)
{
	pmi_unregister_handler(&cbe_pmi_handler);
	cbe_cpufreq_has_pmi = false;
}
EXPORT_SYMBOL_GPL(cbe_cpufreq_pmi_exit);
>>>>>>> upstream/android-13
