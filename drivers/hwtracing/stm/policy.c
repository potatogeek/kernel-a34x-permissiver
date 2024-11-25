// SPDX-License-Identifier: GPL-2.0
/*
 * System Trace Module (STM) master/channel allocation policy management
 * Copyright (c) 2014, Intel Corporation.
 *
 * A master/channel allocation policy allows mapping string identifiers to
 * master and channel ranges, where allocation can be done.
 */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/types.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/configfs.h>
#include <linux/slab.h>
#include <linux/stm.h>
#include "stm.h"

/*
 * STP Master/Channel allocation policy configfs layout.
 */

struct stp_policy {
	struct config_group	group;
	struct stm_device	*stm;
};

struct stp_policy_node {
	struct config_group	group;
	struct stp_policy	*policy;
	unsigned int		first_master;
	unsigned int		last_master;
	unsigned int		first_channel;
	unsigned int		last_channel;
<<<<<<< HEAD
};

=======
	/* this is the one that's exposed to the attributes */
	unsigned char		priv[];
};

void *stp_policy_node_priv(struct stp_policy_node *pn)
{
	if (!pn)
		return NULL;

	return pn->priv;
}

>>>>>>> upstream/android-13
static struct configfs_subsystem stp_policy_subsys;

void stp_policy_node_get_ranges(struct stp_policy_node *policy_node,
				unsigned int *mstart, unsigned int *mend,
				unsigned int *cstart, unsigned int *cend)
{
	*mstart	= policy_node->first_master;
	*mend	= policy_node->last_master;
	*cstart	= policy_node->first_channel;
	*cend	= policy_node->last_channel;
}

<<<<<<< HEAD
static inline char *stp_policy_node_name(struct stp_policy_node *policy_node)
{
	return policy_node->group.cg_item.ci_name ? : "<none>";
}

=======
>>>>>>> upstream/android-13
static inline struct stp_policy *to_stp_policy(struct config_item *item)
{
	return item ?
		container_of(to_config_group(item), struct stp_policy, group) :
		NULL;
}

static inline struct stp_policy_node *
to_stp_policy_node(struct config_item *item)
{
	return item ?
		container_of(to_config_group(item), struct stp_policy_node,
			     group) :
		NULL;
}

<<<<<<< HEAD
=======
void *to_pdrv_policy_node(struct config_item *item)
{
	struct stp_policy_node *node = to_stp_policy_node(item);

	return stp_policy_node_priv(node);
}
EXPORT_SYMBOL_GPL(to_pdrv_policy_node);

>>>>>>> upstream/android-13
static ssize_t
stp_policy_node_masters_show(struct config_item *item, char *page)
{
	struct stp_policy_node *policy_node = to_stp_policy_node(item);
	ssize_t count;

	count = sprintf(page, "%u %u\n", policy_node->first_master,
			policy_node->last_master);

	return count;
}

static ssize_t
stp_policy_node_masters_store(struct config_item *item, const char *page,
			      size_t count)
{
	struct stp_policy_node *policy_node = to_stp_policy_node(item);
	unsigned int first, last;
	struct stm_device *stm;
	char *p = (char *)page;
	ssize_t ret = -ENODEV;

	if (sscanf(p, "%u %u", &first, &last) != 2)
		return -EINVAL;

	mutex_lock(&stp_policy_subsys.su_mutex);
	stm = policy_node->policy->stm;
	if (!stm)
		goto unlock;

	/* must be within [sw_start..sw_end], which is an inclusive range */
	if (first > last || first < stm->data->sw_start ||
	    last > stm->data->sw_end) {
		ret = -ERANGE;
		goto unlock;
	}

	ret = count;
	policy_node->first_master = first;
	policy_node->last_master = last;

unlock:
	mutex_unlock(&stp_policy_subsys.su_mutex);

	return ret;
}

static ssize_t
stp_policy_node_channels_show(struct config_item *item, char *page)
{
	struct stp_policy_node *policy_node = to_stp_policy_node(item);
	ssize_t count;

	count = sprintf(page, "%u %u\n", policy_node->first_channel,
			policy_node->last_channel);

	return count;
}

static ssize_t
stp_policy_node_channels_store(struct config_item *item, const char *page,
			       size_t count)
{
	struct stp_policy_node *policy_node = to_stp_policy_node(item);
	unsigned int first, last;
	struct stm_device *stm;
	char *p = (char *)page;
	ssize_t ret = -ENODEV;

	if (sscanf(p, "%u %u", &first, &last) != 2)
		return -EINVAL;

	mutex_lock(&stp_policy_subsys.su_mutex);
	stm = policy_node->policy->stm;
	if (!stm)
		goto unlock;

	if (first > INT_MAX || last > INT_MAX || first > last ||
	    last >= stm->data->sw_nchannels) {
		ret = -ERANGE;
		goto unlock;
	}

	ret = count;
	policy_node->first_channel = first;
	policy_node->last_channel = last;

unlock:
	mutex_unlock(&stp_policy_subsys.su_mutex);

	return ret;
}

