// SPDX-License-Identifier: GPL-2.0
/*
 * Componentized device handling.
 *
 * This is work in progress.  We gather up the component devices into a list,
 * and bind them when instructed.  At the moment, we're specific to the DRM
 * subsystem, and only handles one master device, but this doesn't have to be
 * the case.
 */
#include <linux/component.h>
#include <linux/device.h>
#include <linux/kref.h>
#include <linux/list.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
>>>>>>> upstream/android-13
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/debugfs.h>

<<<<<<< HEAD
=======
/**
 * DOC: overview
 *
 * The component helper allows drivers to collect a pile of sub-devices,
 * including their bound drivers, into an aggregate driver. Various subsystems
 * already provide functions to get hold of such components, e.g.
 * of_clk_get_by_name(). The component helper can be used when such a
 * subsystem-specific way to find a device is not available: The component
 * helper fills the niche of aggregate drivers for specific hardware, where
 * further standardization into a subsystem would not be practical. The common
 * example is when a logical device (e.g. a DRM display driver) is spread around
 * the SoC on various components (scanout engines, blending blocks, transcoders
 * for various outputs and so on).
 *
 * The component helper also doesn't solve runtime dependencies, e.g. for system
 * suspend and resume operations. See also :ref:`device links<device_link>`.
 *
 * Components are registered using component_add() and unregistered with
 * component_del(), usually from the driver's probe and disconnect functions.
 *
 * Aggregate drivers first assemble a component match list of what they need
 * using component_match_add(). This is then registered as an aggregate driver
 * using component_master_add_with_match(), and unregistered using
 * component_master_del().
 */

>>>>>>> upstream/android-13
struct component;

struct component_match_array {
	void *data;
	int (*compare)(struct device *, void *);
<<<<<<< HEAD
=======
	int (*compare_typed)(struct device *, int, void *);
>>>>>>> upstream/android-13
	void (*release)(struct device *, void *);
	struct component *component;
	bool duplicate;
};

struct component_match {
	size_t alloc;
	size_t num;
	struct component_match_array *compare;
};

struct master {
	struct list_head node;
	bool bound;

	const struct component_master_ops *ops;
<<<<<<< HEAD
	struct device *dev;
	struct component_match *match;
	struct dentry *dentry;
=======
	struct device *parent;
	struct component_match *match;
>>>>>>> upstream/android-13
};

struct component {
	struct list_head node;
	struct master *master;
	bool bound;

	const struct component_ops *ops;
<<<<<<< HEAD
=======
	int subcomponent;
>>>>>>> upstream/android-13
	struct device *dev;
};

static DEFINE_MUTEX(component_mutex);
static LIST_HEAD(component_list);
static LIST_HEAD(masters);

#ifdef CONFIG_DEBUG_FS

static struct dentry *component_debugfs_dir;

static int component_devices_show(struct seq_file *s, void *data)
{
	struct master *m = s->private;
	struct component_match *match = m->match;
	size_t i;

	mutex_lock(&component_mutex);
	seq_printf(s, "%-40s %20s\n", "master name", "status");
	seq_puts(s, "-------------------------------------------------------------\n");
	seq_printf(s, "%-40s %20s\n\n",
<<<<<<< HEAD
		   dev_name(m->dev), m->bound ? "bound" : "not bound");
=======
		   dev_name(m->parent), m->bound ? "bound" : "not bound");
>>>>>>> upstream/android-13

	seq_printf(s, "%-40s %20s\n", "device name", "status");
	seq_puts(s, "-------------------------------------------------------------\n");
	for (i = 0; i < match->num; i++) {
		struct component *component = match->compare[i].component;

		seq_printf(s, "%-40s %20s\n",
			   component ? dev_name(component->dev) : "(unknown)",
			   component ? (component->bound ? "bound" : "not bound") : "not registered");
	}
	mutex_unlock(&component_mutex);

	return 0;
}

