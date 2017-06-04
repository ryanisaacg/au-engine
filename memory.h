#pragma once

#include <stddef.h>

void* au_memory_alloc(size_t length);
void* au_memory_calloc(size_t num, size_t length);
void* au_memory_realloc(void* buffer, size_t length);
