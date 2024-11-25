// SPDX-License-Identifier: GPL-2.0
/* XDP user-space packet buffer
 * Copyright(c) 2018 Intel Corporation.
 */

#include <linux/init.h>
#include <linux/sched/mm.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/bpf.h>
#include <linux/mm.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>
<<<<<<< HEAD
=======
#include <linux/idr.h>
#include <linux/vmalloc.h>
>>>>>>> upstream/android-13

#include "xdp_umem.h"
#include "xsk_queue.h"

#define XDP_UMEM_MIN_CHUNK_SIZE 2048

<<<<<<< HEAD
void xdp_add_sk_umem(struct xdp_umem *umem, struct xdp_sock *xs)
{
	unsigned long flags;

	if (!xs->tx)
		return;

	spin_lock_irqsave(&umem->xsk_list_lock, flags);
	list_add_rcu(&xs->list, &umem->xsk_list);
	spin_unlock_irqrestore(&umem->xsk_list_lock, flags);
}

void xdp_del_sk_umem(struct xdp_umem *umem, struct xdp_sock *xs)
{
	unsigned long flags;

	if (!xs->tx)
		return;

	spin_lock_irqsave(&umem->xsk_list_lock, flags);
	list_del_rcu(&xs->list);
	spin_unlock_irqrestore(&umem->xsk_list_lock, flags);
}

int xdp_umem_query(struct net_device *dev, u16 queue_id)
{
	struct netdev_bpf bpf;

	ASSERT_RTNL();

	memset(&bpf, 0, sizeof(bpf));
	bpf.command = XDP_QUERY_XSK_UMEM;
	bpf.xsk.queue_id = queue_id;

	if (!dev->netdev_ops->ndo_bpf)
		return 0;
	return dev->netdev_ops->ndo_bpf(dev, &bpf) ?: !!bpf.xsk.umem;
}

int xdp_umem_assign_dev(struct xdp_umem *umem, struct net_device *dev,
			u32 queue_id, u16 flags)
{
	bool force_zc, force_copy;
	struct netdev_bpf bpf;
	int err;

	force_zc = flags & XDP_ZEROCOPY;
	force_copy = flags & XDP_COPY;

	if (force_zc && force_copy)
		return -EINVAL;

	if (force_copy)
		return 0;

	if (!dev->netdev_ops->ndo_bpf || !dev->netdev_ops->ndo_xsk_async_xmit)
		return force_zc ? -EOPNOTSUPP : 0; /* fail or fallback */

	bpf.command = XDP_QUERY_XSK_UMEM;

	rtnl_lock();
	err = xdp_umem_query(dev, queue_id);
	if (err) {
		err = err < 0 ? -EOPNOTSUPP : -EBUSY;
		goto err_rtnl_unlock;
	}

	bpf.command = XDP_SETUP_XSK_UMEM;
	bpf.xsk.umem = umem;
	bpf.xsk.queue_id = queue_id;

	err = dev->netdev_ops->ndo_bpf(dev, &bpf);
	if (err)
		goto err_rtnl_unlock;
	rtnl_unlock();

	dev_hold(dev);
	umem->dev = dev;
	umem->queue_id = queue_id;
	umem->zc = true;
	return 0;

err_rtnl_unlock:
	rtnl_unlock();
	return force_zc ? err : 0; /* fail or fallback */
}

static void xdp_umem_clear_dev(struct xdp_umem *umem)
{
	struct netdev_bpf bpf;
	int err;

	if (umem->dev) {
		bpf.command = XDP_SETUP_XSK_UMEM;
		bpf.xsk.umem = NULL;
		bpf.xsk.queue_id = umem->queue_id;

		rtnl_lock();
		err = umem->dev->netdev_ops->ndo_bpf(umem->dev, &bpf);
		rtnl_unlock();

		if (err)
			WARN(1, "failed to disable umem!\n");

		dev_put(umem->dev);
		umem->dev = NULL;
	}
}

