#pragma once


static inline float au_util_clamp(float n, float lower, float upper) {
	return n <= lower ? lower : n >= upper ? upper : n;
}
