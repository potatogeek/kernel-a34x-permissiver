/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/wait.h>

struct fs_pin {
	wait_queue_head_t	wait;
	int			done;
	struct hlist_node	s_list;
	struct hlist_node	m_list;
	void (*kill)(struct fs_pin *);
};

struct vfsmount;

static inline void init_fs_pin(struct fs_pin *p, void (*kill)(struct fs_pin *))
{
	init_waitqueue_head(&p->wait);
	INIT_HLIST_NODE(&p->s_list);
	INIT_HLIST_NODE(&p->m_list);
	p->kill = kill;
}

void pin_remove(struct fs_pin *);
<<<<<<< HEAD
void pin_insert_group(struct fs_pin *, struct vfsmount *, struct hlist_head *);
=======
>>>>>>> upstream/android-13
void pin_insert(struct fs_pin *, struct vfsmount *);
void pin_kill(struct fs_pin *);
