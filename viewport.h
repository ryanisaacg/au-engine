#pragma once

#include "geom.h"

typedef enum { STRETCH, LETTERBOX, FILL } AU_ViewportType;

typedef struct {
	AU_ViewportType type;
	float aspect_ratio;
} AU_Viewport;

AU_Viewport au_viewport_new(AU_ViewportType, float);
AU_Rectangle au_viewport_screen_region(AU_Viewport, float window_width, float window_height);
void au_viewport_apply(AU_Viewport, float window_width, float window_height);
AU_Vector au_viewport_project(AU_Viewport, AU_Vector, float width, float height);
AU_Vector au_viewport_unproject(AU_Viewport, AU_Vector, float width, float height);
