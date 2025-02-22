/*
 *  Copyright (C) 2020, Samsung Electronics Co. Ltd. All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

#ifndef __SHUB_CMD_H
#define __SHUB_CMD_H

#include <linux/kernel.h>


/* command */
typedef enum
{
	CMD_ADD = 0,
	CMD_REMOVE,
	CMD_CHANGERATE,
	CMD_SETVALUE,
	CMD_GETVALUE,
	CMD_REPORT,
	CMD_MAX,
} SHUB_COMMAND;

/* sub command */
enum {
<<<<<<< HEAD
	SCREEN_STATE = 0,
	AP_RESUME,
	AP_SUSPEND,
	TIME_SYNC,
	LCD_LPM, // Reserved
=======
	AP_SHUTDOWN = 0,
	AP_RESUME,
	AP_SUSPEND,
	SYSTEM_TIME,
	RTC_TIME,
>>>>>>> upstream/android-13
	LCD_ON,
	LCD_OFF,
	POW_CONNECTED,
	POW_DISCONNECTED,
	CALL_IDLE,
	CALL_ACTIVE = 10,
<<<<<<< HEAD
	COPR_STATUS,
=======
	WHO_AM_I,
>>>>>>> upstream/android-13
	HUB_SYSTEM_INFO,
	MOBEAM_DATA,
	MOBEAM_HOP_TABLE,
	MOBEAM_HOP_COUNT,
	LIBRARY_DATA,
	LIBRARY_CURRENTSTATUS,
	LIBRARY_CURRENTSTATUS_BATCH,
	LIBRARY_VERSIONINFO,
	PEDOMETER_USERHEIGHT = 20,
	PEDOMETER_USERWEIGHT,
	PEDOMETER_USERGENDER,
	PEDOMETER_INFOUPDATETIME,
	SENSOR_FACTORY,
	SENSOR_FLUSH,
	SENSOR_AXIS,
	SENSOR_SPEC,
	SENSOR_REGISTER_DUMP,
	SENSOR_REGISTER_RW,
	PROXIMITY_THRESHOLD = 30,
	MAGNETIC_HWOFFSET,
	MAGNETIC_STATIC_MATRIX,
	PROXIMITY_OFFSET,
	LIGHT_COEF,
	CAL_DATA,
	ACCELOMETER_REACTIVE_ALERT,
	ACCELOMETER_LPF_ON_OFF,
	GYROSCOPE_TEMPERATURE_FACTORY,
	MAGNETIC_LOGGING,
	PROXIMITY_SETTING = 40,
	CURRENT_SYSTEM_TIME,
	GYROSCOPE_DPS_FACTORY,
	MOBEAM_BIT_LEN,
	MCU_SLEEP_TEST,
	SENSOR_NAME,
	PROXIMITY_ADDVAL,
	LIGHT_BRIGHTNESS,
	LIGHT_AB_HYSTERESIS,
	LIGHT_CAM_STATUS,
	LIGHT_SET_SAMPLING = 50,
	HALL_IC_STATUS,
	PROXIMITY_SETTING_MODE,
	LIGHT_FACTORY_STATE,
	LIGHT_PRJ_REGION,
	DMA_ADDRESS,
	PM_STATE,
	NO_EVENT_CHECK,
	MOTOR_ON,
	MOTOR_OFF,
	HUB_SYSTEM_CHECK = 60,
	PROXIMITY_LED_TEST,
	PROXIMITY_CALL_MIN,
	DDI_COPR,
	TEST_COPR,
	COPR_ROIX,
<<<<<<< HEAD
	CMD_CTS_STATE_NOTIFICATION,
=======
	PANEL_STATE,
>>>>>>> upstream/android-13
	DEBUG_INFO,
	MAGNETIC_COVER_MATRIX,
	EXTRA_EXTERNAL_CLIENT_ADDED,
	EXTRA_EXTERNAL_CLIENT_REMOVED,
<<<<<<< HEAD
	TYPE_OPTIONAL_PROXIMITY = 71, // TYPE_OPTIONAL_PROXIMITY in SensorService side. @ref ISensorHubCmdProtocol.java
	// 72 (0x48)                  // DISPLAY_REFRESH_RATE @ref SensorAdditionalInfo.java
	// 73 (0x49)                  // WIFI_DATA_ACTIVITY_ @ref SensorAdditionalInfo.java
	// 74 (0x50)                  // SENSORHUB_INFO_POWERSHARE_ @ref MotionRecognitionService.java
	MODEL_NAME_INFO = 75,
	CMD_VALUE_MAX,
};

/* common operation for each sensor */
#define DATA_INJECTION 125

=======
	CMD_VALUE_MAX,
};
>>>>>>> upstream/android-13
#endif
