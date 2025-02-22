<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Kernel-based Virtual Machine driver for Linux
 *
 * This module enables machines with Intel VT-x extensions to run virtual
 * machines without emulation or binary translation.
 *
 * Copyright (C) 2006 Qumranet, Inc.
 * Copyright 2010 Red Hat, Inc. and/or its affiliates.
 *
 * Authors:
 *   Avi Kivity   <avi@qumranet.com>
 *   Yaniv Kamay  <yaniv@qumranet.com>
<<<<<<< HEAD
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
=======
>>>>>>> upstream/android-13
 */

#include <kvm/iodev.h>

#include <linux/kvm_host.h>
#include <linux/kvm.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/percpu.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/vmalloc.h>
#include <linux/reboot.h>
#include <linux/debugfs.h>
#include <linux/highmem.h>
#include <linux/file.h>
#include <linux/syscore_ops.h>
#include <linux/cpu.h>
#include <linux/sched/signal.h>
#include <linux/sched/mm.h>
#include <linux/sched/stat.h>
#include <linux/cpumask.h>
#include <linux/smp.h>
#include <linux/anon_inodes.h>
#include <linux/profile.h>
#include <linux/kvm_para.h>
#include <linux/pagemap.h>
#include <linux/mman.h>
#include <linux/swap.h>
#include <linux/bitops.h>
#include <linux/spinlock.h>
#include <linux/compat.h>
#include <linux/srcu.h>
#include <linux/hugetlb.h>
#include <linux/slab.h>
#include <linux/sort.h>
#include <linux/bsearch.h>
<<<<<<< HEAD
#include <linux/kthread.h>
#include <linux/io.h>
=======
#include <linux/io.h>
#include <linux/lockdep.h>
#include <linux/kthread.h>
#include <linux/suspend.h>
>>>>>>> upstream/android-13

#include <asm/processor.h>
#include <asm/ioctl.h>
#include <linux/uaccess.h>
<<<<<<< HEAD
#include <asm/pgtable.h>

#include "coalesced_mmio.h"
#include "async_pf.h"
=======

#include "coalesced_mmio.h"
#include "async_pf.h"
#include "mmu_lock.h"
>>>>>>> upstream/android-13
#include "vfio.h"

#define CREATE_TRACE_POINTS
#include <trace/events/kvm.h>

<<<<<<< HEAD
=======
#include <linux/kvm_dirty_ring.h>

>>>>>>> upstream/android-13
/* Worst case buffer size needed for holding an integer. */
#define ITOA_MAX_LEN 12

MODULE_AUTHOR("Qumranet");
MODULE_LICENSE("GPL");

/* Architectures should define their poll value according to the halt latency */
unsigned int halt_poll_ns = KVM_HALT_POLL_NS_DEFAULT;
module_param(halt_poll_ns, uint, 0644);
EXPORT_SYMBOL_GPL(halt_poll_ns);

/* Default doubles per-vcpu halt_poll_ns. */
unsigned int halt_poll_ns_grow = 2;
module_param(halt_poll_ns_grow, uint, 0644);
EXPORT_SYMBOL_GPL(halt_poll_ns_grow);

<<<<<<< HEAD
=======
/* The start value to grow halt_poll_ns from */
unsigned int halt_poll_ns_grow_start = 10000; /* 10us */
module_param(halt_poll_ns_grow_start, uint, 0644);
EXPORT_SYMBOL_GPL(halt_poll_ns_grow_start);

>>>>>>> upstream/android-13
/* Default resets per-vcpu halt_poll_ns . */
unsigned int halt_poll_ns_shrink;
module_param(halt_poll_ns_shrink, uint, 0644);
EXPORT_SYMBOL_GPL(halt_poll_ns_shrink);

/*
 * Ordering of locks:
 *
 *	kvm->lock --> kvm->slots_lock --> kvm->irq_lock
 */

DEFINE_MUTEX(kvm_lock);
static DEFINE_RAW_SPINLOCK(kvm_count_lock);
LIST_HEAD(vm_list);

static cpumask_var_t cpus_hardware_enabled;
static int kvm_usage_count;
static atomic_t hardware_enable_failed;

<<<<<<< HEAD
struct kmem_cache *kvm_vcpu_cache;
EXPORT_SYMBOL_GPL(kvm_vcpu_cache);

static __read_mostly struct preempt_ops kvm_preempt_ops;
=======
static struct kmem_cache *kvm_vcpu_cache;

static __read_mostly struct preempt_ops kvm_preempt_ops;
static DEFINE_PER_CPU(struct kvm_vcpu *, kvm_running_vcpu);
>>>>>>> upstream/android-13

struct dentry *kvm_debugfs_dir;
EXPORT_SYMBOL_GPL(kvm_debugfs_dir);

<<<<<<< HEAD
static int kvm_debugfs_num_entries;
static const struct file_operations *stat_fops_per_vm[];
=======
static const struct file_operations stat_fops_per_vm;

static struct file_operations kvm_chardev_ops;
>>>>>>> upstream/android-13

static long kvm_vcpu_ioctl(struct file *file, unsigned int ioctl,
			   unsigned long arg);
#ifdef CONFIG_KVM_COMPAT
static long kvm_vcpu_compat_ioctl(struct file *file, unsigned int ioctl,
				  unsigned long arg);
#define KVM_COMPAT(c)	.compat_ioctl	= (c)
#else
<<<<<<< HEAD
static long kvm_no_compat_ioctl(struct file *file, unsigned int ioctl,
				unsigned long arg) { return -EINVAL; }
#define KVM_COMPAT(c)	.compat_ioctl	= kvm_no_compat_ioctl
=======
/*
 * For architectures that don't implement a compat infrastructure,
 * adopt a double line of defense:
 * - Prevent a compat task from opening /dev/kvm
 * - If the open has been done by a 64bit task, and the KVM fd
 *   passed to a compat task, let the ioctls fail.
 */
static long kvm_no_compat_ioctl(struct file *file, unsigned int ioctl,
				unsigned long arg) { return -EINVAL; }

static int kvm_no_compat_open(struct inode *inode, struct file *file)
{
	return is_compat_task() ? -ENODEV : 0;
}
#define KVM_COMPAT(c)	.compat_ioctl	= kvm_no_compat_ioctl,	\
			.open		= kvm_no_compat_open
>>>>>>> upstream/android-13
#endif
static int hardware_enable_all(void);
static void hardware_disable_all(void);

static void kvm_io_bus_destroy(struct kvm_io_bus *bus);

<<<<<<< HEAD
static void mark_page_dirty_in_slot(struct kvm_memory_slot *memslot, gfn_t gfn);

__visible bool kvm_rebooting;
EXPORT_SYMBOL_GPL(kvm_rebooting);

static bool largepages_enabled = true;

=======
__visible bool kvm_rebooting;
EXPORT_SYMBOL_GPL(kvm_rebooting);

>>>>>>> upstream/android-13
#define KVM_EVENT_CREATE_VM 0
#define KVM_EVENT_DESTROY_VM 1
static void kvm_uevent_notify_change(unsigned int type, struct kvm *kvm);
static unsigned long long kvm_createvm_count;
static unsigned long long kvm_active_vms;

__weak void kvm_arch_mmu_notifier_invalidate_range(struct kvm *kvm,
						   unsigned long start, unsigned long end)
{
}

bool kvm_is_zone_device_pfn(kvm_pfn_t pfn)
{
	/*
	 * The metadata used by is_zone_device_page() to determine whether or
	 * not a page is ZONE_DEVICE is guaranteed to be valid if and only if
	 * the device has been pinned, e.g. by get_user_pages().  WARN if the
	 * page_count() is zero to help detect bad usage of this helper.
	 */
	if (!pfn_valid(pfn) || WARN_ON_ONCE(!page_count(pfn_to_page(pfn))))
		return false;

	return is_zone_device_page(pfn_to_page(pfn));
}

bool kvm_is_reserved_pfn(kvm_pfn_t pfn)
{
	/*
	 * ZONE_DEVICE pages currently set PG_reserved, but from a refcounting
	 * perspective they are "normal" pages, albeit with slightly different
	 * usage rules.
	 */
	if (pfn_valid(pfn))
		return PageReserved(pfn_to_page(pfn)) &&
		       !is_zero_pfn(pfn) &&
		       !kvm_is_zone_device_pfn(pfn);

	return true;
}

/*
 * Switches to specified vcpu, until a matching vcpu_put()
 */
void vcpu_load(struct kvm_vcpu *vcpu)
{
	int cpu = get_cpu();
<<<<<<< HEAD
=======

	__this_cpu_write(kvm_running_vcpu, vcpu);
>>>>>>> upstream/android-13
	preempt_notifier_register(&vcpu->preempt_notifier);
	kvm_arch_vcpu_load(vcpu, cpu);
	put_cpu();
}
EXPORT_SYMBOL_GPL(vcpu_load);

void vcpu_put(struct kvm_vcpu *vcpu)
{
	preempt_disable();
	kvm_arch_vcpu_put(vcpu);
	preempt_notifier_unregister(&vcpu->preempt_notifier);
<<<<<<< HEAD
=======
	__this_cpu_write(kvm_running_vcpu, NULL);
>>>>>>> upstream/android-13
	preempt_enable();
}
EXPORT_SYMBOL_GPL(vcpu_put);

/* TODO: merge with kvm_arch_vcpu_should_kick */
static bool kvm_request_needs_ipi(struct kvm_vcpu *vcpu, unsigned req)
{
	int mode = kvm_vcpu_exiting_guest_mode(vcpu);

	/*
	 * We need to wait for the VCPU to reenable interrupts and get out of
	 * READING_SHADOW_PAGE_TABLES mode.
	 */
	if (req & KVM_REQUEST_WAIT)
		return mode != OUTSIDE_GUEST_MODE;

	/*
	 * Need to kick a running VCPU, but otherwise there is nothing to do.
	 */
	return mode == IN_GUEST_MODE;
}

static void ack_flush(void *_completed)
{
}

<<<<<<< HEAD
static inline bool kvm_kick_many_cpus(const struct cpumask *cpus, bool wait)
{
	if (unlikely(!cpus))
=======
static inline bool kvm_kick_many_cpus(cpumask_var_t tmp, bool wait)
{
	const struct cpumask *cpus;

	if (likely(cpumask_available(tmp)))
		cpus = tmp;
	else
>>>>>>> upstream/android-13
		cpus = cpu_online_mask;

	if (cpumask_empty(cpus))
		return false;

	smp_call_function_many(cpus, ack_flush, NULL, wait);
	return true;
}

bool kvm_make_vcpus_request_mask(struct kvm *kvm, unsigned int req,
<<<<<<< HEAD
=======
				 struct kvm_vcpu *except,
>>>>>>> upstream/android-13
				 unsigned long *vcpu_bitmap, cpumask_var_t tmp)
{
	int i, cpu, me;
	struct kvm_vcpu *vcpu;
	bool called;

	me = get_cpu();

	kvm_for_each_vcpu(i, vcpu, kvm) {
<<<<<<< HEAD
		if (!test_bit(i, vcpu_bitmap))
			continue;

		kvm_make_request(req, vcpu);
		cpu = vcpu->cpu;
=======
		if ((vcpu_bitmap && !test_bit(i, vcpu_bitmap)) ||
		    vcpu == except)
			continue;

		kvm_make_request(req, vcpu);
>>>>>>> upstream/android-13

		if (!(req & KVM_REQUEST_NO_WAKEUP) && kvm_vcpu_wake_up(vcpu))
			continue;

<<<<<<< HEAD
		if (tmp != NULL && cpu != -1 && cpu != me &&
		    kvm_request_needs_ipi(vcpu, req))
			__cpumask_set_cpu(cpu, tmp);
=======
		/*
		 * tmp can be "unavailable" if cpumasks are allocated off stack
		 * as allocation of the mask is deliberately not fatal and is
		 * handled by falling back to kicking all online CPUs.
		 */
		if (!cpumask_available(tmp))
			continue;

		/*
		 * Note, the vCPU could get migrated to a different pCPU at any
		 * point after kvm_request_needs_ipi(), which could result in
		 * sending an IPI to the previous pCPU.  But, that's ok because
		 * the purpose of the IPI is to ensure the vCPU returns to
		 * OUTSIDE_GUEST_MODE, which is satisfied if the vCPU migrates.
		 * Entering READING_SHADOW_PAGE_TABLES after this point is also
		 * ok, as the requirement is only that KVM wait for vCPUs that
		 * were reading SPTEs _before_ any changes were finalized.  See
		 * kvm_vcpu_kick() for more details on handling requests.
		 */
		if (kvm_request_needs_ipi(vcpu, req)) {
			cpu = READ_ONCE(vcpu->cpu);
			if (cpu != -1 && cpu != me)
				__cpumask_set_cpu(cpu, tmp);
		}
>>>>>>> upstream/android-13
	}

	called = kvm_kick_many_cpus(tmp, !!(req & KVM_REQUEST_WAIT));
	put_cpu();

	return called;
}

<<<<<<< HEAD
bool kvm_make_all_cpus_request(struct kvm *kvm, unsigned int req)
{
	cpumask_var_t cpus;
	bool called;
	static unsigned long vcpu_bitmap[BITS_TO_LONGS(KVM_MAX_VCPUS)]
		= {[0 ... BITS_TO_LONGS(KVM_MAX_VCPUS)-1] = ULONG_MAX};

	zalloc_cpumask_var(&cpus, GFP_ATOMIC);

	called = kvm_make_vcpus_request_mask(kvm, req, vcpu_bitmap, cpus);
=======
bool kvm_make_all_cpus_request_except(struct kvm *kvm, unsigned int req,
				      struct kvm_vcpu *except)
{
	cpumask_var_t cpus;
	bool called;

	zalloc_cpumask_var(&cpus, GFP_ATOMIC);

	called = kvm_make_vcpus_request_mask(kvm, req, except, NULL, cpus);
>>>>>>> upstream/android-13

	free_cpumask_var(cpus);
	return called;
}

<<<<<<< HEAD
#ifndef CONFIG_HAVE_KVM_ARCH_TLB_FLUSH_ALL
void kvm_flush_remote_tlbs(struct kvm *kvm)
{
	/*
	 * Read tlbs_dirty before setting KVM_REQ_TLB_FLUSH in
	 * kvm_make_all_cpus_request.
	 */
	long dirty_count = smp_load_acquire(&kvm->tlbs_dirty);
=======
bool kvm_make_all_cpus_request(struct kvm *kvm, unsigned int req)
{
	return kvm_make_all_cpus_request_except(kvm, req, NULL);
}
EXPORT_SYMBOL_GPL(kvm_make_all_cpus_request);

#ifndef CONFIG_HAVE_KVM_ARCH_TLB_FLUSH_ALL
void kvm_flush_remote_tlbs(struct kvm *kvm)
{
	++kvm->stat.generic.remote_tlb_flush_requests;
>>>>>>> upstream/android-13

	/*
	 * We want to publish modifications to the page tables before reading
	 * mode. Pairs with a memory barrier in arch-specific code.
	 * - x86: smp_mb__after_srcu_read_unlock in vcpu_enter_guest
	 * and smp_mb in walk_shadow_page_lockless_begin/end.
	 * - powerpc: smp_mb in kvmppc_prepare_to_enter.
	 *
	 * There is already an smp_mb__after_atomic() before
	 * kvm_make_all_cpus_request() reads vcpu->mode. We reuse that
	 * barrier here.
	 */
	if (!kvm_arch_flush_remote_tlb(kvm)
	    || kvm_make_all_cpus_request(kvm, KVM_REQ_TLB_FLUSH))
<<<<<<< HEAD
		++kvm->stat.remote_tlb_flush;
	cmpxchg(&kvm->tlbs_dirty, dirty_count, 0);
=======
		++kvm->stat.generic.remote_tlb_flush;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(kvm_flush_remote_tlbs);
#endif

void kvm_reload_remote_mmus(struct kvm *kvm)
{
	kvm_make_all_cpus_request(kvm, KVM_REQ_MMU_RELOAD);
}

<<<<<<< HEAD
int kvm_vcpu_init(struct kvm_vcpu *vcpu, struct kvm *kvm, unsigned id)
{
	struct page *page;
	int r;

=======
#ifdef KVM_ARCH_NR_OBJS_PER_MEMORY_CACHE
static inline void *mmu_memory_cache_alloc_obj(struct kvm_mmu_memory_cache *mc,
					       gfp_t gfp_flags)
{
	gfp_flags |= mc->gfp_zero;

	if (mc->kmem_cache)
		return kmem_cache_alloc(mc->kmem_cache, gfp_flags);
	else
		return (void *)__get_free_page(gfp_flags);
}

int kvm_mmu_topup_memory_cache(struct kvm_mmu_memory_cache *mc, int min)
{
	void *obj;

	if (mc->nobjs >= min)
		return 0;
	while (mc->nobjs < ARRAY_SIZE(mc->objects)) {
		obj = mmu_memory_cache_alloc_obj(mc, GFP_KERNEL_ACCOUNT);
		if (!obj)
			return mc->nobjs >= min ? 0 : -ENOMEM;
		mc->objects[mc->nobjs++] = obj;
	}
	return 0;
}

int kvm_mmu_memory_cache_nr_free_objects(struct kvm_mmu_memory_cache *mc)
{
	return mc->nobjs;
}

void kvm_mmu_free_memory_cache(struct kvm_mmu_memory_cache *mc)
{
	while (mc->nobjs) {
		if (mc->kmem_cache)
			kmem_cache_free(mc->kmem_cache, mc->objects[--mc->nobjs]);
		else
			free_page((unsigned long)mc->objects[--mc->nobjs]);
	}
}

void *kvm_mmu_memory_cache_alloc(struct kvm_mmu_memory_cache *mc)
{
	void *p;

	if (WARN_ON(!mc->nobjs))
		p = mmu_memory_cache_alloc_obj(mc, GFP_ATOMIC | __GFP_ACCOUNT);
	else
		p = mc->objects[--mc->nobjs];
	BUG_ON(!p);
	return p;
}
#endif

static void kvm_vcpu_init(struct kvm_vcpu *vcpu, struct kvm *kvm, unsigned id)
{
>>>>>>> upstream/android-13
	mutex_init(&vcpu->mutex);
	vcpu->cpu = -1;
	vcpu->kvm = kvm;
	vcpu->vcpu_id = id;
	vcpu->pid = NULL;
<<<<<<< HEAD
	init_swait_queue_head(&vcpu->wq);
=======
	rcuwait_init(&vcpu->wait);
>>>>>>> upstream/android-13
	kvm_async_pf_vcpu_init(vcpu);

	vcpu->pre_pcpu = -1;
	INIT_LIST_HEAD(&vcpu->blocked_vcpu_list);

<<<<<<< HEAD
	page = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (!page) {
		r = -ENOMEM;
		goto fail;
	}
	vcpu->run = page_address(page);

	kvm_vcpu_set_in_spin_loop(vcpu, false);
	kvm_vcpu_set_dy_eligible(vcpu, false);
	vcpu->preempted = false;

	r = kvm_arch_vcpu_init(vcpu);
	if (r < 0)
		goto fail_free_run;
	return 0;

fail_free_run:
	free_page((unsigned long)vcpu->run);
fail:
	return r;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_init);

void kvm_vcpu_uninit(struct kvm_vcpu *vcpu)
{
	/*
	 * no need for rcu_read_lock as VCPU_RUN is the only place that
	 * will change the vcpu->pid pointer and on uninit all file
	 * descriptors are already gone.
	 */
	put_pid(rcu_dereference_protected(vcpu->pid, 1));
	kvm_arch_vcpu_uninit(vcpu);
	free_page((unsigned long)vcpu->run);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_uninit);
=======
	kvm_vcpu_set_in_spin_loop(vcpu, false);
	kvm_vcpu_set_dy_eligible(vcpu, false);
	vcpu->preempted = false;
	vcpu->ready = false;
	preempt_notifier_init(&vcpu->preempt_notifier, &kvm_preempt_ops);
	vcpu->last_used_slot = 0;
}

void kvm_vcpu_destroy(struct kvm_vcpu *vcpu)
{
	kvm_arch_vcpu_destroy(vcpu);
	kvm_dirty_ring_free(&vcpu->dirty_ring);

	/*
	 * No need for rcu_read_lock as VCPU_RUN is the only place that changes
	 * the vcpu->pid pointer, and at destruction time all file descriptors
	 * are already gone.
	 */
	put_pid(rcu_dereference_protected(vcpu->pid, 1));

	free_page((unsigned long)vcpu->run);
	kmem_cache_free(kvm_vcpu_cache, vcpu);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_destroy);
>>>>>>> upstream/android-13

#if defined(CONFIG_MMU_NOTIFIER) && defined(KVM_ARCH_WANT_MMU_NOTIFIER)
static inline struct kvm *mmu_notifier_to_kvm(struct mmu_notifier *mn)
{
	return container_of(mn, struct kvm, mmu_notifier);
}

static void kvm_mmu_notifier_invalidate_range(struct mmu_notifier *mn,
					      struct mm_struct *mm,
					      unsigned long start, unsigned long end)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int idx;

	idx = srcu_read_lock(&kvm->srcu);
	kvm_arch_mmu_notifier_invalidate_range(kvm, start, end);
	srcu_read_unlock(&kvm->srcu, idx);
}

<<<<<<< HEAD
=======
typedef bool (*hva_handler_t)(struct kvm *kvm, struct kvm_gfn_range *range);

typedef void (*on_lock_fn_t)(struct kvm *kvm, unsigned long start,
			     unsigned long end);

struct kvm_hva_range {
	unsigned long start;
	unsigned long end;
	pte_t pte;
	hva_handler_t handler;
	on_lock_fn_t on_lock;
	bool flush_on_ret;
	bool may_block;
};

/*
 * Use a dedicated stub instead of NULL to indicate that there is no callback
 * function/handler.  The compiler technically can't guarantee that a real
 * function will have a non-zero address, and so it will generate code to
 * check for !NULL, whereas comparing against a stub will be elided at compile
 * time (unless the compiler is getting long in the tooth, e.g. gcc 4.9).
 */
static void kvm_null_fn(void)
{

}
#define IS_KVM_NULL_FN(fn) ((fn) == (void *)kvm_null_fn)

static __always_inline int __kvm_handle_hva_range(struct kvm *kvm,
						  const struct kvm_hva_range *range)
{
	bool ret = false, locked = false;
	struct kvm_gfn_range gfn_range;
	struct kvm_memory_slot *slot;
	struct kvm_memslots *slots;
	int i, idx;

	/* A null handler is allowed if and only if on_lock() is provided. */
	if (WARN_ON_ONCE(IS_KVM_NULL_FN(range->on_lock) &&
			 IS_KVM_NULL_FN(range->handler)))
		return 0;

	idx = srcu_read_lock(&kvm->srcu);

	for (i = 0; i < KVM_ADDRESS_SPACE_NUM; i++) {
		slots = __kvm_memslots(kvm, i);
		kvm_for_each_memslot(slot, slots) {
			unsigned long hva_start, hva_end;

			hva_start = max(range->start, slot->userspace_addr);
			hva_end = min(range->end, slot->userspace_addr +
						  (slot->npages << PAGE_SHIFT));
			if (hva_start >= hva_end)
				continue;

			/*
			 * To optimize for the likely case where the address
			 * range is covered by zero or one memslots, don't
			 * bother making these conditional (to avoid writes on
			 * the second or later invocation of the handler).
			 */
			gfn_range.pte = range->pte;
			gfn_range.may_block = range->may_block;

			/*
			 * {gfn(page) | page intersects with [hva_start, hva_end)} =
			 * {gfn_start, gfn_start+1, ..., gfn_end-1}.
			 */
			gfn_range.start = hva_to_gfn_memslot(hva_start, slot);
			gfn_range.end = hva_to_gfn_memslot(hva_end + PAGE_SIZE - 1, slot);
			gfn_range.slot = slot;

			if (!locked) {
				locked = true;
				KVM_MMU_LOCK(kvm);
				if (!IS_KVM_NULL_FN(range->on_lock))
					range->on_lock(kvm, range->start, range->end);
				if (IS_KVM_NULL_FN(range->handler))
					break;
			}
			ret |= range->handler(kvm, &gfn_range);
		}
	}

	if (range->flush_on_ret && ret)
		kvm_flush_remote_tlbs(kvm);

	if (locked)
		KVM_MMU_UNLOCK(kvm);

	srcu_read_unlock(&kvm->srcu, idx);

	/* The notifiers are averse to booleans. :-( */
	return (int)ret;
}

static __always_inline int kvm_handle_hva_range(struct mmu_notifier *mn,
						unsigned long start,
						unsigned long end,
						pte_t pte,
						hva_handler_t handler)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	const struct kvm_hva_range range = {
		.start		= start,
		.end		= end,
		.pte		= pte,
		.handler	= handler,
		.on_lock	= (void *)kvm_null_fn,
		.flush_on_ret	= true,
		.may_block	= false,
	};

	return __kvm_handle_hva_range(kvm, &range);
}

static __always_inline int kvm_handle_hva_range_no_flush(struct mmu_notifier *mn,
							 unsigned long start,
							 unsigned long end,
							 hva_handler_t handler)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	const struct kvm_hva_range range = {
		.start		= start,
		.end		= end,
		.pte		= __pte(0),
		.handler	= handler,
		.on_lock	= (void *)kvm_null_fn,
		.flush_on_ret	= false,
		.may_block	= false,
	};

	return __kvm_handle_hva_range(kvm, &range);
}
>>>>>>> upstream/android-13
static void kvm_mmu_notifier_change_pte(struct mmu_notifier *mn,
					struct mm_struct *mm,
					unsigned long address,
					pte_t pte)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
<<<<<<< HEAD
	int idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
	kvm->mmu_notifier_seq++;
	kvm_set_spte_hva(kvm, address, pte);
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);
}

static int kvm_mmu_notifier_invalidate_range_start(struct mmu_notifier *mn,
						    struct mm_struct *mm,
						    unsigned long start,
						    unsigned long end,
						    bool blockable)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int need_tlb_flush = 0, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
=======

	trace_kvm_set_spte_hva(address);

	/*
	 * .change_pte() must be surrounded by .invalidate_range_{start,end}().
	 * If mmu_notifier_count is zero, then no in-progress invalidations,
	 * including this one, found a relevant memslot at start(); rechecking
	 * memslots here is unnecessary.  Note, a false positive (count elevated
	 * by a different invalidation) is sub-optimal but functionally ok.
	 */
	WARN_ON_ONCE(!READ_ONCE(kvm->mn_active_invalidate_count));
	if (!READ_ONCE(kvm->mmu_notifier_count))
		return;

	kvm_handle_hva_range(mn, address, address + 1, pte, kvm_set_spte_gfn);
}

void kvm_inc_notifier_count(struct kvm *kvm, unsigned long start,
				   unsigned long end)
{
>>>>>>> upstream/android-13
	/*
	 * The count increase must become visible at unlock time as no
	 * spte can be established without taking the mmu_lock and
	 * count is also read inside the mmu_lock critical section.
	 */
	kvm->mmu_notifier_count++;
<<<<<<< HEAD
	need_tlb_flush = kvm_unmap_hva_range(kvm, start, end, blockable);
	/* we've to flush the tlb before the pages can be freed */
	if (need_tlb_flush || kvm->tlbs_dirty)
		kvm_flush_remote_tlbs(kvm);

	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);
=======
	if (likely(kvm->mmu_notifier_count == 1)) {
		kvm->mmu_notifier_range_start = start;
		kvm->mmu_notifier_range_end = end;
	} else {
		/*
		 * Fully tracking multiple concurrent ranges has dimishing
		 * returns. Keep things simple and just find the minimal range
		 * which includes the current and new ranges. As there won't be
		 * enough information to subtract a range after its invalidate
		 * completes, any ranges invalidated concurrently will
		 * accumulate and persist until all outstanding invalidates
		 * complete.
		 */
		kvm->mmu_notifier_range_start =
			min(kvm->mmu_notifier_range_start, start);
		kvm->mmu_notifier_range_end =
			max(kvm->mmu_notifier_range_end, end);
	}
}

static int kvm_mmu_notifier_invalidate_range_start(struct mmu_notifier *mn,
					const struct mmu_notifier_range *range)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	const struct kvm_hva_range hva_range = {
		.start		= range->start,
		.end		= range->end,
		.pte		= __pte(0),
		.handler	= kvm_unmap_gfn_range,
		.on_lock	= kvm_inc_notifier_count,
		.flush_on_ret	= true,
		.may_block	= mmu_notifier_range_blockable(range),
	};

	trace_kvm_unmap_hva_range(range->start, range->end);

	/*
	 * Prevent memslot modification between range_start() and range_end()
	 * so that conditionally locking provides the same result in both
	 * functions.  Without that guarantee, the mmu_notifier_count
	 * adjustments will be imbalanced.
	 *
	 * Pairs with the decrement in range_end().
	 */
	spin_lock(&kvm->mn_invalidate_lock);
	kvm->mn_active_invalidate_count++;
	spin_unlock(&kvm->mn_invalidate_lock);

	__kvm_handle_hva_range(kvm, &hva_range);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void kvm_mmu_notifier_invalidate_range_end(struct mmu_notifier *mn,
						  struct mm_struct *mm,
						  unsigned long start,
						  unsigned long end)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);

	spin_lock(&kvm->mmu_lock);
