

#ifndef _LINUX_CRED_H
#define _LINUX_CRED_H

#include <linux/capability.h>
#include <linux/init.h>
#include <linux/key.h>
#include <linux/selinux.h>
#include <linux/atomic.h>
#include <linux/uidgid.h>
#include <linux/sched.h>
#include <linux/sched/user.h>

#ifdef CONFIG_KDP
#include <linux/kdp.h>
#endif

struct cred;
struct inode;


struct group_info {
	atomic_t	usage;
	int		ngroups;
	kgid_t		gid[0];
} __randomize_layout;


static inline struct group_info *get_group_info(struct group_info *gi)
{
	atomic_inc(&gi->usage);
	return gi;
}


#define put_group_info(group_info)			\
do {							\
	if (atomic_dec_and_test(&(group_info)->usage))	\
		groups_free(group_info);		\
} while (0)

extern struct group_info init_groups;
#ifdef CONFIG_MULTIUSER
extern struct group_info *groups_alloc(int);
extern void groups_free(struct group_info *);

extern int in_group_p(kgid_t);
extern int in_egroup_p(kgid_t);
extern int groups_search(const struct group_info *, kgid_t);

extern int set_current_groups(struct group_info *);
extern void set_groups(struct cred *, struct group_info *);
extern bool may_setgroups(void);
extern void groups_sort(struct group_info *);
#else
static inline void groups_free(struct group_info *group_info)
{
}

static inline int in_group_p(kgid_t grp)
{
        return 1;
}
static inline int in_egroup_p(kgid_t grp)
{
        return 1;
}
static inline int groups_search(const struct group_info *group_info, kgid_t grp)
{
	return 1;
}
#endif


struct cred {
	atomic_t	usage;
#ifdef CONFIG_DEBUG_CREDENTIALS
	atomic_t	subscribers;	
	void		*put_addr;
	unsigned	magic;
#define CRED_MAGIC	0x43736564
#define CRED_MAGIC_DEAD	0x44656144
#endif
	kuid_t		uid;		
	kgid_t		gid;		
	kuid_t		suid;		
	kgid_t		sgid;		
	kuid_t		euid;		
	kgid_t		egid;		
	kuid_t		fsuid;		
	kgid_t		fsgid;		
	unsigned	securebits;	
	kernel_cap_t	cap_inheritable; 
	kernel_cap_t	cap_permitted;	
	kernel_cap_t	cap_effective;	
	kernel_cap_t	cap_bset;	
	kernel_cap_t	cap_ambient;	
#ifdef CONFIG_KEYS
	unsigned char	jit_keyring;	
	struct key __rcu *session_keyring; 
	struct key	*process_keyring; 
	struct key	*thread_keyring; 
	struct key	*request_key_auth; 
#endif
#ifdef CONFIG_SECURITY
	void		*security;	
#endif
	struct user_struct *user;	
	struct user_namespace *user_ns; 
	struct group_info *group_info;	
	
	union {
		int non_rcu;			
		struct rcu_head	rcu;		
	};
#ifdef CONFIG_KDP
	atomic_t *use_cnt;
	struct task_struct *bp_task;
	void *bp_pgd;
	unsigned long long type;
#endif
} __randomize_layout;

extern void __put_cred(struct cred *);
extern void exit_creds(struct task_struct *);
extern int copy_creds(struct task_struct *, unsigned long);
extern const struct cred *get_task_cred(struct task_struct *);
extern struct cred *cred_alloc_blank(void);
extern struct cred *prepare_creds(void);
extern struct cred *prepare_exec_creds(void);
extern int commit_creds(struct cred *);
extern void abort_creds(struct cred *);
extern const struct cred *override_creds(const struct cred *);
extern void revert_creds(const struct cred *);
extern struct cred *prepare_kernel_cred(struct task_struct *);
extern int change_create_files_as(struct cred *, struct inode *);
extern int set_security_override(struct cred *, u32);
extern int set_security_override_from_ctx(struct cred *, const char *);
extern int set_create_files_as(struct cred *, struct inode *);
extern void __init cred_init(void);


#ifdef CONFIG_DEBUG_CREDENTIALS
extern void __invalid_creds(const struct cred *, const char *, unsigned);
extern void __validate_process_creds(struct task_struct *,
				     const char *, unsigned);

extern bool creds_are_invalid(const struct cred *cred);

static inline void __validate_creds(const struct cred *cred,
				    const char *file, unsigned line)
{
	if (unlikely(creds_are_invalid(cred)))
		__invalid_creds(cred, file, line);
}

