// SPDX-License-Identifier: GPL-2.0


#ifndef __LINUX_USB_AUDIO_V2_H
#define __LINUX_USB_AUDIO_V2_H

#include <linux/types.h>





static inline bool uac_v2v3_control_is_readable(u32 bmControls, u8 control)
{
	return (bmControls >> ((control - 1) * 2)) & 0x1;
}

static inline bool uac_v2v3_control_is_writeable(u32 bmControls, u8 control)
{
	return (bmControls >> ((control - 1) * 2)) & 0x2;
}


struct uac2_ac_header_descriptor {
	__u8  bLength;			
	__u8  bDescriptorType;		
	__u8  bDescriptorSubtype;	
	__le16 bcdADC;			
	__u8  bCategory;
	__le16 wTotalLength;		
	__u8  bmControls;
} __packed;


struct uac2_format_type_i_descriptor {
	__u8  bLength;			
	__u8  bDescriptorType;		
	__u8  bDescriptorSubtype;	
	__u8  bFormatType;		
	__u8  bSubslotSize;		
	__u8  bBitResolution;
} __packed;



struct uac_clock_source_descriptor {
	__u8 bLength;
	__u8 bDescriptorType;
	__u8 bDescriptorSubtype;
	__u8 bClockID;
	__u8 bmAttributes;
	__u8 bmControls;
	__u8 bAssocTerminal;
	__u8 iClockSource;
} __attribute__((packed));


#define UAC_CLOCK_SOURCE_TYPE_EXT	0x0
#define UAC_CLOCK_SOURCE_TYPE_INT_FIXED	0x1
#define UAC_CLOCK_SOURCE_TYPE_INT_VAR	0x2
#define UAC_CLOCK_SOURCE_TYPE_INT_PROG	0x3
#define UAC_CLOCK_SOURCE_SYNCED_TO_SOF	(1 << 2)



struct uac_clock_selector_descriptor {
	__u8 bLength;
	__u8 bDescriptorType;
	__u8 bDescriptorSubtype;
	__u8 bClockID;
	__u8 bNrInPins;
	__u8 baCSourceID[];
	
} __attribute__((packed));



struct uac_clock_multiplier_descriptor {
	__u8 bLength;
	__u8 bDescriptorType;
	__u8 bDescriptorSubtype;
	__u8 bClockID;
	__u8 bCSourceID;
	__u8 bmControls;
	__u8 iClockMultiplier;
} __attribute__((packed));



struct uac2_input_terminal_descriptor {
	__u8 bLength;
	__u8 bDescriptorType;
	__u8 bDescriptorSubtype;
	__u8 bTerminalID;
	__le16 wTerminalType;
	__u8 bAssocTerminal;
	__u8 bCSourceID;
	__u8 bNrChannels;
	__le32 bmChannelConfig;
	__u8 iChannelNames;
	__le16 bmControls;
	__u8 iTerminal;
} __attribute__((packed));



struct uac2_output_terminal_descriptor {
	__u8 bLength;
	__u8 bDescriptorType;
	__u8 bDescriptorSubtype;
	__u8 bTerminalID;
	__le16 wTerminalType;
	__u8 bAssocTerminal;
	__u8 bSourceID;
	__u8 bCSourceID;
	__le16 bmControls;
	__u8 iTerminal;
} __attribute__((packed));





struct uac2_feature_unit_descriptor {
	__u8 bLength;
	__u8 bDescriptorType;
	__u8 bDescriptorSubtype;
	__u8 bUnitID;
	__u8 bSourceID;
	
	__u8 bmaControls[0]; 
} __attribute__((packed));



struct uac2_as_header_descriptor {
	__u8 bLength;
	__u8 bDescriptorType;
	__u8 bDescriptorSubtype;
	__u8 bTerminalLink;
	__u8 bmControls;
	__u8 bFormatType;
	__le32 bmFormats;
	__u8 bNrChannels;
	__le32 bmChannelConfig;
	__u8 iChannelNames;
} __attribute__((packed));

#define UAC2_FORMAT_TYPE_I_RAW_DATA	(1 << 31)



