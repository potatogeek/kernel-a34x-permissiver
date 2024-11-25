

#ifndef __CROS_EC_COMMANDS_H
#define __CROS_EC_COMMANDS_H


#define EC_PROTO_VERSION          0x00000002


#define EC_VER_MASK(version) (1UL << (version))


#define EC_LPC_ADDR_ACPI_DATA  0x62
#define EC_LPC_ADDR_ACPI_CMD   0x66


#define EC_LPC_ADDR_HOST_DATA  0x200
#define EC_LPC_ADDR_HOST_CMD   0x204



#define EC_LPC_ADDR_HOST_ARGS    0x800  
#define EC_LPC_ADDR_HOST_PARAM   0x804  

#define EC_LPC_ADDR_HOST_PACKET  0x800  
#define EC_LPC_HOST_PACKET_SIZE  0x100  


#define EC_HOST_CMD_REGION0    0x800
#define EC_HOST_CMD_REGION1    0x880
#define EC_HOST_CMD_REGION_SIZE 0x80


#define EC_LPC_CMDR_DATA	(1 << 0)  
#define EC_LPC_CMDR_PENDING	(1 << 1)  
#define EC_LPC_CMDR_BUSY	(1 << 2)  
#define EC_LPC_CMDR_CMD		(1 << 3)  
#define EC_LPC_CMDR_ACPI_BRST	(1 << 4)  
#define EC_LPC_CMDR_SCI		(1 << 5)  
#define EC_LPC_CMDR_SMI		(1 << 6)  

#define EC_LPC_ADDR_MEMMAP       0x900
#define EC_MEMMAP_SIZE         255 
#define EC_MEMMAP_TEXT_MAX     8   


#define EC_MEMMAP_TEMP_SENSOR      0x00 
#define EC_MEMMAP_FAN              0x10 
#define EC_MEMMAP_TEMP_SENSOR_B    0x18 
#define EC_MEMMAP_ID               0x20 
#define EC_MEMMAP_ID_VERSION       0x22 
#define EC_MEMMAP_THERMAL_VERSION  0x23 
#define EC_MEMMAP_BATTERY_VERSION  0x24 
#define EC_MEMMAP_SWITCHES_VERSION 0x25 
#define EC_MEMMAP_EVENTS_VERSION   0x26 
#define EC_MEMMAP_HOST_CMD_FLAGS   0x27 

#define EC_MEMMAP_SWITCHES         0x30	

#define EC_MEMMAP_HOST_EVENTS      0x34 


#define EC_MEMMAP_BATT_VOLT        0x40 
#define EC_MEMMAP_BATT_RATE        0x44 
#define EC_MEMMAP_BATT_CAP         0x48 
#define EC_MEMMAP_BATT_FLAG        0x4c 
#define EC_MEMMAP_BATT_DCAP        0x50 
#define EC_MEMMAP_BATT_DVLT        0x54 
#define EC_MEMMAP_BATT_LFCC        0x58 
#define EC_MEMMAP_BATT_CCNT        0x5c 

#define EC_MEMMAP_BATT_MFGR        0x60 
#define EC_MEMMAP_BATT_MODEL       0x68 
#define EC_MEMMAP_BATT_SERIAL      0x70 
#define EC_MEMMAP_BATT_TYPE        0x78 
#define EC_MEMMAP_ALS              0x80 

#define EC_MEMMAP_ACC_STATUS       0x90 

#define EC_MEMMAP_ACC_DATA         0x92 
#define EC_MEMMAP_GYRO_DATA        0xa0 




#define EC_MEMMAP_ACC_STATUS_SAMPLE_ID_MASK  0x0f
#define EC_MEMMAP_ACC_STATUS_BUSY_BIT        (1 << 4)
#define EC_MEMMAP_ACC_STATUS_PRESENCE_BIT    (1 << 7)


#define EC_TEMP_SENSOR_ENTRIES     16

#define EC_TEMP_SENSOR_B_ENTRIES      8


#define EC_TEMP_SENSOR_NOT_PRESENT    0xff
#define EC_TEMP_SENSOR_ERROR          0xfe
#define EC_TEMP_SENSOR_NOT_POWERED    0xfd
#define EC_TEMP_SENSOR_NOT_CALIBRATED 0xfc

#define EC_TEMP_SENSOR_OFFSET      200


#define EC_ALS_ENTRIES             2


#define EC_TEMP_SENSOR_DEFAULT     (296 - EC_TEMP_SENSOR_OFFSET)

#define EC_FAN_SPEED_ENTRIES       4       
#define EC_FAN_SPEED_NOT_PRESENT   0xffff  
#define EC_FAN_SPEED_STALLED       0xfffe  


#define EC_BATT_FLAG_AC_PRESENT   0x01
#define EC_BATT_FLAG_BATT_PRESENT 0x02
#define EC_BATT_FLAG_DISCHARGING  0x04
#define EC_BATT_FLAG_CHARGING     0x08
#define EC_BATT_FLAG_LEVEL_CRITICAL 0x10


#define EC_SWITCH_LID_OPEN               0x01
#define EC_SWITCH_POWER_BUTTON_PRESSED   0x02
#define EC_SWITCH_WRITE_PROTECT_DISABLED 0x04

#define EC_SWITCH_IGNORE1		 0x08

#define EC_SWITCH_DEDICATED_RECOVERY     0x10

#define EC_SWITCH_IGNORE0                0x20



#define EC_HOST_CMD_FLAG_LPC_ARGS_SUPPORTED  0x01

#define EC_HOST_CMD_FLAG_VERSION_3   0x02


#define EC_WIRELESS_SWITCH_ALL       ~0x00  
#define EC_WIRELESS_SWITCH_WLAN       0x01  
#define EC_WIRELESS_SWITCH_BLUETOOTH  0x02  
#define EC_WIRELESS_SWITCH_WWAN       0x04  
#define EC_WIRELESS_SWITCH_WLAN_POWER 0x08  


#ifndef __ACPI__


#ifndef __packed
#define __packed __attribute__((packed))
#endif



#define EC_LPC_STATUS_TO_HOST     0x01

#define EC_LPC_STATUS_FROM_HOST   0x02

#define EC_LPC_STATUS_PROCESSING  0x04

#define EC_LPC_STATUS_LAST_CMD    0x08

#define EC_LPC_STATUS_BURST_MODE  0x10

#define EC_LPC_STATUS_SCI_PENDING 0x20

#define EC_LPC_STATUS_SMI_PENDING 0x40

#define EC_LPC_STATUS_RESERVED    0x80


#define EC_LPC_STATUS_BUSY_MASK \
	(EC_LPC_STATUS_FROM_HOST | EC_LPC_STATUS_PROCESSING)


enum ec_status {
	EC_RES_SUCCESS = 0,
	EC_RES_INVALID_COMMAND = 1,
	EC_RES_ERROR = 2,
	EC_RES_INVALID_PARAM = 3,
	EC_RES_ACCESS_DENIED = 4,
	EC_RES_INVALID_RESPONSE = 5,
	EC_RES_INVALID_VERSION = 6,
	EC_RES_INVALID_CHECKSUM = 7,
	EC_RES_IN_PROGRESS = 8,		
	EC_RES_UNAVAILABLE = 9,		
	EC_RES_TIMEOUT = 10,		
	EC_RES_OVERFLOW = 11,		
	EC_RES_INVALID_HEADER = 12,     
	EC_RES_REQUEST_TRUNCATED = 13,  
	EC_RES_RESPONSE_TOO_BIG = 14    
};


enum host_event_code {
	EC_HOST_EVENT_LID_CLOSED = 1,
	EC_HOST_EVENT_LID_OPEN = 2,
	EC_HOST_EVENT_POWER_BUTTON = 3,
	EC_HOST_EVENT_AC_CONNECTED = 4,
	EC_HOST_EVENT_AC_DISCONNECTED = 5,
	EC_HOST_EVENT_BATTERY_LOW = 6,
	EC_HOST_EVENT_BATTERY_CRITICAL = 7,
	EC_HOST_EVENT_BATTERY = 8,
	EC_HOST_EVENT_THERMAL_THRESHOLD = 9,
	EC_HOST_EVENT_THERMAL_OVERLOAD = 10,
	EC_HOST_EVENT_THERMAL = 11,
	EC_HOST_EVENT_USB_CHARGER = 12,
	EC_HOST_EVENT_KEY_PRESSED = 13,
	
