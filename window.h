#pragma once

#include "context.h"

typedef struct {
	AU_Context ctx;
} AU_Engine;

//Intialize the engine with a window title, width, and height
AU_Engine *AU_Init(char*, int, int);



