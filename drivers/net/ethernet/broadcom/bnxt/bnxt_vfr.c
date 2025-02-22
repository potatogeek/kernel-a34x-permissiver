/* Broadcom NetXtreme-C/E network driver.
 *
 * Copyright (c) 2016-2017 Broadcom Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <linux/jhash.h>
#include <net/pkt_cls.h>

#include "bnxt_hsi.h"
#include "bnxt.h"
<<<<<<< HEAD
=======
#include "bnxt_hwrm.h"
>>>>>>> upstream/android-13
#include "bnxt_vfr.h"
#include "bnxt_devlink.h"
#include "bnxt_tc.h"

#ifdef CONFIG_BNXT_SRIOV

#define CFA_HANDLE_INVALID		0xffff
#define VF_IDX_INVALID			0xffff

static int hwrm_cfa_vfr_alloc(struct bnxt *bp, u16 vf_idx,
			      u16 *tx_cfa_action, u16 *rx_cfa_code)
{
<<<<<<< HEAD
	struct hwrm_cfa_vfr_alloc_output *resp = bp->hwrm_cmd_resp_addr;
	struct hwrm_cfa_vfr_alloc_input req = { 0 };
	int rc;

	bnxt_hwrm_cmd_hdr_init(bp, &req, HWRM_CFA_VFR_ALLOC, -1, -1);
	req.vf_id = cpu_to_le16(vf_idx);
	sprintf(req.vfr_name, "vfr%d", vf_idx);

	mutex_lock(&bp->hwrm_cmd_lock);
	rc = _hwrm_send_message(bp, &req, sizeof(req), HWRM_CMD_TIMEOUT);
	if (!rc) {
		*tx_cfa_action = le16_to_cpu(resp->tx_cfa_action);
		*rx_cfa_code = le16_to_cpu(resp->rx_cfa_code);
		netdev_dbg(bp->dev, "tx_cfa_action=0x%x, rx_cfa_code=0x%x",
			   *tx_cfa_action, *rx_cfa_code);
	} else {
		netdev_info(bp->dev, "%s error rc=%d", __func__, rc);
	}

	mutex_unlock(&bp->hwrm_cmd_lock);
=======
	struct hwrm_cfa_vfr_alloc_output *resp;
	struct hwrm_cfa_vfr_alloc_input *req;
	int rc;

	rc = hwrm_req_init(bp, req, HWRM_CFA_VFR_ALLOC);
	if (!rc) {
		req->vf_id = cpu_to_le16(vf_idx);
		sprintf(req->vfr_name, "vfr%d", vf_idx);

		resp = hwrm_req_hold(bp, req);
		rc = hwrm_req_send(bp, req);
		if (!rc) {
			*tx_cfa_action = le16_to_cpu(resp->tx_cfa_action);
			*rx_cfa_code = le16_to_cpu(resp->rx_cfa_code);
			netdev_dbg(bp->dev, "tx_cfa_action=0x%x, rx_cfa_code=0x%x",
				   *tx_cfa_action, *rx_cfa_code);
		}
		hwrm_req_drop(bp, req);
	}
	if (rc)
		netdev_info(bp->dev, "%s error rc=%d\n", __func__, rc);
>>>>>>> upstream/android-13
	return rc;
}

static int hwrm_cfa_vfr_free(struct bnxt *bp, u16 vf_idx)
{
<<<<<<< HEAD
	struct hwrm_cfa_vfr_free_input req = { 0 };
	int rc;

	bnxt_hwrm_cmd_hdr_init(bp, &req, HWRM_CFA_VFR_FREE, -1, -1);
	sprintf(req.vfr_name, "vfr%d", vf_idx);

	rc = hwrm_send_message(bp, &req, sizeof(req), HWRM_CMD_TIMEOUT);
	if (rc)
		netdev_info(bp->dev, "%s error rc=%d", __func__, rc);
=======
	struct hwrm_cfa_vfr_free_input *req;
	int rc;

	rc = hwrm_req_init(bp, req, HWRM_CFA_VFR_FREE);
	if (!rc) {
		sprintf(req->vfr_name, "vfr%d", vf_idx);
		rc = hwrm_req_send(bp, req);
	}
	if (rc)
		netdev_info(bp->dev, "%s error rc=%d\n", __func__, rc);
>>>>>>> upstream/android-13
	return rc;
}

static int bnxt_hwrm_vfr_qcfg(struct bnxt *bp, struct bnxt_vf_rep *vf_rep,
			      u16 *max_mtu)
{
<<<<<<< HEAD
	struct hwrm_func_qcfg_output *resp = bp->hwrm_cmd_resp_addr;
	struct hwrm_func_qcfg_input req = {0};
	u16 mtu;
	int rc;

	bnxt_hwrm_cmd_hdr_init(bp, &req, HWRM_FUNC_QCFG, -1, -1);
	req.fid = cpu_to_le16(bp->pf.vf[vf_rep->vf_idx].fw_fid);

	mutex_lock(&bp->hwrm_cmd_lock);

	rc = _hwrm_send_message(bp, &req, sizeof(req), HWRM_CMD_TIMEOUT);
=======
	struct hwrm_func_qcfg_output *resp;
	struct hwrm_func_qcfg_input *req;
	u16 mtu;
	int rc;

	rc = hwrm_req_init(bp, req, HWRM_FUNC_QCFG);
	if (rc)
		return rc;

	req->fid = cpu_to_le16(bp->pf.vf[vf_rep->vf_idx].fw_fid);
	resp = hwrm_req_hold(bp, req);
	rc = hwrm_req_send(bp, req);
>>>>>>> upstream/android-13
	if (!rc) {
		mtu = le16_to_cpu(resp->max_mtu_configured);
		if (!mtu)
			*max_mtu = BNXT_MAX_MTU;
		else
			*max_mtu = mtu;
	}
<<<<<<< HEAD
	mutex_unlock(&bp->hwrm_cmd_lock);
=======
	hwrm_req_drop(bp, req);
>>>>>>> upstream/android-13
	return rc;
}

static int bnxt_vf_rep_open(struct net_device *dev)
{
	struct bnxt_vf_rep *vf_rep = netdev_priv(dev);
	struct bnxt *bp = vf_rep->bp;

	/* Enable link and TX only if the parent PF is open. */
	if (netif_running(bp->dev)) {
		netif_carrier_on(dev);
		netif_tx_start_all_queues(dev);
	}
	return 0;
}

