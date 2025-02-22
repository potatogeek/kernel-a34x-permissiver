/*
 * Copyright (c) 2016 Hisilicon Limited.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _HNS_ROCE_HW_V1_H
#define _HNS_ROCE_HW_V1_H

#define CQ_STATE_VALID					2

#define HNS_ROCE_V1_MAX_PD_NUM				0x8000
#define HNS_ROCE_V1_MAX_CQ_NUM				0x10000
#define HNS_ROCE_V1_MAX_CQE_NUM				0x8000

#define HNS_ROCE_V1_MAX_QP_NUM				0x40000
#define HNS_ROCE_V1_MAX_WQE_NUM				0x4000

#define HNS_ROCE_V1_MAX_MTPT_NUM			0x80000

#define HNS_ROCE_V1_MAX_MTT_SEGS			0x100000

#define HNS_ROCE_V1_MAX_QP_INIT_RDMA			128
#define HNS_ROCE_V1_MAX_QP_DEST_RDMA			128

#define HNS_ROCE_V1_MAX_SQ_DESC_SZ			64
#define HNS_ROCE_V1_MAX_RQ_DESC_SZ			64
#define HNS_ROCE_V1_SG_NUM				2
#define HNS_ROCE_V1_INLINE_SIZE				32

#define HNS_ROCE_V1_UAR_NUM				256
#define HNS_ROCE_V1_PHY_UAR_NUM				8

#define HNS_ROCE_V1_GID_NUM				16
#define HNS_ROCE_V1_RESV_QP				8

#define HNS_ROCE_V1_MAX_IRQ_NUM				34
#define HNS_ROCE_V1_COMP_VEC_NUM			32
#define HNS_ROCE_V1_AEQE_VEC_NUM			1
#define HNS_ROCE_V1_ABNORMAL_VEC_NUM			1

#define HNS_ROCE_V1_COMP_EQE_NUM			0x8000
#define HNS_ROCE_V1_ASYNC_EQE_NUM			0x400

<<<<<<< HEAD
#define HNS_ROCE_V1_QPC_ENTRY_SIZE			256
=======
#define HNS_ROCE_V1_QPC_SIZE				256
>>>>>>> upstream/android-13
#define HNS_ROCE_V1_IRRL_ENTRY_SIZE			8
#define HNS_ROCE_V1_CQC_ENTRY_SIZE			64
#define HNS_ROCE_V1_MTPT_ENTRY_SIZE			64
#define HNS_ROCE_V1_MTT_ENTRY_SIZE			64

<<<<<<< HEAD
#define HNS_ROCE_V1_CQE_ENTRY_SIZE			32
=======
#define HNS_ROCE_V1_CQE_SIZE				32
>>>>>>> upstream/android-13
#define HNS_ROCE_V1_PAGE_SIZE_SUPPORT			0xFFFFF000

#define HNS_ROCE_V1_TABLE_CHUNK_SIZE			(1 << 17)

#define HNS_ROCE_V1_EXT_RAQ_WF				8
#define HNS_ROCE_V1_RAQ_ENTRY				64
#define HNS_ROCE_V1_RAQ_DEPTH				32768
#define HNS_ROCE_V1_RAQ_SIZE	(HNS_ROCE_V1_RAQ_ENTRY * HNS_ROCE_V1_RAQ_DEPTH)

#define HNS_ROCE_V1_SDB_DEPTH				0x400
#define HNS_ROCE_V1_ODB_DEPTH				0x400

#define HNS_ROCE_V1_DB_RSVD				0x80

#define HNS_ROCE_V1_SDB_ALEPT				HNS_ROCE_V1_DB_RSVD
#define HNS_ROCE_V1_SDB_ALFUL	(HNS_ROCE_V1_SDB_DEPTH - HNS_ROCE_V1_DB_RSVD)
#define HNS_ROCE_V1_ODB_ALEPT				HNS_ROCE_V1_DB_RSVD
#define HNS_ROCE_V1_ODB_ALFUL	(HNS_ROCE_V1_ODB_DEPTH - HNS_ROCE_V1_DB_RSVD)

#define HNS_ROCE_V1_EXT_SDB_DEPTH			0x4000
#define HNS_ROCE_V1_EXT_ODB_DEPTH			0x4000
#define HNS_ROCE_V1_EXT_SDB_ENTRY			16
#define HNS_ROCE_V1_EXT_ODB_ENTRY			16
#define HNS_ROCE_V1_EXT_SDB_SIZE  \
	(HNS_ROCE_V1_EXT_SDB_DEPTH * HNS_ROCE_V1_EXT_SDB_ENTRY)
#define HNS_ROCE_V1_EXT_ODB_SIZE  \
	(HNS_ROCE_V1_EXT_ODB_DEPTH * HNS_ROCE_V1_EXT_ODB_ENTRY)

#define HNS_ROCE_V1_EXT_SDB_ALEPT			HNS_ROCE_V1_DB_RSVD
#define HNS_ROCE_V1_EXT_SDB_ALFUL  \
	(HNS_ROCE_V1_EXT_SDB_DEPTH - HNS_ROCE_V1_DB_RSVD)
#define HNS_ROCE_V1_EXT_ODB_ALEPT			HNS_ROCE_V1_DB_RSVD
#define HNS_ROCE_V1_EXT_ODB_ALFUL	\
	(HNS_ROCE_V1_EXT_ODB_DEPTH - HNS_ROCE_V1_DB_RSVD)

<<<<<<< HEAD
#define HNS_ROCE_V1_DB_WAIT_OK				0
#define HNS_ROCE_V1_DB_STAGE1				1
#define HNS_ROCE_V1_DB_STAGE2				2
#define HNS_ROCE_V1_CHECK_DB_TIMEOUT_MSECS		10000
#define HNS_ROCE_V1_CHECK_DB_SLEEP_MSECS		20
=======
>>>>>>> upstream/android-13
#define HNS_ROCE_V1_FREE_MR_TIMEOUT_MSECS		50000
#define HNS_ROCE_V1_RECREATE_LP_QP_TIMEOUT_MSECS	10000
#define HNS_ROCE_V1_FREE_MR_WAIT_VALUE			5
#define HNS_ROCE_V1_RECREATE_LP_QP_WAIT_VALUE		20

#define HNS_ROCE_BT_RSV_BUF_SIZE			(1 << 17)

#define HNS_ROCE_V1_TPTR_ENTRY_SIZE			2
#define HNS_ROCE_V1_TPTR_BUF_SIZE	\
	(HNS_ROCE_V1_TPTR_ENTRY_SIZE * HNS_ROCE_V1_MAX_CQ_NUM)

#define HNS_ROCE_ODB_POLL_MODE				0

#define HNS_ROCE_SDB_NORMAL_MODE			0
#define HNS_ROCE_SDB_EXTEND_MODE			1

#define HNS_ROCE_ODB_EXTEND_MODE			1

#define KEY_VALID					0x02

#define HNS_ROCE_CQE_QPN_MASK				0x3ffff
#define HNS_ROCE_CQE_STATUS_MASK			0x1f
#define HNS_ROCE_CQE_OPCODE_MASK			0xf

#define HNS_ROCE_CQE_SUCCESS				0x00
#define HNS_ROCE_CQE_SYNDROME_LOCAL_LENGTH_ERR		0x01
#define HNS_ROCE_CQE_SYNDROME_LOCAL_QP_OP_ERR		0x02
#define HNS_ROCE_CQE_SYNDROME_LOCAL_PROT_ERR		0x03
#define HNS_ROCE_CQE_SYNDROME_WR_FLUSH_ERR		0x04
#define HNS_ROCE_CQE_SYNDROME_MEM_MANAGE_OPERATE_ERR	0x05
#define HNS_ROCE_CQE_SYNDROME_BAD_RESP_ERR		0x06
#define HNS_ROCE_CQE_SYNDROME_LOCAL_ACCESS_ERR		0x07
#define HNS_ROCE_CQE_SYNDROME_REMOTE_INVAL_REQ_ERR	0x08
#define HNS_ROCE_CQE_SYNDROME_REMOTE_ACCESS_ERR		0x09
#define HNS_ROCE_CQE_SYNDROME_REMOTE_OP_ERR		0x0a
#define HNS_ROCE_CQE_SYNDROME_TRANSPORT_RETRY_EXC_ERR	0x0b
#define HNS_ROCE_CQE_SYNDROME_RNR_RETRY_EXC_ERR		0x0c

#define QP1C_CFGN_OFFSET				0x28
#define PHY_PORT_OFFSET					0x8
#define MTPT_IDX_SHIFT					16
#define ALL_PORT_VAL_OPEN				0x3f
#define POL_TIME_INTERVAL_VAL				0x80
#define SLEEP_TIME_INTERVAL				20
#define SQ_PSN_SHIFT					8
#define QKEY_VAL					0x80010000
#define SDB_INV_CNT_OFFSET				8
<<<<<<< HEAD
#define SDB_ST_CMP_VAL					8
=======
>>>>>>> upstream/android-13

#define HNS_ROCE_CEQ_DEFAULT_INTERVAL			0x10
#define HNS_ROCE_CEQ_DEFAULT_BURST_NUM			0x10

#define HNS_ROCE_INT_MASK_DISABLE			0
#define HNS_ROCE_INT_MASK_ENABLE			1

#define CEQ_REG_OFFSET					0x18

#define HNS_ROCE_CEQE_CEQE_COMP_OWNER_S	0

#define HNS_ROCE_V1_CONS_IDX_M GENMASK(15, 0)

#define HNS_ROCE_CEQE_CEQE_COMP_CQN_S 16
#define HNS_ROCE_CEQE_CEQE_COMP_CQN_M GENMASK(31, 16)

#define HNS_ROCE_AEQE_U32_4_EVENT_TYPE_S 16
#define HNS_ROCE_AEQE_U32_4_EVENT_TYPE_M GENMASK(23, 16)

#define HNS_ROCE_AEQE_U32_4_EVENT_SUB_TYPE_S 24
#define HNS_ROCE_AEQE_U32_4_EVENT_SUB_TYPE_M GENMASK(30, 24)

#define HNS_ROCE_AEQE_U32_4_OWNER_S 31

#define HNS_ROCE_AEQE_EVENT_QP_EVENT_QP_QPN_S 0
#define HNS_ROCE_AEQE_EVENT_QP_EVENT_QP_QPN_M GENMASK(23, 0)

#define HNS_ROCE_AEQE_EVENT_QP_EVENT_PORT_NUM_S 25
#define HNS_ROCE_AEQE_EVENT_QP_EVENT_PORT_NUM_M GENMASK(27, 25)

#define HNS_ROCE_AEQE_EVENT_CQ_EVENT_CQ_CQN_S 0
#define HNS_ROCE_AEQE_EVENT_CQ_EVENT_CQ_CQN_M GENMASK(15, 0)

#define HNS_ROCE_AEQE_EVENT_CE_EVENT_CEQE_CEQN_S 0
#define HNS_ROCE_AEQE_EVENT_CE_EVENT_CEQE_CEQN_M GENMASK(4, 0)

<<<<<<< HEAD
=======
/* Local Work Queue Catastrophic Error,SUBTYPE 0x5 */
enum {
	HNS_ROCE_LWQCE_QPC_ERROR = 1,
	HNS_ROCE_LWQCE_MTU_ERROR,
	HNS_ROCE_LWQCE_WQE_BA_ADDR_ERROR,
	HNS_ROCE_LWQCE_WQE_ADDR_ERROR,
	HNS_ROCE_LWQCE_SQ_WQE_SHIFT_ERROR,
	HNS_ROCE_LWQCE_SL_ERROR,
	HNS_ROCE_LWQCE_PORT_ERROR,
};

