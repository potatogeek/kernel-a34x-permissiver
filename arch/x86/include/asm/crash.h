/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CRASH_H
#define _ASM_X86_CRASH_H

struct kimage;

int crash_load_segments(struct kimage *image);
<<<<<<< HEAD
int crash_copy_backup_region(struct kimage *image);
int crash_setup_memmap_entries(struct kimage *image,
		struct boot_params *params);
=======
int crash_setup_memmap_entries(struct kimage *image,
		struct boot_params *params);
void crash_smp_send_stop(void);
>>>>>>> upstream/android-13

#endif /* _ASM_X86_CRASH_H */