static int bnxt_vf_rep_close(struct net_device *dev)
{
	netif_carrier_off(dev);
	netif_tx_disable(dev);

	return 0;
}

static netdev_tx_t bnxt_vf_rep_xmit(struct sk_buff *skb,
				    struct net_device *dev)
{
	struct bnxt_vf_rep *vf_rep = netdev_priv(dev);
	int rc, len = skb->len;

	skb_dst_drop(skb);
	dst_hold((struct dst_entry *)vf_rep->dst);
	skb_dst_set(skb, (struct dst_entry *)vf_rep->dst);
	skb->dev = vf_rep->dst->u.port_info.lower_dev;

	rc = dev_queue_xmit(skb);
	if (!rc) {
		vf_rep->tx_stats.packets++;
		vf_rep->tx_stats.bytes += len;
	}
	return rc;
}

static void
bnxt_vf_rep_get_stats64(struct net_device *dev,
			struct rtnl_link_stats64 *stats)
{
	struct bnxt_vf_rep *vf_rep = netdev_priv(dev);

	stats->rx_packets = vf_rep->rx_stats.packets;
	stats->rx_bytes = vf_rep->rx_stats.bytes;
	stats->tx_packets = vf_rep->tx_stats.packets;
	stats->tx_bytes = vf_rep->tx_stats.bytes;
}

static int bnxt_vf_rep_setup_tc_block_cb(enum tc_setup_type type,
					 void *type_data,
					 void *cb_priv)
{
	struct bnxt_vf_rep *vf_rep = cb_priv;
	struct bnxt *bp = vf_rep->bp;
	int vf_fid = bp->pf.vf[vf_rep->vf_idx].fw_fid;

	if (!bnxt_tc_flower_enabled(vf_rep->bp) ||
	    !tc_cls_can_offload_and_chain0(bp->dev, type_data))
		return -EOPNOTSUPP;

	switch (type) {
	case TC_SETUP_CLSFLOWER:
		return bnxt_tc_setup_flower(bp, vf_fid, type_data);
	default:
		return -EOPNOTSUPP;
	}
}

