<<<<<<< HEAD
/*
 * Copyright (c) 2016 Avago Technologies.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED, EXCEPT TO
 * THE EXTENT THAT SUCH DISCLAIMERS ARE HELD TO BE LEGALLY INVALID.
 * See the GNU General Public License for more details, a copy of which
 * can be found in the file COPYING included with this package
 *
 */

/*
 * This file contains definitions relative to FC-NVME r1.14 (16-020vB).
 * The fcnvme_lsdesc_cr_assoc_cmd struct reflects expected r1.16 content.
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2016 Avago Technologies.  All rights reserved.
 */

/*
 * This file contains definitions relative to FC-NVME-2 r1.08
 * (T11-2019-00210-v004).
>>>>>>> upstream/android-13
 */

#ifndef _NVME_FC_H
#define _NVME_FC_H 1

<<<<<<< HEAD

#define NVME_CMD_SCSI_ID		0xFD
#define NVME_CMD_FC_ID			FC_TYPE_NVME

/* FC-NVME Cmd IU Flags */
#define FCNVME_CMD_FLAGS_DIRMASK	0x03
#define FCNVME_CMD_FLAGS_WRITE		0x01
#define FCNVME_CMD_FLAGS_READ		0x02

struct nvme_fc_cmd_iu {
	__u8			scsi_id;
	__u8			fc_id;
	__be16			iu_len;
	__u8			rsvd4[3];
=======
#include <uapi/scsi/fc/fc_fs.h>

#define NVME_CMD_FORMAT_ID		0xFD
#define NVME_CMD_FC_ID			FC_TYPE_NVME

/* FC-NVME Cmd IU Flags */
enum {
	FCNVME_CMD_FLAGS_DIRMASK	= 0x03,
	FCNVME_CMD_FLAGS_WRITE		= (1 << 0),
	FCNVME_CMD_FLAGS_READ		= (1 << 1),

