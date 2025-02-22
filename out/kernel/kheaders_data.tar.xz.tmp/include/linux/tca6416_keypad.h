

#ifndef _TCA6416_KEYS_H
#define _TCA6416_KEYS_H

#include <linux/types.h>

struct tca6416_button {
	
	int code;		
	int active_low;
	int type;		
};

struct tca6416_keys_platform_data {
	struct tca6416_button *buttons;
	int nbuttons;
	unsigned int rep:1;	
	uint16_t pinmask;
	uint16_t invert;
	int irq_is_gpio;
	int use_polling;	
};
#endif
