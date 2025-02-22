/*
 * Copyright 2017 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */
#ifndef __DC_FEATURES_H__
#define __DC_FEATURES_H__

// local features
#define DC__PRESENT 1
#define DC__PRESENT__1 1
#define DC__NUM_DPP 4
<<<<<<< HEAD
#define DC__VOLTAGE_STATES 7
=======
#define DC__VOLTAGE_STATES 9
>>>>>>> upstream/android-13
#define DC__NUM_DPP__4 1
#define DC__NUM_DPP__0_PRESENT 1
#define DC__NUM_DPP__1_PRESENT 1
#define DC__NUM_DPP__2_PRESENT 1
#define DC__NUM_DPP__3_PRESENT 1
#define DC__NUM_DPP__MAX 8
#define DC__NUM_DPP__MAX__8 1
#define DC__PIPE_10BIT 0
#define DC__PIPE_10BIT__0 1
#define DC__PIPE_10BIT__MAX 1
#define DC__PIPE_10BIT__MAX__1 1
#define DC__NUM_OPP 4
#define DC__NUM_OPP__4 1
#define DC__NUM_OPP__0_PRESENT 1
#define DC__NUM_OPP__1_PRESENT 1
#define DC__NUM_OPP__2_PRESENT 1
#define DC__NUM_OPP__3_PRESENT 1
#define DC__NUM_OPP__MAX 6
#define DC__NUM_OPP__MAX__6 1
#define DC__NUM_DSC 0
#define DC__NUM_DSC__0 1
#define DC__NUM_DSC__MAX 6
#define DC__NUM_DSC__MAX__6 1
#define DC__NUM_ABM 1
#define DC__NUM_ABM__1 1
#define DC__NUM_ABM__0_PRESENT 1
#define DC__NUM_ABM__MAX 2
#define DC__NUM_ABM__MAX__2 1
#define DC__ODM_PRESENT 0
#define DC__ODM_PRESENT__0 1
#define DC__NUM_OTG 4
#define DC__NUM_OTG__4 1
#define DC__NUM_OTG__0_PRESENT 1
#define DC__NUM_OTG__1_PRESENT 1
#define DC__NUM_OTG__2_PRESENT 1
#define DC__NUM_OTG__3_PRESENT 1
#define DC__NUM_OTG__MAX 6
#define DC__NUM_OTG__MAX__6 1
#define DC__NUM_DWB 2
#define DC__NUM_DWB__2 1
#define DC__NUM_DWB__0_PRESENT 1
#define DC__NUM_DWB__1_PRESENT 1
#define DC__NUM_DWB__MAX 2
#define DC__NUM_DWB__MAX__2 1
#define DC__NUM_DIG 4
#define DC__NUM_DIG__4 1
#define DC__NUM_DIG__0_PRESENT 1
#define DC__NUM_DIG__1_PRESENT 1
#define DC__NUM_DIG__2_PRESENT 1
#define DC__NUM_DIG__3_PRESENT 1
#define DC__NUM_DIG__MAX 6
#define DC__NUM_DIG__MAX__6 1
#define DC__NUM_AUX 4
#define DC__NUM_AUX__4 1
#define DC__NUM_AUX__0_PRESENT 1
#define DC__NUM_AUX__1_PRESENT 1
#define DC__NUM_AUX__2_PRESENT 1
#define DC__NUM_AUX__3_PRESENT 1
#define DC__NUM_AUX__MAX 6
#define DC__NUM_AUX__MAX__6 1
#define DC__NUM_AUDIO_STREAMS 4
#define DC__NUM_AUDIO_STREAMS__4 1
#define DC__NUM_AUDIO_STREAMS__0_PRESENT 1
#define DC__NUM_AUDIO_STREAMS__1_PRESENT 1
#define DC__NUM_AUDIO_STREAMS__2_PRESENT 1
#define DC__NUM_AUDIO_STREAMS__3_PRESENT 1
#define DC__NUM_AUDIO_STREAMS__MAX 8
#define DC__NUM_AUDIO_STREAMS__MAX__8 1
#define DC__NUM_AUDIO_ENDPOINTS 6
#define DC__NUM_AUDIO_ENDPOINTS__6 1
#define DC__NUM_AUDIO_ENDPOINTS__0_PRESENT 1
#define DC__NUM_AUDIO_ENDPOINTS__1_PRESENT 1
#define DC__NUM_AUDIO_ENDPOINTS__2_PRESENT 1
#define DC__NUM_AUDIO_ENDPOINTS__3_PRESENT 1
#define DC__NUM_AUDIO_ENDPOINTS__4_PRESENT 1
#define DC__NUM_AUDIO_ENDPOINTS__5_PRESENT 1
#define DC__NUM_AUDIO_ENDPOINTS__MAX 8
#define DC__NUM_AUDIO_ENDPOINTS__MAX__8 1
#define DC__NUM_AUDIO_INPUT_STREAMS 0
#define DC__NUM_AUDIO_INPUT_STREAMS__0 1
#define DC__NUM_AUDIO_INPUT_STREAMS__MAX 8
#define DC__NUM_AUDIO_INPUT_STREAMS__MAX__8 1
#define DC__NUM_AUDIO_INPUT_ENDPOINTS 0
#define DC__NUM_AUDIO_INPUT_ENDPOINTS__0 1
#define DC__NUM_AUDIO_INPUT_ENDPOINTS__MAX 8
#define DC__NUM_AUDIO_INPUT_ENDPOINTS__MAX__8 1
#define DC__NUM_CURSOR 1
#define DC__NUM_CURSOR__1 1
#define DC__NUM_CURSOR__0_PRESENT 1
#define DC__NUM_CURSOR__MAX 2
#define DC__NUM_CURSOR__MAX__2 1
#define DC__DIGITAL_BYPASS_PRESENT 0
#define DC__DIGITAL_BYPASS_PRESENT__0 1
#define DC__HCID_HWMAJVER 1
#define DC__HCID_HWMAJVER__1 1
#define DC__HCID_HWMINVER 0
#define DC__HCID_HWMINVER__0 1
#define DC__HCID_HWREV 0
#define DC__HCID_HWREV__0 1
#define DC__ROMSTRAP_PRESENT 0
#define DC__ROMSTRAP_PRESENT__0 1
#define DC__NUM_RBBMIF_DECODES 30
#define DC__NUM_RBBMIF_DECODES__30 1
#define DC__NUM_DBG_REGS 36
#define DC__NUM_DBG_REGS__36 1
#define DC__NUM_PIPES_UNDERLAY 0
#define DC__NUM_PIPES_UNDERLAY__0 1
#define DC__NUM_PIPES_UNDERLAY__MAX 2
#define DC__NUM_PIPES_UNDERLAY__MAX__2 1
#define DC__NUM_VCE_ENGINE 1
#define DC__NUM_VCE_ENGINE__1 1
#define DC__NUM_VCE_ENGINE__0_PRESENT 1
#define DC__NUM_VCE_ENGINE__MAX 2
#define DC__NUM_VCE_ENGINE__MAX__2 1
#define DC__OTG_EXTERNAL_SYNC_PRESENT 0
#define DC__OTG_EXTERNAL_SYNC_PRESENT__0 1
#define DC__OTG_CRC_PRESENT 1
#define DC__OTG_CRC_PRESENT__1 1
#define DC__VIP_PRESENT 0
#define DC__VIP_PRESENT__0 1
#define DC__DTMTEST_PRESENT 0
#define DC__DTMTEST_PRESENT__0 1
#define DC__POWER_GATE_PRESENT 1
#define DC__POWER_GATE_PRESENT__1 1
#define DC__MEM_PG 1
#define DC__MEM_PG__1 1
#define DC__FMT_SRC_SEL_PRESENT 0
#define DC__FMT_SRC_SEL_PRESENT__0 1
#define DC__DIG_FEATURES__HDMI_PRESENT 1
#define DC__DIG_FEATURES__HDMI_PRESENT__1 1
#define DC__DIG_FEATURES__DP_PRESENT 1
#define DC__DIG_FEATURES__DP_PRESENT__1 1
#define DC__DIG_FEATURES__DP_MST_PRESENT 1
#define DC__DIG_FEATURES__DP_MST_PRESENT__1 1
#define DC__DIG_LP_FEATURES__HDMI_PRESENT 0
#define DC__DIG_LP_FEATURES__HDMI_PRESENT__0 1
#define DC__DIG_LP_FEATURES__DP_PRESENT 1
#define DC__DIG_LP_FEATURES__DP_PRESENT__1 1
#define DC__DIG_LP_FEATURES__DP_MST_PRESENT 0
#define DC__DIG_LP_FEATURES__DP_MST_PRESENT__0 1
#define DC__DIG_RESYNC_FIFO_SIZE 14
#define DC__DIG_RESYNC_FIFO_SIZE__14 1
#define DC__DIG_RESYNC_FIFO_SIZE__0_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__1_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__2_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__3_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__4_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__5_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__6_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__7_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__8_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__9_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__10_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__11_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__12_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__13_PRESENT 1
#define DC__DIG_RESYNC_FIFO_SIZE__MAX 16
#define DC__DIG_RESYNC_FIFO_SIZE__MAX__16 1
#define DC__DAC_RESYNC_FIFO_SIZE 12
#define DC__DAC_RESYNC_FIFO_SIZE__12 1
#define DC__DAC_RESYNC_FIFO_SIZE__0_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__1_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__2_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__3_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__4_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__5_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__6_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__7_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__8_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__9_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__10_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__11_PRESENT 1
#define DC__DAC_RESYNC_FIFO_SIZE__MAX 16
#define DC__DAC_RESYNC_FIFO_SIZE__MAX__16 1
#define DC__DVO_RESYNC_FIFO_SIZE 12
#define DC__DVO_RESYNC_FIFO_SIZE__12 1
#define DC__DVO_RESYNC_FIFO_SIZE__0_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__1_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__2_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__3_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__4_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__5_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__6_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__7_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__8_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__9_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__10_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__11_PRESENT 1
#define DC__DVO_RESYNC_FIFO_SIZE__MAX 16
#define DC__DVO_RESYNC_FIFO_SIZE__MAX__16 1
#define DC__MEM_CDC_PRESENT 1
#define DC__MEM_CDC_PRESENT__1 1
#define DC__NUM_HPD 4
#define DC__NUM_HPD__4 1
#define DC__NUM_HPD__0_PRESENT 1
#define DC__NUM_HPD__1_PRESENT 1
#define DC__NUM_HPD__2_PRESENT 1
#define DC__NUM_HPD__3_PRESENT 1
#define DC__NUM_HPD__MAX 6
#define DC__NUM_HPD__MAX__6 1
#define DC__NUM_DDC_PAIRS 4
#define DC__NUM_DDC_PAIRS__4 1
#define DC__NUM_DDC_PAIRS__0_PRESENT 1
#define DC__NUM_DDC_PAIRS__1_PRESENT 1
#define DC__NUM_DDC_PAIRS__2_PRESENT 1
#define DC__NUM_DDC_PAIRS__3_PRESENT 1
#define DC__NUM_DDC_PAIRS__MAX 6
#define DC__NUM_DDC_PAIRS__MAX__6 1
#define DC__NUM_AUDIO_PLL 0
#define DC__NUM_AUDIO_PLL__0 1
#define DC__NUM_AUDIO_PLL__MAX 2
#define DC__NUM_AUDIO_PLL__MAX__2 1
#define DC__NUM_PIXEL_PLL 1
#define DC__NUM_PIXEL_PLL__1 1
#define DC__NUM_PIXEL_PLL__0_PRESENT 1
#define DC__NUM_PIXEL_PLL__MAX 4
#define DC__NUM_PIXEL_PLL__MAX__4 1
#define DC__NUM_CASCADED_PLL 0
#define DC__NUM_CASCADED_PLL__0 1
#define DC__NUM_CASCADED_PLL__MAX 3
#define DC__NUM_CASCADED_PLL__MAX__3 1
#define DC__PIXCLK_FROM_PHYPLL 1
#define DC__PIXCLK_FROM_PHYPLL__1 1
#define DC__NB_STUTTER_MODE_PRESENT 0
#define DC__NB_STUTTER_MODE_PRESENT__0 1
#define DC__I2S0_AND_SPDIF0_PRESENT 0
#define DC__I2S0_AND_SPDIF0_PRESENT__0 1
#define DC__I2S1_PRESENT 0
#define DC__I2S1_PRESENT__0 1
#define DC__SPDIF1_PRESENT 0
#define DC__SPDIF1_PRESENT__0 1
#define DC__DSI_PRESENT 0
#define DC__DSI_PRESENT__0 1
#define DC__DACA_PRESENT 0
#define DC__DACA_PRESENT__0 1
#define DC__DACB_PRESENT 0
#define DC__DACB_PRESENT__0 1
#define DC__NUM_PIPES 4
#define DC__NUM_PIPES__4 1
#define DC__NUM_PIPES__0_PRESENT 1
#define DC__NUM_PIPES__1_PRESENT 1
#define DC__NUM_PIPES__2_PRESENT 1
#define DC__NUM_PIPES__3_PRESENT 1
#define DC__NUM_PIPES__MAX 6
#define DC__NUM_PIPES__MAX__6 1
#define DC__NUM_DIG_LP 0
#define DC__NUM_DIG_LP__0 1
#define DC__NUM_DIG_LP__MAX 2
#define DC__NUM_DIG_LP__MAX__2 1
#define DC__DPDEBUG_PRESENT 0
#define DC__DPDEBUG_PRESENT__0 1
#define DC__DISPLAY_WB_PRESENT 1
#define DC__DISPLAY_WB_PRESENT__1 1
#define DC__NUM_CWB 0
#define DC__NUM_CWB__0 1
#define DC__NUM_CWB__MAX 2
#define DC__NUM_CWB__MAX__2 1
#define DC__MVP_PRESENT 0
#define DC__MVP_PRESENT__0 1
#define DC__DVO_PRESENT 0
#define DC__DVO_PRESENT__0 1
#define DC__ABM_PRESENT 0
#define DC__ABM_PRESENT__0 1
#define DC__BPHYC_PLL_PRESENT 0
#define DC__BPHYC_PLL_PRESENT__0 1
#define DC__BPHYC_UNIPHY_PRESENT 0
#define DC__BPHYC_UNIPHY_PRESENT__0 1
#define DC__PHY_BROADCAST_PRESENT 0
#define DC__PHY_BROADCAST_PRESENT__0 1
#define DC__NUM_OF_DCRX_SD 0
#define DC__NUM_OF_DCRX_SD__0 1
#define DC__DVO_17BIT_MAPPING 0
#define DC__DVO_17BIT_MAPPING__0 1
#define DC__AVSYNC_PRESENT 0
#define DC__AVSYNC_PRESENT__0 1
#define DC__NUM_OF_DCRX_PORTS 0
#define DC__NUM_OF_DCRX_PORTS__0 1
#define DC__NUM_OF_DCRX_PORTS__MAX 1
#define DC__NUM_OF_DCRX_PORTS__MAX__1 1
#define DC__NUM_PHY 4
#define DC__NUM_PHY__4 1
#define DC__NUM_PHY__0_PRESENT 1
#define DC__NUM_PHY__1_PRESENT 1
#define DC__NUM_PHY__2_PRESENT 1
#define DC__NUM_PHY__3_PRESENT 1
#define DC__NUM_PHY__MAX 7
#define DC__NUM_PHY__MAX__7 1
#define DC__NUM_PHY_LP 0
#define DC__NUM_PHY_LP__0 1
#define DC__NUM_PHY_LP__MAX 2
#define DC__NUM_PHY_LP__MAX__2 1
#define DC__SYNC_CELL vid_sync_gf14lpp
#define DC__SYNC_CELL__VID_SYNC_GF14LPP 1
#define DC__USE_NEW_VSS 1
#define DC__USE_NEW_VSS__1 1
#define DC__SYNC_CELL_DISPCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_DISPCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_DVOCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_DVOCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_PIXCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_PIXCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_SYMCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_SYMCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_DPPCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_DPPCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_DPREFCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_DPREFCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_REFCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_REFCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_PCIE_REFCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_PCIE_REFCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_MVPCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_MVPCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_SCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_SCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_DCEFCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_DCEFCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_AMCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_AMCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_DSICLK_NUM_LATCHES 6
#define DC__SYNC_CELL_DSICLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_BYTECLK_NUM_LATCHES 6
#define DC__SYNC_CELL_BYTECLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_ESCCLK_NUM_LATCHES 6
#define DC__SYNC_CELL_ESCCLK_NUM_LATCHES__6 1
#define DC__SYNC_CELL_DB_CLK_NUM_LATCHES 6
#define DC__SYNC_CELL_DB_CLK_NUM_LATCHES__6 1
#define UNIPHYA_PRESENT 1
#define UNIPHYA_PRESENT__1 1
#define DC__UNIPHYA_PRESENT 1
#define DC__UNIPHYA_PRESENT__1 1
#define UNIPHYB_PRESENT 1
#define UNIPHYB_PRESENT__1 1
#define DC__UNIPHYB_PRESENT 1
#define DC__UNIPHYB_PRESENT__1 1
#define UNIPHYC_PRESENT 1
#define UNIPHYC_PRESENT__1 1
#define DC__UNIPHYC_PRESENT 1
#define DC__UNIPHYC_PRESENT__1 1
#define UNIPHYD_PRESENT 1
#define UNIPHYD_PRESENT__1 1
#define DC__UNIPHYD_PRESENT 1
#define DC__UNIPHYD_PRESENT__1 1
#define UNIPHYE_PRESENT 0
#define UNIPHYE_PRESENT__0 1
#define DC__UNIPHYE_PRESENT 0
#define DC__UNIPHYE_PRESENT__0 1
#define UNIPHYF_PRESENT 0
#define UNIPHYF_PRESENT__0 1
#define DC__UNIPHYF_PRESENT 0
#define DC__UNIPHYF_PRESENT__0 1
#define UNIPHYG_PRESENT 0
#define UNIPHYG_PRESENT__0 1
#define DC__UNIPHYG_PRESENT 0
#define DC__UNIPHYG_PRESENT__0 1
#define DC__TMDS_LINK tmds_link_dual
#define DC__TMDS_LINK__TMDS_LINK_DUAL 1
#define DC__WBSCL_PIXBW 8
#define DC__WBSCL_PIXBW__8 1
#define DC__DWB_CSC_PRESENT 0
#define DC__DWB_CSC_PRESENT__0 1
#define DC__DWB_LUMA_SCL_PRESENT 0
#define DC__DWB_LUMA_SCL_PRESENT__0 1
#define DC__DENTIST_INTERFACE_PRESENT 1
#define DC__DENTIST_INTERFACE_PRESENT__1 1
#define DC__GENERICA_PRESENT 1
#define DC__GENERICA_PRESENT__1 1
#define DC__GENERICB_PRESENT 1
#define DC__GENERICB_PRESENT__1 1
#define DC__GENERICC_PRESENT 0
#define DC__GENERICC_PRESENT__0 1
#define DC__GENERICD_PRESENT 0
#define DC__GENERICD_PRESENT__0 1
#define DC__GENERICE_PRESENT 0
#define DC__GENERICE_PRESENT__0 1
#define DC__GENERICF_PRESENT 0
#define DC__GENERICF_PRESENT__0 1
#define DC__GENERICG_PRESENT 0
#define DC__GENERICG_PRESENT__0 1
#define DC__UNIPHY_VOLTAGE_MODE 1
#define DC__UNIPHY_VOLTAGE_MODE__1 1
#define DC__BLON_TYPE dedicated
#define DC__BLON_TYPE__DEDICATED 1
#define DC__UNIPHY_STAGGER_CH_PRESENT 1
#define DC__UNIPHY_STAGGER_CH_PRESENT__1 1
#define DC__XDMA_PRESENT 0
#define DC__XDMA_PRESENT__0 1
#define XDMA__PRESENT 0
#define XDMA__PRESENT__0 1
#define DC__DP_MEM_PG 0
#define DC__DP_MEM_PG__0 1
#define DP__MEM_PG 0
#define DP__MEM_PG__0 1
#define DC__AFMT_MEM_PG 0
#define DC__AFMT_MEM_PG__0 1
#define AFMT__MEM_PG 0
#define AFMT__MEM_PG__0 1
#define DC__HDMI_MEM_PG 0
#define DC__HDMI_MEM_PG__0 1
#define HDMI__MEM_PG 0
#define HDMI__MEM_PG__0 1
#define DC__I2C_MEM_PG 0
#define DC__I2C_MEM_PG__0 1
#define I2C__MEM_PG 0
#define I2C__MEM_PG__0 1
#define DC__DSCL_MEM_PG 0
#define DC__DSCL_MEM_PG__0 1
#define DSCL__MEM_PG 0
#define DSCL__MEM_PG__0 1
#define DC__CM_MEM_PG 0
#define DC__CM_MEM_PG__0 1
#define CM__MEM_PG 0
#define CM__MEM_PG__0 1
#define DC__OBUF_MEM_PG 0
#define DC__OBUF_MEM_PG__0 1
#define OBUF__MEM_PG 0
#define OBUF__MEM_PG__0 1
#define DC__WBIF_MEM_PG 1
#define DC__WBIF_MEM_PG__1 1
#define WBIF__MEM_PG 1
#define WBIF__MEM_PG__1 1
#define DC__VGA_MEM_PG 0
#define DC__VGA_MEM_PG__0 1
#define VGA__MEM_PG 0
#define VGA__MEM_PG__0 1
#define DC__FMT_MEM_PG 0
#define DC__FMT_MEM_PG__0 1
#define FMT__MEM_PG 0
#define FMT__MEM_PG__0 1
#define DC__ODM_MEM_PG 0
#define DC__ODM_MEM_PG__0 1
#define ODM__MEM_PG 0
#define ODM__MEM_PG__0 1
#define DC__DSI_MEM_PG 0
#define DC__DSI_MEM_PG__0 1
#define DSI__MEM_PG 0
#define DSI__MEM_PG__0 1
#define DC__AZ_MEM_PG 1
#define DC__AZ_MEM_PG__1 1
#define AZ__MEM_PG 1
#define AZ__MEM_PG__1 1
#define DC__WBSCL_MEM1P1024X64QS_MEM_PG 1
#define DC__WBSCL_MEM1P1024X64QS_MEM_PG__1 1
#define WBSCL_MEM1P1024X64QS__MEM_PG 1
#define WBSCL_MEM1P1024X64QS__MEM_PG__1 1
#define DC__WBSCL_MEM1P528X64QS_MEM_PG 1
#define DC__WBSCL_MEM1P528X64QS_MEM_PG__1 1
#define WBSCL_MEM1P528X64QS__MEM_PG 1
#define WBSCL_MEM1P528X64QS__MEM_PG__1 1
#define DC__DMCU_MEM1P1024X32BQS_MEM_PG 1
#define DC__DMCU_MEM1P1024X32BQS_MEM_PG__1 1
#define DMCU_MEM1P1024X32BQS__MEM_PG 1
#define DMCU_MEM1P1024X32BQS__MEM_PG__1 1
#define DC__HUBBUB_SDP_TAG_INT_MEM_PG 0
#define DC__HUBBUB_SDP_TAG_INT_MEM_PG__0 1
#define HUBBUB_SDP_TAG_INT__MEM_PG 0
#define HUBBUB_SDP_TAG_INT__MEM_PG__0 1
#define DC__HUBBUB_SDP_TAG_EXT_MEM_PG 0
#define DC__HUBBUB_SDP_TAG_EXT_MEM_PG__0 1
#define HUBBUB_SDP_TAG_EXT__MEM_PG 0
#define HUBBUB_SDP_TAG_EXT__MEM_PG__0 1
#define DC__HUBBUB_RET_ZERO_MEM_PG 0
#define DC__HUBBUB_RET_ZERO_MEM_PG__0 1
#define HUBBUB_RET_ZERO__MEM_PG 0
#define HUBBUB_RET_ZERO__MEM_PG__0 1
#define DC__HUBBUB_RET_ROB_MEM_PG 0
#define DC__HUBBUB_RET_ROB_MEM_PG__0 1
#define HUBBUB_RET_ROB__MEM_PG 0
#define HUBBUB_RET_ROB__MEM_PG__0 1
#define DC__HUBPRET_CUR_ROB_MEM_PG 0
#define DC__HUBPRET_CUR_ROB_MEM_PG__0 1
#define HUBPRET_CUR_ROB__MEM_PG 0
#define HUBPRET_CUR_ROB__MEM_PG__0 1
#define DC__HUBPRET_CUR_CDC_MEM_PG 0
#define DC__HUBPRET_CUR_CDC_MEM_PG__0 1
#define HUBPRET_CUR_CDC__MEM_PG 0
#define HUBPRET_CUR_CDC__MEM_PG__0 1
#define DC__HUBPREQ_MPTE_MEM_PG 0
#define DC__HUBPREQ_MPTE_MEM_PG__0 1
#define HUBPREQ_MPTE__MEM_PG 0
#define HUBPREQ_MPTE__MEM_PG__0 1
#define DC__HUBPREQ_META_MEM_PG 0
#define DC__HUBPREQ_META_MEM_PG__0 1
#define HUBPREQ_META__MEM_PG 0
#define HUBPREQ_META__MEM_PG__0 1
#define DC__HUBPREQ_DPTE_MEM_PG 0
#define DC__HUBPREQ_DPTE_MEM_PG__0 1
#define HUBPREQ_DPTE__MEM_PG 0
#define HUBPREQ_DPTE__MEM_PG__0 1
#define DC__HUBPRET_DET_MEM_PG 0
#define DC__HUBPRET_DET_MEM_PG__0 1
#define HUBPRET_DET__MEM_PG 0
#define HUBPRET_DET__MEM_PG__0 1
#define DC__HUBPRET_PIX_CDC_MEM_PG 0
#define DC__HUBPRET_PIX_CDC_MEM_PG__0 1
#define HUBPRET_PIX_CDC__MEM_PG 0
#define HUBPRET_PIX_CDC__MEM_PG__0 1
#define DC__TOP_BLKS__DCCG 1
#define DC__TOP_BLKS__DCHUBBUB 1
#define DC__TOP_BLKS__DCHUBP 1
#define DC__TOP_BLKS__HDA 1
#define DC__TOP_BLKS__DIO 1
#define DC__TOP_BLKS__DCIO 1
#define DC__TOP_BLKS__DMU 1
#define DC__TOP_BLKS__DPP 1
#define DC__TOP_BLKS__MPC 1
#define DC__TOP_BLKS__OPP 1
#define DC__TOP_BLKS__OPTC 1
#define DC__TOP_BLKS__MMHUBBUB 1
#define DC__TOP_BLKS__WB 1
#define DC__TOP_BLKS__MAX 13
#define DC__TOP_BLKS__MAX__13 1
#define DC__DCHUBP_DPP_SF_PIXEL_CREDITS 9
#define DC__DCHUBP_DPP_SF_PIXEL_CREDITS__9 1
#define DC__DPP_MPC_SF_PIXEL_CREDITS 9
#define DC__DPP_MPC_SF_PIXEL_CREDITS__9 1
#define DC__MPC_OPP_SF_PIXEL_CREDITS 8
#define DC__MPC_OPP_SF_PIXEL_CREDITS__8 1
#define DC__OPP_OPTC_SF_PIXEL_CREDITS 8
#define DC__OPP_OPTC_SF_PIXEL_CREDITS__8 1
#define DC__SFR_SFT_ROUND_TRIP_DELAY 5
#define DC__SFR_SFT_ROUND_TRIP_DELAY__5 1
#define DC__REPEATER_PROJECT_MAX 8
#define DC__REPEATER_PROJECT_MAX__8 1
#define DC__SURFACE_422_CAPABLE 0
#define DC__SURFACE_422_CAPABLE__0 1
#endif