	EC_HOST_EVENT_INTERFACE_READY = 14,
	
	EC_HOST_EVENT_KEYBOARD_RECOVERY = 15,

	
	EC_HOST_EVENT_THERMAL_SHUTDOWN = 16,
	
	EC_HOST_EVENT_BATTERY_SHUTDOWN = 17,

	
	EC_HOST_EVENT_THROTTLE_START = 18,
	
	EC_HOST_EVENT_THROTTLE_STOP = 19,

	
	EC_HOST_EVENT_HANG_DETECT = 20,
	
	EC_HOST_EVENT_HANG_REBOOT = 21,
	
	EC_HOST_EVENT_PD_MCU = 22,

	
	EC_HOST_EVENT_USB_MUX = 28,

	
	EC_HOST_EVENT_RTC = 26,

	
	EC_HOST_EVENT_INVALID = 32
};

#define EC_HOST_EVENT_MASK(event_code) (1UL << ((event_code) - 1))


struct ec_lpc_host_args {
	uint8_t flags;
	uint8_t command_version;
	uint8_t data_size;
	
	uint8_t checksum;
} __packed;



#define EC_HOST_ARGS_FLAG_FROM_HOST 0x01

#define EC_HOST_ARGS_FLAG_TO_HOST   0x02





#define EC_SPI_FRAME_START    0xec


#define EC_SPI_PAST_END       0xed


#define EC_SPI_RX_READY       0xf8


#define EC_SPI_RECEIVING      0xf9


#define EC_SPI_PROCESSING     0xfa


#define EC_SPI_RX_BAD_DATA    0xfb


#define EC_SPI_NOT_READY      0xfc


#define EC_SPI_OLD_READY      0xfd




#define EC_PROTO2_REQUEST_HEADER_BYTES 3
#define EC_PROTO2_REQUEST_TRAILER_BYTES 1
#define EC_PROTO2_REQUEST_OVERHEAD (EC_PROTO2_REQUEST_HEADER_BYTES +	\
				    EC_PROTO2_REQUEST_TRAILER_BYTES)

#define EC_PROTO2_RESPONSE_HEADER_BYTES 2
#define EC_PROTO2_RESPONSE_TRAILER_BYTES 1
#define EC_PROTO2_RESPONSE_OVERHEAD (EC_PROTO2_RESPONSE_HEADER_BYTES +	\
				     EC_PROTO2_RESPONSE_TRAILER_BYTES)


#define EC_PROTO2_MAX_PARAM_SIZE 0xfc


#define EC_PROTO2_MAX_REQUEST_SIZE (EC_PROTO2_REQUEST_OVERHEAD +	\
				    EC_PROTO2_MAX_PARAM_SIZE)
#define EC_PROTO2_MAX_RESPONSE_SIZE (EC_PROTO2_RESPONSE_OVERHEAD +	\
				     EC_PROTO2_MAX_PARAM_SIZE)




#define EC_COMMAND_PROTOCOL_3 0xda

#define EC_HOST_REQUEST_VERSION 3


struct ec_host_request {
	
	uint8_t struct_version;

	
	uint8_t checksum;

	
	uint16_t command;

	
	uint8_t command_version;

	
	uint8_t reserved;

	
	uint16_t data_len;
} __packed;

#define EC_HOST_RESPONSE_VERSION 3


struct ec_host_response {
	
	uint8_t struct_version;

	
	uint8_t checksum;

	
	uint16_t result;

	
	uint16_t data_len;

	
	uint16_t reserved;
} __packed;








#define EC_CMD_PROTO_VERSION 0x00

struct ec_response_proto_version {
	uint32_t version;
} __packed;


#define EC_CMD_HELLO 0x01

struct ec_params_hello {
	uint32_t in_data;  
} __packed;

struct ec_response_hello {
	uint32_t out_data;  
} __packed;


#define EC_CMD_GET_VERSION 0x02

enum ec_current_image {
	EC_IMAGE_UNKNOWN = 0,
	EC_IMAGE_RO,
	EC_IMAGE_RW
};

struct ec_response_get_version {
	
	char version_string_ro[32];
	char version_string_rw[32];
	char reserved[32];       
	uint32_t current_image;  
} __packed;


#define EC_CMD_READ_TEST 0x03

struct ec_params_read_test {
	uint32_t offset;   
	uint32_t size;     
} __packed;

struct ec_response_read_test {
	uint32_t data[32];
} __packed;


#define EC_CMD_GET_BUILD_INFO 0x04


#define EC_CMD_GET_CHIP_INFO 0x05

struct ec_response_get_chip_info {
	
	char vendor[32];
	char name[32];
	char revision[32];  
} __packed;


#define EC_CMD_GET_BOARD_VERSION 0x06

struct ec_response_board_version {
	uint16_t board_version;  
} __packed;


#define EC_CMD_READ_MEMMAP 0x07

struct ec_params_read_memmap {
	uint8_t offset;   
	uint8_t size;     
} __packed;


#define EC_CMD_GET_CMD_VERSIONS 0x08

struct ec_params_get_cmd_versions {
	uint8_t cmd;      
} __packed;

struct ec_params_get_cmd_versions_v1 {
	uint16_t cmd;     
} __packed;

struct ec_response_get_cmd_versions {
	
	uint32_t version_mask;
} __packed;


#define EC_CMD_GET_COMMS_STATUS		0x09


enum ec_comms_status {
	EC_COMMS_STATUS_PROCESSING	= 1 << 0,	
};

struct ec_response_get_comms_status {
	uint32_t flags;		
} __packed;


#define EC_CMD_TEST_PROTOCOL		0x0a


struct ec_params_test_protocol {
	uint32_t ec_result;
	uint32_t ret_len;
	uint8_t buf[32];
} __packed;


struct ec_response_test_protocol {
	uint8_t buf[32];
} __packed;


#define EC_CMD_GET_PROTOCOL_INFO	0x0b



#define EC_PROTOCOL_INFO_IN_PROGRESS_SUPPORTED (1 << 0)

struct ec_response_get_protocol_info {
	

	
	uint32_t protocol_versions;

	
	uint16_t max_request_packet_size;

	
	uint16_t max_response_packet_size;

	
	uint32_t flags;
} __packed;






#define EC_GSV_SET        0x80000000


#define EC_GSV_PARAM_MASK 0x00ffffff

struct ec_params_get_set_value {
	uint32_t flags;
	uint32_t value;
} __packed;

struct ec_response_get_set_value {
	uint32_t flags;
	uint32_t value;
} __packed;


#define EC_CMD_GSV_PAUSE_IN_S5	0x0c



#define EC_CMD_GET_FEATURES  0x0d


enum ec_feature_code {
	
	EC_FEATURE_LIMITED = 0,
	
	EC_FEATURE_FLASH = 1,
	
	EC_FEATURE_PWM_FAN = 2,
	
	EC_FEATURE_PWM_KEYB = 3,
	
	EC_FEATURE_LIGHTBAR = 4,
	
	EC_FEATURE_LED = 5,
	
	EC_FEATURE_MOTION_SENSE = 6,
	
	EC_FEATURE_KEYB = 7,
	
	EC_FEATURE_PSTORE = 8,
	
	EC_FEATURE_PORT80 = 9,
	
	EC_FEATURE_THERMAL = 10,
	
	EC_FEATURE_BKLIGHT_SWITCH = 11,
	
	EC_FEATURE_WIFI_SWITCH = 12,
	
	EC_FEATURE_HOST_EVENTS = 13,
	
	EC_FEATURE_GPIO = 14,
	
	EC_FEATURE_I2C = 15,
	
	EC_FEATURE_CHARGER = 16,
	
	EC_FEATURE_BATTERY = 17,
	
	EC_FEATURE_SMART_BATTERY = 18,
	
	EC_FEATURE_HANG_DETECT = 19,
	
	EC_FEATURE_PMU = 20,
	
	EC_FEATURE_SUB_MCU = 21,
	
	EC_FEATURE_USB_PD = 22,
	
	EC_FEATURE_USB_MUX = 23,
	
	EC_FEATURE_MOTION_SENSE_FIFO = 24,
	
