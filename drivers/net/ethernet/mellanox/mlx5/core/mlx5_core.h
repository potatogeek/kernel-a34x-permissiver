/*
 * Copyright (c) 2013-2015, Mellanox Technologies, Ltd.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __MLX5_CORE_H__
#define __MLX5_CORE_H__

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/if_link.h>
#include <linux/firmware.h>
#include <linux/mlx5/cq.h>
<<<<<<< HEAD

#define DRIVER_NAME "mlx5_core"
#define DRIVER_VERSION "5.0-0"
=======
#include <linux/mlx5/fs.h>
#include <linux/mlx5/driver.h>
>>>>>>> upstream/android-13

extern uint mlx5_core_debug_mask;

#define mlx5_core_dbg(__dev, format, ...)				\
<<<<<<< HEAD
	dev_dbg(&(__dev)->pdev->dev, "%s:%d:(pid %d): " format,		\
		 __func__, __LINE__, current->pid,			\
		 ##__VA_ARGS__)

#define mlx5_core_dbg_once(__dev, format, ...)				\
	dev_dbg_once(&(__dev)->pdev->dev, "%s:%d:(pid %d): " format,	\
		     __func__, __LINE__, current->pid,			\
		     ##__VA_ARGS__)

#define mlx5_core_dbg_mask(__dev, mask, format, ...)			\
do {									\
	if ((mask) & mlx5_core_debug_mask)				\
		mlx5_core_dbg(__dev, format, ##__VA_ARGS__);		\
} while (0)

#define mlx5_core_err(__dev, format, ...)				\
	dev_err(&(__dev)->pdev->dev, "%s:%d:(pid %d): " format,	\
		__func__, __LINE__, current->pid,	\
	       ##__VA_ARGS__)

#define mlx5_core_err_rl(__dev, format, ...)				\
	dev_err_ratelimited(&(__dev)->pdev->dev,			\
			   "%s:%d:(pid %d): " format,			\
			   __func__, __LINE__, current->pid,		\
			   ##__VA_ARGS__)

#define mlx5_core_warn(__dev, format, ...)				\
	dev_warn(&(__dev)->pdev->dev, "%s:%d:(pid %d): " format,	\
		 __func__, __LINE__, current->pid,			\
		##__VA_ARGS__)

#define mlx5_core_info(__dev, format, ...)				\
	dev_info(&(__dev)->pdev->dev, format, ##__VA_ARGS__)
=======
	dev_dbg((__dev)->device, "%s:%d:(pid %d): " format,		\
		 __func__, __LINE__, current->pid,			\
		 ##__VA_ARGS__)

#define mlx5_core_dbg_once(__dev, format, ...)		\
	dev_dbg_once((__dev)->device,		\
		     "%s:%d:(pid %d): " format,		\
		     __func__, __LINE__, current->pid,	\
		     ##__VA_ARGS__)

#define mlx5_core_dbg_mask(__dev, mask, format, ...)		\
do {								\
	if ((mask) & mlx5_core_debug_mask)			\
		mlx5_core_dbg(__dev, format, ##__VA_ARGS__);	\
} while (0)

#define mlx5_core_err(__dev, format, ...)			\
	dev_err((__dev)->device, "%s:%d:(pid %d): " format,	\
		__func__, __LINE__, current->pid,		\
	       ##__VA_ARGS__)

#define mlx5_core_err_rl(__dev, format, ...)			\
	dev_err_ratelimited((__dev)->device,			\
			    "%s:%d:(pid %d): " format,		\
			    __func__, __LINE__, current->pid,	\
			    ##__VA_ARGS__)

#define mlx5_core_warn(__dev, format, ...)			\
	dev_warn((__dev)->device, "%s:%d:(pid %d): " format,	\
		 __func__, __LINE__, current->pid,		\
		 ##__VA_ARGS__)

#define mlx5_core_warn_once(__dev, format, ...)				\
	dev_warn_once((__dev)->device, "%s:%d:(pid %d): " format,	\
		      __func__, __LINE__, current->pid,			\
		      ##__VA_ARGS__)

#define mlx5_core_warn_rl(__dev, format, ...)			\
	dev_warn_ratelimited((__dev)->device,			\
			     "%s:%d:(pid %d): " format,		\
			     __func__, __LINE__, current->pid,	\
			     ##__VA_ARGS__)

#define mlx5_core_info(__dev, format, ...)		\
	dev_info((__dev)->device, format, ##__VA_ARGS__)

#define mlx5_core_info_rl(__dev, format, ...)			\
	dev_info_ratelimited((__dev)->device,			\
			     "%s:%d:(pid %d): " format,		\
			     __func__, __LINE__, current->pid,	\
			     ##__VA_ARGS__)

static inline struct device *mlx5_core_dma_dev(struct mlx5_core_dev *dev)
{
	return &dev->pdev->dev;
}
>>>>>>> upstream/android-13

enum {
	MLX5_CMD_DATA, /* print command payload only */
	MLX5_CMD_TIME, /* print command execution time */
};

