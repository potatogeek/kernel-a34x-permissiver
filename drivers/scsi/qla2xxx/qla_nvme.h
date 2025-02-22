<<<<<<< HEAD
/*
 * QLogic Fibre Channel HBA Driver
 * Copyright (c)  2003-2017 QLogic Corporation
 *
 * See LICENSE.qla2xxx for copyright and licensing details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * QLogic Fibre Channel HBA Driver
 * Copyright (c)  2003-2017 QLogic Corporation
>>>>>>> upstream/android-13
 */
#ifndef __QLA_NVME_H
#define __QLA_NVME_H

<<<<<<< HEAD
#include <linux/blk-mq.h>
=======
>>>>>>> upstream/android-13
#include <uapi/scsi/fc/fc_fs.h>
#include <uapi/scsi/fc/fc_els.h>
#include <linux/nvme-fc-driver.h>

#include "qla_def.h"
<<<<<<< HEAD

/* default dev loss time (seconds) before transport tears down ctrl */
#define NVME_FC_DEV_LOSS_TMO  30
=======
#include "qla_dsd.h"
>>>>>>> upstream/android-13

#define NVME_ATIO_CMD_OFF 32
#define NVME_FIRST_PACKET_CMDLEN (64 - NVME_ATIO_CMD_OFF)
#define Q2T_NVME_NUM_TAGS 2048
#define QLA_MAX_FC_SEGMENTS 64

struct scsi_qla_host;
struct qla_hw_data;
struct req_que;
struct srb;

struct nvme_private {
	struct srb	*sp;
	struct nvmefc_ls_req *fd;
	struct work_struct ls_work;
	struct work_struct abort_work;
	int comp_status;
<<<<<<< HEAD
};

struct qla_nvme_rport {
	struct list_head list;
=======
	spinlock_t cmd_lock;
};

struct qla_nvme_rport {
>>>>>>> upstream/android-13
	struct fc_port *fcport;
};

#define COMMAND_NVME    0x88            /* Command Type FC-NVMe IOCB */
struct cmd_nvme {
	uint8_t entry_type;             /* Entry type. */
	uint8_t entry_count;            /* Entry count. */
	uint8_t sys_define;             /* System defined. */
	uint8_t entry_status;           /* Entry Status. */

	uint32_t handle;                /* System handle. */
<<<<<<< HEAD
	uint16_t nport_handle;          /* N_PORT handle. */
	uint16_t timeout;               /* Command timeout. */

	uint16_t dseg_count;            /* Data segment count. */
	uint16_t nvme_rsp_dsd_len;      /* NVMe RSP DSD length */

	uint64_t rsvd;

	uint16_t control_flags;         /* Control Flags */
#define CF_NVME_ENABLE                  BIT_9
=======
	__le16	nport_handle;		/* N_PORT handle. */
	__le16	timeout;		/* Command timeout. */

	__le16	dseg_count;		/* Data segment count. */
	__le16	nvme_rsp_dsd_len;	/* NVMe RSP DSD length */

	uint64_t rsvd;

	__le16	control_flags;		/* Control Flags */
#define CF_ADMIN_ASYNC_EVENT		BIT_13
#define CF_NVME_FIRST_BURST_ENABLE	BIT_11
>>>>>>> upstream/android-13
#define CF_DIF_SEG_DESCR_ENABLE         BIT_3
#define CF_DATA_SEG_DESCR_ENABLE        BIT_2
#define CF_READ_DATA                    BIT_1
#define CF_WRITE_DATA                   BIT_0

<<<<<<< HEAD
	uint16_t nvme_cmnd_dseg_len;             /* Data segment length. */
	uint32_t nvme_cmnd_dseg_address[2];      /* Data segment address. */
	uint32_t nvme_rsp_dseg_address[2];       /* Data segment address. */

	uint32_t byte_count;            /* Total byte count. */
=======
	__le16	nvme_cmnd_dseg_len;             /* Data segment length. */
	__le64	 nvme_cmnd_dseg_address __packed;/* Data segment address. */
	__le64	 nvme_rsp_dseg_address __packed; /* Data segment address. */

	__le32	byte_count;		/* Total byte count. */
>>>>>>> upstream/android-13

	uint8_t port_id[3];             /* PortID of destination port. */
	uint8_t vp_index;

<<<<<<< HEAD
	uint32_t nvme_data_dseg_address[2];      /* Data segment address. */
	uint32_t nvme_data_dseg_len;             /* Data segment length. */
=======
	struct dsd64 nvme_dsd;
>>>>>>> upstream/android-13
};

