

#ifndef __LINUX_USB_PD_H
#define __LINUX_USB_PD_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/usb/typec.h>


enum pd_ctrl_msg_type {
	
	PD_CTRL_GOOD_CRC = 1,
	PD_CTRL_GOTO_MIN = 2,
	PD_CTRL_ACCEPT = 3,
	PD_CTRL_REJECT = 4,
	PD_CTRL_PING = 5,
	PD_CTRL_PS_RDY = 6,
	PD_CTRL_GET_SOURCE_CAP = 7,
	PD_CTRL_GET_SINK_CAP = 8,
	PD_CTRL_DR_SWAP = 9,
	PD_CTRL_PR_SWAP = 10,
	PD_CTRL_VCONN_SWAP = 11,
	PD_CTRL_WAIT = 12,
	PD_CTRL_SOFT_RESET = 13,
	
	PD_CTRL_NOT_SUPP = 16,
	PD_CTRL_GET_SOURCE_CAP_EXT = 17,
	PD_CTRL_GET_STATUS = 18,
	PD_CTRL_FR_SWAP = 19,
	PD_CTRL_GET_PPS_STATUS = 20,
	PD_CTRL_GET_COUNTRY_CODES = 21,
	
};

enum pd_data_msg_type {
	
	PD_DATA_SOURCE_CAP = 1,
	PD_DATA_REQUEST = 2,
	PD_DATA_BIST = 3,
	PD_DATA_SINK_CAP = 4,
	PD_DATA_BATT_STATUS = 5,
	PD_DATA_ALERT = 6,
	PD_DATA_GET_COUNTRY_INFO = 7,
	
	PD_DATA_VENDOR_DEF = 15,
	
};

enum pd_ext_msg_type {
	
	PD_EXT_SOURCE_CAP_EXT = 1,
	PD_EXT_STATUS = 2,
	PD_EXT_GET_BATT_CAP = 3,
	PD_EXT_GET_BATT_STATUS = 4,
	PD_EXT_BATT_CAP = 5,
	PD_EXT_GET_MANUFACTURER_INFO = 6,
	PD_EXT_MANUFACTURER_INFO = 7,
	PD_EXT_SECURITY_REQUEST = 8,
	PD_EXT_SECURITY_RESPONSE = 9,
	PD_EXT_FW_UPDATE_REQUEST = 10,
	PD_EXT_FW_UPDATE_RESPONSE = 11,
	PD_EXT_PPS_STATUS = 12,
	PD_EXT_COUNTRY_INFO = 13,
	PD_EXT_COUNTRY_CODES = 14,
	
};

#define PD_REV10	0x0
#define PD_REV20	0x1
#define PD_REV30	0x2
#define PD_MAX_REV	PD_REV30

#define PD_HEADER_EXT_HDR	BIT(15)
#define PD_HEADER_CNT_SHIFT	12
#define PD_HEADER_CNT_MASK	0x7
#define PD_HEADER_ID_SHIFT	9
#define PD_HEADER_ID_MASK	0x7
#define PD_HEADER_PWR_ROLE	BIT(8)
#define PD_HEADER_REV_SHIFT	6
#define PD_HEADER_REV_MASK	0x3
#define PD_HEADER_DATA_ROLE	BIT(5)
#define PD_HEADER_TYPE_SHIFT	0
#define PD_HEADER_TYPE_MASK	0x1f

#define PD_HEADER(type, pwr, data, rev, id, cnt, ext_hdr)		\
	((((type) & PD_HEADER_TYPE_MASK) << PD_HEADER_TYPE_SHIFT) |	\
	 ((pwr) == TYPEC_SOURCE ? PD_HEADER_PWR_ROLE : 0) |		\
	 ((data) == TYPEC_HOST ? PD_HEADER_DATA_ROLE : 0) |		\
	 (rev << PD_HEADER_REV_SHIFT) |					\
	 (((id) & PD_HEADER_ID_MASK) << PD_HEADER_ID_SHIFT) |		\
	 (((cnt) & PD_HEADER_CNT_MASK) << PD_HEADER_CNT_SHIFT) |	\
	 ((ext_hdr) ? PD_HEADER_EXT_HDR : 0))

#define PD_HEADER_LE(type, pwr, data, rev, id, cnt) \
	cpu_to_le16(PD_HEADER((type), (pwr), (data), (rev), (id), (cnt), (0)))

