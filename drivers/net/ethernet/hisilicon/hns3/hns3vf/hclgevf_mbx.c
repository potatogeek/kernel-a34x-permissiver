// SPDX-License-Identifier: GPL-2.0+
// Copyright (c) 2016-2017 Hisilicon Limited.

#include "hclge_mbx.h"
#include "hclgevf_main.h"
#include "hnae3.h"

<<<<<<< HEAD
=======
#define CREATE_TRACE_POINTS
#include "hclgevf_trace.h"

static int hclgevf_resp_to_errno(u16 resp_code)
{
	return resp_code ? -resp_code : 0;
}

#define HCLGEVF_MBX_MATCH_ID_START	1
>>>>>>> upstream/android-13
static void hclgevf_reset_mbx_resp_status(struct hclgevf_dev *hdev)
{
	/* this function should be called with mbx_resp.mbx_mutex held
	 * to prtect the received_response from race condition
	 */
	hdev->mbx_resp.received_resp  = false;
	hdev->mbx_resp.origin_mbx_msg = 0;
	hdev->mbx_resp.resp_status    = 0;
<<<<<<< HEAD
=======
	hdev->mbx_resp.match_id++;
	/* Update match_id and ensure the value of match_id is not zero */
	if (hdev->mbx_resp.match_id == 0)
		hdev->mbx_resp.match_id = HCLGEVF_MBX_MATCH_ID_START;
>>>>>>> upstream/android-13
	memset(hdev->mbx_resp.additional_info, 0, HCLGE_MBX_MAX_RESP_DATA_SIZE);
}

/* hclgevf_get_mbx_resp: used to get a response from PF after VF sends a mailbox
 * message to PF.
 * @hdev: pointer to struct hclgevf_dev
 * @resp_msg: pointer to store the original message type and response status
 * @len: the resp_msg data array length.
 */
static int hclgevf_get_mbx_resp(struct hclgevf_dev *hdev, u16 code0, u16 code1,
				u8 *resp_data, u16 resp_len)
{
#define HCLGEVF_MAX_TRY_TIMES	500
<<<<<<< HEAD
#define HCLGEVF_SLEEP_USCOEND	1000
=======
#define HCLGEVF_SLEEP_USECOND	1000
>>>>>>> upstream/android-13
	struct hclgevf_mbx_resp_status *mbx_resp;
	u16 r_code0, r_code1;
	int i = 0;

	if (resp_len > HCLGE_MBX_MAX_RESP_DATA_SIZE) {
		dev_err(&hdev->pdev->dev,
<<<<<<< HEAD
			"VF mbx response len(=%d) exceeds maximum(=%d)\n",
=======
			"VF mbx response len(=%u) exceeds maximum(=%u)\n",
>>>>>>> upstream/android-13
			resp_len,
			HCLGE_MBX_MAX_RESP_DATA_SIZE);
		return -EINVAL;
	}

	while ((!hdev->mbx_resp.received_resp) && (i < HCLGEVF_MAX_TRY_TIMES)) {
<<<<<<< HEAD
		udelay(HCLGEVF_SLEEP_USCOEND);
=======
		if (test_bit(HCLGEVF_STATE_CMD_DISABLE, &hdev->state))
			return -EIO;

		usleep_range(HCLGEVF_SLEEP_USECOND, HCLGEVF_SLEEP_USECOND * 2);
>>>>>>> upstream/android-13
		i++;
	}

	if (i >= HCLGEVF_MAX_TRY_TIMES) {
		dev_err(&hdev->pdev->dev,
<<<<<<< HEAD
			"VF could not get mbx resp(=%d) from PF in %d tries\n",
			hdev->mbx_resp.received_resp, i);
=======
			"VF could not get mbx(%u,%u) resp(=%d) from PF in %d tries\n",
			code0, code1, hdev->mbx_resp.received_resp, i);
>>>>>>> upstream/android-13
		return -EIO;
	}

	mbx_resp = &hdev->mbx_resp;
	r_code0 = (u16)(mbx_resp->origin_mbx_msg >> 16);
	r_code1 = (u16)(mbx_resp->origin_mbx_msg & 0xff);

	if (mbx_resp->resp_status)
		return mbx_resp->resp_status;

	if (resp_data)
		memcpy(resp_data, &mbx_resp->additional_info[0], resp_len);

	hclgevf_reset_mbx_resp_status(hdev);

	if (!(r_code0 == code0 && r_code1 == code1 && !mbx_resp->resp_status)) {
		dev_err(&hdev->pdev->dev,
<<<<<<< HEAD
			"VF could not match resp code(code0=%d,code1=%d), %d",
			code0, code1, mbx_resp->resp_status);
=======
			"VF could not match resp code(code0=%u,code1=%u), %d\n",
			code0, code1, mbx_resp->resp_status);
		dev_err(&hdev->pdev->dev,
			"VF could not match resp r_code(r_code0=%u,r_code1=%u)\n",
			r_code0, r_code1);
