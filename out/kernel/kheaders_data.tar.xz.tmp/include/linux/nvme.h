

#ifndef _LINUX_NVME_H
#define _LINUX_NVME_H

#include <linux/types.h>
#include <linux/uuid.h>


#define NVMF_NQN_FIELD_LEN	256


#define NVMF_NQN_SIZE		223

#define NVMF_TRSVCID_SIZE	32
#define NVMF_TRADDR_SIZE	256
#define NVMF_TSAS_SIZE		256

#define NVME_DISC_SUBSYS_NAME	"nqn.2014-08.org.nvmexpress.discovery"

#define NVME_RDMA_IP_PORT	4420

#define NVME_NSID_ALL		0xffffffff

enum nvme_subsys_type {
	NVME_NQN_DISC	= 1,		
	NVME_NQN_NVME	= 2,		
};


enum {
	NVMF_ADDR_FAMILY_PCI	= 0,	
	NVMF_ADDR_FAMILY_IP4	= 1,	
	NVMF_ADDR_FAMILY_IP6	= 2,	
	NVMF_ADDR_FAMILY_IB	= 3,	
	NVMF_ADDR_FAMILY_FC	= 4,	
};


enum {
	NVMF_TRTYPE_RDMA	= 1,	
	NVMF_TRTYPE_FC		= 2,	
	NVMF_TRTYPE_LOOP	= 254,	
	NVMF_TRTYPE_MAX,
};


enum {
	NVMF_TREQ_NOT_SPECIFIED	= 0,	
	NVMF_TREQ_REQUIRED	= 1,	
	NVMF_TREQ_NOT_REQUIRED	= 2,	
};


enum {
	NVMF_RDMA_QPTYPE_CONNECTED	= 1, 
	NVMF_RDMA_QPTYPE_DATAGRAM	= 2, 
};


enum {
	NVMF_RDMA_PRTYPE_NOT_SPECIFIED	= 1, 
	NVMF_RDMA_PRTYPE_IB		= 2, 
	NVMF_RDMA_PRTYPE_ROCE		= 3, 
	NVMF_RDMA_PRTYPE_ROCEV2		= 4, 
	NVMF_RDMA_PRTYPE_IWARP		= 5, 
};


enum {
	NVMF_RDMA_CMS_RDMA_CM	= 1, 
};

#define NVME_AQ_DEPTH		32
#define NVME_NR_AEN_COMMANDS	1
#define NVME_AQ_BLK_MQ_DEPTH	(NVME_AQ_DEPTH - NVME_NR_AEN_COMMANDS)


#define NVME_AQ_MQ_TAG_DEPTH	(NVME_AQ_BLK_MQ_DEPTH - 1)

enum {
	NVME_REG_CAP	= 0x0000,	
	NVME_REG_VS	= 0x0008,	
	NVME_REG_INTMS	= 0x000c,	
	NVME_REG_INTMC	= 0x0010,	
	NVME_REG_CC	= 0x0014,	
	NVME_REG_CSTS	= 0x001c,	
	NVME_REG_NSSR	= 0x0020,	
	NVME_REG_AQA	= 0x0024,	
	NVME_REG_ASQ	= 0x0028,	
	NVME_REG_ACQ	= 0x0030,	
	NVME_REG_CMBLOC = 0x0038,	
	NVME_REG_CMBSZ	= 0x003c,	
	NVME_REG_DBS	= 0x1000,	
};

#define NVME_CAP_MQES(cap)	((cap) & 0xffff)
#define NVME_CAP_TIMEOUT(cap)	(((cap) >> 24) & 0xff)
#define NVME_CAP_STRIDE(cap)	(((cap) >> 32) & 0xf)
#define NVME_CAP_NSSRC(cap)	(((cap) >> 36) & 0x1)
#define NVME_CAP_MPSMIN(cap)	(((cap) >> 48) & 0xf)
#define NVME_CAP_MPSMAX(cap)	(((cap) >> 52) & 0xf)

#define NVME_CMB_BIR(cmbloc)	((cmbloc) & 0x7)
#define NVME_CMB_OFST(cmbloc)	(((cmbloc) >> 12) & 0xfffff)

enum {
	NVME_CMBSZ_SQS		= 1 << 0,
	NVME_CMBSZ_CQS		= 1 << 1,
	NVME_CMBSZ_LISTS	= 1 << 2,
	NVME_CMBSZ_RDS		= 1 << 3,
	NVME_CMBSZ_WDS		= 1 << 4,

	NVME_CMBSZ_SZ_SHIFT	= 12,
	NVME_CMBSZ_SZ_MASK	= 0xfffff,

	NVME_CMBSZ_SZU_SHIFT	= 8,
	NVME_CMBSZ_SZU_MASK	= 0xf,
};


#define NVME_NVM_IOSQES		6
#define NVME_NVM_IOCQES		4