	FCNVME_CMD_FLAGS_PICWP		= (1 << 2),
};

enum {
	FCNVME_CMD_CAT_MASK		= 0x0F,
	FCNVME_CMD_CAT_ADMINQ		= 0x01,
	FCNVME_CMD_CAT_CSSMASK		= 0x07,
	FCNVME_CMD_CAT_CSSFLAG		= 0x08,
};

static inline __u8 fccmnd_set_cat_admin(__u8 rsv_cat)
{
	return (rsv_cat & ~FCNVME_CMD_CAT_MASK) | FCNVME_CMD_CAT_ADMINQ;
}

static inline __u8 fccmnd_set_cat_css(__u8 rsv_cat, __u8 css)
{
	return (rsv_cat & ~FCNVME_CMD_CAT_MASK) | FCNVME_CMD_CAT_CSSFLAG |
		(css & FCNVME_CMD_CAT_CSSMASK);
}

struct nvme_fc_cmd_iu {
	__u8			format_id;
	__u8			fc_id;
	__be16			iu_len;
	__u8			rsvd4[2];
	__u8			rsv_cat;
>>>>>>> upstream/android-13
	__u8			flags;
	__be64			connection_id;
	__be32			csn;
	__be32			data_len;
	struct nvme_command	sqe;
<<<<<<< HEAD
	__be32			rsvd88[2];
=======
	__u8			dps;
	__u8			lbads;
	__be16			ms;
	__be32			rsvd92;
>>>>>>> upstream/android-13
};

#define NVME_FC_SIZEOF_ZEROS_RSP	12

enum {
	FCNVME_SC_SUCCESS		= 0,
	FCNVME_SC_INVALID_FIELD		= 1,
<<<<<<< HEAD
	FCNVME_SC_INVALID_CONNID	= 2,
};

struct nvme_fc_ersp_iu {
	__u8			status_code;
=======
	/* reserved			  2 */
	FCNVME_SC_ILL_CONN_PARAMS	= 3,
};

struct nvme_fc_ersp_iu {
	__u8			ersp_result;
>>>>>>> upstream/android-13
	__u8			rsvd1;
	__be16			iu_len;
	__be32			rsn;
	__be32			xfrd_len;
	__be32			rsvd12;
	struct nvme_completion	cqe;
	/* for now - no additional payload */
};


<<<<<<< HEAD
/* FC-NVME Link Services */
=======
#define FCNVME_NVME_SR_OPCODE		0x01
#define FCNVME_NVME_SR_RSP_OPCODE	0x02

struct nvme_fc_nvme_sr_iu {
	__u8			fc_id;
	__u8			opcode;
	__u8			rsvd2;
	__u8			retry_rctl;
	__be32			rsvd4;
};


enum {
	FCNVME_SRSTAT_ACC		= 0x0,
	/* reserved			  0x1 */
	/* reserved			  0x2 */
	FCNVME_SRSTAT_LOGICAL_ERR	= 0x3,
	FCNVME_SRSTAT_INV_QUALIF	= 0x4,
	FCNVME_SRSTAT_UNABL2PERFORM	= 0x9,
};

struct nvme_fc_nvme_sr_rsp_iu {
	__u8			fc_id;
	__u8			opcode;
	__u8			rsvd2;
	__u8			status;
	__be32			rsvd4;
};


/* FC-NVME Link Services - LS cmd values (w0 bits 31:24) */
>>>>>>> upstream/android-13
enum {
	FCNVME_LS_RSVD			= 0,
	FCNVME_LS_RJT			= 1,
	FCNVME_LS_ACC			= 2,
<<<<<<< HEAD
	FCNVME_LS_CREATE_ASSOCIATION	= 3,
	FCNVME_LS_CREATE_CONNECTION	= 4,
	FCNVME_LS_DISCONNECT		= 5,
=======
	FCNVME_LS_CREATE_ASSOCIATION	= 3,	/* Create Association */
	FCNVME_LS_CREATE_CONNECTION	= 4,	/* Create I/O Connection */
	FCNVME_LS_DISCONNECT_ASSOC	= 5,	/* Disconnect Association */
	FCNVME_LS_DISCONNECT_CONN	= 6,	/* Disconnect Connection */
>>>>>>> upstream/android-13
};

/* FC-NVME Link Service Descriptors */
enum {
	FCNVME_LSDESC_RSVD		= 0x0,
	FCNVME_LSDESC_RQST		= 0x1,
	FCNVME_LSDESC_RJT		= 0x2,
	FCNVME_LSDESC_CREATE_ASSOC_CMD	= 0x3,
	FCNVME_LSDESC_CREATE_CONN_CMD	= 0x4,
	FCNVME_LSDESC_DISCONN_CMD	= 0x5,
	FCNVME_LSDESC_CONN_ID		= 0x6,
	FCNVME_LSDESC_ASSOC_ID		= 0x7,
};


/* ********** start of Link Service Descriptors ********** */


/*
 * fills in length of a descriptor. Struture minus descriptor header
 */
static inline __be32 fcnvme_lsdesc_len(size_t sz)
{
	return cpu_to_be32(sz - (2 * sizeof(u32)));
}

struct fcnvme_ls_rqst_w0 {
	u8	ls_cmd;			/* FCNVME_LS_xxx */
	u8	zeros[3];
};

/* FCNVME_LSDESC_RQST */
struct fcnvme_lsdesc_rqst {
	__be32	desc_tag;		/* FCNVME_LSDESC_xxx */
	__be32	desc_len;
	struct fcnvme_ls_rqst_w0	w0;
	__be32	rsvd12;
};

/* FC-NVME LS RJT reason_code values */
enum fcnvme_ls_rjt_reason {
	FCNVME_RJT_RC_NONE		= 0,
	/* no reason - not to be sent */

	FCNVME_RJT_RC_INVAL		= 0x01,
	/* invalid NVMe_LS command code */

	FCNVME_RJT_RC_LOGIC		= 0x03,
	/* logical error */

	FCNVME_RJT_RC_UNAB		= 0x09,
	/* unable to perform command request */

	FCNVME_RJT_RC_UNSUP		= 0x0b,
	/* command not supported */

<<<<<<< HEAD
	FCNVME_RJT_RC_INPROG		= 0x0e,
	/* command already in progress */

	FCNVME_RJT_RC_INV_ASSOC		= 0x40,
	/* Invalid Association ID*/

	FCNVME_RJT_RC_INV_CONN		= 0x41,
	/* Invalid Connection ID*/
=======
	FCNVME_RJT_RC_INV_ASSOC		= 0x40,
	/* Invalid Association ID */

