/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_PAGE_OWNER_H
#define __LINUX_PAGE_OWNER_H

#include <linux/jump_label.h>

#ifdef CONFIG_PAGE_OWNER
extern struct static_key_false page_owner_inited;
extern struct page_ext_operations page_owner_ops;

<<<<<<< HEAD
extern void __reset_page_owner(struct page *page, unsigned int order);
extern void __set_page_owner(struct page *page,
			unsigned int order, gfp_t gfp_mask);
extern void __split_page_owner(struct page *page, unsigned int order);
extern void __copy_page_owner(struct page *oldpage, struct page *newpage);
extern void __set_page_owner_migrate_reason(struct page *page, int reason);
extern void __dump_page_owner(struct page *page);
=======
extern depot_stack_handle_t get_page_owner_handle(struct page_ext *page_ext,
					unsigned long pfn);
extern void __reset_page_owner(struct page *page, unsigned int order);
extern void __set_page_owner(struct page *page,
			unsigned int order, gfp_t gfp_mask);
extern void __split_page_owner(struct page *page, unsigned int nr);
extern void __copy_page_owner(struct page *oldpage, struct page *newpage);
extern void __set_page_owner_migrate_reason(struct page *page, int reason);
extern void __dump_page_owner(const struct page *page);
>>>>>>> upstream/android-13
extern void pagetypeinfo_showmixedcount_print(struct seq_file *m,
					pg_data_t *pgdat, struct zone *zone);

static inline void reset_page_owner(struct page *page, unsigned int order)
{
	if (static_branch_unlikely(&page_owner_inited))
		__reset_page_owner(page, order);
}

static inline void set_page_owner(struct page *page,
			unsigned int order, gfp_t gfp_mask)
{
	if (static_branch_unlikely(&page_owner_inited))
		__set_page_owner(page, order, gfp_mask);
}

<<<<<<< HEAD
static inline void split_page_owner(struct page *page, unsigned int order)
{
	if (static_branch_unlikely(&page_owner_inited))
		__split_page_owner(page, order);
=======
static inline void split_page_owner(struct page *page, unsigned int nr)
{
	if (static_branch_unlikely(&page_owner_inited))
		__split_page_owner(page, nr);
>>>>>>> upstream/android-13
}
static inline void copy_page_owner(struct page *oldpage, struct page *newpage)
{
	if (static_branch_unlikely(&page_owner_inited))
		__copy_page_owner(oldpage, newpage);
}
static inline void set_page_owner_migrate_reason(struct page *page, int reason)
{
	if (static_branch_unlikely(&page_owner_inited))
		__set_page_owner_migrate_reason(page, reason);
}
<<<<<<< HEAD
static inline void dump_page_owner(struct page *page)
=======
static inline void dump_page_owner(const struct page *page)
>>>>>>> upstream/android-13
{
	if (static_branch_unlikely(&page_owner_inited))
		__dump_page_owner(page);
}
#else
static inline void reset_page_owner(struct page *page, unsigned int order)
{
}
static inline void set_page_owner(struct page *page,
			unsigned int order, gfp_t gfp_mask)
{
}
static inline void split_page_owner(struct page *page,
			unsigned int order)
{
}
static inline void copy_page_owner(struct page *oldpage, struct page *newpage)
{
}
static inline void set_page_owner_migrate_reason(struct page *page, int reason)
{
}
<<<<<<< HEAD
static inline void dump_page_owner(struct page *page)
=======
static inline void dump_page_owner(const struct page *page)
>>>>>>> upstream/android-13
{
}
#endif /* CONFIG_PAGE_OWNER */
#endif /* __LINUX_PAGE_OWNER_H */
