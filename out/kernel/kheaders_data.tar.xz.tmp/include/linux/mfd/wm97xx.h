

#ifndef __LINUX_MFD_WM97XX_H
#define __LINUX_MFD_WM97XX_H

struct regmap;
struct wm97xx_batt_pdata;
struct snd_ac97;

struct wm97xx_platform_data {
	struct snd_ac97 *ac97;
	struct regmap *regmap;
	struct wm97xx_batt_pdata *batt_pdata;
};

#endif
