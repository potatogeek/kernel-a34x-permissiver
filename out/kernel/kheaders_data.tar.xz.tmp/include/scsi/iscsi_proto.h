

#ifndef ISCSI_PROTO_H
#define ISCSI_PROTO_H

#include <linux/types.h>
#include <scsi/scsi.h>

#define ISCSI_DRAFT20_VERSION	0x00


#define ISCSI_LISTEN_PORT	3260


#define ISCSI_HDR_LEN		48


#define ISCSI_CRC_LEN		4


#define ISCSI_PAD_LEN		4



static inline int iscsi_sna_lt(u32 n1, u32 n2)
{
	return (s32)(n1 - n2) < 0;
}

static inline int iscsi_sna_lte(u32 n1, u32 n2)
{
	return (s32)(n1 - n2) <= 0;
}

static inline int iscsi_sna_gt(u32 n1, u32 n2)
{
	return (s32)(n1 - n2) > 0;
}

static inline int iscsi_sna_gte(u32 n1, u32 n2)
{
	return (s32)(n1 - n2) >= 0;
}


#define ntoh24(p) (((p)[0] << 16) | ((p)[1] << 8) | ((p)[2]))
#define hton24(p, v) { \
        p[0] = (((v) >> 16) & 0xFF); \
        p[1] = (((v) >> 8) & 0xFF); \
        p[2] = ((v) & 0xFF); \
}
#define zero_data(p) {p[0]=0;p[1]=0;p[2]=0;}


typedef uint32_t __bitwise itt_t;

#define build_itt(itt, age) ((__force itt_t)\
	((itt) | ((age) << ISCSI_AGE_SHIFT)))
#define get_itt(itt) ((__force uint32_t)(itt_t)(itt) & ISCSI_ITT_MASK)
#define RESERVED_ITT ((__force itt_t)0xffffffff)


struct iscsi_hdr {
	uint8_t		opcode;
	uint8_t		flags;		
	uint8_t		rsvd2[2];
	uint8_t		hlength;	
	uint8_t		dlength[3];	
	struct scsi_lun	lun;
	itt_t		itt;		
	__be32		ttt;		
	__be32		statsn;
	__be32		exp_statsn;
	__be32		max_statsn;
	uint8_t		other[12];
};



#define ISCSI_RESERVED_TAG		0xffffffff


#define ISCSI_OP_RETRY			0x80
#define ISCSI_OP_IMMEDIATE		0x40
#define ISCSI_OPCODE_MASK		0x3F


#define ISCSI_OP_NOOP_OUT		0x00
#define ISCSI_OP_SCSI_CMD		0x01
#define ISCSI_OP_SCSI_TMFUNC		0x02
#define ISCSI_OP_LOGIN			0x03
#define ISCSI_OP_TEXT			0x04
#define ISCSI_OP_SCSI_DATA_OUT		0x05
#define ISCSI_OP_LOGOUT			0x06
#define ISCSI_OP_SNACK			0x10

#define ISCSI_OP_VENDOR1_CMD		0x1c
#define ISCSI_OP_VENDOR2_CMD		0x1d
#define ISCSI_OP_VENDOR3_CMD		0x1e
#define ISCSI_OP_VENDOR4_CMD		0x1f


#define ISCSI_OP_NOOP_IN		0x20
#define ISCSI_OP_SCSI_CMD_RSP		0x21
#define ISCSI_OP_SCSI_TMFUNC_RSP	0x22
#define ISCSI_OP_LOGIN_RSP		0x23
#define ISCSI_OP_TEXT_RSP		0x24
#define ISCSI_OP_SCSI_DATA_IN		0x25
#define ISCSI_OP_LOGOUT_RSP		0x26
#define ISCSI_OP_R2T			0x31
#define ISCSI_OP_ASYNC_EVENT		0x32
#define ISCSI_OP_REJECT			0x3f

struct iscsi_ahs_hdr {
	__be16 ahslength;
	uint8_t ahstype;
	uint8_t ahspec[5];
};

#define ISCSI_AHSTYPE_CDB		1
#define ISCSI_AHSTYPE_RLENGTH		2
#define ISCSI_CDB_SIZE			16