static inline unsigned int pd_header_cnt(u16 header)
{
	return (header >> PD_HEADER_CNT_SHIFT) & PD_HEADER_CNT_MASK;
}

static inline unsigned int pd_header_cnt_le(__le16 header)
{
	return pd_header_cnt(le16_to_cpu(header));
}

static inline unsigned int pd_header_type(u16 header)
{
	return (header >> PD_HEADER_TYPE_SHIFT) & PD_HEADER_TYPE_MASK;
}

static inline unsigned int pd_header_type_le(__le16 header)
{
	return pd_header_type(le16_to_cpu(header));
}

static inline unsigned int pd_header_msgid(u16 header)
{
	return (header >> PD_HEADER_ID_SHIFT) & PD_HEADER_ID_MASK;
}

static inline unsigned int pd_header_msgid_le(__le16 header)
{
	return pd_header_msgid(le16_to_cpu(header));
}

static inline unsigned int pd_header_rev(u16 header)
{
	return (header >> PD_HEADER_REV_SHIFT) & PD_HEADER_REV_MASK;
}

static inline unsigned int pd_header_rev_le(__le16 header)
{
	return pd_header_rev(le16_to_cpu(header));
}

#define PD_EXT_HDR_CHUNKED		BIT(15)
#define PD_EXT_HDR_CHUNK_NUM_SHIFT	11
#define PD_EXT_HDR_CHUNK_NUM_MASK	0xf
#define PD_EXT_HDR_REQ_CHUNK		BIT(10)
#define PD_EXT_HDR_DATA_SIZE_SHIFT	0
#define PD_EXT_HDR_DATA_SIZE_MASK	0x1ff

#define PD_EXT_HDR(data_size, req_chunk, chunk_num, chunked)				\
	((((data_size) & PD_EXT_HDR_DATA_SIZE_MASK) << PD_EXT_HDR_DATA_SIZE_SHIFT) |	\
	 ((req_chunk) ? PD_EXT_HDR_REQ_CHUNK : 0) |					\
	 (((chunk_num) & PD_EXT_HDR_CHUNK_NUM_MASK) << PD_EXT_HDR_CHUNK_NUM_SHIFT) |	\
	 ((chunked) ? PD_EXT_HDR_CHUNKED : 0))

#define PD_EXT_HDR_LE(data_size, req_chunk, chunk_num, chunked) \
	cpu_to_le16(PD_EXT_HDR((data_size), (req_chunk), (chunk_num), (chunked)))

static inline unsigned int pd_ext_header_chunk_num(u16 ext_header)
{
	return (ext_header >> PD_EXT_HDR_CHUNK_NUM_SHIFT) &
		PD_EXT_HDR_CHUNK_NUM_MASK;
}

static inline unsigned int pd_ext_header_data_size(u16 ext_header)
{
	return (ext_header >> PD_EXT_HDR_DATA_SIZE_SHIFT) &
		PD_EXT_HDR_DATA_SIZE_MASK;
}

static inline unsigned int pd_ext_header_data_size_le(__le16 ext_header)
{
	return pd_ext_header_data_size(le16_to_cpu(ext_header));
}

#define PD_MAX_PAYLOAD		7
#define PD_EXT_MAX_CHUNK_DATA	26


struct pd_chunked_ext_message_data {
	__le16 header;
	u8 data[PD_EXT_MAX_CHUNK_DATA];
} __packed;


struct pd_message {
	__le16 header;
	union {
		__le32 payload[PD_MAX_PAYLOAD];
		struct pd_chunked_ext_message_data ext_msg;
	};
} __packed;


#define PDO_MAX_OBJECTS		7

enum pd_pdo_type {
	PDO_TYPE_FIXED = 0,
	PDO_TYPE_BATT = 1,
	PDO_TYPE_VAR = 2,
	PDO_TYPE_APDO = 3,
};

#define PDO_TYPE_SHIFT		30
#define PDO_TYPE_MASK		0x3

#define PDO_TYPE(t)	((t) << PDO_TYPE_SHIFT)

#define PDO_VOLT_MASK		0x3ff
#define PDO_CURR_MASK		0x3ff
#define PDO_PWR_MASK		0x3ff