static void xdp_umem_unpin_pages(struct xdp_umem *umem)
{
	unsigned int i;

	for (i = 0; i < umem->npgs; i++) {
		struct page *page = umem->pgs[i];

		set_page_dirty_lock(page);
		put_page(page);
	}

	kfree(umem->pgs);
=======
static DEFINE_IDA(umem_ida);

static void xdp_umem_unpin_pages(struct xdp_umem *umem)
{
	unpin_user_pages_dirty_lock(umem->pgs, umem->npgs, true);

	kvfree(umem->pgs);
>>>>>>> upstream/android-13
	umem->pgs = NULL;
}

static void xdp_umem_unaccount_pages(struct xdp_umem *umem)
{
	if (umem->user) {
		atomic_long_sub(umem->npgs, &umem->user->locked_vm);
		free_uid(umem->user);
	}
}

<<<<<<< HEAD
static void xdp_umem_release(struct xdp_umem *umem)
{
	xdp_umem_clear_dev(umem);

	if (umem->fq) {
		xskq_destroy(umem->fq);
		umem->fq = NULL;
	}

	if (umem->cq) {
		xskq_destroy(umem->cq);
		umem->cq = NULL;
	}

	xdp_umem_unpin_pages(umem);

	kfree(umem->pages);
	umem->pages = NULL;

=======
static void xdp_umem_addr_unmap(struct xdp_umem *umem)
{
	vunmap(umem->addrs);
	umem->addrs = NULL;
}

static int xdp_umem_addr_map(struct xdp_umem *umem, struct page **pages,
			     u32 nr_pages)
{
	umem->addrs = vmap(pages, nr_pages, VM_MAP, PAGE_KERNEL);
	if (!umem->addrs)
		return -ENOMEM;
	return 0;
}

static void xdp_umem_release(struct xdp_umem *umem)
{
	umem->zc = false;
	ida_simple_remove(&umem_ida, umem->id);

	xdp_umem_addr_unmap(umem);
	xdp_umem_unpin_pages(umem);

>>>>>>> upstream/android-13
	xdp_umem_unaccount_pages(umem);
	kfree(umem);
}

static void xdp_umem_release_deferred(struct work_struct *work)
{
	struct xdp_umem *umem = container_of(work, struct xdp_umem, work);

	xdp_umem_release(umem);
}

void xdp_get_umem(struct xdp_umem *umem)
{
	refcount_inc(&umem->users);
}

<<<<<<< HEAD
void xdp_put_umem(struct xdp_umem *umem)
=======
void xdp_put_umem(struct xdp_umem *umem, bool defer_cleanup)
>>>>>>> upstream/android-13
{
	if (!umem)
		return;

	if (refcount_dec_and_test(&umem->users)) {
<<<<<<< HEAD
		INIT_WORK(&umem->work, xdp_umem_release_deferred);
		schedule_work(&umem->work);
	}
}

static int xdp_umem_pin_pages(struct xdp_umem *umem)
=======
		if (defer_cleanup) {
			INIT_WORK(&umem->work, xdp_umem_release_deferred);
			schedule_work(&umem->work);
		} else {
			xdp_umem_release(umem);
		}
	}
}

static int xdp_umem_pin_pages(struct xdp_umem *umem, unsigned long address)
>>>>>>> upstream/android-13
{
	unsigned int gup_flags = FOLL_WRITE;
	long npgs;
	int err;

<<<<<<< HEAD
	umem->pgs = kcalloc(umem->npgs, sizeof(*umem->pgs),
			    GFP_KERNEL | __GFP_NOWARN);
	if (!umem->pgs)
		return -ENOMEM;

	down_write(&current->mm->mmap_sem);
	npgs = get_user_pages(umem->address, umem->npgs,
			      gup_flags, &umem->pgs[0], NULL);
	up_write(&current->mm->mmap_sem);
=======
	umem->pgs = kvcalloc(umem->npgs, sizeof(*umem->pgs), GFP_KERNEL | __GFP_NOWARN);
	if (!umem->pgs)
		return -ENOMEM;

	mmap_read_lock(current->mm);
	npgs = pin_user_pages(address, umem->npgs,
			      gup_flags | FOLL_LONGTERM, &umem->pgs[0], NULL);
	mmap_read_unlock(current->mm);
>>>>>>> upstream/android-13

	if (npgs != umem->npgs) {
		if (npgs >= 0) {
			umem->npgs = npgs;
			err = -ENOMEM;
			goto out_pin;
		}
		err = npgs;
		goto out_pgs;
	}
	return 0;

out_pin:
	xdp_umem_unpin_pages(umem);
out_pgs:
<<<<<<< HEAD
	kfree(umem->pgs);
=======
	kvfree(umem->pgs);
>>>>>>> upstream/android-13
	umem->pgs = NULL;
	return err;
}

static int xdp_umem_account_pages(struct xdp_umem *umem)
{
	unsigned long lock_limit, new_npgs, old_npgs;

	if (capable(CAP_IPC_LOCK))
		return 0;

	lock_limit = rlimit(RLIMIT_MEMLOCK) >> PAGE_SHIFT;
	umem->user = get_uid(current_user());

	do {
		old_npgs = atomic_long_read(&umem->user->locked_vm);
		new_npgs = old_npgs + umem->npgs;
		if (new_npgs > lock_limit) {
			free_uid(umem->user);
			umem->user = NULL;
			return -ENOBUFS;
		}
	} while (atomic_long_cmpxchg(&umem->user->locked_vm, old_npgs,
				     new_npgs) != old_npgs);
	return 0;
}

