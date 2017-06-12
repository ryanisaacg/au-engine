#include "animation.h"

#include <stdio.h>

#include "memory.h"

AU_Animation au_anim_new(AU_TextureRegion region, int delay) {
	AU_Animation anim;
	anim.frames = au_memory_alloc(sizeof(AU_AnimationFrame) * 16);
	anim.frames[0] = (AU_AnimationFrame) {
		region, delay
	};
	anim.frame_capacity = 16;
	anim.frame_count = 1;
	anim.time_elapsed = 0;
	anim.current_frame= 0;
	return anim;
}

AU_Animation au_anim_from_array(AU_TextureRegion* buffer, size_t length, int delay) {
	AU_Animation anim = au_anim_new(buffer[0], delay);
	for(unsigned int i = 1; i < length; i++) {
		au_anim_add_frame(&anim, buffer[i], delay);
	}
	return anim;
}

void au_anim_add_frame(AU_Animation* anim, AU_TextureRegion image, int delay) {
	if (anim->frame_count >= anim->frame_capacity) {
		anim->frame_capacity *= 2;
		anim->frames = au_memory_realloc(anim->frames, sizeof(AU_TextureRegion) * anim->frame_capacity);
	}
	anim->frames[anim->frame_count] = (AU_AnimationFrame) {
		image, delay
	};
	anim->frame_count++;
}

void au_anim_update(AU_Animation* anim) {
	anim->time_elapsed++;
	if (anim->time_elapsed >= anim->frames[anim->current_frame].frame_delay) {
		anim->time_elapsed = 0;
		anim->current_frame = (anim->current_frame + 1) % anim->frame_count;
	}

}

AU_TextureRegion au_anim_get_frame(AU_Animation* anim) {
	return anim->frames[anim->current_frame].image;
}

void au_anim_destroy(AU_Animation anim) {
	free(anim.frames);
}

AU_AnimationManager au_anim_manager_new() {
	AU_AnimationManager manager;
	manager.animations = au_memory_alloc(sizeof(AU_Animation) * 4);
	manager.anim_count = 0;
	manager.anim_capacity = 4;
	manager.current_anim = -1;
	return manager;
}

int au_anim_manager_register(AU_AnimationManager* manager, AU_Animation* anim) {
	if (manager->anim_count >= manager->anim_capacity) {
		manager->anim_capacity *= 2;
		manager->animations = au_memory_realloc(manager->animations, sizeof(AU_Animation) * manager->anim_capacity);
	}
	manager->animations[manager->anim_count] = *anim;
	int index = manager->anim_count;
	manager->anim_count++;
	return index;
}

void au_anim_manager_switch(AU_AnimationManager* manager, int index) {
	manager->current_anim = index;
}

void au_anim_manager_update(AU_AnimationManager* manager) {
	if (manager->current_anim == -1) {
		fprintf(stderr, "Tried to update an unset animation manager\n");
		exit(1);
	}
	au_anim_update(manager->animations + manager->current_anim);
}

AU_TextureRegion au_anim_manager_get_frame(AU_AnimationManager* manager) {
	if (manager->current_anim == -1) {
		fprintf(stderr, "Tried to get a frame from an unset animation manager\n");
		exit(1);
	}
	return au_anim_get_frame(manager->animations + manager->current_anim);
}

void au_anim_manager_destroy(AU_AnimationManager manager) {
	for (unsigned int i = 0; i < manager.anim_count; i++) {
		au_anim_destroy(manager.animations[i]);
	}
	free(manager.animations);
}