struct uac2_iso_endpoint_descriptor {
	__u8  bLength;			
	__u8  bDescriptorType;		
	__u8  bDescriptorSubtype;	
	__u8  bmAttributes;
	__u8  bmControls;
	__u8  bLockDelayUnits;
	__le16 wLockDelay;
} __attribute__((packed));

#define UAC2_CONTROL_PITCH		(3 << 0)
#define UAC2_CONTROL_DATA_OVERRUN	(3 << 2)
#define UAC2_CONTROL_DATA_UNDERRUN	(3 << 4)


struct uac2_connectors_ctl_blk {
	__u8 bNrChannels;
	__le32 bmChannelConfig;
	__u8 iChannelNames;
} __attribute__((packed));



#define UAC2_INTERRUPT_DATA_MSG_VENDOR	(1 << 0)
#define UAC2_INTERRUPT_DATA_MSG_EP	(1 << 1)

struct uac2_interrupt_data_msg {
	__u8 bInfo;
	__u8 bAttribute;
	__le16 wValue;
	__le16 wIndex;
} __attribute__((packed));


#define UAC2_FUNCTION_SUBCLASS_UNDEFINED	0x00
#define UAC2_FUNCTION_DESKTOP_SPEAKER		0x01
#define UAC2_FUNCTION_HOME_THEATER		0x02
#define UAC2_FUNCTION_MICROPHONE		0x03
#define UAC2_FUNCTION_HEADSET			0x04
#define UAC2_FUNCTION_TELEPHONE			0x05
#define UAC2_FUNCTION_CONVERTER			0x06
#define UAC2_FUNCTION_SOUND_RECORDER		0x07
#define UAC2_FUNCTION_IO_BOX			0x08
#define UAC2_FUNCTION_MUSICAL_INSTRUMENT	0x09
#define UAC2_FUNCTION_PRO_AUDIO			0x0a
#define UAC2_FUNCTION_AUDIO_VIDEO		0x0b
#define UAC2_FUNCTION_CONTROL_PANEL		0x0c
#define UAC2_FUNCTION_OTHER			0xff



#define UAC2_EFFECT_UNIT			0x07
#define UAC2_PROCESSING_UNIT_V2		0x08
#define UAC2_EXTENSION_UNIT_V2		0x09
#define UAC2_CLOCK_SOURCE		0x0a
#define UAC2_CLOCK_SELECTOR		0x0b
#define UAC2_CLOCK_MULTIPLIER		0x0c
#define UAC2_SAMPLE_RATE_CONVERTER	0x0d



#define UAC2_ENCODER			0x03
#define UAC2_DECODER			0x04


#define UAC2_EFFECT_UNDEFINED		0x00
#define UAC2_EFFECT_PARAM_EQ		0x01
#define UAC2_EFFECT_REVERB		0x02
#define UAC2_EFFECT_MOD_DELAY		0x03
#define UAC2_EFFECT_DYN_RANGE_COMP	0x04


#define UAC2_PROCESS_UNDEFINED		0x00
#define UAC2_PROCESS_UP_DOWNMIX		0x01
#define UAC2_PROCESS_DOLBY_PROLOCIC	0x02
#define UAC2_PROCESS_STEREO_EXTENDER	0x03


#define UAC2_CS_CUR			0x01
#define UAC2_CS_RANGE			0x02
#define UAC2_CS_MEM			0x03


#define UAC2_ENCODER_UNDEFINED		0x00
#define UAC2_ENCODER_OTHER		0x01
#define UAC2_ENCODER_MPEG		0x02
#define UAC2_ENCODER_AC3		0x03
#define UAC2_ENCODER_WMA		0x04
#define UAC2_ENCODER_DTS		0x05


#define UAC2_DECODER_UNDEFINED		0x00
#define UAC2_DECODER_OTHER		0x01
#define UAC2_DECODER_MPEG		0x02
#define UAC2_DECODER_AC3		0x03
#define UAC2_DECODER_WMA		0x04
#define UAC2_DECODER_DTS		0x05


#define UAC2_CS_UNDEFINED		0x00
#define UAC2_CS_CONTROL_SAM_FREQ	0x01
#define UAC2_CS_CONTROL_CLOCK_VALID	0x02