enum {
	NVME_CC_ENABLE		= 1 << 0,
	NVME_CC_CSS_NVM		= 0 << 4,
	NVME_CC_EN_SHIFT	= 0,
	NVME_CC_CSS_SHIFT	= 4,
	NVME_CC_MPS_SHIFT	= 7,
	NVME_CC_AMS_SHIFT	= 11,
	NVME_CC_SHN_SHIFT	= 14,
	NVME_CC_IOSQES_SHIFT	= 16,
	NVME_CC_IOCQES_SHIFT	= 20,
	NVME_CC_AMS_RR		= 0 << NVME_CC_AMS_SHIFT,
	NVME_CC_AMS_WRRU	= 1 << NVME_CC_AMS_SHIFT,
	NVME_CC_AMS_VS		= 7 << NVME_CC_AMS_SHIFT,
	NVME_CC_SHN_NONE	= 0 << NVME_CC_SHN_SHIFT,
	NVME_CC_SHN_NORMAL	= 1 << NVME_CC_SHN_SHIFT,
	NVME_CC_SHN_ABRUPT	= 2 << NVME_CC_SHN_SHIFT,
	NVME_CC_SHN_MASK	= 3 << NVME_CC_SHN_SHIFT,
	NVME_CC_IOSQES		= NVME_NVM_IOSQES << NVME_CC_IOSQES_SHIFT,
	NVME_CC_IOCQES		= NVME_NVM_IOCQES << NVME_CC_IOCQES_SHIFT,
	NVME_CSTS_RDY		= 1 << 0,
	NVME_CSTS_CFS		= 1 << 1,
	NVME_CSTS_NSSRO		= 1 << 4,
	NVME_CSTS_PP		= 1 << 5,
	NVME_CSTS_SHST_NORMAL	= 0 << 2,
	NVME_CSTS_SHST_OCCUR	= 1 << 2,
	NVME_CSTS_SHST_CMPLT	= 2 << 2,
	NVME_CSTS_SHST_MASK	= 3 << 2,
};

struct nvme_id_power_state {
	__le16			max_power;	
	__u8			rsvd2;
	__u8			flags;
	__le32			entry_lat;	
	__le32			exit_lat;	
	__u8			read_tput;
	__u8			read_lat;
	__u8			write_tput;
	__u8			write_lat;
	__le16			idle_power;
	__u8			idle_scale;
	__u8			rsvd19;
	__le16			active_power;
	__u8			active_work_scale;
	__u8			rsvd23[9];
};

enum {
	NVME_PS_FLAGS_MAX_POWER_SCALE	= 1 << 0,
	NVME_PS_FLAGS_NON_OP_STATE	= 1 << 1,
};

struct nvme_id_ctrl {
	__le16			vid;
	__le16			ssvid;
	char			sn[20];
	char			mn[40];
	char			fr[8];
	__u8			rab;
	__u8			ieee[3];
	__u8			cmic;
	__u8			mdts;
	__le16			cntlid;
	__le32			ver;
	__le32			rtd3r;
	__le32			rtd3e;
	__le32			oaes;
	__le32			ctratt;
	__u8			rsvd100[156];
	__le16			oacs;
	__u8			acl;
	__u8			aerl;
	__u8			frmw;
	__u8			lpa;
	__u8			elpe;
	__u8			npss;
	__u8			avscc;
	__u8			apsta;
	__le16			wctemp;
	__le16			cctemp;
	__le16			mtfa;
	__le32			hmpre;
	__le32			hmmin;
	__u8			tnvmcap[16];
	__u8			unvmcap[16];
	__le32			rpmbs;
	__le16			edstt;
	__u8			dsto;
	__u8			fwug;
	__le16			kas;
	__le16			hctma;
	__le16			mntmt;
	__le16			mxtmt;
	__le32			sanicap;
	__le32			hmminds;
	__le16			hmmaxd;
	__u8			rsvd338[4];
	__u8			anatt;
	__u8			anacap;
	__le32			anagrpmax;
	__le32			nanagrpid;
	__u8			rsvd352[160];
	__u8			sqes;
	__u8			cqes;
	__le16			maxcmd;
	__le32			nn;
	__le16			oncs;
	__le16			fuses;
	__u8			fna;
	__u8			vwc;
	__le16			awun;
	__le16			awupf;
	__u8			nvscc;
	__u8			nwpc;
	__le16			acwu;
	__u8			rsvd534[2];
	__le32			sgls;
	__le32			mnan;
	__u8			rsvd544[224];
	char			subnqn[256];
	__u8			rsvd1024[768];
	__le32			ioccsz;
	__le32			iorcsz;
	__le16			icdoff;
	__u8			ctrattr;
	__u8			msdbd;
	__u8			rsvd1804[244];
	struct nvme_id_power_state	psd[32];
	__u8			vs[1024];
};

enum {
	NVME_CTRL_ONCS_COMPARE			= 1 << 0,
	NVME_CTRL_ONCS_WRITE_UNCORRECTABLE	= 1 << 1,
	NVME_CTRL_ONCS_DSM			= 1 << 2,
	NVME_CTRL_ONCS_WRITE_ZEROES		= 1 << 3,
	NVME_CTRL_ONCS_TIMESTAMP		= 1 << 6,
	NVME_CTRL_VWC_PRESENT			= 1 << 0,
	NVME_CTRL_OACS_SEC_SUPP                 = 1 << 0,
	NVME_CTRL_OACS_DIRECTIVES		= 1 << 5,
	NVME_CTRL_OACS_DBBUF_SUPP		= 1 << 8,
	NVME_CTRL_LPA_CMD_EFFECTS_LOG		= 1 << 1,
};

