/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_TRAPS_H
#define __ASM_SH_TRAPS_H

#include <linux/compiler.h>

<<<<<<< HEAD
#ifdef CONFIG_SUPERH32
# include <asm/traps_32.h>
#else
# include <asm/traps_64.h>
#endif
=======
# include <asm/traps_32.h>
>>>>>>> upstream/android-13

BUILD_TRAP_HANDLER(address_error);
BUILD_TRAP_HANDLER(debug);
BUILD_TRAP_HANDLER(bug);
BUILD_TRAP_HANDLER(breakpoint);
BUILD_TRAP_HANDLER(singlestep);
BUILD_TRAP_HANDLER(fpu_error);
BUILD_TRAP_HANDLER(fpu_state_restore);
BUILD_TRAP_HANDLER(nmi);

#endif /* __ASM_SH_TRAPS_H */
