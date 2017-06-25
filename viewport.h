#pragma once

#include "geom.h"

typedef enum { STRETCH, LETTERBOX, FILL } AU_ViewportType;

typedef struct {
	AU_ViewportType type;
	float aspect_ratio;
} AU_Viewport;

AU_Viewport au_viewport_new(AU_ViewportType, float);
void au_viewport_apply(AU_Viewport*, float window_width, float window_height);
