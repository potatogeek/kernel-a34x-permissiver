// SPDX-License-Identifier: GPL-2.0
/*
<<<<<<< HEAD
 * Energy Model of CPUs
 *
 * Copyright (c) 2018, Arm ltd.
 * Written by: Quentin Perret, Arm ltd.
=======
 * Energy Model of devices
 *
 * Copyright (c) 2018-2020, Arm ltd.
 * Written by: Quentin Perret, Arm ltd.
 * Improvements provided by: Lukasz Luba, Arm ltd.
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) "energy_model: " fmt

#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/debugfs.h>
#include <linux/energy_model.h>
#include <linux/sched/topology.h>
#include <linux/slab.h>
<<<<<<< HEAD

/* Mapping of each CPU to the performance domain to which it belongs. */
static DEFINE_PER_CPU(struct em_perf_domain *, em_data);
=======
#include <trace/hooks/sched.h>
>>>>>>> upstream/android-13

/*
 * Mutex serializing the registrations of performance domains and letting
 * callbacks defined by drivers sleep.
 */
static DEFINE_MUTEX(em_pd_mutex);

<<<<<<< HEAD
#ifdef CONFIG_DEBUG_FS
static struct dentry *rootdir;

static void em_debug_create_cs(struct em_cap_state *cs, struct dentry *pd)
=======
static bool _is_cpu_device(struct device *dev)
{
	return (dev->bus == &cpu_subsys);
}

#ifdef CONFIG_DEBUG_FS
static struct dentry *rootdir;

static void em_debug_create_ps(struct em_perf_state *ps, struct dentry *pd)
>>>>>>> upstream/android-13
{
	struct dentry *d;
	char name[24];

<<<<<<< HEAD
	snprintf(name, sizeof(name), "cs:%lu", cs->frequency);

	/* Create per-cs directory */
	d = debugfs_create_dir(name, pd);
	debugfs_create_ulong("frequency", 0444, d, &cs->frequency);
	debugfs_create_ulong("power", 0444, d, &cs->power);
	debugfs_create_ulong("cost", 0444, d, &cs->cost);
=======
	snprintf(name, sizeof(name), "ps:%lu", ps->frequency);

	/* Create per-ps directory */
	d = debugfs_create_dir(name, pd);
	debugfs_create_ulong("frequency", 0444, d, &ps->frequency);
	debugfs_create_ulong("power", 0444, d, &ps->power);
	debugfs_create_ulong("cost", 0444, d, &ps->cost);
>>>>>>> upstream/android-13
}

