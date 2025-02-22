
#ifndef __TXX9_NDFMC_H
#define __TXX9_NDFMC_H

#define NDFMC_PLAT_FLAG_USE_BSPRT	0x01
#define NDFMC_PLAT_FLAG_NO_RSTR		0x02
#define NDFMC_PLAT_FLAG_HOLDADD		0x04
#define NDFMC_PLAT_FLAG_DUMMYWRITE	0x08

struct txx9ndfmc_platform_data {
	unsigned int shift;
	unsigned int gbus_clock;
	unsigned int hold;		
	unsigned int spw;		
	unsigned int flags;
	unsigned char ch_mask;		
	unsigned char wp_mask;		
	unsigned char wide_mask;	
};

void txx9_ndfmc_init(unsigned long baseaddr,
		     const struct txx9ndfmc_platform_data *plat_data);

#endif 