/* Local Access Violation Work Queue Error,SUBTYPE 0x7 */
enum {
	HNS_ROCE_LAVWQE_R_KEY_VIOLATION = 1,
	HNS_ROCE_LAVWQE_LENGTH_ERROR,
	HNS_ROCE_LAVWQE_VA_ERROR,
	HNS_ROCE_LAVWQE_PD_ERROR,
	HNS_ROCE_LAVWQE_RW_ACC_ERROR,
	HNS_ROCE_LAVWQE_KEY_STATE_ERROR,
	HNS_ROCE_LAVWQE_MR_OPERATION_ERROR,
};

/* DOORBELL overflow subtype */
enum {
	HNS_ROCE_DB_SUBTYPE_SDB_OVF = 1,
	HNS_ROCE_DB_SUBTYPE_SDB_ALM_OVF,
	HNS_ROCE_DB_SUBTYPE_ODB_OVF,
	HNS_ROCE_DB_SUBTYPE_ODB_ALM_OVF,
	HNS_ROCE_DB_SUBTYPE_SDB_ALM_EMP,
	HNS_ROCE_DB_SUBTYPE_ODB_ALM_EMP,
};

enum {
	/* RQ&SRQ related operations */
	HNS_ROCE_OPCODE_SEND_DATA_RECEIVE = 0x06,
	HNS_ROCE_OPCODE_RDMA_WITH_IMM_RECEIVE,
};

enum {
	HNS_ROCE_PORT_DOWN = 0,
	HNS_ROCE_PORT_UP,
};

>>>>>>> upstream/android-13
struct hns_roce_cq_context {
	__le32 cqc_byte_4;
	__le32 cq_bt_l;
	__le32 cqc_byte_12;
	__le32 cur_cqe_ba0_l;
	__le32 cqc_byte_20;
	__le32 cqe_tptr_addr_l;
	__le32 cur_cqe_ba1_l;
	__le32 cqc_byte_32;
};

#define CQ_CONTEXT_CQC_BYTE_4_CQC_STATE_S 0
#define CQ_CONTEXT_CQC_BYTE_4_CQC_STATE_M   \
	(((1UL << 2) - 1) << CQ_CONTEXT_CQC_BYTE_4_CQC_STATE_S)

#define CQ_CONTEXT_CQC_BYTE_4_CQN_S 16
#define CQ_CONTEXT_CQC_BYTE_4_CQN_M   \
	(((1UL << 16) - 1) << CQ_CONTEXT_CQC_BYTE_4_CQN_S)

#define CQ_CONTEXT_CQC_BYTE_12_CQ_BT_H_S 0
#define CQ_CONTEXT_CQC_BYTE_12_CQ_BT_H_M   \
	(((1UL << 17) - 1) << CQ_CONTEXT_CQC_BYTE_12_CQ_BT_H_S)

#define CQ_CONTEXT_CQC_BYTE_12_CQ_CQE_SHIFT_S 20
#define CQ_CONTEXT_CQC_BYTE_12_CQ_CQE_SHIFT_M   \
	(((1UL << 4) - 1) << CQ_CONTEXT_CQC_BYTE_12_CQ_CQE_SHIFT_S)

#define CQ_CONTEXT_CQC_BYTE_12_CEQN_S 24
#define CQ_CONTEXT_CQC_BYTE_12_CEQN_M   \
	(((1UL << 5) - 1) << CQ_CONTEXT_CQC_BYTE_12_CEQN_S)

#define CQ_CONTEXT_CQC_BYTE_20_CUR_CQE_BA0_H_S 0
#define CQ_CONTEXT_CQC_BYTE_20_CUR_CQE_BA0_H_M   \
	(((1UL << 5) - 1) << CQ_CONTEXT_CQC_BYTE_20_CUR_CQE_BA0_H_S)

#define CQ_CONTEXT_CQC_BYTE_20_CQ_CUR_INDEX_S 16
#define CQ_CONTEXT_CQC_BYTE_20_CQ_CUR_INDEX_M   \
	(((1UL << 16) - 1) << CQ_CONTEXT_CQC_BYTE_20_CQ_CUR_INDEX_S)

#define CQ_CONTEXT_CQC_BYTE_20_CQE_TPTR_ADDR_H_S 8
#define CQ_CONTEXT_CQC_BYTE_20_CQE_TPTR_ADDR_H_M   \
	(((1UL << 5) - 1) << CQ_CONTEXT_CQC_BYTE_20_CQE_TPTR_ADDR_H_S)

#define CQ_CONTEXT_CQC_BYTE_32_CUR_CQE_BA1_H_S 0
#define CQ_CONTEXT_CQC_BYTE_32_CUR_CQE_BA1_H_M   \
	(((1UL << 5) - 1) << CQ_CONTEXT_CQC_BYTE_32_CUR_CQE_BA1_H_S)