<<<<<<< HEAD
static int component_devices_open(struct inode *inode, struct file *file)
{
	return single_open(file, component_devices_show, inode->i_private);
}

static const struct file_operations component_devices_fops = {
	.open = component_devices_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};
=======
DEFINE_SHOW_ATTRIBUTE(component_devices);
>>>>>>> upstream/android-13

static int __init component_debug_init(void)
{
	component_debugfs_dir = debugfs_create_dir("device_component", NULL);

	return 0;
}

core_initcall(component_debug_init);

static void component_master_debugfs_add(struct master *m)
{
<<<<<<< HEAD
	m->dentry = debugfs_create_file(dev_name(m->dev), 0444,
					component_debugfs_dir,
					m, &component_devices_fops);
=======
	debugfs_create_file(dev_name(m->parent), 0444, component_debugfs_dir, m,
			    &component_devices_fops);
>>>>>>> upstream/android-13
}

static void component_master_debugfs_del(struct master *m)
{
<<<<<<< HEAD
	debugfs_remove(m->dentry);
	m->dentry = NULL;
=======
	debugfs_remove(debugfs_lookup(dev_name(m->parent), component_debugfs_dir));
>>>>>>> upstream/android-13
}

#else

static void component_master_debugfs_add(struct master *m)
{ }

static void component_master_debugfs_del(struct master *m)
{ }

#endif

<<<<<<< HEAD
static struct master *__master_find(struct device *dev,
=======
static struct master *__master_find(struct device *parent,
>>>>>>> upstream/android-13
	const struct component_master_ops *ops)
{
	struct master *m;

	list_for_each_entry(m, &masters, node)
<<<<<<< HEAD
		if (m->dev == dev && (!ops || m->ops == ops))
=======
		if (m->parent == parent && (!ops || m->ops == ops))
>>>>>>> upstream/android-13
			return m;

	return NULL;
}

static struct component *find_component(struct master *master,
<<<<<<< HEAD
	int (*compare)(struct device *, void *), void *compare_data)
=======
	struct component_match_array *mc)
>>>>>>> upstream/android-13
{
	struct component *c;

	list_for_each_entry(c, &component_list, node) {
		if (c->master && c->master != master)
			continue;

<<<<<<< HEAD
		if (compare(c->dev, compare_data))
=======
		if (mc->compare && mc->compare(c->dev, mc->data))
			return c;

		if (mc->compare_typed &&
		    mc->compare_typed(c->dev, c->subcomponent, mc->data))
>>>>>>> upstream/android-13
			return c;
	}

	return NULL;
}

static int find_components(struct master *master)
{
	struct component_match *match = master->match;
	size_t i;
	int ret = 0;

	/*
	 * Scan the array of match functions and attach
	 * any components which are found to this master.
	 */
	for (i = 0; i < match->num; i++) {
		struct component_match_array *mc = &match->compare[i];
		struct component *c;

<<<<<<< HEAD
		dev_dbg(master->dev, "Looking for component %zu\n", i);
=======
		dev_dbg(master->parent, "Looking for component %zu\n", i);
>>>>>>> upstream/android-13

		if (match->compare[i].component)
			continue;

<<<<<<< HEAD
		c = find_component(master, mc->compare, mc->data);
=======
		c = find_component(master, mc);
>>>>>>> upstream/android-13
		if (!c) {
			ret = -ENXIO;
			break;
		}

<<<<<<< HEAD
		dev_dbg(master->dev, "found component %s, duplicate %u\n", dev_name(c->dev), !!c->master);
=======
		dev_dbg(master->parent, "found component %s, duplicate %u\n", dev_name(c->dev), !!c->master);
>>>>>>> upstream/android-13

		/* Attach this component to the master */
		match->compare[i].duplicate = !!c->master;
		match->compare[i].component = c;
		c->master = master;
	}
	return ret;
}

/* Detach component from associated master */
static void remove_component(struct master *master, struct component *c)
{
	size_t i;

	/* Detach the component from this master. */
	for (i = 0; i < master->match->num; i++)
		if (master->match->compare[i].component == c)
			master->match->compare[i].component = NULL;
}

