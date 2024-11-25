<<<<<<< HEAD
/* bpf/cpumap.c
 *
 * Copyright (c) 2017 Jesper Dangaard Brouer, Red Hat Inc.
 * Released under terms in GPL version 2.  See COPYING.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* bpf/cpumap.c
 *
 * Copyright (c) 2017 Jesper Dangaard Brouer, Red Hat Inc.
>>>>>>> upstream/android-13
 */

/* The 'cpumap' is primarily used as a backend map for XDP BPF helper
 * call bpf_redirect_map() and XDP_REDIRECT action, like 'devmap'.
 *
 * Unlike devmap which redirects XDP frames out another NIC device,
 * this map type redirects raw XDP frames to another CPU.  The remote
 * CPU will do SKB-allocation and call the normal network stack.
 *
 * This is a scalability and isolation mechanism, that allow
 * separating the early driver network XDP layer, from the rest of the
 * netstack, and assigning dedicated CPUs for this stage.  This
 * basically allows for 10G wirespeed pre-filtering via bpf.
 */
<<<<<<< HEAD
=======
#include <linux/bitops.h>
>>>>>>> upstream/android-13
#include <linux/bpf.h>
#include <linux/filter.h>
#include <linux/ptr_ring.h>
#include <net/xdp.h>

#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/capability.h>
#include <trace/events/xdp.h>

<<<<<<< HEAD
#include <linux/netdevice.h>   /* netif_receive_skb_core */
=======
#include <linux/netdevice.h>   /* netif_receive_skb_list */
>>>>>>> upstream/android-13
#include <linux/etherdevice.h> /* eth_type_trans */

/* General idea: XDP packets getting XDP redirected to another CPU,
 * will maximum be stored/queued for one driver ->poll() call.  It is
<<<<<<< HEAD
 * guaranteed that setting flush bit and flush operation happen on
=======
 * guaranteed that queueing the frame and the flush operation happen on
>>>>>>> upstream/android-13
 * same CPU.  Thus, cpu_map_flush operation can deduct via this_cpu_ptr()
 * which queue in bpf_cpu_map_entry contains packets.
 */

#define CPU_MAP_BULK_SIZE 8  /* 8 == one cacheline on 64-bit archs */
<<<<<<< HEAD
struct xdp_bulk_queue {
	void *q[CPU_MAP_BULK_SIZE];
=======
struct bpf_cpu_map_entry;
struct bpf_cpu_map;

struct xdp_bulk_queue {
	void *q[CPU_MAP_BULK_SIZE];
	struct list_head flush_node;
	struct bpf_cpu_map_entry *obj;
>>>>>>> upstream/android-13
	unsigned int count;
};

/* Struct for every remote "destination" CPU in map */
struct bpf_cpu_map_entry {
	u32 cpu;    /* kthread CPU and map index */
	int map_id; /* Back reference to map */
<<<<<<< HEAD
	u32 qsize;  /* Queue size placeholder for map lookup */
=======
>>>>>>> upstream/android-13

	/* XDP can run multiple RX-ring queues, need __percpu enqueue store */
	struct xdp_bulk_queue __percpu *bulkq;

<<<<<<< HEAD
	/* Queue with potential multi-producers, and single-consumer kthread */
	struct ptr_ring *queue;
	struct task_struct *kthread;
	struct work_struct kthread_stop_wq;

	atomic_t refcnt; /* Control when this struct can be free'ed */
	struct rcu_head rcu;
=======
	struct bpf_cpu_map *cmap;

	/* Queue with potential multi-producers, and single-consumer kthread */
	struct ptr_ring *queue;
	struct task_struct *kthread;

	struct bpf_cpumap_val value;
	struct bpf_prog *prog;

	atomic_t refcnt; /* Control when this struct can be free'ed */
	struct rcu_head rcu;

	struct work_struct kthread_stop_wq;
>>>>>>> upstream/android-13
};

struct bpf_cpu_map {
	struct bpf_map map;
	/* Below members specific for map type */
<<<<<<< HEAD
	struct bpf_cpu_map_entry **cpu_map;
	unsigned long __percpu *flush_needed;
};

static int bq_flush_to_queue(struct bpf_cpu_map_entry *rcpu,
			     struct xdp_bulk_queue *bq, bool in_napi_ctx);

static u64 cpu_map_bitmap_size(const union bpf_attr *attr)
{
	return BITS_TO_LONGS(attr->max_entries) * sizeof(unsigned long);
}

static struct bpf_map *cpu_map_alloc(union bpf_attr *attr)
{
	struct bpf_cpu_map *cmap;
	int err = -ENOMEM;
	u64 cost;
	int ret;

	if (!capable(CAP_SYS_ADMIN))
=======
	struct bpf_cpu_map_entry __rcu **cpu_map;
};

static DEFINE_PER_CPU(struct list_head, cpu_map_flush_list);

static struct bpf_map *cpu_map_alloc(union bpf_attr *attr)
{
	u32 value_size = attr->value_size;
	struct bpf_cpu_map *cmap;
	int err = -ENOMEM;

	if (!bpf_capable())
>>>>>>> upstream/android-13
		return ERR_PTR(-EPERM);

	/* check sanity of attributes */
	if (attr->max_entries == 0 || attr->key_size != 4 ||
<<<<<<< HEAD
	    attr->value_size != 4 || attr->map_flags & ~BPF_F_NUMA_NODE)
		return ERR_PTR(-EINVAL);

