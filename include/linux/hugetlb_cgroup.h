/*
 * Copyright IBM Corporation, 2012
 * Author Aneesh Kumar K.V <aneesh.kumar@linux.vnet.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _LINUX_HUGETLB_CGROUP_H
#define _LINUX_HUGETLB_CGROUP_H

#include <linux/mmdebug.h>

struct hugetlb_cgroup;
<<<<<<< HEAD
/*
 * Minimum page order trackable by hugetlb cgroup.
 * At least 3 pages are necessary for all the tracking information.
 */
#define HUGETLB_CGROUP_MIN_ORDER	2

#ifdef CONFIG_CGROUP_HUGETLB

static inline struct hugetlb_cgroup *hugetlb_cgroup_from_page(struct page *page)
=======
struct resv_map;
struct file_region;

#ifdef CONFIG_CGROUP_HUGETLB
/*
 * Minimum page order trackable by hugetlb cgroup.
 * At least 4 pages are necessary for all the tracking information.
 * The second tail page (hpage[SUBPAGE_INDEX_CGROUP]) is the fault
 * usage cgroup. The third tail page (hpage[SUBPAGE_INDEX_CGROUP_RSVD])
 * is the reservation usage cgroup.
 */
#define HUGETLB_CGROUP_MIN_ORDER order_base_2(__MAX_CGROUP_SUBPAGE_INDEX + 1)

enum hugetlb_memory_event {
	HUGETLB_MAX,
	HUGETLB_NR_MEMORY_EVENTS,
};

struct hugetlb_cgroup {
	struct cgroup_subsys_state css;

	/*
	 * the counter to account for hugepages from hugetlb.
	 */
	struct page_counter hugepage[HUGE_MAX_HSTATE];

	/*
	 * the counter to account for hugepage reservations from hugetlb.
	 */
	struct page_counter rsvd_hugepage[HUGE_MAX_HSTATE];

	atomic_long_t events[HUGE_MAX_HSTATE][HUGETLB_NR_MEMORY_EVENTS];
	atomic_long_t events_local[HUGE_MAX_HSTATE][HUGETLB_NR_MEMORY_EVENTS];

	/* Handle for "hugetlb.events" */
	struct cgroup_file events_file[HUGE_MAX_HSTATE];

	/* Handle for "hugetlb.events.local" */
	struct cgroup_file events_local_file[HUGE_MAX_HSTATE];
};

static inline struct hugetlb_cgroup *
__hugetlb_cgroup_from_page(struct page *page, bool rsvd)
>>>>>>> upstream/android-13
{
	VM_BUG_ON_PAGE(!PageHuge(page), page);

	if (compound_order(page) < HUGETLB_CGROUP_MIN_ORDER)
		return NULL;
<<<<<<< HEAD
	return (struct hugetlb_cgroup *)page[2].private;
}

static inline
int set_hugetlb_cgroup(struct page *page, struct hugetlb_cgroup *h_cg)
=======
	if (rsvd)
		return (void *)page_private(page + SUBPAGE_INDEX_CGROUP_RSVD);
	else
		return (void *)page_private(page + SUBPAGE_INDEX_CGROUP);
}

static inline struct hugetlb_cgroup *hugetlb_cgroup_from_page(struct page *page)
{
	return __hugetlb_cgroup_from_page(page, false);
}

static inline struct hugetlb_cgroup *
hugetlb_cgroup_from_page_rsvd(struct page *page)
{
	return __hugetlb_cgroup_from_page(page, true);
}

static inline int __set_hugetlb_cgroup(struct page *page,
				       struct hugetlb_cgroup *h_cg, bool rsvd)
>>>>>>> upstream/android-13
{
	VM_BUG_ON_PAGE(!PageHuge(page), page);

	if (compound_order(page) < HUGETLB_CGROUP_MIN_ORDER)
		return -1;
<<<<<<< HEAD
	page[2].private	= (unsigned long)h_cg;
	return 0;
}

=======
	if (rsvd)
		set_page_private(page + SUBPAGE_INDEX_CGROUP_RSVD,
				 (unsigned long)h_cg);
	else
		set_page_private(page + SUBPAGE_INDEX_CGROUP,
				 (unsigned long)h_cg);
	return 0;
}

static inline int set_hugetlb_cgroup(struct page *page,
				     struct hugetlb_cgroup *h_cg)
{
	return __set_hugetlb_cgroup(page, h_cg, false);
}

static inline int set_hugetlb_cgroup_rsvd(struct page *page,
					  struct hugetlb_cgroup *h_cg)
{
	return __set_hugetlb_cgroup(page, h_cg, true);
}

>>>>>>> upstream/android-13
static inline bool hugetlb_cgroup_disabled(void)
{
	return !cgroup_subsys_enabled(hugetlb_cgrp_subsys);
}

<<<<<<< HEAD
extern int hugetlb_cgroup_charge_cgroup(int idx, unsigned long nr_pages,
					struct hugetlb_cgroup **ptr);
extern void hugetlb_cgroup_commit_charge(int idx, unsigned long nr_pages,
					 struct hugetlb_cgroup *h_cg,
					 struct page *page);
extern void hugetlb_cgroup_uncharge_page(int idx, unsigned long nr_pages,
					 struct page *page);
extern void hugetlb_cgroup_uncharge_cgroup(int idx, unsigned long nr_pages,
					   struct hugetlb_cgroup *h_cg);
=======
static inline void hugetlb_cgroup_put_rsvd_cgroup(struct hugetlb_cgroup *h_cg)
{
	css_put(&h_cg->css);
}

static inline void resv_map_dup_hugetlb_cgroup_uncharge_info(
						struct resv_map *resv_map)
{
	if (resv_map->css)
		css_get(resv_map->css);
}