struct iscsi_scsi_req {
	uint8_t opcode;
	uint8_t flags;
	__be16 rsvd2;
	uint8_t hlength;
	uint8_t dlength[3];
	struct scsi_lun lun;
	itt_t	 itt;	
	__be32 data_length;
	__be32 cmdsn;
	__be32 exp_statsn;
	uint8_t cdb[ISCSI_CDB_SIZE];	
	
};


#define ISCSI_FLAG_CMD_FINAL		0x80
#define ISCSI_FLAG_CMD_READ		0x40
#define ISCSI_FLAG_CMD_WRITE		0x20
#define ISCSI_FLAG_CMD_ATTR_MASK	0x07	


#define ISCSI_ATTR_UNTAGGED		0
#define ISCSI_ATTR_SIMPLE		1
#define ISCSI_ATTR_ORDERED		2
#define ISCSI_ATTR_HEAD_OF_QUEUE	3
#define ISCSI_ATTR_ACA			4

struct iscsi_rlength_ahdr {
	__be16 ahslength;
	uint8_t ahstype;
	uint8_t reserved;
	__be32 read_length;
};


struct iscsi_ecdb_ahdr {
	__be16 ahslength;	
	uint8_t ahstype;
	uint8_t reserved;
	
	uint8_t ecdb[SCSI_MAX_VARLEN_CDB_SIZE - ISCSI_CDB_SIZE];
};


struct iscsi_scsi_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t response;
	uint8_t cmd_status;
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd[8];
	itt_t	 itt;	
	__be32	rsvd1;
	__be32	statsn;
	__be32	exp_cmdsn;
	__be32	max_cmdsn;
	__be32	exp_datasn;
	__be32	bi_residual_count;
	__be32	residual_count;
	
};


#define ISCSI_FLAG_CMD_BIDI_OVERFLOW	0x10
#define ISCSI_FLAG_CMD_BIDI_UNDERFLOW	0x08
#define ISCSI_FLAG_CMD_OVERFLOW		0x04
#define ISCSI_FLAG_CMD_UNDERFLOW	0x02


#define ISCSI_STATUS_CMD_COMPLETED	0
#define ISCSI_STATUS_TARGET_FAILURE	1
#define ISCSI_STATUS_SUBSYS_FAILURE	2


struct iscsi_async {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t rsvd3;
	uint8_t dlength[3];
	struct scsi_lun	lun;
	uint8_t rsvd4[8];
	__be32	statsn;
	__be32	exp_cmdsn;
	__be32	max_cmdsn;
	uint8_t async_event;
	uint8_t async_vcode;
	__be16	param1;
	__be16	param2;
	__be16	param3;
	uint8_t rsvd5[4];
};


#define ISCSI_ASYNC_MSG_SCSI_EVENT			0
#define ISCSI_ASYNC_MSG_REQUEST_LOGOUT			1
#define ISCSI_ASYNC_MSG_DROPPING_CONNECTION		2
#define ISCSI_ASYNC_MSG_DROPPING_ALL_CONNECTIONS	3
#define ISCSI_ASYNC_MSG_PARAM_NEGOTIATION		4
#define ISCSI_ASYNC_MSG_VENDOR_SPECIFIC			255


struct iscsi_nopout {
	uint8_t opcode;
	uint8_t flags;
	__be16	rsvd2;
	uint8_t rsvd3;
	uint8_t dlength[3];
	struct scsi_lun	lun;
	itt_t	 itt;	
	__be32	ttt;	
	__be32	cmdsn;
	__be32	exp_statsn;
	uint8_t rsvd4[16];
};


struct iscsi_nopin {
	uint8_t opcode;
	uint8_t flags;
	__be16	rsvd2;
	uint8_t rsvd3;
	uint8_t dlength[3];
	struct scsi_lun	lun;
	itt_t	 itt;	
	__be32	ttt;	
	__be32	statsn;
	__be32	exp_cmdsn;
	__be32	max_cmdsn;
	uint8_t rsvd4[12];
};


