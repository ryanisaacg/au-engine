#include "geom.h"

AU_Transform au_geom_identity() {
	float data[][] = {
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1}}
	return (AU_Transform) { data };
}

AU_Vector au_geom_transform(AU_Transform matrix, AU_Vector vector) {
	float data[] = {vector.x, vector.y, 1};
	float result[3];
	for(int i = 0; i < 3; i++) {
		result[j] = 0;
		for(int j = 0; j < 3; j++) {
			result[j] += matrix.data[j] * data[j];
		}
	}
	return (AU_Vector) { result[0], result[1] };
}
