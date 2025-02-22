/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _BATYPE_H_
#define _BATYPE_H_

<<<<<<< HEAD
#define		TOTAL_TXBA_NUM	16
#define	TOTAL_RXBA_NUM	16

#define	BA_SETUP_TIMEOUT	200
#define	BA_INACT_TIMEOUT	60000

#define	BA_POLICY_DELAYED		0
#define	BA_POLICY_IMMEDIATE	1

#define	ADDBA_STATUS_SUCCESS			0
#define	ADDBA_STATUS_REFUSED		37
#define	ADDBA_STATUS_INVALID_PARAM	38

#define	DELBA_REASON_QSTA_LEAVING	36
#define	DELBA_REASON_END_BA			37
#define	DELBA_REASON_UNKNOWN_BA	38
#define	DELBA_REASON_TIMEOUT			39
/*  whether need define BA Action frames here?
struct ieee80211_ADDBA_Req{
	struct ieee80211_header_data header;
	u8	category;
	u8
} __attribute__ ((packed));
*/
//Is this need?I put here just to make it easier to define structure BA_RECORD //WB
typedef union _SEQUENCE_CONTROL{
	u16 ShortData;
	struct {
		u16	FragNum:4;
		u16	SeqNum:12;
	} field;
} SEQUENCE_CONTROL, *PSEQUENCE_CONTROL;

typedef union _BA_PARAM_SET {
	u8 charData[2];
	u16 shortData;
	struct {
		u16 AMSDU_Support:1;
		u16 BAPolicy:1;
		u16 TID:4;
		u16 BufferSize:10;
	} field;
} BA_PARAM_SET, *PBA_PARAM_SET;

typedef union _DELBA_PARAM_SET {
	u8 charData[2];
	u16 shortData;
	struct {
		u16 Reserved:11;
		u16 Initiator:1;
		u16 TID:4;
	} field;
} DELBA_PARAM_SET, *PDELBA_PARAM_SET;

typedef struct _BA_RECORD {
	struct timer_list		Timer;
	u8				bValid;
	u8				DialogToken;
	BA_PARAM_SET		BaParamSet;
	u16				BaTimeoutValue;
	SEQUENCE_CONTROL	BaStartSeqCtrl;
} BA_RECORD, *PBA_RECORD;
=======
#define	BA_SETUP_TIMEOUT	       200

#define	BA_POLICY_DELAYED		 0
#define	BA_POLICY_IMMEDIATE	         1

#define	ADDBA_STATUS_SUCCESS		 0
#define	ADDBA_STATUS_REFUSED		37
#define	ADDBA_STATUS_INVALID_PARAM	38

#define	DELBA_REASON_END_BA		37
#define	DELBA_REASON_UNKNOWN_BA	        38
#define	DELBA_REASON_TIMEOUT		39

union sequence_control {
	u16 short_data;
	struct {
		u16	frag_num:4;
		u16	seq_num:12;
	} field;
};

union ba_param_set {
	u16 short_data;
	struct {
		u16	amsdu_support:1;
		u16	ba_policy:1;
		u16	tid:4;
		u16	buffer_size:10;
	} field;
};

union delba_param_set {
	u16 short_data;
	struct {
		u16	reserved:11;
		u16	initiator:1;
		u16	tid:4;
	} field;
};

struct ba_record {
	struct timer_list	timer;
	u8			valid;
	u8			dialog_token;
	union ba_param_set	param_set;
	u16			timeout_value;
	union sequence_control	start_seq_ctrl;
};
>>>>>>> upstream/android-13

#endif //end _BATYPE_H_