	cmap = kzalloc(sizeof(*cmap), GFP_USER);
=======
	    (value_size != offsetofend(struct bpf_cpumap_val, qsize) &&
	     value_size != offsetofend(struct bpf_cpumap_val, bpf_prog.fd)) ||
	    attr->map_flags & ~BPF_F_NUMA_NODE)
		return ERR_PTR(-EINVAL);

	cmap = kzalloc(sizeof(*cmap), GFP_USER | __GFP_ACCOUNT);
>>>>>>> upstream/android-13
	if (!cmap)
		return ERR_PTR(-ENOMEM);

	bpf_map_init_from_attr(&cmap->map, attr);

	/* Pre-limit array size based on NR_CPUS, not final CPU check */
	if (cmap->map.max_entries > NR_CPUS) {
		err = -E2BIG;
		goto free_cmap;
	}

<<<<<<< HEAD
	/* make sure page count doesn't overflow */
	cost = (u64) cmap->map.max_entries * sizeof(struct bpf_cpu_map_entry *);
	cost += cpu_map_bitmap_size(attr) * num_possible_cpus();
	if (cost >= U32_MAX - PAGE_SIZE)
		goto free_cmap;
	cmap->map.pages = round_up(cost, PAGE_SIZE) >> PAGE_SHIFT;

	/* Notice returns -EPERM on if map size is larger than memlock limit */
	ret = bpf_map_precharge_memlock(cmap->map.pages);
	if (ret) {
		err = ret;
		goto free_cmap;
	}

	/* A per cpu bitfield with a bit per possible CPU in map  */
	cmap->flush_needed = __alloc_percpu(cpu_map_bitmap_size(attr),
					    __alignof__(unsigned long));
	if (!cmap->flush_needed)
		goto free_cmap;

=======
>>>>>>> upstream/android-13
	/* Alloc array for possible remote "destination" CPUs */
	cmap->cpu_map = bpf_map_area_alloc(cmap->map.max_entries *
					   sizeof(struct bpf_cpu_map_entry *),
					   cmap->map.numa_node);
	if (!cmap->cpu_map)
<<<<<<< HEAD
		goto free_percpu;

	return &cmap->map;
free_percpu:
	free_percpu(cmap->flush_needed);
=======
		goto free_cmap;

	return &cmap->map;
>>>>>>> upstream/android-13
free_cmap:
	kfree(cmap);
	return ERR_PTR(err);
}

static void get_cpu_map_entry(struct bpf_cpu_map_entry *rcpu)
{
	atomic_inc(&rcpu->refcnt);
}

/* called from workqueue, to workaround syscall using preempt_disable */
static void cpu_map_kthread_stop(struct work_struct *work)
{
	struct bpf_cpu_map_entry *rcpu;

	rcpu = container_of(work, struct bpf_cpu_map_entry, kthread_stop_wq);

	/* Wait for flush in __cpu_map_entry_free(), via full RCU barrier,
	 * as it waits until all in-flight call_rcu() callbacks complete.
	 */
	rcu_barrier();

	/* kthread_stop will wake_up_process and wait for it to complete */
	kthread_stop(rcpu->kthread);
}

<<<<<<< HEAD
static struct sk_buff *cpu_map_build_skb(struct bpf_cpu_map_entry *rcpu,
					 struct xdp_frame *xdpf)
{
	unsigned int hard_start_headroom;
	unsigned int frame_size;
	void *pkt_data_start;
	struct sk_buff *skb;

	/* Part of headroom was reserved to xdpf */
	hard_start_headroom = sizeof(struct xdp_frame) +  xdpf->headroom;

	/* build_skb need to place skb_shared_info after SKB end, and
	 * also want to know the memory "truesize".  Thus, need to
	 * know the memory frame size backing xdp_buff.
	 *
	 * XDP was designed to have PAGE_SIZE frames, but this
	 * assumption is not longer true with ixgbe and i40e.  It
	 * would be preferred to set frame_size to 2048 or 4096
	 * depending on the driver.
	 *   frame_size = 2048;
	 *   frame_len  = frame_size - sizeof(*xdp_frame);
	 *
	 * Instead, with info avail, skb_shared_info in placed after
	 * packet len.  This, unfortunately fakes the truesize.
	 * Another disadvantage of this approach, the skb_shared_info
	 * is not at a fixed memory location, with mixed length
	 * packets, which is bad for cache-line hotness.
	 */
	frame_size = SKB_DATA_ALIGN(xdpf->len + hard_start_headroom) +
		SKB_DATA_ALIGN(sizeof(struct skb_shared_info));

	pkt_data_start = xdpf->data - hard_start_headroom;
	skb = build_skb(pkt_data_start, frame_size);
	if (!skb)
		return NULL;

	skb_reserve(skb, hard_start_headroom);
	__skb_put(skb, xdpf->len);
	if (xdpf->metasize)
		skb_metadata_set(skb, xdpf->metasize);

	/* Essential SKB info: protocol and skb->dev */
	skb->protocol = eth_type_trans(skb, xdpf->dev_rx);

	/* Optional SKB info, currently missing:
	 * - HW checksum info		(skb->ip_summed)
	 * - HW RX hash			(skb_set_hash)
	 * - RX ring dev queue index	(skb_record_rx_queue)
	 */

	/* Allow SKB to reuse area used by xdp_frame */
	xdp_scrub_frame(xdpf);

