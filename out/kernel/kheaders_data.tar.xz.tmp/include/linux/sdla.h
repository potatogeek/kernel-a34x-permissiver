
#ifndef SDLA_H
#define SDLA_H

#include <uapi/linux/sdla.h>



#define SDLA_CONTROL_WND		0xE000

#define SDLA_502_CMD_BUF		0xEF60
#define SDLA_502_RCV_BUF		0xA900
#define	SDLA_502_TXN_AVAIL		0xFFF1
#define SDLA_502_RCV_AVAIL		0xFFF2
#define SDLA_502_EVENT_FLAGS		0xFFF3
#define SDLA_502_MDM_STATUS		0xFFF4
#define SDLA_502_IRQ_INTERFACE		0xFFFD
#define SDLA_502_IRQ_PERMISSION		0xFFFE
#define SDLA_502_DATA_OFS		0x0010

#define SDLA_508_CMD_BUF		0xE000
#define SDLA_508_TXBUF_INFO		0xF100
#define SDLA_508_RXBUF_INFO		0xF120
#define SDLA_508_EVENT_FLAGS		0xF003
#define SDLA_508_MDM_STATUS		0xF004
#define SDLA_508_IRQ_INTERFACE		0xF010
#define SDLA_508_IRQ_PERMISSION		0xF011
#define SDLA_508_TSE_OFFSET		0xF012


#define SDLA_EVENT_STATUS		0x01
#define SDLA_EVENT_DLCI_STATUS		0x02
#define SDLA_EVENT_BAD_DLCI		0x04
#define SDLA_EVENT_LINK_DOWN		0x40


#define SDLA_INTR_RX			0x01
#define SDLA_INTR_TX			0x02
#define SDLA_INTR_MODEM			0x04
#define SDLA_INTR_COMPLETE		0x08
#define SDLA_INTR_STATUS		0x10
#define SDLA_INTR_TIMER			0x20


#define SDLA_DLCI_DELETED		0x01
#define SDLA_DLCI_ACTIVE		0x02
#define SDLA_DLCI_WAITING		0x04
#define SDLA_DLCI_NEW			0x08
#define SDLA_DLCI_INCLUDED		0x40


#define	SDLA_INFORMATION_WRITE		0x01
#define	SDLA_INFORMATION_READ		0x02
#define SDLA_ISSUE_IN_CHANNEL_SIGNAL	0x03
#define	SDLA_SET_DLCI_CONFIGURATION	0x10
#define	SDLA_READ_DLCI_CONFIGURATION	0x11
#define	SDLA_DISABLE_COMMUNICATIONS	0x12
#define	SDLA_ENABLE_COMMUNICATIONS	0x13
#define	SDLA_READ_DLC_STATUS		0x14
#define	SDLA_READ_DLC_STATISTICS	0x15
#define	SDLA_FLUSH_DLC_STATISTICS	0x16
#define	SDLA_LIST_ACTIVE_DLCI		0x17
#define	SDLA_FLUSH_INFORMATION_BUFFERS	0x18
#define	SDLA_ADD_DLCI			0x20
#define	SDLA_DELETE_DLCI		0x21
#define	SDLA_ACTIVATE_DLCI		0x22
#define	SDLA_DEACTIVATE_DLCI		0x23
#define	SDLA_READ_MODEM_STATUS		0x30
#define	SDLA_SET_MODEM_STATUS		0x31
#define	SDLA_READ_COMMS_ERR_STATS	0x32
#define SDLA_FLUSH_COMMS_ERR_STATS	0x33
#define	SDLA_READ_CODE_VERSION		0x40
#define SDLA_SET_IRQ_TRIGGER		0x50
#define SDLA_GET_IRQ_TRIGGER		0x51


#define SDLA_ICS_LINK_VERIFY		0x02
#define SDLA_ICS_STATUS_ENQ		0x03


#define SDLA_MODEM_DTR_HIGH		0x01
#define SDLA_MODEM_RTS_HIGH		0x02
#define SDLA_MODEM_DCD_HIGH		0x08
#define SDLA_MODEM_CTS_HIGH		0x20


#define SDLA_MODEM_DCD_LOW		0x01
#define SDLA_MODEM_CTS_LOW		0x02


