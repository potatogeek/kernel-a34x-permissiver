
#ifndef LINUX_SSB_EXTIFCORE_H_
#define LINUX_SSB_EXTIFCORE_H_


#define	SSB_EXTIF_PCMCIA_MEMBASE(x)	(x)
#define	SSB_EXTIF_PCMCIA_IOBASE(x)	((x) + 0x100000)
#define	SSB_EXTIF_PCMCIA_CFGBASE(x)	((x) + 0x200000)
#define	SSB_EXTIF_CFGIF_BASE(x)		((x) + 0x800000)
#define	SSB_EXTIF_FLASH_BASE(x)		((x) + 0xc00000)

#define SSB_EXTIF_NR_GPIOOUT		5

#define SSB_EXTIF_GPIO_OUT(index)	({		\
	BUILD_BUG_ON(index >= SSB_EXTIF_NR_GPIOOUT);	\
	SSB_EXTIF_GPIO_OUT_BASE + ((index) * 8);	\
					})
#define SSB_EXTIF_GPIO_OUTEN(index)	({		\
	BUILD_BUG_ON(index >= SSB_EXTIF_NR_GPIOOUT);	\
	SSB_EXTIF_GPIO_OUTEN_BASE + ((index) * 8);	\
					})



#define SSB_EXTIF_CTL			0x0000
#define  SSB_EXTIF_CTL_UARTEN		(1 << 0) 
#define SSB_EXTIF_EXTSTAT		0x0004
#define  SSB_EXTIF_EXTSTAT_EMODE	(1 << 0) 
#define  SSB_EXTIF_EXTSTAT_EIRQPIN	(1 << 1) 
#define  SSB_EXTIF_EXTSTAT_GPIOIRQPIN	(1 << 2) 
#define SSB_EXTIF_PCMCIA_CFG		0x0010
#define SSB_EXTIF_PCMCIA_MEMWAIT	0x0014
#define SSB_EXTIF_PCMCIA_ATTRWAIT	0x0018
#define SSB_EXTIF_PCMCIA_IOWAIT		0x001C
#define SSB_EXTIF_PROG_CFG		0x0020
#define SSB_EXTIF_PROG_WAITCNT		0x0024
#define SSB_EXTIF_FLASH_CFG		0x0028
#define SSB_EXTIF_FLASH_WAITCNT		0x002C
#define SSB_EXTIF_WATCHDOG		0x0040
#define SSB_EXTIF_CLOCK_N		0x0044
#define SSB_EXTIF_CLOCK_SB		0x0048
#define SSB_EXTIF_CLOCK_PCI		0x004C
#define SSB_EXTIF_CLOCK_MII		0x0050
#define SSB_EXTIF_GPIO_IN		0x0060
#define SSB_EXTIF_GPIO_OUT_BASE		0x0064
#define SSB_EXTIF_GPIO_OUTEN_BASE	0x0068
#define SSB_EXTIF_EJTAG_OUTEN		0x0090
#define SSB_EXTIF_GPIO_INTPOL		0x0094
#define SSB_EXTIF_GPIO_INTMASK		0x0098
#define SSB_EXTIF_UART_DATA		0x0300
#define SSB_EXTIF_UART_TIMER		0x0310
#define SSB_EXTIF_UART_FCR		0x0320
#define SSB_EXTIF_UART_LCR		0x0330
#define SSB_EXTIF_UART_MCR		0x0340
#define SSB_EXTIF_UART_LSR		0x0350
#define SSB_EXTIF_UART_MSR		0x0360
#define SSB_EXTIF_UART_SCRATCH		0x0370





#define	SSB_EXTCFG_EN			(1 << 0)	
#define	SSB_EXTCFG_MODE			0xE		
#define	SSB_EXTCFG_MODE_SHIFT		1
#define	 SSB_EXTCFG_MODE_FLASH		0x0		
#define	 SSB_EXTCFG_MODE_SYNC		0x2		
#define	 SSB_EXTCFG_MODE_PCMCIA		0x4		
#define	SSB_EXTCFG_DS16			(1 << 4)	
#define	SSB_EXTCFG_BSWAP		(1 << 5)	
#define	SSB_EXTCFG_CLKDIV		0xC0		
#define	SSB_EXTCFG_CLKDIV_SHIFT		6
#define	 SSB_EXTCFG_CLKDIV_2		0x0		
#define	 SSB_EXTCFG_CLKDIV_3		0x40		
#define	 SSB_EXTCFG_CLKDIV_4		0x80		
#define	SSB_EXTCFG_CLKEN		(1 << 8)	
#define	SSB_EXTCFG_STROBE		(1 << 9)	


#define	SSB_PCMCIA_MEMW_0		0x0000003F	
#define	SSB_PCMCIA_MEMW_1		0x00001F00	
#define	SSB_PCMCIA_MEMW_1_SHIFT		8
#define	SSB_PCMCIA_MEMW_2		0x001F0000	
#define	SSB_PCMCIA_MEMW_2_SHIFT		16
#define	SSB_PCMCIA_MEMW_3		0x1F000000	
#define	SSB_PCMCIA_MEMW_3_SHIFT		24


