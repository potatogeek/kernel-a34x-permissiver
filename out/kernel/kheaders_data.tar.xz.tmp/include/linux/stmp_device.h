

#ifndef __STMP_DEVICE_H__
#define __STMP_DEVICE_H__

#define STMP_OFFSET_REG_SET	0x4
#define STMP_OFFSET_REG_CLR	0x8
#define STMP_OFFSET_REG_TOG	0xc

extern int stmp_reset_block(void __iomem *);
#endif 
