
#ifndef __LINUX_MTD_RAWNAND_H
#define __LINUX_MTD_RAWNAND_H

#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/flashchip.h>
#include <linux/mtd/bbm.h>
#include <linux/of.h>
#include <linux/types.h>

struct nand_chip;
struct nand_flash_dev;


int nand_scan_with_ids(struct nand_chip *chip, int max_chips,
		       struct nand_flash_dev *ids);

static inline int nand_scan(struct nand_chip *chip, int max_chips)
{
	return nand_scan_with_ids(chip, max_chips, NULL);
}


void nand_wait_ready(struct mtd_info *mtd);


#define NAND_MAX_CHIPS		8



#define NAND_NCE		0x01

#define NAND_CLE		0x02

#define NAND_ALE		0x04

#define NAND_CTRL_CLE		(NAND_NCE | NAND_CLE)
#define NAND_CTRL_ALE		(NAND_NCE | NAND_ALE)
#define NAND_CTRL_CHANGE	0x80


#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1
#define NAND_CMD_RNDOUT		5
#define NAND_CMD_PAGEPROG	0x10
#define NAND_CMD_READOOB	0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_RNDIN		0x85
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_PARAM		0xec
#define NAND_CMD_GET_FEATURES	0xee
#define NAND_CMD_SET_FEATURES	0xef
#define NAND_CMD_RESET		0xff


#define NAND_CMD_READSTART	0x30
#define NAND_CMD_RNDOUTSTART	0xE0
#define NAND_CMD_CACHEDPROG	0x15

#define NAND_CMD_NONE		-1


#define NAND_STATUS_FAIL	0x01
#define NAND_STATUS_FAIL_N1	0x02
#define NAND_STATUS_TRUE_READY	0x20
#define NAND_STATUS_READY	0x40
#define NAND_STATUS_WP		0x80

#define NAND_DATA_IFACE_CHECK_ONLY	-1


typedef enum {
	NAND_ECC_NONE,
	NAND_ECC_SOFT,
	NAND_ECC_HW,
	NAND_ECC_HW_SYNDROME,
	NAND_ECC_HW_OOB_FIRST,
	NAND_ECC_ON_DIE,
} nand_ecc_modes_t;

enum nand_ecc_algo {
	NAND_ECC_UNKNOWN,
	NAND_ECC_HAMMING,
	NAND_ECC_BCH,
	NAND_ECC_RS,
};



#define NAND_ECC_READ		0

#define NAND_ECC_WRITE		1

#define NAND_ECC_READSYN	2


#define NAND_ECC_GENERIC_ERASED_CHECK	BIT(0)
#define NAND_ECC_MAXIMIZE		BIT(1)


#define NAND_GET_DEVICE		0x80




#define NAND_BUSWIDTH_16	0x00000002

#define NAND_CACHEPRG		0x00000008

#define NAND_NEED_READRDY	0x00000100


#define NAND_NO_SUBPAGE_WRITE	0x00000200


#define NAND_BROKEN_XD		0x00000400


#define NAND_ROM		0x00000800


#define NAND_SUBPAGE_READ	0x00001000


#define NAND_NEED_SCRAMBLING	0x00002000


#define NAND_ROW_ADDR_3		0x00004000


#define NAND_SAMSUNG_LP_OPTIONS NAND_CACHEPRG


#define NAND_HAS_CACHEPROG(chip) ((chip->options & NAND_CACHEPRG))
#define NAND_HAS_SUBPAGE_READ(chip) ((chip->options & NAND_SUBPAGE_READ))
#define NAND_HAS_SUBPAGE_WRITE(chip) !((chip)->options & NAND_NO_SUBPAGE_WRITE)



#define NAND_SKIP_BBTSCAN	0x00010000

#define NAND_SCAN_SILENT_NODEV	0x00040000

#define NAND_BUSWIDTH_AUTO      0x00080000

#define NAND_USE_BOUNCE_BUFFER	0x00100000


#define NAND_WAIT_TCCS		0x00200000


#define NAND_IS_BOOT_MEDIUM	0x00400000



#define NAND_CONTROLLER_ALLOC	0x80000000


#define NAND_CI_CHIPNR_MSK	0x03
#define NAND_CI_CELLTYPE_MSK	0x0C
#define NAND_CI_CELLTYPE_SHIFT	2


struct nand_chip;


#define ONFI_VERSION_1_0		BIT(1)
#define ONFI_VERSION_2_0		BIT(2)
#define ONFI_VERSION_2_1		BIT(3)
#define ONFI_VERSION_2_2		BIT(4)
#define ONFI_VERSION_2_3		BIT(5)
#define ONFI_VERSION_3_0		BIT(6)
#define ONFI_VERSION_3_1		BIT(7)
#define ONFI_VERSION_3_2		BIT(8)
#define ONFI_VERSION_4_0		BIT(9)


