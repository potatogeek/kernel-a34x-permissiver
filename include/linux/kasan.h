/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KASAN_H
#define _LINUX_KASAN_H

<<<<<<< HEAD
=======
#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/static_key.h>
>>>>>>> upstream/android-13
#include <linux/types.h>

struct kmem_cache;
struct page;
struct vm_struct;
struct task_struct;

#ifdef CONFIG_KASAN

<<<<<<< HEAD
#include <asm/kasan.h>
#include <asm/pgtable.h>

extern unsigned char kasan_early_shadow_page[PAGE_SIZE];
extern pte_t kasan_early_shadow_pte[PTRS_PER_PTE];
extern pmd_t kasan_early_shadow_pmd[PTRS_PER_PMD];
extern pud_t kasan_early_shadow_pud[PTRS_PER_PUD];
=======
#include <linux/linkage.h>
#include <asm/kasan.h>

#endif

typedef unsigned int __bitwise kasan_vmalloc_flags_t;

#define KASAN_VMALLOC_NONE		((__force kasan_vmalloc_flags_t)0x00u)
#define KASAN_VMALLOC_INIT		((__force kasan_vmalloc_flags_t)0x01u)
#define KASAN_VMALLOC_VM_ALLOC		((__force kasan_vmalloc_flags_t)0x02u)
#define KASAN_VMALLOC_PROT_NORMAL	((__force kasan_vmalloc_flags_t)0x04u)

#if defined(CONFIG_KASAN_GENERIC) || defined(CONFIG_KASAN_SW_TAGS)

#include <linux/pgtable.h>

/* Software KASAN implementations use shadow memory. */

#ifdef CONFIG_KASAN_SW_TAGS
/* This matches KASAN_TAG_INVALID. */
#define KASAN_SHADOW_INIT 0xFE
#else
#define KASAN_SHADOW_INIT 0
#endif

#ifndef PTE_HWTABLE_PTRS
#define PTE_HWTABLE_PTRS 0
#endif

extern unsigned char kasan_early_shadow_page[PAGE_SIZE];
extern pte_t kasan_early_shadow_pte[MAX_PTRS_PER_PTE + PTE_HWTABLE_PTRS];
extern pmd_t kasan_early_shadow_pmd[MAX_PTRS_PER_PMD];
extern pud_t kasan_early_shadow_pud[MAX_PTRS_PER_PUD];
>>>>>>> upstream/android-13
extern p4d_t kasan_early_shadow_p4d[MAX_PTRS_PER_P4D];

int kasan_populate_early_shadow(const void *shadow_start,
				const void *shadow_end);

static inline void *kasan_mem_to_shadow(const void *addr)
{
	return (void *)((unsigned long)addr >> KASAN_SHADOW_SCALE_SHIFT)
		+ KASAN_SHADOW_OFFSET;
}

<<<<<<< HEAD
=======
int kasan_add_zero_shadow(void *start, unsigned long size);
void kasan_remove_zero_shadow(void *start, unsigned long size);

>>>>>>> upstream/android-13
/* Enable reporting bugs after kasan_disable_current() */
extern void kasan_enable_current(void);

/* Disable reporting bugs for current task */
extern void kasan_disable_current(void);

<<<<<<< HEAD
void kasan_unpoison_shadow(const void *address, size_t size);

void kasan_unpoison_task_stack(struct task_struct *task);
void kasan_unpoison_stack_above_sp_to(const void *watermark);

void kasan_alloc_pages(struct page *page, unsigned int order);
void kasan_free_pages(struct page *page, unsigned int order);

void kasan_cache_create(struct kmem_cache *cache, unsigned int *size,
			slab_flags_t *flags);

void kasan_poison_slab(struct page *page);
void kasan_unpoison_object_data(struct kmem_cache *cache, void *object);
void kasan_poison_object_data(struct kmem_cache *cache, void *object);
void * __must_check kasan_init_slab_obj(struct kmem_cache *cache,
					const void *object);

