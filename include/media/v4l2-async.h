<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * V4L2 asynchronous subdevice registration API
 *
 * Copyright (C) 2012-2013, Guennadi Liakhovetski <g.liakhovetski@gmx.de>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef V4L2_ASYNC_H
#define V4L2_ASYNC_H

#include <linux/list.h>
#include <linux/mutex.h>

<<<<<<< HEAD
=======
struct dentry;
>>>>>>> upstream/android-13
struct device;
struct device_node;
struct v4l2_device;
struct v4l2_subdev;
struct v4l2_async_notifier;

<<<<<<< HEAD
/* A random max subdevice number, used to allocate an array on stack */
#define V4L2_MAX_SUBDEVS 128U

=======
>>>>>>> upstream/android-13
/**
 * enum v4l2_async_match_type - type of asynchronous subdevice logic to be used
 *	in order to identify a match
 *
<<<<<<< HEAD
 * @V4L2_ASYNC_MATCH_CUSTOM: Match will use the logic provided by &struct
 *	v4l2_async_subdev.match ops
 * @V4L2_ASYNC_MATCH_DEVNAME: Match will use the device name
 * @V4L2_ASYNC_MATCH_I2C: Match will check for I2C adapter ID and address
 * @V4L2_ASYNC_MATCH_FWNODE: Match will use firmware node
 *
 * This enum is used by the asyncrhronous sub-device logic to define the
 * algorithm that will be used to match an asynchronous device.
 */
enum v4l2_async_match_type {
	V4L2_ASYNC_MATCH_CUSTOM,
	V4L2_ASYNC_MATCH_DEVNAME,
=======
 * @V4L2_ASYNC_MATCH_I2C: Match will check for I2C adapter ID and address
 * @V4L2_ASYNC_MATCH_FWNODE: Match will use firmware node
 *
 * This enum is used by the asynchronous sub-device logic to define the
 * algorithm that will be used to match an asynchronous device.
 */
enum v4l2_async_match_type {
>>>>>>> upstream/android-13
	V4L2_ASYNC_MATCH_I2C,
	V4L2_ASYNC_MATCH_FWNODE,
};

/**
 * struct v4l2_async_subdev - sub-device descriptor, as known to a bridge
 *
 * @match_type:	type of match that will be used
 * @match:	union of per-bus type matching data sets
 * @match.fwnode:
 *		pointer to &struct fwnode_handle to be matched.
 *		Used if @match_type is %V4L2_ASYNC_MATCH_FWNODE.
<<<<<<< HEAD
 * @match.device_name:
 *		string containing the device name to be matched.
 *		Used if @match_type is %V4L2_ASYNC_MATCH_DEVNAME.
=======
>>>>>>> upstream/android-13
 * @match.i2c:	embedded struct with I2C parameters to be matched.
 *		Both @match.i2c.adapter_id and @match.i2c.address
 *		should be matched.
 *		Used if @match_type is %V4L2_ASYNC_MATCH_I2C.
 * @match.i2c.adapter_id:
 *		I2C adapter ID to be matched.
 *		Used if @match_type is %V4L2_ASYNC_MATCH_I2C.
 * @match.i2c.address:
 *		I2C address to be matched.
 *		Used if @match_type is %V4L2_ASYNC_MATCH_I2C.
<<<<<<< HEAD
 * @match.custom:
 *		Driver-specific match criteria.
 *		Used if @match_type is %V4L2_ASYNC_MATCH_CUSTOM.
 * @match.custom.match:
 *		Driver-specific match function to be used if
 *		%V4L2_ASYNC_MATCH_CUSTOM.
 * @match.custom.priv:
 *		Driver-specific private struct with match parameters
 *		to be used if %V4L2_ASYNC_MATCH_CUSTOM.
=======
 * @asd_list:	used to add struct v4l2_async_subdev objects to the
 *		master notifier @asd_list
>>>>>>> upstream/android-13
 * @list:	used to link struct v4l2_async_subdev objects, waiting to be
 *		probed, to a notifier->waiting list
 *
 * When this struct is used as a member in a driver specific struct,
 * the driver specific struct shall contain the &struct
 * v4l2_async_subdev as its first member.
 */
struct v4l2_async_subdev {
	enum v4l2_async_match_type match_type;
	union {
		struct fwnode_handle *fwnode;
<<<<<<< HEAD
		const char *device_name;
=======
>>>>>>> upstream/android-13
		struct {
			int adapter_id;
			unsigned short address;
		} i2c;
<<<<<<< HEAD
		struct {
			bool (*match)(struct device *,
				      struct v4l2_async_subdev *);
			void *priv;
		} custom;
=======
>>>>>>> upstream/android-13
	} match;