struct iscsi_tm {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd1[2];
	uint8_t hlength;
	uint8_t dlength[3];
	struct scsi_lun lun;
	itt_t	 itt;	
	itt_t	 rtt;	
	__be32	cmdsn;
	__be32	exp_statsn;
	__be32	refcmdsn;
	__be32	exp_datasn;
	uint8_t rsvd2[8];
};

#define ISCSI_FLAG_TM_FUNC_MASK			0x7F


#define ISCSI_TM_FUNC_ABORT_TASK		1
#define ISCSI_TM_FUNC_ABORT_TASK_SET		2
#define ISCSI_TM_FUNC_CLEAR_ACA			3
#define ISCSI_TM_FUNC_CLEAR_TASK_SET		4
#define ISCSI_TM_FUNC_LOGICAL_UNIT_RESET	5
#define ISCSI_TM_FUNC_TARGET_WARM_RESET		6
#define ISCSI_TM_FUNC_TARGET_COLD_RESET		7
#define ISCSI_TM_FUNC_TASK_REASSIGN		8

#define ISCSI_TM_FUNC_VALUE(hdr) ((hdr)->flags & ISCSI_FLAG_TM_FUNC_MASK)


struct iscsi_tm_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t response;	
	uint8_t qualifier;
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd2[8];
	itt_t	 itt;	
	itt_t	 rtt;	
	__be32	statsn;
	__be32	exp_cmdsn;
	__be32	max_cmdsn;
	uint8_t rsvd3[12];
};


#define ISCSI_TMF_RSP_COMPLETE		0x00
#define ISCSI_TMF_RSP_NO_TASK		0x01
#define ISCSI_TMF_RSP_NO_LUN		0x02
#define ISCSI_TMF_RSP_TASK_ALLEGIANT	0x03
#define ISCSI_TMF_RSP_NO_FAILOVER	0x04
#define ISCSI_TMF_RSP_NOT_SUPPORTED	0x05
#define ISCSI_TMF_RSP_AUTH_FAILED	0x06
#define ISCSI_TMF_RSP_REJECTED		0xff


struct iscsi_r2t_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t	hlength;
	uint8_t	dlength[3];
	struct scsi_lun	lun;
	itt_t	 itt;	
	__be32	ttt;	
	__be32	statsn;
	__be32	exp_cmdsn;
	__be32	max_cmdsn;
	__be32	r2tsn;
	__be32	data_offset;
	__be32	data_length;
};


struct iscsi_data {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t rsvd3;
	uint8_t dlength[3];
	struct scsi_lun lun;
	itt_t	 itt;
	__be32	ttt;
	__be32	rsvd4;
	__be32	exp_statsn;
	__be32	rsvd5;
	__be32	datasn;
	__be32	offset;
	__be32	rsvd6;
	
};


struct iscsi_data_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2;
	uint8_t cmd_status;
	uint8_t hlength;
	uint8_t dlength[3];
	struct scsi_lun	lun;
	itt_t	 itt;
	__be32	ttt;
	__be32	statsn;
	__be32	exp_cmdsn;
	__be32	max_cmdsn;
	__be32	datasn;
	__be32	offset;
	__be32	residual_count;
};


#define ISCSI_FLAG_DATA_ACK		0x40
#define ISCSI_FLAG_DATA_OVERFLOW	0x04
#define ISCSI_FLAG_DATA_UNDERFLOW	0x02
#define ISCSI_FLAG_DATA_STATUS		0x01


struct iscsi_text {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd4[8];
	itt_t	 itt;
	__be32	ttt;
	__be32	cmdsn;
	__be32	exp_statsn;
	uint8_t rsvd5[16];
	
};

#define ISCSI_FLAG_TEXT_CONTINUE	0x40


struct iscsi_text_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd4[8];
	itt_t	 itt;
	__be32	ttt;
	__be32	statsn;
	__be32	exp_cmdsn;
	__be32	max_cmdsn;
	uint8_t rsvd5[12];
	
};


struct iscsi_login_req {
	uint8_t opcode;
	uint8_t flags;
	uint8_t max_version;	
	uint8_t min_version;	
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t isid[6];	
	__be16	tsih;	
	itt_t	 itt;	
	__be16	cid;
	__be16	rsvd3;
	__be32	cmdsn;
	__be32	exp_statsn;
	uint8_t rsvd5[16];
};