	return skb;
}

=======
>>>>>>> upstream/android-13
static void __cpu_map_ring_cleanup(struct ptr_ring *ring)
{
	/* The tear-down procedure should have made sure that queue is
	 * empty.  See __cpu_map_entry_replace() and work-queue
	 * invoked cpu_map_kthread_stop(). Catch any broken behaviour
	 * gracefully and warn once.
	 */
	struct xdp_frame *xdpf;

	while ((xdpf = ptr_ring_consume(ring)))
		if (WARN_ON_ONCE(xdpf))
			xdp_return_frame(xdpf);
}

static void put_cpu_map_entry(struct bpf_cpu_map_entry *rcpu)
{
	if (atomic_dec_and_test(&rcpu->refcnt)) {
<<<<<<< HEAD
=======
		if (rcpu->prog)
			bpf_prog_put(rcpu->prog);
>>>>>>> upstream/android-13
		/* The queue should be empty at this point */
		__cpu_map_ring_cleanup(rcpu->queue);
		ptr_ring_cleanup(rcpu->queue, NULL);
		kfree(rcpu->queue);
		kfree(rcpu);
	}
}

<<<<<<< HEAD
=======
static void cpu_map_bpf_prog_run_skb(struct bpf_cpu_map_entry *rcpu,
				     struct list_head *listp,
				     struct xdp_cpumap_stats *stats)
{
	struct sk_buff *skb, *tmp;
	struct xdp_buff xdp;
	u32 act;
	int err;

	list_for_each_entry_safe(skb, tmp, listp, list) {
		act = bpf_prog_run_generic_xdp(skb, &xdp, rcpu->prog);
		switch (act) {
		case XDP_PASS:
			break;
		case XDP_REDIRECT:
			skb_list_del_init(skb);
			err = xdp_do_generic_redirect(skb->dev, skb, &xdp,
						      rcpu->prog);
			if (unlikely(err)) {
				kfree_skb(skb);
				stats->drop++;
			} else {
				stats->redirect++;
			}
			return;
		default:
			bpf_warn_invalid_xdp_action(act);
			fallthrough;
		case XDP_ABORTED:
			trace_xdp_exception(skb->dev, rcpu->prog, act);
			fallthrough;
		case XDP_DROP:
			skb_list_del_init(skb);
			kfree_skb(skb);
			stats->drop++;
			return;
		}
	}
}

static int cpu_map_bpf_prog_run_xdp(struct bpf_cpu_map_entry *rcpu,
				    void **frames, int n,
				    struct xdp_cpumap_stats *stats)
{
	struct xdp_rxq_info rxq;
	struct xdp_buff xdp;
	int i, nframes = 0;

	xdp_set_return_frame_no_direct();
	xdp.rxq = &rxq;

	for (i = 0; i < n; i++) {
		struct xdp_frame *xdpf = frames[i];
		u32 act;
		int err;

		rxq.dev = xdpf->dev_rx;
		rxq.mem = xdpf->mem;
		/* TODO: report queue_index to xdp_rxq_info */

		xdp_convert_frame_to_buff(xdpf, &xdp);

		act = bpf_prog_run_xdp(rcpu->prog, &xdp);
		switch (act) {
		case XDP_PASS:
			err = xdp_update_frame_from_buff(&xdp, xdpf);
			if (err < 0) {
				xdp_return_frame(xdpf);
				stats->drop++;
			} else {
				frames[nframes++] = xdpf;
				stats->pass++;
			}
			break;
		case XDP_REDIRECT:
			err = xdp_do_redirect(xdpf->dev_rx, &xdp,
					      rcpu->prog);
			if (unlikely(err)) {
				xdp_return_frame(xdpf);
				stats->drop++;
			} else {
				stats->redirect++;
			}
			break;
		default:
			bpf_warn_invalid_xdp_action(act);
			fallthrough;
		case XDP_DROP:
			xdp_return_frame(xdpf);
			stats->drop++;
			break;
		}
	}

	xdp_clear_return_frame_no_direct();

	return nframes;
}

#define CPUMAP_BATCH 8

static int cpu_map_bpf_prog_run(struct bpf_cpu_map_entry *rcpu, void **frames,
				int xdp_n, struct xdp_cpumap_stats *stats,
				struct list_head *list)
{
	int nframes;

	if (!rcpu->prog)
		return xdp_n;

	rcu_read_lock_bh();

	nframes = cpu_map_bpf_prog_run_xdp(rcpu, frames, xdp_n, stats);

	if (stats->redirect)
		xdp_do_flush();

	if (unlikely(!list_empty(list)))
		cpu_map_bpf_prog_run_skb(rcpu, list, stats);

	rcu_read_unlock_bh(); /* resched point, may call do_softirq() */

	return nframes;
}


