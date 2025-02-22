
#ifndef ASM_ARM_HARDWARE_SERIAL_AMBA_H
#define ASM_ARM_HARDWARE_SERIAL_AMBA_H

#include <linux/types.h>


#define UART01x_DR		0x00	
#define UART01x_RSR		0x04	
#define UART01x_ECR		0x04	
#define UART010_LCRH		0x08	
#define ST_UART011_DMAWM	0x08    
#define UART010_LCRM		0x0C	
#define ST_UART011_TIMEOUT	0x0C    
#define UART010_LCRL		0x10	
#define UART010_CR		0x14	
#define UART01x_FR		0x18	
#define UART010_IIR		0x1C	
#define UART010_ICR		0x1C	
#define ST_UART011_LCRH_RX	0x1C    
#define UART01x_ILPR		0x20	
#define UART011_IBRD		0x24	
#define UART011_FBRD		0x28	
#define UART011_LCRH		0x2c	
#define ST_UART011_LCRH_TX	0x2c    
#define UART011_CR		0x30	
#define UART011_IFLS		0x34	
#define UART011_IMSC		0x38	
#define UART011_RIS		0x3c	
#define UART011_MIS		0x40	
#define UART011_ICR		0x44	
#define UART011_DMACR		0x48	
#define ST_UART011_XFCR		0x50	
#define ST_UART011_XON1		0x54	
#define ST_UART011_XON2		0x58	
#define ST_UART011_XOFF1	0x5C	
#define ST_UART011_XOFF2	0x60	
#define ST_UART011_ITCR		0x80	
#define ST_UART011_ITIP		0x84	
#define ST_UART011_ABCR		0x100	
#define ST_UART011_ABIMSC	0x15C	


#define ZX_UART011_DR		0x04
#define ZX_UART011_FR		0x14
#define ZX_UART011_IBRD		0x24
#define ZX_UART011_FBRD		0x28
#define ZX_UART011_LCRH		0x30
#define ZX_UART011_CR		0x34
#define ZX_UART011_IFLS		0x38
#define ZX_UART011_IMSC		0x40
#define ZX_UART011_RIS		0x44
#define ZX_UART011_MIS		0x48
#define ZX_UART011_ICR		0x4c
#define ZX_UART011_DMACR	0x50

#define UART011_DR_OE		(1 << 11)
#define UART011_DR_BE		(1 << 10)
#define UART011_DR_PE		(1 << 9)
#define UART011_DR_FE		(1 << 8)

#define UART01x_RSR_OE 		0x08
#define UART01x_RSR_BE 		0x04
#define UART01x_RSR_PE 		0x02
#define UART01x_RSR_FE 		0x01

#define UART011_FR_RI		0x100
#define UART011_FR_TXFE		0x080
#define UART011_FR_RXFF		0x040
#define UART01x_FR_TXFF		0x020
#define UART01x_FR_RXFE		0x010
#define UART01x_FR_BUSY		0x008
#define UART01x_FR_DCD 		0x004
#define UART01x_FR_DSR 		0x002
#define UART01x_FR_CTS 		0x001
#define UART01x_FR_TMSK		(UART01x_FR_TXFF + UART01x_FR_BUSY)


#define ZX_UART01x_FR_BUSY	0x100
#define ZX_UART01x_FR_DSR	0x008
#define ZX_UART01x_FR_CTS	0x002
#define ZX_UART011_FR_RI	0x001

#define UART011_CR_CTSEN	0x8000	
#define UART011_CR_RTSEN	0x4000	
#define UART011_CR_OUT2		0x2000	
#define UART011_CR_OUT1		0x1000	
#define UART011_CR_RTS		0x0800	
#define UART011_CR_DTR		0x0400	
#define UART011_CR_RXE		0x0200	
#define UART011_CR_TXE		0x0100	
#define UART011_CR_LBE		0x0080	
#define UART010_CR_RTIE		0x0040
#define UART010_CR_TIE 		0x0020
#define UART010_CR_RIE 		0x0010
#define UART010_CR_MSIE		0x0008
#define ST_UART011_CR_OVSFACT	0x0008	
#define UART01x_CR_IIRLP	0x0004	
#define UART01x_CR_SIREN	0x0002	
#define UART01x_CR_UARTEN	0x0001	
 
#define UART011_LCRH_SPS	0x80
#define UART01x_LCRH_WLEN_8	0x60
#define UART01x_LCRH_WLEN_7	0x40
#define UART01x_LCRH_WLEN_6	0x20
#define UART01x_LCRH_WLEN_5	0x00
#define UART01x_LCRH_FEN	0x10
#define UART01x_LCRH_STP2	0x08
#define UART01x_LCRH_EPS	0x04
#define UART01x_LCRH_PEN	0x02
#define UART01x_LCRH_BRK	0x01

