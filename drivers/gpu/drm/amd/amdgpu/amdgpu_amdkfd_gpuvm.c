/*
 * Copyright 2014-2018 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
<<<<<<< HEAD

#define pr_fmt(fmt) "kfd2kgd: " fmt

#include <linux/list.h>
#include <linux/pagemap.h>
#include <linux/sched/mm.h>
#include <drm/drmP.h>
#include "amdgpu_object.h"
#include "amdgpu_vm.h"
#include "amdgpu_amdkfd.h"

/* Special VM and GART address alignment needed for VI pre-Fiji due to
 * a HW bug.
 */
#define VI_BO_SIZE_ALIGN (0x8000)

/* BO flag to indicate a KFD userptr BO */
#define AMDGPU_AMDKFD_USERPTR_BO (1ULL << 63)
=======
#include <linux/dma-buf.h>
#include <linux/list.h>
#include <linux/pagemap.h>
#include <linux/sched/mm.h>
#include <linux/sched/task.h>

#include "amdgpu_object.h"
#include "amdgpu_gem.h"
#include "amdgpu_vm.h"
#include "amdgpu_amdkfd.h"
#include "amdgpu_dma_buf.h"
#include <uapi/linux/kfd_ioctl.h>
#include "amdgpu_xgmi.h"
>>>>>>> upstream/android-13

/* Userptr restore delay, just long enough to allow consecutive VM
 * changes to accumulate
 */
#define AMDGPU_USERPTR_RESTORE_DELAY_MS 1

/* Impose limit on how much memory KFD can use */
static struct {
	uint64_t max_system_mem_limit;
<<<<<<< HEAD
	uint64_t max_userptr_mem_limit;
	int64_t system_mem_used;
	int64_t userptr_mem_used;
	spinlock_t mem_limit_lock;
} kfd_mem_limit;

/* Struct used for amdgpu_amdkfd_bo_validate */
struct amdgpu_vm_parser {
	uint32_t        domain;
	bool            wait;
};

=======
	uint64_t max_ttm_mem_limit;
	int64_t system_mem_used;
	int64_t ttm_mem_used;
	spinlock_t mem_limit_lock;
} kfd_mem_limit;

>>>>>>> upstream/android-13
static const char * const domain_bit_to_string[] = {
		"CPU",
		"GTT",
		"VRAM",
		"GDS",
		"GWS",
		"OA"
};

#define domain_string(domain) domain_bit_to_string[ffs(domain)-1]

static void amdgpu_amdkfd_restore_userptr_worker(struct work_struct *work);


static inline struct amdgpu_device *get_amdgpu_device(struct kgd_dev *kgd)
{
	return (struct amdgpu_device *)kgd;
}

<<<<<<< HEAD
static bool check_if_add_bo_to_vm(struct amdgpu_vm *avm,
		struct kgd_mem *mem)
{
	struct kfd_bo_va_list *entry;

	list_for_each_entry(entry, &mem->bo_va_list, bo_list)
		if (entry->bo_va->base.vm == avm)
			return false;

	return true;
}

/* Set memory usage limits. Current, limits are
 *  System (kernel) memory - 3/8th System RAM
 *  Userptr memory - 3/4th System RAM
=======
static bool kfd_mem_is_attached(struct amdgpu_vm *avm,
		struct kgd_mem *mem)
{
	struct kfd_mem_attachment *entry;

	list_for_each_entry(entry, &mem->attachments, list)
		if (entry->bo_va->base.vm == avm)
			return true;

	return false;
}

/* Set memory usage limits. Current, limits are
 *  System (TTM + userptr) memory - 15/16th System RAM
 *  TTM memory - 3/8th System RAM
>>>>>>> upstream/android-13
 */
void amdgpu_amdkfd_gpuvm_init_mem_limits(void)
{
	struct sysinfo si;
	uint64_t mem;

	si_meminfo(&si);
<<<<<<< HEAD
	mem = si.totalram - si.totalhigh;
	mem *= si.mem_unit;

	spin_lock_init(&kfd_mem_limit.mem_limit_lock);
	kfd_mem_limit.max_system_mem_limit = (mem >> 1) - (mem >> 3);
	kfd_mem_limit.max_userptr_mem_limit = mem - (mem >> 2);
	pr_debug("Kernel memory limit %lluM, userptr limit %lluM\n",
		(kfd_mem_limit.max_system_mem_limit >> 20),
		(kfd_mem_limit.max_userptr_mem_limit >> 20));
}

static int amdgpu_amdkfd_reserve_system_mem_limit(struct amdgpu_device *adev,
					      uint64_t size, u32 domain)
{
	size_t acc_size;
	int ret = 0;

	acc_size = ttm_bo_dma_acc_size(&adev->mman.bdev, size,
				       sizeof(struct amdgpu_bo));

	spin_lock(&kfd_mem_limit.mem_limit_lock);
	if (domain == AMDGPU_GEM_DOMAIN_GTT) {
		if (kfd_mem_limit.system_mem_used + (acc_size + size) >
			kfd_mem_limit.max_system_mem_limit) {
			ret = -ENOMEM;
			goto err_no_mem;
		}
		kfd_mem_limit.system_mem_used += (acc_size + size);
	} else if (domain == AMDGPU_GEM_DOMAIN_CPU) {
		if ((kfd_mem_limit.system_mem_used + acc_size >
			kfd_mem_limit.max_system_mem_limit) ||
			(kfd_mem_limit.userptr_mem_used + (size + acc_size) >
			kfd_mem_limit.max_userptr_mem_limit)) {
			ret = -ENOMEM;
			goto err_no_mem;
		}
		kfd_mem_limit.system_mem_used += acc_size;
		kfd_mem_limit.userptr_mem_used += size;
	}
err_no_mem:
=======
	mem = si.freeram - si.freehigh;
	mem *= si.mem_unit;

	spin_lock_init(&kfd_mem_limit.mem_limit_lock);
	kfd_mem_limit.max_system_mem_limit = mem - (mem >> 4);
	kfd_mem_limit.max_ttm_mem_limit = (mem >> 1) - (mem >> 3);
	pr_debug("Kernel memory limit %lluM, TTM limit %lluM\n",
		(kfd_mem_limit.max_system_mem_limit >> 20),
		(kfd_mem_limit.max_ttm_mem_limit >> 20));
}

void amdgpu_amdkfd_reserve_system_mem(uint64_t size)
{
	kfd_mem_limit.system_mem_used += size;
}

/* Estimate page table size needed to represent a given memory size
 *
 * With 4KB pages, we need one 8 byte PTE for each 4KB of memory
 * (factor 512, >> 9). With 2MB pages, we need one 8 byte PTE for 2MB
 * of memory (factor 256K, >> 18). ROCm user mode tries to optimize
 * for 2MB pages for TLB efficiency. However, small allocations and
 * fragmented system memory still need some 4KB pages. We choose a
 * compromise that should work in most cases without reserving too
 * much memory for page tables unnecessarily (factor 16K, >> 14).
 */
#define ESTIMATE_PT_SIZE(mem_size) ((mem_size) >> 14)

static size_t amdgpu_amdkfd_acc_size(uint64_t size)
{
	size >>= PAGE_SHIFT;
	size *= sizeof(dma_addr_t) + sizeof(void *);

	return __roundup_pow_of_two(sizeof(struct amdgpu_bo)) +
		__roundup_pow_of_two(sizeof(struct ttm_tt)) +
		PAGE_ALIGN(size);
}

static int amdgpu_amdkfd_reserve_mem_limit(struct amdgpu_device *adev,
		uint64_t size, u32 domain, bool sg)
{
	uint64_t reserved_for_pt =
		ESTIMATE_PT_SIZE(amdgpu_amdkfd_total_mem_size);
	size_t acc_size, system_mem_needed, ttm_mem_needed, vram_needed;
	int ret = 0;

	acc_size = amdgpu_amdkfd_acc_size(size);

	vram_needed = 0;
	if (domain == AMDGPU_GEM_DOMAIN_GTT) {
		/* TTM GTT memory */
		system_mem_needed = acc_size + size;
		ttm_mem_needed = acc_size + size;
	} else if (domain == AMDGPU_GEM_DOMAIN_CPU && !sg) {
		/* Userptr */
		system_mem_needed = acc_size + size;
		ttm_mem_needed = acc_size;
	} else {
		/* VRAM and SG */
		system_mem_needed = acc_size;
		ttm_mem_needed = acc_size;
		if (domain == AMDGPU_GEM_DOMAIN_VRAM)
			vram_needed = size;
	}

	spin_lock(&kfd_mem_limit.mem_limit_lock);

	if (kfd_mem_limit.system_mem_used + system_mem_needed >
	    kfd_mem_limit.max_system_mem_limit)
		pr_debug("Set no_system_mem_limit=1 if using shared memory\n");

	if ((kfd_mem_limit.system_mem_used + system_mem_needed >
	     kfd_mem_limit.max_system_mem_limit && !no_system_mem_limit) ||
	    (kfd_mem_limit.ttm_mem_used + ttm_mem_needed >
	     kfd_mem_limit.max_ttm_mem_limit) ||
	    (adev->kfd.vram_used + vram_needed >
	     adev->gmc.real_vram_size - reserved_for_pt)) {
		ret = -ENOMEM;
	} else {
		kfd_mem_limit.system_mem_used += system_mem_needed;
		kfd_mem_limit.ttm_mem_used += ttm_mem_needed;
		adev->kfd.vram_used += vram_needed;
	}

>>>>>>> upstream/android-13
	spin_unlock(&kfd_mem_limit.mem_limit_lock);
	return ret;
}

<<<<<<< HEAD
static void unreserve_system_mem_limit(struct amdgpu_device *adev,
				       uint64_t size, u32 domain)
{
	size_t acc_size;

	acc_size = ttm_bo_dma_acc_size(&adev->mman.bdev, size,
				       sizeof(struct amdgpu_bo));
=======
static void unreserve_mem_limit(struct amdgpu_device *adev,
		uint64_t size, u32 domain, bool sg)
{
	size_t acc_size;

	acc_size = amdgpu_amdkfd_acc_size(size);
>>>>>>> upstream/android-13

	spin_lock(&kfd_mem_limit.mem_limit_lock);
	if (domain == AMDGPU_GEM_DOMAIN_GTT) {
		kfd_mem_limit.system_mem_used -= (acc_size + size);
<<<<<<< HEAD
	} else if (domain == AMDGPU_GEM_DOMAIN_CPU) {
		kfd_mem_limit.system_mem_used -= acc_size;
		kfd_mem_limit.userptr_mem_used -= size;
	}
	WARN_ONCE(kfd_mem_limit.system_mem_used < 0,
		  "kfd system memory accounting unbalanced");
	WARN_ONCE(kfd_mem_limit.userptr_mem_used < 0,
		  "kfd userptr memory accounting unbalanced");
=======
		kfd_mem_limit.ttm_mem_used -= (acc_size + size);
	} else if (domain == AMDGPU_GEM_DOMAIN_CPU && !sg) {
		kfd_mem_limit.system_mem_used -= (acc_size + size);
		kfd_mem_limit.ttm_mem_used -= acc_size;
	} else {
		kfd_mem_limit.system_mem_used -= acc_size;
		kfd_mem_limit.ttm_mem_used -= acc_size;
		if (domain == AMDGPU_GEM_DOMAIN_VRAM) {
			adev->kfd.vram_used -= size;
			WARN_ONCE(adev->kfd.vram_used < 0,
				  "kfd VRAM memory accounting unbalanced");
		}
	}
	WARN_ONCE(kfd_mem_limit.system_mem_used < 0,
		  "kfd system memory accounting unbalanced");
	WARN_ONCE(kfd_mem_limit.ttm_mem_used < 0,
		  "kfd TTM memory accounting unbalanced");
>>>>>>> upstream/android-13

	spin_unlock(&kfd_mem_limit.mem_limit_lock);
}

<<<<<<< HEAD
void amdgpu_amdkfd_unreserve_system_memory_limit(struct amdgpu_bo *bo)
{
	spin_lock(&kfd_mem_limit.mem_limit_lock);

	if (bo->flags & AMDGPU_AMDKFD_USERPTR_BO) {
		kfd_mem_limit.system_mem_used -= bo->tbo.acc_size;
		kfd_mem_limit.userptr_mem_used -= amdgpu_bo_size(bo);
	} else if (bo->preferred_domains == AMDGPU_GEM_DOMAIN_GTT) {
		kfd_mem_limit.system_mem_used -=
			(bo->tbo.acc_size + amdgpu_bo_size(bo));
	}
	WARN_ONCE(kfd_mem_limit.system_mem_used < 0,
		  "kfd system memory accounting unbalanced");
	WARN_ONCE(kfd_mem_limit.userptr_mem_used < 0,
		  "kfd userptr memory accounting unbalanced");

	spin_unlock(&kfd_mem_limit.mem_limit_lock);
}


/* amdgpu_amdkfd_remove_eviction_fence - Removes eviction fence(s) from BO's
 *  reservation object.
 *
 * @bo: [IN] Remove eviction fence(s) from this BO
 * @ef: [IN] If ef is specified, then this eviction fence is removed if it
 *  is present in the shared list.
 * @ef_list: [OUT] Returns list of eviction fences. These fences are removed
 *  from BO's reservation object shared list.
 * @ef_count: [OUT] Number of fences in ef_list.
 *
 * NOTE: If called with ef_list, then amdgpu_amdkfd_add_eviction_fence must be
 *  called to restore the eviction fences and to avoid memory leak. This is
 *  useful for shared BOs.
 * NOTE: Must be called with BO reserved i.e. bo->tbo.resv->lock held.
 */