#define CQ_CONTEXT_CQC_BYTE_32_SE_FLAG_S 9

#define CQ_CONTEXT_CQC_BYTE_32_CE_FLAG_S 8
#define CQ_CONTEXT_CQC_BYTE_32_NOTIFICATION_FLAG_S 14
#define CQ_CQNTEXT_CQC_BYTE_32_TYPE_OF_COMPLETION_NOTIFICATION_S 15

#define CQ_CONTEXT_CQC_BYTE_32_CQ_CONS_IDX_S 16
#define CQ_CONTEXT_CQC_BYTE_32_CQ_CONS_IDX_M   \
	(((1UL << 16) - 1) << CQ_CONTEXT_CQC_BYTE_32_CQ_CONS_IDX_S)

struct hns_roce_cqe {
	__le32 cqe_byte_4;
	union {
		__le32 r_key;
		__le32 immediate_data;
	};
	__le32 byte_cnt;
	__le32 cqe_byte_16;
	__le32 cqe_byte_20;
	__le32 s_mac_l;
	__le32 cqe_byte_28;
	__le32 reserved;
};

#define CQE_BYTE_4_OWNER_S 7
#define CQE_BYTE_4_SQ_RQ_FLAG_S 14

#define CQE_BYTE_4_STATUS_OF_THE_OPERATION_S 8
#define CQE_BYTE_4_STATUS_OF_THE_OPERATION_M   \
	(((1UL << 5) - 1) << CQE_BYTE_4_STATUS_OF_THE_OPERATION_S)

#define CQE_BYTE_4_WQE_INDEX_S 16
#define CQE_BYTE_4_WQE_INDEX_M	(((1UL << 14) - 1) << CQE_BYTE_4_WQE_INDEX_S)

#define CQE_BYTE_4_OPERATION_TYPE_S 0
#define CQE_BYTE_4_OPERATION_TYPE_M   \
	(((1UL << 4) - 1) << CQE_BYTE_4_OPERATION_TYPE_S)

#define CQE_BYTE_4_IMM_INDICATOR_S 15

#define CQE_BYTE_16_LOCAL_QPN_S 0
#define CQE_BYTE_16_LOCAL_QPN_M	(((1UL << 24) - 1) << CQE_BYTE_16_LOCAL_QPN_S)

#define CQE_BYTE_20_PORT_NUM_S 26
#define CQE_BYTE_20_PORT_NUM_M	(((1UL << 3) - 1) << CQE_BYTE_20_PORT_NUM_S)

#define CQE_BYTE_20_SL_S 24
#define CQE_BYTE_20_SL_M	(((1UL << 2) - 1) << CQE_BYTE_20_SL_S)

#define CQE_BYTE_20_REMOTE_QPN_S 0
#define CQE_BYTE_20_REMOTE_QPN_M   \
	(((1UL << 24) - 1) << CQE_BYTE_20_REMOTE_QPN_S)

#define CQE_BYTE_20_GRH_PRESENT_S 29

#define CQE_BYTE_28_P_KEY_IDX_S 16
#define CQE_BYTE_28_P_KEY_IDX_M	(((1UL << 16) - 1) << CQE_BYTE_28_P_KEY_IDX_S)

#define CQ_DB_REQ_NOT_SOL	0
#define CQ_DB_REQ_NOT		(1 << 16)

struct hns_roce_v1_mpt_entry {
	__le32  mpt_byte_4;
	__le32  pbl_addr_l;
	__le32  mpt_byte_12;
	__le32  virt_addr_l;
	__le32  virt_addr_h;
	__le32  length;
	__le32  mpt_byte_28;
	__le32  pa0_l;
	__le32  mpt_byte_36;
	__le32  mpt_byte_40;
	__le32  mpt_byte_44;
	__le32  mpt_byte_48;
	__le32  pa4_l;
	__le32  mpt_byte_56;
	__le32  mpt_byte_60;
	__le32  mpt_byte_64;
};

#define MPT_BYTE_4_KEY_STATE_S 0
#define MPT_BYTE_4_KEY_STATE_M	(((1UL << 2) - 1) << MPT_BYTE_4_KEY_STATE_S)

#define MPT_BYTE_4_KEY_S 8
#define MPT_BYTE_4_KEY_M	(((1UL << 8) - 1) << MPT_BYTE_4_KEY_S)

#define MPT_BYTE_4_PAGE_SIZE_S 16
#define MPT_BYTE_4_PAGE_SIZE_M	(((1UL << 2) - 1) << MPT_BYTE_4_PAGE_SIZE_S)

#define MPT_BYTE_4_MW_TYPE_S 20

#define MPT_BYTE_4_MW_BIND_ENABLE_S 21

#define MPT_BYTE_4_OWN_S 22

#define MPT_BYTE_4_MEMORY_LOCATION_TYPE_S 24
#define MPT_BYTE_4_MEMORY_LOCATION_TYPE_M   \
	(((1UL << 2) - 1) << MPT_BYTE_4_MEMORY_LOCATION_TYPE_S)

#define MPT_BYTE_4_REMOTE_ATOMIC_S 26
#define MPT_BYTE_4_LOCAL_WRITE_S 27
#define MPT_BYTE_4_REMOTE_WRITE_S 28
#define MPT_BYTE_4_REMOTE_READ_S 29
#define MPT_BYTE_4_REMOTE_INVAL_ENABLE_S 30
#define MPT_BYTE_4_ADDRESS_TYPE_S 31

#define MPT_BYTE_12_PBL_ADDR_H_S 0
#define MPT_BYTE_12_PBL_ADDR_H_M   \
	(((1UL << 17) - 1) << MPT_BYTE_12_PBL_ADDR_H_S)

#define MPT_BYTE_12_MW_BIND_COUNTER_S 17
#define MPT_BYTE_12_MW_BIND_COUNTER_M   \
	(((1UL << 15) - 1) << MPT_BYTE_12_MW_BIND_COUNTER_S)

#define MPT_BYTE_28_PD_S 0
#define MPT_BYTE_28_PD_M	(((1UL << 16) - 1) << MPT_BYTE_28_PD_S)

#define MPT_BYTE_28_L_KEY_IDX_L_S 16
#define MPT_BYTE_28_L_KEY_IDX_L_M   \
	(((1UL << 16) - 1) << MPT_BYTE_28_L_KEY_IDX_L_S)

#define MPT_BYTE_36_PA0_H_S 0
#define MPT_BYTE_36_PA0_H_M	(((1UL << 5) - 1) << MPT_BYTE_36_PA0_H_S)

#define MPT_BYTE_36_PA1_L_S 8
#define MPT_BYTE_36_PA1_L_M	(((1UL << 24) - 1) << MPT_BYTE_36_PA1_L_S)

#define MPT_BYTE_40_PA1_H_S 0
#define MPT_BYTE_40_PA1_H_M	(((1UL << 13) - 1) << MPT_BYTE_40_PA1_H_S)

#define MPT_BYTE_40_PA2_L_S 16
#define MPT_BYTE_40_PA2_L_M	(((1UL << 16) - 1) << MPT_BYTE_40_PA2_L_S)

#define MPT_BYTE_44_PA2_H_S 0
#define MPT_BYTE_44_PA2_H_M	(((1UL << 21) - 1) << MPT_BYTE_44_PA2_H_S)

#define MPT_BYTE_44_PA3_L_S 24
#define MPT_BYTE_44_PA3_L_M	(((1UL << 8) - 1) << MPT_BYTE_44_PA3_L_S)

#define MPT_BYTE_48_PA3_H_S 0
#define MPT_BYTE_48_PA3_H_M	(((1UL << 29) - 1) << MPT_BYTE_48_PA3_H_S)

#define MPT_BYTE_56_PA4_H_S 0
#define MPT_BYTE_56_PA4_H_M	(((1UL << 5) - 1) << MPT_BYTE_56_PA4_H_S)

#define MPT_BYTE_56_PA5_L_S 8
#define MPT_BYTE_56_PA5_L_M	(((1UL << 24) - 1) << MPT_BYTE_56_PA5_L_S)

#define MPT_BYTE_60_PA5_H_S 0
#define MPT_BYTE_60_PA5_H_M	(((1UL << 13) - 1) << MPT_BYTE_60_PA5_H_S)