#define SDLA_RET_OK			0x00
#define SDLA_RET_COMMUNICATIONS		0x01
#define SDLA_RET_CHANNEL_INACTIVE	0x02
#define SDLA_RET_DLCI_INACTIVE		0x03
#define SDLA_RET_DLCI_CONFIG		0x04
#define SDLA_RET_BUF_TOO_BIG		0x05
#define SDLA_RET_NO_DATA		0x05
#define SDLA_RET_BUF_OVERSIZE		0x06
#define SDLA_RET_CIR_OVERFLOW		0x07
#define SDLA_RET_NO_BUFS		0x08
#define SDLA_RET_TIMEOUT		0x0A
#define SDLA_RET_MODEM			0x10
#define SDLA_RET_CHANNEL_OFF		0x11
#define SDLA_RET_CHANNEL_ON		0x12
#define SDLA_RET_DLCI_STATUS		0x13
#define SDLA_RET_DLCI_UNKNOWN       	0x14
#define SDLA_RET_COMMAND_INVALID    	0x1F


#define SDLA_DIRECT_RECV		0x0080
#define SDLA_TX_NO_EXCEPT		0x0020
#define SDLA_NO_ICF_MSGS		0x1000
#define SDLA_TX50_RX50			0x0000
#define SDLA_TX70_RX30			0x2000
#define SDLA_TX30_RX70			0x4000


#define SDLA_IRQ_RECEIVE		0x01
#define SDLA_IRQ_TRANSMIT		0x02
#define SDLA_IRQ_MODEM_STAT		0x04
#define SDLA_IRQ_COMMAND		0x08
#define SDLA_IRQ_CHANNEL		0x10
#define SDLA_IRQ_TIMER			0x20


#define SDLA_8K_WINDOW			0x01
#define SDLA_S502_SEG_A			0x10
#define SDLA_S502_SEG_C			0x20
#define SDLA_S502_SEG_D			0x00
#define SDLA_S502_SEG_E			0x30
#define SDLA_S507_SEG_A			0x00
#define SDLA_S507_SEG_B			0x40
#define SDLA_S507_SEG_C			0x80
#define SDLA_S507_SEG_E			0xC0
#define SDLA_S508_SEG_A			0x00
#define SDLA_S508_SEG_C			0x10
#define SDLA_S508_SEG_D			0x08
#define SDLA_S508_SEG_E			0x18


#define SDLA_IO_EXTENTS			0x04
	
#define SDLA_REG_CONTROL		0x00
#define SDLA_REG_PC_WINDOW		0x01	
#define SDLA_REG_Z80_WINDOW 		0x02	
#define SDLA_REG_Z80_CONTROL		0x03	
	
#define SDLA_S502_STS			0x00	
#define SDLA_S508_GNRL			0x00	
#define SDLA_S508_STS			0x01	
#define SDLA_S508_IDR			0x02	
	

#define SDLA_S502A_START		0x00	
#define SDLA_S502A_INTREQ		0x02
#define SDLA_S502A_INTEN		0x04
#define SDLA_S502A_HALT			0x08		
#define SDLA_S502A_NMI			0x10	

#define SDLA_S502E_CPUEN		0x01
#define SDLA_S502E_ENABLE		0x02
#define SDLA_S502E_INTACK		0x04
	
#define SDLA_S507_ENABLE		0x01
#define SDLA_S507_IRQ3			0x00
#define SDLA_S507_IRQ4			0x20
#define SDLA_S507_IRQ5			0x40
#define SDLA_S507_IRQ7			0x60
#define SDLA_S507_IRQ10			0x80
#define SDLA_S507_IRQ11			0xA0
#define SDLA_S507_IRQ12			0xC0
#define SDLA_S507_IRQ15			0xE0
	
#define SDLA_HALT			0x00
#define SDLA_CPUEN			0x02
#define SDLA_MEMEN			0x04
#define SDLA_S507_EPROMWR		0x08
#define SDLA_S507_EPROMCLK		0x10
#define SDLA_S508_INTRQ			0x08
#define SDLA_S508_INTEN			0x10

struct sdla_cmd {
   char  opp_flag;
   char  cmd;
   short length;
   char  retval;
   short dlci;
   char  flags;
   short rxlost_int;
   long  rxlost_app;
   char  reserve[2];
   char  data[SDLA_MAX_DATA];	
} __attribute__((packed));

struct intr_info {
   char  flags;
   short txlen;
   char  irq;
   char  flags2;
   short timeout;
} __attribute__((packed));


struct buf_info {
   unsigned short rse_num;
   unsigned long  rse_base;
   unsigned long  rse_next;
   unsigned long  buf_base;
   unsigned short reserved;
   unsigned long  buf_top;
} __attribute__((packed));


struct buf_entry {
   char  opp_flag;
   short length;
   short dlci;
   char  flags;
   short timestamp;
   short reserved[2];
   long  buf_addr;
} __attribute__((packed));

#endif
