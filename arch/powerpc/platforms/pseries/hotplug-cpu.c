<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * pseries CPU Hotplug infrastructure.
 *
 * Split out from arch/powerpc/platforms/pseries/setup.c
 *  arch/powerpc/kernel/rtas.c, and arch/powerpc/platforms/pseries/smp.c
 *
 * Peter Bergner, IBM	March 2001.
 * Copyright (C) 2001 IBM.
 * Dave Engebretsen, Peter Bergner, and
 * Mike Corrigan {engebret|bergner|mikec}@us.ibm.com
 * Plus various changes from other IBM teams...
 *
 * Copyright (C) 2006 Michael Ellerman, IBM Corporation
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt)     "pseries-hotplug-cpu: " fmt

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/sched.h>	/* for idle_task_exit */
#include <linux/sched/hotplug.h>
#include <linux/cpu.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <asm/prom.h>
#include <asm/rtas.h>
#include <asm/firmware.h>
#include <asm/machdep.h>
#include <asm/vdso_datapage.h>
#include <asm/xics.h>
#include <asm/xive.h>
#include <asm/plpar_wrappers.h>
#include <asm/topology.h>

#include "pseries.h"
<<<<<<< HEAD
#include "offline_states.h"
=======
>>>>>>> upstream/android-13

/* This version can't take the spinlock, because it never returns */
static int rtas_stop_self_token = RTAS_UNKNOWN_SERVICE;

<<<<<<< HEAD
static DEFINE_PER_CPU(enum cpu_state_vals, preferred_offline_state) =
							CPU_STATE_OFFLINE;
static DEFINE_PER_CPU(enum cpu_state_vals, current_state) = CPU_STATE_OFFLINE;

static enum cpu_state_vals default_offline_state = CPU_STATE_OFFLINE;

static bool cede_offline_enabled __read_mostly = true;

/*
 * Enable/disable cede_offline when available.
 */
static int __init setup_cede_offline(char *str)
{
	return (kstrtobool(str, &cede_offline_enabled) == 0);
}

__setup("cede_offline=", setup_cede_offline);

enum cpu_state_vals get_cpu_current_state(int cpu)
{
	return per_cpu(current_state, cpu);
}

void set_cpu_current_state(int cpu, enum cpu_state_vals state)
{
	per_cpu(current_state, cpu) = state;
}

enum cpu_state_vals get_preferred_offline_state(int cpu)
{
	return per_cpu(preferred_offline_state, cpu);
}

void set_preferred_offline_state(int cpu, enum cpu_state_vals state)
{
	per_cpu(preferred_offline_state, cpu) = state;
}

void set_default_offline_state(int cpu)
{
	per_cpu(preferred_offline_state, cpu) = default_offline_state;
}
=======
/*
 * Record the CPU ids used on each nodes.
 * Protected by cpu_add_remove_lock.
 */
static cpumask_var_t node_recorded_ids_map[MAX_NUMNODES];
>>>>>>> upstream/android-13

static void rtas_stop_self(void)
{
	static struct rtas_args args;

	local_irq_disable();

	BUG_ON(rtas_stop_self_token == RTAS_UNKNOWN_SERVICE);

	rtas_call_unlocked(&args, rtas_stop_self_token, 0, 1, NULL);

	panic("Alas, I survived.\n");
}

