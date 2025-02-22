// SPDX-License-Identifier: GPL-2.0
#include <linux/kmod.h>
#include <linux/netdevice.h>
<<<<<<< HEAD
=======
#include <linux/inetdevice.h>
>>>>>>> upstream/android-13
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <linux/net_tstamp.h>
#include <linux/wireless.h>
<<<<<<< HEAD
=======
#include <linux/if_bridge.h>
#include <net/dsa.h>
>>>>>>> upstream/android-13
#include <net/wext.h>

/*
 *	Map an interface index to its name (SIOCGIFNAME)
 */

/*
 *	We need this ioctl for efficient implementation of the
 *	if_indextoname() function required by the IPv6 API.  Without
 *	it, we would have to search all the interfaces to find a
 *	match.  --pb
 */

static int dev_ifname(struct net *net, struct ifreq *ifr)
{
	ifr->ifr_name[IFNAMSIZ-1] = 0;
	return netdev_get_name(net, ifr->ifr_name, ifr->ifr_ifindex);
}

<<<<<<< HEAD
static gifconf_func_t *gifconf_list[NPROTO];

/**
 *	register_gifconf	-	register a SIOCGIF handler
 *	@family: Address family
 *	@gifconf: Function handler
 *
 *	Register protocol dependent address dumping routines. The handler
 *	that is passed must not be freed or reused until it has been replaced
 *	by another handler.
 */
int register_gifconf(unsigned int family, gifconf_func_t *gifconf)
{
	if (family >= NPROTO)
		return -EINVAL;
	gifconf_list[family] = gifconf;
	return 0;
}
EXPORT_SYMBOL(register_gifconf);

=======
>>>>>>> upstream/android-13
/*
 *	Perform a SIOCGIFCONF call. This structure will change
 *	size eventually, and there is nothing I can do about it.
 *	Thus we will need a 'compatibility mode'.
 */
<<<<<<< HEAD

int dev_ifconf(struct net *net, struct ifconf *ifc, int size)
{
	struct net_device *dev;
	char __user *pos;
	int len;
	int total;
	int i;

	/*
	 *	Fetch the caller's info block.
	 */

	pos = ifc->ifc_buf;
	len = ifc->ifc_len;

	/*
	 *	Loop over the interfaces, and write an info block for each.
	 */

	total = 0;
	for_each_netdev(net, dev) {
		for (i = 0; i < NPROTO; i++) {
			if (gifconf_list[i]) {
				int done;
				if (!pos)
					done = gifconf_list[i](dev, NULL, 0, size);
				else
					done = gifconf_list[i](dev, pos + total,
							       len - total, size);
				if (done < 0)
					return -EFAULT;
				total += done;
			}
		}
	}

	/*
	 *	All done.  Write the updated control block back to the caller.
	 */
	ifc->ifc_len = total;

	/*
	 * 	Both BSD and Solaris return 0 here, so we do too.
	 */
	return 0;
}

=======
int dev_ifconf(struct net *net, struct ifconf __user *uifc)
{
	struct net_device *dev;
	void __user *pos;
	size_t size;
	int len, total = 0, done;

	/* both the ifconf and the ifreq structures are slightly different */
	if (in_compat_syscall()) {
		struct compat_ifconf ifc32;

		if (copy_from_user(&ifc32, uifc, sizeof(struct compat_ifconf)))
			return -EFAULT;

		pos = compat_ptr(ifc32.ifcbuf);
		len = ifc32.ifc_len;
		size = sizeof(struct compat_ifreq);
	} else {
		struct ifconf ifc;

		if (copy_from_user(&ifc, uifc, sizeof(struct ifconf)))
			return -EFAULT;

		pos = ifc.ifc_buf;
		len = ifc.ifc_len;
		size = sizeof(struct ifreq);
	}

	/* Loop over the interfaces, and write an info block for each. */
	rtnl_lock();
	for_each_netdev(net, dev) {
		if (!pos)
			done = inet_gifconf(dev, NULL, 0, size);
		else
			done = inet_gifconf(dev, pos + total,
					    len - total, size);
		if (done < 0) {
			rtnl_unlock();
			return -EFAULT;
		}
		total += done;
	}
	rtnl_unlock();

	return put_user(total, &uifc->ifc_len);
}