#define validate_creds(cred)				\
do {							\
	__validate_creds((cred), __FILE__, __LINE__);	\
} while(0)

#define validate_process_creds()				\
do {								\
	__validate_process_creds(current, __FILE__, __LINE__);	\
} while(0)

extern void validate_creds_for_do_exit(struct task_struct *);
#else
static inline void validate_creds(const struct cred *cred)
{
}
static inline void validate_creds_for_do_exit(struct task_struct *tsk)
{
}
static inline void validate_process_creds(void)
{
}
#endif

static inline bool cap_ambient_invariant_ok(const struct cred *cred)
{
	return cap_issubset(cred->cap_ambient,
			    cap_intersect(cred->cap_permitted,
					  cred->cap_inheritable));
}


#ifndef CONFIG_KDP_CRED
static inline struct cred *get_new_cred(struct cred *cred)
{
	atomic_inc(&cred->usage);
	return cred;
}
#endif


static inline const struct cred *get_cred(const struct cred *cred)
{
	struct cred *nonconst_cred = (struct cred *) cred;
	validate_creds(cred);
#ifdef CONFIG_KDP_CRED
	if (is_kdp_protect_addr((unsigned long)nonconst_cred))
		GET_ROCRED_RCU(nonconst_cred)->non_rcu = 0;
	else
#endif
	nonconst_cred->non_rcu = 0;
	return get_new_cred(nonconst_cred);
}


#ifndef CONFIG_KDP_CRED
static inline void put_cred(const struct cred *_cred)
{
	struct cred *cred = (struct cred *) _cred;

	validate_creds(cred);
	if (atomic_dec_and_test(&(cred)->usage))
		__put_cred(cred);
}
#endif


#define current_cred() \
	rcu_dereference_protected(current->cred, 1)


#define current_real_cred() \
	rcu_dereference_protected(current->real_cred, 1)


#define __task_cred(task)	\
	rcu_dereference((task)->real_cred)


#define get_current_cred()				\
	(get_cred(current_cred()))


#define get_current_user()				\
({							\
	struct user_struct *__u;			\
	const struct cred *__cred;			\
	__cred = current_cred();			\
	__u = get_uid(__cred->user);			\
	__u;						\
})


#define get_current_groups()				\
({							\
	struct group_info *__groups;			\
	const struct cred *__cred;			\
	__cred = current_cred();			\
	__groups = get_group_info(__cred->group_info);	\
	__groups;					\
})

#define task_cred_xxx(task, xxx)			\
({							\
	__typeof__(((struct cred *)NULL)->xxx) ___val;	\
	rcu_read_lock();				\
	___val = __task_cred((task))->xxx;		\
	rcu_read_unlock();				\
	___val;						\
})

#define task_uid(task)		(task_cred_xxx((task), uid))
#define task_euid(task)		(task_cred_xxx((task), euid))

#define current_cred_xxx(xxx)			\
({						\
	current_cred()->xxx;			\
})

#define current_uid()		(current_cred_xxx(uid))
#define current_gid()		(current_cred_xxx(gid))
#define current_euid()		(current_cred_xxx(euid))
#define current_egid()		(current_cred_xxx(egid))
#define current_suid()		(current_cred_xxx(suid))
#define current_sgid()		(current_cred_xxx(sgid))
#define current_fsuid() 	(current_cred_xxx(fsuid))
#define current_fsgid() 	(current_cred_xxx(fsgid))
#define current_cap()		(current_cred_xxx(cap_effective))
#define current_user()		(current_cred_xxx(user))
#define current_security()	(current_cred_xxx(security))

extern struct user_namespace init_user_ns;
#ifdef CONFIG_USER_NS
#define current_user_ns()	(current_cred_xxx(user_ns))
#else
static inline struct user_namespace *current_user_ns(void)
{
	return &init_user_ns;
}
#endif


#define current_uid_gid(_uid, _gid)		\
do {						\
	const struct cred *__cred;		\
	__cred = current_cred();		\
	*(_uid) = __cred->uid;			\
	*(_gid) = __cred->gid;			\
} while(0)

#define current_euid_egid(_euid, _egid)		\
do {						\
	const struct cred *__cred;		\
	__cred = current_cred();		\
	*(_euid) = __cred->euid;		\
	*(_egid) = __cred->egid;		\
} while(0)

#define current_fsuid_fsgid(_fsuid, _fsgid)	\
do {						\
	const struct cred *__cred;		\
	__cred = current_cred();		\
	*(_fsuid) = __cred->fsuid;		\
	*(_fsgid) = __cred->fsgid;		\
} while(0)

#endif 
