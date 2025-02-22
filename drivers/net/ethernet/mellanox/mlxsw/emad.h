/* SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0 */
/* Copyright (c) 2015-2018 Mellanox Technologies. All rights reserved */

#ifndef _MLXSW_EMAD_H
#define _MLXSW_EMAD_H

#define MLXSW_EMAD_MAX_FRAME_LEN 1518	/* Length in u8 */
#define MLXSW_EMAD_MAX_RETRY 5

/* EMAD Ethernet header */
#define MLXSW_EMAD_ETH_HDR_LEN 0x10	/* Length in u8 */
#define MLXSW_EMAD_EH_DMAC "\x01\x02\xc9\x00\x00\x01"
#define MLXSW_EMAD_EH_SMAC "\x00\x02\xc9\x01\x02\x03"
#define MLXSW_EMAD_EH_ETHERTYPE 0x8932
#define MLXSW_EMAD_EH_MLX_PROTO 0
#define MLXSW_EMAD_EH_PROTO_VERSION 0

/* EMAD TLV Types */
enum {
	MLXSW_EMAD_TLV_TYPE_END,
	MLXSW_EMAD_TLV_TYPE_OP,
<<<<<<< HEAD
	MLXSW_EMAD_TLV_TYPE_DR,
	MLXSW_EMAD_TLV_TYPE_REG,
	MLXSW_EMAD_TLV_TYPE_USERDATA,
	MLXSW_EMAD_TLV_TYPE_OOBETH,
=======
	MLXSW_EMAD_TLV_TYPE_STRING,
	MLXSW_EMAD_TLV_TYPE_REG,
>>>>>>> upstream/android-13
};

/* OP TLV */
#define MLXSW_EMAD_OP_TLV_LEN 4		/* Length in u32 */

enum {
	MLXSW_EMAD_OP_TLV_CLASS_REG_ACCESS = 1,
	MLXSW_EMAD_OP_TLV_CLASS_IPC = 2,
};

enum mlxsw_emad_op_tlv_status {
	MLXSW_EMAD_OP_TLV_STATUS_SUCCESS,
	MLXSW_EMAD_OP_TLV_STATUS_BUSY,
	MLXSW_EMAD_OP_TLV_STATUS_VERSION_NOT_SUPPORTED,
	MLXSW_EMAD_OP_TLV_STATUS_UNKNOWN_TLV,
	MLXSW_EMAD_OP_TLV_STATUS_REGISTER_NOT_SUPPORTED,
	MLXSW_EMAD_OP_TLV_STATUS_CLASS_NOT_SUPPORTED,
	MLXSW_EMAD_OP_TLV_STATUS_METHOD_NOT_SUPPORTED,
	MLXSW_EMAD_OP_TLV_STATUS_BAD_PARAMETER,
	MLXSW_EMAD_OP_TLV_STATUS_RESOURCE_NOT_AVAILABLE,
	MLXSW_EMAD_OP_TLV_STATUS_MESSAGE_RECEIPT_ACK,
	MLXSW_EMAD_OP_TLV_STATUS_INTERNAL_ERROR = 0x70,
};

static inline char *mlxsw_emad_op_tlv_status_str(u8 status)
{
	switch (status) {
	case MLXSW_EMAD_OP_TLV_STATUS_SUCCESS:
		return "operation performed";
	case MLXSW_EMAD_OP_TLV_STATUS_BUSY:
		return "device is busy";
	case MLXSW_EMAD_OP_TLV_STATUS_VERSION_NOT_SUPPORTED:
		return "version not supported";
	case MLXSW_EMAD_OP_TLV_STATUS_UNKNOWN_TLV:
		return "unknown TLV";
	case MLXSW_EMAD_OP_TLV_STATUS_REGISTER_NOT_SUPPORTED:
		return "register not supported";
	case MLXSW_EMAD_OP_TLV_STATUS_CLASS_NOT_SUPPORTED:
		return "class not supported";
	case MLXSW_EMAD_OP_TLV_STATUS_METHOD_NOT_SUPPORTED:
		return "method not supported";
	case MLXSW_EMAD_OP_TLV_STATUS_BAD_PARAMETER:
		return "bad parameter";
	case MLXSW_EMAD_OP_TLV_STATUS_RESOURCE_NOT_AVAILABLE:
		return "resource not available";
	case MLXSW_EMAD_OP_TLV_STATUS_MESSAGE_RECEIPT_ACK:
		return "acknowledged. retransmit";
	case MLXSW_EMAD_OP_TLV_STATUS_INTERNAL_ERROR:
		return "internal error";
	default:
		return "*UNKNOWN*";
	}
}

enum {
	MLXSW_EMAD_OP_TLV_REQUEST,
	MLXSW_EMAD_OP_TLV_RESPONSE
};

enum {
	MLXSW_EMAD_OP_TLV_METHOD_QUERY = 1,
	MLXSW_EMAD_OP_TLV_METHOD_WRITE = 2,
	MLXSW_EMAD_OP_TLV_METHOD_SEND = 3,
	MLXSW_EMAD_OP_TLV_METHOD_EVENT = 5,
};

<<<<<<< HEAD
=======
/* STRING TLV */
#define MLXSW_EMAD_STRING_TLV_LEN 33	/* Length in u32 */

>>>>>>> upstream/android-13
/* END TLV */
#define MLXSW_EMAD_END_TLV_LEN 1	/* Length in u32 */

#endif