#define ONFI_FEATURE_16_BIT_BUS		(1 << 0)
#define ONFI_FEATURE_EXT_PARAM_PAGE	(1 << 7)


#define ONFI_TIMING_MODE_0		(1 << 0)
#define ONFI_TIMING_MODE_1		(1 << 1)
#define ONFI_TIMING_MODE_2		(1 << 2)
#define ONFI_TIMING_MODE_3		(1 << 3)
#define ONFI_TIMING_MODE_4		(1 << 4)
#define ONFI_TIMING_MODE_5		(1 << 5)
#define ONFI_TIMING_MODE_UNKNOWN	(1 << 6)


#define ONFI_FEATURE_NUMBER		256
#define ONFI_FEATURE_ADDR_TIMING_MODE	0x1


#define ONFI_FEATURE_ADDR_READ_RETRY	0x89
#define ONFI_FEATURE_ON_DIE_ECC		0x90
#define   ONFI_FEATURE_ON_DIE_ECC_EN	BIT(3)


#define ONFI_SUBFEATURE_PARAM_LEN	4


#define ONFI_OPT_CMD_SET_GET_FEATURES	(1 << 2)

struct nand_onfi_params {
	
	
	u8 sig[4];
	__le16 revision;
	__le16 features;
	__le16 opt_cmd;
	u8 reserved0[2];
	__le16 ext_param_page_length; 
	u8 num_of_param_pages;        
	u8 reserved1[17];

	
	char manufacturer[12];
	char model[20];
	u8 jedec_id;
	__le16 date_code;
	u8 reserved2[13];

	
	__le32 byte_per_page;
	__le16 spare_bytes_per_page;
	__le32 data_bytes_per_ppage;
	__le16 spare_bytes_per_ppage;
	__le32 pages_per_block;
	__le32 blocks_per_lun;
	u8 lun_count;
	u8 addr_cycles;
	u8 bits_per_cell;
	__le16 bb_per_lun;
	__le16 block_endurance;
	u8 guaranteed_good_blocks;
	__le16 guaranteed_block_endurance;
	u8 programs_per_page;
	u8 ppage_attr;
	u8 ecc_bits;
	u8 interleaved_bits;
	u8 interleaved_ops;
	u8 reserved3[13];

	
	u8 io_pin_capacitance_max;
	__le16 async_timing_mode;
	__le16 program_cache_timing_mode;
	__le16 t_prog;
	__le16 t_bers;
	__le16 t_r;
	__le16 t_ccs;
	__le16 src_sync_timing_mode;
	u8 src_ssync_features;
	__le16 clk_pin_capacitance_typ;
	__le16 io_pin_capacitance_typ;
	__le16 input_pin_capacitance_typ;
	u8 input_pin_capacitance_max;
	u8 driver_strength_support;
	__le16 t_int_r;
	__le16 t_adl;
	u8 reserved4[8];

	
	__le16 vendor_revision;
	u8 vendor[88];

	__le16 crc;
} __packed;

#define ONFI_CRC_BASE	0x4F4E


struct onfi_ext_ecc_info {
	u8 ecc_bits;
	u8 codeword_size;
	__le16 bb_per_lun;
	__le16 block_endurance;
	u8 reserved[2];
} __packed;

#define ONFI_SECTION_TYPE_0	0	
#define ONFI_SECTION_TYPE_1	1	
#define ONFI_SECTION_TYPE_2	2	
struct onfi_ext_section {
	u8 type;
	u8 length;
} __packed;

#define ONFI_EXT_SECTION_MAX 8


struct onfi_ext_param_page {
	__le16 crc;
	u8 sig[4];             
	u8 reserved0[10];
	struct onfi_ext_section sections[ONFI_EXT_SECTION_MAX];

	
} __packed;

struct jedec_ecc_info {
	u8 ecc_bits;
	u8 codeword_size;
	__le16 bb_per_lun;
	__le16 block_endurance;
	u8 reserved[2];
} __packed;


#define JEDEC_FEATURE_16_BIT_BUS	(1 << 0)

