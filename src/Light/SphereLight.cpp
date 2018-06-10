#include "SphereLight.h"
#include "../Math/Util.h"

LightHit SphereLight::hit(Ray& ray, RayHit& rh) {
	LightHit lh(ray, rh);
	Vec3 v = origin - rh.point;
	lh.direction = v + randomUnitVec3(size);
	lh.direction = Vec3::normalize(lh.direction);
	lh.color = color;
	return lh;
}