static void stp_policy_node_release(struct config_item *item)
{
<<<<<<< HEAD
	kfree(to_stp_policy_node(item));
=======
	struct stp_policy_node *node = to_stp_policy_node(item);

	kfree(node);
>>>>>>> upstream/android-13
}

static struct configfs_item_operations stp_policy_node_item_ops = {
	.release		= stp_policy_node_release,
};

CONFIGFS_ATTR(stp_policy_node_, masters);
CONFIGFS_ATTR(stp_policy_node_, channels);

static struct configfs_attribute *stp_policy_node_attrs[] = {
	&stp_policy_node_attr_masters,
	&stp_policy_node_attr_channels,
	NULL,
};

static const struct config_item_type stp_policy_type;
static const struct config_item_type stp_policy_node_type;

<<<<<<< HEAD
static struct config_group *
stp_policy_node_make(struct config_group *group, const char *name)
{
	struct stp_policy_node *policy_node, *parent_node;
=======
const struct config_item_type *
get_policy_node_type(struct configfs_attribute **attrs)
{
	struct config_item_type *type;
	struct configfs_attribute **merged;

	type = kmemdup(&stp_policy_node_type, sizeof(stp_policy_node_type),
		       GFP_KERNEL);
	if (!type)
		return NULL;

	merged = memcat_p(stp_policy_node_attrs, attrs);
	if (!merged) {
		kfree(type);
		return NULL;
	}

	type->ct_attrs = merged;

	return type;
}

static struct config_group *
stp_policy_node_make(struct config_group *group, const char *name)
{
	const struct config_item_type *type = &stp_policy_node_type;
	struct stp_policy_node *policy_node, *parent_node;
	const struct stm_protocol_driver *pdrv;
>>>>>>> upstream/android-13
	struct stp_policy *policy;

	if (group->cg_item.ci_type == &stp_policy_type) {
		policy = container_of(group, struct stp_policy, group);
	} else {
		parent_node = container_of(group, struct stp_policy_node,
					   group);
		policy = parent_node->policy;
	}

	if (!policy->stm)
		return ERR_PTR(-ENODEV);

<<<<<<< HEAD
	policy_node = kzalloc(sizeof(struct stp_policy_node), GFP_KERNEL);
	if (!policy_node)
		return ERR_PTR(-ENOMEM);

	config_group_init_type_name(&policy_node->group, name,
				    &stp_policy_node_type);
=======
	pdrv = policy->stm->pdrv;
	policy_node =
		kzalloc(offsetof(struct stp_policy_node, priv[pdrv->priv_sz]),
			GFP_KERNEL);
	if (!policy_node)
		return ERR_PTR(-ENOMEM);

	if (pdrv->policy_node_init)
		pdrv->policy_node_init((void *)policy_node->priv);

	if (policy->stm->pdrv_node_type)
		type = policy->stm->pdrv_node_type;

	config_group_init_type_name(&policy_node->group, name, type);
>>>>>>> upstream/android-13

	policy_node->policy = policy;

	/* default values for the attributes */
	policy_node->first_master = policy->stm->data->sw_start;
	policy_node->last_master = policy->stm->data->sw_end;
	policy_node->first_channel = 0;
	policy_node->last_channel = policy->stm->data->sw_nchannels - 1;

	return &policy_node->group;
}

static void
stp_policy_node_drop(struct config_group *group, struct config_item *item)
{
	config_item_put(item);
}

static struct configfs_group_operations stp_policy_node_group_ops = {
	.make_group	= stp_policy_node_make,
	.drop_item	= stp_policy_node_drop,
};

static const struct config_item_type stp_policy_node_type = {
	.ct_item_ops	= &stp_policy_node_item_ops,
	.ct_group_ops	= &stp_policy_node_group_ops,
	.ct_attrs	= stp_policy_node_attrs,
	.ct_owner	= THIS_MODULE,
};

/*
 * Root group: policies.
 */
static ssize_t stp_policy_device_show(struct config_item *item,
				      char *page)
{
	struct stp_policy *policy = to_stp_policy(item);
	ssize_t count;

	count = sprintf(page, "%s\n",
			(policy && policy->stm) ?
			policy->stm->data->name :
			"<none>");

	return count;
}

CONFIGFS_ATTR_RO(stp_policy_, device);

<<<<<<< HEAD
static struct configfs_attribute *stp_policy_attrs[] = {
	&stp_policy_attr_device,
=======
static ssize_t stp_policy_protocol_show(struct config_item *item,
					char *page)
{
	struct stp_policy *policy = to_stp_policy(item);
	ssize_t count;

	count = sprintf(page, "%s\n",
			(policy && policy->stm) ?
			policy->stm->pdrv->name :
			"<none>");

	return count;
}

CONFIGFS_ATTR_RO(stp_policy_, protocol);

static struct configfs_attribute *stp_policy_attrs[] = {
	&stp_policy_attr_device,
	&stp_policy_attr_protocol,
>>>>>>> upstream/android-13
	NULL,
};

void stp_policy_unbind(struct stp_policy *policy)
{
	struct stm_device *stm = policy->stm;

	/*
	 * stp_policy_release() will not call here if the policy is already
	 * unbound; other users should not either, as no link exists between
	 * this policy and anything else in that case
	 */
	if (WARN_ON_ONCE(!policy->stm))
		return;

	lockdep_assert_held(&stm->policy_mutex);

	stm->policy = NULL;
	policy->stm = NULL;

<<<<<<< HEAD
=======
	/*
	 * Drop the reference on the protocol driver and lose the link.
	 */
	stm_put_protocol(stm->pdrv);
	stm->pdrv = NULL;
>>>>>>> upstream/android-13
	stm_put_device(stm);
}

static void stp_policy_release(struct config_item *item)
{
	struct stp_policy *policy = to_stp_policy(item);
	struct stm_device *stm = policy->stm;

	/* a policy *can* be unbound and still exist in configfs tree */
	if (!stm)
		return;

	mutex_lock(&stm->policy_mutex);
	stp_policy_unbind(policy);
	mutex_unlock(&stm->policy_mutex);

	kfree(policy);
}

static struct configfs_item_operations stp_policy_item_ops = {
	.release		= stp_policy_release,
};

static struct configfs_group_operations stp_policy_group_ops = {
	.make_group	= stp_policy_node_make,
};

static const struct config_item_type stp_policy_type = {
	.ct_item_ops	= &stp_policy_item_ops,
	.ct_group_ops	= &stp_policy_group_ops,
	.ct_attrs	= stp_policy_attrs,
	.ct_owner	= THIS_MODULE,
};

static struct config_group *
<<<<<<< HEAD
stp_policies_make(struct config_group *group, const char *name)
{
	struct config_group *ret;
	struct stm_device *stm;
	char *devname, *p;
=======
stp_policy_make(struct config_group *group, const char *name)
{
	const struct config_item_type *pdrv_node_type;
	const struct stm_protocol_driver *pdrv;
	char *devname, *proto, *p;
	struct config_group *ret;
	struct stm_device *stm;
	int err;
>>>>>>> upstream/android-13

	devname = kasprintf(GFP_KERNEL, "%s", name);
	if (!devname)
		return ERR_PTR(-ENOMEM);

	/*
	 * node must look like <device_name>.<policy_name>, where
	 * <device_name> is the name of an existing stm device; may
	 *               contain dots;
	 * <policy_name> is an arbitrary string; may not contain dots
<<<<<<< HEAD
=======
	 * <device_name>:<protocol_name>.<policy_name>
>>>>>>> upstream/android-13
	 */
	p = strrchr(devname, '.');
	if (!p) {
		kfree(devname);
		return ERR_PTR(-EINVAL);
	}

	*p = '\0';

<<<<<<< HEAD
	stm = stm_find_device(devname);
	kfree(devname);

	if (!stm)
		return ERR_PTR(-ENODEV);
=======
	/*
	 * look for ":<protocol_name>":
	 *  + no protocol suffix: fall back to whatever is available;
	 *  + unknown protocol: fail the whole thing
	 */
	proto = strrchr(devname, ':');
	if (proto)
		*proto++ = '\0';

	stm = stm_find_device(devname);
	if (!stm) {
		kfree(devname);
		return ERR_PTR(-ENODEV);
	}

	err = stm_lookup_protocol(proto, &pdrv, &pdrv_node_type);
	kfree(devname);

	if (err) {
		stm_put_device(stm);
		return ERR_PTR(-ENODEV);
	}
>>>>>>> upstream/android-13

	mutex_lock(&stm->policy_mutex);
	if (stm->policy) {
		ret = ERR_PTR(-EBUSY);
		goto unlock_policy;
	}

	stm->policy = kzalloc(sizeof(*stm->policy), GFP_KERNEL);
	if (!stm->policy) {
		ret = ERR_PTR(-ENOMEM);
		goto unlock_policy;
	}

	config_group_init_type_name(&stm->policy->group, name,
				    &stp_policy_type);
<<<<<<< HEAD
	stm->policy->stm = stm;

=======

	stm->pdrv = pdrv;
	stm->pdrv_node_type = pdrv_node_type;
	stm->policy->stm = stm;
>>>>>>> upstream/android-13
	ret = &stm->policy->group;

unlock_policy:
	mutex_unlock(&stm->policy_mutex);

<<<<<<< HEAD
	if (IS_ERR(ret))
		stm_put_device(stm);
=======
	if (IS_ERR(ret)) {
		/*
		 * pdrv and stm->pdrv at this point can be quite different,
		 * and only one of them needs to be 'put'
		 */
		stm_put_protocol(pdrv);
		stm_put_device(stm);
	}
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static struct configfs_group_operations stp_policies_group_ops = {
	.make_group	= stp_policies_make,
};

static const struct config_item_type stp_policies_type = {
	.ct_group_ops	= &stp_policies_group_ops,
=======
static struct configfs_group_operations stp_policy_root_group_ops = {
	.make_group	= stp_policy_make,
};

static const struct config_item_type stp_policy_root_type = {
	.ct_group_ops	= &stp_policy_root_group_ops,
>>>>>>> upstream/android-13
	.ct_owner	= THIS_MODULE,
};

static struct configfs_subsystem stp_policy_subsys = {
	.su_group = {
		.cg_item = {
			.ci_namebuf	= "stp-policy",
<<<<<<< HEAD
			.ci_type	= &stp_policies_type,
=======
			.ci_type	= &stp_policy_root_type,
>>>>>>> upstream/android-13
		},
	},
};

/*
 * Lock the policy mutex from the outside
 */
static struct stp_policy_node *
__stp_policy_node_lookup(struct stp_policy *policy, char *s)
{
<<<<<<< HEAD
	struct stp_policy_node *policy_node, *ret;
=======
	struct stp_policy_node *policy_node, *ret = NULL;
>>>>>>> upstream/android-13
	struct list_head *head = &policy->group.cg_children;
	struct config_item *item;
	char *start, *end = s;

	if (list_empty(head))
		return NULL;

<<<<<<< HEAD
	/* return the first entry if everything else fails */
	item = list_entry(head->next, struct config_item, ci_entry);
	ret = to_stp_policy_node(item);

=======
>>>>>>> upstream/android-13
next:
	for (;;) {
		start = strsep(&end, "/");
		if (!start)
			break;

		if (!*start)
			continue;

		list_for_each_entry(item, head, ci_entry) {
			policy_node = to_stp_policy_node(item);

			if (!strcmp(start,
				    policy_node->group.cg_item.ci_name)) {
				ret = policy_node;

				if (!end)
					goto out;

				head = &policy_node->group.cg_children;
				goto next;
			}
		}
		break;
	}

out:
	return ret;
}


struct stp_policy_node *
stp_policy_node_lookup(struct stm_device *stm, char *s)
{
	struct stp_policy_node *policy_node = NULL;

	mutex_lock(&stp_policy_subsys.su_mutex);

	mutex_lock(&stm->policy_mutex);
	if (stm->policy)
		policy_node = __stp_policy_node_lookup(stm->policy, s);
	mutex_unlock(&stm->policy_mutex);

	if (policy_node)
		config_item_get(&policy_node->group.cg_item);
<<<<<<< HEAD
	mutex_unlock(&stp_policy_subsys.su_mutex);
=======
	else
		mutex_unlock(&stp_policy_subsys.su_mutex);
>>>>>>> upstream/android-13

	return policy_node;
}

void stp_policy_node_put(struct stp_policy_node *policy_node)
{
<<<<<<< HEAD
=======
	lockdep_assert_held(&stp_policy_subsys.su_mutex);

	mutex_unlock(&stp_policy_subsys.su_mutex);
>>>>>>> upstream/android-13
	config_item_put(&policy_node->group.cg_item);
}

int __init stp_configfs_init(void)
{
<<<<<<< HEAD
	int err;

	config_group_init(&stp_policy_subsys.su_group);
	mutex_init(&stp_policy_subsys.su_mutex);
	err = configfs_register_subsystem(&stp_policy_subsys);

	return err;
=======
	config_group_init(&stp_policy_subsys.su_group);
	mutex_init(&stp_policy_subsys.su_mutex);
	return configfs_register_subsystem(&stp_policy_subsys);
>>>>>>> upstream/android-13
}

void __exit stp_configfs_exit(void)
{
	configfs_unregister_subsystem(&stp_policy_subsys);
}