static int amdgpu_amdkfd_remove_eviction_fence(struct amdgpu_bo *bo,
					struct amdgpu_amdkfd_fence *ef,
					struct amdgpu_amdkfd_fence ***ef_list,
					unsigned int *ef_count)
{
	struct reservation_object *resv = bo->tbo.resv;
	struct reservation_object_list *old, *new;
	unsigned int i, j, k;

	if (!ef && !ef_list)
		return -EINVAL;

	if (ef_list) {
		*ef_list = NULL;
		*ef_count = 0;
	}

	old = reservation_object_get_list(resv);
	if (!old)
		return 0;

	new = kmalloc(offsetof(typeof(*new), shared[old->shared_max]),
		      GFP_KERNEL);
=======
void amdgpu_amdkfd_unreserve_memory_limit(struct amdgpu_bo *bo)
{
	struct amdgpu_device *adev = amdgpu_ttm_adev(bo->tbo.bdev);
	u32 domain = bo->preferred_domains;
	bool sg = (bo->preferred_domains == AMDGPU_GEM_DOMAIN_CPU);

	if (bo->flags & AMDGPU_AMDKFD_CREATE_USERPTR_BO) {
		domain = AMDGPU_GEM_DOMAIN_CPU;
		sg = false;
	}

	unreserve_mem_limit(adev, amdgpu_bo_size(bo), domain, sg);
}


/* amdgpu_amdkfd_remove_eviction_fence - Removes eviction fence from BO's
 *  reservation object.
 *
 * @bo: [IN] Remove eviction fence(s) from this BO
 * @ef: [IN] This eviction fence is removed if it
 *  is present in the shared list.
 *
 * NOTE: Must be called with BO reserved i.e. bo->tbo.resv->lock held.
 */
static int amdgpu_amdkfd_remove_eviction_fence(struct amdgpu_bo *bo,
					struct amdgpu_amdkfd_fence *ef)
{
	struct dma_resv *resv = bo->tbo.base.resv;
	struct dma_resv_list *old, *new;
	unsigned int i, j, k;

	if (!ef)
		return -EINVAL;

	old = dma_resv_shared_list(resv);
	if (!old)
		return 0;

	new = kmalloc(struct_size(new, shared, old->shared_max), GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!new)
		return -ENOMEM;

	/* Go through all the shared fences in the resevation object and sort
	 * the interesting ones to the end of the list.
	 */
	for (i = 0, j = old->shared_count, k = 0; i < old->shared_count; ++i) {
		struct dma_fence *f;

		f = rcu_dereference_protected(old->shared[i],
<<<<<<< HEAD
					      reservation_object_held(resv));

		if ((ef && f->context == ef->base.context) ||
		    (!ef && to_amdgpu_amdkfd_fence(f)))
=======
					      dma_resv_held(resv));

		if (f->context == ef->base.context)
>>>>>>> upstream/android-13
			RCU_INIT_POINTER(new->shared[--j], f);
		else
			RCU_INIT_POINTER(new->shared[k++], f);
	}
	new->shared_max = old->shared_max;
	new->shared_count = k;

<<<<<<< HEAD
	if (!ef) {
		unsigned int count = old->shared_count - j;

		/* Alloc memory for count number of eviction fence pointers.
		 * Fill the ef_list array and ef_count
		 */
		*ef_list = kcalloc(count, sizeof(**ef_list), GFP_KERNEL);
		*ef_count = count;

		if (!*ef_list) {
			kfree(new);
			return -ENOMEM;
		}
	}

	/* Install the new fence list, seqcount provides the barriers */
	preempt_disable();
	write_seqcount_begin(&resv->seq);
	RCU_INIT_POINTER(resv->fence, new);
	write_seqcount_end(&resv->seq);
	preempt_enable();

	/* Drop the references to the removed fences or move them to ef_list */
	for (i = j, k = 0; i < old->shared_count; ++i) {
		struct dma_fence *f;

		f = rcu_dereference_protected(new->shared[i],
					      reservation_object_held(resv));
		if (!ef)
			(*ef_list)[k++] = to_amdgpu_amdkfd_fence(f);
		else
			dma_fence_put(f);
=======
	/* Install the new fence list, seqcount provides the barriers */
	write_seqcount_begin(&resv->seq);
	RCU_INIT_POINTER(resv->fence, new);
	write_seqcount_end(&resv->seq);

	/* Drop the references to the removed fences or move them to ef_list */
	for (i = j; i < old->shared_count; ++i) {
		struct dma_fence *f;

		f = rcu_dereference_protected(new->shared[i],
					      dma_resv_held(resv));
		dma_fence_put(f);
>>>>>>> upstream/android-13
	}
	kfree_rcu(old, rcu);

	return 0;
}

<<<<<<< HEAD
/* amdgpu_amdkfd_add_eviction_fence - Adds eviction fence(s) back into BO's
 *  reservation object.
 *
 * @bo: [IN] Add eviction fences to this BO
 * @ef_list: [IN] List of eviction fences to be added
 * @ef_count: [IN] Number of fences in ef_list.
 *
 * NOTE: Must call amdgpu_amdkfd_remove_eviction_fence before calling this
 *  function.
 */
static void amdgpu_amdkfd_add_eviction_fence(struct amdgpu_bo *bo,
				struct amdgpu_amdkfd_fence **ef_list,
				unsigned int ef_count)
{
	int i;

	if (!ef_list || !ef_count)
		return;

	for (i = 0; i < ef_count; i++) {
		amdgpu_bo_fence(bo, &ef_list[i]->base, true);
		/* Re-adding the fence takes an additional reference. Drop that
		 * reference.
		 */
		dma_fence_put(&ef_list[i]->base);
	}

	kfree(ef_list);
=======
int amdgpu_amdkfd_remove_fence_on_pt_pd_bos(struct amdgpu_bo *bo)
{
	struct amdgpu_bo *root = bo;
	struct amdgpu_vm_bo_base *vm_bo;
	struct amdgpu_vm *vm;
	struct amdkfd_process_info *info;
	struct amdgpu_amdkfd_fence *ef;
	int ret;

	/* we can always get vm_bo from root PD bo.*/
	while (root->parent)
		root = root->parent;

	vm_bo = root->vm_bo;
	if (!vm_bo)
		return 0;

	vm = vm_bo->vm;
	if (!vm)
		return 0;

	info = vm->process_info;
	if (!info || !info->eviction_fence)
		return 0;

	ef = container_of(dma_fence_get(&info->eviction_fence->base),
			struct amdgpu_amdkfd_fence, base);

	BUG_ON(!dma_resv_trylock(bo->tbo.base.resv));
	ret = amdgpu_amdkfd_remove_eviction_fence(bo, ef);
	dma_resv_unlock(bo->tbo.base.resv);

	dma_fence_put(&ef->base);
	return ret;
>>>>>>> upstream/android-13
}

static int amdgpu_amdkfd_bo_validate(struct amdgpu_bo *bo, uint32_t domain,
				     bool wait)
{
	struct ttm_operation_ctx ctx = { false, false };
	int ret;

	if (WARN(amdgpu_ttm_tt_get_usermm(bo->tbo.ttm),
		 "Called with userptr BO"))
		return -EINVAL;

	amdgpu_bo_placement_from_domain(bo, domain);

	ret = ttm_bo_validate(&bo->tbo, &bo->placement, &ctx);
	if (ret)
		goto validate_fail;
<<<<<<< HEAD
	if (wait) {
		struct amdgpu_amdkfd_fence **ef_list;
		unsigned int ef_count;

		ret = amdgpu_amdkfd_remove_eviction_fence(bo, NULL, &ef_list,
							  &ef_count);
		if (ret)
			goto validate_fail;

		ttm_bo_wait(&bo->tbo, false, false);
		amdgpu_amdkfd_add_eviction_fence(bo, ef_list, ef_count);
	}
=======
	if (wait)
		amdgpu_bo_sync_wait(bo, AMDGPU_FENCE_OWNER_KFD, false);
>>>>>>> upstream/android-13

validate_fail:
	return ret;
}

<<<<<<< HEAD
static int amdgpu_amdkfd_validate(void *param, struct amdgpu_bo *bo)
{
	struct amdgpu_vm_parser *p = param;

	return amdgpu_amdkfd_bo_validate(bo, p->domain, p->wait);
=======
static int amdgpu_amdkfd_validate_vm_bo(void *_unused, struct amdgpu_bo *bo)
{
	return amdgpu_amdkfd_bo_validate(bo, bo->allowed_domains, false);
>>>>>>> upstream/android-13
}

/* vm_validate_pt_pd_bos - Validate page table and directory BOs
 *
 * Page directories are not updated here because huge page handling
 * during page table updates can invalidate page directory entries
 * again. Page directories are only updated after updating page
 * tables.
 */
static int vm_validate_pt_pd_bos(struct amdgpu_vm *vm)
{
<<<<<<< HEAD
	struct amdgpu_bo *pd = vm->root.base.bo;
	struct amdgpu_device *adev = amdgpu_ttm_adev(pd->tbo.bdev);
	struct amdgpu_vm_parser param;
	uint64_t addr, flags = AMDGPU_PTE_VALID;
	int ret;

	param.domain = AMDGPU_GEM_DOMAIN_VRAM;
	param.wait = false;

	ret = amdgpu_vm_validate_pt_bos(adev, vm, amdgpu_amdkfd_validate,
					&param);
	if (ret) {
		pr_err("amdgpu: failed to validate PT BOs\n");
		return ret;
	}

	ret = amdgpu_amdkfd_validate(&param, pd);
	if (ret) {
		pr_err("amdgpu: failed to validate PD\n");
		return ret;
	}

	addr = amdgpu_bo_gpu_offset(vm->root.base.bo);
	amdgpu_gmc_get_vm_pde(adev, -1, &addr, &flags);
	vm->pd_phys_addr = addr;
=======
	struct amdgpu_bo *pd = vm->root.bo;
	struct amdgpu_device *adev = amdgpu_ttm_adev(pd->tbo.bdev);
	int ret;

	ret = amdgpu_vm_validate_pt_bos(adev, vm, amdgpu_amdkfd_validate_vm_bo, NULL);
	if (ret) {
		pr_err("failed to validate PT BOs\n");
		return ret;
	}

	ret = amdgpu_amdkfd_validate_vm_bo(NULL, pd);
	if (ret) {
		pr_err("failed to validate PD\n");
		return ret;
	}

	vm->pd_phys_addr = amdgpu_gmc_pd_addr(vm->root.bo);
>>>>>>> upstream/android-13

	if (vm->use_cpu_for_update) {
		ret = amdgpu_bo_kmap(pd, NULL);
		if (ret) {
<<<<<<< HEAD
			pr_err("amdgpu: failed to kmap PD, ret=%d\n", ret);
=======
			pr_err("failed to kmap PD, ret=%d\n", ret);
>>>>>>> upstream/android-13
			return ret;
		}
	}

	return 0;
}

<<<<<<< HEAD
static int sync_vm_fence(struct amdgpu_device *adev, struct amdgpu_sync *sync,
			 struct dma_fence *f)
{
	int ret = amdgpu_sync_fence(adev, sync, f, false);

	/* Sync objects can't handle multiple GPUs (contexts) updating
	 * sync->last_vm_update. Fortunately we don't need it for
	 * KFD's purposes, so we can just drop that fence.
	 */
	if (sync->last_vm_update) {
		dma_fence_put(sync->last_vm_update);
		sync->last_vm_update = NULL;
	}

	return ret;
}

static int vm_update_pds(struct amdgpu_vm *vm, struct amdgpu_sync *sync)
{
	struct amdgpu_bo *pd = vm->root.base.bo;
	struct amdgpu_device *adev = amdgpu_ttm_adev(pd->tbo.bdev);
	int ret;

	ret = amdgpu_vm_update_directories(adev, vm);
	if (ret)
		return ret;

	return sync_vm_fence(adev, sync, vm->last_update);
}

/* add_bo_to_vm - Add a BO to a VM
=======
static int vm_update_pds(struct amdgpu_vm *vm, struct amdgpu_sync *sync)
{
	struct amdgpu_bo *pd = vm->root.bo;
	struct amdgpu_device *adev = amdgpu_ttm_adev(pd->tbo.bdev);
	int ret;

	ret = amdgpu_vm_update_pdes(adev, vm, false);
	if (ret)
		return ret;

	return amdgpu_sync_fence(sync, vm->last_update);
}

static uint64_t get_pte_flags(struct amdgpu_device *adev, struct kgd_mem *mem)
{
	struct amdgpu_device *bo_adev = amdgpu_ttm_adev(mem->bo->tbo.bdev);
	bool coherent = mem->alloc_flags & KFD_IOC_ALLOC_MEM_FLAGS_COHERENT;
	bool uncached = mem->alloc_flags & KFD_IOC_ALLOC_MEM_FLAGS_UNCACHED;
	uint32_t mapping_flags;
	uint64_t pte_flags;
	bool snoop = false;

	mapping_flags = AMDGPU_VM_PAGE_READABLE;
	if (mem->alloc_flags & KFD_IOC_ALLOC_MEM_FLAGS_WRITABLE)
		mapping_flags |= AMDGPU_VM_PAGE_WRITEABLE;
	if (mem->alloc_flags & KFD_IOC_ALLOC_MEM_FLAGS_EXECUTABLE)
		mapping_flags |= AMDGPU_VM_PAGE_EXECUTABLE;

	switch (adev->asic_type) {
	case CHIP_ARCTURUS:
		if (mem->alloc_flags & KFD_IOC_ALLOC_MEM_FLAGS_VRAM) {
			if (bo_adev == adev)
				mapping_flags |= coherent ?
					AMDGPU_VM_MTYPE_CC : AMDGPU_VM_MTYPE_RW;
			else
				mapping_flags |= coherent ?
					AMDGPU_VM_MTYPE_UC : AMDGPU_VM_MTYPE_NC;
		} else {
			mapping_flags |= coherent ?
				AMDGPU_VM_MTYPE_UC : AMDGPU_VM_MTYPE_NC;
		}
		break;
	case CHIP_ALDEBARAN:
		if (coherent && uncached) {
			if (adev->gmc.xgmi.connected_to_cpu ||
				!(mem->alloc_flags & KFD_IOC_ALLOC_MEM_FLAGS_VRAM))
				snoop = true;
			mapping_flags |= AMDGPU_VM_MTYPE_UC;
		} else if (mem->alloc_flags & KFD_IOC_ALLOC_MEM_FLAGS_VRAM) {
			if (bo_adev == adev) {
				mapping_flags |= coherent ?
					AMDGPU_VM_MTYPE_CC : AMDGPU_VM_MTYPE_RW;
				if (adev->gmc.xgmi.connected_to_cpu)
					snoop = true;
			} else {
				mapping_flags |= coherent ?
					AMDGPU_VM_MTYPE_UC : AMDGPU_VM_MTYPE_NC;
				if (amdgpu_xgmi_same_hive(adev, bo_adev))
					snoop = true;
			}
		} else {
			snoop = true;
			mapping_flags |= coherent ?
				AMDGPU_VM_MTYPE_UC : AMDGPU_VM_MTYPE_NC;
		}
		break;
	default:
		mapping_flags |= coherent ?
			AMDGPU_VM_MTYPE_UC : AMDGPU_VM_MTYPE_NC;
	}

	pte_flags = amdgpu_gem_va_map_flags(adev, mapping_flags);
	pte_flags |= snoop ? AMDGPU_PTE_SNOOPED : 0;

	return pte_flags;
}

static int
kfd_mem_dmamap_userptr(struct kgd_mem *mem,
		       struct kfd_mem_attachment *attachment)
{
	enum dma_data_direction direction =
		mem->alloc_flags & KFD_IOC_ALLOC_MEM_FLAGS_WRITABLE ?
		DMA_BIDIRECTIONAL : DMA_TO_DEVICE;
	struct ttm_operation_ctx ctx = {.interruptible = true};
	struct amdgpu_bo *bo = attachment->bo_va->base.bo;
	struct amdgpu_device *adev = attachment->adev;
	struct ttm_tt *src_ttm = mem->bo->tbo.ttm;
	struct ttm_tt *ttm = bo->tbo.ttm;
	int ret;

	ttm->sg = kmalloc(sizeof(*ttm->sg), GFP_KERNEL);
	if (unlikely(!ttm->sg))
		return -ENOMEM;

	if (WARN_ON(ttm->num_pages != src_ttm->num_pages))
		return -EINVAL;

	/* Same sequence as in amdgpu_ttm_tt_pin_userptr */
	ret = sg_alloc_table_from_pages(ttm->sg, src_ttm->pages,
					ttm->num_pages, 0,
					(u64)ttm->num_pages << PAGE_SHIFT,
					GFP_KERNEL);
	if (unlikely(ret))
		goto free_sg;

	ret = dma_map_sgtable(adev->dev, ttm->sg, direction, 0);
	if (unlikely(ret))
		goto release_sg;

	drm_prime_sg_to_dma_addr_array(ttm->sg, ttm->dma_address,
				       ttm->num_pages);

	amdgpu_bo_placement_from_domain(bo, AMDGPU_GEM_DOMAIN_GTT);
	ret = ttm_bo_validate(&bo->tbo, &bo->placement, &ctx);
	if (ret)
		goto unmap_sg;

	return 0;

unmap_sg:
	dma_unmap_sgtable(adev->dev, ttm->sg, direction, 0);
release_sg:
	pr_err("DMA map userptr failed: %d\n", ret);
	sg_free_table(ttm->sg);
free_sg:
	kfree(ttm->sg);
	ttm->sg = NULL;
	return ret;
}

static int
kfd_mem_dmamap_dmabuf(struct kfd_mem_attachment *attachment)
{
	struct ttm_operation_ctx ctx = {.interruptible = true};
	struct amdgpu_bo *bo = attachment->bo_va->base.bo;

	amdgpu_bo_placement_from_domain(bo, AMDGPU_GEM_DOMAIN_GTT);
	return ttm_bo_validate(&bo->tbo, &bo->placement, &ctx);
}

static int
kfd_mem_dmamap_attachment(struct kgd_mem *mem,
			  struct kfd_mem_attachment *attachment)
{
	switch (attachment->type) {
	case KFD_MEM_ATT_SHARED:
		return 0;
	case KFD_MEM_ATT_USERPTR:
		return kfd_mem_dmamap_userptr(mem, attachment);
	case KFD_MEM_ATT_DMABUF:
		return kfd_mem_dmamap_dmabuf(attachment);
	default:
		WARN_ON_ONCE(1);
	}
	return -EINVAL;
}

static void
kfd_mem_dmaunmap_userptr(struct kgd_mem *mem,
			 struct kfd_mem_attachment *attachment)
{
	enum dma_data_direction direction =
		mem->alloc_flags & KFD_IOC_ALLOC_MEM_FLAGS_WRITABLE ?
		DMA_BIDIRECTIONAL : DMA_TO_DEVICE;
	struct ttm_operation_ctx ctx = {.interruptible = false};
	struct amdgpu_bo *bo = attachment->bo_va->base.bo;
	struct amdgpu_device *adev = attachment->adev;
	struct ttm_tt *ttm = bo->tbo.ttm;

	if (unlikely(!ttm->sg))
		return;

	amdgpu_bo_placement_from_domain(bo, AMDGPU_GEM_DOMAIN_CPU);
	ttm_bo_validate(&bo->tbo, &bo->placement, &ctx);

	dma_unmap_sgtable(adev->dev, ttm->sg, direction, 0);
	sg_free_table(ttm->sg);
	kfree(ttm->sg);
	ttm->sg = NULL;
}

static void
kfd_mem_dmaunmap_dmabuf(struct kfd_mem_attachment *attachment)
{
	struct ttm_operation_ctx ctx = {.interruptible = true};
	struct amdgpu_bo *bo = attachment->bo_va->base.bo;

	amdgpu_bo_placement_from_domain(bo, AMDGPU_GEM_DOMAIN_CPU);
	ttm_bo_validate(&bo->tbo, &bo->placement, &ctx);
}

static void
kfd_mem_dmaunmap_attachment(struct kgd_mem *mem,
			    struct kfd_mem_attachment *attachment)
{
	switch (attachment->type) {
	case KFD_MEM_ATT_SHARED:
		break;
	case KFD_MEM_ATT_USERPTR:
		kfd_mem_dmaunmap_userptr(mem, attachment);
		break;
	case KFD_MEM_ATT_DMABUF:
		kfd_mem_dmaunmap_dmabuf(attachment);
		break;
	default:
		WARN_ON_ONCE(1);
	}
}

static int
kfd_mem_attach_userptr(struct amdgpu_device *adev, struct kgd_mem *mem,
		       struct amdgpu_bo **bo)
{
	unsigned long bo_size = mem->bo->tbo.base.size;
	struct drm_gem_object *gobj;
	int ret;

	ret = amdgpu_bo_reserve(mem->bo, false);
	if (ret)
		return ret;

	ret = amdgpu_gem_object_create(adev, bo_size, 1,
				       AMDGPU_GEM_DOMAIN_CPU,
				       AMDGPU_GEM_CREATE_PREEMPTIBLE,
				       ttm_bo_type_sg, mem->bo->tbo.base.resv,
				       &gobj);
	amdgpu_bo_unreserve(mem->bo);
	if (ret)
		return ret;

	*bo = gem_to_amdgpu_bo(gobj);
	(*bo)->parent = amdgpu_bo_ref(mem->bo);

	return 0;
}

static int
kfd_mem_attach_dmabuf(struct amdgpu_device *adev, struct kgd_mem *mem,
		      struct amdgpu_bo **bo)
{
	struct drm_gem_object *gobj;
	int ret;

	if (!mem->dmabuf) {
		mem->dmabuf = amdgpu_gem_prime_export(&mem->bo->tbo.base,
			mem->alloc_flags & KFD_IOC_ALLOC_MEM_FLAGS_WRITABLE ?
				DRM_RDWR : 0);
		if (IS_ERR(mem->dmabuf)) {
			ret = PTR_ERR(mem->dmabuf);
			mem->dmabuf = NULL;
			return ret;
		}
	}

	gobj = amdgpu_gem_prime_import(adev_to_drm(adev), mem->dmabuf);
	if (IS_ERR(gobj))
		return PTR_ERR(gobj);

	/* Import takes an extra reference on the dmabuf. Drop it now to
	 * avoid leaking it. We only need the one reference in
	 * kgd_mem->dmabuf.
	 */
	dma_buf_put(mem->dmabuf);

	*bo = gem_to_amdgpu_bo(gobj);
	(*bo)->flags |= AMDGPU_GEM_CREATE_PREEMPTIBLE;
	(*bo)->parent = amdgpu_bo_ref(mem->bo);

	return 0;
}

/* kfd_mem_attach - Add a BO to a VM
>>>>>>> upstream/android-13
 *
 * Everything that needs to bo done only once when a BO is first added
 * to a VM. It can later be mapped and unmapped many times without
 * repeating these steps.
 *
<<<<<<< HEAD
=======
 * 0. Create BO for DMA mapping, if needed
>>>>>>> upstream/android-13
 * 1. Allocate and initialize BO VA entry data structure
 * 2. Add BO to the VM
 * 3. Determine ASIC-specific PTE flags
 * 4. Alloc page tables and directories if needed
 * 4a.  Validate new page tables and directories
 */
<<<<<<< HEAD
static int add_bo_to_vm(struct amdgpu_device *adev, struct kgd_mem *mem,
		struct amdgpu_vm *vm, bool is_aql,
		struct kfd_bo_va_list **p_bo_va_entry)
{
	int ret;
	struct kfd_bo_va_list *bo_va_entry;
	struct amdgpu_bo *pd = vm->root.base.bo;
	struct amdgpu_bo *bo = mem->bo;
	uint64_t va = mem->va;
	struct list_head *list_bo_va = &mem->bo_va_list;
	unsigned long bo_size = bo->tbo.mem.size;
=======
static int kfd_mem_attach(struct amdgpu_device *adev, struct kgd_mem *mem,
		struct amdgpu_vm *vm, bool is_aql)
{
	struct amdgpu_device *bo_adev = amdgpu_ttm_adev(mem->bo->tbo.bdev);
	unsigned long bo_size = mem->bo->tbo.base.size;
	uint64_t va = mem->va;
	struct kfd_mem_attachment *attachment[2] = {NULL, NULL};
	struct amdgpu_bo *bo[2] = {NULL, NULL};
	int i, ret;
>>>>>>> upstream/android-13

	if (!va) {
		pr_err("Invalid VA when adding BO to VM\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	if (is_aql)
		va += bo_size;

	bo_va_entry = kzalloc(sizeof(*bo_va_entry), GFP_KERNEL);
	if (!bo_va_entry)
		return -ENOMEM;

	pr_debug("\t add VA 0x%llx - 0x%llx to vm %p\n", va,
			va + bo_size, vm);

	/* Add BO to VM internal data structures*/
	bo_va_entry->bo_va = amdgpu_vm_bo_add(adev, vm, bo);
	if (!bo_va_entry->bo_va) {
		ret = -EINVAL;
		pr_err("Failed to add BO object to VM. ret == %d\n",
				ret);
		goto err_vmadd;
	}

	bo_va_entry->va = va;
	bo_va_entry->pte_flags = amdgpu_gmc_get_pte_flags(adev,
							 mem->mapping_flags);
	bo_va_entry->kgd_dev = (void *)adev;
	list_add(&bo_va_entry->bo_list, list_bo_va);

	if (p_bo_va_entry)
		*p_bo_va_entry = bo_va_entry;

	/* Allocate new page tables if needed and validate
	 * them. Clearing of new page tables and validate need to wait
	 * on move fences. We don't want that to trigger the eviction
	 * fence, so remove it temporarily.
	 */
	amdgpu_amdkfd_remove_eviction_fence(pd,
					vm->process_info->eviction_fence,
					NULL, NULL);

	ret = amdgpu_vm_alloc_pts(adev, vm, va, amdgpu_bo_size(bo));
	if (ret) {
		pr_err("Failed to allocate pts, err=%d\n", ret);
		goto err_alloc_pts;
	}

	ret = vm_validate_pt_pd_bos(vm);
	if (ret) {
		pr_err("validate_pt_pd_bos() failed\n");
		goto err_alloc_pts;
	}

	/* Add the eviction fence back */
	amdgpu_bo_fence(pd, &vm->process_info->eviction_fence->base, true);

	return 0;

err_alloc_pts:
	amdgpu_bo_fence(pd, &vm->process_info->eviction_fence->base, true);
	amdgpu_vm_bo_rmv(adev, bo_va_entry->bo_va);
	list_del(&bo_va_entry->bo_list);
err_vmadd:
	kfree(bo_va_entry);
	return ret;
}

static void remove_bo_from_vm(struct amdgpu_device *adev,
		struct kfd_bo_va_list *entry, unsigned long size)
{
	pr_debug("\t remove VA 0x%llx - 0x%llx in entry %p\n",
			entry->va,
			entry->va + size, entry);
	amdgpu_vm_bo_rmv(adev, entry->bo_va);
	list_del(&entry->bo_list);
	kfree(entry);
=======
	for (i = 0; i <= is_aql; i++) {
		attachment[i] = kzalloc(sizeof(*attachment[i]), GFP_KERNEL);
		if (unlikely(!attachment[i])) {
			ret = -ENOMEM;
			goto unwind;
		}

		pr_debug("\t add VA 0x%llx - 0x%llx to vm %p\n", va,
			 va + bo_size, vm);

		if (adev == bo_adev || (mem->domain == AMDGPU_GEM_DOMAIN_VRAM &&
					amdgpu_xgmi_same_hive(adev, bo_adev))) {
			/* Mappings on the local GPU and VRAM mappings in the
			 * local hive share the original BO
			 */
			attachment[i]->type = KFD_MEM_ATT_SHARED;
			bo[i] = mem->bo;
			drm_gem_object_get(&bo[i]->tbo.base);
		} else if (i > 0) {
			/* Multiple mappings on the same GPU share the BO */
			attachment[i]->type = KFD_MEM_ATT_SHARED;
			bo[i] = bo[0];
			drm_gem_object_get(&bo[i]->tbo.base);
		} else if (amdgpu_ttm_tt_get_usermm(mem->bo->tbo.ttm)) {
			/* Create an SG BO to DMA-map userptrs on other GPUs */
			attachment[i]->type = KFD_MEM_ATT_USERPTR;
			ret = kfd_mem_attach_userptr(adev, mem, &bo[i]);
			if (ret)
				goto unwind;
		} else if (mem->domain == AMDGPU_GEM_DOMAIN_GTT &&
			   mem->bo->tbo.type != ttm_bo_type_sg) {
			/* GTT BOs use DMA-mapping ability of dynamic-attach
			 * DMA bufs. TODO: The same should work for VRAM on
			 * large-BAR GPUs.
			 */
			attachment[i]->type = KFD_MEM_ATT_DMABUF;
			ret = kfd_mem_attach_dmabuf(adev, mem, &bo[i]);
			if (ret)
				goto unwind;
		} else {
			/* FIXME: Need to DMA-map other BO types:
			 * large-BAR VRAM, doorbells, MMIO remap
			 */
			attachment[i]->type = KFD_MEM_ATT_SHARED;
			bo[i] = mem->bo;
			drm_gem_object_get(&bo[i]->tbo.base);
		}

		/* Add BO to VM internal data structures */
		attachment[i]->bo_va = amdgpu_vm_bo_add(adev, vm, bo[i]);
		if (unlikely(!attachment[i]->bo_va)) {
			ret = -ENOMEM;
			pr_err("Failed to add BO object to VM. ret == %d\n",
			       ret);
			goto unwind;
		}

		attachment[i]->va = va;
		attachment[i]->pte_flags = get_pte_flags(adev, mem);
		attachment[i]->adev = adev;
		list_add(&attachment[i]->list, &mem->attachments);

		va += bo_size;
	}

	return 0;

unwind:
	for (; i >= 0; i--) {
		if (!attachment[i])
			continue;
		if (attachment[i]->bo_va) {
			amdgpu_vm_bo_rmv(adev, attachment[i]->bo_va);
			list_del(&attachment[i]->list);
		}
		if (bo[i])
			drm_gem_object_put(&bo[i]->tbo.base);
		kfree(attachment[i]);
	}
	return ret;
}

static void kfd_mem_detach(struct kfd_mem_attachment *attachment)
{
	struct amdgpu_bo *bo = attachment->bo_va->base.bo;

	pr_debug("\t remove VA 0x%llx in entry %p\n",
			attachment->va, attachment);
	amdgpu_vm_bo_rmv(attachment->adev, attachment->bo_va);
	drm_gem_object_put(&bo->tbo.base);
	list_del(&attachment->list);
	kfree(attachment);
>>>>>>> upstream/android-13
}

static void add_kgd_mem_to_kfd_bo_list(struct kgd_mem *mem,
				struct amdkfd_process_info *process_info,
				bool userptr)
{
	struct ttm_validate_buffer *entry = &mem->validate_list;
	struct amdgpu_bo *bo = mem->bo;

	INIT_LIST_HEAD(&entry->head);
<<<<<<< HEAD
	entry->shared = true;
=======
	entry->num_shared = 1;
>>>>>>> upstream/android-13
	entry->bo = &bo->tbo;
	mutex_lock(&process_info->lock);
	if (userptr)
		list_add_tail(&entry->head, &process_info->userptr_valid_list);
	else
		list_add_tail(&entry->head, &process_info->kfd_bo_list);
	mutex_unlock(&process_info->lock);
}

<<<<<<< HEAD
=======
static void remove_kgd_mem_from_kfd_bo_list(struct kgd_mem *mem,
		struct amdkfd_process_info *process_info)
{
	struct ttm_validate_buffer *bo_list_entry;

	bo_list_entry = &mem->validate_list;
	mutex_lock(&process_info->lock);
	list_del(&bo_list_entry->head);
	mutex_unlock(&process_info->lock);
}

>>>>>>> upstream/android-13
/* Initializes user pages. It registers the MMU notifier and validates
 * the userptr BO in the GTT domain.
 *
 * The BO must already be on the userptr_valid_list. Otherwise an
 * eviction and restore may happen that leaves the new BO unmapped
 * with the user mode queues running.
 *
 * Takes the process_info->lock to protect against concurrent restore
 * workers.
 *
 * Returns 0 for success, negative errno for errors.
 */
<<<<<<< HEAD
static int init_user_pages(struct kgd_mem *mem, struct mm_struct *mm,
			   uint64_t user_addr)
=======
static int init_user_pages(struct kgd_mem *mem, uint64_t user_addr)
>>>>>>> upstream/android-13
{
	struct amdkfd_process_info *process_info = mem->process_info;
	struct amdgpu_bo *bo = mem->bo;
	struct ttm_operation_ctx ctx = { true, false };
	int ret = 0;

	mutex_lock(&process_info->lock);

<<<<<<< HEAD
	ret = amdgpu_ttm_tt_set_userptr(bo->tbo.ttm, user_addr, 0);
=======
	ret = amdgpu_ttm_tt_set_userptr(&bo->tbo, user_addr, 0);
>>>>>>> upstream/android-13
	if (ret) {
		pr_err("%s: Failed to set userptr: %d\n", __func__, ret);
		goto out;
	}

	ret = amdgpu_mn_register(bo, user_addr);
	if (ret) {
		pr_err("%s: Failed to register MMU notifier: %d\n",
		       __func__, ret);
		goto out;
	}

<<<<<<< HEAD
	/* If no restore worker is running concurrently, user_pages
	 * should not be allocated
	 */
	WARN(mem->user_pages, "Leaking user_pages array");

	mem->user_pages = kvmalloc_array(bo->tbo.ttm->num_pages,
					   sizeof(struct page *),
					   GFP_KERNEL | __GFP_ZERO);
	if (!mem->user_pages) {
		pr_err("%s: Failed to allocate pages array\n", __func__);
		ret = -ENOMEM;
		goto unregister_out;
	}

	ret = amdgpu_ttm_tt_get_user_pages(bo->tbo.ttm, mem->user_pages);
	if (ret) {
		pr_err("%s: Failed to get user pages: %d\n", __func__, ret);
		goto free_out;
	}

	amdgpu_ttm_tt_set_user_pages(bo->tbo.ttm, mem->user_pages);

=======
	ret = amdgpu_ttm_tt_get_user_pages(bo, bo->tbo.ttm->pages);
	if (ret) {
		pr_err("%s: Failed to get user pages: %d\n", __func__, ret);
		goto unregister_out;
	}

>>>>>>> upstream/android-13
	ret = amdgpu_bo_reserve(bo, true);
	if (ret) {
		pr_err("%s: Failed to reserve BO\n", __func__);
		goto release_out;
	}
	amdgpu_bo_placement_from_domain(bo, mem->domain);
	ret = ttm_bo_validate(&bo->tbo, &bo->placement, &ctx);
	if (ret)
		pr_err("%s: failed to validate BO\n", __func__);
	amdgpu_bo_unreserve(bo);

release_out:
<<<<<<< HEAD
	if (ret)
		release_pages(mem->user_pages, bo->tbo.ttm->num_pages);
free_out:
	kvfree(mem->user_pages);
	mem->user_pages = NULL;
=======
	amdgpu_ttm_tt_get_user_pages_done(bo->tbo.ttm);
>>>>>>> upstream/android-13
unregister_out:
	if (ret)
		amdgpu_mn_unregister(bo);
out:
	mutex_unlock(&process_info->lock);
	return ret;
}

/* Reserving a BO and its page table BOs must happen atomically to
 * avoid deadlocks. Some operations update multiple VMs at once. Track
 * all the reservation info in a context structure. Optionally a sync
 * object can track VM updates.
 */
struct bo_vm_reservation_context {
	struct amdgpu_bo_list_entry kfd_bo; /* BO list entry for the KFD BO */
	unsigned int n_vms;		    /* Number of VMs reserved	    */
	struct amdgpu_bo_list_entry *vm_pd; /* Array of VM BO list entries  */
	struct ww_acquire_ctx ticket;	    /* Reservation ticket	    */
	struct list_head list, duplicates;  /* BO lists			    */
	struct amdgpu_sync *sync;	    /* Pointer to sync object	    */
	bool reserved;			    /* Whether BOs are reserved	    */
};

enum bo_vm_match {
	BO_VM_NOT_MAPPED = 0,	/* Match VMs where a BO is not mapped */
	BO_VM_MAPPED,		/* Match VMs where a BO is mapped     */
	BO_VM_ALL,		/* Match all VMs a BO was added to    */
};

/**
 * reserve_bo_and_vm - reserve a BO and a VM unconditionally.
 * @mem: KFD BO structure.
 * @vm: the VM to reserve.
 * @ctx: the struct that will be used in unreserve_bo_and_vms().
 */
static int reserve_bo_and_vm(struct kgd_mem *mem,
			      struct amdgpu_vm *vm,
			      struct bo_vm_reservation_context *ctx)
{
	struct amdgpu_bo *bo = mem->bo;
	int ret;

	WARN_ON(!vm);

	ctx->reserved = false;
	ctx->n_vms = 1;
	ctx->sync = &mem->sync;

	INIT_LIST_HEAD(&ctx->list);
	INIT_LIST_HEAD(&ctx->duplicates);

	ctx->vm_pd = kcalloc(ctx->n_vms, sizeof(*ctx->vm_pd), GFP_KERNEL);
	if (!ctx->vm_pd)
		return -ENOMEM;

<<<<<<< HEAD
	ctx->kfd_bo.robj = bo;
	ctx->kfd_bo.priority = 0;
	ctx->kfd_bo.tv.bo = &bo->tbo;
	ctx->kfd_bo.tv.shared = true;
	ctx->kfd_bo.user_pages = NULL;
=======
	ctx->kfd_bo.priority = 0;
	ctx->kfd_bo.tv.bo = &bo->tbo;
	ctx->kfd_bo.tv.num_shared = 1;
>>>>>>> upstream/android-13
	list_add(&ctx->kfd_bo.tv.head, &ctx->list);

	amdgpu_vm_get_pd_bo(vm, &ctx->list, &ctx->vm_pd[0]);

	ret = ttm_eu_reserve_buffers(&ctx->ticket, &ctx->list,
				     false, &ctx->duplicates);
<<<<<<< HEAD
	if (!ret)
		ctx->reserved = true;
	else {
		pr_err("Failed to reserve buffers in ttm\n");
		kfree(ctx->vm_pd);
		ctx->vm_pd = NULL;
	}

	return ret;
=======
	if (ret) {
		pr_err("Failed to reserve buffers in ttm.\n");
		kfree(ctx->vm_pd);
		ctx->vm_pd = NULL;
		return ret;
	}

	ctx->reserved = true;
	return 0;
>>>>>>> upstream/android-13
}

/**
 * reserve_bo_and_cond_vms - reserve a BO and some VMs conditionally
 * @mem: KFD BO structure.
 * @vm: the VM to reserve. If NULL, then all VMs associated with the BO
 * is used. Otherwise, a single VM associated with the BO.
 * @map_type: the mapping status that will be used to filter the VMs.
 * @ctx: the struct that will be used in unreserve_bo_and_vms().
 *
 * Returns 0 for success, negative for failure.
 */
static int reserve_bo_and_cond_vms(struct kgd_mem *mem,
				struct amdgpu_vm *vm, enum bo_vm_match map_type,
				struct bo_vm_reservation_context *ctx)
{
	struct amdgpu_bo *bo = mem->bo;
<<<<<<< HEAD
	struct kfd_bo_va_list *entry;
=======
	struct kfd_mem_attachment *entry;
>>>>>>> upstream/android-13
	unsigned int i;
	int ret;

	ctx->reserved = false;
	ctx->n_vms = 0;
	ctx->vm_pd = NULL;
	ctx->sync = &mem->sync;

	INIT_LIST_HEAD(&ctx->list);
	INIT_LIST_HEAD(&ctx->duplicates);

<<<<<<< HEAD
	list_for_each_entry(entry, &mem->bo_va_list, bo_list) {
=======
	list_for_each_entry(entry, &mem->attachments, list) {
>>>>>>> upstream/android-13
		if ((vm && vm != entry->bo_va->base.vm) ||
			(entry->is_mapped != map_type
			&& map_type != BO_VM_ALL))
			continue;

		ctx->n_vms++;
	}

	if (ctx->n_vms != 0) {
		ctx->vm_pd = kcalloc(ctx->n_vms, sizeof(*ctx->vm_pd),
				     GFP_KERNEL);
		if (!ctx->vm_pd)
			return -ENOMEM;
	}

<<<<<<< HEAD
	ctx->kfd_bo.robj = bo;
	ctx->kfd_bo.priority = 0;
	ctx->kfd_bo.tv.bo = &bo->tbo;
	ctx->kfd_bo.tv.shared = true;
	ctx->kfd_bo.user_pages = NULL;
	list_add(&ctx->kfd_bo.tv.head, &ctx->list);

	i = 0;
	list_for_each_entry(entry, &mem->bo_va_list, bo_list) {
=======
	ctx->kfd_bo.priority = 0;
	ctx->kfd_bo.tv.bo = &bo->tbo;
	ctx->kfd_bo.tv.num_shared = 1;
	list_add(&ctx->kfd_bo.tv.head, &ctx->list);

	i = 0;
	list_for_each_entry(entry, &mem->attachments, list) {
>>>>>>> upstream/android-13
		if ((vm && vm != entry->bo_va->base.vm) ||
			(entry->is_mapped != map_type
			&& map_type != BO_VM_ALL))
			continue;

		amdgpu_vm_get_pd_bo(entry->bo_va->base.vm, &ctx->list,
				&ctx->vm_pd[i]);
		i++;
	}

	ret = ttm_eu_reserve_buffers(&ctx->ticket, &ctx->list,
				     false, &ctx->duplicates);
<<<<<<< HEAD
	if (!ret)
		ctx->reserved = true;
	else
		pr_err("Failed to reserve buffers in ttm.\n");

	if (ret) {
		kfree(ctx->vm_pd);
		ctx->vm_pd = NULL;
	}

	return ret;
=======
	if (ret) {
		pr_err("Failed to reserve buffers in ttm.\n");
		kfree(ctx->vm_pd);
		ctx->vm_pd = NULL;
		return ret;
	}

	ctx->reserved = true;
	return 0;
>>>>>>> upstream/android-13
}

/**
 * unreserve_bo_and_vms - Unreserve BO and VMs from a reservation context
 * @ctx: Reservation context to unreserve
 * @wait: Optionally wait for a sync object representing pending VM updates
 * @intr: Whether the wait is interruptible
 *
 * Also frees any resources allocated in
 * reserve_bo_and_(cond_)vm(s). Returns the status from
 * amdgpu_sync_wait.
 */
static int unreserve_bo_and_vms(struct bo_vm_reservation_context *ctx,
				 bool wait, bool intr)
{
	int ret = 0;

	if (wait)
		ret = amdgpu_sync_wait(ctx->sync, intr);

	if (ctx->reserved)
		ttm_eu_backoff_reservation(&ctx->ticket, &ctx->list);
	kfree(ctx->vm_pd);

	ctx->sync = NULL;

	ctx->reserved = false;
	ctx->vm_pd = NULL;

	return ret;
}

<<<<<<< HEAD
static int unmap_bo_from_gpuvm(struct amdgpu_device *adev,
				struct kfd_bo_va_list *entry,
				struct amdgpu_sync *sync)
{
	struct amdgpu_bo_va *bo_va = entry->bo_va;
	struct amdgpu_vm *vm = bo_va->base.vm;
	struct amdgpu_bo *pd = vm->root.base.bo;

	/* Remove eviction fence from PD (and thereby from PTs too as
	 * they share the resv. object). Otherwise during PT update
	 * job (see amdgpu_vm_bo_update_mapping), eviction fence would
	 * get added to job->sync object and job execution would
	 * trigger the eviction fence.
	 */
	amdgpu_amdkfd_remove_eviction_fence(pd,
					    vm->process_info->eviction_fence,
					    NULL, NULL);
=======
static void unmap_bo_from_gpuvm(struct kgd_mem *mem,
				struct kfd_mem_attachment *entry,
				struct amdgpu_sync *sync)
{
	struct amdgpu_bo_va *bo_va = entry->bo_va;
	struct amdgpu_device *adev = entry->adev;
	struct amdgpu_vm *vm = bo_va->base.vm;

>>>>>>> upstream/android-13
	amdgpu_vm_bo_unmap(adev, bo_va, entry->va);

	amdgpu_vm_clear_freed(adev, vm, &bo_va->last_pt_update);

<<<<<<< HEAD
	/* Add the eviction fence back */
	amdgpu_bo_fence(pd, &vm->process_info->eviction_fence->base, true);

	sync_vm_fence(adev, sync, bo_va->last_pt_update);

	return 0;
}

static int update_gpuvm_pte(struct amdgpu_device *adev,
		struct kfd_bo_va_list *entry,
		struct amdgpu_sync *sync)
{
	int ret;
	struct amdgpu_vm *vm;
	struct amdgpu_bo_va *bo_va;
	struct amdgpu_bo *bo;

	bo_va = entry->bo_va;
	vm = bo_va->base.vm;
	bo = bo_va->base.bo;

	/* Update the page tables  */
	ret = amdgpu_vm_bo_update(adev, bo_va, false);
=======
	amdgpu_sync_fence(sync, bo_va->last_pt_update);

	kfd_mem_dmaunmap_attachment(mem, entry);
}

static int update_gpuvm_pte(struct kgd_mem *mem,
			    struct kfd_mem_attachment *entry,
			    struct amdgpu_sync *sync,
			    bool *table_freed)
{
	struct amdgpu_bo_va *bo_va = entry->bo_va;
	struct amdgpu_device *adev = entry->adev;
	int ret;

	ret = kfd_mem_dmamap_attachment(mem, entry);
	if (ret)
		return ret;

	/* Update the page tables  */
	ret = amdgpu_vm_bo_update(adev, bo_va, false, table_freed);
>>>>>>> upstream/android-13
	if (ret) {
		pr_err("amdgpu_vm_bo_update failed\n");
		return ret;
	}

<<<<<<< HEAD
	return sync_vm_fence(adev, sync, bo_va->last_pt_update);
}

static int map_bo_to_gpuvm(struct amdgpu_device *adev,
		struct kfd_bo_va_list *entry, struct amdgpu_sync *sync,
		bool no_update_pte)
=======
	return amdgpu_sync_fence(sync, bo_va->last_pt_update);
}

static int map_bo_to_gpuvm(struct kgd_mem *mem,
			   struct kfd_mem_attachment *entry,
			   struct amdgpu_sync *sync,
			   bool no_update_pte,
			   bool *table_freed)
>>>>>>> upstream/android-13
{
	int ret;

	/* Set virtual address for the allocation */
<<<<<<< HEAD
	ret = amdgpu_vm_bo_map(adev, entry->bo_va, entry->va, 0,
=======
	ret = amdgpu_vm_bo_map(entry->adev, entry->bo_va, entry->va, 0,
>>>>>>> upstream/android-13
			       amdgpu_bo_size(entry->bo_va->base.bo),
			       entry->pte_flags);
	if (ret) {
		pr_err("Failed to map VA 0x%llx in vm. ret %d\n",
				entry->va, ret);
		return ret;
	}

	if (no_update_pte)
		return 0;

<<<<<<< HEAD
	ret = update_gpuvm_pte(adev, entry, sync);
=======
	ret = update_gpuvm_pte(mem, entry, sync, table_freed);
>>>>>>> upstream/android-13
	if (ret) {
		pr_err("update_gpuvm_pte() failed\n");
		goto update_gpuvm_pte_failed;
	}

	return 0;

update_gpuvm_pte_failed:
<<<<<<< HEAD
	unmap_bo_from_gpuvm(adev, entry, sync);
	return ret;
}

=======
	unmap_bo_from_gpuvm(mem, entry, sync);
	return ret;
}

static struct sg_table *create_doorbell_sg(uint64_t addr, uint32_t size)
{
	struct sg_table *sg = kmalloc(sizeof(*sg), GFP_KERNEL);

	if (!sg)
		return NULL;
	if (sg_alloc_table(sg, 1, GFP_KERNEL)) {
		kfree(sg);
		return NULL;
	}
	sg->sgl->dma_address = addr;
	sg->sgl->length = size;
#ifdef CONFIG_NEED_SG_DMA_LENGTH
	sg->sgl->dma_length = size;
#endif
	return sg;
}

>>>>>>> upstream/android-13
static int process_validate_vms(struct amdkfd_process_info *process_info)
{
	struct amdgpu_vm *peer_vm;
	int ret;

	list_for_each_entry(peer_vm, &process_info->vm_list_head,
			    vm_list_node) {
		ret = vm_validate_pt_pd_bos(peer_vm);
		if (ret)
			return ret;
	}

	return 0;
}

<<<<<<< HEAD
=======
static int process_sync_pds_resv(struct amdkfd_process_info *process_info,
				 struct amdgpu_sync *sync)
{
	struct amdgpu_vm *peer_vm;
	int ret;

	list_for_each_entry(peer_vm, &process_info->vm_list_head,
			    vm_list_node) {
		struct amdgpu_bo *pd = peer_vm->root.bo;

		ret = amdgpu_sync_resv(NULL, sync, pd->tbo.base.resv,
				       AMDGPU_SYNC_NE_OWNER,
				       AMDGPU_FENCE_OWNER_KFD);
		if (ret)
			return ret;
	}

	return 0;
}

>>>>>>> upstream/android-13
static int process_update_pds(struct amdkfd_process_info *process_info,
			      struct amdgpu_sync *sync)
{
	struct amdgpu_vm *peer_vm;
	int ret;

	list_for_each_entry(peer_vm, &process_info->vm_list_head,
			    vm_list_node) {
		ret = vm_update_pds(peer_vm, sync);
		if (ret)
			return ret;
	}

	return 0;
}

static int init_kfd_vm(struct amdgpu_vm *vm, void **process_info,
		       struct dma_fence **ef)
{
	struct amdkfd_process_info *info = NULL;
	int ret;

	if (!*process_info) {
		info = kzalloc(sizeof(*info), GFP_KERNEL);
		if (!info)
			return -ENOMEM;

		mutex_init(&info->lock);
		INIT_LIST_HEAD(&info->vm_list_head);
		INIT_LIST_HEAD(&info->kfd_bo_list);
		INIT_LIST_HEAD(&info->userptr_valid_list);
		INIT_LIST_HEAD(&info->userptr_inval_list);

		info->eviction_fence =
			amdgpu_amdkfd_fence_create(dma_fence_context_alloc(1),
<<<<<<< HEAD
						   current->mm);
=======
						   current->mm,
						   NULL);
>>>>>>> upstream/android-13
		if (!info->eviction_fence) {
			pr_err("Failed to create eviction fence\n");
			ret = -ENOMEM;
			goto create_evict_fence_fail;
		}

		info->pid = get_task_pid(current->group_leader, PIDTYPE_PID);
		atomic_set(&info->evicted_bos, 0);
		INIT_DELAYED_WORK(&info->restore_userptr_work,
				  amdgpu_amdkfd_restore_userptr_worker);

		*process_info = info;
		*ef = dma_fence_get(&info->eviction_fence->base);
	}

	vm->process_info = *process_info;

	/* Validate page directory and attach eviction fence */
<<<<<<< HEAD
	ret = amdgpu_bo_reserve(vm->root.base.bo, true);
=======
	ret = amdgpu_bo_reserve(vm->root.bo, true);
>>>>>>> upstream/android-13
	if (ret)
		goto reserve_pd_fail;
	ret = vm_validate_pt_pd_bos(vm);
	if (ret) {
		pr_err("validate_pt_pd_bos() failed\n");
		goto validate_pd_fail;
	}
<<<<<<< HEAD
	ret = ttm_bo_wait(&vm->root.base.bo->tbo, false, false);
	if (ret)
		goto wait_pd_fail;
	amdgpu_bo_fence(vm->root.base.bo,
			&vm->process_info->eviction_fence->base, true);
	amdgpu_bo_unreserve(vm->root.base.bo);
=======
	ret = amdgpu_bo_sync_wait(vm->root.bo,
				  AMDGPU_FENCE_OWNER_KFD, false);
	if (ret)
		goto wait_pd_fail;
	ret = dma_resv_reserve_shared(vm->root.bo->tbo.base.resv, 1);
	if (ret)
		goto reserve_shared_fail;
	amdgpu_bo_fence(vm->root.bo,
			&vm->process_info->eviction_fence->base, true);
	amdgpu_bo_unreserve(vm->root.bo);
>>>>>>> upstream/android-13

	/* Update process info */
	mutex_lock(&vm->process_info->lock);
	list_add_tail(&vm->vm_list_node,
			&(vm->process_info->vm_list_head));
	vm->process_info->n_vms++;
	mutex_unlock(&vm->process_info->lock);

	return 0;

<<<<<<< HEAD
wait_pd_fail:
validate_pd_fail:
	amdgpu_bo_unreserve(vm->root.base.bo);
=======
reserve_shared_fail:
wait_pd_fail:
validate_pd_fail:
	amdgpu_bo_unreserve(vm->root.bo);
>>>>>>> upstream/android-13
reserve_pd_fail:
	vm->process_info = NULL;
	if (info) {
		/* Two fence references: one in info and one in *ef */
		dma_fence_put(&info->eviction_fence->base);
		dma_fence_put(*ef);
		*ef = NULL;
		*process_info = NULL;
		put_pid(info->pid);
create_evict_fence_fail:
		mutex_destroy(&info->lock);
		kfree(info);
	}
	return ret;
}

<<<<<<< HEAD
int amdgpu_amdkfd_gpuvm_create_process_vm(struct kgd_dev *kgd, void **vm,
					  void **process_info,
					  struct dma_fence **ef)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct amdgpu_vm *new_vm;
	int ret;

	new_vm = kzalloc(sizeof(*new_vm), GFP_KERNEL);
	if (!new_vm)
		return -ENOMEM;

	/* Initialize AMDGPU part of the VM */
	ret = amdgpu_vm_init(adev, new_vm, AMDGPU_VM_CONTEXT_COMPUTE, 0);
	if (ret) {
		pr_err("Failed init vm ret %d\n", ret);
		goto amdgpu_vm_init_fail;
	}

	/* Initialize KFD part of the VM and process info */
	ret = init_kfd_vm(new_vm, process_info, ef);
	if (ret)
		goto init_kfd_vm_fail;

	*vm = (void *) new_vm;

	return 0;

init_kfd_vm_fail:
	amdgpu_vm_fini(adev, new_vm);
amdgpu_vm_init_fail:
	kfree(new_vm);
	return ret;
}

int amdgpu_amdkfd_gpuvm_acquire_process_vm(struct kgd_dev *kgd,
					   struct file *filp,
					   void **vm, void **process_info,
					   struct dma_fence **ef)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct drm_file *drm_priv = filp->private_data;
	struct amdgpu_fpriv *drv_priv = drm_priv->driver_priv;
	struct amdgpu_vm *avm = &drv_priv->vm;
	int ret;

=======
int amdgpu_amdkfd_gpuvm_acquire_process_vm(struct kgd_dev *kgd,
					   struct file *filp, u32 pasid,
					   void **process_info,
					   struct dma_fence **ef)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct amdgpu_fpriv *drv_priv;
	struct amdgpu_vm *avm;
	int ret;

	ret = amdgpu_file_to_fpriv(filp, &drv_priv);
	if (ret)
		return ret;
	avm = &drv_priv->vm;

>>>>>>> upstream/android-13
	/* Already a compute VM? */
	if (avm->process_info)
		return -EINVAL;

<<<<<<< HEAD
=======
	/* Free the original amdgpu allocated pasid,
	 * will be replaced with kfd allocated pasid.
	 */
	if (avm->pasid) {
		amdgpu_pasid_free(avm->pasid);
		amdgpu_vm_set_pasid(adev, avm, 0);
	}

>>>>>>> upstream/android-13
	/* Convert VM into a compute VM */
	ret = amdgpu_vm_make_compute(adev, avm);
	if (ret)
		return ret;

<<<<<<< HEAD
=======
	ret = amdgpu_vm_set_pasid(adev, avm, pasid);
	if (ret)
		return ret;
>>>>>>> upstream/android-13
	/* Initialize KFD part of the VM and process info */
	ret = init_kfd_vm(avm, process_info, ef);
	if (ret)
		return ret;

<<<<<<< HEAD
	*vm = (void *)avm;
=======
	amdgpu_vm_set_task_info(avm);
>>>>>>> upstream/android-13

	return 0;
}

void amdgpu_amdkfd_gpuvm_destroy_cb(struct amdgpu_device *adev,
				    struct amdgpu_vm *vm)
{
	struct amdkfd_process_info *process_info = vm->process_info;
<<<<<<< HEAD
	struct amdgpu_bo *pd = vm->root.base.bo;
=======
	struct amdgpu_bo *pd = vm->root.bo;
>>>>>>> upstream/android-13

	if (!process_info)
		return;

	/* Release eviction fence from PD */
	amdgpu_bo_reserve(pd, false);
	amdgpu_bo_fence(pd, NULL, false);
	amdgpu_bo_unreserve(pd);

	/* Update process info */
	mutex_lock(&process_info->lock);
	process_info->n_vms--;
	list_del(&vm->vm_list_node);
	mutex_unlock(&process_info->lock);

<<<<<<< HEAD
=======
	vm->process_info = NULL;

>>>>>>> upstream/android-13
	/* Release per-process resources when last compute VM is destroyed */
	if (!process_info->n_vms) {
		WARN_ON(!list_empty(&process_info->kfd_bo_list));
		WARN_ON(!list_empty(&process_info->userptr_valid_list));
		WARN_ON(!list_empty(&process_info->userptr_inval_list));

		dma_fence_put(&process_info->eviction_fence->base);
		cancel_delayed_work_sync(&process_info->restore_userptr_work);
		put_pid(process_info->pid);
		mutex_destroy(&process_info->lock);
		kfree(process_info);
	}
}

<<<<<<< HEAD
void amdgpu_amdkfd_gpuvm_destroy_process_vm(struct kgd_dev *kgd, void *vm)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct amdgpu_vm *avm = (struct amdgpu_vm *)vm;

	if (WARN_ON(!kgd || !vm))
		return;

	pr_debug("Destroying process vm %p\n", vm);

	/* Release the VM context */
	amdgpu_vm_fini(adev, avm);
	kfree(vm);
}

uint32_t amdgpu_amdkfd_gpuvm_get_process_page_dir(void *vm)
{
	struct amdgpu_vm *avm = (struct amdgpu_vm *)vm;

	return avm->pd_phys_addr >> AMDGPU_GPU_PAGE_SHIFT;
=======
void amdgpu_amdkfd_gpuvm_release_process_vm(struct kgd_dev *kgd, void *drm_priv)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct amdgpu_vm *avm;

	if (WARN_ON(!kgd || !drm_priv))
		return;

	avm = drm_priv_to_vm(drm_priv);

	pr_debug("Releasing process vm %p\n", avm);

	/* The original pasid of amdgpu vm has already been
	 * released during making a amdgpu vm to a compute vm
	 * The current pasid is managed by kfd and will be
	 * released on kfd process destroy. Set amdgpu pasid
	 * to 0 to avoid duplicate release.
	 */
	amdgpu_vm_release_compute(adev, avm);
}

uint64_t amdgpu_amdkfd_gpuvm_get_process_page_dir(void *drm_priv)
{
	struct amdgpu_vm *avm = drm_priv_to_vm(drm_priv);
	struct amdgpu_bo *pd = avm->root.bo;
	struct amdgpu_device *adev = amdgpu_ttm_adev(pd->tbo.bdev);

	if (adev->asic_type < CHIP_VEGA10)
		return avm->pd_phys_addr >> AMDGPU_GPU_PAGE_SHIFT;
	return avm->pd_phys_addr;
>>>>>>> upstream/android-13
}

int amdgpu_amdkfd_gpuvm_alloc_memory_of_gpu(
		struct kgd_dev *kgd, uint64_t va, uint64_t size,
<<<<<<< HEAD
		void *vm, struct kgd_mem **mem,
		uint64_t *offset, uint32_t flags)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct amdgpu_vm *avm = (struct amdgpu_vm *)vm;
	uint64_t user_addr = 0;
	struct amdgpu_bo *bo;
	struct amdgpu_bo_param bp;
	int byte_align;
	u32 domain, alloc_domain;
	u64 alloc_flags;
	uint32_t mapping_flags;
=======
		void *drm_priv, struct kgd_mem **mem,
		uint64_t *offset, uint32_t flags)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct amdgpu_vm *avm = drm_priv_to_vm(drm_priv);
	enum ttm_bo_type bo_type = ttm_bo_type_device;
	struct sg_table *sg = NULL;
	uint64_t user_addr = 0;
	struct amdgpu_bo *bo;
	struct drm_gem_object *gobj = NULL;
	u32 domain, alloc_domain;
	u64 alloc_flags;
>>>>>>> upstream/android-13
	int ret;

	/*
	 * Check on which domain to allocate BO
	 */
<<<<<<< HEAD
	if (flags & ALLOC_MEM_FLAGS_VRAM) {
		domain = alloc_domain = AMDGPU_GEM_DOMAIN_VRAM;
		alloc_flags = AMDGPU_GEM_CREATE_VRAM_CLEARED;
		alloc_flags |= (flags & ALLOC_MEM_FLAGS_PUBLIC) ?
			AMDGPU_GEM_CREATE_CPU_ACCESS_REQUIRED :
			AMDGPU_GEM_CREATE_NO_CPU_ACCESS;
	} else if (flags & ALLOC_MEM_FLAGS_GTT) {
		domain = alloc_domain = AMDGPU_GEM_DOMAIN_GTT;
		alloc_flags = 0;
	} else if (flags & ALLOC_MEM_FLAGS_USERPTR) {
		domain = AMDGPU_GEM_DOMAIN_GTT;
		alloc_domain = AMDGPU_GEM_DOMAIN_CPU;
		alloc_flags = 0;
		if (!offset || !*offset)
			return -EINVAL;
		user_addr = untagged_addr(*offset);
=======
	if (flags & KFD_IOC_ALLOC_MEM_FLAGS_VRAM) {
		domain = alloc_domain = AMDGPU_GEM_DOMAIN_VRAM;
		alloc_flags = AMDGPU_GEM_CREATE_VRAM_WIPE_ON_RELEASE;
		alloc_flags |= (flags & KFD_IOC_ALLOC_MEM_FLAGS_PUBLIC) ?
			AMDGPU_GEM_CREATE_CPU_ACCESS_REQUIRED : 0;
	} else if (flags & KFD_IOC_ALLOC_MEM_FLAGS_GTT) {
		domain = alloc_domain = AMDGPU_GEM_DOMAIN_GTT;
		alloc_flags = 0;
	} else if (flags & KFD_IOC_ALLOC_MEM_FLAGS_USERPTR) {
		domain = AMDGPU_GEM_DOMAIN_GTT;
		alloc_domain = AMDGPU_GEM_DOMAIN_CPU;
		alloc_flags = AMDGPU_GEM_CREATE_PREEMPTIBLE;
		if (!offset || !*offset)
			return -EINVAL;
		user_addr = untagged_addr(*offset);
	} else if (flags & (KFD_IOC_ALLOC_MEM_FLAGS_DOORBELL |
			KFD_IOC_ALLOC_MEM_FLAGS_MMIO_REMAP)) {
		domain = AMDGPU_GEM_DOMAIN_GTT;
		alloc_domain = AMDGPU_GEM_DOMAIN_CPU;
		bo_type = ttm_bo_type_sg;
		alloc_flags = 0;
		if (size > UINT_MAX)
			return -EINVAL;
		sg = create_doorbell_sg(*offset, size);
		if (!sg)
			return -ENOMEM;
>>>>>>> upstream/android-13
	} else {
		return -EINVAL;
	}

	*mem = kzalloc(sizeof(struct kgd_mem), GFP_KERNEL);
<<<<<<< HEAD
	if (!*mem)
		return -ENOMEM;
	INIT_LIST_HEAD(&(*mem)->bo_va_list);
	mutex_init(&(*mem)->lock);
	(*mem)->aql_queue = !!(flags & ALLOC_MEM_FLAGS_AQL_QUEUE_MEM);
=======
	if (!*mem) {
		ret = -ENOMEM;
		goto err;
	}
	INIT_LIST_HEAD(&(*mem)->attachments);
	mutex_init(&(*mem)->lock);
	(*mem)->aql_queue = !!(flags & KFD_IOC_ALLOC_MEM_FLAGS_AQL_QUEUE_MEM);
>>>>>>> upstream/android-13

	/* Workaround for AQL queue wraparound bug. Map the same
	 * memory twice. That means we only actually allocate half
	 * the memory.
	 */
	if ((*mem)->aql_queue)
		size = size >> 1;

<<<<<<< HEAD
	/* Workaround for TLB bug on older VI chips */
	byte_align = (adev->family == AMDGPU_FAMILY_VI &&
			adev->asic_type != CHIP_FIJI &&
			adev->asic_type != CHIP_POLARIS10 &&
			adev->asic_type != CHIP_POLARIS11) ?
			VI_BO_SIZE_ALIGN : 1;

	mapping_flags = AMDGPU_VM_PAGE_READABLE;
	if (flags & ALLOC_MEM_FLAGS_WRITABLE)
		mapping_flags |= AMDGPU_VM_PAGE_WRITEABLE;
	if (flags & ALLOC_MEM_FLAGS_EXECUTABLE)
		mapping_flags |= AMDGPU_VM_PAGE_EXECUTABLE;
	if (flags & ALLOC_MEM_FLAGS_COHERENT)
		mapping_flags |= AMDGPU_VM_MTYPE_UC;
	else
		mapping_flags |= AMDGPU_VM_MTYPE_NC;
	(*mem)->mapping_flags = mapping_flags;

	amdgpu_sync_create(&(*mem)->sync);

	ret = amdgpu_amdkfd_reserve_system_mem_limit(adev, size, alloc_domain);
	if (ret) {
		pr_debug("Insufficient system memory\n");
		goto err_reserve_system_mem;
=======
	(*mem)->alloc_flags = flags;

	amdgpu_sync_create(&(*mem)->sync);

	ret = amdgpu_amdkfd_reserve_mem_limit(adev, size, alloc_domain, !!sg);
	if (ret) {
		pr_debug("Insufficient memory\n");
		goto err_reserve_limit;
>>>>>>> upstream/android-13
	}

	pr_debug("\tcreate BO VA 0x%llx size 0x%llx domain %s\n",
			va, size, domain_string(alloc_domain));

<<<<<<< HEAD
	memset(&bp, 0, sizeof(bp));
	bp.size = size;
	bp.byte_align = byte_align;
	bp.domain = alloc_domain;
	bp.flags = alloc_flags;
	bp.type = ttm_bo_type_device;
	bp.resv = NULL;
	ret = amdgpu_bo_create(adev, &bp, &bo);
	if (ret) {
		pr_debug("Failed to create BO on domain %s. ret %d\n",
				domain_string(alloc_domain), ret);
		goto err_bo_create;
	}
	bo->kfd_bo = *mem;
	(*mem)->bo = bo;
	if (user_addr)
		bo->flags |= AMDGPU_AMDKFD_USERPTR_BO;
=======
	ret = amdgpu_gem_object_create(adev, size, 1, alloc_domain, alloc_flags,
				       bo_type, NULL, &gobj);
	if (ret) {
		pr_debug("Failed to create BO on domain %s. ret %d\n",
			 domain_string(alloc_domain), ret);
		goto err_bo_create;
	}
	ret = drm_vma_node_allow(&gobj->vma_node, drm_priv);
	if (ret) {
		pr_debug("Failed to allow vma node access. ret %d\n", ret);
		goto err_node_allow;
	}
	bo = gem_to_amdgpu_bo(gobj);
	if (bo_type == ttm_bo_type_sg) {
		bo->tbo.sg = sg;
		bo->tbo.ttm->sg = sg;
	}
	bo->kfd_bo = *mem;
	(*mem)->bo = bo;
	if (user_addr)
		bo->flags |= AMDGPU_AMDKFD_CREATE_USERPTR_BO;
>>>>>>> upstream/android-13

	(*mem)->va = va;
	(*mem)->domain = domain;
	(*mem)->mapped_to_gpu_memory = 0;
	(*mem)->process_info = avm->process_info;
	add_kgd_mem_to_kfd_bo_list(*mem, avm->process_info, user_addr);

	if (user_addr) {
<<<<<<< HEAD
		ret = init_user_pages(*mem, current->mm, user_addr);
		if (ret) {
			mutex_lock(&avm->process_info->lock);
			list_del(&(*mem)->validate_list.head);
			mutex_unlock(&avm->process_info->lock);
			goto allocate_init_user_pages_failed;
		}
=======
		ret = init_user_pages(*mem, user_addr);
		if (ret)
			goto allocate_init_user_pages_failed;
>>>>>>> upstream/android-13
	}

	if (offset)
		*offset = amdgpu_bo_mmap_offset(bo);

	return 0;

allocate_init_user_pages_failed:
<<<<<<< HEAD
	amdgpu_bo_unref(&bo);
	/* Don't unreserve system mem limit twice */
	goto err_reserve_system_mem;
err_bo_create:
	unreserve_system_mem_limit(adev, size, alloc_domain);
err_reserve_system_mem:
	mutex_destroy(&(*mem)->lock);
	kfree(*mem);
=======
	remove_kgd_mem_from_kfd_bo_list(*mem, avm->process_info);
	drm_vma_node_revoke(&gobj->vma_node, drm_priv);
err_node_allow:
	/* Don't unreserve system mem limit twice */
	goto err_reserve_limit;
err_bo_create:
	unreserve_mem_limit(adev, size, alloc_domain, !!sg);
err_reserve_limit:
	mutex_destroy(&(*mem)->lock);
	if (gobj)
		drm_gem_object_put(gobj);
	else
		kfree(*mem);
err:
	if (sg) {
		sg_free_table(sg);
		kfree(sg);
	}
>>>>>>> upstream/android-13
	return ret;
}

int amdgpu_amdkfd_gpuvm_free_memory_of_gpu(
<<<<<<< HEAD
		struct kgd_dev *kgd, struct kgd_mem *mem)
{
	struct amdkfd_process_info *process_info = mem->process_info;
	unsigned long bo_size = mem->bo->tbo.mem.size;
	struct kfd_bo_va_list *entry, *tmp;
	struct bo_vm_reservation_context ctx;
	struct ttm_validate_buffer *bo_list_entry;
	int ret;

	mutex_lock(&mem->lock);

	if (mem->mapped_to_gpu_memory > 0) {
		pr_debug("BO VA 0x%llx size 0x%lx is still mapped.\n",
				mem->va, bo_size);
		mutex_unlock(&mem->lock);
		return -EBUSY;
	}

=======
		struct kgd_dev *kgd, struct kgd_mem *mem, void *drm_priv,
		uint64_t *size)
{
	struct amdkfd_process_info *process_info = mem->process_info;
	unsigned long bo_size = mem->bo->tbo.base.size;
	struct kfd_mem_attachment *entry, *tmp;
	struct bo_vm_reservation_context ctx;
	struct ttm_validate_buffer *bo_list_entry;
	unsigned int mapped_to_gpu_memory;
	int ret;
	bool is_imported = false;

	mutex_lock(&mem->lock);
	mapped_to_gpu_memory = mem->mapped_to_gpu_memory;
	is_imported = mem->is_imported;
>>>>>>> upstream/android-13
	mutex_unlock(&mem->lock);
	/* lock is not needed after this, since mem is unused and will
	 * be freed anyway
	 */

<<<<<<< HEAD
	/* No more MMU notifiers */
	amdgpu_mn_unregister(mem->bo);
=======
	if (mapped_to_gpu_memory > 0) {
		pr_debug("BO VA 0x%llx size 0x%lx is still mapped.\n",
				mem->va, bo_size);
		return -EBUSY;
	}
>>>>>>> upstream/android-13

	/* Make sure restore workers don't access the BO any more */
	bo_list_entry = &mem->validate_list;
	mutex_lock(&process_info->lock);
	list_del(&bo_list_entry->head);
	mutex_unlock(&process_info->lock);

<<<<<<< HEAD
	/* Free user pages if necessary */
	if (mem->user_pages) {
		pr_debug("%s: Freeing user_pages array\n", __func__);
		if (mem->user_pages[0])
			release_pages(mem->user_pages,
					mem->bo->tbo.ttm->num_pages);
		kvfree(mem->user_pages);
	}
=======
	/* No more MMU notifiers */
	amdgpu_mn_unregister(mem->bo);
>>>>>>> upstream/android-13

	ret = reserve_bo_and_cond_vms(mem, NULL, BO_VM_ALL, &ctx);
	if (unlikely(ret))
		return ret;

	/* The eviction fence should be removed by the last unmap.
	 * TODO: Log an error condition if the bo still has the eviction fence
	 * attached
	 */
	amdgpu_amdkfd_remove_eviction_fence(mem->bo,
<<<<<<< HEAD
					process_info->eviction_fence,
					NULL, NULL);
	pr_debug("Release VA 0x%llx - 0x%llx\n", mem->va,
		mem->va + bo_size * (1 + mem->aql_queue));

	/* Remove from VM internal data structures */
	list_for_each_entry_safe(entry, tmp, &mem->bo_va_list, bo_list)
		remove_bo_from_vm((struct amdgpu_device *)entry->kgd_dev,
				entry, bo_size);

	ret = unreserve_bo_and_vms(&ctx, false, false);

	/* Free the sync object */
	amdgpu_sync_free(&mem->sync);

	/* Free the BO*/
	amdgpu_bo_unref(&mem->bo);
=======
					process_info->eviction_fence);
	pr_debug("Release VA 0x%llx - 0x%llx\n", mem->va,
		mem->va + bo_size * (1 + mem->aql_queue));

	ret = unreserve_bo_and_vms(&ctx, false, false);

	/* Remove from VM internal data structures */
	list_for_each_entry_safe(entry, tmp, &mem->attachments, list)
		kfd_mem_detach(entry);

	/* Free the sync object */
	amdgpu_sync_free(&mem->sync);

	/* If the SG is not NULL, it's one we created for a doorbell or mmio
	 * remap BO. We need to free it.
	 */
	if (mem->bo->tbo.sg) {
		sg_free_table(mem->bo->tbo.sg);
		kfree(mem->bo->tbo.sg);
	}

	/* Update the size of the BO being freed if it was allocated from
	 * VRAM and is not imported.
	 */
	if (size) {
		if ((mem->bo->preferred_domains == AMDGPU_GEM_DOMAIN_VRAM) &&
		    (!is_imported))
			*size = bo_size;
		else
			*size = 0;
	}

	/* Free the BO*/
	drm_vma_node_revoke(&mem->bo->tbo.base.vma_node, drm_priv);
	if (mem->dmabuf)
		dma_buf_put(mem->dmabuf);
	drm_gem_object_put(&mem->bo->tbo.base);
>>>>>>> upstream/android-13
	mutex_destroy(&mem->lock);
	kfree(mem);

	return ret;
}

int amdgpu_amdkfd_gpuvm_map_memory_to_gpu(
<<<<<<< HEAD
		struct kgd_dev *kgd, struct kgd_mem *mem, void *vm)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct amdgpu_vm *avm = (struct amdgpu_vm *)vm;
	int ret;
	struct amdgpu_bo *bo;
	uint32_t domain;
	struct kfd_bo_va_list *entry;
	struct bo_vm_reservation_context ctx;
	struct kfd_bo_va_list *bo_va_entry = NULL;
	struct kfd_bo_va_list *bo_va_entry_aql = NULL;
=======
		struct kgd_dev *kgd, struct kgd_mem *mem,
		void *drm_priv, bool *table_freed)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct amdgpu_vm *avm = drm_priv_to_vm(drm_priv);
	int ret;
	struct amdgpu_bo *bo;
	uint32_t domain;
	struct kfd_mem_attachment *entry;
	struct bo_vm_reservation_context ctx;
>>>>>>> upstream/android-13
	unsigned long bo_size;
	bool is_invalid_userptr = false;

	bo = mem->bo;
	if (!bo) {
		pr_err("Invalid BO when mapping memory to GPU\n");
		return -EINVAL;
	}

	/* Make sure restore is not running concurrently. Since we
	 * don't map invalid userptr BOs, we rely on the next restore
	 * worker to do the mapping
	 */
	mutex_lock(&mem->process_info->lock);

	/* Lock mmap-sem. If we find an invalid userptr BO, we can be
	 * sure that the MMU notifier is no longer running
	 * concurrently and the queues are actually stopped
	 */
	if (amdgpu_ttm_tt_get_usermm(bo->tbo.ttm)) {
<<<<<<< HEAD
		down_write(&current->mm->mmap_sem);
		is_invalid_userptr = atomic_read(&mem->invalid);
		up_write(&current->mm->mmap_sem);
=======
		mmap_write_lock(current->mm);
		is_invalid_userptr = atomic_read(&mem->invalid);
		mmap_write_unlock(current->mm);
>>>>>>> upstream/android-13
	}

	mutex_lock(&mem->lock);

	domain = mem->domain;
<<<<<<< HEAD
	bo_size = bo->tbo.mem.size;
=======
	bo_size = bo->tbo.base.size;
>>>>>>> upstream/android-13

	pr_debug("Map VA 0x%llx - 0x%llx to vm %p domain %s\n",
			mem->va,
			mem->va + bo_size * (1 + mem->aql_queue),
<<<<<<< HEAD
			vm, domain_string(domain));

	ret = reserve_bo_and_vm(mem, vm, &ctx);
=======
			avm, domain_string(domain));

	if (!kfd_mem_is_attached(avm, mem)) {
		ret = kfd_mem_attach(adev, mem, avm, mem->aql_queue);
		if (ret)
			goto out;
	}

	ret = reserve_bo_and_vm(mem, avm, &ctx);
