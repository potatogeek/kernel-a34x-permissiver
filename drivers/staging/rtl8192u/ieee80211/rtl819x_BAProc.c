// SPDX-License-Identifier: GPL-2.0
/********************************************************************************************************************************
 * This file is created to process BA Action Frame. According to 802.11 spec, there are 3 BA action types at all. And as BA is
 * related to TS, this part need some structure defined in QOS side code. Also TX RX is going to be resturctured, so how to send
 * ADDBAREQ ADDBARSP and DELBA packet is still on consideration. Temporarily use MANAGE QUEUE instead of Normal Queue.
 * WB 2008-05-27
 * *****************************************************************************************************************************/
#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include "ieee80211.h"
#include "rtl819x_BA.h"

/********************************************************************************************************************
 *function:  Activate BA entry. And if Time is nozero, start timer.
<<<<<<< HEAD
 *   input:  PBA_RECORD			pBA  //BA entry to be enabled
 *	     u16			Time //indicate time delay.
 *  output:  none
 ********************************************************************************************************************/
static void ActivateBAEntry(struct ieee80211_device *ieee, PBA_RECORD pBA, u16 Time)
{
	pBA->bValid = true;
	if (Time != 0)
		mod_timer(&pBA->Timer, jiffies + msecs_to_jiffies(Time));
=======
 *   input:  struct ba_record          *pBA  //BA entry to be enabled
 *	     u16			Time //indicate time delay.
 *  output:  none
 ********************************************************************************************************************/
static void ActivateBAEntry(struct ieee80211_device *ieee, struct ba_record *pBA, u16 Time)
{
	pBA->valid = true;
	if (Time != 0)
		mod_timer(&pBA->timer, jiffies + msecs_to_jiffies(Time));
>>>>>>> upstream/android-13
}

/********************************************************************************************************************
 *function:  deactivate BA entry, including its timer.
<<<<<<< HEAD
 *   input:  PBA_RECORD			pBA  //BA entry to be disabled
 *  output:  none
 ********************************************************************************************************************/
static void DeActivateBAEntry(struct ieee80211_device *ieee, PBA_RECORD pBA)
{
	pBA->bValid = false;
	del_timer_sync(&pBA->Timer);
=======
 *   input:  struct ba_record       *pBA  //BA entry to be disabled
 *  output:  none
 ********************************************************************************************************************/
static void DeActivateBAEntry(struct ieee80211_device *ieee, struct ba_record *pBA)
{
	pBA->valid = false;
	del_timer_sync(&pBA->timer);
>>>>>>> upstream/android-13
}
/********************************************************************************************************************
 *function: deactivete BA entry in Tx Ts, and send DELBA.
 *   input:
 *	     struct tx_ts_record *pTxTs //Tx Ts which is to deactivate BA entry.
 *  output:  none
 *  notice:  As struct tx_ts_record * structure will be defined in QOS, so wait to be merged. //FIXME
 ********************************************************************************************************************/
static u8 TxTsDeleteBA(struct ieee80211_device *ieee, struct tx_ts_record *pTxTs)
{
<<<<<<< HEAD
	PBA_RECORD		pAdmittedBa = &pTxTs->tx_admitted_ba_record;  //These two BA entries must exist in TS structure
	PBA_RECORD		pPendingBa = &pTxTs->tx_pending_ba_record;
	u8			bSendDELBA = false;

	// Delete pending BA
	if (pPendingBa->bValid) {
=======
	struct ba_record *pAdmittedBa = &pTxTs->tx_admitted_ba_record;  //These two BA entries must exist in TS structure
	struct ba_record *pPendingBa = &pTxTs->tx_pending_ba_record;
	u8			bSendDELBA = false;

	// Delete pending BA
	if (pPendingBa->valid) {
>>>>>>> upstream/android-13
		DeActivateBAEntry(ieee, pPendingBa);
		bSendDELBA = true;
	}

	// Delete admitted BA
<<<<<<< HEAD
	if (pAdmittedBa->bValid) {
=======
	if (pAdmittedBa->valid) {
>>>>>>> upstream/android-13
		DeActivateBAEntry(ieee, pAdmittedBa);
		bSendDELBA = true;
	}

	return bSendDELBA;
}

/********************************************************************************************************************
 *function: deactivete BA entry in Tx Ts, and send DELBA.
 *   input:
 *	     struct rx_ts_record  *pRxTs //Rx Ts which is to deactivate BA entry.
 *  output:  none
 *  notice:  As struct rx_ts_record * structure will be defined in QOS, so wait to be merged. //FIXME, same with above
 ********************************************************************************************************************/
static u8 RxTsDeleteBA(struct ieee80211_device *ieee, struct rx_ts_record *pRxTs)
{
<<<<<<< HEAD
	PBA_RECORD		pBa = &pRxTs->rx_admitted_ba_record;
	u8			bSendDELBA = false;

	if (pBa->bValid) {
=======
	struct ba_record       *pBa = &pRxTs->rx_admitted_ba_record;
	u8			bSendDELBA = false;

	if (pBa->valid) {
>>>>>>> upstream/android-13
		DeActivateBAEntry(ieee, pBa);
		bSendDELBA = true;
	}

	return bSendDELBA;
}

/********************************************************************************************************************
 *function: reset BA entry
 *   input:
<<<<<<< HEAD
 *	     PBA_RECORD		pBA //entry to be reset
 *  output:  none
 ********************************************************************************************************************/
void ResetBaEntry(PBA_RECORD pBA)
{
	pBA->bValid			= false;
	pBA->BaParamSet.shortData	= 0;
	pBA->BaTimeoutValue		= 0;
	pBA->DialogToken		= 0;
	pBA->BaStartSeqCtrl.ShortData	= 0;
=======
 *	     struct ba_record *pBA //entry to be reset
 *  output:  none
 ********************************************************************************************************************/
void ResetBaEntry(struct ba_record *pBA)
{
	pBA->valid			= false;
	pBA->param_set.short_data	= 0;
	pBA->timeout_value		= 0;
	pBA->dialog_token		= 0;
	pBA->start_seq_ctrl.short_data	= 0;
>>>>>>> upstream/android-13
}
//These functions need porting here or not?
/*******************************************************************************************************************************
 *function:  construct ADDBAREQ and ADDBARSP frame here together.
 *   input:  u8*		Dst	//ADDBA frame's destination
<<<<<<< HEAD
 *	     PBA_RECORD		pBA	//BA_RECORD entry which stores the necessary information for BA.
=======
 *	     struct ba_record  *pBA	//BA_RECORD entry which stores the necessary information for BA.
>>>>>>> upstream/android-13
 *	     u16		StatusCode  //status code in RSP and I will use it to indicate whether it's RSP or REQ(will I?)
 *	     u8			type	//indicate whether it's RSP(ACT_ADDBARSP) ow REQ(ACT_ADDBAREQ)
 *  output:  none
 *  return:  sk_buff*		skb     //return constructed skb to xmit
 *******************************************************************************************************************************/
<<<<<<< HEAD
static struct sk_buff *ieee80211_ADDBA(struct ieee80211_device *ieee, u8 *Dst, PBA_RECORD pBA, u16 StatusCode, u8 type)
{
	struct sk_buff *skb = NULL;
	 struct rtl_80211_hdr_3addr *BAReq = NULL;
=======
static struct sk_buff *ieee80211_ADDBA(struct ieee80211_device *ieee, u8 *Dst, struct ba_record *pBA, u16 StatusCode, u8 type)
{
	struct sk_buff *skb = NULL;
	struct rtl_80211_hdr_3addr *BAReq = NULL;
>>>>>>> upstream/android-13
	u8 *tag = NULL;
	u16 len = ieee->tx_headroom + 9;
	//category(1) + action field(1) + Dialog Token(1) + BA Parameter Set(2) +  BA Timeout Value(2) +  BA Start SeqCtrl(2)(or StatusCode(2))
	IEEE80211_DEBUG(IEEE80211_DL_TRACE | IEEE80211_DL_BA, "========>%s(), frame(%d) sentd to:%pM, ieee->dev:%p\n", __func__, type, Dst, ieee->dev);
	if (pBA == NULL) {
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "pBA is NULL\n");
		return NULL;
	}
	skb = dev_alloc_skb(len + sizeof(struct rtl_80211_hdr_3addr)); //need to add something others? FIXME
<<<<<<< HEAD
	if (!skb) {
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "can't alloc skb for ADDBA_REQ\n");
		return NULL;
	}
=======
	if (!skb)
		return NULL;
>>>>>>> upstream/android-13

	memset(skb->data, 0, sizeof(struct rtl_80211_hdr_3addr));	//I wonder whether it's necessary. Apparently kernel will not do it when alloc a skb.
	skb_reserve(skb, ieee->tx_headroom);

	BAReq = skb_put(skb, sizeof(struct rtl_80211_hdr_3addr));

	memcpy(BAReq->addr1, Dst, ETH_ALEN);
	memcpy(BAReq->addr2, ieee->dev->dev_addr, ETH_ALEN);

	memcpy(BAReq->addr3, ieee->current_network.bssid, ETH_ALEN);

	BAReq->frame_ctl = cpu_to_le16(IEEE80211_STYPE_MANAGE_ACT); //action frame

	//tag += sizeof( struct rtl_80211_hdr_3addr); //move to action field
	tag = skb_put(skb, 9);
	*tag++ = ACT_CAT_BA;
	*tag++ = type;
	// Dialog Token
<<<<<<< HEAD
	*tag++ = pBA->DialogToken;

	if (ACT_ADDBARSP == type) {
=======
	*tag++ = pBA->dialog_token;

	if (type == ACT_ADDBARSP) {
>>>>>>> upstream/android-13
		// Status Code
		netdev_info(ieee->dev, "=====>to send ADDBARSP\n");

		put_unaligned_le16(StatusCode, tag);
		tag += 2;
	}
	// BA Parameter Set

<<<<<<< HEAD
	put_unaligned_le16(pBA->BaParamSet.shortData, tag);
	tag += 2;
	// BA Timeout Value

	put_unaligned_le16(pBA->BaTimeoutValue, tag);
	tag += 2;

	if (ACT_ADDBAREQ == type) {
	// BA Start SeqCtrl
		memcpy(tag, (u8 *)&(pBA->BaStartSeqCtrl), 2);
		tag += 2;
	}

	IEEE80211_DEBUG_DATA(IEEE80211_DL_DATA|IEEE80211_DL_BA, skb->data, skb->len);
=======
	put_unaligned_le16(pBA->param_set.short_data, tag);
	tag += 2;
	// BA Timeout Value

	put_unaligned_le16(pBA->timeout_value, tag);
	tag += 2;

	if (type == ACT_ADDBAREQ) {
	// BA Start SeqCtrl
		memcpy(tag, (u8 *)&(pBA->start_seq_ctrl), 2);
		tag += 2;
	}

	IEEE80211_DEBUG_DATA(IEEE80211_DL_DATA | IEEE80211_DL_BA, skb->data, skb->len);
>>>>>>> upstream/android-13
	return skb;
	//return NULL;
}


/********************************************************************************************************************
 *function:  construct DELBA frame
 *   input:  u8*		dst	//DELBA frame's destination
<<<<<<< HEAD
 *	     PBA_RECORD		pBA	//BA_RECORD entry which stores the necessary information for BA
=======
 *	     struct ba_record  *pBA	//BA_RECORD entry which stores the necessary information for BA
>>>>>>> upstream/android-13
 *	     enum tr_select	TxRxSelect  //TX RX direction
 *	     u16		ReasonCode  //status code.
 *  output:  none
 *  return:  sk_buff*		skb     //return constructed skb to xmit
 ********************************************************************************************************************/
static struct sk_buff *ieee80211_DELBA(
	struct ieee80211_device  *ieee,
	u8		         *dst,
<<<<<<< HEAD
	PBA_RECORD		 pBA,
=======
	struct ba_record         *pBA,
>>>>>>> upstream/android-13
	enum tr_select		 TxRxSelect,
	u16			 ReasonCode
	)
{
<<<<<<< HEAD
	DELBA_PARAM_SET	DelbaParamSet;
	struct sk_buff *skb = NULL;
	 struct rtl_80211_hdr_3addr *Delba = NULL;
=======
	union delba_param_set	DelbaParamSet;
	struct sk_buff *skb = NULL;
	struct rtl_80211_hdr_3addr *Delba = NULL;
>>>>>>> upstream/android-13
	u8 *tag = NULL;
	//len = head len + DELBA Parameter Set(2) + Reason Code(2)
	u16 len = 6 + ieee->tx_headroom;

	if (net_ratelimit())
		IEEE80211_DEBUG(IEEE80211_DL_TRACE | IEEE80211_DL_BA,
				"========>%s(), ReasonCode(%d) sentd to:%pM\n",
				__func__, ReasonCode, dst);

	memset(&DelbaParamSet, 0, 2);

<<<<<<< HEAD
	DelbaParamSet.field.Initiator	= (TxRxSelect == TX_DIR) ? 1 : 0;
	DelbaParamSet.field.TID	= pBA->BaParamSet.field.TID;

	skb = dev_alloc_skb(len + sizeof(struct rtl_80211_hdr_3addr)); //need to add something others? FIXME
	if (!skb) {
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "can't alloc skb for ADDBA_REQ\n");
		return NULL;
	}
=======
	DelbaParamSet.field.initiator	= (TxRxSelect == TX_DIR) ? 1 : 0;
	DelbaParamSet.field.tid	= pBA->param_set.field.tid;

	skb = dev_alloc_skb(len + sizeof(struct rtl_80211_hdr_3addr)); //need to add something others? FIXME
	if (!skb)
		return NULL;
>>>>>>> upstream/android-13
//	memset(skb->data, 0, len+sizeof( struct rtl_80211_hdr_3addr));
	skb_reserve(skb, ieee->tx_headroom);

	Delba = skb_put(skb, sizeof(struct rtl_80211_hdr_3addr));

	memcpy(Delba->addr1, dst, ETH_ALEN);
	memcpy(Delba->addr2, ieee->dev->dev_addr, ETH_ALEN);
	memcpy(Delba->addr3, ieee->current_network.bssid, ETH_ALEN);
	Delba->frame_ctl = cpu_to_le16(IEEE80211_STYPE_MANAGE_ACT); //action frame

	tag = skb_put(skb, 6);

	*tag++ = ACT_CAT_BA;
	*tag++ = ACT_DELBA;

	// DELBA Parameter Set

<<<<<<< HEAD
	put_unaligned_le16(DelbaParamSet.shortData, tag);
=======
	put_unaligned_le16(DelbaParamSet.short_data, tag);
>>>>>>> upstream/android-13
	tag += 2;
	// Reason Code

	put_unaligned_le16(ReasonCode, tag);
	tag += 2;

<<<<<<< HEAD
	IEEE80211_DEBUG_DATA(IEEE80211_DL_DATA|IEEE80211_DL_BA, skb->data, skb->len);
=======
	IEEE80211_DEBUG_DATA(IEEE80211_DL_DATA | IEEE80211_DL_BA, skb->data, skb->len);
>>>>>>> upstream/android-13
	if (net_ratelimit())
		IEEE80211_DEBUG(IEEE80211_DL_TRACE | IEEE80211_DL_BA,
				"<=====%s()\n", __func__);
	return skb;
}

/********************************************************************************************************************
 *function: send ADDBAReq frame out
 *   input:  u8*		dst	//ADDBAReq frame's destination
<<<<<<< HEAD
 *	     PBA_RECORD		pBA	//BA_RECORD entry which stores the necessary information for BA
=======
 *	     struct ba_record  *pBA	//BA_RECORD entry which stores the necessary information for BA
>>>>>>> upstream/android-13
 *  output:  none
 *  notice: If any possible, please hide pBA in ieee. And temporarily use Manage Queue as softmac_mgmt_xmit() usually does
 ********************************************************************************************************************/
static void ieee80211_send_ADDBAReq(struct ieee80211_device *ieee,
<<<<<<< HEAD
				    u8 *dst, PBA_RECORD pBA)
=======
				    u8 *dst, struct ba_record *pBA)
>>>>>>> upstream/android-13
{
	struct sk_buff *skb;
	skb = ieee80211_ADDBA(ieee, dst, pBA, 0, ACT_ADDBAREQ); //construct ACT_ADDBAREQ frames so set statuscode zero.

	if (skb) {
		softmac_mgmt_xmit(skb, ieee);
		//add statistic needed here.
		//and skb will be freed in softmac_mgmt_xmit(), so omit all dev_kfree_skb_any() outside softmac_mgmt_xmit()
		//WB
	} else {
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "alloc skb error in function %s()\n", __func__);
	}
}

/********************************************************************************************************************
 *function: send ADDBARSP frame out
 *   input:  u8*		dst	//DELBA frame's destination
<<<<<<< HEAD
 *	     PBA_RECORD		pBA	//BA_RECORD entry which stores the necessary information for BA
=======
 *	     struct ba_record  *pBA	//BA_RECORD entry which stores the necessary information for BA
>>>>>>> upstream/android-13
 *	     u16		StatusCode //RSP StatusCode
 *  output:  none
 *  notice: If any possible, please hide pBA in ieee. And temporarily use Manage Queue as softmac_mgmt_xmit() usually does
 ********************************************************************************************************************/
static void ieee80211_send_ADDBARsp(struct ieee80211_device *ieee, u8 *dst,
<<<<<<< HEAD
				    PBA_RECORD pBA, u16 StatusCode)
=======
				    struct ba_record *pBA, u16 StatusCode)
>>>>>>> upstream/android-13
{
	struct sk_buff *skb;
	skb = ieee80211_ADDBA(ieee, dst, pBA, StatusCode, ACT_ADDBARSP); //construct ACT_ADDBARSP frames
	if (skb) {
		softmac_mgmt_xmit(skb, ieee);
		//same above
	} else {
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "alloc skb error in function %s()\n", __func__);
	}

	return;

}
/********************************************************************************************************************
 *function: send ADDBARSP frame out
 *   input:  u8*		dst	//DELBA frame's destination
<<<<<<< HEAD
 *	     PBA_RECORD		pBA	//BA_RECORD entry which stores the necessary information for BA
=======
 *	     struct ba_record  *pBA	//BA_RECORD entry which stores the necessary information for BA
>>>>>>> upstream/android-13
 *	     enum tr_select     TxRxSelect //TX or RX
 *	     u16		ReasonCode //DEL ReasonCode
 *  output:  none
 *  notice: If any possible, please hide pBA in ieee. And temporarily use Manage Queue as softmac_mgmt_xmit() usually does
 ********************************************************************************************************************/

static void ieee80211_send_DELBA(struct ieee80211_device *ieee, u8 *dst,
<<<<<<< HEAD
				 PBA_RECORD pBA, enum tr_select TxRxSelect,
=======
				 struct ba_record *pBA, enum tr_select TxRxSelect,
>>>>>>> upstream/android-13
				 u16 ReasonCode)
{
	struct sk_buff *skb;
	skb = ieee80211_DELBA(ieee, dst, pBA, TxRxSelect, ReasonCode); //construct ACT_ADDBARSP frames
	if (skb) {
		softmac_mgmt_xmit(skb, ieee);
		//same above
	} else {
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "alloc skb error in function %s()\n", __func__);
	}
}

