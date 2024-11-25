/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_SOCKET_H
#define _ASM_SOCKET_H

#include <uapi/asm/socket.h>

<<<<<<< HEAD
/* O_NONBLOCK clashes with the bits used for socket types.  Therefore we
 * have to define SOCK_NONBLOCK to a different value here.
=======
/* O_NONBLOCK clashed with the bits used for socket types.  Therefore we
 * had to define SOCK_NONBLOCK to a different value here.
>>>>>>> upstream/android-13
 */
#define SOCK_NONBLOCK	0x40000000

#endif /* _ASM_SOCKET_H */
