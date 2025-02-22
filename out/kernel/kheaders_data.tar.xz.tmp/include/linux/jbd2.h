

#ifndef _LINUX_JBD2_H
#define _LINUX_JBD2_H


#ifndef __KERNEL__
#include "jfs_compat.h"
#define JBD2_DEBUG
#else

#include <linux/types.h>
#include <linux/buffer_head.h>
#include <linux/journal-head.h>
#include <linux/stddef.h>
#include <linux/mutex.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/bit_spinlock.h>
#include <crypto/hash.h>
#endif

#define journal_oom_retry 1


#undef JBD2_PARANOID_IOFAIL


#define JBD2_DEFAULT_MAX_COMMIT_AGE 5

#ifdef CONFIG_JBD2_DEBUG

#define JBD2_EXPENSIVE_CHECKING
extern ushort jbd2_journal_enable_debug;
void __jbd2_debug(int level, const char *file, const char *func,
		  unsigned int line, const char *fmt, ...);

#define jbd_debug(n, fmt, a...) \
	__jbd2_debug((n), __FILE__, __func__, __LINE__, (fmt), ##a)
#else
#define jbd_debug(n, fmt, a...)    
#endif

extern void *jbd2_alloc(size_t size, gfp_t flags);
extern void jbd2_free(void *ptr, size_t size);

#define JBD2_MIN_JOURNAL_BLOCKS 1024

#ifdef __KERNEL__


typedef struct jbd2_journal_handle handle_t;	



typedef struct journal_s	journal_t;	
#endif



#define JBD2_MAGIC_NUMBER 0xc03b3998U 





#define JBD2_DESCRIPTOR_BLOCK	1
#define JBD2_COMMIT_BLOCK	2
#define JBD2_SUPERBLOCK_V1	3
#define JBD2_SUPERBLOCK_V2	4
#define JBD2_REVOKE_BLOCK	5


typedef struct journal_header_s
{
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
} journal_header_t;


#define JBD2_CRC32_CHKSUM   1
#define JBD2_MD5_CHKSUM     2
#define JBD2_SHA1_CHKSUM    3
#define JBD2_CRC32C_CHKSUM  4

#define JBD2_CRC32_CHKSUM_SIZE 4

#define JBD2_CHECKSUM_BYTES (32 / sizeof(u32))

struct commit_header {
	__be32		h_magic;
	__be32          h_blocktype;
	__be32          h_sequence;
	unsigned char   h_chksum_type;
	unsigned char   h_chksum_size;
	unsigned char 	h_padding[2];
	__be32 		h_chksum[JBD2_CHECKSUM_BYTES];
	__be64		h_commit_sec;
	__be32		h_commit_nsec;
};


typedef struct journal_block_tag3_s
{
	__be32		t_blocknr;	
	__be32		t_flags;	
	__be32		t_blocknr_high; 
	__be32		t_checksum;	
} journal_block_tag3_t;

typedef struct journal_block_tag_s
{
	__be32		t_blocknr;	
	__be16		t_checksum;	
	__be16		t_flags;	
	__be32		t_blocknr_high; 
} journal_block_tag_t;


struct jbd2_journal_block_tail {
	__be32		t_checksum;	
};


typedef struct jbd2_journal_revoke_header_s
{
	journal_header_t r_header;
	__be32		 r_count;	
} jbd2_journal_revoke_header_t;


#define JBD2_FLAG_ESCAPE		1	
#define JBD2_FLAG_SAME_UUID	2	
#define JBD2_FLAG_DELETED	4	
#define JBD2_FLAG_LAST_TAG	8	



typedef struct journal_superblock_s
{

	journal_header_t s_header;


	
	__be32	s_blocksize;		
	__be32	s_maxlen;		
	__be32	s_first;		


	
	__be32	s_sequence;		
	__be32	s_start;		


	
	__be32	s_errno;


	
	__be32	s_feature_compat;	
	__be32	s_feature_incompat;	
	__be32	s_feature_ro_compat;	

	__u8	s_uuid[16];		


	__be32	s_nr_users;		

	__be32	s_dynsuper;		


	__be32	s_max_transaction;	
	__be32	s_max_trans_data;	


	__u8	s_checksum_type;	
	__u8	s_padding2[3];
	__u32	s_padding[42];
	__be32	s_checksum;		


	__u8	s_users[16*48];		

} journal_superblock_t;


#define JBD2_HAS_COMPAT_FEATURE(j,mask)					\
	((j)->j_format_version >= 2 &&					\
	 ((j)->j_superblock->s_feature_compat & cpu_to_be32((mask))))
#define JBD2_HAS_RO_COMPAT_FEATURE(j,mask)				\
	((j)->j_format_version >= 2 &&					\
	 ((j)->j_superblock->s_feature_ro_compat & cpu_to_be32((mask))))
#define JBD2_HAS_INCOMPAT_FEATURE(j,mask)				\
	((j)->j_format_version >= 2 &&					\
	 ((j)->j_superblock->s_feature_incompat & cpu_to_be32((mask))))

#define JBD2_FEATURE_COMPAT_CHECKSUM		0x00000001

#define JBD2_FEATURE_INCOMPAT_REVOKE		0x00000001
#define JBD2_FEATURE_INCOMPAT_64BIT		0x00000002
#define JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT	0x00000004
#define JBD2_FEATURE_INCOMPAT_CSUM_V2		0x00000008
#define JBD2_FEATURE_INCOMPAT_CSUM_V3		0x00000010




#define JBD2_KNOWN_COMPAT_FEATURES	JBD2_FEATURE_COMPAT_CHECKSUM
#define JBD2_KNOWN_ROCOMPAT_FEATURES	0
#define JBD2_KNOWN_INCOMPAT_FEATURES	(JBD2_FEATURE_INCOMPAT_REVOKE | \
					JBD2_FEATURE_INCOMPAT_64BIT | \
					JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT | \
					JBD2_FEATURE_INCOMPAT_CSUM_V2 | \
					JBD2_FEATURE_INCOMPAT_CSUM_V3)

