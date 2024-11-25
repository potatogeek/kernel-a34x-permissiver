/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 * Xen balloon functionality
 */
<<<<<<< HEAD
=======
#ifndef _XEN_BALLOON_H
#define _XEN_BALLOON_H
>>>>>>> upstream/android-13

#define RETRY_UNLIMITED	0

struct balloon_stats {
	/* We aim for 'current allocation' == 'target allocation'. */
	unsigned long current_pages;
	unsigned long target_pages;
	unsigned long target_unpopulated;
	/* Number of pages in high- and low-memory balloons. */
	unsigned long balloon_low;
	unsigned long balloon_high;
	unsigned long total_pages;
	unsigned long schedule_delay;
	unsigned long max_schedule_delay;
	unsigned long retry_count;
	unsigned long max_retry_count;
};

extern struct balloon_stats balloon_stats;

void balloon_set_new_target(unsigned long target);

int alloc_xenballooned_pages(int nr_pages, struct page **pages);
void free_xenballooned_pages(int nr_pages, struct page **pages);

<<<<<<< HEAD
struct device;
#ifdef CONFIG_XEN_SELFBALLOONING
extern int register_xen_selfballooning(struct device *dev);
#else
static inline int register_xen_selfballooning(struct device *dev)
{
	return -ENOSYS;
}
#endif

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_XEN_BALLOON
void xen_balloon_init(void);
#else
static inline void xen_balloon_init(void)
{
}
#endif
<<<<<<< HEAD
=======

#endif	/* _XEN_BALLOON_H */
>>>>>>> upstream/android-13
