

#ifndef _AB8500_BM_H
#define _AB8500_BM_H

#include <linux/kernel.h>
#include <linux/mfd/abx500.h>


#define AB8500_MAIN_WDOG_CTRL_REG	0x01
#define AB8500_LOW_BAT_REG		0x03
#define AB8500_BATT_OK_REG		0x04

#define AB8500_USB_LINE_STAT_REG	0x80
#define AB8500_USB_LINE_CTRL2_REG	0x82
#define AB8500_USB_LINK1_STAT_REG	0x94


#define AB8500_CH_STATUS1_REG		0x00
#define AB8500_CH_STATUS2_REG		0x01
#define AB8500_CH_USBCH_STAT1_REG	0x02
#define AB8500_CH_USBCH_STAT2_REG	0x03
#define AB8540_CH_USBCH_STAT3_REG	0x04
#define AB8500_CH_STAT_REG		0x05


#define AB8500_CH_VOLT_LVL_REG		0x40
#define AB8500_CH_VOLT_LVL_MAX_REG	0x41  
#define AB8500_CH_OPT_CRNTLVL_REG	0x42
#define AB8500_CH_OPT_CRNTLVL_MAX_REG	0x43  
#define AB8500_CH_WD_TIMER_REG		0x50
#define AB8500_CHARG_WD_CTRL		0x51
#define AB8500_BTEMP_HIGH_TH		0x52
#define AB8500_LED_INDICATOR_PWM_CTRL	0x53
#define AB8500_LED_INDICATOR_PWM_DUTY	0x54
#define AB8500_BATT_OVV			0x55
#define AB8500_CHARGER_CTRL		0x56
#define AB8500_BAT_CTRL_CURRENT_SOURCE	0x60  


#define AB8500_MCH_CTRL1		0x80
#define AB8500_MCH_CTRL2		0x81
#define AB8500_MCH_IPT_CURLVL_REG	0x82
#define AB8500_CH_WD_REG		0x83


#define AB8500_USBCH_CTRL1_REG		0xC0
#define AB8500_USBCH_CTRL2_REG		0xC1
#define AB8500_USBCH_IPT_CRNTLVL_REG	0xC2
#define AB8540_USB_PP_MODE_REG		0xC5
#define AB8540_USB_PP_CHR_REG		0xC6


#define AB8500_GASG_CC_CTRL_REG		0x00
#define AB8500_GASG_CC_ACCU1_REG	0x01
#define AB8500_GASG_CC_ACCU2_REG	0x02
#define AB8500_GASG_CC_ACCU3_REG	0x03
#define AB8500_GASG_CC_ACCU4_REG	0x04
#define AB8500_GASG_CC_SMPL_CNTRL_REG	0x05
#define AB8500_GASG_CC_SMPL_CNTRH_REG	0x06
#define AB8500_GASG_CC_SMPL_CNVL_REG	0x07
#define AB8500_GASG_CC_SMPL_CNVH_REG	0x08
#define AB8500_GASG_CC_CNTR_AVGOFF_REG	0x09
#define AB8500_GASG_CC_OFFSET_REG	0x0A
#define AB8500_GASG_CC_NCOV_ACCU	0x10
#define AB8500_GASG_CC_NCOV_ACCU_CTRL	0x11
#define AB8500_GASG_CC_NCOV_ACCU_LOW	0x12
#define AB8500_GASG_CC_NCOV_ACCU_MED	0x13
#define AB8500_GASG_CC_NCOV_ACCU_HIGH	0x14


#define AB8500_IT_SOURCE2_REG		0x01
#define AB8500_IT_SOURCE21_REG		0x14


#define AB8500_RTC_BACKUP_CHG_REG	0x0C
#define AB8500_RTC_CC_CONF_REG		0x01
#define AB8500_RTC_CTRL_REG		0x0B
#define AB8500_RTC_CTRL1_REG		0x11


#define AB8500_OTP_CONF_15		0x0E


#define ADC_RESOLUTION			1024
#define ADC_CH_MAIN_MIN			0
#define ADC_CH_MAIN_MAX			20030
#define ADC_CH_VBUS_MIN			0
#define ADC_CH_VBUS_MAX			20030
#define ADC_CH_VBAT_MIN			2300
#define ADC_CH_VBAT_MAX			4800
#define ADC_CH_BKBAT_MIN		0
#define ADC_CH_BKBAT_MAX		3200