<<<<<<< HEAD
static int bnxt_vf_rep_setup_tc_block(struct net_device *dev,
				      struct tc_block_offload *f)
{
	struct bnxt_vf_rep *vf_rep = netdev_priv(dev);

	if (f->binder_type != TCF_BLOCK_BINDER_TYPE_CLSACT_INGRESS)
		return -EOPNOTSUPP;

	switch (f->command) {
	case TC_BLOCK_BIND:
		return tcf_block_cb_register(f->block,
					     bnxt_vf_rep_setup_tc_block_cb,
					     vf_rep, vf_rep, f->extack);
	case TC_BLOCK_UNBIND:
		tcf_block_cb_unregister(f->block,
					bnxt_vf_rep_setup_tc_block_cb, vf_rep);
		return 0;
	default:
		return -EOPNOTSUPP;
	}
}
=======
static LIST_HEAD(bnxt_vf_block_cb_list);
>>>>>>> upstream/android-13

static int bnxt_vf_rep_setup_tc(struct net_device *dev, enum tc_setup_type type,
				void *type_data)
{
<<<<<<< HEAD
	switch (type) {
	case TC_SETUP_BLOCK:
		return bnxt_vf_rep_setup_tc_block(dev, type_data);
=======
	struct bnxt_vf_rep *vf_rep = netdev_priv(dev);

	switch (type) {
	case TC_SETUP_BLOCK:
		return flow_block_cb_setup_simple(type_data,
						  &bnxt_vf_block_cb_list,
						  bnxt_vf_rep_setup_tc_block_cb,
						  vf_rep, vf_rep, true);
>>>>>>> upstream/android-13
	default:
		return -EOPNOTSUPP;
	}
}

struct net_device *bnxt_get_vf_rep(struct bnxt *bp, u16 cfa_code)
{
	u16 vf_idx;

	if (cfa_code && bp->cfa_code_map && BNXT_PF(bp)) {
		vf_idx = bp->cfa_code_map[cfa_code];
		if (vf_idx != VF_IDX_INVALID)
			return bp->vf_reps[vf_idx]->dev;
	}
	return NULL;
}

void bnxt_vf_rep_rx(struct bnxt *bp, struct sk_buff *skb)
{
	struct bnxt_vf_rep *vf_rep = netdev_priv(skb->dev);
<<<<<<< HEAD
	struct bnxt_vf_rep_stats *rx_stats;

	rx_stats = &vf_rep->rx_stats;
=======

>>>>>>> upstream/android-13
	vf_rep->rx_stats.bytes += skb->len;
	vf_rep->rx_stats.packets++;

	netif_receive_skb(skb);
}

static int bnxt_vf_rep_get_phys_port_name(struct net_device *dev, char *buf,
					  size_t len)
{
	struct bnxt_vf_rep *vf_rep = netdev_priv(dev);
	struct pci_dev *pf_pdev = vf_rep->bp->pdev;
	int rc;

	rc = snprintf(buf, len, "pf%dvf%d", PCI_FUNC(pf_pdev->devfn),
		      vf_rep->vf_idx);
	if (rc >= len)
		return -EOPNOTSUPP;
	return 0;
}

static void bnxt_vf_rep_get_drvinfo(struct net_device *dev,
				    struct ethtool_drvinfo *info)
{
	strlcpy(info->driver, DRV_MODULE_NAME, sizeof(info->driver));
<<<<<<< HEAD
	strlcpy(info->version, DRV_MODULE_VERSION, sizeof(info->version));
}

static int bnxt_vf_rep_port_attr_get(struct net_device *dev,
				     struct switchdev_attr *attr)
=======
}

static int bnxt_vf_rep_get_port_parent_id(struct net_device *dev,
					  struct netdev_phys_item_id *ppid)
>>>>>>> upstream/android-13
{
	struct bnxt_vf_rep *vf_rep = netdev_priv(dev);

	/* as only PORT_PARENT_ID is supported currently use common code
	 * between PF and VF-rep for now.
	 */
<<<<<<< HEAD
	return bnxt_port_attr_get(vf_rep->bp, attr);
}

static const struct switchdev_ops bnxt_vf_rep_switchdev_ops = {
	.switchdev_port_attr_get	= bnxt_vf_rep_port_attr_get
};

=======
	return bnxt_get_port_parent_id(vf_rep->bp->dev, ppid);
}