>>>>>>> upstream/android-13
	if (unlikely(ret))
		goto out;

	/* Userptr can be marked as "not invalid", but not actually be
	 * validated yet (still in the system domain). In that case
	 * the queues are still stopped and we can leave mapping for
	 * the next restore worker
	 */
<<<<<<< HEAD
	if (bo->tbo.mem.mem_type == TTM_PL_SYSTEM)
		is_invalid_userptr = true;

	if (check_if_add_bo_to_vm(avm, mem)) {
		ret = add_bo_to_vm(adev, mem, avm, false,
				&bo_va_entry);
		if (ret)
			goto add_bo_to_vm_failed;
		if (mem->aql_queue) {
			ret = add_bo_to_vm(adev, mem, avm,
					true, &bo_va_entry_aql);
			if (ret)
				goto add_bo_to_vm_failed_aql;
		}
	} else {
		ret = vm_validate_pt_pd_bos(avm);
		if (unlikely(ret))
			goto add_bo_to_vm_failed;
	}
=======
	if (amdgpu_ttm_tt_get_usermm(bo->tbo.ttm) &&
	    bo->tbo.resource->mem_type == TTM_PL_SYSTEM)
		is_invalid_userptr = true;

	ret = vm_validate_pt_pd_bos(avm);
	if (unlikely(ret))
		goto out_unreserve;
