/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MM_TYPES_H
#define _LINUX_MM_TYPES_H

#include <linux/mm_types_task.h>

#include <linux/auxvec.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/rbtree.h>
#include <linux/rwsem.h>
#include <linux/completion.h>
#include <linux/cpumask.h>
#include <linux/uprobes.h>
#include <linux/page-flags-layout.h>
#include <linux/workqueue.h>
#include <linux/android_kabi.h>

#include <asm/mmu.h>

#ifndef AT_VECTOR_SIZE_ARCH
#define AT_VECTOR_SIZE_ARCH 0
#endif
#define AT_VECTOR_SIZE (2*(AT_VECTOR_SIZE_ARCH + AT_VECTOR_SIZE_BASE + 1))

typedef int vm_fault_t;

struct address_space;
struct mem_cgroup;
struct hmm;


#ifdef CONFIG_HAVE_ALIGNED_STRUCT_PAGE
#define _struct_page_alignment	__aligned(2 * sizeof(unsigned long))
#else
#define _struct_page_alignment
#endif

struct page {
	unsigned long flags;		
	
	union {
		struct {	
			
			struct list_head lru;
			
			struct address_space *mapping;
			pgoff_t index;		
			
			unsigned long private;
		};
		struct {	
			union {
				struct list_head slab_list;	
				struct {	
					struct page *next;
#ifdef CONFIG_64BIT
					int pages;	
					int pobjects;	
#else
					short int pages;
					short int pobjects;
#endif
				};
			};
			struct kmem_cache *slab_cache; 
			
			void *freelist;		
			union {
				void *s_mem;	
				unsigned long counters;		
				struct {			
					unsigned inuse:16;
					unsigned objects:15;
					unsigned frozen:1;
				};
			};
		};
		struct {	
			unsigned long compound_head;	

			
			unsigned char compound_dtor;
			unsigned char compound_order;
			atomic_t compound_mapcount;
		};
		struct {	
			unsigned long _compound_pad_1;	
			unsigned long _compound_pad_2;
			struct list_head deferred_list;
		};
		struct {	
			unsigned long _pt_pad_1;	
			pgtable_t pmd_huge_pte; 
			unsigned long _pt_pad_2;	
			union {
				struct mm_struct *pt_mm; 
				atomic_t pt_frag_refcount; 
			};
#if ALLOC_SPLIT_PTLOCKS
			spinlock_t *ptl;
#else
			spinlock_t ptl;
#endif
		};
		struct {	
			
			struct dev_pagemap *pgmap;
			unsigned long hmm_data;
			unsigned long _zd_pad_1;	
		};

		
		struct rcu_head rcu_head;
	};

	union {		
		
		atomic_t _mapcount;

		
		unsigned int page_type;

		unsigned int active;		
		int units;			
	};

	
	atomic_t _refcount;

#ifdef CONFIG_MEMCG
	struct mem_cgroup *mem_cgroup;
#endif

	
#if defined(WANT_PAGE_VIRTUAL)
	void *virtual;			
#endif 

#ifdef LAST_CPUPID_NOT_IN_PAGE_FLAGS
	int _last_cpupid;
#endif
} _struct_page_alignment;

#define PAGE_FRAG_CACHE_MAX_SIZE	__ALIGN_MASK(32768, ~PAGE_MASK)
#define PAGE_FRAG_CACHE_MAX_ORDER	get_order(PAGE_FRAG_CACHE_MAX_SIZE)

struct page_frag_cache {
	void * va;
#if (PAGE_SIZE < PAGE_FRAG_CACHE_MAX_SIZE)
	__u16 offset;
	__u16 size;
#else
	__u32 offset;
#endif
	
	unsigned int		pagecnt_bias;
	bool pfmemalloc;
};

typedef unsigned long vm_flags_t;

static inline atomic_t *compound_mapcount_ptr(struct page *page)
{
	return &page[1].compound_mapcount;
}


struct vm_region {
	struct rb_node	vm_rb;		
	vm_flags_t	vm_flags;	
	unsigned long	vm_start;	
	unsigned long	vm_end;		
	unsigned long	vm_top;		
	unsigned long	vm_pgoff;	
	struct file	*vm_file;	