struct nand_jedec_params {
	
	
	u8 sig[4];
	__le16 revision;
	__le16 features;
	u8 opt_cmd[3];
	__le16 sec_cmd;
	u8 num_of_param_pages;
	u8 reserved0[18];

	
	char manufacturer[12];
	char model[20];
	u8 jedec_id[6];
	u8 reserved1[10];

	
	__le32 byte_per_page;
	__le16 spare_bytes_per_page;
	u8 reserved2[6];
	__le32 pages_per_block;
	__le32 blocks_per_lun;
	u8 lun_count;
	u8 addr_cycles;
	u8 bits_per_cell;
	u8 programs_per_page;
	u8 multi_plane_addr;
	u8 multi_plane_op_attr;
	u8 reserved3[38];

	
	__le16 async_sdr_speed_grade;
	__le16 toggle_ddr_speed_grade;
	__le16 sync_ddr_speed_grade;
	u8 async_sdr_features;
	u8 toggle_ddr_features;
	u8 sync_ddr_features;
	__le16 t_prog;
	__le16 t_bers;
	__le16 t_r;
	__le16 t_r_multi_plane;
	__le16 t_ccs;
	__le16 io_pin_capacitance_typ;
	__le16 input_pin_capacitance_typ;
	__le16 clk_pin_capacitance_typ;
	u8 driver_strength_support;
	__le16 t_adl;
	u8 reserved4[36];

	
	u8 guaranteed_good_blocks;
	__le16 guaranteed_block_endurance;
	struct jedec_ecc_info ecc_info[4];
	u8 reserved5[29];

	
	u8 reserved6[148];

	
	__le16 vendor_rev_num;
	u8 reserved7[88];

	
	__le16 crc;
} __packed;


struct onfi_params {
	int version;
	u16 tPROG;
	u16 tBERS;
	u16 tR;
	u16 tCCS;
	u16 async_timing_mode;
	u16 vendor_revision;
	u8 vendor[88];
};


struct nand_parameters {
	
	const char *model;
	bool supports_set_get_features;
	DECLARE_BITMAP(set_feature_list, ONFI_FEATURE_NUMBER);
	DECLARE_BITMAP(get_feature_list, ONFI_FEATURE_NUMBER);

	
	struct onfi_params *onfi;
};


#define NAND_MAX_ID_LEN 8


struct nand_id {
	u8 data[NAND_MAX_ID_LEN];
	int len;
};


struct nand_controller_ops {
	int (*attach_chip)(struct nand_chip *chip);
	void (*detach_chip)(struct nand_chip *chip);
};


struct nand_controller {
	spinlock_t lock;
	struct nand_chip *active;
	wait_queue_head_t wq;
	const struct nand_controller_ops *ops;
};

static inline void nand_controller_init(struct nand_controller *nfc)
{
	nfc->active = NULL;
	spin_lock_init(&nfc->lock);
	init_waitqueue_head(&nfc->wq);
}


struct nand_ecc_step_info {
	int stepsize;
	const int *strengths;
	int nstrengths;
};


struct nand_ecc_caps {
	const struct nand_ecc_step_info *stepinfos;
	int nstepinfos;
	int (*calc_ecc_bytes)(int step_size, int strength);
};


#define NAND_ECC_CAPS_SINGLE(__name, __calc, __step, ...)	\
static const int __name##_strengths[] = { __VA_ARGS__ };	\
static const struct nand_ecc_step_info __name##_stepinfo = {	\
	.stepsize = __step,					\
	.strengths = __name##_strengths,			\
	.nstrengths = ARRAY_SIZE(__name##_strengths),		\
};								\
static const struct nand_ecc_caps __name = {			\
	.stepinfos = &__name##_stepinfo,			\
	.nstepinfos = 1,					\
	.calc_ecc_bytes = __calc,				\
}


struct nand_ecc_ctrl {
	nand_ecc_modes_t mode;
	enum nand_ecc_algo algo;
	int steps;
	int size;
	int bytes;
	int total;
	int strength;
	int prepad;
	int postpad;
	unsigned int options;
	void *priv;
	u8 *calc_buf;
	u8 *code_buf;
	void (*hwctl)(struct mtd_info *mtd, int mode);
	int (*calculate)(struct mtd_info *mtd, const uint8_t *dat,
			uint8_t *ecc_code);
	int (*correct)(struct mtd_info *mtd, uint8_t *dat, uint8_t *read_ecc,
			uint8_t *calc_ecc);
	int (*read_page_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			uint8_t *buf, int oob_required, int page);
	int (*write_page_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			const uint8_t *buf, int oob_required, int page);
	int (*read_page)(struct mtd_info *mtd, struct nand_chip *chip,
			uint8_t *buf, int oob_required, int page);
	int (*read_subpage)(struct mtd_info *mtd, struct nand_chip *chip,
			uint32_t offs, uint32_t len, uint8_t *buf, int page);
	int (*write_subpage)(struct mtd_info *mtd, struct nand_chip *chip,
			uint32_t offset, uint32_t data_len,
			const uint8_t *data_buf, int oob_required, int page);
	int (*write_page)(struct mtd_info *mtd, struct nand_chip *chip,
			const uint8_t *buf, int oob_required, int page);
	int (*write_oob_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			int page);
	int (*read_oob_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			int page);
	int (*read_oob)(struct mtd_info *mtd, struct nand_chip *chip, int page);
	int (*write_oob)(struct mtd_info *mtd, struct nand_chip *chip,
			int page);
};


