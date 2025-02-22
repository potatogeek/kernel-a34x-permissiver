

#ifndef __LINUX_SND_WM5100_H
#define __LINUX_SND_WM5100_H

enum wm5100_in_mode {
	WM5100_IN_SE = 0,
	WM5100_IN_DIFF = 1,
	WM5100_IN_DMIC = 2,
};

enum wm5100_dmic_sup {
	WM5100_DMIC_SUP_MICVDD = 0,
	WM5100_DMIC_SUP_MICBIAS1 = 1,
	WM5100_DMIC_SUP_MICBIAS2 = 2,
	WM5100_DMIC_SUP_MICBIAS3 = 3,
};

enum wm5100_micdet_bias {
	WM5100_MICDET_MICBIAS1 = 0,
	WM5100_MICDET_MICBIAS2 = 1,
	WM5100_MICDET_MICBIAS3 = 2,
};

struct wm5100_jack_mode {
	enum wm5100_micdet_bias bias;
	int hp_pol;
	int micd_src;
};

#define WM5100_GPIO_SET 0x10000

struct wm5100_pdata {
	int reset;      
	int ldo_ena;    
	int hp_pol;     
	int irq_flags;
	int gpio_base;

	struct wm5100_jack_mode jack_modes[2];

	
	enum wm5100_in_mode in_mode[4];

	
	enum wm5100_dmic_sup dmic_sup[4];

	int gpio_defaults[6];
};

#endif