	int		vm_usage;	
	bool		vm_icache_flushed : 1; 
};

#ifdef CONFIG_USERFAULTFD
#define NULL_VM_UFFD_CTX ((struct vm_userfaultfd_ctx) { NULL, })
struct vm_userfaultfd_ctx {
	struct userfaultfd_ctx *ctx;
};
#else 
#define NULL_VM_UFFD_CTX ((struct vm_userfaultfd_ctx) {})
struct vm_userfaultfd_ctx {};
#endif 


struct vm_area_struct {
	

	unsigned long vm_start;		
	unsigned long vm_end;		

	
	struct vm_area_struct *vm_next, *vm_prev;

	struct rb_node vm_rb;

	
	unsigned long rb_subtree_gap;

	

	struct mm_struct *vm_mm;	
	pgprot_t vm_page_prot;		
	unsigned long vm_flags;		

	
	union {
		struct {
			struct rb_node rb;
			unsigned long rb_subtree_last;
		} shared;
		const char __user *anon_name;
	};

	
	struct list_head anon_vma_chain; 
	struct anon_vma *anon_vma;	

	
	const struct vm_operations_struct *vm_ops;

	
	unsigned long vm_pgoff;		
	struct file * vm_file;		
	void * vm_private_data;		

	atomic_long_t swap_readahead_info;
#ifndef CONFIG_MMU
	struct vm_region *vm_region;	
#endif
#ifdef CONFIG_NUMA
	struct mempolicy *vm_policy;	
#endif
	struct vm_userfaultfd_ctx vm_userfaultfd_ctx;
#ifdef CONFIG_SPECULATIVE_PAGE_FAULT
	seqcount_t vm_sequence;
	atomic_t vm_ref_count;		
#endif

	ANDROID_KABI_RESERVE(1);
	ANDROID_KABI_RESERVE(2);
	ANDROID_KABI_RESERVE(3);
	ANDROID_KABI_RESERVE(4);
} __randomize_layout;

struct core_thread {
	struct task_struct *task;
	struct core_thread *next;
};

struct core_state {
	atomic_t nr_threads;
	struct core_thread dumper;
	struct completion startup;
};

struct kioctx_table;
struct mm_struct {
	struct {
		struct vm_area_struct *mmap;		
		struct rb_root mm_rb;
#ifdef CONFIG_SPECULATIVE_PAGE_FAULT
		rwlock_t mm_rb_lock;
#endif
		u64 vmacache_seqnum;                   
#ifdef CONFIG_MMU
		unsigned long (*get_unmapped_area) (struct file *filp,
				unsigned long addr, unsigned long len,
				unsigned long pgoff, unsigned long flags);
#endif
		unsigned long mmap_base;	
		unsigned long mmap_legacy_base;	
#ifdef CONFIG_HAVE_ARCH_COMPAT_MMAP_BASES
		
		unsigned long mmap_compat_base;
		unsigned long mmap_compat_legacy_base;
#endif
		unsigned long task_size;	
		unsigned long highest_vm_end;	
		pgd_t * pgd;

		
		atomic_t mm_users;

		
		atomic_t mm_count;

#ifdef CONFIG_MMU
		atomic_long_t pgtables_bytes;	
#endif
		int map_count;			

		spinlock_t page_table_lock; 
		struct rw_semaphore mmap_sem;

		struct list_head mmlist; 


		unsigned long hiwater_rss; 
		unsigned long hiwater_vm;  

		unsigned long total_vm;	   
		unsigned long locked_vm;   
		unsigned long pinned_vm;   
		unsigned long data_vm;	   
		unsigned long exec_vm;	   
		unsigned long stack_vm;	   
		unsigned long def_flags;

		spinlock_t arg_lock; 
		unsigned long start_code, end_code, start_data, end_data;
		unsigned long start_brk, brk, start_stack;
		unsigned long arg_start, arg_end, env_start, env_end;

		unsigned long saved_auxv[AT_VECTOR_SIZE]; 

		
		struct mm_rss_stat rss_stat;

