/* SPDX-License-Identifier: GPL-2.0 */
#ifndef BUG_H
#define BUG_H

<<<<<<< HEAD
=======
#include <asm/bug.h>

>>>>>>> upstream/android-13
#define BUG_ON(__BUG_ON_cond) assert(!(__BUG_ON_cond))

#define BUILD_BUG_ON(x)

#define BUG() abort()

#endif /* BUG_H */
