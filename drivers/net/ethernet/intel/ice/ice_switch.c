// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2018, Intel Corporation. */

<<<<<<< HEAD
=======
#include "ice_lib.h"
>>>>>>> upstream/android-13
#include "ice_switch.h"

#define ICE_ETH_DA_OFFSET		0
#define ICE_ETH_ETHTYPE_OFFSET		12
#define ICE_ETH_VLAN_TCI_OFFSET		14
#define ICE_MAX_VLAN_ID			0xFFF

/* Dummy ethernet header needed in the ice_aqc_sw_rules_elem
 * struct to configure any switch filter rules.
 * {DA (6 bytes), SA(6 bytes),
 * Ether type (2 bytes for header without VLAN tag) OR
 * VLAN tag (4 bytes for header with VLAN tag) }
 *
 * Word on Hardcoded values
 * byte 0 = 0x2: to identify it as locally administered DA MAC
 * byte 6 = 0x2: to identify it as locally administered SA MAC
 * byte 12 = 0x81 & byte 13 = 0x00:
 *	In case of VLAN filter first two bytes defines ether type (0x8100)
<<<<<<< HEAD
 *	and remaining two bytes are placeholder for programming a given VLAN id
=======
 *	and remaining two bytes are placeholder for programming a given VLAN ID
>>>>>>> upstream/android-13
 *	In case of Ether type filter it is treated as header without VLAN tag
 *	and byte 12 and 13 is used to program a given Ether type instead
 */
#define DUMMY_ETH_HDR_LEN		16
static const u8 dummy_eth_header[DUMMY_ETH_HDR_LEN] = { 0x2, 0, 0, 0, 0, 0,
							0x2, 0, 0, 0, 0, 0,
							0x81, 0, 0, 0};

#define ICE_SW_RULE_RX_TX_ETH_HDR_SIZE \
<<<<<<< HEAD
	(sizeof(struct ice_aqc_sw_rules_elem) - \
	 sizeof(((struct ice_aqc_sw_rules_elem *)0)->pdata) + \
	 sizeof(struct ice_sw_rule_lkup_rx_tx) + DUMMY_ETH_HDR_LEN - 1)
#define ICE_SW_RULE_RX_TX_NO_HDR_SIZE \
	(sizeof(struct ice_aqc_sw_rules_elem) - \
	 sizeof(((struct ice_aqc_sw_rules_elem *)0)->pdata) + \
	 sizeof(struct ice_sw_rule_lkup_rx_tx) - 1)
#define ICE_SW_RULE_LG_ACT_SIZE(n) \
	(sizeof(struct ice_aqc_sw_rules_elem) - \
	 sizeof(((struct ice_aqc_sw_rules_elem *)0)->pdata) + \
	 sizeof(struct ice_sw_rule_lg_act) - \
	 sizeof(((struct ice_sw_rule_lg_act *)0)->act) + \
	 ((n) * sizeof(((struct ice_sw_rule_lg_act *)0)->act)))
#define ICE_SW_RULE_VSI_LIST_SIZE(n) \
	(sizeof(struct ice_aqc_sw_rules_elem) - \
	 sizeof(((struct ice_aqc_sw_rules_elem *)0)->pdata) + \
	 sizeof(struct ice_sw_rule_vsi_list) - \
	 sizeof(((struct ice_sw_rule_vsi_list *)0)->vsi) + \
	 ((n) * sizeof(((struct ice_sw_rule_vsi_list *)0)->vsi)))

/**
 * ice_aq_alloc_free_res - command to allocate/free resources
 * @hw: pointer to the hw struct
 * @num_entries: number of resource entries in buffer
 * @buf: Indirect buffer to hold data parameters and response
 * @buf_size: size of buffer for indirect commands
 * @opc: pass in the command opcode
 * @cd: pointer to command details structure or NULL
 *
 * Helper function to allocate/free resources using the admin queue commands
 */
static enum ice_status
ice_aq_alloc_free_res(struct ice_hw *hw, u16 num_entries,
		      struct ice_aqc_alloc_free_res_elem *buf, u16 buf_size,
		      enum ice_adminq_opc opc, struct ice_sq_cd *cd)
{
	struct ice_aqc_alloc_free_res_cmd *cmd;
	struct ice_aq_desc desc;

	cmd = &desc.params.sw_res_ctrl;

	if (!buf)
		return ICE_ERR_PARAM;

	if (buf_size < (num_entries * sizeof(buf->elem[0])))
		return ICE_ERR_PARAM;

	ice_fill_dflt_direct_cmd_desc(&desc, opc);

	desc.flags |= cpu_to_le16(ICE_AQ_FLAG_RD);

	cmd->num_entries = cpu_to_le16(num_entries);

	return ice_aq_send_cmd(hw, &desc, buf, buf_size, cd);
=======
	(offsetof(struct ice_aqc_sw_rules_elem, pdata.lkup_tx_rx.hdr) + \
	 (DUMMY_ETH_HDR_LEN * \
	  sizeof(((struct ice_sw_rule_lkup_rx_tx *)0)->hdr[0])))
#define ICE_SW_RULE_RX_TX_NO_HDR_SIZE \
	(offsetof(struct ice_aqc_sw_rules_elem, pdata.lkup_tx_rx.hdr))
#define ICE_SW_RULE_LG_ACT_SIZE(n) \
	(offsetof(struct ice_aqc_sw_rules_elem, pdata.lg_act.act) + \
	 ((n) * sizeof(((struct ice_sw_rule_lg_act *)0)->act[0])))
#define ICE_SW_RULE_VSI_LIST_SIZE(n) \
	(offsetof(struct ice_aqc_sw_rules_elem, pdata.vsi_list.vsi) + \
	 ((n) * sizeof(((struct ice_sw_rule_vsi_list *)0)->vsi[0])))

/**
 * ice_init_def_sw_recp - initialize the recipe book keeping tables
 * @hw: pointer to the HW struct
 *
 * Allocate memory for the entire recipe table and initialize the structures/
 * entries corresponding to basic recipes.
 */
enum ice_status ice_init_def_sw_recp(struct ice_hw *hw)
{
	struct ice_sw_recipe *recps;
	u8 i;

	recps = devm_kcalloc(ice_hw_to_dev(hw), ICE_MAX_NUM_RECIPES,
			     sizeof(*recps), GFP_KERNEL);
	if (!recps)
		return ICE_ERR_NO_MEMORY;

	for (i = 0; i < ICE_SW_LKUP_LAST; i++) {
		recps[i].root_rid = i;
		INIT_LIST_HEAD(&recps[i].filt_rules);
		INIT_LIST_HEAD(&recps[i].filt_replay_rules);
		mutex_init(&recps[i].filt_rule_lock);
	}

	hw->switch_info->recp_list = recps;

