<<<<<<< HEAD
/*
 *
 * Intel Management Engine Interface (Intel MEI) Linux driver
 * Copyright (c) 2003-2012, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2003-2018, Intel Corporation. All rights reserved.
 * Intel Management Engine Interface (Intel MEI) Linux driver
>>>>>>> upstream/android-13
 */

#ifndef _MEI_HBM_H_
#define _MEI_HBM_H_

struct mei_device;
struct mei_msg_hdr;
struct mei_cl;
<<<<<<< HEAD
=======
struct mei_dma_data;
>>>>>>> upstream/android-13

/**
 * enum mei_hbm_state - host bus message protocol state
 *
 * @MEI_HBM_IDLE : protocol not started
 * @MEI_HBM_STARTING : start request message was sent
<<<<<<< HEAD
=======
 * @MEI_HBM_CAP_SETUP : capabilities request message was sent
 * @MEI_HBM_DR_SETUP : dma ring setup request message was sent
>>>>>>> upstream/android-13
 * @MEI_HBM_ENUM_CLIENTS : enumeration request was sent
 * @MEI_HBM_CLIENT_PROPERTIES : acquiring clients properties
 * @MEI_HBM_STARTED : enumeration was completed
 * @MEI_HBM_STOPPED : stopping exchange
 */
enum mei_hbm_state {
	MEI_HBM_IDLE = 0,
	MEI_HBM_STARTING,
<<<<<<< HEAD
=======
	MEI_HBM_CAP_SETUP,
	MEI_HBM_DR_SETUP,
>>>>>>> upstream/android-13
	MEI_HBM_ENUM_CLIENTS,
	MEI_HBM_CLIENT_PROPERTIES,
	MEI_HBM_STARTED,
	MEI_HBM_STOPPED,
};

const char *mei_hbm_state_str(enum mei_hbm_state state);

int mei_hbm_dispatch(struct mei_device *dev, struct mei_msg_hdr *hdr);

void mei_hbm_idle(struct mei_device *dev);
void mei_hbm_reset(struct mei_device *dev);
int mei_hbm_start_req(struct mei_device *dev);
int mei_hbm_start_wait(struct mei_device *dev);
int mei_hbm_cl_flow_control_req(struct mei_device *dev, struct mei_cl *cl);
int mei_hbm_cl_disconnect_req(struct mei_device *dev, struct mei_cl *cl);
int mei_hbm_cl_disconnect_rsp(struct mei_device *dev, struct mei_cl *cl);
int mei_hbm_cl_connect_req(struct mei_device *dev, struct mei_cl *cl);
bool mei_hbm_version_is_supported(struct mei_device *dev);
int mei_hbm_pg(struct mei_device *dev, u8 pg_cmd);
void mei_hbm_pg_resume(struct mei_device *dev);
int mei_hbm_cl_notify_req(struct mei_device *dev,
			  struct mei_cl *cl, u8 request);
<<<<<<< HEAD

=======
int mei_hbm_cl_dma_map_req(struct mei_device *dev, struct mei_cl *cl);
int mei_hbm_cl_dma_unmap_req(struct mei_device *dev, struct mei_cl *cl);
>>>>>>> upstream/android-13
#endif /* _MEI_HBM_H_ */

