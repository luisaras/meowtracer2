#include "PathTracer.h"
#include "../Reflection/CookTorrance.h"
#include "../Math/Util.h"

inline Ray sampleRay(Ray& ray, RayHit& rh, float& cosi, Vec3& dir) {
	cosi = Vec3::dot(dir, rh.normal);
	Vec3 bias = ERR * rh.normal;
	Point3 orig = cosi > 0 ? rh.point + bias : rh.point - bias;
	return Ray(orig, dir, ray.refraction, ray.absorb);
}

inline Ray sampleRay(Ray& ray, RayHit &rh, float& cosi) {
	// TODO: importance sampling
	Vec3 dir = randomUnitVec3();
	return sampleRay(ray, rh, cosi, dir);
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

	Color light, texture = rh.hitable->getTexture(rh.uv) * mat->kd;
	float cosi;

// ============================================================================
//  Lambertian (random reflection)
// ============================================================================
	if (mat->type == LAMBERTIAN) {
		Vec3 direction = rh.normal + randomUnitVec3() * mat->roughness;
		Ray scattered = sampleRay(ray, rh, cosi, direction);
		light = getColor(scattered, x, y, depth - 1);
		light *= PI2; // Sphere surface
	}

// ============================================================================
//  Metal (mirror reflection)
// ============================================================================
	else if (mat->type == METAL) {
		Vec3 reflectDir = ray.direction - 
			2 * Vec3::dot(ray.direction, rh.normal) * rh.normal;
		reflectDir += randomUnitVec3() * mat->roughness;
		Ray scattered = sampleRay(ray, rh, cosi, reflectDir);
		light = getColor(scattered, x, y, depth - 1);
	}

// ============================================================================
//  Dielectric
// ============================================================================
	else { 
		Vec3 direction = rh.normal + randomUnitVec3();
		Ray sample = sampleRay(ray, rh, cosi, direction);
		if (cosi > 0) {
			CookTorrance ct;
			Vec3 view = -ray.direction;

			Color fr = ct.reflectedColor(rh.normal, view, sample.direction, mat);
			Color ft = Color(0, 0, 0); // TODO: refraction

			Color fs = (fr + ft);
			light = getColor(sample, x, y, depth - 1) * fs;
			light *= PI2; // Sphere surface
		} else {
			light = Color(0, 0, 0);
		}
	}

	return absorbed * (mat->ke + texture * light * abs(cosi));
}