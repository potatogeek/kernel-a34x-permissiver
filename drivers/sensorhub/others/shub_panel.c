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

#include "../utility/shub_utility.h"
#include "../comm/shub_comm.h"
#include "../sensor/light.h"
<<<<<<< HEAD
#include "../sensorhub/shub_device.h"
#include "../sensormanager/shub_sensor_type.h"
#include "../utility/shub_file_manager.h"
#include "shub_panel.h"
#include <linux/string.h>
#include <linux/notifier.h>

#if IS_ENABLED(CONFIG_SEC_PANEL_NOTIFIER_V2) && IS_ENABLED(CONFIG_SHUB_PANEL_NOTIFY)
#include <linux/sec_panel_notifier_v2.h>
#include "../sensor/light.h"
#include "../sensormanager/shub_sensor_type.h"

#define PANEL_MAX (1)

static struct panel_event_bl_data panel_event_data[PANEL_MAX];
static struct panel_event_dms_data panel_event_dms_data[PANEL_MAX];
static int panel_screen_mode[PANEL_MAX];
static u8 copr_state;
static u8 ub_state;
#endif

#define LCD_PANEL_LCD_TYPE "/sys/class/lcd/panel/lcd_type"
#define LCD_PANEL_SVC_OCTA "/sys/class/lcd/panel/SVC_OCTA"
#define SVC_OCTA_DATA_SIZE 23

static char panel_ubid[SVC_OCTA_DATA_SIZE] = {0,};

int get_panel_lcd_type(void)
=======
#include "../sensormanager/shub_sensor_type.h"
#include "../utility/shub_file_manager.h"
#include "shub_panel.h"

#include <linux/notifier.h>

#define LCD_PANEL_LCD_TYPE "/sys/class/lcd/panel/lcd_type"
static u8 lcd_type_flag;

static void get_panel_lcd_type(void)
>>>>>>> upstream/android-13
{
	char lcd_type_data[256];
	int ret;

	ret = shub_file_read(LCD_PANEL_LCD_TYPE, lcd_type_data, sizeof(lcd_type_data), 0);
	if (ret < 0) {
		shub_errf("file read error %d", ret);
<<<<<<< HEAD
		return ret;
	} else if (ret < 2){
		shub_errf("unexpected type = %s(%d)", lcd_type_data, ret);
		return ret;
	}


	if (strstr(lcd_type_data, SDC_STR))
		return SDC;
	else if (strstr(lcd_type_data, BOE_STR))
		return BOE;
	else if (strstr(lcd_type_data, CSOT_STR))
		return CSOT;
	else if (strstr(lcd_type_data, DTC_STR))
		return DTC;
	else if (strstr(lcd_type_data, Tianma_STR))
		return Tianma;
	else if (strstr(lcd_type_data, Skyworth_STR))
		return Skyworth;
	return OTHER;
=======
		return;
	} else if (ret < 2){
		shub_errf("unexpected type = %s(%d)", lcd_type_data, ret);
		return;
	}

	/*
	 * lcd_type_data[ret - 2], which type have different transmission ratio.
	 * [0 ~ 2] : 0.7%, [3] : 15%, [4] : 40%
	 */
	if (lcd_type_data[ret - 2] >= '0' && lcd_type_data[ret - 2] <= '2')
		lcd_type_flag = 0;
	else if (lcd_type_data[ret - 2] == '3')
		lcd_type_flag = 1;
	else
		lcd_type_flag = 2;

	shub_infof("lcd_type_flag : %d", lcd_type_flag);
>>>>>>> upstream/android-13
}

static int fm_ready_panel(struct notifier_block *this, unsigned long event, void *ptr)
{
<<<<<<< HEAD
	shub_infof("notify event %d", (int)event);
=======
	shub_infof("notify event %d", event);
>>>>>>> upstream/android-13
	return NOTIFY_OK;
}

static struct notifier_block fm_notifier = {
    .notifier_call = fm_ready_panel,
};

void init_shub_panel(void)
{
	shub_infof();
	register_file_manager_ready_callback(&fm_notifier);
}

void remove_shub_panel(void)
{
	shub_infof();
}

<<<<<<< HEAD
int get_panel_ubid(void)
{
	int ret;

	ret = shub_file_read(LCD_PANEL_SVC_OCTA, panel_ubid, sizeof(panel_ubid), 0);
	if (ret < 0) {
		shub_errf("file read error %d", ret);
		return ret;
	}

	return ret;
}