=======
void kvm_dec_notifier_count(struct kvm *kvm, unsigned long start,
				   unsigned long end)
{
>>>>>>> upstream/android-13
	/*
	 * This sequence increase will notify the kvm page fault that
	 * the page that is going to be mapped in the spte could have
	 * been freed.
	 */
	kvm->mmu_notifier_seq++;
	smp_wmb();
	/*
	 * The above sequence increase must be visible before the
	 * below count decrease, which is ensured by the smp_wmb above
	 * in conjunction with the smp_rmb in mmu_notifier_retry().
	 */
	kvm->mmu_notifier_count--;
<<<<<<< HEAD
	spin_unlock(&kvm->mmu_lock);
=======
}

static void kvm_mmu_notifier_invalidate_range_end(struct mmu_notifier *mn,
					const struct mmu_notifier_range *range)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	const struct kvm_hva_range hva_range = {
		.start		= range->start,
		.end		= range->end,
		.pte		= __pte(0),
		.handler	= (void *)kvm_null_fn,
		.on_lock	= kvm_dec_notifier_count,
		.flush_on_ret	= false,
		.may_block	= mmu_notifier_range_blockable(range),
	};
	bool wake;

	__kvm_handle_hva_range(kvm, &hva_range);

	/* Pairs with the increment in range_start(). */
	spin_lock(&kvm->mn_invalidate_lock);
	wake = (--kvm->mn_active_invalidate_count == 0);
	spin_unlock(&kvm->mn_invalidate_lock);

	/*
	 * There can only be one waiter, since the wait happens under
	 * slots_lock.
	 */
	if (wake)
		rcuwait_wake_up(&kvm->mn_memslots_update_rcuwait);
>>>>>>> upstream/android-13

	BUG_ON(kvm->mmu_notifier_count < 0);
}

static int kvm_mmu_notifier_clear_flush_young(struct mmu_notifier *mn,
					      struct mm_struct *mm,
					      unsigned long start,
					      unsigned long end)
{
<<<<<<< HEAD
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int young, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);

	young = kvm_age_hva(kvm, start, end);
	if (young)
		kvm_flush_remote_tlbs(kvm);

	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

	return young;
=======
	trace_kvm_age_hva(start, end);

	return kvm_handle_hva_range(mn, start, end, __pte(0), kvm_age_gfn);
>>>>>>> upstream/android-13
}

static int kvm_mmu_notifier_clear_young(struct mmu_notifier *mn,
					struct mm_struct *mm,
					unsigned long start,
					unsigned long end)
{
<<<<<<< HEAD
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int young, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
=======
	trace_kvm_age_hva(start, end);

>>>>>>> upstream/android-13
	/*
	 * Even though we do not flush TLB, this will still adversely
	 * affect performance on pre-Haswell Intel EPT, where there is
	 * no EPT Access Bit to clear so that we have to tear down EPT
	 * tables instead. If we find this unacceptable, we can always
	 * add a parameter to kvm_age_hva so that it effectively doesn't
	 * do anything on clear_young.
	 *
	 * Also note that currently we never issue secondary TLB flushes
	 * from clear_young, leaving this job up to the regular system
	 * cadence. If we find this inaccurate, we might come up with a
	 * more sophisticated heuristic later.
	 */
<<<<<<< HEAD
	young = kvm_age_hva(kvm, start, end);
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

	return young;
=======
	return kvm_handle_hva_range_no_flush(mn, start, end, kvm_age_gfn);
>>>>>>> upstream/android-13
}

static int kvm_mmu_notifier_test_young(struct mmu_notifier *mn,
				       struct mm_struct *mm,
				       unsigned long address)
{
<<<<<<< HEAD
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int young, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
	young = kvm_test_age_hva(kvm, address);
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

	return young;
=======
	trace_kvm_test_age_hva(address);

	return kvm_handle_hva_range_no_flush(mn, address, address + 1,
					     kvm_test_age_gfn);
>>>>>>> upstream/android-13
}

static void kvm_mmu_notifier_release(struct mmu_notifier *mn,
				     struct mm_struct *mm)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int idx;

	idx = srcu_read_lock(&kvm->srcu);
	kvm_arch_flush_shadow_all(kvm);
	srcu_read_unlock(&kvm->srcu, idx);
}

static const struct mmu_notifier_ops kvm_mmu_notifier_ops = {
<<<<<<< HEAD
	.flags			= MMU_INVALIDATE_DOES_NOT_BLOCK,
=======
>>>>>>> upstream/android-13
	.invalidate_range	= kvm_mmu_notifier_invalidate_range,
	.invalidate_range_start	= kvm_mmu_notifier_invalidate_range_start,
	.invalidate_range_end	= kvm_mmu_notifier_invalidate_range_end,
	.clear_flush_young	= kvm_mmu_notifier_clear_flush_young,
	.clear_young		= kvm_mmu_notifier_clear_young,
	.test_young		= kvm_mmu_notifier_test_young,
	.change_pte		= kvm_mmu_notifier_change_pte,
	.release		= kvm_mmu_notifier_release,
};

static int kvm_init_mmu_notifier(struct kvm *kvm)
{
	kvm->mmu_notifier.ops = &kvm_mmu_notifier_ops;
	return mmu_notifier_register(&kvm->mmu_notifier, current->mm);
}

#else  /* !(CONFIG_MMU_NOTIFIER && KVM_ARCH_WANT_MMU_NOTIFIER) */

static int kvm_init_mmu_notifier(struct kvm *kvm)
{
	return 0;
}

#endif /* CONFIG_MMU_NOTIFIER && KVM_ARCH_WANT_MMU_NOTIFIER */

<<<<<<< HEAD
=======
#ifdef CONFIG_HAVE_KVM_PM_NOTIFIER
static int kvm_pm_notifier_call(struct notifier_block *bl,
				unsigned long state,
				void *unused)
{
	struct kvm *kvm = container_of(bl, struct kvm, pm_notifier);

	return kvm_arch_pm_notifier(kvm, state);
}

static void kvm_init_pm_notifier(struct kvm *kvm)
{
	kvm->pm_notifier.notifier_call = kvm_pm_notifier_call;
	/* Suspend KVM before we suspend ftrace, RCU, etc. */
	kvm->pm_notifier.priority = INT_MAX;
	register_pm_notifier(&kvm->pm_notifier);
}

static void kvm_destroy_pm_notifier(struct kvm *kvm)
{
	unregister_pm_notifier(&kvm->pm_notifier);
}
#else /* !CONFIG_HAVE_KVM_PM_NOTIFIER */
static void kvm_init_pm_notifier(struct kvm *kvm)
{
}

static void kvm_destroy_pm_notifier(struct kvm *kvm)
{
}
#endif /* CONFIG_HAVE_KVM_PM_NOTIFIER */

>>>>>>> upstream/android-13
static struct kvm_memslots *kvm_alloc_memslots(void)
{
	int i;
	struct kvm_memslots *slots;

<<<<<<< HEAD
	slots = kvzalloc(sizeof(struct kvm_memslots), GFP_KERNEL);
=======
	slots = kvzalloc(sizeof(struct kvm_memslots), GFP_KERNEL_ACCOUNT);
>>>>>>> upstream/android-13
	if (!slots)
		return NULL;

	for (i = 0; i < KVM_MEM_SLOTS_NUM; i++)
<<<<<<< HEAD
		slots->id_to_index[i] = slots->memslots[i].id = i;
=======
		slots->id_to_index[i] = -1;
>>>>>>> upstream/android-13

	return slots;
}

static void kvm_destroy_dirty_bitmap(struct kvm_memory_slot *memslot)
{
	if (!memslot->dirty_bitmap)
		return;

	kvfree(memslot->dirty_bitmap);
	memslot->dirty_bitmap = NULL;
}

<<<<<<< HEAD
/*
 * Free any memory in @free but not in @dont.
 */
static void kvm_free_memslot(struct kvm *kvm, struct kvm_memory_slot *free,
			      struct kvm_memory_slot *dont)
{
	if (!dont || free->dirty_bitmap != dont->dirty_bitmap)
		kvm_destroy_dirty_bitmap(free);

	kvm_arch_free_memslot(kvm, free, dont);

