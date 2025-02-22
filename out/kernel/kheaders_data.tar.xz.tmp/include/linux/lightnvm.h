/* SPDX-License-Identifier: GPL-2.0 */
#ifndef NVM_H
#define NVM_H

#include <linux/blkdev.h>
#include <linux/types.h>
#include <uapi/linux/lightnvm.h>

enum {
	NVM_IO_OK = 0,
	NVM_IO_REQUEUE = 1,
	NVM_IO_DONE = 2,
	NVM_IO_ERR = 3,

	NVM_IOTYPE_NONE = 0,
	NVM_IOTYPE_GC = 1,
};


#define NVM_GEN_CH_BITS  (8)
#define NVM_GEN_LUN_BITS (8)
#define NVM_GEN_BLK_BITS (16)
#define NVM_GEN_RESERVED (32)


#define NVM_12_PG_BITS  (16)
#define NVM_12_PL_BITS  (4)
#define NVM_12_SEC_BITS (4)
#define NVM_12_RESERVED (8)


#define NVM_20_SEC_BITS (24)
#define NVM_20_RESERVED (8)

enum {
	NVM_OCSSD_SPEC_12 = 12,
	NVM_OCSSD_SPEC_20 = 20,
};

struct ppa_addr {
	
	union {
		
		struct {
			u64 ch		: NVM_GEN_CH_BITS;
			u64 lun		: NVM_GEN_LUN_BITS;
			u64 blk		: NVM_GEN_BLK_BITS;
			u64 reserved	: NVM_GEN_RESERVED;
		} a;

		
		struct {
			u64 ch		: NVM_GEN_CH_BITS;
			u64 lun		: NVM_GEN_LUN_BITS;
			u64 blk		: NVM_GEN_BLK_BITS;
			u64 pg		: NVM_12_PG_BITS;
			u64 pl		: NVM_12_PL_BITS;
			u64 sec		: NVM_12_SEC_BITS;
			u64 reserved	: NVM_12_RESERVED;
		} g;

		
		struct {
			u64 grp		: NVM_GEN_CH_BITS;
			u64 pu		: NVM_GEN_LUN_BITS;
			u64 chk		: NVM_GEN_BLK_BITS;
			u64 sec		: NVM_20_SEC_BITS;
			u64 reserved	: NVM_20_RESERVED;
		} m;

		struct {
			u64 line	: 63;
			u64 is_cached	: 1;
		} c;

		u64 ppa;
	};
};

struct nvm_rq;
struct nvm_id;
struct nvm_dev;
struct nvm_tgt_dev;
struct nvm_chk_meta;

typedef int (nvm_id_fn)(struct nvm_dev *);
typedef int (nvm_op_bb_tbl_fn)(struct nvm_dev *, struct ppa_addr, u8 *);
typedef int (nvm_op_set_bb_fn)(struct nvm_dev *, struct ppa_addr *, int, int);
typedef int (nvm_get_chk_meta_fn)(struct nvm_dev *, struct nvm_chk_meta *,
								sector_t, int);
typedef int (nvm_submit_io_fn)(struct nvm_dev *, struct nvm_rq *);
typedef int (nvm_submit_io_sync_fn)(struct nvm_dev *, struct nvm_rq *);
typedef void *(nvm_create_dma_pool_fn)(struct nvm_dev *, char *);
typedef void (nvm_destroy_dma_pool_fn)(void *);
typedef void *(nvm_dev_dma_alloc_fn)(struct nvm_dev *, void *, gfp_t,
								dma_addr_t *);
typedef void (nvm_dev_dma_free_fn)(void *, void*, dma_addr_t);

struct nvm_dev_ops {
	nvm_id_fn		*identity;
	nvm_op_bb_tbl_fn	*get_bb_tbl;
	nvm_op_set_bb_fn	*set_bb_tbl;

	nvm_get_chk_meta_fn	*get_chk_meta;

	nvm_submit_io_fn	*submit_io;
	nvm_submit_io_sync_fn	*submit_io_sync;

	nvm_create_dma_pool_fn	*create_dma_pool;
	nvm_destroy_dma_pool_fn	*destroy_dma_pool;
	nvm_dev_dma_alloc_fn	*dev_dma_alloc;
	nvm_dev_dma_free_fn	*dev_dma_free;
};

#ifdef CONFIG_NVM

