#pragma once

#include "geom.h"

typedef int AU_Tile;
typedef AU_Tile* AU_Tilemap;

#define AU_TL_WIDTH(m) m[2]
#define AU_TL_HEIGHT(m) m[3]
#define AU_TL_TILE_WIDTH(m) m[0]
#define AU_TL_TILE_HEIGHT(m) m[1]

AU_Tilemap au_tmap_init(int width, int height, int tl_width, int tl_height);
AU_Tile au_tmap_get(AU_Tilemap map, float x, float y);
void au_tmap_set(AU_Tilemap map, float x, float y, AU_Tile tile);
AU_Tile au_tmap_first(AU_Tilemap map, float x, float y, float width, float height);
AU_Tile au_tmap_first_rect(AU_Tilemap map, AU_Rectangle rect);
AU_Vector au_tmap_slide(AU_Tilemap map, AU_Rectangle rect, AU_Vector vec);
