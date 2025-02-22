

#ifndef _WM8962_PDATA_H
#define _WM8962_PDATA_H

#define WM8962_MAX_GPIO 6


#define WM8962_GPIO_SET 0x10000

#define WM8962_GPIO_FN_CLKOUT           0
#define WM8962_GPIO_FN_LOGIC            1
#define WM8962_GPIO_FN_SDOUT            2
#define WM8962_GPIO_FN_IRQ              3
#define WM8962_GPIO_FN_THERMAL          4
#define WM8962_GPIO_FN_PLL2_LOCK        6
#define WM8962_GPIO_FN_PLL3_LOCK        7
#define WM8962_GPIO_FN_FLL_LOCK         9
#define WM8962_GPIO_FN_DRC_ACT         10
#define WM8962_GPIO_FN_WSEQ_DONE       11
#define WM8962_GPIO_FN_ALC_NG_ACT      12
#define WM8962_GPIO_FN_ALC_PEAK_LIMIT  13
#define WM8962_GPIO_FN_ALC_SATURATION  14
#define WM8962_GPIO_FN_ALC_LEVEL_THR   15
#define WM8962_GPIO_FN_ALC_LEVEL_LOCK  16
#define WM8962_GPIO_FN_FIFO_ERR        17
#define WM8962_GPIO_FN_OPCLK           18
#define WM8962_GPIO_FN_DMICCLK         19
#define WM8962_GPIO_FN_DMICDAT         20
#define WM8962_GPIO_FN_MICD            21
#define WM8962_GPIO_FN_MICSCD          22

struct wm8962_pdata {
	struct clk *mclk;
	int gpio_base;
	u32 gpio_init[WM8962_MAX_GPIO];

	
	u32 mic_cfg;

	bool irq_active_low;

	bool spk_mono;   

	
	bool in4_dc_measure;
};

#endif