static int dev_getifmap(struct net_device *dev, struct ifreq *ifr)
{
	struct ifmap *ifmap = &ifr->ifr_map;

	if (in_compat_syscall()) {
		struct compat_ifmap *cifmap = (struct compat_ifmap *)ifmap;

		cifmap->mem_start = dev->mem_start;
		cifmap->mem_end   = dev->mem_end;
		cifmap->base_addr = dev->base_addr;
		cifmap->irq       = dev->irq;
		cifmap->dma       = dev->dma;
		cifmap->port      = dev->if_port;

		return 0;
	}

	ifmap->mem_start  = dev->mem_start;
	ifmap->mem_end    = dev->mem_end;
	ifmap->base_addr  = dev->base_addr;
	ifmap->irq        = dev->irq;
	ifmap->dma        = dev->dma;
	ifmap->port       = dev->if_port;

	return 0;
}

static int dev_setifmap(struct net_device *dev, struct ifreq *ifr)
{
	struct compat_ifmap *cifmap = (struct compat_ifmap *)&ifr->ifr_map;

	if (!dev->netdev_ops->ndo_set_config)
		return -EOPNOTSUPP;

	if (in_compat_syscall()) {
		struct ifmap ifmap = {
			.mem_start  = cifmap->mem_start,
			.mem_end    = cifmap->mem_end,
			.base_addr  = cifmap->base_addr,
			.irq        = cifmap->irq,
			.dma        = cifmap->dma,
			.port       = cifmap->port,
		};

		return dev->netdev_ops->ndo_set_config(dev, &ifmap);
	}

	return dev->netdev_ops->ndo_set_config(dev, &ifr->ifr_map);
}

>>>>>>> upstream/android-13
/*
 *	Perform the SIOCxIFxxx calls, inside rcu_read_lock()
 */
static int dev_ifsioc_locked(struct net *net, struct ifreq *ifr, unsigned int cmd)
{
	int err;
	struct net_device *dev = dev_get_by_name_rcu(net, ifr->ifr_name);

	if (!dev)
		return -ENODEV;

	switch (cmd) {
	case SIOCGIFFLAGS:	/* Get interface flags */
		ifr->ifr_flags = (short) dev_get_flags(dev);
		return 0;

	case SIOCGIFMETRIC:	/* Get the metric on the interface
				   (currently unused) */
		ifr->ifr_metric = 0;
		return 0;

	case SIOCGIFMTU:	/* Get the MTU of a device */
		ifr->ifr_mtu = dev->mtu;
		return 0;

<<<<<<< HEAD
	case SIOCGIFHWADDR:
		if (!dev->addr_len)
			memset(ifr->ifr_hwaddr.sa_data, 0,
			       sizeof(ifr->ifr_hwaddr.sa_data));
		else
			memcpy(ifr->ifr_hwaddr.sa_data, dev->dev_addr,
			       min(sizeof(ifr->ifr_hwaddr.sa_data),
				   (size_t)dev->addr_len));
		ifr->ifr_hwaddr.sa_family = dev->type;
		return 0;

=======
>>>>>>> upstream/android-13
	case SIOCGIFSLAVE:
		err = -EINVAL;
		break;

	case SIOCGIFMAP:
<<<<<<< HEAD
		ifr->ifr_map.mem_start = dev->mem_start;
		ifr->ifr_map.mem_end   = dev->mem_end;
		ifr->ifr_map.base_addr = dev->base_addr;
		ifr->ifr_map.irq       = dev->irq;
		ifr->ifr_map.dma       = dev->dma;
		ifr->ifr_map.port      = dev->if_port;
		return 0;
=======
		return dev_getifmap(dev, ifr);
>>>>>>> upstream/android-13

	case SIOCGIFINDEX:
		ifr->ifr_ifindex = dev->ifindex;
		return 0;

	case SIOCGIFTXQLEN:
		ifr->ifr_qlen = dev->tx_queue_len;
		return 0;

	default:
		/* dev_ioctl() should ensure this case
		 * is never reached
		 */
		WARN_ON(1);
		err = -ENOTTY;
		break;

	}
	return err;
}

