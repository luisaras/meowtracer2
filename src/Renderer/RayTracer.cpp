#include "RayTracer.h"
#include "../Reflection/BlinnPhong.h"
#include "../Reflection/CookTorrance.h"
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
		if (mat->type == BLINNPHONG || mat->type == COOKTORRANCE) {
			Color color = mat->type == BLINNPHONG ?
				BlinnPhong().getColor(tree, scene, ray, rh) :
				CookTorrance().getColor(tree, scene, ray, rh);
			if (mat->reflectivity > 0) {
				Vec3 bias = ERR * rh.normal;
				Vec3 reflected = reflect(ray.direction, rh.normal);
				Ray reflectedRay(rh.point + bias, reflected, ray.refraction);
				color = color * (1 - mat->reflectivity) + 
					getColor(reflectedRay, x, y, depth - 1) * mat->reflectivity;
			}
			return mat->ke + color;
		}

		Vec3 bias = ERR * rh.normal;
		Color color = rh.hitable->getTexture(rh.uv) * mat->kd;

		// Lambertian
		if (mat->type == LAMBERTIAN) {
			Vec3 direction = randomUnitVec3() + rh.normal;
			Ray scattered(rh.point + bias, direction, ray.refraction);
			return mat->ke + color * getColor(scattered, x, y, depth - 1);
		}

		Vec3 reflectionDir = reflect(ray.direction, rh.normal);
		reflectionDir = Vec3::normalize(reflectionDir);
		Ray reflectedRay(rh.point, reflectionDir, ray.refraction);

		// Metal
		if (mat->type == METAL) {
			reflectedRay.direction += randomUnitVec3(mat->fuzz);
			reflectedRay.origin += bias;
			return color * getColor(reflectedRay, x, y, depth - 1);
		}

		// Dielectric
		float refr = mat->refraction / ray.refraction;
		float fr = fresnel(ray.direction, rh.normal, refr);
		fr = mat->reflectivity + ( 1.0 - mat->reflectivity) * fr;
		bool outside = Vec3::dot(ray.direction, rh.normal) < 0;

		Vec3 refractionColor(0, 0, 0);
		if (fr < 1 - ERR) {
			Vec3 refractedDir = refract(ray.direction, rh.normal, refr);
			refractedDir = Vec3::normalize(refractedDir);
			Vec3 refractedRayOrig = outside ? rh.point - bias : rh.point + bias;
			Ray refractedRay(refractedRayOrig, refractedDir, mat->refraction);
			refractionColor = getColor(refractedRay, x, y, depth - 1);
			// Beer's Law
			if (mat->type == BEERS && !outside) {
				//RayHit rh = tree->hit(refractedRay);
				float distance = Vec3::distance(ray.origin, rh.point);
				Vec3 absorb = (-mat->absorb * distance).exp();
				color = color * absorb;
			}
		}
		Vec3 reflectionColor(0, 0, 0);
		if (fr > ERR) {
			outside ? reflectedRay.origin += bias : reflectedRay.origin -= bias;
			reflectionColor = getColor(reflectedRay, x, y, depth - 1);
		}

		return color * (reflectionColor * fr + refractionColor * (1 - fr));
	} else {
		return backgroundColor(x, y);
	}

}

RayTracer::~RayTracer() {
	delete tree;
}