struct nvme_lbaf {
	__le16			ms;
	__u8			ds;
	__u8			rp;
};

struct nvme_id_ns {
	__le64			nsze;
	__le64			ncap;
	__le64			nuse;
	__u8			nsfeat;
	__u8			nlbaf;
	__u8			flbas;
	__u8			mc;
	__u8			dpc;
	__u8			dps;
	__u8			nmic;
	__u8			rescap;
	__u8			fpi;
	__u8			rsvd33;
	__le16			nawun;
	__le16			nawupf;
	__le16			nacwu;
	__le16			nabsn;
	__le16			nabo;
	__le16			nabspf;
	__le16			noiob;
	__u8			nvmcap[16];
	__u8			rsvd64[28];
	__le32			anagrpid;
	__u8			rsvd96[3];
	__u8			nsattr;
	__u8			rsvd100[4];
	__u8			nguid[16];
	__u8			eui64[8];
	struct nvme_lbaf	lbaf[16];
	__u8			rsvd192[192];
	__u8			vs[3712];
};

enum {
	NVME_ID_CNS_NS			= 0x00,
	NVME_ID_CNS_CTRL		= 0x01,
	NVME_ID_CNS_NS_ACTIVE_LIST	= 0x02,
	NVME_ID_CNS_NS_DESC_LIST	= 0x03,
	NVME_ID_CNS_NS_PRESENT_LIST	= 0x10,
	NVME_ID_CNS_NS_PRESENT		= 0x11,
	NVME_ID_CNS_CTRL_NS_LIST	= 0x12,
	NVME_ID_CNS_CTRL_LIST		= 0x13,
};

enum {
	NVME_DIR_IDENTIFY		= 0x00,
	NVME_DIR_STREAMS		= 0x01,
	NVME_DIR_SND_ID_OP_ENABLE	= 0x01,
	NVME_DIR_SND_ST_OP_REL_ID	= 0x01,
	NVME_DIR_SND_ST_OP_REL_RSC	= 0x02,
	NVME_DIR_RCV_ID_OP_PARAM	= 0x01,
	NVME_DIR_RCV_ST_OP_PARAM	= 0x01,
	NVME_DIR_RCV_ST_OP_STATUS	= 0x02,
	NVME_DIR_RCV_ST_OP_RESOURCE	= 0x03,
	NVME_DIR_ENDIR			= 0x01,
};

enum {
	NVME_NS_FEAT_THIN	= 1 << 0,
	NVME_NS_FLBAS_LBA_MASK	= 0xf,
	NVME_NS_FLBAS_META_EXT	= 0x10,
	NVME_LBAF_RP_BEST	= 0,
	NVME_LBAF_RP_BETTER	= 1,
	NVME_LBAF_RP_GOOD	= 2,
	NVME_LBAF_RP_DEGRADED	= 3,
	NVME_NS_DPC_PI_LAST	= 1 << 4,
	NVME_NS_DPC_PI_FIRST	= 1 << 3,
	NVME_NS_DPC_PI_TYPE3	= 1 << 2,
	NVME_NS_DPC_PI_TYPE2	= 1 << 1,
	NVME_NS_DPC_PI_TYPE1	= 1 << 0,
	NVME_NS_DPS_PI_FIRST	= 1 << 3,
	NVME_NS_DPS_PI_MASK	= 0x7,
	NVME_NS_DPS_PI_TYPE1	= 1,
	NVME_NS_DPS_PI_TYPE2	= 2,
	NVME_NS_DPS_PI_TYPE3	= 3,
};

struct nvme_ns_id_desc {
	__u8 nidt;
	__u8 nidl;
	__le16 reserved;
};

#define NVME_NIDT_EUI64_LEN	8
#define NVME_NIDT_NGUID_LEN	16
#define NVME_NIDT_UUID_LEN	16

enum {
	NVME_NIDT_EUI64		= 0x01,
	NVME_NIDT_NGUID		= 0x02,
	NVME_NIDT_UUID		= 0x03,
};

struct nvme_smart_log {
	__u8			critical_warning;
	__u8			temperature[2];
	__u8			avail_spare;
	__u8			spare_thresh;
	__u8			percent_used;
	__u8			rsvd6[26];
	__u8			data_units_read[16];
	__u8			data_units_written[16];
	__u8			host_reads[16];
	__u8			host_writes[16];
	__u8			ctrl_busy_time[16];
	__u8			power_cycles[16];
	__u8			power_on_hours[16];
	__u8			unsafe_shutdowns[16];
	__u8			media_errors[16];
	__u8			num_err_log_entries[16];
	__le32			warning_temp_time;
	__le32			critical_comp_time;
	__le16			temp_sensor[8];
	__u8			rsvd216[296];
};

struct nvme_fw_slot_info_log {
	__u8			afi;
	__u8			rsvd1[7];
	__le64			frs[7];
	__u8			rsvd64[448];
};

enum {
	NVME_CMD_EFFECTS_CSUPP		= 1 << 0,
	NVME_CMD_EFFECTS_LBCC		= 1 << 1,
	NVME_CMD_EFFECTS_NCC		= 1 << 2,
	NVME_CMD_EFFECTS_NIC		= 1 << 3,
	NVME_CMD_EFFECTS_CCC		= 1 << 4,
	NVME_CMD_EFFECTS_CSE_MASK	= 3 << 16,
};

