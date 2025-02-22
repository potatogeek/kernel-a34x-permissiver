/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SWAPFILE_H
#define _LINUX_SWAPFILE_H


extern spinlock_t swap_lock;
extern struct plist_head swap_active_head;
extern struct swap_info_struct *swap_info[];
extern int try_to_unuse(unsigned int, bool, unsigned long);
extern unsigned long generic_max_swapfile_size(void);
extern unsigned long max_swapfile_size(void);

#endif 
