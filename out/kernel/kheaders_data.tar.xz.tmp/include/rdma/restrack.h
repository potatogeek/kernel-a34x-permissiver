/* SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB */


#ifndef _RDMA_RESTRACK_H_
#define _RDMA_RESTRACK_H_

#include <linux/typecheck.h>
#include <linux/rwsem.h>
#include <linux/sched.h>
#include <linux/kref.h>
#include <linux/completion.h>
#include <linux/sched/task.h>
#include <uapi/rdma/rdma_netlink.h>


enum rdma_restrack_type {
	
	RDMA_RESTRACK_PD,
	
	RDMA_RESTRACK_CQ,
	
	RDMA_RESTRACK_QP,
	
	RDMA_RESTRACK_CM_ID,
	
	RDMA_RESTRACK_MR,
	
	RDMA_RESTRACK_MAX
};

#define RDMA_RESTRACK_HASH_BITS	8
struct rdma_restrack_entry;


struct rdma_restrack_root {
	
	struct rw_semaphore	rwsem;
	
	DECLARE_HASHTABLE(hash, RDMA_RESTRACK_HASH_BITS);
	
	int (*fill_res_entry)(struct sk_buff *msg,
			      struct rdma_restrack_entry *entry);
};


struct rdma_restrack_entry {
	
	bool			valid;
	
	struct kref		kref;
	
	struct completion	comp;
	
	struct task_struct	*task;
	
	const char		*kern_name;
	
	struct hlist_node	node;
	
	enum rdma_restrack_type	type;
};


void rdma_restrack_init(struct rdma_restrack_root *res);


void rdma_restrack_clean(struct rdma_restrack_root *res);


int rdma_restrack_count(struct rdma_restrack_root *res,
			enum rdma_restrack_type type,
			struct pid_namespace *ns);


void rdma_restrack_add(struct rdma_restrack_entry *res);


void rdma_restrack_del(struct rdma_restrack_entry *res);


static inline bool rdma_is_kernel_res(struct rdma_restrack_entry *res)
{
	return !res->task;
}


int __must_check rdma_restrack_get(struct rdma_restrack_entry *res);


int rdma_restrack_put(struct rdma_restrack_entry *res);


static inline void rdma_restrack_set_task(struct rdma_restrack_entry *res,
					  struct task_struct *task)
{
	if (res->task)
		put_task_struct(res->task);
	get_task_struct(task);
	res->task = task;
}


int rdma_nl_put_driver_u32(struct sk_buff *msg, const char *name, u32 value);
int rdma_nl_put_driver_u32_hex(struct sk_buff *msg, const char *name,
			       u32 value);
int rdma_nl_put_driver_u64(struct sk_buff *msg, const char *name, u64 value);
int rdma_nl_put_driver_u64_hex(struct sk_buff *msg, const char *name,
			       u64 value);
#endif 