>>>>>>> upstream/android-13

	if (mem->mapped_to_gpu_memory == 0 &&
	    !amdgpu_ttm_tt_get_usermm(bo->tbo.ttm)) {
		/* Validate BO only once. The eviction fence gets added to BO
		 * the first time it is mapped. Validate will wait for all
		 * background evictions to complete.
		 */
		ret = amdgpu_amdkfd_bo_validate(bo, domain, true);
		if (ret) {
			pr_debug("Validate failed\n");
<<<<<<< HEAD
			goto map_bo_to_gpuvm_failed;
		}
	}

	list_for_each_entry(entry, &mem->bo_va_list, bo_list) {
		if (entry->bo_va->base.vm == vm && !entry->is_mapped) {
			pr_debug("\t map VA 0x%llx - 0x%llx in entry %p\n",
					entry->va, entry->va + bo_size,
					entry);

			ret = map_bo_to_gpuvm(adev, entry, ctx.sync,
					      is_invalid_userptr);
			if (ret) {
				pr_err("Failed to map radeon bo to gpuvm\n");
				goto map_bo_to_gpuvm_failed;
			}

			ret = vm_update_pds(vm, ctx.sync);
			if (ret) {
				pr_err("Failed to update page directories\n");
				goto map_bo_to_gpuvm_failed;
			}

			entry->is_mapped = true;
			mem->mapped_to_gpu_memory++;
			pr_debug("\t INC mapping count %d\n",
					mem->mapped_to_gpu_memory);
		}
	}

	if (!amdgpu_ttm_tt_get_usermm(bo->tbo.ttm) && !bo->pin_count)