struct nvme_effects_log {
	__le32 acs[256];
	__le32 iocs[256];
	__u8   resv[2048];
};

enum nvme_ana_state {
	NVME_ANA_OPTIMIZED		= 0x01,
	NVME_ANA_NONOPTIMIZED		= 0x02,
	NVME_ANA_INACCESSIBLE		= 0x03,
	NVME_ANA_PERSISTENT_LOSS	= 0x04,
	NVME_ANA_CHANGE			= 0x0f,
};

struct nvme_ana_group_desc {
	__le32	grpid;
	__le32	nnsids;
	__le64	chgcnt;
	__u8	state;
	__u8	rsvd17[15];
	__le32	nsids[];
};


#define NVME_ANA_LOG_RGO	(1 << 0)

struct nvme_ana_rsp_hdr {
	__le64	chgcnt;
	__le16	ngrps;
	__le16	rsvd10[3];
};

enum {
	NVME_SMART_CRIT_SPARE		= 1 << 0,
	NVME_SMART_CRIT_TEMPERATURE	= 1 << 1,
	NVME_SMART_CRIT_RELIABILITY	= 1 << 2,
	NVME_SMART_CRIT_MEDIA		= 1 << 3,
	NVME_SMART_CRIT_VOLATILE_MEMORY	= 1 << 4,
};

enum {
	NVME_AER_ERROR			= 0,
	NVME_AER_SMART			= 1,
	NVME_AER_NOTICE			= 2,
	NVME_AER_CSS			= 6,
	NVME_AER_VS			= 7,
};

enum {
	NVME_AER_NOTICE_NS_CHANGED	= 0x00,
	NVME_AER_NOTICE_FW_ACT_STARTING = 0x01,
	NVME_AER_NOTICE_ANA		= 0x03,
};

enum {
	NVME_AEN_CFG_NS_ATTR		= 1 << 8,
	NVME_AEN_CFG_FW_ACT		= 1 << 9,
	NVME_AEN_CFG_ANA_CHANGE		= 1 << 11,
};

struct nvme_lba_range_type {
	__u8			type;
	__u8			attributes;
	__u8			rsvd2[14];
	__u64			slba;
	__u64			nlb;
	__u8			guid[16];
	__u8			rsvd48[16];
};

enum {
	NVME_LBART_TYPE_FS	= 0x01,
	NVME_LBART_TYPE_RAID	= 0x02,
	NVME_LBART_TYPE_CACHE	= 0x03,
	NVME_LBART_TYPE_SWAP	= 0x04,

	NVME_LBART_ATTRIB_TEMP	= 1 << 0,
	NVME_LBART_ATTRIB_HIDE	= 1 << 1,
};

struct nvme_reservation_status {
	__le32	gen;
	__u8	rtype;
	__u8	regctl[2];
	__u8	resv5[2];
	__u8	ptpls;
	__u8	resv10[13];
	struct {
		__le16	cntlid;
		__u8	rcsts;
		__u8	resv3[5];
		__le64	hostid;
		__le64	rkey;
	} regctl_ds[];
};

enum nvme_async_event_type {
	NVME_AER_TYPE_ERROR	= 0,
	NVME_AER_TYPE_SMART	= 1,
	NVME_AER_TYPE_NOTICE	= 2,
};



enum nvme_opcode {
	nvme_cmd_flush		= 0x00,
	nvme_cmd_write		= 0x01,
	nvme_cmd_read		= 0x02,
	nvme_cmd_write_uncor	= 0x04,
	nvme_cmd_compare	= 0x05,
	nvme_cmd_write_zeroes	= 0x08,
	nvme_cmd_dsm		= 0x09,
	nvme_cmd_resv_register	= 0x0d,
	nvme_cmd_resv_report	= 0x0e,
	nvme_cmd_resv_acquire	= 0x11,
	nvme_cmd_resv_release	= 0x15,
};


enum {
	NVME_SGL_FMT_ADDRESS		= 0x00,
	NVME_SGL_FMT_OFFSET		= 0x01,
	NVME_SGL_FMT_TRANSPORT_A	= 0x0A,
	NVME_SGL_FMT_INVALIDATE		= 0x0f,
};


enum {
	NVME_SGL_FMT_DATA_DESC		= 0x00,
	NVME_SGL_FMT_SEG_DESC		= 0x02,
	NVME_SGL_FMT_LAST_SEG_DESC	= 0x03,
	NVME_KEY_SGL_FMT_DATA_DESC	= 0x04,
	NVME_TRANSPORT_SGL_DATA_DESC	= 0x05,
};

struct nvme_sgl_desc {
	__le64	addr;
	__le32	length;
	__u8	rsvd[3];
	__u8	type;
};

struct nvme_keyed_sgl_desc {
	__le64	addr;
	__u8	length[3];
	__u8	key[4];
	__u8	type;
};

union nvme_data_ptr {
	struct {
		__le64	prp1;
		__le64	prp2;
	};
	struct nvme_sgl_desc	sgl;
	struct nvme_keyed_sgl_desc ksgl;
};


enum {
	NVME_CMD_FUSE_FIRST	= (1 << 0),
	NVME_CMD_FUSE_SECOND	= (1 << 1),

