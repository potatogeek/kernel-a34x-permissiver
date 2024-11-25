/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  Copyright IBM Corp. 2006
 *  Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 */
#ifndef __S390_STP_H
#define __S390_STP_H

<<<<<<< HEAD
=======
#include <linux/compiler.h>

>>>>>>> upstream/android-13
/* notifier for syncs */
extern struct atomic_notifier_head s390_epoch_delta_notifier;

/* STP interruption parameter */
struct stp_irq_parm {
<<<<<<< HEAD
	unsigned int _pad0	: 14;
	unsigned int tsc	: 1;	/* Timing status change */
	unsigned int lac	: 1;	/* Link availability change */
	unsigned int tcpc	: 1;	/* Time control parameter change */
	unsigned int _pad2	: 15;
} __attribute__ ((packed));
=======
	u32		: 14;
	u32 tsc		:  1;	/* Timing status change */
	u32 lac		:  1;	/* Link availability change */
	u32 tcpc	:  1;	/* Time control parameter change */
	u32		: 15;
} __packed;
>>>>>>> upstream/android-13

#define STP_OP_SYNC	1
#define STP_OP_CTRL	3

struct stp_sstpi {
<<<<<<< HEAD
	unsigned int rsvd0;
	unsigned int rsvd1 : 8;
	unsigned int stratum : 8;
	unsigned int vbits : 16;
	unsigned int leaps : 16;
	unsigned int tmd : 4;
	unsigned int ctn : 4;
	unsigned int rsvd2 : 3;
	unsigned int c : 1;
	unsigned int tst : 4;
	unsigned int tzo : 16;
	unsigned int dsto : 16;
	unsigned int ctrl : 16;
	unsigned int rsvd3 : 16;
	unsigned int tto;
	unsigned int rsvd4;
	unsigned int ctnid[3];
	unsigned int rsvd5;
	unsigned int todoff[4];
	unsigned int rsvd6[48];
} __attribute__ ((packed));
=======
	u32		: 32;
	u32 tu		:  1;
	u32 lu		:  1;
	u32		:  6;
	u32 stratum	:  8;
	u32 vbits	: 16;
	u32 leaps	: 16;
	u32 tmd		:  4;
	u32 ctn		:  4;
	u32		:  3;
	u32 c		:  1;
	u32 tst		:  4;
	u32 tzo		: 16;
	u32 dsto	: 16;
	u32 ctrl	: 16;
	u32		: 16;
	u32 tto;
	u32		: 32;
	u32 ctnid[3];
	u32		: 32;
	u32 todoff[4];
	u32 rsvd[48];
} __packed;

struct stp_tzib {
	u32 tzan	: 16;
	u32		: 16;
	u32 tzo		: 16;
	u32 dsto	: 16;
	u32 stn;
	u32 dstn;
	u64 dst_on_alg;
	u64 dst_off_alg;
} __packed;

struct stp_tcpib {
	u32 atcode	: 4;
	u32 ntcode	: 4;
	u32 d		: 1;
	u32		: 23;
	s32 tto;
	struct stp_tzib atzib;
	struct stp_tzib ntzib;
	s32 adst_offset : 16;
	s32 ndst_offset : 16;
	u32 rsvd1;
	u64 ntzib_update;
	u64 ndsto_update;
} __packed;

struct stp_lsoib {
	u32 p		: 1;
	u32		: 31;
	s32 also	: 16;
	s32 nlso	: 16;
	u64 nlsout;
} __packed;

struct stp_stzi {
	u32 rsvd0[3];
	u64 data_ts;
	u32 rsvd1[22];
	struct stp_tcpib tcpib;
	struct stp_lsoib lsoib;
} __packed;
>>>>>>> upstream/android-13

/* Functions needed by the machine check handler */
int stp_sync_check(void);
int stp_island_check(void);
void stp_queue_work(void);

#endif /* __S390_STP_H */