/********************************************************************************************************************
 *function: RX ADDBAReq
 *   input:  struct sk_buff *   skb	//incoming ADDBAReq skb.
 *  return:  0(pass), other(fail)
 *  notice:  As this function need support of QOS, I comment some code out. And when qos is ready, this code need to be support.
 ********************************************************************************************************************/
int ieee80211_rx_ADDBAReq(struct ieee80211_device *ieee, struct sk_buff *skb)
{
<<<<<<< HEAD
	 struct rtl_80211_hdr_3addr *req = NULL;
	u16 rc = 0;
	u8 *dst = NULL, *pDialogToken = NULL, *tag = NULL;
	PBA_RECORD pBA = NULL;
	PBA_PARAM_SET	pBaParamSet = NULL;
	u16 *pBaTimeoutVal = NULL;
	PSEQUENCE_CONTROL pBaStartSeqCtrl = NULL;
=======
	struct rtl_80211_hdr_3addr *req = NULL;
	u16 rc = 0;
	u8 *dst = NULL, *pDialogToken = NULL, *tag = NULL;
	struct ba_record *pBA = NULL;
	union ba_param_set     *pBaParamSet = NULL;
	u16 *pBaTimeoutVal = NULL;
	union sequence_control *pBaStartSeqCtrl = NULL;
>>>>>>> upstream/android-13
	struct rx_ts_record  *pTS = NULL;

	if (skb->len < sizeof(struct rtl_80211_hdr_3addr) + 9) {
		IEEE80211_DEBUG(IEEE80211_DL_ERR,
				" Invalid skb len in BAREQ(%d / %zu)\n",
				skb->len,
				(sizeof(struct rtl_80211_hdr_3addr) + 9));
		return -1;
	}

<<<<<<< HEAD
	IEEE80211_DEBUG_DATA(IEEE80211_DL_DATA|IEEE80211_DL_BA, skb->data, skb->len);

	req = (struct rtl_80211_hdr_3addr *) skb->data;
=======
	IEEE80211_DEBUG_DATA(IEEE80211_DL_DATA | IEEE80211_DL_BA, skb->data, skb->len);

	req = (struct rtl_80211_hdr_3addr *)skb->data;
>>>>>>> upstream/android-13
	tag = (u8 *)req;
	dst = &req->addr2[0];
	tag += sizeof(struct rtl_80211_hdr_3addr);
	pDialogToken = tag + 2;  //category+action
<<<<<<< HEAD
	pBaParamSet = (PBA_PARAM_SET)(tag + 3);   //+DialogToken
	pBaTimeoutVal = (u16 *)(tag + 5);
	pBaStartSeqCtrl = (PSEQUENCE_CONTROL)(req + 7);
=======
	pBaParamSet = (union ba_param_set *)(tag + 3);   //+DialogToken
	pBaTimeoutVal = (u16 *)(tag + 5);
	pBaStartSeqCtrl = (union sequence_control *)(req + 7);
>>>>>>> upstream/android-13

	netdev_info(ieee->dev, "====================>rx ADDBAREQ from :%pM\n", dst);
//some other capability is not ready now.
	if ((ieee->current_network.qos_data.active == 0) ||
		(!ieee->pHTInfo->bCurrentHTSupport)) //||
	//	(!ieee->pStaQos->bEnableRxImmBA)	)
	{
		rc = ADDBA_STATUS_REFUSED;
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "Failed to reply on ADDBA_REQ as some capability is not ready(%d, %d)\n", ieee->current_network.qos_data.active, ieee->pHTInfo->bCurrentHTSupport);
		goto OnADDBAReq_Fail;
	}
	// Search for related traffic stream.
	// If there is no matched TS, reject the ADDBA request.
	if (!GetTs(
			ieee,
			(struct ts_common_info **)(&pTS),
			dst,
<<<<<<< HEAD
			(u8)(pBaParamSet->field.TID),
=======
			(u8)(pBaParamSet->field.tid),
>>>>>>> upstream/android-13
			RX_DIR,
			true)) {
		rc = ADDBA_STATUS_REFUSED;
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "can't get TS in %s()\n", __func__);
		goto OnADDBAReq_Fail;
	}
	pBA = &pTS->rx_admitted_ba_record;
	// To Determine the ADDBA Req content
