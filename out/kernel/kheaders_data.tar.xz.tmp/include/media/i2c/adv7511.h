/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef ADV7511_H
#define ADV7511_H


#define ADV7511_MONITOR_DETECT 0
#define ADV7511_EDID_DETECT 1


struct adv7511_monitor_detect {
	int present;
};

struct adv7511_edid_detect {
	int present;
	int segment;
	uint16_t phys_addr;
};

struct adv7511_platform_data {
	u8 i2c_edid;
	u8 i2c_cec;
	u8 i2c_pktmem;
	u32 cec_clk;
};

#endif
