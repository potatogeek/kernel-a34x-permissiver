// SPDX-License-Identifier: GPL-2.0
/*
 *    Copyright IBM Corp. 2007, 2009
 *    Author(s): Utz Bacher <utz.bacher@de.ibm.com>,
 *		 Frank Pavlic <fpavlic@de.ibm.com>,
 *		 Thomas Spatzier <tspat@de.ibm.com>,
 *		 Frank Blaschka <frank.blaschka@de.ibm.com>
 */

#define KMSG_COMPONENT "qeth"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/etherdevice.h>
<<<<<<< HEAD
#include <linux/list.h>
#include <linux/hash.h>
#include <linux/hashtable.h>
=======
#include <linux/if_bridge.h>
#include <linux/list.h>
#include <linux/hash.h>
#include <linux/hashtable.h>
#include <net/switchdev.h>
#include <asm/chsc.h>
#include <asm/css_chars.h>
>>>>>>> upstream/android-13
#include <asm/setup.h>
#include "qeth_core.h"
#include "qeth_l2.h"

<<<<<<< HEAD
static int qeth_l2_set_offline(struct ccwgroup_device *);
static int qeth_l2_stop(struct net_device *);
static void qeth_bridgeport_query_support(struct qeth_card *card);
static void qeth_bridge_state_change(struct qeth_card *card,
					struct qeth_ipa_cmd *cmd);
static void qeth_bridge_host_event(struct qeth_card *card,
					struct qeth_ipa_cmd *cmd);
static void qeth_l2_vnicc_set_defaults(struct qeth_card *card);
static void qeth_l2_vnicc_init(struct qeth_card *card);
static bool qeth_l2_vnicc_recover_timeout(struct qeth_card *card, u32 vnicc,
					  u32 *timeout);

static struct net_device *qeth_l2_netdev_by_devno(unsigned char *read_dev_no)
{
	struct qeth_card *card;
	struct net_device *ndev;
	__u16 temp_dev_no;
	unsigned long flags;
	struct ccw_dev_id read_devid;

	ndev = NULL;
	memcpy(&temp_dev_no, read_dev_no, 2);
	read_lock_irqsave(&qeth_core_card_list.rwlock, flags);
	list_for_each_entry(card, &qeth_core_card_list.list, list) {
		ccw_device_get_id(CARD_RDEV(card), &read_devid);
		if (read_devid.devno == temp_dev_no) {
			ndev = card->dev;
			break;
		}
	}
	read_unlock_irqrestore(&qeth_core_card_list.rwlock, flags);
	return ndev;
}

static int qeth_setdelmac_makerc(struct qeth_card *card, int retcode)
=======
static int qeth_l2_setdelmac_makerc(struct qeth_card *card, u16 retcode)
>>>>>>> upstream/android-13
{
	int rc;

	if (retcode)
		QETH_CARD_TEXT_(card, 2, "err%04x", retcode);
	switch (retcode) {
	case IPA_RC_SUCCESS:
		rc = 0;
		break;
	case IPA_RC_L2_UNSUPPORTED_CMD:
		rc = -EOPNOTSUPP;
		break;
	case IPA_RC_L2_ADDR_TABLE_FULL:
		rc = -ENOSPC;
		break;
	case IPA_RC_L2_DUP_MAC:
	case IPA_RC_L2_DUP_LAYER3_MAC:
<<<<<<< HEAD
		rc = -EEXIST;
		break;
	case IPA_RC_L2_MAC_NOT_AUTH_BY_HYP:
	case IPA_RC_L2_MAC_NOT_AUTH_BY_ADP:
		rc = -EPERM;
=======
		rc = -EADDRINUSE;
		break;
	case IPA_RC_L2_MAC_NOT_AUTH_BY_HYP:
	case IPA_RC_L2_MAC_NOT_AUTH_BY_ADP:
		rc = -EADDRNOTAVAIL;
>>>>>>> upstream/android-13
		break;
	case IPA_RC_L2_MAC_NOT_FOUND:
		rc = -ENOENT;
		break;
<<<<<<< HEAD
	case -ENOMEM:
		rc = -ENOMEM;
		break;
=======
>>>>>>> upstream/android-13
	default:
		rc = -EIO;
		break;
	}
	return rc;
}

<<<<<<< HEAD
=======
static int qeth_l2_send_setdelmac_cb(struct qeth_card *card,
				     struct qeth_reply *reply,
				     unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;

	return qeth_l2_setdelmac_makerc(card, cmd->hdr.return_code);
}

>>>>>>> upstream/android-13
static int qeth_l2_send_setdelmac(struct qeth_card *card, __u8 *mac,
			   enum qeth_ipa_cmds ipacmd)
{
	struct qeth_ipa_cmd *cmd;
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "L2sdmac");
<<<<<<< HEAD
	iob = qeth_get_ipacmd_buffer(card, ipacmd, QETH_PROT_IPV4);
=======
	iob = qeth_ipa_alloc_cmd(card, ipacmd, QETH_PROT_IPV4,
				 IPA_DATA_SIZEOF(setdelmac));
>>>>>>> upstream/android-13
	if (!iob)
		return -ENOMEM;
	cmd = __ipa_cmd(iob);
	cmd->data.setdelmac.mac_length = ETH_ALEN;
	ether_addr_copy(cmd->data.setdelmac.mac, mac);
<<<<<<< HEAD
	return qeth_setdelmac_makerc(card, qeth_send_ipa_cmd(card, iob,
					   NULL, NULL));
=======
	return qeth_send_ipa_cmd(card, iob, qeth_l2_send_setdelmac_cb, NULL);
>>>>>>> upstream/android-13
}

static int qeth_l2_send_setmac(struct qeth_card *card, __u8 *mac)
{
	int rc;

	QETH_CARD_TEXT(card, 2, "L2Setmac");
	rc = qeth_l2_send_setdelmac(card, mac, IPA_CMD_SETVMAC);
	if (rc == 0) {
		dev_info(&card->gdev->dev,
<<<<<<< HEAD
			 "MAC address %pM successfully registered on device %s\n",
			 mac, card->dev->name);
	} else {
		switch (rc) {
		case -EEXIST:
			dev_warn(&card->gdev->dev,
				"MAC address %pM already exists\n", mac);
			break;
		case -EPERM:
=======
			 "MAC address %pM successfully registered\n", mac);
	} else {
		switch (rc) {
		case -EADDRINUSE:
			dev_warn(&card->gdev->dev,
				"MAC address %pM already exists\n", mac);
			break;
		case -EADDRNOTAVAIL:
>>>>>>> upstream/android-13
			dev_warn(&card->gdev->dev,
				"MAC address %pM is not authorized\n", mac);
			break;
		}
	}
	return rc;
}

static int qeth_l2_write_mac(struct qeth_card *card, u8 *mac)
{
	enum qeth_ipa_cmds cmd = is_multicast_ether_addr(mac) ?
					IPA_CMD_SETGMAC : IPA_CMD_SETVMAC;
	int rc;

	QETH_CARD_TEXT(card, 2, "L2Wmac");
	rc = qeth_l2_send_setdelmac(card, mac, cmd);
<<<<<<< HEAD
	if (rc == -EEXIST)
		QETH_DBF_MESSAGE(2, "MAC %pM already registered on %s\n",
				 mac, QETH_CARD_IFNAME(card));
	else if (rc)
		QETH_DBF_MESSAGE(2, "Failed to register MAC %pM on %s: %d\n",
				 mac, QETH_CARD_IFNAME(card), rc);
=======
	if (rc == -EADDRINUSE)
		QETH_DBF_MESSAGE(2, "MAC already registered on device %x\n",
				 CARD_DEVID(card));
	else if (rc)
		QETH_DBF_MESSAGE(2, "Failed to register MAC on device %x: %d\n",
				 CARD_DEVID(card), rc);
>>>>>>> upstream/android-13
	return rc;
}

static int qeth_l2_remove_mac(struct qeth_card *card, u8 *mac)
{
	enum qeth_ipa_cmds cmd = is_multicast_ether_addr(mac) ?
					IPA_CMD_DELGMAC : IPA_CMD_DELVMAC;
	int rc;

	QETH_CARD_TEXT(card, 2, "L2Rmac");
	rc = qeth_l2_send_setdelmac(card, mac, cmd);
	if (rc)
<<<<<<< HEAD
		QETH_DBF_MESSAGE(2, "Failed to delete MAC %pM on %s: %d\n",
				 mac, QETH_CARD_IFNAME(card), rc);
	return rc;
}

static void qeth_l2_del_all_macs(struct qeth_card *card)
=======
		QETH_DBF_MESSAGE(2, "Failed to delete MAC on device %u: %d\n",
				 CARD_DEVID(card), rc);
	return rc;
}

static void qeth_l2_drain_rx_mode_cache(struct qeth_card *card)
>>>>>>> upstream/android-13
{
	struct qeth_mac *mac;
	struct hlist_node *tmp;
	int i;

<<<<<<< HEAD
	spin_lock_bh(&card->mclock);
	hash_for_each_safe(card->mac_htable, i, tmp, mac, hnode) {
		hash_del(&mac->hnode);
		kfree(mac);
	}
	spin_unlock_bh(&card->mclock);
}

static int qeth_l2_get_cast_type(struct qeth_card *card, struct sk_buff *skb)
{
	if (card->info.type == QETH_CARD_TYPE_OSN)
		return RTN_UNICAST;
	if (is_broadcast_ether_addr(skb->data))
		return RTN_BROADCAST;
	if (is_multicast_ether_addr(skb->data))
		return RTN_MULTICAST;
	return RTN_UNICAST;
}

static void qeth_l2_fill_header(struct qeth_hdr *hdr, struct sk_buff *skb,
				int cast_type, unsigned int data_len)
{
	struct vlan_ethhdr *veth = (struct vlan_ethhdr *)skb_mac_header(skb);

	memset(hdr, 0, sizeof(struct qeth_hdr));
	hdr->hdr.l2.id = QETH_HEADER_TYPE_LAYER2;
	hdr->hdr.l2.pkt_length = data_len;

=======
	hash_for_each_safe(card->rx_mode_addrs, i, tmp, mac, hnode) {
		hash_del(&mac->hnode);
		kfree(mac);
	}
}

static void qeth_l2_fill_header(struct qeth_qdio_out_q *queue,
				struct qeth_hdr *hdr, struct sk_buff *skb,
				__be16 proto, unsigned int data_len)
{
	int cast_type = qeth_get_ether_cast_type(skb);
	struct vlan_ethhdr *veth = vlan_eth_hdr(skb);

	hdr->hdr.l2.pkt_length = data_len;

	if (skb_is_gso(skb)) {
		hdr->hdr.l2.id = QETH_HEADER_TYPE_L2_TSO;
	} else {
		hdr->hdr.l2.id = QETH_HEADER_TYPE_LAYER2;
		if (skb->ip_summed == CHECKSUM_PARTIAL)
			qeth_tx_csum(skb, &hdr->hdr.l2.flags[1], proto);
	}

>>>>>>> upstream/android-13
	/* set byte byte 3 to casting flags */
	if (cast_type == RTN_MULTICAST)
		hdr->hdr.l2.flags[2] |= QETH_LAYER2_FLAG_MULTICAST;
	else if (cast_type == RTN_BROADCAST)
		hdr->hdr.l2.flags[2] |= QETH_LAYER2_FLAG_BROADCAST;
	else
		hdr->hdr.l2.flags[2] |= QETH_LAYER2_FLAG_UNICAST;

	/* VSWITCH relies on the VLAN
	 * information to be present in
	 * the QDIO header */
<<<<<<< HEAD
	if (veth->h_vlan_proto == __constant_htons(ETH_P_8021Q)) {
=======
	if (veth->h_vlan_proto == htons(ETH_P_8021Q)) {
>>>>>>> upstream/android-13
		hdr->hdr.l2.flags[2] |= QETH_LAYER2_FLAG_VLAN;
		hdr->hdr.l2.vlan_id = ntohs(veth->h_vlan_TCI);
	}
}

<<<<<<< HEAD
static int qeth_setdelvlan_makerc(struct qeth_card *card, int retcode)
=======
static int qeth_l2_setdelvlan_makerc(struct qeth_card *card, u16 retcode)
>>>>>>> upstream/android-13
{
	if (retcode)
		QETH_CARD_TEXT_(card, 2, "err%04x", retcode);

	switch (retcode) {
	case IPA_RC_SUCCESS:
		return 0;
	case IPA_RC_L2_INVALID_VLAN_ID:
		return -EINVAL;
	case IPA_RC_L2_DUP_VLAN_ID:
		return -EEXIST;
	case IPA_RC_L2_VLAN_ID_NOT_FOUND:
		return -ENOENT;
	case IPA_RC_L2_VLAN_ID_NOT_ALLOWED:
		return -EPERM;
<<<<<<< HEAD
	case -ENOMEM:
		return -ENOMEM;
=======
>>>>>>> upstream/android-13
	default:
		return -EIO;
	}
}

static int qeth_l2_send_setdelvlan_cb(struct qeth_card *card,
				      struct qeth_reply *reply,
				      unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;

	QETH_CARD_TEXT(card, 2, "L2sdvcb");
	if (cmd->hdr.return_code) {
<<<<<<< HEAD
		QETH_DBF_MESSAGE(2, "Error in processing VLAN %i on %s: 0x%x.\n",
				 cmd->data.setdelvlan.vlan_id,
				 QETH_CARD_IFNAME(card), cmd->hdr.return_code);
		QETH_CARD_TEXT_(card, 2, "L2VL%4x", cmd->hdr.command);
		QETH_CARD_TEXT_(card, 2, "err%d", cmd->hdr.return_code);
	}
	return 0;
=======
		QETH_DBF_MESSAGE(2, "Error in processing VLAN %u on device %x: %#x.\n",
				 cmd->data.setdelvlan.vlan_id,
				 CARD_DEVID(card), cmd->hdr.return_code);
		QETH_CARD_TEXT_(card, 2, "L2VL%4x", cmd->hdr.command);
	}
	return qeth_l2_setdelvlan_makerc(card, cmd->hdr.return_code);
>>>>>>> upstream/android-13
}

static int qeth_l2_send_setdelvlan(struct qeth_card *card, __u16 i,
				   enum qeth_ipa_cmds ipacmd)
{
	struct qeth_ipa_cmd *cmd;
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT_(card, 4, "L2sdv%x", ipacmd);
<<<<<<< HEAD
	iob = qeth_get_ipacmd_buffer(card, ipacmd, QETH_PROT_IPV4);
=======
	iob = qeth_ipa_alloc_cmd(card, ipacmd, QETH_PROT_IPV4,
				 IPA_DATA_SIZEOF(setdelvlan));
>>>>>>> upstream/android-13
	if (!iob)
		return -ENOMEM;
	cmd = __ipa_cmd(iob);
	cmd->data.setdelvlan.vlan_id = i;
<<<<<<< HEAD
	return qeth_setdelvlan_makerc(card, qeth_send_ipa_cmd(card, iob,
					    qeth_l2_send_setdelvlan_cb, NULL));
}

static void qeth_l2_process_vlans(struct qeth_card *card)
{
	struct qeth_vlan_vid *id;

	QETH_CARD_TEXT(card, 3, "L2prcvln");
	mutex_lock(&card->vid_list_mutex);
	list_for_each_entry(id, &card->vid_list, list) {
		qeth_l2_send_setdelvlan(card, id->vid, IPA_CMD_SETVLAN);
	}
	mutex_unlock(&card->vid_list_mutex);
=======
	return qeth_send_ipa_cmd(card, iob, qeth_l2_send_setdelvlan_cb, NULL);
>>>>>>> upstream/android-13
}

static int qeth_l2_vlan_rx_add_vid(struct net_device *dev,
				   __be16 proto, u16 vid)
{
	struct qeth_card *card = dev->ml_priv;
<<<<<<< HEAD
	struct qeth_vlan_vid *id;
	int rc;
=======
>>>>>>> upstream/android-13

	QETH_CARD_TEXT_(card, 4, "aid:%d", vid);
	if (!vid)
		return 0;
<<<<<<< HEAD
	if (qeth_wait_for_threads(card, QETH_RECOVER_THREAD)) {
		QETH_CARD_TEXT(card, 3, "aidREC");
		return 0;
	}
	id = kmalloc(sizeof(*id), GFP_KERNEL);
	if (id) {
		id->vid = vid;
		rc = qeth_l2_send_setdelvlan(card, vid, IPA_CMD_SETVLAN);
		if (rc) {
			kfree(id);
			return rc;
		}
		mutex_lock(&card->vid_list_mutex);
		list_add_tail(&id->list, &card->vid_list);
		mutex_unlock(&card->vid_list_mutex);
	} else {
		return -ENOMEM;
	}
	return 0;
=======

	return qeth_l2_send_setdelvlan(card, vid, IPA_CMD_SETVLAN);
>>>>>>> upstream/android-13
}

static int qeth_l2_vlan_rx_kill_vid(struct net_device *dev,
				    __be16 proto, u16 vid)
{
<<<<<<< HEAD
	struct qeth_vlan_vid *id, *tmpid = NULL;
	struct qeth_card *card = dev->ml_priv;
	int rc = 0;

	QETH_CARD_TEXT_(card, 4, "kid:%d", vid);
	if (qeth_wait_for_threads(card, QETH_RECOVER_THREAD)) {
		QETH_CARD_TEXT(card, 3, "kidREC");
		return 0;
	}
	mutex_lock(&card->vid_list_mutex);
	list_for_each_entry(id, &card->vid_list, list) {
		if (id->vid == vid) {
			list_del(&id->list);
			tmpid = id;
			break;
		}
	}
	mutex_unlock(&card->vid_list_mutex);
	if (tmpid) {
		rc = qeth_l2_send_setdelvlan(card, vid, IPA_CMD_DELVLAN);
		kfree(tmpid);
	}
	return rc;
}

static void qeth_l2_stop_card(struct qeth_card *card, int recovery_mode)
{
	QETH_DBF_TEXT(SETUP , 2, "stopcard");
	QETH_DBF_HEX(SETUP, 2, &card, sizeof(void *));

	qeth_set_allowed_threads(card, 0, 1);
	if (card->read.state == CH_STATE_UP &&
	    card->write.state == CH_STATE_UP &&
	    (card->state == CARD_STATE_UP)) {
		if (recovery_mode &&
		    card->info.type != QETH_CARD_TYPE_OSN) {
			qeth_l2_stop(card->dev);
		} else {
			rtnl_lock();
			dev_close(card->dev);
			rtnl_unlock();
		}
		card->info.mac_bits &= ~QETH_LAYER2_MAC_REGISTERED;
		card->state = CARD_STATE_SOFTSETUP;
	}
	if (card->state == CARD_STATE_SOFTSETUP) {
		qeth_l2_del_all_macs(card);
		qeth_clear_ipacmd_list(card);
		card->state = CARD_STATE_HARDSETUP;
	}
	if (card->state == CARD_STATE_HARDSETUP) {
		qeth_qdio_clear_card(card, 0);
		qeth_clear_qdio_buffers(card);
		qeth_clear_working_pool_list(card);
		card->state = CARD_STATE_DOWN;
	}
	if (card->state == CARD_STATE_DOWN) {
		qeth_clear_cmd_buffers(&card->read);
		qeth_clear_cmd_buffers(&card->write);
	}
}

