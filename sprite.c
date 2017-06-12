#include "sprite.h"

AU_SpriteTransform au_sprite_transform_new() {
	return (AU_SpriteTransform) {
		.x = 0,
		 .y = 0,
		  .width = 0,
		   .height = 0,
			.rotation = 0,
			 .origin_x = 0,
			  .origin_y = 0,
			   .scale_x = 1,
				.scale_y = 1,
				 .flip_x = false,
				  .flip_y = false,
				   .color = AU_WHITE,
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
