/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  thermal_core.h
 *
 *  Copyright (C) 2012  Intel Corp
 *  Author: Durgadoss R <durgadoss.r@intel.com>
 */

#ifndef __THERMAL_CORE_H__
#define __THERMAL_CORE_H__

#include <linux/device.h>
#include <linux/thermal.h>

<<<<<<< HEAD
/* Initial state of a cooling device during binding */
#define THERMAL_NO_TARGET -1UL

=======
#include "thermal_netlink.h"

/* Default Thermal Governor */
#if defined(CONFIG_THERMAL_DEFAULT_GOV_STEP_WISE)
#define DEFAULT_THERMAL_GOVERNOR       "step_wise"
#elif defined(CONFIG_THERMAL_DEFAULT_GOV_FAIR_SHARE)
#define DEFAULT_THERMAL_GOVERNOR       "fair_share"
#elif defined(CONFIG_THERMAL_DEFAULT_GOV_USER_SPACE)
#define DEFAULT_THERMAL_GOVERNOR       "user_space"
#elif defined(CONFIG_THERMAL_DEFAULT_GOV_POWER_ALLOCATOR)
#define DEFAULT_THERMAL_GOVERNOR       "power_allocator"
#endif

/* Initial state of a cooling device during binding */
#define THERMAL_NO_TARGET -1UL

/* Init section thermal table */
extern struct thermal_governor *__governor_thermal_table[];
extern struct thermal_governor *__governor_thermal_table_end[];