	NVME_CMD_SGL_METABUF	= (1 << 6),
	NVME_CMD_SGL_METASEG	= (1 << 7),
	NVME_CMD_SGL_ALL	= NVME_CMD_SGL_METABUF | NVME_CMD_SGL_METASEG,
};

struct nvme_common_command {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__le32			nsid;
	__le32			cdw2[2];
	__le64			metadata;
	union nvme_data_ptr	dptr;
	__le32			cdw10[6];
};

struct nvme_rw_command {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__le32			nsid;
	__u64			rsvd2;
	__le64			metadata;
	union nvme_data_ptr	dptr;
	__le64			slba;
	__le16			length;
	__le16			control;
	__le32			dsmgmt;
	__le32			reftag;
	__le16			apptag;
	__le16			appmask;
};

enum {
	NVME_RW_LR			= 1 << 15,
	NVME_RW_FUA			= 1 << 14,
	NVME_RW_DSM_FREQ_UNSPEC		= 0,
	NVME_RW_DSM_FREQ_TYPICAL	= 1,
	NVME_RW_DSM_FREQ_RARE		= 2,
	NVME_RW_DSM_FREQ_READS		= 3,
	NVME_RW_DSM_FREQ_WRITES		= 4,
	NVME_RW_DSM_FREQ_RW		= 5,
	NVME_RW_DSM_FREQ_ONCE		= 6,
	NVME_RW_DSM_FREQ_PREFETCH	= 7,
	NVME_RW_DSM_FREQ_TEMP		= 8,
	NVME_RW_DSM_LATENCY_NONE	= 0 << 4,
	NVME_RW_DSM_LATENCY_IDLE	= 1 << 4,
	NVME_RW_DSM_LATENCY_NORM	= 2 << 4,
	NVME_RW_DSM_LATENCY_LOW		= 3 << 4,
	NVME_RW_DSM_SEQ_REQ		= 1 << 6,
	NVME_RW_DSM_COMPRESSED		= 1 << 7,
	NVME_RW_PRINFO_PRCHK_REF	= 1 << 10,
	NVME_RW_PRINFO_PRCHK_APP	= 1 << 11,
	NVME_RW_PRINFO_PRCHK_GUARD	= 1 << 12,
	NVME_RW_PRINFO_PRACT		= 1 << 13,
	NVME_RW_DTYPE_STREAMS		= 1 << 4,
};

struct nvme_dsm_cmd {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__le32			nsid;
	__u64			rsvd2[2];
	union nvme_data_ptr	dptr;
	__le32			nr;
	__le32			attributes;
	__u32			rsvd12[4];
};

enum {
	NVME_DSMGMT_IDR		= 1 << 0,
	NVME_DSMGMT_IDW		= 1 << 1,
	NVME_DSMGMT_AD		= 1 << 2,
};

#define NVME_DSM_MAX_RANGES	256

struct nvme_dsm_range {
	__le32			cattr;
	__le32			nlb;
	__le64			slba;
};

struct nvme_write_zeroes_cmd {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__le32			nsid;
	__u64			rsvd2;
	__le64			metadata;
	union nvme_data_ptr	dptr;
	__le64			slba;
	__le16			length;
	__le16			control;
	__le32			dsmgmt;
	__le32			reftag;
	__le16			apptag;
	__le16			appmask;
};



struct nvme_feat_auto_pst {
	__le64 entries[32];
};

enum {
	NVME_HOST_MEM_ENABLE	= (1 << 0),
	NVME_HOST_MEM_RETURN	= (1 << 1),
};



enum nvme_admin_opcode {
	nvme_admin_delete_sq		= 0x00,
	nvme_admin_create_sq		= 0x01,
	nvme_admin_get_log_page		= 0x02,
	nvme_admin_delete_cq		= 0x04,
	nvme_admin_create_cq		= 0x05,
	nvme_admin_identify		= 0x06,
	nvme_admin_abort_cmd		= 0x08,
	nvme_admin_set_features		= 0x09,
	nvme_admin_get_features		= 0x0a,
	nvme_admin_async_event		= 0x0c,
	nvme_admin_ns_mgmt		= 0x0d,
	nvme_admin_activate_fw		= 0x10,
	nvme_admin_download_fw		= 0x11,
	nvme_admin_ns_attach		= 0x15,
	nvme_admin_keep_alive		= 0x18,
	nvme_admin_directive_send	= 0x19,
	nvme_admin_directive_recv	= 0x1a,
	nvme_admin_dbbuf		= 0x7C,
	nvme_admin_format_nvm		= 0x80,
	nvme_admin_security_send	= 0x81,
	nvme_admin_security_recv	= 0x82,
	nvme_admin_sanitize_nvm		= 0x84,
};

