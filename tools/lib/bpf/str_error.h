<<<<<<< HEAD
// SPDX-License-Identifier: LGPL-2.1
#ifndef BPF_STR_ERROR
#define BPF_STR_ERROR

char *str_error(int err, char *dst, int len);
#endif // BPF_STR_ERROR
=======
/* SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause) */
#ifndef __LIBBPF_STR_ERROR_H
#define __LIBBPF_STR_ERROR_H

char *libbpf_strerror_r(int err, char *dst, int len);
#endif /* __LIBBPF_STR_ERROR_H */
>>>>>>> upstream/android-13
