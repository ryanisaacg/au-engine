#include "window.h"

#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

AU_Engine *au_init(char* title, int w, int h) {
	AU_Engine* engine = chk_alloc(sizeof(AU_Engine));
	engine->ctx = au_context_init_stack(title, w, h);
	return engine;
}

AU_Texture au_load_texture(AU_Engine* eng, char* name) {
	GPU_Image* image = GPU_LoadImage(name);
	if(image == NULL) {
		fprintf(stderr, "Failed to load image with filename %s", name);
		exit(1);
	}
	int id = au_context_register_texture(&(eng->ctx), image);
	return (AU_Texture) { id, image->w, image->h };
}