enum {
	MLX5_DRIVER_STATUS_ABORTED = 0xfe,
	MLX5_DRIVER_SYND = 0xbadd00de,
};

<<<<<<< HEAD
int mlx5_query_hca_caps(struct mlx5_core_dev *dev);
int mlx5_query_board_id(struct mlx5_core_dev *dev);
int mlx5_cmd_init_hca(struct mlx5_core_dev *dev, uint32_t *sw_owner_id);
int mlx5_cmd_teardown_hca(struct mlx5_core_dev *dev);
int mlx5_cmd_force_teardown_hca(struct mlx5_core_dev *dev);
void mlx5_core_event(struct mlx5_core_dev *dev, enum mlx5_dev_event event,
		     unsigned long param);
void mlx5_core_page_fault(struct mlx5_core_dev *dev,
			  struct mlx5_pagefault *pfault);
void mlx5_port_module_event(struct mlx5_core_dev *dev, struct mlx5_eqe *eqe);
void mlx5_enter_error_state(struct mlx5_core_dev *dev, bool force);
void mlx5_disable_device(struct mlx5_core_dev *dev);
void mlx5_recover_device(struct mlx5_core_dev *dev);
=======
enum mlx5_semaphore_space_address {
	MLX5_SEMAPHORE_SPACE_DOMAIN     = 0xA,
	MLX5_SEMAPHORE_SW_RESET         = 0x20,
};

#define MLX5_DEFAULT_PROF       2

int mlx5_query_hca_caps(struct mlx5_core_dev *dev);
int mlx5_query_board_id(struct mlx5_core_dev *dev);
int mlx5_cmd_init(struct mlx5_core_dev *dev);
void mlx5_cmd_cleanup(struct mlx5_core_dev *dev);
void mlx5_cmd_set_state(struct mlx5_core_dev *dev,
			enum mlx5_cmdif_state cmdif_state);
int mlx5_cmd_init_hca(struct mlx5_core_dev *dev, uint32_t *sw_owner_id);
int mlx5_cmd_teardown_hca(struct mlx5_core_dev *dev);
int mlx5_cmd_force_teardown_hca(struct mlx5_core_dev *dev);
int mlx5_cmd_fast_teardown_hca(struct mlx5_core_dev *dev);
void mlx5_enter_error_state(struct mlx5_core_dev *dev, bool force);
void mlx5_error_sw_reset(struct mlx5_core_dev *dev);
u32 mlx5_health_check_fatal_sensors(struct mlx5_core_dev *dev);
int mlx5_health_wait_pci_up(struct mlx5_core_dev *dev);
void mlx5_disable_device(struct mlx5_core_dev *dev);
int mlx5_recover_device(struct mlx5_core_dev *dev);
>>>>>>> upstream/android-13
int mlx5_sriov_init(struct mlx5_core_dev *dev);
void mlx5_sriov_cleanup(struct mlx5_core_dev *dev);
int mlx5_sriov_attach(struct mlx5_core_dev *dev);
void mlx5_sriov_detach(struct mlx5_core_dev *dev);
int mlx5_core_sriov_configure(struct pci_dev *dev, int num_vfs);
<<<<<<< HEAD
bool mlx5_sriov_is_enabled(struct mlx5_core_dev *dev);
=======
int mlx5_core_sriov_set_msix_vec_count(struct pci_dev *vf, int msix_vec_count);
>>>>>>> upstream/android-13
int mlx5_core_enable_hca(struct mlx5_core_dev *dev, u16 func_id);
int mlx5_core_disable_hca(struct mlx5_core_dev *dev, u16 func_id);
int mlx5_create_scheduling_element_cmd(struct mlx5_core_dev *dev, u8 hierarchy,
				       void *context, u32 *element_id);
