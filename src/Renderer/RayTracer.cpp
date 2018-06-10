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
				Ray reflectedRay(rh.point + bias, reflected, ray.refraction);
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
			Ray scattered(rh.point + bias, direction, ray.refraction);
			return mat->ke + color * getColor(scattered, x, y, depth - 1);
		}

		Vec3 reflectionDir = reflect(ray.direction, rh.normal);
		reflectionDir = Vec3::normalize(reflectionDir);
		Ray reflectedRay(rh.point, reflectionDir, ray.refraction);

		// Metal
		if (mat->type == 2) {
			reflectedRay.direction += randomUnitVec3(mat->fuzz);
			reflectedRay.origin += bias;
			return color * getColor(reflectedRay, x, y, depth - 1);
		}

		// Dielectric
		float refr = mat->refraction / ray.refraction;
		float fr = fresnel(ray.direction, rh.normal, refr);
		bool outside = Vec3::dot(ray.direction, rh.normal) < 0;
		Vec3 refractionColor(0, 0, 0);
		if (fr < 1) {
			Vec3 refractedDir = refract(ray.direction, rh.normal, refr);
			refractedDir = Vec3::normalize(refractedDir);
			Vec3 refractedRayOrig = outside ? rh.point - bias : rh.point + bias;
			Ray refractedRay(refractedRayOrig, refractedDir, mat->refraction);
			refractionColor = getColor(refractedRay, x, y, depth - 1);
		}

		outside ? reflectedRay.origin += bias : reflectedRay.origin -= bias;
		Vec3 reflectionColor = getColor(reflectedRay, x, y, depth - 1);

		return color * (reflectionColor * fr + refractionColor * (1 - fr));
	} else {
		return backgroundColor(x, y);
	}

}

RayTracer::~RayTracer() {
	delete tree;
	delete reflectionModel;
}