static int qeth_l2_process_inbound_buffer(struct qeth_card *card,
				int budget, int *done)
{
	int work_done = 0;
	struct sk_buff *skb;
	struct qeth_hdr *hdr;
	unsigned int len;

	*done = 0;
	WARN_ON_ONCE(!budget);
	while (budget) {
		skb = qeth_core_get_next_skb(card,
			&card->qdio.in_q->bufs[card->rx.b_index],
			&card->rx.b_element, &card->rx.e_offset, &hdr);
		if (!skb) {
			*done = 1;
			break;
		}
		switch (hdr->hdr.l2.id) {
		case QETH_HEADER_TYPE_LAYER2:
			skb->protocol = eth_type_trans(skb, skb->dev);
			qeth_rx_csum(card, skb, hdr->hdr.l2.flags[1]);
			if (skb->protocol == htons(ETH_P_802_2))
				*((__u32 *)skb->cb) = ++card->seqno.pkt_seqno;
			len = skb->len;
			napi_gro_receive(&card->napi, skb);
			break;
		case QETH_HEADER_TYPE_OSN:
			if (card->info.type == QETH_CARD_TYPE_OSN) {
				skb_push(skb, sizeof(struct qeth_hdr));
				skb_copy_to_linear_data(skb, hdr,
						sizeof(struct qeth_hdr));
				len = skb->len;
				card->osn_info.data_cb(skb);
				break;
			}
			/* else unknown */
		default:
			dev_kfree_skb_any(skb);
			QETH_CARD_TEXT(card, 3, "inbunkno");
			QETH_DBF_HEX(CTRL, 3, hdr, sizeof(*hdr));
			continue;
		}
		work_done++;
		budget--;
		card->stats.rx_packets++;
		card->stats.rx_bytes += len;
	}
	return work_done;
=======
	struct qeth_card *card = dev->ml_priv;

	QETH_CARD_TEXT_(card, 4, "kid:%d", vid);
	if (!vid)
		return 0;

	return qeth_l2_send_setdelvlan(card, vid, IPA_CMD_DELVLAN);
}

static void qeth_l2_set_pnso_mode(struct qeth_card *card,
				  enum qeth_pnso_mode mode)
{
	spin_lock_irq(get_ccwdev_lock(CARD_RDEV(card)));
	WRITE_ONCE(card->info.pnso_mode, mode);
	spin_unlock_irq(get_ccwdev_lock(CARD_RDEV(card)));

	if (mode == QETH_PNSO_NONE)
		drain_workqueue(card->event_wq);
}

static void qeth_l2_dev2br_fdb_flush(struct qeth_card *card)
{
	struct switchdev_notifier_fdb_info info = {};

	QETH_CARD_TEXT(card, 2, "fdbflush");

	info.addr = NULL;
	/* flush all VLANs: */
	info.vid = 0;
	info.added_by_user = false;
	info.offloaded = true;

	call_switchdev_notifiers(SWITCHDEV_FDB_FLUSH_TO_BRIDGE,
				 card->dev, &info.info, NULL);
>>>>>>> upstream/android-13
}

static int qeth_l2_request_initial_mac(struct qeth_card *card)
{
	int rc = 0;

<<<<<<< HEAD
	QETH_DBF_TEXT(SETUP, 2, "l2reqmac");
	QETH_DBF_TEXT_(SETUP, 2, "doL2%s", CARD_BUS_ID(card));
=======
	QETH_CARD_TEXT(card, 2, "l2reqmac");
>>>>>>> upstream/android-13

	if (MACHINE_IS_VM) {
		rc = qeth_vm_request_mac(card);
		if (!rc)
			goto out;
<<<<<<< HEAD
		QETH_DBF_MESSAGE(2, "z/VM MAC Service failed on device %s: x%x\n",
				 CARD_BUS_ID(card), rc);
		QETH_DBF_TEXT_(SETUP, 2, "err%04x", rc);
		/* fall back to alternative mechanism: */
	}

	if (card->info.type == QETH_CARD_TYPE_IQD ||
	    card->info.type == QETH_CARD_TYPE_OSM ||
	    card->info.type == QETH_CARD_TYPE_OSX ||
	    card->info.guestlan) {
		rc = qeth_setadpparms_change_macaddr(card);
		if (!rc)
			goto out;
		QETH_DBF_MESSAGE(2, "READ_MAC Assist failed on device %s: x%x\n",
				 CARD_BUS_ID(card), rc);
		QETH_DBF_TEXT_(SETUP, 2, "1err%04x", rc);
		/* fall back once more: */
	}

	/* some devices don't support a custom MAC address: */
	if (card->info.type == QETH_CARD_TYPE_OSM ||
	    card->info.type == QETH_CARD_TYPE_OSX)
=======
		QETH_DBF_MESSAGE(2, "z/VM MAC Service failed on device %x: %#x\n",
				 CARD_DEVID(card), rc);
		QETH_CARD_TEXT_(card, 2, "err%04x", rc);
		/* fall back to alternative mechanism: */
	}

	rc = qeth_setadpparms_change_macaddr(card);
	if (!rc)
		goto out;
	QETH_DBF_MESSAGE(2, "READ_MAC Assist failed on device %x: %#x\n",
			 CARD_DEVID(card), rc);
	QETH_CARD_TEXT_(card, 2, "1err%04x", rc);

	/* Fall back once more, but some devices don't support a custom MAC
	 * address:
	 */
	if (IS_OSM(card) || IS_OSX(card))
>>>>>>> upstream/android-13
		return (rc) ? rc : -EADDRNOTAVAIL;
	eth_hw_addr_random(card->dev);

out:
<<<<<<< HEAD
	QETH_DBF_HEX(SETUP, 2, card->dev->dev_addr, card->dev->addr_len);
	return 0;
}

=======
	QETH_CARD_HEX(card, 2, card->dev->dev_addr, card->dev->addr_len);
	return 0;
}

static void qeth_l2_register_dev_addr(struct qeth_card *card)
{
	if (!is_valid_ether_addr(card->dev->dev_addr))
		qeth_l2_request_initial_mac(card);

	if (!qeth_l2_send_setmac(card, card->dev->dev_addr))
		card->info.dev_addr_is_registered = 1;
	else
		card->info.dev_addr_is_registered = 0;
}

static int qeth_l2_validate_addr(struct net_device *dev)
{
	struct qeth_card *card = dev->ml_priv;

	if (card->info.dev_addr_is_registered)
		return eth_validate_addr(dev);

	QETH_CARD_TEXT(card, 4, "nomacadr");
	return -EPERM;
}