enum {
	NVME_QUEUE_PHYS_CONTIG	= (1 << 0),
	NVME_CQ_IRQ_ENABLED	= (1 << 1),
	NVME_SQ_PRIO_URGENT	= (0 << 1),
	NVME_SQ_PRIO_HIGH	= (1 << 1),
	NVME_SQ_PRIO_MEDIUM	= (2 << 1),
	NVME_SQ_PRIO_LOW	= (3 << 1),
	NVME_FEAT_ARBITRATION	= 0x01,
	NVME_FEAT_POWER_MGMT	= 0x02,
	NVME_FEAT_LBA_RANGE	= 0x03,
	NVME_FEAT_TEMP_THRESH	= 0x04,
	NVME_FEAT_ERR_RECOVERY	= 0x05,
	NVME_FEAT_VOLATILE_WC	= 0x06,
	NVME_FEAT_NUM_QUEUES	= 0x07,
	NVME_FEAT_IRQ_COALESCE	= 0x08,
	NVME_FEAT_IRQ_CONFIG	= 0x09,
	NVME_FEAT_WRITE_ATOMIC	= 0x0a,
	NVME_FEAT_ASYNC_EVENT	= 0x0b,
	NVME_FEAT_AUTO_PST	= 0x0c,
	NVME_FEAT_HOST_MEM_BUF	= 0x0d,
	NVME_FEAT_TIMESTAMP	= 0x0e,
	NVME_FEAT_KATO		= 0x0f,
	NVME_FEAT_HCTM		= 0x10,
	NVME_FEAT_NOPSC		= 0x11,
	NVME_FEAT_RRL		= 0x12,
	NVME_FEAT_PLM_CONFIG	= 0x13,
	NVME_FEAT_PLM_WINDOW	= 0x14,
	NVME_FEAT_SW_PROGRESS	= 0x80,
	NVME_FEAT_HOST_ID	= 0x81,
	NVME_FEAT_RESV_MASK	= 0x82,
	NVME_FEAT_RESV_PERSIST	= 0x83,
	NVME_FEAT_WRITE_PROTECT	= 0x84,
	NVME_LOG_ERROR		= 0x01,
	NVME_LOG_SMART		= 0x02,
	NVME_LOG_FW_SLOT	= 0x03,
	NVME_LOG_CHANGED_NS	= 0x04,
	NVME_LOG_CMD_EFFECTS	= 0x05,
	NVME_LOG_ANA		= 0x0c,
	NVME_LOG_DISC		= 0x70,
	NVME_LOG_RESERVATION	= 0x80,
	NVME_FWACT_REPL		= (0 << 3),
	NVME_FWACT_REPL_ACTV	= (1 << 3),
	NVME_FWACT_ACTV		= (2 << 3),
};


enum {
	NVME_NS_NO_WRITE_PROTECT = 0,
	NVME_NS_WRITE_PROTECT,
	NVME_NS_WRITE_PROTECT_POWER_CYCLE,
	NVME_NS_WRITE_PROTECT_PERMANENT,
};

#define NVME_MAX_CHANGED_NAMESPACES	1024

struct nvme_identify {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__le32			nsid;
	__u64			rsvd2[2];
	union nvme_data_ptr	dptr;
	__u8			cns;
	__u8			rsvd3;
	__le16			ctrlid;
	__u32			rsvd11[5];
};

#define NVME_IDENTIFY_DATA_SIZE 4096

struct nvme_features {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__le32			nsid;
	__u64			rsvd2[2];
	union nvme_data_ptr	dptr;
	__le32			fid;
	__le32			dword11;
	__le32                  dword12;
	__le32                  dword13;
	__le32                  dword14;
	__le32                  dword15;
};

struct nvme_host_mem_buf_desc {
	__le64			addr;
	__le32			size;
	__u32			rsvd;
};

struct nvme_create_cq {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__u32			rsvd1[5];
	__le64			prp1;
	__u64			rsvd8;
	__le16			cqid;
	__le16			qsize;
	__le16			cq_flags;
	__le16			irq_vector;
	__u32			rsvd12[4];
};

struct nvme_create_sq {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__u32			rsvd1[5];
	__le64			prp1;
	__u64			rsvd8;
	__le16			sqid;
	__le16			qsize;
	__le16			sq_flags;
	__le16			cqid;
	__u32			rsvd12[4];
};

struct nvme_delete_queue {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__u32			rsvd1[9];
	__le16			qid;
	__u16			rsvd10;
	__u32			rsvd11[5];
};

struct nvme_abort_cmd {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__u32			rsvd1[9];
	__le16			sqid;
	__u16			cid;
	__u32			rsvd11[5];
};

struct nvme_download_firmware {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__u32			rsvd1[5];
	union nvme_data_ptr	dptr;
	__le32			numd;
	__le32			offset;
	__u32			rsvd12[4];
};

struct nvme_format_cmd {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__le32			nsid;
	__u64			rsvd2[4];
	__le32			cdw10;
	__u32			rsvd11[5];
};

struct nvme_get_log_page_command {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__le32			nsid;
	__u64			rsvd2[2];
	union nvme_data_ptr	dptr;
	__u8			lid;
	__u8			lsp; 
	__le16			numdl;
	__le16			numdu;
	__u16			rsvd11;
	__le32			lpol;
	__le32			lpou;
	__u32			rsvd14[2];
};

struct nvme_directive_cmd {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__le32			nsid;
	__u64			rsvd2[2];
	union nvme_data_ptr	dptr;
	__le32			numd;
	__u8			doper;
	__u8			dtype;
	__le16			dspec;
	__u8			endir;
	__u8			tdtype;
	__u16			rsvd15;

	__u32			rsvd16[3];
};


enum nvmf_fabrics_opcode {
	nvme_fabrics_command		= 0x7f,
};

