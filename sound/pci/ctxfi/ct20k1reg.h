<<<<<<< HEAD
/**
 * Copyright (C) 2008, Creative Technology Ltd. All Rights Reserved.
 *
 * This source file is released under GPL v2 license (no other versions).
 * See the COPYING file included in the main directory of this source
 * distribution for the license terms and conditions.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2008, Creative Technology Ltd. All Rights Reserved.
>>>>>>> upstream/android-13
 */

#ifndef CT20K1REG_H
#define CT20K1REG_H

/* 20k1 registers */
#define 	DSPXRAM_START 			0x000000
#define 	DSPXRAM_END 			0x013FFC
#define 	DSPAXRAM_START 			0x020000
#define 	DSPAXRAM_END 			0x023FFC
#define 	DSPYRAM_START 			0x040000
#define 	DSPYRAM_END 			0x04FFFC
#define 	DSPAYRAM_START 			0x020000
#define 	DSPAYRAM_END 			0x063FFC
#define 	DSPMICRO_START 			0x080000
#define 	DSPMICRO_END 			0x0B3FFC
#define 	DSP0IO_START 			0x100000
#define 	DSP0IO_END	 		0x101FFC
#define 	AUDIORINGIPDSP0_START 		0x100000
#define 	AUDIORINGIPDSP0_END 		0x1003FC
#define 	AUDIORINGOPDSP0_START 		0x100400
#define 	AUDIORINGOPDSP0_END 		0x1007FC
#define 	AUDPARARINGIODSP0_START 	0x100800
#define 	AUDPARARINGIODSP0_END	 	0x100BFC
#define 	DSP0LOCALHWREG_START 		0x100C00
#define 	DSP0LOCALHWREG_END	 	0x100C3C
#define 	DSP0XYRAMAGINDEX_START 		0x100C40
#define 	DSP0XYRAMAGINDEX_END	 	0x100C5C
#define 	DSP0XYRAMAGMDFR_START 		0x100C60
#define 	DSP0XYRAMAGMDFR_END	 	0x100C7C
#define 	DSP0INTCONTLVEC_START 		0x100C80
#define 	DSP0INTCONTLVEC_END	 	0x100CD8
#define 	INTCONTLGLOBALREG_START 	0x100D1C
#define 	INTCONTLGLOBALREG_END	 	0x100D3C
#define		HOSTINTFPORTADDRCONTDSP0	0x100D40
#define		HOSTINTFPORTDATADSP0		0x100D44
#define		TIME0PERENBDSP0			0x100D60
#define		TIME0COUNTERDSP0		0x100D64
#define		TIME1PERENBDSP0			0x100D68
#define		TIME1COUNTERDSP0		0x100D6C
#define		TIME2PERENBDSP0			0x100D70
#define		TIME2COUNTERDSP0		0x100D74
#define		TIME3PERENBDSP0			0x100D78
#define		TIME3COUNTERDSP0		0x100D7C
#define 	XRAMINDOPERREFNOUP_STARTDSP0 	0x100D80
#define 	XRAMINDOPERREFNOUP_ENDDSP0	0x100D9C
#define 	XRAMINDOPERREFUP_STARTDSP0	0x100DA0
#define 	XRAMINDOPERREFUP_ENDDSP0	0x100DBC
#define 	YRAMINDOPERREFNOUP_STARTDSP0 	0x100DC0
#define 	YRAMINDOPERREFNOUP_ENDDSP0 	0x100DDC
#define 	YRAMINDOPERREFUP_STARTDSP0	0x100DE0
#define 	YRAMINDOPERREFUP_ENDDSP0 	0x100DFC
#define 	DSP0CONDCODE 			0x100E00
#define 	DSP0STACKFLAG 			0x100E04
#define 	DSP0PROGCOUNTSTACKPTREG 	0x100E08
#define 	DSP0PROGCOUNTSTACKDATAREG 	0x100E0C
#define 	DSP0CURLOOPADDRREG 		0x100E10
#define 	DSP0CURLOOPCOUNT 		0x100E14
#define 	DSP0TOPLOOPCOUNTSTACK 		0x100E18
#define 	DSP0TOPLOOPADDRSTACK 		0x100E1C
#define 	DSP0LOOPSTACKPTR 		0x100E20
#define 	DSP0STASSTACKDATAREG 		0x100E24
#define 	DSP0STASSTACKPTR 		0x100E28
#define 	DSP0PROGCOUNT 			0x100E2C
#define  	GLOBDSPDEBGREG			0x100E30
#define  	GLOBDSPBREPTRREG		0x100E30
#define 	DSP0XYRAMBASE_START 		0x100EA0
#define 	DSP0XYRAMBASE_END 		0x100EBC
#define 	DSP0XYRAMLENG_START 		0x100EC0
#define 	DSP0XYRAMLENG_END 		0x100EDC
#define		SEMAPHOREREGDSP0		0x100EE0
#define		DSP0INTCONTMASKREG		0x100EE4
#define		DSP0INTCONTPENDREG		0x100EE8
#define		DSP0INTCONTSERVINT		0x100EEC
#define		DSPINTCONTEXTINTMODREG		0x100EEC
#define		GPIODSP0			0x100EFC
#define 	DMADSPBASEADDRREG_STARTDSP0	0x100F00
#define 	DMADSPBASEADDRREG_ENDDSP0	0x100F1C
#define 	DMAHOSTBASEADDRREG_STARTDSP0	0x100F20
#define 	DMAHOSTBASEADDRREG_ENDDSP0	0x100F3C
#define 	DMADSPCURADDRREG_STARTDSP0	0x100F40
#define 	DMADSPCURADDRREG_ENDDSP0	0x100F5C
#define 	DMAHOSTCURADDRREG_STARTDSP0	0x100F60
#define 	DMAHOSTCURADDRREG_ENDDSP0	0x100F7C
#define 	DMATANXCOUNTREG_STARTDSP0	0x100F80
#define 	DMATANXCOUNTREG_ENDDSP0		0x100F9C
#define 	DMATIMEBUGREG_STARTDSP0		0x100FA0
#define 	DMATIMEBUGREG_ENDDSP0		0x100FAC
#define 	DMACNTLMODFREG_STARTDSP0	0x100FA0
#define 	DMACNTLMODFREG_ENDDSP0		0x100FAC