int mlx5_modify_scheduling_element_cmd(struct mlx5_core_dev *dev, u8 hierarchy,
				       void *context, u32 element_id,
				       u32 modify_bitmask);
int mlx5_destroy_scheduling_element_cmd(struct mlx5_core_dev *dev, u8 hierarchy,
					u32 element_id);
<<<<<<< HEAD
int mlx5_wait_for_vf_pages(struct mlx5_core_dev *dev);
u64 mlx5_read_internal_timer(struct mlx5_core_dev *dev);

int mlx5_eq_init(struct mlx5_core_dev *dev);
void mlx5_eq_cleanup(struct mlx5_core_dev *dev);
int mlx5_create_map_eq(struct mlx5_core_dev *dev, struct mlx5_eq *eq, u8 vecidx,
		       int nent, u64 mask, const char *name,
		       enum mlx5_eq_type type);
int mlx5_destroy_unmap_eq(struct mlx5_core_dev *dev, struct mlx5_eq *eq);
int mlx5_eq_add_cq(struct mlx5_eq *eq, struct mlx5_core_cq *cq);
int mlx5_eq_del_cq(struct mlx5_eq *eq, struct mlx5_core_cq *cq);
int mlx5_core_eq_query(struct mlx5_core_dev *dev, struct mlx5_eq *eq,
		       u32 *out, int outlen);
int mlx5_start_eqs(struct mlx5_core_dev *dev);
void mlx5_stop_eqs(struct mlx5_core_dev *dev);
/* This function should only be called after mlx5_cmd_force_teardown_hca */
void mlx5_core_eq_free_irqs(struct mlx5_core_dev *dev);
struct mlx5_eq *mlx5_eqn2eq(struct mlx5_core_dev *dev, int eqn);
u32 mlx5_eq_poll_irq_disabled(struct mlx5_eq *eq);
void mlx5_cq_tasklet_cb(unsigned long data);
void mlx5_cmd_comp_handler(struct mlx5_core_dev *dev, u64 vec, bool forced);
int mlx5_debug_eq_add(struct mlx5_core_dev *dev, struct mlx5_eq *eq);
void mlx5_debug_eq_remove(struct mlx5_core_dev *dev, struct mlx5_eq *eq);
int mlx5_eq_debugfs_init(struct mlx5_core_dev *dev);
void mlx5_eq_debugfs_cleanup(struct mlx5_core_dev *dev);
int mlx5_cq_debugfs_init(struct mlx5_core_dev *dev);
=======
int mlx5_wait_for_pages(struct mlx5_core_dev *dev, int *pages);

void mlx5_cmd_trigger_completions(struct mlx5_core_dev *dev);
void mlx5_cmd_flush(struct mlx5_core_dev *dev);
void mlx5_cq_debugfs_init(struct mlx5_core_dev *dev);
>>>>>>> upstream/android-13
void mlx5_cq_debugfs_cleanup(struct mlx5_core_dev *dev);

int mlx5_query_pcam_reg(struct mlx5_core_dev *dev, u32 *pcam, u8 feature_group,
			u8 access_reg_group);
int mlx5_query_mcam_reg(struct mlx5_core_dev *dev, u32 *mcap, u8 feature_group,
			u8 access_reg_group);
int mlx5_query_qcam_reg(struct mlx5_core_dev *mdev, u32 *qcam,
			u8 feature_group, u8 access_reg_group);

<<<<<<< HEAD
void mlx5_lag_add(struct mlx5_core_dev *dev, struct net_device *netdev);
void mlx5_lag_remove(struct mlx5_core_dev *dev);

