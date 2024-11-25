<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * include/net/switchdev.h - Switch device API
 * Copyright (c) 2014-2015 Jiri Pirko <jiri@resnulli.us>
 * Copyright (c) 2014-2015 Scott Feldman <sfeldma@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#ifndef _LINUX_SWITCHDEV_H_
#define _LINUX_SWITCHDEV_H_

#include <linux/netdevice.h>
#include <linux/notifier.h>
#include <linux/list.h>
#include <net/ip_fib.h>

#define SWITCHDEV_F_NO_RECURSE		BIT(0)
#define SWITCHDEV_F_SKIP_EOPNOTSUPP	BIT(1)
#define SWITCHDEV_F_DEFER		BIT(2)

<<<<<<< HEAD
struct switchdev_trans_item {
	struct list_head list;
	void *data;
	void (*destructor)(const void *data);
};

struct switchdev_trans {
	struct list_head item_list;
	bool ph_prepare;
};

static inline bool switchdev_trans_ph_prepare(struct switchdev_trans *trans)
{
	return trans && trans->ph_prepare;
}

static inline bool switchdev_trans_ph_commit(struct switchdev_trans *trans)
{
	return trans && !trans->ph_prepare;
}

enum switchdev_attr_id {
	SWITCHDEV_ATTR_ID_UNDEFINED,
	SWITCHDEV_ATTR_ID_PORT_PARENT_ID,
	SWITCHDEV_ATTR_ID_PORT_STP_STATE,
	SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS,
	SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS_SUPPORT,
	SWITCHDEV_ATTR_ID_PORT_MROUTER,
	SWITCHDEV_ATTR_ID_BRIDGE_AGEING_TIME,
	SWITCHDEV_ATTR_ID_BRIDGE_VLAN_FILTERING,
	SWITCHDEV_ATTR_ID_BRIDGE_MC_DISABLED,
	SWITCHDEV_ATTR_ID_BRIDGE_MROUTER,
=======
enum switchdev_attr_id {
	SWITCHDEV_ATTR_ID_UNDEFINED,
	SWITCHDEV_ATTR_ID_PORT_STP_STATE,
	SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS,
	SWITCHDEV_ATTR_ID_PORT_PRE_BRIDGE_FLAGS,
	SWITCHDEV_ATTR_ID_PORT_MROUTER,
	SWITCHDEV_ATTR_ID_BRIDGE_AGEING_TIME,
	SWITCHDEV_ATTR_ID_BRIDGE_VLAN_FILTERING,
	SWITCHDEV_ATTR_ID_BRIDGE_VLAN_PROTOCOL,
	SWITCHDEV_ATTR_ID_BRIDGE_MC_DISABLED,
	SWITCHDEV_ATTR_ID_BRIDGE_MROUTER,
	SWITCHDEV_ATTR_ID_MRP_PORT_ROLE,
};

struct switchdev_brport_flags {
	unsigned long val;
	unsigned long mask;
>>>>>>> upstream/android-13
};

struct switchdev_attr {
	struct net_device *orig_dev;
	enum switchdev_attr_id id;
	u32 flags;
	void *complete_priv;
	void (*complete)(struct net_device *dev, int err, void *priv);
	union {
<<<<<<< HEAD
		struct netdev_phys_item_id ppid;	/* PORT_PARENT_ID */
		u8 stp_state;				/* PORT_STP_STATE */
		unsigned long brport_flags;		/* PORT_BRIDGE_FLAGS */
		unsigned long brport_flags_support;	/* PORT_BRIDGE_FLAGS_SUPPORT */
		bool mrouter;				/* PORT_MROUTER */
		clock_t ageing_time;			/* BRIDGE_AGEING_TIME */
		bool vlan_filtering;			/* BRIDGE_VLAN_FILTERING */
		bool mc_disabled;			/* MC_DISABLED */
=======
		u8 stp_state;				/* PORT_STP_STATE */
		struct switchdev_brport_flags brport_flags; /* PORT_BRIDGE_FLAGS */
		bool mrouter;				/* PORT_MROUTER */
		clock_t ageing_time;			/* BRIDGE_AGEING_TIME */
		bool vlan_filtering;			/* BRIDGE_VLAN_FILTERING */
		u16 vlan_protocol;			/* BRIDGE_VLAN_PROTOCOL */
		bool mc_disabled;			/* MC_DISABLED */
		u8 mrp_port_role;			/* MRP_PORT_ROLE */
>>>>>>> upstream/android-13
	} u;
};

enum switchdev_obj_id {
	SWITCHDEV_OBJ_ID_UNDEFINED,
	SWITCHDEV_OBJ_ID_PORT_VLAN,
	SWITCHDEV_OBJ_ID_PORT_MDB,
	SWITCHDEV_OBJ_ID_HOST_MDB,
<<<<<<< HEAD
};

struct switchdev_obj {
=======
	SWITCHDEV_OBJ_ID_MRP,
	SWITCHDEV_OBJ_ID_RING_TEST_MRP,
	SWITCHDEV_OBJ_ID_RING_ROLE_MRP,
	SWITCHDEV_OBJ_ID_RING_STATE_MRP,
	SWITCHDEV_OBJ_ID_IN_TEST_MRP,
	SWITCHDEV_OBJ_ID_IN_ROLE_MRP,
	SWITCHDEV_OBJ_ID_IN_STATE_MRP,
};

struct switchdev_obj {
	struct list_head list;
>>>>>>> upstream/android-13
	struct net_device *orig_dev;
	enum switchdev_obj_id id;
	u32 flags;
	void *complete_priv;
	void (*complete)(struct net_device *dev, int err, void *priv);
};

/* SWITCHDEV_OBJ_ID_PORT_VLAN */
struct switchdev_obj_port_vlan {
	struct switchdev_obj obj;
	u16 flags;
<<<<<<< HEAD
	u16 vid_begin;
	u16 vid_end;
};

#define SWITCHDEV_OBJ_PORT_VLAN(obj) \
	container_of(obj, struct switchdev_obj_port_vlan, obj)
=======
	u16 vid;
};

