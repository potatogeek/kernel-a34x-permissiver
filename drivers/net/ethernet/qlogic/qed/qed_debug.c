<<<<<<< HEAD
/* QLogic qed NIC Driver
 * Copyright (c) 2015 QLogic Corporation
 *
 * This software is available under the terms of the GNU General Public License
 * (GPL) Version 2, available from the file COPYING in the main directory of
 * this source tree.
=======
// SPDX-License-Identifier: (GPL-2.0-only OR BSD-3-Clause)
/* QLogic qed NIC Driver
 * Copyright (c) 2015 QLogic Corporation
 * Copyright (c) 2019-2020 Marvell International Ltd.
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/crc32.h>
#include "qed.h"
<<<<<<< HEAD
=======
#include "qed_cxt.h"
>>>>>>> upstream/android-13
#include "qed_hsi.h"
#include "qed_hw.h"
#include "qed_mcp.h"
#include "qed_reg_addr.h"

/* Memory groups enum */
enum mem_groups {
	MEM_GROUP_PXP_MEM,
	MEM_GROUP_DMAE_MEM,
	MEM_GROUP_CM_MEM,
	MEM_GROUP_QM_MEM,
	MEM_GROUP_DORQ_MEM,
	MEM_GROUP_BRB_RAM,
	MEM_GROUP_BRB_MEM,
	MEM_GROUP_PRS_MEM,
<<<<<<< HEAD
	MEM_GROUP_IOR,
	MEM_GROUP_BTB_RAM,
	MEM_GROUP_CONN_CFC_MEM,
	MEM_GROUP_TASK_CFC_MEM,
	MEM_GROUP_CAU_PI,
	MEM_GROUP_CAU_MEM,
	MEM_GROUP_PXP_ILT,
	MEM_GROUP_TM_MEM,
	MEM_GROUP_SDM_MEM,
	MEM_GROUP_PBUF,
	MEM_GROUP_RAM,
	MEM_GROUP_MULD_MEM,
	MEM_GROUP_BTB_MEM,
	MEM_GROUP_RDIF_CTX,
	MEM_GROUP_TDIF_CTX,
	MEM_GROUP_CFC_MEM,
=======
	MEM_GROUP_SDM_MEM,
	MEM_GROUP_PBUF,
	MEM_GROUP_IOR,
	MEM_GROUP_RAM,
	MEM_GROUP_BTB_RAM,
	MEM_GROUP_RDIF_CTX,
	MEM_GROUP_TDIF_CTX,
	MEM_GROUP_CFC_MEM,
	MEM_GROUP_CONN_CFC_MEM,
	MEM_GROUP_CAU_PI,
	MEM_GROUP_CAU_MEM,
	MEM_GROUP_CAU_MEM_EXT,
	MEM_GROUP_PXP_ILT,
	MEM_GROUP_MULD_MEM,
	MEM_GROUP_BTB_MEM,
>>>>>>> upstream/android-13
	MEM_GROUP_IGU_MEM,
	MEM_GROUP_IGU_MSIX,
	MEM_GROUP_CAU_SB,
	MEM_GROUP_BMB_RAM,
	MEM_GROUP_BMB_MEM,
<<<<<<< HEAD
=======
	MEM_GROUP_TM_MEM,
	MEM_GROUP_TASK_CFC_MEM,
>>>>>>> upstream/android-13
	MEM_GROUPS_NUM
};

/* Memory groups names */
static const char * const s_mem_group_names[] = {
	"PXP_MEM",
	"DMAE_MEM",
	"CM_MEM",
	"QM_MEM",
	"DORQ_MEM",
	"BRB_RAM",
	"BRB_MEM",
	"PRS_MEM",
<<<<<<< HEAD
	"IOR",
	"BTB_RAM",
	"CONN_CFC_MEM",
	"TASK_CFC_MEM",
	"CAU_PI",
	"CAU_MEM",
	"PXP_ILT",
	"TM_MEM",
	"SDM_MEM",
	"PBUF",
	"RAM",
	"MULD_MEM",
	"BTB_MEM",
	"RDIF_CTX",
	"TDIF_CTX",
	"CFC_MEM",
=======
	"SDM_MEM",
	"PBUF",
	"IOR",
	"RAM",
	"BTB_RAM",
	"RDIF_CTX",
	"TDIF_CTX",
	"CFC_MEM",
	"CONN_CFC_MEM",
	"CAU_PI",
	"CAU_MEM",
	"CAU_MEM_EXT",
	"PXP_ILT",
	"MULD_MEM",
	"BTB_MEM",
>>>>>>> upstream/android-13
	"IGU_MEM",
	"IGU_MSIX",
	"CAU_SB",
	"BMB_RAM",
	"BMB_MEM",
<<<<<<< HEAD
=======
	"TM_MEM",
	"TASK_CFC_MEM",
>>>>>>> upstream/android-13
};

/* Idle check conditions */

static u32 cond5(const u32 *r, const u32 *imm)
{
	return ((r[0] & imm[0]) != imm[1]) && ((r[1] & imm[2]) != imm[3]);
}

static u32 cond7(const u32 *r, const u32 *imm)
{
	return ((r[0] >> imm[0]) & imm[1]) != imm[2];
}

static u32 cond6(const u32 *r, const u32 *imm)
{
	return (r[0] & imm[0]) != imm[1];
}

static u32 cond9(const u32 *r, const u32 *imm)
{
	return ((r[0] & imm[0]) >> imm[1]) !=
	    (((r[0] & imm[2]) >> imm[3]) | ((r[1] & imm[4]) << imm[5]));
}

static u32 cond10(const u32 *r, const u32 *imm)
{
	return ((r[0] & imm[0]) >> imm[1]) != (r[0] & imm[2]);
}

static u32 cond4(const u32 *r, const u32 *imm)
{
	return (r[0] & ~imm[0]) != imm[1];
}

static u32 cond0(const u32 *r, const u32 *imm)
{
	return (r[0] & ~r[1]) != imm[0];
}

static u32 cond1(const u32 *r, const u32 *imm)
{
	return r[0] != imm[0];
}

static u32 cond11(const u32 *r, const u32 *imm)
{
	return r[0] != r[1] && r[2] == imm[0];
}

static u32 cond12(const u32 *r, const u32 *imm)
{
	return r[0] != r[1] && r[2] > imm[0];
}

static u32 cond3(const u32 *r, const u32 *imm)
{
	return r[0] != r[1];
}

static u32 cond13(const u32 *r, const u32 *imm)
{
	return r[0] & imm[0];
}

static u32 cond8(const u32 *r, const u32 *imm)
{
	return r[0] < (r[1] - imm[0]);
}

static u32 cond2(const u32 *r, const u32 *imm)
{
	return r[0] > imm[0];
}

/* Array of Idle Check conditions */
static u32(*cond_arr[]) (const u32 *r, const u32 *imm) = {
	cond0,
	cond1,
	cond2,
	cond3,
	cond4,
	cond5,
	cond6,
	cond7,
	cond8,
	cond9,
	cond10,
	cond11,
	cond12,
	cond13,
};

<<<<<<< HEAD
/******************************* Data Types **********************************/

enum platform_ids {
	PLATFORM_ASIC,
	PLATFORM_RESERVED,
	PLATFORM_RESERVED2,
	PLATFORM_RESERVED3,
	MAX_PLATFORM_IDS
=======
#define NUM_PHYS_BLOCKS 84

#define NUM_DBG_RESET_REGS 8

/******************************* Data Types **********************************/

enum hw_types {
	HW_TYPE_ASIC,
	PLATFORM_RESERVED,
	PLATFORM_RESERVED2,
	PLATFORM_RESERVED3,
	PLATFORM_RESERVED4,
	MAX_HW_TYPES
};

/* CM context types */
enum cm_ctx_types {
	CM_CTX_CONN_AG,
	CM_CTX_CONN_ST,
	CM_CTX_TASK_AG,
	CM_CTX_TASK_ST,
	NUM_CM_CTX_TYPES
};

/* Debug bus frame modes */
enum dbg_bus_frame_modes {
	DBG_BUS_FRAME_MODE_4ST = 0,	/* 4 Storm dwords (no HW) */
	DBG_BUS_FRAME_MODE_2ST_2HW = 1,	/* 2 Storm dwords, 2 HW dwords */
	DBG_BUS_FRAME_MODE_1ST_3HW = 2,	/* 1 Storm dwords, 3 HW dwords */
	DBG_BUS_FRAME_MODE_4HW = 3,	/* 4 HW dwords (no Storms) */
	DBG_BUS_FRAME_MODE_8HW = 4,	/* 8 HW dwords (no Storms) */
	DBG_BUS_NUM_FRAME_MODES
>>>>>>> upstream/android-13
};

/* Chip constant definitions */
struct chip_defs {
	const char *name;
<<<<<<< HEAD
};

/* Platform constant definitions */
struct platform_defs {
=======
	u32 num_ilt_pages;
};

/* HW type constant definitions */
struct hw_type_defs {
>>>>>>> upstream/android-13
	const char *name;
	u32 delay_factor;
	u32 dmae_thresh;
	u32 log_thresh;
};

<<<<<<< HEAD
=======
/* RBC reset definitions */
struct rbc_reset_defs {
	u32 reset_reg_addr;
	u32 reset_val[MAX_CHIP_IDS];
};

>>>>>>> upstream/android-13
/* Storm constant definitions.
 * Addresses are in bytes, sizes are in quad-regs.
 */
struct storm_defs {
	char letter;
<<<<<<< HEAD
	enum block_id block_id;
=======
	enum block_id sem_block_id;
>>>>>>> upstream/android-13
	enum dbg_bus_clients dbg_client_id[MAX_CHIP_IDS];
	bool has_vfc;
	u32 sem_fast_mem_addr;
	u32 sem_frame_mode_addr;
	u32 sem_slow_enable_addr;
	u32 sem_slow_mode_addr;
	u32 sem_slow_mode1_conf_addr;
	u32 sem_sync_dbg_empty_addr;
<<<<<<< HEAD
	u32 sem_slow_dbg_empty_addr;
	u32 cm_ctx_wr_addr;
	u32 cm_conn_ag_ctx_lid_size;
	u32 cm_conn_ag_ctx_rd_addr;
	u32 cm_conn_st_ctx_lid_size;
	u32 cm_conn_st_ctx_rd_addr;
	u32 cm_task_ag_ctx_lid_size;
	u32 cm_task_ag_ctx_rd_addr;
	u32 cm_task_st_ctx_lid_size;
	u32 cm_task_st_ctx_rd_addr;
};

/* Block constant definitions */
struct block_defs {
	const char *name;
	bool exists[MAX_CHIP_IDS];
	bool associated_to_storm;

	/* Valid only if associated_to_storm is true */
	u32 storm_id;
	enum dbg_bus_clients dbg_client_id[MAX_CHIP_IDS];
	u32 dbg_select_addr;
	u32 dbg_enable_addr;
	u32 dbg_shift_addr;
	u32 dbg_force_valid_addr;
	u32 dbg_force_frame_addr;
	bool has_reset_bit;

	/* If true, block is taken out of reset before dump */
	bool unreset;
	enum dbg_reset_regs reset_reg;

	/* Bit offset in reset register */
	u8 reset_bit_offset;
};

/* Reset register definitions */
struct reset_reg_defs {
	u32 addr;
	bool exists[MAX_CHIP_IDS];
	u32 unreset_val[MAX_CHIP_IDS];
=======
	u32 sem_gpre_vect_addr;
	u32 cm_ctx_wr_addr;
	u32 cm_ctx_rd_addr[NUM_CM_CTX_TYPES];
	u32 cm_ctx_lid_sizes[MAX_CHIP_IDS][NUM_CM_CTX_TYPES];
};

/* Debug Bus Constraint operation constant definitions */
struct dbg_bus_constraint_op_defs {
	u8 hw_op_val;
	bool is_cyclic;
};

/* Storm Mode definitions */
struct storm_mode_defs {
	const char *name;
	bool is_fast_dbg;
	u8 id_in_hw;
	u32 src_disable_reg_addr;
	u32 src_enable_val;
	bool exists[MAX_CHIP_IDS];
>>>>>>> upstream/android-13
};

struct grc_param_defs {
	u32 default_val[MAX_CHIP_IDS];
	u32 min;
	u32 max;
	bool is_preset;
	bool is_persistent;
	u32 exclude_all_preset_val;
<<<<<<< HEAD
	u32 crash_preset_val;
=======
	u32 crash_preset_val[MAX_CHIP_IDS];
>>>>>>> upstream/android-13
};

/* Address is in 128b units. Width is in bits. */
struct rss_mem_defs {
	const char *mem_name;
	const char *type_name;
	u32 addr;
	u32 entry_width;
	u32 num_entries[MAX_CHIP_IDS];
};

struct vfc_ram_defs {
	const char *mem_name;
	const char *type_name;
	u32 base_row;
	u32 num_rows;
};

struct big_ram_defs {
	const char *instance_name;
	enum mem_groups mem_group_id;
	enum mem_groups ram_mem_group_id;
	enum dbg_grc_params grc_param;
	u32 addr_reg_addr;
	u32 data_reg_addr;
	u32 is_256b_reg_addr;
	u32 is_256b_bit_offset[MAX_CHIP_IDS];
	u32 ram_size[MAX_CHIP_IDS]; /* In dwords */
};

struct phy_defs {
	const char *phy_name;

	/* PHY base GRC address */
	u32 base_addr;

	/* Relative address of indirect TBUS address register (bits 0..7) */
	u32 tbus_addr_lo_addr;

	/* Relative address of indirect TBUS address register (bits 8..10) */
	u32 tbus_addr_hi_addr;

	/* Relative address of indirect TBUS data register (bits 0..7) */
	u32 tbus_data_lo_addr;

	/* Relative address of indirect TBUS data register (bits 8..11) */
	u32 tbus_data_hi_addr;
};

/* Split type definitions */
struct split_type_defs {
	const char *name;
};

/******************************** Constants **********************************/

<<<<<<< HEAD
#define MAX_LCIDS			320
#define MAX_LTIDS			320

#define NUM_IOR_SETS			2
#define IORS_PER_SET			176
#define IOR_SET_OFFSET(set_id)		((set_id) * 256)

#define BYTES_IN_DWORD			sizeof(u32)

=======
#define BYTES_IN_DWORD			sizeof(u32)
>>>>>>> upstream/android-13
/* In the macros below, size and offset are specified in bits */
#define CEIL_DWORDS(size)		DIV_ROUND_UP(size, 32)
#define FIELD_BIT_OFFSET(type, field)	type ## _ ## field ## _ ## OFFSET
#define FIELD_BIT_SIZE(type, field)	type ## _ ## field ## _ ## SIZE
#define FIELD_DWORD_OFFSET(type, field) \
	 (int)(FIELD_BIT_OFFSET(type, field) / 32)
#define FIELD_DWORD_SHIFT(type, field)	(FIELD_BIT_OFFSET(type, field) % 32)
#define FIELD_BIT_MASK(type, field) \
	(((1 << FIELD_BIT_SIZE(type, field)) - 1) << \
	 FIELD_DWORD_SHIFT(type, field))

#define SET_VAR_FIELD(var, type, field, val) \
	do { \
		var[FIELD_DWORD_OFFSET(type, field)] &=	\
		(~FIELD_BIT_MASK(type, field));	\
		var[FIELD_DWORD_OFFSET(type, field)] |= \
		(val) << FIELD_DWORD_SHIFT(type, field); \
	} while (0)

#define ARR_REG_WR(dev, ptt, addr, arr, arr_size) \
	do { \
		for (i = 0; i < (arr_size); i++) \
			qed_wr(dev, ptt, addr,	(arr)[i]); \
	} while (0)

<<<<<<< HEAD
#define ARR_REG_RD(dev, ptt, addr, arr, arr_size) \
	do { \
		for (i = 0; i < (arr_size); i++) \
			(arr)[i] = qed_rd(dev, ptt, addr); \
	} while (0)

#define DWORDS_TO_BYTES(dwords)		((dwords) * BYTES_IN_DWORD)
#define BYTES_TO_DWORDS(bytes)		((bytes) / BYTES_IN_DWORD)

/* Extra lines include a signature line + optional latency events line */
#define NUM_EXTRA_DBG_LINES(block_desc) \
	(1 + ((block_desc)->has_latency_events ? 1 : 0))
#define NUM_DBG_LINES(block_desc) \
	((block_desc)->num_of_lines + NUM_EXTRA_DBG_LINES(block_desc))
=======
#define DWORDS_TO_BYTES(dwords)		((dwords) * BYTES_IN_DWORD)
#define BYTES_TO_DWORDS(bytes)		((bytes) / BYTES_IN_DWORD)

/* extra lines include a signature line + optional latency events line */
#define NUM_EXTRA_DBG_LINES(block) \
	(GET_FIELD((block)->flags, DBG_BLOCK_CHIP_HAS_LATENCY_EVENTS) ? 2 : 1)
#define NUM_DBG_LINES(block) \
	((block)->num_of_dbg_bus_lines + NUM_EXTRA_DBG_LINES(block))

#define USE_DMAE			true
#define PROTECT_WIDE_BUS		true
>>>>>>> upstream/android-13

#define RAM_LINES_TO_DWORDS(lines)	((lines) * 2)
#define RAM_LINES_TO_BYTES(lines) \
	DWORDS_TO_BYTES(RAM_LINES_TO_DWORDS(lines))

#define REG_DUMP_LEN_SHIFT		24
#define MEM_DUMP_ENTRY_SIZE_DWORDS \
	BYTES_TO_DWORDS(sizeof(struct dbg_dump_mem))

#define IDLE_CHK_RULE_SIZE_DWORDS \
	BYTES_TO_DWORDS(sizeof(struct dbg_idle_chk_rule))

#define IDLE_CHK_RESULT_HDR_DWORDS \
	BYTES_TO_DWORDS(sizeof(struct dbg_idle_chk_result_hdr))

#define IDLE_CHK_RESULT_REG_HDR_DWORDS \
	BYTES_TO_DWORDS(sizeof(struct dbg_idle_chk_result_reg_hdr))

<<<<<<< HEAD
=======
#define PAGE_MEM_DESC_SIZE_DWORDS \
	BYTES_TO_DWORDS(sizeof(struct phys_mem_desc))

>>>>>>> upstream/android-13
#define IDLE_CHK_MAX_ENTRIES_SIZE	32

/* The sizes and offsets below are specified in bits */
#define VFC_CAM_CMD_STRUCT_SIZE		64
#define VFC_CAM_CMD_ROW_OFFSET		48
#define VFC_CAM_CMD_ROW_SIZE		9
#define VFC_CAM_ADDR_STRUCT_SIZE	16
#define VFC_CAM_ADDR_OP_OFFSET		0
#define VFC_CAM_ADDR_OP_SIZE		4
#define VFC_CAM_RESP_STRUCT_SIZE	256
#define VFC_RAM_ADDR_STRUCT_SIZE	16
#define VFC_RAM_ADDR_OP_OFFSET		0
#define VFC_RAM_ADDR_OP_SIZE		2
#define VFC_RAM_ADDR_ROW_OFFSET		2
#define VFC_RAM_ADDR_ROW_SIZE		10
#define VFC_RAM_RESP_STRUCT_SIZE	256

#define VFC_CAM_CMD_DWORDS		CEIL_DWORDS(VFC_CAM_CMD_STRUCT_SIZE)
#define VFC_CAM_ADDR_DWORDS		CEIL_DWORDS(VFC_CAM_ADDR_STRUCT_SIZE)
#define VFC_CAM_RESP_DWORDS		CEIL_DWORDS(VFC_CAM_RESP_STRUCT_SIZE)
#define VFC_RAM_CMD_DWORDS		VFC_CAM_CMD_DWORDS
#define VFC_RAM_ADDR_DWORDS		CEIL_DWORDS(VFC_RAM_ADDR_STRUCT_SIZE)
#define VFC_RAM_RESP_DWORDS		CEIL_DWORDS(VFC_RAM_RESP_STRUCT_SIZE)

#define NUM_VFC_RAM_TYPES		4

#define VFC_CAM_NUM_ROWS		512

#define VFC_OPCODE_CAM_RD		14
#define VFC_OPCODE_RAM_RD		0

#define NUM_RSS_MEM_TYPES		5

#define NUM_BIG_RAM_TYPES		3
#define BIG_RAM_NAME_LEN		3

#define NUM_PHY_TBUS_ADDRESSES		2048
#define PHY_DUMP_SIZE_DWORDS		(NUM_PHY_TBUS_ADDRESSES / 2)

#define RESET_REG_UNRESET_OFFSET	4

#define STALL_DELAY_MS			500

#define STATIC_DEBUG_LINE_DWORDS	9

<<<<<<< HEAD
#define NUM_COMMON_GLOBAL_PARAMS	8
=======
#define NUM_COMMON_GLOBAL_PARAMS	9

#define MAX_RECURSION_DEPTH		10
>>>>>>> upstream/android-13

#define FW_IMG_MAIN			1

#define REG_FIFO_ELEMENT_DWORDS		2
#define REG_FIFO_DEPTH_ELEMENTS		32
#define REG_FIFO_DEPTH_DWORDS \
	(REG_FIFO_ELEMENT_DWORDS * REG_FIFO_DEPTH_ELEMENTS)

#define IGU_FIFO_ELEMENT_DWORDS		4
#define IGU_FIFO_DEPTH_ELEMENTS		64
#define IGU_FIFO_DEPTH_DWORDS \
	(IGU_FIFO_ELEMENT_DWORDS * IGU_FIFO_DEPTH_ELEMENTS)

#define PROTECTION_OVERRIDE_ELEMENT_DWORDS	2
#define PROTECTION_OVERRIDE_DEPTH_ELEMENTS	20
#define PROTECTION_OVERRIDE_DEPTH_DWORDS \
	(PROTECTION_OVERRIDE_DEPTH_ELEMENTS * \
	 PROTECTION_OVERRIDE_ELEMENT_DWORDS)

#define MCP_SPAD_TRACE_OFFSIZE_ADDR \
	(MCP_REG_SCRATCH + \
	 offsetof(struct static_init, sections[SPAD_SECTION_TRACE]))

<<<<<<< HEAD
=======
#define MAX_SW_PLTAFORM_STR_SIZE	64

>>>>>>> upstream/android-13
#define EMPTY_FW_VERSION_STR		"???_???_???_???"
#define EMPTY_FW_IMAGE_STR		"???????????????"

/***************************** Constant Arrays *******************************/

<<<<<<< HEAD
struct dbg_array {
	const u32 *ptr;
	u32 size_in_dwords;
};

/* Debug arrays */
static struct dbg_array s_dbg_arrays[MAX_BIN_DBG_BUFFER_TYPE] = { {NULL} };

/* Chip constant definitions array */
static struct chip_defs s_chip_defs[MAX_CHIP_IDS] = {
	{"bb"},
	{"ah"},
	{"reserved"},
=======
/* Chip constant definitions array */
static struct chip_defs s_chip_defs[MAX_CHIP_IDS] = {
	{"bb", PSWRQ2_REG_ILT_MEMORY_SIZE_BB / 2},
	{"ah", PSWRQ2_REG_ILT_MEMORY_SIZE_K2 / 2}
>>>>>>> upstream/android-13
};

/* Storm constant definitions array */
static struct storm_defs s_storm_defs[] = {
	/* Tstorm */
	{'T', BLOCK_TSEM,
<<<<<<< HEAD
	 {DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCT,
	  DBG_BUS_CLIENT_RBCT}, true,
	 TSEM_REG_FAST_MEMORY,
	 TSEM_REG_DBG_FRAME_MODE_BB_K2, TSEM_REG_SLOW_DBG_ACTIVE_BB_K2,
	 TSEM_REG_SLOW_DBG_MODE_BB_K2, TSEM_REG_DBG_MODE1_CFG_BB_K2,
	 TSEM_REG_SYNC_DBG_EMPTY, TSEM_REG_SLOW_DBG_EMPTY_BB_K2,
	 TCM_REG_CTX_RBC_ACCS,
	 4, TCM_REG_AGG_CON_CTX,
	 16, TCM_REG_SM_CON_CTX,
	 2, TCM_REG_AGG_TASK_CTX,
	 4, TCM_REG_SM_TASK_CTX},

	/* Mstorm */
	{'M', BLOCK_MSEM,
	 {DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCM,
	  DBG_BUS_CLIENT_RBCM}, false,
	 MSEM_REG_FAST_MEMORY,
	 MSEM_REG_DBG_FRAME_MODE_BB_K2, MSEM_REG_SLOW_DBG_ACTIVE_BB_K2,
	 MSEM_REG_SLOW_DBG_MODE_BB_K2, MSEM_REG_DBG_MODE1_CFG_BB_K2,
	 MSEM_REG_SYNC_DBG_EMPTY, MSEM_REG_SLOW_DBG_EMPTY_BB_K2,
	 MCM_REG_CTX_RBC_ACCS,
	 1, MCM_REG_AGG_CON_CTX,
	 10, MCM_REG_SM_CON_CTX,
	 2, MCM_REG_AGG_TASK_CTX,
	 7, MCM_REG_SM_TASK_CTX},

	/* Ustorm */
	{'U', BLOCK_USEM,
	 {DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU,
	  DBG_BUS_CLIENT_RBCU}, false,
	 USEM_REG_FAST_MEMORY,
	 USEM_REG_DBG_FRAME_MODE_BB_K2, USEM_REG_SLOW_DBG_ACTIVE_BB_K2,
	 USEM_REG_SLOW_DBG_MODE_BB_K2, USEM_REG_DBG_MODE1_CFG_BB_K2,
	 USEM_REG_SYNC_DBG_EMPTY, USEM_REG_SLOW_DBG_EMPTY_BB_K2,
	 UCM_REG_CTX_RBC_ACCS,
	 2, UCM_REG_AGG_CON_CTX,
	 13, UCM_REG_SM_CON_CTX,
	 3, UCM_REG_AGG_TASK_CTX,
	 3, UCM_REG_SM_TASK_CTX},

	/* Xstorm */
	{'X', BLOCK_XSEM,
	 {DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCX,
	  DBG_BUS_CLIENT_RBCX}, false,
	 XSEM_REG_FAST_MEMORY,
	 XSEM_REG_DBG_FRAME_MODE_BB_K2, XSEM_REG_SLOW_DBG_ACTIVE_BB_K2,
	 XSEM_REG_SLOW_DBG_MODE_BB_K2, XSEM_REG_DBG_MODE1_CFG_BB_K2,
	 XSEM_REG_SYNC_DBG_EMPTY, XSEM_REG_SLOW_DBG_EMPTY_BB_K2,
	 XCM_REG_CTX_RBC_ACCS,
	 9, XCM_REG_AGG_CON_CTX,
	 15, XCM_REG_SM_CON_CTX,
	 0, 0,
	 0, 0},

	/* Ystorm */
	{'Y', BLOCK_YSEM,
	 {DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCY,
	  DBG_BUS_CLIENT_RBCY}, false,
	 YSEM_REG_FAST_MEMORY,
	 YSEM_REG_DBG_FRAME_MODE_BB_K2, YSEM_REG_SLOW_DBG_ACTIVE_BB_K2,
	 YSEM_REG_SLOW_DBG_MODE_BB_K2, YSEM_REG_DBG_MODE1_CFG_BB_K2,
	 YSEM_REG_SYNC_DBG_EMPTY, TSEM_REG_SLOW_DBG_EMPTY_BB_K2,
	 YCM_REG_CTX_RBC_ACCS,
	 2, YCM_REG_AGG_CON_CTX,
	 3, YCM_REG_SM_CON_CTX,
	 2, YCM_REG_AGG_TASK_CTX,
	 12, YCM_REG_SM_TASK_CTX},

	/* Pstorm */
	{'P', BLOCK_PSEM,
	 {DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS,
	  DBG_BUS_CLIENT_RBCS}, true,
	 PSEM_REG_FAST_MEMORY,
	 PSEM_REG_DBG_FRAME_MODE_BB_K2, PSEM_REG_SLOW_DBG_ACTIVE_BB_K2,
	 PSEM_REG_SLOW_DBG_MODE_BB_K2, PSEM_REG_DBG_MODE1_CFG_BB_K2,
	 PSEM_REG_SYNC_DBG_EMPTY, PSEM_REG_SLOW_DBG_EMPTY_BB_K2,
	 PCM_REG_CTX_RBC_ACCS,
	 0, 0,
	 10, PCM_REG_SM_CON_CTX,
	 0, 0,
	 0, 0}
};

/* Block definitions array */

static struct block_defs block_grc_defs = {
	"grc",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCN, DBG_BUS_CLIENT_RBCN, DBG_BUS_CLIENT_RBCN},
	GRC_REG_DBG_SELECT, GRC_REG_DBG_DWORD_ENABLE,
	GRC_REG_DBG_SHIFT, GRC_REG_DBG_FORCE_VALID,
	GRC_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISC_PL_UA, 1
};

static struct block_defs block_miscs_defs = {
	"miscs", {true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_misc_defs = {
	"misc", {true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_dbu_defs = {
	"dbu", {true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_pglue_b_defs = {
	"pglue_b",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCH, DBG_BUS_CLIENT_RBCH, DBG_BUS_CLIENT_RBCH},
	PGLUE_B_REG_DBG_SELECT, PGLUE_B_REG_DBG_DWORD_ENABLE,
	PGLUE_B_REG_DBG_SHIFT, PGLUE_B_REG_DBG_FORCE_VALID,
	PGLUE_B_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISCS_PL_HV, 1
};

static struct block_defs block_cnig_defs = {
	"cnig",
	{true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCW,
	 DBG_BUS_CLIENT_RBCW},
	CNIG_REG_DBG_SELECT_K2_E5, CNIG_REG_DBG_DWORD_ENABLE_K2_E5,
	CNIG_REG_DBG_SHIFT_K2_E5, CNIG_REG_DBG_FORCE_VALID_K2_E5,
	CNIG_REG_DBG_FORCE_FRAME_K2_E5,
	true, false, DBG_RESET_REG_MISCS_PL_HV, 0
};

static struct block_defs block_cpmu_defs = {
	"cpmu", {true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	true, false, DBG_RESET_REG_MISCS_PL_HV, 8
};

static struct block_defs block_ncsi_defs = {
	"ncsi",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCZ, DBG_BUS_CLIENT_RBCZ, DBG_BUS_CLIENT_RBCZ},
	NCSI_REG_DBG_SELECT, NCSI_REG_DBG_DWORD_ENABLE,
	NCSI_REG_DBG_SHIFT, NCSI_REG_DBG_FORCE_VALID,
	NCSI_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISCS_PL_HV, 5
};

static struct block_defs block_opte_defs = {
	"opte", {true, true, false}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	true, false, DBG_RESET_REG_MISCS_PL_HV, 4
};

static struct block_defs block_bmb_defs = {
	"bmb",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCZ, DBG_BUS_CLIENT_RBCB, DBG_BUS_CLIENT_RBCB},
	BMB_REG_DBG_SELECT, BMB_REG_DBG_DWORD_ENABLE,
	BMB_REG_DBG_SHIFT, BMB_REG_DBG_FORCE_VALID,
	BMB_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISCS_PL_UA, 7
};

static struct block_defs block_pcie_defs = {
	"pcie",
	{true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCH,
	 DBG_BUS_CLIENT_RBCH},
	PCIE_REG_DBG_COMMON_SELECT_K2_E5,
	PCIE_REG_DBG_COMMON_DWORD_ENABLE_K2_E5,
	PCIE_REG_DBG_COMMON_SHIFT_K2_E5,
	PCIE_REG_DBG_COMMON_FORCE_VALID_K2_E5,
	PCIE_REG_DBG_COMMON_FORCE_FRAME_K2_E5,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_mcp_defs = {
	"mcp", {true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_mcp2_defs = {
	"mcp2",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCZ, DBG_BUS_CLIENT_RBCZ, DBG_BUS_CLIENT_RBCZ},
	MCP2_REG_DBG_SELECT, MCP2_REG_DBG_DWORD_ENABLE,
	MCP2_REG_DBG_SHIFT, MCP2_REG_DBG_FORCE_VALID,
	MCP2_REG_DBG_FORCE_FRAME,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_pswhst_defs = {
	"pswhst",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	PSWHST_REG_DBG_SELECT, PSWHST_REG_DBG_DWORD_ENABLE,
	PSWHST_REG_DBG_SHIFT, PSWHST_REG_DBG_FORCE_VALID,
	PSWHST_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISC_PL_HV, 0
};

static struct block_defs block_pswhst2_defs = {
	"pswhst2",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	PSWHST2_REG_DBG_SELECT, PSWHST2_REG_DBG_DWORD_ENABLE,
	PSWHST2_REG_DBG_SHIFT, PSWHST2_REG_DBG_FORCE_VALID,
	PSWHST2_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISC_PL_HV, 0
};

static struct block_defs block_pswrd_defs = {
	"pswrd",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	PSWRD_REG_DBG_SELECT, PSWRD_REG_DBG_DWORD_ENABLE,
	PSWRD_REG_DBG_SHIFT, PSWRD_REG_DBG_FORCE_VALID,
	PSWRD_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISC_PL_HV, 2
};

static struct block_defs block_pswrd2_defs = {
	"pswrd2",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	PSWRD2_REG_DBG_SELECT, PSWRD2_REG_DBG_DWORD_ENABLE,
	PSWRD2_REG_DBG_SHIFT, PSWRD2_REG_DBG_FORCE_VALID,
	PSWRD2_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISC_PL_HV, 2
};

static struct block_defs block_pswwr_defs = {
	"pswwr",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	PSWWR_REG_DBG_SELECT, PSWWR_REG_DBG_DWORD_ENABLE,
	PSWWR_REG_DBG_SHIFT, PSWWR_REG_DBG_FORCE_VALID,
	PSWWR_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISC_PL_HV, 3
};

static struct block_defs block_pswwr2_defs = {
	"pswwr2", {true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	true, false, DBG_RESET_REG_MISC_PL_HV, 3
};

static struct block_defs block_pswrq_defs = {
	"pswrq",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	PSWRQ_REG_DBG_SELECT, PSWRQ_REG_DBG_DWORD_ENABLE,
	PSWRQ_REG_DBG_SHIFT, PSWRQ_REG_DBG_FORCE_VALID,
	PSWRQ_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISC_PL_HV, 1
};

static struct block_defs block_pswrq2_defs = {
	"pswrq2",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	PSWRQ2_REG_DBG_SELECT, PSWRQ2_REG_DBG_DWORD_ENABLE,
	PSWRQ2_REG_DBG_SHIFT, PSWRQ2_REG_DBG_FORCE_VALID,
	PSWRQ2_REG_DBG_FORCE_FRAME,
	true, false, DBG_RESET_REG_MISC_PL_HV, 1
};

static struct block_defs block_pglcs_defs = {
	"pglcs",
	{true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCH,
	 DBG_BUS_CLIENT_RBCH},
	PGLCS_REG_DBG_SELECT_K2_E5, PGLCS_REG_DBG_DWORD_ENABLE_K2_E5,
	PGLCS_REG_DBG_SHIFT_K2_E5, PGLCS_REG_DBG_FORCE_VALID_K2_E5,
	PGLCS_REG_DBG_FORCE_FRAME_K2_E5,
	true, false, DBG_RESET_REG_MISCS_PL_HV, 2
};

static struct block_defs block_ptu_defs = {
	"ptu",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	PTU_REG_DBG_SELECT, PTU_REG_DBG_DWORD_ENABLE,
	PTU_REG_DBG_SHIFT, PTU_REG_DBG_FORCE_VALID,
	PTU_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 20
};

static struct block_defs block_dmae_defs = {
	"dmae",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	DMAE_REG_DBG_SELECT, DMAE_REG_DBG_DWORD_ENABLE,
	DMAE_REG_DBG_SHIFT, DMAE_REG_DBG_FORCE_VALID,
	DMAE_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 28
};

static struct block_defs block_tcm_defs = {
	"tcm",
	{true, true, true}, true, DBG_TSTORM_ID,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCT},
	TCM_REG_DBG_SELECT, TCM_REG_DBG_DWORD_ENABLE,
	TCM_REG_DBG_SHIFT, TCM_REG_DBG_FORCE_VALID,
	TCM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 5
};

static struct block_defs block_mcm_defs = {
	"mcm",
	{true, true, true}, true, DBG_MSTORM_ID,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCM, DBG_BUS_CLIENT_RBCM},
	MCM_REG_DBG_SELECT, MCM_REG_DBG_DWORD_ENABLE,
	MCM_REG_DBG_SHIFT, MCM_REG_DBG_FORCE_VALID,
	MCM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 3
};

static struct block_defs block_ucm_defs = {
	"ucm",
	{true, true, true}, true, DBG_USTORM_ID,
	{DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU},
	UCM_REG_DBG_SELECT, UCM_REG_DBG_DWORD_ENABLE,
	UCM_REG_DBG_SHIFT, UCM_REG_DBG_FORCE_VALID,
	UCM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 8
};

static struct block_defs block_xcm_defs = {
	"xcm",
	{true, true, true}, true, DBG_XSTORM_ID,
	{DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCX},
	XCM_REG_DBG_SELECT, XCM_REG_DBG_DWORD_ENABLE,
	XCM_REG_DBG_SHIFT, XCM_REG_DBG_FORCE_VALID,
	XCM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 19
};

static struct block_defs block_ycm_defs = {
	"ycm",
	{true, true, true}, true, DBG_YSTORM_ID,
	{DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCY, DBG_BUS_CLIENT_RBCY},
	YCM_REG_DBG_SELECT, YCM_REG_DBG_DWORD_ENABLE,
	YCM_REG_DBG_SHIFT, YCM_REG_DBG_FORCE_VALID,
	YCM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 5
};

static struct block_defs block_pcm_defs = {
	"pcm",
	{true, true, true}, true, DBG_PSTORM_ID,
	{DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS},
	PCM_REG_DBG_SELECT, PCM_REG_DBG_DWORD_ENABLE,
	PCM_REG_DBG_SHIFT, PCM_REG_DBG_FORCE_VALID,
	PCM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 4
};

static struct block_defs block_qm_defs = {
	"qm",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCQ, DBG_BUS_CLIENT_RBCQ},
	QM_REG_DBG_SELECT, QM_REG_DBG_DWORD_ENABLE,
	QM_REG_DBG_SHIFT, QM_REG_DBG_FORCE_VALID,
	QM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 16
};

static struct block_defs block_tm_defs = {
	"tm",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS},
	TM_REG_DBG_SELECT, TM_REG_DBG_DWORD_ENABLE,
	TM_REG_DBG_SHIFT, TM_REG_DBG_FORCE_VALID,
	TM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 17
};

static struct block_defs block_dorq_defs = {
	"dorq",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCY, DBG_BUS_CLIENT_RBCY},
	DORQ_REG_DBG_SELECT, DORQ_REG_DBG_DWORD_ENABLE,
	DORQ_REG_DBG_SHIFT, DORQ_REG_DBG_FORCE_VALID,
	DORQ_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 18
};

static struct block_defs block_brb_defs = {
	"brb",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCR, DBG_BUS_CLIENT_RBCR, DBG_BUS_CLIENT_RBCR},
	BRB_REG_DBG_SELECT, BRB_REG_DBG_DWORD_ENABLE,
	BRB_REG_DBG_SHIFT, BRB_REG_DBG_FORCE_VALID,
	BRB_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 0
};

static struct block_defs block_src_defs = {
	"src",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCF, DBG_BUS_CLIENT_RBCF, DBG_BUS_CLIENT_RBCF},
	SRC_REG_DBG_SELECT, SRC_REG_DBG_DWORD_ENABLE,
	SRC_REG_DBG_SHIFT, SRC_REG_DBG_FORCE_VALID,
	SRC_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 2
};

static struct block_defs block_prs_defs = {
	"prs",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCR, DBG_BUS_CLIENT_RBCR, DBG_BUS_CLIENT_RBCR},
	PRS_REG_DBG_SELECT, PRS_REG_DBG_DWORD_ENABLE,
	PRS_REG_DBG_SHIFT, PRS_REG_DBG_FORCE_VALID,
	PRS_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 1
};

static struct block_defs block_tsdm_defs = {
	"tsdm",
	{true, true, true}, true, DBG_TSTORM_ID,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCT},
	TSDM_REG_DBG_SELECT, TSDM_REG_DBG_DWORD_ENABLE,
	TSDM_REG_DBG_SHIFT, TSDM_REG_DBG_FORCE_VALID,
	TSDM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 3
};

static struct block_defs block_msdm_defs = {
	"msdm",
	{true, true, true}, true, DBG_MSTORM_ID,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCM, DBG_BUS_CLIENT_RBCM},
	MSDM_REG_DBG_SELECT, MSDM_REG_DBG_DWORD_ENABLE,
	MSDM_REG_DBG_SHIFT, MSDM_REG_DBG_FORCE_VALID,
	MSDM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 6
};

static struct block_defs block_usdm_defs = {
	"usdm",
	{true, true, true}, true, DBG_USTORM_ID,
	{DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU},
	USDM_REG_DBG_SELECT, USDM_REG_DBG_DWORD_ENABLE,
	USDM_REG_DBG_SHIFT, USDM_REG_DBG_FORCE_VALID,
	USDM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 7
};

static struct block_defs block_xsdm_defs = {
	"xsdm",
	{true, true, true}, true, DBG_XSTORM_ID,
	{DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCX},
	XSDM_REG_DBG_SELECT, XSDM_REG_DBG_DWORD_ENABLE,
	XSDM_REG_DBG_SHIFT, XSDM_REG_DBG_FORCE_VALID,
	XSDM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 20
};

static struct block_defs block_ysdm_defs = {
	"ysdm",
	{true, true, true}, true, DBG_YSTORM_ID,
	{DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCY, DBG_BUS_CLIENT_RBCY},
	YSDM_REG_DBG_SELECT, YSDM_REG_DBG_DWORD_ENABLE,
	YSDM_REG_DBG_SHIFT, YSDM_REG_DBG_FORCE_VALID,
	YSDM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 8
};

static struct block_defs block_psdm_defs = {
	"psdm",
	{true, true, true}, true, DBG_PSTORM_ID,
	{DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS},
	PSDM_REG_DBG_SELECT, PSDM_REG_DBG_DWORD_ENABLE,
	PSDM_REG_DBG_SHIFT, PSDM_REG_DBG_FORCE_VALID,
	PSDM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 7
};

static struct block_defs block_tsem_defs = {
	"tsem",
	{true, true, true}, true, DBG_TSTORM_ID,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCT},
	TSEM_REG_DBG_SELECT, TSEM_REG_DBG_DWORD_ENABLE,
	TSEM_REG_DBG_SHIFT, TSEM_REG_DBG_FORCE_VALID,
	TSEM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 4
};

static struct block_defs block_msem_defs = {
	"msem",
	{true, true, true}, true, DBG_MSTORM_ID,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCM, DBG_BUS_CLIENT_RBCM},
	MSEM_REG_DBG_SELECT, MSEM_REG_DBG_DWORD_ENABLE,
	MSEM_REG_DBG_SHIFT, MSEM_REG_DBG_FORCE_VALID,
	MSEM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 9
};

static struct block_defs block_usem_defs = {
	"usem",
	{true, true, true}, true, DBG_USTORM_ID,
	{DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU},
	USEM_REG_DBG_SELECT, USEM_REG_DBG_DWORD_ENABLE,
	USEM_REG_DBG_SHIFT, USEM_REG_DBG_FORCE_VALID,
	USEM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 9
};

static struct block_defs block_xsem_defs = {
	"xsem",
	{true, true, true}, true, DBG_XSTORM_ID,
	{DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCX},
	XSEM_REG_DBG_SELECT, XSEM_REG_DBG_DWORD_ENABLE,
	XSEM_REG_DBG_SHIFT, XSEM_REG_DBG_FORCE_VALID,
	XSEM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 21
};

static struct block_defs block_ysem_defs = {
	"ysem",
	{true, true, true}, true, DBG_YSTORM_ID,
	{DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCY, DBG_BUS_CLIENT_RBCY},
	YSEM_REG_DBG_SELECT, YSEM_REG_DBG_DWORD_ENABLE,
	YSEM_REG_DBG_SHIFT, YSEM_REG_DBG_FORCE_VALID,
	YSEM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 11
};

static struct block_defs block_psem_defs = {
	"psem",
	{true, true, true}, true, DBG_PSTORM_ID,
	{DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS},
	PSEM_REG_DBG_SELECT, PSEM_REG_DBG_DWORD_ENABLE,
	PSEM_REG_DBG_SHIFT, PSEM_REG_DBG_FORCE_VALID,
	PSEM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 10
};

static struct block_defs block_rss_defs = {
	"rss",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCT},
	RSS_REG_DBG_SELECT, RSS_REG_DBG_DWORD_ENABLE,
	RSS_REG_DBG_SHIFT, RSS_REG_DBG_FORCE_VALID,
	RSS_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 18
};

static struct block_defs block_tmld_defs = {
	"tmld",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCM, DBG_BUS_CLIENT_RBCM},
	TMLD_REG_DBG_SELECT, TMLD_REG_DBG_DWORD_ENABLE,
	TMLD_REG_DBG_SHIFT, TMLD_REG_DBG_FORCE_VALID,
	TMLD_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 13
};

static struct block_defs block_muld_defs = {
	"muld",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU},
	MULD_REG_DBG_SELECT, MULD_REG_DBG_DWORD_ENABLE,
	MULD_REG_DBG_SHIFT, MULD_REG_DBG_FORCE_VALID,
	MULD_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 14
};

static struct block_defs block_yuld_defs = {
	"yuld",
	{true, true, false}, false, 0,
	{DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU,
	 MAX_DBG_BUS_CLIENTS},
	YULD_REG_DBG_SELECT_BB_K2, YULD_REG_DBG_DWORD_ENABLE_BB_K2,
	YULD_REG_DBG_SHIFT_BB_K2, YULD_REG_DBG_FORCE_VALID_BB_K2,
	YULD_REG_DBG_FORCE_FRAME_BB_K2,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2,
	15
};

static struct block_defs block_xyld_defs = {
	"xyld",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCX},
	XYLD_REG_DBG_SELECT, XYLD_REG_DBG_DWORD_ENABLE,
	XYLD_REG_DBG_SHIFT, XYLD_REG_DBG_FORCE_VALID,
	XYLD_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 12
};

static struct block_defs block_ptld_defs = {
	"ptld",
	{false, false, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCT},
	PTLD_REG_DBG_SELECT_E5, PTLD_REG_DBG_DWORD_ENABLE_E5,
	PTLD_REG_DBG_SHIFT_E5, PTLD_REG_DBG_FORCE_VALID_E5,
	PTLD_REG_DBG_FORCE_FRAME_E5,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2,
	28
};

static struct block_defs block_ypld_defs = {
	"ypld",
	{false, false, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCS},
	YPLD_REG_DBG_SELECT_E5, YPLD_REG_DBG_DWORD_ENABLE_E5,
	YPLD_REG_DBG_SHIFT_E5, YPLD_REG_DBG_FORCE_VALID_E5,
	YPLD_REG_DBG_FORCE_FRAME_E5,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2,
	27
};

static struct block_defs block_prm_defs = {
	"prm",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCM, DBG_BUS_CLIENT_RBCM},
	PRM_REG_DBG_SELECT, PRM_REG_DBG_DWORD_ENABLE,
	PRM_REG_DBG_SHIFT, PRM_REG_DBG_FORCE_VALID,
	PRM_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 21
};

static struct block_defs block_pbf_pb1_defs = {
	"pbf_pb1",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCV, DBG_BUS_CLIENT_RBCV},
	PBF_PB1_REG_DBG_SELECT, PBF_PB1_REG_DBG_DWORD_ENABLE,
	PBF_PB1_REG_DBG_SHIFT, PBF_PB1_REG_DBG_FORCE_VALID,
	PBF_PB1_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1,
	11
};

static struct block_defs block_pbf_pb2_defs = {
	"pbf_pb2",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCV, DBG_BUS_CLIENT_RBCV},
	PBF_PB2_REG_DBG_SELECT, PBF_PB2_REG_DBG_DWORD_ENABLE,
	PBF_PB2_REG_DBG_SHIFT, PBF_PB2_REG_DBG_FORCE_VALID,
	PBF_PB2_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1,
	12
};

static struct block_defs block_rpb_defs = {
	"rpb",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCM, DBG_BUS_CLIENT_RBCM},
	RPB_REG_DBG_SELECT, RPB_REG_DBG_DWORD_ENABLE,
	RPB_REG_DBG_SHIFT, RPB_REG_DBG_FORCE_VALID,
	RPB_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 13
};

static struct block_defs block_btb_defs = {
	"btb",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCR, DBG_BUS_CLIENT_RBCV, DBG_BUS_CLIENT_RBCV},
	BTB_REG_DBG_SELECT, BTB_REG_DBG_DWORD_ENABLE,
	BTB_REG_DBG_SHIFT, BTB_REG_DBG_FORCE_VALID,
	BTB_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 10
};

static struct block_defs block_pbf_defs = {
	"pbf",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCV, DBG_BUS_CLIENT_RBCV},
	PBF_REG_DBG_SELECT, PBF_REG_DBG_DWORD_ENABLE,
	PBF_REG_DBG_SHIFT, PBF_REG_DBG_FORCE_VALID,
	PBF_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 15
};

static struct block_defs block_rdif_defs = {
	"rdif",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCM, DBG_BUS_CLIENT_RBCM},
	RDIF_REG_DBG_SELECT, RDIF_REG_DBG_DWORD_ENABLE,
	RDIF_REG_DBG_SHIFT, RDIF_REG_DBG_FORCE_VALID,
	RDIF_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 16
};

static struct block_defs block_tdif_defs = {
	"tdif",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS},
	TDIF_REG_DBG_SELECT, TDIF_REG_DBG_DWORD_ENABLE,
	TDIF_REG_DBG_SHIFT, TDIF_REG_DBG_FORCE_VALID,
	TDIF_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 17
};

static struct block_defs block_cdu_defs = {
	"cdu",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCF, DBG_BUS_CLIENT_RBCF, DBG_BUS_CLIENT_RBCF},
	CDU_REG_DBG_SELECT, CDU_REG_DBG_DWORD_ENABLE,
	CDU_REG_DBG_SHIFT, CDU_REG_DBG_FORCE_VALID,
	CDU_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 23
};

static struct block_defs block_ccfc_defs = {
	"ccfc",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCF, DBG_BUS_CLIENT_RBCF, DBG_BUS_CLIENT_RBCF},
	CCFC_REG_DBG_SELECT, CCFC_REG_DBG_DWORD_ENABLE,
	CCFC_REG_DBG_SHIFT, CCFC_REG_DBG_FORCE_VALID,
	CCFC_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 24
};

static struct block_defs block_tcfc_defs = {
	"tcfc",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCF, DBG_BUS_CLIENT_RBCF, DBG_BUS_CLIENT_RBCF},
	TCFC_REG_DBG_SELECT, TCFC_REG_DBG_DWORD_ENABLE,
	TCFC_REG_DBG_SHIFT, TCFC_REG_DBG_FORCE_VALID,
	TCFC_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 25
};

static struct block_defs block_igu_defs = {
	"igu",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	IGU_REG_DBG_SELECT, IGU_REG_DBG_DWORD_ENABLE,
	IGU_REG_DBG_SHIFT, IGU_REG_DBG_FORCE_VALID,
	IGU_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1, 27
};

static struct block_defs block_cau_defs = {
	"cau",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP, DBG_BUS_CLIENT_RBCP},
	CAU_REG_DBG_SELECT, CAU_REG_DBG_DWORD_ENABLE,
	CAU_REG_DBG_SHIFT, CAU_REG_DBG_FORCE_VALID,
	CAU_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 19
};

static struct block_defs block_rgfs_defs = {
	"rgfs", {false, false, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 29
};

static struct block_defs block_rgsrc_defs = {
	"rgsrc",
	{false, false, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCH},
	RGSRC_REG_DBG_SELECT_E5, RGSRC_REG_DBG_DWORD_ENABLE_E5,
	RGSRC_REG_DBG_SHIFT_E5, RGSRC_REG_DBG_FORCE_VALID_E5,
	RGSRC_REG_DBG_FORCE_FRAME_E5,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1,
	30
};

static struct block_defs block_tgfs_defs = {
	"tgfs", {false, false, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_2, 30
};

static struct block_defs block_tgsrc_defs = {
	"tgsrc",
	{false, false, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCV},
	TGSRC_REG_DBG_SELECT_E5, TGSRC_REG_DBG_DWORD_ENABLE_E5,
	TGSRC_REG_DBG_SHIFT_E5, TGSRC_REG_DBG_FORCE_VALID_E5,
	TGSRC_REG_DBG_FORCE_FRAME_E5,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VMAIN_1,
	31
};

static struct block_defs block_umac_defs = {
	"umac",
	{true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCZ,
	 DBG_BUS_CLIENT_RBCZ},
	UMAC_REG_DBG_SELECT_K2_E5, UMAC_REG_DBG_DWORD_ENABLE_K2_E5,
	UMAC_REG_DBG_SHIFT_K2_E5, UMAC_REG_DBG_FORCE_VALID_K2_E5,
	UMAC_REG_DBG_FORCE_FRAME_K2_E5,
	true, false, DBG_RESET_REG_MISCS_PL_HV, 6
};

static struct block_defs block_xmac_defs = {
	"xmac", {true, false, false}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_dbg_defs = {
	"dbg", {true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VAUX, 3
};

static struct block_defs block_nig_defs = {
	"nig",
	{true, true, true}, false, 0,
	{DBG_BUS_CLIENT_RBCN, DBG_BUS_CLIENT_RBCN, DBG_BUS_CLIENT_RBCN},
	NIG_REG_DBG_SELECT, NIG_REG_DBG_DWORD_ENABLE,
	NIG_REG_DBG_SHIFT, NIG_REG_DBG_FORCE_VALID,
	NIG_REG_DBG_FORCE_FRAME,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VAUX, 0
};

static struct block_defs block_wol_defs = {
	"wol",
	{false, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCZ, DBG_BUS_CLIENT_RBCZ},
	WOL_REG_DBG_SELECT_K2_E5, WOL_REG_DBG_DWORD_ENABLE_K2_E5,
	WOL_REG_DBG_SHIFT_K2_E5, WOL_REG_DBG_FORCE_VALID_K2_E5,
	WOL_REG_DBG_FORCE_FRAME_K2_E5,
	true, true, DBG_RESET_REG_MISC_PL_PDA_VAUX, 7
};

static struct block_defs block_bmbn_defs = {
	"bmbn",
	{false, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCB,
	 DBG_BUS_CLIENT_RBCB},
	BMBN_REG_DBG_SELECT_K2_E5, BMBN_REG_DBG_DWORD_ENABLE_K2_E5,
	BMBN_REG_DBG_SHIFT_K2_E5, BMBN_REG_DBG_FORCE_VALID_K2_E5,
	BMBN_REG_DBG_FORCE_FRAME_K2_E5,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_ipc_defs = {
	"ipc", {true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	true, false, DBG_RESET_REG_MISCS_PL_UA, 8
};

static struct block_defs block_nwm_defs = {
	"nwm",
	{false, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCW, DBG_BUS_CLIENT_RBCW},
	NWM_REG_DBG_SELECT_K2_E5, NWM_REG_DBG_DWORD_ENABLE_K2_E5,
	NWM_REG_DBG_SHIFT_K2_E5, NWM_REG_DBG_FORCE_VALID_K2_E5,
	NWM_REG_DBG_FORCE_FRAME_K2_E5,
	true, false, DBG_RESET_REG_MISCS_PL_HV_2, 0
};

static struct block_defs block_nws_defs = {
	"nws",
	{false, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCW, DBG_BUS_CLIENT_RBCW},
	NWS_REG_DBG_SELECT_K2_E5, NWS_REG_DBG_DWORD_ENABLE_K2_E5,
	NWS_REG_DBG_SHIFT_K2_E5, NWS_REG_DBG_FORCE_VALID_K2_E5,
	NWS_REG_DBG_FORCE_FRAME_K2_E5,
	true, false, DBG_RESET_REG_MISCS_PL_HV, 12
};

static struct block_defs block_ms_defs = {
	"ms",
	{false, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCZ, DBG_BUS_CLIENT_RBCZ},
	MS_REG_DBG_SELECT_K2_E5, MS_REG_DBG_DWORD_ENABLE_K2_E5,
	MS_REG_DBG_SHIFT_K2_E5, MS_REG_DBG_FORCE_VALID_K2_E5,
	MS_REG_DBG_FORCE_FRAME_K2_E5,
	true, false, DBG_RESET_REG_MISCS_PL_HV, 13
};

static struct block_defs block_phy_pcie_defs = {
	"phy_pcie",
	{false, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, DBG_BUS_CLIENT_RBCH,
	 DBG_BUS_CLIENT_RBCH},
	PCIE_REG_DBG_COMMON_SELECT_K2_E5,
	PCIE_REG_DBG_COMMON_DWORD_ENABLE_K2_E5,
	PCIE_REG_DBG_COMMON_SHIFT_K2_E5,
	PCIE_REG_DBG_COMMON_FORCE_VALID_K2_E5,
	PCIE_REG_DBG_COMMON_FORCE_FRAME_K2_E5,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_led_defs = {
	"led", {false, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	true, false, DBG_RESET_REG_MISCS_PL_HV, 14
};

static struct block_defs block_avs_wrap_defs = {
	"avs_wrap", {false, true, false}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	true, false, DBG_RESET_REG_MISCS_PL_UA, 11
};

static struct block_defs block_pxpreqbus_defs = {
	"pxpreqbus", {false, false, false}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_misc_aeu_defs = {
	"misc_aeu", {true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs block_bar0_map_defs = {
	"bar0_map", {true, true, true}, false, 0,
	{MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS, MAX_DBG_BUS_CLIENTS},
	0, 0, 0, 0, 0,
	false, false, MAX_DBG_RESET_REGS, 0
};

static struct block_defs *s_block_defs[MAX_BLOCK_ID] = {
	&block_grc_defs,
	&block_miscs_defs,
	&block_misc_defs,
	&block_dbu_defs,
	&block_pglue_b_defs,
	&block_cnig_defs,
	&block_cpmu_defs,
	&block_ncsi_defs,
	&block_opte_defs,
	&block_bmb_defs,
	&block_pcie_defs,
	&block_mcp_defs,
	&block_mcp2_defs,
	&block_pswhst_defs,
	&block_pswhst2_defs,
	&block_pswrd_defs,
	&block_pswrd2_defs,
	&block_pswwr_defs,
	&block_pswwr2_defs,
	&block_pswrq_defs,
	&block_pswrq2_defs,
	&block_pglcs_defs,
	&block_dmae_defs,
	&block_ptu_defs,
	&block_tcm_defs,
	&block_mcm_defs,
	&block_ucm_defs,
	&block_xcm_defs,
	&block_ycm_defs,
	&block_pcm_defs,
	&block_qm_defs,
	&block_tm_defs,
	&block_dorq_defs,
	&block_brb_defs,
	&block_src_defs,
	&block_prs_defs,
	&block_tsdm_defs,
	&block_msdm_defs,
	&block_usdm_defs,
	&block_xsdm_defs,
	&block_ysdm_defs,
	&block_psdm_defs,
	&block_tsem_defs,
	&block_msem_defs,
	&block_usem_defs,
	&block_xsem_defs,
	&block_ysem_defs,
	&block_psem_defs,
	&block_rss_defs,
	&block_tmld_defs,
	&block_muld_defs,
	&block_yuld_defs,
	&block_xyld_defs,
	&block_ptld_defs,
	&block_ypld_defs,
	&block_prm_defs,
	&block_pbf_pb1_defs,
	&block_pbf_pb2_defs,
	&block_rpb_defs,
	&block_btb_defs,
	&block_pbf_defs,
	&block_rdif_defs,
	&block_tdif_defs,
	&block_cdu_defs,
	&block_ccfc_defs,
	&block_tcfc_defs,
	&block_igu_defs,
	&block_cau_defs,
	&block_rgfs_defs,
	&block_rgsrc_defs,
	&block_tgfs_defs,
	&block_tgsrc_defs,
	&block_umac_defs,
	&block_xmac_defs,
	&block_dbg_defs,
	&block_nig_defs,
	&block_wol_defs,
	&block_bmbn_defs,
	&block_ipc_defs,
	&block_nwm_defs,
	&block_nws_defs,
	&block_ms_defs,
	&block_phy_pcie_defs,
	&block_led_defs,
	&block_avs_wrap_defs,
	&block_pxpreqbus_defs,
	&block_misc_aeu_defs,
	&block_bar0_map_defs,
};

static struct platform_defs s_platform_defs[] = {
=======
		{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCT},
		true,
		TSEM_REG_FAST_MEMORY,
		TSEM_REG_DBG_FRAME_MODE_BB_K2, TSEM_REG_SLOW_DBG_ACTIVE_BB_K2,
		TSEM_REG_SLOW_DBG_MODE_BB_K2, TSEM_REG_DBG_MODE1_CFG_BB_K2,
		TSEM_REG_SYNC_DBG_EMPTY, TSEM_REG_DBG_GPRE_VECT,
		TCM_REG_CTX_RBC_ACCS,
		{TCM_REG_AGG_CON_CTX, TCM_REG_SM_CON_CTX, TCM_REG_AGG_TASK_CTX,
		 TCM_REG_SM_TASK_CTX},
		{{4, 16, 2, 4}, {4, 16, 2, 4}} /* {bb} {k2} */
	},

	/* Mstorm */
	{'M', BLOCK_MSEM,
		{DBG_BUS_CLIENT_RBCT, DBG_BUS_CLIENT_RBCM},
		false,
		MSEM_REG_FAST_MEMORY,
		MSEM_REG_DBG_FRAME_MODE_BB_K2,
		MSEM_REG_SLOW_DBG_ACTIVE_BB_K2,
		MSEM_REG_SLOW_DBG_MODE_BB_K2,
		MSEM_REG_DBG_MODE1_CFG_BB_K2,
		MSEM_REG_SYNC_DBG_EMPTY,
		MSEM_REG_DBG_GPRE_VECT,
		MCM_REG_CTX_RBC_ACCS,
		{MCM_REG_AGG_CON_CTX, MCM_REG_SM_CON_CTX, MCM_REG_AGG_TASK_CTX,
		 MCM_REG_SM_TASK_CTX },
		{{1, 10, 2, 7}, {1, 10, 2, 7}} /* {bb} {k2}*/
	},

	/* Ustorm */
	{'U', BLOCK_USEM,
		{DBG_BUS_CLIENT_RBCU, DBG_BUS_CLIENT_RBCU},
		false,
		USEM_REG_FAST_MEMORY,
		USEM_REG_DBG_FRAME_MODE_BB_K2,
		USEM_REG_SLOW_DBG_ACTIVE_BB_K2,
		USEM_REG_SLOW_DBG_MODE_BB_K2,
		USEM_REG_DBG_MODE1_CFG_BB_K2,
		USEM_REG_SYNC_DBG_EMPTY,
		USEM_REG_DBG_GPRE_VECT,
		UCM_REG_CTX_RBC_ACCS,
		{UCM_REG_AGG_CON_CTX, UCM_REG_SM_CON_CTX, UCM_REG_AGG_TASK_CTX,
		 UCM_REG_SM_TASK_CTX},
		{{2, 13, 3, 3}, {2, 13, 3, 3}} /* {bb} {k2} */
	},

	/* Xstorm */
	{'X', BLOCK_XSEM,
		{DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCX},
		false,
		XSEM_REG_FAST_MEMORY,
		XSEM_REG_DBG_FRAME_MODE_BB_K2,
		XSEM_REG_SLOW_DBG_ACTIVE_BB_K2,
		XSEM_REG_SLOW_DBG_MODE_BB_K2,
		XSEM_REG_DBG_MODE1_CFG_BB_K2,
		XSEM_REG_SYNC_DBG_EMPTY,
		XSEM_REG_DBG_GPRE_VECT,
		XCM_REG_CTX_RBC_ACCS,
		{XCM_REG_AGG_CON_CTX, XCM_REG_SM_CON_CTX, 0, 0},
		{{9, 15, 0, 0}, {9, 15,	0, 0}} /* {bb} {k2} */
	},

	/* Ystorm */
	{'Y', BLOCK_YSEM,
		{DBG_BUS_CLIENT_RBCX, DBG_BUS_CLIENT_RBCY},
		false,
		YSEM_REG_FAST_MEMORY,
		YSEM_REG_DBG_FRAME_MODE_BB_K2,
		YSEM_REG_SLOW_DBG_ACTIVE_BB_K2,
		YSEM_REG_SLOW_DBG_MODE_BB_K2,
		YSEM_REG_DBG_MODE1_CFG_BB_K2,
		YSEM_REG_SYNC_DBG_EMPTY,
		YSEM_REG_DBG_GPRE_VECT,
		YCM_REG_CTX_RBC_ACCS,
		{YCM_REG_AGG_CON_CTX, YCM_REG_SM_CON_CTX, YCM_REG_AGG_TASK_CTX,
		 YCM_REG_SM_TASK_CTX},
		{{2, 3, 2, 12}, {2, 3, 2, 12}} /* {bb} {k2} */
	},

	/* Pstorm */
	{'P', BLOCK_PSEM,
		{DBG_BUS_CLIENT_RBCS, DBG_BUS_CLIENT_RBCS},
		true,
		PSEM_REG_FAST_MEMORY,
		PSEM_REG_DBG_FRAME_MODE_BB_K2,
		PSEM_REG_SLOW_DBG_ACTIVE_BB_K2,
		PSEM_REG_SLOW_DBG_MODE_BB_K2,
		PSEM_REG_DBG_MODE1_CFG_BB_K2,
		PSEM_REG_SYNC_DBG_EMPTY,
		PSEM_REG_DBG_GPRE_VECT,
		PCM_REG_CTX_RBC_ACCS,
		{0, PCM_REG_SM_CON_CTX, 0, 0},
		{{0, 10, 0, 0}, {0, 10, 0, 0}} /* {bb} {k2} */
	},
};

static struct hw_type_defs s_hw_type_defs[] = {
	/* HW_TYPE_ASIC */
>>>>>>> upstream/android-13
	{"asic", 1, 256, 32768},
	{"reserved", 0, 0, 0},
	{"reserved2", 0, 0, 0},
	{"reserved3", 0, 0, 0}
};

static struct grc_param_defs s_grc_param_defs[] = {
	/* DBG_GRC_PARAM_DUMP_TSTORM */
<<<<<<< HEAD
	{{1, 1, 1}, 0, 1, false, false, 1, 1},

	/* DBG_GRC_PARAM_DUMP_MSTORM */
	{{1, 1, 1}, 0, 1, false, false, 1, 1},

	/* DBG_GRC_PARAM_DUMP_USTORM */
	{{1, 1, 1}, 0, 1, false, false, 1, 1},

	/* DBG_GRC_PARAM_DUMP_XSTORM */
	{{1, 1, 1}, 0, 1, false, false, 1, 1},

	/* DBG_GRC_PARAM_DUMP_YSTORM */
	{{1, 1, 1}, 0, 1, false, false, 1, 1},

	/* DBG_GRC_PARAM_DUMP_PSTORM */
	{{1, 1, 1}, 0, 1, false, false, 1, 1},

	/* DBG_GRC_PARAM_DUMP_REGS */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_RAM */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_PBUF */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_IOR */
	{{0, 0, 0}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_VFC */
	{{0, 0, 0}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_CM_CTX */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_ILT */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_RSS */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_CAU */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_QM */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_MCP */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_MCP_TRACE_META_SIZE */
	{{1, 1, 1}, 1, 0xffffffff, false, true, 0, 1},

	/* DBG_GRC_PARAM_DUMP_CFC */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_IGU */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_BRB */
	{{0, 0, 0}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_BTB */
	{{0, 0, 0}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_BMB */
	{{0, 0, 0}, 0, 1, false, false, 0, 0},

	/* DBG_GRC_PARAM_DUMP_NIG */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_MULD */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_PRS */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_DMAE */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_TM */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_SDM */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_DIF */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_STATIC */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_UNSTALL */
	{{0, 0, 0}, 0, 1, false, false, 0, 0},

	/* DBG_GRC_PARAM_NUM_LCIDS */
	{{MAX_LCIDS, MAX_LCIDS, MAX_LCIDS}, 1, MAX_LCIDS, false, false,
	 MAX_LCIDS, MAX_LCIDS},

	/* DBG_GRC_PARAM_NUM_LTIDS */
	{{MAX_LTIDS, MAX_LTIDS, MAX_LTIDS}, 1, MAX_LTIDS, false, false,
	 MAX_LTIDS, MAX_LTIDS},

	/* DBG_GRC_PARAM_EXCLUDE_ALL */
	{{0, 0, 0}, 0, 1, true, false, 0, 0},

	/* DBG_GRC_PARAM_CRASH */
	{{0, 0, 0}, 0, 1, true, false, 0, 0},

	/* DBG_GRC_PARAM_PARITY_SAFE */
	{{0, 0, 0}, 0, 1, false, false, 1, 0},

	/* DBG_GRC_PARAM_DUMP_CM */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_DUMP_PHY */
	{{1, 1, 1}, 0, 1, false, false, 0, 1},

	/* DBG_GRC_PARAM_NO_MCP */
	{{0, 0, 0}, 0, 1, false, false, 0, 0},

	/* DBG_GRC_PARAM_NO_FW_VER */
	{{0, 0, 0}, 0, 1, false, false, 0, 0}
};

static struct rss_mem_defs s_rss_mem_defs[] = {
	{ "rss_mem_cid", "rss_cid", 0, 32,
	  {256, 320, 512} },

	{ "rss_mem_key_msb", "rss_key", 1024, 256,
	  {128, 208, 257} },

	{ "rss_mem_key_lsb", "rss_key", 2048, 64,
	  {128, 208, 257} },

	{ "rss_mem_info", "rss_info", 3072, 16,
	  {128, 208, 256} },

	{ "rss_mem_ind", "rss_ind", 4096, 16,
	  {16384, 26624, 32768} }
=======
	{{1, 1}, 0, 1, false, false, 1, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_MSTORM */
	{{1, 1}, 0, 1, false, false, 1, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_USTORM */
	{{1, 1}, 0, 1, false, false, 1, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_XSTORM */
	{{1, 1}, 0, 1, false, false, 1, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_YSTORM */
	{{1, 1}, 0, 1, false, false, 1, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_PSTORM */
	{{1, 1}, 0, 1, false, false, 1, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_REGS */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_RAM */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_PBUF */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_IOR */
	{{0, 0}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_VFC */
	{{0, 0}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_CM_CTX */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_ILT */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_RSS */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_CAU */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_QM */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_MCP */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_DORQ */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_CFC */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_IGU */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_BRB */
	{{0, 0}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_BTB */
	{{0, 0}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_BMB */
	{{0, 0}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_RESERVED1 */
	{{0, 0}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_DUMP_MULD */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_PRS */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_DMAE */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_TM */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_SDM */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_DIF */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_STATIC */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_UNSTALL */
	{{0, 0}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_RESERVED2 */
	{{0, 0}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_MCP_TRACE_META_SIZE */
	{{0, 0}, 1, 0xffffffff, false, true, 0, {0, 0}},

	/* DBG_GRC_PARAM_EXCLUDE_ALL */
	{{0, 0}, 0, 1, true, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_CRASH */
	{{0, 0}, 0, 1, true, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_PARITY_SAFE */
	{{0, 0}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_DUMP_CM */
	{{1, 1}, 0, 1, false, false, 0, {1, 1}},

	/* DBG_GRC_PARAM_DUMP_PHY */
	{{0, 0}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_NO_MCP */
	{{0, 0}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_NO_FW_VER */
	{{0, 0}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_RESERVED3 */
	{{0, 0}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_DUMP_MCP_HW_DUMP */
	{{0, 1}, 0, 1, false, false, 0, {0, 1}},

	/* DBG_GRC_PARAM_DUMP_ILT_CDUC */
	{{1, 1}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_DUMP_ILT_CDUT */
	{{1, 1}, 0, 1, false, false, 0, {0, 0}},

	/* DBG_GRC_PARAM_DUMP_CAU_EXT */
	{{0, 0}, 0, 1, false, false, 0, {1, 1}}
};

static struct rss_mem_defs s_rss_mem_defs[] = {
	{"rss_mem_cid", "rss_cid", 0, 32,
	 {256, 320}},

	{"rss_mem_key_msb", "rss_key", 1024, 256,
	 {128, 208}},

	{"rss_mem_key_lsb", "rss_key", 2048, 64,
	 {128, 208}},

	{"rss_mem_info", "rss_info", 3072, 16,
	 {128, 208}},

	{"rss_mem_ind", "rss_ind", 4096, 16,
	 {16384, 26624}}
>>>>>>> upstream/android-13
};

static struct vfc_ram_defs s_vfc_ram_defs[] = {
	{"vfc_ram_tt1", "vfc_ram", 0, 512},
	{"vfc_ram_mtt2", "vfc_ram", 512, 128},
	{"vfc_ram_stt2", "vfc_ram", 640, 32},
	{"vfc_ram_ro_vect", "vfc_ram", 672, 32}
};

static struct big_ram_defs s_big_ram_defs[] = {
<<<<<<< HEAD
	{ "BRB", MEM_GROUP_BRB_MEM, MEM_GROUP_BRB_RAM, DBG_GRC_PARAM_DUMP_BRB,
	  BRB_REG_BIG_RAM_ADDRESS, BRB_REG_BIG_RAM_DATA,
	  MISC_REG_BLOCK_256B_EN, {0, 0, 0},
	  {153600, 180224, 282624} },

	{ "BTB", MEM_GROUP_BTB_MEM, MEM_GROUP_BTB_RAM, DBG_GRC_PARAM_DUMP_BTB,
	  BTB_REG_BIG_RAM_ADDRESS, BTB_REG_BIG_RAM_DATA,
	  MISC_REG_BLOCK_256B_EN, {0, 1, 1},
	  {92160, 117760, 168960} },

	{ "BMB", MEM_GROUP_BMB_MEM, MEM_GROUP_BMB_RAM, DBG_GRC_PARAM_DUMP_BMB,
	  BMB_REG_BIG_RAM_ADDRESS, BMB_REG_BIG_RAM_DATA,
	  MISCS_REG_BLOCK_256B_EN, {0, 0, 0},
	  {36864, 36864, 36864} }
};

static struct reset_reg_defs s_reset_regs_defs[] = {
	/* DBG_RESET_REG_MISCS_PL_UA */
	{ MISCS_REG_RESET_PL_UA,
	  {true, true, true}, {0x0, 0x0, 0x0} },

	/* DBG_RESET_REG_MISCS_PL_HV */
	{ MISCS_REG_RESET_PL_HV,
	  {true, true, true}, {0x0, 0x400, 0x600} },

	/* DBG_RESET_REG_MISCS_PL_HV_2 */
	{ MISCS_REG_RESET_PL_HV_2_K2_E5,
	  {false, true, true}, {0x0, 0x0, 0x0} },

	/* DBG_RESET_REG_MISC_PL_UA */
	{ MISC_REG_RESET_PL_UA,
	  {true, true, true}, {0x0, 0x0, 0x0} },

	/* DBG_RESET_REG_MISC_PL_HV */
	{ MISC_REG_RESET_PL_HV,
	  {true, true, true}, {0x0, 0x0, 0x0} },

	/* DBG_RESET_REG_MISC_PL_PDA_VMAIN_1 */
	{ MISC_REG_RESET_PL_PDA_VMAIN_1,
	  {true, true, true}, {0x4404040, 0x4404040, 0x404040} },

	/* DBG_RESET_REG_MISC_PL_PDA_VMAIN_2 */
	{ MISC_REG_RESET_PL_PDA_VMAIN_2,
	  {true, true, true}, {0x7, 0x7c00007, 0x5c08007} },

	/* DBG_RESET_REG_MISC_PL_PDA_VAUX */
	{ MISC_REG_RESET_PL_PDA_VAUX,
	  {true, true, true}, {0x2, 0x2, 0x2} },
=======
	{"BRB", MEM_GROUP_BRB_MEM, MEM_GROUP_BRB_RAM, DBG_GRC_PARAM_DUMP_BRB,
	 BRB_REG_BIG_RAM_ADDRESS, BRB_REG_BIG_RAM_DATA,
	 MISC_REG_BLOCK_256B_EN, {0, 0},
	 {153600, 180224}},

	{"BTB", MEM_GROUP_BTB_MEM, MEM_GROUP_BTB_RAM, DBG_GRC_PARAM_DUMP_BTB,
	 BTB_REG_BIG_RAM_ADDRESS, BTB_REG_BIG_RAM_DATA,
	 MISC_REG_BLOCK_256B_EN, {0, 1},
	 {92160, 117760}},

	{"BMB", MEM_GROUP_BMB_MEM, MEM_GROUP_BMB_RAM, DBG_GRC_PARAM_DUMP_BMB,
	 BMB_REG_BIG_RAM_ADDRESS, BMB_REG_BIG_RAM_DATA,
	 MISCS_REG_BLOCK_256B_EN, {0, 0},
	 {36864, 36864}}
};

static struct rbc_reset_defs s_rbc_reset_defs[] = {
	{MISCS_REG_RESET_PL_HV,
	 {0x0, 0x400}},
	{MISC_REG_RESET_PL_PDA_VMAIN_1,
	 {0x4404040, 0x4404040}},
	{MISC_REG_RESET_PL_PDA_VMAIN_2,
	 {0x7, 0x7c00007}},
	{MISC_REG_RESET_PL_PDA_VAUX,
	 {0x2, 0x2}},
>>>>>>> upstream/android-13
};

static struct phy_defs s_phy_defs[] = {
	{"nw_phy", NWS_REG_NWS_CMU_K2,
	 PHY_NW_IP_REG_PHY0_TOP_TBUS_ADDR_7_0_K2_E5,
	 PHY_NW_IP_REG_PHY0_TOP_TBUS_ADDR_15_8_K2_E5,
	 PHY_NW_IP_REG_PHY0_TOP_TBUS_DATA_7_0_K2_E5,
	 PHY_NW_IP_REG_PHY0_TOP_TBUS_DATA_11_8_K2_E5},
	{"sgmii_phy", MS_REG_MS_CMU_K2_E5,
	 PHY_SGMII_IP_REG_AHB_CMU_CSR_0_X132_K2_E5,
	 PHY_SGMII_IP_REG_AHB_CMU_CSR_0_X133_K2_E5,
	 PHY_SGMII_IP_REG_AHB_CMU_CSR_0_X130_K2_E5,
	 PHY_SGMII_IP_REG_AHB_CMU_CSR_0_X131_K2_E5},
	{"pcie_phy0", PHY_PCIE_REG_PHY0_K2_E5,
	 PHY_PCIE_IP_REG_AHB_CMU_CSR_0_X132_K2_E5,
	 PHY_PCIE_IP_REG_AHB_CMU_CSR_0_X133_K2_E5,
	 PHY_PCIE_IP_REG_AHB_CMU_CSR_0_X130_K2_E5,
	 PHY_PCIE_IP_REG_AHB_CMU_CSR_0_X131_K2_E5},
	{"pcie_phy1", PHY_PCIE_REG_PHY1_K2_E5,
	 PHY_PCIE_IP_REG_AHB_CMU_CSR_0_X132_K2_E5,
	 PHY_PCIE_IP_REG_AHB_CMU_CSR_0_X133_K2_E5,
	 PHY_PCIE_IP_REG_AHB_CMU_CSR_0_X130_K2_E5,
	 PHY_PCIE_IP_REG_AHB_CMU_CSR_0_X131_K2_E5},
};

static struct split_type_defs s_split_type_defs[] = {
	/* SPLIT_TYPE_NONE */
	{"eng"},

	/* SPLIT_TYPE_PORT */
	{"port"},

	/* SPLIT_TYPE_PF */
	{"pf"},

	/* SPLIT_TYPE_PORT_PF */
	{"port"},

	/* SPLIT_TYPE_VF */
	{"vf"}
};

/**************************** Private Functions ******************************/

/* Reads and returns a single dword from the specified unaligned buffer */
static u32 qed_read_unaligned_dword(u8 *buf)
{
	u32 dword;

	memcpy((u8 *)&dword, buf, sizeof(dword));
	return dword;
}

<<<<<<< HEAD
=======
/* Sets the value of the specified GRC param */
static void qed_grc_set_param(struct qed_hwfn *p_hwfn,
			      enum dbg_grc_params grc_param, u32 val)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;

	dev_data->grc.param_val[grc_param] = val;
}

>>>>>>> upstream/android-13
/* Returns the value of the specified GRC param */
static u32 qed_grc_get_param(struct qed_hwfn *p_hwfn,
			     enum dbg_grc_params grc_param)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;

	return dev_data->grc.param_val[grc_param];
}

/* Initializes the GRC parameters */
static void qed_dbg_grc_init_params(struct qed_hwfn *p_hwfn)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;

	if (!dev_data->grc.params_initialized) {
		qed_dbg_grc_set_params_default(p_hwfn);
		dev_data->grc.params_initialized = 1;
	}
}

<<<<<<< HEAD
/* Initializes debug data for the specified device */
static enum dbg_status qed_dbg_dev_init(struct qed_hwfn *p_hwfn,
					struct qed_ptt *p_ptt)
=======
/* Sets pointer and size for the specified binary buffer type */
static void qed_set_dbg_bin_buf(struct qed_hwfn *p_hwfn,
				enum bin_dbg_buffer_type buf_type,
				const u32 *ptr, u32 size)
{
	struct virt_mem_desc *buf = &p_hwfn->dbg_arrays[buf_type];

	buf->ptr = (void *)ptr;
	buf->size = size;
}

/* Initializes debug data for the specified device */
static enum dbg_status qed_dbg_dev_init(struct qed_hwfn *p_hwfn)
>>>>>>> upstream/android-13
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u8 num_pfs = 0, max_pfs_per_port = 0;

	if (dev_data->initialized)
		return DBG_STATUS_OK;

	/* Set chip */
	if (QED_IS_K2(p_hwfn->cdev)) {
		dev_data->chip_id = CHIP_K2;
		dev_data->mode_enable[MODE_K2] = 1;
		dev_data->num_vfs = MAX_NUM_VFS_K2;
		num_pfs = MAX_NUM_PFS_K2;
		max_pfs_per_port = MAX_NUM_PFS_K2 / 2;
	} else if (QED_IS_BB_B0(p_hwfn->cdev)) {
		dev_data->chip_id = CHIP_BB;
		dev_data->mode_enable[MODE_BB] = 1;
		dev_data->num_vfs = MAX_NUM_VFS_BB;
		num_pfs = MAX_NUM_PFS_BB;
		max_pfs_per_port = MAX_NUM_PFS_BB;
	} else {
		return DBG_STATUS_UNKNOWN_CHIP;
	}

<<<<<<< HEAD
	/* Set platofrm */
	dev_data->platform_id = PLATFORM_ASIC;
	dev_data->mode_enable[MODE_ASIC] = 1;

	/* Set port mode */
	switch (qed_rd(p_hwfn, p_ptt, MISC_REG_PORT_MODE)) {
	case 0:
		dev_data->mode_enable[MODE_PORTS_PER_ENG_1] = 1;
		break;
	case 1:
		dev_data->mode_enable[MODE_PORTS_PER_ENG_2] = 1;
		break;
	case 2:
=======
	/* Set HW type */
	dev_data->hw_type = HW_TYPE_ASIC;
	dev_data->mode_enable[MODE_ASIC] = 1;

	/* Set port mode */
	switch (p_hwfn->cdev->num_ports_in_engine) {
	case 1:
		dev_data->mode_enable[MODE_PORTS_PER_ENG_1] = 1;
		break;
	case 2:
		dev_data->mode_enable[MODE_PORTS_PER_ENG_2] = 1;
		break;
	case 4:
>>>>>>> upstream/android-13
		dev_data->mode_enable[MODE_PORTS_PER_ENG_4] = 1;
		break;
	}

	/* Set 100G mode */
<<<<<<< HEAD
	if (dev_data->chip_id == CHIP_BB &&
	    qed_rd(p_hwfn, p_ptt, CNIG_REG_NW_PORT_MODE_BB) == 2)
=======
	if (QED_IS_CMT(p_hwfn->cdev))
>>>>>>> upstream/android-13
		dev_data->mode_enable[MODE_100G] = 1;

	/* Set number of ports */
	if (dev_data->mode_enable[MODE_PORTS_PER_ENG_1] ||
	    dev_data->mode_enable[MODE_100G])
		dev_data->num_ports = 1;
	else if (dev_data->mode_enable[MODE_PORTS_PER_ENG_2])
		dev_data->num_ports = 2;
	else if (dev_data->mode_enable[MODE_PORTS_PER_ENG_4])
		dev_data->num_ports = 4;

	/* Set number of PFs per port */
	dev_data->num_pfs_per_port = min_t(u32,
					   num_pfs / dev_data->num_ports,
					   max_pfs_per_port);

	/* Initializes the GRC parameters */
	qed_dbg_grc_init_params(p_hwfn);

	dev_data->use_dmae = true;
	dev_data->initialized = 1;

	return DBG_STATUS_OK;
}

<<<<<<< HEAD
static struct dbg_bus_block *get_dbg_bus_block_desc(struct qed_hwfn *p_hwfn,
						    enum block_id block_id)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;

	return (struct dbg_bus_block *)&dbg_bus_blocks[block_id *
						       MAX_CHIP_IDS +
						       dev_data->chip_id];
=======
static const struct dbg_block *get_dbg_block(struct qed_hwfn *p_hwfn,
					     enum block_id block_id)
{
	const struct dbg_block *dbg_block;

	dbg_block = p_hwfn->dbg_arrays[BIN_BUF_DBG_BLOCKS].ptr;
	return dbg_block + block_id;
}

static const struct dbg_block_chip *qed_get_dbg_block_per_chip(struct qed_hwfn
							       *p_hwfn,
							       enum block_id
							       block_id)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;

	return (const struct dbg_block_chip *)
	    p_hwfn->dbg_arrays[BIN_BUF_DBG_BLOCKS_CHIP_DATA].ptr +
	    block_id * MAX_CHIP_IDS + dev_data->chip_id;
}

static const struct dbg_reset_reg *qed_get_dbg_reset_reg(struct qed_hwfn
							 *p_hwfn,
							 u8 reset_reg_id)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;

	return (const struct dbg_reset_reg *)
	    p_hwfn->dbg_arrays[BIN_BUF_DBG_RESET_REGS].ptr +
	    reset_reg_id * MAX_CHIP_IDS + dev_data->chip_id;
>>>>>>> upstream/android-13
}

/* Reads the FW info structure for the specified Storm from the chip,
 * and writes it to the specified fw_info pointer.
 */
static void qed_read_storm_fw_info(struct qed_hwfn *p_hwfn,
				   struct qed_ptt *p_ptt,
				   u8 storm_id, struct fw_info *fw_info)
{
	struct storm_defs *storm = &s_storm_defs[storm_id];
	struct fw_info_location fw_info_location;
<<<<<<< HEAD
	u32 addr, i, *dest;
=======
	u32 addr, i, size, *dest;
>>>>>>> upstream/android-13

	memset(&fw_info_location, 0, sizeof(fw_info_location));
	memset(fw_info, 0, sizeof(*fw_info));

	/* Read first the address that points to fw_info location.
	 * The address is located in the last line of the Storm RAM.
	 */
	addr = storm->sem_fast_mem_addr + SEM_FAST_REG_INT_RAM +
<<<<<<< HEAD
	       DWORDS_TO_BYTES(SEM_FAST_REG_INT_RAM_SIZE_BB_K2) -
	       sizeof(fw_info_location);
	dest = (u32 *)&fw_info_location;

	for (i = 0; i < BYTES_TO_DWORDS(sizeof(fw_info_location));
	     i++, addr += BYTES_IN_DWORD)
		dest[i] = qed_rd(p_hwfn, p_ptt, addr);

	/* Read FW version info from Storm RAM */
	if (fw_info_location.size > 0 && fw_info_location.size <=
	    sizeof(*fw_info)) {
		addr = fw_info_location.grc_addr;
		dest = (u32 *)fw_info;
		for (i = 0; i < BYTES_TO_DWORDS(fw_info_location.size);
		     i++, addr += BYTES_IN_DWORD)
			dest[i] = qed_rd(p_hwfn, p_ptt, addr);
	}
=======
	    DWORDS_TO_BYTES(SEM_FAST_REG_INT_RAM_SIZE) -
	    sizeof(fw_info_location);

	dest = (u32 *)&fw_info_location;
	size = BYTES_TO_DWORDS(sizeof(fw_info_location));

	for (i = 0; i < size; i++, addr += BYTES_IN_DWORD)
		dest[i] = qed_rd(p_hwfn, p_ptt, addr);

	/* qed_rq() fetches data in CPU byteorder. Swap it back to
	 * the device's to get right structure layout.
	 */
	cpu_to_le32_array(dest, size);

	/* Read FW version info from Storm RAM */
	size = le32_to_cpu(fw_info_location.size);
	if (!size || size > sizeof(*fw_info))
		return;

	addr = le32_to_cpu(fw_info_location.grc_addr);
	dest = (u32 *)fw_info;
	size = BYTES_TO_DWORDS(size);

	for (i = 0; i < size; i++, addr += BYTES_IN_DWORD)
		dest[i] = qed_rd(p_hwfn, p_ptt, addr);

	cpu_to_le32_array(dest, size);
>>>>>>> upstream/android-13
}

/* Dumps the specified string to the specified buffer.
 * Returns the dumped size in bytes.
 */
static u32 qed_dump_str(char *dump_buf, bool dump, const char *str)
{
	if (dump)
		strcpy(dump_buf, str);

	return (u32)strlen(str) + 1;
}

/* Dumps zeros to align the specified buffer to dwords.
 * Returns the dumped size in bytes.
 */
static u32 qed_dump_align(char *dump_buf, bool dump, u32 byte_offset)
{
	u8 offset_in_dword, align_size;

	offset_in_dword = (u8)(byte_offset & 0x3);
	align_size = offset_in_dword ? BYTES_IN_DWORD - offset_in_dword : 0;

	if (dump && align_size)
		memset(dump_buf, 0, align_size);

	return align_size;
}

/* Writes the specified string param to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_dump_str_param(u32 *dump_buf,
			      bool dump,
			      const char *param_name, const char *param_val)
{
	char *char_buf = (char *)dump_buf;
	u32 offset = 0;

	/* Dump param name */
	offset += qed_dump_str(char_buf + offset, dump, param_name);

	/* Indicate a string param value */
	if (dump)
		*(char_buf + offset) = 1;
	offset++;

	/* Dump param value */
	offset += qed_dump_str(char_buf + offset, dump, param_val);

	/* Align buffer to next dword */
	offset += qed_dump_align(char_buf + offset, dump, offset);

	return BYTES_TO_DWORDS(offset);
}

/* Writes the specified numeric param to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_dump_num_param(u32 *dump_buf,
			      bool dump, const char *param_name, u32 param_val)
{
	char *char_buf = (char *)dump_buf;
	u32 offset = 0;

	/* Dump param name */
	offset += qed_dump_str(char_buf + offset, dump, param_name);

	/* Indicate a numeric param value */
	if (dump)
		*(char_buf + offset) = 0;
	offset++;

	/* Align buffer to next dword */
	offset += qed_dump_align(char_buf + offset, dump, offset);

	/* Dump param value (and change offset from bytes to dwords) */
	offset = BYTES_TO_DWORDS(offset);
	if (dump)
		*(dump_buf + offset) = param_val;
	offset++;

	return offset;
}

/* Reads the FW version and writes it as a param to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_dump_fw_ver_param(struct qed_hwfn *p_hwfn,
				 struct qed_ptt *p_ptt,
				 u32 *dump_buf, bool dump)
{
	char fw_ver_str[16] = EMPTY_FW_VERSION_STR;
	char fw_img_str[16] = EMPTY_FW_IMAGE_STR;
	struct fw_info fw_info = { {0}, {0} };
	u32 offset = 0;

	if (dump && !qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_NO_FW_VER)) {
		/* Read FW info from chip */
		qed_read_fw_info(p_hwfn, p_ptt, &fw_info);

		/* Create FW version/image strings */
		if (snprintf(fw_ver_str, sizeof(fw_ver_str),
			     "%d_%d_%d_%d", fw_info.ver.num.major,
			     fw_info.ver.num.minor, fw_info.ver.num.rev,
			     fw_info.ver.num.eng) < 0)
			DP_NOTICE(p_hwfn,
				  "Unexpected debug error: invalid FW version string\n");
		switch (fw_info.ver.image_id) {
		case FW_IMG_MAIN:
			strcpy(fw_img_str, "main");
			break;
		default:
			strcpy(fw_img_str, "unknown");
			break;
		}
	}

	/* Dump FW version, image and timestamp */
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "fw-version", fw_ver_str);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "fw-image", fw_img_str);
<<<<<<< HEAD
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "fw-timestamp", fw_info.ver.timestamp);
=======
	offset += qed_dump_num_param(dump_buf + offset, dump, "fw-timestamp",
				     le32_to_cpu(fw_info.ver.timestamp));
>>>>>>> upstream/android-13

	return offset;
}

/* Reads the MFW version and writes it as a param to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_dump_mfw_ver_param(struct qed_hwfn *p_hwfn,
				  struct qed_ptt *p_ptt,
				  u32 *dump_buf, bool dump)
{
	char mfw_ver_str[16] = EMPTY_FW_VERSION_STR;

	if (dump &&
	    !qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_NO_FW_VER)) {
		u32 global_section_offsize, global_section_addr, mfw_ver;
		u32 public_data_addr, global_section_offsize_addr;

		/* Find MCP public data GRC address. Needs to be ORed with
		 * MCP_REG_SCRATCH due to a HW bug.
		 */
		public_data_addr = qed_rd(p_hwfn,
					  p_ptt,
					  MISC_REG_SHARED_MEM_ADDR) |
				   MCP_REG_SCRATCH;

		/* Find MCP public global section offset */
		global_section_offsize_addr = public_data_addr +
					      offsetof(struct mcp_public_data,
						       sections) +
					      sizeof(offsize_t) * PUBLIC_GLOBAL;
		global_section_offsize = qed_rd(p_hwfn, p_ptt,
						global_section_offsize_addr);
		global_section_addr =
			MCP_REG_SCRATCH +
			(global_section_offsize & OFFSIZE_OFFSET_MASK) * 4;

		/* Read MFW version from MCP public global section */
		mfw_ver = qed_rd(p_hwfn, p_ptt,
				 global_section_addr +
				 offsetof(struct public_global, mfw_ver));

		/* Dump MFW version param */
		if (snprintf(mfw_ver_str, sizeof(mfw_ver_str), "%d_%d_%d_%d",
			     (u8)(mfw_ver >> 24), (u8)(mfw_ver >> 16),
			     (u8)(mfw_ver >> 8), (u8)mfw_ver) < 0)
			DP_NOTICE(p_hwfn,
				  "Unexpected debug error: invalid MFW version string\n");
	}

	return qed_dump_str_param(dump_buf, dump, "mfw-version", mfw_ver_str);
}

<<<<<<< HEAD
=======
/* Reads the chip revision from the chip and writes it as a param to the
 * specified buffer. Returns the dumped size in dwords.
 */
static u32 qed_dump_chip_revision_param(struct qed_hwfn *p_hwfn,
					struct qed_ptt *p_ptt,
					u32 *dump_buf, bool dump)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	char param_str[3] = "??";

	if (dev_data->hw_type == HW_TYPE_ASIC) {
		u32 chip_rev, chip_metal;

		chip_rev = qed_rd(p_hwfn, p_ptt, MISCS_REG_CHIP_REV);
		chip_metal = qed_rd(p_hwfn, p_ptt, MISCS_REG_CHIP_METAL);

		param_str[0] = 'a' + (u8)chip_rev;
		param_str[1] = '0' + (u8)chip_metal;
	}

	return qed_dump_str_param(dump_buf, dump, "chip-revision", param_str);
}

>>>>>>> upstream/android-13
/* Writes a section header to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_dump_section_hdr(u32 *dump_buf,
				bool dump, const char *name, u32 num_params)
{
	return qed_dump_num_param(dump_buf, dump, name, num_params);
}

/* Writes the common global params to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_dump_common_global_params(struct qed_hwfn *p_hwfn,
					 struct qed_ptt *p_ptt,
					 u32 *dump_buf,
					 bool dump,
					 u8 num_specific_global_params)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 offset = 0;
	u8 num_params;

	/* Dump global params section header */
<<<<<<< HEAD
	num_params = NUM_COMMON_GLOBAL_PARAMS + num_specific_global_params;
=======
	num_params = NUM_COMMON_GLOBAL_PARAMS + num_specific_global_params +
		(dev_data->chip_id == CHIP_BB ? 1 : 0);
>>>>>>> upstream/android-13
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "global_params", num_params);

	/* Store params */
	offset += qed_dump_fw_ver_param(p_hwfn, p_ptt, dump_buf + offset, dump);
	offset += qed_dump_mfw_ver_param(p_hwfn,
					 p_ptt, dump_buf + offset, dump);
<<<<<<< HEAD
=======
	offset += qed_dump_chip_revision_param(p_hwfn,
					       p_ptt, dump_buf + offset, dump);
>>>>>>> upstream/android-13
	offset += qed_dump_num_param(dump_buf + offset,
				     dump, "tools-version", TOOLS_VERSION);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump,
				     "chip",
				     s_chip_defs[dev_data->chip_id].name);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump,
				     "platform",
<<<<<<< HEAD
				     s_platform_defs[dev_data->platform_id].
				     name);
	offset +=
	    qed_dump_num_param(dump_buf + offset, dump, "pci-func",
			       p_hwfn->abs_pf_id);
=======
				     s_hw_type_defs[dev_data->hw_type].name);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump, "pci-func", p_hwfn->abs_pf_id);
	if (dev_data->chip_id == CHIP_BB)
		offset += qed_dump_num_param(dump_buf + offset,
					     dump, "path", QED_PATH_ID(p_hwfn));
>>>>>>> upstream/android-13

	return offset;
}

/* Writes the "last" section (including CRC) to the specified buffer at the
 * given offset. Returns the dumped size in dwords.
 */
static u32 qed_dump_last_section(u32 *dump_buf, u32 offset, bool dump)
{
	u32 start_offset = offset;

	/* Dump CRC section header */
	offset += qed_dump_section_hdr(dump_buf + offset, dump, "last", 0);

	/* Calculate CRC32 and add it to the dword after the "last" section */
	if (dump)
		*(dump_buf + offset) = ~crc32(0xffffffff,
					      (u8 *)dump_buf,
					      DWORDS_TO_BYTES(offset));

	offset++;

	return offset - start_offset;
}

/* Update blocks reset state  */
static void qed_update_blocks_reset_state(struct qed_hwfn *p_hwfn,
					  struct qed_ptt *p_ptt)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
<<<<<<< HEAD
	u32 reg_val[MAX_DBG_RESET_REGS] = { 0 };
	u32 i;

	/* Read reset registers */
	for (i = 0; i < MAX_DBG_RESET_REGS; i++)
		if (s_reset_regs_defs[i].exists[dev_data->chip_id])
			reg_val[i] = qed_rd(p_hwfn,
					    p_ptt, s_reset_regs_defs[i].addr);

	/* Check if blocks are in reset */
	for (i = 0; i < MAX_BLOCK_ID; i++) {
		struct block_defs *block = s_block_defs[i];

		dev_data->block_in_reset[i] = block->has_reset_bit &&
		    !(reg_val[block->reset_reg] & BIT(block->reset_bit_offset));
	}
}

=======
	u32 reg_val[NUM_DBG_RESET_REGS] = { 0 };
	u8 rst_reg_id;
	u32 blk_id;

	/* Read reset registers */
	for (rst_reg_id = 0; rst_reg_id < NUM_DBG_RESET_REGS; rst_reg_id++) {
		const struct dbg_reset_reg *rst_reg;
		bool rst_reg_removed;
		u32 rst_reg_addr;

		rst_reg = qed_get_dbg_reset_reg(p_hwfn, rst_reg_id);
		rst_reg_removed = GET_FIELD(rst_reg->data,
					    DBG_RESET_REG_IS_REMOVED);
		rst_reg_addr = DWORDS_TO_BYTES(GET_FIELD(rst_reg->data,
							 DBG_RESET_REG_ADDR));

		if (!rst_reg_removed)
			reg_val[rst_reg_id] = qed_rd(p_hwfn, p_ptt,
						     rst_reg_addr);
	}

	/* Check if blocks are in reset */
	for (blk_id = 0; blk_id < NUM_PHYS_BLOCKS; blk_id++) {
		const struct dbg_block_chip *blk;
		bool has_rst_reg;
		bool is_removed;

		blk = qed_get_dbg_block_per_chip(p_hwfn, (enum block_id)blk_id);
		is_removed = GET_FIELD(blk->flags, DBG_BLOCK_CHIP_IS_REMOVED);
		has_rst_reg = GET_FIELD(blk->flags,
					DBG_BLOCK_CHIP_HAS_RESET_REG);

		if (!is_removed && has_rst_reg)
			dev_data->block_in_reset[blk_id] =
			    !(reg_val[blk->reset_reg_id] &
			      BIT(blk->reset_reg_bit_offset));
	}
}

/* is_mode_match recursive function */
static bool qed_is_mode_match_rec(struct qed_hwfn *p_hwfn,
				  u16 *modes_buf_offset, u8 rec_depth)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u8 *dbg_array;
	bool arg1, arg2;
	u8 tree_val;

	if (rec_depth > MAX_RECURSION_DEPTH) {
		DP_NOTICE(p_hwfn,
			  "Unexpected error: is_mode_match_rec exceeded the max recursion depth. This is probably due to a corrupt init/debug buffer.\n");
		return false;
	}

	/* Get next element from modes tree buffer */
	dbg_array = p_hwfn->dbg_arrays[BIN_BUF_DBG_MODE_TREE].ptr;
	tree_val = dbg_array[(*modes_buf_offset)++];

	switch (tree_val) {
	case INIT_MODE_OP_NOT:
		return !qed_is_mode_match_rec(p_hwfn,
					      modes_buf_offset, rec_depth + 1);
	case INIT_MODE_OP_OR:
	case INIT_MODE_OP_AND:
		arg1 = qed_is_mode_match_rec(p_hwfn,
					     modes_buf_offset, rec_depth + 1);
		arg2 = qed_is_mode_match_rec(p_hwfn,
					     modes_buf_offset, rec_depth + 1);
		return (tree_val == INIT_MODE_OP_OR) ? (arg1 ||
							arg2) : (arg1 && arg2);
	default:
		return dev_data->mode_enable[tree_val - MAX_INIT_MODE_OPS] > 0;
	}
}

/* Returns true if the mode (specified using modes_buf_offset) is enabled */
static bool qed_is_mode_match(struct qed_hwfn *p_hwfn, u16 *modes_buf_offset)
{
	return qed_is_mode_match_rec(p_hwfn, modes_buf_offset, 0);
}

>>>>>>> upstream/android-13
/* Enable / disable the Debug block */
static void qed_bus_enable_dbg_block(struct qed_hwfn *p_hwfn,
				     struct qed_ptt *p_ptt, bool enable)
{
	qed_wr(p_hwfn, p_ptt, DBG_REG_DBG_BLOCK_ON, enable ? 1 : 0);
}

/* Resets the Debug block */
static void qed_bus_reset_dbg_block(struct qed_hwfn *p_hwfn,
				    struct qed_ptt *p_ptt)
{
<<<<<<< HEAD
	u32 dbg_reset_reg_addr, old_reset_reg_val, new_reset_reg_val;
	struct block_defs *dbg_block = s_block_defs[BLOCK_DBG];

	dbg_reset_reg_addr = s_reset_regs_defs[dbg_block->reset_reg].addr;
	old_reset_reg_val = qed_rd(p_hwfn, p_ptt, dbg_reset_reg_addr);
	new_reset_reg_val =
	    old_reset_reg_val & ~BIT(dbg_block->reset_bit_offset);

	qed_wr(p_hwfn, p_ptt, dbg_reset_reg_addr, new_reset_reg_val);
	qed_wr(p_hwfn, p_ptt, dbg_reset_reg_addr, old_reset_reg_val);
}

static void qed_bus_set_framing_mode(struct qed_hwfn *p_hwfn,
				     struct qed_ptt *p_ptt,
				     enum dbg_bus_frame_modes mode)
{
	qed_wr(p_hwfn, p_ptt, DBG_REG_FRAMING_MODE, (u8)mode);
=======
	u32 reset_reg_addr, old_reset_reg_val, new_reset_reg_val;
	const struct dbg_reset_reg *reset_reg;
	const struct dbg_block_chip *block;

	block = qed_get_dbg_block_per_chip(p_hwfn, BLOCK_DBG);
	reset_reg = qed_get_dbg_reset_reg(p_hwfn, block->reset_reg_id);
	reset_reg_addr =
	    DWORDS_TO_BYTES(GET_FIELD(reset_reg->data, DBG_RESET_REG_ADDR));

	old_reset_reg_val = qed_rd(p_hwfn, p_ptt, reset_reg_addr);
	new_reset_reg_val =
	    old_reset_reg_val & ~BIT(block->reset_reg_bit_offset);

	qed_wr(p_hwfn, p_ptt, reset_reg_addr, new_reset_reg_val);
	qed_wr(p_hwfn, p_ptt, reset_reg_addr, old_reset_reg_val);
>>>>>>> upstream/android-13
}

/* Enable / disable Debug Bus clients according to the specified mask
 * (1 = enable, 0 = disable).
 */
static void qed_bus_enable_clients(struct qed_hwfn *p_hwfn,
				   struct qed_ptt *p_ptt, u32 client_mask)
{
	qed_wr(p_hwfn, p_ptt, DBG_REG_CLIENT_ENABLE, client_mask);
}

<<<<<<< HEAD
static bool qed_is_mode_match(struct qed_hwfn *p_hwfn, u16 *modes_buf_offset)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	bool arg1, arg2;
	const u32 *ptr;
	u8 tree_val;

	/* Get next element from modes tree buffer */
	ptr = s_dbg_arrays[BIN_BUF_DBG_MODE_TREE].ptr;
	tree_val = ((u8 *)ptr)[(*modes_buf_offset)++];

	switch (tree_val) {
	case INIT_MODE_OP_NOT:
		return !qed_is_mode_match(p_hwfn, modes_buf_offset);
	case INIT_MODE_OP_OR:
	case INIT_MODE_OP_AND:
		arg1 = qed_is_mode_match(p_hwfn, modes_buf_offset);
		arg2 = qed_is_mode_match(p_hwfn, modes_buf_offset);
		return (tree_val == INIT_MODE_OP_OR) ? (arg1 ||
							arg2) : (arg1 && arg2);
	default:
		return dev_data->mode_enable[tree_val - MAX_INIT_MODE_OPS] > 0;
=======
static void qed_bus_config_dbg_line(struct qed_hwfn *p_hwfn,
				    struct qed_ptt *p_ptt,
				    enum block_id block_id,
				    u8 line_id,
				    u8 enable_mask,
				    u8 right_shift,
				    u8 force_valid_mask, u8 force_frame_mask)
{
	const struct dbg_block_chip *block =
		qed_get_dbg_block_per_chip(p_hwfn, block_id);

	qed_wr(p_hwfn, p_ptt, DWORDS_TO_BYTES(block->dbg_select_reg_addr),
	       line_id);
	qed_wr(p_hwfn, p_ptt, DWORDS_TO_BYTES(block->dbg_dword_enable_reg_addr),
	       enable_mask);
	qed_wr(p_hwfn, p_ptt, DWORDS_TO_BYTES(block->dbg_shift_reg_addr),
	       right_shift);
	qed_wr(p_hwfn, p_ptt, DWORDS_TO_BYTES(block->dbg_force_valid_reg_addr),
	       force_valid_mask);
	qed_wr(p_hwfn, p_ptt, DWORDS_TO_BYTES(block->dbg_force_frame_reg_addr),
	       force_frame_mask);
}

/* Disable debug bus in all blocks */
static void qed_bus_disable_blocks(struct qed_hwfn *p_hwfn,
				   struct qed_ptt *p_ptt)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 block_id;

	/* Disable all blocks */
	for (block_id = 0; block_id < MAX_BLOCK_ID; block_id++) {
		const struct dbg_block_chip *block_per_chip =
		    qed_get_dbg_block_per_chip(p_hwfn,
					       (enum block_id)block_id);

		if (GET_FIELD(block_per_chip->flags,
			      DBG_BLOCK_CHIP_IS_REMOVED) ||
		    dev_data->block_in_reset[block_id])
			continue;

		/* Disable debug bus */
		if (GET_FIELD(block_per_chip->flags,
			      DBG_BLOCK_CHIP_HAS_DBG_BUS)) {
			u32 dbg_en_addr =
				block_per_chip->dbg_dword_enable_reg_addr;
			u16 modes_buf_offset =
			    GET_FIELD(block_per_chip->dbg_bus_mode.data,
				      DBG_MODE_HDR_MODES_BUF_OFFSET);
			bool eval_mode =
			    GET_FIELD(block_per_chip->dbg_bus_mode.data,
				      DBG_MODE_HDR_EVAL_MODE) > 0;

			if (!eval_mode ||
			    qed_is_mode_match(p_hwfn, &modes_buf_offset))
				qed_wr(p_hwfn, p_ptt,
				       DWORDS_TO_BYTES(dbg_en_addr),
				       0);
		}
>>>>>>> upstream/android-13
	}
}

/* Returns true if the specified entity (indicated by GRC param) should be
 * included in the dump, false otherwise.
 */
static bool qed_grc_is_included(struct qed_hwfn *p_hwfn,
				enum dbg_grc_params grc_param)
{
	return qed_grc_get_param(p_hwfn, grc_param) > 0;
}

<<<<<<< HEAD
=======
/* Returns the storm_id that matches the specified Storm letter,
 * or MAX_DBG_STORMS if invalid storm letter.
 */
static enum dbg_storms qed_get_id_from_letter(char storm_letter)
{
	u8 storm_id;

	for (storm_id = 0; storm_id < MAX_DBG_STORMS; storm_id++)
		if (s_storm_defs[storm_id].letter == storm_letter)
			return (enum dbg_storms)storm_id;

	return MAX_DBG_STORMS;
}

>>>>>>> upstream/android-13
/* Returns true of the specified Storm should be included in the dump, false
 * otherwise.
 */
static bool qed_grc_is_storm_included(struct qed_hwfn *p_hwfn,
				      enum dbg_storms storm)
{
	return qed_grc_get_param(p_hwfn, (enum dbg_grc_params)storm) > 0;
}

/* Returns true if the specified memory should be included in the dump, false
 * otherwise.
 */
static bool qed_grc_is_mem_included(struct qed_hwfn *p_hwfn,
				    enum block_id block_id, u8 mem_group_id)
{
<<<<<<< HEAD
	struct block_defs *block = s_block_defs[block_id];
	u8 i;

	/* Check Storm match */
	if (block->associated_to_storm &&
	    !qed_grc_is_storm_included(p_hwfn,
				       (enum dbg_storms)block->storm_id))
		return false;
=======
	const struct dbg_block *block;
	u8 i;

	block = get_dbg_block(p_hwfn, block_id);

	/* If the block is associated with a Storm, check Storm match */
	if (block->associated_storm_letter) {
		enum dbg_storms associated_storm_id =
		    qed_get_id_from_letter(block->associated_storm_letter);

		if (associated_storm_id == MAX_DBG_STORMS ||
		    !qed_grc_is_storm_included(p_hwfn, associated_storm_id))
			return false;
	}
>>>>>>> upstream/android-13

	for (i = 0; i < NUM_BIG_RAM_TYPES; i++) {
		struct big_ram_defs *big_ram = &s_big_ram_defs[i];

		if (mem_group_id == big_ram->mem_group_id ||
		    mem_group_id == big_ram->ram_mem_group_id)
			return qed_grc_is_included(p_hwfn, big_ram->grc_param);
	}

	switch (mem_group_id) {
	case MEM_GROUP_PXP_ILT:
	case MEM_GROUP_PXP_MEM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_PXP);
	case MEM_GROUP_RAM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_RAM);
	case MEM_GROUP_PBUF:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_PBUF);
	case MEM_GROUP_CAU_MEM:
	case MEM_GROUP_CAU_SB:
	case MEM_GROUP_CAU_PI:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_CAU);
<<<<<<< HEAD
=======
	case MEM_GROUP_CAU_MEM_EXT:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_CAU_EXT);
>>>>>>> upstream/android-13
	case MEM_GROUP_QM_MEM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_QM);
	case MEM_GROUP_CFC_MEM:
	case MEM_GROUP_CONN_CFC_MEM:
	case MEM_GROUP_TASK_CFC_MEM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_CFC) ||
		       qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_CM_CTX);
<<<<<<< HEAD
=======
	case MEM_GROUP_DORQ_MEM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_DORQ);
>>>>>>> upstream/android-13
	case MEM_GROUP_IGU_MEM:
	case MEM_GROUP_IGU_MSIX:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_IGU);
	case MEM_GROUP_MULD_MEM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_MULD);
	case MEM_GROUP_PRS_MEM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_PRS);
	case MEM_GROUP_DMAE_MEM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_DMAE);
	case MEM_GROUP_TM_MEM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_TM);
	case MEM_GROUP_SDM_MEM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_SDM);
	case MEM_GROUP_TDIF_CTX:
	case MEM_GROUP_RDIF_CTX:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_DIF);
	case MEM_GROUP_CM_MEM:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_CM);
	case MEM_GROUP_IOR:
		return qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_IOR);
	default:
		return true;
	}
}

/* Stalls all Storms */
static void qed_grc_stall_storms(struct qed_hwfn *p_hwfn,
				 struct qed_ptt *p_ptt, bool stall)
{
	u32 reg_addr;
	u8 storm_id;

	for (storm_id = 0; storm_id < MAX_DBG_STORMS; storm_id++) {
		if (!qed_grc_is_storm_included(p_hwfn,
					       (enum dbg_storms)storm_id))
			continue;

		reg_addr = s_storm_defs[storm_id].sem_fast_mem_addr +
		    SEM_FAST_REG_STALL_0_BB_K2;
		qed_wr(p_hwfn, p_ptt, reg_addr, stall ? 1 : 0);
	}

	msleep(STALL_DELAY_MS);
}

<<<<<<< HEAD
/* Takes all blocks out of reset */
static void qed_grc_unreset_blocks(struct qed_hwfn *p_hwfn,
				   struct qed_ptt *p_ptt)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 reg_val[MAX_DBG_RESET_REGS] = { 0 };
	u32 block_id, i;

	/* Fill reset regs values */
	for (block_id = 0; block_id < MAX_BLOCK_ID; block_id++) {
		struct block_defs *block = s_block_defs[block_id];

		if (block->exists[dev_data->chip_id] && block->has_reset_bit &&
		    block->unreset)
			reg_val[block->reset_reg] |=
			    BIT(block->reset_bit_offset);
	}

	/* Write reset registers */
	for (i = 0; i < MAX_DBG_RESET_REGS; i++) {
		if (!s_reset_regs_defs[i].exists[dev_data->chip_id])
			continue;

		reg_val[i] |=
			s_reset_regs_defs[i].unreset_val[dev_data->chip_id];

		if (reg_val[i])
			qed_wr(p_hwfn,
			       p_ptt,
			       s_reset_regs_defs[i].addr +
			       RESET_REG_UNRESET_OFFSET, reg_val[i]);
=======
/* Takes all blocks out of reset. If rbc_only is true, only RBC clients are
 * taken out of reset.
 */
static void qed_grc_unreset_blocks(struct qed_hwfn *p_hwfn,
				   struct qed_ptt *p_ptt, bool rbc_only)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u8 chip_id = dev_data->chip_id;
	u32 i;

	/* Take RBCs out of reset */
	for (i = 0; i < ARRAY_SIZE(s_rbc_reset_defs); i++)
		if (s_rbc_reset_defs[i].reset_val[dev_data->chip_id])
			qed_wr(p_hwfn,
			       p_ptt,
			       s_rbc_reset_defs[i].reset_reg_addr +
			       RESET_REG_UNRESET_OFFSET,
			       s_rbc_reset_defs[i].reset_val[chip_id]);

	if (!rbc_only) {
		u32 reg_val[NUM_DBG_RESET_REGS] = { 0 };
		u8 reset_reg_id;
		u32 block_id;

		/* Fill reset regs values */
		for (block_id = 0; block_id < NUM_PHYS_BLOCKS; block_id++) {
			bool is_removed, has_reset_reg, unreset_before_dump;
			const struct dbg_block_chip *block;

			block = qed_get_dbg_block_per_chip(p_hwfn,
							   (enum block_id)
							   block_id);
			is_removed =
			    GET_FIELD(block->flags, DBG_BLOCK_CHIP_IS_REMOVED);
			has_reset_reg =
			    GET_FIELD(block->flags,
				      DBG_BLOCK_CHIP_HAS_RESET_REG);
			unreset_before_dump =
			    GET_FIELD(block->flags,
				      DBG_BLOCK_CHIP_UNRESET_BEFORE_DUMP);

			if (!is_removed && has_reset_reg && unreset_before_dump)
				reg_val[block->reset_reg_id] |=
				    BIT(block->reset_reg_bit_offset);
		}

		/* Write reset registers */
		for (reset_reg_id = 0; reset_reg_id < NUM_DBG_RESET_REGS;
		     reset_reg_id++) {
			const struct dbg_reset_reg *reset_reg;
			u32 reset_reg_addr;

			reset_reg = qed_get_dbg_reset_reg(p_hwfn, reset_reg_id);

			if (GET_FIELD
			    (reset_reg->data, DBG_RESET_REG_IS_REMOVED))
				continue;

			if (reg_val[reset_reg_id]) {
				reset_reg_addr =
				    GET_FIELD(reset_reg->data,
					      DBG_RESET_REG_ADDR);
				qed_wr(p_hwfn,
				       p_ptt,
				       DWORDS_TO_BYTES(reset_reg_addr) +
				       RESET_REG_UNRESET_OFFSET,
				       reg_val[reset_reg_id]);
			}
		}
>>>>>>> upstream/android-13
	}
}

/* Returns the attention block data of the specified block */
static const struct dbg_attn_block_type_data *
<<<<<<< HEAD
qed_get_block_attn_data(enum block_id block_id, enum dbg_attn_type attn_type)
{
	const struct dbg_attn_block *base_attn_block_arr =
		(const struct dbg_attn_block *)
		s_dbg_arrays[BIN_BUF_DBG_ATTN_BLOCKS].ptr;
=======
qed_get_block_attn_data(struct qed_hwfn *p_hwfn,
			enum block_id block_id, enum dbg_attn_type attn_type)
{
	const struct dbg_attn_block *base_attn_block_arr =
	    (const struct dbg_attn_block *)
	    p_hwfn->dbg_arrays[BIN_BUF_DBG_ATTN_BLOCKS].ptr;
>>>>>>> upstream/android-13

	return &base_attn_block_arr[block_id].per_type_data[attn_type];
}

/* Returns the attention registers of the specified block */
static const struct dbg_attn_reg *
<<<<<<< HEAD
qed_get_block_attn_regs(enum block_id block_id, enum dbg_attn_type attn_type,
			u8 *num_attn_regs)
{
	const struct dbg_attn_block_type_data *block_type_data =
		qed_get_block_attn_data(block_id, attn_type);

	*num_attn_regs = block_type_data->num_regs;

	return &((const struct dbg_attn_reg *)
		 s_dbg_arrays[BIN_BUF_DBG_ATTN_REGS].ptr)[block_type_data->
							  regs_offset];
=======
qed_get_block_attn_regs(struct qed_hwfn *p_hwfn,
			enum block_id block_id, enum dbg_attn_type attn_type,
			u8 *num_attn_regs)
{
	const struct dbg_attn_block_type_data *block_type_data =
	    qed_get_block_attn_data(p_hwfn, block_id, attn_type);

	*num_attn_regs = block_type_data->num_regs;

	return (const struct dbg_attn_reg *)
		p_hwfn->dbg_arrays[BIN_BUF_DBG_ATTN_REGS].ptr +
		block_type_data->regs_offset;
>>>>>>> upstream/android-13
}

/* For each block, clear the status of all parities */
static void qed_grc_clear_all_prty(struct qed_hwfn *p_hwfn,
				   struct qed_ptt *p_ptt)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	const struct dbg_attn_reg *attn_reg_arr;
	u8 reg_idx, num_attn_regs;
	u32 block_id;

<<<<<<< HEAD
	for (block_id = 0; block_id < MAX_BLOCK_ID; block_id++) {
		if (dev_data->block_in_reset[block_id])
			continue;

		attn_reg_arr = qed_get_block_attn_regs((enum block_id)block_id,
=======
	for (block_id = 0; block_id < NUM_PHYS_BLOCKS; block_id++) {
		if (dev_data->block_in_reset[block_id])
			continue;

		attn_reg_arr = qed_get_block_attn_regs(p_hwfn,
						       (enum block_id)block_id,
>>>>>>> upstream/android-13
						       ATTN_TYPE_PARITY,
						       &num_attn_regs);

		for (reg_idx = 0; reg_idx < num_attn_regs; reg_idx++) {
			const struct dbg_attn_reg *reg_data =
				&attn_reg_arr[reg_idx];
			u16 modes_buf_offset;
			bool eval_mode;

			/* Check mode */
			eval_mode = GET_FIELD(reg_data->mode.data,
					      DBG_MODE_HDR_EVAL_MODE) > 0;
			modes_buf_offset =
				GET_FIELD(reg_data->mode.data,
					  DBG_MODE_HDR_MODES_BUF_OFFSET);

			/* If Mode match: clear parity status */
			if (!eval_mode ||
			    qed_is_mode_match(p_hwfn, &modes_buf_offset))
				qed_rd(p_hwfn, p_ptt,
				       DWORDS_TO_BYTES(reg_data->
						       sts_clr_address));
		}
	}
}

/* Dumps GRC registers section header. Returns the dumped size in dwords.
<<<<<<< HEAD
 * The following parameters are dumped:
 * - count: no. of dumped entries
 * - split_type: split type
 * - split_id: split ID (dumped only if split_id != SPLIT_TYPE_NONE)
 * - param_name: user parameter value (dumped only if param_name != NULL
 *		 and param_val != NULL).
=======
 * the following parameters are dumped:
 * - count: no. of dumped entries
 * - split_type: split type
 * - split_id: split ID (dumped only if split_id != SPLIT_TYPE_NONE)
 * - reg_type_name: register type name (dumped only if reg_type_name != NULL)
>>>>>>> upstream/android-13
 */
static u32 qed_grc_dump_regs_hdr(u32 *dump_buf,
				 bool dump,
				 u32 num_reg_entries,
				 enum init_split_types split_type,
<<<<<<< HEAD
				 u8 split_id,
				 const char *param_name, const char *param_val)
{
	u8 num_params = 2 +
	    (split_type != SPLIT_TYPE_NONE ? 1 : 0) + (param_name ? 1 : 0);
=======
				 u8 split_id, const char *reg_type_name)
{
	u8 num_params = 2 +
	    (split_type != SPLIT_TYPE_NONE ? 1 : 0) + (reg_type_name ? 1 : 0);
>>>>>>> upstream/android-13
	u32 offset = 0;

	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "grc_regs", num_params);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump, "count", num_reg_entries);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "split",
				     s_split_type_defs[split_type].name);
	if (split_type != SPLIT_TYPE_NONE)
		offset += qed_dump_num_param(dump_buf + offset,
					     dump, "id", split_id);
<<<<<<< HEAD
	if (param_name && param_val)
		offset += qed_dump_str_param(dump_buf + offset,
					     dump, param_name, param_val);
=======
	if (reg_type_name)
		offset += qed_dump_str_param(dump_buf + offset,
					     dump, "type", reg_type_name);
>>>>>>> upstream/android-13

	return offset;
}

/* Reads the specified registers into the specified buffer.
 * The addr and len arguments are specified in dwords.
 */
void qed_read_regs(struct qed_hwfn *p_hwfn,
		   struct qed_ptt *p_ptt, u32 *buf, u32 addr, u32 len)
{
	u32 i;

	for (i = 0; i < len; i++)
		buf[i] = qed_rd(p_hwfn, p_ptt, DWORDS_TO_BYTES(addr + i));
}

/* Dumps the GRC registers in the specified address range.
 * Returns the dumped size in dwords.
 * The addr and len arguments are specified in dwords.
 */
static u32 qed_grc_dump_addr_range(struct qed_hwfn *p_hwfn,
				   struct qed_ptt *p_ptt,
				   u32 *dump_buf,
				   bool dump, u32 addr, u32 len, bool wide_bus,
				   enum init_split_types split_type,
				   u8 split_id)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u8 port_id = 0, pf_id = 0, vf_id = 0, fid = 0;
<<<<<<< HEAD
=======
	bool read_using_dmae = false;
	u32 thresh;
>>>>>>> upstream/android-13

	if (!dump)
		return len;

<<<<<<< HEAD
	/* Print log if needed */
	dev_data->num_regs_read += len;
	if (dev_data->num_regs_read >=
	    s_platform_defs[dev_data->platform_id].log_thresh) {
		DP_VERBOSE(p_hwfn,
			   QED_MSG_DEBUG,
			   "Dumping %d registers...\n",
			   dev_data->num_regs_read);
		dev_data->num_regs_read = 0;
	}

=======
>>>>>>> upstream/android-13
	switch (split_type) {
	case SPLIT_TYPE_PORT:
		port_id = split_id;
		break;
	case SPLIT_TYPE_PF:
		pf_id = split_id;
		break;
	case SPLIT_TYPE_PORT_PF:
		port_id = split_id / dev_data->num_pfs_per_port;
		pf_id = port_id + dev_data->num_ports *
		    (split_id % dev_data->num_pfs_per_port);
		break;
	case SPLIT_TYPE_VF:
		vf_id = split_id;
		break;
	default:
		break;
	}

	/* Try reading using DMAE */
<<<<<<< HEAD
	if (dev_data->use_dmae && split_type == SPLIT_TYPE_NONE &&
	    (len >= s_platform_defs[dev_data->platform_id].dmae_thresh ||
	     wide_bus)) {
		if (!qed_dmae_grc2host(p_hwfn, p_ptt, DWORDS_TO_BYTES(addr),
				       (u64)(uintptr_t)(dump_buf), len, 0))
			return len;
		dev_data->use_dmae = 0;
		DP_VERBOSE(p_hwfn,
			   QED_MSG_DEBUG,
			   "Failed reading from chip using DMAE, using GRC instead\n");
	}

	/* If not read using DMAE, read using GRC */

	/* Set pretend */
	if (split_type != dev_data->pretend.split_type || split_id !=
	    dev_data->pretend.split_id) {
=======
	if (dev_data->use_dmae && split_type != SPLIT_TYPE_VF &&
	    (len >= s_hw_type_defs[dev_data->hw_type].dmae_thresh ||
	     (PROTECT_WIDE_BUS && wide_bus))) {
		struct qed_dmae_params dmae_params;

		/* Set DMAE params */
		memset(&dmae_params, 0, sizeof(dmae_params));
		SET_FIELD(dmae_params.flags, QED_DMAE_PARAMS_COMPLETION_DST, 1);
		switch (split_type) {
		case SPLIT_TYPE_PORT:
			SET_FIELD(dmae_params.flags, QED_DMAE_PARAMS_PORT_VALID,
				  1);
			dmae_params.port_id = port_id;
			break;
		case SPLIT_TYPE_PF:
			SET_FIELD(dmae_params.flags,
				  QED_DMAE_PARAMS_SRC_PF_VALID, 1);
			dmae_params.src_pfid = pf_id;
			break;
		case SPLIT_TYPE_PORT_PF:
			SET_FIELD(dmae_params.flags, QED_DMAE_PARAMS_PORT_VALID,
				  1);
			SET_FIELD(dmae_params.flags,
				  QED_DMAE_PARAMS_SRC_PF_VALID, 1);
			dmae_params.port_id = port_id;
			dmae_params.src_pfid = pf_id;
			break;
		default:
			break;
		}

		/* Execute DMAE command */
		read_using_dmae = !qed_dmae_grc2host(p_hwfn,
						     p_ptt,
						     DWORDS_TO_BYTES(addr),
						     (u64)(uintptr_t)(dump_buf),
						     len, &dmae_params);
		if (!read_using_dmae) {
			dev_data->use_dmae = 0;
			DP_VERBOSE(p_hwfn,
				   QED_MSG_DEBUG,
				   "Failed reading from chip using DMAE, using GRC instead\n");
		}
	}

	if (read_using_dmae)
		goto print_log;

	/* If not read using DMAE, read using GRC */

	/* Set pretend */
	if (split_type != dev_data->pretend.split_type ||
	    split_id != dev_data->pretend.split_id) {
>>>>>>> upstream/android-13
		switch (split_type) {
		case SPLIT_TYPE_PORT:
			qed_port_pretend(p_hwfn, p_ptt, port_id);
			break;
		case SPLIT_TYPE_PF:
<<<<<<< HEAD
			fid = pf_id << PXP_PRETEND_CONCRETE_FID_PFID_SHIFT;
			qed_fid_pretend(p_hwfn, p_ptt, fid);
			break;
		case SPLIT_TYPE_PORT_PF:
			fid = pf_id << PXP_PRETEND_CONCRETE_FID_PFID_SHIFT;
			qed_port_fid_pretend(p_hwfn, p_ptt, port_id, fid);
			break;
		case SPLIT_TYPE_VF:
			fid = BIT(PXP_PRETEND_CONCRETE_FID_VFVALID_SHIFT) |
			      (vf_id << PXP_PRETEND_CONCRETE_FID_VFID_SHIFT);
=======
			fid = FIELD_VALUE(PXP_PRETEND_CONCRETE_FID_PFID,
					  pf_id);
			qed_fid_pretend(p_hwfn, p_ptt, fid);
			break;
		case SPLIT_TYPE_PORT_PF:
			fid = FIELD_VALUE(PXP_PRETEND_CONCRETE_FID_PFID,
					  pf_id);
			qed_port_fid_pretend(p_hwfn, p_ptt, port_id, fid);
			break;
		case SPLIT_TYPE_VF:
			fid = FIELD_VALUE(PXP_PRETEND_CONCRETE_FID_VFVALID, 1)
			      | FIELD_VALUE(PXP_PRETEND_CONCRETE_FID_VFID,
					  vf_id);
>>>>>>> upstream/android-13
			qed_fid_pretend(p_hwfn, p_ptt, fid);
			break;
		default:
			break;
		}

		dev_data->pretend.split_type = (u8)split_type;
		dev_data->pretend.split_id = split_id;
	}

	/* Read registers using GRC */
	qed_read_regs(p_hwfn, p_ptt, dump_buf, addr, len);

<<<<<<< HEAD
=======
print_log:
	/* Print log */
	dev_data->num_regs_read += len;
	thresh = s_hw_type_defs[dev_data->hw_type].log_thresh;
	if ((dev_data->num_regs_read / thresh) >
	    ((dev_data->num_regs_read - len) / thresh))
		DP_VERBOSE(p_hwfn,
			   QED_MSG_DEBUG,
			   "Dumped %d registers...\n", dev_data->num_regs_read);

>>>>>>> upstream/android-13
	return len;
}

/* Dumps GRC registers sequence header. Returns the dumped size in dwords.
 * The addr and len arguments are specified in dwords.
 */
static u32 qed_grc_dump_reg_entry_hdr(u32 *dump_buf,
				      bool dump, u32 addr, u32 len)
{
	if (dump)
		*dump_buf = addr | (len << REG_DUMP_LEN_SHIFT);

	return 1;
}

/* Dumps GRC registers sequence. Returns the dumped size in dwords.
 * The addr and len arguments are specified in dwords.
 */
static u32 qed_grc_dump_reg_entry(struct qed_hwfn *p_hwfn,
				  struct qed_ptt *p_ptt,
				  u32 *dump_buf,
				  bool dump, u32 addr, u32 len, bool wide_bus,
				  enum init_split_types split_type, u8 split_id)
{
	u32 offset = 0;

	offset += qed_grc_dump_reg_entry_hdr(dump_buf, dump, addr, len);
	offset += qed_grc_dump_addr_range(p_hwfn,
					  p_ptt,
					  dump_buf + offset,
					  dump, addr, len, wide_bus,
					  split_type, split_id);

	return offset;
}

/* Dumps GRC registers sequence with skip cycle.
 * Returns the dumped size in dwords.
 * - addr:	start GRC address in dwords
 * - total_len:	total no. of dwords to dump
 * - read_len:	no. consecutive dwords to read
 * - skip_len:	no. of dwords to skip (and fill with zeros)
 */
static u32 qed_grc_dump_reg_entry_skip(struct qed_hwfn *p_hwfn,
				       struct qed_ptt *p_ptt,
				       u32 *dump_buf,
				       bool dump,
				       u32 addr,
				       u32 total_len,
				       u32 read_len, u32 skip_len)
{
	u32 offset = 0, reg_offset = 0;

	offset += qed_grc_dump_reg_entry_hdr(dump_buf, dump, addr, total_len);

	if (!dump)
		return offset + total_len;

	while (reg_offset < total_len) {
		u32 curr_len = min_t(u32, read_len, total_len - reg_offset);

		offset += qed_grc_dump_addr_range(p_hwfn,
						  p_ptt,
						  dump_buf + offset,
						  dump,  addr, curr_len, false,
						  SPLIT_TYPE_NONE, 0);
		reg_offset += curr_len;
		addr += curr_len;

		if (reg_offset < total_len) {
			curr_len = min_t(u32, skip_len, total_len - skip_len);
			memset(dump_buf + offset, 0, DWORDS_TO_BYTES(curr_len));
			offset += curr_len;
			reg_offset += curr_len;
			addr += curr_len;
		}
	}

	return offset;
}

/* Dumps GRC registers entries. Returns the dumped size in dwords. */
static u32 qed_grc_dump_regs_entries(struct qed_hwfn *p_hwfn,
				     struct qed_ptt *p_ptt,
<<<<<<< HEAD
				     struct dbg_array input_regs_arr,
=======
				     struct virt_mem_desc input_regs_arr,
>>>>>>> upstream/android-13
				     u32 *dump_buf,
				     bool dump,
				     enum init_split_types split_type,
				     u8 split_id,
				     bool block_enable[MAX_BLOCK_ID],
				     u32 *num_dumped_reg_entries)
{
	u32 i, offset = 0, input_offset = 0;
	bool mode_match = true;

	*num_dumped_reg_entries = 0;

<<<<<<< HEAD
	while (input_offset < input_regs_arr.size_in_dwords) {
		const struct dbg_dump_cond_hdr *cond_hdr =
		    (const struct dbg_dump_cond_hdr *)
		    &input_regs_arr.ptr[input_offset++];
=======
	while (input_offset < BYTES_TO_DWORDS(input_regs_arr.size)) {
		const struct dbg_dump_cond_hdr *cond_hdr =
		    (const struct dbg_dump_cond_hdr *)
		    input_regs_arr.ptr + input_offset++;
>>>>>>> upstream/android-13
		u16 modes_buf_offset;
		bool eval_mode;

		/* Check mode/block */
		eval_mode = GET_FIELD(cond_hdr->mode.data,
				      DBG_MODE_HDR_EVAL_MODE) > 0;
		if (eval_mode) {
			modes_buf_offset =
				GET_FIELD(cond_hdr->mode.data,
					  DBG_MODE_HDR_MODES_BUF_OFFSET);
			mode_match = qed_is_mode_match(p_hwfn,
						       &modes_buf_offset);
		}

		if (!mode_match || !block_enable[cond_hdr->block_id]) {
			input_offset += cond_hdr->data_size;
			continue;
		}

		for (i = 0; i < cond_hdr->data_size; i++, input_offset++) {
			const struct dbg_dump_reg *reg =
			    (const struct dbg_dump_reg *)
<<<<<<< HEAD
			    &input_regs_arr.ptr[input_offset];
=======
			    input_regs_arr.ptr + input_offset;
>>>>>>> upstream/android-13
			u32 addr, len;
			bool wide_bus;

			addr = GET_FIELD(reg->data, DBG_DUMP_REG_ADDRESS);
			len = GET_FIELD(reg->data, DBG_DUMP_REG_LENGTH);
			wide_bus = GET_FIELD(reg->data, DBG_DUMP_REG_WIDE_BUS);
			offset += qed_grc_dump_reg_entry(p_hwfn,
							 p_ptt,
							 dump_buf + offset,
							 dump,
							 addr,
							 len,
							 wide_bus,
							 split_type, split_id);
			(*num_dumped_reg_entries)++;
		}
	}

	return offset;
}

/* Dumps GRC registers entries. Returns the dumped size in dwords. */
static u32 qed_grc_dump_split_data(struct qed_hwfn *p_hwfn,
				   struct qed_ptt *p_ptt,
<<<<<<< HEAD
				   struct dbg_array input_regs_arr,
=======
				   struct virt_mem_desc input_regs_arr,
>>>>>>> upstream/android-13
				   u32 *dump_buf,
				   bool dump,
				   bool block_enable[MAX_BLOCK_ID],
				   enum init_split_types split_type,
<<<<<<< HEAD
				   u8 split_id,
				   const char *param_name,
				   const char *param_val)
=======
				   u8 split_id, const char *reg_type_name)
>>>>>>> upstream/android-13
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	enum init_split_types hdr_split_type = split_type;
	u32 num_dumped_reg_entries, offset;
	u8 hdr_split_id = split_id;

	/* In PORT_PF split type, print a port split header */
	if (split_type == SPLIT_TYPE_PORT_PF) {
		hdr_split_type = SPLIT_TYPE_PORT;
		hdr_split_id = split_id / dev_data->num_pfs_per_port;
	}

	/* Calculate register dump header size (and skip it for now) */
	offset = qed_grc_dump_regs_hdr(dump_buf,
				       false,
				       0,
				       hdr_split_type,
<<<<<<< HEAD
				       hdr_split_id, param_name, param_val);
=======
				       hdr_split_id, reg_type_name);
>>>>>>> upstream/android-13

	/* Dump registers */
	offset += qed_grc_dump_regs_entries(p_hwfn,
					    p_ptt,
					    input_regs_arr,
					    dump_buf + offset,
					    dump,
					    split_type,
					    split_id,
					    block_enable,
					    &num_dumped_reg_entries);

	/* Write register dump header */
	if (dump && num_dumped_reg_entries > 0)
		qed_grc_dump_regs_hdr(dump_buf,
				      dump,
				      num_dumped_reg_entries,
				      hdr_split_type,
<<<<<<< HEAD
				      hdr_split_id, param_name, param_val);
=======
				      hdr_split_id, reg_type_name);
>>>>>>> upstream/android-13

	return num_dumped_reg_entries > 0 ? offset : 0;
}

/* Dumps registers according to the input registers array. Returns the dumped
 * size in dwords.
 */
static u32 qed_grc_dump_registers(struct qed_hwfn *p_hwfn,
				  struct qed_ptt *p_ptt,
				  u32 *dump_buf,
				  bool dump,
				  bool block_enable[MAX_BLOCK_ID],
<<<<<<< HEAD
				  const char *param_name, const char *param_val)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 offset = 0, input_offset = 0;
	u16 fid;
	while (input_offset <
	       s_dbg_arrays[BIN_BUF_DBG_DUMP_REG].size_in_dwords) {
		const struct dbg_dump_split_hdr *split_hdr;
		struct dbg_array curr_input_regs_arr;
=======
				  const char *reg_type_name)
{
	struct virt_mem_desc *dbg_buf =
	    &p_hwfn->dbg_arrays[BIN_BUF_DBG_DUMP_REG];
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 offset = 0, input_offset = 0;

	while (input_offset < BYTES_TO_DWORDS(dbg_buf->size)) {
		const struct dbg_dump_split_hdr *split_hdr;
		struct virt_mem_desc curr_input_regs_arr;
>>>>>>> upstream/android-13
		enum init_split_types split_type;
		u16 split_count = 0;
		u32 split_data_size;
		u8 split_id;

		split_hdr =
<<<<<<< HEAD
			(const struct dbg_dump_split_hdr *)
			&s_dbg_arrays[BIN_BUF_DBG_DUMP_REG].ptr[input_offset++];
		split_type =
			GET_FIELD(split_hdr->hdr,
				  DBG_DUMP_SPLIT_HDR_SPLIT_TYPE_ID);
		split_data_size =
			GET_FIELD(split_hdr->hdr,
				  DBG_DUMP_SPLIT_HDR_DATA_SIZE);
		curr_input_regs_arr.ptr =
			&s_dbg_arrays[BIN_BUF_DBG_DUMP_REG].ptr[input_offset];
		curr_input_regs_arr.size_in_dwords = split_data_size;
=======
		    (const struct dbg_dump_split_hdr *)
		    dbg_buf->ptr + input_offset++;
		split_type =
		    GET_FIELD(split_hdr->hdr,
			      DBG_DUMP_SPLIT_HDR_SPLIT_TYPE_ID);
		split_data_size = GET_FIELD(split_hdr->hdr,
					    DBG_DUMP_SPLIT_HDR_DATA_SIZE);
		curr_input_regs_arr.ptr =
		    (u32 *)p_hwfn->dbg_arrays[BIN_BUF_DBG_DUMP_REG].ptr +
		    input_offset;
		curr_input_regs_arr.size = DWORDS_TO_BYTES(split_data_size);
>>>>>>> upstream/android-13

		switch (split_type) {
		case SPLIT_TYPE_NONE:
			split_count = 1;
			break;
		case SPLIT_TYPE_PORT:
			split_count = dev_data->num_ports;
			break;
		case SPLIT_TYPE_PF:
		case SPLIT_TYPE_PORT_PF:
			split_count = dev_data->num_ports *
			    dev_data->num_pfs_per_port;
			break;
		case SPLIT_TYPE_VF:
			split_count = dev_data->num_vfs;
			break;
		default:
			return 0;
		}

		for (split_id = 0; split_id < split_count; split_id++)
			offset += qed_grc_dump_split_data(p_hwfn, p_ptt,
							  curr_input_regs_arr,
							  dump_buf + offset,
							  dump, block_enable,
							  split_type,
							  split_id,
<<<<<<< HEAD
							  param_name,
							  param_val);
=======
							  reg_type_name);
>>>>>>> upstream/android-13

		input_offset += split_data_size;
	}

	/* Cancel pretends (pretend to original PF) */
	if (dump) {
<<<<<<< HEAD
		fid = p_hwfn->rel_pf_id << PXP_PRETEND_CONCRETE_FID_PFID_SHIFT;
		qed_fid_pretend(p_hwfn, p_ptt, fid);
=======
		qed_fid_pretend(p_hwfn, p_ptt,
				FIELD_VALUE(PXP_PRETEND_CONCRETE_FID_PFID,
					    p_hwfn->rel_pf_id));
>>>>>>> upstream/android-13
		dev_data->pretend.split_type = SPLIT_TYPE_NONE;
		dev_data->pretend.split_id = 0;
	}

	return offset;
}

/* Dump reset registers. Returns the dumped size in dwords. */
static u32 qed_grc_dump_reset_regs(struct qed_hwfn *p_hwfn,
				   struct qed_ptt *p_ptt,
				   u32 *dump_buf, bool dump)
{
<<<<<<< HEAD
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 i, offset = 0, num_regs = 0;

	/* Calculate header size */
	offset += qed_grc_dump_regs_hdr(dump_buf,
					false, 0,
					SPLIT_TYPE_NONE, 0, NULL, NULL);

	/* Write reset registers */
	for (i = 0; i < MAX_DBG_RESET_REGS; i++) {
		if (!s_reset_regs_defs[i].exists[dev_data->chip_id])
			continue;

=======
	u32 offset = 0, num_regs = 0;
	u8 reset_reg_id;

	/* Calculate header size */
	offset += qed_grc_dump_regs_hdr(dump_buf,
					false,
					0, SPLIT_TYPE_NONE, 0, "RESET_REGS");

	/* Write reset registers */
	for (reset_reg_id = 0; reset_reg_id < NUM_DBG_RESET_REGS;
	     reset_reg_id++) {
		const struct dbg_reset_reg *reset_reg;
		u32 reset_reg_addr;

		reset_reg = qed_get_dbg_reset_reg(p_hwfn, reset_reg_id);

		if (GET_FIELD(reset_reg->data, DBG_RESET_REG_IS_REMOVED))
			continue;

		reset_reg_addr = GET_FIELD(reset_reg->data, DBG_RESET_REG_ADDR);
>>>>>>> upstream/android-13
		offset += qed_grc_dump_reg_entry(p_hwfn,
						 p_ptt,
						 dump_buf + offset,
						 dump,
<<<<<<< HEAD
						 BYTES_TO_DWORDS
						 (s_reset_regs_defs[i].addr), 1,
						 false, SPLIT_TYPE_NONE, 0);
=======
						 reset_reg_addr,
						 1, false, SPLIT_TYPE_NONE, 0);
>>>>>>> upstream/android-13
		num_regs++;
	}

	/* Write header */
	if (dump)
		qed_grc_dump_regs_hdr(dump_buf,
				      true, num_regs, SPLIT_TYPE_NONE,
<<<<<<< HEAD
				      0, NULL, NULL);
=======
				      0, "RESET_REGS");
>>>>>>> upstream/android-13

	return offset;
}

/* Dump registers that are modified during GRC Dump and therefore must be
 * dumped first. Returns the dumped size in dwords.
 */
static u32 qed_grc_dump_modified_regs(struct qed_hwfn *p_hwfn,
				      struct qed_ptt *p_ptt,
				      u32 *dump_buf, bool dump)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
<<<<<<< HEAD
	u32 block_id, offset = 0, num_reg_entries = 0;
	const struct dbg_attn_reg *attn_reg_arr;
	u8 storm_id, reg_idx, num_attn_regs;

	/* Calculate header size */
	offset += qed_grc_dump_regs_hdr(dump_buf,
					false, 0, SPLIT_TYPE_NONE,
					0, NULL, NULL);

	/* Write parity registers */
	for (block_id = 0; block_id < MAX_BLOCK_ID; block_id++) {
		if (dev_data->block_in_reset[block_id] && dump)
			continue;

		attn_reg_arr = qed_get_block_attn_regs((enum block_id)block_id,
=======
	u32 block_id, offset = 0, stall_regs_offset;
	const struct dbg_attn_reg *attn_reg_arr;
	u8 storm_id, reg_idx, num_attn_regs;
	u32 num_reg_entries = 0;

	/* Write empty header for attention registers */
	offset += qed_grc_dump_regs_hdr(dump_buf,
					false,
					0, SPLIT_TYPE_NONE, 0, "ATTN_REGS");

	/* Write parity registers */
	for (block_id = 0; block_id < NUM_PHYS_BLOCKS; block_id++) {
		if (dev_data->block_in_reset[block_id] && dump)
			continue;

		attn_reg_arr = qed_get_block_attn_regs(p_hwfn,
						       (enum block_id)block_id,
>>>>>>> upstream/android-13
						       ATTN_TYPE_PARITY,
						       &num_attn_regs);

		for (reg_idx = 0; reg_idx < num_attn_regs; reg_idx++) {
			const struct dbg_attn_reg *reg_data =
				&attn_reg_arr[reg_idx];
			u16 modes_buf_offset;
			bool eval_mode;
			u32 addr;

			/* Check mode */
			eval_mode = GET_FIELD(reg_data->mode.data,
					      DBG_MODE_HDR_EVAL_MODE) > 0;
			modes_buf_offset =
				GET_FIELD(reg_data->mode.data,
					  DBG_MODE_HDR_MODES_BUF_OFFSET);
			if (eval_mode &&
			    !qed_is_mode_match(p_hwfn, &modes_buf_offset))
				continue;

			/* Mode match: read & dump registers */
			addr = reg_data->mask_address;
			offset += qed_grc_dump_reg_entry(p_hwfn,
							 p_ptt,
							 dump_buf + offset,
							 dump,
							 addr,
							 1, false,
							 SPLIT_TYPE_NONE, 0);
			addr = GET_FIELD(reg_data->data,
					 DBG_ATTN_REG_STS_ADDRESS);
			offset += qed_grc_dump_reg_entry(p_hwfn,
							 p_ptt,
							 dump_buf + offset,
							 dump,
							 addr,
							 1, false,
							 SPLIT_TYPE_NONE, 0);
			num_reg_entries += 2;
		}
	}

<<<<<<< HEAD
	/* Write Storm stall status registers */
	for (storm_id = 0; storm_id < MAX_DBG_STORMS; storm_id++) {
		struct storm_defs *storm = &s_storm_defs[storm_id];
		u32 addr;

		if (dev_data->block_in_reset[storm->block_id] && dump)
			continue;

		addr =
		    BYTES_TO_DWORDS(s_storm_defs[storm_id].sem_fast_mem_addr +
=======
	/* Overwrite header for attention registers */
	if (dump)
		qed_grc_dump_regs_hdr(dump_buf,
				      true,
				      num_reg_entries,
				      SPLIT_TYPE_NONE, 0, "ATTN_REGS");

	/* Write empty header for stall registers */
	stall_regs_offset = offset;
	offset += qed_grc_dump_regs_hdr(dump_buf,
					false, 0, SPLIT_TYPE_NONE, 0, "REGS");

	/* Write Storm stall status registers */
	for (storm_id = 0, num_reg_entries = 0; storm_id < MAX_DBG_STORMS;
	     storm_id++) {
		struct storm_defs *storm = &s_storm_defs[storm_id];
		u32 addr;

		if (dev_data->block_in_reset[storm->sem_block_id] && dump)
			continue;

		addr =
		    BYTES_TO_DWORDS(storm->sem_fast_mem_addr +
>>>>>>> upstream/android-13
				    SEM_FAST_REG_STALLED);
		offset += qed_grc_dump_reg_entry(p_hwfn,
						 p_ptt,
						 dump_buf + offset,
						 dump,
						 addr,
						 1,
						 false, SPLIT_TYPE_NONE, 0);
		num_reg_entries++;
	}

<<<<<<< HEAD
	/* Write header */
	if (dump)
		qed_grc_dump_regs_hdr(dump_buf,
				      true,
				      num_reg_entries, SPLIT_TYPE_NONE,
				      0, NULL, NULL);
=======
	/* Overwrite header for stall registers */
	if (dump)
		qed_grc_dump_regs_hdr(dump_buf + stall_regs_offset,
				      true,
				      num_reg_entries,
				      SPLIT_TYPE_NONE, 0, "REGS");
>>>>>>> upstream/android-13

	return offset;
}

/* Dumps registers that can't be represented in the debug arrays */
static u32 qed_grc_dump_special_regs(struct qed_hwfn *p_hwfn,
				     struct qed_ptt *p_ptt,
				     u32 *dump_buf, bool dump)
{
	u32 offset = 0, addr;

	offset += qed_grc_dump_regs_hdr(dump_buf,
<<<<<<< HEAD
					dump, 2, SPLIT_TYPE_NONE, 0,
					NULL, NULL);
=======
					dump, 2, SPLIT_TYPE_NONE, 0, "REGS");
>>>>>>> upstream/android-13

	/* Dump R/TDIF_REG_DEBUG_ERROR_INFO_SIZE (every 8'th register should be
	 * skipped).
	 */
	addr = BYTES_TO_DWORDS(RDIF_REG_DEBUG_ERROR_INFO);
	offset += qed_grc_dump_reg_entry_skip(p_hwfn,
					      p_ptt,
					      dump_buf + offset,
					      dump,
					      addr,
					      RDIF_REG_DEBUG_ERROR_INFO_SIZE,
					      7,
					      1);
	addr = BYTES_TO_DWORDS(TDIF_REG_DEBUG_ERROR_INFO);
	offset +=
	    qed_grc_dump_reg_entry_skip(p_hwfn,
					p_ptt,
					dump_buf + offset,
					dump,
					addr,
					TDIF_REG_DEBUG_ERROR_INFO_SIZE,
					7,
					1);

	return offset;
}

/* Dumps a GRC memory header (section and params). Returns the dumped size in
 * dwords. The following parameters are dumped:
 * - name:	   dumped only if it's not NULL.
 * - addr:	   in dwords, dumped only if name is NULL.
 * - len:	   in dwords, always dumped.
 * - width:	   dumped if it's not zero.
 * - packed:	   dumped only if it's not false.
 * - mem_group:	   always dumped.
 * - is_storm:	   true only if the memory is related to a Storm.
 * - storm_letter: valid only if is_storm is true.
 *
 */
static u32 qed_grc_dump_mem_hdr(struct qed_hwfn *p_hwfn,
				u32 *dump_buf,
				bool dump,
				const char *name,
				u32 addr,
				u32 len,
				u32 bit_width,
				bool packed,
<<<<<<< HEAD
				const char *mem_group,
				bool is_storm, char storm_letter)
=======
				const char *mem_group, char storm_letter)
>>>>>>> upstream/android-13
{
	u8 num_params = 3;
	u32 offset = 0;
	char buf[64];

	if (!len)
		DP_NOTICE(p_hwfn,
			  "Unexpected GRC Dump error: dumped memory size must be non-zero\n");

	if (bit_width)
		num_params++;
	if (packed)
		num_params++;

	/* Dump section header */
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "grc_mem", num_params);

	if (name) {
		/* Dump name */
<<<<<<< HEAD
		if (is_storm) {
=======
		if (storm_letter) {
>>>>>>> upstream/android-13
			strcpy(buf, "?STORM_");
			buf[0] = storm_letter;
			strcpy(buf + strlen(buf), name);
		} else {
			strcpy(buf, name);
		}

		offset += qed_dump_str_param(dump_buf + offset,
					     dump, "name", buf);
	} else {
		/* Dump address */
		u32 addr_in_bytes = DWORDS_TO_BYTES(addr);

		offset += qed_dump_num_param(dump_buf + offset,
					     dump, "addr", addr_in_bytes);
	}

	/* Dump len */
	offset += qed_dump_num_param(dump_buf + offset, dump, "len", len);

	/* Dump bit width */
	if (bit_width)
		offset += qed_dump_num_param(dump_buf + offset,
					     dump, "width", bit_width);

	/* Dump packed */
	if (packed)
		offset += qed_dump_num_param(dump_buf + offset,
					     dump, "packed", 1);

	/* Dump reg type */
<<<<<<< HEAD
	if (is_storm) {
=======
	if (storm_letter) {
>>>>>>> upstream/android-13
		strcpy(buf, "?STORM_");
		buf[0] = storm_letter;
		strcpy(buf + strlen(buf), mem_group);
	} else {
		strcpy(buf, mem_group);
	}

	offset += qed_dump_str_param(dump_buf + offset, dump, "type", buf);

	return offset;
}

/* Dumps a single GRC memory. If name is NULL, the memory is stored by address.
 * Returns the dumped size in dwords.
 * The addr and len arguments are specified in dwords.
 */
static u32 qed_grc_dump_mem(struct qed_hwfn *p_hwfn,
			    struct qed_ptt *p_ptt,
			    u32 *dump_buf,
			    bool dump,
			    const char *name,
			    u32 addr,
			    u32 len,
			    bool wide_bus,
			    u32 bit_width,
			    bool packed,
<<<<<<< HEAD
			    const char *mem_group,
			    bool is_storm, char storm_letter)
=======
			    const char *mem_group, char storm_letter)
>>>>>>> upstream/android-13
{
	u32 offset = 0;

	offset += qed_grc_dump_mem_hdr(p_hwfn,
				       dump_buf + offset,
				       dump,
				       name,
				       addr,
				       len,
				       bit_width,
<<<<<<< HEAD
				       packed,
				       mem_group, is_storm, storm_letter);
=======
				       packed, mem_group, storm_letter);
>>>>>>> upstream/android-13
	offset += qed_grc_dump_addr_range(p_hwfn,
					  p_ptt,
					  dump_buf + offset,
					  dump, addr, len, wide_bus,
					  SPLIT_TYPE_NONE, 0);

	return offset;
}

/* Dumps GRC memories entries. Returns the dumped size in dwords. */
static u32 qed_grc_dump_mem_entries(struct qed_hwfn *p_hwfn,
				    struct qed_ptt *p_ptt,
<<<<<<< HEAD
				    struct dbg_array input_mems_arr,
=======
				    struct virt_mem_desc input_mems_arr,
>>>>>>> upstream/android-13
				    u32 *dump_buf, bool dump)
{
	u32 i, offset = 0, input_offset = 0;
	bool mode_match = true;

<<<<<<< HEAD
	while (input_offset < input_mems_arr.size_in_dwords) {
=======
	while (input_offset < BYTES_TO_DWORDS(input_mems_arr.size)) {
>>>>>>> upstream/android-13
		const struct dbg_dump_cond_hdr *cond_hdr;
		u16 modes_buf_offset;
		u32 num_entries;
		bool eval_mode;

<<<<<<< HEAD
		cond_hdr = (const struct dbg_dump_cond_hdr *)
			   &input_mems_arr.ptr[input_offset++];
=======
		cond_hdr =
		    (const struct dbg_dump_cond_hdr *)input_mems_arr.ptr +
		    input_offset++;
>>>>>>> upstream/android-13
		num_entries = cond_hdr->data_size / MEM_DUMP_ENTRY_SIZE_DWORDS;

		/* Check required mode */
		eval_mode = GET_FIELD(cond_hdr->mode.data,
				      DBG_MODE_HDR_EVAL_MODE) > 0;
		if (eval_mode) {
			modes_buf_offset =
				GET_FIELD(cond_hdr->mode.data,
					  DBG_MODE_HDR_MODES_BUF_OFFSET);
			mode_match = qed_is_mode_match(p_hwfn,
						       &modes_buf_offset);
		}

		if (!mode_match) {
			input_offset += cond_hdr->data_size;
			continue;
		}

		for (i = 0; i < num_entries;
		     i++, input_offset += MEM_DUMP_ENTRY_SIZE_DWORDS) {
			const struct dbg_dump_mem *mem =
<<<<<<< HEAD
				(const struct dbg_dump_mem *)
				&input_mems_arr.ptr[input_offset];
			u8 mem_group_id = GET_FIELD(mem->dword0,
						    DBG_DUMP_MEM_MEM_GROUP_ID);
			bool is_storm = false, mem_wide_bus;
			enum dbg_grc_params grc_param;
			char storm_letter = 'a';
			enum block_id block_id;
			u32 mem_addr, mem_len;

=======
			    (const struct dbg_dump_mem *)((u32 *)
							  input_mems_arr.ptr
							  + input_offset);
			const struct dbg_block *block;
			char storm_letter = 0;
			u32 mem_addr, mem_len;
			bool mem_wide_bus;
			u8 mem_group_id;

			mem_group_id = GET_FIELD(mem->dword0,
						 DBG_DUMP_MEM_MEM_GROUP_ID);
>>>>>>> upstream/android-13
			if (mem_group_id >= MEM_GROUPS_NUM) {
				DP_NOTICE(p_hwfn, "Invalid mem_group_id\n");
				return 0;
			}

<<<<<<< HEAD
			block_id = (enum block_id)cond_hdr->block_id;
			if (!qed_grc_is_mem_included(p_hwfn,
						     block_id,
=======
			if (!qed_grc_is_mem_included(p_hwfn,
						     (enum block_id)
						     cond_hdr->block_id,
>>>>>>> upstream/android-13
						     mem_group_id))
				continue;

			mem_addr = GET_FIELD(mem->dword0, DBG_DUMP_MEM_ADDRESS);
			mem_len = GET_FIELD(mem->dword1, DBG_DUMP_MEM_LENGTH);
			mem_wide_bus = GET_FIELD(mem->dword1,
						 DBG_DUMP_MEM_WIDE_BUS);

<<<<<<< HEAD
			/* Update memory length for CCFC/TCFC memories
			 * according to number of LCIDs/LTIDs.
			 */
			if (mem_group_id == MEM_GROUP_CONN_CFC_MEM) {
				if (mem_len % MAX_LCIDS) {
					DP_NOTICE(p_hwfn,
						  "Invalid CCFC connection memory size\n");
					return 0;
				}

				grc_param = DBG_GRC_PARAM_NUM_LCIDS;
				mem_len = qed_grc_get_param(p_hwfn, grc_param) *
					  (mem_len / MAX_LCIDS);
			} else if (mem_group_id == MEM_GROUP_TASK_CFC_MEM) {
				if (mem_len % MAX_LTIDS) {
					DP_NOTICE(p_hwfn,
						  "Invalid TCFC task memory size\n");
					return 0;
				}

				grc_param = DBG_GRC_PARAM_NUM_LTIDS;
				mem_len = qed_grc_get_param(p_hwfn, grc_param) *
					  (mem_len / MAX_LTIDS);
			}

			/* If memory is associated with Storm, update Storm
			 * details.
			 */
			if (s_block_defs
			    [cond_hdr->block_id]->associated_to_storm) {
				is_storm = true;
				storm_letter =
				    s_storm_defs[s_block_defs
						 [cond_hdr->block_id]->
						 storm_id].letter;
			}
=======
			block = get_dbg_block(p_hwfn,
					      cond_hdr->block_id);

			/* If memory is associated with Storm,
			 * update storm details
			 */
			if (block->associated_storm_letter)
				storm_letter = block->associated_storm_letter;
>>>>>>> upstream/android-13

			/* Dump memory */
			offset += qed_grc_dump_mem(p_hwfn,
						p_ptt,
						dump_buf + offset,
						dump,
						NULL,
						mem_addr,
						mem_len,
						mem_wide_bus,
						0,
						false,
						s_mem_group_names[mem_group_id],
<<<<<<< HEAD
						is_storm,
=======
>>>>>>> upstream/android-13
						storm_letter);
		}
	}

	return offset;
}

/* Dumps GRC memories according to the input array dump_mem.
 * Returns the dumped size in dwords.
 */
static u32 qed_grc_dump_memories(struct qed_hwfn *p_hwfn,
				 struct qed_ptt *p_ptt,
				 u32 *dump_buf, bool dump)
{
<<<<<<< HEAD
	u32 offset = 0, input_offset = 0;

	while (input_offset <
	       s_dbg_arrays[BIN_BUF_DBG_DUMP_MEM].size_in_dwords) {
		const struct dbg_dump_split_hdr *split_hdr;
		struct dbg_array curr_input_mems_arr;
		enum init_split_types split_type;
		u32 split_data_size;

		split_hdr = (const struct dbg_dump_split_hdr *)
			&s_dbg_arrays[BIN_BUF_DBG_DUMP_MEM].ptr[input_offset++];
		split_type =
			GET_FIELD(split_hdr->hdr,
				  DBG_DUMP_SPLIT_HDR_SPLIT_TYPE_ID);
		split_data_size =
			GET_FIELD(split_hdr->hdr,
				  DBG_DUMP_SPLIT_HDR_DATA_SIZE);
		curr_input_mems_arr.ptr =
			&s_dbg_arrays[BIN_BUF_DBG_DUMP_MEM].ptr[input_offset];
		curr_input_mems_arr.size_in_dwords = split_data_size;
=======
	struct virt_mem_desc *dbg_buf =
	    &p_hwfn->dbg_arrays[BIN_BUF_DBG_DUMP_MEM];
	u32 offset = 0, input_offset = 0;

	while (input_offset < BYTES_TO_DWORDS(dbg_buf->size)) {
		const struct dbg_dump_split_hdr *split_hdr;
		struct virt_mem_desc curr_input_mems_arr;
		enum init_split_types split_type;
		u32 split_data_size;

		split_hdr =
		    (const struct dbg_dump_split_hdr *)dbg_buf->ptr +
		    input_offset++;
		split_type = GET_FIELD(split_hdr->hdr,
				       DBG_DUMP_SPLIT_HDR_SPLIT_TYPE_ID);
		split_data_size = GET_FIELD(split_hdr->hdr,
					    DBG_DUMP_SPLIT_HDR_DATA_SIZE);
		curr_input_mems_arr.ptr = (u32 *)dbg_buf->ptr + input_offset;
		curr_input_mems_arr.size = DWORDS_TO_BYTES(split_data_size);
>>>>>>> upstream/android-13

		if (split_type == SPLIT_TYPE_NONE)
			offset += qed_grc_dump_mem_entries(p_hwfn,
							   p_ptt,
							   curr_input_mems_arr,
							   dump_buf + offset,
							   dump);
		else
			DP_NOTICE(p_hwfn,
				  "Dumping split memories is currently not supported\n");

		input_offset += split_data_size;
	}

	return offset;
}

/* Dumps GRC context data for the specified Storm.
 * Returns the dumped size in dwords.
 * The lid_size argument is specified in quad-regs.
 */
static u32 qed_grc_dump_ctx_data(struct qed_hwfn *p_hwfn,
				 struct qed_ptt *p_ptt,
				 u32 *dump_buf,
				 bool dump,
				 const char *name,
				 u32 num_lids,
<<<<<<< HEAD
				 u32 lid_size,
				 u32 rd_reg_addr,
				 u8 storm_id)
{
	struct storm_defs *storm = &s_storm_defs[storm_id];
	u32 i, lid, total_size, offset = 0;
=======
				 enum cm_ctx_types ctx_type, u8 storm_id)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	struct storm_defs *storm = &s_storm_defs[storm_id];
	u32 i, lid, lid_size, total_size;
	u32 rd_reg_addr, offset = 0;

	/* Convert quad-regs to dwords */
	lid_size = storm->cm_ctx_lid_sizes[dev_data->chip_id][ctx_type] * 4;
>>>>>>> upstream/android-13

	if (!lid_size)
		return 0;

<<<<<<< HEAD
	lid_size *= BYTES_IN_DWORD;
=======
>>>>>>> upstream/android-13
	total_size = num_lids * lid_size;

	offset += qed_grc_dump_mem_hdr(p_hwfn,
				       dump_buf + offset,
				       dump,
				       name,
				       0,
				       total_size,
				       lid_size * 32,
<<<<<<< HEAD
				       false, name, true, storm->letter);
=======
				       false, name, storm->letter);
>>>>>>> upstream/android-13

	if (!dump)
		return offset + total_size;

<<<<<<< HEAD
	/* Dump context data */
	for (lid = 0; lid < num_lids; lid++) {
		for (i = 0; i < lid_size; i++, offset++) {
			qed_wr(p_hwfn,
			       p_ptt, storm->cm_ctx_wr_addr, (i << 9) | lid);
			*(dump_buf + offset) = qed_rd(p_hwfn,
						      p_ptt, rd_reg_addr);
=======
	rd_reg_addr = BYTES_TO_DWORDS(storm->cm_ctx_rd_addr[ctx_type]);

	/* Dump context data */
	for (lid = 0; lid < num_lids; lid++) {
		for (i = 0; i < lid_size; i++) {
			qed_wr(p_hwfn,
			       p_ptt, storm->cm_ctx_wr_addr, (i << 9) | lid);
			offset += qed_grc_dump_addr_range(p_hwfn,
							  p_ptt,
							  dump_buf + offset,
							  dump,
							  rd_reg_addr,
							  1,
							  false,
							  SPLIT_TYPE_NONE, 0);
>>>>>>> upstream/android-13
		}
	}

	return offset;
}

/* Dumps GRC contexts. Returns the dumped size in dwords. */
static u32 qed_grc_dump_ctx(struct qed_hwfn *p_hwfn,
			    struct qed_ptt *p_ptt, u32 *dump_buf, bool dump)
{
<<<<<<< HEAD
	enum dbg_grc_params grc_param;
=======
>>>>>>> upstream/android-13
	u32 offset = 0;
	u8 storm_id;

	for (storm_id = 0; storm_id < MAX_DBG_STORMS; storm_id++) {
<<<<<<< HEAD
		struct storm_defs *storm = &s_storm_defs[storm_id];

=======
>>>>>>> upstream/android-13
		if (!qed_grc_is_storm_included(p_hwfn,
					       (enum dbg_storms)storm_id))
			continue;

		/* Dump Conn AG context size */
<<<<<<< HEAD
		grc_param = DBG_GRC_PARAM_NUM_LCIDS;
		offset +=
			qed_grc_dump_ctx_data(p_hwfn,
					      p_ptt,
					      dump_buf + offset,
					      dump,
					      "CONN_AG_CTX",
					      qed_grc_get_param(p_hwfn,
								grc_param),
					      storm->cm_conn_ag_ctx_lid_size,
					      storm->cm_conn_ag_ctx_rd_addr,
					      storm_id);

		/* Dump Conn ST context size */
		grc_param = DBG_GRC_PARAM_NUM_LCIDS;
		offset +=
			qed_grc_dump_ctx_data(p_hwfn,
					      p_ptt,
					      dump_buf + offset,
					      dump,
					      "CONN_ST_CTX",
					      qed_grc_get_param(p_hwfn,
								grc_param),
					      storm->cm_conn_st_ctx_lid_size,
					      storm->cm_conn_st_ctx_rd_addr,
					      storm_id);

		/* Dump Task AG context size */
		grc_param = DBG_GRC_PARAM_NUM_LTIDS;
		offset +=
			qed_grc_dump_ctx_data(p_hwfn,
					      p_ptt,
					      dump_buf + offset,
					      dump,
					      "TASK_AG_CTX",
					      qed_grc_get_param(p_hwfn,
								grc_param),
					      storm->cm_task_ag_ctx_lid_size,
					      storm->cm_task_ag_ctx_rd_addr,
					      storm_id);

		/* Dump Task ST context size */
		grc_param = DBG_GRC_PARAM_NUM_LTIDS;
		offset +=
			qed_grc_dump_ctx_data(p_hwfn,
					      p_ptt,
					      dump_buf + offset,
					      dump,
					      "TASK_ST_CTX",
					      qed_grc_get_param(p_hwfn,
								grc_param),
					      storm->cm_task_st_ctx_lid_size,
					      storm->cm_task_st_ctx_rd_addr,
					      storm_id);
=======
		offset += qed_grc_dump_ctx_data(p_hwfn,
						p_ptt,
						dump_buf + offset,
						dump,
						"CONN_AG_CTX",
						NUM_OF_LCIDS,
						CM_CTX_CONN_AG, storm_id);

		/* Dump Conn ST context size */
		offset += qed_grc_dump_ctx_data(p_hwfn,
						p_ptt,
						dump_buf + offset,
						dump,
						"CONN_ST_CTX",
						NUM_OF_LCIDS,
						CM_CTX_CONN_ST, storm_id);

		/* Dump Task AG context size */
		offset += qed_grc_dump_ctx_data(p_hwfn,
						p_ptt,
						dump_buf + offset,
						dump,
						"TASK_AG_CTX",
						NUM_OF_LTIDS,
						CM_CTX_TASK_AG, storm_id);

		/* Dump Task ST context size */
		offset += qed_grc_dump_ctx_data(p_hwfn,
						p_ptt,
						dump_buf + offset,
						dump,
						"TASK_ST_CTX",
						NUM_OF_LTIDS,
						CM_CTX_TASK_ST, storm_id);
>>>>>>> upstream/android-13
	}

	return offset;
}

<<<<<<< HEAD
/* Dumps GRC IORs data. Returns the dumped size in dwords. */
static u32 qed_grc_dump_iors(struct qed_hwfn *p_hwfn,
			     struct qed_ptt *p_ptt, u32 *dump_buf, bool dump)
{
	char buf[10] = "IOR_SET_?";
	u32 addr, offset = 0;
	u8 storm_id, set_id;

	for (storm_id = 0; storm_id < MAX_DBG_STORMS; storm_id++) {
		struct storm_defs *storm = &s_storm_defs[storm_id];

		if (!qed_grc_is_storm_included(p_hwfn,
					       (enum dbg_storms)storm_id))
			continue;

		for (set_id = 0; set_id < NUM_IOR_SETS; set_id++) {
			addr = BYTES_TO_DWORDS(storm->sem_fast_mem_addr +
					       SEM_FAST_REG_STORM_REG_FILE) +
			       IOR_SET_OFFSET(set_id);
			buf[strlen(buf) - 1] = '0' + set_id;
			offset += qed_grc_dump_mem(p_hwfn,
						   p_ptt,
						   dump_buf + offset,
						   dump,
						   buf,
						   addr,
						   IORS_PER_SET,
						   false,
						   32,
						   false,
						   "ior",
						   true,
						   storm->letter);
		}
	}

	return offset;
=======
#define VFC_STATUS_RESP_READY_BIT	0
#define VFC_STATUS_BUSY_BIT		1
#define VFC_STATUS_SENDING_CMD_BIT	2

#define VFC_POLLING_DELAY_MS	1
#define VFC_POLLING_COUNT		20

/* Reads data from VFC. Returns the number of dwords read (0 on error).
 * Sizes are specified in dwords.
 */
static u32 qed_grc_dump_read_from_vfc(struct qed_hwfn *p_hwfn,
				      struct qed_ptt *p_ptt,
				      struct storm_defs *storm,
				      u32 *cmd_data,
				      u32 cmd_size,
				      u32 *addr_data,
				      u32 addr_size,
				      u32 resp_size, u32 *dump_buf)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 vfc_status, polling_ms, polling_count = 0, i;
	u32 reg_addr, sem_base;
	bool is_ready = false;

	sem_base = storm->sem_fast_mem_addr;
	polling_ms = VFC_POLLING_DELAY_MS *
	    s_hw_type_defs[dev_data->hw_type].delay_factor;

	/* Write VFC command */
	ARR_REG_WR(p_hwfn,
		   p_ptt,
		   sem_base + SEM_FAST_REG_VFC_DATA_WR,
		   cmd_data, cmd_size);

	/* Write VFC address */
	ARR_REG_WR(p_hwfn,
		   p_ptt,
		   sem_base + SEM_FAST_REG_VFC_ADDR,
		   addr_data, addr_size);

	/* Read response */
	for (i = 0; i < resp_size; i++) {
		/* Poll until ready */
		do {
			reg_addr = sem_base + SEM_FAST_REG_VFC_STATUS;
			qed_grc_dump_addr_range(p_hwfn,
						p_ptt,
						&vfc_status,
						true,
						BYTES_TO_DWORDS(reg_addr),
						1,
						false, SPLIT_TYPE_NONE, 0);
			is_ready = vfc_status & BIT(VFC_STATUS_RESP_READY_BIT);

			if (!is_ready) {
				if (polling_count++ == VFC_POLLING_COUNT)
					return 0;

				msleep(polling_ms);
			}
		} while (!is_ready);

		reg_addr = sem_base + SEM_FAST_REG_VFC_DATA_RD;
		qed_grc_dump_addr_range(p_hwfn,
					p_ptt,
					dump_buf + i,
					true,
					BYTES_TO_DWORDS(reg_addr),
					1, false, SPLIT_TYPE_NONE, 0);
	}

	return resp_size;
>>>>>>> upstream/android-13
}

/* Dump VFC CAM. Returns the dumped size in dwords. */
static u32 qed_grc_dump_vfc_cam(struct qed_hwfn *p_hwfn,
				struct qed_ptt *p_ptt,
				u32 *dump_buf, bool dump, u8 storm_id)
{
	u32 total_size = VFC_CAM_NUM_ROWS * VFC_CAM_RESP_DWORDS;
	struct storm_defs *storm = &s_storm_defs[storm_id];
	u32 cam_addr[VFC_CAM_ADDR_DWORDS] = { 0 };
	u32 cam_cmd[VFC_CAM_CMD_DWORDS] = { 0 };
<<<<<<< HEAD
	u32 row, i, offset = 0;
=======
	u32 row, offset = 0;
>>>>>>> upstream/android-13

	offset += qed_grc_dump_mem_hdr(p_hwfn,
				       dump_buf + offset,
				       dump,
				       "vfc_cam",
				       0,
				       total_size,
				       256,
<<<<<<< HEAD
				       false, "vfc_cam", true, storm->letter);
=======
				       false, "vfc_cam", storm->letter);
>>>>>>> upstream/android-13

	if (!dump)
		return offset + total_size;

	/* Prepare CAM address */
	SET_VAR_FIELD(cam_addr, VFC_CAM_ADDR, OP, VFC_OPCODE_CAM_RD);

<<<<<<< HEAD
	for (row = 0; row < VFC_CAM_NUM_ROWS;
	     row++, offset += VFC_CAM_RESP_DWORDS) {
		/* Write VFC CAM command */
		SET_VAR_FIELD(cam_cmd, VFC_CAM_CMD, ROW, row);
		ARR_REG_WR(p_hwfn,
			   p_ptt,
			   storm->sem_fast_mem_addr + SEM_FAST_REG_VFC_DATA_WR,
			   cam_cmd, VFC_CAM_CMD_DWORDS);

		/* Write VFC CAM address */
		ARR_REG_WR(p_hwfn,
			   p_ptt,
			   storm->sem_fast_mem_addr + SEM_FAST_REG_VFC_ADDR,
			   cam_addr, VFC_CAM_ADDR_DWORDS);

		/* Read VFC CAM read response */
		ARR_REG_RD(p_hwfn,
			   p_ptt,
			   storm->sem_fast_mem_addr + SEM_FAST_REG_VFC_DATA_RD,
			   dump_buf + offset, VFC_CAM_RESP_DWORDS);
=======
	/* Read VFC CAM data */
	for (row = 0; row < VFC_CAM_NUM_ROWS; row++) {
		SET_VAR_FIELD(cam_cmd, VFC_CAM_CMD, ROW, row);
		offset += qed_grc_dump_read_from_vfc(p_hwfn,
						     p_ptt,
						     storm,
						     cam_cmd,
						     VFC_CAM_CMD_DWORDS,
						     cam_addr,
						     VFC_CAM_ADDR_DWORDS,
						     VFC_CAM_RESP_DWORDS,
						     dump_buf + offset);
>>>>>>> upstream/android-13
	}

	return offset;
}

/* Dump VFC RAM. Returns the dumped size in dwords. */
static u32 qed_grc_dump_vfc_ram(struct qed_hwfn *p_hwfn,
				struct qed_ptt *p_ptt,
				u32 *dump_buf,
				bool dump,
				u8 storm_id, struct vfc_ram_defs *ram_defs)
{
	u32 total_size = ram_defs->num_rows * VFC_RAM_RESP_DWORDS;
	struct storm_defs *storm = &s_storm_defs[storm_id];
	u32 ram_addr[VFC_RAM_ADDR_DWORDS] = { 0 };
	u32 ram_cmd[VFC_RAM_CMD_DWORDS] = { 0 };
<<<<<<< HEAD
	u32 row, i, offset = 0;
=======
	u32 row, offset = 0;
>>>>>>> upstream/android-13

	offset += qed_grc_dump_mem_hdr(p_hwfn,
				       dump_buf + offset,
				       dump,
				       ram_defs->mem_name,
				       0,
				       total_size,
				       256,
				       false,
				       ram_defs->type_name,
<<<<<<< HEAD
				       true, storm->letter);

	/* Prepare RAM address */
	SET_VAR_FIELD(ram_addr, VFC_RAM_ADDR, OP, VFC_OPCODE_RAM_RD);
=======
				       storm->letter);
>>>>>>> upstream/android-13

	if (!dump)
		return offset + total_size;

<<<<<<< HEAD
	for (row = ram_defs->base_row;
	     row < ram_defs->base_row + ram_defs->num_rows;
	     row++, offset += VFC_RAM_RESP_DWORDS) {
		/* Write VFC RAM command */
		ARR_REG_WR(p_hwfn,
			   p_ptt,
			   storm->sem_fast_mem_addr + SEM_FAST_REG_VFC_DATA_WR,
			   ram_cmd, VFC_RAM_CMD_DWORDS);

		/* Write VFC RAM address */
		SET_VAR_FIELD(ram_addr, VFC_RAM_ADDR, ROW, row);
		ARR_REG_WR(p_hwfn,
			   p_ptt,
			   storm->sem_fast_mem_addr + SEM_FAST_REG_VFC_ADDR,
			   ram_addr, VFC_RAM_ADDR_DWORDS);

		/* Read VFC RAM read response */
		ARR_REG_RD(p_hwfn,
			   p_ptt,
			   storm->sem_fast_mem_addr + SEM_FAST_REG_VFC_DATA_RD,
			   dump_buf + offset, VFC_RAM_RESP_DWORDS);
=======
	/* Prepare RAM address */
	SET_VAR_FIELD(ram_addr, VFC_RAM_ADDR, OP, VFC_OPCODE_RAM_RD);

	/* Read VFC RAM data */
	for (row = ram_defs->base_row;
	     row < ram_defs->base_row + ram_defs->num_rows; row++) {
		SET_VAR_FIELD(ram_addr, VFC_RAM_ADDR, ROW, row);
		offset += qed_grc_dump_read_from_vfc(p_hwfn,
						     p_ptt,
						     storm,
						     ram_cmd,
						     VFC_RAM_CMD_DWORDS,
						     ram_addr,
						     VFC_RAM_ADDR_DWORDS,
						     VFC_RAM_RESP_DWORDS,
						     dump_buf + offset);
>>>>>>> upstream/android-13
	}

	return offset;
}

/* Dumps GRC VFC data. Returns the dumped size in dwords. */
static u32 qed_grc_dump_vfc(struct qed_hwfn *p_hwfn,
			    struct qed_ptt *p_ptt, u32 *dump_buf, bool dump)
{
<<<<<<< HEAD
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
=======
>>>>>>> upstream/android-13
	u8 storm_id, i;
	u32 offset = 0;

	for (storm_id = 0; storm_id < MAX_DBG_STORMS; storm_id++) {
		if (!qed_grc_is_storm_included(p_hwfn,
					       (enum dbg_storms)storm_id) ||
<<<<<<< HEAD
		    !s_storm_defs[storm_id].has_vfc ||
		    (storm_id == DBG_PSTORM_ID && dev_data->platform_id !=
		     PLATFORM_ASIC))
=======
		    !s_storm_defs[storm_id].has_vfc)
>>>>>>> upstream/android-13
			continue;

		/* Read CAM */
		offset += qed_grc_dump_vfc_cam(p_hwfn,
					       p_ptt,
					       dump_buf + offset,
					       dump, storm_id);

		/* Read RAM */
		for (i = 0; i < NUM_VFC_RAM_TYPES; i++)
			offset += qed_grc_dump_vfc_ram(p_hwfn,
						       p_ptt,
						       dump_buf + offset,
						       dump,
						       storm_id,
						       &s_vfc_ram_defs[i]);
	}

	return offset;
}

/* Dumps GRC RSS data. Returns the dumped size in dwords. */
static u32 qed_grc_dump_rss(struct qed_hwfn *p_hwfn,
			    struct qed_ptt *p_ptt, u32 *dump_buf, bool dump)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 offset = 0;
	u8 rss_mem_id;

	for (rss_mem_id = 0; rss_mem_id < NUM_RSS_MEM_TYPES; rss_mem_id++) {
		u32 rss_addr, num_entries, total_dwords;
		struct rss_mem_defs *rss_defs;
		u32 addr, num_dwords_to_read;
		bool packed;

		rss_defs = &s_rss_mem_defs[rss_mem_id];
		rss_addr = rss_defs->addr;
		num_entries = rss_defs->num_entries[dev_data->chip_id];
		total_dwords = (num_entries * rss_defs->entry_width) / 32;
		packed = (rss_defs->entry_width == 16);

		offset += qed_grc_dump_mem_hdr(p_hwfn,
					       dump_buf + offset,
					       dump,
					       rss_defs->mem_name,
					       0,
					       total_dwords,
					       rss_defs->entry_width,
					       packed,
<<<<<<< HEAD
					       rss_defs->type_name, false, 0);
=======
					       rss_defs->type_name, 0);
>>>>>>> upstream/android-13

		/* Dump RSS data */
		if (!dump) {
			offset += total_dwords;
			continue;
		}

		addr = BYTES_TO_DWORDS(RSS_REG_RSS_RAM_DATA);
		while (total_dwords) {
			num_dwords_to_read = min_t(u32,
						   RSS_REG_RSS_RAM_DATA_SIZE,
						   total_dwords);
			qed_wr(p_hwfn, p_ptt, RSS_REG_RSS_RAM_ADDR, rss_addr);
			offset += qed_grc_dump_addr_range(p_hwfn,
							  p_ptt,
							  dump_buf + offset,
							  dump,
							  addr,
							  num_dwords_to_read,
							  false,
							  SPLIT_TYPE_NONE, 0);
			total_dwords -= num_dwords_to_read;
			rss_addr++;
		}
	}

	return offset;
}

/* Dumps GRC Big RAM. Returns the dumped size in dwords. */
static u32 qed_grc_dump_big_ram(struct qed_hwfn *p_hwfn,
				struct qed_ptt *p_ptt,
				u32 *dump_buf, bool dump, u8 big_ram_id)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 block_size, ram_size, offset = 0, reg_val, i;
	char mem_name[12] = "???_BIG_RAM";
	char type_name[8] = "???_RAM";
	struct big_ram_defs *big_ram;

	big_ram = &s_big_ram_defs[big_ram_id];
	ram_size = big_ram->ram_size[dev_data->chip_id];

	reg_val = qed_rd(p_hwfn, p_ptt, big_ram->is_256b_reg_addr);
	block_size = reg_val &
		     BIT(big_ram->is_256b_bit_offset[dev_data->chip_id]) ? 256
									 : 128;

	strncpy(type_name, big_ram->instance_name, BIG_RAM_NAME_LEN);
	strncpy(mem_name, big_ram->instance_name, BIG_RAM_NAME_LEN);

	/* Dump memory header */
	offset += qed_grc_dump_mem_hdr(p_hwfn,
				       dump_buf + offset,
				       dump,
				       mem_name,
				       0,
				       ram_size,
				       block_size * 8,
<<<<<<< HEAD
				       false, type_name, false, 0);
=======
				       false, type_name, 0);
>>>>>>> upstream/android-13

	/* Read and dump Big RAM data */
	if (!dump)
		return offset + ram_size;

	/* Dump Big RAM */
	for (i = 0; i < DIV_ROUND_UP(ram_size, BRB_REG_BIG_RAM_DATA_SIZE);
	     i++) {
		u32 addr, len;

		qed_wr(p_hwfn, p_ptt, big_ram->addr_reg_addr, i);
		addr = BYTES_TO_DWORDS(big_ram->data_reg_addr);
		len = BRB_REG_BIG_RAM_DATA_SIZE;
		offset += qed_grc_dump_addr_range(p_hwfn,
						  p_ptt,
						  dump_buf + offset,
						  dump,
						  addr,
						  len,
						  false, SPLIT_TYPE_NONE, 0);
	}

	return offset;
}

<<<<<<< HEAD
=======
/* Dumps MCP scratchpad. Returns the dumped size in dwords. */
>>>>>>> upstream/android-13
static u32 qed_grc_dump_mcp(struct qed_hwfn *p_hwfn,
			    struct qed_ptt *p_ptt, u32 *dump_buf, bool dump)
{
	bool block_enable[MAX_BLOCK_ID] = { 0 };
	u32 offset = 0, addr;
	bool halted = false;

	/* Halt MCP */
	if (dump && !qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_NO_MCP)) {
		halted = !qed_mcp_halt(p_hwfn, p_ptt);
		if (!halted)
			DP_NOTICE(p_hwfn, "MCP halt failed!\n");
	}

	/* Dump MCP scratchpad */
	offset += qed_grc_dump_mem(p_hwfn,
				   p_ptt,
				   dump_buf + offset,
				   dump,
				   NULL,
				   BYTES_TO_DWORDS(MCP_REG_SCRATCH),
<<<<<<< HEAD
				   MCP_REG_SCRATCH_SIZE_BB_K2,
				   false, 0, false, "MCP", false, 0);
=======
				   MCP_REG_SCRATCH_SIZE,
				   false, 0, false, "MCP", 0);
>>>>>>> upstream/android-13

	/* Dump MCP cpu_reg_file */
	offset += qed_grc_dump_mem(p_hwfn,
				   p_ptt,
				   dump_buf + offset,
				   dump,
				   NULL,
				   BYTES_TO_DWORDS(MCP_REG_CPU_REG_FILE),
				   MCP_REG_CPU_REG_FILE_SIZE,
<<<<<<< HEAD
				   false, 0, false, "MCP", false, 0);
=======
				   false, 0, false, "MCP", 0);
>>>>>>> upstream/android-13

	/* Dump MCP registers */
	block_enable[BLOCK_MCP] = true;
	offset += qed_grc_dump_registers(p_hwfn,
					 p_ptt,
					 dump_buf + offset,
<<<<<<< HEAD
					 dump, block_enable, "block", "MCP");
=======
					 dump, block_enable, "MCP");
>>>>>>> upstream/android-13

	/* Dump required non-MCP registers */
	offset += qed_grc_dump_regs_hdr(dump_buf + offset,
					dump, 1, SPLIT_TYPE_NONE, 0,
<<<<<<< HEAD
					"block", "MCP");
=======
					"MCP");
>>>>>>> upstream/android-13
	addr = BYTES_TO_DWORDS(MISC_REG_SHARED_MEM_ADDR);
	offset += qed_grc_dump_reg_entry(p_hwfn,
					 p_ptt,
					 dump_buf + offset,
					 dump,
					 addr,
					 1,
					 false, SPLIT_TYPE_NONE, 0);

	/* Release MCP */
	if (halted && qed_mcp_resume(p_hwfn, p_ptt))
		DP_NOTICE(p_hwfn, "Failed to resume MCP after halt!\n");

	return offset;
}

<<<<<<< HEAD
/* Dumps the tbus indirect memory for all PHYs. */
=======
/* Dumps the tbus indirect memory for all PHYs.
 * Returns the dumped size in dwords.
 */
>>>>>>> upstream/android-13
static u32 qed_grc_dump_phy(struct qed_hwfn *p_hwfn,
			    struct qed_ptt *p_ptt, u32 *dump_buf, bool dump)
{
	u32 offset = 0, tbus_lo_offset, tbus_hi_offset;
	char mem_name[32];
	u8 phy_id;

	for (phy_id = 0; phy_id < ARRAY_SIZE(s_phy_defs); phy_id++) {
		u32 addr_lo_addr, addr_hi_addr, data_lo_addr, data_hi_addr;
		struct phy_defs *phy_defs;
		u8 *bytes_buf;

		phy_defs = &s_phy_defs[phy_id];
		addr_lo_addr = phy_defs->base_addr +
			       phy_defs->tbus_addr_lo_addr;
		addr_hi_addr = phy_defs->base_addr +
			       phy_defs->tbus_addr_hi_addr;
		data_lo_addr = phy_defs->base_addr +
			       phy_defs->tbus_data_lo_addr;
		data_hi_addr = phy_defs->base_addr +
			       phy_defs->tbus_data_hi_addr;

		if (snprintf(mem_name, sizeof(mem_name), "tbus_%s",
			     phy_defs->phy_name) < 0)
			DP_NOTICE(p_hwfn,
				  "Unexpected debug error: invalid PHY memory name\n");

		offset += qed_grc_dump_mem_hdr(p_hwfn,
					       dump_buf + offset,
					       dump,
					       mem_name,
					       0,
					       PHY_DUMP_SIZE_DWORDS,
<<<<<<< HEAD
					       16, true, mem_name, false, 0);
=======
					       16, true, mem_name, 0);
>>>>>>> upstream/android-13

		if (!dump) {
			offset += PHY_DUMP_SIZE_DWORDS;
			continue;
		}

		bytes_buf = (u8 *)(dump_buf + offset);
		for (tbus_hi_offset = 0;
		     tbus_hi_offset < (NUM_PHY_TBUS_ADDRESSES >> 8);
		     tbus_hi_offset++) {
			qed_wr(p_hwfn, p_ptt, addr_hi_addr, tbus_hi_offset);
			for (tbus_lo_offset = 0; tbus_lo_offset < 256;
			     tbus_lo_offset++) {
				qed_wr(p_hwfn,
				       p_ptt, addr_lo_addr, tbus_lo_offset);
				*(bytes_buf++) = (u8)qed_rd(p_hwfn,
							    p_ptt,
							    data_lo_addr);
				*(bytes_buf++) = (u8)qed_rd(p_hwfn,
							    p_ptt,
							    data_hi_addr);
			}
		}

		offset += PHY_DUMP_SIZE_DWORDS;
	}

	return offset;
}

<<<<<<< HEAD
static void qed_config_dbg_line(struct qed_hwfn *p_hwfn,
				struct qed_ptt *p_ptt,
				enum block_id block_id,
				u8 line_id,
				u8 enable_mask,
				u8 right_shift,
				u8 force_valid_mask, u8 force_frame_mask)
{
	struct block_defs *block = s_block_defs[block_id];

	qed_wr(p_hwfn, p_ptt, block->dbg_select_addr, line_id);
	qed_wr(p_hwfn, p_ptt, block->dbg_enable_addr, enable_mask);
	qed_wr(p_hwfn, p_ptt, block->dbg_shift_addr, right_shift);
	qed_wr(p_hwfn, p_ptt, block->dbg_force_valid_addr, force_valid_mask);
	qed_wr(p_hwfn, p_ptt, block->dbg_force_frame_addr, force_frame_mask);
=======
static enum dbg_status qed_find_nvram_image(struct qed_hwfn *p_hwfn,
					    struct qed_ptt *p_ptt,
					    u32 image_type,
					    u32 *nvram_offset_bytes,
					    u32 *nvram_size_bytes);

static enum dbg_status qed_nvram_read(struct qed_hwfn *p_hwfn,
				      struct qed_ptt *p_ptt,
				      u32 nvram_offset_bytes,
				      u32 nvram_size_bytes, u32 *ret_buf);

/* Dumps the MCP HW dump from NVRAM. Returns the dumped size in dwords. */
static u32 qed_grc_dump_mcp_hw_dump(struct qed_hwfn *p_hwfn,
				    struct qed_ptt *p_ptt,
				    u32 *dump_buf, bool dump)
{
	u32 hw_dump_offset_bytes = 0, hw_dump_size_bytes = 0;
	u32 hw_dump_size_dwords = 0, offset = 0;
	enum dbg_status status;

	/* Read HW dump image from NVRAM */
	status = qed_find_nvram_image(p_hwfn,
				      p_ptt,
				      NVM_TYPE_HW_DUMP_OUT,
				      &hw_dump_offset_bytes,
				      &hw_dump_size_bytes);
	if (status != DBG_STATUS_OK)
		return 0;

	hw_dump_size_dwords = BYTES_TO_DWORDS(hw_dump_size_bytes);

	/* Dump HW dump image section */
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "mcp_hw_dump", 1);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump, "size", hw_dump_size_dwords);

	/* Read MCP HW dump image into dump buffer */
	if (dump && hw_dump_size_dwords) {
		status = qed_nvram_read(p_hwfn,
					p_ptt,
					hw_dump_offset_bytes,
					hw_dump_size_bytes, dump_buf + offset);
		if (status != DBG_STATUS_OK) {
			DP_NOTICE(p_hwfn,
				  "Failed to read MCP HW Dump image from NVRAM\n");
			return 0;
		}
	}
	offset += hw_dump_size_dwords;

	return offset;
>>>>>>> upstream/android-13
}

/* Dumps Static Debug data. Returns the dumped size in dwords. */
static u32 qed_grc_dump_static_debug(struct qed_hwfn *p_hwfn,
				     struct qed_ptt *p_ptt,
				     u32 *dump_buf, bool dump)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
<<<<<<< HEAD
	u32 block_id, line_id, offset = 0;
=======
	u32 block_id, line_id, offset = 0, addr, len;
>>>>>>> upstream/android-13

	/* Don't dump static debug if a debug bus recording is in progress */
	if (dump && qed_rd(p_hwfn, p_ptt, DBG_REG_DBG_BLOCK_ON))
		return 0;

	if (dump) {
<<<<<<< HEAD
		/* Disable all blocks debug output */
		for (block_id = 0; block_id < MAX_BLOCK_ID; block_id++) {
			struct block_defs *block = s_block_defs[block_id];

			if (block->dbg_client_id[dev_data->chip_id] !=
			    MAX_DBG_BUS_CLIENTS)
				qed_wr(p_hwfn, p_ptt, block->dbg_enable_addr,
				       0);
		}

		qed_bus_reset_dbg_block(p_hwfn, p_ptt);
		qed_bus_set_framing_mode(p_hwfn,
					 p_ptt, DBG_BUS_FRAME_MODE_8HW_0ST);
=======
		/* Disable debug bus in all blocks */
		qed_bus_disable_blocks(p_hwfn, p_ptt);

		qed_bus_reset_dbg_block(p_hwfn, p_ptt);
		qed_wr(p_hwfn,
		       p_ptt, DBG_REG_FRAMING_MODE, DBG_BUS_FRAME_MODE_8HW);
>>>>>>> upstream/android-13
		qed_wr(p_hwfn,
		       p_ptt, DBG_REG_DEBUG_TARGET, DBG_BUS_TARGET_ID_INT_BUF);
		qed_wr(p_hwfn, p_ptt, DBG_REG_FULL_MODE, 1);
		qed_bus_enable_dbg_block(p_hwfn, p_ptt, true);
	}

	/* Dump all static debug lines for each relevant block */
	for (block_id = 0; block_id < MAX_BLOCK_ID; block_id++) {
<<<<<<< HEAD
		struct block_defs *block = s_block_defs[block_id];
		struct dbg_bus_block *block_desc;
		u32 block_dwords, addr, len;
		u8 dbg_client_id;

		if (block->dbg_client_id[dev_data->chip_id] ==
		    MAX_DBG_BUS_CLIENTS)
			continue;

		block_desc = get_dbg_bus_block_desc(p_hwfn,
						    (enum block_id)block_id);
		block_dwords = NUM_DBG_LINES(block_desc) *
			       STATIC_DEBUG_LINE_DWORDS;

		/* Dump static section params */
=======
		const struct dbg_block_chip *block_per_chip;
		const struct dbg_block *block;
		bool is_removed, has_dbg_bus;
		u16 modes_buf_offset;
		u32 block_dwords;

		block_per_chip =
		    qed_get_dbg_block_per_chip(p_hwfn, (enum block_id)block_id);
		is_removed = GET_FIELD(block_per_chip->flags,
				       DBG_BLOCK_CHIP_IS_REMOVED);
		has_dbg_bus = GET_FIELD(block_per_chip->flags,
					DBG_BLOCK_CHIP_HAS_DBG_BUS);

		/* read+clear for NWS parity is not working, skip NWS block */
		if (block_id == BLOCK_NWS)
			continue;

		if (!is_removed && has_dbg_bus &&
		    GET_FIELD(block_per_chip->dbg_bus_mode.data,
			      DBG_MODE_HDR_EVAL_MODE) > 0) {
			modes_buf_offset =
			    GET_FIELD(block_per_chip->dbg_bus_mode.data,
				      DBG_MODE_HDR_MODES_BUF_OFFSET);
			if (!qed_is_mode_match(p_hwfn, &modes_buf_offset))
				has_dbg_bus = false;
		}

		if (is_removed || !has_dbg_bus)
			continue;

		block_dwords = NUM_DBG_LINES(block_per_chip) *
			       STATIC_DEBUG_LINE_DWORDS;

		/* Dump static section params */
		block = get_dbg_block(p_hwfn, (enum block_id)block_id);
>>>>>>> upstream/android-13
		offset += qed_grc_dump_mem_hdr(p_hwfn,
					       dump_buf + offset,
					       dump,
					       block->name,
					       0,
					       block_dwords,
<<<<<<< HEAD
					       32, false, "STATIC", false, 0);
=======
					       32, false, "STATIC", 0);
>>>>>>> upstream/android-13

		if (!dump) {
			offset += block_dwords;
			continue;
		}

		/* If all lines are invalid - dump zeros */
		if (dev_data->block_in_reset[block_id]) {
			memset(dump_buf + offset, 0,
			       DWORDS_TO_BYTES(block_dwords));
			offset += block_dwords;
			continue;
		}

		/* Enable block's client */
<<<<<<< HEAD
		dbg_client_id = block->dbg_client_id[dev_data->chip_id];
		qed_bus_enable_clients(p_hwfn,
				       p_ptt,
				       BIT(dbg_client_id));

		addr = BYTES_TO_DWORDS(DBG_REG_CALENDAR_OUT_DATA);
		len = STATIC_DEBUG_LINE_DWORDS;
		for (line_id = 0; line_id < (u32)NUM_DBG_LINES(block_desc);
		     line_id++) {
			/* Configure debug line ID */
			qed_config_dbg_line(p_hwfn,
					    p_ptt,
					    (enum block_id)block_id,
					    (u8)line_id, 0xf, 0, 0, 0);
=======
		qed_bus_enable_clients(p_hwfn,
				       p_ptt,
				       BIT(block_per_chip->dbg_client_id));

		addr = BYTES_TO_DWORDS(DBG_REG_CALENDAR_OUT_DATA);
		len = STATIC_DEBUG_LINE_DWORDS;
		for (line_id = 0; line_id < (u32)NUM_DBG_LINES(block_per_chip);
		     line_id++) {
			/* Configure debug line ID */
			qed_bus_config_dbg_line(p_hwfn,
						p_ptt,
						(enum block_id)block_id,
						(u8)line_id, 0xf, 0, 0, 0);
>>>>>>> upstream/android-13

			/* Read debug line info */
			offset += qed_grc_dump_addr_range(p_hwfn,
							  p_ptt,
							  dump_buf + offset,
							  dump,
							  addr,
							  len,
							  true, SPLIT_TYPE_NONE,
							  0);
		}

		/* Disable block's client and debug output */
		qed_bus_enable_clients(p_hwfn, p_ptt, 0);
<<<<<<< HEAD
		qed_wr(p_hwfn, p_ptt, block->dbg_enable_addr, 0);
=======
		qed_bus_config_dbg_line(p_hwfn, p_ptt,
					(enum block_id)block_id, 0, 0, 0, 0, 0);
>>>>>>> upstream/android-13
	}

	if (dump) {
		qed_bus_enable_dbg_block(p_hwfn, p_ptt, false);
		qed_bus_enable_clients(p_hwfn, p_ptt, 0);
	}

	return offset;
}

/* Performs GRC Dump to the specified buffer.
 * Returns the dumped size in dwords.
 */
static enum dbg_status qed_grc_dump(struct qed_hwfn *p_hwfn,
				    struct qed_ptt *p_ptt,
				    u32 *dump_buf,
				    bool dump, u32 *num_dumped_dwords)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
<<<<<<< HEAD
	bool parities_masked = false;
	u32 offset = 0;
=======
	u32 dwords_read, offset = 0;
	bool parities_masked = false;
>>>>>>> upstream/android-13
	u8 i;

	*num_dumped_dwords = 0;
	dev_data->num_regs_read = 0;

	/* Update reset state */
	if (dump)
		qed_update_blocks_reset_state(p_hwfn, p_ptt);

	/* Dump global params */
	offset += qed_dump_common_global_params(p_hwfn,
						p_ptt,
						dump_buf + offset, dump, 4);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "dump-type", "grc-dump");
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "num-lcids",
<<<<<<< HEAD
				     qed_grc_get_param(p_hwfn,
						DBG_GRC_PARAM_NUM_LCIDS));
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "num-ltids",
				     qed_grc_get_param(p_hwfn,
						DBG_GRC_PARAM_NUM_LTIDS));
=======
				     NUM_OF_LCIDS);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "num-ltids",
				     NUM_OF_LTIDS);
>>>>>>> upstream/android-13
	offset += qed_dump_num_param(dump_buf + offset,
				     dump, "num-ports", dev_data->num_ports);

	/* Dump reset registers (dumped before taking blocks out of reset ) */
	if (qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_REGS))
		offset += qed_grc_dump_reset_regs(p_hwfn,
						  p_ptt,
						  dump_buf + offset, dump);

	/* Take all blocks out of reset (using reset registers) */
	if (dump) {
<<<<<<< HEAD
		qed_grc_unreset_blocks(p_hwfn, p_ptt);
=======
		qed_grc_unreset_blocks(p_hwfn, p_ptt, false);
>>>>>>> upstream/android-13
		qed_update_blocks_reset_state(p_hwfn, p_ptt);
	}

	/* Disable all parities using MFW command */
	if (dump &&
	    !qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_NO_MCP)) {
		parities_masked = !qed_mcp_mask_parities(p_hwfn, p_ptt, 1);
		if (!parities_masked) {
			DP_NOTICE(p_hwfn,
				  "Failed to mask parities using MFW\n");
			if (qed_grc_get_param
			    (p_hwfn, DBG_GRC_PARAM_PARITY_SAFE))
				return DBG_STATUS_MCP_COULD_NOT_MASK_PRTY;
		}
	}

	/* Dump modified registers (dumped before modifying them) */
	if (qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_REGS))
		offset += qed_grc_dump_modified_regs(p_hwfn,
						     p_ptt,
						     dump_buf + offset, dump);

	/* Stall storms */
	if (dump &&
	    (qed_grc_is_included(p_hwfn,
				 DBG_GRC_PARAM_DUMP_IOR) ||
	     qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_VFC)))
		qed_grc_stall_storms(p_hwfn, p_ptt, true);

	/* Dump all regs  */
	if (qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_REGS)) {
		bool block_enable[MAX_BLOCK_ID];

		/* Dump all blocks except MCP */
		for (i = 0; i < MAX_BLOCK_ID; i++)
			block_enable[i] = true;
		block_enable[BLOCK_MCP] = false;
		offset += qed_grc_dump_registers(p_hwfn,
						 p_ptt,
						 dump_buf +
						 offset,
						 dump,
<<<<<<< HEAD
						 block_enable, NULL, NULL);
=======
						 block_enable, NULL);
>>>>>>> upstream/android-13

		/* Dump special registers */
		offset += qed_grc_dump_special_regs(p_hwfn,
						    p_ptt,
						    dump_buf + offset, dump);
	}

	/* Dump memories */
	offset += qed_grc_dump_memories(p_hwfn, p_ptt, dump_buf + offset, dump);

	/* Dump MCP */
	if (qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_MCP))
		offset += qed_grc_dump_mcp(p_hwfn,
					   p_ptt, dump_buf + offset, dump);

	/* Dump context */
	if (qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_CM_CTX))
		offset += qed_grc_dump_ctx(p_hwfn,
					   p_ptt, dump_buf + offset, dump);

	/* Dump RSS memories */
	if (qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_RSS))
		offset += qed_grc_dump_rss(p_hwfn,
					   p_ptt, dump_buf + offset, dump);

	/* Dump Big RAM */
	for (i = 0; i < NUM_BIG_RAM_TYPES; i++)
		if (qed_grc_is_included(p_hwfn, s_big_ram_defs[i].grc_param))
			offset += qed_grc_dump_big_ram(p_hwfn,
						       p_ptt,
						       dump_buf + offset,
						       dump, i);

<<<<<<< HEAD
	/* Dump IORs */
	if (qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_IOR))
		offset += qed_grc_dump_iors(p_hwfn,
					    p_ptt, dump_buf + offset, dump);

	/* Dump VFC */
	if (qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_VFC))
		offset += qed_grc_dump_vfc(p_hwfn,
					   p_ptt, dump_buf + offset, dump);
=======
	/* Dump VFC */
	if (qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_VFC)) {
		dwords_read = qed_grc_dump_vfc(p_hwfn,
					       p_ptt, dump_buf + offset, dump);
		offset += dwords_read;
		if (!dwords_read)
			return DBG_STATUS_VFC_READ_ERROR;
	}
>>>>>>> upstream/android-13

	/* Dump PHY tbus */
	if (qed_grc_is_included(p_hwfn,
				DBG_GRC_PARAM_DUMP_PHY) && dev_data->chip_id ==
<<<<<<< HEAD
	    CHIP_K2 && dev_data->platform_id == PLATFORM_ASIC)
		offset += qed_grc_dump_phy(p_hwfn,
					   p_ptt, dump_buf + offset, dump);

=======
	    CHIP_K2 && dev_data->hw_type == HW_TYPE_ASIC)
		offset += qed_grc_dump_phy(p_hwfn,
					   p_ptt, dump_buf + offset, dump);

	/* Dump MCP HW Dump */
	if (qed_grc_is_included(p_hwfn, DBG_GRC_PARAM_DUMP_MCP_HW_DUMP) &&
	    !qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_NO_MCP) && 1)
		offset += qed_grc_dump_mcp_hw_dump(p_hwfn,
						   p_ptt,
						   dump_buf + offset, dump);

>>>>>>> upstream/android-13
	/* Dump static debug data (only if not during debug bus recording) */
	if (qed_grc_is_included(p_hwfn,
				DBG_GRC_PARAM_DUMP_STATIC) &&
	    (!dump || dev_data->bus.state == DBG_BUS_STATE_IDLE))
		offset += qed_grc_dump_static_debug(p_hwfn,
						    p_ptt,
						    dump_buf + offset, dump);

	/* Dump last section */
	offset += qed_dump_last_section(dump_buf, offset, dump);

	if (dump) {
		/* Unstall storms */
		if (qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_UNSTALL))
			qed_grc_stall_storms(p_hwfn, p_ptt, false);

		/* Clear parity status */
		qed_grc_clear_all_prty(p_hwfn, p_ptt);

		/* Enable all parities using MFW command */
		if (parities_masked)
			qed_mcp_mask_parities(p_hwfn, p_ptt, 0);
	}

	*num_dumped_dwords = offset;

	return DBG_STATUS_OK;
}

/* Writes the specified failing Idle Check rule to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_idle_chk_dump_failure(struct qed_hwfn *p_hwfn,
				     struct qed_ptt *p_ptt,
				     u32 *
				     dump_buf,
				     bool dump,
				     u16 rule_id,
				     const struct dbg_idle_chk_rule *rule,
				     u16 fail_entry_id, u32 *cond_reg_values)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	const struct dbg_idle_chk_cond_reg *cond_regs;
	const struct dbg_idle_chk_info_reg *info_regs;
	u32 i, next_reg_offset = 0, offset = 0;
	struct dbg_idle_chk_result_hdr *hdr;
	const union dbg_idle_chk_reg *regs;
	u8 reg_id;

	hdr = (struct dbg_idle_chk_result_hdr *)dump_buf;
<<<<<<< HEAD
	regs = &((const union dbg_idle_chk_reg *)
		 s_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_REGS].ptr)[rule->reg_offset];
=======
	regs = (const union dbg_idle_chk_reg *)
		p_hwfn->dbg_arrays[BIN_BUF_DBG_IDLE_CHK_REGS].ptr +
		rule->reg_offset;
>>>>>>> upstream/android-13
	cond_regs = &regs[0].cond_reg;
	info_regs = &regs[rule->num_cond_regs].info_reg;

	/* Dump rule data */
	if (dump) {
		memset(hdr, 0, sizeof(*hdr));
		hdr->rule_id = rule_id;
		hdr->mem_entry_id = fail_entry_id;
		hdr->severity = rule->severity;
		hdr->num_dumped_cond_regs = rule->num_cond_regs;
	}

	offset += IDLE_CHK_RESULT_HDR_DWORDS;

	/* Dump condition register values */
	for (reg_id = 0; reg_id < rule->num_cond_regs; reg_id++) {
		const struct dbg_idle_chk_cond_reg *reg = &cond_regs[reg_id];
		struct dbg_idle_chk_result_reg_hdr *reg_hdr;

<<<<<<< HEAD
		reg_hdr = (struct dbg_idle_chk_result_reg_hdr *)
			  (dump_buf + offset);
=======
		reg_hdr =
		    (struct dbg_idle_chk_result_reg_hdr *)(dump_buf + offset);
>>>>>>> upstream/android-13

		/* Write register header */
		if (!dump) {
			offset += IDLE_CHK_RESULT_REG_HDR_DWORDS +
			    reg->entry_size;
			continue;
		}

		offset += IDLE_CHK_RESULT_REG_HDR_DWORDS;
		memset(reg_hdr, 0, sizeof(*reg_hdr));
		reg_hdr->start_entry = reg->start_entry;
		reg_hdr->size = reg->entry_size;
		SET_FIELD(reg_hdr->data,
			  DBG_IDLE_CHK_RESULT_REG_HDR_IS_MEM,
			  reg->num_entries > 1 || reg->start_entry > 0 ? 1 : 0);
		SET_FIELD(reg_hdr->data,
			  DBG_IDLE_CHK_RESULT_REG_HDR_REG_ID, reg_id);

		/* Write register values */
		for (i = 0; i < reg_hdr->size; i++, next_reg_offset++, offset++)
			dump_buf[offset] = cond_reg_values[next_reg_offset];
	}

	/* Dump info register values */
	for (reg_id = 0; reg_id < rule->num_info_regs; reg_id++) {
		const struct dbg_idle_chk_info_reg *reg = &info_regs[reg_id];
		u32 block_id;

		/* Check if register's block is in reset */
		if (!dump) {
			offset += IDLE_CHK_RESULT_REG_HDR_DWORDS + reg->size;
			continue;
		}

		block_id = GET_FIELD(reg->data, DBG_IDLE_CHK_INFO_REG_BLOCK_ID);
		if (block_id >= MAX_BLOCK_ID) {
			DP_NOTICE(p_hwfn, "Invalid block_id\n");
			return 0;
		}

		if (!dev_data->block_in_reset[block_id]) {
			struct dbg_idle_chk_result_reg_hdr *reg_hdr;
			bool wide_bus, eval_mode, mode_match = true;
			u16 modes_buf_offset;
			u32 addr;

			reg_hdr = (struct dbg_idle_chk_result_reg_hdr *)
				  (dump_buf + offset);

			/* Check mode */
			eval_mode = GET_FIELD(reg->mode.data,
					      DBG_MODE_HDR_EVAL_MODE) > 0;
			if (eval_mode) {
				modes_buf_offset =
				    GET_FIELD(reg->mode.data,
					      DBG_MODE_HDR_MODES_BUF_OFFSET);
				mode_match =
					qed_is_mode_match(p_hwfn,
							  &modes_buf_offset);
			}

			if (!mode_match)
				continue;

			addr = GET_FIELD(reg->data,
					 DBG_IDLE_CHK_INFO_REG_ADDRESS);
			wide_bus = GET_FIELD(reg->data,
					     DBG_IDLE_CHK_INFO_REG_WIDE_BUS);

			/* Write register header */
			offset += IDLE_CHK_RESULT_REG_HDR_DWORDS;
			hdr->num_dumped_info_regs++;
			memset(reg_hdr, 0, sizeof(*reg_hdr));
			reg_hdr->size = reg->size;
			SET_FIELD(reg_hdr->data,
				  DBG_IDLE_CHK_RESULT_REG_HDR_REG_ID,
				  rule->num_cond_regs + reg_id);

			/* Write register values */
			offset += qed_grc_dump_addr_range(p_hwfn,
							  p_ptt,
							  dump_buf + offset,
							  dump,
							  addr,
							  reg->size, wide_bus,
							  SPLIT_TYPE_NONE, 0);
		}
	}

	return offset;
}

/* Dumps idle check rule entries. Returns the dumped size in dwords. */
static u32
qed_idle_chk_dump_rule_entries(struct qed_hwfn *p_hwfn, struct qed_ptt *p_ptt,
			       u32 *dump_buf, bool dump,
			       const struct dbg_idle_chk_rule *input_rules,
			       u32 num_input_rules, u32 *num_failing_rules)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 cond_reg_values[IDLE_CHK_MAX_ENTRIES_SIZE];
	u32 i, offset = 0;
	u16 entry_id;
	u8 reg_id;

	*num_failing_rules = 0;

	for (i = 0; i < num_input_rules; i++) {
		const struct dbg_idle_chk_cond_reg *cond_regs;
		const struct dbg_idle_chk_rule *rule;
		const union dbg_idle_chk_reg *regs;
		u16 num_reg_entries = 1;
		bool check_rule = true;
		const u32 *imm_values;

		rule = &input_rules[i];
<<<<<<< HEAD
		regs = &((const union dbg_idle_chk_reg *)
			 s_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_REGS].ptr)
			[rule->reg_offset];
		cond_regs = &regs[0].cond_reg;
		imm_values = &s_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_IMMS].ptr
			     [rule->imm_offset];
=======
		regs = (const union dbg_idle_chk_reg *)
			p_hwfn->dbg_arrays[BIN_BUF_DBG_IDLE_CHK_REGS].ptr +
			rule->reg_offset;
		cond_regs = &regs[0].cond_reg;
		imm_values =
		    (u32 *)p_hwfn->dbg_arrays[BIN_BUF_DBG_IDLE_CHK_IMMS].ptr +
		    rule->imm_offset;
>>>>>>> upstream/android-13

		/* Check if all condition register blocks are out of reset, and
		 * find maximal number of entries (all condition registers that
		 * are memories must have the same size, which is > 1).
		 */
		for (reg_id = 0; reg_id < rule->num_cond_regs && check_rule;
		     reg_id++) {
			u32 block_id =
				GET_FIELD(cond_regs[reg_id].data,
					  DBG_IDLE_CHK_COND_REG_BLOCK_ID);

			if (block_id >= MAX_BLOCK_ID) {
				DP_NOTICE(p_hwfn, "Invalid block_id\n");
				return 0;
			}

			check_rule = !dev_data->block_in_reset[block_id];
			if (cond_regs[reg_id].num_entries > num_reg_entries)
				num_reg_entries = cond_regs[reg_id].num_entries;
		}

		if (!check_rule && dump)
			continue;

		if (!dump) {
			u32 entry_dump_size =
				qed_idle_chk_dump_failure(p_hwfn,
							  p_ptt,
							  dump_buf + offset,
							  false,
							  rule->rule_id,
							  rule,
							  0,
							  NULL);

			offset += num_reg_entries * entry_dump_size;
			(*num_failing_rules) += num_reg_entries;
			continue;
		}

		/* Go over all register entries (number of entries is the same
		 * for all condition registers).
		 */
		for (entry_id = 0; entry_id < num_reg_entries; entry_id++) {
			u32 next_reg_offset = 0;

			/* Read current entry of all condition registers */
			for (reg_id = 0; reg_id < rule->num_cond_regs;
			     reg_id++) {
				const struct dbg_idle_chk_cond_reg *reg =
					&cond_regs[reg_id];
				u32 padded_entry_size, addr;
				bool wide_bus;

				/* Find GRC address (if it's a memory, the
				 * address of the specific entry is calculated).
				 */
				addr = GET_FIELD(reg->data,
						 DBG_IDLE_CHK_COND_REG_ADDRESS);
				wide_bus =
				    GET_FIELD(reg->data,
					      DBG_IDLE_CHK_COND_REG_WIDE_BUS);
				if (reg->num_entries > 1 ||
				    reg->start_entry > 0) {
					padded_entry_size =
					   reg->entry_size > 1 ?
					   roundup_pow_of_two(reg->entry_size) :
					   1;
					addr += (reg->start_entry + entry_id) *
						padded_entry_size;
				}

				/* Read registers */
				if (next_reg_offset + reg->entry_size >=
				    IDLE_CHK_MAX_ENTRIES_SIZE) {
					DP_NOTICE(p_hwfn,
						  "idle check registers entry is too large\n");
					return 0;
				}

				next_reg_offset +=
				    qed_grc_dump_addr_range(p_hwfn, p_ptt,
							    cond_reg_values +
							    next_reg_offset,
							    dump, addr,
							    reg->entry_size,
							    wide_bus,
							    SPLIT_TYPE_NONE, 0);
			}

			/* Call rule condition function.
			 * If returns true, it's a failure.
			 */
			if ((*cond_arr[rule->cond_id]) (cond_reg_values,
							imm_values)) {
				offset += qed_idle_chk_dump_failure(p_hwfn,
							p_ptt,
							dump_buf + offset,
							dump,
							rule->rule_id,
							rule,
							entry_id,
							cond_reg_values);
				(*num_failing_rules)++;
			}
		}
	}

	return offset;
}

/* Performs Idle Check Dump to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_idle_chk_dump(struct qed_hwfn *p_hwfn,
			     struct qed_ptt *p_ptt, u32 *dump_buf, bool dump)
{
<<<<<<< HEAD
	u32 num_failing_rules_offset, offset = 0, input_offset = 0;
	u32 num_failing_rules = 0;

	/* Dump global params */
=======
	struct virt_mem_desc *dbg_buf =
	    &p_hwfn->dbg_arrays[BIN_BUF_DBG_IDLE_CHK_RULES];
	u32 num_failing_rules_offset, offset = 0,
	    input_offset = 0, num_failing_rules = 0;

	/* Dump global params  - 1 must match below amount of params */
>>>>>>> upstream/android-13
	offset += qed_dump_common_global_params(p_hwfn,
						p_ptt,
						dump_buf + offset, dump, 1);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "dump-type", "idle-chk");

	/* Dump idle check section header with a single parameter */
	offset += qed_dump_section_hdr(dump_buf + offset, dump, "idle_chk", 1);
	num_failing_rules_offset = offset;
	offset += qed_dump_num_param(dump_buf + offset, dump, "num_rules", 0);

<<<<<<< HEAD
	while (input_offset <
	       s_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_RULES].size_in_dwords) {
		const struct dbg_idle_chk_cond_hdr *cond_hdr =
			(const struct dbg_idle_chk_cond_hdr *)
			&s_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_RULES].ptr
			[input_offset++];
=======
	while (input_offset < BYTES_TO_DWORDS(dbg_buf->size)) {
		const struct dbg_idle_chk_cond_hdr *cond_hdr =
		    (const struct dbg_idle_chk_cond_hdr *)dbg_buf->ptr +
		    input_offset++;
>>>>>>> upstream/android-13
		bool eval_mode, mode_match = true;
		u32 curr_failing_rules;
		u16 modes_buf_offset;

		/* Check mode */
		eval_mode = GET_FIELD(cond_hdr->mode.data,
				      DBG_MODE_HDR_EVAL_MODE) > 0;
		if (eval_mode) {
			modes_buf_offset =
				GET_FIELD(cond_hdr->mode.data,
					  DBG_MODE_HDR_MODES_BUF_OFFSET);
			mode_match = qed_is_mode_match(p_hwfn,
						       &modes_buf_offset);
		}

		if (mode_match) {
<<<<<<< HEAD
			offset +=
			    qed_idle_chk_dump_rule_entries(p_hwfn,
				p_ptt,
				dump_buf + offset,
				dump,
				(const struct dbg_idle_chk_rule *)
				&s_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_RULES].
				ptr[input_offset],
				cond_hdr->data_size / IDLE_CHK_RULE_SIZE_DWORDS,
				&curr_failing_rules);
=======
			const struct dbg_idle_chk_rule *rule =
			    (const struct dbg_idle_chk_rule *)((u32 *)
							       dbg_buf->ptr
							       + input_offset);
			u32 num_input_rules =
				cond_hdr->data_size / IDLE_CHK_RULE_SIZE_DWORDS;
			offset +=
			    qed_idle_chk_dump_rule_entries(p_hwfn,
							   p_ptt,
							   dump_buf +
							   offset,
							   dump,
							   rule,
							   num_input_rules,
							   &curr_failing_rules);
>>>>>>> upstream/android-13
			num_failing_rules += curr_failing_rules;
		}

		input_offset += cond_hdr->data_size;
	}

	/* Overwrite num_rules parameter */
	if (dump)
		qed_dump_num_param(dump_buf + num_failing_rules_offset,
				   dump, "num_rules", num_failing_rules);

	/* Dump last section */
	offset += qed_dump_last_section(dump_buf, offset, dump);

	return offset;
}

/* Finds the meta data image in NVRAM */
static enum dbg_status qed_find_nvram_image(struct qed_hwfn *p_hwfn,
					    struct qed_ptt *p_ptt,
					    u32 image_type,
					    u32 *nvram_offset_bytes,
					    u32 *nvram_size_bytes)
{
	u32 ret_mcp_resp, ret_mcp_param, ret_txn_size;
	struct mcp_file_att file_att;
	int nvm_result;

	/* Call NVRAM get file command */
	nvm_result = qed_mcp_nvm_rd_cmd(p_hwfn,
					p_ptt,
					DRV_MSG_CODE_NVM_GET_FILE_ATT,
					image_type,
					&ret_mcp_resp,
					&ret_mcp_param,
					&ret_txn_size, (u32 *)&file_att);

	/* Check response */
	if (nvm_result ||
	    (ret_mcp_resp & FW_MSG_CODE_MASK) != FW_MSG_CODE_NVM_OK)
		return DBG_STATUS_NVRAM_GET_IMAGE_FAILED;

	/* Update return values */
	*nvram_offset_bytes = file_att.nvm_start_addr;
	*nvram_size_bytes = file_att.len;

	DP_VERBOSE(p_hwfn,
		   QED_MSG_DEBUG,
		   "find_nvram_image: found NVRAM image of type %d in NVRAM offset %d bytes with size %d bytes\n",
		   image_type, *nvram_offset_bytes, *nvram_size_bytes);

	/* Check alignment */
	if (*nvram_size_bytes & 0x3)
		return DBG_STATUS_NON_ALIGNED_NVRAM_IMAGE;

	return DBG_STATUS_OK;
}

/* Reads data from NVRAM */
static enum dbg_status qed_nvram_read(struct qed_hwfn *p_hwfn,
				      struct qed_ptt *p_ptt,
				      u32 nvram_offset_bytes,
				      u32 nvram_size_bytes, u32 *ret_buf)
{
	u32 ret_mcp_resp, ret_mcp_param, ret_read_size, bytes_to_copy;
	s32 bytes_left = nvram_size_bytes;
<<<<<<< HEAD
	u32 read_offset = 0;
=======
	u32 read_offset = 0, param = 0;
>>>>>>> upstream/android-13

	DP_VERBOSE(p_hwfn,
		   QED_MSG_DEBUG,
		   "nvram_read: reading image of size %d bytes from NVRAM\n",
		   nvram_size_bytes);

	do {
		bytes_to_copy =
		    (bytes_left >
		     MCP_DRV_NVM_BUF_LEN) ? MCP_DRV_NVM_BUF_LEN : bytes_left;

		/* Call NVRAM read command */
<<<<<<< HEAD
		if (qed_mcp_nvm_rd_cmd(p_hwfn, p_ptt,
				       DRV_MSG_CODE_NVM_READ_NVRAM,
				       (nvram_offset_bytes +
					read_offset) |
				       (bytes_to_copy <<
					DRV_MB_PARAM_NVM_LEN_OFFSET),
				       &ret_mcp_resp, &ret_mcp_param,
				       &ret_read_size,
=======
		SET_MFW_FIELD(param,
			      DRV_MB_PARAM_NVM_OFFSET,
			      nvram_offset_bytes + read_offset);
		SET_MFW_FIELD(param, DRV_MB_PARAM_NVM_LEN, bytes_to_copy);
		if (qed_mcp_nvm_rd_cmd(p_hwfn, p_ptt,
				       DRV_MSG_CODE_NVM_READ_NVRAM, param,
				       &ret_mcp_resp,
				       &ret_mcp_param, &ret_read_size,
>>>>>>> upstream/android-13
				       (u32 *)((u8 *)ret_buf + read_offset)))
			return DBG_STATUS_NVRAM_READ_FAILED;

		/* Check response */
		if ((ret_mcp_resp & FW_MSG_CODE_MASK) != FW_MSG_CODE_NVM_OK)
			return DBG_STATUS_NVRAM_READ_FAILED;

		/* Update read offset */
		read_offset += ret_read_size;
		bytes_left -= ret_read_size;
	} while (bytes_left > 0);

	return DBG_STATUS_OK;
}

/* Get info on the MCP Trace data in the scratchpad:
 * - trace_data_grc_addr (OUT): trace data GRC address in bytes
 * - trace_data_size (OUT): trace data size in bytes (without the header)
 */
static enum dbg_status qed_mcp_trace_get_data_info(struct qed_hwfn *p_hwfn,
						   struct qed_ptt *p_ptt,
						   u32 *trace_data_grc_addr,
						   u32 *trace_data_size)
{
	u32 spad_trace_offsize, signature;

	/* Read trace section offsize structure from MCP scratchpad */
	spad_trace_offsize = qed_rd(p_hwfn, p_ptt, MCP_SPAD_TRACE_OFFSIZE_ADDR);

	/* Extract trace section address from offsize (in scratchpad) */
	*trace_data_grc_addr =
		MCP_REG_SCRATCH + SECTION_OFFSET(spad_trace_offsize);

	/* Read signature from MCP trace section */
	signature = qed_rd(p_hwfn, p_ptt,
			   *trace_data_grc_addr +
			   offsetof(struct mcp_trace, signature));

	if (signature != MFW_TRACE_SIGNATURE)
		return DBG_STATUS_INVALID_TRACE_SIGNATURE;

	/* Read trace size from MCP trace section */
	*trace_data_size = qed_rd(p_hwfn,
				  p_ptt,
				  *trace_data_grc_addr +
				  offsetof(struct mcp_trace, size));

	return DBG_STATUS_OK;
}

/* Reads MCP trace meta data image from NVRAM
 * - running_bundle_id (OUT): running bundle ID (invalid when loaded from file)
 * - trace_meta_offset (OUT): trace meta offset in NVRAM in bytes (invalid when
 *			      loaded from file).
 * - trace_meta_size (OUT):   size in bytes of the trace meta data.
 */
static enum dbg_status qed_mcp_trace_get_meta_info(struct qed_hwfn *p_hwfn,
						   struct qed_ptt *p_ptt,
						   u32 trace_data_size_bytes,
						   u32 *running_bundle_id,
						   u32 *trace_meta_offset,
						   u32 *trace_meta_size)
{
	u32 spad_trace_offsize, nvram_image_type, running_mfw_addr;

	/* Read MCP trace section offsize structure from MCP scratchpad */
	spad_trace_offsize = qed_rd(p_hwfn, p_ptt, MCP_SPAD_TRACE_OFFSIZE_ADDR);

	/* Find running bundle ID */
	running_mfw_addr =
		MCP_REG_SCRATCH + SECTION_OFFSET(spad_trace_offsize) +
		QED_SECTION_SIZE(spad_trace_offsize) + trace_data_size_bytes;
	*running_bundle_id = qed_rd(p_hwfn, p_ptt, running_mfw_addr);
	if (*running_bundle_id > 1)
		return DBG_STATUS_INVALID_NVRAM_BUNDLE;

	/* Find image in NVRAM */
	nvram_image_type =
	    (*running_bundle_id ==
	     DIR_ID_1) ? NVM_TYPE_MFW_TRACE1 : NVM_TYPE_MFW_TRACE2;
	return qed_find_nvram_image(p_hwfn,
				    p_ptt,
				    nvram_image_type,
				    trace_meta_offset, trace_meta_size);
}

/* Reads the MCP Trace meta data from NVRAM into the specified buffer */
static enum dbg_status qed_mcp_trace_read_meta(struct qed_hwfn *p_hwfn,
					       struct qed_ptt *p_ptt,
					       u32 nvram_offset_in_bytes,
					       u32 size_in_bytes, u32 *buf)
{
	u8 modules_num, module_len, i, *byte_buf = (u8 *)buf;
	enum dbg_status status;
	u32 signature;

	/* Read meta data from NVRAM */
	status = qed_nvram_read(p_hwfn,
				p_ptt,
				nvram_offset_in_bytes, size_in_bytes, buf);
	if (status != DBG_STATUS_OK)
		return status;

	/* Extract and check first signature */
	signature = qed_read_unaligned_dword(byte_buf);
	byte_buf += sizeof(signature);
	if (signature != NVM_MAGIC_VALUE)
		return DBG_STATUS_INVALID_TRACE_SIGNATURE;

	/* Extract number of modules */
	modules_num = *(byte_buf++);

	/* Skip all modules */
	for (i = 0; i < modules_num; i++) {
		module_len = *(byte_buf++);
		byte_buf += module_len;
	}

	/* Extract and check second signature */
	signature = qed_read_unaligned_dword(byte_buf);
	byte_buf += sizeof(signature);
	if (signature != NVM_MAGIC_VALUE)
		return DBG_STATUS_INVALID_TRACE_SIGNATURE;

	return DBG_STATUS_OK;
}

/* Dump MCP Trace */
static enum dbg_status qed_mcp_trace_dump(struct qed_hwfn *p_hwfn,
					  struct qed_ptt *p_ptt,
					  u32 *dump_buf,
					  bool dump, u32 *num_dumped_dwords)
{
	u32 trace_data_grc_addr, trace_data_size_bytes, trace_data_size_dwords;
	u32 trace_meta_size_dwords = 0, running_bundle_id, offset = 0;
	u32 trace_meta_offset_bytes = 0, trace_meta_size_bytes = 0;
	enum dbg_status status;
<<<<<<< HEAD
	bool mcp_access;
	int halted = 0;

	*num_dumped_dwords = 0;

	mcp_access = !qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_NO_MCP);
=======
	int halted = 0;
	bool use_mfw;

	*num_dumped_dwords = 0;

	use_mfw = !qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_NO_MCP);
>>>>>>> upstream/android-13

	/* Get trace data info */
	status = qed_mcp_trace_get_data_info(p_hwfn,
					     p_ptt,
					     &trace_data_grc_addr,
					     &trace_data_size_bytes);
	if (status != DBG_STATUS_OK)
		return status;

	/* Dump global params */
	offset += qed_dump_common_global_params(p_hwfn,
						p_ptt,
						dump_buf + offset, dump, 1);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "dump-type", "mcp-trace");

	/* Halt MCP while reading from scratchpad so the read data will be
	 * consistent. if halt fails, MCP trace is taken anyway, with a small
	 * risk that it may be corrupt.
	 */
<<<<<<< HEAD
	if (dump && mcp_access) {
=======
	if (dump && use_mfw) {
>>>>>>> upstream/android-13
		halted = !qed_mcp_halt(p_hwfn, p_ptt);
		if (!halted)
			DP_NOTICE(p_hwfn, "MCP halt failed!\n");
	}

	/* Find trace data size */
	trace_data_size_dwords =
	    DIV_ROUND_UP(trace_data_size_bytes + sizeof(struct mcp_trace),
			 BYTES_IN_DWORD);

	/* Dump trace data section header and param */
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "mcp_trace_data", 1);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump, "size", trace_data_size_dwords);

	/* Read trace data from scratchpad into dump buffer */
	offset += qed_grc_dump_addr_range(p_hwfn,
					  p_ptt,
					  dump_buf + offset,
					  dump,
					  BYTES_TO_DWORDS(trace_data_grc_addr),
					  trace_data_size_dwords, false,
					  SPLIT_TYPE_NONE, 0);

	/* Resume MCP (only if halt succeeded) */
	if (halted && qed_mcp_resume(p_hwfn, p_ptt))
		DP_NOTICE(p_hwfn, "Failed to resume MCP after halt!\n");

	/* Dump trace meta section header */
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "mcp_trace_meta", 1);

	/* If MCP Trace meta size parameter was set, use it.
	 * Otherwise, read trace meta.
	 * trace_meta_size_bytes is dword-aligned.
	 */
	trace_meta_size_bytes =
		qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_MCP_TRACE_META_SIZE);
<<<<<<< HEAD
	if ((!trace_meta_size_bytes || dump) && mcp_access) {
=======
	if ((!trace_meta_size_bytes || dump) && use_mfw)
>>>>>>> upstream/android-13
		status = qed_mcp_trace_get_meta_info(p_hwfn,
						     p_ptt,
						     trace_data_size_bytes,
						     &running_bundle_id,
						     &trace_meta_offset_bytes,
						     &trace_meta_size_bytes);
<<<<<<< HEAD
		if (status == DBG_STATUS_OK)
			trace_meta_size_dwords =
				BYTES_TO_DWORDS(trace_meta_size_bytes);
	}
=======
	if (status == DBG_STATUS_OK)
		trace_meta_size_dwords = BYTES_TO_DWORDS(trace_meta_size_bytes);
>>>>>>> upstream/android-13

	/* Dump trace meta size param */
	offset += qed_dump_num_param(dump_buf + offset,
				     dump, "size", trace_meta_size_dwords);

	/* Read trace meta image into dump buffer */
	if (dump && trace_meta_size_dwords)
		status = qed_mcp_trace_read_meta(p_hwfn,
						 p_ptt,
						 trace_meta_offset_bytes,
						 trace_meta_size_bytes,
						 dump_buf + offset);
	if (status == DBG_STATUS_OK)
		offset += trace_meta_size_dwords;

	/* Dump last section */
	offset += qed_dump_last_section(dump_buf, offset, dump);

	*num_dumped_dwords = offset;

	/* If no mcp access, indicate that the dump doesn't contain the meta
	 * data from NVRAM.
	 */
<<<<<<< HEAD
	return mcp_access ? status : DBG_STATUS_NVRAM_GET_IMAGE_FAILED;
=======
	return use_mfw ? status : DBG_STATUS_NVRAM_GET_IMAGE_FAILED;
>>>>>>> upstream/android-13
}

/* Dump GRC FIFO */
static enum dbg_status qed_reg_fifo_dump(struct qed_hwfn *p_hwfn,
					 struct qed_ptt *p_ptt,
					 u32 *dump_buf,
					 bool dump, u32 *num_dumped_dwords)
{
	u32 dwords_read, size_param_offset, offset = 0, addr, len;
	bool fifo_has_data;

	*num_dumped_dwords = 0;

	/* Dump global params */
	offset += qed_dump_common_global_params(p_hwfn,
						p_ptt,
						dump_buf + offset, dump, 1);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "dump-type", "reg-fifo");

	/* Dump fifo data section header and param. The size param is 0 for
	 * now, and is overwritten after reading the FIFO.
	 */
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "reg_fifo_data", 1);
	size_param_offset = offset;
	offset += qed_dump_num_param(dump_buf + offset, dump, "size", 0);

	if (!dump) {
		/* FIFO max size is REG_FIFO_DEPTH_DWORDS. There is no way to
		 * test how much data is available, except for reading it.
		 */
		offset += REG_FIFO_DEPTH_DWORDS;
		goto out;
	}

	fifo_has_data = qed_rd(p_hwfn, p_ptt,
			       GRC_REG_TRACE_FIFO_VALID_DATA) > 0;

	/* Pull available data from fifo. Use DMAE since this is widebus memory
	 * and must be accessed atomically. Test for dwords_read not passing
	 * buffer size since more entries could be added to the buffer as we are
	 * emptying it.
	 */
	addr = BYTES_TO_DWORDS(GRC_REG_TRACE_FIFO);
	len = REG_FIFO_ELEMENT_DWORDS;
	for (dwords_read = 0;
	     fifo_has_data && dwords_read < REG_FIFO_DEPTH_DWORDS;
	     dwords_read += REG_FIFO_ELEMENT_DWORDS) {
		offset += qed_grc_dump_addr_range(p_hwfn,
						  p_ptt,
						  dump_buf + offset,
						  true,
						  addr,
						  len,
						  true, SPLIT_TYPE_NONE,
						  0);
		fifo_has_data = qed_rd(p_hwfn, p_ptt,
				       GRC_REG_TRACE_FIFO_VALID_DATA) > 0;
	}

	qed_dump_num_param(dump_buf + size_param_offset, dump, "size",
			   dwords_read);
out:
	/* Dump last section */
	offset += qed_dump_last_section(dump_buf, offset, dump);

	*num_dumped_dwords = offset;

	return DBG_STATUS_OK;
}

/* Dump IGU FIFO */
static enum dbg_status qed_igu_fifo_dump(struct qed_hwfn *p_hwfn,
					 struct qed_ptt *p_ptt,
					 u32 *dump_buf,
					 bool dump, u32 *num_dumped_dwords)
{
	u32 dwords_read, size_param_offset, offset = 0, addr, len;
	bool fifo_has_data;

	*num_dumped_dwords = 0;

	/* Dump global params */
	offset += qed_dump_common_global_params(p_hwfn,
						p_ptt,
						dump_buf + offset, dump, 1);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "dump-type", "igu-fifo");

	/* Dump fifo data section header and param. The size param is 0 for
	 * now, and is overwritten after reading the FIFO.
	 */
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "igu_fifo_data", 1);
	size_param_offset = offset;
	offset += qed_dump_num_param(dump_buf + offset, dump, "size", 0);

	if (!dump) {
		/* FIFO max size is IGU_FIFO_DEPTH_DWORDS. There is no way to
		 * test how much data is available, except for reading it.
		 */
		offset += IGU_FIFO_DEPTH_DWORDS;
		goto out;
	}

	fifo_has_data = qed_rd(p_hwfn, p_ptt,
			       IGU_REG_ERROR_HANDLING_DATA_VALID) > 0;

	/* Pull available data from fifo. Use DMAE since this is widebus memory
	 * and must be accessed atomically. Test for dwords_read not passing
	 * buffer size since more entries could be added to the buffer as we are
	 * emptying it.
	 */
	addr = BYTES_TO_DWORDS(IGU_REG_ERROR_HANDLING_MEMORY);
	len = IGU_FIFO_ELEMENT_DWORDS;
	for (dwords_read = 0;
	     fifo_has_data && dwords_read < IGU_FIFO_DEPTH_DWORDS;
	     dwords_read += IGU_FIFO_ELEMENT_DWORDS) {
		offset += qed_grc_dump_addr_range(p_hwfn,
						  p_ptt,
						  dump_buf + offset,
						  true,
						  addr,
						  len,
						  true, SPLIT_TYPE_NONE,
						  0);
		fifo_has_data = qed_rd(p_hwfn, p_ptt,
				       IGU_REG_ERROR_HANDLING_DATA_VALID) > 0;
	}

	qed_dump_num_param(dump_buf + size_param_offset, dump, "size",
			   dwords_read);
out:
	/* Dump last section */
	offset += qed_dump_last_section(dump_buf, offset, dump);

	*num_dumped_dwords = offset;

	return DBG_STATUS_OK;
}

/* Protection Override dump */
static enum dbg_status qed_protection_override_dump(struct qed_hwfn *p_hwfn,
						    struct qed_ptt *p_ptt,
						    u32 *dump_buf,
						    bool dump,
						    u32 *num_dumped_dwords)
{
	u32 size_param_offset, override_window_dwords, offset = 0, addr;

	*num_dumped_dwords = 0;

	/* Dump global params */
	offset += qed_dump_common_global_params(p_hwfn,
						p_ptt,
						dump_buf + offset, dump, 1);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "dump-type", "protection-override");

	/* Dump data section header and param. The size param is 0 for now,
	 * and is overwritten after reading the data.
	 */
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "protection_override_data", 1);
	size_param_offset = offset;
	offset += qed_dump_num_param(dump_buf + offset, dump, "size", 0);

	if (!dump) {
		offset += PROTECTION_OVERRIDE_DEPTH_DWORDS;
		goto out;
	}

	/* Add override window info to buffer */
	override_window_dwords =
		qed_rd(p_hwfn, p_ptt, GRC_REG_NUMBER_VALID_OVERRIDE_WINDOW) *
		PROTECTION_OVERRIDE_ELEMENT_DWORDS;
<<<<<<< HEAD
	addr = BYTES_TO_DWORDS(GRC_REG_PROTECTION_OVERRIDE_WINDOW);
	offset += qed_grc_dump_addr_range(p_hwfn,
					  p_ptt,
					  dump_buf + offset,
					  true,
					  addr,
					  override_window_dwords,
					  true, SPLIT_TYPE_NONE, 0);
	qed_dump_num_param(dump_buf + size_param_offset, dump, "size",
			   override_window_dwords);
=======
	if (override_window_dwords) {
		addr = BYTES_TO_DWORDS(GRC_REG_PROTECTION_OVERRIDE_WINDOW);
		offset += qed_grc_dump_addr_range(p_hwfn,
						  p_ptt,
						  dump_buf + offset,
						  true,
						  addr,
						  override_window_dwords,
						  true, SPLIT_TYPE_NONE, 0);
		qed_dump_num_param(dump_buf + size_param_offset, dump, "size",
				   override_window_dwords);
	}
>>>>>>> upstream/android-13
out:
	/* Dump last section */
	offset += qed_dump_last_section(dump_buf, offset, dump);

	*num_dumped_dwords = offset;

	return DBG_STATUS_OK;
}

/* Performs FW Asserts Dump to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_fw_asserts_dump(struct qed_hwfn *p_hwfn,
			       struct qed_ptt *p_ptt, u32 *dump_buf, bool dump)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	struct fw_asserts_ram_section *asserts;
	char storm_letter_str[2] = "?";
	struct fw_info fw_info;
	u32 offset = 0;
	u8 storm_id;

	/* Dump global params */
	offset += qed_dump_common_global_params(p_hwfn,
						p_ptt,
						dump_buf + offset, dump, 1);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "dump-type", "fw-asserts");

	/* Find Storm dump size */
	for (storm_id = 0; storm_id < MAX_DBG_STORMS; storm_id++) {
		u32 fw_asserts_section_addr, next_list_idx_addr, next_list_idx;
		struct storm_defs *storm = &s_storm_defs[storm_id];
		u32 last_list_idx, addr;

<<<<<<< HEAD
		if (dev_data->block_in_reset[storm->block_id])
=======
		if (dev_data->block_in_reset[storm->sem_block_id])
>>>>>>> upstream/android-13
			continue;

		/* Read FW info for the current Storm */
		qed_read_storm_fw_info(p_hwfn, p_ptt, storm_id, &fw_info);

		asserts = &fw_info.fw_asserts_section;

		/* Dump FW Asserts section header and params */
		storm_letter_str[0] = storm->letter;
		offset += qed_dump_section_hdr(dump_buf + offset,
					       dump, "fw_asserts", 2);
		offset += qed_dump_str_param(dump_buf + offset,
					     dump, "storm", storm_letter_str);
		offset += qed_dump_num_param(dump_buf + offset,
					     dump,
					     "size",
					     asserts->list_element_dword_size);

		/* Read and dump FW Asserts data */
		if (!dump) {
			offset += asserts->list_element_dword_size;
			continue;
		}

<<<<<<< HEAD
		fw_asserts_section_addr = storm->sem_fast_mem_addr +
			SEM_FAST_REG_INT_RAM +
			RAM_LINES_TO_BYTES(asserts->section_ram_line_offset);
=======
		addr = le16_to_cpu(asserts->section_ram_line_offset);
		fw_asserts_section_addr = storm->sem_fast_mem_addr +
					  SEM_FAST_REG_INT_RAM +
					  RAM_LINES_TO_BYTES(addr);

>>>>>>> upstream/android-13
		next_list_idx_addr = fw_asserts_section_addr +
			DWORDS_TO_BYTES(asserts->list_next_index_dword_offset);
		next_list_idx = qed_rd(p_hwfn, p_ptt, next_list_idx_addr);
		last_list_idx = (next_list_idx > 0 ?
				 next_list_idx :
				 asserts->list_num_elements) - 1;
		addr = BYTES_TO_DWORDS(fw_asserts_section_addr) +
		       asserts->list_dword_offset +
		       last_list_idx * asserts->list_element_dword_size;
		offset +=
		    qed_grc_dump_addr_range(p_hwfn, p_ptt,
					    dump_buf + offset,
					    dump, addr,
					    asserts->list_element_dword_size,
						  false, SPLIT_TYPE_NONE, 0);
	}

	/* Dump last section */
	offset += qed_dump_last_section(dump_buf, offset, dump);

	return offset;
}

<<<<<<< HEAD
/***************************** Public Functions *******************************/

enum dbg_status qed_dbg_set_bin_ptr(const u8 * const bin_ptr)
{
	struct bin_buffer_hdr *buf_array = (struct bin_buffer_hdr *)bin_ptr;
	u8 buf_id;

	/* convert binary data to debug arrays */
	for (buf_id = 0; buf_id < MAX_BIN_DBG_BUFFER_TYPE; buf_id++) {
		s_dbg_arrays[buf_id].ptr =
		    (u32 *)(bin_ptr + buf_array[buf_id].offset);
		s_dbg_arrays[buf_id].size_in_dwords =
		    BYTES_TO_DWORDS(buf_array[buf_id].length);
	}
=======
/* Dumps the specified ILT pages to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_ilt_dump_pages_range(u32 *dump_buf,
				    bool dump,
				    u32 start_page_id,
				    u32 num_pages,
				    struct phys_mem_desc *ilt_pages,
				    bool dump_page_ids)
{
	u32 page_id, end_page_id, offset = 0;

	if (num_pages == 0)
		return offset;

	end_page_id = start_page_id + num_pages - 1;

	for (page_id = start_page_id; page_id <= end_page_id; page_id++) {
		struct phys_mem_desc *mem_desc = &ilt_pages[page_id];

		/**
		 *
		 * if (page_id >= ->p_cxt_mngr->ilt_shadow_size)
		 *     break;
		 */

		if (!ilt_pages[page_id].virt_addr)
			continue;

		if (dump_page_ids) {
			/* Copy page ID to dump buffer */
			if (dump)
				*(dump_buf + offset) = page_id;
			offset++;
		} else {
			/* Copy page memory to dump buffer */
			if (dump)
				memcpy(dump_buf + offset,
				       mem_desc->virt_addr, mem_desc->size);
			offset += BYTES_TO_DWORDS(mem_desc->size);
		}
	}

	return offset;
}

/* Dumps a section containing the dumped ILT pages.
 * Returns the dumped size in dwords.
 */
static u32 qed_ilt_dump_pages_section(struct qed_hwfn *p_hwfn,
				      u32 *dump_buf,
				      bool dump,
				      u32 valid_conn_pf_pages,
				      u32 valid_conn_vf_pages,
				      struct phys_mem_desc *ilt_pages,
				      bool dump_page_ids)
{
	struct qed_ilt_client_cfg *clients = p_hwfn->p_cxt_mngr->clients;
	u32 pf_start_line, start_page_id, offset = 0;
	u32 cdut_pf_init_pages, cdut_vf_init_pages;
	u32 cdut_pf_work_pages, cdut_vf_work_pages;
	u32 base_data_offset, size_param_offset;
	u32 cdut_pf_pages, cdut_vf_pages;
	const char *section_name;
	u8 i;

	section_name = dump_page_ids ? "ilt_page_ids" : "ilt_page_mem";
	cdut_pf_init_pages = qed_get_cdut_num_pf_init_pages(p_hwfn);
	cdut_vf_init_pages = qed_get_cdut_num_vf_init_pages(p_hwfn);
	cdut_pf_work_pages = qed_get_cdut_num_pf_work_pages(p_hwfn);
	cdut_vf_work_pages = qed_get_cdut_num_vf_work_pages(p_hwfn);
	cdut_pf_pages = cdut_pf_init_pages + cdut_pf_work_pages;
	cdut_vf_pages = cdut_vf_init_pages + cdut_vf_work_pages;
	pf_start_line = p_hwfn->p_cxt_mngr->pf_start_line;

	offset +=
	    qed_dump_section_hdr(dump_buf + offset, dump, section_name, 1);

	/* Dump size parameter (0 for now, overwritten with real size later) */
	size_param_offset = offset;
	offset += qed_dump_num_param(dump_buf + offset, dump, "size", 0);
	base_data_offset = offset;

	/* CDUC pages are ordered as follows:
	 * - PF pages - valid section (included in PF connection type mapping)
	 * - PF pages - invalid section (not dumped)
	 * - For each VF in the PF:
	 *   - VF pages - valid section (included in VF connection type mapping)
	 *   - VF pages - invalid section (not dumped)
	 */
	if (qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_DUMP_ILT_CDUC)) {
		/* Dump connection PF pages */
		start_page_id = clients[ILT_CLI_CDUC].first.val - pf_start_line;
		offset += qed_ilt_dump_pages_range(dump_buf + offset,
						   dump,
						   start_page_id,
						   valid_conn_pf_pages,
						   ilt_pages, dump_page_ids);

		/* Dump connection VF pages */
		start_page_id += clients[ILT_CLI_CDUC].pf_total_lines;
		for (i = 0; i < p_hwfn->p_cxt_mngr->vf_count;
		     i++, start_page_id += clients[ILT_CLI_CDUC].vf_total_lines)
			offset += qed_ilt_dump_pages_range(dump_buf + offset,
							   dump,
							   start_page_id,
							   valid_conn_vf_pages,
							   ilt_pages,
							   dump_page_ids);
	}

	/* CDUT pages are ordered as follows:
	 * - PF init pages (not dumped)
	 * - PF work pages
	 * - For each VF in the PF:
	 *   - VF init pages (not dumped)
	 *   - VF work pages
	 */
	if (qed_grc_get_param(p_hwfn, DBG_GRC_PARAM_DUMP_ILT_CDUT)) {
		/* Dump task PF pages */
		start_page_id = clients[ILT_CLI_CDUT].first.val +
		    cdut_pf_init_pages - pf_start_line;
		offset += qed_ilt_dump_pages_range(dump_buf + offset,
						   dump,
						   start_page_id,
						   cdut_pf_work_pages,
						   ilt_pages, dump_page_ids);

		/* Dump task VF pages */
		start_page_id = clients[ILT_CLI_CDUT].first.val +
		    cdut_pf_pages + cdut_vf_init_pages - pf_start_line;
		for (i = 0; i < p_hwfn->p_cxt_mngr->vf_count;
		     i++, start_page_id += cdut_vf_pages)
			offset += qed_ilt_dump_pages_range(dump_buf + offset,
							   dump,
							   start_page_id,
							   cdut_vf_work_pages,
							   ilt_pages,
							   dump_page_ids);
	}

	/* Overwrite size param */
	if (dump)
		qed_dump_num_param(dump_buf + size_param_offset,
				   dump, "size", offset - base_data_offset);

	return offset;
}

/* Performs ILT Dump to the specified buffer.
 * Returns the dumped size in dwords.
 */
static u32 qed_ilt_dump(struct qed_hwfn *p_hwfn,
			struct qed_ptt *p_ptt, u32 *dump_buf, bool dump)
{
	struct qed_ilt_client_cfg *clients = p_hwfn->p_cxt_mngr->clients;
	u32 valid_conn_vf_cids, valid_conn_vf_pages, offset = 0;
	u32 valid_conn_pf_cids, valid_conn_pf_pages, num_pages;
	u32 num_cids_per_page, conn_ctx_size;
	u32 cduc_page_size, cdut_page_size;
	struct phys_mem_desc *ilt_pages;
	u8 conn_type;

	cduc_page_size = 1 <<
	    (clients[ILT_CLI_CDUC].p_size.val + PXP_ILT_PAGE_SIZE_NUM_BITS_MIN);
	cdut_page_size = 1 <<
	    (clients[ILT_CLI_CDUT].p_size.val + PXP_ILT_PAGE_SIZE_NUM_BITS_MIN);
	conn_ctx_size = p_hwfn->p_cxt_mngr->conn_ctx_size;
	num_cids_per_page = (int)(cduc_page_size / conn_ctx_size);
	ilt_pages = p_hwfn->p_cxt_mngr->ilt_shadow;

	/* Dump global params - 22 must match number of params below */
	offset += qed_dump_common_global_params(p_hwfn, p_ptt,
						dump_buf + offset, dump, 22);
	offset += qed_dump_str_param(dump_buf + offset,
				     dump, "dump-type", "ilt-dump");
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cduc-page-size", cduc_page_size);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cduc-first-page-id",
				     clients[ILT_CLI_CDUC].first.val);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cduc-last-page-id",
				     clients[ILT_CLI_CDUC].last.val);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cduc-num-pf-pages",
				     clients
				     [ILT_CLI_CDUC].pf_total_lines);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cduc-num-vf-pages",
				     clients
				     [ILT_CLI_CDUC].vf_total_lines);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "max-conn-ctx-size",
				     conn_ctx_size);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cdut-page-size", cdut_page_size);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cdut-first-page-id",
				     clients[ILT_CLI_CDUT].first.val);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cdut-last-page-id",
				     clients[ILT_CLI_CDUT].last.val);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cdut-num-pf-init-pages",
				     qed_get_cdut_num_pf_init_pages(p_hwfn));
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cdut-num-vf-init-pages",
				     qed_get_cdut_num_vf_init_pages(p_hwfn));
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cdut-num-pf-work-pages",
				     qed_get_cdut_num_pf_work_pages(p_hwfn));
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "cdut-num-vf-work-pages",
				     qed_get_cdut_num_vf_work_pages(p_hwfn));
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "max-task-ctx-size",
				     p_hwfn->p_cxt_mngr->task_ctx_size);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "task-type-id",
				     p_hwfn->p_cxt_mngr->task_type_id);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "first-vf-id-in-pf",
				     p_hwfn->p_cxt_mngr->first_vf_in_pf);
	offset += /* 18 */ qed_dump_num_param(dump_buf + offset,
					      dump,
					      "num-vfs-in-pf",
					      p_hwfn->p_cxt_mngr->vf_count);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "ptr-size-bytes", sizeof(void *));
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "pf-start-line",
				     p_hwfn->p_cxt_mngr->pf_start_line);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "page-mem-desc-size-dwords",
				     PAGE_MEM_DESC_SIZE_DWORDS);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "ilt-shadow-size",
				     p_hwfn->p_cxt_mngr->ilt_shadow_size);
	/* Additional/Less parameters require matching of number in call to
	 * dump_common_global_params()
	 */

	/* Dump section containing number of PF CIDs per connection type */
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "num_pf_cids_per_conn_type", 1);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump, "size", NUM_OF_CONNECTION_TYPES_E4);
	for (conn_type = 0, valid_conn_pf_cids = 0;
	     conn_type < NUM_OF_CONNECTION_TYPES_E4; conn_type++, offset++) {
		u32 num_pf_cids =
		    p_hwfn->p_cxt_mngr->conn_cfg[conn_type].cid_count;

		if (dump)
			*(dump_buf + offset) = num_pf_cids;
		valid_conn_pf_cids += num_pf_cids;
	}

	/* Dump section containing number of VF CIDs per connection type */
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "num_vf_cids_per_conn_type", 1);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump, "size", NUM_OF_CONNECTION_TYPES_E4);
	for (conn_type = 0, valid_conn_vf_cids = 0;
	     conn_type < NUM_OF_CONNECTION_TYPES_E4; conn_type++, offset++) {
		u32 num_vf_cids =
		    p_hwfn->p_cxt_mngr->conn_cfg[conn_type].cids_per_vf;

		if (dump)
			*(dump_buf + offset) = num_vf_cids;
		valid_conn_vf_cids += num_vf_cids;
	}

	/* Dump section containing physical memory descs for each ILT page */
	num_pages = p_hwfn->p_cxt_mngr->ilt_shadow_size;
	offset += qed_dump_section_hdr(dump_buf + offset,
				       dump, "ilt_page_desc", 1);
	offset += qed_dump_num_param(dump_buf + offset,
				     dump,
				     "size",
				     num_pages * PAGE_MEM_DESC_SIZE_DWORDS);

	/* Copy memory descriptors to dump buffer */
	if (dump) {
		u32 page_id;

		for (page_id = 0; page_id < num_pages;
		     page_id++, offset += PAGE_MEM_DESC_SIZE_DWORDS)
			memcpy(dump_buf + offset,
			       &ilt_pages[page_id],
			       DWORDS_TO_BYTES(PAGE_MEM_DESC_SIZE_DWORDS));
	} else {
		offset += num_pages * PAGE_MEM_DESC_SIZE_DWORDS;
	}

	valid_conn_pf_pages = DIV_ROUND_UP(valid_conn_pf_cids,
					   num_cids_per_page);
	valid_conn_vf_pages = DIV_ROUND_UP(valid_conn_vf_cids,
					   num_cids_per_page);

	/* Dump ILT pages IDs */
	offset += qed_ilt_dump_pages_section(p_hwfn,
					     dump_buf + offset,
					     dump,
					     valid_conn_pf_pages,
					     valid_conn_vf_pages,
					     ilt_pages, true);

	/* Dump ILT pages memory */
	offset += qed_ilt_dump_pages_section(p_hwfn,
					     dump_buf + offset,
					     dump,
					     valid_conn_pf_pages,
					     valid_conn_vf_pages,
					     ilt_pages, false);

	/* Dump last section */
	offset += qed_dump_last_section(dump_buf, offset, dump);

	return offset;
}

/***************************** Public Functions *******************************/

enum dbg_status qed_dbg_set_bin_ptr(struct qed_hwfn *p_hwfn,
				    const u8 * const bin_ptr)
{
	struct bin_buffer_hdr *buf_hdrs = (struct bin_buffer_hdr *)bin_ptr;
	u8 buf_id;

	/* Convert binary data to debug arrays */
	for (buf_id = 0; buf_id < MAX_BIN_DBG_BUFFER_TYPE; buf_id++)
		qed_set_dbg_bin_buf(p_hwfn,
				    buf_id,
				    (u32 *)(bin_ptr + buf_hdrs[buf_id].offset),
				    buf_hdrs[buf_id].length);
>>>>>>> upstream/android-13

	return DBG_STATUS_OK;
}

bool qed_read_fw_info(struct qed_hwfn *p_hwfn,
		      struct qed_ptt *p_ptt, struct fw_info *fw_info)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u8 storm_id;

	for (storm_id = 0; storm_id < MAX_DBG_STORMS; storm_id++) {
		struct storm_defs *storm = &s_storm_defs[storm_id];

		/* Skip Storm if it's in reset */
<<<<<<< HEAD
		if (dev_data->block_in_reset[storm->block_id])
=======
		if (dev_data->block_in_reset[storm->sem_block_id])
>>>>>>> upstream/android-13
			continue;

		/* Read FW info for the current Storm */
		qed_read_storm_fw_info(p_hwfn, p_ptt, storm_id, fw_info);

		return true;
	}

	return false;
}

<<<<<<< HEAD
=======
enum dbg_status qed_dbg_grc_config(struct qed_hwfn *p_hwfn,
				   enum dbg_grc_params grc_param, u32 val)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	enum dbg_status status;
	int i;

	DP_VERBOSE(p_hwfn,
		   QED_MSG_DEBUG,
		   "dbg_grc_config: paramId = %d, val = %d\n", grc_param, val);

	status = qed_dbg_dev_init(p_hwfn);
	if (status != DBG_STATUS_OK)
		return status;

	/* Initializes the GRC parameters (if not initialized). Needed in order
	 * to set the default parameter values for the first time.
	 */
	qed_dbg_grc_init_params(p_hwfn);

	if (grc_param >= MAX_DBG_GRC_PARAMS)
		return DBG_STATUS_INVALID_ARGS;
	if (val < s_grc_param_defs[grc_param].min ||
	    val > s_grc_param_defs[grc_param].max)
		return DBG_STATUS_INVALID_ARGS;

	if (s_grc_param_defs[grc_param].is_preset) {
		/* Preset param */

		/* Disabling a preset is not allowed. Call
		 * dbg_grc_set_params_default instead.
		 */
		if (!val)
			return DBG_STATUS_INVALID_ARGS;

		/* Update all params with the preset values */
		for (i = 0; i < MAX_DBG_GRC_PARAMS; i++) {
			struct grc_param_defs *defs = &s_grc_param_defs[i];
			u32 preset_val;
			/* Skip persistent params */
			if (defs->is_persistent)
				continue;

			/* Find preset value */
			if (grc_param == DBG_GRC_PARAM_EXCLUDE_ALL)
				preset_val =
				    defs->exclude_all_preset_val;
			else if (grc_param == DBG_GRC_PARAM_CRASH)
				preset_val =
				    defs->crash_preset_val[dev_data->chip_id];
			else
				return DBG_STATUS_INVALID_ARGS;

			qed_grc_set_param(p_hwfn, i, preset_val);
		}
	} else {
		/* Regular param - set its value */
		qed_grc_set_param(p_hwfn, grc_param, val);
	}

	return DBG_STATUS_OK;
}

>>>>>>> upstream/android-13
/* Assign default GRC param values */
void qed_dbg_grc_set_params_default(struct qed_hwfn *p_hwfn)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	u32 i;

	for (i = 0; i < MAX_DBG_GRC_PARAMS; i++)
		if (!s_grc_param_defs[i].is_persistent)
			dev_data->grc.param_val[i] =
			    s_grc_param_defs[i].default_val[dev_data->chip_id];
}

enum dbg_status qed_dbg_grc_get_dump_buf_size(struct qed_hwfn *p_hwfn,
					      struct qed_ptt *p_ptt,
					      u32 *buf_size)
{
<<<<<<< HEAD
	enum dbg_status status = qed_dbg_dev_init(p_hwfn, p_ptt);
=======
	enum dbg_status status = qed_dbg_dev_init(p_hwfn);
>>>>>>> upstream/android-13

	*buf_size = 0;

	if (status != DBG_STATUS_OK)
		return status;

<<<<<<< HEAD
	if (!s_dbg_arrays[BIN_BUF_DBG_MODE_TREE].ptr ||
	    !s_dbg_arrays[BIN_BUF_DBG_DUMP_REG].ptr ||
	    !s_dbg_arrays[BIN_BUF_DBG_DUMP_MEM].ptr ||
	    !s_dbg_arrays[BIN_BUF_DBG_ATTN_BLOCKS].ptr ||
	    !s_dbg_arrays[BIN_BUF_DBG_ATTN_REGS].ptr)
=======
	if (!p_hwfn->dbg_arrays[BIN_BUF_DBG_MODE_TREE].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_DUMP_REG].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_DUMP_MEM].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_ATTN_BLOCKS].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_ATTN_REGS].ptr)
>>>>>>> upstream/android-13
		return DBG_STATUS_DBG_ARRAY_NOT_SET;

	return qed_grc_dump(p_hwfn, p_ptt, NULL, false, buf_size);
}

enum dbg_status qed_dbg_grc_dump(struct qed_hwfn *p_hwfn,
				 struct qed_ptt *p_ptt,
				 u32 *dump_buf,
				 u32 buf_size_in_dwords,
				 u32 *num_dumped_dwords)
{
	u32 needed_buf_size_in_dwords;
	enum dbg_status status;

	*num_dumped_dwords = 0;

	status = qed_dbg_grc_get_dump_buf_size(p_hwfn,
					       p_ptt,
					       &needed_buf_size_in_dwords);
	if (status != DBG_STATUS_OK)
		return status;

	if (buf_size_in_dwords < needed_buf_size_in_dwords)
		return DBG_STATUS_DUMP_BUF_TOO_SMALL;

	/* GRC Dump */
	status = qed_grc_dump(p_hwfn, p_ptt, dump_buf, true, num_dumped_dwords);

	/* Revert GRC params to their default */
	qed_dbg_grc_set_params_default(p_hwfn);

	return status;
}

enum dbg_status qed_dbg_idle_chk_get_dump_buf_size(struct qed_hwfn *p_hwfn,
						   struct qed_ptt *p_ptt,
						   u32 *buf_size)
{
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
<<<<<<< HEAD
	struct idle_chk_data *idle_chk;
	enum dbg_status status;

	idle_chk = &dev_data->idle_chk;
	*buf_size = 0;

	status = qed_dbg_dev_init(p_hwfn, p_ptt);
	if (status != DBG_STATUS_OK)
		return status;

	if (!s_dbg_arrays[BIN_BUF_DBG_MODE_TREE].ptr ||
	    !s_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_REGS].ptr ||
	    !s_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_IMMS].ptr ||
	    !s_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_RULES].ptr)
=======
	struct idle_chk_data *idle_chk = &dev_data->idle_chk;
	enum dbg_status status;

	*buf_size = 0;

	status = qed_dbg_dev_init(p_hwfn);
	if (status != DBG_STATUS_OK)
		return status;

	if (!p_hwfn->dbg_arrays[BIN_BUF_DBG_MODE_TREE].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_IDLE_CHK_REGS].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_IDLE_CHK_IMMS].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_IDLE_CHK_RULES].ptr)
>>>>>>> upstream/android-13
		return DBG_STATUS_DBG_ARRAY_NOT_SET;

	if (!idle_chk->buf_size_set) {
		idle_chk->buf_size = qed_idle_chk_dump(p_hwfn,
						       p_ptt, NULL, false);
		idle_chk->buf_size_set = true;
	}

	*buf_size = idle_chk->buf_size;

	return DBG_STATUS_OK;
}

enum dbg_status qed_dbg_idle_chk_dump(struct qed_hwfn *p_hwfn,
				      struct qed_ptt *p_ptt,
				      u32 *dump_buf,
				      u32 buf_size_in_dwords,
				      u32 *num_dumped_dwords)
{
	u32 needed_buf_size_in_dwords;
	enum dbg_status status;

	*num_dumped_dwords = 0;

	status = qed_dbg_idle_chk_get_dump_buf_size(p_hwfn,
						    p_ptt,
						    &needed_buf_size_in_dwords);
	if (status != DBG_STATUS_OK)
		return status;

	if (buf_size_in_dwords < needed_buf_size_in_dwords)
		return DBG_STATUS_DUMP_BUF_TOO_SMALL;

	/* Update reset state */
<<<<<<< HEAD
=======
	qed_grc_unreset_blocks(p_hwfn, p_ptt, true);
>>>>>>> upstream/android-13
	qed_update_blocks_reset_state(p_hwfn, p_ptt);

	/* Idle Check Dump */
	*num_dumped_dwords = qed_idle_chk_dump(p_hwfn, p_ptt, dump_buf, true);

	/* Revert GRC params to their default */
	qed_dbg_grc_set_params_default(p_hwfn);

	return DBG_STATUS_OK;
}

enum dbg_status qed_dbg_mcp_trace_get_dump_buf_size(struct qed_hwfn *p_hwfn,
						    struct qed_ptt *p_ptt,
						    u32 *buf_size)
{
<<<<<<< HEAD
	enum dbg_status status = qed_dbg_dev_init(p_hwfn, p_ptt);
=======
	enum dbg_status status = qed_dbg_dev_init(p_hwfn);
>>>>>>> upstream/android-13

	*buf_size = 0;

	if (status != DBG_STATUS_OK)
		return status;

	return qed_mcp_trace_dump(p_hwfn, p_ptt, NULL, false, buf_size);
}

enum dbg_status qed_dbg_mcp_trace_dump(struct qed_hwfn *p_hwfn,
				       struct qed_ptt *p_ptt,
				       u32 *dump_buf,
				       u32 buf_size_in_dwords,
				       u32 *num_dumped_dwords)
{
	u32 needed_buf_size_in_dwords;
	enum dbg_status status;

	status =
		qed_dbg_mcp_trace_get_dump_buf_size(p_hwfn,
						    p_ptt,
						    &needed_buf_size_in_dwords);
	if (status != DBG_STATUS_OK && status !=
	    DBG_STATUS_NVRAM_GET_IMAGE_FAILED)
		return status;

	if (buf_size_in_dwords < needed_buf_size_in_dwords)
		return DBG_STATUS_DUMP_BUF_TOO_SMALL;

	/* Update reset state */
	qed_update_blocks_reset_state(p_hwfn, p_ptt);

	/* Perform dump */
	status = qed_mcp_trace_dump(p_hwfn,
				    p_ptt, dump_buf, true, num_dumped_dwords);

	/* Revert GRC params to their default */
	qed_dbg_grc_set_params_default(p_hwfn);

	return status;
}

enum dbg_status qed_dbg_reg_fifo_get_dump_buf_size(struct qed_hwfn *p_hwfn,
						   struct qed_ptt *p_ptt,
						   u32 *buf_size)
{
<<<<<<< HEAD
	enum dbg_status status = qed_dbg_dev_init(p_hwfn, p_ptt);
=======
	enum dbg_status status = qed_dbg_dev_init(p_hwfn);
>>>>>>> upstream/android-13

	*buf_size = 0;

	if (status != DBG_STATUS_OK)
		return status;

	return qed_reg_fifo_dump(p_hwfn, p_ptt, NULL, false, buf_size);
}

enum dbg_status qed_dbg_reg_fifo_dump(struct qed_hwfn *p_hwfn,
				      struct qed_ptt *p_ptt,
				      u32 *dump_buf,
				      u32 buf_size_in_dwords,
				      u32 *num_dumped_dwords)
{
	u32 needed_buf_size_in_dwords;
	enum dbg_status status;

	*num_dumped_dwords = 0;

	status = qed_dbg_reg_fifo_get_dump_buf_size(p_hwfn,
						    p_ptt,
						    &needed_buf_size_in_dwords);
	if (status != DBG_STATUS_OK)
		return status;

	if (buf_size_in_dwords < needed_buf_size_in_dwords)
		return DBG_STATUS_DUMP_BUF_TOO_SMALL;

	/* Update reset state */
	qed_update_blocks_reset_state(p_hwfn, p_ptt);

	status = qed_reg_fifo_dump(p_hwfn,
				   p_ptt, dump_buf, true, num_dumped_dwords);

	/* Revert GRC params to their default */
	qed_dbg_grc_set_params_default(p_hwfn);

	return status;
}

enum dbg_status qed_dbg_igu_fifo_get_dump_buf_size(struct qed_hwfn *p_hwfn,
						   struct qed_ptt *p_ptt,
						   u32 *buf_size)
{
<<<<<<< HEAD
	enum dbg_status status = qed_dbg_dev_init(p_hwfn, p_ptt);
=======
	enum dbg_status status = qed_dbg_dev_init(p_hwfn);
>>>>>>> upstream/android-13

	*buf_size = 0;

	if (status != DBG_STATUS_OK)
		return status;

	return qed_igu_fifo_dump(p_hwfn, p_ptt, NULL, false, buf_size);
}

enum dbg_status qed_dbg_igu_fifo_dump(struct qed_hwfn *p_hwfn,
				      struct qed_ptt *p_ptt,
				      u32 *dump_buf,
				      u32 buf_size_in_dwords,
				      u32 *num_dumped_dwords)
{
	u32 needed_buf_size_in_dwords;
	enum dbg_status status;

	*num_dumped_dwords = 0;

	status = qed_dbg_igu_fifo_get_dump_buf_size(p_hwfn,
						    p_ptt,
						    &needed_buf_size_in_dwords);
	if (status != DBG_STATUS_OK)
		return status;

	if (buf_size_in_dwords < needed_buf_size_in_dwords)
		return DBG_STATUS_DUMP_BUF_TOO_SMALL;

	/* Update reset state */
	qed_update_blocks_reset_state(p_hwfn, p_ptt);

	status = qed_igu_fifo_dump(p_hwfn,
				   p_ptt, dump_buf, true, num_dumped_dwords);
	/* Revert GRC params to their default */
	qed_dbg_grc_set_params_default(p_hwfn);

	return status;
}

enum dbg_status
qed_dbg_protection_override_get_dump_buf_size(struct qed_hwfn *p_hwfn,
					      struct qed_ptt *p_ptt,
					      u32 *buf_size)
{
<<<<<<< HEAD
	enum dbg_status status = qed_dbg_dev_init(p_hwfn, p_ptt);
=======
	enum dbg_status status = qed_dbg_dev_init(p_hwfn);
>>>>>>> upstream/android-13

	*buf_size = 0;

	if (status != DBG_STATUS_OK)
		return status;

	return qed_protection_override_dump(p_hwfn,
					    p_ptt, NULL, false, buf_size);
}

enum dbg_status qed_dbg_protection_override_dump(struct qed_hwfn *p_hwfn,
						 struct qed_ptt *p_ptt,
						 u32 *dump_buf,
						 u32 buf_size_in_dwords,
						 u32 *num_dumped_dwords)
{
	u32 needed_buf_size_in_dwords, *p_size = &needed_buf_size_in_dwords;
	enum dbg_status status;

	*num_dumped_dwords = 0;

	status =
		qed_dbg_protection_override_get_dump_buf_size(p_hwfn,
							      p_ptt,
							      p_size);
	if (status != DBG_STATUS_OK)
		return status;

	if (buf_size_in_dwords < needed_buf_size_in_dwords)
		return DBG_STATUS_DUMP_BUF_TOO_SMALL;

	/* Update reset state */
	qed_update_blocks_reset_state(p_hwfn, p_ptt);

	status = qed_protection_override_dump(p_hwfn,
					      p_ptt,
					      dump_buf,
					      true, num_dumped_dwords);

	/* Revert GRC params to their default */
	qed_dbg_grc_set_params_default(p_hwfn);

	return status;
}

enum dbg_status qed_dbg_fw_asserts_get_dump_buf_size(struct qed_hwfn *p_hwfn,
						     struct qed_ptt *p_ptt,
						     u32 *buf_size)
{
<<<<<<< HEAD
	enum dbg_status status = qed_dbg_dev_init(p_hwfn, p_ptt);
=======
	enum dbg_status status = qed_dbg_dev_init(p_hwfn);
>>>>>>> upstream/android-13

	*buf_size = 0;

	if (status != DBG_STATUS_OK)
		return status;

	/* Update reset state */
	qed_update_blocks_reset_state(p_hwfn, p_ptt);

	*buf_size = qed_fw_asserts_dump(p_hwfn, p_ptt, NULL, false);

	return DBG_STATUS_OK;
}

enum dbg_status qed_dbg_fw_asserts_dump(struct qed_hwfn *p_hwfn,
					struct qed_ptt *p_ptt,
					u32 *dump_buf,
					u32 buf_size_in_dwords,
					u32 *num_dumped_dwords)
{
	u32 needed_buf_size_in_dwords, *p_size = &needed_buf_size_in_dwords;
	enum dbg_status status;

	*num_dumped_dwords = 0;

	status =
		qed_dbg_fw_asserts_get_dump_buf_size(p_hwfn,
						     p_ptt,
						     p_size);
	if (status != DBG_STATUS_OK)
		return status;

	if (buf_size_in_dwords < needed_buf_size_in_dwords)
		return DBG_STATUS_DUMP_BUF_TOO_SMALL;

	*num_dumped_dwords = qed_fw_asserts_dump(p_hwfn, p_ptt, dump_buf, true);

	/* Revert GRC params to their default */
	qed_dbg_grc_set_params_default(p_hwfn);

	return DBG_STATUS_OK;
}

<<<<<<< HEAD
=======
static enum dbg_status qed_dbg_ilt_get_dump_buf_size(struct qed_hwfn *p_hwfn,
						     struct qed_ptt *p_ptt,
						     u32 *buf_size)
{
	enum dbg_status status = qed_dbg_dev_init(p_hwfn);

	*buf_size = 0;

	if (status != DBG_STATUS_OK)
		return status;

	*buf_size = qed_ilt_dump(p_hwfn, p_ptt, NULL, false);

	return DBG_STATUS_OK;
}

static enum dbg_status qed_dbg_ilt_dump(struct qed_hwfn *p_hwfn,
					struct qed_ptt *p_ptt,
					u32 *dump_buf,
					u32 buf_size_in_dwords,
					u32 *num_dumped_dwords)
{
	u32 needed_buf_size_in_dwords;
	enum dbg_status status;

	*num_dumped_dwords = 0;

	status = qed_dbg_ilt_get_dump_buf_size(p_hwfn,
					       p_ptt,
					       &needed_buf_size_in_dwords);
	if (status != DBG_STATUS_OK)
		return status;

	if (buf_size_in_dwords < needed_buf_size_in_dwords)
		return DBG_STATUS_DUMP_BUF_TOO_SMALL;

	*num_dumped_dwords = qed_ilt_dump(p_hwfn, p_ptt, dump_buf, true);

	/* Reveret GRC params to their default */
	qed_dbg_grc_set_params_default(p_hwfn);

	return DBG_STATUS_OK;
}

>>>>>>> upstream/android-13
enum dbg_status qed_dbg_read_attn(struct qed_hwfn *p_hwfn,
				  struct qed_ptt *p_ptt,
				  enum block_id block_id,
				  enum dbg_attn_type attn_type,
				  bool clear_status,
				  struct dbg_attn_block_result *results)
{
<<<<<<< HEAD
	enum dbg_status status = qed_dbg_dev_init(p_hwfn, p_ptt);
=======
	enum dbg_status status = qed_dbg_dev_init(p_hwfn);
>>>>>>> upstream/android-13
	u8 reg_idx, num_attn_regs, num_result_regs = 0;
	const struct dbg_attn_reg *attn_reg_arr;

	if (status != DBG_STATUS_OK)
		return status;

<<<<<<< HEAD
	if (!s_dbg_arrays[BIN_BUF_DBG_MODE_TREE].ptr ||
	    !s_dbg_arrays[BIN_BUF_DBG_ATTN_BLOCKS].ptr ||
	    !s_dbg_arrays[BIN_BUF_DBG_ATTN_REGS].ptr)
		return DBG_STATUS_DBG_ARRAY_NOT_SET;

	attn_reg_arr = qed_get_block_attn_regs(block_id,
=======
	if (!p_hwfn->dbg_arrays[BIN_BUF_DBG_MODE_TREE].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_ATTN_BLOCKS].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_ATTN_REGS].ptr)
		return DBG_STATUS_DBG_ARRAY_NOT_SET;

	attn_reg_arr = qed_get_block_attn_regs(p_hwfn,
					       block_id,
>>>>>>> upstream/android-13
					       attn_type, &num_attn_regs);

	for (reg_idx = 0; reg_idx < num_attn_regs; reg_idx++) {
		const struct dbg_attn_reg *reg_data = &attn_reg_arr[reg_idx];
		struct dbg_attn_reg_result *reg_result;
		u32 sts_addr, sts_val;
		u16 modes_buf_offset;
		bool eval_mode;

		/* Check mode */
		eval_mode = GET_FIELD(reg_data->mode.data,
				      DBG_MODE_HDR_EVAL_MODE) > 0;
		modes_buf_offset = GET_FIELD(reg_data->mode.data,
					     DBG_MODE_HDR_MODES_BUF_OFFSET);
		if (eval_mode && !qed_is_mode_match(p_hwfn, &modes_buf_offset))
			continue;

		/* Mode match - read attention status register */
		sts_addr = DWORDS_TO_BYTES(clear_status ?
					   reg_data->sts_clr_address :
					   GET_FIELD(reg_data->data,
						     DBG_ATTN_REG_STS_ADDRESS));
		sts_val = qed_rd(p_hwfn, p_ptt, sts_addr);
		if (!sts_val)
			continue;

		/* Non-zero attention status - add to results */
		reg_result = &results->reg_results[num_result_regs];
		SET_FIELD(reg_result->data,
			  DBG_ATTN_REG_RESULT_STS_ADDRESS, sts_addr);
		SET_FIELD(reg_result->data,
			  DBG_ATTN_REG_RESULT_NUM_REG_ATTN,
			  GET_FIELD(reg_data->data, DBG_ATTN_REG_NUM_REG_ATTN));
		reg_result->block_attn_offset = reg_data->block_attn_offset;
		reg_result->sts_val = sts_val;
		reg_result->mask_val = qed_rd(p_hwfn,
					      p_ptt,
					      DWORDS_TO_BYTES
					      (reg_data->mask_address));
		num_result_regs++;
	}

	results->block_id = (u8)block_id;
	results->names_offset =
<<<<<<< HEAD
	    qed_get_block_attn_data(block_id, attn_type)->names_offset;
=======
	    qed_get_block_attn_data(p_hwfn, block_id, attn_type)->names_offset;
>>>>>>> upstream/android-13
	SET_FIELD(results->data, DBG_ATTN_BLOCK_RESULT_ATTN_TYPE, attn_type);
	SET_FIELD(results->data,
		  DBG_ATTN_BLOCK_RESULT_NUM_REGS, num_result_regs);

	return DBG_STATUS_OK;
}

/******************************* Data Types **********************************/

<<<<<<< HEAD
struct block_info {
	const char *name;
	enum block_id id;
};

struct mcp_trace_format {
	u32 data;
#define MCP_TRACE_FORMAT_MODULE_MASK	0x0000ffff
#define MCP_TRACE_FORMAT_MODULE_SHIFT	0
#define MCP_TRACE_FORMAT_LEVEL_MASK	0x00030000
#define MCP_TRACE_FORMAT_LEVEL_SHIFT	16
#define MCP_TRACE_FORMAT_P1_SIZE_MASK	0x000c0000
#define MCP_TRACE_FORMAT_P1_SIZE_SHIFT	18
#define MCP_TRACE_FORMAT_P2_SIZE_MASK	0x00300000
#define MCP_TRACE_FORMAT_P2_SIZE_SHIFT	20
#define MCP_TRACE_FORMAT_P3_SIZE_MASK	0x00c00000
#define MCP_TRACE_FORMAT_P3_SIZE_SHIFT	22
#define MCP_TRACE_FORMAT_LEN_MASK	0xff000000
#define MCP_TRACE_FORMAT_LEN_SHIFT	24

	char *format_str;
};

/* Meta data structure, generated by a perl script during MFW build. therefore,
 * the structs mcp_trace_meta and mcp_trace_format are duplicated in the perl
 * script.
 */
struct mcp_trace_meta {
	u32 modules_num;
	char **modules;
	u32 formats_num;
	struct mcp_trace_format *formats;
};

=======
>>>>>>> upstream/android-13
/* REG fifo element */
struct reg_fifo_element {
	u64 data;
#define REG_FIFO_ELEMENT_ADDRESS_SHIFT		0
#define REG_FIFO_ELEMENT_ADDRESS_MASK		0x7fffff
#define REG_FIFO_ELEMENT_ACCESS_SHIFT		23
#define REG_FIFO_ELEMENT_ACCESS_MASK		0x1
#define REG_FIFO_ELEMENT_PF_SHIFT		24
#define REG_FIFO_ELEMENT_PF_MASK		0xf
#define REG_FIFO_ELEMENT_VF_SHIFT		28
#define REG_FIFO_ELEMENT_VF_MASK		0xff
#define REG_FIFO_ELEMENT_PORT_SHIFT		36
#define REG_FIFO_ELEMENT_PORT_MASK		0x3
#define REG_FIFO_ELEMENT_PRIVILEGE_SHIFT	38
#define REG_FIFO_ELEMENT_PRIVILEGE_MASK		0x3
#define REG_FIFO_ELEMENT_PROTECTION_SHIFT	40
#define REG_FIFO_ELEMENT_PROTECTION_MASK	0x7
#define REG_FIFO_ELEMENT_MASTER_SHIFT		43
#define REG_FIFO_ELEMENT_MASTER_MASK		0xf
#define REG_FIFO_ELEMENT_ERROR_SHIFT		47
#define REG_FIFO_ELEMENT_ERROR_MASK		0x1f
};

<<<<<<< HEAD
=======
/* REG fifo error element */
struct reg_fifo_err {
	u32 err_code;
	const char *err_msg;
};

>>>>>>> upstream/android-13
/* IGU fifo element */
struct igu_fifo_element {
	u32 dword0;
#define IGU_FIFO_ELEMENT_DWORD0_FID_SHIFT		0
#define IGU_FIFO_ELEMENT_DWORD0_FID_MASK		0xff
#define IGU_FIFO_ELEMENT_DWORD0_IS_PF_SHIFT		8
#define IGU_FIFO_ELEMENT_DWORD0_IS_PF_MASK		0x1
#define IGU_FIFO_ELEMENT_DWORD0_SOURCE_SHIFT		9
#define IGU_FIFO_ELEMENT_DWORD0_SOURCE_MASK		0xf
#define IGU_FIFO_ELEMENT_DWORD0_ERR_TYPE_SHIFT		13
#define IGU_FIFO_ELEMENT_DWORD0_ERR_TYPE_MASK		0xf
#define IGU_FIFO_ELEMENT_DWORD0_CMD_ADDR_SHIFT		17
#define IGU_FIFO_ELEMENT_DWORD0_CMD_ADDR_MASK		0x7fff
	u32 dword1;
	u32 dword2;
#define IGU_FIFO_ELEMENT_DWORD12_IS_WR_CMD_SHIFT	0
#define IGU_FIFO_ELEMENT_DWORD12_IS_WR_CMD_MASK		0x1
#define IGU_FIFO_ELEMENT_DWORD12_WR_DATA_SHIFT		1
#define IGU_FIFO_ELEMENT_DWORD12_WR_DATA_MASK		0xffffffff
	u32 reserved;
};

struct igu_fifo_wr_data {
	u32 data;
#define IGU_FIFO_WR_DATA_PROD_CONS_SHIFT		0
#define IGU_FIFO_WR_DATA_PROD_CONS_MASK			0xffffff
#define IGU_FIFO_WR_DATA_UPDATE_FLAG_SHIFT		24
#define IGU_FIFO_WR_DATA_UPDATE_FLAG_MASK		0x1
#define IGU_FIFO_WR_DATA_EN_DIS_INT_FOR_SB_SHIFT	25
#define IGU_FIFO_WR_DATA_EN_DIS_INT_FOR_SB_MASK		0x3
#define IGU_FIFO_WR_DATA_SEGMENT_SHIFT			27
#define IGU_FIFO_WR_DATA_SEGMENT_MASK			0x1
#define IGU_FIFO_WR_DATA_TIMER_MASK_SHIFT		28
#define IGU_FIFO_WR_DATA_TIMER_MASK_MASK		0x1
#define IGU_FIFO_WR_DATA_CMD_TYPE_SHIFT			31
#define IGU_FIFO_WR_DATA_CMD_TYPE_MASK			0x1
};

struct igu_fifo_cleanup_wr_data {
	u32 data;
#define IGU_FIFO_CLEANUP_WR_DATA_RESERVED_SHIFT		0
#define IGU_FIFO_CLEANUP_WR_DATA_RESERVED_MASK		0x7ffffff
#define IGU_FIFO_CLEANUP_WR_DATA_CLEANUP_VAL_SHIFT	27
#define IGU_FIFO_CLEANUP_WR_DATA_CLEANUP_VAL_MASK	0x1
#define IGU_FIFO_CLEANUP_WR_DATA_CLEANUP_TYPE_SHIFT	28
#define IGU_FIFO_CLEANUP_WR_DATA_CLEANUP_TYPE_MASK	0x7
#define IGU_FIFO_CLEANUP_WR_DATA_CMD_TYPE_SHIFT		31
#define IGU_FIFO_CLEANUP_WR_DATA_CMD_TYPE_MASK		0x1
};

/* Protection override element */
struct protection_override_element {
	u64 data;
#define PROTECTION_OVERRIDE_ELEMENT_ADDRESS_SHIFT		0
#define PROTECTION_OVERRIDE_ELEMENT_ADDRESS_MASK		0x7fffff
#define PROTECTION_OVERRIDE_ELEMENT_WINDOW_SIZE_SHIFT		23
#define PROTECTION_OVERRIDE_ELEMENT_WINDOW_SIZE_MASK		0xffffff
#define PROTECTION_OVERRIDE_ELEMENT_READ_SHIFT			47
#define PROTECTION_OVERRIDE_ELEMENT_READ_MASK			0x1
#define PROTECTION_OVERRIDE_ELEMENT_WRITE_SHIFT			48
#define PROTECTION_OVERRIDE_ELEMENT_WRITE_MASK			0x1
#define PROTECTION_OVERRIDE_ELEMENT_READ_PROTECTION_SHIFT	49
#define PROTECTION_OVERRIDE_ELEMENT_READ_PROTECTION_MASK	0x7
#define PROTECTION_OVERRIDE_ELEMENT_WRITE_PROTECTION_SHIFT	52
#define PROTECTION_OVERRIDE_ELEMENT_WRITE_PROTECTION_MASK	0x7
};

enum igu_fifo_sources {
	IGU_SRC_PXP0,
	IGU_SRC_PXP1,
	IGU_SRC_PXP2,
	IGU_SRC_PXP3,
	IGU_SRC_PXP4,
	IGU_SRC_PXP5,
	IGU_SRC_PXP6,
	IGU_SRC_PXP7,
	IGU_SRC_CAU,
	IGU_SRC_ATTN,
	IGU_SRC_GRC
};

enum igu_fifo_addr_types {
	IGU_ADDR_TYPE_MSIX_MEM,
	IGU_ADDR_TYPE_WRITE_PBA,
	IGU_ADDR_TYPE_WRITE_INT_ACK,
	IGU_ADDR_TYPE_WRITE_ATTN_BITS,
	IGU_ADDR_TYPE_READ_INT,
	IGU_ADDR_TYPE_WRITE_PROD_UPDATE,
	IGU_ADDR_TYPE_RESERVED
};

struct igu_fifo_addr_data {
	u16 start_addr;
	u16 end_addr;
	char *desc;
	char *vf_desc;
	enum igu_fifo_addr_types type;
};

/******************************** Constants **********************************/

#define MAX_MSG_LEN				1024

#define MCP_TRACE_MAX_MODULE_LEN		8
#define MCP_TRACE_FORMAT_MAX_PARAMS		3
#define MCP_TRACE_FORMAT_PARAM_WIDTH \
<<<<<<< HEAD
	(MCP_TRACE_FORMAT_P2_SIZE_SHIFT - MCP_TRACE_FORMAT_P1_SIZE_SHIFT)
=======
	(MCP_TRACE_FORMAT_P2_SIZE_OFFSET - MCP_TRACE_FORMAT_P1_SIZE_OFFSET)
>>>>>>> upstream/android-13

#define REG_FIFO_ELEMENT_ADDR_FACTOR		4
#define REG_FIFO_ELEMENT_IS_PF_VF_VAL		127

#define PROTECTION_OVERRIDE_ELEMENT_ADDR_FACTOR	4

/***************************** Constant Arrays *******************************/

<<<<<<< HEAD
struct user_dbg_array {
	const u32 *ptr;
	u32 size_in_dwords;
};

/* Debug arrays */
static struct user_dbg_array
s_user_dbg_arrays[MAX_BIN_DBG_BUFFER_TYPE] = { {NULL} };

/* Block names array */
static struct block_info s_block_info_arr[] = {
	{"grc", BLOCK_GRC},
	{"miscs", BLOCK_MISCS},
	{"misc", BLOCK_MISC},
	{"dbu", BLOCK_DBU},
	{"pglue_b", BLOCK_PGLUE_B},
	{"cnig", BLOCK_CNIG},
	{"cpmu", BLOCK_CPMU},
	{"ncsi", BLOCK_NCSI},
	{"opte", BLOCK_OPTE},
	{"bmb", BLOCK_BMB},
	{"pcie", BLOCK_PCIE},
	{"mcp", BLOCK_MCP},
	{"mcp2", BLOCK_MCP2},
	{"pswhst", BLOCK_PSWHST},
	{"pswhst2", BLOCK_PSWHST2},
	{"pswrd", BLOCK_PSWRD},
	{"pswrd2", BLOCK_PSWRD2},
	{"pswwr", BLOCK_PSWWR},
	{"pswwr2", BLOCK_PSWWR2},
	{"pswrq", BLOCK_PSWRQ},
	{"pswrq2", BLOCK_PSWRQ2},
	{"pglcs", BLOCK_PGLCS},
	{"ptu", BLOCK_PTU},
	{"dmae", BLOCK_DMAE},
	{"tcm", BLOCK_TCM},
	{"mcm", BLOCK_MCM},
	{"ucm", BLOCK_UCM},
	{"xcm", BLOCK_XCM},
	{"ycm", BLOCK_YCM},
	{"pcm", BLOCK_PCM},
	{"qm", BLOCK_QM},
	{"tm", BLOCK_TM},
	{"dorq", BLOCK_DORQ},
	{"brb", BLOCK_BRB},
	{"src", BLOCK_SRC},
	{"prs", BLOCK_PRS},
	{"tsdm", BLOCK_TSDM},
	{"msdm", BLOCK_MSDM},
	{"usdm", BLOCK_USDM},
	{"xsdm", BLOCK_XSDM},
	{"ysdm", BLOCK_YSDM},
	{"psdm", BLOCK_PSDM},
	{"tsem", BLOCK_TSEM},
	{"msem", BLOCK_MSEM},
	{"usem", BLOCK_USEM},
	{"xsem", BLOCK_XSEM},
	{"ysem", BLOCK_YSEM},
	{"psem", BLOCK_PSEM},
	{"rss", BLOCK_RSS},
	{"tmld", BLOCK_TMLD},
	{"muld", BLOCK_MULD},
	{"yuld", BLOCK_YULD},
	{"xyld", BLOCK_XYLD},
	{"ptld", BLOCK_PTLD},
	{"ypld", BLOCK_YPLD},
	{"prm", BLOCK_PRM},
	{"pbf_pb1", BLOCK_PBF_PB1},
	{"pbf_pb2", BLOCK_PBF_PB2},
	{"rpb", BLOCK_RPB},
	{"btb", BLOCK_BTB},
	{"pbf", BLOCK_PBF},
	{"rdif", BLOCK_RDIF},
	{"tdif", BLOCK_TDIF},
	{"cdu", BLOCK_CDU},
	{"ccfc", BLOCK_CCFC},
	{"tcfc", BLOCK_TCFC},
	{"igu", BLOCK_IGU},
	{"cau", BLOCK_CAU},
	{"rgfs", BLOCK_RGFS},
	{"rgsrc", BLOCK_RGSRC},
	{"tgfs", BLOCK_TGFS},
	{"tgsrc", BLOCK_TGSRC},
	{"umac", BLOCK_UMAC},
	{"xmac", BLOCK_XMAC},
	{"dbg", BLOCK_DBG},
	{"nig", BLOCK_NIG},
	{"wol", BLOCK_WOL},
	{"bmbn", BLOCK_BMBN},
	{"ipc", BLOCK_IPC},
	{"nwm", BLOCK_NWM},
	{"nws", BLOCK_NWS},
	{"ms", BLOCK_MS},
	{"phy_pcie", BLOCK_PHY_PCIE},
	{"led", BLOCK_LED},
	{"avs_wrap", BLOCK_AVS_WRAP},
	{"pxpreqbus", BLOCK_PXPREQBUS},
	{"misc_aeu", BLOCK_MISC_AEU},
	{"bar0_map", BLOCK_BAR0_MAP}
};

=======
>>>>>>> upstream/android-13
/* Status string array */
static const char * const s_status_str[] = {
	/* DBG_STATUS_OK */
	"Operation completed successfully",

	/* DBG_STATUS_APP_VERSION_NOT_SET */
	"Debug application version wasn't set",

	/* DBG_STATUS_UNSUPPORTED_APP_VERSION */
	"Unsupported debug application version",

	/* DBG_STATUS_DBG_BLOCK_NOT_RESET */
	"The debug block wasn't reset since the last recording",

	/* DBG_STATUS_INVALID_ARGS */
	"Invalid arguments",

	/* DBG_STATUS_OUTPUT_ALREADY_SET */
	"The debug output was already set",

	/* DBG_STATUS_INVALID_PCI_BUF_SIZE */
	"Invalid PCI buffer size",

	/* DBG_STATUS_PCI_BUF_ALLOC_FAILED */
	"PCI buffer allocation failed",

	/* DBG_STATUS_PCI_BUF_NOT_ALLOCATED */
	"A PCI buffer wasn't allocated",

<<<<<<< HEAD
	/* DBG_STATUS_TOO_MANY_INPUTS */
	"Too many inputs were enabled. Enabled less inputs, or set 'unifyInputs' to true",

	/* DBG_STATUS_INPUT_OVERLAP */
	"Overlapping debug bus inputs",

	/* DBG_STATUS_HW_ONLY_RECORDING */
	"Cannot record Storm data since the entire recording cycle is used by HW",
=======
	/* DBG_STATUS_INVALID_FILTER_TRIGGER_DWORDS */
	"The filter/trigger constraint dword offsets are not enabled for recording",
	/* DBG_STATUS_NO_MATCHING_FRAMING_MODE */
	"No matching framing mode",

	/* DBG_STATUS_VFC_READ_ERROR */
	"Error reading from VFC",
>>>>>>> upstream/android-13

	/* DBG_STATUS_STORM_ALREADY_ENABLED */
	"The Storm was already enabled",

	/* DBG_STATUS_STORM_NOT_ENABLED */
	"The specified Storm wasn't enabled",

	/* DBG_STATUS_BLOCK_ALREADY_ENABLED */
	"The block was already enabled",

	/* DBG_STATUS_BLOCK_NOT_ENABLED */
	"The specified block wasn't enabled",

	/* DBG_STATUS_NO_INPUT_ENABLED */
	"No input was enabled for recording",

<<<<<<< HEAD
	/* DBG_STATUS_NO_FILTER_TRIGGER_64B */
	"Filters and triggers are not allowed when recording in 64b units",
=======
	/* DBG_STATUS_NO_FILTER_TRIGGER_256B */
	"Filters and triggers are not allowed in E4 256-bit mode",
>>>>>>> upstream/android-13

	/* DBG_STATUS_FILTER_ALREADY_ENABLED */
	"The filter was already enabled",

	/* DBG_STATUS_TRIGGER_ALREADY_ENABLED */
	"The trigger was already enabled",

	/* DBG_STATUS_TRIGGER_NOT_ENABLED */
	"The trigger wasn't enabled",

	/* DBG_STATUS_CANT_ADD_CONSTRAINT */
	"A constraint can be added only after a filter was enabled or a trigger state was added",

	/* DBG_STATUS_TOO_MANY_TRIGGER_STATES */
	"Cannot add more than 3 trigger states",

	/* DBG_STATUS_TOO_MANY_CONSTRAINTS */
	"Cannot add more than 4 constraints per filter or trigger state",

	/* DBG_STATUS_RECORDING_NOT_STARTED */
	"The recording wasn't started",

	/* DBG_STATUS_DATA_DIDNT_TRIGGER */
	"A trigger was configured, but it didn't trigger",

	/* DBG_STATUS_NO_DATA_RECORDED */
	"No data was recorded",

	/* DBG_STATUS_DUMP_BUF_TOO_SMALL */
	"Dump buffer is too small",

	/* DBG_STATUS_DUMP_NOT_CHUNK_ALIGNED */
	"Dumped data is not aligned to chunks",

	/* DBG_STATUS_UNKNOWN_CHIP */
	"Unknown chip",

	/* DBG_STATUS_VIRT_MEM_ALLOC_FAILED */
	"Failed allocating virtual memory",

	/* DBG_STATUS_BLOCK_IN_RESET */
	"The input block is in reset",

	/* DBG_STATUS_INVALID_TRACE_SIGNATURE */
	"Invalid MCP trace signature found in NVRAM",

	/* DBG_STATUS_INVALID_NVRAM_BUNDLE */
	"Invalid bundle ID found in NVRAM",

	/* DBG_STATUS_NVRAM_GET_IMAGE_FAILED */
	"Failed getting NVRAM image",

	/* DBG_STATUS_NON_ALIGNED_NVRAM_IMAGE */
	"NVRAM image is not dword-aligned",

	/* DBG_STATUS_NVRAM_READ_FAILED */
	"Failed reading from NVRAM",

	/* DBG_STATUS_IDLE_CHK_PARSE_FAILED */
	"Idle check parsing failed",

	/* DBG_STATUS_MCP_TRACE_BAD_DATA */
	"MCP Trace data is corrupt",

	/* DBG_STATUS_MCP_TRACE_NO_META */
	"Dump doesn't contain meta data - it must be provided in image file",

	/* DBG_STATUS_MCP_COULD_NOT_HALT */
	"Failed to halt MCP",

	/* DBG_STATUS_MCP_COULD_NOT_RESUME */
	"Failed to resume MCP after halt",

<<<<<<< HEAD
	/* DBG_STATUS_RESERVED2 */
	"Reserved debug status - shouldn't be returned",
=======
	/* DBG_STATUS_RESERVED0 */
	"",
>>>>>>> upstream/android-13

	/* DBG_STATUS_SEMI_FIFO_NOT_EMPTY */
	"Failed to empty SEMI sync FIFO",

	/* DBG_STATUS_IGU_FIFO_BAD_DATA */
	"IGU FIFO data is corrupt",

	/* DBG_STATUS_MCP_COULD_NOT_MASK_PRTY */
	"MCP failed to mask parities",

	/* DBG_STATUS_FW_ASSERTS_PARSE_FAILED */
	"FW Asserts parsing failed",

	/* DBG_STATUS_REG_FIFO_BAD_DATA */
	"GRC FIFO data is corrupt",

	/* DBG_STATUS_PROTECTION_OVERRIDE_BAD_DATA */
	"Protection Override data is corrupt",

	/* DBG_STATUS_DBG_ARRAY_NOT_SET */
	"Debug arrays were not set (when using binary files, dbg_set_bin_ptr must be called)",

<<<<<<< HEAD
	/* DBG_STATUS_FILTER_BUG */
	"Debug Bus filtering requires the -unifyInputs option (due to a HW bug)",

	/* DBG_STATUS_NON_MATCHING_LINES */
	"Non-matching debug lines - all lines must be of the same type (either 128b or 256b)",

	/* DBG_STATUS_INVALID_TRIGGER_DWORD_OFFSET */
	"The selected trigger dword offset wasn't enabled in the recorded HW block",

	/* DBG_STATUS_DBG_BUS_IN_USE */
	"The debug bus is in use"
=======
	/* DBG_STATUS_RESERVED1 */
	"",

	/* DBG_STATUS_NON_MATCHING_LINES */
	"Non-matching debug lines - in E4, all lines must be of the same type (either 128b or 256b)",

	/* DBG_STATUS_INSUFFICIENT_HW_IDS */
	"Insufficient HW IDs. Try to record less Storms/blocks",

	/* DBG_STATUS_DBG_BUS_IN_USE */
	"The debug bus is in use",

	/* DBG_STATUS_INVALID_STORM_DBG_MODE */
	"The storm debug mode is not supported in the current chip",

	/* DBG_STATUS_OTHER_ENGINE_BB_ONLY */
	"Other engine is supported only in BB",

	/* DBG_STATUS_FILTER_SINGLE_HW_ID */
	"The configured filter mode requires a single Storm/block input",

	/* DBG_STATUS_TRIGGER_SINGLE_HW_ID */
	"The configured filter mode requires that all the constraints of a single trigger state will be defined on a single Storm/block input",

	/* DBG_STATUS_MISSING_TRIGGER_STATE_STORM */
	"When triggering on Storm data, the Storm to trigger on must be specified"
>>>>>>> upstream/android-13
};

/* Idle check severity names array */
static const char * const s_idle_chk_severity_str[] = {
	"Error",
	"Error if no traffic",
	"Warning"
};

/* MCP Trace level names array */
static const char * const s_mcp_trace_level_str[] = {
	"ERROR",
	"TRACE",
	"DEBUG"
};

/* Access type names array */
static const char * const s_access_strs[] = {
	"read",
	"write"
};

/* Privilege type names array */
static const char * const s_privilege_strs[] = {
	"VF",
	"PDA",
	"HV",
	"UA"
};

/* Protection type names array */
static const char * const s_protection_strs[] = {
	"(default)",
	"(default)",
	"(default)",
	"(default)",
	"override VF",
	"override PDA",
	"override HV",
	"override UA"
};

/* Master type names array */
static const char * const s_master_strs[] = {
	"???",
	"pxp",
	"mcp",
	"msdm",
	"psdm",
	"ysdm",
	"usdm",
	"tsdm",
	"xsdm",
	"dbu",
	"dmae",
<<<<<<< HEAD
	"???",
=======
	"jdap",
>>>>>>> upstream/android-13
	"???",
	"???",
	"???",
	"???"
};

/* REG FIFO error messages array */
<<<<<<< HEAD
static const char * const s_reg_fifo_error_strs[] = {
	"grc timeout",
	"address doesn't belong to any block",
	"reserved address in block or write to read-only address",
	"privilege/protection mismatch",
	"path isolation error"
=======
static struct reg_fifo_err s_reg_fifo_errors[] = {
	{1, "grc timeout"},
	{2, "address doesn't belong to any block"},
	{4, "reserved address in block or write to read-only address"},
	{8, "privilege/protection mismatch"},
	{16, "path isolation error"},
	{17, "RSL error"}
>>>>>>> upstream/android-13
};

/* IGU FIFO sources array */
static const char * const s_igu_fifo_source_strs[] = {
	"TSTORM",
	"MSTORM",
	"USTORM",
	"XSTORM",
	"YSTORM",
	"PSTORM",
	"PCIE",
	"NIG_QM_PBF",
	"CAU",
	"ATTN",
	"GRC",
};

/* IGU FIFO error messages */
static const char * const s_igu_fifo_error_strs[] = {
	"no error",
	"length error",
	"function disabled",
<<<<<<< HEAD
	"VF sent command to attnetion address",
=======
	"VF sent command to attention address",
>>>>>>> upstream/android-13
	"host sent prod update command",
	"read of during interrupt register while in MIMD mode",
	"access to PXP BAR reserved address",
	"producer update command to attention index",
	"unknown error",
	"SB index not valid",
	"SB relative index and FID not found",
	"FID not match",
	"command with error flag asserted (PCI error or CAU discard)",
	"VF sent cleanup and RF cleanup is disabled",
	"cleanup command on type bigger than 4"
};

/* IGU FIFO address data */
static const struct igu_fifo_addr_data s_igu_fifo_addr_data[] = {
	{0x0, 0x101, "MSI-X Memory", NULL,
	 IGU_ADDR_TYPE_MSIX_MEM},
	{0x102, 0x1ff, "reserved", NULL,
	 IGU_ADDR_TYPE_RESERVED},
	{0x200, 0x200, "Write PBA[0:63]", NULL,
	 IGU_ADDR_TYPE_WRITE_PBA},
	{0x201, 0x201, "Write PBA[64:127]", "reserved",
	 IGU_ADDR_TYPE_WRITE_PBA},
	{0x202, 0x202, "Write PBA[128]", "reserved",
	 IGU_ADDR_TYPE_WRITE_PBA},
	{0x203, 0x3ff, "reserved", NULL,
	 IGU_ADDR_TYPE_RESERVED},
	{0x400, 0x5ef, "Write interrupt acknowledgment", NULL,
	 IGU_ADDR_TYPE_WRITE_INT_ACK},
	{0x5f0, 0x5f0, "Attention bits update", NULL,
	 IGU_ADDR_TYPE_WRITE_ATTN_BITS},
	{0x5f1, 0x5f1, "Attention bits set", NULL,
	 IGU_ADDR_TYPE_WRITE_ATTN_BITS},
	{0x5f2, 0x5f2, "Attention bits clear", NULL,
	 IGU_ADDR_TYPE_WRITE_ATTN_BITS},
	{0x5f3, 0x5f3, "Read interrupt 0:63 with mask", NULL,
	 IGU_ADDR_TYPE_READ_INT},
	{0x5f4, 0x5f4, "Read interrupt 0:31 with mask", NULL,
	 IGU_ADDR_TYPE_READ_INT},
	{0x5f5, 0x5f5, "Read interrupt 32:63 with mask", NULL,
	 IGU_ADDR_TYPE_READ_INT},
	{0x5f6, 0x5f6, "Read interrupt 0:63 without mask", NULL,
	 IGU_ADDR_TYPE_READ_INT},
	{0x5f7, 0x5ff, "reserved", NULL,
	 IGU_ADDR_TYPE_RESERVED},
	{0x600, 0x7ff, "Producer update", NULL,
	 IGU_ADDR_TYPE_WRITE_PROD_UPDATE}
};

/******************************** Variables **********************************/

<<<<<<< HEAD
/* MCP Trace meta data array - used in case the dump doesn't contain the
 * meta data (e.g. due to no NVRAM access).
 */
static struct user_dbg_array s_mcp_trace_meta_arr = { NULL, 0 };

/* Parsed MCP Trace meta data info, based on MCP trace meta array */
static struct mcp_trace_meta s_mcp_trace_meta;
static bool s_mcp_trace_meta_valid;

=======
>>>>>>> upstream/android-13
/* Temporary buffer, used for print size calculations */
static char s_temp_buf[MAX_MSG_LEN];

/**************************** Private Functions ******************************/

static u32 qed_cyclic_add(u32 a, u32 b, u32 size)
{
	return (a + b) % size;
}

static u32 qed_cyclic_sub(u32 a, u32 b, u32 size)
{
	return (size + a - b) % size;
}

/* Reads the specified number of bytes from the specified cyclic buffer (up to 4
 * bytes) and returns them as a dword value. the specified buffer offset is
 * updated.
 */
static u32 qed_read_from_cyclic_buf(void *buf,
				    u32 *offset,
				    u32 buf_size, u8 num_bytes_to_read)
{
	u8 i, *val_ptr, *bytes_buf = (u8 *)buf;
	u32 val = 0;

	val_ptr = (u8 *)&val;

	/* Assume running on a LITTLE ENDIAN and the buffer is network order
	 * (BIG ENDIAN), as high order bytes are placed in lower memory address.
	 */
	for (i = 0; i < num_bytes_to_read; i++) {
		val_ptr[i] = bytes_buf[*offset];
		*offset = qed_cyclic_add(*offset, 1, buf_size);
	}

	return val;
}

/* Reads and returns the next byte from the specified buffer.
 * The specified buffer offset is updated.
 */
static u8 qed_read_byte_from_buf(void *buf, u32 *offset)
{
	return ((u8 *)buf)[(*offset)++];
}

/* Reads and returns the next dword from the specified buffer.
 * The specified buffer offset is updated.
 */
static u32 qed_read_dword_from_buf(void *buf, u32 *offset)
{
	u32 dword_val = *(u32 *)&((u8 *)buf)[*offset];

	*offset += 4;

	return dword_val;
}

/* Reads the next string from the specified buffer, and copies it to the
 * specified pointer. The specified buffer offset is updated.
 */
static void qed_read_str_from_buf(void *buf, u32 *offset, u32 size, char *dest)
{
	const char *source_str = &((const char *)buf)[*offset];

	strncpy(dest, source_str, size);
	dest[size - 1] = '\0';
	*offset += size;
}

/* Returns a pointer to the specified offset (in bytes) of the specified buffer.
 * If the specified buffer in NULL, a temporary buffer pointer is returned.
 */
static char *qed_get_buf_ptr(void *buf, u32 offset)
{
	return buf ? (char *)buf + offset : s_temp_buf;
}

/* Reads a param from the specified buffer. Returns the number of dwords read.
 * If the returned str_param is NULL, the param is numeric and its value is
 * returned in num_param.
 * Otheriwise, the param is a string and its pointer is returned in str_param.
 */
static u32 qed_read_param(u32 *dump_buf,
			  const char **param_name,
			  const char **param_str_val, u32 *param_num_val)
{
	char *char_buf = (char *)dump_buf;
	size_t offset = 0;

	/* Extract param name */
	*param_name = char_buf;
	offset += strlen(*param_name) + 1;

	/* Check param type */
	if (*(char_buf + offset++)) {
		/* String param */
		*param_str_val = char_buf + offset;
		*param_num_val = 0;
		offset += strlen(*param_str_val) + 1;
		if (offset & 0x3)
			offset += (4 - (offset & 0x3));
	} else {
		/* Numeric param */
		*param_str_val = NULL;
		if (offset & 0x3)
			offset += (4 - (offset & 0x3));
		*param_num_val = *(u32 *)(char_buf + offset);
		offset += 4;
	}

	return (u32)offset / 4;
}

/* Reads a section header from the specified buffer.
 * Returns the number of dwords read.
 */
static u32 qed_read_section_hdr(u32 *dump_buf,
				const char **section_name,
				u32 *num_section_params)
{
	const char *param_str_val;

	return qed_read_param(dump_buf,
			      section_name, &param_str_val, num_section_params);
}

/* Reads section params from the specified buffer and prints them to the results
 * buffer. Returns the number of dwords read.
 */
static u32 qed_print_section_params(u32 *dump_buf,
				    u32 num_section_params,
				    char *results_buf, u32 *num_chars_printed)
{
	u32 i, dump_offset = 0, results_offset = 0;

	for (i = 0; i < num_section_params; i++) {
		const char *param_name, *param_str_val;
		u32 param_num_val = 0;

		dump_offset += qed_read_param(dump_buf + dump_offset,
					      &param_name,
					      &param_str_val, &param_num_val);

		if (param_str_val)
			results_offset +=
				sprintf(qed_get_buf_ptr(results_buf,
							results_offset),
					"%s: %s\n", param_name, param_str_val);
		else if (strcmp(param_name, "fw-timestamp"))
			results_offset +=
				sprintf(qed_get_buf_ptr(results_buf,
							results_offset),
					"%s: %d\n", param_name, param_num_val);
	}

	results_offset += sprintf(qed_get_buf_ptr(results_buf, results_offset),
				  "\n");

	*num_chars_printed = results_offset;

	return dump_offset;
}

<<<<<<< HEAD
/* Parses the idle check rules and returns the number of characters printed.
 * In case of parsing error, returns 0.
 */
static u32 qed_parse_idle_chk_dump_rules(u32 *dump_buf,
=======
/* Returns the block name that matches the specified block ID,
 * or NULL if not found.
 */
static const char *qed_dbg_get_block_name(struct qed_hwfn *p_hwfn,
					  enum block_id block_id)
{
	const struct dbg_block_user *block =
	    (const struct dbg_block_user *)
	    p_hwfn->dbg_arrays[BIN_BUF_DBG_BLOCKS_USER_DATA].ptr + block_id;

	return (const char *)block->name;
}

static struct dbg_tools_user_data *qed_dbg_get_user_data(struct qed_hwfn
							 *p_hwfn)
{
	return (struct dbg_tools_user_data *)p_hwfn->dbg_user_info;
}

/* Parses the idle check rules and returns the number of characters printed.
 * In case of parsing error, returns 0.
 */
static u32 qed_parse_idle_chk_dump_rules(struct qed_hwfn *p_hwfn,
					 u32 *dump_buf,
>>>>>>> upstream/android-13
					 u32 *dump_buf_end,
					 u32 num_rules,
					 bool print_fw_idle_chk,
					 char *results_buf,
					 u32 *num_errors, u32 *num_warnings)
{
	/* Offset in results_buf in bytes */
	u32 results_offset = 0;

	u32 rule_idx;
	u16 i, j;

	*num_errors = 0;
	*num_warnings = 0;

	/* Go over dumped results */
	for (rule_idx = 0; rule_idx < num_rules && dump_buf < dump_buf_end;
	     rule_idx++) {
		const struct dbg_idle_chk_rule_parsing_data *rule_parsing_data;
		struct dbg_idle_chk_result_hdr *hdr;
		const char *parsing_str, *lsi_msg;
		u32 parsing_str_offset;
		bool has_fw_msg;
		u8 curr_reg_id;

		hdr = (struct dbg_idle_chk_result_hdr *)dump_buf;
		rule_parsing_data =
<<<<<<< HEAD
			(const struct dbg_idle_chk_rule_parsing_data *)
			&s_user_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_PARSING_DATA].
			ptr[hdr->rule_id];
		parsing_str_offset =
			GET_FIELD(rule_parsing_data->data,
				  DBG_IDLE_CHK_RULE_PARSING_DATA_STR_OFFSET);
		has_fw_msg =
			GET_FIELD(rule_parsing_data->data,
				DBG_IDLE_CHK_RULE_PARSING_DATA_HAS_FW_MSG) > 0;
		parsing_str =
			&((const char *)
			s_user_dbg_arrays[BIN_BUF_DBG_PARSING_STRINGS].ptr)
			[parsing_str_offset];
=======
		    (const struct dbg_idle_chk_rule_parsing_data *)
		    p_hwfn->dbg_arrays[BIN_BUF_DBG_IDLE_CHK_PARSING_DATA].ptr +
		    hdr->rule_id;
		parsing_str_offset =
		    GET_FIELD(rule_parsing_data->data,
			      DBG_IDLE_CHK_RULE_PARSING_DATA_STR_OFFSET);
		has_fw_msg =
		    GET_FIELD(rule_parsing_data->data,
			      DBG_IDLE_CHK_RULE_PARSING_DATA_HAS_FW_MSG) > 0;
		parsing_str = (const char *)
		    p_hwfn->dbg_arrays[BIN_BUF_DBG_PARSING_STRINGS].ptr +
		    parsing_str_offset;
>>>>>>> upstream/android-13
		lsi_msg = parsing_str;
		curr_reg_id = 0;

		if (hdr->severity >= MAX_DBG_IDLE_CHK_SEVERITY_TYPES)
			return 0;

		/* Skip rule header */
		dump_buf += BYTES_TO_DWORDS(sizeof(*hdr));

		/* Update errors/warnings count */
		if (hdr->severity == IDLE_CHK_SEVERITY_ERROR ||
		    hdr->severity == IDLE_CHK_SEVERITY_ERROR_NO_TRAFFIC)
			(*num_errors)++;
		else
			(*num_warnings)++;

		/* Print rule severity */
		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf,
					    results_offset), "%s: ",
			    s_idle_chk_severity_str[hdr->severity]);

		/* Print rule message */
		if (has_fw_msg)
			parsing_str += strlen(parsing_str) + 1;
		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf,
					    results_offset), "%s.",
			    has_fw_msg &&
			    print_fw_idle_chk ? parsing_str : lsi_msg);
		parsing_str += strlen(parsing_str) + 1;

		/* Print register values */
		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf,
					    results_offset), " Registers:");
		for (i = 0;
		     i < hdr->num_dumped_cond_regs + hdr->num_dumped_info_regs;
		     i++) {
			struct dbg_idle_chk_result_reg_hdr *reg_hdr;
			bool is_mem;
			u8 reg_id;

			reg_hdr =
				(struct dbg_idle_chk_result_reg_hdr *)dump_buf;
			is_mem = GET_FIELD(reg_hdr->data,
					   DBG_IDLE_CHK_RESULT_REG_HDR_IS_MEM);
			reg_id = GET_FIELD(reg_hdr->data,
					   DBG_IDLE_CHK_RESULT_REG_HDR_REG_ID);

			/* Skip reg header */
			dump_buf += BYTES_TO_DWORDS(sizeof(*reg_hdr));

			/* Skip register names until the required reg_id is
			 * reached.
			 */
			for (; reg_id > curr_reg_id;
			     curr_reg_id++,
			     parsing_str += strlen(parsing_str) + 1);

			results_offset +=
			    sprintf(qed_get_buf_ptr(results_buf,
						    results_offset), " %s",
				    parsing_str);
			if (i < hdr->num_dumped_cond_regs && is_mem)
				results_offset +=
				    sprintf(qed_get_buf_ptr(results_buf,
							    results_offset),
					    "[%d]", hdr->mem_entry_id +
					    reg_hdr->start_entry);
			results_offset +=
			    sprintf(qed_get_buf_ptr(results_buf,
						    results_offset), "=");
			for (j = 0; j < reg_hdr->size; j++, dump_buf++) {
				results_offset +=
				    sprintf(qed_get_buf_ptr(results_buf,
							    results_offset),
					    "0x%x", *dump_buf);
				if (j < reg_hdr->size - 1)
					results_offset +=
					    sprintf(qed_get_buf_ptr
						    (results_buf,
						     results_offset), ",");
			}
		}

		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf, results_offset), "\n");
	}

	/* Check if end of dump buffer was exceeded */
	if (dump_buf > dump_buf_end)
		return 0;

	return results_offset;
}

/* Parses an idle check dump buffer.
 * If result_buf is not NULL, the idle check results are printed to it.
 * In any case, the required results buffer size is assigned to
 * parsed_results_bytes.
 * The parsing status is returned.
 */
<<<<<<< HEAD
static enum dbg_status qed_parse_idle_chk_dump(u32 *dump_buf,
=======
static enum dbg_status qed_parse_idle_chk_dump(struct qed_hwfn *p_hwfn,
					       u32 *dump_buf,
>>>>>>> upstream/android-13
					       u32 num_dumped_dwords,
					       char *results_buf,
					       u32 *parsed_results_bytes,
					       u32 *num_errors,
					       u32 *num_warnings)
{
	const char *section_name, *param_name, *param_str_val;
	u32 *dump_buf_end = dump_buf + num_dumped_dwords;
	u32 num_section_params = 0, num_rules;

	/* Offset in results_buf in bytes */
	u32 results_offset = 0;

	*parsed_results_bytes = 0;
	*num_errors = 0;
	*num_warnings = 0;

<<<<<<< HEAD
	if (!s_user_dbg_arrays[BIN_BUF_DBG_PARSING_STRINGS].ptr ||
	    !s_user_dbg_arrays[BIN_BUF_DBG_IDLE_CHK_PARSING_DATA].ptr)
=======
	if (!p_hwfn->dbg_arrays[BIN_BUF_DBG_PARSING_STRINGS].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_IDLE_CHK_PARSING_DATA].ptr)
>>>>>>> upstream/android-13
		return DBG_STATUS_DBG_ARRAY_NOT_SET;

	/* Read global_params section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "global_params"))
		return DBG_STATUS_IDLE_CHK_PARSE_FAILED;

	/* Print global params */
	dump_buf += qed_print_section_params(dump_buf,
					     num_section_params,
					     results_buf, &results_offset);

	/* Read idle_chk section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "idle_chk") || num_section_params != 1)
		return DBG_STATUS_IDLE_CHK_PARSE_FAILED;
	dump_buf += qed_read_param(dump_buf,
				   &param_name, &param_str_val, &num_rules);
	if (strcmp(param_name, "num_rules"))
		return DBG_STATUS_IDLE_CHK_PARSE_FAILED;

	if (num_rules) {
		u32 rules_print_size;

		/* Print FW output */
		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf,
					    results_offset),
			    "FW_IDLE_CHECK:\n");
		rules_print_size =
<<<<<<< HEAD
			qed_parse_idle_chk_dump_rules(dump_buf,
=======
			qed_parse_idle_chk_dump_rules(p_hwfn,
						      dump_buf,
>>>>>>> upstream/android-13
						      dump_buf_end,
						      num_rules,
						      true,
						      results_buf ?
						      results_buf +
						      results_offset :
						      NULL,
						      num_errors,
						      num_warnings);
		results_offset += rules_print_size;
		if (!rules_print_size)
			return DBG_STATUS_IDLE_CHK_PARSE_FAILED;

		/* Print LSI output */
		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf,
					    results_offset),
			    "\nLSI_IDLE_CHECK:\n");
		rules_print_size =
<<<<<<< HEAD
			qed_parse_idle_chk_dump_rules(dump_buf,
=======
			qed_parse_idle_chk_dump_rules(p_hwfn,
						      dump_buf,
>>>>>>> upstream/android-13
						      dump_buf_end,
						      num_rules,
						      false,
						      results_buf ?
						      results_buf +
						      results_offset :
						      NULL,
						      num_errors,
						      num_warnings);
		results_offset += rules_print_size;
		if (!rules_print_size)
			return DBG_STATUS_IDLE_CHK_PARSE_FAILED;
	}

	/* Print errors/warnings count */
	if (*num_errors)
		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf,
					    results_offset),
			    "\nIdle Check failed!!! (with %d errors and %d warnings)\n",
			    *num_errors, *num_warnings);
	else if (*num_warnings)
		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf,
					    results_offset),
			    "\nIdle Check completed successfully (with %d warnings)\n",
			    *num_warnings);
	else
		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf,
					    results_offset),
			    "\nIdle Check completed successfully\n");

	/* Add 1 for string NULL termination */
	*parsed_results_bytes = results_offset + 1;

	return DBG_STATUS_OK;
}

<<<<<<< HEAD
/* Frees the specified MCP Trace meta data */
static void qed_mcp_trace_free_meta(struct qed_hwfn *p_hwfn,
				    struct mcp_trace_meta *meta)
{
	u32 i;

	s_mcp_trace_meta_valid = false;

	/* Release modules */
	if (meta->modules) {
		for (i = 0; i < meta->modules_num; i++)
			kfree(meta->modules[i]);
		kfree(meta->modules);
	}

	/* Release formats */
	if (meta->formats) {
		for (i = 0; i < meta->formats_num; i++)
			kfree(meta->formats[i].format_str);
		kfree(meta->formats);
	}
}

=======
>>>>>>> upstream/android-13
/* Allocates and fills MCP Trace meta data based on the specified meta data
 * dump buffer.
 * Returns debug status code.
 */
<<<<<<< HEAD
static enum dbg_status qed_mcp_trace_alloc_meta(struct qed_hwfn *p_hwfn,
						const u32 *meta_buf,
						struct mcp_trace_meta *meta)
{
	u8 *meta_buf_bytes = (u8 *)meta_buf;
	u32 offset = 0, signature, i;

	/* Free the previous meta before loading a new one. */
	if (s_mcp_trace_meta_valid)
		qed_mcp_trace_free_meta(p_hwfn, meta);
=======
static enum dbg_status
qed_mcp_trace_alloc_meta_data(struct qed_hwfn *p_hwfn,
			      const u32 *meta_buf)
{
	struct dbg_tools_user_data *dev_user_data;
	u32 offset = 0, signature, i;
	struct mcp_trace_meta *meta;
	u8 *meta_buf_bytes;

	dev_user_data = qed_dbg_get_user_data(p_hwfn);
	meta = &dev_user_data->mcp_trace_meta;
	meta_buf_bytes = (u8 *)meta_buf;

	/* Free the previous meta before loading a new one. */
	if (meta->is_allocated)
		qed_mcp_trace_free_meta_data(p_hwfn);
>>>>>>> upstream/android-13

	memset(meta, 0, sizeof(*meta));

	/* Read first signature */
	signature = qed_read_dword_from_buf(meta_buf_bytes, &offset);
	if (signature != NVM_MAGIC_VALUE)
		return DBG_STATUS_INVALID_TRACE_SIGNATURE;

	/* Read no. of modules and allocate memory for their pointers */
	meta->modules_num = qed_read_byte_from_buf(meta_buf_bytes, &offset);
	meta->modules = kcalloc(meta->modules_num, sizeof(char *),
				GFP_KERNEL);
	if (!meta->modules)
		return DBG_STATUS_VIRT_MEM_ALLOC_FAILED;

	/* Allocate and read all module strings */
	for (i = 0; i < meta->modules_num; i++) {
		u8 module_len = qed_read_byte_from_buf(meta_buf_bytes, &offset);

		*(meta->modules + i) = kzalloc(module_len, GFP_KERNEL);
		if (!(*(meta->modules + i))) {
			/* Update number of modules to be released */
			meta->modules_num = i ? i - 1 : 0;
			return DBG_STATUS_VIRT_MEM_ALLOC_FAILED;
		}

		qed_read_str_from_buf(meta_buf_bytes, &offset, module_len,
				      *(meta->modules + i));
		if (module_len > MCP_TRACE_MAX_MODULE_LEN)
			(*(meta->modules + i))[MCP_TRACE_MAX_MODULE_LEN] = '\0';
	}

	/* Read second signature */
	signature = qed_read_dword_from_buf(meta_buf_bytes, &offset);
	if (signature != NVM_MAGIC_VALUE)
		return DBG_STATUS_INVALID_TRACE_SIGNATURE;

	/* Read number of formats and allocate memory for all formats */
	meta->formats_num = qed_read_dword_from_buf(meta_buf_bytes, &offset);
	meta->formats = kcalloc(meta->formats_num,
				sizeof(struct mcp_trace_format),
				GFP_KERNEL);
	if (!meta->formats)
		return DBG_STATUS_VIRT_MEM_ALLOC_FAILED;

	/* Allocate and read all strings */
	for (i = 0; i < meta->formats_num; i++) {
		struct mcp_trace_format *format_ptr = &meta->formats[i];
		u8 format_len;

		format_ptr->data = qed_read_dword_from_buf(meta_buf_bytes,
							   &offset);
<<<<<<< HEAD
		format_len =
		    (format_ptr->data &
		     MCP_TRACE_FORMAT_LEN_MASK) >> MCP_TRACE_FORMAT_LEN_SHIFT;
=======
		format_len = GET_MFW_FIELD(format_ptr->data,
					   MCP_TRACE_FORMAT_LEN);
>>>>>>> upstream/android-13
		format_ptr->format_str = kzalloc(format_len, GFP_KERNEL);
		if (!format_ptr->format_str) {
			/* Update number of modules to be released */
			meta->formats_num = i ? i - 1 : 0;
			return DBG_STATUS_VIRT_MEM_ALLOC_FAILED;
		}

		qed_read_str_from_buf(meta_buf_bytes,
				      &offset,
				      format_len, format_ptr->format_str);
	}

<<<<<<< HEAD
	s_mcp_trace_meta_valid = true;
=======
	meta->is_allocated = true;
>>>>>>> upstream/android-13
	return DBG_STATUS_OK;
}

/* Parses an MCP trace buffer. If result_buf is not NULL, the MCP Trace results
 * are printed to it. The parsing status is returned.
 * Arguments:
 * trace_buf - MCP trace cyclic buffer
 * trace_buf_size - MCP trace cyclic buffer size in bytes
 * data_offset - offset in bytes of the data to parse in the MCP trace cyclic
<<<<<<< HEAD
 *               buffer.
 * data_size - size in bytes of data to parse.
 * parsed_buf - destination buffer for parsed data.
 * parsed_bytes - size of parsed data in bytes.
 */
static enum dbg_status qed_parse_mcp_trace_buf(u8 *trace_buf,
=======
 *		 buffer.
 * data_size - size in bytes of data to parse.
 * parsed_buf - destination buffer for parsed data.
 * parsed_results_bytes - size of parsed data in bytes.
 */
static enum dbg_status qed_parse_mcp_trace_buf(struct qed_hwfn *p_hwfn,
					       u8 *trace_buf,
>>>>>>> upstream/android-13
					       u32 trace_buf_size,
					       u32 data_offset,
					       u32 data_size,
					       char *parsed_buf,
<<<<<<< HEAD
					       u32 *parsed_bytes)
{
	u32 param_mask, param_shift;
	enum dbg_status status;

	*parsed_bytes = 0;

	if (!s_mcp_trace_meta_valid)
=======
					       u32 *parsed_results_bytes)
{
	struct dbg_tools_user_data *dev_user_data;
	struct mcp_trace_meta *meta;
	u32 param_mask, param_shift;
	enum dbg_status status;

	dev_user_data = qed_dbg_get_user_data(p_hwfn);
	meta = &dev_user_data->mcp_trace_meta;
	*parsed_results_bytes = 0;

	if (!meta->is_allocated)
>>>>>>> upstream/android-13
		return DBG_STATUS_MCP_TRACE_BAD_DATA;

	status = DBG_STATUS_OK;

	while (data_size) {
		struct mcp_trace_format *format_ptr;
		u8 format_level, format_module;
		u32 params[3] = { 0, 0, 0 };
		u32 header, format_idx, i;

		if (data_size < MFW_TRACE_ENTRY_SIZE)
			return DBG_STATUS_MCP_TRACE_BAD_DATA;

		header = qed_read_from_cyclic_buf(trace_buf,
						  &data_offset,
						  trace_buf_size,
						  MFW_TRACE_ENTRY_SIZE);
		data_size -= MFW_TRACE_ENTRY_SIZE;
		format_idx = header & MFW_TRACE_EVENTID_MASK;

		/* Skip message if its index doesn't exist in the meta data */
<<<<<<< HEAD
		if (format_idx >= s_mcp_trace_meta.formats_num) {
			u8 format_size =
				(u8)((header & MFW_TRACE_PRM_SIZE_MASK) >>
				     MFW_TRACE_PRM_SIZE_SHIFT);
=======
		if (format_idx >= meta->formats_num) {
			u8 format_size = (u8)GET_MFW_FIELD(header,
							   MFW_TRACE_PRM_SIZE);
>>>>>>> upstream/android-13

			if (data_size < format_size)
				return DBG_STATUS_MCP_TRACE_BAD_DATA;

			data_offset = qed_cyclic_add(data_offset,
						     format_size,
						     trace_buf_size);
			data_size -= format_size;
			continue;
		}

<<<<<<< HEAD
		format_ptr = &s_mcp_trace_meta.formats[format_idx];

		for (i = 0,
		     param_mask = MCP_TRACE_FORMAT_P1_SIZE_MASK,
		     param_shift = MCP_TRACE_FORMAT_P1_SIZE_SHIFT;
		     i < MCP_TRACE_FORMAT_MAX_PARAMS;
		     i++,
		     param_mask <<= MCP_TRACE_FORMAT_PARAM_WIDTH,
=======
		format_ptr = &meta->formats[format_idx];

		for (i = 0,
		     param_mask = MCP_TRACE_FORMAT_P1_SIZE_MASK, param_shift =
		     MCP_TRACE_FORMAT_P1_SIZE_OFFSET;
		     i < MCP_TRACE_FORMAT_MAX_PARAMS;
		     i++, param_mask <<= MCP_TRACE_FORMAT_PARAM_WIDTH,
>>>>>>> upstream/android-13
		     param_shift += MCP_TRACE_FORMAT_PARAM_WIDTH) {
			/* Extract param size (0..3) */
			u8 param_size = (u8)((format_ptr->data & param_mask) >>
					     param_shift);

			/* If the param size is zero, there are no other
			 * parameters.
			 */
			if (!param_size)
				break;

			/* Size is encoded using 2 bits, where 3 is used to
			 * encode 4.
			 */
			if (param_size == 3)
				param_size = 4;

			if (data_size < param_size)
				return DBG_STATUS_MCP_TRACE_BAD_DATA;

			params[i] = qed_read_from_cyclic_buf(trace_buf,
							     &data_offset,
							     trace_buf_size,
							     param_size);
			data_size -= param_size;
		}

<<<<<<< HEAD
		format_level = (u8)((format_ptr->data &
				     MCP_TRACE_FORMAT_LEVEL_MASK) >>
				    MCP_TRACE_FORMAT_LEVEL_SHIFT);
		format_module = (u8)((format_ptr->data &
				      MCP_TRACE_FORMAT_MODULE_MASK) >>
				     MCP_TRACE_FORMAT_MODULE_SHIFT);
=======
		format_level = (u8)GET_MFW_FIELD(format_ptr->data,
						 MCP_TRACE_FORMAT_LEVEL);
		format_module = (u8)GET_MFW_FIELD(format_ptr->data,
						  MCP_TRACE_FORMAT_MODULE);
>>>>>>> upstream/android-13
		if (format_level >= ARRAY_SIZE(s_mcp_trace_level_str))
			return DBG_STATUS_MCP_TRACE_BAD_DATA;

		/* Print current message to results buffer */
<<<<<<< HEAD
		*parsed_bytes +=
			sprintf(qed_get_buf_ptr(parsed_buf, *parsed_bytes),
				"%s %-8s: ",
				s_mcp_trace_level_str[format_level],
				s_mcp_trace_meta.modules[format_module]);
		*parsed_bytes +=
		    sprintf(qed_get_buf_ptr(parsed_buf, *parsed_bytes),
=======
		*parsed_results_bytes +=
			sprintf(qed_get_buf_ptr(parsed_buf,
						*parsed_results_bytes),
				"%s %-8s: ",
				s_mcp_trace_level_str[format_level],
				meta->modules[format_module]);
		*parsed_results_bytes +=
		    sprintf(qed_get_buf_ptr(parsed_buf, *parsed_results_bytes),
>>>>>>> upstream/android-13
			    format_ptr->format_str,
			    params[0], params[1], params[2]);
	}

	/* Add string NULL terminator */
<<<<<<< HEAD
	(*parsed_bytes)++;
=======
	(*parsed_results_bytes)++;
>>>>>>> upstream/android-13

	return status;
}

/* Parses an MCP Trace dump buffer.
 * If result_buf is not NULL, the MCP Trace results are printed to it.
 * In any case, the required results buffer size is assigned to
<<<<<<< HEAD
 * parsed_bytes.
=======
 * parsed_results_bytes.
>>>>>>> upstream/android-13
 * The parsing status is returned.
 */
static enum dbg_status qed_parse_mcp_trace_dump(struct qed_hwfn *p_hwfn,
						u32 *dump_buf,
<<<<<<< HEAD
						char *parsed_buf,
						u32 *parsed_bytes)
{
	const char *section_name, *param_name, *param_str_val;
	u32 data_size, trace_data_dwords, trace_meta_dwords;
	u32 offset, results_offset, parsed_buf_bytes;
=======
						char *results_buf,
						u32 *parsed_results_bytes,
						bool free_meta_data)
{
	const char *section_name, *param_name, *param_str_val;
	u32 data_size, trace_data_dwords, trace_meta_dwords;
	u32 offset, results_offset, results_buf_bytes;
>>>>>>> upstream/android-13
	u32 param_num_val, num_section_params;
	struct mcp_trace *trace;
	enum dbg_status status;
	const u32 *meta_buf;
	u8 *trace_buf;

<<<<<<< HEAD
	*parsed_bytes = 0;
=======
	*parsed_results_bytes = 0;
>>>>>>> upstream/android-13

	/* Read global_params section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "global_params"))
		return DBG_STATUS_MCP_TRACE_BAD_DATA;

	/* Print global params */
	dump_buf += qed_print_section_params(dump_buf,
					     num_section_params,
<<<<<<< HEAD
					     parsed_buf, &results_offset);
=======
					     results_buf, &results_offset);
>>>>>>> upstream/android-13

	/* Read trace_data section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "mcp_trace_data") || num_section_params != 1)
		return DBG_STATUS_MCP_TRACE_BAD_DATA;
	dump_buf += qed_read_param(dump_buf,
				   &param_name, &param_str_val, &param_num_val);
	if (strcmp(param_name, "size"))
		return DBG_STATUS_MCP_TRACE_BAD_DATA;
	trace_data_dwords = param_num_val;

	/* Prepare trace info */
	trace = (struct mcp_trace *)dump_buf;
<<<<<<< HEAD
=======
	if (trace->signature != MFW_TRACE_SIGNATURE || !trace->size)
		return DBG_STATUS_MCP_TRACE_BAD_DATA;

>>>>>>> upstream/android-13
	trace_buf = (u8 *)dump_buf + sizeof(*trace);
	offset = trace->trace_oldest;
	data_size = qed_cyclic_sub(trace->trace_prod, offset, trace->size);
	dump_buf += trace_data_dwords;

	/* Read meta_data section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "mcp_trace_meta"))
		return DBG_STATUS_MCP_TRACE_BAD_DATA;
	dump_buf += qed_read_param(dump_buf,
				   &param_name, &param_str_val, &param_num_val);
	if (strcmp(param_name, "size"))
		return DBG_STATUS_MCP_TRACE_BAD_DATA;
	trace_meta_dwords = param_num_val;

	/* Choose meta data buffer */
	if (!trace_meta_dwords) {
		/* Dump doesn't include meta data */
<<<<<<< HEAD
		if (!s_mcp_trace_meta_arr.ptr)
			return DBG_STATUS_MCP_TRACE_NO_META;
		meta_buf = s_mcp_trace_meta_arr.ptr;
=======
		struct dbg_tools_user_data *dev_user_data =
			qed_dbg_get_user_data(p_hwfn);

		if (!dev_user_data->mcp_trace_user_meta_buf)
			return DBG_STATUS_MCP_TRACE_NO_META;

		meta_buf = dev_user_data->mcp_trace_user_meta_buf;
>>>>>>> upstream/android-13
	} else {
		/* Dump includes meta data */
		meta_buf = dump_buf;
	}

	/* Allocate meta data memory */
<<<<<<< HEAD
	status = qed_mcp_trace_alloc_meta(p_hwfn, meta_buf, &s_mcp_trace_meta);
	if (status != DBG_STATUS_OK)
		return status;

	status = qed_parse_mcp_trace_buf(trace_buf,
					 trace->size,
					 offset,
					 data_size,
					 parsed_buf ?
					 parsed_buf + results_offset :
					 NULL,
					 &parsed_buf_bytes);
	if (status != DBG_STATUS_OK)
		return status;

	*parsed_bytes = results_offset + parsed_buf_bytes;
=======
	status = qed_mcp_trace_alloc_meta_data(p_hwfn, meta_buf);
	if (status != DBG_STATUS_OK)
		return status;

	status = qed_parse_mcp_trace_buf(p_hwfn,
					 trace_buf,
					 trace->size,
					 offset,
					 data_size,
					 results_buf ?
					 results_buf + results_offset :
					 NULL,
					 &results_buf_bytes);
	if (status != DBG_STATUS_OK)
		return status;

	if (free_meta_data)
		qed_mcp_trace_free_meta_data(p_hwfn);

	*parsed_results_bytes = results_offset + results_buf_bytes;
>>>>>>> upstream/android-13

	return DBG_STATUS_OK;
}

/* Parses a Reg FIFO dump buffer.
 * If result_buf is not NULL, the Reg FIFO results are printed to it.
 * In any case, the required results buffer size is assigned to
 * parsed_results_bytes.
 * The parsing status is returned.
 */
static enum dbg_status qed_parse_reg_fifo_dump(u32 *dump_buf,
					       char *results_buf,
					       u32 *parsed_results_bytes)
{
	const char *section_name, *param_name, *param_str_val;
	u32 param_num_val, num_section_params, num_elements;
	struct reg_fifo_element *elements;
<<<<<<< HEAD
	u8 i, j, err_val, vf_val;
=======
	u8 i, j, err_code, vf_val;
>>>>>>> upstream/android-13
	u32 results_offset = 0;
	char vf_str[4];

	/* Read global_params section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "global_params"))
		return DBG_STATUS_REG_FIFO_BAD_DATA;

	/* Print global params */
	dump_buf += qed_print_section_params(dump_buf,
					     num_section_params,
					     results_buf, &results_offset);

	/* Read reg_fifo_data section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "reg_fifo_data"))
		return DBG_STATUS_REG_FIFO_BAD_DATA;
	dump_buf += qed_read_param(dump_buf,
				   &param_name, &param_str_val, &param_num_val);
	if (strcmp(param_name, "size"))
		return DBG_STATUS_REG_FIFO_BAD_DATA;
	if (param_num_val % REG_FIFO_ELEMENT_DWORDS)
		return DBG_STATUS_REG_FIFO_BAD_DATA;
	num_elements = param_num_val / REG_FIFO_ELEMENT_DWORDS;
	elements = (struct reg_fifo_element *)dump_buf;

	/* Decode elements */
	for (i = 0; i < num_elements; i++) {
<<<<<<< HEAD
		bool err_printed = false;
=======
		const char *err_msg = NULL;
>>>>>>> upstream/android-13

		/* Discover if element belongs to a VF or a PF */
		vf_val = GET_FIELD(elements[i].data, REG_FIFO_ELEMENT_VF);
		if (vf_val == REG_FIFO_ELEMENT_IS_PF_VF_VAL)
			sprintf(vf_str, "%s", "N/A");
		else
			sprintf(vf_str, "%d", vf_val);

<<<<<<< HEAD
=======
		/* Find error message */
		err_code = GET_FIELD(elements[i].data, REG_FIFO_ELEMENT_ERROR);
		for (j = 0; j < ARRAY_SIZE(s_reg_fifo_errors) && !err_msg; j++)
			if (err_code == s_reg_fifo_errors[j].err_code)
				err_msg = s_reg_fifo_errors[j].err_msg;

>>>>>>> upstream/android-13
		/* Add parsed element to parsed buffer */
		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf,
					    results_offset),
<<<<<<< HEAD
			    "raw: 0x%016llx, address: 0x%07x, access: %-5s, pf: %2d, vf: %s, port: %d, privilege: %-3s, protection: %-12s, master: %-4s, errors: ",
=======
			    "raw: 0x%016llx, address: 0x%07x, access: %-5s, pf: %2d, vf: %s, port: %d, privilege: %-3s, protection: %-12s, master: %-4s, error: %s\n",
>>>>>>> upstream/android-13
			    elements[i].data,
			    (u32)GET_FIELD(elements[i].data,
					   REG_FIFO_ELEMENT_ADDRESS) *
			    REG_FIFO_ELEMENT_ADDR_FACTOR,
			    s_access_strs[GET_FIELD(elements[i].data,
						    REG_FIFO_ELEMENT_ACCESS)],
			    (u32)GET_FIELD(elements[i].data,
					   REG_FIFO_ELEMENT_PF),
			    vf_str,
			    (u32)GET_FIELD(elements[i].data,
					   REG_FIFO_ELEMENT_PORT),
			    s_privilege_strs[GET_FIELD(elements[i].data,
						REG_FIFO_ELEMENT_PRIVILEGE)],
			    s_protection_strs[GET_FIELD(elements[i].data,
						REG_FIFO_ELEMENT_PROTECTION)],
			    s_master_strs[GET_FIELD(elements[i].data,
<<<<<<< HEAD
						REG_FIFO_ELEMENT_MASTER)]);

		/* Print errors */
		for (j = 0,
		     err_val = GET_FIELD(elements[i].data,
					 REG_FIFO_ELEMENT_ERROR);
		     j < ARRAY_SIZE(s_reg_fifo_error_strs);
		     j++, err_val >>= 1) {
			if (err_val & 0x1) {
				if (err_printed)
					results_offset +=
					    sprintf(qed_get_buf_ptr
						    (results_buf,
						     results_offset), ", ");
				results_offset +=
				    sprintf(qed_get_buf_ptr
					    (results_buf, results_offset), "%s",
					    s_reg_fifo_error_strs[j]);
				err_printed = true;
			}
		}

		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf, results_offset), "\n");
=======
						    REG_FIFO_ELEMENT_MASTER)],
			    err_msg ? err_msg : "unknown error code");
>>>>>>> upstream/android-13
	}

	results_offset += sprintf(qed_get_buf_ptr(results_buf,
						  results_offset),
				  "fifo contained %d elements", num_elements);

	/* Add 1 for string NULL termination */
	*parsed_results_bytes = results_offset + 1;

	return DBG_STATUS_OK;
}

static enum dbg_status qed_parse_igu_fifo_element(struct igu_fifo_element
						  *element, char
						  *results_buf,
						  u32 *results_offset)
{
	const struct igu_fifo_addr_data *found_addr = NULL;
	u8 source, err_type, i, is_cleanup;
	char parsed_addr_data[32];
	char parsed_wr_data[256];
	u32 wr_data, prod_cons;
	bool is_wr_cmd, is_pf;
	u16 cmd_addr;
	u64 dword12;

	/* Dword12 (dword index 1 and 2) contains bits 32..95 of the
	 * FIFO element.
	 */
	dword12 = ((u64)element->dword2 << 32) | element->dword1;
	is_wr_cmd = GET_FIELD(dword12, IGU_FIFO_ELEMENT_DWORD12_IS_WR_CMD);
	is_pf = GET_FIELD(element->dword0, IGU_FIFO_ELEMENT_DWORD0_IS_PF);
	cmd_addr = GET_FIELD(element->dword0, IGU_FIFO_ELEMENT_DWORD0_CMD_ADDR);
	source = GET_FIELD(element->dword0, IGU_FIFO_ELEMENT_DWORD0_SOURCE);
	err_type = GET_FIELD(element->dword0, IGU_FIFO_ELEMENT_DWORD0_ERR_TYPE);

	if (source >= ARRAY_SIZE(s_igu_fifo_source_strs))
		return DBG_STATUS_IGU_FIFO_BAD_DATA;
	if (err_type >= ARRAY_SIZE(s_igu_fifo_error_strs))
		return DBG_STATUS_IGU_FIFO_BAD_DATA;

	/* Find address data */
	for (i = 0; i < ARRAY_SIZE(s_igu_fifo_addr_data) && !found_addr; i++) {
		const struct igu_fifo_addr_data *curr_addr =
			&s_igu_fifo_addr_data[i];

		if (cmd_addr >= curr_addr->start_addr && cmd_addr <=
		    curr_addr->end_addr)
			found_addr = curr_addr;
	}

	if (!found_addr)
		return DBG_STATUS_IGU_FIFO_BAD_DATA;

	/* Prepare parsed address data */
	switch (found_addr->type) {
	case IGU_ADDR_TYPE_MSIX_MEM:
		sprintf(parsed_addr_data, " vector_num = 0x%x", cmd_addr / 2);
		break;
	case IGU_ADDR_TYPE_WRITE_INT_ACK:
	case IGU_ADDR_TYPE_WRITE_PROD_UPDATE:
		sprintf(parsed_addr_data,
			" SB = 0x%x", cmd_addr - found_addr->start_addr);
		break;
	default:
		parsed_addr_data[0] = '\0';
	}

	if (!is_wr_cmd) {
		parsed_wr_data[0] = '\0';
		goto out;
	}

	/* Prepare parsed write data */
	wr_data = GET_FIELD(dword12, IGU_FIFO_ELEMENT_DWORD12_WR_DATA);
	prod_cons = GET_FIELD(wr_data, IGU_FIFO_WR_DATA_PROD_CONS);
	is_cleanup = GET_FIELD(wr_data, IGU_FIFO_WR_DATA_CMD_TYPE);

	if (source == IGU_SRC_ATTN) {
		sprintf(parsed_wr_data, "prod: 0x%x, ", prod_cons);
	} else {
		if (is_cleanup) {
			u8 cleanup_val, cleanup_type;

			cleanup_val =
				GET_FIELD(wr_data,
					  IGU_FIFO_CLEANUP_WR_DATA_CLEANUP_VAL);
			cleanup_type =
			    GET_FIELD(wr_data,
				      IGU_FIFO_CLEANUP_WR_DATA_CLEANUP_TYPE);

			sprintf(parsed_wr_data,
				"cmd_type: cleanup, cleanup_val: %s, cleanup_type : %d, ",
				cleanup_val ? "set" : "clear",
				cleanup_type);
		} else {
			u8 update_flag, en_dis_int_for_sb, segment;
			u8 timer_mask;

			update_flag = GET_FIELD(wr_data,
						IGU_FIFO_WR_DATA_UPDATE_FLAG);
			en_dis_int_for_sb =
				GET_FIELD(wr_data,
					  IGU_FIFO_WR_DATA_EN_DIS_INT_FOR_SB);
			segment = GET_FIELD(wr_data,
					    IGU_FIFO_WR_DATA_SEGMENT);
			timer_mask = GET_FIELD(wr_data,
					       IGU_FIFO_WR_DATA_TIMER_MASK);

			sprintf(parsed_wr_data,
				"cmd_type: prod/cons update, prod/cons: 0x%x, update_flag: %s, en_dis_int_for_sb : %s, segment : %s, timer_mask = %d, ",
				prod_cons,
				update_flag ? "update" : "nop",
				en_dis_int_for_sb ?
				(en_dis_int_for_sb == 1 ? "disable" : "nop") :
				"enable",
				segment ? "attn" : "regular",
				timer_mask);
		}
	}
out:
	/* Add parsed element to parsed buffer */
	*results_offset += sprintf(qed_get_buf_ptr(results_buf,
						   *results_offset),
				   "raw: 0x%01x%08x%08x, %s: %d, source : %s, type : %s, cmd_addr : 0x%x(%s%s), %serror: %s\n",
				   element->dword2, element->dword1,
				   element->dword0,
				   is_pf ? "pf" : "vf",
				   GET_FIELD(element->dword0,
					     IGU_FIFO_ELEMENT_DWORD0_FID),
				   s_igu_fifo_source_strs[source],
				   is_wr_cmd ? "wr" : "rd",
				   cmd_addr,
				   (!is_pf && found_addr->vf_desc)
				   ? found_addr->vf_desc
				   : found_addr->desc,
				   parsed_addr_data,
				   parsed_wr_data,
				   s_igu_fifo_error_strs[err_type]);

	return DBG_STATUS_OK;
}

/* Parses an IGU FIFO dump buffer.
 * If result_buf is not NULL, the IGU FIFO results are printed to it.
 * In any case, the required results buffer size is assigned to
 * parsed_results_bytes.
 * The parsing status is returned.
 */
static enum dbg_status qed_parse_igu_fifo_dump(u32 *dump_buf,
					       char *results_buf,
					       u32 *parsed_results_bytes)
{
	const char *section_name, *param_name, *param_str_val;
	u32 param_num_val, num_section_params, num_elements;
	struct igu_fifo_element *elements;
	enum dbg_status status;
	u32 results_offset = 0;
	u8 i;

	/* Read global_params section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "global_params"))
		return DBG_STATUS_IGU_FIFO_BAD_DATA;

	/* Print global params */
	dump_buf += qed_print_section_params(dump_buf,
					     num_section_params,
					     results_buf, &results_offset);

	/* Read igu_fifo_data section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "igu_fifo_data"))
		return DBG_STATUS_IGU_FIFO_BAD_DATA;
	dump_buf += qed_read_param(dump_buf,
				   &param_name, &param_str_val, &param_num_val);
	if (strcmp(param_name, "size"))
		return DBG_STATUS_IGU_FIFO_BAD_DATA;
	if (param_num_val % IGU_FIFO_ELEMENT_DWORDS)
		return DBG_STATUS_IGU_FIFO_BAD_DATA;
	num_elements = param_num_val / IGU_FIFO_ELEMENT_DWORDS;
	elements = (struct igu_fifo_element *)dump_buf;

	/* Decode elements */
	for (i = 0; i < num_elements; i++) {
		status = qed_parse_igu_fifo_element(&elements[i],
						    results_buf,
						    &results_offset);
		if (status != DBG_STATUS_OK)
			return status;
	}

	results_offset += sprintf(qed_get_buf_ptr(results_buf,
						  results_offset),
				  "fifo contained %d elements", num_elements);

	/* Add 1 for string NULL termination */
	*parsed_results_bytes = results_offset + 1;

	return DBG_STATUS_OK;
}

static enum dbg_status
qed_parse_protection_override_dump(u32 *dump_buf,
				   char *results_buf,
				   u32 *parsed_results_bytes)
{
	const char *section_name, *param_name, *param_str_val;
	u32 param_num_val, num_section_params, num_elements;
	struct protection_override_element *elements;
	u32 results_offset = 0;
	u8 i;

	/* Read global_params section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "global_params"))
		return DBG_STATUS_PROTECTION_OVERRIDE_BAD_DATA;

	/* Print global params */
	dump_buf += qed_print_section_params(dump_buf,
					     num_section_params,
					     results_buf, &results_offset);

	/* Read protection_override_data section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "protection_override_data"))
		return DBG_STATUS_PROTECTION_OVERRIDE_BAD_DATA;
	dump_buf += qed_read_param(dump_buf,
				   &param_name, &param_str_val, &param_num_val);
	if (strcmp(param_name, "size"))
		return DBG_STATUS_PROTECTION_OVERRIDE_BAD_DATA;
	if (param_num_val % PROTECTION_OVERRIDE_ELEMENT_DWORDS)
		return DBG_STATUS_PROTECTION_OVERRIDE_BAD_DATA;
	num_elements = param_num_val / PROTECTION_OVERRIDE_ELEMENT_DWORDS;
	elements = (struct protection_override_element *)dump_buf;

	/* Decode elements */
	for (i = 0; i < num_elements; i++) {
		u32 address = GET_FIELD(elements[i].data,
					PROTECTION_OVERRIDE_ELEMENT_ADDRESS) *
			      PROTECTION_OVERRIDE_ELEMENT_ADDR_FACTOR;

		results_offset +=
		    sprintf(qed_get_buf_ptr(results_buf,
					    results_offset),
			    "window %2d, address: 0x%07x, size: %7d regs, read: %d, write: %d, read protection: %-12s, write protection: %-12s\n",
			    i, address,
			    (u32)GET_FIELD(elements[i].data,
				      PROTECTION_OVERRIDE_ELEMENT_WINDOW_SIZE),
			    (u32)GET_FIELD(elements[i].data,
				      PROTECTION_OVERRIDE_ELEMENT_READ),
			    (u32)GET_FIELD(elements[i].data,
				      PROTECTION_OVERRIDE_ELEMENT_WRITE),
			    s_protection_strs[GET_FIELD(elements[i].data,
				PROTECTION_OVERRIDE_ELEMENT_READ_PROTECTION)],
			    s_protection_strs[GET_FIELD(elements[i].data,
				PROTECTION_OVERRIDE_ELEMENT_WRITE_PROTECTION)]);
	}

	results_offset += sprintf(qed_get_buf_ptr(results_buf,
						  results_offset),
				  "protection override contained %d elements",
				  num_elements);

	/* Add 1 for string NULL termination */
	*parsed_results_bytes = results_offset + 1;

	return DBG_STATUS_OK;
}

/* Parses a FW Asserts dump buffer.
 * If result_buf is not NULL, the FW Asserts results are printed to it.
 * In any case, the required results buffer size is assigned to
 * parsed_results_bytes.
 * The parsing status is returned.
 */
static enum dbg_status qed_parse_fw_asserts_dump(u32 *dump_buf,
						 char *results_buf,
						 u32 *parsed_results_bytes)
{
	u32 num_section_params, param_num_val, i, results_offset = 0;
	const char *param_name, *param_str_val, *section_name;
	bool last_section_found = false;

	*parsed_results_bytes = 0;

	/* Read global_params section */
	dump_buf += qed_read_section_hdr(dump_buf,
					 &section_name, &num_section_params);
	if (strcmp(section_name, "global_params"))
		return DBG_STATUS_FW_ASSERTS_PARSE_FAILED;

	/* Print global params */
	dump_buf += qed_print_section_params(dump_buf,
					     num_section_params,
					     results_buf, &results_offset);

	while (!last_section_found) {
		dump_buf += qed_read_section_hdr(dump_buf,
						 &section_name,
						 &num_section_params);
		if (!strcmp(section_name, "fw_asserts")) {
			/* Extract params */
			const char *storm_letter = NULL;
			u32 storm_dump_size = 0;

			for (i = 0; i < num_section_params; i++) {
				dump_buf += qed_read_param(dump_buf,
							   &param_name,
							   &param_str_val,
							   &param_num_val);
				if (!strcmp(param_name, "storm"))
					storm_letter = param_str_val;
				else if (!strcmp(param_name, "size"))
					storm_dump_size = param_num_val;
				else
					return
					    DBG_STATUS_FW_ASSERTS_PARSE_FAILED;
			}

			if (!storm_letter || !storm_dump_size)
				return DBG_STATUS_FW_ASSERTS_PARSE_FAILED;

			/* Print data */
			results_offset +=
			    sprintf(qed_get_buf_ptr(results_buf,
						    results_offset),
				    "\n%sSTORM_ASSERT: size=%d\n",
				    storm_letter, storm_dump_size);
			for (i = 0; i < storm_dump_size; i++, dump_buf++)
				results_offset +=
				    sprintf(qed_get_buf_ptr(results_buf,
							    results_offset),
					    "%08x\n", *dump_buf);
		} else if (!strcmp(section_name, "last")) {
			last_section_found = true;
		} else {
			return DBG_STATUS_FW_ASSERTS_PARSE_FAILED;
		}
	}

	/* Add 1 for string NULL termination */
	*parsed_results_bytes = results_offset + 1;

	return DBG_STATUS_OK;
}

/***************************** Public Functions *******************************/

<<<<<<< HEAD
enum dbg_status qed_dbg_user_set_bin_ptr(const u8 * const bin_ptr)
{
	struct bin_buffer_hdr *buf_array = (struct bin_buffer_hdr *)bin_ptr;
	u8 buf_id;

	/* Convert binary data to debug arrays */
	for (buf_id = 0; buf_id < MAX_BIN_DBG_BUFFER_TYPE; buf_id++) {
		s_user_dbg_arrays[buf_id].ptr =
			(u32 *)(bin_ptr + buf_array[buf_id].offset);
		s_user_dbg_arrays[buf_id].size_in_dwords =
			BYTES_TO_DWORDS(buf_array[buf_id].length);
	}
=======
enum dbg_status qed_dbg_user_set_bin_ptr(struct qed_hwfn *p_hwfn,
					 const u8 * const bin_ptr)
{
	struct bin_buffer_hdr *buf_hdrs = (struct bin_buffer_hdr *)bin_ptr;
	u8 buf_id;

	/* Convert binary data to debug arrays */
	for (buf_id = 0; buf_id < MAX_BIN_DBG_BUFFER_TYPE; buf_id++)
		qed_set_dbg_bin_buf(p_hwfn,
				    (enum bin_dbg_buffer_type)buf_id,
				    (u32 *)(bin_ptr + buf_hdrs[buf_id].offset),
				    buf_hdrs[buf_id].length);

	return DBG_STATUS_OK;
}

enum dbg_status qed_dbg_alloc_user_data(struct qed_hwfn *p_hwfn,
					void **user_data_ptr)
{
	*user_data_ptr = kzalloc(sizeof(struct dbg_tools_user_data),
				 GFP_KERNEL);
	if (!(*user_data_ptr))
		return DBG_STATUS_VIRT_MEM_ALLOC_FAILED;
>>>>>>> upstream/android-13

	return DBG_STATUS_OK;
}

const char *qed_dbg_get_status_str(enum dbg_status status)
{
	return (status <
		MAX_DBG_STATUS) ? s_status_str[status] : "Invalid debug status";
}

enum dbg_status qed_get_idle_chk_results_buf_size(struct qed_hwfn *p_hwfn,
						  u32 *dump_buf,
						  u32 num_dumped_dwords,
						  u32 *results_buf_size)
{
	u32 num_errors, num_warnings;

<<<<<<< HEAD
	return qed_parse_idle_chk_dump(dump_buf,
=======
	return qed_parse_idle_chk_dump(p_hwfn,
				       dump_buf,
>>>>>>> upstream/android-13
				       num_dumped_dwords,
				       NULL,
				       results_buf_size,
				       &num_errors, &num_warnings);
}

enum dbg_status qed_print_idle_chk_results(struct qed_hwfn *p_hwfn,
					   u32 *dump_buf,
					   u32 num_dumped_dwords,
					   char *results_buf,
					   u32 *num_errors,
					   u32 *num_warnings)
{
	u32 parsed_buf_size;

<<<<<<< HEAD
	return qed_parse_idle_chk_dump(dump_buf,
=======
	return qed_parse_idle_chk_dump(p_hwfn,
				       dump_buf,
>>>>>>> upstream/android-13
				       num_dumped_dwords,
				       results_buf,
				       &parsed_buf_size,
				       num_errors, num_warnings);
}

<<<<<<< HEAD
void qed_dbg_mcp_trace_set_meta_data(u32 *data, u32 size)
{
	s_mcp_trace_meta_arr.ptr = data;
	s_mcp_trace_meta_arr.size_in_dwords = size;
=======
void qed_dbg_mcp_trace_set_meta_data(struct qed_hwfn *p_hwfn,
				     const u32 *meta_buf)
{
	struct dbg_tools_user_data *dev_user_data =
		qed_dbg_get_user_data(p_hwfn);

	dev_user_data->mcp_trace_user_meta_buf = meta_buf;
>>>>>>> upstream/android-13
}

enum dbg_status qed_get_mcp_trace_results_buf_size(struct qed_hwfn *p_hwfn,
						   u32 *dump_buf,
						   u32 num_dumped_dwords,
						   u32 *results_buf_size)
{
	return qed_parse_mcp_trace_dump(p_hwfn,
<<<<<<< HEAD
					dump_buf, NULL, results_buf_size);
=======
					dump_buf, NULL, results_buf_size, true);
>>>>>>> upstream/android-13
}

enum dbg_status qed_print_mcp_trace_results(struct qed_hwfn *p_hwfn,
					    u32 *dump_buf,
					    u32 num_dumped_dwords,
					    char *results_buf)
{
	u32 parsed_buf_size;

	return qed_parse_mcp_trace_dump(p_hwfn,
					dump_buf,
<<<<<<< HEAD
					results_buf, &parsed_buf_size);
}

enum dbg_status qed_print_mcp_trace_line(u8 *dump_buf,
					 u32 num_dumped_bytes,
					 char *results_buf)
{
	u32 parsed_bytes;

	return qed_parse_mcp_trace_buf(dump_buf,
				       num_dumped_bytes,
				       0,
				       num_dumped_bytes,
				       results_buf, &parsed_bytes);
=======
					results_buf, &parsed_buf_size, true);
}

enum dbg_status qed_print_mcp_trace_results_cont(struct qed_hwfn *p_hwfn,
						 u32 *dump_buf,
						 char *results_buf)
{
	u32 parsed_buf_size;

	return qed_parse_mcp_trace_dump(p_hwfn, dump_buf, results_buf,
					&parsed_buf_size, false);
}

enum dbg_status qed_print_mcp_trace_line(struct qed_hwfn *p_hwfn,
					 u8 *dump_buf,
					 u32 num_dumped_bytes,
					 char *results_buf)
{
	u32 parsed_results_bytes;

	return qed_parse_mcp_trace_buf(p_hwfn,
				       dump_buf,
				       num_dumped_bytes,
				       0,
				       num_dumped_bytes,
				       results_buf, &parsed_results_bytes);
}

/* Frees the specified MCP Trace meta data */
void qed_mcp_trace_free_meta_data(struct qed_hwfn *p_hwfn)
{
	struct dbg_tools_user_data *dev_user_data;
	struct mcp_trace_meta *meta;
	u32 i;

	dev_user_data = qed_dbg_get_user_data(p_hwfn);
	meta = &dev_user_data->mcp_trace_meta;
	if (!meta->is_allocated)
		return;

	/* Release modules */
	if (meta->modules) {
		for (i = 0; i < meta->modules_num; i++)
			kfree(meta->modules[i]);
		kfree(meta->modules);
	}

	/* Release formats */
	if (meta->formats) {
		for (i = 0; i < meta->formats_num; i++)
			kfree(meta->formats[i].format_str);
		kfree(meta->formats);
	}

	meta->is_allocated = false;
>>>>>>> upstream/android-13
}

enum dbg_status qed_get_reg_fifo_results_buf_size(struct qed_hwfn *p_hwfn,
						  u32 *dump_buf,
						  u32 num_dumped_dwords,
						  u32 *results_buf_size)
{
	return qed_parse_reg_fifo_dump(dump_buf, NULL, results_buf_size);
}

enum dbg_status qed_print_reg_fifo_results(struct qed_hwfn *p_hwfn,
					   u32 *dump_buf,
					   u32 num_dumped_dwords,
					   char *results_buf)
{
	u32 parsed_buf_size;

	return qed_parse_reg_fifo_dump(dump_buf, results_buf, &parsed_buf_size);
}

enum dbg_status qed_get_igu_fifo_results_buf_size(struct qed_hwfn *p_hwfn,
						  u32 *dump_buf,
						  u32 num_dumped_dwords,
						  u32 *results_buf_size)
{
	return qed_parse_igu_fifo_dump(dump_buf, NULL, results_buf_size);
}

enum dbg_status qed_print_igu_fifo_results(struct qed_hwfn *p_hwfn,
					   u32 *dump_buf,
					   u32 num_dumped_dwords,
					   char *results_buf)
{
	u32 parsed_buf_size;

	return qed_parse_igu_fifo_dump(dump_buf, results_buf, &parsed_buf_size);
}

enum dbg_status
qed_get_protection_override_results_buf_size(struct qed_hwfn *p_hwfn,
					     u32 *dump_buf,
					     u32 num_dumped_dwords,
					     u32 *results_buf_size)
{
	return qed_parse_protection_override_dump(dump_buf,
						  NULL, results_buf_size);
}

enum dbg_status qed_print_protection_override_results(struct qed_hwfn *p_hwfn,
						      u32 *dump_buf,
						      u32 num_dumped_dwords,
						      char *results_buf)
{
	u32 parsed_buf_size;

	return qed_parse_protection_override_dump(dump_buf,
						  results_buf,
						  &parsed_buf_size);
}

enum dbg_status qed_get_fw_asserts_results_buf_size(struct qed_hwfn *p_hwfn,
						    u32 *dump_buf,
						    u32 num_dumped_dwords,
						    u32 *results_buf_size)
{
	return qed_parse_fw_asserts_dump(dump_buf, NULL, results_buf_size);
}

enum dbg_status qed_print_fw_asserts_results(struct qed_hwfn *p_hwfn,
					     u32 *dump_buf,
					     u32 num_dumped_dwords,
					     char *results_buf)
{
	u32 parsed_buf_size;

	return qed_parse_fw_asserts_dump(dump_buf,
					 results_buf, &parsed_buf_size);
}

enum dbg_status qed_dbg_parse_attn(struct qed_hwfn *p_hwfn,
				   struct dbg_attn_block_result *results)
{
<<<<<<< HEAD
	struct user_dbg_array *block_attn, *pstrings;
	const u32 *block_attn_name_offsets;
	enum dbg_attn_type attn_type;
	const char *block_name;
	u8 num_regs, i, j;

	num_regs = GET_FIELD(results->data, DBG_ATTN_BLOCK_RESULT_NUM_REGS);
	attn_type = (enum dbg_attn_type)
		    GET_FIELD(results->data,
			      DBG_ATTN_BLOCK_RESULT_ATTN_TYPE);
	block_name = s_block_info_arr[results->block_id].name;

	if (!s_user_dbg_arrays[BIN_BUF_DBG_ATTN_INDEXES].ptr ||
	    !s_user_dbg_arrays[BIN_BUF_DBG_ATTN_NAME_OFFSETS].ptr ||
	    !s_user_dbg_arrays[BIN_BUF_DBG_PARSING_STRINGS].ptr)
		return DBG_STATUS_DBG_ARRAY_NOT_SET;

	block_attn = &s_user_dbg_arrays[BIN_BUF_DBG_ATTN_NAME_OFFSETS];
	block_attn_name_offsets = &block_attn->ptr[results->names_offset];
=======
	const u32 *block_attn_name_offsets;
	const char *attn_name_base;
	const char *block_name;
	enum dbg_attn_type attn_type;
	u8 num_regs, i, j;

	num_regs = GET_FIELD(results->data, DBG_ATTN_BLOCK_RESULT_NUM_REGS);
	attn_type = GET_FIELD(results->data, DBG_ATTN_BLOCK_RESULT_ATTN_TYPE);
	block_name = qed_dbg_get_block_name(p_hwfn, results->block_id);
	if (!block_name)
		return DBG_STATUS_INVALID_ARGS;

	if (!p_hwfn->dbg_arrays[BIN_BUF_DBG_ATTN_INDEXES].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_ATTN_NAME_OFFSETS].ptr ||
	    !p_hwfn->dbg_arrays[BIN_BUF_DBG_PARSING_STRINGS].ptr)
		return DBG_STATUS_DBG_ARRAY_NOT_SET;

	block_attn_name_offsets =
	    (u32 *)p_hwfn->dbg_arrays[BIN_BUF_DBG_ATTN_NAME_OFFSETS].ptr +
	    results->names_offset;

	attn_name_base = p_hwfn->dbg_arrays[BIN_BUF_DBG_PARSING_STRINGS].ptr;
>>>>>>> upstream/android-13

	/* Go over registers with a non-zero attention status */
	for (i = 0; i < num_regs; i++) {
		struct dbg_attn_bit_mapping *bit_mapping;
		struct dbg_attn_reg_result *reg_result;
		u8 num_reg_attn, bit_idx = 0;

		reg_result = &results->reg_results[i];
		num_reg_attn = GET_FIELD(reg_result->data,
					 DBG_ATTN_REG_RESULT_NUM_REG_ATTN);
<<<<<<< HEAD
		block_attn = &s_user_dbg_arrays[BIN_BUF_DBG_ATTN_INDEXES];
		bit_mapping = &((struct dbg_attn_bit_mapping *)
				block_attn->ptr)[reg_result->block_attn_offset];

		pstrings = &s_user_dbg_arrays[BIN_BUF_DBG_PARSING_STRINGS];

		/* Go over attention status bits */
		for (j = 0; j < num_reg_attn; j++) {
			u16 attn_idx_val = GET_FIELD(bit_mapping[j].data,
						     DBG_ATTN_BIT_MAPPING_VAL);
			const char *attn_name, *attn_type_str, *masked_str;
			u32 attn_name_offset, sts_addr;
=======
		bit_mapping = (struct dbg_attn_bit_mapping *)
		    p_hwfn->dbg_arrays[BIN_BUF_DBG_ATTN_INDEXES].ptr +
		    reg_result->block_attn_offset;

		/* Go over attention status bits */
		for (j = 0; j < num_reg_attn; j++, bit_idx++) {
			u16 attn_idx_val = GET_FIELD(bit_mapping[j].data,
						     DBG_ATTN_BIT_MAPPING_VAL);
			const char *attn_name, *attn_type_str, *masked_str;
			u32 attn_name_offset;
			u32 sts_addr;
>>>>>>> upstream/android-13

			/* Check if bit mask should be advanced (due to unused
			 * bits).
			 */
			if (GET_FIELD(bit_mapping[j].data,
				      DBG_ATTN_BIT_MAPPING_IS_UNUSED_BIT_CNT)) {
				bit_idx += (u8)attn_idx_val;
				continue;
			}

			/* Check current bit index */
<<<<<<< HEAD
			if (!(reg_result->sts_val & BIT(bit_idx))) {
				bit_idx++;
				continue;
			}

			/* Find attention name */
			attn_name_offset =
				block_attn_name_offsets[attn_idx_val];
			attn_name = &((const char *)
				      pstrings->ptr)[attn_name_offset];
			attn_type_str = attn_type == ATTN_TYPE_INTERRUPT ?
					"Interrupt" : "Parity";
=======
			if (!(reg_result->sts_val & BIT(bit_idx)))
				continue;

			/* An attention bit with value=1 was found
			 * Find attention name
			 */
			attn_name_offset =
				block_attn_name_offsets[attn_idx_val];
			attn_name = attn_name_base + attn_name_offset;
			attn_type_str =
				(attn_type ==
				 ATTN_TYPE_INTERRUPT ? "Interrupt" :
				 "Parity");
>>>>>>> upstream/android-13
			masked_str = reg_result->mask_val & BIT(bit_idx) ?
				     " [masked]" : "";
			sts_addr = GET_FIELD(reg_result->data,
					     DBG_ATTN_REG_RESULT_STS_ADDRESS);
			DP_NOTICE(p_hwfn,
				  "%s (%s) : %s [address 0x%08x, bit %d]%s\n",
				  block_name, attn_type_str, attn_name,
<<<<<<< HEAD
				  sts_addr, bit_idx, masked_str);

			bit_idx++;
=======
				  sts_addr * 4, bit_idx, masked_str);
>>>>>>> upstream/android-13
		}
	}

	return DBG_STATUS_OK;
}

<<<<<<< HEAD
=======
static DEFINE_MUTEX(qed_dbg_lock);

>>>>>>> upstream/android-13
/* Wrapper for unifying the idle_chk and mcp_trace api */
static enum dbg_status
qed_print_idle_chk_results_wrapper(struct qed_hwfn *p_hwfn,
				   u32 *dump_buf,
				   u32 num_dumped_dwords,
				   char *results_buf)
{
	u32 num_errors, num_warnnings;

	return qed_print_idle_chk_results(p_hwfn, dump_buf, num_dumped_dwords,
					  results_buf, &num_errors,
					  &num_warnnings);
}

/* Feature meta data lookup table */
static struct {
	char *name;
	enum dbg_status (*get_size)(struct qed_hwfn *p_hwfn,
				    struct qed_ptt *p_ptt, u32 *size);
	enum dbg_status (*perform_dump)(struct qed_hwfn *p_hwfn,
					struct qed_ptt *p_ptt, u32 *dump_buf,
					u32 buf_size, u32 *dumped_dwords);
	enum dbg_status (*print_results)(struct qed_hwfn *p_hwfn,
					 u32 *dump_buf, u32 num_dumped_dwords,
					 char *results_buf);
	enum dbg_status (*results_buf_size)(struct qed_hwfn *p_hwfn,
					    u32 *dump_buf,
					    u32 num_dumped_dwords,
					    u32 *results_buf_size);
} qed_features_lookup[] = {
	{
	"grc", qed_dbg_grc_get_dump_buf_size,
		    qed_dbg_grc_dump, NULL, NULL}, {
	"idle_chk",
		    qed_dbg_idle_chk_get_dump_buf_size,
		    qed_dbg_idle_chk_dump,
		    qed_print_idle_chk_results_wrapper,
		    qed_get_idle_chk_results_buf_size}, {
	"mcp_trace",
		    qed_dbg_mcp_trace_get_dump_buf_size,
		    qed_dbg_mcp_trace_dump, qed_print_mcp_trace_results,
		    qed_get_mcp_trace_results_buf_size}, {
	"reg_fifo",
		    qed_dbg_reg_fifo_get_dump_buf_size,
		    qed_dbg_reg_fifo_dump, qed_print_reg_fifo_results,
		    qed_get_reg_fifo_results_buf_size}, {
	"igu_fifo",
		    qed_dbg_igu_fifo_get_dump_buf_size,
		    qed_dbg_igu_fifo_dump, qed_print_igu_fifo_results,
		    qed_get_igu_fifo_results_buf_size}, {
	"protection_override",
		    qed_dbg_protection_override_get_dump_buf_size,
		    qed_dbg_protection_override_dump,
		    qed_print_protection_override_results,
		    qed_get_protection_override_results_buf_size}, {
	"fw_asserts",
		    qed_dbg_fw_asserts_get_dump_buf_size,
		    qed_dbg_fw_asserts_dump,
		    qed_print_fw_asserts_results,
<<<<<<< HEAD
		    qed_get_fw_asserts_results_buf_size},};
=======
		    qed_get_fw_asserts_results_buf_size}, {
	"ilt",
		    qed_dbg_ilt_get_dump_buf_size,
		    qed_dbg_ilt_dump, NULL, NULL},};
>>>>>>> upstream/android-13

static void qed_dbg_print_feature(u8 *p_text_buf, u32 text_size)
{
	u32 i, precision = 80;

	if (!p_text_buf)
		return;

	pr_notice("\n%.*s", precision, p_text_buf);
	for (i = precision; i < text_size; i += precision)
		pr_cont("%.*s", precision, p_text_buf + i);
	pr_cont("\n");
}

#define QED_RESULTS_BUF_MIN_SIZE 16
/* Generic function for decoding debug feature info */
static enum dbg_status format_feature(struct qed_hwfn *p_hwfn,
				      enum qed_dbg_features feature_idx)
{
	struct qed_dbg_feature *feature =
<<<<<<< HEAD
	    &p_hwfn->cdev->dbg_params.features[feature_idx];
=======
	    &p_hwfn->cdev->dbg_features[feature_idx];
>>>>>>> upstream/android-13
	u32 text_size_bytes, null_char_pos, i;
	enum dbg_status rc;
	char *text_buf;

	/* Check if feature supports formatting capability */
	if (!qed_features_lookup[feature_idx].results_buf_size)
		return DBG_STATUS_OK;

	/* Obtain size of formatted output */
	rc = qed_features_lookup[feature_idx].
		results_buf_size(p_hwfn, (u32 *)feature->dump_buf,
				 feature->dumped_dwords, &text_size_bytes);
	if (rc != DBG_STATUS_OK)
		return rc;

	/* Make sure that the allocated size is a multiple of dword (4 bytes) */
	null_char_pos = text_size_bytes - 1;
	text_size_bytes = (text_size_bytes + 3) & ~0x3;

	if (text_size_bytes < QED_RESULTS_BUF_MIN_SIZE) {
		DP_NOTICE(p_hwfn->cdev,
			  "formatted size of feature was too small %d. Aborting\n",
			  text_size_bytes);
		return DBG_STATUS_INVALID_ARGS;
	}

	/* Allocate temp text buf */
	text_buf = vzalloc(text_size_bytes);
	if (!text_buf)
		return DBG_STATUS_VIRT_MEM_ALLOC_FAILED;

	/* Decode feature opcodes to string on temp buf */
	rc = qed_features_lookup[feature_idx].
		print_results(p_hwfn, (u32 *)feature->dump_buf,
			      feature->dumped_dwords, text_buf);
	if (rc != DBG_STATUS_OK) {
		vfree(text_buf);
		return rc;
	}

	/* Replace the original null character with a '\n' character.
	 * The bytes that were added as a result of the dword alignment are also
	 * padded with '\n' characters.
	 */
	for (i = null_char_pos; i < text_size_bytes; i++)
		text_buf[i] = '\n';

	/* Dump printable feature to log */
<<<<<<< HEAD
	if (p_hwfn->cdev->dbg_params.print_data)
		qed_dbg_print_feature(text_buf, text_size_bytes);

=======
	if (p_hwfn->cdev->print_dbg_data)
		qed_dbg_print_feature(text_buf, text_size_bytes);

	/* Just return the original binary buffer if requested */
	if (p_hwfn->cdev->dbg_bin_dump) {
		vfree(text_buf);
		return DBG_STATUS_OK;
	}

>>>>>>> upstream/android-13
	/* Free the old dump_buf and point the dump_buf to the newly allocagted
	 * and formatted text buffer.
	 */
	vfree(feature->dump_buf);
	feature->dump_buf = text_buf;
	feature->buf_size = text_size_bytes;
	feature->dumped_dwords = text_size_bytes / 4;
	return rc;
}

<<<<<<< HEAD
=======
#define MAX_DBG_FEATURE_SIZE_DWORDS	0x3FFFFFFF

>>>>>>> upstream/android-13
/* Generic function for performing the dump of a debug feature. */
static enum dbg_status qed_dbg_dump(struct qed_hwfn *p_hwfn,
				    struct qed_ptt *p_ptt,
				    enum qed_dbg_features feature_idx)
{
	struct qed_dbg_feature *feature =
<<<<<<< HEAD
	    &p_hwfn->cdev->dbg_params.features[feature_idx];
=======
	    &p_hwfn->cdev->dbg_features[feature_idx];
>>>>>>> upstream/android-13
	u32 buf_size_dwords;
	enum dbg_status rc;

	DP_NOTICE(p_hwfn->cdev, "Collecting a debug feature [\"%s\"]\n",
		  qed_features_lookup[feature_idx].name);

	/* Dump_buf was already allocated need to free (this can happen if dump
	 * was called but file was never read).
	 * We can't use the buffer as is since size may have changed.
	 */
	if (feature->dump_buf) {
		vfree(feature->dump_buf);
		feature->dump_buf = NULL;
	}

	/* Get buffer size from hsi, allocate accordingly, and perform the
	 * dump.
	 */
	rc = qed_features_lookup[feature_idx].get_size(p_hwfn, p_ptt,
						       &buf_size_dwords);
	if (rc != DBG_STATUS_OK && rc != DBG_STATUS_NVRAM_GET_IMAGE_FAILED)
		return rc;
<<<<<<< HEAD
=======

	if (buf_size_dwords > MAX_DBG_FEATURE_SIZE_DWORDS) {
		feature->buf_size = 0;
		DP_NOTICE(p_hwfn->cdev,
			  "Debug feature [\"%s\"] size (0x%x dwords) exceeds maximum size (0x%x dwords)\n",
			  qed_features_lookup[feature_idx].name,
			  buf_size_dwords, MAX_DBG_FEATURE_SIZE_DWORDS);

		return DBG_STATUS_OK;
	}

>>>>>>> upstream/android-13
	feature->buf_size = buf_size_dwords * sizeof(u32);
	feature->dump_buf = vmalloc(feature->buf_size);
	if (!feature->dump_buf)
		return DBG_STATUS_VIRT_MEM_ALLOC_FAILED;

	rc = qed_features_lookup[feature_idx].
		perform_dump(p_hwfn, p_ptt, (u32 *)feature->dump_buf,
			     feature->buf_size / sizeof(u32),
			     &feature->dumped_dwords);

	/* If mcp is stuck we get DBG_STATUS_NVRAM_GET_IMAGE_FAILED error.
	 * In this case the buffer holds valid binary data, but we wont able
	 * to parse it (since parsing relies on data in NVRAM which is only
	 * accessible when MFW is responsive). skip the formatting but return
	 * success so that binary data is provided.
	 */
	if (rc == DBG_STATUS_NVRAM_GET_IMAGE_FAILED)
		return DBG_STATUS_OK;

	if (rc != DBG_STATUS_OK)
		return rc;

	/* Format output */
	rc = format_feature(p_hwfn, feature_idx);
	return rc;
}

int qed_dbg_grc(struct qed_dev *cdev, void *buffer, u32 *num_dumped_bytes)
{
	return qed_dbg_feature(cdev, buffer, DBG_FEATURE_GRC, num_dumped_bytes);
}

int qed_dbg_grc_size(struct qed_dev *cdev)
{
	return qed_dbg_feature_size(cdev, DBG_FEATURE_GRC);
}

int qed_dbg_idle_chk(struct qed_dev *cdev, void *buffer, u32 *num_dumped_bytes)
{
	return qed_dbg_feature(cdev, buffer, DBG_FEATURE_IDLE_CHK,
			       num_dumped_bytes);
}

int qed_dbg_idle_chk_size(struct qed_dev *cdev)
{
	return qed_dbg_feature_size(cdev, DBG_FEATURE_IDLE_CHK);
}

int qed_dbg_reg_fifo(struct qed_dev *cdev, void *buffer, u32 *num_dumped_bytes)
{
	return qed_dbg_feature(cdev, buffer, DBG_FEATURE_REG_FIFO,
			       num_dumped_bytes);
}

int qed_dbg_reg_fifo_size(struct qed_dev *cdev)
{
	return qed_dbg_feature_size(cdev, DBG_FEATURE_REG_FIFO);
}

int qed_dbg_igu_fifo(struct qed_dev *cdev, void *buffer, u32 *num_dumped_bytes)
{
	return qed_dbg_feature(cdev, buffer, DBG_FEATURE_IGU_FIFO,
			       num_dumped_bytes);
}

int qed_dbg_igu_fifo_size(struct qed_dev *cdev)
{
	return qed_dbg_feature_size(cdev, DBG_FEATURE_IGU_FIFO);
}

static int qed_dbg_nvm_image_length(struct qed_hwfn *p_hwfn,
				    enum qed_nvm_images image_id, u32 *length)
{
	struct qed_nvm_image_att image_att;
	int rc;

	*length = 0;
	rc = qed_mcp_get_nvm_image_att(p_hwfn, image_id, &image_att);
	if (rc)
		return rc;

	*length = image_att.length;

	return rc;
}

static int qed_dbg_nvm_image(struct qed_dev *cdev, void *buffer,
			     u32 *num_dumped_bytes,
			     enum qed_nvm_images image_id)
{
	struct qed_hwfn *p_hwfn =
<<<<<<< HEAD
		&cdev->hwfns[cdev->dbg_params.engine_for_debug];
	u32 len_rounded, i;
	__be32 val;
=======
		&cdev->hwfns[cdev->engine_for_debug];
	u32 len_rounded;
>>>>>>> upstream/android-13
	int rc;

	*num_dumped_bytes = 0;
	rc = qed_dbg_nvm_image_length(p_hwfn, image_id, &len_rounded);
	if (rc)
		return rc;

	DP_NOTICE(p_hwfn->cdev,
		  "Collecting a debug feature [\"nvram image %d\"]\n",
		  image_id);

	len_rounded = roundup(len_rounded, sizeof(u32));
	rc = qed_mcp_get_nvm_image(p_hwfn, image_id, buffer, len_rounded);
	if (rc)
		return rc;

	/* QED_NVM_IMAGE_NVM_META image is not swapped like other images */
	if (image_id != QED_NVM_IMAGE_NVM_META)
<<<<<<< HEAD
		for (i = 0; i < len_rounded; i += 4) {
			val = cpu_to_be32(*(u32 *)(buffer + i));
			*(u32 *)(buffer + i) = val;
		}
=======
		cpu_to_be32_array((__force __be32 *)buffer,
				  (const u32 *)buffer,
				  len_rounded / sizeof(u32));
>>>>>>> upstream/android-13

	*num_dumped_bytes = len_rounded;

	return rc;
}

int qed_dbg_protection_override(struct qed_dev *cdev, void *buffer,
				u32 *num_dumped_bytes)
{
	return qed_dbg_feature(cdev, buffer, DBG_FEATURE_PROTECTION_OVERRIDE,
			       num_dumped_bytes);
}

int qed_dbg_protection_override_size(struct qed_dev *cdev)
{
	return qed_dbg_feature_size(cdev, DBG_FEATURE_PROTECTION_OVERRIDE);
}

int qed_dbg_fw_asserts(struct qed_dev *cdev, void *buffer,
		       u32 *num_dumped_bytes)
{
	return qed_dbg_feature(cdev, buffer, DBG_FEATURE_FW_ASSERTS,
			       num_dumped_bytes);
}

int qed_dbg_fw_asserts_size(struct qed_dev *cdev)
{
	return qed_dbg_feature_size(cdev, DBG_FEATURE_FW_ASSERTS);
}

<<<<<<< HEAD
=======
int qed_dbg_ilt(struct qed_dev *cdev, void *buffer, u32 *num_dumped_bytes)
{
	return qed_dbg_feature(cdev, buffer, DBG_FEATURE_ILT, num_dumped_bytes);
}

int qed_dbg_ilt_size(struct qed_dev *cdev)
{
	return qed_dbg_feature_size(cdev, DBG_FEATURE_ILT);
}

>>>>>>> upstream/android-13
int qed_dbg_mcp_trace(struct qed_dev *cdev, void *buffer,
		      u32 *num_dumped_bytes)
{
	return qed_dbg_feature(cdev, buffer, DBG_FEATURE_MCP_TRACE,
			       num_dumped_bytes);
}

int qed_dbg_mcp_trace_size(struct qed_dev *cdev)
{
	return qed_dbg_feature_size(cdev, DBG_FEATURE_MCP_TRACE);
}

/* Defines the amount of bytes allocated for recording the length of debugfs
 * feature buffer.
 */
#define REGDUMP_HEADER_SIZE			sizeof(u32)
<<<<<<< HEAD
#define REGDUMP_HEADER_FEATURE_SHIFT		24
#define REGDUMP_HEADER_ENGINE_SHIFT		31
#define REGDUMP_HEADER_OMIT_ENGINE_SHIFT	30
=======
#define REGDUMP_HEADER_SIZE_SHIFT		0
#define REGDUMP_HEADER_SIZE_MASK		0xffffff
#define REGDUMP_HEADER_FEATURE_SHIFT		24
#define REGDUMP_HEADER_FEATURE_MASK		0x1f
#define REGDUMP_HEADER_BIN_DUMP_SHIFT		29
#define REGDUMP_HEADER_BIN_DUMP_MASK		0x1
#define REGDUMP_HEADER_OMIT_ENGINE_SHIFT	30
#define REGDUMP_HEADER_OMIT_ENGINE_MASK		0x1
#define REGDUMP_HEADER_ENGINE_SHIFT		31
#define REGDUMP_HEADER_ENGINE_MASK		0x1
#define REGDUMP_MAX_SIZE			0x1000000
#define ILT_DUMP_MAX_SIZE			(1024 * 1024 * 15)

>>>>>>> upstream/android-13
enum debug_print_features {
	OLD_MODE = 0,
	IDLE_CHK = 1,
	GRC_DUMP = 2,
	MCP_TRACE = 3,
	REG_FIFO = 4,
	PROTECTION_OVERRIDE = 5,
	IGU_FIFO = 6,
	PHY = 7,
	FW_ASSERTS = 8,
	NVM_CFG1 = 9,
	DEFAULT_CFG = 10,
	NVM_META = 11,
<<<<<<< HEAD
};

static u32 qed_calc_regdump_header(enum debug_print_features feature,
				   int engine, u32 feature_size, u8 omit_engine)
{
	/* Insert the engine, feature and mode inside the header and combine it
	 * with feature size.
	 */
	return feature_size | (feature << REGDUMP_HEADER_FEATURE_SHIFT) |
	       (omit_engine << REGDUMP_HEADER_OMIT_ENGINE_SHIFT) |
	       (engine << REGDUMP_HEADER_ENGINE_SHIFT);
=======
	MDUMP = 12,
	ILT_DUMP = 13,
};

static u32 qed_calc_regdump_header(struct qed_dev *cdev,
				   enum debug_print_features feature,
				   int engine, u32 feature_size, u8 omit_engine)
{
	u32 res = 0;

	SET_FIELD(res, REGDUMP_HEADER_SIZE, feature_size);
	if (res != feature_size)
		DP_NOTICE(cdev,
			  "Feature %d is too large (size 0x%x) and will corrupt the dump\n",
			  feature, feature_size);

	SET_FIELD(res, REGDUMP_HEADER_FEATURE, feature);
	SET_FIELD(res, REGDUMP_HEADER_BIN_DUMP, 1);
	SET_FIELD(res, REGDUMP_HEADER_OMIT_ENGINE, omit_engine);
	SET_FIELD(res, REGDUMP_HEADER_ENGINE, engine);

	return res;
>>>>>>> upstream/android-13
}

int qed_dbg_all_data(struct qed_dev *cdev, void *buffer)
{
	u8 cur_engine, omit_engine = 0, org_engine;
<<<<<<< HEAD
	u32 offset = 0, feature_size;
	int rc;

	if (cdev->num_hwfns == 1)
		omit_engine = 1;

=======
	struct qed_hwfn *p_hwfn =
		&cdev->hwfns[cdev->engine_for_debug];
	struct dbg_tools_data *dev_data = &p_hwfn->dbg_info;
	int grc_params[MAX_DBG_GRC_PARAMS], i;
	u32 offset = 0, feature_size;
	int rc;

	for (i = 0; i < MAX_DBG_GRC_PARAMS; i++)
		grc_params[i] = dev_data->grc.param_val[i];

	if (!QED_IS_CMT(cdev))
		omit_engine = 1;

	mutex_lock(&qed_dbg_lock);
	cdev->dbg_bin_dump = true;

>>>>>>> upstream/android-13
	org_engine = qed_get_debug_engine(cdev);
	for (cur_engine = 0; cur_engine < cdev->num_hwfns; cur_engine++) {
		/* Collect idle_chks and grcDump for each hw function */
		DP_VERBOSE(cdev, QED_MSG_DEBUG,
			   "obtaining idle_chk and grcdump for current engine\n");
		qed_set_debug_engine(cdev, cur_engine);

		/* First idle_chk */
		rc = qed_dbg_idle_chk(cdev, (u8 *)buffer + offset +
				      REGDUMP_HEADER_SIZE, &feature_size);
		if (!rc) {
			*(u32 *)((u8 *)buffer + offset) =
<<<<<<< HEAD
			    qed_calc_regdump_header(IDLE_CHK, cur_engine,
=======
			    qed_calc_regdump_header(cdev, IDLE_CHK, cur_engine,
>>>>>>> upstream/android-13
						    feature_size, omit_engine);
			offset += (feature_size + REGDUMP_HEADER_SIZE);
		} else {
			DP_ERR(cdev, "qed_dbg_idle_chk failed. rc = %d\n", rc);
		}

		/* Second idle_chk */
		rc = qed_dbg_idle_chk(cdev, (u8 *)buffer + offset +
				      REGDUMP_HEADER_SIZE, &feature_size);
		if (!rc) {
			*(u32 *)((u8 *)buffer + offset) =
<<<<<<< HEAD
			    qed_calc_regdump_header(IDLE_CHK, cur_engine,
=======
			    qed_calc_regdump_header(cdev, IDLE_CHK, cur_engine,
>>>>>>> upstream/android-13
						    feature_size, omit_engine);
			offset += (feature_size + REGDUMP_HEADER_SIZE);
		} else {
			DP_ERR(cdev, "qed_dbg_idle_chk failed. rc = %d\n", rc);
		}

		/* reg_fifo dump */
		rc = qed_dbg_reg_fifo(cdev, (u8 *)buffer + offset +
				      REGDUMP_HEADER_SIZE, &feature_size);
		if (!rc) {
			*(u32 *)((u8 *)buffer + offset) =
<<<<<<< HEAD
			    qed_calc_regdump_header(REG_FIFO, cur_engine,
=======
			    qed_calc_regdump_header(cdev, REG_FIFO, cur_engine,
>>>>>>> upstream/android-13
						    feature_size, omit_engine);
			offset += (feature_size + REGDUMP_HEADER_SIZE);
		} else {
			DP_ERR(cdev, "qed_dbg_reg_fifo failed. rc = %d\n", rc);
		}

		/* igu_fifo dump */
		rc = qed_dbg_igu_fifo(cdev, (u8 *)buffer + offset +
				      REGDUMP_HEADER_SIZE, &feature_size);
		if (!rc) {
			*(u32 *)((u8 *)buffer + offset) =
<<<<<<< HEAD
			    qed_calc_regdump_header(IGU_FIFO, cur_engine,
=======
			    qed_calc_regdump_header(cdev, IGU_FIFO, cur_engine,
>>>>>>> upstream/android-13
						    feature_size, omit_engine);
			offset += (feature_size + REGDUMP_HEADER_SIZE);
		} else {
			DP_ERR(cdev, "qed_dbg_igu_fifo failed. rc = %d", rc);
		}

		/* protection_override dump */
		rc = qed_dbg_protection_override(cdev, (u8 *)buffer + offset +
						 REGDUMP_HEADER_SIZE,
						 &feature_size);
		if (!rc) {
			*(u32 *)((u8 *)buffer + offset) =
<<<<<<< HEAD
			    qed_calc_regdump_header(PROTECTION_OVERRIDE,
=======
			    qed_calc_regdump_header(cdev, PROTECTION_OVERRIDE,
>>>>>>> upstream/android-13
						    cur_engine,
						    feature_size, omit_engine);
			offset += (feature_size + REGDUMP_HEADER_SIZE);
		} else {
			DP_ERR(cdev,
			       "qed_dbg_protection_override failed. rc = %d\n",
			       rc);
		}

		/* fw_asserts dump */
		rc = qed_dbg_fw_asserts(cdev, (u8 *)buffer + offset +
					REGDUMP_HEADER_SIZE, &feature_size);
		if (!rc) {
			*(u32 *)((u8 *)buffer + offset) =
<<<<<<< HEAD
			    qed_calc_regdump_header(FW_ASSERTS, cur_engine,
						    feature_size, omit_engine);
=======
			    qed_calc_regdump_header(cdev, FW_ASSERTS,
						    cur_engine, feature_size,
						    omit_engine);
>>>>>>> upstream/android-13
			offset += (feature_size + REGDUMP_HEADER_SIZE);
		} else {
			DP_ERR(cdev, "qed_dbg_fw_asserts failed. rc = %d\n",
			       rc);
		}

<<<<<<< HEAD
		/* GRC dump - must be last because when mcp stuck it will
		 * clutter idle_chk, reg_fifo, ...
		 */
=======
		feature_size = qed_dbg_ilt_size(cdev);
		if (!cdev->disable_ilt_dump &&
		    feature_size < ILT_DUMP_MAX_SIZE) {
			rc = qed_dbg_ilt(cdev, (u8 *)buffer + offset +
					 REGDUMP_HEADER_SIZE, &feature_size);
			if (!rc) {
				*(u32 *)((u8 *)buffer + offset) =
				    qed_calc_regdump_header(cdev, ILT_DUMP,
							    cur_engine,
							    feature_size,
							    omit_engine);
				offset += feature_size + REGDUMP_HEADER_SIZE;
			} else {
				DP_ERR(cdev, "qed_dbg_ilt failed. rc = %d\n",
				       rc);
			}
		}

		/* GRC dump - must be last because when mcp stuck it will
		 * clutter idle_chk, reg_fifo, ...
		 */
		for (i = 0; i < MAX_DBG_GRC_PARAMS; i++)
			dev_data->grc.param_val[i] = grc_params[i];

>>>>>>> upstream/android-13
		rc = qed_dbg_grc(cdev, (u8 *)buffer + offset +
				 REGDUMP_HEADER_SIZE, &feature_size);
		if (!rc) {
			*(u32 *)((u8 *)buffer + offset) =
<<<<<<< HEAD
			    qed_calc_regdump_header(GRC_DUMP, cur_engine,
=======
			    qed_calc_regdump_header(cdev, GRC_DUMP,
						    cur_engine,
>>>>>>> upstream/android-13
						    feature_size, omit_engine);
			offset += (feature_size + REGDUMP_HEADER_SIZE);
		} else {
			DP_ERR(cdev, "qed_dbg_grc failed. rc = %d", rc);
		}
	}

	qed_set_debug_engine(cdev, org_engine);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	/* mcp_trace */
	rc = qed_dbg_mcp_trace(cdev, (u8 *)buffer + offset +
			       REGDUMP_HEADER_SIZE, &feature_size);
	if (!rc) {
		*(u32 *)((u8 *)buffer + offset) =
<<<<<<< HEAD
		    qed_calc_regdump_header(MCP_TRACE, cur_engine,
=======
		    qed_calc_regdump_header(cdev, MCP_TRACE, cur_engine,
>>>>>>> upstream/android-13
					    feature_size, omit_engine);
		offset += (feature_size + REGDUMP_HEADER_SIZE);
	} else {
		DP_ERR(cdev, "qed_dbg_mcp_trace failed. rc = %d\n", rc);
	}

<<<<<<< HEAD
	/* nvm cfg1 */
	rc = qed_dbg_nvm_image(cdev,
			       (u8 *)buffer + offset + REGDUMP_HEADER_SIZE,
			       &feature_size, QED_NVM_IMAGE_NVM_CFG1);
	if (!rc) {
		*(u32 *)((u8 *)buffer + offset) =
		    qed_calc_regdump_header(NVM_CFG1, cur_engine,
=======
	/* Re-populate nvm attribute info */
	qed_mcp_nvm_info_free(p_hwfn);
	qed_mcp_nvm_info_populate(p_hwfn);

	/* nvm cfg1 */
	rc = qed_dbg_nvm_image(cdev,
			       (u8 *)buffer + offset +
			       REGDUMP_HEADER_SIZE, &feature_size,
			       QED_NVM_IMAGE_NVM_CFG1);
	if (!rc) {
		*(u32 *)((u8 *)buffer + offset) =
		    qed_calc_regdump_header(cdev, NVM_CFG1, cur_engine,
>>>>>>> upstream/android-13
					    feature_size, omit_engine);
		offset += (feature_size + REGDUMP_HEADER_SIZE);
	} else if (rc != -ENOENT) {
		DP_ERR(cdev,
		       "qed_dbg_nvm_image failed for image  %d (%s), rc = %d\n",
		       QED_NVM_IMAGE_NVM_CFG1, "QED_NVM_IMAGE_NVM_CFG1", rc);
	}

	/* nvm default */
	rc = qed_dbg_nvm_image(cdev,
			       (u8 *)buffer + offset + REGDUMP_HEADER_SIZE,
			       &feature_size, QED_NVM_IMAGE_DEFAULT_CFG);
	if (!rc) {
		*(u32 *)((u8 *)buffer + offset) =
<<<<<<< HEAD
		    qed_calc_regdump_header(DEFAULT_CFG, cur_engine,
=======
		    qed_calc_regdump_header(cdev, DEFAULT_CFG, cur_engine,
>>>>>>> upstream/android-13
					    feature_size, omit_engine);
		offset += (feature_size + REGDUMP_HEADER_SIZE);
	} else if (rc != -ENOENT) {
		DP_ERR(cdev,
		       "qed_dbg_nvm_image failed for image %d (%s), rc = %d\n",
		       QED_NVM_IMAGE_DEFAULT_CFG, "QED_NVM_IMAGE_DEFAULT_CFG",
		       rc);
	}

	/* nvm meta */
	rc = qed_dbg_nvm_image(cdev,
			       (u8 *)buffer + offset + REGDUMP_HEADER_SIZE,
			       &feature_size, QED_NVM_IMAGE_NVM_META);
	if (!rc) {
		*(u32 *)((u8 *)buffer + offset) =
<<<<<<< HEAD
		    qed_calc_regdump_header(NVM_META, cur_engine,
					    feature_size, omit_engine);
=======
			qed_calc_regdump_header(cdev, NVM_META, cur_engine,
						feature_size, omit_engine);
>>>>>>> upstream/android-13
		offset += (feature_size + REGDUMP_HEADER_SIZE);
	} else if (rc != -ENOENT) {
		DP_ERR(cdev,
		       "qed_dbg_nvm_image failed for image %d (%s), rc = %d\n",
		       QED_NVM_IMAGE_NVM_META, "QED_NVM_IMAGE_NVM_META", rc);
	}

<<<<<<< HEAD
=======
	/* nvm mdump */
	rc = qed_dbg_nvm_image(cdev, (u8 *)buffer + offset +
			       REGDUMP_HEADER_SIZE, &feature_size,
			       QED_NVM_IMAGE_MDUMP);
	if (!rc) {
		*(u32 *)((u8 *)buffer + offset) =
			qed_calc_regdump_header(cdev, MDUMP, cur_engine,
						feature_size, omit_engine);
		offset += (feature_size + REGDUMP_HEADER_SIZE);
	} else if (rc != -ENOENT) {
		DP_ERR(cdev,
		       "qed_dbg_nvm_image failed for image %d (%s), rc = %d\n",
		       QED_NVM_IMAGE_MDUMP, "QED_NVM_IMAGE_MDUMP", rc);
	}

	cdev->dbg_bin_dump = false;
	mutex_unlock(&qed_dbg_lock);

>>>>>>> upstream/android-13
	return 0;
}

int qed_dbg_all_data_size(struct qed_dev *cdev)
{
	struct qed_hwfn *p_hwfn =
<<<<<<< HEAD
		&cdev->hwfns[cdev->dbg_params.engine_for_debug];
	u32 regs_len = 0, image_len = 0;
	u8 cur_engine, org_engine;

=======
		&cdev->hwfns[cdev->engine_for_debug];
	u32 regs_len = 0, image_len = 0, ilt_len = 0, total_ilt_len = 0;
	u8 cur_engine, org_engine;

	cdev->disable_ilt_dump = false;
>>>>>>> upstream/android-13
	org_engine = qed_get_debug_engine(cdev);
	for (cur_engine = 0; cur_engine < cdev->num_hwfns; cur_engine++) {
		/* Engine specific */
		DP_VERBOSE(cdev, QED_MSG_DEBUG,
			   "calculating idle_chk and grcdump register length for current engine\n");
		qed_set_debug_engine(cdev, cur_engine);
		regs_len += REGDUMP_HEADER_SIZE + qed_dbg_idle_chk_size(cdev) +
			    REGDUMP_HEADER_SIZE + qed_dbg_idle_chk_size(cdev) +
			    REGDUMP_HEADER_SIZE + qed_dbg_grc_size(cdev) +
			    REGDUMP_HEADER_SIZE + qed_dbg_reg_fifo_size(cdev) +
			    REGDUMP_HEADER_SIZE + qed_dbg_igu_fifo_size(cdev) +
			    REGDUMP_HEADER_SIZE +
			    qed_dbg_protection_override_size(cdev) +
			    REGDUMP_HEADER_SIZE + qed_dbg_fw_asserts_size(cdev);
<<<<<<< HEAD
=======

		ilt_len = REGDUMP_HEADER_SIZE + qed_dbg_ilt_size(cdev);
		if (ilt_len < ILT_DUMP_MAX_SIZE) {
			total_ilt_len += ilt_len;
			regs_len += ilt_len;
		}
>>>>>>> upstream/android-13
	}

	qed_set_debug_engine(cdev, org_engine);

	/* Engine common */
	regs_len += REGDUMP_HEADER_SIZE + qed_dbg_mcp_trace_size(cdev);
	qed_dbg_nvm_image_length(p_hwfn, QED_NVM_IMAGE_NVM_CFG1, &image_len);
	if (image_len)
		regs_len += REGDUMP_HEADER_SIZE + image_len;
	qed_dbg_nvm_image_length(p_hwfn, QED_NVM_IMAGE_DEFAULT_CFG, &image_len);
	if (image_len)
		regs_len += REGDUMP_HEADER_SIZE + image_len;
	qed_dbg_nvm_image_length(p_hwfn, QED_NVM_IMAGE_NVM_META, &image_len);
	if (image_len)
		regs_len += REGDUMP_HEADER_SIZE + image_len;
<<<<<<< HEAD
=======
	qed_dbg_nvm_image_length(p_hwfn, QED_NVM_IMAGE_MDUMP, &image_len);
	if (image_len)
		regs_len += REGDUMP_HEADER_SIZE + image_len;

	if (regs_len > REGDUMP_MAX_SIZE) {
		DP_VERBOSE(cdev, QED_MSG_DEBUG,
			   "Dump exceeds max size 0x%x, disable ILT dump\n",
			   REGDUMP_MAX_SIZE);
		cdev->disable_ilt_dump = true;
		regs_len -= total_ilt_len;
	}
>>>>>>> upstream/android-13

	return regs_len;
}

int qed_dbg_feature(struct qed_dev *cdev, void *buffer,
		    enum qed_dbg_features feature, u32 *num_dumped_bytes)
{
	struct qed_hwfn *p_hwfn =
<<<<<<< HEAD
		&cdev->hwfns[cdev->dbg_params.engine_for_debug];
	struct qed_dbg_feature *qed_feature =
		&cdev->dbg_params.features[feature];
=======
		&cdev->hwfns[cdev->engine_for_debug];
	struct qed_dbg_feature *qed_feature =
		&cdev->dbg_features[feature];
>>>>>>> upstream/android-13
	enum dbg_status dbg_rc;
	struct qed_ptt *p_ptt;
	int rc = 0;

	/* Acquire ptt */
	p_ptt = qed_ptt_acquire(p_hwfn);
	if (!p_ptt)
		return -EINVAL;

	/* Get dump */
	dbg_rc = qed_dbg_dump(p_hwfn, p_ptt, feature);
	if (dbg_rc != DBG_STATUS_OK) {
		DP_VERBOSE(cdev, QED_MSG_DEBUG, "%s\n",
			   qed_dbg_get_status_str(dbg_rc));
		*num_dumped_bytes = 0;
		rc = -EINVAL;
		goto out;
	}

	DP_VERBOSE(cdev, QED_MSG_DEBUG,
		   "copying debugfs feature to external buffer\n");
	memcpy(buffer, qed_feature->dump_buf, qed_feature->buf_size);
<<<<<<< HEAD
	*num_dumped_bytes = cdev->dbg_params.features[feature].dumped_dwords *
=======
	*num_dumped_bytes = cdev->dbg_features[feature].dumped_dwords *
>>>>>>> upstream/android-13
			    4;

out:
	qed_ptt_release(p_hwfn, p_ptt);
	return rc;
}

int qed_dbg_feature_size(struct qed_dev *cdev, enum qed_dbg_features feature)
{
	struct qed_hwfn *p_hwfn =
<<<<<<< HEAD
		&cdev->hwfns[cdev->dbg_params.engine_for_debug];
	struct qed_ptt *p_ptt = qed_ptt_acquire(p_hwfn);
	struct qed_dbg_feature *qed_feature =
		&cdev->dbg_params.features[feature];
=======
		&cdev->hwfns[cdev->engine_for_debug];
	struct qed_dbg_feature *qed_feature = &cdev->dbg_features[feature];
	struct qed_ptt *p_ptt = qed_ptt_acquire(p_hwfn);
>>>>>>> upstream/android-13
	u32 buf_size_dwords;
	enum dbg_status rc;

	if (!p_ptt)
		return -EINVAL;

	rc = qed_features_lookup[feature].get_size(p_hwfn, p_ptt,
						   &buf_size_dwords);
	if (rc != DBG_STATUS_OK)
		buf_size_dwords = 0;

<<<<<<< HEAD
=======
	/* Feature will not be dumped if it exceeds maximum size */
	if (buf_size_dwords > MAX_DBG_FEATURE_SIZE_DWORDS)
		buf_size_dwords = 0;

>>>>>>> upstream/android-13
	qed_ptt_release(p_hwfn, p_ptt);
	qed_feature->buf_size = buf_size_dwords * sizeof(u32);
	return qed_feature->buf_size;
}

u8 qed_get_debug_engine(struct qed_dev *cdev)
{
<<<<<<< HEAD
	return cdev->dbg_params.engine_for_debug;
=======
	return cdev->engine_for_debug;
>>>>>>> upstream/android-13
}

void qed_set_debug_engine(struct qed_dev *cdev, int engine_number)
{
	DP_VERBOSE(cdev, QED_MSG_DEBUG, "set debug engine to %d\n",
		   engine_number);
<<<<<<< HEAD
	cdev->dbg_params.engine_for_debug = engine_number;
=======
	cdev->engine_for_debug = engine_number;
>>>>>>> upstream/android-13
}

void qed_dbg_pf_init(struct qed_dev *cdev)
{
<<<<<<< HEAD
	const u8 *dbg_values;
=======
	const u8 *dbg_values = NULL;
	int i;
>>>>>>> upstream/android-13

	/* Debug values are after init values.
	 * The offset is the first dword of the file.
	 */
	dbg_values = cdev->firmware->data + *(u32 *)cdev->firmware->data;
<<<<<<< HEAD
	qed_dbg_set_bin_ptr((u8 *)dbg_values);
	qed_dbg_user_set_bin_ptr((u8 *)dbg_values);
=======

	for_each_hwfn(cdev, i) {
		qed_dbg_set_bin_ptr(&cdev->hwfns[i], dbg_values);
		qed_dbg_user_set_bin_ptr(&cdev->hwfns[i], dbg_values);
	}

	/* Set the hwfn to be 0 as default */
	cdev->engine_for_debug = 0;
>>>>>>> upstream/android-13
}

void qed_dbg_pf_exit(struct qed_dev *cdev)
{
	struct qed_dbg_feature *feature = NULL;
	enum qed_dbg_features feature_idx;

<<<<<<< HEAD
	/* Debug features' buffers may be allocated if debug feature was used
	 * but dump wasn't called.
	 */
	for (feature_idx = 0; feature_idx < DBG_FEATURE_NUM; feature_idx++) {
		feature = &cdev->dbg_params.features[feature_idx];
=======
	/* debug features' buffers may be allocated if debug feature was used
	 * but dump wasn't called
	 */
	for (feature_idx = 0; feature_idx < DBG_FEATURE_NUM; feature_idx++) {
		feature = &cdev->dbg_features[feature_idx];
>>>>>>> upstream/android-13
		if (feature->dump_buf) {
			vfree(feature->dump_buf);
			feature->dump_buf = NULL;
		}
	}
}
