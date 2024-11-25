/*
 * Copyright (c) 2014 Mellanox Technologies. All rights reserved.
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

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/sched/task.h>
#include <linux/pid.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <linux/vmalloc.h>
#include <linux/hugetlb.h>
<<<<<<< HEAD
#include <linux/interval_tree_generic.h>
=======
#include <linux/interval_tree.h>
#include <linux/hmm.h>
#include <linux/pagemap.h>
>>>>>>> upstream/android-13

#include <rdma/ib_verbs.h>
#include <rdma/ib_umem.h>
#include <rdma/ib_umem_odp.h>

<<<<<<< HEAD
/*
 * The ib_umem list keeps track of memory regions for which the HW
 * device request to receive notification when the related memory
 * mapping is changed.
 *
 * ib_umem_lock protects the list.
 */

static u64 node_start(struct umem_odp_node *n)
{
	struct ib_umem_odp *umem_odp =
			container_of(n, struct ib_umem_odp, interval_tree);

	return ib_umem_start(umem_odp->umem);
}

/* Note that the representation of the intervals in the interval tree
 * considers the ending point as contained in the interval, while the
 * function ib_umem_end returns the first address which is not contained
 * in the umem.
 */
static u64 node_last(struct umem_odp_node *n)
{
	struct ib_umem_odp *umem_odp =
			container_of(n, struct ib_umem_odp, interval_tree);

	return ib_umem_end(umem_odp->umem) - 1;
}

INTERVAL_TREE_DEFINE(struct umem_odp_node, rb, u64, __subtree_last,
		     node_start, node_last, static, rbt_ib_umem)

static void ib_umem_notifier_start_account(struct ib_umem *item)
{
	mutex_lock(&item->odp_data->umem_mutex);

	/* Only update private counters for this umem if it has them.
	 * Otherwise skip it. All page faults will be delayed for this umem. */
	if (item->odp_data->mn_counters_active) {
		int notifiers_count = item->odp_data->notifiers_count++;

		if (notifiers_count == 0)
			/* Initialize the completion object for waiting on
			 * notifiers. Since notifier_count is zero, no one
			 * should be waiting right now. */
			reinit_completion(&item->odp_data->notifier_completion);
	}
	mutex_unlock(&item->odp_data->umem_mutex);
}

static void ib_umem_notifier_end_account(struct ib_umem *item)
{
	mutex_lock(&item->odp_data->umem_mutex);

	/* Only update private counters for this umem if it has them.
	 * Otherwise skip it. All page faults will be delayed for this umem. */
	if (item->odp_data->mn_counters_active) {
		/*
		 * This sequence increase will notify the QP page fault that
		 * the page that is going to be mapped in the spte could have
		 * been freed.
		 */
		++item->odp_data->notifiers_seq;
		if (--item->odp_data->notifiers_count == 0)
			complete_all(&item->odp_data->notifier_completion);
	}
	mutex_unlock(&item->odp_data->umem_mutex);
}

/* Account for a new mmu notifier in an ib_ucontext. */
static void ib_ucontext_notifier_start_account(struct ib_ucontext *context)
{
	atomic_inc(&context->notifier_count);
}

/* Account for a terminating mmu notifier in an ib_ucontext.
 *
 * Must be called with the ib_ucontext->umem_rwsem semaphore unlocked, since
 * the function takes the semaphore itself. */
static void ib_ucontext_notifier_end_account(struct ib_ucontext *context)
{
	int zero_notifiers = atomic_dec_and_test(&context->notifier_count);

	if (zero_notifiers &&
	    !list_empty(&context->no_private_counters)) {
		/* No currently running mmu notifiers. Now is the chance to
		 * add private accounting to all previously added umems. */
		struct ib_umem_odp *odp_data, *next;

		/* Prevent concurrent mmu notifiers from working on the
		 * no_private_counters list. */
		down_write(&context->umem_rwsem);

		/* Read the notifier_count again, with the umem_rwsem
		 * semaphore taken for write. */
		if (!atomic_read(&context->notifier_count)) {
			list_for_each_entry_safe(odp_data, next,
						 &context->no_private_counters,
						 no_private_counters) {
				mutex_lock(&odp_data->umem_mutex);
				odp_data->mn_counters_active = true;
				list_del(&odp_data->no_private_counters);
				complete_all(&odp_data->notifier_completion);
				mutex_unlock(&odp_data->umem_mutex);
			}
		}

		up_write(&context->umem_rwsem);
	}
}

static int ib_umem_notifier_release_trampoline(struct ib_umem *item, u64 start,
					       u64 end, void *cookie) {
	/*
	 * Increase the number of notifiers running, to
	 * prevent any further fault handling on this MR.
	 */
	ib_umem_notifier_start_account(item);
	item->odp_data->dying = 1;
	/* Make sure that the fact the umem is dying is out before we release
	 * all pending page faults. */
	smp_wmb();
	complete_all(&item->odp_data->notifier_completion);
	item->context->invalidate_range(item, ib_umem_start(item),
					ib_umem_end(item));
	return 0;
}

static void ib_umem_notifier_release(struct mmu_notifier *mn,
				     struct mm_struct *mm)
{
	struct ib_ucontext *context = container_of(mn, struct ib_ucontext, mn);

	if (!context->invalidate_range)
		return;

	ib_ucontext_notifier_start_account(context);
	down_read(&context->umem_rwsem);
	rbt_ib_umem_for_each_in_range(&context->umem_tree, 0,
				      ULLONG_MAX,
				      ib_umem_notifier_release_trampoline,
				      true,
				      NULL);
	up_read(&context->umem_rwsem);
}

