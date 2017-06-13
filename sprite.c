#include "sprite.h"

AU_SpriteTransform au_sprite_transform_new() {
	return (AU_SpriteTransform) {
		0, 0, 0, 0, 0, 0, 0, 1, 1, false, false, AU_WHITE
	};
}

static AU_SpriteTransform au_sprite_transform_from_region(AU_TextureRegion region) {
	AU_SpriteTransform trans = au_sprite_transform_new();
	trans.width = region.rect.width;
	trans.height = region.rect.height;
	return trans;
}

AU_Sprite au_sprite_new(AU_TextureRegion region) {
	return (AU_Sprite) {
		au_sprite_transform_from_region(region), region
	};
}

AU_AnimatedSprite au_sprite_anim_new(AU_AnimationManager manager) {
	return (AU_AnimatedSprite) {
		au_sprite_transform_from_region(au_anim_manager_get_frame(&manager)), manager
	};
}
