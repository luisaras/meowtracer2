#include "PathTracer.h"
#include "../Reflection/CookTorrance.h"
#include "../Math/Util.h"

Ray sampleRay(RayHit &rh, float refr, Vec3& absorb) {
	// TODO: importance sampling
	Vec3 dir = rh.normal + randomUnitVec3();
	dir = Vec3::normalize(dir);
	float cosi = Vec3::dot(dir, rh.normal);
	Vec3 bias = ERR * rh.normal;
	Point3 orig = cosi < 0 ? rh.point + bias : rh.point - bias;
	return Ray(orig, dir, refr, absorb);
}

Color PathTracer::getColor(Ray &ray, float x, float y, int depth) {
	if (depth == 0)
		return Color(0, 0, 0);

	RayHit rh = tree->hit(ray);
	if (!rh.hitable)
		return backgroundColor(x, y);

// ============================================================================

	Material* mat = rh.hitable->material;

	float distance = Vec3::distance(ray.origin, rh.point);
	Color absorbed = (-ray.absorb * distance).exp();
	absorbed = absorbed.clamp(0, 1);

	Color texture = rh.hitable->getTexture(rh.uv) * mat->kd;

	float cosi = Vec3::dot(ray.direction, rh.normal);
	bool outside = cosi < 0;
	Vec3 bias = ERR * rh.normal;
	Point3 reflectPoint = outside ? rh.point + bias : rh.point - bias;

// ============================================================================
//  Lambertian (random reflection)
// ============================================================================
	if (mat->type == LAMBERTIAN) {
		Vec3 direction = rh.normal + randomUnitVec3() * mat->fuzz;
		Ray scattered(reflectPoint, direction, ray.refraction, ray.absorb);
		Color color = getColor(scattered, x, y, depth - 1);
		return absorbed * (mat->ke + texture * color);
	}

// ============================================================================
//  Metal (mirror reflection)
// ============================================================================
	if (mat->type == METAL) {
		Vec3 reflectDir = ray.direction - 2 * cosi * rh.normal;
		reflectDir += randomUnitVec3() * mat->fuzz;
		Ray reflectedRay(reflectPoint, reflectDir, ray.refraction, ray.absorb);
		Color color = getColor(reflectedRay, x, y, depth - 1);
		return absorbed * (mat->ke + texture * color);
	}

// ============================================================================
//  Dielectric
// ============================================================================
	// TODO:
	Ray sample = sampleRay(rh, ray.refraction, ray.absorb);

	Vec3 rayDir = -ray.direction;

	CookTorrance ct;
	Color fr = ct.reflectedColor(rh.normal, rayDir, sample.direction, mat);
	Color ft = Color(0, 0, 0); // TODO

	Color fs = (fr + ft) * PI2;
	Color color = getColor(sample, x, y, depth - 1) * texture * fs;
	return absorbed * (mat->ke + color);
}