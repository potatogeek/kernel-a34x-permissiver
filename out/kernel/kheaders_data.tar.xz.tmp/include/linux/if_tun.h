
#ifndef __IF_TUN_H
#define __IF_TUN_H

#include <uapi/linux/if_tun.h>

#define TUN_XDP_FLAG 0x1UL

#if defined(CONFIG_TUN) || defined(CONFIG_TUN_MODULE)
struct socket *tun_get_socket(struct file *);
struct ptr_ring *tun_get_tx_ring(struct file *file);
bool tun_is_xdp_frame(void *ptr);
void *tun_xdp_to_ptr(void *ptr);
void *tun_ptr_to_xdp(void *ptr);
void tun_ptr_free(void *ptr);
#else
#include <linux/err.h>
#include <linux/errno.h>
struct file;
struct socket;
static inline struct socket *tun_get_socket(struct file *f)
{
	return ERR_PTR(-EINVAL);
}
static inline struct ptr_ring *tun_get_tx_ring(struct file *f)
{
	return ERR_PTR(-EINVAL);
}
static inline bool tun_is_xdp_frame(void *ptr)
{
	return false;
}
static inline void *tun_xdp_to_ptr(void *ptr)
{
	return NULL;
}
static inline void *tun_ptr_to_xdp(void *ptr)
{
	return NULL;
}
static inline void tun_ptr_free(void *ptr)
{
}
#endif 
#endif 