#define MAIN_CH_IP_CUR_0P9A		0x80
#define MAIN_CH_IP_CUR_1P0A		0x90
#define MAIN_CH_IP_CUR_1P1A		0xA0
#define MAIN_CH_IP_CUR_1P2A		0xB0
#define MAIN_CH_IP_CUR_1P3A		0xC0
#define MAIN_CH_IP_CUR_1P4A		0xD0
#define MAIN_CH_IP_CUR_1P5A		0xE0


#define CH_VOL_LVL_3P5			0x00
#define CH_VOL_LVL_4P0			0x14
#define CH_VOL_LVL_4P05			0x16
#define CH_VOL_LVL_4P1			0x1B
#define CH_VOL_LVL_4P15			0x20
#define CH_VOL_LVL_4P2			0x25
#define CH_VOL_LVL_4P6			0x4D


#define CH_OP_CUR_LVL_0P1		0x00
#define CH_OP_CUR_LVL_0P2		0x01
#define CH_OP_CUR_LVL_0P3		0x02
#define CH_OP_CUR_LVL_0P4		0x03
#define CH_OP_CUR_LVL_0P5		0x04
#define CH_OP_CUR_LVL_0P6		0x05
#define CH_OP_CUR_LVL_0P7		0x06
#define CH_OP_CUR_LVL_0P8		0x07
#define CH_OP_CUR_LVL_0P9		0x08
#define CH_OP_CUR_LVL_1P4		0x0D
#define CH_OP_CUR_LVL_1P5		0x0E
#define CH_OP_CUR_LVL_1P6		0x0F
#define CH_OP_CUR_LVL_2P		0x3F


#define BTEMP_HIGH_TH_57_0		0x00
#define BTEMP_HIGH_TH_52		0x01
#define BTEMP_HIGH_TH_57_1		0x02
#define BTEMP_HIGH_TH_62		0x03


#define USB_0P1A			100
#define USB_0P2A			200
#define USB_0P3A			300
#define USB_0P4A			400
#define USB_0P5A			500

#define LOW_BAT_3P1V			0x20
#define LOW_BAT_2P3V			0x00
#define LOW_BAT_RESET			0x01
#define LOW_BAT_ENABLE			0x01


#define BUP_ICH_SEL_50UA		0x00
#define BUP_ICH_SEL_150UA		0x04
#define BUP_ICH_SEL_300UA		0x08
#define BUP_ICH_SEL_700UA		0x0C

enum bup_vch_sel {
	BUP_VCH_SEL_2P5V,
	BUP_VCH_SEL_2P6V,
	BUP_VCH_SEL_2P8V,
	BUP_VCH_SEL_3P1V,
	
	BUP_VCH_SEL_2P7V,
	BUP_VCH_SEL_2P9V,
	BUP_VCH_SEL_3P0V,
	BUP_VCH_SEL_3P2V,
	BUP_VCH_SEL_3P3V,
};

#define BUP_VCH_RANGE		0x02
#define VBUP33_VRTCN		0x01


#define BATT_OVV_ENA			0x02
#define BATT_OVV_TH_3P7			0x00
#define BATT_OVV_TH_4P75		0x01


#define BATT_OVV_VALUE			4750


#define VBUS_OVV_SELECT_MASK		0x78
#define VBUS_OVV_SELECT_5P6V		0x00
#define VBUS_OVV_SELECT_5P7V		0x08
#define VBUS_OVV_SELECT_5P8V		0x10
#define VBUS_OVV_SELECT_5P9V		0x18
#define VBUS_OVV_SELECT_6P0V		0x20
#define VBUS_OVV_SELECT_6P1V		0x28
#define VBUS_OVV_SELECT_6P2V		0x30
#define VBUS_OVV_SELECT_6P3V		0x38

#define VBUS_AUTO_IN_CURR_LIM_ENA	0x04


#define RESET_ACCU			0x02
#define READ_REQ			0x01
#define CC_DEEP_SLEEP_ENA		0x02
#define CC_PWR_UP_ENA			0x01
#define CC_SAMPLES_40			0x28
#define RD_NCONV_ACCU_REQ		0x01
#define CC_CALIB			0x08
#define CC_INTAVGOFFSET_ENA		0x10
#define CC_MUXOFFSET			0x80
#define CC_INT_CAL_N_AVG_MASK		0x60
#define CC_INT_CAL_SAMPLES_16		0x40
#define CC_INT_CAL_SAMPLES_8		0x20
#define CC_INT_CAL_SAMPLES_4		0x00


#define RTC_BUP_CH_ENA			0x10


