/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright(c) 2009-2010  Realtek Corporation.
 *
 *****************************************************************************/

#ifndef __RTW_WIFI_REGD_H__
#define __RTW_WIFI_REGD_H__

<<<<<<< HEAD
struct country_code_to_enum_rd {
	u16 countrycode;
	const char *iso_name;
};

enum country_code_type_t {
	COUNTRY_CODE_USER = 0,

	/*add new channel plan above this line */
	COUNTRY_CODE_MAX
};

int rtw_regd_init(struct adapter *padapter,
	void (*reg_notifier)(struct wiphy *wiphy,
		struct regulatory_request *request));
=======
void rtw_regd_init(struct wiphy *wiphy,
		   void (*reg_notifier)(struct wiphy *wiphy,
					struct regulatory_request *request));
>>>>>>> upstream/android-13
void rtw_reg_notifier(struct wiphy *wiphy, struct regulatory_request *request);


#endif
