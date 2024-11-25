

#ifndef _I915_COMPONENT_H_
#define _I915_COMPONENT_H_

#include "drm_audio_component.h"


#define MAX_PORTS 6


struct i915_audio_component {
	
	struct drm_audio_component	base;

	
	int aud_sample_rate[MAX_PORTS];
};

#endif 
