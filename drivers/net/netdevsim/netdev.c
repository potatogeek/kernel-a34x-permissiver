/*
 * Copyright (C) 2017 Netronome Systems, Inc.
 *
 * This software is licensed under the GNU General License Version 2,
 * June 1991 as shown in the file COPYING in the top-level directory of this
 * source tree.
 *
 * THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM "AS IS"
 * WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE
 * OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME
 * THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
 */

#include <linux/debugfs.h>
#include <linux/etherdevice.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/slab.h>
#include <net/netlink.h>
#include <net/pkt_cls.h>
#include <net/rtnetlink.h>
<<<<<<< HEAD
#include <net/switchdev.h>

#include "netdevsim.h"

struct nsim_vf_config {
	int link_state;
	u16 min_tx_rate;
	u16 max_tx_rate;
	u16 vlan;
	__be16 vlan_proto;
	u16 qos;
	u8 vf_mac[ETH_ALEN];
	bool spoofchk_enabled;
	bool trusted;
	bool rss_query_enabled;
};

static u32 nsim_dev_id;

static struct dentry *nsim_ddir;
static struct dentry *nsim_sdev_ddir;

static int nsim_num_vf(struct device *dev)
{
	struct netdevsim *ns = to_nsim(dev);

	return ns->num_vfs;
}

static struct bus_type nsim_bus = {
	.name		= DRV_NAME,
	.dev_name	= DRV_NAME,
	.num_vf		= nsim_num_vf,
};

static int nsim_vfs_enable(struct netdevsim *ns, unsigned int num_vfs)
{
	ns->vfconfigs = kcalloc(num_vfs, sizeof(struct nsim_vf_config),
				GFP_KERNEL);
	if (!ns->vfconfigs)
		return -ENOMEM;
	ns->num_vfs = num_vfs;

	return 0;
}

static void nsim_vfs_disable(struct netdevsim *ns)
{
	kfree(ns->vfconfigs);
	ns->vfconfigs = NULL;
	ns->num_vfs = 0;
}

static ssize_t
nsim_numvfs_store(struct device *dev, struct device_attribute *attr,
		  const char *buf, size_t count)
{
	struct netdevsim *ns = to_nsim(dev);
	unsigned int num_vfs;
	int ret;

	ret = kstrtouint(buf, 0, &num_vfs);
	if (ret)
		return ret;

	rtnl_lock();
	if (ns->num_vfs == num_vfs)
		goto exit_good;
	if (ns->num_vfs && num_vfs) {
		ret = -EBUSY;
		goto exit_unlock;
	}

	if (num_vfs) {
		ret = nsim_vfs_enable(ns, num_vfs);
		if (ret)
			goto exit_unlock;
	} else {
		nsim_vfs_disable(ns);
	}
exit_good:
	ret = count;
exit_unlock:
	rtnl_unlock();

	return ret;
}

static ssize_t
nsim_numvfs_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct netdevsim *ns = to_nsim(dev);

	return sprintf(buf, "%u\n", ns->num_vfs);
}

static struct device_attribute nsim_numvfs_attr =
	__ATTR(sriov_numvfs, 0664, nsim_numvfs_show, nsim_numvfs_store);

static struct attribute *nsim_dev_attrs[] = {
	&nsim_numvfs_attr.attr,
	NULL,
};

static const struct attribute_group nsim_dev_attr_group = {
	.attrs = nsim_dev_attrs,
};

static const struct attribute_group *nsim_dev_attr_groups[] = {
	&nsim_dev_attr_group,
	NULL,
};

static void nsim_dev_release(struct device *dev)
{
	struct netdevsim *ns = to_nsim(dev);

	nsim_vfs_disable(ns);
	free_netdev(ns->netdev);
}

static struct device_type nsim_dev_type = {
	.groups = nsim_dev_attr_groups,
	.release = nsim_dev_release,
};

static int
nsim_port_attr_get(struct net_device *dev, struct switchdev_attr *attr)
{
	struct netdevsim *ns = netdev_priv(dev);

	switch (attr->id) {
	case SWITCHDEV_ATTR_ID_PORT_PARENT_ID:
		attr->u.ppid.id_len = sizeof(ns->sdev->switch_id);
		memcpy(&attr->u.ppid.id, &ns->sdev->switch_id,
		       attr->u.ppid.id_len);
		return 0;
	default:
		return -EOPNOTSUPP;
	}
}

