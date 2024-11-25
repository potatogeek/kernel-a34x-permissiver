// SPDX-License-Identifier: GPL-2.0+
// Copyright (c) 2016-2017 Hisilicon Limited.

#include "hclge_main.h"
#include "hclge_mbx.h"
#include "hnae3.h"

<<<<<<< HEAD
=======
#define CREATE_TRACE_POINTS
#include "hclge_trace.h"

static u16 hclge_errno_to_resp(int errno)
{
	int resp = abs(errno);

	/* The status for pf to vf msg cmd is u16, constrainted by HW.
	 * We need to keep the same type with it.
	 * The intput errno is the stander error code, it's safely to
	 * use a u16 to store the abs(errno).
	 */
	return (u16)resp;
}

>>>>>>> upstream/android-13
/* hclge_gen_resp_to_vf: used to generate a synchronous response to VF when PF
 * receives a mailbox message from VF.
 * @vport: pointer to struct hclge_vport
 * @vf_to_pf_req: pointer to hclge_mbx_vf_to_pf_cmd of the original mailbox
 *		  message
 * @resp_status: indicate to VF whether its request success(0) or failed.
 */
static int hclge_gen_resp_to_vf(struct hclge_vport *vport,
				struct hclge_mbx_vf_to_pf_cmd *vf_to_pf_req,
<<<<<<< HEAD
				int resp_status,
				u8 *resp_data, u16 resp_data_len)
=======
				struct hclge_respond_to_vf_msg *resp_msg)
>>>>>>> upstream/android-13
{
	struct hclge_mbx_pf_to_vf_cmd *resp_pf_to_vf;
	struct hclge_dev *hdev = vport->back;
	enum hclge_cmd_status status;
	struct hclge_desc desc;
<<<<<<< HEAD

	resp_pf_to_vf = (struct hclge_mbx_pf_to_vf_cmd *)desc.data;

	if (resp_data_len > HCLGE_MBX_MAX_RESP_DATA_SIZE) {
		dev_err(&hdev->pdev->dev,
			"PF fail to gen resp to VF len %d exceeds max len %d\n",
			resp_data_len,
			HCLGE_MBX_MAX_RESP_DATA_SIZE);
=======
	u16 resp;

	resp_pf_to_vf = (struct hclge_mbx_pf_to_vf_cmd *)desc.data;

	if (resp_msg->len > HCLGE_MBX_MAX_RESP_DATA_SIZE) {
		dev_err(&hdev->pdev->dev,
			"PF fail to gen resp to VF len %u exceeds max len %u\n",
			resp_msg->len,
			HCLGE_MBX_MAX_RESP_DATA_SIZE);
		/* If resp_msg->len is too long, set the value to max length
		 * and return the msg to VF
		 */
		resp_msg->len = HCLGE_MBX_MAX_RESP_DATA_SIZE;
>>>>>>> upstream/android-13
	}

	hclge_cmd_setup_basic_desc(&desc, HCLGEVF_OPC_MBX_PF_TO_VF, false);

	resp_pf_to_vf->dest_vfid = vf_to_pf_req->mbx_src_vfid;
	resp_pf_to_vf->msg_len = vf_to_pf_req->msg_len;
<<<<<<< HEAD

	resp_pf_to_vf->msg[0] = HCLGE_MBX_PF_VF_RESP;
	resp_pf_to_vf->msg[1] = vf_to_pf_req->msg[0];
	resp_pf_to_vf->msg[2] = vf_to_pf_req->msg[1];
	resp_pf_to_vf->msg[3] = (resp_status == 0) ? 0 : 1;

	if (resp_data && resp_data_len > 0)
		memcpy(&resp_pf_to_vf->msg[4], resp_data, resp_data_len);
=======
	resp_pf_to_vf->match_id = vf_to_pf_req->match_id;

	resp_pf_to_vf->msg.code = HCLGE_MBX_PF_VF_RESP;
	resp_pf_to_vf->msg.vf_mbx_msg_code = vf_to_pf_req->msg.code;
	resp_pf_to_vf->msg.vf_mbx_msg_subcode = vf_to_pf_req->msg.subcode;
	resp = hclge_errno_to_resp(resp_msg->status);
	if (resp < SHRT_MAX) {
		resp_pf_to_vf->msg.resp_status = resp;
	} else {
		dev_warn(&hdev->pdev->dev,
			 "failed to send response to VF, response status %u is out-of-bound\n",
			 resp);
		resp_pf_to_vf->msg.resp_status = EIO;
	}

	if (resp_msg->len > 0)
		memcpy(resp_pf_to_vf->msg.resp_data, resp_msg->data,
		       resp_msg->len);

	trace_hclge_pf_mbx_send(hdev, resp_pf_to_vf);
>>>>>>> upstream/android-13

	status = hclge_cmd_send(&hdev->hw, &desc, 1);
	if (status)
		dev_err(&hdev->pdev->dev,
<<<<<<< HEAD
			"PF failed(=%d) to send response to VF\n", status);
=======
			"failed to send response to VF, status: %d, vfid: %u, code: %u, subcode: %u.\n",
			status, vf_to_pf_req->mbx_src_vfid,
			vf_to_pf_req->msg.code, vf_to_pf_req->msg.subcode);
>>>>>>> upstream/android-13

	return status;
}

static int hclge_send_mbx_msg(struct hclge_vport *vport, u8 *msg, u16 msg_len,
			      u16 mbx_opcode, u8 dest_vfid)
{
	struct hclge_mbx_pf_to_vf_cmd *resp_pf_to_vf;
	struct hclge_dev *hdev = vport->back;
	enum hclge_cmd_status status;
	struct hclge_desc desc;

<<<<<<< HEAD
=======
	if (msg_len > HCLGE_MBX_MAX_MSG_SIZE) {
		dev_err(&hdev->pdev->dev,
			"msg data length(=%u) exceeds maximum(=%u)\n",
			msg_len, HCLGE_MBX_MAX_MSG_SIZE);
		return -EMSGSIZE;
	}

>>>>>>> upstream/android-13
	resp_pf_to_vf = (struct hclge_mbx_pf_to_vf_cmd *)desc.data;

	hclge_cmd_setup_basic_desc(&desc, HCLGEVF_OPC_MBX_PF_TO_VF, false);

	resp_pf_to_vf->dest_vfid = dest_vfid;
	resp_pf_to_vf->msg_len = msg_len;
<<<<<<< HEAD
	resp_pf_to_vf->msg[0] = mbx_opcode;

	memcpy(&resp_pf_to_vf->msg[1], msg, msg_len);
=======
	resp_pf_to_vf->msg.code = mbx_opcode;

	memcpy(&resp_pf_to_vf->msg.vf_mbx_msg_code, msg, msg_len);

	trace_hclge_pf_mbx_send(hdev, resp_pf_to_vf);
>>>>>>> upstream/android-13

	status = hclge_cmd_send(&hdev->hw, &desc, 1);
	if (status)
		dev_err(&hdev->pdev->dev,
<<<<<<< HEAD
			"PF failed(=%d) to send mailbox message to VF\n",
			status);
=======
			"failed to send mailbox to VF, status: %d, vfid: %u, opcode: %u\n",
			status, dest_vfid, mbx_opcode);