<<<<<<< HEAD
	// We can do much more check here, including BufferSize, AMSDU_Support, Policy, StartSeqCtrl...
	// I want to check StartSeqCtrl to make sure when we start aggregation!!!
	//
	if (pBaParamSet->field.BAPolicy == BA_POLICY_DELAYED) {
=======
	// We can do much more check here, including buffer_size, AMSDU_Support, Policy, StartSeqCtrl...
	// I want to check StartSeqCtrl to make sure when we start aggregation!!!
	//
	if (pBaParamSet->field.ba_policy == BA_POLICY_DELAYED) {
>>>>>>> upstream/android-13
		rc = ADDBA_STATUS_INVALID_PARAM;
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "BA Policy is not correct in %s()\n", __func__);
		goto OnADDBAReq_Fail;
	}
		// Admit the ADDBA Request
	//
	DeActivateBAEntry(ieee, pBA);
<<<<<<< HEAD
	pBA->DialogToken = *pDialogToken;
	pBA->BaParamSet = *pBaParamSet;
	pBA->BaTimeoutValue = *pBaTimeoutVal;
	pBA->BaStartSeqCtrl = *pBaStartSeqCtrl;
	//for half N mode we only aggregate 1 frame
	if (ieee->GetHalfNmodeSupportByAPsHandler(ieee->dev))
	pBA->BaParamSet.field.BufferSize = 1;
	else
	pBA->BaParamSet.field.BufferSize = 32;
	ActivateBAEntry(ieee, pBA, pBA->BaTimeoutValue);
