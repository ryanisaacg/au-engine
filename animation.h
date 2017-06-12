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

typedef struct {
	AU_Animation* animations;
	size_t anim_count, anim_capacity;
	int current_anim;
} AU_AnimationManager;

//Create an animation manager
AU_AnimationManager au_anim_manager_new();
//Register an animation and return its index
int au_anim_manager_register(AU_AnimationManager*, AU_Animation*);
//Switch to a given animation
void au_anim_manager_switch(AU_AnimationManager*, int);
//Update the current animation
void au_anim_manager_update(AU_AnimationManager*);
//Get the current frame from the manager
AU_TextureRegion au_anim_manager_get_frame(AU_AnimationManager*);
//Destroy the resources allocated by the animation manager
void au_anim_manager_destroy(AU_AnimationManager);

