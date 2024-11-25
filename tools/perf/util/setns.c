<<<<<<< HEAD
#include "util.h"
=======
// SPDX-License-Identifier: LGPL-2.1

#include "namespaces.h"
>>>>>>> upstream/android-13
#include <unistd.h>
#include <sys/syscall.h>

int setns(int fd, int nstype)
{
	return syscall(__NR_setns, fd, nstype);
}
