<<<<<<< HEAD
/*
 * Copyright (C) 2014 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2015 Jakub Kicinski <kubakici@wp.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2014 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2015 Jakub Kicinski <kubakici@wp.pl>
>>>>>>> upstream/android-13
 */

#ifndef __MT76X0U_MCU_H
#define __MT76X0U_MCU_H

<<<<<<< HEAD
struct mt76x0_dev;

/* Register definitions */
#define MT_MCU_RESET_CTL		0x070C
#define MT_MCU_INT_LEVEL		0x0718
#define MT_MCU_COM_REG0			0x0730
#define MT_MCU_COM_REG1			0x0734
#define MT_MCU_COM_REG2			0x0738
#define MT_MCU_COM_REG3			0x073C
=======
#include "../mt76x02_mcu.h"

struct mt76x02_dev;
>>>>>>> upstream/android-13

#define MT_MCU_IVB_SIZE			0x40
#define MT_MCU_DLM_OFFSET		0x80000

<<<<<<< HEAD
#define MT_MCU_MEMMAP_WLAN		0x00410000
=======
>>>>>>> upstream/android-13
/* We use same space for BBP as for MAC regs
 * #define MT_MCU_MEMMAP_BBP		0x40000000
 */
#define MT_MCU_MEMMAP_RF		0x80000000

<<<<<<< HEAD
#define INBAND_PACKET_MAX_LEN		192

enum mcu_cmd {
	CMD_FUN_SET_OP = 1,
	CMD_LOAD_CR = 2,
	CMD_INIT_GAIN_OP = 3,
	CMD_DYNC_VGA_OP = 6,
	CMD_TDLS_CH_SW = 7,
	CMD_BURST_WRITE = 8,
	CMD_READ_MODIFY_WRITE = 9,
	CMD_RANDOM_READ = 10,
	CMD_BURST_READ = 11,
	CMD_RANDOM_WRITE = 12,
	CMD_LED_MODE_OP = 16,
	CMD_POWER_SAVING_OP = 20,
	CMD_WOW_CONFIG = 21,
	CMD_WOW_QUERY = 22,
	CMD_WOW_FEATURE = 24,
	CMD_CARRIER_DETECT_OP = 28,
	CMD_RADOR_DETECT_OP = 29,
	CMD_SWITCH_CHANNEL_OP = 30,
	CMD_CALIBRATION_OP = 31,
	CMD_BEACON_OP = 32,
	CMD_ANTENNA_OP = 33,
};

enum mcu_function {
	Q_SELECT = 1,
	BW_SETTING = 2,
	ATOMIC_TSSI_SETTING = 5,
};

enum mcu_power_mode {
	RADIO_OFF = 0x30,
	RADIO_ON = 0x31,
	RADIO_OFF_AUTO_WAKEUP = 0x32,
	RADIO_OFF_ADVANCE = 0x33,
	RADIO_ON_ADVANCE = 0x34,
};

=======
>>>>>>> upstream/android-13
enum mcu_calibrate {
	MCU_CAL_R = 1,
	MCU_CAL_RXDCOC,
	MCU_CAL_LC,
	MCU_CAL_LOFT,
	MCU_CAL_TXIQ,
	MCU_CAL_BW,
	MCU_CAL_DPD,
	MCU_CAL_RXIQ,
	MCU_CAL_TXDCOC,
	MCU_CAL_RX_GROUP_DELAY,
	MCU_CAL_TX_GROUP_DELAY,
<<<<<<< HEAD
};

int mt76x0_mcu_init(struct mt76x0_dev *dev);
int mt76x0_mcu_cmd_init(struct mt76x0_dev *dev);
void mt76x0_mcu_cmd_deinit(struct mt76x0_dev *dev);

int
mt76x0_mcu_calibrate(struct mt76x0_dev *dev, enum mcu_calibrate cal, u32 val);

int
mt76x0_mcu_function_select(struct mt76x0_dev *dev, enum mcu_function func, u32 val);
=======
	MCU_CAL_VCO,
	MCU_CAL_NO_SIGNAL = 0xfe,
	MCU_CAL_FULL = 0xff,
};

int mt76x0e_mcu_init(struct mt76x02_dev *dev);
int mt76x0u_mcu_init(struct mt76x02_dev *dev);
static inline int mt76x0_firmware_running(struct mt76x02_dev *dev)
{
	return mt76_rr(dev, MT_MCU_COM_REG0) == 1;
}
>>>>>>> upstream/android-13

#endif