struct nand_sdr_timings {
	u64 tBERS_max;
	u32 tCCS_min;
	u64 tPROG_max;
	u64 tR_max;
	u32 tALH_min;
	u32 tADL_min;
	u32 tALS_min;
	u32 tAR_min;
	u32 tCEA_max;
	u32 tCEH_min;
	u32 tCH_min;
	u32 tCHZ_max;
	u32 tCLH_min;
	u32 tCLR_min;
	u32 tCLS_min;
	u32 tCOH_min;
	u32 tCS_min;
	u32 tDH_min;
	u32 tDS_min;
	u32 tFEAT_max;
	u32 tIR_min;
	u32 tITC_max;
	u32 tRC_min;
	u32 tREA_max;
	u32 tREH_min;
	u32 tRHOH_min;
	u32 tRHW_min;
	u32 tRHZ_max;
	u32 tRLOH_min;
	u32 tRP_min;
	u32 tRR_min;
	u64 tRST_max;
	u32 tWB_max;
	u32 tWC_min;
	u32 tWH_min;
	u32 tWHR_min;
	u32 tWP_min;
	u32 tWW_min;
};


enum nand_data_interface_type {
	NAND_SDR_IFACE,
};


struct nand_data_interface {
	enum nand_data_interface_type type;
	union {
		struct nand_sdr_timings sdr;
	} timings;
};


static inline const struct nand_sdr_timings *
nand_get_sdr_timings(const struct nand_data_interface *conf)
{
	if (conf->type != NAND_SDR_IFACE)
		return ERR_PTR(-EINVAL);

	return &conf->timings.sdr;
}


struct nand_manufacturer_ops {
	void (*detect)(struct nand_chip *chip);
	int (*init)(struct nand_chip *chip);
	void (*cleanup)(struct nand_chip *chip);
	void (*fixup_onfi_param_page)(struct nand_chip *chip,
				      struct nand_onfi_params *p);
};


struct nand_op_cmd_instr {
	u8 opcode;
};


struct nand_op_addr_instr {
	unsigned int naddrs;
	const u8 *addrs;
};


struct nand_op_data_instr {
	unsigned int len;
	union {
		void *in;
		const void *out;
	} buf;
	bool force_8bit;
};


struct nand_op_waitrdy_instr {
	unsigned int timeout_ms;
};


enum nand_op_instr_type {
	NAND_OP_CMD_INSTR,
	NAND_OP_ADDR_INSTR,
	NAND_OP_DATA_IN_INSTR,
	NAND_OP_DATA_OUT_INSTR,
	NAND_OP_WAITRDY_INSTR,
};


struct nand_op_instr {
	enum nand_op_instr_type type;
	union {
		struct nand_op_cmd_instr cmd;
		struct nand_op_addr_instr addr;
		struct nand_op_data_instr data;
		struct nand_op_waitrdy_instr waitrdy;
	} ctx;
	unsigned int delay_ns;
};


#define __DIVIDE(dividend, divisor) ({						\
	(__typeof__(dividend))(sizeof(dividend) <= sizeof(unsigned long) ?	\
			       DIV_ROUND_UP(dividend, divisor) :		\
			       DIV_ROUND_UP_ULL(dividend, divisor)); 		\
	})
#define PSEC_TO_NSEC(x) __DIVIDE(x, 1000)
#define PSEC_TO_MSEC(x) __DIVIDE(x, 1000000000)

#define NAND_OP_CMD(id, ns)						\
	{								\
		.type = NAND_OP_CMD_INSTR,				\
		.ctx.cmd.opcode = id,					\
		.delay_ns = ns,						\
	}

#define NAND_OP_ADDR(ncycles, cycles, ns)				\
	{								\
		.type = NAND_OP_ADDR_INSTR,				\
		.ctx.addr = {						\
			.naddrs = ncycles,				\
			.addrs = cycles,				\
		},							\
		.delay_ns = ns,						\
	}

