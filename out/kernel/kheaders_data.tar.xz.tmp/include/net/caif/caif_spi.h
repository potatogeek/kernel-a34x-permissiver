

#ifndef CAIF_SPI_H_
#define CAIF_SPI_H_

#include <net/caif/caif_device.h>

#define SPI_CMD_WR			0x00
#define SPI_CMD_RD			0x01
#define SPI_CMD_EOT			0x02
#define SPI_CMD_IND			0x04

#define SPI_DMA_BUF_LEN			8192

#define WL_SZ				2	
#define SPI_CMD_SZ			4	
#define SPI_IND_SZ			4	

#define SPI_XFER			0
#define SPI_SS_ON			1
#define SPI_SS_OFF			2
#define SPI_TERMINATE			3


#define MIN_TRANSITION_TIME_USEC	50


#define SPI_MASTER_CLK_MHZ		13
#define SPI_XFER_TIME_USEC(bytes, clk) (((bytes) * 8) / clk)


#define CAIF_MAX_SPI_FRAME 4092


#define CAIF_MAX_SPI_PKTS 9


#define CFSPI_DBG_PREFILL		0


struct cfspi_xfer {
	u16 tx_dma_len;
	u16 rx_dma_len;
	void *va_tx[2];
	dma_addr_t pa_tx[2];
	void *va_rx;
	dma_addr_t pa_rx;
};


struct cfspi_ifc {
	void (*ss_cb) (bool assert, struct cfspi_ifc *ifc);
	void (*xfer_done_cb) (struct cfspi_ifc *ifc);
	void *priv;
};


struct cfspi_dev {
	int (*init_xfer) (struct cfspi_xfer *xfer, struct cfspi_dev *dev);
	void (*sig_xfer) (bool xfer, struct cfspi_dev *dev);
	struct cfspi_ifc *ifc;
	char *name;
	u32 clk_mhz;
	void *priv;
};


enum cfspi_state {
	CFSPI_STATE_WAITING = 0,
	CFSPI_STATE_AWAKE,
	CFSPI_STATE_FETCH_PKT,
	CFSPI_STATE_GET_NEXT,
	CFSPI_STATE_INIT_XFER,
	CFSPI_STATE_WAIT_ACTIVE,
	CFSPI_STATE_SIG_ACTIVE,
	CFSPI_STATE_WAIT_XFER_DONE,
	CFSPI_STATE_XFER_DONE,
	CFSPI_STATE_WAIT_INACTIVE,
	CFSPI_STATE_SIG_INACTIVE,
	CFSPI_STATE_DELIVER_PKT,
	CFSPI_STATE_MAX,
};


struct cfspi {
	struct caif_dev_common cfdev;
	struct net_device *ndev;
	struct platform_device *pdev;
	struct sk_buff_head qhead;
	struct sk_buff_head chead;
	u16 cmd;
	u16 tx_cpck_len;
	u16 tx_npck_len;
	u16 rx_cpck_len;
	u16 rx_npck_len;
	struct cfspi_ifc ifc;
	struct cfspi_xfer xfer;
	struct cfspi_dev *dev;
	unsigned long state;
	struct work_struct work;
	struct workqueue_struct *wq;
	struct list_head list;
	int    flow_off_sent;
	u32 qd_low_mark;
	u32 qd_high_mark;
	struct completion comp;
	wait_queue_head_t wait;
	spinlock_t lock;
	bool flow_stop;
	bool slave;
	bool slave_talked;
#ifdef CONFIG_DEBUG_FS
	enum cfspi_state dbg_state;
	u16 pcmd;
	u16 tx_ppck_len;
	u16 rx_ppck_len;
	struct dentry *dbgfs_dir;
	struct dentry *dbgfs_state;
	struct dentry *dbgfs_frame;
#endif				
};

extern int spi_frm_align;
extern int spi_up_head_align;
extern int spi_up_tail_align;
extern int spi_down_head_align;
extern int spi_down_tail_align;
extern struct platform_driver cfspi_spi_driver;

void cfspi_dbg_state(struct cfspi *cfspi, int state);
int cfspi_xmitfrm(struct cfspi *cfspi, u8 *buf, size_t len);
int cfspi_xmitlen(struct cfspi *cfspi);
int cfspi_rxfrm(struct cfspi *cfspi, u8 *buf, size_t len);
int cfspi_spi_remove(struct platform_device *pdev);
int cfspi_spi_probe(struct platform_device *pdev);
int cfspi_xmitfrm(struct cfspi *cfspi, u8 *buf, size_t len);
int cfspi_xmitlen(struct cfspi *cfspi);
int cfspi_rxfrm(struct cfspi *cfspi, u8 *buf, size_t len);
void cfspi_xfer(struct work_struct *work);

#endif				
