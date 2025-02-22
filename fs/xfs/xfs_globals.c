// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"
<<<<<<< HEAD
#include "xfs_sysctl.h"
=======
>>>>>>> upstream/android-13

/*
 * Tunable XFS parameters.  xfs_params is required even when CONFIG_SYSCTL=n,
 * other XFS code uses these values.  Times are measured in centisecs (i.e.
<<<<<<< HEAD
 * 100ths of a second) with the exception of eofb_timer and cowb_timer, which
 * are measured in seconds.
=======
 * 100ths of a second) with the exception of blockgc_timer, which is measured
 * in seconds.
>>>>>>> upstream/android-13
 */
xfs_param_t xfs_params = {
			  /*	MIN		DFLT		MAX	*/
	.sgid_inherit	= {	0,		0,		1	},
	.symlink_mode	= {	0,		0,		1	},
<<<<<<< HEAD
	.panic_mask	= {	0,		0,		255	},
=======
	.panic_mask	= {	0,		0,		256	},
>>>>>>> upstream/android-13
	.error_level	= {	0,		3,		11	},
	.syncd_timer	= {	1*100,		30*100,		7200*100},
	.stats_clear	= {	0,		0,		1	},
	.inherit_sync	= {	0,		1,		1	},
	.inherit_nodump	= {	0,		1,		1	},
	.inherit_noatim = {	0,		1,		1	},
	.xfs_buf_timer	= {	100/2,		1*100,		30*100	},
	.xfs_buf_age	= {	1*100,		15*100,		7200*100},
	.inherit_nosym	= {	0,		0,		1	},
	.rotorstep	= {	1,		1,		255	},
	.inherit_nodfrg	= {	0,		1,		1	},
	.fstrm_timer	= {	1,		30*100,		3600*100},
<<<<<<< HEAD
	.eofb_timer	= {	1,		300,		3600*24},
	.cowb_timer	= {	1,		1800,		3600*24},
=======
	.blockgc_timer	= {	1,		300,		3600*24},
>>>>>>> upstream/android-13
};

struct xfs_globals xfs_globals = {
	.log_recovery_delay	=	0,	/* no delay by default */
	.mount_delay		=	0,	/* no delay by default */
#ifdef XFS_ASSERT_FATAL
	.bug_on_assert		=	true,	/* assert failures BUG() */
#else
	.bug_on_assert		=	false,	/* assert failures WARN() */
#endif
<<<<<<< HEAD
=======
#ifdef DEBUG
	.pwork_threads		=	-1,	/* automatic thread detection */
#endif
>>>>>>> upstream/android-13
};
