/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef __ASM_SH_SOCKIOS_H
#define __ASM_SH_SOCKIOS_H

<<<<<<< HEAD
=======
#include <linux/time_types.h>

>>>>>>> upstream/android-13
/* Socket-level I/O control calls. */
#define FIOGETOWN	_IOR('f', 123, int)
#define FIOSETOWN 	_IOW('f', 124, int)

#define SIOCATMARK	_IOR('s', 7, int)
#define SIOCSPGRP	_IOW('s', 8, pid_t)
#define SIOCGPGRP	_IOR('s', 9, pid_t)

<<<<<<< HEAD
#define SIOCGSTAMP	_IOR('s', 100, struct timeval) /* Get stamp (timeval) */
#define SIOCGSTAMPNS	_IOR('s', 101, struct timespec) /* Get stamp (timespec) */
=======
#define SIOCGSTAMP_OLD	_IOR('s', 100, struct __kernel_old_timeval) /* Get stamp (timeval) */
#define SIOCGSTAMPNS_OLD _IOR('s', 101, struct __kernel_old_timespec) /* Get stamp (timespec) */

>>>>>>> upstream/android-13
#endif /* __ASM_SH_SOCKIOS_H */