#ifdef __KERNEL__

#include <linux/fs.h>
#include <linux/sched.h>

enum jbd_state_bits {
	BH_JBD			
	  = BH_PrivateStart,
	BH_JWrite,		
	BH_Freed,		
	BH_Revoked,		
	BH_RevokeValid,		
	BH_JBDDirty,		
	BH_State,		
	BH_JournalHead,		
	BH_Shadow,		
	BH_Verified,		
	BH_JBDPrivateStart,	
};

BUFFER_FNS(JBD, jbd)
BUFFER_FNS(JWrite, jwrite)
BUFFER_FNS(JBDDirty, jbddirty)
TAS_BUFFER_FNS(JBDDirty, jbddirty)
BUFFER_FNS(Revoked, revoked)
TAS_BUFFER_FNS(Revoked, revoked)
BUFFER_FNS(RevokeValid, revokevalid)
TAS_BUFFER_FNS(RevokeValid, revokevalid)
BUFFER_FNS(Freed, freed)
BUFFER_FNS(Shadow, shadow)
BUFFER_FNS(Verified, verified)

static inline struct buffer_head *jh2bh(struct journal_head *jh)
{
	return jh->b_bh;
}

static inline struct journal_head *bh2jh(struct buffer_head *bh)
{
	return bh->b_private;
}

static inline void jbd_lock_bh_state(struct buffer_head *bh)
{
	bit_spin_lock(BH_State, &bh->b_state);
}

static inline int jbd_trylock_bh_state(struct buffer_head *bh)
{
	return bit_spin_trylock(BH_State, &bh->b_state);
}

static inline int jbd_is_locked_bh_state(struct buffer_head *bh)
{
	return bit_spin_is_locked(BH_State, &bh->b_state);
}

static inline void jbd_unlock_bh_state(struct buffer_head *bh)
{
	bit_spin_unlock(BH_State, &bh->b_state);
}

static inline void jbd_lock_bh_journal_head(struct buffer_head *bh)
{
	bit_spin_lock(BH_JournalHead, &bh->b_state);
}

static inline void jbd_unlock_bh_journal_head(struct buffer_head *bh)
{
	bit_spin_unlock(BH_JournalHead, &bh->b_state);
}

