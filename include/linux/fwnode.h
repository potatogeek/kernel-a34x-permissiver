<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * fwnode.h - Firmware device node object handle type definition.
 *
 * Copyright (C) 2015, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _LINUX_FWNODE_H_
#define _LINUX_FWNODE_H_

#include <linux/types.h>
<<<<<<< HEAD
=======
#include <linux/list.h>
#include <linux/err.h>
>>>>>>> upstream/android-13
#include <linux/android_kabi.h>

struct fwnode_operations;
struct device;

<<<<<<< HEAD
=======
/*
 * fwnode link flags
 *
 * LINKS_ADDED:	The fwnode has already be parsed to add fwnode links.
 * NOT_DEVICE:	The fwnode will never be populated as a struct device.
 * INITIALIZED: The hardware corresponding to fwnode has been initialized.
 * NEEDS_CHILD_BOUND_ON_ADD: For this fwnode/device to probe successfully, its
 *			     driver needs its child devices to be bound with
 *			     their respective drivers as soon as they are
 *			     added.
 */
#define FWNODE_FLAG_LINKS_ADDED			BIT(0)
#define FWNODE_FLAG_NOT_DEVICE			BIT(1)
#define FWNODE_FLAG_INITIALIZED			BIT(2)
#define FWNODE_FLAG_NEEDS_CHILD_BOUND_ON_ADD	BIT(3)

>>>>>>> upstream/android-13
struct fwnode_handle {
	struct fwnode_handle *secondary;
	const struct fwnode_operations *ops;
	struct device *dev;
<<<<<<< HEAD

	ANDROID_KABI_RESERVE(1);
	ANDROID_KABI_RESERVE(2);
	ANDROID_KABI_RESERVE(3);
	ANDROID_KABI_RESERVE(4);
=======
	struct list_head suppliers;
	struct list_head consumers;
	u8 flags;
	ANDROID_KABI_RESERVE(1);
};

struct fwnode_link {
	struct fwnode_handle *supplier;
	struct list_head s_hook;
	struct fwnode_handle *consumer;
	struct list_head c_hook;
	ANDROID_KABI_RESERVE(1);
	ANDROID_KABI_RESERVE(2);
	ANDROID_KABI_RESERVE(3);
>>>>>>> upstream/android-13
};

/**
 * struct fwnode_endpoint - Fwnode graph endpoint
 * @port: Port number
 * @id: Endpoint id
 * @local_fwnode: reference to the related fwnode
 */
struct fwnode_endpoint {
	unsigned int port;
	unsigned int id;
	const struct fwnode_handle *local_fwnode;
};

<<<<<<< HEAD
=======
/*
 * ports and endpoints defined as software_nodes should all follow a common
 * naming scheme; use these macros to ensure commonality.
 */
#define SWNODE_GRAPH_PORT_NAME_FMT		"port@%u"
#define SWNODE_GRAPH_ENDPOINT_NAME_FMT		"endpoint@%u"

>>>>>>> upstream/android-13
#define NR_FWNODE_REFERENCE_ARGS	8

/**
 * struct fwnode_reference_args - Fwnode reference with additional arguments
 * @fwnode:- A reference to the base fwnode
 * @nargs: Number of elements in @args array
 * @args: Integer arguments on the fwnode
 */
struct fwnode_reference_args {
	struct fwnode_handle *fwnode;
	unsigned int nargs;
	u64 args[NR_FWNODE_REFERENCE_ARGS];
};

