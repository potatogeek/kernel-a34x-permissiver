// SPDX-License-Identifier: GPL-2.0


#ifndef __VT_MUIC_H__
#define __VT_MUIC_H__

struct vt_muic_ops {
	void (*afc_dpdm_ctrl)(bool onoff);
	int (*afc_set_voltage)(int voltage);
	int (*get_adc)(void *mdata);
	int (*get_vbus_value)(void *mdata);
	void (*set_afc_disable)(void *mdata);
	int (*set_hiccup_mode)(void *mdata, bool en);
};

struct vt_muic_ic_data {
	struct vt_muic_ops m_ops;
	void *mdata;
};

#if IS_ENABLED(CONFIG_VIRTUAL_MUIC)
extern void vt_muic_set_attached_afc_dev(int attached_afc_dev);
extern int vt_muic_get_attached_dev(void);
extern int vt_muic_get_afc_disable(void);
extern void vt_muic_register_ic_data(struct vt_muic_ic_data *ic_data);
#else
static inline void vt_muic_set_attached_afc_dev(int attached_afc_dev)
	{return; }
static inline int vt_muic_get_attached_dev(void)
	{return 0; }
static inline int vt_muic_get_afc_disable(void)
	{return 0; }
static inline void vt_muic_register_ic_data(struct vt_muic_ic_data *ic_data)
	{return; }
#endif

#endif 
