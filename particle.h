#pragma once

#include "texture.h"

typedef struct {
	AU_TextureRegion region;
	AU_Vector position, velocity, acceleration, scale, scale_velocity;
	float rotation, rotational_velocity;
	int lifetime;
} AU_Particle;
