/* SPDX-License-Identifier: (GPL-2.0+ OR BSD-3-Clause) */

#ifndef __FSL_DPAA2_GLOBAL_H
#define __FSL_DPAA2_GLOBAL_H

#include <linux/types.h>
#include <linux/cpumask.h>
#include "dpaa2-fd.h"

struct dpaa2_dq {
	union {
		struct common {
			u8 verb;
			u8 reserved[63];
		} common;
		struct dq {
			u8 verb;
			u8 stat;
			__le16 seqnum;
			__le16 oprid;
			u8 reserved;
			u8 tok;
			__le32 fqid;
			u32 reserved2;
			__le32 fq_byte_cnt;
			__le32 fq_frm_cnt;
			__le64 fqd_ctx;
			u8 fd[32];
		} dq;
		struct scn {
			u8 verb;
			u8 stat;
			u8 state;
			u8 reserved;
			__le32 rid_tok;
			__le64 ctx;
		} scn;
	};
};



#define DPAA2_DQ_STAT_FQEMPTY       0x80

#define DPAA2_DQ_STAT_HELDACTIVE    0x40

#define DPAA2_DQ_STAT_FORCEELIGIBLE 0x20

#define DPAA2_DQ_STAT_VALIDFRAME    0x10

#define DPAA2_DQ_STAT_ODPVALID      0x04

#define DPAA2_DQ_STAT_VOLATILE      0x02

#define DPAA2_DQ_STAT_EXPIRED       0x01

#define DQ_FQID_MASK		0x00FFFFFF
#define DQ_FRAME_COUNT_MASK	0x00FFFFFF


static inline u32 dpaa2_dq_flags(const struct dpaa2_dq *dq)
{
	return dq->dq.stat;
}


static inline int dpaa2_dq_is_pull(const struct dpaa2_dq *dq)
{
	return (int)(dpaa2_dq_flags(dq) & DPAA2_DQ_STAT_VOLATILE);
}


static inline bool dpaa2_dq_is_pull_complete(const struct dpaa2_dq *dq)
{
	return !!(dpaa2_dq_flags(dq) & DPAA2_DQ_STAT_EXPIRED);
}


static inline u16 dpaa2_dq_seqnum(const struct dpaa2_dq *dq)
{
	return le16_to_cpu(dq->dq.seqnum);
}


static inline u16 dpaa2_dq_odpid(const struct dpaa2_dq *dq)
{
	return le16_to_cpu(dq->dq.oprid);
}


static inline u32 dpaa2_dq_fqid(const struct dpaa2_dq *dq)
{
	return le32_to_cpu(dq->dq.fqid) & DQ_FQID_MASK;
}


static inline u32 dpaa2_dq_byte_count(const struct dpaa2_dq *dq)
{
	return le32_to_cpu(dq->dq.fq_byte_cnt);
}


static inline u32 dpaa2_dq_frame_count(const struct dpaa2_dq *dq)
{
	return le32_to_cpu(dq->dq.fq_frm_cnt) & DQ_FRAME_COUNT_MASK;
}


static inline u64 dpaa2_dq_fqd_ctx(const struct dpaa2_dq *dq)
{
	return le64_to_cpu(dq->dq.fqd_ctx);
}


static inline const struct dpaa2_fd *dpaa2_dq_fd(const struct dpaa2_dq *dq)
{
	return (const struct dpaa2_fd *)&dq->dq.fd[0];
}

#endif 