#define 	DMAGLOBSTATSREGDSP0		0x100FEC
#define 	DSP0XGPRAM_START 		0x101000
#define 	DSP0XGPRAM_END 			0x1017FC
#define 	DSP0YGPRAM_START 		0x101800
#define 	DSP0YGPRAM_END 			0x101FFC




#define 	AUDIORINGIPDSP1_START 		0x102000
#define 	AUDIORINGIPDSP1_END	 	0x1023FC
#define 	AUDIORINGOPDSP1_START 		0x102400
#define 	AUDIORINGOPDSP1_END	 	0x1027FC
#define 	AUDPARARINGIODSP1_START 	0x102800
#define 	AUDPARARINGIODSP1_END	 	0x102BFC
#define 	DSP1LOCALHWREG_START 		0x102C00
#define 	DSP1LOCALHWREG_END	 	0x102C3C
#define 	DSP1XYRAMAGINDEX_START 		0x102C40
#define 	DSP1XYRAMAGINDEX_END	 	0x102C5C
#define 	DSP1XYRAMAGMDFR_START 		0x102C60
#define 	DSP1XYRAMAGMDFR_END	 	0x102C7C
#define 	DSP1INTCONTLVEC_START 		0x102C80
#define 	DSP1INTCONTLVEC_END	 	0x102CD8
#define		HOSTINTFPORTADDRCONTDSP1	0x102D40
#define		HOSTINTFPORTDATADSP1		0x102D44
#define		TIME0PERENBDSP1			0x102D60
#define		TIME0COUNTERDSP1		0x102D64
#define		TIME1PERENBDSP1			0x102D68
#define		TIME1COUNTERDSP1		0x102D6C
#define		TIME2PERENBDSP1			0x102D70
#define		TIME2COUNTERDSP1		0x102D74
#define		TIME3PERENBDSP1			0x102D78
#define		TIME3COUNTERDSP1		0x102D7C
#define 	XRAMINDOPERREFNOUP_STARTDSP1 	0x102D80
#define 	XRAMINDOPERREFNOUP_ENDDSP1	0x102D9C
#define 	XRAMINDOPERREFUP_STARTDSP1	0x102DA0
#define 	XRAMINDOPERREFUP_ENDDSP1	0x102DBC
#define 	YRAMINDOPERREFNOUP_STARTDSP1 	0x102DC0
#define 	YRAMINDOPERREFNOUP_ENDDSP1	0x102DDC
#define 	YRAMINDOPERREFUP_STARTDSP1	0x102DE0
#define 	YRAMINDOPERREFUP_ENDDSP1	0x102DFC