#define MPT_BYTE_60_PA6_L_S 16
#define MPT_BYTE_60_PA6_L_M	(((1UL << 16) - 1) << MPT_BYTE_60_PA6_L_S)

#define MPT_BYTE_64_PA6_H_S 0
#define MPT_BYTE_64_PA6_H_M	(((1UL << 21) - 1) << MPT_BYTE_64_PA6_H_S)

#define MPT_BYTE_64_L_KEY_IDX_H_S 24
#define MPT_BYTE_64_L_KEY_IDX_H_M   \
	(((1UL << 8) - 1) << MPT_BYTE_64_L_KEY_IDX_H_S)

struct hns_roce_wqe_ctrl_seg {
	__le32 sgl_pa_h;
	__le32 flag;
	union {
		__be32 imm_data;
		__le32 inv_key;
	};
	__le32 msg_length;
};

struct hns_roce_wqe_data_seg {
	__le64    addr;
	__le32    lkey;
	__le32    len;
};

struct hns_roce_wqe_raddr_seg {
	__le32 rkey;
<<<<<<< HEAD
	__le32 len;/* reserved */
=======
	__le32 len; /* reserved */
>>>>>>> upstream/android-13
	__le64 raddr;
};

struct hns_roce_rq_wqe_ctrl {
	__le32 rwqe_byte_4;
	__le32 rocee_sgl_ba_l;
	__le32 rwqe_byte_12;
	__le32 reserved[5];
};

#define RQ_WQE_CTRL_RWQE_BYTE_12_RWQE_SGE_NUM_S 16
#define RQ_WQE_CTRL_RWQE_BYTE_12_RWQE_SGE_NUM_M   \
	(((1UL << 6) - 1) << RQ_WQE_CTRL_RWQE_BYTE_12_RWQE_SGE_NUM_S)

#define HNS_ROCE_QP_DESTROY_TIMEOUT_MSECS	10000

#define GID_LEN					16

struct hns_roce_ud_send_wqe {
	__le32 dmac_h;
	__le32 u32_8;
	__le32 immediate_data;

	__le32 u32_16;
	union {
		unsigned char dgid[GID_LEN];
		struct {
			__le32 u32_20;
			__le32 u32_24;
			__le32 u32_28;
			__le32 u32_32;
		};
	};

	__le32 u32_36;
	__le32 u32_40;

	__le32 va0_l;
	__le32 va0_h;
	__le32 l_key0;

	__le32 va1_l;
	__le32 va1_h;
	__le32 l_key1;
};

#define UD_SEND_WQE_U32_4_DMAC_0_S 0
#define UD_SEND_WQE_U32_4_DMAC_0_M   \
	(((1UL << 8) - 1) << UD_SEND_WQE_U32_4_DMAC_0_S)

#define UD_SEND_WQE_U32_4_DMAC_1_S 8
#define UD_SEND_WQE_U32_4_DMAC_1_M   \
	(((1UL << 8) - 1) << UD_SEND_WQE_U32_4_DMAC_1_S)

#define UD_SEND_WQE_U32_4_DMAC_2_S 16
#define UD_SEND_WQE_U32_4_DMAC_2_M   \
	(((1UL << 8) - 1) << UD_SEND_WQE_U32_4_DMAC_2_S)

#define UD_SEND_WQE_U32_4_DMAC_3_S 24
#define UD_SEND_WQE_U32_4_DMAC_3_M   \
	(((1UL << 8) - 1) << UD_SEND_WQE_U32_4_DMAC_3_S)

#define UD_SEND_WQE_U32_8_DMAC_4_S 0
#define UD_SEND_WQE_U32_8_DMAC_4_M   \
	(((1UL << 8) - 1) << UD_SEND_WQE_U32_8_DMAC_4_S)

#define UD_SEND_WQE_U32_8_DMAC_5_S 8
#define UD_SEND_WQE_U32_8_DMAC_5_M   \
	(((1UL << 8) - 1) << UD_SEND_WQE_U32_8_DMAC_5_S)

#define UD_SEND_WQE_U32_8_LOOPBACK_INDICATOR_S 22

#define UD_SEND_WQE_U32_8_OPERATION_TYPE_S 16
#define UD_SEND_WQE_U32_8_OPERATION_TYPE_M   \
	(((1UL << 4) - 1) << UD_SEND_WQE_U32_8_OPERATION_TYPE_S)

#define UD_SEND_WQE_U32_8_NUMBER_OF_DATA_SEG_S 24
#define UD_SEND_WQE_U32_8_NUMBER_OF_DATA_SEG_M   \
	(((1UL << 6) - 1) << UD_SEND_WQE_U32_8_NUMBER_OF_DATA_SEG_S)

#define UD_SEND_WQE_U32_8_SEND_GL_ROUTING_HDR_FLAG_S 31

#define UD_SEND_WQE_U32_16_DEST_QP_S 0
#define UD_SEND_WQE_U32_16_DEST_QP_M   \
	(((1UL << 24) - 1) << UD_SEND_WQE_U32_16_DEST_QP_S)

#define UD_SEND_WQE_U32_16_MAX_STATIC_RATE_S 24
#define UD_SEND_WQE_U32_16_MAX_STATIC_RATE_M   \
	(((1UL << 8) - 1) << UD_SEND_WQE_U32_16_MAX_STATIC_RATE_S)

#define UD_SEND_WQE_U32_36_FLOW_LABEL_S 0
#define UD_SEND_WQE_U32_36_FLOW_LABEL_M   \
	(((1UL << 20) - 1) << UD_SEND_WQE_U32_36_FLOW_LABEL_S)

#define UD_SEND_WQE_U32_36_PRIORITY_S 20
#define UD_SEND_WQE_U32_36_PRIORITY_M   \
	(((1UL << 4) - 1) << UD_SEND_WQE_U32_36_PRIORITY_S)

#define UD_SEND_WQE_U32_36_SGID_INDEX_S 24
#define UD_SEND_WQE_U32_36_SGID_INDEX_M   \
	(((1UL << 8) - 1) << UD_SEND_WQE_U32_36_SGID_INDEX_S)

#define UD_SEND_WQE_U32_40_HOP_LIMIT_S 0
#define UD_SEND_WQE_U32_40_HOP_LIMIT_M   \
	(((1UL << 8) - 1) << UD_SEND_WQE_U32_40_HOP_LIMIT_S)

#define UD_SEND_WQE_U32_40_TRAFFIC_CLASS_S 8
#define UD_SEND_WQE_U32_40_TRAFFIC_CLASS_M   \
	(((1UL << 8) - 1) << UD_SEND_WQE_U32_40_TRAFFIC_CLASS_S)

struct hns_roce_sqp_context {
	__le32 qp1c_bytes_4;
	__le32 sq_rq_bt_l;
	__le32 qp1c_bytes_12;
	__le32 qp1c_bytes_16;
	__le32 qp1c_bytes_20;
	__le32 cur_rq_wqe_ba_l;
	__le32 qp1c_bytes_28;
	__le32 qp1c_bytes_32;
	__le32 cur_sq_wqe_ba_l;
	__le32 qp1c_bytes_40;
};

#define QP1C_BYTES_4_QP_STATE_S 0
#define QP1C_BYTES_4_QP_STATE_M   \
	(((1UL << 3) - 1) << QP1C_BYTES_4_QP_STATE_S)

#define QP1C_BYTES_4_SQ_WQE_SHIFT_S 8
#define QP1C_BYTES_4_SQ_WQE_SHIFT_M   \
	(((1UL << 4) - 1) << QP1C_BYTES_4_SQ_WQE_SHIFT_S)

#define QP1C_BYTES_4_RQ_WQE_SHIFT_S 12
#define QP1C_BYTES_4_RQ_WQE_SHIFT_M   \
	(((1UL << 4) - 1) << QP1C_BYTES_4_RQ_WQE_SHIFT_S)

#define QP1C_BYTES_4_PD_S 16
#define QP1C_BYTES_4_PD_M	(((1UL << 16) - 1) << QP1C_BYTES_4_PD_S)

#define QP1C_BYTES_12_SQ_RQ_BT_H_S 0
#define QP1C_BYTES_12_SQ_RQ_BT_H_M   \
	(((1UL << 17) - 1) << QP1C_BYTES_12_SQ_RQ_BT_H_S)