	FCNVME_RJT_RC_INV_CONN		= 0x41,
	/* Invalid Connection ID */

	FCNVME_RJT_RC_INV_PARAM		= 0x42,
	/* Invalid Parameters */

	FCNVME_RJT_RC_INSUF_RES		= 0x43,
	/* Insufficient Resources */
>>>>>>> upstream/android-13

	FCNVME_RJT_RC_VENDOR		= 0xff,
	/* vendor specific error */
};

/* FC-NVME LS RJT reason_explanation values */
enum fcnvme_ls_rjt_explan {
	FCNVME_RJT_EXP_NONE		= 0x00,
	/* No additional explanation */

	FCNVME_RJT_EXP_OXID_RXID	= 0x17,
	/* invalid OX_ID-RX_ID combination */

<<<<<<< HEAD
	FCNVME_RJT_EXP_INSUF_RES	= 0x29,
	/* insufficient resources */

=======
>>>>>>> upstream/android-13
	FCNVME_RJT_EXP_UNAB_DATA	= 0x2a,
	/* unable to supply requested data */

	FCNVME_RJT_EXP_INV_LEN		= 0x2d,
	/* Invalid payload length */
<<<<<<< HEAD
=======

	FCNVME_RJT_EXP_INV_ERSP_RAT	= 0x40,
	/* Invalid NVMe_ERSP Ratio */

	FCNVME_RJT_EXP_INV_CTLR_ID	= 0x41,
	/* Invalid Controller ID */

	FCNVME_RJT_EXP_INV_QUEUE_ID	= 0x42,
	/* Invalid Queue ID */

	FCNVME_RJT_EXP_INV_SQSIZE	= 0x43,
	/* Invalid Submission Queue Size */

	FCNVME_RJT_EXP_INV_HOSTID	= 0x44,
	/* Invalid HOST ID */

	FCNVME_RJT_EXP_INV_HOSTNQN	= 0x45,
	/* Invalid HOSTNQN */

	FCNVME_RJT_EXP_INV_SUBNQN	= 0x46,
	/* Invalid SUBNQN */
>>>>>>> upstream/android-13
};

/* FCNVME_LSDESC_RJT */
struct fcnvme_lsdesc_rjt {
	__be32	desc_tag;		/* FCNVME_LSDESC_xxx */
	__be32	desc_len;
	u8	rsvd8;

	/*
	 * Reject reason and explanaction codes are generic
	 * to ELs's from LS-3.
	 */
	u8	reason_code;		/* fcnvme_ls_rjt_reason */
	u8	reason_explanation;	/* fcnvme_ls_rjt_explan */