#define 	DSP1CONDCODE 			0x102E00
#define 	DSP1STACKFLAG 			0x102E04
#define 	DSP1PROGCOUNTSTACKPTREG 	0x102E08
#define 	DSP1PROGCOUNTSTACKDATAREG 	0x102E0C
#define 	DSP1CURLOOPADDRREG 		0x102E10
#define 	DSP1CURLOOPCOUNT 		0x102E14
#define 	DSP1TOPLOOPCOUNTSTACK 		0x102E18
#define 	DSP1TOPLOOPADDRSTACK 		0x102E1C
#define 	DSP1LOOPSTACKPTR 		0x102E20
#define 	DSP1STASSTACKDATAREG 		0x102E24
#define 	DSP1STASSTACKPTR 		0x102E28
#define 	DSP1PROGCOUNT 			0x102E2C
#define 	DSP1XYRAMBASE_START 		0x102EA0
#define 	DSP1XYRAMBASE_END 		0x102EBC
#define 	DSP1XYRAMLENG_START 		0x102EC0
#define 	DSP1XYRAMLENG_END 		0x102EDC
#define		SEMAPHOREREGDSP1		0x102EE0
#define		DSP1INTCONTMASKREG		0x102EE4
#define		DSP1INTCONTPENDREG		0x102EE8
#define		DSP1INTCONTSERVINT		0x102EEC
#define		GPIODSP1			0x102EFC
#define 	DMADSPBASEADDRREG_STARTDSP1	0x102F00
#define 	DMADSPBASEADDRREG_ENDDSP1	0x102F1C
#define 	DMAHOSTBASEADDRREG_STARTDSP1	0x102F20
#define 	DMAHOSTBASEADDRREG_ENDDSP1	0x102F3C
#define 	DMADSPCURADDRREG_STARTDSP1	0x102F40
#define 	DMADSPCURADDRREG_ENDDSP1	0x102F5C
#define 	DMAHOSTCURADDRREG_STARTDSP1	0x102F60
#define 	DMAHOSTCURADDRREG_ENDDSP1	0x102F7C
#define 	DMATANXCOUNTREG_STARTDSP1	0x102F80
#define 	DMATANXCOUNTREG_ENDDSP1		0x102F9C
#define 	DMATIMEBUGREG_STARTDSP1		0x102FA0
#define 	DMATIMEBUGREG_ENDDSP1		0x102FAC
#define 	DMACNTLMODFREG_STARTDSP1	0x102FA0
#define 	DMACNTLMODFREG_ENDDSP1		0x102FAC

#define 	DMAGLOBSTATSREGDSP1		0x102FEC
#define 	DSP1XGPRAM_START 		0x103000
#define 	DSP1XGPRAM_END 			0x1033FC
#define 	DSP1YGPRAM_START 		0x103400
#define 	DSP1YGPRAM_END 			0x1037FC