#define NAND_OP_DATA_IN(l, b, ns)					\
	{								\
		.type = NAND_OP_DATA_IN_INSTR,				\
		.ctx.data = {						\
			.len = l,					\
			.buf.in = b,					\
			.force_8bit = false,				\
		},							\
		.delay_ns = ns,						\
	}

#define NAND_OP_DATA_OUT(l, b, ns)					\
	{								\
		.type = NAND_OP_DATA_OUT_INSTR,				\
		.ctx.data = {						\
			.len = l,					\
			.buf.out = b,					\
			.force_8bit = false,				\
		},							\
		.delay_ns = ns,						\
	}

#define NAND_OP_8BIT_DATA_IN(l, b, ns)					\
	{								\
		.type = NAND_OP_DATA_IN_INSTR,				\
		.ctx.data = {						\
			.len = l,					\
			.buf.in = b,					\
			.force_8bit = true,				\
		},							\
		.delay_ns = ns,						\
	}

#define NAND_OP_8BIT_DATA_OUT(l, b, ns)					\
	{								\
		.type = NAND_OP_DATA_OUT_INSTR,				\
		.ctx.data = {						\
			.len = l,					\
			.buf.out = b,					\
			.force_8bit = true,				\
		},							\
		.delay_ns = ns,						\
	}

#define NAND_OP_WAIT_RDY(tout_ms, ns)					\
	{								\
		.type = NAND_OP_WAITRDY_INSTR,				\
		.ctx.waitrdy.timeout_ms = tout_ms,			\
		.delay_ns = ns,						\
	}


struct nand_subop {
	const struct nand_op_instr *instrs;
	unsigned int ninstrs;
	unsigned int first_instr_start_off;
	unsigned int last_instr_end_off;
};

unsigned int nand_subop_get_addr_start_off(const struct nand_subop *subop,
					   unsigned int op_id);
unsigned int nand_subop_get_num_addr_cyc(const struct nand_subop *subop,
					 unsigned int op_id);
unsigned int nand_subop_get_data_start_off(const struct nand_subop *subop,
					   unsigned int op_id);
unsigned int nand_subop_get_data_len(const struct nand_subop *subop,
				     unsigned int op_id);


struct nand_op_parser_addr_constraints {
	unsigned int maxcycles;
};


struct nand_op_parser_data_constraints {
	unsigned int maxlen;
};


struct nand_op_parser_pattern_elem {
	enum nand_op_instr_type type;
	bool optional;
	union {
		struct nand_op_parser_addr_constraints addr;
		struct nand_op_parser_data_constraints data;
	} ctx;
};

#define NAND_OP_PARSER_PAT_CMD_ELEM(_opt)			\
	{							\
		.type = NAND_OP_CMD_INSTR,			\
		.optional = _opt,				\
	}

#define NAND_OP_PARSER_PAT_ADDR_ELEM(_opt, _maxcycles)		\
	{							\
		.type = NAND_OP_ADDR_INSTR,			\
		.optional = _opt,				\
		.ctx.addr.maxcycles = _maxcycles,		\
	}

#define NAND_OP_PARSER_PAT_DATA_IN_ELEM(_opt, _maxlen)		\
	{							\
		.type = NAND_OP_DATA_IN_INSTR,			\
		.optional = _opt,				\
		.ctx.data.maxlen = _maxlen,			\
	}

#define NAND_OP_PARSER_PAT_DATA_OUT_ELEM(_opt, _maxlen)		\
	{							\
		.type = NAND_OP_DATA_OUT_INSTR,			\
		.optional = _opt,				\
		.ctx.data.maxlen = _maxlen,			\
	}

#define NAND_OP_PARSER_PAT_WAITRDY_ELEM(_opt)			\
	{							\
		.type = NAND_OP_WAITRDY_INSTR,			\
		.optional = _opt,				\
	}


struct nand_op_parser_pattern {
	const struct nand_op_parser_pattern_elem *elems;
	unsigned int nelems;
	int (*exec)(struct nand_chip *chip, const struct nand_subop *subop);
};

#define NAND_OP_PARSER_PATTERN(_exec, ...)							\
	{											\
		.exec = _exec,									\
		.elems = (struct nand_op_parser_pattern_elem[]) { __VA_ARGS__ },		\
		.nelems = sizeof((struct nand_op_parser_pattern_elem[]) { __VA_ARGS__ }) /	\
			  sizeof(struct nand_op_parser_pattern_elem),				\
	}


struct nand_op_parser {
	const struct nand_op_parser_pattern *patterns;
	unsigned int npatterns;
};

#define NAND_OP_PARSER(...)									\
	{											\
		.patterns = (struct nand_op_parser_pattern[]) { __VA_ARGS__ },			\
		.npatterns = sizeof((struct nand_op_parser_pattern[]) { __VA_ARGS__ }) /	\
			     sizeof(struct nand_op_parser_pattern),				\
	}


