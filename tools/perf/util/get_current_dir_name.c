<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2018, Red Hat Inc, Arnaldo Carvalho de Melo <acme@redhat.com>
//
#ifndef HAVE_GET_CURRENT_DIR_NAME
#include "util.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
=======
// SPDX-License-Identifier: LGPL-2.1
// Copyright (C) 2018, 2019 Red Hat Inc, Arnaldo Carvalho de Melo <acme@redhat.com>
//
#ifndef HAVE_GET_CURRENT_DIR_NAME
#include "get_current_dir_name.h"
#include <limits.h>
#include <string.h>
#include <unistd.h>
>>>>>>> upstream/android-13

/* Android's 'bionic' library, for one, doesn't have this */

char *get_current_dir_name(void)
{
	char pwd[PATH_MAX];

	return getcwd(pwd, sizeof(pwd)) == NULL ? NULL : strdup(pwd);
}
#endif // HAVE_GET_CURRENT_DIR_NAME