static const struct switchdev_ops nsim_switchdev_ops = {
	.switchdev_port_attr_get	= nsim_port_attr_get,
};

static int nsim_init(struct net_device *dev)
{
	char sdev_ddir_name[10], sdev_link_name[32];
	struct netdevsim *ns = netdev_priv(dev);
	int err;

	ns->netdev = dev;
	ns->ddir = debugfs_create_dir(netdev_name(dev), nsim_ddir);
	if (IS_ERR_OR_NULL(ns->ddir))
		return -ENOMEM;

	if (!ns->sdev) {
		ns->sdev = kzalloc(sizeof(*ns->sdev), GFP_KERNEL);
		if (!ns->sdev) {
			err = -ENOMEM;
			goto err_debugfs_destroy;
		}
		ns->sdev->refcnt = 1;
		ns->sdev->switch_id = nsim_dev_id;
		sprintf(sdev_ddir_name, "%u", ns->sdev->switch_id);
		ns->sdev->ddir = debugfs_create_dir(sdev_ddir_name,
						    nsim_sdev_ddir);
		if (IS_ERR_OR_NULL(ns->sdev->ddir)) {
			err = PTR_ERR_OR_ZERO(ns->sdev->ddir) ?: -EINVAL;
			goto err_sdev_free;
		}
	} else {
		sprintf(sdev_ddir_name, "%u", ns->sdev->switch_id);
		ns->sdev->refcnt++;
	}

	sprintf(sdev_link_name, "../../" DRV_NAME "_sdev/%s", sdev_ddir_name);
	debugfs_create_symlink("sdev", ns->ddir, sdev_link_name);

	err = nsim_bpf_init(ns);
	if (err)
		goto err_sdev_destroy;

	ns->dev.id = nsim_dev_id++;
	ns->dev.bus = &nsim_bus;
	ns->dev.type = &nsim_dev_type;
	err = device_register(&ns->dev);
	if (err)
		goto err_bpf_uninit;

	SET_NETDEV_DEV(dev, &ns->dev);
	SWITCHDEV_SET_OPS(dev, &nsim_switchdev_ops);

	err = nsim_devlink_setup(ns);
	if (err)
		goto err_unreg_dev;

	nsim_ipsec_init(ns);

	return 0;

err_unreg_dev:
	device_unregister(&ns->dev);
err_bpf_uninit:
	nsim_bpf_uninit(ns);
err_sdev_destroy:
	if (!--ns->sdev->refcnt) {
		debugfs_remove_recursive(ns->sdev->ddir);
err_sdev_free:
		kfree(ns->sdev);
	}
err_debugfs_destroy:
	debugfs_remove_recursive(ns->ddir);
	return err;
}

static void nsim_uninit(struct net_device *dev)
{
	struct netdevsim *ns = netdev_priv(dev);

	nsim_ipsec_teardown(ns);
	nsim_devlink_teardown(ns);
	debugfs_remove_recursive(ns->ddir);
	nsim_bpf_uninit(ns);
	if (!--ns->sdev->refcnt) {
		debugfs_remove_recursive(ns->sdev->ddir);
		kfree(ns->sdev);
	}
}

static void nsim_free(struct net_device *dev)
{
	struct netdevsim *ns = netdev_priv(dev);

	device_unregister(&ns->dev);
	/* netdev and vf state will be freed out of device_release() */
}

=======
#include <net/udp_tunnel.h>

#include "netdevsim.h"

>>>>>>> upstream/android-13
static netdev_tx_t nsim_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct netdevsim *ns = netdev_priv(dev);

	if (!nsim_ipsec_tx(ns, skb))
		goto out;

	u64_stats_update_begin(&ns->syncp);
	ns->tx_packets++;
	ns->tx_bytes += skb->len;
	u64_stats_update_end(&ns->syncp);

out:
	dev_kfree_skb(skb);

	return NETDEV_TX_OK;
}

static void nsim_set_rx_mode(struct net_device *dev)
{
}