	EC_FEATURE_RTC = 27,
	
	EC_FEATURE_CEC = 35,
};

#define EC_FEATURE_MASK_0(event_code) (1UL << (event_code % 32))
#define EC_FEATURE_MASK_1(event_code) (1UL << (event_code - 32))
struct ec_response_get_features {
	uint32_t flags[2];
} __packed;





#define EC_CMD_FLASH_INFO 0x10


struct ec_response_flash_info {
	
	uint32_t flash_size;
	
	uint32_t write_block_size;
	
	uint32_t erase_block_size;
	
	uint32_t protect_block_size;
} __packed;



#define EC_FLASH_INFO_ERASE_TO_0 (1 << 0)


struct ec_response_flash_info_1 {
	
	uint32_t flash_size;
	uint32_t write_block_size;
	uint32_t erase_block_size;
	uint32_t protect_block_size;

	
	
	uint32_t write_ideal_size;

	
	uint32_t flags;
} __packed;


#define EC_CMD_FLASH_READ 0x11

struct ec_params_flash_read {
	uint32_t offset;   
	uint32_t size;     
} __packed;


#define EC_CMD_FLASH_WRITE 0x12
#define EC_VER_FLASH_WRITE 1


#define EC_FLASH_WRITE_VER0_SIZE 64

struct ec_params_flash_write {
	uint32_t offset;   
	uint32_t size;     
	
} __packed;


#define EC_CMD_FLASH_ERASE 0x13

struct ec_params_flash_erase {
	uint32_t offset;   
	uint32_t size;     
} __packed;


#define EC_CMD_FLASH_PROTECT 0x15
#define EC_VER_FLASH_PROTECT 1  



#define EC_FLASH_PROTECT_RO_AT_BOOT         (1 << 0)

#define EC_FLASH_PROTECT_RO_NOW             (1 << 1)

#define EC_FLASH_PROTECT_ALL_NOW            (1 << 2)

#define EC_FLASH_PROTECT_GPIO_ASSERTED      (1 << 3)

#define EC_FLASH_PROTECT_ERROR_STUCK        (1 << 4)

#define EC_FLASH_PROTECT_ERROR_INCONSISTENT (1 << 5)

#define EC_FLASH_PROTECT_ALL_AT_BOOT        (1 << 6)

struct ec_params_flash_protect {
	uint32_t mask;   
	uint32_t flags;  
} __packed;

struct ec_response_flash_protect {
	
	uint32_t flags;
	
	uint32_t valid_flags;
	
	uint32_t writable_flags;
} __packed;




#define EC_CMD_FLASH_REGION_INFO 0x16
#define EC_VER_FLASH_REGION_INFO 1

enum ec_flash_region {
	
	EC_FLASH_REGION_RO = 0,
	
	EC_FLASH_REGION_RW,
	
	EC_FLASH_REGION_WP_RO,
	
	EC_FLASH_REGION_COUNT,
};

struct ec_params_flash_region_info {
	uint32_t region;  
} __packed;

struct ec_response_flash_region_info {
	uint32_t offset;
	uint32_t size;
} __packed;


#define EC_CMD_VBNV_CONTEXT 0x17
#define EC_VER_VBNV_CONTEXT 1
#define EC_VBNV_BLOCK_SIZE 16

enum ec_vbnvcontext_op {
	EC_VBNV_CONTEXT_OP_READ,
	EC_VBNV_CONTEXT_OP_WRITE,
};

struct ec_params_vbnvcontext {
	uint32_t op;
	uint8_t block[EC_VBNV_BLOCK_SIZE];
} __packed;

struct ec_response_vbnvcontext {
	uint8_t block[EC_VBNV_BLOCK_SIZE];
} __packed;





#define EC_CMD_PWM_GET_FAN_TARGET_RPM 0x20

struct ec_response_pwm_get_fan_rpm {
	uint32_t rpm;
} __packed;


#define EC_CMD_PWM_SET_FAN_TARGET_RPM 0x21

struct ec_params_pwm_set_fan_target_rpm {
	uint32_t rpm;
} __packed;


#define EC_CMD_PWM_GET_KEYBOARD_BACKLIGHT 0x22

struct ec_response_pwm_get_keyboard_backlight {
	uint8_t percent;
	uint8_t enabled;
} __packed;


#define EC_CMD_PWM_SET_KEYBOARD_BACKLIGHT 0x23

struct ec_params_pwm_set_keyboard_backlight {
	uint8_t percent;
} __packed;


#define EC_CMD_PWM_SET_FAN_DUTY 0x24

struct ec_params_pwm_set_fan_duty {
	uint32_t percent;
} __packed;

#define EC_CMD_PWM_SET_DUTY 0x25

#define EC_PWM_MAX_DUTY 0xffff

enum ec_pwm_type {
	
	EC_PWM_TYPE_GENERIC = 0,
	
	EC_PWM_TYPE_KB_LIGHT,
	
	EC_PWM_TYPE_DISPLAY_LIGHT,
	EC_PWM_TYPE_COUNT,
};

struct ec_params_pwm_set_duty {
	uint16_t duty;     
	uint8_t pwm_type;  
	uint8_t index;     
} __packed;

#define EC_CMD_PWM_GET_DUTY 0x26

struct ec_params_pwm_get_duty {
	uint8_t pwm_type;  
	uint8_t index;     
} __packed;

struct ec_response_pwm_get_duty {
	uint16_t duty;     
} __packed;



#define EC_CMD_LIGHTBAR_CMD 0x28

struct rgb_s {
	uint8_t r, g, b;
};

#define LB_BATTERY_LEVELS 4

struct lightbar_params_v0 {
	
	int32_t google_ramp_up;
	int32_t google_ramp_down;
	int32_t s3s0_ramp_up;
	int32_t s0_tick_delay[2];		
	int32_t s0a_tick_delay[2];		
	int32_t s0s3_ramp_down;
	int32_t s3_sleep_for;
	int32_t s3_ramp_up;
	int32_t s3_ramp_down;

	
	uint8_t new_s0;
	uint8_t osc_min[2];			
	uint8_t osc_max[2];			
	uint8_t w_ofs[2];			

	
	uint8_t bright_bl_off_fixed[2];		
	uint8_t bright_bl_on_min[2];		
	uint8_t bright_bl_on_max[2];		

	
	uint8_t battery_threshold[LB_BATTERY_LEVELS - 1];

	
	uint8_t s0_idx[2][LB_BATTERY_LEVELS];	
	uint8_t s3_idx[2][LB_BATTERY_LEVELS];	

	
	struct rgb_s color[8];			
} __packed;

struct lightbar_params_v1 {
	
	int32_t google_ramp_up;
	int32_t google_ramp_down;
	int32_t s3s0_ramp_up;
	int32_t s0_tick_delay[2];		
	int32_t s0a_tick_delay[2];		
	int32_t s0s3_ramp_down;
	int32_t s3_sleep_for;
	int32_t s3_ramp_up;
	int32_t s3_ramp_down;
	int32_t tap_tick_delay;
	int32_t tap_display_time;

	
	uint8_t tap_pct_red;
	uint8_t tap_pct_green;
	uint8_t tap_seg_min_on;
	uint8_t tap_seg_max_on;
	uint8_t tap_seg_osc;
	uint8_t tap_idx[3];

	
	uint8_t osc_min[2];			
	uint8_t osc_max[2];			
	uint8_t w_ofs[2];			

	
	uint8_t bright_bl_off_fixed[2];		
	uint8_t bright_bl_on_min[2];		
	uint8_t bright_bl_on_max[2];		

	
	uint8_t battery_threshold[LB_BATTERY_LEVELS - 1];

	
	uint8_t s0_idx[2][LB_BATTERY_LEVELS];	
	uint8_t s3_idx[2][LB_BATTERY_LEVELS];	

	
	struct rgb_s color[8];			
} __packed;


#define EC_LB_PROG_LEN 192
struct lightbar_program {
	uint8_t size;
	uint8_t data[EC_LB_PROG_LEN];
};

struct ec_params_lightbar {
	uint8_t cmd;		      
	union {
		struct {
			
		} dump, off, on, init, get_seq, get_params_v0, get_params_v1,
			version, get_brightness, get_demo, suspend, resume;

		struct {
			uint8_t num;
		} set_brightness, seq, demo;

