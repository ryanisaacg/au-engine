#include <stdio.h>
#include <stdlib.h>

#include "au.h"
#include "memory.h"

typedef struct {
	AU_Rectangle* packed;
	size_t rectangle_count;
	AU_Rectangle total_size;
} PackedRectangles;

//Compare two rectangles: higher heights go first
int compare_rects(const void* ptr1, const void* ptr2) {
	const AU_Rectangle* r1 = ptr1;
	const AU_Rectangle* r2 = ptr2;
	return r2->height - r1->height;
}

AU_Rectangle pack_rects(AU_Rectangle* rects, size_t length) {
	qsort(rects, length, sizeof(AU_Rectangle), &compare_rects);
	float max_width = 0;
	for(size_t i = 0; i < length; i++) {
		if(rects[i].width > max_width) 
			max_width = rects[i].width;
	}
	max_width *= 4; //The goal is to be able to maximize level efficiency
	float current_width = 0, current_y = 0, current_height = rects[0].height;
	for(size_t i = 0; i < length; i++) {
		if(current_width + rects[i].width > max_width) {
			current_width = 0;
			current_y += current_height;
			current_height = rects[i].height;
		}
		float old_width = current_width;
		current_width += rects[i].width;	
		rects[i] = (AU_Rectangle) { old_width, current_y, rects[i].width, rects[i].height };
	}
	return (AU_Rectangle) { 0, 0, max_width, current_y + current_height };
}

void pack_to_file(char** filenames, size_t length, char* image_file, FILE* manifest) {
	SDL_Surface** surfaces = au_memory_alloc(sizeof(SDL_Surface*) * length);
	AU_Rectangle* rectangles = au_memory_alloc(sizeof(AU_Rectangle) * length);
	for(size_t i = 0; i < length; i++) {
		surfaces[i] = GPU_LoadSurface(filenames[i]);
		rectangles[i] = (AU_Rectangle) { 0, 0, surfaces[i]->w, surfaces[i]->h };
	}
	AU_Rectangle total = pack_rects(rectangles, length);
	SDL_Surface* full_surface =  SDL_CreateRGBSurface(0, total.width, total.height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	for(size_t i = 0; i < length; i++) {
		SDL_Rect dest = {(int)rectangles[i].x, (int)rectangles[i].y, (int)rectangles[i].width, (int)rectangles[i].height };
		SDL_BlitSurface(surfaces[i], NULL, full_surface, &dest);
	}
	GPU_SaveSurface(full_surface, image_file, GPU_FILE_AUTO);
	for(size_t i = 0; i < length; i++) {
		fprintf(manifest, "%f %f %f %f \n", rectangles[i].x, rectangles[i].y, rectangles[i].width, rectangles[i].height);
		fprintf(manifest, "%s\n", filenames[i]);
	}
}

void print_rect(AU_Rectangle r) {
	printf("%f:%f:%f:%f\n", r.x, r.y, r.width, r.height);
}


int main(int argc, char *argv[]) {
	if(argc < 3) {
		printf("Usage: packer [image-filename] [manifest-filename] [input images]...\n");
	} else {
		FILE* manifest = fopen(argv[2], "w");
		pack_to_file(argv + 3, argc - 3, argv[1], manifest);
		fclose(manifest);
	}
	return 0;
}