static int net_hwtstamp_validate(struct ifreq *ifr)
{
	struct hwtstamp_config cfg;
	enum hwtstamp_tx_types tx_type;
	enum hwtstamp_rx_filters rx_filter;
	int tx_type_valid = 0;
	int rx_filter_valid = 0;

	if (copy_from_user(&cfg, ifr->ifr_data, sizeof(cfg)))
		return -EFAULT;

	if (cfg.flags) /* reserved for future extensions */
		return -EINVAL;

	tx_type = cfg.tx_type;
	rx_filter = cfg.rx_filter;

	switch (tx_type) {
	case HWTSTAMP_TX_OFF:
	case HWTSTAMP_TX_ON:
	case HWTSTAMP_TX_ONESTEP_SYNC:
<<<<<<< HEAD
		tx_type_valid = 1;
		break;
=======
	case HWTSTAMP_TX_ONESTEP_P2P:
		tx_type_valid = 1;
		break;
	case __HWTSTAMP_TX_CNT:
		/* not a real value */
		break;
>>>>>>> upstream/android-13
	}

	switch (rx_filter) {
	case HWTSTAMP_FILTER_NONE:
	case HWTSTAMP_FILTER_ALL:
	case HWTSTAMP_FILTER_SOME:
	case HWTSTAMP_FILTER_PTP_V1_L4_EVENT:
	case HWTSTAMP_FILTER_PTP_V1_L4_SYNC:
	case HWTSTAMP_FILTER_PTP_V1_L4_DELAY_REQ:
	case HWTSTAMP_FILTER_PTP_V2_L4_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_L4_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_L4_DELAY_REQ:
	case HWTSTAMP_FILTER_PTP_V2_L2_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_L2_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_L2_DELAY_REQ:
	case HWTSTAMP_FILTER_PTP_V2_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_DELAY_REQ:
	case HWTSTAMP_FILTER_NTP_ALL:
		rx_filter_valid = 1;
		break;
<<<<<<< HEAD
=======
	case __HWTSTAMP_FILTER_CNT:
		/* not a real value */
		break;
>>>>>>> upstream/android-13
	}

	if (!tx_type_valid || !rx_filter_valid)
		return -ERANGE;

	return 0;
}

<<<<<<< HEAD
/*
 *	Perform the SIOCxIFxxx calls, inside rtnl_lock()
 */
static int dev_ifsioc(struct net *net, struct ifreq *ifr, unsigned int cmd)
=======
static int dev_eth_ioctl(struct net_device *dev,
			 struct ifreq *ifr, unsigned int cmd)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	int err;

	err = dsa_ndo_eth_ioctl(dev, ifr, cmd);
	if (err == 0 || err != -EOPNOTSUPP)
		return err;

	if (ops->ndo_eth_ioctl) {
		if (netif_device_present(dev))
			err = ops->ndo_eth_ioctl(dev, ifr, cmd);
		else
			err = -ENODEV;
	}

	return err;
}

static int dev_siocbond(struct net_device *dev,
			struct ifreq *ifr, unsigned int cmd)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	if (ops->ndo_siocbond) {
		if (netif_device_present(dev))
			return ops->ndo_siocbond(dev, ifr, cmd);
		else
			return -ENODEV;
	}

	return -EOPNOTSUPP;
}

static int dev_siocdevprivate(struct net_device *dev, struct ifreq *ifr,
			      void __user *data, unsigned int cmd)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	if (ops->ndo_siocdevprivate) {
		if (netif_device_present(dev))
			return ops->ndo_siocdevprivate(dev, ifr, data, cmd);
		else
			return -ENODEV;
	}

	return -EOPNOTSUPP;
}

static int dev_siocwandev(struct net_device *dev, struct if_settings *ifs)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	if (ops->ndo_siocwandev) {
		if (netif_device_present(dev))
			return ops->ndo_siocwandev(dev, ifs);
		else
			return -ENODEV;
	}

	return -EOPNOTSUPP;
}

/*
 *	Perform the SIOCxIFxxx calls, inside rtnl_lock()
 */
static int dev_ifsioc(struct net *net, struct ifreq *ifr, void __user *data,
		      unsigned int cmd)