static int nsim_change_mtu(struct net_device *dev, int new_mtu)
{
	struct netdevsim *ns = netdev_priv(dev);

	if (ns->xdp.prog && new_mtu > NSIM_XDP_MAX_MTU)
		return -EBUSY;

	dev->mtu = new_mtu;

	return 0;
}

static void
nsim_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *stats)
{
	struct netdevsim *ns = netdev_priv(dev);
	unsigned int start;

	do {
		start = u64_stats_fetch_begin(&ns->syncp);
		stats->tx_bytes = ns->tx_bytes;
		stats->tx_packets = ns->tx_packets;
	} while (u64_stats_fetch_retry(&ns->syncp, start));
}

static int
nsim_setup_tc_block_cb(enum tc_setup_type type, void *type_data, void *cb_priv)
{
	return nsim_bpf_setup_tc_block_cb(type, type_data, cb_priv);
}

<<<<<<< HEAD
static int
nsim_setup_tc_block(struct net_device *dev, struct tc_block_offload *f)
{
	struct netdevsim *ns = netdev_priv(dev);

	if (f->binder_type != TCF_BLOCK_BINDER_TYPE_CLSACT_INGRESS)
		return -EOPNOTSUPP;

	switch (f->command) {
	case TC_BLOCK_BIND:
		return tcf_block_cb_register(f->block, nsim_setup_tc_block_cb,
					     ns, ns, f->extack);
	case TC_BLOCK_UNBIND:
		tcf_block_cb_unregister(f->block, nsim_setup_tc_block_cb, ns);
		return 0;
	default:
		return -EOPNOTSUPP;
	}
}

