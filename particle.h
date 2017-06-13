#pragma once

#include "texture.h"

typedef struct {
	AU_TextureRegion region;
	AU_Vector position, velocity, acceleration, scale, scale_velocity;
	float rotation, rotational_velocity;
	int lifetime;
} AU_Particle;

typedef struct {
	AU_Vector top_left, bottom_right, velocity_min, velocity_max, acceleration_min, acceleration_max, scale_min, scale_max,
			  scale_velocity_min, scale_velocity_min;
	float rotation_min, rotation_max, rotational_velocity_min, rotational_velocity_max;
	int lifetime_min, lifetime_max;
} AU_ParticleEmitter;
