

#ifndef __CAPILLI_H__
#define __CAPILLI_H__

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/capi.h>
#include <linux/kernelcapi.h>

typedef struct capiloaddatapart {
	int user;		
	int len;
	unsigned char *data;
} capiloaddatapart;

typedef struct capiloaddata {
	capiloaddatapart firmware;
	capiloaddatapart configuration;
} capiloaddata;

typedef struct capicardparams {
	unsigned int port;
	unsigned irq;
	int cardtype;
	int cardnr;
	unsigned int membase;
} capicardparams;

struct capi_ctr {
	
	struct module *owner;
	void *driverdata;			
	char name[32];				
	char *driver_name;			
	int (*load_firmware)(struct capi_ctr *, capiloaddata *);
	void (*reset_ctr)(struct capi_ctr *);
	void (*register_appl)(struct capi_ctr *, u16 appl,
			      capi_register_params *);
	void (*release_appl)(struct capi_ctr *, u16 appl);
	u16  (*send_message)(struct capi_ctr *, struct sk_buff *skb);
	
	char *(*procinfo)(struct capi_ctr *);
	int (*proc_show)(struct seq_file *, void *);

	
	u8 manu[CAPI_MANUFACTURER_LEN];		
	capi_version version;			
	capi_profile profile;			
	u8 serial[CAPI_SERIAL_LEN];		

	

	unsigned long nrecvctlpkt;
	unsigned long nrecvdatapkt;
	unsigned long nsentctlpkt;
	unsigned long nsentdatapkt;

	int cnr;				
	unsigned short state;			
	int blocked;				
	int traceflag;				
	wait_queue_head_t state_wait_queue;

	struct proc_dir_entry *procent;
        char procfn[128];
};

int attach_capi_ctr(struct capi_ctr *);
int detach_capi_ctr(struct capi_ctr *);

void capi_ctr_ready(struct capi_ctr * card);
void capi_ctr_down(struct capi_ctr * card);
void capi_ctr_suspend_output(struct capi_ctr * card);
void capi_ctr_resume_output(struct capi_ctr * card);
void capi_ctr_handle_message(struct capi_ctr * card, u16 appl, struct sk_buff *skb);

// ---------------------------------------------------------------------------
// needed for AVM capi drivers

struct capi_driver {
	char name[32];				
	char revision[32];

	int (*add_card)(struct capi_driver *driver, capicardparams *data);

	
	struct list_head list; 
};

void register_capi_driver(struct capi_driver *driver);
void unregister_capi_driver(struct capi_driver *driver);

// ---------------------------------------------------------------------------
// library functions for use by hardware controller drivers

void capilib_new_ncci(struct list_head *head, u16 applid, u32 ncci, u32 winsize);
void capilib_free_ncci(struct list_head *head, u16 applid, u32 ncci);
void capilib_release_appl(struct list_head *head, u16 applid);
void capilib_release(struct list_head *head);
void capilib_data_b3_conf(struct list_head *head, u16 applid, u32 ncci, u16 msgid);
u16  capilib_data_b3_req(struct list_head *head, u16 applid, u32 ncci, u16 msgid);

#endif				
