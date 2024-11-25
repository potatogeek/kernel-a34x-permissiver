/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_UM_IO_H
#define _ASM_UM_IO_H
<<<<<<< HEAD

#define ioremap ioremap
static inline void __iomem *ioremap(phys_addr_t offset, size_t size)
{
	return (void __iomem *)(unsigned long)offset;
}

=======
#include <linux/types.h>

/* get emulated iomem (if desired) */
#include <asm-generic/logic_io.h>

#ifndef ioremap
#define ioremap ioremap
static inline void __iomem *ioremap(phys_addr_t offset, size_t size)
{
	return NULL;
}
#endif /* ioremap */

#ifndef iounmap
>>>>>>> upstream/android-13
#define iounmap iounmap
static inline void iounmap(void __iomem *addr)
{
}
<<<<<<< HEAD
=======
#endif /* iounmap */
>>>>>>> upstream/android-13

#include <asm-generic/io.h>

#endif