=======
			goto out_unreserve;
		}
	}

	list_for_each_entry(entry, &mem->attachments, list) {
		if (entry->bo_va->base.vm != avm || entry->is_mapped)
			continue;

		pr_debug("\t map VA 0x%llx - 0x%llx in entry %p\n",
			 entry->va, entry->va + bo_size, entry);

		ret = map_bo_to_gpuvm(mem, entry, ctx.sync,
				      is_invalid_userptr, table_freed);
		if (ret) {
			pr_err("Failed to map bo to gpuvm\n");
			goto out_unreserve;
		}

		ret = vm_update_pds(avm, ctx.sync);
		if (ret) {
			pr_err("Failed to update page directories\n");
			goto out_unreserve;
		}

		entry->is_mapped = true;
		mem->mapped_to_gpu_memory++;
		pr_debug("\t INC mapping count %d\n",
			 mem->mapped_to_gpu_memory);
	}

	if (!amdgpu_ttm_tt_get_usermm(bo->tbo.ttm) && !bo->tbo.pin_count)
>>>>>>> upstream/android-13
		amdgpu_bo_fence(bo,
				&avm->process_info->eviction_fence->base,
				true);
	ret = unreserve_bo_and_vms(&ctx, false, false);

<<<<<<< HEAD
	goto out;

