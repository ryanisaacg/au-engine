#include "particle.h"

#include "util.h"

void au_particle_update(AU_Particle* part) {
	part->velocity = au_geom_vec_add(part->velocity, part->acceleration);
	part->position = au_geom_vec_add(part->position, part->velocity);
	part->scale = au_geom_vec_add(part->scale, part->scale_velocity);
	part->rotation += part->rotational_velocity;
	part->lifetime--;
}

AU_ParticleEmitter au_particle_emitter_new(AU_TextureRegion* possibleTextures, size_t num_textures) {
	return (AU_ParticleEmitter) {
		possibleTextures, num_textures, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0,
						  0, 0, 0, 0, 0
	};
}

AU_Particle au_particle_emitter_emit(AU_ParticleEmitter* emit) {
	return (AU_Particle) {
		emit->regions[au_util_randi_range(0, emit->num_textures)],
			 au_geom_vec_rand(emit->top_left, emit->bottom_right),
			 au_geom_vec_rand(emit->velocity_min, emit->velocity_max),
			 au_geom_vec_rand(emit->acceleration_min, emit->acceleration_max),
			 au_geom_vec_rand(emit->scale_min, emit->scale_max),
			 au_geom_vec_rand(emit->scale_velocity_min, emit->scale_velocity_max),
			 au_util_randf_range(emit->rotation_min, emit->rotation_max),
			 au_util_randf_range(emit->rotational_velocity_min, emit->rotational_velocity_max),
			 au_util_randi_range(emit->lifetime_min, emit->lifetime_max)
	};
}