void mlx5_add_device(struct mlx5_interface *intf, struct mlx5_priv *priv);
void mlx5_remove_device(struct mlx5_interface *intf, struct mlx5_priv *priv);
void mlx5_attach_device(struct mlx5_core_dev *dev);
void mlx5_detach_device(struct mlx5_core_dev *dev);
bool mlx5_device_registered(struct mlx5_core_dev *dev);
int mlx5_register_device(struct mlx5_core_dev *dev);
void mlx5_unregister_device(struct mlx5_core_dev *dev);
void mlx5_add_dev_by_protocol(struct mlx5_core_dev *dev, int protocol);
void mlx5_remove_dev_by_protocol(struct mlx5_core_dev *dev, int protocol);
=======
void mlx5_lag_add_netdev(struct mlx5_core_dev *dev, struct net_device *netdev);
void mlx5_lag_remove_netdev(struct mlx5_core_dev *dev, struct net_device *netdev);
void mlx5_lag_add_mdev(struct mlx5_core_dev *dev);
void mlx5_lag_remove_mdev(struct mlx5_core_dev *dev);
void mlx5_lag_disable_change(struct mlx5_core_dev *dev);
void mlx5_lag_enable_change(struct mlx5_core_dev *dev);

int mlx5_events_init(struct mlx5_core_dev *dev);
void mlx5_events_cleanup(struct mlx5_core_dev *dev);
void mlx5_events_start(struct mlx5_core_dev *dev);
void mlx5_events_stop(struct mlx5_core_dev *dev);

int mlx5_adev_idx_alloc(void);
void mlx5_adev_idx_free(int idx);
void mlx5_adev_cleanup(struct mlx5_core_dev *dev);
int mlx5_adev_init(struct mlx5_core_dev *dev);

int mlx5_attach_device(struct mlx5_core_dev *dev);
void mlx5_detach_device(struct mlx5_core_dev *dev);
int mlx5_register_device(struct mlx5_core_dev *dev);
void mlx5_unregister_device(struct mlx5_core_dev *dev);
>>>>>>> upstream/android-13
struct mlx5_core_dev *mlx5_get_next_phys_dev(struct mlx5_core_dev *dev);
void mlx5_dev_list_lock(void);
void mlx5_dev_list_unlock(void);
int mlx5_dev_list_trylock(void);
<<<<<<< HEAD
int mlx5_encap_alloc(struct mlx5_core_dev *dev,
		     int header_type,
		     size_t size,
		     void *encap_header,
		     u32 *encap_id);
void mlx5_encap_dealloc(struct mlx5_core_dev *dev, u32 encap_id);

int mlx5_modify_header_alloc(struct mlx5_core_dev *dev,
			     u8 namespace, u8 num_actions,
			     void *modify_actions, u32 *modify_header_id);
void mlx5_modify_header_dealloc(struct mlx5_core_dev *dev, u32 modify_header_id);

bool mlx5_lag_intf_add(struct mlx5_interface *intf, struct mlx5_priv *priv);
=======
>>>>>>> upstream/android-13

int mlx5_query_mtpps(struct mlx5_core_dev *dev, u32 *mtpps, u32 mtpps_size);
int mlx5_set_mtpps(struct mlx5_core_dev *mdev, u32 *mtpps, u32 mtpps_size);
int mlx5_query_mtppse(struct mlx5_core_dev *mdev, u8 pin, u8 *arm, u8 *mode);
int mlx5_set_mtppse(struct mlx5_core_dev *mdev, u8 pin, u8 arm, u8 mode);

<<<<<<< HEAD
=======
struct mlx5_dm *mlx5_dm_create(struct mlx5_core_dev *dev);
void mlx5_dm_cleanup(struct mlx5_core_dev *dev);

>>>>>>> upstream/android-13
#define MLX5_PPS_CAP(mdev) (MLX5_CAP_GEN((mdev), pps) &&		\
			    MLX5_CAP_GEN((mdev), pps_modify) &&		\
			    MLX5_CAP_MCAM_FEATURE((mdev), mtpps_fs) &&	\
			    MLX5_CAP_MCAM_FEATURE((mdev), mtpps_enh_out_per_adj))

