#pragma once

//A 3x3 matrix indexed by [row][col]
typedef struct {
	float data[3][3];
} AU_Transform;

//A 2D vector
typedef struct {
	float x, y;
} AU_Vector;

//Return an identity transformation matrix
AU_Transform au_geom_identity();
//Apply a transform matrix to a vector
AU_Vector au_geom_transform(AU_Transform, AU_Vector);
//Concatenate two transformations together
AU_Transform au_geom_trasnform_concat(AU_Transform, AU_Transform);
//Get a translation matrix
AU_Transform au_geom_transform_translate(float x, float y);
//Get a rotation matrix (in degrees)
AU_Transform au_geom_transform_rotate(float angle);
//Get a scale matrix
AU_Transform au_geom_transform_scale(float x_scale, float y_scale);