#define THERMAL_TABLE_ENTRY(table, name)			\
	static typeof(name) *__thermal_table_entry_##name	\
	__used __section("__" #table "_thermal_table") = &name

#define THERMAL_GOVERNOR_DECLARE(name)	THERMAL_TABLE_ENTRY(governor, name)

#define for_each_governor_table(__governor)		\
	for (__governor = __governor_thermal_table;	\
	     __governor < __governor_thermal_table_end;	\
	     __governor++)

int for_each_thermal_zone(int (*cb)(struct thermal_zone_device *, void *),
			  void *);

int for_each_thermal_cooling_device(int (*cb)(struct thermal_cooling_device *,
					      void *), void *);

int for_each_thermal_governor(int (*cb)(struct thermal_governor *, void *),
			      void *thermal_governor);

struct thermal_zone_device *thermal_zone_get_by_id(int id);

struct thermal_attr {
	struct device_attribute attr;
	char name[THERMAL_NAME_LENGTH];
};

static inline bool cdev_is_power_actor(struct thermal_cooling_device *cdev)
{
	return cdev->ops->get_requested_power && cdev->ops->state2power &&
		cdev->ops->power2state;
}

void thermal_cdev_update(struct thermal_cooling_device *);
void __thermal_cdev_update(struct thermal_cooling_device *cdev);

/**
 * struct thermal_trip - representation of a point in temperature domain
 * @np: pointer to struct device_node that this trip point was created from
 * @temperature: temperature value in miliCelsius
 * @hysteresis: relative hysteresis in miliCelsius
 * @type: trip point type
 */
struct thermal_trip {
	struct device_node *np;
	int temperature;
	int hysteresis;
	enum thermal_trip_type type;
};

int get_tz_trend(struct thermal_zone_device *tz, int trip);

struct thermal_instance *
get_thermal_instance(struct thermal_zone_device *tz,
		     struct thermal_cooling_device *cdev,
		     int trip);

>>>>>>> upstream/android-13
/*
 * This structure is used to describe the behavior of
 * a certain cooling device on a certain trip point
 * in a certain thermal zone
 */
struct thermal_instance {
	int id;
	char name[THERMAL_NAME_LENGTH];
	struct thermal_zone_device *tz;
	struct thermal_cooling_device *cdev;
	int trip;
	bool initialized;
	unsigned long upper;	/* Highest cooling state for this trip point */
	unsigned long lower;	/* Lowest cooling state for this trip point */
	unsigned long target;	/* expected cooling state */
	char attr_name[THERMAL_NAME_LENGTH];
	struct device_attribute attr;
	char weight_attr_name[THERMAL_NAME_LENGTH];
	struct device_attribute weight_attr;
	struct list_head tz_node; /* node in tz->thermal_instances */
	struct list_head cdev_node; /* node in cdev->thermal_instances */
	unsigned int weight; /* The weight of the cooling device */
};

#define to_thermal_zone(_dev) \
	container_of(_dev, struct thermal_zone_device, device)

#define to_cooling_device(_dev)	\
	container_of(_dev, struct thermal_cooling_device, device)

int thermal_register_governor(struct thermal_governor *);
void thermal_unregister_governor(struct thermal_governor *);
<<<<<<< HEAD
void thermal_zone_device_rebind_exception(struct thermal_zone_device *,
					  const char *, size_t);
void thermal_zone_device_unbind_exception(struct thermal_zone_device *,
					  const char *, size_t);
int thermal_zone_device_set_policy(struct thermal_zone_device *, char *);
int thermal_build_list_of_policies(char *buf);

=======
int thermal_zone_device_set_policy(struct thermal_zone_device *, char *);
int thermal_build_list_of_policies(char *buf);

/* Helpers */
void thermal_zone_set_trips(struct thermal_zone_device *tz);
void thermal_set_delay_jiffies(unsigned long *delay_jiffies, int delay_ms);

>>>>>>> upstream/android-13
/* sysfs I/F */
int thermal_zone_create_device_groups(struct thermal_zone_device *, int);
void thermal_zone_destroy_device_groups(struct thermal_zone_device *);
void thermal_cooling_device_setup_sysfs(struct thermal_cooling_device *);
void thermal_cooling_device_destroy_sysfs(struct thermal_cooling_device *cdev);
/* used only at binding time */
ssize_t trip_point_show(struct device *, struct device_attribute *, char *);
<<<<<<< HEAD
ssize_t trip_point_store(struct device *, struct device_attribute *,
			 const char *, size_t);
=======
>>>>>>> upstream/android-13
ssize_t weight_show(struct device *, struct device_attribute *, char *);
ssize_t weight_store(struct device *, struct device_attribute *, const char *,
		     size_t);

#ifdef CONFIG_THERMAL_STATISTICS
void thermal_cooling_device_stats_update(struct thermal_cooling_device *cdev,
					 unsigned long new_state);
#else
static inline void
thermal_cooling_device_stats_update(struct thermal_cooling_device *cdev,
				    unsigned long new_state) {}
#endif /* CONFIG_THERMAL_STATISTICS */

<<<<<<< HEAD
#ifdef CONFIG_THERMAL_GOV_STEP_WISE
int thermal_gov_step_wise_register(void);
void thermal_gov_step_wise_unregister(void);
#else
static inline int thermal_gov_step_wise_register(void) { return 0; }
static inline void thermal_gov_step_wise_unregister(void) {}
#endif /* CONFIG_THERMAL_GOV_STEP_WISE */

#ifdef CONFIG_THERMAL_GOV_FAIR_SHARE
int thermal_gov_fair_share_register(void);
void thermal_gov_fair_share_unregister(void);
#else
static inline int thermal_gov_fair_share_register(void) { return 0; }
static inline void thermal_gov_fair_share_unregister(void) {}
#endif /* CONFIG_THERMAL_GOV_FAIR_SHARE */

#ifdef CONFIG_THERMAL_GOV_BANG_BANG
int thermal_gov_bang_bang_register(void);
void thermal_gov_bang_bang_unregister(void);
#else
static inline int thermal_gov_bang_bang_register(void) { return 0; }
static inline void thermal_gov_bang_bang_unregister(void) {}
#endif /* CONFIG_THERMAL_GOV_BANG_BANG */

#ifdef CONFIG_THERMAL_GOV_USER_SPACE
int thermal_gov_user_space_register(void);
void thermal_gov_user_space_unregister(void);
#else
static inline int thermal_gov_user_space_register(void) { return 0; }
static inline void thermal_gov_user_space_unregister(void) {}
#endif /* CONFIG_THERMAL_GOV_USER_SPACE */

#ifdef CONFIG_THERMAL_GOV_POWER_ALLOCATOR
int thermal_gov_power_allocator_register(void);
void thermal_gov_power_allocator_unregister(void);
#else
static inline int thermal_gov_power_allocator_register(void) { return 0; }
static inline void thermal_gov_power_allocator_unregister(void) {}
#endif /* CONFIG_THERMAL_GOV_POWER_ALLOCATOR */

#ifdef CONFIG_THERMAL_GOV_BACKWARD_COMPATIBLE
int thermal_gov_backward_compatible_register(void);
void thermal_gov_backward_compatible_unregister(void);
#else
static inline int thermal_gov_backward_compatible_register(void) { return 0; }
static inline void thermal_gov_backward_compatible_unregister(void) {}
#endif /* CONFIG_THERMAL_GOV_BACKWARD_COMPATIBLE */

/* device tree support */
#ifdef CONFIG_THERMAL_OF
int of_parse_thermal_zones(void);
void of_thermal_destroy_zones(void);
=======
/* device tree support */
#ifdef CONFIG_THERMAL_OF
int of_parse_thermal_zones(void);
>>>>>>> upstream/android-13
int of_thermal_get_ntrips(struct thermal_zone_device *);
bool of_thermal_is_trip_valid(struct thermal_zone_device *, int);
const struct thermal_trip *
of_thermal_get_trip_points(struct thermal_zone_device *);
<<<<<<< HEAD
void of_thermal_handle_trip(struct thermal_zone_device *tz);
void of_thermal_handle_trip_temp(struct thermal_zone_device *tz,
					int trip_temp);
#else
static inline int of_parse_thermal_zones(void) { return 0; }
static inline void of_thermal_destroy_zones(void) { }
=======
#else
static inline int of_parse_thermal_zones(void) { return 0; }
>>>>>>> upstream/android-13
static inline int of_thermal_get_ntrips(struct thermal_zone_device *tz)
{
	return 0;
}
static inline bool of_thermal_is_trip_valid(struct thermal_zone_device *tz,
					    int trip)
{
	return false;
}
static inline const struct thermal_trip *
of_thermal_get_trip_points(struct thermal_zone_device *tz)
{
	return NULL;
}
<<<<<<< HEAD
static inline
void of_thermal_handle_trip(struct thermal_zone_device *tz)
{ }
static inline
void of_thermal_handle_trip_temp(struct thermal_zone_device *tz,
					int trip_temp)
{ }
#endif

=======
#endif

int thermal_zone_device_is_enabled(struct thermal_zone_device *tz);

>>>>>>> upstream/android-13
#endif /* __THERMAL_CORE_H__ */