		struct {
			uint8_t ctrl, reg, value;
		} reg;

		struct {
			uint8_t led, red, green, blue;
		} set_rgb;

		struct {
			uint8_t led;
		} get_rgb;

		struct {
			uint8_t enable;
		} manual_suspend_ctrl;

		struct lightbar_params_v0 set_params_v0;
		struct lightbar_params_v1 set_params_v1;
		struct lightbar_program set_program;
	};
} __packed;

struct ec_response_lightbar {
	union {
		struct {
			struct {
				uint8_t reg;
				uint8_t ic0;
				uint8_t ic1;
			} vals[23];
		} dump;

		struct  {
			uint8_t num;
		} get_seq, get_brightness, get_demo;

		struct lightbar_params_v0 get_params_v0;
		struct lightbar_params_v1 get_params_v1;

		struct {
			uint32_t num;
			uint32_t flags;
		} version;

		struct {
			uint8_t red, green, blue;
		} get_rgb;

		struct {
			
		} off, on, init, set_brightness, seq, reg, set_rgb,
			demo, set_params_v0, set_params_v1,
			set_program, manual_suspend_ctrl, suspend, resume;
	};
} __packed;


enum lightbar_command {
	LIGHTBAR_CMD_DUMP = 0,
	LIGHTBAR_CMD_OFF = 1,
	LIGHTBAR_CMD_ON = 2,
	LIGHTBAR_CMD_INIT = 3,
	LIGHTBAR_CMD_SET_BRIGHTNESS = 4,
	LIGHTBAR_CMD_SEQ = 5,
	LIGHTBAR_CMD_REG = 6,
	LIGHTBAR_CMD_SET_RGB = 7,
	LIGHTBAR_CMD_GET_SEQ = 8,
	LIGHTBAR_CMD_DEMO = 9,
	LIGHTBAR_CMD_GET_PARAMS_V0 = 10,
	LIGHTBAR_CMD_SET_PARAMS_V0 = 11,
	LIGHTBAR_CMD_VERSION = 12,
	LIGHTBAR_CMD_GET_BRIGHTNESS = 13,
	LIGHTBAR_CMD_GET_RGB = 14,
	LIGHTBAR_CMD_GET_DEMO = 15,
	LIGHTBAR_CMD_GET_PARAMS_V1 = 16,
	LIGHTBAR_CMD_SET_PARAMS_V1 = 17,
	LIGHTBAR_CMD_SET_PROGRAM = 18,
	LIGHTBAR_CMD_MANUAL_SUSPEND_CTRL = 19,
	LIGHTBAR_CMD_SUSPEND = 20,
	LIGHTBAR_CMD_RESUME = 21,
	LIGHTBAR_NUM_CMDS
};




#define EC_CMD_LED_CONTROL 0x29

enum ec_led_id {
	
	EC_LED_ID_BATTERY_LED = 0,
	
	EC_LED_ID_POWER_LED,
	
	EC_LED_ID_ADAPTER_LED,

	EC_LED_ID_COUNT
};


#define EC_LED_FLAGS_QUERY (1 << 0) 
#define EC_LED_FLAGS_AUTO  (1 << 1) 

enum ec_led_colors {
	EC_LED_COLOR_RED = 0,
	EC_LED_COLOR_GREEN,
	EC_LED_COLOR_BLUE,
	EC_LED_COLOR_YELLOW,
	EC_LED_COLOR_WHITE,

	EC_LED_COLOR_COUNT
};

struct ec_params_led_control {
	uint8_t led_id;     
	uint8_t flags;      

	uint8_t brightness[EC_LED_COLOR_COUNT];
} __packed;

struct ec_response_led_control {
	
	uint8_t brightness_range[EC_LED_COLOR_COUNT];
} __packed;







#define EC_CMD_VBOOT_HASH 0x2A

struct ec_params_vboot_hash {
	uint8_t cmd;             
	uint8_t hash_type;       
	uint8_t nonce_size;      
	uint8_t reserved0;       
	uint32_t offset;         
	uint32_t size;           
	uint8_t nonce_data[64];  
} __packed;

struct ec_response_vboot_hash {
	uint8_t status;          
	uint8_t hash_type;       
	uint8_t digest_size;     
	uint8_t reserved0;       
	uint32_t offset;         
	uint32_t size;           
	uint8_t hash_digest[64]; 
} __packed;

enum ec_vboot_hash_cmd {
	EC_VBOOT_HASH_GET = 0,       
	EC_VBOOT_HASH_ABORT = 1,     
	EC_VBOOT_HASH_START = 2,     
	EC_VBOOT_HASH_RECALC = 3,    
};

enum ec_vboot_hash_type {
	EC_VBOOT_HASH_TYPE_SHA256 = 0, 
};

enum ec_vboot_hash_status {
	EC_VBOOT_HASH_STATUS_NONE = 0, 
	EC_VBOOT_HASH_STATUS_DONE = 1, 
	EC_VBOOT_HASH_STATUS_BUSY = 2, 
};


#define EC_VBOOT_HASH_OFFSET_RO 0xfffffffe
#define EC_VBOOT_HASH_OFFSET_RW 0xfffffffd



#define EC_CMD_MOTION_SENSE_CMD 0x2B


enum motionsense_command {
	
	MOTIONSENSE_CMD_DUMP = 0,

	
	MOTIONSENSE_CMD_INFO = 1,

	
	MOTIONSENSE_CMD_EC_RATE = 2,

	
	MOTIONSENSE_CMD_SENSOR_ODR = 3,

	
	MOTIONSENSE_CMD_SENSOR_RANGE = 4,

	
	MOTIONSENSE_CMD_KB_WAKE_ANGLE = 5,

	
	MOTIONSENSE_CMD_DATA = 6,

	
	MOTIONSENSE_CMD_PERFORM_CALIB = 10,

	
	MOTIONSENSE_CMD_SENSOR_OFFSET = 11,

	
	MOTIONSENSE_NUM_CMDS
};

enum motionsensor_id {
	EC_MOTION_SENSOR_ACCEL_BASE = 0,
	EC_MOTION_SENSOR_ACCEL_LID = 1,
	EC_MOTION_SENSOR_GYRO = 2,

	
	EC_MOTION_SENSOR_COUNT = 3
};


enum motionsensor_type {
	MOTIONSENSE_TYPE_ACCEL = 0,
	MOTIONSENSE_TYPE_GYRO = 1,
	MOTIONSENSE_TYPE_MAG = 2,
	MOTIONSENSE_TYPE_PROX = 3,
	MOTIONSENSE_TYPE_LIGHT = 4,
	MOTIONSENSE_TYPE_ACTIVITY = 5,
	MOTIONSENSE_TYPE_BARO = 6,
	MOTIONSENSE_TYPE_MAX,
};


enum motionsensor_location {
	MOTIONSENSE_LOC_BASE = 0,
	MOTIONSENSE_LOC_LID = 1,
	MOTIONSENSE_LOC_MAX,
};


enum motionsensor_chip {
	MOTIONSENSE_CHIP_KXCJ9 = 0,
};


#define MOTIONSENSE_MODULE_FLAG_ACTIVE (1<<0)


#define MOTIONSENSE_SENSOR_FLAG_PRESENT (1<<0)


#define EC_MOTION_SENSE_NO_VALUE -1

#define EC_MOTION_SENSE_INVALID_CALIB_TEMP 0x8000


#define MOTION_SENSE_SET_OFFSET	1

struct ec_response_motion_sensor_data {
	
	uint8_t flags;
	
	uint8_t sensor_num;
	
	union {
		int16_t             data[3];
		struct {
			uint16_t    rsvd;
			uint32_t    timestamp;
		} __packed;
		struct {
			uint8_t     activity; 
			uint8_t     state;
			int16_t     add_info[2];
		};
	};
} __packed;

struct ec_params_motion_sense {
	uint8_t cmd;
	union {
		
		struct {
			
		} dump;

		
		struct {
			
			int16_t data;
		} ec_rate, kb_wake_angle;

		
		struct {
			uint8_t sensor_num;

			
			uint16_t flags;

			
			int16_t temp;

			
			int16_t offset[3];
		} __packed sensor_offset;

		
		struct {
			uint8_t sensor_num;
		} info;

		
		struct {
			
