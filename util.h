#pragma once

#include <math.h>

static inline float au_util_clamp(float n, float lower, float upper) {
	return n <= lower ? lower : n >= upper ? upper : n;
}

static inline bool au_util_floateq(float a, float b) {
	return fabs(a - b) < 0.00001;
}
