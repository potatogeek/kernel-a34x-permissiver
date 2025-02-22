/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_NVRAM_H
#define _LINUX_NVRAM_H

#include <uapi/linux/nvram.h>


extern unsigned char __nvram_read_byte(int i);
extern unsigned char nvram_read_byte(int i);
extern void __nvram_write_byte(unsigned char c, int i);
extern void nvram_write_byte(unsigned char c, int i);
extern int __nvram_check_checksum(void);
extern int nvram_check_checksum(void);
#endif  
