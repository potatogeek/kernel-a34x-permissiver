

#ifndef CFCTRL_H_
#define CFCTRL_H_
#include <net/caif/caif_layer.h>
#include <net/caif/cfsrvl.h>


enum cfctrl_cmd {
	CFCTRL_CMD_LINK_SETUP = 0,
	CFCTRL_CMD_LINK_DESTROY = 1,
	CFCTRL_CMD_LINK_ERR = 2,
	CFCTRL_CMD_ENUM = 3,
	CFCTRL_CMD_SLEEP = 4,
	CFCTRL_CMD_WAKE = 5,
	CFCTRL_CMD_LINK_RECONF = 6,
	CFCTRL_CMD_START_REASON = 7,
	CFCTRL_CMD_RADIO_SET = 8,
	CFCTRL_CMD_MODEM_SET = 9,
	CFCTRL_CMD_MASK = 0xf
};


enum cfctrl_srv {
	CFCTRL_SRV_DECM = 0,
	CFCTRL_SRV_VEI = 1,
	CFCTRL_SRV_VIDEO = 2,
	CFCTRL_SRV_DBG = 3,
	CFCTRL_SRV_DATAGRAM = 4,
	CFCTRL_SRV_RFM = 5,
	CFCTRL_SRV_UTIL = 6,
	CFCTRL_SRV_MASK = 0xf
};

#define CFCTRL_RSP_BIT 0x20
#define CFCTRL_ERR_BIT 0x10

struct cfctrl_rsp {
	void (*linksetup_rsp)(struct cflayer *layer, u8 linkid,
			      enum cfctrl_srv serv, u8 phyid,
			      struct cflayer *adapt_layer);
	void (*linkdestroy_rsp)(struct cflayer *layer, u8 linkid);
	void (*linkerror_ind)(void);
	void (*enum_rsp)(void);
	void (*sleep_rsp)(void);
	void (*wake_rsp)(void);
	void (*restart_rsp)(void);
	void (*radioset_rsp)(void);
	void (*reject_rsp)(struct cflayer *layer, u8 linkid,
				struct cflayer *client_layer);
};


struct cfctrl_link_param {
	enum cfctrl_srv linktype;
	u8 priority;		  
	u8 phyid;		  
	u8 endpoint;		  
	u8 chtype;		  
	union {
		struct {
			u8 connid;	
		} video;

		struct {
			u32 connid;	
		} datagram;

		struct {
			u32 connid;	
			char volume[20];	
		} rfm;		

		struct {
			u16 fifosize_kb;	
			u16 fifosize_bufs;	
			char name[16];	
			u8 params[255];	
			u16 paramlen;	
		} utility;	
	} u;
};


struct cfctrl_request_info {
	int sequence_no;
	enum cfctrl_cmd cmd;
	u8 channel_id;
	struct cfctrl_link_param param;
	struct cflayer *client_layer;
	struct list_head list;
};

struct cfctrl {
	struct cfsrvl serv;
	struct cfctrl_rsp res;
	atomic_t req_seq_no;
	atomic_t rsp_seq_no;
	struct list_head list;
	
	spinlock_t info_list_lock;
#ifndef CAIF_NO_LOOP
	u8 loop_linkid;
	int loop_linkused[256];
	
	spinlock_t loop_linkid_lock;
#endif

};

void cfctrl_enum_req(struct cflayer *cfctrl, u8 physlinkid);
int cfctrl_linkup_request(struct cflayer *cfctrl,
			   struct cfctrl_link_param *param,
			   struct cflayer *user_layer);
int  cfctrl_linkdown_req(struct cflayer *cfctrl, u8 linkid,
			 struct cflayer *client);

struct cflayer *cfctrl_create(void);
struct cfctrl_rsp *cfctrl_get_respfuncs(struct cflayer *layer);
int cfctrl_cancel_req(struct cflayer *layr, struct cflayer *adap_layer);
void cfctrl_remove(struct cflayer *layr);

#endif				