			uint8_t sensor_num;

			
			uint8_t roundup;

			uint16_t reserved;

			
			int32_t data;
		} sensor_odr, sensor_range;
	};
} __packed;

struct ec_response_motion_sense {
	union {
		
		struct {
			
			uint8_t module_flags;

			
			uint8_t sensor_count;

			
			struct ec_response_motion_sensor_data sensor[0];
		} dump;

		
		struct {
			
			uint8_t type;

			
			uint8_t location;

			
			uint8_t chip;
		} info;

		
		struct ec_response_motion_sensor_data data;

		
		struct {
			
			int32_t ret;
		} ec_rate, sensor_odr, sensor_range, kb_wake_angle;

		
		struct {
			int16_t temp;
			int16_t offset[3];
		} sensor_offset, perform_calib;
	};
} __packed;





#define EC_CMD_USB_CHARGE_SET_MODE 0x30

struct ec_params_usb_charge_set_mode {
	uint8_t usb_port_id;
	uint8_t mode;
} __packed;





#define EC_PSTORE_SIZE_MAX 64


#define EC_CMD_PSTORE_INFO 0x40

struct ec_response_pstore_info {
	
	uint32_t pstore_size;
	
	uint32_t access_size;
} __packed;


#define EC_CMD_PSTORE_READ 0x41

struct ec_params_pstore_read {
	uint32_t offset;   
	uint32_t size;     
} __packed;


#define EC_CMD_PSTORE_WRITE 0x42

struct ec_params_pstore_write {
	uint32_t offset;   
	uint32_t size;     
	uint8_t data[EC_PSTORE_SIZE_MAX];
} __packed;





struct ec_params_rtc {
	uint32_t time;
} __packed;

struct ec_response_rtc {
	uint32_t time;
} __packed;


#define EC_CMD_RTC_GET_VALUE 0x44
#define EC_CMD_RTC_GET_ALARM 0x45


#define EC_CMD_RTC_SET_VALUE 0x46
#define EC_CMD_RTC_SET_ALARM 0x47


#define EC_RTC_ALARM_CLEAR 0





#define EC_PORT80_SIZE_MAX 32


#define EC_CMD_PORT80_LAST_BOOT 0x48
#define EC_CMD_PORT80_READ 0x48

enum ec_port80_subcmd {
	EC_PORT80_GET_INFO = 0,
	EC_PORT80_READ_BUFFER,
};

struct ec_params_port80_read {
	uint16_t subcmd;
	union {
		struct {
			uint32_t offset;
			uint32_t num_entries;
		} read_buffer;
	};
} __packed;

struct ec_response_port80_read {
	union {
		struct {
			uint32_t writes;
			uint32_t history_size;
			uint32_t last_boot;
		} get_info;
		struct {
			uint16_t codes[EC_PORT80_SIZE_MAX];
		} data;
	};
} __packed;

struct ec_response_port80_last_boot {
	uint16_t code;
} __packed;




#define EC_CMD_THERMAL_SET_THRESHOLD 0x50
#define EC_CMD_THERMAL_GET_THRESHOLD 0x51




struct ec_params_thermal_set_threshold {
	uint8_t sensor_type;
	uint8_t threshold_id;
	uint16_t value;
} __packed;


struct ec_params_thermal_get_threshold {
	uint8_t sensor_type;
	uint8_t threshold_id;
} __packed;

struct ec_response_thermal_get_threshold {
	uint16_t value;
} __packed;



enum ec_temp_thresholds {
	EC_TEMP_THRESH_WARN = 0,
	EC_TEMP_THRESH_HIGH,
	EC_TEMP_THRESH_HALT,

	EC_TEMP_THRESH_COUNT
};


struct ec_thermal_config {
	uint32_t temp_host[EC_TEMP_THRESH_COUNT]; 
	uint32_t temp_fan_off;		
	uint32_t temp_fan_max;		
} __packed;


struct ec_params_thermal_get_threshold_v1 {
	uint32_t sensor_num;
} __packed;



struct ec_params_thermal_set_threshold_v1 {
	uint32_t sensor_num;
	struct ec_thermal_config cfg;
} __packed;





#define EC_CMD_THERMAL_AUTO_FAN_CTRL 0x52


#define EC_CMD_TMP006_GET_CALIBRATION 0x53

struct ec_params_tmp006_get_calibration {
	uint8_t index;
} __packed;

struct ec_response_tmp006_get_calibration {
	float s0;
	float b0;
	float b1;
	float b2;
} __packed;


#define EC_CMD_TMP006_SET_CALIBRATION 0x54

struct ec_params_tmp006_set_calibration {
	uint8_t index;
	uint8_t reserved[3];  
	float s0;
	float b0;
	float b1;
	float b2;
} __packed;


#define EC_CMD_TMP006_GET_RAW 0x55

struct ec_params_tmp006_get_raw {
	uint8_t index;
} __packed;

struct ec_response_tmp006_get_raw {
	int32_t t;  
	int32_t v;  
};





#define EC_CMD_MKBP_STATE 0x60


#define EC_CMD_MKBP_INFO 0x61

struct ec_response_mkbp_info {
	uint32_t rows;
	uint32_t cols;
	
	uint8_t reserved;
} __packed;

struct ec_params_mkbp_info {
	uint8_t info_type;
	uint8_t event_type;
} __packed;

enum ec_mkbp_info_type {
	
	EC_MKBP_INFO_KBD = 0,

	
	EC_MKBP_INFO_SUPPORTED = 1,

	
	EC_MKBP_INFO_CURRENT = 2,
};


#define EC_CMD_MKBP_SIMULATE_KEY 0x62

struct ec_params_mkbp_simulate_key {
	uint8_t col;
	uint8_t row;
	uint8_t pressed;
} __packed;


#define EC_CMD_MKBP_SET_CONFIG 0x64
#define EC_CMD_MKBP_GET_CONFIG 0x65


enum mkbp_config_flags {
	EC_MKBP_FLAGS_ENABLE = 1,	
};

enum mkbp_config_valid {
	EC_MKBP_VALID_SCAN_PERIOD		= 1 << 0,
	EC_MKBP_VALID_POLL_TIMEOUT		= 1 << 1,
	EC_MKBP_VALID_MIN_POST_SCAN_DELAY	= 1 << 3,
	EC_MKBP_VALID_OUTPUT_SETTLE		= 1 << 4,
	EC_MKBP_VALID_DEBOUNCE_DOWN		= 1 << 5,
	EC_MKBP_VALID_DEBOUNCE_UP		= 1 << 6,
	EC_MKBP_VALID_FIFO_MAX_DEPTH		= 1 << 7,
};


struct ec_mkbp_config {
	uint32_t valid_mask;		
	uint8_t flags;		
	uint8_t valid_flags;		
	uint16_t scan_period_us;	
	
	uint32_t poll_timeout_us;
	
	uint16_t min_post_scan_delay_us;
	
	uint16_t output_settle_us;
	uint16_t debounce_down_us;	
	uint16_t debounce_up_us;	
	
	uint8_t fifo_max_depth;
} __packed;

struct ec_params_mkbp_set_config {
	struct ec_mkbp_config config;
} __packed;

struct ec_response_mkbp_get_config {
	struct ec_mkbp_config config;
} __packed;


#define EC_CMD_KEYSCAN_SEQ_CTRL 0x66

enum ec_keyscan_seq_cmd {
	EC_KEYSCAN_SEQ_STATUS = 0,	
	EC_KEYSCAN_SEQ_CLEAR = 1,	
	EC_KEYSCAN_SEQ_ADD = 2,		
	EC_KEYSCAN_SEQ_START = 3,	
	EC_KEYSCAN_SEQ_COLLECT = 4,	
};

enum ec_collect_flags {
	
	EC_KEYSCAN_SEQ_FLAG_DONE	= 1 << 0,
};

struct ec_collect_item {
	uint8_t flags;		
};

struct ec_params_keyscan_seq_ctrl {
	uint8_t cmd;	
	union {
		struct {
			uint8_t active;		
			uint8_t num_items;	
			
			uint8_t cur_item;
		} status;
		struct {
			
			uint32_t time_us;
			uint8_t scan[0];	
		} add;
		struct {
			uint8_t start_item;	
			uint8_t num_items;	
		} collect;
	};
} __packed;