map_bo_to_gpuvm_failed:
	if (bo_va_entry_aql)
		remove_bo_from_vm(adev, bo_va_entry_aql, bo_size);
add_bo_to_vm_failed_aql:
	if (bo_va_entry)
		remove_bo_from_vm(adev, bo_va_entry, bo_size);
add_bo_to_vm_failed:
=======
	/* Only apply no TLB flush on Aldebaran to
	 * workaround regressions on other Asics.
	 */
	if (table_freed && (adev->asic_type != CHIP_ALDEBARAN))
		*table_freed = true;

	goto out;

out_unreserve:
>>>>>>> upstream/android-13
	unreserve_bo_and_vms(&ctx, false, false);
out:
	mutex_unlock(&mem->process_info->lock);
	mutex_unlock(&mem->lock);
	return ret;
}

int amdgpu_amdkfd_gpuvm_unmap_memory_from_gpu(
<<<<<<< HEAD
		struct kgd_dev *kgd, struct kgd_mem *mem, void *vm)
{
	struct amdgpu_device *adev = get_amdgpu_device(kgd);
	struct amdkfd_process_info *process_info =
		((struct amdgpu_vm *)vm)->process_info;
	unsigned long bo_size = mem->bo->tbo.mem.size;
	struct kfd_bo_va_list *entry;
=======
		struct kgd_dev *kgd, struct kgd_mem *mem, void *drm_priv)
{
	struct amdgpu_vm *avm = drm_priv_to_vm(drm_priv);
	struct amdkfd_process_info *process_info = avm->process_info;
	unsigned long bo_size = mem->bo->tbo.base.size;
	struct kfd_mem_attachment *entry;
>>>>>>> upstream/android-13
	struct bo_vm_reservation_context ctx;
	int ret;

	mutex_lock(&mem->lock);

<<<<<<< HEAD
	ret = reserve_bo_and_cond_vms(mem, vm, BO_VM_MAPPED, &ctx);
=======
	ret = reserve_bo_and_cond_vms(mem, avm, BO_VM_MAPPED, &ctx);
>>>>>>> upstream/android-13
	if (unlikely(ret))
		goto out;
	/* If no VMs were reserved, it means the BO wasn't actually mapped */
	if (ctx.n_vms == 0) {
		ret = -EINVAL;
		goto unreserve_out;
	}

<<<<<<< HEAD
	ret = vm_validate_pt_pd_bos((struct amdgpu_vm *)vm);
=======
	ret = vm_validate_pt_pd_bos(avm);
>>>>>>> upstream/android-13
	if (unlikely(ret))
		goto unreserve_out;

	pr_debug("Unmap VA 0x%llx - 0x%llx from vm %p\n",
		mem->va,
		mem->va + bo_size * (1 + mem->aql_queue),
<<<<<<< HEAD
		vm);

	list_for_each_entry(entry, &mem->bo_va_list, bo_list) {
		if (entry->bo_va->base.vm == vm && entry->is_mapped) {
			pr_debug("\t unmap VA 0x%llx - 0x%llx from entry %p\n",
					entry->va,
					entry->va + bo_size,
					entry);

			ret = unmap_bo_from_gpuvm(adev, entry, ctx.sync);
			if (ret == 0) {
				entry->is_mapped = false;
			} else {
				pr_err("failed to unmap VA 0x%llx\n",
						mem->va);
				goto unreserve_out;
			}

			mem->mapped_to_gpu_memory--;
			pr_debug("\t DEC mapping count %d\n",
					mem->mapped_to_gpu_memory);
		}
=======
		avm);

	list_for_each_entry(entry, &mem->attachments, list) {
		if (entry->bo_va->base.vm != avm || !entry->is_mapped)
			continue;

		pr_debug("\t unmap VA 0x%llx - 0x%llx from entry %p\n",
			 entry->va, entry->va + bo_size, entry);

		unmap_bo_from_gpuvm(mem, entry, ctx.sync);
		entry->is_mapped = false;

		mem->mapped_to_gpu_memory--;
		pr_debug("\t DEC mapping count %d\n",
			 mem->mapped_to_gpu_memory);
>>>>>>> upstream/android-13
	}

