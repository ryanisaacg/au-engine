#include "sprite.h"

AU_SpriteTransform au_sprite_transform_new() {
	return (AU_SpriteTransform) {
		0, 0, 0, 0, 0, 0, 0, 1, 1, false, false, AU_WHITE
	};
}

AU_Sprite au_sprite_new(AU_TextureRegion region) {
	return (AU_Sprite) {
		au_sprite_transform_new(), region
	};
}

AU_AnimatedSprite au_sprite_anim_new(AU_AnimationManager manager) {
	return (AU_AnimatedSprite) {
		au_sprite_transform_new(), manager
	};
}
