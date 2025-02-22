/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_CONNTRACK_SANE_H
#define _NF_CONNTRACK_SANE_H


#ifdef __KERNEL__

#define SANE_PORT	6566

enum sane_state {
	SANE_STATE_NORMAL,
	SANE_STATE_START_REQUESTED,
};


struct nf_ct_sane_master {
	enum sane_state state;
};

#endif 

#endif 
