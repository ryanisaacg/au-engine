#pragma once

#include "texture.h"

typedef struct {
	AU_TextureRegion image;
	int frame_delay;
} AU_AnimationFrame;

typedef struct {
	AU_AnimationFrame* frames;
	int current_frame, time_elapsed;
	size_t num_frames, frame_capacity;
} AU_Animation;

AU_Animation au_anim_new();
void au_anim_add_frame(AU_Animation*, AU_TextureRegion, int delay);
void au_anim_destroy(AU_Animation);
