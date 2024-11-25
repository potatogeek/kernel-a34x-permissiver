

#ifndef __DRM_PANEL_H__
#define __DRM_PANEL_H__

#include <linux/err.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/notifier.h>


#define DRM_PANEL_EVENT_BLANK		0x01

#define DRM_PANEL_EARLY_EVENT_BLANK	0x02

enum {
	
	DRM_PANEL_BLANK_UNBLANK,
	
	DRM_PANEL_BLANK_POWERDOWN,
};

struct drm_panel_notifier {
	int refresh_rate;
	void *data;
	uint32_t id;
};

struct device_node;
struct drm_connector;
struct drm_device;
struct drm_panel;
struct display_timing;


struct drm_panel_funcs {
	int (*disable)(struct drm_panel *panel);
	int (*unprepare)(struct drm_panel *panel);
	int (*prepare)(struct drm_panel *panel);
	int (*enable)(struct drm_panel *panel);
	int (*get_modes)(struct drm_panel *panel);
	int (*get_timings)(struct drm_panel *panel, unsigned int num_timings,
			   struct display_timing *timings);
};


struct drm_panel {
	struct drm_device *drm;
	struct drm_connector *connector;
	struct device *dev;

	const struct drm_panel_funcs *funcs;

	struct list_head list;

	
	struct blocking_notifier_head nh;
};


static inline int drm_panel_unprepare(struct drm_panel *panel)
{
	if (panel && panel->funcs && panel->funcs->unprepare)
		return panel->funcs->unprepare(panel);

	return panel ? -ENOSYS : -EINVAL;
}


static inline int drm_panel_disable(struct drm_panel *panel)
{
	if (panel && panel->funcs && panel->funcs->disable)
		return panel->funcs->disable(panel);

	return panel ? -ENOSYS : -EINVAL;
}


static inline int drm_panel_prepare(struct drm_panel *panel)
{
	if (panel && panel->funcs && panel->funcs->prepare)
		return panel->funcs->prepare(panel);

	return panel ? -ENOSYS : -EINVAL;
}


static inline int drm_panel_enable(struct drm_panel *panel)
{
	if (panel && panel->funcs && panel->funcs->enable)
		return panel->funcs->enable(panel);

	return panel ? -ENOSYS : -EINVAL;
}


static inline int drm_panel_get_modes(struct drm_panel *panel)
{
	if (panel && panel->funcs && panel->funcs->get_modes)
		return panel->funcs->get_modes(panel);

	return panel ? -ENOSYS : -EINVAL;
}

void drm_panel_init(struct drm_panel *panel);

int drm_panel_add(struct drm_panel *panel);
void drm_panel_remove(struct drm_panel *panel);

int drm_panel_attach(struct drm_panel *panel, struct drm_connector *connector);
int drm_panel_detach(struct drm_panel *panel);

int drm_panel_notifier_register(struct drm_panel *panel,
	struct notifier_block *nb);
int drm_panel_notifier_unregister(struct drm_panel *panel,
	struct notifier_block *nb);
int drm_panel_notifier_call_chain(struct drm_panel *panel,
	unsigned long val, void *v);

#if defined(CONFIG_OF) && defined(CONFIG_DRM_PANEL)
struct drm_panel *of_drm_find_panel(const struct device_node *np);
#else
static inline struct drm_panel *of_drm_find_panel(const struct device_node *np)
{
	return ERR_PTR(-ENODEV);
}
#endif

#endif
