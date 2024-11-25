<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * u_midi.h
 *
 * Utility definitions for the midi function
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
<<<<<<< HEAD
 * Author: Andrzej Pietrasiewicz <andrzej.p@samsung.com>
=======
 * Author: Andrzej Pietrasiewicz <andrzejtp2010@gmail.com>
>>>>>>> upstream/android-13
 */

#ifndef U_MIDI_H
#define U_MIDI_H

#include <linux/usb/composite.h>

struct f_midi_opts {
	struct usb_function_instance	func_inst;
	int				index;
	char				*id;
	bool				id_allocated;
	unsigned int			in_ports;
	unsigned int			out_ports;
	unsigned int			buflen;
	unsigned int			qlen;

	/*
	 * Protect the data form concurrent access by read/write
	 * and create symlink/remove symlink.
	 */
<<<<<<< HEAD
	 struct mutex			lock;
	 int				refcnt;
=======
	struct mutex			lock;
	int				refcnt;
>>>>>>> upstream/android-13
};

#endif /* U_MIDI_H */