#define SWITCHDEV_OBJ_PORT_VLAN(OBJ) \
	container_of((OBJ), struct switchdev_obj_port_vlan, obj)
>>>>>>> upstream/android-13

/* SWITCHDEV_OBJ_ID_PORT_MDB */
struct switchdev_obj_port_mdb {
	struct switchdev_obj obj;
	unsigned char addr[ETH_ALEN];
	u16 vid;
};

<<<<<<< HEAD
#define SWITCHDEV_OBJ_PORT_MDB(obj) \
	container_of(obj, struct switchdev_obj_port_mdb, obj)

void switchdev_trans_item_enqueue(struct switchdev_trans *trans,
				  void *data, void (*destructor)(void const *),
				  struct switchdev_trans_item *tritem);
void *switchdev_trans_item_dequeue(struct switchdev_trans *trans);

typedef int switchdev_obj_dump_cb_t(struct switchdev_obj *obj);

/**
 * struct switchdev_ops - switchdev operations
 *
 * @switchdev_port_attr_get: Get a port attribute (see switchdev_attr).
 *
 * @switchdev_port_attr_set: Set a port attribute (see switchdev_attr).
 *
 * @switchdev_port_obj_add: Add an object to port (see switchdev_obj_*).
 *
 * @switchdev_port_obj_del: Delete an object from port (see switchdev_obj_*).
 */