>>>>>>> upstream/android-13
		return -EIO;
	}

	return 0;
}

<<<<<<< HEAD
int hclgevf_send_mbx_msg(struct hclgevf_dev *hdev, u16 code, u16 subcode,
			 const u8 *msg_data, u8 msg_len, bool need_resp,
=======
int hclgevf_send_mbx_msg(struct hclgevf_dev *hdev,
			 struct hclge_vf_to_pf_msg *send_msg, bool need_resp,
>>>>>>> upstream/android-13
			 u8 *resp_data, u16 resp_len)
{
	struct hclge_mbx_vf_to_pf_cmd *req;
	struct hclgevf_desc desc;
	int status;

	req = (struct hclge_mbx_vf_to_pf_cmd *)desc.data;

<<<<<<< HEAD
	/* first two bytes are reserved for code & subcode */
	if (msg_len > (HCLGE_MBX_MAX_MSG_SIZE - 2)) {
		dev_err(&hdev->pdev->dev,
			"VF send mbx msg fail, msg len %d exceeds max len %d\n",
			msg_len, HCLGE_MBX_MAX_MSG_SIZE);
=======
	if (!send_msg) {
		dev_err(&hdev->pdev->dev,
			"failed to send mbx, msg is NULL\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	hclgevf_cmd_setup_basic_desc(&desc, HCLGEVF_OPC_MBX_VF_TO_PF, false);
<<<<<<< HEAD
	req->msg[0] = code;
	req->msg[1] = subcode;
	memcpy(&req->msg[2], msg_data, msg_len);
=======
	if (need_resp)
		hnae3_set_bit(req->mbx_need_resp, HCLGE_MBX_NEED_RESP_B, 1);

	memcpy(&req->msg, send_msg, sizeof(struct hclge_vf_to_pf_msg));

	if (test_bit(HCLGEVF_STATE_NIC_REGISTERED, &hdev->state))
		trace_hclge_vf_mbx_send(hdev, req);
>>>>>>> upstream/android-13

	/* synchronous send */
	if (need_resp) {
		mutex_lock(&hdev->mbx_resp.mbx_mutex);
		hclgevf_reset_mbx_resp_status(hdev);
<<<<<<< HEAD
=======
		req->match_id = hdev->mbx_resp.match_id;
>>>>>>> upstream/android-13
		status = hclgevf_cmd_send(&hdev->hw, &desc, 1);
		if (status) {
			dev_err(&hdev->pdev->dev,
				"VF failed(=%d) to send mbx message to PF\n",
				status);
			mutex_unlock(&hdev->mbx_resp.mbx_mutex);
			return status;
		}

<<<<<<< HEAD
		status = hclgevf_get_mbx_resp(hdev, code, subcode, resp_data,
=======
		status = hclgevf_get_mbx_resp(hdev, send_msg->code,
					      send_msg->subcode, resp_data,
>>>>>>> upstream/android-13
					      resp_len);
		mutex_unlock(&hdev->mbx_resp.mbx_mutex);
	} else {
		/* asynchronous send */
		status = hclgevf_cmd_send(&hdev->hw, &desc, 1);
		if (status) {
			dev_err(&hdev->pdev->dev,
				"VF failed(=%d) to send mbx message to PF\n",
				status);
			return status;
		}
	}

	return status;
}

static bool hclgevf_cmd_crq_empty(struct hclgevf_hw *hw)
{
	u32 tail = hclgevf_read_dev(hw, HCLGEVF_NIC_CRQ_TAIL_REG);

	return tail == hw->cmq.crq.next_to_use;
}

<<<<<<< HEAD
void hclgevf_mbx_handler(struct hclgevf_dev *hdev)
{
	struct hclgevf_mbx_resp_status *resp;
	struct hclge_mbx_pf_to_vf_cmd *req;
	struct hclgevf_cmq_ring *crq;
	struct hclgevf_desc *desc;
	u16 *msg_q;
	u16 flag;
	u8 *temp;
	int i;

	resp = &hdev->mbx_resp;
	crq = &hdev->hw.cmq.crq;

	while (!hclgevf_cmd_crq_empty(&hdev->hw)) {
=======
static void hclgevf_handle_mbx_response(struct hclgevf_dev *hdev,
					struct hclge_mbx_pf_to_vf_cmd *req)
{
	struct hclgevf_mbx_resp_status *resp = &hdev->mbx_resp;

	if (resp->received_resp)
		dev_warn(&hdev->pdev->dev,
			 "VF mbx resp flag not clear(%u)\n",
			 req->msg.vf_mbx_msg_code);

	resp->origin_mbx_msg =
			(req->msg.vf_mbx_msg_code << 16);
	resp->origin_mbx_msg |= req->msg.vf_mbx_msg_subcode;
	resp->resp_status =
		hclgevf_resp_to_errno(req->msg.resp_status);
	memcpy(resp->additional_info, req->msg.resp_data,
	       HCLGE_MBX_MAX_RESP_DATA_SIZE * sizeof(u8));
	if (req->match_id) {
		/* If match_id is not zero, it means PF support match_id.
		 * if the match_id is right, VF get the right response, or
		 * ignore the response. and driver will clear hdev->mbx_resp
		 * when send next message which need response.
		 */
		if (req->match_id == resp->match_id)
			resp->received_resp = true;
	} else {
		resp->received_resp = true;
	}
}

static void hclgevf_handle_mbx_msg(struct hclgevf_dev *hdev,
				   struct hclge_mbx_pf_to_vf_cmd *req)
{
	/* we will drop the async msg if we find ARQ as full
	 * and continue with next message
	 */
	if (atomic_read(&hdev->arq.count) >=
	    HCLGE_MBX_MAX_ARQ_MSG_NUM) {
		dev_warn(&hdev->pdev->dev,
			 "Async Q full, dropping msg(%u)\n",
			 req->msg.code);
		return;
	}

	/* tail the async message in arq */
	memcpy(hdev->arq.msg_q[hdev->arq.tail], &req->msg,
	       HCLGE_MBX_MAX_ARQ_MSG_SIZE * sizeof(u16));
	hclge_mbx_tail_ptr_move_arq(hdev->arq);
	atomic_inc(&hdev->arq.count);

	hclgevf_mbx_task_schedule(hdev);
}

void hclgevf_mbx_handler(struct hclgevf_dev *hdev)
{
	struct hclge_mbx_pf_to_vf_cmd *req;
	struct hclgevf_cmq_ring *crq;
	struct hclgevf_desc *desc;
	u16 flag;

	crq = &hdev->hw.cmq.crq;

	while (!hclgevf_cmd_crq_empty(&hdev->hw)) {
		if (test_bit(HCLGEVF_STATE_CMD_DISABLE, &hdev->state)) {
			dev_info(&hdev->pdev->dev, "vf crq need init\n");
			return;
		}

>>>>>>> upstream/android-13
		desc = &crq->desc[crq->next_to_use];
		req = (struct hclge_mbx_pf_to_vf_cmd *)desc->data;

		flag = le16_to_cpu(crq->desc[crq->next_to_use].flag);
		if (unlikely(!hnae3_get_bit(flag, HCLGEVF_CMDQ_RX_OUTVLD_B))) {
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

<<<<<<< HEAD
=======
		trace_hclge_vf_mbx_get(hdev, req);

>>>>>>> upstream/android-13
		/* synchronous messages are time critical and need preferential
		 * treatment. Therefore, we need to acknowledge all the sync
		 * responses as quickly as possible so that waiting tasks do not
		 * timeout and simultaneously queue the async messages for later
		 * prcessing in context of mailbox task i.e. the slow path.
		 */
<<<<<<< HEAD
		switch (req->msg[0]) {
		case HCLGE_MBX_PF_VF_RESP:
			if (resp->received_resp)
				dev_warn(&hdev->pdev->dev,
					 "VF mbx resp flag not clear(%d)\n",
					 req->msg[1]);
			resp->received_resp = true;

			resp->origin_mbx_msg = (req->msg[1] << 16);
			resp->origin_mbx_msg |= req->msg[2];
			resp->resp_status = req->msg[3];

			temp = (u8 *)&req->msg[4];
			for (i = 0; i < HCLGE_MBX_MAX_RESP_DATA_SIZE; i++) {
				resp->additional_info[i] = *temp;
				temp++;
			}
			break;
		case HCLGE_MBX_LINK_STAT_CHANGE:
		case HCLGE_MBX_ASSERTING_RESET:
			/* set this mbx event as pending. This is required as we
			 * might loose interrupt event when mbx task is busy
			 * handling. This shall be cleared when mbx task just
			 * enters handling state.
			 */
			hdev->mbx_event_pending = true;

			/* we will drop the async msg if we find ARQ as full
			 * and continue with next message
			 */
			if (hdev->arq.count >= HCLGE_MBX_MAX_ARQ_MSG_NUM) {
				dev_warn(&hdev->pdev->dev,
					 "Async Q full, dropping msg(%d)\n",
					 req->msg[1]);
				break;
			}

			/* tail the async message in arq */
			msg_q = hdev->arq.msg_q[hdev->arq.tail];
			memcpy(&msg_q[0], req->msg,
			       HCLGE_MBX_MAX_ARQ_MSG_SIZE * sizeof(u16));
			hclge_mbx_tail_ptr_move_arq(hdev->arq);
			hdev->arq.count++;

			hclgevf_mbx_task_schedule(hdev);

			break;
		default:
			dev_err(&hdev->pdev->dev,
				"VF received unsupported(%d) mbx msg from PF\n",
				req->msg[0]);
=======
		switch (req->msg.code) {
		case HCLGE_MBX_PF_VF_RESP:
			hclgevf_handle_mbx_response(hdev, req);
			break;
		case HCLGE_MBX_LINK_STAT_CHANGE:
		case HCLGE_MBX_ASSERTING_RESET:
		case HCLGE_MBX_LINK_STAT_MODE:
		case HCLGE_MBX_PUSH_VLAN_INFO:
		case HCLGE_MBX_PUSH_PROMISC_INFO:
			hclgevf_handle_mbx_msg(hdev, req);
			break;
		default:
			dev_err(&hdev->pdev->dev,
				"VF received unsupported(%u) mbx msg from PF\n",
				req->msg.code);
>>>>>>> upstream/android-13
			break;
		}
		crq->desc[crq->next_to_use].flag = 0;
		hclge_mbx_ring_ptr_move_crq(crq);
	}

	/* Write back CMDQ_RQ header pointer, M7 need this pointer */
	hclgevf_write_dev(&hdev->hw, HCLGEVF_NIC_CRQ_HEAD_REG,
			  crq->next_to_use);
}

<<<<<<< HEAD
void hclgevf_mbx_async_handler(struct hclgevf_dev *hdev)
{
	u16 link_status;
	u16 *msg_q;
	u8 duplex;
	u32 speed;
	u32 tail;

	/* we can safely clear it now as we are at start of the async message
	 * processing
	 */
	hdev->mbx_event_pending = false;
=======
static void hclgevf_parse_promisc_info(struct hclgevf_dev *hdev,
				       u16 promisc_info)
{
	if (!promisc_info)
		dev_info(&hdev->pdev->dev,
			 "Promisc mode is closed by host for being untrusted.\n");
}

void hclgevf_mbx_async_handler(struct hclgevf_dev *hdev)
{
	enum hnae3_reset_type reset_type;
	u16 link_status, state;
	u16 *msg_q, *vlan_info;
	u8 duplex;
	u32 speed;
	u32 tail;
	u8 flag;
	u8 idx;
>>>>>>> upstream/android-13

	tail = hdev->arq.tail;

	/* process all the async queue messages */
	while (tail != hdev->arq.head) {
<<<<<<< HEAD
=======
		if (test_bit(HCLGEVF_STATE_CMD_DISABLE, &hdev->state)) {
			dev_info(&hdev->pdev->dev,
				 "vf crq need init in async\n");
			return;
		}

>>>>>>> upstream/android-13
		msg_q = hdev->arq.msg_q[hdev->arq.head];

		switch (msg_q[0]) {
		case HCLGE_MBX_LINK_STAT_CHANGE:
<<<<<<< HEAD
			link_status = le16_to_cpu(msg_q[1]);
			memcpy(&speed, &msg_q[2], sizeof(speed));
			duplex = (u8)le16_to_cpu(msg_q[4]);

			/* update upper layer with new link link status */
			hclgevf_update_link_status(hdev, link_status);
			hclgevf_update_speed_duplex(hdev, speed, duplex);

=======
			link_status = msg_q[1];
			memcpy(&speed, &msg_q[2], sizeof(speed));
			duplex = (u8)msg_q[4];
			flag = (u8)msg_q[5];

			/* update upper layer with new link link status */
			hclgevf_update_speed_duplex(hdev, speed, duplex);
			hclgevf_update_link_status(hdev, link_status);

			if (flag & HCLGE_MBX_PUSH_LINK_STATUS_EN)
				set_bit(HCLGEVF_STATE_PF_PUSH_LINK_STATUS,
					&hdev->state);

			break;
		case HCLGE_MBX_LINK_STAT_MODE:
			idx = (u8)msg_q[1];
			if (idx)
				memcpy(&hdev->hw.mac.supported, &msg_q[2],
				       sizeof(unsigned long));
			else
				memcpy(&hdev->hw.mac.advertising, &msg_q[2],
				       sizeof(unsigned long));
>>>>>>> upstream/android-13
			break;
		case HCLGE_MBX_ASSERTING_RESET:
			/* PF has asserted reset hence VF should go in pending
			 * state and poll for the hardware reset status till it
			 * has been completely reset. After this stack should
			 * eventually be re-initialized.
			 */
<<<<<<< HEAD
			hdev->nic.reset_level = HNAE3_VF_RESET;
=======
			reset_type = (enum hnae3_reset_type)msg_q[1];
			set_bit(reset_type, &hdev->reset_pending);
>>>>>>> upstream/android-13
			set_bit(HCLGEVF_RESET_PENDING, &hdev->reset_state);
			hclgevf_reset_task_schedule(hdev);

			break;
<<<<<<< HEAD
		default:
			dev_err(&hdev->pdev->dev,
				"fetched unsupported(%d) message from arq\n",
=======
		case HCLGE_MBX_PUSH_VLAN_INFO:
			state = msg_q[1];
			vlan_info = &msg_q[1];
			hclgevf_update_port_base_vlan_info(hdev, state,
							   (u8 *)vlan_info, 8);
			break;
		case HCLGE_MBX_PUSH_PROMISC_INFO:
			hclgevf_parse_promisc_info(hdev, msg_q[1]);
			break;
		default:
			dev_err(&hdev->pdev->dev,
				"fetched unsupported(%u) message from arq\n",
>>>>>>> upstream/android-13
				msg_q[0]);
			break;
		}

		hclge_mbx_head_ptr_move_arq(hdev->arq);
<<<<<<< HEAD
		hdev->arq.count--;
=======
		atomic_dec(&hdev->arq.count);
>>>>>>> upstream/android-13
		msg_q = hdev->arq.msg_q[hdev->arq.head];
	}
}