struct ec_result_keyscan_seq_ctrl {
	union {
		struct {
			uint8_t num_items;	
			
			struct ec_collect_item item[0];
		} collect;
	};
} __packed;


#define EC_CMD_GET_NEXT_EVENT 0x67

enum ec_mkbp_event {
	
	EC_MKBP_EVENT_KEY_MATRIX = 0,

	
	EC_MKBP_EVENT_HOST_EVENT = 1,

	
	EC_MKBP_EVENT_SENSOR_FIFO = 2,

	
	EC_MKBP_EVENT_BUTTON = 3,

	
	EC_MKBP_EVENT_SWITCH = 4,

	
	EC_MKBP_EVENT_SYSRQ = 6,

	
	EC_MKBP_EVENT_CEC_EVENT = 8,

	
	EC_MKBP_EVENT_CEC_MESSAGE = 9,

	
	EC_MKBP_EVENT_COUNT,
};

union ec_response_get_next_data {
	uint8_t   key_matrix[13];

	
	uint32_t  host_event;

	uint32_t   buttons;
	uint32_t   switches;
	uint32_t   sysrq;
} __packed;

union ec_response_get_next_data_v1 {
	uint8_t key_matrix[16];
	uint32_t host_event;
	uint32_t buttons;
	uint32_t switches;
	uint32_t sysrq;
	uint32_t cec_events;
	uint8_t cec_message[16];
} __packed;

struct ec_response_get_next_event {
	uint8_t event_type;
	
	union ec_response_get_next_data data;
} __packed;

struct ec_response_get_next_event_v1 {
	uint8_t event_type;
	
	union ec_response_get_next_data_v1 data;
} __packed;



#define EC_MKBP_POWER_BUTTON	0
#define EC_MKBP_VOL_UP		1
#define EC_MKBP_VOL_DOWN	2


#define EC_MKBP_LID_OPEN	0
#define EC_MKBP_TABLET_MODE	1





#define EC_CMD_TEMP_SENSOR_GET_INFO 0x70

struct ec_params_temp_sensor_get_info {
	uint8_t id;
} __packed;

struct ec_response_temp_sensor_get_info {
	char sensor_name[32];
	uint8_t sensor_type;
} __packed;









struct ec_params_host_event_mask {
	uint32_t mask;
} __packed;

struct ec_response_host_event_mask {
	uint32_t mask;
} __packed;


#define EC_CMD_HOST_EVENT_GET_B         0x87
#define EC_CMD_HOST_EVENT_GET_SMI_MASK  0x88
#define EC_CMD_HOST_EVENT_GET_SCI_MASK  0x89
#define EC_CMD_HOST_EVENT_GET_WAKE_MASK 0x8d


#define EC_CMD_HOST_EVENT_SET_SMI_MASK  0x8a
#define EC_CMD_HOST_EVENT_SET_SCI_MASK  0x8b
#define EC_CMD_HOST_EVENT_CLEAR         0x8c
#define EC_CMD_HOST_EVENT_SET_WAKE_MASK 0x8e
#define EC_CMD_HOST_EVENT_CLEAR_B       0x8f





#define EC_CMD_SWITCH_ENABLE_BKLIGHT 0x90

struct ec_params_switch_enable_backlight {
	uint8_t enabled;
} __packed;


#define EC_CMD_SWITCH_ENABLE_WIRELESS 0x91
#define EC_VER_SWITCH_ENABLE_WIRELESS 1


struct ec_params_switch_enable_wireless_v0 {
	uint8_t enabled;
} __packed;


struct ec_params_switch_enable_wireless_v1 {
	
	uint8_t now_flags;

	
	uint8_t now_mask;

	
	uint8_t suspend_flags;

	
	uint8_t suspend_mask;
} __packed;


struct ec_response_switch_enable_wireless_v1 {
	
	uint8_t now_flags;

	
	uint8_t suspend_flags;
} __packed;





#define EC_CMD_GPIO_SET 0x92

struct ec_params_gpio_set {
	char name[32];
	uint8_t val;
} __packed;


#define EC_CMD_GPIO_GET 0x93


struct ec_params_gpio_get {
	char name[32];
} __packed;
struct ec_response_gpio_get {
	uint8_t val;
} __packed;


struct ec_params_gpio_get_v1 {
	uint8_t subcmd;
	union {
		struct {
			char name[32];
		} get_value_by_name;
		struct {
			uint8_t index;
		} get_info;
	};
} __packed;

struct ec_response_gpio_get_v1 {
	union {
		struct {
			uint8_t val;
		} get_value_by_name, get_count;
		struct {
			uint8_t val;
			char name[32];
			uint32_t flags;
		} get_info;
	};
} __packed;

enum gpio_get_subcmd {
	EC_GPIO_GET_BY_NAME = 0,
	EC_GPIO_GET_COUNT = 1,
	EC_GPIO_GET_INFO = 2,
};







#define EC_CMD_I2C_READ 0x94

struct ec_params_i2c_read {
	uint16_t addr; 
	uint8_t read_size; 
	uint8_t port;
	uint8_t offset;
} __packed;
struct ec_response_i2c_read {
	uint16_t data;
} __packed;


#define EC_CMD_I2C_WRITE 0x95

struct ec_params_i2c_write {
	uint16_t data;
	uint16_t addr; 
	uint8_t write_size; 
	uint8_t port;
	uint8_t offset;
} __packed;





#define EC_CMD_CHARGE_CONTROL 0x96
#define EC_VER_CHARGE_CONTROL 1

enum ec_charge_control_mode {
	CHARGE_CONTROL_NORMAL = 0,
	CHARGE_CONTROL_IDLE,
	CHARGE_CONTROL_DISCHARGE,
};

struct ec_params_charge_control {
	uint32_t mode;  
} __packed;





#define EC_CMD_CONSOLE_SNAPSHOT 0x97


#define EC_CMD_CONSOLE_READ 0x98

enum ec_console_read_subcmd {
	CONSOLE_READ_NEXT = 0,
	CONSOLE_READ_RECENT
};

struct ec_params_console_read_v1 {
	uint8_t subcmd; 
} __packed;





#define EC_CMD_BATTERY_CUT_OFF 0x99

#define EC_BATTERY_CUTOFF_FLAG_AT_SHUTDOWN	(1 << 0)

struct ec_params_battery_cutoff {
	uint8_t flags;
} __packed;





#define EC_CMD_USB_MUX 0x9a

struct ec_params_usb_mux {
	uint8_t mux;
} __packed;




enum ec_ldo_state {
	EC_LDO_STATE_OFF = 0,	
	EC_LDO_STATE_ON = 1,	
};


#define EC_CMD_LDO_SET 0x9b

struct ec_params_ldo_set {
	uint8_t index;
	uint8_t state;
} __packed;


#define EC_CMD_LDO_GET 0x9c

struct ec_params_ldo_get {
	uint8_t index;
} __packed;

struct ec_response_ldo_get {
	uint8_t state;
} __packed;





#define EC_CMD_POWER_INFO 0x9d

struct ec_response_power_info {
	uint32_t usb_dev_type;
	uint16_t voltage_ac;
	uint16_t voltage_system;
	uint16_t current_system;
	uint16_t usb_current_limit;
} __packed;




#define EC_CMD_I2C_PASSTHRU 0x9e


#define EC_I2C_FLAG_READ	(1 << 15)


#define EC_I2C_ADDR_MASK	0x3ff

#define EC_I2C_STATUS_NAK	(1 << 0) 
#define EC_I2C_STATUS_TIMEOUT	(1 << 1) 


#define EC_I2C_STATUS_ERROR	(EC_I2C_STATUS_NAK | EC_I2C_STATUS_TIMEOUT)

struct ec_params_i2c_passthru_msg {
	uint16_t addr_flags;	
	uint16_t len;		
} __packed;

struct ec_params_i2c_passthru {
	uint8_t port;		
	uint8_t num_msgs;	
	struct ec_params_i2c_passthru_msg msg[];
	
} __packed;

struct ec_response_i2c_passthru {
	uint8_t i2c_status;	
	uint8_t num_msgs;	
	uint8_t data[];		
} __packed;




#define EC_CMD_HANG_DETECT 0x9f



#define EC_HANG_START_ON_POWER_PRESS  (1 << 0)


