/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
=======
#define  _HAVE_ARCH_COPY_AND_CSUM_FROM_USER 1
#define HAVE_CSUM_COPY_USER
#define _HAVE_ARCH_CSUM_AND_COPY
>>>>>>> upstream/android-13
#ifdef CONFIG_X86_32
# include <asm/checksum_32.h>
#else
# include <asm/checksum_64.h>
#endif