/**
 * struct fwnode_operations - Operations for fwnode interface
 * @get: Get a reference to an fwnode.
 * @put: Put a reference to an fwnode.
<<<<<<< HEAD
 * @device_get_match_data: Return the device driver match data.
 * @property_present: Return true if a property is present.
 * @property_read_integer_array: Read an array of integer properties. Return
 *				 zero on success, a negative error code
 *				 otherwise.
 * @property_read_string_array: Read an array of string properties. Return zero
 *				on success, a negative error code otherwise.
=======
 * @device_is_available: Return true if the device is available.
 * @device_get_match_data: Return the device driver match data.
 * @property_present: Return true if a property is present.
 * @property_read_int_array: Read an array of integer properties. Return zero on
 *			     success, a negative error code otherwise.
 * @property_read_string_array: Read an array of string properties. Return zero
 *				on success, a negative error code otherwise.
 * @get_name: Return the name of an fwnode.
 * @get_name_prefix: Get a prefix for a node (for printing purposes).
>>>>>>> upstream/android-13
 * @get_parent: Return the parent of an fwnode.
 * @get_next_child_node: Return the next child node in an iteration.
 * @get_named_child_node: Return a child node with a given name.
 * @get_reference_args: Return a reference pointed to by a property, with args
 * @graph_get_next_endpoint: Return an endpoint node in an iteration.
 * @graph_get_remote_endpoint: Return the remote endpoint node of a local
 *			       endpoint node.
 * @graph_get_port_parent: Return the parent node of a port node.
 * @graph_parse_endpoint: Parse endpoint for port and endpoint id.
<<<<<<< HEAD
 * @add_links:	Called after the device corresponding to the fwnode is added
 *		using device_add(). The function is expected to create device
 *		links to all the suppliers of the device that are available at
 *		the time this function is called.  The function must NOT stop
 *		at the first failed device link if other unlinked supplier
 *		devices are present in the system.  This is necessary for the
 *		driver/bus sync_state() callbacks to work correctly.
 *
 *		For example, say Device-C depends on suppliers Device-S1 and
 *		Device-S2 and the dependency is listed in that order in the
 *		firmware.  Say, S1 gets populated from the firmware after
 *		late_initcall_sync().  Say S2 is populated and probed way
 *		before that in device_initcall(). When C is populated, if this
 *		add_links() function doesn't continue past a "failed linking to
 *		S1" and continue linking C to S2, then S2 will get a
 *		sync_state() callback before C is probed. This is because from
 *		the perspective of S2, C was never a consumer when its
 *		sync_state() evaluation is done. To avoid this, the add_links()
 *		function has to go through all available suppliers of the
 *		device (that corresponds to this fwnode) and link to them
 *		before returning.
 *
 *		If some suppliers are not yet available (indicated by an error
 *		return value), this function will be called again when other
 *		devices are added to allow creating device links to any newly
 *		available suppliers.
 *
 *		Return 0 if device links have been successfully created to all
 *		the known suppliers of this device or if the supplier
 *		information is not known.
 *
 *		Return -ENODEV if the suppliers needed for probing this device
 *		have not been registered yet (because device links can only be
 *		created to devices registered with the driver core).
 *
 *		Return -EAGAIN if some of the suppliers of this device have not
 *		been registered yet, but none of those suppliers are necessary
 *		for probing the device.
=======
 * @add_links:	Create fwnode links to all the suppliers of the fwnode. Return
 *		zero on success, a negative error code otherwise.
>>>>>>> upstream/android-13
 */
