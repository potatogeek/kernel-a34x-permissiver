/* Broadcom NetXtreme-C/E network driver.
 *
 * Copyright (c) 2017-2018 Broadcom Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */

<<<<<<< HEAD
#include <linux/net_dim.h>
=======
#include <linux/dim.h>
>>>>>>> upstream/android-13
#include "bnxt_hsi.h"
#include "bnxt.h"

void bnxt_dim_work(struct work_struct *work)
{
<<<<<<< HEAD
	struct net_dim *dim = container_of(work, struct net_dim,
					   work);
=======
	struct dim *dim = container_of(work, struct dim, work);
>>>>>>> upstream/android-13
	struct bnxt_cp_ring_info *cpr = container_of(dim,
						     struct bnxt_cp_ring_info,
						     dim);
	struct bnxt_napi *bnapi = container_of(cpr,
					       struct bnxt_napi,
					       cp_ring);
<<<<<<< HEAD
	struct net_dim_cq_moder cur_moder =
=======
	struct dim_cq_moder cur_moder =
>>>>>>> upstream/android-13
		net_dim_get_rx_moderation(dim->mode, dim->profile_ix);

	cpr->rx_ring_coal.coal_ticks = cur_moder.usec;
	cpr->rx_ring_coal.coal_bufs = cur_moder.pkts;

	bnxt_hwrm_set_ring_coal(bnapi->bp, bnapi);
<<<<<<< HEAD
	dim->state = NET_DIM_START_MEASURE;
=======
	dim->state = DIM_START_MEASURE;
>>>>>>> upstream/android-13
}
