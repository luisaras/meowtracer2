#include "Util.h"

Vec2 randomVec2() {
	return Vec2(rand() / RAND_MAX, rand() / RAND_MAX);
}

Vec3 randomUnitVec3(float radius) {
	float phi = PI2 * rand() / RAND_MAX;
	float cost = 1.0 * rand() / RAND_MAX;
	float sint = sqrt(1 - cost*cost);
	float x = sint * cos(phi);
	float y = sint * sin(phi);
	return Vec3(x * radius, y * radius, cost * radius);
}

Vec2 beckmannImportance(Vec2& e, float m2) {
	float tan = sqrt(-m2 * log(1 - e.x));
	float tetha = atan(tan);
	float phi = PI2 * e.y;
	return Vec2(tetha, phi);
}