#define PDO_FIXED_DUAL_ROLE	BIT(29)	
#define PDO_FIXED_SUSPEND	BIT(28) 
#define PDO_FIXED_HIGHER_CAP	BIT(28) 
#define PDO_FIXED_EXTPOWER	BIT(27) 
#define PDO_FIXED_USB_COMM	BIT(26) 
#define PDO_FIXED_DATA_SWAP	BIT(25) 
#define PDO_FIXED_VOLT_SHIFT	10	
#define PDO_FIXED_CURR_SHIFT	0	

#define PDO_FIXED_VOLT(mv)	((((mv) / 50) & PDO_VOLT_MASK) << PDO_FIXED_VOLT_SHIFT)
#define PDO_FIXED_CURR(ma)	((((ma) / 10) & PDO_CURR_MASK) << PDO_FIXED_CURR_SHIFT)

#define PDO_FIXED(mv, ma, flags)			\
	(PDO_TYPE(PDO_TYPE_FIXED) | (flags) |		\
	 PDO_FIXED_VOLT(mv) | PDO_FIXED_CURR(ma))

#define VSAFE5V 5000 

#define PDO_BATT_MAX_VOLT_SHIFT	20	
#define PDO_BATT_MIN_VOLT_SHIFT	10	
#define PDO_BATT_MAX_PWR_SHIFT	0	

#define PDO_BATT_MIN_VOLT(mv) ((((mv) / 50) & PDO_VOLT_MASK) << PDO_BATT_MIN_VOLT_SHIFT)
#define PDO_BATT_MAX_VOLT(mv) ((((mv) / 50) & PDO_VOLT_MASK) << PDO_BATT_MAX_VOLT_SHIFT)
#define PDO_BATT_MAX_POWER(mw) ((((mw) / 250) & PDO_PWR_MASK) << PDO_BATT_MAX_PWR_SHIFT)

#define PDO_BATT(min_mv, max_mv, max_mw)			\
	(PDO_TYPE(PDO_TYPE_BATT) | PDO_BATT_MIN_VOLT(min_mv) |	\
	 PDO_BATT_MAX_VOLT(max_mv) | PDO_BATT_MAX_POWER(max_mw))

#define PDO_VAR_MAX_VOLT_SHIFT	20	
#define PDO_VAR_MIN_VOLT_SHIFT	10	
#define PDO_VAR_MAX_CURR_SHIFT	0	

#define PDO_VAR_MIN_VOLT(mv) ((((mv) / 50) & PDO_VOLT_MASK) << PDO_VAR_MIN_VOLT_SHIFT)
#define PDO_VAR_MAX_VOLT(mv) ((((mv) / 50) & PDO_VOLT_MASK) << PDO_VAR_MAX_VOLT_SHIFT)
#define PDO_VAR_MAX_CURR(ma) ((((ma) / 10) & PDO_CURR_MASK) << PDO_VAR_MAX_CURR_SHIFT)

#define PDO_VAR(min_mv, max_mv, max_ma)				\
	(PDO_TYPE(PDO_TYPE_VAR) | PDO_VAR_MIN_VOLT(min_mv) |	\
	 PDO_VAR_MAX_VOLT(max_mv) | PDO_VAR_MAX_CURR(max_ma))

enum pd_apdo_type {
	APDO_TYPE_PPS = 0,
};

#define PDO_APDO_TYPE_SHIFT	28	
#define PDO_APDO_TYPE_MASK	0x3

#define PDO_APDO_TYPE(t)	((t) << PDO_APDO_TYPE_SHIFT)

#define PDO_PPS_APDO_MAX_VOLT_SHIFT	17	
#define PDO_PPS_APDO_MIN_VOLT_SHIFT	8	
#define PDO_PPS_APDO_MAX_CURR_SHIFT	0	

#define PDO_PPS_APDO_VOLT_MASK	0xff
#define PDO_PPS_APDO_CURR_MASK	0x7f

#define PDO_PPS_APDO_MIN_VOLT(mv)	\
	((((mv) / 100) & PDO_PPS_APDO_VOLT_MASK) << PDO_PPS_APDO_MIN_VOLT_SHIFT)
#define PDO_PPS_APDO_MAX_VOLT(mv)	\
	((((mv) / 100) & PDO_PPS_APDO_VOLT_MASK) << PDO_PPS_APDO_MAX_VOLT_SHIFT)