=======
	pBA->dialog_token = *pDialogToken;
	pBA->param_set = *pBaParamSet;
	pBA->timeout_value = *pBaTimeoutVal;
	pBA->start_seq_ctrl = *pBaStartSeqCtrl;
	//for half N mode we only aggregate 1 frame
	if (ieee->GetHalfNmodeSupportByAPsHandler(ieee->dev))
		pBA->param_set.field.buffer_size = 1;
	else
		pBA->param_set.field.buffer_size = 32;
	ActivateBAEntry(ieee, pBA, pBA->timeout_value);
>>>>>>> upstream/android-13
	ieee80211_send_ADDBARsp(ieee, dst, pBA, ADDBA_STATUS_SUCCESS);

	// End of procedure.
	return 0;

OnADDBAReq_Fail:
	{
<<<<<<< HEAD
		BA_RECORD	BA;
		BA.BaParamSet = *pBaParamSet;
		BA.BaTimeoutValue = *pBaTimeoutVal;
		BA.DialogToken = *pDialogToken;
		BA.BaParamSet.field.BAPolicy = BA_POLICY_IMMEDIATE;
=======
		struct ba_record	BA;
		BA.param_set = *pBaParamSet;
		BA.timeout_value = *pBaTimeoutVal;
		BA.dialog_token = *pDialogToken;
		BA.param_set.field.ba_policy = BA_POLICY_IMMEDIATE;
>>>>>>> upstream/android-13
		ieee80211_send_ADDBARsp(ieee, dst, &BA, rc);
		return 0; //we send RSP out.
	}

}

