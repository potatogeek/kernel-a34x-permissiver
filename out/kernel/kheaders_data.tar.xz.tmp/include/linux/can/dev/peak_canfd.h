
#ifndef PUCAN_H
#define PUCAN_H


#define PUCAN_CMD_NOP			0x000
#define PUCAN_CMD_RESET_MODE		0x001
#define PUCAN_CMD_NORMAL_MODE		0x002
#define PUCAN_CMD_LISTEN_ONLY_MODE	0x003
#define PUCAN_CMD_TIMING_SLOW		0x004
#define PUCAN_CMD_TIMING_FAST		0x005
#define PUCAN_CMD_SET_STD_FILTER	0x006
#define PUCAN_CMD_RESERVED2		0x007
#define PUCAN_CMD_FILTER_STD		0x008
#define PUCAN_CMD_TX_ABORT		0x009
#define PUCAN_CMD_WR_ERR_CNT		0x00a
#define PUCAN_CMD_SET_EN_OPTION		0x00b
#define PUCAN_CMD_CLR_DIS_OPTION	0x00c
#define PUCAN_CMD_RX_BARRIER		0x010
#define PUCAN_CMD_END_OF_COLLECTION	0x3ff


#define PUCAN_MSG_CAN_RX		0x0001
#define PUCAN_MSG_ERROR			0x0002
#define PUCAN_MSG_STATUS		0x0003
#define PUCAN_MSG_BUSLOAD		0x0004

#define PUCAN_MSG_CACHE_CRITICAL	0x0102


#define PUCAN_MSG_CAN_TX		0x1000


struct __packed pucan_command {
	__le16	opcode_channel;
	u16	args[3];
};


static inline u16 pucan_cmd_get_opcode(struct pucan_command *c)
{
	return le16_to_cpu(c->opcode_channel) & 0x3ff;
}

#define PUCAN_TSLOW_BRP_BITS		10
#define PUCAN_TSLOW_TSGEG1_BITS		8
#define PUCAN_TSLOW_TSGEG2_BITS		7
#define PUCAN_TSLOW_SJW_BITS		7

#define PUCAN_TSLOW_BRP_MASK		((1 << PUCAN_TSLOW_BRP_BITS) - 1)
#define PUCAN_TSLOW_TSEG1_MASK		((1 << PUCAN_TSLOW_TSGEG1_BITS) - 1)
#define PUCAN_TSLOW_TSEG2_MASK		((1 << PUCAN_TSLOW_TSGEG2_BITS) - 1)
#define PUCAN_TSLOW_SJW_MASK		((1 << PUCAN_TSLOW_SJW_BITS) - 1)


#define PUCAN_TSLOW_SJW_T(s, t)		(((s) & PUCAN_TSLOW_SJW_MASK) | \
								((!!(t)) << 7))
#define PUCAN_TSLOW_TSEG2(t)		((t) & PUCAN_TSLOW_TSEG2_MASK)
#define PUCAN_TSLOW_TSEG1(t)		((t) & PUCAN_TSLOW_TSEG1_MASK)
#define PUCAN_TSLOW_BRP(b)		((b) & PUCAN_TSLOW_BRP_MASK)

struct __packed pucan_timing_slow {
	__le16	opcode_channel;

	u8	ewl;		
	u8	sjw_t;		
	u8	tseg2;		
	u8	tseg1;		

	__le16	brp;		
};

#define PUCAN_TFAST_BRP_BITS		10
#define PUCAN_TFAST_TSGEG1_BITS		5
#define PUCAN_TFAST_TSGEG2_BITS		4
#define PUCAN_TFAST_SJW_BITS		4

#define PUCAN_TFAST_BRP_MASK		((1 << PUCAN_TFAST_BRP_BITS) - 1)
#define PUCAN_TFAST_TSEG1_MASK		((1 << PUCAN_TFAST_TSGEG1_BITS) - 1)
#define PUCAN_TFAST_TSEG2_MASK		((1 << PUCAN_TFAST_TSGEG2_BITS) - 1)
#define PUCAN_TFAST_SJW_MASK		((1 << PUCAN_TFAST_SJW_BITS) - 1)


#define PUCAN_TFAST_SJW(s)		((s) & PUCAN_TFAST_SJW_MASK)
#define PUCAN_TFAST_TSEG2(t)		((t) & PUCAN_TFAST_TSEG2_MASK)
#define PUCAN_TFAST_TSEG1(t)		((t) & PUCAN_TFAST_TSEG1_MASK)
#define PUCAN_TFAST_BRP(b)		((b) & PUCAN_TFAST_BRP_MASK)

struct __packed pucan_timing_fast {
	__le16	opcode_channel;

	u8	unused;
	u8	sjw;		
	u8	tseg2;		
	u8	tseg1;		

	__le16	brp;		
};


#define PUCAN_FLTSTD_ROW_IDX_BITS	6

struct __packed pucan_filter_std {
	__le16	opcode_channel;

	__le16	idx;
	__le32	mask;		
};

#define PUCAN_FLTSTD_ROW_IDX_MAX	((1 << PUCAN_FLTSTD_ROW_IDX_BITS) - 1)


