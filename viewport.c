#include "viewport.h"

#include <glad/glad.h>
#include <stdio.h>

AU_Viewport au_viewport_new(AU_ViewportType type, AU_Rectangle rect, float ratio) {
	return (AU_Viewport) { rect, type, ratio };
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

void au_viewport_apply(AU_Viewport* viewport, float window_width, float window_height) {
	AU_Rectangle area = viewport->area;
	switch(viewport->type) {
	case STRETCH:
		break;
	case LETTERBOX:
		if(area.width > area.height * viewport->aspect_ratio) {
			area = vertical(area, viewport->aspect_ratio);
		} else {
			area = horizontal(area, viewport->aspect_ratio);
		}
		break;
	case FILL:
		if(area.width > area.height * viewport->aspect_ratio) {
			area = horizontal(area, viewport->aspect_ratio);
		} else {
			area = vertical(area, viewport->aspect_ratio);
		}
		break;
	}
	glViewport(area.x, area.y, area.width, area.height);
}