static int em_debug_cpus_show(struct seq_file *s, void *unused)
{
	seq_printf(s, "%*pbl\n", cpumask_pr_args(to_cpumask(s->private)));

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(em_debug_cpus);

<<<<<<< HEAD
static void em_debug_create_pd(struct em_perf_domain *pd, int cpu)
{
	struct dentry *d;
	char name[8];
	int i;

	snprintf(name, sizeof(name), "pd%d", cpu);

	/* Create the directory of the performance domain */
	d = debugfs_create_dir(name, rootdir);

	debugfs_create_file("cpus", 0444, d, pd->cpus, &em_debug_cpus_fops);

	/* Create a sub-directory for each capacity state */
	for (i = 0; i < pd->nr_cap_states; i++)
		em_debug_create_cs(&pd->table[i], d);
=======
static int em_debug_units_show(struct seq_file *s, void *unused)
{
	struct em_perf_domain *pd = s->private;
	char *units = pd->milliwatts ? "milliWatts" : "bogoWatts";

	seq_printf(s, "%s\n", units);

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(em_debug_units);

static void em_debug_create_pd(struct device *dev)
{
	struct dentry *d;
	int i;

	/* Create the directory of the performance domain */
	d = debugfs_create_dir(dev_name(dev), rootdir);

	if (_is_cpu_device(dev))
		debugfs_create_file("cpus", 0444, d, dev->em_pd->cpus,
				    &em_debug_cpus_fops);

	debugfs_create_file("units", 0444, d, dev->em_pd, &em_debug_units_fops);

	/* Create a sub-directory for each performance state */
	for (i = 0; i < dev->em_pd->nr_perf_states; i++)
		em_debug_create_ps(&dev->em_pd->table[i], d);

}

static void em_debug_remove_pd(struct device *dev)
{
	struct dentry *debug_dir;

	debug_dir = debugfs_lookup(dev_name(dev), rootdir);
	debugfs_remove_recursive(debug_dir);
>>>>>>> upstream/android-13
}

static int __init em_debug_init(void)
{
	/* Create /sys/kernel/debug/energy_model directory */
	rootdir = debugfs_create_dir("energy_model", NULL);

	return 0;
}
<<<<<<< HEAD
core_initcall(em_debug_init);
#else /* CONFIG_DEBUG_FS */
static void em_debug_create_pd(struct em_perf_domain *pd, int cpu) {}
#endif
static struct em_perf_domain *em_create_pd(cpumask_t *span, int nr_states,
						struct em_data_callback *cb)
{
	unsigned long opp_eff, prev_opp_eff = ULONG_MAX;
	unsigned long power, freq, prev_freq = 0;
	int i, ret, cpu = cpumask_first(span);
	struct em_cap_state *table;
	struct em_perf_domain *pd;
	u64 fmax;

	if (!cb->active_power)
		return NULL;

	pd = kzalloc(sizeof(*pd) + cpumask_size(), GFP_KERNEL);
	if (!pd)
		return NULL;

	table = kcalloc(nr_states, sizeof(*table), GFP_KERNEL);
	if (!table)
		goto free_pd;

	/* Build the list of capacity states for this performance domain */
	for (i = 0, freq = 0; i < nr_states; i++, freq++) {
		/*
		 * active_power() is a driver callback which ceils 'freq' to
		 * lowest capacity state of 'cpu' above 'freq' and updates
		 * 'power' and 'freq' accordingly.
		 */
		ret = cb->active_power(&power, &freq, cpu);
		if (ret) {
			pr_err("pd%d: invalid cap. state: %d\n", cpu, ret);
			goto free_cs_table;
=======
fs_initcall(em_debug_init);
#else /* CONFIG_DEBUG_FS */
static void em_debug_create_pd(struct device *dev) {}
static void em_debug_remove_pd(struct device *dev) {}
#endif

static int em_create_perf_table(struct device *dev, struct em_perf_domain *pd,
				int nr_states, struct em_data_callback *cb)
{
	unsigned long power, freq, prev_freq = 0, prev_cost = ULONG_MAX;
	struct em_perf_state *table;
	int i, ret;
	u64 fmax;

	table = kcalloc(nr_states, sizeof(*table), GFP_KERNEL);
	if (!table)
		return -ENOMEM;

	/* Build the list of performance states for this performance domain */
	for (i = 0, freq = 0; i < nr_states; i++, freq++) {
		/*
		 * active_power() is a driver callback which ceils 'freq' to
		 * lowest performance state of 'dev' above 'freq' and updates
		 * 'power' and 'freq' accordingly.
		 */
		ret = cb->active_power(&power, &freq, dev);
		if (ret) {
			dev_err(dev, "EM: invalid perf. state: %d\n",
				ret);
			goto free_ps_table;
>>>>>>> upstream/android-13
		}

		/*
		 * We expect the driver callback to increase the frequency for
<<<<<<< HEAD
		 * higher capacity states.
		 */
		if (freq <= prev_freq) {
			pr_err("pd%d: non-increasing freq: %lu\n", cpu, freq);
			goto free_cs_table;
=======
		 * higher performance states.
		 */
		if (freq <= prev_freq) {
			dev_err(dev, "EM: non-increasing freq: %lu\n",
				freq);
			goto free_ps_table;
>>>>>>> upstream/android-13
		}

		/*
		 * The power returned by active_state() is expected to be
<<<<<<< HEAD
		 * positive, in milli-watts and to fit into 16 bits.
		 */
		if (!power || power > EM_CPU_MAX_POWER) {
			pr_err("pd%d: invalid power: %lu\n", cpu, power);
			goto free_cs_table;
=======
		 * positive and to fit into 16 bits.
		 */
		if (!power || power > EM_MAX_POWER) {
			dev_err(dev, "EM: invalid power: %lu\n",
				power);
			goto free_ps_table;
>>>>>>> upstream/android-13
		}

		table[i].power = power;
		table[i].frequency = prev_freq = freq;
<<<<<<< HEAD

		/*
		 * The hertz/watts efficiency ratio should decrease as the
		 * frequency grows on sane platforms. But this isn't always
		 * true in practice so warn the user if a higher OPP is more
		 * power efficient than a lower one.
		 */
		opp_eff = freq / power;
		if (opp_eff >= prev_opp_eff)
			pr_warn("pd%d: hertz/watts ratio non-monotonically decreasing: em_cap_state %d >= em_cap_state%d\n",
					cpu, i, i - 1);
		prev_opp_eff = opp_eff;
	}

	/* Compute the cost of each capacity_state. */
	fmax = (u64) table[nr_states - 1].frequency;
	for (i = 0; i < nr_states; i++) {
		table[i].cost = div64_u64(fmax * table[i].power,
					  table[i].frequency);
	}

	pd->table = table;
	pd->nr_cap_states = nr_states;
	cpumask_copy(to_cpumask(pd->cpus), span);

	em_debug_create_pd(pd, cpu);

	return pd;

free_cs_table:
	kfree(table);
free_pd:
	kfree(pd);

	return NULL;
}

=======
	}

	/* Compute the cost of each performance state. */
	fmax = (u64) table[nr_states - 1].frequency;
	for (i = nr_states - 1; i >= 0; i--) {
		unsigned long power_res = em_scale_power(table[i].power);

		table[i].cost = div64_u64(fmax * power_res,
					  table[i].frequency);
		if (table[i].cost >= prev_cost) {
			dev_dbg(dev, "EM: OPP:%lu is inefficient\n",
				table[i].frequency);
		} else {
			prev_cost = table[i].cost;
		}
	}

	pd->table = table;
	pd->nr_perf_states = nr_states;

	return 0;

free_ps_table:
	kfree(table);
	return -EINVAL;
}

static int em_create_pd(struct device *dev, int nr_states,
			struct em_data_callback *cb, cpumask_t *cpus)
{
	struct em_perf_domain *pd;
	struct device *cpu_dev;
	int cpu, ret;

	if (_is_cpu_device(dev)) {
		pd = kzalloc(sizeof(*pd) + cpumask_size(), GFP_KERNEL);
		if (!pd)
			return -ENOMEM;

		cpumask_copy(em_span_cpus(pd), cpus);
	} else {
		pd = kzalloc(sizeof(*pd), GFP_KERNEL);
		if (!pd)
			return -ENOMEM;
	}

	ret = em_create_perf_table(dev, pd, nr_states, cb);
	if (ret) {
		kfree(pd);
		return ret;
	}

	if (_is_cpu_device(dev))
		for_each_cpu(cpu, cpus) {
			cpu_dev = get_cpu_device(cpu);
			cpu_dev->em_pd = pd;
		}

	dev->em_pd = pd;

	return 0;
}

/**
 * em_pd_get() - Return the performance domain for a device
 * @dev : Device to find the performance domain for
 *
 * Returns the performance domain to which @dev belongs, or NULL if it doesn't
 * exist.
 */
struct em_perf_domain *em_pd_get(struct device *dev)
{
	if (IS_ERR_OR_NULL(dev))
		return NULL;

	return dev->em_pd;
}
EXPORT_SYMBOL_GPL(em_pd_get);

>>>>>>> upstream/android-13
/**
 * em_cpu_get() - Return the performance domain for a CPU
 * @cpu : CPU to find the performance domain for
 *
<<<<<<< HEAD
 * Return: the performance domain to which 'cpu' belongs, or NULL if it doesn't
=======
 * Returns the performance domain to which @cpu belongs, or NULL if it doesn't
>>>>>>> upstream/android-13
 * exist.
 */
struct em_perf_domain *em_cpu_get(int cpu)
{
<<<<<<< HEAD
	return READ_ONCE(per_cpu(em_data, cpu));
=======
	struct device *cpu_dev;

	cpu_dev = get_cpu_device(cpu);
	if (!cpu_dev)
		return NULL;

	return em_pd_get(cpu_dev);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(em_cpu_get);

/**
<<<<<<< HEAD
 * em_register_perf_domain() - Register the Energy Model of a performance domain
 * @span	: Mask of CPUs in the performance domain
 * @nr_states	: Number of capacity states to register
 * @cb		: Callback functions providing the data of the Energy Model
=======
 * em_dev_register_perf_domain() - Register the Energy Model (EM) for a device
 * @dev		: Device for which the EM is to register
 * @nr_states	: Number of performance states to register
 * @cb		: Callback functions providing the data of the Energy Model
 * @cpus	: Pointer to cpumask_t, which in case of a CPU device is
 *		obligatory. It can be taken from i.e. 'policy->cpus'. For other
 *		type of devices this should be set to NULL.
 * @milliwatts	: Flag indicating that the power values are in milliWatts or
 *		in some other scale. It must be set properly.
>>>>>>> upstream/android-13
 *
 * Create Energy Model tables for a performance domain using the callbacks
 * defined in cb.
 *
<<<<<<< HEAD
=======
 * The @milliwatts is important to set with correct value. Some kernel
 * sub-systems might rely on this flag and check if all devices in the EM are
 * using the same scale.
 *
>>>>>>> upstream/android-13
 * If multiple clients register the same performance domain, all but the first
 * registration will be ignored.
 *
 * Return 0 on success
 */
<<<<<<< HEAD
int em_register_perf_domain(cpumask_t *span, unsigned int nr_states,
						struct em_data_callback *cb)
{
	unsigned long cap, prev_cap = 0;
	struct em_perf_domain *pd;
	int cpu, ret = 0;

	if (!span || !nr_states || !cb)
=======
int em_dev_register_perf_domain(struct device *dev, unsigned int nr_states,
				struct em_data_callback *cb, cpumask_t *cpus,
				bool milliwatts)
{
	unsigned long cap, prev_cap = 0;
	int cpu, ret;
	bool cond = false;

	if (!dev || !nr_states || !cb)
>>>>>>> upstream/android-13
		return -EINVAL;

	/*
	 * Use a mutex to serialize the registration of performance domains and
	 * let the driver-defined callback functions sleep.
	 */
	mutex_lock(&em_pd_mutex);

<<<<<<< HEAD
	for_each_cpu(cpu, span) {
		/* Make sure we don't register again an existing domain. */
		if (READ_ONCE(per_cpu(em_data, cpu))) {
			ret = -EEXIST;
			goto unlock;
		}

		/*
		 * All CPUs of a domain must have the same micro-architecture
		 * since they all share the same table.
		 */
		cap = arch_scale_cpu_capacity(NULL, cpu);
		if (prev_cap && prev_cap != cap) {
			pr_err("CPUs of %*pbl must have the same capacity\n",
							cpumask_pr_args(span));
			ret = -EINVAL;
			goto unlock;
		}
		prev_cap = cap;
	}

	/* Create the performance domain and add it to the Energy Model. */
	pd = em_create_pd(span, nr_states, cb);
	if (!pd) {
		ret = -EINVAL;
		goto unlock;
	}

	for_each_cpu(cpu, span) {
		/*
		 * The per-cpu array can be read concurrently from em_cpu_get().
		 * The barrier enforces the ordering needed to make sure readers
		 * can only access well formed em_perf_domain structs.
		 */
		smp_store_release(per_cpu_ptr(&em_data, cpu), pd);
	}

	pr_debug("Created perf domain %*pbl\n", cpumask_pr_args(span));
unlock:
	mutex_unlock(&em_pd_mutex);

	return ret;
}
EXPORT_SYMBOL_GPL(em_register_perf_domain);
=======
	if (dev->em_pd) {
		ret = -EEXIST;
		goto unlock;
	}

	if (_is_cpu_device(dev)) {
		if (!cpus) {
			dev_err(dev, "EM: invalid CPU mask\n");
			ret = -EINVAL;
			goto unlock;
		}

		for_each_cpu(cpu, cpus) {
			if (em_cpu_get(cpu)) {
				dev_err(dev, "EM: exists for CPU%d\n", cpu);
				ret = -EEXIST;
				goto unlock;
			}

			trace_android_vh_em_dev_register_pd(&cond);
			if (cond)
				continue;
			/*
			 * All CPUs of a domain must have the same
			 * micro-architecture since they all share the same
			 * table.
			 */
			cap = arch_scale_cpu_capacity(cpu);
			if (prev_cap && prev_cap != cap) {
				dev_err(dev, "EM: CPUs of %*pbl must have the same capacity\n",
					cpumask_pr_args(cpus));

				ret = -EINVAL;
				goto unlock;
			}
			prev_cap = cap;
		}
	}

	ret = em_create_pd(dev, nr_states, cb, cpus);
	if (ret)
		goto unlock;

	dev->em_pd->milliwatts = milliwatts;

	em_debug_create_pd(dev);
	dev_info(dev, "EM: created perf domain\n");

unlock:
	mutex_unlock(&em_pd_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(em_dev_register_perf_domain);

/**
 * em_dev_unregister_perf_domain() - Unregister Energy Model (EM) for a device
 * @dev		: Device for which the EM is registered
 *
 * Unregister the EM for the specified @dev (but not a CPU device).
 */
void em_dev_unregister_perf_domain(struct device *dev)
{
	if (IS_ERR_OR_NULL(dev) || !dev->em_pd)
		return;

	if (_is_cpu_device(dev))
		return;

	/*
	 * The mutex separates all register/unregister requests and protects
	 * from potential clean-up/setup issues in the debugfs directories.
	 * The debugfs directory name is the same as device's name.
	 */
	mutex_lock(&em_pd_mutex);
	em_debug_remove_pd(dev);

	kfree(dev->em_pd->table);
	kfree(dev->em_pd);
	dev->em_pd = NULL;
	mutex_unlock(&em_pd_mutex);
}
EXPORT_SYMBOL_GPL(em_dev_unregister_perf_domain);
>>>>>>> upstream/android-13
