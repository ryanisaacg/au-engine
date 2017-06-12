#include "animation.h"

#include "memory.h"

AU_Animation au_anim_new(AU_TextureRegion region, int delay) {
	AU_Animation anim;
	anim.frames = au_memory_alloc(sizeof(AU_TextureRegion) * 16);
	anim.frames[0] = (AU_AnimationFrame) { region, delay };
	anim.frame_capacity = 16;
	anim.frame_count = 1;
	anim.time_elapsed = 0;
	anim.current_time = 0;
	return anim;
}

void au_anim_add_frame(AU_Animation* anim, AU_TextureRegion image, int delay) {
	if(anim->frame_count >= anim->frame_capacity) {
		anim->frame_capacity *= 2;
		anim->frames = au_memory_realloc(sizeof(AU_TextureRegion) * anim->frame_capacity);
	}
	anim->frames[anim->frame_count] = (AU_AnimationFrame) { image, delay };
	anim->frame_count++;
}

void au_anim_update(AU_Animation* anim) {
	anim->time_elapsed++;
	if(anim->time_elapsed >= anim->frames[anim->current_frame].frame_delay) {
		anim->time_elapsed = 0;
		anim->current_frame = (anim_current_frame + 1) % anim->frame_count;
	}

}

AU_TextureRegion au_anim_get_frame(AU_Animation* anim) {
	return anim->frames[anim->current_frame].image;
}

void au_anim_destroy(AU_Animation anim) {
	free(anim.frames);
}