#define UAC2_CX_UNDEFINED		0x00
#define UAC2_CX_CLOCK_SELECTOR		0x01


#define UAC2_CM_UNDEFINED		0x00
#define UAC2_CM_NUMERATOR		0x01
#define UAC2_CM_DENOMINTATOR		0x02


#define UAC2_TE_UNDEFINED		0x00
#define UAC2_TE_COPY_PROTECT		0x01
#define UAC2_TE_CONNECTOR		0x02
#define UAC2_TE_OVERLOAD		0x03
#define UAC2_TE_CLUSTER			0x04
#define UAC2_TE_UNDERFLOW		0x05
#define UAC2_TE_OVERFLOW		0x06
#define UAC2_TE_LATENCY			0x07


#define UAC2_MU_UNDEFINED		0x00
#define UAC2_MU_MIXER			0x01
#define UAC2_MU_CLUSTER			0x02
#define UAC2_MU_UNDERFLOW		0x03
#define UAC2_MU_OVERFLOW		0x04
#define UAC2_MU_LATENCY			0x05


#define UAC2_SU_UNDEFINED		0x00
#define UAC2_SU_SELECTOR		0x01
#define UAC2_SU_LATENCY			0x02



#define UAC2_FU_INPUT_GAIN		0x0b
#define UAC2_FU_INPUT_GAIN_PAD		0x0c
#define UAC2_FU_PHASE_INVERTER		0x0d
#define UAC2_FU_UNDERFLOW		0x0e
#define UAC2_FU_OVERFLOW		0x0f
#define UAC2_FU_LATENCY			0x10


#define UAC2_PE_UNDEFINED		0x00
#define UAC2_PE_ENABLE			0x01
#define UAC2_PE_CENTERFREQ		0x02
#define UAC2_PE_QFACTOR			0x03
#define UAC2_PE_GAIN			0x04
#define UAC2_PE_UNDERFLOW		0x05
#define UAC2_PE_OVERFLOW		0x06
#define UAC2_PE_LATENCY			0x07


#define UAC2_RV_UNDEFINED		0x00
#define UAC2_RV_ENABLE			0x01
#define UAC2_RV_TYPE			0x02
#define UAC2_RV_LEVEL			0x03
#define UAC2_RV_TIME			0x04
#define UAC2_RV_FEEDBACK		0x05
#define UAC2_RV_PREDELAY		0x06
#define UAC2_RV_DENSITY			0x07
#define UAC2_RV_HIFREQ_ROLLOFF		0x08
#define UAC2_RV_UNDERFLOW		0x09
#define UAC2_RV_OVERFLOW		0x0a
#define UAC2_RV_LATENCY			0x0b


#define UAC2_MD_UNDEFINED		0x00
#define UAC2_MD_ENABLE			0x01
#define UAC2_MD_BALANCE			0x02
#define UAC2_MD_RATE			0x03
#define UAC2_MD_DEPTH			0x04
#define UAC2_MD_TIME			0x05
#define UAC2_MD_FEEDBACK		0x06
#define UAC2_MD_UNDERFLOW		0x07
#define UAC2_MD_OVERFLOW		0x08
#define UAC2_MD_LATENCY			0x09


#define UAC2_DR_UNDEFINED		0x00
#define UAC2_DR_ENABLE			0x01
#define UAC2_DR_COMPRESSION_RATE	0x02
#define UAC2_DR_MAXAMPL			0x03
#define UAC2_DR_THRESHOLD		0x04
#define UAC2_DR_ATTACK_TIME		0x05
#define UAC2_DR_RELEASE_TIME		0x06
#define UAC2_DR_UNDEFLOW		0x07
#define UAC2_DR_OVERFLOW		0x08
#define UAC2_DR_LATENCY			0x09


#define UAC2_UD_UNDEFINED		0x00
#define UAC2_UD_ENABLE			0x01
#define UAC2_UD_MODE_SELECT		0x02
#define UAC2_UD_CLUSTER			0x03
#define UAC2_UD_UNDERFLOW		0x04
#define UAC2_UD_OVERFLOW		0x05
#define UAC2_UD_LATENCY			0x06


