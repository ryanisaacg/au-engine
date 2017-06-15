#include <stdio.h>
#include <stdlib.h>

#include "geom.h"
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

//TODO: change the function to be in-place

PackedRectangles pack_rects(AU_Rectangle* rects, size_t length) {
	qsort(rects, length, sizeof(AU_Rectangle), &compare_rects);
	float max_width = 0;
	for(size_t i = 0; i < length; i++) {
		if(rects[i].width > max_width) 
			max_width = rects[i].width;
	}
	max_width *= 4; //The goal is to be able to maximize level efficiency
	AU_Rectangle* packed = au_memory_alloc(sizeof(AU_Rectangle) * length);
	float current_width = 0, current_y = 0, current_height = rects[0].height;
	for(size_t i = 0; i < length; i++) {
		if(current_width + rects[i].width > max_width) {
			current_width = 0;
			current_y += current_height;
			current_height = rects[i].height;
		}
		packed[i] = (AU_Rectangle) { current_width, current_y, rects[i].width, rects[i].height };
		current_width += rects[i].width;	
	}
	return (PackedRectangles) { packed, length, (AU_Rectangle) { 0, 0, max_width, current_y + current_height } };
}

void print_rect(AU_Rectangle r) {
	printf("%f:%f:%f:%f\n", r.x, r.y, r.width, r.height);
}

int main() {
	AU_Rectangle rectangles[] = {{0, 0, 100, 50}, {0, 0, 12, 12}, {0, 0, 32, 32}, {0, 0, 48, 48}, {9, 9, 32, 32}, {0, 0, 32, 32}, {0, 0, 32, 32}, {0, 0, 48, 48}, {0, 0, 128, 128}, {0, 0, 128, 4}, {0, 0, 16, 48}};
	PackedRectangles packed = pack_rects(rectangles, sizeof(rectangles) / sizeof(AU_Rectangle));
	printf("%f\n", packed.total_size.width * packed.total_size.height);
	print_rect(packed.total_size);
	for(size_t i = 0; i < packed.rectangle_count; i++) {
		print_rect(packed.packed[i]);
	}
	return 0;
}
