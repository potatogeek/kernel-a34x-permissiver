

#ifndef _LINUX_WMI_H
#define _LINUX_WMI_H

#include <linux/device.h>
#include <linux/acpi.h>
#include <uapi/linux/wmi.h>

struct wmi_device {
	struct device dev;

	 
	bool setable;
};


extern acpi_status wmidev_evaluate_method(struct wmi_device *wdev,
					  u8 instance, u32 method_id,
					  const struct acpi_buffer *in,
					  struct acpi_buffer *out);


extern union acpi_object *wmidev_block_query(struct wmi_device *wdev,
					     u8 instance);

extern int set_required_buffer_size(struct wmi_device *wdev, u64 length);

struct wmi_device_id {
	const char *guid_string;
};

struct wmi_driver {
	struct device_driver driver;
	const struct wmi_device_id *id_table;

	int (*probe)(struct wmi_device *wdev);
	int (*remove)(struct wmi_device *wdev);
	void (*notify)(struct wmi_device *device, union acpi_object *data);
	long (*filter_callback)(struct wmi_device *wdev, unsigned int cmd,
				struct wmi_ioctl_buffer *arg);
};

extern int __must_check __wmi_driver_register(struct wmi_driver *driver,
					      struct module *owner);
extern void wmi_driver_unregister(struct wmi_driver *driver);
#define wmi_driver_register(driver) __wmi_driver_register((driver), THIS_MODULE)

#define module_wmi_driver(__wmi_driver) \
	module_driver(__wmi_driver, wmi_driver_register, \
		      wmi_driver_unregister)

#endif