>>>>>>> upstream/android-13
static int qeth_l2_set_mac_address(struct net_device *dev, void *p)
{
	struct sockaddr *addr = p;
	struct qeth_card *card = dev->ml_priv;
	u8 old_addr[ETH_ALEN];
	int rc = 0;

	QETH_CARD_TEXT(card, 3, "setmac");

<<<<<<< HEAD
	if (card->info.type == QETH_CARD_TYPE_OSN ||
	    card->info.type == QETH_CARD_TYPE_OSM ||
	    card->info.type == QETH_CARD_TYPE_OSX) {
=======
	if (IS_OSM(card) || IS_OSX(card)) {
>>>>>>> upstream/android-13
		QETH_CARD_TEXT(card, 3, "setmcTYP");
		return -EOPNOTSUPP;
	}
	QETH_CARD_HEX(card, 3, addr->sa_data, ETH_ALEN);
	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

<<<<<<< HEAD
	if (qeth_wait_for_threads(card, QETH_RECOVER_THREAD)) {
		QETH_CARD_TEXT(card, 3, "setmcREC");
		return -ERESTARTSYS;
	}

	/* avoid racing against concurrent state change: */
	if (!mutex_trylock(&card->conf_mutex))
		return -EAGAIN;

	if (!qeth_card_hw_is_reachable(card)) {
		ether_addr_copy(dev->dev_addr, addr->sa_data);
		goto out_unlock;
	}

	/* don't register the same address twice */
	if (ether_addr_equal_64bits(dev->dev_addr, addr->sa_data) &&
	    (card->info.mac_bits & QETH_LAYER2_MAC_REGISTERED))
		goto out_unlock;
=======
	/* don't register the same address twice */
	if (ether_addr_equal_64bits(dev->dev_addr, addr->sa_data) &&
	    card->info.dev_addr_is_registered)
		return 0;
>>>>>>> upstream/android-13

	/* add the new address, switch over, drop the old */
	rc = qeth_l2_send_setmac(card, addr->sa_data);
	if (rc)
<<<<<<< HEAD
		goto out_unlock;
	ether_addr_copy(old_addr, dev->dev_addr);
	ether_addr_copy(dev->dev_addr, addr->sa_data);

	if (card->info.mac_bits & QETH_LAYER2_MAC_REGISTERED)
		qeth_l2_remove_mac(card, old_addr);
	card->info.mac_bits |= QETH_LAYER2_MAC_REGISTERED;

out_unlock:
	mutex_unlock(&card->conf_mutex);
	return rc;
}

static void qeth_promisc_to_bridge(struct qeth_card *card)
{
	struct net_device *dev = card->dev;
	enum qeth_ipa_promisc_modes promisc_mode;
=======
		return rc;
	ether_addr_copy(old_addr, dev->dev_addr);
	ether_addr_copy(dev->dev_addr, addr->sa_data);

	if (card->info.dev_addr_is_registered)
		qeth_l2_remove_mac(card, old_addr);
	card->info.dev_addr_is_registered = 1;
	return 0;
}

static void qeth_l2_promisc_to_bridge(struct qeth_card *card, bool enable)
{
>>>>>>> upstream/android-13
	int role;
	int rc;

	QETH_CARD_TEXT(card, 3, "pmisc2br");

<<<<<<< HEAD
	if (!card->options.sbp.reflect_promisc)
		return;
	promisc_mode = (dev->flags & IFF_PROMISC) ? SET_PROMISC_MODE_ON
						: SET_PROMISC_MODE_OFF;
	if (promisc_mode == card->info.promisc_mode)
		return;

	if (promisc_mode == SET_PROMISC_MODE_ON) {
=======
	if (enable) {
>>>>>>> upstream/android-13
		if (card->options.sbp.reflect_promisc_primary)
			role = QETH_SBP_ROLE_PRIMARY;
		else
			role = QETH_SBP_ROLE_SECONDARY;
	} else
		role = QETH_SBP_ROLE_NONE;

	rc = qeth_bridgeport_setrole(card, role);
<<<<<<< HEAD
	QETH_DBF_TEXT_(SETUP, 2, "bpm%c%04x",
			(promisc_mode == SET_PROMISC_MODE_ON) ? '+' : '-', rc);
	if (!rc) {
		card->options.sbp.role = role;
		card->info.promisc_mode = promisc_mode;
	}

}
=======
	QETH_CARD_TEXT_(card, 2, "bpm%c%04x", enable ? '+' : '-', rc);
	if (!rc) {
		card->options.sbp.role = role;
		card->info.promisc_mode = enable;
	}
}

static void qeth_l2_set_promisc_mode(struct qeth_card *card)
{
	bool enable = card->dev->flags & IFF_PROMISC;

	if (card->info.promisc_mode == enable)
		return;

	if (qeth_adp_supported(card, IPA_SETADP_SET_PROMISC_MODE)) {
		qeth_setadp_promisc_mode(card, enable);
	} else {
		mutex_lock(&card->sbp_lock);
		if (card->options.sbp.reflect_promisc)
			qeth_l2_promisc_to_bridge(card, enable);
		mutex_unlock(&card->sbp_lock);
	}
}

>>>>>>> upstream/android-13
/* New MAC address is added to the hash table and marked to be written on card
 * only if there is not in the hash table storage already
 *
*/
static void qeth_l2_add_mac(struct qeth_card *card, struct netdev_hw_addr *ha)
{
	u32 mac_hash = get_unaligned((u32 *)(&ha->addr[2]));
	struct qeth_mac *mac;

<<<<<<< HEAD
	hash_for_each_possible(card->mac_htable, mac, hnode, mac_hash) {
=======
	hash_for_each_possible(card->rx_mode_addrs, mac, hnode, mac_hash) {
>>>>>>> upstream/android-13
		if (ether_addr_equal_64bits(ha->addr, mac->mac_addr)) {
			mac->disp_flag = QETH_DISP_ADDR_DO_NOTHING;
			return;
		}
	}

	mac = kzalloc(sizeof(struct qeth_mac), GFP_ATOMIC);
	if (!mac)
		return;

	ether_addr_copy(mac->mac_addr, ha->addr);
	mac->disp_flag = QETH_DISP_ADDR_ADD;

<<<<<<< HEAD
	hash_add(card->mac_htable, &mac->hnode, mac_hash);
}

static void qeth_l2_set_rx_mode(struct net_device *dev)
{
	struct qeth_card *card = dev->ml_priv;
=======
	hash_add(card->rx_mode_addrs, &mac->hnode, mac_hash);
}

static void qeth_l2_rx_mode_work(struct work_struct *work)
{
	struct qeth_card *card = container_of(work, struct qeth_card,
					      rx_mode_work);
	struct net_device *dev = card->dev;
>>>>>>> upstream/android-13
	struct netdev_hw_addr *ha;
	struct qeth_mac *mac;
	struct hlist_node *tmp;
	int i;
	int rc;

<<<<<<< HEAD
	if (card->info.type == QETH_CARD_TYPE_OSN)
		return;

	QETH_CARD_TEXT(card, 3, "setmulti");
	if (qeth_threads_running(card, QETH_RECOVER_THREAD) &&
	    (card->state != CARD_STATE_UP))
		return;

	spin_lock_bh(&card->mclock);

=======
	QETH_CARD_TEXT(card, 3, "setmulti");

	netif_addr_lock_bh(dev);
>>>>>>> upstream/android-13
	netdev_for_each_mc_addr(ha, dev)
		qeth_l2_add_mac(card, ha);
	netdev_for_each_uc_addr(ha, dev)
		qeth_l2_add_mac(card, ha);
<<<<<<< HEAD

	hash_for_each_safe(card->mac_htable, i, tmp, mac, hnode) {
=======
	netif_addr_unlock_bh(dev);

	hash_for_each_safe(card->rx_mode_addrs, i, tmp, mac, hnode) {
>>>>>>> upstream/android-13
		switch (mac->disp_flag) {
		case QETH_DISP_ADDR_DELETE:
			qeth_l2_remove_mac(card, mac->mac_addr);
			hash_del(&mac->hnode);
			kfree(mac);
			break;
		case QETH_DISP_ADDR_ADD:
			rc = qeth_l2_write_mac(card, mac->mac_addr);
			if (rc) {
				hash_del(&mac->hnode);
				kfree(mac);
				break;
			}
<<<<<<< HEAD
			/* fall through */
=======
			fallthrough;
>>>>>>> upstream/android-13
		default:
			/* for next call to set_rx_mode(): */
			mac->disp_flag = QETH_DISP_ADDR_DELETE;
		}
	}

<<<<<<< HEAD
	spin_unlock_bh(&card->mclock);

	if (qeth_adp_supported(card, IPA_SETADP_SET_PROMISC_MODE))
		qeth_setadp_promisc_mode(card);
	else
		qeth_promisc_to_bridge(card);
}

static int qeth_l2_xmit(struct qeth_card *card, struct sk_buff *skb,
			struct qeth_qdio_out_q *queue, int cast_type, int ipv)
{
	const unsigned int proto_len = IS_IQD(card) ? ETH_HLEN : 0;
	const unsigned int hw_hdr_len = sizeof(struct qeth_hdr);
	unsigned int frame_len = skb->len;
	unsigned int data_offset = 0;
	struct qeth_hdr *hdr = NULL;
	unsigned int hd_len = 0;
	unsigned int elements;
	int push_len, rc;
	bool is_sg;

	rc = skb_cow_head(skb, hw_hdr_len);
	if (rc)
		return rc;

	push_len = qeth_add_hw_header(card, skb, &hdr, hw_hdr_len, proto_len,
				      &elements);
	if (push_len < 0)
		return push_len;
	if (!push_len) {
		/* HW header needs its own buffer element. */
		hd_len = hw_hdr_len + proto_len;
		data_offset = proto_len;
	}
	qeth_l2_fill_header(hdr, skb, cast_type, frame_len);
	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		qeth_tx_csum(skb, &hdr->hdr.l2.flags[1], ipv);
		if (card->options.performance_stats)
			card->perf_stats.tx_csum++;
	}

	is_sg = skb_is_nonlinear(skb);
	if (IS_IQD(card)) {
		rc = qeth_do_send_packet_fast(queue, skb, hdr, data_offset,
					      hd_len);
	} else {
		/* TODO: drop skb_orphan() once TX completion is fast enough */
		skb_orphan(skb);
		rc = qeth_do_send_packet(card, queue, skb, hdr, data_offset,
					 hd_len, elements);
	}

	if (!rc) {
		if (card->options.performance_stats) {
			card->perf_stats.buf_elements_sent += elements;
			if (is_sg)
				card->perf_stats.sg_skbs_sent++;
		}
	} else {
		if (!push_len)
			kmem_cache_free(qeth_core_header_cache, hdr);
		if (rc == -EBUSY)
			/* roll back to ETH header */
			skb_pull(skb, push_len);
	}
	return rc;
}

static int qeth_l2_xmit_osn(struct qeth_card *card, struct sk_buff *skb,
			    struct qeth_qdio_out_q *queue)
{
	unsigned int elements;
	struct qeth_hdr *hdr;

	if (skb->protocol == htons(ETH_P_IPV6))
		return -EPROTONOSUPPORT;

	hdr = (struct qeth_hdr *)skb->data;
	elements = qeth_get_elements_no(card, skb, 0, 0);
	if (!elements)
		return -E2BIG;
	if (qeth_hdr_chk_and_bounce(skb, &hdr, sizeof(*hdr)))
		return -EINVAL;
	return qeth_do_send_packet(card, queue, skb, hdr, 0, 0, elements);
=======
	qeth_l2_set_promisc_mode(card);
>>>>>>> upstream/android-13
}

static netdev_tx_t qeth_l2_hard_start_xmit(struct sk_buff *skb,
					   struct net_device *dev)
{
	struct qeth_card *card = dev->ml_priv;
<<<<<<< HEAD
	int cast_type = qeth_l2_get_cast_type(card, skb);
	int ipv = qeth_get_ip_version(skb);
	struct qeth_qdio_out_q *queue;
	int tx_bytes = skb->len;
	int rc;

	if ((card->state != CARD_STATE_UP) || !card->lan_online) {
		card->stats.tx_carrier_errors++;
		goto tx_drop;
	}

	queue = qeth_get_tx_queue(card, skb, ipv, cast_type);

	if (card->options.performance_stats) {
		card->perf_stats.outbound_cnt++;
		card->perf_stats.outbound_start_time = qeth_get_micros();
	}
	netif_stop_queue(dev);

	if (IS_OSN(card))
		rc = qeth_l2_xmit_osn(card, skb, queue);
	else
		rc = qeth_l2_xmit(card, skb, queue, cast_type, ipv);

	if (!rc) {
		card->stats.tx_packets++;
		card->stats.tx_bytes += tx_bytes;
		if (card->options.performance_stats)
			card->perf_stats.outbound_time += qeth_get_micros() -
				card->perf_stats.outbound_start_time;
		netif_wake_queue(dev);
		return NETDEV_TX_OK;
	} else if (rc == -EBUSY) {
		return NETDEV_TX_BUSY;
	} /* else fall through */

tx_drop:
	card->stats.tx_dropped++;
	card->stats.tx_errors++;
	dev_kfree_skb_any(skb);
	netif_wake_queue(dev);
	return NETDEV_TX_OK;
}

static int __qeth_l2_open(struct net_device *dev)
{
	struct qeth_card *card = dev->ml_priv;
	int rc = 0;

	QETH_CARD_TEXT(card, 4, "qethopen");
	if (card->state == CARD_STATE_UP)
		return rc;
	if (card->state != CARD_STATE_SOFTSETUP)
		return -ENODEV;

	if ((card->info.type != QETH_CARD_TYPE_OSN) &&
	     (!(card->info.mac_bits & QETH_LAYER2_MAC_REGISTERED))) {
		QETH_CARD_TEXT(card, 4, "nomacadr");
		return -EPERM;
	}
	card->data.state = CH_STATE_UP;
	card->state = CARD_STATE_UP;
	netif_start_queue(dev);

	if (qdio_stop_irq(card->data.ccwdev, 0) >= 0) {
		napi_enable(&card->napi);
		local_bh_disable();
		napi_schedule(&card->napi);
		/* kick-start the NAPI softirq: */
		local_bh_enable();
	} else
		rc = -EIO;
	return rc;
}

static int qeth_l2_open(struct net_device *dev)
{
	struct qeth_card *card = dev->ml_priv;

	QETH_CARD_TEXT(card, 5, "qethope_");
	if (qeth_wait_for_threads(card, QETH_RECOVER_THREAD)) {
		QETH_CARD_TEXT(card, 3, "openREC");
		return -ERESTARTSYS;
	}
	return __qeth_l2_open(dev);
}

static int qeth_l2_stop(struct net_device *dev)
{
	struct qeth_card *card = dev->ml_priv;

	QETH_CARD_TEXT(card, 4, "qethstop");
	netif_tx_disable(dev);
	if (card->state == CARD_STATE_UP) {
		card->state = CARD_STATE_SOFTSETUP;
		napi_disable(&card->napi);
	}
	return 0;
}

static const struct device_type qeth_l2_devtype = {
	.name = "qeth_layer2",
	.groups = qeth_l2_attr_groups,
};

static int qeth_l2_probe_device(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	int rc;

	if (gdev->dev.type == &qeth_generic_devtype) {
		rc = qeth_l2_create_device_attributes(&gdev->dev);
		if (rc)
			return rc;
	}
	INIT_LIST_HEAD(&card->vid_list);
	hash_init(card->mac_htable);
	card->options.layer2 = 1;
	card->info.hwtrap = 0;
	qeth_l2_vnicc_set_defaults(card);
	return 0;
}

static void qeth_l2_remove_device(struct ccwgroup_device *cgdev)
{
	struct qeth_card *card = dev_get_drvdata(&cgdev->dev);

	if (cgdev->dev.type == &qeth_generic_devtype)
		qeth_l2_remove_device_attributes(&cgdev->dev);
	qeth_set_allowed_threads(card, 0, 1);
	wait_event(card->wait_q, qeth_threads_running(card, 0xffffffff) == 0);

	if (cgdev->state == CCWGROUP_ONLINE)
		qeth_l2_set_offline(cgdev);

	cancel_work_sync(&card->close_dev_work);
	if (qeth_netdev_is_registered(card->dev))
		unregister_netdev(card->dev);
}

static const struct ethtool_ops qeth_l2_ethtool_ops = {
	.get_link = ethtool_op_get_link,
	.get_strings = qeth_core_get_strings,
	.get_ethtool_stats = qeth_core_get_ethtool_stats,
	.get_sset_count = qeth_core_get_sset_count,
	.get_drvinfo = qeth_core_get_drvinfo,
	.get_link_ksettings = qeth_core_ethtool_get_link_ksettings,
};

static const struct ethtool_ops qeth_l2_osn_ops = {
	.get_strings = qeth_core_get_strings,
	.get_ethtool_stats = qeth_core_get_ethtool_stats,
	.get_sset_count = qeth_core_get_sset_count,
	.get_drvinfo = qeth_core_get_drvinfo,
};

static const struct net_device_ops qeth_l2_netdev_ops = {
	.ndo_open		= qeth_l2_open,
	.ndo_stop		= qeth_l2_stop,
	.ndo_get_stats		= qeth_get_stats,
	.ndo_start_xmit		= qeth_l2_hard_start_xmit,
	.ndo_features_check	= qeth_features_check,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_set_rx_mode	= qeth_l2_set_rx_mode,
	.ndo_do_ioctl		= qeth_do_ioctl,
	.ndo_set_mac_address    = qeth_l2_set_mac_address,
	.ndo_vlan_rx_add_vid	= qeth_l2_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid   = qeth_l2_vlan_rx_kill_vid,
	.ndo_tx_timeout	   	= qeth_tx_timeout,
	.ndo_fix_features	= qeth_fix_features,
	.ndo_set_features	= qeth_set_features
=======
	u16 txq = skb_get_queue_mapping(skb);
	struct qeth_qdio_out_q *queue;
	int rc;

	if (!skb_is_gso(skb))
		qdisc_skb_cb(skb)->pkt_len = skb->len;
	if (IS_IQD(card))
		txq = qeth_iqd_translate_txq(dev, txq);
	queue = card->qdio.out_qs[txq];

	rc = qeth_xmit(card, skb, queue, vlan_get_protocol(skb),
		       qeth_l2_fill_header);
	if (!rc)
		return NETDEV_TX_OK;

	QETH_TXQ_STAT_INC(queue, tx_dropped);
	kfree_skb(skb);
	return NETDEV_TX_OK;
}

static u16 qeth_l2_select_queue(struct net_device *dev, struct sk_buff *skb,
				struct net_device *sb_dev)
{
	struct qeth_card *card = dev->ml_priv;

	if (IS_IQD(card))
		return qeth_iqd_select_queue(dev, skb,
					     qeth_get_ether_cast_type(skb),
					     sb_dev);
	if (qeth_uses_tx_prio_queueing(card))
		return qeth_get_priority_queue(card, skb);

	return netdev_pick_tx(dev, skb, sb_dev);
}

static void qeth_l2_set_rx_mode(struct net_device *dev)
{
	struct qeth_card *card = dev->ml_priv;

	schedule_work(&card->rx_mode_work);
}

/**
 *	qeth_l2_pnso() - perform network subchannel operation
 *	@card: qeth_card structure pointer
 *	@oc: Operation Code
 *	@cnc: Boolean Change-Notification Control
 *	@cb: Callback function will be executed for each element
 *		of the address list
 *	@priv: Pointer to pass to the callback function.
 *
 *	Collects network information in a network address list and calls the
 *	callback function for every entry in the list. If "change-notification-
 *	control" is set, further changes in the address list will be reported
 *	via the IPA command.
 */
static int qeth_l2_pnso(struct qeth_card *card, u8 oc, int cnc,
			void (*cb)(void *priv, struct chsc_pnso_naid_l2 *entry),
			void *priv)
{
	struct ccw_device *ddev = CARD_DDEV(card);
	struct chsc_pnso_area *rr;
	u32 prev_instance = 0;
	int isfirstblock = 1;
	int i, size, elems;
	int rc;

	rr = (struct chsc_pnso_area *)get_zeroed_page(GFP_KERNEL);
	if (rr == NULL)
		return -ENOMEM;
	do {
		QETH_CARD_TEXT(card, 2, "PNSO");
		/* on the first iteration, naihdr.resume_token will be zero */
		rc = ccw_device_pnso(ddev, rr, oc, rr->naihdr.resume_token,
				     cnc);
		if (rc)
			continue;
		if (cb == NULL)
			continue;

		size = rr->naihdr.naids;
		if (size != sizeof(struct chsc_pnso_naid_l2)) {
			WARN_ON_ONCE(1);
			continue;
		}

		elems = (rr->response.length - sizeof(struct chsc_header) -
			 sizeof(struct chsc_pnso_naihdr)) / size;

		if (!isfirstblock && (rr->naihdr.instance != prev_instance)) {
			/* Inform the caller that they need to scrap */
			/* the data that was already reported via cb */
			rc = -EAGAIN;
			break;
		}
		isfirstblock = 0;
		prev_instance = rr->naihdr.instance;
		for (i = 0; i < elems; i++)
			(*cb)(priv, &rr->entries[i]);
	} while ((rc == -EBUSY) || (!rc && /* list stored */
		   /* resume token is non-zero => list incomplete */
		   (rr->naihdr.resume_token.t1 || rr->naihdr.resume_token.t2)));

	if (rc)
		QETH_CARD_TEXT_(card, 2, "PNrp%04x", rr->response.code);

	free_page((unsigned long)rr);
	return rc;
}

static bool qeth_is_my_net_if_token(struct qeth_card *card,
				    struct net_if_token *token)
{
	return ((card->info.ddev_devno == token->devnum) &&
		(card->info.cssid == token->cssid) &&
		(card->info.iid == token->iid) &&
		(card->info.ssid == token->ssid) &&
		(card->info.chpid == token->chpid) &&
		(card->info.chid == token->chid));
}

/**
 *	qeth_l2_dev2br_fdb_notify() - update fdb of master bridge
 *	@card:	qeth_card structure pointer
 *	@code:	event bitmask: high order bit 0x80 set to
 *				1 - removal of an object
 *				0 - addition of an object
 *			       Object type(s):
 *				0x01 - VLAN, 0x02 - MAC, 0x03 - VLAN and MAC
 *	@token: "network token" structure identifying 'physical' location
 *		of the target
 *	@addr_lnid: structure with MAC address and VLAN ID of the target
 */
static void qeth_l2_dev2br_fdb_notify(struct qeth_card *card, u8 code,
				      struct net_if_token *token,
				      struct mac_addr_lnid *addr_lnid)
{
	struct switchdev_notifier_fdb_info info = {};
	u8 ntfy_mac[ETH_ALEN];

	ether_addr_copy(ntfy_mac, addr_lnid->mac);
	/* Ignore VLAN only changes */
	if (!(code & IPA_ADDR_CHANGE_CODE_MACADDR))
		return;
	/* Ignore mcast entries */
	if (is_multicast_ether_addr(ntfy_mac))
		return;
	/* Ignore my own addresses */
	if (qeth_is_my_net_if_token(card, token))
		return;

	info.addr = ntfy_mac;
	/* don't report VLAN IDs */
	info.vid = 0;
	info.added_by_user = false;
	info.offloaded = true;

	if (code & IPA_ADDR_CHANGE_CODE_REMOVAL) {
		call_switchdev_notifiers(SWITCHDEV_FDB_DEL_TO_BRIDGE,
					 card->dev, &info.info, NULL);
		QETH_CARD_TEXT(card, 4, "andelmac");
		QETH_CARD_TEXT_(card, 4,
				"mc%012lx", ether_addr_to_u64(ntfy_mac));
	} else {
		call_switchdev_notifiers(SWITCHDEV_FDB_ADD_TO_BRIDGE,
					 card->dev, &info.info, NULL);
		QETH_CARD_TEXT(card, 4, "anaddmac");
		QETH_CARD_TEXT_(card, 4,
				"mc%012lx", ether_addr_to_u64(ntfy_mac));
	}
}

static void qeth_l2_dev2br_an_set_cb(void *priv,
				     struct chsc_pnso_naid_l2 *entry)
{
	u8 code = IPA_ADDR_CHANGE_CODE_MACADDR;
	struct qeth_card *card = priv;

	if (entry->addr_lnid.lnid < VLAN_N_VID)
		code |= IPA_ADDR_CHANGE_CODE_VLANID;
	qeth_l2_dev2br_fdb_notify(card, code,
				  (struct net_if_token *)&entry->nit,
				  (struct mac_addr_lnid *)&entry->addr_lnid);
}

/**
 *	qeth_l2_dev2br_an_set() -
 *	Enable or disable 'dev to bridge network address notification'
 *	@card: qeth_card structure pointer
 *	@enable: Enable or disable 'dev to bridge network address notification'
 *
 *	Returns negative errno-compatible error indication or 0 on success.
 *
 *	On enable, emits a series of address notifications for all
 *	currently registered hosts.
 */
static int qeth_l2_dev2br_an_set(struct qeth_card *card, bool enable)
{
	int rc;

	if (enable) {
		QETH_CARD_TEXT(card, 2, "anseton");
		rc = qeth_l2_pnso(card, PNSO_OC_NET_ADDR_INFO, 1,
				  qeth_l2_dev2br_an_set_cb, card);
		if (rc == -EAGAIN)
			/* address notification enabled, but inconsistent
			 * addresses reported -> disable address notification
			 */
			qeth_l2_pnso(card, PNSO_OC_NET_ADDR_INFO, 0,
				     NULL, NULL);
	} else {
		QETH_CARD_TEXT(card, 2, "ansetoff");
		rc = qeth_l2_pnso(card, PNSO_OC_NET_ADDR_INFO, 0, NULL, NULL);
	}

	return rc;
}

struct qeth_l2_br2dev_event_work {
	struct work_struct work;
	struct net_device *br_dev;
	struct net_device *lsync_dev;
	struct net_device *dst_dev;
	unsigned long event;
	unsigned char addr[ETH_ALEN];
};

static const struct net_device_ops qeth_l2_netdev_ops;

static bool qeth_l2_must_learn(struct net_device *netdev,
			       struct net_device *dstdev)
{
	struct qeth_priv *priv;

	priv = netdev_priv(netdev);
	return (netdev != dstdev &&
		(priv->brport_features & BR_LEARNING_SYNC) &&
		!(br_port_flag_is_set(netdev, BR_ISOLATED) &&
		  br_port_flag_is_set(dstdev, BR_ISOLATED)) &&
		netdev->netdev_ops == &qeth_l2_netdev_ops);
}

/**
 *	qeth_l2_br2dev_worker() - update local MACs
 *	@work: bridge to device FDB update
 *
 *	Update local MACs of a learning_sync bridgeport so it can receive
 *	messages for a destination port.
 *	In case of an isolated learning_sync port, also update its isolated
 *	siblings.
 */
static void qeth_l2_br2dev_worker(struct work_struct *work)
{
	struct qeth_l2_br2dev_event_work *br2dev_event_work =
		container_of(work, struct qeth_l2_br2dev_event_work, work);
	struct net_device *lsyncdev = br2dev_event_work->lsync_dev;
	struct net_device *dstdev = br2dev_event_work->dst_dev;
	struct net_device *brdev = br2dev_event_work->br_dev;
	unsigned long event = br2dev_event_work->event;
	unsigned char *addr = br2dev_event_work->addr;
	struct qeth_card *card = lsyncdev->ml_priv;
	struct net_device *lowerdev;
	struct list_head *iter;
	int err = 0;

	kfree(br2dev_event_work);
	QETH_CARD_TEXT_(card, 4, "b2dw%04x", event);
	QETH_CARD_TEXT_(card, 4, "ma%012lx", ether_addr_to_u64(addr));

	rcu_read_lock();
	/* Verify preconditions are still valid: */
	if (!netif_is_bridge_port(lsyncdev) ||
	    brdev != netdev_master_upper_dev_get_rcu(lsyncdev))
		goto unlock;
	if (!qeth_l2_must_learn(lsyncdev, dstdev))
		goto unlock;

	if (br_port_flag_is_set(lsyncdev, BR_ISOLATED)) {
		/* Update lsyncdev and its isolated sibling(s): */
		iter = &brdev->adj_list.lower;
		lowerdev = netdev_next_lower_dev_rcu(brdev, &iter);
		while (lowerdev) {
			if (br_port_flag_is_set(lowerdev, BR_ISOLATED)) {
				switch (event) {
				case SWITCHDEV_FDB_ADD_TO_DEVICE:
					err = dev_uc_add(lowerdev, addr);
					break;
				case SWITCHDEV_FDB_DEL_TO_DEVICE:
					err = dev_uc_del(lowerdev, addr);
					break;
				default:
					break;
				}
				if (err) {
					QETH_CARD_TEXT(card, 2, "b2derris");
					QETH_CARD_TEXT_(card, 2,
							"err%02x%03d", event,
							lowerdev->ifindex);
				}
			}
			lowerdev = netdev_next_lower_dev_rcu(brdev, &iter);
		}
	} else {
		switch (event) {
		case SWITCHDEV_FDB_ADD_TO_DEVICE:
			err = dev_uc_add(lsyncdev, addr);
			break;
		case SWITCHDEV_FDB_DEL_TO_DEVICE:
			err = dev_uc_del(lsyncdev, addr);
			break;
		default:
			break;
		}
		if (err)
			QETH_CARD_TEXT_(card, 2, "b2derr%02x", event);
	}

unlock:
	rcu_read_unlock();
	dev_put(brdev);
	dev_put(lsyncdev);
	dev_put(dstdev);
}

static int qeth_l2_br2dev_queue_work(struct net_device *brdev,
				     struct net_device *lsyncdev,
				     struct net_device *dstdev,
				     unsigned long event,
				     const unsigned char *addr)
{
	struct qeth_l2_br2dev_event_work *worker_data;
	struct qeth_card *card;

	worker_data = kzalloc(sizeof(*worker_data), GFP_ATOMIC);
	if (!worker_data)
		return -ENOMEM;
	INIT_WORK(&worker_data->work, qeth_l2_br2dev_worker);
	worker_data->br_dev = brdev;
	worker_data->lsync_dev = lsyncdev;
	worker_data->dst_dev = dstdev;
	worker_data->event = event;
	ether_addr_copy(worker_data->addr, addr);

	card = lsyncdev->ml_priv;
	/* Take a reference on the sw port devices and the bridge */
	dev_hold(brdev);
	dev_hold(lsyncdev);
	dev_hold(dstdev);
	queue_work(card->event_wq, &worker_data->work);
	return 0;
}

/* Called under rtnl_lock */
static int qeth_l2_switchdev_event(struct notifier_block *unused,
				   unsigned long event, void *ptr)
{
	struct net_device *dstdev, *brdev, *lowerdev;
	struct switchdev_notifier_fdb_info *fdb_info;
	struct switchdev_notifier_info *info = ptr;
	struct list_head *iter;
	struct qeth_card *card;
	int rc;

	if (!(event == SWITCHDEV_FDB_ADD_TO_DEVICE ||
	      event == SWITCHDEV_FDB_DEL_TO_DEVICE))
		return NOTIFY_DONE;

	dstdev = switchdev_notifier_info_to_dev(info);
	brdev = netdev_master_upper_dev_get_rcu(dstdev);
	if (!brdev || !netif_is_bridge_master(brdev))
		return NOTIFY_DONE;
	fdb_info = container_of(info,
				struct switchdev_notifier_fdb_info,
				info);
	iter = &brdev->adj_list.lower;
	lowerdev = netdev_next_lower_dev_rcu(brdev, &iter);
	while (lowerdev) {
		if (qeth_l2_must_learn(lowerdev, dstdev)) {
			card = lowerdev->ml_priv;
			QETH_CARD_TEXT_(card, 4, "b2dqw%03x", event);
			rc = qeth_l2_br2dev_queue_work(brdev, lowerdev,
						       dstdev, event,
						       fdb_info->addr);
			if (rc) {
				QETH_CARD_TEXT(card, 2, "b2dqwerr");
				return NOTIFY_BAD;
			}
		}
		lowerdev = netdev_next_lower_dev_rcu(brdev, &iter);
	}
	return NOTIFY_DONE;
}

static struct notifier_block qeth_l2_sw_notifier = {
		.notifier_call = qeth_l2_switchdev_event,
};

static refcount_t qeth_l2_switchdev_notify_refcnt;

/* Called under rtnl_lock */
static void qeth_l2_br2dev_get(void)
{
	int rc;

	if (!refcount_inc_not_zero(&qeth_l2_switchdev_notify_refcnt)) {
		rc = register_switchdev_notifier(&qeth_l2_sw_notifier);
		if (rc) {
			QETH_DBF_MESSAGE(2,
					 "failed to register qeth_l2_sw_notifier: %d\n",
					 rc);
		} else {
			refcount_set(&qeth_l2_switchdev_notify_refcnt, 1);
			QETH_DBF_MESSAGE(2, "qeth_l2_sw_notifier registered\n");
		}
	}
	QETH_DBF_TEXT_(SETUP, 2, "b2d+%04d",
		       qeth_l2_switchdev_notify_refcnt.refs.counter);
}

/* Called under rtnl_lock */
static void qeth_l2_br2dev_put(void)
{
	int rc;

	if (refcount_dec_and_test(&qeth_l2_switchdev_notify_refcnt)) {
		rc = unregister_switchdev_notifier(&qeth_l2_sw_notifier);
		if (rc) {
			QETH_DBF_MESSAGE(2,
					 "failed to unregister qeth_l2_sw_notifier: %d\n",
					 rc);
		} else {
			QETH_DBF_MESSAGE(2,
					 "qeth_l2_sw_notifier unregistered\n");
		}
	}
	QETH_DBF_TEXT_(SETUP, 2, "b2d-%04d",
		       qeth_l2_switchdev_notify_refcnt.refs.counter);
}

static int qeth_l2_bridge_getlink(struct sk_buff *skb, u32 pid, u32 seq,
				  struct net_device *dev, u32 filter_mask,
				  int nlflags)
{
	struct qeth_priv *priv = netdev_priv(dev);
	struct qeth_card *card = dev->ml_priv;
	u16 mode = BRIDGE_MODE_UNDEF;

	/* Do not even show qeth devs that cannot do bridge_setlink */
	if (!priv->brport_hw_features || !netif_device_present(dev) ||
	    qeth_bridgeport_is_in_use(card))
		return -EOPNOTSUPP;

	return ndo_dflt_bridge_getlink(skb, pid, seq, dev,
				       mode, priv->brport_features,
				       priv->brport_hw_features,
				       nlflags, filter_mask, NULL);
}

static const struct nla_policy qeth_brport_policy[IFLA_BRPORT_MAX + 1] = {
	[IFLA_BRPORT_LEARNING_SYNC]	= { .type = NLA_U8 },
};

/**
 *	qeth_l2_bridge_setlink() - set bridgeport attributes
 *	@dev: netdevice
 *	@nlh: netlink message header
 *	@flags: bridge flags (here: BRIDGE_FLAGS_SELF)
 *	@extack: extended ACK report struct
 *
 *	Called under rtnl_lock
 */
static int qeth_l2_bridge_setlink(struct net_device *dev, struct nlmsghdr *nlh,
				  u16 flags, struct netlink_ext_ack *extack)
{
	struct qeth_priv *priv = netdev_priv(dev);
	struct nlattr *bp_tb[IFLA_BRPORT_MAX + 1];
	struct qeth_card *card = dev->ml_priv;
	struct nlattr *attr, *nested_attr;
	bool enable, has_protinfo = false;
	int rem1, rem2;
	int rc;

	if (!netif_device_present(dev))
		return -ENODEV;

	nlmsg_for_each_attr(attr, nlh, sizeof(struct ifinfomsg), rem1) {
		if (nla_type(attr) == IFLA_PROTINFO) {
			rc = nla_parse_nested(bp_tb, IFLA_BRPORT_MAX, attr,
					      qeth_brport_policy, extack);
			if (rc)
				return rc;
			has_protinfo = true;
		} else if (nla_type(attr) == IFLA_AF_SPEC) {
			nla_for_each_nested(nested_attr, attr, rem2) {
				if (nla_type(nested_attr) == IFLA_BRIDGE_FLAGS)
					continue;
				NL_SET_ERR_MSG_ATTR(extack, nested_attr,
						    "Unsupported attribute");
				return -EINVAL;
			}
		} else {
			NL_SET_ERR_MSG_ATTR(extack, attr, "Unsupported attribute");
			return -EINVAL;
		}
	}
	if (!has_protinfo)
		return 0;
	if (!bp_tb[IFLA_BRPORT_LEARNING_SYNC])
		return -EINVAL;
	if (!(priv->brport_hw_features & BR_LEARNING_SYNC)) {
		NL_SET_ERR_MSG_ATTR(extack, bp_tb[IFLA_BRPORT_LEARNING_SYNC],
				    "Operation not supported by HW");
		return -EOPNOTSUPP;
	}
	if (!IS_ENABLED(CONFIG_NET_SWITCHDEV)) {
		NL_SET_ERR_MSG_ATTR(extack, bp_tb[IFLA_BRPORT_LEARNING_SYNC],
				    "Requires NET_SWITCHDEV");
		return -EOPNOTSUPP;
	}
	enable = !!nla_get_u8(bp_tb[IFLA_BRPORT_LEARNING_SYNC]);

	if (enable == !!(priv->brport_features & BR_LEARNING_SYNC))
		return 0;

	mutex_lock(&card->sbp_lock);
	/* do not change anything if BridgePort is enabled */
	if (qeth_bridgeport_is_in_use(card)) {
		NL_SET_ERR_MSG(extack, "n/a (BridgePort)");
		rc = -EBUSY;
	} else if (enable) {
		qeth_l2_set_pnso_mode(card, QETH_PNSO_ADDR_INFO);
		rc = qeth_l2_dev2br_an_set(card, true);
		if (rc) {
			qeth_l2_set_pnso_mode(card, QETH_PNSO_NONE);
		} else {
			priv->brport_features |= BR_LEARNING_SYNC;
			qeth_l2_br2dev_get();
		}
	} else {
		rc = qeth_l2_dev2br_an_set(card, false);
		if (!rc) {
			qeth_l2_set_pnso_mode(card, QETH_PNSO_NONE);
			priv->brport_features ^= BR_LEARNING_SYNC;
			qeth_l2_dev2br_fdb_flush(card);
			qeth_l2_br2dev_put();
		}
	}
	mutex_unlock(&card->sbp_lock);

	return rc;
}

static const struct net_device_ops qeth_l2_netdev_ops = {
	.ndo_open		= qeth_open,
	.ndo_stop		= qeth_stop,
	.ndo_get_stats64	= qeth_get_stats64,
	.ndo_start_xmit		= qeth_l2_hard_start_xmit,
	.ndo_features_check	= qeth_features_check,
	.ndo_select_queue	= qeth_l2_select_queue,
	.ndo_validate_addr	= qeth_l2_validate_addr,
	.ndo_set_rx_mode	= qeth_l2_set_rx_mode,
	.ndo_eth_ioctl		= qeth_do_ioctl,
	.ndo_siocdevprivate	= qeth_siocdevprivate,
	.ndo_set_mac_address    = qeth_l2_set_mac_address,
	.ndo_vlan_rx_add_vid	= qeth_l2_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid   = qeth_l2_vlan_rx_kill_vid,
	.ndo_tx_timeout		= qeth_tx_timeout,
	.ndo_fix_features	= qeth_fix_features,
	.ndo_set_features	= qeth_set_features,
	.ndo_bridge_getlink	= qeth_l2_bridge_getlink,
	.ndo_bridge_setlink	= qeth_l2_bridge_setlink,
>>>>>>> upstream/android-13
};

static int qeth_l2_setup_netdev(struct qeth_card *card)
{
<<<<<<< HEAD
	int rc;

	if (qeth_netdev_is_registered(card->dev))
		return 0;

	card->dev->priv_flags |= IFF_UNICAST_FLT;
	card->dev->netdev_ops = &qeth_l2_netdev_ops;
	if (card->info.type == QETH_CARD_TYPE_OSN) {
		card->dev->ethtool_ops = &qeth_l2_osn_ops;
		card->dev->flags |= IFF_NOARP;
	} else {
		card->dev->ethtool_ops = &qeth_l2_ethtool_ops;
		card->dev->needed_headroom = sizeof(struct qeth_hdr);
	}

	if (card->info.type == QETH_CARD_TYPE_OSM)
		card->dev->features |= NETIF_F_VLAN_CHALLENGED;
	else
		card->dev->features |= NETIF_F_HW_VLAN_CTAG_FILTER;

	if (card->info.type == QETH_CARD_TYPE_OSD && !card->info.guestlan) {
=======
	card->dev->needed_headroom = sizeof(struct qeth_hdr);
	card->dev->netdev_ops = &qeth_l2_netdev_ops;
	card->dev->priv_flags |= IFF_UNICAST_FLT;

	if (IS_OSM(card)) {
		card->dev->features |= NETIF_F_VLAN_CHALLENGED;
	} else {
		if (!IS_VM_NIC(card))
			card->dev->hw_features |= NETIF_F_HW_VLAN_CTAG_FILTER;
		card->dev->features |= NETIF_F_HW_VLAN_CTAG_FILTER;
	}

	if (IS_OSD(card) && !IS_VM_NIC(card)) {
>>>>>>> upstream/android-13
		card->dev->features |= NETIF_F_SG;
		/* OSA 3S and earlier has no RX/TX support */
		if (qeth_is_supported(card, IPA_OUTBOUND_CHECKSUM)) {
			card->dev->hw_features |= NETIF_F_IP_CSUM;
			card->dev->vlan_features |= NETIF_F_IP_CSUM;
		}
	}
	if (qeth_is_supported6(card, IPA_OUTBOUND_CHECKSUM_V6)) {
		card->dev->hw_features |= NETIF_F_IPV6_CSUM;
		card->dev->vlan_features |= NETIF_F_IPV6_CSUM;
	}
	if (qeth_is_supported(card, IPA_INBOUND_CHECKSUM) ||
	    qeth_is_supported6(card, IPA_INBOUND_CHECKSUM_V6)) {
		card->dev->hw_features |= NETIF_F_RXCSUM;
		card->dev->vlan_features |= NETIF_F_RXCSUM;
	}
<<<<<<< HEAD

	qeth_l2_request_initial_mac(card);
	netif_napi_add(card->dev, &card->napi, qeth_poll, QETH_NAPI_WEIGHT);
	rc = register_netdev(card->dev);
	if (rc)
		card->dev->netdev_ops = NULL;
	return rc;
}

static int qeth_l2_start_ipassists(struct qeth_card *card)
{
	/* configure isolation level */
	if (qeth_set_access_ctrl_online(card, 0))
		return -ENODEV;
	return 0;
=======
	if (qeth_is_supported(card, IPA_OUTBOUND_TSO)) {
		card->dev->hw_features |= NETIF_F_TSO;
		card->dev->vlan_features |= NETIF_F_TSO;
	}
	if (qeth_is_supported6(card, IPA_OUTBOUND_TSO)) {
		card->dev->hw_features |= NETIF_F_TSO6;
		card->dev->vlan_features |= NETIF_F_TSO6;
	}

	if (card->dev->hw_features & (NETIF_F_TSO | NETIF_F_TSO6)) {
		card->dev->needed_headroom = sizeof(struct qeth_hdr_tso);
		netif_keep_dst(card->dev);
		netif_set_gso_max_size(card->dev,
				       PAGE_SIZE * (QDIO_MAX_ELEMENTS_PER_BUFFER - 1));
	}

	netif_napi_add(card->dev, &card->napi, qeth_poll, QETH_NAPI_WEIGHT);
	return register_netdev(card->dev);
>>>>>>> upstream/android-13
}

static void qeth_l2_trace_features(struct qeth_card *card)
{
	/* Set BridgePort features */
	QETH_CARD_TEXT(card, 2, "featuSBP");
	QETH_CARD_HEX(card, 2, &card->options.sbp.supported_funcs,
		      sizeof(card->options.sbp.supported_funcs));
	/* VNIC Characteristics features */
	QETH_CARD_TEXT(card, 2, "feaVNICC");
	QETH_CARD_HEX(card, 2, &card->options.vnicc.sup_chars,
		      sizeof(card->options.vnicc.sup_chars));
}

<<<<<<< HEAD
static int __qeth_l2_set_online(struct ccwgroup_device *gdev, int recovery_mode)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	int rc = 0;
	enum qeth_card_states recover_flag;

	mutex_lock(&card->discipline_mutex);
	mutex_lock(&card->conf_mutex);
	QETH_DBF_TEXT(SETUP, 2, "setonlin");
	QETH_DBF_HEX(SETUP, 2, &card, sizeof(void *));

	recover_flag = card->state;
	rc = qeth_core_hardsetup_card(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "2err%04x", rc);
		rc = -ENODEV;
		goto out_remove;
	}
	qeth_bridgeport_query_support(card);
	if (card->options.sbp.supported_funcs)
		dev_info(&card->gdev->dev,
		"The device represents a Bridge Capable Port\n");

	rc = qeth_l2_setup_netdev(card);
	if (rc)
		goto out_remove;

	if (card->info.type != QETH_CARD_TYPE_OSN &&
	    !qeth_l2_send_setmac(card, card->dev->dev_addr))
		card->info.mac_bits |= QETH_LAYER2_MAC_REGISTERED;

	if (qeth_is_diagass_supported(card, QETH_DIAGS_CMD_TRAP)) {
		if (card->info.hwtrap &&
		    qeth_hw_trap(card, QETH_DIAGS_TRAP_ARM))
			card->info.hwtrap = 0;
	} else
		card->info.hwtrap = 0;

	/* for the rx_bcast characteristic, init VNICC after setmac */
	qeth_l2_vnicc_init(card);

	qeth_trace_features(card);
	qeth_l2_trace_features(card);

	qeth_l2_setup_bridgeport_attrs(card);

	card->state = CARD_STATE_HARDSETUP;
	qeth_print_status_message(card);

	/* softsetup */
	QETH_DBF_TEXT(SETUP, 2, "softsetp");

	if ((card->info.type == QETH_CARD_TYPE_OSD) ||
	    (card->info.type == QETH_CARD_TYPE_OSX)) {
		rc = qeth_l2_start_ipassists(card);
		if (rc)
			goto out_remove;
	}

	if (card->info.type != QETH_CARD_TYPE_OSN)
		qeth_l2_process_vlans(card);

	netif_tx_disable(card->dev);

	rc = qeth_init_qdio_queues(card);
	if (rc) {
		QETH_DBF_TEXT_(SETUP, 2, "6err%d", rc);
		rc = -ENODEV;
		goto out_remove;
	}
	card->state = CARD_STATE_SOFTSETUP;
	if (card->lan_online)
		netif_carrier_on(card->dev);
	else
		netif_carrier_off(card->dev);

	qeth_set_allowed_threads(card, 0xffffffff, 0);

	qeth_enable_hw_features(card->dev);
	if (recover_flag == CARD_STATE_RECOVER) {
		if (recovery_mode &&
		    card->info.type != QETH_CARD_TYPE_OSN) {
			__qeth_l2_open(card->dev);
			qeth_l2_set_rx_mode(card->dev);
		} else {
			rtnl_lock();
			dev_open(card->dev);
			rtnl_unlock();
		}
	}
	/* let user_space know that device is online */
	kobject_uevent(&gdev->dev.kobj, KOBJ_CHANGE);
	mutex_unlock(&card->conf_mutex);
	mutex_unlock(&card->discipline_mutex);
	return 0;

out_remove:
	qeth_l2_stop_card(card, 0);
	ccw_device_set_offline(CARD_DDEV(card));
	ccw_device_set_offline(CARD_WDEV(card));
	ccw_device_set_offline(CARD_RDEV(card));
	qdio_free(CARD_DDEV(card));
	if (recover_flag == CARD_STATE_RECOVER)
		card->state = CARD_STATE_RECOVER;
	else
		card->state = CARD_STATE_DOWN;
	mutex_unlock(&card->conf_mutex);
	mutex_unlock(&card->discipline_mutex);
	return rc;
}

static int qeth_l2_set_online(struct ccwgroup_device *gdev)
{
	return __qeth_l2_set_online(gdev, 0);
}

static int __qeth_l2_set_offline(struct ccwgroup_device *cgdev,
					int recovery_mode)
{
	struct qeth_card *card = dev_get_drvdata(&cgdev->dev);
	int rc = 0, rc2 = 0, rc3 = 0;
	enum qeth_card_states recover_flag;

	mutex_lock(&card->discipline_mutex);
	mutex_lock(&card->conf_mutex);
	QETH_DBF_TEXT(SETUP, 3, "setoffl");
	QETH_DBF_HEX(SETUP, 3, &card, sizeof(void *));

	netif_carrier_off(card->dev);
	recover_flag = card->state;
	if ((!recovery_mode && card->info.hwtrap) || card->info.hwtrap == 2) {
		qeth_hw_trap(card, QETH_DIAGS_TRAP_DISARM);
		card->info.hwtrap = 1;
	}
	qeth_l2_stop_card(card, recovery_mode);
	rc  = ccw_device_set_offline(CARD_DDEV(card));
	rc2 = ccw_device_set_offline(CARD_WDEV(card));
	rc3 = ccw_device_set_offline(CARD_RDEV(card));
	if (!rc)
		rc = (rc2) ? rc2 : rc3;
	if (rc)
		QETH_DBF_TEXT_(SETUP, 2, "1err%d", rc);
	qdio_free(CARD_DDEV(card));
	if (recover_flag == CARD_STATE_UP)
		card->state = CARD_STATE_RECOVER;
	/* let user_space know that device is offline */
	kobject_uevent(&cgdev->dev.kobj, KOBJ_CHANGE);
	mutex_unlock(&card->conf_mutex);
	mutex_unlock(&card->discipline_mutex);
	return 0;
}

static int qeth_l2_set_offline(struct ccwgroup_device *cgdev)
{
	return __qeth_l2_set_offline(cgdev, 0);
}

static int qeth_l2_recover(void *ptr)
{
	struct qeth_card *card;
	int rc = 0;

	card = (struct qeth_card *) ptr;
	QETH_CARD_TEXT(card, 2, "recover1");
	if (!qeth_do_run_thread(card, QETH_RECOVER_THREAD))
		return 0;
	QETH_CARD_TEXT(card, 2, "recover2");
	dev_warn(&card->gdev->dev,
		"A recovery process has been started for the device\n");
	qeth_set_recovery_task(card);
	__qeth_l2_set_offline(card->gdev, 1);
	rc = __qeth_l2_set_online(card->gdev, 1);
	if (!rc)
		dev_info(&card->gdev->dev,
			"Device successfully recovered!\n");
	else {
		qeth_close_dev(card);
		dev_warn(&card->gdev->dev, "The qeth device driver "
				"failed to recover an error on the device\n");
	}
	qeth_clear_recovery_task(card);
	qeth_clear_thread_start_bit(card, QETH_RECOVER_THREAD);
	qeth_clear_thread_running_bit(card, QETH_RECOVER_THREAD);
	return 0;
}

static int __init qeth_l2_init(void)
{
	pr_info("register layer 2 discipline\n");
	return 0;
}

static void __exit qeth_l2_exit(void)
{
	pr_info("unregister layer 2 discipline\n");
}

static int qeth_l2_pm_suspend(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);

	netif_device_detach(card->dev);
	qeth_set_allowed_threads(card, 0, 1);
	wait_event(card->wait_q, qeth_threads_running(card, 0xffffffff) == 0);
	if (gdev->state == CCWGROUP_OFFLINE)
		return 0;
	if (card->state == CARD_STATE_UP) {
		if (card->info.hwtrap)
			qeth_hw_trap(card, QETH_DIAGS_TRAP_DISARM);
		__qeth_l2_set_offline(card->gdev, 1);
	} else
		__qeth_l2_set_offline(card->gdev, 0);
	return 0;
}