void * __must_check kasan_kmalloc_large(const void *ptr, size_t size,
						gfp_t flags);
void kasan_kfree_large(void *ptr, unsigned long ip);
void kasan_poison_kfree(void *ptr, unsigned long ip);
void * __must_check kasan_kmalloc(struct kmem_cache *s, const void *object,
					size_t size, gfp_t flags);
void * __must_check kasan_krealloc(const void *object, size_t new_size,
					gfp_t flags);

void * __must_check kasan_slab_alloc(struct kmem_cache *s, void *object,
					gfp_t flags);
bool kasan_slab_free(struct kmem_cache *s, void *object, unsigned long ip);
=======
#else /* CONFIG_KASAN_GENERIC || CONFIG_KASAN_SW_TAGS */

static inline int kasan_add_zero_shadow(void *start, unsigned long size)
{
	return 0;
}
static inline void kasan_remove_zero_shadow(void *start,
					unsigned long size)
{}

static inline void kasan_enable_current(void) {}
static inline void kasan_disable_current(void) {}

#endif /* CONFIG_KASAN_GENERIC || CONFIG_KASAN_SW_TAGS */

#ifdef CONFIG_KASAN_HW_TAGS

DECLARE_STATIC_KEY_FALSE(kasan_flag_enabled);

static __always_inline bool kasan_enabled(void)
{
	return static_branch_likely(&kasan_flag_enabled);
}

static inline bool kasan_hw_tags_enabled(void)
{
	return kasan_enabled();
}

#else /* CONFIG_KASAN_HW_TAGS */

static inline bool kasan_enabled(void)
{
	return IS_ENABLED(CONFIG_KASAN);
}

static inline bool kasan_hw_tags_enabled(void)
{
	return false;
}

#endif /* CONFIG_KASAN_HW_TAGS */

static inline bool kasan_has_integrated_init(void)
{
	return kasan_hw_tags_enabled();
}

#ifdef CONFIG_KASAN
>>>>>>> upstream/android-13

struct kasan_cache {
	int alloc_meta_offset;
	int free_meta_offset;
<<<<<<< HEAD
};

int kasan_module_alloc(void *addr, size_t size);
void kasan_free_shadow(const struct vm_struct *vm);

int kasan_add_zero_shadow(void *start, unsigned long size);
void kasan_remove_zero_shadow(void *start, unsigned long size);

size_t ksize(const void *);
static inline void kasan_unpoison_slab(const void *ptr) { ksize(ptr); }
size_t kasan_metadata_size(struct kmem_cache *cache);
=======
	bool is_kmalloc;
};

slab_flags_t __kasan_never_merge(void);
static __always_inline slab_flags_t kasan_never_merge(void)
{
	if (kasan_enabled())
		return __kasan_never_merge();
	return 0;
}

void __kasan_unpoison_range(const void *addr, size_t size);
static __always_inline void kasan_unpoison_range(const void *addr, size_t size)
{
	if (kasan_enabled())
		__kasan_unpoison_range(addr, size);
}

void __kasan_poison_pages(struct page *page, unsigned int order, bool init);
static __always_inline void kasan_poison_pages(struct page *page,
						unsigned int order, bool init)
{
	if (kasan_enabled())
		__kasan_poison_pages(page, order, init);
}

void __kasan_unpoison_pages(struct page *page, unsigned int order, bool init);
static __always_inline void kasan_unpoison_pages(struct page *page,
						 unsigned int order, bool init)
{
	if (kasan_enabled())
		__kasan_unpoison_pages(page, order, init);
}

void __kasan_cache_create(struct kmem_cache *cache, unsigned int *size,
				slab_flags_t *flags);
static __always_inline void kasan_cache_create(struct kmem_cache *cache,
				unsigned int *size, slab_flags_t *flags)
{
	if (kasan_enabled())
		__kasan_cache_create(cache, size, flags);
}