>>>>>>> upstream/android-13
static const struct ethtool_ops bnxt_vf_rep_ethtool_ops = {
	.get_drvinfo		= bnxt_vf_rep_get_drvinfo
};

static const struct net_device_ops bnxt_vf_rep_netdev_ops = {
	.ndo_open		= bnxt_vf_rep_open,
	.ndo_stop		= bnxt_vf_rep_close,
	.ndo_start_xmit		= bnxt_vf_rep_xmit,
	.ndo_get_stats64	= bnxt_vf_rep_get_stats64,
	.ndo_setup_tc		= bnxt_vf_rep_setup_tc,
<<<<<<< HEAD
=======
	.ndo_get_port_parent_id	= bnxt_vf_rep_get_port_parent_id,
>>>>>>> upstream/android-13
	.ndo_get_phys_port_name = bnxt_vf_rep_get_phys_port_name
};

bool bnxt_dev_is_vf_rep(struct net_device *dev)
{
	return dev->netdev_ops == &bnxt_vf_rep_netdev_ops;
}

/* Called when the parent PF interface is closed:
 * As the mode transition from SWITCHDEV to LEGACY
 * happens under the rtnl_lock() this routine is safe
 * under the rtnl_lock()
 */
void bnxt_vf_reps_close(struct bnxt *bp)
{
	struct bnxt_vf_rep *vf_rep;
	u16 num_vfs, i;

	if (bp->eswitch_mode != DEVLINK_ESWITCH_MODE_SWITCHDEV)
		return;

	num_vfs = pci_num_vf(bp->pdev);
	for (i = 0; i < num_vfs; i++) {
		vf_rep = bp->vf_reps[i];
		if (netif_running(vf_rep->dev))
			bnxt_vf_rep_close(vf_rep->dev);
	}
}

/* Called when the parent PF interface is opened (re-opened):
 * As the mode transition from SWITCHDEV to LEGACY
 * happen under the rtnl_lock() this routine is safe
 * under the rtnl_lock()
 */
void bnxt_vf_reps_open(struct bnxt *bp)
{
	int i;

	if (bp->eswitch_mode != DEVLINK_ESWITCH_MODE_SWITCHDEV)
		return;

<<<<<<< HEAD
	for (i = 0; i < pci_num_vf(bp->pdev); i++)
		bnxt_vf_rep_open(bp->vf_reps[i]->dev);
=======
	for (i = 0; i < pci_num_vf(bp->pdev); i++) {
		/* Open the VF-Rep only if it is allocated in the FW */
		if (bp->vf_reps[i]->tx_cfa_action != CFA_HANDLE_INVALID)
			bnxt_vf_rep_open(bp->vf_reps[i]->dev);
	}
}

static void __bnxt_free_one_vf_rep(struct bnxt *bp, struct bnxt_vf_rep *vf_rep)
{
	if (!vf_rep)
		return;

	if (vf_rep->dst) {
		dst_release((struct dst_entry *)vf_rep->dst);
		vf_rep->dst = NULL;
	}
	if (vf_rep->tx_cfa_action != CFA_HANDLE_INVALID) {
		hwrm_cfa_vfr_free(bp, vf_rep->vf_idx);
		vf_rep->tx_cfa_action = CFA_HANDLE_INVALID;
	}
>>>>>>> upstream/android-13
}

static void __bnxt_vf_reps_destroy(struct bnxt *bp)
{
	u16 num_vfs = pci_num_vf(bp->pdev);
	struct bnxt_vf_rep *vf_rep;
	int i;

	for (i = 0; i < num_vfs; i++) {
		vf_rep = bp->vf_reps[i];
		if (vf_rep) {
<<<<<<< HEAD
			dst_release((struct dst_entry *)vf_rep->dst);

			if (vf_rep->tx_cfa_action != CFA_HANDLE_INVALID)
				hwrm_cfa_vfr_free(bp, vf_rep->vf_idx);

=======
			__bnxt_free_one_vf_rep(bp, vf_rep);
>>>>>>> upstream/android-13
			if (vf_rep->dev) {
				/* if register_netdev failed, then netdev_ops
				 * would have been set to NULL
				 */
				if (vf_rep->dev->netdev_ops)
					unregister_netdev(vf_rep->dev);
				free_netdev(vf_rep->dev);
			}
		}
	}

	kfree(bp->vf_reps);
	bp->vf_reps = NULL;
}

