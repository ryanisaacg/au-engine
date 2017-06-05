#include "tmap.h"

#include <math.h>
#include <stdlib.h>

#include "memory.h"
#include "util.h"

#define VALID (x >= 0 && y >= 0 && x < AU_TL_WIDTH(map) && y < AU_TL_HEIGHT(map))

inline static int tl_index(AU_Tilemap map, int x, int y) {
	x /= AU_TL_TILE_WIDTH(map);
	y /= AU_TL_TILE_HEIGHT(map);
	return x * AU_TL_HEIGHT(map) / AU_TL_TILE_HEIGHT(map) + y + 4;
}

AU_Tilemap au_tmap_init(int width, int height, int tl_width, int tl_height) {
	AU_Tile* buffer = au_memory_calloc(4 + width / tl_width * height / tl_height, sizeof(AU_Tile));
	AU_TL_WIDTH(buffer) = width;
	AU_TL_HEIGHT(buffer) = height;
	AU_TL_TILE_WIDTH(buffer) = tl_width;
	AU_TL_TILE_HEIGHT(buffer) = tl_height;
	return buffer;
}

AU_Tile au_tmap_get (AU_Tilemap map, float x, float y) {
	if (VALID) {
		return map[tl_index(map, x, y)];
	} else {
		return -1;
	}
}

void au_tmap_set (AU_Tilemap map, float x, float y, AU_Tile val) {
	if (VALID) {
		map[tl_index(map, x, y)] = val;
	}
}

AU_Tile au_tmap_first (AU_Tilemap map, float x, float y, float w, float h) {
	int left = (int)floor(x / AU_TL_TILE_WIDTH(map));
	int top = (int)floor(y / AU_TL_TILE_HEIGHT(map));
	int right = (int)floor((x + w) / AU_TL_TILE_WIDTH(map));
	int bottom = (int)floor((y + h) / AU_TL_TILE_HEIGHT(map));
	if (left < 0 || top < 0 || right > AU_TL_WIDTH(map) / AU_TL_TILE_WIDTH(map) || bottom > AU_TL_HEIGHT(map) / AU_TL_TILE_HEIGHT(map)) {
		return -1;
	}
	for (int i = left; i <= right; i++) {
		for (int j = top; j <= bottom; j++) {
			int index = i * AU_TL_HEIGHT(map) / AU_TL_TILE_HEIGHT(map) + j + 4;
			if (map[index]) {
				return map[index];
			}
		}
	}
	return 0;
}

AU_Tile au_tmap_first_rect(AU_Tilemap m, AU_Rectangle r) {
	return au_tmap_first(m, r.x, r.y, r.width, r.height);
}

#include <stdio.h>

AU_Vector au_tmap_slide(AU_Tilemap m, AU_Rectangle r, AU_Vector v) {
	if (!au_tmap_first(m, r.x + v.x, r.y + v.y, r.width, r.height)) {
		return v;
	} else {
		while (au_tmap_first(m, r.x + v.x, r.y, r.width, r.height)) {
			if (fabs(v.x) < 0.1f) {
				v.x = 0;
				break;
			}
			v.x *= 0.5f;
		}
		while (au_tmap_first(m, r.x + v.x, r.y + v.y, r.width, r.height)) {
			if (fabs(v.y) < 0.1f) {
				v.y = 0;
				break;
			}
			v.y *= 0.5f;
		}
		return v;
	}
}