static int invalidate_page_trampoline(struct ib_umem *item, u64 start,
				      u64 end, void *cookie)
{
	ib_umem_notifier_start_account(item);
	item->context->invalidate_range(item, start, start + PAGE_SIZE);
	ib_umem_notifier_end_account(item);
	return 0;
}

static int invalidate_range_start_trampoline(struct ib_umem *item, u64 start,
					     u64 end, void *cookie)
{
	ib_umem_notifier_start_account(item);
	item->context->invalidate_range(item, start, end);
	return 0;
}

static int ib_umem_notifier_invalidate_range_start(struct mmu_notifier *mn,
						    struct mm_struct *mm,
						    unsigned long start,
						    unsigned long end,
						    bool blockable)
{
	struct ib_ucontext *context = container_of(mn, struct ib_ucontext, mn);
	int ret;

	if (!context->invalidate_range)
		return 0;

	if (blockable)
		down_read(&context->umem_rwsem);
	else if (!down_read_trylock(&context->umem_rwsem))
		return -EAGAIN;

	ib_ucontext_notifier_start_account(context);
	ret = rbt_ib_umem_for_each_in_range(&context->umem_tree, start,
				      end,
				      invalidate_range_start_trampoline,
				      blockable, NULL);
	up_read(&context->umem_rwsem);

	return ret;
}

static int invalidate_range_end_trampoline(struct ib_umem *item, u64 start,
					   u64 end, void *cookie)
{
	ib_umem_notifier_end_account(item);
	return 0;
}

static void ib_umem_notifier_invalidate_range_end(struct mmu_notifier *mn,
						  struct mm_struct *mm,
						  unsigned long start,
						  unsigned long end)
{
	struct ib_ucontext *context = container_of(mn, struct ib_ucontext, mn);

	if (!context->invalidate_range)
		return;

	/*
	 * TODO: we currently bail out if there is any sleepable work to be done
	 * in ib_umem_notifier_invalidate_range_start so we shouldn't really block
	 * here. But this is ugly and fragile.
	 */
	down_read(&context->umem_rwsem);
	rbt_ib_umem_for_each_in_range(&context->umem_tree, start,
				      end,
				      invalidate_range_end_trampoline, true, NULL);
	up_read(&context->umem_rwsem);
	ib_ucontext_notifier_end_account(context);
}

static const struct mmu_notifier_ops ib_umem_notifiers = {
	.release                    = ib_umem_notifier_release,
	.invalidate_range_start     = ib_umem_notifier_invalidate_range_start,
	.invalidate_range_end       = ib_umem_notifier_invalidate_range_end,
};

struct ib_umem *ib_alloc_odp_umem(struct ib_ucontext *context,
				  unsigned long addr,
				  size_t size)
{
	struct ib_umem *umem;
	struct ib_umem_odp *odp_data;
	int pages = size >> PAGE_SHIFT;
	int ret;

	umem = kzalloc(sizeof(*umem), GFP_KERNEL);
	if (!umem)
		return ERR_PTR(-ENOMEM);

	umem->context    = context;
	umem->length     = size;
	umem->address    = addr;
	umem->page_shift = PAGE_SHIFT;
	umem->writable   = 1;

	odp_data = kzalloc(sizeof(*odp_data), GFP_KERNEL);
	if (!odp_data) {
		ret = -ENOMEM;
		goto out_umem;
	}
	odp_data->umem = umem;

	mutex_init(&odp_data->umem_mutex);
	init_completion(&odp_data->notifier_completion);

	odp_data->page_list =
		vzalloc(array_size(pages, sizeof(*odp_data->page_list)));
	if (!odp_data->page_list) {
		ret = -ENOMEM;
		goto out_odp_data;
	}

	odp_data->dma_list =
		vzalloc(array_size(pages, sizeof(*odp_data->dma_list)));
	if (!odp_data->dma_list) {
		ret = -ENOMEM;
		goto out_page_list;
	}

	down_write(&context->umem_rwsem);
	context->odp_mrs_count++;
	rbt_ib_umem_insert(&odp_data->interval_tree, &context->umem_tree);
	if (likely(!atomic_read(&context->notifier_count)))
		odp_data->mn_counters_active = true;
	else
		list_add(&odp_data->no_private_counters,
			 &context->no_private_counters);
	up_write(&context->umem_rwsem);

	umem->odp_data = odp_data;

	return umem;

out_page_list:
	vfree(odp_data->page_list);
out_odp_data:
	kfree(odp_data);
out_umem:
	kfree(umem);
	return ERR_PTR(ret);
}
EXPORT_SYMBOL(ib_alloc_odp_umem);

