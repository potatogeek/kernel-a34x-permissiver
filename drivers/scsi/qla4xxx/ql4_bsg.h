<<<<<<< HEAD
/*
 * QLogic iSCSI HBA Driver
 * Copyright (c) 2011 QLogic Corporation
 *
 * See LICENSE.qla4xxx for copyright and licensing details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * QLogic iSCSI HBA Driver
 * Copyright (c) 2011 QLogic Corporation
>>>>>>> upstream/android-13
 */
#ifndef __QL4_BSG_H
#define __QL4_BSG_H

/* BSG Vendor specific commands */
#define QLISCSI_VND_READ_FLASH		1
#define QLISCSI_VND_UPDATE_FLASH	2
#define QLISCSI_VND_GET_ACB_STATE	3
#define QLISCSI_VND_READ_NVRAM		4
#define QLISCSI_VND_UPDATE_NVRAM	5
#define QLISCSI_VND_RESTORE_DEFAULTS	6
#define QLISCSI_VND_GET_ACB		7
#define QLISCSI_VND_DIAG_TEST		8

/* QLISCSI_VND_DIAG_CMD sub code */
#define QL_DIAG_CMD_TEST_DDR_SIZE	0x2
#define QL_DIAG_CMD_TEST_DDR_RW		0x3
#define QL_DIAG_CMD_TEST_ONCHIP_MEM_RW	0x4
#define QL_DIAG_CMD_TEST_NVRAM		0x5	/* Only ISP4XXX */
#define QL_DIAG_CMD_TEST_FLASH_ROM	0x6
#define QL_DIAG_CMD_TEST_INT_LOOPBACK	0x7
#define QL_DIAG_CMD_TEST_EXT_LOOPBACK	0x8
#define QL_DIAG_CMD_TEST_DMA_XFER	0x9	/* Only ISP4XXX */
#define QL_DIAG_CMD_SELF_DDR_RW		0xC
#define QL_DIAG_CMD_SELF_ONCHIP_MEM_RW	0xD

#endif