void bnxt_vf_reps_destroy(struct bnxt *bp)
{
	bool closed = false;

	if (bp->eswitch_mode != DEVLINK_ESWITCH_MODE_SWITCHDEV)
		return;

	if (!bp->vf_reps)
		return;

	/* Ensure that parent PF's and VF-reps' RX/TX has been quiesced
	 * before proceeding with VF-rep cleanup.
	 */
	rtnl_lock();
	if (netif_running(bp->dev)) {
		bnxt_close_nic(bp, false, false);
		closed = true;
	}
	/* un-publish cfa_code_map so that RX path can't see it anymore */
	kfree(bp->cfa_code_map);
	bp->cfa_code_map = NULL;
	bp->eswitch_mode = DEVLINK_ESWITCH_MODE_LEGACY;

	if (closed)
		bnxt_open_nic(bp, false, false);
	rtnl_unlock();

	/* Need to call vf_reps_destroy() outside of rntl_lock
	 * as unregister_netdev takes rtnl_lock
	 */
	__bnxt_vf_reps_destroy(bp);
}

<<<<<<< HEAD
=======
/* Free the VF-Reps in firmware, during firmware hot-reset processing.
 * Note that the VF-Rep netdevs are still active (not unregistered) during
 * this process. As the mode transition from SWITCHDEV to LEGACY happens
 * under the rtnl_lock() this routine is safe under the rtnl_lock().
 */
void bnxt_vf_reps_free(struct bnxt *bp)
{
	u16 num_vfs = pci_num_vf(bp->pdev);
	int i;

	if (bp->eswitch_mode != DEVLINK_ESWITCH_MODE_SWITCHDEV)
		return;

	for (i = 0; i < num_vfs; i++)
		__bnxt_free_one_vf_rep(bp, bp->vf_reps[i]);
}

static int bnxt_alloc_vf_rep(struct bnxt *bp, struct bnxt_vf_rep *vf_rep,
			     u16 *cfa_code_map)
{
	/* get cfa handles from FW */
	if (hwrm_cfa_vfr_alloc(bp, vf_rep->vf_idx, &vf_rep->tx_cfa_action,
			       &vf_rep->rx_cfa_code))
		return -ENOLINK;

	cfa_code_map[vf_rep->rx_cfa_code] = vf_rep->vf_idx;
	vf_rep->dst = metadata_dst_alloc(0, METADATA_HW_PORT_MUX, GFP_KERNEL);
	if (!vf_rep->dst)
		return -ENOMEM;

	/* only cfa_action is needed to mux a packet while TXing */
	vf_rep->dst->u.port_info.port_id = vf_rep->tx_cfa_action;
	vf_rep->dst->u.port_info.lower_dev = bp->dev;

	return 0;
}

/* Allocate the VF-Reps in firmware, during firmware hot-reset processing.
 * Note that the VF-Rep netdevs are still active (not unregistered) during
 * this process. As the mode transition from SWITCHDEV to LEGACY happens
 * under the rtnl_lock() this routine is safe under the rtnl_lock().
 */
int bnxt_vf_reps_alloc(struct bnxt *bp)
{
	u16 *cfa_code_map = bp->cfa_code_map, num_vfs = pci_num_vf(bp->pdev);
	struct bnxt_vf_rep *vf_rep;
	int rc, i;

	if (bp->eswitch_mode != DEVLINK_ESWITCH_MODE_SWITCHDEV)
		return 0;

	if (!cfa_code_map)
		return -EINVAL;

	for (i = 0; i < MAX_CFA_CODE; i++)
		cfa_code_map[i] = VF_IDX_INVALID;

	for (i = 0; i < num_vfs; i++) {
		vf_rep = bp->vf_reps[i];
		vf_rep->vf_idx = i;

		rc = bnxt_alloc_vf_rep(bp, vf_rep, cfa_code_map);
		if (rc)
			goto err;
	}

	return 0;

err:
	netdev_info(bp->dev, "%s error=%d\n", __func__, rc);
	bnxt_vf_reps_free(bp);
	return rc;
}

>>>>>>> upstream/android-13
/* Use the OUI of the PF's perm addr and report the same mac addr
 * for the same VF-rep each time
 */