#define PDO_PPS_APDO_MAX_CURR(ma)	\
	((((ma) / 50) & PDO_PPS_APDO_CURR_MASK) << PDO_PPS_APDO_MAX_CURR_SHIFT)

#define PDO_PPS_APDO(min_mv, max_mv, max_ma)				\
	(PDO_TYPE(PDO_TYPE_APDO) | PDO_APDO_TYPE(APDO_TYPE_PPS) |	\
	PDO_PPS_APDO_MIN_VOLT(min_mv) | PDO_PPS_APDO_MAX_VOLT(max_mv) |	\
	PDO_PPS_APDO_MAX_CURR(max_ma))

static inline enum pd_pdo_type pdo_type(u32 pdo)
{
	return (pdo >> PDO_TYPE_SHIFT) & PDO_TYPE_MASK;
}

static inline unsigned int pdo_fixed_voltage(u32 pdo)
{
	return ((pdo >> PDO_FIXED_VOLT_SHIFT) & PDO_VOLT_MASK) * 50;
}

static inline unsigned int pdo_min_voltage(u32 pdo)
{
	return ((pdo >> PDO_VAR_MIN_VOLT_SHIFT) & PDO_VOLT_MASK) * 50;
}

static inline unsigned int pdo_max_voltage(u32 pdo)
{
	return ((pdo >> PDO_VAR_MAX_VOLT_SHIFT) & PDO_VOLT_MASK) * 50;
}

static inline unsigned int pdo_max_current(u32 pdo)
{
	return ((pdo >> PDO_VAR_MAX_CURR_SHIFT) & PDO_CURR_MASK) * 10;
}

static inline unsigned int pdo_max_power(u32 pdo)
{
	return ((pdo >> PDO_BATT_MAX_PWR_SHIFT) & PDO_PWR_MASK) * 250;
}

static inline enum pd_apdo_type pdo_apdo_type(u32 pdo)
{
	return (pdo >> PDO_APDO_TYPE_SHIFT) & PDO_APDO_TYPE_MASK;
}

static inline unsigned int pdo_pps_apdo_min_voltage(u32 pdo)
{
	return ((pdo >> PDO_PPS_APDO_MIN_VOLT_SHIFT) &
		PDO_PPS_APDO_VOLT_MASK) * 100;
}

static inline unsigned int pdo_pps_apdo_max_voltage(u32 pdo)
{
	return ((pdo >> PDO_PPS_APDO_MAX_VOLT_SHIFT) &
		PDO_PPS_APDO_VOLT_MASK) * 100;
}

static inline unsigned int pdo_pps_apdo_max_current(u32 pdo)
{
	return ((pdo >> PDO_PPS_APDO_MAX_CURR_SHIFT) &
		PDO_PPS_APDO_CURR_MASK) * 50;
}


#define RDO_OBJ_POS_SHIFT	28
#define RDO_OBJ_POS_MASK	0x7
#define RDO_GIVE_BACK		BIT(27)	
#define RDO_CAP_MISMATCH	BIT(26) 
#define RDO_USB_COMM		BIT(25) 
#define RDO_NO_SUSPEND		BIT(24) 

#define RDO_PWR_MASK			0x3ff
#define RDO_CURR_MASK			0x3ff

#define RDO_FIXED_OP_CURR_SHIFT		10
#define RDO_FIXED_MAX_CURR_SHIFT	0

#define RDO_OBJ(idx) (((idx) & RDO_OBJ_POS_MASK) << RDO_OBJ_POS_SHIFT)

#define PDO_FIXED_OP_CURR(ma) ((((ma) / 10) & RDO_CURR_MASK) << RDO_FIXED_OP_CURR_SHIFT)
#define PDO_FIXED_MAX_CURR(ma) ((((ma) / 10) & RDO_CURR_MASK) << RDO_FIXED_MAX_CURR_SHIFT)

#define RDO_FIXED(idx, op_ma, max_ma, flags)			\
	(RDO_OBJ(idx) | (flags) |				\
	 PDO_FIXED_OP_CURR(op_ma) | PDO_FIXED_MAX_CURR(max_ma))

#define RDO_BATT_OP_PWR_SHIFT		10	
#define RDO_BATT_MAX_PWR_SHIFT		0	