void __kasan_cache_create_kmalloc(struct kmem_cache *cache);
static __always_inline void kasan_cache_create_kmalloc(struct kmem_cache *cache)
{
	if (kasan_enabled())
		__kasan_cache_create_kmalloc(cache);
}

size_t __kasan_metadata_size(struct kmem_cache *cache);
static __always_inline size_t kasan_metadata_size(struct kmem_cache *cache)
{
	if (kasan_enabled())
		return __kasan_metadata_size(cache);
	return 0;
}

void __kasan_poison_slab(struct page *page);
static __always_inline void kasan_poison_slab(struct page *page)
{
	if (kasan_enabled())
		__kasan_poison_slab(page);
}

void __kasan_unpoison_object_data(struct kmem_cache *cache, void *object);
static __always_inline void kasan_unpoison_object_data(struct kmem_cache *cache,
							void *object)
{
	if (kasan_enabled())
		__kasan_unpoison_object_data(cache, object);
}

void __kasan_poison_object_data(struct kmem_cache *cache, void *object);
static __always_inline void kasan_poison_object_data(struct kmem_cache *cache,
							void *object)
{
	if (kasan_enabled())
		__kasan_poison_object_data(cache, object);
}

void * __must_check __kasan_init_slab_obj(struct kmem_cache *cache,
					  const void *object);
static __always_inline void * __must_check kasan_init_slab_obj(
				struct kmem_cache *cache, const void *object)
{
	if (kasan_enabled())
		return __kasan_init_slab_obj(cache, object);
	return (void *)object;
}

bool __kasan_slab_free(struct kmem_cache *s, void *object,
			unsigned long ip, bool init);
static __always_inline bool kasan_slab_free(struct kmem_cache *s,
						void *object, bool init)
{
	if (kasan_enabled())
		return __kasan_slab_free(s, object, _RET_IP_, init);
	return false;
}

void __kasan_kfree_large(void *ptr, unsigned long ip);
static __always_inline void kasan_kfree_large(void *ptr)
{
	if (kasan_enabled())
		__kasan_kfree_large(ptr, _RET_IP_);
}

void __kasan_slab_free_mempool(void *ptr, unsigned long ip);
static __always_inline void kasan_slab_free_mempool(void *ptr)
{
	if (kasan_enabled())
		__kasan_slab_free_mempool(ptr, _RET_IP_);
}

void * __must_check __kasan_slab_alloc(struct kmem_cache *s,
				       void *object, gfp_t flags, bool init);
static __always_inline void * __must_check kasan_slab_alloc(
		struct kmem_cache *s, void *object, gfp_t flags, bool init)
{
	if (kasan_enabled())
		return __kasan_slab_alloc(s, object, flags, init);
	return object;
}

void * __must_check __kasan_kmalloc(struct kmem_cache *s, const void *object,
				    size_t size, gfp_t flags);
static __always_inline void * __must_check kasan_kmalloc(struct kmem_cache *s,
				const void *object, size_t size, gfp_t flags)
{
	if (kasan_enabled())
		return __kasan_kmalloc(s, object, size, flags);
	return (void *)object;
}

void * __must_check __kasan_kmalloc_large(const void *ptr,
					  size_t size, gfp_t flags);
static __always_inline void * __must_check kasan_kmalloc_large(const void *ptr,
						      size_t size, gfp_t flags)
{
	if (kasan_enabled())
		return __kasan_kmalloc_large(ptr, size, flags);
	return (void *)ptr;
}

void * __must_check __kasan_krealloc(const void *object,
				     size_t new_size, gfp_t flags);
static __always_inline void * __must_check kasan_krealloc(const void *object,
						 size_t new_size, gfp_t flags)
{
	if (kasan_enabled())
		return __kasan_krealloc(object, new_size, flags);
	return (void *)object;
}

