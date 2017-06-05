#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

void* au_memory_alloc(size_t length) {
	void* buf = malloc(length);
	if (buf == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return buf;
}

void* au_memory_calloc(size_t num, size_t length) {
	void* buf = calloc(num, length);
	if (buf == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return buf;
}

void* au_memory_realloc(void* buffer, size_t length) {
	void* buf = realloc(buffer, length);
	if (buf == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return buf;
}