#define 	AUDIORINGIPDSP2_START 		0x104000
#define 	AUDIORINGIPDSP2_END	 	0x1043FC
#define 	AUDIORINGOPDSP2_START 		0x104400
#define 	AUDIORINGOPDSP2_END	 	0x1047FC
#define 	AUDPARARINGIODSP2_START 	0x104800
#define 	AUDPARARINGIODSP2_END	 	0x104BFC
#define 	DSP2LOCALHWREG_START 		0x104C00
#define 	DSP2LOCALHWREG_END	 	0x104C3C
#define 	DSP2XYRAMAGINDEX_START 		0x104C40
#define 	DSP2XYRAMAGINDEX_END	 	0x104C5C
#define 	DSP2XYRAMAGMDFR_START 		0x104C60
#define 	DSP2XYRAMAGMDFR_END		0x104C7C
#define 	DSP2INTCONTLVEC_START 		0x104C80
#define 	DSP2INTCONTLVEC_END	 	0x104CD8
#define		HOSTINTFPORTADDRCONTDSP2	0x104D40
#define		HOSTINTFPORTDATADSP2		0x104D44
#define		TIME0PERENBDSP2			0x104D60
#define		TIME0COUNTERDSP2		0x104D64
#define		TIME1PERENBDSP2			0x104D68
#define		TIME1COUNTERDSP2		0x104D6C
#define		TIME2PERENBDSP2			0x104D70
#define		TIME2COUNTERDSP2		0x104D74
#define		TIME3PERENBDSP2			0x104D78
#define		TIME3COUNTERDSP2		0x104D7C
#define 	XRAMINDOPERREFNOUP_STARTDSP2 	0x104D80
#define 	XRAMINDOPERREFNOUP_ENDDSP2	0x104D9C
#define 	XRAMINDOPERREFUP_STARTDSP2	0x104DA0
#define 	XRAMINDOPERREFUP_ENDDSP2	0x104DBC
#define 	YRAMINDOPERREFNOUP_STARTDSP2 	0x104DC0
#define 	YRAMINDOPERREFNOUP_ENDDSP2	0x104DDC
#define 	YRAMINDOPERREFUP_STARTDSP2	0x104DE0
#define 	YRAMINDOPERREFUP_ENDDSP2 	0x104DFC
#define 	DSP2CONDCODE 			0x104E00
#define 	DSP2STACKFLAG 			0x104E04
#define 	DSP2PROGCOUNTSTACKPTREG 	0x104E08
#define 	DSP2PROGCOUNTSTACKDATAREG 	0x104E0C
#define 	DSP2CURLOOPADDRREG 		0x104E10
#define 	DSP2CURLOOPCOUNT 		0x104E14
#define 	DSP2TOPLOOPCOUNTSTACK 		0x104E18
#define 	DSP2TOPLOOPADDRSTACK 		0x104E1C
#define 	DSP2LOOPSTACKPTR 		0x104E20
#define 	DSP2STASSTACKDATAREG 		0x104E24
#define 	DSP2STASSTACKPTR 		0x104E28
#define 	DSP2PROGCOUNT 			0x104E2C
#define 	DSP2XYRAMBASE_START 		0x104EA0
#define 	DSP2XYRAMBASE_END 		0x104EBC
#define 	DSP2XYRAMLENG_START 		0x104EC0
#define 	DSP2XYRAMLENG_END 		0x104EDC
#define		SEMAPHOREREGDSP2		0x104EE0
#define		DSP2INTCONTMASKREG		0x104EE4
#define		DSP2INTCONTPENDREG		0x104EE8
#define		DSP2INTCONTSERVINT		0x104EEC
#define		GPIODSP2			0x104EFC
#define 	DMADSPBASEADDRREG_STARTDSP2	0x104F00
#define 	DMADSPBASEADDRREG_ENDDSP2	0x104F1C
#define 	DMAHOSTBASEADDRREG_STARTDSP2	0x104F20
#define 	DMAHOSTBASEADDRREG_ENDDSP2	0x104F3C
#define 	DMADSPCURADDRREG_STARTDSP2	0x104F40
#define 	DMADSPCURADDRREG_ENDDSP2	0x104F5C
#define 	DMAHOSTCURADDRREG_STARTDSP2	0x104F60
#define 	DMAHOSTCURADDRREG_ENDDSP2	0x104F7C
#define 	DMATANXCOUNTREG_STARTDSP2	0x104F80
#define 	DMATANXCOUNTREG_ENDDSP2		0x104F9C
#define 	DMATIMEBUGREG_STARTDSP2		0x104FA0
#define 	DMATIMEBUGREG_ENDDSP2		0x104FAC
#define 	DMACNTLMODFREG_STARTDSP2	0x104FA0
#define 	DMACNTLMODFREG_ENDDSP2		0x104FAC

#define 	DMAGLOBSTATSREGDSP2		0x104FEC
#define 	DSP2XGPRAM_START 		0x105000
#define 	DSP2XGPRAM_END 			0x1051FC
#define 	DSP2YGPRAM_START 		0x105800
#define 	DSP2YGPRAM_END 			0x1059FC