struct nand_operation {
	const struct nand_op_instr *instrs;
	unsigned int ninstrs;
};

#define NAND_OPERATION(_instrs)					\
	{							\
		.instrs = _instrs,				\
		.ninstrs = ARRAY_SIZE(_instrs),			\
	}

int nand_op_parser_exec_op(struct nand_chip *chip,
			   const struct nand_op_parser *parser,
			   const struct nand_operation *op, bool check_only);



struct nand_chip {
	struct mtd_info mtd;
	void __iomem *IO_ADDR_R;
	void __iomem *IO_ADDR_W;

	uint8_t (*read_byte)(struct mtd_info *mtd);
	u16 (*read_word)(struct mtd_info *mtd);
	void (*write_byte)(struct mtd_info *mtd, uint8_t byte);
	void (*write_buf)(struct mtd_info *mtd, const uint8_t *buf, int len);
	void (*read_buf)(struct mtd_info *mtd, uint8_t *buf, int len);
	void (*select_chip)(struct mtd_info *mtd, int chip);
	int (*block_bad)(struct mtd_info *mtd, loff_t ofs);
	int (*block_markbad)(struct mtd_info *mtd, loff_t ofs);
	void (*cmd_ctrl)(struct mtd_info *mtd, int dat, unsigned int ctrl);
	int (*dev_ready)(struct mtd_info *mtd);
	void (*cmdfunc)(struct mtd_info *mtd, unsigned command, int column,
			int page_addr);
	int(*waitfunc)(struct mtd_info *mtd, struct nand_chip *this);
	int (*exec_op)(struct nand_chip *chip,
		       const struct nand_operation *op,
		       bool check_only);
	int (*erase)(struct mtd_info *mtd, int page);
	int (*set_features)(struct mtd_info *mtd, struct nand_chip *chip,
			    int feature_addr, uint8_t *subfeature_para);
	int (*get_features)(struct mtd_info *mtd, struct nand_chip *chip,
			    int feature_addr, uint8_t *subfeature_para);
	int (*setup_read_retry)(struct mtd_info *mtd, int retry_mode);
	int (*setup_data_interface)(struct mtd_info *mtd, int chipnr,
				    const struct nand_data_interface *conf);

	int chip_delay;
	unsigned int options;
	unsigned int bbt_options;

	int page_shift;
	int phys_erase_shift;
	int bbt_erase_shift;
	int chip_shift;
	int numchips;
	uint64_t chipsize;
	int pagemask;
	u8 *data_buf;
	int pagebuf;
	unsigned int pagebuf_bitflips;
	int subpagesize;
	uint8_t bits_per_cell;
	uint16_t ecc_strength_ds;
	uint16_t ecc_step_ds;
	int onfi_timing_mode_default;
	int badblockpos;
	int badblockbits;

	struct nand_id id;
	struct nand_parameters parameters;
	u16 max_bb_per_die;
	u32 blocks_per_die;

	struct nand_data_interface data_interface;

	int read_retries;

	flstate_t state;

	uint8_t *oob_poi;
	struct nand_controller *controller;

	struct nand_ecc_ctrl ecc;
	unsigned long buf_align;
	struct nand_controller dummy_controller;

	uint8_t *bbt;
	struct nand_bbt_descr *bbt_td;
	struct nand_bbt_descr *bbt_md;

	struct nand_bbt_descr *badblock_pattern;

	void *priv;

	struct {
		const struct nand_manufacturer *desc;
		void *priv;
	} manufacturer;
};

static inline int nand_exec_op(struct nand_chip *chip,
			       const struct nand_operation *op)
{
	if (!chip->exec_op)
		return -ENOTSUPP;

	return chip->exec_op(chip, op, false);
}

extern const struct mtd_ooblayout_ops nand_ooblayout_sp_ops;
extern const struct mtd_ooblayout_ops nand_ooblayout_lp_ops;

static inline void nand_set_flash_node(struct nand_chip *chip,
				       struct device_node *np)
{
	mtd_set_of_node(&chip->mtd, np);
}

static inline struct device_node *nand_get_flash_node(struct nand_chip *chip)
{
	return mtd_get_of_node(&chip->mtd);
}

static inline struct nand_chip *mtd_to_nand(struct mtd_info *mtd)
{
	return container_of(mtd, struct nand_chip, mtd);
}

static inline struct mtd_info *nand_to_mtd(struct nand_chip *chip)
{
	return &chip->mtd;
}

