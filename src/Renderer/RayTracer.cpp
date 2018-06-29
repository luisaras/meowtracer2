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

		Color texture = rh.hitable->getTexture(rh.uv) * mat->kd;

// ============================================================================
//  Blinn-Phong
// ============================================================================
		if (mat->type == BLINNPHONG) {
			BlinnPhong bp;
			Color color(0, 0, 0);
			for (uint i = 0; i < scene.lights.size(); i++) {
				LightHit lh = scene.lights[i]->hit(ray, rh);
				if (!tree->hitsLight(scene.lights[i], lh)) {
					color += bp.diffuseColor(scene.lights[i], lh, texture);
					color += bp.specularColor(scene.lights[i], lh);
				}
			}
			color = color + rh.hitable->material->ka * scene.ambientColor;
			if (mat->reflectivity > 0) {
				Vec3 bias = 100 * ERR * rh.normal;
				Vec3 reflected = ray.direction - 2 * Vec3::dot(ray.direction, rh.normal) * rh.normal;
				Ray reflectedRay(rh.point + bias, reflected, ray.refraction, ray.absorb);
				color = color * (1 - mat->reflectivity) + 
					getColor(reflectedRay, x, y, depth - 1) * mat->reflectivity;
			}
			return absorbed * (mat->ke + color);
		}

// ============================================================================
//  Cook-Torrance
// ============================================================================
		// Bias
		float cosi = Vec3::dot(ray.direction, rh.normal);
		bool outside = cosi < 0;
		Vec3 bias = 100 * ERR * rh.normal;
		Point3 reflectedRayOrig = outside ? rh.point + bias : rh.point - bias;

		if (mat->type == COOKTORRANCE) {
			CookTorrance ct;
			Color color(0, 0, 0);
			for (uint i = 0; i < scene.lights.size(); i++) {
				LightHit lh = scene.lights[i]->hit(ray, rh);
				if (!tree->hitsLight(scene.lights[i], lh)) {
					color += ct.lightColor(scene.lights[i], lh, texture);
				}
			}
			color = color + rh.hitable->material->ka * scene.ambientColor;
			return absorbed * (mat->ke + color);
		}

// ============================================================================
//  Lambertian
// ============================================================================
		if (mat->type == LAMBERTIAN) {
			Vec3 direction = randomUnitVec3() + rh.normal;
			Ray scattered(reflectedRayOrig, direction, ray.refraction, ray.absorb);
			return absorbed * (mat->ke + texture * getColor(scattered, x, y, depth - 1));
		}

// ============================================================================
//  Metal
// ============================================================================
		// Reflection ray
		Vec3 reflectedDir = ray.direction - 2 * cosi * rh.normal;
		reflectedDir = Vec3::normalize(reflectedDir);
		Ray reflectedRay(reflectedRayOrig, reflectedDir, ray.refraction, ray.absorb);

		if (mat->type == METAL) {
			reflectedRay.direction += randomUnitVec3() * mat->fuzz;
			return absorbed * (mat->ke + texture * getColor(reflectedRay, x, y, depth - 1));
		}

// ============================================================================
//  Dielectric
// ============================================================================
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

		Color color = texture * (reflectionColor * fr + refractionColor * (1 - fr));
		return absorbed * (mat->ke + color);

// ============================================================================	
	} else {
		return backgroundColor(x, y);
	}
}

RayTracer::~RayTracer() {
	delete tree;
}