#include "window.h"

#include "memory.h"

AU_Engine *AU_Init(char* title, int w, int h) {
	AU_Engine* engine = chk_alloc(sizeof(AU_Engine));
	engine->ctx = AU_Context_Init_Stack(title, w, h);
	return engine;
}