#include <linux/blkdev.h>
#include <linux/file.h>
#include <linux/dmapool.h>
#include <uapi/linux/lightnvm.h>

enum {
	
	NVM_RSP_L2P	= 1 << 0,
	NVM_RSP_ECC	= 1 << 1,

	
	NVM_ADDRMODE_LINEAR	= 0,
	NVM_ADDRMODE_CHANNEL	= 1,

	
	NVM_PLANE_SINGLE	= 1,
	NVM_PLANE_DOUBLE	= 2,
	NVM_PLANE_QUAD		= 4,

	
	NVM_RSP_SUCCESS		= 0x0,
	NVM_RSP_NOT_CHANGEABLE	= 0x1,
	NVM_RSP_ERR_FAILWRITE	= 0x40ff,
	NVM_RSP_ERR_EMPTYPAGE	= 0x42ff,
	NVM_RSP_ERR_FAILECC	= 0x4281,
	NVM_RSP_ERR_FAILCRC	= 0x4004,
	NVM_RSP_WARN_HIGHECC	= 0x4700,

	
	NVM_OP_PWRITE		= 0x91,
	NVM_OP_PREAD		= 0x92,
	NVM_OP_ERASE		= 0x90,

	
	NVM_IO_SNGL_ACCESS	= 0x0,
	NVM_IO_DUAL_ACCESS	= 0x1,
	NVM_IO_QUAD_ACCESS	= 0x2,

	
	NVM_IO_SUSPEND		= 0x80,
	NVM_IO_SLC_MODE		= 0x100,
	NVM_IO_SCRAMBLE_ENABLE	= 0x200,

	
	NVM_BLK_T_FREE		= 0x0,
	NVM_BLK_T_BAD		= 0x1,
	NVM_BLK_T_GRWN_BAD	= 0x2,
	NVM_BLK_T_DEV		= 0x4,
	NVM_BLK_T_HOST		= 0x8,

	
	NVM_ID_CAP_SLC		= 0x1,
	NVM_ID_CAP_CMD_SUSPEND	= 0x2,
	NVM_ID_CAP_SCRAMBLE	= 0x4,
	NVM_ID_CAP_ENCRYPT	= 0x8,

	
	NVM_ID_FMTYPE_SLC	= 0,
	NVM_ID_FMTYPE_MLC	= 1,

	
	NVM_ID_DCAP_BBLKMGMT	= 0x1,
	NVM_UD_DCAP_ECC		= 0x2,
};

struct nvm_id_lp_mlc {
	u16	num_pairs;
	u8	pairs[886];
};

struct nvm_id_lp_tbl {
	__u8	id[8];
	struct nvm_id_lp_mlc mlc;
};

struct nvm_addrf_12 {
	u8	ch_len;
	u8	lun_len;
	u8	blk_len;
	u8	pg_len;
	u8	pln_len;
	u8	sec_len;

	u8	ch_offset;
	u8	lun_offset;
	u8	blk_offset;
	u8	pg_offset;
	u8	pln_offset;
	u8	sec_offset;

	u64	ch_mask;
	u64	lun_mask;
	u64	blk_mask;
	u64	pg_mask;
	u64	pln_mask;
	u64	sec_mask;
};

struct nvm_addrf {
	u8	ch_len;
	u8	lun_len;
	u8	chk_len;
	u8	sec_len;
	u8	rsv_len[2];

	u8	ch_offset;
	u8	lun_offset;
	u8	chk_offset;
	u8	sec_offset;
	u8	rsv_off[2];

	u64	ch_mask;
	u64	lun_mask;
	u64	chk_mask;
	u64	sec_mask;
	u64	rsv_mask[2];
};

enum {
	
	NVM_CHK_ST_FREE =	1 << 0,
	NVM_CHK_ST_CLOSED =	1 << 1,
	NVM_CHK_ST_OPEN =	1 << 2,
	NVM_CHK_ST_OFFLINE =	1 << 3,

	
	NVM_CHK_TP_W_SEQ =	1 << 0,
	NVM_CHK_TP_W_RAN =	1 << 1,
	NVM_CHK_TP_SZ_SPEC =	1 << 4,
};


struct nvm_chk_meta {
	u8	state;
	u8	type;
	u8	wi;
	u8	rsvd[5];
	u64	slba;
	u64	cnlb;
	u64	wp;
};

