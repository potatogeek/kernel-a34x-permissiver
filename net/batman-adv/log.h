/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
/* Copyright (C) 2007-2018  B.A.T.M.A.N. contributors:
 *
 * Marek Lindner, Simon Wunderlich
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
/* Copyright (C) B.A.T.M.A.N. contributors:
 *
 * Marek Lindner, Simon Wunderlich
>>>>>>> upstream/android-13
 */

#ifndef _NET_BATMAN_ADV_LOG_H_
#define _NET_BATMAN_ADV_LOG_H_

#include "main.h"

<<<<<<< HEAD
=======
#include <linux/atomic.h>
>>>>>>> upstream/android-13
#include <linux/bitops.h>
#include <linux/compiler.h>
#include <linux/printk.h>

#ifdef CONFIG_BATMAN_ADV_DEBUG

int batadv_debug_log_setup(struct batadv_priv *bat_priv);
void batadv_debug_log_cleanup(struct batadv_priv *bat_priv);

#else

static inline int batadv_debug_log_setup(struct batadv_priv *bat_priv)
{
	return 0;
}

static inline void batadv_debug_log_cleanup(struct batadv_priv *bat_priv)
{
}

#endif

/**
 * enum batadv_dbg_level - available log levels
 */
enum batadv_dbg_level {
	/** @BATADV_DBG_BATMAN: OGM and TQ computations related messages */
	BATADV_DBG_BATMAN	= BIT(0),

	/** @BATADV_DBG_ROUTES: route added / changed / deleted */
	BATADV_DBG_ROUTES	= BIT(1),

	/** @BATADV_DBG_TT: translation table messages */
	BATADV_DBG_TT		= BIT(2),

	/** @BATADV_DBG_BLA: bridge loop avoidance messages */
	BATADV_DBG_BLA		= BIT(3),

	/** @BATADV_DBG_DAT: ARP snooping and DAT related messages */
	BATADV_DBG_DAT		= BIT(4),

	/** @BATADV_DBG_NC: network coding related messages */
	BATADV_DBG_NC		= BIT(5),

	/** @BATADV_DBG_MCAST: multicast related messages */
	BATADV_DBG_MCAST	= BIT(6),

	/** @BATADV_DBG_TP_METER: throughput meter messages */
	BATADV_DBG_TP_METER	= BIT(7),

	/** @BATADV_DBG_ALL: the union of all the above log levels */
	BATADV_DBG_ALL		= 255,
};

#ifdef CONFIG_BATMAN_ADV_DEBUG
int batadv_debug_log(struct batadv_priv *bat_priv, const char *fmt, ...)
__printf(2, 3);

/**
<<<<<<< HEAD
 * _batadv_dbg() - Store debug output with(out) ratelimiting
=======
 * _batadv_dbg() - Store debug output with(out) rate limiting
>>>>>>> upstream/android-13
 * @type: type of debug message
 * @bat_priv: the bat priv with all the soft interface information
 * @ratelimited: whether output should be rate limited
 * @fmt: format string
<<<<<<< HEAD
 * @arg...: variable arguments
=======
 * @arg: variable arguments
>>>>>>> upstream/android-13
 */
#define _batadv_dbg(type, bat_priv, ratelimited, fmt, arg...)		\
	do {								\
		struct batadv_priv *__batpriv = (bat_priv);		\
		if (atomic_read(&__batpriv->log_level) & (type) &&	\
		    (!(ratelimited) || net_ratelimit()))		\
			batadv_debug_log(__batpriv, fmt, ## arg);	\
	}								\
	while (0)
#else /* !CONFIG_BATMAN_ADV_DEBUG */
__printf(4, 5)
static inline void _batadv_dbg(int type __always_unused,
			       struct batadv_priv *bat_priv __always_unused,
			       int ratelimited __always_unused,
			       const char *fmt __always_unused, ...)
{
}
#endif

/**
<<<<<<< HEAD
 * batadv_dbg() - Store debug output without ratelimiting
 * @type: type of debug message
 * @bat_priv: the bat priv with all the soft interface information
 * @arg...: format string and variable arguments
=======
 * batadv_dbg() - Store debug output without rate limiting
 * @type: type of debug message
 * @bat_priv: the bat priv with all the soft interface information
 * @arg: format string and variable arguments
>>>>>>> upstream/android-13
 */
#define batadv_dbg(type, bat_priv, arg...) \
	_batadv_dbg(type, bat_priv, 0, ## arg)

/**
<<<<<<< HEAD
 * batadv_dbg_ratelimited() - Store debug output with ratelimiting
 * @type: type of debug message
 * @bat_priv: the bat priv with all the soft interface information
 * @arg...: format string and variable arguments
=======
 * batadv_dbg_ratelimited() - Store debug output with rate limiting
 * @type: type of debug message
 * @bat_priv: the bat priv with all the soft interface information
 * @arg: format string and variable arguments
>>>>>>> upstream/android-13
 */
#define batadv_dbg_ratelimited(type, bat_priv, arg...) \
	_batadv_dbg(type, bat_priv, 1, ## arg)

/**
 * batadv_info() - Store message in debug buffer and print it to kmsg buffer
 * @net_dev: the soft interface net device
 * @fmt: format string
<<<<<<< HEAD
 * @arg...: variable arguments
=======
 * @arg: variable arguments
>>>>>>> upstream/android-13
 */
#define batadv_info(net_dev, fmt, arg...)				\
	do {								\
		struct net_device *_netdev = (net_dev);                 \
		struct batadv_priv *_batpriv = netdev_priv(_netdev);    \
		batadv_dbg(BATADV_DBG_ALL, _batpriv, fmt, ## arg);	\
		pr_info("%s: " fmt, _netdev->name, ## arg);		\
	} while (0)

/**
 * batadv_err() - Store error in debug buffer and print it to kmsg buffer
 * @net_dev: the soft interface net device
 * @fmt: format string
<<<<<<< HEAD
 * @arg...: variable arguments
=======
 * @arg: variable arguments
>>>>>>> upstream/android-13
 */
#define batadv_err(net_dev, fmt, arg...)				\
	do {								\
		struct net_device *_netdev = (net_dev);                 \
		struct batadv_priv *_batpriv = netdev_priv(_netdev);    \
		batadv_dbg(BATADV_DBG_ALL, _batpriv, fmt, ## arg);	\
		pr_err("%s: " fmt, _netdev->name, ## arg);		\
	} while (0)

#endif /* _NET_BATMAN_ADV_LOG_H_ */
