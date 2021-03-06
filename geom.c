#include "geom.h"

#include <math.h>

#include "util.h"

AU_Vector au_geom_vec_add(AU_Vector a, AU_Vector b) {
	return (AU_Vector) {
		a.x + b.x, a.y + b.y
	};
}

AU_Vector au_geom_vec_sub(AU_Vector a, AU_Vector b) {
	return (AU_Vector) {
		a.x - b.x, a.y - b.y
	};
}

AU_Vector au_geom_vec_cmp_clamp(AU_Vector vec, float lower, float higher) {
	return (AU_Vector) {
		au_util_clamp(vec.x, lower, higher), au_util_clamp(vec.y, lower, higher)
	};
}

AU_Vector au_geom_vec_len_clamp(AU_Vector vec, float lower, float higher) {
	return au_geom_vec_set_len(vec, au_util_clamp(au_geom_vec_len(vec), lower, higher));
}

AU_Vector au_geom_vec_scl(AU_Vector vec, float scl) {
	return (AU_Vector) {
		vec.x* scl, vec.y* scl
	};
}

AU_Vector au_geom_vec_nor(AU_Vector vec) {
	float len = au_geom_vec_len(vec);
	return (AU_Vector) {
		vec.x / len, vec.y / len
	};
}

AU_Vector au_geom_vec_set_len(AU_Vector vec, float len) {
	return au_geom_vec_scl(au_geom_vec_nor(vec), len);
}

float au_geom_vec_len2(AU_Vector vec) {
	return vec.x * vec.x + vec.y * vec.y;
}

float au_geom_vec_len(AU_Vector vec) {
	return sqrt(au_geom_vec_len2(vec));
}

bool au_geom_vec_eq(AU_Vector a, AU_Vector b) {
	return au_util_floateq(a.x, b.x) && au_util_floateq(a.y, b.y);
}

AU_Vector au_geom_vec_rand(AU_Vector min, AU_Vector max) {
	return (AU_Vector) {
		au_util_randf_range(min.x, max.x), au_util_randf_range(min.y, max.y)
	};
}

float au_geom_vec_dot(AU_Vector a, AU_Vector b) {
	return a.x * b.x + a.y * b.y;
}

AU_Vector au_geom_vec_rot_abt(AU_Vector v, AU_Vector origin, float angle) {
	float c = cos(angle * M_PI / 180);
	float s = sin(angle * M_PI / 180);
	AU_Vector translated = { v.x - origin.x, v.y - origin.y };
	AU_Vector rotated = { translated.x * c - translated.y * s, translated.y * c - translated.x * s };
	return au_geom_vec_add(rotated, origin);
}

AU_Transform au_geom_identity() {
	return (AU_Transform) {
		{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1}
		}
	};
}

AU_Vector au_geom_transform(AU_Transform matrix, AU_Vector vector) {
	float data[3] = {vector.x, vector.y, 1};
	float result[3];
	for (int i = 0; i < 3; i++) {
		result[i] = 0;
		for (int j = 0; j < 3; j++) {
			result[i] += matrix.data[i][j] * data[j];
		}
	}
	return (AU_Vector) {
		result[0], result[1]
	};
}

AU_Transform au_geom_transform_concat(AU_Transform a, AU_Transform b) {
	AU_Transform c = {
		{
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0}
		}
	};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				c.data[i][j] += a.data[i][k] * b.data[k][j];
			}
		}
	}
	return c;
}

AU_Transform au_geom_transform_translate(float x, float y) {
	return (AU_Transform) {
		{
			{1, 0, x},
			{0, 1, y},
			{0, 0, 1}
		}
	};
}

AU_Transform au_geom_transform_rotate(float angle) {
	float c = cos(angle * M_PI / 180);
	float s = sin(angle * M_PI / 180);
	return (AU_Transform) {
		{
			{c, -s, 0},
			{s, c, 0},
			{0, 0, 1}
		}
	};
}

AU_Transform au_geom_transform_scale(float x_scale, float y_scale) {
	return (AU_Transform) {
		{
			{x_scale, 0, 0},
			{0, y_scale, 0},
			{0, 0, 1}
		}
	};
}

bool au_geom_rect_overlaps_rect(AU_Rectangle a, AU_Rectangle b) {
	return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}

bool au_geom_circ_overlaps_circ(AU_Circle a, AU_Circle b) {
	float x = a.x - b.x;
	float y = a.y - b.y;
	float rad = a.radius + b.radius;
	return x * x + y * y < rad * rad;
}

bool au_geom_rect_overlaps_circ(AU_Rectangle r, AU_Circle c) {
	AU_Vector closest;
	if (c.x < r.x) {
		closest.x = r.x;
	} else if (c.x > r.x + r.width) {
		closest.x = r.x + r.width;
	} else {
		closest.x = c.x;
	}
	if (c.y < r.y) {
		closest.y = r.y;
	} else if (c.y > r.y + r.height) {
		closest.y = r.y + r.height;
	} else {
		closest.y = c.y;
	}
	closest.x -= c.x;
	closest.y -= c.y;
	return (closest.x * closest.x) + (closest.y * closest.y) < c.radius * c.radius;
}

bool au_geom_rect_contains(AU_Rectangle r, AU_Vector v) {
	return v.x >= r.x && v.y >= r.y && v.x < r.x + r.width && v.y < r.y + r.height;
}

bool au_geom_circ_contains(AU_Circle c, AU_Vector v) {
	AU_Vector dist = { v.x - c.x, v.y - c.y };
	return au_geom_vec_len2(dist) < c.radius * c.radius;
}