struct switchdev_ops {
	int	(*switchdev_port_attr_get)(struct net_device *dev,
					   struct switchdev_attr *attr);
	int	(*switchdev_port_attr_set)(struct net_device *dev,
					   const struct switchdev_attr *attr,
					   struct switchdev_trans *trans);
	int	(*switchdev_port_obj_add)(struct net_device *dev,
					  const struct switchdev_obj *obj,
					  struct switchdev_trans *trans);
	int	(*switchdev_port_obj_del)(struct net_device *dev,
					  const struct switchdev_obj *obj);
=======
#define SWITCHDEV_OBJ_PORT_MDB(OBJ) \
	container_of((OBJ), struct switchdev_obj_port_mdb, obj)


/* SWITCHDEV_OBJ_ID_MRP */
struct switchdev_obj_mrp {
	struct switchdev_obj obj;
	struct net_device *p_port;
	struct net_device *s_port;
	u32 ring_id;
	u16 prio;
};

#define SWITCHDEV_OBJ_MRP(OBJ) \
	container_of((OBJ), struct switchdev_obj_mrp, obj)

/* SWITCHDEV_OBJ_ID_RING_TEST_MRP */
struct switchdev_obj_ring_test_mrp {
	struct switchdev_obj obj;
	/* The value is in us and a value of 0 represents to stop */
	u32 interval;
	u8 max_miss;
	u32 ring_id;
	u32 period;
	bool monitor;
};

#define SWITCHDEV_OBJ_RING_TEST_MRP(OBJ) \
	container_of((OBJ), struct switchdev_obj_ring_test_mrp, obj)

/* SWICHDEV_OBJ_ID_RING_ROLE_MRP */
struct switchdev_obj_ring_role_mrp {
	struct switchdev_obj obj;
	u8 ring_role;
	u32 ring_id;
	u8 sw_backup;
};

#define SWITCHDEV_OBJ_RING_ROLE_MRP(OBJ) \
	container_of((OBJ), struct switchdev_obj_ring_role_mrp, obj)

struct switchdev_obj_ring_state_mrp {
	struct switchdev_obj obj;
	u8 ring_state;
	u32 ring_id;
};

#define SWITCHDEV_OBJ_RING_STATE_MRP(OBJ) \
	container_of((OBJ), struct switchdev_obj_ring_state_mrp, obj)

/* SWITCHDEV_OBJ_ID_IN_TEST_MRP */
struct switchdev_obj_in_test_mrp {
	struct switchdev_obj obj;
	/* The value is in us and a value of 0 represents to stop */
	u32 interval;
	u32 in_id;
	u32 period;
	u8 max_miss;
};

#define SWITCHDEV_OBJ_IN_TEST_MRP(OBJ) \
	container_of((OBJ), struct switchdev_obj_in_test_mrp, obj)

/* SWICHDEV_OBJ_ID_IN_ROLE_MRP */
struct switchdev_obj_in_role_mrp {
	struct switchdev_obj obj;
	struct net_device *i_port;
	u32 ring_id;
	u16 in_id;
	u8 in_role;
	u8 sw_backup;
};

#define SWITCHDEV_OBJ_IN_ROLE_MRP(OBJ) \
	container_of((OBJ), struct switchdev_obj_in_role_mrp, obj)

struct switchdev_obj_in_state_mrp {
	struct switchdev_obj obj;
	u32 in_id;
	u8 in_state;
};

#define SWITCHDEV_OBJ_IN_STATE_MRP(OBJ) \
	container_of((OBJ), struct switchdev_obj_in_state_mrp, obj)

typedef int switchdev_obj_dump_cb_t(struct switchdev_obj *obj);

struct switchdev_brport {
	struct net_device *dev;
	const void *ctx;
	struct notifier_block *atomic_nb;
	struct notifier_block *blocking_nb;
	bool tx_fwd_offload;
>>>>>>> upstream/android-13
};

enum switchdev_notifier_type {
	SWITCHDEV_FDB_ADD_TO_BRIDGE = 1,
	SWITCHDEV_FDB_DEL_TO_BRIDGE,
	SWITCHDEV_FDB_ADD_TO_DEVICE,
	SWITCHDEV_FDB_DEL_TO_DEVICE,
	SWITCHDEV_FDB_OFFLOADED,
<<<<<<< HEAD
=======
	SWITCHDEV_FDB_FLUSH_TO_BRIDGE,