#define QP1C_BYTES_16_RQ_HEAD_S 0
#define QP1C_BYTES_16_RQ_HEAD_M	(((1UL << 15) - 1) << QP1C_BYTES_16_RQ_HEAD_S)

#define QP1C_BYTES_16_PORT_NUM_S 16
#define QP1C_BYTES_16_PORT_NUM_M   \
	(((1UL << 3) - 1) << QP1C_BYTES_16_PORT_NUM_S)

#define QP1C_BYTES_16_SIGNALING_TYPE_S 27
#define QP1C_BYTES_16_LOCAL_ENABLE_E2E_CREDIT_S 28
#define QP1C_BYTES_16_RQ_BA_FLG_S 29
#define QP1C_BYTES_16_SQ_BA_FLG_S 30
#define QP1C_BYTES_16_QP1_ERR_S 31

#define QP1C_BYTES_20_SQ_HEAD_S 0
#define QP1C_BYTES_20_SQ_HEAD_M	(((1UL << 15) - 1) << QP1C_BYTES_20_SQ_HEAD_S)

#define QP1C_BYTES_20_PKEY_IDX_S 16
#define QP1C_BYTES_20_PKEY_IDX_M   \
	(((1UL << 16) - 1) << QP1C_BYTES_20_PKEY_IDX_S)

#define QP1C_BYTES_28_CUR_RQ_WQE_BA_H_S 0
#define QP1C_BYTES_28_CUR_RQ_WQE_BA_H_M   \
	(((1UL << 5) - 1) << QP1C_BYTES_28_CUR_RQ_WQE_BA_H_S)

#define QP1C_BYTES_28_RQ_CUR_IDX_S 16
#define QP1C_BYTES_28_RQ_CUR_IDX_M   \
	(((1UL << 15) - 1) << QP1C_BYTES_28_RQ_CUR_IDX_S)

#define QP1C_BYTES_32_TX_CQ_NUM_S 0
#define QP1C_BYTES_32_TX_CQ_NUM_M   \
	(((1UL << 16) - 1) << QP1C_BYTES_32_TX_CQ_NUM_S)

#define QP1C_BYTES_32_RX_CQ_NUM_S 16
#define QP1C_BYTES_32_RX_CQ_NUM_M   \
	(((1UL << 16) - 1) << QP1C_BYTES_32_RX_CQ_NUM_S)

#define QP1C_BYTES_40_CUR_SQ_WQE_BA_H_S 0
#define QP1C_BYTES_40_CUR_SQ_WQE_BA_H_M   \
	(((1UL << 5) - 1) << QP1C_BYTES_40_CUR_SQ_WQE_BA_H_S)

#define QP1C_BYTES_40_SQ_CUR_IDX_S 16
#define QP1C_BYTES_40_SQ_CUR_IDX_M   \
	(((1UL << 15) - 1) << QP1C_BYTES_40_SQ_CUR_IDX_S)

#define HNS_ROCE_WQE_INLINE		(1UL<<31)
#define HNS_ROCE_WQE_SE			(1UL<<30)

#define HNS_ROCE_WQE_SGE_NUM_BIT	24
#define HNS_ROCE_WQE_IMM		(1UL<<23)
#define HNS_ROCE_WQE_FENCE		(1UL<<21)
#define HNS_ROCE_WQE_CQ_NOTIFY		(1UL<<20)

#define HNS_ROCE_WQE_OPCODE_SEND	(0<<16)
#define HNS_ROCE_WQE_OPCODE_RDMA_READ	(1<<16)
#define HNS_ROCE_WQE_OPCODE_RDMA_WRITE	(2<<16)
#define HNS_ROCE_WQE_OPCODE_LOCAL_INV	(4<<16)
#define HNS_ROCE_WQE_OPCODE_UD_SEND	(7<<16)
#define HNS_ROCE_WQE_OPCODE_MASK	(15<<16)

struct hns_roce_qp_context {
	__le32 qpc_bytes_4;
	__le32 qpc_bytes_8;
	__le32 qpc_bytes_12;
	__le32 qpc_bytes_16;
	__le32 sq_rq_bt_l;
	__le32 qpc_bytes_24;
	__le32 irrl_ba_l;
	__le32 qpc_bytes_32;
	__le32 qpc_bytes_36;
	__le32 dmac_l;
	__le32 qpc_bytes_44;
	__le32 qpc_bytes_48;
	u8     dgid[16];
	__le32 qpc_bytes_68;
	__le32 cur_rq_wqe_ba_l;
	__le32 qpc_bytes_76;
	__le32 rx_rnr_time;
	__le32 qpc_bytes_84;
	__le32 qpc_bytes_88;
	union {
		__le32 rx_sge_len;
		__le32 dma_length;
	};
	union {
		__le32 rx_sge_num;
		__le32 rx_send_pktn;
		__le32 r_key;
	};
	__le32 va_l;
	__le32 va_h;
	__le32 qpc_bytes_108;
	__le32 qpc_bytes_112;
	__le32 rx_cur_sq_wqe_ba_l;
	__le32 qpc_bytes_120;
	__le32 qpc_bytes_124;
	__le32 qpc_bytes_128;
	__le32 qpc_bytes_132;
	__le32 qpc_bytes_136;
	__le32 qpc_bytes_140;
	__le32 qpc_bytes_144;
	__le32 qpc_bytes_148;
	union {
		__le32 rnr_retry;
		__le32 ack_time;
	};
	__le32 qpc_bytes_156;
	__le32 pkt_use_len;
	__le32 qpc_bytes_164;
	__le32 qpc_bytes_168;
	union {
		__le32 sge_use_len;
		__le32 pa_use_len;
	};
	__le32 qpc_bytes_176;
	__le32 qpc_bytes_180;
	__le32 tx_cur_sq_wqe_ba_l;
	__le32 qpc_bytes_188;
	__le32 rvd21;
};

#define QP_CONTEXT_QPC_BYTES_4_TRANSPORT_SERVICE_TYPE_S 0
#define QP_CONTEXT_QPC_BYTES_4_TRANSPORT_SERVICE_TYPE_M   \
	(((1UL << 3) - 1) << QP_CONTEXT_QPC_BYTES_4_TRANSPORT_SERVICE_TYPE_S)

#define QP_CONTEXT_QPC_BYTE_4_ENABLE_FPMR_S 3
#define QP_CONTEXT_QPC_BYTE_4_RDMA_READ_ENABLE_S 4
#define QP_CONTEXT_QPC_BYTE_4_RDMA_WRITE_ENABLE_S 5
#define QP_CONTEXT_QPC_BYTE_4_ATOMIC_OPERATION_ENABLE_S 6
#define QP_CONTEXT_QPC_BYTE_4_RDMAR_USE_S 7

#define QP_CONTEXT_QPC_BYTES_4_SQ_WQE_SHIFT_S 8
#define QP_CONTEXT_QPC_BYTES_4_SQ_WQE_SHIFT_M   \
	(((1UL << 4) - 1) << QP_CONTEXT_QPC_BYTES_4_SQ_WQE_SHIFT_S)

#define QP_CONTEXT_QPC_BYTES_4_RQ_WQE_SHIFT_S 12
#define QP_CONTEXT_QPC_BYTES_4_RQ_WQE_SHIFT_M   \
	(((1UL << 4) - 1) << QP_CONTEXT_QPC_BYTES_4_RQ_WQE_SHIFT_S)

#define QP_CONTEXT_QPC_BYTES_4_PD_S 16
#define QP_CONTEXT_QPC_BYTES_4_PD_M   \
	(((1UL << 16) - 1) << QP_CONTEXT_QPC_BYTES_4_PD_S)

#define QP_CONTEXT_QPC_BYTES_8_TX_COMPLETION_S 0
#define QP_CONTEXT_QPC_BYTES_8_TX_COMPLETION_M   \
	(((1UL << 16) - 1) << QP_CONTEXT_QPC_BYTES_8_TX_COMPLETION_S)

#define QP_CONTEXT_QPC_BYTES_8_RX_COMPLETION_S 16
#define QP_CONTEXT_QPC_BYTES_8_RX_COMPLETION_M   \
	(((1UL << 16) - 1) << QP_CONTEXT_QPC_BYTES_8_RX_COMPLETION_S)