>>>>>>> upstream/android-13

	return status;
}

<<<<<<< HEAD
static int hclge_inform_reset_assert_to_vf(struct hclge_vport *vport)
{
	u8 msg_data[2];
	u8 dest_vfid;

	dest_vfid = (u8)vport->vport_id;

	/* send this requested info to VF */
	return hclge_send_mbx_msg(vport, msg_data, sizeof(u8),
=======
int hclge_inform_reset_assert_to_vf(struct hclge_vport *vport)
{
	struct hclge_dev *hdev = vport->back;
	u16 reset_type;
	u8 msg_data[2];
	u8 dest_vfid;

	BUILD_BUG_ON(HNAE3_MAX_RESET > U16_MAX);

	dest_vfid = (u8)vport->vport_id;

	if (hdev->reset_type == HNAE3_FUNC_RESET)
		reset_type = HNAE3_VF_PF_FUNC_RESET;
	else if (hdev->reset_type == HNAE3_FLR_RESET)
		reset_type = HNAE3_VF_FULL_RESET;
	else
		reset_type = HNAE3_VF_FUNC_RESET;

	memcpy(&msg_data[0], &reset_type, sizeof(u16));

	/* send this requested info to VF */
	return hclge_send_mbx_msg(vport, msg_data, sizeof(msg_data),
>>>>>>> upstream/android-13
				  HCLGE_MBX_ASSERTING_RESET, dest_vfid);
}

static void hclge_free_vector_ring_chain(struct hnae3_ring_chain_node *head)
{
	struct hnae3_ring_chain_node *chain_tmp, *chain;

	chain = head->next;

	while (chain) {
		chain_tmp = chain->next;
<<<<<<< HEAD
		kzfree(chain);
=======
		kfree_sensitive(chain);
>>>>>>> upstream/android-13
		chain = chain_tmp;
	}
}

/* hclge_get_ring_chain_from_mbx: get ring type & tqp id & int_gl idx
 * from mailbox message
 * msg[0]: opcode
 * msg[1]: <not relevant to this function>
 * msg[2]: ring_num
 * msg[3]: first ring type (TX|RX)
 * msg[4]: first tqp id
 * msg[5]: first int_gl idx
 * msg[6] ~ msg[14]: other ring type, tqp id and int_gl idx
 */
static int hclge_get_ring_chain_from_mbx(
			struct hclge_mbx_vf_to_pf_cmd *req,
			struct hnae3_ring_chain_node *ring_chain,
			struct hclge_vport *vport)
{
	struct hnae3_ring_chain_node *cur_chain, *new_chain;
<<<<<<< HEAD
	int ring_num;
	int i;

	ring_num = req->msg[2];

	if (ring_num > ((HCLGE_MBX_VF_MSG_DATA_NUM -
		HCLGE_MBX_RING_MAP_BASIC_MSG_NUM) /
		HCLGE_MBX_RING_NODE_VARIABLE_NUM))
		return -ENOMEM;

	hnae3_set_bit(ring_chain->flag, HNAE3_RING_TYPE_B, req->msg[3]);
	ring_chain->tqp_index =
			hclge_get_queue_id(vport->nic.kinfo.tqp[req->msg[4]]);
	hnae3_set_field(ring_chain->int_gl_idx, HNAE3_RING_GL_IDX_M,
			HNAE3_RING_GL_IDX_S,
			req->msg[5]);
=======
	struct hclge_dev *hdev = vport->back;
	int ring_num;
	int i;

	ring_num = req->msg.ring_num;

	if (ring_num > HCLGE_MBX_MAX_RING_CHAIN_PARAM_NUM)
		return -EINVAL;

	for (i = 0; i < ring_num; i++) {
		if (req->msg.param[i].tqp_index >= vport->nic.kinfo.rss_size) {
			dev_err(&hdev->pdev->dev, "tqp index(%u) is out of range(0-%u)\n",
				req->msg.param[i].tqp_index,
				vport->nic.kinfo.rss_size - 1);
			return -EINVAL;
		}
	}

	hnae3_set_bit(ring_chain->flag, HNAE3_RING_TYPE_B,
		      req->msg.param[0].ring_type);
	ring_chain->tqp_index =
		hclge_get_queue_id(vport->nic.kinfo.tqp
				   [req->msg.param[0].tqp_index]);
	hnae3_set_field(ring_chain->int_gl_idx, HNAE3_RING_GL_IDX_M,
			HNAE3_RING_GL_IDX_S, req->msg.param[0].int_gl_index);
>>>>>>> upstream/android-13

	cur_chain = ring_chain;

	for (i = 1; i < ring_num; i++) {
		new_chain = kzalloc(sizeof(*new_chain), GFP_KERNEL);
		if (!new_chain)
			goto err;

		hnae3_set_bit(new_chain->flag, HNAE3_RING_TYPE_B,
<<<<<<< HEAD
			      req->msg[HCLGE_MBX_RING_NODE_VARIABLE_NUM * i +
			      HCLGE_MBX_RING_MAP_BASIC_MSG_NUM]);

		new_chain->tqp_index =
		hclge_get_queue_id(vport->nic.kinfo.tqp
			[req->msg[HCLGE_MBX_RING_NODE_VARIABLE_NUM * i +
			HCLGE_MBX_RING_MAP_BASIC_MSG_NUM + 1]]);

		hnae3_set_field(new_chain->int_gl_idx, HNAE3_RING_GL_IDX_M,
				HNAE3_RING_GL_IDX_S,
				req->msg[HCLGE_MBX_RING_NODE_VARIABLE_NUM * i +
				HCLGE_MBX_RING_MAP_BASIC_MSG_NUM + 2]);
=======
			      req->msg.param[i].ring_type);

		new_chain->tqp_index =
		hclge_get_queue_id(vport->nic.kinfo.tqp
			[req->msg.param[i].tqp_index]);

		hnae3_set_field(new_chain->int_gl_idx, HNAE3_RING_GL_IDX_M,
				HNAE3_RING_GL_IDX_S,
				req->msg.param[i].int_gl_index);
>>>>>>> upstream/android-13

		cur_chain->next = new_chain;
		cur_chain = new_chain;
	}

	return 0;
err:
	hclge_free_vector_ring_chain(ring_chain);
	return -ENOMEM;
}

static int hclge_map_unmap_ring_to_vf_vector(struct hclge_vport *vport, bool en,
					     struct hclge_mbx_vf_to_pf_cmd *req)
{
	struct hnae3_ring_chain_node ring_chain;
<<<<<<< HEAD
	int vector_id = req->msg[1];
=======
	int vector_id = req->msg.vector_id;
>>>>>>> upstream/android-13
	int ret;

	memset(&ring_chain, 0, sizeof(ring_chain));
	ret = hclge_get_ring_chain_from_mbx(req, &ring_chain, vport);
	if (ret)
		return ret;

	ret = hclge_bind_ring_with_vector(vport, vector_id, en, &ring_chain);

	hclge_free_vector_ring_chain(&ring_chain);

	return ret;
}

<<<<<<< HEAD
static int hclge_set_vf_promisc_mode(struct hclge_vport *vport,
				     struct hclge_mbx_vf_to_pf_cmd *req)
{
	bool en_uc = req->msg[1] ? true : false;
	bool en_mc = req->msg[2] ? true : false;
	struct hclge_promisc_param param;

	/* always enable broadcast promisc bit */
	hclge_promisc_param_init(&param, en_uc, en_mc, true, vport->vport_id);
	return hclge_cmd_set_promisc_mode(vport->back, &param);
}

static int hclge_set_vf_uc_mac_addr(struct hclge_vport *vport,
				    struct hclge_mbx_vf_to_pf_cmd *mbx_req,
				    bool gen_resp)
{
	const u8 *mac_addr = (const u8 *)(&mbx_req->msg[2]);
	struct hclge_dev *hdev = vport->back;
	int status;

	if (mbx_req->msg[1] == HCLGE_MBX_MAC_VLAN_UC_MODIFY) {
		const u8 *old_addr = (const u8 *)(&mbx_req->msg[8]);

		hclge_rm_uc_addr_common(vport, old_addr);
		status = hclge_add_uc_addr_common(vport, mac_addr);
		if (status)
			hclge_add_uc_addr_common(vport, old_addr);
	} else if (mbx_req->msg[1] == HCLGE_MBX_MAC_VLAN_UC_ADD) {
		status = hclge_add_uc_addr_common(vport, mac_addr);
	} else if (mbx_req->msg[1] == HCLGE_MBX_MAC_VLAN_UC_REMOVE) {
		status = hclge_rm_uc_addr_common(vport, mac_addr);
	} else {
		dev_err(&hdev->pdev->dev,
			"failed to set unicast mac addr, unknown subcode %d\n",
			mbx_req->msg[1]);
		return -EIO;
	}

	if (gen_resp)
		hclge_gen_resp_to_vf(vport, mbx_req, status, NULL, 0);

	return 0;
}

static int hclge_set_vf_mc_mta_status(struct hclge_vport *vport,
				      u8 *msg, u8 idx, bool is_end)
{
#define HCLGE_MTA_STATUS_MSG_SIZE 13
#define HCLGE_MTA_STATUS_MSG_BITS \
				(HCLGE_MTA_STATUS_MSG_SIZE * BITS_PER_BYTE)
#define HCLGE_MTA_STATUS_MSG_END_BITS \
				(HCLGE_MTA_TBL_SIZE % HCLGE_MTA_STATUS_MSG_BITS)
	unsigned long status[BITS_TO_LONGS(HCLGE_MTA_STATUS_MSG_BITS)];
	u16 tbl_cnt;
	u16 tbl_idx;
	u8 msg_ofs;
	u8 msg_bit;

	tbl_cnt = is_end ? HCLGE_MTA_STATUS_MSG_END_BITS :
			HCLGE_MTA_STATUS_MSG_BITS;

	/* set msg field */
	msg_ofs = 0;
	msg_bit = 0;
	memset(status, 0, sizeof(status));
	for (tbl_idx = 0; tbl_idx < tbl_cnt; tbl_idx++) {
		if (msg[msg_ofs] & BIT(msg_bit))
			set_bit(tbl_idx, status);

		msg_bit++;
		if (msg_bit == BITS_PER_BYTE) {
			msg_bit = 0;
			msg_ofs++;
		}
	}

	return hclge_update_mta_status_common(vport,
					status, idx * HCLGE_MTA_STATUS_MSG_BITS,
					tbl_cnt, is_end);
}

static int hclge_set_vf_mc_mac_addr(struct hclge_vport *vport,
				    struct hclge_mbx_vf_to_pf_cmd *mbx_req,
				    bool gen_resp)
{
	const u8 *mac_addr = (const u8 *)(&mbx_req->msg[2]);
	struct hclge_dev *hdev = vport->back;
	u8 resp_len = 0;
	u8 resp_data;
	int status;

	if (mbx_req->msg[1] == HCLGE_MBX_MAC_VLAN_MC_ADD) {
		status = hclge_add_mc_addr_common(vport, mac_addr);
	} else if (mbx_req->msg[1] == HCLGE_MBX_MAC_VLAN_MC_REMOVE) {
		status = hclge_rm_mc_addr_common(vport, mac_addr);
	} else if (mbx_req->msg[1] == HCLGE_MBX_MAC_VLAN_MC_FUNC_MTA_ENABLE) {
		u8 func_id = vport->vport_id;
		bool enable = mbx_req->msg[2];

		status = hclge_cfg_func_mta_filter(hdev, func_id, enable);
	} else if (mbx_req->msg[1] == HCLGE_MBX_MAC_VLAN_MTA_TYPE_READ) {
		resp_data = hdev->mta_mac_sel_type;
		resp_len = sizeof(u8);
		gen_resp = true;
		status = 0;
	} else if (mbx_req->msg[1] == HCLGE_MBX_MAC_VLAN_MTA_STATUS_UPDATE) {
		/* mta status update msg format
		 * msg[2.6 : 2.0]  msg index
		 * msg[2.7]        msg is end
		 * msg[15 : 3]     mta status bits[103 : 0]
		 */
		bool is_end = (mbx_req->msg[2] & 0x80) ? true : false;

		status = hclge_set_vf_mc_mta_status(vport, &mbx_req->msg[3],
						    mbx_req->msg[2] & 0x7F,
						    is_end);
	} else {
		dev_err(&hdev->pdev->dev,
			"failed to set mcast mac addr, unknown subcode %d\n",
			mbx_req->msg[1]);
		return -EIO;
	}

	if (gen_resp)
		hclge_gen_resp_to_vf(vport, mbx_req, status,
				     &resp_data, resp_len);

	return 0;
}

static int hclge_set_vf_vlan_cfg(struct hclge_vport *vport,
				 struct hclge_mbx_vf_to_pf_cmd *mbx_req,
				 bool gen_resp)
{
	int status = 0;

	if (mbx_req->msg[1] == HCLGE_MBX_VLAN_FILTER) {
		struct hnae3_handle *handle = &vport->nic;
		u16 vlan, proto;
		bool is_kill;

		is_kill = !!mbx_req->msg[2];
		memcpy(&vlan, &mbx_req->msg[3], sizeof(vlan));
		memcpy(&proto, &mbx_req->msg[5], sizeof(proto));
		status = hclge_set_vlan_filter(handle, cpu_to_be16(proto),
					       vlan, is_kill);
	} else if (mbx_req->msg[1] == HCLGE_MBX_VLAN_RX_OFF_CFG) {
		struct hnae3_handle *handle = &vport->nic;
		bool en = mbx_req->msg[2] ? true : false;

		status = hclge_en_hw_strip_rxvtag(handle, en);
	}

	if (gen_resp)
		status = hclge_gen_resp_to_vf(vport, mbx_req, status, NULL, 0);

	return status;
}

static int hclge_get_vf_tcinfo(struct hclge_vport *vport,
			       struct hclge_mbx_vf_to_pf_cmd *mbx_req,
			       bool gen_resp)
{
	struct hclge_dev *hdev = vport->back;
	int ret;

	ret = hclge_gen_resp_to_vf(vport, mbx_req, 0, &hdev->hw_tc_map,
				   sizeof(u8));
=======
static void hclge_set_vf_promisc_mode(struct hclge_vport *vport,
				      struct hclge_mbx_vf_to_pf_cmd *req)
{
	struct hnae3_handle *handle = &vport->nic;
	struct hclge_dev *hdev = vport->back;

	vport->vf_info.request_uc_en = req->msg.en_uc;
	vport->vf_info.request_mc_en = req->msg.en_mc;
	vport->vf_info.request_bc_en = req->msg.en_bc;

	if (req->msg.en_limit_promisc)
		set_bit(HNAE3_PFLAG_LIMIT_PROMISC, &handle->priv_flags);
	else
		clear_bit(HNAE3_PFLAG_LIMIT_PROMISC,
			  &handle->priv_flags);

	set_bit(HCLGE_VPORT_STATE_PROMISC_CHANGE, &vport->state);
	hclge_task_schedule(hdev, 0);
}

static int hclge_set_vf_uc_mac_addr(struct hclge_vport *vport,
				    struct hclge_mbx_vf_to_pf_cmd *mbx_req)
{
#define HCLGE_MBX_VF_OLD_MAC_ADDR_OFFSET	6

	const u8 *mac_addr = (const u8 *)(mbx_req->msg.data);
	struct hclge_dev *hdev = vport->back;
	int status;

	if (mbx_req->msg.subcode == HCLGE_MBX_MAC_VLAN_UC_MODIFY) {
		const u8 *old_addr = (const u8 *)
		(&mbx_req->msg.data[HCLGE_MBX_VF_OLD_MAC_ADDR_OFFSET]);

		/* If VF MAC has been configured by the host then it
		 * cannot be overridden by the MAC specified by the VM.
		 */
		if (!is_zero_ether_addr(vport->vf_info.mac) &&
		    !ether_addr_equal(mac_addr, vport->vf_info.mac))
			return -EPERM;

		if (!is_valid_ether_addr(mac_addr))
			return -EINVAL;

		spin_lock_bh(&vport->mac_list_lock);
		status = hclge_update_mac_node_for_dev_addr(vport, old_addr,
							    mac_addr);
		spin_unlock_bh(&vport->mac_list_lock);
		hclge_task_schedule(hdev, 0);
	} else if (mbx_req->msg.subcode == HCLGE_MBX_MAC_VLAN_UC_ADD) {
		status = hclge_update_mac_list(vport, HCLGE_MAC_TO_ADD,
					       HCLGE_MAC_ADDR_UC, mac_addr);
	} else if (mbx_req->msg.subcode == HCLGE_MBX_MAC_VLAN_UC_REMOVE) {
		status = hclge_update_mac_list(vport, HCLGE_MAC_TO_DEL,
					       HCLGE_MAC_ADDR_UC, mac_addr);
	} else {
		dev_err(&hdev->pdev->dev,
			"failed to set unicast mac addr, unknown subcode %u\n",
			mbx_req->msg.subcode);
		return -EIO;
	}

	return status;
}

static int hclge_set_vf_mc_mac_addr(struct hclge_vport *vport,
				    struct hclge_mbx_vf_to_pf_cmd *mbx_req)
{
	const u8 *mac_addr = (const u8 *)(mbx_req->msg.data);
	struct hclge_dev *hdev = vport->back;

	if (mbx_req->msg.subcode == HCLGE_MBX_MAC_VLAN_MC_ADD) {
		hclge_update_mac_list(vport, HCLGE_MAC_TO_ADD,
				      HCLGE_MAC_ADDR_MC, mac_addr);
	} else if (mbx_req->msg.subcode == HCLGE_MBX_MAC_VLAN_MC_REMOVE) {
		hclge_update_mac_list(vport, HCLGE_MAC_TO_DEL,
				      HCLGE_MAC_ADDR_MC, mac_addr);
	} else {
		dev_err(&hdev->pdev->dev,
			"failed to set mcast mac addr, unknown subcode %u\n",
			mbx_req->msg.subcode);
		return -EIO;
	}

	return 0;
}

int hclge_push_vf_port_base_vlan_info(struct hclge_vport *vport, u8 vfid,
				      u16 state,
				      struct hclge_vlan_info *vlan_info)
{
#define MSG_DATA_SIZE	8

	u8 msg_data[MSG_DATA_SIZE];

	memcpy(&msg_data[0], &state, sizeof(u16));
	memcpy(&msg_data[2], &vlan_info->vlan_proto, sizeof(u16));
	memcpy(&msg_data[4], &vlan_info->qos, sizeof(u16));
	memcpy(&msg_data[6], &vlan_info->vlan_tag, sizeof(u16));

	return hclge_send_mbx_msg(vport, msg_data, sizeof(msg_data),
				  HCLGE_MBX_PUSH_VLAN_INFO, vfid);
}

static int hclge_set_vf_vlan_cfg(struct hclge_vport *vport,
				 struct hclge_mbx_vf_to_pf_cmd *mbx_req,
				 struct hclge_respond_to_vf_msg *resp_msg)
{
#define HCLGE_MBX_VLAN_STATE_OFFSET	0
#define HCLGE_MBX_VLAN_INFO_OFFSET	2

	struct hnae3_handle *handle = &vport->nic;
	struct hclge_dev *hdev = vport->back;
	struct hclge_vf_vlan_cfg *msg_cmd;

	msg_cmd = (struct hclge_vf_vlan_cfg *)&mbx_req->msg;
	switch (msg_cmd->subcode) {
	case HCLGE_MBX_VLAN_FILTER:
		return hclge_set_vlan_filter(handle,
					     cpu_to_be16(msg_cmd->proto),
					     msg_cmd->vlan, msg_cmd->is_kill);
	case HCLGE_MBX_VLAN_RX_OFF_CFG:
		return hclge_en_hw_strip_rxvtag(handle, msg_cmd->enable);
	case HCLGE_MBX_GET_PORT_BASE_VLAN_STATE:
		/* vf does not need to know about the port based VLAN state
		 * on device HNAE3_DEVICE_VERSION_V3. So always return disable
		 * on device HNAE3_DEVICE_VERSION_V3 if vf queries the port
		 * based VLAN state.
		 */
		resp_msg->data[0] =
			hdev->ae_dev->dev_version >= HNAE3_DEVICE_VERSION_V3 ?
			HNAE3_PORT_BASE_VLAN_DISABLE :
			vport->port_base_vlan_cfg.state;
		resp_msg->len = sizeof(u8);
		return 0;
	case HCLGE_MBX_ENABLE_VLAN_FILTER:
		return hclge_enable_vport_vlan_filter(vport, msg_cmd->enable);
	default:
		return 0;
	}
}

static int hclge_set_vf_alive(struct hclge_vport *vport,
			      struct hclge_mbx_vf_to_pf_cmd *mbx_req)
{
	bool alive = !!mbx_req->msg.data[0];
	int ret = 0;

	if (alive)
		ret = hclge_vport_start(vport);
	else
		hclge_vport_stop(vport);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int hclge_get_vf_queue_info(struct hclge_vport *vport,
				   struct hclge_mbx_vf_to_pf_cmd *mbx_req,
				   bool gen_resp)
{
#define HCLGE_TQPS_RSS_INFO_LEN		8
	u8 resp_data[HCLGE_TQPS_RSS_INFO_LEN];
	struct hclge_dev *hdev = vport->back;

	/* get the queue related info */
	memcpy(&resp_data[0], &vport->alloc_tqps, sizeof(u16));
	memcpy(&resp_data[2], &vport->nic.kinfo.rss_size, sizeof(u16));
	memcpy(&resp_data[4], &hdev->num_desc, sizeof(u16));
	memcpy(&resp_data[6], &hdev->rx_buf_len, sizeof(u16));

	return hclge_gen_resp_to_vf(vport, mbx_req, 0, resp_data,
				    HCLGE_TQPS_RSS_INFO_LEN);
}

static int hclge_get_link_info(struct hclge_vport *vport,
			       struct hclge_mbx_vf_to_pf_cmd *mbx_req)
{
	struct hclge_dev *hdev = vport->back;
	u16 link_status;
	u8 msg_data[8];
	u8 dest_vfid;
	u16 duplex;

	/* mac.link can only be 0 or 1 */
	link_status = (u16)hdev->hw.mac.link;
=======
static void hclge_get_basic_info(struct hclge_vport *vport,
				 struct hclge_respond_to_vf_msg *resp_msg)
{
	struct hnae3_knic_private_info *kinfo = &vport->nic.kinfo;
	struct hnae3_ae_dev *ae_dev = vport->back->ae_dev;
	struct hclge_basic_info *basic_info;
	unsigned int i;

	basic_info = (struct hclge_basic_info *)resp_msg->data;
	for (i = 0; i < kinfo->tc_info.num_tc; i++)
		basic_info->hw_tc_map |= BIT(i);

	if (test_bit(HNAE3_DEV_SUPPORT_VLAN_FLTR_MDF_B, ae_dev->caps))
		hnae3_set_bit(basic_info->pf_caps,
			      HNAE3_PF_SUPPORT_VLAN_FLTR_MDF_B, 1);

	resp_msg->len = HCLGE_MBX_MAX_RESP_DATA_SIZE;
}

static void hclge_get_vf_queue_info(struct hclge_vport *vport,
				    struct hclge_respond_to_vf_msg *resp_msg)
{
#define HCLGE_TQPS_RSS_INFO_LEN		6
#define HCLGE_TQPS_ALLOC_OFFSET		0
#define HCLGE_TQPS_RSS_SIZE_OFFSET	2
#define HCLGE_TQPS_RX_BUFFER_LEN_OFFSET	4

	struct hclge_dev *hdev = vport->back;

	/* get the queue related info */
	memcpy(&resp_msg->data[HCLGE_TQPS_ALLOC_OFFSET],
	       &vport->alloc_tqps, sizeof(u16));
	memcpy(&resp_msg->data[HCLGE_TQPS_RSS_SIZE_OFFSET],
	       &vport->nic.kinfo.rss_size, sizeof(u16));
	memcpy(&resp_msg->data[HCLGE_TQPS_RX_BUFFER_LEN_OFFSET],
	       &hdev->rx_buf_len, sizeof(u16));
	resp_msg->len = HCLGE_TQPS_RSS_INFO_LEN;
}

static void hclge_get_vf_mac_addr(struct hclge_vport *vport,
				  struct hclge_respond_to_vf_msg *resp_msg)
{
	ether_addr_copy(resp_msg->data, vport->vf_info.mac);
	resp_msg->len = ETH_ALEN;
}

static void hclge_get_vf_queue_depth(struct hclge_vport *vport,
				     struct hclge_respond_to_vf_msg *resp_msg)
{
#define HCLGE_TQPS_DEPTH_INFO_LEN	4
#define HCLGE_TQPS_NUM_TX_DESC_OFFSET	0
#define HCLGE_TQPS_NUM_RX_DESC_OFFSET	2

	struct hclge_dev *hdev = vport->back;

	/* get the queue depth info */
	memcpy(&resp_msg->data[HCLGE_TQPS_NUM_TX_DESC_OFFSET],
	       &hdev->num_tx_desc, sizeof(u16));
	memcpy(&resp_msg->data[HCLGE_TQPS_NUM_RX_DESC_OFFSET],
	       &hdev->num_rx_desc, sizeof(u16));
	resp_msg->len = HCLGE_TQPS_DEPTH_INFO_LEN;
}

static void hclge_get_vf_media_type(struct hclge_vport *vport,
				    struct hclge_respond_to_vf_msg *resp_msg)
{
#define HCLGE_VF_MEDIA_TYPE_OFFSET	0
#define HCLGE_VF_MODULE_TYPE_OFFSET	1
#define HCLGE_VF_MEDIA_TYPE_LENGTH	2

	struct hclge_dev *hdev = vport->back;

	resp_msg->data[HCLGE_VF_MEDIA_TYPE_OFFSET] =
		hdev->hw.mac.media_type;
	resp_msg->data[HCLGE_VF_MODULE_TYPE_OFFSET] =
		hdev->hw.mac.module_type;
	resp_msg->len = HCLGE_VF_MEDIA_TYPE_LENGTH;
}

int hclge_push_vf_link_status(struct hclge_vport *vport)
{
#define HCLGE_VF_LINK_STATE_UP		1U
#define HCLGE_VF_LINK_STATE_DOWN	0U

	struct hclge_dev *hdev = vport->back;
	u16 link_status;
	u8 msg_data[9];
	u16 duplex;

	/* mac.link can only be 0 or 1 */
	switch (vport->vf_info.link_state) {
	case IFLA_VF_LINK_STATE_ENABLE:
		link_status = HCLGE_VF_LINK_STATE_UP;
		break;
	case IFLA_VF_LINK_STATE_DISABLE:
		link_status = HCLGE_VF_LINK_STATE_DOWN;
		break;
	case IFLA_VF_LINK_STATE_AUTO:
	default:
		link_status = (u16)hdev->hw.mac.link;
		break;
	}

>>>>>>> upstream/android-13
	duplex = hdev->hw.mac.duplex;
	memcpy(&msg_data[0], &link_status, sizeof(u16));
	memcpy(&msg_data[2], &hdev->hw.mac.speed, sizeof(u32));
	memcpy(&msg_data[6], &duplex, sizeof(u16));
<<<<<<< HEAD
	dest_vfid = mbx_req->mbx_src_vfid;

	/* send this requested info to VF */
	return hclge_send_mbx_msg(vport, msg_data, sizeof(msg_data),
				  HCLGE_MBX_LINK_STAT_CHANGE, dest_vfid);
}

static void hclge_mbx_reset_vf_queue(struct hclge_vport *vport,
				     struct hclge_mbx_vf_to_pf_cmd *mbx_req)
{
	u16 queue_id;

	memcpy(&queue_id, &mbx_req->msg[2], sizeof(queue_id));

	hclge_reset_vf_queue(vport, queue_id);

	/* send response msg to VF after queue reset complete*/
	hclge_gen_resp_to_vf(vport, mbx_req, 0, NULL, 0);
}

static void hclge_reset_vf(struct hclge_vport *vport,
			   struct hclge_mbx_vf_to_pf_cmd *mbx_req)
{
	struct hclge_dev *hdev = vport->back;
	int ret;

	dev_warn(&hdev->pdev->dev, "PF received VF reset request from VF %d!",
		 mbx_req->mbx_src_vfid);

	/* Acknowledge VF that PF is now about to assert the reset for the VF.
	 * On receiving this message VF will get into pending state and will
	 * start polling for the hardware reset completion status.
	 */
	ret = hclge_inform_reset_assert_to_vf(vport);
	if (ret) {
		dev_err(&hdev->pdev->dev,
			"PF fail(%d) to inform VF(%d)of reset, reset failed!\n",
			ret, vport->vport_id);
		return;
	}

	dev_warn(&hdev->pdev->dev, "PF is now resetting VF %d.\n",
		 mbx_req->mbx_src_vfid);
	/* reset this virtual function */
	hclge_func_reset_cmd(hdev, mbx_req->mbx_src_vfid);
=======
	msg_data[8] = HCLGE_MBX_PUSH_LINK_STATUS_EN;

	/* send this requested info to VF */
	return hclge_send_mbx_msg(vport, msg_data, sizeof(msg_data),
				  HCLGE_MBX_LINK_STAT_CHANGE, vport->vport_id);
}

static void hclge_get_link_mode(struct hclge_vport *vport,
				struct hclge_mbx_vf_to_pf_cmd *mbx_req)
{
#define HCLGE_SUPPORTED   1
	struct hclge_dev *hdev = vport->back;
	unsigned long advertising;
	unsigned long supported;
	unsigned long send_data;
	u8 msg_data[10] = {};
	u8 dest_vfid;

	advertising = hdev->hw.mac.advertising[0];
	supported = hdev->hw.mac.supported[0];
	dest_vfid = mbx_req->mbx_src_vfid;
	msg_data[0] = mbx_req->msg.data[0];

	send_data = msg_data[0] == HCLGE_SUPPORTED ? supported : advertising;

	memcpy(&msg_data[2], &send_data, sizeof(unsigned long));
	hclge_send_mbx_msg(vport, msg_data, sizeof(msg_data),
			   HCLGE_MBX_LINK_STAT_MODE, dest_vfid);
}

static int hclge_mbx_reset_vf_queue(struct hclge_vport *vport,
				    struct hclge_mbx_vf_to_pf_cmd *mbx_req,
				    struct hclge_respond_to_vf_msg *resp_msg)
{
#define HCLGE_RESET_ALL_QUEUE_DONE	1U
	struct hnae3_handle *handle = &vport->nic;
	struct hclge_dev *hdev = vport->back;
	u16 queue_id;
	int ret;

	memcpy(&queue_id, mbx_req->msg.data, sizeof(queue_id));
	resp_msg->data[0] = HCLGE_RESET_ALL_QUEUE_DONE;
	resp_msg->len = sizeof(u8);

	/* pf will reset vf's all queues at a time. So it is unnecessary
	 * to reset queues if queue_id > 0, just return success.
	 */
	if (queue_id > 0)
		return 0;

	ret = hclge_reset_tqp(handle);
	if (ret)
		dev_err(&hdev->pdev->dev, "failed to reset vf %u queue, ret = %d\n",
			vport->vport_id - HCLGE_VF_VPORT_START_NUM, ret);

	return ret;
}

static int hclge_reset_vf(struct hclge_vport *vport)
{
	struct hclge_dev *hdev = vport->back;

	dev_warn(&hdev->pdev->dev, "PF received VF reset request from VF %u!",
		 vport->vport_id - HCLGE_VF_VPORT_START_NUM);

	return hclge_func_reset_cmd(hdev, vport->vport_id);
}

static void hclge_vf_keep_alive(struct hclge_vport *vport)
{
	vport->last_active_jiffies = jiffies;
}

static int hclge_set_vf_mtu(struct hclge_vport *vport,
			    struct hclge_mbx_vf_to_pf_cmd *mbx_req)
{
	u32 mtu;

	memcpy(&mtu, mbx_req->msg.data, sizeof(mtu));

	return hclge_set_vport_mtu(vport, mtu);
}

static int hclge_get_queue_id_in_pf(struct hclge_vport *vport,
				    struct hclge_mbx_vf_to_pf_cmd *mbx_req,
				    struct hclge_respond_to_vf_msg *resp_msg)
{
	struct hnae3_handle *handle = &vport->nic;
	struct hclge_dev *hdev = vport->back;
	u16 queue_id, qid_in_pf;

	memcpy(&queue_id, mbx_req->msg.data, sizeof(queue_id));
	if (queue_id >= handle->kinfo.num_tqps) {
		dev_err(&hdev->pdev->dev, "Invalid queue id(%u) from VF %u\n",
			queue_id, mbx_req->mbx_src_vfid);
		return -EINVAL;
	}

	qid_in_pf = hclge_covert_handle_qid_global(&vport->nic, queue_id);
	memcpy(resp_msg->data, &qid_in_pf, sizeof(qid_in_pf));
	resp_msg->len = sizeof(qid_in_pf);
	return 0;
}

static int hclge_get_rss_key(struct hclge_vport *vport,
			     struct hclge_mbx_vf_to_pf_cmd *mbx_req,
			     struct hclge_respond_to_vf_msg *resp_msg)
{
#define HCLGE_RSS_MBX_RESP_LEN	8
	struct hclge_dev *hdev = vport->back;
	u8 index;

	index = mbx_req->msg.data[0];

	/* Check the query index of rss_hash_key from VF, make sure no
	 * more than the size of rss_hash_key.
	 */
	if (((index + 1) * HCLGE_RSS_MBX_RESP_LEN) >
	      sizeof(vport[0].rss_hash_key)) {
		dev_warn(&hdev->pdev->dev,
			 "failed to get the rss hash key, the index(%u) invalid !\n",
			 index);
		return -EINVAL;
	}

	memcpy(resp_msg->data,
	       &hdev->vport[0].rss_hash_key[index * HCLGE_RSS_MBX_RESP_LEN],
	       HCLGE_RSS_MBX_RESP_LEN);
	resp_msg->len = HCLGE_RSS_MBX_RESP_LEN;
	return 0;
}

static void hclge_link_fail_parse(struct hclge_dev *hdev, u8 link_fail_code)
{
	switch (link_fail_code) {
	case HCLGE_LF_REF_CLOCK_LOST:
		dev_warn(&hdev->pdev->dev, "Reference clock lost!\n");
		break;
	case HCLGE_LF_XSFP_TX_DISABLE:
		dev_warn(&hdev->pdev->dev, "SFP tx is disabled!\n");
		break;
	case HCLGE_LF_XSFP_ABSENT:
		dev_warn(&hdev->pdev->dev, "SFP is absent!\n");
		break;
	default:
		break;
	}
}

static void hclge_handle_link_change_event(struct hclge_dev *hdev,
					   struct hclge_mbx_vf_to_pf_cmd *req)
{
	hclge_task_schedule(hdev, 0);

	if (!req->msg.subcode)
		hclge_link_fail_parse(hdev, req->msg.data[0]);
>>>>>>> upstream/android-13
}

static bool hclge_cmd_crq_empty(struct hclge_hw *hw)
{
	u32 tail = hclge_read_dev(hw, HCLGE_NIC_CRQ_TAIL_REG);

	return tail == hw->cmq.crq.next_to_use;
}

<<<<<<< HEAD
void hclge_mbx_handler(struct hclge_dev *hdev)
{
	struct hclge_cmq_ring *crq = &hdev->hw.cmq.crq;
	struct hclge_mbx_vf_to_pf_cmd *req;
	struct hclge_vport *vport;
	struct hclge_desc *desc;
	int ret, flag;
=======
static void hclge_handle_ncsi_error(struct hclge_dev *hdev)
{
	struct hnae3_ae_dev *ae_dev = hdev->ae_dev;

	ae_dev->ops->set_default_reset_request(ae_dev, HNAE3_GLOBAL_RESET);
	dev_warn(&hdev->pdev->dev, "requesting reset due to NCSI error\n");
	ae_dev->ops->reset_event(hdev->pdev, NULL);
}

static void hclge_handle_vf_tbl(struct hclge_vport *vport,
				struct hclge_mbx_vf_to_pf_cmd *mbx_req)
{
	struct hclge_dev *hdev = vport->back;
	struct hclge_vf_vlan_cfg *msg_cmd;

	msg_cmd = (struct hclge_vf_vlan_cfg *)&mbx_req->msg;
	if (msg_cmd->subcode == HCLGE_MBX_VPORT_LIST_CLEAR) {
		hclge_rm_vport_all_mac_table(vport, true, HCLGE_MAC_ADDR_UC);
		hclge_rm_vport_all_mac_table(vport, true, HCLGE_MAC_ADDR_MC);
		hclge_rm_vport_all_vlan_table(vport, true);
	} else {
		dev_warn(&hdev->pdev->dev, "Invalid cmd(%u)\n",
			 msg_cmd->subcode);
	}
}

void hclge_mbx_handler(struct hclge_dev *hdev)
{
	struct hclge_cmq_ring *crq = &hdev->hw.cmq.crq;
	struct hclge_respond_to_vf_msg resp_msg;
	struct hclge_mbx_vf_to_pf_cmd *req;
	struct hclge_vport *vport;
	struct hclge_desc *desc;
	bool is_del = false;
	unsigned int flag;
	int ret = 0;
>>>>>>> upstream/android-13

	/* handle all the mailbox requests in the queue */
	while (!hclge_cmd_crq_empty(&hdev->hw)) {
		if (test_bit(HCLGE_STATE_CMD_DISABLE, &hdev->state)) {
			dev_warn(&hdev->pdev->dev,
				 "command queue needs re-initializing\n");
			return;
		}

		desc = &crq->desc[crq->next_to_use];
		req = (struct hclge_mbx_vf_to_pf_cmd *)desc->data;

		flag = le16_to_cpu(crq->desc[crq->next_to_use].flag);
		if (unlikely(!hnae3_get_bit(flag, HCLGE_CMDQ_RX_OUTVLD_B))) {
			dev_warn(&hdev->pdev->dev,
<<<<<<< HEAD
				 "dropped invalid mailbox message, code = %d\n",
				 req->msg[0]);
=======
				 "dropped invalid mailbox message, code = %u\n",
				 req->msg.code);
>>>>>>> upstream/android-13

			/* dropping/not processing this invalid message */
			crq->desc[crq->next_to_use].flag = 0;
			hclge_mbx_ring_ptr_move_crq(crq);
			continue;
		}

		vport = &hdev->vport[req->mbx_src_vfid];

<<<<<<< HEAD
		switch (req->msg[0]) {
=======
		trace_hclge_pf_mbx_get(hdev, req);

		/* clear the resp_msg before processing every mailbox message */
		memset(&resp_msg, 0, sizeof(resp_msg));

		switch (req->msg.code) {
>>>>>>> upstream/android-13
		case HCLGE_MBX_MAP_RING_TO_VECTOR:
			ret = hclge_map_unmap_ring_to_vf_vector(vport, true,
								req);
			break;
		case HCLGE_MBX_UNMAP_RING_TO_VECTOR:
			ret = hclge_map_unmap_ring_to_vf_vector(vport, false,
								req);
			break;
		case HCLGE_MBX_SET_PROMISC_MODE:
<<<<<<< HEAD
			ret = hclge_set_vf_promisc_mode(vport, req);
			if (ret)
				dev_err(&hdev->pdev->dev,
					"PF fail(%d) to set VF promisc mode\n",
					ret);
			break;
		case HCLGE_MBX_SET_UNICAST:
			ret = hclge_set_vf_uc_mac_addr(vport, req, true);
=======
			hclge_set_vf_promisc_mode(vport, req);
			break;
		case HCLGE_MBX_SET_UNICAST:
			ret = hclge_set_vf_uc_mac_addr(vport, req);
>>>>>>> upstream/android-13
			if (ret)
				dev_err(&hdev->pdev->dev,
					"PF fail(%d) to set VF UC MAC Addr\n",
					ret);
			break;
		case HCLGE_MBX_SET_MULTICAST:
<<<<<<< HEAD
			ret = hclge_set_vf_mc_mac_addr(vport, req, false);
=======
			ret = hclge_set_vf_mc_mac_addr(vport, req);
>>>>>>> upstream/android-13
			if (ret)
				dev_err(&hdev->pdev->dev,
					"PF fail(%d) to set VF MC MAC Addr\n",
					ret);
			break;
		case HCLGE_MBX_SET_VLAN:
<<<<<<< HEAD
			ret = hclge_set_vf_vlan_cfg(vport, req, false);
=======
			ret = hclge_set_vf_vlan_cfg(vport, req, &resp_msg);
>>>>>>> upstream/android-13
			if (ret)
				dev_err(&hdev->pdev->dev,
					"PF failed(%d) to config VF's VLAN\n",
					ret);
			break;
<<<<<<< HEAD
		case HCLGE_MBX_GET_QINFO:
			ret = hclge_get_vf_queue_info(vport, req, true);
			if (ret)
				dev_err(&hdev->pdev->dev,
					"PF failed(%d) to get Q info for VF\n",
					ret);
			break;
		case HCLGE_MBX_GET_TCINFO:
			ret = hclge_get_vf_tcinfo(vport, req, true);
			if (ret)
				dev_err(&hdev->pdev->dev,
					"PF failed(%d) to get TC info for VF\n",
					ret);
			break;
		case HCLGE_MBX_GET_LINK_STATUS:
			ret = hclge_get_link_info(vport, req);
			if (ret)
				dev_err(&hdev->pdev->dev,
					"PF fail(%d) to get link stat for VF\n",
					ret);
			break;
		case HCLGE_MBX_QUEUE_RESET:
			hclge_mbx_reset_vf_queue(vport, req);
			break;
		case HCLGE_MBX_RESET:
			hclge_reset_vf(vport, req);
			break;
		default:
			dev_err(&hdev->pdev->dev,
				"un-supported mailbox message, code = %d\n",
				req->msg[0]);
			break;
		}
		crq->desc[crq->next_to_use].flag = 0;
		hclge_mbx_ring_ptr_move_crq(crq);
=======
		case HCLGE_MBX_SET_ALIVE:
			ret = hclge_set_vf_alive(vport, req);
			if (ret)
				dev_err(&hdev->pdev->dev,
					"PF failed(%d) to set VF's ALIVE\n",
					ret);
			break;
		case HCLGE_MBX_GET_QINFO:
			hclge_get_vf_queue_info(vport, &resp_msg);
			break;
		case HCLGE_MBX_GET_QDEPTH:
			hclge_get_vf_queue_depth(vport, &resp_msg);
			break;
		case HCLGE_MBX_GET_BASIC_INFO:
			hclge_get_basic_info(vport, &resp_msg);
			break;
		case HCLGE_MBX_GET_LINK_STATUS:
			ret = hclge_push_vf_link_status(vport);
			if (ret)
				dev_err(&hdev->pdev->dev,
					"failed to inform link stat to VF, ret = %d\n",
					ret);
			break;
		case HCLGE_MBX_QUEUE_RESET:
			ret = hclge_mbx_reset_vf_queue(vport, req, &resp_msg);
			break;
		case HCLGE_MBX_RESET:
			ret = hclge_reset_vf(vport);
			break;
		case HCLGE_MBX_KEEP_ALIVE:
			hclge_vf_keep_alive(vport);
			break;
		case HCLGE_MBX_SET_MTU:
			ret = hclge_set_vf_mtu(vport, req);
			if (ret)
				dev_err(&hdev->pdev->dev,
					"VF fail(%d) to set mtu\n", ret);
			break;
		case HCLGE_MBX_GET_QID_IN_PF:
			ret = hclge_get_queue_id_in_pf(vport, req, &resp_msg);
			break;
		case HCLGE_MBX_GET_RSS_KEY:
			ret = hclge_get_rss_key(vport, req, &resp_msg);
			break;
		case HCLGE_MBX_GET_LINK_MODE:
			hclge_get_link_mode(vport, req);
			break;
		case HCLGE_MBX_GET_VF_FLR_STATUS:
		case HCLGE_MBX_VF_UNINIT:
			is_del = req->msg.code == HCLGE_MBX_VF_UNINIT;
			hclge_rm_vport_all_mac_table(vport, is_del,
						     HCLGE_MAC_ADDR_UC);
			hclge_rm_vport_all_mac_table(vport, is_del,
						     HCLGE_MAC_ADDR_MC);
			hclge_rm_vport_all_vlan_table(vport, is_del);
			break;
		case HCLGE_MBX_GET_MEDIA_TYPE:
			hclge_get_vf_media_type(vport, &resp_msg);
			break;
		case HCLGE_MBX_PUSH_LINK_STATUS:
			hclge_handle_link_change_event(hdev, req);
			break;
		case HCLGE_MBX_GET_MAC_ADDR:
			hclge_get_vf_mac_addr(vport, &resp_msg);
			break;
		case HCLGE_MBX_NCSI_ERROR:
			hclge_handle_ncsi_error(hdev);
			break;
		case HCLGE_MBX_HANDLE_VF_TBL:
			hclge_handle_vf_tbl(vport, req);
			break;
		default:
			dev_err(&hdev->pdev->dev,
				"un-supported mailbox message, code = %u\n",
				req->msg.code);
			break;
		}

		/* PF driver should not reply IMP */
		if (hnae3_get_bit(req->mbx_need_resp, HCLGE_MBX_NEED_RESP_B) &&
		    req->msg.code < HCLGE_MBX_GET_VF_FLR_STATUS) {
			resp_msg.status = ret;
			hclge_gen_resp_to_vf(vport, req, &resp_msg);
		}

		crq->desc[crq->next_to_use].flag = 0;
		hclge_mbx_ring_ptr_move_crq(crq);

		/* reinitialize ret after complete the mbx message processing */
		ret = 0;
>>>>>>> upstream/android-13
	}

	/* Write back CMDQ_RQ header pointer, M7 need this pointer */
	hclge_write_dev(&hdev->hw, HCLGE_NIC_CRQ_HEAD_REG, crq->next_to_use);
}
