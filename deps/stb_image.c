#include "memory.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MALLOC(a) au_memory_alloc(a)
#define STBI_REALLOC(a, b) au_memory_realloc(a, b)
#define STBI_FREE(a) free(a);

#include "stb_image.h"