#define J_ASSERT(assert)	BUG_ON(!(assert))

#define J_ASSERT_BH(bh, expr)	J_ASSERT(expr)
#define J_ASSERT_JH(jh, expr)	J_ASSERT(expr)

#if defined(JBD2_PARANOID_IOFAIL)
#define J_EXPECT(expr, why...)		J_ASSERT(expr)
#define J_EXPECT_BH(bh, expr, why...)	J_ASSERT_BH(bh, expr)
#define J_EXPECT_JH(jh, expr, why...)	J_ASSERT_JH(jh, expr)
#else
#define __journal_expect(expr, why...)					     \
	({								     \
		int val = (expr);					     \
		if (!val) {						     \
			printk(KERN_ERR					     \
			       "JBD2 unexpected failure: %s: %s;\n",	     \
			       __func__, #expr);			     \
			printk(KERN_ERR why "\n");			     \
		}							     \
		val;							     \
	})
#define J_EXPECT(expr, why...)		__journal_expect(expr, ## why)
#define J_EXPECT_BH(bh, expr, why...)	__journal_expect(expr, ## why)
#define J_EXPECT_JH(jh, expr, why...)	__journal_expect(expr, ## why)
#endif


#define __JI_COMMIT_RUNNING 0
#define __JI_WRITE_DATA 1
#define __JI_WAIT_DATA 2


#define JI_COMMIT_RUNNING (1 << __JI_COMMIT_RUNNING)

#define JI_WRITE_DATA (1 << __JI_WRITE_DATA)

#define JI_WAIT_DATA (1 << __JI_WAIT_DATA)


struct jbd2_inode {
	
	transaction_t *i_transaction;

	
	transaction_t *i_next_transaction;

	
	struct list_head i_list;

	
	struct inode *i_vfs_inode;

	
	unsigned long i_flags;

	
	loff_t i_dirty_start;

	
	loff_t i_dirty_end;
};

struct jbd2_revoke_table_s;





struct jbd2_journal_handle
{
	union {
		transaction_t	*h_transaction;
		
		journal_t	*h_journal;
	};

	handle_t		*h_rsv_handle;
	int			h_buffer_credits;
	int			h_ref;
	int			h_err;

	
	unsigned int	h_sync:		1;
	unsigned int	h_jdata:	1;
	unsigned int	h_reserved:	1;
	unsigned int	h_aborted:	1;
	unsigned int	h_type:		8;
	unsigned int	h_line_no:	16;

	unsigned long		h_start_jiffies;
	unsigned int		h_requested_credits;

	unsigned int		saved_alloc_context;
};



struct transaction_chp_stats_s {
	unsigned long		cs_chp_time;
	__u32			cs_forced_to_close;
	__u32			cs_written;
	__u32			cs_dropped;
};





struct transaction_s
{
	
	journal_t		*t_journal;

	
	tid_t			t_tid;

	
	enum {
		T_RUNNING,
		T_LOCKED,
		T_FLUSH,
		T_COMMIT,
		T_COMMIT_DFLUSH,
		T_COMMIT_JFLUSH,
		T_COMMIT_CALLBACK,
		T_FINISHED
	}			t_state;

	
	unsigned long		t_log_start;

	
	int			t_nr_buffers;

	
	struct journal_head	*t_reserved_list;

	
	struct journal_head	*t_buffers;

	
	struct journal_head	*t_forget;

	
	struct journal_head	*t_checkpoint_list;

	
	struct journal_head	*t_checkpoint_io_list;

	
	struct journal_head	*t_shadow_list;

	
	struct list_head	t_inode_list;

	
	spinlock_t		t_handle_lock;

	
	unsigned long		t_max_wait;

	
	unsigned long		t_start;

	
	unsigned long		t_requested;

	
	struct transaction_chp_stats_s t_chp_stats;

	
	atomic_t		t_updates;

	
	atomic_t		t_outstanding_credits;

	
	transaction_t		*t_cpnext, *t_cpprev;

	
	unsigned long		t_expires;

	
	ktime_t			t_start_time;

	
	atomic_t		t_handle_count;

	
	unsigned int t_synchronous_commit:1;

	
	int			t_need_data_flush;

	
	struct list_head	t_private_list;
};

struct transaction_run_stats_s {
	unsigned long		rs_wait;
	unsigned long		rs_request_delay;
	unsigned long		rs_running;
	unsigned long		rs_locked;
	unsigned long		rs_flushing;
	unsigned long		rs_logging;

	__u32			rs_handle_count;
	__u32			rs_blocks;
	__u32			rs_blocks_logged;
};

struct transaction_stats_s {
	unsigned long		ts_tid;
	unsigned long		ts_requested;
	struct transaction_run_stats_s run;
};

static inline unsigned long
jbd2_time_diff(unsigned long start, unsigned long end)
{
	if (end >= start)
		return end - start;

	return end + (MAX_JIFFY_OFFSET - start);
}

#define JBD2_NR_BATCH	64


struct journal_s
{
	
	unsigned long		j_flags;

	
	int			j_errno;

	
	struct buffer_head	*j_sb_buffer;

	
	journal_superblock_t	*j_superblock;

	
	int			j_format_version;

	
	rwlock_t		j_state_lock;

	
	int			j_barrier_count;

	
	struct mutex		j_barrier;

	
	transaction_t		*j_running_transaction;

	
	transaction_t		*j_committing_transaction;

	
	transaction_t		*j_checkpoint_transactions;

	
	wait_queue_head_t	j_wait_transaction_locked;

	
	wait_queue_head_t	j_wait_done_commit;

	
	wait_queue_head_t	j_wait_commit;

	
	wait_queue_head_t	j_wait_updates;

	
	wait_queue_head_t	j_wait_reserved;

	
	struct mutex		j_checkpoint_mutex;

	
	struct buffer_head	*j_chkpt_bhs[JBD2_NR_BATCH];

	
	unsigned long		j_head;

	
	unsigned long		j_tail;

	
	unsigned long		j_free;

	
	unsigned long		j_first;

	
	unsigned long		j_last;

	
	struct block_device	*j_dev;

	
	int			j_blocksize;

	
	unsigned long long	j_blk_offset;

	
	char			j_devname[BDEVNAME_SIZE+24];

	
	struct block_device	*j_fs_dev;

	
	unsigned int		j_maxlen;

	
	atomic_t		j_reserved_credits;

	
	spinlock_t		j_list_lock;

	
	struct inode		*j_inode;

	
	tid_t			j_tail_sequence;

	
	tid_t			j_transaction_sequence;

	
	tid_t			j_commit_sequence;

	
	tid_t			j_commit_request;

	
	__u8			j_uuid[16];

	
	struct task_struct	*j_task;

	
	int			j_max_transaction_buffers;

	
	unsigned long		j_commit_interval;

	
	struct timer_list	j_commit_timer;

	
	spinlock_t		j_revoke_lock;

	
	struct jbd2_revoke_table_s *j_revoke;

	
	struct jbd2_revoke_table_s *j_revoke_table[2];

	
	struct buffer_head	**j_wbuf;

	
	int			j_wbufsize;

	
	pid_t			j_last_sync_writer;

	
	u64			j_average_commit_time;

	
	u32			j_min_batch_time;

	
	u32			j_max_batch_time;

	
	void			(*j_commit_callback)(journal_t *,
						     transaction_t *);

	

	
	spinlock_t		j_history_lock;

	
	struct proc_dir_entry	*j_proc_entry;

	
	struct transaction_stats_s j_stats;

	
	unsigned int		j_failed_commit;

	
	void *j_private;

	
	struct crypto_shash *j_chksum_driver;

	
	__u32 j_csum_seed;

#ifdef CONFIG_DEBUG_LOCK_ALLOC
	
	struct lockdep_map	j_trans_commit_map;
#endif
};

#define jbd2_might_wait_for_commit(j) \
	do { \
		rwsem_acquire(&j->j_trans_commit_map, 0, 0, _THIS_IP_); \
		rwsem_release(&j->j_trans_commit_map, 1, _THIS_IP_); \
	} while (0)


#define JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
static inline bool jbd2_has_feature_##name(journal_t *j) \
{ \
	return ((j)->j_format_version >= 2 && \
		((j)->j_superblock->s_feature_compat & \
		 cpu_to_be32(JBD2_FEATURE_COMPAT_##flagname)) != 0); \
} \
static inline void jbd2_set_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat |= \
		cpu_to_be32(JBD2_FEATURE_COMPAT_##flagname); \
} \
static inline void jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat &= \
		~cpu_to_be32(JBD2_FEATURE_COMPAT_##flagname); \
}

#define JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
static inline bool jbd2_has_feature_##name(journal_t *j) \
{ \
	return ((j)->j_format_version >= 2 && \
		((j)->j_superblock->s_feature_ro_compat & \
		 cpu_to_be32(JBD2_FEATURE_RO_COMPAT_##flagname)) != 0); \
} \
static inline void jbd2_set_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat |= \
		cpu_to_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
} \
static inline void jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat &= \
		~cpu_to_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
}

#define JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
static inline bool jbd2_has_feature_##name(journal_t *j) \
{ \
	return ((j)->j_format_version >= 2 && \
		((j)->j_superblock->s_feature_incompat & \
		 cpu_to_be32(JBD2_FEATURE_INCOMPAT_##flagname)) != 0); \
} \
static inline void jbd2_set_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat |= \
		cpu_to_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
} \
static inline void jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat &= \
		~cpu_to_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
}

JBD2_FEATURE_COMPAT_FUNCS(checksum,		CHECKSUM)

JBD2_FEATURE_INCOMPAT_FUNCS(revoke,		REVOKE)
JBD2_FEATURE_INCOMPAT_FUNCS(64bit,		64BIT)
JBD2_FEATURE_INCOMPAT_FUNCS(async_commit,	ASYNC_COMMIT)
JBD2_FEATURE_INCOMPAT_FUNCS(csum2,		CSUM_V2)
JBD2_FEATURE_INCOMPAT_FUNCS(csum3,		CSUM_V3)


#define JBD2_UNMOUNT	0x001	
#define JBD2_ABORT	0x002	
#define JBD2_ACK_ERR	0x004	
#define JBD2_FLUSHED	0x008	
#define JBD2_LOADED	0x010	
#define JBD2_BARRIER	0x020	
#define JBD2_ABORT_ON_SYNCDATA_ERR	0x040	
#define JBD2_REC_ERR	0x080	




extern void jbd2_journal_unfile_buffer(journal_t *, struct journal_head *);
extern void __jbd2_journal_refile_buffer(struct journal_head *);
extern void jbd2_journal_refile_buffer(journal_t *, struct journal_head *);
extern void __jbd2_journal_file_buffer(struct journal_head *, transaction_t *, int);
extern void __journal_free_buffer(struct journal_head *bh);
extern void jbd2_journal_file_buffer(struct journal_head *, transaction_t *, int);
extern void __journal_clean_data_list(transaction_t *transaction);
static inline void jbd2_file_log_bh(struct list_head *head, struct buffer_head *bh)
{
	list_add_tail(&bh->b_assoc_buffers, head);
}
static inline void jbd2_unfile_log_bh(struct buffer_head *bh)
{
	list_del_init(&bh->b_assoc_buffers);
}


struct buffer_head *jbd2_journal_get_descriptor_buffer(transaction_t *, int);
void jbd2_descriptor_block_csum_set(journal_t *, struct buffer_head *);
int jbd2_journal_next_log_block(journal_t *, unsigned long long *);
int jbd2_journal_get_log_tail(journal_t *journal, tid_t *tid,
			      unsigned long *block);
int __jbd2_update_log_tail(journal_t *journal, tid_t tid, unsigned long block);
void jbd2_update_log_tail(journal_t *journal, tid_t tid, unsigned long block);


extern void jbd2_journal_commit_transaction(journal_t *);


void __jbd2_journal_clean_checkpoint_list(journal_t *journal, bool destroy);
int __jbd2_journal_remove_checkpoint(struct journal_head *);
void jbd2_journal_destroy_checkpoint(journal_t *journal);
void __jbd2_journal_insert_checkpoint(struct journal_head *, transaction_t *);




struct jbd2_buffer_trigger_type {
	
	void (*t_frozen)(struct jbd2_buffer_trigger_type *type,
			 struct buffer_head *bh, void *mapped_data,
			 size_t size);

	
	void (*t_abort)(struct jbd2_buffer_trigger_type *type,
			struct buffer_head *bh);
};

extern void jbd2_buffer_frozen_trigger(struct journal_head *jh,
				       void *mapped_data,
				       struct jbd2_buffer_trigger_type *triggers);
extern void jbd2_buffer_abort_trigger(struct journal_head *jh,
				      struct jbd2_buffer_trigger_type *triggers);


extern int jbd2_journal_write_metadata_buffer(transaction_t *transaction,
					      struct journal_head *jh_in,
					      struct buffer_head **bh_out,
					      sector_t blocknr);


extern void		__wait_on_journal (journal_t *);


extern void jbd2_journal_destroy_transaction_cache(void);
extern int __init jbd2_journal_init_transaction_cache(void);
extern void jbd2_journal_free_transaction(transaction_t *);



static inline handle_t *journal_current_handle(void)
{
	return current->journal_info;
}



extern handle_t *jbd2_journal_start(journal_t *, int nblocks);
extern handle_t *jbd2__journal_start(journal_t *, int blocks, int rsv_blocks,
				     gfp_t gfp_mask, unsigned int type,
				     unsigned int line_no);
extern int	 jbd2_journal_restart(handle_t *, int nblocks);
extern int	 jbd2__journal_restart(handle_t *, int nblocks, gfp_t gfp_mask);
extern int	 jbd2_journal_start_reserved(handle_t *handle,
				unsigned int type, unsigned int line_no);
extern void	 jbd2_journal_free_reserved(handle_t *handle);
extern int	 jbd2_journal_extend (handle_t *, int nblocks);
extern int	 jbd2_journal_get_write_access(handle_t *, struct buffer_head *);
extern int	 jbd2_journal_get_create_access (handle_t *, struct buffer_head *);
extern int	 jbd2_journal_get_undo_access(handle_t *, struct buffer_head *);
void		 jbd2_journal_set_triggers(struct buffer_head *,
					   struct jbd2_buffer_trigger_type *type);
extern int	 jbd2_journal_dirty_metadata (handle_t *, struct buffer_head *);
extern int	 jbd2_journal_forget (handle_t *, struct buffer_head *);
extern void	 journal_sync_buffer (struct buffer_head *);
extern int	 jbd2_journal_invalidatepage(journal_t *,
				struct page *, unsigned int, unsigned int);
extern int	 jbd2_journal_try_to_free_buffers(journal_t *, struct page *, gfp_t);
extern int	 jbd2_journal_stop(handle_t *);
extern int	 jbd2_journal_flush (journal_t *);
extern void	 jbd2_journal_lock_updates (journal_t *);
extern void	 jbd2_journal_unlock_updates (journal_t *);

extern journal_t * jbd2_journal_init_dev(struct block_device *bdev,
				struct block_device *fs_dev,
				unsigned long long start, int len, int bsize);
extern journal_t * jbd2_journal_init_inode (struct inode *);
extern int	   jbd2_journal_update_format (journal_t *);
extern int	   jbd2_journal_check_used_features
		   (journal_t *, unsigned long, unsigned long, unsigned long);
extern int	   jbd2_journal_check_available_features
		   (journal_t *, unsigned long, unsigned long, unsigned long);
extern int	   jbd2_journal_set_features
		   (journal_t *, unsigned long, unsigned long, unsigned long);
extern void	   jbd2_journal_clear_features
		   (journal_t *, unsigned long, unsigned long, unsigned long);
extern int	   jbd2_journal_load       (journal_t *journal);
extern int	   jbd2_journal_destroy    (journal_t *);
extern int	   jbd2_journal_recover    (journal_t *journal);
extern int	   jbd2_journal_wipe       (journal_t *, int);
extern int	   jbd2_journal_skip_recovery	(journal_t *);
extern void	   jbd2_journal_update_sb_errno(journal_t *);
extern int	   jbd2_journal_update_sb_log_tail	(journal_t *, tid_t,
				unsigned long, int);
extern void	   __jbd2_journal_abort_hard	(journal_t *);
extern void	   jbd2_journal_abort      (journal_t *, int);
extern int	   jbd2_journal_errno      (journal_t *);
extern void	   jbd2_journal_ack_err    (journal_t *);
extern int	   jbd2_journal_clear_err  (journal_t *);
extern int	   jbd2_journal_bmap(journal_t *, unsigned long, unsigned long long *);
extern int	   jbd2_journal_force_commit(journal_t *);
extern int	   jbd2_journal_force_commit_nested(journal_t *);
extern int	   jbd2_journal_inode_add_write(handle_t *handle, struct jbd2_inode *inode);
extern int	   jbd2_journal_inode_add_wait(handle_t *handle, struct jbd2_inode *inode);
extern int	   jbd2_journal_inode_ranged_write(handle_t *handle,
			struct jbd2_inode *inode, loff_t start_byte,
			loff_t length);
extern int	   jbd2_journal_inode_ranged_wait(handle_t *handle,
			struct jbd2_inode *inode, loff_t start_byte,
			loff_t length);
extern int	   jbd2_journal_begin_ordered_truncate(journal_t *journal,
				struct jbd2_inode *inode, loff_t new_size);
extern void	   jbd2_journal_init_jbd_inode(struct jbd2_inode *jinode, struct inode *inode);
extern void	   jbd2_journal_release_jbd_inode(journal_t *journal, struct jbd2_inode *jinode);


struct journal_head *jbd2_journal_add_journal_head(struct buffer_head *bh);
struct journal_head *jbd2_journal_grab_journal_head(struct buffer_head *bh);
void jbd2_journal_put_journal_head(struct journal_head *jh);


extern struct kmem_cache *jbd2_handle_cache;

static inline handle_t *jbd2_alloc_handle(gfp_t gfp_flags)
{
	return kmem_cache_zalloc(jbd2_handle_cache, gfp_flags);
}

static inline void jbd2_free_handle(handle_t *handle)
{
	kmem_cache_free(jbd2_handle_cache, handle);
}


extern struct kmem_cache *jbd2_inode_cache;

static inline struct jbd2_inode *jbd2_alloc_inode(gfp_t gfp_flags)
{
	return kmem_cache_alloc(jbd2_inode_cache, gfp_flags);
}

static inline void jbd2_free_inode(struct jbd2_inode *jinode)
{
	kmem_cache_free(jbd2_inode_cache, jinode);
}


#define JOURNAL_REVOKE_DEFAULT_HASH 256
extern int	   jbd2_journal_init_revoke(journal_t *, int);
extern void	   jbd2_journal_destroy_revoke_record_cache(void);
extern void	   jbd2_journal_destroy_revoke_table_cache(void);
extern int __init jbd2_journal_init_revoke_record_cache(void);
extern int __init jbd2_journal_init_revoke_table_cache(void);

extern void	   jbd2_journal_destroy_revoke(journal_t *);
extern int	   jbd2_journal_revoke (handle_t *, unsigned long long, struct buffer_head *);
extern int	   jbd2_journal_cancel_revoke(handle_t *, struct journal_head *);
extern void	   jbd2_journal_write_revoke_records(transaction_t *transaction,
						     struct list_head *log_bufs);


extern int	jbd2_journal_set_revoke(journal_t *, unsigned long long, tid_t);
extern int	jbd2_journal_test_revoke(journal_t *, unsigned long long, tid_t);
extern void	jbd2_journal_clear_revoke(journal_t *);
extern void	jbd2_journal_switch_revoke_table(journal_t *journal);
extern void	jbd2_clear_buffer_revoked_flags(journal_t *journal);



int jbd2_log_start_commit(journal_t *journal, tid_t tid);
int __jbd2_log_start_commit(journal_t *journal, tid_t tid);
int jbd2_journal_start_commit(journal_t *journal, tid_t *tid);
int jbd2_log_wait_commit(journal_t *journal, tid_t tid);
int jbd2_transaction_committed(journal_t *journal, tid_t tid);
int jbd2_complete_transaction(journal_t *journal, tid_t tid);
int jbd2_log_do_checkpoint(journal_t *journal);
int jbd2_trans_will_send_data_barrier(journal_t *journal, tid_t tid);

void __jbd2_log_wait_for_space(journal_t *journal);
extern void __jbd2_journal_drop_transaction(journal_t *, transaction_t *);
extern int jbd2_cleanup_journal_tail(journal_t *);



static inline int is_journal_aborted(journal_t *journal)
{
	return journal->j_flags & JBD2_ABORT;
}

static inline int is_handle_aborted(handle_t *handle)
{
	if (handle->h_aborted || !handle->h_transaction)
		return 1;
	return is_journal_aborted(handle->h_transaction->t_journal);
}

static inline void jbd2_journal_abort_handle(handle_t *handle)
{
	handle->h_aborted = 1;
}

#endif 



static inline int tid_gt(tid_t x, tid_t y)
{
	int difference = (x - y);
	return (difference > 0);
}

static inline int tid_geq(tid_t x, tid_t y)
{
	int difference = (x - y);
	return (difference >= 0);
}

extern int jbd2_journal_blocks_per_page(struct inode *inode);
extern size_t journal_tag_bytes(journal_t *journal);

static inline bool jbd2_journal_has_csum_v2or3_feature(journal_t *j)
{
	return jbd2_has_feature_csum2(j) || jbd2_has_feature_csum3(j);
}

static inline int jbd2_journal_has_csum_v2or3(journal_t *journal)
{
	WARN_ON_ONCE(jbd2_journal_has_csum_v2or3_feature(journal) &&
		     journal->j_chksum_driver == NULL);

	return journal->j_chksum_driver != NULL;
}


#define JBD2_CONTROL_BLOCKS_SHIFT 5


static inline int jbd2_space_needed(journal_t *journal)
{
	int nblocks = journal->j_max_transaction_buffers;
	return nblocks + (nblocks >> JBD2_CONTROL_BLOCKS_SHIFT);
}


static inline unsigned long jbd2_log_space_left(journal_t *journal)
{
	
	long free = journal->j_free - 32;

	if (journal->j_committing_transaction) {
		unsigned long committing = atomic_read(&journal->
			j_committing_transaction->t_outstanding_credits);

		
		free -= committing + (committing >> JBD2_CONTROL_BLOCKS_SHIFT);
	}
	return max_t(long, free, 0);
}




#define BJ_None		0	
#define BJ_Metadata	1	
#define BJ_Forget	2	
#define BJ_Shadow	3	
#define BJ_Reserved	4	
#define BJ_Types	5

extern int jbd_blocks_per_page(struct inode *inode);


#define JBD_MAX_CHECKSUM_SIZE 4

static inline u32 jbd2_chksum(journal_t *journal, u32 crc,
			      const void *address, unsigned int length)
{
	struct {
		struct shash_desc shash;
		char ctx[JBD_MAX_CHECKSUM_SIZE];
	} desc;
	int err;

	BUG_ON(crypto_shash_descsize(journal->j_chksum_driver) >
		JBD_MAX_CHECKSUM_SIZE);

	desc.shash.tfm = journal->j_chksum_driver;
	desc.shash.flags = 0;
	*(u32 *)desc.ctx = crc;

	err = crypto_shash_update(&desc.shash, address, length);
	BUG_ON(err);

	return *(u32 *)desc.ctx;
}


static inline tid_t  jbd2_get_latest_transaction(journal_t *journal)
{
	tid_t tid;

	read_lock(&journal->j_state_lock);
	tid = journal->j_commit_request;
	if (journal->j_running_transaction)
		tid = journal->j_running_transaction->t_tid;
	read_unlock(&journal->j_state_lock);
	return tid;
}

#ifdef __KERNEL__

#define buffer_trace_init(bh)	do {} while (0)
#define print_buffer_fields(bh)	do {} while (0)
#define print_buffer_trace(bh)	do {} while (0)
#define BUFFER_TRACE(bh, info)	do {} while (0)
#define BUFFER_TRACE2(bh, bh2, info)	do {} while (0)
#define JBUFFER_TRACE(jh, info)	do {} while (0)

#endif	

#define EFSBADCRC	EBADMSG		
#define EFSCORRUPTED	EUCLEAN		

#endif	
