// SPDX-License-Identifier: GPL-2.0
#include <liblockdep/mutex.h>
#include "common.h"

void main(void)
{
	pthread_mutex_t a, b, c;

	pthread_mutex_init(&a, NULL);
	pthread_mutex_init(&b, NULL);
	pthread_mutex_init(&c, NULL);

	LOCK_UNLOCK_2(a, b);
	LOCK_UNLOCK_2(c, a);
	LOCK_UNLOCK_2(b, c);
<<<<<<< HEAD
=======

	pthread_mutex_destroy(&c);
	pthread_mutex_destroy(&b);
	pthread_mutex_destroy(&a);
>>>>>>> upstream/android-13
}
