/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _FS_CEPH_MSGPOOL
#define _FS_CEPH_MSGPOOL

#include <linux/mempool.h>


struct ceph_msgpool {
	const char *name;
	mempool_t *pool;
	int type;               
	int front_len;          
};

extern int ceph_msgpool_init(struct ceph_msgpool *pool, int type,
			     int front_len, int size, bool blocking,
			     const char *name);
extern void ceph_msgpool_destroy(struct ceph_msgpool *pool);
extern struct ceph_msg *ceph_msgpool_get(struct ceph_msgpool *,
					 int front_len);
extern void ceph_msgpool_put(struct ceph_msgpool *, struct ceph_msg *);

#endif
