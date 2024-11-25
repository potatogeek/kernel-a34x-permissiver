// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2018, Intel Corporation. */

/* Intel(R) Ethernet Connection E800 Series Linux Driver */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

<<<<<<< HEAD
#include "ice.h"

#define DRV_VERSION	"0.7.1-k"
#define DRV_SUMMARY	"Intel(R) Ethernet Connection E800 Series Linux Driver"
const char ice_drv_ver[] = DRV_VERSION;
static const char ice_driver_string[] = DRV_SUMMARY;
static const char ice_copyright[] = "Copyright (c) 2018, Intel Corporation.";

MODULE_AUTHOR("Intel Corporation, <linux.nics@intel.com>");
MODULE_DESCRIPTION(DRV_SUMMARY);
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
=======
#include <generated/utsrelease.h>
#include "ice.h"
#include "ice_base.h"
#include "ice_lib.h"
#include "ice_fltr.h"
#include "ice_dcb_lib.h"
#include "ice_dcb_nl.h"
#include "ice_devlink.h"
/* Including ice_trace.h with CREATE_TRACE_POINTS defined will generate the
 * ice tracepoint functions. This must be done exactly once across the
 * ice driver.
 */
#define CREATE_TRACE_POINTS
#include "ice_trace.h"

#define DRV_SUMMARY	"Intel(R) Ethernet Connection E800 Series Linux Driver"
static const char ice_driver_string[] = DRV_SUMMARY;
static const char ice_copyright[] = "Copyright (c) 2018, Intel Corporation.";

/* DDP Package file located in firmware search paths (e.g. /lib/firmware/) */
#define ICE_DDP_PKG_PATH	"intel/ice/ddp/"
#define ICE_DDP_PKG_FILE	ICE_DDP_PKG_PATH "ice.pkg"

MODULE_AUTHOR("Intel Corporation, <linux.nics@intel.com>");
MODULE_DESCRIPTION(DRV_SUMMARY);
MODULE_LICENSE("GPL v2");
MODULE_FIRMWARE(ICE_DDP_PKG_FILE);
>>>>>>> upstream/android-13

static int debug = -1;
module_param(debug, int, 0644);
#ifndef CONFIG_DYNAMIC_DEBUG
MODULE_PARM_DESC(debug, "netif level (0=none,...,16=all), hw debug_mask (0x8XXXXXXX)");
#else
MODULE_PARM_DESC(debug, "netif level (0=none,...,16=all)");
#endif /* !CONFIG_DYNAMIC_DEBUG */

<<<<<<< HEAD
static struct workqueue_struct *ice_wq;
static const struct net_device_ops ice_netdev_ops;

static void ice_pf_dis_all_vsi(struct ice_pf *pf);
static void ice_rebuild(struct ice_pf *pf);
static int ice_vsi_release(struct ice_vsi *vsi);
static void ice_update_vsi_stats(struct ice_vsi *vsi);
static void ice_update_pf_stats(struct ice_pf *pf);

/**
 * ice_get_free_slot - get the next non-NULL location index in array
 * @array: array to search
 * @size: size of the array
 * @curr: last known occupied index to be used as a search hint
 *
 * void * is being used to keep the functionality generic. This lets us use this
 * function on any array of pointers.
 */
static int ice_get_free_slot(void *array, int size, int curr)
{
	int **tmp_array = (int **)array;
	int next;

	if (curr < (size - 1) && !tmp_array[curr + 1]) {
		next = curr + 1;
	} else {
		int i = 0;

		while ((i < size) && (tmp_array[i]))
			i++;
		if (i == size)
			next = ICE_NO_VSI;
		else
			next = i;
	}
	return next;
}

/**
 * ice_search_res - Search the tracker for a block of resources
 * @res: pointer to the resource
 * @needed: size of the block needed
 * @id: identifier to track owner
 * Returns the base item index of the block, or -ENOMEM for error
 */
static int ice_search_res(struct ice_res_tracker *res, u16 needed, u16 id)
{
	int start = res->search_hint;
	int end = start;

	id |= ICE_RES_VALID_BIT;

	do {
		/* skip already allocated entries */
		if (res->list[end++] & ICE_RES_VALID_BIT) {
			start = end;
			if ((start + needed) > res->num_entries)
				break;
		}

		if (end == (start + needed)) {
			int i = start;

			/* there was enough, so assign it to the requestor */
			while (i != end)
				res->list[i++] = id;

			if (end == res->num_entries)
				end = 0;

			res->search_hint = end;
			return start;
		}
	} while (1);

	return -ENOMEM;
}

/**
 * ice_get_res - get a block of resources
 * @pf: board private structure
 * @res: pointer to the resource
 * @needed: size of the block needed
 * @id: identifier to track owner
 *
 * Returns the base item index of the block, or -ENOMEM for error
 * The search_hint trick and lack of advanced fit-finding only works
 * because we're highly likely to have all the same sized requests.
 * Linear search time and any fragmentation should be minimal.
 */
static int
ice_get_res(struct ice_pf *pf, struct ice_res_tracker *res, u16 needed, u16 id)
{
	int ret;

	if (!res || !pf)
		return -EINVAL;

	if (!needed || needed > res->num_entries || id >= ICE_RES_VALID_BIT) {
		dev_err(&pf->pdev->dev,
			"param err: needed=%d, num_entries = %d id=0x%04x\n",
			needed, res->num_entries, id);
		return -EINVAL;
	}

	/* search based on search_hint */
	ret = ice_search_res(res, needed, id);

	if (ret < 0) {
		/* previous search failed. Reset search hint and try again */
		res->search_hint = 0;
		ret = ice_search_res(res, needed, id);
	}

	return ret;
}

/**
 * ice_free_res - free a block of resources
 * @res: pointer to the resource
 * @index: starting index previously returned by ice_get_res
 * @id: identifier to track owner
 * Returns number of resources freed
 */
static int ice_free_res(struct ice_res_tracker *res, u16 index, u16 id)
{
	int count = 0;
	int i;

	if (!res || index >= res->num_entries)
		return -EINVAL;

	id |= ICE_RES_VALID_BIT;
	for (i = index; i < res->num_entries && res->list[i] == id; i++) {
		res->list[i] = 0;
		count++;
	}

	return count;
}

/**
 * ice_add_mac_to_list - Add a mac address filter entry to the list
 * @vsi: the VSI to be forwarded to
 * @add_list: pointer to the list which contains MAC filter entries
 * @macaddr: the MAC address to be added.
 *
 * Adds mac address filter entry to the temp list
 *
 * Returns 0 on success or ENOMEM on failure.
 */
static int ice_add_mac_to_list(struct ice_vsi *vsi, struct list_head *add_list,
			       const u8 *macaddr)
{
	struct ice_fltr_list_entry *tmp;
	struct ice_pf *pf = vsi->back;

	tmp = devm_kzalloc(&pf->pdev->dev, sizeof(*tmp), GFP_ATOMIC);
	if (!tmp)
		return -ENOMEM;

	tmp->fltr_info.flag = ICE_FLTR_TX;
	tmp->fltr_info.src = vsi->vsi_num;
	tmp->fltr_info.lkup_type = ICE_SW_LKUP_MAC;
	tmp->fltr_info.fltr_act = ICE_FWD_TO_VSI;
	tmp->fltr_info.fwd_id.vsi_id = vsi->vsi_num;
	ether_addr_copy(tmp->fltr_info.l_data.mac.mac_addr, macaddr);

	INIT_LIST_HEAD(&tmp->list_entry);
	list_add(&tmp->list_entry, add_list);
=======
static DEFINE_IDA(ice_aux_ida);

static struct workqueue_struct *ice_wq;
static const struct net_device_ops ice_netdev_safe_mode_ops;
static const struct net_device_ops ice_netdev_ops;
static int ice_vsi_open(struct ice_vsi *vsi);

static void ice_rebuild(struct ice_pf *pf, enum ice_reset_req reset_type);

static void ice_vsi_release_all(struct ice_pf *pf);

bool netif_is_ice(struct net_device *dev)
{
	return dev && (dev->netdev_ops == &ice_netdev_ops);
}

/**
 * ice_get_tx_pending - returns number of Tx descriptors not processed
 * @ring: the ring of descriptors
 */
static u16 ice_get_tx_pending(struct ice_ring *ring)
{
	u16 head, tail;

	head = ring->next_to_clean;
	tail = ring->next_to_use;

	if (head != tail)
		return (head < tail) ?
			tail - head : (tail + ring->count - head);
	return 0;
}

/**
 * ice_check_for_hang_subtask - check for and recover hung queues
 * @pf: pointer to PF struct
 */
static void ice_check_for_hang_subtask(struct ice_pf *pf)
{
	struct ice_vsi *vsi = NULL;
	struct ice_hw *hw;
	unsigned int i;
	int packets;
	u32 v;

	ice_for_each_vsi(pf, v)
		if (pf->vsi[v] && pf->vsi[v]->type == ICE_VSI_PF) {
			vsi = pf->vsi[v];
			break;
		}

	if (!vsi || test_bit(ICE_VSI_DOWN, vsi->state))
		return;

	if (!(vsi->netdev && netif_carrier_ok(vsi->netdev)))
		return;

	hw = &vsi->back->hw;

	for (i = 0; i < vsi->num_txq; i++) {
		struct ice_ring *tx_ring = vsi->tx_rings[i];

		if (tx_ring && tx_ring->desc) {
			/* If packet counter has not changed the queue is
			 * likely stalled, so force an interrupt for this
			 * queue.
			 *
			 * prev_pkt would be negative if there was no
			 * pending work.
			 */
			packets = tx_ring->stats.pkts & INT_MAX;
			if (tx_ring->tx_stats.prev_pkt == packets) {
				/* Trigger sw interrupt to revive the queue */
				ice_trigger_sw_intr(hw, tx_ring->q_vector);
				continue;
			}

			/* Memory barrier between read of packet count and call
			 * to ice_get_tx_pending()
			 */
			smp_rmb();
			tx_ring->tx_stats.prev_pkt =
			    ice_get_tx_pending(tx_ring) ? packets : -1;
		}
	}
}

/**
 * ice_init_mac_fltr - Set initial MAC filters
 * @pf: board private structure
 *
 * Set initial set of MAC filters for PF VSI; configure filters for permanent
 * address and broadcast address. If an error is encountered, netdevice will be
 * unregistered.
 */
static int ice_init_mac_fltr(struct ice_pf *pf)
{
	enum ice_status status;
	struct ice_vsi *vsi;
	u8 *perm_addr;

	vsi = ice_get_main_vsi(pf);
	if (!vsi)
		return -EINVAL;

	perm_addr = vsi->port_info->mac.perm_addr;
	status = ice_fltr_add_mac_and_broadcast(vsi, perm_addr, ICE_FWD_TO_VSI);
	if (status)
		return -EIO;
>>>>>>> upstream/android-13

	return 0;
}

/**
<<<<<<< HEAD
 * ice_add_mac_to_sync_list - creates list of mac addresses to be synced
 * @netdev: the net device on which the sync is happening
 * @addr: mac address to sync
=======
 * ice_add_mac_to_sync_list - creates list of MAC addresses to be synced
 * @netdev: the net device on which the sync is happening
 * @addr: MAC address to sync
>>>>>>> upstream/android-13
 *
 * This is a callback function which is called by the in kernel device sync
 * functions (like __dev_uc_sync, __dev_mc_sync, etc). This function only
 * populates the tmp_sync_list, which is later used by ice_add_mac to add the
<<<<<<< HEAD
 * mac filters from the hardware.
=======
 * MAC filters from the hardware.
>>>>>>> upstream/android-13
 */
static int ice_add_mac_to_sync_list(struct net_device *netdev, const u8 *addr)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;

<<<<<<< HEAD
	if (ice_add_mac_to_list(vsi, &vsi->tmp_sync_list, addr))
=======
	if (ice_fltr_add_mac_to_list(vsi, &vsi->tmp_sync_list, addr,
				     ICE_FWD_TO_VSI))
>>>>>>> upstream/android-13
		return -EINVAL;

	return 0;
}

/**
<<<<<<< HEAD
 * ice_add_mac_to_unsync_list - creates list of mac addresses to be unsynced
 * @netdev: the net device on which the unsync is happening
 * @addr: mac address to unsync
=======
 * ice_add_mac_to_unsync_list - creates list of MAC addresses to be unsynced
 * @netdev: the net device on which the unsync is happening
 * @addr: MAC address to unsync
>>>>>>> upstream/android-13
 *
 * This is a callback function which is called by the in kernel device unsync
 * functions (like __dev_uc_unsync, __dev_mc_unsync, etc). This function only
 * populates the tmp_unsync_list, which is later used by ice_remove_mac to
<<<<<<< HEAD
 * delete the mac filters from the hardware.
=======
 * delete the MAC filters from the hardware.
>>>>>>> upstream/android-13
 */
static int ice_add_mac_to_unsync_list(struct net_device *netdev, const u8 *addr)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;

<<<<<<< HEAD
	if (ice_add_mac_to_list(vsi, &vsi->tmp_unsync_list, addr))
=======
	/* Under some circumstances, we might receive a request to delete our
	 * own device address from our uc list. Because we store the device
	 * address in the VSI's MAC filter list, we need to ignore such
	 * requests and not delete our device address from this list.
	 */
	if (ether_addr_equal(addr, netdev->dev_addr))
		return 0;

	if (ice_fltr_add_mac_to_list(vsi, &vsi->tmp_unsync_list, addr,
				     ICE_FWD_TO_VSI))
>>>>>>> upstream/android-13
		return -EINVAL;

	return 0;
}

/**
<<<<<<< HEAD
 * ice_free_fltr_list - free filter lists helper
 * @dev: pointer to the device struct
 * @h: pointer to the list head to be freed
 *
 * Helper function to free filter lists previously created using
 * ice_add_mac_to_list
 */
static void ice_free_fltr_list(struct device *dev, struct list_head *h)
{
	struct ice_fltr_list_entry *e, *tmp;

	list_for_each_entry_safe(e, tmp, h, list_entry) {
		list_del(&e->list_entry);
		devm_kfree(dev, e);
	}
}

/**
=======
>>>>>>> upstream/android-13
 * ice_vsi_fltr_changed - check if filter state changed
 * @vsi: VSI to be checked
 *
 * returns true if filter state has changed, false otherwise.
 */
static bool ice_vsi_fltr_changed(struct ice_vsi *vsi)
{
<<<<<<< HEAD
	return test_bit(ICE_VSI_FLAG_UMAC_FLTR_CHANGED, vsi->flags) ||
	       test_bit(ICE_VSI_FLAG_MMAC_FLTR_CHANGED, vsi->flags) ||
	       test_bit(ICE_VSI_FLAG_VLAN_FLTR_CHANGED, vsi->flags);
=======
	return test_bit(ICE_VSI_UMAC_FLTR_CHANGED, vsi->state) ||
	       test_bit(ICE_VSI_MMAC_FLTR_CHANGED, vsi->state) ||
	       test_bit(ICE_VSI_VLAN_FLTR_CHANGED, vsi->state);
}

/**
 * ice_cfg_promisc - Enable or disable promiscuous mode for a given PF
 * @vsi: the VSI being configured
 * @promisc_m: mask of promiscuous config bits
 * @set_promisc: enable or disable promisc flag request
 *
 */
static int ice_cfg_promisc(struct ice_vsi *vsi, u8 promisc_m, bool set_promisc)
{
	struct ice_hw *hw = &vsi->back->hw;
	enum ice_status status = 0;

	if (vsi->type != ICE_VSI_PF)
		return 0;

	if (vsi->num_vlan > 1) {
		status = ice_set_vlan_vsi_promisc(hw, vsi->idx, promisc_m,
						  set_promisc);
	} else {
		if (set_promisc)
			status = ice_set_vsi_promisc(hw, vsi->idx, promisc_m,
						     0);
		else
			status = ice_clear_vsi_promisc(hw, vsi->idx, promisc_m,
						       0);
	}

	if (status)
		return -EIO;

	return 0;
>>>>>>> upstream/android-13
}

/**
 * ice_vsi_sync_fltr - Update the VSI filter list to the HW
 * @vsi: ptr to the VSI
 *
 * Push any outstanding VSI filter changes through the AdminQ.
 */
static int ice_vsi_sync_fltr(struct ice_vsi *vsi)
{
<<<<<<< HEAD
	struct device *dev = &vsi->back->pdev->dev;
=======
	struct device *dev = ice_pf_to_dev(vsi->back);
>>>>>>> upstream/android-13
	struct net_device *netdev = vsi->netdev;
	bool promisc_forced_on = false;
	struct ice_pf *pf = vsi->back;
	struct ice_hw *hw = &pf->hw;
	enum ice_status status = 0;
	u32 changed_flags = 0;
<<<<<<< HEAD
=======
	u8 promisc_m;
>>>>>>> upstream/android-13
	int err = 0;

	if (!vsi->netdev)
		return -EINVAL;

<<<<<<< HEAD
	while (test_and_set_bit(__ICE_CFG_BUSY, vsi->state))
=======
	while (test_and_set_bit(ICE_CFG_BUSY, vsi->state))
>>>>>>> upstream/android-13
		usleep_range(1000, 2000);

	changed_flags = vsi->current_netdev_flags ^ vsi->netdev->flags;
	vsi->current_netdev_flags = vsi->netdev->flags;

	INIT_LIST_HEAD(&vsi->tmp_sync_list);
	INIT_LIST_HEAD(&vsi->tmp_unsync_list);

	if (ice_vsi_fltr_changed(vsi)) {
<<<<<<< HEAD
		clear_bit(ICE_VSI_FLAG_UMAC_FLTR_CHANGED, vsi->flags);
		clear_bit(ICE_VSI_FLAG_MMAC_FLTR_CHANGED, vsi->flags);
		clear_bit(ICE_VSI_FLAG_VLAN_FLTR_CHANGED, vsi->flags);
=======
		clear_bit(ICE_VSI_UMAC_FLTR_CHANGED, vsi->state);
		clear_bit(ICE_VSI_MMAC_FLTR_CHANGED, vsi->state);
		clear_bit(ICE_VSI_VLAN_FLTR_CHANGED, vsi->state);
>>>>>>> upstream/android-13

		/* grab the netdev's addr_list_lock */
		netif_addr_lock_bh(netdev);
		__dev_uc_sync(netdev, ice_add_mac_to_sync_list,
			      ice_add_mac_to_unsync_list);
		__dev_mc_sync(netdev, ice_add_mac_to_sync_list,
			      ice_add_mac_to_unsync_list);
		/* our temp lists are populated. release lock */
		netif_addr_unlock_bh(netdev);
	}

<<<<<<< HEAD
	/* Remove mac addresses in the unsync list */
	status = ice_remove_mac(hw, &vsi->tmp_unsync_list);
	ice_free_fltr_list(dev, &vsi->tmp_unsync_list);
=======
	/* Remove MAC addresses in the unsync list */
	status = ice_fltr_remove_mac_list(vsi, &vsi->tmp_unsync_list);
	ice_fltr_free_list(dev, &vsi->tmp_unsync_list);
>>>>>>> upstream/android-13
	if (status) {
		netdev_err(netdev, "Failed to delete MAC filters\n");
		/* if we failed because of alloc failures, just bail */
		if (status == ICE_ERR_NO_MEMORY) {
			err = -ENOMEM;
			goto out;
		}
	}

<<<<<<< HEAD
	/* Add mac addresses in the sync list */
	status = ice_add_mac(hw, &vsi->tmp_sync_list);
	ice_free_fltr_list(dev, &vsi->tmp_sync_list);
	if (status) {
		netdev_err(netdev, "Failed to add MAC filters\n");
		/* If there is no more space for new umac filters, vsi
=======
	/* Add MAC addresses in the sync list */
	status = ice_fltr_add_mac_list(vsi, &vsi->tmp_sync_list);
	ice_fltr_free_list(dev, &vsi->tmp_sync_list);
	/* If filter is added successfully or already exists, do not go into
	 * 'if' condition and report it as error. Instead continue processing
	 * rest of the function.
	 */
	if (status && status != ICE_ERR_ALREADY_EXISTS) {
		netdev_err(netdev, "Failed to add MAC filters\n");
		/* If there is no more space for new umac filters, VSI
>>>>>>> upstream/android-13
		 * should go into promiscuous mode. There should be some
		 * space reserved for promiscuous filters.
		 */
		if (hw->adminq.sq_last_status == ICE_AQ_RC_ENOSPC &&
<<<<<<< HEAD
		    !test_and_set_bit(__ICE_FLTR_OVERFLOW_PROMISC,
				      vsi->state)) {
			promisc_forced_on = true;
			netdev_warn(netdev,
				    "Reached MAC filter limit, forcing promisc mode on VSI %d\n",
=======
		    !test_and_set_bit(ICE_FLTR_OVERFLOW_PROMISC,
				      vsi->state)) {
			promisc_forced_on = true;
			netdev_warn(netdev, "Reached MAC filter limit, forcing promisc mode on VSI %d\n",
>>>>>>> upstream/android-13
				    vsi->vsi_num);
		} else {
			err = -EIO;
			goto out;
		}
	}
	/* check for changes in promiscuous modes */
<<<<<<< HEAD
	if (changed_flags & IFF_ALLMULTI)
		netdev_warn(netdev, "Unsupported configuration\n");

	if (((changed_flags & IFF_PROMISC) || promisc_forced_on) ||
	    test_bit(ICE_VSI_FLAG_PROMISC_CHANGED, vsi->flags)) {
		clear_bit(ICE_VSI_FLAG_PROMISC_CHANGED, vsi->flags);
		if (vsi->current_netdev_flags & IFF_PROMISC) {
			/* Apply TX filter rule to get traffic from VMs */
			status = ice_cfg_dflt_vsi(hw, vsi->vsi_num, true,
						  ICE_FLTR_TX);
			if (status) {
				netdev_err(netdev, "Error setting default VSI %i tx rule\n",
					   vsi->vsi_num);
				vsi->current_netdev_flags &= ~IFF_PROMISC;
				err = -EIO;
				goto out_promisc;
			}
			/* Apply RX filter rule to get traffic from wire */
			status = ice_cfg_dflt_vsi(hw, vsi->vsi_num, true,
						  ICE_FLTR_RX);
			if (status) {
				netdev_err(netdev, "Error setting default VSI %i rx rule\n",
					   vsi->vsi_num);
				vsi->current_netdev_flags &= ~IFF_PROMISC;
				err = -EIO;
				goto out_promisc;
			}
		} else {
			/* Clear TX filter rule to stop traffic from VMs */
			status = ice_cfg_dflt_vsi(hw, vsi->vsi_num, false,
						  ICE_FLTR_TX);
			if (status) {
				netdev_err(netdev, "Error clearing default VSI %i tx rule\n",
					   vsi->vsi_num);
				vsi->current_netdev_flags |= IFF_PROMISC;
				err = -EIO;
				goto out_promisc;
			}
			/* Clear filter RX to remove traffic from wire */
			status = ice_cfg_dflt_vsi(hw, vsi->vsi_num, false,
						  ICE_FLTR_RX);
			if (status) {
				netdev_err(netdev, "Error clearing default VSI %i rx rule\n",
					   vsi->vsi_num);
				vsi->current_netdev_flags |= IFF_PROMISC;
				err = -EIO;
				goto out_promisc;
=======
	if (changed_flags & IFF_ALLMULTI) {
		if (vsi->current_netdev_flags & IFF_ALLMULTI) {
			if (vsi->num_vlan > 1)
				promisc_m = ICE_MCAST_VLAN_PROMISC_BITS;
			else
				promisc_m = ICE_MCAST_PROMISC_BITS;

			err = ice_cfg_promisc(vsi, promisc_m, true);
			if (err) {
				netdev_err(netdev, "Error setting Multicast promiscuous mode on VSI %i\n",
					   vsi->vsi_num);
				vsi->current_netdev_flags &= ~IFF_ALLMULTI;
				goto out_promisc;
			}
		} else {
			/* !(vsi->current_netdev_flags & IFF_ALLMULTI) */
			if (vsi->num_vlan > 1)
				promisc_m = ICE_MCAST_VLAN_PROMISC_BITS;
			else
				promisc_m = ICE_MCAST_PROMISC_BITS;

			err = ice_cfg_promisc(vsi, promisc_m, false);
			if (err) {
				netdev_err(netdev, "Error clearing Multicast promiscuous mode on VSI %i\n",
					   vsi->vsi_num);
				vsi->current_netdev_flags |= IFF_ALLMULTI;
				goto out_promisc;
			}
		}
	}

	if (((changed_flags & IFF_PROMISC) || promisc_forced_on) ||
	    test_bit(ICE_VSI_PROMISC_CHANGED, vsi->state)) {
		clear_bit(ICE_VSI_PROMISC_CHANGED, vsi->state);
		if (vsi->current_netdev_flags & IFF_PROMISC) {
			/* Apply Rx filter rule to get traffic from wire */
			if (!ice_is_dflt_vsi_in_use(pf->first_sw)) {
				err = ice_set_dflt_vsi(pf->first_sw, vsi);
				if (err && err != -EEXIST) {
					netdev_err(netdev, "Error %d setting default VSI %i Rx rule\n",
						   err, vsi->vsi_num);
					vsi->current_netdev_flags &=
						~IFF_PROMISC;
					goto out_promisc;
				}
				ice_cfg_vlan_pruning(vsi, false, false);
			}
		} else {
			/* Clear Rx filter to remove traffic from wire */
			if (ice_is_vsi_dflt_vsi(pf->first_sw, vsi)) {
				err = ice_clear_dflt_vsi(pf->first_sw);
				if (err) {
					netdev_err(netdev, "Error %d clearing default VSI %i Rx rule\n",
						   err, vsi->vsi_num);
					vsi->current_netdev_flags |=
						IFF_PROMISC;
					goto out_promisc;
				}
				if (vsi->num_vlan > 1)
					ice_cfg_vlan_pruning(vsi, true, false);
>>>>>>> upstream/android-13
			}
		}
	}
	goto exit;

out_promisc:
<<<<<<< HEAD
	set_bit(ICE_VSI_FLAG_PROMISC_CHANGED, vsi->flags);
	goto exit;
out:
	/* if something went wrong then set the changed flag so we try again */
	set_bit(ICE_VSI_FLAG_UMAC_FLTR_CHANGED, vsi->flags);
	set_bit(ICE_VSI_FLAG_MMAC_FLTR_CHANGED, vsi->flags);
exit:
	clear_bit(__ICE_CFG_BUSY, vsi->state);
=======
	set_bit(ICE_VSI_PROMISC_CHANGED, vsi->state);
	goto exit;
out:
	/* if something went wrong then set the changed flag so we try again */
	set_bit(ICE_VSI_UMAC_FLTR_CHANGED, vsi->state);
	set_bit(ICE_VSI_MMAC_FLTR_CHANGED, vsi->state);
exit:
	clear_bit(ICE_CFG_BUSY, vsi->state);
>>>>>>> upstream/android-13
	return err;
}

/**
 * ice_sync_fltr_subtask - Sync the VSI filter list with HW
 * @pf: board private structure
 */
static void ice_sync_fltr_subtask(struct ice_pf *pf)
{
	int v;

	if (!pf || !(test_bit(ICE_FLAG_FLTR_SYNC, pf->flags)))
		return;

	clear_bit(ICE_FLAG_FLTR_SYNC, pf->flags);

<<<<<<< HEAD
	for (v = 0; v < pf->num_alloc_vsi; v++)
=======
	ice_for_each_vsi(pf, v)
>>>>>>> upstream/android-13
		if (pf->vsi[v] && ice_vsi_fltr_changed(pf->vsi[v]) &&
		    ice_vsi_sync_fltr(pf->vsi[v])) {
			/* come back and try again later */
			set_bit(ICE_FLAG_FLTR_SYNC, pf->flags);
			break;
		}
}

/**
<<<<<<< HEAD
 * ice_is_reset_recovery_pending - schedule a reset
 * @state: pf state field
 */
static bool ice_is_reset_recovery_pending(unsigned long int *state)
{
	return test_bit(__ICE_RESET_RECOVERY_PENDING, state);
=======
 * ice_pf_dis_all_vsi - Pause all VSIs on a PF
 * @pf: the PF
 * @locked: is the rtnl_lock already held
 */
static void ice_pf_dis_all_vsi(struct ice_pf *pf, bool locked)
{
	int node;
	int v;

	ice_for_each_vsi(pf, v)
		if (pf->vsi[v])
			ice_dis_vsi(pf->vsi[v], locked);

	for (node = 0; node < ICE_MAX_PF_AGG_NODES; node++)
		pf->pf_agg_node[node].num_vsis = 0;

	for (node = 0; node < ICE_MAX_VF_AGG_NODES; node++)
		pf->vf_agg_node[node].num_vsis = 0;
>>>>>>> upstream/android-13
}

/**
 * ice_prepare_for_reset - prep for the core to reset
 * @pf: board private structure
 *
 * Inform or close all dependent features in prep for reset.
 */
static void
ice_prepare_for_reset(struct ice_pf *pf)
{
	struct ice_hw *hw = &pf->hw;
<<<<<<< HEAD
	u32 v;

	ice_for_each_vsi(pf, v)
		if (pf->vsi[v])
			ice_remove_vsi_fltr(hw, pf->vsi[v]->vsi_num);

	dev_dbg(&pf->pdev->dev, "Tearing down internal switch for reset\n");

	/* disable the VSIs and their queues that are not already DOWN */
	/* pf_dis_all_vsi modifies netdev structures -rtnl_lock needed */
	ice_pf_dis_all_vsi(pf);

	ice_for_each_vsi(pf, v)
		if (pf->vsi[v])
			pf->vsi[v]->vsi_num = 0;

	ice_shutdown_all_ctrlq(hw);
=======
	unsigned int i;

	/* already prepared for reset */
	if (test_bit(ICE_PREPARED_FOR_RESET, pf->state))
		return;

	ice_unplug_aux_dev(pf);

	/* Notify VFs of impending reset */
	if (ice_check_sq_alive(hw, &hw->mailboxq))
		ice_vc_notify_reset(pf);

	/* Disable VFs until reset is completed */
	ice_for_each_vf(pf, i)
		ice_set_vf_state_qs_dis(&pf->vf[i]);

	/* clear SW filtering DB */
	ice_clear_hw_tbls(hw);
	/* disable the VSIs and their queues that are not already DOWN */
	ice_pf_dis_all_vsi(pf, false);

	if (test_bit(ICE_FLAG_PTP_SUPPORTED, pf->flags))
		ice_ptp_release(pf);

	if (hw->port_info)
		ice_sched_clear_port(hw->port_info);

	ice_shutdown_all_ctrlq(hw);

	set_bit(ICE_PREPARED_FOR_RESET, pf->state);
>>>>>>> upstream/android-13
}

/**
 * ice_do_reset - Initiate one of many types of resets
 * @pf: board private structure
 * @reset_type: reset type requested
 * before this function was called.
 */
static void ice_do_reset(struct ice_pf *pf, enum ice_reset_req reset_type)
{
<<<<<<< HEAD
	struct device *dev = &pf->pdev->dev;
	struct ice_hw *hw = &pf->hw;

	dev_dbg(dev, "reset_type 0x%x requested\n", reset_type);
	WARN_ON(in_interrupt());

	/* PFR is a bit of a special case because it doesn't result in an OICR
	 * interrupt. So for PFR, we prepare for reset, issue the reset and
	 * rebuild sequentially.
	 */
	if (reset_type == ICE_RESET_PFR) {
		set_bit(__ICE_RESET_RECOVERY_PENDING, pf->state);
		ice_prepare_for_reset(pf);
	}
=======
	struct device *dev = ice_pf_to_dev(pf);
	struct ice_hw *hw = &pf->hw;

	dev_dbg(dev, "reset_type 0x%x requested\n", reset_type);

	ice_prepare_for_reset(pf);
>>>>>>> upstream/android-13

	/* trigger the reset */
	if (ice_reset(hw, reset_type)) {
		dev_err(dev, "reset %d failed\n", reset_type);
<<<<<<< HEAD
		set_bit(__ICE_RESET_FAILED, pf->state);
		clear_bit(__ICE_RESET_RECOVERY_PENDING, pf->state);
		return;
	}

	if (reset_type == ICE_RESET_PFR) {
		pf->pfr_count++;
		ice_rebuild(pf);
		clear_bit(__ICE_RESET_RECOVERY_PENDING, pf->state);
=======
		set_bit(ICE_RESET_FAILED, pf->state);
		clear_bit(ICE_RESET_OICR_RECV, pf->state);
		clear_bit(ICE_PREPARED_FOR_RESET, pf->state);
		clear_bit(ICE_PFR_REQ, pf->state);
		clear_bit(ICE_CORER_REQ, pf->state);
		clear_bit(ICE_GLOBR_REQ, pf->state);
		wake_up(&pf->reset_wait_queue);
		return;
	}

	/* PFR is a bit of a special case because it doesn't result in an OICR
	 * interrupt. So for PFR, rebuild after the reset and clear the reset-
	 * associated state bits.
	 */
	if (reset_type == ICE_RESET_PFR) {
		pf->pfr_count++;
		ice_rebuild(pf, reset_type);
		clear_bit(ICE_PREPARED_FOR_RESET, pf->state);
		clear_bit(ICE_PFR_REQ, pf->state);
		wake_up(&pf->reset_wait_queue);
		ice_reset_all_vfs(pf, true);
>>>>>>> upstream/android-13
	}
}

/**
 * ice_reset_subtask - Set up for resetting the device and driver
 * @pf: board private structure
 */
static void ice_reset_subtask(struct ice_pf *pf)
{
<<<<<<< HEAD
	enum ice_reset_req reset_type;

	rtnl_lock();

	/* When a CORER/GLOBR/EMPR is about to happen, the hardware triggers an
	 * OICR interrupt. The OICR handler (ice_misc_intr) determines what
	 * type of reset happened and sets __ICE_RESET_RECOVERY_PENDING bit in
	 * pf->state. So if reset/recovery is pending (as indicated by this bit)
	 * we do a rebuild and return.
	 */
	if (ice_is_reset_recovery_pending(pf->state)) {
		clear_bit(__ICE_GLOBR_RECV, pf->state);
		clear_bit(__ICE_CORER_RECV, pf->state);
=======
	enum ice_reset_req reset_type = ICE_RESET_INVAL;

	/* When a CORER/GLOBR/EMPR is about to happen, the hardware triggers an
	 * OICR interrupt. The OICR handler (ice_misc_intr) determines what type
	 * of reset is pending and sets bits in pf->state indicating the reset
	 * type and ICE_RESET_OICR_RECV. So, if the latter bit is set
	 * prepare for pending reset if not already (for PF software-initiated
	 * global resets the software should already be prepared for it as
	 * indicated by ICE_PREPARED_FOR_RESET; for global resets initiated
	 * by firmware or software on other PFs, that bit is not set so prepare
	 * for the reset now), poll for reset done, rebuild and return.
	 */
	if (test_bit(ICE_RESET_OICR_RECV, pf->state)) {
		/* Perform the largest reset requested */
		if (test_and_clear_bit(ICE_CORER_RECV, pf->state))
			reset_type = ICE_RESET_CORER;
		if (test_and_clear_bit(ICE_GLOBR_RECV, pf->state))
			reset_type = ICE_RESET_GLOBR;
		if (test_and_clear_bit(ICE_EMPR_RECV, pf->state))
			reset_type = ICE_RESET_EMPR;
		/* return if no valid reset type requested */
		if (reset_type == ICE_RESET_INVAL)
			return;
>>>>>>> upstream/android-13
		ice_prepare_for_reset(pf);

		/* make sure we are ready to rebuild */
		if (ice_check_reset(&pf->hw)) {
<<<<<<< HEAD
			set_bit(__ICE_RESET_FAILED, pf->state);
		} else {
			/* done with reset. start rebuild */
			pf->hw.reset_ongoing = false;
			ice_rebuild(pf);
		}
		clear_bit(__ICE_RESET_RECOVERY_PENDING, pf->state);
		goto unlock;
	}

	/* No pending resets to finish processing. Check for new resets */
	if (test_and_clear_bit(__ICE_GLOBR_REQ, pf->state))
		reset_type = ICE_RESET_GLOBR;
	else if (test_and_clear_bit(__ICE_CORER_REQ, pf->state))
		reset_type = ICE_RESET_CORER;
	else if (test_and_clear_bit(__ICE_PFR_REQ, pf->state))
		reset_type = ICE_RESET_PFR;
	else
		goto unlock;

	/* reset if not already down or resetting */
	if (!test_bit(__ICE_DOWN, pf->state) &&
	    !test_bit(__ICE_CFG_BUSY, pf->state)) {
		ice_do_reset(pf, reset_type);
	}

unlock:
	rtnl_unlock();
}

/**
 * ice_watchdog_subtask - periodic tasks not using event driven scheduling
 * @pf: board private structure
 */
static void ice_watchdog_subtask(struct ice_pf *pf)
{
	int i;

	/* if interface is down do nothing */
	if (test_bit(__ICE_DOWN, pf->state) ||
	    test_bit(__ICE_CFG_BUSY, pf->state))
		return;

	/* make sure we don't do these things too often */
	if (time_before(jiffies,
			pf->serv_tmr_prev + pf->serv_tmr_period))
		return;

	pf->serv_tmr_prev = jiffies;

	/* Update the stats for active netdevs so the network stack
	 * can look at updated numbers whenever it cares to
	 */
	ice_update_pf_stats(pf);
	for (i = 0; i < pf->num_alloc_vsi; i++)
		if (pf->vsi[i] && pf->vsi[i]->netdev)
			ice_update_vsi_stats(pf->vsi[i]);
=======
			set_bit(ICE_RESET_FAILED, pf->state);
		} else {
			/* done with reset. start rebuild */
			pf->hw.reset_ongoing = false;
			ice_rebuild(pf, reset_type);
			/* clear bit to resume normal operations, but
			 * ICE_NEEDS_RESTART bit is set in case rebuild failed
			 */
			clear_bit(ICE_RESET_OICR_RECV, pf->state);
			clear_bit(ICE_PREPARED_FOR_RESET, pf->state);
			clear_bit(ICE_PFR_REQ, pf->state);
			clear_bit(ICE_CORER_REQ, pf->state);
			clear_bit(ICE_GLOBR_REQ, pf->state);
			wake_up(&pf->reset_wait_queue);
			ice_reset_all_vfs(pf, true);
		}

		return;
	}

	/* No pending resets to finish processing. Check for new resets */
	if (test_bit(ICE_PFR_REQ, pf->state))
		reset_type = ICE_RESET_PFR;
	if (test_bit(ICE_CORER_REQ, pf->state))
		reset_type = ICE_RESET_CORER;
	if (test_bit(ICE_GLOBR_REQ, pf->state))
		reset_type = ICE_RESET_GLOBR;
	/* If no valid reset type requested just return */
	if (reset_type == ICE_RESET_INVAL)
		return;

	/* reset if not already down or busy */
	if (!test_bit(ICE_DOWN, pf->state) &&
	    !test_bit(ICE_CFG_BUSY, pf->state)) {
		ice_do_reset(pf, reset_type);
	}
}

/**
 * ice_print_topo_conflict - print topology conflict message
 * @vsi: the VSI whose topology status is being checked
 */
static void ice_print_topo_conflict(struct ice_vsi *vsi)
{
	switch (vsi->port_info->phy.link_info.topo_media_conflict) {
	case ICE_AQ_LINK_TOPO_CONFLICT:
	case ICE_AQ_LINK_MEDIA_CONFLICT:
	case ICE_AQ_LINK_TOPO_UNREACH_PRT:
	case ICE_AQ_LINK_TOPO_UNDRUTIL_PRT:
	case ICE_AQ_LINK_TOPO_UNDRUTIL_MEDIA:
		netdev_info(vsi->netdev, "Potential misconfiguration of the Ethernet port detected. If it was not intended, please use the Intel (R) Ethernet Port Configuration Tool to address the issue.\n");
		break;
	case ICE_AQ_LINK_TOPO_UNSUPP_MEDIA:
		netdev_info(vsi->netdev, "Rx/Tx is disabled on this device because an unsupported module type was detected. Refer to the Intel(R) Ethernet Adapters and Devices User Guide for a list of supported modules.\n");
		break;
	default:
		break;
	}
>>>>>>> upstream/android-13
}

/**
 * ice_print_link_msg - print link up or down message
 * @vsi: the VSI whose link status is being queried
 * @isup: boolean for if the link is now up or down
 */
void ice_print_link_msg(struct ice_vsi *vsi, bool isup)
{
<<<<<<< HEAD
	const char *speed;
	const char *fc;
=======
	struct ice_aqc_get_phy_caps_data *caps;
	const char *an_advertised;
	enum ice_status status;
	const char *fec_req;
	const char *speed;
	const char *fec;
	const char *fc;
	const char *an;

	if (!vsi)
		return;
>>>>>>> upstream/android-13

	if (vsi->current_isup == isup)
		return;

	vsi->current_isup = isup;

	if (!isup) {
		netdev_info(vsi->netdev, "NIC Link is Down\n");
		return;
	}

	switch (vsi->port_info->phy.link_info.link_speed) {
<<<<<<< HEAD
=======
	case ICE_AQ_LINK_SPEED_100GB:
		speed = "100 G";
		break;
	case ICE_AQ_LINK_SPEED_50GB:
		speed = "50 G";
		break;
>>>>>>> upstream/android-13
	case ICE_AQ_LINK_SPEED_40GB:
		speed = "40 G";
		break;
	case ICE_AQ_LINK_SPEED_25GB:
		speed = "25 G";
		break;
	case ICE_AQ_LINK_SPEED_20GB:
		speed = "20 G";
		break;
	case ICE_AQ_LINK_SPEED_10GB:
		speed = "10 G";
		break;
	case ICE_AQ_LINK_SPEED_5GB:
		speed = "5 G";
		break;
	case ICE_AQ_LINK_SPEED_2500MB:
		speed = "2.5 G";
		break;
	case ICE_AQ_LINK_SPEED_1000MB:
		speed = "1 G";
		break;
	case ICE_AQ_LINK_SPEED_100MB:
		speed = "100 M";
		break;
	default:
<<<<<<< HEAD
		speed = "Unknown";
=======
		speed = "Unknown ";
>>>>>>> upstream/android-13
		break;
	}

	switch (vsi->port_info->fc.current_mode) {
	case ICE_FC_FULL:
<<<<<<< HEAD
		fc = "RX/TX";
		break;
	case ICE_FC_TX_PAUSE:
		fc = "TX";
		break;
	case ICE_FC_RX_PAUSE:
		fc = "RX";
=======
		fc = "Rx/Tx";
		break;
	case ICE_FC_TX_PAUSE:
		fc = "Tx";
		break;
	case ICE_FC_RX_PAUSE:
		fc = "Rx";
>>>>>>> upstream/android-13
		break;
	case ICE_FC_NONE:
		fc = "None";
		break;
	default:
		fc = "Unknown";
		break;
	}

<<<<<<< HEAD
	netdev_info(vsi->netdev, "NIC Link is up %sbps, Flow Control: %s\n",
		    speed, fc);
=======
	/* Get FEC mode based on negotiated link info */
	switch (vsi->port_info->phy.link_info.fec_info) {
	case ICE_AQ_LINK_25G_RS_528_FEC_EN:
	case ICE_AQ_LINK_25G_RS_544_FEC_EN:
		fec = "RS-FEC";
		break;
	case ICE_AQ_LINK_25G_KR_FEC_EN:
		fec = "FC-FEC/BASE-R";
		break;
	default:
		fec = "NONE";
		break;
	}

	/* check if autoneg completed, might be false due to not supported */
	if (vsi->port_info->phy.link_info.an_info & ICE_AQ_AN_COMPLETED)
		an = "True";
	else
		an = "False";

	/* Get FEC mode requested based on PHY caps last SW configuration */
	caps = kzalloc(sizeof(*caps), GFP_KERNEL);
	if (!caps) {
		fec_req = "Unknown";
		an_advertised = "Unknown";
		goto done;
	}

	status = ice_aq_get_phy_caps(vsi->port_info, false,
				     ICE_AQC_REPORT_ACTIVE_CFG, caps, NULL);
	if (status)
		netdev_info(vsi->netdev, "Get phy capability failed.\n");

	an_advertised = ice_is_phy_caps_an_enabled(caps) ? "On" : "Off";

	if (caps->link_fec_options & ICE_AQC_PHY_FEC_25G_RS_528_REQ ||
	    caps->link_fec_options & ICE_AQC_PHY_FEC_25G_RS_544_REQ)
		fec_req = "RS-FEC";
	else if (caps->link_fec_options & ICE_AQC_PHY_FEC_10G_KR_40G_KR4_REQ ||
		 caps->link_fec_options & ICE_AQC_PHY_FEC_25G_KR_REQ)
		fec_req = "FC-FEC/BASE-R";
	else
		fec_req = "NONE";

	kfree(caps);

done:
	netdev_info(vsi->netdev, "NIC Link is up %sbps Full Duplex, Requested FEC: %s, Negotiated FEC: %s, Autoneg Advertised: %s, Autoneg Negotiated: %s, Flow Control: %s\n",
		    speed, fec_req, fec, an_advertised, an, fc);
	ice_print_topo_conflict(vsi);
}

/**
 * ice_vsi_link_event - update the VSI's netdev
 * @vsi: the VSI on which the link event occurred
 * @link_up: whether or not the VSI needs to be set up or down
 */
static void ice_vsi_link_event(struct ice_vsi *vsi, bool link_up)
{
	if (!vsi)
		return;

	if (test_bit(ICE_VSI_DOWN, vsi->state) || !vsi->netdev)
		return;

	if (vsi->type == ICE_VSI_PF) {
		if (link_up == netif_carrier_ok(vsi->netdev))
			return;

		if (link_up) {
			netif_carrier_on(vsi->netdev);
			netif_tx_wake_all_queues(vsi->netdev);
		} else {
			netif_carrier_off(vsi->netdev);
			netif_tx_stop_all_queues(vsi->netdev);
		}
	}
}

/**
 * ice_set_dflt_mib - send a default config MIB to the FW
 * @pf: private PF struct
 *
 * This function sends a default configuration MIB to the FW.
 *
 * If this function errors out at any point, the driver is still able to
 * function.  The main impact is that LFC may not operate as expected.
 * Therefore an error state in this function should be treated with a DBG
 * message and continue on with driver rebuild/reenable.
 */
static void ice_set_dflt_mib(struct ice_pf *pf)
{
	struct device *dev = ice_pf_to_dev(pf);
	u8 mib_type, *buf, *lldpmib = NULL;
	u16 len, typelen, offset = 0;
	struct ice_lldp_org_tlv *tlv;
	struct ice_hw *hw = &pf->hw;
	u32 ouisubtype;

	mib_type = SET_LOCAL_MIB_TYPE_LOCAL_MIB;
	lldpmib = kzalloc(ICE_LLDPDU_SIZE, GFP_KERNEL);
	if (!lldpmib) {
		dev_dbg(dev, "%s Failed to allocate MIB memory\n",
			__func__);
		return;
	}

	/* Add ETS CFG TLV */
	tlv = (struct ice_lldp_org_tlv *)lldpmib;
	typelen = ((ICE_TLV_TYPE_ORG << ICE_LLDP_TLV_TYPE_S) |
		   ICE_IEEE_ETS_TLV_LEN);
	tlv->typelen = htons(typelen);
	ouisubtype = ((ICE_IEEE_8021QAZ_OUI << ICE_LLDP_TLV_OUI_S) |
		      ICE_IEEE_SUBTYPE_ETS_CFG);
	tlv->ouisubtype = htonl(ouisubtype);

	buf = tlv->tlvinfo;
	buf[0] = 0;

	/* ETS CFG all UPs map to TC 0. Next 4 (1 - 4) Octets = 0.
	 * Octets 5 - 12 are BW values, set octet 5 to 100% BW.
	 * Octets 13 - 20 are TSA values - leave as zeros
	 */
	buf[5] = 0x64;
	len = (typelen & ICE_LLDP_TLV_LEN_M) >> ICE_LLDP_TLV_LEN_S;
	offset += len + 2;
	tlv = (struct ice_lldp_org_tlv *)
		((char *)tlv + sizeof(tlv->typelen) + len);

	/* Add ETS REC TLV */
	buf = tlv->tlvinfo;
	tlv->typelen = htons(typelen);

	ouisubtype = ((ICE_IEEE_8021QAZ_OUI << ICE_LLDP_TLV_OUI_S) |
		      ICE_IEEE_SUBTYPE_ETS_REC);
	tlv->ouisubtype = htonl(ouisubtype);

	/* First octet of buf is reserved
	 * Octets 1 - 4 map UP to TC - all UPs map to zero
	 * Octets 5 - 12 are BW values - set TC 0 to 100%.
	 * Octets 13 - 20 are TSA value - leave as zeros
	 */
	buf[5] = 0x64;
	offset += len + 2;
	tlv = (struct ice_lldp_org_tlv *)
		((char *)tlv + sizeof(tlv->typelen) + len);

	/* Add PFC CFG TLV */
	typelen = ((ICE_TLV_TYPE_ORG << ICE_LLDP_TLV_TYPE_S) |
		   ICE_IEEE_PFC_TLV_LEN);
	tlv->typelen = htons(typelen);

	ouisubtype = ((ICE_IEEE_8021QAZ_OUI << ICE_LLDP_TLV_OUI_S) |
		      ICE_IEEE_SUBTYPE_PFC_CFG);
	tlv->ouisubtype = htonl(ouisubtype);

	/* Octet 1 left as all zeros - PFC disabled */
	buf[0] = 0x08;
	len = (typelen & ICE_LLDP_TLV_LEN_M) >> ICE_LLDP_TLV_LEN_S;
	offset += len + 2;

	if (ice_aq_set_lldp_mib(hw, mib_type, (void *)lldpmib, offset, NULL))
		dev_dbg(dev, "%s Failed to set default LLDP MIB\n", __func__);

	kfree(lldpmib);
}

/**
 * ice_check_module_power
 * @pf: pointer to PF struct
 * @link_cfg_err: bitmap from the link info structure
 *
 * check module power level returned by a previous call to aq_get_link_info
 * and print error messages if module power level is not supported
 */
static void ice_check_module_power(struct ice_pf *pf, u8 link_cfg_err)
{
	/* if module power level is supported, clear the flag */
	if (!(link_cfg_err & (ICE_AQ_LINK_INVAL_MAX_POWER_LIMIT |
			      ICE_AQ_LINK_MODULE_POWER_UNSUPPORTED))) {
		clear_bit(ICE_FLAG_MOD_POWER_UNSUPPORTED, pf->flags);
		return;
	}

	/* if ICE_FLAG_MOD_POWER_UNSUPPORTED was previously set and the
	 * above block didn't clear this bit, there's nothing to do
	 */
	if (test_bit(ICE_FLAG_MOD_POWER_UNSUPPORTED, pf->flags))
		return;

	if (link_cfg_err & ICE_AQ_LINK_INVAL_MAX_POWER_LIMIT) {
		dev_err(ice_pf_to_dev(pf), "The installed module is incompatible with the device's NVM image. Cannot start link\n");
		set_bit(ICE_FLAG_MOD_POWER_UNSUPPORTED, pf->flags);
	} else if (link_cfg_err & ICE_AQ_LINK_MODULE_POWER_UNSUPPORTED) {
		dev_err(ice_pf_to_dev(pf), "The module's power requirements exceed the device's power supply. Cannot start link\n");
		set_bit(ICE_FLAG_MOD_POWER_UNSUPPORTED, pf->flags);
	}
}

/**
 * ice_link_event - process the link event
 * @pf: PF that the link event is associated with
 * @pi: port_info for the port that the link event is associated with
 * @link_up: true if the physical link is up and false if it is down
 * @link_speed: current link speed received from the link event
 *
 * Returns 0 on success and negative on failure
 */
static int
ice_link_event(struct ice_pf *pf, struct ice_port_info *pi, bool link_up,
	       u16 link_speed)
{
	struct device *dev = ice_pf_to_dev(pf);
	struct ice_phy_info *phy_info;
	enum ice_status status;
	struct ice_vsi *vsi;
	u16 old_link_speed;
	bool old_link;

	phy_info = &pi->phy;
	phy_info->link_info_old = phy_info->link_info;

	old_link = !!(phy_info->link_info_old.link_info & ICE_AQ_LINK_UP);
	old_link_speed = phy_info->link_info_old.link_speed;

	/* update the link info structures and re-enable link events,
	 * don't bail on failure due to other book keeping needed
	 */
	status = ice_update_link_info(pi);
	if (status)
		dev_dbg(dev, "Failed to update link status on port %d, err %s aq_err %s\n",
			pi->lport, ice_stat_str(status),
			ice_aq_str(pi->hw->adminq.sq_last_status));

	ice_check_module_power(pf, pi->phy.link_info.link_cfg_err);

	/* Check if the link state is up after updating link info, and treat
	 * this event as an UP event since the link is actually UP now.
	 */
	if (phy_info->link_info.link_info & ICE_AQ_LINK_UP)
		link_up = true;

	vsi = ice_get_main_vsi(pf);
	if (!vsi || !vsi->port_info)
		return -EINVAL;

	/* turn off PHY if media was removed */
	if (!test_bit(ICE_FLAG_NO_MEDIA, pf->flags) &&
	    !(pi->phy.link_info.link_info & ICE_AQ_MEDIA_AVAILABLE)) {
		set_bit(ICE_FLAG_NO_MEDIA, pf->flags);
		ice_set_link(vsi, false);
	}

	/* if the old link up/down and speed is the same as the new */
	if (link_up == old_link && link_speed == old_link_speed)
		return 0;

	if (ice_is_dcb_active(pf)) {
		if (test_bit(ICE_FLAG_DCB_ENA, pf->flags))
			ice_dcb_rebuild(pf);
	} else {
		if (link_up)
			ice_set_dflt_mib(pf);
	}
	ice_vsi_link_event(vsi, link_up);
	ice_print_link_msg(vsi, link_up);

	ice_vc_notify_link_state(pf);

	return 0;
}

/**
 * ice_watchdog_subtask - periodic tasks not using event driven scheduling
 * @pf: board private structure
 */
static void ice_watchdog_subtask(struct ice_pf *pf)
{
	int i;

	/* if interface is down do nothing */
	if (test_bit(ICE_DOWN, pf->state) ||
	    test_bit(ICE_CFG_BUSY, pf->state))
		return;

	/* make sure we don't do these things too often */
	if (time_before(jiffies,
			pf->serv_tmr_prev + pf->serv_tmr_period))
		return;

	pf->serv_tmr_prev = jiffies;

	/* Update the stats for active netdevs so the network stack
	 * can look at updated numbers whenever it cares to
	 */
	ice_update_pf_stats(pf);
	ice_for_each_vsi(pf, i)
		if (pf->vsi[i] && pf->vsi[i]->netdev)
			ice_update_vsi_stats(pf->vsi[i]);
>>>>>>> upstream/android-13
}

/**
 * ice_init_link_events - enable/initialize link events
 * @pi: pointer to the port_info instance
 *
 * Returns -EIO on failure, 0 on success
 */
static int ice_init_link_events(struct ice_port_info *pi)
{
	u16 mask;

	mask = ~((u16)(ICE_AQ_LINK_EVENT_UPDOWN | ICE_AQ_LINK_EVENT_MEDIA_NA |
		       ICE_AQ_LINK_EVENT_MODULE_QUAL_FAIL));

	if (ice_aq_set_event_mask(pi->hw, pi->lport, mask, NULL)) {
<<<<<<< HEAD
		dev_dbg(ice_hw_to_dev(pi->hw),
			"Failed to set link event mask for port %d\n",
=======
		dev_dbg(ice_hw_to_dev(pi->hw), "Failed to set link event mask for port %d\n",
>>>>>>> upstream/android-13
			pi->lport);
		return -EIO;
	}

	if (ice_aq_get_link_info(pi, true, NULL, NULL)) {
<<<<<<< HEAD
		dev_dbg(ice_hw_to_dev(pi->hw),
			"Failed to enable link events for port %d\n",
=======
		dev_dbg(ice_hw_to_dev(pi->hw), "Failed to enable link events for port %d\n",
>>>>>>> upstream/android-13
			pi->lport);
		return -EIO;
	}

	return 0;
}

/**
<<<<<<< HEAD
 * ice_vsi_link_event - update the vsi's netdev
 * @vsi: the vsi on which the link event occurred
 * @link_up: whether or not the vsi needs to be set up or down
 */
static void ice_vsi_link_event(struct ice_vsi *vsi, bool link_up)
{
	if (!vsi || test_bit(__ICE_DOWN, vsi->state))
		return;

	if (vsi->type == ICE_VSI_PF) {
		if (!vsi->netdev) {
			dev_dbg(&vsi->back->pdev->dev,
				"vsi->netdev is not initialized!\n");
			return;
		}
		if (link_up) {
			netif_carrier_on(vsi->netdev);
			netif_tx_wake_all_queues(vsi->netdev);
		} else {
			netif_carrier_off(vsi->netdev);
			netif_tx_stop_all_queues(vsi->netdev);
		}
	}
}

/**
 * ice_link_event - process the link event
 * @pf: pf that the link event is associated with
 * @pi: port_info for the port that the link event is associated with
 *
 * Returns -EIO if ice_get_link_status() fails
 * Returns 0 on success
 */
static int
ice_link_event(struct ice_pf *pf, struct ice_port_info *pi)
{
	u8 new_link_speed, old_link_speed;
	struct ice_phy_info *phy_info;
	bool new_link_same_as_old;
	bool new_link, old_link;
	u8 lport;
	u16 v;

	phy_info = &pi->phy;
	phy_info->link_info_old = phy_info->link_info;
	/* Force ice_get_link_status() to update link info */
	phy_info->get_link_info = true;

	old_link = (phy_info->link_info_old.link_info & ICE_AQ_LINK_UP);
	old_link_speed = phy_info->link_info_old.link_speed;

	lport = pi->lport;
	if (ice_get_link_status(pi, &new_link)) {
		dev_dbg(&pf->pdev->dev,
			"Could not get link status for port %d\n", lport);
		return -EIO;
	}

	new_link_speed = phy_info->link_info.link_speed;

	new_link_same_as_old = (new_link == old_link &&
				new_link_speed == old_link_speed);

	ice_for_each_vsi(pf, v) {
		struct ice_vsi *vsi = pf->vsi[v];

		if (!vsi || !vsi->port_info)
			continue;

		if (new_link_same_as_old &&
		    (test_bit(__ICE_DOWN, vsi->state) ||
		    new_link == netif_carrier_ok(vsi->netdev)))
			continue;

		if (vsi->port_info->lport == lport) {
			ice_print_link_msg(vsi, new_link);
			ice_vsi_link_event(vsi, new_link);
		}
	}

	return 0;
}

/**
 * ice_handle_link_event - handle link event via ARQ
 * @pf: pf that the link event is associated with
 *
 * Return -EINVAL if port_info is null
 * Return status on succes
 */
static int ice_handle_link_event(struct ice_pf *pf)
{
	struct ice_port_info *port_info;
	int status;

=======
 * ice_handle_link_event - handle link event via ARQ
 * @pf: PF that the link event is associated with
 * @event: event structure containing link status info
 */
static int
ice_handle_link_event(struct ice_pf *pf, struct ice_rq_event_info *event)
{
	struct ice_aqc_get_link_status_data *link_data;
	struct ice_port_info *port_info;
	int status;

	link_data = (struct ice_aqc_get_link_status_data *)event->msg_buf;
>>>>>>> upstream/android-13
	port_info = pf->hw.port_info;
	if (!port_info)
		return -EINVAL;

<<<<<<< HEAD
	status = ice_link_event(pf, port_info);
	if (status)
		dev_dbg(&pf->pdev->dev,
			"Could not process link event, error %d\n", status);
=======
	status = ice_link_event(pf, port_info,
				!!(link_data->link_info & ICE_AQ_LINK_UP),
				le16_to_cpu(link_data->link_speed));
	if (status)
		dev_dbg(ice_pf_to_dev(pf), "Could not process link event, error %d\n",
			status);
>>>>>>> upstream/android-13

	return status;
}

<<<<<<< HEAD
=======
enum ice_aq_task_state {
	ICE_AQ_TASK_WAITING = 0,
	ICE_AQ_TASK_COMPLETE,
	ICE_AQ_TASK_CANCELED,
};

struct ice_aq_task {
	struct hlist_node entry;

	u16 opcode;
	struct ice_rq_event_info *event;
	enum ice_aq_task_state state;
};

/**
 * ice_aq_wait_for_event - Wait for an AdminQ event from firmware
 * @pf: pointer to the PF private structure
 * @opcode: the opcode to wait for
 * @timeout: how long to wait, in jiffies
 * @event: storage for the event info
 *
 * Waits for a specific AdminQ completion event on the ARQ for a given PF. The
 * current thread will be put to sleep until the specified event occurs or
 * until the given timeout is reached.
 *
 * To obtain only the descriptor contents, pass an event without an allocated
 * msg_buf. If the complete data buffer is desired, allocate the
 * event->msg_buf with enough space ahead of time.
 *
 * Returns: zero on success, or a negative error code on failure.
 */
int ice_aq_wait_for_event(struct ice_pf *pf, u16 opcode, unsigned long timeout,
			  struct ice_rq_event_info *event)
{
	struct device *dev = ice_pf_to_dev(pf);
	struct ice_aq_task *task;
	unsigned long start;
	long ret;
	int err;

	task = kzalloc(sizeof(*task), GFP_KERNEL);
	if (!task)
		return -ENOMEM;

	INIT_HLIST_NODE(&task->entry);
	task->opcode = opcode;
	task->event = event;
	task->state = ICE_AQ_TASK_WAITING;

	spin_lock_bh(&pf->aq_wait_lock);
	hlist_add_head(&task->entry, &pf->aq_wait_list);
	spin_unlock_bh(&pf->aq_wait_lock);

	start = jiffies;

	ret = wait_event_interruptible_timeout(pf->aq_wait_queue, task->state,
					       timeout);
	switch (task->state) {
	case ICE_AQ_TASK_WAITING:
		err = ret < 0 ? ret : -ETIMEDOUT;
		break;
	case ICE_AQ_TASK_CANCELED:
		err = ret < 0 ? ret : -ECANCELED;
		break;
	case ICE_AQ_TASK_COMPLETE:
		err = ret < 0 ? ret : 0;
		break;
	default:
		WARN(1, "Unexpected AdminQ wait task state %u", task->state);
		err = -EINVAL;
		break;
	}

	dev_dbg(dev, "Waited %u msecs (max %u msecs) for firmware response to op 0x%04x\n",
		jiffies_to_msecs(jiffies - start),
		jiffies_to_msecs(timeout),
		opcode);

	spin_lock_bh(&pf->aq_wait_lock);
	hlist_del(&task->entry);
	spin_unlock_bh(&pf->aq_wait_lock);
	kfree(task);

	return err;
}

/**
 * ice_aq_check_events - Check if any thread is waiting for an AdminQ event
 * @pf: pointer to the PF private structure
 * @opcode: the opcode of the event
 * @event: the event to check
 *
 * Loops over the current list of pending threads waiting for an AdminQ event.
 * For each matching task, copy the contents of the event into the task
 * structure and wake up the thread.
 *
 * If multiple threads wait for the same opcode, they will all be woken up.
 *
 * Note that event->msg_buf will only be duplicated if the event has a buffer
 * with enough space already allocated. Otherwise, only the descriptor and
 * message length will be copied.
 *
 * Returns: true if an event was found, false otherwise
 */
static void ice_aq_check_events(struct ice_pf *pf, u16 opcode,
				struct ice_rq_event_info *event)
{
	struct ice_aq_task *task;
	bool found = false;

	spin_lock_bh(&pf->aq_wait_lock);
	hlist_for_each_entry(task, &pf->aq_wait_list, entry) {
		if (task->state || task->opcode != opcode)
			continue;

		memcpy(&task->event->desc, &event->desc, sizeof(event->desc));
		task->event->msg_len = event->msg_len;

		/* Only copy the data buffer if a destination was set */
		if (task->event->msg_buf &&
		    task->event->buf_len > event->buf_len) {
			memcpy(task->event->msg_buf, event->msg_buf,
			       event->buf_len);
			task->event->buf_len = event->buf_len;
		}

		task->state = ICE_AQ_TASK_COMPLETE;
		found = true;
	}
	spin_unlock_bh(&pf->aq_wait_lock);

	if (found)
		wake_up(&pf->aq_wait_queue);
}

/**
 * ice_aq_cancel_waiting_tasks - Immediately cancel all waiting tasks
 * @pf: the PF private structure
 *
 * Set all waiting tasks to ICE_AQ_TASK_CANCELED, and wake up their threads.
 * This will then cause ice_aq_wait_for_event to exit with -ECANCELED.
 */
static void ice_aq_cancel_waiting_tasks(struct ice_pf *pf)
{
	struct ice_aq_task *task;

	spin_lock_bh(&pf->aq_wait_lock);
	hlist_for_each_entry(task, &pf->aq_wait_list, entry)
		task->state = ICE_AQ_TASK_CANCELED;
	spin_unlock_bh(&pf->aq_wait_lock);

	wake_up(&pf->aq_wait_queue);
}

>>>>>>> upstream/android-13
/**
 * __ice_clean_ctrlq - helper function to clean controlq rings
 * @pf: ptr to struct ice_pf
 * @q_type: specific Control queue type
 */
static int __ice_clean_ctrlq(struct ice_pf *pf, enum ice_ctl_q q_type)
{
<<<<<<< HEAD
=======
	struct device *dev = ice_pf_to_dev(pf);
>>>>>>> upstream/android-13
	struct ice_rq_event_info event;
	struct ice_hw *hw = &pf->hw;
	struct ice_ctl_q_info *cq;
	u16 pending, i = 0;
	const char *qtype;
	u32 oldval, val;

	/* Do not clean control queue if/when PF reset fails */
<<<<<<< HEAD
	if (test_bit(__ICE_RESET_FAILED, pf->state))
=======
	if (test_bit(ICE_RESET_FAILED, pf->state))
>>>>>>> upstream/android-13
		return 0;

	switch (q_type) {
	case ICE_CTL_Q_ADMIN:
		cq = &hw->adminq;
		qtype = "Admin";
		break;
<<<<<<< HEAD
	default:
		dev_warn(&pf->pdev->dev, "Unknown control queue type 0x%x\n",
			 q_type);
=======
	case ICE_CTL_Q_SB:
		cq = &hw->sbq;
		qtype = "Sideband";
		break;
	case ICE_CTL_Q_MAILBOX:
		cq = &hw->mailboxq;
		qtype = "Mailbox";
		/* we are going to try to detect a malicious VF, so set the
		 * state to begin detection
		 */
		hw->mbx_snapshot.mbx_buf.state = ICE_MAL_VF_DETECT_STATE_NEW_SNAPSHOT;
		break;
	default:
		dev_warn(dev, "Unknown control queue type 0x%x\n", q_type);
>>>>>>> upstream/android-13
		return 0;
	}

	/* check for error indications - PF_xx_AxQLEN register layout for
	 * FW/MBX/SB are identical so just use defines for PF_FW_AxQLEN.
	 */
	val = rd32(hw, cq->rq.len);
	if (val & (PF_FW_ARQLEN_ARQVFE_M | PF_FW_ARQLEN_ARQOVFL_M |
		   PF_FW_ARQLEN_ARQCRIT_M)) {
		oldval = val;
		if (val & PF_FW_ARQLEN_ARQVFE_M)
<<<<<<< HEAD
			dev_dbg(&pf->pdev->dev,
				"%s Receive Queue VF Error detected\n", qtype);
		if (val & PF_FW_ARQLEN_ARQOVFL_M) {
			dev_dbg(&pf->pdev->dev,
				"%s Receive Queue Overflow Error detected\n",
				qtype);
		}
		if (val & PF_FW_ARQLEN_ARQCRIT_M)
			dev_dbg(&pf->pdev->dev,
				"%s Receive Queue Critical Error detected\n",
=======
			dev_dbg(dev, "%s Receive Queue VF Error detected\n",
				qtype);
		if (val & PF_FW_ARQLEN_ARQOVFL_M) {
			dev_dbg(dev, "%s Receive Queue Overflow Error detected\n",
				qtype);
		}
		if (val & PF_FW_ARQLEN_ARQCRIT_M)
			dev_dbg(dev, "%s Receive Queue Critical Error detected\n",
>>>>>>> upstream/android-13
				qtype);
		val &= ~(PF_FW_ARQLEN_ARQVFE_M | PF_FW_ARQLEN_ARQOVFL_M |
			 PF_FW_ARQLEN_ARQCRIT_M);
		if (oldval != val)
			wr32(hw, cq->rq.len, val);
	}

	val = rd32(hw, cq->sq.len);
	if (val & (PF_FW_ATQLEN_ATQVFE_M | PF_FW_ATQLEN_ATQOVFL_M |
		   PF_FW_ATQLEN_ATQCRIT_M)) {
		oldval = val;
		if (val & PF_FW_ATQLEN_ATQVFE_M)
<<<<<<< HEAD
			dev_dbg(&pf->pdev->dev,
				"%s Send Queue VF Error detected\n", qtype);
		if (val & PF_FW_ATQLEN_ATQOVFL_M) {
			dev_dbg(&pf->pdev->dev,
				"%s Send Queue Overflow Error detected\n",
				qtype);
		}
		if (val & PF_FW_ATQLEN_ATQCRIT_M)
			dev_dbg(&pf->pdev->dev,
				"%s Send Queue Critical Error detected\n",
=======
			dev_dbg(dev, "%s Send Queue VF Error detected\n",
				qtype);
		if (val & PF_FW_ATQLEN_ATQOVFL_M) {
			dev_dbg(dev, "%s Send Queue Overflow Error detected\n",
				qtype);
		}
		if (val & PF_FW_ATQLEN_ATQCRIT_M)
			dev_dbg(dev, "%s Send Queue Critical Error detected\n",
>>>>>>> upstream/android-13
				qtype);
		val &= ~(PF_FW_ATQLEN_ATQVFE_M | PF_FW_ATQLEN_ATQOVFL_M |
			 PF_FW_ATQLEN_ATQCRIT_M);
		if (oldval != val)
			wr32(hw, cq->sq.len, val);
	}

	event.buf_len = cq->rq_buf_size;
<<<<<<< HEAD
	event.msg_buf = devm_kzalloc(&pf->pdev->dev, event.buf_len,
				     GFP_KERNEL);
=======
	event.msg_buf = kzalloc(event.buf_len, GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!event.msg_buf)
		return 0;

	do {
		enum ice_status ret;
		u16 opcode;

		ret = ice_clean_rq_elem(hw, cq, &event, &pending);
		if (ret == ICE_ERR_AQ_NO_WORK)
			break;
		if (ret) {
<<<<<<< HEAD
			dev_err(&pf->pdev->dev,
				"%s Receive Queue event error %d\n", qtype,
				ret);
=======
			dev_err(dev, "%s Receive Queue event error %s\n", qtype,
				ice_stat_str(ret));
>>>>>>> upstream/android-13
			break;
		}

		opcode = le16_to_cpu(event.desc.opcode);

<<<<<<< HEAD
		switch (opcode) {
		case ice_aqc_opc_get_link_status:
			if (ice_handle_link_event(pf))
				dev_err(&pf->pdev->dev,
					"Could not handle link event\n");
			break;
		default:
			dev_dbg(&pf->pdev->dev,
				"%s Receive Queue unknown event 0x%04x ignored\n",
=======
		/* Notify any thread that might be waiting for this event */
		ice_aq_check_events(pf, opcode, &event);

		switch (opcode) {
		case ice_aqc_opc_get_link_status:
			if (ice_handle_link_event(pf, &event))
				dev_err(dev, "Could not handle link event\n");
			break;
		case ice_aqc_opc_event_lan_overflow:
			ice_vf_lan_overflow_event(pf, &event);
			break;
		case ice_mbx_opc_send_msg_to_pf:
			if (!ice_is_malicious_vf(pf, &event, i, pending))
				ice_vc_process_vf_msg(pf, &event);
			break;
		case ice_aqc_opc_fw_logging:
			ice_output_fw_log(hw, &event.desc, event.msg_buf);
			break;
		case ice_aqc_opc_lldp_set_mib_change:
			ice_dcb_process_lldp_set_mib_change(pf, &event);
			break;
		default:
			dev_dbg(dev, "%s Receive Queue unknown event 0x%04x ignored\n",
>>>>>>> upstream/android-13
				qtype, opcode);
			break;
		}
	} while (pending && (i++ < ICE_DFLT_IRQ_WORK));

<<<<<<< HEAD
	devm_kfree(&pf->pdev->dev, event.msg_buf);
=======
	kfree(event.msg_buf);
>>>>>>> upstream/android-13

	return pending && (i == ICE_DFLT_IRQ_WORK);
}

/**
 * ice_ctrlq_pending - check if there is a difference between ntc and ntu
 * @hw: pointer to hardware info
 * @cq: control queue information
 *
 * returns true if there are pending messages in a queue, false if there aren't
 */
static bool ice_ctrlq_pending(struct ice_hw *hw, struct ice_ctl_q_info *cq)
{
	u16 ntu;

	ntu = (u16)(rd32(hw, cq->rq.head) & cq->rq.head_mask);
	return cq->rq.next_to_clean != ntu;
}

/**
 * ice_clean_adminq_subtask - clean the AdminQ rings
 * @pf: board private structure
 */
static void ice_clean_adminq_subtask(struct ice_pf *pf)
{
	struct ice_hw *hw = &pf->hw;

<<<<<<< HEAD
	if (!test_bit(__ICE_ADMINQ_EVENT_PENDING, pf->state))
=======
	if (!test_bit(ICE_ADMINQ_EVENT_PENDING, pf->state))
>>>>>>> upstream/android-13
		return;

	if (__ice_clean_ctrlq(pf, ICE_CTL_Q_ADMIN))
		return;

<<<<<<< HEAD
	clear_bit(__ICE_ADMINQ_EVENT_PENDING, pf->state);
=======
	clear_bit(ICE_ADMINQ_EVENT_PENDING, pf->state);
>>>>>>> upstream/android-13

	/* There might be a situation where new messages arrive to a control
	 * queue between processing the last message and clearing the
	 * EVENT_PENDING bit. So before exiting, check queue head again (using
	 * ice_ctrlq_pending) and process new messages if any.
	 */
	if (ice_ctrlq_pending(hw, &hw->adminq))
		__ice_clean_ctrlq(pf, ICE_CTL_Q_ADMIN);

	ice_flush(hw);
}

/**
<<<<<<< HEAD
=======
 * ice_clean_mailboxq_subtask - clean the MailboxQ rings
 * @pf: board private structure
 */
static void ice_clean_mailboxq_subtask(struct ice_pf *pf)
{
	struct ice_hw *hw = &pf->hw;

	if (!test_bit(ICE_MAILBOXQ_EVENT_PENDING, pf->state))
		return;

	if (__ice_clean_ctrlq(pf, ICE_CTL_Q_MAILBOX))
		return;

	clear_bit(ICE_MAILBOXQ_EVENT_PENDING, pf->state);

	if (ice_ctrlq_pending(hw, &hw->mailboxq))
		__ice_clean_ctrlq(pf, ICE_CTL_Q_MAILBOX);

	ice_flush(hw);
}

/**
 * ice_clean_sbq_subtask - clean the Sideband Queue rings
 * @pf: board private structure
 */
static void ice_clean_sbq_subtask(struct ice_pf *pf)
{
	struct ice_hw *hw = &pf->hw;

	/* Nothing to do here if sideband queue is not supported */
	if (!ice_is_sbq_supported(hw)) {
		clear_bit(ICE_SIDEBANDQ_EVENT_PENDING, pf->state);
		return;
	}

	if (!test_bit(ICE_SIDEBANDQ_EVENT_PENDING, pf->state))
		return;

	if (__ice_clean_ctrlq(pf, ICE_CTL_Q_SB))
		return;

	clear_bit(ICE_SIDEBANDQ_EVENT_PENDING, pf->state);

	if (ice_ctrlq_pending(hw, &hw->sbq))
		__ice_clean_ctrlq(pf, ICE_CTL_Q_SB);

	ice_flush(hw);
}

/**
>>>>>>> upstream/android-13
 * ice_service_task_schedule - schedule the service task to wake up
 * @pf: board private structure
 *
 * If not already scheduled, this puts the task into the work queue.
 */
<<<<<<< HEAD
static void ice_service_task_schedule(struct ice_pf *pf)
{
	if (!test_bit(__ICE_DOWN, pf->state) &&
	    !test_and_set_bit(__ICE_SERVICE_SCHED, pf->state))
=======
void ice_service_task_schedule(struct ice_pf *pf)
{
	if (!test_bit(ICE_SERVICE_DIS, pf->state) &&
	    !test_and_set_bit(ICE_SERVICE_SCHED, pf->state) &&
	    !test_bit(ICE_NEEDS_RESTART, pf->state))
>>>>>>> upstream/android-13
		queue_work(ice_wq, &pf->serv_task);
}

/**
 * ice_service_task_complete - finish up the service task
 * @pf: board private structure
 */
static void ice_service_task_complete(struct ice_pf *pf)
{
<<<<<<< HEAD
	WARN_ON(!test_bit(__ICE_SERVICE_SCHED, pf->state));

	/* force memory (pf->state) to sync before next service task */
	smp_mb__before_atomic();
	clear_bit(__ICE_SERVICE_SCHED, pf->state);
=======
	WARN_ON(!test_bit(ICE_SERVICE_SCHED, pf->state));

	/* force memory (pf->state) to sync before next service task */
	smp_mb__before_atomic();
	clear_bit(ICE_SERVICE_SCHED, pf->state);
}

/**
 * ice_service_task_stop - stop service task and cancel works
 * @pf: board private structure
 *
 * Return 0 if the ICE_SERVICE_DIS bit was not already set,
 * 1 otherwise.
 */
static int ice_service_task_stop(struct ice_pf *pf)
{
	int ret;

	ret = test_and_set_bit(ICE_SERVICE_DIS, pf->state);

	if (pf->serv_tmr.function)
		del_timer_sync(&pf->serv_tmr);
	if (pf->serv_task.func)
		cancel_work_sync(&pf->serv_task);

	clear_bit(ICE_SERVICE_SCHED, pf->state);
	return ret;
}

/**
 * ice_service_task_restart - restart service task and schedule works
 * @pf: board private structure
 *
 * This function is needed for suspend and resume works (e.g WoL scenario)
 */
static void ice_service_task_restart(struct ice_pf *pf)
{
	clear_bit(ICE_SERVICE_DIS, pf->state);
	ice_service_task_schedule(pf);
>>>>>>> upstream/android-13
}

/**
 * ice_service_timer - timer callback to schedule service task
 * @t: pointer to timer_list
 */
static void ice_service_timer(struct timer_list *t)
{
	struct ice_pf *pf = from_timer(pf, t, serv_tmr);

	mod_timer(&pf->serv_tmr, round_jiffies(pf->serv_tmr_period + jiffies));
	ice_service_task_schedule(pf);
}

/**
<<<<<<< HEAD
=======
 * ice_handle_mdd_event - handle malicious driver detect event
 * @pf: pointer to the PF structure
 *
 * Called from service task. OICR interrupt handler indicates MDD event.
 * VF MDD logging is guarded by net_ratelimit. Additional PF and VF log
 * messages are wrapped by netif_msg_[rx|tx]_err. Since VF Rx MDD events
 * disable the queue, the PF can be configured to reset the VF using ethtool
 * private flag mdd-auto-reset-vf.
 */
static void ice_handle_mdd_event(struct ice_pf *pf)
{
	struct device *dev = ice_pf_to_dev(pf);
	struct ice_hw *hw = &pf->hw;
	unsigned int i;
	u32 reg;

	if (!test_and_clear_bit(ICE_MDD_EVENT_PENDING, pf->state)) {
		/* Since the VF MDD event logging is rate limited, check if
		 * there are pending MDD events.
		 */
		ice_print_vfs_mdd_events(pf);
		return;
	}

	/* find what triggered an MDD event */
	reg = rd32(hw, GL_MDET_TX_PQM);
	if (reg & GL_MDET_TX_PQM_VALID_M) {
		u8 pf_num = (reg & GL_MDET_TX_PQM_PF_NUM_M) >>
				GL_MDET_TX_PQM_PF_NUM_S;
		u16 vf_num = (reg & GL_MDET_TX_PQM_VF_NUM_M) >>
				GL_MDET_TX_PQM_VF_NUM_S;
		u8 event = (reg & GL_MDET_TX_PQM_MAL_TYPE_M) >>
				GL_MDET_TX_PQM_MAL_TYPE_S;
		u16 queue = ((reg & GL_MDET_TX_PQM_QNUM_M) >>
				GL_MDET_TX_PQM_QNUM_S);

		if (netif_msg_tx_err(pf))
			dev_info(dev, "Malicious Driver Detection event %d on TX queue %d PF# %d VF# %d\n",
				 event, queue, pf_num, vf_num);
		wr32(hw, GL_MDET_TX_PQM, 0xffffffff);
	}

	reg = rd32(hw, GL_MDET_TX_TCLAN);
	if (reg & GL_MDET_TX_TCLAN_VALID_M) {
		u8 pf_num = (reg & GL_MDET_TX_TCLAN_PF_NUM_M) >>
				GL_MDET_TX_TCLAN_PF_NUM_S;
		u16 vf_num = (reg & GL_MDET_TX_TCLAN_VF_NUM_M) >>
				GL_MDET_TX_TCLAN_VF_NUM_S;
		u8 event = (reg & GL_MDET_TX_TCLAN_MAL_TYPE_M) >>
				GL_MDET_TX_TCLAN_MAL_TYPE_S;
		u16 queue = ((reg & GL_MDET_TX_TCLAN_QNUM_M) >>
				GL_MDET_TX_TCLAN_QNUM_S);

		if (netif_msg_tx_err(pf))
			dev_info(dev, "Malicious Driver Detection event %d on TX queue %d PF# %d VF# %d\n",
				 event, queue, pf_num, vf_num);
		wr32(hw, GL_MDET_TX_TCLAN, 0xffffffff);
	}

	reg = rd32(hw, GL_MDET_RX);
	if (reg & GL_MDET_RX_VALID_M) {
		u8 pf_num = (reg & GL_MDET_RX_PF_NUM_M) >>
				GL_MDET_RX_PF_NUM_S;
		u16 vf_num = (reg & GL_MDET_RX_VF_NUM_M) >>
				GL_MDET_RX_VF_NUM_S;
		u8 event = (reg & GL_MDET_RX_MAL_TYPE_M) >>
				GL_MDET_RX_MAL_TYPE_S;
		u16 queue = ((reg & GL_MDET_RX_QNUM_M) >>
				GL_MDET_RX_QNUM_S);

		if (netif_msg_rx_err(pf))
			dev_info(dev, "Malicious Driver Detection event %d on RX queue %d PF# %d VF# %d\n",
				 event, queue, pf_num, vf_num);
		wr32(hw, GL_MDET_RX, 0xffffffff);
	}

	/* check to see if this PF caused an MDD event */
	reg = rd32(hw, PF_MDET_TX_PQM);
	if (reg & PF_MDET_TX_PQM_VALID_M) {
		wr32(hw, PF_MDET_TX_PQM, 0xFFFF);
		if (netif_msg_tx_err(pf))
			dev_info(dev, "Malicious Driver Detection event TX_PQM detected on PF\n");
	}

	reg = rd32(hw, PF_MDET_TX_TCLAN);
	if (reg & PF_MDET_TX_TCLAN_VALID_M) {
		wr32(hw, PF_MDET_TX_TCLAN, 0xFFFF);
		if (netif_msg_tx_err(pf))
			dev_info(dev, "Malicious Driver Detection event TX_TCLAN detected on PF\n");
	}

	reg = rd32(hw, PF_MDET_RX);
	if (reg & PF_MDET_RX_VALID_M) {
		wr32(hw, PF_MDET_RX, 0xFFFF);
		if (netif_msg_rx_err(pf))
			dev_info(dev, "Malicious Driver Detection event RX detected on PF\n");
	}

	/* Check to see if one of the VFs caused an MDD event, and then
	 * increment counters and set print pending
	 */
	ice_for_each_vf(pf, i) {
		struct ice_vf *vf = &pf->vf[i];

		reg = rd32(hw, VP_MDET_TX_PQM(i));
		if (reg & VP_MDET_TX_PQM_VALID_M) {
			wr32(hw, VP_MDET_TX_PQM(i), 0xFFFF);
			vf->mdd_tx_events.count++;
			set_bit(ICE_MDD_VF_PRINT_PENDING, pf->state);
			if (netif_msg_tx_err(pf))
				dev_info(dev, "Malicious Driver Detection event TX_PQM detected on VF %d\n",
					 i);
		}

		reg = rd32(hw, VP_MDET_TX_TCLAN(i));
		if (reg & VP_MDET_TX_TCLAN_VALID_M) {
			wr32(hw, VP_MDET_TX_TCLAN(i), 0xFFFF);
			vf->mdd_tx_events.count++;
			set_bit(ICE_MDD_VF_PRINT_PENDING, pf->state);
			if (netif_msg_tx_err(pf))
				dev_info(dev, "Malicious Driver Detection event TX_TCLAN detected on VF %d\n",
					 i);
		}

		reg = rd32(hw, VP_MDET_TX_TDPU(i));
		if (reg & VP_MDET_TX_TDPU_VALID_M) {
			wr32(hw, VP_MDET_TX_TDPU(i), 0xFFFF);
			vf->mdd_tx_events.count++;
			set_bit(ICE_MDD_VF_PRINT_PENDING, pf->state);
			if (netif_msg_tx_err(pf))
				dev_info(dev, "Malicious Driver Detection event TX_TDPU detected on VF %d\n",
					 i);
		}

		reg = rd32(hw, VP_MDET_RX(i));
		if (reg & VP_MDET_RX_VALID_M) {
			wr32(hw, VP_MDET_RX(i), 0xFFFF);
			vf->mdd_rx_events.count++;
			set_bit(ICE_MDD_VF_PRINT_PENDING, pf->state);
			if (netif_msg_rx_err(pf))
				dev_info(dev, "Malicious Driver Detection event RX detected on VF %d\n",
					 i);

			/* Since the queue is disabled on VF Rx MDD events, the
			 * PF can be configured to reset the VF through ethtool
			 * private flag mdd-auto-reset-vf.
			 */
			if (test_bit(ICE_FLAG_MDD_AUTO_RESET_VF, pf->flags)) {
				/* VF MDD event counters will be cleared by
				 * reset, so print the event prior to reset.
				 */
				ice_print_vf_rx_mdd_event(vf);
				mutex_lock(&pf->vf[i].cfg_lock);
				ice_reset_vf(&pf->vf[i], false);
				mutex_unlock(&pf->vf[i].cfg_lock);
			}
		}
	}

	ice_print_vfs_mdd_events(pf);
}

/**
 * ice_force_phys_link_state - Force the physical link state
 * @vsi: VSI to force the physical link state to up/down
 * @link_up: true/false indicates to set the physical link to up/down
 *
 * Force the physical link state by getting the current PHY capabilities from
 * hardware and setting the PHY config based on the determined capabilities. If
 * link changes a link event will be triggered because both the Enable Automatic
 * Link Update and LESM Enable bits are set when setting the PHY capabilities.
 *
 * Returns 0 on success, negative on failure
 */
static int ice_force_phys_link_state(struct ice_vsi *vsi, bool link_up)
{
	struct ice_aqc_get_phy_caps_data *pcaps;
	struct ice_aqc_set_phy_cfg_data *cfg;
	struct ice_port_info *pi;
	struct device *dev;
	int retcode;

	if (!vsi || !vsi->port_info || !vsi->back)
		return -EINVAL;
	if (vsi->type != ICE_VSI_PF)
		return 0;

	dev = ice_pf_to_dev(vsi->back);

	pi = vsi->port_info;

	pcaps = kzalloc(sizeof(*pcaps), GFP_KERNEL);
	if (!pcaps)
		return -ENOMEM;

	retcode = ice_aq_get_phy_caps(pi, false, ICE_AQC_REPORT_ACTIVE_CFG, pcaps,
				      NULL);
	if (retcode) {
		dev_err(dev, "Failed to get phy capabilities, VSI %d error %d\n",
			vsi->vsi_num, retcode);
		retcode = -EIO;
		goto out;
	}

	/* No change in link */
	if (link_up == !!(pcaps->caps & ICE_AQC_PHY_EN_LINK) &&
	    link_up == !!(pi->phy.link_info.link_info & ICE_AQ_LINK_UP))
		goto out;

	/* Use the current user PHY configuration. The current user PHY
	 * configuration is initialized during probe from PHY capabilities
	 * software mode, and updated on set PHY configuration.
	 */
	cfg = kmemdup(&pi->phy.curr_user_phy_cfg, sizeof(*cfg), GFP_KERNEL);
	if (!cfg) {
		retcode = -ENOMEM;
		goto out;
	}

	cfg->caps |= ICE_AQ_PHY_ENA_AUTO_LINK_UPDT;
	if (link_up)
		cfg->caps |= ICE_AQ_PHY_ENA_LINK;
	else
		cfg->caps &= ~ICE_AQ_PHY_ENA_LINK;

	retcode = ice_aq_set_phy_cfg(&vsi->back->hw, pi, cfg, NULL);
	if (retcode) {
		dev_err(dev, "Failed to set phy config, VSI %d error %d\n",
			vsi->vsi_num, retcode);
		retcode = -EIO;
	}

	kfree(cfg);
out:
	kfree(pcaps);
	return retcode;
}

/**
 * ice_init_nvm_phy_type - Initialize the NVM PHY type
 * @pi: port info structure
 *
 * Initialize nvm_phy_type_[low|high] for link lenient mode support
 */
static int ice_init_nvm_phy_type(struct ice_port_info *pi)
{
	struct ice_aqc_get_phy_caps_data *pcaps;
	struct ice_pf *pf = pi->hw->back;
	enum ice_status status;
	int err = 0;

	pcaps = kzalloc(sizeof(*pcaps), GFP_KERNEL);
	if (!pcaps)
		return -ENOMEM;

	status = ice_aq_get_phy_caps(pi, false, ICE_AQC_REPORT_TOPO_CAP_NO_MEDIA, pcaps,
				     NULL);

	if (status) {
		dev_err(ice_pf_to_dev(pf), "Get PHY capability failed.\n");
		err = -EIO;
		goto out;
	}

	pf->nvm_phy_type_hi = pcaps->phy_type_high;
	pf->nvm_phy_type_lo = pcaps->phy_type_low;

out:
	kfree(pcaps);
	return err;
}

/**
 * ice_init_link_dflt_override - Initialize link default override
 * @pi: port info structure
 *
 * Initialize link default override and PHY total port shutdown during probe
 */
static void ice_init_link_dflt_override(struct ice_port_info *pi)
{
	struct ice_link_default_override_tlv *ldo;
	struct ice_pf *pf = pi->hw->back;

	ldo = &pf->link_dflt_override;
	if (ice_get_link_default_override(ldo, pi))
		return;

	if (!(ldo->options & ICE_LINK_OVERRIDE_PORT_DIS))
		return;

	/* Enable Total Port Shutdown (override/replace link-down-on-close
	 * ethtool private flag) for ports with Port Disable bit set.
	 */
	set_bit(ICE_FLAG_TOTAL_PORT_SHUTDOWN_ENA, pf->flags);
	set_bit(ICE_FLAG_LINK_DOWN_ON_CLOSE_ENA, pf->flags);
}

/**
 * ice_init_phy_cfg_dflt_override - Initialize PHY cfg default override settings
 * @pi: port info structure
 *
 * If default override is enabled, initialize the user PHY cfg speed and FEC
 * settings using the default override mask from the NVM.
 *
 * The PHY should only be configured with the default override settings the
 * first time media is available. The ICE_LINK_DEFAULT_OVERRIDE_PENDING state
 * is used to indicate that the user PHY cfg default override is initialized
 * and the PHY has not been configured with the default override settings. The
 * state is set here, and cleared in ice_configure_phy the first time the PHY is
 * configured.
 *
 * This function should be called only if the FW doesn't support default
 * configuration mode, as reported by ice_fw_supports_report_dflt_cfg.
 */
static void ice_init_phy_cfg_dflt_override(struct ice_port_info *pi)
{
	struct ice_link_default_override_tlv *ldo;
	struct ice_aqc_set_phy_cfg_data *cfg;
	struct ice_phy_info *phy = &pi->phy;
	struct ice_pf *pf = pi->hw->back;

	ldo = &pf->link_dflt_override;

	/* If link default override is enabled, use to mask NVM PHY capabilities
	 * for speed and FEC default configuration.
	 */
	cfg = &phy->curr_user_phy_cfg;

	if (ldo->phy_type_low || ldo->phy_type_high) {
		cfg->phy_type_low = pf->nvm_phy_type_lo &
				    cpu_to_le64(ldo->phy_type_low);
		cfg->phy_type_high = pf->nvm_phy_type_hi &
				     cpu_to_le64(ldo->phy_type_high);
	}
	cfg->link_fec_opt = ldo->fec_options;
	phy->curr_user_fec_req = ICE_FEC_AUTO;

	set_bit(ICE_LINK_DEFAULT_OVERRIDE_PENDING, pf->state);
}

/**
 * ice_init_phy_user_cfg - Initialize the PHY user configuration
 * @pi: port info structure
 *
 * Initialize the current user PHY configuration, speed, FEC, and FC requested
 * mode to default. The PHY defaults are from get PHY capabilities topology
 * with media so call when media is first available. An error is returned if
 * called when media is not available. The PHY initialization completed state is
 * set here.
 *
 * These configurations are used when setting PHY
 * configuration. The user PHY configuration is updated on set PHY
 * configuration. Returns 0 on success, negative on failure
 */
static int ice_init_phy_user_cfg(struct ice_port_info *pi)
{
	struct ice_aqc_get_phy_caps_data *pcaps;
	struct ice_phy_info *phy = &pi->phy;
	struct ice_pf *pf = pi->hw->back;
	enum ice_status status;
	int err = 0;

	if (!(phy->link_info.link_info & ICE_AQ_MEDIA_AVAILABLE))
		return -EIO;

	pcaps = kzalloc(sizeof(*pcaps), GFP_KERNEL);
	if (!pcaps)
		return -ENOMEM;

	if (ice_fw_supports_report_dflt_cfg(pi->hw))
		status = ice_aq_get_phy_caps(pi, false, ICE_AQC_REPORT_DFLT_CFG,
					     pcaps, NULL);
	else
		status = ice_aq_get_phy_caps(pi, false, ICE_AQC_REPORT_TOPO_CAP_MEDIA,
					     pcaps, NULL);
	if (status) {
		dev_err(ice_pf_to_dev(pf), "Get PHY capability failed.\n");
		err = -EIO;
		goto err_out;
	}

	ice_copy_phy_caps_to_cfg(pi, pcaps, &pi->phy.curr_user_phy_cfg);

	/* check if lenient mode is supported and enabled */
	if (ice_fw_supports_link_override(pi->hw) &&
	    !(pcaps->module_compliance_enforcement &
	      ICE_AQC_MOD_ENFORCE_STRICT_MODE)) {
		set_bit(ICE_FLAG_LINK_LENIENT_MODE_ENA, pf->flags);

		/* if the FW supports default PHY configuration mode, then the driver
		 * does not have to apply link override settings. If not,
		 * initialize user PHY configuration with link override values
		 */
		if (!ice_fw_supports_report_dflt_cfg(pi->hw) &&
		    (pf->link_dflt_override.options & ICE_LINK_OVERRIDE_EN)) {
			ice_init_phy_cfg_dflt_override(pi);
			goto out;
		}
	}

	/* if link default override is not enabled, set user flow control and
	 * FEC settings based on what get_phy_caps returned
	 */
	phy->curr_user_fec_req = ice_caps_to_fec_mode(pcaps->caps,
						      pcaps->link_fec_options);
	phy->curr_user_fc_req = ice_caps_to_fc_mode(pcaps->caps);

out:
	phy->curr_user_speed_req = ICE_AQ_LINK_SPEED_M;
	set_bit(ICE_PHY_INIT_COMPLETE, pf->state);
err_out:
	kfree(pcaps);
	return err;
}

/**
 * ice_configure_phy - configure PHY
 * @vsi: VSI of PHY
 *
 * Set the PHY configuration. If the current PHY configuration is the same as
 * the curr_user_phy_cfg, then do nothing to avoid link flap. Otherwise
 * configure the based get PHY capabilities for topology with media.
 */
static int ice_configure_phy(struct ice_vsi *vsi)
{
	struct device *dev = ice_pf_to_dev(vsi->back);
	struct ice_port_info *pi = vsi->port_info;
	struct ice_aqc_get_phy_caps_data *pcaps;
	struct ice_aqc_set_phy_cfg_data *cfg;
	struct ice_phy_info *phy = &pi->phy;
	struct ice_pf *pf = vsi->back;
	enum ice_status status;
	int err = 0;

	/* Ensure we have media as we cannot configure a medialess port */
	if (!(phy->link_info.link_info & ICE_AQ_MEDIA_AVAILABLE))
		return -EPERM;

	ice_print_topo_conflict(vsi);

	if (phy->link_info.topo_media_conflict == ICE_AQ_LINK_TOPO_UNSUPP_MEDIA)
		return -EPERM;

	if (test_bit(ICE_FLAG_LINK_DOWN_ON_CLOSE_ENA, pf->flags))
		return ice_force_phys_link_state(vsi, true);

	pcaps = kzalloc(sizeof(*pcaps), GFP_KERNEL);
	if (!pcaps)
		return -ENOMEM;

	/* Get current PHY config */
	status = ice_aq_get_phy_caps(pi, false, ICE_AQC_REPORT_ACTIVE_CFG, pcaps,
				     NULL);
	if (status) {
		dev_err(dev, "Failed to get PHY configuration, VSI %d error %s\n",
			vsi->vsi_num, ice_stat_str(status));
		err = -EIO;
		goto done;
	}

	/* If PHY enable link is configured and configuration has not changed,
	 * there's nothing to do
	 */
	if (pcaps->caps & ICE_AQC_PHY_EN_LINK &&
	    ice_phy_caps_equals_cfg(pcaps, &phy->curr_user_phy_cfg))
		goto done;

	/* Use PHY topology as baseline for configuration */
	memset(pcaps, 0, sizeof(*pcaps));
	if (ice_fw_supports_report_dflt_cfg(pi->hw))
		status = ice_aq_get_phy_caps(pi, false, ICE_AQC_REPORT_DFLT_CFG,
					     pcaps, NULL);
	else
		status = ice_aq_get_phy_caps(pi, false, ICE_AQC_REPORT_TOPO_CAP_MEDIA,
					     pcaps, NULL);
	if (status) {
		dev_err(dev, "Failed to get PHY caps, VSI %d error %s\n",
			vsi->vsi_num, ice_stat_str(status));
		err = -EIO;
		goto done;
	}

	cfg = kzalloc(sizeof(*cfg), GFP_KERNEL);
	if (!cfg) {
		err = -ENOMEM;
		goto done;
	}

	ice_copy_phy_caps_to_cfg(pi, pcaps, cfg);

	/* Speed - If default override pending, use curr_user_phy_cfg set in
	 * ice_init_phy_user_cfg_ldo.
	 */
	if (test_and_clear_bit(ICE_LINK_DEFAULT_OVERRIDE_PENDING,
			       vsi->back->state)) {
		cfg->phy_type_low = phy->curr_user_phy_cfg.phy_type_low;
		cfg->phy_type_high = phy->curr_user_phy_cfg.phy_type_high;
	} else {
		u64 phy_low = 0, phy_high = 0;

		ice_update_phy_type(&phy_low, &phy_high,
				    pi->phy.curr_user_speed_req);
		cfg->phy_type_low = pcaps->phy_type_low & cpu_to_le64(phy_low);
		cfg->phy_type_high = pcaps->phy_type_high &
				     cpu_to_le64(phy_high);
	}

	/* Can't provide what was requested; use PHY capabilities */
	if (!cfg->phy_type_low && !cfg->phy_type_high) {
		cfg->phy_type_low = pcaps->phy_type_low;
		cfg->phy_type_high = pcaps->phy_type_high;
	}

	/* FEC */
	ice_cfg_phy_fec(pi, cfg, phy->curr_user_fec_req);

	/* Can't provide what was requested; use PHY capabilities */
	if (cfg->link_fec_opt !=
	    (cfg->link_fec_opt & pcaps->link_fec_options)) {
		cfg->caps |= pcaps->caps & ICE_AQC_PHY_EN_AUTO_FEC;
		cfg->link_fec_opt = pcaps->link_fec_options;
	}

	/* Flow Control - always supported; no need to check against
	 * capabilities
	 */
	ice_cfg_phy_fc(pi, cfg, phy->curr_user_fc_req);

	/* Enable link and link update */
	cfg->caps |= ICE_AQ_PHY_ENA_AUTO_LINK_UPDT | ICE_AQ_PHY_ENA_LINK;

	status = ice_aq_set_phy_cfg(&pf->hw, pi, cfg, NULL);
	if (status) {
		dev_err(dev, "Failed to set phy config, VSI %d error %s\n",
			vsi->vsi_num, ice_stat_str(status));
		err = -EIO;
	}

	kfree(cfg);
done:
	kfree(pcaps);
	return err;
}

/**
 * ice_check_media_subtask - Check for media
 * @pf: pointer to PF struct
 *
 * If media is available, then initialize PHY user configuration if it is not
 * been, and configure the PHY if the interface is up.
 */
static void ice_check_media_subtask(struct ice_pf *pf)
{
	struct ice_port_info *pi;
	struct ice_vsi *vsi;
	int err;

	/* No need to check for media if it's already present */
	if (!test_bit(ICE_FLAG_NO_MEDIA, pf->flags))
		return;

	vsi = ice_get_main_vsi(pf);
	if (!vsi)
		return;

	/* Refresh link info and check if media is present */
	pi = vsi->port_info;
	err = ice_update_link_info(pi);
	if (err)
		return;

	ice_check_module_power(pf, pi->phy.link_info.link_cfg_err);

	if (pi->phy.link_info.link_info & ICE_AQ_MEDIA_AVAILABLE) {
		if (!test_bit(ICE_PHY_INIT_COMPLETE, pf->state))
			ice_init_phy_user_cfg(pi);

		/* PHY settings are reset on media insertion, reconfigure
		 * PHY to preserve settings.
		 */
		if (test_bit(ICE_VSI_DOWN, vsi->state) &&
		    test_bit(ICE_FLAG_LINK_DOWN_ON_CLOSE_ENA, vsi->back->flags))
			return;

		err = ice_configure_phy(vsi);
		if (!err)
			clear_bit(ICE_FLAG_NO_MEDIA, pf->flags);

		/* A Link Status Event will be generated; the event handler
		 * will complete bringing the interface up
		 */
	}
}

/**
>>>>>>> upstream/android-13
 * ice_service_task - manage and run subtasks
 * @work: pointer to work_struct contained by the PF struct
 */
static void ice_service_task(struct work_struct *work)
{
	struct ice_pf *pf = container_of(work, struct ice_pf, serv_task);
	unsigned long start_time = jiffies;

	/* subtasks */

	/* process reset requests first */
	ice_reset_subtask(pf);

<<<<<<< HEAD
	/* bail if a reset/recovery cycle is pending */
	if (ice_is_reset_recovery_pending(pf->state) ||
	    test_bit(__ICE_SUSPENDED, pf->state)) {
=======
	/* bail if a reset/recovery cycle is pending or rebuild failed */
	if (ice_is_reset_in_progress(pf->state) ||
	    test_bit(ICE_SUSPENDED, pf->state) ||
	    test_bit(ICE_NEEDS_RESTART, pf->state)) {
>>>>>>> upstream/android-13
		ice_service_task_complete(pf);
		return;
	}

<<<<<<< HEAD
	ice_sync_fltr_subtask(pf);
	ice_watchdog_subtask(pf);
	ice_clean_adminq_subtask(pf);

	/* Clear __ICE_SERVICE_SCHED flag to allow scheduling next event */
=======
	if (test_and_clear_bit(ICE_AUX_ERR_PENDING, pf->state)) {
		struct iidc_event *event;

		event = kzalloc(sizeof(*event), GFP_KERNEL);
		if (event) {
			set_bit(IIDC_EVENT_CRIT_ERR, event->type);
			/* report the entire OICR value to AUX driver */
			swap(event->reg, pf->oicr_err_reg);
			ice_send_event_to_aux(pf, event);
			kfree(event);
		}
	}

	if (test_bit(ICE_FLAG_PLUG_AUX_DEV, pf->flags)) {
		/* Plug aux device per request */
		ice_plug_aux_dev(pf);

		/* Mark plugging as done but check whether unplug was
		 * requested during ice_plug_aux_dev() call
		 * (e.g. from ice_clear_rdma_cap()) and if so then
		 * plug aux device.
		 */
		if (!test_and_clear_bit(ICE_FLAG_PLUG_AUX_DEV, pf->flags))
			ice_unplug_aux_dev(pf);
	}

	if (test_and_clear_bit(ICE_FLAG_MTU_CHANGED, pf->flags)) {
		struct iidc_event *event;

		event = kzalloc(sizeof(*event), GFP_KERNEL);
		if (event) {
			set_bit(IIDC_EVENT_AFTER_MTU_CHANGE, event->type);
			ice_send_event_to_aux(pf, event);
			kfree(event);
		}
	}

	ice_clean_adminq_subtask(pf);
	ice_check_media_subtask(pf);
	ice_check_for_hang_subtask(pf);
	ice_sync_fltr_subtask(pf);
	ice_handle_mdd_event(pf);
	ice_watchdog_subtask(pf);

	if (ice_is_safe_mode(pf)) {
		ice_service_task_complete(pf);
		return;
	}

	ice_process_vflr_event(pf);
	ice_clean_mailboxq_subtask(pf);
	ice_clean_sbq_subtask(pf);
	ice_sync_arfs_fltrs(pf);
	ice_flush_fdir_ctx(pf);

	/* Clear ICE_SERVICE_SCHED flag to allow scheduling next event */
>>>>>>> upstream/android-13
	ice_service_task_complete(pf);

	/* If the tasks have taken longer than one service timer period
	 * or there is more work to be done, reset the service timer to
	 * schedule the service task now.
	 */
	if (time_after(jiffies, (start_time + pf->serv_tmr_period)) ||
<<<<<<< HEAD
	    test_bit(__ICE_ADMINQ_EVENT_PENDING, pf->state))
=======
	    test_bit(ICE_MDD_EVENT_PENDING, pf->state) ||
	    test_bit(ICE_VFLR_EVENT_PENDING, pf->state) ||
	    test_bit(ICE_MAILBOXQ_EVENT_PENDING, pf->state) ||
	    test_bit(ICE_FD_VF_FLUSH_CTX, pf->state) ||
	    test_bit(ICE_SIDEBANDQ_EVENT_PENDING, pf->state) ||
	    test_bit(ICE_ADMINQ_EVENT_PENDING, pf->state))
>>>>>>> upstream/android-13
		mod_timer(&pf->serv_tmr, jiffies);
}

/**
 * ice_set_ctrlq_len - helper function to set controlq length
<<<<<<< HEAD
 * @hw: pointer to the hw instance
=======
 * @hw: pointer to the HW instance
>>>>>>> upstream/android-13
 */
static void ice_set_ctrlq_len(struct ice_hw *hw)
{
	hw->adminq.num_rq_entries = ICE_AQ_LEN;
	hw->adminq.num_sq_entries = ICE_AQ_LEN;
	hw->adminq.rq_buf_size = ICE_AQ_MAX_BUF_LEN;
	hw->adminq.sq_buf_size = ICE_AQ_MAX_BUF_LEN;
<<<<<<< HEAD
=======
	hw->mailboxq.num_rq_entries = PF_MBX_ARQLEN_ARQLEN_M;
	hw->mailboxq.num_sq_entries = ICE_MBXSQ_LEN;
	hw->mailboxq.rq_buf_size = ICE_MBXQ_MAX_BUF_LEN;
	hw->mailboxq.sq_buf_size = ICE_MBXQ_MAX_BUF_LEN;
	hw->sbq.num_rq_entries = ICE_SBQ_LEN;
	hw->sbq.num_sq_entries = ICE_SBQ_LEN;
	hw->sbq.rq_buf_size = ICE_SBQ_MAX_BUF_LEN;
	hw->sbq.sq_buf_size = ICE_SBQ_MAX_BUF_LEN;
}

/**
 * ice_schedule_reset - schedule a reset
 * @pf: board private structure
 * @reset: reset being requested
 */
int ice_schedule_reset(struct ice_pf *pf, enum ice_reset_req reset)
{
	struct device *dev = ice_pf_to_dev(pf);

	/* bail out if earlier reset has failed */
	if (test_bit(ICE_RESET_FAILED, pf->state)) {
		dev_dbg(dev, "earlier reset has failed\n");
		return -EIO;
	}
	/* bail if reset/recovery already in progress */
	if (ice_is_reset_in_progress(pf->state)) {
		dev_dbg(dev, "Reset already in progress\n");
		return -EBUSY;
	}

	ice_unplug_aux_dev(pf);

	switch (reset) {
	case ICE_RESET_PFR:
		set_bit(ICE_PFR_REQ, pf->state);
		break;
	case ICE_RESET_CORER:
		set_bit(ICE_CORER_REQ, pf->state);
		break;
	case ICE_RESET_GLOBR:
		set_bit(ICE_GLOBR_REQ, pf->state);
		break;
	default:
		return -EINVAL;
	}

	ice_service_task_schedule(pf);
	return 0;
>>>>>>> upstream/android-13
}

/**
 * ice_irq_affinity_notify - Callback for affinity changes
 * @notify: context as to what irq was changed
 * @mask: the new affinity mask
 *
 * This is a callback function used by the irq_set_affinity_notifier function
 * so that we may register to receive changes to the irq affinity masks.
 */
<<<<<<< HEAD
static void ice_irq_affinity_notify(struct irq_affinity_notify *notify,
				    const cpumask_t *mask)
=======
static void
ice_irq_affinity_notify(struct irq_affinity_notify *notify,
			const cpumask_t *mask)
>>>>>>> upstream/android-13
{
	struct ice_q_vector *q_vector =
		container_of(notify, struct ice_q_vector, affinity_notify);

	cpumask_copy(&q_vector->affinity_mask, mask);
}

/**
 * ice_irq_affinity_release - Callback for affinity notifier release
 * @ref: internal core kernel usage
 *
 * This is a callback function used by the irq_set_affinity_notifier function
 * to inform the current notification subscriber that they will no longer
 * receive notifications.
 */
static void ice_irq_affinity_release(struct kref __always_unused *ref) {}

/**
<<<<<<< HEAD
 * ice_vsi_dis_irq - Mask off queue interrupt generation on the VSI
 * @vsi: the VSI being un-configured
 */
static void ice_vsi_dis_irq(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	struct ice_hw *hw = &pf->hw;
	int base = vsi->base_vector;
	u32 val;
	int i;

	/* disable interrupt causation from each queue */
	if (vsi->tx_rings) {
		ice_for_each_txq(vsi, i) {
			if (vsi->tx_rings[i]) {
				u16 reg;

				reg = vsi->tx_rings[i]->reg_idx;
				val = rd32(hw, QINT_TQCTL(reg));
				val &= ~QINT_TQCTL_CAUSE_ENA_M;
				wr32(hw, QINT_TQCTL(reg), val);
			}
		}
	}

	if (vsi->rx_rings) {
		ice_for_each_rxq(vsi, i) {
			if (vsi->rx_rings[i]) {
				u16 reg;

				reg = vsi->rx_rings[i]->reg_idx;
				val = rd32(hw, QINT_RQCTL(reg));
				val &= ~QINT_RQCTL_CAUSE_ENA_M;
				wr32(hw, QINT_RQCTL(reg), val);
			}
		}
	}

	/* disable each interrupt */
	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags)) {
		for (i = vsi->base_vector;
		     i < (vsi->num_q_vectors + vsi->base_vector); i++)
			wr32(hw, GLINT_DYN_CTL(i), 0);

		ice_flush(hw);
		for (i = 0; i < vsi->num_q_vectors; i++)
			synchronize_irq(pf->msix_entries[i + base].vector);
	}
}

/**
=======
>>>>>>> upstream/android-13
 * ice_vsi_ena_irq - Enable IRQ for the given VSI
 * @vsi: the VSI being configured
 */
static int ice_vsi_ena_irq(struct ice_vsi *vsi)
{
<<<<<<< HEAD
	struct ice_pf *pf = vsi->back;
	struct ice_hw *hw = &pf->hw;

	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags)) {
		int i;

		for (i = 0; i < vsi->num_q_vectors; i++)
			ice_irq_dynamic_ena(hw, vsi, vsi->q_vectors[i]);
	}
=======
	struct ice_hw *hw = &vsi->back->hw;
	int i;

	ice_for_each_q_vector(vsi, i)
		ice_irq_dynamic_ena(hw, vsi, vsi->q_vectors[i]);
>>>>>>> upstream/android-13

	ice_flush(hw);
	return 0;
}

/**
<<<<<<< HEAD
 * ice_vsi_delete - delete a VSI from the switch
 * @vsi: pointer to VSI being removed
 */
static void ice_vsi_delete(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	struct ice_vsi_ctx ctxt;
	enum ice_status status;

	ctxt.vsi_num = vsi->vsi_num;

	memcpy(&ctxt.info, &vsi->info, sizeof(struct ice_aqc_vsi_props));

	status = ice_aq_free_vsi(&pf->hw, &ctxt, false, NULL);
	if (status)
		dev_err(&pf->pdev->dev, "Failed to delete VSI %i in FW\n",
			vsi->vsi_num);
}

/**
=======
>>>>>>> upstream/android-13
 * ice_vsi_req_irq_msix - get MSI-X vectors from the OS for the VSI
 * @vsi: the VSI being configured
 * @basename: name for the vector
 */
static int ice_vsi_req_irq_msix(struct ice_vsi *vsi, char *basename)
{
	int q_vectors = vsi->num_q_vectors;
	struct ice_pf *pf = vsi->back;
	int base = vsi->base_vector;
<<<<<<< HEAD
=======
	struct device *dev;
>>>>>>> upstream/android-13
	int rx_int_idx = 0;
	int tx_int_idx = 0;
	int vector, err;
	int irq_num;

<<<<<<< HEAD
=======
	dev = ice_pf_to_dev(pf);
>>>>>>> upstream/android-13
	for (vector = 0; vector < q_vectors; vector++) {
		struct ice_q_vector *q_vector = vsi->q_vectors[vector];

		irq_num = pf->msix_entries[base + vector].vector;

		if (q_vector->tx.ring && q_vector->rx.ring) {
			snprintf(q_vector->name, sizeof(q_vector->name) - 1,
				 "%s-%s-%d", basename, "TxRx", rx_int_idx++);
			tx_int_idx++;
		} else if (q_vector->rx.ring) {
			snprintf(q_vector->name, sizeof(q_vector->name) - 1,
				 "%s-%s-%d", basename, "rx", rx_int_idx++);
		} else if (q_vector->tx.ring) {
			snprintf(q_vector->name, sizeof(q_vector->name) - 1,
				 "%s-%s-%d", basename, "tx", tx_int_idx++);
		} else {
			/* skip this unused q_vector */
			continue;
		}
<<<<<<< HEAD
		err = devm_request_irq(&pf->pdev->dev,
				       pf->msix_entries[base + vector].vector,
				       vsi->irq_handler, 0, q_vector->name,
				       q_vector);
		if (err) {
			netdev_err(vsi->netdev,
				   "MSIX request_irq failed, error: %d\n", err);
=======
		if (vsi->type == ICE_VSI_CTRL && vsi->vf_id != ICE_INVAL_VFID)
			err = devm_request_irq(dev, irq_num, vsi->irq_handler,
					       IRQF_SHARED, q_vector->name,
					       q_vector);
		else
			err = devm_request_irq(dev, irq_num, vsi->irq_handler,
					       0, q_vector->name, q_vector);
		if (err) {
			netdev_err(vsi->netdev, "MSIX request_irq failed, error: %d\n",
				   err);
>>>>>>> upstream/android-13
			goto free_q_irqs;
		}

		/* register for affinity change notifications */
<<<<<<< HEAD
		q_vector->affinity_notify.notify = ice_irq_affinity_notify;
		q_vector->affinity_notify.release = ice_irq_affinity_release;
		irq_set_affinity_notifier(irq_num, &q_vector->affinity_notify);
=======
		if (!IS_ENABLED(CONFIG_RFS_ACCEL)) {
			struct irq_affinity_notify *affinity_notify;

			affinity_notify = &q_vector->affinity_notify;
			affinity_notify->notify = ice_irq_affinity_notify;
			affinity_notify->release = ice_irq_affinity_release;
			irq_set_affinity_notifier(irq_num, affinity_notify);
		}
>>>>>>> upstream/android-13

		/* assign the mask for this irq */
		irq_set_affinity_hint(irq_num, &q_vector->affinity_mask);
	}

	vsi->irqs_ready = true;
	return 0;

free_q_irqs:
	while (vector) {
		vector--;
<<<<<<< HEAD
		irq_num = pf->msix_entries[base + vector].vector,
		irq_set_affinity_notifier(irq_num, NULL);
		irq_set_affinity_hint(irq_num, NULL);
		devm_free_irq(&pf->pdev->dev, irq_num, &vsi->q_vectors[vector]);
=======
		irq_num = pf->msix_entries[base + vector].vector;
		if (!IS_ENABLED(CONFIG_RFS_ACCEL))
			irq_set_affinity_notifier(irq_num, NULL);
		irq_set_affinity_hint(irq_num, NULL);
		devm_free_irq(dev, irq_num, &vsi->q_vectors[vector]);
>>>>>>> upstream/android-13
	}
	return err;
}

/**
<<<<<<< HEAD
 * ice_vsi_set_rss_params - Setup RSS capabilities per VSI type
 * @vsi: the VSI being configured
 */
static void ice_vsi_set_rss_params(struct ice_vsi *vsi)
{
	struct ice_hw_common_caps *cap;
	struct ice_pf *pf = vsi->back;

	if (!test_bit(ICE_FLAG_RSS_ENA, pf->flags)) {
		vsi->rss_size = 1;
		return;
	}

	cap = &pf->hw.func_caps.common_cap;
	switch (vsi->type) {
	case ICE_VSI_PF:
		/* PF VSI will inherit RSS instance of PF */
		vsi->rss_table_size = cap->rss_table_size;
		vsi->rss_size = min_t(int, num_online_cpus(),
				      BIT(cap->rss_table_entry_width));
		vsi->rss_lut_type = ICE_AQC_GSET_RSS_LUT_TABLE_TYPE_PF;
		break;
	default:
		dev_warn(&pf->pdev->dev, "Unknown VSI type %d\n", vsi->type);
		break;
	}
}

/**
 * ice_vsi_setup_q_map - Setup a VSI queue map
 * @vsi: the VSI being configured
 * @ctxt: VSI context structure
 */
static void ice_vsi_setup_q_map(struct ice_vsi *vsi, struct ice_vsi_ctx *ctxt)
{
	u16 offset = 0, qmap = 0, numq_tc;
	u16 pow = 0, max_rss = 0, qcount;
	u16 qcount_tx = vsi->alloc_txq;
	u16 qcount_rx = vsi->alloc_rxq;
	bool ena_tc0 = false;
	int i;

	/* at least TC0 should be enabled by default */
	if (vsi->tc_cfg.numtc) {
		if (!(vsi->tc_cfg.ena_tc & BIT(0)))
			ena_tc0 =  true;
	} else {
		ena_tc0 =  true;
	}

	if (ena_tc0) {
		vsi->tc_cfg.numtc++;
		vsi->tc_cfg.ena_tc |= 1;
	}

	numq_tc = qcount_rx / vsi->tc_cfg.numtc;

	/* TC mapping is a function of the number of Rx queues assigned to the
	 * VSI for each traffic class and the offset of these queues.
	 * The first 10 bits are for queue offset for TC0, next 4 bits for no:of
	 * queues allocated to TC0. No:of queues is a power-of-2.
	 *
	 * If TC is not enabled, the queue offset is set to 0, and allocate one
	 * queue, this way, traffic for the given TC will be sent to the default
	 * queue.
	 *
	 * Setup number and offset of Rx queues for all TCs for the VSI
	 */

	/* qcount will change if RSS is enabled */
	if (test_bit(ICE_FLAG_RSS_ENA, vsi->back->flags)) {
		if (vsi->type == ICE_VSI_PF)
			max_rss = ICE_MAX_LG_RSS_QS;
		else
			max_rss = ICE_MAX_SMALL_RSS_QS;

		qcount = min_t(int, numq_tc, max_rss);
		qcount = min_t(int, qcount, vsi->rss_size);
	} else {
		qcount = numq_tc;
	}

	/* find the (rounded up) power-of-2 of qcount */
	pow = order_base_2(qcount);

	for (i = 0; i < ICE_MAX_TRAFFIC_CLASS; i++) {
		if (!(vsi->tc_cfg.ena_tc & BIT(i))) {
			/* TC is not enabled */
			vsi->tc_cfg.tc_info[i].qoffset = 0;
			vsi->tc_cfg.tc_info[i].qcount = 1;
			ctxt->info.tc_mapping[i] = 0;
			continue;
		}

		/* TC is enabled */
		vsi->tc_cfg.tc_info[i].qoffset = offset;
		vsi->tc_cfg.tc_info[i].qcount = qcount;

		qmap = ((offset << ICE_AQ_VSI_TC_Q_OFFSET_S) &
			ICE_AQ_VSI_TC_Q_OFFSET_M) |
			((pow << ICE_AQ_VSI_TC_Q_NUM_S) &
			 ICE_AQ_VSI_TC_Q_NUM_M);
		offset += qcount;
		ctxt->info.tc_mapping[i] = cpu_to_le16(qmap);
	}

	vsi->num_txq = qcount_tx;
	vsi->num_rxq = offset;

	/* Rx queue mapping */
	ctxt->info.mapping_flags |= cpu_to_le16(ICE_AQ_VSI_Q_MAP_CONTIG);
	/* q_mapping buffer holds the info for the first queue allocated for
	 * this VSI in the PF space and also the number of queues associated
	 * with this VSI.
	 */
	ctxt->info.q_mapping[0] = cpu_to_le16(vsi->rxq_map[0]);
	ctxt->info.q_mapping[1] = cpu_to_le16(vsi->num_rxq);
}

/**
 * ice_set_dflt_vsi_ctx - Set default VSI context before adding a VSI
 * @ctxt: the VSI context being set
 *
 * This initializes a default VSI context for all sections except the Queues.
 */
static void ice_set_dflt_vsi_ctx(struct ice_vsi_ctx *ctxt)
{
	u32 table = 0;

	memset(&ctxt->info, 0, sizeof(ctxt->info));
	/* VSI's should be allocated from shared pool */
	ctxt->alloc_from_pool = true;
	/* Src pruning enabled by default */
	ctxt->info.sw_flags = ICE_AQ_VSI_SW_FLAG_SRC_PRUNE;
	/* Traffic from VSI can be sent to LAN */
	ctxt->info.sw_flags2 = ICE_AQ_VSI_SW_FLAG_LAN_ENA;

	/* By default bits 3 and 4 in vlan_flags are 0's which results in legacy
	 * behavior (show VLAN, DEI, and UP) in descriptor. Also, allow all
	 * packets untagged/tagged.
	 */
	ctxt->info.vlan_flags = ((ICE_AQ_VSI_VLAN_MODE_ALL &
				  ICE_AQ_VSI_VLAN_MODE_M) >>
				 ICE_AQ_VSI_VLAN_MODE_S);

	/* Have 1:1 UP mapping for both ingress/egress tables */
	table |= ICE_UP_TABLE_TRANSLATE(0, 0);
	table |= ICE_UP_TABLE_TRANSLATE(1, 1);
	table |= ICE_UP_TABLE_TRANSLATE(2, 2);
	table |= ICE_UP_TABLE_TRANSLATE(3, 3);
	table |= ICE_UP_TABLE_TRANSLATE(4, 4);
	table |= ICE_UP_TABLE_TRANSLATE(5, 5);
	table |= ICE_UP_TABLE_TRANSLATE(6, 6);
	table |= ICE_UP_TABLE_TRANSLATE(7, 7);
	ctxt->info.ingress_table = cpu_to_le32(table);
	ctxt->info.egress_table = cpu_to_le32(table);
	/* Have 1:1 UP mapping for outer to inner UP table */
	ctxt->info.outer_up_table = cpu_to_le32(table);
	/* No Outer tag support outer_tag_flags remains to zero */
}

/**
 * ice_set_rss_vsi_ctx - Set RSS VSI context before adding a VSI
 * @ctxt: the VSI context being set
 * @vsi: the VSI being configured
 */
static void ice_set_rss_vsi_ctx(struct ice_vsi_ctx *ctxt, struct ice_vsi *vsi)
{
	u8 lut_type, hash_type;

	switch (vsi->type) {
	case ICE_VSI_PF:
		/* PF VSI will inherit RSS instance of PF */
		lut_type = ICE_AQ_VSI_Q_OPT_RSS_LUT_PF;
		hash_type = ICE_AQ_VSI_Q_OPT_RSS_TPLZ;
		break;
	default:
		dev_warn(&vsi->back->pdev->dev, "Unknown VSI type %d\n",
			 vsi->type);
		return;
	}

	ctxt->info.q_opt_rss = ((lut_type << ICE_AQ_VSI_Q_OPT_RSS_LUT_S) &
				ICE_AQ_VSI_Q_OPT_RSS_LUT_M) |
				((hash_type << ICE_AQ_VSI_Q_OPT_RSS_HASH_S) &
				 ICE_AQ_VSI_Q_OPT_RSS_HASH_M);
}

/**
 * ice_vsi_add - Create a new VSI or fetch preallocated VSI
 * @vsi: the VSI being configured
 *
 * This initializes a VSI context depending on the VSI type to be added and
 * passes it down to the add_vsi aq command to create a new VSI.
 */
static int ice_vsi_add(struct ice_vsi *vsi)
{
	struct ice_vsi_ctx ctxt = { 0 };
	struct ice_pf *pf = vsi->back;
	struct ice_hw *hw = &pf->hw;
	int ret = 0;

	switch (vsi->type) {
	case ICE_VSI_PF:
		ctxt.flags = ICE_AQ_VSI_TYPE_PF;
		break;
	default:
		return -ENODEV;
	}

	ice_set_dflt_vsi_ctx(&ctxt);
	/* if the switch is in VEB mode, allow VSI loopback */
	if (vsi->vsw->bridge_mode == BRIDGE_MODE_VEB)
		ctxt.info.sw_flags |= ICE_AQ_VSI_SW_FLAG_ALLOW_LB;

	/* Set LUT type and HASH type if RSS is enabled */
	if (test_bit(ICE_FLAG_RSS_ENA, pf->flags))
		ice_set_rss_vsi_ctx(&ctxt, vsi);

	ctxt.info.sw_id = vsi->port_info->sw_id;
	ice_vsi_setup_q_map(vsi, &ctxt);

	ret = ice_aq_add_vsi(hw, &ctxt, NULL);
	if (ret) {
		dev_err(&vsi->back->pdev->dev,
			"Add VSI AQ call failed, err %d\n", ret);
		return -EIO;
	}
	vsi->info = ctxt.info;
	vsi->vsi_num = ctxt.vsi_num;

	return ret;
}

/**
 * ice_vsi_release_msix - Clear the queue to Interrupt mapping in HW
 * @vsi: the VSI being cleaned up
 */
static void ice_vsi_release_msix(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	u16 vector = vsi->base_vector;
	struct ice_hw *hw = &pf->hw;
	u32 txq = 0;
	u32 rxq = 0;
	int i, q;

	for (i = 0; i < vsi->num_q_vectors; i++, vector++) {
		struct ice_q_vector *q_vector = vsi->q_vectors[i];

		wr32(hw, GLINT_ITR(ICE_RX_ITR, vector), 0);
		wr32(hw, GLINT_ITR(ICE_TX_ITR, vector), 0);
		for (q = 0; q < q_vector->num_ring_tx; q++) {
			wr32(hw, QINT_TQCTL(vsi->txq_map[txq]), 0);
			txq++;
		}

		for (q = 0; q < q_vector->num_ring_rx; q++) {
			wr32(hw, QINT_RQCTL(vsi->rxq_map[rxq]), 0);
			rxq++;
		}
	}

	ice_flush(hw);
}

/**
 * ice_vsi_clear_rings - Deallocates the Tx and Rx rings for VSI
 * @vsi: the VSI having rings deallocated
 */
static void ice_vsi_clear_rings(struct ice_vsi *vsi)
{
	int i;

	if (vsi->tx_rings) {
		for (i = 0; i < vsi->alloc_txq; i++) {
			if (vsi->tx_rings[i]) {
				kfree_rcu(vsi->tx_rings[i], rcu);
				vsi->tx_rings[i] = NULL;
			}
		}
	}
	if (vsi->rx_rings) {
		for (i = 0; i < vsi->alloc_rxq; i++) {
			if (vsi->rx_rings[i]) {
				kfree_rcu(vsi->rx_rings[i], rcu);
				vsi->rx_rings[i] = NULL;
			}
		}
	}
}

/**
 * ice_vsi_alloc_rings - Allocates Tx and Rx rings for the VSI
 * @vsi: VSI which is having rings allocated
 */
static int ice_vsi_alloc_rings(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	int i;

	/* Allocate tx_rings */
	for (i = 0; i < vsi->alloc_txq; i++) {
		struct ice_ring *ring;

		/* allocate with kzalloc(), free with kfree_rcu() */
		ring = kzalloc(sizeof(*ring), GFP_KERNEL);

		if (!ring)
			goto err_out;

		ring->q_index = i;
		ring->reg_idx = vsi->txq_map[i];
		ring->ring_active = false;
		ring->vsi = vsi;
		ring->netdev = vsi->netdev;
		ring->dev = &pf->pdev->dev;
		ring->count = vsi->num_desc;

		vsi->tx_rings[i] = ring;
	}

	/* Allocate rx_rings */
	for (i = 0; i < vsi->alloc_rxq; i++) {
		struct ice_ring *ring;

		/* allocate with kzalloc(), free with kfree_rcu() */
		ring = kzalloc(sizeof(*ring), GFP_KERNEL);
		if (!ring)
			goto err_out;

		ring->q_index = i;
		ring->reg_idx = vsi->rxq_map[i];
		ring->ring_active = false;
		ring->vsi = vsi;
		ring->netdev = vsi->netdev;
		ring->dev = &pf->pdev->dev;
		ring->count = vsi->num_desc;
		vsi->rx_rings[i] = ring;
=======
 * ice_xdp_alloc_setup_rings - Allocate and setup Tx rings for XDP
 * @vsi: VSI to setup Tx rings used by XDP
 *
 * Return 0 on success and negative value on error
 */
static int ice_xdp_alloc_setup_rings(struct ice_vsi *vsi)
{
	struct device *dev = ice_pf_to_dev(vsi->back);
	int i;

	for (i = 0; i < vsi->num_xdp_txq; i++) {
		u16 xdp_q_idx = vsi->alloc_txq + i;
		struct ice_ring *xdp_ring;

		xdp_ring = kzalloc(sizeof(*xdp_ring), GFP_KERNEL);

		if (!xdp_ring)
			goto free_xdp_rings;

		xdp_ring->q_index = xdp_q_idx;
		xdp_ring->reg_idx = vsi->txq_map[xdp_q_idx];
		xdp_ring->ring_active = false;
		xdp_ring->vsi = vsi;
		xdp_ring->netdev = NULL;
		xdp_ring->dev = dev;
		xdp_ring->count = vsi->num_tx_desc;
		WRITE_ONCE(vsi->xdp_rings[i], xdp_ring);
		if (ice_setup_tx_ring(xdp_ring))
			goto free_xdp_rings;
		ice_set_ring_xdp(xdp_ring);
		xdp_ring->xsk_pool = ice_xsk_pool(xdp_ring);
>>>>>>> upstream/android-13
	}

	return 0;

<<<<<<< HEAD
err_out:
	ice_vsi_clear_rings(vsi);
=======
free_xdp_rings:
	for (; i >= 0; i--)
		if (vsi->xdp_rings[i] && vsi->xdp_rings[i]->desc)
			ice_free_tx_ring(vsi->xdp_rings[i]);
>>>>>>> upstream/android-13
	return -ENOMEM;
}

/**
<<<<<<< HEAD
 * ice_vsi_free_irq - Free the irq association with the OS
 * @vsi: the VSI being configured
 */
static void ice_vsi_free_irq(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	int base = vsi->base_vector;

	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags)) {
		int i;

		if (!vsi->q_vectors || !vsi->irqs_ready)
			return;

		vsi->irqs_ready = false;
		for (i = 0; i < vsi->num_q_vectors; i++) {
			u16 vector = i + base;
			int irq_num;

			irq_num = pf->msix_entries[vector].vector;

			/* free only the irqs that were actually requested */
			if (!vsi->q_vectors[i] ||
			    !(vsi->q_vectors[i]->num_ring_tx ||
			      vsi->q_vectors[i]->num_ring_rx))
				continue;

			/* clear the affinity notifier in the IRQ descriptor */
			irq_set_affinity_notifier(irq_num, NULL);

			/* clear the affinity_mask in the IRQ descriptor */
			irq_set_affinity_hint(irq_num, NULL);
			synchronize_irq(irq_num);
			devm_free_irq(&pf->pdev->dev, irq_num,
				      vsi->q_vectors[i]);
		}
		ice_vsi_release_msix(vsi);
=======
 * ice_vsi_assign_bpf_prog - set or clear bpf prog pointer on VSI
 * @vsi: VSI to set the bpf prog on
 * @prog: the bpf prog pointer
 */
static void ice_vsi_assign_bpf_prog(struct ice_vsi *vsi, struct bpf_prog *prog)
{
	struct bpf_prog *old_prog;
	int i;

	old_prog = xchg(&vsi->xdp_prog, prog);
	if (old_prog)
		bpf_prog_put(old_prog);

	ice_for_each_rxq(vsi, i)
		WRITE_ONCE(vsi->rx_rings[i]->xdp_prog, vsi->xdp_prog);
}

/**
 * ice_prepare_xdp_rings - Allocate, configure and setup Tx rings for XDP
 * @vsi: VSI to bring up Tx rings used by XDP
 * @prog: bpf program that will be assigned to VSI
 *
 * Return 0 on success and negative value on error
 */
int ice_prepare_xdp_rings(struct ice_vsi *vsi, struct bpf_prog *prog)
{
	u16 max_txqs[ICE_MAX_TRAFFIC_CLASS] = { 0 };
	int xdp_rings_rem = vsi->num_xdp_txq;
	struct ice_pf *pf = vsi->back;
	struct ice_qs_cfg xdp_qs_cfg = {
		.qs_mutex = &pf->avail_q_mutex,
		.pf_map = pf->avail_txqs,
		.pf_map_size = pf->max_pf_txqs,
		.q_count = vsi->num_xdp_txq,
		.scatter_count = ICE_MAX_SCATTER_TXQS,
		.vsi_map = vsi->txq_map,
		.vsi_map_offset = vsi->alloc_txq,
		.mapping_mode = ICE_VSI_MAP_CONTIG
	};
	enum ice_status status;
	struct device *dev;
	int i, v_idx;

	dev = ice_pf_to_dev(pf);
	vsi->xdp_rings = devm_kcalloc(dev, vsi->num_xdp_txq,
				      sizeof(*vsi->xdp_rings), GFP_KERNEL);
	if (!vsi->xdp_rings)
		return -ENOMEM;

	vsi->xdp_mapping_mode = xdp_qs_cfg.mapping_mode;
	if (__ice_vsi_get_qs(&xdp_qs_cfg))
		goto err_map_xdp;

	if (ice_xdp_alloc_setup_rings(vsi))
		goto clear_xdp_rings;

	/* follow the logic from ice_vsi_map_rings_to_vectors */
	ice_for_each_q_vector(vsi, v_idx) {
		struct ice_q_vector *q_vector = vsi->q_vectors[v_idx];
		int xdp_rings_per_v, q_id, q_base;

		xdp_rings_per_v = DIV_ROUND_UP(xdp_rings_rem,
					       vsi->num_q_vectors - v_idx);
		q_base = vsi->num_xdp_txq - xdp_rings_rem;

		for (q_id = q_base; q_id < (q_base + xdp_rings_per_v); q_id++) {
			struct ice_ring *xdp_ring = vsi->xdp_rings[q_id];

			xdp_ring->q_vector = q_vector;
			xdp_ring->next = q_vector->tx.ring;
			q_vector->tx.ring = xdp_ring;
		}
		xdp_rings_rem -= xdp_rings_per_v;
	}

	/* omit the scheduler update if in reset path; XDP queues will be
	 * taken into account at the end of ice_vsi_rebuild, where
	 * ice_cfg_vsi_lan is being called
	 */
	if (ice_is_reset_in_progress(pf->state))
		return 0;

	/* tell the Tx scheduler that right now we have
	 * additional queues
	 */
	for (i = 0; i < vsi->tc_cfg.numtc; i++)
		max_txqs[i] = vsi->num_txq + vsi->num_xdp_txq;

	status = ice_cfg_vsi_lan(vsi->port_info, vsi->idx, vsi->tc_cfg.ena_tc,
				 max_txqs);
	if (status) {
		dev_err(dev, "Failed VSI LAN queue config for XDP, error: %s\n",
			ice_stat_str(status));
		goto clear_xdp_rings;
	}

	/* assign the prog only when it's not already present on VSI;
	 * this flow is a subject of both ethtool -L and ndo_bpf flows;
	 * VSI rebuild that happens under ethtool -L can expose us to
	 * the bpf_prog refcount issues as we would be swapping same
	 * bpf_prog pointers from vsi->xdp_prog and calling bpf_prog_put
	 * on it as it would be treated as an 'old_prog'; for ndo_bpf
	 * this is not harmful as dev_xdp_install bumps the refcount
	 * before calling the op exposed by the driver;
	 */
	if (!ice_is_xdp_ena_vsi(vsi))
		ice_vsi_assign_bpf_prog(vsi, prog);

	return 0;
clear_xdp_rings:
	for (i = 0; i < vsi->num_xdp_txq; i++)
		if (vsi->xdp_rings[i]) {
			kfree_rcu(vsi->xdp_rings[i], rcu);
			vsi->xdp_rings[i] = NULL;
		}

err_map_xdp:
	mutex_lock(&pf->avail_q_mutex);
	for (i = 0; i < vsi->num_xdp_txq; i++) {
		clear_bit(vsi->txq_map[i + vsi->alloc_txq], pf->avail_txqs);
		vsi->txq_map[i + vsi->alloc_txq] = ICE_INVAL_Q_INDEX;
	}
	mutex_unlock(&pf->avail_q_mutex);

	devm_kfree(dev, vsi->xdp_rings);
	return -ENOMEM;
}

/**
 * ice_destroy_xdp_rings - undo the configuration made by ice_prepare_xdp_rings
 * @vsi: VSI to remove XDP rings
 *
 * Detach XDP rings from irq vectors, clean up the PF bitmap and free
 * resources
 */
int ice_destroy_xdp_rings(struct ice_vsi *vsi)
{
	u16 max_txqs[ICE_MAX_TRAFFIC_CLASS] = { 0 };
	struct ice_pf *pf = vsi->back;
	int i, v_idx;

	/* q_vectors are freed in reset path so there's no point in detaching
	 * rings; in case of rebuild being triggered not from reset bits
	 * in pf->state won't be set, so additionally check first q_vector
	 * against NULL
	 */
	if (ice_is_reset_in_progress(pf->state) || !vsi->q_vectors[0])
		goto free_qmap;

	ice_for_each_q_vector(vsi, v_idx) {
		struct ice_q_vector *q_vector = vsi->q_vectors[v_idx];
		struct ice_ring *ring;

		ice_for_each_ring(ring, q_vector->tx)
			if (!ring->tx_buf || !ice_ring_is_xdp(ring))
				break;

		/* restore the value of last node prior to XDP setup */
		q_vector->tx.ring = ring;
	}

free_qmap:
	mutex_lock(&pf->avail_q_mutex);
	for (i = 0; i < vsi->num_xdp_txq; i++) {
		clear_bit(vsi->txq_map[i + vsi->alloc_txq], pf->avail_txqs);
		vsi->txq_map[i + vsi->alloc_txq] = ICE_INVAL_Q_INDEX;
	}
	mutex_unlock(&pf->avail_q_mutex);

	for (i = 0; i < vsi->num_xdp_txq; i++)
		if (vsi->xdp_rings[i]) {
			if (vsi->xdp_rings[i]->desc) {
				synchronize_rcu();
				ice_free_tx_ring(vsi->xdp_rings[i]);
			}
			kfree_rcu(vsi->xdp_rings[i], rcu);
			vsi->xdp_rings[i] = NULL;
		}

	devm_kfree(ice_pf_to_dev(pf), vsi->xdp_rings);
	vsi->xdp_rings = NULL;

	if (ice_is_reset_in_progress(pf->state) || !vsi->q_vectors[0])
		return 0;

	ice_vsi_assign_bpf_prog(vsi, NULL);

	/* notify Tx scheduler that we destroyed XDP queues and bring
	 * back the old number of child nodes
	 */
	for (i = 0; i < vsi->tc_cfg.numtc; i++)
		max_txqs[i] = vsi->num_txq;

	/* change number of XDP Tx queues to 0 */
	vsi->num_xdp_txq = 0;

	return ice_cfg_vsi_lan(vsi->port_info, vsi->idx, vsi->tc_cfg.ena_tc,
			       max_txqs);
}

/**
 * ice_vsi_rx_napi_schedule - Schedule napi on RX queues from VSI
 * @vsi: VSI to schedule napi on
 */
static void ice_vsi_rx_napi_schedule(struct ice_vsi *vsi)
{
	int i;

	ice_for_each_rxq(vsi, i) {
		struct ice_ring *rx_ring = vsi->rx_rings[i];

		if (rx_ring->xsk_pool)
			napi_schedule(&rx_ring->q_vector->napi);
>>>>>>> upstream/android-13
	}
}

/**
<<<<<<< HEAD
 * ice_vsi_cfg_msix - MSIX mode Interrupt Config in the HW
 * @vsi: the VSI being configured
 */
static void ice_vsi_cfg_msix(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	u16 vector = vsi->base_vector;
	struct ice_hw *hw = &pf->hw;
	u32 txq = 0, rxq = 0;
	int i, q, itr;
	u8 itr_gran;

	for (i = 0; i < vsi->num_q_vectors; i++, vector++) {
		struct ice_q_vector *q_vector = vsi->q_vectors[i];

		itr_gran = hw->itr_gran_200;

		if (q_vector->num_ring_rx) {
			q_vector->rx.itr =
				ITR_TO_REG(vsi->rx_rings[rxq]->rx_itr_setting,
					   itr_gran);
			q_vector->rx.latency_range = ICE_LOW_LATENCY;
		}

		if (q_vector->num_ring_tx) {
			q_vector->tx.itr =
				ITR_TO_REG(vsi->tx_rings[txq]->tx_itr_setting,
					   itr_gran);
			q_vector->tx.latency_range = ICE_LOW_LATENCY;
		}
		wr32(hw, GLINT_ITR(ICE_RX_ITR, vector), q_vector->rx.itr);
		wr32(hw, GLINT_ITR(ICE_TX_ITR, vector), q_vector->tx.itr);

		/* Both Transmit Queue Interrupt Cause Control register
		 * and Receive Queue Interrupt Cause control register
		 * expects MSIX_INDX field to be the vector index
		 * within the function space and not the absolute
		 * vector index across PF or across device.
		 * For SR-IOV VF VSIs queue vector index always starts
		 * with 1 since first vector index(0) is used for OICR
		 * in VF space. Since VMDq and other PF VSIs are withtin
		 * the PF function space, use the vector index thats
		 * tracked for this PF.
		 */
		for (q = 0; q < q_vector->num_ring_tx; q++) {
			u32 val;

			itr = ICE_TX_ITR;
			val = QINT_TQCTL_CAUSE_ENA_M |
			      (itr << QINT_TQCTL_ITR_INDX_S)  |
			      (vector << QINT_TQCTL_MSIX_INDX_S);
			wr32(hw, QINT_TQCTL(vsi->txq_map[txq]), val);
			txq++;
		}

		for (q = 0; q < q_vector->num_ring_rx; q++) {
			u32 val;

			itr = ICE_RX_ITR;
			val = QINT_RQCTL_CAUSE_ENA_M |
			      (itr << QINT_RQCTL_ITR_INDX_S)  |
			      (vector << QINT_RQCTL_MSIX_INDX_S);
			wr32(hw, QINT_RQCTL(vsi->rxq_map[rxq]), val);
			rxq++;
		}
	}

	ice_flush(hw);
=======
 * ice_xdp_setup_prog - Add or remove XDP eBPF program
 * @vsi: VSI to setup XDP for
 * @prog: XDP program
 * @extack: netlink extended ack
 */
static int
ice_xdp_setup_prog(struct ice_vsi *vsi, struct bpf_prog *prog,
		   struct netlink_ext_ack *extack)
{
	int frame_size = vsi->netdev->mtu + ICE_ETH_PKT_HDR_PAD;
	bool if_running = netif_running(vsi->netdev);
	int ret = 0, xdp_ring_err = 0;

	if (frame_size > vsi->rx_buf_len) {
		NL_SET_ERR_MSG_MOD(extack, "MTU too large for loading XDP");
		return -EOPNOTSUPP;
	}

	/* need to stop netdev while setting up the program for Rx rings */
	if (if_running && !test_and_set_bit(ICE_VSI_DOWN, vsi->state)) {
		ret = ice_down(vsi);
		if (ret) {
			NL_SET_ERR_MSG_MOD(extack, "Preparing device for XDP attach failed");
			return ret;
		}
	}

	if (!ice_is_xdp_ena_vsi(vsi) && prog) {
		vsi->num_xdp_txq = vsi->alloc_rxq;
		xdp_ring_err = ice_prepare_xdp_rings(vsi, prog);
		if (xdp_ring_err)
			NL_SET_ERR_MSG_MOD(extack, "Setting up XDP Tx resources failed");
	} else if (ice_is_xdp_ena_vsi(vsi) && !prog) {
		xdp_ring_err = ice_destroy_xdp_rings(vsi);
		if (xdp_ring_err)
			NL_SET_ERR_MSG_MOD(extack, "Freeing XDP Tx resources failed");
	} else {
		/* safe to call even when prog == vsi->xdp_prog as
		 * dev_xdp_install in net/core/dev.c incremented prog's
		 * refcount so corresponding bpf_prog_put won't cause
		 * underflow
		 */
		ice_vsi_assign_bpf_prog(vsi, prog);
	}

	if (if_running)
		ret = ice_up(vsi);

	if (!ret && prog)
		ice_vsi_rx_napi_schedule(vsi);

	return (ret || xdp_ring_err) ? -ENOMEM : 0;
}

/**
 * ice_xdp_safe_mode - XDP handler for safe mode
 * @dev: netdevice
 * @xdp: XDP command
 */
static int ice_xdp_safe_mode(struct net_device __always_unused *dev,
			     struct netdev_bpf *xdp)
{
	NL_SET_ERR_MSG_MOD(xdp->extack,
			   "Please provide working DDP firmware package in order to use XDP\n"
			   "Refer to Documentation/networking/device_drivers/ethernet/intel/ice.rst");
	return -EOPNOTSUPP;
}

/**
 * ice_xdp - implements XDP handler
 * @dev: netdevice
 * @xdp: XDP command
 */
static int ice_xdp(struct net_device *dev, struct netdev_bpf *xdp)
{
	struct ice_netdev_priv *np = netdev_priv(dev);
	struct ice_vsi *vsi = np->vsi;

	if (vsi->type != ICE_VSI_PF) {
		NL_SET_ERR_MSG_MOD(xdp->extack, "XDP can be loaded only on PF VSI");
		return -EINVAL;
	}

	switch (xdp->command) {
	case XDP_SETUP_PROG:
		return ice_xdp_setup_prog(vsi, xdp->prog, xdp->extack);
	case XDP_SETUP_XSK_POOL:
		return ice_xsk_pool_setup(vsi, xdp->xsk.pool,
					  xdp->xsk.queue_id);
	default:
		return -EINVAL;
	}
>>>>>>> upstream/android-13
}

/**
 * ice_ena_misc_vector - enable the non-queue interrupts
 * @pf: board private structure
 */
static void ice_ena_misc_vector(struct ice_pf *pf)
{
	struct ice_hw *hw = &pf->hw;
	u32 val;

<<<<<<< HEAD
=======
	/* Disable anti-spoof detection interrupt to prevent spurious event
	 * interrupts during a function reset. Anti-spoof functionally is
	 * still supported.
	 */
	val = rd32(hw, GL_MDCK_TX_TDPU);
	val |= GL_MDCK_TX_TDPU_RCU_ANTISPOOF_ITR_DIS_M;
	wr32(hw, GL_MDCK_TX_TDPU, val);

>>>>>>> upstream/android-13
	/* clear things first */
	wr32(hw, PFINT_OICR_ENA, 0);	/* disable all */
	rd32(hw, PFINT_OICR);		/* read to clear */

	val = (PFINT_OICR_ECC_ERR_M |
	       PFINT_OICR_MAL_DETECT_M |
	       PFINT_OICR_GRST_M |
	       PFINT_OICR_PCI_EXCEPTION_M |
<<<<<<< HEAD
	       PFINT_OICR_HMC_ERR_M |
=======
	       PFINT_OICR_VFLR_M |
	       PFINT_OICR_HMC_ERR_M |
	       PFINT_OICR_PE_PUSH_M |
>>>>>>> upstream/android-13
	       PFINT_OICR_PE_CRITERR_M);

	wr32(hw, PFINT_OICR_ENA, val);

	/* SW_ITR_IDX = 0, but don't change INTENA */
	wr32(hw, GLINT_DYN_CTL(pf->oicr_idx),
	     GLINT_DYN_CTL_SW_ITR_INDX_M | GLINT_DYN_CTL_INTENA_MSK_M);
}

/**
 * ice_misc_intr - misc interrupt handler
 * @irq: interrupt number
 * @data: pointer to a q_vector
 */
static irqreturn_t ice_misc_intr(int __always_unused irq, void *data)
{
	struct ice_pf *pf = (struct ice_pf *)data;
	struct ice_hw *hw = &pf->hw;
	irqreturn_t ret = IRQ_NONE;
<<<<<<< HEAD
	u32 oicr, ena_mask;

	set_bit(__ICE_ADMINQ_EVENT_PENDING, pf->state);
=======
	struct device *dev;
	u32 oicr, ena_mask;

	dev = ice_pf_to_dev(pf);
	set_bit(ICE_ADMINQ_EVENT_PENDING, pf->state);
	set_bit(ICE_MAILBOXQ_EVENT_PENDING, pf->state);
	set_bit(ICE_SIDEBANDQ_EVENT_PENDING, pf->state);
>>>>>>> upstream/android-13

	oicr = rd32(hw, PFINT_OICR);
	ena_mask = rd32(hw, PFINT_OICR_ENA);

<<<<<<< HEAD
	if (oicr & PFINT_OICR_GRST_M) {
		u32 reset;
=======
	if (oicr & PFINT_OICR_SWINT_M) {
		ena_mask &= ~PFINT_OICR_SWINT_M;
		pf->sw_int_count++;
	}

	if (oicr & PFINT_OICR_MAL_DETECT_M) {
		ena_mask &= ~PFINT_OICR_MAL_DETECT_M;
		set_bit(ICE_MDD_EVENT_PENDING, pf->state);
	}
	if (oicr & PFINT_OICR_VFLR_M) {
		/* disable any further VFLR event notifications */
		if (test_bit(ICE_VF_RESETS_DISABLED, pf->state)) {
			u32 reg = rd32(hw, PFINT_OICR_ENA);

			reg &= ~PFINT_OICR_VFLR_M;
			wr32(hw, PFINT_OICR_ENA, reg);
		} else {
			ena_mask &= ~PFINT_OICR_VFLR_M;
			set_bit(ICE_VFLR_EVENT_PENDING, pf->state);
		}
	}

	if (oicr & PFINT_OICR_GRST_M) {
		u32 reset;

>>>>>>> upstream/android-13
		/* we have a reset warning */
		ena_mask &= ~PFINT_OICR_GRST_M;
		reset = (rd32(hw, GLGEN_RSTAT) & GLGEN_RSTAT_RESET_TYPE_M) >>
			GLGEN_RSTAT_RESET_TYPE_S;

		if (reset == ICE_RESET_CORER)
			pf->corer_count++;
		else if (reset == ICE_RESET_GLOBR)
			pf->globr_count++;
<<<<<<< HEAD
		else
			pf->empr_count++;

		/* If a reset cycle isn't already in progress, we set a bit in
		 * pf->state so that the service task can start a reset/rebuild.
		 * We also make note of which reset happened so that peer
		 * devices/drivers can be informed.
		 */
		if (!test_and_set_bit(__ICE_RESET_RECOVERY_PENDING,
				      pf->state)) {
			if (reset == ICE_RESET_CORER)
				set_bit(__ICE_CORER_RECV, pf->state);
			else if (reset == ICE_RESET_GLOBR)
				set_bit(__ICE_GLOBR_RECV, pf->state);
			else
				set_bit(__ICE_EMPR_RECV, pf->state);
=======
		else if (reset == ICE_RESET_EMPR)
			pf->empr_count++;
		else
			dev_dbg(dev, "Invalid reset type %d\n", reset);

		/* If a reset cycle isn't already in progress, we set a bit in
		 * pf->state so that the service task can start a reset/rebuild.
		 */
		if (!test_and_set_bit(ICE_RESET_OICR_RECV, pf->state)) {
			if (reset == ICE_RESET_CORER)
				set_bit(ICE_CORER_RECV, pf->state);
			else if (reset == ICE_RESET_GLOBR)
				set_bit(ICE_GLOBR_RECV, pf->state);
			else
				set_bit(ICE_EMPR_RECV, pf->state);
>>>>>>> upstream/android-13

			/* There are couple of different bits at play here.
			 * hw->reset_ongoing indicates whether the hardware is
			 * in reset. This is set to true when a reset interrupt
			 * is received and set back to false after the driver
			 * has determined that the hardware is out of reset.
			 *
<<<<<<< HEAD
			 * __ICE_RESET_RECOVERY_PENDING in pf->state indicates
=======
			 * ICE_RESET_OICR_RECV in pf->state indicates
>>>>>>> upstream/android-13
			 * that a post reset rebuild is required before the
			 * driver is operational again. This is set above.
			 *
			 * As this is the start of the reset/rebuild cycle, set
			 * both to indicate that.
			 */
			hw->reset_ongoing = true;
		}
	}

<<<<<<< HEAD
	if (oicr & PFINT_OICR_HMC_ERR_M) {
		ena_mask &= ~PFINT_OICR_HMC_ERR_M;
		dev_dbg(&pf->pdev->dev,
			"HMC Error interrupt - info 0x%x, data 0x%x\n",
			rd32(hw, PFHMC_ERRORINFO),
			rd32(hw, PFHMC_ERRORDATA));
	}

	/* Report and mask off any remaining unexpected interrupts */
	oicr &= ena_mask;
	if (oicr) {
		dev_dbg(&pf->pdev->dev, "unhandled interrupt oicr=0x%08x\n",
			oicr);
		/* If a critical error is pending there is no choice but to
		 * reset the device.
		 */
		if (oicr & (PFINT_OICR_PE_CRITERR_M |
			    PFINT_OICR_PCI_EXCEPTION_M |
			    PFINT_OICR_ECC_ERR_M)) {
			set_bit(__ICE_PFR_REQ, pf->state);
			ice_service_task_schedule(pf);
		}
		ena_mask &= ~oicr;
	}
	ret = IRQ_HANDLED;

	/* re-enable interrupt causes that are not handled during this pass */
	wr32(hw, PFINT_OICR_ENA, ena_mask);
	if (!test_bit(__ICE_DOWN, pf->state)) {
		ice_service_task_schedule(pf);
		ice_irq_dynamic_ena(hw, NULL, NULL);
	}
=======
	if (oicr & PFINT_OICR_TSYN_TX_M) {
		ena_mask &= ~PFINT_OICR_TSYN_TX_M;
		ice_ptp_process_ts(pf);
	}

	if (oicr & PFINT_OICR_TSYN_EVNT_M) {
		u8 tmr_idx = hw->func_caps.ts_func_info.tmr_index_owned;
		u32 gltsyn_stat = rd32(hw, GLTSYN_STAT(tmr_idx));

		/* Save EVENTs from GTSYN register */
		pf->ptp.ext_ts_irq |= gltsyn_stat & (GLTSYN_STAT_EVENT0_M |
						     GLTSYN_STAT_EVENT1_M |
						     GLTSYN_STAT_EVENT2_M);
		ena_mask &= ~PFINT_OICR_TSYN_EVNT_M;
		kthread_queue_work(pf->ptp.kworker, &pf->ptp.extts_work);
	}

#define ICE_AUX_CRIT_ERR (PFINT_OICR_PE_CRITERR_M | PFINT_OICR_HMC_ERR_M | PFINT_OICR_PE_PUSH_M)
	if (oicr & ICE_AUX_CRIT_ERR) {
		pf->oicr_err_reg |= oicr;
		set_bit(ICE_AUX_ERR_PENDING, pf->state);
		ena_mask &= ~ICE_AUX_CRIT_ERR;
	}

	/* Report any remaining unexpected interrupts */
	oicr &= ena_mask;
	if (oicr) {
		dev_dbg(dev, "unhandled interrupt oicr=0x%08x\n", oicr);
		/* If a critical error is pending there is no choice but to
		 * reset the device.
		 */
		if (oicr & (PFINT_OICR_PCI_EXCEPTION_M |
			    PFINT_OICR_ECC_ERR_M)) {
			set_bit(ICE_PFR_REQ, pf->state);
			ice_service_task_schedule(pf);
		}
	}
	ret = IRQ_HANDLED;

	ice_service_task_schedule(pf);
	ice_irq_dynamic_ena(hw, NULL, NULL);
>>>>>>> upstream/android-13

	return ret;
}

/**
<<<<<<< HEAD
 * ice_vsi_map_rings_to_vectors - Map VSI rings to interrupt vectors
 * @vsi: the VSI being configured
 *
 * This function maps descriptor rings to the queue-specific vectors allotted
 * through the MSI-X enabling code. On a constrained vector budget, we map Tx
 * and Rx rings to the vector as "efficiently" as possible.
 */
static void ice_vsi_map_rings_to_vectors(struct ice_vsi *vsi)
{
	int q_vectors = vsi->num_q_vectors;
	int tx_rings_rem, rx_rings_rem;
	int v_id;

	/* initially assigning remaining rings count to VSIs num queue value */
	tx_rings_rem = vsi->num_txq;
	rx_rings_rem = vsi->num_rxq;

	for (v_id = 0; v_id < q_vectors; v_id++) {
		struct ice_q_vector *q_vector = vsi->q_vectors[v_id];
		int tx_rings_per_v, rx_rings_per_v, q_id, q_base;

		/* Tx rings mapping to vector */
		tx_rings_per_v = DIV_ROUND_UP(tx_rings_rem, q_vectors - v_id);
		q_vector->num_ring_tx = tx_rings_per_v;
		q_vector->tx.ring = NULL;
		q_base = vsi->num_txq - tx_rings_rem;

		for (q_id = q_base; q_id < (q_base + tx_rings_per_v); q_id++) {
			struct ice_ring *tx_ring = vsi->tx_rings[q_id];

			tx_ring->q_vector = q_vector;
			tx_ring->next = q_vector->tx.ring;
			q_vector->tx.ring = tx_ring;
		}
		tx_rings_rem -= tx_rings_per_v;

		/* Rx rings mapping to vector */
		rx_rings_per_v = DIV_ROUND_UP(rx_rings_rem, q_vectors - v_id);
		q_vector->num_ring_rx = rx_rings_per_v;
		q_vector->rx.ring = NULL;
		q_base = vsi->num_rxq - rx_rings_rem;

		for (q_id = q_base; q_id < (q_base + rx_rings_per_v); q_id++) {
			struct ice_ring *rx_ring = vsi->rx_rings[q_id];

			rx_ring->q_vector = q_vector;
			rx_ring->next = q_vector->rx.ring;
			q_vector->rx.ring = rx_ring;
		}
		rx_rings_rem -= rx_rings_per_v;
	}
}

/**
 * ice_vsi_set_num_qs - Set num queues, descriptors and vectors for a VSI
 * @vsi: the VSI being configured
 *
 * Return 0 on success and a negative value on error
 */
static void ice_vsi_set_num_qs(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;

	switch (vsi->type) {
	case ICE_VSI_PF:
		vsi->alloc_txq = pf->num_lan_tx;
		vsi->alloc_rxq = pf->num_lan_rx;
		vsi->num_desc = ALIGN(ICE_DFLT_NUM_DESC, ICE_REQ_DESC_MULTIPLE);
		vsi->num_q_vectors = max_t(int, pf->num_lan_rx, pf->num_lan_tx);
		break;
	default:
		dev_warn(&vsi->back->pdev->dev, "Unknown VSI type %d\n",
			 vsi->type);
		break;
	}
}

/**
 * ice_vsi_alloc_arrays - Allocate queue and vector pointer arrays for the vsi
 * @vsi: VSI pointer
 * @alloc_qvectors: a bool to specify if q_vectors need to be allocated.
 *
 * On error: returns error code (negative)
 * On success: returns 0
 */
static int ice_vsi_alloc_arrays(struct ice_vsi *vsi, bool alloc_qvectors)
{
	struct ice_pf *pf = vsi->back;

	/* allocate memory for both Tx and Rx ring pointers */
	vsi->tx_rings = devm_kcalloc(&pf->pdev->dev, vsi->alloc_txq,
				     sizeof(struct ice_ring *), GFP_KERNEL);
	if (!vsi->tx_rings)
		goto err_txrings;

	vsi->rx_rings = devm_kcalloc(&pf->pdev->dev, vsi->alloc_rxq,
				     sizeof(struct ice_ring *), GFP_KERNEL);
	if (!vsi->rx_rings)
		goto err_rxrings;

	if (alloc_qvectors) {
		/* allocate memory for q_vector pointers */
		vsi->q_vectors = devm_kcalloc(&pf->pdev->dev,
					      vsi->num_q_vectors,
					      sizeof(struct ice_q_vector *),
					      GFP_KERNEL);
		if (!vsi->q_vectors)
			goto err_vectors;
	}

	return 0;

err_vectors:
	devm_kfree(&pf->pdev->dev, vsi->rx_rings);
err_rxrings:
	devm_kfree(&pf->pdev->dev, vsi->tx_rings);
err_txrings:
	return -ENOMEM;
}

/**
 * ice_msix_clean_rings - MSIX mode Interrupt Handler
 * @irq: interrupt number
 * @data: pointer to a q_vector
 */
static irqreturn_t ice_msix_clean_rings(int __always_unused irq, void *data)
{
	struct ice_q_vector *q_vector = (struct ice_q_vector *)data;

	if (!q_vector->tx.ring && !q_vector->rx.ring)
		return IRQ_HANDLED;

	napi_schedule(&q_vector->napi);

	return IRQ_HANDLED;
}

/**
 * ice_vsi_alloc - Allocates the next available struct vsi in the PF
 * @pf: board private structure
 * @type: type of VSI
 *
 * returns a pointer to a VSI on success, NULL on failure.
 */
static struct ice_vsi *ice_vsi_alloc(struct ice_pf *pf, enum ice_vsi_type type)
{
	struct ice_vsi *vsi = NULL;

	/* Need to protect the allocation of the VSIs at the PF level */
	mutex_lock(&pf->sw_mutex);

	/* If we have already allocated our maximum number of VSIs,
	 * pf->next_vsi will be ICE_NO_VSI. If not, pf->next_vsi index
	 * is available to be populated
	 */
	if (pf->next_vsi == ICE_NO_VSI) {
		dev_dbg(&pf->pdev->dev, "out of VSI slots!\n");
		goto unlock_pf;
	}

	vsi = devm_kzalloc(&pf->pdev->dev, sizeof(*vsi), GFP_KERNEL);
	if (!vsi)
		goto unlock_pf;

	vsi->type = type;
	vsi->back = pf;
	set_bit(__ICE_DOWN, vsi->state);
	vsi->idx = pf->next_vsi;
	vsi->work_lmt = ICE_DFLT_IRQ_WORK;

	ice_vsi_set_num_qs(vsi);

	switch (vsi->type) {
	case ICE_VSI_PF:
		if (ice_vsi_alloc_arrays(vsi, true))
			goto err_rings;

		/* Setup default MSIX irq handler for VSI */
		vsi->irq_handler = ice_msix_clean_rings;
		break;
	default:
		dev_warn(&pf->pdev->dev, "Unknown VSI type %d\n", vsi->type);
		goto unlock_pf;
	}

	/* fill VSI slot in the PF struct */
	pf->vsi[pf->next_vsi] = vsi;

	/* prepare pf->next_vsi for next use */
	pf->next_vsi = ice_get_free_slot(pf->vsi, pf->num_alloc_vsi,
					 pf->next_vsi);
	goto unlock_pf;

err_rings:
	devm_kfree(&pf->pdev->dev, vsi);
	vsi = NULL;
unlock_pf:
	mutex_unlock(&pf->sw_mutex);
	return vsi;
=======
 * ice_dis_ctrlq_interrupts - disable control queue interrupts
 * @hw: pointer to HW structure
 */
static void ice_dis_ctrlq_interrupts(struct ice_hw *hw)
{
	/* disable Admin queue Interrupt causes */
	wr32(hw, PFINT_FW_CTL,
	     rd32(hw, PFINT_FW_CTL) & ~PFINT_FW_CTL_CAUSE_ENA_M);

	/* disable Mailbox queue Interrupt causes */
	wr32(hw, PFINT_MBX_CTL,
	     rd32(hw, PFINT_MBX_CTL) & ~PFINT_MBX_CTL_CAUSE_ENA_M);

	wr32(hw, PFINT_SB_CTL,
	     rd32(hw, PFINT_SB_CTL) & ~PFINT_SB_CTL_CAUSE_ENA_M);

	/* disable Control queue Interrupt causes */
	wr32(hw, PFINT_OICR_CTL,
	     rd32(hw, PFINT_OICR_CTL) & ~PFINT_OICR_CTL_CAUSE_ENA_M);

	ice_flush(hw);
>>>>>>> upstream/android-13
}

/**
 * ice_free_irq_msix_misc - Unroll misc vector setup
 * @pf: board private structure
 */
static void ice_free_irq_msix_misc(struct ice_pf *pf)
{
<<<<<<< HEAD
	/* disable OICR interrupt */
	wr32(&pf->hw, PFINT_OICR_ENA, 0);
	ice_flush(&pf->hw);

	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags) && pf->msix_entries) {
		synchronize_irq(pf->msix_entries[pf->oicr_idx].vector);
		devm_free_irq(&pf->pdev->dev,
			      pf->msix_entries[pf->oicr_idx].vector, pf);
	}

=======
	struct ice_hw *hw = &pf->hw;

	ice_dis_ctrlq_interrupts(hw);

	/* disable OICR interrupt */
	wr32(hw, PFINT_OICR_ENA, 0);
	ice_flush(hw);

	if (pf->msix_entries) {
		synchronize_irq(pf->msix_entries[pf->oicr_idx].vector);
		devm_free_irq(ice_pf_to_dev(pf),
			      pf->msix_entries[pf->oicr_idx].vector, pf);
	}

	pf->num_avail_sw_msix += 1;
>>>>>>> upstream/android-13
	ice_free_res(pf->irq_tracker, pf->oicr_idx, ICE_RES_MISC_VEC_ID);
}

/**
<<<<<<< HEAD
=======
 * ice_ena_ctrlq_interrupts - enable control queue interrupts
 * @hw: pointer to HW structure
 * @reg_idx: HW vector index to associate the control queue interrupts with
 */
static void ice_ena_ctrlq_interrupts(struct ice_hw *hw, u16 reg_idx)
{
	u32 val;

	val = ((reg_idx & PFINT_OICR_CTL_MSIX_INDX_M) |
	       PFINT_OICR_CTL_CAUSE_ENA_M);
	wr32(hw, PFINT_OICR_CTL, val);

	/* enable Admin queue Interrupt causes */
	val = ((reg_idx & PFINT_FW_CTL_MSIX_INDX_M) |
	       PFINT_FW_CTL_CAUSE_ENA_M);
	wr32(hw, PFINT_FW_CTL, val);

	/* enable Mailbox queue Interrupt causes */
	val = ((reg_idx & PFINT_MBX_CTL_MSIX_INDX_M) |
	       PFINT_MBX_CTL_CAUSE_ENA_M);
	wr32(hw, PFINT_MBX_CTL, val);

	/* This enables Sideband queue Interrupt causes */
	val = ((reg_idx & PFINT_SB_CTL_MSIX_INDX_M) |
	       PFINT_SB_CTL_CAUSE_ENA_M);
	wr32(hw, PFINT_SB_CTL, val);

	ice_flush(hw);
}

/**
>>>>>>> upstream/android-13
 * ice_req_irq_msix_misc - Setup the misc vector to handle non queue events
 * @pf: board private structure
 *
 * This sets up the handler for MSIX 0, which is used to manage the
<<<<<<< HEAD
 * non-queue interrupts, e.g. AdminQ and errors.  This is not used
=======
 * non-queue interrupts, e.g. AdminQ and errors. This is not used
>>>>>>> upstream/android-13
 * when in MSI or Legacy interrupt mode.
 */
static int ice_req_irq_msix_misc(struct ice_pf *pf)
{
<<<<<<< HEAD
	struct ice_hw *hw = &pf->hw;
	int oicr_idx, err = 0;
	u8 itr_gran;
	u32 val;

	if (!pf->int_name[0])
		snprintf(pf->int_name, sizeof(pf->int_name) - 1, "%s-%s:misc",
			 dev_driver_string(&pf->pdev->dev),
			 dev_name(&pf->pdev->dev));
=======
	struct device *dev = ice_pf_to_dev(pf);
	struct ice_hw *hw = &pf->hw;
	int oicr_idx, err = 0;

	if (!pf->int_name[0])
		snprintf(pf->int_name, sizeof(pf->int_name) - 1, "%s-%s:misc",
			 dev_driver_string(dev), dev_name(dev));
>>>>>>> upstream/android-13

	/* Do not request IRQ but do enable OICR interrupt since settings are
	 * lost during reset. Note that this function is called only during
	 * rebuild path and not while reset is in progress.
	 */
<<<<<<< HEAD
	if (ice_is_reset_recovery_pending(pf->state))
=======
	if (ice_is_reset_in_progress(pf->state))
>>>>>>> upstream/android-13
		goto skip_req_irq;

	/* reserve one vector in irq_tracker for misc interrupts */
	oicr_idx = ice_get_res(pf, pf->irq_tracker, 1, ICE_RES_MISC_VEC_ID);
	if (oicr_idx < 0)
		return oicr_idx;

<<<<<<< HEAD
	pf->oicr_idx = oicr_idx;

	err = devm_request_irq(&pf->pdev->dev,
			       pf->msix_entries[pf->oicr_idx].vector,
			       ice_misc_intr, 0, pf->int_name, pf);
	if (err) {
		dev_err(&pf->pdev->dev,
			"devm_request_irq for %s failed: %d\n",
			pf->int_name, err);
		ice_free_res(pf->irq_tracker, 1, ICE_RES_MISC_VEC_ID);
=======
	pf->num_avail_sw_msix -= 1;
	pf->oicr_idx = (u16)oicr_idx;

	err = devm_request_irq(dev, pf->msix_entries[pf->oicr_idx].vector,
			       ice_misc_intr, 0, pf->int_name, pf);
	if (err) {
		dev_err(dev, "devm_request_irq for %s failed: %d\n",
			pf->int_name, err);
		ice_free_res(pf->irq_tracker, 1, ICE_RES_MISC_VEC_ID);
		pf->num_avail_sw_msix += 1;
>>>>>>> upstream/android-13
		return err;
	}

skip_req_irq:
	ice_ena_misc_vector(pf);

<<<<<<< HEAD
	val = ((pf->oicr_idx & PFINT_OICR_CTL_MSIX_INDX_M) |
	       PFINT_OICR_CTL_CAUSE_ENA_M);
	wr32(hw, PFINT_OICR_CTL, val);

	/* This enables Admin queue Interrupt causes */
	val = ((pf->oicr_idx & PFINT_FW_CTL_MSIX_INDX_M) |
	       PFINT_FW_CTL_CAUSE_ENA_M);
	wr32(hw, PFINT_FW_CTL, val);

	itr_gran = hw->itr_gran_200;

	wr32(hw, GLINT_ITR(ICE_RX_ITR, pf->oicr_idx),
	     ITR_TO_REG(ICE_ITR_8K, itr_gran));
=======
	ice_ena_ctrlq_interrupts(hw, pf->oicr_idx);
	wr32(hw, GLINT_ITR(ICE_RX_ITR, pf->oicr_idx),
	     ITR_REG_ALIGN(ICE_ITR_8K) >> ICE_ITR_GRAN_S);
>>>>>>> upstream/android-13

	ice_flush(hw);
	ice_irq_dynamic_ena(hw, NULL, NULL);

	return 0;
}

/**
<<<<<<< HEAD
 * ice_vsi_get_qs_contig - Assign a contiguous chunk of queues to VSI
 * @vsi: the VSI getting queues
 *
 * Return 0 on success and a negative value on error
 */
static int ice_vsi_get_qs_contig(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	int offset, ret = 0;

	mutex_lock(&pf->avail_q_mutex);
	/* look for contiguous block of queues for tx */
	offset = bitmap_find_next_zero_area(pf->avail_txqs, ICE_MAX_TXQS,
					    0, vsi->alloc_txq, 0);
	if (offset < ICE_MAX_TXQS) {
		int i;

		bitmap_set(pf->avail_txqs, offset, vsi->alloc_txq);
		for (i = 0; i < vsi->alloc_txq; i++)
			vsi->txq_map[i] = i + offset;
	} else {
		ret = -ENOMEM;
		vsi->tx_mapping_mode = ICE_VSI_MAP_SCATTER;
	}

	/* look for contiguous block of queues for rx */
	offset = bitmap_find_next_zero_area(pf->avail_rxqs, ICE_MAX_RXQS,
					    0, vsi->alloc_rxq, 0);
	if (offset < ICE_MAX_RXQS) {
		int i;

		bitmap_set(pf->avail_rxqs, offset, vsi->alloc_rxq);
		for (i = 0; i < vsi->alloc_rxq; i++)
			vsi->rxq_map[i] = i + offset;
	} else {
		ret = -ENOMEM;
		vsi->rx_mapping_mode = ICE_VSI_MAP_SCATTER;
	}
	mutex_unlock(&pf->avail_q_mutex);

	return ret;
}

/**
 * ice_vsi_get_qs_scatter - Assign a scattered queues to VSI
 * @vsi: the VSI getting queues
 *
 * Return 0 on success and a negative value on error
 */
static int ice_vsi_get_qs_scatter(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	int i, index = 0;

	mutex_lock(&pf->avail_q_mutex);

	if (vsi->tx_mapping_mode == ICE_VSI_MAP_SCATTER) {
		for (i = 0; i < vsi->alloc_txq; i++) {
			index = find_next_zero_bit(pf->avail_txqs,
						   ICE_MAX_TXQS, index);
			if (index < ICE_MAX_TXQS) {
				set_bit(index, pf->avail_txqs);
				vsi->txq_map[i] = index;
			} else {
				goto err_scatter_tx;
			}
		}
	}

	if (vsi->rx_mapping_mode == ICE_VSI_MAP_SCATTER) {
		for (i = 0; i < vsi->alloc_rxq; i++) {
			index = find_next_zero_bit(pf->avail_rxqs,
						   ICE_MAX_RXQS, index);
			if (index < ICE_MAX_RXQS) {
				set_bit(index, pf->avail_rxqs);
				vsi->rxq_map[i] = index;
			} else {
				goto err_scatter_rx;
			}
		}
	}

	mutex_unlock(&pf->avail_q_mutex);
	return 0;

err_scatter_rx:
	/* unflag any queues we have grabbed (i is failed position) */
	for (index = 0; index < i; index++) {
		clear_bit(vsi->rxq_map[index], pf->avail_rxqs);
		vsi->rxq_map[index] = 0;
	}
	i = vsi->alloc_txq;
err_scatter_tx:
	/* i is either position of failed attempt or vsi->alloc_txq */
	for (index = 0; index < i; index++) {
		clear_bit(vsi->txq_map[index], pf->avail_txqs);
		vsi->txq_map[index] = 0;
	}

	mutex_unlock(&pf->avail_q_mutex);
	return -ENOMEM;
}

/**
 * ice_vsi_get_qs - Assign queues from PF to VSI
 * @vsi: the VSI to assign queues to
 *
 * Returns 0 on success and a negative value on error
 */
static int ice_vsi_get_qs(struct ice_vsi *vsi)
{
	int ret = 0;

	vsi->tx_mapping_mode = ICE_VSI_MAP_CONTIG;
	vsi->rx_mapping_mode = ICE_VSI_MAP_CONTIG;

	/* NOTE: ice_vsi_get_qs_contig() will set the rx/tx mapping
	 * modes individually to scatter if assigning contiguous queues
	 * to rx or tx fails
	 */
	ret = ice_vsi_get_qs_contig(vsi);
	if (ret < 0) {
		if (vsi->tx_mapping_mode == ICE_VSI_MAP_SCATTER)
			vsi->alloc_txq = max_t(u16, vsi->alloc_txq,
					       ICE_MAX_SCATTER_TXQS);
		if (vsi->rx_mapping_mode == ICE_VSI_MAP_SCATTER)
			vsi->alloc_rxq = max_t(u16, vsi->alloc_rxq,
					       ICE_MAX_SCATTER_RXQS);
		ret = ice_vsi_get_qs_scatter(vsi);
	}

	return ret;
}

/**
 * ice_vsi_put_qs - Release queues from VSI to PF
 * @vsi: the VSI thats going to release queues
 */
static void ice_vsi_put_qs(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	int i;

	mutex_lock(&pf->avail_q_mutex);

	for (i = 0; i < vsi->alloc_txq; i++) {
		clear_bit(vsi->txq_map[i], pf->avail_txqs);
		vsi->txq_map[i] = ICE_INVAL_Q_INDEX;
	}

	for (i = 0; i < vsi->alloc_rxq; i++) {
		clear_bit(vsi->rxq_map[i], pf->avail_rxqs);
		vsi->rxq_map[i] = ICE_INVAL_Q_INDEX;
	}

	mutex_unlock(&pf->avail_q_mutex);
}

/**
 * ice_free_q_vector - Free memory allocated for a specific interrupt vector
 * @vsi: VSI having the memory freed
 * @v_idx: index of the vector to be freed
 */
static void ice_free_q_vector(struct ice_vsi *vsi, int v_idx)
{
	struct ice_q_vector *q_vector;
	struct ice_ring *ring;

	if (!vsi->q_vectors[v_idx]) {
		dev_dbg(&vsi->back->pdev->dev, "Queue vector at index %d not found\n",
			v_idx);
		return;
	}
	q_vector = vsi->q_vectors[v_idx];

	ice_for_each_ring(ring, q_vector->tx)
		ring->q_vector = NULL;
	ice_for_each_ring(ring, q_vector->rx)
		ring->q_vector = NULL;

	/* only VSI with an associated netdev is set up with NAPI */
	if (vsi->netdev)
		netif_napi_del(&q_vector->napi);

	devm_kfree(&vsi->back->pdev->dev, q_vector);
	vsi->q_vectors[v_idx] = NULL;
}

/**
 * ice_vsi_free_q_vectors - Free memory allocated for interrupt vectors
 * @vsi: the VSI having memory freed
 */
static void ice_vsi_free_q_vectors(struct ice_vsi *vsi)
{
	int v_idx;

	for (v_idx = 0; v_idx < vsi->num_q_vectors; v_idx++)
		ice_free_q_vector(vsi, v_idx);
}

/**
 * ice_cfg_netdev - Setup the netdev flags
 * @vsi: the VSI being configured
 *
 * Returns 0 on success, negative value on failure
 */
static int ice_cfg_netdev(struct ice_vsi *vsi)
{
=======
 * ice_napi_add - register NAPI handler for the VSI
 * @vsi: VSI for which NAPI handler is to be registered
 *
 * This function is only called in the driver's load path. Registering the NAPI
 * handler is done in ice_vsi_alloc_q_vector() for all other cases (i.e. resume,
 * reset/rebuild, etc.)
 */
static void ice_napi_add(struct ice_vsi *vsi)
{
	int v_idx;

	if (!vsi->netdev)
		return;

	ice_for_each_q_vector(vsi, v_idx)
		netif_napi_add(vsi->netdev, &vsi->q_vectors[v_idx]->napi,
			       ice_napi_poll, NAPI_POLL_WEIGHT);
}

/**
 * ice_set_ops - set netdev and ethtools ops for the given netdev
 * @netdev: netdev instance
 */
static void ice_set_ops(struct net_device *netdev)
{
	struct ice_pf *pf = ice_netdev_to_pf(netdev);

	if (ice_is_safe_mode(pf)) {
		netdev->netdev_ops = &ice_netdev_safe_mode_ops;
		ice_set_ethtool_safe_mode_ops(netdev);
		return;
	}

	netdev->netdev_ops = &ice_netdev_ops;
	netdev->udp_tunnel_nic_info = &pf->hw.udp_tunnel_nic;
	ice_set_ethtool_ops(netdev);
}

/**
 * ice_set_netdev_features - set features for the given netdev
 * @netdev: netdev instance
 */
static void ice_set_netdev_features(struct net_device *netdev)
{
	struct ice_pf *pf = ice_netdev_to_pf(netdev);
>>>>>>> upstream/android-13
	netdev_features_t csumo_features;
	netdev_features_t vlano_features;
	netdev_features_t dflt_features;
	netdev_features_t tso_features;
<<<<<<< HEAD
	struct ice_netdev_priv *np;
	struct net_device *netdev;
	u8 mac_addr[ETH_ALEN];

	netdev = alloc_etherdev_mqs(sizeof(struct ice_netdev_priv),
				    vsi->alloc_txq, vsi->alloc_rxq);
	if (!netdev)
		return -ENOMEM;

	vsi->netdev = netdev;
	np = netdev_priv(netdev);
	np->vsi = vsi;

	dflt_features = NETIF_F_SG	|
			NETIF_F_HIGHDMA	|
=======

	if (ice_is_safe_mode(pf)) {
		/* safe mode */
		netdev->features = NETIF_F_SG | NETIF_F_HIGHDMA;
		netdev->hw_features = netdev->features;
		return;
	}

	dflt_features = NETIF_F_SG	|
			NETIF_F_HIGHDMA	|
			NETIF_F_NTUPLE	|
>>>>>>> upstream/android-13
			NETIF_F_RXHASH;

	csumo_features = NETIF_F_RXCSUM	  |
			 NETIF_F_IP_CSUM  |
<<<<<<< HEAD
=======
			 NETIF_F_SCTP_CRC |
>>>>>>> upstream/android-13
			 NETIF_F_IPV6_CSUM;

	vlano_features = NETIF_F_HW_VLAN_CTAG_FILTER |
			 NETIF_F_HW_VLAN_CTAG_TX     |
			 NETIF_F_HW_VLAN_CTAG_RX;

<<<<<<< HEAD
	tso_features = NETIF_F_TSO;

=======
	tso_features = NETIF_F_TSO			|
		       NETIF_F_TSO_ECN			|
		       NETIF_F_TSO6			|
		       NETIF_F_GSO_GRE			|
		       NETIF_F_GSO_UDP_TUNNEL		|
		       NETIF_F_GSO_GRE_CSUM		|
		       NETIF_F_GSO_UDP_TUNNEL_CSUM	|
		       NETIF_F_GSO_PARTIAL		|
		       NETIF_F_GSO_IPXIP4		|
		       NETIF_F_GSO_IPXIP6		|
		       NETIF_F_GSO_UDP_L4;

	netdev->gso_partial_features |= NETIF_F_GSO_UDP_TUNNEL_CSUM |
					NETIF_F_GSO_GRE_CSUM;
>>>>>>> upstream/android-13
	/* set features that user can change */
	netdev->hw_features = dflt_features | csumo_features |
			      vlano_features | tso_features;

<<<<<<< HEAD
=======
	/* add support for HW_CSUM on packets with MPLS header */
	netdev->mpls_features =  NETIF_F_HW_CSUM;

>>>>>>> upstream/android-13
	/* enable features */
	netdev->features |= netdev->hw_features;
	/* encap and VLAN devices inherit default, csumo and tso features */
	netdev->hw_enc_features |= dflt_features | csumo_features |
				   tso_features;
	netdev->vlan_features |= dflt_features | csumo_features |
				 tso_features;
<<<<<<< HEAD

	if (vsi->type == ICE_VSI_PF) {
		SET_NETDEV_DEV(netdev, &vsi->back->pdev->dev);
		ether_addr_copy(mac_addr, vsi->port_info->mac.perm_addr);

=======
}

/**
 * ice_cfg_netdev - Allocate, configure and register a netdev
 * @vsi: the VSI associated with the new netdev
 *
 * Returns 0 on success, negative value on failure
 */
static int ice_cfg_netdev(struct ice_vsi *vsi)
{
	struct ice_netdev_priv *np;
	struct net_device *netdev;
	u8 mac_addr[ETH_ALEN];

	netdev = alloc_etherdev_mqs(sizeof(*np), vsi->alloc_txq,
				    vsi->alloc_rxq);
	if (!netdev)
		return -ENOMEM;

	set_bit(ICE_VSI_NETDEV_ALLOCD, vsi->state);
	vsi->netdev = netdev;
	np = netdev_priv(netdev);
	np->vsi = vsi;

	ice_set_netdev_features(netdev);

	ice_set_ops(netdev);

	if (vsi->type == ICE_VSI_PF) {
		SET_NETDEV_DEV(netdev, ice_pf_to_dev(vsi->back));
		ether_addr_copy(mac_addr, vsi->port_info->mac.perm_addr);
>>>>>>> upstream/android-13
		ether_addr_copy(netdev->dev_addr, mac_addr);
		ether_addr_copy(netdev->perm_addr, mac_addr);
	}

	netdev->priv_flags |= IFF_UNICAST_FLT;

<<<<<<< HEAD
	/* assign netdev_ops */
	netdev->netdev_ops = &ice_netdev_ops;
=======
	/* Setup netdev TC information */
	ice_vsi_cfg_netdev_tc(vsi, vsi->tc_cfg.ena_tc);
>>>>>>> upstream/android-13

	/* setup watchdog timeout value to be 5 second */
	netdev->watchdog_timeo = 5 * HZ;

<<<<<<< HEAD
	ice_set_ethtool_ops(netdev);

=======
>>>>>>> upstream/android-13
	netdev->min_mtu = ETH_MIN_MTU;
	netdev->max_mtu = ICE_MAX_MTU;

	return 0;
}

/**
<<<<<<< HEAD
 * ice_vsi_free_arrays - clean up vsi resources
 * @vsi: pointer to VSI being cleared
 * @free_qvectors: bool to specify if q_vectors should be deallocated
 */
static void ice_vsi_free_arrays(struct ice_vsi *vsi, bool free_qvectors)
{
	struct ice_pf *pf = vsi->back;

	/* free the ring and vector containers */
	if (free_qvectors && vsi->q_vectors) {
		devm_kfree(&pf->pdev->dev, vsi->q_vectors);
		vsi->q_vectors = NULL;
	}
	if (vsi->tx_rings) {
		devm_kfree(&pf->pdev->dev, vsi->tx_rings);
		vsi->tx_rings = NULL;
	}
	if (vsi->rx_rings) {
		devm_kfree(&pf->pdev->dev, vsi->rx_rings);
		vsi->rx_rings = NULL;
	}
}

/**
 * ice_vsi_clear - clean up and deallocate the provided vsi
 * @vsi: pointer to VSI being cleared
 *
 * This deallocates the vsi's queue resources, removes it from the PF's
 * VSI array if necessary, and deallocates the VSI
 *
 * Returns 0 on success, negative on failure
 */
static int ice_vsi_clear(struct ice_vsi *vsi)
{
	struct ice_pf *pf = NULL;

	if (!vsi)
		return 0;

	if (!vsi->back)
		return -EINVAL;

	pf = vsi->back;

	if (!pf->vsi[vsi->idx] || pf->vsi[vsi->idx] != vsi) {
		dev_dbg(&pf->pdev->dev, "vsi does not exist at pf->vsi[%d]\n",
			vsi->idx);
		return -EINVAL;
	}

	mutex_lock(&pf->sw_mutex);
	/* updates the PF for this cleared vsi */

	pf->vsi[vsi->idx] = NULL;
	if (vsi->idx < pf->next_vsi)
		pf->next_vsi = vsi->idx;

	ice_vsi_free_arrays(vsi, true);
	mutex_unlock(&pf->sw_mutex);
	devm_kfree(&pf->pdev->dev, vsi);

	return 0;
}

/**
 * ice_vsi_alloc_q_vector - Allocate memory for a single interrupt vector
 * @vsi: the VSI being configured
 * @v_idx: index of the vector in the vsi struct
 *
 * We allocate one q_vector.  If allocation fails we return -ENOMEM.
 */
static int ice_vsi_alloc_q_vector(struct ice_vsi *vsi, int v_idx)
{
	struct ice_pf *pf = vsi->back;
	struct ice_q_vector *q_vector;

	/* allocate q_vector */
	q_vector = devm_kzalloc(&pf->pdev->dev, sizeof(*q_vector), GFP_KERNEL);
	if (!q_vector)
		return -ENOMEM;

	q_vector->vsi = vsi;
	q_vector->v_idx = v_idx;
	/* only set affinity_mask if the CPU is online */
	if (cpu_online(v_idx))
		cpumask_set_cpu(v_idx, &q_vector->affinity_mask);

	if (vsi->netdev)
		netif_napi_add(vsi->netdev, &q_vector->napi, ice_napi_poll,
			       NAPI_POLL_WEIGHT);
	/* tie q_vector and vsi together */
	vsi->q_vectors[v_idx] = q_vector;

	return 0;
}

/**
 * ice_vsi_alloc_q_vectors - Allocate memory for interrupt vectors
 * @vsi: the VSI being configured
 *
 * We allocate one q_vector per queue interrupt.  If allocation fails we
 * return -ENOMEM.
 */
static int ice_vsi_alloc_q_vectors(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	int v_idx = 0, num_q_vectors;
	int err;

	if (vsi->q_vectors[0]) {
		dev_dbg(&pf->pdev->dev, "VSI %d has existing q_vectors\n",
			vsi->vsi_num);
		return -EEXIST;
	}

	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags)) {
		num_q_vectors = vsi->num_q_vectors;
	} else {
		err = -EINVAL;
		goto err_out;
	}

	for (v_idx = 0; v_idx < num_q_vectors; v_idx++) {
		err = ice_vsi_alloc_q_vector(vsi, v_idx);
		if (err)
			goto err_out;
	}

	return 0;

err_out:
	while (v_idx--)
		ice_free_q_vector(vsi, v_idx);

	dev_err(&pf->pdev->dev,
		"Failed to allocate %d q_vector for VSI %d, ret=%d\n",
		vsi->num_q_vectors, vsi->vsi_num, err);
	vsi->num_q_vectors = 0;
	return err;
}

/**
 * ice_vsi_setup_vector_base - Set up the base vector for the given VSI
 * @vsi: ptr to the VSI
 *
 * This should only be called after ice_vsi_alloc() which allocates the
 * corresponding SW VSI structure and initializes num_queue_pairs for the
 * newly allocated VSI.
 *
 * Returns 0 on success or negative on failure
 */
static int ice_vsi_setup_vector_base(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	int num_q_vectors = 0;

	if (vsi->base_vector) {
		dev_dbg(&pf->pdev->dev, "VSI %d has non-zero base vector %d\n",
			vsi->vsi_num, vsi->base_vector);
		return -EEXIST;
	}

	if (!test_bit(ICE_FLAG_MSIX_ENA, pf->flags))
		return -ENOENT;

	switch (vsi->type) {
	case ICE_VSI_PF:
		num_q_vectors = vsi->num_q_vectors;
		break;
	default:
		dev_warn(&vsi->back->pdev->dev, "Unknown VSI type %d\n",
			 vsi->type);
		break;
	}

	if (num_q_vectors)
		vsi->base_vector = ice_get_res(pf, pf->irq_tracker,
					       num_q_vectors, vsi->idx);

	if (vsi->base_vector < 0) {
		dev_err(&pf->pdev->dev,
			"Failed to get tracking for %d vectors for VSI %d, err=%d\n",
			num_q_vectors, vsi->vsi_num, vsi->base_vector);
		return -ENOENT;
	}

	return 0;
}

/**
=======
>>>>>>> upstream/android-13
 * ice_fill_rss_lut - Fill the RSS lookup table with default values
 * @lut: Lookup table
 * @rss_table_size: Lookup table size
 * @rss_size: Range of queue number for hashing
 */
void ice_fill_rss_lut(u8 *lut, u16 rss_table_size, u16 rss_size)
{
	u16 i;

	for (i = 0; i < rss_table_size; i++)
		lut[i] = i % rss_size;
}

/**
<<<<<<< HEAD
 * ice_vsi_cfg_rss - Configure RSS params for a VSI
 * @vsi: VSI to be configured
 */
static int ice_vsi_cfg_rss(struct ice_vsi *vsi)
{
	u8 seed[ICE_AQC_GET_SET_RSS_KEY_DATA_RSS_KEY_SIZE];
	struct ice_aqc_get_set_rss_keys *key;
	struct ice_pf *pf = vsi->back;
	enum ice_status status;
	int err = 0;
	u8 *lut;

	vsi->rss_size = min_t(int, vsi->rss_size, vsi->num_rxq);

	lut = devm_kzalloc(&pf->pdev->dev, vsi->rss_table_size, GFP_KERNEL);
	if (!lut)
		return -ENOMEM;

	if (vsi->rss_lut_user)
		memcpy(lut, vsi->rss_lut_user, vsi->rss_table_size);
	else
		ice_fill_rss_lut(lut, vsi->rss_table_size, vsi->rss_size);

	status = ice_aq_set_rss_lut(&pf->hw, vsi->vsi_num, vsi->rss_lut_type,
				    lut, vsi->rss_table_size);

	if (status) {
		dev_err(&vsi->back->pdev->dev,
			"set_rss_lut failed, error %d\n", status);
		err = -EIO;
		goto ice_vsi_cfg_rss_exit;
	}

	key = devm_kzalloc(&vsi->back->pdev->dev, sizeof(*key), GFP_KERNEL);
	if (!key) {
		err = -ENOMEM;
		goto ice_vsi_cfg_rss_exit;
	}

	if (vsi->rss_hkey_user)
		memcpy(seed, vsi->rss_hkey_user,
		       ICE_AQC_GET_SET_RSS_KEY_DATA_RSS_KEY_SIZE);
	else
		netdev_rss_key_fill((void *)seed,
				    ICE_AQC_GET_SET_RSS_KEY_DATA_RSS_KEY_SIZE);
	memcpy(&key->standard_rss_key, seed,
	       ICE_AQC_GET_SET_RSS_KEY_DATA_RSS_KEY_SIZE);

	status = ice_aq_set_rss_key(&pf->hw, vsi->vsi_num, key);

	if (status) {
		dev_err(&vsi->back->pdev->dev, "set_rss_key failed, error %d\n",
			status);
		err = -EIO;
	}

	devm_kfree(&pf->pdev->dev, key);
ice_vsi_cfg_rss_exit:
	devm_kfree(&pf->pdev->dev, lut);
	return err;
}

/**
 * ice_vsi_reinit_setup - return resource and reallocate resource for a VSI
 * @vsi: pointer to the ice_vsi
 *
 * This reallocates the VSIs queue resources
 *
 * Returns 0 on success and negative value on failure
 */
static int ice_vsi_reinit_setup(struct ice_vsi *vsi)
{
	u16 max_txqs[ICE_MAX_TRAFFIC_CLASS] = { 0 };
	int ret, i;

	if (!vsi)
		return -EINVAL;

	ice_vsi_free_q_vectors(vsi);
	ice_free_res(vsi->back->irq_tracker, vsi->base_vector, vsi->idx);
	vsi->base_vector = 0;
	ice_vsi_clear_rings(vsi);
	ice_vsi_free_arrays(vsi, false);
	ice_vsi_set_num_qs(vsi);

	/* Initialize VSI struct elements and create VSI in FW */
	ret = ice_vsi_add(vsi);
	if (ret < 0)
		goto err_vsi;

	ret = ice_vsi_alloc_arrays(vsi, false);
	if (ret < 0)
		goto err_vsi;

	switch (vsi->type) {
	case ICE_VSI_PF:
		if (!vsi->netdev) {
			ret = ice_cfg_netdev(vsi);
			if (ret)
				goto err_rings;

			ret = register_netdev(vsi->netdev);
			if (ret)
				goto err_rings;

			netif_carrier_off(vsi->netdev);
			netif_tx_stop_all_queues(vsi->netdev);
		}

		ret = ice_vsi_alloc_q_vectors(vsi);
		if (ret)
			goto err_rings;

		ret = ice_vsi_setup_vector_base(vsi);
		if (ret)
			goto err_vectors;

		ret = ice_vsi_alloc_rings(vsi);
		if (ret)
			goto err_vectors;

		ice_vsi_map_rings_to_vectors(vsi);
		break;
	default:
		break;
	}

	ice_vsi_set_tc_cfg(vsi);

	/* configure VSI nodes based on number of queues and TC's */
	for (i = 0; i < vsi->tc_cfg.numtc; i++)
		max_txqs[i] = vsi->num_txq;

	ret = ice_cfg_vsi_lan(vsi->port_info, vsi->vsi_num,
			      vsi->tc_cfg.ena_tc, max_txqs);
	if (ret) {
		dev_info(&vsi->back->pdev->dev,
			 "Failed VSI lan queue config\n");
		goto err_vectors;
	}
	return 0;

err_vectors:
	ice_vsi_free_q_vectors(vsi);
err_rings:
	if (vsi->netdev) {
		vsi->current_netdev_flags = 0;
		unregister_netdev(vsi->netdev);
		free_netdev(vsi->netdev);
		vsi->netdev = NULL;
	}
err_vsi:
	ice_vsi_clear(vsi);
	set_bit(__ICE_RESET_FAILED, vsi->back->state);
	return ret;
}

/**
 * ice_vsi_setup - Set up a VSI by a given type
 * @pf: board private structure
 * @type: VSI type
 * @pi: pointer to the port_info instance
 *
 * This allocates the sw VSI structure and its queue resources.
 *
 * Returns pointer to the successfully allocated and configure VSI sw struct on
 * success, otherwise returns NULL on failure.
 */
static struct ice_vsi *
ice_vsi_setup(struct ice_pf *pf, enum ice_vsi_type type,
	      struct ice_port_info *pi)
{
	u16 max_txqs[ICE_MAX_TRAFFIC_CLASS] = { 0 };
	struct device *dev = &pf->pdev->dev;
	struct ice_vsi_ctx ctxt = { 0 };
	struct ice_vsi *vsi;
	int ret, i;

	vsi = ice_vsi_alloc(pf, type);
	if (!vsi) {
		dev_err(dev, "could not allocate VSI\n");
		return NULL;
	}

	vsi->port_info = pi;
	vsi->vsw = pf->first_sw;

	if (ice_vsi_get_qs(vsi)) {
		dev_err(dev, "Failed to allocate queues. vsi->idx = %d\n",
			vsi->idx);
		goto err_get_qs;
	}

	/* set RSS capabilities */
	ice_vsi_set_rss_params(vsi);

	/* create the VSI */
	ret = ice_vsi_add(vsi);
	if (ret)
		goto err_vsi;

	ctxt.vsi_num = vsi->vsi_num;

	switch (vsi->type) {
	case ICE_VSI_PF:
		ret = ice_cfg_netdev(vsi);
		if (ret)
			goto err_cfg_netdev;

		ret = register_netdev(vsi->netdev);
		if (ret)
			goto err_register_netdev;

		netif_carrier_off(vsi->netdev);

		/* make sure transmit queues start off as stopped */
		netif_tx_stop_all_queues(vsi->netdev);
		ret = ice_vsi_alloc_q_vectors(vsi);
		if (ret)
			goto err_msix;

		ret = ice_vsi_setup_vector_base(vsi);
		if (ret)
			goto err_rings;

		ret = ice_vsi_alloc_rings(vsi);
		if (ret)
			goto err_rings;

		ice_vsi_map_rings_to_vectors(vsi);

		/* Do not exit if configuring RSS had an issue, at least
		 * receive traffic on first queue. Hence no need to capture
		 * return value
		 */
		if (test_bit(ICE_FLAG_RSS_ENA, pf->flags))
			ice_vsi_cfg_rss(vsi);
		break;
	default:
		/* if vsi type is not recognized, clean up the resources and
		 * exit
		 */
		goto err_rings;
	}

	ice_vsi_set_tc_cfg(vsi);

	/* configure VSI nodes based on number of queues and TC's */
	for (i = 0; i < vsi->tc_cfg.numtc; i++)
		max_txqs[i] = vsi->num_txq;

	ret = ice_cfg_vsi_lan(vsi->port_info, vsi->vsi_num,
			      vsi->tc_cfg.ena_tc, max_txqs);
	if (ret) {
		dev_info(&pf->pdev->dev, "Failed VSI lan queue config\n");
		goto err_rings;
	}

	return vsi;

err_rings:
	ice_vsi_free_q_vectors(vsi);
err_msix:
	if (vsi->netdev && vsi->netdev->reg_state == NETREG_REGISTERED)
		unregister_netdev(vsi->netdev);
err_register_netdev:
	if (vsi->netdev) {
		free_netdev(vsi->netdev);
		vsi->netdev = NULL;
	}
err_cfg_netdev:
	ret = ice_aq_free_vsi(&pf->hw, &ctxt, false, NULL);
	if (ret)
		dev_err(&vsi->back->pdev->dev,
			"Free VSI AQ call failed, err %d\n", ret);
err_vsi:
	ice_vsi_put_qs(vsi);
err_get_qs:
	pf->q_left_tx += vsi->alloc_txq;
	pf->q_left_rx += vsi->alloc_rxq;
	ice_vsi_clear(vsi);

	return NULL;
}

/**
 * ice_vsi_add_vlan - Add vsi membership for given vlan
 * @vsi: the vsi being configured
 * @vid: vlan id to be added
 */
static int ice_vsi_add_vlan(struct ice_vsi *vsi, u16 vid)
{
	struct ice_fltr_list_entry *tmp;
	struct ice_pf *pf = vsi->back;
	LIST_HEAD(tmp_add_list);
	enum ice_status status;
	int err = 0;

	tmp = devm_kzalloc(&pf->pdev->dev, sizeof(*tmp), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;

	tmp->fltr_info.lkup_type = ICE_SW_LKUP_VLAN;
	tmp->fltr_info.fltr_act = ICE_FWD_TO_VSI;
	tmp->fltr_info.flag = ICE_FLTR_TX;
	tmp->fltr_info.src = vsi->vsi_num;
	tmp->fltr_info.fwd_id.vsi_id = vsi->vsi_num;
	tmp->fltr_info.l_data.vlan.vlan_id = vid;

	INIT_LIST_HEAD(&tmp->list_entry);
	list_add(&tmp->list_entry, &tmp_add_list);

	status = ice_add_vlan(&pf->hw, &tmp_add_list);
	if (status) {
		err = -ENODEV;
		dev_err(&pf->pdev->dev, "Failure Adding VLAN %d on VSI %i\n",
			vid, vsi->vsi_num);
	}

	ice_free_fltr_list(&pf->pdev->dev, &tmp_add_list);
	return err;
}

/**
 * ice_vlan_rx_add_vid - Add a vlan id filter to HW offload
 * @netdev: network interface to be adjusted
 * @proto: unused protocol
 * @vid: vlan id to be added
 *
 * net_device_ops implementation for adding vlan ids
 */
static int ice_vlan_rx_add_vid(struct net_device *netdev,
			       __always_unused __be16 proto, u16 vid)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	int ret = 0;

	if (vid >= VLAN_N_VID) {
		netdev_err(netdev, "VLAN id requested %d is out of range %d\n",
			   vid, VLAN_N_VID);
		return -EINVAL;
	}

	if (vsi->info.pvid)
		return -EINVAL;

	/* Add all VLAN ids including 0 to the switch filter. VLAN id 0 is
	 * needed to continue allowing all untagged packets since VLAN prune
	 * list is applied to all packets by the switch
	 */
	ret = ice_vsi_add_vlan(vsi, vid);

	if (!ret)
		set_bit(vid, vsi->active_vlans);
=======
 * ice_pf_vsi_setup - Set up a PF VSI
 * @pf: board private structure
 * @pi: pointer to the port_info instance
 *
 * Returns pointer to the successfully allocated VSI software struct
 * on success, otherwise returns NULL on failure.
 */
static struct ice_vsi *
ice_pf_vsi_setup(struct ice_pf *pf, struct ice_port_info *pi)
{
	return ice_vsi_setup(pf, pi, ICE_VSI_PF, ICE_INVAL_VFID);
}

/**
 * ice_ctrl_vsi_setup - Set up a control VSI
 * @pf: board private structure
 * @pi: pointer to the port_info instance
 *
 * Returns pointer to the successfully allocated VSI software struct
 * on success, otherwise returns NULL on failure.
 */
static struct ice_vsi *
ice_ctrl_vsi_setup(struct ice_pf *pf, struct ice_port_info *pi)
{
	return ice_vsi_setup(pf, pi, ICE_VSI_CTRL, ICE_INVAL_VFID);
}

/**
 * ice_lb_vsi_setup - Set up a loopback VSI
 * @pf: board private structure
 * @pi: pointer to the port_info instance
 *
 * Returns pointer to the successfully allocated VSI software struct
 * on success, otherwise returns NULL on failure.
 */
struct ice_vsi *
ice_lb_vsi_setup(struct ice_pf *pf, struct ice_port_info *pi)
{
	return ice_vsi_setup(pf, pi, ICE_VSI_LB, ICE_INVAL_VFID);
}

/**
 * ice_vlan_rx_add_vid - Add a VLAN ID filter to HW offload
 * @netdev: network interface to be adjusted
 * @proto: unused protocol
 * @vid: VLAN ID to be added
 *
 * net_device_ops implementation for adding VLAN IDs
 */
static int
ice_vlan_rx_add_vid(struct net_device *netdev, __always_unused __be16 proto,
		    u16 vid)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	int ret;

	/* VLAN 0 is added by default during load/reset */
	if (!vid)
		return 0;

	/* Enable VLAN pruning when a VLAN other than 0 is added */
	if (!ice_vsi_is_vlan_pruning_ena(vsi)) {
		ret = ice_cfg_vlan_pruning(vsi, true, false);
		if (ret)
			return ret;
	}

	/* Add a switch rule for this VLAN ID so its corresponding VLAN tagged
	 * packets aren't pruned by the device's internal switch on Rx
	 */
	ret = ice_vsi_add_vlan(vsi, vid, ICE_FWD_TO_VSI);
	if (!ret)
		set_bit(ICE_VSI_VLAN_FLTR_CHANGED, vsi->state);
>>>>>>> upstream/android-13

	return ret;
}

/**
<<<<<<< HEAD
 * ice_vsi_kill_vlan - Remove VSI membership for a given VLAN
 * @vsi: the VSI being configured
 * @vid: VLAN id to be removed
 */
static void ice_vsi_kill_vlan(struct ice_vsi *vsi, u16 vid)
{
	struct ice_fltr_list_entry *list;
	struct ice_pf *pf = vsi->back;
	LIST_HEAD(tmp_add_list);

	list = devm_kzalloc(&pf->pdev->dev, sizeof(*list), GFP_KERNEL);
	if (!list)
		return;

	list->fltr_info.lkup_type = ICE_SW_LKUP_VLAN;
	list->fltr_info.fwd_id.vsi_id = vsi->vsi_num;
	list->fltr_info.fltr_act = ICE_FWD_TO_VSI;
	list->fltr_info.l_data.vlan.vlan_id = vid;
	list->fltr_info.flag = ICE_FLTR_TX;
	list->fltr_info.src = vsi->vsi_num;

	INIT_LIST_HEAD(&list->list_entry);
	list_add(&list->list_entry, &tmp_add_list);

	if (ice_remove_vlan(&pf->hw, &tmp_add_list))
		dev_err(&pf->pdev->dev, "Error removing VLAN %d on vsi %i\n",
			vid, vsi->vsi_num);

	ice_free_fltr_list(&pf->pdev->dev, &tmp_add_list);
}

/**
 * ice_vlan_rx_kill_vid - Remove a vlan id filter from HW offload
 * @netdev: network interface to be adjusted
 * @proto: unused protocol
 * @vid: vlan id to be removed
 *
 * net_device_ops implementation for removing vlan ids
 */
static int ice_vlan_rx_kill_vid(struct net_device *netdev,
				__always_unused __be16 proto, u16 vid)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;

	if (vsi->info.pvid)
		return -EINVAL;

	/* return code is ignored as there is nothing a user
	 * can do about failure to remove and a log message was
	 * already printed from the other function
	 */
	ice_vsi_kill_vlan(vsi, vid);

	clear_bit(vid, vsi->active_vlans);

	return 0;
=======
 * ice_vlan_rx_kill_vid - Remove a VLAN ID filter from HW offload
 * @netdev: network interface to be adjusted
 * @proto: unused protocol
 * @vid: VLAN ID to be removed
 *
 * net_device_ops implementation for removing VLAN IDs
 */
static int
ice_vlan_rx_kill_vid(struct net_device *netdev, __always_unused __be16 proto,
		     u16 vid)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	int ret;

	/* don't allow removal of VLAN 0 */
	if (!vid)
		return 0;

	/* Make sure ice_vsi_kill_vlan is successful before updating VLAN
	 * information
	 */
	ret = ice_vsi_kill_vlan(vsi, vid);
	if (ret)
		return ret;

	/* Disable pruning when VLAN 0 is the only VLAN rule */
	if (vsi->num_vlan == 1 && ice_vsi_is_vlan_pruning_ena(vsi))
		ret = ice_cfg_vlan_pruning(vsi, false, false);

	set_bit(ICE_VSI_VLAN_FLTR_CHANGED, vsi->state);
	return ret;
>>>>>>> upstream/android-13
}

/**
 * ice_setup_pf_sw - Setup the HW switch on startup or after reset
 * @pf: board private structure
 *
 * Returns 0 on success, negative value on failure
 */
static int ice_setup_pf_sw(struct ice_pf *pf)
{
<<<<<<< HEAD
	LIST_HEAD(tmp_add_list);
	u8 broadcast[ETH_ALEN];
	struct ice_vsi *vsi;
	int status = 0;

	if (!ice_is_reset_recovery_pending(pf->state)) {
		vsi = ice_vsi_setup(pf, ICE_VSI_PF, pf->hw.port_info);
		if (!vsi) {
			status = -ENOMEM;
			goto error_exit;
		}
	} else {
		vsi = pf->vsi[0];
		status = ice_vsi_reinit_setup(vsi);
		if (status < 0)
			return -EIO;
	}

	/* tmp_add_list contains a list of MAC addresses for which MAC
	 * filters need to be programmed. Add the VSI's unicast MAC to
	 * this list
	 */
	status = ice_add_mac_to_list(vsi, &tmp_add_list,
				     vsi->port_info->mac.perm_addr);
	if (status)
		goto error_exit;

	/* VSI needs to receive broadcast traffic, so add the broadcast
	 * MAC address to the list.
	 */
	eth_broadcast_addr(broadcast);
	status = ice_add_mac_to_list(vsi, &tmp_add_list, broadcast);
	if (status)
		goto error_exit;

	/* program MAC filters for entries in tmp_add_list */
	status = ice_add_mac(&pf->hw, &tmp_add_list);
	if (status) {
		dev_err(&pf->pdev->dev, "Could not add MAC filters\n");
		status = -ENOMEM;
		goto error_exit;
	}

	ice_free_fltr_list(&pf->pdev->dev, &tmp_add_list);
	return status;

error_exit:
	ice_free_fltr_list(&pf->pdev->dev, &tmp_add_list);

	if (vsi) {
		ice_vsi_free_q_vectors(vsi);
		if (vsi->netdev && vsi->netdev->reg_state == NETREG_REGISTERED)
			unregister_netdev(vsi->netdev);
		if (vsi->netdev) {
			free_netdev(vsi->netdev);
			vsi->netdev = NULL;
		}

		ice_vsi_delete(vsi);
		ice_vsi_put_qs(vsi);
		pf->q_left_tx += vsi->alloc_txq;
		pf->q_left_rx += vsi->alloc_rxq;
		ice_vsi_clear(vsi);
	}
=======
	struct ice_vsi *vsi;
	int status = 0;

	if (ice_is_reset_in_progress(pf->state))
		return -EBUSY;

	vsi = ice_pf_vsi_setup(pf, pf->hw.port_info);
	if (!vsi)
		return -ENOMEM;

	status = ice_cfg_netdev(vsi);
	if (status) {
		status = -ENODEV;
		goto unroll_vsi_setup;
	}
	/* netdev has to be configured before setting frame size */
	ice_vsi_cfg_frame_size(vsi);

	/* Setup DCB netlink interface */
	ice_dcbnl_setup(vsi);

	/* registering the NAPI handler requires both the queues and
	 * netdev to be created, which are done in ice_pf_vsi_setup()
	 * and ice_cfg_netdev() respectively
	 */
	ice_napi_add(vsi);

	status = ice_set_cpu_rx_rmap(vsi);
	if (status) {
		dev_err(ice_pf_to_dev(pf), "Failed to set CPU Rx map VSI %d error %d\n",
			vsi->vsi_num, status);
		status = -EINVAL;
		goto unroll_napi_add;
	}
	status = ice_init_mac_fltr(pf);
	if (status)
		goto free_cpu_rx_map;

	return status;

free_cpu_rx_map:
	ice_free_cpu_rx_rmap(vsi);

unroll_napi_add:
	if (vsi) {
		ice_napi_del(vsi);
		if (vsi->netdev) {
			clear_bit(ICE_VSI_NETDEV_ALLOCD, vsi->state);
			free_netdev(vsi->netdev);
			vsi->netdev = NULL;
		}
	}

unroll_vsi_setup:
	ice_vsi_release(vsi);
>>>>>>> upstream/android-13
	return status;
}

/**
<<<<<<< HEAD
 * ice_determine_q_usage - Calculate queue distribution
 * @pf: board private structure
 *
 * Return -ENOMEM if we don't get enough queues for all ports
 */
static void ice_determine_q_usage(struct ice_pf *pf)
{
	u16 q_left_tx, q_left_rx;

	q_left_tx = pf->hw.func_caps.common_cap.num_txq;
	q_left_rx = pf->hw.func_caps.common_cap.num_rxq;

	pf->num_lan_tx = min_t(int, q_left_tx, num_online_cpus());

	/* only 1 rx queue unless RSS is enabled */
	if (!test_bit(ICE_FLAG_RSS_ENA, pf->flags))
		pf->num_lan_rx = 1;
	else
		pf->num_lan_rx = min_t(int, q_left_rx, num_online_cpus());

	pf->q_left_tx = q_left_tx - pf->num_lan_tx;
	pf->q_left_rx = q_left_rx - pf->num_lan_rx;
=======
 * ice_get_avail_q_count - Get count of queues in use
 * @pf_qmap: bitmap to get queue use count from
 * @lock: pointer to a mutex that protects access to pf_qmap
 * @size: size of the bitmap
 */
static u16
ice_get_avail_q_count(unsigned long *pf_qmap, struct mutex *lock, u16 size)
{
	unsigned long bit;
	u16 count = 0;

	mutex_lock(lock);
	for_each_clear_bit(bit, pf_qmap, size)
		count++;
	mutex_unlock(lock);

	return count;
}

/**
 * ice_get_avail_txq_count - Get count of Tx queues in use
 * @pf: pointer to an ice_pf instance
 */
u16 ice_get_avail_txq_count(struct ice_pf *pf)
{
	return ice_get_avail_q_count(pf->avail_txqs, &pf->avail_q_mutex,
				     pf->max_pf_txqs);
}

/**
 * ice_get_avail_rxq_count - Get count of Rx queues in use
 * @pf: pointer to an ice_pf instance
 */
u16 ice_get_avail_rxq_count(struct ice_pf *pf)
{
	return ice_get_avail_q_count(pf->avail_rxqs, &pf->avail_q_mutex,
				     pf->max_pf_rxqs);
>>>>>>> upstream/android-13
}

/**
 * ice_deinit_pf - Unrolls initialziations done by ice_init_pf
 * @pf: board private structure to initialize
 */
static void ice_deinit_pf(struct ice_pf *pf)
{
<<<<<<< HEAD
	if (pf->serv_tmr.function)
		del_timer_sync(&pf->serv_tmr);
	if (pf->serv_task.func)
		cancel_work_sync(&pf->serv_task);
	mutex_destroy(&pf->sw_mutex);
	mutex_destroy(&pf->avail_q_mutex);
=======
	ice_service_task_stop(pf);
	mutex_destroy(&pf->adev_mutex);
	mutex_destroy(&pf->sw_mutex);
	mutex_destroy(&pf->tc_mutex);
	mutex_destroy(&pf->avail_q_mutex);

	if (pf->avail_txqs) {
		bitmap_free(pf->avail_txqs);
		pf->avail_txqs = NULL;
	}

	if (pf->avail_rxqs) {
		bitmap_free(pf->avail_rxqs);
		pf->avail_rxqs = NULL;
	}

	if (pf->ptp.clock)
		ptp_clock_unregister(pf->ptp.clock);
}

/**
 * ice_set_pf_caps - set PFs capability flags
 * @pf: pointer to the PF instance
 */
static void ice_set_pf_caps(struct ice_pf *pf)
{
	struct ice_hw_func_caps *func_caps = &pf->hw.func_caps;

	clear_bit(ICE_FLAG_RDMA_ENA, pf->flags);
	clear_bit(ICE_FLAG_AUX_ENA, pf->flags);
	if (func_caps->common_cap.rdma) {
		set_bit(ICE_FLAG_RDMA_ENA, pf->flags);
		set_bit(ICE_FLAG_AUX_ENA, pf->flags);
	}
	clear_bit(ICE_FLAG_DCB_CAPABLE, pf->flags);
	if (func_caps->common_cap.dcb)
		set_bit(ICE_FLAG_DCB_CAPABLE, pf->flags);
	clear_bit(ICE_FLAG_SRIOV_CAPABLE, pf->flags);
	if (func_caps->common_cap.sr_iov_1_1) {
		set_bit(ICE_FLAG_SRIOV_CAPABLE, pf->flags);
		pf->num_vfs_supported = min_t(int, func_caps->num_allocd_vfs,
					      ICE_MAX_VF_COUNT);
	}
	clear_bit(ICE_FLAG_RSS_ENA, pf->flags);
	if (func_caps->common_cap.rss_table_size)
		set_bit(ICE_FLAG_RSS_ENA, pf->flags);

	clear_bit(ICE_FLAG_FD_ENA, pf->flags);
	if (func_caps->fd_fltr_guar > 0 || func_caps->fd_fltr_best_effort > 0) {
		u16 unused;

		/* ctrl_vsi_idx will be set to a valid value when flow director
		 * is setup by ice_init_fdir
		 */
		pf->ctrl_vsi_idx = ICE_NO_VSI;
		set_bit(ICE_FLAG_FD_ENA, pf->flags);
		/* force guaranteed filter pool for PF */
		ice_alloc_fd_guar_item(&pf->hw, &unused,
				       func_caps->fd_fltr_guar);
		/* force shared filter pool for PF */
		ice_alloc_fd_shrd_item(&pf->hw, &unused,
				       func_caps->fd_fltr_best_effort);
	}

	clear_bit(ICE_FLAG_PTP_SUPPORTED, pf->flags);
	if (func_caps->common_cap.ieee_1588)
		set_bit(ICE_FLAG_PTP_SUPPORTED, pf->flags);

	pf->max_pf_txqs = func_caps->common_cap.num_txq;
	pf->max_pf_rxqs = func_caps->common_cap.num_rxq;
>>>>>>> upstream/android-13
}

/**
 * ice_init_pf - Initialize general software structures (struct ice_pf)
 * @pf: board private structure to initialize
 */
<<<<<<< HEAD
static void ice_init_pf(struct ice_pf *pf)
{
	bitmap_zero(pf->flags, ICE_PF_FLAGS_NBITS);
	set_bit(ICE_FLAG_MSIX_ENA, pf->flags);

	mutex_init(&pf->sw_mutex);
	mutex_init(&pf->avail_q_mutex);

	/* Clear avail_[t|r]x_qs bitmaps (set all to avail) */
	mutex_lock(&pf->avail_q_mutex);
	bitmap_zero(pf->avail_txqs, ICE_MAX_TXQS);
	bitmap_zero(pf->avail_rxqs, ICE_MAX_RXQS);
	mutex_unlock(&pf->avail_q_mutex);

	if (pf->hw.func_caps.common_cap.rss_table_size)
		set_bit(ICE_FLAG_RSS_ENA, pf->flags);
=======
static int ice_init_pf(struct ice_pf *pf)
{
	ice_set_pf_caps(pf);

	mutex_init(&pf->sw_mutex);
	mutex_init(&pf->tc_mutex);
	mutex_init(&pf->adev_mutex);

	INIT_HLIST_HEAD(&pf->aq_wait_list);
	spin_lock_init(&pf->aq_wait_lock);
	init_waitqueue_head(&pf->aq_wait_queue);

	init_waitqueue_head(&pf->reset_wait_queue);
>>>>>>> upstream/android-13

	/* setup service timer and periodic service task */
	timer_setup(&pf->serv_tmr, ice_service_timer, 0);
	pf->serv_tmr_period = HZ;
	INIT_WORK(&pf->serv_task, ice_service_task);
<<<<<<< HEAD
	clear_bit(__ICE_SERVICE_SCHED, pf->state);
=======
	clear_bit(ICE_SERVICE_SCHED, pf->state);

	mutex_init(&pf->avail_q_mutex);
	pf->avail_txqs = bitmap_zalloc(pf->max_pf_txqs, GFP_KERNEL);
	if (!pf->avail_txqs)
		return -ENOMEM;

	pf->avail_rxqs = bitmap_zalloc(pf->max_pf_rxqs, GFP_KERNEL);
	if (!pf->avail_rxqs) {
		devm_kfree(ice_pf_to_dev(pf), pf->avail_txqs);
		pf->avail_txqs = NULL;
		return -ENOMEM;
	}

	return 0;
>>>>>>> upstream/android-13
}

/**
 * ice_ena_msix_range - Request a range of MSIX vectors from the OS
 * @pf: board private structure
 *
 * compute the number of MSIX vectors required (v_budget) and request from
 * the OS. Return the number of vectors reserved or negative on failure
 */
static int ice_ena_msix_range(struct ice_pf *pf)
{
<<<<<<< HEAD
	int v_left, v_actual, v_budget = 0;
	int needed, err, i;

	v_left = pf->hw.func_caps.common_cap.num_msix_vectors;

	/* reserve one vector for miscellaneous handler */
	needed = 1;
	v_budget += needed;
	v_left -= needed;

	/* reserve vectors for LAN traffic */
	pf->num_lan_msix = min_t(int, num_online_cpus(), v_left);
	v_budget += pf->num_lan_msix;

	pf->msix_entries = devm_kcalloc(&pf->pdev->dev, v_budget,
					sizeof(struct msix_entry), GFP_KERNEL);

=======
	int num_cpus, v_left, v_actual, v_other, v_budget = 0;
	struct device *dev = ice_pf_to_dev(pf);
	int needed, err, i;

	v_left = pf->hw.func_caps.common_cap.num_msix_vectors;
	num_cpus = num_online_cpus();

	/* reserve for LAN miscellaneous handler */
	needed = ICE_MIN_LAN_OICR_MSIX;
	if (v_left < needed)
		goto no_hw_vecs_left_err;
	v_budget += needed;
	v_left -= needed;

	/* reserve for flow director */
	if (test_bit(ICE_FLAG_FD_ENA, pf->flags)) {
		needed = ICE_FDIR_MSIX;
		if (v_left < needed)
			goto no_hw_vecs_left_err;
		v_budget += needed;
		v_left -= needed;
	}

	/* total used for non-traffic vectors */
	v_other = v_budget;

	/* reserve vectors for LAN traffic */
	needed = num_cpus;
	if (v_left < needed)
		goto no_hw_vecs_left_err;
	pf->num_lan_msix = needed;
	v_budget += needed;
	v_left -= needed;

	/* reserve vectors for RDMA auxiliary driver */
	if (test_bit(ICE_FLAG_RDMA_ENA, pf->flags)) {
		needed = num_cpus + ICE_RDMA_NUM_AEQ_MSIX;
		if (v_left < needed)
			goto no_hw_vecs_left_err;
		pf->num_rdma_msix = needed;
		v_budget += needed;
		v_left -= needed;
	}

	pf->msix_entries = devm_kcalloc(dev, v_budget,
					sizeof(*pf->msix_entries), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!pf->msix_entries) {
		err = -ENOMEM;
		goto exit_err;
	}

	for (i = 0; i < v_budget; i++)
		pf->msix_entries[i].entry = i;

	/* actually reserve the vectors */
	v_actual = pci_enable_msix_range(pf->pdev, pf->msix_entries,
					 ICE_MIN_MSIX, v_budget);
<<<<<<< HEAD

	if (v_actual < 0) {
		dev_err(&pf->pdev->dev, "unable to reserve MSI-X vectors\n");
=======
	if (v_actual < 0) {
		dev_err(dev, "unable to reserve MSI-X vectors\n");
>>>>>>> upstream/android-13
		err = v_actual;
		goto msix_err;
	}

	if (v_actual < v_budget) {
<<<<<<< HEAD
		dev_warn(&pf->pdev->dev,
			 "not enough vectors. requested = %d, obtained = %d\n",
			 v_budget, v_actual);
		if (v_actual >= (pf->num_lan_msix + 1)) {
			pf->num_avail_msix = v_actual - (pf->num_lan_msix + 1);
		} else if (v_actual >= 2) {
			pf->num_lan_msix = 1;
			pf->num_avail_msix = v_actual - 2;
		} else {
			pci_disable_msix(pf->pdev);
			err = -ERANGE;
			goto msix_err;
=======
		dev_warn(dev, "not enough OS MSI-X vectors. requested = %d, obtained = %d\n",
			 v_budget, v_actual);

		if (v_actual < ICE_MIN_MSIX) {
			/* error if we can't get minimum vectors */
			pci_disable_msix(pf->pdev);
			err = -ERANGE;
			goto msix_err;
		} else {
			int v_remain = v_actual - v_other;
			int v_rdma = 0, v_min_rdma = 0;

			if (test_bit(ICE_FLAG_RDMA_ENA, pf->flags)) {
				/* Need at least 1 interrupt in addition to
				 * AEQ MSIX
				 */
				v_rdma = ICE_RDMA_NUM_AEQ_MSIX + 1;
				v_min_rdma = ICE_MIN_RDMA_MSIX;
			}

			if (v_actual == ICE_MIN_MSIX ||
			    v_remain < ICE_MIN_LAN_TXRX_MSIX + v_min_rdma) {
				dev_warn(dev, "Not enough MSI-X vectors to support RDMA.\n");
				clear_bit(ICE_FLAG_RDMA_ENA, pf->flags);

				pf->num_rdma_msix = 0;
				pf->num_lan_msix = ICE_MIN_LAN_TXRX_MSIX;
			} else if ((v_remain < ICE_MIN_LAN_TXRX_MSIX + v_rdma) ||
				   (v_remain - v_rdma < v_rdma)) {
				/* Support minimum RDMA and give remaining
				 * vectors to LAN MSIX
				 */
				pf->num_rdma_msix = v_min_rdma;
				pf->num_lan_msix = v_remain - v_min_rdma;
			} else {
				/* Split remaining MSIX with RDMA after
				 * accounting for AEQ MSIX
				 */
				pf->num_rdma_msix = (v_remain - ICE_RDMA_NUM_AEQ_MSIX) / 2 +
						    ICE_RDMA_NUM_AEQ_MSIX;
				pf->num_lan_msix = v_remain - pf->num_rdma_msix;
			}

			dev_notice(dev, "Enabled %d MSI-X vectors for LAN traffic.\n",
				   pf->num_lan_msix);

			if (test_bit(ICE_FLAG_RDMA_ENA, pf->flags))
				dev_notice(dev, "Enabled %d MSI-X vectors for RDMA.\n",
					   pf->num_rdma_msix);
>>>>>>> upstream/android-13
		}
	}

	return v_actual;

msix_err:
<<<<<<< HEAD
	devm_kfree(&pf->pdev->dev, pf->msix_entries);
	goto exit_err;

exit_err:
	pf->num_lan_msix = 0;
	clear_bit(ICE_FLAG_MSIX_ENA, pf->flags);
=======
	devm_kfree(dev, pf->msix_entries);
	goto exit_err;

no_hw_vecs_left_err:
	dev_err(dev, "not enough device MSI-X vectors. requested = %d, available = %d\n",
		needed, v_left);
	err = -ERANGE;
exit_err:
	pf->num_rdma_msix = 0;
	pf->num_lan_msix = 0;
>>>>>>> upstream/android-13
	return err;
}

/**
 * ice_dis_msix - Disable MSI-X interrupt setup in OS
 * @pf: board private structure
 */
static void ice_dis_msix(struct ice_pf *pf)
{
	pci_disable_msix(pf->pdev);
<<<<<<< HEAD
	devm_kfree(&pf->pdev->dev, pf->msix_entries);
	pf->msix_entries = NULL;
	clear_bit(ICE_FLAG_MSIX_ENA, pf->flags);
}

/**
 * ice_init_interrupt_scheme - Determine proper interrupt scheme
 * @pf: board private structure to initialize
 */
static int ice_init_interrupt_scheme(struct ice_pf *pf)
{
	int vectors = 0;
	ssize_t size;

	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags))
		vectors = ice_ena_msix_range(pf);
	else
		return -ENODEV;

	if (vectors < 0)
		return vectors;

	/* set up vector assignment tracking */
	size = sizeof(struct ice_res_tracker) + (sizeof(u16) * vectors);

	pf->irq_tracker = devm_kzalloc(&pf->pdev->dev, size, GFP_KERNEL);
	if (!pf->irq_tracker) {
		ice_dis_msix(pf);
		return -ENOMEM;
	}

	pf->irq_tracker->num_entries = vectors;

	return 0;
=======
	devm_kfree(ice_pf_to_dev(pf), pf->msix_entries);
	pf->msix_entries = NULL;
>>>>>>> upstream/android-13
}

/**
 * ice_clear_interrupt_scheme - Undo things done by ice_init_interrupt_scheme
 * @pf: board private structure
 */
static void ice_clear_interrupt_scheme(struct ice_pf *pf)
{
<<<<<<< HEAD
	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags))
		ice_dis_msix(pf);

	if (pf->irq_tracker) {
		devm_kfree(&pf->pdev->dev, pf->irq_tracker);
=======
	ice_dis_msix(pf);

	if (pf->irq_tracker) {
		devm_kfree(ice_pf_to_dev(pf), pf->irq_tracker);
>>>>>>> upstream/android-13
		pf->irq_tracker = NULL;
	}
}

/**
<<<<<<< HEAD
=======
 * ice_init_interrupt_scheme - Determine proper interrupt scheme
 * @pf: board private structure to initialize
 */
static int ice_init_interrupt_scheme(struct ice_pf *pf)
{
	int vectors;

	vectors = ice_ena_msix_range(pf);

	if (vectors < 0)
		return vectors;

	/* set up vector assignment tracking */
	pf->irq_tracker = devm_kzalloc(ice_pf_to_dev(pf),
				       struct_size(pf->irq_tracker, list, vectors),
				       GFP_KERNEL);
	if (!pf->irq_tracker) {
		ice_dis_msix(pf);
		return -ENOMEM;
	}

	/* populate SW interrupts pool with number of OS granted IRQs. */
	pf->num_avail_sw_msix = (u16)vectors;
	pf->irq_tracker->num_entries = (u16)vectors;
	pf->irq_tracker->end = pf->irq_tracker->num_entries;

	return 0;
}

/**
 * ice_is_wol_supported - check if WoL is supported
 * @hw: pointer to hardware info
 *
 * Check if WoL is supported based on the HW configuration.
 * Returns true if NVM supports and enables WoL for this port, false otherwise
 */
bool ice_is_wol_supported(struct ice_hw *hw)
{
	u16 wol_ctrl;

	/* A bit set to 1 in the NVM Software Reserved Word 2 (WoL control
	 * word) indicates WoL is not supported on the corresponding PF ID.
	 */
	if (ice_read_sr_word(hw, ICE_SR_NVM_WOL_CFG, &wol_ctrl))
		return false;

	return !(BIT(hw->port_info->lport) & wol_ctrl);
}

/**
 * ice_vsi_recfg_qs - Change the number of queues on a VSI
 * @vsi: VSI being changed
 * @new_rx: new number of Rx queues
 * @new_tx: new number of Tx queues
 *
 * Only change the number of queues if new_tx, or new_rx is non-0.
 *
 * Returns 0 on success.
 */
int ice_vsi_recfg_qs(struct ice_vsi *vsi, int new_rx, int new_tx)
{
	struct ice_pf *pf = vsi->back;
	int err = 0, timeout = 50;

	if (!new_rx && !new_tx)
		return -EINVAL;

	while (test_and_set_bit(ICE_CFG_BUSY, pf->state)) {
		timeout--;
		if (!timeout)
			return -EBUSY;
		usleep_range(1000, 2000);
	}

	if (new_tx)
		vsi->req_txq = (u16)new_tx;
	if (new_rx)
		vsi->req_rxq = (u16)new_rx;

	/* set for the next time the netdev is started */
	if (!netif_running(vsi->netdev)) {
		ice_vsi_rebuild(vsi, false);
		dev_dbg(ice_pf_to_dev(pf), "Link is down, queue count change happens when link is brought up\n");
		goto done;
	}

	ice_vsi_close(vsi);
	ice_vsi_rebuild(vsi, false);
	ice_pf_dcb_recfg(pf);
	ice_vsi_open(vsi);
done:
	clear_bit(ICE_CFG_BUSY, pf->state);
	return err;
}

/**
 * ice_set_safe_mode_vlan_cfg - configure PF VSI to allow all VLANs in safe mode
 * @pf: PF to configure
 *
 * No VLAN offloads/filtering are advertised in safe mode so make sure the PF
 * VSI can still Tx/Rx VLAN tagged packets.
 */
static void ice_set_safe_mode_vlan_cfg(struct ice_pf *pf)
{
	struct ice_vsi *vsi = ice_get_main_vsi(pf);
	struct ice_vsi_ctx *ctxt;
	enum ice_status status;
	struct ice_hw *hw;

	if (!vsi)
		return;

	ctxt = kzalloc(sizeof(*ctxt), GFP_KERNEL);
	if (!ctxt)
		return;

	hw = &pf->hw;
	ctxt->info = vsi->info;

	ctxt->info.valid_sections =
		cpu_to_le16(ICE_AQ_VSI_PROP_VLAN_VALID |
			    ICE_AQ_VSI_PROP_SECURITY_VALID |
			    ICE_AQ_VSI_PROP_SW_VALID);

	/* disable VLAN anti-spoof */
	ctxt->info.sec_flags &= ~(ICE_AQ_VSI_SEC_TX_VLAN_PRUNE_ENA <<
				  ICE_AQ_VSI_SEC_TX_PRUNE_ENA_S);

	/* disable VLAN pruning and keep all other settings */
	ctxt->info.sw_flags2 &= ~ICE_AQ_VSI_SW_FLAG_RX_VLAN_PRUNE_ENA;

	/* allow all VLANs on Tx and don't strip on Rx */
	ctxt->info.vlan_flags = ICE_AQ_VSI_VLAN_MODE_ALL |
		ICE_AQ_VSI_VLAN_EMOD_NOTHING;

	status = ice_update_vsi(hw, vsi->idx, ctxt, NULL);
	if (status) {
		dev_err(ice_pf_to_dev(vsi->back), "Failed to update VSI for safe mode VLANs, err %s aq_err %s\n",
			ice_stat_str(status),
			ice_aq_str(hw->adminq.sq_last_status));
	} else {
		vsi->info.sec_flags = ctxt->info.sec_flags;
		vsi->info.sw_flags2 = ctxt->info.sw_flags2;
		vsi->info.vlan_flags = ctxt->info.vlan_flags;
	}

	kfree(ctxt);
}

/**
 * ice_log_pkg_init - log result of DDP package load
 * @hw: pointer to hardware info
 * @status: status of package load
 */
static void
ice_log_pkg_init(struct ice_hw *hw, enum ice_status *status)
{
	struct ice_pf *pf = (struct ice_pf *)hw->back;
	struct device *dev = ice_pf_to_dev(pf);

	switch (*status) {
	case ICE_SUCCESS:
		/* The package download AdminQ command returned success because
		 * this download succeeded or ICE_ERR_AQ_NO_WORK since there is
		 * already a package loaded on the device.
		 */
		if (hw->pkg_ver.major == hw->active_pkg_ver.major &&
		    hw->pkg_ver.minor == hw->active_pkg_ver.minor &&
		    hw->pkg_ver.update == hw->active_pkg_ver.update &&
		    hw->pkg_ver.draft == hw->active_pkg_ver.draft &&
		    !memcmp(hw->pkg_name, hw->active_pkg_name,
			    sizeof(hw->pkg_name))) {
			if (hw->pkg_dwnld_status == ICE_AQ_RC_EEXIST)
				dev_info(dev, "DDP package already present on device: %s version %d.%d.%d.%d\n",
					 hw->active_pkg_name,
					 hw->active_pkg_ver.major,
					 hw->active_pkg_ver.minor,
					 hw->active_pkg_ver.update,
					 hw->active_pkg_ver.draft);
			else
				dev_info(dev, "The DDP package was successfully loaded: %s version %d.%d.%d.%d\n",
					 hw->active_pkg_name,
					 hw->active_pkg_ver.major,
					 hw->active_pkg_ver.minor,
					 hw->active_pkg_ver.update,
					 hw->active_pkg_ver.draft);
		} else if (hw->active_pkg_ver.major != ICE_PKG_SUPP_VER_MAJ ||
			   hw->active_pkg_ver.minor != ICE_PKG_SUPP_VER_MNR) {
			dev_err(dev, "The device has a DDP package that is not supported by the driver.  The device has package '%s' version %d.%d.x.x.  The driver requires version %d.%d.x.x.  Entering Safe Mode.\n",
				hw->active_pkg_name,
				hw->active_pkg_ver.major,
				hw->active_pkg_ver.minor,
				ICE_PKG_SUPP_VER_MAJ, ICE_PKG_SUPP_VER_MNR);
			*status = ICE_ERR_NOT_SUPPORTED;
		} else if (hw->active_pkg_ver.major == ICE_PKG_SUPP_VER_MAJ &&
			   hw->active_pkg_ver.minor == ICE_PKG_SUPP_VER_MNR) {
			dev_info(dev, "The driver could not load the DDP package file because a compatible DDP package is already present on the device.  The device has package '%s' version %d.%d.%d.%d.  The package file found by the driver: '%s' version %d.%d.%d.%d.\n",
				 hw->active_pkg_name,
				 hw->active_pkg_ver.major,
				 hw->active_pkg_ver.minor,
				 hw->active_pkg_ver.update,
				 hw->active_pkg_ver.draft,
				 hw->pkg_name,
				 hw->pkg_ver.major,
				 hw->pkg_ver.minor,
				 hw->pkg_ver.update,
				 hw->pkg_ver.draft);
		} else {
			dev_err(dev, "An unknown error occurred when loading the DDP package, please reboot the system.  If the problem persists, update the NVM.  Entering Safe Mode.\n");
			*status = ICE_ERR_NOT_SUPPORTED;
		}
		break;
	case ICE_ERR_FW_DDP_MISMATCH:
		dev_err(dev, "The firmware loaded on the device is not compatible with the DDP package.  Please update the device's NVM.  Entering safe mode.\n");
		break;
	case ICE_ERR_BUF_TOO_SHORT:
	case ICE_ERR_CFG:
		dev_err(dev, "The DDP package file is invalid. Entering Safe Mode.\n");
		break;
	case ICE_ERR_NOT_SUPPORTED:
		/* Package File version not supported */
		if (hw->pkg_ver.major > ICE_PKG_SUPP_VER_MAJ ||
		    (hw->pkg_ver.major == ICE_PKG_SUPP_VER_MAJ &&
		     hw->pkg_ver.minor > ICE_PKG_SUPP_VER_MNR))
			dev_err(dev, "The DDP package file version is higher than the driver supports.  Please use an updated driver.  Entering Safe Mode.\n");
		else if (hw->pkg_ver.major < ICE_PKG_SUPP_VER_MAJ ||
			 (hw->pkg_ver.major == ICE_PKG_SUPP_VER_MAJ &&
			  hw->pkg_ver.minor < ICE_PKG_SUPP_VER_MNR))
			dev_err(dev, "The DDP package file version is lower than the driver supports.  The driver requires version %d.%d.x.x.  Please use an updated DDP Package file.  Entering Safe Mode.\n",
				ICE_PKG_SUPP_VER_MAJ, ICE_PKG_SUPP_VER_MNR);
		break;
	case ICE_ERR_AQ_ERROR:
		switch (hw->pkg_dwnld_status) {
		case ICE_AQ_RC_ENOSEC:
		case ICE_AQ_RC_EBADSIG:
			dev_err(dev, "The DDP package could not be loaded because its signature is not valid.  Please use a valid DDP Package.  Entering Safe Mode.\n");
			return;
		case ICE_AQ_RC_ESVN:
			dev_err(dev, "The DDP Package could not be loaded because its security revision is too low.  Please use an updated DDP Package.  Entering Safe Mode.\n");
			return;
		case ICE_AQ_RC_EBADMAN:
		case ICE_AQ_RC_EBADBUF:
			dev_err(dev, "An error occurred on the device while loading the DDP package.  The device will be reset.\n");
			/* poll for reset to complete */
			if (ice_check_reset(hw))
				dev_err(dev, "Error resetting device. Please reload the driver\n");
			return;
		default:
			break;
		}
		fallthrough;
	default:
		dev_err(dev, "An unknown error (%d) occurred when loading the DDP package.  Entering Safe Mode.\n",
			*status);
		break;
	}
}

/**
 * ice_load_pkg - load/reload the DDP Package file
 * @firmware: firmware structure when firmware requested or NULL for reload
 * @pf: pointer to the PF instance
 *
 * Called on probe and post CORER/GLOBR rebuild to load DDP Package and
 * initialize HW tables.
 */
static void
ice_load_pkg(const struct firmware *firmware, struct ice_pf *pf)
{
	enum ice_status status = ICE_ERR_PARAM;
	struct device *dev = ice_pf_to_dev(pf);
	struct ice_hw *hw = &pf->hw;

	/* Load DDP Package */
	if (firmware && !hw->pkg_copy) {
		status = ice_copy_and_init_pkg(hw, firmware->data,
					       firmware->size);
		ice_log_pkg_init(hw, &status);
	} else if (!firmware && hw->pkg_copy) {
		/* Reload package during rebuild after CORER/GLOBR reset */
		status = ice_init_pkg(hw, hw->pkg_copy, hw->pkg_size);
		ice_log_pkg_init(hw, &status);
	} else {
		dev_err(dev, "The DDP package file failed to load. Entering Safe Mode.\n");
	}

	if (status) {
		/* Safe Mode */
		clear_bit(ICE_FLAG_ADV_FEATURES, pf->flags);
		return;
	}

	/* Successful download package is the precondition for advanced
	 * features, hence setting the ICE_FLAG_ADV_FEATURES flag
	 */
	set_bit(ICE_FLAG_ADV_FEATURES, pf->flags);
}

/**
 * ice_verify_cacheline_size - verify driver's assumption of 64 Byte cache lines
 * @pf: pointer to the PF structure
 *
 * There is no error returned here because the driver should be able to handle
 * 128 Byte cache lines, so we only print a warning in case issues are seen,
 * specifically with Tx.
 */
static void ice_verify_cacheline_size(struct ice_pf *pf)
{
	if (rd32(&pf->hw, GLPCI_CNF2) & GLPCI_CNF2_CACHELINE_SIZE_M)
		dev_warn(ice_pf_to_dev(pf), "%d Byte cache line assumption is invalid, driver may have Tx timeouts!\n",
			 ICE_CACHE_LINE_BYTES);
}

/**
 * ice_send_version - update firmware with driver version
 * @pf: PF struct
 *
 * Returns ICE_SUCCESS on success, else error code
 */
static enum ice_status ice_send_version(struct ice_pf *pf)
{
	struct ice_driver_ver dv;

	dv.major_ver = 0xff;
	dv.minor_ver = 0xff;
	dv.build_ver = 0xff;
	dv.subbuild_ver = 0;
	strscpy((char *)dv.driver_string, UTS_RELEASE,
		sizeof(dv.driver_string));
	return ice_aq_send_driver_ver(&pf->hw, &dv, NULL);
}

/**
 * ice_init_fdir - Initialize flow director VSI and configuration
 * @pf: pointer to the PF instance
 *
 * returns 0 on success, negative on error
 */
static int ice_init_fdir(struct ice_pf *pf)
{
	struct device *dev = ice_pf_to_dev(pf);
	struct ice_vsi *ctrl_vsi;
	int err;

	/* Side Band Flow Director needs to have a control VSI.
	 * Allocate it and store it in the PF.
	 */
	ctrl_vsi = ice_ctrl_vsi_setup(pf, pf->hw.port_info);
	if (!ctrl_vsi) {
		dev_dbg(dev, "could not create control VSI\n");
		return -ENOMEM;
	}

	err = ice_vsi_open_ctrl(ctrl_vsi);
	if (err) {
		dev_dbg(dev, "could not open control VSI\n");
		goto err_vsi_open;
	}

	mutex_init(&pf->hw.fdir_fltr_lock);

	err = ice_fdir_create_dflt_rules(pf);
	if (err)
		goto err_fdir_rule;

	return 0;

err_fdir_rule:
	ice_fdir_release_flows(&pf->hw);
	ice_vsi_close(ctrl_vsi);
err_vsi_open:
	ice_vsi_release(ctrl_vsi);
	if (pf->ctrl_vsi_idx != ICE_NO_VSI) {
		pf->vsi[pf->ctrl_vsi_idx] = NULL;
		pf->ctrl_vsi_idx = ICE_NO_VSI;
	}
	return err;
}

/**
 * ice_get_opt_fw_name - return optional firmware file name or NULL
 * @pf: pointer to the PF instance
 */
static char *ice_get_opt_fw_name(struct ice_pf *pf)
{
	/* Optional firmware name same as default with additional dash
	 * followed by a EUI-64 identifier (PCIe Device Serial Number)
	 */
	struct pci_dev *pdev = pf->pdev;
	char *opt_fw_filename;
	u64 dsn;

	/* Determine the name of the optional file using the DSN (two
	 * dwords following the start of the DSN Capability).
	 */
	dsn = pci_get_dsn(pdev);
	if (!dsn)
		return NULL;

	opt_fw_filename = kzalloc(NAME_MAX, GFP_KERNEL);
	if (!opt_fw_filename)
		return NULL;

	snprintf(opt_fw_filename, NAME_MAX, "%sice-%016llx.pkg",
		 ICE_DDP_PKG_PATH, dsn);

	return opt_fw_filename;
}

/**
 * ice_request_fw - Device initialization routine
 * @pf: pointer to the PF instance
 */
static void ice_request_fw(struct ice_pf *pf)
{
	char *opt_fw_filename = ice_get_opt_fw_name(pf);
	const struct firmware *firmware = NULL;
	struct device *dev = ice_pf_to_dev(pf);
	int err = 0;

	/* optional device-specific DDP (if present) overrides the default DDP
	 * package file. kernel logs a debug message if the file doesn't exist,
	 * and warning messages for other errors.
	 */
	if (opt_fw_filename) {
		err = firmware_request_nowarn(&firmware, opt_fw_filename, dev);
		if (err) {
			kfree(opt_fw_filename);
			goto dflt_pkg_load;
		}

		/* request for firmware was successful. Download to device */
		ice_load_pkg(firmware, pf);
		kfree(opt_fw_filename);
		release_firmware(firmware);
		return;
	}

dflt_pkg_load:
	err = request_firmware(&firmware, ICE_DDP_PKG_FILE, dev);
	if (err) {
		dev_err(dev, "The DDP package file was not found or could not be read. Entering Safe Mode\n");
		return;
	}

	/* request for firmware was successful. Download to device */
	ice_load_pkg(firmware, pf);
	release_firmware(firmware);
}

/**
 * ice_print_wake_reason - show the wake up cause in the log
 * @pf: pointer to the PF struct
 */
static void ice_print_wake_reason(struct ice_pf *pf)
{
	u32 wus = pf->wakeup_reason;
	const char *wake_str;

	/* if no wake event, nothing to print */
	if (!wus)
		return;

	if (wus & PFPM_WUS_LNKC_M)
		wake_str = "Link\n";
	else if (wus & PFPM_WUS_MAG_M)
		wake_str = "Magic Packet\n";
	else if (wus & PFPM_WUS_MNG_M)
		wake_str = "Management\n";
	else if (wus & PFPM_WUS_FW_RST_WK_M)
		wake_str = "Firmware Reset\n";
	else
		wake_str = "Unknown\n";

	dev_info(ice_pf_to_dev(pf), "Wake reason: %s", wake_str);
}

/**
 * ice_register_netdev - register netdev and devlink port
 * @pf: pointer to the PF struct
 */
static int ice_register_netdev(struct ice_pf *pf)
{
	struct ice_vsi *vsi;
	int err = 0;

	vsi = ice_get_main_vsi(pf);
	if (!vsi || !vsi->netdev)
		return -EIO;

	err = register_netdev(vsi->netdev);
	if (err)
		goto err_register_netdev;

	set_bit(ICE_VSI_NETDEV_REGISTERED, vsi->state);
	netif_carrier_off(vsi->netdev);
	netif_tx_stop_all_queues(vsi->netdev);
	err = ice_devlink_create_pf_port(pf);
	if (err)
		goto err_devlink_create;

	devlink_port_type_eth_set(&pf->devlink_port, vsi->netdev);

	return 0;
err_devlink_create:
	unregister_netdev(vsi->netdev);
	clear_bit(ICE_VSI_NETDEV_REGISTERED, vsi->state);
err_register_netdev:
	free_netdev(vsi->netdev);
	vsi->netdev = NULL;
	clear_bit(ICE_VSI_NETDEV_ALLOCD, vsi->state);
	return err;
}

/**
>>>>>>> upstream/android-13
 * ice_probe - Device initialization routine
 * @pdev: PCI device information struct
 * @ent: entry in ice_pci_tbl
 *
 * Returns 0 on success, negative on failure
 */
<<<<<<< HEAD
static int ice_probe(struct pci_dev *pdev,
		     const struct pci_device_id __always_unused *ent)
{
	struct ice_pf *pf;
	struct ice_hw *hw;
	int err;

	/* this driver uses devres, see Documentation/driver-model/devres.txt */
=======
static int
ice_probe(struct pci_dev *pdev, const struct pci_device_id __always_unused *ent)
{
	struct device *dev = &pdev->dev;
	struct ice_pf *pf;
	struct ice_hw *hw;
	int i, err;

	if (pdev->is_virtfn) {
		dev_err(dev, "can't probe a virtual function\n");
		return -EINVAL;
	}

	/* this driver uses devres, see
	 * Documentation/driver-api/driver-model/devres.rst
	 */
>>>>>>> upstream/android-13
	err = pcim_enable_device(pdev);
	if (err)
		return err;

<<<<<<< HEAD
	err = pcim_iomap_regions(pdev, BIT(ICE_BAR0), pci_name(pdev));
	if (err) {
		dev_err(&pdev->dev, "BAR0 I/O map error %d\n", err);
		return err;
	}

	pf = devm_kzalloc(&pdev->dev, sizeof(*pf), GFP_KERNEL);
	if (!pf)
		return -ENOMEM;

	/* set up for high or low dma */
	err = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));
	if (err)
		err = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	if (err) {
		dev_err(&pdev->dev, "DMA configuration failed: 0x%x\n", err);
=======
	err = pcim_iomap_regions(pdev, BIT(ICE_BAR0), dev_driver_string(dev));
	if (err) {
		dev_err(dev, "BAR0 I/O map error %d\n", err);
		return err;
	}

	pf = ice_allocate_pf(dev);
	if (!pf)
		return -ENOMEM;

	/* initialize Auxiliary index to invalid value */
	pf->aux_idx = -1;

	/* set up for high or low DMA */
	err = dma_set_mask_and_coherent(dev, DMA_BIT_MASK(64));
	if (err)
		err = dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32));
	if (err) {
		dev_err(dev, "DMA configuration failed: 0x%x\n", err);
>>>>>>> upstream/android-13
		return err;
	}

	pci_enable_pcie_error_reporting(pdev);
	pci_set_master(pdev);

	pf->pdev = pdev;
	pci_set_drvdata(pdev, pf);
<<<<<<< HEAD
	set_bit(__ICE_DOWN, pf->state);

	hw = &pf->hw;
	hw->hw_addr = pcim_iomap_table(pdev)[ICE_BAR0];
=======
	set_bit(ICE_DOWN, pf->state);
	/* Disable service task until DOWN bit is cleared */
	set_bit(ICE_SERVICE_DIS, pf->state);

	hw = &pf->hw;
	hw->hw_addr = pcim_iomap_table(pdev)[ICE_BAR0];
	pci_save_state(pdev);

>>>>>>> upstream/android-13
	hw->back = pf;
	hw->vendor_id = pdev->vendor;
	hw->device_id = pdev->device;
	pci_read_config_byte(pdev, PCI_REVISION_ID, &hw->revision_id);
	hw->subsystem_vendor_id = pdev->subsystem_vendor;
	hw->subsystem_device_id = pdev->subsystem_device;
	hw->bus.device = PCI_SLOT(pdev->devfn);
	hw->bus.func = PCI_FUNC(pdev->devfn);
	ice_set_ctrlq_len(hw);

	pf->msg_enable = netif_msg_init(debug, ICE_DFLT_NETIF_M);

<<<<<<< HEAD
=======
	err = ice_devlink_register(pf);
	if (err) {
		dev_err(dev, "ice_devlink_register failed: %d\n", err);
		goto err_exit_unroll;
	}

>>>>>>> upstream/android-13
#ifndef CONFIG_DYNAMIC_DEBUG
	if (debug < -1)
		hw->debug_mask = debug;
#endif

	err = ice_init_hw(hw);
	if (err) {
<<<<<<< HEAD
		dev_err(&pdev->dev, "ice_init_hw failed: %d\n", err);
=======
		dev_err(dev, "ice_init_hw failed: %d\n", err);
>>>>>>> upstream/android-13
		err = -EIO;
		goto err_exit_unroll;
	}

<<<<<<< HEAD
	dev_info(&pdev->dev, "firmware %d.%d.%05d api %d.%d\n",
		 hw->fw_maj_ver, hw->fw_min_ver, hw->fw_build,
		 hw->api_maj_ver, hw->api_min_ver);

	ice_init_pf(pf);

	ice_determine_q_usage(pf);

	pf->num_alloc_vsi = min_t(u16, ICE_MAX_VSI_ALLOC,
				  hw->func_caps.guaranteed_num_vsi);
=======
	ice_request_fw(pf);

	/* if ice_request_fw fails, ICE_FLAG_ADV_FEATURES bit won't be
	 * set in pf->state, which will cause ice_is_safe_mode to return
	 * true
	 */
	if (ice_is_safe_mode(pf)) {
		dev_err(dev, "Package download failed. Advanced features disabled - Device now in Safe Mode\n");
		/* we already got function/device capabilities but these don't
		 * reflect what the driver needs to do in safe mode. Instead of
		 * adding conditional logic everywhere to ignore these
		 * device/function capabilities, override them.
		 */
		ice_set_safe_mode_caps(hw);
	}

	err = ice_init_pf(pf);
	if (err) {
		dev_err(dev, "ice_init_pf failed: %d\n", err);
		goto err_init_pf_unroll;
	}

	ice_devlink_init_regions(pf);

	pf->hw.udp_tunnel_nic.set_port = ice_udp_tunnel_set_port;
	pf->hw.udp_tunnel_nic.unset_port = ice_udp_tunnel_unset_port;
	pf->hw.udp_tunnel_nic.flags = UDP_TUNNEL_NIC_INFO_MAY_SLEEP;
	pf->hw.udp_tunnel_nic.shared = &pf->hw.udp_tunnel_shared;
	i = 0;
	if (pf->hw.tnl.valid_count[TNL_VXLAN]) {
		pf->hw.udp_tunnel_nic.tables[i].n_entries =
			pf->hw.tnl.valid_count[TNL_VXLAN];
		pf->hw.udp_tunnel_nic.tables[i].tunnel_types =
			UDP_TUNNEL_TYPE_VXLAN;
		i++;
	}
	if (pf->hw.tnl.valid_count[TNL_GENEVE]) {
		pf->hw.udp_tunnel_nic.tables[i].n_entries =
			pf->hw.tnl.valid_count[TNL_GENEVE];
		pf->hw.udp_tunnel_nic.tables[i].tunnel_types =
			UDP_TUNNEL_TYPE_GENEVE;
		i++;
	}

	pf->num_alloc_vsi = hw->func_caps.guar_num_vsi;
>>>>>>> upstream/android-13
	if (!pf->num_alloc_vsi) {
		err = -EIO;
		goto err_init_pf_unroll;
	}
<<<<<<< HEAD

	pf->vsi = devm_kcalloc(&pdev->dev, pf->num_alloc_vsi,
			       sizeof(struct ice_vsi *), GFP_KERNEL);
=======
	if (pf->num_alloc_vsi > UDP_TUNNEL_NIC_MAX_SHARING_DEVICES) {
		dev_warn(&pf->pdev->dev,
			 "limiting the VSI count due to UDP tunnel limitation %d > %d\n",
			 pf->num_alloc_vsi, UDP_TUNNEL_NIC_MAX_SHARING_DEVICES);
		pf->num_alloc_vsi = UDP_TUNNEL_NIC_MAX_SHARING_DEVICES;
	}

	pf->vsi = devm_kcalloc(dev, pf->num_alloc_vsi, sizeof(*pf->vsi),
			       GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!pf->vsi) {
		err = -ENOMEM;
		goto err_init_pf_unroll;
	}

	err = ice_init_interrupt_scheme(pf);
	if (err) {
<<<<<<< HEAD
		dev_err(&pdev->dev,
			"ice_init_interrupt_scheme failed: %d\n", err);
		err = -EIO;
		goto err_init_interrupt_unroll;
=======
		dev_err(dev, "ice_init_interrupt_scheme failed: %d\n", err);
		err = -EIO;
		goto err_init_vsi_unroll;
>>>>>>> upstream/android-13
	}

	/* In case of MSIX we are going to setup the misc vector right here
	 * to handle admin queue events etc. In case of legacy and MSI
	 * the misc functionality and queue processing is combined in
	 * the same vector and that gets setup at open.
	 */
<<<<<<< HEAD
	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags)) {
		err = ice_req_irq_msix_misc(pf);
		if (err) {
			dev_err(&pdev->dev,
				"setup of misc vector failed: %d\n", err);
			goto err_init_interrupt_unroll;
		}
	}

	/* create switch struct for the switch element created by FW on boot */
	pf->first_sw = devm_kzalloc(&pdev->dev, sizeof(struct ice_sw),
				    GFP_KERNEL);
=======
	err = ice_req_irq_msix_misc(pf);
	if (err) {
		dev_err(dev, "setup of misc vector failed: %d\n", err);
		goto err_init_interrupt_unroll;
	}

	/* create switch struct for the switch element created by FW on boot */
	pf->first_sw = devm_kzalloc(dev, sizeof(*pf->first_sw), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!pf->first_sw) {
		err = -ENOMEM;
		goto err_msix_misc_unroll;
	}

<<<<<<< HEAD
	pf->first_sw->bridge_mode = BRIDGE_MODE_VEB;
=======
	if (hw->evb_veb)
		pf->first_sw->bridge_mode = BRIDGE_MODE_VEB;
	else
		pf->first_sw->bridge_mode = BRIDGE_MODE_VEPA;

>>>>>>> upstream/android-13
	pf->first_sw->pf = pf;

	/* record the sw_id available for later use */
	pf->first_sw->sw_id = hw->port_info->sw_id;

	err = ice_setup_pf_sw(pf);
	if (err) {
<<<<<<< HEAD
		dev_err(&pdev->dev,
			"probe failed due to setup pf switch:%d\n", err);
		goto err_alloc_sw_unroll;
	}

	/* Driver is mostly up */
	clear_bit(__ICE_DOWN, pf->state);
=======
		dev_err(dev, "probe failed due to setup PF switch: %d\n", err);
		goto err_alloc_sw_unroll;
	}

	clear_bit(ICE_SERVICE_DIS, pf->state);

	/* tell the firmware we are up */
	err = ice_send_version(pf);
	if (err) {
		dev_err(dev, "probe failed sending driver version %s. error: %d\n",
			UTS_RELEASE, err);
		goto err_send_version_unroll;
	}
>>>>>>> upstream/android-13

	/* since everything is good, start the service timer */
	mod_timer(&pf->serv_tmr, round_jiffies(jiffies + pf->serv_tmr_period));

	err = ice_init_link_events(pf->hw.port_info);
	if (err) {
<<<<<<< HEAD
		dev_err(&pdev->dev, "ice_init_link_events failed: %d\n", err);
		goto err_alloc_sw_unroll;
=======
		dev_err(dev, "ice_init_link_events failed: %d\n", err);
		goto err_send_version_unroll;
	}

	/* not a fatal error if this fails */
	err = ice_init_nvm_phy_type(pf->hw.port_info);
	if (err)
		dev_err(dev, "ice_init_nvm_phy_type failed: %d\n", err);

	/* not a fatal error if this fails */
	err = ice_update_link_info(pf->hw.port_info);
	if (err)
		dev_err(dev, "ice_update_link_info failed: %d\n", err);

	ice_init_link_dflt_override(pf->hw.port_info);

	ice_check_module_power(pf, pf->hw.port_info->phy.link_info.link_cfg_err);

	/* if media available, initialize PHY settings */
	if (pf->hw.port_info->phy.link_info.link_info &
	    ICE_AQ_MEDIA_AVAILABLE) {
		/* not a fatal error if this fails */
		err = ice_init_phy_user_cfg(pf->hw.port_info);
		if (err)
			dev_err(dev, "ice_init_phy_user_cfg failed: %d\n", err);

		if (!test_bit(ICE_FLAG_LINK_DOWN_ON_CLOSE_ENA, pf->flags)) {
			struct ice_vsi *vsi = ice_get_main_vsi(pf);

			if (vsi)
				ice_configure_phy(vsi);
		}
	} else {
		set_bit(ICE_FLAG_NO_MEDIA, pf->flags);
	}

	ice_verify_cacheline_size(pf);

	/* Save wakeup reason register for later use */
	pf->wakeup_reason = rd32(hw, PFPM_WUS);

	/* check for a power management event */
	ice_print_wake_reason(pf);

	/* clear wake status, all bits */
	wr32(hw, PFPM_WUS, U32_MAX);

	/* Disable WoL at init, wait for user to enable */
	device_set_wakeup_enable(dev, false);

	if (ice_is_safe_mode(pf)) {
		ice_set_safe_mode_vlan_cfg(pf);
		goto probe_done;
	}

	/* initialize DDP driven features */
	if (test_bit(ICE_FLAG_PTP_SUPPORTED, pf->flags))
		ice_ptp_init(pf);

	/* Note: Flow director init failure is non-fatal to load */
	if (ice_init_fdir(pf))
		dev_err(dev, "could not initialize flow director\n");

	/* Note: DCB init failure is non-fatal to load */
	if (ice_init_pf_dcb(pf, false)) {
		clear_bit(ICE_FLAG_DCB_CAPABLE, pf->flags);
		clear_bit(ICE_FLAG_DCB_ENA, pf->flags);
	} else {
		ice_cfg_lldp_mib_change(&pf->hw, true);
	}

	if (ice_init_lag(pf))
		dev_warn(dev, "Failed to init link aggregation support\n");

	/* print PCI link speed and width */
	pcie_print_link_status(pf->pdev);

probe_done:
	err = ice_register_netdev(pf);
	if (err)
		goto err_netdev_reg;

	/* ready to go, so clear down state bit */
	clear_bit(ICE_DOWN, pf->state);
	if (ice_is_aux_ena(pf)) {
		pf->aux_idx = ida_alloc(&ice_aux_ida, GFP_KERNEL);
		if (pf->aux_idx < 0) {
			dev_err(dev, "Failed to allocate device ID for AUX driver\n");
			err = -ENOMEM;
			goto err_netdev_reg;
		}

		err = ice_init_rdma(pf);
		if (err) {
			dev_err(dev, "Failed to initialize RDMA: %d\n", err);
			err = -EIO;
			goto err_init_aux_unroll;
		}
	} else {
		dev_warn(dev, "RDMA is not supported on this device\n");
>>>>>>> upstream/android-13
	}

	return 0;

<<<<<<< HEAD
err_alloc_sw_unroll:
	set_bit(__ICE_DOWN, pf->state);
	devm_kfree(&pf->pdev->dev, pf->first_sw);
=======
err_init_aux_unroll:
	pf->adev = NULL;
	ida_free(&ice_aux_ida, pf->aux_idx);
err_netdev_reg:
err_send_version_unroll:
	ice_vsi_release_all(pf);
err_alloc_sw_unroll:
	set_bit(ICE_SERVICE_DIS, pf->state);
	set_bit(ICE_DOWN, pf->state);
	devm_kfree(dev, pf->first_sw);
>>>>>>> upstream/android-13
err_msix_misc_unroll:
	ice_free_irq_msix_misc(pf);
err_init_interrupt_unroll:
	ice_clear_interrupt_scheme(pf);
<<<<<<< HEAD
	devm_kfree(&pdev->dev, pf->vsi);
err_init_pf_unroll:
	ice_deinit_pf(pf);
	ice_deinit_hw(hw);
err_exit_unroll:
	pci_disable_pcie_error_reporting(pdev);
=======
err_init_vsi_unroll:
	devm_kfree(dev, pf->vsi);
err_init_pf_unroll:
	ice_deinit_pf(pf);
	ice_devlink_destroy_regions(pf);
	ice_deinit_hw(hw);
err_exit_unroll:
	ice_devlink_unregister(pf);
	pci_disable_pcie_error_reporting(pdev);
	pci_disable_device(pdev);
>>>>>>> upstream/android-13
	return err;
}

/**
<<<<<<< HEAD
=======
 * ice_set_wake - enable or disable Wake on LAN
 * @pf: pointer to the PF struct
 *
 * Simple helper for WoL control
 */
static void ice_set_wake(struct ice_pf *pf)
{
	struct ice_hw *hw = &pf->hw;
	bool wol = pf->wol_ena;

	/* clear wake state, otherwise new wake events won't fire */
	wr32(hw, PFPM_WUS, U32_MAX);

	/* enable / disable APM wake up, no RMW needed */
	wr32(hw, PFPM_APM, wol ? PFPM_APM_APME_M : 0);

	/* set magic packet filter enabled */
	wr32(hw, PFPM_WUFC, wol ? PFPM_WUFC_MAG_M : 0);
}

/**
 * ice_setup_mc_magic_wake - setup device to wake on multicast magic packet
 * @pf: pointer to the PF struct
 *
 * Issue firmware command to enable multicast magic wake, making
 * sure that any locally administered address (LAA) is used for
 * wake, and that PF reset doesn't undo the LAA.
 */
static void ice_setup_mc_magic_wake(struct ice_pf *pf)
{
	struct device *dev = ice_pf_to_dev(pf);
	struct ice_hw *hw = &pf->hw;
	enum ice_status status;
	u8 mac_addr[ETH_ALEN];
	struct ice_vsi *vsi;
	u8 flags;

	if (!pf->wol_ena)
		return;

	vsi = ice_get_main_vsi(pf);
	if (!vsi)
		return;

	/* Get current MAC address in case it's an LAA */
	if (vsi->netdev)
		ether_addr_copy(mac_addr, vsi->netdev->dev_addr);
	else
		ether_addr_copy(mac_addr, vsi->port_info->mac.perm_addr);

	flags = ICE_AQC_MAN_MAC_WR_MC_MAG_EN |
		ICE_AQC_MAN_MAC_UPDATE_LAA_WOL |
		ICE_AQC_MAN_MAC_WR_WOL_LAA_PFR_KEEP;

	status = ice_aq_manage_mac_write(hw, mac_addr, flags, NULL);
	if (status)
		dev_err(dev, "Failed to enable Multicast Magic Packet wake, err %s aq_err %s\n",
			ice_stat_str(status),
			ice_aq_str(hw->adminq.sq_last_status));
}

/**
>>>>>>> upstream/android-13
 * ice_remove - Device removal routine
 * @pdev: PCI device information struct
 */
static void ice_remove(struct pci_dev *pdev)
{
	struct ice_pf *pf = pci_get_drvdata(pdev);
<<<<<<< HEAD
	int i = 0;
	int err;

	if (!pf)
		return;

	set_bit(__ICE_DOWN, pf->state);

	for (i = 0; i < pf->num_alloc_vsi; i++) {
		if (!pf->vsi[i])
			continue;

		err = ice_vsi_release(pf->vsi[i]);
		if (err)
			dev_dbg(&pf->pdev->dev, "Failed to release VSI index %d (err %d)\n",
				i, err);
	}

	ice_free_irq_msix_misc(pf);
	ice_clear_interrupt_scheme(pf);
	ice_deinit_pf(pf);
	ice_deinit_hw(&pf->hw);
	pci_disable_pcie_error_reporting(pdev);
=======
	int i;

	for (i = 0; i < ICE_MAX_RESET_WAIT; i++) {
		if (!ice_is_reset_in_progress(pf->state))
			break;
		msleep(100);
	}

	if (test_bit(ICE_FLAG_SRIOV_ENA, pf->flags)) {
		set_bit(ICE_VF_RESETS_DISABLED, pf->state);
		ice_free_vfs(pf);
	}

	ice_service_task_stop(pf);

	ice_aq_cancel_waiting_tasks(pf);
	ice_unplug_aux_dev(pf);
	if (pf->aux_idx >= 0)
		ida_free(&ice_aux_ida, pf->aux_idx);
	set_bit(ICE_DOWN, pf->state);

	mutex_destroy(&(&pf->hw)->fdir_fltr_lock);
	ice_deinit_lag(pf);
	if (test_bit(ICE_FLAG_PTP_SUPPORTED, pf->flags))
		ice_ptp_release(pf);
	if (!ice_is_safe_mode(pf))
		ice_remove_arfs(pf);
	ice_setup_mc_magic_wake(pf);
	ice_vsi_release_all(pf);
	ice_set_wake(pf);
	ice_free_irq_msix_misc(pf);
	ice_for_each_vsi(pf, i) {
		if (!pf->vsi[i])
			continue;
		ice_vsi_free_q_vectors(pf->vsi[i]);
	}
	ice_deinit_pf(pf);
	ice_devlink_destroy_regions(pf);
	ice_deinit_hw(&pf->hw);
	ice_devlink_unregister(pf);

	/* Issue a PFR as part of the prescribed driver unload flow.  Do not
	 * do it via ice_schedule_reset() since there is no need to rebuild
	 * and the service task is already stopped.
	 */
	ice_reset(&pf->hw, ICE_RESET_PFR);
	pci_wait_for_pending_transaction(pdev);
	ice_clear_interrupt_scheme(pf);
	pci_disable_pcie_error_reporting(pdev);
	pci_disable_device(pdev);
}

/**
 * ice_shutdown - PCI callback for shutting down device
 * @pdev: PCI device information struct
 */
static void ice_shutdown(struct pci_dev *pdev)
{
	struct ice_pf *pf = pci_get_drvdata(pdev);

	ice_remove(pdev);

	if (system_state == SYSTEM_POWER_OFF) {
		pci_wake_from_d3(pdev, pf->wol_ena);
		pci_set_power_state(pdev, PCI_D3hot);
	}
}

#ifdef CONFIG_PM
/**
 * ice_prepare_for_shutdown - prep for PCI shutdown
 * @pf: board private structure
 *
 * Inform or close all dependent features in prep for PCI device shutdown
 */
static void ice_prepare_for_shutdown(struct ice_pf *pf)
{
	struct ice_hw *hw = &pf->hw;
	u32 v;

	/* Notify VFs of impending reset */
	if (ice_check_sq_alive(hw, &hw->mailboxq))
		ice_vc_notify_reset(pf);

	dev_dbg(ice_pf_to_dev(pf), "Tearing down internal switch for shutdown\n");

	/* disable the VSIs and their queues that are not already DOWN */
	ice_pf_dis_all_vsi(pf, false);

	ice_for_each_vsi(pf, v)
		if (pf->vsi[v])
			pf->vsi[v]->vsi_num = 0;

	ice_shutdown_all_ctrlq(hw);
}

/**
 * ice_reinit_interrupt_scheme - Reinitialize interrupt scheme
 * @pf: board private structure to reinitialize
 *
 * This routine reinitialize interrupt scheme that was cleared during
 * power management suspend callback.
 *
 * This should be called during resume routine to re-allocate the q_vectors
 * and reacquire interrupts.
 */
static int ice_reinit_interrupt_scheme(struct ice_pf *pf)
{
	struct device *dev = ice_pf_to_dev(pf);
	int ret, v;

	/* Since we clear MSIX flag during suspend, we need to
	 * set it back during resume...
	 */

	ret = ice_init_interrupt_scheme(pf);
	if (ret) {
		dev_err(dev, "Failed to re-initialize interrupt %d\n", ret);
		return ret;
	}

	/* Remap vectors and rings, after successful re-init interrupts */
	ice_for_each_vsi(pf, v) {
		if (!pf->vsi[v])
			continue;

		ret = ice_vsi_alloc_q_vectors(pf->vsi[v]);
		if (ret)
			goto err_reinit;
		ice_vsi_map_rings_to_vectors(pf->vsi[v]);
	}

	ret = ice_req_irq_msix_misc(pf);
	if (ret) {
		dev_err(dev, "Setting up misc vector failed after device suspend %d\n",
			ret);
		goto err_reinit;
	}

	return 0;

err_reinit:
	while (v--)
		if (pf->vsi[v])
			ice_vsi_free_q_vectors(pf->vsi[v]);

	return ret;
}

/**
 * ice_suspend
 * @dev: generic device information structure
 *
 * Power Management callback to quiesce the device and prepare
 * for D3 transition.
 */
static int __maybe_unused ice_suspend(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct ice_pf *pf;
	int disabled, v;

	pf = pci_get_drvdata(pdev);

	if (!ice_pf_state_is_nominal(pf)) {
		dev_err(dev, "Device is not ready, no need to suspend it\n");
		return -EBUSY;
	}

	/* Stop watchdog tasks until resume completion.
	 * Even though it is most likely that the service task is
	 * disabled if the device is suspended or down, the service task's
	 * state is controlled by a different state bit, and we should
	 * store and honor whatever state that bit is in at this point.
	 */
	disabled = ice_service_task_stop(pf);

	ice_unplug_aux_dev(pf);

	/* Already suspended?, then there is nothing to do */
	if (test_and_set_bit(ICE_SUSPENDED, pf->state)) {
		if (!disabled)
			ice_service_task_restart(pf);
		return 0;
	}

	if (test_bit(ICE_DOWN, pf->state) ||
	    ice_is_reset_in_progress(pf->state)) {
		dev_err(dev, "can't suspend device in reset or already down\n");
		if (!disabled)
			ice_service_task_restart(pf);
		return 0;
	}

	ice_setup_mc_magic_wake(pf);

	ice_prepare_for_shutdown(pf);

	ice_set_wake(pf);

	/* Free vectors, clear the interrupt scheme and release IRQs
	 * for proper hibernation, especially with large number of CPUs.
	 * Otherwise hibernation might fail when mapping all the vectors back
	 * to CPU0.
	 */
	ice_free_irq_msix_misc(pf);
	ice_for_each_vsi(pf, v) {
		if (!pf->vsi[v])
			continue;
		ice_vsi_free_q_vectors(pf->vsi[v]);
	}
	ice_free_cpu_rx_rmap(ice_get_main_vsi(pf));
	ice_clear_interrupt_scheme(pf);

	pci_save_state(pdev);
	pci_wake_from_d3(pdev, pf->wol_ena);
	pci_set_power_state(pdev, PCI_D3hot);
	return 0;
}

/**
 * ice_resume - PM callback for waking up from D3
 * @dev: generic device information structure
 */
static int __maybe_unused ice_resume(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	enum ice_reset_req reset_type;
	struct ice_pf *pf;
	struct ice_hw *hw;
	int ret;

	pci_set_power_state(pdev, PCI_D0);
	pci_restore_state(pdev);
	pci_save_state(pdev);

	if (!pci_device_is_present(pdev))
		return -ENODEV;

	ret = pci_enable_device_mem(pdev);
	if (ret) {
		dev_err(dev, "Cannot enable device after suspend\n");
		return ret;
	}

	pf = pci_get_drvdata(pdev);
	hw = &pf->hw;

	pf->wakeup_reason = rd32(hw, PFPM_WUS);
	ice_print_wake_reason(pf);

	/* We cleared the interrupt scheme when we suspended, so we need to
	 * restore it now to resume device functionality.
	 */
	ret = ice_reinit_interrupt_scheme(pf);
	if (ret)
		dev_err(dev, "Cannot restore interrupt scheme: %d\n", ret);

	clear_bit(ICE_DOWN, pf->state);
	/* Now perform PF reset and rebuild */
	reset_type = ICE_RESET_PFR;
	/* re-enable service task for reset, but allow reset to schedule it */
	clear_bit(ICE_SERVICE_DIS, pf->state);

	if (ice_schedule_reset(pf, reset_type))
		dev_err(dev, "Reset during resume failed.\n");

	clear_bit(ICE_SUSPENDED, pf->state);
	ice_service_task_restart(pf);

	/* Restart the service task */
	mod_timer(&pf->serv_tmr, round_jiffies(jiffies + pf->serv_tmr_period));

	return 0;
}
#endif /* CONFIG_PM */

/**
 * ice_pci_err_detected - warning that PCI error has been detected
 * @pdev: PCI device information struct
 * @err: the type of PCI error
 *
 * Called to warn that something happened on the PCI bus and the error handling
 * is in progress.  Allows the driver to gracefully prepare/handle PCI errors.
 */
static pci_ers_result_t
ice_pci_err_detected(struct pci_dev *pdev, pci_channel_state_t err)
{
	struct ice_pf *pf = pci_get_drvdata(pdev);

	if (!pf) {
		dev_err(&pdev->dev, "%s: unrecoverable device error %d\n",
			__func__, err);
		return PCI_ERS_RESULT_DISCONNECT;
	}

	if (!test_bit(ICE_SUSPENDED, pf->state)) {
		ice_service_task_stop(pf);

		if (!test_bit(ICE_PREPARED_FOR_RESET, pf->state)) {
			set_bit(ICE_PFR_REQ, pf->state);
			ice_prepare_for_reset(pf);
		}
	}

	return PCI_ERS_RESULT_NEED_RESET;
}

/**
 * ice_pci_err_slot_reset - a PCI slot reset has just happened
 * @pdev: PCI device information struct
 *
 * Called to determine if the driver can recover from the PCI slot reset by
 * using a register read to determine if the device is recoverable.
 */
static pci_ers_result_t ice_pci_err_slot_reset(struct pci_dev *pdev)
{
	struct ice_pf *pf = pci_get_drvdata(pdev);
	pci_ers_result_t result;
	int err;
	u32 reg;

	err = pci_enable_device_mem(pdev);
	if (err) {
		dev_err(&pdev->dev, "Cannot re-enable PCI device after reset, error %d\n",
			err);
		result = PCI_ERS_RESULT_DISCONNECT;
	} else {
		pci_set_master(pdev);
		pci_restore_state(pdev);
		pci_save_state(pdev);
		pci_wake_from_d3(pdev, false);

		/* Check for life */
		reg = rd32(&pf->hw, GLGEN_RTRIG);
		if (!reg)
			result = PCI_ERS_RESULT_RECOVERED;
		else
			result = PCI_ERS_RESULT_DISCONNECT;
	}

	err = pci_aer_clear_nonfatal_status(pdev);
	if (err)
		dev_dbg(&pdev->dev, "pci_aer_clear_nonfatal_status() failed, error %d\n",
			err);
		/* non-fatal, continue */

	return result;
}

/**
 * ice_pci_err_resume - restart operations after PCI error recovery
 * @pdev: PCI device information struct
 *
 * Called to allow the driver to bring things back up after PCI error and/or
 * reset recovery have finished
 */
static void ice_pci_err_resume(struct pci_dev *pdev)
{
	struct ice_pf *pf = pci_get_drvdata(pdev);

	if (!pf) {
		dev_err(&pdev->dev, "%s failed, device is unrecoverable\n",
			__func__);
		return;
	}

	if (test_bit(ICE_SUSPENDED, pf->state)) {
		dev_dbg(&pdev->dev, "%s failed to resume normal operations!\n",
			__func__);
		return;
	}

	ice_restore_all_vfs_msi_state(pdev);

	ice_do_reset(pf, ICE_RESET_PFR);
	ice_service_task_restart(pf);
	mod_timer(&pf->serv_tmr, round_jiffies(jiffies + pf->serv_tmr_period));
}

/**
 * ice_pci_err_reset_prepare - prepare device driver for PCI reset
 * @pdev: PCI device information struct
 */
static void ice_pci_err_reset_prepare(struct pci_dev *pdev)
{
	struct ice_pf *pf = pci_get_drvdata(pdev);

	if (!test_bit(ICE_SUSPENDED, pf->state)) {
		ice_service_task_stop(pf);

		if (!test_bit(ICE_PREPARED_FOR_RESET, pf->state)) {
			set_bit(ICE_PFR_REQ, pf->state);
			ice_prepare_for_reset(pf);
		}
	}
}

/**
 * ice_pci_err_reset_done - PCI reset done, device driver reset can begin
 * @pdev: PCI device information struct
 */
static void ice_pci_err_reset_done(struct pci_dev *pdev)
{
	ice_pci_err_resume(pdev);
>>>>>>> upstream/android-13
}

/* ice_pci_tbl - PCI Device ID Table
 *
 * Wildcard entries (PCI_ANY_ID) should come last
 * Last entry must be all 0s
 *
 * { Vendor ID, Device ID, SubVendor ID, SubDevice ID,
 *   Class, Class Mask, private data (not used) }
 */
static const struct pci_device_id ice_pci_tbl[] = {
<<<<<<< HEAD
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_C810_BACKPLANE), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_C810_QSFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_C810_SFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_C810_10G_BASE_T), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_C810_SGMII), 0 },
=======
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E810C_BACKPLANE), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E810C_QSFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E810C_SFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E810_XXV_BACKPLANE), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E810_XXV_QSFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E810_XXV_SFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E823C_BACKPLANE), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E823C_QSFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E823C_SFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E823C_10G_BASE_T), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E823C_SGMII), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E822C_BACKPLANE), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E822C_QSFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E822C_SFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E822C_10G_BASE_T), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E822C_SGMII), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E822L_BACKPLANE), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E822L_SFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E822L_10G_BASE_T), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E822L_SGMII), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E823L_BACKPLANE), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E823L_SFP), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E823L_10G_BASE_T), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E823L_1GBE), 0 },
	{ PCI_VDEVICE(INTEL, ICE_DEV_ID_E823L_QSFP), 0 },
>>>>>>> upstream/android-13
	/* required last entry */
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, ice_pci_tbl);

<<<<<<< HEAD
=======
static __maybe_unused SIMPLE_DEV_PM_OPS(ice_pm_ops, ice_suspend, ice_resume);

static const struct pci_error_handlers ice_pci_err_handler = {
	.error_detected = ice_pci_err_detected,
	.slot_reset = ice_pci_err_slot_reset,
	.reset_prepare = ice_pci_err_reset_prepare,
	.reset_done = ice_pci_err_reset_done,
	.resume = ice_pci_err_resume
};

>>>>>>> upstream/android-13
static struct pci_driver ice_driver = {
	.name = KBUILD_MODNAME,
	.id_table = ice_pci_tbl,
	.probe = ice_probe,
	.remove = ice_remove,
<<<<<<< HEAD
=======
#ifdef CONFIG_PM
	.driver.pm = &ice_pm_ops,
#endif /* CONFIG_PM */
	.shutdown = ice_shutdown,
	.sriov_configure = ice_sriov_configure,
	.err_handler = &ice_pci_err_handler
>>>>>>> upstream/android-13
};

/**
 * ice_module_init - Driver registration routine
 *
 * ice_module_init is the first routine called when the driver is
 * loaded. All it does is register with the PCI subsystem.
 */
static int __init ice_module_init(void)
{
	int status;

<<<<<<< HEAD
	pr_info("%s - version %s\n", ice_driver_string, ice_drv_ver);
	pr_info("%s\n", ice_copyright);

	ice_wq = alloc_ordered_workqueue("%s", WQ_MEM_RECLAIM, KBUILD_MODNAME);
=======
	pr_info("%s\n", ice_driver_string);
	pr_info("%s\n", ice_copyright);

	ice_wq = alloc_workqueue("%s", WQ_MEM_RECLAIM, 0, KBUILD_MODNAME);
>>>>>>> upstream/android-13
	if (!ice_wq) {
		pr_err("Failed to create workqueue\n");
		return -ENOMEM;
	}

	status = pci_register_driver(&ice_driver);
	if (status) {
<<<<<<< HEAD
		pr_err("failed to register pci driver, err %d\n", status);
=======
		pr_err("failed to register PCI driver, err %d\n", status);
>>>>>>> upstream/android-13
		destroy_workqueue(ice_wq);
	}

	return status;
}
module_init(ice_module_init);

/**
 * ice_module_exit - Driver exit cleanup routine
 *
 * ice_module_exit is called just before the driver is removed
 * from memory.
 */
static void __exit ice_module_exit(void)
{
	pci_unregister_driver(&ice_driver);
	destroy_workqueue(ice_wq);
	pr_info("module unloaded\n");
}
module_exit(ice_module_exit);

/**
<<<<<<< HEAD
 * ice_set_mac_address - NDO callback to set mac address
=======
 * ice_set_mac_address - NDO callback to set MAC address
>>>>>>> upstream/android-13
 * @netdev: network interface device structure
 * @pi: pointer to an address structure
 *
 * Returns 0 on success, negative on failure
 */
static int ice_set_mac_address(struct net_device *netdev, void *pi)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	struct ice_pf *pf = vsi->back;
	struct ice_hw *hw = &pf->hw;
	struct sockaddr *addr = pi;
	enum ice_status status;
<<<<<<< HEAD
	LIST_HEAD(a_mac_list);
	LIST_HEAD(r_mac_list);
	u8 flags = 0;
	int err;
=======
	u8 old_mac[ETH_ALEN];
	u8 flags = 0;
	int err = 0;
>>>>>>> upstream/android-13
	u8 *mac;

	mac = (u8 *)addr->sa_data;

	if (!is_valid_ether_addr(mac))
		return -EADDRNOTAVAIL;

	if (ether_addr_equal(netdev->dev_addr, mac)) {
<<<<<<< HEAD
		netdev_warn(netdev, "already using mac %pM\n", mac);
		return 0;
	}

	if (test_bit(__ICE_DOWN, pf->state) ||
	    ice_is_reset_recovery_pending(pf->state)) {
=======
		netdev_dbg(netdev, "already using mac %pM\n", mac);
		return 0;
	}

	if (test_bit(ICE_DOWN, pf->state) ||
	    ice_is_reset_in_progress(pf->state)) {
>>>>>>> upstream/android-13
		netdev_err(netdev, "can't set mac %pM. device not ready\n",
			   mac);
		return -EBUSY;
	}

<<<<<<< HEAD
	/* When we change the mac address we also have to change the mac address
	 * based filter rules that were created previously for the old mac
	 * address. So first, we remove the old filter rule using ice_remove_mac
	 * and then create a new filter rule using ice_add_mac. Note that for
	 * both these operations, we first need to form a "list" of mac
	 * addresses (even though in this case, we have only 1 mac address to be
	 * added/removed) and this done using ice_add_mac_to_list. Depending on
	 * the ensuing operation this "list" of mac addresses is either to be
	 * added or removed from the filter.
	 */
	err = ice_add_mac_to_list(vsi, &r_mac_list, netdev->dev_addr);
	if (err) {
		err = -EADDRNOTAVAIL;
		goto free_lists;
	}

	status = ice_remove_mac(hw, &r_mac_list);
	if (status) {
		err = -EADDRNOTAVAIL;
		goto free_lists;
	}

	err = ice_add_mac_to_list(vsi, &a_mac_list, mac);
	if (err) {
		err = -EADDRNOTAVAIL;
		goto free_lists;
	}

	status = ice_add_mac(hw, &a_mac_list);
	if (status) {
		err = -EADDRNOTAVAIL;
		goto free_lists;
	}

free_lists:
	/* free list entries */
	ice_free_fltr_list(&pf->pdev->dev, &r_mac_list);
	ice_free_fltr_list(&pf->pdev->dev, &a_mac_list);

	if (err) {
		netdev_err(netdev, "can't set mac %pM. filter update failed\n",
			   mac);
		return err;
	}

	/* change the netdev's mac address */
	memcpy(netdev->dev_addr, mac, netdev->addr_len);
	netdev_dbg(vsi->netdev, "updated mac address to %pM\n",
		   netdev->dev_addr);

	/* write new mac address to the firmware */
	flags = ICE_AQC_MAN_MAC_UPDATE_LAA_WOL;
	status = ice_aq_manage_mac_write(hw, mac, flags, NULL);
	if (status) {
		netdev_err(netdev, "can't set mac %pM. write to firmware failed.\n",
			   mac);
=======
	netif_addr_lock_bh(netdev);
	ether_addr_copy(old_mac, netdev->dev_addr);
	/* change the netdev's MAC address */
	memcpy(netdev->dev_addr, mac, netdev->addr_len);
	netif_addr_unlock_bh(netdev);

	/* Clean up old MAC filter. Not an error if old filter doesn't exist */
	status = ice_fltr_remove_mac(vsi, old_mac, ICE_FWD_TO_VSI);
	if (status && status != ICE_ERR_DOES_NOT_EXIST) {
		err = -EADDRNOTAVAIL;
		goto err_update_filters;
	}

	/* Add filter for new MAC. If filter exists, return success */
	status = ice_fltr_add_mac(vsi, mac, ICE_FWD_TO_VSI);
	if (status == ICE_ERR_ALREADY_EXISTS)
		/* Although this MAC filter is already present in hardware it's
		 * possible in some cases (e.g. bonding) that dev_addr was
		 * modified outside of the driver and needs to be restored back
		 * to this value.
		 */
		netdev_dbg(netdev, "filter for MAC %pM already exists\n", mac);
	else if (status)
		/* error if the new filter addition failed */
		err = -EADDRNOTAVAIL;

err_update_filters:
	if (err) {
		netdev_err(netdev, "can't set MAC %pM. filter update failed\n",
			   mac);
		netif_addr_lock_bh(netdev);
		ether_addr_copy(netdev->dev_addr, old_mac);
		netif_addr_unlock_bh(netdev);
		return err;
	}

	netdev_dbg(vsi->netdev, "updated MAC address to %pM\n",
		   netdev->dev_addr);

	/* write new MAC address to the firmware */
	flags = ICE_AQC_MAN_MAC_UPDATE_LAA_WOL;
	status = ice_aq_manage_mac_write(hw, mac, flags, NULL);
	if (status) {
		netdev_err(netdev, "can't set MAC %pM. write to firmware failed error %s\n",
			   mac, ice_stat_str(status));
>>>>>>> upstream/android-13
	}
	return 0;
}

/**
 * ice_set_rx_mode - NDO callback to set the netdev filters
 * @netdev: network interface device structure
 */
static void ice_set_rx_mode(struct net_device *netdev)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;

	if (!vsi)
		return;

	/* Set the flags to synchronize filters
	 * ndo_set_rx_mode may be triggered even without a change in netdev
	 * flags
	 */
<<<<<<< HEAD
	set_bit(ICE_VSI_FLAG_UMAC_FLTR_CHANGED, vsi->flags);
	set_bit(ICE_VSI_FLAG_MMAC_FLTR_CHANGED, vsi->flags);
=======
	set_bit(ICE_VSI_UMAC_FLTR_CHANGED, vsi->state);
	set_bit(ICE_VSI_MMAC_FLTR_CHANGED, vsi->state);
>>>>>>> upstream/android-13
	set_bit(ICE_FLAG_FLTR_SYNC, vsi->back->flags);

	/* schedule our worker thread which will take care of
	 * applying the new filter changes
	 */
	ice_service_task_schedule(vsi->back);
}

/**
<<<<<<< HEAD
=======
 * ice_set_tx_maxrate - NDO callback to set the maximum per-queue bitrate
 * @netdev: network interface device structure
 * @queue_index: Queue ID
 * @maxrate: maximum bandwidth in Mbps
 */
static int
ice_set_tx_maxrate(struct net_device *netdev, int queue_index, u32 maxrate)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	enum ice_status status;
	u16 q_handle;
	u8 tc;

	/* Validate maxrate requested is within permitted range */
	if (maxrate && (maxrate > (ICE_SCHED_MAX_BW / 1000))) {
		netdev_err(netdev, "Invalid max rate %d specified for the queue %d\n",
			   maxrate, queue_index);
		return -EINVAL;
	}

	q_handle = vsi->tx_rings[queue_index]->q_handle;
	tc = ice_dcb_get_tc(vsi, queue_index);

	/* Set BW back to default, when user set maxrate to 0 */
	if (!maxrate)
		status = ice_cfg_q_bw_dflt_lmt(vsi->port_info, vsi->idx, tc,
					       q_handle, ICE_MAX_BW);
	else
		status = ice_cfg_q_bw_lmt(vsi->port_info, vsi->idx, tc,
					  q_handle, ICE_MAX_BW, maxrate * 1000);
	if (status) {
		netdev_err(netdev, "Unable to set Tx max rate, error %s\n",
			   ice_stat_str(status));
		return -EIO;
	}

	return 0;
}

/**
>>>>>>> upstream/android-13
 * ice_fdb_add - add an entry to the hardware database
 * @ndm: the input from the stack
 * @tb: pointer to array of nladdr (unused)
 * @dev: the net device pointer
 * @addr: the MAC address entry being added
<<<<<<< HEAD
 * @vid: VLAN id
 * @flags: instructions from stack about fdb operation
 */
static int ice_fdb_add(struct ndmsg *ndm, struct nlattr __always_unused *tb[],
		       struct net_device *dev, const unsigned char *addr,
		       u16 vid, u16 flags)
=======
 * @vid: VLAN ID
 * @flags: instructions from stack about fdb operation
 * @extack: netlink extended ack
 */
static int
ice_fdb_add(struct ndmsg *ndm, struct nlattr __always_unused *tb[],
	    struct net_device *dev, const unsigned char *addr, u16 vid,
	    u16 flags, struct netlink_ext_ack __always_unused *extack)
>>>>>>> upstream/android-13
{
	int err;

	if (vid) {
		netdev_err(dev, "VLANs aren't supported yet for dev_uc|mc_add()\n");
		return -EINVAL;
	}
	if (ndm->ndm_state && !(ndm->ndm_state & NUD_PERMANENT)) {
		netdev_err(dev, "FDB only supports static addresses\n");
		return -EINVAL;
	}

	if (is_unicast_ether_addr(addr) || is_link_local_ether_addr(addr))
		err = dev_uc_add_excl(dev, addr);
	else if (is_multicast_ether_addr(addr))
		err = dev_mc_add_excl(dev, addr);
	else
		err = -EINVAL;

	/* Only return duplicate errors if NLM_F_EXCL is set */
	if (err == -EEXIST && !(flags & NLM_F_EXCL))
		err = 0;

	return err;
}

/**
 * ice_fdb_del - delete an entry from the hardware database
 * @ndm: the input from the stack
 * @tb: pointer to array of nladdr (unused)
 * @dev: the net device pointer
 * @addr: the MAC address entry being added
<<<<<<< HEAD
 * @vid: VLAN id
 */
static int ice_fdb_del(struct ndmsg *ndm, __always_unused struct nlattr *tb[],
		       struct net_device *dev, const unsigned char *addr,
		       __always_unused u16 vid)
=======
 * @vid: VLAN ID
 */
static int
ice_fdb_del(struct ndmsg *ndm, __always_unused struct nlattr *tb[],
	    struct net_device *dev, const unsigned char *addr,
	    __always_unused u16 vid)
>>>>>>> upstream/android-13
{
	int err;

	if (ndm->ndm_state & NUD_PERMANENT) {
		netdev_err(dev, "FDB only supports static addresses\n");
		return -EINVAL;
	}

	if (is_unicast_ether_addr(addr))
		err = dev_uc_del(dev, addr);
	else if (is_multicast_ether_addr(addr))
		err = dev_mc_del(dev, addr);
	else
		err = -EINVAL;

	return err;
}

/**
<<<<<<< HEAD
 * ice_vsi_manage_vlan_insertion - Manage VLAN insertion for the VSI for Tx
 * @vsi: the vsi being changed
 */
static int ice_vsi_manage_vlan_insertion(struct ice_vsi *vsi)
{
	struct device *dev = &vsi->back->pdev->dev;
	struct ice_hw *hw = &vsi->back->hw;
	struct ice_vsi_ctx ctxt = { 0 };
	enum ice_status status;

	/* Here we are configuring the VSI to let the driver add VLAN tags by
	 * setting vlan_flags to ICE_AQ_VSI_VLAN_MODE_ALL. The actual VLAN tag
	 * insertion happens in the Tx hot path, in ice_tx_map.
	 */
	ctxt.info.vlan_flags = ICE_AQ_VSI_VLAN_MODE_ALL;

	ctxt.info.valid_sections = cpu_to_le16(ICE_AQ_VSI_PROP_VLAN_VALID);
	ctxt.vsi_num = vsi->vsi_num;

	status = ice_aq_update_vsi(hw, &ctxt, NULL);
	if (status) {
		dev_err(dev, "update VSI for VLAN insert failed, err %d aq_err %d\n",
			status, hw->adminq.sq_last_status);
		return -EIO;
	}

	vsi->info.vlan_flags = ctxt.info.vlan_flags;
	return 0;
}

/**
 * ice_vsi_manage_vlan_stripping - Manage VLAN stripping for the VSI for Rx
 * @vsi: the vsi being changed
 * @ena: boolean value indicating if this is a enable or disable request
 */
static int ice_vsi_manage_vlan_stripping(struct ice_vsi *vsi, bool ena)
{
	struct device *dev = &vsi->back->pdev->dev;
	struct ice_hw *hw = &vsi->back->hw;
	struct ice_vsi_ctx ctxt = { 0 };
	enum ice_status status;

	/* Here we are configuring what the VSI should do with the VLAN tag in
	 * the Rx packet. We can either leave the tag in the packet or put it in
	 * the Rx descriptor.
	 */
	if (ena) {
		/* Strip VLAN tag from Rx packet and put it in the desc */
		ctxt.info.vlan_flags = ICE_AQ_VSI_VLAN_EMOD_STR_BOTH;
	} else {
		/* Disable stripping. Leave tag in packet */
		ctxt.info.vlan_flags = ICE_AQ_VSI_VLAN_EMOD_NOTHING;
	}

	/* Allow all packets untagged/tagged */
	ctxt.info.vlan_flags |= ICE_AQ_VSI_VLAN_MODE_ALL;

	ctxt.info.valid_sections = cpu_to_le16(ICE_AQ_VSI_PROP_VLAN_VALID);
	ctxt.vsi_num = vsi->vsi_num;

	status = ice_aq_update_vsi(hw, &ctxt, NULL);
	if (status) {
		dev_err(dev, "update VSI for VALN strip failed, ena = %d err %d aq_err %d\n",
			ena, status, hw->adminq.sq_last_status);
		return -EIO;
	}

	vsi->info.vlan_flags = ctxt.info.vlan_flags;
	return 0;
}

/**
=======
>>>>>>> upstream/android-13
 * ice_set_features - set the netdev feature flags
 * @netdev: ptr to the netdev being adjusted
 * @features: the feature set that the stack is suggesting
 */
<<<<<<< HEAD
static int ice_set_features(struct net_device *netdev,
			    netdev_features_t features)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	int ret = 0;

=======
static int
ice_set_features(struct net_device *netdev, netdev_features_t features)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	struct ice_pf *pf = vsi->back;
	int ret = 0;

	/* Don't set any netdev advanced features with device in Safe Mode */
	if (ice_is_safe_mode(vsi->back)) {
		dev_err(ice_pf_to_dev(vsi->back), "Device is in Safe Mode - not enabling advanced netdev features\n");
		return ret;
	}

	/* Do not change setting during reset */
	if (ice_is_reset_in_progress(pf->state)) {
		dev_err(ice_pf_to_dev(vsi->back), "Device is resetting, changing advanced netdev features temporarily unavailable.\n");
		return -EBUSY;
	}

	/* Multiple features can be changed in one call so keep features in
	 * separate if/else statements to guarantee each feature is checked
	 */
	if (features & NETIF_F_RXHASH && !(netdev->features & NETIF_F_RXHASH))
		ice_vsi_manage_rss_lut(vsi, true);
	else if (!(features & NETIF_F_RXHASH) &&
		 netdev->features & NETIF_F_RXHASH)
		ice_vsi_manage_rss_lut(vsi, false);

>>>>>>> upstream/android-13
	if ((features & NETIF_F_HW_VLAN_CTAG_RX) &&
	    !(netdev->features & NETIF_F_HW_VLAN_CTAG_RX))
		ret = ice_vsi_manage_vlan_stripping(vsi, true);
	else if (!(features & NETIF_F_HW_VLAN_CTAG_RX) &&
		 (netdev->features & NETIF_F_HW_VLAN_CTAG_RX))
		ret = ice_vsi_manage_vlan_stripping(vsi, false);
<<<<<<< HEAD
	else if ((features & NETIF_F_HW_VLAN_CTAG_TX) &&
		 !(netdev->features & NETIF_F_HW_VLAN_CTAG_TX))
=======

	if ((features & NETIF_F_HW_VLAN_CTAG_TX) &&
	    !(netdev->features & NETIF_F_HW_VLAN_CTAG_TX))
>>>>>>> upstream/android-13
		ret = ice_vsi_manage_vlan_insertion(vsi);
	else if (!(features & NETIF_F_HW_VLAN_CTAG_TX) &&
		 (netdev->features & NETIF_F_HW_VLAN_CTAG_TX))
		ret = ice_vsi_manage_vlan_insertion(vsi);

<<<<<<< HEAD
=======
	if ((features & NETIF_F_HW_VLAN_CTAG_FILTER) &&
	    !(netdev->features & NETIF_F_HW_VLAN_CTAG_FILTER))
		ret = ice_cfg_vlan_pruning(vsi, true, false);
	else if (!(features & NETIF_F_HW_VLAN_CTAG_FILTER) &&
		 (netdev->features & NETIF_F_HW_VLAN_CTAG_FILTER))
		ret = ice_cfg_vlan_pruning(vsi, false, false);

	if ((features & NETIF_F_NTUPLE) &&
	    !(netdev->features & NETIF_F_NTUPLE)) {
		ice_vsi_manage_fdir(vsi, true);
		ice_init_arfs(vsi);
	} else if (!(features & NETIF_F_NTUPLE) &&
		 (netdev->features & NETIF_F_NTUPLE)) {
		ice_vsi_manage_fdir(vsi, false);
		ice_clear_arfs(vsi);
	}

>>>>>>> upstream/android-13
	return ret;
}

/**
<<<<<<< HEAD
 * ice_vsi_vlan_setup - Setup vlan offload properties on a VSI
 * @vsi: VSI to setup vlan properties for
=======
 * ice_vsi_vlan_setup - Setup VLAN offload properties on a VSI
 * @vsi: VSI to setup VLAN properties for
>>>>>>> upstream/android-13
 */
static int ice_vsi_vlan_setup(struct ice_vsi *vsi)
{
	int ret = 0;

	if (vsi->netdev->features & NETIF_F_HW_VLAN_CTAG_RX)
		ret = ice_vsi_manage_vlan_stripping(vsi, true);
	if (vsi->netdev->features & NETIF_F_HW_VLAN_CTAG_TX)
		ret = ice_vsi_manage_vlan_insertion(vsi);

	return ret;
}

/**
<<<<<<< HEAD
 * ice_restore_vlan - Reinstate VLANs when vsi/netdev comes back up
 * @vsi: the VSI being brought back up
 */
static int ice_restore_vlan(struct ice_vsi *vsi)
{
	int err;
	u16 vid;

	if (!vsi->netdev)
		return -EINVAL;

	err = ice_vsi_vlan_setup(vsi);
	if (err)
		return err;

	for_each_set_bit(vid, vsi->active_vlans, VLAN_N_VID) {
		err = ice_vlan_rx_add_vid(vsi->netdev, htons(ETH_P_8021Q), vid);
		if (err)
			break;
	}

	return err;
}

/**
 * ice_setup_tx_ctx - setup a struct ice_tlan_ctx instance
 * @ring: The Tx ring to configure
 * @tlan_ctx: Pointer to the Tx LAN queue context structure to be initialized
 * @pf_q: queue index in the PF space
 *
 * Configure the Tx descriptor ring in TLAN context.
 */
static void
ice_setup_tx_ctx(struct ice_ring *ring, struct ice_tlan_ctx *tlan_ctx, u16 pf_q)
{
	struct ice_vsi *vsi = ring->vsi;
	struct ice_hw *hw = &vsi->back->hw;

	tlan_ctx->base = ring->dma >> ICE_TLAN_CTX_BASE_S;

	tlan_ctx->port_num = vsi->port_info->lport;

	/* Transmit Queue Length */
	tlan_ctx->qlen = ring->count;

	/* PF number */
	tlan_ctx->pf_num = hw->pf_id;

	/* queue belongs to a specific VSI type
	 * VF / VM index should be programmed per vmvf_type setting:
	 * for vmvf_type = VF, it is VF number between 0-256
	 * for vmvf_type = VM, it is VM number between 0-767
	 * for PF or EMP this field should be set to zero
	 */
	switch (vsi->type) {
	case ICE_VSI_PF:
		tlan_ctx->vmvf_type = ICE_TLAN_CTX_VMVF_TYPE_PF;
		break;
	default:
		return;
	}

	/* make sure the context is associated with the right VSI */
	tlan_ctx->src_vsi = vsi->vsi_num;

	tlan_ctx->tso_ena = ICE_TX_LEGACY;
	tlan_ctx->tso_qnum = pf_q;

	/* Legacy or Advanced Host Interface:
	 * 0: Advanced Host Interface
	 * 1: Legacy Host Interface
	 */
	tlan_ctx->legacy_int = ICE_TX_LEGACY;
}

/**
 * ice_vsi_cfg_txqs - Configure the VSI for Tx
 * @vsi: the VSI being configured
 *
 * Return 0 on success and a negative value on error
 * Configure the Tx VSI for operation.
 */
static int ice_vsi_cfg_txqs(struct ice_vsi *vsi)
{
	struct ice_aqc_add_tx_qgrp *qg_buf;
	struct ice_aqc_add_txqs_perq *txq;
	struct ice_pf *pf = vsi->back;
	enum ice_status status;
	u16 buf_len, i, pf_q;
	int err = 0, tc = 0;
	u8 num_q_grps;

	buf_len = sizeof(struct ice_aqc_add_tx_qgrp);
	qg_buf = devm_kzalloc(&pf->pdev->dev, buf_len, GFP_KERNEL);
	if (!qg_buf)
		return -ENOMEM;

	if (vsi->num_txq > ICE_MAX_TXQ_PER_TXQG) {
		err = -EINVAL;
		goto err_cfg_txqs;
	}
	qg_buf->num_txqs = 1;
	num_q_grps = 1;

	/* set up and configure the tx queues */
	ice_for_each_txq(vsi, i) {
		struct ice_tlan_ctx tlan_ctx = { 0 };

		pf_q = vsi->txq_map[i];
		ice_setup_tx_ctx(vsi->tx_rings[i], &tlan_ctx, pf_q);
		/* copy context contents into the qg_buf */
		qg_buf->txqs[0].txq_id = cpu_to_le16(pf_q);
		ice_set_ctx((u8 *)&tlan_ctx, qg_buf->txqs[0].txq_ctx,
			    ice_tlan_ctx_info);

		/* init queue specific tail reg. It is referred as transmit
		 * comm scheduler queue doorbell.
		 */
		vsi->tx_rings[i]->tail = pf->hw.hw_addr + QTX_COMM_DBELL(pf_q);
		status = ice_ena_vsi_txq(vsi->port_info, vsi->vsi_num, tc,
					 num_q_grps, qg_buf, buf_len, NULL);
		if (status) {
			dev_err(&vsi->back->pdev->dev,
				"Failed to set LAN Tx queue context, error: %d\n",
				status);
			err = -ENODEV;
			goto err_cfg_txqs;
		}

		/* Add Tx Queue TEID into the VSI tx ring from the response
		 * This will complete configuring and enabling the queue.
		 */
		txq = &qg_buf->txqs[0];
		if (pf_q == le16_to_cpu(txq->txq_id))
			vsi->tx_rings[i]->txq_teid =
				le32_to_cpu(txq->q_teid);
	}
err_cfg_txqs:
	devm_kfree(&pf->pdev->dev, qg_buf);
	return err;
}

/**
 * ice_setup_rx_ctx - Configure a receive ring context
 * @ring: The Rx ring to configure
 *
 * Configure the Rx descriptor ring in RLAN context.
 */
static int ice_setup_rx_ctx(struct ice_ring *ring)
{
	struct ice_vsi *vsi = ring->vsi;
	struct ice_hw *hw = &vsi->back->hw;
	u32 rxdid = ICE_RXDID_FLEX_NIC;
	struct ice_rlan_ctx rlan_ctx;
	u32 regval;
	u16 pf_q;
	int err;

	/* what is RX queue number in global space of 2K rx queues */
	pf_q = vsi->rxq_map[ring->q_index];

	/* clear the context structure first */
	memset(&rlan_ctx, 0, sizeof(rlan_ctx));

	rlan_ctx.base = ring->dma >> ICE_RLAN_BASE_S;

	rlan_ctx.qlen = ring->count;

	/* Receive Packet Data Buffer Size.
	 * The Packet Data Buffer Size is defined in 128 byte units.
	 */
	rlan_ctx.dbuf = vsi->rx_buf_len >> ICE_RLAN_CTX_DBUF_S;

	/* use 32 byte descriptors */
	rlan_ctx.dsize = 1;

	/* Strip the Ethernet CRC bytes before the packet is posted to host
	 * memory.
	 */
	rlan_ctx.crcstrip = 1;

	/* L2TSEL flag defines the reported L2 Tags in the receive descriptor */
	rlan_ctx.l2tsel = 1;

	rlan_ctx.dtype = ICE_RX_DTYPE_NO_SPLIT;
	rlan_ctx.hsplit_0 = ICE_RLAN_RX_HSPLIT_0_NO_SPLIT;
	rlan_ctx.hsplit_1 = ICE_RLAN_RX_HSPLIT_1_NO_SPLIT;

	/* This controls whether VLAN is stripped from inner headers
	 * The VLAN in the inner L2 header is stripped to the receive
	 * descriptor if enabled by this flag.
	 */
	rlan_ctx.showiv = 0;

	/* Max packet size for this queue - must not be set to a larger value
	 * than 5 x DBUF
	 */
	rlan_ctx.rxmax = min_t(u16, vsi->max_frame,
			       ICE_MAX_CHAINED_RX_BUFS * vsi->rx_buf_len);

	/* Rx queue threshold in units of 64 */
	rlan_ctx.lrxqthresh = 1;

	 /* Enable Flexible Descriptors in the queue context which
	  * allows this driver to select a specific receive descriptor format
	  */
	regval = rd32(hw, QRXFLXP_CNTXT(pf_q));
	regval |= (rxdid << QRXFLXP_CNTXT_RXDID_IDX_S) &
		QRXFLXP_CNTXT_RXDID_IDX_M;

	/* increasing context priority to pick up profile id;
	 * default is 0x01; setting to 0x03 to ensure profile
	 * is programming if prev context is of same priority
	 */
	regval |= (0x03 << QRXFLXP_CNTXT_RXDID_PRIO_S) &
		QRXFLXP_CNTXT_RXDID_PRIO_M;

	wr32(hw, QRXFLXP_CNTXT(pf_q), regval);

	/* Absolute queue number out of 2K needs to be passed */
	err = ice_write_rxq_ctx(hw, &rlan_ctx, pf_q);
	if (err) {
		dev_err(&vsi->back->pdev->dev,
			"Failed to set LAN Rx queue context for absolute Rx queue %d error: %d\n",
			pf_q, err);
		return -EIO;
	}

	/* init queue specific tail register */
	ring->tail = hw->hw_addr + QRX_TAIL(pf_q);
	writel(0, ring->tail);
	ice_alloc_rx_bufs(ring, ICE_DESC_UNUSED(ring));

	return 0;
}

/**
 * ice_vsi_cfg_rxqs - Configure the VSI for Rx
 * @vsi: the VSI being configured
 *
 * Return 0 on success and a negative value on error
 * Configure the Rx VSI for operation.
 */
static int ice_vsi_cfg_rxqs(struct ice_vsi *vsi)
{
	int err = 0;
	u16 i;

	if (vsi->netdev && vsi->netdev->mtu > ETH_DATA_LEN)
		vsi->max_frame = vsi->netdev->mtu +
			ETH_HLEN + ETH_FCS_LEN + VLAN_HLEN;
	else
		vsi->max_frame = ICE_RXBUF_2048;

	vsi->rx_buf_len = ICE_RXBUF_2048;
	/* set up individual rings */
	for (i = 0; i < vsi->num_rxq && !err; i++)
		err = ice_setup_rx_ctx(vsi->rx_rings[i]);

	if (err) {
		dev_err(&vsi->back->pdev->dev, "ice_setup_rx_ctx failed\n");
		return -EIO;
	}
	return err;
}

/**
=======
>>>>>>> upstream/android-13
 * ice_vsi_cfg - Setup the VSI
 * @vsi: the VSI being configured
 *
 * Return 0 on success and negative value on error
 */
<<<<<<< HEAD
static int ice_vsi_cfg(struct ice_vsi *vsi)
=======
int ice_vsi_cfg(struct ice_vsi *vsi)
>>>>>>> upstream/android-13
{
	int err;

	if (vsi->netdev) {
		ice_set_rx_mode(vsi->netdev);
<<<<<<< HEAD
		err = ice_restore_vlan(vsi);
		if (err)
			return err;
	}

	err = ice_vsi_cfg_txqs(vsi);
=======

		err = ice_vsi_vlan_setup(vsi);

		if (err)
			return err;
	}
	ice_vsi_cfg_dcb_rings(vsi);

	err = ice_vsi_cfg_lan_txqs(vsi);
	if (!err && ice_is_xdp_ena_vsi(vsi))
		err = ice_vsi_cfg_xdp_txqs(vsi);
>>>>>>> upstream/android-13
	if (!err)
		err = ice_vsi_cfg_rxqs(vsi);

	return err;
}

<<<<<<< HEAD
/**
 * ice_vsi_stop_tx_rings - Disable Tx rings
 * @vsi: the VSI being configured
 */
static int ice_vsi_stop_tx_rings(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	struct ice_hw *hw = &pf->hw;
	enum ice_status status;
	u32 *q_teids, val;
	u16 *q_ids, i;
	int err = 0;

	if (vsi->num_txq > ICE_LAN_TXQ_MAX_QDIS)
		return -EINVAL;

	q_teids = devm_kcalloc(&pf->pdev->dev, vsi->num_txq, sizeof(*q_teids),
			       GFP_KERNEL);
	if (!q_teids)
		return -ENOMEM;

	q_ids = devm_kcalloc(&pf->pdev->dev, vsi->num_txq, sizeof(*q_ids),
			     GFP_KERNEL);
	if (!q_ids) {
		err = -ENOMEM;
		goto err_alloc_q_ids;
	}

	/* set up the tx queue list to be disabled */
	ice_for_each_txq(vsi, i) {
		u16 v_idx;

		if (!vsi->tx_rings || !vsi->tx_rings[i]) {
			err = -EINVAL;
			goto err_out;
		}

		q_ids[i] = vsi->txq_map[i];
		q_teids[i] = vsi->tx_rings[i]->txq_teid;

		/* clear cause_ena bit for disabled queues */
		val = rd32(hw, QINT_TQCTL(vsi->tx_rings[i]->reg_idx));
		val &= ~QINT_TQCTL_CAUSE_ENA_M;
		wr32(hw, QINT_TQCTL(vsi->tx_rings[i]->reg_idx), val);

		/* software is expected to wait for 100 ns */
		ndelay(100);

		/* trigger a software interrupt for the vector associated to
		 * the queue to schedule napi handler
		 */
		v_idx = vsi->tx_rings[i]->q_vector->v_idx;
		wr32(hw, GLINT_DYN_CTL(vsi->base_vector + v_idx),
		     GLINT_DYN_CTL_SWINT_TRIG_M | GLINT_DYN_CTL_INTENA_MSK_M);
	}
	status = ice_dis_vsi_txq(vsi->port_info, vsi->num_txq, q_ids, q_teids,
				 NULL);
	/* if the disable queue command was exercised during an active reset
	 * flow, ICE_ERR_RESET_ONGOING is returned. This is not an error as
	 * the reset operation disables queues at the hardware level anyway.
	 */
	if (status == ICE_ERR_RESET_ONGOING) {
		dev_dbg(&pf->pdev->dev,
			"Reset in progress. LAN Tx queues already disabled\n");
	} else if (status) {
		dev_err(&pf->pdev->dev,
			"Failed to disable LAN Tx queues, error: %d\n",
			status);
		err = -ENODEV;
	}

err_out:
	devm_kfree(&pf->pdev->dev, q_ids);

err_alloc_q_ids:
	devm_kfree(&pf->pdev->dev, q_teids);

	return err;
}

/**
 * ice_pf_rxq_wait - Wait for a PF's Rx queue to be enabled or disabled
 * @pf: the PF being configured
 * @pf_q: the PF queue
 * @ena: enable or disable state of the queue
 *
 * This routine will wait for the given Rx queue of the PF to reach the
 * enabled or disabled state.
 * Returns -ETIMEDOUT in case of failing to reach the requested state after
 * multiple retries; else will return 0 in case of success.
 */
static int ice_pf_rxq_wait(struct ice_pf *pf, int pf_q, bool ena)
{
	int i;

	for (i = 0; i < ICE_Q_WAIT_RETRY_LIMIT; i++) {
		u32 rx_reg = rd32(&pf->hw, QRX_CTRL(pf_q));

		if (ena == !!(rx_reg & QRX_CTRL_QENA_STAT_M))
			break;

		usleep_range(10, 20);
	}
	if (i >= ICE_Q_WAIT_RETRY_LIMIT)
		return -ETIMEDOUT;

	return 0;
}

/**
 * ice_vsi_ctrl_rx_rings - Start or stop a VSI's rx rings
 * @vsi: the VSI being configured
 * @ena: start or stop the rx rings
 */
static int ice_vsi_ctrl_rx_rings(struct ice_vsi *vsi, bool ena)
{
	struct ice_pf *pf = vsi->back;
	struct ice_hw *hw = &pf->hw;
	int i, j, ret = 0;

	for (i = 0; i < vsi->num_rxq; i++) {
		int pf_q = vsi->rxq_map[i];
		u32 rx_reg;

		for (j = 0; j < ICE_Q_WAIT_MAX_RETRY; j++) {
			rx_reg = rd32(hw, QRX_CTRL(pf_q));
			if (((rx_reg >> QRX_CTRL_QENA_REQ_S) & 1) ==
			    ((rx_reg >> QRX_CTRL_QENA_STAT_S) & 1))
				break;
			usleep_range(1000, 2000);
		}

		/* Skip if the queue is already in the requested state */
		if (ena == !!(rx_reg & QRX_CTRL_QENA_STAT_M))
			continue;

		/* turn on/off the queue */
		if (ena)
			rx_reg |= QRX_CTRL_QENA_REQ_M;
		else
			rx_reg &= ~QRX_CTRL_QENA_REQ_M;
		wr32(hw, QRX_CTRL(pf_q), rx_reg);

		/* wait for the change to finish */
		ret = ice_pf_rxq_wait(pf, pf_q, ena);
		if (ret) {
			dev_err(&pf->pdev->dev,
				"VSI idx %d Rx ring %d %sable timeout\n",
				vsi->idx, pf_q, (ena ? "en" : "dis"));
			break;
		}
	}

	return ret;
}

/**
 * ice_vsi_start_rx_rings - start VSI's rx rings
 * @vsi: the VSI whose rings are to be started
 *
 * Returns 0 on success and a negative value on error
 */
static int ice_vsi_start_rx_rings(struct ice_vsi *vsi)
{
	return ice_vsi_ctrl_rx_rings(vsi, true);
}

/**
 * ice_vsi_stop_rx_rings - stop VSI's rx rings
 * @vsi: the VSI
 *
 * Returns 0 on success and a negative value on error
 */
static int ice_vsi_stop_rx_rings(struct ice_vsi *vsi)
{
	return ice_vsi_ctrl_rx_rings(vsi, false);
}

/**
 * ice_vsi_stop_tx_rx_rings - stop VSI's tx and rx rings
 * @vsi: the VSI
 * Returns 0 on success and a negative value on error
 */
static int ice_vsi_stop_tx_rx_rings(struct ice_vsi *vsi)
{
	int err_tx, err_rx;

	err_tx = ice_vsi_stop_tx_rings(vsi);
	if (err_tx)
		dev_dbg(&vsi->back->pdev->dev, "Failed to disable Tx rings\n");

	err_rx = ice_vsi_stop_rx_rings(vsi);
	if (err_rx)
		dev_dbg(&vsi->back->pdev->dev, "Failed to disable Rx rings\n");

	if (err_tx || err_rx)
		return -EIO;

	return 0;
=======
/* THEORY OF MODERATION:
 * The below code creates custom DIM profiles for use by this driver, because
 * the ice driver hardware works differently than the hardware that DIMLIB was
 * originally made for. ice hardware doesn't have packet count limits that
 * can trigger an interrupt, but it *does* have interrupt rate limit support,
 * and this code adds that capability to be used by the driver when it's using
 * DIMLIB. The DIMLIB code was always designed to be a suggestion to the driver
 * for how to "respond" to traffic and interrupts, so this driver uses a
 * slightly different set of moderation parameters to get best performance.
 */
struct ice_dim {
	/* the throttle rate for interrupts, basically worst case delay before
	 * an initial interrupt fires, value is stored in microseconds.
	 */
	u16 itr;
	/* the rate limit for interrupts, which can cap a delay from a small
	 * ITR at a certain amount of interrupts per second. f.e. a 2us ITR
	 * could yield as much as 500,000 interrupts per second, but with a
	 * 10us rate limit, it limits to 100,000 interrupts per second. Value
	 * is stored in microseconds.
	 */
	u16 intrl;
};

/* Make a different profile for Rx that doesn't allow quite so aggressive
 * moderation at the high end (it maxes out at 128us or about 8k interrupts a
 * second. The INTRL/rate parameters here are only useful to cap small ITR
 * values, which is why for larger ITR's - like 128, which can only generate
 * 8k interrupts per second, there is no point to rate limit and the values
 * are set to zero. The rate limit values do affect latency, and so must
 * be reasonably small so to not impact latency sensitive tests.
 */
static const struct ice_dim rx_profile[] = {
	{2, 10},
	{8, 16},
	{32, 0},
	{96, 0},
	{128, 0}
};

/* The transmit profile, which has the same sorts of values
 * as the previous struct
 */
static const struct ice_dim tx_profile[] = {
	{2, 10},
	{8, 16},
	{64, 0},
	{128, 0},
	{256, 0}
};

static void ice_tx_dim_work(struct work_struct *work)
{
	struct ice_ring_container *rc;
	struct ice_q_vector *q_vector;
	struct dim *dim;
	u16 itr, intrl;

	dim = container_of(work, struct dim, work);
	rc = container_of(dim, struct ice_ring_container, dim);
	q_vector = container_of(rc, struct ice_q_vector, tx);

	if (dim->profile_ix >= ARRAY_SIZE(tx_profile))
		dim->profile_ix = ARRAY_SIZE(tx_profile) - 1;

	/* look up the values in our local table */
	itr = tx_profile[dim->profile_ix].itr;
	intrl = tx_profile[dim->profile_ix].intrl;

	ice_trace(tx_dim_work, q_vector, dim);
	ice_write_itr(rc, itr);
	ice_write_intrl(q_vector, intrl);

	dim->state = DIM_START_MEASURE;
}

static void ice_rx_dim_work(struct work_struct *work)
{
	struct ice_ring_container *rc;
	struct ice_q_vector *q_vector;
	struct dim *dim;
	u16 itr, intrl;

	dim = container_of(work, struct dim, work);
	rc = container_of(dim, struct ice_ring_container, dim);
	q_vector = container_of(rc, struct ice_q_vector, rx);

	if (dim->profile_ix >= ARRAY_SIZE(rx_profile))
		dim->profile_ix = ARRAY_SIZE(rx_profile) - 1;

	/* look up the values in our local table */
	itr = rx_profile[dim->profile_ix].itr;
	intrl = rx_profile[dim->profile_ix].intrl;

	ice_trace(rx_dim_work, q_vector, dim);
	ice_write_itr(rc, itr);
	ice_write_intrl(q_vector, intrl);

	dim->state = DIM_START_MEASURE;
>>>>>>> upstream/android-13
}

/**
 * ice_napi_enable_all - Enable NAPI for all q_vectors in the VSI
 * @vsi: the VSI being configured
 */
static void ice_napi_enable_all(struct ice_vsi *vsi)
{
	int q_idx;

	if (!vsi->netdev)
		return;

<<<<<<< HEAD
	for (q_idx = 0; q_idx < vsi->num_q_vectors; q_idx++) {
		struct ice_q_vector *q_vector = vsi->q_vectors[q_idx];

=======
	ice_for_each_q_vector(vsi, q_idx) {
		struct ice_q_vector *q_vector = vsi->q_vectors[q_idx];

		INIT_WORK(&q_vector->tx.dim.work, ice_tx_dim_work);
		q_vector->tx.dim.mode = DIM_CQ_PERIOD_MODE_START_FROM_EQE;

		INIT_WORK(&q_vector->rx.dim.work, ice_rx_dim_work);
		q_vector->rx.dim.mode = DIM_CQ_PERIOD_MODE_START_FROM_EQE;

>>>>>>> upstream/android-13
		if (q_vector->rx.ring || q_vector->tx.ring)
			napi_enable(&q_vector->napi);
	}
}

/**
 * ice_up_complete - Finish the last steps of bringing up a connection
 * @vsi: The VSI being configured
 *
 * Return 0 on success and negative value on error
 */
static int ice_up_complete(struct ice_vsi *vsi)
{
	struct ice_pf *pf = vsi->back;
	int err;

<<<<<<< HEAD
	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags))
		ice_vsi_cfg_msix(vsi);
	else
		return -ENOTSUPP;
=======
	ice_vsi_cfg_msix(vsi);
>>>>>>> upstream/android-13

	/* Enable only Rx rings, Tx rings were enabled by the FW when the
	 * Tx queue group list was configured and the context bits were
	 * programmed using ice_vsi_cfg_txqs
	 */
<<<<<<< HEAD
	err = ice_vsi_start_rx_rings(vsi);
	if (err)
		return err;

	clear_bit(__ICE_DOWN, vsi->state);
=======
	err = ice_vsi_start_all_rx_rings(vsi);
	if (err)
		return err;

	clear_bit(ICE_VSI_DOWN, vsi->state);
>>>>>>> upstream/android-13
	ice_napi_enable_all(vsi);
	ice_vsi_ena_irq(vsi);

	if (vsi->port_info &&
	    (vsi->port_info->phy.link_info.link_info & ICE_AQ_LINK_UP) &&
	    vsi->netdev) {
		ice_print_link_msg(vsi, true);
		netif_tx_start_all_queues(vsi->netdev);
		netif_carrier_on(vsi->netdev);
	}

<<<<<<< HEAD
	ice_service_task_schedule(pf);

	return err;
=======
	/* clear this now, and the first stats read will be used as baseline */
	vsi->stat_offsets_loaded = false;

	ice_service_task_schedule(pf);

	return 0;
>>>>>>> upstream/android-13
}

/**
 * ice_up - Bring the connection back up after being down
 * @vsi: VSI being configured
 */
int ice_up(struct ice_vsi *vsi)
{
	int err;

	err = ice_vsi_cfg(vsi);
	if (!err)
		err = ice_up_complete(vsi);

	return err;
}

/**
 * ice_fetch_u64_stats_per_ring - get packets and bytes stats per ring
 * @ring: Tx or Rx ring to read stats from
 * @pkts: packets stats counter
 * @bytes: bytes stats counter
 *
 * This function fetches stats from the ring considering the atomic operations
 * that needs to be performed to read u64 values in 32 bit machine.
 */
<<<<<<< HEAD
static void ice_fetch_u64_stats_per_ring(struct ice_ring *ring, u64 *pkts,
					 u64 *bytes)
=======
static void
ice_fetch_u64_stats_per_ring(struct ice_ring *ring, u64 *pkts, u64 *bytes)
>>>>>>> upstream/android-13
{
	unsigned int start;
	*pkts = 0;
	*bytes = 0;

	if (!ring)
		return;
	do {
		start = u64_stats_fetch_begin_irq(&ring->syncp);
		*pkts = ring->stats.pkts;
		*bytes = ring->stats.bytes;
	} while (u64_stats_fetch_retry_irq(&ring->syncp, start));
}

/**
<<<<<<< HEAD
 * ice_stat_update40 - read 40 bit stat from the chip and update stat values
 * @hw: ptr to the hardware info
 * @hireg: high 32 bit HW register to read from
 * @loreg: low 32 bit HW register to read from
 * @prev_stat_loaded: bool to specify if previous stats are loaded
 * @prev_stat: ptr to previous loaded stat value
 * @cur_stat: ptr to current stat value
 */
static void ice_stat_update40(struct ice_hw *hw, u32 hireg, u32 loreg,
			      bool prev_stat_loaded, u64 *prev_stat,
			      u64 *cur_stat)
{
	u64 new_data;

	new_data = rd32(hw, loreg);
	new_data |= ((u64)(rd32(hw, hireg) & 0xFFFF)) << 32;

	/* device stats are not reset at PFR, they likely will not be zeroed
	 * when the driver starts. So save the first values read and use them as
	 * offsets to be subtracted from the raw values in order to report stats
	 * that count from zero.
	 */
	if (!prev_stat_loaded)
		*prev_stat = new_data;
	if (likely(new_data >= *prev_stat))
		*cur_stat = new_data - *prev_stat;
	else
		/* to manage the potential roll-over */
		*cur_stat = (new_data + BIT_ULL(40)) - *prev_stat;
	*cur_stat &= 0xFFFFFFFFFFULL;
}

/**
 * ice_stat_update32 - read 32 bit stat from the chip and update stat values
 * @hw: ptr to the hardware info
 * @reg: HW register to read from
 * @prev_stat_loaded: bool to specify if previous stats are loaded
 * @prev_stat: ptr to previous loaded stat value
 * @cur_stat: ptr to current stat value
 */
static void ice_stat_update32(struct ice_hw *hw, u32 reg, bool prev_stat_loaded,
			      u64 *prev_stat, u64 *cur_stat)
{
	u32 new_data;

	new_data = rd32(hw, reg);

	/* device stats are not reset at PFR, they likely will not be zeroed
	 * when the driver starts. So save the first values read and use them as
	 * offsets to be subtracted from the raw values in order to report stats
	 * that count from zero.
	 */
	if (!prev_stat_loaded)
		*prev_stat = new_data;
	if (likely(new_data >= *prev_stat))
		*cur_stat = new_data - *prev_stat;
	else
		/* to manage the potential roll-over */
		*cur_stat = (new_data + BIT_ULL(32)) - *prev_stat;
}

/**
 * ice_update_eth_stats - Update VSI-specific ethernet statistics counters
 * @vsi: the VSI to be updated
 */
static void ice_update_eth_stats(struct ice_vsi *vsi)
{
	struct ice_eth_stats *prev_es, *cur_es;
	struct ice_hw *hw = &vsi->back->hw;
	u16 vsi_num = vsi->vsi_num;    /* HW absolute index of a VSI */

	prev_es = &vsi->eth_stats_prev;
	cur_es = &vsi->eth_stats;

	ice_stat_update40(hw, GLV_GORCH(vsi_num), GLV_GORCL(vsi_num),
			  vsi->stat_offsets_loaded, &prev_es->rx_bytes,
			  &cur_es->rx_bytes);

	ice_stat_update40(hw, GLV_UPRCH(vsi_num), GLV_UPRCL(vsi_num),
			  vsi->stat_offsets_loaded, &prev_es->rx_unicast,
			  &cur_es->rx_unicast);

	ice_stat_update40(hw, GLV_MPRCH(vsi_num), GLV_MPRCL(vsi_num),
			  vsi->stat_offsets_loaded, &prev_es->rx_multicast,
			  &cur_es->rx_multicast);

	ice_stat_update40(hw, GLV_BPRCH(vsi_num), GLV_BPRCL(vsi_num),
			  vsi->stat_offsets_loaded, &prev_es->rx_broadcast,
			  &cur_es->rx_broadcast);

	ice_stat_update32(hw, GLV_RDPC(vsi_num), vsi->stat_offsets_loaded,
			  &prev_es->rx_discards, &cur_es->rx_discards);

	ice_stat_update40(hw, GLV_GOTCH(vsi_num), GLV_GOTCL(vsi_num),
			  vsi->stat_offsets_loaded, &prev_es->tx_bytes,
			  &cur_es->tx_bytes);

	ice_stat_update40(hw, GLV_UPTCH(vsi_num), GLV_UPTCL(vsi_num),
			  vsi->stat_offsets_loaded, &prev_es->tx_unicast,
			  &cur_es->tx_unicast);

	ice_stat_update40(hw, GLV_MPTCH(vsi_num), GLV_MPTCL(vsi_num),
			  vsi->stat_offsets_loaded, &prev_es->tx_multicast,
			  &cur_es->tx_multicast);

	ice_stat_update40(hw, GLV_BPTCH(vsi_num), GLV_BPTCL(vsi_num),
			  vsi->stat_offsets_loaded, &prev_es->tx_broadcast,
			  &cur_es->tx_broadcast);

	ice_stat_update32(hw, GLV_TEPC(vsi_num), vsi->stat_offsets_loaded,
			  &prev_es->tx_errors, &cur_es->tx_errors);

	vsi->stat_offsets_loaded = true;
=======
 * ice_update_vsi_tx_ring_stats - Update VSI Tx ring stats counters
 * @vsi: the VSI to be updated
 * @rings: rings to work on
 * @count: number of rings
 */
static void
ice_update_vsi_tx_ring_stats(struct ice_vsi *vsi, struct ice_ring **rings,
			     u16 count)
{
	struct rtnl_link_stats64 *vsi_stats = &vsi->net_stats;
	u16 i;

	for (i = 0; i < count; i++) {
		struct ice_ring *ring;
		u64 pkts, bytes;

		ring = READ_ONCE(rings[i]);
		ice_fetch_u64_stats_per_ring(ring, &pkts, &bytes);
		vsi_stats->tx_packets += pkts;
		vsi_stats->tx_bytes += bytes;
		vsi->tx_restart += ring->tx_stats.restart_q;
		vsi->tx_busy += ring->tx_stats.tx_busy;
		vsi->tx_linearize += ring->tx_stats.tx_linearize;
	}
>>>>>>> upstream/android-13
}

/**
 * ice_update_vsi_ring_stats - Update VSI stats counters
 * @vsi: the VSI to be updated
 */
static void ice_update_vsi_ring_stats(struct ice_vsi *vsi)
{
	struct rtnl_link_stats64 *vsi_stats = &vsi->net_stats;
<<<<<<< HEAD
	struct ice_ring *ring;
=======
>>>>>>> upstream/android-13
	u64 pkts, bytes;
	int i;

	/* reset netdev stats */
	vsi_stats->tx_packets = 0;
	vsi_stats->tx_bytes = 0;
	vsi_stats->rx_packets = 0;
	vsi_stats->rx_bytes = 0;

	/* reset non-netdev (extended) stats */
	vsi->tx_restart = 0;
	vsi->tx_busy = 0;
	vsi->tx_linearize = 0;
	vsi->rx_buf_failed = 0;
	vsi->rx_page_failed = 0;

	rcu_read_lock();

	/* update Tx rings counters */
<<<<<<< HEAD
	ice_for_each_txq(vsi, i) {
		ring = READ_ONCE(vsi->tx_rings[i]);
		ice_fetch_u64_stats_per_ring(ring, &pkts, &bytes);
		vsi_stats->tx_packets += pkts;
		vsi_stats->tx_bytes += bytes;
		vsi->tx_restart += ring->tx_stats.restart_q;
		vsi->tx_busy += ring->tx_stats.tx_busy;
		vsi->tx_linearize += ring->tx_stats.tx_linearize;
	}

	/* update Rx rings counters */
	ice_for_each_rxq(vsi, i) {
		ring = READ_ONCE(vsi->rx_rings[i]);
=======
	ice_update_vsi_tx_ring_stats(vsi, vsi->tx_rings, vsi->num_txq);

	/* update Rx rings counters */
	ice_for_each_rxq(vsi, i) {
		struct ice_ring *ring = READ_ONCE(vsi->rx_rings[i]);

>>>>>>> upstream/android-13
		ice_fetch_u64_stats_per_ring(ring, &pkts, &bytes);
		vsi_stats->rx_packets += pkts;
		vsi_stats->rx_bytes += bytes;
		vsi->rx_buf_failed += ring->rx_stats.alloc_buf_failed;
		vsi->rx_page_failed += ring->rx_stats.alloc_page_failed;
	}

<<<<<<< HEAD
=======
	/* update XDP Tx rings counters */
	if (ice_is_xdp_ena_vsi(vsi))
		ice_update_vsi_tx_ring_stats(vsi, vsi->xdp_rings,
					     vsi->num_xdp_txq);

>>>>>>> upstream/android-13
	rcu_read_unlock();
}

/**
 * ice_update_vsi_stats - Update VSI stats counters
 * @vsi: the VSI to be updated
 */
<<<<<<< HEAD
static void ice_update_vsi_stats(struct ice_vsi *vsi)
=======
void ice_update_vsi_stats(struct ice_vsi *vsi)
>>>>>>> upstream/android-13
{
	struct rtnl_link_stats64 *cur_ns = &vsi->net_stats;
	struct ice_eth_stats *cur_es = &vsi->eth_stats;
	struct ice_pf *pf = vsi->back;

<<<<<<< HEAD
	if (test_bit(__ICE_DOWN, vsi->state) ||
	    test_bit(__ICE_CFG_BUSY, pf->state))
=======
	if (test_bit(ICE_VSI_DOWN, vsi->state) ||
	    test_bit(ICE_CFG_BUSY, pf->state))
>>>>>>> upstream/android-13
		return;

	/* get stats as recorded by Tx/Rx rings */
	ice_update_vsi_ring_stats(vsi);

	/* get VSI stats as recorded by the hardware */
	ice_update_eth_stats(vsi);

	cur_ns->tx_errors = cur_es->tx_errors;
	cur_ns->rx_dropped = cur_es->rx_discards;
	cur_ns->tx_dropped = cur_es->tx_discards;
	cur_ns->multicast = cur_es->rx_multicast;

	/* update some more netdev stats if this is main VSI */
	if (vsi->type == ICE_VSI_PF) {
		cur_ns->rx_crc_errors = pf->stats.crc_errors;
		cur_ns->rx_errors = pf->stats.crc_errors +
<<<<<<< HEAD
				    pf->stats.illegal_bytes;
		cur_ns->rx_length_errors = pf->stats.rx_len_errors;
=======
				    pf->stats.illegal_bytes +
				    pf->stats.rx_len_errors +
				    pf->stats.rx_undersize +
				    pf->hw_csum_rx_error +
				    pf->stats.rx_jabber +
				    pf->stats.rx_fragments +
				    pf->stats.rx_oversize;
		cur_ns->rx_length_errors = pf->stats.rx_len_errors;
		/* record drops from the port level */
		cur_ns->rx_missed_errors = pf->stats.eth.rx_discards;
>>>>>>> upstream/android-13
	}
}

/**
 * ice_update_pf_stats - Update PF port stats counters
 * @pf: PF whose stats needs to be updated
 */
<<<<<<< HEAD
static void ice_update_pf_stats(struct ice_pf *pf)
{
	struct ice_hw_port_stats *prev_ps, *cur_ps;
	struct ice_hw *hw = &pf->hw;
	u8 pf_id;

	prev_ps = &pf->stats_prev;
	cur_ps = &pf->stats;
	pf_id = hw->pf_id;

	ice_stat_update40(hw, GLPRT_GORCH(pf_id), GLPRT_GORCL(pf_id),
			  pf->stat_prev_loaded, &prev_ps->eth.rx_bytes,
			  &cur_ps->eth.rx_bytes);

	ice_stat_update40(hw, GLPRT_UPRCH(pf_id), GLPRT_UPRCL(pf_id),
			  pf->stat_prev_loaded, &prev_ps->eth.rx_unicast,
			  &cur_ps->eth.rx_unicast);

	ice_stat_update40(hw, GLPRT_MPRCH(pf_id), GLPRT_MPRCL(pf_id),
			  pf->stat_prev_loaded, &prev_ps->eth.rx_multicast,
			  &cur_ps->eth.rx_multicast);

	ice_stat_update40(hw, GLPRT_BPRCH(pf_id), GLPRT_BPRCL(pf_id),
			  pf->stat_prev_loaded, &prev_ps->eth.rx_broadcast,
			  &cur_ps->eth.rx_broadcast);

	ice_stat_update40(hw, GLPRT_GOTCH(pf_id), GLPRT_GOTCL(pf_id),
			  pf->stat_prev_loaded, &prev_ps->eth.tx_bytes,
			  &cur_ps->eth.tx_bytes);

	ice_stat_update40(hw, GLPRT_UPTCH(pf_id), GLPRT_UPTCL(pf_id),
			  pf->stat_prev_loaded, &prev_ps->eth.tx_unicast,
			  &cur_ps->eth.tx_unicast);

	ice_stat_update40(hw, GLPRT_MPTCH(pf_id), GLPRT_MPTCL(pf_id),
			  pf->stat_prev_loaded, &prev_ps->eth.tx_multicast,
			  &cur_ps->eth.tx_multicast);

	ice_stat_update40(hw, GLPRT_BPTCH(pf_id), GLPRT_BPTCL(pf_id),
			  pf->stat_prev_loaded, &prev_ps->eth.tx_broadcast,
			  &cur_ps->eth.tx_broadcast);

	ice_stat_update32(hw, GLPRT_TDOLD(pf_id), pf->stat_prev_loaded,
			  &prev_ps->tx_dropped_link_down,
			  &cur_ps->tx_dropped_link_down);

	ice_stat_update40(hw, GLPRT_PRC64H(pf_id), GLPRT_PRC64L(pf_id),
			  pf->stat_prev_loaded, &prev_ps->rx_size_64,
			  &cur_ps->rx_size_64);

	ice_stat_update40(hw, GLPRT_PRC127H(pf_id), GLPRT_PRC127L(pf_id),
			  pf->stat_prev_loaded, &prev_ps->rx_size_127,
			  &cur_ps->rx_size_127);

	ice_stat_update40(hw, GLPRT_PRC255H(pf_id), GLPRT_PRC255L(pf_id),
			  pf->stat_prev_loaded, &prev_ps->rx_size_255,
			  &cur_ps->rx_size_255);

	ice_stat_update40(hw, GLPRT_PRC511H(pf_id), GLPRT_PRC511L(pf_id),
			  pf->stat_prev_loaded, &prev_ps->rx_size_511,
			  &cur_ps->rx_size_511);

	ice_stat_update40(hw, GLPRT_PRC1023H(pf_id),
			  GLPRT_PRC1023L(pf_id), pf->stat_prev_loaded,
			  &prev_ps->rx_size_1023, &cur_ps->rx_size_1023);

	ice_stat_update40(hw, GLPRT_PRC1522H(pf_id),
			  GLPRT_PRC1522L(pf_id), pf->stat_prev_loaded,
			  &prev_ps->rx_size_1522, &cur_ps->rx_size_1522);

	ice_stat_update40(hw, GLPRT_PRC9522H(pf_id),
			  GLPRT_PRC9522L(pf_id), pf->stat_prev_loaded,
			  &prev_ps->rx_size_big, &cur_ps->rx_size_big);

	ice_stat_update40(hw, GLPRT_PTC64H(pf_id), GLPRT_PTC64L(pf_id),
			  pf->stat_prev_loaded, &prev_ps->tx_size_64,
			  &cur_ps->tx_size_64);

	ice_stat_update40(hw, GLPRT_PTC127H(pf_id), GLPRT_PTC127L(pf_id),
			  pf->stat_prev_loaded, &prev_ps->tx_size_127,
			  &cur_ps->tx_size_127);

	ice_stat_update40(hw, GLPRT_PTC255H(pf_id), GLPRT_PTC255L(pf_id),
			  pf->stat_prev_loaded, &prev_ps->tx_size_255,
			  &cur_ps->tx_size_255);

	ice_stat_update40(hw, GLPRT_PTC511H(pf_id), GLPRT_PTC511L(pf_id),
			  pf->stat_prev_loaded, &prev_ps->tx_size_511,
			  &cur_ps->tx_size_511);

	ice_stat_update40(hw, GLPRT_PTC1023H(pf_id),
			  GLPRT_PTC1023L(pf_id), pf->stat_prev_loaded,
			  &prev_ps->tx_size_1023, &cur_ps->tx_size_1023);

	ice_stat_update40(hw, GLPRT_PTC1522H(pf_id),
			  GLPRT_PTC1522L(pf_id), pf->stat_prev_loaded,
			  &prev_ps->tx_size_1522, &cur_ps->tx_size_1522);

	ice_stat_update40(hw, GLPRT_PTC9522H(pf_id),
			  GLPRT_PTC9522L(pf_id), pf->stat_prev_loaded,
			  &prev_ps->tx_size_big, &cur_ps->tx_size_big);

	ice_stat_update32(hw, GLPRT_LXONRXC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->link_xon_rx, &cur_ps->link_xon_rx);

	ice_stat_update32(hw, GLPRT_LXOFFRXC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->link_xoff_rx, &cur_ps->link_xoff_rx);

	ice_stat_update32(hw, GLPRT_LXONTXC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->link_xon_tx, &cur_ps->link_xon_tx);

	ice_stat_update32(hw, GLPRT_LXOFFTXC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->link_xoff_tx, &cur_ps->link_xoff_tx);

	ice_stat_update32(hw, GLPRT_CRCERRS(pf_id), pf->stat_prev_loaded,
			  &prev_ps->crc_errors, &cur_ps->crc_errors);

	ice_stat_update32(hw, GLPRT_ILLERRC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->illegal_bytes, &cur_ps->illegal_bytes);

	ice_stat_update32(hw, GLPRT_MLFC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->mac_local_faults,
			  &cur_ps->mac_local_faults);

	ice_stat_update32(hw, GLPRT_MRFC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->mac_remote_faults,
			  &cur_ps->mac_remote_faults);

	ice_stat_update32(hw, GLPRT_RLEC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->rx_len_errors, &cur_ps->rx_len_errors);

	ice_stat_update32(hw, GLPRT_RUC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->rx_undersize, &cur_ps->rx_undersize);

	ice_stat_update32(hw, GLPRT_RFC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->rx_fragments, &cur_ps->rx_fragments);

	ice_stat_update32(hw, GLPRT_ROC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->rx_oversize, &cur_ps->rx_oversize);

	ice_stat_update32(hw, GLPRT_RJC(pf_id), pf->stat_prev_loaded,
			  &prev_ps->rx_jabber, &cur_ps->rx_jabber);

=======
void ice_update_pf_stats(struct ice_pf *pf)
{
	struct ice_hw_port_stats *prev_ps, *cur_ps;
	struct ice_hw *hw = &pf->hw;
	u16 fd_ctr_base;
	u8 port;

	port = hw->port_info->lport;
	prev_ps = &pf->stats_prev;
	cur_ps = &pf->stats;

	ice_stat_update40(hw, GLPRT_GORCL(port), pf->stat_prev_loaded,
			  &prev_ps->eth.rx_bytes,
			  &cur_ps->eth.rx_bytes);

	ice_stat_update40(hw, GLPRT_UPRCL(port), pf->stat_prev_loaded,
			  &prev_ps->eth.rx_unicast,
			  &cur_ps->eth.rx_unicast);

	ice_stat_update40(hw, GLPRT_MPRCL(port), pf->stat_prev_loaded,
			  &prev_ps->eth.rx_multicast,
			  &cur_ps->eth.rx_multicast);

	ice_stat_update40(hw, GLPRT_BPRCL(port), pf->stat_prev_loaded,
			  &prev_ps->eth.rx_broadcast,
			  &cur_ps->eth.rx_broadcast);

	ice_stat_update32(hw, PRTRPB_RDPC, pf->stat_prev_loaded,
			  &prev_ps->eth.rx_discards,
			  &cur_ps->eth.rx_discards);

	ice_stat_update40(hw, GLPRT_GOTCL(port), pf->stat_prev_loaded,
			  &prev_ps->eth.tx_bytes,
			  &cur_ps->eth.tx_bytes);

	ice_stat_update40(hw, GLPRT_UPTCL(port), pf->stat_prev_loaded,
			  &prev_ps->eth.tx_unicast,
			  &cur_ps->eth.tx_unicast);

	ice_stat_update40(hw, GLPRT_MPTCL(port), pf->stat_prev_loaded,
			  &prev_ps->eth.tx_multicast,
			  &cur_ps->eth.tx_multicast);

	ice_stat_update40(hw, GLPRT_BPTCL(port), pf->stat_prev_loaded,
			  &prev_ps->eth.tx_broadcast,
			  &cur_ps->eth.tx_broadcast);

	ice_stat_update32(hw, GLPRT_TDOLD(port), pf->stat_prev_loaded,
			  &prev_ps->tx_dropped_link_down,
			  &cur_ps->tx_dropped_link_down);

	ice_stat_update40(hw, GLPRT_PRC64L(port), pf->stat_prev_loaded,
			  &prev_ps->rx_size_64, &cur_ps->rx_size_64);

	ice_stat_update40(hw, GLPRT_PRC127L(port), pf->stat_prev_loaded,
			  &prev_ps->rx_size_127, &cur_ps->rx_size_127);

	ice_stat_update40(hw, GLPRT_PRC255L(port), pf->stat_prev_loaded,
			  &prev_ps->rx_size_255, &cur_ps->rx_size_255);

	ice_stat_update40(hw, GLPRT_PRC511L(port), pf->stat_prev_loaded,
			  &prev_ps->rx_size_511, &cur_ps->rx_size_511);

	ice_stat_update40(hw, GLPRT_PRC1023L(port), pf->stat_prev_loaded,
			  &prev_ps->rx_size_1023, &cur_ps->rx_size_1023);

	ice_stat_update40(hw, GLPRT_PRC1522L(port), pf->stat_prev_loaded,
			  &prev_ps->rx_size_1522, &cur_ps->rx_size_1522);

	ice_stat_update40(hw, GLPRT_PRC9522L(port), pf->stat_prev_loaded,
			  &prev_ps->rx_size_big, &cur_ps->rx_size_big);

	ice_stat_update40(hw, GLPRT_PTC64L(port), pf->stat_prev_loaded,
			  &prev_ps->tx_size_64, &cur_ps->tx_size_64);

	ice_stat_update40(hw, GLPRT_PTC127L(port), pf->stat_prev_loaded,
			  &prev_ps->tx_size_127, &cur_ps->tx_size_127);

	ice_stat_update40(hw, GLPRT_PTC255L(port), pf->stat_prev_loaded,
			  &prev_ps->tx_size_255, &cur_ps->tx_size_255);

	ice_stat_update40(hw, GLPRT_PTC511L(port), pf->stat_prev_loaded,
			  &prev_ps->tx_size_511, &cur_ps->tx_size_511);

	ice_stat_update40(hw, GLPRT_PTC1023L(port), pf->stat_prev_loaded,
			  &prev_ps->tx_size_1023, &cur_ps->tx_size_1023);

	ice_stat_update40(hw, GLPRT_PTC1522L(port), pf->stat_prev_loaded,
			  &prev_ps->tx_size_1522, &cur_ps->tx_size_1522);

	ice_stat_update40(hw, GLPRT_PTC9522L(port), pf->stat_prev_loaded,
			  &prev_ps->tx_size_big, &cur_ps->tx_size_big);

	fd_ctr_base = hw->fd_ctr_base;

	ice_stat_update40(hw,
			  GLSTAT_FD_CNT0L(ICE_FD_SB_STAT_IDX(fd_ctr_base)),
			  pf->stat_prev_loaded, &prev_ps->fd_sb_match,
			  &cur_ps->fd_sb_match);
	ice_stat_update32(hw, GLPRT_LXONRXC(port), pf->stat_prev_loaded,
			  &prev_ps->link_xon_rx, &cur_ps->link_xon_rx);

	ice_stat_update32(hw, GLPRT_LXOFFRXC(port), pf->stat_prev_loaded,
			  &prev_ps->link_xoff_rx, &cur_ps->link_xoff_rx);

	ice_stat_update32(hw, GLPRT_LXONTXC(port), pf->stat_prev_loaded,
			  &prev_ps->link_xon_tx, &cur_ps->link_xon_tx);

	ice_stat_update32(hw, GLPRT_LXOFFTXC(port), pf->stat_prev_loaded,
			  &prev_ps->link_xoff_tx, &cur_ps->link_xoff_tx);

	ice_update_dcb_stats(pf);

	ice_stat_update32(hw, GLPRT_CRCERRS(port), pf->stat_prev_loaded,
			  &prev_ps->crc_errors, &cur_ps->crc_errors);

	ice_stat_update32(hw, GLPRT_ILLERRC(port), pf->stat_prev_loaded,
			  &prev_ps->illegal_bytes, &cur_ps->illegal_bytes);

	ice_stat_update32(hw, GLPRT_MLFC(port), pf->stat_prev_loaded,
			  &prev_ps->mac_local_faults,
			  &cur_ps->mac_local_faults);

	ice_stat_update32(hw, GLPRT_MRFC(port), pf->stat_prev_loaded,
			  &prev_ps->mac_remote_faults,
			  &cur_ps->mac_remote_faults);

	ice_stat_update32(hw, GLPRT_RLEC(port), pf->stat_prev_loaded,
			  &prev_ps->rx_len_errors, &cur_ps->rx_len_errors);

	ice_stat_update32(hw, GLPRT_RUC(port), pf->stat_prev_loaded,
			  &prev_ps->rx_undersize, &cur_ps->rx_undersize);

	ice_stat_update32(hw, GLPRT_RFC(port), pf->stat_prev_loaded,
			  &prev_ps->rx_fragments, &cur_ps->rx_fragments);

	ice_stat_update32(hw, GLPRT_ROC(port), pf->stat_prev_loaded,
			  &prev_ps->rx_oversize, &cur_ps->rx_oversize);

	ice_stat_update32(hw, GLPRT_RJC(port), pf->stat_prev_loaded,
			  &prev_ps->rx_jabber, &cur_ps->rx_jabber);

	cur_ps->fd_sb_status = test_bit(ICE_FLAG_FD_ENA, pf->flags) ? 1 : 0;

>>>>>>> upstream/android-13
	pf->stat_prev_loaded = true;
}

/**
 * ice_get_stats64 - get statistics for network device structure
 * @netdev: network interface device structure
 * @stats: main device statistics structure
 */
static
void ice_get_stats64(struct net_device *netdev, struct rtnl_link_stats64 *stats)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct rtnl_link_stats64 *vsi_stats;
	struct ice_vsi *vsi = np->vsi;

	vsi_stats = &vsi->net_stats;

<<<<<<< HEAD
	if (test_bit(__ICE_DOWN, vsi->state) || !vsi->num_txq || !vsi->num_rxq)
		return;
	/* netdev packet/byte stats come from ring counter. These are obtained
	 * by summing up ring counters (done by ice_update_vsi_ring_stats).
	 */
	ice_update_vsi_ring_stats(vsi);
=======
	if (!vsi->num_txq || !vsi->num_rxq)
		return;

	/* netdev packet/byte stats come from ring counter. These are obtained
	 * by summing up ring counters (done by ice_update_vsi_ring_stats).
	 * But, only call the update routine and read the registers if VSI is
	 * not down.
	 */
	if (!test_bit(ICE_VSI_DOWN, vsi->state))
		ice_update_vsi_ring_stats(vsi);
>>>>>>> upstream/android-13
	stats->tx_packets = vsi_stats->tx_packets;
	stats->tx_bytes = vsi_stats->tx_bytes;
	stats->rx_packets = vsi_stats->rx_packets;
	stats->rx_bytes = vsi_stats->rx_bytes;

	/* The rest of the stats can be read from the hardware but instead we
	 * just return values that the watchdog task has already obtained from
	 * the hardware.
	 */
	stats->multicast = vsi_stats->multicast;
	stats->tx_errors = vsi_stats->tx_errors;
	stats->tx_dropped = vsi_stats->tx_dropped;
	stats->rx_errors = vsi_stats->rx_errors;
	stats->rx_dropped = vsi_stats->rx_dropped;
	stats->rx_crc_errors = vsi_stats->rx_crc_errors;
	stats->rx_length_errors = vsi_stats->rx_length_errors;
}

/**
 * ice_napi_disable_all - Disable NAPI for all q_vectors in the VSI
 * @vsi: VSI having NAPI disabled
 */
static void ice_napi_disable_all(struct ice_vsi *vsi)
{
	int q_idx;

	if (!vsi->netdev)
		return;

<<<<<<< HEAD
	for (q_idx = 0; q_idx < vsi->num_q_vectors; q_idx++) {
=======
	ice_for_each_q_vector(vsi, q_idx) {
>>>>>>> upstream/android-13
		struct ice_q_vector *q_vector = vsi->q_vectors[q_idx];

		if (q_vector->rx.ring || q_vector->tx.ring)
			napi_disable(&q_vector->napi);
<<<<<<< HEAD
=======

		cancel_work_sync(&q_vector->tx.dim.work);
		cancel_work_sync(&q_vector->rx.dim.work);
>>>>>>> upstream/android-13
	}
}

/**
 * ice_down - Shutdown the connection
 * @vsi: The VSI being stopped
 */
int ice_down(struct ice_vsi *vsi)
{
<<<<<<< HEAD
	int i, err;

	/* Caller of this function is expected to set the
	 * vsi->state __ICE_DOWN bit
=======
	int i, tx_err, rx_err, link_err = 0;

	/* Caller of this function is expected to set the
	 * vsi->state ICE_DOWN bit
>>>>>>> upstream/android-13
	 */
	if (vsi->netdev) {
		netif_carrier_off(vsi->netdev);
		netif_tx_disable(vsi->netdev);
	}

	ice_vsi_dis_irq(vsi);
<<<<<<< HEAD
	err = ice_vsi_stop_tx_rx_rings(vsi);
	ice_napi_disable_all(vsi);

=======

	tx_err = ice_vsi_stop_lan_tx_rings(vsi, ICE_NO_RESET, 0);
	if (tx_err)
		netdev_err(vsi->netdev, "Failed stop Tx rings, VSI %d error %d\n",
			   vsi->vsi_num, tx_err);
	if (!tx_err && ice_is_xdp_ena_vsi(vsi)) {
		tx_err = ice_vsi_stop_xdp_tx_rings(vsi);
		if (tx_err)
			netdev_err(vsi->netdev, "Failed stop XDP rings, VSI %d error %d\n",
				   vsi->vsi_num, tx_err);
	}

	rx_err = ice_vsi_stop_all_rx_rings(vsi);
	if (rx_err)
		netdev_err(vsi->netdev, "Failed stop Rx rings, VSI %d error %d\n",
			   vsi->vsi_num, rx_err);

	ice_napi_disable_all(vsi);

	if (test_bit(ICE_FLAG_LINK_DOWN_ON_CLOSE_ENA, vsi->back->flags)) {
		link_err = ice_force_phys_link_state(vsi, false);
		if (link_err)
			netdev_err(vsi->netdev, "Failed to set physical link down, VSI %d error %d\n",
				   vsi->vsi_num, link_err);
	}

>>>>>>> upstream/android-13
	ice_for_each_txq(vsi, i)
		ice_clean_tx_ring(vsi->tx_rings[i]);

	ice_for_each_rxq(vsi, i)
		ice_clean_rx_ring(vsi->rx_rings[i]);

<<<<<<< HEAD
	if (err)
		netdev_err(vsi->netdev, "Failed to close VSI 0x%04X on switch 0x%04X\n",
			   vsi->vsi_num, vsi->vsw->sw_id);
	return err;
=======
	if (tx_err || rx_err || link_err) {
		netdev_err(vsi->netdev, "Failed to close VSI 0x%04X on switch 0x%04X\n",
			   vsi->vsi_num, vsi->vsw->sw_id);
		return -EIO;
	}

	return 0;
>>>>>>> upstream/android-13
}

/**
 * ice_vsi_setup_tx_rings - Allocate VSI Tx queue resources
 * @vsi: VSI having resources allocated
 *
 * Return 0 on success, negative on failure
 */
<<<<<<< HEAD
static int ice_vsi_setup_tx_rings(struct ice_vsi *vsi)
=======
int ice_vsi_setup_tx_rings(struct ice_vsi *vsi)
>>>>>>> upstream/android-13
{
	int i, err = 0;

	if (!vsi->num_txq) {
<<<<<<< HEAD
		dev_err(&vsi->back->pdev->dev, "VSI %d has 0 Tx queues\n",
=======
		dev_err(ice_pf_to_dev(vsi->back), "VSI %d has 0 Tx queues\n",
>>>>>>> upstream/android-13
			vsi->vsi_num);
		return -EINVAL;
	}

	ice_for_each_txq(vsi, i) {
<<<<<<< HEAD
		err = ice_setup_tx_ring(vsi->tx_rings[i]);
=======
		struct ice_ring *ring = vsi->tx_rings[i];

		if (!ring)
			return -EINVAL;

		ring->netdev = vsi->netdev;
		err = ice_setup_tx_ring(ring);
>>>>>>> upstream/android-13
		if (err)
			break;
	}

	return err;
}

/**
 * ice_vsi_setup_rx_rings - Allocate VSI Rx queue resources
 * @vsi: VSI having resources allocated
 *
 * Return 0 on success, negative on failure
 */
<<<<<<< HEAD
static int ice_vsi_setup_rx_rings(struct ice_vsi *vsi)
=======
int ice_vsi_setup_rx_rings(struct ice_vsi *vsi)
>>>>>>> upstream/android-13
{
	int i, err = 0;

	if (!vsi->num_rxq) {
<<<<<<< HEAD
		dev_err(&vsi->back->pdev->dev, "VSI %d has 0 Rx queues\n",
=======
		dev_err(ice_pf_to_dev(vsi->back), "VSI %d has 0 Rx queues\n",
>>>>>>> upstream/android-13
			vsi->vsi_num);
		return -EINVAL;
	}

	ice_for_each_rxq(vsi, i) {
<<<<<<< HEAD
		err = ice_setup_rx_ring(vsi->rx_rings[i]);
=======
		struct ice_ring *ring = vsi->rx_rings[i];

		if (!ring)
			return -EINVAL;

		ring->netdev = vsi->netdev;
		err = ice_setup_rx_ring(ring);
>>>>>>> upstream/android-13
		if (err)
			break;
	}

	return err;
}

/**
<<<<<<< HEAD
 * ice_vsi_req_irq - Request IRQ from the OS
 * @vsi: The VSI IRQ is being requested for
 * @basename: name for the vector
 *
 * Return 0 on success and a negative value on error
 */
static int ice_vsi_req_irq(struct ice_vsi *vsi, char *basename)
{
	struct ice_pf *pf = vsi->back;
	int err = -EINVAL;

	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags))
		err = ice_vsi_req_irq_msix(vsi, basename);
=======
 * ice_vsi_open_ctrl - open control VSI for use
 * @vsi: the VSI to open
 *
 * Initialization of the Control VSI
 *
 * Returns 0 on success, negative value on error
 */
int ice_vsi_open_ctrl(struct ice_vsi *vsi)
{
	char int_name[ICE_INT_NAME_STR_LEN];
	struct ice_pf *pf = vsi->back;
	struct device *dev;
	int err;

	dev = ice_pf_to_dev(pf);
	/* allocate descriptors */
	err = ice_vsi_setup_tx_rings(vsi);
	if (err)
		goto err_setup_tx;

	err = ice_vsi_setup_rx_rings(vsi);
	if (err)
		goto err_setup_rx;

	err = ice_vsi_cfg(vsi);
	if (err)
		goto err_setup_rx;

	snprintf(int_name, sizeof(int_name) - 1, "%s-%s:ctrl",
		 dev_driver_string(dev), dev_name(dev));
	err = ice_vsi_req_irq_msix(vsi, int_name);
	if (err)
		goto err_setup_rx;

	ice_vsi_cfg_msix(vsi);

	err = ice_vsi_start_all_rx_rings(vsi);
	if (err)
		goto err_up_complete;

	clear_bit(ICE_VSI_DOWN, vsi->state);
	ice_vsi_ena_irq(vsi);

	return 0;

err_up_complete:
	ice_down(vsi);
err_setup_rx:
	ice_vsi_free_rx_rings(vsi);
err_setup_tx:
	ice_vsi_free_tx_rings(vsi);
>>>>>>> upstream/android-13

	return err;
}

/**
<<<<<<< HEAD
 * ice_vsi_free_tx_rings - Free Tx resources for VSI queues
 * @vsi: the VSI having resources freed
 */
static void ice_vsi_free_tx_rings(struct ice_vsi *vsi)
{
	int i;

	if (!vsi->tx_rings)
		return;

	ice_for_each_txq(vsi, i)
		if (vsi->tx_rings[i] && vsi->tx_rings[i]->desc)
			ice_free_tx_ring(vsi->tx_rings[i]);
}

/**
 * ice_vsi_free_rx_rings - Free Rx resources for VSI queues
 * @vsi: the VSI having resources freed
 */
static void ice_vsi_free_rx_rings(struct ice_vsi *vsi)
{
	int i;

	if (!vsi->rx_rings)
		return;

	ice_for_each_rxq(vsi, i)
		if (vsi->rx_rings[i] && vsi->rx_rings[i]->desc)
			ice_free_rx_ring(vsi->rx_rings[i]);
}

/**
=======
>>>>>>> upstream/android-13
 * ice_vsi_open - Called when a network interface is made active
 * @vsi: the VSI to open
 *
 * Initialization of the VSI
 *
 * Returns 0 on success, negative value on error
 */
static int ice_vsi_open(struct ice_vsi *vsi)
{
	char int_name[ICE_INT_NAME_STR_LEN];
	struct ice_pf *pf = vsi->back;
	int err;

	/* allocate descriptors */
	err = ice_vsi_setup_tx_rings(vsi);
	if (err)
		goto err_setup_tx;

	err = ice_vsi_setup_rx_rings(vsi);
	if (err)
		goto err_setup_rx;

	err = ice_vsi_cfg(vsi);
	if (err)
		goto err_setup_rx;

	snprintf(int_name, sizeof(int_name) - 1, "%s-%s",
<<<<<<< HEAD
		 dev_driver_string(&pf->pdev->dev), vsi->netdev->name);
	err = ice_vsi_req_irq(vsi, int_name);
=======
		 dev_driver_string(ice_pf_to_dev(pf)), vsi->netdev->name);
	err = ice_vsi_req_irq_msix(vsi, int_name);
>>>>>>> upstream/android-13
	if (err)
		goto err_setup_rx;

	/* Notify the stack of the actual queue counts. */
	err = netif_set_real_num_tx_queues(vsi->netdev, vsi->num_txq);
	if (err)
		goto err_set_qs;

	err = netif_set_real_num_rx_queues(vsi->netdev, vsi->num_rxq);
	if (err)
		goto err_set_qs;

	err = ice_up_complete(vsi);
	if (err)
		goto err_up_complete;

	return 0;

err_up_complete:
	ice_down(vsi);
err_set_qs:
	ice_vsi_free_irq(vsi);
err_setup_rx:
	ice_vsi_free_rx_rings(vsi);
err_setup_tx:
	ice_vsi_free_tx_rings(vsi);

	return err;
}

/**
<<<<<<< HEAD
 * ice_vsi_close - Shut down a VSI
 * @vsi: the VSI being shut down
 */
static void ice_vsi_close(struct ice_vsi *vsi)
{
	if (!test_and_set_bit(__ICE_DOWN, vsi->state))
		ice_down(vsi);

	ice_vsi_free_irq(vsi);
	ice_vsi_free_tx_rings(vsi);
	ice_vsi_free_rx_rings(vsi);
}

/**
 * ice_rss_clean - Delete RSS related VSI structures that hold user inputs
 * @vsi: the VSI being removed
 */
static void ice_rss_clean(struct ice_vsi *vsi)
{
	struct ice_pf *pf;

	pf = vsi->back;

	if (vsi->rss_hkey_user)
		devm_kfree(&pf->pdev->dev, vsi->rss_hkey_user);
	if (vsi->rss_lut_user)
		devm_kfree(&pf->pdev->dev, vsi->rss_lut_user);
}

/**
 * ice_vsi_release - Delete a VSI and free its resources
 * @vsi: the VSI being removed
 *
 * Returns 0 on success or < 0 on error
 */
static int ice_vsi_release(struct ice_vsi *vsi)
{
	struct ice_pf *pf;

	if (!vsi->back)
		return -ENODEV;
	pf = vsi->back;

	if (vsi->netdev) {
		unregister_netdev(vsi->netdev);
		free_netdev(vsi->netdev);
		vsi->netdev = NULL;
	}

	if (test_bit(ICE_FLAG_RSS_ENA, pf->flags))
		ice_rss_clean(vsi);

	/* Disable VSI and free resources */
	ice_vsi_dis_irq(vsi);
	ice_vsi_close(vsi);

	/* reclaim interrupt vectors back to PF */
	ice_free_res(vsi->back->irq_tracker, vsi->base_vector, vsi->idx);
	pf->num_avail_msix += vsi->num_q_vectors;

	ice_remove_vsi_fltr(&pf->hw, vsi->vsi_num);
	ice_vsi_delete(vsi);
	ice_vsi_free_q_vectors(vsi);
	ice_vsi_clear_rings(vsi);

	ice_vsi_put_qs(vsi);
	pf->q_left_tx += vsi->alloc_txq;
	pf->q_left_rx += vsi->alloc_rxq;

	ice_vsi_clear(vsi);
=======
 * ice_vsi_release_all - Delete all VSIs
 * @pf: PF from which all VSIs are being removed
 */
static void ice_vsi_release_all(struct ice_pf *pf)
{
	int err, i;

	if (!pf->vsi)
		return;

	ice_for_each_vsi(pf, i) {
		if (!pf->vsi[i])
			continue;

		err = ice_vsi_release(pf->vsi[i]);
		if (err)
			dev_dbg(ice_pf_to_dev(pf), "Failed to release pf->vsi[%d], err %d, vsi_num = %d\n",
				i, err, pf->vsi[i]->vsi_num);
	}
}

/**
 * ice_vsi_rebuild_by_type - Rebuild VSI of a given type
 * @pf: pointer to the PF instance
 * @type: VSI type to rebuild
 *
 * Iterates through the pf->vsi array and rebuilds VSIs of the requested type
 */
static int ice_vsi_rebuild_by_type(struct ice_pf *pf, enum ice_vsi_type type)
{
	struct device *dev = ice_pf_to_dev(pf);
	enum ice_status status;
	int i, err;

	ice_for_each_vsi(pf, i) {
		struct ice_vsi *vsi = pf->vsi[i];

		if (!vsi || vsi->type != type)
			continue;

		/* rebuild the VSI */
		err = ice_vsi_rebuild(vsi, true);
		if (err) {
			dev_err(dev, "rebuild VSI failed, err %d, VSI index %d, type %s\n",
				err, vsi->idx, ice_vsi_type_str(type));
			return err;
		}

		/* replay filters for the VSI */
		status = ice_replay_vsi(&pf->hw, vsi->idx);
		if (status) {
			dev_err(dev, "replay VSI failed, status %s, VSI index %d, type %s\n",
				ice_stat_str(status), vsi->idx,
				ice_vsi_type_str(type));
			return -EIO;
		}

		/* Re-map HW VSI number, using VSI handle that has been
		 * previously validated in ice_replay_vsi() call above
		 */
		vsi->vsi_num = ice_get_hw_vsi_num(&pf->hw, vsi->idx);

		/* enable the VSI */
		err = ice_ena_vsi(vsi, false);
		if (err) {
			dev_err(dev, "enable VSI failed, err %d, VSI index %d, type %s\n",
				err, vsi->idx, ice_vsi_type_str(type));
			return err;
		}

		dev_info(dev, "VSI rebuilt. VSI index %d, type %s\n", vsi->idx,
			 ice_vsi_type_str(type));
	}
>>>>>>> upstream/android-13

	return 0;
}

/**
<<<<<<< HEAD
 * ice_dis_vsi - pause a VSI
 * @vsi: the VSI being paused
 */
static void ice_dis_vsi(struct ice_vsi *vsi)
{
	if (test_bit(__ICE_DOWN, vsi->state))
		return;

	set_bit(__ICE_NEEDS_RESTART, vsi->state);

	if (vsi->netdev && netif_running(vsi->netdev) &&
	    vsi->type == ICE_VSI_PF)
		vsi->netdev->netdev_ops->ndo_stop(vsi->netdev);

	ice_vsi_close(vsi);
}

/**
 * ice_ena_vsi - resume a VSI
 * @vsi: the VSI being resume
 */
static void ice_ena_vsi(struct ice_vsi *vsi)
{
	if (!test_and_clear_bit(__ICE_NEEDS_RESTART, vsi->state))
		return;

	if (vsi->netdev && netif_running(vsi->netdev))
		vsi->netdev->netdev_ops->ndo_open(vsi->netdev);
	else if (ice_vsi_open(vsi))
		/* this clears the DOWN bit */
		dev_dbg(&vsi->back->pdev->dev, "Failed open VSI 0x%04X on switch 0x%04X\n",
			vsi->vsi_num, vsi->vsw->sw_id);
}

/**
 * ice_pf_dis_all_vsi - Pause all VSIs on a PF
 * @pf: the PF
 */
static void ice_pf_dis_all_vsi(struct ice_pf *pf)
{
	int v;

	ice_for_each_vsi(pf, v)
		if (pf->vsi[v])
			ice_dis_vsi(pf->vsi[v]);
}

/**
 * ice_pf_ena_all_vsi - Resume all VSIs on a PF
 * @pf: the PF
 */
static void ice_pf_ena_all_vsi(struct ice_pf *pf)
{
	int v;

	ice_for_each_vsi(pf, v)
		if (pf->vsi[v])
			ice_ena_vsi(pf->vsi[v]);
=======
 * ice_update_pf_netdev_link - Update PF netdev link status
 * @pf: pointer to the PF instance
 */
static void ice_update_pf_netdev_link(struct ice_pf *pf)
{
	bool link_up;
	int i;

	ice_for_each_vsi(pf, i) {
		struct ice_vsi *vsi = pf->vsi[i];

		if (!vsi || vsi->type != ICE_VSI_PF)
			return;

		ice_get_link_status(pf->vsi[i]->port_info, &link_up);
		if (link_up) {
			netif_carrier_on(pf->vsi[i]->netdev);
			netif_tx_wake_all_queues(pf->vsi[i]->netdev);
		} else {
			netif_carrier_off(pf->vsi[i]->netdev);
			netif_tx_stop_all_queues(pf->vsi[i]->netdev);
		}
	}
>>>>>>> upstream/android-13
}

/**
 * ice_rebuild - rebuild after reset
<<<<<<< HEAD
 * @pf: pf to rebuild
 */
static void ice_rebuild(struct ice_pf *pf)
{
	struct device *dev = &pf->pdev->dev;
=======
 * @pf: PF to rebuild
 * @reset_type: type of reset
 *
 * Do not rebuild VF VSI in this flow because that is already handled via
 * ice_reset_all_vfs(). This is because requirements for resetting a VF after a
 * PFR/CORER/GLOBER/etc. are different than the normal flow. Also, we don't want
 * to reset/rebuild all the VF VSI twice.
 */
static void ice_rebuild(struct ice_pf *pf, enum ice_reset_req reset_type)
{
	struct device *dev = ice_pf_to_dev(pf);
>>>>>>> upstream/android-13
	struct ice_hw *hw = &pf->hw;
	enum ice_status ret;
	int err;

<<<<<<< HEAD
	if (test_bit(__ICE_DOWN, pf->state))
		goto clear_recovery;

	dev_dbg(dev, "rebuilding pf\n");

	ret = ice_init_all_ctrlq(hw);
	if (ret) {
		dev_err(dev, "control queues init failed %d\n", ret);
		goto fail_reset;
=======
	if (test_bit(ICE_DOWN, pf->state))
		goto clear_recovery;

	dev_dbg(dev, "rebuilding PF after reset_type=%d\n", reset_type);

	ret = ice_init_all_ctrlq(hw);
	if (ret) {
		dev_err(dev, "control queues init failed %s\n",
			ice_stat_str(ret));
		goto err_init_ctrlq;
	}

	/* if DDP was previously loaded successfully */
	if (!ice_is_safe_mode(pf)) {
		/* reload the SW DB of filter tables */
		if (reset_type == ICE_RESET_PFR)
			ice_fill_blk_tbls(hw);
		else
			/* Reload DDP Package after CORER/GLOBR reset */
			ice_load_pkg(NULL, pf);
>>>>>>> upstream/android-13
	}

	ret = ice_clear_pf_cfg(hw);
	if (ret) {
<<<<<<< HEAD
		dev_err(dev, "clear PF configuration failed %d\n", ret);
		goto fail_reset;
	}

	ice_clear_pxe_mode(hw);

	ret = ice_get_caps(hw);
	if (ret) {
		dev_err(dev, "ice_get_caps failed %d\n", ret);
		goto fail_reset;
	}

	/* basic nic switch setup */
	err = ice_setup_pf_sw(pf);
	if (err) {
		dev_err(dev, "ice_setup_pf_sw failed\n");
		goto fail_reset;
	}

	/* start misc vector */
	if (test_bit(ICE_FLAG_MSIX_ENA, pf->flags)) {
		err = ice_req_irq_msix_misc(pf);
		if (err) {
			dev_err(dev, "misc vector setup failed: %d\n", err);
			goto fail_reset;
		}
	}

	/* restart the VSIs that were rebuilt and running before the reset */
	ice_pf_ena_all_vsi(pf);

	return;

fail_reset:
	ice_shutdown_all_ctrlq(hw);
	set_bit(__ICE_RESET_FAILED, pf->state);
clear_recovery:
	set_bit(__ICE_RESET_RECOVERY_PENDING, pf->state);
=======
		dev_err(dev, "clear PF configuration failed %s\n",
			ice_stat_str(ret));
		goto err_init_ctrlq;
	}

	if (pf->first_sw->dflt_vsi_ena)
		dev_info(dev, "Clearing default VSI, re-enable after reset completes\n");
	/* clear the default VSI configuration if it exists */
	pf->first_sw->dflt_vsi = NULL;
	pf->first_sw->dflt_vsi_ena = false;

	ice_clear_pxe_mode(hw);

	ret = ice_init_nvm(hw);
	if (ret) {
		dev_err(dev, "ice_init_nvm failed %s\n", ice_stat_str(ret));
		goto err_init_ctrlq;
	}

	ret = ice_get_caps(hw);
	if (ret) {
		dev_err(dev, "ice_get_caps failed %s\n", ice_stat_str(ret));
		goto err_init_ctrlq;
	}

	ret = ice_aq_set_mac_cfg(hw, ICE_AQ_SET_MAC_FRAME_SIZE_MAX, NULL);
	if (ret) {
		dev_err(dev, "set_mac_cfg failed %s\n", ice_stat_str(ret));
		goto err_init_ctrlq;
	}

	err = ice_sched_init_port(hw->port_info);
	if (err)
		goto err_sched_init_port;

	/* start misc vector */
	err = ice_req_irq_msix_misc(pf);
	if (err) {
		dev_err(dev, "misc vector setup failed: %d\n", err);
		goto err_sched_init_port;
	}

	if (test_bit(ICE_FLAG_FD_ENA, pf->flags)) {
		wr32(hw, PFQF_FD_ENA, PFQF_FD_ENA_FD_ENA_M);
		if (!rd32(hw, PFQF_FD_SIZE)) {
			u16 unused, guar, b_effort;

			guar = hw->func_caps.fd_fltr_guar;
			b_effort = hw->func_caps.fd_fltr_best_effort;

			/* force guaranteed filter pool for PF */
			ice_alloc_fd_guar_item(hw, &unused, guar);
			/* force shared filter pool for PF */
			ice_alloc_fd_shrd_item(hw, &unused, b_effort);
		}
	}

	if (test_bit(ICE_FLAG_DCB_ENA, pf->flags))
		ice_dcb_rebuild(pf);

	/* If the PF previously had enabled PTP, PTP init needs to happen before
	 * the VSI rebuild. If not, this causes the PTP link status events to
	 * fail.
	 */
	if (test_bit(ICE_FLAG_PTP_SUPPORTED, pf->flags))
		ice_ptp_init(pf);

	/* rebuild PF VSI */
	err = ice_vsi_rebuild_by_type(pf, ICE_VSI_PF);
	if (err) {
		dev_err(dev, "PF VSI rebuild failed: %d\n", err);
		goto err_vsi_rebuild;
	}

	/* If Flow Director is active */
	if (test_bit(ICE_FLAG_FD_ENA, pf->flags)) {
		err = ice_vsi_rebuild_by_type(pf, ICE_VSI_CTRL);
		if (err) {
			dev_err(dev, "control VSI rebuild failed: %d\n", err);
			goto err_vsi_rebuild;
		}

		/* replay HW Flow Director recipes */
		if (hw->fdir_prof)
			ice_fdir_replay_flows(hw);

		/* replay Flow Director filters */
		ice_fdir_replay_fltrs(pf);

		ice_rebuild_arfs(pf);
	}

	ice_update_pf_netdev_link(pf);

	/* tell the firmware we are up */
	ret = ice_send_version(pf);
	if (ret) {
		dev_err(dev, "Rebuild failed due to error sending driver version: %s\n",
			ice_stat_str(ret));
		goto err_vsi_rebuild;
	}

	ice_replay_post(hw);

	/* if we get here, reset flow is successful */
	clear_bit(ICE_RESET_FAILED, pf->state);

	ice_plug_aux_dev(pf);
	return;

err_vsi_rebuild:
err_sched_init_port:
	ice_sched_cleanup_all(hw);
err_init_ctrlq:
	ice_shutdown_all_ctrlq(hw);
	set_bit(ICE_RESET_FAILED, pf->state);
clear_recovery:
	/* set this bit in PF state to control service task scheduling */
	set_bit(ICE_NEEDS_RESTART, pf->state);
	dev_err(dev, "Rebuild failed, unload and reload driver\n");
}

/**
 * ice_max_xdp_frame_size - returns the maximum allowed frame size for XDP
 * @vsi: Pointer to VSI structure
 */
static int ice_max_xdp_frame_size(struct ice_vsi *vsi)
{
	if (PAGE_SIZE >= 8192 || test_bit(ICE_FLAG_LEGACY_RX, vsi->back->flags))
		return ICE_RXBUF_2048 - XDP_PACKET_HEADROOM;
	else
		return ICE_RXBUF_3072;
>>>>>>> upstream/android-13
}

/**
 * ice_change_mtu - NDO callback to change the MTU
 * @netdev: network interface device structure
 * @new_mtu: new value for maximum frame size
 *
 * Returns 0 on success, negative on failure
 */
static int ice_change_mtu(struct net_device *netdev, int new_mtu)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	struct ice_pf *pf = vsi->back;
	u8 count = 0;
<<<<<<< HEAD

	if (new_mtu == netdev->mtu) {
		netdev_warn(netdev, "mtu is already %u\n", netdev->mtu);
		return 0;
	}

	if (new_mtu < netdev->min_mtu) {
		netdev_err(netdev, "new mtu invalid. min_mtu is %d\n",
			   netdev->min_mtu);
		return -EINVAL;
	} else if (new_mtu > netdev->max_mtu) {
		netdev_err(netdev, "new mtu invalid. max_mtu is %d\n",
			   netdev->min_mtu);
		return -EINVAL;
	}
	/* if a reset is in progress, wait for some time for it to complete */
	do {
		if (ice_is_reset_recovery_pending(pf->state)) {
=======
	int err = 0;

	if (new_mtu == (int)netdev->mtu) {
		netdev_warn(netdev, "MTU is already %u\n", netdev->mtu);
		return 0;
	}

	if (ice_is_xdp_ena_vsi(vsi)) {
		int frame_size = ice_max_xdp_frame_size(vsi);

		if (new_mtu + ICE_ETH_PKT_HDR_PAD > frame_size) {
			netdev_err(netdev, "max MTU for XDP usage is %d\n",
				   frame_size - ICE_ETH_PKT_HDR_PAD);
			return -EINVAL;
		}
	}

	/* if a reset is in progress, wait for some time for it to complete */
	do {
		if (ice_is_reset_in_progress(pf->state)) {
>>>>>>> upstream/android-13
			count++;
			usleep_range(1000, 2000);
		} else {
			break;
		}

	} while (count < 100);

	if (count == 100) {
<<<<<<< HEAD
		netdev_err(netdev, "can't change mtu. Device is busy\n");
		return -EBUSY;
	}

	netdev->mtu = new_mtu;

	/* if VSI is up, bring it down and then back up */
	if (!test_and_set_bit(__ICE_DOWN, vsi->state)) {
		int err;

		err = ice_down(vsi);
		if (err) {
			netdev_err(netdev, "change mtu if_up err %d\n", err);
=======
		netdev_err(netdev, "can't change MTU. Device is busy\n");
		return -EBUSY;
	}

	netdev->mtu = (unsigned int)new_mtu;

	/* if VSI is up, bring it down and then back up */
	if (!test_and_set_bit(ICE_VSI_DOWN, vsi->state)) {
		err = ice_down(vsi);
		if (err) {
			netdev_err(netdev, "change MTU if_down err %d\n", err);
>>>>>>> upstream/android-13
			return err;
		}

		err = ice_up(vsi);
		if (err) {
<<<<<<< HEAD
			netdev_err(netdev, "change mtu if_up err %d\n", err);
=======
			netdev_err(netdev, "change MTU if_up err %d\n", err);
>>>>>>> upstream/android-13
			return err;
		}
	}

<<<<<<< HEAD
	netdev_dbg(netdev, "changed mtu to %d\n", new_mtu);
	return 0;
}

/**
 * ice_set_rss - Set RSS keys and lut
 * @vsi: Pointer to VSI structure
 * @seed: RSS hash seed
=======
	netdev_dbg(netdev, "changed MTU to %d\n", new_mtu);
	set_bit(ICE_FLAG_MTU_CHANGED, pf->flags);

	return err;
}

/**
 * ice_eth_ioctl - Access the hwtstamp interface
 * @netdev: network interface device structure
 * @ifr: interface request data
 * @cmd: ioctl command
 */
static int ice_eth_ioctl(struct net_device *netdev, struct ifreq *ifr, int cmd)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_pf *pf = np->vsi->back;

	switch (cmd) {
	case SIOCGHWTSTAMP:
		return ice_ptp_get_ts_config(pf, ifr);
	case SIOCSHWTSTAMP:
		return ice_ptp_set_ts_config(pf, ifr);
	default:
		return -EOPNOTSUPP;
	}
}

/**
 * ice_aq_str - convert AQ err code to a string
 * @aq_err: the AQ error code to convert
 */
const char *ice_aq_str(enum ice_aq_err aq_err)
{
	switch (aq_err) {
	case ICE_AQ_RC_OK:
		return "OK";
	case ICE_AQ_RC_EPERM:
		return "ICE_AQ_RC_EPERM";
	case ICE_AQ_RC_ENOENT:
		return "ICE_AQ_RC_ENOENT";
	case ICE_AQ_RC_ENOMEM:
		return "ICE_AQ_RC_ENOMEM";
	case ICE_AQ_RC_EBUSY:
		return "ICE_AQ_RC_EBUSY";
	case ICE_AQ_RC_EEXIST:
		return "ICE_AQ_RC_EEXIST";
	case ICE_AQ_RC_EINVAL:
		return "ICE_AQ_RC_EINVAL";
	case ICE_AQ_RC_ENOSPC:
		return "ICE_AQ_RC_ENOSPC";
	case ICE_AQ_RC_ENOSYS:
		return "ICE_AQ_RC_ENOSYS";
	case ICE_AQ_RC_EMODE:
		return "ICE_AQ_RC_EMODE";
	case ICE_AQ_RC_ENOSEC:
		return "ICE_AQ_RC_ENOSEC";
	case ICE_AQ_RC_EBADSIG:
		return "ICE_AQ_RC_EBADSIG";
	case ICE_AQ_RC_ESVN:
		return "ICE_AQ_RC_ESVN";
	case ICE_AQ_RC_EBADMAN:
		return "ICE_AQ_RC_EBADMAN";
	case ICE_AQ_RC_EBADBUF:
		return "ICE_AQ_RC_EBADBUF";
	}

	return "ICE_AQ_RC_UNKNOWN";
}

/**
 * ice_stat_str - convert status err code to a string
 * @stat_err: the status error code to convert
 */
const char *ice_stat_str(enum ice_status stat_err)
{
	switch (stat_err) {
	case ICE_SUCCESS:
		return "OK";
	case ICE_ERR_PARAM:
		return "ICE_ERR_PARAM";
	case ICE_ERR_NOT_IMPL:
		return "ICE_ERR_NOT_IMPL";
	case ICE_ERR_NOT_READY:
		return "ICE_ERR_NOT_READY";
	case ICE_ERR_NOT_SUPPORTED:
		return "ICE_ERR_NOT_SUPPORTED";
	case ICE_ERR_BAD_PTR:
		return "ICE_ERR_BAD_PTR";
	case ICE_ERR_INVAL_SIZE:
		return "ICE_ERR_INVAL_SIZE";
	case ICE_ERR_DEVICE_NOT_SUPPORTED:
		return "ICE_ERR_DEVICE_NOT_SUPPORTED";
	case ICE_ERR_RESET_FAILED:
		return "ICE_ERR_RESET_FAILED";
	case ICE_ERR_FW_API_VER:
		return "ICE_ERR_FW_API_VER";
	case ICE_ERR_NO_MEMORY:
		return "ICE_ERR_NO_MEMORY";
	case ICE_ERR_CFG:
		return "ICE_ERR_CFG";
	case ICE_ERR_OUT_OF_RANGE:
		return "ICE_ERR_OUT_OF_RANGE";
	case ICE_ERR_ALREADY_EXISTS:
		return "ICE_ERR_ALREADY_EXISTS";
	case ICE_ERR_NVM:
		return "ICE_ERR_NVM";
	case ICE_ERR_NVM_CHECKSUM:
		return "ICE_ERR_NVM_CHECKSUM";
	case ICE_ERR_BUF_TOO_SHORT:
		return "ICE_ERR_BUF_TOO_SHORT";
	case ICE_ERR_NVM_BLANK_MODE:
		return "ICE_ERR_NVM_BLANK_MODE";
	case ICE_ERR_IN_USE:
		return "ICE_ERR_IN_USE";
	case ICE_ERR_MAX_LIMIT:
		return "ICE_ERR_MAX_LIMIT";
	case ICE_ERR_RESET_ONGOING:
		return "ICE_ERR_RESET_ONGOING";
	case ICE_ERR_HW_TABLE:
		return "ICE_ERR_HW_TABLE";
	case ICE_ERR_DOES_NOT_EXIST:
		return "ICE_ERR_DOES_NOT_EXIST";
	case ICE_ERR_FW_DDP_MISMATCH:
		return "ICE_ERR_FW_DDP_MISMATCH";
	case ICE_ERR_AQ_ERROR:
		return "ICE_ERR_AQ_ERROR";
	case ICE_ERR_AQ_TIMEOUT:
		return "ICE_ERR_AQ_TIMEOUT";
	case ICE_ERR_AQ_FULL:
		return "ICE_ERR_AQ_FULL";
	case ICE_ERR_AQ_NO_WORK:
		return "ICE_ERR_AQ_NO_WORK";
	case ICE_ERR_AQ_EMPTY:
		return "ICE_ERR_AQ_EMPTY";
	case ICE_ERR_AQ_FW_CRITICAL:
		return "ICE_ERR_AQ_FW_CRITICAL";
	}

	return "ICE_ERR_UNKNOWN";
}

/**
 * ice_set_rss_lut - Set RSS LUT
 * @vsi: Pointer to VSI structure
>>>>>>> upstream/android-13
 * @lut: Lookup table
 * @lut_size: Lookup table size
 *
 * Returns 0 on success, negative on failure
 */
<<<<<<< HEAD
int ice_set_rss(struct ice_vsi *vsi, u8 *seed, u8 *lut, u16 lut_size)
{
	struct ice_pf *pf = vsi->back;
	struct ice_hw *hw = &pf->hw;
	enum ice_status status;

	if (seed) {
		struct ice_aqc_get_set_rss_keys *buf =
				  (struct ice_aqc_get_set_rss_keys *)seed;

		status = ice_aq_set_rss_key(hw, vsi->vsi_num, buf);

		if (status) {
			dev_err(&pf->pdev->dev,
				"Cannot set RSS key, err %d aq_err %d\n",
				status, hw->adminq.rq_last_status);
			return -EIO;
		}
	}

	if (lut) {
		status = ice_aq_set_rss_lut(hw, vsi->vsi_num,
					    vsi->rss_lut_type, lut, lut_size);
		if (status) {
			dev_err(&pf->pdev->dev,
				"Cannot set RSS lut, err %d aq_err %d\n",
				status, hw->adminq.rq_last_status);
			return -EIO;
		}
=======
int ice_set_rss_lut(struct ice_vsi *vsi, u8 *lut, u16 lut_size)
{
	struct ice_aq_get_set_rss_lut_params params = {};
	struct ice_hw *hw = &vsi->back->hw;
	enum ice_status status;

	if (!lut)
		return -EINVAL;

	params.vsi_handle = vsi->idx;
	params.lut_size = lut_size;
	params.lut_type = vsi->rss_lut_type;
	params.lut = lut;

	status = ice_aq_set_rss_lut(hw, &params);
	if (status) {
		dev_err(ice_pf_to_dev(vsi->back), "Cannot set RSS lut, err %s aq_err %s\n",
			ice_stat_str(status),
			ice_aq_str(hw->adminq.sq_last_status));
		return -EIO;
>>>>>>> upstream/android-13
	}

	return 0;
}

/**
<<<<<<< HEAD
 * ice_get_rss - Get RSS keys and lut
 * @vsi: Pointer to VSI structure
 * @seed: Buffer to store the keys
=======
 * ice_set_rss_key - Set RSS key
 * @vsi: Pointer to the VSI structure
 * @seed: RSS hash seed
 *
 * Returns 0 on success, negative on failure
 */
int ice_set_rss_key(struct ice_vsi *vsi, u8 *seed)
{
	struct ice_hw *hw = &vsi->back->hw;
	enum ice_status status;

	if (!seed)
		return -EINVAL;

	status = ice_aq_set_rss_key(hw, vsi->idx, (struct ice_aqc_get_set_rss_keys *)seed);
	if (status) {
		dev_err(ice_pf_to_dev(vsi->back), "Cannot set RSS key, err %s aq_err %s\n",
			ice_stat_str(status),
			ice_aq_str(hw->adminq.sq_last_status));
		return -EIO;
	}

	return 0;
}

/**
 * ice_get_rss_lut - Get RSS LUT
 * @vsi: Pointer to VSI structure
>>>>>>> upstream/android-13
 * @lut: Buffer to store the lookup table entries
 * @lut_size: Size of buffer to store the lookup table entries
 *
 * Returns 0 on success, negative on failure
 */
<<<<<<< HEAD
int ice_get_rss(struct ice_vsi *vsi, u8 *seed, u8 *lut, u16 lut_size)
{
	struct ice_pf *pf = vsi->back;
	struct ice_hw *hw = &pf->hw;
	enum ice_status status;

	if (seed) {
		struct ice_aqc_get_set_rss_keys *buf =
				  (struct ice_aqc_get_set_rss_keys *)seed;

		status = ice_aq_get_rss_key(hw, vsi->vsi_num, buf);
		if (status) {
			dev_err(&pf->pdev->dev,
				"Cannot get RSS key, err %d aq_err %d\n",
				status, hw->adminq.rq_last_status);
			return -EIO;
		}
	}

	if (lut) {
		status = ice_aq_get_rss_lut(hw, vsi->vsi_num,
					    vsi->rss_lut_type, lut, lut_size);
		if (status) {
			dev_err(&pf->pdev->dev,
				"Cannot get RSS lut, err %d aq_err %d\n",
				status, hw->adminq.rq_last_status);
			return -EIO;
		}
=======
int ice_get_rss_lut(struct ice_vsi *vsi, u8 *lut, u16 lut_size)
{
	struct ice_aq_get_set_rss_lut_params params = {};
	struct ice_hw *hw = &vsi->back->hw;
	enum ice_status status;

	if (!lut)
		return -EINVAL;

	params.vsi_handle = vsi->idx;
	params.lut_size = lut_size;
	params.lut_type = vsi->rss_lut_type;
	params.lut = lut;

	status = ice_aq_get_rss_lut(hw, &params);
	if (status) {
		dev_err(ice_pf_to_dev(vsi->back), "Cannot get RSS lut, err %s aq_err %s\n",
			ice_stat_str(status),
			ice_aq_str(hw->adminq.sq_last_status));
		return -EIO;
>>>>>>> upstream/android-13
	}

	return 0;
}

/**
<<<<<<< HEAD
=======
 * ice_get_rss_key - Get RSS key
 * @vsi: Pointer to VSI structure
 * @seed: Buffer to store the key in
 *
 * Returns 0 on success, negative on failure
 */
int ice_get_rss_key(struct ice_vsi *vsi, u8 *seed)
{
	struct ice_hw *hw = &vsi->back->hw;
	enum ice_status status;

	if (!seed)
		return -EINVAL;

	status = ice_aq_get_rss_key(hw, vsi->idx, (struct ice_aqc_get_set_rss_keys *)seed);
	if (status) {
		dev_err(ice_pf_to_dev(vsi->back), "Cannot get RSS key, err %s aq_err %s\n",
			ice_stat_str(status),
			ice_aq_str(hw->adminq.sq_last_status));
		return -EIO;
	}

	return 0;
}

/**
 * ice_bridge_getlink - Get the hardware bridge mode
 * @skb: skb buff
 * @pid: process ID
 * @seq: RTNL message seq
 * @dev: the netdev being configured
 * @filter_mask: filter mask passed in
 * @nlflags: netlink flags passed in
 *
 * Return the bridge mode (VEB/VEPA)
 */
static int
ice_bridge_getlink(struct sk_buff *skb, u32 pid, u32 seq,
		   struct net_device *dev, u32 filter_mask, int nlflags)
{
	struct ice_netdev_priv *np = netdev_priv(dev);
	struct ice_vsi *vsi = np->vsi;
	struct ice_pf *pf = vsi->back;
	u16 bmode;

	bmode = pf->first_sw->bridge_mode;

	return ndo_dflt_bridge_getlink(skb, pid, seq, dev, bmode, 0, 0, nlflags,
				       filter_mask, NULL);
}

/**
 * ice_vsi_update_bridge_mode - Update VSI for switching bridge mode (VEB/VEPA)
 * @vsi: Pointer to VSI structure
 * @bmode: Hardware bridge mode (VEB/VEPA)
 *
 * Returns 0 on success, negative on failure
 */
static int ice_vsi_update_bridge_mode(struct ice_vsi *vsi, u16 bmode)
{
	struct ice_aqc_vsi_props *vsi_props;
	struct ice_hw *hw = &vsi->back->hw;
	struct ice_vsi_ctx *ctxt;
	enum ice_status status;
	int ret = 0;

	vsi_props = &vsi->info;

	ctxt = kzalloc(sizeof(*ctxt), GFP_KERNEL);
	if (!ctxt)
		return -ENOMEM;

	ctxt->info = vsi->info;

	if (bmode == BRIDGE_MODE_VEB)
		/* change from VEPA to VEB mode */
		ctxt->info.sw_flags |= ICE_AQ_VSI_SW_FLAG_ALLOW_LB;
	else
		/* change from VEB to VEPA mode */
		ctxt->info.sw_flags &= ~ICE_AQ_VSI_SW_FLAG_ALLOW_LB;
	ctxt->info.valid_sections = cpu_to_le16(ICE_AQ_VSI_PROP_SW_VALID);

	status = ice_update_vsi(hw, vsi->idx, ctxt, NULL);
	if (status) {
		dev_err(ice_pf_to_dev(vsi->back), "update VSI for bridge mode failed, bmode = %d err %s aq_err %s\n",
			bmode, ice_stat_str(status),
			ice_aq_str(hw->adminq.sq_last_status));
		ret = -EIO;
		goto out;
	}
	/* Update sw flags for book keeping */
	vsi_props->sw_flags = ctxt->info.sw_flags;

out:
	kfree(ctxt);
	return ret;
}

/**
 * ice_bridge_setlink - Set the hardware bridge mode
 * @dev: the netdev being configured
 * @nlh: RTNL message
 * @flags: bridge setlink flags
 * @extack: netlink extended ack
 *
 * Sets the bridge mode (VEB/VEPA) of the switch to which the netdev (VSI) is
 * hooked up to. Iterates through the PF VSI list and sets the loopback mode (if
 * not already set for all VSIs connected to this switch. And also update the
 * unicast switch filter rules for the corresponding switch of the netdev.
 */
static int
ice_bridge_setlink(struct net_device *dev, struct nlmsghdr *nlh,
		   u16 __always_unused flags,
		   struct netlink_ext_ack __always_unused *extack)
{
	struct ice_netdev_priv *np = netdev_priv(dev);
	struct ice_pf *pf = np->vsi->back;
	struct nlattr *attr, *br_spec;
	struct ice_hw *hw = &pf->hw;
	enum ice_status status;
	struct ice_sw *pf_sw;
	int rem, v, err = 0;

	pf_sw = pf->first_sw;
	/* find the attribute in the netlink message */
	br_spec = nlmsg_find_attr(nlh, sizeof(struct ifinfomsg), IFLA_AF_SPEC);

	nla_for_each_nested(attr, br_spec, rem) {
		__u16 mode;

		if (nla_type(attr) != IFLA_BRIDGE_MODE)
			continue;
		mode = nla_get_u16(attr);
		if (mode != BRIDGE_MODE_VEPA && mode != BRIDGE_MODE_VEB)
			return -EINVAL;
		/* Continue  if bridge mode is not being flipped */
		if (mode == pf_sw->bridge_mode)
			continue;
		/* Iterates through the PF VSI list and update the loopback
		 * mode of the VSI
		 */
		ice_for_each_vsi(pf, v) {
			if (!pf->vsi[v])
				continue;
			err = ice_vsi_update_bridge_mode(pf->vsi[v], mode);
			if (err)
				return err;
		}

		hw->evb_veb = (mode == BRIDGE_MODE_VEB);
		/* Update the unicast switch filter rules for the corresponding
		 * switch of the netdev
		 */
		status = ice_update_sw_rule_bridge_mode(hw);
		if (status) {
			netdev_err(dev, "switch rule update failed, mode = %d err %s aq_err %s\n",
				   mode, ice_stat_str(status),
				   ice_aq_str(hw->adminq.sq_last_status));
			/* revert hw->evb_veb */
			hw->evb_veb = (pf_sw->bridge_mode == BRIDGE_MODE_VEB);
			return -EIO;
		}

		pf_sw->bridge_mode = mode;
	}

	return 0;
}

/**
 * ice_tx_timeout - Respond to a Tx Hang
 * @netdev: network interface device structure
 * @txqueue: Tx queue
 */
static void ice_tx_timeout(struct net_device *netdev, unsigned int txqueue)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_ring *tx_ring = NULL;
	struct ice_vsi *vsi = np->vsi;
	struct ice_pf *pf = vsi->back;
	u32 i;

	pf->tx_timeout_count++;

	/* Check if PFC is enabled for the TC to which the queue belongs
	 * to. If yes then Tx timeout is not caused by a hung queue, no
	 * need to reset and rebuild
	 */
	if (ice_is_pfc_causing_hung_q(pf, txqueue)) {
		dev_info(ice_pf_to_dev(pf), "Fake Tx hang detected on queue %u, timeout caused by PFC storm\n",
			 txqueue);
		return;
	}

	/* now that we have an index, find the tx_ring struct */
	for (i = 0; i < vsi->num_txq; i++)
		if (vsi->tx_rings[i] && vsi->tx_rings[i]->desc)
			if (txqueue == vsi->tx_rings[i]->q_index) {
				tx_ring = vsi->tx_rings[i];
				break;
			}

	/* Reset recovery level if enough time has elapsed after last timeout.
	 * Also ensure no new reset action happens before next timeout period.
	 */
	if (time_after(jiffies, (pf->tx_timeout_last_recovery + HZ * 20)))
		pf->tx_timeout_recovery_level = 1;
	else if (time_before(jiffies, (pf->tx_timeout_last_recovery +
				       netdev->watchdog_timeo)))
		return;

	if (tx_ring) {
		struct ice_hw *hw = &pf->hw;
		u32 head, val = 0;

		head = (rd32(hw, QTX_COMM_HEAD(vsi->txq_map[txqueue])) &
			QTX_COMM_HEAD_HEAD_M) >> QTX_COMM_HEAD_HEAD_S;
		/* Read interrupt register */
		val = rd32(hw, GLINT_DYN_CTL(tx_ring->q_vector->reg_idx));

		netdev_info(netdev, "tx_timeout: VSI_num: %d, Q %u, NTC: 0x%x, HW_HEAD: 0x%x, NTU: 0x%x, INT: 0x%x\n",
			    vsi->vsi_num, txqueue, tx_ring->next_to_clean,
			    head, tx_ring->next_to_use, val);
	}

	pf->tx_timeout_last_recovery = jiffies;
	netdev_info(netdev, "tx_timeout recovery level %d, txqueue %u\n",
		    pf->tx_timeout_recovery_level, txqueue);

	switch (pf->tx_timeout_recovery_level) {
	case 1:
		set_bit(ICE_PFR_REQ, pf->state);
		break;
	case 2:
		set_bit(ICE_CORER_REQ, pf->state);
		break;
	case 3:
		set_bit(ICE_GLOBR_REQ, pf->state);
		break;
	default:
		netdev_err(netdev, "tx_timeout recovery unsuccessful, device is in unrecoverable state.\n");
		set_bit(ICE_DOWN, pf->state);
		set_bit(ICE_VSI_NEEDS_RESTART, vsi->state);
		set_bit(ICE_SERVICE_DIS, pf->state);
		break;
	}

	ice_service_task_schedule(pf);
	pf->tx_timeout_recovery_level++;
}

/**
>>>>>>> upstream/android-13
 * ice_open - Called when a network interface becomes active
 * @netdev: network interface device structure
 *
 * The open entry point is called when a network interface is made
<<<<<<< HEAD
 * active by the system (IFF_UP).  At this point all resources needed
=======
 * active by the system (IFF_UP). At this point all resources needed
>>>>>>> upstream/android-13
 * for transmit and receive operations are allocated, the interrupt
 * handler is registered with the OS, the netdev watchdog is enabled,
 * and the stack is notified that the interface is ready.
 *
 * Returns 0 on success, negative value on failure
 */
<<<<<<< HEAD
static int ice_open(struct net_device *netdev)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	int err;

	netif_carrier_off(netdev);

	err = ice_vsi_open(vsi);

	if (err)
		netdev_err(netdev, "Failed to open VSI 0x%04X on switch 0x%04X\n",
			   vsi->vsi_num, vsi->vsw->sw_id);
=======
int ice_open(struct net_device *netdev)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_pf *pf = np->vsi->back;

	if (ice_is_reset_in_progress(pf->state)) {
		netdev_err(netdev, "can't open net device while reset is in progress");
		return -EBUSY;
	}

	return ice_open_internal(netdev);
}

/**
 * ice_open_internal - Called when a network interface becomes active
 * @netdev: network interface device structure
 *
 * Internal ice_open implementation. Should not be used directly except for ice_open and reset
 * handling routine
 *
 * Returns 0 on success, negative value on failure
 */
int ice_open_internal(struct net_device *netdev)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	struct ice_pf *pf = vsi->back;
	struct ice_port_info *pi;
	enum ice_status status;
	int err;

	if (test_bit(ICE_NEEDS_RESTART, pf->state)) {
		netdev_err(netdev, "driver needs to be unloaded and reloaded\n");
		return -EIO;
	}

	netif_carrier_off(netdev);

	pi = vsi->port_info;
	status = ice_update_link_info(pi);
	if (status) {
		netdev_err(netdev, "Failed to get link info, error %s\n",
			   ice_stat_str(status));
		return -EIO;
	}

	ice_check_module_power(pf, pi->phy.link_info.link_cfg_err);

	/* Set PHY if there is media, otherwise, turn off PHY */
	if (pi->phy.link_info.link_info & ICE_AQ_MEDIA_AVAILABLE) {
		clear_bit(ICE_FLAG_NO_MEDIA, pf->flags);
		if (!test_bit(ICE_PHY_INIT_COMPLETE, pf->state)) {
			err = ice_init_phy_user_cfg(pi);
			if (err) {
				netdev_err(netdev, "Failed to initialize PHY settings, error %d\n",
					   err);
				return err;
			}
		}

		err = ice_configure_phy(vsi);
		if (err) {
			netdev_err(netdev, "Failed to set physical link up, error %d\n",
				   err);
			return err;
		}
	} else {
		set_bit(ICE_FLAG_NO_MEDIA, pf->flags);
		ice_set_link(vsi, false);
	}

	err = ice_vsi_open(vsi);
	if (err)
		netdev_err(netdev, "Failed to open VSI 0x%04X on switch 0x%04X\n",
			   vsi->vsi_num, vsi->vsw->sw_id);

	/* Update existing tunnels information */
	udp_tunnel_get_rx_info(netdev);

>>>>>>> upstream/android-13
	return err;
}

/**
 * ice_stop - Disables a network interface
 * @netdev: network interface device structure
 *
 * The stop entry point is called when an interface is de-activated by the OS,
<<<<<<< HEAD
 * and the netdevice enters the DOWN state.  The hardware is still under the
=======
 * and the netdevice enters the DOWN state. The hardware is still under the
>>>>>>> upstream/android-13
 * driver's control, but the netdev interface is disabled.
 *
 * Returns success only - not allowed to fail
 */
<<<<<<< HEAD
static int ice_stop(struct net_device *netdev)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
=======
int ice_stop(struct net_device *netdev)
{
	struct ice_netdev_priv *np = netdev_priv(netdev);
	struct ice_vsi *vsi = np->vsi;
	struct ice_pf *pf = vsi->back;

	if (ice_is_reset_in_progress(pf->state)) {
		netdev_err(netdev, "can't stop net device while reset is in progress");
		return -EBUSY;
	}
>>>>>>> upstream/android-13

	ice_vsi_close(vsi);

	return 0;
}

/**
 * ice_features_check - Validate encapsulated packet conforms to limits
 * @skb: skb buffer
 * @netdev: This port's netdev
 * @features: Offload features that the stack believes apply
 */
static netdev_features_t
ice_features_check(struct sk_buff *skb,
		   struct net_device __always_unused *netdev,
		   netdev_features_t features)
{
<<<<<<< HEAD
	size_t len;

	/* No point in doing any of this if neither checksum nor GSO are
	 * being requested for this frame.  We can rule out both by just
=======
	bool gso = skb_is_gso(skb);
	size_t len;

	/* No point in doing any of this if neither checksum nor GSO are
	 * being requested for this frame. We can rule out both by just
>>>>>>> upstream/android-13
	 * checking for CHECKSUM_PARTIAL
	 */
	if (skb->ip_summed != CHECKSUM_PARTIAL)
		return features;

	/* We cannot support GSO if the MSS is going to be less than
<<<<<<< HEAD
	 * 64 bytes.  If it is then we need to drop support for GSO.
	 */
	if (skb_is_gso(skb) && (skb_shinfo(skb)->gso_size < 64))
		features &= ~NETIF_F_GSO_MASK;

	len = skb_network_header(skb) - skb->data;
	if (len & ~(ICE_TXD_MACLEN_MAX))
		goto out_rm_features;

	len = skb_transport_header(skb) - skb_network_header(skb);
	if (len & ~(ICE_TXD_IPLEN_MAX))
		goto out_rm_features;

	if (skb->encapsulation) {
		len = skb_inner_network_header(skb) - skb_transport_header(skb);
		if (len & ~(ICE_TXD_L4LEN_MAX))
			goto out_rm_features;

		len = skb_inner_transport_header(skb) -
		      skb_inner_network_header(skb);
		if (len & ~(ICE_TXD_IPLEN_MAX))
=======
	 * 64 bytes. If it is then we need to drop support for GSO.
	 */
	if (gso && (skb_shinfo(skb)->gso_size < ICE_TXD_CTX_MIN_MSS))
		features &= ~NETIF_F_GSO_MASK;

	len = skb_network_offset(skb);
	if (len > ICE_TXD_MACLEN_MAX || len & 0x1)
		goto out_rm_features;

	len = skb_network_header_len(skb);
	if (len > ICE_TXD_IPLEN_MAX || len & 0x1)
		goto out_rm_features;

	if (skb->encapsulation) {
		/* this must work for VXLAN frames AND IPIP/SIT frames, and in
		 * the case of IPIP frames, the transport header pointer is
		 * after the inner header! So check to make sure that this
		 * is a GRE or UDP_TUNNEL frame before doing that math.
		 */
		if (gso && (skb_shinfo(skb)->gso_type &
			    (SKB_GSO_GRE | SKB_GSO_UDP_TUNNEL))) {
			len = skb_inner_network_header(skb) -
			      skb_transport_header(skb);
			if (len > ICE_TXD_L4LEN_MAX || len & 0x1)
				goto out_rm_features;
		}

		len = skb_inner_network_header_len(skb);
		if (len > ICE_TXD_IPLEN_MAX || len & 0x1)
>>>>>>> upstream/android-13
			goto out_rm_features;
	}

	return features;
out_rm_features:
	return features & ~(NETIF_F_CSUM_MASK | NETIF_F_GSO_MASK);
}

<<<<<<< HEAD
=======
static const struct net_device_ops ice_netdev_safe_mode_ops = {
	.ndo_open = ice_open,
	.ndo_stop = ice_stop,
	.ndo_start_xmit = ice_start_xmit,
	.ndo_set_mac_address = ice_set_mac_address,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_change_mtu = ice_change_mtu,
	.ndo_get_stats64 = ice_get_stats64,
	.ndo_tx_timeout = ice_tx_timeout,
	.ndo_bpf = ice_xdp_safe_mode,
};

>>>>>>> upstream/android-13
static const struct net_device_ops ice_netdev_ops = {
	.ndo_open = ice_open,
	.ndo_stop = ice_stop,
	.ndo_start_xmit = ice_start_xmit,
	.ndo_features_check = ice_features_check,
	.ndo_set_rx_mode = ice_set_rx_mode,
	.ndo_set_mac_address = ice_set_mac_address,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_change_mtu = ice_change_mtu,
	.ndo_get_stats64 = ice_get_stats64,
<<<<<<< HEAD
	.ndo_vlan_rx_add_vid = ice_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid = ice_vlan_rx_kill_vid,
	.ndo_set_features = ice_set_features,
	.ndo_fdb_add = ice_fdb_add,
	.ndo_fdb_del = ice_fdb_del,
=======
	.ndo_set_tx_maxrate = ice_set_tx_maxrate,
	.ndo_eth_ioctl = ice_eth_ioctl,
	.ndo_set_vf_spoofchk = ice_set_vf_spoofchk,
	.ndo_set_vf_mac = ice_set_vf_mac,
	.ndo_get_vf_config = ice_get_vf_cfg,
	.ndo_set_vf_trust = ice_set_vf_trust,
	.ndo_set_vf_vlan = ice_set_vf_port_vlan,
	.ndo_set_vf_link_state = ice_set_vf_link_state,
	.ndo_get_vf_stats = ice_get_vf_stats,
	.ndo_vlan_rx_add_vid = ice_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid = ice_vlan_rx_kill_vid,
	.ndo_set_features = ice_set_features,
	.ndo_bridge_getlink = ice_bridge_getlink,
	.ndo_bridge_setlink = ice_bridge_setlink,
	.ndo_fdb_add = ice_fdb_add,
	.ndo_fdb_del = ice_fdb_del,
#ifdef CONFIG_RFS_ACCEL
	.ndo_rx_flow_steer = ice_rx_flow_steer,
#endif
	.ndo_tx_timeout = ice_tx_timeout,
	.ndo_bpf = ice_xdp,
	.ndo_xdp_xmit = ice_xdp_xmit,
	.ndo_xsk_wakeup = ice_xsk_wakeup,
>>>>>>> upstream/android-13
};
