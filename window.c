#include "window.h"

#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

AU_Engine *AU_Init(char* title, int w, int h) {
	AU_Engine* engine = chk_alloc(sizeof(AU_Engine));
	engine->ctx = AU_Context_Init_Stack(title, w, h);
	return engine;
}

AU_Texture AU_LoadTexture(AU_Engine* eng, char* name) {
	GPU_Image* image = GPU_LoadImage(name);
	if(image == NULL) {
		fprintf(stderr, "Failed to load image with filename %s", name);
		exit(1);
	}
	int id = AU_Context_RegisterTexture(&(eng->ctx), image);
	return (AU_Texture) { id, image->w, image->h };
}
