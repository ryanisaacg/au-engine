#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

void* chk_alloc(size_t length) {
	void* buf = malloc(length);
	if(buf == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return buf;
}

void* chk_realloc(void* buffer, size_t length) {
	void* buf = realloc(buffer, length);
	if(buf == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return buf;
}