extern int hugetlb_cgroup_charge_cgroup(int idx, unsigned long nr_pages,
					struct hugetlb_cgroup **ptr);
extern int hugetlb_cgroup_charge_cgroup_rsvd(int idx, unsigned long nr_pages,
					     struct hugetlb_cgroup **ptr);
extern void hugetlb_cgroup_commit_charge(int idx, unsigned long nr_pages,
					 struct hugetlb_cgroup *h_cg,
					 struct page *page);
extern void hugetlb_cgroup_commit_charge_rsvd(int idx, unsigned long nr_pages,
					      struct hugetlb_cgroup *h_cg,
					      struct page *page);
extern void hugetlb_cgroup_uncharge_page(int idx, unsigned long nr_pages,
					 struct page *page);
extern void hugetlb_cgroup_uncharge_page_rsvd(int idx, unsigned long nr_pages,
					      struct page *page);

extern void hugetlb_cgroup_uncharge_cgroup(int idx, unsigned long nr_pages,
					   struct hugetlb_cgroup *h_cg);
extern void hugetlb_cgroup_uncharge_cgroup_rsvd(int idx, unsigned long nr_pages,
						struct hugetlb_cgroup *h_cg);
extern void hugetlb_cgroup_uncharge_counter(struct resv_map *resv,
					    unsigned long start,
					    unsigned long end);

extern void hugetlb_cgroup_uncharge_file_region(struct resv_map *resv,
						struct file_region *rg,
						unsigned long nr_pages,
						bool region_del);

>>>>>>> upstream/android-13
extern void hugetlb_cgroup_file_init(void) __init;
extern void hugetlb_cgroup_migrate(struct page *oldhpage,
				   struct page *newhpage);

#else
<<<<<<< HEAD
=======
static inline void hugetlb_cgroup_uncharge_file_region(struct resv_map *resv,
						       struct file_region *rg,
						       unsigned long nr_pages,
						       bool region_del)
{
}

>>>>>>> upstream/android-13
static inline struct hugetlb_cgroup *hugetlb_cgroup_from_page(struct page *page)
{
	return NULL;
}

<<<<<<< HEAD
static inline
int set_hugetlb_cgroup(struct page *page, struct hugetlb_cgroup *h_cg)
=======
static inline struct hugetlb_cgroup *
hugetlb_cgroup_from_page_resv(struct page *page)
{
	return NULL;
}

static inline struct hugetlb_cgroup *
hugetlb_cgroup_from_page_rsvd(struct page *page)
{
	return NULL;
}

static inline int set_hugetlb_cgroup(struct page *page,
				     struct hugetlb_cgroup *h_cg)
{
	return 0;
}

static inline int set_hugetlb_cgroup_rsvd(struct page *page,
					  struct hugetlb_cgroup *h_cg)
>>>>>>> upstream/android-13
{
	return 0;
}

static inline bool hugetlb_cgroup_disabled(void)
{
	return true;
}

<<<<<<< HEAD
static inline int
hugetlb_cgroup_charge_cgroup(int idx, unsigned long nr_pages,
			     struct hugetlb_cgroup **ptr)
=======
static inline void hugetlb_cgroup_put_rsvd_cgroup(struct hugetlb_cgroup *h_cg)
{
}

static inline void resv_map_dup_hugetlb_cgroup_uncharge_info(
						struct resv_map *resv_map)
{
}

static inline int hugetlb_cgroup_charge_cgroup(int idx, unsigned long nr_pages,
					       struct hugetlb_cgroup **ptr)
>>>>>>> upstream/android-13
{
	return 0;
}

<<<<<<< HEAD
static inline void
hugetlb_cgroup_commit_charge(int idx, unsigned long nr_pages,
			     struct hugetlb_cgroup *h_cg,
			     struct page *page)
=======
static inline int hugetlb_cgroup_charge_cgroup_rsvd(int idx,
						    unsigned long nr_pages,
						    struct hugetlb_cgroup **ptr)
{
	return 0;
}

static inline void hugetlb_cgroup_commit_charge(int idx, unsigned long nr_pages,
						struct hugetlb_cgroup *h_cg,
						struct page *page)
>>>>>>> upstream/android-13
{
}

static inline void
<<<<<<< HEAD
hugetlb_cgroup_uncharge_page(int idx, unsigned long nr_pages, struct page *page)
=======
hugetlb_cgroup_commit_charge_rsvd(int idx, unsigned long nr_pages,
				  struct hugetlb_cgroup *h_cg,
				  struct page *page)
{
}

static inline void hugetlb_cgroup_uncharge_page(int idx, unsigned long nr_pages,
						struct page *page)
{
}

static inline void hugetlb_cgroup_uncharge_page_rsvd(int idx,
						     unsigned long nr_pages,
						     struct page *page)
{
}
static inline void hugetlb_cgroup_uncharge_cgroup(int idx,
						  unsigned long nr_pages,
						  struct hugetlb_cgroup *h_cg)
>>>>>>> upstream/android-13
{
}

static inline void
<<<<<<< HEAD
hugetlb_cgroup_uncharge_cgroup(int idx, unsigned long nr_pages,
			       struct hugetlb_cgroup *h_cg)
=======
hugetlb_cgroup_uncharge_cgroup_rsvd(int idx, unsigned long nr_pages,
				    struct hugetlb_cgroup *h_cg)
{
}

static inline void hugetlb_cgroup_uncharge_counter(struct resv_map *resv,
						   unsigned long start,
						   unsigned long end)
>>>>>>> upstream/android-13
{
}

static inline void hugetlb_cgroup_file_init(void)
{
}

static inline void hugetlb_cgroup_migrate(struct page *oldhpage,
					  struct page *newhpage)
{
}

#endif  /* CONFIG_MEM_RES_CTLR_HUGETLB */
#endif