#define ISCSI_FLAG_LOGIN_TRANSIT		0x80
#define ISCSI_FLAG_LOGIN_CONTINUE		0x40
#define ISCSI_FLAG_LOGIN_CURRENT_STAGE_MASK	0x0C	
#define ISCSI_FLAG_LOGIN_CURRENT_STAGE1		0x04
#define ISCSI_FLAG_LOGIN_CURRENT_STAGE2		0x08
#define ISCSI_FLAG_LOGIN_CURRENT_STAGE3		0x0C
#define ISCSI_FLAG_LOGIN_NEXT_STAGE_MASK	0x03	
#define ISCSI_FLAG_LOGIN_NEXT_STAGE1		0x01
#define ISCSI_FLAG_LOGIN_NEXT_STAGE2		0x02
#define ISCSI_FLAG_LOGIN_NEXT_STAGE3		0x03

#define ISCSI_LOGIN_CURRENT_STAGE(flags) \
	((flags & ISCSI_FLAG_LOGIN_CURRENT_STAGE_MASK) >> 2)
#define ISCSI_LOGIN_NEXT_STAGE(flags) \
	(flags & ISCSI_FLAG_LOGIN_NEXT_STAGE_MASK)


struct iscsi_login_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t max_version;	
	uint8_t active_version;	
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t isid[6];	
	__be16	tsih;	
	itt_t	 itt;	
	__be32	rsvd3;
	__be32	statsn;
	__be32	exp_cmdsn;
	__be32	max_cmdsn;
	uint8_t status_class;	
	uint8_t status_detail;	
	uint8_t rsvd4[10];
};


#define ISCSI_INITIAL_LOGIN_STAGE		-1
#define ISCSI_SECURITY_NEGOTIATION_STAGE	0
#define ISCSI_OP_PARMS_NEGOTIATION_STAGE	1
#define ISCSI_FULL_FEATURE_PHASE		3


#define ISCSI_STATUS_CLS_SUCCESS		0x00
#define ISCSI_STATUS_CLS_REDIRECT		0x01
#define ISCSI_STATUS_CLS_INITIATOR_ERR		0x02
#define ISCSI_STATUS_CLS_TARGET_ERR		0x03



#define ISCSI_LOGIN_STATUS_ACCEPT		0x00


#define ISCSI_LOGIN_STATUS_TGT_MOVED_TEMP	0x01
#define ISCSI_LOGIN_STATUS_TGT_MOVED_PERM	0x02


#define ISCSI_LOGIN_STATUS_INIT_ERR		0x00
#define ISCSI_LOGIN_STATUS_AUTH_FAILED		0x01
#define ISCSI_LOGIN_STATUS_TGT_FORBIDDEN	0x02
#define ISCSI_LOGIN_STATUS_TGT_NOT_FOUND	0x03
#define ISCSI_LOGIN_STATUS_TGT_REMOVED		0x04
#define ISCSI_LOGIN_STATUS_NO_VERSION		0x05
#define ISCSI_LOGIN_STATUS_ISID_ERROR		0x06
#define ISCSI_LOGIN_STATUS_MISSING_FIELDS	0x07
#define ISCSI_LOGIN_STATUS_CONN_ADD_FAILED	0x08
#define ISCSI_LOGIN_STATUS_NO_SESSION_TYPE	0x09
#define ISCSI_LOGIN_STATUS_NO_SESSION		0x0a
#define ISCSI_LOGIN_STATUS_INVALID_REQUEST	0x0b


#define ISCSI_LOGIN_STATUS_TARGET_ERROR		0x00
#define ISCSI_LOGIN_STATUS_SVC_UNAVAILABLE	0x01
#define ISCSI_LOGIN_STATUS_NO_RESOURCES		0x02


struct iscsi_logout {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd1[2];
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd2[8];
	itt_t	 itt;	
	__be16	cid;
	uint8_t rsvd3[2];
	__be32	cmdsn;
	__be32	exp_statsn;
	uint8_t rsvd4[16];
};


#define ISCSI_FLAG_LOGOUT_REASON_MASK	0x7F