struct fwnode_operations {
	struct fwnode_handle *(*get)(struct fwnode_handle *fwnode);
	void (*put)(struct fwnode_handle *fwnode);
	bool (*device_is_available)(const struct fwnode_handle *fwnode);
	const void *(*device_get_match_data)(const struct fwnode_handle *fwnode,
					     const struct device *dev);
	bool (*property_present)(const struct fwnode_handle *fwnode,
				 const char *propname);
	int (*property_read_int_array)(const struct fwnode_handle *fwnode,
				       const char *propname,
				       unsigned int elem_size, void *val,
				       size_t nval);
	int
	(*property_read_string_array)(const struct fwnode_handle *fwnode_handle,
				      const char *propname, const char **val,
				      size_t nval);
<<<<<<< HEAD
=======
	const char *(*get_name)(const struct fwnode_handle *fwnode);
	const char *(*get_name_prefix)(const struct fwnode_handle *fwnode);
>>>>>>> upstream/android-13
	struct fwnode_handle *(*get_parent)(const struct fwnode_handle *fwnode);
	struct fwnode_handle *
	(*get_next_child_node)(const struct fwnode_handle *fwnode,
			       struct fwnode_handle *child);
	struct fwnode_handle *
	(*get_named_child_node)(const struct fwnode_handle *fwnode,
				const char *name);
	int (*get_reference_args)(const struct fwnode_handle *fwnode,
				  const char *prop, const char *nargs_prop,
				  unsigned int nargs, unsigned int index,
				  struct fwnode_reference_args *args);
	struct fwnode_handle *
	(*graph_get_next_endpoint)(const struct fwnode_handle *fwnode,
				   struct fwnode_handle *prev);
	struct fwnode_handle *
	(*graph_get_remote_endpoint)(const struct fwnode_handle *fwnode);
	struct fwnode_handle *
	(*graph_get_port_parent)(struct fwnode_handle *fwnode);
	int (*graph_parse_endpoint)(const struct fwnode_handle *fwnode,
				    struct fwnode_endpoint *endpoint);
<<<<<<< HEAD
	int (*add_links)(const struct fwnode_handle *fwnode,
			 struct device *dev);
=======
	int (*add_links)(struct fwnode_handle *fwnode);
>>>>>>> upstream/android-13
};

#define fwnode_has_op(fwnode, op)				\
	((fwnode) && (fwnode)->ops && (fwnode)->ops->op)
#define fwnode_call_int_op(fwnode, op, ...)				\
	(fwnode ? (fwnode_has_op(fwnode, op) ?				\
		   (fwnode)->ops->op(fwnode, ## __VA_ARGS__) : -ENXIO) : \
	 -EINVAL)
<<<<<<< HEAD
#define fwnode_call_bool_op(fwnode, op, ...)				\
	(fwnode ? (fwnode_has_op(fwnode, op) ?				\
		   (fwnode)->ops->op(fwnode, ## __VA_ARGS__) : false) : \
	 false)
=======

#define fwnode_call_bool_op(fwnode, op, ...)		\
	(fwnode_has_op(fwnode, op) ?			\
	 (fwnode)->ops->op(fwnode, ## __VA_ARGS__) : false)

>>>>>>> upstream/android-13
#define fwnode_call_ptr_op(fwnode, op, ...)		\
	(fwnode_has_op(fwnode, op) ?			\
	 (fwnode)->ops->op(fwnode, ## __VA_ARGS__) : NULL)
#define fwnode_call_void_op(fwnode, op, ...)				\
	do {								\
		if (fwnode_has_op(fwnode, op))				\
			(fwnode)->ops->op(fwnode, ## __VA_ARGS__);	\
	} while (false)
#define get_dev_from_fwnode(fwnode)	get_device((fwnode)->dev)

<<<<<<< HEAD
void fw_devlink_pause(void);
void fw_devlink_resume(void);
=======
static inline void fwnode_init(struct fwnode_handle *fwnode,
			       const struct fwnode_operations *ops)
{
	fwnode->ops = ops;
	INIT_LIST_HEAD(&fwnode->consumers);
	INIT_LIST_HEAD(&fwnode->suppliers);
}

static inline void fwnode_dev_initialized(struct fwnode_handle *fwnode,
					  bool initialized)
{
	if (IS_ERR_OR_NULL(fwnode))
		return;

	if (initialized)
		fwnode->flags |= FWNODE_FLAG_INITIALIZED;
	else
		fwnode->flags &= ~FWNODE_FLAG_INITIALIZED;
}

extern u32 fw_devlink_get_flags(void);
extern bool fw_devlink_is_strict(void);
int fwnode_link_add(struct fwnode_handle *con, struct fwnode_handle *sup);
void fwnode_links_purge(struct fwnode_handle *fwnode);
void fw_devlink_purge_absent_suppliers(struct fwnode_handle *fwnode);

>>>>>>> upstream/android-13
#endif
