<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
	Fujitsu MB86A16 DVB-S/DSS DC Receiver driver

	Copyright (C) Manu Abraham (abraham.manu@gmail.com)

<<<<<<< HEAD
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> upstream/android-13
*/

#ifndef __MB86A16_PRIV_H
#define __MB86A16_PRIV_H

#define MB86A16_TSOUT		0x00
#define MB86A16_TSOUT_HIZSEL	(0x01 << 5)
#define MB86A16_TSOUT_HIZCNTI	(0x01 << 4)
#define MB86A16_TSOUT_MODE	(0x01 << 3)
#define MB86A16_TSOUT_ORDER	(0x01 << 2)
#define MB86A16_TSOUT_ERROR	(0x01 << 1)
#define Mb86A16_TSOUT_EDGE	(0x01 << 0)

#define MB86A16_FEC		0x01
#define MB86A16_FEC_FSYNC	(0x01 << 5)
#define MB86A16_FEC_PCKB8	(0x01 << 4)
#define MB86A16_FEC_DVDS	(0x01 << 3)
#define MB86A16_FEC_EREN	(0x01 << 2)
#define Mb86A16_FEC_RSEN	(0x01 << 1)
#define MB86A16_FEC_DIEN	(0x01 << 0)

#define MB86A16_AGC		0x02
#define MB86A16_AGC_AGMD	(0x01 << 6)
#define MB86A16_AGC_AGCW	(0x0f << 2)
#define MB86A16_AGC_AGCP	(0x01 << 1)
#define MB86A16_AGC_AGCR	(0x01 << 0)

#define MB86A16_SRATE1		0x03
#define MB86A16_SRATE1_DECI	(0x07 << 2)
#define MB86A16_SRATE1_CSEL	(0x01 << 1)
#define MB86A16_SRATE1_RSEL	(0x01 << 0)

#define MB86A16_SRATE2		0x04
#define MB86A16_SRATE2_STOFSL	(0xff << 0)

#define MB86A16_SRATE3		0x05
#define MB86A16_SRATE2_STOFSH	(0xff << 0)

#define MB86A16_VITERBI		0x06
#define MB86A16_FRAMESYNC	0x07
#define MB86A16_CRLFILTCOEF1	0x08
#define MB86A16_CRLFILTCOEF2	0x09
#define MB86A16_STRFILTCOEF1	0x0a
#define MB86A16_STRFILTCOEF2	0x0b
#define MB86A16_RESET		0x0c
#define MB86A16_STATUS		0x0d
#define MB86A16_AFCML		0x0e
#define MB86A16_AFCMH		0x0f
#define MB86A16_BERMON		0x10
#define MB86A16_BERTAB		0x11
#define MB86A16_BERLSB		0x12
#define MB86A16_BERMID		0x13
#define MB86A16_BERMSB		0x14
#define MB86A16_AGCM		0x15

#define MB86A16_DCC1		0x16
#define MB86A16_DCC1_DISTA	(0x01 << 7)
#define MB86A16_DCC1_PRTY	(0x01 << 6)
#define MB86A16_DCC1_CTOE	(0x01 << 5)
#define MB86A16_DCC1_TBEN	(0x01 << 4)
#define MB86A16_DCC1_TBO	(0x01 << 3)
#define MB86A16_DCC1_NUM	(0x07 << 0)

#define MB86A16_DCC2		0x17
#define MB86A16_DCC2_DCBST	(0x01 << 0)

#define MB86A16_DCC3		0x18
#define MB86A16_DCC3_CODE0	(0xff << 0)

#define MB86A16_DCC4		0x19
#define MB86A16_DCC4_CODE1	(0xff << 0)

#define MB86A16_DCC5		0x1a
#define MB86A16_DCC5_CODE2	(0xff << 0)

#define MB86A16_DCC6		0x1b
#define MB86A16_DCC6_CODE3	(0xff << 0)

#define MB86A16_DCC7		0x1c
#define MB86A16_DCC7_CODE4	(0xff << 0)

#define MB86A16_DCC8		0x1d
#define MB86A16_DCC8_CODE5	(0xff << 0)

#define MB86A16_DCCOUT		0x1e
#define MB86A16_DCCOUT_DISEN	(0x01 << 0)

#define MB86A16_TONEOUT1	0x1f
#define MB86A16_TONE_TDIVL	(0xff << 0)

#define MB86A16_TONEOUT2	0x20
#define MB86A16_TONE_TMD	(0x03 << 2)
#define MB86A16_TONE_TDIVH	(0x03 << 0)

#define MB86A16_FREQ1		0x21
#define MB86A16_FREQ2		0x22
#define MB86A16_FREQ3		0x23
#define MB86A16_FREQ4		0x24
#define MB86A16_FREQSET		0x25
#define MB86A16_CNM		0x26
#define MB86A16_PORT0		0x27
#define MB86A16_PORT1		0x28
#define MB86A16_DRCFILT		0x29
#define MB86A16_AFC		0x2a
#define MB86A16_AFCEXL		0x2b
#define MB86A16_AFCEXH		0x2c
#define MB86A16_DAGC		0x2d
#define MB86A16_SEQMODE		0x32
#define MB86A16_S0S1T		0x33
#define MB86A16_S2S3T		0x34
#define MB86A16_S4S5T		0x35
#define MB86A16_CNTMR		0x36
#define MB86A16_SIG1		0x37
#define MB86A16_SIG2		0x38
#define MB86A16_VIMAG		0x39
#define MB86A16_VISET1		0x3a
#define MB86A16_VISET2		0x3b
#define MB86A16_VISET3		0x3c
#define MB86A16_FAGCS1		0x3d
#define MB86A16_FAGCS2		0x3e
#define MB86A16_FAGCS3		0x3f
#define MB86A16_FAGCS4		0x40
#define MB86A16_FAGCS5		0x41
#define MB86A16_FAGCS6		0x42
#define MB86A16_CRM		0x43
#define MB86A16_STRM		0x44
#define MB86A16_DAGCML		0x45
#define MB86A16_DAGCMH		0x46
#define MB86A16_QPSKTST		0x49
#define MB86A16_DISTMON		0x52
#define MB86A16_VERSION		0x7f

#endif /* __MB86A16_PRIV_H */
