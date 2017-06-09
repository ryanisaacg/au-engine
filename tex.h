#pragma once

#include "geom.h"

typedef struct {
	int id, width, height;
} AU_Texture;

typedef struct {
	AU_Texture source;
	AU_Rectangle rect;
} AU_TextureRegion;

//Turns the entire texture into a texture region
AU_TextureRegion au_tex_region(AU_Texture);
