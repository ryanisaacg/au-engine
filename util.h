#pragma once


float au_util_clamp(float n, float lower, float higher) {
	return n <= lower ? lower : n >= upper ? upper : n;
}
