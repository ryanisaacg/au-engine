#include "animation.h"

#include "memory.h"

AU_Animation au_anim_new() {
	AU_Animation anim;
	anim.frames = au_memory_alloc(sizeof(AU_TextureRegion) * 16);
	anim.frame_capacity = 16;
}

void au_anim_add_frame(AU_Animation* anim, AU_TextureRegion image, int delay) {

}

void au_anim_destroy(AU_Animation anim) {

}