/*
 * Unlike kasan_check_read/write(), kasan_check_byte() is performed even for
 * the hardware tag-based mode that doesn't rely on compiler instrumentation.
 */
bool __kasan_check_byte(const void *addr, unsigned long ip);
static __always_inline bool kasan_check_byte(const void *addr)
{
	if (kasan_enabled())
		return __kasan_check_byte(addr, _RET_IP_);
	return true;
}

>>>>>>> upstream/android-13

bool kasan_save_enable_multi_shot(void);
void kasan_restore_multi_shot(bool enabled);

#else /* CONFIG_KASAN */

<<<<<<< HEAD
static inline void kasan_unpoison_shadow(const void *address, size_t size) {}

static inline void kasan_unpoison_task_stack(struct task_struct *task) {}
static inline void kasan_unpoison_stack_above_sp_to(const void *watermark) {}

static inline void kasan_enable_current(void) {}
static inline void kasan_disable_current(void) {}

static inline void kasan_alloc_pages(struct page *page, unsigned int order) {}
static inline void kasan_free_pages(struct page *page, unsigned int order) {}

static inline void kasan_cache_create(struct kmem_cache *cache,
				      unsigned int *size,
				      slab_flags_t *flags) {}

=======
static inline slab_flags_t kasan_never_merge(void)
{
	return 0;
}
static inline void kasan_unpoison_range(const void *address, size_t size) {}
static inline void kasan_poison_pages(struct page *page, unsigned int order,
				      bool init) {}
static inline void kasan_unpoison_pages(struct page *page, unsigned int order,
					bool init) {}
static inline void kasan_cache_create(struct kmem_cache *cache,
				      unsigned int *size,
				      slab_flags_t *flags) {}
static inline void kasan_cache_create_kmalloc(struct kmem_cache *cache) {}
static inline size_t kasan_metadata_size(struct kmem_cache *cache) { return 0; }
>>>>>>> upstream/android-13
static inline void kasan_poison_slab(struct page *page) {}
static inline void kasan_unpoison_object_data(struct kmem_cache *cache,
					void *object) {}
static inline void kasan_poison_object_data(struct kmem_cache *cache,
					void *object) {}
static inline void *kasan_init_slab_obj(struct kmem_cache *cache,
				const void *object)
{
	return (void *)object;
}
<<<<<<< HEAD

static inline void *kasan_kmalloc_large(void *ptr, size_t size, gfp_t flags)
{
	return ptr;
}
static inline void kasan_kfree_large(void *ptr, unsigned long ip) {}
static inline void kasan_poison_kfree(void *ptr, unsigned long ip) {}
=======
static inline bool kasan_slab_free(struct kmem_cache *s, void *object, bool init)
{
	return false;
}
static inline void kasan_kfree_large(void *ptr) {}
static inline void kasan_slab_free_mempool(void *ptr) {}
static inline void *kasan_slab_alloc(struct kmem_cache *s, void *object,
				   gfp_t flags, bool init)
{
	return object;
}
>>>>>>> upstream/android-13
static inline void *kasan_kmalloc(struct kmem_cache *s, const void *object,
				size_t size, gfp_t flags)
{
	return (void *)object;
}
<<<<<<< HEAD
=======
static inline void *kasan_kmalloc_large(const void *ptr, size_t size, gfp_t flags)
{
	return (void *)ptr;
}
>>>>>>> upstream/android-13
static inline void *kasan_krealloc(const void *object, size_t new_size,
				 gfp_t flags)
{
	return (void *)object;
}
<<<<<<< HEAD

static inline void *kasan_slab_alloc(struct kmem_cache *s, void *object,
				   gfp_t flags)
{
	return object;
}
static inline bool kasan_slab_free(struct kmem_cache *s, void *object,
				   unsigned long ip)
{
	return false;
}

static inline int kasan_module_alloc(void *addr, size_t size) { return 0; }
static inline void kasan_free_shadow(const struct vm_struct *vm) {}