	/* v4l2-async core private: not to be used by drivers */
	struct list_head list;
<<<<<<< HEAD
=======
	struct list_head asd_list;
>>>>>>> upstream/android-13
};

/**
 * struct v4l2_async_notifier_operations - Asynchronous V4L2 notifier operations
 * @bound:	a subdevice driver has successfully probed one of the subdevices
 * @complete:	All subdevices have been probed successfully. The complete
 *		callback is only executed for the root notifier.
 * @unbind:	a subdevice is leaving
 */
struct v4l2_async_notifier_operations {
	int (*bound)(struct v4l2_async_notifier *notifier,
		     struct v4l2_subdev *subdev,
		     struct v4l2_async_subdev *asd);
	int (*complete)(struct v4l2_async_notifier *notifier);
	void (*unbind)(struct v4l2_async_notifier *notifier,
		       struct v4l2_subdev *subdev,
		       struct v4l2_async_subdev *asd);
};

/**
 * struct v4l2_async_notifier - v4l2_device notifier data
 *
 * @ops:	notifier operations
<<<<<<< HEAD
 * @num_subdevs: number of subdevices used in the subdevs array
 * @max_subdevs: number of subdevices allocated in the subdevs array
 * @subdevs:	array of pointers to subdevice descriptors
 * @v4l2_dev:	v4l2_device of the root notifier, NULL otherwise
 * @sd:		sub-device that registered the notifier, NULL otherwise
 * @parent:	parent notifier
=======
 * @v4l2_dev:	v4l2_device of the root notifier, NULL otherwise
 * @sd:		sub-device that registered the notifier, NULL otherwise
 * @parent:	parent notifier
 * @asd_list:	master list of struct v4l2_async_subdev
>>>>>>> upstream/android-13
 * @waiting:	list of struct v4l2_async_subdev, waiting for their drivers
 * @done:	list of struct v4l2_subdev, already probed
 * @list:	member in a global list of notifiers
 */
struct v4l2_async_notifier {
	const struct v4l2_async_notifier_operations *ops;
<<<<<<< HEAD
	unsigned int num_subdevs;
	unsigned int max_subdevs;
	struct v4l2_async_subdev **subdevs;
	struct v4l2_device *v4l2_dev;
	struct v4l2_subdev *sd;
	struct v4l2_async_notifier *parent;
=======
	struct v4l2_device *v4l2_dev;
	struct v4l2_subdev *sd;
	struct v4l2_async_notifier *parent;
	struct list_head asd_list;
>>>>>>> upstream/android-13
	struct list_head waiting;
	struct list_head done;
	struct list_head list;
};

/**
<<<<<<< HEAD
=======
 * v4l2_async_debug_init - Initialize debugging tools.
 *
 * @debugfs_dir: pointer to the parent debugfs &struct dentry
 */
void v4l2_async_debug_init(struct dentry *debugfs_dir);

/**
 * v4l2_async_notifier_init - Initialize a notifier.
 *
 * @notifier: pointer to &struct v4l2_async_notifier
 *
 * This function initializes the notifier @asd_list. It must be called
 * before adding a subdevice to a notifier, using one of:
 * v4l2_async_notifier_add_fwnode_remote_subdev(),
 * v4l2_async_notifier_add_fwnode_subdev(),
 * v4l2_async_notifier_add_i2c_subdev(),
 * __v4l2_async_notifier_add_subdev() or
 * v4l2_async_notifier_parse_fwnode_endpoints().
 */
void v4l2_async_notifier_init(struct v4l2_async_notifier *notifier);

/**
 * __v4l2_async_notifier_add_subdev - Add an async subdev to the
 *				notifier's master asd list.
 *
 * @notifier: pointer to &struct v4l2_async_notifier
 * @asd: pointer to &struct v4l2_async_subdev
 *
 * \warning: Drivers should avoid using this function and instead use one of:
 * v4l2_async_notifier_add_fwnode_subdev(),
 * v4l2_async_notifier_add_fwnode_remote_subdev() or
 * v4l2_async_notifier_add_i2c_subdev().
 *
 * Call this function before registering a notifier to link the provided @asd to
 * the notifiers master @asd_list. The @asd must be allocated with k*alloc() as
 * it will be freed by the framework when the notifier is destroyed.
 */
int __v4l2_async_notifier_add_subdev(struct v4l2_async_notifier *notifier,
				   struct v4l2_async_subdev *asd);

struct v4l2_async_subdev *
__v4l2_async_notifier_add_fwnode_subdev(struct v4l2_async_notifier *notifier,
					struct fwnode_handle *fwnode,
					unsigned int asd_struct_size);
/**
 * v4l2_async_notifier_add_fwnode_subdev - Allocate and add a fwnode async
 *				subdev to the notifier's master asd_list.
 *
 * @notifier: pointer to &struct v4l2_async_notifier
 * @fwnode: fwnode handle of the sub-device to be matched, pointer to
 *	    &struct fwnode_handle
 * @type: Type of the driver's async sub-device struct. The &struct
 *	  v4l2_async_subdev shall be the first member of the driver's async
 *	  sub-device struct, i.e. both begin at the same memory address.
 *
 * Allocate a fwnode-matched asd of size asd_struct_size, and add it to the
 * notifiers @asd_list. The function also gets a reference of the fwnode which
 * is released later at notifier cleanup time.
 */
#define v4l2_async_notifier_add_fwnode_subdev(notifier, fwnode, type)	\
	((type *)__v4l2_async_notifier_add_fwnode_subdev(notifier, fwnode, \
							   sizeof(type)))

struct v4l2_async_subdev *
__v4l2_async_notifier_add_fwnode_remote_subdev(struct v4l2_async_notifier *notif,
					       struct fwnode_handle *endpoint,
					       unsigned int asd_struct_size);
/**
 * v4l2_async_notifier_add_fwnode_remote_subdev - Allocate and add a fwnode
 *						  remote async subdev to the
 *						  notifier's master asd_list.
 *
 * @notifier: pointer to &struct v4l2_async_notifier
 * @ep: local endpoint pointing to the remote sub-device to be matched,
 *	pointer to &struct fwnode_handle
 * @type: Type of the driver's async sub-device struct. The &struct
 *	  v4l2_async_subdev shall be the first member of the driver's async
 *	  sub-device struct, i.e. both begin at the same memory address.
 *
 * Gets the remote endpoint of a given local endpoint, set it up for fwnode
 * matching and adds the async sub-device to the notifier's @asd_list. The
 * function also gets a reference of the fwnode which is released later at
 * notifier cleanup time.
 *
 * This is just like v4l2_async_notifier_add_fwnode_subdev(), but with the
 * exception that the fwnode refers to a local endpoint, not the remote one.
 */
#define v4l2_async_notifier_add_fwnode_remote_subdev(notifier, ep, type) \
	((type *)							\
	 __v4l2_async_notifier_add_fwnode_remote_subdev(notifier, ep,	\
							sizeof(type)))

struct v4l2_async_subdev *
__v4l2_async_notifier_add_i2c_subdev(struct v4l2_async_notifier *notifier,
				     int adapter_id, unsigned short address,
				     unsigned int asd_struct_size);
/**
 * v4l2_async_notifier_add_i2c_subdev - Allocate and add an i2c async
 *				subdev to the notifier's master asd_list.
 *
 * @notifier: pointer to &struct v4l2_async_notifier
 * @adapter: I2C adapter ID to be matched
 * @address: I2C address of sub-device to be matched
 * @type: Type of the driver's async sub-device struct. The &struct
 *	  v4l2_async_subdev shall be the first member of the driver's async
 *	  sub-device struct, i.e. both begin at the same memory address.
 *
 * Same as v4l2_async_notifier_add_fwnode_subdev() but for I2C matched
 * sub-devices.
 */
#define v4l2_async_notifier_add_i2c_subdev(notifier, adapter, address, type) \
	((type *)__v4l2_async_notifier_add_i2c_subdev(notifier, adapter, \
						      address, sizeof(type)))

/**
>>>>>>> upstream/android-13
 * v4l2_async_notifier_register - registers a subdevice asynchronous notifier
 *
 * @v4l2_dev: pointer to &struct v4l2_device
 * @notifier: pointer to &struct v4l2_async_notifier
 */
int v4l2_async_notifier_register(struct v4l2_device *v4l2_dev,
				 struct v4l2_async_notifier *notifier);

/**
 * v4l2_async_subdev_notifier_register - registers a subdevice asynchronous
 *					 notifier for a sub-device
 *
 * @sd: pointer to &struct v4l2_subdev
 * @notifier: pointer to &struct v4l2_async_notifier
 */
int v4l2_async_subdev_notifier_register(struct v4l2_subdev *sd,
					struct v4l2_async_notifier *notifier);

/**
<<<<<<< HEAD
 * v4l2_async_notifier_unregister - unregisters a subdevice asynchronous notifier
=======
 * v4l2_async_notifier_unregister - unregisters a subdevice
 *	asynchronous notifier
>>>>>>> upstream/android-13
 *
 * @notifier: pointer to &struct v4l2_async_notifier
 */
void v4l2_async_notifier_unregister(struct v4l2_async_notifier *notifier);

/**
 * v4l2_async_notifier_cleanup - clean up notifier resources
 * @notifier: the notifier the resources of which are to be cleaned up
 *
 * Release memory resources related to a notifier, including the async
 * sub-devices allocated for the purposes of the notifier but not the notifier
 * itself. The user is responsible for calling this function to clean up the
<<<<<<< HEAD
 * notifier after calling @v4l2_async_notifier_parse_fwnode_endpoints or
 * @v4l2_fwnode_reference_parse_sensor_common.
 *
 * There is no harm from calling v4l2_async_notifier_cleanup in other
=======
 * notifier after calling
 * v4l2_async_notifier_add_fwnode_remote_subdev(),
 * v4l2_async_notifier_add_fwnode_subdev(),
 * v4l2_async_notifier_add_i2c_subdev(),
 * __v4l2_async_notifier_add_subdev() or
 * v4l2_async_notifier_parse_fwnode_endpoints().
 *
 * There is no harm from calling v4l2_async_notifier_cleanup() in other
>>>>>>> upstream/android-13
 * cases as long as its memory has been zeroed after it has been
 * allocated.
 */
void v4l2_async_notifier_cleanup(struct v4l2_async_notifier *notifier);

/**
 * v4l2_async_register_subdev - registers a sub-device to the asynchronous
 *	subdevice framework
 *
 * @sd: pointer to &struct v4l2_subdev
 */
int v4l2_async_register_subdev(struct v4l2_subdev *sd);

/**
<<<<<<< HEAD
 * v4l2_async_register_subdev_sensor_common - registers a sensor sub-device to
 *					      the asynchronous sub-device
 *					      framework and parse set up common
 *					      sensor related devices
=======
 * v4l2_async_register_subdev_sensor - registers a sensor sub-device to the
 *				       asynchronous sub-device framework and
 *				       parse set up common sensor related
 *				       devices
>>>>>>> upstream/android-13
 *
 * @sd: pointer to struct &v4l2_subdev
 *
 * This function is just like v4l2_async_register_subdev() with the exception
 * that calling it will also parse firmware interfaces for remote references
<<<<<<< HEAD
 * using v4l2_async_notifier_parse_fwnode_sensor_common() and registers the
=======
 * using v4l2_async_notifier_parse_fwnode_sensor() and registers the
>>>>>>> upstream/android-13
 * async sub-devices. The sub-device is similarly unregistered by calling
 * v4l2_async_unregister_subdev().
 *
 * While registered, the subdev module is marked as in-use.
 *
 * An error is returned if the module is no longer loaded on any attempts
 * to register it.
 */
<<<<<<< HEAD
int __must_check v4l2_async_register_subdev_sensor_common(
	struct v4l2_subdev *sd);
=======
int __must_check
v4l2_async_register_subdev_sensor(struct v4l2_subdev *sd);
>>>>>>> upstream/android-13

/**
 * v4l2_async_unregister_subdev - unregisters a sub-device to the asynchronous
 *	subdevice framework
 *
 * @sd: pointer to &struct v4l2_subdev
 */
void v4l2_async_unregister_subdev(struct v4l2_subdev *sd);
#endif
