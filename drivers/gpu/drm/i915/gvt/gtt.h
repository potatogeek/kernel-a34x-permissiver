/*
 * Copyright(c) 2011-2016 Intel Corporation. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Zhi Wang <zhi.a.wang@intel.com>
 *    Zhenyu Wang <zhenyuw@linux.intel.com>
 *    Xiao Zheng <xiao.zheng@intel.com>
 *
 * Contributors:
 *    Min He <min.he@intel.com>
 *    Bing Niu <bing.niu@intel.com>
 *
 */

#ifndef _GVT_GTT_H_
#define _GVT_GTT_H_

<<<<<<< HEAD
#define I915_GTT_PAGE_SHIFT         12

struct intel_vgpu_mm;

=======
#include <linux/kernel.h>
#include <linux/kref.h>
#include <linux/mutex.h>
#include <linux/radix-tree.h>

#include "gt/intel_gtt.h"

struct intel_gvt;
struct intel_vgpu;
struct intel_vgpu_mm;

#define I915_GTT_PAGE_SHIFT         12

>>>>>>> upstream/android-13
#define INTEL_GVT_INVALID_ADDR (~0UL)

struct intel_gvt_gtt_entry {
	u64 val64;
	int type;
};

struct intel_gvt_gtt_pte_ops {
	int (*get_entry)(void *pt,
			 struct intel_gvt_gtt_entry *e,
			 unsigned long index,
			 bool hypervisor_access,
			 unsigned long gpa,
			 struct intel_vgpu *vgpu);
	int (*set_entry)(void *pt,
			 struct intel_gvt_gtt_entry *e,
			 unsigned long index,
			 bool hypervisor_access,
			 unsigned long gpa,
			 struct intel_vgpu *vgpu);
	bool (*test_present)(struct intel_gvt_gtt_entry *e);
	void (*clear_present)(struct intel_gvt_gtt_entry *e);
	void (*set_present)(struct intel_gvt_gtt_entry *e);
	bool (*test_pse)(struct intel_gvt_gtt_entry *e);
	void (*clear_pse)(struct intel_gvt_gtt_entry *e);
	bool (*test_ips)(struct intel_gvt_gtt_entry *e);
	void (*clear_ips)(struct intel_gvt_gtt_entry *e);
	bool (*test_64k_splited)(struct intel_gvt_gtt_entry *e);
	void (*clear_64k_splited)(struct intel_gvt_gtt_entry *e);
	void (*set_64k_splited)(struct intel_gvt_gtt_entry *e);
	void (*set_pfn)(struct intel_gvt_gtt_entry *e, unsigned long pfn);
	unsigned long (*get_pfn)(struct intel_gvt_gtt_entry *e);
};

struct intel_gvt_gtt_gma_ops {
	unsigned long (*gma_to_ggtt_pte_index)(unsigned long gma);
	unsigned long (*gma_to_pte_index)(unsigned long gma);
	unsigned long (*gma_to_pde_index)(unsigned long gma);
	unsigned long (*gma_to_l3_pdp_index)(unsigned long gma);
	unsigned long (*gma_to_l4_pdp_index)(unsigned long gma);
	unsigned long (*gma_to_pml4_index)(unsigned long gma);
};

struct intel_gvt_gtt {
	struct intel_gvt_gtt_pte_ops *pte_ops;
	struct intel_gvt_gtt_gma_ops *gma_ops;
	int (*mm_alloc_page_table)(struct intel_vgpu_mm *mm);
	void (*mm_free_page_table)(struct intel_vgpu_mm *mm);
	struct list_head oos_page_use_list_head;
	struct list_head oos_page_free_list_head;
<<<<<<< HEAD
=======
	struct mutex ppgtt_mm_lock;
>>>>>>> upstream/android-13
	struct list_head ppgtt_mm_lru_list_head;

	struct page *scratch_page;
	unsigned long scratch_mfn;
};