static inline void *nand_get_controller_data(struct nand_chip *chip)
{
	return chip->priv;
}

static inline void nand_set_controller_data(struct nand_chip *chip, void *priv)
{
	chip->priv = priv;
}

static inline void nand_set_manufacturer_data(struct nand_chip *chip,
					      void *priv)
{
	chip->manufacturer.priv = priv;
}

static inline void *nand_get_manufacturer_data(struct nand_chip *chip)
{
	return chip->manufacturer.priv;
}


#define NAND_MFR_TOSHIBA	0x98
#define NAND_MFR_ESMT		0xc8
#define NAND_MFR_SAMSUNG	0xec
#define NAND_MFR_FUJITSU	0x04
#define NAND_MFR_NATIONAL	0x8f
#define NAND_MFR_RENESAS	0x07
#define NAND_MFR_STMICRO	0x20
#define NAND_MFR_HYNIX		0xad
#define NAND_MFR_MICRON		0x2c
#define NAND_MFR_AMD		0x01
#define NAND_MFR_MACRONIX	0xc2
#define NAND_MFR_EON		0x92
#define NAND_MFR_SANDISK	0x45
#define NAND_MFR_INTEL		0x89
#define NAND_MFR_ATO		0x9b
#define NAND_MFR_WINBOND	0xef



#define LEGACY_ID_NAND(nm, devid, chipsz, erasesz, opts)          \
	{ .name = (nm), {{ .dev_id = (devid) }}, .pagesize = 512, \
	  .chipsize = (chipsz), .erasesize = (erasesz), .options = (opts) }


#define EXTENDED_ID_NAND(nm, devid, chipsz, opts)                      \
	{ .name = (nm), {{ .dev_id = (devid) }}, .chipsize = (chipsz), \
	  .options = (opts) }

#define NAND_ECC_INFO(_strength, _step)	\
			{ .strength_ds = (_strength), .step_ds = (_step) }
#define NAND_ECC_STRENGTH(type)		((type)->ecc.strength_ds)
#define NAND_ECC_STEP(type)		((type)->ecc.step_ds)


struct nand_flash_dev {
	char *name;
	union {
		struct {
			uint8_t mfr_id;
			uint8_t dev_id;
		};
		uint8_t id[NAND_MAX_ID_LEN];
	};
	unsigned int pagesize;
	unsigned int chipsize;
	unsigned int erasesize;
	unsigned int options;
	uint16_t id_len;
	uint16_t oobsize;
	struct {
		uint16_t strength_ds;
		uint16_t step_ds;
	} ecc;
	int onfi_timing_mode_default;
};


struct nand_manufacturer {
	int id;
	char *name;
	const struct nand_manufacturer_ops *ops;
};

const struct nand_manufacturer *nand_get_manufacturer(u8 id);

static inline const char *
nand_manufacturer_name(const struct nand_manufacturer *manufacturer)
{
	return manufacturer ? manufacturer->name : "Unknown";
}

extern struct nand_flash_dev nand_flash_ids[];

extern const struct nand_manufacturer_ops toshiba_nand_manuf_ops;
extern const struct nand_manufacturer_ops samsung_nand_manuf_ops;
extern const struct nand_manufacturer_ops hynix_nand_manuf_ops;
extern const struct nand_manufacturer_ops micron_nand_manuf_ops;
extern const struct nand_manufacturer_ops amd_nand_manuf_ops;
extern const struct nand_manufacturer_ops macronix_nand_manuf_ops;

int nand_create_bbt(struct nand_chip *chip);
int nand_markbad_bbt(struct mtd_info *mtd, loff_t offs);
int nand_isreserved_bbt(struct mtd_info *mtd, loff_t offs);
int nand_isbad_bbt(struct mtd_info *mtd, loff_t offs, int allowbbt);
int nand_erase_nand(struct mtd_info *mtd, struct erase_info *instr,
		    int allowbbt);


struct platform_nand_chip {
	int nr_chips;
	int chip_offset;
	int nr_partitions;
	struct mtd_partition *partitions;
	int chip_delay;
	unsigned int options;
	unsigned int bbt_options;
	const char **part_probe_types;
};


struct platform_device;


struct platform_nand_ctrl {
	int (*probe)(struct platform_device *pdev);
	void (*remove)(struct platform_device *pdev);
	int (*dev_ready)(struct mtd_info *mtd);
	void (*select_chip)(struct mtd_info *mtd, int chip);
	void (*cmd_ctrl)(struct mtd_info *mtd, int dat, unsigned int ctrl);
	void (*write_buf)(struct mtd_info *mtd, const uint8_t *buf, int len);
	void (*read_buf)(struct mtd_info *mtd, uint8_t *buf, int len);
	void *priv;
};