	SWITCHDEV_PORT_OBJ_ADD, /* Blocking. */
	SWITCHDEV_PORT_OBJ_DEL, /* Blocking. */
	SWITCHDEV_PORT_ATTR_SET, /* May be blocking . */

	SWITCHDEV_VXLAN_FDB_ADD_TO_BRIDGE,
	SWITCHDEV_VXLAN_FDB_DEL_TO_BRIDGE,
	SWITCHDEV_VXLAN_FDB_ADD_TO_DEVICE,
	SWITCHDEV_VXLAN_FDB_DEL_TO_DEVICE,
	SWITCHDEV_VXLAN_FDB_OFFLOADED,

	SWITCHDEV_BRPORT_OFFLOADED,
	SWITCHDEV_BRPORT_UNOFFLOADED,
>>>>>>> upstream/android-13
};

struct switchdev_notifier_info {
	struct net_device *dev;
<<<<<<< HEAD
=======
	struct netlink_ext_ack *extack;
	const void *ctx;
>>>>>>> upstream/android-13
};

struct switchdev_notifier_fdb_info {
	struct switchdev_notifier_info info; /* must be first */
	const unsigned char *addr;
	u16 vid;
<<<<<<< HEAD
	bool added_by_user;
=======
	u8 added_by_user:1,
	   is_local:1,
	   offloaded:1;
};

struct switchdev_notifier_port_obj_info {
	struct switchdev_notifier_info info; /* must be first */
	const struct switchdev_obj *obj;
	bool handled;
};

struct switchdev_notifier_port_attr_info {
	struct switchdev_notifier_info info; /* must be first */
	const struct switchdev_attr *attr;
	bool handled;
};

struct switchdev_notifier_brport_info {
	struct switchdev_notifier_info info; /* must be first */
	const struct switchdev_brport brport;
>>>>>>> upstream/android-13
};

static inline struct net_device *
switchdev_notifier_info_to_dev(const struct switchdev_notifier_info *info)
{
	return info->dev;
}

<<<<<<< HEAD
#ifdef CONFIG_NET_SWITCHDEV

void switchdev_deferred_process(void);
int switchdev_port_attr_get(struct net_device *dev,
			    struct switchdev_attr *attr);
int switchdev_port_attr_set(struct net_device *dev,
			    const struct switchdev_attr *attr);
int switchdev_port_obj_add(struct net_device *dev,
			   const struct switchdev_obj *obj);
int switchdev_port_obj_del(struct net_device *dev,
			   const struct switchdev_obj *obj);
int register_switchdev_notifier(struct notifier_block *nb);
int unregister_switchdev_notifier(struct notifier_block *nb);
int call_switchdev_notifiers(unsigned long val, struct net_device *dev,
			     struct switchdev_notifier_info *info);
=======
static inline struct netlink_ext_ack *
switchdev_notifier_info_to_extack(const struct switchdev_notifier_info *info)
{
	return info->extack;
}

static inline bool
switchdev_fdb_is_dynamically_learned(const struct switchdev_notifier_fdb_info *fdb_info)
{
	return !fdb_info->added_by_user && !fdb_info->is_local;
}

#ifdef CONFIG_NET_SWITCHDEV

int switchdev_bridge_port_offload(struct net_device *brport_dev,
				  struct net_device *dev, const void *ctx,
				  struct notifier_block *atomic_nb,
				  struct notifier_block *blocking_nb,
				  bool tx_fwd_offload,
				  struct netlink_ext_ack *extack);
void switchdev_bridge_port_unoffload(struct net_device *brport_dev,
				     const void *ctx,
				     struct notifier_block *atomic_nb,
				     struct notifier_block *blocking_nb);

void switchdev_deferred_process(void);
int switchdev_port_attr_set(struct net_device *dev,
			    const struct switchdev_attr *attr,
			    struct netlink_ext_ack *extack);
int switchdev_port_obj_add(struct net_device *dev,
			   const struct switchdev_obj *obj,
			   struct netlink_ext_ack *extack);
int switchdev_port_obj_del(struct net_device *dev,
			   const struct switchdev_obj *obj);

int register_switchdev_notifier(struct notifier_block *nb);
int unregister_switchdev_notifier(struct notifier_block *nb);
int call_switchdev_notifiers(unsigned long val, struct net_device *dev,
			     struct switchdev_notifier_info *info,
			     struct netlink_ext_ack *extack);

int register_switchdev_blocking_notifier(struct notifier_block *nb);
int unregister_switchdev_blocking_notifier(struct notifier_block *nb);
int call_switchdev_blocking_notifiers(unsigned long val, struct net_device *dev,
				      struct switchdev_notifier_info *info,
				      struct netlink_ext_ack *extack);

>>>>>>> upstream/android-13
void switchdev_port_fwd_mark_set(struct net_device *dev,
				 struct net_device *group_dev,
				 bool joining);

<<<<<<< HEAD
bool switchdev_port_same_parent_id(struct net_device *a,
				   struct net_device *b);

#define SWITCHDEV_SET_OPS(netdev, ops) ((netdev)->switchdev_ops = (ops))
#else

=======
int switchdev_handle_fdb_add_to_device(struct net_device *dev,
		const struct switchdev_notifier_fdb_info *fdb_info,
		bool (*check_cb)(const struct net_device *dev),
		bool (*foreign_dev_check_cb)(const struct net_device *dev,
					     const struct net_device *foreign_dev),
		int (*add_cb)(struct net_device *dev,
			      const struct net_device *orig_dev, const void *ctx,
			      const struct switchdev_notifier_fdb_info *fdb_info),
		int (*lag_add_cb)(struct net_device *dev,
				  const struct net_device *orig_dev, const void *ctx,
				  const struct switchdev_notifier_fdb_info *fdb_info));

int switchdev_handle_fdb_del_to_device(struct net_device *dev,
		const struct switchdev_notifier_fdb_info *fdb_info,
		bool (*check_cb)(const struct net_device *dev),
		bool (*foreign_dev_check_cb)(const struct net_device *dev,
					     const struct net_device *foreign_dev),
		int (*del_cb)(struct net_device *dev,
			      const struct net_device *orig_dev, const void *ctx,
			      const struct switchdev_notifier_fdb_info *fdb_info),
		int (*lag_del_cb)(struct net_device *dev,
				  const struct net_device *orig_dev, const void *ctx,
				  const struct switchdev_notifier_fdb_info *fdb_info));

int switchdev_handle_port_obj_add(struct net_device *dev,
			struct switchdev_notifier_port_obj_info *port_obj_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*add_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_obj *obj,
				      struct netlink_ext_ack *extack));