>>>>>>> upstream/android-13
static int cpu_map_kthread_run(void *data)
{
	struct bpf_cpu_map_entry *rcpu = data;

	set_current_state(TASK_INTERRUPTIBLE);

	/* When kthread gives stop order, then rcpu have been disconnected
	 * from map, thus no new packets can enter. Remaining in-flight
	 * per CPU stored packets are flushed to this queue.  Wait honoring
	 * kthread_stop signal until queue is empty.
	 */
	while (!kthread_should_stop() || !__ptr_ring_empty(rcpu->queue)) {
<<<<<<< HEAD
		unsigned int processed = 0, drops = 0, sched = 0;
		struct xdp_frame *xdpf;
=======
		struct xdp_cpumap_stats stats = {}; /* zero stats */
		unsigned int kmem_alloc_drops = 0, sched = 0;
		gfp_t gfp = __GFP_ZERO | GFP_ATOMIC;
		int i, n, m, nframes, xdp_n;
		void *frames[CPUMAP_BATCH];
		void *skbs[CPUMAP_BATCH];
		LIST_HEAD(list);
>>>>>>> upstream/android-13

		/* Release CPU reschedule checks */
		if (__ptr_ring_empty(rcpu->queue)) {
			set_current_state(TASK_INTERRUPTIBLE);
			/* Recheck to avoid lost wake-up */
			if (__ptr_ring_empty(rcpu->queue)) {
				schedule();
				sched = 1;
			} else {
				__set_current_state(TASK_RUNNING);
			}
		} else {
			sched = cond_resched();
		}

<<<<<<< HEAD
		/* Process packets in rcpu->queue */
		local_bh_disable();
=======
>>>>>>> upstream/android-13
		/*
		 * The bpf_cpu_map_entry is single consumer, with this
		 * kthread CPU pinned. Lockless access to ptr_ring
		 * consume side valid as no-resize allowed of queue.
		 */
<<<<<<< HEAD
		while ((xdpf = __ptr_ring_consume(rcpu->queue))) {
			struct sk_buff *skb;
			int ret;

			skb = cpu_map_build_skb(rcpu, xdpf);
=======
		n = __ptr_ring_consume_batched(rcpu->queue, frames,
					       CPUMAP_BATCH);
		for (i = 0, xdp_n = 0; i < n; i++) {
			void *f = frames[i];
			struct page *page;

			if (unlikely(__ptr_test_bit(0, &f))) {
				struct sk_buff *skb = f;

				__ptr_clear_bit(0, &skb);
				list_add_tail(&skb->list, &list);
				continue;
			}

			frames[xdp_n++] = f;
			page = virt_to_page(f);

			/* Bring struct page memory area to curr CPU. Read by
			 * build_skb_around via page_is_pfmemalloc(), and when
			 * freed written by page_frag_free call.
			 */
			prefetchw(page);
		}

		/* Support running another XDP prog on this CPU */
		nframes = cpu_map_bpf_prog_run(rcpu, frames, xdp_n, &stats, &list);
		if (nframes) {
			m = kmem_cache_alloc_bulk(skbuff_head_cache, gfp, nframes, skbs);
			if (unlikely(m == 0)) {
				for (i = 0; i < nframes; i++)
					skbs[i] = NULL; /* effect: xdp_return_frame */
				kmem_alloc_drops += nframes;
			}
		}

		local_bh_disable();
		for (i = 0; i < nframes; i++) {
			struct xdp_frame *xdpf = frames[i];
			struct sk_buff *skb = skbs[i];

			skb = __xdp_build_skb_from_frame(xdpf, skb,
							 xdpf->dev_rx);
>>>>>>> upstream/android-13
			if (!skb) {
				xdp_return_frame(xdpf);
				continue;
			}

<<<<<<< HEAD
			/* Inject into network stack */
			ret = netif_receive_skb_core(skb);
			if (ret == NET_RX_DROP)
				drops++;

			/* Limit BH-disable period */
			if (++processed == 8)
				break;
		}
		/* Feedback loop via tracepoint */
		trace_xdp_cpumap_kthread(rcpu->map_id, processed, drops, sched);
=======
			list_add_tail(&skb->list, &list);
		}
		netif_receive_skb_list(&list);

		/* Feedback loop via tracepoint */
		trace_xdp_cpumap_kthread(rcpu->map_id, n, kmem_alloc_drops,
					 sched, &stats);
>>>>>>> upstream/android-13

		local_bh_enable(); /* resched point, may call do_softirq() */
	}
	__set_current_state(TASK_RUNNING);

	put_cpu_map_entry(rcpu);
	return 0;
}

