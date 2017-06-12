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

//Instantiate an animation
AU_Animation au_anim_new(AU_TextureRegion initial, int delay);
//Append a frame to an animation
void au_anim_add_frame(AU_Animation*, AU_TextureRegion, int delay);
//Update the animation by a frame
void au_anim_update(AU_Animation*);
//Get the current frame from the animation
AU_TextureRegion au_anim_get_frame(AU_Animation*);
//Destroy the resources allocated by the animation
void au_anim_destroy(AU_Animation);