static int qeth_l2_pm_resume(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	int rc = 0;

	if (gdev->state == CCWGROUP_OFFLINE)
		goto out;

	if (card->state == CARD_STATE_RECOVER) {
		rc = __qeth_l2_set_online(card->gdev, 1);
		if (rc) {
			rtnl_lock();
			dev_close(card->dev);
			rtnl_unlock();
		}
	} else
		rc = __qeth_l2_set_online(card->gdev, 0);
out:
	qeth_set_allowed_threads(card, 0xffffffff, 0);
	netif_device_attach(card->dev);
	if (rc)
		dev_warn(&card->gdev->dev, "The qeth device driver "
			"failed to recover an error on the device\n");
	return rc;
}

/* Returns zero if the command is successfully "consumed" */
static int qeth_l2_control_event(struct qeth_card *card,
					struct qeth_ipa_cmd *cmd)
{
	switch (cmd->hdr.command) {
	case IPA_CMD_SETBRIDGEPORT_OSA:
	case IPA_CMD_SETBRIDGEPORT_IQD:
		if (cmd->data.sbp.hdr.command_code ==
				IPA_SBP_BRIDGE_PORT_STATE_CHANGE) {
			qeth_bridge_state_change(card, cmd);
			return 0;
		} else
			return 1;
	case IPA_CMD_ADDRESS_CHANGE_NOTIF:
		qeth_bridge_host_event(card, cmd);
		return 0;
	default:
		return 1;
	}
}

