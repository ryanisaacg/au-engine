#include "geom.h"

#include <math.h>

AU_Transform au_geom_identity() {
	return (AU_Transform) {{
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1}}};
}

AU_Vector au_geom_transform(AU_Transform matrix, AU_Vector vector) {
	float data[3] = {vector.x, vector.y, 1};
	float result[3];
	for(int i = 0; i < 3; i++) {
		result[i] = 0;
		for(int j = 0; j < 3; j++) {
			result[i] += matrix.data[i][j] * data[j];
		}
	}
	return (AU_Vector) { result[0], result[1] };
}

AU_Transform au_geom_trasnform_concat(AU_Transform a, AU_Transform b) {
	AU_Transform c = a;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			c.data[i][j] = a.data[i][j] * b.data[j][i];
		}
	}
	return c;
}

AU_Transform au_geom_transform_translate(float x, float y) {
	return (AU_Transform) {{
		{1, 0, x},
		{0, 1, y},
		{0, 0, 1}}};
}

AU_Transform au_geom_transform_rotate(float angle) {
	float c = cos(angle * 180.0f / M_PI);
	float s = sin(angle * 180.0f / M_PI);
	return (AU_Transform) {{
		{c, -s, 0},
		{s, c, 0},
		{0, 0, 1}}};
}

AU_Transform au_geom_transform_scale(float x_scale, float y_scale) {
	return (AU_Transform) {{
		{x_scale, 0, 0},
		{0, y_scale, 0},
		{0, 0, 1}}};
}