static int nsim_set_vf_mac(struct net_device *dev, int vf, u8 *mac)
{
	struct netdevsim *ns = netdev_priv(dev);

	/* Only refuse multicast addresses, zero address can mean unset/any. */
	if (vf >= ns->num_vfs || is_multicast_ether_addr(mac))
		return -EINVAL;
	memcpy(ns->vfconfigs[vf].vf_mac, mac, ETH_ALEN);
=======
static int nsim_set_vf_mac(struct net_device *dev, int vf, u8 *mac)
{
	struct netdevsim *ns = netdev_priv(dev);
	struct nsim_bus_dev *nsim_bus_dev = ns->nsim_bus_dev;

	/* Only refuse multicast addresses, zero address can mean unset/any. */
	if (vf >= nsim_bus_dev->num_vfs || is_multicast_ether_addr(mac))
		return -EINVAL;
	memcpy(nsim_bus_dev->vfconfigs[vf].vf_mac, mac, ETH_ALEN);
>>>>>>> upstream/android-13

	return 0;
}

static int nsim_set_vf_vlan(struct net_device *dev, int vf,
			    u16 vlan, u8 qos, __be16 vlan_proto)
{
	struct netdevsim *ns = netdev_priv(dev);
<<<<<<< HEAD

	if (vf >= ns->num_vfs || vlan > 4095 || qos > 7)
		return -EINVAL;

	ns->vfconfigs[vf].vlan = vlan;
	ns->vfconfigs[vf].qos = qos;
	ns->vfconfigs[vf].vlan_proto = vlan_proto;
=======
	struct nsim_bus_dev *nsim_bus_dev = ns->nsim_bus_dev;

	if (vf >= nsim_bus_dev->num_vfs || vlan > 4095 || qos > 7)
		return -EINVAL;

	nsim_bus_dev->vfconfigs[vf].vlan = vlan;
	nsim_bus_dev->vfconfigs[vf].qos = qos;
	nsim_bus_dev->vfconfigs[vf].vlan_proto = vlan_proto;
>>>>>>> upstream/android-13

	return 0;
}

static int nsim_set_vf_rate(struct net_device *dev, int vf, int min, int max)
{
	struct netdevsim *ns = netdev_priv(dev);
<<<<<<< HEAD

	if (vf >= ns->num_vfs)
		return -EINVAL;

	ns->vfconfigs[vf].min_tx_rate = min;
	ns->vfconfigs[vf].max_tx_rate = max;
=======
	struct nsim_bus_dev *nsim_bus_dev = ns->nsim_bus_dev;

	if (nsim_esw_mode_is_switchdev(ns->nsim_dev)) {
		pr_err("Not supported in switchdev mode. Please use devlink API.\n");
		return -EOPNOTSUPP;
	}

	if (vf >= nsim_bus_dev->num_vfs)
		return -EINVAL;

	nsim_bus_dev->vfconfigs[vf].min_tx_rate = min;
	nsim_bus_dev->vfconfigs[vf].max_tx_rate = max;
>>>>>>> upstream/android-13

	return 0;
}

static int nsim_set_vf_spoofchk(struct net_device *dev, int vf, bool val)
{
	struct netdevsim *ns = netdev_priv(dev);
<<<<<<< HEAD

	if (vf >= ns->num_vfs)
		return -EINVAL;
	ns->vfconfigs[vf].spoofchk_enabled = val;
=======
	struct nsim_bus_dev *nsim_bus_dev = ns->nsim_bus_dev;

	if (vf >= nsim_bus_dev->num_vfs)
		return -EINVAL;
	nsim_bus_dev->vfconfigs[vf].spoofchk_enabled = val;
>>>>>>> upstream/android-13

	return 0;
}

static int nsim_set_vf_rss_query_en(struct net_device *dev, int vf, bool val)
{
	struct netdevsim *ns = netdev_priv(dev);
<<<<<<< HEAD

	if (vf >= ns->num_vfs)
		return -EINVAL;
	ns->vfconfigs[vf].rss_query_enabled = val;
=======
	struct nsim_bus_dev *nsim_bus_dev = ns->nsim_bus_dev;

	if (vf >= nsim_bus_dev->num_vfs)
		return -EINVAL;
	nsim_bus_dev->vfconfigs[vf].rss_query_enabled = val;
>>>>>>> upstream/android-13

	return 0;
}

static int nsim_set_vf_trust(struct net_device *dev, int vf, bool val)
{
	struct netdevsim *ns = netdev_priv(dev);
<<<<<<< HEAD

	if (vf >= ns->num_vfs)
		return -EINVAL;
	ns->vfconfigs[vf].trusted = val;
=======
	struct nsim_bus_dev *nsim_bus_dev = ns->nsim_bus_dev;

	if (vf >= nsim_bus_dev->num_vfs)
		return -EINVAL;
	nsim_bus_dev->vfconfigs[vf].trusted = val;
>>>>>>> upstream/android-13

	return 0;
}

static int
nsim_get_vf_config(struct net_device *dev, int vf, struct ifla_vf_info *ivi)
{
	struct netdevsim *ns = netdev_priv(dev);
<<<<<<< HEAD

	if (vf >= ns->num_vfs)
		return -EINVAL;

	ivi->vf = vf;
	ivi->linkstate = ns->vfconfigs[vf].link_state;
	ivi->min_tx_rate = ns->vfconfigs[vf].min_tx_rate;
	ivi->max_tx_rate = ns->vfconfigs[vf].max_tx_rate;
	ivi->vlan = ns->vfconfigs[vf].vlan;
	ivi->vlan_proto = ns->vfconfigs[vf].vlan_proto;
	ivi->qos = ns->vfconfigs[vf].qos;
	memcpy(&ivi->mac, ns->vfconfigs[vf].vf_mac, ETH_ALEN);
	ivi->spoofchk = ns->vfconfigs[vf].spoofchk_enabled;
	ivi->trusted = ns->vfconfigs[vf].trusted;
	ivi->rss_query_en = ns->vfconfigs[vf].rss_query_enabled;
=======
	struct nsim_bus_dev *nsim_bus_dev = ns->nsim_bus_dev;

	if (vf >= nsim_bus_dev->num_vfs)
		return -EINVAL;

	ivi->vf = vf;
	ivi->linkstate = nsim_bus_dev->vfconfigs[vf].link_state;
	ivi->min_tx_rate = nsim_bus_dev->vfconfigs[vf].min_tx_rate;
	ivi->max_tx_rate = nsim_bus_dev->vfconfigs[vf].max_tx_rate;
	ivi->vlan = nsim_bus_dev->vfconfigs[vf].vlan;
	ivi->vlan_proto = nsim_bus_dev->vfconfigs[vf].vlan_proto;
	ivi->qos = nsim_bus_dev->vfconfigs[vf].qos;
	memcpy(&ivi->mac, nsim_bus_dev->vfconfigs[vf].vf_mac, ETH_ALEN);
	ivi->spoofchk = nsim_bus_dev->vfconfigs[vf].spoofchk_enabled;
	ivi->trusted = nsim_bus_dev->vfconfigs[vf].trusted;
	ivi->rss_query_en = nsim_bus_dev->vfconfigs[vf].rss_query_enabled;
>>>>>>> upstream/android-13

	return 0;
}

static int nsim_set_vf_link_state(struct net_device *dev, int vf, int state)
{
	struct netdevsim *ns = netdev_priv(dev);
<<<<<<< HEAD

	if (vf >= ns->num_vfs)
=======
	struct nsim_bus_dev *nsim_bus_dev = ns->nsim_bus_dev;

	if (vf >= nsim_bus_dev->num_vfs)
>>>>>>> upstream/android-13
		return -EINVAL;

	switch (state) {
	case IFLA_VF_LINK_STATE_AUTO:
	case IFLA_VF_LINK_STATE_ENABLE:
	case IFLA_VF_LINK_STATE_DISABLE:
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	ns->vfconfigs[vf].link_state = state;
=======
	nsim_bus_dev->vfconfigs[vf].link_state = state;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int
nsim_setup_tc(struct net_device *dev, enum tc_setup_type type, void *type_data)
{
	switch (type) {
	case TC_SETUP_BLOCK:
		return nsim_setup_tc_block(dev, type_data);
=======
static LIST_HEAD(nsim_block_cb_list);

static int
nsim_setup_tc(struct net_device *dev, enum tc_setup_type type, void *type_data)
{
	struct netdevsim *ns = netdev_priv(dev);

	switch (type) {
	case TC_SETUP_BLOCK:
		return flow_block_cb_setup_simple(type_data,
						  &nsim_block_cb_list,
						  nsim_setup_tc_block_cb,
						  ns, ns, true);
>>>>>>> upstream/android-13
	default:
		return -EOPNOTSUPP;
	}
}

static int
nsim_set_features(struct net_device *dev, netdev_features_t features)
{
	struct netdevsim *ns = netdev_priv(dev);

	if ((dev->features & NETIF_F_HW_TC) > (features & NETIF_F_HW_TC))
		return nsim_bpf_disable_tc(ns);

	return 0;
}

<<<<<<< HEAD
static const struct net_device_ops nsim_netdev_ops = {
	.ndo_init		= nsim_init,
	.ndo_uninit		= nsim_uninit,
=======
static struct devlink_port *nsim_get_devlink_port(struct net_device *dev)
{
	struct netdevsim *ns = netdev_priv(dev);

	return &ns->nsim_dev_port->devlink_port;
}

static const struct net_device_ops nsim_netdev_ops = {
>>>>>>> upstream/android-13
	.ndo_start_xmit		= nsim_start_xmit,
	.ndo_set_rx_mode	= nsim_set_rx_mode,
	.ndo_set_mac_address	= eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_change_mtu		= nsim_change_mtu,
	.ndo_get_stats64	= nsim_get_stats64,
	.ndo_set_vf_mac		= nsim_set_vf_mac,
	.ndo_set_vf_vlan	= nsim_set_vf_vlan,
	.ndo_set_vf_rate	= nsim_set_vf_rate,
	.ndo_set_vf_spoofchk	= nsim_set_vf_spoofchk,
	.ndo_set_vf_trust	= nsim_set_vf_trust,
	.ndo_get_vf_config	= nsim_get_vf_config,
	.ndo_set_vf_link_state	= nsim_set_vf_link_state,
	.ndo_set_vf_rss_query_en = nsim_set_vf_rss_query_en,
	.ndo_setup_tc		= nsim_setup_tc,
	.ndo_set_features	= nsim_set_features,
	.ndo_bpf		= nsim_bpf,
<<<<<<< HEAD
=======
	.ndo_get_devlink_port	= nsim_get_devlink_port,
};

static const struct net_device_ops nsim_vf_netdev_ops = {
	.ndo_start_xmit		= nsim_start_xmit,
	.ndo_set_rx_mode	= nsim_set_rx_mode,
	.ndo_set_mac_address	= eth_mac_addr,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_change_mtu		= nsim_change_mtu,
	.ndo_get_stats64	= nsim_get_stats64,
	.ndo_setup_tc		= nsim_setup_tc,
	.ndo_set_features	= nsim_set_features,
	.ndo_get_devlink_port	= nsim_get_devlink_port,
>>>>>>> upstream/android-13
};

static void nsim_setup(struct net_device *dev)
{
	ether_setup(dev);
	eth_hw_addr_random(dev);

<<<<<<< HEAD
	dev->netdev_ops = &nsim_netdev_ops;
	dev->priv_destructor = nsim_free;

=======
>>>>>>> upstream/android-13
	dev->tx_queue_len = 0;
	dev->flags |= IFF_NOARP;
	dev->flags &= ~IFF_MULTICAST;
	dev->priv_flags |= IFF_LIVE_ADDR_CHANGE |
			   IFF_NO_QUEUE;
	dev->features |= NETIF_F_HIGHDMA |
			 NETIF_F_SG |
			 NETIF_F_FRAGLIST |
			 NETIF_F_HW_CSUM |
			 NETIF_F_TSO;
	dev->hw_features |= NETIF_F_HW_TC;
	dev->max_mtu = ETH_MAX_MTU;
}

<<<<<<< HEAD
static int nsim_validate(struct nlattr *tb[], struct nlattr *data[],
			 struct netlink_ext_ack *extack)
{
	if (tb[IFLA_ADDRESS]) {
		if (nla_len(tb[IFLA_ADDRESS]) != ETH_ALEN)
			return -EINVAL;
		if (!is_valid_ether_addr(nla_data(tb[IFLA_ADDRESS])))
			return -EADDRNOTAVAIL;
	}
	return 0;
}

static int nsim_newlink(struct net *src_net, struct net_device *dev,
			struct nlattr *tb[], struct nlattr *data[],
			struct netlink_ext_ack *extack)
{
	struct netdevsim *ns = netdev_priv(dev);

	if (tb[IFLA_LINK]) {
		struct net_device *joindev;
		struct netdevsim *joinns;

		joindev = __dev_get_by_index(src_net,
					     nla_get_u32(tb[IFLA_LINK]));
		if (!joindev)
			return -ENODEV;
		if (joindev->netdev_ops != &nsim_netdev_ops)
			return -EINVAL;

		joinns = netdev_priv(joindev);
		if (!joinns->sdev || !joinns->sdev->refcnt)
			return -EINVAL;
		ns->sdev = joinns->sdev;
	}

	return register_netdevice(dev);
}

static void nsim_dellink(struct net_device *dev, struct list_head *head)
{
	unregister_netdevice_queue(dev, head);
=======
static int nsim_init_netdevsim(struct netdevsim *ns)
{
	int err;

	ns->netdev->netdev_ops = &nsim_netdev_ops;

	err = nsim_udp_tunnels_info_create(ns->nsim_dev, ns->netdev);
	if (err)
		return err;

	rtnl_lock();
	err = nsim_bpf_init(ns);
	if (err)
		goto err_utn_destroy;

	nsim_ipsec_init(ns);

	err = register_netdevice(ns->netdev);
	if (err)
		goto err_ipsec_teardown;
	rtnl_unlock();
	return 0;

err_ipsec_teardown:
	nsim_ipsec_teardown(ns);
	nsim_bpf_uninit(ns);
err_utn_destroy:
	rtnl_unlock();
	nsim_udp_tunnels_info_destroy(ns->netdev);
	return err;
}

static int nsim_init_netdevsim_vf(struct netdevsim *ns)
{
	int err;

	ns->netdev->netdev_ops = &nsim_vf_netdev_ops;
	rtnl_lock();
	err = register_netdevice(ns->netdev);
	rtnl_unlock();
	return err;
}

struct netdevsim *
nsim_create(struct nsim_dev *nsim_dev, struct nsim_dev_port *nsim_dev_port)
{
	struct net_device *dev;
	struct netdevsim *ns;
	int err;

	dev = alloc_netdev_mq(sizeof(*ns), "eth%d", NET_NAME_UNKNOWN, nsim_setup,
			      nsim_dev->nsim_bus_dev->num_queues);
	if (!dev)
		return ERR_PTR(-ENOMEM);

	dev_net_set(dev, nsim_dev_net(nsim_dev));
	ns = netdev_priv(dev);
	ns->netdev = dev;
	u64_stats_init(&ns->syncp);
	ns->nsim_dev = nsim_dev;
	ns->nsim_dev_port = nsim_dev_port;
	ns->nsim_bus_dev = nsim_dev->nsim_bus_dev;
	SET_NETDEV_DEV(dev, &ns->nsim_bus_dev->dev);
	nsim_ethtool_init(ns);
	if (nsim_dev_port_is_pf(nsim_dev_port))
		err = nsim_init_netdevsim(ns);
	else
		err = nsim_init_netdevsim_vf(ns);
	if (err)
		goto err_free_netdev;
	return ns;

err_free_netdev:
	free_netdev(dev);
	return ERR_PTR(err);
}

void nsim_destroy(struct netdevsim *ns)
{
	struct net_device *dev = ns->netdev;

	rtnl_lock();
	unregister_netdevice(dev);
	if (nsim_dev_port_is_pf(ns->nsim_dev_port)) {
		nsim_ipsec_teardown(ns);
		nsim_bpf_uninit(ns);
	}
	rtnl_unlock();
	if (nsim_dev_port_is_pf(ns->nsim_dev_port))
		nsim_udp_tunnels_info_destroy(dev);
	free_netdev(dev);
}

static int nsim_validate(struct nlattr *tb[], struct nlattr *data[],
			 struct netlink_ext_ack *extack)
{
	NL_SET_ERR_MSG_MOD(extack,
			   "Please use: echo \"[ID] [PORT_COUNT] [NUM_QUEUES]\" > /sys/bus/netdevsim/new_device");
	return -EOPNOTSUPP;
>>>>>>> upstream/android-13
}

static struct rtnl_link_ops nsim_link_ops __read_mostly = {
	.kind		= DRV_NAME,
<<<<<<< HEAD
	.priv_size	= sizeof(struct netdevsim),
	.setup		= nsim_setup,
	.validate	= nsim_validate,
	.newlink	= nsim_newlink,
	.dellink	= nsim_dellink,
=======
	.validate	= nsim_validate,
>>>>>>> upstream/android-13
};

static int __init nsim_module_init(void)
{
	int err;

<<<<<<< HEAD
	nsim_ddir = debugfs_create_dir(DRV_NAME, NULL);
	if (IS_ERR_OR_NULL(nsim_ddir))
		return -ENOMEM;

	nsim_sdev_ddir = debugfs_create_dir(DRV_NAME "_sdev", NULL);
	if (IS_ERR_OR_NULL(nsim_sdev_ddir)) {
		err = -ENOMEM;
		goto err_debugfs_destroy;
	}

	err = bus_register(&nsim_bus);
	if (err)
		goto err_sdir_destroy;

	err = nsim_devlink_init();
	if (err)
		goto err_unreg_bus;

	err = rtnl_link_register(&nsim_link_ops);
	if (err)
		goto err_dl_fini;

	return 0;

err_dl_fini:
	nsim_devlink_exit();
err_unreg_bus:
	bus_unregister(&nsim_bus);
err_sdir_destroy:
	debugfs_remove_recursive(nsim_sdev_ddir);
err_debugfs_destroy:
	debugfs_remove_recursive(nsim_ddir);
=======
	err = nsim_dev_init();
	if (err)
		return err;

	err = nsim_bus_init();
	if (err)
		goto err_dev_exit;

	err = rtnl_link_register(&nsim_link_ops);
	if (err)
		goto err_bus_exit;

	return 0;

err_bus_exit:
	nsim_bus_exit();
err_dev_exit:
	nsim_dev_exit();
>>>>>>> upstream/android-13
	return err;
}

static void __exit nsim_module_exit(void)
{
	rtnl_link_unregister(&nsim_link_ops);
<<<<<<< HEAD
	nsim_devlink_exit();
	bus_unregister(&nsim_bus);
	debugfs_remove_recursive(nsim_sdev_ddir);
	debugfs_remove_recursive(nsim_ddir);
=======
	nsim_bus_exit();
	nsim_dev_exit();
>>>>>>> upstream/android-13
}

module_init(nsim_module_init);
module_exit(nsim_module_exit);
MODULE_LICENSE("GPL");
MODULE_ALIAS_RTNL_LINK(DRV_NAME);
