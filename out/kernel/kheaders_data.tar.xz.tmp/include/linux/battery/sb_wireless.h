

#ifndef __SB_WIRELESS_H__
#define __SB_WIRELESS_H__

enum wpc_op_mode {
	WPC_OP_MODE_NONE = 0, 
	WPC_OP_MODE_BPP,
	WPC_OP_MODE_PPDE,
	WPC_OP_MODE_EPP,
	WPC_OP_MODE_MPP,
	WPC_OP_MODE_MAX
};

enum wpc_auth_mode {
	WPC_AUTH_MODE_NONE = 0,
	WPC_AUTH_MODE_BPP,
	WPC_AUTH_MODE_PPDE,
	WPC_AUTH_MODE_EPP,
	WPC_AUTH_MODE_MPP,
	WPC_AUTH_MODE_MAX
};

struct sb_wireless_op {
	int (*get_op_mode)(void *pdata);
	int (*get_qi_ver)(void *pdata);
	int (*get_auth_mode)(void *pdata);
};

const char *sb_wrl_op_mode_str(int op_mode);

int sb_wireless_set_op(void *pdata, const struct sb_wireless_op *op);

#endif 