enum nvmf_capsule_command {
	nvme_fabrics_type_property_set	= 0x00,
	nvme_fabrics_type_connect	= 0x01,
	nvme_fabrics_type_property_get	= 0x04,
};

struct nvmf_common_command {
	__u8	opcode;
	__u8	resv1;
	__u16	command_id;
	__u8	fctype;
	__u8	resv2[35];
	__u8	ts[24];
};


#define NVME_CNTLID_MIN		1
#define NVME_CNTLID_MAX		0xffef
#define NVME_CNTLID_DYNAMIC	0xffff

#define MAX_DISC_LOGS	255


struct nvmf_disc_rsp_page_entry {
	__u8		trtype;
	__u8		adrfam;
	__u8		subtype;
	__u8		treq;
	__le16		portid;
	__le16		cntlid;
	__le16		asqsz;
	__u8		resv8[22];
	char		trsvcid[NVMF_TRSVCID_SIZE];
	__u8		resv64[192];
	char		subnqn[NVMF_NQN_FIELD_LEN];
	char		traddr[NVMF_TRADDR_SIZE];
	union tsas {
		char		common[NVMF_TSAS_SIZE];
		struct rdma {
			__u8	qptype;
			__u8	prtype;
			__u8	cms;
			__u8	resv3[5];
			__u16	pkey;
			__u8	resv10[246];
		} rdma;
	} tsas;
};


struct nvmf_disc_rsp_page_hdr {
	__le64		genctr;
	__le64		numrec;
	__le16		recfmt;
	__u8		resv14[1006];
	struct nvmf_disc_rsp_page_entry entries[0];
};

struct nvmf_connect_command {
	__u8		opcode;
	__u8		resv1;
	__u16		command_id;
	__u8		fctype;
	__u8		resv2[19];
	union nvme_data_ptr dptr;
	__le16		recfmt;
	__le16		qid;
	__le16		sqsize;
	__u8		cattr;
	__u8		resv3;
	__le32		kato;
	__u8		resv4[12];
};

struct nvmf_connect_data {
	uuid_t		hostid;
	__le16		cntlid;
	char		resv4[238];
	char		subsysnqn[NVMF_NQN_FIELD_LEN];
	char		hostnqn[NVMF_NQN_FIELD_LEN];
	char		resv5[256];
};

struct nvmf_property_set_command {
	__u8		opcode;
	__u8		resv1;
	__u16		command_id;
	__u8		fctype;
	__u8		resv2[35];
	__u8		attrib;
	__u8		resv3[3];
	__le32		offset;
	__le64		value;
	__u8		resv4[8];
};

struct nvmf_property_get_command {
	__u8		opcode;
	__u8		resv1;
	__u16		command_id;
	__u8		fctype;
	__u8		resv2[35];
	__u8		attrib;
	__u8		resv3[3];
	__le32		offset;
	__u8		resv4[16];
};

struct nvme_dbbuf {
	__u8			opcode;
	__u8			flags;
	__u16			command_id;
	__u32			rsvd1[5];
	__le64			prp1;
	__le64			prp2;
	__u32			rsvd12[6];
};

struct streams_directive_params {
	__le16	msl;
	__le16	nssa;
	__le16	nsso;
	__u8	rsvd[10];
	__le32	sws;
	__le16	sgs;
	__le16	nsa;
	__le16	nso;
	__u8	rsvd2[6];
};

struct nvme_command {
	union {
		struct nvme_common_command common;
		struct nvme_rw_command rw;
		struct nvme_identify identify;
		struct nvme_features features;
		struct nvme_create_cq create_cq;
		struct nvme_create_sq create_sq;
		struct nvme_delete_queue delete_queue;
		struct nvme_download_firmware dlfw;
		struct nvme_format_cmd format;
		struct nvme_dsm_cmd dsm;
		struct nvme_write_zeroes_cmd write_zeroes;
		struct nvme_abort_cmd abort;
		struct nvme_get_log_page_command get_log_page;
		struct nvmf_common_command fabrics;
		struct nvmf_connect_command connect;
		struct nvmf_property_set_command prop_set;
		struct nvmf_property_get_command prop_get;
		struct nvme_dbbuf dbbuf;
		struct nvme_directive_cmd directive;
	};
};

static inline bool nvme_is_write(struct nvme_command *cmd)
{
	
	if (unlikely(cmd->common.opcode == nvme_fabrics_command))
		return cmd->fabrics.fctype & 1;
	return cmd->common.opcode & 1;
}

enum {
	
	NVME_SC_SUCCESS			= 0x0,
	NVME_SC_INVALID_OPCODE		= 0x1,
	NVME_SC_INVALID_FIELD		= 0x2,
	NVME_SC_CMDID_CONFLICT		= 0x3,
	NVME_SC_DATA_XFER_ERROR		= 0x4,
	NVME_SC_POWER_LOSS		= 0x5,
	NVME_SC_INTERNAL		= 0x6,
	NVME_SC_ABORT_REQ		= 0x7,
	NVME_SC_ABORT_QUEUE		= 0x8,
	NVME_SC_FUSED_FAIL		= 0x9,
	NVME_SC_FUSED_MISSING		= 0xa,
	NVME_SC_INVALID_NS		= 0xb,
	NVME_SC_CMD_SEQ_ERROR		= 0xc,
	NVME_SC_SGL_INVALID_LAST	= 0xd,
	NVME_SC_SGL_INVALID_COUNT	= 0xe,
	NVME_SC_SGL_INVALID_DATA	= 0xf,
	NVME_SC_SGL_INVALID_METADATA	= 0x10,
	NVME_SC_SGL_INVALID_TYPE	= 0x11,