	/* If BO is unmapped from all VMs, unfence it. It can be evicted if
	 * required.
	 */
	if (mem->mapped_to_gpu_memory == 0 &&
<<<<<<< HEAD
	    !amdgpu_ttm_tt_get_usermm(mem->bo->tbo.ttm) && !mem->bo->pin_count)
		amdgpu_amdkfd_remove_eviction_fence(mem->bo,
						process_info->eviction_fence,
						    NULL, NULL);
=======
	    !amdgpu_ttm_tt_get_usermm(mem->bo->tbo.ttm) &&
	    !mem->bo->tbo.pin_count)
		amdgpu_amdkfd_remove_eviction_fence(mem->bo,
						process_info->eviction_fence);
>>>>>>> upstream/android-13

unreserve_out:
	unreserve_bo_and_vms(&ctx, false, false);
out:
	mutex_unlock(&mem->lock);
	return ret;
}

int amdgpu_amdkfd_gpuvm_sync_memory(
		struct kgd_dev *kgd, struct kgd_mem *mem, bool intr)
{
	struct amdgpu_sync sync;
	int ret;

	amdgpu_sync_create(&sync);

	mutex_lock(&mem->lock);
	amdgpu_sync_clone(&mem->sync, &sync);
	mutex_unlock(&mem->lock);

	ret = amdgpu_sync_wait(&sync, intr);
	amdgpu_sync_free(&sync);
	return ret;
}

int amdgpu_amdkfd_gpuvm_map_gtt_bo_to_kernel(struct kgd_dev *kgd,
		struct kgd_mem *mem, void **kptr, uint64_t *size)
{
	int ret;
	struct amdgpu_bo *bo = mem->bo;

	if (amdgpu_ttm_tt_get_usermm(bo->tbo.ttm)) {
		pr_err("userptr can't be mapped to kernel\n");
		return -EINVAL;
	}

	/* delete kgd_mem from kfd_bo_list to avoid re-validating
	 * this BO in BO's restoring after eviction.
	 */
	mutex_lock(&mem->process_info->lock);

	ret = amdgpu_bo_reserve(bo, true);
	if (ret) {
		pr_err("Failed to reserve bo. ret %d\n", ret);
		goto bo_reserve_failed;
	}

	ret = amdgpu_bo_pin(bo, AMDGPU_GEM_DOMAIN_GTT);
	if (ret) {
		pr_err("Failed to pin bo. ret %d\n", ret);
		goto pin_failed;
	}

	ret = amdgpu_bo_kmap(bo, kptr);
	if (ret) {
		pr_err("Failed to map bo to kernel. ret %d\n", ret);
		goto kmap_failed;
	}

	amdgpu_amdkfd_remove_eviction_fence(
<<<<<<< HEAD
		bo, mem->process_info->eviction_fence, NULL, NULL);
=======
		bo, mem->process_info->eviction_fence);
>>>>>>> upstream/android-13
	list_del_init(&mem->validate_list.head);

	if (size)
		*size = amdgpu_bo_size(bo);

	amdgpu_bo_unreserve(bo);

	mutex_unlock(&mem->process_info->lock);
	return 0;

kmap_failed:
	amdgpu_bo_unpin(bo);
pin_failed:
	amdgpu_bo_unreserve(bo);
bo_reserve_failed:
	mutex_unlock(&mem->process_info->lock);

	return ret;
}

int amdgpu_amdkfd_gpuvm_get_vm_fault_info(struct kgd_dev *kgd,
					      struct kfd_vm_fault_info *mem)
{
	struct amdgpu_device *adev;

	adev = (struct amdgpu_device *)kgd;
	if (atomic_read(&adev->gmc.vm_fault_info_updated) == 1) {
		*mem = *adev->gmc.vm_fault_info;
		mb();
		atomic_set(&adev->gmc.vm_fault_info_updated, 0);
	}
	return 0;
}

<<<<<<< HEAD
=======
int amdgpu_amdkfd_gpuvm_import_dmabuf(struct kgd_dev *kgd,
				      struct dma_buf *dma_buf,
				      uint64_t va, void *drm_priv,
				      struct kgd_mem **mem, uint64_t *size,
				      uint64_t *mmap_offset)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;
	struct amdgpu_vm *avm = drm_priv_to_vm(drm_priv);
	struct drm_gem_object *obj;
	struct amdgpu_bo *bo;
	int ret;

	if (dma_buf->ops != &amdgpu_dmabuf_ops)
		/* Can't handle non-graphics buffers */
		return -EINVAL;

	obj = dma_buf->priv;
	if (drm_to_adev(obj->dev) != adev)
		/* Can't handle buffers from other devices */
		return -EINVAL;

	bo = gem_to_amdgpu_bo(obj);
	if (!(bo->preferred_domains & (AMDGPU_GEM_DOMAIN_VRAM |
				    AMDGPU_GEM_DOMAIN_GTT)))
		/* Only VRAM and GTT BOs are supported */
		return -EINVAL;

	*mem = kzalloc(sizeof(struct kgd_mem), GFP_KERNEL);
	if (!*mem)
		return -ENOMEM;

	ret = drm_vma_node_allow(&obj->vma_node, drm_priv);
	if (ret) {
		kfree(mem);
		return ret;
	}

	if (size)
		*size = amdgpu_bo_size(bo);

	if (mmap_offset)
		*mmap_offset = amdgpu_bo_mmap_offset(bo);

	INIT_LIST_HEAD(&(*mem)->attachments);
	mutex_init(&(*mem)->lock);

	(*mem)->alloc_flags =
		((bo->preferred_domains & AMDGPU_GEM_DOMAIN_VRAM) ?
		KFD_IOC_ALLOC_MEM_FLAGS_VRAM : KFD_IOC_ALLOC_MEM_FLAGS_GTT)
		| KFD_IOC_ALLOC_MEM_FLAGS_WRITABLE
		| KFD_IOC_ALLOC_MEM_FLAGS_EXECUTABLE;

	drm_gem_object_get(&bo->tbo.base);
	(*mem)->bo = bo;
	(*mem)->va = va;
	(*mem)->domain = (bo->preferred_domains & AMDGPU_GEM_DOMAIN_VRAM) ?
		AMDGPU_GEM_DOMAIN_VRAM : AMDGPU_GEM_DOMAIN_GTT;
	(*mem)->mapped_to_gpu_memory = 0;
	(*mem)->process_info = avm->process_info;
	add_kgd_mem_to_kfd_bo_list(*mem, avm->process_info, false);
	amdgpu_sync_create(&(*mem)->sync);
	(*mem)->is_imported = true;

	return 0;
}

>>>>>>> upstream/android-13
/* Evict a userptr BO by stopping the queues if necessary
 *
 * Runs in MMU notifier, may be in RECLAIM_FS context. This means it
 * cannot do any memory allocations, and cannot take any locks that
 * are held elsewhere while allocating memory. Therefore this is as
 * simple as possible, using atomic counters.
 *
 * It doesn't do anything to the BO itself. The real work happens in
 * restore, where we get updated page addresses. This function only
 * ensures that GPU access to the BO is stopped.
 */
int amdgpu_amdkfd_evict_userptr(struct kgd_mem *mem,
				struct mm_struct *mm)
{
	struct amdkfd_process_info *process_info = mem->process_info;
<<<<<<< HEAD
	int invalid, evicted_bos;
	int r = 0;

	invalid = atomic_inc_return(&mem->invalid);
	evicted_bos = atomic_inc_return(&process_info->evicted_bos);
	if (evicted_bos == 1) {
		/* First eviction, stop the queues */
		r = kgd2kfd->quiesce_mm(mm);
=======
	int evicted_bos;
	int r = 0;

	atomic_inc(&mem->invalid);
	evicted_bos = atomic_inc_return(&process_info->evicted_bos);
	if (evicted_bos == 1) {
		/* First eviction, stop the queues */
		r = kgd2kfd_quiesce_mm(mm);
>>>>>>> upstream/android-13
		if (r)
			pr_err("Failed to quiesce KFD\n");
		schedule_delayed_work(&process_info->restore_userptr_work,
			msecs_to_jiffies(AMDGPU_USERPTR_RESTORE_DELAY_MS));
	}

	return r;
}

/* Update invalid userptr BOs
 *
 * Moves invalidated (evicted) userptr BOs from userptr_valid_list to
 * userptr_inval_list and updates user pages for all BOs that have
 * been invalidated since their last update.
 */
static int update_invalid_user_pages(struct amdkfd_process_info *process_info,
				     struct mm_struct *mm)
{
	struct kgd_mem *mem, *tmp_mem;
	struct amdgpu_bo *bo;
	struct ttm_operation_ctx ctx = { false, false };
	int invalid, ret;

	/* Move all invalidated BOs to the userptr_inval_list and
	 * release their user pages by migration to the CPU domain
	 */
	list_for_each_entry_safe(mem, tmp_mem,
				 &process_info->userptr_valid_list,
				 validate_list.head) {
		if (!atomic_read(&mem->invalid))
			continue; /* BO is still valid */

		bo = mem->bo;

		if (amdgpu_bo_reserve(bo, true))
			return -EAGAIN;
		amdgpu_bo_placement_from_domain(bo, AMDGPU_GEM_DOMAIN_CPU);
		ret = ttm_bo_validate(&bo->tbo, &bo->placement, &ctx);
		amdgpu_bo_unreserve(bo);
		if (ret) {
			pr_err("%s: Failed to invalidate userptr BO\n",
			       __func__);
			return -EAGAIN;
		}

		list_move_tail(&mem->validate_list.head,
			       &process_info->userptr_inval_list);
	}

	if (list_empty(&process_info->userptr_inval_list))
		return 0; /* All evicted userptr BOs were freed */

	/* Go through userptr_inval_list and update any invalid user_pages */
	list_for_each_entry(mem, &process_info->userptr_inval_list,
			    validate_list.head) {
		invalid = atomic_read(&mem->invalid);
		if (!invalid)
			/* BO hasn't been invalidated since the last
			 * revalidation attempt. Keep its BO list.
			 */
			continue;

		bo = mem->bo;

<<<<<<< HEAD
		if (!mem->user_pages) {
			mem->user_pages =
				kvmalloc_array(bo->tbo.ttm->num_pages,
						 sizeof(struct page *),
						 GFP_KERNEL | __GFP_ZERO);
			if (!mem->user_pages) {
				pr_err("%s: Failed to allocate pages array\n",
				       __func__);
				return -ENOMEM;
			}
		} else if (mem->user_pages[0]) {
			release_pages(mem->user_pages, bo->tbo.ttm->num_pages);
		}

		/* Get updated user pages */
		ret = amdgpu_ttm_tt_get_user_pages(bo->tbo.ttm,
						   mem->user_pages);
		if (ret) {
			mem->user_pages[0] = NULL;
			pr_info("%s: Failed to get user pages: %d\n",
				__func__, ret);
			/* Pretend it succeeded. It will fail later
			 * with a VM fault if the GPU tries to access
			 * it. Better than hanging indefinitely with
			 * stalled user mode queues.
			 */
		}

		/* Mark the BO as valid unless it was invalidated
		 * again concurrently
=======
		/* Get updated user pages */
		ret = amdgpu_ttm_tt_get_user_pages(bo, bo->tbo.ttm->pages);
		if (ret) {
			pr_debug("%s: Failed to get user pages: %d\n",
				__func__, ret);

			/* Return error -EBUSY or -ENOMEM, retry restore */
			return ret;
		}

		/*
		 * FIXME: Cannot ignore the return code, must hold
		 * notifier_lock
		 */
		amdgpu_ttm_tt_get_user_pages_done(bo->tbo.ttm);

		/* Mark the BO as valid unless it was invalidated
		 * again concurrently.
>>>>>>> upstream/android-13
		 */
		if (atomic_cmpxchg(&mem->invalid, invalid, 0) != invalid)
			return -EAGAIN;
	}

