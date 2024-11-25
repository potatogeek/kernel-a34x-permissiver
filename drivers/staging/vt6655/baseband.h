<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0+
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
<<<<<<< HEAD
 * File: baseband.h
 *
=======
>>>>>>> upstream/android-13
 * Purpose: Implement functions to access baseband
 *
 * Author: Jerry Chen
 *
 * Date: Jun. 5, 2002
 *
 */

#ifndef __BASEBAND_H__
#define __BASEBAND_H__

#include "device.h"

/*
 * Registers in the BASEBAND
 */
#define BB_MAX_CONTEXT_SIZE 256

/*
 * Baseband RF pair definition in eeprom (Bits 6..0)
 */

#define PREAMBLE_LONG   0
#define PREAMBLE_SHORT  1

#define F5G             0
#define F2_4G           1

#define TOP_RATE_54M        0x80000000
#define TOP_RATE_48M        0x40000000
#define TOP_RATE_36M        0x20000000
#define TOP_RATE_24M        0x10000000
#define TOP_RATE_18M        0x08000000
#define TOP_RATE_12M        0x04000000
#define TOP_RATE_11M        0x02000000
#define TOP_RATE_9M         0x01000000
#define TOP_RATE_6M         0x00800000
#define TOP_RATE_55M        0x00400000
#define TOP_RATE_2M         0x00200000
#define TOP_RATE_1M         0x00100000

<<<<<<< HEAD
unsigned int BBuGetFrameTime(unsigned char byPreambleType,
			     unsigned char byPktType,
			     unsigned int cbFrameLength,
			     unsigned short wRate);
=======
unsigned int bb_get_frame_time(unsigned char by_preamble_type,
			       unsigned char by_pkt_type,
			       unsigned int cb_frame_length,
			       unsigned short w_rate);
>>>>>>> upstream/android-13

void vnt_get_phy_field(struct vnt_private *priv, u32 frame_length,
		       u16 tx_rate, u8 pkt_type, struct vnt_phy_field *phy);

<<<<<<< HEAD
bool BBbReadEmbedded(struct vnt_private *priv, unsigned char byBBAddr,
		     unsigned char *pbyData);
bool BBbWriteEmbedded(struct vnt_private *priv, unsigned char byBBAddr,
		      unsigned char byData);

void BBvSetShortSlotTime(struct vnt_private *priv);
void BBvSetVGAGainOffset(struct vnt_private *priv, unsigned char byData);

/* VT3253 Baseband */
bool BBbVT3253Init(struct vnt_private *priv);
void BBvSoftwareReset(struct vnt_private *priv);
void BBvPowerSaveModeON(struct vnt_private *priv);
void BBvPowerSaveModeOFF(struct vnt_private *priv);
void BBvSetTxAntennaMode(struct vnt_private *priv, unsigned char byAntennaMode);
void BBvSetRxAntennaMode(struct vnt_private *priv, unsigned char byAntennaMode);
void BBvSetDeepSleep(struct vnt_private *priv, unsigned char byLocalID);
void BBvExitDeepSleep(struct vnt_private *priv, unsigned char byLocalID);
=======
bool bb_read_embedded(struct vnt_private *priv, unsigned char by_bb_addr,
		      unsigned char *pby_data);
bool bb_write_embedded(struct vnt_private *priv, unsigned char by_bb_addr,
		       unsigned char by_data);

void bb_set_short_slot_time(struct vnt_private *priv);
void bb_set_vga_gain_offset(struct vnt_private *priv, unsigned char by_data);

/* VT3253 Baseband */
bool bb_vt3253_init(struct vnt_private *priv);
void bb_software_reset(struct vnt_private *priv);
void bb_power_save_mode_on(struct vnt_private *priv);
void bb_power_save_mode_off(struct vnt_private *priv);
void bb_set_tx_antenna_mode(struct vnt_private *priv,
			    unsigned char by_antenna_mode);
void bb_set_rx_antenna_mode(struct vnt_private *priv,
			    unsigned char by_antenna_mode);
void bb_set_deep_sleep(struct vnt_private *priv, unsigned char by_local_id);
>>>>>>> upstream/android-13

#endif /* __BASEBAND_H__ */
