

#ifndef __LM3646_H__
#define __LM3646_H__

#include <media/v4l2-subdev.h>

#define LM3646_NAME	"lm3646"
#define LM3646_I2C_ADDR_REV1	(0x67)
#define LM3646_I2C_ADDR_REV0	(0x63)


#define LM3646_TOTAL_FLASH_BRT_MIN 93350
#define LM3646_TOTAL_FLASH_BRT_STEP 93750
#define LM3646_TOTAL_FLASH_BRT_MAX 1499600
#define LM3646_TOTAL_FLASH_BRT_uA_TO_REG(a)	\
	((a) < LM3646_TOTAL_FLASH_BRT_MIN ? 0 :	\
	 ((((a) - LM3646_TOTAL_FLASH_BRT_MIN) / LM3646_TOTAL_FLASH_BRT_STEP)))


#define LM3646_TOTAL_TORCH_BRT_MIN 23040
#define LM3646_TOTAL_TORCH_BRT_STEP 23430
#define LM3646_TOTAL_TORCH_BRT_MAX 187100
#define LM3646_TOTAL_TORCH_BRT_uA_TO_REG(a)	\
	((a) < LM3646_TOTAL_TORCH_BRT_MIN ? 0 :	\
	 ((((a) - LM3646_TOTAL_TORCH_BRT_MIN) / LM3646_TOTAL_TORCH_BRT_STEP)))


#define LM3646_LED1_FLASH_BRT_MIN 23040
#define LM3646_LED1_FLASH_BRT_STEP 11718
#define LM3646_LED1_FLASH_BRT_MAX 1499600
#define LM3646_LED1_FLASH_BRT_uA_TO_REG(a)	\
	((a) <= LM3646_LED1_FLASH_BRT_MIN ? 0 :	\
	 ((((a) - LM3646_LED1_FLASH_BRT_MIN) / LM3646_LED1_FLASH_BRT_STEP))+1)


#define LM3646_LED1_TORCH_BRT_MIN 2530
#define LM3646_LED1_TORCH_BRT_STEP 1460
#define LM3646_LED1_TORCH_BRT_MAX 187100
#define LM3646_LED1_TORCH_BRT_uA_TO_REG(a)	\
	((a) <= LM3646_LED1_TORCH_BRT_MIN ? 0 :	\
	 ((((a) - LM3646_LED1_TORCH_BRT_MIN) / LM3646_LED1_TORCH_BRT_STEP))+1)


#define LM3646_FLASH_TOUT_MIN 50
#define LM3646_FLASH_TOUT_STEP 50
#define LM3646_FLASH_TOUT_MAX 400
#define LM3646_FLASH_TOUT_ms_TO_REG(a)	\
	((a) <= LM3646_FLASH_TOUT_MIN ? 0 :	\
	 (((a) - LM3646_FLASH_TOUT_MIN) / LM3646_FLASH_TOUT_STEP))


struct lm3646_platform_data {

	u32 flash_timeout;

	u32 led1_flash_brt;
	u32 led1_torch_brt;
};

#endif 