#define 	AUDIORINGIPDSP3_START 		0x106000
#define 	AUDIORINGIPDSP3_END	 	0x1063FC
#define 	AUDIORINGOPDSP3_START 		0x106400
#define 	AUDIORINGOPDSP3_END	 	0x1067FC
#define 	AUDPARARINGIODSP3_START 	0x106800
#define 	AUDPARARINGIODSP3_END	 	0x106BFC
#define 	DSP3LOCALHWREG_START 		0x106C00
#define 	DSP3LOCALHWREG_END	 	0x106C3C
#define 	DSP3XYRAMAGINDEX_START 		0x106C40
#define 	DSP3XYRAMAGINDEX_END	 	0x106C5C
#define 	DSP3XYRAMAGMDFR_START 		0x106C60
#define 	DSP3XYRAMAGMDFR_END		0x106C7C
#define 	DSP3INTCONTLVEC_START 		0x106C80
#define 	DSP3INTCONTLVEC_END	 	0x106CD8
#define		HOSTINTFPORTADDRCONTDSP3	0x106D40
#define		HOSTINTFPORTDATADSP3		0x106D44
#define		TIME0PERENBDSP3			0x106D60
#define		TIME0COUNTERDSP3		0x106D64
#define		TIME1PERENBDSP3			0x106D68
#define		TIME1COUNTERDSP3		0x106D6C
#define		TIME2PERENBDSP3			0x106D70
#define		TIME2COUNTERDSP3		0x106D74
#define		TIME3PERENBDSP3			0x106D78
#define		TIME3COUNTERDSP3		0x106D7C
#define 	XRAMINDOPERREFNOUP_STARTDSP3 	0x106D80
#define 	XRAMINDOPERREFNOUP_ENDDSP3	0x106D9C
#define 	XRAMINDOPERREFUP_STARTDSP3	0x106DA0
#define 	XRAMINDOPERREFUP_ENDDSP3	0x106DBC
#define 	YRAMINDOPERREFNOUP_STARTDSP3 	0x106DC0
#define 	YRAMINDOPERREFNOUP_ENDDSP3	0x106DDC
#define 	YRAMINDOPERREFUP_STARTDSP3	0x106DE0
#define 	YRAMINDOPERREFUP_ENDDSP3	0x100DFC

#define 	DSP3CONDCODE 			0x106E00
#define 	DSP3STACKFLAG 			0x106E04
#define 	DSP3PROGCOUNTSTACKPTREG 	0x106E08
#define 	DSP3PROGCOUNTSTACKDATAREG 	0x106E0C
#define 	DSP3CURLOOPADDRREG 		0x106E10
#define 	DSP3CURLOOPCOUNT 		0x106E14
#define 	DSP3TOPLOOPCOUNTSTACK 		0x106E18
#define 	DSP3TOPLOOPADDRSTACK 		0x106E1C
#define 	DSP3LOOPSTACKPTR 		0x106E20
#define 	DSP3STASSTACKDATAREG 		0x106E24
#define 	DSP3STASSTACKPTR 		0x106E28
#define 	DSP3PROGCOUNT 			0x106E2C
#define 	DSP3XYRAMBASE_START 		0x106EA0
#define 	DSP3XYRAMBASE_END 		0x106EBC
#define 	DSP3XYRAMLENG_START 		0x106EC0
#define 	DSP3XYRAMLENG_END 		0x106EDC
#define		SEMAPHOREREGDSP3		0x106EE0
#define		DSP3INTCONTMASKREG		0x106EE4
#define		DSP3INTCONTPENDREG		0x106EE8
#define		DSP3INTCONTSERVINT		0x106EEC
#define		GPIODSP3			0x106EFC
#define 	DMADSPBASEADDRREG_STARTDSP3	0x106F00
#define 	DMADSPBASEADDRREG_ENDDSP3	0x106F1C
#define 	DMAHOSTBASEADDRREG_STARTDSP3	0x106F20
#define 	DMAHOSTBASEADDRREG_ENDDSP3	0x106F3C
#define 	DMADSPCURADDRREG_STARTDSP3	0x106F40
#define 	DMADSPCURADDRREG_ENDDSP3	0x106F5C
#define 	DMAHOSTCURADDRREG_STARTDSP3	0x106F60
#define 	DMAHOSTCURADDRREG_ENDDSP3	0x106F7C
#define 	DMATANXCOUNTREG_STARTDSP3	0x106F80
#define 	DMATANXCOUNTREG_ENDDSP3		0x106F9C
#define 	DMATIMEBUGREG_STARTDSP3		0x106FA0
#define 	DMATIMEBUGREG_ENDDSP3		0x106FAC
#define 	DMACNTLMODFREG_STARTDSP3	0x106FA0
#define 	DMACNTLMODFREG_ENDDSP3		0x106FAC

