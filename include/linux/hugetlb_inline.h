/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_HUGETLB_INLINE_H
#define _LINUX_HUGETLB_INLINE_H

#ifdef CONFIG_HUGETLB_PAGE

#include <linux/mm.h>

static inline bool is_vm_hugetlb_page(struct vm_area_struct *vma)
{
<<<<<<< HEAD
	return !!(READ_ONCE(vma->vm_flags) & VM_HUGETLB);
=======
	return !!(vma->vm_flags & VM_HUGETLB);
>>>>>>> upstream/android-13
}

#else

static inline bool is_vm_hugetlb_page(struct vm_area_struct *vma)
{
	return false;
}

#endif

#endif
