<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Generic HDLC support routines for Linux
 * HDLC support
 *
 * Copyright (C) 1999 - 2006 Krzysztof Halasa <khc@pm.waw.pl>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/errno.h>
#include <linux/hdlc.h>
#include <linux/if_arp.h>
#include <linux/inetdevice.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pkt_sched.h>
#include <linux/poll.h>
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>


<<<<<<< HEAD
static int raw_ioctl(struct net_device *dev, struct ifreq *ifr);
=======
static int raw_ioctl(struct net_device *dev, struct if_settings *ifs);
>>>>>>> upstream/android-13

static __be16 raw_type_trans(struct sk_buff *skb, struct net_device *dev)
{
	return cpu_to_be16(ETH_P_IP);
}

static struct hdlc_proto proto = {
	.type_trans	= raw_type_trans,
	.ioctl		= raw_ioctl,
	.module		= THIS_MODULE,
};


<<<<<<< HEAD
static int raw_ioctl(struct net_device *dev, struct ifreq *ifr)
{
	raw_hdlc_proto __user *raw_s = ifr->ifr_settings.ifs_ifsu.raw_hdlc;
=======
static int raw_ioctl(struct net_device *dev, struct if_settings *ifs)
{
	raw_hdlc_proto __user *raw_s = ifs->ifs_ifsu.raw_hdlc;
>>>>>>> upstream/android-13
	const size_t size = sizeof(raw_hdlc_proto);
	raw_hdlc_proto new_settings;
	hdlc_device *hdlc = dev_to_hdlc(dev);
	int result;

<<<<<<< HEAD
	switch (ifr->ifr_settings.type) {
	case IF_GET_PROTO:
		if (dev_to_hdlc(dev)->proto != &proto)
			return -EINVAL;
		ifr->ifr_settings.type = IF_PROTO_HDLC;
		if (ifr->ifr_settings.size < size) {
			ifr->ifr_settings.size = size; /* data size wanted */
=======
	switch (ifs->type) {
	case IF_GET_PROTO:
		if (dev_to_hdlc(dev)->proto != &proto)
			return -EINVAL;
		ifs->type = IF_PROTO_HDLC;
		if (ifs->size < size) {
			ifs->size = size; /* data size wanted */
>>>>>>> upstream/android-13
			return -ENOBUFS;
		}
		if (copy_to_user(raw_s, hdlc->state, size))
			return -EFAULT;
		return 0;

	case IF_PROTO_HDLC:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if (dev->flags & IFF_UP)
			return -EBUSY;

		if (copy_from_user(&new_settings, raw_s, size))
			return -EFAULT;

		if (new_settings.encoding == ENCODING_DEFAULT)
			new_settings.encoding = ENCODING_NRZ;

		if (new_settings.parity == PARITY_DEFAULT)
			new_settings.parity = PARITY_CRC16_PR1_CCITT;

		result = hdlc->attach(dev, new_settings.encoding,
				      new_settings.parity);
		if (result)
			return result;

		result = attach_hdlc_protocol(dev, &proto,
					      sizeof(raw_hdlc_proto));
		if (result)
			return result;
		memcpy(hdlc->state, &new_settings, size);
		dev->type = ARPHRD_RAWHDLC;
		call_netdevice_notifiers(NETDEV_POST_TYPE_CHANGE, dev);
		netif_dormant_off(dev);
		return 0;
	}

	return -EINVAL;
}


<<<<<<< HEAD
static int __init mod_init(void)
=======
static int __init hdlc_raw_init(void)
>>>>>>> upstream/android-13
{
	register_hdlc_protocol(&proto);
	return 0;
}



<<<<<<< HEAD
static void __exit mod_exit(void)
=======
static void __exit hdlc_raw_exit(void)
>>>>>>> upstream/android-13
{
	unregister_hdlc_protocol(&proto);
}


<<<<<<< HEAD
module_init(mod_init);
module_exit(mod_exit);
=======
module_init(hdlc_raw_init);
module_exit(hdlc_raw_exit);
>>>>>>> upstream/android-13

MODULE_AUTHOR("Krzysztof Halasa <khc@pm.waw.pl>");
MODULE_DESCRIPTION("Raw HDLC protocol support for generic HDLC");
MODULE_LICENSE("GPL v2");
