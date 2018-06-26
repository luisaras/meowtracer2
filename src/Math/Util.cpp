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

Vec3 reflect(const Vec3 &dir, const Vec3 &normal) {
	return dir - 2 * Vec3::dot(dir, normal) * normal;
}

Vec3 refract(const Vec3 &ray, const Vec3 &normal, float ior) {
	float cosi = clamp(Vec3::dot(ray, normal), -1, 1);
	float etai = 1, etat = ior;
	Vec3 n = normal;
	if (cosi < 0)
		cosi = -cosi;
	else  {
		std::swap(etai, etat); 
		n = -normal;
	}
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0 ? Vec3(0, 0, 0) : eta * ray + (eta * cosi - sqrtf(k)) * n; 
}

float fresnel(const Vec3 &ray, const Vec3 &normal, float ior) {
	float kr;
	float cosi = clamp(Vec3::dot(ray, normal), -1, 1);
	float etai = 1, etat = ior;
	if (cosi > 0)
		std::swap(etai, etat);
	// Compute sini using Snell's law
	float eta = etai / etat;
	float sint = eta * std::max(0.f, 1 - cosi * cosi);
	// Total internal reflection
	if (sint >= 1)
		return 1;
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * eta));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
	return kr;
} 

float schlick(float cos, float ref) {
    float r0 = (1 - ref) / (1 + ref);
    float r02 = r0*r0;
    return r02 + (1 - r02) * pow((1 - cos), 5);
}