struct qeth_discipline qeth_l2_discipline = {
	.devtype = &qeth_l2_devtype,
	.process_rx_buffer = qeth_l2_process_inbound_buffer,
	.recover = qeth_l2_recover,
	.setup = qeth_l2_probe_device,
	.remove = qeth_l2_remove_device,
	.set_online = qeth_l2_set_online,
	.set_offline = qeth_l2_set_offline,
	.freeze = qeth_l2_pm_suspend,
	.thaw = qeth_l2_pm_resume,
	.restore = qeth_l2_pm_resume,
	.do_ioctl = NULL,
	.control_event_handler = qeth_l2_control_event,
};
EXPORT_SYMBOL_GPL(qeth_l2_discipline);

static int qeth_osn_send_control_data(struct qeth_card *card, int len,
			   struct qeth_cmd_buffer *iob)
{
	struct qeth_channel *channel = iob->channel;
	unsigned long flags;
	int rc = 0;

	QETH_CARD_TEXT(card, 5, "osndctrd");

	wait_event(card->wait_q,
		   atomic_cmpxchg(&channel->irq_pending, 0, 1) == 0);
	qeth_prepare_control_data(card, len, iob);
	QETH_CARD_TEXT(card, 6, "osnoirqp");
	spin_lock_irqsave(get_ccwdev_lock(channel->ccwdev), flags);
	rc = ccw_device_start_timeout(channel->ccwdev, channel->ccw,
				      (addr_t) iob, 0, 0, QETH_IPA_TIMEOUT);
	spin_unlock_irqrestore(get_ccwdev_lock(channel->ccwdev), flags);
	if (rc) {
		QETH_DBF_MESSAGE(2, "qeth_osn_send_control_data: "
			   "ccw_device_start rc = %i\n", rc);
		QETH_CARD_TEXT_(card, 2, " err%d", rc);
		qeth_release_buffer(channel, iob);
		atomic_set(&channel->irq_pending, 0);
		wake_up(&card->wait_q);
	}
	return rc;
}

static int qeth_osn_send_ipa_cmd(struct qeth_card *card,
			struct qeth_cmd_buffer *iob, int data_len)
{
	u16 s1, s2;

	QETH_CARD_TEXT(card, 4, "osndipa");

	qeth_prepare_ipa_cmd(card, iob);
	s1 = (u16)(IPA_PDU_HEADER_SIZE + data_len);
	s2 = (u16)data_len;
	memcpy(QETH_IPA_PDU_LEN_TOTAL(iob->data), &s1, 2);
	memcpy(QETH_IPA_PDU_LEN_PDU1(iob->data), &s2, 2);
	memcpy(QETH_IPA_PDU_LEN_PDU2(iob->data), &s2, 2);
	memcpy(QETH_IPA_PDU_LEN_PDU3(iob->data), &s2, 2);
	return qeth_osn_send_control_data(card, s1, iob);
}

int qeth_osn_assist(struct net_device *dev, void *data, int data_len)
{
	struct qeth_cmd_buffer *iob;
	struct qeth_card *card;

	if (!dev)
		return -ENODEV;
	card = dev->ml_priv;
	if (!card)
		return -ENODEV;
	QETH_CARD_TEXT(card, 2, "osnsdmc");
	if (!qeth_card_hw_is_reachable(card))
		return -ENODEV;
	iob = qeth_wait_for_buffer(&card->write);
	memcpy(__ipa_cmd(iob), data, data_len);
	return qeth_osn_send_ipa_cmd(card, iob, data_len);
}
EXPORT_SYMBOL(qeth_osn_assist);

int qeth_osn_register(unsigned char *read_dev_no, struct net_device **dev,
		  int (*assist_cb)(struct net_device *, void *),
		  int (*data_cb)(struct sk_buff *))
{
	struct qeth_card *card;

	*dev = qeth_l2_netdev_by_devno(read_dev_no);
	if (*dev == NULL)
		return -ENODEV;
	card = (*dev)->ml_priv;
	if (!card)
		return -ENODEV;
	QETH_CARD_TEXT(card, 2, "osnreg");
	if ((assist_cb == NULL) || (data_cb == NULL))
		return -EINVAL;
	card->osn_info.assist_cb = assist_cb;
	card->osn_info.data_cb = data_cb;
	return 0;
}
EXPORT_SYMBOL(qeth_osn_register);

void qeth_osn_deregister(struct net_device *dev)
{
	struct qeth_card *card;

	if (!dev)
		return;
	card = dev->ml_priv;
	if (!card)
		return;
	QETH_CARD_TEXT(card, 2, "osndereg");
	card->osn_info.assist_cb = NULL;
	card->osn_info.data_cb = NULL;
	return;
}
EXPORT_SYMBOL(qeth_osn_deregister);

=======
static void qeth_l2_setup_bridgeport_attrs(struct qeth_card *card)
{
	if (!card->options.sbp.reflect_promisc &&
	    card->options.sbp.role != QETH_SBP_ROLE_NONE) {
		/* Conditional to avoid spurious error messages */
		qeth_bridgeport_setrole(card, card->options.sbp.role);
		/* Let the callback function refresh the stored role value. */
		qeth_bridgeport_query_ports(card, &card->options.sbp.role,
					    NULL);
	}
	if (card->options.sbp.hostnotification) {
		if (qeth_bridgeport_an_set(card, 1))
			card->options.sbp.hostnotification = 0;
	}
}

/**
 *	qeth_l2_detect_dev2br_support() -
 *	Detect whether this card supports 'dev to bridge fdb network address
 *	change notification' and thus can support the learning_sync bridgeport
 *	attribute
 *	@card: qeth_card structure pointer
 */
static void qeth_l2_detect_dev2br_support(struct qeth_card *card)
{
	struct qeth_priv *priv = netdev_priv(card->dev);
	bool dev2br_supported;

	QETH_CARD_TEXT(card, 2, "d2brsup");
	if (!IS_IQD(card))
		return;

	/* dev2br requires valid cssid,iid,chid */
	dev2br_supported = card->info.ids_valid &&
			   css_general_characteristics.enarf;
	QETH_CARD_TEXT_(card, 2, "D2Bsup%02x", dev2br_supported);

	if (dev2br_supported)
		priv->brport_hw_features |= BR_LEARNING_SYNC;
	else
		priv->brport_hw_features &= ~BR_LEARNING_SYNC;
}

static void qeth_l2_enable_brport_features(struct qeth_card *card)
{
	struct qeth_priv *priv = netdev_priv(card->dev);
	int rc;

	if (priv->brport_features & BR_LEARNING_SYNC) {
		if (priv->brport_hw_features & BR_LEARNING_SYNC) {
			qeth_l2_set_pnso_mode(card, QETH_PNSO_ADDR_INFO);
			rc = qeth_l2_dev2br_an_set(card, true);
			if (rc == -EAGAIN) {
				/* Recoverable error, retry once */
				qeth_l2_set_pnso_mode(card, QETH_PNSO_NONE);
				qeth_l2_dev2br_fdb_flush(card);
				qeth_l2_set_pnso_mode(card, QETH_PNSO_ADDR_INFO);
				rc = qeth_l2_dev2br_an_set(card, true);
			}
			if (rc) {
				netdev_err(card->dev,
					   "failed to enable bridge learning_sync: %d\n",
					   rc);
				qeth_l2_set_pnso_mode(card, QETH_PNSO_NONE);
				qeth_l2_dev2br_fdb_flush(card);
				priv->brport_features ^= BR_LEARNING_SYNC;
			}
		} else {
			dev_warn(&card->gdev->dev,
				"bridge learning_sync not supported\n");
			priv->brport_features ^= BR_LEARNING_SYNC;
		}
	}
}

>>>>>>> upstream/android-13
/* SETBRIDGEPORT support, async notifications */

enum qeth_an_event_type {anev_reg_unreg, anev_abort, anev_reset};

/**
 * qeth_bridge_emit_host_event() - bridgeport address change notification
 * @card:  qeth_card structure pointer, for udev events.
 * @evtype:  "normal" register/unregister, or abort, or reset. For abort
 *	      and reset token and addr_lnid are unused and may be NULL.
 * @code:  event bitmask: high order bit 0x80 value 1 means removal of an
 *			  object, 0 - addition of an object.
 *			  0x01 - VLAN, 0x02 - MAC, 0x03 - VLAN and MAC.
 * @token: "network token" structure identifying physical address of the port.
 * @addr_lnid: pointer to structure with MAC address and VLAN ID.
 *
 * This function is called when registrations and deregistrations are
 * reported by the hardware, and also when notifications are enabled -
 * for all currently registered addresses.
 */
static void qeth_bridge_emit_host_event(struct qeth_card *card,
<<<<<<< HEAD
	enum qeth_an_event_type evtype,
	u8 code, struct net_if_token *token, struct mac_addr_lnid *addr_lnid)
=======
					enum qeth_an_event_type evtype,
					u8 code,
					struct net_if_token *token,
					struct mac_addr_lnid *addr_lnid)
>>>>>>> upstream/android-13
{
	char str[7][32];
	char *env[8];
	int i = 0;

	switch (evtype) {
	case anev_reg_unreg:
		snprintf(str[i], sizeof(str[i]), "BRIDGEDHOST=%s",
				(code & IPA_ADDR_CHANGE_CODE_REMOVAL)
				? "deregister" : "register");
		env[i] = str[i]; i++;
		if (code & IPA_ADDR_CHANGE_CODE_VLANID) {
			snprintf(str[i], sizeof(str[i]), "VLAN=%d",
				addr_lnid->lnid);
			env[i] = str[i]; i++;
		}
		if (code & IPA_ADDR_CHANGE_CODE_MACADDR) {
			snprintf(str[i], sizeof(str[i]), "MAC=%pM",
				addr_lnid->mac);
			env[i] = str[i]; i++;
		}
		snprintf(str[i], sizeof(str[i]), "NTOK_BUSID=%x.%x.%04x",
			token->cssid, token->ssid, token->devnum);
		env[i] = str[i]; i++;
		snprintf(str[i], sizeof(str[i]), "NTOK_IID=%02x", token->iid);
		env[i] = str[i]; i++;
		snprintf(str[i], sizeof(str[i]), "NTOK_CHPID=%02x",
				token->chpid);
		env[i] = str[i]; i++;
		snprintf(str[i], sizeof(str[i]), "NTOK_CHID=%04x", token->chid);
		env[i] = str[i]; i++;
		break;
	case anev_abort:
		snprintf(str[i], sizeof(str[i]), "BRIDGEDHOST=abort");
		env[i] = str[i]; i++;
		break;
	case anev_reset:
		snprintf(str[i], sizeof(str[i]), "BRIDGEDHOST=reset");
		env[i] = str[i]; i++;
		break;
	}
	env[i] = NULL;
	kobject_uevent_env(&card->gdev->dev.kobj, KOBJ_CHANGE, env);
}

struct qeth_bridge_state_data {
	struct work_struct worker;
	struct qeth_card *card;
<<<<<<< HEAD
	struct qeth_sbp_state_change qports;
=======
	u8 role;
	u8 state;
>>>>>>> upstream/android-13
};