struct nvm_target {
	struct list_head list;
	struct nvm_tgt_dev *dev;
	struct nvm_tgt_type *type;
	struct gendisk *disk;
};

#define ADDR_EMPTY (~0ULL)

#define NVM_TARGET_DEFAULT_OP (101)
#define NVM_TARGET_MIN_OP (3)
#define NVM_TARGET_MAX_OP (80)

#define NVM_VERSION_MAJOR 1
#define NVM_VERSION_MINOR 0
#define NVM_VERSION_PATCH 0

#define NVM_MAX_VLBA (64) 

struct nvm_rq;
typedef void (nvm_end_io_fn)(struct nvm_rq *);

struct nvm_rq {
	struct nvm_tgt_dev *dev;

	struct bio *bio;

	union {
		struct ppa_addr ppa_addr;
		dma_addr_t dma_ppa_list;
	};

	struct ppa_addr *ppa_list;

	void *meta_list;
	dma_addr_t dma_meta_list;

	nvm_end_io_fn *end_io;

	uint8_t opcode;
	uint16_t nr_ppas;
	uint16_t flags;

	u64 ppa_status; 
	int error;

	void *private;
};

static inline struct nvm_rq *nvm_rq_from_pdu(void *pdu)
{
	return pdu - sizeof(struct nvm_rq);
}

static inline void *nvm_rq_to_pdu(struct nvm_rq *rqdata)
{
	return rqdata + 1;
}

enum {
	NVM_BLK_ST_FREE =	0x1,	
	NVM_BLK_ST_TGT =	0x2,	
	NVM_BLK_ST_BAD =	0x8,	
};


struct nvm_geo {
	
	u8	major_ver_id;
	u8	minor_ver_id;

	
	u8	version;

	
	int num_ch;
	int num_lun;		

	
	int all_luns;		
	int all_chunks;		

	int op;			

	sector_t total_secs;	

	
	u32	num_chk;	
	u32	clba;		
	u16	csecs;		
	u16	sos;		

	
	u32	ws_min;		
	u32	ws_opt;		
	u32	mw_cunits;	
	u32	maxoc;		
	u32	maxocpu;	

	
	u32	mccap;

	
	u32	trdt;		
	u32	trdm;		
	u32	tprt;		
	u32	tprm;		
	u32	tbet;		
	u32	tbem;		

	
	struct nvm_addrf addrf;

	
	u8	vmnt;
	u32	cap;
	u32	dom;

	u8	mtype;
	u8	fmtype;

	u16	cpar;
	u32	mpos;

	u8	num_pln;
	u8	pln_mode;
	u16	num_pg;
	u16	fpg_sz;
};


struct nvm_tgt_dev {
	
	struct nvm_geo geo;

	
	struct ppa_addr *luns;

	struct request_queue *q;

	struct nvm_dev *parent;
	void *map;
};

struct nvm_dev {
	struct nvm_dev_ops *ops;

	struct list_head devices;

	
	struct nvm_geo geo;

	unsigned long *lun_map;
	void *dma_pool;

	
	struct request_queue *q;
	char name[DISK_NAME_LEN];
	void *private_data;

	void *rmap;

	struct mutex mlock;
	spinlock_t lock;

	
	struct list_head area_list;
	struct list_head targets;
};

static inline struct ppa_addr generic_to_dev_addr(struct nvm_dev *dev,
						  struct ppa_addr r)
{
	struct nvm_geo *geo = &dev->geo;
	struct ppa_addr l;

	if (geo->version == NVM_OCSSD_SPEC_12) {
		struct nvm_addrf_12 *ppaf = (struct nvm_addrf_12 *)&geo->addrf;

		l.ppa = ((u64)r.g.ch) << ppaf->ch_offset;
		l.ppa |= ((u64)r.g.lun) << ppaf->lun_offset;
		l.ppa |= ((u64)r.g.blk) << ppaf->blk_offset;
		l.ppa |= ((u64)r.g.pg) << ppaf->pg_offset;
		l.ppa |= ((u64)r.g.pl) << ppaf->pln_offset;
		l.ppa |= ((u64)r.g.sec) << ppaf->sec_offset;
	} else {
		struct nvm_addrf *lbaf = &geo->addrf;

		l.ppa = ((u64)r.m.grp) << lbaf->ch_offset;
		l.ppa |= ((u64)r.m.pu) << lbaf->lun_offset;
		l.ppa |= ((u64)r.m.chk) << lbaf->chk_offset;
		l.ppa |= ((u64)r.m.sec) << lbaf->sec_offset;
	}