int ib_umem_odp_get(struct ib_ucontext *context, struct ib_umem *umem,
		    int access)
{
	int ret_val;
	struct pid *our_pid;
	struct mm_struct *mm = get_task_mm(current);

	if (!mm)
		return -EINVAL;

	if (access & IB_ACCESS_HUGETLB) {
		struct vm_area_struct *vma;
		struct hstate *h;

		down_read(&mm->mmap_sem);
		vma = find_vma(mm, ib_umem_start(umem));
		if (!vma || !is_vm_hugetlb_page(vma)) {
			up_read(&mm->mmap_sem);
			ret_val = -EINVAL;
			goto out_mm;
		}
		h = hstate_vma(vma);
		umem->page_shift = huge_page_shift(h);
		up_read(&mm->mmap_sem);
		umem->hugetlb = 1;
	} else {
		umem->hugetlb = 0;
	}

	/* Prevent creating ODP MRs in child processes */
	rcu_read_lock();
	our_pid = get_task_pid(current->group_leader, PIDTYPE_PID);
	rcu_read_unlock();
	put_pid(our_pid);
	if (context->tgid != our_pid) {
		ret_val = -EINVAL;
		goto out_mm;
	}

	umem->odp_data = kzalloc(sizeof(*umem->odp_data), GFP_KERNEL);
	if (!umem->odp_data) {
		ret_val = -ENOMEM;
		goto out_mm;
	}
	umem->odp_data->umem = umem;

	mutex_init(&umem->odp_data->umem_mutex);

	init_completion(&umem->odp_data->notifier_completion);

	if (ib_umem_num_pages(umem)) {
		umem->odp_data->page_list =
			vzalloc(array_size(sizeof(*umem->odp_data->page_list),
					   ib_umem_num_pages(umem)));
		if (!umem->odp_data->page_list) {
			ret_val = -ENOMEM;
			goto out_odp_data;
		}

		umem->odp_data->dma_list =
			vzalloc(array_size(sizeof(*umem->odp_data->dma_list),
					   ib_umem_num_pages(umem)));
		if (!umem->odp_data->dma_list) {
			ret_val = -ENOMEM;
			goto out_page_list;
		}
	}

	/*
	 * When using MMU notifiers, we will get a
	 * notification before the "current" task (and MM) is
	 * destroyed. We use the umem_rwsem semaphore to synchronize.
	 */
	down_write(&context->umem_rwsem);
	context->odp_mrs_count++;
	if (likely(ib_umem_start(umem) != ib_umem_end(umem)))
		rbt_ib_umem_insert(&umem->odp_data->interval_tree,
				   &context->umem_tree);
	if (likely(!atomic_read(&context->notifier_count)) ||
	    context->odp_mrs_count == 1)
		umem->odp_data->mn_counters_active = true;
	else
		list_add(&umem->odp_data->no_private_counters,
			 &context->no_private_counters);
	downgrade_write(&context->umem_rwsem);

	if (context->odp_mrs_count == 1) {
		/*
		 * Note that at this point, no MMU notifier is running
		 * for this context!
		 */
		atomic_set(&context->notifier_count, 0);
		INIT_HLIST_NODE(&context->mn.hlist);
		context->mn.ops = &ib_umem_notifiers;
		/*
		 * Lock-dep detects a false positive for mmap_sem vs.
		 * umem_rwsem, due to not grasping downgrade_write correctly.
		 */
		lockdep_off();
		ret_val = mmu_notifier_register(&context->mn, mm);
		lockdep_on();
		if (ret_val) {
			pr_err("Failed to register mmu_notifier %d\n", ret_val);
			ret_val = -EBUSY;
			goto out_mutex;
		}
	}

	up_read(&context->umem_rwsem);

	/*
	 * Note that doing an mmput can cause a notifier for the relevant mm.
	 * If the notifier is called while we hold the umem_rwsem, this will
	 * cause a deadlock. Therefore, we release the reference only after we
	 * released the semaphore.
	 */
	mmput(mm);
	return 0;

out_mutex:
	up_read(&context->umem_rwsem);
	vfree(umem->odp_data->dma_list);
out_page_list:
	vfree(umem->odp_data->page_list);
out_odp_data:
	kfree(umem->odp_data);
out_mm:
	mmput(mm);
	return ret_val;
}