static int xdp_umem_reg(struct xdp_umem *umem, struct xdp_umem_reg *mr)
{
<<<<<<< HEAD
	u32 chunk_size = mr->chunk_size, headroom = mr->headroom;
	u64 npgs, addr = mr->addr, size = mr->len;
	unsigned int chunks, chunks_per_page;
	int err, i;
=======
	u32 npgs_rem, chunk_size = mr->chunk_size, headroom = mr->headroom;
	bool unaligned_chunks = mr->flags & XDP_UMEM_UNALIGNED_CHUNK_FLAG;
	u64 npgs, addr = mr->addr, size = mr->len;
	unsigned int chunks, chunks_rem;
	int err;
>>>>>>> upstream/android-13

	if (chunk_size < XDP_UMEM_MIN_CHUNK_SIZE || chunk_size > PAGE_SIZE) {
		/* Strictly speaking we could support this, if:
		 * - huge pages, or*
		 * - using an IOMMU, or
		 * - making sure the memory area is consecutive
		 * but for now, we simply say "computer says no".
		 */
		return -EINVAL;
	}

<<<<<<< HEAD
	if (!is_power_of_2(chunk_size))
=======
	if (mr->flags & ~XDP_UMEM_UNALIGNED_CHUNK_FLAG)
		return -EINVAL;

	if (!unaligned_chunks && !is_power_of_2(chunk_size))
>>>>>>> upstream/android-13
		return -EINVAL;

	if (!PAGE_ALIGNED(addr)) {
		/* Memory area has to be page size aligned. For
		 * simplicity, this might change.
		 */
		return -EINVAL;
	}

	if ((addr + size) < addr)
		return -EINVAL;

<<<<<<< HEAD
	npgs = div_u64(size, PAGE_SIZE);
	if (npgs > U32_MAX)
		return -EINVAL;

	chunks = (unsigned int)div_u64(size, chunk_size);
	if (chunks == 0)
		return -EINVAL;

	chunks_per_page = PAGE_SIZE / chunk_size;
	if (chunks < chunks_per_page || chunks % chunks_per_page)
		return -EINVAL;

	headroom = ALIGN(headroom, 64);

	if (headroom >= chunk_size - XDP_PACKET_HEADROOM)
		return -EINVAL;

	umem->address = (unsigned long)addr;
	umem->props.chunk_mask = ~((u64)chunk_size - 1);
	umem->props.size = size;
	umem->headroom = headroom;
	umem->chunk_size_nohr = chunk_size - headroom;
	umem->npgs = (u32)npgs;
	umem->pgs = NULL;
	umem->user = NULL;
	INIT_LIST_HEAD(&umem->xsk_list);
	spin_lock_init(&umem->xsk_list_lock);

=======
	npgs = div_u64_rem(size, PAGE_SIZE, &npgs_rem);
	if (npgs_rem)
		npgs++;
	if (npgs > U32_MAX)
		return -EINVAL;

	chunks = (unsigned int)div_u64_rem(size, chunk_size, &chunks_rem);
	if (chunks == 0)
		return -EINVAL;

	if (!unaligned_chunks && chunks_rem)
		return -EINVAL;

	if (headroom >= chunk_size - XDP_PACKET_HEADROOM)
		return -EINVAL;

	umem->size = size;
	umem->headroom = headroom;
	umem->chunk_size = chunk_size;
	umem->chunks = chunks;
	umem->npgs = (u32)npgs;
	umem->pgs = NULL;
	umem->user = NULL;
	umem->flags = mr->flags;

	INIT_LIST_HEAD(&umem->xsk_dma_list);
>>>>>>> upstream/android-13
	refcount_set(&umem->users, 1);

	err = xdp_umem_account_pages(umem);
	if (err)
		return err;

<<<<<<< HEAD
	err = xdp_umem_pin_pages(umem);
	if (err)
		goto out_account;

	umem->pages = kcalloc(umem->npgs, sizeof(*umem->pages), GFP_KERNEL);
	if (!umem->pages) {
		err = -ENOMEM;
		goto out_pin;
	}

	for (i = 0; i < umem->npgs; i++)
		umem->pages[i].addr = page_address(umem->pgs[i]);

	return 0;

out_pin:
=======
	err = xdp_umem_pin_pages(umem, (unsigned long)addr);
	if (err)
		goto out_account;

	err = xdp_umem_addr_map(umem, umem->pgs, umem->npgs);
	if (err)
		goto out_unpin;

	return 0;

out_unpin:
>>>>>>> upstream/android-13
	xdp_umem_unpin_pages(umem);
out_account:
	xdp_umem_unaccount_pages(umem);
	return err;
}

struct xdp_umem *xdp_umem_create(struct xdp_umem_reg *mr)
{
	struct xdp_umem *umem;
	int err;

	umem = kzalloc(sizeof(*umem), GFP_KERNEL);
	if (!umem)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	err = xdp_umem_reg(umem, mr);
	if (err) {
=======
	err = ida_simple_get(&umem_ida, 0, 0, GFP_KERNEL);
	if (err < 0) {
		kfree(umem);
		return ERR_PTR(err);
	}
	umem->id = err;

	err = xdp_umem_reg(umem, mr);
	if (err) {
		ida_simple_remove(&umem_ida, umem->id);
>>>>>>> upstream/android-13
		kfree(umem);
		return ERR_PTR(err);
	}

	return umem;
}
<<<<<<< HEAD

bool xdp_umem_validate_queues(struct xdp_umem *umem)
{
	return umem->fq && umem->cq;
}
=======
>>>>>>> upstream/android-13