<<<<<<< HEAD
typedef enum {
	GTT_TYPE_INVALID = -1,
=======
enum intel_gvt_gtt_type {
	GTT_TYPE_INVALID = 0,
>>>>>>> upstream/android-13

	GTT_TYPE_GGTT_PTE,

	GTT_TYPE_PPGTT_PTE_4K_ENTRY,
	GTT_TYPE_PPGTT_PTE_64K_ENTRY,
	GTT_TYPE_PPGTT_PTE_2M_ENTRY,
	GTT_TYPE_PPGTT_PTE_1G_ENTRY,

	GTT_TYPE_PPGTT_PTE_ENTRY,

	GTT_TYPE_PPGTT_PDE_ENTRY,
	GTT_TYPE_PPGTT_PDP_ENTRY,
	GTT_TYPE_PPGTT_PML4_ENTRY,

	GTT_TYPE_PPGTT_ROOT_ENTRY,

	GTT_TYPE_PPGTT_ROOT_L3_ENTRY,
	GTT_TYPE_PPGTT_ROOT_L4_ENTRY,

	GTT_TYPE_PPGTT_ENTRY,

	GTT_TYPE_PPGTT_PTE_PT,
	GTT_TYPE_PPGTT_PDE_PT,
	GTT_TYPE_PPGTT_PDP_PT,
	GTT_TYPE_PPGTT_PML4_PT,

	GTT_TYPE_MAX,
<<<<<<< HEAD
} intel_gvt_gtt_type_t;
=======
};
>>>>>>> upstream/android-13

enum intel_gvt_mm_type {
	INTEL_GVT_MM_GGTT,
	INTEL_GVT_MM_PPGTT,
};

#define GVT_RING_CTX_NR_PDPS	GEN8_3LVL_PDPES

<<<<<<< HEAD
=======
struct intel_gvt_partial_pte {
	unsigned long offset;
	u64 data;
	struct list_head list;
};

>>>>>>> upstream/android-13
struct intel_vgpu_mm {
	enum intel_gvt_mm_type type;
	struct intel_vgpu *vgpu;

	struct kref ref;
	atomic_t pincount;

	union {
		struct {
<<<<<<< HEAD
			intel_gvt_gtt_type_t root_entry_type;
=======
			enum intel_gvt_gtt_type root_entry_type;
>>>>>>> upstream/android-13
			/*
			 * The 4 PDPs in ring context. For 48bit addressing,
			 * only PDP0 is valid and point to PML4. For 32it
			 * addressing, all 4 are used as true PDPs.
			 */
			u64 guest_pdps[GVT_RING_CTX_NR_PDPS];
			u64 shadow_pdps[GVT_RING_CTX_NR_PDPS];
			bool shadowed;

			struct list_head list;
			struct list_head lru_list;
<<<<<<< HEAD
		} ppgtt_mm;
		struct {
			void *virtual_ggtt;
			unsigned long last_partial_off;
			u64 last_partial_data;
=======
			struct list_head link; /* possible LRI shadow mm list */
		} ppgtt_mm;
		struct {
			void *virtual_ggtt;
			/* Save/restore for PM */
			u64 *host_ggtt_aperture;
			u64 *host_ggtt_hidden;
			struct list_head partial_pte_list;
>>>>>>> upstream/android-13
		} ggtt_mm;
	};
};

struct intel_vgpu_mm *intel_vgpu_create_ppgtt_mm(struct intel_vgpu *vgpu,
<<<<<<< HEAD
		intel_gvt_gtt_type_t root_entry_type, u64 pdps[]);
=======
		enum intel_gvt_gtt_type root_entry_type, u64 pdps[]);
>>>>>>> upstream/android-13

static inline void intel_vgpu_mm_get(struct intel_vgpu_mm *mm)
{
	kref_get(&mm->ref);
}

void _intel_vgpu_mm_release(struct kref *mm_ref);

static inline void intel_vgpu_mm_put(struct intel_vgpu_mm *mm)
{
	kref_put(&mm->ref, _intel_vgpu_mm_release);
}

static inline void intel_vgpu_destroy_mm(struct intel_vgpu_mm *mm)
{
	intel_vgpu_mm_put(mm);
}

struct intel_vgpu_guest_page;

struct intel_vgpu_scratch_pt {
	struct page *page;
	unsigned long page_mfn;
};

struct intel_vgpu_gtt {
	struct intel_vgpu_mm *ggtt_mm;
	unsigned long active_ppgtt_mm_bitmap;
	struct list_head ppgtt_mm_list_head;
	struct radix_tree_root spt_tree;
	struct list_head oos_page_list_head;
	struct list_head post_shadow_list_head;
	struct intel_vgpu_scratch_pt scratch_pt[GTT_TYPE_MAX];
};

<<<<<<< HEAD
extern int intel_vgpu_init_gtt(struct intel_vgpu *vgpu);
extern void intel_vgpu_clean_gtt(struct intel_vgpu *vgpu);
void intel_vgpu_reset_ggtt(struct intel_vgpu *vgpu, bool invalidate_old);
void intel_vgpu_invalidate_ppgtt(struct intel_vgpu *vgpu);

