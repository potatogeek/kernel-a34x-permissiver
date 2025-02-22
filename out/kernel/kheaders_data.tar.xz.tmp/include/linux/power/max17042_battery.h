

#ifndef __MAX17042_BATTERY_H_
#define __MAX17042_BATTERY_H_

#define MAX17042_STATUS_BattAbsent	(1 << 3)
#define MAX17042_BATTERY_FULL		(95)   
#define MAX17042_DEFAULT_SNS_RESISTOR	(10000)
#define MAX17042_DEFAULT_VMIN		(3000)
#define MAX17042_DEFAULT_VMAX		(4500) 
#define MAX17042_DEFAULT_TEMP_MIN	(0)    
#define MAX17042_DEFAULT_TEMP_MAX	(700)  


#define MAX17042_FULL_THRESHOLD		10

#define MAX17042_CHARACTERIZATION_DATA_SIZE 48

enum max17042_register {
	MAX17042_STATUS		= 0x00,
	MAX17042_VALRT_Th	= 0x01,
	MAX17042_TALRT_Th	= 0x02,
	MAX17042_SALRT_Th	= 0x03,
	MAX17042_AtRate		= 0x04,
	MAX17042_RepCap		= 0x05,
	MAX17042_RepSOC		= 0x06,
	MAX17042_Age		= 0x07,
	MAX17042_TEMP		= 0x08,
	MAX17042_VCELL		= 0x09,
	MAX17042_Current	= 0x0A,
	MAX17042_AvgCurrent	= 0x0B,

	MAX17042_SOC		= 0x0D,
	MAX17042_AvSOC		= 0x0E,
	MAX17042_RemCap		= 0x0F,
	MAX17042_FullCAP	= 0x10,
	MAX17042_TTE		= 0x11,
	MAX17042_V_empty	= 0x12,

	MAX17042_RSLOW		= 0x14,

	MAX17042_AvgTA		= 0x16,
	MAX17042_Cycles		= 0x17,
	MAX17042_DesignCap	= 0x18,
	MAX17042_AvgVCELL	= 0x19,
	MAX17042_MinMaxTemp	= 0x1A,
	MAX17042_MinMaxVolt	= 0x1B,
	MAX17042_MinMaxCurr	= 0x1C,
	MAX17042_CONFIG		= 0x1D,
	MAX17042_ICHGTerm	= 0x1E,
	MAX17042_AvCap		= 0x1F,
	MAX17042_ManName	= 0x20,
	MAX17042_DevName	= 0x21,

	MAX17042_FullCAPNom	= 0x23,
	MAX17042_TempNom	= 0x24,
	MAX17042_TempLim	= 0x25,
	MAX17042_TempHot	= 0x26,
	MAX17042_AIN		= 0x27,
	MAX17042_LearnCFG	= 0x28,
	MAX17042_FilterCFG	= 0x29,
	MAX17042_RelaxCFG	= 0x2A,
	MAX17042_MiscCFG	= 0x2B,
	MAX17042_TGAIN		= 0x2C,
	MAx17042_TOFF		= 0x2D,
	MAX17042_CGAIN		= 0x2E,
	MAX17042_COFF		= 0x2F,

	MAX17042_MaskSOC	= 0x32,
	MAX17042_SOC_empty	= 0x33,
	MAX17042_T_empty	= 0x34,

	MAX17042_FullCAP0       = 0x35,
	MAX17042_LAvg_empty	= 0x36,
	MAX17042_FCTC		= 0x37,
	MAX17042_RCOMP0		= 0x38,
	MAX17042_TempCo		= 0x39,
	MAX17042_EmptyTempCo	= 0x3A,
	MAX17042_K_empty0	= 0x3B,
	MAX17042_TaskPeriod	= 0x3C,
	MAX17042_FSTAT		= 0x3D,

	MAX17042_SHDNTIMER	= 0x3F,

	MAX17042_dQacc		= 0x45,
	MAX17042_dPacc		= 0x46,

	MAX17042_VFSOC0		= 0x48,

	MAX17042_QH		= 0x4D,
	MAX17042_QL		= 0x4E,

	MAX17042_VFSOC0Enable	= 0x60,
	MAX17042_MLOCKReg1	= 0x62,
	MAX17042_MLOCKReg2	= 0x63,

	MAX17042_MODELChrTbl	= 0x80,

	MAX17042_OCV		= 0xEE,

	MAX17042_OCVInternal	= 0xFB,

	MAX17042_VFSOC		= 0xFF,
};


enum max17047_register {
	MAX17047_QRTbl00	= 0x12,
	MAX17047_FullSOCThr	= 0x13,
	MAX17047_QRTbl10	= 0x22,
	MAX17047_QRTbl20	= 0x32,
	MAX17047_V_empty	= 0x3A,
	MAX17047_QRTbl30	= 0x42,
};

enum max170xx_chip_type {
	MAXIM_DEVICE_TYPE_UNKNOWN	= 0,
	MAXIM_DEVICE_TYPE_MAX17042,
	MAXIM_DEVICE_TYPE_MAX17047,
	MAXIM_DEVICE_TYPE_MAX17050,

	MAXIM_DEVICE_TYPE_NUM
};


struct max17042_reg_data {
	u8 addr;
	u16 data;
};

struct max17042_config_data {
	
	u32	cur_sense_val;

	
	u16	tgain;		
	u16	toff;		
	u16	cgain;		
	u16	coff;		

	
	u16	valrt_thresh;	
	u16	talrt_thresh;	
	u16	soc_alrt_thresh;	
	u16	config;		
	u16	shdntimer;	

	
	u16	full_soc_thresh;	
	u16	design_cap;	
	u16	ichgt_term;	

	
	u16	at_rate;	
	u16	learn_cfg;	
	u16	filter_cfg;	
	u16	relax_cfg;	
	u16	misc_cfg;	
	u16	masksoc;	

	
	u16	fullcap;	
	u16	fullcapnom;	
	u16	socempty;	
	u16	lavg_empty;	
	u16	dqacc;		
	u16	dpacc;		
	u16	qrtbl00;	
	u16	qrtbl10;	
	u16	qrtbl20;	
	u16	qrtbl30;	

	
	u16	cell_technology;

	
	u16	vempty;		
	u16	temp_nom;	
	u16	temp_lim;	
	u16	fctc;		
	u16	rcomp0;		
	u16	tcompc0;	
	u16	empty_tempco;	
	u16	kempty0;	
	u16	cell_char_tbl[MAX17042_CHARACTERIZATION_DATA_SIZE];
} __packed;

struct max17042_platform_data {
	struct max17042_reg_data *init_data;
	struct max17042_config_data *config_data;
	int num_init_data; 
	bool enable_current_sense;
	bool enable_por_init; 

	
	unsigned int r_sns;
	int         vmin;	
	int         vmax;	
	int         temp_min;	
	int         temp_max;	
};

#endif 