<<<<<<< HEAD
int mlx5_firmware_flash(struct mlx5_core_dev *dev, const struct firmware *fw);

void mlx5e_init(void);
void mlx5e_cleanup(void);
=======
int mlx5_firmware_flash(struct mlx5_core_dev *dev, const struct firmware *fw,
			struct netlink_ext_ack *extack);
int mlx5_fw_version_query(struct mlx5_core_dev *dev,
			  u32 *running_ver, u32 *stored_ver);

#ifdef CONFIG_MLX5_CORE_EN
int mlx5e_init(void);
void mlx5e_cleanup(void);
#else
static inline int mlx5e_init(void){ return 0; }
static inline void mlx5e_cleanup(void){}
#endif

static inline bool mlx5_sriov_is_enabled(struct mlx5_core_dev *dev)
{
	return pci_num_vf(dev->pdev) ? true : false;
}
>>>>>>> upstream/android-13

static inline int mlx5_lag_is_lacp_owner(struct mlx5_core_dev *dev)
{
	/* LACP owner conditions:
	 * 1) Function is physical.
	 * 2) LAG is supported by FW.
	 * 3) LAG is managed by driver (currently the only option).
	 */
	return  MLX5_CAP_GEN(dev, vport_group_manager) &&
		   (MLX5_CAP_GEN(dev, num_lag_ports) > 1) &&
		    MLX5_CAP_GEN(dev, lag_master);
}

<<<<<<< HEAD
int mlx5_lag_allow(struct mlx5_core_dev *dev);
int mlx5_lag_forbid(struct mlx5_core_dev *dev);

void mlx5_reload_interface(struct mlx5_core_dev *mdev, int protocol);
=======
int mlx5_rescan_drivers_locked(struct mlx5_core_dev *dev);
static inline int mlx5_rescan_drivers(struct mlx5_core_dev *dev)
{
	int ret;

	mlx5_dev_list_lock();
	ret = mlx5_rescan_drivers_locked(dev);
	mlx5_dev_list_unlock();
	return ret;
}

void mlx5_lag_update(struct mlx5_core_dev *dev);

enum {
	MLX5_NIC_IFC_FULL		= 0,
	MLX5_NIC_IFC_DISABLED		= 1,
	MLX5_NIC_IFC_NO_DRAM_NIC	= 2,
	MLX5_NIC_IFC_SW_RESET		= 7
};

u8 mlx5_get_nic_state(struct mlx5_core_dev *dev);
void mlx5_set_nic_state(struct mlx5_core_dev *dev, u8 state);

static inline bool mlx5_core_is_sf(const struct mlx5_core_dev *dev)
{
	return dev->coredev_type == MLX5_COREDEV_SF;
}

int mlx5_mdev_init(struct mlx5_core_dev *dev, int profile_idx);
void mlx5_mdev_uninit(struct mlx5_core_dev *dev);
int mlx5_init_one(struct mlx5_core_dev *dev);
void mlx5_uninit_one(struct mlx5_core_dev *dev);
void mlx5_unload_one(struct mlx5_core_dev *dev);
int mlx5_load_one(struct mlx5_core_dev *dev);

int mlx5_vport_get_other_func_cap(struct mlx5_core_dev *dev, u16 function_id, void *out);

void mlx5_events_work_enqueue(struct mlx5_core_dev *dev, struct work_struct *work);
static inline u32 mlx5_sriov_get_vf_total_msix(struct pci_dev *pdev)
{
	struct mlx5_core_dev *dev = pci_get_drvdata(pdev);

	return MLX5_CAP_GEN_MAX(dev, num_total_dynamic_vf_msix);
}

bool mlx5_eth_supported(struct mlx5_core_dev *dev);
bool mlx5_rdma_supported(struct mlx5_core_dev *dev);
bool mlx5_vnet_supported(struct mlx5_core_dev *dev);

>>>>>>> upstream/android-13
#endif /* __MLX5_CORE_H__ */