#define QP_CONTEXT_QPC_BYTES_12_SRQ_NUMBER_S 0
#define QP_CONTEXT_QPC_BYTES_12_SRQ_NUMBER_M   \
	(((1UL << 16) - 1) << QP_CONTEXT_QPC_BYTES_12_SRQ_NUMBER_S)

#define QP_CONTEXT_QPC_BYTES_12_P_KEY_INDEX_S 16
#define QP_CONTEXT_QPC_BYTES_12_P_KEY_INDEX_M   \
	(((1UL << 16) - 1) << QP_CONTEXT_QPC_BYTES_12_P_KEY_INDEX_S)

#define QP_CONTEXT_QPC_BYTES_16_QP_NUM_S 0
#define QP_CONTEXT_QPC_BYTES_16_QP_NUM_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_16_QP_NUM_S)

#define QP_CONTEXT_QPC_BYTES_24_SQ_RQ_BT_H_S 0
#define QP_CONTEXT_QPC_BYTES_24_SQ_RQ_BT_H_M   \
	(((1UL << 17) - 1) << QP_CONTEXT_QPC_BYTES_24_SQ_RQ_BT_H_S)

#define QP_CONTEXT_QPC_BYTES_24_MINIMUM_RNR_NAK_TIMER_S 18
#define QP_CONTEXT_QPC_BYTES_24_MINIMUM_RNR_NAK_TIMER_M   \
	(((1UL << 5) - 1) << QP_CONTEXT_QPC_BYTES_24_MINIMUM_RNR_NAK_TIMER_S)

#define QP_CONTEXT_QPC_BYTE_24_REMOTE_ENABLE_E2E_CREDITS_S 23

#define QP_CONTEXT_QPC_BYTES_32_IRRL_BA_H_S 0
#define QP_CONTEXT_QPC_BYTES_32_IRRL_BA_H_M   \
	(((1UL << 17) - 1) << QP_CONTEXT_QPC_BYTES_32_IRRL_BA_H_S)

#define QP_CONTEXT_QPC_BYTES_32_MIG_STATE_S 18
#define QP_CONTEXT_QPC_BYTES_32_MIG_STATE_M   \
	(((1UL << 2) - 1) << QP_CONTEXT_QPC_BYTES_32_MIG_STATE_S)

#define QP_CONTEXT_QPC_BYTE_32_LOCAL_ENABLE_E2E_CREDITS_S 20
#define QP_CONTEXT_QPC_BYTE_32_SIGNALING_TYPE_S 21
#define QP_CONTEXT_QPC_BYTE_32_LOOPBACK_INDICATOR_S 22
#define QP_CONTEXT_QPC_BYTE_32_GLOBAL_HEADER_S 23

#define QP_CONTEXT_QPC_BYTES_32_RESPONDER_RESOURCES_S 24
#define QP_CONTEXT_QPC_BYTES_32_RESPONDER_RESOURCES_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_32_RESPONDER_RESOURCES_S)

#define QP_CONTEXT_QPC_BYTES_36_DEST_QP_S 0
#define QP_CONTEXT_QPC_BYTES_36_DEST_QP_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_36_DEST_QP_S)

#define QP_CONTEXT_QPC_BYTES_36_SGID_INDEX_S 24
#define QP_CONTEXT_QPC_BYTES_36_SGID_INDEX_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_36_SGID_INDEX_S)

#define QP_CONTEXT_QPC_BYTES_44_DMAC_H_S 0
#define QP_CONTEXT_QPC_BYTES_44_DMAC_H_M   \
	(((1UL << 16) - 1) << QP_CONTEXT_QPC_BYTES_44_DMAC_H_S)

#define QP_CONTEXT_QPC_BYTES_44_MAXIMUM_STATIC_RATE_S 16
#define QP_CONTEXT_QPC_BYTES_44_MAXIMUM_STATIC_RATE_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_44_MAXIMUM_STATIC_RATE_S)

#define QP_CONTEXT_QPC_BYTES_44_HOPLMT_S 24
#define QP_CONTEXT_QPC_BYTES_44_HOPLMT_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_44_HOPLMT_S)

#define QP_CONTEXT_QPC_BYTES_48_FLOWLABEL_S 0
#define QP_CONTEXT_QPC_BYTES_48_FLOWLABEL_M   \
	(((1UL << 20) - 1) << QP_CONTEXT_QPC_BYTES_48_FLOWLABEL_S)

#define QP_CONTEXT_QPC_BYTES_48_TCLASS_S 20
#define QP_CONTEXT_QPC_BYTES_48_TCLASS_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_48_TCLASS_S)

#define QP_CONTEXT_QPC_BYTES_48_MTU_S 28
#define QP_CONTEXT_QPC_BYTES_48_MTU_M   \
	(((1UL << 4) - 1) << QP_CONTEXT_QPC_BYTES_48_MTU_S)

#define QP_CONTEXT_QPC_BYTES_68_RQ_HEAD_S 0
#define QP_CONTEXT_QPC_BYTES_68_RQ_HEAD_M   \
	(((1UL << 15) - 1) << QP_CONTEXT_QPC_BYTES_68_RQ_HEAD_S)

#define QP_CONTEXT_QPC_BYTES_68_RQ_CUR_INDEX_S 16
#define QP_CONTEXT_QPC_BYTES_68_RQ_CUR_INDEX_M   \
	(((1UL << 15) - 1) << QP_CONTEXT_QPC_BYTES_68_RQ_CUR_INDEX_S)

#define QP_CONTEXT_QPC_BYTES_76_CUR_RQ_WQE_BA_H_S 0
#define QP_CONTEXT_QPC_BYTES_76_CUR_RQ_WQE_BA_H_M   \
	(((1UL << 5) - 1) << QP_CONTEXT_QPC_BYTES_76_CUR_RQ_WQE_BA_H_S)

#define QP_CONTEXT_QPC_BYTES_76_RX_REQ_MSN_S 8
#define QP_CONTEXT_QPC_BYTES_76_RX_REQ_MSN_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_76_RX_REQ_MSN_S)

#define QP_CONTEXT_QPC_BYTES_84_LAST_ACK_PSN_S 0
#define QP_CONTEXT_QPC_BYTES_84_LAST_ACK_PSN_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_84_LAST_ACK_PSN_S)

#define QP_CONTEXT_QPC_BYTES_84_TRRL_HEAD_S 24
#define QP_CONTEXT_QPC_BYTES_84_TRRL_HEAD_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_84_TRRL_HEAD_S)

#define QP_CONTEXT_QPC_BYTES_88_RX_REQ_EPSN_S 0
#define QP_CONTEXT_QPC_BYTES_88_RX_REQ_EPSN_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_88_RX_REQ_EPSN_S)

#define QP_CONTEXT_QPC_BYTES_88_RX_REQ_PSN_ERR_FLAG_S 24
#define QP_CONTEXT_QPC_BYTES_88_RX_LAST_OPCODE_FLG_S 25

#define QP_CONTEXT_QPC_BYTES_88_RQ_REQ_LAST_OPERATION_TYPE_S 26
#define QP_CONTEXT_QPC_BYTES_88_RQ_REQ_LAST_OPERATION_TYPE_M   \
	(((1UL << 2) - 1) << \
	QP_CONTEXT_QPC_BYTES_88_RQ_REQ_LAST_OPERATION_TYPE_S)

#define QP_CONTEXT_QPC_BYTES_88_RQ_REQ_RDMA_WR_FLAG_S 29
#define QP_CONTEXT_QPC_BYTES_88_RQ_REQ_RDMA_WR_FLAG_M   \
	(((1UL << 2) - 1) << QP_CONTEXT_QPC_BYTES_88_RQ_REQ_RDMA_WR_FLAG_S)

#define QP_CONTEXT_QPC_BYTES_108_TRRL_SDB_PSN_S 0
#define QP_CONTEXT_QPC_BYTES_108_TRRL_SDB_PSN_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_108_TRRL_SDB_PSN_S)

#define QP_CONTEXT_QPC_BYTES_108_TRRL_SDB_PSN_FLG_S 24
#define QP_CONTEXT_QPC_BYTES_108_TRRL_TDB_PSN_FLG_S 25

#define QP_CONTEXT_QPC_BYTES_112_TRRL_TDB_PSN_S 0
#define QP_CONTEXT_QPC_BYTES_112_TRRL_TDB_PSN_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_112_TRRL_TDB_PSN_S)

