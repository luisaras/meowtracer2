#include "Util.h"

float clamp(float x, float min, float max) {
	return fmin(max, fmax(min, x));
}

Color clampColor(Color& color) {
	return Color(clamp(color.x, 0, 1), clamp(color.y, 0, 1), clamp(color.z, 0, 1));
}

Vec2 randomVec2() {
	return Vec2(rand() / RAND_MAX, rand() / RAND_MAX);
}

Vec3 angle2Vec3(float cost, float phi) {
	float sint = sqrt(1 - cost * cost);
	float x = sint * cos(phi);
	float y = sint * sin(phi);
	return Vec3(x, y, cost);
}

Vec3 randomUnitVec3() {
	float phi = PI2 * rand() / RAND_MAX;
	float cost = 1.0 * rand() / RAND_MAX;
	return angle2Vec3(cost, phi);
}

Vec3 GGX(const Vec2 &e, float r) {
	float tetha = atan(r * sqrt(e.x) / sqrt(1 - e.x));
	float phi = PI2 * e.y;
	return angle2Vec3(cos(tetha), phi);
}

Vec3 GGX(float r) {
	Vec2 e = randomVec2();
	return GGX(e, r);
}