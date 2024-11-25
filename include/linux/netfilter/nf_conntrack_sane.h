/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_CONNTRACK_SANE_H
#define _NF_CONNTRACK_SANE_H
/* SANE tracking. */

<<<<<<< HEAD
#ifdef __KERNEL__

=======
>>>>>>> upstream/android-13
#define SANE_PORT	6566

enum sane_state {
	SANE_STATE_NORMAL,
	SANE_STATE_START_REQUESTED,
};

/* This structure exists only once per master */
struct nf_ct_sane_master {
	enum sane_state state;
};

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> upstream/android-13
#endif /* _NF_CONNTRACK_SANE_H */
