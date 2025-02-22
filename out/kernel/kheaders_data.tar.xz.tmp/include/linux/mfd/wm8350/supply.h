

#ifndef __LINUX_MFD_WM8350_SUPPLY_H_
#define __LINUX_MFD_WM8350_SUPPLY_H_

#include <linux/mutex.h>
#include <linux/power_supply.h>


#define WM8350_BATTERY_CHARGER_CONTROL_1        0xA8
#define WM8350_BATTERY_CHARGER_CONTROL_2        0xA9
#define WM8350_BATTERY_CHARGER_CONTROL_3        0xAA


#define WM8350_CHG_ENA_R168                     0x8000
#define WM8350_CHG_THR                          0x2000
#define WM8350_CHG_EOC_SEL_MASK                 0x1C00
#define WM8350_CHG_TRICKLE_TEMP_CHOKE           0x0200
#define WM8350_CHG_TRICKLE_USB_CHOKE            0x0100
#define WM8350_CHG_RECOVER_T                    0x0080
#define WM8350_CHG_END_ACT                      0x0040
#define WM8350_CHG_FAST                         0x0020
#define WM8350_CHG_FAST_USB_THROTTLE            0x0010
#define WM8350_CHG_NTC_MON                      0x0008
#define WM8350_CHG_BATT_HOT_MON                 0x0004
#define WM8350_CHG_BATT_COLD_MON                0x0002
#define WM8350_CHG_CHIP_TEMP_MON                0x0001


#define WM8350_CHG_ACTIVE                       0x8000
#define WM8350_CHG_PAUSE                        0x4000
#define WM8350_CHG_STS_MASK                     0x3000
#define WM8350_CHG_TIME_MASK                    0x0F00
#define WM8350_CHG_MASK_WALL_FB                 0x0080
#define WM8350_CHG_TRICKLE_SEL                  0x0040
#define WM8350_CHG_VSEL_MASK                    0x0030
#define WM8350_CHG_ISEL_MASK                    0x000F
#define WM8350_CHG_STS_OFF                      0x0000
#define WM8350_CHG_STS_TRICKLE                  0x1000
#define WM8350_CHG_STS_FAST                     0x2000


#define WM8350_CHG_THROTTLE_T_MASK              0x0060
#define WM8350_CHG_SMART                        0x0010
#define WM8350_CHG_TIMER_ADJT_MASK              0x000F


#define WM8350_IRQ_CHG_BAT_HOT			0
#define WM8350_IRQ_CHG_BAT_COLD			1
#define WM8350_IRQ_CHG_BAT_FAIL			2
#define WM8350_IRQ_CHG_TO			3
#define WM8350_IRQ_CHG_END			4
#define WM8350_IRQ_CHG_START			5
#define WM8350_IRQ_CHG_FAST_RDY			6
#define WM8350_IRQ_CHG_VBATT_LT_3P9		10
#define WM8350_IRQ_CHG_VBATT_LT_3P1		11
#define WM8350_IRQ_CHG_VBATT_LT_2P85		12


#define WM8350_CHG_TRICKLE_50mA			(0 << 6)
#define WM8350_CHG_TRICKLE_100mA		(1 << 6)
#define WM8350_CHG_4_05V			(0 << 4)
#define WM8350_CHG_4_10V			(1 << 4)
#define WM8350_CHG_4_15V			(2 << 4)
#define WM8350_CHG_4_20V			(3 << 4)
#define WM8350_CHG_FAST_LIMIT_mA(x)		((x / 50) & 0xf)
#define WM8350_CHG_EOC_mA(x)			(((x - 10) & 0x7) << 10)
#define WM8350_CHG_TRICKLE_3_1V			(0 << 13)
#define WM8350_CHG_TRICKLE_3_9V			(1 << 13)


#define WM8350_USB_VOLTAGE_READBACK             0x9C
#define WM8350_LINE_VOLTAGE_READBACK            0x9D
#define WM8350_BATT_VOLTAGE_READBACK            0x9E


#define WM8350_IRQ_USB_LIMIT			15
#define WM8350_IRQ_EXT_USB_FB			36
#define WM8350_IRQ_EXT_WALL_FB			37
#define WM8350_IRQ_EXT_BAT_FB			38


struct wm8350_charger_policy {

	
	int eoc_mA;		
	int charge_mV;		
	int fast_limit_mA;	
	int fast_limit_USB_mA;	
	int charge_timeout;	
	int trickle_start_mV;	
	int trickle_charge_mA;	
	int trickle_charge_USB_mA;	
};

struct wm8350_power {
	struct platform_device *pdev;
	struct power_supply *battery;
	struct power_supply *usb;
	struct power_supply *ac;
	struct wm8350_charger_policy *policy;

	int rev_g_coeff;
};

#endif
