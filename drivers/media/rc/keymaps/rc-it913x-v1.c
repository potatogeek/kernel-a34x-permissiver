<<<<<<< HEAD
/* ITE Generic remotes Version 1
 *
 * Copyright (C) 2012 Malcolm Priestley (tvboxspy@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/* ITE Generic remotes Version 1
 *
 * Copyright (C) 2012 Malcolm Priestley (tvboxspy@gmail.com)
>>>>>>> upstream/android-13
 */

#include <media/rc-map.h>
#include <linux/module.h>


static struct rc_map_table it913x_v1_rc[] = {
	/* Type 1 */
	{ 0x61d601, KEY_VIDEO },           /* Source */
<<<<<<< HEAD
	{ 0x61d602, KEY_3 },
	{ 0x61d603, KEY_POWER },           /* ShutDown */
	{ 0x61d604, KEY_1 },
	{ 0x61d605, KEY_5 },
	{ 0x61d606, KEY_6 },
	{ 0x61d607, KEY_CHANNELDOWN },     /* CH- */
	{ 0x61d608, KEY_2 },
	{ 0x61d609, KEY_CHANNELUP },       /* CH+ */
	{ 0x61d60a, KEY_9 },
	{ 0x61d60b, KEY_ZOOM },            /* Zoom */
	{ 0x61d60c, KEY_7 },
	{ 0x61d60d, KEY_8 },
	{ 0x61d60e, KEY_VOLUMEUP },        /* Vol+ */
	{ 0x61d60f, KEY_4 },
	{ 0x61d610, KEY_ESC },             /* [back up arrow] */
	{ 0x61d611, KEY_0 },
=======
	{ 0x61d602, KEY_NUMERIC_3 },
	{ 0x61d603, KEY_POWER },           /* ShutDown */
	{ 0x61d604, KEY_NUMERIC_1 },
	{ 0x61d605, KEY_NUMERIC_5 },
	{ 0x61d606, KEY_NUMERIC_6 },
	{ 0x61d607, KEY_CHANNELDOWN },     /* CH- */
	{ 0x61d608, KEY_NUMERIC_2 },
	{ 0x61d609, KEY_CHANNELUP },       /* CH+ */
	{ 0x61d60a, KEY_NUMERIC_9 },
	{ 0x61d60b, KEY_ZOOM },            /* Zoom */
	{ 0x61d60c, KEY_NUMERIC_7 },
	{ 0x61d60d, KEY_NUMERIC_8 },
	{ 0x61d60e, KEY_VOLUMEUP },        /* Vol+ */
	{ 0x61d60f, KEY_NUMERIC_4 },
	{ 0x61d610, KEY_ESC },             /* [back up arrow] */
	{ 0x61d611, KEY_NUMERIC_0 },
>>>>>>> upstream/android-13
	{ 0x61d612, KEY_OK },              /* [enter arrow] */
	{ 0x61d613, KEY_VOLUMEDOWN },      /* Vol- */
	{ 0x61d614, KEY_RECORD },          /* Rec */
	{ 0x61d615, KEY_STOP },            /* Stop */
	{ 0x61d616, KEY_PLAY },            /* Play */
	{ 0x61d617, KEY_MUTE },            /* Mute */
	{ 0x61d618, KEY_UP },
	{ 0x61d619, KEY_DOWN },
	{ 0x61d61a, KEY_LEFT },
	{ 0x61d61b, KEY_RIGHT },
	{ 0x61d61c, KEY_RED },
	{ 0x61d61d, KEY_GREEN },
	{ 0x61d61e, KEY_YELLOW },
	{ 0x61d61f, KEY_BLUE },
	{ 0x61d643, KEY_POWER2 },          /* [red power button] */
	/* Type 2 - 20 buttons */
<<<<<<< HEAD
	{ 0x807f0d, KEY_0 },
	{ 0x807f04, KEY_1 },
	{ 0x807f05, KEY_2 },
	{ 0x807f06, KEY_3 },
	{ 0x807f07, KEY_4 },
	{ 0x807f08, KEY_5 },
	{ 0x807f09, KEY_6 },
	{ 0x807f0a, KEY_7 },
	{ 0x807f1b, KEY_8 },
	{ 0x807f1f, KEY_9 },
=======
	{ 0x807f0d, KEY_NUMERIC_0 },
	{ 0x807f04, KEY_NUMERIC_1 },
	{ 0x807f05, KEY_NUMERIC_2 },
	{ 0x807f06, KEY_NUMERIC_3 },
	{ 0x807f07, KEY_NUMERIC_4 },
	{ 0x807f08, KEY_NUMERIC_5 },
	{ 0x807f09, KEY_NUMERIC_6 },
	{ 0x807f0a, KEY_NUMERIC_7 },
	{ 0x807f1b, KEY_NUMERIC_8 },
	{ 0x807f1f, KEY_NUMERIC_9 },
>>>>>>> upstream/android-13
	{ 0x807f12, KEY_POWER },
	{ 0x807f01, KEY_MEDIA_REPEAT}, /* Recall */
	{ 0x807f19, KEY_PAUSE }, /* Timeshift */
	{ 0x807f1e, KEY_VOLUMEUP }, /* 2 x -/+ Keys not marked */
	{ 0x807f03, KEY_VOLUMEDOWN }, /* Volume defined as right hand*/
	{ 0x807f1a, KEY_CHANNELUP },
	{ 0x807f02, KEY_CHANNELDOWN },
	{ 0x807f0c, KEY_ZOOM },
	{ 0x807f00, KEY_RECORD },
	{ 0x807f0e, KEY_STOP },
};

static struct rc_map_list it913x_v1_map = {
	.map = {
		.scan     = it913x_v1_rc,
		.size     = ARRAY_SIZE(it913x_v1_rc),
<<<<<<< HEAD
		.rc_proto = RC_PROTO_NEC,
=======
		.rc_proto = RC_PROTO_NECX,
>>>>>>> upstream/android-13
		.name     = RC_MAP_IT913X_V1,
	}
};

static int __init init_rc_it913x_v1_map(void)
{
	return rc_map_register(&it913x_v1_map);
}

static void __exit exit_rc_it913x_v1_map(void)
{
	rc_map_unregister(&it913x_v1_map);
}

module_init(init_rc_it913x_v1_map)
module_exit(exit_rc_it913x_v1_map)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Malcolm Priestley tvboxspy@gmail.com");
