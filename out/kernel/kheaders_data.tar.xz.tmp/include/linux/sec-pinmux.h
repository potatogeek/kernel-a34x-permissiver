
#ifndef __SEC_PINMUX_H
#define __SEC_PINMUX_H

#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/secgpio_dvs.h>
#include <dt-bindings/pinctrl/mt65xx.h>

enum gpiomux_func {
	GPIOMUX_FUNC_GPIO = 0,
	GPIOMUX_FUNC_1,
	GPIOMUX_FUNC_2,
	GPIOMUX_FUNC_3,
	GPIOMUX_FUNC_4,
	GPIOMUX_FUNC_5,
	GPIOMUX_FUNC_6,
	GPIOMUX_FUNC_7,
};

enum gpiomux_dir {
	GPIOMUX_IN = 0,
	GPIOMUX_OUT,
};

enum gpiomux_level {
	GPIOMUX_LOW = 0,
	GPIOMUX_HIGH,
};

enum gpiomux_pull_en {
	GPIOMUX_PULL_DIS = 0, 
	GPIOMUX_PULL_EN,
	GPIOMUX_PULL_NONE = MTK_PUPD_SET_R1R0_00,
	GPIOMUX_PULL1,
	GPIOMUX_PULL2,
	GPIOMUX_PULL3,
};

enum gpiomux_pull {
	GPIOMUX_PULL_DOWN = 0,
	GPIOMUX_PULL_UP,
};

struct gpiomux_setting {
	int func;
	int dir;
	u32 pull_en;
	u32 pull;
};

#endif 