int switchdev_handle_port_obj_del(struct net_device *dev,
			struct switchdev_notifier_port_obj_info *port_obj_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*del_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_obj *obj));

int switchdev_handle_port_attr_set(struct net_device *dev,
			struct switchdev_notifier_port_attr_info *port_attr_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*set_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_attr *attr,
				      struct netlink_ext_ack *extack));
#else

static inline int
switchdev_bridge_port_offload(struct net_device *brport_dev,
			      struct net_device *dev, const void *ctx,
			      struct notifier_block *atomic_nb,
			      struct notifier_block *blocking_nb,
			      bool tx_fwd_offload,
			      struct netlink_ext_ack *extack)
{
	return -EOPNOTSUPP;
}

static inline void
switchdev_bridge_port_unoffload(struct net_device *brport_dev,
				const void *ctx,
				struct notifier_block *atomic_nb,
				struct notifier_block *blocking_nb)
{
}

>>>>>>> upstream/android-13
static inline void switchdev_deferred_process(void)
{
}

<<<<<<< HEAD
static inline int switchdev_port_attr_get(struct net_device *dev,
					  struct switchdev_attr *attr)
{
	return -EOPNOTSUPP;
}

static inline int switchdev_port_attr_set(struct net_device *dev,
					  const struct switchdev_attr *attr)