void ib_umem_odp_release(struct ib_umem *umem)
{
	struct ib_ucontext *context = umem->context;

=======
#include "uverbs.h"

static inline int ib_init_umem_odp(struct ib_umem_odp *umem_odp,
				   const struct mmu_interval_notifier_ops *ops)
{
	int ret;

	umem_odp->umem.is_odp = 1;
	mutex_init(&umem_odp->umem_mutex);

	if (!umem_odp->is_implicit_odp) {
		size_t page_size = 1UL << umem_odp->page_shift;
		unsigned long start;
		unsigned long end;
		size_t ndmas, npfns;

		start = ALIGN_DOWN(umem_odp->umem.address, page_size);
		if (check_add_overflow(umem_odp->umem.address,
				       (unsigned long)umem_odp->umem.length,
				       &end))
			return -EOVERFLOW;
		end = ALIGN(end, page_size);
		if (unlikely(end < page_size))
			return -EOVERFLOW;

		ndmas = (end - start) >> umem_odp->page_shift;
		if (!ndmas)
			return -EINVAL;

		npfns = (end - start) >> PAGE_SHIFT;
		umem_odp->pfn_list = kvcalloc(
			npfns, sizeof(*umem_odp->pfn_list), GFP_KERNEL);
		if (!umem_odp->pfn_list)
			return -ENOMEM;

		umem_odp->dma_list = kvcalloc(
			ndmas, sizeof(*umem_odp->dma_list), GFP_KERNEL);
		if (!umem_odp->dma_list) {
			ret = -ENOMEM;
			goto out_pfn_list;
		}

		ret = mmu_interval_notifier_insert(&umem_odp->notifier,
						   umem_odp->umem.owning_mm,
						   start, end - start, ops);
		if (ret)
			goto out_dma_list;
	}

	return 0;

out_dma_list:
	kvfree(umem_odp->dma_list);
out_pfn_list:
	kvfree(umem_odp->pfn_list);
	return ret;
}

/**
 * ib_umem_odp_alloc_implicit - Allocate a parent implicit ODP umem
 *
 * Implicit ODP umems do not have a VA range and do not have any page lists.
 * They exist only to hold the per_mm reference to help the driver create
 * children umems.
 *
 * @device: IB device to create UMEM
 * @access: ib_reg_mr access flags
 */
struct ib_umem_odp *ib_umem_odp_alloc_implicit(struct ib_device *device,
					       int access)
{
	struct ib_umem *umem;
	struct ib_umem_odp *umem_odp;
	int ret;

	if (access & IB_ACCESS_HUGETLB)
		return ERR_PTR(-EINVAL);

	umem_odp = kzalloc(sizeof(*umem_odp), GFP_KERNEL);
	if (!umem_odp)
		return ERR_PTR(-ENOMEM);
	umem = &umem_odp->umem;
	umem->ibdev = device;
	umem->writable = ib_access_writable(access);
	umem->owning_mm = current->mm;
	umem_odp->is_implicit_odp = 1;
	umem_odp->page_shift = PAGE_SHIFT;

	umem_odp->tgid = get_task_pid(current->group_leader, PIDTYPE_PID);
	ret = ib_init_umem_odp(umem_odp, NULL);
	if (ret) {
		put_pid(umem_odp->tgid);
		kfree(umem_odp);
		return ERR_PTR(ret);
	}
	return umem_odp;
}
EXPORT_SYMBOL(ib_umem_odp_alloc_implicit);

/**
 * ib_umem_odp_alloc_child - Allocate a child ODP umem under an implicit
 *                           parent ODP umem
 *
 * @root: The parent umem enclosing the child. This must be allocated using
 *        ib_alloc_implicit_odp_umem()
 * @addr: The starting userspace VA
 * @size: The length of the userspace VA
 * @ops: MMU interval ops, currently only @invalidate
 */
struct ib_umem_odp *
ib_umem_odp_alloc_child(struct ib_umem_odp *root, unsigned long addr,
			size_t size,
			const struct mmu_interval_notifier_ops *ops)
{
	/*
	 * Caller must ensure that root cannot be freed during the call to
	 * ib_alloc_odp_umem.
	 */
	struct ib_umem_odp *odp_data;
	struct ib_umem *umem;
	int ret;

	if (WARN_ON(!root->is_implicit_odp))
		return ERR_PTR(-EINVAL);

	odp_data = kzalloc(sizeof(*odp_data), GFP_KERNEL);
	if (!odp_data)
		return ERR_PTR(-ENOMEM);
	umem = &odp_data->umem;
	umem->ibdev = root->umem.ibdev;
	umem->length     = size;
	umem->address    = addr;
	umem->writable   = root->umem.writable;
	umem->owning_mm  = root->umem.owning_mm;
	odp_data->page_shift = PAGE_SHIFT;
	odp_data->notifier.ops = ops;

	/*
	 * A mmget must be held when registering a notifier, the owming_mm only
	 * has a mm_grab at this point.
	 */
	if (!mmget_not_zero(umem->owning_mm)) {
		ret = -EFAULT;
		goto out_free;
	}

	odp_data->tgid = get_pid(root->tgid);
	ret = ib_init_umem_odp(odp_data, ops);
	if (ret)
		goto out_tgid;
	mmput(umem->owning_mm);
	return odp_data;

out_tgid:
	put_pid(odp_data->tgid);
	mmput(umem->owning_mm);
out_free:
	kfree(odp_data);
	return ERR_PTR(ret);
}
EXPORT_SYMBOL(ib_umem_odp_alloc_child);

/**
 * ib_umem_odp_get - Create a umem_odp for a userspace va
 *
 * @device: IB device struct to get UMEM
 * @addr: userspace virtual address to start at
 * @size: length of region to pin
 * @access: IB_ACCESS_xxx flags for memory being pinned
 * @ops: MMU interval ops, currently only @invalidate
 *
 * The driver should use when the access flags indicate ODP memory. It avoids
 * pinning, instead, stores the mm for future page fault handling in
 * conjunction with MMU notifiers.
 */
struct ib_umem_odp *ib_umem_odp_get(struct ib_device *device,
				    unsigned long addr, size_t size, int access,
				    const struct mmu_interval_notifier_ops *ops)
{
	struct ib_umem_odp *umem_odp;
	struct mm_struct *mm;
	int ret;

	if (WARN_ON_ONCE(!(access & IB_ACCESS_ON_DEMAND)))
		return ERR_PTR(-EINVAL);

	umem_odp = kzalloc(sizeof(struct ib_umem_odp), GFP_KERNEL);
	if (!umem_odp)
		return ERR_PTR(-ENOMEM);

	umem_odp->umem.ibdev = device;
	umem_odp->umem.length = size;
	umem_odp->umem.address = addr;
	umem_odp->umem.writable = ib_access_writable(access);
	umem_odp->umem.owning_mm = mm = current->mm;
	umem_odp->notifier.ops = ops;

	umem_odp->page_shift = PAGE_SHIFT;
#ifdef CONFIG_HUGETLB_PAGE
	if (access & IB_ACCESS_HUGETLB)
		umem_odp->page_shift = HPAGE_SHIFT;
#endif

	umem_odp->tgid = get_task_pid(current->group_leader, PIDTYPE_PID);
	ret = ib_init_umem_odp(umem_odp, ops);
	if (ret)
		goto err_put_pid;
	return umem_odp;

err_put_pid:
	put_pid(umem_odp->tgid);
	kfree(umem_odp);
	return ERR_PTR(ret);
}
EXPORT_SYMBOL(ib_umem_odp_get);

void ib_umem_odp_release(struct ib_umem_odp *umem_odp)
{
>>>>>>> upstream/android-13
	/*
	 * Ensure that no more pages are mapped in the umem.
	 *
	 * It is the driver's responsibility to ensure, before calling us,
	 * that the hardware will not attempt to access the MR any more.
	 */
<<<<<<< HEAD
	ib_umem_odp_unmap_dma_pages(umem, ib_umem_start(umem),
				    ib_umem_end(umem));

	down_write(&context->umem_rwsem);
	if (likely(ib_umem_start(umem) != ib_umem_end(umem)))
		rbt_ib_umem_remove(&umem->odp_data->interval_tree,
				   &context->umem_tree);
	context->odp_mrs_count--;
	if (!umem->odp_data->mn_counters_active) {
		list_del(&umem->odp_data->no_private_counters);
		complete_all(&umem->odp_data->notifier_completion);
	}

	/*
	 * Downgrade the lock to a read lock. This ensures that the notifiers
	 * (who lock the mutex for reading) will be able to finish, and we
	 * will be able to enventually obtain the mmu notifiers SRCU. Note
	 * that since we are doing it atomically, no other user could register
	 * and unregister while we do the check.
	 */
	downgrade_write(&context->umem_rwsem);
	if (!context->odp_mrs_count) {
		struct task_struct *owning_process = NULL;
		struct mm_struct *owning_mm        = NULL;

		owning_process = get_pid_task(context->tgid,
					      PIDTYPE_PID);
		if (owning_process == NULL)
			/*
			 * The process is already dead, notifier were removed
			 * already.
			 */
			goto out;

		owning_mm = get_task_mm(owning_process);
		if (owning_mm == NULL)
			/*
			 * The process' mm is already dead, notifier were
			 * removed already.
			 */
			goto out_put_task;
		mmu_notifier_unregister(&context->mn, owning_mm);

		mmput(owning_mm);

out_put_task:
		put_task_struct(owning_process);
	}
out:
	up_read(&context->umem_rwsem);

	vfree(umem->odp_data->dma_list);
	vfree(umem->odp_data->page_list);
	kfree(umem->odp_data);
	kfree(umem);
}
=======
	if (!umem_odp->is_implicit_odp) {
		mutex_lock(&umem_odp->umem_mutex);
		ib_umem_odp_unmap_dma_pages(umem_odp, ib_umem_start(umem_odp),
					    ib_umem_end(umem_odp));
		mutex_unlock(&umem_odp->umem_mutex);
		mmu_interval_notifier_remove(&umem_odp->notifier);
		kvfree(umem_odp->dma_list);
		kvfree(umem_odp->pfn_list);
	}
	put_pid(umem_odp->tgid);
	kfree(umem_odp);
}
EXPORT_SYMBOL(ib_umem_odp_release);
>>>>>>> upstream/android-13

