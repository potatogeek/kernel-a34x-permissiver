/* SPDX-License-Identifier: GPL-2.0 */
/*
 * can in net namespaces
 */

#ifndef __NETNS_CAN_H__
#define __NETNS_CAN_H__

#include <linux/spinlock.h>
<<<<<<< HEAD

struct can_dev_rcv_lists;
struct s_stats;
struct s_pstats;
=======
#include <linux/android_kabi.h>

struct can_dev_rcv_lists;
struct can_pkg_stats;
struct can_rcv_lists_stats;
>>>>>>> upstream/android-13

struct netns_can {
#if IS_ENABLED(CONFIG_PROC_FS)
	struct proc_dir_entry *proc_dir;
<<<<<<< HEAD
	struct proc_dir_entry *pde_version;
=======
>>>>>>> upstream/android-13
	struct proc_dir_entry *pde_stats;
	struct proc_dir_entry *pde_reset_stats;
	struct proc_dir_entry *pde_rcvlist_all;
	struct proc_dir_entry *pde_rcvlist_fil;
	struct proc_dir_entry *pde_rcvlist_inv;
	struct proc_dir_entry *pde_rcvlist_sff;
	struct proc_dir_entry *pde_rcvlist_eff;
	struct proc_dir_entry *pde_rcvlist_err;
	struct proc_dir_entry *bcmproc_dir;
#endif

	/* receive filters subscribed for 'all' CAN devices */
<<<<<<< HEAD
	struct can_dev_rcv_lists *can_rx_alldev_list;
	spinlock_t can_rcvlists_lock;
	struct timer_list can_stattimer;/* timer for statistics update */
	struct s_stats *can_stats;	/* packet statistics */
	struct s_pstats *can_pstats;	/* receive list statistics */

	/* CAN GW per-net gateway jobs */
	struct hlist_head cgw_list;
=======
	struct can_dev_rcv_lists *rx_alldev_list;
	spinlock_t rcvlists_lock;
	struct timer_list stattimer; /* timer for statistics update */
	struct can_pkg_stats *pkg_stats;
	struct can_rcv_lists_stats *rcv_lists_stats;

	/* CAN GW per-net gateway jobs */
	struct hlist_head cgw_list;

	ANDROID_KABI_RESERVE(1);
>>>>>>> upstream/android-13
};

#endif /* __NETNS_CAN_H__ */