static void bnxt_vf_rep_eth_addr_gen(u8 *src_mac, u16 vf_idx, u8 *mac)
{
	u32 addr;

	ether_addr_copy(mac, src_mac);

	addr = jhash(src_mac, ETH_ALEN, 0) + vf_idx;
	mac[3] = (u8)(addr & 0xFF);
	mac[4] = (u8)((addr >> 8) & 0xFF);
	mac[5] = (u8)((addr >> 16) & 0xFF);
}

static void bnxt_vf_rep_netdev_init(struct bnxt *bp, struct bnxt_vf_rep *vf_rep,
				    struct net_device *dev)
{
	struct net_device *pf_dev = bp->dev;
	u16 max_mtu;

	dev->netdev_ops = &bnxt_vf_rep_netdev_ops;
	dev->ethtool_ops = &bnxt_vf_rep_ethtool_ops;
<<<<<<< HEAD
	SWITCHDEV_SET_OPS(dev, &bnxt_vf_rep_switchdev_ops);
=======
>>>>>>> upstream/android-13
	/* Just inherit all the featues of the parent PF as the VF-R
	 * uses the RX/TX rings of the parent PF
	 */
	dev->hw_features = pf_dev->hw_features;
	dev->gso_partial_features = pf_dev->gso_partial_features;
	dev->vlan_features = pf_dev->vlan_features;
	dev->hw_enc_features = pf_dev->hw_enc_features;
	dev->features |= pf_dev->features;
	bnxt_vf_rep_eth_addr_gen(bp->pf.mac_addr, vf_rep->vf_idx,
				 dev->perm_addr);
	ether_addr_copy(dev->dev_addr, dev->perm_addr);
	/* Set VF-Rep's max-mtu to the corresponding VF's max-mtu */
	if (!bnxt_hwrm_vfr_qcfg(bp, vf_rep, &max_mtu))
		dev->max_mtu = max_mtu;
	dev->min_mtu = ETH_ZLEN;
}

<<<<<<< HEAD
static int bnxt_pcie_dsn_get(struct bnxt *bp, u8 dsn[])
{
	struct pci_dev *pdev = bp->pdev;
	int pos = pci_find_ext_capability(pdev, PCI_EXT_CAP_ID_DSN);
	u32 dw;

	if (!pos) {
		netdev_info(bp->dev, "Unable do read adapter's DSN");
		return -EOPNOTSUPP;
	}

	/* DSN (two dw) is at an offset of 4 from the cap pos */
	pos += 4;
	pci_read_config_dword(pdev, pos, &dw);
	put_unaligned_le32(dw, &dsn[0]);
	pci_read_config_dword(pdev, pos + 4, &dw);
	put_unaligned_le32(dw, &dsn[4]);
	return 0;
}