#define QP_CONTEXT_QPC_BYTES_112_TRRL_TAIL_S 24
#define QP_CONTEXT_QPC_BYTES_112_TRRL_TAIL_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_112_TRRL_TAIL_S)

#define QP_CONTEXT_QPC_BYTES_120_RX_CUR_SQ_WQE_BA_H_S 0
#define QP_CONTEXT_QPC_BYTES_120_RX_CUR_SQ_WQE_BA_H_M   \
	(((1UL << 5) - 1) << QP_CONTEXT_QPC_BYTES_120_RX_CUR_SQ_WQE_BA_H_S)

#define QP_CONTEXT_QPC_BYTES_124_RX_ACK_MSN_S 0
#define QP_CONTEXT_QPC_BYTES_124_RX_ACK_MSN_M   \
	(((1UL << 15) - 1) << QP_CONTEXT_QPC_BYTES_124_RX_ACK_MSN_S)

#define QP_CONTEXT_QPC_BYTES_124_IRRL_MSG_IDX_S 16
#define QP_CONTEXT_QPC_BYTES_124_IRRL_MSG_IDX_M   \
	(((1UL << 15) - 1) << QP_CONTEXT_QPC_BYTES_124_IRRL_MSG_IDX_S)

#define QP_CONTEXT_QPC_BYTES_128_RX_ACK_EPSN_S 0
#define QP_CONTEXT_QPC_BYTES_128_RX_ACK_EPSN_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_128_RX_ACK_EPSN_S)

#define QP_CONTEXT_QPC_BYTES_128_RX_ACK_PSN_ERR_FLG_S 24

#define QP_CONTEXT_QPC_BYTES_128_ACK_LAST_OPERATION_TYPE_S 25
#define QP_CONTEXT_QPC_BYTES_128_ACK_LAST_OPERATION_TYPE_M   \
	(((1UL << 2) - 1) << QP_CONTEXT_QPC_BYTES_128_ACK_LAST_OPERATION_TYPE_S)

#define QP_CONTEXT_QPC_BYTES_128_IRRL_PSN_VLD_FLG_S 27

#define QP_CONTEXT_QPC_BYTES_132_IRRL_PSN_S 0
#define QP_CONTEXT_QPC_BYTES_132_IRRL_PSN_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_132_IRRL_PSN_S)

#define QP_CONTEXT_QPC_BYTES_132_IRRL_TAIL_S 24
#define QP_CONTEXT_QPC_BYTES_132_IRRL_TAIL_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_132_IRRL_TAIL_S)

#define QP_CONTEXT_QPC_BYTES_136_RETRY_MSG_PSN_S 0
#define QP_CONTEXT_QPC_BYTES_136_RETRY_MSG_PSN_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_136_RETRY_MSG_PSN_S)

#define QP_CONTEXT_QPC_BYTES_136_RETRY_MSG_FPKT_PSN_L_S 24
#define QP_CONTEXT_QPC_BYTES_136_RETRY_MSG_FPKT_PSN_L_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_136_RETRY_MSG_FPKT_PSN_L_S)

#define QP_CONTEXT_QPC_BYTES_140_RETRY_MSG_FPKT_PSN_H_S 0
#define QP_CONTEXT_QPC_BYTES_140_RETRY_MSG_FPKT_PSN_H_M   \
	(((1UL << 16) - 1) << QP_CONTEXT_QPC_BYTES_140_RETRY_MSG_FPKT_PSN_H_S)

#define QP_CONTEXT_QPC_BYTES_140_RETRY_MSG_MSN_S 16
#define QP_CONTEXT_QPC_BYTES_140_RETRY_MSG_MSN_M   \
	(((1UL << 15) - 1) << QP_CONTEXT_QPC_BYTES_140_RETRY_MSG_MSN_S)

#define QP_CONTEXT_QPC_BYTES_140_RNR_RETRY_FLG_S 31

#define QP_CONTEXT_QPC_BYTES_144_QP_STATE_S 0
#define QP_CONTEXT_QPC_BYTES_144_QP_STATE_M   \
	(((1UL << 3) - 1) << QP_CONTEXT_QPC_BYTES_144_QP_STATE_S)

#define QP_CONTEXT_QPC_BYTES_148_CHECK_FLAG_S 0
#define QP_CONTEXT_QPC_BYTES_148_CHECK_FLAG_M   \
	(((1UL << 2) - 1) << QP_CONTEXT_QPC_BYTES_148_CHECK_FLAG_S)

#define QP_CONTEXT_QPC_BYTES_148_RETRY_COUNT_S 2
#define QP_CONTEXT_QPC_BYTES_148_RETRY_COUNT_M   \
	(((1UL << 3) - 1) << QP_CONTEXT_QPC_BYTES_148_RETRY_COUNT_S)

#define QP_CONTEXT_QPC_BYTES_148_RNR_RETRY_COUNT_S 5
#define QP_CONTEXT_QPC_BYTES_148_RNR_RETRY_COUNT_M   \
	(((1UL << 3) - 1) << QP_CONTEXT_QPC_BYTES_148_RNR_RETRY_COUNT_S)

#define QP_CONTEXT_QPC_BYTES_148_LSN_S 8
#define QP_CONTEXT_QPC_BYTES_148_LSN_M   \
	(((1UL << 16) - 1) << QP_CONTEXT_QPC_BYTES_148_LSN_S)

#define QP_CONTEXT_QPC_BYTES_156_RETRY_COUNT_INIT_S 0
#define QP_CONTEXT_QPC_BYTES_156_RETRY_COUNT_INIT_M   \
	(((1UL << 3) - 1) << QP_CONTEXT_QPC_BYTES_156_RETRY_COUNT_INIT_S)

#define QP_CONTEXT_QPC_BYTES_156_ACK_TIMEOUT_S 3
#define QP_CONTEXT_QPC_BYTES_156_ACK_TIMEOUT_M   \
	(((1UL << 5) - 1) << QP_CONTEXT_QPC_BYTES_156_ACK_TIMEOUT_S)

#define QP_CONTEXT_QPC_BYTES_156_RNR_RETRY_COUNT_INIT_S 8
#define QP_CONTEXT_QPC_BYTES_156_RNR_RETRY_COUNT_INIT_M   \
	(((1UL << 3) - 1) << QP_CONTEXT_QPC_BYTES_156_RNR_RETRY_COUNT_INIT_S)

#define QP_CONTEXT_QPC_BYTES_156_PORT_NUM_S 11
#define QP_CONTEXT_QPC_BYTES_156_PORT_NUM_M   \
	(((1UL << 3) - 1) << QP_CONTEXT_QPC_BYTES_156_PORT_NUM_S)

#define QP_CONTEXT_QPC_BYTES_156_SL_S 14
#define QP_CONTEXT_QPC_BYTES_156_SL_M   \
	(((1UL << 2) - 1) << QP_CONTEXT_QPC_BYTES_156_SL_S)

#define QP_CONTEXT_QPC_BYTES_156_INITIATOR_DEPTH_S 16
#define QP_CONTEXT_QPC_BYTES_156_INITIATOR_DEPTH_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_156_INITIATOR_DEPTH_S)

#define QP_CONTEXT_QPC_BYTES_156_ACK_REQ_IND_S 24
#define QP_CONTEXT_QPC_BYTES_156_ACK_REQ_IND_M   \
	(((1UL << 2) - 1) << QP_CONTEXT_QPC_BYTES_156_ACK_REQ_IND_S)

#define QP_CONTEXT_QPC_BYTES_164_SQ_PSN_S 0
#define QP_CONTEXT_QPC_BYTES_164_SQ_PSN_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_164_SQ_PSN_S)

#define QP_CONTEXT_QPC_BYTES_164_IRRL_HEAD_S 24
#define QP_CONTEXT_QPC_BYTES_164_IRRL_HEAD_M   \
	(((1UL << 8) - 1) << QP_CONTEXT_QPC_BYTES_164_IRRL_HEAD_S)

#define QP_CONTEXT_QPC_BYTES_168_RETRY_SQ_PSN_S 0
#define QP_CONTEXT_QPC_BYTES_168_RETRY_SQ_PSN_M   \
	(((1UL << 24) - 1) << QP_CONTEXT_QPC_BYTES_168_RETRY_SQ_PSN_S)

