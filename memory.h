#pragma once

#include <stddef.h>

void* chk_alloc(size_t length);
void* chk_realloc(void* buffer, size_t length);
