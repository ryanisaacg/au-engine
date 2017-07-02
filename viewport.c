#include "viewport.h"

#include <glad/glad.h>
#include <stdio.h>

AU_Viewport au_viewport_new(AU_ViewportType type, float ratio) {
	return (AU_Viewport) { type, ratio };
}

inline static AU_Rectangle horizontal(AU_Rectangle r, float aspect_ratio) {
	AU_Rectangle orig = r;
	r.height = r.width / aspect_ratio;
	r.y += (orig.height - r.height) / 2;
	return r;
}

inline static AU_Rectangle vertical(AU_Rectangle r, float aspect_ratio) {
	AU_Rectangle orig = r;
	r.width = r.height * aspect_ratio;
	r.x += (orig.width - r.width) / 2;
	return r;
}

AU_Rectangle au_viewport_screen_region(AU_Viewport viewport, float window_width, float window_height) {
	AU_Rectangle area = (AU_Rectangle) { 0, 0, window_width, window_height };
	switch(viewport.type) {
	case STRETCH:
		break;
	case LETTERBOX:
		if(area.width > area.height * viewport.aspect_ratio) {
			area = vertical(area, viewport.aspect_ratio);
		} else {
			area = horizontal(area, viewport.aspect_ratio);
		}
		break;
	case FILL:
		if(area.width > area.height * viewport.aspect_ratio) {
			area = horizontal(area, viewport.aspect_ratio);
		} else {
			area = vertical(area, viewport.aspect_ratio);
		}
		break;
	}
	return area;
}

void au_viewport_apply(AU_Viewport viewport, float window_width, float window_height) {
	AU_Rectangle area = au_viewport_screen_region(viewport, window_width, window_height);
	glViewport(area.x, area.y, area.width, area.height);
}

AU_Vector au_viewport_project(AU_Viewport port, AU_Vector v, float width, float height) {
	AU_Rectangle region = au_viewport_screen_region(port, width, height);
	return (AU_Vector) { v.x + region.x, v.y + region.y };
}

AU_Vector au_viewport_unproject(AU_Viewport port, AU_Vector v, float width, float height) {
	AU_Rectangle region = au_viewport_screen_region(port, width, height);
	return (AU_Vector) { v.x - region.x, v.y - region.y };
}
