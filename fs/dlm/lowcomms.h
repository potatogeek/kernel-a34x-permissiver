<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
**  Copyright (C) 2004-2009 Red Hat, Inc.  All rights reserved.
**
<<<<<<< HEAD
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
=======
>>>>>>> upstream/android-13
**
*******************************************************************************
******************************************************************************/

#ifndef __LOWCOMMS_DOT_H__
#define __LOWCOMMS_DOT_H__

<<<<<<< HEAD
int dlm_lowcomms_start(void);
void dlm_lowcomms_stop(void);
void dlm_lowcomms_exit(void);
int dlm_lowcomms_close(int nodeid);
void *dlm_lowcomms_get_buffer(int nodeid, int len, gfp_t allocation, char **ppc);
void dlm_lowcomms_commit_buffer(void *mh);
int dlm_lowcomms_connect_node(int nodeid);
int dlm_lowcomms_addr(int nodeid, struct sockaddr_storage *addr, int len);
=======
#include "dlm_internal.h"

#define DLM_MIDCOMMS_OPT_LEN		sizeof(struct dlm_opts)
#define DLM_MAX_APP_BUFSIZE		(DLM_MAX_SOCKET_BUFSIZE - \
					 DLM_MIDCOMMS_OPT_LEN)

#define CONN_HASH_SIZE 32

/* This is deliberately very simple because most clusters have simple
 * sequential nodeids, so we should be able to go straight to a connection
 * struct in the array
 */
static inline int nodeid_hash(int nodeid)
{
	return nodeid & (CONN_HASH_SIZE-1);
}

/* switch to check if dlm is running */
extern int dlm_allow_conn;

int dlm_lowcomms_start(void);
void dlm_lowcomms_shutdown(void);
void dlm_lowcomms_stop(void);
void dlm_lowcomms_exit(void);
int dlm_lowcomms_close(int nodeid);
struct dlm_msg *dlm_lowcomms_new_msg(int nodeid, int len, gfp_t allocation,
				     char **ppc, void (*cb)(struct dlm_mhandle *mh),
				     struct dlm_mhandle *mh);
void dlm_lowcomms_commit_msg(struct dlm_msg *msg);
void dlm_lowcomms_put_msg(struct dlm_msg *msg);
int dlm_lowcomms_resend_msg(struct dlm_msg *msg);
int dlm_lowcomms_connect_node(int nodeid);
int dlm_lowcomms_nodes_set_mark(int nodeid, unsigned int mark);
int dlm_lowcomms_addr(int nodeid, struct sockaddr_storage *addr, int len);
void dlm_midcomms_receive_done(int nodeid);
>>>>>>> upstream/android-13

#endif				/* __LOWCOMMS_DOT_H__ */