int save_panel_ubid(void)
{
	int ret = 0;

	get_panel_ubid();

	ret = shub_file_write_no_wait(UB_ID_FILE_PATH, panel_ubid, sizeof(panel_ubid), 0);
	if (ret != sizeof(panel_ubid)) {
=======
int save_panel_lcd_type(void)
{
	int ret = 0;

	get_panel_lcd_type();

	ret = shub_file_write_no_wait(UID_FILE_PATH, (char *)&lcd_type_flag, sizeof(lcd_type_flag), 0);
	if (ret != sizeof(lcd_type_flag)) {
>>>>>>> upstream/android-13
		shub_errf("failed");
		return -EIO;
	}

<<<<<<< HEAD
	shub_infof("ubid(%s)", panel_ubid);
=======
	shub_infof("save lcd_type_flag %d", lcd_type_flag);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
bool is_panel_ubid_changed(void)
{
	int ret = 0;
	char efs_panel_ubid[SVC_OCTA_DATA_SIZE] = {0,};

	ret = shub_file_read(UB_ID_FILE_PATH, efs_panel_ubid, sizeof(efs_panel_ubid), 0);
	if (ret != sizeof(panel_ubid)) {
		shub_infof("there is no efs_ub_id");
		return false;
	}
	ret = get_panel_ubid();
	if (ret != sizeof(panel_ubid)) {
		shub_errf("failed read get_ub_id");
		return false;
	}
	if (strncmp(efs_panel_ubid, panel_ubid, SVC_OCTA_DATA_SIZE) != 0) {
		shub_errf("previous_ubid(%s)", efs_panel_ubid);
		shub_errf("current_ubid(%s)", panel_ubid);
		return true;
	}

	return false;
}

#if IS_ENABLED(CONFIG_SEC_PANEL_NOTIFIER_V2) && IS_ENABLED(CONFIG_SHUB_PANEL_NOTIFY)
int send_panel_information(int display_index, struct panel_event_bl_data *evdata)
{
	int buf[5] = { display_index, evdata->level, evdata->aor, evdata->acl_status, evdata->gradual_acl_val };
	int ret = 0;

	//TODO: send brightness + aor_ratio information to sensorhub
	ret = shub_send_command(CMD_SETVALUE, SENSOR_TYPE_LIGHT,
					LIGHT_SUBCMD_PANEL_INFORMATION, (char *)&buf, sizeof(buf));

	return ret;
}

u8 get_lcd_status(void)
{
	struct shub_data_t *shub_data = get_shub_data();
	u8 ret = 0;

	if (ub_state == PANEL_EVENT_UB_CON_STATE_CONNECTED
		&& shub_data->lcd_status == LCD_ON)
		ret = LCD_ON;
	else
		ret = LCD_OFF;

	return ret;
}

int send_ub_state(void)
{
	int ret = 0;
	char enable = 0;

	if (ub_state == PANEL_EVENT_UB_CON_STATE_CONNECTED)
		enable = 1;
	else if (ub_state == PANEL_EVENT_UB_CON_STATE_DISCONNECTED)
		enable = 0;
	else
		return -EINVAL;

	ret = shub_send_command(CMD_SETVALUE, SENSOR_TYPE_LIGHT,
					LIGHT_SUBCMD_UB_CONNECTED, &enable, sizeof(enable));

	if (ub_state == PANEL_EVENT_UB_CON_STATE_DISCONNECTED)
		ret = shub_send_status(get_lcd_status());

	return ret;
}

int send_copr_state(void)
{
	int ret = 0;
	char enable = 0;

	if (copr_state == PANEL_EVENT_COPR_STATE_ENABLED)
		enable = 1;
	else if (copr_state == PANEL_EVENT_COPR_STATE_DISABLED)
		enable = 0;
	else
		return -EINVAL;

	ret = shub_send_command(CMD_SETVALUE, TYPE_HUB, COPR_STATUS, &enable, sizeof(enable));

	return ret;
}

int send_screen_mode_information(int display_index, int screen_mode)
{
	int buf[2] = { display_index, screen_mode };
	int ret = 0;

	ret = shub_send_command(CMD_SETVALUE, SENSOR_TYPE_LIGHT,
					LIGHT_SUBCMD_SCREEN_MODE_INFORMATION, (char *)&buf, sizeof(buf));

	return ret;
}

static int panel_notifier_callback(struct notifier_block *nb, unsigned long event, void *data)
{
	struct panel_notifier_event_data *evtdata = data;
	int index = evtdata->display_index;

	if (index >= PANEL_MAX)
	{
		shub_infof("invalid display_index=%d", index);
		return 0;
	}
	//shub_infof("event=%lu", event);

	if (event == PANEL_EVENT_BL_STATE_CHANGED) {
		struct shub_data_t *shub_data = get_shub_data();
		u32 brightness_resolution = 1;

		if (strcmp(shub_data->model_name, "S921") == 0 || strcmp(shub_data->model_name, "S926") == 0)
			brightness_resolution = 10;

		evtdata->d.bl.level = evtdata->d.bl.level / brightness_resolution;

		if (panel_event_data[evtdata->display_index].level != evtdata->d.bl.level) {
			// store these values for reset
			memcpy(&panel_event_data[index], &evtdata->d.bl, sizeof(struct panel_event_bl_data));
			shub_infof("PANEL_EVENT_BL_STATE_CHANGED, level(%d) aor(%d) acl_status(%d) acl_val(%d) resolution(%d)\n",
							evtdata->d.bl.level, evtdata->d.bl.aor, evtdata->d.bl.acl_status, evtdata->d.bl.gradual_acl_val, brightness_resolution);
			send_panel_information(index, &evtdata->d.bl);
		}
	} else if (event == PANEL_EVENT_UB_CON_STATE_CHANGED) {
		if (evtdata->state != PANEL_EVENT_UB_CON_STATE_CONNECTED
			&& evtdata->state != PANEL_EVENT_UB_CON_STATE_DISCONNECTED)	{
			shub_infof("PANEL_EVENT_UB_CON_CHANGED, event errno(%d)\n", evtdata->state);
		} else {
			ub_state = evtdata->state;
			shub_infof("PANEL_EVENT_UB_CON_CHANGED, state(%d)\n", ub_state);
			send_ub_state();
		}
	} else if (event == PANEL_EVENT_PANEL_STATE_CHANGED) {
		if (evtdata->state >= PANEL_EVENT_PANEL_STATE_OFF
			&& evtdata->state <= PANEL_EVENT_PANEL_STATE_LPM) {
			struct shub_data_t *shub_data = get_shub_data();

			if (evtdata->state == PANEL_EVENT_PANEL_STATE_OFF)
				shub_data->lcd_status = LCD_OFF;
			else if (evtdata->state == PANEL_EVENT_PANEL_STATE_ON)
				shub_data->lcd_status = LCD_ON;
			else if (evtdata->state == PANEL_EVENT_PANEL_STATE_LPM)
				shub_data->lcd_status = LCD_LPM;

			shub_infof("PANEL_EVENT_PANEL_STATE_CHANGED, event(%d) lcd_status(%d)\n",
							evtdata->state, shub_data->lcd_status);

			//shub_send_status(get_lcd_status());
		}
	} else if (event == PANEL_EVENT_COPR_STATE_CHANGED) {
		if (evtdata->state == PANEL_EVENT_COPR_STATE_ENABLED
			|| evtdata->state == PANEL_EVENT_COPR_STATE_DISABLED) {
			struct shub_data_t *shub_data = get_shub_data();
			copr_state = evtdata->state;
			shub_infof("PANEL_EVENT_COPR_STATE_CHANGED, event(%d) lcd_status(%d)\n",
							evtdata->state, shub_data->lcd_status);
			send_copr_state();
		}
	} else if (event == PANEL_EVENT_SCREEN_MODE_STATE_CHANGED) {
		memcpy(&panel_event_dms_data[index], &evtdata->d.dms, sizeof(struct panel_event_dms_data));
		panel_screen_mode[index] = evtdata->d.screen_mode;

		shub_infof("panel screen mode %d %d", evtdata->display_index, evtdata->d.screen_mode);

		send_screen_mode_information(index, evtdata->d.screen_mode);
	}

	return 0;
}

struct notifier_block panel_notify =  {
	.notifier_call = panel_notifier_callback,
};

void init_shub_panel_callback(void)
{
	int ret = 0;
	int i = 0;

	copr_state = PANEL_EVENT_COPR_STATE_ENABLED;
	ub_state = PANEL_EVENT_UB_CON_STATE_CONNECTED;

	for (i = 0; i < PANEL_MAX; i++) {
		panel_event_data[i].level = -1;
	}

	ret = panel_notifier_register(&panel_notify);
	if (ret < 0)
		shub_infof("panel_notifier_register failed(%d)", ret);

	shub_info();
}

void remove_shub_panel_callback(void)
{
	panel_notifier_unregister(&panel_notify);
}

void sync_panel_state(void)
{
	int i = 0;

	send_ub_state();
	send_copr_state();

	for (i = 0; i < PANEL_MAX; i++)
	{
		if (panel_event_data[i].level != -1)
			send_panel_information(i, &panel_event_data[i]);
		send_screen_mode_information(i, panel_screen_mode[i]);
	}
}
#else
void init_shub_panel_callback(void) {}
void remove_shub_panel_callback(void) {}
void sync_panel_state(void) {}
#endif
=======
bool is_lcd_changed(void)
{
	int ret = 0;
	u8 curr_lcd_type_flag = 0;

	get_panel_lcd_type();
	ret = shub_file_read(UID_FILE_PATH, &curr_lcd_type_flag, sizeof(curr_lcd_type_flag), 0);
	if (ret != sizeof(curr_lcd_type_flag)) {
		shub_errf("saved lcd type read failed %d", ret);
		return false;
	}
	shub_infof("%d -> %d", lcd_type_flag, curr_lcd_type_flag);
	return (lcd_type_flag != curr_lcd_type_flag);
}
>>>>>>> upstream/android-13
