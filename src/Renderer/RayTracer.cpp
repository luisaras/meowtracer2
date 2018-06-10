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
		Material* mat = rh.hitable->material;
		// Phong
		if (mat->type == 0) {
			Color color = reflectionModel->getColor(tree, scene, ray, rh);
			if (mat->reflexivity > 0) {
				Vec3 bias = E * rh.normal;
				Vec3 reflected = reflect(ray.direction, rh.normal);
				Ray reflectedRay(rh.point + bias, reflected);
				color = color * (1 - mat->reflexivity) + 
					getColor(reflectedRay, x, y, depth - 1) * mat->reflexivity;
			}
			return mat->ke + color;
		}

		Vec3 bias = E * rh.normal;
		Color color = rh.hitable->getTexture(rh.uv) * mat->kd;

		// Lambertian
		if (mat->type == 1) {
			Vec3 direction = randomUnitVec3() + rh.normal;
			Ray scattered(rh.point + bias, direction);
			return mat->ke + color * getColor(scattered, x, y, depth - 1);
		}

		Vec3 reflected = reflect(ray.direction, rh.normal);
		Ray reflectedRay(rh.point + bias, reflected);

		// Metal
		if (mat->type == 2) {
			reflectedRay.direction += randomUnitVec3(mat->fuzz);
			return color * getColor(reflectedRay, x, y, depth - 1);
		}

		// Dielectric
		float fr = fresnel(ray.direction, rh.normal, mat->refraction);
		bool outside = Vec3::dot(ray.direction, rh.normal) < 0;
		Vec3 refractionColor(0, 0, 0);
		if (fr < 1) {
			Vec3 refractionDir = refract(ray.direction, rh.normal, mat->refraction);
			refractionDir = Vec3::normalize(refractionDir);
			Vec3 refractionRayOrig = outside ? rh.point - bias : rh.point + bias;
			Ray refractionRay(refractionRayOrig, refractionDir);
			refractionColor = getColor(refractionRay, x, y, depth - 1);
		}

		Vec3 reflectionDir = reflect(ray.direction, rh.normal);
		reflectionDir = Vec3::normalize(reflectionDir);

		Vec3 reflectionRayOrig = outside ? rh.point + bias : rh.point - bias;
		Ray reflectionRay(reflectionRayOrig, reflectionDir);
		Vec3 reflectionColor = getColor(reflectionRay, x, y, depth - 1);

		return color * (reflectionColor * fr + refractionColor * (1 - fr));
	} else {
		return backgroundColor(x, y);
	}

}

RayTracer::~RayTracer() {
	delete tree;
	delete reflectionModel;
}