#define	SSB_PCMCIA_ATTW_0		0x0000003F	
#define	SSB_PCMCIA_ATTW_1		0x00001F00	
#define	SSB_PCMCIA_ATTW_1_SHIFT		8
#define	SSB_PCMCIA_ATTW_2		0x001F0000	
#define	SSB_PCMCIA_ATTW_2_SHIFT		16
#define	SSB_PCMCIA_ATTW_3		0x1F000000	
#define	SSB_PCMCIA_ATTW_3_SHIFT		24


#define	SSB_PCMCIA_IOW_0		0x0000003F	
#define	SSB_PCMCIA_IOW_1		0x00001F00	
#define	SSB_PCMCIA_IOW_1_SHIFT		8
#define	SSB_PCMCIA_IOW_2		0x001F0000	
#define	SSB_PCMCIA_IOW_2_SHIFT		16
#define	SSB_PCMCIA_IOW_3		0x1F000000	
#define	SSB_PCMCIA_IOW_3_SHIFT		24


#define	SSB_PROG_WCNT_0			0x0000001F	
#define	SSB_PROG_WCNT_1			0x00001F00	
#define	SSB_PROG_WCNT_1_SHIFT		8
#define	SSB_PROG_WCNT_2			0x001F0000	
#define	SSB_PROG_WCNT_2_SHIFT		16
#define	SSB_PROG_WCNT_3			0x1F000000	
#define	SSB_PROG_WCNT_3_SHIFT		24

#define SSB_PROG_W0			0x0000000C
#define SSB_PROG_W1			0x00000A00
#define SSB_PROG_W2			0x00020000
#define SSB_PROG_W3			0x01000000


#define	SSB_FLASH_WCNT_0		0x0000001F	
#define	SSB_FLASH_WCNT_1		0x00001F00	
#define	SSB_FLASH_WCNT_1_SHIFT		8
#define	SSB_FLASH_WCNT_2		0x001F0000	
#define	SSB_FLASH_WCNT_2_SHIFT		16
#define	SSB_FLASH_WCNT_3		0x1F000000	
#define	SSB_FLASH_WCNT_3_SHIFT		24


#define SSB_EXTIF_WATCHDOG_CLK		48000000	

#define SSB_EXTIF_WATCHDOG_MAX_TIMER	((1 << 28) - 1)
#define SSB_EXTIF_WATCHDOG_MAX_TIMER_MS	(SSB_EXTIF_WATCHDOG_MAX_TIMER \
					 / (SSB_EXTIF_WATCHDOG_CLK / 1000))


#ifdef CONFIG_SSB_DRIVER_EXTIF

struct ssb_extif {
	struct ssb_device *dev;
	spinlock_t gpio_lock;
};

static inline bool ssb_extif_available(struct ssb_extif *extif)
{
	return (extif->dev != NULL);
}

extern void ssb_extif_get_clockcontrol(struct ssb_extif *extif,
			               u32 *plltype, u32 *n, u32 *m);

extern void ssb_extif_timing_init(struct ssb_extif *extif,
				  unsigned long ns);

extern u32 ssb_extif_watchdog_timer_set(struct ssb_extif *extif, u32 ticks);


u32 ssb_extif_gpio_in(struct ssb_extif *extif, u32 mask);
u32 ssb_extif_gpio_out(struct ssb_extif *extif, u32 mask, u32 value);
u32 ssb_extif_gpio_outen(struct ssb_extif *extif, u32 mask, u32 value);
u32 ssb_extif_gpio_polarity(struct ssb_extif *extif, u32 mask, u32 value);
u32 ssb_extif_gpio_intmask(struct ssb_extif *extif, u32 mask, u32 value);

#ifdef CONFIG_SSB_SERIAL
extern int ssb_extif_serial_init(struct ssb_extif *extif,
				 struct ssb_serial_port *ports);
#endif 


#else 


struct ssb_extif {
};

static inline bool ssb_extif_available(struct ssb_extif *extif)
{
	return 0;
}

static inline
void ssb_extif_get_clockcontrol(struct ssb_extif *extif,
			        u32 *plltype, u32 *n, u32 *m)
{
}

static inline
void ssb_extif_timing_init(struct ssb_extif *extif, unsigned long ns)
{
}

static inline
u32 ssb_extif_watchdog_timer_set(struct ssb_extif *extif, u32 ticks)
{
	return 0;
}

static inline u32 ssb_extif_gpio_in(struct ssb_extif *extif, u32 mask)
{
	return 0;
}

static inline u32 ssb_extif_gpio_out(struct ssb_extif *extif, u32 mask,
				     u32 value)
{
	return 0;
}

static inline u32 ssb_extif_gpio_outen(struct ssb_extif *extif, u32 mask,
				       u32 value)
{
	return 0;
}

static inline u32 ssb_extif_gpio_polarity(struct ssb_extif *extif, u32 mask,
					  u32 value)
{
	return 0;
}

static inline u32 ssb_extif_gpio_intmask(struct ssb_extif *extif, u32 mask,
					 u32 value)
{
	return 0;
}

#ifdef CONFIG_SSB_SERIAL
static inline int ssb_extif_serial_init(struct ssb_extif *extif,
					struct ssb_serial_port *ports)
{
	return 0;
}
#endif 

#endif 
#endif 