=======
>>>>>>> upstream/android-13
static int bnxt_vf_reps_create(struct bnxt *bp)
{
	u16 *cfa_code_map = NULL, num_vfs = pci_num_vf(bp->pdev);
	struct bnxt_vf_rep *vf_rep;
	struct net_device *dev;
	int rc, i;

<<<<<<< HEAD
=======
	if (!(bp->flags & BNXT_FLAG_DSN_VALID))
		return -ENODEV;

>>>>>>> upstream/android-13
	bp->vf_reps = kcalloc(num_vfs, sizeof(vf_rep), GFP_KERNEL);
	if (!bp->vf_reps)
		return -ENOMEM;

	/* storage for cfa_code to vf-idx mapping */
	cfa_code_map = kmalloc_array(MAX_CFA_CODE, sizeof(*bp->cfa_code_map),
				     GFP_KERNEL);
	if (!cfa_code_map) {
		rc = -ENOMEM;
		goto err;
	}
	for (i = 0; i < MAX_CFA_CODE; i++)
		cfa_code_map[i] = VF_IDX_INVALID;

	for (i = 0; i < num_vfs; i++) {
		dev = alloc_etherdev(sizeof(*vf_rep));
		if (!dev) {
			rc = -ENOMEM;
			goto err;
		}

		vf_rep = netdev_priv(dev);
		bp->vf_reps[i] = vf_rep;
		vf_rep->dev = dev;
		vf_rep->bp = bp;
		vf_rep->vf_idx = i;
		vf_rep->tx_cfa_action = CFA_HANDLE_INVALID;

<<<<<<< HEAD
		/* get cfa handles from FW */
		rc = hwrm_cfa_vfr_alloc(bp, vf_rep->vf_idx,
					&vf_rep->tx_cfa_action,
					&vf_rep->rx_cfa_code);
		if (rc) {
			rc = -ENOLINK;
			goto err;
		}
		cfa_code_map[vf_rep->rx_cfa_code] = vf_rep->vf_idx;

		vf_rep->dst = metadata_dst_alloc(0, METADATA_HW_PORT_MUX,
						 GFP_KERNEL);
		if (!vf_rep->dst) {
			rc = -ENOMEM;
			goto err;
		}
		/* only cfa_action is needed to mux a packet while TXing */
		vf_rep->dst->u.port_info.port_id = vf_rep->tx_cfa_action;
		vf_rep->dst->u.port_info.lower_dev = bp->dev;
=======
		rc = bnxt_alloc_vf_rep(bp, vf_rep, cfa_code_map);
		if (rc)
			goto err;
>>>>>>> upstream/android-13

		bnxt_vf_rep_netdev_init(bp, vf_rep, dev);
		rc = register_netdev(dev);
		if (rc) {
			/* no need for unregister_netdev in cleanup */
			dev->netdev_ops = NULL;
			goto err;
		}
	}

<<<<<<< HEAD
	/* Read the adapter's DSN to use as the eswitch switch_id */
	rc = bnxt_pcie_dsn_get(bp, bp->switch_id);
	if (rc)
		goto err;

=======
>>>>>>> upstream/android-13
	/* publish cfa_code_map only after all VF-reps have been initialized */
	bp->cfa_code_map = cfa_code_map;
	bp->eswitch_mode = DEVLINK_ESWITCH_MODE_SWITCHDEV;
	netif_keep_dst(bp->dev);
	return 0;

err:
<<<<<<< HEAD
	netdev_info(bp->dev, "%s error=%d", __func__, rc);
=======
	netdev_info(bp->dev, "%s error=%d\n", __func__, rc);
>>>>>>> upstream/android-13
	kfree(cfa_code_map);
	__bnxt_vf_reps_destroy(bp);
	return rc;
}

/* Devlink related routines */
int bnxt_dl_eswitch_mode_get(struct devlink *devlink, u16 *mode)
{
	struct bnxt *bp = bnxt_get_bp_from_dl(devlink);

	*mode = bp->eswitch_mode;
	return 0;
}

<<<<<<< HEAD
int bnxt_dl_eswitch_mode_set(struct devlink *devlink, u16 mode)
=======
int bnxt_dl_eswitch_mode_set(struct devlink *devlink, u16 mode,
			     struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	struct bnxt *bp = bnxt_get_bp_from_dl(devlink);
	int rc = 0;

	mutex_lock(&bp->sriov_lock);
	if (bp->eswitch_mode == mode) {
<<<<<<< HEAD
		netdev_info(bp->dev, "already in %s eswitch mode",
=======
		netdev_info(bp->dev, "already in %s eswitch mode\n",
>>>>>>> upstream/android-13
			    mode == DEVLINK_ESWITCH_MODE_LEGACY ?
			    "legacy" : "switchdev");
		rc = -EINVAL;
		goto done;
	}

	switch (mode) {
	case DEVLINK_ESWITCH_MODE_LEGACY:
		bnxt_vf_reps_destroy(bp);
		break;

	case DEVLINK_ESWITCH_MODE_SWITCHDEV:
		if (bp->hwrm_spec_code < 0x10803) {
			netdev_warn(bp->dev, "FW does not support SRIOV E-Switch SWITCHDEV mode\n");
			rc = -ENOTSUPP;
			goto done;
		}

		if (pci_num_vf(bp->pdev) == 0) {
<<<<<<< HEAD
			netdev_info(bp->dev, "Enable VFs before setting switchdev mode");
=======
			netdev_info(bp->dev, "Enable VFs before setting switchdev mode\n");
>>>>>>> upstream/android-13
			rc = -EPERM;
			goto done;
		}
		rc = bnxt_vf_reps_create(bp);
		break;

	default:
		rc = -EINVAL;
		goto done;
	}
done:
	mutex_unlock(&bp->sriov_lock);
	return rc;
}

#endif
