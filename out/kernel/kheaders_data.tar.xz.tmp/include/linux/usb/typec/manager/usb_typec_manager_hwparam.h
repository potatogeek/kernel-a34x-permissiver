

#ifndef __USB_TYPEC_MANAGER_HWPARAM_H__
#define __USB_TYPEC_MANAGER_HWPARAM_H__

void water_dry_time_update(int mode);
void wVbus_time_update(int status);
unsigned long manager_hw_param_update(int param);
void manager_hw_param_init(void);
void usb_enum_hw_param_data_update(int speed);

#endif 