/********************************************************************************************************************
 *function: RX ADDBARSP
 *   input:  struct sk_buff *   skb	//incoming ADDBAReq skb.
 *  return:  0(pass), other(fail)
 *  notice:  As this function need support of QOS, I comment some code out. And when qos is ready, this code need to be support.
 ********************************************************************************************************************/
int ieee80211_rx_ADDBARsp(struct ieee80211_device *ieee, struct sk_buff *skb)
{
<<<<<<< HEAD
	 struct rtl_80211_hdr_3addr *rsp = NULL;
	PBA_RECORD		pPendingBA, pAdmittedBA;
	struct tx_ts_record     *pTS = NULL;
	u8 *dst = NULL, *pDialogToken = NULL, *tag = NULL;
	u16 *pStatusCode = NULL, *pBaTimeoutVal = NULL;
	PBA_PARAM_SET		pBaParamSet = NULL;
=======
	struct rtl_80211_hdr_3addr *rsp = NULL;
	struct ba_record        *pPendingBA, *pAdmittedBA;
	struct tx_ts_record     *pTS = NULL;
	u8 *dst = NULL, *pDialogToken = NULL, *tag = NULL;
	u16 *pStatusCode = NULL, *pBaTimeoutVal = NULL;
	union ba_param_set       *pBaParamSet = NULL;
>>>>>>> upstream/android-13
	u16			ReasonCode;

	if (skb->len < sizeof(struct rtl_80211_hdr_3addr) + 9) {
		IEEE80211_DEBUG(IEEE80211_DL_ERR,
				" Invalid skb len in BARSP(%d / %zu)\n",
				skb->len,
				(sizeof(struct rtl_80211_hdr_3addr) + 9));
		return -1;
	}
	rsp = (struct rtl_80211_hdr_3addr *)skb->data;
	tag = (u8 *)rsp;
	dst = &rsp->addr2[0];
	tag += sizeof(struct rtl_80211_hdr_3addr);
	pDialogToken = tag + 2;
	pStatusCode = (u16 *)(tag + 3);
<<<<<<< HEAD
	pBaParamSet = (PBA_PARAM_SET)(tag + 5);
=======
	pBaParamSet = (union ba_param_set *)(tag + 5);
>>>>>>> upstream/android-13
	pBaTimeoutVal = (u16 *)(tag + 7);

	// Check the capability
	// Since we can always receive A-MPDU, we just check if it is under HT mode.
	if (ieee->current_network.qos_data.active == 0  ||
	    !ieee->pHTInfo->bCurrentHTSupport ||
	    !ieee->pHTInfo->bCurrentAMPDUEnable) {
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "reject to ADDBA_RSP as some capability is not ready(%d, %d, %d)\n", ieee->current_network.qos_data.active, ieee->pHTInfo->bCurrentHTSupport, ieee->pHTInfo->bCurrentAMPDUEnable);
		ReasonCode = DELBA_REASON_UNKNOWN_BA;
		goto OnADDBARsp_Reject;
	}


	//
	// Search for related TS.
	// If there is no TS found, we wil reject ADDBA Rsp by sending DELBA frame.
	//
	if (!GetTs(
			ieee,
			(struct ts_common_info **)(&pTS),
			dst,
<<<<<<< HEAD
			(u8)(pBaParamSet->field.TID),
=======
			(u8)(pBaParamSet->field.tid),
>>>>>>> upstream/android-13
			TX_DIR,
			false)) {
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "can't get TS in %s()\n", __func__);
		ReasonCode = DELBA_REASON_UNKNOWN_BA;
		goto OnADDBARsp_Reject;
	}

	pTS->add_ba_req_in_progress = false;
	pPendingBA = &pTS->tx_pending_ba_record;
	pAdmittedBA = &pTS->tx_admitted_ba_record;


	//
	// Check if related BA is waiting for setup.
	// If not, reject by sending DELBA frame.
	//