#define 	DMAGLOBSTATSREGDSP3		0x106FEC
#define 	DSP3XGPRAM_START 		0x107000
#define 	DSP3XGPRAM_END 			0x1071FC
#define 	DSP3YGPRAM_START 		0x107800
#define 	DSP3YGPRAM_END 			0x1079FC

/* end of DSP reg definitions */

#define  	DSPAIMAP_START			0x108000
#define  	DSPAIMAP_END			0x1083FC
#define  	DSPPIMAP_START			0x108400
#define  	DSPPIMAP_END			0x1087FC
#define  	DSPPOMAP_START			0x108800
#define  	DSPPOMAP_END			0x108BFC
#define  	DSPPOCTL			0x108C00
#define 	TKCTL_START			0x110000
#define 	TKCTL_END			0x110FFC
#define 	TKCC_START			0x111000
#define 	TKCC_END			0x111FFC
#define 	TKIMAP_START			0x112000
#define 	TKIMAP_END			0x112FFC
#define		TKDCTR16			0x113000
#define		TKPB16				0x113004
#define		TKBS16				0x113008
#define		TKDCTR32			0x11300C
#define		TKPB32				0x113010
#define		TKBS32				0x113014
#define		ICDCTR16			0x113018
#define		ITBS16				0x11301C
#define		ICDCTR32			0x113020
#define		ITBS32				0x113024
#define		ITSTART				0x113028
#define		TKSQ				0x11302C

#define		TKSCCTL_START			0x114000
#define		TKSCCTL_END			0x11403C
#define		TKSCADR_START			0x114100
#define		TKSCADR_END			0x11413C
#define		TKSCDATAX_START			0x114800
#define		TKSCDATAX_END			0x1149FC
#define		TKPCDATAX_START			0x120000
#define		TKPCDATAX_END			0x12FFFC

#define		MALSA				0x130000
#define		MAPPHA				0x130004
#define		MAPPLA				0x130008
#define		MALSB				0x130010
#define		MAPPHB				0x130014
#define		MAPPLB				0x130018

#define 	TANSPORTMAPABREGS_START		0x130020
#define 	TANSPORTMAPABREGS_END		0x13A2FC

#define		PTPAHX				0x13B000
#define		PTPALX				0x13B004

#define		TANSPPAGETABLEPHYADDR015_START	0x13B008
#define		TANSPPAGETABLEPHYADDR015_END	0x13B07C
#define		TRNQADRX_START			0x13B100
#define		TRNQADRX_END			0x13B13C
#define		TRNQTIMX_START			0x13B200
#define		TRNQTIMX_END			0x13B23C
#define		TRNQAPARMX_START		0x13B300
#define		TRNQAPARMX_END			0x13B33C

#define		TRNQCNT				0x13B400
#define		TRNCTL				0x13B404
#define		TRNIS				0x13B408
#define		TRNCURTS			0x13B40C

#define		AMOP_START			0x140000
#define		AMOPLO				0x140000
#define		AMOPHI				0x140004
#define		AMOP_END			0x147FFC
#define		PMOP_START			0x148000
#define		PMOPLO				0x148000
#define		PMOPHI				0x148004
#define		PMOP_END			0x14FFFC
#define		PCURR_START			0x150000
#define		PCURR_END			0x153FFC
#define		PTRAG_START			0x154000
#define		PTRAG_END			0x157FFC
#define		PSR_START			0x158000
#define		PSR_END				0x15BFFC

