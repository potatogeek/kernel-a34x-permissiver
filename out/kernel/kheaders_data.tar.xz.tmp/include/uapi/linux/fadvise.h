/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef FADVISE_H_INCLUDED
#define FADVISE_H_INCLUDED

#define POSIX_FADV_NORMAL	0 
#define POSIX_FADV_RANDOM	1 
#define POSIX_FADV_SEQUENTIAL	2 
#define POSIX_FADV_WILLNEED	3 


#if defined(__s390x__)
#define POSIX_FADV_DONTNEED	6 
#define POSIX_FADV_NOREUSE	7 
#else
#define POSIX_FADV_DONTNEED	4 
#define POSIX_FADV_NOREUSE	5 
#endif

#endif	