<<<<<<< HEAD
	if (pAdmittedBA->bValid) {
		// Since BA is already setup, we ignore all other ADDBA Response.
		IEEE80211_DEBUG(IEEE80211_DL_BA, "OnADDBARsp(): Recv ADDBA Rsp. Drop because already admit it! \n");
		return -1;
	} else if ((!pPendingBA->bValid) || (*pDialogToken != pPendingBA->DialogToken)) {
=======
	if (pAdmittedBA->valid) {
		// Since BA is already setup, we ignore all other ADDBA Response.
		IEEE80211_DEBUG(IEEE80211_DL_BA, "OnADDBARsp(): Recv ADDBA Rsp. Drop because already admit it! \n");
		return -1;
	} else if ((!pPendingBA->valid) || (*pDialogToken != pPendingBA->dialog_token)) {
>>>>>>> upstream/android-13
		IEEE80211_DEBUG(IEEE80211_DL_ERR,  "OnADDBARsp(): Recv ADDBA Rsp. BA invalid, DELBA! \n");
		ReasonCode = DELBA_REASON_UNKNOWN_BA;
		goto OnADDBARsp_Reject;
	} else {
		IEEE80211_DEBUG(IEEE80211_DL_BA, "OnADDBARsp(): Recv ADDBA Rsp. BA is admitted! Status code:%X\n", *pStatusCode);
		DeActivateBAEntry(ieee, pPendingBA);
	}


	if (*pStatusCode == ADDBA_STATUS_SUCCESS) {
		//
		// Determine ADDBA Rsp content here.
		// We can compare the value of BA parameter set that Peer returned and Self sent.
		// If it is OK, then admitted. Or we can send DELBA to cancel BA mechanism.
		//
<<<<<<< HEAD
		if (pBaParamSet->field.BAPolicy == BA_POLICY_DELAYED) {
=======
		if (pBaParamSet->field.ba_policy == BA_POLICY_DELAYED) {
>>>>>>> upstream/android-13
			// Since this is a kind of ADDBA failed, we delay next ADDBA process.
			pTS->add_ba_req_delayed = true;
			DeActivateBAEntry(ieee, pAdmittedBA);
			ReasonCode = DELBA_REASON_END_BA;
			goto OnADDBARsp_Reject;
		}


		//
		// Admitted condition
		//
<<<<<<< HEAD
		pAdmittedBA->DialogToken = *pDialogToken;
		pAdmittedBA->BaTimeoutValue = *pBaTimeoutVal;
		pAdmittedBA->BaStartSeqCtrl = pPendingBA->BaStartSeqCtrl;
		pAdmittedBA->BaParamSet = *pBaParamSet;
=======
		pAdmittedBA->dialog_token = *pDialogToken;
		pAdmittedBA->timeout_value = *pBaTimeoutVal;
		pAdmittedBA->start_seq_ctrl = pPendingBA->start_seq_ctrl;
		pAdmittedBA->param_set = *pBaParamSet;
>>>>>>> upstream/android-13
		DeActivateBAEntry(ieee, pAdmittedBA);
		ActivateBAEntry(ieee, pAdmittedBA, *pBaTimeoutVal);
	} else {
		// Delay next ADDBA process.
		pTS->add_ba_req_delayed = true;
	}

	// End of procedure
	return 0;

OnADDBARsp_Reject:
	{
<<<<<<< HEAD
		BA_RECORD	BA;
		BA.BaParamSet = *pBaParamSet;
=======
		struct ba_record	BA;
		BA.param_set = *pBaParamSet;
>>>>>>> upstream/android-13
		ieee80211_send_DELBA(ieee, dst, &BA, TX_DIR, ReasonCode);
		return 0;
	}

}

/********************************************************************************************************************
 *function: RX DELBA
 *   input:  struct sk_buff *   skb	//incoming ADDBAReq skb.
 *  return:  0(pass), other(fail)
 *  notice:  As this function need support of QOS, I comment some code out. And when qos is ready, this code need to be support.
 ********************************************************************************************************************/
int ieee80211_rx_DELBA(struct ieee80211_device *ieee, struct sk_buff *skb)
{
<<<<<<< HEAD
	 struct rtl_80211_hdr_3addr *delba = NULL;
	PDELBA_PARAM_SET	pDelBaParamSet = NULL;
=======
	struct rtl_80211_hdr_3addr *delba = NULL;
	union delba_param_set   *pDelBaParamSet = NULL;
>>>>>>> upstream/android-13
	u8			*dst = NULL;

	if (skb->len < sizeof(struct rtl_80211_hdr_3addr) + 6) {
		IEEE80211_DEBUG(IEEE80211_DL_ERR,
				" Invalid skb len in DELBA(%d / %zu)\n",
				skb->len,
				(sizeof(struct rtl_80211_hdr_3addr) + 6));
		return -1;
	}

	if (ieee->current_network.qos_data.active == 0 ||
	    !ieee->pHTInfo->bCurrentHTSupport) {
		IEEE80211_DEBUG(IEEE80211_DL_ERR, "received DELBA while QOS or HT is not supported(%d, %d)\n", ieee->current_network.qos_data.active, ieee->pHTInfo->bCurrentHTSupport);
		return -1;
	}

<<<<<<< HEAD
	IEEE80211_DEBUG_DATA(IEEE80211_DL_DATA|IEEE80211_DL_BA, skb->data, skb->len);
	delba = (struct rtl_80211_hdr_3addr *)skb->data;
	dst = &delba->addr2[0];
	pDelBaParamSet = (PDELBA_PARAM_SET)&delba->payload[2];

	if (pDelBaParamSet->field.Initiator == 1) {
=======
	IEEE80211_DEBUG_DATA(IEEE80211_DL_DATA | IEEE80211_DL_BA, skb->data, skb->len);
	delba = (struct rtl_80211_hdr_3addr *)skb->data;
	dst = &delba->addr2[0];
	pDelBaParamSet = (union delba_param_set *)&delba->payload[2];

	if (pDelBaParamSet->field.initiator == 1) {
>>>>>>> upstream/android-13
		struct rx_ts_record *pRxTs;

		if (!GetTs(
				ieee,
				(struct ts_common_info **)&pRxTs,
				dst,
<<<<<<< HEAD
				(u8)pDelBaParamSet->field.TID,
=======
				(u8)pDelBaParamSet->field.tid,
>>>>>>> upstream/android-13
				RX_DIR,
				false)) {
			IEEE80211_DEBUG(IEEE80211_DL_ERR,  "can't get TS for RXTS in %s()\n", __func__);
			return -1;
		}

		RxTsDeleteBA(ieee, pRxTs);
	} else {
		struct tx_ts_record *pTxTs;

		if (!GetTs(
			ieee,
			(struct ts_common_info **)&pTxTs,
			dst,
<<<<<<< HEAD
			(u8)pDelBaParamSet->field.TID,
=======
			(u8)pDelBaParamSet->field.tid,
>>>>>>> upstream/android-13
			TX_DIR,
			false)) {
			IEEE80211_DEBUG(IEEE80211_DL_ERR,  "can't get TS for TXTS in %s()\n", __func__);
			return -1;
		}

		pTxTs->using_ba = false;
		pTxTs->add_ba_req_in_progress = false;
		pTxTs->add_ba_req_delayed = false;
		del_timer_sync(&pTxTs->ts_add_ba_timer);
		//PlatformCancelTimer(Adapter, &pTxTs->ts_add_ba_timer);
		TxTsDeleteBA(ieee, pTxTs);
	}
	return 0;
}

//
// ADDBA initiate. This can only be called by TX side.
//
void
TsInitAddBA(
	struct ieee80211_device *ieee,
	struct tx_ts_record     *pTS,
	u8		Policy,
	u8		bOverwritePending
	)
{
<<<<<<< HEAD
	PBA_RECORD			pBA = &pTS->tx_pending_ba_record;

	if (pBA->bValid && !bOverwritePending)
=======
	struct ba_record *pBA = &pTS->tx_pending_ba_record;

	if (pBA->valid && !bOverwritePending)
>>>>>>> upstream/android-13
		return;

	// Set parameters to "Pending" variable set
	DeActivateBAEntry(ieee, pBA);

<<<<<<< HEAD
	pBA->DialogToken++;						// DialogToken: Only keep the latest dialog token
	pBA->BaParamSet.field.AMSDU_Support = 0;	// Do not support A-MSDU with A-MPDU now!!
	pBA->BaParamSet.field.BAPolicy = Policy;	// Policy: Delayed or Immediate
	pBA->BaParamSet.field.TID = pTS->ts_common_info.t_spec.ts_info.uc_tsid;	// TID
	// BufferSize: This need to be set according to A-MPDU vector
	pBA->BaParamSet.field.BufferSize = 32;		// BufferSize: This need to be set according to A-MPDU vector
	pBA->BaTimeoutValue = 0;					// Timeout value: Set 0 to disable Timer
	pBA->BaStartSeqCtrl.field.SeqNum = (pTS->tx_cur_seq + 3) % 4096;	// Block Ack will start after 3 packets later.
=======
	pBA->dialog_token++;						// DialogToken: Only keep the latest dialog token
	pBA->param_set.field.amsdu_support = 0;	// Do not support A-MSDU with A-MPDU now!!
	pBA->param_set.field.ba_policy = Policy;	// Policy: Delayed or Immediate
	pBA->param_set.field.tid = pTS->ts_common_info.t_spec.ts_info.uc_tsid;	// TID
	// buffer_size: This need to be set according to A-MPDU vector
	pBA->param_set.field.buffer_size = 32;		// buffer_size: This need to be set according to A-MPDU vector
	pBA->timeout_value = 0;					// Timeout value: Set 0 to disable Timer
	pBA->start_seq_ctrl.field.seq_num = (pTS->tx_cur_seq + 3) % 4096;	// Block Ack will start after 3 packets later.
>>>>>>> upstream/android-13

	ActivateBAEntry(ieee, pBA, BA_SETUP_TIMEOUT);

	ieee80211_send_ADDBAReq(ieee, pTS->ts_common_info.addr, pBA);
}

void
TsInitDelBA(struct ieee80211_device *ieee, struct ts_common_info *pTsCommonInfo, enum tr_select TxRxSelect)
{
	if (TxRxSelect == TX_DIR) {
		struct tx_ts_record *pTxTs = (struct tx_ts_record *)pTsCommonInfo;

		if (TxTsDeleteBA(ieee, pTxTs))
			ieee80211_send_DELBA(
				ieee,
				pTsCommonInfo->addr,
<<<<<<< HEAD
				(pTxTs->tx_admitted_ba_record.bValid)?(&pTxTs->tx_admitted_ba_record):(&pTxTs->tx_pending_ba_record),
=======
				(pTxTs->tx_admitted_ba_record.valid) ? (&pTxTs->tx_admitted_ba_record) : (&pTxTs->tx_pending_ba_record),
>>>>>>> upstream/android-13
				TxRxSelect,
				DELBA_REASON_END_BA);
	} else if (TxRxSelect == RX_DIR) {
		struct rx_ts_record *pRxTs = (struct rx_ts_record *)pTsCommonInfo;
		if (RxTsDeleteBA(ieee, pRxTs))
			ieee80211_send_DELBA(
				ieee,
				pTsCommonInfo->addr,
				&pRxTs->rx_admitted_ba_record,
				TxRxSelect,
				DELBA_REASON_END_BA);
	}
}
/********************************************************************************************************************
 *function:  BA setup timer
 *   input:  unsigned long	 data		//acturally we send struct tx_ts_record or struct rx_ts_record to these timer
 *  return:  NULL
 *  notice:
 ********************************************************************************************************************/
void BaSetupTimeOut(struct timer_list *t)
{
<<<<<<< HEAD
	struct tx_ts_record *pTxTs = from_timer(pTxTs, t, tx_pending_ba_record.Timer);

	pTxTs->add_ba_req_in_progress = false;
	pTxTs->add_ba_req_delayed = true;
	pTxTs->tx_pending_ba_record.bValid = false;
=======
	struct tx_ts_record *pTxTs = from_timer(pTxTs, t, tx_pending_ba_record.timer);

	pTxTs->add_ba_req_in_progress = false;
	pTxTs->add_ba_req_delayed = true;
	pTxTs->tx_pending_ba_record.valid = false;
>>>>>>> upstream/android-13
}

void TxBaInactTimeout(struct timer_list *t)
{
<<<<<<< HEAD
	struct tx_ts_record *pTxTs = from_timer(pTxTs, t, tx_admitted_ba_record.Timer);
=======
	struct tx_ts_record *pTxTs = from_timer(pTxTs, t, tx_admitted_ba_record.timer);
>>>>>>> upstream/android-13
	struct ieee80211_device *ieee = container_of(pTxTs, struct ieee80211_device, TxTsRecord[pTxTs->num]);
	TxTsDeleteBA(ieee, pTxTs);
	ieee80211_send_DELBA(
		ieee,
		pTxTs->ts_common_info.addr,
		&pTxTs->tx_admitted_ba_record,
		TX_DIR,
		DELBA_REASON_TIMEOUT);
}

void RxBaInactTimeout(struct timer_list *t)
{
<<<<<<< HEAD
	struct rx_ts_record *pRxTs = from_timer(pRxTs, t, rx_admitted_ba_record.Timer);
=======
	struct rx_ts_record *pRxTs = from_timer(pRxTs, t, rx_admitted_ba_record.timer);
>>>>>>> upstream/android-13
	struct ieee80211_device *ieee = container_of(pRxTs, struct ieee80211_device, RxTsRecord[pRxTs->num]);

	RxTsDeleteBA(ieee, pRxTs);
	ieee80211_send_DELBA(
		ieee,
		pRxTs->ts_common_info.addr,
		&pRxTs->rx_admitted_ba_record,
		RX_DIR,
		DELBA_REASON_TIMEOUT);
}
