

#ifndef TFA_SRC_TFA_EXT_H_
#define TFA_SRC_TFA_EXT_H_



#define LVM_MAXENUM (0xffff)

enum tfadsp_event_en {
	TFADSP_CMD_ACK            =  1,   
	TFADSP_SOFT_MUTE_READY    =  8,   
	TFADSP_VOLUME_READY       = 16,   
	TFADSP_DAMAGED_SPEAKER    = 32,   
	TFADSP_CALIBRATE_DONE     = 64,   
	TFADSP_SPARSESIG_DETECTED = 128,  
	TFADSP_CMD_READY          = 256,  
	TFADSP_EXT_PWRUP          = 0x8000, 
	TFADSP_EXT_PWRDOWN        = 0x8001, 
	TFADSP_EVENT_DUMMY        = LVM_MAXENUM
};

typedef int (*dsp_send_message_t)(void *tfa,
	int length, const char *buf);
typedef int (*dsp_read_message_t)(void *tfa,
	int length, unsigned char *buf);
typedef int (*tfa_event_handler_t)(enum tfadsp_event_en tfadsp_event);

int tfa_ext_register(dsp_send_message_t tfa_send_message,
	dsp_read_message_t tfa_read_message,
	tfa_event_handler_t *tfa_event_handler);

enum tfa98xx_blackbox_id {
	ID_MAXX_LOG = 0,
	ID_MAXT_LOG = 1,
	ID_OVERXMAX_COUNT = 2,
	ID_OVERTMAX_COUNT = 3,
	ID_MAXX_KEEP_LOG = 4,
	ID_MAXT_KEEP_LOG = 5,
	ID_BLACKBOX_MAX
};

int tfa98xx_set_blackbox(int enable);
int tfa98xx_get_blackbox_data(int dev, int *data);
int tfa98xx_get_blackbox_data_index(int dev, int index, int reset);
int tfa98xx_get_blackbox_data_index_channel(int channel,
	int index, int reset);

int tfa98xx_update_spkt_data(int idx);
int tfa98xx_update_spkt_data_channel(int channel);
int tfa98xx_write_sknt_control(int idx, int value);
int tfa98xx_write_sknt_control_channel(int channel, int value);

#endif 