#define PT_LS4_REQUEST 0x89	/* Link Service pass-through IOCB (request) */
struct pt_ls4_request {
	uint8_t entry_type;
	uint8_t entry_count;
	uint8_t sys_define;
	uint8_t entry_status;
	uint32_t handle;
<<<<<<< HEAD
	uint16_t status;
	uint16_t nport_handle;
	uint16_t tx_dseg_count;
	uint8_t  vp_index;
	uint8_t  rsvd;
	uint16_t timeout;
	uint16_t control_flags;
=======
	__le16	status;
	__le16	nport_handle;
	__le16	tx_dseg_count;
	uint8_t  vp_index;
	uint8_t  rsvd;
	__le16	timeout;
	__le16	control_flags;
>>>>>>> upstream/android-13
#define CF_LS4_SHIFT		13
#define CF_LS4_ORIGINATOR	0
#define CF_LS4_RESPONDER	1
#define CF_LS4_RESPONDER_TERM	2

<<<<<<< HEAD
	uint16_t rx_dseg_count;
	uint16_t rsvd2;
	uint32_t exchange_address;
	uint32_t rsvd3;
	uint32_t rx_byte_count;
	uint32_t tx_byte_count;
	uint32_t dseg0_address[2];
	uint32_t dseg0_len;
	uint32_t dseg1_address[2];
	uint32_t dseg1_len;
=======
	__le16	rx_dseg_count;
	__le16	rsvd2;
	__le32	exchange_address;
	__le32	rsvd3;
	__le32	rx_byte_count;
	__le32	tx_byte_count;
	struct dsd64 dsd[2];
>>>>>>> upstream/android-13
};

#define PT_LS4_UNSOL 0x56	/* pass-up unsolicited rec FC-NVMe request */
struct pt_ls4_rx_unsol {
	uint8_t entry_type;
	uint8_t entry_count;
<<<<<<< HEAD
	uint16_t rsvd0;
	uint16_t rsvd1;
	uint8_t vp_index;
	uint8_t rsvd2;
	uint16_t rsvd3;
	uint16_t nport_handle;
	uint16_t frame_size;
	uint16_t rsvd4;
	uint32_t exchange_address;
	uint8_t d_id[3];
	uint8_t r_ctl;
	uint8_t s_id[3];
	uint8_t cs_ctl;
	uint8_t f_ctl[3];
	uint8_t type;
	uint16_t seq_cnt;
	uint8_t df_ctl;
	uint8_t seq_id;
	uint16_t rx_id;
	uint16_t ox_id;
	uint32_t param;
	uint32_t desc0;
#define PT_LS4_PAYLOAD_OFFSET 0x2c
#define PT_LS4_FIRST_PACKET_LEN 20
	uint32_t desc_len;
	uint32_t payload[3];
=======
	__le16	rsvd0;
	__le16	rsvd1;
	uint8_t vp_index;
	uint8_t rsvd2;
	__le16	rsvd3;
	__le16	nport_handle;
	__le16	frame_size;
	__le16	rsvd4;
	__le32	exchange_address;
	uint8_t d_id[3];
	uint8_t r_ctl;
	be_id_t s_id;
	uint8_t cs_ctl;
	uint8_t f_ctl[3];
	uint8_t type;
	__le16	seq_cnt;
	uint8_t df_ctl;
	uint8_t seq_id;
	__le16	rx_id;
	__le16	ox_id;
	__le32	param;
	__le32	desc0;
#define PT_LS4_PAYLOAD_OFFSET 0x2c
#define PT_LS4_FIRST_PACKET_LEN 20
	__le32	desc_len;
	__le32	payload[3];
>>>>>>> upstream/android-13
};

/*
 * Global functions prototype in qla_nvme.c source file.
 */
int qla_nvme_register_hba(struct scsi_qla_host *);
int  qla_nvme_register_remote(struct scsi_qla_host *, struct fc_port *);
void qla_nvme_delete(struct scsi_qla_host *);
<<<<<<< HEAD
void qla_nvme_abort(struct qla_hw_data *, struct srb *sp, int res);
void qla24xx_nvme_ls4_iocb(struct scsi_qla_host *, struct pt_ls4_request *,
    struct req_que *);
void qla24xx_async_gffid_sp_done(void *, int);
=======
void qla24xx_nvme_ls4_iocb(struct scsi_qla_host *, struct pt_ls4_request *,
    struct req_que *);
void qla24xx_async_gffid_sp_done(struct srb *sp, int);
>>>>>>> upstream/android-13
#endif