#define EC_HANG_START_ON_LID_CLOSE    (1 << 1)

 
#define EC_HANG_START_ON_LID_OPEN     (1 << 2)


#define EC_HANG_START_ON_RESUME       (1 << 3)




#define EC_HANG_STOP_ON_POWER_RELEASE (1 << 8)


#define EC_HANG_STOP_ON_HOST_COMMAND  (1 << 9)


#define EC_HANG_STOP_ON_SUSPEND       (1 << 10)


#define EC_HANG_START_NOW             (1 << 30)


#define EC_HANG_STOP_NOW              (1 << 31)

struct ec_params_hang_detect {
	
	uint32_t flags;

	
	uint16_t host_event_timeout_msec;

	
	uint16_t warm_reboot_timeout_msec;
} __packed;





#define EC_CMD_CHARGE_STATE 0xa0


enum charge_state_command {
	CHARGE_STATE_CMD_GET_STATE,
	CHARGE_STATE_CMD_GET_PARAM,
	CHARGE_STATE_CMD_SET_PARAM,
	CHARGE_STATE_NUM_CMDS
};


enum charge_state_params {
	CS_PARAM_CHG_VOLTAGE,	      
	CS_PARAM_CHG_CURRENT,	      
	CS_PARAM_CHG_INPUT_CURRENT,   
	CS_PARAM_CHG_STATUS,	      
	CS_PARAM_CHG_OPTION,	      
	
	CS_NUM_BASE_PARAMS,

	
	CS_PARAM_CUSTOM_PROFILE_MIN = 0x10000,
	CS_PARAM_CUSTOM_PROFILE_MAX = 0x1ffff,

	
};

struct ec_params_charge_state {
	uint8_t cmd;				
	union {
		struct {
			
		} get_state;

		struct {
			uint32_t param;		
		} get_param;

		struct {
			uint32_t param;		
			uint32_t value;		
		} set_param;
	};
} __packed;

struct ec_response_charge_state {
	union {
		struct {
			int ac;
			int chg_voltage;
			int chg_current;
			int chg_input_current;
			int batt_state_of_charge;
		} get_state;

		struct {
			uint32_t value;
		} get_param;
		struct {
			
		} set_param;
	};
} __packed;



#define EC_CMD_CHARGE_CURRENT_LIMIT 0xa1

struct ec_params_current_limit {
	uint32_t limit; 
} __packed;


#define EC_CMD_EXTERNAL_POWER_LIMIT 0x00A2


struct ec_params_external_power_limit_v1 {
	uint16_t current_lim; 
	uint16_t voltage_lim; 
} __packed;

#define EC_POWER_LIMIT_NONE 0xffff


#define EC_CMD_HOST_SLEEP_EVENT 0xa9

enum host_sleep_event {
	HOST_SLEEP_EVENT_S3_SUSPEND   = 1,
	HOST_SLEEP_EVENT_S3_RESUME    = 2,
	HOST_SLEEP_EVENT_S0IX_SUSPEND = 3,
	HOST_SLEEP_EVENT_S0IX_RESUME  = 4
};

struct ec_params_host_sleep_event {
	uint8_t sleep_event;
} __packed;





#define EC_CMD_SB_READ_WORD   0xb0
#define EC_CMD_SB_WRITE_WORD  0xb1


#define EC_CMD_SB_READ_BLOCK  0xb2
#define EC_CMD_SB_WRITE_BLOCK 0xb3

struct ec_params_sb_rd {
	uint8_t reg;
} __packed;

struct ec_response_sb_rd_word {
	uint16_t value;
} __packed;

struct ec_params_sb_wr_word {
	uint8_t reg;
	uint16_t value;
} __packed;

struct ec_response_sb_rd_block {
	uint8_t data[32];
} __packed;

struct ec_params_sb_wr_block {
	uint8_t reg;
	uint16_t data[32];
} __packed;




#define EC_CMD_BATTERY_VENDOR_PARAM 0xb4

enum ec_battery_vendor_param_mode {
	BATTERY_VENDOR_PARAM_MODE_GET = 0,
	BATTERY_VENDOR_PARAM_MODE_SET,
};

struct ec_params_battery_vendor_param {
	uint32_t param;
	uint32_t value;
	uint8_t mode;
} __packed;

struct ec_response_battery_vendor_param {
	uint32_t value;
} __packed;





#define EC_CMD_REBOOT_EC 0xd2


enum ec_reboot_cmd {
	EC_REBOOT_CANCEL = 0,        
	EC_REBOOT_JUMP_RO = 1,       
	EC_REBOOT_JUMP_RW = 2,       
	
	EC_REBOOT_COLD = 4,          
	EC_REBOOT_DISABLE_JUMP = 5,  
	EC_REBOOT_HIBERNATE = 6      
};


#define EC_REBOOT_FLAG_RESERVED0      (1 << 0)  
#define EC_REBOOT_FLAG_ON_AP_SHUTDOWN (1 << 1)  

struct ec_params_reboot_ec {
	uint8_t cmd;           
	uint8_t flags;         
} __packed;


#define EC_CMD_GET_PANIC_INFO 0xd3





#define EC_CMD_ACPI_READ 0x80


#define EC_CMD_ACPI_WRITE 0x81


#define EC_CMD_ACPI_QUERY_EVENT 0x84




#define EC_ACPI_MEM_VERSION            0x00

#define EC_ACPI_MEM_TEST               0x01

#define EC_ACPI_MEM_TEST_COMPLIMENT    0x02


#define EC_ACPI_MEM_KEYBOARD_BACKLIGHT 0x03

#define EC_ACPI_MEM_FAN_DUTY           0x04


#define EC_ACPI_MEM_TEMP_ID            0x05
#define EC_ACPI_MEM_TEMP_THRESHOLD     0x06
#define EC_ACPI_MEM_TEMP_COMMIT        0x07

#define EC_ACPI_MEM_TEMP_COMMIT_SELECT_MASK (1 << 0)
#define EC_ACPI_MEM_TEMP_COMMIT_ENABLE_MASK (1 << 1)



#define EC_ACPI_MEM_CHARGING_LIMIT     0x08


#define EC_ACPI_MEM_CHARGING_LIMIT_STEP_MA   64

#define EC_ACPI_MEM_CHARGING_LIMIT_DISABLED  0xff


#define EC_ACPI_MEM_VERSION_CURRENT 1




#define EC_MAX_CEC_MSG_LEN 16


#define EC_CMD_CEC_WRITE_MSG 0x00B8


struct ec_params_cec_write {
	uint8_t msg[EC_MAX_CEC_MSG_LEN];
} __packed;


#define EC_CMD_CEC_SET 0x00BA


struct ec_params_cec_set {
	uint8_t cmd; 
	uint8_t val;
} __packed;


#define EC_CMD_CEC_GET 0x00BB


struct ec_params_cec_get {
	uint8_t cmd; 
} __packed;


struct ec_response_cec_get {
	uint8_t val;
} __packed;


enum ec_cec_command {
	
	CEC_CMD_ENABLE,
	
	CEC_CMD_LOGICAL_ADDRESS,
};


enum mkbp_cec_event {
	
	EC_MKBP_CEC_SEND_OK			= BIT(0),
	
	EC_MKBP_CEC_SEND_FAILED			= BIT(1),
};





#define EC_CMD_REBOOT 0xd1  


#define EC_CMD_RESEND_RESPONSE 0xdb


#define EC_CMD_VERSION0 0xdc

#endif  





#define EC_CMD_PD_EXCHANGE_STATUS 0x100


struct ec_params_pd_status {
	int8_t batt_soc; 
} __packed;


struct ec_response_pd_status {
	int8_t status;        
	uint32_t curr_lim_ma; 
} __packed;


#define EC_CMD_USB_PD_CONTROL 0x101

enum usb_pd_control_role {
	USB_PD_CTRL_ROLE_NO_CHANGE = 0,
	USB_PD_CTRL_ROLE_TOGGLE_ON = 1, 
	USB_PD_CTRL_ROLE_TOGGLE_OFF = 2,
	USB_PD_CTRL_ROLE_FORCE_SINK = 3,
	USB_PD_CTRL_ROLE_FORCE_SOURCE = 4,
};