static inline int kasan_add_zero_shadow(void *start, unsigned long size)
{
	return 0;
}
static inline void kasan_remove_zero_shadow(void *start,
					unsigned long size)
{}

static inline void kasan_unpoison_slab(const void *ptr) { }
static inline size_t kasan_metadata_size(struct kmem_cache *cache) { return 0; }

#endif /* CONFIG_KASAN */

#ifdef CONFIG_KASAN_GENERIC

#define KASAN_SHADOW_INIT 0

void kasan_cache_shrink(struct kmem_cache *cache);
void kasan_cache_shutdown(struct kmem_cache *cache);
=======
static inline bool kasan_check_byte(const void *address)
{
	return true;
}

#endif /* CONFIG_KASAN */

#if defined(CONFIG_KASAN) && defined(CONFIG_KASAN_STACK)
void kasan_unpoison_task_stack(struct task_struct *task);
#else
static inline void kasan_unpoison_task_stack(struct task_struct *task) {}
#endif

#ifdef CONFIG_KASAN_GENERIC

void kasan_cache_shrink(struct kmem_cache *cache);
void kasan_cache_shutdown(struct kmem_cache *cache);
void kasan_record_aux_stack(void *ptr);
void kasan_record_aux_stack_noalloc(void *ptr);
>>>>>>> upstream/android-13

#else /* CONFIG_KASAN_GENERIC */

static inline void kasan_cache_shrink(struct kmem_cache *cache) {}
static inline void kasan_cache_shutdown(struct kmem_cache *cache) {}
<<<<<<< HEAD

#endif /* CONFIG_KASAN_GENERIC */

#ifdef CONFIG_KASAN_SW_TAGS

#define KASAN_SHADOW_INIT 0xFF

void kasan_init_tags(void);

void *kasan_reset_tag(const void *addr);

void kasan_report(unsigned long addr, size_t size,
		bool is_write, unsigned long ip);

#else /* CONFIG_KASAN_SW_TAGS */

static inline void kasan_init_tags(void) { }
=======
static inline void kasan_record_aux_stack(void *ptr) {}
static inline void kasan_record_aux_stack_noalloc(void *ptr) {}

#endif /* CONFIG_KASAN_GENERIC */

#if defined(CONFIG_KASAN_SW_TAGS) || defined(CONFIG_KASAN_HW_TAGS)

static inline void *kasan_reset_tag(const void *addr)
{
	return (void *)arch_kasan_reset_tag(addr);
}

/**
 * kasan_report - print a report about a bad memory access detected by KASAN
 * @addr: address of the bad access
 * @size: size of the bad access
 * @is_write: whether the bad access is a write or a read
 * @ip: instruction pointer for the accessibility check or the bad access itself
 */
bool kasan_report(unsigned long addr, size_t size,
		bool is_write, unsigned long ip);

#else /* CONFIG_KASAN_SW_TAGS || CONFIG_KASAN_HW_TAGS */
>>>>>>> upstream/android-13

static inline void *kasan_reset_tag(const void *addr)
{
	return (void *)addr;
}

<<<<<<< HEAD
#endif /* CONFIG_KASAN_SW_TAGS */
=======
#endif /* CONFIG_KASAN_SW_TAGS || CONFIG_KASAN_HW_TAGS*/

#ifdef CONFIG_KASAN_HW_TAGS

void kasan_report_async(void);

#endif /* CONFIG_KASAN_HW_TAGS */

#ifdef CONFIG_KASAN_SW_TAGS
void __init kasan_init_sw_tags(void);
#else
static inline void kasan_init_sw_tags(void) { }
#endif

#ifdef CONFIG_KASAN_HW_TAGS
void kasan_init_hw_tags_cpu(void);
void __init kasan_init_hw_tags(void);
#else
static inline void kasan_init_hw_tags_cpu(void) { }
static inline void kasan_init_hw_tags(void) { }
#endif