=======
static inline int switchdev_port_attr_set(struct net_device *dev,
					  const struct switchdev_attr *attr,
					  struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	return -EOPNOTSUPP;
}

static inline int switchdev_port_obj_add(struct net_device *dev,
<<<<<<< HEAD
					 const struct switchdev_obj *obj)
=======
					 const struct switchdev_obj *obj,
					 struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	return -EOPNOTSUPP;
}

static inline int switchdev_port_obj_del(struct net_device *dev,
					 const struct switchdev_obj *obj)
{
	return -EOPNOTSUPP;
}

static inline int register_switchdev_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int unregister_switchdev_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int call_switchdev_notifiers(unsigned long val,
					   struct net_device *dev,
<<<<<<< HEAD
					   struct switchdev_notifier_info *info)
=======
					   struct switchdev_notifier_info *info,
					   struct netlink_ext_ack *extack)
>>>>>>> upstream/android-13
{
	return NOTIFY_DONE;
}

<<<<<<< HEAD
static inline bool switchdev_port_same_parent_id(struct net_device *a,
						 struct net_device *b)
{
	return false;
}

#define SWITCHDEV_SET_OPS(netdev, ops) do {} while (0)

=======
static inline int
register_switchdev_blocking_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int
unregister_switchdev_blocking_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int
call_switchdev_blocking_notifiers(unsigned long val,
				  struct net_device *dev,
				  struct switchdev_notifier_info *info,
				  struct netlink_ext_ack *extack)
{
	return NOTIFY_DONE;
}

static inline int
switchdev_handle_fdb_add_to_device(struct net_device *dev,
		const struct switchdev_notifier_fdb_info *fdb_info,
		bool (*check_cb)(const struct net_device *dev),
		bool (*foreign_dev_check_cb)(const struct net_device *dev,
					     const struct net_device *foreign_dev),
		int (*add_cb)(struct net_device *dev,
			      const struct net_device *orig_dev, const void *ctx,
			      const struct switchdev_notifier_fdb_info *fdb_info),
		int (*lag_add_cb)(struct net_device *dev,
				  const struct net_device *orig_dev, const void *ctx,
				  const struct switchdev_notifier_fdb_info *fdb_info))
{
	return 0;
}

static inline int
switchdev_handle_fdb_del_to_device(struct net_device *dev,
		const struct switchdev_notifier_fdb_info *fdb_info,
		bool (*check_cb)(const struct net_device *dev),
		bool (*foreign_dev_check_cb)(const struct net_device *dev,
					     const struct net_device *foreign_dev),
		int (*del_cb)(struct net_device *dev,
			      const struct net_device *orig_dev, const void *ctx,
			      const struct switchdev_notifier_fdb_info *fdb_info),
		int (*lag_del_cb)(struct net_device *dev,
				  const struct net_device *orig_dev, const void *ctx,
				  const struct switchdev_notifier_fdb_info *fdb_info))
{
	return 0;
}

static inline int
switchdev_handle_port_obj_add(struct net_device *dev,
			struct switchdev_notifier_port_obj_info *port_obj_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*add_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_obj *obj,
				      struct netlink_ext_ack *extack))
{
	return 0;
}

static inline int
switchdev_handle_port_obj_del(struct net_device *dev,
			struct switchdev_notifier_port_obj_info *port_obj_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*del_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_obj *obj))
{
	return 0;
}

static inline int
switchdev_handle_port_attr_set(struct net_device *dev,
			struct switchdev_notifier_port_attr_info *port_attr_info,
			bool (*check_cb)(const struct net_device *dev),
			int (*set_cb)(struct net_device *dev, const void *ctx,
				      const struct switchdev_attr *attr,
				      struct netlink_ext_ack *extack))
{
	return 0;
}
>>>>>>> upstream/android-13
#endif

#endif /* _LINUX_SWITCHDEV_H_ */