#define RDO_BATT_OP_PWR(mw) ((((mw) / 250) & RDO_PWR_MASK) << RDO_BATT_OP_PWR_SHIFT)
#define RDO_BATT_MAX_PWR(mw) ((((mw) / 250) & RDO_PWR_MASK) << RDO_BATT_MAX_PWR_SHIFT)

#define RDO_BATT(idx, op_mw, max_mw, flags)			\
	(RDO_OBJ(idx) | (flags) |				\
	 RDO_BATT_OP_PWR(op_mw) | RDO_BATT_MAX_PWR(max_mw))

#define RDO_PROG_VOLT_MASK	0x7ff
#define RDO_PROG_CURR_MASK	0x7f

#define RDO_PROG_VOLT_SHIFT	9
#define RDO_PROG_CURR_SHIFT	0

#define RDO_PROG_VOLT_MV_STEP	20
#define RDO_PROG_CURR_MA_STEP	50

#define PDO_PROG_OUT_VOLT(mv)	\
	((((mv) / RDO_PROG_VOLT_MV_STEP) & RDO_PROG_VOLT_MASK) << RDO_PROG_VOLT_SHIFT)
#define PDO_PROG_OP_CURR(ma)	\
	((((ma) / RDO_PROG_CURR_MA_STEP) & RDO_PROG_CURR_MASK) << RDO_PROG_CURR_SHIFT)

#define RDO_PROG(idx, out_mv, op_ma, flags)			\
	(RDO_OBJ(idx) | (flags) |				\
	 PDO_PROG_OUT_VOLT(out_mv) | PDO_PROG_OP_CURR(op_ma))

static inline unsigned int rdo_index(u32 rdo)
{
	return (rdo >> RDO_OBJ_POS_SHIFT) & RDO_OBJ_POS_MASK;
}

static inline unsigned int rdo_op_current(u32 rdo)
{
	return ((rdo >> RDO_FIXED_OP_CURR_SHIFT) & RDO_CURR_MASK) * 10;
}

static inline unsigned int rdo_max_current(u32 rdo)
{
	return ((rdo >> RDO_FIXED_MAX_CURR_SHIFT) &
		RDO_CURR_MASK) * 10;
}

static inline unsigned int rdo_op_power(u32 rdo)
{
	return ((rdo >> RDO_BATT_OP_PWR_SHIFT) & RDO_PWR_MASK) * 250;
}

static inline unsigned int rdo_max_power(u32 rdo)
{
	return ((rdo >> RDO_BATT_MAX_PWR_SHIFT) & RDO_PWR_MASK) * 250;
}


#define PD_T_NO_RESPONSE	5000	
#define PD_T_DB_DETECT		10000	
#define PD_T_SEND_SOURCE_CAP	150	
#define PD_T_SENDER_RESPONSE	60	
#define PD_T_SOURCE_ACTIVITY	45
#define PD_T_SINK_ACTIVITY	135
#define PD_T_SINK_WAIT_CAP	240
#define PD_T_PS_TRANSITION	500
#define PD_T_SRC_TRANSITION	35
#define PD_T_DRP_SNK		40
#define PD_T_DRP_SRC		30
#define PD_T_PS_SOURCE_OFF	920
#define PD_T_PS_SOURCE_ON	480
#define PD_T_PS_HARD_RESET	30
#define PD_T_SRC_RECOVER	760
#define PD_T_SRC_RECOVER_MAX	1000
#define PD_T_SRC_TURN_ON	275
#define PD_T_SAFE_0V		650
#define PD_T_VCONN_SOURCE_ON	100
#define PD_T_SINK_REQUEST	100	
#define PD_T_ERROR_RECOVERY	100	
#define PD_T_SRCSWAPSTDBY      625     
#define PD_T_NEWSRC            250     
#define PD_T_SWAP_SRC_START	20	

#define PD_T_DRP_TRY		100	
#define PD_T_DRP_TRYWAIT	600	

#define PD_T_CC_DEBOUNCE	200	
#define PD_T_PD_DEBOUNCE	20	

#define PD_N_CAPS_COUNT		(PD_T_NO_RESPONSE / PD_T_SEND_SOURCE_CAP)
#define PD_N_HARD_RESET_COUNT	2

#endif 