struct __packed pucan_std_filter {
	__le16	opcode_channel;

	u8	unused;
	u8	idx;
	__le32	mask;		
};


#define PUCAN_TX_ABORT_FLUSH		0x0001

struct __packed pucan_tx_abort {
	__le16	opcode_channel;

	__le16	flags;
	u32	unused;
};


#define PUCAN_WRERRCNT_TE		0x4000	
#define PUCAN_WRERRCNT_RE		0x8000	

struct __packed pucan_wr_err_cnt {
	__le16	opcode_channel;

	__le16	sel_mask;
	u8	tx_counter;	
	u8	rx_counter;	

	u16	unused;
};


#define PUCAN_OPTION_ERROR		0x0001
#define PUCAN_OPTION_BUSLOAD		0x0002
#define PUCAN_OPTION_CANDFDISO		0x0004

struct __packed pucan_options {
	__le16	opcode_channel;

	__le16	options;
	u32	unused;
};


struct __packed pucan_msg {
	__le16	size;
	__le16	type;
	__le32	ts_low;
	__le32	ts_high;
};


#define PUCAN_MSG_SELF_RECEIVE		0x80
#define PUCAN_MSG_ERROR_STATE_IND	0x40	
#define PUCAN_MSG_BITRATE_SWITCH	0x20	
#define PUCAN_MSG_EXT_DATA_LEN		0x10	
#define PUCAN_MSG_SINGLE_SHOT		0x08
#define PUCAN_MSG_LOOPED_BACK		0x04
#define PUCAN_MSG_EXT_ID		0x02
#define PUCAN_MSG_RTR			0x01

struct __packed pucan_rx_msg {
	__le16	size;
	__le16	type;
	__le32	ts_low;
	__le32	ts_high;
	__le32	tag_low;
	__le32	tag_high;
	u8	channel_dlc;
	u8	client;
	__le16	flags;
	__le32	can_id;
	u8	d[0];
};


#define PUCAN_ERMSG_BIT_ERROR		0
#define PUCAN_ERMSG_FORM_ERROR		1
#define PUCAN_ERMSG_STUFF_ERROR		2
#define PUCAN_ERMSG_OTHER_ERROR		3
#define PUCAN_ERMSG_ERR_CNT_DEC		4

struct __packed pucan_error_msg {
	__le16	size;
	__le16	type;
	__le32	ts_low;
	__le32	ts_high;
	u8	channel_type_d;
	u8	code_g;
	u8	tx_err_cnt;
	u8	rx_err_cnt;
};

static inline int pucan_error_get_channel(const struct pucan_error_msg *msg)
{
	return msg->channel_type_d & 0x0f;
}

#define PUCAN_RX_BARRIER		0x10
#define PUCAN_BUS_PASSIVE		0x20
#define PUCAN_BUS_WARNING		0x40
#define PUCAN_BUS_BUSOFF		0x80

struct __packed pucan_status_msg {
	__le16	size;
	__le16	type;
	__le32	ts_low;
	__le32	ts_high;
	u8	channel_p_w_b;
	u8	unused[3];
};

static inline int pucan_status_get_channel(const struct pucan_status_msg *msg)
{
	return msg->channel_p_w_b & 0x0f;
}

static inline int pucan_status_is_rx_barrier(const struct pucan_status_msg *msg)
{
	return msg->channel_p_w_b & PUCAN_RX_BARRIER;
}

static inline int pucan_status_is_passive(const struct pucan_status_msg *msg)
{
	return msg->channel_p_w_b & PUCAN_BUS_PASSIVE;
}

static inline int pucan_status_is_warning(const struct pucan_status_msg *msg)
{
	return msg->channel_p_w_b & PUCAN_BUS_WARNING;
}

static inline int pucan_status_is_busoff(const struct pucan_status_msg *msg)
{
	return msg->channel_p_w_b & PUCAN_BUS_BUSOFF;
}


#define PUCAN_MSG_CHANNEL_DLC(c, d)	(((c) & 0xf) | ((d) << 4))

struct __packed pucan_tx_msg {
	__le16	size;
	__le16	type;
	__le32	tag_low;
	__le32	tag_high;
	u8	channel_dlc;
	u8	client;
	__le16	flags;
	__le32	can_id;
	u8	d[0];
};


static inline __le16 pucan_cmd_opcode_channel(int index, int opcode)
{
	return cpu_to_le16(((index) << 12) | ((opcode) & 0x3ff));
}


static inline int pucan_msg_get_channel(const struct pucan_rx_msg *msg)
{
	return msg->channel_dlc & 0xf;
}


static inline int pucan_msg_get_dlc(const struct pucan_rx_msg *msg)
{
	return msg->channel_dlc >> 4;
}

static inline int pucan_ermsg_get_channel(const struct pucan_error_msg *msg)
{
	return msg->channel_type_d & 0x0f;
}

static inline int pucan_stmsg_get_channel(const struct pucan_status_msg *msg)
{
	return msg->channel_p_w_b & 0x0f;
}

#endif