#define ST_UART011_DMAWM_RX_1	(0 << 3)
#define ST_UART011_DMAWM_RX_2	(1 << 3)
#define ST_UART011_DMAWM_RX_4	(2 << 3)
#define ST_UART011_DMAWM_RX_8	(3 << 3)
#define ST_UART011_DMAWM_RX_16	(4 << 3)
#define ST_UART011_DMAWM_RX_32	(5 << 3)
#define ST_UART011_DMAWM_RX_48	(6 << 3)
#define ST_UART011_DMAWM_TX_1	0
#define ST_UART011_DMAWM_TX_2	1
#define ST_UART011_DMAWM_TX_4	2
#define ST_UART011_DMAWM_TX_8	3
#define ST_UART011_DMAWM_TX_16	4
#define ST_UART011_DMAWM_TX_32	5
#define ST_UART011_DMAWM_TX_48	6

#define UART010_IIR_RTIS	0x08
#define UART010_IIR_TIS		0x04
#define UART010_IIR_RIS		0x02
#define UART010_IIR_MIS		0x01

#define UART011_IFLS_RX1_8	(0 << 3)
#define UART011_IFLS_RX2_8	(1 << 3)
#define UART011_IFLS_RX4_8	(2 << 3)
#define UART011_IFLS_RX6_8	(3 << 3)
#define UART011_IFLS_RX7_8	(4 << 3)
#define UART011_IFLS_TX1_8	(0 << 0)
#define UART011_IFLS_TX2_8	(1 << 0)
#define UART011_IFLS_TX4_8	(2 << 0)
#define UART011_IFLS_TX6_8	(3 << 0)
#define UART011_IFLS_TX7_8	(4 << 0)

#define UART011_IFLS_RX_HALF	(5 << 3)
#define UART011_IFLS_TX_HALF	(5 << 0)

#define UART011_OEIM		(1 << 10)	
#define UART011_BEIM		(1 << 9)	
#define UART011_PEIM		(1 << 8)	
#define UART011_FEIM		(1 << 7)	
#define UART011_RTIM		(1 << 6)	
#define UART011_TXIM		(1 << 5)	
#define UART011_RXIM		(1 << 4)	
#define UART011_DSRMIM		(1 << 3)	
#define UART011_DCDMIM		(1 << 2)	
#define UART011_CTSMIM		(1 << 1)	
#define UART011_RIMIM		(1 << 0)	

#define UART011_OEIS		(1 << 10)	
#define UART011_BEIS		(1 << 9)	
#define UART011_PEIS		(1 << 8)	
#define UART011_FEIS		(1 << 7)	
#define UART011_RTIS		(1 << 6)	
#define UART011_TXIS		(1 << 5)	
#define UART011_RXIS		(1 << 4)	
#define UART011_DSRMIS		(1 << 3)	
#define UART011_DCDMIS		(1 << 2)	
#define UART011_CTSMIS		(1 << 1)	
#define UART011_RIMIS		(1 << 0)	

#define UART011_OEIC		(1 << 10)	
#define UART011_BEIC		(1 << 9)	
#define UART011_PEIC		(1 << 8)	
#define UART011_FEIC		(1 << 7)	
#define UART011_RTIC		(1 << 6)	
#define UART011_TXIC		(1 << 5)	
#define UART011_RXIC		(1 << 4)	
#define UART011_DSRMIC		(1 << 3)	
#define UART011_DCDMIC		(1 << 2)	
#define UART011_CTSMIC		(1 << 1)	
#define UART011_RIMIC		(1 << 0)	

#define UART011_DMAONERR	(1 << 2)	
#define UART011_TXDMAE		(1 << 1)	
#define UART011_RXDMAE		(1 << 0)	

#define UART01x_RSR_ANY		(UART01x_RSR_OE|UART01x_RSR_BE|UART01x_RSR_PE|UART01x_RSR_FE)
#define UART01x_FR_MODEM_ANY	(UART01x_FR_DCD|UART01x_FR_DSR|UART01x_FR_CTS)

#ifndef __ASSEMBLY__
struct amba_device; 
struct amba_pl010_data {
	void (*set_mctrl)(struct amba_device *dev, void __iomem *base, unsigned int mctrl);
};

struct dma_chan;
struct amba_pl011_data {
	bool (*dma_filter)(struct dma_chan *chan, void *filter_param);
	void *dma_rx_param;
	void *dma_tx_param;
	bool dma_rx_poll_enable;
	unsigned int dma_rx_poll_rate;
	unsigned int dma_rx_poll_timeout;
        void (*init) (void);
	void (*exit) (void);
};
#endif

#endif
