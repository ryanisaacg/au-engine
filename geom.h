#pragma once

#include <stdbool.h>

//A 2D vector
typedef struct {
	float x, y;
} AU_Vector;

//Add two vectors
AU_Vector au_geom_vec_add(AU_Vector, AU_Vector);
//Subtract two vectors
AU_Vector au_geom_vec_sub(AU_Vector, AU_Vector);
//Clamp a vector by component
AU_Vector au_geom_vec_cmp_clamp(AU_Vector, float lower, float higher);
//Clamp a veector by length
AU_Vector au_geom_vec_len_clamp(AU_Vector, float lower, float higher);
//Scale a vector by a scalar
AU_Vector au_geom_vec_scl(AU_Vector, float);
//Normalize a vector to a length of one
AU_Vector au_geom_vec_nor(AU_Vector);
//Set the length of a vector
AU_Vector au_geom_vec_set_len(AU_Vector, float);
//Get the squared length of a vector
float au_geom_vec_len2(AU_Vector);
//Get the length of a vector
float au_geom_vec_len(AU_Vector);
//Check if two vectors are equal
bool au_geom_vec_eq(AU_Vector, AU_Vector);


//A 3x3 matrix indexed by [row][col]
typedef struct {
	float data[3][3];
} AU_Transform;

//Return an identity transformation matrix
AU_Transform au_geom_identity();
//Apply a transform matrix to a vector
AU_Vector au_geom_transform(AU_Transform, AU_Vector);
//Concatenate two transformations together
AU_Transform au_geom_transform_concat(AU_Transform, AU_Transform);
//Get a translation matrix
AU_Transform au_geom_transform_translate(float x, float y);
//Get a rotation matrix (in degrees)
AU_Transform au_geom_transform_rotate(float angle);
//Get a scale matrix
AU_Transform au_geom_transform_scale(float x_scale, float y_scale);

//An axis aligned rectangle
typedef struct {
	float x, y, width, height;
} AU_Rectangle;

//A circle
typedef struct {
	float x, y, radius;
} AU_Circle;

bool au_geom_rect_overlaps_rect(AU_Rectangle, AU_Rectangle);
bool au_geom_rect_overlaps_circ(AU_Rectangle, AU_Circle);
bool au_geom_circ_overlaps_circ(AU_Circle, AU_Circle);
#define au_geom_circ_overlaps_rect(circ, rect) (au_geom_rect_overlaps_circ(rect, circ))
bool au_geom_rect_contains(AU_Rectangle, AU_Vector);
bool au_geom_circ_contains(AU_Circle, AU_Vector);