>>>>>>> upstream/android-13
{
	int err;
	struct net_device *dev = __dev_get_by_name(net, ifr->ifr_name);
	const struct net_device_ops *ops;

	if (!dev)
		return -ENODEV;

	ops = dev->netdev_ops;

	switch (cmd) {
	case SIOCSIFFLAGS:	/* Set interface flags */
<<<<<<< HEAD
		return dev_change_flags(dev, ifr->ifr_flags);
=======
		return dev_change_flags(dev, ifr->ifr_flags, NULL);
>>>>>>> upstream/android-13

	case SIOCSIFMETRIC:	/* Set the metric on the interface
				   (currently unused) */
		return -EOPNOTSUPP;

	case SIOCSIFMTU:	/* Set the MTU of a device */
		return dev_set_mtu(dev, ifr->ifr_mtu);

	case SIOCSIFHWADDR:
		if (dev->addr_len > sizeof(struct sockaddr))
			return -EINVAL;
<<<<<<< HEAD
		return dev_set_mac_address(dev, &ifr->ifr_hwaddr);
=======
		return dev_set_mac_address_user(dev, &ifr->ifr_hwaddr, NULL);
>>>>>>> upstream/android-13

	case SIOCSIFHWBROADCAST:
		if (ifr->ifr_hwaddr.sa_family != dev->type)
			return -EINVAL;
		memcpy(dev->broadcast, ifr->ifr_hwaddr.sa_data,
		       min(sizeof(ifr->ifr_hwaddr.sa_data),
			   (size_t)dev->addr_len));
		call_netdevice_notifiers(NETDEV_CHANGEADDR, dev);
		return 0;

	case SIOCSIFMAP:
<<<<<<< HEAD
		if (ops->ndo_set_config) {
			if (!netif_device_present(dev))
				return -ENODEV;
			return ops->ndo_set_config(dev, &ifr->ifr_map);
		}
		return -EOPNOTSUPP;
=======
		return dev_setifmap(dev, ifr);
>>>>>>> upstream/android-13

	case SIOCADDMULTI:
		if (!ops->ndo_set_rx_mode ||
		    ifr->ifr_hwaddr.sa_family != AF_UNSPEC)
			return -EINVAL;
		if (!netif_device_present(dev))
			return -ENODEV;
		return dev_mc_add_global(dev, ifr->ifr_hwaddr.sa_data);

	case SIOCDELMULTI:
		if (!ops->ndo_set_rx_mode ||
		    ifr->ifr_hwaddr.sa_family != AF_UNSPEC)
			return -EINVAL;
		if (!netif_device_present(dev))
			return -ENODEV;
		return dev_mc_del_global(dev, ifr->ifr_hwaddr.sa_data);

	case SIOCSIFTXQLEN:
		if (ifr->ifr_qlen < 0)
			return -EINVAL;
		return dev_change_tx_queue_len(dev, ifr->ifr_qlen);

	case SIOCSIFNAME:
		ifr->ifr_newname[IFNAMSIZ-1] = '\0';
		return dev_change_name(dev, ifr->ifr_newname);

<<<<<<< HEAD
=======
	case SIOCWANDEV:
		return dev_siocwandev(dev, &ifr->ifr_settings);

	case SIOCBRADDIF:
	case SIOCBRDELIF:
		if (!netif_device_present(dev))
			return -ENODEV;
		if (!netif_is_bridge_master(dev))
			return -EOPNOTSUPP;
		dev_hold(dev);
		rtnl_unlock();
		err = br_ioctl_call(net, netdev_priv(dev), cmd, ifr, NULL);
		dev_put(dev);
		rtnl_lock();
		return err;

>>>>>>> upstream/android-13
	case SIOCSHWTSTAMP:
		err = net_hwtstamp_validate(ifr);
		if (err)
			return err;
<<<<<<< HEAD
		/* fall through */
=======
		fallthrough;
>>>>>>> upstream/android-13

	/*
	 *	Unknown or private ioctl
	 */
	default:
<<<<<<< HEAD
		if ((cmd >= SIOCDEVPRIVATE &&
		    cmd <= SIOCDEVPRIVATE + 15) ||
		    cmd == SIOCBONDENSLAVE ||
=======
		if (cmd >= SIOCDEVPRIVATE &&
		    cmd <= SIOCDEVPRIVATE + 15)
			return dev_siocdevprivate(dev, ifr, data, cmd);

		if (cmd == SIOCGMIIPHY ||
		    cmd == SIOCGMIIREG ||
		    cmd == SIOCSMIIREG ||
		    cmd == SIOCSHWTSTAMP ||
		    cmd == SIOCGHWTSTAMP) {
			err = dev_eth_ioctl(dev, ifr, cmd);
		} else if (cmd == SIOCBONDENSLAVE ||
>>>>>>> upstream/android-13
		    cmd == SIOCBONDRELEASE ||
		    cmd == SIOCBONDSETHWADDR ||
		    cmd == SIOCBONDSLAVEINFOQUERY ||
		    cmd == SIOCBONDINFOQUERY ||
<<<<<<< HEAD
		    cmd == SIOCBONDCHANGEACTIVE ||
		    cmd == SIOCGMIIPHY ||
		    cmd == SIOCGMIIREG ||
		    cmd == SIOCSMIIREG ||
		    cmd == SIOCBRADDIF ||
		    cmd == SIOCBRDELIF ||
		    cmd == SIOCSHWTSTAMP ||
		    cmd == SIOCGHWTSTAMP ||
		    cmd == SIOCWANDEV) {
			err = -EOPNOTSUPP;
			if (ops->ndo_do_ioctl) {
				if (netif_device_present(dev))
					err = ops->ndo_do_ioctl(dev, ifr, cmd);
				else
					err = -ENODEV;
			}
=======
		    cmd == SIOCBONDCHANGEACTIVE) {
			err = dev_siocbond(dev, ifr, cmd);
>>>>>>> upstream/android-13
		} else
			err = -EINVAL;

	}
	return err;
}

/**
 *	dev_load 	- load a network module
 *	@net: the applicable net namespace
 *	@name: name of interface
 *
 *	If a network interface is not present and the process has suitable
 *	privileges this function loads the module. If module loading is not
 *	available in this kernel then it becomes a nop.
 */

void dev_load(struct net *net, const char *name)
{
	struct net_device *dev;
	int no_module;

	rcu_read_lock();
	dev = dev_get_by_name_rcu(net, name);
	rcu_read_unlock();

	no_module = !dev;
	if (no_module && capable(CAP_NET_ADMIN))
		no_module = request_module("netdev-%s", name);
	if (no_module && capable(CAP_SYS_MODULE))
		request_module("%s", name);
}
EXPORT_SYMBOL(dev_load);

/*
 *	This function handles all "interface"-type I/O control requests. The actual
 *	'doing' part of this is dev_ifsioc above.
 */

/**
 *	dev_ioctl	-	network device ioctl
 *	@net: the applicable net namespace
 *	@cmd: command to issue
<<<<<<< HEAD
 *	@arg: pointer to a struct ifreq in user space
=======
 *	@ifr: pointer to a struct ifreq in user space
 *	@need_copyout: whether or not copy_to_user() should be called
>>>>>>> upstream/android-13
 *
 *	Issue ioctl functions to devices. This is normally called by the
 *	user space syscall interfaces but can sometimes be useful for
 *	other purposes. The return value is the return from the syscall if
 *	positive or a negative errno code on error.
 */

<<<<<<< HEAD
int dev_ioctl(struct net *net, unsigned int cmd, struct ifreq *ifr, bool *need_copyout)
=======
int dev_ioctl(struct net *net, unsigned int cmd, struct ifreq *ifr,
	      void __user *data, bool *need_copyout)
>>>>>>> upstream/android-13
{
	int ret;
	char *colon;

	if (need_copyout)
		*need_copyout = true;
	if (cmd == SIOCGIFNAME)
		return dev_ifname(net, ifr);

	ifr->ifr_name[IFNAMSIZ-1] = 0;

	colon = strchr(ifr->ifr_name, ':');
	if (colon)
		*colon = 0;

	/*
	 *	See which interface the caller is talking about.
	 */

	switch (cmd) {
<<<<<<< HEAD
=======
	case SIOCGIFHWADDR:
		dev_load(net, ifr->ifr_name);
		ret = dev_get_mac_address(&ifr->ifr_hwaddr, net, ifr->ifr_name);
		if (colon)
			*colon = ':';
		return ret;
>>>>>>> upstream/android-13
	/*
	 *	These ioctl calls:
	 *	- can be done by all.
	 *	- atomic and do not require locking.
	 *	- return a value
	 */
	case SIOCGIFFLAGS:
	case SIOCGIFMETRIC:
	case SIOCGIFMTU:
<<<<<<< HEAD
	case SIOCGIFHWADDR:
=======
>>>>>>> upstream/android-13
	case SIOCGIFSLAVE:
	case SIOCGIFMAP:
	case SIOCGIFINDEX:
	case SIOCGIFTXQLEN:
		dev_load(net, ifr->ifr_name);
		rcu_read_lock();
		ret = dev_ifsioc_locked(net, ifr, cmd);
		rcu_read_unlock();
		if (colon)
			*colon = ':';
		return ret;

	case SIOCETHTOOL:
		dev_load(net, ifr->ifr_name);
		rtnl_lock();
<<<<<<< HEAD
		ret = dev_ethtool(net, ifr);
=======
		ret = dev_ethtool(net, ifr, data);
>>>>>>> upstream/android-13
		rtnl_unlock();
		if (colon)
			*colon = ':';
		return ret;

	/*
	 *	These ioctl calls:
	 *	- require superuser power.
	 *	- require strict serialization.
	 *	- return a value
	 */
	case SIOCGMIIPHY:
	case SIOCGMIIREG:
	case SIOCSIFNAME:
		dev_load(net, ifr->ifr_name);
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			return -EPERM;
		rtnl_lock();
<<<<<<< HEAD
		ret = dev_ifsioc(net, ifr, cmd);
=======
		ret = dev_ifsioc(net, ifr, data, cmd);
>>>>>>> upstream/android-13
		rtnl_unlock();
		if (colon)
			*colon = ':';
		return ret;

	/*
	 *	These ioctl calls:
	 *	- require superuser power.
	 *	- require strict serialization.
	 *	- do not return a value
	 */
	case SIOCSIFMAP:
	case SIOCSIFTXQLEN:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
<<<<<<< HEAD
		/* fall through */
=======
		fallthrough;
>>>>>>> upstream/android-13
	/*
	 *	These ioctl calls:
	 *	- require local superuser power.
	 *	- require strict serialization.
	 *	- do not return a value
	 */
	case SIOCSIFFLAGS:
	case SIOCSIFMETRIC:
	case SIOCSIFMTU:
	case SIOCSIFHWADDR:
	case SIOCSIFSLAVE:
	case SIOCADDMULTI:
	case SIOCDELMULTI:
	case SIOCSIFHWBROADCAST:
	case SIOCSMIIREG:
	case SIOCBONDENSLAVE:
	case SIOCBONDRELEASE:
	case SIOCBONDSETHWADDR:
	case SIOCBONDCHANGEACTIVE:
	case SIOCBRADDIF:
	case SIOCBRDELIF:
	case SIOCSHWTSTAMP:
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			return -EPERM;
<<<<<<< HEAD
		/* fall through */
=======
		fallthrough;
>>>>>>> upstream/android-13
	case SIOCBONDSLAVEINFOQUERY:
	case SIOCBONDINFOQUERY:
		dev_load(net, ifr->ifr_name);
		rtnl_lock();
<<<<<<< HEAD
		ret = dev_ifsioc(net, ifr, cmd);
=======
		ret = dev_ifsioc(net, ifr, data, cmd);
>>>>>>> upstream/android-13
		rtnl_unlock();
		if (need_copyout)
			*need_copyout = false;
		return ret;

	case SIOCGIFMEM:
		/* Get the per device memory space. We can add this but
		 * currently do not support it */
	case SIOCSIFMEM:
		/* Set the per device memory buffer space.
		 * Not applicable in our case */
	case SIOCSIFLINK:
		return -ENOTTY;

	/*
	 *	Unknown or private ioctl.
	 */
	default:
		if (cmd == SIOCWANDEV ||
		    cmd == SIOCGHWTSTAMP ||
		    (cmd >= SIOCDEVPRIVATE &&
		     cmd <= SIOCDEVPRIVATE + 15)) {
			dev_load(net, ifr->ifr_name);
			rtnl_lock();
<<<<<<< HEAD
			ret = dev_ifsioc(net, ifr, cmd);
=======
			ret = dev_ifsioc(net, ifr, data, cmd);
>>>>>>> upstream/android-13
			rtnl_unlock();
			return ret;
		}
		return -ENOTTY;
	}
}
