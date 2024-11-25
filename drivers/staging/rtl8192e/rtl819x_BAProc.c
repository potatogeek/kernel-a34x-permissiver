<<<<<<< HEAD
/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 ******************************************************************************/
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Contact Information: wlanfae <wlanfae@realtek.com>
 */
>>>>>>> upstream/android-13
#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include <linux/etherdevice.h>
#include "rtllib.h"
#include "rtl819x_BA.h"

static void ActivateBAEntry(struct rtllib_device *ieee, struct ba_record *pBA,
			    u16 Time)
{
<<<<<<< HEAD
	pBA->bValid = true;
	if (Time != 0)
		mod_timer(&pBA->Timer, jiffies + msecs_to_jiffies(Time));
=======
	pBA->b_valid = true;
	if (Time != 0)
		mod_timer(&pBA->timer, jiffies + msecs_to_jiffies(Time));
>>>>>>> upstream/android-13
}

static void DeActivateBAEntry(struct rtllib_device *ieee, struct ba_record *pBA)
{
<<<<<<< HEAD
	pBA->bValid = false;
	del_timer_sync(&pBA->Timer);
=======
	pBA->b_valid = false;
	del_timer_sync(&pBA->timer);
>>>>>>> upstream/android-13
}

static u8 TxTsDeleteBA(struct rtllib_device *ieee, struct tx_ts_record *pTxTs)
{
	struct ba_record *pAdmittedBa = &pTxTs->TxAdmittedBARecord;
	struct ba_record *pPendingBa = &pTxTs->TxPendingBARecord;
	u8 bSendDELBA = false;

<<<<<<< HEAD
	if (pPendingBa->bValid) {
=======
	if (pPendingBa->b_valid) {
>>>>>>> upstream/android-13
		DeActivateBAEntry(ieee, pPendingBa);
		bSendDELBA = true;
	}

<<<<<<< HEAD
	if (pAdmittedBa->bValid) {
=======
	if (pAdmittedBa->b_valid) {
>>>>>>> upstream/android-13
		DeActivateBAEntry(ieee, pAdmittedBa);
		bSendDELBA = true;
	}
	return bSendDELBA;
}

static u8 RxTsDeleteBA(struct rtllib_device *ieee, struct rx_ts_record *pRxTs)
{
<<<<<<< HEAD
	struct ba_record *pBa = &pRxTs->RxAdmittedBARecord;
	u8			bSendDELBA = false;

	if (pBa->bValid) {
=======
	struct ba_record *pBa = &pRxTs->rx_admitted_ba_record;
	u8			bSendDELBA = false;

	if (pBa->b_valid) {
>>>>>>> upstream/android-13
		DeActivateBAEntry(ieee, pBa);
		bSendDELBA = true;
	}

	return bSendDELBA;
}

void ResetBaEntry(struct ba_record *pBA)
{
<<<<<<< HEAD
	pBA->bValid			= false;
	pBA->BaParamSet.shortData	= 0;
	pBA->BaTimeoutValue		= 0;
	pBA->DialogToken		= 0;
	pBA->BaStartSeqCtrl.ShortData	= 0;
=======
	pBA->b_valid			  = false;
	pBA->ba_param_set.short_data	  = 0;
	pBA->ba_timeout_value		  = 0;
	pBA->dialog_token		  = 0;
	pBA->ba_start_seq_ctrl.short_data = 0;
>>>>>>> upstream/android-13
}
static struct sk_buff *rtllib_ADDBA(struct rtllib_device *ieee, u8 *Dst,
				    struct ba_record *pBA,
				    u16 StatusCode, u8 type)
{
	struct sk_buff *skb = NULL;
	struct rtllib_hdr_3addr *BAReq = NULL;
	u8 *tag = NULL;
	u16 len = ieee->tx_headroom + 9;

	netdev_dbg(ieee->dev, "%s(): frame(%d) sentd to: %pM, ieee->dev:%p\n",
		   __func__, type, Dst, ieee->dev);

	if (!pBA) {
		netdev_warn(ieee->dev, "pBA is NULL\n");
		return NULL;
	}
	skb = dev_alloc_skb(len + sizeof(struct rtllib_hdr_3addr));
	if (!skb)
		return NULL;

	memset(skb->data, 0, sizeof(struct rtllib_hdr_3addr));

	skb_reserve(skb, ieee->tx_headroom);

	BAReq = skb_put(skb, sizeof(struct rtllib_hdr_3addr));

	ether_addr_copy(BAReq->addr1, Dst);
	ether_addr_copy(BAReq->addr2, ieee->dev->dev_addr);

	ether_addr_copy(BAReq->addr3, ieee->current_network.bssid);
	BAReq->frame_ctl = cpu_to_le16(RTLLIB_STYPE_MANAGE_ACT);

	tag = skb_put(skb, 9);
	*tag++ = ACT_CAT_BA;
	*tag++ = type;
<<<<<<< HEAD
	*tag++ = pBA->DialogToken;
=======
	*tag++ = pBA->dialog_token;
>>>>>>> upstream/android-13

	if (type == ACT_ADDBARSP) {
		RT_TRACE(COMP_DBG, "====>to send ADDBARSP\n");

		put_unaligned_le16(StatusCode, tag);
		tag += 2;
	}

<<<<<<< HEAD
	put_unaligned_le16(pBA->BaParamSet.shortData, tag);
	tag += 2;

	put_unaligned_le16(pBA->BaTimeoutValue, tag);
	tag += 2;

	if (type == ACT_ADDBAREQ) {
		memcpy(tag, (u8 *)&(pBA->BaStartSeqCtrl), 2);
=======
	put_unaligned_le16(pBA->ba_param_set.short_data, tag);
	tag += 2;

	put_unaligned_le16(pBA->ba_timeout_value, tag);
	tag += 2;

	if (type == ACT_ADDBAREQ) {
		memcpy(tag, (u8 *)&(pBA->ba_start_seq_ctrl), 2);
>>>>>>> upstream/android-13
		tag += 2;
	}

#ifdef VERBOSE_DEBUG
<<<<<<< HEAD
	print_hex_dump_bytes("rtllib_ADDBA(): ", DUMP_PREFIX_NONE, skb->data,
			     skb->len);
=======
	print_hex_dump_bytes("%s: ", DUMP_PREFIX_NONE, skb->data,
			     __func__, skb->len);
>>>>>>> upstream/android-13
#endif
	return skb;
}

static struct sk_buff *rtllib_DELBA(struct rtllib_device *ieee, u8 *dst,
				    struct ba_record *pBA,
				    enum tr_select TxRxSelect, u16 ReasonCode)
{
	union delba_param_set DelbaParamSet;
	struct sk_buff *skb = NULL;
<<<<<<< HEAD
	 struct rtllib_hdr_3addr *Delba = NULL;
=======
	struct rtllib_hdr_3addr *Delba = NULL;
>>>>>>> upstream/android-13
	u8 *tag = NULL;
	u16 len = 6 + ieee->tx_headroom;

	if (net_ratelimit())
		netdev_dbg(ieee->dev, "%s(): ReasonCode(%d) sentd to: %pM\n",
			   __func__, ReasonCode, dst);

	memset(&DelbaParamSet, 0, 2);

<<<<<<< HEAD
	DelbaParamSet.field.Initiator = (TxRxSelect == TX_DIR) ? 1 : 0;
	DelbaParamSet.field.TID	= pBA->BaParamSet.field.TID;
=======
	DelbaParamSet.field.initiator = (TxRxSelect == TX_DIR) ? 1 : 0;
	DelbaParamSet.field.tid	= pBA->ba_param_set.field.tid;
>>>>>>> upstream/android-13

	skb = dev_alloc_skb(len + sizeof(struct rtllib_hdr_3addr));
	if (!skb)
		return NULL;

	skb_reserve(skb, ieee->tx_headroom);

	Delba = skb_put(skb, sizeof(struct rtllib_hdr_3addr));

	ether_addr_copy(Delba->addr1, dst);
	ether_addr_copy(Delba->addr2, ieee->dev->dev_addr);
	ether_addr_copy(Delba->addr3, ieee->current_network.bssid);
	Delba->frame_ctl = cpu_to_le16(RTLLIB_STYPE_MANAGE_ACT);

	tag = skb_put(skb, 6);

	*tag++ = ACT_CAT_BA;
	*tag++ = ACT_DELBA;


<<<<<<< HEAD
	put_unaligned_le16(DelbaParamSet.shortData, tag);
=======
	put_unaligned_le16(DelbaParamSet.short_data, tag);
>>>>>>> upstream/android-13
	tag += 2;

	put_unaligned_le16(ReasonCode, tag);
	tag += 2;

#ifdef VERBOSE_DEBUG
<<<<<<< HEAD
	print_hex_dump_bytes("rtllib_DELBA(): ", DUMP_PREFIX_NONE, skb->data,
			     skb->len);
=======
	print_hex_dump_bytes("%s: ", DUMP_PREFIX_NONE, skb->data,
			     __func__, skb->len);
>>>>>>> upstream/android-13
#endif
	return skb;
}

static void rtllib_send_ADDBAReq(struct rtllib_device *ieee, u8 *dst,
				 struct ba_record *pBA)
{
	struct sk_buff *skb;

	skb = rtllib_ADDBA(ieee, dst, pBA, 0, ACT_ADDBAREQ);

	if (skb) {
		RT_TRACE(COMP_DBG, "====>to send ADDBAREQ!!!!!\n");
		softmac_mgmt_xmit(skb, ieee);
	} else {
		netdev_dbg(ieee->dev, "Failed to generate ADDBAReq packet.\n");
	}
}

static void rtllib_send_ADDBARsp(struct rtllib_device *ieee, u8 *dst,
				 struct ba_record *pBA, u16 StatusCode)
{
	struct sk_buff *skb;

	skb = rtllib_ADDBA(ieee, dst, pBA, StatusCode, ACT_ADDBARSP);
	if (skb)
		softmac_mgmt_xmit(skb, ieee);
	else
		netdev_dbg(ieee->dev, "Failed to generate ADDBARsp packet.\n");
}

static void rtllib_send_DELBA(struct rtllib_device *ieee, u8 *dst,
			      struct ba_record *pBA, enum tr_select TxRxSelect,
			      u16 ReasonCode)
{
	struct sk_buff *skb;

	skb = rtllib_DELBA(ieee, dst, pBA, TxRxSelect, ReasonCode);
	if (skb)
		softmac_mgmt_xmit(skb, ieee);
	else
		netdev_dbg(ieee->dev, "Failed to generate DELBA packet.\n");
}

int rtllib_rx_ADDBAReq(struct rtllib_device *ieee, struct sk_buff *skb)
{
	struct rtllib_hdr_3addr *req = NULL;
	u16 rc = 0;
	u8 *dst = NULL, *pDialogToken = NULL, *tag = NULL;
	struct ba_record *pBA = NULL;
	union ba_param_set *pBaParamSet = NULL;
	u16 *pBaTimeoutVal = NULL;
	union sequence_control *pBaStartSeqCtrl = NULL;
	struct rx_ts_record *pTS = NULL;

	if (skb->len < sizeof(struct rtllib_hdr_3addr) + 9) {
		netdev_warn(ieee->dev, "Invalid skb len in BAREQ(%d / %d)\n",
			    (int)skb->len,
			    (int)(sizeof(struct rtllib_hdr_3addr) + 9));
		return -1;
	}

#ifdef VERBOSE_DEBUG
<<<<<<< HEAD
	print_hex_dump_bytes("rtllib_rx_ADDBAReq(): ", DUMP_PREFIX_NONE,
=======
	print_hex_dump_bytes("%s: ", DUMP_PREFIX_NONE, __func__,
>>>>>>> upstream/android-13
			     skb->data, skb->len);
#endif

	req = (struct rtllib_hdr_3addr *) skb->data;
	tag = (u8 *)req;
	dst = (u8 *)(&req->addr2[0]);
	tag += sizeof(struct rtllib_hdr_3addr);
	pDialogToken = tag + 2;
	pBaParamSet = (union ba_param_set *)(tag + 3);
	pBaTimeoutVal = (u16 *)(tag + 5);
	pBaStartSeqCtrl = (union sequence_control *)(req + 7);

	RT_TRACE(COMP_DBG, "====>rx ADDBAREQ from : %pM\n", dst);
	if (!ieee->current_network.qos_data.active ||
	    !ieee->pHTInfo->bCurrentHTSupport ||
	    (ieee->pHTInfo->IOTAction & HT_IOT_ACT_REJECT_ADDBA_REQ)) {
		rc = ADDBA_STATUS_REFUSED;
		netdev_warn(ieee->dev,
			    "Failed to reply on ADDBA_REQ as some capability is not ready(%d, %d)\n",
			    ieee->current_network.qos_data.active,
			    ieee->pHTInfo->bCurrentHTSupport);
		goto OnADDBAReq_Fail;
	}
	if (!GetTs(ieee, (struct ts_common_info **)(&pTS), dst,
<<<<<<< HEAD
	    (u8)(pBaParamSet->field.TID), RX_DIR, true)) {
=======
	    (u8)(pBaParamSet->field.tid), RX_DIR, true)) {
>>>>>>> upstream/android-13
		rc = ADDBA_STATUS_REFUSED;
		netdev_warn(ieee->dev, "%s(): can't get TS\n", __func__);
		goto OnADDBAReq_Fail;
	}
<<<<<<< HEAD
	pBA = &pTS->RxAdmittedBARecord;

	if (pBaParamSet->field.BAPolicy == BA_POLICY_DELAYED) {
=======
	pBA = &pTS->rx_admitted_ba_record;

	if (pBaParamSet->field.ba_policy == BA_POLICY_DELAYED) {
>>>>>>> upstream/android-13
		rc = ADDBA_STATUS_INVALID_PARAM;
		netdev_warn(ieee->dev, "%s(): BA Policy is not correct\n",
			    __func__);
		goto OnADDBAReq_Fail;
	}

	rtllib_FlushRxTsPendingPkts(ieee, pTS);

	DeActivateBAEntry(ieee, pBA);
<<<<<<< HEAD
	pBA->DialogToken = *pDialogToken;
	pBA->BaParamSet = *pBaParamSet;
	pBA->BaTimeoutValue = *pBaTimeoutVal;
	pBA->BaStartSeqCtrl = *pBaStartSeqCtrl;

	if (ieee->GetHalfNmodeSupportByAPsHandler(ieee->dev) ||
	   (ieee->pHTInfo->IOTAction & HT_IOT_ACT_ALLOW_PEER_AGG_ONE_PKT))
		pBA->BaParamSet.field.BufferSize = 1;
	else
		pBA->BaParamSet.field.BufferSize = 32;
=======
	pBA->dialog_token = *pDialogToken;
	pBA->ba_param_set = *pBaParamSet;
	pBA->ba_timeout_value = *pBaTimeoutVal;
	pBA->ba_start_seq_ctrl = *pBaStartSeqCtrl;

	if (ieee->GetHalfNmodeSupportByAPsHandler(ieee->dev) ||
	   (ieee->pHTInfo->IOTAction & HT_IOT_ACT_ALLOW_PEER_AGG_ONE_PKT))
		pBA->ba_param_set.field.buffer_size = 1;
	else
		pBA->ba_param_set.field.buffer_size = 32;
>>>>>>> upstream/android-13

	ActivateBAEntry(ieee, pBA, 0);
	rtllib_send_ADDBARsp(ieee, dst, pBA, ADDBA_STATUS_SUCCESS);

	return 0;

OnADDBAReq_Fail:
	{
		struct ba_record BA;

<<<<<<< HEAD
		BA.BaParamSet = *pBaParamSet;
		BA.BaTimeoutValue = *pBaTimeoutVal;
		BA.DialogToken = *pDialogToken;
		BA.BaParamSet.field.BAPolicy = BA_POLICY_IMMEDIATE;
=======
		BA.ba_param_set = *pBaParamSet;
		BA.ba_timeout_value = *pBaTimeoutVal;
		BA.dialog_token = *pDialogToken;
		BA.ba_param_set.field.ba_policy = BA_POLICY_IMMEDIATE;
>>>>>>> upstream/android-13
		rtllib_send_ADDBARsp(ieee, dst, &BA, rc);
		return 0;
	}
}

int rtllib_rx_ADDBARsp(struct rtllib_device *ieee, struct sk_buff *skb)
{
<<<<<<< HEAD
	 struct rtllib_hdr_3addr *rsp = NULL;
=======
	struct rtllib_hdr_3addr *rsp = NULL;
>>>>>>> upstream/android-13
	struct ba_record *pPendingBA, *pAdmittedBA;
	struct tx_ts_record *pTS = NULL;
	u8 *dst = NULL, *pDialogToken = NULL, *tag = NULL;
	u16 *pStatusCode = NULL, *pBaTimeoutVal = NULL;
	union ba_param_set *pBaParamSet = NULL;
	u16			ReasonCode;

	if (skb->len < sizeof(struct rtllib_hdr_3addr) + 9) {
		netdev_warn(ieee->dev, "Invalid skb len in BARSP(%d / %d)\n",
			    (int)skb->len,
			    (int)(sizeof(struct rtllib_hdr_3addr) + 9));
		return -1;
	}
	rsp = (struct rtllib_hdr_3addr *)skb->data;
	tag = (u8 *)rsp;
	dst = (u8 *)(&rsp->addr2[0]);
	tag += sizeof(struct rtllib_hdr_3addr);
	pDialogToken = tag + 2;
	pStatusCode = (u16 *)(tag + 3);
	pBaParamSet = (union ba_param_set *)(tag + 5);
	pBaTimeoutVal = (u16 *)(tag + 7);

	RT_TRACE(COMP_DBG, "====>rx ADDBARSP from : %pM\n", dst);
	if (!ieee->current_network.qos_data.active ||
	    !ieee->pHTInfo->bCurrentHTSupport ||
	    !ieee->pHTInfo->bCurrentAMPDUEnable) {
		netdev_warn(ieee->dev,
			    "reject to ADDBA_RSP as some capability is not ready(%d, %d, %d)\n",
			    ieee->current_network.qos_data.active,
			    ieee->pHTInfo->bCurrentHTSupport,
			    ieee->pHTInfo->bCurrentAMPDUEnable);
		ReasonCode = DELBA_REASON_UNKNOWN_BA;
		goto OnADDBARsp_Reject;
	}


	if (!GetTs(ieee, (struct ts_common_info **)(&pTS), dst,
<<<<<<< HEAD
		   (u8)(pBaParamSet->field.TID), TX_DIR, false)) {
=======
		   (u8)(pBaParamSet->field.tid), TX_DIR, false)) {
>>>>>>> upstream/android-13
		netdev_warn(ieee->dev, "%s(): can't get TS\n", __func__);
		ReasonCode = DELBA_REASON_UNKNOWN_BA;
		goto OnADDBARsp_Reject;
	}

	pTS->bAddBaReqInProgress = false;
	pPendingBA = &pTS->TxPendingBARecord;
	pAdmittedBA = &pTS->TxAdmittedBARecord;


<<<<<<< HEAD
	if (pAdmittedBA->bValid) {
		netdev_dbg(ieee->dev, "%s(): ADDBA response already admitted\n",
			   __func__);
		return -1;
	} else if (!pPendingBA->bValid ||
		   (*pDialogToken != pPendingBA->DialogToken)) {
=======
	if (pAdmittedBA->b_valid) {
		netdev_dbg(ieee->dev, "%s(): ADDBA response already admitted\n",
			   __func__);
		return -1;
	} else if (!pPendingBA->b_valid ||
		   (*pDialogToken != pPendingBA->dialog_token)) {
>>>>>>> upstream/android-13
		netdev_warn(ieee->dev,
			    "%s(): ADDBA Rsp. BA invalid, DELBA!\n",
			    __func__);
		ReasonCode = DELBA_REASON_UNKNOWN_BA;
		goto OnADDBARsp_Reject;
	} else {
		netdev_dbg(ieee->dev,
			   "%s(): Recv ADDBA Rsp. BA is admitted! Status code:%X\n",
			   __func__, *pStatusCode);
		DeActivateBAEntry(ieee, pPendingBA);
	}


	if (*pStatusCode == ADDBA_STATUS_SUCCESS) {
<<<<<<< HEAD
		if (pBaParamSet->field.BAPolicy == BA_POLICY_DELAYED) {
=======
		if (pBaParamSet->field.ba_policy == BA_POLICY_DELAYED) {
>>>>>>> upstream/android-13
			pTS->bAddBaReqDelayed = true;
			DeActivateBAEntry(ieee, pAdmittedBA);
			ReasonCode = DELBA_REASON_END_BA;
			goto OnADDBARsp_Reject;
		}


<<<<<<< HEAD
		pAdmittedBA->DialogToken = *pDialogToken;
		pAdmittedBA->BaTimeoutValue = *pBaTimeoutVal;
		pAdmittedBA->BaStartSeqCtrl = pPendingBA->BaStartSeqCtrl;
		pAdmittedBA->BaParamSet = *pBaParamSet;
=======
		pAdmittedBA->dialog_token = *pDialogToken;
		pAdmittedBA->ba_timeout_value = *pBaTimeoutVal;
		pAdmittedBA->ba_start_seq_ctrl = pPendingBA->ba_start_seq_ctrl;
		pAdmittedBA->ba_param_set = *pBaParamSet;
>>>>>>> upstream/android-13
		DeActivateBAEntry(ieee, pAdmittedBA);
		ActivateBAEntry(ieee, pAdmittedBA, *pBaTimeoutVal);
	} else {
		pTS->bAddBaReqDelayed = true;
		pTS->bDisable_AddBa = true;
		ReasonCode = DELBA_REASON_END_BA;
		goto OnADDBARsp_Reject;
	}

	return 0;

OnADDBARsp_Reject:
	{
		struct ba_record BA;

<<<<<<< HEAD
		BA.BaParamSet = *pBaParamSet;
=======
		BA.ba_param_set = *pBaParamSet;
>>>>>>> upstream/android-13
		rtllib_send_DELBA(ieee, dst, &BA, TX_DIR, ReasonCode);
		return 0;
	}
}

int rtllib_rx_DELBA(struct rtllib_device *ieee, struct sk_buff *skb)
{
<<<<<<< HEAD
	 struct rtllib_hdr_3addr *delba = NULL;
=======
	struct rtllib_hdr_3addr *delba = NULL;
>>>>>>> upstream/android-13
	union delba_param_set *pDelBaParamSet = NULL;
	u8 *dst = NULL;

	if (skb->len < sizeof(struct rtllib_hdr_3addr) + 6) {
		netdev_warn(ieee->dev, "Invalid skb len in DELBA(%d / %d)\n",
			    (int)skb->len,
			    (int)(sizeof(struct rtllib_hdr_3addr) + 6));
		return -1;
	}

	if (!ieee->current_network.qos_data.active ||
		!ieee->pHTInfo->bCurrentHTSupport) {
		netdev_warn(ieee->dev,
			    "received DELBA while QOS or HT is not supported(%d, %d)\n",
			    ieee->current_network. qos_data.active,
			    ieee->pHTInfo->bCurrentHTSupport);
		return -1;
	}

#ifdef VERBOSE_DEBUG
<<<<<<< HEAD
	print_hex_dump_bytes("rtllib_rx_DELBA(): ", DUMP_PREFIX_NONE, skb->data,
			     skb->len);
=======
	print_hex_dump_bytes("%s: ", DUMP_PREFIX_NONE, skb->data,
			     __func__, skb->len);
>>>>>>> upstream/android-13
#endif
	delba = (struct rtllib_hdr_3addr *)skb->data;
	dst = (u8 *)(&delba->addr2[0]);
	pDelBaParamSet = (union delba_param_set *)&delba->payload[2];

<<<<<<< HEAD
	if (pDelBaParamSet->field.Initiator == 1) {
		struct rx_ts_record *pRxTs;

		if (!GetTs(ieee, (struct ts_common_info **)&pRxTs, dst,
		    (u8)pDelBaParamSet->field.TID, RX_DIR, false)) {
			netdev_warn(ieee->dev,
				    "%s(): can't get TS for RXTS. dst:%pM TID:%d\n",
				    __func__, dst,
				    (u8)pDelBaParamSet->field.TID);
=======
	if (pDelBaParamSet->field.initiator == 1) {
		struct rx_ts_record *pRxTs;

		if (!GetTs(ieee, (struct ts_common_info **)&pRxTs, dst,
		    (u8)pDelBaParamSet->field.tid, RX_DIR, false)) {
			netdev_warn(ieee->dev,
				    "%s(): can't get TS for RXTS. dst:%pM TID:%d\n",
				    __func__, dst,
				    (u8)pDelBaParamSet->field.tid);
>>>>>>> upstream/android-13
			return -1;
		}

		RxTsDeleteBA(ieee, pRxTs);
	} else {
		struct tx_ts_record *pTxTs;

		if (!GetTs(ieee, (struct ts_common_info **)&pTxTs, dst,
<<<<<<< HEAD
			   (u8)pDelBaParamSet->field.TID, TX_DIR, false)) {
=======
			   (u8)pDelBaParamSet->field.tid, TX_DIR, false)) {
>>>>>>> upstream/android-13
			netdev_warn(ieee->dev, "%s(): can't get TS for TXTS\n",
				    __func__);
			return -1;
		}

		pTxTs->bUsingBa = false;
		pTxTs->bAddBaReqInProgress = false;
		pTxTs->bAddBaReqDelayed = false;
		del_timer_sync(&pTxTs->TsAddBaTimer);
		TxTsDeleteBA(ieee, pTxTs);
	}
	return 0;
}

void TsInitAddBA(struct rtllib_device *ieee, struct tx_ts_record *pTS,
		 u8 Policy, u8	bOverwritePending)
{
	struct ba_record *pBA = &pTS->TxPendingBARecord;

<<<<<<< HEAD
	if (pBA->bValid && !bOverwritePending)
=======
	if (pBA->b_valid && !bOverwritePending)
>>>>>>> upstream/android-13
		return;

	DeActivateBAEntry(ieee, pBA);

<<<<<<< HEAD
	pBA->DialogToken++;
	pBA->BaParamSet.field.AMSDU_Support = 0;
	pBA->BaParamSet.field.BAPolicy = Policy;
	pBA->BaParamSet.field.TID =
			 pTS->TsCommonInfo.TSpec.f.TSInfo.field.ucTSID;
	pBA->BaParamSet.field.BufferSize = 32;
	pBA->BaTimeoutValue = 0;
	pBA->BaStartSeqCtrl.field.SeqNum = (pTS->TxCurSeq + 3) % 4096;
=======
	pBA->dialog_token++;
	pBA->ba_param_set.field.amsdu_support = 0;
	pBA->ba_param_set.field.ba_policy = Policy;
	pBA->ba_param_set.field.tid = pTS->TsCommonInfo.TSpec.f.TSInfo.field.ucTSID;
	pBA->ba_param_set.field.buffer_size = 32;
	pBA->ba_timeout_value = 0;
	pBA->ba_start_seq_ctrl.field.seq_num = (pTS->TxCurSeq + 3) % 4096;
>>>>>>> upstream/android-13

	ActivateBAEntry(ieee, pBA, BA_SETUP_TIMEOUT);

	rtllib_send_ADDBAReq(ieee, pTS->TsCommonInfo.Addr, pBA);
}

void TsInitDelBA(struct rtllib_device *ieee,
		 struct ts_common_info *pTsCommonInfo,
		 enum tr_select TxRxSelect)
{
	if (TxRxSelect == TX_DIR) {
		struct tx_ts_record *pTxTs =
			 (struct tx_ts_record *)pTsCommonInfo;

		if (TxTsDeleteBA(ieee, pTxTs))
			rtllib_send_DELBA(ieee, pTsCommonInfo->Addr,
<<<<<<< HEAD
					  (pTxTs->TxAdmittedBARecord.bValid) ?
=======
					  (pTxTs->TxAdmittedBARecord.b_valid) ?
>>>>>>> upstream/android-13
					 (&pTxTs->TxAdmittedBARecord) :
					(&pTxTs->TxPendingBARecord),
					 TxRxSelect, DELBA_REASON_END_BA);
	} else if (TxRxSelect == RX_DIR) {
		struct rx_ts_record *pRxTs =
				 (struct rx_ts_record *)pTsCommonInfo;
		if (RxTsDeleteBA(ieee, pRxTs))
			rtllib_send_DELBA(ieee, pTsCommonInfo->Addr,
<<<<<<< HEAD
					  &pRxTs->RxAdmittedBARecord,
=======
					  &pRxTs->rx_admitted_ba_record,
>>>>>>> upstream/android-13
					  TxRxSelect, DELBA_REASON_END_BA);
	}
}

void BaSetupTimeOut(struct timer_list *t)
{
	struct tx_ts_record *pTxTs = from_timer(pTxTs, t,
<<<<<<< HEAD
					      TxPendingBARecord.Timer);

	pTxTs->bAddBaReqInProgress = false;
	pTxTs->bAddBaReqDelayed = true;
	pTxTs->TxPendingBARecord.bValid = false;
=======
					      TxPendingBARecord.timer);

	pTxTs->bAddBaReqInProgress = false;
	pTxTs->bAddBaReqDelayed = true;
	pTxTs->TxPendingBARecord.b_valid = false;
>>>>>>> upstream/android-13
}

void TxBaInactTimeout(struct timer_list *t)
{
	struct tx_ts_record *pTxTs = from_timer(pTxTs, t,
<<<<<<< HEAD
					      TxAdmittedBARecord.Timer);
=======
					      TxAdmittedBARecord.timer);
>>>>>>> upstream/android-13
	struct rtllib_device *ieee = container_of(pTxTs, struct rtllib_device,
				     TxTsRecord[pTxTs->num]);
	TxTsDeleteBA(ieee, pTxTs);
	rtllib_send_DELBA(ieee, pTxTs->TsCommonInfo.Addr,
			  &pTxTs->TxAdmittedBARecord, TX_DIR,
			  DELBA_REASON_TIMEOUT);
}

void RxBaInactTimeout(struct timer_list *t)
{
	struct rx_ts_record *pRxTs = from_timer(pRxTs, t,
<<<<<<< HEAD
					      RxAdmittedBARecord.Timer);
=======
					      rx_admitted_ba_record.timer);
>>>>>>> upstream/android-13
	struct rtllib_device *ieee = container_of(pRxTs, struct rtllib_device,
				     RxTsRecord[pRxTs->num]);

	RxTsDeleteBA(ieee, pRxTs);
<<<<<<< HEAD
	rtllib_send_DELBA(ieee, pRxTs->TsCommonInfo.Addr,
			  &pRxTs->RxAdmittedBARecord, RX_DIR,
=======
	rtllib_send_DELBA(ieee, pRxTs->ts_common_info.Addr,
			  &pRxTs->rx_admitted_ba_record, RX_DIR,
>>>>>>> upstream/android-13
			  DELBA_REASON_TIMEOUT);
}
