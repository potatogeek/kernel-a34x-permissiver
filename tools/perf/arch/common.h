/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ARCH_PERF_COMMON_H
#define ARCH_PERF_COMMON_H

<<<<<<< HEAD
#include "../util/env.h"
=======
#include <stdbool.h>

struct perf_env;
>>>>>>> upstream/android-13

int perf_env__lookup_objdump(struct perf_env *env, const char **path);
bool perf_env__single_address_space(struct perf_env *env);

#endif /* ARCH_PERF_COMMON_H */
