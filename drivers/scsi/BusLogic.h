<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*

  Linux Driver for BusLogic MultiMaster and FlashPoint SCSI Host Adapters

  Copyright 1995-1998 by Leonard N. Zubkoff <lnz@dandelion.com>

<<<<<<< HEAD
  This program is free software; you may redistribute and/or modify it under
  the terms of the GNU General Public License Version 2 as published by the
  Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY, without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
  for complete details.
=======
>>>>>>> upstream/android-13

  The author respectfully requests that any modifications to this software be
  sent directly to him for evaluation and testing.

  Special thanks to Wayne Yen, Jin-Lon Hon, and Alex Win of BusLogic, whose
  advice has been invaluable, to David Gentzel, for writing the original Linux
  BusLogic driver, and to Paul Gortmaker, for being such a dedicated test site.

  Finally, special thanks to Mylex/BusLogic for making the FlashPoint SCCB
  Manager available as freely redistributable source code.

*/

#ifndef _BUSLOGIC_H
#define _BUSLOGIC_H


#ifndef PACKED
#define PACKED __attribute__((packed))
#endif

/*
  Define the maximum number of BusLogic Host Adapters supported by this driver.
*/

#define BLOGIC_MAX_ADAPTERS		16


/*
  Define the maximum number of Target Devices supported by this driver.
*/

#define BLOGIC_MAXDEV			16


/*
  Define the maximum number of Scatter/Gather Segments used by this driver.
  For optimal performance, it is important that this limit be at least as
  large as the largest single request generated by the I/O Subsystem.
*/

#define BLOGIC_SG_LIMIT		128


/*
  Define the maximum, maximum automatic, minimum automatic, and default Queue
  Depth to allow for Target Devices depending on whether or not they support
  Tagged Queuing and whether or not ISA Bounce Buffers are required.
*/

#define BLOGIC_MAX_TAG_DEPTH		64
#define BLOGIC_MAX_AUTO_TAG_DEPTH	28
#define BLOGIC_MIN_AUTO_TAG_DEPTH	7
#define BLOGIC_TAG_DEPTH_BB		3
#define BLOGIC_UNTAG_DEPTH		3
#define BLOGIC_UNTAG_DEPTH_BB		2


/*
  Define the default amount of time in seconds to wait between a Host Adapter
  Hard Reset which initiates a SCSI Bus Reset and issuing any SCSI commands.
  Some SCSI devices get confused if they receive SCSI commands too soon after
  a SCSI Bus Reset.
*/

#define BLOGIC_BUS_SETTLE_TIME		2


/*
  Define the maximum number of Mailboxes that should be used for MultiMaster
  Host Adapters.  This number is chosen to be larger than the maximum Host
  Adapter Queue Depth and small enough so that the Host Adapter structure
  does not cross an allocation block size boundary.
*/

#define BLOGIC_MAX_MAILBOX		211


/*
  Define the number of CCBs that should be allocated as a group to optimize
  Kernel memory allocation.
*/

#define BLOGIC_CCB_GRP_ALLOCSIZE	7


/*
  Define the Host Adapter Line and Message Buffer Sizes.
*/

#define BLOGIC_LINEBUF_SIZE		100
#define BLOGIC_MSGBUF_SIZE		9700


/*
  Define the Driver Message Levels.
*/

enum blogic_msglevel {
	BLOGIC_ANNOUNCE_LEVEL = 0,
	BLOGIC_INFO_LEVEL = 1,
	BLOGIC_NOTICE_LEVEL = 2,
	BLOGIC_WARN_LEVEL = 3,
	BLOGIC_ERR_LEVEL = 4
};

static char *blogic_msglevelmap[] = { KERN_NOTICE, KERN_NOTICE, KERN_NOTICE, KERN_WARNING, KERN_ERR };


/*
  Define Driver Message macros.
*/