#define		PFSTAT4SEG_START		0x160000
#define		PFSTAT4SEG_END			0x160BFC
#define		PFSTAT2SEG_START		0x160C00
#define		PFSTAT2SEG_END			0x1617FC
#define		PFTARG4SEG_START		0x164000
#define		PFTARG4SEG_END			0x164BFC
#define		PFTARG2SEG_START		0x164C00
#define		PFTARG2SEG_END			0x1657FC
#define		PFSR4SEG_START			0x168000
#define		PFSR4SEG_END			0x168BFC
#define		PFSR2SEG_START			0x168C00
#define		PFSR2SEG_END			0x1697FC
#define		PCURRMS4SEG_START		0x16C000
#define		PCURRMS4SEG_END			0x16CCFC
#define		PCURRMS2SEG_START		0x16CC00
#define		PCURRMS2SEG_END			0x16D7FC
#define		PTARGMS4SEG_START		0x170000
#define		PTARGMS4SEG_END			0x172FFC
#define		PTARGMS2SEG_START		0x173000
#define		PTARGMS2SEG_END			0x1747FC
#define		PSRMS4SEG_START			0x170000
#define		PSRMS4SEG_END			0x172FFC
#define		PSRMS2SEG_START			0x173000
#define		PSRMS2SEG_END			0x1747FC

#define		PRING_LO_START			0x190000
#define		PRING_LO_END			0x193FFC
#define		PRING_HI_START			0x194000
#define		PRING_HI_END			0x197FFC
#define		PRING_LO_HI_START		0x198000
#define		PRING_LO_HI			0x198000
#define		PRING_LO_HI_END			0x19BFFC

#define		PINTFIFO			0x1A0000
#define		SRCCTL				0x1B0000
#define		SRCCCR				0x1B0004
#define		SRCIMAP				0x1B0008
#define		SRCODDC				0x1B000C
#define		SRCCA				0x1B0010
#define		SRCCF				0x1B0014
#define		SRCSA				0x1B0018
#define		SRCLA				0x1B001C
#define		SRCCTLSWR			0x1B0020

/* SRC HERE */
#define		SRCALBA				0x1B002C
#define		SRCMCTL				0x1B012C
#define		SRCCERR				0x1B022C
#define		SRCITB				0x1B032C
#define		SRCIPM				0x1B082C
#define		SRCIP				0x1B102C
#define		SRCENBSTAT			0x1B202C
#define		SRCENBLO			0x1B212C
#define		SRCENBHI			0x1B222C
#define		SRCENBS				0x1B232C
#define		SRCENB				0x1B282C
#define		SRCENB07			0x1B282C
#define		SRCENBS07			0x1B302C

#define		SRCDN0Z				0x1B0030
#define		SRCDN0Z0			0x1B0030
#define		SRCDN0Z1			0x1B0034
#define		SRCDN0Z2			0x1B0038
#define		SRCDN0Z3			0x1B003C
#define		SRCDN1Z				0x1B0040
#define		SRCDN1Z0			0x1B0040
#define		SRCDN1Z1			0x1B0044
#define		SRCDN1Z2			0x1B0048
#define		SRCDN1Z3			0x1B004C
#define		SRCDN1Z4			0x1B0050
#define		SRCDN1Z5			0x1B0054
#define		SRCDN1Z6			0x1B0058
#define		SRCDN1Z7			0x1B005C
#define		SRCUPZ				0x1B0060
#define		SRCUPZ0				0x1B0060
#define		SRCUPZ1				0x1B0064
#define		SRCUPZ2				0x1B0068
#define		SRCUPZ3				0x1B006C
#define		SRCUPZ4				0x1B0070
#define		SRCUPZ5				0x1B0074
#define		SRCUPZ6				0x1B0078
#define		SRCUPZ7				0x1B007C
#define		SRCCD0				0x1B0080
#define		SRCCD1				0x1B0084
#define		SRCCD2				0x1B0088
#define		SRCCD3				0x1B008C
#define		SRCCD4				0x1B0090
#define		SRCCD5				0x1B0094
#define		SRCCD6				0x1B0098
#define		SRCCD7				0x1B009C
#define		SRCCD8				0x1B00A0
#define		SRCCD9				0x1B00A4
#define		SRCCDA				0x1B00A8
#define		SRCCDB				0x1B00AC
#define		SRCCDC				0x1B00B0
#define		SRCCDD				0x1B00B4
#define		SRCCDE				0x1B00B8
#define		SRCCDF				0x1B00BC
#define		SRCCD10				0x1B00C0
#define		SRCCD11				0x1B00C4
#define		SRCCD12				0x1B00C8
#define		SRCCD13				0x1B00CC
#define		SRCCD14				0x1B00D0
#define		SRCCD15				0x1B00D4
#define		SRCCD16				0x1B00D8
#define		SRCCD17				0x1B00DC
#define		SRCCD18				0x1B00E0
#define		SRCCD19				0x1B00E4
#define		SRCCD1A				0x1B00E8
#define		SRCCD1B				0x1B00EC
#define		SRCCD1C				0x1B00F0
#define		SRCCD1D				0x1B00F4
#define		SRCCD1E				0x1B00F8
#define		SRCCD1F				0x1B00FC

