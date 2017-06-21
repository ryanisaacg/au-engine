#pragma once

#include "animation.h"
#include "color.h"
#include "texture.h"

typedef struct {
	float x, y, width, height, rotation, origin_x, origin_y, scale_x, scale_y;
	bool flip_x, flip_y;
	AU_Color color;
	int depth;
} AU_SpriteTransform;

AU_SpriteTransform au_sprite_transform_new();

typedef struct {
	AU_SpriteTransform transform;
	AU_TextureRegion region;
} AU_Sprite;

AU_Sprite au_sprite_new(AU_TextureRegion);

typedef struct {
	AU_SpriteTransform transform;
	AU_AnimationManager animations;
} AU_AnimatedSprite;

AU_AnimatedSprite au_sprite_anim_new(AU_AnimationManager);