extern int intel_gvt_init_gtt(struct intel_gvt *gvt);
void intel_vgpu_reset_gtt(struct intel_vgpu *vgpu);
extern void intel_gvt_clean_gtt(struct intel_gvt *gvt);

extern struct intel_vgpu_mm *intel_gvt_find_ppgtt_mm(struct intel_vgpu *vgpu,
		int page_table_level, void *root_entry);
=======
int intel_vgpu_init_gtt(struct intel_vgpu *vgpu);
void intel_vgpu_clean_gtt(struct intel_vgpu *vgpu);
void intel_vgpu_reset_ggtt(struct intel_vgpu *vgpu, bool invalidate_old);
void intel_vgpu_invalidate_ppgtt(struct intel_vgpu *vgpu);

int intel_gvt_init_gtt(struct intel_gvt *gvt);
void intel_vgpu_reset_gtt(struct intel_vgpu *vgpu);
void intel_gvt_clean_gtt(struct intel_gvt *gvt);

struct intel_vgpu_mm *intel_gvt_find_ppgtt_mm(struct intel_vgpu *vgpu,
					      int page_table_level,
					      void *root_entry);
>>>>>>> upstream/android-13

struct intel_vgpu_oos_page {
	struct intel_vgpu_ppgtt_spt *spt;
	struct list_head list;
	struct list_head vm_list;
	int id;
<<<<<<< HEAD
	unsigned char mem[I915_GTT_PAGE_SIZE];
=======
	void *mem;
>>>>>>> upstream/android-13
};

#define GTT_ENTRY_NUM_IN_ONE_PAGE 512

/* Represent a vgpu shadow page table. */
struct intel_vgpu_ppgtt_spt {
	atomic_t refcount;
	struct intel_vgpu *vgpu;

	struct {
<<<<<<< HEAD
		intel_gvt_gtt_type_t type;
=======
		enum intel_gvt_gtt_type type;
>>>>>>> upstream/android-13
		bool pde_ips; /* for 64KB PTEs */
		void *vaddr;
		struct page *page;
		unsigned long mfn;
	} shadow_page;

	struct {
<<<<<<< HEAD
		intel_gvt_gtt_type_t type;
=======
		enum intel_gvt_gtt_type type;
>>>>>>> upstream/android-13
		bool pde_ips; /* for 64KB PTEs */
		unsigned long gfn;
		unsigned long write_cnt;
		struct intel_vgpu_oos_page *oos_page;
	} guest_page;

	DECLARE_BITMAP(post_shadow_bitmap, GTT_ENTRY_NUM_IN_ONE_PAGE);
	struct list_head post_shadow_list;
};

int intel_vgpu_sync_oos_pages(struct intel_vgpu *vgpu);

int intel_vgpu_flush_post_shadow(struct intel_vgpu *vgpu);

int intel_vgpu_pin_mm(struct intel_vgpu_mm *mm);

void intel_vgpu_unpin_mm(struct intel_vgpu_mm *mm);

unsigned long intel_vgpu_gma_to_gpa(struct intel_vgpu_mm *mm,
		unsigned long gma);

struct intel_vgpu_mm *intel_vgpu_find_ppgtt_mm(struct intel_vgpu *vgpu,
		u64 pdps[]);

struct intel_vgpu_mm *intel_vgpu_get_ppgtt_mm(struct intel_vgpu *vgpu,
<<<<<<< HEAD
		intel_gvt_gtt_type_t root_entry_type, u64 pdps[]);
=======
		enum intel_gvt_gtt_type root_entry_type, u64 pdps[]);
>>>>>>> upstream/android-13

int intel_vgpu_put_ppgtt_mm(struct intel_vgpu *vgpu, u64 pdps[]);

int intel_vgpu_emulate_ggtt_mmio_read(struct intel_vgpu *vgpu,
	unsigned int off, void *p_data, unsigned int bytes);

int intel_vgpu_emulate_ggtt_mmio_write(struct intel_vgpu *vgpu,
	unsigned int off, void *p_data, unsigned int bytes);

<<<<<<< HEAD
=======
void intel_vgpu_destroy_all_ppgtt_mm(struct intel_vgpu *vgpu);
void intel_gvt_restore_ggtt(struct intel_gvt *gvt);

>>>>>>> upstream/android-13
#endif /* _GVT_GTT_H_ */