#define		SRCCONTRBLOCK_START		0x1B0100
#define		SRCCONTRBLOCK_END		0x1BFFFC
#define		FILTOP_START	0x1C0000
#define		FILTOP_END	0x1C05FC
#define		FILTIMAP_START	0x1C0800
#define		FILTIMAP_END	0x1C0DFC
#define		FILTZ1_START	0x1C1000
#define		FILTZ1_END	0x1C15FC
#define		FILTZ2_START	0x1C1800
#define		FILTZ2_END	0x1C1DFC
#define		DAOIMAP_START	0x1C5000
#define		DAOIMAP		0x1C5000
#define		DAOIMAP_END	0x1C5124

#define		AC97D		0x1C5400
#define		AC97A		0x1C5404
#define		AC97CTL		0x1C5408
#define		I2SCTL		0x1C5420

#define		SPOS		0x1C5440
#define		SPOSA		0x1C5440
#define		SPOSB		0x1C5444
#define		SPOSC		0x1C5448
#define		SPOSD		0x1C544C

#define		SPISA		0x1C5450
#define		SPISB		0x1C5454
#define		SPISC		0x1C5458
#define		SPISD		0x1C545C

#define		SPFSCTL		0x1C5460

#define		SPFS0		0x1C5468
#define		SPFS1		0x1C546C
#define		SPFS2		0x1C5470
#define		SPFS3		0x1C5474
#define		SPFS4		0x1C5478
#define		SPFS5		0x1C547C

#define		SPOCTL		0x1C5480
#define		SPICTL		0x1C5484
#define		SPISTS		0x1C5488
#define		SPINTP		0x1C548C
#define		SPINTE		0x1C5490
#define		SPUTCTLAB	0x1C5494
#define		SPUTCTLCD	0x1C5498

#define		SRTSPA		0x1C54C0
#define		SRTSPB		0x1C54C4
#define		SRTSPC		0x1C54C8
#define		SRTSPD		0x1C54CC

#define		SRTSCTL		0x1C54D0
#define		SRTSCTLA	0x1C54D0
#define		SRTSCTLB	0x1C54D4
#define		SRTSCTLC	0x1C54D8
#define		SRTSCTLD	0x1C54DC

#define		SRTI2S		0x1C54E0
#define		SRTICTL		0x1C54F0

#define		WC		0x1C6000
#define		TIMR		0x1C6004
# define	TIMR_IE		(1<<15)
# define	TIMR_IP		(1<<14)

#define		GIP		0x1C6010
#define		GIE		0x1C6014
#define		DIE		0x1C6018
#define		DIC		0x1C601C
#define		GPIO		0x1C6020
#define		GPIOCTL		0x1C6024
#define		GPIP		0x1C6028
#define		GPIE		0x1C602C
#define		DSPINT0		0x1C6030
#define		DSPEIOC		0x1C6034
#define		MUADAT		0x1C6040
#define		MUACMD		0x1C6044
#define 	MUASTAT		0x1C6044
#define		MUBDAT		0x1C6048
#define		MUBCMD		0x1C604C
#define		MUBSTAT		0x1C604C
#define		UARTCMA		0x1C6050
#define		UARTCMB		0x1C6054
#define		UARTIP		0x1C6058
#define		UARTIE		0x1C605C
#define		PLLCTL		0x1C6060
#define		PLLDCD		0x1C6064
#define		GCTL		0x1C6070
#define		ID0		0x1C6080
#define		ID1		0x1C6084
#define		ID2		0x1C6088
#define		ID3		0x1C608C
#define		SDRCTL		0x1C7000


#define I2SA_L    0x0L
#define I2SA_R    0x1L
#define I2SB_L    0x8L
#define I2SB_R    0x9L
#define I2SC_L    0x10L
#define I2SC_R    0x11L
#define I2SD_L    0x18L
#define I2SD_R    0x19L

#endif /* CT20K1REG_H */
