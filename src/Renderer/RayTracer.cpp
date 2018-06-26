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

		float distance = Vec3::distance(ray.origin, rh.point);
		Color absorbed = distance > ERR * 100 ? (-ray.absorb * distance).exp() : Color(1, 1, 1);
		absorbed = absorbed.clamp(0, 1);

		// Phong & Cook-Torrance
		if (mat->type == BLINNPHONG || mat->type == COOKTORRANCE) {
			Color color = mat->type == BLINNPHONG ?
				BlinnPhong().getColor(tree, scene, ray, rh) :
				CookTorrance().getColor(tree, scene, ray, rh);
			if (mat->reflectivity > 0) {
				Vec3 bias = 100 * ERR * rh.normal;
				Vec3 reflected = reflect(ray.direction, rh.normal);
				Ray reflectedRay(rh.point + bias, reflected, ray.refraction, ray.absorb);
				color = color * (1 - mat->reflectivity) + 
					getColor(reflectedRay, x, y, depth - 1) * mat->reflectivity;
			}
			return absorbed * (mat->ke + color);
		}

		float cosi = Vec3::dot(ray.direction, rh.normal);
		bool outside = cosi < 0;
		Vec3 bias = 100 * ERR * rh.normal;
		Point3 reflectedRayOrig = outside ? rh.point + bias : rh.point - bias;

		Color color = rh.hitable->getTexture(rh.uv) * mat->kd;

		// Lambertian
		if (mat->type == LAMBERTIAN) {
			Vec3 direction = randomUnitVec3() + rh.normal;
			Ray scattered(reflectedRayOrig, direction, ray.refraction, ray.absorb);
			return absorbed * (mat->ke + color * getColor(scattered, x, y, depth - 1));
		}

		// Reflection ray
		Vec3 reflectedDir = reflect(ray.direction, rh.normal);
		reflectedDir = Vec3::normalize(reflectedDir);
		Ray reflectedRay(reflectedRayOrig, reflectedDir, ray.refraction, ray.absorb);

		// Metal
		if (mat->type == METAL) {
			reflectedRay.direction += randomUnitVec3(mat->fuzz);
			return absorbed * (mat->ke + color * getColor(reflectedRay, x, y, depth - 1));
		}

		// Dielectric
		float cost = 0;
		float refr1 = ray.refraction;
		float refr2 = mat->refraction;
		if (outside)
			cosi = -cosi;
		else
			std::swap(refr1, refr2); 
		float refr = refr1 / refr2;

		float k = 1 - refr * refr * (1 - cosi * cosi);

		float fr = 1;
		if (k > 0) {
			cost = sqrtf(k);
			float Rs = ((refr2 * cosi) - (refr1 * cost)) / ((refr2 * cosi) + (refr1 * cost));
			float Rp = ((refr1 * cosi) - (refr2 * cost)) / ((refr1 * cosi) + (refr2 * cost));
			fr = (Rs * Rs + Rp * Rp) / 2;
		} // Else: total internal reflection
		fr = mat->reflectivity + (1.0 - mat->reflectivity) * fr;

		Vec3 refractionColor(0, 0, 0);
		if (fr < 1 - ERR) {
			// Refraction ray
			Vec3 refractedDir = refr * ray.direction + (refr * cosi - cost) * rh.normal;
			Point3 refractedRayOrig = outside ? rh.point - bias : rh.point + bias;
			Vec3 absorb = ray.absorb + (outside ? mat->absorb : -mat->absorb);
			Ray refractedRay(refractedRayOrig, refractedDir, mat->refraction, absorb);
			refractionColor = getColor(refractedRay, x, y, depth - 1);
		}

		Vec3 reflectionColor(0, 0, 0);
		if (fr > ERR) {
			reflectionColor = getColor(reflectedRay, x, y, depth - 1);
		}

		color = color * (reflectionColor * fr + refractionColor * (1 - fr));
		return absorbed * (mat->ke + color);
	} else {
		return backgroundColor(x, y);
	}
}

RayTracer::~RayTracer() {
	delete tree;
}