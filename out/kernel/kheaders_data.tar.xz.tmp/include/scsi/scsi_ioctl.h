/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SCSI_IOCTL_H
#define _SCSI_IOCTL_H 

#define SCSI_IOCTL_SEND_COMMAND 1
#define SCSI_IOCTL_TEST_UNIT_READY 2
#define SCSI_IOCTL_BENCHMARK_COMMAND 3
#define SCSI_IOCTL_SYNC 4			
#define SCSI_IOCTL_START_UNIT 5
#define SCSI_IOCTL_STOP_UNIT 6

#define SCSI_IOCTL_DOORLOCK 0x5380		
#define SCSI_IOCTL_DOORUNLOCK 0x5381		

#define	SCSI_REMOVAL_PREVENT	1
#define	SCSI_REMOVAL_ALLOW	0

#ifdef __KERNEL__

struct scsi_device;



typedef struct scsi_ioctl_command {
	unsigned int inlen;
	unsigned int outlen;
	unsigned char data[0];
} Scsi_Ioctl_Command;

typedef struct scsi_idlun {
	__u32 dev_id;
	__u32 host_unique_id;
} Scsi_Idlun;


typedef struct scsi_fctargaddress {
	__u32 host_port_id;
	unsigned char host_wwn[8]; // include NULL term.
} Scsi_FCTargAddress;

int scsi_ioctl_block_when_processing_errors(struct scsi_device *sdev,
		int cmd, bool ndelay);
extern int scsi_ioctl(struct scsi_device *, int, void __user *);

#endif 
#endif 
