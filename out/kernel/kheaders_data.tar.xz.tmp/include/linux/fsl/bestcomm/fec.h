

#ifndef __BESTCOMM_FEC_H__
#define __BESTCOMM_FEC_H__


struct bcom_fec_bd {
	u32	status;
	u32	skb_pa;
};

#define BCOM_FEC_TX_BD_TFD	0x08000000ul	
#define BCOM_FEC_TX_BD_TC	0x04000000ul	
#define BCOM_FEC_TX_BD_ABC	0x02000000ul	

#define BCOM_FEC_RX_BD_L	0x08000000ul	
#define BCOM_FEC_RX_BD_BC	0x00800000ul	
#define BCOM_FEC_RX_BD_MC	0x00400000ul	
#define BCOM_FEC_RX_BD_LG	0x00200000ul	
#define BCOM_FEC_RX_BD_NO	0x00100000ul	
#define BCOM_FEC_RX_BD_CR	0x00040000ul	
#define BCOM_FEC_RX_BD_OV	0x00020000ul	
#define BCOM_FEC_RX_BD_TR	0x00010000ul	
#define BCOM_FEC_RX_BD_LEN_MASK	0x000007fful	
#define BCOM_FEC_RX_BD_ERRORS	(BCOM_FEC_RX_BD_LG | BCOM_FEC_RX_BD_NO | \
		BCOM_FEC_RX_BD_CR | BCOM_FEC_RX_BD_OV | BCOM_FEC_RX_BD_TR)


extern struct bcom_task *
bcom_fec_rx_init(int queue_len, phys_addr_t fifo, int maxbufsize);

extern int
bcom_fec_rx_reset(struct bcom_task *tsk);

extern void
bcom_fec_rx_release(struct bcom_task *tsk);


extern struct bcom_task *
bcom_fec_tx_init(int queue_len, phys_addr_t fifo);

extern int
bcom_fec_tx_reset(struct bcom_task *tsk);

extern void
bcom_fec_tx_release(struct bcom_task *tsk);


#endif 