	return 0;
>>>>>>> upstream/android-13
}

/**
 * ice_aq_get_sw_cfg - get switch configuration
 * @hw: pointer to the hardware structure
 * @buf: pointer to the result buffer
 * @buf_size: length of the buffer available for response
 * @req_desc: pointer to requested descriptor
 * @num_elems: pointer to number of elements
 * @cd: pointer to command details structure or NULL
 *
<<<<<<< HEAD
 * Get switch configuration (0x0200) to be placed in 'buff'.
=======
 * Get switch configuration (0x0200) to be placed in buf.
>>>>>>> upstream/android-13
 * This admin command returns information such as initial VSI/port number
 * and switch ID it belongs to.
 *
 * NOTE: *req_desc is both an input/output parameter.
 * The caller of this function first calls this function with *request_desc set
<<<<<<< HEAD
 * to 0.  If the response from f/w has *req_desc set to 0, all the switch
=======
 * to 0. If the response from f/w has *req_desc set to 0, all the switch
>>>>>>> upstream/android-13
 * configuration information has been returned; if non-zero (meaning not all
 * the information was returned), the caller should call this function again
 * with *req_desc set to the previous value returned by f/w to get the
 * next block of switch configuration information.
 *
 * *num_elems is output only parameter. This reflects the number of elements
 * in response buffer. The caller of this function to use *num_elems while
 * parsing the response buffer.
 */
static enum ice_status
<<<<<<< HEAD
ice_aq_get_sw_cfg(struct ice_hw *hw, struct ice_aqc_get_sw_cfg_resp *buf,
=======
ice_aq_get_sw_cfg(struct ice_hw *hw, struct ice_aqc_get_sw_cfg_resp_elem *buf,
>>>>>>> upstream/android-13
		  u16 buf_size, u16 *req_desc, u16 *num_elems,
		  struct ice_sq_cd *cd)
{
	struct ice_aqc_get_sw_cfg *cmd;
<<<<<<< HEAD
	enum ice_status status;
	struct ice_aq_desc desc;
=======
	struct ice_aq_desc desc;
	enum ice_status status;
>>>>>>> upstream/android-13

	ice_fill_dflt_direct_cmd_desc(&desc, ice_aqc_opc_get_sw_cfg);
	cmd = &desc.params.get_sw_conf;
	cmd->element = cpu_to_le16(*req_desc);

	status = ice_aq_send_cmd(hw, &desc, buf, buf_size, cd);
	if (!status) {
		*req_desc = le16_to_cpu(cmd->element);
		*num_elems = le16_to_cpu(cmd->num_elems);
	}

	return status;
}

/**
 * ice_aq_add_vsi
<<<<<<< HEAD
 * @hw: pointer to the hw struct
=======
 * @hw: pointer to the HW struct
>>>>>>> upstream/android-13
 * @vsi_ctx: pointer to a VSI context struct
 * @cd: pointer to command details structure or NULL
 *
 * Add a VSI context to the hardware (0x0210)
 */
<<<<<<< HEAD
enum ice_status
=======
static enum ice_status
>>>>>>> upstream/android-13
ice_aq_add_vsi(struct ice_hw *hw, struct ice_vsi_ctx *vsi_ctx,
	       struct ice_sq_cd *cd)
{
	struct ice_aqc_add_update_free_vsi_resp *res;
	struct ice_aqc_add_get_update_free_vsi *cmd;
<<<<<<< HEAD
	enum ice_status status;
	struct ice_aq_desc desc;

	cmd = &desc.params.vsi_cmd;
	res = (struct ice_aqc_add_update_free_vsi_resp *)&desc.params.raw;
=======
	struct ice_aq_desc desc;
	enum ice_status status;

	cmd = &desc.params.vsi_cmd;
	res = &desc.params.add_update_free_vsi_res;
>>>>>>> upstream/android-13

	ice_fill_dflt_direct_cmd_desc(&desc, ice_aqc_opc_add_vsi);

	if (!vsi_ctx->alloc_from_pool)
		cmd->vsi_num = cpu_to_le16(vsi_ctx->vsi_num |
					   ICE_AQ_VSI_IS_VALID);
<<<<<<< HEAD
=======
	cmd->vf_id = vsi_ctx->vf_num;
>>>>>>> upstream/android-13

	cmd->vsi_flags = cpu_to_le16(vsi_ctx->flags);

	desc.flags |= cpu_to_le16(ICE_AQ_FLAG_RD);

	status = ice_aq_send_cmd(hw, &desc, &vsi_ctx->info,
				 sizeof(vsi_ctx->info), cd);

	if (!status) {
		vsi_ctx->vsi_num = le16_to_cpu(res->vsi_num) & ICE_AQ_VSI_NUM_M;
		vsi_ctx->vsis_allocd = le16_to_cpu(res->vsi_used);
		vsi_ctx->vsis_unallocated = le16_to_cpu(res->vsi_free);
	}

	return status;
}

/**
<<<<<<< HEAD
 * ice_aq_update_vsi
 * @hw: pointer to the hw struct
=======
 * ice_aq_free_vsi
 * @hw: pointer to the HW struct
 * @vsi_ctx: pointer to a VSI context struct
 * @keep_vsi_alloc: keep VSI allocation as part of this PF's resources
 * @cd: pointer to command details structure or NULL
 *
 * Free VSI context info from hardware (0x0213)
 */
static enum ice_status
ice_aq_free_vsi(struct ice_hw *hw, struct ice_vsi_ctx *vsi_ctx,
		bool keep_vsi_alloc, struct ice_sq_cd *cd)
{
	struct ice_aqc_add_update_free_vsi_resp *resp;
	struct ice_aqc_add_get_update_free_vsi *cmd;
	struct ice_aq_desc desc;
	enum ice_status status;

	cmd = &desc.params.vsi_cmd;
	resp = &desc.params.add_update_free_vsi_res;

	ice_fill_dflt_direct_cmd_desc(&desc, ice_aqc_opc_free_vsi);

	cmd->vsi_num = cpu_to_le16(vsi_ctx->vsi_num | ICE_AQ_VSI_IS_VALID);
	if (keep_vsi_alloc)
		cmd->cmd_flags = cpu_to_le16(ICE_AQ_VSI_KEEP_ALLOC);

	status = ice_aq_send_cmd(hw, &desc, NULL, 0, cd);
	if (!status) {
		vsi_ctx->vsis_allocd = le16_to_cpu(resp->vsi_used);
		vsi_ctx->vsis_unallocated = le16_to_cpu(resp->vsi_free);
	}

	return status;
}

/**
 * ice_aq_update_vsi
 * @hw: pointer to the HW struct
>>>>>>> upstream/android-13
 * @vsi_ctx: pointer to a VSI context struct
 * @cd: pointer to command details structure or NULL
 *
 * Update VSI context in the hardware (0x0211)
 */
<<<<<<< HEAD
enum ice_status
=======
static enum ice_status
>>>>>>> upstream/android-13
ice_aq_update_vsi(struct ice_hw *hw, struct ice_vsi_ctx *vsi_ctx,
		  struct ice_sq_cd *cd)
{
	struct ice_aqc_add_update_free_vsi_resp *resp;
	struct ice_aqc_add_get_update_free_vsi *cmd;
	struct ice_aq_desc desc;
	enum ice_status status;

	cmd = &desc.params.vsi_cmd;
<<<<<<< HEAD
	resp = (struct ice_aqc_add_update_free_vsi_resp *)&desc.params.raw;
=======
	resp = &desc.params.add_update_free_vsi_res;
>>>>>>> upstream/android-13

	ice_fill_dflt_direct_cmd_desc(&desc, ice_aqc_opc_update_vsi);

	cmd->vsi_num = cpu_to_le16(vsi_ctx->vsi_num | ICE_AQ_VSI_IS_VALID);

	desc.flags |= cpu_to_le16(ICE_AQ_FLAG_RD);

	status = ice_aq_send_cmd(hw, &desc, &vsi_ctx->info,
				 sizeof(vsi_ctx->info), cd);

	if (!status) {
		vsi_ctx->vsis_allocd = le16_to_cpu(resp->vsi_used);
		vsi_ctx->vsis_unallocated = le16_to_cpu(resp->vsi_free);
	}

	return status;
}

/**
<<<<<<< HEAD
 * ice_aq_free_vsi
 * @hw: pointer to the hw struct
=======
 * ice_is_vsi_valid - check whether the VSI is valid or not
 * @hw: pointer to the HW struct
 * @vsi_handle: VSI handle
 *
 * check whether the VSI is valid or not
 */
bool ice_is_vsi_valid(struct ice_hw *hw, u16 vsi_handle)
{
	return vsi_handle < ICE_MAX_VSI && hw->vsi_ctx[vsi_handle];
}

/**
 * ice_get_hw_vsi_num - return the HW VSI number
 * @hw: pointer to the HW struct
 * @vsi_handle: VSI handle
 *
 * return the HW VSI number
 * Caution: call this function only if VSI is valid (ice_is_vsi_valid)
 */
u16 ice_get_hw_vsi_num(struct ice_hw *hw, u16 vsi_handle)
{
	return hw->vsi_ctx[vsi_handle]->vsi_num;
}

/**
 * ice_get_vsi_ctx - return the VSI context entry for a given VSI handle
 * @hw: pointer to the HW struct
 * @vsi_handle: VSI handle
 *
 * return the VSI context entry for a given VSI handle
 */
struct ice_vsi_ctx *ice_get_vsi_ctx(struct ice_hw *hw, u16 vsi_handle)
{
	return (vsi_handle >= ICE_MAX_VSI) ? NULL : hw->vsi_ctx[vsi_handle];
}

/**
 * ice_save_vsi_ctx - save the VSI context for a given VSI handle
 * @hw: pointer to the HW struct
 * @vsi_handle: VSI handle
 * @vsi: VSI context pointer
 *
 * save the VSI context entry for a given VSI handle
 */
static void
ice_save_vsi_ctx(struct ice_hw *hw, u16 vsi_handle, struct ice_vsi_ctx *vsi)
{
	hw->vsi_ctx[vsi_handle] = vsi;
}

/**
 * ice_clear_vsi_q_ctx - clear VSI queue contexts for all TCs
 * @hw: pointer to the HW struct
 * @vsi_handle: VSI handle
 */
static void ice_clear_vsi_q_ctx(struct ice_hw *hw, u16 vsi_handle)
{
	struct ice_vsi_ctx *vsi;
	u8 i;

	vsi = ice_get_vsi_ctx(hw, vsi_handle);
	if (!vsi)
		return;
	ice_for_each_traffic_class(i) {
		if (vsi->lan_q_ctx[i]) {
			devm_kfree(ice_hw_to_dev(hw), vsi->lan_q_ctx[i]);
			vsi->lan_q_ctx[i] = NULL;
		}
		if (vsi->rdma_q_ctx[i]) {
			devm_kfree(ice_hw_to_dev(hw), vsi->rdma_q_ctx[i]);
			vsi->rdma_q_ctx[i] = NULL;
		}
	}
}

/**
 * ice_clear_vsi_ctx - clear the VSI context entry
 * @hw: pointer to the HW struct
 * @vsi_handle: VSI handle
 *
 * clear the VSI context entry
 */
static void ice_clear_vsi_ctx(struct ice_hw *hw, u16 vsi_handle)
{
	struct ice_vsi_ctx *vsi;

	vsi = ice_get_vsi_ctx(hw, vsi_handle);
	if (vsi) {
		ice_clear_vsi_q_ctx(hw, vsi_handle);
		devm_kfree(ice_hw_to_dev(hw), vsi);
		hw->vsi_ctx[vsi_handle] = NULL;
	}
}

/**
 * ice_clear_all_vsi_ctx - clear all the VSI context entries
 * @hw: pointer to the HW struct
 */
void ice_clear_all_vsi_ctx(struct ice_hw *hw)
{
	u16 i;

	for (i = 0; i < ICE_MAX_VSI; i++)
		ice_clear_vsi_ctx(hw, i);
}

/**
 * ice_add_vsi - add VSI context to the hardware and VSI handle list
 * @hw: pointer to the HW struct
 * @vsi_handle: unique VSI handle provided by drivers
 * @vsi_ctx: pointer to a VSI context struct
 * @cd: pointer to command details structure or NULL
 *
 * Add a VSI context to the hardware also add it into the VSI handle list.
 * If this function gets called after reset for existing VSIs then update
 * with the new HW VSI number in the corresponding VSI handle list entry.
 */
enum ice_status
ice_add_vsi(struct ice_hw *hw, u16 vsi_handle, struct ice_vsi_ctx *vsi_ctx,
	    struct ice_sq_cd *cd)
{
	struct ice_vsi_ctx *tmp_vsi_ctx;
	enum ice_status status;

	if (vsi_handle >= ICE_MAX_VSI)
		return ICE_ERR_PARAM;
	status = ice_aq_add_vsi(hw, vsi_ctx, cd);
	if (status)
		return status;
	tmp_vsi_ctx = ice_get_vsi_ctx(hw, vsi_handle);
	if (!tmp_vsi_ctx) {
		/* Create a new VSI context */
		tmp_vsi_ctx = devm_kzalloc(ice_hw_to_dev(hw),
					   sizeof(*tmp_vsi_ctx), GFP_KERNEL);
		if (!tmp_vsi_ctx) {
			ice_aq_free_vsi(hw, vsi_ctx, false, cd);
			return ICE_ERR_NO_MEMORY;
		}
		*tmp_vsi_ctx = *vsi_ctx;
		ice_save_vsi_ctx(hw, vsi_handle, tmp_vsi_ctx);
	} else {
		/* update with new HW VSI num */
		tmp_vsi_ctx->vsi_num = vsi_ctx->vsi_num;
	}

	return 0;
}

/**
 * ice_free_vsi- free VSI context from hardware and VSI handle list
 * @hw: pointer to the HW struct
 * @vsi_handle: unique VSI handle
>>>>>>> upstream/android-13
 * @vsi_ctx: pointer to a VSI context struct
 * @keep_vsi_alloc: keep VSI allocation as part of this PF's resources
 * @cd: pointer to command details structure or NULL
 *
<<<<<<< HEAD
 * Get VSI context info from hardware (0x0213)
 */
enum ice_status
ice_aq_free_vsi(struct ice_hw *hw, struct ice_vsi_ctx *vsi_ctx,
		bool keep_vsi_alloc, struct ice_sq_cd *cd)
{
	struct ice_aqc_add_update_free_vsi_resp *resp;
	struct ice_aqc_add_get_update_free_vsi *cmd;
	struct ice_aq_desc desc;
	enum ice_status status;

	cmd = &desc.params.vsi_cmd;
	resp = (struct ice_aqc_add_update_free_vsi_resp *)&desc.params.raw;

	ice_fill_dflt_direct_cmd_desc(&desc, ice_aqc_opc_free_vsi);

	cmd->vsi_num = cpu_to_le16(vsi_ctx->vsi_num | ICE_AQ_VSI_IS_VALID);
	if (keep_vsi_alloc)
		cmd->cmd_flags = cpu_to_le16(ICE_AQ_VSI_KEEP_ALLOC);

	status = ice_aq_send_cmd(hw, &desc, NULL, 0, cd);
	if (!status) {
		vsi_ctx->vsis_allocd = le16_to_cpu(resp->vsi_used);
		vsi_ctx->vsis_unallocated = le16_to_cpu(resp->vsi_free);
	}

=======
 * Free VSI context info from hardware as well as from VSI handle list
 */
enum ice_status
ice_free_vsi(struct ice_hw *hw, u16 vsi_handle, struct ice_vsi_ctx *vsi_ctx,
	     bool keep_vsi_alloc, struct ice_sq_cd *cd)
{
	enum ice_status status;

	if (!ice_is_vsi_valid(hw, vsi_handle))
		return ICE_ERR_PARAM;
	vsi_ctx->vsi_num = ice_get_hw_vsi_num(hw, vsi_handle);
	status = ice_aq_free_vsi(hw, vsi_ctx, keep_vsi_alloc, cd);
	if (!status)
		ice_clear_vsi_ctx(hw, vsi_handle);
>>>>>>> upstream/android-13
	return status;
}

/**
<<<<<<< HEAD
 * ice_aq_alloc_free_vsi_list
 * @hw: pointer to the hw struct
 * @vsi_list_id: VSI list id returned or used for lookup
=======
 * ice_update_vsi
 * @hw: pointer to the HW struct
 * @vsi_handle: unique VSI handle
 * @vsi_ctx: pointer to a VSI context struct
 * @cd: pointer to command details structure or NULL
 *
 * Update VSI context in the hardware
 */
enum ice_status
ice_update_vsi(struct ice_hw *hw, u16 vsi_handle, struct ice_vsi_ctx *vsi_ctx,
	       struct ice_sq_cd *cd)
{
	if (!ice_is_vsi_valid(hw, vsi_handle))
		return ICE_ERR_PARAM;
	vsi_ctx->vsi_num = ice_get_hw_vsi_num(hw, vsi_handle);
	return ice_aq_update_vsi(hw, vsi_ctx, cd);
}

/**
 * ice_cfg_rdma_fltr - enable/disable RDMA filtering on VSI
 * @hw: pointer to HW struct
 * @vsi_handle: VSI SW index
 * @enable: boolean for enable/disable
 */
int
ice_cfg_rdma_fltr(struct ice_hw *hw, u16 vsi_handle, bool enable)
{
	struct ice_vsi_ctx *ctx;

	ctx = ice_get_vsi_ctx(hw, vsi_handle);
	if (!ctx)
		return -EIO;

	if (enable)
		ctx->info.q_opt_flags |= ICE_AQ_VSI_Q_OPT_PE_FLTR_EN;
	else
		ctx->info.q_opt_flags &= ~ICE_AQ_VSI_Q_OPT_PE_FLTR_EN;

	return ice_status_to_errno(ice_update_vsi(hw, vsi_handle, ctx, NULL));
}

/**
 * ice_aq_alloc_free_vsi_list
 * @hw: pointer to the HW struct
 * @vsi_list_id: VSI list ID returned or used for lookup
>>>>>>> upstream/android-13
 * @lkup_type: switch rule filter lookup type
 * @opc: switch rules population command type - pass in the command opcode
 *
 * allocates or free a VSI list resource
 */
static enum ice_status
ice_aq_alloc_free_vsi_list(struct ice_hw *hw, u16 *vsi_list_id,
			   enum ice_sw_lkup_type lkup_type,
			   enum ice_adminq_opc opc)
{
	struct ice_aqc_alloc_free_res_elem *sw_buf;
	struct ice_aqc_res_elem *vsi_ele;
	enum ice_status status;
	u16 buf_len;

<<<<<<< HEAD
	buf_len = sizeof(*sw_buf);
=======
	buf_len = struct_size(sw_buf, elem, 1);
>>>>>>> upstream/android-13
	sw_buf = devm_kzalloc(ice_hw_to_dev(hw), buf_len, GFP_KERNEL);
	if (!sw_buf)
		return ICE_ERR_NO_MEMORY;
	sw_buf->num_elems = cpu_to_le16(1);

	if (lkup_type == ICE_SW_LKUP_MAC ||
	    lkup_type == ICE_SW_LKUP_MAC_VLAN ||
	    lkup_type == ICE_SW_LKUP_ETHERTYPE ||
	    lkup_type == ICE_SW_LKUP_ETHERTYPE_MAC ||
	    lkup_type == ICE_SW_LKUP_PROMISC ||
	    lkup_type == ICE_SW_LKUP_PROMISC_VLAN) {
		sw_buf->res_type = cpu_to_le16(ICE_AQC_RES_TYPE_VSI_LIST_REP);
	} else if (lkup_type == ICE_SW_LKUP_VLAN) {
		sw_buf->res_type =
			cpu_to_le16(ICE_AQC_RES_TYPE_VSI_LIST_PRUNE);
	} else {
		status = ICE_ERR_PARAM;
		goto ice_aq_alloc_free_vsi_list_exit;
	}

	if (opc == ice_aqc_opc_free_res)
		sw_buf->elem[0].e.sw_resp = cpu_to_le16(*vsi_list_id);

	status = ice_aq_alloc_free_res(hw, 1, sw_buf, buf_len, opc, NULL);
	if (status)
		goto ice_aq_alloc_free_vsi_list_exit;

	if (opc == ice_aqc_opc_alloc_res) {
		vsi_ele = &sw_buf->elem[0];
		*vsi_list_id = le16_to_cpu(vsi_ele->e.sw_resp);
	}

ice_aq_alloc_free_vsi_list_exit:
	devm_kfree(ice_hw_to_dev(hw), sw_buf);
	return status;
}

/**
 * ice_aq_sw_rules - add/update/remove switch rules
<<<<<<< HEAD
 * @hw: pointer to the hw struct
=======
 * @hw: pointer to the HW struct
>>>>>>> upstream/android-13
 * @rule_list: pointer to switch rule population list
 * @rule_list_sz: total size of the rule list in bytes
 * @num_rules: number of switch rules in the rule_list
 * @opc: switch rules population command type - pass in the command opcode
 * @cd: pointer to command details structure or NULL
 *
 * Add(0x02a0)/Update(0x02a1)/Remove(0x02a2) switch rules commands to firmware
 */
static enum ice_status
ice_aq_sw_rules(struct ice_hw *hw, void *rule_list, u16 rule_list_sz,
		u8 num_rules, enum ice_adminq_opc opc, struct ice_sq_cd *cd)
{
	struct ice_aq_desc desc;
<<<<<<< HEAD
=======
	enum ice_status status;
>>>>>>> upstream/android-13

	if (opc != ice_aqc_opc_add_sw_rules &&
	    opc != ice_aqc_opc_update_sw_rules &&
	    opc != ice_aqc_opc_remove_sw_rules)
		return ICE_ERR_PARAM;

	ice_fill_dflt_direct_cmd_desc(&desc, opc);

	desc.flags |= cpu_to_le16(ICE_AQ_FLAG_RD);
	desc.params.sw_rules.num_rules_fltr_entry_index =
		cpu_to_le16(num_rules);
<<<<<<< HEAD
	return ice_aq_send_cmd(hw, &desc, rule_list, rule_list_sz, cd);
=======
	status = ice_aq_send_cmd(hw, &desc, rule_list, rule_list_sz, cd);
	if (opc != ice_aqc_opc_add_sw_rules &&
	    hw->adminq.sq_last_status == ICE_AQ_RC_ENOENT)
		status = ICE_ERR_DOES_NOT_EXIST;

	return status;
>>>>>>> upstream/android-13
}

/* ice_init_port_info - Initialize port_info with switch configuration data
 * @pi: pointer to port_info
 * @vsi_port_num: VSI number or port number
 * @type: Type of switch element (port or VSI)
 * @swid: switch ID of the switch the element is attached to
 * @pf_vf_num: PF or VF number
 * @is_vf: true if the element is a VF, false otherwise
 */
static void
ice_init_port_info(struct ice_port_info *pi, u16 vsi_port_num, u8 type,
		   u16 swid, u16 pf_vf_num, bool is_vf)
{
	switch (type) {
	case ICE_AQC_GET_SW_CONF_RESP_PHYS_PORT:
		pi->lport = (u8)(vsi_port_num & ICE_LPORT_MASK);
		pi->sw_id = swid;
		pi->pf_vf_num = pf_vf_num;
		pi->is_vf = is_vf;
		pi->dflt_tx_vsi_num = ICE_DFLT_VSI_INVAL;
		pi->dflt_rx_vsi_num = ICE_DFLT_VSI_INVAL;
		break;
	default:
<<<<<<< HEAD
		ice_debug(pi->hw, ICE_DBG_SW,
			  "incorrect VSI/port type received\n");
=======
		ice_debug(pi->hw, ICE_DBG_SW, "incorrect VSI/port type received\n");
>>>>>>> upstream/android-13
		break;
	}
}

/* ice_get_initial_sw_cfg - Get initial port and default VSI data
 * @hw: pointer to the hardware structure
 */
enum ice_status ice_get_initial_sw_cfg(struct ice_hw *hw)
{
<<<<<<< HEAD
	struct ice_aqc_get_sw_cfg_resp *rbuf;
=======
	struct ice_aqc_get_sw_cfg_resp_elem *rbuf;
>>>>>>> upstream/android-13
	enum ice_status status;
	u16 req_desc = 0;
	u16 num_elems;
	u16 i;

	rbuf = devm_kzalloc(ice_hw_to_dev(hw), ICE_SW_CFG_MAX_BUF_LEN,
			    GFP_KERNEL);

	if (!rbuf)
		return ICE_ERR_NO_MEMORY;

	/* Multiple calls to ice_aq_get_sw_cfg may be required
	 * to get all the switch configuration information. The need
	 * for additional calls is indicated by ice_aq_get_sw_cfg
	 * writing a non-zero value in req_desc
	 */
	do {
<<<<<<< HEAD
=======
		struct ice_aqc_get_sw_cfg_resp_elem *ele;

>>>>>>> upstream/android-13
		status = ice_aq_get_sw_cfg(hw, rbuf, ICE_SW_CFG_MAX_BUF_LEN,
					   &req_desc, &num_elems, NULL);

		if (status)
			break;

<<<<<<< HEAD
		for (i = 0; i < num_elems; i++) {
			struct ice_aqc_get_sw_cfg_resp_elem *ele;
			u16 pf_vf_num, swid, vsi_port_num;
			bool is_vf = false;
			u8 type;

			ele = rbuf[i].elements;
=======
		for (i = 0, ele = rbuf; i < num_elems; i++, ele++) {
			u16 pf_vf_num, swid, vsi_port_num;
			bool is_vf = false;
			u8 res_type;

>>>>>>> upstream/android-13
			vsi_port_num = le16_to_cpu(ele->vsi_port_num) &
				ICE_AQC_GET_SW_CONF_RESP_VSI_PORT_NUM_M;

			pf_vf_num = le16_to_cpu(ele->pf_vf_num) &
				ICE_AQC_GET_SW_CONF_RESP_FUNC_NUM_M;

			swid = le16_to_cpu(ele->swid);

			if (le16_to_cpu(ele->pf_vf_num) &
			    ICE_AQC_GET_SW_CONF_RESP_IS_VF)
				is_vf = true;

<<<<<<< HEAD
			type = le16_to_cpu(ele->vsi_port_num) >>
				ICE_AQC_GET_SW_CONF_RESP_TYPE_S;

			if (type == ICE_AQC_GET_SW_CONF_RESP_VSI) {
=======
			res_type = (u8)(le16_to_cpu(ele->vsi_port_num) >>
					ICE_AQC_GET_SW_CONF_RESP_TYPE_S);

			if (res_type == ICE_AQC_GET_SW_CONF_RESP_VSI) {
>>>>>>> upstream/android-13
				/* FW VSI is not needed. Just continue. */
				continue;
			}

			ice_init_port_info(hw->port_info, vsi_port_num,
<<<<<<< HEAD
					   type, swid, pf_vf_num, is_vf);
		}
	} while (req_desc && !status);

	devm_kfree(ice_hw_to_dev(hw), (void *)rbuf);
=======
					   res_type, swid, pf_vf_num, is_vf);
		}
	} while (req_desc && !status);

	devm_kfree(ice_hw_to_dev(hw), rbuf);
>>>>>>> upstream/android-13
	return status;
}

/**
 * ice_fill_sw_info - Helper function to populate lb_en and lan_en
 * @hw: pointer to the hardware structure
<<<<<<< HEAD
 * @f_info: filter info structure to fill/update
=======
 * @fi: filter info structure to fill/update
>>>>>>> upstream/android-13
 *
 * This helper function populates the lb_en and lan_en elements of the provided
 * ice_fltr_info struct using the switch's type and characteristics of the
 * switch rule being configured.
 */
<<<<<<< HEAD
static void ice_fill_sw_info(struct ice_hw *hw, struct ice_fltr_info *f_info)
{
	f_info->lb_en = false;
	f_info->lan_en = false;
	if ((f_info->flag & ICE_FLTR_TX) &&
	    (f_info->fltr_act == ICE_FWD_TO_VSI ||
	     f_info->fltr_act == ICE_FWD_TO_VSI_LIST ||
	     f_info->fltr_act == ICE_FWD_TO_Q ||
	     f_info->fltr_act == ICE_FWD_TO_QGRP)) {
		f_info->lb_en = true;
		if (!(hw->evb_veb && f_info->lkup_type == ICE_SW_LKUP_MAC &&
		      is_unicast_ether_addr(f_info->l_data.mac.mac_addr)))
			f_info->lan_en = true;
=======
static void ice_fill_sw_info(struct ice_hw *hw, struct ice_fltr_info *fi)
{
	fi->lb_en = false;
	fi->lan_en = false;
	if ((fi->flag & ICE_FLTR_TX) &&
	    (fi->fltr_act == ICE_FWD_TO_VSI ||
	     fi->fltr_act == ICE_FWD_TO_VSI_LIST ||
	     fi->fltr_act == ICE_FWD_TO_Q ||
	     fi->fltr_act == ICE_FWD_TO_QGRP)) {
		/* Setting LB for prune actions will result in replicated
		 * packets to the internal switch that will be dropped.
		 */
		if (fi->lkup_type != ICE_SW_LKUP_VLAN)
			fi->lb_en = true;

		/* Set lan_en to TRUE if
		 * 1. The switch is a VEB AND
		 * 2
		 * 2.1 The lookup is a directional lookup like ethertype,
		 * promiscuous, ethertype-MAC, promiscuous-VLAN
		 * and default-port OR
		 * 2.2 The lookup is VLAN, OR
		 * 2.3 The lookup is MAC with mcast or bcast addr for MAC, OR
		 * 2.4 The lookup is MAC_VLAN with mcast or bcast addr for MAC.
		 *
		 * OR
		 *
		 * The switch is a VEPA.
		 *
		 * In all other cases, the LAN enable has to be set to false.
		 */
		if (hw->evb_veb) {
			if (fi->lkup_type == ICE_SW_LKUP_ETHERTYPE ||
			    fi->lkup_type == ICE_SW_LKUP_PROMISC ||
			    fi->lkup_type == ICE_SW_LKUP_ETHERTYPE_MAC ||
			    fi->lkup_type == ICE_SW_LKUP_PROMISC_VLAN ||
			    fi->lkup_type == ICE_SW_LKUP_DFLT ||
			    fi->lkup_type == ICE_SW_LKUP_VLAN ||
			    (fi->lkup_type == ICE_SW_LKUP_MAC &&
			     !is_unicast_ether_addr(fi->l_data.mac.mac_addr)) ||
			    (fi->lkup_type == ICE_SW_LKUP_MAC_VLAN &&
			     !is_unicast_ether_addr(fi->l_data.mac.mac_addr)))
				fi->lan_en = true;
		} else {
			fi->lan_en = true;
		}
>>>>>>> upstream/android-13
	}
}

/**
 * ice_fill_sw_rule - Helper function to fill switch rule structure
 * @hw: pointer to the hardware structure
 * @f_info: entry containing packet forwarding information
 * @s_rule: switch rule structure to be filled in based on mac_entry
 * @opc: switch rules population command type - pass in the command opcode
 */
static void
ice_fill_sw_rule(struct ice_hw *hw, struct ice_fltr_info *f_info,
		 struct ice_aqc_sw_rules_elem *s_rule, enum ice_adminq_opc opc)
{
	u16 vlan_id = ICE_MAX_VLAN_ID + 1;
<<<<<<< HEAD
	u8 eth_hdr[DUMMY_ETH_HDR_LEN];
	void *daddr = NULL;
=======
	void *daddr = NULL;
	u16 eth_hdr_sz;
	u8 *eth_hdr;
>>>>>>> upstream/android-13
	u32 act = 0;
	__be16 *off;
	u8 q_rgn;

	if (opc == ice_aqc_opc_remove_sw_rules) {
		s_rule->pdata.lkup_tx_rx.act = 0;
		s_rule->pdata.lkup_tx_rx.index =
			cpu_to_le16(f_info->fltr_rule_id);
		s_rule->pdata.lkup_tx_rx.hdr_len = 0;
		return;
	}

<<<<<<< HEAD
	/* initialize the ether header with a dummy header */
	memcpy(eth_hdr, dummy_eth_header, sizeof(dummy_eth_header));
=======
	eth_hdr_sz = sizeof(dummy_eth_header);
	eth_hdr = s_rule->pdata.lkup_tx_rx.hdr;

	/* initialize the ether header with a dummy header */
	memcpy(eth_hdr, dummy_eth_header, eth_hdr_sz);
>>>>>>> upstream/android-13
	ice_fill_sw_info(hw, f_info);

	switch (f_info->fltr_act) {
	case ICE_FWD_TO_VSI:
<<<<<<< HEAD
		act |= (f_info->fwd_id.vsi_id << ICE_SINGLE_ACT_VSI_ID_S) &
=======
		act |= (f_info->fwd_id.hw_vsi_id << ICE_SINGLE_ACT_VSI_ID_S) &
>>>>>>> upstream/android-13
			ICE_SINGLE_ACT_VSI_ID_M;
		if (f_info->lkup_type != ICE_SW_LKUP_VLAN)
			act |= ICE_SINGLE_ACT_VSI_FORWARDING |
				ICE_SINGLE_ACT_VALID_BIT;
		break;
	case ICE_FWD_TO_VSI_LIST:
		act |= ICE_SINGLE_ACT_VSI_LIST;
		act |= (f_info->fwd_id.vsi_list_id <<
			ICE_SINGLE_ACT_VSI_LIST_ID_S) &
			ICE_SINGLE_ACT_VSI_LIST_ID_M;
		if (f_info->lkup_type != ICE_SW_LKUP_VLAN)
			act |= ICE_SINGLE_ACT_VSI_FORWARDING |
				ICE_SINGLE_ACT_VALID_BIT;
		break;
	case ICE_FWD_TO_Q:
		act |= ICE_SINGLE_ACT_TO_Q;
		act |= (f_info->fwd_id.q_id << ICE_SINGLE_ACT_Q_INDEX_S) &
			ICE_SINGLE_ACT_Q_INDEX_M;
		break;
	case ICE_DROP_PACKET:
		act |= ICE_SINGLE_ACT_VSI_FORWARDING | ICE_SINGLE_ACT_DROP |
			ICE_SINGLE_ACT_VALID_BIT;
		break;
	case ICE_FWD_TO_QGRP:
		q_rgn = f_info->qgrp_size > 0 ?
			(u8)ilog2(f_info->qgrp_size) : 0;
		act |= ICE_SINGLE_ACT_TO_Q;
		act |= (f_info->fwd_id.q_id << ICE_SINGLE_ACT_Q_INDEX_S) &
			ICE_SINGLE_ACT_Q_INDEX_M;
		act |= (q_rgn << ICE_SINGLE_ACT_Q_REGION_S) &
			ICE_SINGLE_ACT_Q_REGION_M;
		break;
	default:
		return;
	}

	if (f_info->lb_en)
		act |= ICE_SINGLE_ACT_LB_ENABLE;
	if (f_info->lan_en)
		act |= ICE_SINGLE_ACT_LAN_ENABLE;

	switch (f_info->lkup_type) {
	case ICE_SW_LKUP_MAC:
		daddr = f_info->l_data.mac.mac_addr;
		break;
	case ICE_SW_LKUP_VLAN:
		vlan_id = f_info->l_data.vlan.vlan_id;
		if (f_info->fltr_act == ICE_FWD_TO_VSI ||
		    f_info->fltr_act == ICE_FWD_TO_VSI_LIST) {
			act |= ICE_SINGLE_ACT_PRUNE;
			act |= ICE_SINGLE_ACT_EGRESS | ICE_SINGLE_ACT_INGRESS;
		}
		break;
	case ICE_SW_LKUP_ETHERTYPE_MAC:
		daddr = f_info->l_data.ethertype_mac.mac_addr;
<<<<<<< HEAD
		/* fall-through */
	case ICE_SW_LKUP_ETHERTYPE:
		off = (__be16 *)&eth_hdr[ICE_ETH_ETHTYPE_OFFSET];
=======
		fallthrough;
	case ICE_SW_LKUP_ETHERTYPE:
		off = (__force __be16 *)(eth_hdr + ICE_ETH_ETHTYPE_OFFSET);
>>>>>>> upstream/android-13
		*off = cpu_to_be16(f_info->l_data.ethertype_mac.ethertype);
		break;
	case ICE_SW_LKUP_MAC_VLAN:
		daddr = f_info->l_data.mac_vlan.mac_addr;
		vlan_id = f_info->l_data.mac_vlan.vlan_id;
		break;
	case ICE_SW_LKUP_PROMISC_VLAN:
		vlan_id = f_info->l_data.mac_vlan.vlan_id;
<<<<<<< HEAD
		/* fall-through */
=======
		fallthrough;
>>>>>>> upstream/android-13
	case ICE_SW_LKUP_PROMISC:
		daddr = f_info->l_data.mac_vlan.mac_addr;
		break;
	default:
		break;
	}

	s_rule->type = (f_info->flag & ICE_FLTR_RX) ?
		cpu_to_le16(ICE_AQC_SW_RULES_T_LKUP_RX) :
		cpu_to_le16(ICE_AQC_SW_RULES_T_LKUP_TX);

	/* Recipe set depending on lookup type */
	s_rule->pdata.lkup_tx_rx.recipe_id = cpu_to_le16(f_info->lkup_type);
	s_rule->pdata.lkup_tx_rx.src = cpu_to_le16(f_info->src);
	s_rule->pdata.lkup_tx_rx.act = cpu_to_le32(act);

	if (daddr)
<<<<<<< HEAD
		ether_addr_copy(&eth_hdr[ICE_ETH_DA_OFFSET], daddr);

	if (!(vlan_id > ICE_MAX_VLAN_ID)) {
		off = (__be16 *)&eth_hdr[ICE_ETH_VLAN_TCI_OFFSET];
=======
		ether_addr_copy(eth_hdr + ICE_ETH_DA_OFFSET, daddr);

	if (!(vlan_id > ICE_MAX_VLAN_ID)) {
		off = (__force __be16 *)(eth_hdr + ICE_ETH_VLAN_TCI_OFFSET);
>>>>>>> upstream/android-13
		*off = cpu_to_be16(vlan_id);
	}

	/* Create the switch rule with the final dummy Ethernet header */
	if (opc != ice_aqc_opc_update_sw_rules)
<<<<<<< HEAD
		s_rule->pdata.lkup_tx_rx.hdr_len = cpu_to_le16(sizeof(eth_hdr));

	memcpy(s_rule->pdata.lkup_tx_rx.hdr, eth_hdr, sizeof(eth_hdr));
=======
		s_rule->pdata.lkup_tx_rx.hdr_len = cpu_to_le16(eth_hdr_sz);
>>>>>>> upstream/android-13
}

/**
 * ice_add_marker_act
 * @hw: pointer to the hardware structure
 * @m_ent: the management entry for which sw marker needs to be added
 * @sw_marker: sw marker to tag the Rx descriptor with
<<<<<<< HEAD
 * @l_id: large action resource id
=======
 * @l_id: large action resource ID
>>>>>>> upstream/android-13
 *
 * Create a large action to hold software marker and update the switch rule
 * entry pointed by m_ent with newly created large action
 */
static enum ice_status
ice_add_marker_act(struct ice_hw *hw, struct ice_fltr_mgmt_list_entry *m_ent,
		   u16 sw_marker, u16 l_id)
{
	struct ice_aqc_sw_rules_elem *lg_act, *rx_tx;
	/* For software marker we need 3 large actions
	 * 1. FWD action: FWD TO VSI or VSI LIST
<<<<<<< HEAD
	 * 2. GENERIC VALUE action to hold the profile id
	 * 3. GENERIC VALUE action to hold the software marker id
=======
	 * 2. GENERIC VALUE action to hold the profile ID
	 * 3. GENERIC VALUE action to hold the software marker ID
>>>>>>> upstream/android-13
	 */
	const u16 num_lg_acts = 3;
	enum ice_status status;
	u16 lg_act_size;
	u16 rules_size;
<<<<<<< HEAD
	u16 vsi_info;
	u32 act;
=======
	u32 act;
	u16 id;
>>>>>>> upstream/android-13

	if (m_ent->fltr_info.lkup_type != ICE_SW_LKUP_MAC)
		return ICE_ERR_PARAM;

	/* Create two back-to-back switch rules and submit them to the HW using
	 * one memory buffer:
	 *    1. Large Action
<<<<<<< HEAD
	 *    2. Look up tx rx
=======
	 *    2. Look up Tx Rx
>>>>>>> upstream/android-13
	 */
	lg_act_size = (u16)ICE_SW_RULE_LG_ACT_SIZE(num_lg_acts);
	rules_size = lg_act_size + ICE_SW_RULE_RX_TX_ETH_HDR_SIZE;
	lg_act = devm_kzalloc(ice_hw_to_dev(hw), rules_size, GFP_KERNEL);
	if (!lg_act)
		return ICE_ERR_NO_MEMORY;

	rx_tx = (struct ice_aqc_sw_rules_elem *)((u8 *)lg_act + lg_act_size);

	/* Fill in the first switch rule i.e. large action */
	lg_act->type = cpu_to_le16(ICE_AQC_SW_RULES_T_LG_ACT);
	lg_act->pdata.lg_act.index = cpu_to_le16(l_id);
	lg_act->pdata.lg_act.size = cpu_to_le16(num_lg_acts);

	/* First action VSI forwarding or VSI list forwarding depending on how
	 * many VSIs
	 */
<<<<<<< HEAD
	vsi_info = (m_ent->vsi_count > 1) ?
		m_ent->fltr_info.fwd_id.vsi_list_id :
		m_ent->fltr_info.fwd_id.vsi_id;

	act = ICE_LG_ACT_VSI_FORWARDING | ICE_LG_ACT_VALID_BIT;
	act |= (vsi_info << ICE_LG_ACT_VSI_LIST_ID_S) &
		ICE_LG_ACT_VSI_LIST_ID_M;
=======
	id = (m_ent->vsi_count > 1) ? m_ent->fltr_info.fwd_id.vsi_list_id :
		m_ent->fltr_info.fwd_id.hw_vsi_id;

	act = ICE_LG_ACT_VSI_FORWARDING | ICE_LG_ACT_VALID_BIT;
	act |= (id << ICE_LG_ACT_VSI_LIST_ID_S) & ICE_LG_ACT_VSI_LIST_ID_M;
>>>>>>> upstream/android-13
	if (m_ent->vsi_count > 1)
		act |= ICE_LG_ACT_VSI_LIST;
	lg_act->pdata.lg_act.act[0] = cpu_to_le32(act);

	/* Second action descriptor type */
	act = ICE_LG_ACT_GENERIC;

	act |= (1 << ICE_LG_ACT_GENERIC_VALUE_S) & ICE_LG_ACT_GENERIC_VALUE_M;
	lg_act->pdata.lg_act.act[1] = cpu_to_le32(act);

	act = (ICE_LG_ACT_GENERIC_OFF_RX_DESC_PROF_IDX <<
	       ICE_LG_ACT_GENERIC_OFFSET_S) & ICE_LG_ACT_GENERIC_OFFSET_M;

	/* Third action Marker value */
	act |= ICE_LG_ACT_GENERIC;
	act |= (sw_marker << ICE_LG_ACT_GENERIC_VALUE_S) &
		ICE_LG_ACT_GENERIC_VALUE_M;

	lg_act->pdata.lg_act.act[2] = cpu_to_le32(act);

<<<<<<< HEAD
	/* call the fill switch rule to fill the lookup tx rx structure */
	ice_fill_sw_rule(hw, &m_ent->fltr_info, rx_tx,
			 ice_aqc_opc_update_sw_rules);

	/* Update the action to point to the large action id */
=======
	/* call the fill switch rule to fill the lookup Tx Rx structure */
	ice_fill_sw_rule(hw, &m_ent->fltr_info, rx_tx,
			 ice_aqc_opc_update_sw_rules);

	/* Update the action to point to the large action ID */
>>>>>>> upstream/android-13
	rx_tx->pdata.lkup_tx_rx.act =
		cpu_to_le32(ICE_SINGLE_ACT_PTR |
			    ((l_id << ICE_SINGLE_ACT_PTR_VAL_S) &
			     ICE_SINGLE_ACT_PTR_VAL_M));

<<<<<<< HEAD
	/* Use the filter rule id of the previously created rule with single
=======
	/* Use the filter rule ID of the previously created rule with single
>>>>>>> upstream/android-13
	 * act. Once the update happens, hardware will treat this as large
	 * action
	 */
	rx_tx->pdata.lkup_tx_rx.index =
		cpu_to_le16(m_ent->fltr_info.fltr_rule_id);

	status = ice_aq_sw_rules(hw, lg_act, rules_size, 2,
				 ice_aqc_opc_update_sw_rules, NULL);
	if (!status) {
		m_ent->lg_act_idx = l_id;
		m_ent->sw_marker_id = sw_marker;
	}

	devm_kfree(ice_hw_to_dev(hw), lg_act);
	return status;
}

/**
 * ice_create_vsi_list_map
 * @hw: pointer to the hardware structure
<<<<<<< HEAD
 * @vsi_array: array of VSIs to form a VSI list
 * @num_vsi: num VSI in the array
 * @vsi_list_id: VSI list id generated as part of allocate resource
 *
 * Helper function to create a new entry of VSI list id to VSI mapping
 * using the given VSI list id
 */
static struct ice_vsi_list_map_info *
ice_create_vsi_list_map(struct ice_hw *hw, u16 *vsi_array, u16 num_vsi,
=======
 * @vsi_handle_arr: array of VSI handles to set in the VSI mapping
 * @num_vsi: number of VSI handles in the array
 * @vsi_list_id: VSI list ID generated as part of allocate resource
 *
 * Helper function to create a new entry of VSI list ID to VSI mapping
 * using the given VSI list ID
 */
static struct ice_vsi_list_map_info *
ice_create_vsi_list_map(struct ice_hw *hw, u16 *vsi_handle_arr, u16 num_vsi,
>>>>>>> upstream/android-13
			u16 vsi_list_id)
{
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_vsi_list_map_info *v_map;
	int i;

<<<<<<< HEAD
	v_map = devm_kcalloc(ice_hw_to_dev(hw), 1, sizeof(*v_map), GFP_KERNEL);
=======
	v_map = devm_kzalloc(ice_hw_to_dev(hw), sizeof(*v_map), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!v_map)
		return NULL;

	v_map->vsi_list_id = vsi_list_id;
<<<<<<< HEAD

	for (i = 0; i < num_vsi; i++)
		set_bit(vsi_array[i], v_map->vsi_map);
=======
	v_map->ref_cnt = 1;
	for (i = 0; i < num_vsi; i++)
		set_bit(vsi_handle_arr[i], v_map->vsi_map);
>>>>>>> upstream/android-13

	list_add(&v_map->list_entry, &sw->vsi_list_map_head);
	return v_map;
}

/**
 * ice_update_vsi_list_rule
 * @hw: pointer to the hardware structure
<<<<<<< HEAD
 * @vsi_array: array of VSIs to form a VSI list
 * @num_vsi: num VSI in the array
 * @vsi_list_id: VSI list id generated as part of allocate resource
=======
 * @vsi_handle_arr: array of VSI handles to form a VSI list
 * @num_vsi: number of VSI handles in the array
 * @vsi_list_id: VSI list ID generated as part of allocate resource
>>>>>>> upstream/android-13
 * @remove: Boolean value to indicate if this is a remove action
 * @opc: switch rules population command type - pass in the command opcode
 * @lkup_type: lookup type of the filter
 *
 * Call AQ command to add a new switch rule or update existing switch rule
<<<<<<< HEAD
 * using the given VSI list id
 */
static enum ice_status
ice_update_vsi_list_rule(struct ice_hw *hw, u16 *vsi_array, u16 num_vsi,
=======
 * using the given VSI list ID
 */
static enum ice_status
ice_update_vsi_list_rule(struct ice_hw *hw, u16 *vsi_handle_arr, u16 num_vsi,
>>>>>>> upstream/android-13
			 u16 vsi_list_id, bool remove, enum ice_adminq_opc opc,
			 enum ice_sw_lkup_type lkup_type)
{
	struct ice_aqc_sw_rules_elem *s_rule;
	enum ice_status status;
	u16 s_rule_size;
<<<<<<< HEAD
	u16 type;
=======
	u16 rule_type;
>>>>>>> upstream/android-13
	int i;

	if (!num_vsi)
		return ICE_ERR_PARAM;

	if (lkup_type == ICE_SW_LKUP_MAC ||
	    lkup_type == ICE_SW_LKUP_MAC_VLAN ||
	    lkup_type == ICE_SW_LKUP_ETHERTYPE ||
	    lkup_type == ICE_SW_LKUP_ETHERTYPE_MAC ||
	    lkup_type == ICE_SW_LKUP_PROMISC ||
	    lkup_type == ICE_SW_LKUP_PROMISC_VLAN)
<<<<<<< HEAD
		type = remove ? ICE_AQC_SW_RULES_T_VSI_LIST_CLEAR :
				ICE_AQC_SW_RULES_T_VSI_LIST_SET;
	else if (lkup_type == ICE_SW_LKUP_VLAN)
		type = remove ? ICE_AQC_SW_RULES_T_PRUNE_LIST_CLEAR :
				ICE_AQC_SW_RULES_T_PRUNE_LIST_SET;
=======
		rule_type = remove ? ICE_AQC_SW_RULES_T_VSI_LIST_CLEAR :
			ICE_AQC_SW_RULES_T_VSI_LIST_SET;
	else if (lkup_type == ICE_SW_LKUP_VLAN)
		rule_type = remove ? ICE_AQC_SW_RULES_T_PRUNE_LIST_CLEAR :
			ICE_AQC_SW_RULES_T_PRUNE_LIST_SET;
>>>>>>> upstream/android-13
	else
		return ICE_ERR_PARAM;

	s_rule_size = (u16)ICE_SW_RULE_VSI_LIST_SIZE(num_vsi);
	s_rule = devm_kzalloc(ice_hw_to_dev(hw), s_rule_size, GFP_KERNEL);
	if (!s_rule)
		return ICE_ERR_NO_MEMORY;
<<<<<<< HEAD

	for (i = 0; i < num_vsi; i++)
		s_rule->pdata.vsi_list.vsi[i] = cpu_to_le16(vsi_array[i]);

	s_rule->type = cpu_to_le16(type);
=======
	for (i = 0; i < num_vsi; i++) {
		if (!ice_is_vsi_valid(hw, vsi_handle_arr[i])) {
			status = ICE_ERR_PARAM;
			goto exit;
		}
		/* AQ call requires hw_vsi_id(s) */
		s_rule->pdata.vsi_list.vsi[i] =
			cpu_to_le16(ice_get_hw_vsi_num(hw, vsi_handle_arr[i]));
	}

	s_rule->type = cpu_to_le16(rule_type);
>>>>>>> upstream/android-13
	s_rule->pdata.vsi_list.number_vsi = cpu_to_le16(num_vsi);
	s_rule->pdata.vsi_list.index = cpu_to_le16(vsi_list_id);

	status = ice_aq_sw_rules(hw, s_rule, s_rule_size, 1, opc, NULL);

<<<<<<< HEAD
=======
exit:
>>>>>>> upstream/android-13
	devm_kfree(ice_hw_to_dev(hw), s_rule);
	return status;
}

/**
 * ice_create_vsi_list_rule - Creates and populates a VSI list rule
<<<<<<< HEAD
 * @hw: pointer to the hw struct
 * @vsi_array: array of VSIs to form a VSI list
 * @num_vsi: number of VSIs in the array
=======
 * @hw: pointer to the HW struct
 * @vsi_handle_arr: array of VSI handles to form a VSI list
 * @num_vsi: number of VSI handles in the array
>>>>>>> upstream/android-13
 * @vsi_list_id: stores the ID of the VSI list to be created
 * @lkup_type: switch rule filter's lookup type
 */
static enum ice_status
<<<<<<< HEAD
ice_create_vsi_list_rule(struct ice_hw *hw, u16 *vsi_array, u16 num_vsi,
			 u16 *vsi_list_id, enum ice_sw_lkup_type lkup_type)
{
	enum ice_status status;
	int i;

	for (i = 0; i < num_vsi; i++)
		if (vsi_array[i] >= ICE_MAX_VSI)
			return ICE_ERR_OUT_OF_RANGE;
=======
ice_create_vsi_list_rule(struct ice_hw *hw, u16 *vsi_handle_arr, u16 num_vsi,
			 u16 *vsi_list_id, enum ice_sw_lkup_type lkup_type)
{
	enum ice_status status;
>>>>>>> upstream/android-13

	status = ice_aq_alloc_free_vsi_list(hw, vsi_list_id, lkup_type,
					    ice_aqc_opc_alloc_res);
	if (status)
		return status;

	/* Update the newly created VSI list to include the specified VSIs */
<<<<<<< HEAD
	return ice_update_vsi_list_rule(hw, vsi_array, num_vsi, *vsi_list_id,
					false, ice_aqc_opc_add_sw_rules,
					lkup_type);
=======
	return ice_update_vsi_list_rule(hw, vsi_handle_arr, num_vsi,
					*vsi_list_id, false,
					ice_aqc_opc_add_sw_rules, lkup_type);
>>>>>>> upstream/android-13
}

/**
 * ice_create_pkt_fwd_rule
 * @hw: pointer to the hardware structure
 * @f_entry: entry containing packet forwarding information
 *
 * Create switch rule with given filter information and add an entry
 * to the corresponding filter management list to track this switch rule
 * and VSI mapping
 */
static enum ice_status
ice_create_pkt_fwd_rule(struct ice_hw *hw,
			struct ice_fltr_list_entry *f_entry)
{
<<<<<<< HEAD
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_mgmt_list_entry *fm_entry;
	struct ice_aqc_sw_rules_elem *s_rule;
	enum ice_sw_lkup_type l_type;
=======
	struct ice_fltr_mgmt_list_entry *fm_entry;
	struct ice_aqc_sw_rules_elem *s_rule;
	enum ice_sw_lkup_type l_type;
	struct ice_sw_recipe *recp;
>>>>>>> upstream/android-13
	enum ice_status status;

	s_rule = devm_kzalloc(ice_hw_to_dev(hw),
			      ICE_SW_RULE_RX_TX_ETH_HDR_SIZE, GFP_KERNEL);
	if (!s_rule)
		return ICE_ERR_NO_MEMORY;
	fm_entry = devm_kzalloc(ice_hw_to_dev(hw), sizeof(*fm_entry),
				GFP_KERNEL);
	if (!fm_entry) {
		status = ICE_ERR_NO_MEMORY;
		goto ice_create_pkt_fwd_rule_exit;
	}

	fm_entry->fltr_info = f_entry->fltr_info;

	/* Initialize all the fields for the management entry */
	fm_entry->vsi_count = 1;
	fm_entry->lg_act_idx = ICE_INVAL_LG_ACT_INDEX;
	fm_entry->sw_marker_id = ICE_INVAL_SW_MARKER_ID;
	fm_entry->counter_index = ICE_INVAL_COUNTER_ID;

	ice_fill_sw_rule(hw, &fm_entry->fltr_info, s_rule,
			 ice_aqc_opc_add_sw_rules);

	status = ice_aq_sw_rules(hw, s_rule, ICE_SW_RULE_RX_TX_ETH_HDR_SIZE, 1,
				 ice_aqc_opc_add_sw_rules, NULL);
	if (status) {
		devm_kfree(ice_hw_to_dev(hw), fm_entry);
		goto ice_create_pkt_fwd_rule_exit;
	}

	f_entry->fltr_info.fltr_rule_id =
		le16_to_cpu(s_rule->pdata.lkup_tx_rx.index);
	fm_entry->fltr_info.fltr_rule_id =
		le16_to_cpu(s_rule->pdata.lkup_tx_rx.index);

	/* The book keeping entries will get removed when base driver
	 * calls remove filter AQ command
	 */
	l_type = fm_entry->fltr_info.lkup_type;
<<<<<<< HEAD
	if (l_type == ICE_SW_LKUP_MAC) {
		mutex_lock(&sw->mac_list_lock);
		list_add(&fm_entry->list_entry, &sw->mac_list_head);
		mutex_unlock(&sw->mac_list_lock);
	} else if (l_type == ICE_SW_LKUP_VLAN) {
		mutex_lock(&sw->vlan_list_lock);
		list_add(&fm_entry->list_entry, &sw->vlan_list_head);
		mutex_unlock(&sw->vlan_list_lock);
	} else if (l_type == ICE_SW_LKUP_ETHERTYPE ||
		   l_type == ICE_SW_LKUP_ETHERTYPE_MAC) {
		mutex_lock(&sw->eth_m_list_lock);
		list_add(&fm_entry->list_entry, &sw->eth_m_list_head);
		mutex_unlock(&sw->eth_m_list_lock);
	} else if (l_type == ICE_SW_LKUP_PROMISC ||
		   l_type == ICE_SW_LKUP_PROMISC_VLAN) {
		mutex_lock(&sw->promisc_list_lock);
		list_add(&fm_entry->list_entry, &sw->promisc_list_head);
		mutex_unlock(&sw->promisc_list_lock);
	} else if (fm_entry->fltr_info.lkup_type == ICE_SW_LKUP_MAC_VLAN) {
		mutex_lock(&sw->mac_vlan_list_lock);
		list_add(&fm_entry->list_entry, &sw->mac_vlan_list_head);
		mutex_unlock(&sw->mac_vlan_list_lock);
	} else {
		status = ICE_ERR_NOT_IMPL;
	}
=======
	recp = &hw->switch_info->recp_list[l_type];
	list_add(&fm_entry->list_entry, &recp->filt_rules);

>>>>>>> upstream/android-13
ice_create_pkt_fwd_rule_exit:
	devm_kfree(ice_hw_to_dev(hw), s_rule);
	return status;
}

/**
 * ice_update_pkt_fwd_rule
 * @hw: pointer to the hardware structure
<<<<<<< HEAD
 * @rule_id: rule of previously created switch rule to update
 * @vsi_list_id: VSI list id to be updated with
 * @f_info: ice_fltr_info to pull other information for switch rule
 *
 * Call AQ command to update a previously created switch rule with a
 * VSI list id
 */
static enum ice_status
ice_update_pkt_fwd_rule(struct ice_hw *hw, u16 rule_id, u16 vsi_list_id,
			struct ice_fltr_info f_info)
{
	struct ice_aqc_sw_rules_elem *s_rule;
	struct ice_fltr_info tmp_fltr;
=======
 * @f_info: filter information for switch rule
 *
 * Call AQ command to update a previously created switch rule with a
 * VSI list ID
 */
static enum ice_status
ice_update_pkt_fwd_rule(struct ice_hw *hw, struct ice_fltr_info *f_info)
{
	struct ice_aqc_sw_rules_elem *s_rule;
>>>>>>> upstream/android-13
	enum ice_status status;

	s_rule = devm_kzalloc(ice_hw_to_dev(hw),
			      ICE_SW_RULE_RX_TX_ETH_HDR_SIZE, GFP_KERNEL);
	if (!s_rule)
		return ICE_ERR_NO_MEMORY;

<<<<<<< HEAD
	tmp_fltr = f_info;
	tmp_fltr.fltr_act = ICE_FWD_TO_VSI_LIST;
	tmp_fltr.fwd_id.vsi_list_id = vsi_list_id;

	ice_fill_sw_rule(hw, &tmp_fltr, s_rule,
			 ice_aqc_opc_update_sw_rules);

	s_rule->pdata.lkup_tx_rx.index = cpu_to_le16(rule_id);
=======
	ice_fill_sw_rule(hw, f_info, s_rule, ice_aqc_opc_update_sw_rules);

	s_rule->pdata.lkup_tx_rx.index = cpu_to_le16(f_info->fltr_rule_id);
>>>>>>> upstream/android-13

	/* Update switch rule with new rule set to forward VSI list */
	status = ice_aq_sw_rules(hw, s_rule, ICE_SW_RULE_RX_TX_ETH_HDR_SIZE, 1,
				 ice_aqc_opc_update_sw_rules, NULL);

	devm_kfree(ice_hw_to_dev(hw), s_rule);
	return status;
}

/**
<<<<<<< HEAD
 * ice_handle_vsi_list_mgmt
=======
 * ice_update_sw_rule_bridge_mode
 * @hw: pointer to the HW struct
 *
 * Updates unicast switch filter rules based on VEB/VEPA mode
 */
enum ice_status ice_update_sw_rule_bridge_mode(struct ice_hw *hw)
{
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_mgmt_list_entry *fm_entry;
	enum ice_status status = 0;
	struct list_head *rule_head;
	struct mutex *rule_lock; /* Lock to protect filter rule list */

	rule_lock = &sw->recp_list[ICE_SW_LKUP_MAC].filt_rule_lock;
	rule_head = &sw->recp_list[ICE_SW_LKUP_MAC].filt_rules;

	mutex_lock(rule_lock);
	list_for_each_entry(fm_entry, rule_head, list_entry) {
		struct ice_fltr_info *fi = &fm_entry->fltr_info;
		u8 *addr = fi->l_data.mac.mac_addr;

		/* Update unicast Tx rules to reflect the selected
		 * VEB/VEPA mode
		 */
		if ((fi->flag & ICE_FLTR_TX) && is_unicast_ether_addr(addr) &&
		    (fi->fltr_act == ICE_FWD_TO_VSI ||
		     fi->fltr_act == ICE_FWD_TO_VSI_LIST ||
		     fi->fltr_act == ICE_FWD_TO_Q ||
		     fi->fltr_act == ICE_FWD_TO_QGRP)) {
			status = ice_update_pkt_fwd_rule(hw, fi);
			if (status)
				break;
		}
	}

	mutex_unlock(rule_lock);

	return status;
}

/**
 * ice_add_update_vsi_list
>>>>>>> upstream/android-13
 * @hw: pointer to the hardware structure
 * @m_entry: pointer to current filter management list entry
 * @cur_fltr: filter information from the book keeping entry
 * @new_fltr: filter information with the new VSI to be added
 *
 * Call AQ command to add or update previously created VSI list with new VSI.
 *
 * Helper function to do book keeping associated with adding filter information
<<<<<<< HEAD
 * The algorithm to do the booking keeping is described below :
 * When a VSI needs to subscribe to a given filter( MAC/VLAN/Ethtype etc.)
=======
 * The algorithm to do the book keeping is described below :
 * When a VSI needs to subscribe to a given filter (MAC/VLAN/Ethtype etc.)
>>>>>>> upstream/android-13
 *	if only one VSI has been added till now
 *		Allocate a new VSI list and add two VSIs
 *		to this list using switch rule command
 *		Update the previously created switch rule with the
<<<<<<< HEAD
 *		newly created VSI list id
=======
 *		newly created VSI list ID
>>>>>>> upstream/android-13
 *	if a VSI list was previously created
 *		Add the new VSI to the previously created VSI list set
 *		using the update switch rule command
 */
static enum ice_status
<<<<<<< HEAD
ice_handle_vsi_list_mgmt(struct ice_hw *hw,
			 struct ice_fltr_mgmt_list_entry *m_entry,
			 struct ice_fltr_info *cur_fltr,
			 struct ice_fltr_info *new_fltr)
=======
ice_add_update_vsi_list(struct ice_hw *hw,
			struct ice_fltr_mgmt_list_entry *m_entry,
			struct ice_fltr_info *cur_fltr,
			struct ice_fltr_info *new_fltr)
>>>>>>> upstream/android-13
{
	enum ice_status status = 0;
	u16 vsi_list_id = 0;

	if ((cur_fltr->fltr_act == ICE_FWD_TO_Q ||
	     cur_fltr->fltr_act == ICE_FWD_TO_QGRP))
		return ICE_ERR_NOT_IMPL;

	if ((new_fltr->fltr_act == ICE_FWD_TO_Q ||
	     new_fltr->fltr_act == ICE_FWD_TO_QGRP) &&
	    (cur_fltr->fltr_act == ICE_FWD_TO_VSI ||
	     cur_fltr->fltr_act == ICE_FWD_TO_VSI_LIST))
		return ICE_ERR_NOT_IMPL;

	if (m_entry->vsi_count < 2 && !m_entry->vsi_list_info) {
		/* Only one entry existed in the mapping and it was not already
		 * a part of a VSI list. So, create a VSI list with the old and
		 * new VSIs.
		 */
<<<<<<< HEAD
		u16 vsi_id_arr[2];
		u16 fltr_rule;

		/* A rule already exists with the new VSI being added */
		if (cur_fltr->fwd_id.vsi_id == new_fltr->fwd_id.vsi_id)
			return ICE_ERR_ALREADY_EXISTS;

		vsi_id_arr[0] = cur_fltr->fwd_id.vsi_id;
		vsi_id_arr[1] = new_fltr->fwd_id.vsi_id;
		status = ice_create_vsi_list_rule(hw, &vsi_id_arr[0], 2,
=======
		struct ice_fltr_info tmp_fltr;
		u16 vsi_handle_arr[2];

		/* A rule already exists with the new VSI being added */
		if (cur_fltr->fwd_id.hw_vsi_id == new_fltr->fwd_id.hw_vsi_id)
			return ICE_ERR_ALREADY_EXISTS;

		vsi_handle_arr[0] = cur_fltr->vsi_handle;
		vsi_handle_arr[1] = new_fltr->vsi_handle;
		status = ice_create_vsi_list_rule(hw, &vsi_handle_arr[0], 2,
>>>>>>> upstream/android-13
						  &vsi_list_id,
						  new_fltr->lkup_type);
		if (status)
			return status;

<<<<<<< HEAD
		fltr_rule = cur_fltr->fltr_rule_id;
		/* Update the previous switch rule of "MAC forward to VSI" to
		 * "MAC fwd to VSI list"
		 */
		status = ice_update_pkt_fwd_rule(hw, fltr_rule, vsi_list_id,
						 *new_fltr);
=======
		tmp_fltr = *new_fltr;
		tmp_fltr.fltr_rule_id = cur_fltr->fltr_rule_id;
		tmp_fltr.fltr_act = ICE_FWD_TO_VSI_LIST;
		tmp_fltr.fwd_id.vsi_list_id = vsi_list_id;
		/* Update the previous switch rule of "MAC forward to VSI" to
		 * "MAC fwd to VSI list"
		 */
		status = ice_update_pkt_fwd_rule(hw, &tmp_fltr);
>>>>>>> upstream/android-13
		if (status)
			return status;

		cur_fltr->fwd_id.vsi_list_id = vsi_list_id;
		cur_fltr->fltr_act = ICE_FWD_TO_VSI_LIST;
		m_entry->vsi_list_info =
<<<<<<< HEAD
			ice_create_vsi_list_map(hw, &vsi_id_arr[0], 2,
						vsi_list_id);

=======
			ice_create_vsi_list_map(hw, &vsi_handle_arr[0], 2,
						vsi_list_id);

		if (!m_entry->vsi_list_info)
			return ICE_ERR_NO_MEMORY;

>>>>>>> upstream/android-13
		/* If this entry was large action then the large action needs
		 * to be updated to point to FWD to VSI list
		 */
		if (m_entry->sw_marker_id != ICE_INVAL_SW_MARKER_ID)
			status =
			    ice_add_marker_act(hw, m_entry,
					       m_entry->sw_marker_id,
					       m_entry->lg_act_idx);
	} else {
<<<<<<< HEAD
		u16 vsi_id = new_fltr->fwd_id.vsi_id;
=======
		u16 vsi_handle = new_fltr->vsi_handle;
>>>>>>> upstream/android-13
		enum ice_adminq_opc opcode;

		if (!m_entry->vsi_list_info)
			return ICE_ERR_CFG;

		/* A rule already exists with the new VSI being added */
<<<<<<< HEAD
		if (test_bit(vsi_id, m_entry->vsi_list_info->vsi_map))
			return 0;

		/* Update the previously created VSI list set with
		 * the new VSI id passed in
=======
		if (test_bit(vsi_handle, m_entry->vsi_list_info->vsi_map))
			return 0;

		/* Update the previously created VSI list set with
		 * the new VSI ID passed in
>>>>>>> upstream/android-13
		 */
		vsi_list_id = cur_fltr->fwd_id.vsi_list_id;
		opcode = ice_aqc_opc_update_sw_rules;

<<<<<<< HEAD
		status = ice_update_vsi_list_rule(hw, &vsi_id, 1, vsi_list_id,
						  false, opcode,
						  new_fltr->lkup_type);
		/* update VSI list mapping info with new VSI id */
		if (!status)
			set_bit(vsi_id, m_entry->vsi_list_info->vsi_map);
=======
		status = ice_update_vsi_list_rule(hw, &vsi_handle, 1,
						  vsi_list_id, false, opcode,
						  new_fltr->lkup_type);
		/* update VSI list mapping info with new VSI ID */
		if (!status)
			set_bit(vsi_handle, m_entry->vsi_list_info->vsi_map);
>>>>>>> upstream/android-13
	}
	if (!status)
		m_entry->vsi_count++;
	return status;
}

/**
<<<<<<< HEAD
 * ice_find_mac_entry
 * @hw: pointer to the hardware structure
 * @mac_addr: MAC address to search for
 *
 * Helper function to search for a MAC entry using a given MAC address
 * Returns pointer to the entry if found.
 */
static struct ice_fltr_mgmt_list_entry *
ice_find_mac_entry(struct ice_hw *hw, u8 *mac_addr)
{
	struct ice_fltr_mgmt_list_entry *m_list_itr, *mac_ret = NULL;
	struct ice_switch_info *sw = hw->switch_info;

	mutex_lock(&sw->mac_list_lock);
	list_for_each_entry(m_list_itr, &sw->mac_list_head, list_entry) {
		u8 *buf = &m_list_itr->fltr_info.l_data.mac.mac_addr[0];

		if (ether_addr_equal(buf, mac_addr)) {
			mac_ret = m_list_itr;
			break;
		}
	}
	mutex_unlock(&sw->mac_list_lock);
	return mac_ret;
}

/**
 * ice_add_shared_mac - Add one MAC shared filter rule
 * @hw: pointer to the hardware structure
 * @f_entry: structure containing MAC forwarding information
 *
 * Adds or updates the book keeping list for the MAC addresses
 */
static enum ice_status
ice_add_shared_mac(struct ice_hw *hw, struct ice_fltr_list_entry *f_entry)
{
	struct ice_fltr_info *new_fltr, *cur_fltr;
	struct ice_fltr_mgmt_list_entry *m_entry;

	new_fltr = &f_entry->fltr_info;

	m_entry = ice_find_mac_entry(hw, &new_fltr->l_data.mac.mac_addr[0]);
	if (!m_entry)
		return ice_create_pkt_fwd_rule(hw, f_entry);

	cur_fltr = &m_entry->fltr_info;

	return ice_handle_vsi_list_mgmt(hw, m_entry, cur_fltr, new_fltr);
}

/**
 * ice_add_mac - Add a MAC address based filter rule
 * @hw: pointer to the hardware structure
 * @m_list: list of MAC addresses and forwarding information
 *
 * IMPORTANT: When the ucast_shared flag is set to false and m_list has
 * multiple unicast addresses, the function assumes that all the
 * addresses are unique in a given add_mac call. It doesn't
 * check for duplicates in this case, removing duplicates from a given
 * list should be taken care of in the caller of this function.
 */
enum ice_status
ice_add_mac(struct ice_hw *hw, struct list_head *m_list)
{
	struct ice_aqc_sw_rules_elem *s_rule, *r_iter;
	struct ice_fltr_list_entry *m_list_itr;
	u16 elem_sent, total_elem_left;
	enum ice_status status = 0;
	u16 num_unicast = 0;
	u16 s_rule_size;

	if (!m_list || !hw)
		return ICE_ERR_PARAM;

	list_for_each_entry(m_list_itr, m_list, list_entry) {
		u8 *add = &m_list_itr->fltr_info.l_data.mac.mac_addr[0];

		if (m_list_itr->fltr_info.lkup_type != ICE_SW_LKUP_MAC)
			return ICE_ERR_PARAM;
		if (is_zero_ether_addr(add))
			return ICE_ERR_PARAM;
		if (is_unicast_ether_addr(add) && !hw->ucast_shared) {
			/* Don't overwrite the unicast address */
			if (ice_find_mac_entry(hw, add))
				return ICE_ERR_ALREADY_EXISTS;
			num_unicast++;
		} else if (is_multicast_ether_addr(add) ||
			   (is_unicast_ether_addr(add) && hw->ucast_shared)) {
			status = ice_add_shared_mac(hw, m_list_itr);
			if (status) {
				m_list_itr->status = ICE_FLTR_STATUS_FW_FAIL;
				return status;
			}
			m_list_itr->status = ICE_FLTR_STATUS_FW_SUCCESS;
		}
	}

	/* Exit if no suitable entries were found for adding bulk switch rule */
	if (!num_unicast)
		return 0;

	/* Allocate switch rule buffer for the bulk update for unicast */
	s_rule_size = ICE_SW_RULE_RX_TX_ETH_HDR_SIZE;
	s_rule = devm_kcalloc(ice_hw_to_dev(hw), num_unicast, s_rule_size,
			      GFP_KERNEL);
	if (!s_rule)
		return ICE_ERR_NO_MEMORY;

	r_iter = s_rule;
	list_for_each_entry(m_list_itr, m_list, list_entry) {
		struct ice_fltr_info *f_info = &m_list_itr->fltr_info;
		u8 *addr = &f_info->l_data.mac.mac_addr[0];

		if (is_unicast_ether_addr(addr)) {
			ice_fill_sw_rule(hw, &m_list_itr->fltr_info,
					 r_iter, ice_aqc_opc_add_sw_rules);
			r_iter = (struct ice_aqc_sw_rules_elem *)
				((u8 *)r_iter + s_rule_size);
		}
	}

	/* Call AQ bulk switch rule update for all unicast addresses */
	r_iter = s_rule;
	/* Call AQ switch rule in AQ_MAX chunk */
	for (total_elem_left = num_unicast; total_elem_left > 0;
	     total_elem_left -= elem_sent) {
		struct ice_aqc_sw_rules_elem *entry = r_iter;

		elem_sent = min(total_elem_left,
				(u16)(ICE_AQ_MAX_BUF_LEN / s_rule_size));
		status = ice_aq_sw_rules(hw, entry, elem_sent * s_rule_size,
					 elem_sent, ice_aqc_opc_add_sw_rules,
					 NULL);
		if (status)
			goto ice_add_mac_exit;
		r_iter = (struct ice_aqc_sw_rules_elem *)
			((u8 *)r_iter + (elem_sent * s_rule_size));
	}

	/* Fill up rule id based on the value returned from FW */
	r_iter = s_rule;
	list_for_each_entry(m_list_itr, m_list, list_entry) {
		struct ice_fltr_info *f_info = &m_list_itr->fltr_info;
		u8 *addr = &f_info->l_data.mac.mac_addr[0];
		struct ice_switch_info *sw = hw->switch_info;
		struct ice_fltr_mgmt_list_entry *fm_entry;

		if (is_unicast_ether_addr(addr)) {
			f_info->fltr_rule_id =
				le16_to_cpu(r_iter->pdata.lkup_tx_rx.index);
			f_info->fltr_act = ICE_FWD_TO_VSI;
			/* Create an entry to track this MAC address */
			fm_entry = devm_kzalloc(ice_hw_to_dev(hw),
						sizeof(*fm_entry), GFP_KERNEL);
			if (!fm_entry) {
				status = ICE_ERR_NO_MEMORY;
				goto ice_add_mac_exit;
			}
			fm_entry->fltr_info = *f_info;
			fm_entry->vsi_count = 1;
			/* The book keeping entries will get removed when
			 * base driver calls remove filter AQ command
			 */
			mutex_lock(&sw->mac_list_lock);
			list_add(&fm_entry->list_entry, &sw->mac_list_head);
			mutex_unlock(&sw->mac_list_lock);

			r_iter = (struct ice_aqc_sw_rules_elem *)
				((u8 *)r_iter + s_rule_size);
		}
	}

ice_add_mac_exit:
	devm_kfree(ice_hw_to_dev(hw), s_rule);
=======
 * ice_find_rule_entry - Search a rule entry
 * @hw: pointer to the hardware structure
 * @recp_id: lookup type for which the specified rule needs to be searched
 * @f_info: rule information
 *
 * Helper function to search for a given rule entry
 * Returns pointer to entry storing the rule if found
 */
static struct ice_fltr_mgmt_list_entry *
ice_find_rule_entry(struct ice_hw *hw, u8 recp_id, struct ice_fltr_info *f_info)
{
	struct ice_fltr_mgmt_list_entry *list_itr, *ret = NULL;
	struct ice_switch_info *sw = hw->switch_info;
	struct list_head *list_head;

	list_head = &sw->recp_list[recp_id].filt_rules;
	list_for_each_entry(list_itr, list_head, list_entry) {
		if (!memcmp(&f_info->l_data, &list_itr->fltr_info.l_data,
			    sizeof(f_info->l_data)) &&
		    f_info->flag == list_itr->fltr_info.flag) {
			ret = list_itr;
			break;
		}
	}
	return ret;
}

/**
 * ice_find_vsi_list_entry - Search VSI list map with VSI count 1
 * @hw: pointer to the hardware structure
 * @recp_id: lookup type for which VSI lists needs to be searched
 * @vsi_handle: VSI handle to be found in VSI list
 * @vsi_list_id: VSI list ID found containing vsi_handle
 *
 * Helper function to search a VSI list with single entry containing given VSI
 * handle element. This can be extended further to search VSI list with more
 * than 1 vsi_count. Returns pointer to VSI list entry if found.
 */
static struct ice_vsi_list_map_info *
ice_find_vsi_list_entry(struct ice_hw *hw, u8 recp_id, u16 vsi_handle,
			u16 *vsi_list_id)
{
	struct ice_vsi_list_map_info *map_info = NULL;
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_mgmt_list_entry *list_itr;
	struct list_head *list_head;

	list_head = &sw->recp_list[recp_id].filt_rules;
	list_for_each_entry(list_itr, list_head, list_entry) {
		if (list_itr->vsi_count == 1 && list_itr->vsi_list_info) {
			map_info = list_itr->vsi_list_info;
			if (test_bit(vsi_handle, map_info->vsi_map)) {
				*vsi_list_id = map_info->vsi_list_id;
				return map_info;
			}
		}
	}
	return NULL;
}

/**
 * ice_add_rule_internal - add rule for a given lookup type
 * @hw: pointer to the hardware structure
 * @recp_id: lookup type (recipe ID) for which rule has to be added
 * @f_entry: structure containing MAC forwarding information
 *
 * Adds or updates the rule lists for a given recipe
 */
static enum ice_status
ice_add_rule_internal(struct ice_hw *hw, u8 recp_id,
		      struct ice_fltr_list_entry *f_entry)
{
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_info *new_fltr, *cur_fltr;
	struct ice_fltr_mgmt_list_entry *m_entry;
	struct mutex *rule_lock; /* Lock to protect filter rule list */
	enum ice_status status = 0;

	if (!ice_is_vsi_valid(hw, f_entry->fltr_info.vsi_handle))
		return ICE_ERR_PARAM;
	f_entry->fltr_info.fwd_id.hw_vsi_id =
		ice_get_hw_vsi_num(hw, f_entry->fltr_info.vsi_handle);

	rule_lock = &sw->recp_list[recp_id].filt_rule_lock;

	mutex_lock(rule_lock);
	new_fltr = &f_entry->fltr_info;
	if (new_fltr->flag & ICE_FLTR_RX)
		new_fltr->src = hw->port_info->lport;
	else if (new_fltr->flag & ICE_FLTR_TX)
		new_fltr->src = f_entry->fltr_info.fwd_id.hw_vsi_id;

	m_entry = ice_find_rule_entry(hw, recp_id, new_fltr);
	if (!m_entry) {
		mutex_unlock(rule_lock);
		return ice_create_pkt_fwd_rule(hw, f_entry);
	}

	cur_fltr = &m_entry->fltr_info;
	status = ice_add_update_vsi_list(hw, m_entry, cur_fltr, new_fltr);
	mutex_unlock(rule_lock);

>>>>>>> upstream/android-13
	return status;
}

/**
<<<<<<< HEAD
 * ice_find_vlan_entry
 * @hw: pointer to the hardware structure
 * @vlan_id: VLAN id to search for
 *
 * Helper function to search for a VLAN entry using a given VLAN id
 * Returns pointer to the entry if found.
 */
static struct ice_fltr_mgmt_list_entry *
ice_find_vlan_entry(struct ice_hw *hw, u16 vlan_id)
{
	struct ice_fltr_mgmt_list_entry *vlan_list_itr, *vlan_ret = NULL;
	struct ice_switch_info *sw = hw->switch_info;

	mutex_lock(&sw->vlan_list_lock);
	list_for_each_entry(vlan_list_itr, &sw->vlan_list_head, list_entry)
		if (vlan_list_itr->fltr_info.l_data.vlan.vlan_id == vlan_id) {
			vlan_ret = vlan_list_itr;
			break;
		}

	mutex_unlock(&sw->vlan_list_lock);
	return vlan_ret;
}

/**
 * ice_add_vlan_internal - Add one VLAN based filter rule
 * @hw: pointer to the hardware structure
 * @f_entry: filter entry containing one VLAN information
 */
static enum ice_status
ice_add_vlan_internal(struct ice_hw *hw, struct ice_fltr_list_entry *f_entry)
{
	struct ice_fltr_info *new_fltr, *cur_fltr;
	struct ice_fltr_mgmt_list_entry *v_list_itr;
	u16 vlan_id;

	new_fltr = &f_entry->fltr_info;
	/* VLAN id should only be 12 bits */
	if (new_fltr->l_data.vlan.vlan_id > ICE_MAX_VLAN_ID)
		return ICE_ERR_PARAM;

	vlan_id = new_fltr->l_data.vlan.vlan_id;
	v_list_itr = ice_find_vlan_entry(hw, vlan_id);
	if (!v_list_itr) {
		u16 vsi_id = ICE_VSI_INVAL_ID;
		enum ice_status status;
		u16 vsi_list_id = 0;

		if (new_fltr->fltr_act == ICE_FWD_TO_VSI) {
			enum ice_sw_lkup_type lkup_type = new_fltr->lkup_type;

			/* All VLAN pruning rules use a VSI list.
			 * Convert the action to forwarding to a VSI list.
			 */
			vsi_id = new_fltr->fwd_id.vsi_id;
			status = ice_create_vsi_list_rule(hw, &vsi_id, 1,
							  &vsi_list_id,
							  lkup_type);
			if (status)
				return status;
			new_fltr->fltr_act = ICE_FWD_TO_VSI_LIST;
			new_fltr->fwd_id.vsi_list_id = vsi_list_id;
		}

		status = ice_create_pkt_fwd_rule(hw, f_entry);
		if (!status && vsi_id != ICE_VSI_INVAL_ID) {
			v_list_itr = ice_find_vlan_entry(hw, vlan_id);
			if (!v_list_itr)
				return ICE_ERR_DOES_NOT_EXIST;
			v_list_itr->vsi_list_info =
				ice_create_vsi_list_map(hw, &vsi_id, 1,
							vsi_list_id);
		}

		return status;
	}

	cur_fltr = &v_list_itr->fltr_info;
	return ice_handle_vsi_list_mgmt(hw, v_list_itr, cur_fltr, new_fltr);
}

/**
 * ice_add_vlan - Add VLAN based filter rule
 * @hw: pointer to the hardware structure
 * @v_list: list of VLAN entries and forwarding information
 */
enum ice_status
ice_add_vlan(struct ice_hw *hw, struct list_head *v_list)
{
	struct ice_fltr_list_entry *v_list_itr;

	if (!v_list || !hw)
		return ICE_ERR_PARAM;

	list_for_each_entry(v_list_itr, v_list, list_entry) {
		enum ice_status status;

		if (v_list_itr->fltr_info.lkup_type != ICE_SW_LKUP_VLAN)
			return ICE_ERR_PARAM;

		status = ice_add_vlan_internal(hw, v_list_itr);
		if (status) {
			v_list_itr->status = ICE_FLTR_STATUS_FW_FAIL;
			return status;
		}
		v_list_itr->status = ICE_FLTR_STATUS_FW_SUCCESS;
	}
	return 0;
}

/**
 * ice_remove_vsi_list_rule
 * @hw: pointer to the hardware structure
 * @vsi_list_id: VSI list id generated as part of allocate resource
 * @lkup_type: switch rule filter lookup type
=======
 * ice_remove_vsi_list_rule
 * @hw: pointer to the hardware structure
 * @vsi_list_id: VSI list ID generated as part of allocate resource
 * @lkup_type: switch rule filter lookup type
 *
 * The VSI list should be emptied before this function is called to remove the
 * VSI list.
>>>>>>> upstream/android-13
 */
static enum ice_status
ice_remove_vsi_list_rule(struct ice_hw *hw, u16 vsi_list_id,
			 enum ice_sw_lkup_type lkup_type)
{
	struct ice_aqc_sw_rules_elem *s_rule;
	enum ice_status status;
	u16 s_rule_size;

	s_rule_size = (u16)ICE_SW_RULE_VSI_LIST_SIZE(0);
	s_rule = devm_kzalloc(ice_hw_to_dev(hw), s_rule_size, GFP_KERNEL);
	if (!s_rule)
		return ICE_ERR_NO_MEMORY;

	s_rule->type = cpu_to_le16(ICE_AQC_SW_RULES_T_VSI_LIST_CLEAR);
	s_rule->pdata.vsi_list.index = cpu_to_le16(vsi_list_id);
<<<<<<< HEAD
	/* FW expects number of VSIs in vsi_list resource to be 0 for clear
	 * command. Since memory is zero'ed out during initialization, it's not
	 * necessary to explicitly initialize the variable to 0.
	 */

	status = ice_aq_sw_rules(hw, s_rule, s_rule_size, 1,
				 ice_aqc_opc_remove_sw_rules, NULL);
	if (!status)
		/* Free the vsi_list resource that we allocated */
		status = ice_aq_alloc_free_vsi_list(hw, &vsi_list_id, lkup_type,
						    ice_aqc_opc_free_res);
=======

	/* Free the vsi_list resource that we allocated. It is assumed that the
	 * list is empty at this point.
	 */
	status = ice_aq_alloc_free_vsi_list(hw, &vsi_list_id, lkup_type,
					    ice_aqc_opc_free_res);
>>>>>>> upstream/android-13

	devm_kfree(ice_hw_to_dev(hw), s_rule);
	return status;
}

/**
<<<<<<< HEAD
 * ice_handle_rem_vsi_list_mgmt
 * @hw: pointer to the hardware structure
 * @vsi_id: ID of the VSI to remove
 * @fm_list_itr: filter management entry for which the VSI list management
 * needs to be done
 */
static enum ice_status
ice_handle_rem_vsi_list_mgmt(struct ice_hw *hw, u16 vsi_id,
			     struct ice_fltr_mgmt_list_entry *fm_list_itr)
{
	struct ice_switch_info *sw = hw->switch_info;
	enum ice_status status = 0;
	enum ice_sw_lkup_type lkup_type;
	bool is_last_elem = true;
	bool conv_list = false;
	bool del_list = false;
	u16 vsi_list_id;

	lkup_type = fm_list_itr->fltr_info.lkup_type;
	vsi_list_id = fm_list_itr->fltr_info.fwd_id.vsi_list_id;

	if (fm_list_itr->vsi_count > 1) {
		status = ice_update_vsi_list_rule(hw, &vsi_id, 1, vsi_list_id,
						  true,
=======
 * ice_rem_update_vsi_list
 * @hw: pointer to the hardware structure
 * @vsi_handle: VSI handle of the VSI to remove
 * @fm_list: filter management entry for which the VSI list management needs to
 *           be done
 */
static enum ice_status
ice_rem_update_vsi_list(struct ice_hw *hw, u16 vsi_handle,
			struct ice_fltr_mgmt_list_entry *fm_list)
{
	enum ice_sw_lkup_type lkup_type;
	enum ice_status status = 0;
	u16 vsi_list_id;

	if (fm_list->fltr_info.fltr_act != ICE_FWD_TO_VSI_LIST ||
	    fm_list->vsi_count == 0)
		return ICE_ERR_PARAM;

	/* A rule with the VSI being removed does not exist */
	if (!test_bit(vsi_handle, fm_list->vsi_list_info->vsi_map))
		return ICE_ERR_DOES_NOT_EXIST;

	lkup_type = fm_list->fltr_info.lkup_type;
	vsi_list_id = fm_list->fltr_info.fwd_id.vsi_list_id;
	status = ice_update_vsi_list_rule(hw, &vsi_handle, 1, vsi_list_id, true,
					  ice_aqc_opc_update_sw_rules,
					  lkup_type);
	if (status)
		return status;

	fm_list->vsi_count--;
	clear_bit(vsi_handle, fm_list->vsi_list_info->vsi_map);

	if (fm_list->vsi_count == 1 && lkup_type != ICE_SW_LKUP_VLAN) {
		struct ice_fltr_info tmp_fltr_info = fm_list->fltr_info;
		struct ice_vsi_list_map_info *vsi_list_info =
			fm_list->vsi_list_info;
		u16 rem_vsi_handle;

		rem_vsi_handle = find_first_bit(vsi_list_info->vsi_map,
						ICE_MAX_VSI);
		if (!ice_is_vsi_valid(hw, rem_vsi_handle))
			return ICE_ERR_OUT_OF_RANGE;

		/* Make sure VSI list is empty before removing it below */
		status = ice_update_vsi_list_rule(hw, &rem_vsi_handle, 1,
						  vsi_list_id, true,
>>>>>>> upstream/android-13
						  ice_aqc_opc_update_sw_rules,
						  lkup_type);
		if (status)
			return status;
<<<<<<< HEAD
		fm_list_itr->vsi_count--;
		is_last_elem = false;
		clear_bit(vsi_id, fm_list_itr->vsi_list_info->vsi_map);
	}

	/* For non-VLAN rules that forward packets to a VSI list, convert them
	 * to forwarding packets to a VSI if there is only one VSI left in the
	 * list.  Unused lists are then removed.
	 * VLAN rules need to use VSI lists even with only one VSI.
	 */
	if (fm_list_itr->fltr_info.fltr_act == ICE_FWD_TO_VSI_LIST) {
		if (lkup_type == ICE_SW_LKUP_VLAN) {
			del_list = is_last_elem;
		} else if (fm_list_itr->vsi_count == 1) {
			conv_list = true;
			del_list = true;
		}
	}

	if (del_list) {
		/* Remove the VSI list since it is no longer used */
		struct ice_vsi_list_map_info *vsi_list_info =
			fm_list_itr->vsi_list_info;

		status = ice_remove_vsi_list_rule(hw, vsi_list_id, lkup_type);
		if (status)
			return status;

		if (conv_list) {
			u16 rem_vsi_id;

			rem_vsi_id = find_first_bit(vsi_list_info->vsi_map,
						    ICE_MAX_VSI);

			/* Error out when the expected last element is not in
			 * the VSI list map
			 */
			if (rem_vsi_id == ICE_MAX_VSI)
				return ICE_ERR_OUT_OF_RANGE;

			/* Change the list entry action from VSI_LIST to VSI */
			fm_list_itr->fltr_info.fltr_act = ICE_FWD_TO_VSI;
			fm_list_itr->fltr_info.fwd_id.vsi_id = rem_vsi_id;
=======

		tmp_fltr_info.fltr_act = ICE_FWD_TO_VSI;
		tmp_fltr_info.fwd_id.hw_vsi_id =
			ice_get_hw_vsi_num(hw, rem_vsi_handle);
		tmp_fltr_info.vsi_handle = rem_vsi_handle;
		status = ice_update_pkt_fwd_rule(hw, &tmp_fltr_info);
		if (status) {
			ice_debug(hw, ICE_DBG_SW, "Failed to update pkt fwd rule to FWD_TO_VSI on HW VSI %d, error %d\n",
				  tmp_fltr_info.fwd_id.hw_vsi_id, status);
			return status;
		}

		fm_list->fltr_info = tmp_fltr_info;
	}

	if ((fm_list->vsi_count == 1 && lkup_type != ICE_SW_LKUP_VLAN) ||
	    (fm_list->vsi_count == 0 && lkup_type == ICE_SW_LKUP_VLAN)) {
		struct ice_vsi_list_map_info *vsi_list_info =
			fm_list->vsi_list_info;

		/* Remove the VSI list since it is no longer used */
		status = ice_remove_vsi_list_rule(hw, vsi_list_id, lkup_type);
		if (status) {
			ice_debug(hw, ICE_DBG_SW, "Failed to remove VSI list %d, error %d\n",
				  vsi_list_id, status);
			return status;
>>>>>>> upstream/android-13
		}

		list_del(&vsi_list_info->list_entry);
		devm_kfree(ice_hw_to_dev(hw), vsi_list_info);
<<<<<<< HEAD
		fm_list_itr->vsi_list_info = NULL;
	}

	if (conv_list) {
		/* Convert the rule's forward action to forwarding packets to
		 * a VSI
		 */
		struct ice_aqc_sw_rules_elem *s_rule;

		s_rule = devm_kzalloc(ice_hw_to_dev(hw),
				      ICE_SW_RULE_RX_TX_ETH_HDR_SIZE,
				      GFP_KERNEL);
		if (!s_rule)
			return ICE_ERR_NO_MEMORY;

		ice_fill_sw_rule(hw, &fm_list_itr->fltr_info, s_rule,
				 ice_aqc_opc_update_sw_rules);

		s_rule->pdata.lkup_tx_rx.index =
			cpu_to_le16(fm_list_itr->fltr_info.fltr_rule_id);

		status = ice_aq_sw_rules(hw, s_rule,
					 ICE_SW_RULE_RX_TX_ETH_HDR_SIZE, 1,
					 ice_aqc_opc_update_sw_rules, NULL);
		devm_kfree(ice_hw_to_dev(hw), s_rule);
		if (status)
			return status;
	}

	if (is_last_elem) {
=======
		fm_list->vsi_list_info = NULL;
	}

	return status;
}

/**
 * ice_remove_rule_internal - Remove a filter rule of a given type
 * @hw: pointer to the hardware structure
 * @recp_id: recipe ID for which the rule needs to removed
 * @f_entry: rule entry containing filter information
 */
static enum ice_status
ice_remove_rule_internal(struct ice_hw *hw, u8 recp_id,
			 struct ice_fltr_list_entry *f_entry)
{
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_mgmt_list_entry *list_elem;
	struct mutex *rule_lock; /* Lock to protect filter rule list */
	enum ice_status status = 0;
	bool remove_rule = false;
	u16 vsi_handle;

	if (!ice_is_vsi_valid(hw, f_entry->fltr_info.vsi_handle))
		return ICE_ERR_PARAM;
	f_entry->fltr_info.fwd_id.hw_vsi_id =
		ice_get_hw_vsi_num(hw, f_entry->fltr_info.vsi_handle);

	rule_lock = &sw->recp_list[recp_id].filt_rule_lock;
	mutex_lock(rule_lock);
	list_elem = ice_find_rule_entry(hw, recp_id, &f_entry->fltr_info);
	if (!list_elem) {
		status = ICE_ERR_DOES_NOT_EXIST;
		goto exit;
	}

	if (list_elem->fltr_info.fltr_act != ICE_FWD_TO_VSI_LIST) {
		remove_rule = true;
	} else if (!list_elem->vsi_list_info) {
		status = ICE_ERR_DOES_NOT_EXIST;
		goto exit;
	} else if (list_elem->vsi_list_info->ref_cnt > 1) {
		/* a ref_cnt > 1 indicates that the vsi_list is being
		 * shared by multiple rules. Decrement the ref_cnt and
		 * remove this rule, but do not modify the list, as it
		 * is in-use by other rules.
		 */
		list_elem->vsi_list_info->ref_cnt--;
		remove_rule = true;
	} else {
		/* a ref_cnt of 1 indicates the vsi_list is only used
		 * by one rule. However, the original removal request is only
		 * for a single VSI. Update the vsi_list first, and only
		 * remove the rule if there are no further VSIs in this list.
		 */
		vsi_handle = f_entry->fltr_info.vsi_handle;
		status = ice_rem_update_vsi_list(hw, vsi_handle, list_elem);
		if (status)
			goto exit;
		/* if VSI count goes to zero after updating the VSI list */
		if (list_elem->vsi_count == 0)
			remove_rule = true;
	}

	if (remove_rule) {
>>>>>>> upstream/android-13
		/* Remove the lookup rule */
		struct ice_aqc_sw_rules_elem *s_rule;

		s_rule = devm_kzalloc(ice_hw_to_dev(hw),
				      ICE_SW_RULE_RX_TX_NO_HDR_SIZE,
				      GFP_KERNEL);
<<<<<<< HEAD
		if (!s_rule)
			return ICE_ERR_NO_MEMORY;

		ice_fill_sw_rule(hw, &fm_list_itr->fltr_info, s_rule,
=======
		if (!s_rule) {
			status = ICE_ERR_NO_MEMORY;
			goto exit;
		}

		ice_fill_sw_rule(hw, &list_elem->fltr_info, s_rule,
>>>>>>> upstream/android-13
				 ice_aqc_opc_remove_sw_rules);

		status = ice_aq_sw_rules(hw, s_rule,
					 ICE_SW_RULE_RX_TX_NO_HDR_SIZE, 1,
					 ice_aqc_opc_remove_sw_rules, NULL);
<<<<<<< HEAD
		if (status)
			return status;

		/* Remove a book keeping entry from the MAC address list */
		mutex_lock(&sw->mac_list_lock);
		list_del(&fm_list_itr->list_entry);
		mutex_unlock(&sw->mac_list_lock);
		devm_kfree(ice_hw_to_dev(hw), fm_list_itr);
		devm_kfree(ice_hw_to_dev(hw), s_rule);
	}
=======

		/* Remove a book keeping from the list */
		devm_kfree(ice_hw_to_dev(hw), s_rule);

		if (status)
			goto exit;

		list_del(&list_elem->list_entry);
		devm_kfree(ice_hw_to_dev(hw), list_elem);
	}
exit:
	mutex_unlock(rule_lock);
>>>>>>> upstream/android-13
	return status;
}

/**
<<<<<<< HEAD
 * ice_remove_mac_entry
 * @hw: pointer to the hardware structure
 * @f_entry: structure containing MAC forwarding information
 */
static enum ice_status
ice_remove_mac_entry(struct ice_hw *hw, struct ice_fltr_list_entry *f_entry)
{
	struct ice_fltr_mgmt_list_entry *m_entry;
	u16 vsi_id;
	u8 *add;

	add = &f_entry->fltr_info.l_data.mac.mac_addr[0];

	m_entry = ice_find_mac_entry(hw, add);
	if (!m_entry)
		return ICE_ERR_PARAM;

	vsi_id = f_entry->fltr_info.fwd_id.vsi_id;
	return ice_handle_rem_vsi_list_mgmt(hw, vsi_id, m_entry);
}

/**
 * ice_remove_mac - remove a MAC address based filter rule
 * @hw: pointer to the hardware structure
 * @m_list: list of MAC addresses and forwarding information
 *
 * This function removes either a MAC filter rule or a specific VSI from a
 * VSI list for a multicast MAC address.
 *
 * Returns ICE_ERR_DOES_NOT_EXIST if a given entry was not added by
 * ice_add_mac. Caller should be aware that this call will only work if all
 * the entries passed into m_list were added previously. It will not attempt to
 * do a partial remove of entries that were found.
 */
enum ice_status
ice_remove_mac(struct ice_hw *hw, struct list_head *m_list)
{
	struct ice_aqc_sw_rules_elem *s_rule, *r_iter;
	u8 s_rule_size = ICE_SW_RULE_RX_TX_NO_HDR_SIZE;
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_mgmt_list_entry *m_entry;
	struct ice_fltr_list_entry *m_list_itr;
	u16 elem_sent, total_elem_left;
	enum ice_status status = 0;
	u16 num_unicast = 0;

	if (!m_list)
		return ICE_ERR_PARAM;

	list_for_each_entry(m_list_itr, m_list, list_entry) {
		u8 *addr = m_list_itr->fltr_info.l_data.mac.mac_addr;

		if (is_unicast_ether_addr(addr) && !hw->ucast_shared)
			num_unicast++;
		else if (is_multicast_ether_addr(addr) ||
			 (is_unicast_ether_addr(addr) && hw->ucast_shared))
			ice_remove_mac_entry(hw, m_list_itr);
	}

	/* Exit if no unicast addresses found. Multicast switch rules
	 * were added individually
	 */
	if (!num_unicast)
		return 0;

	/* Allocate switch rule buffer for the bulk update for unicast */
	s_rule = devm_kcalloc(ice_hw_to_dev(hw), num_unicast, s_rule_size,
			      GFP_KERNEL);
	if (!s_rule)
		return ICE_ERR_NO_MEMORY;

	r_iter = s_rule;
	list_for_each_entry(m_list_itr, m_list, list_entry) {
		u8 *addr = m_list_itr->fltr_info.l_data.mac.mac_addr;

		if (is_unicast_ether_addr(addr)) {
			m_entry = ice_find_mac_entry(hw, addr);
			if (!m_entry) {
				status = ICE_ERR_DOES_NOT_EXIST;
				goto ice_remove_mac_exit;
			}

			ice_fill_sw_rule(hw, &m_entry->fltr_info,
					 r_iter, ice_aqc_opc_remove_sw_rules);
=======
 * ice_add_mac - Add a MAC address based filter rule
 * @hw: pointer to the hardware structure
 * @m_list: list of MAC addresses and forwarding information
 *
 * IMPORTANT: When the ucast_shared flag is set to false and m_list has
 * multiple unicast addresses, the function assumes that all the
 * addresses are unique in a given add_mac call. It doesn't
 * check for duplicates in this case, removing duplicates from a given
 * list should be taken care of in the caller of this function.
 */
enum ice_status ice_add_mac(struct ice_hw *hw, struct list_head *m_list)
{
	struct ice_aqc_sw_rules_elem *s_rule, *r_iter;
	struct ice_fltr_list_entry *m_list_itr;
	struct list_head *rule_head;
	u16 total_elem_left, s_rule_size;
	struct ice_switch_info *sw;
	struct mutex *rule_lock; /* Lock to protect filter rule list */
	enum ice_status status = 0;
	u16 num_unicast = 0;
	u8 elem_sent;

	if (!m_list || !hw)
		return ICE_ERR_PARAM;

	s_rule = NULL;
	sw = hw->switch_info;
	rule_lock = &sw->recp_list[ICE_SW_LKUP_MAC].filt_rule_lock;
	list_for_each_entry(m_list_itr, m_list, list_entry) {
		u8 *add = &m_list_itr->fltr_info.l_data.mac.mac_addr[0];
		u16 vsi_handle;
		u16 hw_vsi_id;

		m_list_itr->fltr_info.flag = ICE_FLTR_TX;
		vsi_handle = m_list_itr->fltr_info.vsi_handle;
		if (!ice_is_vsi_valid(hw, vsi_handle))
			return ICE_ERR_PARAM;
		hw_vsi_id = ice_get_hw_vsi_num(hw, vsi_handle);
		m_list_itr->fltr_info.fwd_id.hw_vsi_id = hw_vsi_id;
		/* update the src in case it is VSI num */
		if (m_list_itr->fltr_info.src_id != ICE_SRC_ID_VSI)
			return ICE_ERR_PARAM;
		m_list_itr->fltr_info.src = hw_vsi_id;
		if (m_list_itr->fltr_info.lkup_type != ICE_SW_LKUP_MAC ||
		    is_zero_ether_addr(add))
			return ICE_ERR_PARAM;
		if (is_unicast_ether_addr(add) && !hw->ucast_shared) {
			/* Don't overwrite the unicast address */
			mutex_lock(rule_lock);
			if (ice_find_rule_entry(hw, ICE_SW_LKUP_MAC,
						&m_list_itr->fltr_info)) {
				mutex_unlock(rule_lock);
				return ICE_ERR_ALREADY_EXISTS;
			}
			mutex_unlock(rule_lock);
			num_unicast++;
		} else if (is_multicast_ether_addr(add) ||
			   (is_unicast_ether_addr(add) && hw->ucast_shared)) {
			m_list_itr->status =
				ice_add_rule_internal(hw, ICE_SW_LKUP_MAC,
						      m_list_itr);
			if (m_list_itr->status)
				return m_list_itr->status;
		}
	}

	mutex_lock(rule_lock);
	/* Exit if no suitable entries were found for adding bulk switch rule */
	if (!num_unicast) {
		status = 0;
		goto ice_add_mac_exit;
	}

	rule_head = &sw->recp_list[ICE_SW_LKUP_MAC].filt_rules;

	/* Allocate switch rule buffer for the bulk update for unicast */
	s_rule_size = ICE_SW_RULE_RX_TX_ETH_HDR_SIZE;
	s_rule = devm_kcalloc(ice_hw_to_dev(hw), num_unicast, s_rule_size,
			      GFP_KERNEL);
	if (!s_rule) {
		status = ICE_ERR_NO_MEMORY;
		goto ice_add_mac_exit;
	}

	r_iter = s_rule;
	list_for_each_entry(m_list_itr, m_list, list_entry) {
		struct ice_fltr_info *f_info = &m_list_itr->fltr_info;
		u8 *mac_addr = &f_info->l_data.mac.mac_addr[0];

		if (is_unicast_ether_addr(mac_addr)) {
			ice_fill_sw_rule(hw, &m_list_itr->fltr_info, r_iter,
					 ice_aqc_opc_add_sw_rules);
>>>>>>> upstream/android-13
			r_iter = (struct ice_aqc_sw_rules_elem *)
				((u8 *)r_iter + s_rule_size);
		}
	}

	/* Call AQ bulk switch rule update for all unicast addresses */
	r_iter = s_rule;
	/* Call AQ switch rule in AQ_MAX chunk */
	for (total_elem_left = num_unicast; total_elem_left > 0;
	     total_elem_left -= elem_sent) {
		struct ice_aqc_sw_rules_elem *entry = r_iter;

<<<<<<< HEAD
		elem_sent = min(total_elem_left,
				(u16)(ICE_AQ_MAX_BUF_LEN / s_rule_size));
		status = ice_aq_sw_rules(hw, entry, elem_sent * s_rule_size,
					 elem_sent, ice_aqc_opc_remove_sw_rules,
					 NULL);
		if (status)
			break;
		r_iter = (struct ice_aqc_sw_rules_elem *)
			((u8 *)r_iter + s_rule_size);
	}

	list_for_each_entry(m_list_itr, m_list, list_entry) {
		u8 *addr = m_list_itr->fltr_info.l_data.mac.mac_addr;

		if (is_unicast_ether_addr(addr)) {
			m_entry = ice_find_mac_entry(hw, addr);
			if (!m_entry)
				return ICE_ERR_OUT_OF_RANGE;
			mutex_lock(&sw->mac_list_lock);
			list_del(&m_entry->list_entry);
			mutex_unlock(&sw->mac_list_lock);
			devm_kfree(ice_hw_to_dev(hw), m_entry);
		}
	}

ice_remove_mac_exit:
	devm_kfree(ice_hw_to_dev(hw), s_rule);
=======
		elem_sent = min_t(u8, total_elem_left,
				  (ICE_AQ_MAX_BUF_LEN / s_rule_size));
		status = ice_aq_sw_rules(hw, entry, elem_sent * s_rule_size,
					 elem_sent, ice_aqc_opc_add_sw_rules,
					 NULL);
		if (status)
			goto ice_add_mac_exit;
		r_iter = (struct ice_aqc_sw_rules_elem *)
			((u8 *)r_iter + (elem_sent * s_rule_size));
	}

	/* Fill up rule ID based on the value returned from FW */
	r_iter = s_rule;
	list_for_each_entry(m_list_itr, m_list, list_entry) {
		struct ice_fltr_info *f_info = &m_list_itr->fltr_info;
		u8 *mac_addr = &f_info->l_data.mac.mac_addr[0];
		struct ice_fltr_mgmt_list_entry *fm_entry;

		if (is_unicast_ether_addr(mac_addr)) {
			f_info->fltr_rule_id =
				le16_to_cpu(r_iter->pdata.lkup_tx_rx.index);
			f_info->fltr_act = ICE_FWD_TO_VSI;
			/* Create an entry to track this MAC address */
			fm_entry = devm_kzalloc(ice_hw_to_dev(hw),
						sizeof(*fm_entry), GFP_KERNEL);
			if (!fm_entry) {
				status = ICE_ERR_NO_MEMORY;
				goto ice_add_mac_exit;
			}
			fm_entry->fltr_info = *f_info;
			fm_entry->vsi_count = 1;
			/* The book keeping entries will get removed when
			 * base driver calls remove filter AQ command
			 */

			list_add(&fm_entry->list_entry, rule_head);
			r_iter = (struct ice_aqc_sw_rules_elem *)
				((u8 *)r_iter + s_rule_size);
		}
	}

ice_add_mac_exit:
	mutex_unlock(rule_lock);
	if (s_rule)
		devm_kfree(ice_hw_to_dev(hw), s_rule);
>>>>>>> upstream/android-13
	return status;
}

/**
<<<<<<< HEAD
 * ice_cfg_dflt_vsi - add filter rule to set/unset given VSI as default
 * VSI for the switch (represented by swid)
 * @hw: pointer to the hardware structure
 * @vsi_id: number of VSI to set as default
 * @set: true to add the above mentioned switch rule, false to remove it
 * @direction: ICE_FLTR_RX or ICE_FLTR_TX
 */
enum ice_status
ice_cfg_dflt_vsi(struct ice_hw *hw, u16 vsi_id, bool set, u8 direction)
=======
 * ice_add_vlan_internal - Add one VLAN based filter rule
 * @hw: pointer to the hardware structure
 * @f_entry: filter entry containing one VLAN information
 */
static enum ice_status
ice_add_vlan_internal(struct ice_hw *hw, struct ice_fltr_list_entry *f_entry)
{
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_mgmt_list_entry *v_list_itr;
	struct ice_fltr_info *new_fltr, *cur_fltr;
	enum ice_sw_lkup_type lkup_type;
	u16 vsi_list_id = 0, vsi_handle;
	struct mutex *rule_lock; /* Lock to protect filter rule list */
	enum ice_status status = 0;

	if (!ice_is_vsi_valid(hw, f_entry->fltr_info.vsi_handle))
		return ICE_ERR_PARAM;

	f_entry->fltr_info.fwd_id.hw_vsi_id =
		ice_get_hw_vsi_num(hw, f_entry->fltr_info.vsi_handle);
	new_fltr = &f_entry->fltr_info;

	/* VLAN ID should only be 12 bits */
	if (new_fltr->l_data.vlan.vlan_id > ICE_MAX_VLAN_ID)
		return ICE_ERR_PARAM;

	if (new_fltr->src_id != ICE_SRC_ID_VSI)
		return ICE_ERR_PARAM;

	new_fltr->src = new_fltr->fwd_id.hw_vsi_id;
	lkup_type = new_fltr->lkup_type;
	vsi_handle = new_fltr->vsi_handle;
	rule_lock = &sw->recp_list[ICE_SW_LKUP_VLAN].filt_rule_lock;
	mutex_lock(rule_lock);
	v_list_itr = ice_find_rule_entry(hw, ICE_SW_LKUP_VLAN, new_fltr);
	if (!v_list_itr) {
		struct ice_vsi_list_map_info *map_info = NULL;

		if (new_fltr->fltr_act == ICE_FWD_TO_VSI) {
			/* All VLAN pruning rules use a VSI list. Check if
			 * there is already a VSI list containing VSI that we
			 * want to add. If found, use the same vsi_list_id for
			 * this new VLAN rule or else create a new list.
			 */
			map_info = ice_find_vsi_list_entry(hw, ICE_SW_LKUP_VLAN,
							   vsi_handle,
							   &vsi_list_id);
			if (!map_info) {
				status = ice_create_vsi_list_rule(hw,
								  &vsi_handle,
								  1,
								  &vsi_list_id,
								  lkup_type);
				if (status)
					goto exit;
			}
			/* Convert the action to forwarding to a VSI list. */
			new_fltr->fltr_act = ICE_FWD_TO_VSI_LIST;
			new_fltr->fwd_id.vsi_list_id = vsi_list_id;
		}

		status = ice_create_pkt_fwd_rule(hw, f_entry);
		if (!status) {
			v_list_itr = ice_find_rule_entry(hw, ICE_SW_LKUP_VLAN,
							 new_fltr);
			if (!v_list_itr) {
				status = ICE_ERR_DOES_NOT_EXIST;
				goto exit;
			}
			/* reuse VSI list for new rule and increment ref_cnt */
			if (map_info) {
				v_list_itr->vsi_list_info = map_info;
				map_info->ref_cnt++;
			} else {
				v_list_itr->vsi_list_info =
					ice_create_vsi_list_map(hw, &vsi_handle,
								1, vsi_list_id);
			}
		}
	} else if (v_list_itr->vsi_list_info->ref_cnt == 1) {
		/* Update existing VSI list to add new VSI ID only if it used
		 * by one VLAN rule.
		 */
		cur_fltr = &v_list_itr->fltr_info;
		status = ice_add_update_vsi_list(hw, v_list_itr, cur_fltr,
						 new_fltr);
	} else {
		/* If VLAN rule exists and VSI list being used by this rule is
		 * referenced by more than 1 VLAN rule. Then create a new VSI
		 * list appending previous VSI with new VSI and update existing
		 * VLAN rule to point to new VSI list ID
		 */
		struct ice_fltr_info tmp_fltr;
		u16 vsi_handle_arr[2];
		u16 cur_handle;

		/* Current implementation only supports reusing VSI list with
		 * one VSI count. We should never hit below condition
		 */
		if (v_list_itr->vsi_count > 1 &&
		    v_list_itr->vsi_list_info->ref_cnt > 1) {
			ice_debug(hw, ICE_DBG_SW, "Invalid configuration: Optimization to reuse VSI list with more than one VSI is not being done yet\n");
			status = ICE_ERR_CFG;
			goto exit;
		}

		cur_handle =
			find_first_bit(v_list_itr->vsi_list_info->vsi_map,
				       ICE_MAX_VSI);

		/* A rule already exists with the new VSI being added */
		if (cur_handle == vsi_handle) {
			status = ICE_ERR_ALREADY_EXISTS;
			goto exit;
		}

		vsi_handle_arr[0] = cur_handle;
		vsi_handle_arr[1] = vsi_handle;
		status = ice_create_vsi_list_rule(hw, &vsi_handle_arr[0], 2,
						  &vsi_list_id, lkup_type);
		if (status)
			goto exit;

		tmp_fltr = v_list_itr->fltr_info;
		tmp_fltr.fltr_rule_id = v_list_itr->fltr_info.fltr_rule_id;
		tmp_fltr.fwd_id.vsi_list_id = vsi_list_id;
		tmp_fltr.fltr_act = ICE_FWD_TO_VSI_LIST;
		/* Update the previous switch rule to a new VSI list which
		 * includes current VSI that is requested
		 */
		status = ice_update_pkt_fwd_rule(hw, &tmp_fltr);
		if (status)
			goto exit;

		/* before overriding VSI list map info. decrement ref_cnt of
		 * previous VSI list
		 */
		v_list_itr->vsi_list_info->ref_cnt--;

		/* now update to newly created list */
		v_list_itr->fltr_info.fwd_id.vsi_list_id = vsi_list_id;
		v_list_itr->vsi_list_info =
			ice_create_vsi_list_map(hw, &vsi_handle_arr[0], 2,
						vsi_list_id);
		v_list_itr->vsi_count++;
	}

exit:
	mutex_unlock(rule_lock);
	return status;
}

/**
 * ice_add_vlan - Add VLAN based filter rule
 * @hw: pointer to the hardware structure
 * @v_list: list of VLAN entries and forwarding information
 */
enum ice_status ice_add_vlan(struct ice_hw *hw, struct list_head *v_list)
{
	struct ice_fltr_list_entry *v_list_itr;

	if (!v_list || !hw)
		return ICE_ERR_PARAM;

	list_for_each_entry(v_list_itr, v_list, list_entry) {
		if (v_list_itr->fltr_info.lkup_type != ICE_SW_LKUP_VLAN)
			return ICE_ERR_PARAM;
		v_list_itr->fltr_info.flag = ICE_FLTR_TX;
		v_list_itr->status = ice_add_vlan_internal(hw, v_list_itr);
		if (v_list_itr->status)
			return v_list_itr->status;
	}
	return 0;
}

/**
 * ice_add_eth_mac - Add ethertype and MAC based filter rule
 * @hw: pointer to the hardware structure
 * @em_list: list of ether type MAC filter, MAC is optional
 *
 * This function requires the caller to populate the entries in
 * the filter list with the necessary fields (including flags to
 * indicate Tx or Rx rules).
 */
enum ice_status
ice_add_eth_mac(struct ice_hw *hw, struct list_head *em_list)
{
	struct ice_fltr_list_entry *em_list_itr;

	if (!em_list || !hw)
		return ICE_ERR_PARAM;

	list_for_each_entry(em_list_itr, em_list, list_entry) {
		enum ice_sw_lkup_type l_type =
			em_list_itr->fltr_info.lkup_type;

		if (l_type != ICE_SW_LKUP_ETHERTYPE_MAC &&
		    l_type != ICE_SW_LKUP_ETHERTYPE)
			return ICE_ERR_PARAM;

		em_list_itr->status = ice_add_rule_internal(hw, l_type,
							    em_list_itr);
		if (em_list_itr->status)
			return em_list_itr->status;
	}
	return 0;
}

/**
 * ice_remove_eth_mac - Remove an ethertype (or MAC) based filter rule
 * @hw: pointer to the hardware structure
 * @em_list: list of ethertype or ethertype MAC entries
 */
enum ice_status
ice_remove_eth_mac(struct ice_hw *hw, struct list_head *em_list)
{
	struct ice_fltr_list_entry *em_list_itr, *tmp;

	if (!em_list || !hw)
		return ICE_ERR_PARAM;

	list_for_each_entry_safe(em_list_itr, tmp, em_list, list_entry) {
		enum ice_sw_lkup_type l_type =
			em_list_itr->fltr_info.lkup_type;

		if (l_type != ICE_SW_LKUP_ETHERTYPE_MAC &&
		    l_type != ICE_SW_LKUP_ETHERTYPE)
			return ICE_ERR_PARAM;

		em_list_itr->status = ice_remove_rule_internal(hw, l_type,
							       em_list_itr);
		if (em_list_itr->status)
			return em_list_itr->status;
	}
	return 0;
}

/**
 * ice_rem_sw_rule_info
 * @hw: pointer to the hardware structure
 * @rule_head: pointer to the switch list structure that we want to delete
 */
static void
ice_rem_sw_rule_info(struct ice_hw *hw, struct list_head *rule_head)
{
	if (!list_empty(rule_head)) {
		struct ice_fltr_mgmt_list_entry *entry;
		struct ice_fltr_mgmt_list_entry *tmp;

		list_for_each_entry_safe(entry, tmp, rule_head, list_entry) {
			list_del(&entry->list_entry);
			devm_kfree(ice_hw_to_dev(hw), entry);
		}
	}
}

/**
 * ice_cfg_dflt_vsi - change state of VSI to set/clear default
 * @hw: pointer to the hardware structure
 * @vsi_handle: VSI handle to set as default
 * @set: true to add the above mentioned switch rule, false to remove it
 * @direction: ICE_FLTR_RX or ICE_FLTR_TX
 *
 * add filter rule to set/unset given VSI as default VSI for the switch
 * (represented by swid)
 */
enum ice_status
ice_cfg_dflt_vsi(struct ice_hw *hw, u16 vsi_handle, bool set, u8 direction)
>>>>>>> upstream/android-13
{
	struct ice_aqc_sw_rules_elem *s_rule;
	struct ice_fltr_info f_info;
	enum ice_adminq_opc opcode;
	enum ice_status status;
	u16 s_rule_size;
<<<<<<< HEAD

	s_rule_size = set ? ICE_SW_RULE_RX_TX_ETH_HDR_SIZE :
			    ICE_SW_RULE_RX_TX_NO_HDR_SIZE;
=======
	u16 hw_vsi_id;

	if (!ice_is_vsi_valid(hw, vsi_handle))
		return ICE_ERR_PARAM;
	hw_vsi_id = ice_get_hw_vsi_num(hw, vsi_handle);

	s_rule_size = set ? ICE_SW_RULE_RX_TX_ETH_HDR_SIZE :
		ICE_SW_RULE_RX_TX_NO_HDR_SIZE;

>>>>>>> upstream/android-13
	s_rule = devm_kzalloc(ice_hw_to_dev(hw), s_rule_size, GFP_KERNEL);
	if (!s_rule)
		return ICE_ERR_NO_MEMORY;

	memset(&f_info, 0, sizeof(f_info));

	f_info.lkup_type = ICE_SW_LKUP_DFLT;
	f_info.flag = direction;
	f_info.fltr_act = ICE_FWD_TO_VSI;
<<<<<<< HEAD
	f_info.fwd_id.vsi_id = vsi_id;

	if (f_info.flag & ICE_FLTR_RX) {
		f_info.src = hw->port_info->lport;
=======
	f_info.fwd_id.hw_vsi_id = hw_vsi_id;

	if (f_info.flag & ICE_FLTR_RX) {
		f_info.src = hw->port_info->lport;
		f_info.src_id = ICE_SRC_ID_LPORT;
>>>>>>> upstream/android-13
		if (!set)
			f_info.fltr_rule_id =
				hw->port_info->dflt_rx_vsi_rule_id;
	} else if (f_info.flag & ICE_FLTR_TX) {
<<<<<<< HEAD
		f_info.src = vsi_id;
=======
		f_info.src_id = ICE_SRC_ID_VSI;
		f_info.src = hw_vsi_id;
>>>>>>> upstream/android-13
		if (!set)
			f_info.fltr_rule_id =
				hw->port_info->dflt_tx_vsi_rule_id;
	}

	if (set)
		opcode = ice_aqc_opc_add_sw_rules;
	else
		opcode = ice_aqc_opc_remove_sw_rules;

	ice_fill_sw_rule(hw, &f_info, s_rule, opcode);

	status = ice_aq_sw_rules(hw, s_rule, s_rule_size, 1, opcode, NULL);
	if (status || !(f_info.flag & ICE_FLTR_TX_RX))
		goto out;
	if (set) {
		u16 index = le16_to_cpu(s_rule->pdata.lkup_tx_rx.index);

		if (f_info.flag & ICE_FLTR_TX) {
<<<<<<< HEAD
			hw->port_info->dflt_tx_vsi_num = vsi_id;
			hw->port_info->dflt_tx_vsi_rule_id = index;
		} else if (f_info.flag & ICE_FLTR_RX) {
			hw->port_info->dflt_rx_vsi_num = vsi_id;
=======
			hw->port_info->dflt_tx_vsi_num = hw_vsi_id;
			hw->port_info->dflt_tx_vsi_rule_id = index;
		} else if (f_info.flag & ICE_FLTR_RX) {
			hw->port_info->dflt_rx_vsi_num = hw_vsi_id;
>>>>>>> upstream/android-13
			hw->port_info->dflt_rx_vsi_rule_id = index;
		}
	} else {
		if (f_info.flag & ICE_FLTR_TX) {
			hw->port_info->dflt_tx_vsi_num = ICE_DFLT_VSI_INVAL;
			hw->port_info->dflt_tx_vsi_rule_id = ICE_INVAL_ACT;
		} else if (f_info.flag & ICE_FLTR_RX) {
			hw->port_info->dflt_rx_vsi_num = ICE_DFLT_VSI_INVAL;
			hw->port_info->dflt_rx_vsi_rule_id = ICE_INVAL_ACT;
		}
	}

out:
	devm_kfree(ice_hw_to_dev(hw), s_rule);
	return status;
}

/**
<<<<<<< HEAD
 * ice_remove_vlan_internal - Remove one VLAN based filter rule
 * @hw: pointer to the hardware structure
 * @f_entry: filter entry containing one VLAN information
 */
static enum ice_status
ice_remove_vlan_internal(struct ice_hw *hw,
			 struct ice_fltr_list_entry *f_entry)
{
	struct ice_fltr_info *new_fltr;
	struct ice_fltr_mgmt_list_entry *v_list_elem;
	u16 vsi_id;

	new_fltr = &f_entry->fltr_info;

	v_list_elem = ice_find_vlan_entry(hw, new_fltr->l_data.vlan.vlan_id);
	if (!v_list_elem)
		return ICE_ERR_PARAM;

	vsi_id = f_entry->fltr_info.fwd_id.vsi_id;
	return ice_handle_rem_vsi_list_mgmt(hw, vsi_id, v_list_elem);
=======
 * ice_find_ucast_rule_entry - Search for a unicast MAC filter rule entry
 * @hw: pointer to the hardware structure
 * @recp_id: lookup type for which the specified rule needs to be searched
 * @f_info: rule information
 *
 * Helper function to search for a unicast rule entry - this is to be used
 * to remove unicast MAC filter that is not shared with other VSIs on the
 * PF switch.
 *
 * Returns pointer to entry storing the rule if found
 */
static struct ice_fltr_mgmt_list_entry *
ice_find_ucast_rule_entry(struct ice_hw *hw, u8 recp_id,
			  struct ice_fltr_info *f_info)
{
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_mgmt_list_entry *list_itr;
	struct list_head *list_head;

	list_head = &sw->recp_list[recp_id].filt_rules;
	list_for_each_entry(list_itr, list_head, list_entry) {
		if (!memcmp(&f_info->l_data, &list_itr->fltr_info.l_data,
			    sizeof(f_info->l_data)) &&
		    f_info->fwd_id.hw_vsi_id ==
		    list_itr->fltr_info.fwd_id.hw_vsi_id &&
		    f_info->flag == list_itr->fltr_info.flag)
			return list_itr;
	}
	return NULL;
}

/**
 * ice_remove_mac - remove a MAC address based filter rule
 * @hw: pointer to the hardware structure
 * @m_list: list of MAC addresses and forwarding information
 *
 * This function removes either a MAC filter rule or a specific VSI from a
 * VSI list for a multicast MAC address.
 *
 * Returns ICE_ERR_DOES_NOT_EXIST if a given entry was not added by
 * ice_add_mac. Caller should be aware that this call will only work if all
 * the entries passed into m_list were added previously. It will not attempt to
 * do a partial remove of entries that were found.
 */
enum ice_status ice_remove_mac(struct ice_hw *hw, struct list_head *m_list)
{
	struct ice_fltr_list_entry *list_itr, *tmp;
	struct mutex *rule_lock; /* Lock to protect filter rule list */

	if (!m_list)
		return ICE_ERR_PARAM;

	rule_lock = &hw->switch_info->recp_list[ICE_SW_LKUP_MAC].filt_rule_lock;
	list_for_each_entry_safe(list_itr, tmp, m_list, list_entry) {
		enum ice_sw_lkup_type l_type = list_itr->fltr_info.lkup_type;
		u8 *add = &list_itr->fltr_info.l_data.mac.mac_addr[0];
		u16 vsi_handle;

		if (l_type != ICE_SW_LKUP_MAC)
			return ICE_ERR_PARAM;

		vsi_handle = list_itr->fltr_info.vsi_handle;
		if (!ice_is_vsi_valid(hw, vsi_handle))
			return ICE_ERR_PARAM;

		list_itr->fltr_info.fwd_id.hw_vsi_id =
					ice_get_hw_vsi_num(hw, vsi_handle);
		if (is_unicast_ether_addr(add) && !hw->ucast_shared) {
			/* Don't remove the unicast address that belongs to
			 * another VSI on the switch, since it is not being
			 * shared...
			 */
			mutex_lock(rule_lock);
			if (!ice_find_ucast_rule_entry(hw, ICE_SW_LKUP_MAC,
						       &list_itr->fltr_info)) {
				mutex_unlock(rule_lock);
				return ICE_ERR_DOES_NOT_EXIST;
			}
			mutex_unlock(rule_lock);
		}
		list_itr->status = ice_remove_rule_internal(hw,
							    ICE_SW_LKUP_MAC,
							    list_itr);
		if (list_itr->status)
			return list_itr->status;
	}
	return 0;
>>>>>>> upstream/android-13
}

/**
 * ice_remove_vlan - Remove VLAN based filter rule
 * @hw: pointer to the hardware structure
 * @v_list: list of VLAN entries and forwarding information
 */
enum ice_status
ice_remove_vlan(struct ice_hw *hw, struct list_head *v_list)
{
<<<<<<< HEAD
	struct ice_fltr_list_entry *v_list_itr;
	enum ice_status status = 0;
=======
	struct ice_fltr_list_entry *v_list_itr, *tmp;
>>>>>>> upstream/android-13

	if (!v_list || !hw)
		return ICE_ERR_PARAM;

<<<<<<< HEAD
	list_for_each_entry(v_list_itr, v_list, list_entry) {
		status = ice_remove_vlan_internal(hw, v_list_itr);
		if (status) {
			v_list_itr->status = ICE_FLTR_STATUS_FW_FAIL;
			return status;
		}
		v_list_itr->status = ICE_FLTR_STATUS_FW_SUCCESS;
	}
	return status;
}

/**
 * ice_add_to_vsi_fltr_list - Add VSI filters to the list
 * @hw: pointer to the hardware structure
 * @vsi_id: ID of VSI to remove filters from
 * @lkup_list_head: pointer to the list that has certain lookup type filters
 * @vsi_list_head: pointer to the list pertaining to VSI with vsi_id
 */
static enum ice_status
ice_add_to_vsi_fltr_list(struct ice_hw *hw, u16 vsi_id,
			 struct list_head *lkup_list_head,
			 struct list_head *vsi_list_head)
{
	struct ice_fltr_mgmt_list_entry *fm_entry;

	/* check to make sure VSI id is valid and within boundary */
	if (vsi_id >=
	    (sizeof(fm_entry->vsi_list_info->vsi_map) * BITS_PER_BYTE - 1))
		return ICE_ERR_PARAM;

	list_for_each_entry(fm_entry, lkup_list_head, list_entry) {
		struct ice_fltr_info *fi;

		fi = &fm_entry->fltr_info;
		if ((fi->fltr_act == ICE_FWD_TO_VSI &&
		     fi->fwd_id.vsi_id == vsi_id) ||
		    (fi->fltr_act == ICE_FWD_TO_VSI_LIST &&
		     (test_bit(vsi_id, fm_entry->vsi_list_info->vsi_map)))) {
			struct ice_fltr_list_entry *tmp;

			/* this memory is freed up in the caller function
			 * ice_remove_vsi_lkup_fltr() once filters for
			 * this VSI are removed
			 */
			tmp = devm_kzalloc(ice_hw_to_dev(hw), sizeof(*tmp),
					   GFP_KERNEL);
			if (!tmp)
				return ICE_ERR_NO_MEMORY;

			memcpy(&tmp->fltr_info, fi, sizeof(*fi));

			/* Expected below fields to be set to ICE_FWD_TO_VSI and
			 * the particular VSI id since we are only removing this
			 * one VSI
			 */
			if (fi->fltr_act == ICE_FWD_TO_VSI_LIST) {
				tmp->fltr_info.fltr_act = ICE_FWD_TO_VSI;
				tmp->fltr_info.fwd_id.vsi_id = vsi_id;
			}

			list_add(&tmp->list_entry, vsi_list_head);
		}
=======
	list_for_each_entry_safe(v_list_itr, tmp, v_list, list_entry) {
		enum ice_sw_lkup_type l_type = v_list_itr->fltr_info.lkup_type;

		if (l_type != ICE_SW_LKUP_VLAN)
			return ICE_ERR_PARAM;
		v_list_itr->status = ice_remove_rule_internal(hw,
							      ICE_SW_LKUP_VLAN,
							      v_list_itr);
		if (v_list_itr->status)
			return v_list_itr->status;
>>>>>>> upstream/android-13
	}
	return 0;
}

/**
<<<<<<< HEAD
 * ice_remove_vsi_lkup_fltr - Remove lookup type filters for a VSI
 * @hw: pointer to the hardware structure
 * @vsi_id: ID of VSI to remove filters from
 * @lkup: switch rule filter lookup type
 */
static void
ice_remove_vsi_lkup_fltr(struct ice_hw *hw, u16 vsi_id,
=======
 * ice_vsi_uses_fltr - Determine if given VSI uses specified filter
 * @fm_entry: filter entry to inspect
 * @vsi_handle: VSI handle to compare with filter info
 */
static bool
ice_vsi_uses_fltr(struct ice_fltr_mgmt_list_entry *fm_entry, u16 vsi_handle)
{
	return ((fm_entry->fltr_info.fltr_act == ICE_FWD_TO_VSI &&
		 fm_entry->fltr_info.vsi_handle == vsi_handle) ||
		(fm_entry->fltr_info.fltr_act == ICE_FWD_TO_VSI_LIST &&
		 fm_entry->vsi_list_info &&
		 (test_bit(vsi_handle, fm_entry->vsi_list_info->vsi_map))));
}

/**
 * ice_add_entry_to_vsi_fltr_list - Add copy of fltr_list_entry to remove list
 * @hw: pointer to the hardware structure
 * @vsi_handle: VSI handle to remove filters from
 * @vsi_list_head: pointer to the list to add entry to
 * @fi: pointer to fltr_info of filter entry to copy & add
 *
 * Helper function, used when creating a list of filters to remove from
 * a specific VSI. The entry added to vsi_list_head is a COPY of the
 * original filter entry, with the exception of fltr_info.fltr_act and
 * fltr_info.fwd_id fields. These are set such that later logic can
 * extract which VSI to remove the fltr from, and pass on that information.
 */
static enum ice_status
ice_add_entry_to_vsi_fltr_list(struct ice_hw *hw, u16 vsi_handle,
			       struct list_head *vsi_list_head,
			       struct ice_fltr_info *fi)
{
	struct ice_fltr_list_entry *tmp;

	/* this memory is freed up in the caller function
	 * once filters for this VSI are removed
	 */
	tmp = devm_kzalloc(ice_hw_to_dev(hw), sizeof(*tmp), GFP_KERNEL);
	if (!tmp)
		return ICE_ERR_NO_MEMORY;

	tmp->fltr_info = *fi;

	/* Overwrite these fields to indicate which VSI to remove filter from,
	 * so find and remove logic can extract the information from the
	 * list entries. Note that original entries will still have proper
	 * values.
	 */
	tmp->fltr_info.fltr_act = ICE_FWD_TO_VSI;
	tmp->fltr_info.vsi_handle = vsi_handle;
	tmp->fltr_info.fwd_id.hw_vsi_id = ice_get_hw_vsi_num(hw, vsi_handle);

	list_add(&tmp->list_entry, vsi_list_head);

	return 0;
}

/**
 * ice_add_to_vsi_fltr_list - Add VSI filters to the list
 * @hw: pointer to the hardware structure
 * @vsi_handle: VSI handle to remove filters from
 * @lkup_list_head: pointer to the list that has certain lookup type filters
 * @vsi_list_head: pointer to the list pertaining to VSI with vsi_handle
 *
 * Locates all filters in lkup_list_head that are used by the given VSI,
 * and adds COPIES of those entries to vsi_list_head (intended to be used
 * to remove the listed filters).
 * Note that this means all entries in vsi_list_head must be explicitly
 * deallocated by the caller when done with list.
 */
static enum ice_status
ice_add_to_vsi_fltr_list(struct ice_hw *hw, u16 vsi_handle,
			 struct list_head *lkup_list_head,
			 struct list_head *vsi_list_head)
{
	struct ice_fltr_mgmt_list_entry *fm_entry;
	enum ice_status status = 0;

	/* check to make sure VSI ID is valid and within boundary */
	if (!ice_is_vsi_valid(hw, vsi_handle))
		return ICE_ERR_PARAM;

	list_for_each_entry(fm_entry, lkup_list_head, list_entry) {
		if (!ice_vsi_uses_fltr(fm_entry, vsi_handle))
			continue;

		status = ice_add_entry_to_vsi_fltr_list(hw, vsi_handle,
							vsi_list_head,
							&fm_entry->fltr_info);
		if (status)
			return status;
	}
	return status;
}

/**
 * ice_determine_promisc_mask
 * @fi: filter info to parse
 *
 * Helper function to determine which ICE_PROMISC_ mask corresponds
 * to given filter into.
 */
static u8 ice_determine_promisc_mask(struct ice_fltr_info *fi)
{
	u16 vid = fi->l_data.mac_vlan.vlan_id;
	u8 *macaddr = fi->l_data.mac.mac_addr;
	bool is_tx_fltr = false;
	u8 promisc_mask = 0;

	if (fi->flag == ICE_FLTR_TX)
		is_tx_fltr = true;

	if (is_broadcast_ether_addr(macaddr))
		promisc_mask |= is_tx_fltr ?
			ICE_PROMISC_BCAST_TX : ICE_PROMISC_BCAST_RX;
	else if (is_multicast_ether_addr(macaddr))
		promisc_mask |= is_tx_fltr ?
			ICE_PROMISC_MCAST_TX : ICE_PROMISC_MCAST_RX;
	else if (is_unicast_ether_addr(macaddr))
		promisc_mask |= is_tx_fltr ?
			ICE_PROMISC_UCAST_TX : ICE_PROMISC_UCAST_RX;
	if (vid)
		promisc_mask |= is_tx_fltr ?
			ICE_PROMISC_VLAN_TX : ICE_PROMISC_VLAN_RX;

	return promisc_mask;
}

/**
 * ice_remove_promisc - Remove promisc based filter rules
 * @hw: pointer to the hardware structure
 * @recp_id: recipe ID for which the rule needs to removed
 * @v_list: list of promisc entries
 */
static enum ice_status
ice_remove_promisc(struct ice_hw *hw, u8 recp_id,
		   struct list_head *v_list)
{
	struct ice_fltr_list_entry *v_list_itr, *tmp;

	list_for_each_entry_safe(v_list_itr, tmp, v_list, list_entry) {
		v_list_itr->status =
			ice_remove_rule_internal(hw, recp_id, v_list_itr);
		if (v_list_itr->status)
			return v_list_itr->status;
	}
	return 0;
}

/**
 * ice_clear_vsi_promisc - clear specified promiscuous mode(s) for given VSI
 * @hw: pointer to the hardware structure
 * @vsi_handle: VSI handle to clear mode
 * @promisc_mask: mask of promiscuous config bits to clear
 * @vid: VLAN ID to clear VLAN promiscuous
 */
enum ice_status
ice_clear_vsi_promisc(struct ice_hw *hw, u16 vsi_handle, u8 promisc_mask,
		      u16 vid)
{
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_list_entry *fm_entry, *tmp;
	struct list_head remove_list_head;
	struct ice_fltr_mgmt_list_entry *itr;
	struct list_head *rule_head;
	struct mutex *rule_lock;	/* Lock to protect filter rule list */
	enum ice_status status = 0;
	u8 recipe_id;

	if (!ice_is_vsi_valid(hw, vsi_handle))
		return ICE_ERR_PARAM;

	if (promisc_mask & (ICE_PROMISC_VLAN_RX | ICE_PROMISC_VLAN_TX))
		recipe_id = ICE_SW_LKUP_PROMISC_VLAN;
	else
		recipe_id = ICE_SW_LKUP_PROMISC;

	rule_head = &sw->recp_list[recipe_id].filt_rules;
	rule_lock = &sw->recp_list[recipe_id].filt_rule_lock;

	INIT_LIST_HEAD(&remove_list_head);

	mutex_lock(rule_lock);
	list_for_each_entry(itr, rule_head, list_entry) {
		struct ice_fltr_info *fltr_info;
		u8 fltr_promisc_mask = 0;

		if (!ice_vsi_uses_fltr(itr, vsi_handle))
			continue;
		fltr_info = &itr->fltr_info;

		if (recipe_id == ICE_SW_LKUP_PROMISC_VLAN &&
		    vid != fltr_info->l_data.mac_vlan.vlan_id)
			continue;

		fltr_promisc_mask |= ice_determine_promisc_mask(fltr_info);

		/* Skip if filter is not completely specified by given mask */
		if (fltr_promisc_mask & ~promisc_mask)
			continue;

		status = ice_add_entry_to_vsi_fltr_list(hw, vsi_handle,
							&remove_list_head,
							fltr_info);
		if (status) {
			mutex_unlock(rule_lock);
			goto free_fltr_list;
		}
	}
	mutex_unlock(rule_lock);

	status = ice_remove_promisc(hw, recipe_id, &remove_list_head);

free_fltr_list:
	list_for_each_entry_safe(fm_entry, tmp, &remove_list_head, list_entry) {
		list_del(&fm_entry->list_entry);
		devm_kfree(ice_hw_to_dev(hw), fm_entry);
	}

	return status;
}

/**
 * ice_set_vsi_promisc - set given VSI to given promiscuous mode(s)
 * @hw: pointer to the hardware structure
 * @vsi_handle: VSI handle to configure
 * @promisc_mask: mask of promiscuous config bits
 * @vid: VLAN ID to set VLAN promiscuous
 */
enum ice_status
ice_set_vsi_promisc(struct ice_hw *hw, u16 vsi_handle, u8 promisc_mask, u16 vid)
{
	enum { UCAST_FLTR = 1, MCAST_FLTR, BCAST_FLTR };
	struct ice_fltr_list_entry f_list_entry;
	struct ice_fltr_info new_fltr;
	enum ice_status status = 0;
	bool is_tx_fltr;
	u16 hw_vsi_id;
	int pkt_type;
	u8 recipe_id;

	if (!ice_is_vsi_valid(hw, vsi_handle))
		return ICE_ERR_PARAM;
	hw_vsi_id = ice_get_hw_vsi_num(hw, vsi_handle);

	memset(&new_fltr, 0, sizeof(new_fltr));

	if (promisc_mask & (ICE_PROMISC_VLAN_RX | ICE_PROMISC_VLAN_TX)) {
		new_fltr.lkup_type = ICE_SW_LKUP_PROMISC_VLAN;
		new_fltr.l_data.mac_vlan.vlan_id = vid;
		recipe_id = ICE_SW_LKUP_PROMISC_VLAN;
	} else {
		new_fltr.lkup_type = ICE_SW_LKUP_PROMISC;
		recipe_id = ICE_SW_LKUP_PROMISC;
	}

	/* Separate filters must be set for each direction/packet type
	 * combination, so we will loop over the mask value, store the
	 * individual type, and clear it out in the input mask as it
	 * is found.
	 */
	while (promisc_mask) {
		u8 *mac_addr;

		pkt_type = 0;
		is_tx_fltr = false;

		if (promisc_mask & ICE_PROMISC_UCAST_RX) {
			promisc_mask &= ~ICE_PROMISC_UCAST_RX;
			pkt_type = UCAST_FLTR;
		} else if (promisc_mask & ICE_PROMISC_UCAST_TX) {
			promisc_mask &= ~ICE_PROMISC_UCAST_TX;
			pkt_type = UCAST_FLTR;
			is_tx_fltr = true;
		} else if (promisc_mask & ICE_PROMISC_MCAST_RX) {
			promisc_mask &= ~ICE_PROMISC_MCAST_RX;
			pkt_type = MCAST_FLTR;
		} else if (promisc_mask & ICE_PROMISC_MCAST_TX) {
			promisc_mask &= ~ICE_PROMISC_MCAST_TX;
			pkt_type = MCAST_FLTR;
			is_tx_fltr = true;
		} else if (promisc_mask & ICE_PROMISC_BCAST_RX) {
			promisc_mask &= ~ICE_PROMISC_BCAST_RX;
			pkt_type = BCAST_FLTR;
		} else if (promisc_mask & ICE_PROMISC_BCAST_TX) {
			promisc_mask &= ~ICE_PROMISC_BCAST_TX;
			pkt_type = BCAST_FLTR;
			is_tx_fltr = true;
		}

		/* Check for VLAN promiscuous flag */
		if (promisc_mask & ICE_PROMISC_VLAN_RX) {
			promisc_mask &= ~ICE_PROMISC_VLAN_RX;
		} else if (promisc_mask & ICE_PROMISC_VLAN_TX) {
			promisc_mask &= ~ICE_PROMISC_VLAN_TX;
			is_tx_fltr = true;
		}

		/* Set filter DA based on packet type */
		mac_addr = new_fltr.l_data.mac.mac_addr;
		if (pkt_type == BCAST_FLTR) {
			eth_broadcast_addr(mac_addr);
		} else if (pkt_type == MCAST_FLTR ||
			   pkt_type == UCAST_FLTR) {
			/* Use the dummy ether header DA */
			ether_addr_copy(mac_addr, dummy_eth_header);
			if (pkt_type == MCAST_FLTR)
				mac_addr[0] |= 0x1;	/* Set multicast bit */
		}

		/* Need to reset this to zero for all iterations */
		new_fltr.flag = 0;
		if (is_tx_fltr) {
			new_fltr.flag |= ICE_FLTR_TX;
			new_fltr.src = hw_vsi_id;
		} else {
			new_fltr.flag |= ICE_FLTR_RX;
			new_fltr.src = hw->port_info->lport;
		}

		new_fltr.fltr_act = ICE_FWD_TO_VSI;
		new_fltr.vsi_handle = vsi_handle;
		new_fltr.fwd_id.hw_vsi_id = hw_vsi_id;
		f_list_entry.fltr_info = new_fltr;

		status = ice_add_rule_internal(hw, recipe_id, &f_list_entry);
		if (status)
			goto set_promisc_exit;
	}

set_promisc_exit:
	return status;
}

/**
 * ice_set_vlan_vsi_promisc
 * @hw: pointer to the hardware structure
 * @vsi_handle: VSI handle to configure
 * @promisc_mask: mask of promiscuous config bits
 * @rm_vlan_promisc: Clear VLANs VSI promisc mode
 *
 * Configure VSI with all associated VLANs to given promiscuous mode(s)
 */
enum ice_status
ice_set_vlan_vsi_promisc(struct ice_hw *hw, u16 vsi_handle, u8 promisc_mask,
			 bool rm_vlan_promisc)
{
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_list_entry *list_itr, *tmp;
	struct list_head vsi_list_head;
	struct list_head *vlan_head;
	struct mutex *vlan_lock; /* Lock to protect filter rule list */
	enum ice_status status;
	u16 vlan_id;

	INIT_LIST_HEAD(&vsi_list_head);
	vlan_lock = &sw->recp_list[ICE_SW_LKUP_VLAN].filt_rule_lock;
	vlan_head = &sw->recp_list[ICE_SW_LKUP_VLAN].filt_rules;
	mutex_lock(vlan_lock);
	status = ice_add_to_vsi_fltr_list(hw, vsi_handle, vlan_head,
					  &vsi_list_head);
	mutex_unlock(vlan_lock);
	if (status)
		goto free_fltr_list;

	list_for_each_entry(list_itr, &vsi_list_head, list_entry) {
		vlan_id = list_itr->fltr_info.l_data.vlan.vlan_id;
		if (rm_vlan_promisc)
			status = ice_clear_vsi_promisc(hw, vsi_handle,
						       promisc_mask, vlan_id);
		else
			status = ice_set_vsi_promisc(hw, vsi_handle,
						     promisc_mask, vlan_id);
		if (status)
			break;
	}

free_fltr_list:
	list_for_each_entry_safe(list_itr, tmp, &vsi_list_head, list_entry) {
		list_del(&list_itr->list_entry);
		devm_kfree(ice_hw_to_dev(hw), list_itr);
	}
	return status;
}

/**
 * ice_remove_vsi_lkup_fltr - Remove lookup type filters for a VSI
 * @hw: pointer to the hardware structure
 * @vsi_handle: VSI handle to remove filters from
 * @lkup: switch rule filter lookup type
 */
static void
ice_remove_vsi_lkup_fltr(struct ice_hw *hw, u16 vsi_handle,
>>>>>>> upstream/android-13
			 enum ice_sw_lkup_type lkup)
{
	struct ice_switch_info *sw = hw->switch_info;
	struct ice_fltr_list_entry *fm_entry;
	struct list_head remove_list_head;
<<<<<<< HEAD
	struct ice_fltr_list_entry *tmp;
	enum ice_status status;

	INIT_LIST_HEAD(&remove_list_head);
	switch (lkup) {
	case ICE_SW_LKUP_MAC:
		mutex_lock(&sw->mac_list_lock);
		status = ice_add_to_vsi_fltr_list(hw, vsi_id,
						  &sw->mac_list_head,
						  &remove_list_head);
		mutex_unlock(&sw->mac_list_lock);
		if (!status) {
			ice_remove_mac(hw, &remove_list_head);
			goto free_fltr_list;
		}
		break;
	case ICE_SW_LKUP_VLAN:
		mutex_lock(&sw->vlan_list_lock);
		status = ice_add_to_vsi_fltr_list(hw, vsi_id,
						  &sw->vlan_list_head,
						  &remove_list_head);
		mutex_unlock(&sw->vlan_list_lock);
		if (!status) {
			ice_remove_vlan(hw, &remove_list_head);
			goto free_fltr_list;
		}
=======
	struct list_head *rule_head;
	struct ice_fltr_list_entry *tmp;
	struct mutex *rule_lock;	/* Lock to protect filter rule list */
	enum ice_status status;

	INIT_LIST_HEAD(&remove_list_head);
	rule_lock = &sw->recp_list[lkup].filt_rule_lock;
	rule_head = &sw->recp_list[lkup].filt_rules;
	mutex_lock(rule_lock);
	status = ice_add_to_vsi_fltr_list(hw, vsi_handle, rule_head,
					  &remove_list_head);
	mutex_unlock(rule_lock);
	if (status)
		goto free_fltr_list;

	switch (lkup) {
	case ICE_SW_LKUP_MAC:
		ice_remove_mac(hw, &remove_list_head);
		break;
	case ICE_SW_LKUP_VLAN:
		ice_remove_vlan(hw, &remove_list_head);
		break;
	case ICE_SW_LKUP_PROMISC:
	case ICE_SW_LKUP_PROMISC_VLAN:
		ice_remove_promisc(hw, lkup, &remove_list_head);
>>>>>>> upstream/android-13
		break;
	case ICE_SW_LKUP_MAC_VLAN:
	case ICE_SW_LKUP_ETHERTYPE:
	case ICE_SW_LKUP_ETHERTYPE_MAC:
<<<<<<< HEAD
	case ICE_SW_LKUP_PROMISC:
	case ICE_SW_LKUP_PROMISC_VLAN:
	case ICE_SW_LKUP_DFLT:
		ice_debug(hw, ICE_DBG_SW,
			  "Remove filters for this lookup type hasn't been implemented yet\n");
		break;
	}

	return;
=======
	case ICE_SW_LKUP_DFLT:
	case ICE_SW_LKUP_LAST:
	default:
		ice_debug(hw, ICE_DBG_SW, "Unsupported lookup type %d\n", lkup);
		break;
	}

>>>>>>> upstream/android-13
free_fltr_list:
	list_for_each_entry_safe(fm_entry, tmp, &remove_list_head, list_entry) {
		list_del(&fm_entry->list_entry);
		devm_kfree(ice_hw_to_dev(hw), fm_entry);
	}
}

/**
 * ice_remove_vsi_fltr - Remove all filters for a VSI
 * @hw: pointer to the hardware structure
<<<<<<< HEAD
 * @vsi_id: ID of VSI to remove filters from
 */
void ice_remove_vsi_fltr(struct ice_hw *hw, u16 vsi_id)
{
	ice_remove_vsi_lkup_fltr(hw, vsi_id, ICE_SW_LKUP_MAC);
	ice_remove_vsi_lkup_fltr(hw, vsi_id, ICE_SW_LKUP_MAC_VLAN);
	ice_remove_vsi_lkup_fltr(hw, vsi_id, ICE_SW_LKUP_PROMISC);
	ice_remove_vsi_lkup_fltr(hw, vsi_id, ICE_SW_LKUP_VLAN);
	ice_remove_vsi_lkup_fltr(hw, vsi_id, ICE_SW_LKUP_DFLT);
	ice_remove_vsi_lkup_fltr(hw, vsi_id, ICE_SW_LKUP_ETHERTYPE);
	ice_remove_vsi_lkup_fltr(hw, vsi_id, ICE_SW_LKUP_ETHERTYPE_MAC);
	ice_remove_vsi_lkup_fltr(hw, vsi_id, ICE_SW_LKUP_PROMISC_VLAN);
=======
 * @vsi_handle: VSI handle to remove filters from
 */
void ice_remove_vsi_fltr(struct ice_hw *hw, u16 vsi_handle)
{
	ice_remove_vsi_lkup_fltr(hw, vsi_handle, ICE_SW_LKUP_MAC);
	ice_remove_vsi_lkup_fltr(hw, vsi_handle, ICE_SW_LKUP_MAC_VLAN);
	ice_remove_vsi_lkup_fltr(hw, vsi_handle, ICE_SW_LKUP_PROMISC);
	ice_remove_vsi_lkup_fltr(hw, vsi_handle, ICE_SW_LKUP_VLAN);
	ice_remove_vsi_lkup_fltr(hw, vsi_handle, ICE_SW_LKUP_DFLT);
	ice_remove_vsi_lkup_fltr(hw, vsi_handle, ICE_SW_LKUP_ETHERTYPE);
	ice_remove_vsi_lkup_fltr(hw, vsi_handle, ICE_SW_LKUP_ETHERTYPE_MAC);
	ice_remove_vsi_lkup_fltr(hw, vsi_handle, ICE_SW_LKUP_PROMISC_VLAN);
}

/**
 * ice_alloc_res_cntr - allocating resource counter
 * @hw: pointer to the hardware structure
 * @type: type of resource
 * @alloc_shared: if set it is shared else dedicated
 * @num_items: number of entries requested for FD resource type
 * @counter_id: counter index returned by AQ call
 */
enum ice_status
ice_alloc_res_cntr(struct ice_hw *hw, u8 type, u8 alloc_shared, u16 num_items,
		   u16 *counter_id)
{
	struct ice_aqc_alloc_free_res_elem *buf;
	enum ice_status status;
	u16 buf_len;

	/* Allocate resource */
	buf_len = struct_size(buf, elem, 1);
	buf = kzalloc(buf_len, GFP_KERNEL);
	if (!buf)
		return ICE_ERR_NO_MEMORY;

	buf->num_elems = cpu_to_le16(num_items);
	buf->res_type = cpu_to_le16(((type << ICE_AQC_RES_TYPE_S) &
				      ICE_AQC_RES_TYPE_M) | alloc_shared);

	status = ice_aq_alloc_free_res(hw, 1, buf, buf_len,
				       ice_aqc_opc_alloc_res, NULL);
	if (status)
		goto exit;

	*counter_id = le16_to_cpu(buf->elem[0].e.sw_resp);

exit:
	kfree(buf);
	return status;
}

/**
 * ice_free_res_cntr - free resource counter
 * @hw: pointer to the hardware structure
 * @type: type of resource
 * @alloc_shared: if set it is shared else dedicated
 * @num_items: number of entries to be freed for FD resource type
 * @counter_id: counter ID resource which needs to be freed
 */
enum ice_status
ice_free_res_cntr(struct ice_hw *hw, u8 type, u8 alloc_shared, u16 num_items,
		  u16 counter_id)
{
	struct ice_aqc_alloc_free_res_elem *buf;
	enum ice_status status;
	u16 buf_len;

	/* Free resource */
	buf_len = struct_size(buf, elem, 1);
	buf = kzalloc(buf_len, GFP_KERNEL);
	if (!buf)
		return ICE_ERR_NO_MEMORY;

	buf->num_elems = cpu_to_le16(num_items);
	buf->res_type = cpu_to_le16(((type << ICE_AQC_RES_TYPE_S) &
				      ICE_AQC_RES_TYPE_M) | alloc_shared);
	buf->elem[0].e.sw_resp = cpu_to_le16(counter_id);

	status = ice_aq_alloc_free_res(hw, 1, buf, buf_len,
				       ice_aqc_opc_free_res, NULL);
	if (status)
		ice_debug(hw, ICE_DBG_SW, "counter resource could not be freed\n");

	kfree(buf);
	return status;
}

/**
 * ice_replay_vsi_fltr - Replay filters for requested VSI
 * @hw: pointer to the hardware structure
 * @vsi_handle: driver VSI handle
 * @recp_id: Recipe ID for which rules need to be replayed
 * @list_head: list for which filters need to be replayed
 *
 * Replays the filter of recipe recp_id for a VSI represented via vsi_handle.
 * It is required to pass valid VSI handle.
 */
static enum ice_status
ice_replay_vsi_fltr(struct ice_hw *hw, u16 vsi_handle, u8 recp_id,
		    struct list_head *list_head)
{
	struct ice_fltr_mgmt_list_entry *itr;
	enum ice_status status = 0;
	u16 hw_vsi_id;

	if (list_empty(list_head))
		return status;
	hw_vsi_id = ice_get_hw_vsi_num(hw, vsi_handle);

	list_for_each_entry(itr, list_head, list_entry) {
		struct ice_fltr_list_entry f_entry;

		f_entry.fltr_info = itr->fltr_info;
		if (itr->vsi_count < 2 && recp_id != ICE_SW_LKUP_VLAN &&
		    itr->fltr_info.vsi_handle == vsi_handle) {
			/* update the src in case it is VSI num */
			if (f_entry.fltr_info.src_id == ICE_SRC_ID_VSI)
				f_entry.fltr_info.src = hw_vsi_id;
			status = ice_add_rule_internal(hw, recp_id, &f_entry);
			if (status)
				goto end;
			continue;
		}
		if (!itr->vsi_list_info ||
		    !test_bit(vsi_handle, itr->vsi_list_info->vsi_map))
			continue;
		/* Clearing it so that the logic can add it back */
		clear_bit(vsi_handle, itr->vsi_list_info->vsi_map);
		f_entry.fltr_info.vsi_handle = vsi_handle;
		f_entry.fltr_info.fltr_act = ICE_FWD_TO_VSI;
		/* update the src in case it is VSI num */
		if (f_entry.fltr_info.src_id == ICE_SRC_ID_VSI)
			f_entry.fltr_info.src = hw_vsi_id;
		if (recp_id == ICE_SW_LKUP_VLAN)
			status = ice_add_vlan_internal(hw, &f_entry);
		else
			status = ice_add_rule_internal(hw, recp_id, &f_entry);
		if (status)
			goto end;
	}
end:
	return status;
}

/**
 * ice_replay_vsi_all_fltr - replay all filters stored in bookkeeping lists
 * @hw: pointer to the hardware structure
 * @vsi_handle: driver VSI handle
 *
 * Replays filters for requested VSI via vsi_handle.
 */
enum ice_status ice_replay_vsi_all_fltr(struct ice_hw *hw, u16 vsi_handle)
{
	struct ice_switch_info *sw = hw->switch_info;
	enum ice_status status = 0;
	u8 i;

	for (i = 0; i < ICE_SW_LKUP_LAST; i++) {
		struct list_head *head;

		head = &sw->recp_list[i].filt_replay_rules;
		status = ice_replay_vsi_fltr(hw, vsi_handle, i, head);
		if (status)
			return status;
	}
	return status;
}

/**
 * ice_rm_all_sw_replay_rule_info - deletes filter replay rules
 * @hw: pointer to the HW struct
 *
 * Deletes the filter replay rules.
 */
void ice_rm_all_sw_replay_rule_info(struct ice_hw *hw)
{
	struct ice_switch_info *sw = hw->switch_info;
	u8 i;

	if (!sw)
		return;

	for (i = 0; i < ICE_SW_LKUP_LAST; i++) {
		if (!list_empty(&sw->recp_list[i].filt_replay_rules)) {
			struct list_head *l_head;

			l_head = &sw->recp_list[i].filt_replay_rules;
			ice_rem_sw_rule_info(hw, l_head);
		}
	}
>>>>>>> upstream/android-13
}