/*
 * Try to bring up a master.  If component is NULL, we're interested in
 * this master, otherwise it's a component which must be present to try
 * and bring up the master.
 *
 * Returns 1 for successful bringup, 0 if not ready, or -ve errno.
 */
static int try_to_bring_up_master(struct master *master,
	struct component *component)
{
	int ret;

<<<<<<< HEAD
	dev_dbg(master->dev, "trying to bring up master\n");

	if (find_components(master)) {
		dev_dbg(master->dev, "master has incomplete components\n");
=======
	dev_dbg(master->parent, "trying to bring up master\n");

	if (find_components(master)) {
		dev_dbg(master->parent, "master has incomplete components\n");
>>>>>>> upstream/android-13
		return 0;
	}

	if (component && component->master != master) {
<<<<<<< HEAD
		dev_dbg(master->dev, "master is not for this component (%s)\n",
=======
		dev_dbg(master->parent, "master is not for this component (%s)\n",
>>>>>>> upstream/android-13
			dev_name(component->dev));
		return 0;
	}

<<<<<<< HEAD
	if (!devres_open_group(master->dev, NULL, GFP_KERNEL))
		return -ENOMEM;

	/* Found all components */
	ret = master->ops->bind(master->dev);
	if (ret < 0) {
		devres_release_group(master->dev, NULL);
		if (ret != -EPROBE_DEFER)
			dev_info(master->dev, "master bind failed: %d\n", ret);
		return ret;
	}

=======
	if (!devres_open_group(master->parent, master, GFP_KERNEL))
		return -ENOMEM;

	/* Found all components */
	ret = master->ops->bind(master->parent);
	if (ret < 0) {
		devres_release_group(master->parent, NULL);
		if (ret != -EPROBE_DEFER)
			dev_info(master->parent, "master bind failed: %d\n", ret);
		return ret;
	}

	devres_close_group(master->parent, NULL);
>>>>>>> upstream/android-13
	master->bound = true;
	return 1;
}

static int try_to_bring_up_masters(struct component *component)
{
	struct master *m;
	int ret = 0;

	list_for_each_entry(m, &masters, node) {
		if (!m->bound) {
			ret = try_to_bring_up_master(m, component);
			if (ret != 0)
				break;
		}
	}

	return ret;
}

static void take_down_master(struct master *master)
{
	if (master->bound) {
<<<<<<< HEAD
		master->ops->unbind(master->dev);
		devres_release_group(master->dev, NULL);
=======
		master->ops->unbind(master->parent);
		devres_release_group(master->parent, master);
>>>>>>> upstream/android-13
		master->bound = false;
	}
}

<<<<<<< HEAD
static void component_match_release(struct device *master,
	struct component_match *match)
{
=======
static void devm_component_match_release(struct device *parent, void *res)
{
	struct component_match *match = res;
>>>>>>> upstream/android-13
	unsigned int i;

	for (i = 0; i < match->num; i++) {
		struct component_match_array *mc = &match->compare[i];

		if (mc->release)
<<<<<<< HEAD
			mc->release(master, mc->data);
=======
			mc->release(parent, mc->data);
>>>>>>> upstream/android-13
	}

	kfree(match->compare);
}

<<<<<<< HEAD
static void devm_component_match_release(struct device *dev, void *res)
{
	component_match_release(dev, res);
}

static int component_match_realloc(struct device *dev,
	struct component_match *match, size_t num)
=======
static int component_match_realloc(struct component_match *match, size_t num)
>>>>>>> upstream/android-13
{
	struct component_match_array *new;

	if (match->alloc == num)
		return 0;

	new = kmalloc_array(num, sizeof(*new), GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	if (match->compare) {
		memcpy(new, match->compare, sizeof(*new) *
					    min(match->num, num));
		kfree(match->compare);
	}
	match->compare = new;
	match->alloc = num;

	return 0;
}

<<<<<<< HEAD
/*
 * Add a component to be matched, with a release function.
 *
 * The match array is first created or extended if necessary.
 */
void component_match_add_release(struct device *master,
	struct component_match **matchptr,
	void (*release)(struct device *, void *),
	int (*compare)(struct device *, void *), void *compare_data)
=======
static void __component_match_add(struct device *master,
	struct component_match **matchptr,
	void (*release)(struct device *, void *),
	int (*compare)(struct device *, void *),
	int (*compare_typed)(struct device *, int, void *),
	void *compare_data)
>>>>>>> upstream/android-13
{
	struct component_match *match = *matchptr;

	if (IS_ERR(match))
		return;

	if (!match) {
		match = devres_alloc(devm_component_match_release,
				     sizeof(*match), GFP_KERNEL);
		if (!match) {
			*matchptr = ERR_PTR(-ENOMEM);
			return;
		}

		devres_add(master, match);

		*matchptr = match;
	}

	if (match->num == match->alloc) {
		size_t new_size = match->alloc + 16;
		int ret;

<<<<<<< HEAD
		ret = component_match_realloc(master, match, new_size);
=======
		ret = component_match_realloc(match, new_size);
>>>>>>> upstream/android-13
		if (ret) {
			*matchptr = ERR_PTR(ret);
			return;
		}
	}

	match->compare[match->num].compare = compare;
<<<<<<< HEAD
=======
	match->compare[match->num].compare_typed = compare_typed;
>>>>>>> upstream/android-13
	match->compare[match->num].release = release;
	match->compare[match->num].data = compare_data;
	match->compare[match->num].component = NULL;
	match->num++;
}
<<<<<<< HEAD
EXPORT_SYMBOL(component_match_add_release);

=======

/**
 * component_match_add_release - add a component match entry with release callback
 * @master: device with the aggregate driver
 * @matchptr: pointer to the list of component matches
 * @release: release function for @compare_data
 * @compare: compare function to match against all components
 * @compare_data: opaque pointer passed to the @compare function
 *
 * Adds a new component match to the list stored in @matchptr, which the @master
 * aggregate driver needs to function. The list of component matches pointed to
 * by @matchptr must be initialized to NULL before adding the first match. This
 * only matches against components added with component_add().
 *
 * The allocated match list in @matchptr is automatically released using devm
 * actions, where upon @release will be called to free any references held by
 * @compare_data, e.g. when @compare_data is a &device_node that must be
 * released with of_node_put().
 *
 * See also component_match_add() and component_match_add_typed().
 */
void component_match_add_release(struct device *master,
	struct component_match **matchptr,
	void (*release)(struct device *, void *),
	int (*compare)(struct device *, void *), void *compare_data)
{
	__component_match_add(master, matchptr, release, compare, NULL,
			      compare_data);
}
EXPORT_SYMBOL(component_match_add_release);

/**
 * component_match_add_typed - add a component match entry for a typed component
 * @master: device with the aggregate driver
 * @matchptr: pointer to the list of component matches
 * @compare_typed: compare function to match against all typed components
 * @compare_data: opaque pointer passed to the @compare function
 *
 * Adds a new component match to the list stored in @matchptr, which the @master
 * aggregate driver needs to function. The list of component matches pointed to
 * by @matchptr must be initialized to NULL before adding the first match. This
 * only matches against components added with component_add_typed().
 *
 * The allocated match list in @matchptr is automatically released using devm
 * actions.
 *
 * See also component_match_add_release() and component_match_add_typed().
 */
void component_match_add_typed(struct device *master,
	struct component_match **matchptr,
	int (*compare_typed)(struct device *, int, void *), void *compare_data)
{
	__component_match_add(master, matchptr, NULL, NULL, compare_typed,
			      compare_data);
}
EXPORT_SYMBOL(component_match_add_typed);

>>>>>>> upstream/android-13
static void free_master(struct master *master)
{
	struct component_match *match = master->match;
	int i;

	component_master_debugfs_del(master);
	list_del(&master->node);

	if (match) {
		for (i = 0; i < match->num; i++) {
			struct component *c = match->compare[i].component;
			if (c)
				c->master = NULL;
		}
	}

	kfree(master);
}

<<<<<<< HEAD
int component_master_add_with_match(struct device *dev,
=======
/**
 * component_master_add_with_match - register an aggregate driver
 * @parent: parent device of the aggregate driver
 * @ops: callbacks for the aggregate driver
 * @match: component match list for the aggregate driver
 *
 * Registers a new aggregate driver consisting of the components added to @match
 * by calling one of the component_match_add() functions. Once all components in
 * @match are available, it will be assembled by calling
 * &component_master_ops.bind from @ops. Must be unregistered by calling
 * component_master_del().
 */
int component_master_add_with_match(struct device *parent,
>>>>>>> upstream/android-13
	const struct component_master_ops *ops,
	struct component_match *match)
{
	struct master *master;
	int ret;

	/* Reallocate the match array for its true size */
<<<<<<< HEAD
	ret = component_match_realloc(dev, match, match->num);
=======
	ret = component_match_realloc(match, match->num);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	master = kzalloc(sizeof(*master), GFP_KERNEL);
	if (!master)
		return -ENOMEM;

<<<<<<< HEAD
	master->dev = dev;
=======
	master->parent = parent;
>>>>>>> upstream/android-13
	master->ops = ops;
	master->match = match;

	component_master_debugfs_add(master);
	/* Add to the list of available masters. */
	mutex_lock(&component_mutex);
	list_add(&master->node, &masters);

	ret = try_to_bring_up_master(master, NULL);

	if (ret < 0)
		free_master(master);

	mutex_unlock(&component_mutex);

	return ret < 0 ? ret : 0;
}
EXPORT_SYMBOL_GPL(component_master_add_with_match);

<<<<<<< HEAD
void component_master_del(struct device *dev,
=======
/**
 * component_master_del - unregister an aggregate driver
 * @parent: parent device of the aggregate driver
 * @ops: callbacks for the aggregate driver
 *
 * Unregisters an aggregate driver registered with
 * component_master_add_with_match(). If necessary the aggregate driver is first
 * disassembled by calling &component_master_ops.unbind from @ops.
 */
void component_master_del(struct device *parent,
>>>>>>> upstream/android-13
	const struct component_master_ops *ops)
{
	struct master *master;

	mutex_lock(&component_mutex);
<<<<<<< HEAD
	master = __master_find(dev, ops);
=======
	master = __master_find(parent, ops);
>>>>>>> upstream/android-13
	if (master) {
		take_down_master(master);
		free_master(master);
	}
	mutex_unlock(&component_mutex);
}
EXPORT_SYMBOL_GPL(component_master_del);

static void component_unbind(struct component *component,
	struct master *master, void *data)
{
	WARN_ON(!component->bound);

<<<<<<< HEAD
	component->ops->unbind(component->dev, master->dev, data);
=======
	if (component->ops && component->ops->unbind)
		component->ops->unbind(component->dev, master->parent, data);
>>>>>>> upstream/android-13
	component->bound = false;

	/* Release all resources claimed in the binding of this component */
	devres_release_group(component->dev, component);
}

<<<<<<< HEAD
void component_unbind_all(struct device *master_dev, void *data)
=======
/**
 * component_unbind_all - unbind all components of an aggregate driver
 * @parent: parent device of the aggregate driver
 * @data: opaque pointer, passed to all components
 *
 * Unbinds all components of the aggregate device by passing @data to their
 * &component_ops.unbind functions. Should be called from
 * &component_master_ops.unbind.
 */
void component_unbind_all(struct device *parent, void *data)
>>>>>>> upstream/android-13
{
	struct master *master;
	struct component *c;
	size_t i;

	WARN_ON(!mutex_is_locked(&component_mutex));

<<<<<<< HEAD
	master = __master_find(master_dev, NULL);
=======
	master = __master_find(parent, NULL);
>>>>>>> upstream/android-13
	if (!master)
		return;

	/* Unbind components in reverse order */
	for (i = master->match->num; i--; )
		if (!master->match->compare[i].duplicate) {
			c = master->match->compare[i].component;
			component_unbind(c, master, data);
		}
}
EXPORT_SYMBOL_GPL(component_unbind_all);

static int component_bind(struct component *component, struct master *master,
	void *data)
{
	int ret;

	/*
	 * Each component initialises inside its own devres group.
	 * This allows us to roll-back a failed component without
	 * affecting anything else.
	 */
<<<<<<< HEAD
	if (!devres_open_group(master->dev, NULL, GFP_KERNEL))
=======
	if (!devres_open_group(master->parent, NULL, GFP_KERNEL))
>>>>>>> upstream/android-13
		return -ENOMEM;

	/*
	 * Also open a group for the device itself: this allows us
	 * to release the resources claimed against the sub-device
	 * at the appropriate moment.
	 */
	if (!devres_open_group(component->dev, component, GFP_KERNEL)) {
<<<<<<< HEAD
		devres_release_group(master->dev, NULL);
		return -ENOMEM;
	}

	dev_dbg(master->dev, "binding %s (ops %ps)\n",
		dev_name(component->dev), component->ops);

	ret = component->ops->bind(component->dev, master->dev, data);
=======
		devres_release_group(master->parent, NULL);
		return -ENOMEM;
	}

	dev_dbg(master->parent, "binding %s (ops %ps)\n",
		dev_name(component->dev), component->ops);

	ret = component->ops->bind(component->dev, master->parent, data);
>>>>>>> upstream/android-13
	if (!ret) {
		component->bound = true;

		/*
		 * Close the component device's group so that resources
		 * allocated in the binding are encapsulated for removal
		 * at unbind.  Remove the group on the DRM device as we
		 * can clean those resources up independently.
		 */
		devres_close_group(component->dev, NULL);
<<<<<<< HEAD
		devres_remove_group(master->dev, NULL);

		dev_info(master->dev, "bound %s (ops %ps)\n",
			 dev_name(component->dev), component->ops);
	} else {
		devres_release_group(component->dev, NULL);
		devres_release_group(master->dev, NULL);

		if (ret != -EPROBE_DEFER)
			dev_err(master->dev, "failed to bind %s (ops %ps): %d\n",
=======
		devres_remove_group(master->parent, NULL);

		dev_info(master->parent, "bound %s (ops %ps)\n",
			 dev_name(component->dev), component->ops);
	} else {
		devres_release_group(component->dev, NULL);
		devres_release_group(master->parent, NULL);

		if (ret != -EPROBE_DEFER)
			dev_err(master->parent, "failed to bind %s (ops %ps): %d\n",
>>>>>>> upstream/android-13
				dev_name(component->dev), component->ops, ret);
	}

	return ret;
}

<<<<<<< HEAD
int component_bind_all(struct device *master_dev, void *data)
=======
/**
 * component_bind_all - bind all components of an aggregate driver
 * @parent: parent device of the aggregate driver
 * @data: opaque pointer, passed to all components
 *
 * Binds all components of the aggregate @dev by passing @data to their
 * &component_ops.bind functions. Should be called from
 * &component_master_ops.bind.
 */
int component_bind_all(struct device *parent, void *data)
>>>>>>> upstream/android-13
{
	struct master *master;
	struct component *c;
	size_t i;
	int ret = 0;

	WARN_ON(!mutex_is_locked(&component_mutex));

<<<<<<< HEAD
	master = __master_find(master_dev, NULL);
=======
	master = __master_find(parent, NULL);
>>>>>>> upstream/android-13
	if (!master)
		return -EINVAL;

	/* Bind components in match order */
	for (i = 0; i < master->match->num; i++)
		if (!master->match->compare[i].duplicate) {
			c = master->match->compare[i].component;
			ret = component_bind(c, master, data);
			if (ret)
				break;
		}

	if (ret != 0) {
		for (; i > 0; i--)
			if (!master->match->compare[i - 1].duplicate) {
				c = master->match->compare[i - 1].component;
				component_unbind(c, master, data);
			}
	}

	return ret;
}
EXPORT_SYMBOL_GPL(component_bind_all);

<<<<<<< HEAD
int component_add(struct device *dev, const struct component_ops *ops)
=======
static int __component_add(struct device *dev, const struct component_ops *ops,
	int subcomponent)
>>>>>>> upstream/android-13
{
	struct component *component;
	int ret;

	component = kzalloc(sizeof(*component), GFP_KERNEL);
	if (!component)
		return -ENOMEM;

	component->ops = ops;
	component->dev = dev;
<<<<<<< HEAD
=======
	component->subcomponent = subcomponent;
>>>>>>> upstream/android-13

	dev_dbg(dev, "adding component (ops %ps)\n", ops);

	mutex_lock(&component_mutex);
	list_add_tail(&component->node, &component_list);

	ret = try_to_bring_up_masters(component);
	if (ret < 0) {
		if (component->master)
			remove_component(component->master, component);
		list_del(&component->node);

		kfree(component);
	}
	mutex_unlock(&component_mutex);

	return ret < 0 ? ret : 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(component_add);

=======

/**
 * component_add_typed - register a component
 * @dev: component device
 * @ops: component callbacks
 * @subcomponent: nonzero identifier for subcomponents
 *
 * Register a new component for @dev. Functions in @ops will be call when the
 * aggregate driver is ready to bind the overall driver by calling
 * component_bind_all(). See also &struct component_ops.
 *
 * @subcomponent must be nonzero and is used to differentiate between multiple
 * components registerd on the same device @dev. These components are match
 * using component_match_add_typed().
 *
 * The component needs to be unregistered at driver unload/disconnect by
 * calling component_del().
 *
 * See also component_add().
 */
int component_add_typed(struct device *dev, const struct component_ops *ops,
	int subcomponent)
{
	if (WARN_ON(subcomponent == 0))
		return -EINVAL;

	return __component_add(dev, ops, subcomponent);
}
EXPORT_SYMBOL_GPL(component_add_typed);

/**
 * component_add - register a component
 * @dev: component device
 * @ops: component callbacks
 *
 * Register a new component for @dev. Functions in @ops will be called when the
 * aggregate driver is ready to bind the overall driver by calling
 * component_bind_all(). See also &struct component_ops.
 *
 * The component needs to be unregistered at driver unload/disconnect by
 * calling component_del().
 *
 * See also component_add_typed() for a variant that allows multipled different
 * components on the same device.
 */
int component_add(struct device *dev, const struct component_ops *ops)
{
	return __component_add(dev, ops, 0);
}
EXPORT_SYMBOL_GPL(component_add);

/**
 * component_del - unregister a component
 * @dev: component device
 * @ops: component callbacks
 *
 * Unregister a component added with component_add(). If the component is bound
 * into an aggregate driver, this will force the entire aggregate driver, including
 * all its components, to be unbound.
 */
>>>>>>> upstream/android-13
void component_del(struct device *dev, const struct component_ops *ops)
{
	struct component *c, *component = NULL;

	mutex_lock(&component_mutex);
	list_for_each_entry(c, &component_list, node)
		if (c->dev == dev && c->ops == ops) {
			list_del(&c->node);
			component = c;
			break;
		}

	if (component && component->master) {
		take_down_master(component->master);
		remove_component(component->master, component);
	}

	mutex_unlock(&component_mutex);

	WARN_ON(!component);
	kfree(component);
}
EXPORT_SYMBOL_GPL(component_del);
<<<<<<< HEAD

MODULE_LICENSE("GPL v2");
=======
>>>>>>> upstream/android-13
