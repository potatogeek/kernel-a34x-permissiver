/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _DRM_SYSFS_H_
#define _DRM_SYSFS_H_

struct drm_device;
struct device;
<<<<<<< HEAD
=======
struct drm_connector;
struct drm_property;
>>>>>>> upstream/android-13

int drm_class_device_register(struct device *dev);
void drm_class_device_unregister(struct device *dev);

void drm_sysfs_hotplug_event(struct drm_device *dev);
<<<<<<< HEAD

=======
void drm_sysfs_connector_status_event(struct drm_connector *connector,
				      struct drm_property *property);
>>>>>>> upstream/android-13
#endif