#define blogic_announce(format, args...) \
	blogic_msg(BLOGIC_ANNOUNCE_LEVEL, format, ##args)

#define blogic_info(format, args...) \
	blogic_msg(BLOGIC_INFO_LEVEL, format, ##args)

#define blogic_notice(format, args...) \
	blogic_msg(BLOGIC_NOTICE_LEVEL, format, ##args)

#define blogic_warn(format, args...) \
	blogic_msg(BLOGIC_WARN_LEVEL, format, ##args)

#define blogic_err(format, args...) \
	blogic_msg(BLOGIC_ERR_LEVEL, format, ##args)


/*
  Define the types of BusLogic Host Adapters that are supported and the number
  of I/O Addresses required by each type.
*/

enum blogic_adapter_type {
	BLOGIC_MULTIMASTER = 1,
	BLOGIC_FLASHPOINT = 2
} PACKED;

#define BLOGIC_MULTIMASTER_ADDR_COUNT	4
#define BLOGIC_FLASHPOINT_ADDR_COUNT	256

static int blogic_adapter_addr_count[3] = { 0, BLOGIC_MULTIMASTER_ADDR_COUNT, BLOGIC_FLASHPOINT_ADDR_COUNT };


/*
  Define macros for testing the Host Adapter Type.
*/

#ifdef CONFIG_SCSI_FLASHPOINT

#define blogic_multimaster_type(adapter) \
	(adapter->adapter_type == BLOGIC_MULTIMASTER)

#define blogic_flashpoint_type(adapter) \
	(adapter->adapter_type == BLOGIC_FLASHPOINT)

#else

#define blogic_multimaster_type(adapter)	(true)
#define blogic_flashpoint_type(adapter)		(false)

#endif


/*
  Define the possible Host Adapter Bus Types.
*/

enum blogic_adapter_bus_type {
	BLOGIC_UNKNOWN_BUS = 0,
	BLOGIC_ISA_BUS = 1,
	BLOGIC_EISA_BUS = 2,
	BLOGIC_PCI_BUS = 3,
	BLOGIC_VESA_BUS = 4,
	BLOGIC_MCA_BUS = 5
} PACKED;

static char *blogic_adapter_busnames[] = { "Unknown", "ISA", "EISA", "PCI", "VESA", "MCA" };

static enum blogic_adapter_bus_type blogic_adater_bus_types[] = {
	BLOGIC_VESA_BUS,	/* BT-4xx */
	BLOGIC_ISA_BUS,		/* BT-5xx */
	BLOGIC_MCA_BUS,		/* BT-6xx */
	BLOGIC_EISA_BUS,	/* BT-7xx */
	BLOGIC_UNKNOWN_BUS,	/* BT-8xx */
	BLOGIC_PCI_BUS		/* BT-9xx */
};

/*
  Define the possible Host Adapter BIOS Disk Geometry Translations.
*/

enum blogic_bios_diskgeometry {
	BLOGIC_BIOS_NODISK = 0,
	BLOGIC_BIOS_DISK64x32 = 1,
	BLOGIC_BIOS_DISK128x32 = 2,
	BLOGIC_BIOS_DISK255x63 = 3
} PACKED;


/*
  Define a 10^18 Statistics Byte Counter data type.
*/

struct blogic_byte_count {
	unsigned int units;
	unsigned int billions;
};


/*
  Define the structure for I/O Address and Bus Probing Information.
*/

struct blogic_probeinfo {
	enum blogic_adapter_type adapter_type;
	enum blogic_adapter_bus_type adapter_bus_type;
	unsigned long io_addr;
	unsigned long pci_addr;
	struct pci_dev *pci_device;
	unsigned char bus;
	unsigned char dev;
	unsigned char irq_ch;
};

/*
  Define the Probe Options.
*/

struct blogic_probe_options {
	bool noprobe:1;			/* Bit 0 */
<<<<<<< HEAD
	bool noprobe_isa:1;		/* Bit 1 */
=======
>>>>>>> upstream/android-13
	bool noprobe_pci:1;		/* Bit 2 */
	bool nosort_pci:1;		/* Bit 3 */
	bool multimaster_first:1;	/* Bit 4 */
	bool flashpoint_first:1;	/* Bit 5 */
<<<<<<< HEAD
	bool limited_isa:1;		/* Bit 6 */
	bool probe330:1;		/* Bit 7 */
	bool probe334:1;		/* Bit 8 */
	bool probe230:1;		/* Bit 9 */
	bool probe234:1;		/* Bit 10 */
	bool probe130:1;		/* Bit 11 */
	bool probe134:1;		/* Bit 12 */
=======
>>>>>>> upstream/android-13
};

/*
  Define the Global Options.
*/

struct blogic_global_options {
	bool trace_probe:1;	/* Bit 0 */
	bool trace_hw_reset:1;	/* Bit 1 */
	bool trace_config:1;	/* Bit 2 */
	bool trace_err:1;	/* Bit 3 */
};

/*
  Define the BusLogic SCSI Host Adapter I/O Register Offsets.
*/

#define BLOGIC_CNTRL_REG	0	/* WO register */
#define BLOGIC_STATUS_REG	0	/* RO register */
#define BLOGIC_CMD_PARM_REG	1	/* WO register */
#define BLOGIC_DATAIN_REG	1	/* RO register */
#define BLOGIC_INT_REG		2	/* RO register */
#define BLOGIC_GEOMETRY_REG	3	/* RO register */

/*
  Define the structure of the write-only Control Register.
*/

union blogic_cntrl_reg {
	unsigned char all;
	struct {
		unsigned char:4;	/* Bits 0-3 */
		bool bus_reset:1;	/* Bit 4 */
		bool int_reset:1;	/* Bit 5 */
		bool soft_reset:1;	/* Bit 6 */
		bool hard_reset:1;	/* Bit 7 */
	} cr;
};

/*
  Define the structure of the read-only Status Register.
*/

union blogic_stat_reg {
	unsigned char all;
	struct {
		bool cmd_invalid:1;	/* Bit 0 */
		bool rsvd:1;		/* Bit 1 */
		bool datain_ready:1;	/* Bit 2 */
		bool cmd_param_busy:1;	/* Bit 3 */
		bool adapter_ready:1;	/* Bit 4 */
		bool init_reqd:1;	/* Bit 5 */
		bool diag_failed:1;	/* Bit 6 */
		bool diag_active:1;	/* Bit 7 */
	} sr;
};

/*
  Define the structure of the read-only Interrupt Register.
*/

union blogic_int_reg {
	unsigned char all;
	struct {
		bool mailin_loaded:1;	/* Bit 0 */
		bool mailout_avail:1;	/* Bit 1 */
		bool cmd_complete:1;	/* Bit 2 */
		bool ext_busreset:1;	/* Bit 3 */
		unsigned char rsvd:3;	/* Bits 4-6 */
		bool int_valid:1;	/* Bit 7 */
	} ir;
};

/*
  Define the structure of the read-only Geometry Register.
*/

union blogic_geo_reg {
	unsigned char all;
	struct {
		enum blogic_bios_diskgeometry d0_geo:2;	/* Bits 0-1 */
		enum blogic_bios_diskgeometry d1_geo:2;	/* Bits 2-3 */
		unsigned char:3;	/* Bits 4-6 */
		bool ext_trans_enable:1;	/* Bit 7 */
	} gr;
};

/*
  Define the BusLogic SCSI Host Adapter Command Register Operation Codes.
*/

enum blogic_opcode {
	BLOGIC_TEST_CMP_COMPLETE = 0x00,
	BLOGIC_INIT_MBOX = 0x01,
	BLOGIC_EXEC_MBOX_CMD = 0x02,
	BLOGIC_EXEC_BIOS_CMD = 0x03,
	BLOGIC_GET_BOARD_ID = 0x04,
	BLOGIC_ENABLE_OUTBOX_AVAIL_INT = 0x05,
	BLOGIC_SET_SELECT_TIMEOUT = 0x06,
	BLOGIC_SET_PREEMPT_TIME = 0x07,
	BLOGIC_SET_TIMEOFF_BUS = 0x08,
	BLOGIC_SET_TXRATE = 0x09,
	BLOGIC_INQ_DEV0TO7 = 0x0A,
	BLOGIC_INQ_CONFIG = 0x0B,
	BLOGIC_TGT_MODE = 0x0C,
	BLOGIC_INQ_SETUPINFO = 0x0D,
	BLOGIC_WRITE_LOCALRAM = 0x1A,
	BLOGIC_READ_LOCALRAM = 0x1B,
	BLOGIC_WRITE_BUSMASTER_FIFO = 0x1C,
	BLOGIC_READ_BUSMASTER_FIFO = 0x1D,
	BLOGIC_ECHO_CMDDATA = 0x1F,
	BLOGIC_ADAPTER_DIAG = 0x20,
	BLOGIC_SET_OPTIONS = 0x21,
	BLOGIC_INQ_DEV8TO15 = 0x23,
	BLOGIC_INQ_DEV = 0x24,
	BLOGIC_DISABLE_INT = 0x25,
	BLOGIC_INIT_EXT_MBOX = 0x81,
	BLOGIC_EXEC_SCS_CMD = 0x83,
	BLOGIC_INQ_FWVER_D3 = 0x84,
	BLOGIC_INQ_FWVER_LETTER = 0x85,
	BLOGIC_INQ_PCI_INFO = 0x86,
	BLOGIC_INQ_MODELNO = 0x8B,
	BLOGIC_INQ_SYNC_PERIOD = 0x8C,
	BLOGIC_INQ_EXTSETUP = 0x8D,
	BLOGIC_STRICT_RR = 0x8F,
	BLOGIC_STORE_LOCALRAM = 0x90,
	BLOGIC_FETCH_LOCALRAM = 0x91,
	BLOGIC_STORE_TO_EEPROM = 0x92,
	BLOGIC_LOAD_AUTOSCSICODE = 0x94,
	BLOGIC_MOD_IOADDR = 0x95,
	BLOGIC_SETCCB_FMT = 0x96,
	BLOGIC_WRITE_INQBUF = 0x9A,
	BLOGIC_READ_INQBUF = 0x9B,
	BLOGIC_FLASH_LOAD = 0xA7,
	BLOGIC_READ_SCAMDATA = 0xA8,
	BLOGIC_WRITE_SCAMDATA = 0xA9
};

/*
  Define the Inquire Board ID reply structure.
*/

struct blogic_board_id {
	unsigned char type;		/* Byte 0 */
	unsigned char custom_features;	/* Byte 1 */
	unsigned char fw_ver_digit1;	/* Byte 2 */
	unsigned char fw_ver_digit2;	/* Byte 3 */
};

/*
  Define the Inquire Configuration reply structure.
*/

struct blogic_config {
	unsigned char:5;	/* Byte 0 Bits 0-4 */
	bool dma_ch5:1;		/* Byte 0 Bit 5 */
	bool dma_ch6:1;		/* Byte 0 Bit 6 */
	bool dma_ch7:1;		/* Byte 0 Bit 7 */
	bool irq_ch9:1;		/* Byte 1 Bit 0 */
	bool irq_ch10:1;	/* Byte 1 Bit 1 */
	bool irq_ch11:1;	/* Byte 1 Bit 2 */
	bool irq_ch12:1;	/* Byte 1 Bit 3 */
	unsigned char:1;	/* Byte 1 Bit 4 */
	bool irq_ch14:1;	/* Byte 1 Bit 5 */
	bool irq_ch15:1;	/* Byte 1 Bit 6 */
	unsigned char:1;	/* Byte 1 Bit 7 */
	unsigned char id:4;	/* Byte 2 Bits 0-3 */
	unsigned char:4;	/* Byte 2 Bits 4-7 */
};

/*
  Define the Inquire Setup Information reply structure.
*/

struct blogic_syncval {
	unsigned char offset:4;		/* Bits 0-3 */
	unsigned char tx_period:3;	/* Bits 4-6 */
	bool sync:1;			/* Bit 7 */
};

struct blogic_setup_info {
	bool sync:1;				/* Byte 0 Bit 0 */
	bool parity:1;				/* Byte 0 Bit 1 */
	unsigned char:6;			/* Byte 0 Bits 2-7 */
	unsigned char tx_rate;			/* Byte 1 */
	unsigned char preempt_time;		/* Byte 2 */
	unsigned char timeoff_bus;		/* Byte 3 */
	unsigned char mbox_count;		/* Byte 4 */
	unsigned char mbox_addr[3];		/* Bytes 5-7 */
	struct blogic_syncval sync0to7[8];	/* Bytes 8-15 */
	unsigned char disconnect_ok0to7;	/* Byte 16 */
	unsigned char sig;			/* Byte 17 */
	unsigned char char_d;			/* Byte 18 */
	unsigned char bus_type;			/* Byte 19 */
	unsigned char wide_tx_ok0to7;		/* Byte 20 */
	unsigned char wide_tx_active0to7;	/* Byte 21 */
	struct blogic_syncval sync8to15[8];	/* Bytes 22-29 */
	unsigned char disconnect_ok8to15;	/* Byte 30 */
	unsigned char:8;			/* Byte 31 */
	unsigned char wide_tx_ok8to15;		/* Byte 32 */
	unsigned char wide_tx_active8to15;	/* Byte 33 */
};

/*
  Define the Initialize Extended Mailbox request structure.
*/

struct blogic_extmbox_req {
	unsigned char mbox_count;	/* Byte 0 */
	u32 base_mbox_addr;		/* Bytes 1-4 */
} PACKED;


/*
  Define the Inquire PCI Host Adapter Information reply type.  The ISA
  Compatible I/O Port values are defined here and are also used with
  the Modify I/O Address command.
*/

enum blogic_isa_ioport {
	BLOGIC_IO_330 = 0,
	BLOGIC_IO_334 = 1,
	BLOGIC_IO_230 = 2,
	BLOGIC_IO_234 = 3,
	BLOGIC_IO_130 = 4,
	BLOGIC_IO_134 = 5,
	BLOGIC_IO_DISABLE = 6,
	BLOGIC_IO_DISABLE2 = 7
} PACKED;

struct blogic_adapter_info {
	enum blogic_isa_ioport isa_port;	/* Byte 0 */
	unsigned char irq_ch;		/* Byte 1 */
	bool low_term:1;		/* Byte 2 Bit 0 */
	bool high_term:1;		/* Byte 2 Bit 1 */
	unsigned char:2;		/* Byte 2 Bits 2-3 */
	bool JP1:1;			/* Byte 2 Bit 4 */
	bool JP2:1;			/* Byte 2 Bit 5 */
	bool JP3:1;			/* Byte 2 Bit 6 */
	bool genericinfo_valid:1;	/* Byte 2 Bit 7 */
	unsigned char:8;		/* Byte 3 */
};

/*
  Define the Inquire Extended Setup Information reply structure.
*/

struct blogic_ext_setup {
	unsigned char bus_type;		/* Byte 0 */
	unsigned char bios_addr;	/* Byte 1 */
	unsigned short sg_limit;	/* Bytes 2-3 */
	unsigned char mbox_count;	/* Byte 4 */
	u32 base_mbox_addr;		/* Bytes 5-8 */
	struct {
		unsigned char:2;	/* Byte 9 Bits 0-1 */
		bool fast_on_eisa:1;	/* Byte 9 Bit 2 */
		unsigned char:3;	/* Byte 9 Bits 3-5 */
		bool level_int:1;	/* Byte 9 Bit 6 */
		unsigned char:1;	/* Byte 9 Bit 7 */
	} misc;
	unsigned char fw_rev[3];	/* Bytes 10-12 */
	bool wide:1;			/* Byte 13 Bit 0 */
	bool differential:1;		/* Byte 13 Bit 1 */
	bool scam:1;			/* Byte 13 Bit 2 */
	bool ultra:1;			/* Byte 13 Bit 3 */
	bool smart_term:1;		/* Byte 13 Bit 4 */
	unsigned char:3;		/* Byte 13 Bits 5-7 */
} PACKED;

/*
  Define the Enable Strict Round Robin Mode request type.
*/

enum blogic_rr_req {
	BLOGIC_AGGRESSIVE_RR = 0,
	BLOGIC_STRICT_RR_MODE = 1
} PACKED;


/*
  Define the Fetch Host Adapter Local RAM request type.
*/

#define BLOGIC_BIOS_BASE		0
#define BLOGIC_AUTOSCSI_BASE		64

struct blogic_fetch_localram {
	unsigned char offset;	/* Byte 0 */
	unsigned char count;	/* Byte 1 */
};

/*
  Define the Host Adapter Local RAM AutoSCSI structure.
*/

struct blogic_autoscsi {
	unsigned char factory_sig[2];		/* Bytes 0-1 */
	unsigned char info_bytes;		/* Byte 2 */
	unsigned char adapter_type[6];		/* Bytes 3-8 */
	unsigned char:8;			/* Byte 9 */
	bool floppy:1;				/* Byte 10 Bit 0 */
	bool floppy_sec:1;			/* Byte 10 Bit 1 */
	bool level_int:1;			/* Byte 10 Bit 2 */
	unsigned char:2;			/* Byte 10 Bits 3-4 */
	unsigned char systemram_bios:3;		/* Byte 10 Bits 5-7 */
	unsigned char dma_ch:7;			/* Byte 11 Bits 0-6 */
	bool dma_autoconf:1;			/* Byte 11 Bit 7 */
	unsigned char irq_ch:7;			/* Byte 12 Bits 0-6 */
	bool irq_autoconf:1;			/* Byte 12 Bit 7 */
	unsigned char dma_tx_rate;		/* Byte 13 */
	unsigned char scsi_id;			/* Byte 14 */
	bool low_term:1;			/* Byte 15 Bit 0 */
	bool parity:1;				/* Byte 15 Bit 1 */
	bool high_term:1;			/* Byte 15 Bit 2 */
	bool noisy_cable:1;			/* Byte 15 Bit 3 */
	bool fast_sync_neg:1;			/* Byte 15 Bit 4 */
	bool reset_enabled:1;			/* Byte 15 Bit 5 */
	bool:1;					/* Byte 15 Bit 6 */
	bool active_negation:1;			/* Byte 15 Bit 7 */
	unsigned char bus_on_delay;		/* Byte 16 */
	unsigned char bus_off_delay;		/* Byte 17 */
	bool bios_enabled:1;			/* Byte 18 Bit 0 */
	bool int19_redir_enabled:1;		/* Byte 18 Bit 1 */
	bool ext_trans_enable:1;		/* Byte 18 Bit 2 */
	bool removable_as_fixed:1;		/* Byte 18 Bit 3 */
	bool:1;					/* Byte 18 Bit 4 */
	bool morethan2_drives:1;		/* Byte 18 Bit 5 */
	bool bios_int:1;			/* Byte 18 Bit 6 */
	bool floptical:1;			/* Byte 19 Bit 7 */
	unsigned short dev_enabled;		/* Bytes 19-20 */
	unsigned short wide_ok;			/* Bytes 21-22 */
	unsigned short fast_ok;			/* Bytes 23-24 */
	unsigned short sync_ok;			/* Bytes 25-26 */
	unsigned short discon_ok;		/* Bytes 27-28 */
	unsigned short send_start_unit;		/* Bytes 29-30 */
	unsigned short ignore_bios_scan;	/* Bytes 31-32 */
	unsigned char pci_int_pin:2;		/* Byte 33 Bits 0-1 */
	unsigned char adapter_ioport:2;		/* Byte 33 Bits 2-3 */
	bool strict_rr_enabled:1;		/* Byte 33 Bit 4 */
	bool vesabus_33mhzplus:1;		/* Byte 33 Bit 5 */
	bool vesa_burst_write:1;		/* Byte 33 Bit 6 */
	bool vesa_burst_read:1;			/* Byte 33 Bit 7 */
	unsigned short ultra_ok;		/* Bytes 34-35 */
	unsigned int:32;			/* Bytes 36-39 */
	unsigned char:8;			/* Byte 40 */
	unsigned char autoscsi_maxlun;		/* Byte 41 */
	bool:1;					/* Byte 42 Bit 0 */
	bool scam_dominant:1;			/* Byte 42 Bit 1 */
	bool scam_enabled:1;			/* Byte 42 Bit 2 */
	bool scam_lev2:1;			/* Byte 42 Bit 3 */
	unsigned char:4;			/* Byte 42 Bits 4-7 */
	bool int13_exten:1;			/* Byte 43 Bit 0 */
	bool:1;					/* Byte 43 Bit 1 */
	bool cd_boot:1;				/* Byte 43 Bit 2 */
	unsigned char:5;			/* Byte 43 Bits 3-7 */
	unsigned char boot_id:4;		/* Byte 44 Bits 0-3 */
	unsigned char boot_ch:4;		/* Byte 44 Bits 4-7 */
	unsigned char force_scan_order:1;	/* Byte 45 Bit 0 */
	unsigned char:7;			/* Byte 45 Bits 1-7 */
	unsigned short nontagged_to_alt_ok;	/* Bytes 46-47 */
	unsigned short reneg_sync_on_check;	/* Bytes 48-49 */
	unsigned char rsvd[10];			/* Bytes 50-59 */
	unsigned char manuf_diag[2];		/* Bytes 60-61 */
	unsigned short cksum;			/* Bytes 62-63 */
} PACKED;

/*
  Define the Host Adapter Local RAM Auto SCSI Byte 45 structure.
*/

struct blogic_autoscsi_byte45 {
	unsigned char force_scan_order:1;	/* Bit 0 */
	unsigned char:7;	/* Bits 1-7 */
};

/*
  Define the Host Adapter Local RAM BIOS Drive Map Byte structure.
*/

#define BLOGIC_BIOS_DRVMAP		17

struct blogic_bios_drvmap {
	unsigned char tgt_idbit3:1;			/* Bit 0 */
	unsigned char:2;				/* Bits 1-2 */
	enum blogic_bios_diskgeometry diskgeom:2;	/* Bits 3-4 */
	unsigned char tgt_id:3;				/* Bits 5-7 */
};

/*
  Define the Set CCB Format request type.  Extended LUN Format CCBs are
  necessary to support more than 8 Logical Units per Target Device.
*/

enum blogic_setccb_fmt {
	BLOGIC_LEGACY_LUN_CCB = 0,
	BLOGIC_EXT_LUN_CCB = 1
} PACKED;

/*
  Define the Outgoing Mailbox Action Codes.
*/

enum blogic_action {
	BLOGIC_OUTBOX_FREE = 0x00,
	BLOGIC_MBOX_START = 0x01,
	BLOGIC_MBOX_ABORT = 0x02
} PACKED;


/*
  Define the Incoming Mailbox Completion Codes.  The MultiMaster Firmware
  only uses codes 0 - 4.  The FlashPoint SCCB Manager has no mailboxes, so
  completion codes are stored in the CCB; it only uses codes 1, 2, 4, and 5.
*/

enum blogic_cmplt_code {
	BLOGIC_INBOX_FREE = 0x00,
	BLOGIC_CMD_COMPLETE_GOOD = 0x01,
	BLOGIC_CMD_ABORT_BY_HOST = 0x02,
	BLOGIC_CMD_NOTFOUND = 0x03,
	BLOGIC_CMD_COMPLETE_ERROR = 0x04,
	BLOGIC_INVALID_CCB = 0x05
} PACKED;

/*
  Define the Command Control Block (CCB) Opcodes.
*/

enum blogic_ccb_opcode {
	BLOGIC_INITIATOR_CCB = 0x00,
	BLOGIC_TGT_CCB = 0x01,
	BLOGIC_INITIATOR_CCB_SG = 0x02,
	BLOGIC_INITIATOR_CCBB_RESIDUAL = 0x03,
	BLOGIC_INITIATOR_CCB_SG_RESIDUAL = 0x04,
	BLOGIC_BDR = 0x81
} PACKED;


/*
  Define the CCB Data Direction Codes.
*/

enum blogic_datadir {
	BLOGIC_UNCHECKED_TX = 0,
	BLOGIC_DATAIN_CHECKED = 1,
	BLOGIC_DATAOUT_CHECKED = 2,
	BLOGIC_NOTX = 3
};


/*
  Define the Host Adapter Status Codes.  The MultiMaster Firmware does not
  return status code 0x0C; it uses 0x12 for both overruns and underruns.
*/

enum blogic_adapter_status {
	BLOGIC_CMD_CMPLT_NORMAL = 0x00,
	BLOGIC_LINK_CMD_CMPLT = 0x0A,
	BLOGIC_LINK_CMD_CMPLT_FLAG = 0x0B,
	BLOGIC_DATA_UNDERRUN = 0x0C,
	BLOGIC_SELECT_TIMEOUT = 0x11,
	BLOGIC_DATA_OVERRUN = 0x12,
	BLOGIC_NOEXPECT_BUSFREE = 0x13,
	BLOGIC_INVALID_BUSPHASE = 0x14,
	BLOGIC_INVALID_OUTBOX_CODE = 0x15,
	BLOGIC_INVALID_CMD_CODE = 0x16,
	BLOGIC_LINKCCB_BADLUN = 0x17,
	BLOGIC_BAD_CMD_PARAM = 0x1A,
	BLOGIC_AUTOREQSENSE_FAIL = 0x1B,
	BLOGIC_TAGQUEUE_REJECT = 0x1C,
	BLOGIC_BAD_MSG_RCVD = 0x1D,
	BLOGIC_HW_FAIL = 0x20,
	BLOGIC_NORESPONSE_TO_ATN = 0x21,
	BLOGIC_HW_RESET = 0x22,
	BLOGIC_RST_FROM_OTHERDEV = 0x23,
	BLOGIC_BAD_RECONNECT = 0x24,
	BLOGIC_HW_BDR = 0x25,
	BLOGIC_ABRT_QUEUE = 0x26,
	BLOGIC_ADAPTER_SW_ERROR = 0x27,
	BLOGIC_HW_TIMEOUT = 0x30,
	BLOGIC_PARITY_ERR = 0x34
} PACKED;


/*
  Define the SCSI Target Device Status Codes.
*/

enum blogic_tgt_status {
	BLOGIC_OP_GOOD = 0x00,
	BLOGIC_CHECKCONDITION = 0x02,
	BLOGIC_DEVBUSY = 0x08
} PACKED;

/*
  Define the Queue Tag Codes.
*/

enum blogic_queuetag {
	BLOGIC_SIMPLETAG = 0,
	BLOGIC_HEADTAG = 1,
	BLOGIC_ORDEREDTAG = 2,
	BLOGIC_RSVDTAG = 3
};

/*
  Define the SCSI Command Descriptor Block (CDB).
*/

#define BLOGIC_CDB_MAXLEN			12


/*
  Define the Scatter/Gather Segment structure required by the MultiMaster
  Firmware Interface and the FlashPoint SCCB Manager.
*/

struct blogic_sg_seg {
	u32 segbytes;	/* Bytes 0-3 */
	u32 segdata;	/* Bytes 4-7 */
};

/*
  Define the Driver CCB Status Codes.
*/

enum blogic_ccb_status {
	BLOGIC_CCB_FREE = 0,
	BLOGIC_CCB_ACTIVE = 1,
	BLOGIC_CCB_COMPLETE = 2,
	BLOGIC_CCB_RESET = 3
} PACKED;


/*
  Define the 32 Bit Mode Command Control Block (CCB) structure.  The first 40
  bytes are defined by and common to both the MultiMaster Firmware and the
  FlashPoint SCCB Manager.  The next 60 bytes are defined by the FlashPoint
  SCCB Manager.  The remaining components are defined by the Linux BusLogic
  Driver.  Extended LUN Format CCBs differ from Legacy LUN Format 32 Bit Mode
  CCBs only in having the TagEnable and QueueTag fields moved from byte 17 to
  byte 1, and the Logical Unit field in byte 17 expanded to 6 bits.  In theory,
  Extended LUN Format CCBs can support up to 64 Logical Units, but in practice
  many devices will respond improperly to Logical Units between 32 and 63, and
  the SCSI-2 specification defines Bit 5 as LUNTAR.  Extended LUN Format CCBs
  are used by recent versions of the MultiMaster Firmware, as well as by the
  FlashPoint SCCB Manager; the FlashPoint SCCB Manager only supports 32 Logical
  Units.  Since 64 Logical Units are unlikely to be needed in practice, and
  since they are problematic for the above reasons, and since limiting them to
  5 bits simplifies the CCB structure definition, this driver only supports
  32 Logical Units per Target Device.
*/

struct blogic_ccb {
	/*
	   MultiMaster Firmware and FlashPoint SCCB Manager Common Portion.
	 */
	enum blogic_ccb_opcode opcode;			/* Byte 0 */
	unsigned char:3;				/* Byte 1 Bits 0-2 */
	enum blogic_datadir datadir:2;			/* Byte 1 Bits 3-4 */
	bool tag_enable:1;				/* Byte 1 Bit 5 */
	enum blogic_queuetag queuetag:2;		/* Byte 1 Bits 6-7 */
	unsigned char cdblen;				/* Byte 2 */
	unsigned char sense_datalen;			/* Byte 3 */
	u32 datalen;					/* Bytes 4-7 */
<<<<<<< HEAD
	void *data;					/* Bytes 8-11 */
=======
	u32 data;					/* Bytes 8-11 */
>>>>>>> upstream/android-13
	unsigned char:8;				/* Byte 12 */
	unsigned char:8;				/* Byte 13 */
	enum blogic_adapter_status adapter_status;	/* Byte 14 */
	enum blogic_tgt_status tgt_status;		/* Byte 15 */
	unsigned char tgt_id;				/* Byte 16 */
	unsigned char lun:5;				/* Byte 17 Bits 0-4 */
	bool legacytag_enable:1;			/* Byte 17 Bit 5 */
	enum blogic_queuetag legacy_tag:2;		/* Byte 17 Bits 6-7 */
	unsigned char cdb[BLOGIC_CDB_MAXLEN];		/* Bytes 18-29 */
	unsigned char:8;				/* Byte 30 */
	unsigned char:8;				/* Byte 31 */
	u32 rsvd_int;					/* Bytes 32-35 */
	u32 sensedata;					/* Bytes 36-39 */
	/*
	   FlashPoint SCCB Manager Defined Portion.
	 */
	void (*callback) (struct blogic_ccb *);		/* Bytes 40-43 */
	u32 base_addr;					/* Bytes 44-47 */
	enum blogic_cmplt_code comp_code;		/* Byte 48 */
#ifdef CONFIG_SCSI_FLASHPOINT
	unsigned char:8;				/* Byte 49 */
	u16 os_flags;					/* Bytes 50-51 */
	unsigned char private[24];			/* Bytes 52-99 */
	void *rsvd1;
	void *rsvd2;
	unsigned char private2[16];
#endif
	/*
	   BusLogic Linux Driver Defined Portion.
	 */
	dma_addr_t allocgrp_head;
	unsigned int allocgrp_size;
	u32 dma_handle;
	enum blogic_ccb_status status;
	unsigned long serial;
	struct scsi_cmnd *command;
	struct blogic_adapter *adapter;
	struct blogic_ccb *next;
	struct blogic_ccb *next_all;
	struct blogic_sg_seg sglist[BLOGIC_SG_LIMIT];
};

/*
  Define the 32 Bit Mode Outgoing Mailbox structure.
*/

struct blogic_outbox {
	u32 ccb;			/* Bytes 0-3 */
	u32:24;				/* Bytes 4-6 */
	enum blogic_action action;	/* Byte 7 */
};

/*
  Define the 32 Bit Mode Incoming Mailbox structure.
*/

struct blogic_inbox {
	u32 ccb;					/* Bytes 0-3 */
	enum blogic_adapter_status adapter_status;	/* Byte 4 */
	enum blogic_tgt_status tgt_status;		/* Byte 5 */
	unsigned char:8;				/* Byte 6 */
	enum blogic_cmplt_code comp_code;		/* Byte 7 */
};


/*
  Define the BusLogic Driver Options structure.
*/

struct blogic_drvr_options {
	unsigned short tagq_ok;
	unsigned short tagq_ok_mask;
	unsigned short bus_settle_time;
	unsigned short stop_tgt_inquiry;
	unsigned char common_qdepth;
	unsigned char qdepth[BLOGIC_MAXDEV];
};

/*
  Define the Host Adapter Target Flags structure.
*/

struct blogic_tgt_flags {
	bool tgt_exists:1;
	bool tagq_ok:1;
	bool wide_ok:1;
	bool tagq_active:1;
	bool wide_active:1;
	bool cmd_good:1;
	bool tgt_info_in:1;
};

/*
  Define the Host Adapter Target Statistics structure.
*/

#define BLOGIC_SZ_BUCKETS			10

struct blogic_tgt_stats {
	unsigned int cmds_tried;
	unsigned int cmds_complete;
	unsigned int read_cmds;
	unsigned int write_cmds;
	struct blogic_byte_count bytesread;
	struct blogic_byte_count byteswritten;
	unsigned int read_sz_buckets[BLOGIC_SZ_BUCKETS];
	unsigned int write_sz_buckets[BLOGIC_SZ_BUCKETS];
	unsigned short aborts_request;
	unsigned short aborts_tried;
	unsigned short aborts_done;
	unsigned short bdr_request;
	unsigned short bdr_tried;
	unsigned short bdr_done;
	unsigned short adapter_reset_req;
	unsigned short adapter_reset_attempt;
	unsigned short adapter_reset_done;
};

/*
  Define the FlashPoint Card Handle data type.
*/

#define FPOINT_BADCARD_HANDLE		0xFFFFFFFFL


/*
  Define the FlashPoint Information structure.  This structure is defined
  by the FlashPoint SCCB Manager.
*/

struct fpoint_info {
	u32 base_addr;				/* Bytes 0-3 */
	bool present;				/* Byte 4 */
	unsigned char irq_ch;			/* Byte 5 */
	unsigned char scsi_id;			/* Byte 6 */
	unsigned char scsi_lun;			/* Byte 7 */
	u16 fw_rev;				/* Bytes 8-9 */
	u16 sync_ok;				/* Bytes 10-11 */
	u16 fast_ok;				/* Bytes 12-13 */
	u16 ultra_ok;				/* Bytes 14-15 */
	u16 discon_ok;				/* Bytes 16-17 */
	u16 wide_ok;				/* Bytes 18-19 */
	bool parity:1;				/* Byte 20 Bit 0 */
	bool wide:1;				/* Byte 20 Bit 1 */
	bool softreset:1;			/* Byte 20 Bit 2 */
	bool ext_trans_enable:1;		/* Byte 20 Bit 3 */
	bool low_term:1;			/* Byte 20 Bit 4 */
	bool high_term:1;			/* Byte 20 Bit 5 */
	bool report_underrun:1;			/* Byte 20 Bit 6 */
	bool scam_enabled:1;			/* Byte 20 Bit 7 */
	bool scam_lev2:1;			/* Byte 21 Bit 0 */
	unsigned char:7;			/* Byte 21 Bits 1-7 */
	unsigned char family;			/* Byte 22 */
	unsigned char bus_type;			/* Byte 23 */
	unsigned char model[3];			/* Bytes 24-26 */
	unsigned char relative_cardnum;		/* Byte 27 */
	unsigned char rsvd[4];			/* Bytes 28-31 */
	u32 os_rsvd;				/* Bytes 32-35 */
	unsigned char translation_info[4];	/* Bytes 36-39 */
	u32 rsvd2[5];				/* Bytes 40-59 */
	u32 sec_range;				/* Bytes 60-63 */
};

/*
  Define the BusLogic Driver Host Adapter structure.
*/

struct blogic_adapter {
	struct Scsi_Host *scsi_host;
	struct pci_dev *pci_device;
	enum blogic_adapter_type adapter_type;
	enum blogic_adapter_bus_type adapter_bus_type;
	unsigned long io_addr;
	unsigned long pci_addr;
	unsigned short addr_count;
	unsigned char host_no;
	unsigned char model[9];
	unsigned char fw_ver[6];
	unsigned char full_model[18];
	unsigned char bus;
	unsigned char dev;
	unsigned char irq_ch;
<<<<<<< HEAD
	unsigned char dma_ch;
	unsigned char scsi_id;
	bool irq_acquired:1;
	bool dma_chan_acquired:1;
=======
	unsigned char scsi_id;
	bool irq_acquired:1;
>>>>>>> upstream/android-13
	bool ext_trans_enable:1;
	bool parity:1;
	bool reset_enabled:1;
	bool level_int:1;
	bool wide:1;
	bool differential:1;
	bool scam:1;
	bool ultra:1;
	bool ext_lun:1;
	bool terminfo_valid:1;
	bool low_term:1;
	bool high_term:1;
<<<<<<< HEAD
	bool need_bouncebuf:1;
=======
>>>>>>> upstream/android-13
	bool strict_rr:1;
	bool scam_enabled:1;
	bool scam_lev2:1;
	bool adapter_initd:1;
	bool adapter_extreset:1;
	bool adapter_intern_err:1;
	bool processing_ccbs;
	volatile bool adapter_cmd_complete;
	unsigned short adapter_sglimit;
	unsigned short drvr_sglimit;
	unsigned short maxdev;
	unsigned short maxlun;
	unsigned short mbox_count;
	unsigned short initccbs;
	unsigned short inc_ccbs;
	unsigned short alloc_ccbs;
	unsigned short drvr_qdepth;
	unsigned short adapter_qdepth;
	unsigned short untag_qdepth;
	unsigned short common_qdepth;
	unsigned short bus_settle_time;
	unsigned short sync_ok;
	unsigned short fast_ok;
	unsigned short ultra_ok;
	unsigned short wide_ok;
	unsigned short discon_ok;
	unsigned short tagq_ok;
	unsigned short ext_resets;
	unsigned short adapter_intern_errors;
	unsigned short tgt_count;
	unsigned short msgbuflen;
	u32 bios_addr;
	struct blogic_drvr_options *drvr_opts;
	struct fpoint_info fpinfo;
	void *cardhandle;
	struct list_head host_list;
	struct blogic_ccb *all_ccbs;
	struct blogic_ccb *free_ccbs;
	struct blogic_ccb *firstccb;
	struct blogic_ccb *lastccb;
	struct blogic_ccb *bdr_pend[BLOGIC_MAXDEV];
	struct blogic_tgt_flags tgt_flags[BLOGIC_MAXDEV];
	unsigned char qdepth[BLOGIC_MAXDEV];
	unsigned char sync_period[BLOGIC_MAXDEV];
	unsigned char sync_offset[BLOGIC_MAXDEV];
	unsigned char active_cmds[BLOGIC_MAXDEV];
	unsigned int cmds_since_rst[BLOGIC_MAXDEV];
	unsigned long last_seqpoint[BLOGIC_MAXDEV];
	unsigned long last_resettried[BLOGIC_MAXDEV];
	unsigned long last_resetdone[BLOGIC_MAXDEV];
	struct blogic_outbox *first_outbox;
	struct blogic_outbox *last_outbox;
	struct blogic_outbox *next_outbox;
	struct blogic_inbox *first_inbox;
	struct blogic_inbox *last_inbox;
	struct blogic_inbox *next_inbox;
	struct blogic_tgt_stats tgt_stats[BLOGIC_MAXDEV];
	unsigned char *mbox_space;
	dma_addr_t mbox_space_handle;
	unsigned int mbox_sz;
	unsigned long ccb_offset;
	char msgbuf[BLOGIC_MSGBUF_SIZE];
};

/*
  Define a structure for the BIOS Disk Parameters.
*/

struct bios_diskparam {
	int heads;
	int sectors;
	int cylinders;
};

/*
  Define a structure for the SCSI Inquiry command results.
*/

struct scsi_inquiry {
	unsigned char devtype:5;	/* Byte 0 Bits 0-4 */
	unsigned char dev_qual:3;	/* Byte 0 Bits 5-7 */
	unsigned char dev_modifier:7;	/* Byte 1 Bits 0-6 */
	bool rmb:1;			/* Byte 1 Bit 7 */
	unsigned char ansi_ver:3;	/* Byte 2 Bits 0-2 */
	unsigned char ecma_ver:3;	/* Byte 2 Bits 3-5 */
	unsigned char iso_ver:2;	/* Byte 2 Bits 6-7 */
	unsigned char resp_fmt:4;	/* Byte 3 Bits 0-3 */
	unsigned char:2;		/* Byte 3 Bits 4-5 */
	bool TrmIOP:1;			/* Byte 3 Bit 6 */
	bool AENC:1;			/* Byte 3 Bit 7 */
	unsigned char addl_len;		/* Byte 4 */
	unsigned char:8;		/* Byte 5 */
	unsigned char:8;		/* Byte 6 */
	bool SftRe:1;			/* Byte 7 Bit 0 */
	bool CmdQue:1;			/* Byte 7 Bit 1 */
	bool:1;				/* Byte 7 Bit 2 */
	bool linked:1;			/* Byte 7 Bit 3 */
	bool sync:1;			/* Byte 7 Bit 4 */
	bool WBus16:1;			/* Byte 7 Bit 5 */
	bool WBus32:1;			/* Byte 7 Bit 6 */
	bool RelAdr:1;			/* Byte 7 Bit 7 */
	unsigned char vendor[8];	/* Bytes 8-15 */
	unsigned char product[16];	/* Bytes 16-31 */
	unsigned char product_rev[4];	/* Bytes 32-35 */
};


/*
  Define functions to provide an abstraction for reading and writing the
  Host Adapter I/O Registers.
*/

static inline void blogic_busreset(struct blogic_adapter *adapter)
{
	union blogic_cntrl_reg cr;
	cr.all = 0;
	cr.cr.bus_reset = true;
	outb(cr.all, adapter->io_addr + BLOGIC_CNTRL_REG);
}

static inline void blogic_intreset(struct blogic_adapter *adapter)
{
	union blogic_cntrl_reg cr;
	cr.all = 0;
	cr.cr.int_reset = true;
	outb(cr.all, adapter->io_addr + BLOGIC_CNTRL_REG);
}

static inline void blogic_softreset(struct blogic_adapter *adapter)
{
	union blogic_cntrl_reg cr;
	cr.all = 0;
	cr.cr.soft_reset = true;
	outb(cr.all, adapter->io_addr + BLOGIC_CNTRL_REG);
}

static inline void blogic_hardreset(struct blogic_adapter *adapter)
{
	union blogic_cntrl_reg cr;
	cr.all = 0;
	cr.cr.hard_reset = true;
	outb(cr.all, adapter->io_addr + BLOGIC_CNTRL_REG);
}

static inline unsigned char blogic_rdstatus(struct blogic_adapter *adapter)
{
	return inb(adapter->io_addr + BLOGIC_STATUS_REG);
}

static inline void blogic_setcmdparam(struct blogic_adapter *adapter,
					unsigned char value)
{
	outb(value, adapter->io_addr + BLOGIC_CMD_PARM_REG);
}

static inline unsigned char blogic_rddatain(struct blogic_adapter *adapter)
{
	return inb(adapter->io_addr + BLOGIC_DATAIN_REG);
}

static inline unsigned char blogic_rdint(struct blogic_adapter *adapter)
{
	return inb(adapter->io_addr + BLOGIC_INT_REG);
}

static inline unsigned char blogic_rdgeom(struct blogic_adapter *adapter)
{
	return inb(adapter->io_addr + BLOGIC_GEOMETRY_REG);
}

/*
  blogic_execmbox issues an Execute Mailbox Command, which
  notifies the Host Adapter that an entry has been made in an Outgoing
  Mailbox.
*/

static inline void blogic_execmbox(struct blogic_adapter *adapter)
{
	blogic_setcmdparam(adapter, BLOGIC_EXEC_MBOX_CMD);
}

/*
  blogic_delay waits for Seconds to elapse.
*/

static inline void blogic_delay(int seconds)
{
	mdelay(1000 * seconds);
}

/*
  virt_to_32bit_virt maps between Kernel Virtual Addresses and
  32 bit Kernel Virtual Addresses.  This avoids compilation warnings
  on 64 bit architectures.
*/

static inline u32 virt_to_32bit_virt(void *virt_addr)
{
	return (u32) (unsigned long) virt_addr;
}

/*
  blogic_inc_count increments counter by 1, stopping at
  65535 rather than wrapping around to 0.
*/

static inline void blogic_inc_count(unsigned short *count)
{
	if (*count < 65535)
		(*count)++;
}

/*
  blogic_addcount increments Byte Counter by Amount.
*/

static inline void blogic_addcount(struct blogic_byte_count *bytecount,
					unsigned int amount)
{
	bytecount->units += amount;
	if (bytecount->units > 999999999) {
		bytecount->units -= 1000000000;
		bytecount->billions++;
	}
}

/*
  blogic_incszbucket increments the Bucket for Amount.
*/

static inline void blogic_incszbucket(unsigned int *cmdsz_buckets,
					unsigned int amount)
{
	int index = 0;
	if (amount < 8 * 1024) {
		if (amount < 2 * 1024)
			index = (amount < 1 * 1024 ? 0 : 1);
		else
			index = (amount < 4 * 1024 ? 2 : 3);
	} else if (amount < 128 * 1024) {
		if (amount < 32 * 1024)
			index = (amount < 16 * 1024 ? 4 : 5);
		else
			index = (amount < 64 * 1024 ? 6 : 7);
	} else
		index = (amount < 256 * 1024 ? 8 : 9);
	cmdsz_buckets[index]++;
}

/*
  Define the version number of the FlashPoint Firmware (SCCB Manager).
*/

#define FLASHPOINT_FW_VER		"5.02"

/*
  Define the possible return values from FlashPoint_HandleInterrupt.
*/

#define FPOINT_NORMAL_INT		0x00
#define FPOINT_INTERN_ERR		0xFE
#define FPOINT_EXT_RESET		0xFF

/*
  Define prototypes for the forward referenced BusLogic Driver
  Internal Functions.
*/

static const char *blogic_drvr_info(struct Scsi_Host *);
static int blogic_qcmd(struct Scsi_Host *h, struct scsi_cmnd *);
static int blogic_diskparam(struct scsi_device *, struct block_device *, sector_t, int *);
static int blogic_slaveconfig(struct scsi_device *);
static void blogic_qcompleted_ccb(struct blogic_ccb *);
static irqreturn_t blogic_inthandler(int, void *);
static int blogic_resetadapter(struct blogic_adapter *, bool hard_reset);
static void blogic_msg(enum blogic_msglevel, char *, struct blogic_adapter *, ...);
static int __init blogic_setup(char *);

#endif				/* _BUSLOGIC_H */