	return l;
}

static inline struct ppa_addr dev_to_generic_addr(struct nvm_dev *dev,
						  struct ppa_addr r)
{
	struct nvm_geo *geo = &dev->geo;
	struct ppa_addr l;

	l.ppa = 0;

	if (geo->version == NVM_OCSSD_SPEC_12) {
		struct nvm_addrf_12 *ppaf = (struct nvm_addrf_12 *)&geo->addrf;

		l.g.ch = (r.ppa & ppaf->ch_mask) >> ppaf->ch_offset;
		l.g.lun = (r.ppa & ppaf->lun_mask) >> ppaf->lun_offset;
		l.g.blk = (r.ppa & ppaf->blk_mask) >> ppaf->blk_offset;
		l.g.pg = (r.ppa & ppaf->pg_mask) >> ppaf->pg_offset;
		l.g.pl = (r.ppa & ppaf->pln_mask) >> ppaf->pln_offset;
		l.g.sec = (r.ppa & ppaf->sec_mask) >> ppaf->sec_offset;
	} else {
		struct nvm_addrf *lbaf = &geo->addrf;

		l.m.grp = (r.ppa & lbaf->ch_mask) >> lbaf->ch_offset;
		l.m.pu = (r.ppa & lbaf->lun_mask) >> lbaf->lun_offset;
		l.m.chk = (r.ppa & lbaf->chk_mask) >> lbaf->chk_offset;
		l.m.sec = (r.ppa & lbaf->sec_mask) >> lbaf->sec_offset;
	}

	return l;
}

typedef blk_qc_t (nvm_tgt_make_rq_fn)(struct request_queue *, struct bio *);
typedef sector_t (nvm_tgt_capacity_fn)(void *);
typedef void *(nvm_tgt_init_fn)(struct nvm_tgt_dev *, struct gendisk *,
				int flags);
typedef void (nvm_tgt_exit_fn)(void *, bool);
typedef int (nvm_tgt_sysfs_init_fn)(struct gendisk *);
typedef void (nvm_tgt_sysfs_exit_fn)(struct gendisk *);

struct nvm_tgt_type {
	const char *name;
	unsigned int version[3];

	
	nvm_tgt_make_rq_fn *make_rq;
	nvm_tgt_capacity_fn *capacity;

	
	nvm_tgt_init_fn *init;
	nvm_tgt_exit_fn *exit;

	
	nvm_tgt_sysfs_init_fn *sysfs_init;
	nvm_tgt_sysfs_exit_fn *sysfs_exit;

	
	struct list_head list;
	struct module *owner;
};

extern int nvm_register_tgt_type(struct nvm_tgt_type *);
extern void nvm_unregister_tgt_type(struct nvm_tgt_type *);

extern void *nvm_dev_dma_alloc(struct nvm_dev *, gfp_t, dma_addr_t *);
extern void nvm_dev_dma_free(struct nvm_dev *, void *, dma_addr_t);

extern struct nvm_dev *nvm_alloc_dev(int);
extern int nvm_register(struct nvm_dev *);
extern void nvm_unregister(struct nvm_dev *);


extern int nvm_get_chunk_meta(struct nvm_tgt_dev *tgt_dev,
			      struct nvm_chk_meta *meta, struct ppa_addr ppa,
			      int nchks);

extern int nvm_set_tgt_bb_tbl(struct nvm_tgt_dev *, struct ppa_addr *,
			      int, int);
extern int nvm_submit_io(struct nvm_tgt_dev *, struct nvm_rq *);
extern int nvm_submit_io_sync(struct nvm_tgt_dev *, struct nvm_rq *);
extern void nvm_end_io(struct nvm_rq *);
extern int nvm_bb_tbl_fold(struct nvm_dev *, u8 *, int);
extern int nvm_get_tgt_bb_tbl(struct nvm_tgt_dev *, struct ppa_addr, u8 *);

#else 
struct nvm_dev_ops;

static inline struct nvm_dev *nvm_alloc_dev(int node)
{
	return ERR_PTR(-EINVAL);
}
static inline int nvm_register(struct nvm_dev *dev)
{
	return -EINVAL;
}
static inline void nvm_unregister(struct nvm_dev *dev) {}
#endif 
#endif 