enum usb_pd_control_mux {
	USB_PD_CTRL_MUX_NO_CHANGE = 0,
	USB_PD_CTRL_MUX_NONE = 1,
	USB_PD_CTRL_MUX_USB = 2,
	USB_PD_CTRL_MUX_DP = 3,
	USB_PD_CTRL_MUX_DOCK = 4,
	USB_PD_CTRL_MUX_AUTO = 5,
};

enum usb_pd_control_swap {
	USB_PD_CTRL_SWAP_NONE = 0,
	USB_PD_CTRL_SWAP_DATA = 1,
	USB_PD_CTRL_SWAP_POWER = 2,
	USB_PD_CTRL_SWAP_VCONN = 3,
	USB_PD_CTRL_SWAP_COUNT
};

struct ec_params_usb_pd_control {
	uint8_t port;
	uint8_t role;
	uint8_t mux;
	uint8_t swap;
} __packed;

#define PD_CTRL_RESP_ENABLED_COMMS      (1 << 0) 
#define PD_CTRL_RESP_ENABLED_CONNECTED  (1 << 1) 
#define PD_CTRL_RESP_ENABLED_PD_CAPABLE (1 << 2) 

#define PD_CTRL_RESP_ROLE_POWER         BIT(0) 
#define PD_CTRL_RESP_ROLE_DATA          BIT(1) 
#define PD_CTRL_RESP_ROLE_VCONN         BIT(2) 
#define PD_CTRL_RESP_ROLE_DR_POWER      BIT(3) 
#define PD_CTRL_RESP_ROLE_DR_DATA       BIT(4) 
#define PD_CTRL_RESP_ROLE_USB_COMM      BIT(5) 
#define PD_CTRL_RESP_ROLE_EXT_POWERED   BIT(6) 

struct ec_response_usb_pd_control_v1 {
	uint8_t enabled;
	uint8_t role;
	uint8_t polarity;
	char state[32];
} __packed;

#define EC_CMD_USB_PD_PORTS 0x102


#define EC_USB_PD_MAX_PORTS 8

struct ec_response_usb_pd_ports {
	uint8_t num_ports;
} __packed;

#define EC_CMD_USB_PD_POWER_INFO 0x103

#define PD_POWER_CHARGING_PORT 0xff
struct ec_params_usb_pd_power_info {
	uint8_t port;
} __packed;

enum usb_chg_type {
	USB_CHG_TYPE_NONE,
	USB_CHG_TYPE_PD,
	USB_CHG_TYPE_C,
	USB_CHG_TYPE_PROPRIETARY,
	USB_CHG_TYPE_BC12_DCP,
	USB_CHG_TYPE_BC12_CDP,
	USB_CHG_TYPE_BC12_SDP,
	USB_CHG_TYPE_OTHER,
	USB_CHG_TYPE_VBUS,
	USB_CHG_TYPE_UNKNOWN,
};
enum usb_power_roles {
	USB_PD_PORT_POWER_DISCONNECTED,
	USB_PD_PORT_POWER_SOURCE,
	USB_PD_PORT_POWER_SINK,
	USB_PD_PORT_POWER_SINK_NOT_CHARGING,
};

struct usb_chg_measures {
	uint16_t voltage_max;
	uint16_t voltage_now;
	uint16_t current_max;
	uint16_t current_lim;
} __packed;

struct ec_response_usb_pd_power_info {
	uint8_t role;
	uint8_t type;
	uint8_t dualrole;
	uint8_t reserved1;
	struct usb_chg_measures meas;
	uint32_t max_power;
} __packed;

struct ec_params_usb_pd_info_request {
	uint8_t port;
} __packed;


#define EC_CMD_USB_PD_DISCOVERY 0x0113
struct ec_params_usb_pd_discovery_entry {
	uint16_t vid;  
	uint16_t pid;  
	uint8_t ptype; 
} __packed;


#define EC_CMD_PD_CHARGE_PORT_OVERRIDE 0x0114


enum usb_pd_override_ports {
	OVERRIDE_DONT_CHARGE = -2,
	OVERRIDE_OFF = -1,
	
};

struct ec_params_charge_port_override {
	int16_t override_port; 
} __packed;


#define EC_CMD_PD_GET_LOG_ENTRY 0x0115

struct ec_response_pd_log {
	uint32_t timestamp; 
	uint8_t type;       
	uint8_t size_port;  
	uint16_t data;      
	uint8_t payload[0]; 
} __packed;


#define PD_LOG_TIMESTAMP_SHIFT 10 

#define PD_LOG_SIZE_MASK  0x1f
#define PD_LOG_PORT_MASK  0xe0
#define PD_LOG_PORT_SHIFT    5
#define PD_LOG_PORT_SIZE(port, size) (((port) << PD_LOG_PORT_SHIFT) | \
				      ((size) & PD_LOG_SIZE_MASK))
#define PD_LOG_PORT(size_port) ((size_port) >> PD_LOG_PORT_SHIFT)
#define PD_LOG_SIZE(size_port) ((size_port) & PD_LOG_SIZE_MASK)



#define PD_EVENT_MCU_BASE       0x00
#define PD_EVENT_MCU_CHARGE             (PD_EVENT_MCU_BASE+0)
#define PD_EVENT_MCU_CONNECT            (PD_EVENT_MCU_BASE+1)

#define PD_EVENT_MCU_BOARD_CUSTOM       (PD_EVENT_MCU_BASE+2)

#define PD_EVENT_ACC_BASE       0x20
#define PD_EVENT_ACC_RW_FAIL   (PD_EVENT_ACC_BASE+0)
#define PD_EVENT_ACC_RW_ERASE  (PD_EVENT_ACC_BASE+1)

#define PD_EVENT_PS_BASE        0x40
#define PD_EVENT_PS_FAULT      (PD_EVENT_PS_BASE+0)

#define PD_EVENT_VIDEO_BASE     0x60
#define PD_EVENT_VIDEO_DP_MODE (PD_EVENT_VIDEO_BASE+0)
#define PD_EVENT_VIDEO_CODEC   (PD_EVENT_VIDEO_BASE+1)

#define PD_EVENT_NO_ENTRY       0xff



#define CHARGE_FLAGS_DUAL_ROLE         BIT(15)

#define CHARGE_FLAGS_DELAYED_OVERRIDE  BIT(14)

#define CHARGE_FLAGS_OVERRIDE          BIT(13)

#define CHARGE_FLAGS_TYPE_SHIFT               3
#define CHARGE_FLAGS_TYPE_MASK       (0xf << CHARGE_FLAGS_TYPE_SHIFT)

#define CHARGE_FLAGS_ROLE_MASK         (7 <<  0)


#define PS_FAULT_OCP                          1
#define PS_FAULT_FAST_OCP                     2
#define PS_FAULT_OVP                          3
#define PS_FAULT_DISCH                        4


struct mcdp_version {
	uint8_t major;
	uint8_t minor;
	uint16_t build;
} __packed;

struct mcdp_info {
	uint8_t family[2];
	uint8_t chipid[2];
	struct mcdp_version irom;
	struct mcdp_version fw;
} __packed;


#define MCDP_CHIPID(chipid) ((chipid[0] << 8) | chipid[1])
#define MCDP_FAMILY(family) ((family[0] << 8) | family[1])


#define EC_CMD_USB_PD_MUX_INFO 0x11a

struct ec_params_usb_pd_mux_info {
	uint8_t port; 
} __packed;


#define USB_PD_MUX_USB_ENABLED       (1 << 0)
#define USB_PD_MUX_DP_ENABLED        (1 << 1)
#define USB_PD_MUX_POLARITY_INVERTED (1 << 2)
#define USB_PD_MUX_HPD_IRQ           (1 << 3)

struct ec_response_usb_pd_mux_info {
	uint8_t flags; 
} __packed;





#define EC_CMD_PASSTHRU_OFFSET(n) (0x4000 * (n))
#define EC_CMD_PASSTHRU_MAX(n) (EC_CMD_PASSTHRU_OFFSET(n) + 0x3fff)



#define EC_HOST_PARAM_SIZE      EC_PROTO2_MAX_PARAM_SIZE
#define EC_LPC_ADDR_OLD_PARAM   EC_HOST_CMD_REGION1
#define EC_OLD_PARAM_SIZE       EC_HOST_CMD_REGION_SIZE

#endif  