#ifdef CONFIG_KASAN_VMALLOC

#if defined(CONFIG_KASAN_GENERIC) || defined(CONFIG_KASAN_SW_TAGS)

void kasan_populate_early_vm_area_shadow(void *start, unsigned long size);
int kasan_populate_vmalloc(unsigned long addr, unsigned long size);
void kasan_release_vmalloc(unsigned long start, unsigned long end,
			   unsigned long free_region_start,
			   unsigned long free_region_end);

#else /* CONFIG_KASAN_GENERIC || CONFIG_KASAN_SW_TAGS */

static inline int kasan_populate_vmalloc(unsigned long start,
					unsigned long size)
{
	return 0;
}
static inline void kasan_release_vmalloc(unsigned long start,
					 unsigned long end,
					 unsigned long free_region_start,
					 unsigned long free_region_end) { }

#endif /* CONFIG_KASAN_GENERIC || CONFIG_KASAN_SW_TAGS */

void *__kasan_unpoison_vmalloc(const void *start, unsigned long size,
			       kasan_vmalloc_flags_t flags);
static __always_inline void *kasan_unpoison_vmalloc(const void *start,
						unsigned long size,
						kasan_vmalloc_flags_t flags)
{
	if (kasan_enabled())
		return __kasan_unpoison_vmalloc(start, size, flags);
	return (void *)start;
}

void __kasan_poison_vmalloc(const void *start, unsigned long size);
static __always_inline void kasan_poison_vmalloc(const void *start,
						 unsigned long size)
{
	if (kasan_enabled())
		__kasan_poison_vmalloc(start, size);
}

#else /* CONFIG_KASAN_VMALLOC */

static inline void kasan_populate_early_vm_area_shadow(void *start,
						       unsigned long size) { }
static inline int kasan_populate_vmalloc(unsigned long start,
					unsigned long size)
{
	return 0;
}
static inline void kasan_release_vmalloc(unsigned long start,
					 unsigned long end,
					 unsigned long free_region_start,
					 unsigned long free_region_end) { }

static inline void *kasan_unpoison_vmalloc(const void *start,
					   unsigned long size,
					   kasan_vmalloc_flags_t flags)
{
	return (void *)start;
}
static inline void kasan_poison_vmalloc(const void *start, unsigned long size)
{ }

#endif /* CONFIG_KASAN_VMALLOC */

#if (defined(CONFIG_KASAN_GENERIC) || defined(CONFIG_KASAN_SW_TAGS)) && \
		!defined(CONFIG_KASAN_VMALLOC)

/*
 * These functions allocate and free shadow memory for kernel modules.
 * They are only required when KASAN_VMALLOC is not supported, as otherwise
 * shadow memory is allocated by the generic vmalloc handlers.
 */
int kasan_alloc_module_shadow(void *addr, size_t size, gfp_t gfp_mask);
void kasan_free_module_shadow(const struct vm_struct *vm);

#else /* (CONFIG_KASAN_GENERIC || CONFIG_KASAN_SW_TAGS) && !CONFIG_KASAN_VMALLOC */

static inline int kasan_alloc_module_shadow(void *addr, size_t size, gfp_t gfp_mask) { return 0; }
static inline void kasan_free_module_shadow(const struct vm_struct *vm) {}

#endif /* (CONFIG_KASAN_GENERIC || CONFIG_KASAN_SW_TAGS) && !CONFIG_KASAN_VMALLOC */

#ifdef CONFIG_KASAN_INLINE
void kasan_non_canonical_hook(unsigned long addr);
#else /* CONFIG_KASAN_INLINE */
static inline void kasan_non_canonical_hook(unsigned long addr) { }
#endif /* CONFIG_KASAN_INLINE */
>>>>>>> upstream/android-13

#endif /* LINUX_KASAN_H */
