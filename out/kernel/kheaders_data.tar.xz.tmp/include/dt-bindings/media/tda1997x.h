/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _DT_BINDINGS_MEDIA_TDA1997X_H
#define _DT_BINDINGS_MEDIA_TDA1997X_H


#define TDA1997X_VP36_35_32	0
#define TDA1997X_VP36_31_28	1
#define TDA1997X_VP36_27_24	2
#define TDA1997X_VP36_23_20	3
#define TDA1997X_VP36_19_16	4
#define TDA1997X_VP36_15_12	5
#define TDA1997X_VP36_11_08	6
#define TDA1997X_VP36_07_04	7
#define TDA1997X_VP36_03_00	8


#define TDA1997X_VP24_V23_20	0
#define TDA1997X_VP24_V19_16	1
#define TDA1997X_VP24_V15_12	3
#define TDA1997X_VP24_V11_08	4
#define TDA1997X_VP24_V07_04	6
#define TDA1997X_VP24_V03_00	7


#define TDA1997X_VP_OUT_EN        0x80	
#define TDA1997X_VP_HIZ           0x40	
#define TDA1997X_VP_SWP           0x10	
#define TDA1997X_R_CR_CBCR_3_0    (0 | TDA1997X_VP_OUT_EN | TDA1997X_VP_HIZ)
#define TDA1997X_R_CR_CBCR_7_4    (1 | TDA1997X_VP_OUT_EN | TDA1997X_VP_HIZ)
#define TDA1997X_R_CR_CBCR_11_8   (2 | TDA1997X_VP_OUT_EN | TDA1997X_VP_HIZ)
#define TDA1997X_B_CB_3_0         (3 | TDA1997X_VP_OUT_EN | TDA1997X_VP_HIZ)
#define TDA1997X_B_CB_7_4         (4 | TDA1997X_VP_OUT_EN | TDA1997X_VP_HIZ)
#define TDA1997X_B_CB_11_8        (5 | TDA1997X_VP_OUT_EN | TDA1997X_VP_HIZ)
#define TDA1997X_G_Y_3_0          (6 | TDA1997X_VP_OUT_EN | TDA1997X_VP_HIZ)
#define TDA1997X_G_Y_7_4          (7 | TDA1997X_VP_OUT_EN | TDA1997X_VP_HIZ)
#define TDA1997X_G_Y_11_8         (8 | TDA1997X_VP_OUT_EN | TDA1997X_VP_HIZ)

#define TDA1997X_R_CR_CBCR_3_0_S  (TDA1997X_R_CR_CBCR_3_0 | TDA1997X_VP_SWAP)
#define TDA1997X_R_CR_CBCR_7_4_S  (TDA1997X_R_CR_CBCR_7_4 | TDA1997X_VP_SWAP)
#define TDA1997X_R_CR_CBCR_11_8_S (TDA1997X_R_CR_CBCR_11_8 | TDA1997X_VP_SWAP)
#define TDA1997X_B_CB_3_0_S       (TDA1997X_B_CB_3_0 | TDA1997X_VP_SWAP)
#define TDA1997X_B_CB_7_4_S       (TDA1997X_B_CB_7_4 | TDA1997X_VP_SWAP)
#define TDA1997X_B_CB_11_8_S      (TDA1997X_B_CB_11_8 | TDA1997X_VP_SWAP)
#define TDA1997X_G_Y_3_0_S        (TDA1997X_G_Y_3_0 | TDA1997X_VP_SWAP)
#define TDA1997X_G_Y_7_4_S        (TDA1997X_G_Y_7_4 | TDA1997X_VP_SWAP)
#define TDA1997X_G_Y_11_8_S       (TDA1997X_G_Y_11_8 | TDA1997X_VP_SWAP)


#define TDA1997X_I2S16			1 
#define TDA1997X_I2S32			2 
#define TDA1997X_SPDIF			3 
#define TDA1997X_OBA			4 
#define TDA1997X_DST			5 
#define TDA1997X_I2S16_HBR		6 
#define TDA1997X_I2S16_HBR_DEMUX	7 
#define TDA1997X_I2S32_HBR_DEMUX	8 
#define TDA1997X_SPDIF_HBR_DEMUX	9 


#define TDA1997X_LAYOUT0	0	
#define TDA1997X_LAYOUT1	1	


#define TDA1997X_ACLK_16FS	0
#define TDA1997X_ACLK_32FS	1
#define TDA1997X_ACLK_64FS	2
#define TDA1997X_ACLK_128FS	3
#define TDA1997X_ACLK_256FS	4
#define TDA1997X_ACLK_512FS	5

#endif 