	free->npages = 0;
=======
static void kvm_free_memslot(struct kvm *kvm, struct kvm_memory_slot *slot)
{
	kvm_destroy_dirty_bitmap(slot);

	kvm_arch_free_memslot(kvm, slot);

	slot->flags = 0;
	slot->npages = 0;
>>>>>>> upstream/android-13
}

static void kvm_free_memslots(struct kvm *kvm, struct kvm_memslots *slots)
{
	struct kvm_memory_slot *memslot;

	if (!slots)
		return;

	kvm_for_each_memslot(memslot, slots)
<<<<<<< HEAD
		kvm_free_memslot(kvm, memslot, NULL);
=======
		kvm_free_memslot(kvm, memslot);
>>>>>>> upstream/android-13

	kvfree(slots);
}

<<<<<<< HEAD
static void kvm_destroy_vm_debugfs(struct kvm *kvm)
{
	int i;
=======
static umode_t kvm_stats_debugfs_mode(const struct _kvm_stats_desc *pdesc)
{
	switch (pdesc->desc.flags & KVM_STATS_TYPE_MASK) {
	case KVM_STATS_TYPE_INSTANT:
		return 0444;
	case KVM_STATS_TYPE_CUMULATIVE:
	case KVM_STATS_TYPE_PEAK:
	default:
		return 0644;
	}
}


static void kvm_destroy_vm_debugfs(struct kvm *kvm)
{
	int i;
	int kvm_debugfs_num_entries = kvm_vm_stats_header.num_desc +
				      kvm_vcpu_stats_header.num_desc;
>>>>>>> upstream/android-13

	if (!kvm->debugfs_dentry)
		return;

	debugfs_remove_recursive(kvm->debugfs_dentry);

	if (kvm->debugfs_stat_data) {
		for (i = 0; i < kvm_debugfs_num_entries; i++)
			kfree(kvm->debugfs_stat_data[i]);
		kfree(kvm->debugfs_stat_data);
	}
}

static int kvm_create_vm_debugfs(struct kvm *kvm, int fd)
{
<<<<<<< HEAD
	char dir_name[ITOA_MAX_LEN * 2];
	struct kvm_stat_data *stat_data;
	struct kvm_stats_debugfs_item *p;
=======
	static DEFINE_MUTEX(kvm_debugfs_lock);
	struct dentry *dent;
	char dir_name[ITOA_MAX_LEN * 2];
	struct kvm_stat_data *stat_data;
	const struct _kvm_stats_desc *pdesc;
	int i, ret;
	int kvm_debugfs_num_entries = kvm_vm_stats_header.num_desc +
				      kvm_vcpu_stats_header.num_desc;
>>>>>>> upstream/android-13

	if (!debugfs_initialized())
		return 0;

	snprintf(dir_name, sizeof(dir_name), "%d-%d", task_pid_nr(current), fd);
<<<<<<< HEAD
	kvm->debugfs_dentry = debugfs_create_dir(dir_name, kvm_debugfs_dir);

	kvm->debugfs_stat_data = kcalloc(kvm_debugfs_num_entries,
					 sizeof(*kvm->debugfs_stat_data),
					 GFP_KERNEL);
	if (!kvm->debugfs_stat_data)
		return -ENOMEM;

	for (p = debugfs_entries; p->name; p++) {
		stat_data = kzalloc(sizeof(*stat_data), GFP_KERNEL);
=======
	mutex_lock(&kvm_debugfs_lock);
	dent = debugfs_lookup(dir_name, kvm_debugfs_dir);
	if (dent) {
		pr_warn_ratelimited("KVM: debugfs: duplicate directory %s\n", dir_name);
		dput(dent);
		mutex_unlock(&kvm_debugfs_lock);
		return 0;
	}
	dent = debugfs_create_dir(dir_name, kvm_debugfs_dir);
	mutex_unlock(&kvm_debugfs_lock);
	if (IS_ERR(dent))
		return 0;

	kvm->debugfs_dentry = dent;
	kvm->debugfs_stat_data = kcalloc(kvm_debugfs_num_entries,
					 sizeof(*kvm->debugfs_stat_data),
					 GFP_KERNEL_ACCOUNT);
	if (!kvm->debugfs_stat_data)
		return -ENOMEM;

	for (i = 0; i < kvm_vm_stats_header.num_desc; ++i) {
		pdesc = &kvm_vm_stats_desc[i];
		stat_data = kzalloc(sizeof(*stat_data), GFP_KERNEL_ACCOUNT);
>>>>>>> upstream/android-13
		if (!stat_data)
			return -ENOMEM;

		stat_data->kvm = kvm;
<<<<<<< HEAD
		stat_data->offset = p->offset;
		stat_data->mode = p->mode ? p->mode : 0644;
		kvm->debugfs_stat_data[p - debugfs_entries] = stat_data;
		debugfs_create_file(p->name, stat_data->mode, kvm->debugfs_dentry,
				    stat_data, stat_fops_per_vm[p->kind]);
	}
=======
		stat_data->desc = pdesc;
		stat_data->kind = KVM_STAT_VM;
		kvm->debugfs_stat_data[i] = stat_data;
		debugfs_create_file(pdesc->name, kvm_stats_debugfs_mode(pdesc),
				    kvm->debugfs_dentry, stat_data,
				    &stat_fops_per_vm);
	}

	for (i = 0; i < kvm_vcpu_stats_header.num_desc; ++i) {
		pdesc = &kvm_vcpu_stats_desc[i];
		stat_data = kzalloc(sizeof(*stat_data), GFP_KERNEL_ACCOUNT);
		if (!stat_data)
			return -ENOMEM;

		stat_data->kvm = kvm;
		stat_data->desc = pdesc;
		stat_data->kind = KVM_STAT_VCPU;
		kvm->debugfs_stat_data[i + kvm_vm_stats_header.num_desc] = stat_data;
		debugfs_create_file(pdesc->name, kvm_stats_debugfs_mode(pdesc),
				    kvm->debugfs_dentry, stat_data,
				    &stat_fops_per_vm);
	}

	ret = kvm_arch_create_vm_debugfs(kvm);
	if (ret) {
		kvm_destroy_vm_debugfs(kvm);
		return i;
	}

>>>>>>> upstream/android-13
	return 0;
}

/*
 * Called after the VM is otherwise initialized, but just before adding it to
 * the vm_list.
 */
int __weak kvm_arch_post_init_vm(struct kvm *kvm)
{
	return 0;
}

/*
 * Called just after removing the VM from the vm_list, but before doing any
 * other destruction.
 */
void __weak kvm_arch_pre_destroy_vm(struct kvm *kvm)
{
}

<<<<<<< HEAD
static struct kvm *kvm_create_vm(unsigned long type)
{
	int r, i;
	struct kvm *kvm = kvm_arch_alloc_vm();
=======
/*
 * Called after per-vm debugfs created.  When called kvm->debugfs_dentry should
 * be setup already, so we can create arch-specific debugfs entries under it.
 * Cleanup should be automatic done in kvm_destroy_vm_debugfs() recursively, so
 * a per-arch destroy interface is not needed.
 */
int __weak kvm_arch_create_vm_debugfs(struct kvm *kvm)
{
	return 0;
}

static struct kvm *kvm_create_vm(unsigned long type)
{
	struct kvm *kvm = kvm_arch_alloc_vm();
	int r = -ENOMEM;
	int i;
>>>>>>> upstream/android-13

	if (!kvm)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	spin_lock_init(&kvm->mmu_lock);
=======
	KVM_MMU_LOCK_INIT(kvm);
>>>>>>> upstream/android-13
	mmgrab(current->mm);
	kvm->mm = current->mm;
	kvm_eventfd_init(kvm);
	mutex_init(&kvm->lock);
	mutex_init(&kvm->irq_lock);
	mutex_init(&kvm->slots_lock);
<<<<<<< HEAD
	refcount_set(&kvm->users_count, 1);
	INIT_LIST_HEAD(&kvm->devices);

	r = kvm_arch_init_vm(kvm, type);
	if (r)
		goto out_err_no_disable;
=======
	mutex_init(&kvm->slots_arch_lock);
	spin_lock_init(&kvm->mn_invalidate_lock);
	rcuwait_init(&kvm->mn_memslots_update_rcuwait);

	INIT_LIST_HEAD(&kvm->devices);

	BUILD_BUG_ON(KVM_MEM_SLOTS_NUM > SHRT_MAX);

	if (init_srcu_struct(&kvm->srcu))
		goto out_err_no_srcu;
	if (init_srcu_struct(&kvm->irq_srcu))
		goto out_err_no_irq_srcu;

	refcount_set(&kvm->users_count, 1);
	for (i = 0; i < KVM_ADDRESS_SPACE_NUM; i++) {
		struct kvm_memslots *slots = kvm_alloc_memslots();

		if (!slots)
			goto out_err_no_arch_destroy_vm;
		/* Generations must be different for each address space. */
		slots->generation = i;
		rcu_assign_pointer(kvm->memslots[i], slots);
	}

	for (i = 0; i < KVM_NR_BUSES; i++) {
		rcu_assign_pointer(kvm->buses[i],
			kzalloc(sizeof(struct kvm_io_bus), GFP_KERNEL_ACCOUNT));
		if (!kvm->buses[i])
			goto out_err_no_arch_destroy_vm;
	}

	kvm->max_halt_poll_ns = halt_poll_ns;

	r = kvm_arch_init_vm(kvm, type);
	if (r)
		goto out_err_no_arch_destroy_vm;
>>>>>>> upstream/android-13

	r = hardware_enable_all();
	if (r)
		goto out_err_no_disable;

#ifdef CONFIG_HAVE_KVM_IRQFD
	INIT_HLIST_HEAD(&kvm->irq_ack_notifier_list);
#endif

<<<<<<< HEAD
	BUILD_BUG_ON(KVM_MEM_SLOTS_NUM > SHRT_MAX);

	r = -ENOMEM;
	for (i = 0; i < KVM_ADDRESS_SPACE_NUM; i++) {
		struct kvm_memslots *slots = kvm_alloc_memslots();
		if (!slots)
			goto out_err_no_srcu;
		/*
		 * Generations must be different for each address space.
		 * Init kvm generation close to the maximum to easily test the
		 * code of handling generation number wrap-around.
		 */
		slots->generation = i * 2 - 150;
		rcu_assign_pointer(kvm->memslots[i], slots);
	}

	if (init_srcu_struct(&kvm->srcu))
		goto out_err_no_srcu;
	if (init_srcu_struct(&kvm->irq_srcu))
		goto out_err_no_irq_srcu;
	for (i = 0; i < KVM_NR_BUSES; i++) {
		rcu_assign_pointer(kvm->buses[i],
			kzalloc(sizeof(struct kvm_io_bus), GFP_KERNEL));
		if (!kvm->buses[i])
			goto out_err_no_mmu_notifier;
	}

=======
>>>>>>> upstream/android-13
	r = kvm_init_mmu_notifier(kvm);
	if (r)
		goto out_err_no_mmu_notifier;

	r = kvm_arch_post_init_vm(kvm);
	if (r)
		goto out_err;

	mutex_lock(&kvm_lock);
	list_add(&kvm->vm_list, &vm_list);
	mutex_unlock(&kvm_lock);

	preempt_notifier_inc();
<<<<<<< HEAD
=======
	kvm_init_pm_notifier(kvm);

	/*
	 * When the fd passed to this ioctl() is opened it pins the module,
	 * but try_module_get() also prevents getting a reference if the module
	 * is in MODULE_STATE_GOING (e.g. if someone ran "rmmod --wait").
	 */
	if (!try_module_get(kvm_chardev_ops.owner)) {
		r = -ENODEV;
		goto out_err;
	}
>>>>>>> upstream/android-13

	return kvm;

out_err:
#if defined(CONFIG_MMU_NOTIFIER) && defined(KVM_ARCH_WANT_MMU_NOTIFIER)
	if (kvm->mmu_notifier.ops)
		mmu_notifier_unregister(&kvm->mmu_notifier, current->mm);
#endif
out_err_no_mmu_notifier:
<<<<<<< HEAD
	cleanup_srcu_struct(&kvm->irq_srcu);
out_err_no_irq_srcu:
	cleanup_srcu_struct(&kvm->srcu);
out_err_no_srcu:
	hardware_disable_all();
out_err_no_disable:
	refcount_set(&kvm->users_count, 0);
=======
	hardware_disable_all();
out_err_no_disable:
	kvm_arch_destroy_vm(kvm);
out_err_no_arch_destroy_vm:
	WARN_ON_ONCE(!refcount_dec_and_test(&kvm->users_count));
>>>>>>> upstream/android-13
	for (i = 0; i < KVM_NR_BUSES; i++)
		kfree(kvm_get_bus(kvm, i));
	for (i = 0; i < KVM_ADDRESS_SPACE_NUM; i++)
		kvm_free_memslots(kvm, __kvm_memslots(kvm, i));
<<<<<<< HEAD
=======
	cleanup_srcu_struct(&kvm->irq_srcu);
out_err_no_irq_srcu:
	cleanup_srcu_struct(&kvm->srcu);
out_err_no_srcu:
>>>>>>> upstream/android-13
	kvm_arch_free_vm(kvm);
	mmdrop(current->mm);
	return ERR_PTR(r);
}

static void kvm_destroy_devices(struct kvm *kvm)
{
	struct kvm_device *dev, *tmp;

	/*
	 * We do not need to take the kvm->lock here, because nobody else
	 * has a reference to the struct kvm at this point and therefore
	 * cannot access the devices list anyhow.
	 */
	list_for_each_entry_safe(dev, tmp, &kvm->devices, vm_node) {
		list_del(&dev->vm_node);
		dev->ops->destroy(dev);
	}
}

static void kvm_destroy_vm(struct kvm *kvm)
{
	int i;
	struct mm_struct *mm = kvm->mm;

<<<<<<< HEAD
=======
	kvm_destroy_pm_notifier(kvm);
>>>>>>> upstream/android-13
	kvm_uevent_notify_change(KVM_EVENT_DESTROY_VM, kvm);
	kvm_destroy_vm_debugfs(kvm);
	kvm_arch_sync_events(kvm);
	mutex_lock(&kvm_lock);
	list_del(&kvm->vm_list);
	mutex_unlock(&kvm_lock);
	kvm_arch_pre_destroy_vm(kvm);

	kvm_free_irq_routing(kvm);
	for (i = 0; i < KVM_NR_BUSES; i++) {
		struct kvm_io_bus *bus = kvm_get_bus(kvm, i);

		if (bus)
			kvm_io_bus_destroy(bus);
		kvm->buses[i] = NULL;
	}
	kvm_coalesced_mmio_free(kvm);
#if defined(CONFIG_MMU_NOTIFIER) && defined(KVM_ARCH_WANT_MMU_NOTIFIER)
	mmu_notifier_unregister(&kvm->mmu_notifier, kvm->mm);
<<<<<<< HEAD
=======
	/*
	 * At this point, pending calls to invalidate_range_start()
	 * have completed but no more MMU notifiers will run, so
	 * mn_active_invalidate_count may remain unbalanced.
	 * No threads can be waiting in install_new_memslots as the
	 * last reference on KVM has been dropped, but freeing
	 * memslots would deadlock without this manual intervention.
	 */
	WARN_ON(rcuwait_active(&kvm->mn_memslots_update_rcuwait));
	kvm->mn_active_invalidate_count = 0;
>>>>>>> upstream/android-13
#else
	kvm_arch_flush_shadow_all(kvm);
#endif
	kvm_arch_destroy_vm(kvm);
	kvm_destroy_devices(kvm);
	for (i = 0; i < KVM_ADDRESS_SPACE_NUM; i++)
		kvm_free_memslots(kvm, __kvm_memslots(kvm, i));
	cleanup_srcu_struct(&kvm->irq_srcu);
	cleanup_srcu_struct(&kvm->srcu);
	kvm_arch_free_vm(kvm);
	preempt_notifier_dec();
	hardware_disable_all();
	mmdrop(mm);
<<<<<<< HEAD
=======
	module_put(kvm_chardev_ops.owner);
>>>>>>> upstream/android-13
}

void kvm_get_kvm(struct kvm *kvm)
{
	refcount_inc(&kvm->users_count);
}
EXPORT_SYMBOL_GPL(kvm_get_kvm);

<<<<<<< HEAD
=======
/*
 * Make sure the vm is not during destruction, which is a safe version of
 * kvm_get_kvm().  Return true if kvm referenced successfully, false otherwise.
 */
bool kvm_get_kvm_safe(struct kvm *kvm)
{
	return refcount_inc_not_zero(&kvm->users_count);
}
EXPORT_SYMBOL_GPL(kvm_get_kvm_safe);

>>>>>>> upstream/android-13
void kvm_put_kvm(struct kvm *kvm)
{
	if (refcount_dec_and_test(&kvm->users_count))
		kvm_destroy_vm(kvm);
}
EXPORT_SYMBOL_GPL(kvm_put_kvm);

<<<<<<< HEAD
=======
/*
 * Used to put a reference that was taken on behalf of an object associated
 * with a user-visible file descriptor, e.g. a vcpu or device, if installation
 * of the new file descriptor fails and the reference cannot be transferred to
 * its final owner.  In such cases, the caller is still actively using @kvm and
 * will fail miserably if the refcount unexpectedly hits zero.
 */
void kvm_put_kvm_no_destroy(struct kvm *kvm)
{
	WARN_ON(refcount_dec_and_test(&kvm->users_count));
}
EXPORT_SYMBOL_GPL(kvm_put_kvm_no_destroy);
>>>>>>> upstream/android-13

static int kvm_vm_release(struct inode *inode, struct file *filp)
{
	struct kvm *kvm = filp->private_data;

	kvm_irqfd_release(kvm);

	kvm_put_kvm(kvm);
	return 0;
}

/*
 * Allocation size is twice as large as the actual dirty bitmap size.
<<<<<<< HEAD
 * See x86's kvm_vm_ioctl_get_dirty_log() why this is needed.
 */
static int kvm_create_dirty_bitmap(struct kvm_memory_slot *memslot)
{
	unsigned long dirty_bytes = 2 * kvm_dirty_bitmap_bytes(memslot);

	memslot->dirty_bitmap = kvzalloc(dirty_bytes, GFP_KERNEL);
=======
 * See kvm_vm_ioctl_get_dirty_log() why this is needed.
 */
static int kvm_alloc_dirty_bitmap(struct kvm_memory_slot *memslot)
{
	unsigned long dirty_bytes = 2 * kvm_dirty_bitmap_bytes(memslot);

	memslot->dirty_bitmap = kvzalloc(dirty_bytes, GFP_KERNEL_ACCOUNT);
>>>>>>> upstream/android-13
	if (!memslot->dirty_bitmap)
		return -ENOMEM;

	return 0;
}

/*
<<<<<<< HEAD
 * Insert memslot and re-sort memslots based on their GFN,
 * so binary search could be used to lookup GFN.
 * Sorting algorithm takes advantage of having initially
 * sorted array and known changed memslot position.
 */
static void update_memslots(struct kvm_memslots *slots,
			    struct kvm_memory_slot *new)
{
	int id = new->id;
	int i = slots->id_to_index[id];
	struct kvm_memory_slot *mslots = slots->memslots;

	WARN_ON(mslots[i].id != id);
	if (!new->npages) {
		WARN_ON(!mslots[i].npages);
		if (mslots[i].npages)
			slots->used_slots--;
	} else {
		if (!mslots[i].npages)
			slots->used_slots++;
	}

	while (i < KVM_MEM_SLOTS_NUM - 1 &&
	       new->base_gfn <= mslots[i + 1].base_gfn) {
		if (!mslots[i + 1].npages)
			break;
		mslots[i] = mslots[i + 1];
		slots->id_to_index[mslots[i].id] = i;
		i++;
	}

	/*
	 * The ">=" is needed when creating a slot with base_gfn == 0,
	 * so that it moves before all those with base_gfn == npages == 0.
	 *
	 * On the other hand, if new->npages is zero, the above loop has
	 * already left i pointing to the beginning of the empty part of
	 * mslots, and the ">=" would move the hole backwards in this
	 * case---which is wrong.  So skip the loop when deleting a slot.
	 */
	if (new->npages) {
		while (i > 0 &&
		       new->base_gfn >= mslots[i - 1].base_gfn) {
			mslots[i] = mslots[i - 1];
			slots->id_to_index[mslots[i].id] = i;
			i--;
		}
	} else
		WARN_ON_ONCE(i != slots->used_slots);

	mslots[i] = *new;
	slots->id_to_index[mslots[i].id] = i;
=======
 * Delete a memslot by decrementing the number of used slots and shifting all
 * other entries in the array forward one spot.
 */
static inline void kvm_memslot_delete(struct kvm_memslots *slots,
				      struct kvm_memory_slot *memslot)
{
	struct kvm_memory_slot *mslots = slots->memslots;
	int i;

	if (WARN_ON(slots->id_to_index[memslot->id] == -1))
		return;

	slots->used_slots--;

	if (atomic_read(&slots->last_used_slot) >= slots->used_slots)
		atomic_set(&slots->last_used_slot, 0);

	for (i = slots->id_to_index[memslot->id]; i < slots->used_slots; i++) {
		mslots[i] = mslots[i + 1];
		slots->id_to_index[mslots[i].id] = i;
	}
	mslots[i] = *memslot;
	slots->id_to_index[memslot->id] = -1;
}

/*
 * "Insert" a new memslot by incrementing the number of used slots.  Returns
 * the new slot's initial index into the memslots array.
 */
static inline int kvm_memslot_insert_back(struct kvm_memslots *slots)
{
	return slots->used_slots++;
}

/*
 * Move a changed memslot backwards in the array by shifting existing slots
 * with a higher GFN toward the front of the array.  Note, the changed memslot
 * itself is not preserved in the array, i.e. not swapped at this time, only
 * its new index into the array is tracked.  Returns the changed memslot's
 * current index into the memslots array.
 */
static inline int kvm_memslot_move_backward(struct kvm_memslots *slots,
					    struct kvm_memory_slot *memslot)
{
	struct kvm_memory_slot *mslots = slots->memslots;
	int i;

	if (WARN_ON_ONCE(slots->id_to_index[memslot->id] == -1) ||
	    WARN_ON_ONCE(!slots->used_slots))
		return -1;

	/*
	 * Move the target memslot backward in the array by shifting existing
	 * memslots with a higher GFN (than the target memslot) towards the
	 * front of the array.
	 */
	for (i = slots->id_to_index[memslot->id]; i < slots->used_slots - 1; i++) {
		if (memslot->base_gfn > mslots[i + 1].base_gfn)
			break;

		WARN_ON_ONCE(memslot->base_gfn == mslots[i + 1].base_gfn);

		/* Shift the next memslot forward one and update its index. */
		mslots[i] = mslots[i + 1];
		slots->id_to_index[mslots[i].id] = i;
	}
	return i;
}

/*
 * Move a changed memslot forwards in the array by shifting existing slots with
 * a lower GFN toward the back of the array.  Note, the changed memslot itself
 * is not preserved in the array, i.e. not swapped at this time, only its new
 * index into the array is tracked.  Returns the changed memslot's final index
 * into the memslots array.
 */
static inline int kvm_memslot_move_forward(struct kvm_memslots *slots,
					   struct kvm_memory_slot *memslot,
					   int start)
{
	struct kvm_memory_slot *mslots = slots->memslots;
	int i;

	for (i = start; i > 0; i--) {
		if (memslot->base_gfn < mslots[i - 1].base_gfn)
			break;

		WARN_ON_ONCE(memslot->base_gfn == mslots[i - 1].base_gfn);

		/* Shift the next memslot back one and update its index. */
		mslots[i] = mslots[i - 1];
		slots->id_to_index[mslots[i].id] = i;
	}
	return i;
}

/*
 * Re-sort memslots based on their GFN to account for an added, deleted, or
 * moved memslot.  Sorting memslots by GFN allows using a binary search during
 * memslot lookup.
 *
 * IMPORTANT: Slots are sorted from highest GFN to lowest GFN!  I.e. the entry
 * at memslots[0] has the highest GFN.
 *
 * The sorting algorithm takes advantage of having initially sorted memslots
 * and knowing the position of the changed memslot.  Sorting is also optimized
 * by not swapping the updated memslot and instead only shifting other memslots
 * and tracking the new index for the update memslot.  Only once its final
 * index is known is the updated memslot copied into its position in the array.
 *
 *  - When deleting a memslot, the deleted memslot simply needs to be moved to
 *    the end of the array.
 *
 *  - When creating a memslot, the algorithm "inserts" the new memslot at the
 *    end of the array and then it forward to its correct location.
 *
 *  - When moving a memslot, the algorithm first moves the updated memslot
 *    backward to handle the scenario where the memslot's GFN was changed to a
 *    lower value.  update_memslots() then falls through and runs the same flow
 *    as creating a memslot to move the memslot forward to handle the scenario
 *    where its GFN was changed to a higher value.
 *
 * Note, slots are sorted from highest->lowest instead of lowest->highest for
 * historical reasons.  Originally, invalid memslots where denoted by having
 * GFN=0, thus sorting from highest->lowest naturally sorted invalid memslots
 * to the end of the array.  The current algorithm uses dedicated logic to
 * delete a memslot and thus does not rely on invalid memslots having GFN=0.
 *
 * The other historical motiviation for highest->lowest was to improve the
 * performance of memslot lookup.  KVM originally used a linear search starting
 * at memslots[0].  On x86, the largest memslot usually has one of the highest,
 * if not *the* highest, GFN, as the bulk of the guest's RAM is located in a
 * single memslot above the 4gb boundary.  As the largest memslot is also the
 * most likely to be referenced, sorting it to the front of the array was
 * advantageous.  The current binary search starts from the middle of the array
 * and uses an LRU pointer to improve performance for all memslots and GFNs.
 */
static void update_memslots(struct kvm_memslots *slots,
			    struct kvm_memory_slot *memslot,
			    enum kvm_mr_change change)
{
	int i;

	if (change == KVM_MR_DELETE) {
		kvm_memslot_delete(slots, memslot);
	} else {
		if (change == KVM_MR_CREATE)
			i = kvm_memslot_insert_back(slots);
		else
			i = kvm_memslot_move_backward(slots, memslot);
		i = kvm_memslot_move_forward(slots, memslot, i);

		/*
		 * Copy the memslot to its new position in memslots and update
		 * its index accordingly.
		 */
		slots->memslots[i] = *memslot;
		slots->id_to_index[memslot->id] = i;
	}
>>>>>>> upstream/android-13
}

static int check_memory_region_flags(const struct kvm_userspace_memory_region *mem)
{
	u32 valid_flags = KVM_MEM_LOG_DIRTY_PAGES;

#ifdef __KVM_HAVE_READONLY_MEM
	valid_flags |= KVM_MEM_READONLY;
#endif

	if (mem->flags & ~valid_flags)
		return -EINVAL;

	return 0;
}

static struct kvm_memslots *install_new_memslots(struct kvm *kvm,
		int as_id, struct kvm_memslots *slots)
{
	struct kvm_memslots *old_memslots = __kvm_memslots(kvm, as_id);
<<<<<<< HEAD
	u64 gen;

	/*
	 * Set the low bit in the generation, which disables SPTE caching
	 * until the end of synchronize_srcu_expedited.
	 */
	WARN_ON(old_memslots->generation & 1);
	slots->generation = old_memslots->generation + 1;

	rcu_assign_pointer(kvm->memslots[as_id], slots);
	synchronize_srcu_expedited(&kvm->srcu);

	/*
	 * Increment the new memslot generation a second time. This prevents
	 * vm exits that race with memslot updates from caching a memslot
	 * generation that will (potentially) be valid forever.
	 *
	 * Generations must be unique even across address spaces.  We do not need
	 * a global counter for that, instead the generation space is evenly split
	 * across address spaces.  For example, with two address spaces, address
	 * space 0 will use generations 0, 4, 8, ... while * address space 1 will
	 * use generations 2, 6, 10, 14, ...
	 */
	gen = slots->generation + KVM_ADDRESS_SPACE_NUM * 2 - 1;
=======
	u64 gen = old_memslots->generation;

	WARN_ON(gen & KVM_MEMSLOT_GEN_UPDATE_IN_PROGRESS);
	slots->generation = gen | KVM_MEMSLOT_GEN_UPDATE_IN_PROGRESS;

	/*
	 * Do not store the new memslots while there are invalidations in
	 * progress, otherwise the locking in invalidate_range_start and
	 * invalidate_range_end will be unbalanced.
	 */
	spin_lock(&kvm->mn_invalidate_lock);
	prepare_to_rcuwait(&kvm->mn_memslots_update_rcuwait);
	while (kvm->mn_active_invalidate_count) {
		set_current_state(TASK_UNINTERRUPTIBLE);
		spin_unlock(&kvm->mn_invalidate_lock);
		schedule();
		spin_lock(&kvm->mn_invalidate_lock);
	}
	finish_rcuwait(&kvm->mn_memslots_update_rcuwait);
	rcu_assign_pointer(kvm->memslots[as_id], slots);
	spin_unlock(&kvm->mn_invalidate_lock);

	/*
	 * Acquired in kvm_set_memslot. Must be released before synchronize
	 * SRCU below in order to avoid deadlock with another thread
	 * acquiring the slots_arch_lock in an srcu critical section.
	 */
	mutex_unlock(&kvm->slots_arch_lock);

	synchronize_srcu_expedited(&kvm->srcu);

	/*
	 * Increment the new memslot generation a second time, dropping the
	 * update in-progress flag and incrementing the generation based on
	 * the number of address spaces.  This provides a unique and easily
	 * identifiable generation number while the memslots are in flux.
	 */
	gen = slots->generation & ~KVM_MEMSLOT_GEN_UPDATE_IN_PROGRESS;

	/*
	 * Generations must be unique even across address spaces.  We do not need
	 * a global counter for that, instead the generation space is evenly split
	 * across address spaces.  For example, with two address spaces, address
	 * space 0 will use generations 0, 2, 4, ... while address space 1 will
	 * use generations 1, 3, 5, ...
	 */
	gen += KVM_ADDRESS_SPACE_NUM;
>>>>>>> upstream/android-13

	kvm_arch_memslots_updated(kvm, gen);

	slots->generation = gen;

	return old_memslots;
}

<<<<<<< HEAD
=======
static size_t kvm_memslots_size(int slots)
{
	return sizeof(struct kvm_memslots) +
	       (sizeof(struct kvm_memory_slot) * slots);
}

static void kvm_copy_memslots(struct kvm_memslots *to,
			      struct kvm_memslots *from)
{
	memcpy(to, from, kvm_memslots_size(from->used_slots));
}

/*
 * Note, at a minimum, the current number of used slots must be allocated, even
 * when deleting a memslot, as we need a complete duplicate of the memslots for
 * use when invalidating a memslot prior to deleting/moving the memslot.
 */
static struct kvm_memslots *kvm_dup_memslots(struct kvm_memslots *old,
					     enum kvm_mr_change change)
{
	struct kvm_memslots *slots;
	size_t new_size;

	if (change == KVM_MR_CREATE)
		new_size = kvm_memslots_size(old->used_slots + 1);
	else
		new_size = kvm_memslots_size(old->used_slots);

	slots = kvzalloc(new_size, GFP_KERNEL_ACCOUNT);
	if (likely(slots))
		kvm_copy_memslots(slots, old);

	return slots;
}

static int kvm_set_memslot(struct kvm *kvm,
			   const struct kvm_userspace_memory_region *mem,
			   struct kvm_memory_slot *new, int as_id,
			   enum kvm_mr_change change)
{
	struct kvm_memory_slot *slot, old;
	struct kvm_memslots *slots;
	int r;

	/*
	 * Released in install_new_memslots.
	 *
	 * Must be held from before the current memslots are copied until
	 * after the new memslots are installed with rcu_assign_pointer,
	 * then released before the synchronize srcu in install_new_memslots.
	 *
	 * When modifying memslots outside of the slots_lock, must be held
	 * before reading the pointer to the current memslots until after all
	 * changes to those memslots are complete.
	 *
	 * These rules ensure that installing new memslots does not lose
	 * changes made to the previous memslots.
	 */
	mutex_lock(&kvm->slots_arch_lock);

	slots = kvm_dup_memslots(__kvm_memslots(kvm, as_id), change);
	if (!slots) {
		mutex_unlock(&kvm->slots_arch_lock);
		return -ENOMEM;
	}

	if (change == KVM_MR_DELETE || change == KVM_MR_MOVE) {
		/*
		 * Note, the INVALID flag needs to be in the appropriate entry
		 * in the freshly allocated memslots, not in @old or @new.
		 */
		slot = id_to_memslot(slots, new->id);
		slot->flags |= KVM_MEMSLOT_INVALID;

		/*
		 * We can re-use the memory from the old memslots.
		 * It will be overwritten with a copy of the new memslots
		 * after reacquiring the slots_arch_lock below.
		 */
		slots = install_new_memslots(kvm, as_id, slots);

		/* From this point no new shadow pages pointing to a deleted,
		 * or moved, memslot will be created.
		 *
		 * validation of sp->gfn happens in:
		 *	- gfn_to_hva (kvm_read_guest, gfn_to_pfn)
		 *	- kvm_is_visible_gfn (mmu_check_root)
		 */
		kvm_arch_flush_shadow_memslot(kvm, slot);

		/* Released in install_new_memslots. */
		mutex_lock(&kvm->slots_arch_lock);

		/*
		 * The arch-specific fields of the memslots could have changed
		 * between releasing the slots_arch_lock in
		 * install_new_memslots and here, so get a fresh copy of the
		 * slots.
		 */
		kvm_copy_memslots(slots, __kvm_memslots(kvm, as_id));
	}

	/*
	 * Make a full copy of the old memslot, the pointer will become stale
	 * when the memslots are re-sorted by update_memslots(), and the old
	 * memslot needs to be referenced after calling update_memslots(), e.g.
	 * to free its resources and for arch specific behavior.  This needs to
	 * happen *after* (re)acquiring slots_arch_lock.
	 */
	slot = id_to_memslot(slots, new->id);
	if (slot) {
		old = *slot;
	} else {
		WARN_ON_ONCE(change != KVM_MR_CREATE);
		memset(&old, 0, sizeof(old));
		old.id = new->id;
		old.as_id = as_id;
	}

	/* Copy the arch-specific data, again after (re)acquiring slots_arch_lock. */
	memcpy(&new->arch, &old.arch, sizeof(old.arch));

	r = kvm_arch_prepare_memory_region(kvm, new, mem, change);
	if (r)
		goto out_slots;

	update_memslots(slots, new, change);
	slots = install_new_memslots(kvm, as_id, slots);

	kvm_arch_commit_memory_region(kvm, mem, &old, new, change);

	/* Free the old memslot's metadata.  Note, this is the full copy!!! */
	if (change == KVM_MR_DELETE)
		kvm_free_memslot(kvm, &old);

	kvfree(slots);
	return 0;

out_slots:
	if (change == KVM_MR_DELETE || change == KVM_MR_MOVE) {
		slot = id_to_memslot(slots, new->id);
		slot->flags &= ~KVM_MEMSLOT_INVALID;
		slots = install_new_memslots(kvm, as_id, slots);
	} else {
		mutex_unlock(&kvm->slots_arch_lock);
	}
	kvfree(slots);
	return r;
}

static int kvm_delete_memslot(struct kvm *kvm,
			      const struct kvm_userspace_memory_region *mem,
			      struct kvm_memory_slot *old, int as_id)
{
	struct kvm_memory_slot new;

	if (!old->npages)
		return -EINVAL;

	memset(&new, 0, sizeof(new));
	new.id = old->id;
	/*
	 * This is only for debugging purpose; it should never be referenced
	 * for a removed memslot.
	 */
	new.as_id = as_id;

	return kvm_set_memslot(kvm, mem, &new, as_id, KVM_MR_DELETE);
}

>>>>>>> upstream/android-13
/*
 * Allocate some memory and give it an address in the guest physical address
 * space.
 *
 * Discontiguous memory is allowed, mostly for framebuffers.
 *
 * Must be called holding kvm->slots_lock for write.
 */
int __kvm_set_memory_region(struct kvm *kvm,
			    const struct kvm_userspace_memory_region *mem)
{
<<<<<<< HEAD
	int r;
	gfn_t base_gfn;
	unsigned long npages;
	struct kvm_memory_slot *slot;
	struct kvm_memory_slot old, new;
	struct kvm_memslots *slots = NULL, *old_memslots;
	int as_id, id;
	enum kvm_mr_change change;

	r = check_memory_region_flags(mem);
	if (r)
		goto out;

	r = -EINVAL;
=======
	struct kvm_memory_slot old, new;
	struct kvm_memory_slot *tmp;
	enum kvm_mr_change change;
	int as_id, id;
	int r;

	r = check_memory_region_flags(mem);
	if (r)
		return r;

>>>>>>> upstream/android-13
	as_id = mem->slot >> 16;
	id = (u16)mem->slot;

	/* General sanity checks */
<<<<<<< HEAD
	if (mem->memory_size & (PAGE_SIZE - 1))
		goto out;
	if (mem->guest_phys_addr & (PAGE_SIZE - 1))
		goto out;
	/* We can read the guest memory with __xxx_user() later on. */
	if ((id < KVM_USER_MEM_SLOTS) &&
	    ((mem->userspace_addr & (PAGE_SIZE - 1)) ||
	     !access_ok(VERIFY_WRITE,
			(void __user *)(unsigned long)mem->userspace_addr,
			mem->memory_size)))
		goto out;
	if (as_id >= KVM_ADDRESS_SPACE_NUM || id >= KVM_MEM_SLOTS_NUM)
		goto out;
	if (mem->guest_phys_addr + mem->memory_size < mem->guest_phys_addr)
		goto out;

	slot = id_to_memslot(__kvm_memslots(kvm, as_id), id);
	base_gfn = mem->guest_phys_addr >> PAGE_SHIFT;
	npages = mem->memory_size >> PAGE_SHIFT;

	if (npages > KVM_MEM_MAX_NR_PAGES)
		goto out;

	new = old = *slot;

	new.id = id;
	new.base_gfn = base_gfn;
	new.npages = npages;
	new.flags = mem->flags;

	if (npages) {
		if (!old.npages)
			change = KVM_MR_CREATE;
		else { /* Modify an existing slot. */
			if ((mem->userspace_addr != old.userspace_addr) ||
			    (npages != old.npages) ||
			    ((new.flags ^ old.flags) & KVM_MEM_READONLY))
				goto out;

			if (base_gfn != old.base_gfn)
				change = KVM_MR_MOVE;
			else if (new.flags != old.flags)
				change = KVM_MR_FLAGS_ONLY;
			else { /* Nothing to change. */
				r = 0;
				goto out;
			}
		}
	} else {
		if (!old.npages)
			goto out;

		change = KVM_MR_DELETE;
		new.base_gfn = 0;
		new.flags = 0;
=======
	if ((mem->memory_size & (PAGE_SIZE - 1)) ||
	    (mem->memory_size != (unsigned long)mem->memory_size))
		return -EINVAL;
	if (mem->guest_phys_addr & (PAGE_SIZE - 1))
		return -EINVAL;
	/* We can read the guest memory with __xxx_user() later on. */
	if ((mem->userspace_addr & (PAGE_SIZE - 1)) ||
	    (mem->userspace_addr != untagged_addr(mem->userspace_addr)) ||
	     !access_ok((void __user *)(unsigned long)mem->userspace_addr,
			mem->memory_size))
		return -EINVAL;
	if (as_id >= KVM_ADDRESS_SPACE_NUM || id >= KVM_MEM_SLOTS_NUM)
		return -EINVAL;
	if (mem->guest_phys_addr + mem->memory_size < mem->guest_phys_addr)
		return -EINVAL;

	/*
	 * Make a full copy of the old memslot, the pointer will become stale
	 * when the memslots are re-sorted by update_memslots(), and the old
	 * memslot needs to be referenced after calling update_memslots(), e.g.
	 * to free its resources and for arch specific behavior.
	 */
	tmp = id_to_memslot(__kvm_memslots(kvm, as_id), id);
	if (tmp) {
		old = *tmp;
		tmp = NULL;
	} else {
		memset(&old, 0, sizeof(old));
		old.id = id;
	}

	if (!mem->memory_size)
		return kvm_delete_memslot(kvm, mem, &old, as_id);

	new.as_id = as_id;
	new.id = id;
	new.base_gfn = mem->guest_phys_addr >> PAGE_SHIFT;
	new.npages = mem->memory_size >> PAGE_SHIFT;
	new.flags = mem->flags;
	new.userspace_addr = mem->userspace_addr;

	if (new.npages > KVM_MEM_MAX_NR_PAGES)
		return -EINVAL;

	if (!old.npages) {
		change = KVM_MR_CREATE;
		new.dirty_bitmap = NULL;
	} else { /* Modify an existing slot. */
		if ((new.userspace_addr != old.userspace_addr) ||
		    (new.npages != old.npages) ||
		    ((new.flags ^ old.flags) & KVM_MEM_READONLY))
			return -EINVAL;

		if (new.base_gfn != old.base_gfn)
			change = KVM_MR_MOVE;
		else if (new.flags != old.flags)
			change = KVM_MR_FLAGS_ONLY;
		else /* Nothing to change. */
			return 0;

		/* Copy dirty_bitmap from the current memslot. */
		new.dirty_bitmap = old.dirty_bitmap;
>>>>>>> upstream/android-13
	}

	if ((change == KVM_MR_CREATE) || (change == KVM_MR_MOVE)) {
		/* Check for overlaps */
<<<<<<< HEAD
		r = -EEXIST;
		kvm_for_each_memslot(slot, __kvm_memslots(kvm, as_id)) {
			if (slot->id == id)
				continue;
			if (!((base_gfn + npages <= slot->base_gfn) ||
			      (base_gfn >= slot->base_gfn + slot->npages)))
				goto out;
		}
	}

	/* Free page dirty bitmap if unneeded */
	if (!(new.flags & KVM_MEM_LOG_DIRTY_PAGES))
		new.dirty_bitmap = NULL;

	r = -ENOMEM;
	if (change == KVM_MR_CREATE) {
		new.userspace_addr = mem->userspace_addr;

		if (kvm_arch_create_memslot(kvm, &new, npages))
			goto out_free;
	}

	/* Allocate page dirty bitmap if needed */
	if ((new.flags & KVM_MEM_LOG_DIRTY_PAGES) && !new.dirty_bitmap) {
		if (kvm_create_dirty_bitmap(&new) < 0)
			goto out_free;
	}

	slots = kvzalloc(sizeof(struct kvm_memslots), GFP_KERNEL);
	if (!slots)
		goto out_free;
	memcpy(slots, __kvm_memslots(kvm, as_id), sizeof(struct kvm_memslots));

	if ((change == KVM_MR_DELETE) || (change == KVM_MR_MOVE)) {
		slot = id_to_memslot(slots, id);
		slot->flags |= KVM_MEMSLOT_INVALID;

		old_memslots = install_new_memslots(kvm, as_id, slots);

		/* From this point no new shadow pages pointing to a deleted,
		 * or moved, memslot will be created.
		 *
		 * validation of sp->gfn happens in:
		 *	- gfn_to_hva (kvm_read_guest, gfn_to_pfn)
		 *	- kvm_is_visible_gfn (mmu_check_roots)
		 */
		kvm_arch_flush_shadow_memslot(kvm, slot);

		/*
		 * We can re-use the old_memslots from above, the only difference
		 * from the currently installed memslots is the invalid flag.  This
		 * will get overwritten by update_memslots anyway.
		 */
		slots = old_memslots;
	}

	r = kvm_arch_prepare_memory_region(kvm, &new, mem, change);
	if (r)
		goto out_slots;

	/* actual memory is freed via old in kvm_free_memslot below */
	if (change == KVM_MR_DELETE) {
		new.dirty_bitmap = NULL;
		memset(&new.arch, 0, sizeof(new.arch));
	}

	update_memslots(slots, &new);
	old_memslots = install_new_memslots(kvm, as_id, slots);

	kvm_arch_commit_memory_region(kvm, mem, &old, &new, change);

	kvm_free_memslot(kvm, &old, &new);
	kvfree(old_memslots);
	return 0;

out_slots:
	kvfree(slots);
out_free:
	kvm_free_memslot(kvm, &new, &old);
out:
=======
		kvm_for_each_memslot(tmp, __kvm_memslots(kvm, as_id)) {
			if (tmp->id == id)
				continue;
			if (!((new.base_gfn + new.npages <= tmp->base_gfn) ||
			      (new.base_gfn >= tmp->base_gfn + tmp->npages)))
				return -EEXIST;
		}
	}

	/* Allocate/free page dirty bitmap as needed */
	if (!(new.flags & KVM_MEM_LOG_DIRTY_PAGES))
		new.dirty_bitmap = NULL;
	else if (!new.dirty_bitmap && !kvm->dirty_ring_size) {
		r = kvm_alloc_dirty_bitmap(&new);
		if (r)
			return r;

		if (kvm_dirty_log_manual_protect_and_init_set(kvm))
			bitmap_set(new.dirty_bitmap, 0, new.npages);
	}

	r = kvm_set_memslot(kvm, mem, &new, as_id, change);
	if (r)
		goto out_bitmap;

	if (old.dirty_bitmap && !new.dirty_bitmap)
		kvm_destroy_dirty_bitmap(&old);
	return 0;

out_bitmap:
	if (new.dirty_bitmap && !old.dirty_bitmap)
		kvm_destroy_dirty_bitmap(&new);
>>>>>>> upstream/android-13
	return r;
}
EXPORT_SYMBOL_GPL(__kvm_set_memory_region);

int kvm_set_memory_region(struct kvm *kvm,
			  const struct kvm_userspace_memory_region *mem)
{
	int r;

	mutex_lock(&kvm->slots_lock);
	r = __kvm_set_memory_region(kvm, mem);
	mutex_unlock(&kvm->slots_lock);
	return r;
}
EXPORT_SYMBOL_GPL(kvm_set_memory_region);

static int kvm_vm_ioctl_set_memory_region(struct kvm *kvm,
					  struct kvm_userspace_memory_region *mem)
{
	if ((u16)mem->slot >= KVM_USER_MEM_SLOTS)
		return -EINVAL;

	return kvm_set_memory_region(kvm, mem);
}

<<<<<<< HEAD
int kvm_get_dirty_log(struct kvm *kvm,
			struct kvm_dirty_log *log, int *is_dirty)
{
	struct kvm_memslots *slots;
	struct kvm_memory_slot *memslot;
=======
#ifndef CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT
/**
 * kvm_get_dirty_log - get a snapshot of dirty pages
 * @kvm:	pointer to kvm instance
 * @log:	slot id and address to which we copy the log
 * @is_dirty:	set to '1' if any dirty pages were found
 * @memslot:	set to the associated memslot, always valid on success
 */
int kvm_get_dirty_log(struct kvm *kvm, struct kvm_dirty_log *log,
		      int *is_dirty, struct kvm_memory_slot **memslot)
{
	struct kvm_memslots *slots;
>>>>>>> upstream/android-13
	int i, as_id, id;
	unsigned long n;
	unsigned long any = 0;

<<<<<<< HEAD
=======
	/* Dirty ring tracking is exclusive to dirty log tracking */
	if (kvm->dirty_ring_size)
		return -ENXIO;

	*memslot = NULL;
	*is_dirty = 0;

>>>>>>> upstream/android-13
	as_id = log->slot >> 16;
	id = (u16)log->slot;
	if (as_id >= KVM_ADDRESS_SPACE_NUM || id >= KVM_USER_MEM_SLOTS)
		return -EINVAL;

	slots = __kvm_memslots(kvm, as_id);
<<<<<<< HEAD
	memslot = id_to_memslot(slots, id);
	if (!memslot->dirty_bitmap)
		return -ENOENT;

	n = kvm_dirty_bitmap_bytes(memslot);

	for (i = 0; !any && i < n/sizeof(long); ++i)
		any = memslot->dirty_bitmap[i];

	if (copy_to_user(log->dirty_bitmap, memslot->dirty_bitmap, n))
=======
	*memslot = id_to_memslot(slots, id);
	if (!(*memslot) || !(*memslot)->dirty_bitmap)
		return -ENOENT;

	kvm_arch_sync_dirty_log(kvm, *memslot);

	n = kvm_dirty_bitmap_bytes(*memslot);

	for (i = 0; !any && i < n/sizeof(long); ++i)
		any = (*memslot)->dirty_bitmap[i];

	if (copy_to_user(log->dirty_bitmap, (*memslot)->dirty_bitmap, n))
>>>>>>> upstream/android-13
		return -EFAULT;

	if (any)
		*is_dirty = 1;
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_get_dirty_log);

<<<<<<< HEAD
#ifdef CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT
/**
 * kvm_get_dirty_log_protect - get a snapshot of dirty pages, and if any pages
 *	are dirty write protect them for next write.
 * @kvm:	pointer to kvm instance
 * @log:	slot id and address to which we copy the log
 * @is_dirty:	flag set if any page is dirty
=======
#else /* CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT */
/**
 * kvm_get_dirty_log_protect - get a snapshot of dirty pages
 *	and reenable dirty page tracking for the corresponding pages.
 * @kvm:	pointer to kvm instance
 * @log:	slot id and address to which we copy the log
>>>>>>> upstream/android-13
 *
 * We need to keep it in mind that VCPU threads can write to the bitmap
 * concurrently. So, to avoid losing track of dirty pages we keep the
 * following order:
 *
 *    1. Take a snapshot of the bit and clear it if needed.
 *    2. Write protect the corresponding page.
 *    3. Copy the snapshot to the userspace.
 *    4. Upon return caller flushes TLB's if needed.
 *
 * Between 2 and 4, the guest may write to the page using the remaining TLB
 * entry.  This is not a problem because the page is reported dirty using
 * the snapshot taken before and step 4 ensures that writes done after
 * exiting to userspace will be logged for the next call.
 *
 */
<<<<<<< HEAD
int kvm_get_dirty_log_protect(struct kvm *kvm,
			struct kvm_dirty_log *log, bool *is_dirty)
=======
static int kvm_get_dirty_log_protect(struct kvm *kvm, struct kvm_dirty_log *log)
>>>>>>> upstream/android-13
{
	struct kvm_memslots *slots;
	struct kvm_memory_slot *memslot;
	int i, as_id, id;
	unsigned long n;
	unsigned long *dirty_bitmap;
	unsigned long *dirty_bitmap_buffer;
<<<<<<< HEAD
=======
	bool flush;

	/* Dirty ring tracking is exclusive to dirty log tracking */
	if (kvm->dirty_ring_size)
		return -ENXIO;
>>>>>>> upstream/android-13

	as_id = log->slot >> 16;
	id = (u16)log->slot;
	if (as_id >= KVM_ADDRESS_SPACE_NUM || id >= KVM_USER_MEM_SLOTS)
		return -EINVAL;

	slots = __kvm_memslots(kvm, as_id);
	memslot = id_to_memslot(slots, id);
<<<<<<< HEAD

	dirty_bitmap = memslot->dirty_bitmap;
	if (!dirty_bitmap)
		return -ENOENT;

	n = kvm_dirty_bitmap_bytes(memslot);

	dirty_bitmap_buffer = kvm_second_dirty_bitmap(memslot);
	memset(dirty_bitmap_buffer, 0, n);

	spin_lock(&kvm->mmu_lock);
	*is_dirty = false;
	for (i = 0; i < n / sizeof(long); i++) {
		unsigned long mask;
		gfn_t offset;

		if (!dirty_bitmap[i])
			continue;

		*is_dirty = true;

		mask = xchg(&dirty_bitmap[i], 0);
		dirty_bitmap_buffer[i] = mask;

		if (mask) {
=======
	if (!memslot || !memslot->dirty_bitmap)
		return -ENOENT;

	dirty_bitmap = memslot->dirty_bitmap;

	kvm_arch_sync_dirty_log(kvm, memslot);

	n = kvm_dirty_bitmap_bytes(memslot);
	flush = false;
	if (kvm->manual_dirty_log_protect) {
		/*
		 * Unlike kvm_get_dirty_log, we always return false in *flush,
		 * because no flush is needed until KVM_CLEAR_DIRTY_LOG.  There
		 * is some code duplication between this function and
		 * kvm_get_dirty_log, but hopefully all architecture
		 * transition to kvm_get_dirty_log_protect and kvm_get_dirty_log
		 * can be eliminated.
		 */
		dirty_bitmap_buffer = dirty_bitmap;
	} else {
		dirty_bitmap_buffer = kvm_second_dirty_bitmap(memslot);
		memset(dirty_bitmap_buffer, 0, n);

		KVM_MMU_LOCK(kvm);
		for (i = 0; i < n / sizeof(long); i++) {
			unsigned long mask;
			gfn_t offset;

			if (!dirty_bitmap[i])
				continue;

			flush = true;
			mask = xchg(&dirty_bitmap[i], 0);
			dirty_bitmap_buffer[i] = mask;

>>>>>>> upstream/android-13
			offset = i * BITS_PER_LONG;
			kvm_arch_mmu_enable_log_dirty_pt_masked(kvm, memslot,
								offset, mask);
		}
<<<<<<< HEAD
	}

	spin_unlock(&kvm->mmu_lock);
=======
		KVM_MMU_UNLOCK(kvm);
	}

	if (flush)
		kvm_arch_flush_remote_tlbs_memslot(kvm, memslot);

>>>>>>> upstream/android-13
	if (copy_to_user(log->dirty_bitmap, dirty_bitmap_buffer, n))
		return -EFAULT;
	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(kvm_get_dirty_log_protect);
#endif

bool kvm_largepages_enabled(void)
{
	return largepages_enabled;
}

void kvm_disable_largepages(void)
{
	largepages_enabled = false;
}
EXPORT_SYMBOL_GPL(kvm_disable_largepages);
=======


/**
 * kvm_vm_ioctl_get_dirty_log - get and clear the log of dirty pages in a slot
 * @kvm: kvm instance
 * @log: slot id and address to which we copy the log
 *
 * Steps 1-4 below provide general overview of dirty page logging. See
 * kvm_get_dirty_log_protect() function description for additional details.
 *
 * We call kvm_get_dirty_log_protect() to handle steps 1-3, upon return we
 * always flush the TLB (step 4) even if previous step failed  and the dirty
 * bitmap may be corrupt. Regardless of previous outcome the KVM logging API
 * does not preclude user space subsequent dirty log read. Flushing TLB ensures
 * writes will be marked dirty for next log read.
 *
 *   1. Take a snapshot of the bit and clear it if needed.
 *   2. Write protect the corresponding page.
 *   3. Copy the snapshot to the userspace.
 *   4. Flush TLB's if needed.
 */
static int kvm_vm_ioctl_get_dirty_log(struct kvm *kvm,
				      struct kvm_dirty_log *log)
{
	int r;

	mutex_lock(&kvm->slots_lock);

	r = kvm_get_dirty_log_protect(kvm, log);

	mutex_unlock(&kvm->slots_lock);
	return r;
}

/**
 * kvm_clear_dirty_log_protect - clear dirty bits in the bitmap
 *	and reenable dirty page tracking for the corresponding pages.
 * @kvm:	pointer to kvm instance
 * @log:	slot id and address from which to fetch the bitmap of dirty pages
 */
static int kvm_clear_dirty_log_protect(struct kvm *kvm,
				       struct kvm_clear_dirty_log *log)
{
	struct kvm_memslots *slots;
	struct kvm_memory_slot *memslot;
	int as_id, id;
	gfn_t offset;
	unsigned long i, n;
	unsigned long *dirty_bitmap;
	unsigned long *dirty_bitmap_buffer;
	bool flush;

	/* Dirty ring tracking is exclusive to dirty log tracking */
	if (kvm->dirty_ring_size)
		return -ENXIO;

	as_id = log->slot >> 16;
	id = (u16)log->slot;
	if (as_id >= KVM_ADDRESS_SPACE_NUM || id >= KVM_USER_MEM_SLOTS)
		return -EINVAL;

	if (log->first_page & 63)
		return -EINVAL;

	slots = __kvm_memslots(kvm, as_id);
	memslot = id_to_memslot(slots, id);
	if (!memslot || !memslot->dirty_bitmap)
		return -ENOENT;

	dirty_bitmap = memslot->dirty_bitmap;

	n = ALIGN(log->num_pages, BITS_PER_LONG) / 8;

	if (log->first_page > memslot->npages ||
	    log->num_pages > memslot->npages - log->first_page ||
	    (log->num_pages < memslot->npages - log->first_page && (log->num_pages & 63)))
	    return -EINVAL;

	kvm_arch_sync_dirty_log(kvm, memslot);

	flush = false;
	dirty_bitmap_buffer = kvm_second_dirty_bitmap(memslot);
	if (copy_from_user(dirty_bitmap_buffer, log->dirty_bitmap, n))
		return -EFAULT;

	KVM_MMU_LOCK(kvm);
	for (offset = log->first_page, i = offset / BITS_PER_LONG,
		 n = DIV_ROUND_UP(log->num_pages, BITS_PER_LONG); n--;
	     i++, offset += BITS_PER_LONG) {
		unsigned long mask = *dirty_bitmap_buffer++;
		atomic_long_t *p = (atomic_long_t *) &dirty_bitmap[i];
		if (!mask)
			continue;

		mask &= atomic_long_fetch_andnot(mask, p);

		/*
		 * mask contains the bits that really have been cleared.  This
		 * never includes any bits beyond the length of the memslot (if
		 * the length is not aligned to 64 pages), therefore it is not
		 * a problem if userspace sets them in log->dirty_bitmap.
		*/
		if (mask) {
			flush = true;
			kvm_arch_mmu_enable_log_dirty_pt_masked(kvm, memslot,
								offset, mask);
		}
	}
	KVM_MMU_UNLOCK(kvm);

	if (flush)
		kvm_arch_flush_remote_tlbs_memslot(kvm, memslot);

	return 0;
}

static int kvm_vm_ioctl_clear_dirty_log(struct kvm *kvm,
					struct kvm_clear_dirty_log *log)
{
	int r;

	mutex_lock(&kvm->slots_lock);

	r = kvm_clear_dirty_log_protect(kvm, log);

	mutex_unlock(&kvm->slots_lock);
	return r;
}
#endif /* CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT */
>>>>>>> upstream/android-13

struct kvm_memory_slot *gfn_to_memslot(struct kvm *kvm, gfn_t gfn)
{
	return __gfn_to_memslot(kvm_memslots(kvm), gfn);
}
EXPORT_SYMBOL_GPL(gfn_to_memslot);

struct kvm_memory_slot *kvm_vcpu_gfn_to_memslot(struct kvm_vcpu *vcpu, gfn_t gfn)
{
<<<<<<< HEAD
	return __gfn_to_memslot(kvm_vcpu_memslots(vcpu), gfn);
=======
	struct kvm_memslots *slots = kvm_vcpu_memslots(vcpu);
	struct kvm_memory_slot *slot;
	int slot_index;

	slot = try_get_memslot(slots, vcpu->last_used_slot, gfn);
	if (slot)
		return slot;

	/*
	 * Fall back to searching all memslots. We purposely use
	 * search_memslots() instead of __gfn_to_memslot() to avoid
	 * thrashing the VM-wide last_used_index in kvm_memslots.
	 */
	slot = search_memslots(slots, gfn, &slot_index);
	if (slot) {
		vcpu->last_used_slot = slot_index;
		return slot;
	}

	return NULL;
>>>>>>> upstream/android-13
}

bool kvm_is_visible_gfn(struct kvm *kvm, gfn_t gfn)
{
	struct kvm_memory_slot *memslot = gfn_to_memslot(kvm, gfn);

<<<<<<< HEAD
	if (!memslot || memslot->id >= KVM_USER_MEM_SLOTS ||
	      memslot->flags & KVM_MEMSLOT_INVALID)
		return false;

	return true;
}
EXPORT_SYMBOL_GPL(kvm_is_visible_gfn);

=======
	return kvm_is_visible_memslot(memslot);
}
EXPORT_SYMBOL_GPL(kvm_is_visible_gfn);

bool kvm_vcpu_is_visible_gfn(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	struct kvm_memory_slot *memslot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);

	return kvm_is_visible_memslot(memslot);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_is_visible_gfn);

>>>>>>> upstream/android-13
unsigned long kvm_host_page_size(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	struct vm_area_struct *vma;
	unsigned long addr, size;

	size = PAGE_SIZE;

	addr = kvm_vcpu_gfn_to_hva_prot(vcpu, gfn, NULL);
	if (kvm_is_error_hva(addr))
		return PAGE_SIZE;

<<<<<<< HEAD
	down_read(&current->mm->mmap_sem);
=======
	mmap_read_lock(current->mm);
>>>>>>> upstream/android-13
	vma = find_vma(current->mm, addr);
	if (!vma)
		goto out;

	size = vma_kernel_pagesize(vma);

out:
<<<<<<< HEAD
	up_read(&current->mm->mmap_sem);
=======
	mmap_read_unlock(current->mm);
>>>>>>> upstream/android-13

	return size;
}

static bool memslot_is_readonly(struct kvm_memory_slot *slot)
{
	return slot->flags & KVM_MEM_READONLY;
}

static unsigned long __gfn_to_hva_many(struct kvm_memory_slot *slot, gfn_t gfn,
				       gfn_t *nr_pages, bool write)
{
	if (!slot || slot->flags & KVM_MEMSLOT_INVALID)
		return KVM_HVA_ERR_BAD;

	if (memslot_is_readonly(slot) && write)
		return KVM_HVA_ERR_RO_BAD;

	if (nr_pages)
		*nr_pages = slot->npages - (gfn - slot->base_gfn);

	return __gfn_to_hva_memslot(slot, gfn);
}

static unsigned long gfn_to_hva_many(struct kvm_memory_slot *slot, gfn_t gfn,
				     gfn_t *nr_pages)
{
	return __gfn_to_hva_many(slot, gfn, nr_pages, true);
}

unsigned long gfn_to_hva_memslot(struct kvm_memory_slot *slot,
					gfn_t gfn)
{
	return gfn_to_hva_many(slot, gfn, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_hva_memslot);

unsigned long gfn_to_hva(struct kvm *kvm, gfn_t gfn)
{
	return gfn_to_hva_many(gfn_to_memslot(kvm, gfn), gfn, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_hva);

unsigned long kvm_vcpu_gfn_to_hva(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	return gfn_to_hva_many(kvm_vcpu_gfn_to_memslot(vcpu, gfn), gfn, NULL);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_gfn_to_hva);

/*
<<<<<<< HEAD
 * If writable is set to false, the hva returned by this function is only
 * allowed to be read.
=======
 * Return the hva of a @gfn and the R/W attribute if possible.
 *
 * @slot: the kvm_memory_slot which contains @gfn
 * @gfn: the gfn to be translated
 * @writable: used to return the read/write attribute of the @slot if the hva
 * is valid and @writable is not NULL
>>>>>>> upstream/android-13
 */
unsigned long gfn_to_hva_memslot_prot(struct kvm_memory_slot *slot,
				      gfn_t gfn, bool *writable)
{
	unsigned long hva = __gfn_to_hva_many(slot, gfn, NULL, false);

	if (!kvm_is_error_hva(hva) && writable)
		*writable = !memslot_is_readonly(slot);

	return hva;
}

unsigned long gfn_to_hva_prot(struct kvm *kvm, gfn_t gfn, bool *writable)
{
	struct kvm_memory_slot *slot = gfn_to_memslot(kvm, gfn);

	return gfn_to_hva_memslot_prot(slot, gfn, writable);
}

unsigned long kvm_vcpu_gfn_to_hva_prot(struct kvm_vcpu *vcpu, gfn_t gfn, bool *writable)
{
	struct kvm_memory_slot *slot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);

	return gfn_to_hva_memslot_prot(slot, gfn, writable);
}

static inline int check_user_page_hwpoison(unsigned long addr)
{
	int rc, flags = FOLL_HWPOISON | FOLL_WRITE;

	rc = get_user_pages(addr, 1, flags, NULL, NULL);
	return rc == -EHWPOISON;
}

/*
 * The fast path to get the writable pfn which will be stored in @pfn,
 * true indicates success, otherwise false is returned.  It's also the
<<<<<<< HEAD
 * only part that runs if we can are in atomic context.
=======
 * only part that runs if we can in atomic context.
>>>>>>> upstream/android-13
 */
static bool hva_to_pfn_fast(unsigned long addr, bool write_fault,
			    bool *writable, kvm_pfn_t *pfn)
{
	struct page *page[1];
<<<<<<< HEAD
	int npages;
=======
>>>>>>> upstream/android-13

	/*
	 * Fast pin a writable pfn only if it is a write fault request
	 * or the caller allows to map a writable pfn for a read fault
	 * request.
	 */
	if (!(write_fault || writable))
		return false;

<<<<<<< HEAD
	npages = __get_user_pages_fast(addr, 1, 1, page);
	if (npages == 1) {
=======
	if (get_user_page_fast_only(addr, FOLL_WRITE, page)) {
>>>>>>> upstream/android-13
		*pfn = page_to_pfn(page[0]);

		if (writable)
			*writable = true;
		return true;
	}

	return false;
}

/*
 * The slow path to get the pfn of the specified host virtual address,
 * 1 indicates success, -errno is returned if error is detected.
 */
static int hva_to_pfn_slow(unsigned long addr, bool *async, bool write_fault,
			   bool *writable, kvm_pfn_t *pfn)
{
	unsigned int flags = FOLL_HWPOISON;
	struct page *page;
	int npages = 0;

	might_sleep();

	if (writable)
		*writable = write_fault;

	if (write_fault)
		flags |= FOLL_WRITE;
	if (async)
		flags |= FOLL_NOWAIT;

	npages = get_user_pages_unlocked(addr, 1, &page, flags);
	if (npages != 1)
		return npages;

	/* map read fault as writable if possible */
	if (unlikely(!write_fault) && writable) {
		struct page *wpage;

<<<<<<< HEAD
		if (__get_user_pages_fast(addr, 1, 1, &wpage) == 1) {
=======
		if (get_user_page_fast_only(addr, FOLL_WRITE, &wpage)) {
>>>>>>> upstream/android-13
			*writable = true;
			put_page(page);
			page = wpage;
		}
	}
	*pfn = page_to_pfn(page);
	return npages;
}

static bool vma_is_valid(struct vm_area_struct *vma, bool write_fault)
{
	if (unlikely(!(vma->vm_flags & VM_READ)))
		return false;

	if (write_fault && (unlikely(!(vma->vm_flags & VM_WRITE))))
		return false;

	return true;
}

<<<<<<< HEAD
=======
static int kvm_try_get_pfn(kvm_pfn_t pfn)
{
	if (kvm_is_reserved_pfn(pfn))
		return 1;
	return get_page_unless_zero(pfn_to_page(pfn));
}

>>>>>>> upstream/android-13
static int hva_to_pfn_remapped(struct vm_area_struct *vma,
			       unsigned long addr, bool *async,
			       bool write_fault, bool *writable,
			       kvm_pfn_t *p_pfn)
{
<<<<<<< HEAD
	unsigned long pfn;
	int r;

	r = follow_pfn(vma, addr, &pfn);
=======
	kvm_pfn_t pfn;
	pte_t *ptep;
	spinlock_t *ptl;
	int r;

	r = follow_pte(vma->vm_mm, addr, &ptep, &ptl);
>>>>>>> upstream/android-13
	if (r) {
		/*
		 * get_user_pages fails for VM_IO and VM_PFNMAP vmas and does
		 * not call the fault handler, so do it here.
		 */
		bool unlocked = false;
<<<<<<< HEAD
		r = fixup_user_fault(current, current->mm, addr,
=======
		r = fixup_user_fault(current->mm, addr,
>>>>>>> upstream/android-13
				     (write_fault ? FAULT_FLAG_WRITE : 0),
				     &unlocked);
		if (unlocked)
			return -EAGAIN;
		if (r)
			return r;

<<<<<<< HEAD
		r = follow_pfn(vma, addr, &pfn);
		if (r)
			return r;

	}

	if (writable)
		*writable = true;
=======
		r = follow_pte(vma->vm_mm, addr, &ptep, &ptl);
		if (r)
			return r;
	}

	if (write_fault && !pte_write(*ptep)) {
		pfn = KVM_PFN_ERR_RO_FAULT;
		goto out;
	}

	if (writable)
		*writable = pte_write(*ptep);
	pfn = pte_pfn(*ptep);
>>>>>>> upstream/android-13

	/*
	 * Get a reference here because callers of *hva_to_pfn* and
	 * *gfn_to_pfn* ultimately call kvm_release_pfn_clean on the
	 * returned pfn.  This is only needed if the VMA has VM_MIXEDMAP
<<<<<<< HEAD
	 * set, but the kvm_get_pfn/kvm_release_pfn_clean pair will
=======
	 * set, but the kvm_try_get_pfn/kvm_release_pfn_clean pair will
>>>>>>> upstream/android-13
	 * simply do nothing for reserved pfns.
	 *
	 * Whoever called remap_pfn_range is also going to call e.g.
	 * unmap_mapping_range before the underlying pages are freed,
	 * causing a call to our MMU notifier.
<<<<<<< HEAD
	 */ 
	kvm_get_pfn(pfn);

	*p_pfn = pfn;
	return 0;
=======
	 *
	 * Certain IO or PFNMAP mappings can be backed with valid
	 * struct pages, but be allocated without refcounting e.g.,
	 * tail pages of non-compound higher order allocations, which
	 * would then underflow the refcount when the caller does the
	 * required put_page. Don't allow those pages here.
	 */ 
	if (!kvm_try_get_pfn(pfn))
		r = -EFAULT;

out:
	pte_unmap_unlock(ptep, ptl);
	*p_pfn = pfn;

	return r;
>>>>>>> upstream/android-13
}

/*
 * Pin guest page in memory and return its pfn.
 * @addr: host virtual address which maps memory to the guest
 * @atomic: whether this function can sleep
 * @async: whether this function need to wait IO complete if the
 *         host page is not in the memory
 * @write_fault: whether we should get a writable host page
 * @writable: whether it allows to map a writable host page for !@write_fault
 *
 * The function will map a writable host page for these two cases:
 * 1): @write_fault = true
 * 2): @write_fault = false && @writable, @writable will tell the caller
 *     whether the mapping is writable.
 */
static kvm_pfn_t hva_to_pfn(unsigned long addr, bool atomic, bool *async,
			bool write_fault, bool *writable)
{
	struct vm_area_struct *vma;
	kvm_pfn_t pfn = 0;
	int npages, r;

	/* we can do it either atomically or asynchronously, not both */
	BUG_ON(atomic && async);

	if (hva_to_pfn_fast(addr, write_fault, writable, &pfn))
		return pfn;

	if (atomic)
		return KVM_PFN_ERR_FAULT;

	npages = hva_to_pfn_slow(addr, async, write_fault, writable, &pfn);
	if (npages == 1)
		return pfn;

<<<<<<< HEAD
	down_read(&current->mm->mmap_sem);
=======
	mmap_read_lock(current->mm);
>>>>>>> upstream/android-13
	if (npages == -EHWPOISON ||
	      (!async && check_user_page_hwpoison(addr))) {
		pfn = KVM_PFN_ERR_HWPOISON;
		goto exit;
	}

retry:
<<<<<<< HEAD
	vma = find_vma_intersection(current->mm, addr, addr + 1);
=======
	vma = vma_lookup(current->mm, addr);
>>>>>>> upstream/android-13

	if (vma == NULL)
		pfn = KVM_PFN_ERR_FAULT;
	else if (vma->vm_flags & (VM_IO | VM_PFNMAP)) {
		r = hva_to_pfn_remapped(vma, addr, async, write_fault, writable, &pfn);
		if (r == -EAGAIN)
			goto retry;
		if (r < 0)
			pfn = KVM_PFN_ERR_FAULT;
	} else {
		if (async && vma_is_valid(vma, write_fault))
			*async = true;
		pfn = KVM_PFN_ERR_FAULT;
	}
exit:
<<<<<<< HEAD
	up_read(&current->mm->mmap_sem);
=======
	mmap_read_unlock(current->mm);
>>>>>>> upstream/android-13
	return pfn;
}

kvm_pfn_t __gfn_to_pfn_memslot(struct kvm_memory_slot *slot, gfn_t gfn,
			       bool atomic, bool *async, bool write_fault,
<<<<<<< HEAD
			       bool *writable)
{
	unsigned long addr = __gfn_to_hva_many(slot, gfn, NULL, write_fault);

=======
			       bool *writable, hva_t *hva)
{
	unsigned long addr = __gfn_to_hva_many(slot, gfn, NULL, write_fault);

	if (hva)
		*hva = addr;

>>>>>>> upstream/android-13
	if (addr == KVM_HVA_ERR_RO_BAD) {
		if (writable)
			*writable = false;
		return KVM_PFN_ERR_RO_FAULT;
	}

	if (kvm_is_error_hva(addr)) {
		if (writable)
			*writable = false;
		return KVM_PFN_NOSLOT;
	}

	/* Do not map writable pfn in the readonly memslot. */
	if (writable && memslot_is_readonly(slot)) {
		*writable = false;
		writable = NULL;
	}

	return hva_to_pfn(addr, atomic, async, write_fault,
			  writable);
}
EXPORT_SYMBOL_GPL(__gfn_to_pfn_memslot);

kvm_pfn_t gfn_to_pfn_prot(struct kvm *kvm, gfn_t gfn, bool write_fault,
		      bool *writable)
{
	return __gfn_to_pfn_memslot(gfn_to_memslot(kvm, gfn), gfn, false, NULL,
<<<<<<< HEAD
				    write_fault, writable);
=======
				    write_fault, writable, NULL);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_prot);

kvm_pfn_t gfn_to_pfn_memslot(struct kvm_memory_slot *slot, gfn_t gfn)
{
<<<<<<< HEAD
	return __gfn_to_pfn_memslot(slot, gfn, false, NULL, true, NULL);
=======
	return __gfn_to_pfn_memslot(slot, gfn, false, NULL, true, NULL, NULL);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_memslot);

kvm_pfn_t gfn_to_pfn_memslot_atomic(struct kvm_memory_slot *slot, gfn_t gfn)
{
<<<<<<< HEAD
	return __gfn_to_pfn_memslot(slot, gfn, true, NULL, true, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_memslot_atomic);

kvm_pfn_t gfn_to_pfn_atomic(struct kvm *kvm, gfn_t gfn)
{
	return gfn_to_pfn_memslot_atomic(gfn_to_memslot(kvm, gfn), gfn);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_atomic);

=======
	return __gfn_to_pfn_memslot(slot, gfn, true, NULL, true, NULL, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_memslot_atomic);

>>>>>>> upstream/android-13
kvm_pfn_t kvm_vcpu_gfn_to_pfn_atomic(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	return gfn_to_pfn_memslot_atomic(kvm_vcpu_gfn_to_memslot(vcpu, gfn), gfn);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_gfn_to_pfn_atomic);

kvm_pfn_t gfn_to_pfn(struct kvm *kvm, gfn_t gfn)
{
	return gfn_to_pfn_memslot(gfn_to_memslot(kvm, gfn), gfn);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn);

kvm_pfn_t kvm_vcpu_gfn_to_pfn(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	return gfn_to_pfn_memslot(kvm_vcpu_gfn_to_memslot(vcpu, gfn), gfn);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_gfn_to_pfn);

int gfn_to_page_many_atomic(struct kvm_memory_slot *slot, gfn_t gfn,
			    struct page **pages, int nr_pages)
{
	unsigned long addr;
	gfn_t entry = 0;

	addr = gfn_to_hva_many(slot, gfn, &entry);
	if (kvm_is_error_hva(addr))
		return -1;

	if (entry < nr_pages)
		return 0;

<<<<<<< HEAD
	return __get_user_pages_fast(addr, nr_pages, 1, pages);
=======
	return get_user_pages_fast_only(addr, nr_pages, FOLL_WRITE, pages);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(gfn_to_page_many_atomic);

static struct page *kvm_pfn_to_page(kvm_pfn_t pfn)
{
	if (is_error_noslot_pfn(pfn))
		return KVM_ERR_PTR_BAD_PAGE;

	if (kvm_is_reserved_pfn(pfn)) {
		WARN_ON(1);
		return KVM_ERR_PTR_BAD_PAGE;
	}

	return pfn_to_page(pfn);
}

struct page *gfn_to_page(struct kvm *kvm, gfn_t gfn)
{
	kvm_pfn_t pfn;

	pfn = gfn_to_pfn(kvm, gfn);

	return kvm_pfn_to_page(pfn);
}
EXPORT_SYMBOL_GPL(gfn_to_page);

void kvm_release_pfn(kvm_pfn_t pfn, bool dirty, struct gfn_to_pfn_cache *cache)
{
	if (pfn == 0)
		return;

	if (cache)
		cache->pfn = cache->gfn = 0;

	if (dirty)
		kvm_release_pfn_dirty(pfn);
	else
		kvm_release_pfn_clean(pfn);
}

static void kvm_cache_gfn_to_pfn(struct kvm_memory_slot *slot, gfn_t gfn,
				 struct gfn_to_pfn_cache *cache, u64 gen)
{
	kvm_release_pfn(cache->pfn, cache->dirty, cache);

	cache->pfn = gfn_to_pfn_memslot(slot, gfn);
	cache->gfn = gfn;
	cache->dirty = false;
	cache->generation = gen;
}

static int __kvm_map_gfn(struct kvm_memslots *slots, gfn_t gfn,
			 struct kvm_host_map *map,
			 struct gfn_to_pfn_cache *cache,
			 bool atomic)
{
	kvm_pfn_t pfn;
	void *hva = NULL;
	struct page *page = KVM_UNMAPPED_PAGE;
	struct kvm_memory_slot *slot = __gfn_to_memslot(slots, gfn);
	u64 gen = slots->generation;

	if (!map)
		return -EINVAL;

	if (cache) {
		if (!cache->pfn || cache->gfn != gfn ||
			cache->generation != gen) {
			if (atomic)
				return -EAGAIN;
			kvm_cache_gfn_to_pfn(slot, gfn, cache, gen);
		}
		pfn = cache->pfn;
	} else {
		if (atomic)
			return -EAGAIN;
		pfn = gfn_to_pfn_memslot(slot, gfn);
	}
	if (is_error_noslot_pfn(pfn))
		return -EINVAL;

	if (pfn_valid(pfn)) {
		page = pfn_to_page(pfn);
		if (atomic)
			hva = kmap_atomic(page);
		else
			hva = kmap(page);
#ifdef CONFIG_HAS_IOMEM
	} else if (!atomic) {
		hva = memremap(pfn_to_hpa(pfn), PAGE_SIZE, MEMREMAP_WB);
	} else {
		return -EINVAL;
#endif
	}

	if (!hva)
		return -EFAULT;

	map->page = page;
	map->hva = hva;
	map->pfn = pfn;
	map->gfn = gfn;

	return 0;
}

int kvm_map_gfn(struct kvm_vcpu *vcpu, gfn_t gfn, struct kvm_host_map *map,
		struct gfn_to_pfn_cache *cache, bool atomic)
{
	return __kvm_map_gfn(kvm_memslots(vcpu->kvm), gfn, map,
			cache, atomic);
}
EXPORT_SYMBOL_GPL(kvm_map_gfn);

int kvm_vcpu_map(struct kvm_vcpu *vcpu, gfn_t gfn, struct kvm_host_map *map)
{
	return __kvm_map_gfn(kvm_vcpu_memslots(vcpu), gfn, map,
		NULL, false);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_map);

<<<<<<< HEAD
static void __kvm_unmap_gfn(struct kvm_memory_slot *memslot,
=======
static void __kvm_unmap_gfn(struct kvm *kvm,
			struct kvm_memory_slot *memslot,
>>>>>>> upstream/android-13
			struct kvm_host_map *map,
			struct gfn_to_pfn_cache *cache,
			bool dirty, bool atomic)
{
	if (!map)
		return;

	if (!map->hva)
		return;

	if (map->page != KVM_UNMAPPED_PAGE) {
		if (atomic)
			kunmap_atomic(map->hva);
		else
			kunmap(map->page);
	}
#ifdef CONFIG_HAS_IOMEM
	else if (!atomic)
		memunmap(map->hva);
	else
		WARN_ONCE(1, "Unexpected unmapping in atomic context");
#endif

	if (dirty)
<<<<<<< HEAD
		mark_page_dirty_in_slot(memslot, map->gfn);
=======
		mark_page_dirty_in_slot(kvm, memslot, map->gfn);
>>>>>>> upstream/android-13

	if (cache)
		cache->dirty |= dirty;
	else
		kvm_release_pfn(map->pfn, dirty, NULL);

	map->hva = NULL;
	map->page = NULL;
}

int kvm_unmap_gfn(struct kvm_vcpu *vcpu, struct kvm_host_map *map, 
		  struct gfn_to_pfn_cache *cache, bool dirty, bool atomic)
{
<<<<<<< HEAD
	__kvm_unmap_gfn(gfn_to_memslot(vcpu->kvm, map->gfn), map,
=======
	__kvm_unmap_gfn(vcpu->kvm, gfn_to_memslot(vcpu->kvm, map->gfn), map,
>>>>>>> upstream/android-13
			cache, dirty, atomic);
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_unmap_gfn);

void kvm_vcpu_unmap(struct kvm_vcpu *vcpu, struct kvm_host_map *map, bool dirty)
{
<<<<<<< HEAD
	__kvm_unmap_gfn(kvm_vcpu_gfn_to_memslot(vcpu, map->gfn), map, NULL,
			dirty, false);
=======
	__kvm_unmap_gfn(vcpu->kvm, kvm_vcpu_gfn_to_memslot(vcpu, map->gfn),
			map, NULL, dirty, false);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(kvm_vcpu_unmap);

struct page *kvm_vcpu_gfn_to_page(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	kvm_pfn_t pfn;

	pfn = kvm_vcpu_gfn_to_pfn(vcpu, gfn);

	return kvm_pfn_to_page(pfn);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_gfn_to_page);

void kvm_release_page_clean(struct page *page)
{
	WARN_ON(is_error_page(page));

	kvm_release_pfn_clean(page_to_pfn(page));
}
EXPORT_SYMBOL_GPL(kvm_release_page_clean);

void kvm_release_pfn_clean(kvm_pfn_t pfn)
{
	if (!is_error_noslot_pfn(pfn) && !kvm_is_reserved_pfn(pfn))
		put_page(pfn_to_page(pfn));
}
EXPORT_SYMBOL_GPL(kvm_release_pfn_clean);

void kvm_release_page_dirty(struct page *page)
{
	WARN_ON(is_error_page(page));

	kvm_release_pfn_dirty(page_to_pfn(page));
}
EXPORT_SYMBOL_GPL(kvm_release_page_dirty);

void kvm_release_pfn_dirty(kvm_pfn_t pfn)
{
	kvm_set_pfn_dirty(pfn);
	kvm_release_pfn_clean(pfn);
}
EXPORT_SYMBOL_GPL(kvm_release_pfn_dirty);

void kvm_set_pfn_dirty(kvm_pfn_t pfn)
{
<<<<<<< HEAD
	if (!kvm_is_reserved_pfn(pfn) && !kvm_is_zone_device_pfn(pfn)) {
		struct page *page = pfn_to_page(pfn);

		if (!PageReserved(page))
			SetPageDirty(page);
	}
=======
	if (!kvm_is_reserved_pfn(pfn) && !kvm_is_zone_device_pfn(pfn))
		SetPageDirty(pfn_to_page(pfn));
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(kvm_set_pfn_dirty);

void kvm_set_pfn_accessed(kvm_pfn_t pfn)
{
	if (!kvm_is_reserved_pfn(pfn) && !kvm_is_zone_device_pfn(pfn))
		mark_page_accessed(pfn_to_page(pfn));
}
EXPORT_SYMBOL_GPL(kvm_set_pfn_accessed);

<<<<<<< HEAD
void kvm_get_pfn(kvm_pfn_t pfn)
{
	if (!kvm_is_reserved_pfn(pfn))
		get_page(pfn_to_page(pfn));
}
EXPORT_SYMBOL_GPL(kvm_get_pfn);

=======
>>>>>>> upstream/android-13
static int next_segment(unsigned long len, int offset)
{
	if (len > PAGE_SIZE - offset)
		return PAGE_SIZE - offset;
	else
		return len;
}

static int __kvm_read_guest_page(struct kvm_memory_slot *slot, gfn_t gfn,
				 void *data, int offset, int len)
{
	int r;
	unsigned long addr;

	addr = gfn_to_hva_memslot_prot(slot, gfn, NULL);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	r = __copy_from_user(data, (void __user *)addr + offset, len);
	if (r)
		return -EFAULT;
	return 0;
}

int kvm_read_guest_page(struct kvm *kvm, gfn_t gfn, void *data, int offset,
			int len)
{
	struct kvm_memory_slot *slot = gfn_to_memslot(kvm, gfn);

	return __kvm_read_guest_page(slot, gfn, data, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_read_guest_page);

int kvm_vcpu_read_guest_page(struct kvm_vcpu *vcpu, gfn_t gfn, void *data,
			     int offset, int len)
{
	struct kvm_memory_slot *slot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);

	return __kvm_read_guest_page(slot, gfn, data, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_read_guest_page);

int kvm_read_guest(struct kvm *kvm, gpa_t gpa, void *data, unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_read_guest_page(kvm, gfn, data, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		data += seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_read_guest);

int kvm_vcpu_read_guest(struct kvm_vcpu *vcpu, gpa_t gpa, void *data, unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_vcpu_read_guest_page(vcpu, gfn, data, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		data += seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_read_guest);

static int __kvm_read_guest_atomic(struct kvm_memory_slot *slot, gfn_t gfn,
			           void *data, int offset, unsigned long len)
{
	int r;
	unsigned long addr;

	addr = gfn_to_hva_memslot_prot(slot, gfn, NULL);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	pagefault_disable();
	r = __copy_from_user_inatomic(data, (void __user *)addr + offset, len);
	pagefault_enable();
	if (r)
		return -EFAULT;
	return 0;
}

<<<<<<< HEAD
int kvm_read_guest_atomic(struct kvm *kvm, gpa_t gpa, void *data,
			  unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	struct kvm_memory_slot *slot = gfn_to_memslot(kvm, gfn);
	int offset = offset_in_page(gpa);

	return __kvm_read_guest_atomic(slot, gfn, data, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_read_guest_atomic);

=======
>>>>>>> upstream/android-13
int kvm_vcpu_read_guest_atomic(struct kvm_vcpu *vcpu, gpa_t gpa,
			       void *data, unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	struct kvm_memory_slot *slot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);
	int offset = offset_in_page(gpa);

	return __kvm_read_guest_atomic(slot, gfn, data, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_read_guest_atomic);

<<<<<<< HEAD
static int __kvm_write_guest_page(struct kvm_memory_slot *memslot, gfn_t gfn,
=======
static int __kvm_write_guest_page(struct kvm *kvm,
				  struct kvm_memory_slot *memslot, gfn_t gfn,
>>>>>>> upstream/android-13
			          const void *data, int offset, int len)
{
	int r;
	unsigned long addr;

	addr = gfn_to_hva_memslot(memslot, gfn);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	r = __copy_to_user((void __user *)addr + offset, data, len);
	if (r)
		return -EFAULT;
<<<<<<< HEAD
	mark_page_dirty_in_slot(memslot, gfn);
=======
	mark_page_dirty_in_slot(kvm, memslot, gfn);
>>>>>>> upstream/android-13
	return 0;
}

int kvm_write_guest_page(struct kvm *kvm, gfn_t gfn,
			 const void *data, int offset, int len)
{
	struct kvm_memory_slot *slot = gfn_to_memslot(kvm, gfn);

<<<<<<< HEAD
	return __kvm_write_guest_page(slot, gfn, data, offset, len);
=======
	return __kvm_write_guest_page(kvm, slot, gfn, data, offset, len);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(kvm_write_guest_page);

int kvm_vcpu_write_guest_page(struct kvm_vcpu *vcpu, gfn_t gfn,
			      const void *data, int offset, int len)
{
	struct kvm_memory_slot *slot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);

<<<<<<< HEAD
	return __kvm_write_guest_page(slot, gfn, data, offset, len);
=======
	return __kvm_write_guest_page(vcpu->kvm, slot, gfn, data, offset, len);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(kvm_vcpu_write_guest_page);

int kvm_write_guest(struct kvm *kvm, gpa_t gpa, const void *data,
		    unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_write_guest_page(kvm, gfn, data, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		data += seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_write_guest);

int kvm_vcpu_write_guest(struct kvm_vcpu *vcpu, gpa_t gpa, const void *data,
		         unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_vcpu_write_guest_page(vcpu, gfn, data, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		data += seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_write_guest);

static int __kvm_gfn_to_hva_cache_init(struct kvm_memslots *slots,
				       struct gfn_to_hva_cache *ghc,
				       gpa_t gpa, unsigned long len)
{
	int offset = offset_in_page(gpa);
	gfn_t start_gfn = gpa >> PAGE_SHIFT;
	gfn_t end_gfn = (gpa + len - 1) >> PAGE_SHIFT;
	gfn_t nr_pages_needed = end_gfn - start_gfn + 1;
	gfn_t nr_pages_avail;

<<<<<<< HEAD
	ghc->gpa = gpa;
	ghc->generation = slots->generation;
	ghc->len = len;
	ghc->memslot = __gfn_to_memslot(slots, start_gfn);
	ghc->hva = gfn_to_hva_many(ghc->memslot, start_gfn, NULL);
	if (!kvm_is_error_hva(ghc->hva) && nr_pages_needed <= 1) {
		ghc->hva += offset;
	} else {
		/*
		 * If the requested region crosses two memslots, we still
		 * verify that the entire region is valid here.
		 */
		while (start_gfn <= end_gfn) {
			nr_pages_avail = 0;
			ghc->memslot = __gfn_to_memslot(slots, start_gfn);
			ghc->hva = gfn_to_hva_many(ghc->memslot, start_gfn,
						   &nr_pages_avail);
			if (kvm_is_error_hva(ghc->hva))
				return -EFAULT;
			start_gfn += nr_pages_avail;
		}
		/* Use the slow path for cross page reads and writes. */
		ghc->memslot = NULL;
	}
=======
	/* Update ghc->generation before performing any error checks. */
	ghc->generation = slots->generation;

	if (start_gfn > end_gfn) {
		ghc->hva = KVM_HVA_ERR_BAD;
		return -EINVAL;
	}

	/*
	 * If the requested region crosses two memslots, we still
	 * verify that the entire region is valid here.
	 */
	for ( ; start_gfn <= end_gfn; start_gfn += nr_pages_avail) {
		ghc->memslot = __gfn_to_memslot(slots, start_gfn);
		ghc->hva = gfn_to_hva_many(ghc->memslot, start_gfn,
					   &nr_pages_avail);
		if (kvm_is_error_hva(ghc->hva))
			return -EFAULT;
	}

	/* Use the slow path for cross page reads and writes. */
	if (nr_pages_needed == 1)
		ghc->hva += offset;
	else
		ghc->memslot = NULL;

	ghc->gpa = gpa;
	ghc->len = len;
>>>>>>> upstream/android-13
	return 0;
}

int kvm_gfn_to_hva_cache_init(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			      gpa_t gpa, unsigned long len)
{
	struct kvm_memslots *slots = kvm_memslots(kvm);
	return __kvm_gfn_to_hva_cache_init(slots, ghc, gpa, len);
}
EXPORT_SYMBOL_GPL(kvm_gfn_to_hva_cache_init);

int kvm_write_guest_offset_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
				  void *data, unsigned int offset,
				  unsigned long len)
{
	struct kvm_memslots *slots = kvm_memslots(kvm);
	int r;
	gpa_t gpa = ghc->gpa + offset;

<<<<<<< HEAD
	BUG_ON(len + offset > ghc->len);

	if (slots->generation != ghc->generation)
		__kvm_gfn_to_hva_cache_init(slots, ghc, ghc->gpa, ghc->len);
=======
	if (WARN_ON_ONCE(len + offset > ghc->len))
		return -EINVAL;

	if (slots->generation != ghc->generation) {
		if (__kvm_gfn_to_hva_cache_init(slots, ghc, ghc->gpa, ghc->len))
			return -EFAULT;
	}
>>>>>>> upstream/android-13

	if (kvm_is_error_hva(ghc->hva))
		return -EFAULT;

	if (unlikely(!ghc->memslot))
		return kvm_write_guest(kvm, gpa, data, len);

	r = __copy_to_user((void __user *)ghc->hva + offset, data, len);
	if (r)
		return -EFAULT;
<<<<<<< HEAD
	mark_page_dirty_in_slot(ghc->memslot, gpa >> PAGE_SHIFT);
=======
	mark_page_dirty_in_slot(kvm, ghc->memslot, gpa >> PAGE_SHIFT);
>>>>>>> upstream/android-13

	return 0;
}
EXPORT_SYMBOL_GPL(kvm_write_guest_offset_cached);

int kvm_write_guest_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			   void *data, unsigned long len)
{
	return kvm_write_guest_offset_cached(kvm, ghc, data, 0, len);
}
EXPORT_SYMBOL_GPL(kvm_write_guest_cached);

<<<<<<< HEAD
int kvm_read_guest_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			   void *data, unsigned long len)
{
	struct kvm_memslots *slots = kvm_memslots(kvm);
	int r;

	BUG_ON(len > ghc->len);

	if (slots->generation != ghc->generation)
		__kvm_gfn_to_hva_cache_init(slots, ghc, ghc->gpa, ghc->len);
=======
int kvm_read_guest_offset_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
				 void *data, unsigned int offset,
				 unsigned long len)
{
	struct kvm_memslots *slots = kvm_memslots(kvm);
	int r;
	gpa_t gpa = ghc->gpa + offset;

	if (WARN_ON_ONCE(len + offset > ghc->len))
		return -EINVAL;

	if (slots->generation != ghc->generation) {
		if (__kvm_gfn_to_hva_cache_init(slots, ghc, ghc->gpa, ghc->len))
			return -EFAULT;
	}
>>>>>>> upstream/android-13

	if (kvm_is_error_hva(ghc->hva))
		return -EFAULT;

	if (unlikely(!ghc->memslot))
<<<<<<< HEAD
		return kvm_read_guest(kvm, ghc->gpa, data, len);

	r = __copy_from_user(data, (void __user *)ghc->hva, len);
=======
		return kvm_read_guest(kvm, gpa, data, len);

	r = __copy_from_user(data, (void __user *)ghc->hva + offset, len);
>>>>>>> upstream/android-13
	if (r)
		return -EFAULT;

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(kvm_read_guest_cached);

int kvm_clear_guest_page(struct kvm *kvm, gfn_t gfn, int offset, int len)
{
	const void *zero_page = (const void *) __va(page_to_phys(ZERO_PAGE(0)));

	return kvm_write_guest_page(kvm, gfn, zero_page, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_clear_guest_page);

int kvm_clear_guest(struct kvm *kvm, gpa_t gpa, unsigned long len)
{
=======
EXPORT_SYMBOL_GPL(kvm_read_guest_offset_cached);

int kvm_read_guest_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			  void *data, unsigned long len)
{
	return kvm_read_guest_offset_cached(kvm, ghc, data, 0, len);
}
EXPORT_SYMBOL_GPL(kvm_read_guest_cached);

int kvm_clear_guest(struct kvm *kvm, gpa_t gpa, unsigned long len)
{
	const void *zero_page = (const void *) __va(page_to_phys(ZERO_PAGE(0)));
>>>>>>> upstream/android-13
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
<<<<<<< HEAD
		ret = kvm_clear_guest_page(kvm, gfn, offset, seg);
=======
		ret = kvm_write_guest_page(kvm, gfn, zero_page, offset, len);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_clear_guest);

<<<<<<< HEAD
static void mark_page_dirty_in_slot(struct kvm_memory_slot *memslot,
				    gfn_t gfn)
{
	if (memslot && memslot->dirty_bitmap) {
		unsigned long rel_gfn = gfn - memslot->base_gfn;

		set_bit_le(rel_gfn, memslot->dirty_bitmap);
	}
}
=======
void mark_page_dirty_in_slot(struct kvm *kvm,
			     struct kvm_memory_slot *memslot,
		 	     gfn_t gfn)
{
	if (memslot && kvm_slot_dirty_track_enabled(memslot)) {
		unsigned long rel_gfn = gfn - memslot->base_gfn;
		u32 slot = (memslot->as_id << 16) | memslot->id;

		if (kvm->dirty_ring_size)
			kvm_dirty_ring_push(kvm_dirty_ring_get(kvm),
					    slot, rel_gfn);
		else
			set_bit_le(rel_gfn, memslot->dirty_bitmap);
	}
}
EXPORT_SYMBOL_GPL(mark_page_dirty_in_slot);
>>>>>>> upstream/android-13

void mark_page_dirty(struct kvm *kvm, gfn_t gfn)
{
	struct kvm_memory_slot *memslot;

	memslot = gfn_to_memslot(kvm, gfn);
<<<<<<< HEAD
	mark_page_dirty_in_slot(memslot, gfn);
=======
	mark_page_dirty_in_slot(kvm, memslot, gfn);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(mark_page_dirty);

void kvm_vcpu_mark_page_dirty(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	struct kvm_memory_slot *memslot;

	memslot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);
<<<<<<< HEAD
	mark_page_dirty_in_slot(memslot, gfn);
=======
	mark_page_dirty_in_slot(vcpu->kvm, memslot, gfn);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(kvm_vcpu_mark_page_dirty);

void kvm_sigset_activate(struct kvm_vcpu *vcpu)
{
	if (!vcpu->sigset_active)
		return;

	/*
	 * This does a lockless modification of ->real_blocked, which is fine
	 * because, only current can change ->real_blocked and all readers of
	 * ->real_blocked don't care as long ->real_blocked is always a subset
	 * of ->blocked.
	 */
	sigprocmask(SIG_SETMASK, &vcpu->sigset, &current->real_blocked);
}

void kvm_sigset_deactivate(struct kvm_vcpu *vcpu)
{
	if (!vcpu->sigset_active)
		return;

	sigprocmask(SIG_SETMASK, &current->real_blocked, NULL);
	sigemptyset(&current->real_blocked);
}

static void grow_halt_poll_ns(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	unsigned int old, val, grow;

	old = val = vcpu->halt_poll_ns;
	grow = READ_ONCE(halt_poll_ns_grow);
	/* 10us base */
	if (val == 0 && grow)
		val = 10000;
	else
		val *= grow;

	if (val > halt_poll_ns)
		val = halt_poll_ns;

	vcpu->halt_poll_ns = val;
=======
	unsigned int old, val, grow, grow_start;

	old = val = vcpu->halt_poll_ns;
	grow_start = READ_ONCE(halt_poll_ns_grow_start);
	grow = READ_ONCE(halt_poll_ns_grow);
	if (!grow)
		goto out;

	val *= grow;
	if (val < grow_start)
		val = grow_start;

	if (val > vcpu->kvm->max_halt_poll_ns)
		val = vcpu->kvm->max_halt_poll_ns;

	vcpu->halt_poll_ns = val;
out:
>>>>>>> upstream/android-13
	trace_kvm_halt_poll_ns_grow(vcpu->vcpu_id, val, old);
}

static void shrink_halt_poll_ns(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	unsigned int old, val, shrink;

	old = val = vcpu->halt_poll_ns;
	shrink = READ_ONCE(halt_poll_ns_shrink);
=======
	unsigned int old, val, shrink, grow_start;

	old = val = vcpu->halt_poll_ns;
	shrink = READ_ONCE(halt_poll_ns_shrink);
	grow_start = READ_ONCE(halt_poll_ns_grow_start);
>>>>>>> upstream/android-13
	if (shrink == 0)
		val = 0;
	else
		val /= shrink;

<<<<<<< HEAD
=======
	if (val < grow_start)
		val = 0;

>>>>>>> upstream/android-13
	vcpu->halt_poll_ns = val;
	trace_kvm_halt_poll_ns_shrink(vcpu->vcpu_id, val, old);
}

static int kvm_vcpu_check_block(struct kvm_vcpu *vcpu)
{
	int ret = -EINTR;
	int idx = srcu_read_lock(&vcpu->kvm->srcu);

	if (kvm_arch_vcpu_runnable(vcpu)) {
		kvm_make_request(KVM_REQ_UNHALT, vcpu);
		goto out;
	}
	if (kvm_cpu_has_pending_timer(vcpu))
		goto out;
	if (signal_pending(current))
		goto out;
<<<<<<< HEAD
=======
	if (kvm_check_request(KVM_REQ_UNBLOCK, vcpu))
		goto out;
>>>>>>> upstream/android-13

	ret = 0;
out:
	srcu_read_unlock(&vcpu->kvm->srcu, idx);
	return ret;
}

<<<<<<< HEAD
=======
static inline void
update_halt_poll_stats(struct kvm_vcpu *vcpu, u64 poll_ns, bool waited)
{
	if (waited)
		vcpu->stat.generic.halt_poll_fail_ns += poll_ns;
	else
		vcpu->stat.generic.halt_poll_success_ns += poll_ns;
}

>>>>>>> upstream/android-13
/*
 * The vCPU has executed a HLT instruction with in-kernel mode enabled.
 */
void kvm_vcpu_block(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	ktime_t start, cur;
	DECLARE_SWAITQUEUE(wait);
	bool waited = false;
	u64 block_ns;

	start = cur = ktime_get();
	if (vcpu->halt_poll_ns) {
		ktime_t stop = ktime_add_ns(ktime_get(), vcpu->halt_poll_ns);

		++vcpu->stat.halt_attempted_poll;
=======
	bool halt_poll_allowed = !kvm_arch_no_poll(vcpu);
	ktime_t start, cur, poll_end;
	bool waited = false;
	u64 block_ns;

	kvm_arch_vcpu_blocking(vcpu);

	start = cur = poll_end = ktime_get();
	if (vcpu->halt_poll_ns && halt_poll_allowed) {
		ktime_t stop = ktime_add_ns(ktime_get(), vcpu->halt_poll_ns);

		++vcpu->stat.generic.halt_attempted_poll;
>>>>>>> upstream/android-13
		do {
			/*
			 * This sets KVM_REQ_UNHALT if an interrupt
			 * arrives.
			 */
			if (kvm_vcpu_check_block(vcpu) < 0) {
<<<<<<< HEAD
				++vcpu->stat.halt_successful_poll;
				if (!vcpu_valid_wakeup(vcpu))
					++vcpu->stat.halt_poll_invalid;
				goto out;
			}
			cur = ktime_get();
		} while (single_task_running() && ktime_before(cur, stop));
	}

	kvm_arch_vcpu_blocking(vcpu);

	for (;;) {
		prepare_to_swait_exclusive(&vcpu->wq, &wait, TASK_INTERRUPTIBLE);
=======
				++vcpu->stat.generic.halt_successful_poll;
				if (!vcpu_valid_wakeup(vcpu))
					++vcpu->stat.generic.halt_poll_invalid;

				KVM_STATS_LOG_HIST_UPDATE(
				      vcpu->stat.generic.halt_poll_success_hist,
				      ktime_to_ns(ktime_get()) -
				      ktime_to_ns(start));
				goto out;
			}
			cpu_relax();
			poll_end = cur = ktime_get();
		} while (kvm_vcpu_can_poll(cur, stop));

		KVM_STATS_LOG_HIST_UPDATE(
				vcpu->stat.generic.halt_poll_fail_hist,
				ktime_to_ns(ktime_get()) - ktime_to_ns(start));
	}


	prepare_to_rcuwait(&vcpu->wait);
	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);
>>>>>>> upstream/android-13

		if (kvm_vcpu_check_block(vcpu) < 0)
			break;

		waited = true;
		schedule();
	}
<<<<<<< HEAD

	finish_swait(&vcpu->wq, &wait);
	cur = ktime_get();

	kvm_arch_vcpu_unblocking(vcpu);
out:
	block_ns = ktime_to_ns(cur) - ktime_to_ns(start);

	if (!vcpu_valid_wakeup(vcpu))
		shrink_halt_poll_ns(vcpu);
	else if (halt_poll_ns) {
		if (block_ns <= vcpu->halt_poll_ns)
			;
		/* we had a long block, shrink polling */
		else if (vcpu->halt_poll_ns && block_ns > halt_poll_ns)
			shrink_halt_poll_ns(vcpu);
		/* we had a short halt and our poll time is too small */
		else if (vcpu->halt_poll_ns < halt_poll_ns &&
			block_ns < halt_poll_ns)
			grow_halt_poll_ns(vcpu);
	} else
		vcpu->halt_poll_ns = 0;
=======
	finish_rcuwait(&vcpu->wait);
	cur = ktime_get();
	if (waited) {
		vcpu->stat.generic.halt_wait_ns +=
			ktime_to_ns(cur) - ktime_to_ns(poll_end);
		KVM_STATS_LOG_HIST_UPDATE(vcpu->stat.generic.halt_wait_hist,
				ktime_to_ns(cur) - ktime_to_ns(poll_end));
	}
out:
	kvm_arch_vcpu_unblocking(vcpu);
	block_ns = ktime_to_ns(cur) - ktime_to_ns(start);

	update_halt_poll_stats(
		vcpu, ktime_to_ns(ktime_sub(poll_end, start)), waited);

	if (halt_poll_allowed) {
		if (!vcpu_valid_wakeup(vcpu)) {
			shrink_halt_poll_ns(vcpu);
		} else if (vcpu->kvm->max_halt_poll_ns) {
			if (block_ns <= vcpu->halt_poll_ns)
				;
			/* we had a long block, shrink polling */
			else if (vcpu->halt_poll_ns &&
					block_ns > vcpu->kvm->max_halt_poll_ns)
				shrink_halt_poll_ns(vcpu);
			/* we had a short halt and our poll time is too small */
			else if (vcpu->halt_poll_ns < vcpu->kvm->max_halt_poll_ns &&
					block_ns < vcpu->kvm->max_halt_poll_ns)
				grow_halt_poll_ns(vcpu);
		} else {
			vcpu->halt_poll_ns = 0;
		}
	}
>>>>>>> upstream/android-13

	trace_kvm_vcpu_wakeup(block_ns, waited, vcpu_valid_wakeup(vcpu));
	kvm_arch_vcpu_block_finish(vcpu);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_block);

bool kvm_vcpu_wake_up(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	struct swait_queue_head *wqp;

	wqp = kvm_arch_vcpu_wq(vcpu);
	if (swq_has_sleeper(wqp)) {
		swake_up_one(wqp);
		++vcpu->stat.halt_wakeup;
=======
	struct rcuwait *waitp;

	waitp = kvm_arch_vcpu_get_wait(vcpu);
	if (rcuwait_wake_up(waitp)) {
		WRITE_ONCE(vcpu->ready, true);
		++vcpu->stat.generic.halt_wakeup;
>>>>>>> upstream/android-13
		return true;
	}

	return false;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_wake_up);

#ifndef CONFIG_S390
/*
 * Kick a sleeping VCPU, or a guest VCPU in guest mode, into host kernel mode.
 */
void kvm_vcpu_kick(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	int me;
	int cpu = vcpu->cpu;
=======
	int me, cpu;
>>>>>>> upstream/android-13

	if (kvm_vcpu_wake_up(vcpu))
		return;

<<<<<<< HEAD
	me = get_cpu();
	if (cpu != me && (unsigned)cpu < nr_cpu_ids && cpu_online(cpu))
		if (kvm_arch_vcpu_should_kick(vcpu))
			smp_send_reschedule(cpu);
=======
	/*
	 * Note, the vCPU could get migrated to a different pCPU at any point
	 * after kvm_arch_vcpu_should_kick(), which could result in sending an
	 * IPI to the previous pCPU.  But, that's ok because the purpose of the
	 * IPI is to force the vCPU to leave IN_GUEST_MODE, and migrating the
	 * vCPU also requires it to leave IN_GUEST_MODE.
	 */
	me = get_cpu();
	if (kvm_arch_vcpu_should_kick(vcpu)) {
		cpu = READ_ONCE(vcpu->cpu);
		if (cpu != me && (unsigned)cpu < nr_cpu_ids && cpu_online(cpu))
			smp_send_reschedule(cpu);
	}
>>>>>>> upstream/android-13
	put_cpu();
}
EXPORT_SYMBOL_GPL(kvm_vcpu_kick);
#endif /* !CONFIG_S390 */

int kvm_vcpu_yield_to(struct kvm_vcpu *target)
{
	struct pid *pid;
	struct task_struct *task = NULL;
	int ret = 0;

	rcu_read_lock();
	pid = rcu_dereference(target->pid);
	if (pid)
		task = get_pid_task(pid, PIDTYPE_PID);
	rcu_read_unlock();
	if (!task)
		return ret;
	ret = yield_to(task, 1);
	put_task_struct(task);

	return ret;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_yield_to);

/*
 * Helper that checks whether a VCPU is eligible for directed yield.
 * Most eligible candidate to yield is decided by following heuristics:
 *
 *  (a) VCPU which has not done pl-exit or cpu relax intercepted recently
 *  (preempted lock holder), indicated by @in_spin_loop.
<<<<<<< HEAD
 *  Set at the beiginning and cleared at the end of interception/PLE handler.
=======
 *  Set at the beginning and cleared at the end of interception/PLE handler.
>>>>>>> upstream/android-13
 *
 *  (b) VCPU which has done pl-exit/ cpu relax intercepted but did not get
 *  chance last time (mostly it has become eligible now since we have probably
 *  yielded to lockholder in last iteration. This is done by toggling
 *  @dy_eligible each time a VCPU checked for eligibility.)
 *
 *  Yielding to a recently pl-exited/cpu relax intercepted VCPU before yielding
 *  to preempted lock-holder could result in wrong VCPU selection and CPU
 *  burning. Giving priority for a potential lock-holder increases lock
 *  progress.
 *
 *  Since algorithm is based on heuristics, accessing another VCPU data without
 *  locking does not harm. It may result in trying to yield to  same VCPU, fail
 *  and continue with next VCPU and so on.
 */
static bool kvm_vcpu_eligible_for_directed_yield(struct kvm_vcpu *vcpu)
{
#ifdef CONFIG_HAVE_KVM_CPU_RELAX_INTERCEPT
	bool eligible;

	eligible = !vcpu->spin_loop.in_spin_loop ||
		    vcpu->spin_loop.dy_eligible;

	if (vcpu->spin_loop.in_spin_loop)
		kvm_vcpu_set_dy_eligible(vcpu, !vcpu->spin_loop.dy_eligible);

	return eligible;
#else
	return true;
#endif
}

/*
 * Unlike kvm_arch_vcpu_runnable, this function is called outside
 * a vcpu_load/vcpu_put pair.  However, for most architectures
 * kvm_arch_vcpu_runnable does not require vcpu_load.
 */
bool __weak kvm_arch_dy_runnable(struct kvm_vcpu *vcpu)
{
	return kvm_arch_vcpu_runnable(vcpu);
}

static bool vcpu_dy_runnable(struct kvm_vcpu *vcpu)
{
	if (kvm_arch_dy_runnable(vcpu))
		return true;

#ifdef CONFIG_KVM_ASYNC_PF
	if (!list_empty_careful(&vcpu->async_pf.done))
		return true;
#endif

	return false;
}

<<<<<<< HEAD
=======
bool __weak kvm_arch_dy_has_pending_interrupt(struct kvm_vcpu *vcpu)
{
	return false;
}

>>>>>>> upstream/android-13
void kvm_vcpu_on_spin(struct kvm_vcpu *me, bool yield_to_kernel_mode)
{
	struct kvm *kvm = me->kvm;
	struct kvm_vcpu *vcpu;
	int last_boosted_vcpu = me->kvm->last_boosted_vcpu;
	int yielded = 0;
	int try = 3;
	int pass;
	int i;

	kvm_vcpu_set_in_spin_loop(me, true);
	/*
	 * We boost the priority of a VCPU that is runnable but not
	 * currently running, because it got preempted by something
	 * else and called schedule in __vcpu_run.  Hopefully that
	 * VCPU is holding the lock that we need and will release it.
	 * We approximate round-robin by starting at the last boosted VCPU.
	 */
	for (pass = 0; pass < 2 && !yielded && try; pass++) {
		kvm_for_each_vcpu(i, vcpu, kvm) {
			if (!pass && i <= last_boosted_vcpu) {
				i = last_boosted_vcpu;
				continue;
			} else if (pass && i > last_boosted_vcpu)
				break;
<<<<<<< HEAD
			if (!READ_ONCE(vcpu->preempted))
				continue;
			if (vcpu == me)
				continue;
			if (swait_active(&vcpu->wq) && !vcpu_dy_runnable(vcpu))
				continue;
			if (yield_to_kernel_mode && !kvm_arch_vcpu_in_kernel(vcpu))
=======
			if (!READ_ONCE(vcpu->ready))
				continue;
			if (vcpu == me)
				continue;
			if (rcuwait_active(&vcpu->wait) &&
			    !vcpu_dy_runnable(vcpu))
				continue;
			if (READ_ONCE(vcpu->preempted) && yield_to_kernel_mode &&
			    !kvm_arch_dy_has_pending_interrupt(vcpu) &&
			    !kvm_arch_vcpu_in_kernel(vcpu))
>>>>>>> upstream/android-13
				continue;
			if (!kvm_vcpu_eligible_for_directed_yield(vcpu))
				continue;

			yielded = kvm_vcpu_yield_to(vcpu);
			if (yielded > 0) {
				kvm->last_boosted_vcpu = i;
				break;
			} else if (yielded < 0) {
				try--;
				if (!try)
					break;
			}
		}
	}
	kvm_vcpu_set_in_spin_loop(me, false);

	/* Ensure vcpu is not eligible during next spinloop */
	kvm_vcpu_set_dy_eligible(me, false);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_on_spin);

<<<<<<< HEAD
=======
static bool kvm_page_in_dirty_ring(struct kvm *kvm, unsigned long pgoff)
{
#if KVM_DIRTY_LOG_PAGE_OFFSET > 0
	return (pgoff >= KVM_DIRTY_LOG_PAGE_OFFSET) &&
	    (pgoff < KVM_DIRTY_LOG_PAGE_OFFSET +
	     kvm->dirty_ring_size / PAGE_SIZE);
#else
	return false;
#endif
}

>>>>>>> upstream/android-13
static vm_fault_t kvm_vcpu_fault(struct vm_fault *vmf)
{
	struct kvm_vcpu *vcpu = vmf->vma->vm_file->private_data;
	struct page *page;

	if (vmf->pgoff == 0)
		page = virt_to_page(vcpu->run);
#ifdef CONFIG_X86
	else if (vmf->pgoff == KVM_PIO_PAGE_OFFSET)
		page = virt_to_page(vcpu->arch.pio_data);
#endif
#ifdef CONFIG_KVM_MMIO
	else if (vmf->pgoff == KVM_COALESCED_MMIO_PAGE_OFFSET)
		page = virt_to_page(vcpu->kvm->coalesced_mmio_ring);
#endif
<<<<<<< HEAD
=======
	else if (kvm_page_in_dirty_ring(vcpu->kvm, vmf->pgoff))
		page = kvm_dirty_ring_get_page(
		    &vcpu->dirty_ring,
		    vmf->pgoff - KVM_DIRTY_LOG_PAGE_OFFSET);
>>>>>>> upstream/android-13
	else
		return kvm_arch_vcpu_fault(vcpu, vmf);
	get_page(page);
	vmf->page = page;
	return 0;
}

static const struct vm_operations_struct kvm_vcpu_vm_ops = {
	.fault = kvm_vcpu_fault,
};

static int kvm_vcpu_mmap(struct file *file, struct vm_area_struct *vma)
{
<<<<<<< HEAD
=======
	struct kvm_vcpu *vcpu = file->private_data;
	unsigned long pages = (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;

	if ((kvm_page_in_dirty_ring(vcpu->kvm, vma->vm_pgoff) ||
	     kvm_page_in_dirty_ring(vcpu->kvm, vma->vm_pgoff + pages - 1)) &&
	    ((vma->vm_flags & VM_EXEC) || !(vma->vm_flags & VM_SHARED)))
		return -EINVAL;

>>>>>>> upstream/android-13
	vma->vm_ops = &kvm_vcpu_vm_ops;
	return 0;
}

static int kvm_vcpu_release(struct inode *inode, struct file *filp)
{
	struct kvm_vcpu *vcpu = filp->private_data;

<<<<<<< HEAD
	debugfs_remove_recursive(vcpu->debugfs_dentry);
=======
>>>>>>> upstream/android-13
	kvm_put_kvm(vcpu->kvm);
	return 0;
}

static struct file_operations kvm_vcpu_fops = {
	.release        = kvm_vcpu_release,
	.unlocked_ioctl = kvm_vcpu_ioctl,
	.mmap           = kvm_vcpu_mmap,
	.llseek		= noop_llseek,
	KVM_COMPAT(kvm_vcpu_compat_ioctl),
};

/*
 * Allocates an inode for the vcpu.
 */
static int create_vcpu_fd(struct kvm_vcpu *vcpu)
{
	char name[8 + 1 + ITOA_MAX_LEN + 1];

	snprintf(name, sizeof(name), "kvm-vcpu:%d", vcpu->vcpu_id);
	return anon_inode_getfd(name, &kvm_vcpu_fops, vcpu, O_RDWR | O_CLOEXEC);
}

<<<<<<< HEAD
static int kvm_create_vcpu_debugfs(struct kvm_vcpu *vcpu)
{
	char dir_name[ITOA_MAX_LEN * 2];
	int ret;

	if (!kvm_arch_has_vcpu_debugfs())
		return 0;

	if (!debugfs_initialized())
		return 0;

	snprintf(dir_name, sizeof(dir_name), "vcpu%d", vcpu->vcpu_id);
	vcpu->debugfs_dentry = debugfs_create_dir(dir_name,
								vcpu->kvm->debugfs_dentry);
	if (!vcpu->debugfs_dentry)
		return -ENOMEM;

	ret = kvm_arch_create_vcpu_debugfs(vcpu);
	if (ret < 0) {
		debugfs_remove_recursive(vcpu->debugfs_dentry);
		return ret;
	}

	return 0;
=======
static void kvm_create_vcpu_debugfs(struct kvm_vcpu *vcpu)
{
#ifdef __KVM_HAVE_ARCH_VCPU_DEBUGFS
	struct dentry *debugfs_dentry;
	char dir_name[ITOA_MAX_LEN * 2];

	if (!debugfs_initialized())
		return;

	snprintf(dir_name, sizeof(dir_name), "vcpu%d", vcpu->vcpu_id);
	debugfs_dentry = debugfs_create_dir(dir_name,
					    vcpu->kvm->debugfs_dentry);

	kvm_arch_create_vcpu_debugfs(vcpu, debugfs_dentry);
#endif
>>>>>>> upstream/android-13
}

/*
 * Creates some virtual cpus.  Good luck creating more than one.
 */
static int kvm_vm_ioctl_create_vcpu(struct kvm *kvm, u32 id)
{
	int r;
	struct kvm_vcpu *vcpu;
<<<<<<< HEAD
=======
	struct page *page;
>>>>>>> upstream/android-13

	if (id >= KVM_MAX_VCPU_ID)
		return -EINVAL;

	mutex_lock(&kvm->lock);
	if (kvm->created_vcpus == KVM_MAX_VCPUS) {
		mutex_unlock(&kvm->lock);
		return -EINVAL;
	}

	kvm->created_vcpus++;
	mutex_unlock(&kvm->lock);

<<<<<<< HEAD
	vcpu = kvm_arch_vcpu_create(kvm, id);
	if (IS_ERR(vcpu)) {
		r = PTR_ERR(vcpu);
		goto vcpu_decrement;
	}

	preempt_notifier_init(&vcpu->preempt_notifier, &kvm_preempt_ops);

	r = kvm_arch_vcpu_setup(vcpu);
	if (r)
		goto vcpu_destroy;

	r = kvm_create_vcpu_debugfs(vcpu);
	if (r)
		goto vcpu_destroy;
=======
	r = kvm_arch_vcpu_precreate(kvm, id);
	if (r)
		goto vcpu_decrement;

	vcpu = kmem_cache_zalloc(kvm_vcpu_cache, GFP_KERNEL_ACCOUNT);
	if (!vcpu) {
		r = -ENOMEM;
		goto vcpu_decrement;
	}

	BUILD_BUG_ON(sizeof(struct kvm_run) > PAGE_SIZE);
	page = alloc_page(GFP_KERNEL_ACCOUNT | __GFP_ZERO);
	if (!page) {
		r = -ENOMEM;
		goto vcpu_free;
	}
	vcpu->run = page_address(page);

	kvm_vcpu_init(vcpu, kvm, id);

	r = kvm_arch_vcpu_create(vcpu);
	if (r)
		goto vcpu_free_run_page;

	if (kvm->dirty_ring_size) {
		r = kvm_dirty_ring_alloc(&vcpu->dirty_ring,
					 id, kvm->dirty_ring_size);
		if (r)
			goto arch_vcpu_destroy;
	}
>>>>>>> upstream/android-13

	mutex_lock(&kvm->lock);
	if (kvm_get_vcpu_by_id(kvm, id)) {
		r = -EEXIST;
		goto unlock_vcpu_destroy;
	}

<<<<<<< HEAD
	BUG_ON(kvm->vcpus[atomic_read(&kvm->online_vcpus)]);
=======
	vcpu->vcpu_idx = atomic_read(&kvm->online_vcpus);
	BUG_ON(kvm->vcpus[vcpu->vcpu_idx]);

	/* Fill the stats id string for the vcpu */
	snprintf(vcpu->stats_id, sizeof(vcpu->stats_id), "kvm-%d/vcpu-%d",
		 task_pid_nr(current), id);
>>>>>>> upstream/android-13

	/* Now it's all set up, let userspace reach it */
	kvm_get_kvm(kvm);
	r = create_vcpu_fd(vcpu);
	if (r < 0) {
<<<<<<< HEAD
		kvm_put_kvm(kvm);
		goto unlock_vcpu_destroy;
	}

	kvm->vcpus[atomic_read(&kvm->online_vcpus)] = vcpu;
=======
		kvm_put_kvm_no_destroy(kvm);
		goto unlock_vcpu_destroy;
	}

	kvm->vcpus[vcpu->vcpu_idx] = vcpu;
>>>>>>> upstream/android-13

	/*
	 * Pairs with smp_rmb() in kvm_get_vcpu.  Write kvm->vcpus
	 * before kvm->online_vcpu's incremented value.
	 */
	smp_wmb();
	atomic_inc(&kvm->online_vcpus);

	mutex_unlock(&kvm->lock);
	kvm_arch_vcpu_postcreate(vcpu);
<<<<<<< HEAD
=======
	kvm_create_vcpu_debugfs(vcpu);
>>>>>>> upstream/android-13
	return r;

unlock_vcpu_destroy:
	mutex_unlock(&kvm->lock);
<<<<<<< HEAD
	debugfs_remove_recursive(vcpu->debugfs_dentry);
vcpu_destroy:
	kvm_arch_vcpu_destroy(vcpu);
=======
	kvm_dirty_ring_free(&vcpu->dirty_ring);
arch_vcpu_destroy:
	kvm_arch_vcpu_destroy(vcpu);
vcpu_free_run_page:
	free_page((unsigned long)vcpu->run);
vcpu_free:
	kmem_cache_free(kvm_vcpu_cache, vcpu);
>>>>>>> upstream/android-13
vcpu_decrement:
	mutex_lock(&kvm->lock);
	kvm->created_vcpus--;
	mutex_unlock(&kvm->lock);
	return r;
}

static int kvm_vcpu_ioctl_set_sigmask(struct kvm_vcpu *vcpu, sigset_t *sigset)
{
	if (sigset) {
		sigdelsetmask(sigset, sigmask(SIGKILL)|sigmask(SIGSTOP));
		vcpu->sigset_active = 1;
		vcpu->sigset = *sigset;
	} else
		vcpu->sigset_active = 0;
	return 0;
}

<<<<<<< HEAD
=======
static ssize_t kvm_vcpu_stats_read(struct file *file, char __user *user_buffer,
			      size_t size, loff_t *offset)
{
	struct kvm_vcpu *vcpu = file->private_data;

	return kvm_stats_read(vcpu->stats_id, &kvm_vcpu_stats_header,
			&kvm_vcpu_stats_desc[0], &vcpu->stat,
			sizeof(vcpu->stat), user_buffer, size, offset);
}

static const struct file_operations kvm_vcpu_stats_fops = {
	.read = kvm_vcpu_stats_read,
	.llseek = noop_llseek,
};

static int kvm_vcpu_ioctl_get_stats_fd(struct kvm_vcpu *vcpu)
{
	int fd;
	struct file *file;
	char name[15 + ITOA_MAX_LEN + 1];

	snprintf(name, sizeof(name), "kvm-vcpu-stats:%d", vcpu->vcpu_id);

	fd = get_unused_fd_flags(O_CLOEXEC);
	if (fd < 0)
		return fd;

	file = anon_inode_getfile(name, &kvm_vcpu_stats_fops, vcpu, O_RDONLY);
	if (IS_ERR(file)) {
		put_unused_fd(fd);
		return PTR_ERR(file);
	}
	file->f_mode |= FMODE_PREAD;
	fd_install(fd, file);

	return fd;
}

>>>>>>> upstream/android-13
static long kvm_vcpu_ioctl(struct file *filp,
			   unsigned int ioctl, unsigned long arg)
{
	struct kvm_vcpu *vcpu = filp->private_data;
	void __user *argp = (void __user *)arg;
	int r;
	struct kvm_fpu *fpu = NULL;
	struct kvm_sregs *kvm_sregs = NULL;

<<<<<<< HEAD
	if (vcpu->kvm->mm != current->mm)
=======
	if (vcpu->kvm->mm != current->mm || vcpu->kvm->vm_bugged)
>>>>>>> upstream/android-13
		return -EIO;

	if (unlikely(_IOC_TYPE(ioctl) != KVMIO))
		return -EINVAL;

	/*
	 * Some architectures have vcpu ioctls that are asynchronous to vcpu
	 * execution; mutex_lock() would break them.
	 */
	r = kvm_arch_vcpu_async_ioctl(filp, ioctl, arg);
	if (r != -ENOIOCTLCMD)
		return r;

	if (mutex_lock_killable(&vcpu->mutex))
		return -EINTR;
	switch (ioctl) {
	case KVM_RUN: {
		struct pid *oldpid;
		r = -EINVAL;
		if (arg)
			goto out;
		oldpid = rcu_access_pointer(vcpu->pid);
		if (unlikely(oldpid != task_pid(current))) {
			/* The thread running this VCPU changed. */
			struct pid *newpid;

			r = kvm_arch_vcpu_run_pid_change(vcpu);
			if (r)
				break;

			newpid = get_task_pid(current, PIDTYPE_PID);
			rcu_assign_pointer(vcpu->pid, newpid);
			if (oldpid)
				synchronize_rcu();
			put_pid(oldpid);
		}
<<<<<<< HEAD
		r = kvm_arch_vcpu_ioctl_run(vcpu, vcpu->run);
=======
		r = kvm_arch_vcpu_ioctl_run(vcpu);
>>>>>>> upstream/android-13
		trace_kvm_userspace_exit(vcpu->run->exit_reason, r);
		break;
	}
	case KVM_GET_REGS: {
		struct kvm_regs *kvm_regs;

		r = -ENOMEM;
<<<<<<< HEAD
		kvm_regs = kzalloc(sizeof(struct kvm_regs), GFP_KERNEL);
=======
		kvm_regs = kzalloc(sizeof(struct kvm_regs), GFP_KERNEL_ACCOUNT);
>>>>>>> upstream/android-13
		if (!kvm_regs)
			goto out;
		r = kvm_arch_vcpu_ioctl_get_regs(vcpu, kvm_regs);
		if (r)
			goto out_free1;
		r = -EFAULT;
		if (copy_to_user(argp, kvm_regs, sizeof(struct kvm_regs)))
			goto out_free1;
		r = 0;
out_free1:
		kfree(kvm_regs);
		break;
	}
	case KVM_SET_REGS: {
		struct kvm_regs *kvm_regs;

<<<<<<< HEAD
		r = -ENOMEM;
=======
>>>>>>> upstream/android-13
		kvm_regs = memdup_user(argp, sizeof(*kvm_regs));
		if (IS_ERR(kvm_regs)) {
			r = PTR_ERR(kvm_regs);
			goto out;
		}
		r = kvm_arch_vcpu_ioctl_set_regs(vcpu, kvm_regs);
		kfree(kvm_regs);
		break;
	}
	case KVM_GET_SREGS: {
<<<<<<< HEAD
		kvm_sregs = kzalloc(sizeof(struct kvm_sregs), GFP_KERNEL);
=======
		kvm_sregs = kzalloc(sizeof(struct kvm_sregs),
				    GFP_KERNEL_ACCOUNT);
>>>>>>> upstream/android-13
		r = -ENOMEM;
		if (!kvm_sregs)
			goto out;
		r = kvm_arch_vcpu_ioctl_get_sregs(vcpu, kvm_sregs);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, kvm_sregs, sizeof(struct kvm_sregs)))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_SREGS: {
		kvm_sregs = memdup_user(argp, sizeof(*kvm_sregs));
		if (IS_ERR(kvm_sregs)) {
			r = PTR_ERR(kvm_sregs);
			kvm_sregs = NULL;
			goto out;
		}
		r = kvm_arch_vcpu_ioctl_set_sregs(vcpu, kvm_sregs);
		break;
	}
	case KVM_GET_MP_STATE: {
		struct kvm_mp_state mp_state;

		r = kvm_arch_vcpu_ioctl_get_mpstate(vcpu, &mp_state);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, &mp_state, sizeof(mp_state)))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_MP_STATE: {
		struct kvm_mp_state mp_state;

		r = -EFAULT;
		if (copy_from_user(&mp_state, argp, sizeof(mp_state)))
			goto out;
		r = kvm_arch_vcpu_ioctl_set_mpstate(vcpu, &mp_state);
		break;
	}
	case KVM_TRANSLATE: {
		struct kvm_translation tr;

		r = -EFAULT;
		if (copy_from_user(&tr, argp, sizeof(tr)))
			goto out;
		r = kvm_arch_vcpu_ioctl_translate(vcpu, &tr);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, &tr, sizeof(tr)))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_GUEST_DEBUG: {
		struct kvm_guest_debug dbg;

		r = -EFAULT;
		if (copy_from_user(&dbg, argp, sizeof(dbg)))
			goto out;
		r = kvm_arch_vcpu_ioctl_set_guest_debug(vcpu, &dbg);
		break;
	}
	case KVM_SET_SIGNAL_MASK: {
		struct kvm_signal_mask __user *sigmask_arg = argp;
		struct kvm_signal_mask kvm_sigmask;
		sigset_t sigset, *p;

		p = NULL;
		if (argp) {
			r = -EFAULT;
			if (copy_from_user(&kvm_sigmask, argp,
					   sizeof(kvm_sigmask)))
				goto out;
			r = -EINVAL;
			if (kvm_sigmask.len != sizeof(sigset))
				goto out;
			r = -EFAULT;
			if (copy_from_user(&sigset, sigmask_arg->sigset,
					   sizeof(sigset)))
				goto out;
			p = &sigset;
		}
		r = kvm_vcpu_ioctl_set_sigmask(vcpu, p);
		break;
	}
	case KVM_GET_FPU: {
<<<<<<< HEAD
		fpu = kzalloc(sizeof(struct kvm_fpu), GFP_KERNEL);
=======
		fpu = kzalloc(sizeof(struct kvm_fpu), GFP_KERNEL_ACCOUNT);
>>>>>>> upstream/android-13
		r = -ENOMEM;
		if (!fpu)
			goto out;
		r = kvm_arch_vcpu_ioctl_get_fpu(vcpu, fpu);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, fpu, sizeof(struct kvm_fpu)))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_FPU: {
		fpu = memdup_user(argp, sizeof(*fpu));
		if (IS_ERR(fpu)) {
			r = PTR_ERR(fpu);
			fpu = NULL;
			goto out;
		}
		r = kvm_arch_vcpu_ioctl_set_fpu(vcpu, fpu);
		break;
	}
<<<<<<< HEAD
=======
	case KVM_GET_STATS_FD: {
		r = kvm_vcpu_ioctl_get_stats_fd(vcpu);
		break;
	}
>>>>>>> upstream/android-13
	default:
		r = kvm_arch_vcpu_ioctl(filp, ioctl, arg);
	}
out:
	mutex_unlock(&vcpu->mutex);
	kfree(fpu);
	kfree(kvm_sregs);
	return r;
}

#ifdef CONFIG_KVM_COMPAT
static long kvm_vcpu_compat_ioctl(struct file *filp,
				  unsigned int ioctl, unsigned long arg)
{
	struct kvm_vcpu *vcpu = filp->private_data;
	void __user *argp = compat_ptr(arg);
	int r;

<<<<<<< HEAD
	if (vcpu->kvm->mm != current->mm)
=======
	if (vcpu->kvm->mm != current->mm || vcpu->kvm->vm_bugged)
>>>>>>> upstream/android-13
		return -EIO;

	switch (ioctl) {
	case KVM_SET_SIGNAL_MASK: {
		struct kvm_signal_mask __user *sigmask_arg = argp;
		struct kvm_signal_mask kvm_sigmask;
		sigset_t sigset;

		if (argp) {
			r = -EFAULT;
			if (copy_from_user(&kvm_sigmask, argp,
					   sizeof(kvm_sigmask)))
				goto out;
			r = -EINVAL;
			if (kvm_sigmask.len != sizeof(compat_sigset_t))
				goto out;
			r = -EFAULT;
<<<<<<< HEAD
			if (get_compat_sigset(&sigset, (void *)sigmask_arg->sigset))
=======
			if (get_compat_sigset(&sigset,
					      (compat_sigset_t __user *)sigmask_arg->sigset))
>>>>>>> upstream/android-13
				goto out;
			r = kvm_vcpu_ioctl_set_sigmask(vcpu, &sigset);
		} else
			r = kvm_vcpu_ioctl_set_sigmask(vcpu, NULL);
		break;
	}
	default:
		r = kvm_vcpu_ioctl(filp, ioctl, arg);
	}

out:
	return r;
}
#endif

<<<<<<< HEAD
=======
static int kvm_device_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct kvm_device *dev = filp->private_data;

	if (dev->ops->mmap)
		return dev->ops->mmap(dev, vma);

	return -ENODEV;
}

>>>>>>> upstream/android-13
static int kvm_device_ioctl_attr(struct kvm_device *dev,
				 int (*accessor)(struct kvm_device *dev,
						 struct kvm_device_attr *attr),
				 unsigned long arg)
{
	struct kvm_device_attr attr;

	if (!accessor)
		return -EPERM;

	if (copy_from_user(&attr, (void __user *)arg, sizeof(attr)))
		return -EFAULT;

	return accessor(dev, &attr);
}

static long kvm_device_ioctl(struct file *filp, unsigned int ioctl,
			     unsigned long arg)
{
	struct kvm_device *dev = filp->private_data;

<<<<<<< HEAD
	if (dev->kvm->mm != current->mm)
=======
	if (dev->kvm->mm != current->mm || dev->kvm->vm_bugged)
>>>>>>> upstream/android-13
		return -EIO;

	switch (ioctl) {
	case KVM_SET_DEVICE_ATTR:
		return kvm_device_ioctl_attr(dev, dev->ops->set_attr, arg);
	case KVM_GET_DEVICE_ATTR:
		return kvm_device_ioctl_attr(dev, dev->ops->get_attr, arg);
	case KVM_HAS_DEVICE_ATTR:
		return kvm_device_ioctl_attr(dev, dev->ops->has_attr, arg);
	default:
		if (dev->ops->ioctl)
			return dev->ops->ioctl(dev, ioctl, arg);

		return -ENOTTY;
	}
}

static int kvm_device_release(struct inode *inode, struct file *filp)
{
	struct kvm_device *dev = filp->private_data;
	struct kvm *kvm = dev->kvm;

<<<<<<< HEAD
=======
	if (dev->ops->release) {
		mutex_lock(&kvm->lock);
		list_del(&dev->vm_node);
		dev->ops->release(dev);
		mutex_unlock(&kvm->lock);
	}

>>>>>>> upstream/android-13
	kvm_put_kvm(kvm);
	return 0;
}

static const struct file_operations kvm_device_fops = {
	.unlocked_ioctl = kvm_device_ioctl,
	.release = kvm_device_release,
	KVM_COMPAT(kvm_device_ioctl),
<<<<<<< HEAD
=======
	.mmap = kvm_device_mmap,
>>>>>>> upstream/android-13
};

struct kvm_device *kvm_device_from_filp(struct file *filp)
{
	if (filp->f_op != &kvm_device_fops)
		return NULL;

	return filp->private_data;
}

<<<<<<< HEAD
static struct kvm_device_ops *kvm_device_ops_table[KVM_DEV_TYPE_MAX] = {
=======
static const struct kvm_device_ops *kvm_device_ops_table[KVM_DEV_TYPE_MAX] = {
>>>>>>> upstream/android-13
#ifdef CONFIG_KVM_MPIC
	[KVM_DEV_TYPE_FSL_MPIC_20]	= &kvm_mpic_ops,
	[KVM_DEV_TYPE_FSL_MPIC_42]	= &kvm_mpic_ops,
#endif
};

<<<<<<< HEAD
int kvm_register_device_ops(struct kvm_device_ops *ops, u32 type)
=======
int kvm_register_device_ops(const struct kvm_device_ops *ops, u32 type)
>>>>>>> upstream/android-13
{
	if (type >= ARRAY_SIZE(kvm_device_ops_table))
		return -ENOSPC;

	if (kvm_device_ops_table[type] != NULL)
		return -EEXIST;

	kvm_device_ops_table[type] = ops;
	return 0;
}

void kvm_unregister_device_ops(u32 type)
{
	if (kvm_device_ops_table[type] != NULL)
		kvm_device_ops_table[type] = NULL;
}

static int kvm_ioctl_create_device(struct kvm *kvm,
				   struct kvm_create_device *cd)
{
<<<<<<< HEAD
	struct kvm_device_ops *ops = NULL;
=======
	const struct kvm_device_ops *ops = NULL;
>>>>>>> upstream/android-13
	struct kvm_device *dev;
	bool test = cd->flags & KVM_CREATE_DEVICE_TEST;
	int type;
	int ret;

	if (cd->type >= ARRAY_SIZE(kvm_device_ops_table))
		return -ENODEV;

	type = array_index_nospec(cd->type, ARRAY_SIZE(kvm_device_ops_table));
	ops = kvm_device_ops_table[type];
	if (ops == NULL)
		return -ENODEV;

	if (test)
		return 0;

<<<<<<< HEAD
	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
=======
	dev = kzalloc(sizeof(*dev), GFP_KERNEL_ACCOUNT);
>>>>>>> upstream/android-13
	if (!dev)
		return -ENOMEM;

	dev->ops = ops;
	dev->kvm = kvm;

	mutex_lock(&kvm->lock);
	ret = ops->create(dev, type);
	if (ret < 0) {
		mutex_unlock(&kvm->lock);
		kfree(dev);
		return ret;
	}
	list_add(&dev->vm_node, &kvm->devices);
	mutex_unlock(&kvm->lock);

	if (ops->init)
		ops->init(dev);

	kvm_get_kvm(kvm);
	ret = anon_inode_getfd(ops->name, &kvm_device_fops, dev, O_RDWR | O_CLOEXEC);
	if (ret < 0) {
<<<<<<< HEAD
		kvm_put_kvm(kvm);
=======
		kvm_put_kvm_no_destroy(kvm);
>>>>>>> upstream/android-13
		mutex_lock(&kvm->lock);
		list_del(&dev->vm_node);
		mutex_unlock(&kvm->lock);
		ops->destroy(dev);
		return ret;
	}

	cd->fd = ret;
	return 0;
}

static long kvm_vm_ioctl_check_extension_generic(struct kvm *kvm, long arg)
{
	switch (arg) {
	case KVM_CAP_USER_MEMORY:
	case KVM_CAP_DESTROY_MEMORY_REGION_WORKS:
	case KVM_CAP_JOIN_MEMORY_REGIONS_WORKS:
	case KVM_CAP_INTERNAL_ERROR_DATA:
#ifdef CONFIG_HAVE_KVM_MSI
	case KVM_CAP_SIGNAL_MSI:
#endif
#ifdef CONFIG_HAVE_KVM_IRQFD
	case KVM_CAP_IRQFD:
	case KVM_CAP_IRQFD_RESAMPLE:
#endif
	case KVM_CAP_IOEVENTFD_ANY_LENGTH:
	case KVM_CAP_CHECK_EXTENSION_VM:
<<<<<<< HEAD
=======
	case KVM_CAP_ENABLE_CAP_VM:
	case KVM_CAP_HALT_POLL:
>>>>>>> upstream/android-13
		return 1;
#ifdef CONFIG_KVM_MMIO
	case KVM_CAP_COALESCED_MMIO:
		return KVM_COALESCED_MMIO_PAGE_OFFSET;
<<<<<<< HEAD
=======
	case KVM_CAP_COALESCED_PIO:
		return 1;
#endif
#ifdef CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT
	case KVM_CAP_MANUAL_DIRTY_LOG_PROTECT2:
		return KVM_DIRTY_LOG_MANUAL_CAPS;
>>>>>>> upstream/android-13
#endif
#ifdef CONFIG_HAVE_KVM_IRQ_ROUTING
	case KVM_CAP_IRQ_ROUTING:
		return KVM_MAX_IRQ_ROUTES;
#endif
#if KVM_ADDRESS_SPACE_NUM > 1
	case KVM_CAP_MULTI_ADDRESS_SPACE:
		return KVM_ADDRESS_SPACE_NUM;
#endif
<<<<<<< HEAD
=======
	case KVM_CAP_NR_MEMSLOTS:
		return KVM_USER_MEM_SLOTS;
	case KVM_CAP_DIRTY_LOG_RING:
#if KVM_DIRTY_LOG_PAGE_OFFSET > 0
		return KVM_DIRTY_RING_MAX_ENTRIES * sizeof(struct kvm_dirty_gfn);
#else
		return 0;
#endif
	case KVM_CAP_BINARY_STATS_FD:
		return 1;
>>>>>>> upstream/android-13
	default:
		break;
	}
	return kvm_vm_ioctl_check_extension(kvm, arg);
}

<<<<<<< HEAD
=======
static int kvm_vm_ioctl_enable_dirty_log_ring(struct kvm *kvm, u32 size)
{
	int r;

	if (!KVM_DIRTY_LOG_PAGE_OFFSET)
		return -EINVAL;

	/* the size should be power of 2 */
	if (!size || (size & (size - 1)))
		return -EINVAL;

	/* Should be bigger to keep the reserved entries, or a page */
	if (size < kvm_dirty_ring_get_rsvd_entries() *
	    sizeof(struct kvm_dirty_gfn) || size < PAGE_SIZE)
		return -EINVAL;

	if (size > KVM_DIRTY_RING_MAX_ENTRIES *
	    sizeof(struct kvm_dirty_gfn))
		return -E2BIG;

	/* We only allow it to set once */
	if (kvm->dirty_ring_size)
		return -EINVAL;

	mutex_lock(&kvm->lock);

	if (kvm->created_vcpus) {
		/* We don't allow to change this value after vcpu created */
		r = -EINVAL;
	} else {
		kvm->dirty_ring_size = size;
		r = 0;
	}

	mutex_unlock(&kvm->lock);
	return r;
}

static int kvm_vm_ioctl_reset_dirty_pages(struct kvm *kvm)
{
	int i;
	struct kvm_vcpu *vcpu;
	int cleared = 0;

	if (!kvm->dirty_ring_size)
		return -EINVAL;

	mutex_lock(&kvm->slots_lock);

	kvm_for_each_vcpu(i, vcpu, kvm)
		cleared += kvm_dirty_ring_reset(vcpu->kvm, &vcpu->dirty_ring);

	mutex_unlock(&kvm->slots_lock);

	if (cleared)
		kvm_flush_remote_tlbs(kvm);

	return cleared;
}

int __attribute__((weak)) kvm_vm_ioctl_enable_cap(struct kvm *kvm,
						  struct kvm_enable_cap *cap)
{
	return -EINVAL;
}

static int kvm_vm_ioctl_enable_cap_generic(struct kvm *kvm,
					   struct kvm_enable_cap *cap)
{
	switch (cap->cap) {
#ifdef CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT
	case KVM_CAP_MANUAL_DIRTY_LOG_PROTECT2: {
		u64 allowed_options = KVM_DIRTY_LOG_MANUAL_PROTECT_ENABLE;

		if (cap->args[0] & KVM_DIRTY_LOG_MANUAL_PROTECT_ENABLE)
			allowed_options = KVM_DIRTY_LOG_MANUAL_CAPS;

		if (cap->flags || (cap->args[0] & ~allowed_options))
			return -EINVAL;
		kvm->manual_dirty_log_protect = cap->args[0];
		return 0;
	}
#endif
	case KVM_CAP_HALT_POLL: {
		if (cap->flags || cap->args[0] != (unsigned int)cap->args[0])
			return -EINVAL;

		kvm->max_halt_poll_ns = cap->args[0];
		return 0;
	}
	case KVM_CAP_DIRTY_LOG_RING:
		return kvm_vm_ioctl_enable_dirty_log_ring(kvm, cap->args[0]);
	default:
		return kvm_vm_ioctl_enable_cap(kvm, cap);
	}
}

static ssize_t kvm_vm_stats_read(struct file *file, char __user *user_buffer,
			      size_t size, loff_t *offset)
{
	struct kvm *kvm = file->private_data;

	return kvm_stats_read(kvm->stats_id, &kvm_vm_stats_header,
				&kvm_vm_stats_desc[0], &kvm->stat,
				sizeof(kvm->stat), user_buffer, size, offset);
}

static const struct file_operations kvm_vm_stats_fops = {
	.read = kvm_vm_stats_read,
	.llseek = noop_llseek,
};

static int kvm_vm_ioctl_get_stats_fd(struct kvm *kvm)
{
	int fd;
	struct file *file;

	fd = get_unused_fd_flags(O_CLOEXEC);
	if (fd < 0)
		return fd;

	file = anon_inode_getfile("kvm-vm-stats",
			&kvm_vm_stats_fops, kvm, O_RDONLY);
	if (IS_ERR(file)) {
		put_unused_fd(fd);
		return PTR_ERR(file);
	}
	file->f_mode |= FMODE_PREAD;
	fd_install(fd, file);

	return fd;
}

>>>>>>> upstream/android-13
static long kvm_vm_ioctl(struct file *filp,
			   unsigned int ioctl, unsigned long arg)
{
	struct kvm *kvm = filp->private_data;
	void __user *argp = (void __user *)arg;
	int r;

<<<<<<< HEAD
	if (kvm->mm != current->mm)
=======
	if (kvm->mm != current->mm || kvm->vm_bugged)
>>>>>>> upstream/android-13
		return -EIO;
	switch (ioctl) {
	case KVM_CREATE_VCPU:
		r = kvm_vm_ioctl_create_vcpu(kvm, arg);
		break;
<<<<<<< HEAD
=======
	case KVM_ENABLE_CAP: {
		struct kvm_enable_cap cap;

		r = -EFAULT;
		if (copy_from_user(&cap, argp, sizeof(cap)))
			goto out;
		r = kvm_vm_ioctl_enable_cap_generic(kvm, &cap);
		break;
	}
>>>>>>> upstream/android-13
	case KVM_SET_USER_MEMORY_REGION: {
		struct kvm_userspace_memory_region kvm_userspace_mem;

		r = -EFAULT;
		if (copy_from_user(&kvm_userspace_mem, argp,
						sizeof(kvm_userspace_mem)))
			goto out;

		r = kvm_vm_ioctl_set_memory_region(kvm, &kvm_userspace_mem);
		break;
	}
	case KVM_GET_DIRTY_LOG: {
		struct kvm_dirty_log log;

		r = -EFAULT;
		if (copy_from_user(&log, argp, sizeof(log)))
			goto out;
		r = kvm_vm_ioctl_get_dirty_log(kvm, &log);
		break;
	}
<<<<<<< HEAD
=======
#ifdef CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT
	case KVM_CLEAR_DIRTY_LOG: {
		struct kvm_clear_dirty_log log;

		r = -EFAULT;
		if (copy_from_user(&log, argp, sizeof(log)))
			goto out;
		r = kvm_vm_ioctl_clear_dirty_log(kvm, &log);
		break;
	}
#endif
>>>>>>> upstream/android-13
#ifdef CONFIG_KVM_MMIO
	case KVM_REGISTER_COALESCED_MMIO: {
		struct kvm_coalesced_mmio_zone zone;

		r = -EFAULT;
		if (copy_from_user(&zone, argp, sizeof(zone)))
			goto out;
		r = kvm_vm_ioctl_register_coalesced_mmio(kvm, &zone);
		break;
	}
	case KVM_UNREGISTER_COALESCED_MMIO: {
		struct kvm_coalesced_mmio_zone zone;

		r = -EFAULT;
		if (copy_from_user(&zone, argp, sizeof(zone)))
			goto out;
		r = kvm_vm_ioctl_unregister_coalesced_mmio(kvm, &zone);
		break;
	}
#endif
	case KVM_IRQFD: {
		struct kvm_irqfd data;

		r = -EFAULT;
		if (copy_from_user(&data, argp, sizeof(data)))
			goto out;
		r = kvm_irqfd(kvm, &data);
		break;
	}
	case KVM_IOEVENTFD: {
		struct kvm_ioeventfd data;

		r = -EFAULT;
		if (copy_from_user(&data, argp, sizeof(data)))
			goto out;
		r = kvm_ioeventfd(kvm, &data);
		break;
	}
#ifdef CONFIG_HAVE_KVM_MSI
	case KVM_SIGNAL_MSI: {
		struct kvm_msi msi;

		r = -EFAULT;
		if (copy_from_user(&msi, argp, sizeof(msi)))
			goto out;
		r = kvm_send_userspace_msi(kvm, &msi);
		break;
	}
#endif
#ifdef __KVM_HAVE_IRQ_LINE
	case KVM_IRQ_LINE_STATUS:
	case KVM_IRQ_LINE: {
		struct kvm_irq_level irq_event;

		r = -EFAULT;
		if (copy_from_user(&irq_event, argp, sizeof(irq_event)))
			goto out;

		r = kvm_vm_ioctl_irq_line(kvm, &irq_event,
					ioctl == KVM_IRQ_LINE_STATUS);
		if (r)
			goto out;

		r = -EFAULT;
		if (ioctl == KVM_IRQ_LINE_STATUS) {
			if (copy_to_user(argp, &irq_event, sizeof(irq_event)))
				goto out;
		}

		r = 0;
		break;
	}
#endif
#ifdef CONFIG_HAVE_KVM_IRQ_ROUTING
	case KVM_SET_GSI_ROUTING: {
		struct kvm_irq_routing routing;
		struct kvm_irq_routing __user *urouting;
		struct kvm_irq_routing_entry *entries = NULL;

		r = -EFAULT;
		if (copy_from_user(&routing, argp, sizeof(routing)))
			goto out;
		r = -EINVAL;
		if (!kvm_arch_can_set_irq_routing(kvm))
			goto out;
		if (routing.nr > KVM_MAX_IRQ_ROUTES)
			goto out;
		if (routing.flags)
			goto out;
		if (routing.nr) {
<<<<<<< HEAD
			r = -ENOMEM;
			entries = vmalloc(array_size(sizeof(*entries),
						     routing.nr));
			if (!entries)
				goto out;
			r = -EFAULT;
			urouting = argp;
			if (copy_from_user(entries, urouting->entries,
					   routing.nr * sizeof(*entries)))
				goto out_free_irq_routing;
		}
		r = kvm_set_irq_routing(kvm, entries, routing.nr,
					routing.flags);
out_free_irq_routing:
		vfree(entries);
=======
			urouting = argp;
			entries = vmemdup_user(urouting->entries,
					       array_size(sizeof(*entries),
							  routing.nr));
			if (IS_ERR(entries)) {
				r = PTR_ERR(entries);
				goto out;
			}
		}
		r = kvm_set_irq_routing(kvm, entries, routing.nr,
					routing.flags);
		kvfree(entries);
>>>>>>> upstream/android-13
		break;
	}
#endif /* CONFIG_HAVE_KVM_IRQ_ROUTING */
	case KVM_CREATE_DEVICE: {
		struct kvm_create_device cd;

		r = -EFAULT;
		if (copy_from_user(&cd, argp, sizeof(cd)))
			goto out;

		r = kvm_ioctl_create_device(kvm, &cd);
		if (r)
			goto out;

		r = -EFAULT;
		if (copy_to_user(argp, &cd, sizeof(cd)))
			goto out;

		r = 0;
		break;
	}
	case KVM_CHECK_EXTENSION:
		r = kvm_vm_ioctl_check_extension_generic(kvm, arg);
		break;
<<<<<<< HEAD
=======
	case KVM_RESET_DIRTY_RINGS:
		r = kvm_vm_ioctl_reset_dirty_pages(kvm);
		break;
	case KVM_GET_STATS_FD:
		r = kvm_vm_ioctl_get_stats_fd(kvm);
		break;
>>>>>>> upstream/android-13
	default:
		r = kvm_arch_vm_ioctl(filp, ioctl, arg);
	}
out:
	return r;
}

#ifdef CONFIG_KVM_COMPAT
struct compat_kvm_dirty_log {
	__u32 slot;
	__u32 padding1;
	union {
		compat_uptr_t dirty_bitmap; /* one bit per page */
		__u64 padding2;
	};
};

<<<<<<< HEAD
=======
struct compat_kvm_clear_dirty_log {
	__u32 slot;
	__u32 num_pages;
	__u64 first_page;
	union {
		compat_uptr_t dirty_bitmap; /* one bit per page */
		__u64 padding2;
	};
};

>>>>>>> upstream/android-13
static long kvm_vm_compat_ioctl(struct file *filp,
			   unsigned int ioctl, unsigned long arg)
{
	struct kvm *kvm = filp->private_data;
	int r;

<<<<<<< HEAD
	if (kvm->mm != current->mm)
		return -EIO;
	switch (ioctl) {
=======
	if (kvm->mm != current->mm || kvm->vm_bugged)
		return -EIO;
	switch (ioctl) {
#ifdef CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT
	case KVM_CLEAR_DIRTY_LOG: {
		struct compat_kvm_clear_dirty_log compat_log;
		struct kvm_clear_dirty_log log;

		if (copy_from_user(&compat_log, (void __user *)arg,
				   sizeof(compat_log)))
			return -EFAULT;
		log.slot	 = compat_log.slot;
		log.num_pages	 = compat_log.num_pages;
		log.first_page	 = compat_log.first_page;
		log.padding2	 = compat_log.padding2;
		log.dirty_bitmap = compat_ptr(compat_log.dirty_bitmap);

		r = kvm_vm_ioctl_clear_dirty_log(kvm, &log);
		break;
	}
#endif
>>>>>>> upstream/android-13
	case KVM_GET_DIRTY_LOG: {
		struct compat_kvm_dirty_log compat_log;
		struct kvm_dirty_log log;

		if (copy_from_user(&compat_log, (void __user *)arg,
				   sizeof(compat_log)))
			return -EFAULT;
		log.slot	 = compat_log.slot;
		log.padding1	 = compat_log.padding1;
		log.padding2	 = compat_log.padding2;
		log.dirty_bitmap = compat_ptr(compat_log.dirty_bitmap);

		r = kvm_vm_ioctl_get_dirty_log(kvm, &log);
		break;
	}
	default:
		r = kvm_vm_ioctl(filp, ioctl, arg);
	}
	return r;
}
#endif

static struct file_operations kvm_vm_fops = {
	.release        = kvm_vm_release,
	.unlocked_ioctl = kvm_vm_ioctl,
	.llseek		= noop_llseek,
	KVM_COMPAT(kvm_vm_compat_ioctl),
};

<<<<<<< HEAD
=======
bool file_is_kvm(struct file *file)
{
	return file && file->f_op == &kvm_vm_fops;
}
EXPORT_SYMBOL_GPL(file_is_kvm);

>>>>>>> upstream/android-13
static int kvm_dev_ioctl_create_vm(unsigned long type)
{
	int r;
	struct kvm *kvm;
	struct file *file;

	kvm = kvm_create_vm(type);
	if (IS_ERR(kvm))
		return PTR_ERR(kvm);
#ifdef CONFIG_KVM_MMIO
	r = kvm_coalesced_mmio_init(kvm);
	if (r < 0)
		goto put_kvm;
#endif
	r = get_unused_fd_flags(O_CLOEXEC);
	if (r < 0)
		goto put_kvm;

<<<<<<< HEAD
=======
	snprintf(kvm->stats_id, sizeof(kvm->stats_id),
			"kvm-%d", task_pid_nr(current));

>>>>>>> upstream/android-13
	file = anon_inode_getfile("kvm-vm", &kvm_vm_fops, kvm, O_RDWR);
	if (IS_ERR(file)) {
		put_unused_fd(r);
		r = PTR_ERR(file);
		goto put_kvm;
	}

	/*
	 * Don't call kvm_put_kvm anymore at this point; file->f_op is
	 * already set, with ->release() being kvm_vm_release().  In error
	 * cases it will be called by the final fput(file) and will take
	 * care of doing kvm_put_kvm(kvm).
	 */
	if (kvm_create_vm_debugfs(kvm, r) < 0) {
		put_unused_fd(r);
		fput(file);
		return -ENOMEM;
	}
	kvm_uevent_notify_change(KVM_EVENT_CREATE_VM, kvm);

	fd_install(r, file);
	return r;

put_kvm:
	kvm_put_kvm(kvm);
	return r;
}

static long kvm_dev_ioctl(struct file *filp,
			  unsigned int ioctl, unsigned long arg)
{
	long r = -EINVAL;

	switch (ioctl) {
	case KVM_GET_API_VERSION:
		if (arg)
			goto out;
		r = KVM_API_VERSION;
		break;
	case KVM_CREATE_VM:
		r = kvm_dev_ioctl_create_vm(arg);
		break;
	case KVM_CHECK_EXTENSION:
		r = kvm_vm_ioctl_check_extension_generic(NULL, arg);
		break;
	case KVM_GET_VCPU_MMAP_SIZE:
		if (arg)
			goto out;
		r = PAGE_SIZE;     /* struct kvm_run */
#ifdef CONFIG_X86
		r += PAGE_SIZE;    /* pio data page */
#endif
#ifdef CONFIG_KVM_MMIO
		r += PAGE_SIZE;    /* coalesced mmio ring page */
#endif
		break;
	case KVM_TRACE_ENABLE:
	case KVM_TRACE_PAUSE:
	case KVM_TRACE_DISABLE:
		r = -EOPNOTSUPP;
		break;
	default:
		return kvm_arch_dev_ioctl(filp, ioctl, arg);
	}
out:
	return r;
}

static struct file_operations kvm_chardev_ops = {
	.unlocked_ioctl = kvm_dev_ioctl,
	.llseek		= noop_llseek,
	KVM_COMPAT(kvm_dev_ioctl),
};

static struct miscdevice kvm_dev = {
	KVM_MINOR,
	"kvm",
	&kvm_chardev_ops,
};

static void hardware_enable_nolock(void *junk)
{
	int cpu = raw_smp_processor_id();
	int r;

	if (cpumask_test_cpu(cpu, cpus_hardware_enabled))
		return;

	cpumask_set_cpu(cpu, cpus_hardware_enabled);

	r = kvm_arch_hardware_enable();

	if (r) {
		cpumask_clear_cpu(cpu, cpus_hardware_enabled);
		atomic_inc(&hardware_enable_failed);
		pr_info("kvm: enabling virtualization on CPU%d failed\n", cpu);
	}
}

static int kvm_starting_cpu(unsigned int cpu)
{
	raw_spin_lock(&kvm_count_lock);
	if (kvm_usage_count)
		hardware_enable_nolock(NULL);
	raw_spin_unlock(&kvm_count_lock);
	return 0;
}

static void hardware_disable_nolock(void *junk)
{
	int cpu = raw_smp_processor_id();

	if (!cpumask_test_cpu(cpu, cpus_hardware_enabled))
		return;
	cpumask_clear_cpu(cpu, cpus_hardware_enabled);
	kvm_arch_hardware_disable();
}

static int kvm_dying_cpu(unsigned int cpu)
{
	raw_spin_lock(&kvm_count_lock);
	if (kvm_usage_count)
		hardware_disable_nolock(NULL);
	raw_spin_unlock(&kvm_count_lock);
	return 0;
}

static void hardware_disable_all_nolock(void)
{
	BUG_ON(!kvm_usage_count);

	kvm_usage_count--;
	if (!kvm_usage_count)
		on_each_cpu(hardware_disable_nolock, NULL, 1);
}

static void hardware_disable_all(void)
{
	raw_spin_lock(&kvm_count_lock);
	hardware_disable_all_nolock();
	raw_spin_unlock(&kvm_count_lock);
}

static int hardware_enable_all(void)
{
	int r = 0;

	raw_spin_lock(&kvm_count_lock);

	kvm_usage_count++;
	if (kvm_usage_count == 1) {
		atomic_set(&hardware_enable_failed, 0);
		on_each_cpu(hardware_enable_nolock, NULL, 1);

		if (atomic_read(&hardware_enable_failed)) {
			hardware_disable_all_nolock();
			r = -EBUSY;
		}
	}

	raw_spin_unlock(&kvm_count_lock);

	return r;
}

static int kvm_reboot(struct notifier_block *notifier, unsigned long val,
		      void *v)
{
	/*
	 * Some (well, at least mine) BIOSes hang on reboot if
	 * in vmx root mode.
	 *
	 * And Intel TXT required VMX off for all cpu when system shutdown.
	 */
	pr_info("kvm: exiting hardware virtualization\n");
	kvm_rebooting = true;
	on_each_cpu(hardware_disable_nolock, NULL, 1);
	return NOTIFY_OK;
}

static struct notifier_block kvm_reboot_notifier = {
	.notifier_call = kvm_reboot,
	.priority = 0,
};

static void kvm_io_bus_destroy(struct kvm_io_bus *bus)
{
	int i;

	for (i = 0; i < bus->dev_count; i++) {
		struct kvm_io_device *pos = bus->range[i].dev;

		kvm_iodevice_destructor(pos);
	}
	kfree(bus);
}

static inline int kvm_io_bus_cmp(const struct kvm_io_range *r1,
				 const struct kvm_io_range *r2)
{
	gpa_t addr1 = r1->addr;
	gpa_t addr2 = r2->addr;

	if (addr1 < addr2)
		return -1;

	/* If r2->len == 0, match the exact address.  If r2->len != 0,
	 * accept any overlapping write.  Any order is acceptable for
	 * overlapping ranges, because kvm_io_bus_get_first_dev ensures
	 * we process all of them.
	 */
	if (r2->len) {
		addr1 += r1->len;
		addr2 += r2->len;
	}

	if (addr1 > addr2)
		return 1;

	return 0;
}

static int kvm_io_bus_sort_cmp(const void *p1, const void *p2)
{
	return kvm_io_bus_cmp(p1, p2);
}

static int kvm_io_bus_get_first_dev(struct kvm_io_bus *bus,
			     gpa_t addr, int len)
{
	struct kvm_io_range *range, key;
	int off;

	key = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	range = bsearch(&key, bus->range, bus->dev_count,
			sizeof(struct kvm_io_range), kvm_io_bus_sort_cmp);
	if (range == NULL)
		return -ENOENT;

	off = range - bus->range;

	while (off > 0 && kvm_io_bus_cmp(&key, &bus->range[off-1]) == 0)
		off--;

	return off;
}

static int __kvm_io_bus_write(struct kvm_vcpu *vcpu, struct kvm_io_bus *bus,
			      struct kvm_io_range *range, const void *val)
{
	int idx;

	idx = kvm_io_bus_get_first_dev(bus, range->addr, range->len);
	if (idx < 0)
		return -EOPNOTSUPP;

	while (idx < bus->dev_count &&
		kvm_io_bus_cmp(range, &bus->range[idx]) == 0) {
		if (!kvm_iodevice_write(vcpu, bus->range[idx].dev, range->addr,
					range->len, val))
			return idx;
		idx++;
	}

	return -EOPNOTSUPP;
}

/* kvm_io_bus_write - called under kvm->slots_lock */
int kvm_io_bus_write(struct kvm_vcpu *vcpu, enum kvm_bus bus_idx, gpa_t addr,
		     int len, const void *val)
{
	struct kvm_io_bus *bus;
	struct kvm_io_range range;
	int r;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	bus = srcu_dereference(vcpu->kvm->buses[bus_idx], &vcpu->kvm->srcu);
	if (!bus)
		return -ENOMEM;
	r = __kvm_io_bus_write(vcpu, bus, &range, val);
	return r < 0 ? r : 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvm_io_bus_write);
>>>>>>> upstream/android-13

/* kvm_io_bus_write_cookie - called under kvm->slots_lock */
int kvm_io_bus_write_cookie(struct kvm_vcpu *vcpu, enum kvm_bus bus_idx,
			    gpa_t addr, int len, const void *val, long cookie)
{
	struct kvm_io_bus *bus;
	struct kvm_io_range range;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	bus = srcu_dereference(vcpu->kvm->buses[bus_idx], &vcpu->kvm->srcu);
	if (!bus)
		return -ENOMEM;

	/* First try the device referenced by cookie. */
	if ((cookie >= 0) && (cookie < bus->dev_count) &&
	    (kvm_io_bus_cmp(&range, &bus->range[cookie]) == 0))
		if (!kvm_iodevice_write(vcpu, bus->range[cookie].dev, addr, len,
					val))
			return cookie;

	/*
	 * cookie contained garbage; fall back to search and return the
	 * correct cookie value.
	 */
	return __kvm_io_bus_write(vcpu, bus, &range, val);
}

static int __kvm_io_bus_read(struct kvm_vcpu *vcpu, struct kvm_io_bus *bus,
			     struct kvm_io_range *range, void *val)
{
	int idx;

	idx = kvm_io_bus_get_first_dev(bus, range->addr, range->len);
	if (idx < 0)
		return -EOPNOTSUPP;

	while (idx < bus->dev_count &&
		kvm_io_bus_cmp(range, &bus->range[idx]) == 0) {
		if (!kvm_iodevice_read(vcpu, bus->range[idx].dev, range->addr,
				       range->len, val))
			return idx;
		idx++;
	}

	return -EOPNOTSUPP;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(kvm_io_bus_write);
=======
>>>>>>> upstream/android-13

/* kvm_io_bus_read - called under kvm->slots_lock */
int kvm_io_bus_read(struct kvm_vcpu *vcpu, enum kvm_bus bus_idx, gpa_t addr,
		    int len, void *val)
{
	struct kvm_io_bus *bus;
	struct kvm_io_range range;
	int r;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	bus = srcu_dereference(vcpu->kvm->buses[bus_idx], &vcpu->kvm->srcu);
	if (!bus)
		return -ENOMEM;
	r = __kvm_io_bus_read(vcpu, bus, &range, val);
	return r < 0 ? r : 0;
}

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
/* Caller must hold slots_lock. */
int kvm_io_bus_register_dev(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			    int len, struct kvm_io_device *dev)
{
	int i;
	struct kvm_io_bus *new_bus, *bus;
	struct kvm_io_range range;

	bus = kvm_get_bus(kvm, bus_idx);
	if (!bus)
		return -ENOMEM;

	/* exclude ioeventfd which is limited by maximum fd */
	if (bus->dev_count - bus->ioeventfd_count > NR_IOBUS_DEVS - 1)
		return -ENOSPC;

<<<<<<< HEAD
	new_bus = kmalloc(sizeof(*bus) + ((bus->dev_count + 1) *
			  sizeof(struct kvm_io_range)), GFP_KERNEL);
=======
	new_bus = kmalloc(struct_size(bus, range, bus->dev_count + 1),
			  GFP_KERNEL_ACCOUNT);
>>>>>>> upstream/android-13
	if (!new_bus)
		return -ENOMEM;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
		.dev = dev,
	};

	for (i = 0; i < bus->dev_count; i++)
		if (kvm_io_bus_cmp(&bus->range[i], &range) > 0)
			break;

	memcpy(new_bus, bus, sizeof(*bus) + i * sizeof(struct kvm_io_range));
	new_bus->dev_count++;
	new_bus->range[i] = range;
	memcpy(new_bus->range + i + 1, bus->range + i,
		(bus->dev_count - i) * sizeof(struct kvm_io_range));
	rcu_assign_pointer(kvm->buses[bus_idx], new_bus);
	synchronize_srcu_expedited(&kvm->srcu);
	kfree(bus);

	return 0;
}

<<<<<<< HEAD
/* Caller must hold slots_lock. */
void kvm_io_bus_unregister_dev(struct kvm *kvm, enum kvm_bus bus_idx,
			       struct kvm_io_device *dev)
=======
int kvm_io_bus_unregister_dev(struct kvm *kvm, enum kvm_bus bus_idx,
			      struct kvm_io_device *dev)
>>>>>>> upstream/android-13
{
	int i, j;
	struct kvm_io_bus *new_bus, *bus;

<<<<<<< HEAD
	bus = kvm_get_bus(kvm, bus_idx);
	if (!bus)
		return;

	for (i = 0; i < bus->dev_count; i++)
		if (bus->range[i].dev == dev) {
			break;
		}

	if (i == bus->dev_count)
		return;

	new_bus = kmalloc(sizeof(*bus) + ((bus->dev_count - 1) *
			  sizeof(struct kvm_io_range)), GFP_KERNEL);
	if (new_bus) {
		memcpy(new_bus, bus, sizeof(*bus) + i * sizeof(struct kvm_io_range));
		new_bus->dev_count--;
		memcpy(new_bus->range + i, bus->range + i + 1,
		       (new_bus->dev_count - i) * sizeof(struct kvm_io_range));
	} else {
=======
	lockdep_assert_held(&kvm->slots_lock);

	bus = kvm_get_bus(kvm, bus_idx);
	if (!bus)
		return 0;

	for (i = 0; i < bus->dev_count; i++) {
		if (bus->range[i].dev == dev) {
			break;
		}
	}

	if (i == bus->dev_count)
		return 0;

	new_bus = kmalloc(struct_size(bus, range, bus->dev_count - 1),
			  GFP_KERNEL_ACCOUNT);
	if (new_bus) {
		memcpy(new_bus, bus, struct_size(bus, range, i));
		new_bus->dev_count--;
		memcpy(new_bus->range + i, bus->range + i + 1,
				flex_array_size(new_bus, range, new_bus->dev_count - i));
	}

	rcu_assign_pointer(kvm->buses[bus_idx], new_bus);
	synchronize_srcu_expedited(&kvm->srcu);

	/* Destroy the old bus _after_ installing the (null) bus. */
	if (!new_bus) {
>>>>>>> upstream/android-13
		pr_err("kvm: failed to shrink bus, removing it completely\n");
		for (j = 0; j < bus->dev_count; j++) {
			if (j == i)
				continue;
			kvm_iodevice_destructor(bus->range[j].dev);
		}
	}

<<<<<<< HEAD
	rcu_assign_pointer(kvm->buses[bus_idx], new_bus);
	synchronize_srcu_expedited(&kvm->srcu);
	kfree(bus);
	return;
=======
	kfree(bus);
	return new_bus ? 0 : -ENOMEM;
>>>>>>> upstream/android-13
}

struct kvm_io_device *kvm_io_bus_get_dev(struct kvm *kvm, enum kvm_bus bus_idx,
					 gpa_t addr)
{
	struct kvm_io_bus *bus;
	int dev_idx, srcu_idx;
	struct kvm_io_device *iodev = NULL;

	srcu_idx = srcu_read_lock(&kvm->srcu);

	bus = srcu_dereference(kvm->buses[bus_idx], &kvm->srcu);
	if (!bus)
		goto out_unlock;

	dev_idx = kvm_io_bus_get_first_dev(bus, addr, 1);
	if (dev_idx < 0)
		goto out_unlock;

	iodev = bus->range[dev_idx].dev;

out_unlock:
	srcu_read_unlock(&kvm->srcu, srcu_idx);

	return iodev;
}
EXPORT_SYMBOL_GPL(kvm_io_bus_get_dev);

static int kvm_debugfs_open(struct inode *inode, struct file *file,
			   int (*get)(void *, u64 *), int (*set)(void *, u64),
			   const char *fmt)
{
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)
					  inode->i_private;

<<<<<<< HEAD
	/* The debugfs files are a reference to the kvm struct which
	 * is still valid when kvm_destroy_vm is called.
	 * To avoid the race between open and the removal of the debugfs
	 * directory we test against the users count.
	 */
	if (!refcount_inc_not_zero(&stat_data->kvm->users_count))
		return -ENOENT;

	if (simple_attr_open(inode, file, get,
			     stat_data->mode & S_IWUGO ? set : NULL,
			     fmt)) {
=======
	/*
	 * The debugfs files are a reference to the kvm struct which
        * is still valid when kvm_destroy_vm is called.  kvm_get_kvm_safe
        * avoids the race between open and the removal of the debugfs directory.
	 */
	if (!kvm_get_kvm_safe(stat_data->kvm))
		return -ENOENT;

	if (simple_attr_open(inode, file, get,
		    kvm_stats_debugfs_mode(stat_data->desc) & 0222
		    ? set : NULL,
		    fmt)) {
>>>>>>> upstream/android-13
		kvm_put_kvm(stat_data->kvm);
		return -ENOMEM;
	}

	return 0;
}

static int kvm_debugfs_release(struct inode *inode, struct file *file)
{
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)
					  inode->i_private;

	simple_attr_release(inode, file);
	kvm_put_kvm(stat_data->kvm);

	return 0;
}

<<<<<<< HEAD
static int vm_stat_get_per_vm(void *data, u64 *val)
{
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;

	*val = *(ulong *)((void *)stat_data->kvm + stat_data->offset);
=======
static int kvm_get_stat_per_vm(struct kvm *kvm, size_t offset, u64 *val)
{
	*val = *(u64 *)((void *)(&kvm->stat) + offset);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int vm_stat_clear_per_vm(void *data, u64 val)
{
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;

	if (val)
		return -EINVAL;

	*(ulong *)((void *)stat_data->kvm + stat_data->offset) = 0;
=======
static int kvm_clear_stat_per_vm(struct kvm *kvm, size_t offset)
{
	*(u64 *)((void *)(&kvm->stat) + offset) = 0;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int vm_stat_get_per_vm_open(struct inode *inode, struct file *file)
{
	__simple_attr_check_format("%llu\n", 0ull);
	return kvm_debugfs_open(inode, file, vm_stat_get_per_vm,
				vm_stat_clear_per_vm, "%llu\n");
}

static const struct file_operations vm_stat_get_per_vm_fops = {
	.owner   = THIS_MODULE,
	.open    = vm_stat_get_per_vm_open,
	.release = kvm_debugfs_release,
	.read    = simple_attr_read,
	.write   = simple_attr_write,
	.llseek  = no_llseek,
};

static int vcpu_stat_get_per_vm(void *data, u64 *val)
{
	int i;
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;
=======
static int kvm_get_stat_per_vcpu(struct kvm *kvm, size_t offset, u64 *val)
{
	int i;
>>>>>>> upstream/android-13
	struct kvm_vcpu *vcpu;

	*val = 0;

<<<<<<< HEAD
	kvm_for_each_vcpu(i, vcpu, stat_data->kvm)
		*val += *(u64 *)((void *)vcpu + stat_data->offset);
=======
	kvm_for_each_vcpu(i, vcpu, kvm)
		*val += *(u64 *)((void *)(&vcpu->stat) + offset);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int vcpu_stat_clear_per_vm(void *data, u64 val)
{
	int i;
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;
	struct kvm_vcpu *vcpu;

	if (val)
		return -EINVAL;

	kvm_for_each_vcpu(i, vcpu, stat_data->kvm)
		*(u64 *)((void *)vcpu + stat_data->offset) = 0;

	return 0;
}

static int vcpu_stat_get_per_vm_open(struct inode *inode, struct file *file)
{
	__simple_attr_check_format("%llu\n", 0ull);
	return kvm_debugfs_open(inode, file, vcpu_stat_get_per_vm,
				 vcpu_stat_clear_per_vm, "%llu\n");
}

static const struct file_operations vcpu_stat_get_per_vm_fops = {
	.owner   = THIS_MODULE,
	.open    = vcpu_stat_get_per_vm_open,
	.release = kvm_debugfs_release,
	.read    = simple_attr_read,
	.write   = simple_attr_write,
	.llseek  = no_llseek,
};

static const struct file_operations *stat_fops_per_vm[] = {
	[KVM_STAT_VCPU] = &vcpu_stat_get_per_vm_fops,
	[KVM_STAT_VM]   = &vm_stat_get_per_vm_fops,
=======
static int kvm_clear_stat_per_vcpu(struct kvm *kvm, size_t offset)
{
	int i;
	struct kvm_vcpu *vcpu;

	kvm_for_each_vcpu(i, vcpu, kvm)
		*(u64 *)((void *)(&vcpu->stat) + offset) = 0;

	return 0;
}

static int kvm_stat_data_get(void *data, u64 *val)
{
	int r = -EFAULT;
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;

	switch (stat_data->kind) {
	case KVM_STAT_VM:
		r = kvm_get_stat_per_vm(stat_data->kvm,
					stat_data->desc->desc.offset, val);
		break;
	case KVM_STAT_VCPU:
		r = kvm_get_stat_per_vcpu(stat_data->kvm,
					  stat_data->desc->desc.offset, val);
		break;
	}

	return r;
}

static int kvm_stat_data_clear(void *data, u64 val)
{
	int r = -EFAULT;
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;

	if (val)
		return -EINVAL;

	switch (stat_data->kind) {
	case KVM_STAT_VM:
		r = kvm_clear_stat_per_vm(stat_data->kvm,
					  stat_data->desc->desc.offset);
		break;
	case KVM_STAT_VCPU:
		r = kvm_clear_stat_per_vcpu(stat_data->kvm,
					    stat_data->desc->desc.offset);
		break;
	}

	return r;
}

static int kvm_stat_data_open(struct inode *inode, struct file *file)
{
	__simple_attr_check_format("%llu\n", 0ull);
	return kvm_debugfs_open(inode, file, kvm_stat_data_get,
				kvm_stat_data_clear, "%llu\n");
}

static const struct file_operations stat_fops_per_vm = {
	.owner = THIS_MODULE,
	.open = kvm_stat_data_open,
	.release = kvm_debugfs_release,
	.read = simple_attr_read,
	.write = simple_attr_write,
	.llseek = no_llseek,
>>>>>>> upstream/android-13
};

static int vm_stat_get(void *_offset, u64 *val)
{
	unsigned offset = (long)_offset;
	struct kvm *kvm;
<<<<<<< HEAD
	struct kvm_stat_data stat_tmp = {.offset = offset};
=======
>>>>>>> upstream/android-13
	u64 tmp_val;

	*val = 0;
	mutex_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list) {
<<<<<<< HEAD
		stat_tmp.kvm = kvm;
		vm_stat_get_per_vm((void *)&stat_tmp, &tmp_val);
=======
		kvm_get_stat_per_vm(kvm, offset, &tmp_val);
>>>>>>> upstream/android-13
		*val += tmp_val;
	}
	mutex_unlock(&kvm_lock);
	return 0;
}

static int vm_stat_clear(void *_offset, u64 val)
{
	unsigned offset = (long)_offset;
	struct kvm *kvm;
<<<<<<< HEAD
	struct kvm_stat_data stat_tmp = {.offset = offset};
=======
>>>>>>> upstream/android-13

	if (val)
		return -EINVAL;

	mutex_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list) {
<<<<<<< HEAD
		stat_tmp.kvm = kvm;
		vm_stat_clear_per_vm((void *)&stat_tmp, 0);
=======
		kvm_clear_stat_per_vm(kvm, offset);
>>>>>>> upstream/android-13
	}
	mutex_unlock(&kvm_lock);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(vm_stat_fops, vm_stat_get, vm_stat_clear, "%llu\n");
<<<<<<< HEAD
=======
DEFINE_SIMPLE_ATTRIBUTE(vm_stat_readonly_fops, vm_stat_get, NULL, "%llu\n");
>>>>>>> upstream/android-13

static int vcpu_stat_get(void *_offset, u64 *val)
{
	unsigned offset = (long)_offset;
	struct kvm *kvm;
<<<<<<< HEAD
	struct kvm_stat_data stat_tmp = {.offset = offset};
=======
>>>>>>> upstream/android-13
	u64 tmp_val;

	*val = 0;
	mutex_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list) {
<<<<<<< HEAD
		stat_tmp.kvm = kvm;
		vcpu_stat_get_per_vm((void *)&stat_tmp, &tmp_val);
=======
		kvm_get_stat_per_vcpu(kvm, offset, &tmp_val);
>>>>>>> upstream/android-13
		*val += tmp_val;
	}
	mutex_unlock(&kvm_lock);
	return 0;
}

static int vcpu_stat_clear(void *_offset, u64 val)
{
	unsigned offset = (long)_offset;
	struct kvm *kvm;
<<<<<<< HEAD
	struct kvm_stat_data stat_tmp = {.offset = offset};
=======
>>>>>>> upstream/android-13

	if (val)
		return -EINVAL;

	mutex_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list) {
<<<<<<< HEAD
		stat_tmp.kvm = kvm;
		vcpu_stat_clear_per_vm((void *)&stat_tmp, 0);
=======
		kvm_clear_stat_per_vcpu(kvm, offset);
>>>>>>> upstream/android-13
	}
	mutex_unlock(&kvm_lock);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(vcpu_stat_fops, vcpu_stat_get, vcpu_stat_clear,
			"%llu\n");
<<<<<<< HEAD

static const struct file_operations *stat_fops[] = {
	[KVM_STAT_VCPU] = &vcpu_stat_fops,
	[KVM_STAT_VM]   = &vm_stat_fops,
};
=======
DEFINE_SIMPLE_ATTRIBUTE(vcpu_stat_readonly_fops, vcpu_stat_get, NULL, "%llu\n");
>>>>>>> upstream/android-13

static void kvm_uevent_notify_change(unsigned int type, struct kvm *kvm)
{
	struct kobj_uevent_env *env;
	unsigned long long created, active;

	if (!kvm_dev.this_device || !kvm)
		return;

	mutex_lock(&kvm_lock);
	if (type == KVM_EVENT_CREATE_VM) {
		kvm_createvm_count++;
		kvm_active_vms++;
	} else if (type == KVM_EVENT_DESTROY_VM) {
		kvm_active_vms--;
	}
	created = kvm_createvm_count;
	active = kvm_active_vms;
	mutex_unlock(&kvm_lock);

<<<<<<< HEAD
	env = kzalloc(sizeof(*env), GFP_KERNEL);
=======
	env = kzalloc(sizeof(*env), GFP_KERNEL_ACCOUNT);
>>>>>>> upstream/android-13
	if (!env)
		return;

	add_uevent_var(env, "CREATED=%llu", created);
	add_uevent_var(env, "COUNT=%llu", active);

	if (type == KVM_EVENT_CREATE_VM) {
		add_uevent_var(env, "EVENT=create");
		kvm->userspace_pid = task_pid_nr(current);
	} else if (type == KVM_EVENT_DESTROY_VM) {
		add_uevent_var(env, "EVENT=destroy");
	}
	add_uevent_var(env, "PID=%d", kvm->userspace_pid);

<<<<<<< HEAD
	if (!IS_ERR_OR_NULL(kvm->debugfs_dentry)) {
		char *tmp, *p = kmalloc(PATH_MAX, GFP_KERNEL);
=======
	if (kvm->debugfs_dentry) {
		char *tmp, *p = kmalloc(PATH_MAX, GFP_KERNEL_ACCOUNT);
>>>>>>> upstream/android-13

		if (p) {
			tmp = dentry_path_raw(kvm->debugfs_dentry, p, PATH_MAX);
			if (!IS_ERR(tmp))
				add_uevent_var(env, "STATS_PATH=%s", tmp);
			kfree(p);
		}
	}
	/* no need for checks, since we are adding at most only 5 keys */
	env->envp[env->envp_idx++] = NULL;
	kobject_uevent_env(&kvm_dev.this_device->kobj, KOBJ_CHANGE, env->envp);
	kfree(env);
}

static void kvm_init_debug(void)
{
<<<<<<< HEAD
	struct kvm_stats_debugfs_item *p;

	kvm_debugfs_dir = debugfs_create_dir("kvm", NULL);

	kvm_debugfs_num_entries = 0;
	for (p = debugfs_entries; p->name; ++p, kvm_debugfs_num_entries++) {
		int mode = p->mode ? p->mode : 0644;
		debugfs_create_file(p->name, mode, kvm_debugfs_dir,
				    (void *)(long)p->offset,
				    stat_fops[p->kind]);
=======
	const struct file_operations *fops;
	const struct _kvm_stats_desc *pdesc;
	int i;

	kvm_debugfs_dir = debugfs_create_dir("kvm", NULL);

	for (i = 0; i < kvm_vm_stats_header.num_desc; ++i) {
		pdesc = &kvm_vm_stats_desc[i];
		if (kvm_stats_debugfs_mode(pdesc) & 0222)
			fops = &vm_stat_fops;
		else
			fops = &vm_stat_readonly_fops;
		debugfs_create_file(pdesc->name, kvm_stats_debugfs_mode(pdesc),
				kvm_debugfs_dir,
				(void *)(long)pdesc->desc.offset, fops);
	}

	for (i = 0; i < kvm_vcpu_stats_header.num_desc; ++i) {
		pdesc = &kvm_vcpu_stats_desc[i];
		if (kvm_stats_debugfs_mode(pdesc) & 0222)
			fops = &vcpu_stat_fops;
		else
			fops = &vcpu_stat_readonly_fops;
		debugfs_create_file(pdesc->name, kvm_stats_debugfs_mode(pdesc),
				kvm_debugfs_dir,
				(void *)(long)pdesc->desc.offset, fops);
>>>>>>> upstream/android-13
	}
}

static int kvm_suspend(void)
{
	if (kvm_usage_count)
		hardware_disable_nolock(NULL);
	return 0;
}

static void kvm_resume(void)
{
	if (kvm_usage_count) {
<<<<<<< HEAD
		WARN_ON(raw_spin_is_locked(&kvm_count_lock));
=======
		lockdep_assert_not_held(&kvm_count_lock);
>>>>>>> upstream/android-13
		hardware_enable_nolock(NULL);
	}
}

static struct syscore_ops kvm_syscore_ops = {
	.suspend = kvm_suspend,
	.resume = kvm_resume,
};

static inline
struct kvm_vcpu *preempt_notifier_to_vcpu(struct preempt_notifier *pn)
{
	return container_of(pn, struct kvm_vcpu, preempt_notifier);
}

static void kvm_sched_in(struct preempt_notifier *pn, int cpu)
{
	struct kvm_vcpu *vcpu = preempt_notifier_to_vcpu(pn);

<<<<<<< HEAD
	if (vcpu->preempted)
		vcpu->preempted = false;

	kvm_arch_sched_in(vcpu, cpu);

=======
	WRITE_ONCE(vcpu->preempted, false);
	WRITE_ONCE(vcpu->ready, false);

	__this_cpu_write(kvm_running_vcpu, vcpu);
	kvm_arch_sched_in(vcpu, cpu);
>>>>>>> upstream/android-13
	kvm_arch_vcpu_load(vcpu, cpu);
}

static void kvm_sched_out(struct preempt_notifier *pn,
			  struct task_struct *next)
{
	struct kvm_vcpu *vcpu = preempt_notifier_to_vcpu(pn);

<<<<<<< HEAD
	if (current->state == TASK_RUNNING)
		vcpu->preempted = true;
	kvm_arch_vcpu_put(vcpu);
=======
	if (current->on_rq) {
		WRITE_ONCE(vcpu->preempted, true);
		WRITE_ONCE(vcpu->ready, true);
	}
	kvm_arch_vcpu_put(vcpu);
	__this_cpu_write(kvm_running_vcpu, NULL);
}

/**
 * kvm_get_running_vcpu - get the vcpu running on the current CPU.
 *
 * We can disable preemption locally around accessing the per-CPU variable,
 * and use the resolved vcpu pointer after enabling preemption again,
 * because even if the current thread is migrated to another CPU, reading
 * the per-CPU value later will give us the same value as we update the
 * per-CPU variable in the preempt notifier handlers.
 */
struct kvm_vcpu *kvm_get_running_vcpu(void)
{
	struct kvm_vcpu *vcpu;

	preempt_disable();
	vcpu = __this_cpu_read(kvm_running_vcpu);
	preempt_enable();

	return vcpu;
}
EXPORT_SYMBOL_GPL(kvm_get_running_vcpu);

/**
 * kvm_get_running_vcpus - get the per-CPU array of currently running vcpus.
 */
struct kvm_vcpu * __percpu *kvm_get_running_vcpus(void)
{
        return &kvm_running_vcpu;
}

struct kvm_cpu_compat_check {
	void *opaque;
	int *ret;
};

static void check_processor_compat(void *data)
{
	struct kvm_cpu_compat_check *c = data;

	*c->ret = kvm_arch_check_processor_compat(c->opaque);
>>>>>>> upstream/android-13
}

int kvm_init(void *opaque, unsigned vcpu_size, unsigned vcpu_align,
		  struct module *module)
{
<<<<<<< HEAD
=======
	struct kvm_cpu_compat_check c;
>>>>>>> upstream/android-13
	int r;
	int cpu;

	r = kvm_arch_init(opaque);
	if (r)
		goto out_fail;

	/*
	 * kvm_arch_init makes sure there's at most one caller
	 * for architectures that support multiple implementations,
	 * like intel and amd on x86.
	 * kvm_arch_init must be called before kvm_irqfd_init to avoid creating
	 * conflicts in case kvm is already setup for another implementation.
	 */
	r = kvm_irqfd_init();
	if (r)
		goto out_irqfd;

	if (!zalloc_cpumask_var(&cpus_hardware_enabled, GFP_KERNEL)) {
		r = -ENOMEM;
		goto out_free_0;
	}

<<<<<<< HEAD
	r = kvm_arch_hardware_setup();
	if (r < 0)
		goto out_free_0a;

	for_each_online_cpu(cpu) {
		smp_call_function_single(cpu,
				kvm_arch_check_processor_compat,
				&r, 1);
		if (r < 0)
			goto out_free_1;
=======
	r = kvm_arch_hardware_setup(opaque);
	if (r < 0)
		goto out_free_1;

	c.ret = &r;
	c.opaque = opaque;
	for_each_online_cpu(cpu) {
		smp_call_function_single(cpu, check_processor_compat, &c, 1);
		if (r < 0)
			goto out_free_2;
>>>>>>> upstream/android-13
	}

	r = cpuhp_setup_state_nocalls(CPUHP_AP_KVM_STARTING, "kvm/cpu:starting",
				      kvm_starting_cpu, kvm_dying_cpu);
	if (r)
		goto out_free_2;
	register_reboot_notifier(&kvm_reboot_notifier);

	/* A kmem cache lets us meet the alignment requirements of fx_save. */
	if (!vcpu_align)
		vcpu_align = __alignof__(struct kvm_vcpu);
	kvm_vcpu_cache =
		kmem_cache_create_usercopy("kvm_vcpu", vcpu_size, vcpu_align,
					   SLAB_ACCOUNT,
					   offsetof(struct kvm_vcpu, arch),
<<<<<<< HEAD
					   sizeof_field(struct kvm_vcpu, arch),
=======
					   offsetofend(struct kvm_vcpu, stats_id)
					   - offsetof(struct kvm_vcpu, arch),
>>>>>>> upstream/android-13
					   NULL);
	if (!kvm_vcpu_cache) {
		r = -ENOMEM;
		goto out_free_3;
	}

	r = kvm_async_pf_init();
	if (r)
		goto out_free;

	kvm_chardev_ops.owner = module;
	kvm_vm_fops.owner = module;
	kvm_vcpu_fops.owner = module;

	r = misc_register(&kvm_dev);
	if (r) {
		pr_err("kvm: misc device register failed\n");
		goto out_unreg;
	}

	register_syscore_ops(&kvm_syscore_ops);

	kvm_preempt_ops.sched_in = kvm_sched_in;
	kvm_preempt_ops.sched_out = kvm_sched_out;

	kvm_init_debug();

	r = kvm_vfio_ops_init();
	WARN_ON(r);

	return 0;

out_unreg:
	kvm_async_pf_deinit();
out_free:
	kmem_cache_destroy(kvm_vcpu_cache);
out_free_3:
	unregister_reboot_notifier(&kvm_reboot_notifier);
	cpuhp_remove_state_nocalls(CPUHP_AP_KVM_STARTING);
out_free_2:
<<<<<<< HEAD
out_free_1:
	kvm_arch_hardware_unsetup();
out_free_0a:
=======
	kvm_arch_hardware_unsetup();
out_free_1:
>>>>>>> upstream/android-13
	free_cpumask_var(cpus_hardware_enabled);
out_free_0:
	kvm_irqfd_exit();
out_irqfd:
	kvm_arch_exit();
out_fail:
	return r;
}
EXPORT_SYMBOL_GPL(kvm_init);

void kvm_exit(void)
{
	debugfs_remove_recursive(kvm_debugfs_dir);
	misc_deregister(&kvm_dev);
	kmem_cache_destroy(kvm_vcpu_cache);
	kvm_async_pf_deinit();
	unregister_syscore_ops(&kvm_syscore_ops);
	unregister_reboot_notifier(&kvm_reboot_notifier);
	cpuhp_remove_state_nocalls(CPUHP_AP_KVM_STARTING);
	on_each_cpu(hardware_disable_nolock, NULL, 1);
	kvm_arch_hardware_unsetup();
	kvm_arch_exit();
	kvm_irqfd_exit();
	free_cpumask_var(cpus_hardware_enabled);
	kvm_vfio_ops_exit();
}
EXPORT_SYMBOL_GPL(kvm_exit);

struct kvm_vm_worker_thread_context {
	struct kvm *kvm;
	struct task_struct *parent;
	struct completion init_done;
	kvm_vm_thread_fn_t thread_fn;
	uintptr_t data;
	int err;
};

static int kvm_vm_worker_thread(void *context)
{
	/*
	 * The init_context is allocated on the stack of the parent thread, so
	 * we have to locally copy anything that is needed beyond initialization
	 */
	struct kvm_vm_worker_thread_context *init_context = context;
	struct kvm *kvm = init_context->kvm;
	kvm_vm_thread_fn_t thread_fn = init_context->thread_fn;
	uintptr_t data = init_context->data;
	int err;

	err = kthread_park(current);
	/* kthread_park(current) is never supposed to return an error */
	WARN_ON(err != 0);
	if (err)
		goto init_complete;

	err = cgroup_attach_task_all(init_context->parent, current);
	if (err) {
		kvm_err("%s: cgroup_attach_task_all failed with err %d\n",
			__func__, err);
		goto init_complete;
	}

	set_user_nice(current, task_nice(init_context->parent));

init_complete:
	init_context->err = err;
	complete(&init_context->init_done);
	init_context = NULL;

	if (err)
		return err;

	/* Wait to be woken up by the spawner before proceeding. */
	kthread_parkme();

	if (!kthread_should_stop())
		err = thread_fn(kvm, data);

	return err;
}

int kvm_vm_create_worker_thread(struct kvm *kvm, kvm_vm_thread_fn_t thread_fn,
				uintptr_t data, const char *name,
				struct task_struct **thread_ptr)
{
	struct kvm_vm_worker_thread_context init_context = {};
	struct task_struct *thread;

	*thread_ptr = NULL;
	init_context.kvm = kvm;
	init_context.parent = current;
	init_context.thread_fn = thread_fn;
	init_context.data = data;
	init_completion(&init_context.init_done);

	thread = kthread_run(kvm_vm_worker_thread, &init_context,
			     "%s-%d", name, task_pid_nr(current));
	if (IS_ERR(thread))
		return PTR_ERR(thread);

	/* kthread_run is never supposed to return NULL */
	WARN_ON(thread == NULL);

	wait_for_completion(&init_context.init_done);

	if (!init_context.err)
		*thread_ptr = thread;

	return init_context.err;
}
