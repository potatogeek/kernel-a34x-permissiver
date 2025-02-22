



#define CIRRUS_BD_MAX_TEMP		0x28003a4
#define CIRRUS_BD_MAX_EXC		0x28003a8
#define CIRRUS_BD_OVER_TEMP_COUNT	0x28003ac
#define CIRRUS_BD_OVER_EXC_COUNT	0x28003b0
#define CIRRUS_BD_ABNORMAL_MUTE	        0x28003b4

#define CIRRUS_BD_TEMP_RADIX		14
#define CIRRUS_BD_EXC_RADIX		19

#define CIRRUS_BD_NUM_ATTRS_BASE	1
#define CIRRUS_BD_NUM_ATTRS_AMP		7

void cirrus_bd_amp_err(const char *mfd_suffix);
void cirrus_bd_bst_short(const char *mfd_suffix);

void cirrus_bd_store_values(const char *mfd_suffix);
int cirrus_bd_init(void);
void cirrus_bd_exit(void);
