#include "SphereLight.h"
#include "../Math/Util.h"

LightHit SphereLight::hit(Ray& ray, RayHit& rh) {
	LightHit lh(ray, rh);
	Vec3 v = origin - rh.point;
	Vec3 dir = v + randomUnitVec3() * size;
	lh.distance = dir.length();
	lh.direction = Vec3::normalize(dir);
	return lh;
}