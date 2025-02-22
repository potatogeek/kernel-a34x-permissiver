

#ifndef ATMEL_PDC_H
#define ATMEL_PDC_H

#define ATMEL_PDC_RPR		0x100	
#define ATMEL_PDC_RCR		0x104	
#define ATMEL_PDC_TPR		0x108	
#define ATMEL_PDC_TCR		0x10c	
#define ATMEL_PDC_RNPR		0x110	
#define ATMEL_PDC_RNCR		0x114	
#define ATMEL_PDC_TNPR		0x118	
#define ATMEL_PDC_TNCR		0x11c	

#define ATMEL_PDC_PTCR		0x120	
#define		ATMEL_PDC_RXTEN		(1 << 0)	
#define		ATMEL_PDC_RXTDIS	(1 << 1)	
#define		ATMEL_PDC_TXTEN		(1 << 8)	
#define		ATMEL_PDC_TXTDIS	(1 << 9)	

#define ATMEL_PDC_PTSR		0x124	

#define ATMEL_PDC_SCND_BUF_OFF	0x10	

#endif