/*
 * Map for DMA and insert a single page into the on-demand paging page tables.
 *
 * @umem: the umem to insert the page to.
<<<<<<< HEAD
 * @page_index: index in the umem to add the page to.
 * @page: the page struct to map and add.
 * @access_mask: access permissions needed for this page.
 * @current_seq: sequence number for synchronization with invalidations.
 *               the sequence number is taken from
 *               umem->odp_data->notifiers_seq.
 *
 * The function returns -EFAULT if the DMA mapping operation fails. It returns
 * -EAGAIN if a concurrent invalidation prevents us from updating the page.
 *
 * The page is released via put_page even if the operation failed. For
 * on-demand pinning, the page is released whenever it isn't stored in the
 * umem.
 */
static int ib_umem_odp_map_dma_single_page(
		struct ib_umem *umem,
		int page_index,
		struct page *page,
		u64 access_mask,
		unsigned long current_seq)
{
	struct ib_device *dev = umem->context->device;
	dma_addr_t dma_addr;
	int stored_page = 0;
	int remove_existing_mapping = 0;
	int ret = 0;

	/*
	 * Note: we avoid writing if seq is different from the initial seq, to
	 * handle case of a racing notifier. This check also allows us to bail
	 * early if we have a notifier running in parallel with us.
	 */
	if (ib_umem_mmu_notifier_retry(umem, current_seq)) {
		ret = -EAGAIN;
		goto out;
	}
	if (!(umem->odp_data->dma_list[page_index])) {
		dma_addr = ib_dma_map_page(dev,
					   page,
					   0, BIT(umem->page_shift),
					   DMA_BIDIRECTIONAL);
		if (ib_dma_mapping_error(dev, dma_addr)) {
			ret = -EFAULT;
			goto out;
		}
		umem->odp_data->dma_list[page_index] = dma_addr | access_mask;
		umem->odp_data->page_list[page_index] = page;
		umem->npages++;
		stored_page = 1;
	} else if (umem->odp_data->page_list[page_index] == page) {
		umem->odp_data->dma_list[page_index] |= access_mask;
	} else {
		pr_err("error: got different pages in IB device and from get_user_pages. IB device page: %p, gup page: %p\n",
		       umem->odp_data->page_list[page_index], page);
		/* Better remove the mapping now, to prevent any further
		 * damage. */
		remove_existing_mapping = 1;
	}

out:
	/* On Demand Paging - avoid pinning the page */
	if (umem->context->invalidate_range || !stored_page)
		put_page(page);

	if (remove_existing_mapping && umem->context->invalidate_range) {
		invalidate_page_trampoline(
			umem,
			ib_umem_start(umem) + (page_index >> umem->page_shift),
			ib_umem_start(umem) + ((page_index + 1) >>
					       umem->page_shift),
			NULL);
		ret = -EAGAIN;
	}

	return ret;
}

