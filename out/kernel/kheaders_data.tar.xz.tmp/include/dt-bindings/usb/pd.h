/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __DT_POWER_DELIVERY_H
#define __DT_POWER_DELIVERY_H


#define PDO_TYPE_FIXED		0
#define PDO_TYPE_BATT		1
#define PDO_TYPE_VAR		2
#define PDO_TYPE_APDO		3

#define PDO_TYPE_SHIFT		30
#define PDO_TYPE_MASK		0x3

#define PDO_TYPE(t)	((t) << PDO_TYPE_SHIFT)

#define PDO_VOLT_MASK		0x3ff
#define PDO_CURR_MASK		0x3ff
#define PDO_PWR_MASK		0x3ff

#define PDO_FIXED_DUAL_ROLE	(1 << 29) 
#define PDO_FIXED_SUSPEND	(1 << 28) 
#define PDO_FIXED_HIGHER_CAP	(1 << 28) 
#define PDO_FIXED_EXTPOWER	(1 << 27) 
#define PDO_FIXED_USB_COMM	(1 << 26) 
#define PDO_FIXED_DATA_SWAP	(1 << 25) 
#define PDO_FIXED_VOLT_SHIFT	10	
#define PDO_FIXED_CURR_SHIFT	0	

#define PDO_FIXED_VOLT(mv)	((((mv) / 50) & PDO_VOLT_MASK) << PDO_FIXED_VOLT_SHIFT)
#define PDO_FIXED_CURR(ma)	((((ma) / 10) & PDO_CURR_MASK) << PDO_FIXED_CURR_SHIFT)

#define PDO_FIXED(mv, ma, flags)			\
	(PDO_TYPE(PDO_TYPE_FIXED) | (flags) |		\
	 PDO_FIXED_VOLT(mv) | PDO_FIXED_CURR(ma))

#define VSAFE5V 5000 

#define PDO_BATT_MAX_VOLT_SHIFT	20	
#define PDO_BATT_MIN_VOLT_SHIFT	10	
#define PDO_BATT_MAX_PWR_SHIFT	0	

#define PDO_BATT_MIN_VOLT(mv) ((((mv) / 50) & PDO_VOLT_MASK) << PDO_BATT_MIN_VOLT_SHIFT)
#define PDO_BATT_MAX_VOLT(mv) ((((mv) / 50) & PDO_VOLT_MASK) << PDO_BATT_MAX_VOLT_SHIFT)
#define PDO_BATT_MAX_POWER(mw) ((((mw) / 250) & PDO_PWR_MASK) << PDO_BATT_MAX_PWR_SHIFT)

#define PDO_BATT(min_mv, max_mv, max_mw)			\
	(PDO_TYPE(PDO_TYPE_BATT) | PDO_BATT_MIN_VOLT(min_mv) |	\
	 PDO_BATT_MAX_VOLT(max_mv) | PDO_BATT_MAX_POWER(max_mw))

#define PDO_VAR_MAX_VOLT_SHIFT	20	
#define PDO_VAR_MIN_VOLT_SHIFT	10	
#define PDO_VAR_MAX_CURR_SHIFT	0	

#define PDO_VAR_MIN_VOLT(mv) ((((mv) / 50) & PDO_VOLT_MASK) << PDO_VAR_MIN_VOLT_SHIFT)
#define PDO_VAR_MAX_VOLT(mv) ((((mv) / 50) & PDO_VOLT_MASK) << PDO_VAR_MAX_VOLT_SHIFT)
#define PDO_VAR_MAX_CURR(ma) ((((ma) / 10) & PDO_CURR_MASK) << PDO_VAR_MAX_CURR_SHIFT)

#define PDO_VAR(min_mv, max_mv, max_ma)				\
	(PDO_TYPE(PDO_TYPE_VAR) | PDO_VAR_MIN_VOLT(min_mv) |	\
	 PDO_VAR_MAX_VOLT(max_mv) | PDO_VAR_MAX_CURR(max_ma))

 #endif 