#define UAC2_DP_UNDEFINED		0x00
#define UAC2_DP_ENABLE			0x01
#define UAC2_DP_MODE_SELECT		0x02
#define UAC2_DP_CLUSTER			0x03
#define UAC2_DP_UNDERFFLOW		0x04
#define UAC2_DP_OVERFLOW		0x05
#define UAC2_DP_LATENCY			0x06


#define UAC2_ST_EXT_UNDEFINED		0x00
#define UAC2_ST_EXT_ENABLE		0x01
#define UAC2_ST_EXT_WIDTH		0x02
#define UAC2_ST_EXT_UNDEFLOW		0x03
#define UAC2_ST_EXT_OVERFLOW		0x04
#define UAC2_ST_EXT_LATENCY		0x05


#define UAC2_XU_UNDEFINED		0x00
#define UAC2_XU_ENABLE			0x01
#define UAC2_XU_CLUSTER			0x02
#define UAC2_XU_UNDERFLOW		0x03
#define UAC2_XU_OVERFLOW		0x04
#define UAC2_XU_LATENCY			0x05


#define UAC2_AS_UNDEFINED		0x00
#define UAC2_AS_ACT_ALT_SETTING		0x01
#define UAC2_AS_VAL_ALT_SETTINGS	0x02
#define UAC2_AS_AUDIO_DATA_FORMAT	0x03


#define UAC2_EN_UNDEFINED		0x00
#define UAC2_EN_BIT_RATE		0x01
#define UAC2_EN_QUALITY			0x02
#define UAC2_EN_VBR			0x03
#define UAC2_EN_TYPE			0x04
#define UAC2_EN_UNDERFLOW		0x05
#define UAC2_EN_OVERFLOW		0x06
#define UAC2_EN_ENCODER_ERROR		0x07
#define UAC2_EN_PARAM1			0x08
#define UAC2_EN_PARAM2			0x09
#define UAC2_EN_PARAM3			0x0a
#define UAC2_EN_PARAM4			0x0b
#define UAC2_EN_PARAM5			0x0c
#define UAC2_EN_PARAM6			0x0d
#define UAC2_EN_PARAM7			0x0e
#define UAC2_EN_PARAM8			0x0f


#define UAC2_MPEG_UNDEFINED		0x00
#define UAC2_MPEG_DUAL_CHANNEL		0x01
#define UAC2_MPEG_SECOND_STEREO		0x02
#define UAC2_MPEG_MULTILINGUAL		0x03
#define UAC2_MPEG_DYN_RANGE		0x04
#define UAC2_MPEG_SCALING		0x05
#define UAC2_MPEG_HILO_SCALING		0x06
#define UAC2_MPEG_UNDERFLOW		0x07
#define UAC2_MPEG_OVERFLOW		0x08
#define UAC2_MPEG_DECODER_ERROR		0x09


#define UAC2_AC3_UNDEFINED		0x00
#define UAC2_AC3_MODE			0x01
#define UAC2_AC3_DYN_RANGE		0x02
#define UAC2_AC3_SCALING		0x03
#define UAC2_AC3_HILO_SCALING		0x04
#define UAC2_AC3_UNDERFLOW		0x05
#define UAC2_AC3_OVERFLOW		0x06
#define UAC2_AC3_DECODER_ERROR		0x07


#define UAC2_WMA_UNDEFINED		0x00
#define UAC2_WMA_UNDERFLOW		0x01
#define UAC2_WMA_OVERFLOW		0x02
#define UAC2_WMA_DECODER_ERROR		0x03


#define UAC2_DTS_UNDEFINED		0x00
#define UAC2_DTS_UNDERFLOW		0x01
#define UAC2_DTS_OVERFLOW		0x02
#define UAC2_DTS_DECODER_ERROR		0x03


#define UAC2_EP_CS_UNDEFINED		0x00
#define UAC2_EP_CS_PITCH		0x01
#define UAC2_EP_CS_DATA_OVERRUN		0x02
#define UAC2_EP_CS_DATA_UNDERRUN	0x03

#endif 

