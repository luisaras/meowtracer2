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

		float distance = Vec3::distance2(ray.origin, rh.point);
		Color absorbed = (-ray.absorb * 10 * distance).exp();

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

		bool outside = Vec3::dot(ray.direction, rh.normal) < 0;
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
		float refr = mat->refraction / ray.refraction;
		float cost = 0, cosi = clamp(Vec3::dot(ray.direction, rh.normal), -1, 1);
		float etai = 1, etat = refr;
		if (cosi < 0)
			cosi = -cosi;
		else
			std::swap(etai, etat); 
		float eta = etai / etat;
		float sint = eta * std::max(0.f, 1 - cosi * cosi);
		float k = 1 - eta * sint;

		float fr = 1;
		if (k > 0) {
			cost = sqrtf(k);
			float acosi = fabsf(cosi);
			float Rs = ((etat * acosi) - (etai * cost)) / ((etat * acosi) + (etai * cost));
			float Rp = ((etai * acosi) - (etat * cost)) / ((etai * acosi) + (etat * cost));
			fr = (Rs * Rs + Rp * Rp) / 2;
		} // Else: total internal reflection
		fr = mat->reflectivity + (1.0 - mat->reflectivity) * fr;

		Vec3 refractionColor(0, 0, 0);
		if (fr < 1 - ERR) {
			// Refraction ray
			if (k > 0) {
				Vec3 refractedDir = eta * ray.direction + (eta * cosi - cost) * rh.normal; 
				Point3 refractedRayOrig = outside ? rh.point - bias : rh.point + bias;
				Vec3 absorb = ray.absorb + (outside ? mat->absorb : -mat->absorb);
				Ray refractedRay(refractedRayOrig, refractedDir, mat->refraction, absorb);
				refractionColor = getColor(refractedRay, x, y, depth - 1);
			}
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