/**
 * ib_umem_odp_map_dma_pages - Pin and DMA map userspace memory in an ODP MR.
 *
 * Pins the range of pages passed in the argument, and maps them to
 * DMA addresses. The DMA addresses of the mapped pages is updated in
 * umem->odp_data->dma_list.
 *
 * Returns the number of pages mapped in success, negative error code
 * for failure.
 * An -EAGAIN error code is returned when a concurrent mmu notifier prevents
 * the function from completing its task.
 * An -ENOENT error code indicates that userspace process is being terminated
 * and mm was already destroyed.
 * @umem: the umem to map and pin
=======
 * @dma_index: index in the umem to add the dma to.
 * @page: the page struct to map and add.
 * @access_mask: access permissions needed for this page.
 *
 * The function returns -EFAULT if the DMA mapping operation fails.
 *
 */
static int ib_umem_odp_map_dma_single_page(
		struct ib_umem_odp *umem_odp,
		unsigned int dma_index,
		struct page *page,
		u64 access_mask)
{
	struct ib_device *dev = umem_odp->umem.ibdev;
	dma_addr_t *dma_addr = &umem_odp->dma_list[dma_index];

	if (*dma_addr) {
		/*
		 * If the page is already dma mapped it means it went through
		 * a non-invalidating trasition, like read-only to writable.
		 * Resync the flags.
		 */
		*dma_addr = (*dma_addr & ODP_DMA_ADDR_MASK) | access_mask;
		return 0;
	}

	*dma_addr = ib_dma_map_page(dev, page, 0, 1 << umem_odp->page_shift,
				    DMA_BIDIRECTIONAL);
	if (ib_dma_mapping_error(dev, *dma_addr)) {
		*dma_addr = 0;
		return -EFAULT;
	}
	umem_odp->npages++;
	*dma_addr |= access_mask;
	return 0;
}

/**
 * ib_umem_odp_map_dma_and_lock - DMA map userspace memory in an ODP MR and lock it.
 *
 * Maps the range passed in the argument to DMA addresses.
 * The DMA addresses of the mapped pages is updated in umem_odp->dma_list.
 * Upon success the ODP MR will be locked to let caller complete its device
 * page table update.
 *
 * Returns the number of pages mapped in success, negative error code
 * for failure.
 * @umem_odp: the umem to map and pin
>>>>>>> upstream/android-13
 * @user_virt: the address from which we need to map.
 * @bcnt: the minimal number of bytes to pin and map. The mapping might be
 *        bigger due to alignment, and may also be smaller in case of an error
 *        pinning or mapping a page. The actual pages mapped is returned in
 *        the return value.
 * @access_mask: bit mask of the requested access permissions for the given
 *               range.
<<<<<<< HEAD
 * @current_seq: the MMU notifiers sequance value for synchronization with
 *               invalidations. the sequance number is read from
 *               umem->odp_data->notifiers_seq before calling this function
 */
