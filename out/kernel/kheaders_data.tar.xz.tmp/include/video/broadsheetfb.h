

#ifndef _LINUX_BROADSHEETFB_H_
#define _LINUX_BROADSHEETFB_H_


#define BS_CMD_INIT_SYS_RUN	0x06
#define BS_CMD_INIT_DSPE_CFG	0x09
#define BS_CMD_INIT_DSPE_TMG	0x0A
#define BS_CMD_INIT_ROTMODE	0x0B
#define BS_CMD_RD_REG		0x10
#define BS_CMD_WR_REG		0x11
#define BS_CMD_LD_IMG		0x20
#define BS_CMD_LD_IMG_AREA	0x22
#define BS_CMD_LD_IMG_END	0x23
#define BS_CMD_WAIT_DSPE_TRG	0x28
#define BS_CMD_WAIT_DSPE_FREND	0x29
#define BS_CMD_RD_WFM_INFO	0x30
#define BS_CMD_UPD_INIT		0x32
#define BS_CMD_UPD_FULL		0x33
#define BS_CMD_UPD_GDRV_CLR	0x37


#define BS_REG_REV		0x00
#define BS_REG_PRC		0x02


#define BS_CS	0x01
#define BS_DC 	0x02
#define BS_WR 	0x03


#define BS_MMIO_CMD	0x01
#define BS_MMIO_DATA	0x02


struct broadsheetfb_par {
	struct fb_info *info;
	struct broadsheet_board *board;
	void (*write_reg)(struct broadsheetfb_par *, u16 reg, u16 val);
	u16 (*read_reg)(struct broadsheetfb_par *, u16 reg);
	wait_queue_head_t waitq;
	int panel_index;
	struct mutex io_lock;
};


struct broadsheet_board {
	struct module *owner;
	int (*init)(struct broadsheetfb_par *);
	int (*wait_for_rdy)(struct broadsheetfb_par *);
	void (*cleanup)(struct broadsheetfb_par *);
	int (*get_panel_type)(void);
	int (*setup_irq)(struct fb_info *);

	
	void (*set_ctl)(struct broadsheetfb_par *, unsigned char, u8);
	void (*set_hdb)(struct broadsheetfb_par *, u16);
	u16 (*get_hdb)(struct broadsheetfb_par *);

	
	void (*mmio_write)(struct broadsheetfb_par *, int type, u16);
	u16 (*mmio_read)(struct broadsheetfb_par *);
};
#endif