	NVME_SC_SGL_INVALID_OFFSET	= 0x16,
	NVME_SC_SGL_INVALID_SUBTYPE	= 0x17,

	NVME_SC_NS_WRITE_PROTECTED	= 0x20,

	NVME_SC_LBA_RANGE		= 0x80,
	NVME_SC_CAP_EXCEEDED		= 0x81,
	NVME_SC_NS_NOT_READY		= 0x82,
	NVME_SC_RESERVATION_CONFLICT	= 0x83,

	
	NVME_SC_CQ_INVALID		= 0x100,
	NVME_SC_QID_INVALID		= 0x101,
	NVME_SC_QUEUE_SIZE		= 0x102,
	NVME_SC_ABORT_LIMIT		= 0x103,
	NVME_SC_ABORT_MISSING		= 0x104,
	NVME_SC_ASYNC_LIMIT		= 0x105,
	NVME_SC_FIRMWARE_SLOT		= 0x106,
	NVME_SC_FIRMWARE_IMAGE		= 0x107,
	NVME_SC_INVALID_VECTOR		= 0x108,
	NVME_SC_INVALID_LOG_PAGE	= 0x109,
	NVME_SC_INVALID_FORMAT		= 0x10a,
	NVME_SC_FW_NEEDS_CONV_RESET	= 0x10b,
	NVME_SC_INVALID_QUEUE		= 0x10c,
	NVME_SC_FEATURE_NOT_SAVEABLE	= 0x10d,
	NVME_SC_FEATURE_NOT_CHANGEABLE	= 0x10e,
	NVME_SC_FEATURE_NOT_PER_NS	= 0x10f,
	NVME_SC_FW_NEEDS_SUBSYS_RESET	= 0x110,
	NVME_SC_FW_NEEDS_RESET		= 0x111,
	NVME_SC_FW_NEEDS_MAX_TIME	= 0x112,
	NVME_SC_FW_ACIVATE_PROHIBITED	= 0x113,
	NVME_SC_OVERLAPPING_RANGE	= 0x114,
	NVME_SC_NS_INSUFFICENT_CAP	= 0x115,
	NVME_SC_NS_ID_UNAVAILABLE	= 0x116,
	NVME_SC_NS_ALREADY_ATTACHED	= 0x118,
	NVME_SC_NS_IS_PRIVATE		= 0x119,
	NVME_SC_NS_NOT_ATTACHED		= 0x11a,
	NVME_SC_THIN_PROV_NOT_SUPP	= 0x11b,
	NVME_SC_CTRL_LIST_INVALID	= 0x11c,

	
	NVME_SC_BAD_ATTRIBUTES		= 0x180,
	NVME_SC_INVALID_PI		= 0x181,
	NVME_SC_READ_ONLY		= 0x182,
	NVME_SC_ONCS_NOT_SUPPORTED	= 0x183,

	
	NVME_SC_CONNECT_FORMAT		= 0x180,
	NVME_SC_CONNECT_CTRL_BUSY	= 0x181,
	NVME_SC_CONNECT_INVALID_PARAM	= 0x182,
	NVME_SC_CONNECT_RESTART_DISC	= 0x183,
	NVME_SC_CONNECT_INVALID_HOST	= 0x184,

	NVME_SC_DISCOVERY_RESTART	= 0x190,
	NVME_SC_AUTH_REQUIRED		= 0x191,

	
	NVME_SC_WRITE_FAULT		= 0x280,
	NVME_SC_READ_ERROR		= 0x281,
	NVME_SC_GUARD_CHECK		= 0x282,
	NVME_SC_APPTAG_CHECK		= 0x283,
	NVME_SC_REFTAG_CHECK		= 0x284,
	NVME_SC_COMPARE_FAILED		= 0x285,
	NVME_SC_ACCESS_DENIED		= 0x286,
	NVME_SC_UNWRITTEN_BLOCK		= 0x287,

	
	NVME_SC_ANA_PERSISTENT_LOSS	= 0x301,
	NVME_SC_ANA_INACCESSIBLE	= 0x302,
	NVME_SC_ANA_TRANSITION		= 0x303,
	NVME_SC_HOST_PATH_ERROR		= 0x370,

	NVME_SC_DNR			= 0x4000,
};

struct nvme_completion {
	
	union nvme_result {
		__le16	u16;
		__le32	u32;
		__le64	u64;
	} result;
	__le16	sq_head;	
	__le16	sq_id;		
	__u16	command_id;	
	__le16	status;		
};

#define NVME_VS(major, minor, tertiary) \
	(((major) << 16) | ((minor) << 8) | (tertiary))

#define NVME_MAJOR(ver)		((ver) >> 16)
#define NVME_MINOR(ver)		(((ver) >> 8) & 0xff)
#define NVME_TERTIARY(ver)	((ver) & 0xff)

#endif 
