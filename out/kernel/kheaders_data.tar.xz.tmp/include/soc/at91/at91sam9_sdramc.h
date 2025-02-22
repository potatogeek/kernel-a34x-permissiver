

#ifndef AT91SAM9_SDRAMC_H
#define AT91SAM9_SDRAMC_H


#define AT91_SDRAMC_MR		0x00	
#define		AT91_SDRAMC_MODE	(0xf << 0)		
#define			AT91_SDRAMC_MODE_NORMAL		0
#define			AT91_SDRAMC_MODE_NOP		1
#define			AT91_SDRAMC_MODE_PRECHARGE	2
#define			AT91_SDRAMC_MODE_LMR		3
#define			AT91_SDRAMC_MODE_REFRESH	4
#define			AT91_SDRAMC_MODE_EXT_LMR	5
#define			AT91_SDRAMC_MODE_DEEP		6

#define AT91_SDRAMC_TR		0x04	
#define		AT91_SDRAMC_COUNT	(0xfff << 0)		

#define AT91_SDRAMC_CR		0x08	
#define		AT91_SDRAMC_NC		(3 << 0)		
#define			AT91_SDRAMC_NC_8	(0 << 0)
#define			AT91_SDRAMC_NC_9	(1 << 0)
#define			AT91_SDRAMC_NC_10	(2 << 0)
#define			AT91_SDRAMC_NC_11	(3 << 0)
#define		AT91_SDRAMC_NR		(3 << 2)		
#define			AT91_SDRAMC_NR_11	(0 << 2)
#define			AT91_SDRAMC_NR_12	(1 << 2)
#define			AT91_SDRAMC_NR_13	(2 << 2)
#define		AT91_SDRAMC_NB		(1 << 4)		
#define			AT91_SDRAMC_NB_2	(0 << 4)
#define			AT91_SDRAMC_NB_4	(1 << 4)
#define		AT91_SDRAMC_CAS		(3 << 5)		
#define			AT91_SDRAMC_CAS_1	(1 << 5)
#define			AT91_SDRAMC_CAS_2	(2 << 5)
#define			AT91_SDRAMC_CAS_3	(3 << 5)
#define		AT91_SDRAMC_DBW		(1 << 7)		
#define			AT91_SDRAMC_DBW_32	(0 << 7)
#define			AT91_SDRAMC_DBW_16	(1 << 7)
#define		AT91_SDRAMC_TWR		(0xf <<  8)		
#define		AT91_SDRAMC_TRC		(0xf << 12)		
#define		AT91_SDRAMC_TRP		(0xf << 16)		
#define		AT91_SDRAMC_TRCD	(0xf << 20)		
#define		AT91_SDRAMC_TRAS	(0xf << 24)		
#define		AT91_SDRAMC_TXSR	(0xf << 28)		

#define AT91_SDRAMC_LPR		0x10	
#define		AT91_SDRAMC_LPCB		(3 << 0)	
#define			AT91_SDRAMC_LPCB_DISABLE		0
#define			AT91_SDRAMC_LPCB_SELF_REFRESH		1
#define			AT91_SDRAMC_LPCB_POWER_DOWN		2
#define			AT91_SDRAMC_LPCB_DEEP_POWER_DOWN	3
#define		AT91_SDRAMC_PASR		(7 << 4)	
#define		AT91_SDRAMC_TCSR		(3 << 8)	
#define		AT91_SDRAMC_DS			(3 << 10)	
#define		AT91_SDRAMC_TIMEOUT		(3 << 12)	
#define			AT91_SDRAMC_TIMEOUT_0_CLK_CYCLES	(0 << 12)
#define			AT91_SDRAMC_TIMEOUT_64_CLK_CYCLES	(1 << 12)
#define			AT91_SDRAMC_TIMEOUT_128_CLK_CYCLES	(2 << 12)

#define AT91_SDRAMC_IER		0x14	
#define AT91_SDRAMC_IDR		0x18	
#define AT91_SDRAMC_IMR		0x1C	
#define AT91_SDRAMC_ISR		0x20	
#define		AT91_SDRAMC_RES		(1 << 0)		

#define AT91_SDRAMC_MDR		0x24	
#define		AT91_SDRAMC_MD		(3 << 0)		
#define			AT91_SDRAMC_MD_SDRAM		0
#define			AT91_SDRAMC_MD_LOW_POWER_SDRAM	1

#endif
