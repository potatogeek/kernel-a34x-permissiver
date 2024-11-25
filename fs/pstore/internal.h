/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PSTORE_INTERNAL_H__
#define __PSTORE_INTERNAL_H__

#include <linux/types.h>
#include <linux/time.h>
#include <linux/pstore.h>

<<<<<<< HEAD
#define PSTORE_DEFAULT_KMSG_BYTES 10240
=======
>>>>>>> upstream/android-13
extern unsigned long kmsg_bytes;

#ifdef CONFIG_PSTORE_FTRACE
extern void pstore_register_ftrace(void);
extern void pstore_unregister_ftrace(void);
<<<<<<< HEAD
#else
static inline void pstore_register_ftrace(void) {}
static inline void pstore_unregister_ftrace(void) {}
=======
ssize_t pstore_ftrace_combine_log(char **dest_log, size_t *dest_log_size,
				  const char *src_log, size_t src_log_size);
#else
static inline void pstore_register_ftrace(void) {}
static inline void pstore_unregister_ftrace(void) {}
static inline ssize_t
pstore_ftrace_combine_log(char **dest_log, size_t *dest_log_size,
			  const char *src_log, size_t src_log_size)
{
	*dest_log_size = 0;
	return 0;
}
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_PSTORE_PMSG
extern void pstore_register_pmsg(void);
extern void pstore_unregister_pmsg(void);
#else
static inline void pstore_register_pmsg(void) {}
static inline void pstore_unregister_pmsg(void) {}
#endif

extern struct pstore_info *psinfo;

extern void	pstore_set_kmsg_bytes(int);
extern void	pstore_get_records(int);
extern void	pstore_get_backend_records(struct pstore_info *psi,
					   struct dentry *root, int quiet);
<<<<<<< HEAD
extern int	pstore_mkfile(struct dentry *root,
			      struct pstore_record *record);
extern bool	pstore_is_mounted(void);
=======
extern int	pstore_put_backend_records(struct pstore_info *psi);
extern int	pstore_mkfile(struct dentry *root,
			      struct pstore_record *record);
>>>>>>> upstream/android-13
extern void	pstore_record_init(struct pstore_record *record,
				   struct pstore_info *psi);

/* Called during pstore init/exit. */
int __init	pstore_init_fs(void);
void __exit	pstore_exit_fs(void);

#endif