#define BAT_CTRL_7U_ENA			0x01
#define BAT_CTRL_20U_ENA		0x02
#define BAT_CTRL_18U_ENA		0x01
#define BAT_CTRL_16U_ENA		0x02
#define BAT_CTRL_CMP_ENA		0x04
#define FORCE_BAT_CTRL_CMP_HIGH		0x08
#define BAT_CTRL_PULL_UP_ENA		0x10


#define BATTERY_UNKNOWN			00


#define AB8505_RTC_PCUT_CTL_STATUS_REG	0x12
#define AB8505_RTC_PCUT_TIME_REG	0x13
#define AB8505_RTC_PCUT_MAX_TIME_REG	0x14
#define AB8505_RTC_PCUT_FLAG_TIME_REG	0x15
#define AB8505_RTC_PCUT_RESTART_REG	0x16
#define AB8505_RTC_PCUT_DEBOUNCE_REG	0x17


#define BUS_VSYS_VOL_SELECT_MASK		0x06
#define BUS_VSYS_VOL_SELECT_3P6V		0x00
#define BUS_VSYS_VOL_SELECT_3P325V		0x02
#define BUS_VSYS_VOL_SELECT_3P9V		0x04
#define BUS_VSYS_VOL_SELECT_4P3V		0x06
#define BUS_POWER_PATH_MODE_ENA			0x01
#define BUS_PP_PRECHG_CURRENT_MASK		0x0E
#define BUS_POWER_PATH_PRECHG_ENA		0x01


struct res_to_temp {
	int temp;
	int resist;
};


struct batres_vs_temp {
	int temp;
	int resist;
};


struct ab8500_fg;


struct ab8500_fg_parameters {
	int recovery_sleep_timer;
	int recovery_total_time;
	int init_timer;
	int init_discard_time;
	int init_total_time;
	int high_curr_time;
	int accu_charging;
	int accu_high_curr;
	int high_curr_threshold;
	int lowbat_threshold;
	int battok_falling_th_sel0;
	int battok_raising_th_sel1;
	int user_cap_limit;
	int maint_thres;
	bool pcut_enable;
	u8 pcut_max_time;
	u8 pcut_flag_time;
	u8 pcut_max_restart;
	u8 pcut_debunce_time;
};


struct ab8500_maxim_parameters {
	bool ena_maxi;
	int chg_curr;
	int wait_cycles;
	int charger_curr_step;
};


struct ab8500_bm_capacity_levels {
	int critical;
	int low;
	int normal;
	int high;
	int full;
};


struct ab8500_bm_charger_parameters {
	int usb_volt_max;
	int usb_curr_max;
	int ac_volt_max;
	int ac_curr_max;
};


struct ab8500_bm_data {
	int temp_under;
	int temp_low;
	int temp_high;
	int temp_over;
	int temp_interval_chg;
	int temp_interval_nochg;
	int main_safety_tmr_h;
	int usb_safety_tmr_h;
	int bkup_bat_v;
	int bkup_bat_i;
	bool no_maintenance;
	bool capacity_scaling;
	bool chg_unknown_bat;
	bool enable_overshoot;
	enum abx500_adc_therm adc_therm;
	int fg_res;
	int n_btypes;
	int batt_id;
	int interval_charging;
	int interval_not_charging;
	int temp_hysteresis;
	int gnd_lift_resistance;
	const struct ab8500_maxim_parameters *maxi;
	const struct ab8500_bm_capacity_levels *cap_levels;
	const struct ab8500_bm_charger_parameters *chg_params;
	const struct ab8500_fg_parameters *fg_params;
};

struct ab8500_btemp;
struct ab8500_gpadc;
struct ab8500_fg;

#ifdef CONFIG_AB8500_BM
extern struct abx500_bm_data ab8500_bm_data;

void ab8500_charger_usb_state_changed(u8 bm_usb_state, u16 mA);
struct ab8500_btemp *ab8500_btemp_get(void);
int ab8500_btemp_get_batctrl_temp(struct ab8500_btemp *btemp);
int ab8500_btemp_get_temp(struct ab8500_btemp *btemp);
struct ab8500_fg *ab8500_fg_get(void);
int ab8500_fg_inst_curr_blocking(struct ab8500_fg *dev);
int ab8500_fg_inst_curr_start(struct ab8500_fg *di);
int ab8500_fg_inst_curr_finalize(struct ab8500_fg *di, int *res);
int ab8500_fg_inst_curr_started(struct ab8500_fg *di);
int ab8500_fg_inst_curr_done(struct ab8500_fg *di);

#else
static struct abx500_bm_data ab8500_bm_data;
#endif
#endif 
