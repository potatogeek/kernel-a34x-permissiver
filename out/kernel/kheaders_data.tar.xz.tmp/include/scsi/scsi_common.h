/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _SCSI_COMMON_H_
#define _SCSI_COMMON_H_

#include <linux/types.h>
#include <scsi/scsi_proto.h>

static inline unsigned
scsi_varlen_cdb_length(const void *hdr)
{
	return ((struct scsi_varlen_cdb_hdr *)hdr)->additional_cdb_length + 8;
}

extern const unsigned char scsi_command_size_tbl[8];
#define COMMAND_SIZE(opcode) scsi_command_size_tbl[((opcode) >> 5) & 7]

static inline unsigned
scsi_command_size(const unsigned char *cmnd)
{
	return (cmnd[0] == VARIABLE_LENGTH_CMD) ?
		scsi_varlen_cdb_length(cmnd) : COMMAND_SIZE(cmnd[0]);
}

static inline unsigned char
scsi_command_control(const unsigned char *cmnd)
{
	return (cmnd[0] == VARIABLE_LENGTH_CMD) ?
		cmnd[1] : cmnd[COMMAND_SIZE(cmnd[0]) - 1];
}


extern const char *scsi_device_type(unsigned type);

extern void int_to_scsilun(u64, struct scsi_lun *);
extern u64 scsilun_to_int(struct scsi_lun *);


struct scsi_sense_hdr {		
	u8 response_code;	
	u8 sense_key;
	u8 asc;
	u8 ascq;
	u8 byte4;
	u8 byte5;
	u8 byte6;
	u8 additional_length;	
};

static inline bool scsi_sense_valid(const struct scsi_sense_hdr *sshdr)
{
	if (!sshdr)
		return false;

	return (sshdr->response_code & 0x70) == 0x70;
}

extern bool scsi_normalize_sense(const u8 *sense_buffer, int sb_len,
				 struct scsi_sense_hdr *sshdr);

extern void scsi_build_sense_buffer(int desc, u8 *buf, u8 key, u8 asc, u8 ascq);
int scsi_set_sense_information(u8 *buf, int buf_len, u64 info);
int scsi_set_sense_field_pointer(u8 *buf, int buf_len, u16 fp, u8 bp, bool cd);
extern const u8 * scsi_sense_desc_find(const u8 * sense_buffer, int sb_len,
				       int desc_type);

#endif 
