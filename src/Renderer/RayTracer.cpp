#include "RayTracer.h"
#include "../Math/Util.h"

void RayTracer::preprocess() {
	tree = new CubeTree(scene.hitables, treeDepth, treeSize);
}

Color RayTracer::getColor(Ray &ray, float x, float y) {
	return getColor(ray, x, y, rayDepth);
}

Color RayTracer::getColor(Ray &ray, float x, float y, int depth) {
	if (depth == 0)
		return Color(0, 0, 0);
	RayHit rh = tree->hit(ray);
	if (rh.hitable) {
		if (rh.hitable->material->type == 0) {
			// Lambertian
			Color color = reflectionModel->getColor(tree, scene, ray, rh);
			Vec3 direction = randomUnitVec3() + rh.normal;
			Ray scattered(rh.point + E * direction, direction);
			//color = color * getColor(scattered, x, y, depth - 1);
			return rh.hitable->material->ke + color;
		}

		float fuzz = rh.hitable->material->fuzz;
		Color color = rh.hitable->getTexture(rh.uv) * rh.hitable->material->kd;
		Vec3 reflected = reflect(ray.direction, rh.normal);
		Ray reflectedRay(rh.point + reflected * E, reflected + randomUnitVec3(fuzz));

		if (rh.hitable->material->type == 1) {
			// Metal
			return color * getColor(reflectedRay, x, y, depth - 1);
		}

		// Dielectric

		float ior = rh.hitable->material->refraction;
		// compute fresnel
		float kr = fresnel(ray.direction, rh.normal, ior);
		bool outside = Vec3::Dot(ray.direction, rh.normal) < 0;
		Vec3 bias = E * rh.normal;

		Vec3 refractionColor(0, 0, 0);
		// compute refraction if it is not a case of total internal reflection
		if (kr < 1) {
			Vec3 refractionDir = refract(ray.direction, rh.normal, ior);
			refractionDir = Vec3::Normalize(refractionDir);

			Vec3 refractionRayOrig = outside ? rh.point - bias : rh.point + bias;
			Ray refractionRay(refractionRayOrig, refractionDir);
			refractionColor = getColor(refractionRay, x, y, depth - 1);
		}

		Vec3 reflectionDir = reflect(ray.direction, rh.normal);
		reflectionDir = Vec3::Normalize(reflectionDir);

		Vec3 reflectionRayOrig = outside ? rh.point + bias : rh.point - bias;
		Ray reflectionRay(reflectionRayOrig, reflectionDir);
		Vec3 reflectionColor = getColor(reflectionRay, x, y, depth - 1);

		// mix the two
		return color * (reflectionColor * kr + refractionColor * (1 - kr));

	} else {
		return backgroundColor(x, y);
	}



}

RayTracer::~RayTracer() {
	delete tree;
	delete reflectionModel;
}