struct platform_nand_data {
	struct platform_nand_chip chip;
	struct platform_nand_ctrl ctrl;
};


static inline int onfi_get_async_timing_mode(struct nand_chip *chip)
{
	if (!chip->parameters.onfi)
		return ONFI_TIMING_MODE_UNKNOWN;

	return chip->parameters.onfi->async_timing_mode;
}

int onfi_fill_data_interface(struct nand_chip *chip,
			     enum nand_data_interface_type type,
			     int timing_mode);


static inline bool nand_is_slc(struct nand_chip *chip)
{
	WARN(chip->bits_per_cell == 0,
	     "chip->bits_per_cell is used uninitialized\n");
	return chip->bits_per_cell == 1;
}


static inline int nand_opcode_8bits(unsigned int command)
{
	switch (command) {
	case NAND_CMD_READID:
	case NAND_CMD_PARAM:
	case NAND_CMD_GET_FEATURES:
	case NAND_CMD_SET_FEATURES:
		return 1;
	default:
		break;
	}
	return 0;
}


const struct nand_sdr_timings *onfi_async_timing_mode_to_sdr_timings(int mode);

int nand_check_erased_ecc_chunk(void *data, int datalen,
				void *ecc, int ecclen,
				void *extraoob, int extraooblen,
				int threshold);

int nand_ecc_choose_conf(struct nand_chip *chip,
			 const struct nand_ecc_caps *caps, int oobavail);


int nand_write_oob_std(struct mtd_info *mtd, struct nand_chip *chip, int page);


int nand_write_oob_syndrome(struct mtd_info *mtd, struct nand_chip *chip,
			    int page);


int nand_read_oob_std(struct mtd_info *mtd, struct nand_chip *chip, int page);


int nand_read_oob_syndrome(struct mtd_info *mtd, struct nand_chip *chip,
			   int page);


int nand_get_features(struct nand_chip *chip, int addr, u8 *subfeature_param);
int nand_set_features(struct nand_chip *chip, int addr, u8 *subfeature_param);

int nand_get_set_features_notsupp(struct mtd_info *mtd, struct nand_chip *chip,
				  int addr, u8 *subfeature_param);


int nand_read_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
		       uint8_t *buf, int oob_required, int page);
int nand_read_page_raw_notsupp(struct mtd_info *mtd, struct nand_chip *chip,
			       u8 *buf, int oob_required, int page);


int nand_write_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
			const uint8_t *buf, int oob_required, int page);
int nand_write_page_raw_notsupp(struct mtd_info *mtd, struct nand_chip *chip,
				const u8 *buf, int oob_required, int page);


int nand_reset(struct nand_chip *chip, int chipnr);


int nand_reset_op(struct nand_chip *chip);
int nand_readid_op(struct nand_chip *chip, u8 addr, void *buf,
		   unsigned int len);
int nand_status_op(struct nand_chip *chip, u8 *status);
int nand_exit_status_op(struct nand_chip *chip);
int nand_erase_op(struct nand_chip *chip, unsigned int eraseblock);
int nand_read_page_op(struct nand_chip *chip, unsigned int page,
		      unsigned int offset_in_page, void *buf, unsigned int len);
int nand_change_read_column_op(struct nand_chip *chip,
			       unsigned int offset_in_page, void *buf,
			       unsigned int len, bool force_8bit);
int nand_read_oob_op(struct nand_chip *chip, unsigned int page,
		     unsigned int offset_in_page, void *buf, unsigned int len);
int nand_prog_page_begin_op(struct nand_chip *chip, unsigned int page,
			    unsigned int offset_in_page, const void *buf,
			    unsigned int len);
int nand_prog_page_end_op(struct nand_chip *chip);
int nand_prog_page_op(struct nand_chip *chip, unsigned int page,
		      unsigned int offset_in_page, const void *buf,
		      unsigned int len);
int nand_change_write_column_op(struct nand_chip *chip,
				unsigned int offset_in_page, const void *buf,
				unsigned int len, bool force_8bit);
int nand_read_data_op(struct nand_chip *chip, void *buf, unsigned int len,
		      bool force_8bit);
int nand_write_data_op(struct nand_chip *chip, const void *buf,
		       unsigned int len, bool force_8bit);


void nand_cleanup(struct nand_chip *chip);

void nand_release(struct nand_chip *chip);


void nand_decode_ext_id(struct nand_chip *chip);


int nand_soft_waitrdy(struct nand_chip *chip, unsigned long timeout_ms);

#endif 