int ib_umem_odp_map_dma_pages(struct ib_umem *umem, u64 user_virt, u64 bcnt,
			      u64 access_mask, unsigned long current_seq)
{
	struct task_struct *owning_process  = NULL;
	struct mm_struct   *owning_mm       = NULL;
	struct page       **local_page_list = NULL;
	u64 page_mask, off;
	int j, k, ret = 0, start_idx, npages = 0, page_shift;
	unsigned int flags = 0;
	phys_addr_t p = 0;
=======
 * @fault: is faulting required for the given range
 */
int ib_umem_odp_map_dma_and_lock(struct ib_umem_odp *umem_odp, u64 user_virt,
				 u64 bcnt, u64 access_mask, bool fault)
			__acquires(&umem_odp->umem_mutex)
{
	struct task_struct *owning_process  = NULL;
	struct mm_struct *owning_mm = umem_odp->umem.owning_mm;
	int pfn_index, dma_index, ret = 0, start_idx;
	unsigned int page_shift, hmm_order, pfn_start_idx;
	unsigned long num_pfns, current_seq;
	struct hmm_range range = {};
	unsigned long timeout;
>>>>>>> upstream/android-13

	if (access_mask == 0)
		return -EINVAL;

<<<<<<< HEAD
	if (user_virt < ib_umem_start(umem) ||
	    user_virt + bcnt > ib_umem_end(umem))
		return -EFAULT;

	local_page_list = (struct page **)__get_free_page(GFP_KERNEL);
	if (!local_page_list)
		return -ENOMEM;

	page_shift = umem->page_shift;
	page_mask = ~(BIT(page_shift) - 1);
	off = user_virt & (~page_mask);
	user_virt = user_virt & page_mask;
	bcnt += off; /* Charge for the first page offset as well. */

	owning_process = get_pid_task(umem->context->tgid, PIDTYPE_PID);
	if (owning_process == NULL) {
		ret = -EINVAL;
		goto out_no_task;
	}

	owning_mm = get_task_mm(owning_process);
	if (owning_mm == NULL) {
		ret = -ENOENT;
		goto out_put_task;
	}

	if (access_mask & ODP_WRITE_ALLOWED_BIT)
		flags |= FOLL_WRITE;

	start_idx = (user_virt - ib_umem_start(umem)) >> page_shift;
	k = start_idx;

	while (bcnt > 0) {
		const size_t gup_num_pages = min_t(size_t,
				ALIGN(bcnt, PAGE_SIZE) / PAGE_SIZE,
				PAGE_SIZE / sizeof(struct page *));

		down_read(&owning_mm->mmap_sem);
		/*
		 * Note: this might result in redundent page getting. We can
		 * avoid this by checking dma_list to be 0 before calling
		 * get_user_pages. However, this make the code much more
		 * complex (and doesn't gain us much performance in most use
		 * cases).
		 */
		npages = get_user_pages_remote(owning_process, owning_mm,
				user_virt, gup_num_pages,
				flags, local_page_list, NULL, NULL);
		up_read(&owning_mm->mmap_sem);

		if (npages < 0)
			break;

		bcnt -= min_t(size_t, npages << PAGE_SHIFT, bcnt);
		mutex_lock(&umem->odp_data->umem_mutex);
		for (j = 0; j < npages; j++, user_virt += PAGE_SIZE) {
			if (user_virt & ~page_mask) {
				p += PAGE_SIZE;
				if (page_to_phys(local_page_list[j]) != p) {
					ret = -EFAULT;
					break;
				}
				put_page(local_page_list[j]);
				continue;
			}

			ret = ib_umem_odp_map_dma_single_page(
					umem, k, local_page_list[j],
					access_mask, current_seq);
			if (ret < 0)
				break;

			p = page_to_phys(local_page_list[j]);
			k++;
		}
		mutex_unlock(&umem->odp_data->umem_mutex);

		if (ret < 0) {
			/* Release left over pages when handling errors. */
			for (++j; j < npages; ++j)
				put_page(local_page_list[j]);
			break;
		}
	}

	if (ret >= 0) {
		if (npages < 0 && k == start_idx)
			ret = npages;
		else
			ret = k - start_idx;
	}

	mmput(owning_mm);
out_put_task:
	put_task_struct(owning_process);
out_no_task:
	free_page((unsigned long)local_page_list);
	return ret;
}
EXPORT_SYMBOL(ib_umem_odp_map_dma_pages);

void ib_umem_odp_unmap_dma_pages(struct ib_umem *umem, u64 virt,
				 u64 bound)
{
	int idx;
	u64 addr;
	struct ib_device *dev = umem->context->device;

	virt  = max_t(u64, virt,  ib_umem_start(umem));
	bound = min_t(u64, bound, ib_umem_end(umem));
	/* Note that during the run of this function, the
	 * notifiers_count of the MR is > 0, preventing any racing
	 * faults from completion. We might be racing with other
	 * invalidations, so we must make sure we free each page only
	 * once. */
	mutex_lock(&umem->odp_data->umem_mutex);
	for (addr = virt; addr < bound; addr += BIT(umem->page_shift)) {
		idx = (addr - ib_umem_start(umem)) >> umem->page_shift;
		if (umem->odp_data->page_list[idx]) {
			struct page *page = umem->odp_data->page_list[idx];
			dma_addr_t dma = umem->odp_data->dma_list[idx];
			dma_addr_t dma_addr = dma & ODP_DMA_ADDR_MASK;

			WARN_ON(!dma_addr);

			ib_dma_unmap_page(dev, dma_addr, PAGE_SIZE,
=======
	if (user_virt < ib_umem_start(umem_odp) ||
	    user_virt + bcnt > ib_umem_end(umem_odp))
		return -EFAULT;

	page_shift = umem_odp->page_shift;

	/*
	 * owning_process is allowed to be NULL, this means somehow the mm is
	 * existing beyond the lifetime of the originating process.. Presumably
	 * mmget_not_zero will fail in this case.
	 */
	owning_process = get_pid_task(umem_odp->tgid, PIDTYPE_PID);
	if (!owning_process || !mmget_not_zero(owning_mm)) {
		ret = -EINVAL;
		goto out_put_task;
	}

	range.notifier = &umem_odp->notifier;
	range.start = ALIGN_DOWN(user_virt, 1UL << page_shift);
	range.end = ALIGN(user_virt + bcnt, 1UL << page_shift);
	pfn_start_idx = (range.start - ib_umem_start(umem_odp)) >> PAGE_SHIFT;
	num_pfns = (range.end - range.start) >> PAGE_SHIFT;
	if (fault) {
		range.default_flags = HMM_PFN_REQ_FAULT;

		if (access_mask & ODP_WRITE_ALLOWED_BIT)
			range.default_flags |= HMM_PFN_REQ_WRITE;
	}

	range.hmm_pfns = &(umem_odp->pfn_list[pfn_start_idx]);
	timeout = jiffies + msecs_to_jiffies(HMM_RANGE_DEFAULT_TIMEOUT);

retry:
	current_seq = range.notifier_seq =
		mmu_interval_read_begin(&umem_odp->notifier);

	mmap_read_lock(owning_mm);
	ret = hmm_range_fault(&range);
	mmap_read_unlock(owning_mm);
	if (unlikely(ret)) {
		if (ret == -EBUSY && !time_after(jiffies, timeout))
			goto retry;
		goto out_put_mm;
	}

	start_idx = (range.start - ib_umem_start(umem_odp)) >> page_shift;
	dma_index = start_idx;

	mutex_lock(&umem_odp->umem_mutex);
	if (mmu_interval_read_retry(&umem_odp->notifier, current_seq)) {
		mutex_unlock(&umem_odp->umem_mutex);
		goto retry;
	}

	for (pfn_index = 0; pfn_index < num_pfns;
		pfn_index += 1 << (page_shift - PAGE_SHIFT), dma_index++) {

		if (fault) {
			/*
			 * Since we asked for hmm_range_fault() to populate
			 * pages it shouldn't return an error entry on success.
			 */
			WARN_ON(range.hmm_pfns[pfn_index] & HMM_PFN_ERROR);
			WARN_ON(!(range.hmm_pfns[pfn_index] & HMM_PFN_VALID));
		} else {
			if (!(range.hmm_pfns[pfn_index] & HMM_PFN_VALID)) {
				WARN_ON(umem_odp->dma_list[dma_index]);
				continue;
			}
			access_mask = ODP_READ_ALLOWED_BIT;
			if (range.hmm_pfns[pfn_index] & HMM_PFN_WRITE)
				access_mask |= ODP_WRITE_ALLOWED_BIT;
		}

		hmm_order = hmm_pfn_to_map_order(range.hmm_pfns[pfn_index]);
		/* If a hugepage was detected and ODP wasn't set for, the umem
		 * page_shift will be used, the opposite case is an error.
		 */
		if (hmm_order + PAGE_SHIFT < page_shift) {
			ret = -EINVAL;
			ibdev_dbg(umem_odp->umem.ibdev,
				  "%s: un-expected hmm_order %u, page_shift %u\n",
				  __func__, hmm_order, page_shift);
			break;
		}

		ret = ib_umem_odp_map_dma_single_page(
				umem_odp, dma_index, hmm_pfn_to_page(range.hmm_pfns[pfn_index]),
				access_mask);
		if (ret < 0) {
			ibdev_dbg(umem_odp->umem.ibdev,
				  "ib_umem_odp_map_dma_single_page failed with error %d\n", ret);
			break;
		}
	}
	/* upon sucesss lock should stay on hold for the callee */
	if (!ret)
		ret = dma_index - start_idx;
	else
		mutex_unlock(&umem_odp->umem_mutex);

out_put_mm:
	mmput(owning_mm);
out_put_task:
	if (owning_process)
		put_task_struct(owning_process);
	return ret;
}
EXPORT_SYMBOL(ib_umem_odp_map_dma_and_lock);

void ib_umem_odp_unmap_dma_pages(struct ib_umem_odp *umem_odp, u64 virt,
				 u64 bound)
{
	dma_addr_t dma_addr;
	dma_addr_t dma;
	int idx;
	u64 addr;
	struct ib_device *dev = umem_odp->umem.ibdev;

	lockdep_assert_held(&umem_odp->umem_mutex);

	virt = max_t(u64, virt, ib_umem_start(umem_odp));
	bound = min_t(u64, bound, ib_umem_end(umem_odp));
	for (addr = virt; addr < bound; addr += BIT(umem_odp->page_shift)) {
		idx = (addr - ib_umem_start(umem_odp)) >> umem_odp->page_shift;
		dma = umem_odp->dma_list[idx];

		/* The access flags guaranteed a valid DMA address in case was NULL */
		if (dma) {
			unsigned long pfn_idx = (addr - ib_umem_start(umem_odp)) >> PAGE_SHIFT;
			struct page *page = hmm_pfn_to_page(umem_odp->pfn_list[pfn_idx]);

			dma_addr = dma & ODP_DMA_ADDR_MASK;
			ib_dma_unmap_page(dev, dma_addr,
					  BIT(umem_odp->page_shift),
>>>>>>> upstream/android-13
					  DMA_BIDIRECTIONAL);
			if (dma & ODP_WRITE_ALLOWED_BIT) {
				struct page *head_page = compound_head(page);
				/*
				 * set_page_dirty prefers being called with
				 * the page lock. However, MMU notifiers are
				 * called sometimes with and sometimes without
				 * the lock. We rely on the umem_mutex instead
				 * to prevent other mmu notifiers from
				 * continuing and allowing the page mapping to
				 * be removed.
				 */
				set_page_dirty(head_page);
			}
<<<<<<< HEAD
			/* on demand pinning support */
			if (!umem->context->invalidate_range)
				put_page(page);
			umem->odp_data->page_list[idx] = NULL;
			umem->odp_data->dma_list[idx] = 0;
			umem->npages--;
		}
	}
	mutex_unlock(&umem->odp_data->umem_mutex);
}
EXPORT_SYMBOL(ib_umem_odp_unmap_dma_pages);

/* @last is not a part of the interval. See comment for function
 * node_last.
 */
int rbt_ib_umem_for_each_in_range(struct rb_root_cached *root,
				  u64 start, u64 last,
				  umem_call_back cb,
				  bool blockable,
				  void *cookie)
{
	int ret_val = 0;
	struct umem_odp_node *node, *next;
	struct ib_umem_odp *umem;

	if (unlikely(start == last))
		return ret_val;

	for (node = rbt_ib_umem_iter_first(root, start, last - 1);
			node; node = next) {
		/* TODO move the blockable decision up to the callback */
		if (!blockable)
			return -EAGAIN;
		next = rbt_ib_umem_iter_next(node, start, last - 1);
		umem = container_of(node, struct ib_umem_odp, interval_tree);
		ret_val = cb(umem->umem, start, last, cookie) || ret_val;
	}

	return ret_val;
}
EXPORT_SYMBOL(rbt_ib_umem_for_each_in_range);

struct ib_umem_odp *rbt_ib_umem_lookup(struct rb_root_cached *root,
				       u64 addr, u64 length)
{
	struct umem_odp_node *node;

	node = rbt_ib_umem_iter_first(root, addr, addr + length - 1);
	if (node)
		return container_of(node, struct ib_umem_odp, interval_tree);
	return NULL;

}
EXPORT_SYMBOL(rbt_ib_umem_lookup);
=======
			umem_odp->dma_list[idx] = 0;
			umem_odp->npages--;
		}
	}
}
EXPORT_SYMBOL(ib_umem_odp_unmap_dma_pages);
>>>>>>> upstream/android-13
