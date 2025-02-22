
#ifndef LPC_ICH_H
#define LPC_ICH_H

#include <linux/platform_data/intel-spi.h>


#define ICH_RES_GPIO	0
#define ICH_RES_GPE0	1


enum {
	ICH_I3100_GPIO,
	ICH_V5_GPIO,
	ICH_V6_GPIO,
	ICH_V7_GPIO,
	ICH_V9_GPIO,
	ICH_V10CORP_GPIO,
	ICH_V10CONS_GPIO,
	AVOTON_GPIO,
};

struct lpc_ich_info {
	char name[32];
	unsigned int iTCO_version;
	unsigned int gpio_version;
	enum intel_spi_type spi_type;
	u8 use_gpio;
};

#endif
