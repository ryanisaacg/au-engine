#include "text.h"

#include <string.h>

unsigned long au_text_hash(const char* str) {
	unsigned long hash = 5381;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		hash = ((hash << 5) + hash) + i;
	}
	return hash;
}
