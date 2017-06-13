#pragma once

#include <stddef.h>

#include "texture.h"

typedef struct {
	AU_TextureRegion region;
	AU_Vector position, velocity, acceleration, scale, scale_velocity;
	float rotation, rotational_velocity;
	int lifetime;
} AU_Particle;

void au_particle_update(AU_Particle*);

typedef struct {
	AU_TextureRegion* regions;
	size_t num_textures;
	AU_Vector top_left, bottom_right, velocity_min, velocity_max, acceleration_min, acceleration_max, scale_min, scale_max,
			  scale_velocity_min, scale_velocity_max;
	float rotation_min, rotation_max, rotational_velocity_min, rotational_velocity_max;
	int lifetime_min, lifetime_max;
	int particle_min, particle_max;
} AU_ParticleEmitter;

AU_ParticleEmitter au_particle_emitter_new(AU_TextureRegion* possibleTextures, size_t num_textures);
AU_Particle au_particle_emitter_emit(AU_ParticleEmitter*);
