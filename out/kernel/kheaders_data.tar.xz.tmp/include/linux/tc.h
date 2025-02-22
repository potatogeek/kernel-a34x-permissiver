
#ifndef _LINUX_TC_H
#define _LINUX_TC_H

#include <linux/compiler.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <linux/types.h>


#define TC_OLDCARD	0x3c0000
#define TC_NEWCARD	0x000000

#define TC_ROM_WIDTH	0x3e0
#define TC_ROM_STRIDE	0x3e4
#define TC_ROM_SIZE	0x3e8
#define TC_SLOT_SIZE	0x3ec
#define TC_PATTERN0	0x3f0
#define TC_PATTERN1	0x3f4
#define TC_PATTERN2	0x3f8
#define TC_PATTERN3	0x3fc
#define TC_FIRM_VER	0x400
#define TC_VENDOR	0x420
#define TC_MODULE	0x440
#define TC_FIRM_TYPE	0x460
#define TC_FLAGS	0x470
#define TC_ROM_OBJECTS	0x480


struct tcinfo {
	s32		revision;	
	s32		clk_period;	
	s32		slot_size;	
	s32		io_timeout;	
	s32		dma_range;	
	s32		max_dma_burst;	
	s32		parity;		
	s32		reserved[4];
};


struct tc_bus {
	struct list_head devices;	
	struct resource	resource[2];	

	struct device	dev;
	char		name[13];
	resource_size_t	slot_base;
	resource_size_t	ext_slot_base;
	resource_size_t	ext_slot_size;
	int		num_tcslots;
	struct tcinfo	info;
};


struct tc_dev {
	struct list_head node;		
	struct tc_bus	*bus;		
	struct tc_driver *driver;	
	struct device	dev;		
	struct resource	resource;	
	u64		dma_mask;	
	char		vendor[9];
	char		name[9];
	char		firmware[9];
	int		interrupt;
	int		slot;
};

#define to_tc_dev(n) container_of(n, struct tc_dev, dev)

struct tc_device_id {
	char		vendor[9];
	char		name[9];
};


struct tc_driver {
	struct list_head node;
	const struct tc_device_id *id_table;
	struct device_driver driver;
};

#define to_tc_driver(drv) container_of(drv, struct tc_driver, driver)


static inline unsigned long tc_get_speed(struct tc_bus *tbus)
{
	return 100000 * (10000 / (unsigned long)tbus->info.clk_period);
}

#ifdef CONFIG_TC

extern struct bus_type tc_bus_type;

extern int tc_register_driver(struct tc_driver *tdrv);
extern void tc_unregister_driver(struct tc_driver *tdrv);

#else 

static inline int tc_register_driver(struct tc_driver *tdrv) { return 0; }
static inline void tc_unregister_driver(struct tc_driver *tdrv) { }

#endif 


extern int tc_preadb(u8 *valp, void __iomem *addr);
extern int tc_bus_get_info(struct tc_bus *tbus);
extern void tc_device_get_irq(struct tc_dev *tdev);

#endif 