<<<<<<< HEAD
static struct bpf_cpu_map_entry *__cpu_map_entry_alloc(u32 qsize, u32 cpu,
						       int map_id)
{
	gfp_t gfp = GFP_KERNEL | __GFP_NOWARN;
	struct bpf_cpu_map_entry *rcpu;
	int numa, err;
=======
static int __cpu_map_load_bpf_program(struct bpf_cpu_map_entry *rcpu, int fd)
{
	struct bpf_prog *prog;

	prog = bpf_prog_get_type(fd, BPF_PROG_TYPE_XDP);
	if (IS_ERR(prog))
		return PTR_ERR(prog);

	if (prog->expected_attach_type != BPF_XDP_CPUMAP) {
		bpf_prog_put(prog);
		return -EINVAL;
	}

	rcpu->value.bpf_prog.id = prog->aux->id;
	rcpu->prog = prog;

	return 0;
}

static struct bpf_cpu_map_entry *
__cpu_map_entry_alloc(struct bpf_map *map, struct bpf_cpumap_val *value,
		      u32 cpu)
{
	int numa, err, i, fd = value->bpf_prog.fd;
	gfp_t gfp = GFP_KERNEL | __GFP_NOWARN;
	struct bpf_cpu_map_entry *rcpu;
	struct xdp_bulk_queue *bq;
>>>>>>> upstream/android-13

	/* Have map->numa_node, but choose node of redirect target CPU */
	numa = cpu_to_node(cpu);

<<<<<<< HEAD
	rcpu = kzalloc_node(sizeof(*rcpu), gfp, numa);
=======
	rcpu = bpf_map_kmalloc_node(map, sizeof(*rcpu), gfp | __GFP_ZERO, numa);
>>>>>>> upstream/android-13
	if (!rcpu)
		return NULL;

	/* Alloc percpu bulkq */
<<<<<<< HEAD
	rcpu->bulkq = __alloc_percpu_gfp(sizeof(*rcpu->bulkq),
					 sizeof(void *), gfp);
	if (!rcpu->bulkq)
		goto free_rcu;

	/* Alloc queue */
	rcpu->queue = kzalloc_node(sizeof(*rcpu->queue), gfp, numa);
	if (!rcpu->queue)
		goto free_bulkq;

	err = ptr_ring_init(rcpu->queue, qsize, gfp);
=======
	rcpu->bulkq = bpf_map_alloc_percpu(map, sizeof(*rcpu->bulkq),
					   sizeof(void *), gfp);
	if (!rcpu->bulkq)
		goto free_rcu;

	for_each_possible_cpu(i) {
		bq = per_cpu_ptr(rcpu->bulkq, i);
		bq->obj = rcpu;
	}

	/* Alloc queue */
	rcpu->queue = bpf_map_kmalloc_node(map, sizeof(*rcpu->queue), gfp,
					   numa);
	if (!rcpu->queue)
		goto free_bulkq;

	err = ptr_ring_init(rcpu->queue, value->qsize, gfp);
>>>>>>> upstream/android-13
	if (err)
		goto free_queue;

	rcpu->cpu    = cpu;
<<<<<<< HEAD
	rcpu->map_id = map_id;
	rcpu->qsize  = qsize;

	/* Setup kthread */
	rcpu->kthread = kthread_create_on_node(cpu_map_kthread_run, rcpu, numa,
					       "cpumap/%d/map:%d", cpu, map_id);
	if (IS_ERR(rcpu->kthread))
		goto free_ptr_ring;
=======
	rcpu->map_id = map->id;
	rcpu->value.qsize  = value->qsize;

	if (fd > 0 && __cpu_map_load_bpf_program(rcpu, fd))
		goto free_ptr_ring;

	/* Setup kthread */
	rcpu->kthread = kthread_create_on_node(cpu_map_kthread_run, rcpu, numa,
					       "cpumap/%d/map:%d", cpu,
					       map->id);
	if (IS_ERR(rcpu->kthread))
		goto free_prog;
>>>>>>> upstream/android-13

	get_cpu_map_entry(rcpu); /* 1-refcnt for being in cmap->cpu_map[] */
	get_cpu_map_entry(rcpu); /* 1-refcnt for kthread */

	/* Make sure kthread runs on a single CPU */
	kthread_bind(rcpu->kthread, cpu);
	wake_up_process(rcpu->kthread);

	return rcpu;

<<<<<<< HEAD
=======
free_prog:
	if (rcpu->prog)
		bpf_prog_put(rcpu->prog);
>>>>>>> upstream/android-13
free_ptr_ring:
	ptr_ring_cleanup(rcpu->queue, NULL);
free_queue:
	kfree(rcpu->queue);
free_bulkq:
	free_percpu(rcpu->bulkq);
free_rcu:
	kfree(rcpu);
	return NULL;
}

static void __cpu_map_entry_free(struct rcu_head *rcu)
{
	struct bpf_cpu_map_entry *rcpu;
<<<<<<< HEAD
	int cpu;

	/* This cpu_map_entry have been disconnected from map and one
	 * RCU graze-period have elapsed.  Thus, XDP cannot queue any
=======

	/* This cpu_map_entry have been disconnected from map and one
	 * RCU grace-period have elapsed.  Thus, XDP cannot queue any
>>>>>>> upstream/android-13
	 * new packets and cannot change/set flush_needed that can
	 * find this entry.
	 */
	rcpu = container_of(rcu, struct bpf_cpu_map_entry, rcu);

<<<<<<< HEAD
	/* Flush remaining packets in percpu bulkq */
	for_each_online_cpu(cpu) {
		struct xdp_bulk_queue *bq = per_cpu_ptr(rcpu->bulkq, cpu);

		/* No concurrent bq_enqueue can run at this point */
		bq_flush_to_queue(rcpu, bq, false);
	}
=======
>>>>>>> upstream/android-13
	free_percpu(rcpu->bulkq);
	/* Cannot kthread_stop() here, last put free rcpu resources */
	put_cpu_map_entry(rcpu);
}

/* After xchg pointer to bpf_cpu_map_entry, use the call_rcu() to
 * ensure any driver rcu critical sections have completed, but this
 * does not guarantee a flush has happened yet. Because driver side
 * rcu_read_lock/unlock only protects the running XDP program.  The
 * atomic xchg and NULL-ptr check in __cpu_map_flush() makes sure a
 * pending flush op doesn't fail.
 *
 * The bpf_cpu_map_entry is still used by the kthread, and there can
 * still be pending packets (in queue and percpu bulkq).  A refcnt
 * makes sure to last user (kthread_stop vs. call_rcu) free memory
 * resources.
 *
 * The rcu callback __cpu_map_entry_free flush remaining packets in
 * percpu bulkq to queue.  Due to caller map_delete_elem() disable
 * preemption, cannot call kthread_stop() to make sure queue is empty.
 * Instead a work_queue is started for stopping kthread,
<<<<<<< HEAD
 * cpu_map_kthread_stop, which waits for an RCU graze period before
=======
 * cpu_map_kthread_stop, which waits for an RCU grace period before
>>>>>>> upstream/android-13
 * stopping kthread, emptying the queue.
 */
static void __cpu_map_entry_replace(struct bpf_cpu_map *cmap,
				    u32 key_cpu, struct bpf_cpu_map_entry *rcpu)
{
	struct bpf_cpu_map_entry *old_rcpu;

<<<<<<< HEAD
	old_rcpu = xchg(&cmap->cpu_map[key_cpu], rcpu);
=======
	old_rcpu = unrcu_pointer(xchg(&cmap->cpu_map[key_cpu], RCU_INITIALIZER(rcpu)));
>>>>>>> upstream/android-13
	if (old_rcpu) {
		call_rcu(&old_rcpu->rcu, __cpu_map_entry_free);
		INIT_WORK(&old_rcpu->kthread_stop_wq, cpu_map_kthread_stop);
		schedule_work(&old_rcpu->kthread_stop_wq);
	}
}

static int cpu_map_delete_elem(struct bpf_map *map, void *key)
{
	struct bpf_cpu_map *cmap = container_of(map, struct bpf_cpu_map, map);
	u32 key_cpu = *(u32 *)key;

	if (key_cpu >= map->max_entries)
		return -EINVAL;

	/* notice caller map_delete_elem() use preempt_disable() */
	__cpu_map_entry_replace(cmap, key_cpu, NULL);
	return 0;
}

static int cpu_map_update_elem(struct bpf_map *map, void *key, void *value,
			       u64 map_flags)
{
	struct bpf_cpu_map *cmap = container_of(map, struct bpf_cpu_map, map);
<<<<<<< HEAD
	struct bpf_cpu_map_entry *rcpu;

	/* Array index key correspond to CPU number */
	u32 key_cpu = *(u32 *)key;
	/* Value is the queue size */
	u32 qsize = *(u32 *)value;
=======
	struct bpf_cpumap_val cpumap_value = {};
	struct bpf_cpu_map_entry *rcpu;
	/* Array index key correspond to CPU number */
	u32 key_cpu = *(u32 *)key;

	memcpy(&cpumap_value, value, map->value_size);
>>>>>>> upstream/android-13

	if (unlikely(map_flags > BPF_EXIST))
		return -EINVAL;
	if (unlikely(key_cpu >= cmap->map.max_entries))
		return -E2BIG;
	if (unlikely(map_flags == BPF_NOEXIST))
		return -EEXIST;
<<<<<<< HEAD
	if (unlikely(qsize > 16384)) /* sanity limit on qsize */
=======
	if (unlikely(cpumap_value.qsize > 16384)) /* sanity limit on qsize */
>>>>>>> upstream/android-13
		return -EOVERFLOW;

	/* Make sure CPU is a valid possible cpu */
	if (key_cpu >= nr_cpumask_bits || !cpu_possible(key_cpu))
		return -ENODEV;

<<<<<<< HEAD
	if (qsize == 0) {
		rcpu = NULL; /* Same as deleting */
	} else {
		/* Updating qsize cause re-allocation of bpf_cpu_map_entry */
		rcpu = __cpu_map_entry_alloc(qsize, key_cpu, map->id);
		if (!rcpu)
			return -ENOMEM;
=======
	if (cpumap_value.qsize == 0) {
		rcpu = NULL; /* Same as deleting */
	} else {
		/* Updating qsize cause re-allocation of bpf_cpu_map_entry */
		rcpu = __cpu_map_entry_alloc(map, &cpumap_value, key_cpu);
		if (!rcpu)
			return -ENOMEM;
		rcpu->cmap = cmap;
>>>>>>> upstream/android-13
	}
	rcu_read_lock();
	__cpu_map_entry_replace(cmap, key_cpu, rcpu);
	rcu_read_unlock();
	return 0;
}

static void cpu_map_free(struct bpf_map *map)
{
	struct bpf_cpu_map *cmap = container_of(map, struct bpf_cpu_map, map);
<<<<<<< HEAD
	int cpu;
=======
>>>>>>> upstream/android-13
	u32 i;

	/* At this point bpf_prog->aux->refcnt == 0 and this map->refcnt == 0,
	 * so the bpf programs (can be more than one that used this map) were
	 * disconnected from events. Wait for outstanding critical sections in
	 * these programs to complete. The rcu critical section only guarantees
	 * no further "XDP/bpf-side" reads against bpf_cpu_map->cpu_map.
	 * It does __not__ ensure pending flush operations (if any) are
	 * complete.
	 */

<<<<<<< HEAD
	bpf_clear_redirect_map(map);
	synchronize_rcu();

	/* To ensure all pending flush operations have completed wait for flush
	 * bitmap to indicate all flush_needed bits to be zero on _all_ cpus.
	 * Because the above synchronize_rcu() ensures the map is disconnected
	 * from the program we can assume no new bits will be set.
	 */
	for_each_online_cpu(cpu) {
		unsigned long *bitmap = per_cpu_ptr(cmap->flush_needed, cpu);

		while (!bitmap_empty(bitmap, cmap->map.max_entries))
			cond_resched();
	}

=======
	synchronize_rcu();

>>>>>>> upstream/android-13
	/* For cpu_map the remote CPUs can still be using the entries
	 * (struct bpf_cpu_map_entry).
	 */
	for (i = 0; i < cmap->map.max_entries; i++) {
		struct bpf_cpu_map_entry *rcpu;

<<<<<<< HEAD
		rcpu = READ_ONCE(cmap->cpu_map[i]);
		if (!rcpu)
			continue;

		/* bq flush and cleanup happens after RCU graze-period */
		__cpu_map_entry_replace(cmap, i, NULL); /* call_rcu */
	}
	free_percpu(cmap->flush_needed);
=======
		rcpu = rcu_dereference_raw(cmap->cpu_map[i]);
		if (!rcpu)
			continue;

		/* bq flush and cleanup happens after RCU grace-period */
		__cpu_map_entry_replace(cmap, i, NULL); /* call_rcu */
	}
>>>>>>> upstream/android-13
	bpf_map_area_free(cmap->cpu_map);
	kfree(cmap);
}

<<<<<<< HEAD
struct bpf_cpu_map_entry *__cpu_map_lookup_elem(struct bpf_map *map, u32 key)
=======
/* Elements are kept alive by RCU; either by rcu_read_lock() (from syscall) or
 * by local_bh_disable() (from XDP calls inside NAPI). The
 * rcu_read_lock_bh_held() below makes lockdep accept both.
 */
static void *__cpu_map_lookup_elem(struct bpf_map *map, u32 key)
>>>>>>> upstream/android-13
{
	struct bpf_cpu_map *cmap = container_of(map, struct bpf_cpu_map, map);
	struct bpf_cpu_map_entry *rcpu;

	if (key >= map->max_entries)
		return NULL;

<<<<<<< HEAD
	rcpu = READ_ONCE(cmap->cpu_map[key]);
=======
	rcpu = rcu_dereference_check(cmap->cpu_map[key],
				     rcu_read_lock_bh_held());
>>>>>>> upstream/android-13
	return rcpu;
}

static void *cpu_map_lookup_elem(struct bpf_map *map, void *key)
{
	struct bpf_cpu_map_entry *rcpu =
		__cpu_map_lookup_elem(map, *(u32 *)key);

<<<<<<< HEAD
	return rcpu ? &rcpu->qsize : NULL;
=======
	return rcpu ? &rcpu->value : NULL;
>>>>>>> upstream/android-13
}

static int cpu_map_get_next_key(struct bpf_map *map, void *key, void *next_key)
{
	struct bpf_cpu_map *cmap = container_of(map, struct bpf_cpu_map, map);
	u32 index = key ? *(u32 *)key : U32_MAX;
	u32 *next = next_key;

	if (index >= cmap->map.max_entries) {
		*next = 0;
		return 0;
	}

	if (index == cmap->map.max_entries - 1)
		return -ENOENT;
	*next = index + 1;
	return 0;
}

<<<<<<< HEAD
const struct bpf_map_ops cpu_map_ops = {
=======
static int cpu_map_redirect(struct bpf_map *map, u32 ifindex, u64 flags)
{
	return __bpf_xdp_redirect_map(map, ifindex, flags, 0,
				      __cpu_map_lookup_elem);
}

static int cpu_map_btf_id;
const struct bpf_map_ops cpu_map_ops = {
	.map_meta_equal		= bpf_map_meta_equal,
>>>>>>> upstream/android-13
	.map_alloc		= cpu_map_alloc,
	.map_free		= cpu_map_free,
	.map_delete_elem	= cpu_map_delete_elem,
	.map_update_elem	= cpu_map_update_elem,
	.map_lookup_elem	= cpu_map_lookup_elem,
	.map_get_next_key	= cpu_map_get_next_key,
	.map_check_btf		= map_check_no_btf,
<<<<<<< HEAD
};

static int bq_flush_to_queue(struct bpf_cpu_map_entry *rcpu,
			     struct xdp_bulk_queue *bq, bool in_napi_ctx)
{
=======
	.map_btf_name		= "bpf_cpu_map",
	.map_btf_id		= &cpu_map_btf_id,
	.map_redirect		= cpu_map_redirect,
};

static void bq_flush_to_queue(struct xdp_bulk_queue *bq)
{
	struct bpf_cpu_map_entry *rcpu = bq->obj;
>>>>>>> upstream/android-13
	unsigned int processed = 0, drops = 0;
	const int to_cpu = rcpu->cpu;
	struct ptr_ring *q;
	int i;

	if (unlikely(!bq->count))
<<<<<<< HEAD
		return 0;
=======
		return;
>>>>>>> upstream/android-13

	q = rcpu->queue;
	spin_lock(&q->producer_lock);

	for (i = 0; i < bq->count; i++) {
		struct xdp_frame *xdpf = bq->q[i];
		int err;

		err = __ptr_ring_produce(q, xdpf);
		if (err) {
			drops++;
<<<<<<< HEAD
			if (likely(in_napi_ctx))
				xdp_return_frame_rx_napi(xdpf);
			else
				xdp_return_frame(xdpf);
=======
			xdp_return_frame_rx_napi(xdpf);
>>>>>>> upstream/android-13
		}
		processed++;
	}
	bq->count = 0;
	spin_unlock(&q->producer_lock);

<<<<<<< HEAD
	/* Feedback loop via tracepoints */
	trace_xdp_cpumap_enqueue(rcpu->map_id, processed, drops, to_cpu);
	return 0;
=======
	__list_del_clearprev(&bq->flush_node);

	/* Feedback loop via tracepoints */
	trace_xdp_cpumap_enqueue(rcpu->map_id, processed, drops, to_cpu);
>>>>>>> upstream/android-13
}

/* Runs under RCU-read-side, plus in softirq under NAPI protection.
 * Thus, safe percpu variable access.
 */
<<<<<<< HEAD
static int bq_enqueue(struct bpf_cpu_map_entry *rcpu, struct xdp_frame *xdpf)
{
	struct xdp_bulk_queue *bq = this_cpu_ptr(rcpu->bulkq);

	if (unlikely(bq->count == CPU_MAP_BULK_SIZE))
		bq_flush_to_queue(rcpu, bq, true);
=======
static void bq_enqueue(struct bpf_cpu_map_entry *rcpu, struct xdp_frame *xdpf)
{
	struct list_head *flush_list = this_cpu_ptr(&cpu_map_flush_list);
	struct xdp_bulk_queue *bq = this_cpu_ptr(rcpu->bulkq);

	if (unlikely(bq->count == CPU_MAP_BULK_SIZE))
		bq_flush_to_queue(bq);
>>>>>>> upstream/android-13

	/* Notice, xdp_buff/page MUST be queued here, long enough for
	 * driver to code invoking us to finished, due to driver
	 * (e.g. ixgbe) recycle tricks based on page-refcnt.
	 *
	 * Thus, incoming xdp_frame is always queued here (else we race
	 * with another CPU on page-refcnt and remaining driver code).
	 * Queue time is very short, as driver will invoke flush
	 * operation, when completing napi->poll call.
	 */
	bq->q[bq->count++] = xdpf;
<<<<<<< HEAD
	return 0;
=======

	if (!bq->flush_node.prev)
		list_add(&bq->flush_node, flush_list);
>>>>>>> upstream/android-13
}

int cpu_map_enqueue(struct bpf_cpu_map_entry *rcpu, struct xdp_buff *xdp,
		    struct net_device *dev_rx)
{
	struct xdp_frame *xdpf;

<<<<<<< HEAD
	xdpf = convert_to_xdp_frame(xdp);
=======
	xdpf = xdp_convert_buff_to_frame(xdp);
>>>>>>> upstream/android-13
	if (unlikely(!xdpf))
		return -EOVERFLOW;

	/* Info needed when constructing SKB on remote CPU */
	xdpf->dev_rx = dev_rx;

	bq_enqueue(rcpu, xdpf);
	return 0;
}

<<<<<<< HEAD
void __cpu_map_insert_ctx(struct bpf_map *map, u32 bit)
{
	struct bpf_cpu_map *cmap = container_of(map, struct bpf_cpu_map, map);
	unsigned long *bitmap = this_cpu_ptr(cmap->flush_needed);

	__set_bit(bit, bitmap);
}

void __cpu_map_flush(struct bpf_map *map)
{
	struct bpf_cpu_map *cmap = container_of(map, struct bpf_cpu_map, map);
	unsigned long *bitmap = this_cpu_ptr(cmap->flush_needed);
	u32 bit;

	/* The napi->poll softirq makes sure __cpu_map_insert_ctx()
	 * and __cpu_map_flush() happen on same CPU. Thus, the percpu
	 * bitmap indicate which percpu bulkq have packets.
	 */
	for_each_set_bit(bit, bitmap, map->max_entries) {
		struct bpf_cpu_map_entry *rcpu = READ_ONCE(cmap->cpu_map[bit]);
		struct xdp_bulk_queue *bq;

		/* This is possible if entry is removed by user space
		 * between xdp redirect and flush op.
		 */
		if (unlikely(!rcpu))
			continue;

		__clear_bit(bit, bitmap);

		/* Flush all frames in bulkq to real queue */
		bq = this_cpu_ptr(rcpu->bulkq);
		bq_flush_to_queue(rcpu, bq, true);

		/* If already running, costs spin_lock_irqsave + smb_mb */
		wake_up_process(rcpu->kthread);
	}
}
=======
int cpu_map_generic_redirect(struct bpf_cpu_map_entry *rcpu,
			     struct sk_buff *skb)
{
	int ret;

	__skb_pull(skb, skb->mac_len);
	skb_set_redirected(skb, false);
	__ptr_set_bit(0, &skb);

	ret = ptr_ring_produce(rcpu->queue, skb);
	if (ret < 0)
		goto trace;

	wake_up_process(rcpu->kthread);
trace:
	trace_xdp_cpumap_enqueue(rcpu->map_id, !ret, !!ret, rcpu->cpu);
	return ret;
}

void __cpu_map_flush(void)
{
	struct list_head *flush_list = this_cpu_ptr(&cpu_map_flush_list);
	struct xdp_bulk_queue *bq, *tmp;

	list_for_each_entry_safe(bq, tmp, flush_list, flush_node) {
		bq_flush_to_queue(bq);

		/* If already running, costs spin_lock_irqsave + smb_mb */
		wake_up_process(bq->obj->kthread);
	}
}

static int __init cpu_map_init(void)
{
	int cpu;

	for_each_possible_cpu(cpu)
		INIT_LIST_HEAD(&per_cpu(cpu_map_flush_list, cpu));
	return 0;
}

subsys_initcall(cpu_map_init);
>>>>>>> upstream/android-13
