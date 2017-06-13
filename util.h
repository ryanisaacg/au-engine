#pragma once

#include <math.h>
#include <stdlib.h>

static inline float au_util_clamp(float n, float lower, float upper) {
	return n <= lower ? lower : n >= upper ? upper : n;
}

static inline bool au_util_floateq(float a, float b) {
	return fabs(a - b) < 0.00001;
}

//A normalized random function
static inline float au_util_rand() {
	return (float) rand() / RAND_MAX;
}
static inline float au_util_randf_range(float min, float max) {
	return (max - min) * au_util_rand() + min;
}
static inline int au_util_randi_range(int min, int max) {
	return (int)au_util_randf_range(min, max);
}

