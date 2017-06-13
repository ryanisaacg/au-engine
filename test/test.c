#include "au.h"

#include <stdio.h>

#define ASSERT(cond, name) if(!(cond)) { fprintf(stderr, "Test " name " has failed\n"); }

int main() {
	ASSERT(au_util_floateq(1.5f, 1.5f), "floating point eq 1");
	ASSERT(!au_util_floateq(0.5f, 0.6f), "floating point eq 2");
	AU_Transform trans = au_geom_identity();
	AU_Vector v1 = { 1, 3 };
	AU_Vector v2 = { 15, 1.5 };
	AU_Vector v3 = { 0, 0 };
	ASSERT(au_geom_vec_eq(v1, au_geom_transform(trans, v1)), "identity 1");
	ASSERT(au_geom_vec_eq(v2, au_geom_transform(trans, v2)), "identity 2");
	ASSERT(au_geom_vec_eq(v3, au_geom_transform(trans, v3)), "identity 3");
	printf("All tests run.\n");
}