<<<<<<< HEAD
static void pseries_mach_cpu_die(void)
{
	unsigned int cpu = smp_processor_id();
	unsigned int hwcpu = hard_smp_processor_id();
	u8 cede_latency_hint = 0;
=======
static void pseries_cpu_offline_self(void)
{
	unsigned int hwcpu = hard_smp_processor_id();
>>>>>>> upstream/android-13

	local_irq_disable();
	idle_task_exit();
	if (xive_enabled())
		xive_teardown_cpu();
	else
		xics_teardown_cpu();

<<<<<<< HEAD
	if (get_preferred_offline_state(cpu) == CPU_STATE_INACTIVE) {
		set_cpu_current_state(cpu, CPU_STATE_INACTIVE);
		if (ppc_md.suspend_disable_cpu)
			ppc_md.suspend_disable_cpu();

		cede_latency_hint = 2;

		get_lppaca()->idle = 1;
		if (!lppaca_shared_proc(get_lppaca()))
			get_lppaca()->donate_dedicated_cpu = 1;

		while (get_preferred_offline_state(cpu) == CPU_STATE_INACTIVE) {
			while (!prep_irq_for_idle()) {
				local_irq_enable();
				local_irq_disable();
			}

			extended_cede_processor(cede_latency_hint);
		}

		local_irq_disable();

		if (!lppaca_shared_proc(get_lppaca()))
			get_lppaca()->donate_dedicated_cpu = 0;
		get_lppaca()->idle = 0;

		if (get_preferred_offline_state(cpu) == CPU_STATE_ONLINE) {
			unregister_slb_shadow(hwcpu);

			hard_irq_disable();
			/*
			 * Call to start_secondary_resume() will not return.
			 * Kernel stack will be reset and start_secondary()
			 * will be called to continue the online operation.
			 */
			start_secondary_resume();
		}
	}

	/* Requested state is CPU_STATE_OFFLINE at this point */
	WARN_ON(get_preferred_offline_state(cpu) != CPU_STATE_OFFLINE);

	set_cpu_current_state(cpu, CPU_STATE_OFFLINE);
=======
>>>>>>> upstream/android-13
	unregister_slb_shadow(hwcpu);
	rtas_stop_self();

	/* Should never get here... */
	BUG();
	for(;;);
}

static int pseries_cpu_disable(void)
{
	int cpu = smp_processor_id();

	set_cpu_online(cpu, false);
	vdso_data->processorCount--;

	/*fix boot_cpuid here*/
	if (cpu == boot_cpuid)
		boot_cpuid = cpumask_any(cpu_online_mask);

	/* FIXME: abstract this to not be platform specific later on */
	if (xive_enabled())
		xive_smp_disable_cpu();
	else
		xics_migrate_irqs_away();
<<<<<<< HEAD
=======

	cleanup_cpu_mmu_context();

>>>>>>> upstream/android-13
	return 0;
}

/*
 * pseries_cpu_die: Wait for the cpu to die.
 * @cpu: logical processor id of the CPU whose death we're awaiting.
 *
 * This function is called from the context of the thread which is performing
 * the cpu-offline. Here we wait for long enough to allow the cpu in question
 * to self-destroy so that the cpu-offline thread can send the CPU_DEAD
 * notifications.
 *
<<<<<<< HEAD
 * OTOH, pseries_mach_cpu_die() is called by the @cpu when it wants to
=======
 * OTOH, pseries_cpu_offline_self() is called by the @cpu when it wants to
>>>>>>> upstream/android-13
 * self-destruct.
 */
static void pseries_cpu_die(unsigned int cpu)
{
<<<<<<< HEAD
	int tries;
	int cpu_status = 1;
	unsigned int pcpu = get_hard_smp_processor_id(cpu);

	if (get_preferred_offline_state(cpu) == CPU_STATE_INACTIVE) {
		cpu_status = 1;
		for (tries = 0; tries < 5000; tries++) {
			if (get_cpu_current_state(cpu) == CPU_STATE_INACTIVE) {
				cpu_status = 0;
				break;
			}
			msleep(1);
		}
	} else if (get_preferred_offline_state(cpu) == CPU_STATE_OFFLINE) {

		for (tries = 0; tries < 25; tries++) {
			cpu_status = smp_query_cpu_stopped(pcpu);
			if (cpu_status == QCSS_STOPPED ||
			    cpu_status == QCSS_HARDWARE_ERROR)
				break;
			cpu_relax();
		}
	}

	if (cpu_status != 0) {
		printk("Querying DEAD? cpu %i (%i) shows %i\n",
		       cpu, pcpu, cpu_status);
=======
	int cpu_status = 1;
	unsigned int pcpu = get_hard_smp_processor_id(cpu);
	unsigned long timeout = jiffies + msecs_to_jiffies(120000);

	while (true) {
		cpu_status = smp_query_cpu_stopped(pcpu);
		if (cpu_status == QCSS_STOPPED ||
		    cpu_status == QCSS_HARDWARE_ERROR)
			break;

		if (time_after(jiffies, timeout)) {
			pr_warn("CPU %i (hwid %i) didn't die after 120 seconds\n",
				cpu, pcpu);
			timeout = jiffies + msecs_to_jiffies(120000);
		}

		cond_resched();
	}

	if (cpu_status == QCSS_HARDWARE_ERROR) {
		pr_warn("CPU %i (hwid %i) reported error while dying\n",
			cpu, pcpu);
>>>>>>> upstream/android-13
	}

	/* Isolation and deallocation are definitely done by
	 * drslot_chrp_cpu.  If they were not they would be
	 * done here.  Change isolate state to Isolate and
	 * change allocation-state to Unusable.
	 */
	paca_ptrs[cpu]->cpu_start = 0;
}

<<<<<<< HEAD
/*
 * Update cpu_present_mask and paca(s) for a new cpu node.  The wrinkle
 * here is that a cpu device node may represent up to two logical cpus
=======
/**
 * find_cpu_id_range - found a linear ranger of @nthreads free CPU ids.
 * @nthreads : the number of threads (cpu ids)
 * @assigned_node : the node it belongs to or NUMA_NO_NODE if free ids from any
 *                  node can be peek.
 * @cpu_mask: the returned CPU mask.
 *
 * Returns 0 on success.
 */
static int find_cpu_id_range(unsigned int nthreads, int assigned_node,
			     cpumask_var_t *cpu_mask)
{
	cpumask_var_t candidate_mask;
	unsigned int cpu, node;
	int rc = -ENOSPC;

	if (!zalloc_cpumask_var(&candidate_mask, GFP_KERNEL))
		return -ENOMEM;

	cpumask_clear(*cpu_mask);
	for (cpu = 0; cpu < nthreads; cpu++)
		cpumask_set_cpu(cpu, *cpu_mask);

	BUG_ON(!cpumask_subset(cpu_present_mask, cpu_possible_mask));

	/* Get a bitmap of unoccupied slots. */
	cpumask_xor(candidate_mask, cpu_possible_mask, cpu_present_mask);

	if (assigned_node != NUMA_NO_NODE) {
		/*
		 * Remove free ids previously assigned on the other nodes. We
		 * can walk only online nodes because once a node became online
		 * it is not turned offlined back.
		 */
		for_each_online_node(node) {
			if (node == assigned_node)
				continue;
			cpumask_andnot(candidate_mask, candidate_mask,
				       node_recorded_ids_map[node]);
		}
	}

	if (cpumask_empty(candidate_mask))
		goto out;

	while (!cpumask_empty(*cpu_mask)) {
		if (cpumask_subset(*cpu_mask, candidate_mask))
			/* Found a range where we can insert the new cpu(s) */
			break;
		cpumask_shift_left(*cpu_mask, *cpu_mask, nthreads);
	}

	if (!cpumask_empty(*cpu_mask))
		rc = 0;

out:
	free_cpumask_var(candidate_mask);
	return rc;
}

/*
 * Update cpu_present_mask and paca(s) for a new cpu node.  The wrinkle
 * here is that a cpu device node may represent multiple logical cpus
>>>>>>> upstream/android-13
 * in the SMT case.  We must honor the assumption in other code that
 * the logical ids for sibling SMT threads x and y are adjacent, such
 * that x^1 == y and y^1 == x.
 */
static int pseries_add_processor(struct device_node *np)
{
<<<<<<< HEAD
	unsigned int cpu;
	cpumask_var_t candidate_mask, tmp;
	int err = -ENOSPC, len, nthreads, i;
=======
	int len, nthreads, node, cpu, assigned_node;
	int rc = 0;
	cpumask_var_t cpu_mask;
>>>>>>> upstream/android-13
	const __be32 *intserv;

	intserv = of_get_property(np, "ibm,ppc-interrupt-server#s", &len);
	if (!intserv)
		return 0;

<<<<<<< HEAD
	zalloc_cpumask_var(&candidate_mask, GFP_KERNEL);
	zalloc_cpumask_var(&tmp, GFP_KERNEL);

	nthreads = len / sizeof(u32);
	for (i = 0; i < nthreads; i++)
		cpumask_set_cpu(i, tmp);

	cpu_maps_update_begin();

	BUG_ON(!cpumask_subset(cpu_present_mask, cpu_possible_mask));

	/* Get a bitmap of unoccupied slots. */
	cpumask_xor(candidate_mask, cpu_possible_mask, cpu_present_mask);
	if (cpumask_empty(candidate_mask)) {
		/* If we get here, it most likely means that NR_CPUS is
		 * less than the partition's max processors setting.
		 */
		printk(KERN_ERR "Cannot add cpu %pOF; this system configuration"
		       " supports %d logical cpus.\n", np,
		       num_possible_cpus());
		goto out_unlock;
	}

	while (!cpumask_empty(tmp))
		if (cpumask_subset(tmp, candidate_mask))
			/* Found a range where we can insert the new cpu(s) */
			break;
		else
			cpumask_shift_left(tmp, tmp, nthreads);

	if (cpumask_empty(tmp)) {
		printk(KERN_ERR "Unable to find space in cpu_present_mask for"
		       " processor %s with %d thread(s)\n", np->name,
		       nthreads);
		goto out_unlock;
	}

	for_each_cpu(cpu, tmp) {
=======
	nthreads = len / sizeof(u32);

	if (!alloc_cpumask_var(&cpu_mask, GFP_KERNEL))
		return -ENOMEM;

	/*
	 * Fetch from the DT nodes read by dlpar_configure_connector() the NUMA
	 * node id the added CPU belongs to.
	 */
	node = of_node_to_nid(np);
	if (node < 0 || !node_possible(node))
		node = first_online_node;

	BUG_ON(node == NUMA_NO_NODE);
	assigned_node = node;

	cpu_maps_update_begin();

	rc = find_cpu_id_range(nthreads, node, &cpu_mask);
	if (rc && nr_node_ids > 1) {
		/*
		 * Try again, considering the free CPU ids from the other node.
		 */
		node = NUMA_NO_NODE;
		rc = find_cpu_id_range(nthreads, NUMA_NO_NODE, &cpu_mask);
	}

	if (rc) {
		pr_err("Cannot add cpu %pOF; this system configuration"
		       " supports %d logical cpus.\n", np, num_possible_cpus());
		goto out;
	}

	for_each_cpu(cpu, cpu_mask) {
>>>>>>> upstream/android-13
		BUG_ON(cpu_present(cpu));
		set_cpu_present(cpu, true);
		set_hard_smp_processor_id(cpu, be32_to_cpu(*intserv++));
	}
<<<<<<< HEAD
	err = 0;
out_unlock:
	cpu_maps_update_done();
	free_cpumask_var(candidate_mask);
	free_cpumask_var(tmp);
	return err;
=======

	/* Record the newly used CPU ids for the associate node. */
	cpumask_or(node_recorded_ids_map[assigned_node],
		   node_recorded_ids_map[assigned_node], cpu_mask);

	/*
	 * If node is set to NUMA_NO_NODE, CPU ids have be reused from
	 * another node, remove them from its mask.
	 */
	if (node == NUMA_NO_NODE) {
		cpu = cpumask_first(cpu_mask);
		pr_warn("Reusing free CPU ids %d-%d from another node\n",
			cpu, cpu + nthreads - 1);
		for_each_online_node(node) {
			if (node == assigned_node)
				continue;
			cpumask_andnot(node_recorded_ids_map[node],
				       node_recorded_ids_map[node],
				       cpu_mask);
		}
	}

out:
	cpu_maps_update_done();
	free_cpumask_var(cpu_mask);
	return rc;
>>>>>>> upstream/android-13
}

/*
 * Update the present map for a cpu node which is going away, and set
 * the hard id in the paca(s) to -1 to be consistent with boot time
 * convention for non-present cpus.
 */
static void pseries_remove_processor(struct device_node *np)
{
	unsigned int cpu;
	int len, nthreads, i;
	const __be32 *intserv;
	u32 thread;

	intserv = of_get_property(np, "ibm,ppc-interrupt-server#s", &len);
	if (!intserv)
		return;

	nthreads = len / sizeof(u32);

	cpu_maps_update_begin();
	for (i = 0; i < nthreads; i++) {
		thread = be32_to_cpu(intserv[i]);
		for_each_present_cpu(cpu) {
			if (get_hard_smp_processor_id(cpu) != thread)
				continue;
			BUG_ON(cpu_online(cpu));
			set_cpu_present(cpu, false);
			set_hard_smp_processor_id(cpu, -1);
			update_numa_cpu_lookup_table(cpu, -1);
			break;
		}
		if (cpu >= nr_cpu_ids)
			printk(KERN_WARNING "Could not find cpu to remove "
			       "with physical id 0x%x\n", thread);
	}
	cpu_maps_update_done();
}

<<<<<<< HEAD
=======
static int dlpar_offline_cpu(struct device_node *dn)
{
	int rc = 0;
	unsigned int cpu;
	int len, nthreads, i;
	const __be32 *intserv;
	u32 thread;

	intserv = of_get_property(dn, "ibm,ppc-interrupt-server#s", &len);
	if (!intserv)
		return -EINVAL;

	nthreads = len / sizeof(u32);

	cpu_maps_update_begin();
	for (i = 0; i < nthreads; i++) {
		thread = be32_to_cpu(intserv[i]);
		for_each_present_cpu(cpu) {
			if (get_hard_smp_processor_id(cpu) != thread)
				continue;

			if (!cpu_online(cpu))
				break;

			/*
			 * device_offline() will return -EBUSY (via cpu_down()) if there
			 * is only one CPU left. Check it here to fail earlier and with a
			 * more informative error message, while also retaining the
			 * cpu_add_remove_lock to be sure that no CPUs are being
			 * online/offlined during this check.
			 */
			if (num_online_cpus() == 1) {
				pr_warn("Unable to remove last online CPU %pOFn\n", dn);
				rc = -EBUSY;
				goto out_unlock;
			}

			cpu_maps_update_done();
			rc = device_offline(get_cpu_device(cpu));
			if (rc)
				goto out;
			cpu_maps_update_begin();
			break;
		}
		if (cpu == num_possible_cpus()) {
			pr_warn("Could not find cpu to offline with physical id 0x%x\n",
				thread);
		}
	}
out_unlock:
	cpu_maps_update_done();

out:
	return rc;
}

>>>>>>> upstream/android-13
static int dlpar_online_cpu(struct device_node *dn)
{
	int rc = 0;
	unsigned int cpu;
	int len, nthreads, i;
	const __be32 *intserv;
	u32 thread;

	intserv = of_get_property(dn, "ibm,ppc-interrupt-server#s", &len);
	if (!intserv)
		return -EINVAL;

	nthreads = len / sizeof(u32);

	cpu_maps_update_begin();
	for (i = 0; i < nthreads; i++) {
		thread = be32_to_cpu(intserv[i]);
		for_each_present_cpu(cpu) {
			if (get_hard_smp_processor_id(cpu) != thread)
				continue;
<<<<<<< HEAD
			BUG_ON(get_cpu_current_state(cpu)
					!= CPU_STATE_OFFLINE);
			cpu_maps_update_done();
			timed_topology_update(1);
			find_and_online_cpu_nid(cpu);
			rc = device_online(get_cpu_device(cpu));
			if (rc)
				goto out;
=======
			cpu_maps_update_done();
			find_and_online_cpu_nid(cpu);
			rc = device_online(get_cpu_device(cpu));
			if (rc) {
				dlpar_offline_cpu(dn);
				goto out;
			}
>>>>>>> upstream/android-13
			cpu_maps_update_begin();

			break;
		}
		if (cpu == num_possible_cpus())
			printk(KERN_WARNING "Could not find cpu to online "
			       "with physical id 0x%x\n", thread);
	}
	cpu_maps_update_done();

out:
	return rc;

}

static bool dlpar_cpu_exists(struct device_node *parent, u32 drc_index)
{
	struct device_node *child = NULL;
	u32 my_drc_index;
	bool found;
	int rc;

	/* Assume cpu doesn't exist */
	found = false;

	for_each_child_of_node(parent, child) {
		rc = of_property_read_u32(child, "ibm,my-drc-index",
					  &my_drc_index);
		if (rc)
			continue;

		if (my_drc_index == drc_index) {
			of_node_put(child);
			found = true;
			break;
		}
	}

	return found;
}

<<<<<<< HEAD
=======
static bool drc_info_valid_index(struct device_node *parent, u32 drc_index)
{
	struct property *info;
	struct of_drc_info drc;
	const __be32 *value;
	u32 index;
	int count, i, j;

	info = of_find_property(parent, "ibm,drc-info", NULL);
	if (!info)
		return false;

	value = of_prop_next_u32(info, NULL, &count);

	/* First value of ibm,drc-info is number of drc-info records */
	if (value)
		value++;
	else
		return false;

	for (i = 0; i < count; i++) {
		if (of_read_drc_info_cell(&info, &value, &drc))
			return false;

		if (strncmp(drc.drc_type, "CPU", 3))
			break;

		if (drc_index > drc.last_drc_index)
			continue;

		index = drc.drc_index_start;
		for (j = 0; j < drc.num_sequential_elems; j++) {
			if (drc_index == index)
				return true;

			index += drc.sequential_inc;
		}
	}

	return false;
}

>>>>>>> upstream/android-13
static bool valid_cpu_drc_index(struct device_node *parent, u32 drc_index)
{
	bool found = false;
	int rc, index;

<<<<<<< HEAD
	index = 0;
=======
	if (of_find_property(parent, "ibm,drc-info", NULL))
		return drc_info_valid_index(parent, drc_index);

	/* Note that the format of the ibm,drc-indexes array is
	 * the number of entries in the array followed by the array
	 * of drc values so we start looking at index = 1.
	 */
	index = 1;
>>>>>>> upstream/android-13
	while (!found) {
		u32 drc;

		rc = of_property_read_u32_index(parent, "ibm,drc-indexes",
						index++, &drc);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
		if (rc)
			break;

		if (drc == drc_index)
			found = true;
	}

	return found;
}

static ssize_t dlpar_cpu_add(u32 drc_index)
{
	struct device_node *dn, *parent;
	int rc, saved_rc;

	pr_debug("Attempting to add CPU, drc index: %x\n", drc_index);

	parent = of_find_node_by_path("/cpus");
	if (!parent) {
		pr_warn("Failed to find CPU root node \"/cpus\"\n");
		return -ENODEV;
	}

	if (dlpar_cpu_exists(parent, drc_index)) {
		of_node_put(parent);
		pr_warn("CPU with drc index %x already exists\n", drc_index);
		return -EINVAL;
	}

	if (!valid_cpu_drc_index(parent, drc_index)) {
		of_node_put(parent);
		pr_warn("Cannot find CPU (drc index %x) to add.\n", drc_index);
		return -EINVAL;
	}

	rc = dlpar_acquire_drc(drc_index);
	if (rc) {
		pr_warn("Failed to acquire DRC, rc: %d, drc index: %x\n",
			rc, drc_index);
		of_node_put(parent);
		return -EINVAL;
	}

	dn = dlpar_configure_connector(cpu_to_be32(drc_index), parent);
	if (!dn) {
		pr_warn("Failed call to configure-connector, drc index: %x\n",
			drc_index);
		dlpar_release_drc(drc_index);
		of_node_put(parent);
		return -EINVAL;
	}

	rc = dlpar_attach_node(dn, parent);

	/* Regardless we are done with parent now */
	of_node_put(parent);

	if (rc) {
		saved_rc = rc;
<<<<<<< HEAD
		pr_warn("Failed to attach node %s, rc: %d, drc index: %x\n",
			dn->name, rc, drc_index);
=======
		pr_warn("Failed to attach node %pOFn, rc: %d, drc index: %x\n",
			dn, rc, drc_index);
>>>>>>> upstream/android-13

		rc = dlpar_release_drc(drc_index);
		if (!rc)
			dlpar_free_cc_nodes(dn);

		return saved_rc;
	}

<<<<<<< HEAD
	rc = dlpar_online_cpu(dn);
	if (rc) {
		saved_rc = rc;
		pr_warn("Failed to online cpu %s, rc: %d, drc index: %x\n",
			dn->name, rc, drc_index);
=======
	update_numa_distance(dn);

	rc = dlpar_online_cpu(dn);
	if (rc) {
		saved_rc = rc;
		pr_warn("Failed to online cpu %pOFn, rc: %d, drc index: %x\n",
			dn, rc, drc_index);
>>>>>>> upstream/android-13

		rc = dlpar_detach_node(dn);
		if (!rc)
			dlpar_release_drc(drc_index);

		return saved_rc;
	}

<<<<<<< HEAD
	pr_debug("Successfully added CPU %s, drc index: %x\n", dn->name,
=======
	pr_debug("Successfully added CPU %pOFn, drc index: %x\n", dn,
>>>>>>> upstream/android-13
		 drc_index);
	return rc;
}

<<<<<<< HEAD
static int dlpar_offline_cpu(struct device_node *dn)
{
	int rc = 0;
	unsigned int cpu;
	int len, nthreads, i;
	const __be32 *intserv;
	u32 thread;

	intserv = of_get_property(dn, "ibm,ppc-interrupt-server#s", &len);
	if (!intserv)
		return -EINVAL;

	nthreads = len / sizeof(u32);

	cpu_maps_update_begin();
	for (i = 0; i < nthreads; i++) {
		thread = be32_to_cpu(intserv[i]);
		for_each_present_cpu(cpu) {
			if (get_hard_smp_processor_id(cpu) != thread)
				continue;

			if (get_cpu_current_state(cpu) == CPU_STATE_OFFLINE)
				break;

			if (get_cpu_current_state(cpu) == CPU_STATE_ONLINE) {
				set_preferred_offline_state(cpu,
							    CPU_STATE_OFFLINE);
				cpu_maps_update_done();
				timed_topology_update(1);
				rc = device_offline(get_cpu_device(cpu));
				if (rc)
					goto out;
				cpu_maps_update_begin();
				break;

			}

			/*
			 * The cpu is in CPU_STATE_INACTIVE.
			 * Upgrade it's state to CPU_STATE_OFFLINE.
			 */
			set_preferred_offline_state(cpu, CPU_STATE_OFFLINE);
			BUG_ON(plpar_hcall_norets(H_PROD, thread)
								!= H_SUCCESS);
			__cpu_die(cpu);
			break;
		}
		if (cpu == num_possible_cpus())
			printk(KERN_WARNING "Could not find cpu to offline with physical id 0x%x\n", thread);
	}
	cpu_maps_update_done();

out:
	return rc;

}

=======
>>>>>>> upstream/android-13
static ssize_t dlpar_cpu_remove(struct device_node *dn, u32 drc_index)
{
	int rc;

<<<<<<< HEAD
	pr_debug("Attempting to remove CPU %s, drc index: %x\n",
		 dn->name, drc_index);

	rc = dlpar_offline_cpu(dn);
	if (rc) {
		pr_warn("Failed to offline CPU %s, rc: %d\n", dn->name, rc);
=======
	pr_debug("Attempting to remove CPU %pOFn, drc index: %x\n",
		 dn, drc_index);

	rc = dlpar_offline_cpu(dn);
	if (rc) {
		pr_warn("Failed to offline CPU %pOFn, rc: %d\n", dn, rc);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	rc = dlpar_release_drc(drc_index);
	if (rc) {
<<<<<<< HEAD
		pr_warn("Failed to release drc (%x) for CPU %s, rc: %d\n",
			drc_index, dn->name, rc);
=======
		pr_warn("Failed to release drc (%x) for CPU %pOFn, rc: %d\n",
			drc_index, dn, rc);
>>>>>>> upstream/android-13
		dlpar_online_cpu(dn);
		return rc;
	}

	rc = dlpar_detach_node(dn);
	if (rc) {
		int saved_rc = rc;

<<<<<<< HEAD
		pr_warn("Failed to detach CPU %s, rc: %d", dn->name, rc);
=======
		pr_warn("Failed to detach CPU %pOFn, rc: %d", dn, rc);
>>>>>>> upstream/android-13

		rc = dlpar_acquire_drc(drc_index);
		if (!rc)
			dlpar_online_cpu(dn);

		return saved_rc;
	}

	pr_debug("Successfully removed CPU, drc index: %x\n", drc_index);
	return 0;
}

static struct device_node *cpu_drc_index_to_dn(u32 drc_index)
{
	struct device_node *dn;
	u32 my_index;
	int rc;

	for_each_node_by_type(dn, "cpu") {
		rc = of_property_read_u32(dn, "ibm,my-drc-index", &my_index);
		if (rc)
			continue;

		if (my_index == drc_index)
			break;
	}

	return dn;
}

static int dlpar_cpu_remove_by_index(u32 drc_index)
{
	struct device_node *dn;
	int rc;

	dn = cpu_drc_index_to_dn(drc_index);
	if (!dn) {
		pr_warn("Cannot find CPU (drc index %x) to remove\n",
			drc_index);
		return -ENODEV;
	}

	rc = dlpar_cpu_remove(dn, drc_index);
	of_node_put(dn);
	return rc;
}

static int find_dlpar_cpus_to_remove(u32 *cpu_drcs, int cpus_to_remove)
{
	struct device_node *dn;
	int cpus_found = 0;
	int rc;

	/* We want to find cpus_to_remove + 1 CPUs to ensure we do not
	 * remove the last CPU.
	 */
	for_each_node_by_type(dn, "cpu") {
		cpus_found++;

		if (cpus_found > cpus_to_remove) {
			of_node_put(dn);
			break;
		}

		/* Note that cpus_found is always 1 ahead of the index
		 * into the cpu_drcs array, so we use cpus_found - 1
		 */
		rc = of_property_read_u32(dn, "ibm,my-drc-index",
					  &cpu_drcs[cpus_found - 1]);
		if (rc) {
<<<<<<< HEAD
			pr_warn("Error occurred getting drc-index for %s\n",
				dn->name);
=======
			pr_warn("Error occurred getting drc-index for %pOFn\n",
				dn);
>>>>>>> upstream/android-13
			of_node_put(dn);
			return -1;
		}
	}

	if (cpus_found < cpus_to_remove) {
		pr_warn("Failed to find enough CPUs (%d of %d) to remove\n",
			cpus_found, cpus_to_remove);
	} else if (cpus_found == cpus_to_remove) {
		pr_warn("Cannot remove all CPUs\n");
	}

	return cpus_found;
}

static int dlpar_cpu_remove_by_count(u32 cpus_to_remove)
{
	u32 *cpu_drcs;
	int cpus_found;
	int cpus_removed = 0;
	int i, rc;

	pr_debug("Attempting to hot-remove %d CPUs\n", cpus_to_remove);

	cpu_drcs = kcalloc(cpus_to_remove, sizeof(*cpu_drcs), GFP_KERNEL);
	if (!cpu_drcs)
		return -EINVAL;

	cpus_found = find_dlpar_cpus_to_remove(cpu_drcs, cpus_to_remove);
	if (cpus_found <= cpus_to_remove) {
		kfree(cpu_drcs);
		return -EINVAL;
	}

	for (i = 0; i < cpus_to_remove; i++) {
		rc = dlpar_cpu_remove_by_index(cpu_drcs[i]);
		if (rc)
			break;

		cpus_removed++;
	}

	if (cpus_removed != cpus_to_remove) {
		pr_warn("CPU hot-remove failed, adding back removed CPUs\n");

		for (i = 0; i < cpus_removed; i++)
			dlpar_cpu_add(cpu_drcs[i]);

		rc = -EINVAL;
	} else {
		rc = 0;
	}

	kfree(cpu_drcs);
	return rc;
}

<<<<<<< HEAD
static int find_dlpar_cpus_to_add(u32 *cpu_drcs, u32 cpus_to_add)
{
	struct device_node *parent;
	int cpus_found = 0;
	int index, rc;

	parent = of_find_node_by_path("/cpus");
	if (!parent) {
		pr_warn("Could not find CPU root node in device tree\n");
		kfree(cpu_drcs);
		return -1;
	}
=======
static int find_drc_info_cpus_to_add(struct device_node *cpus,
				     struct property *info,
				     u32 *cpu_drcs, u32 cpus_to_add)
{
	struct of_drc_info drc;
	const __be32 *value;
	u32 count, drc_index;
	int cpus_found = 0;
	int i, j;

	if (!info)
		return -1;

	value = of_prop_next_u32(info, NULL, &count);
	if (value)
		value++;

	for (i = 0; i < count; i++) {
		of_read_drc_info_cell(&info, &value, &drc);
		if (strncmp(drc.drc_type, "CPU", 3))
			break;

		drc_index = drc.drc_index_start;
		for (j = 0; j < drc.num_sequential_elems; j++) {
			if (dlpar_cpu_exists(cpus, drc_index))
				continue;

			cpu_drcs[cpus_found++] = drc_index;

			if (cpus_found == cpus_to_add)
				return cpus_found;

			drc_index += drc.sequential_inc;
		}
	}

	return cpus_found;
}

static int find_drc_index_cpus_to_add(struct device_node *cpus,
				      u32 *cpu_drcs, u32 cpus_to_add)
{
	int cpus_found = 0;
	int index, rc;
	u32 drc_index;
>>>>>>> upstream/android-13

	/* Search the ibm,drc-indexes array for possible CPU drcs to
	 * add. Note that the format of the ibm,drc-indexes array is
	 * the number of entries in the array followed by the array
	 * of drc values so we start looking at index = 1.
	 */
	index = 1;
	while (cpus_found < cpus_to_add) {
<<<<<<< HEAD
		u32 drc;

		rc = of_property_read_u32_index(parent, "ibm,drc-indexes",
						index++, &drc);
		if (rc)
			break;

		if (dlpar_cpu_exists(parent, drc))
			continue;

		cpu_drcs[cpus_found++] = drc;
	}

	of_node_put(parent);
=======
		rc = of_property_read_u32_index(cpus, "ibm,drc-indexes",
						index++, &drc_index);

		if (rc)
			break;

		if (dlpar_cpu_exists(cpus, drc_index))
			continue;

		cpu_drcs[cpus_found++] = drc_index;
	}

>>>>>>> upstream/android-13
	return cpus_found;
}

static int dlpar_cpu_add_by_count(u32 cpus_to_add)
{
<<<<<<< HEAD
=======
	struct device_node *parent;
	struct property *info;
>>>>>>> upstream/android-13
	u32 *cpu_drcs;
	int cpus_added = 0;
	int cpus_found;
	int i, rc;

	pr_debug("Attempting to hot-add %d CPUs\n", cpus_to_add);

	cpu_drcs = kcalloc(cpus_to_add, sizeof(*cpu_drcs), GFP_KERNEL);
	if (!cpu_drcs)
		return -EINVAL;

<<<<<<< HEAD
	cpus_found = find_dlpar_cpus_to_add(cpu_drcs, cpus_to_add);
=======
	parent = of_find_node_by_path("/cpus");
	if (!parent) {
		pr_warn("Could not find CPU root node in device tree\n");
		kfree(cpu_drcs);
		return -1;
	}

	info = of_find_property(parent, "ibm,drc-info", NULL);
	if (info)
		cpus_found = find_drc_info_cpus_to_add(parent, info, cpu_drcs, cpus_to_add);
	else
		cpus_found = find_drc_index_cpus_to_add(parent, cpu_drcs, cpus_to_add);

	of_node_put(parent);

>>>>>>> upstream/android-13
	if (cpus_found < cpus_to_add) {
		pr_warn("Failed to find enough CPUs (%d of %d) to add\n",
			cpus_found, cpus_to_add);
		kfree(cpu_drcs);
		return -EINVAL;
	}

	for (i = 0; i < cpus_to_add; i++) {
		rc = dlpar_cpu_add(cpu_drcs[i]);
		if (rc)
			break;

		cpus_added++;
	}

	if (cpus_added < cpus_to_add) {
		pr_warn("CPU hot-add failed, removing any added CPUs\n");

		for (i = 0; i < cpus_added; i++)
			dlpar_cpu_remove_by_index(cpu_drcs[i]);

		rc = -EINVAL;
	} else {
		rc = 0;
	}

	kfree(cpu_drcs);
	return rc;
}

<<<<<<< HEAD
int dlpar_cpu_readd(int cpu)
{
	struct device_node *dn;
	struct device *dev;
	u32 drc_index;
	int rc;

	dev = get_cpu_device(cpu);
	dn = dev->of_node;

	rc = of_property_read_u32(dn, "ibm,my-drc-index", &drc_index);

	rc = dlpar_cpu_remove_by_index(drc_index);
	if (!rc)
		rc = dlpar_cpu_add(drc_index);

	return rc;
}

=======
>>>>>>> upstream/android-13
int dlpar_cpu(struct pseries_hp_errorlog *hp_elog)
{
	u32 count, drc_index;
	int rc;

	count = hp_elog->_drc_u.drc_count;
	drc_index = hp_elog->_drc_u.drc_index;

	lock_device_hotplug();

	switch (hp_elog->action) {
	case PSERIES_HP_ELOG_ACTION_REMOVE:
		if (hp_elog->id_type == PSERIES_HP_ELOG_ID_DRC_COUNT)
			rc = dlpar_cpu_remove_by_count(count);
<<<<<<< HEAD
		else if (hp_elog->id_type == PSERIES_HP_ELOG_ID_DRC_INDEX)
			rc = dlpar_cpu_remove_by_index(drc_index);
=======
		else if (hp_elog->id_type == PSERIES_HP_ELOG_ID_DRC_INDEX) {
			rc = dlpar_cpu_remove_by_index(drc_index);
			/*
			 * Setting the isolation state of an UNISOLATED/CONFIGURED
			 * device to UNISOLATE is a no-op, but the hypervisor can
			 * use it as a hint that the CPU removal failed.
			 */
			if (rc)
				dlpar_unisolate_drc(drc_index);
		}
>>>>>>> upstream/android-13
		else
			rc = -EINVAL;
		break;
	case PSERIES_HP_ELOG_ACTION_ADD:
		if (hp_elog->id_type == PSERIES_HP_ELOG_ID_DRC_COUNT)
			rc = dlpar_cpu_add_by_count(count);
		else if (hp_elog->id_type == PSERIES_HP_ELOG_ID_DRC_INDEX)
			rc = dlpar_cpu_add(drc_index);
		else
			rc = -EINVAL;
		break;
	default:
		pr_err("Invalid action (%d) specified\n", hp_elog->action);
		rc = -EINVAL;
		break;
	}

	unlock_device_hotplug();
	return rc;
}

#ifdef CONFIG_ARCH_CPU_PROBE_RELEASE

static ssize_t dlpar_cpu_probe(const char *buf, size_t count)
{
	u32 drc_index;
	int rc;

	rc = kstrtou32(buf, 0, &drc_index);
	if (rc)
		return -EINVAL;

	rc = dlpar_cpu_add(drc_index);

	return rc ? rc : count;
}

static ssize_t dlpar_cpu_release(const char *buf, size_t count)
{
	struct device_node *dn;
	u32 drc_index;
	int rc;

	dn = of_find_node_by_path(buf);
	if (!dn)
		return -EINVAL;

	rc = of_property_read_u32(dn, "ibm,my-drc-index", &drc_index);
	if (rc) {
		of_node_put(dn);
		return -EINVAL;
	}

	rc = dlpar_cpu_remove(dn, drc_index);
	of_node_put(dn);

	return rc ? rc : count;
}

#endif /* CONFIG_ARCH_CPU_PROBE_RELEASE */

static int pseries_smp_notifier(struct notifier_block *nb,
				unsigned long action, void *data)
{
	struct of_reconfig_data *rd = data;
	int err = 0;

	switch (action) {
	case OF_RECONFIG_ATTACH_NODE:
		err = pseries_add_processor(rd->dn);
		break;
	case OF_RECONFIG_DETACH_NODE:
		pseries_remove_processor(rd->dn);
		break;
	}
	return notifier_from_errno(err);
}

static struct notifier_block pseries_smp_nb = {
	.notifier_call = pseries_smp_notifier,
};

<<<<<<< HEAD
#define MAX_CEDE_LATENCY_LEVELS		4
#define	CEDE_LATENCY_PARAM_LENGTH	10
#define CEDE_LATENCY_PARAM_MAX_LENGTH	\
	(MAX_CEDE_LATENCY_LEVELS * CEDE_LATENCY_PARAM_LENGTH * sizeof(char))
#define CEDE_LATENCY_TOKEN		45

static char cede_parameters[CEDE_LATENCY_PARAM_MAX_LENGTH];

static int parse_cede_parameters(void)
{
	memset(cede_parameters, 0, CEDE_LATENCY_PARAM_MAX_LENGTH);
	return rtas_call(rtas_token("ibm,get-system-parameter"), 3, 1,
			 NULL,
			 CEDE_LATENCY_TOKEN,
			 __pa(cede_parameters),
			 CEDE_LATENCY_PARAM_MAX_LENGTH);
}

static int __init pseries_cpu_hotplug_init(void)
{
	int cpu;
	int qcss_tok;
=======
static int __init pseries_cpu_hotplug_init(void)
{
	int qcss_tok;
	unsigned int node;
>>>>>>> upstream/android-13

#ifdef CONFIG_ARCH_CPU_PROBE_RELEASE
	ppc_md.cpu_probe = dlpar_cpu_probe;
	ppc_md.cpu_release = dlpar_cpu_release;
#endif /* CONFIG_ARCH_CPU_PROBE_RELEASE */

	rtas_stop_self_token = rtas_token("stop-self");
	qcss_tok = rtas_token("query-cpu-stopped-state");

	if (rtas_stop_self_token == RTAS_UNKNOWN_SERVICE ||
			qcss_tok == RTAS_UNKNOWN_SERVICE) {
		printk(KERN_INFO "CPU Hotplug not supported by firmware "
				"- disabling.\n");
		return 0;
	}

<<<<<<< HEAD
	ppc_md.cpu_die = pseries_mach_cpu_die;
=======
	smp_ops->cpu_offline_self = pseries_cpu_offline_self;
>>>>>>> upstream/android-13
	smp_ops->cpu_disable = pseries_cpu_disable;
	smp_ops->cpu_die = pseries_cpu_die;

	/* Processors can be added/removed only on LPAR */
	if (firmware_has_feature(FW_FEATURE_LPAR)) {
<<<<<<< HEAD
		of_reconfig_notifier_register(&pseries_smp_nb);
		cpu_maps_update_begin();
		if (cede_offline_enabled && parse_cede_parameters() == 0) {
			default_offline_state = CPU_STATE_INACTIVE;
			for_each_online_cpu(cpu)
				set_default_offline_state(cpu);
		}
		cpu_maps_update_done();
=======
		for_each_node(node) {
			alloc_bootmem_cpumask_var(&node_recorded_ids_map[node]);

			/* Record ids of CPU added at boot time */
			cpumask_or(node_recorded_ids_map[node],
				   node_recorded_ids_map[node],
				   cpumask_of_node(node));
		}

		of_reconfig_notifier_register(&pseries_smp_nb);
>>>>>>> upstream/android-13
	}

	return 0;
}
machine_arch_initcall(pseries, pseries_cpu_hotplug_init);
