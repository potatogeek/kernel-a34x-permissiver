// SPDX-License-Identifier: GPL-2.0
#define _GNU_SOURCE
#include <stdlib.h>

int main(void)
{
	return !!reallocarray(NULL, 1, 1);
}
<<<<<<< HEAD
=======

#undef _GNU_SOURCE
>>>>>>> upstream/android-13