		struct linux_binfmt *binfmt;

		
		mm_context_t context;

		unsigned long flags; 

		struct core_state *core_state; 
#ifdef CONFIG_MEMBARRIER
		atomic_t membarrier_state;
#endif
#ifdef CONFIG_AIO
		spinlock_t			ioctx_lock;
		struct kioctx_table __rcu	*ioctx_table;
#endif
#ifdef CONFIG_MEMCG
		
		struct task_struct __rcu *owner;
#endif
		struct user_namespace *user_ns;

		
		struct file __rcu *exe_file;
#ifdef CONFIG_MMU_NOTIFIER
		struct mmu_notifier_mm *mmu_notifier_mm;
#endif
#if defined(CONFIG_TRANSPARENT_HUGEPAGE) && !USE_SPLIT_PMD_PTLOCKS
		pgtable_t pmd_huge_pte; 
#endif
#ifdef CONFIG_NUMA_BALANCING
		
		unsigned long numa_next_scan;

		
		unsigned long numa_scan_offset;

		
		int numa_scan_seq;
#endif
		
		atomic_t tlb_flush_pending;
#ifdef CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH
		
		bool tlb_flush_batched;
#endif
		struct uprobes_state uprobes_state;
#ifdef CONFIG_HUGETLB_PAGE
		atomic_long_t hugetlb_usage;
#endif
		struct work_struct async_put_work;

#if IS_ENABLED(CONFIG_HMM)
		
		struct hmm *hmm;
#endif
	} __randomize_layout;

	
	unsigned long cpu_bitmap[];
};

extern struct mm_struct init_mm;


static inline void mm_init_cpumask(struct mm_struct *mm)
{
	unsigned long cpu_bitmap = (unsigned long)mm;

	cpu_bitmap += offsetof(struct mm_struct, cpu_bitmap);
	cpumask_clear((struct cpumask *)cpu_bitmap);
}


static inline cpumask_t *mm_cpumask(struct mm_struct *mm)
{
	return (struct cpumask *)&mm->cpu_bitmap;
}

struct mmu_gather;
extern void tlb_gather_mmu(struct mmu_gather *tlb, struct mm_struct *mm,
				unsigned long start, unsigned long end);
extern void tlb_finish_mmu(struct mmu_gather *tlb,
				unsigned long start, unsigned long end);

static inline void init_tlb_flush_pending(struct mm_struct *mm)
{
	atomic_set(&mm->tlb_flush_pending, 0);
}

static inline void inc_tlb_flush_pending(struct mm_struct *mm)
{
	atomic_inc(&mm->tlb_flush_pending);
	
}

static inline void dec_tlb_flush_pending(struct mm_struct *mm)
{
	
	atomic_dec(&mm->tlb_flush_pending);
}

static inline bool mm_tlb_flush_pending(struct mm_struct *mm)
{
	
	return atomic_read(&mm->tlb_flush_pending);
}

static inline bool mm_tlb_flush_nested(struct mm_struct *mm)
{
	
	return atomic_read(&mm->tlb_flush_pending) > 1;
}

struct vm_fault;

struct vm_special_mapping {
	const char *name;	

	
	struct page **pages;

	
	vm_fault_t (*fault)(const struct vm_special_mapping *sm,
				struct vm_area_struct *vma,
				struct vm_fault *vmf);

	int (*mremap)(const struct vm_special_mapping *sm,
		     struct vm_area_struct *new_vma);
};

enum tlb_flush_reason {
	TLB_FLUSH_ON_TASK_SWITCH,
	TLB_REMOTE_SHOOTDOWN,
	TLB_LOCAL_SHOOTDOWN,
	TLB_LOCAL_MM_SHOOTDOWN,
	TLB_REMOTE_SEND_IPI,
	NR_TLB_FLUSH_REASONS,
};

 
typedef struct {
	unsigned long val;
} swp_entry_t;


static inline const char __user *vma_get_anon_name(struct vm_area_struct *vma)
{
	if (vma->vm_file)
		return NULL;

	return vma->anon_name;
}

#endif 
