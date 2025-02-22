

#ifndef __VIA_CORE_H__
#define __VIA_CORE_H__
#include <linux/types.h>
#include <linux/io.h>
#include <linux/spinlock.h>
#include <linux/pci.h>


enum via_port_type {
	VIA_PORT_NONE = 0,
	VIA_PORT_I2C,
	VIA_PORT_GPIO,
};

enum via_port_mode {
	VIA_MODE_OFF = 0,
	VIA_MODE_I2C,		
	VIA_MODE_GPIO,	
};

enum viafb_i2c_adap {
	VIA_PORT_26 = 0,
	VIA_PORT_31,
	VIA_PORT_25,
	VIA_PORT_2C,
	VIA_PORT_3D,
};
#define VIAFB_NUM_PORTS 5

struct via_port_cfg {
	enum via_port_type	type;
	enum via_port_mode	mode;
	u16			io_port;
	u8			ioport_index;
};


#ifdef CONFIG_PM
struct viafb_pm_hooks {
	struct list_head list;
	int (*suspend)(void *private);
	int (*resume)(void *private);
	void *private;
};

void viafb_pm_register(struct viafb_pm_hooks *hooks);
void viafb_pm_unregister(struct viafb_pm_hooks *hooks);
#endif 


struct viafb_dev {
	struct pci_dev *pdev;
	int chip_type;
	struct via_port_cfg *port_cfg;
	
	spinlock_t reg_lock;
	
	unsigned long fbmem_start;
	long fbmem_len;
	void __iomem *fbmem;
#if defined(CONFIG_VIDEO_VIA_CAMERA) || defined(CONFIG_VIDEO_VIA_CAMERA_MODULE)
	long camera_fbmem_offset;
	long camera_fbmem_size;
#endif
	
	unsigned long engine_start;
	unsigned long engine_len;
	void __iomem *engine_mmio;

};



void viafb_irq_enable(u32 mask);
void viafb_irq_disable(u32 mask);


#define VDE_INTERRUPT	0x200	
#define   VDE_I_DVISENSE  0x00000001  
#define   VDE_I_VBLANK    0x00000002  
#define   VDE_I_MCCFI	  0x00000004  
#define   VDE_I_VSYNC	  0x00000008  
#define   VDE_I_DMA0DDONE 0x00000010  
#define   VDE_I_DMA0TDONE 0x00000020  
#define   VDE_I_DMA1DDONE 0x00000040  
#define   VDE_I_DMA1TDONE 0x00000080  
#define   VDE_I_C1AV      0x00000100  
#define   VDE_I_HQV0	  0x00000200  
#define   VDE_I_HQV1      0x00000400  
#define   VDE_I_HQV1EN	  0x00000800  
#define   VDE_I_C0AV      0x00001000  
#define   VDE_I_C0VBI     0x00002000  
#define   VDE_I_C1VBI     0x00004000  
#define   VDE_I_VSYNC2    0x00008000  
#define   VDE_I_DVISNSEN  0x00010000  
#define   VDE_I_VSYNC2EN  0x00020000  
#define   VDE_I_MCCFIEN	  0x00040000  
#define   VDE_I_VSYNCEN   0x00080000  
#define   VDE_I_DMA0DDEN  0x00100000  
#define   VDE_I_DMA0TDEN  0x00200000  
#define   VDE_I_DMA1DDEN  0x00400000  
#define   VDE_I_DMA1TDEN  0x00800000  
#define   VDE_I_C1AVEN    0x01000000  
#define   VDE_I_HQV0EN	  0x02000000  
#define   VDE_I_C1VBIEN	  0x04000000  
#define   VDE_I_LVDSSI    0x08000000  
#define   VDE_I_C0AVEN    0x10000000  
#define   VDE_I_C0VBIEN   0x20000000  
#define   VDE_I_LVDSSIEN  0x40000000  
#define   VDE_I_ENABLE	  0x80000000  

#if defined(CONFIG_VIDEO_VIA_CAMERA) || defined(CONFIG_VIDEO_VIA_CAMERA_MODULE)

int viafb_request_dma(void);
void viafb_release_dma(void);

int viafb_dma_copy_out_sg(unsigned int offset, struct scatterlist *sg, int nsg);


#define VDMA_MR0	0xe00		
#define   VDMA_MR_CHAIN   0x01		
#define   VDMA_MR_TDIE    0x02		
#define VDMA_CSR0	0xe04		
#define	  VDMA_C_ENABLE	  0x01		  
#define	  VDMA_C_START	  0x02		  
#define	  VDMA_C_ABORT	  0x04		  
#define	  VDMA_C_DONE	  0x08		  
#define VDMA_MARL0	0xe20		
#define VDMA_MARH0	0xe24		
#define VDMA_DAR0	0xe28		
#define VDMA_DQWCR0	0xe2c		
#define VDMA_TMR0	0xe30		
#define VDMA_DPRL0	0xe34		
#define	  VDMA_DPR_IN	  0x08		
#define VDMA_DPRH0	0xe38
#define VDMA_PMR0	(0xe00 + 0x134) 


#define VGA_WIDTH	640
#define VGA_HEIGHT	480
#endif 



#define VIAStatus   0x3DA  
#define VIACR       0x3D4
#define VIASR       0x3C4
#define VIAGR       0x3CE
#define VIAAR       0x3C0

static inline u8 via_read_reg(u16 port, u8 index)
{
	outb(index, port);
	return inb(port + 1);
}

static inline void via_write_reg(u16 port, u8 index, u8 data)
{
	outb(index, port);
	outb(data, port + 1);
}

static inline void via_write_reg_mask(u16 port, u8 index, u8 data, u8 mask)
{
	u8 old;

	outb(index, port);
	old = inb(port + 1);
	outb((data & mask) | (old & ~mask), port + 1);
}

#define VIA_MISC_REG_READ	0x03CC
#define VIA_MISC_REG_WRITE	0x03C2

static inline void via_write_misc_reg_mask(u8 data, u8 mask)
{
	u8 old = inb(VIA_MISC_REG_READ);
	outb((data & mask) | (old & ~mask), VIA_MISC_REG_WRITE);
}


#endif 