#define QP_CONTEXT_QPC_BYTES_168_SGE_USE_FLA_S 24
#define QP_CONTEXT_QPC_BYTES_168_SGE_USE_FLA_M   \
	(((1UL << 2) - 1) << QP_CONTEXT_QPC_BYTES_168_SGE_USE_FLA_S)

#define QP_CONTEXT_QPC_BYTES_168_DB_TYPE_S 26
#define QP_CONTEXT_QPC_BYTES_168_DB_TYPE_M   \
	(((1UL << 2) - 1) << QP_CONTEXT_QPC_BYTES_168_DB_TYPE_S)

#define QP_CONTEXT_QPC_BYTES_168_MSG_LP_IND_S 28
#define QP_CONTEXT_QPC_BYTES_168_CSDB_LP_IND_S 29
#define QP_CONTEXT_QPC_BYTES_168_QP_ERR_FLG_S 30

#define QP_CONTEXT_QPC_BYTES_176_DB_CUR_INDEX_S 0
#define QP_CONTEXT_QPC_BYTES_176_DB_CUR_INDEX_M   \
	(((1UL << 15) - 1) << QP_CONTEXT_QPC_BYTES_176_DB_CUR_INDEX_S)

#define QP_CONTEXT_QPC_BYTES_176_RETRY_DB_CUR_INDEX_S 16
#define QP_CONTEXT_QPC_BYTES_176_RETRY_DB_CUR_INDEX_M   \
	(((1UL << 15) - 1) << QP_CONTEXT_QPC_BYTES_176_RETRY_DB_CUR_INDEX_S)

#define QP_CONTEXT_QPC_BYTES_180_SQ_HEAD_S 0
#define QP_CONTEXT_QPC_BYTES_180_SQ_HEAD_M   \
	(((1UL << 15) - 1) << QP_CONTEXT_QPC_BYTES_180_SQ_HEAD_S)

#define QP_CONTEXT_QPC_BYTES_180_SQ_CUR_INDEX_S 16
#define QP_CONTEXT_QPC_BYTES_180_SQ_CUR_INDEX_M   \
	(((1UL << 15) - 1) << QP_CONTEXT_QPC_BYTES_180_SQ_CUR_INDEX_S)

#define QP_CONTEXT_QPC_BYTES_188_TX_CUR_SQ_WQE_BA_H_S 0
#define QP_CONTEXT_QPC_BYTES_188_TX_CUR_SQ_WQE_BA_H_M   \
	(((1UL << 5) - 1) << QP_CONTEXT_QPC_BYTES_188_TX_CUR_SQ_WQE_BA_H_S)

#define QP_CONTEXT_QPC_BYTES_188_PKT_RETRY_FLG_S 8

#define QP_CONTEXT_QPC_BYTES_188_TX_RETRY_CUR_INDEX_S 16
#define QP_CONTEXT_QPC_BYTES_188_TX_RETRY_CUR_INDEX_M   \
	(((1UL << 15) - 1) << QP_CONTEXT_QPC_BYTES_188_TX_RETRY_CUR_INDEX_S)

#define STATUS_MASK		0xff
#define GO_BIT_TIMEOUT_MSECS	10000
#define HCR_STATUS_OFFSET	0x18
#define HCR_GO_BIT		15

struct hns_roce_rq_db {
	__le32    u32_4;
	__le32    u32_8;
};

#define RQ_DOORBELL_U32_4_RQ_HEAD_S 0
#define RQ_DOORBELL_U32_4_RQ_HEAD_M   \
	(((1UL << 15) - 1) << RQ_DOORBELL_U32_4_RQ_HEAD_S)

#define RQ_DOORBELL_U32_8_QPN_S 0
#define RQ_DOORBELL_U32_8_QPN_M   (((1UL << 24) - 1) << RQ_DOORBELL_U32_8_QPN_S)

#define RQ_DOORBELL_U32_8_CMD_S 28
#define RQ_DOORBELL_U32_8_CMD_M   (((1UL << 3) - 1) << RQ_DOORBELL_U32_8_CMD_S)

#define RQ_DOORBELL_U32_8_HW_SYNC_S 31

struct hns_roce_sq_db {
	__le32    u32_4;
	__le32    u32_8;
};

#define SQ_DOORBELL_U32_4_SQ_HEAD_S 0
#define SQ_DOORBELL_U32_4_SQ_HEAD_M   \
	(((1UL << 15) - 1) << SQ_DOORBELL_U32_4_SQ_HEAD_S)

#define SQ_DOORBELL_U32_4_SL_S 16
#define SQ_DOORBELL_U32_4_SL_M   \
	(((1UL << 2) - 1) << SQ_DOORBELL_U32_4_SL_S)

#define SQ_DOORBELL_U32_4_PORT_S 18
#define SQ_DOORBELL_U32_4_PORT_M  (((1UL << 3) - 1) << SQ_DOORBELL_U32_4_PORT_S)

#define SQ_DOORBELL_U32_8_QPN_S 0
#define SQ_DOORBELL_U32_8_QPN_M   (((1UL << 24) - 1) << SQ_DOORBELL_U32_8_QPN_S)

#define SQ_DOORBELL_HW_SYNC_S 31

struct hns_roce_ext_db {
	int esdb_dep;
	int eodb_dep;
	struct hns_roce_buf_list *sdb_buf_list;
	struct hns_roce_buf_list *odb_buf_list;
};

struct hns_roce_db_table {
	int  sdb_ext_mod;
	int  odb_ext_mod;
	struct hns_roce_ext_db *ext_db;
};

<<<<<<< HEAD
=======
#define HW_SYNC_SLEEP_TIME_INTERVAL 20
#define HW_SYNC_TIMEOUT_MSECS (25 * HW_SYNC_SLEEP_TIME_INTERVAL)
#define BT_CMD_SYNC_SHIFT 31
#define HNS_ROCE_BA_SIZE (32 * 4096)

>>>>>>> upstream/android-13
struct hns_roce_bt_table {
	struct hns_roce_buf_list qpc_buf;
	struct hns_roce_buf_list mtpt_buf;
	struct hns_roce_buf_list cqc_buf;
};

struct hns_roce_tptr_table {
	struct hns_roce_buf_list tptr_buf;
};

struct hns_roce_qp_work {
	struct	work_struct work;
	struct	ib_device *ib_dev;
	struct	hns_roce_qp *qp;
	u32	db_wait_stage;
	u32	sdb_issue_ptr;
	u32	sdb_inv_cnt;
	u32	sche_cnt;
};

<<<<<<< HEAD
struct hns_roce_des_qp {
	struct workqueue_struct	*qp_wq;
	int	requeue_flag;
};

=======
>>>>>>> upstream/android-13
struct hns_roce_mr_free_work {
	struct	work_struct work;
	struct	ib_device *ib_dev;
	struct	completion *comp;
	int	comp_flag;
	void	*mr;
};

struct hns_roce_recreate_lp_qp_work {
	struct	work_struct work;
	struct	ib_device *ib_dev;
	struct	completion *comp;
	int	comp_flag;
};

struct hns_roce_free_mr {
	struct workqueue_struct *free_mr_wq;
	struct hns_roce_qp *mr_free_qp[HNS_ROCE_V1_RESV_QP];
	struct hns_roce_cq *mr_free_cq;
	struct hns_roce_pd *mr_free_pd;
};

struct hns_roce_v1_priv {
	struct hns_roce_db_table  db_table;
	struct hns_roce_raq_table raq_table;
	struct hns_roce_bt_table  bt_table;
	struct hns_roce_tptr_table tptr_table;
<<<<<<< HEAD
	struct hns_roce_des_qp des_qp;
=======
>>>>>>> upstream/android-13
	struct hns_roce_free_mr free_mr;
};

int hns_dsaf_roce_reset(struct fwnode_handle *dsaf_fwnode, bool dereset);
int hns_roce_v1_poll_cq(struct ib_cq *ibcq, int num_entries, struct ib_wc *wc);
<<<<<<< HEAD
int hns_roce_v1_destroy_qp(struct ib_qp *ibqp);
=======
int hns_roce_v1_destroy_qp(struct ib_qp *ibqp, struct ib_udata *udata);
>>>>>>> upstream/android-13

#endif