static void qeth_bridge_state_change_worker(struct work_struct *work)
{
	struct qeth_bridge_state_data *data =
		container_of(work, struct qeth_bridge_state_data, worker);
<<<<<<< HEAD
	/* We are only interested in the first entry - local port */
	struct qeth_sbp_port_entry *entry = &data->qports.entry[0];
=======
>>>>>>> upstream/android-13
	char env_locrem[32];
	char env_role[32];
	char env_state[32];
	char *env[] = {
		env_locrem,
		env_role,
		env_state,
		NULL
	};

<<<<<<< HEAD
	/* Role should not change by itself, but if it did, */
	/* information from the hardware is authoritative.  */
	mutex_lock(&data->card->conf_mutex);
	data->card->options.sbp.role = entry->role;
	mutex_unlock(&data->card->conf_mutex);

	snprintf(env_locrem, sizeof(env_locrem), "BRIDGEPORT=statechange");
	snprintf(env_role, sizeof(env_role), "ROLE=%s",
		(entry->role == QETH_SBP_ROLE_NONE) ? "none" :
		(entry->role == QETH_SBP_ROLE_PRIMARY) ? "primary" :
		(entry->role == QETH_SBP_ROLE_SECONDARY) ? "secondary" :
		"<INVALID>");
	snprintf(env_state, sizeof(env_state), "STATE=%s",
		(entry->state == QETH_SBP_STATE_INACTIVE) ? "inactive" :
		(entry->state == QETH_SBP_STATE_STANDBY) ? "standby" :
		(entry->state == QETH_SBP_STATE_ACTIVE) ? "active" :
=======
	snprintf(env_locrem, sizeof(env_locrem), "BRIDGEPORT=statechange");
	snprintf(env_role, sizeof(env_role), "ROLE=%s",
		(data->role == QETH_SBP_ROLE_NONE) ? "none" :
		(data->role == QETH_SBP_ROLE_PRIMARY) ? "primary" :
		(data->role == QETH_SBP_ROLE_SECONDARY) ? "secondary" :
		"<INVALID>");
	snprintf(env_state, sizeof(env_state), "STATE=%s",
		(data->state == QETH_SBP_STATE_INACTIVE) ? "inactive" :
		(data->state == QETH_SBP_STATE_STANDBY) ? "standby" :
		(data->state == QETH_SBP_STATE_ACTIVE) ? "active" :
>>>>>>> upstream/android-13
		"<INVALID>");
	kobject_uevent_env(&data->card->gdev->dev.kobj,
				KOBJ_CHANGE, env);
	kfree(data);
}

static void qeth_bridge_state_change(struct qeth_card *card,
					struct qeth_ipa_cmd *cmd)
{
<<<<<<< HEAD
	struct qeth_sbp_state_change *qports =
		 &cmd->data.sbp.data.state_change;
	struct qeth_bridge_state_data *data;
	int extrasize;
=======
	struct qeth_sbp_port_data *qports = &cmd->data.sbp.data.port_data;
	struct qeth_bridge_state_data *data;
>>>>>>> upstream/android-13

	QETH_CARD_TEXT(card, 2, "brstchng");
	if (qports->num_entries == 0) {
		QETH_CARD_TEXT(card, 2, "BPempty");
		return;
	}
	if (qports->entry_length != sizeof(struct qeth_sbp_port_entry)) {
		QETH_CARD_TEXT_(card, 2, "BPsz%04x", qports->entry_length);
		return;
	}
<<<<<<< HEAD
	extrasize = sizeof(struct qeth_sbp_port_entry) * qports->num_entries;
	data = kzalloc(sizeof(struct qeth_bridge_state_data) + extrasize,
		GFP_ATOMIC);
=======

	data = kzalloc(sizeof(*data), GFP_ATOMIC);
>>>>>>> upstream/android-13
	if (!data) {
		QETH_CARD_TEXT(card, 2, "BPSalloc");
		return;
	}
	INIT_WORK(&data->worker, qeth_bridge_state_change_worker);
	data->card = card;
<<<<<<< HEAD
	memcpy(&data->qports, qports,
			sizeof(struct qeth_sbp_state_change) + extrasize);
	queue_work(qeth_wq, &data->worker);
}

struct qeth_bridge_host_data {
	struct work_struct worker;
	struct qeth_card *card;
	struct qeth_ipacmd_addr_change hostevs;
};

static void qeth_bridge_host_event_worker(struct work_struct *work)
{
	struct qeth_bridge_host_data *data =
		container_of(work, struct qeth_bridge_host_data, worker);
	int i;

	if (data->hostevs.lost_event_mask) {
		dev_info(&data->card->gdev->dev,
"Address notification from the Bridge Port stopped %s (%s)\n",
			data->card->dev->name,
			(data->hostevs.lost_event_mask == 0x01)
			? "Overflow"
			: (data->hostevs.lost_event_mask == 0x02)
			? "Bridge port state change"
			: "Unknown reason");
		mutex_lock(&data->card->conf_mutex);
		data->card->options.sbp.hostnotification = 0;
		mutex_unlock(&data->card->conf_mutex);
		qeth_bridge_emit_host_event(data->card, anev_abort,
			0, NULL, NULL);
	} else
		for (i = 0; i < data->hostevs.num_entries; i++) {
			struct qeth_ipacmd_addr_change_entry *entry =
					&data->hostevs.entry[i];
			qeth_bridge_emit_host_event(data->card,
					anev_reg_unreg,
					entry->change_code,
					&entry->token, &entry->addr_lnid);
		}
	kfree(data);
}

static void qeth_bridge_host_event(struct qeth_card *card,
					struct qeth_ipa_cmd *cmd)
{
	struct qeth_ipacmd_addr_change *hostevs =
		 &cmd->data.addrchange;
	struct qeth_bridge_host_data *data;
	int extrasize;

	QETH_CARD_TEXT(card, 2, "brhostev");
=======
	/* Information for the local port: */
	data->role = qports->entry[0].role;
	data->state = qports->entry[0].state;

	queue_work(card->event_wq, &data->worker);
}

struct qeth_addr_change_data {
	struct delayed_work dwork;
	struct qeth_card *card;
	struct qeth_ipacmd_addr_change ac_event;
};

static void qeth_l2_dev2br_worker(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct qeth_addr_change_data *data;
	struct qeth_card *card;
	struct qeth_priv *priv;
	unsigned int i;
	int rc;

	data = container_of(dwork, struct qeth_addr_change_data, dwork);
	card = data->card;
	priv = netdev_priv(card->dev);

	QETH_CARD_TEXT(card, 4, "dev2brew");

	if (READ_ONCE(card->info.pnso_mode) == QETH_PNSO_NONE)
		goto free;

	if (data->ac_event.lost_event_mask) {
		/* Potential re-config in progress, try again later: */
		if (!rtnl_trylock()) {
			queue_delayed_work(card->event_wq, dwork,
					   msecs_to_jiffies(100));
			return;
		}

		if (!netif_device_present(card->dev)) {
			rtnl_unlock();
			goto free;
		}

		QETH_DBF_MESSAGE(3,
				 "Address change notification overflow on device %x\n",
				 CARD_DEVID(card));
		/* Card fdb and bridge fdb are out of sync, card has stopped
		 * notifications (no need to drain_workqueue). Purge all
		 * 'extern_learn' entries from the parent bridge and restart
		 * the notifications.
		 */
		qeth_l2_dev2br_fdb_flush(card);
		rc = qeth_l2_dev2br_an_set(card, true);
		if (rc) {
			/* TODO: if we want to retry after -EAGAIN, be
			 * aware there could be stale entries in the
			 * workqueue now, that need to be drained.
			 * For now we give up:
			 */
			netdev_err(card->dev,
				   "bridge learning_sync failed to recover: %d\n",
				   rc);
			WRITE_ONCE(card->info.pnso_mode,
				   QETH_PNSO_NONE);
			/* To remove fdb entries reported by an_set: */
			qeth_l2_dev2br_fdb_flush(card);
			priv->brport_features ^= BR_LEARNING_SYNC;
		} else {
			QETH_DBF_MESSAGE(3,
					 "Address Notification resynced on device %x\n",
					 CARD_DEVID(card));
		}

		rtnl_unlock();
	} else {
		for (i = 0; i < data->ac_event.num_entries; i++) {
			struct qeth_ipacmd_addr_change_entry *entry =
					&data->ac_event.entry[i];
			qeth_l2_dev2br_fdb_notify(card,
						  entry->change_code,
						  &entry->token,
						  &entry->addr_lnid);
		}
	}

free:
	kfree(data);
}

static void qeth_addr_change_event_worker(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct qeth_addr_change_data *data;
	struct qeth_card *card;
	int i;

	data = container_of(dwork, struct qeth_addr_change_data, dwork);
	card = data->card;

	QETH_CARD_TEXT(data->card, 4, "adrchgew");

	if (READ_ONCE(card->info.pnso_mode) == QETH_PNSO_NONE)
		goto free;

	if (data->ac_event.lost_event_mask) {
		/* Potential re-config in progress, try again later: */
		if (!mutex_trylock(&card->sbp_lock)) {
			queue_delayed_work(card->event_wq, dwork,
					   msecs_to_jiffies(100));
			return;
		}

		dev_info(&data->card->gdev->dev,
			 "Address change notification stopped on %s (%s)\n",
			 netdev_name(card->dev),
			(data->ac_event.lost_event_mask == 0x01)
			? "Overflow"
			: (data->ac_event.lost_event_mask == 0x02)
			? "Bridge port state change"
			: "Unknown reason");

		data->card->options.sbp.hostnotification = 0;
		card->info.pnso_mode = QETH_PNSO_NONE;
		mutex_unlock(&data->card->sbp_lock);
		qeth_bridge_emit_host_event(data->card, anev_abort,
					    0, NULL, NULL);
	} else
		for (i = 0; i < data->ac_event.num_entries; i++) {
			struct qeth_ipacmd_addr_change_entry *entry =
					&data->ac_event.entry[i];
			qeth_bridge_emit_host_event(data->card,
						    anev_reg_unreg,
						    entry->change_code,
						    &entry->token,
						    &entry->addr_lnid);
		}

free:
	kfree(data);
}

static void qeth_addr_change_event(struct qeth_card *card,
				   struct qeth_ipa_cmd *cmd)
{
	struct qeth_ipacmd_addr_change *hostevs =
		 &cmd->data.addrchange;
	struct qeth_addr_change_data *data;
	int extrasize;

	if (card->info.pnso_mode == QETH_PNSO_NONE)
		return;

	QETH_CARD_TEXT(card, 4, "adrchgev");
>>>>>>> upstream/android-13
	if (cmd->hdr.return_code != 0x0000) {
		if (cmd->hdr.return_code == 0x0010) {
			if (hostevs->lost_event_mask == 0x00)
				hostevs->lost_event_mask = 0xff;
		} else {
<<<<<<< HEAD
			QETH_CARD_TEXT_(card, 2, "BPHe%04x",
=======
			QETH_CARD_TEXT_(card, 2, "ACHN%04x",
>>>>>>> upstream/android-13
				cmd->hdr.return_code);
			return;
		}
	}
	extrasize = sizeof(struct qeth_ipacmd_addr_change_entry) *
						hostevs->num_entries;
<<<<<<< HEAD
	data = kzalloc(sizeof(struct qeth_bridge_host_data) + extrasize,
		GFP_ATOMIC);
	if (!data) {
		QETH_CARD_TEXT(card, 2, "BPHalloc");
		return;
	}
	INIT_WORK(&data->worker, qeth_bridge_host_event_worker);
	data->card = card;
	memcpy(&data->hostevs, hostevs,
			sizeof(struct qeth_ipacmd_addr_change) + extrasize);
	queue_work(qeth_wq, &data->worker);
=======
	data = kzalloc(sizeof(struct qeth_addr_change_data) + extrasize,
		       GFP_ATOMIC);
	if (!data) {
		QETH_CARD_TEXT(card, 2, "ACNalloc");
		return;
	}
	if (card->info.pnso_mode == QETH_PNSO_BRIDGEPORT)
		INIT_DELAYED_WORK(&data->dwork, qeth_addr_change_event_worker);
	else
		INIT_DELAYED_WORK(&data->dwork, qeth_l2_dev2br_worker);
	data->card = card;
	memcpy(&data->ac_event, hostevs,
			sizeof(struct qeth_ipacmd_addr_change) + extrasize);
	queue_delayed_work(card->event_wq, &data->dwork, 0);
>>>>>>> upstream/android-13
}

/* SETBRIDGEPORT support; sending commands */

struct _qeth_sbp_cbctl {
<<<<<<< HEAD
	u16 ipa_rc;
	u16 cmd_rc;
=======
>>>>>>> upstream/android-13
	union {
		u32 supported;
		struct {
			enum qeth_sbp_roles *role;
			enum qeth_sbp_states *state;
		} qports;
	} data;
};

<<<<<<< HEAD
/**
 * qeth_bridgeport_makerc() - derive "traditional" error from hardware codes.
 * @card:		      qeth_card structure pointer, for debug messages.
 * @cbctl:		      state structure with hardware return codes.
 * @setcmd:		      IPA command code
 *
 * Returns negative errno-compatible error indication or 0 on success.
 */
static int qeth_bridgeport_makerc(struct qeth_card *card,
	struct _qeth_sbp_cbctl *cbctl, enum qeth_ipa_sbp_cmd setcmd)
{
	int rc;
	int is_iqd = (card->info.type == QETH_CARD_TYPE_IQD);

	if ((is_iqd && (cbctl->ipa_rc == IPA_RC_SUCCESS)) ||
	    (!is_iqd && (cbctl->ipa_rc == cbctl->cmd_rc)))
		switch (cbctl->cmd_rc) {
=======
static int qeth_bridgeport_makerc(struct qeth_card *card,
				  struct qeth_ipa_cmd *cmd)
{
	struct qeth_ipacmd_setbridgeport *sbp = &cmd->data.sbp;
	enum qeth_ipa_sbp_cmd setcmd = sbp->hdr.command_code;
	u16 ipa_rc = cmd->hdr.return_code;
	u16 sbp_rc = sbp->hdr.return_code;
	int rc;

	if (ipa_rc == IPA_RC_SUCCESS && sbp_rc == IPA_RC_SUCCESS)
		return 0;

	if ((IS_IQD(card) && ipa_rc == IPA_RC_SUCCESS) ||
	    (!IS_IQD(card) && ipa_rc == sbp_rc)) {
		switch (sbp_rc) {
>>>>>>> upstream/android-13
		case IPA_RC_SUCCESS:
			rc = 0;
			break;
		case IPA_RC_L2_UNSUPPORTED_CMD:
		case IPA_RC_UNSUPPORTED_COMMAND:
			rc = -EOPNOTSUPP;
			break;
		case IPA_RC_SBP_OSA_NOT_CONFIGURED:
		case IPA_RC_SBP_IQD_NOT_CONFIGURED:
			rc = -ENODEV; /* maybe not the best code here? */
			dev_err(&card->gdev->dev,
	"The device is not configured as a Bridge Port\n");
			break;
		case IPA_RC_SBP_OSA_OS_MISMATCH:
		case IPA_RC_SBP_IQD_OS_MISMATCH:
			rc = -EPERM;
			dev_err(&card->gdev->dev,
	"A Bridge Port is already configured by a different operating system\n");
			break;
		case IPA_RC_SBP_OSA_ANO_DEV_PRIMARY:
		case IPA_RC_SBP_IQD_ANO_DEV_PRIMARY:
			switch (setcmd) {
			case IPA_SBP_SET_PRIMARY_BRIDGE_PORT:
				rc = -EEXIST;
				dev_err(&card->gdev->dev,
	"The LAN already has a primary Bridge Port\n");
				break;
			case IPA_SBP_SET_SECONDARY_BRIDGE_PORT:
				rc = -EBUSY;
				dev_err(&card->gdev->dev,
	"The device is already a primary Bridge Port\n");
				break;
			default:
				rc = -EIO;
			}
			break;
		case IPA_RC_SBP_OSA_CURRENT_SECOND:
		case IPA_RC_SBP_IQD_CURRENT_SECOND:
			rc = -EBUSY;
			dev_err(&card->gdev->dev,
	"The device is already a secondary Bridge Port\n");
			break;
		case IPA_RC_SBP_OSA_LIMIT_SECOND:
		case IPA_RC_SBP_IQD_LIMIT_SECOND:
			rc = -EEXIST;
			dev_err(&card->gdev->dev,
	"The LAN cannot have more secondary Bridge Ports\n");
			break;
		case IPA_RC_SBP_OSA_CURRENT_PRIMARY:
		case IPA_RC_SBP_IQD_CURRENT_PRIMARY:
			rc = -EBUSY;
			dev_err(&card->gdev->dev,
	"The device is already a primary Bridge Port\n");
			break;
		case IPA_RC_SBP_OSA_NOT_AUTHD_BY_ZMAN:
		case IPA_RC_SBP_IQD_NOT_AUTHD_BY_ZMAN:
			rc = -EACCES;
			dev_err(&card->gdev->dev,
	"The device is not authorized to be a Bridge Port\n");
			break;
		default:
			rc = -EIO;
		}
<<<<<<< HEAD
	else
		switch (cbctl->ipa_rc) {
=======
	} else {
		switch (ipa_rc) {
>>>>>>> upstream/android-13
		case IPA_RC_NOTSUPP:
			rc = -EOPNOTSUPP;
			break;
		case IPA_RC_UNSUPPORTED_COMMAND:
			rc = -EOPNOTSUPP;
			break;
		default:
			rc = -EIO;
		}
<<<<<<< HEAD

	if (rc) {
		QETH_CARD_TEXT_(card, 2, "SBPi%04x", cbctl->ipa_rc);
		QETH_CARD_TEXT_(card, 2, "SBPc%04x", cbctl->cmd_rc);
=======
	}

	if (rc) {
		QETH_CARD_TEXT_(card, 2, "SBPi%04x", ipa_rc);
		QETH_CARD_TEXT_(card, 2, "SBPc%04x", sbp_rc);
>>>>>>> upstream/android-13
	}
	return rc;
}

static struct qeth_cmd_buffer *qeth_sbp_build_cmd(struct qeth_card *card,
						  enum qeth_ipa_sbp_cmd sbp_cmd,
<<<<<<< HEAD
						  unsigned int cmd_length)
{
	enum qeth_ipa_cmds ipa_cmd = (card->info.type == QETH_CARD_TYPE_IQD) ?
					IPA_CMD_SETBRIDGEPORT_IQD :
					IPA_CMD_SETBRIDGEPORT_OSA;
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;

	iob = qeth_get_ipacmd_buffer(card, ipa_cmd, 0);
	if (!iob)
		return iob;
	cmd = __ipa_cmd(iob);
	cmd->data.sbp.hdr.cmdlength = sizeof(struct qeth_ipacmd_sbp_hdr) +
				      cmd_length;
	cmd->data.sbp.hdr.command_code = sbp_cmd;
	cmd->data.sbp.hdr.used_total = 1;
	cmd->data.sbp.hdr.seq_no = 1;
=======
						  unsigned int data_length)
{
	enum qeth_ipa_cmds ipa_cmd = IS_IQD(card) ? IPA_CMD_SETBRIDGEPORT_IQD :
						    IPA_CMD_SETBRIDGEPORT_OSA;
	struct qeth_ipacmd_sbp_hdr *hdr;
	struct qeth_cmd_buffer *iob;

	iob = qeth_ipa_alloc_cmd(card, ipa_cmd, QETH_PROT_NONE,
				 data_length +
				 offsetof(struct qeth_ipacmd_setbridgeport,
					  data));
	if (!iob)
		return iob;

	hdr = &__ipa_cmd(iob)->data.sbp.hdr;
	hdr->cmdlength = sizeof(*hdr) + data_length;
	hdr->command_code = sbp_cmd;
	hdr->used_total = 1;
	hdr->seq_no = 1;
>>>>>>> upstream/android-13
	return iob;
}

static int qeth_bridgeport_query_support_cb(struct qeth_card *card,
	struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	struct _qeth_sbp_cbctl *cbctl = (struct _qeth_sbp_cbctl *)reply->param;
<<<<<<< HEAD
	QETH_CARD_TEXT(card, 2, "brqsupcb");
	cbctl->ipa_rc = cmd->hdr.return_code;
	cbctl->cmd_rc = cmd->data.sbp.hdr.return_code;
	if ((cbctl->ipa_rc == 0) && (cbctl->cmd_rc == 0)) {
		cbctl->data.supported =
			cmd->data.sbp.data.query_cmds_supp.supported_cmds;
	} else {
		cbctl->data.supported = 0;
	}
=======
	int rc;

	QETH_CARD_TEXT(card, 2, "brqsupcb");
	rc = qeth_bridgeport_makerc(card, cmd);
	if (rc)
		return rc;

	cbctl->data.supported =
		cmd->data.sbp.data.query_cmds_supp.supported_cmds;
>>>>>>> upstream/android-13
	return 0;
}

/**
 * qeth_bridgeport_query_support() - store bitmask of supported subfunctions.
 * @card:			     qeth_card structure pointer.
 *
 * Sets bitmask of supported setbridgeport subfunctions in the qeth_card
 * strucutre: card->options.sbp.supported_funcs.
 */
static void qeth_bridgeport_query_support(struct qeth_card *card)
{
	struct qeth_cmd_buffer *iob;
	struct _qeth_sbp_cbctl cbctl;

	QETH_CARD_TEXT(card, 2, "brqsuppo");
	iob = qeth_sbp_build_cmd(card, IPA_SBP_QUERY_COMMANDS_SUPPORTED,
<<<<<<< HEAD
				 sizeof(struct qeth_sbp_query_cmds_supp));
	if (!iob)
		return;
	if (qeth_send_ipa_cmd(card, iob, qeth_bridgeport_query_support_cb,
							(void *)&cbctl) ||
	    qeth_bridgeport_makerc(card, &cbctl,
					IPA_SBP_QUERY_COMMANDS_SUPPORTED)) {
		/* non-zero makerc signifies failure, and produce messages */
		card->options.sbp.role = QETH_SBP_ROLE_NONE;
=======
				 SBP_DATA_SIZEOF(query_cmds_supp));
	if (!iob)
		return;

	if (qeth_send_ipa_cmd(card, iob, qeth_bridgeport_query_support_cb,
			      &cbctl)) {
		card->options.sbp.role = QETH_SBP_ROLE_NONE;
		card->options.sbp.supported_funcs = 0;
>>>>>>> upstream/android-13
		return;
	}
	card->options.sbp.supported_funcs = cbctl.data.supported;
}

static int qeth_bridgeport_query_ports_cb(struct qeth_card *card,
	struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
<<<<<<< HEAD
	struct qeth_sbp_query_ports *qports = &cmd->data.sbp.data.query_ports;
	struct _qeth_sbp_cbctl *cbctl = (struct _qeth_sbp_cbctl *)reply->param;

	QETH_CARD_TEXT(card, 2, "brqprtcb");
	cbctl->ipa_rc = cmd->hdr.return_code;
	cbctl->cmd_rc = cmd->data.sbp.hdr.return_code;
	if ((cbctl->ipa_rc != 0) || (cbctl->cmd_rc != 0))
		return 0;
	if (qports->entry_length != sizeof(struct qeth_sbp_port_entry)) {
		cbctl->cmd_rc = 0xffff;
		QETH_CARD_TEXT_(card, 2, "SBPs%04x", qports->entry_length);
		return 0;
=======
	struct _qeth_sbp_cbctl *cbctl = (struct _qeth_sbp_cbctl *)reply->param;
	struct qeth_sbp_port_data *qports;
	int rc;

	QETH_CARD_TEXT(card, 2, "brqprtcb");
	rc = qeth_bridgeport_makerc(card, cmd);
	if (rc)
		return rc;

	qports = &cmd->data.sbp.data.port_data;
	if (qports->entry_length != sizeof(struct qeth_sbp_port_entry)) {
		QETH_CARD_TEXT_(card, 2, "SBPs%04x", qports->entry_length);
		return -EINVAL;
>>>>>>> upstream/android-13
	}
	/* first entry contains the state of the local port */
	if (qports->num_entries > 0) {
		if (cbctl->data.qports.role)
			*cbctl->data.qports.role = qports->entry[0].role;
		if (cbctl->data.qports.state)
			*cbctl->data.qports.state = qports->entry[0].state;
	}
	return 0;
}

/**
 * qeth_bridgeport_query_ports() - query local bridgeport status.
 * @card:			   qeth_card structure pointer.
 * @role:   Role of the port: 0-none, 1-primary, 2-secondary.
 * @state:  State of the port: 0-inactive, 1-standby, 2-active.
 *
 * Returns negative errno-compatible error indication or 0 on success.
 *
 * 'role' and 'state' are not updated in case of hardware operation failure.
 */
int qeth_bridgeport_query_ports(struct qeth_card *card,
	enum qeth_sbp_roles *role, enum qeth_sbp_states *state)
{
<<<<<<< HEAD
	int rc = 0;
=======
>>>>>>> upstream/android-13
	struct qeth_cmd_buffer *iob;
	struct _qeth_sbp_cbctl cbctl = {
		.data = {
			.qports = {
				.role = role,
				.state = state,
			},
		},
	};

	QETH_CARD_TEXT(card, 2, "brqports");
	if (!(card->options.sbp.supported_funcs & IPA_SBP_QUERY_BRIDGE_PORTS))
		return -EOPNOTSUPP;
	iob = qeth_sbp_build_cmd(card, IPA_SBP_QUERY_BRIDGE_PORTS, 0);
	if (!iob)
		return -ENOMEM;
<<<<<<< HEAD
	rc = qeth_send_ipa_cmd(card, iob, qeth_bridgeport_query_ports_cb,
				(void *)&cbctl);
	if (rc < 0)
		return rc;
	return qeth_bridgeport_makerc(card, &cbctl, IPA_SBP_QUERY_BRIDGE_PORTS);
=======

	return qeth_send_ipa_cmd(card, iob, qeth_bridgeport_query_ports_cb,
				 &cbctl);
>>>>>>> upstream/android-13
}

static int qeth_bridgeport_set_cb(struct qeth_card *card,
	struct qeth_reply *reply, unsigned long data)
{
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *)data;
<<<<<<< HEAD
	struct _qeth_sbp_cbctl *cbctl = (struct _qeth_sbp_cbctl *)reply->param;
	QETH_CARD_TEXT(card, 2, "brsetrcb");
	cbctl->ipa_rc = cmd->hdr.return_code;
	cbctl->cmd_rc = cmd->data.sbp.hdr.return_code;
	return 0;
=======

	QETH_CARD_TEXT(card, 2, "brsetrcb");
	return qeth_bridgeport_makerc(card, cmd);
>>>>>>> upstream/android-13
}

/**
 * qeth_bridgeport_setrole() - Assign primary role to the port.
 * @card:		       qeth_card structure pointer.
 * @role:		       Role to assign.
 *
 * Returns negative errno-compatible error indication or 0 on success.
 */
int qeth_bridgeport_setrole(struct qeth_card *card, enum qeth_sbp_roles role)
{
<<<<<<< HEAD
	int rc = 0;
	int cmdlength;
	struct qeth_cmd_buffer *iob;
	struct _qeth_sbp_cbctl cbctl;
	enum qeth_ipa_sbp_cmd setcmd;
=======
	struct qeth_cmd_buffer *iob;
	enum qeth_ipa_sbp_cmd setcmd;
	unsigned int cmdlength = 0;
>>>>>>> upstream/android-13

	QETH_CARD_TEXT(card, 2, "brsetrol");
	switch (role) {
	case QETH_SBP_ROLE_NONE:
		setcmd = IPA_SBP_RESET_BRIDGE_PORT_ROLE;
<<<<<<< HEAD
		cmdlength = sizeof(struct qeth_sbp_reset_role);
		break;
	case QETH_SBP_ROLE_PRIMARY:
		setcmd = IPA_SBP_SET_PRIMARY_BRIDGE_PORT;
		cmdlength = sizeof(struct qeth_sbp_set_primary);
		break;
	case QETH_SBP_ROLE_SECONDARY:
		setcmd = IPA_SBP_SET_SECONDARY_BRIDGE_PORT;
		cmdlength = sizeof(struct qeth_sbp_set_secondary);
=======
		break;
	case QETH_SBP_ROLE_PRIMARY:
		setcmd = IPA_SBP_SET_PRIMARY_BRIDGE_PORT;
		cmdlength = SBP_DATA_SIZEOF(set_primary);
		break;
	case QETH_SBP_ROLE_SECONDARY:
		setcmd = IPA_SBP_SET_SECONDARY_BRIDGE_PORT;
>>>>>>> upstream/android-13
		break;
	default:
		return -EINVAL;
	}
	if (!(card->options.sbp.supported_funcs & setcmd))
		return -EOPNOTSUPP;
	iob = qeth_sbp_build_cmd(card, setcmd, cmdlength);
	if (!iob)
		return -ENOMEM;
<<<<<<< HEAD
	rc = qeth_send_ipa_cmd(card, iob, qeth_bridgeport_set_cb,
				(void *)&cbctl);
	if (rc < 0)
		return rc;
	return qeth_bridgeport_makerc(card, &cbctl, setcmd);
}

/**
 * qeth_anset_makerc() - derive "traditional" error from hardware codes.
 * @card:		      qeth_card structure pointer, for debug messages.
 *
 * Returns negative errno-compatible error indication or 0 on success.
 */
static int qeth_anset_makerc(struct qeth_card *card, int pnso_rc, u16 response)
{
	int rc;

	if (pnso_rc == 0)
		switch (response) {
		case 0x0001:
			rc = 0;
			break;
		case 0x0004:
		case 0x0100:
		case 0x0106:
			rc = -EOPNOTSUPP;
			dev_err(&card->gdev->dev,
				"Setting address notification failed\n");
			break;
		case 0x0107:
			rc = -EAGAIN;
			break;
		default:
			rc = -EIO;
		}
	else
		rc = -EIO;

	if (rc) {
		QETH_CARD_TEXT_(card, 2, "SBPp%04x", pnso_rc);
		QETH_CARD_TEXT_(card, 2, "SBPr%04x", response);
	}
	return rc;
}

static void qeth_bridgeport_an_set_cb(void *priv,
		enum qdio_brinfo_entry_type type, void *entry)
{
	struct qeth_card *card = (struct qeth_card *)priv;
	struct qdio_brinfo_entry_l2 *l2entry;
	u8 code;

	if (type != l2_addr_lnid) {
		WARN_ON_ONCE(1);
		return;
	}

	l2entry = (struct qdio_brinfo_entry_l2 *)entry;
	code = IPA_ADDR_CHANGE_CODE_MACADDR;
	if (l2entry->addr_lnid.lnid < VLAN_N_VID)
		code |= IPA_ADDR_CHANGE_CODE_VLANID;
	qeth_bridge_emit_host_event(card, anev_reg_unreg, code,
		(struct net_if_token *)&l2entry->nit,
		(struct mac_addr_lnid *)&l2entry->addr_lnid);
=======

	return qeth_send_ipa_cmd(card, iob, qeth_bridgeport_set_cb, NULL);
}

static void qeth_bridgeport_an_set_cb(void *priv,
				      struct chsc_pnso_naid_l2 *entry)
{
	struct qeth_card *card = (struct qeth_card *)priv;
	u8 code;

	code = IPA_ADDR_CHANGE_CODE_MACADDR;
	if (entry->addr_lnid.lnid < VLAN_N_VID)
		code |= IPA_ADDR_CHANGE_CODE_VLANID;
	qeth_bridge_emit_host_event(card, anev_reg_unreg, code,
				    (struct net_if_token *)&entry->nit,
				    (struct mac_addr_lnid *)&entry->addr_lnid);
>>>>>>> upstream/android-13
}

/**
 * qeth_bridgeport_an_set() - Enable or disable bridgeport address notification
 * @card:		      qeth_card structure pointer.
 * @enable:		      0 - disable, non-zero - enable notifications
 *
 * Returns negative errno-compatible error indication or 0 on success.
 *
 * On enable, emits a series of address notifications udev events for all
 * currently registered hosts.
 */
int qeth_bridgeport_an_set(struct qeth_card *card, int enable)
{
	int rc;
<<<<<<< HEAD
	u16 response;
	struct ccw_device *ddev;
	struct subchannel_id schid;

	if (!card)
		return -EINVAL;
	if (!card->options.sbp.supported_funcs)
		return -EOPNOTSUPP;
	ddev = CARD_DDEV(card);
	ccw_device_get_schid(ddev, &schid);

	if (enable) {
		qeth_bridge_emit_host_event(card, anev_reset, 0, NULL, NULL);
		rc = qdio_pnso_brinfo(schid, 1, &response,
			qeth_bridgeport_an_set_cb, card);
	} else
		rc = qdio_pnso_brinfo(schid, 0, &response, NULL, NULL);
	return qeth_anset_makerc(card, rc, response);
}

static bool qeth_bridgeport_is_in_use(struct qeth_card *card)
{
	return (card->options.sbp.role || card->options.sbp.reflect_promisc ||
		card->options.sbp.hostnotification);
=======

	if (!card->options.sbp.supported_funcs)
		return -EOPNOTSUPP;

	if (enable) {
		qeth_bridge_emit_host_event(card, anev_reset, 0, NULL, NULL);
		qeth_l2_set_pnso_mode(card, QETH_PNSO_BRIDGEPORT);
		rc = qeth_l2_pnso(card, PNSO_OC_NET_BRIDGE_INFO, 1,
				  qeth_bridgeport_an_set_cb, card);
		if (rc)
			qeth_l2_set_pnso_mode(card, QETH_PNSO_NONE);
	} else {
		rc = qeth_l2_pnso(card, PNSO_OC_NET_BRIDGE_INFO, 0, NULL, NULL);
		qeth_l2_set_pnso_mode(card, QETH_PNSO_NONE);
	}
	return rc;
>>>>>>> upstream/android-13
}

/* VNIC Characteristics support */

/* handle VNICC IPA command return codes; convert to error codes */
<<<<<<< HEAD
static int qeth_l2_vnicc_makerc(struct qeth_card *card, int ipa_rc)
=======
static int qeth_l2_vnicc_makerc(struct qeth_card *card, u16 ipa_rc)
>>>>>>> upstream/android-13
{
	int rc;

	switch (ipa_rc) {
	case IPA_RC_SUCCESS:
		return ipa_rc;
	case IPA_RC_L2_UNSUPPORTED_CMD:
	case IPA_RC_NOTSUPP:
		rc = -EOPNOTSUPP;
		break;
	case IPA_RC_VNICC_OOSEQ:
		rc = -EALREADY;
		break;
	case IPA_RC_VNICC_VNICBP:
		rc = -EBUSY;
		break;
	case IPA_RC_L2_ADDR_TABLE_FULL:
		rc = -ENOSPC;
		break;
	case IPA_RC_L2_MAC_NOT_AUTH_BY_ADP:
		rc = -EACCES;
		break;
	default:
		rc = -EIO;
	}

	QETH_CARD_TEXT_(card, 2, "err%04x", ipa_rc);
	return rc;
}

<<<<<<< HEAD
/* generic VNICC request call back control */
struct _qeth_l2_vnicc_request_cbctl {
	u32 sub_cmd;
	struct {
		u32 vnic_char;
		u32 timeout;
	} param;
	struct {
		union{
			u32 *sup_cmds;
			u32 *timeout;
		};
	} result;
};

=======
>>>>>>> upstream/android-13
/* generic VNICC request call back */
static int qeth_l2_vnicc_request_cb(struct qeth_card *card,
				    struct qeth_reply *reply,
				    unsigned long data)
{
<<<<<<< HEAD
	struct _qeth_l2_vnicc_request_cbctl *cbctl =
		(struct _qeth_l2_vnicc_request_cbctl *) reply->param;
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	struct qeth_ipacmd_vnicc *rep = &cmd->data.vnicc;

	QETH_CARD_TEXT(card, 2, "vniccrcb");
	if (cmd->hdr.return_code)
		return 0;
	/* return results to caller */
	card->options.vnicc.sup_chars = rep->hdr.sup;
	card->options.vnicc.cur_chars = rep->hdr.cur;

	if (cbctl->sub_cmd == IPA_VNICC_QUERY_CMDS)
		*cbctl->result.sup_cmds = rep->query_cmds.sup_cmds;

	if (cbctl->sub_cmd == IPA_VNICC_GET_TIMEOUT)
		*cbctl->result.timeout = rep->getset_timeout.timeout;
=======
	struct qeth_ipa_cmd *cmd = (struct qeth_ipa_cmd *) data;
	struct qeth_ipacmd_vnicc *rep = &cmd->data.vnicc;
	u32 sub_cmd = cmd->data.vnicc.hdr.sub_command;

	QETH_CARD_TEXT(card, 2, "vniccrcb");
	if (cmd->hdr.return_code)
		return qeth_l2_vnicc_makerc(card, cmd->hdr.return_code);
	/* return results to caller */
	card->options.vnicc.sup_chars = rep->vnicc_cmds.supported;
	card->options.vnicc.cur_chars = rep->vnicc_cmds.enabled;

	if (sub_cmd == IPA_VNICC_QUERY_CMDS)
		*(u32 *)reply->param = rep->data.query_cmds.sup_cmds;
	else if (sub_cmd == IPA_VNICC_GET_TIMEOUT)
		*(u32 *)reply->param = rep->data.getset_timeout.timeout;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/* generic VNICC request */
static int qeth_l2_vnicc_request(struct qeth_card *card,
				 struct _qeth_l2_vnicc_request_cbctl *cbctl)
{
	struct qeth_ipacmd_vnicc *req;
	struct qeth_cmd_buffer *iob;
	struct qeth_ipa_cmd *cmd;
	int rc;

	QETH_CARD_TEXT(card, 2, "vniccreq");

	/* get new buffer for request */
	iob = qeth_get_ipacmd_buffer(card, IPA_CMD_VNICC, 0);
	if (!iob)
		return -ENOMEM;

	/* create header for request */
	cmd = __ipa_cmd(iob);
	req = &cmd->data.vnicc;

	/* create sub command header for request */
	req->sub_hdr.data_length = sizeof(req->sub_hdr);
	req->sub_hdr.sub_command = cbctl->sub_cmd;

	/* create sub command specific request fields */
	switch (cbctl->sub_cmd) {
	case IPA_VNICC_QUERY_CHARS:
		break;
	case IPA_VNICC_QUERY_CMDS:
		req->sub_hdr.data_length += sizeof(req->query_cmds);
		req->query_cmds.vnic_char = cbctl->param.vnic_char;
		break;
	case IPA_VNICC_ENABLE:
	case IPA_VNICC_DISABLE:
		req->sub_hdr.data_length += sizeof(req->set_char);
		req->set_char.vnic_char = cbctl->param.vnic_char;
		break;
	case IPA_VNICC_SET_TIMEOUT:
		req->getset_timeout.timeout = cbctl->param.timeout;
		/* fallthrough */
	case IPA_VNICC_GET_TIMEOUT:
		req->sub_hdr.data_length += sizeof(req->getset_timeout);
		req->getset_timeout.vnic_char = cbctl->param.vnic_char;
		break;
	default:
		qeth_release_buffer(iob->channel, iob);
		return -EOPNOTSUPP;
	}

	/* send request */
	rc = qeth_send_ipa_cmd(card, iob, qeth_l2_vnicc_request_cb,
			       (void *) cbctl);

	return qeth_l2_vnicc_makerc(card, rc);
=======
static struct qeth_cmd_buffer *qeth_l2_vnicc_build_cmd(struct qeth_card *card,
						       u32 vnicc_cmd,
						       unsigned int data_length)
{
	struct qeth_ipacmd_vnicc_hdr *hdr;
	struct qeth_cmd_buffer *iob;

	iob = qeth_ipa_alloc_cmd(card, IPA_CMD_VNICC, QETH_PROT_NONE,
				 data_length +
				 offsetof(struct qeth_ipacmd_vnicc, data));
	if (!iob)
		return NULL;

	hdr = &__ipa_cmd(iob)->data.vnicc.hdr;
	hdr->data_length = sizeof(*hdr) + data_length;
	hdr->sub_command = vnicc_cmd;
	return iob;
>>>>>>> upstream/android-13
}

/* VNICC query VNIC characteristics request */
static int qeth_l2_vnicc_query_chars(struct qeth_card *card)
{
<<<<<<< HEAD
	struct _qeth_l2_vnicc_request_cbctl cbctl;

	/* prepare callback control */
	cbctl.sub_cmd = IPA_VNICC_QUERY_CHARS;

	QETH_CARD_TEXT(card, 2, "vniccqch");
	return qeth_l2_vnicc_request(card, &cbctl);
=======
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "vniccqch");
	iob = qeth_l2_vnicc_build_cmd(card, IPA_VNICC_QUERY_CHARS, 0);
	if (!iob)
		return -ENOMEM;

	return qeth_send_ipa_cmd(card, iob, qeth_l2_vnicc_request_cb, NULL);
>>>>>>> upstream/android-13
}

/* VNICC query sub commands request */
static int qeth_l2_vnicc_query_cmds(struct qeth_card *card, u32 vnic_char,
				    u32 *sup_cmds)
{
<<<<<<< HEAD
	struct _qeth_l2_vnicc_request_cbctl cbctl;

	/* prepare callback control */
	cbctl.sub_cmd = IPA_VNICC_QUERY_CMDS;
	cbctl.param.vnic_char = vnic_char;
	cbctl.result.sup_cmds = sup_cmds;

	QETH_CARD_TEXT(card, 2, "vniccqcm");
	return qeth_l2_vnicc_request(card, &cbctl);
=======
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "vniccqcm");
	iob = qeth_l2_vnicc_build_cmd(card, IPA_VNICC_QUERY_CMDS,
				      VNICC_DATA_SIZEOF(query_cmds));
	if (!iob)
		return -ENOMEM;

	__ipa_cmd(iob)->data.vnicc.data.query_cmds.vnic_char = vnic_char;

	return qeth_send_ipa_cmd(card, iob, qeth_l2_vnicc_request_cb, sup_cmds);
>>>>>>> upstream/android-13
}

/* VNICC enable/disable characteristic request */
static int qeth_l2_vnicc_set_char(struct qeth_card *card, u32 vnic_char,
				      u32 cmd)
{
<<<<<<< HEAD
	struct _qeth_l2_vnicc_request_cbctl cbctl;

	/* prepare callback control */
	cbctl.sub_cmd = cmd;
	cbctl.param.vnic_char = vnic_char;

	QETH_CARD_TEXT(card, 2, "vniccedc");
	return qeth_l2_vnicc_request(card, &cbctl);
=======
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "vniccedc");
	iob = qeth_l2_vnicc_build_cmd(card, cmd, VNICC_DATA_SIZEOF(set_char));
	if (!iob)
		return -ENOMEM;

	__ipa_cmd(iob)->data.vnicc.data.set_char.vnic_char = vnic_char;

	return qeth_send_ipa_cmd(card, iob, qeth_l2_vnicc_request_cb, NULL);
>>>>>>> upstream/android-13
}

/* VNICC get/set timeout for characteristic request */
static int qeth_l2_vnicc_getset_timeout(struct qeth_card *card, u32 vnicc,
					u32 cmd, u32 *timeout)
{
<<<<<<< HEAD
	struct _qeth_l2_vnicc_request_cbctl cbctl;

	/* prepare callback control */
	cbctl.sub_cmd = cmd;
	cbctl.param.vnic_char = vnicc;
	if (cmd == IPA_VNICC_SET_TIMEOUT)
		cbctl.param.timeout = *timeout;
	if (cmd == IPA_VNICC_GET_TIMEOUT)
		cbctl.result.timeout = timeout;

	QETH_CARD_TEXT(card, 2, "vniccgst");
	return qeth_l2_vnicc_request(card, &cbctl);
=======
	struct qeth_vnicc_getset_timeout *getset_timeout;
	struct qeth_cmd_buffer *iob;

	QETH_CARD_TEXT(card, 2, "vniccgst");
	iob = qeth_l2_vnicc_build_cmd(card, cmd,
				      VNICC_DATA_SIZEOF(getset_timeout));
	if (!iob)
		return -ENOMEM;

	getset_timeout = &__ipa_cmd(iob)->data.vnicc.data.getset_timeout;
	getset_timeout->vnic_char = vnicc;

	if (cmd == IPA_VNICC_SET_TIMEOUT)
		getset_timeout->timeout = *timeout;

	return qeth_send_ipa_cmd(card, iob, qeth_l2_vnicc_request_cb, timeout);
}

/* recover user timeout setting */
static bool qeth_l2_vnicc_recover_timeout(struct qeth_card *card, u32 vnicc,
					  u32 *timeout)
{
	if (card->options.vnicc.sup_chars & vnicc &&
	    card->options.vnicc.getset_timeout_sup & vnicc &&
	    !qeth_l2_vnicc_getset_timeout(card, vnicc, IPA_VNICC_SET_TIMEOUT,
					  timeout))
		return false;
	*timeout = QETH_VNICC_DEFAULT_TIMEOUT;
	return true;
>>>>>>> upstream/android-13
}

/* set current VNICC flag state; called from sysfs store function */
int qeth_l2_vnicc_set_state(struct qeth_card *card, u32 vnicc, bool state)
{
	int rc = 0;
	u32 cmd;

	QETH_CARD_TEXT(card, 2, "vniccsch");

	/* check if characteristic and enable/disable are supported */
	if (!(card->options.vnicc.sup_chars & vnicc) ||
	    !(card->options.vnicc.set_char_sup & vnicc))
		return -EOPNOTSUPP;

	if (qeth_bridgeport_is_in_use(card))
		return -EBUSY;

	/* set enable/disable command and store wanted characteristic */
	if (state) {
		cmd = IPA_VNICC_ENABLE;
		card->options.vnicc.wanted_chars |= vnicc;
	} else {
		cmd = IPA_VNICC_DISABLE;
		card->options.vnicc.wanted_chars &= ~vnicc;
	}

	/* do we need to do anything? */
	if (card->options.vnicc.cur_chars == card->options.vnicc.wanted_chars)
		return rc;

	/* if card is not ready, simply stop here */
	if (!qeth_card_hw_is_reachable(card)) {
		if (state)
			card->options.vnicc.cur_chars |= vnicc;
		else
			card->options.vnicc.cur_chars &= ~vnicc;
		return rc;
	}

	rc = qeth_l2_vnicc_set_char(card, vnicc, cmd);
	if (rc)
		card->options.vnicc.wanted_chars =
			card->options.vnicc.cur_chars;
	else {
		/* successful online VNICC change; handle special cases */
		if (state && vnicc == QETH_VNICC_RX_BCAST)
			card->options.vnicc.rx_bcast_enabled = true;
		if (!state && vnicc == QETH_VNICC_LEARNING)
			qeth_l2_vnicc_recover_timeout(card, vnicc,
					&card->options.vnicc.learning_timeout);
	}

	return rc;
}

/* get current VNICC flag state; called from sysfs show function */
int qeth_l2_vnicc_get_state(struct qeth_card *card, u32 vnicc, bool *state)
{
	int rc = 0;

	QETH_CARD_TEXT(card, 2, "vniccgch");

	/* check if characteristic is supported */
	if (!(card->options.vnicc.sup_chars & vnicc))
		return -EOPNOTSUPP;

	if (qeth_bridgeport_is_in_use(card))
		return -EBUSY;

	/* if card is ready, query current VNICC state */
	if (qeth_card_hw_is_reachable(card))
		rc = qeth_l2_vnicc_query_chars(card);

	*state = (card->options.vnicc.cur_chars & vnicc) ? true : false;
	return rc;
}

/* set VNICC timeout; called from sysfs store function. Currently, only learning
 * supports timeout
 */
int qeth_l2_vnicc_set_timeout(struct qeth_card *card, u32 timeout)
{
	int rc = 0;

	QETH_CARD_TEXT(card, 2, "vniccsto");

	/* check if characteristic and set_timeout are supported */
	if (!(card->options.vnicc.sup_chars & QETH_VNICC_LEARNING) ||
	    !(card->options.vnicc.getset_timeout_sup & QETH_VNICC_LEARNING))
		return -EOPNOTSUPP;

	if (qeth_bridgeport_is_in_use(card))
		return -EBUSY;

	/* do we need to do anything? */
	if (card->options.vnicc.learning_timeout == timeout)
		return rc;

	/* if card is not ready, simply store the value internally and return */
	if (!qeth_card_hw_is_reachable(card)) {
		card->options.vnicc.learning_timeout = timeout;
		return rc;
	}

	/* send timeout value to card; if successful, store value internally */
	rc = qeth_l2_vnicc_getset_timeout(card, QETH_VNICC_LEARNING,
					  IPA_VNICC_SET_TIMEOUT, &timeout);
	if (!rc)
		card->options.vnicc.learning_timeout = timeout;

	return rc;
}

/* get current VNICC timeout; called from sysfs show function. Currently, only
 * learning supports timeout
 */
int qeth_l2_vnicc_get_timeout(struct qeth_card *card, u32 *timeout)
{
	int rc = 0;

	QETH_CARD_TEXT(card, 2, "vniccgto");

	/* check if characteristic and get_timeout are supported */
	if (!(card->options.vnicc.sup_chars & QETH_VNICC_LEARNING) ||
	    !(card->options.vnicc.getset_timeout_sup & QETH_VNICC_LEARNING))
		return -EOPNOTSUPP;

	if (qeth_bridgeport_is_in_use(card))
		return -EBUSY;

	/* if card is ready, get timeout. Otherwise, just return stored value */
	*timeout = card->options.vnicc.learning_timeout;
	if (qeth_card_hw_is_reachable(card))
		rc = qeth_l2_vnicc_getset_timeout(card, QETH_VNICC_LEARNING,
						  IPA_VNICC_GET_TIMEOUT,
						  timeout);

	return rc;
}

/* check if VNICC is currently enabled */
<<<<<<< HEAD
bool qeth_l2_vnicc_is_in_use(struct qeth_card *card)
=======
static bool _qeth_l2_vnicc_is_in_use(struct qeth_card *card)
>>>>>>> upstream/android-13
{
	if (!card->options.vnicc.sup_chars)
		return false;
	/* default values are only OK if rx_bcast was not enabled by user
	 * or the card is offline.
	 */
	if (card->options.vnicc.cur_chars == QETH_VNICC_DEFAULT) {
		if (!card->options.vnicc.rx_bcast_enabled ||
		    !qeth_card_hw_is_reachable(card))
			return false;
	}
	return true;
}

<<<<<<< HEAD
/* recover user timeout setting */
static bool qeth_l2_vnicc_recover_timeout(struct qeth_card *card, u32 vnicc,
					  u32 *timeout)
{
	if (card->options.vnicc.sup_chars & vnicc &&
	    card->options.vnicc.getset_timeout_sup & vnicc &&
	    !qeth_l2_vnicc_getset_timeout(card, vnicc, IPA_VNICC_SET_TIMEOUT,
					  timeout))
		return false;
	*timeout = QETH_VNICC_DEFAULT_TIMEOUT;
	return true;
=======
/**
 *	qeth_bridgeport_allowed - are any qeth_bridgeport functions allowed?
 *	@card: qeth_card structure pointer
 *
 *	qeth_bridgeport functionality is mutually exclusive with usage of the
 *	VNIC Characteristics and dev2br address notifications
 */
bool qeth_bridgeport_allowed(struct qeth_card *card)
{
	struct qeth_priv *priv = netdev_priv(card->dev);

	return (!_qeth_l2_vnicc_is_in_use(card) &&
		!(priv->brport_features & BR_LEARNING_SYNC));
>>>>>>> upstream/android-13
}

/* recover user characteristic setting */
static bool qeth_l2_vnicc_recover_char(struct qeth_card *card, u32 vnicc,
				       bool enable)
{
	u32 cmd = enable ? IPA_VNICC_ENABLE : IPA_VNICC_DISABLE;

	if (card->options.vnicc.sup_chars & vnicc &&
	    card->options.vnicc.set_char_sup & vnicc &&
	    !qeth_l2_vnicc_set_char(card, vnicc, cmd))
		return false;
	card->options.vnicc.wanted_chars &= ~vnicc;
	card->options.vnicc.wanted_chars |= QETH_VNICC_DEFAULT & vnicc;
	return true;
}

/* (re-)initialize VNICC */
static void qeth_l2_vnicc_init(struct qeth_card *card)
{
	u32 *timeout = &card->options.vnicc.learning_timeout;
	bool enable, error = false;
	unsigned int chars_len, i;
	unsigned long chars_tmp;
	u32 sup_cmds, vnicc;

	QETH_CARD_TEXT(card, 2, "vniccini");
	/* reset rx_bcast */
	card->options.vnicc.rx_bcast_enabled = 0;
	/* initial query and storage of VNIC characteristics */
	if (qeth_l2_vnicc_query_chars(card)) {
		if (card->options.vnicc.wanted_chars != QETH_VNICC_DEFAULT ||
		    *timeout != QETH_VNICC_DEFAULT_TIMEOUT)
			dev_err(&card->gdev->dev, "Configuring the VNIC characteristics failed\n");
		/* fail quietly if user didn't change the default config */
		card->options.vnicc.sup_chars = 0;
		card->options.vnicc.cur_chars = 0;
		card->options.vnicc.wanted_chars = QETH_VNICC_DEFAULT;
		return;
	}
	/* get supported commands for each supported characteristic */
	chars_tmp = card->options.vnicc.sup_chars;
	chars_len = sizeof(card->options.vnicc.sup_chars) * BITS_PER_BYTE;
	for_each_set_bit(i, &chars_tmp, chars_len) {
		vnicc = BIT(i);
		if (qeth_l2_vnicc_query_cmds(card, vnicc, &sup_cmds)) {
			sup_cmds = 0;
			error = true;
		}
		if ((sup_cmds & IPA_VNICC_SET_TIMEOUT) &&
		    (sup_cmds & IPA_VNICC_GET_TIMEOUT))
			card->options.vnicc.getset_timeout_sup |= vnicc;
		else
			card->options.vnicc.getset_timeout_sup &= ~vnicc;
		if ((sup_cmds & IPA_VNICC_ENABLE) &&
		    (sup_cmds & IPA_VNICC_DISABLE))
			card->options.vnicc.set_char_sup |= vnicc;
		else
			card->options.vnicc.set_char_sup &= ~vnicc;
	}
	/* enforce assumed default values and recover settings, if changed  */
	error |= qeth_l2_vnicc_recover_timeout(card, QETH_VNICC_LEARNING,
					       timeout);
<<<<<<< HEAD
	chars_tmp = card->options.vnicc.wanted_chars ^ QETH_VNICC_DEFAULT;
=======
	/* Change chars, if necessary  */
	chars_tmp = card->options.vnicc.wanted_chars ^
		    card->options.vnicc.cur_chars;
>>>>>>> upstream/android-13
	chars_len = sizeof(card->options.vnicc.wanted_chars) * BITS_PER_BYTE;
	for_each_set_bit(i, &chars_tmp, chars_len) {
		vnicc = BIT(i);
		enable = card->options.vnicc.wanted_chars & vnicc;
		error |= qeth_l2_vnicc_recover_char(card, vnicc, enable);
	}
	if (error)
		dev_err(&card->gdev->dev, "Configuring the VNIC characteristics failed\n");
}

/* configure default values of VNIC characteristics */
static void qeth_l2_vnicc_set_defaults(struct qeth_card *card)
{
	/* characteristics values */
	card->options.vnicc.sup_chars = QETH_VNICC_ALL;
	card->options.vnicc.cur_chars = QETH_VNICC_DEFAULT;
	card->options.vnicc.learning_timeout = QETH_VNICC_DEFAULT_TIMEOUT;
	/* supported commands */
	card->options.vnicc.set_char_sup = QETH_VNICC_ALL;
	card->options.vnicc.getset_timeout_sup = QETH_VNICC_LEARNING;
	/* settings wanted by users */
	card->options.vnicc.wanted_chars = QETH_VNICC_DEFAULT;
}

<<<<<<< HEAD
=======
static const struct device_type qeth_l2_devtype = {
	.name = "qeth_layer2",
	.groups = qeth_l2_attr_groups,
};

static int qeth_l2_probe_device(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	int rc;

	qeth_l2_vnicc_set_defaults(card);
	mutex_init(&card->sbp_lock);

	if (gdev->dev.type) {
		rc = device_add_groups(&gdev->dev, qeth_l2_attr_groups);
		if (rc)
			return rc;
	} else {
		gdev->dev.type = &qeth_l2_devtype;
	}

	INIT_WORK(&card->rx_mode_work, qeth_l2_rx_mode_work);
	return 0;
}

static void qeth_l2_remove_device(struct ccwgroup_device *gdev)
{
	struct qeth_card *card = dev_get_drvdata(&gdev->dev);
	struct qeth_priv *priv;

	if (gdev->dev.type != &qeth_l2_devtype)
		device_remove_groups(&gdev->dev, qeth_l2_attr_groups);

	qeth_set_allowed_threads(card, 0, 1);
	wait_event(card->wait_q, qeth_threads_running(card, 0xffffffff) == 0);

	if (gdev->state == CCWGROUP_ONLINE)
		qeth_set_offline(card, card->discipline, false);

	if (card->dev->reg_state == NETREG_REGISTERED) {
		priv = netdev_priv(card->dev);
		if (priv->brport_features & BR_LEARNING_SYNC) {
			rtnl_lock();
			qeth_l2_br2dev_put();
			rtnl_unlock();
		}
		unregister_netdev(card->dev);
	}
}

static int qeth_l2_set_online(struct qeth_card *card, bool carrier_ok)
{
	struct net_device *dev = card->dev;
	int rc = 0;

	qeth_l2_detect_dev2br_support(card);

	mutex_lock(&card->sbp_lock);
	qeth_bridgeport_query_support(card);
	if (card->options.sbp.supported_funcs) {
		qeth_l2_setup_bridgeport_attrs(card);
		dev_info(&card->gdev->dev,
			 "The device represents a Bridge Capable Port\n");
	}
	mutex_unlock(&card->sbp_lock);

	qeth_l2_register_dev_addr(card);

	/* for the rx_bcast characteristic, init VNICC after setmac */
	qeth_l2_vnicc_init(card);

	qeth_l2_trace_features(card);

	/* softsetup */
	QETH_CARD_TEXT(card, 2, "softsetp");

	card->state = CARD_STATE_SOFTSETUP;

	qeth_set_allowed_threads(card, 0xffffffff, 0);

	if (dev->reg_state != NETREG_REGISTERED) {
		rc = qeth_l2_setup_netdev(card);
		if (rc)
			goto err_setup;

		if (carrier_ok)
			netif_carrier_on(dev);
	} else {
		rtnl_lock();
		rc = qeth_set_real_num_tx_queues(card,
						 qeth_tx_actual_queues(card));
		if (rc) {
			rtnl_unlock();
			goto err_set_queues;
		}

		if (carrier_ok)
			netif_carrier_on(dev);
		else
			netif_carrier_off(dev);

		netif_device_attach(dev);
		qeth_enable_hw_features(dev);
		qeth_l2_enable_brport_features(card);

		if (card->info.open_when_online) {
			card->info.open_when_online = 0;
			dev_open(dev, NULL);
		}
		rtnl_unlock();
	}
	return 0;

err_set_queues:
err_setup:
	qeth_set_allowed_threads(card, 0, 1);
	card->state = CARD_STATE_DOWN;
	return rc;
}

static void qeth_l2_set_offline(struct qeth_card *card)
{
	struct qeth_priv *priv = netdev_priv(card->dev);

	qeth_set_allowed_threads(card, 0, 1);
	qeth_l2_drain_rx_mode_cache(card);

	if (card->state == CARD_STATE_SOFTSETUP)
		card->state = CARD_STATE_DOWN;

	qeth_l2_set_pnso_mode(card, QETH_PNSO_NONE);
	if (priv->brport_features & BR_LEARNING_SYNC)
		qeth_l2_dev2br_fdb_flush(card);
}

/* Returns zero if the command is successfully "consumed" */
static int qeth_l2_control_event(struct qeth_card *card,
				 struct qeth_ipa_cmd *cmd)
{
	switch (cmd->hdr.command) {
	case IPA_CMD_SETBRIDGEPORT_OSA:
	case IPA_CMD_SETBRIDGEPORT_IQD:
		if (cmd->data.sbp.hdr.command_code ==
		    IPA_SBP_BRIDGE_PORT_STATE_CHANGE) {
			qeth_bridge_state_change(card, cmd);
			return 0;
		}

		return 1;
	case IPA_CMD_ADDRESS_CHANGE_NOTIF:
		qeth_addr_change_event(card, cmd);
		return 0;
	default:
		return 1;
	}
}

const struct qeth_discipline qeth_l2_discipline = {
	.setup = qeth_l2_probe_device,
	.remove = qeth_l2_remove_device,
	.set_online = qeth_l2_set_online,
	.set_offline = qeth_l2_set_offline,
	.do_ioctl = NULL,
	.control_event_handler = qeth_l2_control_event,
};
EXPORT_SYMBOL_GPL(qeth_l2_discipline);

static int __init qeth_l2_init(void)
{
	pr_info("register layer 2 discipline\n");
	refcount_set(&qeth_l2_switchdev_notify_refcnt, 0);
	return 0;
}

static void __exit qeth_l2_exit(void)
{
	pr_info("unregister layer 2 discipline\n");
}

>>>>>>> upstream/android-13
module_init(qeth_l2_init);
module_exit(qeth_l2_exit);
MODULE_AUTHOR("Frank Blaschka <frank.blaschka@de.ibm.com>");
MODULE_DESCRIPTION("qeth layer 2 discipline");
MODULE_LICENSE("GPL");
