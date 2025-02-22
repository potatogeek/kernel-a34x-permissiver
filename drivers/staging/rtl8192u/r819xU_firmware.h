/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __INC_FIRMWARE_H
#define __INC_FIRMWARE_H

<<<<<<< HEAD
#define RTL8190_CPU_START_OFFSET	0x80
#define GET_COMMAND_PACKET_FRAG_THRESHOLD(v) \
	(4*(v/4) - 8 - USB_HWDESC_HEADER_LEN)

typedef enum _firmware_init_step {
	FW_INIT_STEP0_BOOT = 0,
	FW_INIT_STEP1_MAIN = 1,
	FW_INIT_STEP2_DATA = 2,
} firmware_init_step_e;

typedef enum _opt_rst_type {
	OPT_SYSTEM_RESET = 0,
	OPT_FIRMWARE_RESET = 1,
} opt_rst_type_e;
=======
#define GET_COMMAND_PACKET_FRAG_THRESHOLD(v) \
	(4 * ((v) / 4) - 8 - USB_HWDESC_HEADER_LEN)

enum firmware_init_step_e {
	FW_INIT_STEP0_BOOT = 0,
	FW_INIT_STEP1_MAIN = 1,
	FW_INIT_STEP2_DATA = 2,
};

enum opt_rst_type_e {
	OPT_SYSTEM_RESET = 0,
	OPT_FIRMWARE_RESET = 1,
};
>>>>>>> upstream/android-13

#endif