	u8	vendor;
	__be32	rsvd12;
};


#define FCNVME_ASSOC_HOSTNQN_LEN	256
#define FCNVME_ASSOC_SUBNQN_LEN		256

/* FCNVME_LSDESC_CREATE_ASSOC_CMD */
struct fcnvme_lsdesc_cr_assoc_cmd {
	__be32	desc_tag;		/* FCNVME_LSDESC_xxx */
	__be32	desc_len;
	__be16	ersp_ratio;
	__be16	rsvd10;
	__be32	rsvd12[9];
	__be16	cntlid;
	__be16	sqsize;
	__be32	rsvd52;
	uuid_t	hostid;
	u8	hostnqn[FCNVME_ASSOC_HOSTNQN_LEN];
	u8	subnqn[FCNVME_ASSOC_SUBNQN_LEN];
	__be32	rsvd584[108];		/* pad to 1016 bytes,
					 * which makes overall LS rqst
					 * payload 1024 bytes
					 */
};

#define FCNVME_LSDESC_CRA_CMD_DESC_MINLEN	\
		offsetof(struct fcnvme_lsdesc_cr_assoc_cmd, rsvd584)

#define FCNVME_LSDESC_CRA_CMD_DESC_MIN_DESCLEN	\
		(FCNVME_LSDESC_CRA_CMD_DESC_MINLEN - \
		 offsetof(struct fcnvme_lsdesc_cr_assoc_cmd, ersp_ratio))



/* FCNVME_LSDESC_CREATE_CONN_CMD */
struct fcnvme_lsdesc_cr_conn_cmd {
	__be32	desc_tag;		/* FCNVME_LSDESC_xxx */
	__be32	desc_len;
	__be16	ersp_ratio;
	__be16	rsvd10;
	__be32	rsvd12[9];
	__be16	qid;
	__be16	sqsize;
	__be32  rsvd52;
};

<<<<<<< HEAD
/* Disconnect Scope Values */
enum {
	FCNVME_DISCONN_ASSOCIATION	= 0,
	FCNVME_DISCONN_CONNECTION	= 1,
};

=======
>>>>>>> upstream/android-13
/* FCNVME_LSDESC_DISCONN_CMD */
struct fcnvme_lsdesc_disconn_cmd {
	__be32	desc_tag;		/* FCNVME_LSDESC_xxx */
	__be32	desc_len;
<<<<<<< HEAD
	u8	rsvd8[3];
	/* note: scope is really a 1 bit field */
	u8	scope;			/* FCNVME_DISCONN_xxx */
	__be32	rsvd12;
	__be64	id;
=======
	__be32	rsvd8[4];
>>>>>>> upstream/android-13
};

/* FCNVME_LSDESC_CONN_ID */
struct fcnvme_lsdesc_conn_id {
	__be32	desc_tag;		/* FCNVME_LSDESC_xxx */
	__be32	desc_len;
	__be64	connection_id;
};

/* FCNVME_LSDESC_ASSOC_ID */
struct fcnvme_lsdesc_assoc_id {
	__be32	desc_tag;		/* FCNVME_LSDESC_xxx */
	__be32	desc_len;
	__be64	association_id;
};

/* r_ctl values */
enum {
<<<<<<< HEAD
	FCNVME_RS_RCTL_DATA		= 1,
	FCNVME_RS_RCTL_XFER_RDY		= 5,
	FCNVME_RS_RCTL_RSP		= 8,
=======
	FCNVME_RS_RCTL_CMND		= 0x6,
	FCNVME_RS_RCTL_DATA		= 0x1,
	FCNVME_RS_RCTL_CONF		= 0x3,
	FCNVME_RS_RCTL_SR		= 0x9,
	FCNVME_RS_RCTL_XFER_RDY		= 0x5,
	FCNVME_RS_RCTL_RSP		= 0x7,
	FCNVME_RS_RCTL_ERSP		= 0x8,
	FCNVME_RS_RCTL_SR_RSP		= 0xA,
>>>>>>> upstream/android-13
};


/* ********** start of Link Services ********** */


/* FCNVME_LS_RJT */
struct fcnvme_ls_rjt {
	struct fcnvme_ls_rqst_w0		w0;
	__be32					desc_list_len;
	struct fcnvme_lsdesc_rqst		rqst;
	struct fcnvme_lsdesc_rjt		rjt;
};

/* FCNVME_LS_ACC */
struct fcnvme_ls_acc_hdr {
	struct fcnvme_ls_rqst_w0		w0;
	__be32					desc_list_len;
	struct fcnvme_lsdesc_rqst		rqst;
<<<<<<< HEAD
	/* Followed by cmd-specific ACC descriptors, see next definitions */
=======
	/*
	 * Followed by cmd-specific ACCEPT descriptors, see xxx_acc
	 * definitions below
	 */
>>>>>>> upstream/android-13
};

/* FCNVME_LS_CREATE_ASSOCIATION */
struct fcnvme_ls_cr_assoc_rqst {
	struct fcnvme_ls_rqst_w0		w0;
	__be32					desc_list_len;
	struct fcnvme_lsdesc_cr_assoc_cmd	assoc_cmd;
};

#define FCNVME_LSDESC_CRA_RQST_MINLEN	\
		(offsetof(struct fcnvme_ls_cr_assoc_rqst, assoc_cmd) + \
			FCNVME_LSDESC_CRA_CMD_DESC_MINLEN)

#define FCNVME_LSDESC_CRA_RQST_MIN_LISTLEN	\
		FCNVME_LSDESC_CRA_CMD_DESC_MINLEN


struct fcnvme_ls_cr_assoc_acc {
	struct fcnvme_ls_acc_hdr		hdr;
	struct fcnvme_lsdesc_assoc_id		associd;
	struct fcnvme_lsdesc_conn_id		connectid;
};


/* FCNVME_LS_CREATE_CONNECTION */
struct fcnvme_ls_cr_conn_rqst {
	struct fcnvme_ls_rqst_w0		w0;
	__be32					desc_list_len;
	struct fcnvme_lsdesc_assoc_id		associd;
	struct fcnvme_lsdesc_cr_conn_cmd	connect_cmd;
};

struct fcnvme_ls_cr_conn_acc {
	struct fcnvme_ls_acc_hdr		hdr;
	struct fcnvme_lsdesc_conn_id		connectid;
};

<<<<<<< HEAD
/* FCNVME_LS_DISCONNECT */
struct fcnvme_ls_disconnect_rqst {
=======
/* FCNVME_LS_DISCONNECT_ASSOC */
struct fcnvme_ls_disconnect_assoc_rqst {
>>>>>>> upstream/android-13
	struct fcnvme_ls_rqst_w0		w0;
	__be32					desc_list_len;
	struct fcnvme_lsdesc_assoc_id		associd;
	struct fcnvme_lsdesc_disconn_cmd	discon_cmd;
};

<<<<<<< HEAD
struct fcnvme_ls_disconnect_acc {
=======
struct fcnvme_ls_disconnect_assoc_acc {
	struct fcnvme_ls_acc_hdr		hdr;
};


/* FCNVME_LS_DISCONNECT_CONN */
struct fcnvme_ls_disconnect_conn_rqst {
	struct fcnvme_ls_rqst_w0		w0;
	__be32					desc_list_len;
	struct fcnvme_lsdesc_assoc_id		associd;
	struct fcnvme_lsdesc_conn_id		connectid;
};

struct fcnvme_ls_disconnect_conn_acc {
>>>>>>> upstream/android-13
	struct fcnvme_ls_acc_hdr		hdr;
};


/*
<<<<<<< HEAD
 * Yet to be defined in FC-NVME:
 */
#define NVME_FC_CONNECT_TIMEOUT_SEC	2		/* 2 seconds */
#define NVME_FC_LS_TIMEOUT_SEC		2		/* 2 seconds */
#define NVME_FC_TGTOP_TIMEOUT_SEC	2		/* 2 seconds */
=======
 * Default R_A_TOV is pulled in from fc_fs.h but needs conversion
 * from ms to seconds for our use.
 */
#define FC_TWO_TIMES_R_A_TOV		(2 * (FC_DEF_R_A_TOV / 1000))
#define NVME_FC_LS_TIMEOUT_SEC		FC_TWO_TIMES_R_A_TOV
#define NVME_FC_TGTOP_TIMEOUT_SEC	FC_TWO_TIMES_R_A_TOV
>>>>>>> upstream/android-13

/*
 * TRADDR string must be of form "nn-<16hexdigits>:pn-<16hexdigits>"
 * the string is allowed to be specified with or without a "0x" prefix
 * infront of the <16hexdigits>.  Without is considered the "min" string
 * and with is considered the "max" string. The hexdigits may be upper
 * or lower case.
<<<<<<< HEAD
=======
 * Note: FC-NVME-2 standard requires a "0x" prefix.
>>>>>>> upstream/android-13
 */
#define NVME_FC_TRADDR_NNLEN		3	/* "?n-" */
#define NVME_FC_TRADDR_OXNNLEN		5	/* "?n-0x" */
#define NVME_FC_TRADDR_HEXNAMELEN	16
#define NVME_FC_TRADDR_MINLENGTH	\
		(2 * (NVME_FC_TRADDR_NNLEN + NVME_FC_TRADDR_HEXNAMELEN) + 1)
#define NVME_FC_TRADDR_MAXLENGTH	\
		(2 * (NVME_FC_TRADDR_OXNNLEN + NVME_FC_TRADDR_HEXNAMELEN) + 1)
#define NVME_FC_TRADDR_MIN_PN_OFFSET	\
		(NVME_FC_TRADDR_NNLEN + NVME_FC_TRADDR_HEXNAMELEN + 1)
#define NVME_FC_TRADDR_MAX_PN_OFFSET	\
		(NVME_FC_TRADDR_OXNNLEN + NVME_FC_TRADDR_HEXNAMELEN + 1)


#endif /* _NVME_FC_H */