	return 0;
}

/* Validate invalid userptr BOs
 *
 * Validates BOs on the userptr_inval_list, and moves them back to the
 * userptr_valid_list. Also updates GPUVM page tables with new page
 * addresses and waits for the page table updates to complete.
 */
static int validate_invalid_user_pages(struct amdkfd_process_info *process_info)
{
	struct amdgpu_bo_list_entry *pd_bo_list_entries;
	struct list_head resv_list, duplicates;
	struct ww_acquire_ctx ticket;
	struct amdgpu_sync sync;

	struct amdgpu_vm *peer_vm;
	struct kgd_mem *mem, *tmp_mem;
	struct amdgpu_bo *bo;
	struct ttm_operation_ctx ctx = { false, false };
	int i, ret;

	pd_bo_list_entries = kcalloc(process_info->n_vms,
				     sizeof(struct amdgpu_bo_list_entry),
				     GFP_KERNEL);
	if (!pd_bo_list_entries) {
		pr_err("%s: Failed to allocate PD BO list entries\n", __func__);
<<<<<<< HEAD
		return -ENOMEM;
=======
		ret = -ENOMEM;
		goto out_no_mem;
>>>>>>> upstream/android-13
	}

	INIT_LIST_HEAD(&resv_list);
	INIT_LIST_HEAD(&duplicates);

	/* Get all the page directory BOs that need to be reserved */
	i = 0;
	list_for_each_entry(peer_vm, &process_info->vm_list_head,
			    vm_list_node)
		amdgpu_vm_get_pd_bo(peer_vm, &resv_list,
				    &pd_bo_list_entries[i++]);
	/* Add the userptr_inval_list entries to resv_list */
	list_for_each_entry(mem, &process_info->userptr_inval_list,
			    validate_list.head) {
		list_add_tail(&mem->resv_list.head, &resv_list);
		mem->resv_list.bo = mem->validate_list.bo;
<<<<<<< HEAD
		mem->resv_list.shared = mem->validate_list.shared;
=======
		mem->resv_list.num_shared = mem->validate_list.num_shared;
>>>>>>> upstream/android-13
	}

	/* Reserve all BOs and page tables for validation */
	ret = ttm_eu_reserve_buffers(&ticket, &resv_list, false, &duplicates);
	WARN(!list_empty(&duplicates), "Duplicates should be empty");
	if (ret)
<<<<<<< HEAD
		goto out;

	amdgpu_sync_create(&sync);

	/* Avoid triggering eviction fences when unmapping invalid
	 * userptr BOs (waits for all fences, doesn't use
	 * FENCE_OWNER_VM)
	 */
	list_for_each_entry(peer_vm, &process_info->vm_list_head,
			    vm_list_node)
		amdgpu_amdkfd_remove_eviction_fence(peer_vm->root.base.bo,
						process_info->eviction_fence,
						NULL, NULL);

=======
		goto out_free;

	amdgpu_sync_create(&sync);

>>>>>>> upstream/android-13
	ret = process_validate_vms(process_info);
	if (ret)
		goto unreserve_out;

	/* Validate BOs and update GPUVM page tables */
	list_for_each_entry_safe(mem, tmp_mem,
				 &process_info->userptr_inval_list,
				 validate_list.head) {
<<<<<<< HEAD
		struct kfd_bo_va_list *bo_va_entry;

		bo = mem->bo;

		/* Copy pages array and validate the BO if we got user pages */
		if (mem->user_pages[0]) {
			amdgpu_ttm_tt_set_user_pages(bo->tbo.ttm,
						     mem->user_pages);
=======
		struct kfd_mem_attachment *attachment;

		bo = mem->bo;

		/* Validate the BO if we got user pages */
		if (bo->tbo.ttm->pages[0]) {
>>>>>>> upstream/android-13
			amdgpu_bo_placement_from_domain(bo, mem->domain);
			ret = ttm_bo_validate(&bo->tbo, &bo->placement, &ctx);
			if (ret) {
				pr_err("%s: failed to validate BO\n", __func__);
				goto unreserve_out;
			}
		}

<<<<<<< HEAD
		/* Validate succeeded, now the BO owns the pages, free
		 * our copy of the pointer array. Put this BO back on
		 * the userptr_valid_list. If we need to revalidate
		 * it, we need to start from scratch.
		 */
		kvfree(mem->user_pages);
		mem->user_pages = NULL;
=======
>>>>>>> upstream/android-13
		list_move_tail(&mem->validate_list.head,
			       &process_info->userptr_valid_list);

		/* Update mapping. If the BO was not validated
		 * (because we couldn't get user pages), this will
		 * clear the page table entries, which will result in
		 * VM faults if the GPU tries to access the invalid
		 * memory.
		 */
<<<<<<< HEAD
		list_for_each_entry(bo_va_entry, &mem->bo_va_list, bo_list) {
			if (!bo_va_entry->is_mapped)
				continue;

			ret = update_gpuvm_pte((struct amdgpu_device *)
					       bo_va_entry->kgd_dev,
					       bo_va_entry, &sync);
=======
		list_for_each_entry(attachment, &mem->attachments, list) {
			if (!attachment->is_mapped)
				continue;

			kfd_mem_dmaunmap_attachment(mem, attachment);
			ret = update_gpuvm_pte(mem, attachment, &sync, NULL);
>>>>>>> upstream/android-13
			if (ret) {
				pr_err("%s: update PTE failed\n", __func__);
				/* make sure this gets validated again */
				atomic_inc(&mem->invalid);
				goto unreserve_out;
			}
		}
	}

	/* Update page directories */
	ret = process_update_pds(process_info, &sync);

unreserve_out:
<<<<<<< HEAD
	list_for_each_entry(peer_vm, &process_info->vm_list_head,
			    vm_list_node)
		amdgpu_bo_fence(peer_vm->root.base.bo,
				&process_info->eviction_fence->base, true);
	ttm_eu_backoff_reservation(&ticket, &resv_list);
	amdgpu_sync_wait(&sync, false);
	amdgpu_sync_free(&sync);
out:
	kfree(pd_bo_list_entries);
=======
	ttm_eu_backoff_reservation(&ticket, &resv_list);
	amdgpu_sync_wait(&sync, false);
	amdgpu_sync_free(&sync);
out_free:
	kfree(pd_bo_list_entries);
out_no_mem:
>>>>>>> upstream/android-13

	return ret;
}

/* Worker callback to restore evicted userptr BOs
 *
 * Tries to update and validate all userptr BOs. If successful and no
 * concurrent evictions happened, the queues are restarted. Otherwise,
 * reschedule for another attempt later.
 */
static void amdgpu_amdkfd_restore_userptr_worker(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct amdkfd_process_info *process_info =
		container_of(dwork, struct amdkfd_process_info,
			     restore_userptr_work);
	struct task_struct *usertask;
	struct mm_struct *mm;
	int evicted_bos;

	evicted_bos = atomic_read(&process_info->evicted_bos);
	if (!evicted_bos)
		return;

	/* Reference task and mm in case of concurrent process termination */
	usertask = get_pid_task(process_info->pid, PIDTYPE_PID);
	if (!usertask)
		return;
	mm = get_task_mm(usertask);
	if (!mm) {
		put_task_struct(usertask);
		return;
	}

	mutex_lock(&process_info->lock);

	if (update_invalid_user_pages(process_info, mm))
		goto unlock_out;
	/* userptr_inval_list can be empty if all evicted userptr BOs
	 * have been freed. In that case there is nothing to validate
	 * and we can just restart the queues.
	 */
	if (!list_empty(&process_info->userptr_inval_list)) {
		if (atomic_read(&process_info->evicted_bos) != evicted_bos)
			goto unlock_out; /* Concurrent eviction, try again */

		if (validate_invalid_user_pages(process_info))
			goto unlock_out;
	}
	/* Final check for concurrent evicton and atomic update. If
	 * another eviction happens after successful update, it will
	 * be a first eviction that calls quiesce_mm. The eviction
	 * reference counting inside KFD will handle this case.
	 */
	if (atomic_cmpxchg(&process_info->evicted_bos, evicted_bos, 0) !=
	    evicted_bos)
		goto unlock_out;
	evicted_bos = 0;
<<<<<<< HEAD
	if (kgd2kfd->resume_mm(mm)) {
=======
	if (kgd2kfd_resume_mm(mm)) {
>>>>>>> upstream/android-13
		pr_err("%s: Failed to resume KFD\n", __func__);
		/* No recovery from this failure. Probably the CP is
		 * hanging. No point trying again.
		 */
	}
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
unlock_out:
	mutex_unlock(&process_info->lock);
	mmput(mm);
	put_task_struct(usertask);

	/* If validation failed, reschedule another attempt */
	if (evicted_bos)
		schedule_delayed_work(&process_info->restore_userptr_work,
			msecs_to_jiffies(AMDGPU_USERPTR_RESTORE_DELAY_MS));
}

/** amdgpu_amdkfd_gpuvm_restore_process_bos - Restore all BOs for the given
 *   KFD process identified by process_info
 *
 * @process_info: amdkfd_process_info of the KFD process
 *
 * After memory eviction, restore thread calls this function. The function
 * should be called when the Process is still valid. BO restore involves -
 *
 * 1.  Release old eviction fence and create new one
 * 2.  Get two copies of PD BO list from all the VMs. Keep one copy as pd_list.
 * 3   Use the second PD list and kfd_bo_list to create a list (ctx.list) of
 *     BOs that need to be reserved.
 * 4.  Reserve all the BOs
 * 5.  Validate of PD and PT BOs.
 * 6.  Validate all KFD BOs using kfd_bo_list and Map them and add new fence
 * 7.  Add fence to all PD and PT BOs.
 * 8.  Unreserve all BOs
 */
int amdgpu_amdkfd_gpuvm_restore_process_bos(void *info, struct dma_fence **ef)
{
	struct amdgpu_bo_list_entry *pd_bo_list;
	struct amdkfd_process_info *process_info = info;
	struct amdgpu_vm *peer_vm;
	struct kgd_mem *mem;
	struct bo_vm_reservation_context ctx;
	struct amdgpu_amdkfd_fence *new_fence;
	int ret = 0, i;
	struct list_head duplicate_save;
	struct amdgpu_sync sync_obj;
<<<<<<< HEAD
=======
	unsigned long failed_size = 0;
	unsigned long total_size = 0;
>>>>>>> upstream/android-13

	INIT_LIST_HEAD(&duplicate_save);
	INIT_LIST_HEAD(&ctx.list);
	INIT_LIST_HEAD(&ctx.duplicates);

	pd_bo_list = kcalloc(process_info->n_vms,
			     sizeof(struct amdgpu_bo_list_entry),
			     GFP_KERNEL);
	if (!pd_bo_list)
		return -ENOMEM;

	i = 0;
	mutex_lock(&process_info->lock);
	list_for_each_entry(peer_vm, &process_info->vm_list_head,
			vm_list_node)
		amdgpu_vm_get_pd_bo(peer_vm, &ctx.list, &pd_bo_list[i++]);

	/* Reserve all BOs and page tables/directory. Add all BOs from
	 * kfd_bo_list to ctx.list
	 */
	list_for_each_entry(mem, &process_info->kfd_bo_list,
			    validate_list.head) {

		list_add_tail(&mem->resv_list.head, &ctx.list);
		mem->resv_list.bo = mem->validate_list.bo;
<<<<<<< HEAD
		mem->resv_list.shared = mem->validate_list.shared;
=======
		mem->resv_list.num_shared = mem->validate_list.num_shared;
>>>>>>> upstream/android-13
	}

	ret = ttm_eu_reserve_buffers(&ctx.ticket, &ctx.list,
				     false, &duplicate_save);
	if (ret) {
		pr_debug("Memory eviction: TTM Reserve Failed. Try again\n");
		goto ttm_reserve_fail;
	}

	amdgpu_sync_create(&sync_obj);

	/* Validate PDs and PTs */
	ret = process_validate_vms(process_info);
	if (ret)
		goto validate_map_fail;

<<<<<<< HEAD
	/* Wait for PD/PTs validate to finish */
	/* FIXME: I think this isn't needed */
	list_for_each_entry(peer_vm, &process_info->vm_list_head,
			    vm_list_node) {
		struct amdgpu_bo *bo = peer_vm->root.base.bo;

		ttm_bo_wait(&bo->tbo, false, false);
=======
	ret = process_sync_pds_resv(process_info, &sync_obj);
	if (ret) {
		pr_debug("Memory eviction: Failed to sync to PD BO moving fence. Try again\n");
		goto validate_map_fail;
>>>>>>> upstream/android-13
	}

	/* Validate BOs and map them to GPUVM (update VM page tables). */
	list_for_each_entry(mem, &process_info->kfd_bo_list,
			    validate_list.head) {

		struct amdgpu_bo *bo = mem->bo;
		uint32_t domain = mem->domain;
<<<<<<< HEAD
		struct kfd_bo_va_list *bo_va_entry;

		ret = amdgpu_amdkfd_bo_validate(bo, domain, false);
		if (ret) {
			pr_debug("Memory eviction: Validate BOs failed. Try again\n");
			goto validate_map_fail;
		}

		list_for_each_entry(bo_va_entry, &mem->bo_va_list,
				    bo_list) {
			ret = update_gpuvm_pte((struct amdgpu_device *)
					      bo_va_entry->kgd_dev,
					      bo_va_entry,
					      &sync_obj);
=======
		struct kfd_mem_attachment *attachment;

		total_size += amdgpu_bo_size(bo);

		ret = amdgpu_amdkfd_bo_validate(bo, domain, false);
		if (ret) {
			pr_debug("Memory eviction: Validate BOs failed\n");
			failed_size += amdgpu_bo_size(bo);
			ret = amdgpu_amdkfd_bo_validate(bo,
						AMDGPU_GEM_DOMAIN_GTT, false);
			if (ret) {
				pr_debug("Memory eviction: Try again\n");
				goto validate_map_fail;
			}
		}
		ret = amdgpu_sync_fence(&sync_obj, bo->tbo.moving);
		if (ret) {
			pr_debug("Memory eviction: Sync BO fence failed. Try again\n");
			goto validate_map_fail;
		}
		list_for_each_entry(attachment, &mem->attachments, list) {
			if (!attachment->is_mapped)
				continue;

			kfd_mem_dmaunmap_attachment(mem, attachment);
			ret = update_gpuvm_pte(mem, attachment, &sync_obj, NULL);
>>>>>>> upstream/android-13
			if (ret) {
				pr_debug("Memory eviction: update PTE failed. Try again\n");
				goto validate_map_fail;
			}
		}
	}

<<<<<<< HEAD
=======
	if (failed_size)
		pr_debug("0x%lx/0x%lx in system\n", failed_size, total_size);

>>>>>>> upstream/android-13
	/* Update page directories */
	ret = process_update_pds(process_info, &sync_obj);
	if (ret) {
		pr_debug("Memory eviction: update PDs failed. Try again\n");
		goto validate_map_fail;
	}

<<<<<<< HEAD
=======
	/* Wait for validate and PT updates to finish */
>>>>>>> upstream/android-13
	amdgpu_sync_wait(&sync_obj, false);

	/* Release old eviction fence and create new one, because fence only
	 * goes from unsignaled to signaled, fence cannot be reused.
	 * Use context and mm from the old fence.
	 */
	new_fence = amdgpu_amdkfd_fence_create(
				process_info->eviction_fence->base.context,
<<<<<<< HEAD
				process_info->eviction_fence->mm);
=======
				process_info->eviction_fence->mm,
				NULL);
>>>>>>> upstream/android-13
	if (!new_fence) {
		pr_err("Failed to create eviction fence\n");
		ret = -ENOMEM;
		goto validate_map_fail;
	}
	dma_fence_put(&process_info->eviction_fence->base);
	process_info->eviction_fence = new_fence;
	*ef = dma_fence_get(&new_fence->base);

<<<<<<< HEAD
	/* Wait for validate to finish and attach new eviction fence */
	list_for_each_entry(mem, &process_info->kfd_bo_list,
		validate_list.head)
		ttm_bo_wait(&mem->bo->tbo, false, false);
=======
	/* Attach new eviction fence to all BOs */
>>>>>>> upstream/android-13
	list_for_each_entry(mem, &process_info->kfd_bo_list,
		validate_list.head)
		amdgpu_bo_fence(mem->bo,
			&process_info->eviction_fence->base, true);

	/* Attach eviction fence to PD / PT BOs */
	list_for_each_entry(peer_vm, &process_info->vm_list_head,
			    vm_list_node) {
<<<<<<< HEAD
		struct amdgpu_bo *bo = peer_vm->root.base.bo;
=======
		struct amdgpu_bo *bo = peer_vm->root.bo;
>>>>>>> upstream/android-13

		amdgpu_bo_fence(bo, &process_info->eviction_fence->base, true);
	}

validate_map_fail:
	ttm_eu_backoff_reservation(&ctx.ticket, &ctx.list);
	amdgpu_sync_free(&sync_obj);
ttm_reserve_fail:
	mutex_unlock(&process_info->lock);
	kfree(pd_bo_list);
	return ret;
}
<<<<<<< HEAD
=======

int amdgpu_amdkfd_add_gws_to_process(void *info, void *gws, struct kgd_mem **mem)
{
	struct amdkfd_process_info *process_info = (struct amdkfd_process_info *)info;
	struct amdgpu_bo *gws_bo = (struct amdgpu_bo *)gws;
	int ret;

	if (!info || !gws)
		return -EINVAL;

	*mem = kzalloc(sizeof(struct kgd_mem), GFP_KERNEL);
	if (!*mem)
		return -ENOMEM;

	mutex_init(&(*mem)->lock);
	INIT_LIST_HEAD(&(*mem)->attachments);
	(*mem)->bo = amdgpu_bo_ref(gws_bo);
	(*mem)->domain = AMDGPU_GEM_DOMAIN_GWS;
	(*mem)->process_info = process_info;
	add_kgd_mem_to_kfd_bo_list(*mem, process_info, false);
	amdgpu_sync_create(&(*mem)->sync);


	/* Validate gws bo the first time it is added to process */
	mutex_lock(&(*mem)->process_info->lock);
	ret = amdgpu_bo_reserve(gws_bo, false);
	if (unlikely(ret)) {
		pr_err("Reserve gws bo failed %d\n", ret);
		goto bo_reservation_failure;
	}

	ret = amdgpu_amdkfd_bo_validate(gws_bo, AMDGPU_GEM_DOMAIN_GWS, true);
	if (ret) {
		pr_err("GWS BO validate failed %d\n", ret);
		goto bo_validation_failure;
	}
	/* GWS resource is shared b/t amdgpu and amdkfd
	 * Add process eviction fence to bo so they can
	 * evict each other.
	 */
	ret = dma_resv_reserve_shared(gws_bo->tbo.base.resv, 1);
	if (ret)
		goto reserve_shared_fail;
	amdgpu_bo_fence(gws_bo, &process_info->eviction_fence->base, true);
	amdgpu_bo_unreserve(gws_bo);
	mutex_unlock(&(*mem)->process_info->lock);

	return ret;

reserve_shared_fail:
bo_validation_failure:
	amdgpu_bo_unreserve(gws_bo);
bo_reservation_failure:
	mutex_unlock(&(*mem)->process_info->lock);
	amdgpu_sync_free(&(*mem)->sync);
	remove_kgd_mem_from_kfd_bo_list(*mem, process_info);
	amdgpu_bo_unref(&gws_bo);
	mutex_destroy(&(*mem)->lock);
	kfree(*mem);
	*mem = NULL;
	return ret;
}

int amdgpu_amdkfd_remove_gws_from_process(void *info, void *mem)
{
	int ret;
	struct amdkfd_process_info *process_info = (struct amdkfd_process_info *)info;
	struct kgd_mem *kgd_mem = (struct kgd_mem *)mem;
	struct amdgpu_bo *gws_bo = kgd_mem->bo;

	/* Remove BO from process's validate list so restore worker won't touch
	 * it anymore
	 */
	remove_kgd_mem_from_kfd_bo_list(kgd_mem, process_info);

	ret = amdgpu_bo_reserve(gws_bo, false);
	if (unlikely(ret)) {
		pr_err("Reserve gws bo failed %d\n", ret);
		//TODO add BO back to validate_list?
		return ret;
	}
	amdgpu_amdkfd_remove_eviction_fence(gws_bo,
			process_info->eviction_fence);
	amdgpu_bo_unreserve(gws_bo);
	amdgpu_sync_free(&kgd_mem->sync);
	amdgpu_bo_unref(&gws_bo);
	mutex_destroy(&kgd_mem->lock);
	kfree(mem);
	return 0;
}

/* Returns GPU-specific tiling mode information */
int amdgpu_amdkfd_get_tile_config(struct kgd_dev *kgd,
				struct tile_config *config)
{
	struct amdgpu_device *adev = (struct amdgpu_device *)kgd;

	config->gb_addr_config = adev->gfx.config.gb_addr_config;
	config->tile_config_ptr = adev->gfx.config.tile_mode_array;
	config->num_tile_configs =
			ARRAY_SIZE(adev->gfx.config.tile_mode_array);
	config->macro_tile_config_ptr =
			adev->gfx.config.macrotile_mode_array;
	config->num_macro_tile_configs =
			ARRAY_SIZE(adev->gfx.config.macrotile_mode_array);

	/* Those values are not set from GFX9 onwards */
	config->num_banks = adev->gfx.config.num_banks;
	config->num_ranks = adev->gfx.config.num_ranks;

	return 0;
}
>>>>>>> upstream/android-13