#define ISCSI_LOGOUT_REASON_CLOSE_SESSION	0
#define ISCSI_LOGOUT_REASON_CLOSE_CONNECTION	1
#define ISCSI_LOGOUT_REASON_RECOVERY		2
#define ISCSI_LOGOUT_REASON_AEN_REQUEST		3


struct iscsi_logout_rsp {
	uint8_t opcode;
	uint8_t flags;
	uint8_t response;	
	uint8_t rsvd2;
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd3[8];
	itt_t	 itt;	
	__be32	rsvd4;
	__be32	statsn;
	__be32	exp_cmdsn;
	__be32	max_cmdsn;
	__be32	rsvd5;
	__be16	t2wait;
	__be16	t2retain;
	__be32	rsvd6;
};



#define ISCSI_LOGOUT_SUCCESS			0
#define ISCSI_LOGOUT_CID_NOT_FOUND		1
#define ISCSI_LOGOUT_RECOVERY_UNSUPPORTED	2
#define ISCSI_LOGOUT_CLEANUP_FAILED		3


struct iscsi_snack {
	uint8_t opcode;
	uint8_t flags;
	uint8_t rsvd2[2];
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t lun[8];
	itt_t	 itt;
	__be32  ttt;
	uint8_t rsvd3[4];
	__be32  exp_statsn;
	uint8_t rsvd4[8];
	__be32	begrun;
	__be32	runlength;
};


#define ISCSI_FLAG_SNACK_TYPE_DATA		0
#define ISCSI_FLAG_SNACK_TYPE_R2T		0
#define ISCSI_FLAG_SNACK_TYPE_STATUS		1
#define ISCSI_FLAG_SNACK_TYPE_DATA_ACK		2
#define ISCSI_FLAG_SNACK_TYPE_RDATA		3
#define ISCSI_FLAG_SNACK_TYPE_MASK	0x0F	


struct iscsi_reject {
	uint8_t opcode;
	uint8_t flags;
	uint8_t reason;
	uint8_t rsvd2;
	uint8_t hlength;
	uint8_t dlength[3];
	uint8_t rsvd3[8];
	__be32  ffffffff;
	uint8_t rsvd4[4];
	__be32	statsn;
	__be32	exp_cmdsn;
	__be32	max_cmdsn;
	__be32	datasn;
	uint8_t rsvd5[8];
	
};


#define ISCSI_REASON_CMD_BEFORE_LOGIN	1
#define ISCSI_REASON_DATA_DIGEST_ERROR	2
#define ISCSI_REASON_DATA_SNACK_REJECT	3
#define ISCSI_REASON_PROTOCOL_ERROR	4
#define ISCSI_REASON_CMD_NOT_SUPPORTED	5
#define ISCSI_REASON_IMM_CMD_REJECT		6
#define ISCSI_REASON_TASK_IN_PROGRESS	7
#define ISCSI_REASON_INVALID_SNACK		8
#define ISCSI_REASON_BOOKMARK_INVALID	9
#define ISCSI_REASON_BOOKMARK_NO_RESOURCES	10
#define ISCSI_REASON_NEGOTIATION_RESET	11


#define MAX_KEY_VALUE_PAIRS	8192


#define KEY_MAXLEN		64
#define VALUE_MAXLEN		255
#define TARGET_NAME_MAXLEN	VALUE_MAXLEN

#define ISCSI_DEF_MAX_RECV_SEG_LEN		8192
#define ISCSI_MIN_MAX_RECV_SEG_LEN		512
#define ISCSI_MAX_MAX_RECV_SEG_LEN		16777215

#define ISCSI_DEF_FIRST_BURST_LEN		65536
#define ISCSI_MIN_FIRST_BURST_LEN		512
#define ISCSI_MAX_FIRST_BURST_LEN		16777215

#define ISCSI_DEF_MAX_BURST_LEN			262144
#define ISCSI_MIN_MAX_BURST_LEN			512
#define ISCSI_MAX_MAX_BURST_LEN			16777215

#define ISCSI_DEF_TIME2WAIT			2

#define ISCSI_NAME_LEN				224



#endif 
